#ifndef  TXT_HEADER_vs1_vert
#define  TXT_HEADER_vs1_vert


const char vs1_vert [] =
"#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"//layout (location = 1) in vec3 aColor;\n"
"layout (location = 1) in vec2 aTexCoord;\n"
"\n"
"out vec3 ourColor;\n"
"out vec2 TexCoord;\n"
"\n"
"uniform mat4 model;\n"
"uniform mat4 view;\n"
"uniform mat4 projection;\n"
"\n"
"void main()\n"
"{\n"
"    gl_Position = projection * view * model * vec4(aPos, 1.0);\n"
"    TexCoord = aTexCoord;\n"
"}\n"
;


#endif  // #ifdef TXT_HEADER_vs1_vert
