#version 330 core

layout(location = 0) in vec3 position;
// layout(location = 1) in vec3 normal;
// layout(location = 2) in vec3 tangent;
// layout(location = 3) in vec2 uv0;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
//uniform mat4 mvp;
//uniform mat4 modelView;
//uniform mat4 normalMatrix;


// out vec3 o_positionWorld;
// out vec3 o_normalWorld;
out vec3 o_uv0;

void main() {
  o_uv0 = position;
  vec4 positionWorld = model * vec4(position, 1.0);
  gl_Position = projection * view * positionWorld;
}
