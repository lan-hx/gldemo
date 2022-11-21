#ifndef  TXT_HEADER_fs1_frag
#define  TXT_HEADER_fs1_frag


const char fs1_frag [] =
"#version 330 core\n"
"out vec4 FragColor;\n"
"\n"
"//in vec3 ourColor;\n"
"in vec2 TexCoord;\n"
"\n"
"uniform sampler2D ourTexture;\n"
"\n"
"void main()\n"
"{\n"
"//   FragColor = vec4(0.9, 0.8, 0.7, 1.0);\n"
"  FragColor = texture(ourTexture, TexCoord);\n"
"}\n"
;


#endif  // #ifdef TXT_HEADER_fs1_frag
