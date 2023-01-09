#ifndef  TXT_HEADER_vs_light_vert
#define  TXT_HEADER_vs_light_vert


const char vs_light_vert [] =
"#version 330 core\n"
"layout (location = 0) in vec3 aPosition;\n"
"layout (location = 1) in vec3 aNormal;\n"
"layout (location = 2) in vec2 aTexCoord;\n"
"\n"
"out vec3 fPosition;\n"
"out vec2 fNormal;\n"
"out vec2 fTexCoord;\n"
"\n"
"uniform mat4 model;\n"
"uniform mat4 view;\n"
"uniform mat4 projection;\n"
"\n"
"void main()\n"
"{\n"
"  fPosition = vec3(model * vec4(aPosition, 1.0f));;\n"
"  fNormal = mat3(transpose(inverse(model))) * aNormal;\n"
"  fTexCoord = aTexCoord;\n"
"  gl_Position = projection * view * model * vec4(aPosition, 1.0f);\n"
"}\n"
;


#endif  // #ifdef TXT_HEADER_vs_light_vert
