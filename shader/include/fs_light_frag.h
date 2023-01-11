#ifndef  TXT_HEADER_fs_light_frag
#define  TXT_HEADER_fs_light_frag


const char fs_light_frag [] =
"#version 330 core\n"
"in vec3 fPosition;\n"
"in vec3 fNormal;\n"
"in vec2 fTexCoord;\n"
"out vec4 fColor;\n"
"\n"
"uniform sampler2D ourTexture;\n"
"\n"
"struct Material {\n"
"  vec3 ka;\n"
"  vec3 kd;\n"
"  vec3 ks;\n"
"  float ns;\n"
"};\n"
"\n"
"struct Light {\n"
"  vec3 position;\n"
"  vec3 direction;\n"
"  vec3 color;\n"
"  float intensity;\n"
"  float angle;\n"
"  float kc;\n"
"  float kl;\n"
"  float kq;\n"
"  uint type;\n"
"};\n"
"\n"
"layout (std140) uniform Lights\n"
"{\n"
"  Light lights[20];\n"
"  uint light_count;\n"
"};\n"
"\n"
"uniform vec3 viewPos;\n"
"uniform Material material;\n"
"\n"
"vec3 calcLight(in Light light, in vec3 normal, in vec3 viewDir) {\n"
"  if (light.type == 0u) {\n"
"    // ambient light\n"
"    return material.ka * light.color * light.intensity;\n"
"  }\n"
"  else if (light.type == 1u) {\n"
"    // directional light\n"
"    vec3 lightDir = normalize(-light.direction);\n"
"    vec3 diffuse = light.color * max(dot(lightDir, normal), 0.0f) * material.kd;\n"
"    vec3 reflectDir = reflect(-lightDir, normal);\n"
"    vec3 specular = light.color * pow(max(dot(viewDir, reflectDir), 0.0), material.ns) * material.ks;\n"
"    return light.intensity * (diffuse + specular);\n"
"  }\n"
"  else if (light.type == 2u) {\n"
"    // spotlight\n"
"    vec3 lightDir = normalize(light.position - fPosition);\n"
"    float theta = acos(-dot(lightDir, normalize(light.direction)));\n"
"    if (theta > light.angle) {\n"
"      return vec3(0.0f, 0.0f, 0.0f);\n"
"    }\n"
"    vec3 diffuse = light.color * max(dot(lightDir, normal), 0.0f) * material.kd;\n"
"    vec3 reflectDir = reflect(-lightDir, normal);\n"
"    vec3 specular = light.color * pow(max(dot(viewDir, reflectDir), 0.0), material.ns) * material.ks;\n"
"    float distance = length(light.position - fPosition);\n"
"    float attenuation = 1.0f / (light.kc + light.kl * distance + light.kq * (distance * distance));\n"
"    return light.intensity * attenuation * (diffuse + specular);\n"
"  }\n"
"  else {\n"
"    return vec3(0.0f, 0.0f, 0.0f);\n"
"  }\n"
"}\n"
"\n"
"void main()\n"
"{\n"
"  vec3 viewDir = normalize(viewPos - fPosition);\n"
"  vec3 normal = normalize(fNormal);\n"
"  vec3 color = vec3(0.0f, 0.0f, 0.0f);\n"
"  for (uint i = 0u; i < light_count; ++i) {\n"
"    color += calcLight(lights[i], normal, viewDir);\n"
"  }\n"
"  fColor = vec4(color, 1.0f);\n"
"  fColor *= texture(ourTexture, fTexCoord);\n"
"  //  fColor = dot(lights[0].position * lights[0].color, lights[0].direction + material.ka + material.kd + material.ks) * lights[0].type * (lights[0].intensity + lights[0].angle + lights[0].kc + lights[0].kl + lights[0].kq + light_count + material.ns) * texture(ourTexture, fTexCoord);\n"
"}\n"
;


#endif  // #ifdef TXT_HEADER_fs_light_frag