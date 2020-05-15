#include "Encryption/utils.h"
#include "Compression/compression_utils.h"

#include "Encryption/AES128.h"
#include "Encryption/DES.h"
#include "Encryption/SM4.h"

#include "Compression/SHA2.h"
#include "Compression/SHA3.h"
#include "Compression/SM3.h"

#include "BM/BM.h"

#include <cstdlib>
#include <ctime>

using namespace std;

int main()
{

    printf("Create 16K bit random plain text...");
    rand_plain_text();
    printf("Completed!\n\n");
    string test, result, translate;
    ifstream input_file("plain_text.txt");
    input_file >> test;
    input_file.close();

    printf("Begin to test DES:\n");
    DES des_ctx;
    printf("The random key is %s \n", des_ctx.get_key().c_str());
    clock_t start, end;
    double endtime;
    printf("Begin to encrypt...");
    start = clock();
    result = des_ctx.encrypt(test);
    end = clock();
    endtime = (double)(end - start) / CLOCKS_PER_SEC;
    ofstream output_file("DES_cipher_text.txt");
    output_file << result;
    output_file.close();
    printf("Completed\n");
    printf("Use time: %lf  Encrypt speed: %lf Mbps\n", endtime, 16.0 / endtime / 1000);

    printf("Begin to decrypt...");
    start = clock();
    translate = des_ctx.decrypt(result);
    end = clock();
    endtime = (double)(end - start) / CLOCKS_PER_SEC;
    printf("Completed\n");
    printf("Use time: %lf  Decrypt speed: %lf Mbps\n\n\n", endtime, 16.0 / endtime / 1000);
    ofstream translate_file("DES_translate_text.txt");
    translate_file << translate;
    translate_file.close();

    printf("Begin to test AES:\n");
    AES128 aes_ctx;
    printf("The random key is %s \n", aes_ctx.get_key().c_str());
    printf("Begin to encrypt...");
    start = clock();
    result = aes_ctx.encrypt(test);
    end = clock();
    endtime = (double)(end - start) / CLOCKS_PER_SEC;
    output_file.open("AES_cipher_text.txt");
    output_file << result;
    output_file.close();
    printf("Completed\n");
    printf("Use time: %lf  Encrypt speed: %lf Mbps\n", endtime, 16.0 / endtime / 1000);

    printf("Begin to decrypt...");
    start = clock();
    translate = aes_ctx.decrypt(result);
    end = clock();
    endtime = (double)(end - start) / CLOCKS_PER_SEC;
    printf("Completed\n");
    printf("Use time: %lf  Decrypt speed: %lf Mbps\n\n\n", endtime, 16.0 / endtime / 1000);
    translate_file.open("AES_translate_text.txt");
    translate_file << translate;
    translate_file.close();

    printf("Begin to test SM4:\n");
    SM4 sm4_ctx;
    printf("The random key is %s \n", sm4_ctx.get_key().c_str());
    printf("Begin to encrypt...");
    start = clock();
    result = sm4_ctx.encrypt(test);
    end = clock();
    endtime = (double)(end - start) / CLOCKS_PER_SEC;
    output_file.open("SM4_cipher_text.txt");
    output_file << result;
    output_file.close();
    printf("Completed\n");
    printf("Use time: %lf  Encrypt speed: %lf Mbps\n", endtime, 16.0 / endtime / 1000);

    printf("Begin to decrypt...");
    start = clock();
    translate = sm4_ctx.decrypt(result);
    end = clock();
    endtime = (double)(end - start) / CLOCKS_PER_SEC;
    printf("Completed\n");
    printf("Use time: %lf  Decrypt speed: %lf Mbps\n\n\n", endtime, 16.0 / endtime / 1000);
    translate_file.open("SM4_translate_text.txt");
    translate_file << translate;
    translate_file.close();

    printf("Begin to test RC4:\n");
    string key;
    string S, T;
    key.resize(128);
    S.resize(256);
    T.resize(256);
    for (int i = 0; i < 128; i++)
    {
        key[i] = (char)(rand() % 26) + 65;
    }
    printf("The random key is %s \n", key.c_str());
    printf("Begin to generate...");
    start = clock();
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
        int t = (S[i] + S[j]) % 256;
        key_stream[i] = S[t];
    }
    end = clock();
    endtime = (double)(end - start) / CLOCKS_PER_SEC;
    printf("Completed\n");
    printf("Use time: %lf  Translate speed: %lf Mbps\n\n\n", endtime, 16.0 / endtime / 1000);
    output_file.open("RC4_key_stream.txt");
    output_file << key_stream;
    output_file.close();

    printf("Begin to test B-M:\n");
    int a = 0, nn = 10;
    for (int i = 0; i < nn; i++)
    {
        a = a ^ ((rand() % 2) << i);
    }
    printf("Begin to generate LFSR...");
    printf("Completed\n");
    start = clock();
    B_M(a, nn);
    end = clock();
    endtime = (double)(end - start) / CLOCKS_PER_SEC;
    printf("Use time: %lf  Generate speed: %lf Mbps\n\n\n", endtime, 16.0 / endtime / 1000);

    printf("Begin to test SHA2...");
    SHA256_CTX ctx;
    sha256_init(&ctx);
    unsigned char data[2048];
    for (int i = 0; i < 2048; i++)
    {
        data[i] = test[i];
    }
    unsigned char hash_result[32];
    start = clock();
    sha256_update(&ctx, data, 2048, hash_result);
    end = clock();
    endtime = (double)(end - start) / CLOCKS_PER_SEC;
    printf("Completed. Hash speed: %lf Mbps\n", 16.0 / endtime / 1000);
    output_file.open("SHA2_hash.txt");
    for (int i = 0; i < 32; i++)
    {
        output_file << hash_result[i];
    }
    output_file.close();

    printf("Begin to test SM3...");
    SM3_CTX sm3_ctx;
    start = clock();
    SM3_transform(data, 2048, hash_result);
    end = clock();
    endtime = (double)(end - start) / CLOCKS_PER_SEC;
    printf("Completed. Hash speed: %lf Mbps\n", 16.0 / endtime / 1000);
    output_file.open("SM3_hash.txt");
    for (int i = 0; i < 32; i++)
    {
        output_file << hash_result[i];
    }
    output_file.close();

    printf("Begin to test SHA3...");
    unsigned char hash_result_sha3[64];
    start = clock();
    SHA3_transform(data, 2048, hash_result_sha3);
    end = clock();
    endtime = (double)(end - start) / CLOCKS_PER_SEC;
    printf("Completed. Hash speed: %lf Mbps\n", 16.0 / endtime / 1000);
    output_file.open("SHA3_hash.txt");
    for (int i = 0; i < 64; i++)
    {
        output_file << hash_result_sha3[i];
    }
    output_file.close();

    return 0;
}