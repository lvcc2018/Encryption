#include "SHA3.h"

#include <memory.h>

void SHA3_init(SHA3_CTX *ctx)
{
    ctx->H[0] = 0x6a09e667;
    ctx->H[1] = 0xbb67ae85;
    ctx->H[2] = 0x3c6ef372;
    ctx->H[3] = 0xa54ff53a;
    ctx->H[4] = 0x510e527f;
    ctx->H[5] = 0x9b05688c;
    ctx->H[6] = 0x1f83d9ab;
    ctx->H[7] = 0x5be0cd19;
}
char *SHA3_padding(const BYTE *s, WORD &l)
{
    size_t orig = l;
    char *t = new char[l + 512];
    memcpy(t, s, l);
    t[l++] = 0x80;
    while ((l << 3) % 512 != 512 - 64)
        ++l;
    l += 4;
    for (int i = 24; i >= 0; i -= 8)
        t[l++] = (char)((orig << 3) >> i);
    return t;
}

char* sha(char* s, int len) {
	SHA3_CTX ctx;
    SHA3_init(&ctx);
	char* ptr_w = (char*)(&(ctx.w));
	for (int i = 0; i < len; i += 64) {
		for (int j = 0; j < 64; ++j)
			ptr_w[j] = s[i | (j ^ 3)];
		for (int j = 16; j < 64; ++j)
			ctx.w[j] = ctx.w[j - 16] + ctx.w[j - 7] + (ROTRIGHT(ctx.w[j - 15], 7) ^ ROTRIGHT(ctx.w[j - 15], 18) ^ (ctx.w[j - 15] >> 3)) + (ROTRIGHT(ctx.w[j - 2], 17) ^ ROTRIGHT(ctx.w[j - 2], 19) ^ (ctx.w[j - 2] >> 10));
		memcpy(ctx.G, ctx.H, sizeof ctx.H);
		for (int j = 0; j < 64; ++j) {
			size_t t1 = ctx.G[7] + (ROTRIGHT(ctx.G[4], 6) ^ ROTRIGHT(ctx.G[4], 11) ^ ROTRIGHT(ctx.G[4], 25)) + ((ctx.G[4] & ctx.G[5]) ^ ((~ctx.G[4]) & ctx.G[6])) + K[j] + ctx.w[j];
			size_t t2 = (ROTRIGHT(ctx.G[0], 2) ^ ROTRIGHT(ctx.G[0], 13) ^ ROTRIGHT(ctx.G[0], 22)) + ((ctx.G[0] & ctx.G[1]) ^ (ctx.G[1] & ctx.G[2]) ^ (ctx.G[2] & ctx.G[0]));
			ctx.G[7] = ctx.G[6], ctx.G[6] = ctx.G[5], ctx.G[5] = ctx.G[4], ctx.G[4] = ctx.G[3] + t1;
			ctx.G[3] = ctx.G[2], ctx.G[2] = ctx.G[1], ctx.G[1] = ctx.G[0], ctx.G[0] = t1 + t2;
		}
		ctx.H[0] += ctx.G[0], ctx.H[1] += ctx.G[1], ctx.H[2] += ctx.G[2], ctx.H[3] += ctx.G[3];
		ctx.H[4] += ctx.G[4], ctx.H[5] += ctx.G[5], ctx.H[6] += ctx.G[6], ctx.H[7] += ctx.G[7];
	}
    char* ans = (char*) (ctx.H);
    return ans;
}

const char* lut = "0123456789abcdef";
void SHA3_transform(const BYTE *msg, WORD msg_len, BYTE digest[64])
{
    int len = 0;
    char *t = SHA3_padding(msg, msg_len);
    char *r = sha(t, len);
    for (int i = 0; i < 32; ++i)
    {
        BYTE tmp = r[i ^ 3] & 0xff;
        digest[i << 1] = lut[tmp >> 4];
        digest[i << 1 | 1] = lut[tmp & 0xf];
    }
}
