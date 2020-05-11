#ifndef SHA256_H
#define SHA256_H

#include <stddef.h>

typedef struct
{
    unsigned char data[64];
    unsigned int datalen;
    unsigned long long bitlen;
    unsigned int state[8];
} SHA256_CTX;

void sha256_init(SHA256_CTX *ctx);
void sha256_update(SHA256_CTX *ctx, const unsigned char data[], size_t len, unsigned char hash[]);
void sha256_transform(SHA256_CTX *ctx, const unsigned char data[]);

#endif