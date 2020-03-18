#include "string_encoding.h"

#include <windows.h>


void unicode_to_utf8(const wchar_t* in, std::string& out) {
  int len = WideCharToMultiByte(CP_UTF8, 0, in, -1, NULL, 0, NULL, NULL);
  out.resize(len);
  WideCharToMultiByte(CP_UTF8, 0, in, -1, (char*)out.c_str(), len, NULL, NULL);
}