attribute vec3 coord3d;
attribute vec3 v_color;
varying vec3 f_color;

attribute vec2 texcoord;
varying vec2 f_texcoord;

uniform mat4 mvp;

void main(void) {
  gl_Position = mvp * vec4(coord3d, 1.0);
  f_color = v_color;
  f_texcoord = texcoord;
}

