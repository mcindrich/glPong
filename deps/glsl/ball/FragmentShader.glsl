#version 330 core

uniform vec2 uResolution;
uniform float uTime;

out vec4 outColor;

vec3 rgb(vec3 color) {
	return color / 255.0f;
}

vec4 circle(vec2 pos, vec2 center, float radius) {
    if(length(pos - center) < abs(cos(uTime)) * radius) {
        return vec4(rgb(vec3(abs(225.0 * cos(uTime)), 97.0, 60)), 1);
    }
    return vec4(0.0f);
}

void main()
{
    const vec2 res = vec2(800, 600);
	vec2 uv = gl_FragCoord.xy;
	vec2 center = res.xy * 0.5;
	float radius = 0.3 * res.y;

    outColor = circle(uv, center, radius);
}