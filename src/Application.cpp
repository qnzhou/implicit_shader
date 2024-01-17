/**
 * This file is part of the "Learn WebGPU for C++" book.
 *   https://github.com/eliemichel/LearnWebGPU
 *
 * MIT License
 * Copyright (c) 2022-2023 Elie Michel
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <implicit_shader/Application.h>

#include <array>
#include <cassert>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <webgpu/webgpu.hpp>

#include "ResourceManager.h"
#include "webgpu-release.h"

using namespace wgpu;

namespace implicit_shader {

bool Application::onInit(const std::string& shader_path, size_t num_points) {
    m_num_points = num_points;
    if (!initDevice()) return false;
    initBindGroupLayout();
    initComputePipeline(shader_path);
    initBuffers();
    initBindGroup();
    return true;
}

void Application::onFinish() {
    terminateBindGroup();
    terminateBuffers();
    terminateComputePipeline();
    terminateBindGroupLayout();
    terminateDevice();
}

bool Application::initDevice() {
    // Create instance
    m_instance = createInstance(InstanceDescriptor{});
    if (!m_instance) {
        std::cerr << "Could not initialize WebGPU!" << std::endl;
        return false;
    }

    // Create surface and adapter
    std::cout << "Requesting adapter..." << std::endl;
    RequestAdapterOptions adapterOpts{};
    adapterOpts.compatibleSurface = nullptr;
    Adapter adapter = m_instance.requestAdapter(adapterOpts);
    std::cout << "Got adapter: " << adapter << std::endl;

    std::cout << "Requesting device..." << std::endl;
    SupportedLimits supportedLimits;
    adapter.getLimits(&supportedLimits);
    RequiredLimits requiredLimits = Default;
    requiredLimits.limits.maxVertexAttributes = 6;
    requiredLimits.limits.maxVertexBuffers = 1;
    requiredLimits.limits.maxBindGroups = 2;
    requiredLimits.limits.maxUniformBuffersPerShaderStage = 2;
    requiredLimits.limits.maxUniformBufferBindingSize = 16 * 4 * sizeof(float);
    requiredLimits.limits.minStorageBufferOffsetAlignment =
        supportedLimits.limits.minStorageBufferOffsetAlignment;
    requiredLimits.limits.maxBufferSize = m_num_points * 4 * sizeof(float);
    requiredLimits.limits.maxTextureDimension1D = 2048;
    requiredLimits.limits.maxTextureDimension2D = 2048;
    requiredLimits.limits.maxTextureDimension3D = 2048;
    requiredLimits.limits.maxTextureArrayLayers = 1;
    requiredLimits.limits.maxSampledTexturesPerShaderStage = 3;
    requiredLimits.limits.maxSamplersPerShaderStage = 1;
    requiredLimits.limits.maxVertexBufferArrayStride = 68;
    requiredLimits.limits.maxInterStageShaderComponents = 17;
    requiredLimits.limits.maxStorageBuffersPerShaderStage = 2;
    requiredLimits.limits.maxComputeWorkgroupSizeX = 32;
    requiredLimits.limits.maxComputeWorkgroupSizeY = 1;
    requiredLimits.limits.maxComputeWorkgroupSizeZ = 1;
    requiredLimits.limits.maxComputeInvocationsPerWorkgroup = 32;
    requiredLimits.limits.maxComputeWorkgroupsPerDimension = 2;
    requiredLimits.limits.maxStorageBufferBindingSize =
        m_num_points * 4 * sizeof(float);
    requiredLimits.limits.minUniformBufferOffsetAlignment = 256;

    // Create device
    DeviceDescriptor deviceDesc{};
    deviceDesc.label = "My Device";
    deviceDesc.requiredFeatureCount = 0;
    deviceDesc.requiredLimits = &requiredLimits;
    deviceDesc.defaultQueue.label = "The default queue";
    m_device = adapter.requestDevice(deviceDesc);
    std::cout << "Got device: " << m_device << std::endl;

    // Add an error callback for more debug info
    m_uncapturedErrorCallback = m_device.setUncapturedErrorCallback(
        [](ErrorType type, char const* message) {
            std::cout << "Device error: type " << type;
            if (message) std::cout << " (message: " << message << ")";
            std::cout << std::endl;
        });

    m_deviceLostCallback = m_device.setDeviceLostCallback(
        [](DeviceLostReason reason, char const* message) {
            std::cout << "Device error: reason " << reason;
            if (message) std::cout << " (message: " << message << ")";
            std::cout << std::endl;
        });

#ifdef WEBGPU_BACKEND_WGPU
    m_device.getQueue().submit(0, nullptr);
#else
    m_instance.processEvents();
#endif

    return true;
}

void Application::terminateDevice() {
    wgpuDeviceRelease(m_device);
    wgpuInstanceRelease(m_instance);
}

void Application::initBindGroup() {
    // Create compute bind group
    std::vector<BindGroupEntry> entries(1, Default);

    // Input/output buffer
    entries[0].binding = 0;
    entries[0].buffer = m_inputBuffer;
    entries[0].offset = 0;
    entries[0].size = m_num_points * 4 * sizeof(float);

    BindGroupDescriptor bindGroupDesc;
    bindGroupDesc.layout = m_bindGroupLayout;
    bindGroupDesc.entryCount = (uint32_t)entries.size();
    bindGroupDesc.entries = (WGPUBindGroupEntry*)entries.data();
    m_bindGroup = m_device.createBindGroup(bindGroupDesc);
}

void Application::terminateBindGroup() { wgpuBindGroupRelease(m_bindGroup); }

void Application::initBindGroupLayout() {
    // Create bind group layout
    std::vector<BindGroupLayoutEntry> bindings(1, Default);

    // Input/output buffer
    bindings[0].binding = 0;
    bindings[0].buffer.type = BufferBindingType::Storage;
    bindings[0].visibility = ShaderStage::Compute;

    BindGroupLayoutDescriptor bindGroupLayoutDesc;
    bindGroupLayoutDesc.entryCount = (uint32_t)bindings.size();
    bindGroupLayoutDesc.entries = bindings.data();
    m_bindGroupLayout = m_device.createBindGroupLayout(bindGroupLayoutDesc);
}

void Application::terminateBindGroupLayout() {
    wgpuBindGroupLayoutRelease(m_bindGroupLayout);
}

void Application::initComputePipeline(const std::string& shader_path) {
    // Load compute shader
    ShaderModule computeShaderModule =
        ResourceManager::loadShaderModule(shader_path, m_device);

    // Create compute pipeline layout
    PipelineLayoutDescriptor pipelineLayoutDesc;
    pipelineLayoutDesc.bindGroupLayoutCount = 1;
    pipelineLayoutDesc.bindGroupLayouts =
        (WGPUBindGroupLayout*)&m_bindGroupLayout;
    m_pipelineLayout = m_device.createPipelineLayout(pipelineLayoutDesc);

    // Create compute pipeline
    ComputePipelineDescriptor computePipelineDesc;
    computePipelineDesc.compute.constantCount = 0;
    computePipelineDesc.compute.constants = nullptr;
    computePipelineDesc.compute.entryPoint = "run";
    computePipelineDesc.compute.module = computeShaderModule;
    computePipelineDesc.layout = m_pipelineLayout;
    m_pipeline = m_device.createComputePipeline(computePipelineDesc);
}

void Application::terminateComputePipeline() {
    wgpuComputePipelineRelease(m_pipeline);
    wgpuPipelineLayoutRelease(m_pipelineLayout);
}

void Application::initBuffers() {
    // Create input/output buffers
    BufferDescriptor bufferDesc;
    bufferDesc.mappedAtCreation = false;
    bufferDesc.size = m_num_points * 4 * sizeof(float);

    bufferDesc.usage =
        BufferUsage::Storage | BufferUsage::CopyDst | BufferUsage::CopySrc;
    m_inputBuffer = m_device.createBuffer(bufferDesc);

    // Create an intermediary buffer to which we copy the output and that can be
    // used for reading into the CPU memory.
    bufferDesc.usage = BufferUsage::CopyDst | BufferUsage::MapRead;
    m_mapBuffer = m_device.createBuffer(bufferDesc);
}

void Application::terminateBuffers() {
    m_inputBuffer.destroy();
    wgpuBufferRelease(m_inputBuffer);

    m_mapBuffer.destroy();
    wgpuBufferRelease(m_mapBuffer);
}

void Application::onCompute(std::span<float> buffer) {
    Queue queue = m_device.getQueue();

    // Fill in input buffer
    queue.writeBuffer(m_inputBuffer, 0, buffer.data(),
                      buffer.size() * sizeof(float));

    // Initialize a command encoder
    CommandEncoderDescriptor encoderDesc = Default;
    CommandEncoder encoder = m_device.createCommandEncoder(encoderDesc);

    // Create compute pass
    ComputePassDescriptor computePassDesc;
    // computePassDesc.timestampWriteCount = 0;
    computePassDesc.timestampWrites = nullptr;
    ComputePassEncoder computePass = encoder.beginComputePass(computePassDesc);

    // Use compute pass
    computePass.setPipeline(m_pipeline);
    computePass.setBindGroup(0, m_bindGroup, 0, nullptr);

    uint32_t invocationCount = m_num_points;
    uint32_t workgroupSize = 32;
    // This ceils invocationCount / workgroupSize
    uint32_t workgroupCount =
        (invocationCount + workgroupSize - 1) / workgroupSize;
    computePass.dispatchWorkgroups(workgroupCount, 1, 1);

    // Finalize compute pass
    computePass.end();

    // Before encoder.finish
    encoder.copyBufferToBuffer(m_inputBuffer, 0, m_mapBuffer, 0,
                               m_num_points * 4 * sizeof(float));

    // Encode and submit the GPU commands
    CommandBuffer commands = encoder.finish(CommandBufferDescriptor{});
    queue.submit(commands);

    // Print output
    bool done = false;
    auto handle = m_mapBuffer.mapAsync(
        MapMode::Read, 0, m_num_points * 4 * sizeof(float),
        [&](BufferMapAsyncStatus status) {
            if (status == BufferMapAsyncStatus::Success) {
                const float* output =
                    (const float*)m_mapBuffer.getConstMappedRange(
                        0, m_num_points * sizeof(float));
                for (size_t i = 0; i < m_num_points; ++i) {
                    buffer[i * 4 + 3] = output[i * 4 + 3];
                }
                m_mapBuffer.unmap();
            }
            done = true;
        });

    while (!done) {
        // Checks for ongoing asynchronous operations and call their callbacks
        // if needed
#ifdef WEBGPU_BACKEND_WGPU
        queue.submit(0, nullptr);
#else
        m_instance.processEvents();
#endif
    }

#if !defined(WEBGPU_BACKEND_WGPU)
    wgpuCommandBufferRelease(commands);
    wgpuCommandEncoderRelease(encoder);
    wgpuComputePassEncoderRelease(computePass);
    wgpuQueueRelease(queue);
#endif
}

}  // namespace implicit_shader
