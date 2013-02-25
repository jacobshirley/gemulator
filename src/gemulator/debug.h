#ifndef _DEBUG_H
#define _DEBUG_H

#include <iostream>
#include <sstream>

namespace debug {
  static std::string int_to_str(int i) {
    std::stringstream ss;
    ss << i;
    return ss.str();  
  }

  static void output(std::string s) {
    std::cout << s << std::endl;
  }

  static void output(int i) {
    output(int_to_str(i));
  }
};

#endif
