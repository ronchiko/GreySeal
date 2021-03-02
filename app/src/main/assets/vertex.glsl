attribute vec3 vertex;
attribute vec3 normal;
attribute vec2 uv;

uniform mat4 projectionMatrix;
uniform mat4 modelMatrix;

varying vec2 textureUv;
varying vec3 currentNormal;

void main() {
    gl_Position = projectionMatrix * modelMatrix * vec4(vertex, 1.0);
    textureUv = uv;
    currentNormal = normalize((projectionMatrix * modelMatrix * vec4(normal, 0.0)).xyz);
}
