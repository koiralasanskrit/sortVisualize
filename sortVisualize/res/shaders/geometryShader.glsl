#version 330 core
layout (lines) in;
layout (triangle_strip, max_vertices = 4) out;

in vec4 v_fcolor[];
in float v_thickness[];
out vec4 vg_fcolor;

float wa = v_thickness[0];
float wb = v_thickness[1];

// V_p, V_n
vec2 a = gl_in[0].gl_Position.xy;
vec2 b = gl_in[1].gl_Position.xy;

vec2 Vp = normalize(b - a);
vec2 Vn = vec2(Vp.y, - Vp.x);

vec2 M = a + (wa/2) * Vn;
vec2 N = a - (wa/2) * Vn;

vec2 P = b + (wb/2) * Vn;
vec2 B = b - (wb/2) * Vn;

void main() {    
//    gl_Position = gl_in[0].gl_Position; 
    gl_Position = vec4(M, 0.0, 1.0);
    vg_fcolor = v_fcolor[0];
    EmitVertex();

    gl_Position = vec4(N, 0.0, 1.0);
    vg_fcolor = v_fcolor[0];
    EmitVertex();

    gl_Position = vec4(P, 0.0, 1.0);
    vg_fcolor = v_fcolor[0];
    EmitVertex();

    gl_Position = vec4(B, 0.0, 1.0);
    vg_fcolor = v_fcolor[0];
    EmitVertex();

    EndPrimitive();
}  