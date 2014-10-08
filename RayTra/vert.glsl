#version 400

uniform mat4 camera;

in vec3 vp;
out vec3 color;
void main() {
	gl_Position = camera * vec4(vp, 1.0);
}