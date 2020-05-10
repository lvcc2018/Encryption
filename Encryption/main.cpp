#include <cstdlib>
#include <iostream>
#include <fstream>
#include "DES.h"
#include "AES128.h"
#include "SM4.h"
#include <time.h>

using namespace std;

int main()
{
    /*
    rand_plain_text();
    DES cm;
    string test;
    ifstream input_file("plain_text.txt");
    input_file>>test;
    input_file.close();
    srand((unsigned)time(NULL));
    char key[64];
    for (int i=0;i<64;i++)
    {
        key[i] = (rand() % 26) + 65;
    }
    string key_str = key;
    string result = cm.Encrypt(test, key);
    ofstream output_file("cipher_text.txt");
    output_file<<result;
    output_file.close();
    string trans_text = cm.Decrypt(result, key);
    ofstream translate_file("translate_text.txt");
    translate_file<<trans_text;
    translate_file.close();
    
    string test;
    ifstream input_file("plain_text.txt");
    input_file>>test;
    input_file.close();
    AES128 aes;
    string result = aes.encrypt(test);
    ofstream output_file("aes_cipher_text.txt");
    output_file<<result;
    output_file.close();
    string trans_text = aes.decrypt(result);
    ofstream translate_file("aes_translate_text.txt");
    translate_file<<trans_text;
    translate_file.close();

    string test;
    ifstream input_file("plain_text.txt");
    input_file>>test;
    input_file.close();
    SM4 sm;
    string result = sm.encrypt(test);
    ofstream output_file("sm_cipher_text.txt");
    output_file<<result;
    output_file.close();
    string trans_text = sm.decrypt(result);
    ofstream translate_file("sm_translate_text.txt");
    translate_file<<trans_text;
    translate_file.close();
    */

    return 0;
}