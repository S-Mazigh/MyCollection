#pragma once
#include <string>
#define DEBUG_LOG 0 // everything is printed
#define INFO_LOG 1 // only necessary information is printed
#define ERROR_LOG 2 // should always be printed

extern char CURRENT_LEVEL;

namespace myprint {
    void boxedPrint(const std::string &toprint, int paddingHor = 1, int paddingVer = 1);
    void log(const std::string &to_print, char level = 0);
}