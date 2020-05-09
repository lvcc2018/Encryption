#include "cipher_machine.h"
#include <time.h>
#include <fstream>
#include <iostream>

using namespace std;

int cipher_machine::PC1_table[56] = {
    56, 48, 40, 32, 24, 16, 8,
    0, 57, 49, 41, 33, 25, 17,
    9, 1, 58, 50, 42, 34, 26,
    18, 10, 2, 59, 51, 43, 35,
    62, 54, 46, 38, 30, 22, 14,
    6, 61, 53, 45, 37, 29, 21,
    13, 5, 60, 52, 44, 36, 28,
    20, 12, 4, 27, 19, 11, 3};

int cipher_machine::move_table[16] = {23, 10, 2, 5, 9, 2, 3, 2, 3, 2, 5, 7, 2, 9, 2, 7};
int cipher_machine::PC2_table[48] = {
    13, 16, 10, 23, 0, 4, 2, 27,
    14, 5, 20, 9, 22, 18, 11, 3,
    25, 7, 15, 6, 26, 19, 12, 1,
    40, 51, 30, 36, 46, 54, 29, 39,
    50, 44, 32, 46, 43, 48, 38, 55,
    33, 52, 45, 41, 49, 35, 28, 31};

int cipher_machine::SBox_table[8][4][16] = {
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

int cipher_machine::IP_table[64] = {
    57, 49, 41, 33, 25, 17, 9, 1,
    59, 51, 43, 35, 27, 19, 11, 3,
    61, 53, 45, 37, 29, 21, 13, 5,
    63, 55, 47, 39, 31, 23, 15, 7,
    56, 48, 40, 32, 24, 16, 8, 0,
    58, 50, 42, 34, 26, 18, 10, 2,
    60, 52, 44, 36, 28, 20, 12, 4,
    62, 54, 46, 38, 30, 22, 14, 6};
int cipher_machine::anti_IP_table[64] = {
    39, 7, 47, 15, 55, 23, 63, 31,
    38, 6, 46, 14, 54, 22, 62, 30,
    37, 5, 45, 13, 53, 21, 61, 29,
    36, 4, 44, 12, 52, 20, 60, 28,
    35, 3, 43, 11, 51, 19, 59, 27,
    34, 2, 42, 10, 50, 18, 58, 26,
    33, 1, 41, 9, 49, 17, 57, 25,
    32, 0, 40, 8, 48, 16, 56, 24};

int cipher_machine::expand_table[48] = {
    31, 0, 1, 2, 3, 4,
    3, 4, 5, 6, 7, 8,
    7, 8, 9, 10, 11, 12,
    11, 12, 13, 14, 15, 16,
    15, 16, 17, 18, 19, 20,
    19, 20, 21, 22, 23, 24,
    23, 24, 25, 26, 27, 28,
    27, 28, 29, 30, 31, 0};
int cipher_machine::P_table[32] = {
    15, 6, 19, 20, 28, 11, 27, 16,
    0, 14, 22, 25, 4, 17, 30, 9,
    1, 7, 23, 13, 31, 26, 2, 8,
    18, 12, 29, 5, 21, 10, 3, 24};

cipher_machine::cipher_machine()
{
    plain_text = new string[256];
    srand((unsigned)time(NULL));
    key = rand();
    for (int i = 0; i < 16; i++)
    {
        subkey[i].resize(48);
    }
    create_subkey();
}

void cipher_machine::create_subkey()
{
    string PC1BitStr;
    PC1BitStr.resize(56);
    for (int i = 0; i < PC1BitStr.size(); i++)
        PC1BitStr[i] = key[PC1_table[i]];

    for (int i = 0; i < 16; i++)
    {
        LeftCycle(PC1BitStr, 0, 28, move_table[i]);
        LeftCycle(PC1BitStr, 28, 56, move_table[i]);
        for (int j = 0; j < 48; j++)
            subkey[i][j] = PC1BitStr[PC2_table[i]];
    }
}

void cipher_machine::LeftCycle(string str, int beginSection, int endSection, int step)
{
    int tmpStep = step % (endSection - beginSection);
    string tmpStr = str.substr(beginSection + tmpStep, endSection - beginSection - tmpStep);
    tmpStr.append(str.substr(beginSection, tmpStep));
    for (int i = beginSection; i < endSection; ++i)
        str[i] = tmpStr[i - beginSection];
}

void cipher_machine::encrypt()
{
    ifstream input_file("plain_text.txt");
    for (int i = 0; i < 256; i++)
    {
        plain_text[i].resize(64);
        input_file >> plain_text[i];
    }
    input_file.close();
    
    ofstream output_file("cipher_text.txt");
    for (int i = 0; i < 256; i++)
    {
        string temp = encrypt_block(plain_text[i]);
        output_file << temp;
        output_file << endl;
    }
    output_file.close();
}

void cipher_machine::decrypt()
{
    ifstream input_file("cipher_text.txt");
    for (int i = 0; i < 256; i++)
    {
        plain_text[i].resize(64);
        input_file >> plain_text[i];
    }
    input_file.close();
    
    ofstream output_file("translate_text.txt");
    for (int i = 0; i < 256; i++)
    {
        string temp = decrypt_block(plain_text[i]);
        output_file << temp;
        output_file << endl;
    }
    output_file.close();
}

string cipher_machine::encrypt_block(string block)
{
    string IP_str = IP_transform(block);
    string halfBitStr;
    halfBitStr.resize(32);
    std::string eBitStr;
    eBitStr.resize(48);

    for (int i = 0; i < 16; i++)
    {
        string ex_str = expand_transform(IP_str.substr(32));

        string xor_str = XOR_transform(ex_str, subkey[i], 48);

        string sbox_str = Sbox_transform(xor_str);

        string P_str = P_transform(sbox_str);

        string xor2_str = XOR_transform(IP_str, P_str, 32);

        IP_str = xor2_str;
    }

    string cipher_str = anti_IP_transform(IP_str);
    return cipher_str;
}

string cipher_machine::decrypt_block(string block)
{
    string IP_str = IP_transform(block);
    std::string halfBitStr;
    halfBitStr.resize(32);
    std::string eBitStr;
    eBitStr.resize(48);
    for (int i = 15; i >= 0; --i)
    {
        string ex_str = expand_transform(IP_str.substr(32));
        string xor_str = XOR_transform(ex_str, subkey[i], 48);
        string sbox_str = Sbox_transform(xor_str);
        string P_str = P_transform(sbox_str);
        string xor2_str = XOR_transform(IP_str, P_str, 32);
        IP_str = xor2_str;
    }
    string plain_str = anti_IP_transform(IP_str);
    return plain_str;
}

string cipher_machine::expand_transform(string block)
{
    string temp;
    temp.resize(48);
    for (int i = 0; i < 48; i++)
    {
        temp[i] = block[expand_table[i]];
    }
    return temp;
}

string cipher_machine::IP_transform(string block)
{
    std::string tmpBitStr;
    tmpBitStr.resize(64);
    for (size_t i = 0; i < 64; i++)
        tmpBitStr[i] = block[IP_table[i]];
    return tmpBitStr;
}

string cipher_machine::anti_IP_transform(string block)
{
    std::string tmpBitStr;
    tmpBitStr.resize(64);
    for (size_t i = 0; i < 64; i++)
        tmpBitStr[i] = block[anti_IP_table[i]];
    return tmpBitStr;
}

string cipher_machine::XOR_transform(string block1, string block2, int length)
{
    string temp;
    temp.resize(length);
    for (int i = 0; i < length; i++)
        temp[i] = block1[i] ^ block2[i];
    return temp;
}

string cipher_machine::Sbox_transform(string block)
{
    string temp;
    temp.resize(32);
    for (size_t i = 0; i < 8; ++i)
    {
        size_t j = i * 6;
        size_t row = (block[j] << 1) + block[j + block.size() / 8 - 1];
        size_t column = (block[j + 1] << 3) + (block[j + 2] << 2) + (block[j + 3] << 1) + block[j + 4];

        int x = SBox_table[i][row][column];

        temp[i * 4] = x >> 3;
        temp[i * 4 + 1] = (x >> 2) & 0x1;
        temp[i * 4 + 2] = (x >> 1) & 0x1;
        temp[i * 4 + 3] = x & 0x1;
    }
    return temp;
}

string cipher_machine::P_transform(string block)
{

    std::string temp;
    temp.resize(32);

    for (size_t i = 0; i < 32; ++i)
        temp[i] = block[P_table[i]];

    return temp;
}