// Copyright 2020 Flopine @Flopine
// License Creative Commons Attribution-NonCommercial-ShareAlike 3.0 Unported License.
// Link: https://www.shadertoy.com/view/wssBDf

/******************************************************************************
 This work is a derivative of work by Flopine used under CC BY-NC-SA 3.0.
 This work is licensed also under CC BY-NC-SA 3.0 by NVIDIA CORPORATION.
 ******************************************************************************/

const PI: f32 = 3.141592;

fn rot(a: f32) -> mat2x2<f32> {
    return mat2x2<f32>(cos(a), sin(a), -sin(a), cos(a));
}

fn cyl(p: vec3<f32>, r: f32, h: f32) -> f32 {
    return max(length(p.xy) - r, abs(p.z) - h);
}

fn tore(p: vec3<f32>, t: vec2<f32>) -> f32 {
    return length(vec2(length(p.xz) - t.x, p.y)) - t.y;
}

fn key(p: vec3<f32>, t: f32) -> f32 {
    var thick: f32 = t;
    var body: f32 = cyl(p.xzy, thick, 1.5);
    var encoche: f32 = tore(p.xzy + vec3(-(2. * thick), 0.05, 1.), vec2(thick, 0.1));
    var head: f32 = max(-cyl(p - vec3(0., 2.2, 0.), 0.65, thick * 1.5), cyl(p - vec3(0., 2.2, 0.), 0.8, thick));
    var p2: vec3<f32> = p;
    p2.y = abs(abs(p2.y - 0.45) - 0.8) - 0.15;
    var ts: f32 = tore(p2, vec2(thick, 0.08));

    return min(encoche, min(min(body, head), ts));
}

fn SDF(p: vec3<f32>) -> f32 {
    var pp: vec3<f32> = p - vec3(0., 2., 0.);
    var small: f32 = 3.5;
    var thick: f32 = 0.25;
    var d: f32 = key(p, thick);
    for (var i: i32 = 0; i < 2; i = i + 1) {
        d = min(d, key(pp * small, thick) / small);
        pp.y -= 0.55;
        small *= 4.;
    }
    return d;
}

fn sdf(p: vec3<f32>) -> f32 {
    var pp = p;
    pp += vec3(0., 0.2, 0.);
    let scale: f32 = 0.3;
    pp *= 1. / scale;
    return SDF(pp) * scale;
}

@group(0) @binding(0) var<storage,read_write> buffer: array<vec4f>;

@compute @workgroup_size(32)
fn run(@builtin(global_invocation_id) id: vec3<u32>) {
    buffer[id.x].w = sdf(buffer[id.x].xyz);
}
