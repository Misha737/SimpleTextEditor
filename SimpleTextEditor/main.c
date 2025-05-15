#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>

#include "./array.h"

void parser() {
	printf("\nChoose the command:\n");
	char input = _getche();
	switch (input)
	{
	case '1':
		printf("\nEnter text to append: ");
		char appended[100];
		fgets(appended, 100, stdin);
		size_t end = strcspn(appended, "\n");
		if (end == NULL) {
			printf("Please enter text\n");
			return;
		}
		appended[end] = '\0';
		insert(lines - 1, appended, strlen(buffer[lines - 1]));
		printf("\n%s", buffer[0]);
		break;
	case '2':
		printf("\nNew line is started");
		new_line();
		break;
	case '3':
		printf("3");
		break;
	case '4':
		printf("4");
		break;
	case '5':
		printf("5");
		break;
	case '6':
		printf("6");
		break;
	case '7':
		printf("7");
		break;
	default:
		printf("\nThe command is not implemented :(\n");
		break;
	}
}

int main(int argc, char* argv[]) {
	init_buffer();

	while (1) {
		parser();
	}
}