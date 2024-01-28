#version 430

layout (location = 0) in vec2 inUV;
out vec4 colour;

uniform sampler2D screenTexture;

struct Props {
  float time;
  float warp;
};

uniform Props ps;

void main() {
  vec2 uv = inUV;// - pivot;
 // uv *= 2;
  const float f = 0.22;
  const float a = 5.1;
  uv.x = (1 - ps.warp) * uv.x + (ps.warp)*
    pow(uv.x, f*(a + sin(-ps.time) + cos(ps.time)));// * sign(uv.x);
  uv.y = (1 - ps.warp) * uv.y + (ps.warp)*
    pow(uv.y, f*(a + cos(-ps.time) + sin(ps.time)));//* sign(uv.y);
  colour = texture(screenTexture, uv);
}
