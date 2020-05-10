#ifndef _DES_H_
#define _DES_H_

#include <string>

#define BITS_PER_CHAR 8
#define KEY_SZIE 8
#define SUBKEY_NUM 16
#define SUBKEY_LENGHT 48
#define EXPAND_SIZE 48
#define PC_2_SIZE 48
#define PC_1_SIZE 56
#define BIT_STR_SIZE 64

using namespace std;

class DES
{
public:
	DES();
	~DES();

	string encrypt(const string &plain);
	string decrypt(const string &cipher);

private:
	string key;
	char subkey[SUBKEY_NUM][SUBKEY_LENGHT];
	bool create_subkey();
	bool encrypt_block(string &block);
	bool decrypt_block(string &block);
	bool PC1_transform(const string &bitStr, string &PC1BitStr);
	bool PC2_transform(const string &PC1BitStr, char subKey[SUBKEY_LENGHT]);
	bool IP_transform(string &bitStr);
	bool Expand_transform(const string &halfBitStr, string &eBitStr);
	bool SBox_transform(const string &eBitStr, string &halfBitStr);
	bool Permute_transform(string &halfBitStr);
	bool IP_1_transform(string &bitStr);
	bool Char8ToBit64(const string &str, string &bitStr);
	bool Bit64ToChar8(const string &bitStr, string &str);
	bool XOR(string &strFirst, string &strSecond, size_t num);
	bool LeftCycle(string &str, size_t beginSection, size_t endSection, size_t step);
	static int PC1_table[PC_1_SIZE];
	static int Move_table[SUBKEY_NUM];
	static int PC2_table[PC_2_SIZE];
	static int IP_table[BIT_STR_SIZE];
	static int Expand_table[EXPAND_SIZE];
	static int Permute_table[BIT_STR_SIZE / 2];
	static int IP_1_table[BIT_STR_SIZE];
	static int SBox_table[KEY_SZIE][4][16];
};

#endif
