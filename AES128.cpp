#include "AES128.h"
#include <time.h>
#include "utils.h"

int AES128::Rcon[10] = {0x01, 0x02,
                        0x04, 0x08,
                        0x10, 0x20,
                        0x40, 0x80,
                        0x1b, 0x36};

void AES128::create_S_table()
{
    int i, j;
    //初始化S盒
    for (i = 0; i < 0x10; i++)
    {
        for (j = 0; j < 0x10; j++)
        {
            S_table[i][j] = ((i << 4) & 0xF0) + (j & (0xF));
        }
    }
    //求在GF(2^8)域上的逆，0映射到自身
    printf("\r\n");
    for (i = 0; i < 0x10; i++)
    {
        for (j = 0; j < 0x10; j++)
        {
            if (S_table[i][j] != 0)
            {
                S_table[i][j] = extEuclidPolynomial(S_table[i][j], 0x11B);
            }
        }
    }

    //对每个字节做变换
    for (i = 0; i < 0x10; i++)
    {
        for (j = 0; j < 0x10; j++)
        {
            S_table[i][j] = byteTransformation(S_table[i][j], 0x63);
        }
    }
}

//逆S盒产生
void AES128::create_anti_S_table()
{
    int i, j;
    int b = 0, bb = 0;

    //初始化S盒
    for (i = 0; i < 0x10; i++)
    {
        for (j = 0; j < 0x10; j++)
        {
            anti_S_table[i][j] = ((i << 4) & 0xF0) + (j & (0xF));
        }
    }
    //对每个字节做变换
    for (i = 0; i < 0x10; i++)
    {
        for (j = 0; j < 0x10; j++)
        {
            anti_S_table[i][j] = invByteTransformation(anti_S_table[i][j], 0x05);
        }
    }

    //求在GF(2^8)域上的逆，0映射到自身
    printf("\r\n");
    for (i = 0; i < 0x10; i++)
    {
        for (j = 0; j < 0x10; j++)
        {
            if (anti_S_table[i][j] != 0)
            {
                anti_S_table[i][j] = extEuclidPolynomial(anti_S_table[i][j], 0x11B);
            }
        }
    }
}

void AES128::S_transform(string &str, int length)
{
    int xidx = 0, yidx = 0;
    for (int i = 0; i < length; i++)
    {
        xidx = (str[i] & 0xf0) >> 4;
        yidx = str[i] & 0xf;
        str[i] = str[S_table[xidx][yidx]];
    }
    return;
}

void AES128::anti_S_transform(string &str, int length)
{
    int xidx = 0, yidx = 0;
    for (int i = 0; i < length; i++)
    {
        xidx = (str[i] & 0xf0) >> 4;
        yidx = str[i] & 0xf;
        str[i] = str[anti_S_table[xidx][yidx]];
    }
    return;
}

AES128::AES128()
{
    memset(S_table, 0, sizeof(int) * 256);
    memset(anti_S_table, 0, sizeof(int) * 256);
    create_S_table();
    create_anti_S_table();
    srand((unsigned)time(NULL));
    char key_chr[16];
    for (int i = 0; i < 16; i++)
    {
        key_chr[i] = (rand() % 26) + 65;
    }
    key = key_chr;
    key = key.substr(0, 16);
    create_subkey();
}

AES128::~AES128(){}

void AES128::create_subkey()
{
    for (int i = 0; i < 4; i++)
    {
        subkey[i] = key.substr(4 * i, 4 * i + 4);
    }
    for (int i = 4; i < 44; i++)
    {
        if (i % 4 != 0)
        {
            subkey[i] = string_xor(subkey[i - 4], subkey[i - 1], 4);
        }
        else
        {
            subkey[i] = string_xor(subkey[i - 4], T_func(subkey[i - 1], 4, (i - 4) / 4), 4);
        }
    }
    for (int i = 0; i < 44; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            cout<<int(subkey[i][j])<<' ';
        }
        cout<<endl;
    }
}

string AES128::T_func(string str, int length, int round)
{
    left_cycle(str, length, 1);
    S_transform(str, 4);
    str[0] ^= Rcon[round];
    return str;
}