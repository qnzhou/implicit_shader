@group(0) @binding(0) var<storage, read> params : array<f32>;
@group(0) @binding(1) var<storage,read_write> buffer: array<vec4f>;

fn sdf(p : vec3<f32>) -> f32 {
    var c: vec3<f32> = vec3<f32>(params[0], params[1], params[2]);
    var r: f32 = params[3];
    return length(p - c) - r;
}

fn grad(p : vec3<f32>) -> vec3<f32> {
    var c: vec3<f32> = vec3<f32>(params[0], params[1], params[2]);
    return normalize(p - c);
}

@compute @workgroup_size(32)
fn run(@builtin(global_invocation_id) id: vec3<u32>) {
    buffer[id.x].w = sdf(buffer[id.x].xyz);
    var g: vec3<f32> = grad(buffer[id.x].xyz);
    buffer[id.x].x = g.x;
    buffer[id.x].y = g.y;
    buffer[id.x].z = g.z;
}
