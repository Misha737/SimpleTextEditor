#include "caesar_cipher.h"

CaesarCipher::CaesarCipher(int key) : key(key)
{
	handle = LoadLibrary(TEXT("caesar_cipher.dll"));
	if (handle == nullptr || handle == INVALID_HANDLE_VALUE)
	{
		DWORD error = GetLastError();
		throw std::exception("Lib not found "  + error);
	}
	encrypt_ptr = (ptr_encrypt)GetProcAddress(handle, "encrypt");

	if (encrypt_ptr == NULL) {
		throw std::exception("Proc not found");

	}

	decrypt_ptr = (ptr_decrypt)GetProcAddress(handle, "decrypt");
	if (decrypt_ptr == NULL) {
		throw std::exception("Proc not found");
	}
}

CaesarCipher::~CaesarCipher()
{
	FreeLibrary(handle);
}

void CaesarCipher::encrypt(Text& text)
{
	text.add('\0');
	encrypt_ptr(&text[0], key);
	text.pop();
}

void CaesarCipher::decrypt(Text& text)
{
	text.add('\0');
	decrypt_ptr(&text[0], key);
	text.pop();
}
