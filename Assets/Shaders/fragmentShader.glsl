#version 460

in vec2 TexCoord;
in vec2 TileOffset;
in vec3 Normal;
in vec3 FragPos;
in vec4 DirectionalLightSpacePos;

const float tilesPerRow = 64.0;

out vec4 color;

const int MAX_POINT_LIGHTS = 3;
const int MAX_SPOT_LIGHTS  = 3;

struct Light {
	vec3 color;
	float ambientIntensity;
	float diffuseIntensity;
};
struct DirectionalLight {
	Light base;
	vec3 direction;
};
struct PointLight {
	Light base;
	vec3 position;
	float constant;
	float linear;
	float exponent;
};
struct SpotLight {
	PointLight base;
	vec3 direction;
	float edge;
};

uniform samplerCube omniShadowMaps[MAX_POINT_LIGHTS + MAX_SPOT_LIGHTS];
uniform float omniShadowFarPlanes[MAX_POINT_LIGHTS + MAX_SPOT_LIGHTS];

struct Material {
	float specularIntensity;
	float shininess;
};

uniform int pointLightCount;
uniform int  spotLightCount;

uniform DirectionalLight directionalLight;
uniform PointLight pointLights[MAX_POINT_LIGHTS];
uniform SpotLight spotLights[MAX_SPOT_LIGHTS];
//uniform OmniShadowMap omniShadowMaps[MAX_POINT_LIGHTS + MAX_SPOT_LIGHTS];

uniform sampler2D theTexture;
uniform sampler2D directionalShadowMap;
uniform Material material;

uniform vec3 eyePosition;

vec3 sampleOffsetDirections[20] = vec3[] (
	vec3(1,  1,  1), vec3( 1, -1,  1), vec3(-1, -1,  1), vec3(-1, 1,  1),
	vec3(1,  1, -1), vec3( 1, -1, -1), vec3(-1, -1, -1), vec3(-1, 1, -1),
	vec3(1,  1,  0), vec3( 1, -1,  0), vec3(-1, -1,  0), vec3(-1, 1,  0),
	vec3(1,  0,  1), vec3(-1,  0,  1), vec3( 1,  0, -1), vec3(-1, 0, -1),
	vec3(0,  1,  1), vec3( 0, -1,  1), vec3( 0, -1, -1), vec3( 0, 1, -1)
);

