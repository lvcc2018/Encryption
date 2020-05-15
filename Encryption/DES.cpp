
#include "DES.h"
using namespace std;
int DES::PC1_table[56] = {
    56, 48, 40, 32, 24, 16, 8,
    0, 57, 49, 41, 33, 25, 17,
    9, 1, 58, 50, 42, 34, 26,
    18, 10, 2, 59, 51, 43, 35,
    62, 54, 46, 38, 30, 22, 14,
    6, 61, 53, 45, 37, 29, 21,
    13, 5, 60, 52, 44, 36, 28,
    20, 12, 4, 27, 19, 11, 3};

int DES::Move_table[16] = {23, 10, 2, 5, 9, 2, 3, 2, 3, 2, 5, 7, 2, 9, 2, 7};
int DES::PC2_table[48] = {
    13, 16, 10, 23, 0, 4, 2, 27,
    14, 5, 20, 9, 22, 18, 11, 3,
    25, 7, 15, 6, 26, 19, 12, 1,
    40, 51, 30, 36, 46, 54, 29, 39,
    50, 44, 32, 46, 43, 48, 38, 55,
    33, 52, 45, 41, 49, 35, 28, 31};

int DES::IP_table[64] = {
    57, 49, 41, 33, 25, 17, 9, 1,
    59, 51, 43, 35, 27, 19, 11, 3,
    61, 53, 45, 37, 29, 21, 13, 5,
    63, 55, 47, 39, 31, 23, 15, 7,
    56, 48, 40, 32, 24, 16, 8, 0,
    58, 50, 42, 34, 26, 18, 10, 2,
    60, 52, 44, 36, 28, 20, 12, 4,
    62, 54, 46, 38, 30, 22, 14, 6};

int DES::Expand_table[48] = {
    31, 0, 1, 2, 3, 4,
    3, 4, 5, 6, 7, 8,
    7, 8, 9, 10, 11, 12,
    11, 12, 13, 14, 15, 16,
    15, 16, 17, 18, 19, 20,
    19, 20, 21, 22, 23, 24,
    23, 24, 25, 26, 27, 28,
    27, 28, 29, 30, 31, 0};

int DES::Permute_table[32] = {
    15, 6, 19, 20, 28, 11, 27, 16,
    0, 14, 22, 25, 4, 17, 30, 9,
    1, 7, 23, 13, 31, 26, 2, 8,
    18, 12, 29, 5, 21, 10, 3, 24};

int DES::IP_1_table[64] = {
    39, 7, 47, 15, 55, 23, 63, 31,
    38, 6, 46, 14, 54, 22, 62, 30,
    37, 5, 45, 13, 53, 21, 61, 29,
    36, 4, 44, 12, 52, 20, 60, 28,
    35, 3, 43, 11, 51, 19, 59, 27,
    34, 2, 42, 10, 50, 18, 58, 26,
    33, 1, 41, 9, 49, 17, 57, 25,
    32, 0, 40, 8, 48, 16, 56, 24};

