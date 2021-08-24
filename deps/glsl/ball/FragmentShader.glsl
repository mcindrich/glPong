#version 330 core

uniform vec2 uResolution;
uniform float uTime;
uniform vec2 uRectPos;
uniform vec2 uRectSize;

out vec4 outColor;

vec3 rgb(vec3 color) {
	return color / 255.0f;
}

vec4 circle(vec2 pos, vec2 center, float radius) {
    if(length(pos - center) < radius) {
        return vec4(rgb(vec3(225.0, 97.0, 60)), 1);
    }
    return vec4(0);
}

void main()
{

    vec4 outerColor = vec4(0.0f);
    vec4 innerColor = vec4(1.0f, 1.0f, 0.75f, 1.0f);

    vec2 uv = gl_FragCoord.xy;
    float radiusInner = 0.3333 * uRectSize.y, radiusOuter = 0.5 * uRectSize.y;
    vec2 center = uResolution / 2;
    center = uRectPos / 2 + center;
	float dist = length(uv - center);
    outColor = mix(innerColor, outerColor, smoothstep(radiusInner, radiusOuter, dist));

    // outColor = circle(uv, uRectPos / 2 + center, radius);
}
