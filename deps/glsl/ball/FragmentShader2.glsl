#version 330 core

uniform vec2 uResolution;
uniform float uTime;
uniform vec2 uRectPos;
uniform vec2 uRectSize;

out vec4 outColor;
#define PI 3.14159265359

float gradientFn(in float x, in float low, in float high, in float target) {
    float b = ((low * target) - high) / (low - high);
    float m = (1.0 - b) / low;
    return m*x + b;
}


void main()
{
    // Normalized pixel coordinates (from 0 to 1)
    vec2 uv = gl_FragCoord.xy/uResolution.xy;
   
    float aspect = uResolution.x / uResolution.y;
    
    vec2 pos = (uv * 2.0) - vec2(1, 1);
    pos.x *= aspect;
    
    float radius = length(pos);

    float x = radius*1.0 - uTime/4.0;
    float ringColor = mod(x, 1.0) * 10.0;

    vec3 finalColor;
    if (ringColor < 1.0) {
        finalColor = vec3(230,0,63) / 255.0;
    } else if (ringColor < 2.0) {
        finalColor = vec3(255,0,0) / 255.0;
    } else if (ringColor < 3.0) {
        finalColor = vec3(255,81,0) / 255.0;
    } else if (ringColor < 4.0) {
        finalColor = vec3(255,138,0) / 255.0;
    } else if (ringColor < 5.0) {
        finalColor = vec3(255,189,0) / 255.0;
    } else if (ringColor < 6.0) {
        finalColor = vec3(241,236,0) / 255.0;
    } else if (ringColor < 7.0) {
        finalColor = vec3(0,245,138) / 255.0;
    } else if (ringColor < 8.0) {
        finalColor = vec3(0,206,190) / 255.0;
    } else if (ringColor < 9.0) {
        finalColor = vec3(0,118,252) / 255.0;
    } else if (ringColor < 10.0) {
        finalColor = vec3(140,24,195) / 255.0;
    }

    float gradient = mod(ringColor, 1.0);
    if (gradient > 0.6) {
        finalColor *= gradientFn(gradient, 0.6, 1.0, 0.3);
    }

    outColor = vec4(finalColor, 1);
}


#version 330

uniform vec2 uResolution;
uniform float uTime;

out vec4 outColor;


vec3 rgb(float r, float g, float b) {
	return vec3(r, g, b) / 255;
}

vec4 outerColor = vec4(0.0f);
vec4 innerColor = vec4(1.0f, 1.0f, 0.75f, 1.0f);

void main()
{
    vec2 uv = gl_FragCoord.xy;
    float radiusInner = 0.1 * uResolution.y, radiusOuter = 0.2 * uResolution.y;
    vec2 center = uResolution / 2;
	float dist = length(uv - center);
    outColor = mix(innerColor, outerColor, smoothstep(radiusInner, radiusOuter, dist));
}
