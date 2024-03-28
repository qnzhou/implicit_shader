@group(0) @binding(0) var<storage, read> params : array<f32>;
@group(0) @binding(1) var<storage,read_write> buffer: array<vec4f>;

fn outer_product(v1: vec3<f32>, v2: vec3<f32>) -> mat3x3<f32> {
    return mat3x3<f32>(
        v2.x * v1.x, v2.x * v1.y, v2.x * v1.z,
        v2.y * v1.x, v2.y * v1.y, v2.y * v1.z,
        v2.z * v1.x, v2.z * v1.y, v2.z * v1.z
    );
}

fn sdf(p : vec3<f32>) -> vec4<f32> {
    let n: u32 = (arrayLength(&params) - 4) / 7;
    var r_w: f32 = 0.0;
    var r_xyz: vec3<f32> = vec3<f32>(0.0, 0.0, 0.0);
    let I: mat3x3<f32> = mat3x3<f32>(
        1.0, 0.0, 0.0,
        0.0, 1.0, 0.0,
        0.0, 0.0, 1.0
    );
    for (var i = 0u; i < n; i++) {
        let pi: vec3<f32> = vec3<f32>(params[i * 3], params[i * 3 + 1], params[i * 3 + 2]);
        let d: f32 = length(p - pi); // d^3 is the kernel function value.
        let g: vec3<f32> = 3 * d * (p-pi); // kernel function gradient.
        let H: mat3x3<f32> = (I * d + outer_product(p - pi, p - pi) * (1.0 / d)) * 3; // kernel function Hessian.
        let coeff: vec4<f32> = vec4<f32>(
            params[n * 3 + i],
            params[n * 4 + i],
            params[n * 5 + i],
            params[n * 6 + i]
        );
        r_w += d * d * d * coeff.x + dot(g, coeff.yzw);
        r_xyz += g * coeff.x + H * coeff.yzw;
    }

    let c: vec4<f32> = vec4<f32>(params[n * 7], params[n * 7 + 1], params[n * 7 + 2], params[n * 7 + 3]);
    r_w += c.x + dot(p, c.yzw);
    r_xyz += c.yzw;
    return vec4<f32>(r_xyz, r_w);
}

@compute @workgroup_size(32)
fn run(@builtin(global_invocation_id) id: vec3<u32>) {
    buffer[id.x] = sdf(buffer[id.x].xyz);
}
