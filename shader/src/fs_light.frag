#version 330 core
in vec3 fPosition;
in vec3 fNormal;
in vec2 fTexCoord;
out vec4 color;

uniform sampler2D ourTexture;

struct Material {
  vec3 ka;
  vec3 kd;
  vec3 ks;
  float ns;
};

struct Light {
  vec3 position;
  vec3 direction;
  vec3 color;
  float intensity;
  float angle;
  float kc;
  float kl;
  float kq;
  uint type;
};

layout (std140) uniform Lights
{
  Light light[20];
  uint light_count;
};

uniform vec3 viewPos;
uniform Material material;

vec3 calcLight(Light light, vec3 normal, vec3 viewDir) {
  if(light.type == 0) {
    // ambient light
    return material.ka * light.color * light.intensity;
  }
  else if(light.type == 1) {
    // directional light
    vec3 lightDir = normalize(-light.direction);
    vec3 diffuse = light.color * max(dot(lightDir, normal), 0.0f) * material.kd;
    vec3 reflectDir = reflect(-lightDir, normal);
    vec3 specular = light.color * pow(max(dot(viewDir, reflectDir), 0.0), material.ns) * material.ks;
    return light.intensity * (diffuse + specular);
  }
  else if(light.type == 2) {
    // spotlight
    vec3 lightDir = normalize(light.position - fPosition);
    float theta = acos(-dot(lightDir, normalize(light.direction)));
    if (theta > light.angle) {
      return vec3(0.0f, 0.0f, 0.0f);
    }
    vec3 diffuse = light.color * max(dot(lightDir, normal), 0.0f) * material.kd;
    vec3 reflectDir = reflect(-lightDir, normal);
    vec3 specular = light.color * pow(max(dot(viewDir, reflectDir), 0.0), material.ns) * material.ks;
    float distance = length(light.position - fPosition);
    float attenuation = 1.0f / (light.kc + light.kl * distance + light.kq * (distance * distance));
    return light.intensity * attenuation * (diffuse + specular);
  }
}

void main()
{
  vec3 viewDir = normalize(viewPos - fPosition);
  vec3 normal = normalize(fNormal);
  color = vec3(0.0f, 0.0f, 0.0f);
  for(uint i = 0; i < light_count; ++i) {
    color += calcLight(light[i], normal, viewDir);
  }
  color *= texture(ourTexture, fTexCoord);
}