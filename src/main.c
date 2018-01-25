
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <openssl/sha.h>

struct block {
    uint32_t id;
    uint32_t nonce;
    char data[33];
    char hash[SHA256_DIGEST_LENGTH * 2 + 1];
};

int sha256sum_sign(struct block * block, int sign) {
    char data[41];
    unsigned char digest[SHA256_DIGEST_LENGTH];
    SHA256_CTX ctx;

    while (1) {
        sprintf((char*) &data, "%u%u%s", block->id, block->nonce, block->data);

        SHA256_Init(&ctx);
        SHA256_Update(&ctx, data, strlen(data));
        SHA256_Final(digest, &ctx);

        for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
            sprintf(&block->hash[i * 2], "%02x", (unsigned int) digest[i]);
        }

        if ((digest[0] == 0 && digest[1] == 0 && digest[1] == 0) || sign == 0) {
            break;
        } else {
            block->nonce++;
        }
    }

    return 0;
}

int main(int argc, char**argv) {
    int i;
    struct block block;

    block.id = 0;
    block.nonce = 1;

    printf("\n*** Please set the block data ***\n");
    printf("Block data.: ");

    fgets(block.data, sizeof (block.data), stdin);
    if ((strlen(block.data) > 0) && (block.data[strlen(block.data) - 1] == '\n'))
        block.data[strlen(block.data) - 1] = '\0';

    sha256sum_sign(&block, 0);

    printf("\n*** Block without a signed structure ***\n");
    printf("Block id...: %u\n", block.id);
    printf("Block nonce: %u\n", block.nonce);
    printf("Block data.: %s\n", block.data);
    printf("Block hash.: %s\n", block.hash);

    sha256sum_sign(&block, 1);

    printf("\n*** Block signed ***\n");
    printf("Block id...: %u\n", block.id);
    printf("Block nonce: %u\n", block.nonce);
    printf("Block data.: %s\n", block.data);
    printf("Block hash.: %s\n", block.hash);

    return 0;
}
