#ifndef _SM4_H_
#define _SM4_H_

#include <string>
using namespace std;

class SM4
{
private:
    static unsigned char S_table[256];
    static unsigned int CK_table[32];
    static unsigned int FK_table[4];
    string key;
    string subkey[32];
    void create_subkey();
    void S_transform(string& str);
    void L_transform(string& str);
    void anti_L_transform(string& str);

public:
    SM4();
    ~SM4();
};

#endif