@group(0) @binding(0) var<storage,read_write> buffer: array<vec4f>;

fn sdf(p : vec3<f32>) -> f32 {
    let r: f32 = 0.1;
    var dir: vec3<f32> = normalize(vec3<f32>(1, 2, 3));
    return length(p - dot(dir, p) * dir) - r;
}

@compute @workgroup_size(32)
fn run(@builtin(global_invocation_id) id: vec3<u32>) {
    buffer[id.x].w = sdf(buffer[id.x].xyz);
}
