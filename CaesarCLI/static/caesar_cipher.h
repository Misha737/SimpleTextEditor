#pragma once

extern "C" {
	__declspec(dllexport) char* encrypt(char* rawText, int key);
	__declspec(dllexport) char* decrypt(char* encryptedText, int key);
};

void algorithm(char* src, size_t key);

char arrayLoop(const char* src, size_t index);