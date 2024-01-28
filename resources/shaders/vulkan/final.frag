#version 450

layout(set = 1, binding = 0) uniform sampler offscreenSampler;
layout(set = 1, binding = 1) uniform texture2D offscreenTex;
layout(set = 2, binding = 0) uniform propsUBO {
  float time;
  float warp;
} ps;

layout (location = 0) in vec2 inUV;

layout (location = 0) out vec4 outFragColor;

void main()
{
 // vec2 pivot = vec2(0.5, 0.5);
  vec2 uv = inUV;// - pivot;
 // uv *= 2;
  const float f = 0.22;
  const float a = 5.1;
  uv.x = (1 - ps.warp) * uv.x + (ps.warp)*
    pow(uv.x, f*(a + sin(-ps.time) + cos(ps.time)));// * sign(uv.x);
  uv.y = (1 - ps.warp) * uv.y + (ps.warp)*
    pow(uv.y, f*(a + cos(-ps.time) + sin(ps.time)));//* sign(uv.y);
  //uv.x -= 0.1 * (abs(sin(ps.time + uv.x))) * uv.x;
  //uv.y -= 0.1 * (abs(sin(ps.time + uv.y + 0.25))) * uv.y;
  //uv /= 2;
  //uv += pivot;
  outFragColor = texture(sampler2D(offscreenTex, offscreenSampler), uv);
}
