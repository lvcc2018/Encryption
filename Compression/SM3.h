#ifndef SM3_H
#define SM3_H

#include <stddef.h>
#include "compression_utils.h"

typedef struct
{
    WORD Hash[8];
    BYTE message[64];
} SM3_CTX;
void SM3_init(SM3_CTX *ctx);
void SM3_block(SM3_CTX *ctx);
void SM3_transform(const BYTE *msg, WORD msg_len, BYTE digest[32]);

#endif