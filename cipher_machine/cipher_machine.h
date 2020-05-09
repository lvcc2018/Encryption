#ifndef CIPHER_MACHINE
#define CIPHER_MACHINE

#include <string>
using namespace std;

class cipher_machine
{
    private:
        string* plain_text;
        string key;
        string subkey[16];

        static int PC1_table[56];
        static int PC2_table[48];
        static int move_table[16];
        static int SBox_table[8][4][16];
        static int IP_table[64];
        static int anti_IP_table[64];
        static int expand_table[48];
        static int P_table[32];

    
    private:
        void create_subkey();
        void LeftCycle(string str, int beginSection, int endSection, int step);


    public:
        cipher_machine();
        void encrypt();
        void decrypt();

        string encrypt_block(string block);
        string decrypt_block(string block);

        string IP_transform(string block);
        string anti_IP_transform(string block);
        string expand_transform(string block);
        string XOR_transform(string block1, string block2, int length);
        string Sbox_transform(string block);
        string P_transform(string block);
};

#endif