#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "./array.h"

int main(int argc, char* argv[]) {
	char* buffer = (char*)malloc(6 * sizeof(char));
	if (buffer == NULL)
		exit(1);
	strcpy_s(buffer, 6, "Hello");
	insert(&buffer, "12", 2);
	printf("%s\n", buffer);
}