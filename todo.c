#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <string.h>

char VERSION[] = "0.1";

int size_y, size_x;

void display_page(const char content[], int start_line, int end_line) {
	ulong i = 0;
	ulong line = 0;

	printw(" ");
	while (content[i] != '\0' && line <= end_line) {
		if (line >= start_line && line <= end_line) {
			printw("%c", content[i]);
			if (content[i] == '\n') {
				printw(" ");
			}
		}

		if (content[i] == '\n')
			line++;
		i++;
	}

	if (content[i] == '\0')
		printw("\n");
}

int main(int argc, char *argv[]) {
	if (argc != 2) {
		fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
		return EXIT_FAILURE;
	}

	FILE *file = fopen(argv[1], "r");
	if (file == NULL) {
		perror("Error opening file");
		return EXIT_FAILURE;
	}

	initscr();		// Initialize the curses library
	raw();			// Disable line buffering
	keypad(stdscr, TRUE);	// Enable special keys
	noecho();   		// Don't display input characters

	fseek(file, 0, SEEK_END);
	ulong file_size = ftell(file);
	fseek(file, 0, SEEK_SET);
	char *content = malloc(file_size+1);
	fread(content, 1, file_size, file);
	content[file_size] = '\0';


	int start_line = 0;
	int end_line = 0;
	int cur_line = 0;

	int ch;
	do {
		getmaxyx(stdscr, size_y, size_x);
		end_line = size_y - 2 + start_line;

		move(0, 0);
		attron(A_REVERSE);
		hline(' ', size_x);
		printw("\r  TODO %s", VERSION);
		move( 0, (size_x-strlen(argv[1]))*0.5);
		printw("%s", argv[1]);
		attroff(A_REVERSE);
		move(1, 0);
		display_page(content, start_line, end_line);
		move(cur_line-start_line+1, 0);
		refresh();

		ch = getch();
		switch (ch) {
			case KEY_DOWN:
				cur_line++;
				if (cur_line > end_line)
					start_line++;
				break;
			case KEY_UP:
				if (cur_line > 0) {
					cur_line--;
					if (start_line > cur_line)
						start_line--;
				}
				break;
		}

	} while (ch != 'q');

	endwin(); // Clean up and close the curses library
	fclose(file);

	free(content);
	return EXIT_SUCCESS;
}
