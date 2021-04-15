attribute vec3 vertex;
attribute vec3 _normal;
attribute vec2 _uv;

uniform mat4 projectionMatrix;
uniform mat4 modelMatrix;

varying vec2 uv;
varying vec3 normal;

void main() {
    gl_Position = projectionMatrix * modelMatrix * vec4(vertex, 1.0);
    uv = _uv;
    normal = normalize((projectionMatrix * modelMatrix * vec4(_normal, 0.0)).xyz);
}
