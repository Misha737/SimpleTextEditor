#pragma once

extern "C" {
	__declspec(dllexport) char* encrypt(char* rawText, int key);
	__declspec(dllexport) char* decrypt(char* encryptedText, int key);
};

void algorithm(char* src, int key);

char arrayLoop(const char* src, int index);