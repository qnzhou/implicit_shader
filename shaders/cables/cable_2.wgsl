// Copyright 2020 @yuntaRobo
// License Creative Commons Attribution-NonCommercial-ShareAlike 3.0 Unported License.
// Link: https://www.shadertoy.com/view/wlKXWc

/******************************************************************************
 This work is a derivative of work by yuntaRobo used under CC BY-NC-SA 3.0.
 This work is licensed also under CC BY-NC-SA 3.0 by NVIDIA CORPORATION.
 ******************************************************************************/

// Constants
const PI: f32 = 3.1415926;
const TAU: f32 = PI * 2.0;
const E: f32 = 0.01;

// Global time variable
const iTime: f32 = 0;

// Function to rotate a 2D vector
fn rotate2D(rad: f32) -> mat2x2<f32> {
    let c: f32 = cos(rad);
    let s: f32 = sin(rad);
    return mat2x2<f32>(c, s, -s, c);
}

// Function to calculate distance and angle
fn de(p: vec3<f32>) -> vec2<f32> {
    var d: f32 = 100.0;
    var a: f32 = 0.0;
    var pp = p;

    // Rotate around yz-plane
    var p_yz = pp.yz;
    p_yz *= rotate2D(PI / 5.0);
    pp.y = p_yz.x;
    pp.z = p_yz.y;
    pp.y -= 0.5;

    // Reaction
    let reaction: vec3<f32> = vec3<f32>(cos(iTime), 0.0, sin(iTime)) * 3.0;
    pp += exp(-length(reaction - pp) * 1.0) * normalize(reaction - pp);

    // Cables
    var r: f32 = atan2(pp.z, pp.x) * 3.0;
    let ite: i32 = 50;
    let i = 2;

    {
        r += 0.5 / f32(ite) * TAU;
        var s: f32 = 0.5 + sin(f32(i) * 1.618 * TAU) * 0.25;
        s += sin(iTime + f32(i)) * 0.1;
        let q: vec2<f32> = vec2<f32>(length(pp.xz) + cos(r) * s - 3.0, pp.y + sin(r) * s);
        let dd: f32 = length(q) - 0.035;
        a = select(f32(i), a, dd < d);
        d = min(d, dd);
    }

    //// Sphere
    //let dd: f32 = length(pp - reaction) - 0.1;
    //a = select(0.0, a, dd < d);
    //d = min(d, dd);

    return vec2<f32>(d, a);
}

// Signed Distance Function
fn sdf(p: vec3<f32>) -> f32 {
    //p += vec3(-0.11, 0.0, 0.0);
    var pp = p;
    const scale: f32 = 0.23;
    pp *= 1.0 / scale;
    return de(pp).x * scale * 0.7;
}

@group(0) @binding(0) var<storage,read_write> buffer: array<vec4f>;

@compute @workgroup_size(32)
fn run(@builtin(global_invocation_id) id: vec3<u32>) {
    buffer[id.x].w = sdf(buffer[id.x].xyz);
}
