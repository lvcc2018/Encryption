#ifndef UTILS_H
#define UTILS_H

#include <fstream>
#include <time.h>
#include <iostream>

using namespace std;

unsigned int string_to_unsigned_int(string str);
void rand_plain_text();

uint16_t polynomialMutil(int a, int b);

int findHigherBit(uint16_t val);

//GF(2^8)的多项式除法
int gf28_div(uint16_t div_ed, uint16_t div, uint16_t *remainder);
//GF(2^8)多项式的扩展欧几里得算法
int extEuclidPolynomial(int a, uint16_t m);

//S盒字节变换
int byteTransformation(int a, int x);

//逆S盒字节变换
int invByteTransformation(int a, int x);

std::string string_xor(string str1, string str2, int length);

string string_xor_ul(string str, unsigned int integer);

void left_cycle(string &str, int length, int dis);

void leftLoop4int(int array[4], int step);

string leftLoopBit(string str, int dis);

void rightLoop4int(int array[4], int step);

static const int colM[4][4] = {2, 3, 1, 1,
                               1, 2, 3, 1,
                               1, 1, 2, 3,
                               3, 1, 1, 2};
const int deColM[4][4] = {0xe, 0xb, 0xd, 0x9,
                          0x9, 0xe, 0xb, 0xd,
                          0xd, 0x9, 0xe, 0xb,
                          0xb, 0xd, 0x9, 0xe};

int GFMul2(int s);

int GFMul3(int s);

int GFMul4(int s);

int GFMul8(int s);

int GFMul9(int s);

int GFMul11(int s);

int GFMul12(int s);
int GFMul13(int s);

int GFMul14(int s);

int GFMul(int n, int s);

#endif