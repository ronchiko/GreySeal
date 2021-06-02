attribute vec3 vertex;

uniform vec3 position;
uniform vec3 scale;
uniform mat4 projection;

void main() {
    gl_Position = projection  * vec4((vertex * scale) + position, 1.0);
}
