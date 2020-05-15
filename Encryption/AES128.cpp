#include "AES128.h"
#include <time.h>

int AES128::Rcon[10] = {0x01, 0x02,
                        0x04, 0x08,
                        0x10, 0x20,
                        0x40, 0x80,
                        0x1b, 0x36};

void AES128::create_S_table()
{
    int i, j;
    for (i = 0; i < 0x10; i++)
    {
        for (j = 0; j < 0x10; j++)
        {
            S_table[i][j] = ((i << 4) & 0xF0) + (j & (0xF));
        }
    }
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
    for (i = 0; i < 0x10; i++)
    {
        for (j = 0; j < 0x10; j++)
        {
            S_table[i][j] = byteTransformation(S_table[i][j], 0x63);
        }
    }
}

void AES128::create_anti_S_table()
{
    int i, j;
    int b = 0, bb = 0;

    for (i = 0; i < 0x10; i++)
    {
        for (j = 0; j < 0x10; j++)
        {
            anti_S_table[i][j] = ((i << 4) & 0xF0) + (j & (0xF));
        }
    }

    for (i = 0; i < 0x10; i++)
    {
        for (j = 0; j < 0x10; j++)
        {
            anti_S_table[i][j] = invByteTransformation(anti_S_table[i][j], 0x05);
        }
    }

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

AES128::~AES128() {}

string AES128::get_key()
{
    return key;
}

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
}

string AES128::T_func(string str, int length, int round)
{
    left_cycle(str, length, 1);
    S_transform(str, 4);
    str[0] ^= Rcon[round];
    return str;
}

void AES128::encrypt_block(string &str)
{
    int p[4][4];
    convert_str_to_matrix(str, p);
    add_round_key(p, 0);
    for (int i = 1; i < 10; i++)
    {
        sub_bytes(p);
        shift_rows(p);
        mix_columns(p);
        add_round_key(p, i);
    }
    sub_bytes(p);
    shift_rows(p);
    add_round_key(p, 10);
    convert_matrix_to_str(str, p);
}


void AES128::decrypt_block(string &str)
{
    int p[4][4];
    int w[4][4];
    convert_str_to_matrix(str, p);
    add_round_key(p, 10);
    for (int i = 9; i >0; i--)
    {
        desub_bytes(p);
        deshift_rows(p);
        demix_columns(p);
        get_mat_from4W(i, w);
        demix_columns(w);
        
        for(int j = 0; j < 4; j++)
		    for(int k = 0; k < 4; k++)
			    p[j][k] = p[j][k] ^ w[j][k];
    }
    desub_bytes(p);
    deshift_rows(p);
    add_round_key(p, 0);
    convert_matrix_to_str(str, p);
}

void AES128::get_mat_from4W(int i,int mat[4][4])
{
	int index = i * 4;
	for(int i = 0; i < 4; i++) {
		mat[i][0] = subkey[index][i];
		mat[i][1] = subkey[index+1][i];
		mat[i][2] = subkey[index+2][i];
		mat[i][3] = subkey[index+3][i];
	}
}

void AES128::add_round_key(int mat[4][4], int round)
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            mat[j][i] = mat[j][i] ^ subkey[round * 4 + i][j];
        }
    }
}

void AES128::sub_bytes(int mat[4][4])
{
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            mat[i][j] = S_table[((mat[i][j] & 0x000000f0) >> 4)][mat[i][j] & 0xf];
}
void AES128::shift_rows(int mat[4][4])
{
    int rowTwo[4], rowThree[4], rowFour[4];
    for (int i = 0; i < 4; i++)
    {
        rowTwo[i] = mat[1][i];
        rowThree[i] = mat[2][i];
        rowFour[i] = mat[3][i];
    }
    leftLoop4int(rowTwo, 1);
    leftLoop4int(rowThree, 2);
    leftLoop4int(rowFour, 3);
    for (int i = 0; i < 4; i++)
    {
        mat[1][i] = rowTwo[i];
        mat[2][i] = rowThree[i];
        mat[3][i] = rowFour[i];
    }
}

void AES128::mix_columns(int mat[4][4])
{
    int tempmat[4][4];

    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            tempmat[i][j] = mat[i][j];

    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
        {
            mat[i][j] = GFMul(colM[i][0], tempmat[0][j]) ^ GFMul(colM[i][1], tempmat[1][j]) ^ GFMul(colM[i][2], tempmat[2][j]) ^ GFMul(colM[i][3], tempmat[3][j]);
        }
}
void AES128::convert_matrix_to_str(string &str, int mat[4][4])
{
    int k = 0;
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            str[k++] = mat[j][i];
}
void AES128::convert_str_to_matrix(string &str, int mat[4][4])
{
    int k = 0;
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            mat[j][i] = str[k++];
}

void AES128::desub_bytes(int mat[4][4])
{
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            mat[i][j] = anti_S_table[((mat[i][j] & 0x000000f0) >> 4)][mat[i][j] & 0xf];
}

void AES128::deshift_rows(int mat[4][4])
{
    int rowTwo[4], rowThree[4], rowFour[4];
	int i;
	for(i = 0; i < 4; i++) {
		rowTwo[i] = mat[1][i];
		rowThree[i] = mat[2][i];
		rowFour[i] = mat[3][i];
	}

	rightLoop4int(rowTwo, 1);
	rightLoop4int(rowThree, 2);
	rightLoop4int(rowFour, 3);

	for(i = 0; i < 4; i++) {
		mat[1][i] = rowTwo[i];
		mat[2][i] = rowThree[i];
		mat[3][i] = rowFour[i];
	}

}

void AES128::demix_columns(int mat[4][4])
{
    int tempmat[4][4];

	for(int i = 0; i < 4; i++)
		for(int j = 0; j < 4; j++)
			tempmat[i][j] = mat[i][j];

	for(int i = 0; i < 4; i++)
		for(int j = 0; j < 4; j++){
			mat[i][j] = GFMul(deColM[i][0],tempmat[0][j]) ^ GFMul(deColM[i][1],tempmat[1][j]) 
				^ GFMul(deColM[i][2],tempmat[2][j]) ^ GFMul(deColM[i][3], tempmat[3][j]);
		}

}

string AES128::encrypt(string& plain_text)
{
    string result;
    for (size_t i = 0; i < plain_text.size() / 16; ++i)
    {
        string block = plain_text.substr(i * 16, 16);
        encrypt_block(block);
        result.append(block);
    }
    return result;
}

string AES128::decrypt(string& cipher_text)
{
    string result;
    for (size_t i = 0; i < cipher_text.size() / 16; ++i)
    {
        string block = cipher_text.substr(i * 16, 16);
        decrypt_block(block);
        result.append(block);
    }
    return result;
}