#include <stdio.h>
#include <string>
#include <time.h>
#define Len_S 256

using namespace std;

void swap(unsigned char &a, unsigned char &b)
{
    unsigned char tmp = a;
    a = b;
    b = tmp;
}

int main()
{
    string key;
    string S, T;
    key.resize(128);
    S.resize(256);
    T.resize(256);
    for (int i = 0; i < 128; i++)
    {
        key[i] = (char)(rand() % 26) + 65;
    }
    for (int i = 0; i < 256; i++)
        T[i] = key[i % 128];

    int j = 0;
    for (int i = 0; i < 256; i++)
    {
        j = (j + S[i] + T[i]) % 256;
        swap(S[i], S[j]);
    }

    int data_len = 2048;
    string key_stream;
    key_stream.resize(data_len);
    int i = 0;
    j = 0;
    for (int k = 0; k < data_len; k++)
    {
        i = (i + 1) % 256;
        j = (j + S[i]) % 256;
        swap(S[i], S[j]);
        int t = (S[i]+S[j])%256;
        key_stream[i] = S[t];
    }
    return 0;
}
