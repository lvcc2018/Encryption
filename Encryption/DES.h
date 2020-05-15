#ifndef _DES_H_
#define _DES_H_

#include <string>

using namespace std;

class DES
{
public:
	DES();
	~DES();

	string encrypt(const string &plain);
	string decrypt(const string &cipher);
	string get_key();

private:
	string key;
	char subkey[16][48];
	bool create_subkey();
	bool encrypt_block(string &block);
	bool decrypt_block(string &block);
	bool PC1_transform(const string &bs, string &PC1bs);
	bool PC2_transform(const string &PC1bs, char subKey[48]);
	bool IP_transform(string &bs);
	bool Expand_transform(const string &hbs, string &ebs);
	bool SBox_transform(const string &ebs, string &hbs);
	bool Permute_transform(string &hbs);
	bool IP_1_transform(string &bs);
	bool XOR(string &str1, string &str2, size_t num);
	bool LeftCycle(string &str, size_t beginSection, size_t endSection, size_t step);
	static int PC1_table[56];
	static int Move_table[16];
	static int PC2_table[48];
	static int IP_table[64];
	static int Expand_table[48];
	static int Permute_table[64 / 2];
	static int IP_1_table[64];
	static int SBox_table[8][4][16];

	bool Char8ToBit64(const string &str, string &bs);
	bool Bit64ToChar8(const string &bs, string &str);
};

#endif
