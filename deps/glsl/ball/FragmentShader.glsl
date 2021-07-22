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
	vec2 uv = gl_FragCoord.xy;
	vec2 center = uResolution * 0.5;
	float radius = 0.5 * uRectSize.y;

    outColor = circle(uv, uRectPos / 2 + center, radius);
}