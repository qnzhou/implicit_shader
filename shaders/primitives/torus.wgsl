@group(0) @binding(0) var<storage, read> params : array<f32>;
@group(0) @binding(1) var<storage,read_write> buffer: array<vec4f>;

fn sdf(p : vec3<f32>) -> vec4<f32> {
    let center: vec3<f32> = vec3<f32>(params[0], params[1], params[2]);
    let axis: vec3<f32> = vec3<f32>(params[3], params[4], params[5]);
    let R: f32 = params[6];
    let r: f32 = params[7];

    let v: vec3<f32> = normalize(axis);
    let d: vec3<f32> = p - center;
    let l_axis: f32 = dot(d, v);
    let q: vec3<f32> = d - l_axis * v;
    let l_radial: f32 = length(q);

    if (l_radial != 0) {
        let pp: vec3<f32> = center + normalize(q) * R;
        let l: f32 = length(p - pp);
        if (l > 0) {
            return vec4<f32>((p - pp) / l, length(p - pp) - r);
        } else {
            return vec4<f32>(0, 0, 0, -r);
        }
    } else {
        let l: f32 = length(p - center);
        if (l > 0) {
            return vec4<f32>((p - center) / l, length(vec2<f32>(l_axis, R)) - r);
        } else {
            return vec4<f32>(0, 0, 0, -r);
        }
    }
}

@compute @workgroup_size(32)
fn run(@builtin(global_invocation_id) id: vec3<u32>) {
    buffer[id.x] = sdf(buffer[id.x].xyz);
}
