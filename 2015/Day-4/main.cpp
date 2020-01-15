#include <iostream>
#include <string.h>
#include <vector>
#include <bitset>
#include <math.h>
#include <limits.h>
#include <iomanip>
#include "md5.h"

const auto table = []()->std::vector<uint32_t>{
  std::vector<uint32_t> sineTable;
  for(int rad = 1; rad <= 64; ++rad)
  {
    // 2^32 = 4294967296
    sineTable.push_back(static_cast<uint32_t>(std::floor(4294967296 * std::abs(std::sin(rad)))));
    std::cout << std::setfill('0') << std::setw(sizeof(uint32_t)*2) << std::hex
              << sineTable.back()<<std::endl;
  }
  return sineTable;
}();

std::string my_md5(const std::string& message)
{
  //Convert message to bits
  std::vector<bool> bits;
  for (size_t i = 0; i < message.size(); ++i)
  {
    std::bitset<8> charBits = std::bitset<8>(message.c_str()[i]);
    for(size_t b = 0; b < 8; ++b)
    {
      bits.push_back(charBits[b]);
    }
  }

  std::bitset sizeBits64 = std::bitset<64>(bits.size());

  //1. Padding to "congruent to 448, modulo 512"
  bits.push_back(1);
  while(bits.size() % 512 != 448)
  {
    bits.push_back(0);
  }

  //2. Append length
  for(int i = 0; i < 64; ++i)
  {
    bits.push_back(sizeBits64[i]);
  }

  // 3. Initialize MD Buffer
  // Initialising constants according to
  // https://www.ietf.org/rfc/rfc1321.txt
  uint32_t A = 0x67452301;
  uint32_t B = 0xefcdab89;
  uint32_t C = 0x98badcfe;
  uint32_t D = 0x10325476;

  // 4. Process Message in 16-Word Blocks

  // Define four auxiliary functions that each take as input three 32-bit words and produce as output one 32-bit word.
  auto F = [](const uint32_t& X, const uint32_t& Y, const uint32_t& Z)->uint32_t{
    return (X & Y) | (~X & Z);
  };
  auto G = [](const uint32_t& X, const uint32_t& Y, const uint32_t& Z)->uint32_t{
    return (X & Z) | (Y & ~Z);
  };
  auto H = [](const uint32_t& X, const uint32_t& Y, const uint32_t& Z)->uint32_t{
    return X ^ Y ^ Z;
  };
  auto I = [](const uint32_t& X, const uint32_t& Y, const uint32_t& Z)->uint32_t{
    return Y ^ (X | ~Z);
  };

  // Process each 16-word block.
  for(int i = 0; i < bits.size()/16; ++i)
  {
    std::vector<bool> X;
    X.resize(255);

    // Copy block i into X.
    for(int j = 0; j < 16; ++j)
    {
      X[j] = bits[i * 16+j];
    }

    /* Save A as AA, B as BB, C as CC, and D as DD. */
    uint32_t AA = A;
    uint32_t BB = B;
    uint32_t CC = C;
    uint32_t DD = D;

    constexpr auto rotateShiftLeft = [](uint32_t x, uint32_t n)->uint32_t{
      return (x << n) | (x >> (32-n));
    };

    const auto FF = [&](uint32_t &a, uint32_t &b, uint32_t &c, uint32_t &d, uint32_t k, uint32_t s, uint32_t i){
      a = b + rotateShiftLeft((a + F(b, c, d) + X[k] + table[i]), s);
    };
    const auto GG = [&](uint32_t &a, uint32_t &b, uint32_t &c, uint32_t &d, uint32_t k, uint32_t s, uint32_t i){
      a = b + rotateShiftLeft((a + G(b, c, d) + X[k] + table[i]), s);
    };
    const auto HH = [&](uint32_t &a, uint32_t &b, uint32_t &c, uint32_t &d, uint32_t k, uint32_t s, uint32_t i){
      a = b + rotateShiftLeft((a + H(b, c, d) + X[k] + table[i]), s);
    };
    const auto II = [&](uint32_t &a, uint32_t &b, uint32_t &c, uint32_t &d, uint32_t k, uint32_t s, uint32_t i){
      a = b + rotateShiftLeft((a + I(b, c, d) + X[k] + table[i]), s);
    };

    const std::vector<std::vector<uint32_t>> coef1 = {{0, 7,  1}, {1, 12, 2}, {2, 17, 3}, {3, 22, 4},
                                                 {4, 7,  5}, {5, 12, 6}, {6, 17, 7}, {7, 22, 8},
                                                 {8, 7,  9}, {9, 12,10}, {10,17,11}, {11,22,12},
                                                 {12,7, 13}, {13,12,14}, {14,17,15}, {15,22,16}};
    const std::vector<std::vector<uint32_t>> coef2 = {{1, 5, 17}, {6, 9, 18}, {11,14,19}, {0 ,20,20},
                                                 {5, 5, 21}, {10,9, 22}, {15,14,23}, {4 ,20,24},
                                                 {9, 5, 25}, {14,9, 26}, {3, 14,27}, {8 ,20,28},
                                                 {13,5, 29}, {2 ,9, 30}, {7, 14,31}, {12,20,32}};
    const std::vector<std::vector<uint32_t>> coef3 = {{5 ,4, 33}, {8 ,11,34}, {11,16,35}, {14,23,36},
                                                 {1 ,4, 37}, {4 ,11,38}, {7 ,16,39}, {10,23,40},
                                                 {13,4, 41}, {0 ,11,42}, {3 ,16,43}, {6 ,23,44},
                                                 {9 ,4, 45}, {12,11,46}, {15,16,47}, {2 ,23,48}};
    const std::vector<std::vector<uint32_t>> coef4 = {{0, 6, 49}, {7, 10,50}, {14,15,51}, {5 ,21,52},
                                                 {12,6, 53}, {3, 10,54}, {10,15,55}, {1 ,21,56},
                                                 {8, 6, 57}, {15,10,58}, {6, 15,59}, {13,21,60},
                                                 {4, 6, 61}, {11,10,62}, {2, 15,63}, {9 ,21,64}};

    for(int i = 0; i < 4; ++i)
    {
      FF(A, B, C ,D, coef1[i*4+0][0], coef1[i*4+0][1], coef1[i*4+0][2]);
      FF(D, A, B, C, coef1[i*4+1][0], coef1[i*4+1][1], coef1[i*4+1][2]);
      FF(C, D, A, B, coef1[i*4+2][0], coef1[i*4+2][1], coef1[i*4+2][2]);
      FF(B, C, D, A, coef1[i*4+3][0], coef1[i*4+3][1], coef1[i*4+3][2]);
    }
    for(int i = 0; i < 4; ++i)
    {
      GG(A, B, C ,D, coef2[i*4+0][0], coef2[i*4+0][1], coef2[i*4+0][2]);
      GG(D, A, B, C, coef2[i*4+1][0], coef2[i*4+1][1], coef2[i*4+1][2]);
      GG(C, D, A, B, coef2[i*4+2][0], coef2[i*4+2][1], coef2[i*4+2][2]);
      GG(B, C, D, A, coef2[i*4+3][0], coef2[i*4+3][1], coef2[i*4+3][2]);
    }
    for(int i = 0; i < 4; ++i)
    {
      HH(A, B, C ,D, coef3[i*4+0][0], coef3[i*4+0][1], coef3[i*4+0][2]);
      HH(D, A, B, C, coef3[i*4+1][0], coef3[i*4+1][1], coef3[i*4+1][2]);
      HH(C, D, A, B, coef3[i*4+2][0], coef3[i*4+2][1], coef3[i*4+2][2]);
      HH(B, C, D, A, coef3[i*4+3][0], coef3[i*4+3][1], coef3[i*4+3][2]);
    }
    for(int i = 0; i < 4; ++i)
    {
      II(A, B, C ,D, coef4[i*4+0][0], coef4[i*4+0][1], coef4[i*4+0][2]);
      II(D, A, B, C, coef4[i*4+1][0], coef4[i*4+1][1], coef4[i*4+1][2]);
      II(C, D, A, B, coef4[i*4+2][0], coef4[i*4+2][1], coef4[i*4+2][2]);
      II(B, C, D, A, coef4[i*4+3][0], coef4[i*4+3][1], coef4[i*4+3][2]);
    }

    A = A + AA;
    B = B + BB;
    C = C + CC;
    D = D + DD;
  }

  std::stringstream stream;
  stream << std::setfill('0') << std::setw(sizeof(uint32_t)*2) << std::hex << D
         << std::setfill('0') << std::setw(sizeof(uint32_t)*2) << std::hex << C
         << std::setfill('0') << std::setw(sizeof(uint32_t)*2) << std::hex << B
         << std::setfill('0') << std::setw(sizeof(uint32_t)*2) << std::hex << A;
  return stream.str();
}

int main()
{
  std::string input = "bgvyzdsv"; //AoC input
  intmax_t number = 1;
  while(true)
  {
    std::string inputToTry = input + std::to_string(number);
    MD5 md5 = MD5(inputToTry);
    std::string hash = md5.hexdigest();
    std::cout<<inputToTry<<" -> "<<hash<<"\n"<<std::endl;
    if(hash.substr(0,6) == "000000")
    {
      break;
    }
    number++;
  }
  return 0;
}
