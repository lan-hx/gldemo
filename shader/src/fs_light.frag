#version 300 es

#ifdef GL_ES
  precision mediump float;
#endif

in vec3 fPosition;
in vec3 fNormal;
in vec2 fTexCoord;
out vec4 fColor;

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
  Light lights[20];
  uint light_count;
};

uniform vec3 viewPos;
uniform Material material;

vec3 calcLight(in Light light, in vec3 normal, in vec3 viewDir) {
  if (light.type == 0u) {
    // ambient light
    return material.ka * light.color * light.intensity;
  }
  else if (light.type == 1u) {
    // directional light
    vec3 lightDir = normalize(-light.direction);
    vec3 diffuse = light.color * max(dot(lightDir, normal), 0.0f) * material.kd;
    vec3 reflectDir = reflect(-lightDir, normal);
    vec3 specular = light.color * pow(max(dot(viewDir, reflectDir), 0.0), material.ns) * material.ks;
    return light.intensity * (diffuse + specular);
  }
  else if (light.type == 2u) {
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
  else {
    return vec3(0.0f, 0.0f, 0.0f);
  }
}

void main()
{
  vec3 viewDir = normalize(viewPos - fPosition);
  vec3 normal = normalize(fNormal);
  vec3 color = vec3(0.0f, 0.0f, 0.0f);
  for (uint i = 0u; i < light_count; ++i) {
    color += calcLight(lights[i], normal, viewDir);
  }
  fColor = vec4(color, 1.0f);
  fColor *= texture(ourTexture, fTexCoord);
  //  fColor = dot(lights[0].position * lights[0].color, lights[0].direction + material.ka + material.kd + material.ks) * lights[0].type * (lights[0].intensity + lights[0].angle + lights[0].kc + lights[0].kl + lights[0].kq + light_count + material.ns) * texture(ourTexture, fTexCoord);
}