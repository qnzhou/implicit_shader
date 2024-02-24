@group(0) @binding(0) var<storage, read> params : array<f32>;
@group(0) @binding(1) var<storage,read_write> buffer: array<vec4f>;

fn sdf(p : vec3<f32>) -> vec4<f32> {
    let p0: vec3<f32> = vec3<f32>(params[0], params[1], params[2]);
    let p1: vec3<f32> = vec3<f32>(params[3], params[4], params[5]);
    let r: f32 = params[6];

    var d: vec3<f32> = p - p0;
    var v: vec3<f32> = normalize(p1 - p0);
    d = d - dot(d, v) * v;

    let l: f32 = length(d);
    if (l != 0) {
        return vec4<f32>(d / l, l - r);
    } else {
        return vec4<f32>(0, 0, 0, -r);
    }
}


@compute @workgroup_size(32)
fn run(@builtin(global_invocation_id) id: vec3<u32>) {
    buffer[id.x] = sdf(buffer[id.x].xyz);
}
