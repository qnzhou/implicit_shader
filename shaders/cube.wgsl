@group(0) @binding(0) var<storage,read_write> buffer: array<vec4f>;

struct Box {
    b : vec3<f32>,
};

fn sdBox(p : vec3<f32>, b : vec3<f32>) -> f32 {
    let d : vec3<f32> = abs(p) - b;
    return min(max(d.x,max(d.y,d.z)), 0.0) + length(max(d, vec3f(0.0)));
}

fn sdf(p : vec3<f32>) -> f32 {
    return sdBox(p, vec3<f32>(0.5));
}

@compute @workgroup_size(32)
fn run(@builtin(global_invocation_id) id: vec3<u32>) {
    buffer[id.x].w = sdf(buffer[id.x].xyz);
}