int DES::SBox_table[8][4][16] = {
    //S1
    {{14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7},
     {0, 15, 7, 4, 14, 2, 13, 1, 10, 6, 12, 11, 9, 5, 3, 8},
     {4, 1, 14, 8, 13, 6, 2, 11, 15, 12, 9, 7, 3, 10, 5, 0},
     {15, 12, 8, 2, 4, 9, 1, 7, 5, 11, 3, 14, 10, 0, 6, 13}},
    //S2
    {{15, 1, 8, 14, 6, 11, 3, 4, 9, 7, 2, 13, 12, 0, 5, 10},
     {3, 13, 4, 7, 15, 2, 8, 14, 12, 0, 1, 10, 6, 9, 11, 5},
     {0, 14, 7, 11, 10, 4, 13, 1, 5, 8, 12, 6, 9, 3, 2, 15},
     {13, 8, 10, 1, 3, 15, 4, 2, 11, 6, 7, 12, 0, 5, 14, 9}},
    //S3
    {{10, 0, 9, 14, 6, 3, 15, 5, 1, 13, 12, 7, 11, 4, 2, 8},
     {13, 7, 0, 9, 3, 4, 6, 10, 2, 8, 5, 14, 12, 11, 15, 1},
     {13, 6, 4, 9, 8, 15, 3, 0, 11, 1, 2, 12, 5, 10, 14, 7},
     {1, 10, 13, 0, 6, 9, 8, 7, 4, 15, 14, 3, 11, 5, 2, 12}},
    //S4
    {{7, 13, 14, 3, 0, 6, 9, 10, 1, 2, 8, 5, 11, 12, 4, 15},
     {13, 8, 11, 5, 6, 15, 0, 3, 4, 7, 2, 12, 1, 10, 14, 9},
     {10, 6, 9, 0, 12, 11, 7, 13, 15, 1, 3, 14, 5, 2, 8, 4},
     {3, 15, 0, 6, 10, 1, 13, 8, 9, 4, 5, 11, 12, 7, 2, 14}},
    //S5
    {{2, 12, 4, 1, 7, 10, 11, 6, 8, 5, 3, 15, 13, 0, 14, 9},
     {14, 11, 2, 12, 4, 7, 13, 1, 5, 0, 15, 10, 3, 9, 8, 6},
     {4, 2, 1, 11, 10, 13, 7, 8, 15, 9, 12, 5, 6, 3, 0, 14},
     {11, 8, 12, 7, 1, 14, 2, 13, 6, 15, 0, 9, 10, 4, 5, 3}},
    //S6
    {{12, 1, 10, 15, 9, 2, 6, 8, 0, 13, 3, 4, 14, 7, 5, 11},
     {10, 15, 4, 2, 7, 12, 9, 5, 6, 1, 13, 14, 0, 11, 3, 8},
     {9, 14, 15, 5, 2, 8, 12, 3, 7, 0, 4, 10, 1, 13, 11, 6},
     {4, 3, 2, 12, 9, 5, 15, 10, 11, 14, 1, 7, 6, 0, 8, 13}},
    //S7
    {{4, 11, 2, 14, 15, 0, 8, 13, 3, 12, 9, 7, 5, 10, 6, 1},
     {13, 0, 11, 7, 4, 9, 1, 10, 14, 3, 5, 12, 2, 15, 8, 6},
     {1, 4, 11, 13, 12, 3, 7, 14, 10, 15, 6, 8, 0, 5, 9, 2},
     {6, 11, 13, 8, 1, 4, 10, 7, 9, 5, 0, 15, 14, 2, 3, 12}},
    //S8
    {{13, 2, 8, 4, 6, 15, 11, 1, 10, 9, 3, 14, 5, 0, 12, 7},
     {1, 15, 13, 8, 10, 3, 7, 4, 12, 5, 6, 11, 0, 14, 9, 2},
     {7, 11, 4, 1, 9, 12, 14, 2, 0, 6, 10, 13, 15, 3, 5, 8},
     {2, 1, 14, 7, 4, 10, 8, 13, 15, 12, 9, 0, 3, 5, 6, 11}}};

string DES::get_key()
{
    return key;
}

DES::DES()
{
    srand((unsigned)time(NULL));
    char key_chr[8];
    for (int i = 0; i < 8; i++)
    {
        key_chr[i] = (rand() % 26) + 65;
    }
    key = key_chr;
    key.resize(8);
    create_subkey();
}
DES::~DES() {}
string DES::encrypt(const string &plain)
{
    string result;
    for (size_t i = 0; i < plain.size() / 8; ++i)
    {
        string block = plain.substr(i * 8, 8);
        encrypt_block(block);
        result.append(block);
    }
    int r = plain.size() % 8;
    if (r)
    {
        string block = plain.substr(plain.size() - r, r);
        block.append(8 - r, '\0');
        encrypt_block(block);
        result.append(block);
    }
    return result;
}

string DES::decrypt(const string &cipher)
{
    string result;
    if (cipher.empty() || key.empty())
        return result;
    if (cipher.size() % 8)
        return result;
    for (size_t i = 0; i < cipher.size() / 8; ++i)
    {
        string block = cipher.substr(i * 8, 8);
        decrypt_block(block);
        result.append(block);
    }
    return result;
}

bool DES::encrypt_block(string &block)
{
    if (block.size() != 8)
        return false;
    string bs;
    bs.resize(64);
    Char8ToBit64(block, bs);
    IP_transform(bs);
    string hbs;
    hbs.resize(bs.size() / 2);
    string ebs;
    ebs.resize(48);
    for (size_t i = 0; i < 16; ++i)
    {
        Expand_transform(bs.substr(bs.size() / 2), ebs);
        string temp_key = subkey[i];
        XOR(ebs, temp_key, 48);
        SBox_transform(ebs, hbs);
        Permute_transform(hbs);
        XOR(bs, hbs, hbs.size());
        if (i != 16 - 1)
            LeftCycle(bs, 0, bs.size(), bs.size() / 2);
    }
    IP_1_transform(bs);
    Bit64ToChar8(bs, block);
    return true;
}

bool DES::decrypt_block(string &block)
{
    if (block.size() != 8)
        return false;

    string bs;
    bs.resize(64);
    Char8ToBit64(block, bs);
    IP_transform(bs);
    string hbs;
    hbs.resize(bs.size() / 2);
    string ebs;
    ebs.resize(48);
    for (int i = 16 - 1; i >= 0; --i)
    {
        Expand_transform(bs.substr(bs.size() / 2), ebs);
        string temp_key = subkey[i];
        XOR(ebs, temp_key, 48);
        SBox_transform(ebs, hbs);
        Permute_transform(hbs);
        XOR(bs, hbs, hbs.size());
        if (i != 0)
            LeftCycle(bs, 0, bs.size(), bs.size() / 2);
    }
    IP_1_transform(bs);
    Bit64ToChar8(bs, block);
    return true;
}

