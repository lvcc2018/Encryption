#ifndef _AES128_H_
#define _AES128_H_
 
#include <string>
using namespace std;



class AES128
{
private:
    string key;
    string subkey[44];
    int S_table[16][16];
    int anti_S_table[16][16];
    static int Rcon[10];

    void create_S_table();
    void create_anti_S_table();
    void create_subkey();
    void S_transform(string& str, int length);
    void anti_S_transform(string& str, int length);
    string T_func(string str, int length, int round);

public:
    AES128();
    ~AES128();
    void encrypt_block(string& str);
    
};





#endif