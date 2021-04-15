attribute vec2 vertex;
attribute vec2 _uv;

varying vec2 uv;

void main() {
    gl_Position = vec4(vertex, 0.0, 1.0);
    uv = _uv;
}
