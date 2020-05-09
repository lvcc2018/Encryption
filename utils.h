#ifndef UTILS
#define UTILS

#include <fstream>
#include <time.h>
#include <iostream>

using namespace std;

void rand_plain_text()
{
    ofstream out_file("plain_text.txt");
    srand((unsigned)time(NULL));
    for (int i = 0; i < 256 * 64; i++)
    {
        char temp = (rand() % 26) + 65;
        out_file << temp;
    }
    out_file.close();
    return;
}

uint16_t polynomialMutil(int a, int b)
{
    uint16_t tmp[8] = {0};
    int i;
    for (i = 0; i < 8; i++)
    {
        tmp[i] = (a << i) * ((b >> i) & 0x1);
    }

    tmp[0] = tmp[0] ^ tmp[1] ^ tmp[2] ^ tmp[3] ^ tmp[4] ^ tmp[5] ^ tmp[6] ^ tmp[7];

    return tmp[0];
}

int findHigherBit(uint16_t val)
{
    int i = 0;
    while (val)
    {
        i++;
        val = val >> 1;
    }
    return i;
}

//GF(2^8)的多项式除法
int gf28_div(uint16_t div_ed, uint16_t div, uint16_t *remainder)
{
    uint16_t r0 = 0;
    int qn = 0;
    int bitCnt = 0;

    r0 = div_ed;

    bitCnt = findHigherBit(r0) - findHigherBit(div);
    while (bitCnt >= 0)
    {
        qn = qn | (1 << bitCnt);
        r0 = r0 ^ (div << bitCnt);
        bitCnt = findHigherBit(r0) - findHigherBit(div);
    }
    *remainder = r0;
    return qn;
}
//GF(2^8)多项式的扩展欧几里得算法
int extEuclidPolynomial(int a, uint16_t m)
{
    uint16_t r0, r1, r2;
    int qn, v0, v1, v2, w0, w1, w2;

    r0 = m;
    r1 = a;

    v0 = 1;
    v1 = 0;

    w0 = 0;
    w1 = 1;

    while (r1 != 1)
    {
        qn = gf28_div(r0, r1, &r2);

        v2 = v0 ^ polynomialMutil(qn, v1);
        w2 = w0 ^ polynomialMutil(qn, w1);

        r0 = r1;
        r1 = r2;

        v0 = v1;
        v1 = v2;

        w0 = w1;
        w1 = w2;
    }
    return w1;
}

//S盒字节变换
int byteTransformation(int a, int x)
{
    int tmp[8] = {0};

    for (int i = 0; i < 8; i++)
    {
        tmp[i] = (((a >> i) & 0x1) ^ ((a >> ((i + 4) % 8)) & 0x1) ^ ((a >> ((i + 5) % 8)) & 0x1) ^ ((a >> ((i + 6) % 8)) & 0x1) ^ ((a >> ((i + 7) % 8)) & 0x1) ^ ((x >> i) & 0x1)) << i;
    }
    tmp[0] = tmp[0] + tmp[1] + tmp[2] + tmp[3] + tmp[4] + tmp[5] + tmp[6] + tmp[7];
    return tmp[0];
}

//逆S盒字节变换
int invByteTransformation(int a, int x)
{
    int tmp[8] = {0};

    for (int i = 0; i < 8; i++)
    {
        tmp[i] = (((a >> ((i + 2) % 8)) & 0x1) ^ ((a >> ((i + 5) % 8)) & 0x1) ^ ((a >> ((i + 7) % 8)) & 0x1) ^ ((x >> i) & 0x1)) << i;
    }
    tmp[0] = tmp[0] + tmp[1] + tmp[2] + tmp[3] + tmp[4] + tmp[5] + tmp[6] + tmp[7];
    return tmp[0];
}

std::string string_xor(string str1, string str2, int length)
{ 
    string temp;
    temp.resize(length);
    for(int i=0;i<length;i++)
    {
        temp[i] = str1[i]^str2[i];
    }
    return temp;
}

void left_cycle(string& str, int length, int dis)
{
    string temp = str.substr(0, dis);
    for(int i=0;i<length-dis;i++)
    {
        str[i] = str[i+dis];
    }
    for(int i=0;i<dis;i++)
    {
        str[length-dis+i] = temp[i];
    }
    return;
}



#endif