bool DES::create_subkey()
{
    string tmpKey(key);
    if (tmpKey.size() < 8)
        tmpKey.append(8 - tmpKey.size(), '\0');
    else if (tmpKey.size() > 8)
        tmpKey = tmpKey.substr(0, 8);

    string bs;
    bs.resize(64);
    Char8ToBit64(tmpKey, bs);

    string PC1bs;
    PC1bs.resize(56);
    if (!PC1_transform(bs, PC1bs))
        return false;

    for (int i = 0; i < 16; ++i)
    {
        LeftCycle(PC1bs, 0, 56 / 2, Move_table[i]);
        LeftCycle(PC1bs, 56 / 2, 56, Move_table[i]);
        PC2_transform(PC1bs, subkey[i]);
    }

    return true;
}

bool DES::PC1_transform(const string &bs, string &PC1bs)
{
    if (bs.size() != 64 || PC1bs.size() != 56)
        return false;

    for (size_t i = 0; i < PC1bs.size(); ++i)
        PC1bs[i] = bs[PC1_table[i]];

    return true;
}

bool DES::PC2_transform(const string &PC1bs, char subKey[48])
{
    if (PC1bs.size() != 56)
        return false;

    for (size_t i = 0; i < 48; ++i)
        subKey[i] = PC1bs[PC2_table[i]];

    return true;
}

bool DES::IP_transform(string &bs)
{
    if (bs.size() != 64)
        return false;

    string tmpbs;
    tmpbs.resize(bs.size());
    for (size_t i = 0; i < bs.size(); ++i)
        tmpbs[i] = bs[IP_table[i]];

    bs.swap(tmpbs);

    return true;
}

bool DES::Expand_transform(const string &hbs, string &ebs)
{
    if (hbs.size() != 64 / 2 || ebs.size() != 48)
        return false;

    for (size_t i = 0; i < ebs.size(); ++i)
        ebs[i] = hbs[Expand_table[i]];

    return true;
}

bool DES::SBox_transform(const string &ebs, string &hbs)
{
    if (ebs.size() != 48 || hbs.size() != 64 / 2)
        return false;

    for (size_t i = 0; i < 8; ++i)
    {
        size_t j = i * 6;
        size_t row = (ebs[j] << 1) + ebs[j + ebs.size() / 8 - 1];
        size_t column = (ebs[j + 1] << 3) + (ebs[j + 2] << 2) + (ebs[j + 3] << 1) + ebs[j + 4];
        int x = SBox_table[i][row][column];
        hbs[i * 4] = x >> 3;
        hbs[i * 4 + 1] = (x >> 2) & 0x1;
        hbs[i * 4 + 2] = (x >> 1) & 0x1;
        hbs[i * 4 + 3] = x & 0x1;
    }
    return true;
}

bool DES::Permute_transform(string &hbs)
{
    if (hbs.size() != 64 / 2)
        return false;

    string tmpStr;
    tmpStr.resize(hbs.size());

    for (size_t i = 0; i < hbs.size(); ++i)
        tmpStr[i] = hbs[Permute_table[i]];

    hbs.swap(tmpStr);

    return true;
}

bool DES::IP_1_transform(string &bs)
{
    if (bs.size() != 64)
        return false;

    string tmpStr;
    tmpStr.resize(64);
    for (size_t i = 0; i < bs.size(); ++i)
        tmpStr[i] = bs[IP_1_table[i]];

    bs.swap(tmpStr);

    return true;
}

bool DES::Char8ToBit64(const string &str, string &bs)
{
    if (str.size() != 8 || bs.size() != 64)
        return false;

    for (size_t i = 0; i < str.size(); ++i)
    {
        for (size_t j = 0; j < 8; ++j)
            bs[i * 8 + j] = (str[i] >> j) & 0x1;
    }
    return true;
}

bool DES::Bit64ToChar8(const string &bs, string &str)
{
    if (bs.size() < 64 || str.size() != 8)
        return false;

    str = "";
    str.resize(8);

    for (size_t i = 0; i < 8; ++i)
    {
        for (size_t j = 0; j < 8; ++j)
            str[i] |= bs[i * 8 + j] << j;
    }

    return true;
}

bool DES::LeftCycle(string &str, size_t beginSection, size_t endSection, size_t step)
{
    if (endSection > str.size())
        return false;

    size_t tmpStep = step % (endSection - beginSection);
    string tmpStr = str.substr(beginSection + tmpStep, endSection - beginSection - tmpStep);
    tmpStr.append(str.substr(beginSection, tmpStep));
    for (size_t i = beginSection; i < endSection; ++i)
        str[i] = tmpStr[i - beginSection];
    return true;
}

bool DES::XOR(string &str1, string &str2, size_t num)
{
    if (str1.size() < num || str2.size() < num)
        return false;
    for (size_t i = 0; i < num; ++i)
        str1[i] ^= str2[i];
    return true;
}