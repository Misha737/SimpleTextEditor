#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>

#include "./array.h"

char console_line[3];

int read_console_all(){
	memset(console_line, '\0', sizeof(console_line));
	fgets(console_line, sizeof(console_line), stdin);
	size_t end = strcspn(console_line, "\n");
	console_line[end] = '\0';
	//if (end == NULL) {
	//	printf("Please enter text\n");
	//	return 2;
	//}
	if (end >= sizeof(console_line) - 1) {
		return 1;
	}
	return 0;
}

void read_console() {
	int end = read_console_all();
	if (end == 1) {
		int ch;
		while ((ch = getchar()) != '\n' && ch != EOF);
	}
}

void parser() {
	printf("\nChoose the command: ");
	char input = _getche();
	switch (input)
	{
	case '1':
		printf("\nEnter text to append: ");
		int status;
		while((status = read_console_all()) == 1)
			insert(lines - 1, console_line, strlen(buffer[lines - 1]));
		//if (status = 2)
		//	return;
		insert(lines - 1, console_line, strlen(buffer[lines - 1]));
		printf("\n%s", buffer[0]);
		break;
	case '2':
		printf("\nNew line is started");
		new_line();
		break;
	case '3':
		printf("\nEnter the file name for saving: ");
		read_console();
		if (fwrite_buffer(console_line) == 0)
			printf("\nText has been saved successfully\n");
		break;
	case '4':
		printf("4");
		break;
	case '5':
		print_buffer();
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