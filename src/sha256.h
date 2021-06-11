#ifndef _ACL_SHA256_
#define _ACL_SHA256_

#include <cstdlib>
#include <iostream>
#include <stdlib.h>
#include <fstream>

typedef char s8;
typedef short s16;
typedef int s32;
typedef long long s64;
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long u64;

struct Sha256Value
{
     u32 subHashes[8];
     Sha256Value(){
          subHashes[0] = 0x6A09E667;
          subHashes[1] = 0xBB67AE85;
          subHashes[2] = 0x3C6EF372;
          subHashes[3] = 0xA54FF53A;
          subHashes[4] = 0x510E527F;
          subHashes[5] = 0x9B05688C;
          subHashes[6] = 0x1F83D9AB;
          subHashes[7] = 0x5BE0CD19;
     }
     void Print(){
          std::cout << std::hex;
          for (u32 i = 0; i < 8; i++)
               for (u32 j = 0; j < 8; j++)
                    std::cout << ((subHashes[i] >> (28 - 4 * j)) & 0xf);
          std::cout << std::dec << std::endl;
     }
};

class Sha256{
     static u32 Sigma0(u32 val){
          return ((_rotr(val, 7)) ^ (_rotr(val, 18))) ^ (val >> 3);
     }
     static u32 Sigma1(u32 val){
          return ((_rotr(val, 17)) ^ (_rotr(val, 19))) ^ (val >> 10);
     }

     static u32 UpSigma0(u32 val){
          return ((_rotr(val, 2)) ^ (_rotr(val, 13))) ^ (_rotr(val, 22));
     }

     static u32 UpSigma1(u32 val){
          return ((_rotr(val, 6)) ^ (_rotr(val, 11))) ^ (_rotr(val, 25));
     }

public:
     static Sha256Value GetHash(const u8 *data, u32 size){
          const u32 k[64] =
              {0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5,
               0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
               0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3,
               0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
               0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc,
               0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
               0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7,
               0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
               0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13,
               0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
               0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3,
               0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
               0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5,
               0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
               0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208,
               0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2};

          Sha256Value hash = Sha256Value();

          u64 sizeBit = size * 8;
          u32 extendedSizeBit = sizeBit + 8 + 64;
          u32 alignedSizeBit = extendedSizeBit + (512 - (extendedSizeBit % 512));
          u32 blocksCount = alignedSizeBit / 512;

          for (u32 i = 0; i < blocksCount; i++){
               u8 buf[64];
               u32 w[64];
               for (u32 j = 0, l = i * 64; j < 64; j++, l++){
                    if (l < size) buf[j] = data[l];
                    else if (l == size) buf[j] = (1 << 7);
                    else buf[j] = 0;
               }
               if (i == (blocksCount - 1))
                    for (u32 i = 0; i < 8; i++) buf[63 - i] = (sizeBit >> (8 * i)) & 0xff;

               for (u32 j = 0; j < 64; j++) ((u8 *)w)[j] = buf[j + 3 - ((j & 3) << 1)];
               for (u32 j = 16; j < 64; j++) w[j] = w[j - 16] + Sigma0(w[j - 15]) + w[j - 7] + Sigma1(w[j - 2]);

               u32 _h[8];
               for (u32 i = 0; i < 8; i++) _h[i] = hash.subHashes[i];
               for (u32 j = 0; j < 64; j++){
                    u32 ch = (_h[4] & _h[5]) ^ ((~_h[4]) & _h[6]);
                    u32 maj = (_h[0] & _h[1]) ^ (_h[0] & _h[2]) ^ (_h[1] & _h[2]);
                    u32 temp1 = _h[7] + UpSigma1(_h[4]) + ch + k[j] + w[j];
                    u32 temp2 = UpSigma0(_h[0]) + maj;
                    for (int i = 7; i > 0; i--) _h[i] = _h[i - 1];
                    _h[4] += temp1;
                    _h[0] = temp1 + temp2;
               }
               for (u32 i = 0; i < 8; i++) hash.subHashes[i] += _h[i];
          }
          return hash;
     }
};

#endif
