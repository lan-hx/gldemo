// NOLINTBEGIN
#include <cstdio>
#include <cstdlib>
#include <string>
#include <cstdarg>

using namespace std;

FILE* fp_in = 0;
FILE* fp_out = 0;

string in_file = "";
string out_file = "";
string array_name = "";

void out( const char* format, ...) {
  char printBuffer[1024];
  va_list list;
  va_start(list, format);
  vsprintf(printBuffer, format, list);
  va_end(list);
//#ifndef NDEBUG
//  printf(printBuffer);
//#endif
  fprintf(fp_out, printBuffer);
}
void write_file_header() {
  out("#ifndef  TXT_HEADER_%s\n", array_name.c_str());
  out("#define  TXT_HEADER_%s\n", array_name.c_str());
}
void write_file_footer() {
  out("#endif  // #ifdef TXT_HEADER_%s\n", array_name.c_str());
}
void make_c_string(
    string & in) {
  string out;
  for (size_t i = 0; i < in.size(); ++i) {
    char c = in[i];
    if ('"' == c)
      out += "\\\"";
    else if ('\\' == c)
      out += "\\\\";
    else
      out += c;
  }
  in = out;
}
void write_array_header() {
  out("const char %s [] =\n", array_name.c_str());
}
void write_array_footer() {
  out(";\n");
}
void write_line(const string & line) {
  out("\"%s\\n\"\n", line.c_str());
}
int main(int argc, char** args) {
  if (argc != 4) {
    printf("syntax error, usage :  txt2h array_name infile outfile");
    exit(0xff);
  }
  array_name = args[1];
  in_file = args[2];
  out_file = args[3];

  if (fp_in = fopen(in_file.c_str(), "rt")) {
    if (fp_out = fopen(out_file.c_str(), "wt")) {
      write_file_header();
      out("\n\n");
      write_array_header();
      char buff[1024];
      while (fgets(buff, sizeof(buff), fp_in)) {
        string s(buff);
        s = s.substr(0, s.find('\n'));
        make_c_string(s);
        write_line(s);
      }
      write_array_footer();
      out("\n\n");
      write_file_footer();
      fclose(fp_out);
    } else {
      printf("error opening %s\n", out_file.c_str());
      exit(0xff);
    }
    fclose(fp_in);
  } else {
    printf("error opening %s\n", in_file.c_str());
    exit(0xff);
  }
}
// NOLINTEND
