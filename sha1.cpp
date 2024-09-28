#include <iostream>
#include <iomanip>
#include <bitset>
#include <cmath>
#include <cstring>
#include <sstream>
#include <vector>
using namespace std;

int count_digit(int number)
{
   return int(log10(number) + 1);
}

uint32_t circular_shift(uint32_t value, unsigned int count)
{
   return value << count | value >> (32 - count);
}

std::string int_to_hex(u_int32_t integer)
{
   std::stringstream ss;
   ss << std::hex << integer;
   string result(ss.str());
   return result;
}

uint32_t f0(uint32_t B, uint32_t C, uint32_t D)
{
   return (B & C) | ((~B) & D);
}

uint32_t f1(uint32_t B, uint32_t C, uint32_t D)
{
   return B ^ C ^ D;
}

uint32_t f2(uint32_t B, uint32_t C, uint32_t D)
{
   return (B & C) | (B & D) | (C & D);
}

uint32_t f3(uint32_t B, uint32_t C, uint32_t D)
{
   return B ^ C ^ D;
}

const int MSG_SIZE = 448;
const int MSG_FINAL_SIZE = 512;

const uint32_t K[] = {/* Constants defined in SHA-1   */
                      0x5A827999,
                      0x6ED9EBA1,
                      0x8F1BBCDC,
                      0xCA62C1D6};

uint32_t H0 = 0x67452301;
uint32_t H1 = 0xEFCDAB89;
uint32_t H2 = 0x98BADCFE;
uint32_t H3 = 0x10325476;
uint32_t H4 = 0xC3D2E1F0;

int main()
{

   uint32_t temp;          /* Temporary word value        */
   uint32_t A, B, C, D, E; /* Word buffers                */
   u_int32_t W[80];        /* Word sequence               */

   string msg = "hello world"; // Message to hash
   string padded_msg = "";
   u_int msg_no_padding_length = 0;
   vector<bitset<32>> words;

   // convert msg to binari string
   for (char i : msg)
   {
      bitset bts = bitset<8>(i);
      padded_msg += bts.to_string();
   }

   // get msg lengh
   msg_no_padding_length = padded_msg.length();

   // add 1 to the end of the string
   padded_msg += '1';

   u_int temp_size = padded_msg.size();
   // complete msg length with ceroes
   for (int i = 0; i < (MSG_SIZE - temp_size); i++)
   {
      padded_msg += '0';
   }

   // complete the msg to its final size and add the msg size at the end
   string s = bitset<8>(msg_no_padding_length).to_string();
   u_int _size = s.size();
   for (int i = 0; i < (MSG_FINAL_SIZE - MSG_SIZE - _size); i++)
   {
      padded_msg += '0';
   }
   padded_msg += s;

   // step a - devide into 16 words
   for (int i = 0; i < MSG_FINAL_SIZE; i += 32)
   {
      string sb = padded_msg.substr(i, 32);

      bitset bits = bitset<32>(sb);
      words.push_back(bits);
   }
   // convert from bitset to int and add it to W
   for (int i = 0; i < 16; i++)
   {
      W[i] = (u_int32_t)words[i].to_ulong();
   }

   // perform step b
   for (int t = 16; t < 80; t++)
   {
      W[t] = circular_shift((W[t - 3] ^ W[t - 8] ^ W[t - 14] ^ W[t - 16]), 1);
   }

   // step c
   A = H0;
   B = H1;
   C = H2;
   D = H3;
   E = H4;

   // step d
   for (int t = 0; t < 20; t++)
   {
      temp = circular_shift(A, 5) + f0(B, C, D) + E + W[t] + K[0];
      E = D;
      D = C;
      C = circular_shift(B, 30);
      B = A;
      A = temp;
   }

   for (int t = 20; t < 40; t++)
   {
      temp = circular_shift(A, 5) + f1(B, C, D) + E + W[t] + K[1];
      E = D;
      D = C;
      C = circular_shift(B, 30);
      B = A;
      A = temp;
   }

   for (int t = 40; t < 60; t++)
   {
      temp = circular_shift(A, 5) + f2(B, C, D) + E + W[t] + K[2];
      E = D;
      D = C;
      C = circular_shift(B, 30);
      B = A;
      A = temp;
   }

   for (int t = 60; t < 80; t++)
   {
      temp = circular_shift(A, 5) + f3(B, C, D) + E + W[t] + K[3];
      E = D;
      D = C;
      C = circular_shift(B, 30);
      B = A;
      A = temp;
   }

   // step e
   H0 = H0 + A;
   H1 = H1 + B;
   H2 = H2 + C;
   H3 = H3 + D;
   H4 = H4 + E;

   cout << "hash result: ";
   cout << hex << std::setfill('0') << std::setw(8) << H0;
   cout << hex << std::setfill('0') << std::setw(8) << H1;
   cout << hex << std::setfill('0') << std::setw(8) << H2;
   cout << hex << std::setfill('0') << std::setw(8) << H3;
   cout << hex << std::setfill('0') << std::setw(8) << H4 << endl
        << endl;
}
