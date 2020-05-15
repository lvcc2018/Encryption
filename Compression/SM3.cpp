#include "SM3.h"
#include <memory.h>

static const WORD IV[8] = {0x7380166f, 0x4914b2b9, 0x172442d7, 0xda8a0600,
                           0xa96f30bc, 0x163138aa, 0xe38dee4d, 0xb0fb0e4e};

void SM3_init(SM3_CTX *ctx)
{
    for (int i = 0; i < 8; i++)
        ctx->Hash[i] = IV[i];
}

void SM3_block(SM3_CTX *ctx)
{
    WORD W[68], W2[64];
    for (int i = 0; i < 16; i++)
        W[i] = *(WORD *)(ctx->message + i * 4);
    for (int j = 16; j < 68; j++)
    {
        W[j] = P1(W[j - 16] ^ W[j - 9] ^ ROTLEFT(W[j - 3], 15)) ^ ROTLEFT(W[j - 13], 7) ^ W[j - 6];
    }
    for (int i = 0; i < 64; i++)
    {
        W2[i] = W[i] ^ W[i + 4];
    }
    WORD A, B, C, D, E, F, G, H;
    WORD SS1, SS2, TT1, TT2;
    A = ctx->Hash[0];
    B = ctx->Hash[1];
    C = ctx->Hash[2];
    D = ctx->Hash[3];
    E = ctx->Hash[4];
    F = ctx->Hash[5];
    G = ctx->Hash[6];
    H = ctx->Hash[7];
    for (int i = 0; i < 64; i++)
    {
        SS1 = ROTLEFT((ROTLEFT(A, 12) + E + ROTLEFT(T(i), (i % 32))), 7);
        SS2 = SS1 ^ ROTLEFT(A, 12);
        TT1 = FF(i, A, B, C) + D + SS2 + W2[i];
        TT2 = GG(i, E, F, G) + H + SS1 + W[i];
        D = C;
        C = ROTLEFT(B, 9);
        B = A;
        A = TT1;
        H = G;
        G = ROTLEFT(F, 19);
        F = E;
        E = P0(TT2);
    }
    ctx->Hash[0] ^= A;
    ctx->Hash[1] ^= B;
    ctx->Hash[2] ^= C;
    ctx->Hash[3] ^= D;
    ctx->Hash[4] ^= E;
    ctx->Hash[5] ^= F;
    ctx->Hash[6] ^= G;
    ctx->Hash[7] ^= H;
}

void SM3_transform(const BYTE *msg, WORD msg_len, BYTE digest[32])
{
    SM3_CTX ctx;
    unsigned int i, remainder;
    SM3_init(&ctx);
    for (int i = 0; i < msg_len / 64; i++)
    {
        memcpy(ctx.message, msg + i * 64, 64);
        SM3_block(&ctx);
    }
    int bit_len = msg_len * 8;
    remainder = msg_len % 64;
    memcpy(ctx.message, msg + i * 64, remainder);
    ctx.message[remainder] = 0x80;
    if (remainder <= 55)
    {
        memset(ctx.message + remainder + 1, 0, 64 - remainder - 1 - 8 + 4);
        memcpy(ctx.message + 64 - 4, &bit_len, 4);
        SM3_block(&ctx);
    }
    else
    {
        memset(ctx.message + remainder + 1, 0, 64 - remainder - 1);
        SM3_block(&ctx);
        memset(ctx.message, 0, 64 - 4);
        memcpy(ctx.message + 64 - 4, &bit_len, 4);
        SM3_block(&ctx);
    }

    memcpy(digest, ctx.Hash, 32);
}