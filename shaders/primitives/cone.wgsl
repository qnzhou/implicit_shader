@group(0) @binding(0) var<storage, read> params : array<f32>;
@group(0) @binding(1) var<storage,read_write> buffer: array<vec4f>;

fn sdf(p : vec3<f32>) -> vec4<f32> {
    let apex: vec3<f32> = vec3<f32>(params[0], params[1], params[2]);
    let axis: vec3<f32> = vec3<f32>(params[3], params[4], params[5]);
    let angle: f32 = params[6];

    let v: vec3<f32> = normalize(axis);
    let d: vec3<f32> = p - apex;
    let d_axis: vec3<f32> = dot(d, v) * v;
    let d_orth: vec3<f32> = d - d_axis;
    let l_axis: f32 = length(d_axis);
    let l_orth: f32 = length(d_orth);

    if (l_axis <= 0) {
        let l: f32 = length(d);
        if (l == 0) {
            return vec4<f32>(0, 0, 0, 0);
        } else {
            return vec4<f32>(normalize(d), l);
        }
    } else {
        let l_ab: f32 = l_axis + l_orth * tan(angle);
        let l_be: f32 = l_ab * sin(angle);
        let b: vec3<f32> = apex + v * l_ab;

        let l = length(p - b);
        if (l > 0) {
            return vec4<f32>((p - b) / l, l - l_be);
        } else {
            return vec4<f32>(0, 0, 0, -l_be);
        }
    }
}

@compute @workgroup_size(32)
fn run(@builtin(global_invocation_id) id: vec3<u32>) {
    buffer[id.x] = sdf(buffer[id.x].xyz);
}
