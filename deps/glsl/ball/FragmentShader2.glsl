#version 330 core

uniform vec2      iResolution;
uniform float     iTime;

out vec4 outColor;

vec3 rgb(vec3 color) {
	return color / 255.0f;
}

vec4 circle(vec2 uv, vec2 pos, float rad, vec3 color) {
	float d = length(pos - uv) - rad;
	float t = clamp(d, 0.0, 1.0);
	return vec4(color, 1.0 - t);
}

void main()
{
	vec2 uv = gl_FragCoord.xy;
	vec2 center = iResolution * 0.5;
	float radius = iResolution.y;

    // Background layer
	vec4 layer1 = vec4(rgb(vec3(0.0f, 0.0f, 0.0f)), 0.0f);
	
	// Circle
	vec3 red = rgb(vec3(225.0, 97.0, 60));
	vec4 layer2 = circle(uv, center, radius, red);
	
	// Blend the two
	outColor = mix(layer1, layer2, layer2.a);
}