@group(0) @binding(0) var<storage, read> params : array<f32>;
@group(0) @binding(1) var<storage,read_write> buffer: array<vec4f>;

fn sdf(p : vec3<f32>) -> vec4<f32> {
    let c: vec3<f32> = vec3<f32>(params[0], params[1], params[2]);
    let n: vec3<f32> = vec3<f32>(params[3], params[4], params[5]);

    let d: vec3<f32> = p - c;
    let v: vec3<f32> = normalize(n);

    return vec4<f32>(v, dot(d, v));
}


@compute @workgroup_size(32)
fn run(@builtin(global_invocation_id) id: vec3<u32>) {
    buffer[id.x] = sdf(buffer[id.x].xyz);
}
