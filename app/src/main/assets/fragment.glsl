precision mediump float;

uniform sampler2D texture;
uniform vec4 tint;

varying vec2 textureUv;
varying vec3 currentNormal;

void main() {
    vec3 sun = normalize(vec3(1.0, 0.0, 0.0));
    vec3 lightColor = vec3(0.0, 0.0, 0.0);

    float intensity = max(dot(sun, currentNormal) + 1.0, 0.5) * 0.5 + 0.1;
    vec3 fragColour = max(intensity * vec3(1.0, 1.0, 1.0), vec3(0.1, 0.1, 0.1));

    gl_FragColor = texture2D(texture, textureUv) * vec4(fragColour, 1.0) * tint;
}
