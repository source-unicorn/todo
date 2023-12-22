#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <string.h>

char VERSION[] = "0.1";

int size_y, size_x;

void display_page(FILE *file, int start_line, int end_line) {
	char buffer[1024];
	int line_count = 0;
	fseek(file, 0, SEEK_SET);
	while (fgets(buffer, sizeof(buffer), file) != NULL) {
		if (line_count >= start_line && line_count <= end_line) {
			printw("%s", buffer);
		}
		line_count++;

		if (line_count > end_line) {
			break;
		}

	}
	if (line_count <= end_line) {
		printw("\n");
	}
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

	initscr();  // Initialize the curses library
	raw();      // Disable line buffering
	keypad(stdscr, TRUE); // Enable special keys
	noecho();   // Don't display input characters

	fseek(file, 0, SEEK_END);
	ulong file_size = ftell(file);
	fseek(file, 0, SEEK_SET);

	int start_line = 0;

	int ch;
	do {
		move(0, 0);
		getmaxyx(stdscr, size_y, size_x);

		attron(A_REVERSE);
		hline(' ', size_x);
		printw("\r  TODO %s", VERSION);
		move( 0, (size_x-strlen(argv[1]))*0.5);
		printw("%s", argv[1]);
		attroff(A_REVERSE);
		move(1, 0);
		display_page(file, start_line, size_y - 2 + start_line);
		refresh();

		ch = getch();

		switch (ch) {
			case KEY_DOWN:
				start_line++;
				break;
			case KEY_UP:
				if (start_line > 0) {
					start_line--;
				}
				break;
		}

	} while (ch != 'q');

	endwin(); // Clean up and close the curses library

	fclose(file);
	return EXIT_SUCCESS;
}
