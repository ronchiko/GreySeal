precision mediump float;

uniform sampler2D texture;
uniform vec4 tint;

varying vec2 uv;

void main() {
    gl_FragColor = texture2D(texture, uv) * tint;
}
