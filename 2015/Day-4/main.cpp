#include <iostream>

void md5()
{
  auto F = [](const int32_t& X, const int32_t& Y, const int32_t& Z)->int32_t{
    return (X & Y) | (~X & Z);
  };
  auto G = [](const int32_t& X, const int32_t& Y, const int32_t& Z)->int32_t{
    return (X & Y) | (Y & ~Z);
  };
  auto H = [](const int32_t& X, const int32_t& Y, const int32_t& Z)->int32_t{
    return (X ^ Y ^ Z);
  };
  auto I = [](const int32_t& X, const int32_t& Y, const int32_t& Z)->int32_t{
    return (Y ^ (X | ~Z));
  };

  // Initialising constants according to
  // https://www.ietf.org/rfc/rfc1321.txt
  int32_t A = 0x67452301; // least significant byte order
  int32_t B = 0xefcdab89; // least significant byte order
  int32_t C = 0x98badcfe; // least significant byte order
  int32_t D = 0x10325476; // least significant byte order
}

int main()
{
  std::string input = "bgvyzdsv";
  intmax_t number = 1;
  while(true)
  {
    std::string inputToTry = input + std::to_string(number);
  std::cout<<inputToTry<<"\n";
//    break;
  number++;
  }
  return 0;
}
