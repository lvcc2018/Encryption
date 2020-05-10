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
    void convert_matrix_to_str(string& str, int mat[4][4]);
    void convert_str_to_matrix(string& str, int mat[4][4]);
    void add_round_key(int mat[4][4], int round);
    void sub_bytes(int mat[4][4]);
    void shift_rows(int mat[4][4]);
    void mix_columns(int mat[4][4]);
    
    void decrypt_block(string& str);
    void desub_bytes(int mat[4][4]);
    void deshift_rows(int mat[4][4]);
    void demix_columns(int mat[4][4]);
    void get_mat_from4W(int i, int mat[4][4]);
    
    string encrypt(string& plain_text);
    string decrypt(string& cipher_text);

};





#endif