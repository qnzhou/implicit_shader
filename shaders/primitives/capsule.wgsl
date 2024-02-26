@group(0) @binding(0) var<storage, read> params : array<f32>;
@group(0) @binding(1) var<storage,read_write> buffer: array<vec4f>;

fn sdf(p : vec3<f32>) -> vec4<f32> {
    let p1: vec3<f32> = vec3<f32>(params[0], params[1], params[2]);
    let p2: vec3<f32> = vec3<f32>(params[3], params[4], params[5]);
    let r: f32 = params[6];

    let u: vec3<f32> = p - p1;
    let v: vec3<f32> = p2 - p1;
    let h: f32 = clamp(dot(u, v) / dot(v, v), 0.0, 1.0);
    let q: vec3<f32> = p1 + h * v;
    let d: f32 = length(p - q);

    if (d != 0) {
        return vec4<f32>((p - q) / d, d - r);
    } else {
        return vec4<f32>(0.0, 0.0, 0.0, -r);
    }
}


@compute @workgroup_size(32)
fn run(@builtin(global_invocation_id) id: vec3<u32>) {
    buffer[id.x] = sdf(buffer[id.x].xyz);
}
