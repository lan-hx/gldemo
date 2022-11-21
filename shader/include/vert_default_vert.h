#ifndef  TXT_HEADER_vert_default_vert
#define  TXT_HEADER_vert_default_vert


const char vert_default_vert [] =
"#version 330 core\n"
"layout (location = 0) in vec3 aPos; // 位置变量的属性位置值为0\n"
"\n"
"out vec4 vertexColor; // 为片段着色器指定一个颜色输出\n"
"\n"
"void main()\n"
"{\n"
"    gl_Position = vec4(aPos, 1.0); // 注意我们如何把一个vec3作为vec4的构造器的参数\n"
"    vertexColor = vec4(0.5, 0.0, 0.0, 1.0); // 把输出变量设置为暗红色\n"
"}\n"
;


#endif  // #ifdef TXT_HEADER_vert_default_vert