float CalcDirectionalShadowFactor (DirectionalLight light) {
	vec3 projCords = DirectionalLightSpacePos.xyz / DirectionalLightSpacePos.w;
	projCords = (projCords * 0.5) + 0.5;
	float current = projCords.z;

	vec3 normal = normalize(Normal);
	vec3 lightDir = normalize(light.direction); 

	float bias = max(0.0007 * (1 - dot(normal, lightDir)), 0.00005);

	float shadow = 0;

	vec2 texelSize = 1.0 / textureSize(directionalShadowMap, 0);

	for (int x = -1; x <= 1; x++) {
		for (int y = -1; y <= 1; y++) {
			float pcfDepth = texture(directionalShadowMap, projCords.xy + vec2(x, y) * texelSize).r;
			shadow += current - bias > pcfDepth ? 1.0 : 0.0;
		}
	}

	shadow /= 9;

	if (projCords.z > 1.0) {
		shadow = 0.0;
	}

	return shadow;
}
float CalcOmniShadowFactor(PointLight light, int shadowIndex) {
	vec3 fragToLight = FragPos - light.position;
	
	float currect = length(fragToLight);
	
	float shadow = 0;
	float bias = 0.05;
	int samples = 20;

	float viewDistance = length(eyePosition - FragPos);
	float diskRadius = (1.0 + (viewDistance / omniShadowFarPlanes[shadowIndex])) / 25.0;

	for (int i = 0; i < samples; i++) {
		float closest = texture(omniShadowMaps[shadowIndex], fragToLight + sampleOffsetDirections[i]*diskRadius).r;
		closest *= omniShadowFarPlanes[shadowIndex];
		if (currect - bias > closest) {
			shadow += 1.0;
		}
	}

	shadow /= float(samples);

	return shadow;
}
vec4 CalcLightByDirection(Light light, vec3 direction, float shadowFactor) {
	vec4 ambientColor = vec4(light.color, 1.0f) * light.ambientIntensity;
	
	float diffuseFactor = max(dot(normalize(Normal), normalize(direction)), 0.0f);
	vec4 diffuseColor = vec4(light.color, 1.0f) * light.diffuseIntensity * diffuseFactor;

	vec4 specularColor = vec4(0.0f, 0.0f, 0.0f, 0.0f);
	
	if (diffuseFactor > 0.0f) {
		vec3 fragToEye = normalize(eyePosition - FragPos);
		vec3 reflectedVertex = normalize(reflect(direction, normalize(Normal)));
		float specularFactor = dot(fragToEye, reflectedVertex);
		if (specularFactor > 0.0f) {
			specularFactor = pow(specularFactor, material.shininess);
			specularColor = vec4(light.color * material.specularIntensity * specularFactor, 1.0f);
		}
	}
	
	return (ambientColor + (1.0 - shadowFactor) * (diffuseColor + specularColor));
}
vec4 CalcDirectionalLight() {
	return CalcLightByDirection(directionalLight.base, directionalLight.direction, CalcDirectionalShadowFactor(directionalLight));
}
vec4 CalcPointLight(PointLight pLight, int shadowIndex) {
	vec3 direction = FragPos - pLight.position;
	float distance = length(direction);
	direction = normalize(direction); 
		
	float shadowFactor = CalcOmniShadowFactor(pLight, shadowIndex);

	vec4 color = CalcLightByDirection(pLight.base, direction, shadowFactor);

	float attenuation = pLight.exponent * distance * distance +
		                pLight.linear * distance +
						pLight.constant;
	return color/attenuation;
}
vec4 CalcSpotLight(SpotLight sLight, int shadowIndex) {
	vec3 rayDirection = normalize(FragPos - sLight.base.position);
	float slFactor = dot(rayDirection, sLight.direction);

	if (slFactor > sLight.edge) {
		vec4 color = CalcPointLight(sLight.base, shadowIndex);
		return color * (1.0f - (1.0f - slFactor) * (1.0f/(1.0f - sLight.edge)));
	} else {
		return vec4(0); // 0, 0, 0, 0
	}
}
vec4 CalcSpotLights() {
	vec4 totalColor = vec4(0);
	for(int i = 0; i < spotLightCount; i++) {
		totalColor += CalcSpotLight(spotLights[i], i + pointLightCount);
	}
	return totalColor;
}
vec4 CalcPointLights() {
	vec4 totalColor = vec4(0);
	for(int i = 0; i < pointLightCount; i++) {
		totalColor += CalcPointLight(pointLights[i], i);
	}
	return totalColor;
}

void main() {
	
	vec2 localUV = fract(abs(TexCoord)); // Use abs so tiling works

	bool rotateCW = TexCoord.x < 0.0;
	bool rotateCCW = TexCoord.y < 0.0;

	// Rotate if needed
	if (rotateCW) {
		localUV = vec2(1.0 - localUV.y, localUV.x); // 90° clockwise
	}
	else if (rotateCCW) {
		localUV = vec2(localUV.y, 1.0 - localUV.x); // 90° counter-clockwise
	}



	vec2 tiledUV = TileOffset + localUV / 64.0;
	vec4 texColor = texture(theTexture, tiledUV);



    if(texColor.a < 0.1)
        discard;

    vec4 finalColor  = CalcDirectionalLight(); 
         finalColor += CalcPointLights();
         finalColor += CalcSpotLights();

    color = vec4(texColor.rgb * finalColor.rgb, texColor.a);
}