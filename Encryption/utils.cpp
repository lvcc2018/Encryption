#include "utils.h"
using namespace std;

unsigned int string_to_unsigned_int(string str)
{
    unsigned int result(0);
    for (int i = str.size() - 1; i >= 0; i--)
    {
        unsigned int temp(0), k = str.size() - i - 1;
        if (isdigit(str[i]))
        {
            temp = str[i] - '0';
            while (k--)
                temp *= 10;
            result += temp;
        }
        else
            //exit(-1);
            break;
    }
    return result;
}

void rand_plain_text()
{
    ofstream out_file("plain_text.txt");
    srand((unsigned)time(NULL));
    for (int i = 0; i < 256 * 8; i++)
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
    for (int i = 0; i < length; i++)
    {
        temp[i] = str1[i] ^ str2[i];
    }
    return temp;
}

string string_xor_ul(string str, unsigned int integer)
{
    string temp;
    temp.resize(4);
    for (int i = 0; i < 4; i++)
    {
        temp[i] = str[i] ^ (integer >> (i * 8));
    }
    return temp;
}

void left_cycle(string &str, int length, int dis)
{
    string temp = str.substr(0, dis);
    for (int i = 0; i < length - dis; i++)
    {
        str[i] = str[i + dis];
    }
    for (int i = 0; i < dis; i++)
    {
        str[length - dis + i] = temp[i];
    }
    return;
}

void leftLoop4int(int array[4], int step)
{
    int temp[4];
    for (int i = 0; i < 4; i++)
        temp[i] = array[i];

    int index = step % 4 == 0 ? 0 : step % 4;
    for (int i = 0; i < 4; i++)
    {
        array[i] = temp[index];
        index++;
        index = index % 4;
    }
}

string leftLoopBit(string str, int dis)
{
    unsigned int str_int = string_to_unsigned_int(str);
    str_int = (str_int << dis) | (str_int >> (32 - dis));
    string str_result = to_string(str_int);
    return str_result;
}

void rightLoop4int(int array[4], int step)
{
    int temp[4];
    for (int i = 0; i < 4; i++)
        temp[i] = array[i];

    int index = step % 4 == 0 ? 0 : step % 4;
    index = 3 - index;
    for (int i = 3; i >= 0; i--)
    {
        array[i] = temp[index];
        index--;
        index = index == -1 ? 3 : index;
    }
}

int GFMul2(int s)
{
    int result = s << 1;
    int a7 = result & 0x00000100;

    if (a7 != 0)
    {
        result = result & 0x000000ff;
        result = result ^ 0x1b;
    }

    return result;
}

int GFMul3(int s)
{
    return GFMul2(s) ^ s;
}

int GFMul4(int s)
{
    return GFMul2(GFMul2(s));
}

int GFMul8(int s)
{
    return GFMul2(GFMul4(s));
}

int GFMul9(int s)
{
    return GFMul8(s) ^ s;
}

int GFMul11(int s)
{
    return GFMul9(s) ^ GFMul2(s);
}

int GFMul12(int s)
{
    return GFMul8(s) ^ GFMul4(s);
}

int GFMul13(int s)
{
    return GFMul12(s) ^ s;
}

int GFMul14(int s)
{
    return GFMul12(s) ^ GFMul2(s);
}

/**
 * GF上的二元运算
 */
int GFMul(int n, int s)
{
    int result;

    if (n == 1)
        result = s;
    else if (n == 2)
        result = GFMul2(s);
    else if (n == 3)
        result = GFMul3(s);
    else if (n == 0x9)
        result = GFMul9(s);
    else if (n == 0xb) //11
        result = GFMul11(s);
    else if (n == 0xd) //13
        result = GFMul13(s);
    else if (n == 0xe) //14
        result = GFMul14(s);

    return result;
}
