#include "caesar_cipher.h"
#include <string>
#include <iostream>

const char SUMBOLS_UPPER[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
const char SUMBOLS_LOWER[] = "abcdefghijklmnopqrstuvwxyz";

char* encrypt(char* rawText, int key)
{
    algorithm(rawText, key);
    return rawText;
}

char* decrypt(char* encryptedText, int key)
{
    algorithm(encryptedText, -key);
    return encryptedText;
}

void algorithm(char* src, int key)
{
    for (size_t i = 0; i < strlen(src); i++) {
        char find[2];
        strncpy_s(find, 2, &src[i], 1);
        char new_char;
        size_t index;
        if ((index = strcspn(SUMBOLS_UPPER, find)) != strlen(SUMBOLS_UPPER)) {
            new_char = arrayLoop(SUMBOLS_UPPER, index + key);
        }
        else if ((index = strcspn(SUMBOLS_LOWER, find)) != strlen(SUMBOLS_LOWER)) {
            new_char = arrayLoop(SUMBOLS_LOWER, index + key);
        }
        else {
            continue;
        }
        src[i] = new_char;
    }
}

char arrayLoop(const char* src, int index)
{
    int src_len = strlen(src);
    int src_index = index % src_len;
    if (src_index < 0) {
        return src[src_len + src_index];
    }
    return src[src_index];
}
