#include <iostream>
#include <string>
#include "../inputdefault.h"


int main(int argc, char ** argv)
{
  int test = 0;
  float ftest = 0;
  double dtest = 0;
  char buffer[100];
  std::string instr;

  std::cout << "Using cin to enter in" << std::endl;
  std::cin >> test;

  std::cout << "Enter a int" << std::endl;

  if(!MiscUtils::indefault(test))
    std::cout << "Default pressed" << std::endl;

  
  std::cout << "Enter a float" << std::endl;

  if(!MiscUtils::indefault(ftest))
    std::cout << "Default pressed" << std::endl;

  
  std::cout << "Enter a double" << std::endl;

  if(!MiscUtils::indefault(dtest))
    std::cout << "Default pressed" << std::endl;

  std::cout << "Enter a string (100 char)" << std::endl;

  if(!MiscUtils::indefault(buffer, 100))
    std::cout << "Default pressed" << std::endl;

  std::cout << "Enter a string" << std::endl;

  if(!MiscUtils::indefault(instr))
    std::cout << "Default pressed" << std::endl;
  


  return 0;
}
