attribute vec2 vertex;
attribute vec2 _uv;

uniform vec2 screen;

varying vec2 uv;

void main() {
    gl_Position = vec4(vertex.x / screen.x * 2.0 - 1.0, 1.0 - vertex.y / screen.y * 2.0, 0.0, 1.0);
    uv = _uv;
}
