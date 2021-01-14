#pragma once

#include "types.h"


class CGA_Screen{
private:

	CGA_Screen(const CGA_Screen&)            = delete;
	CGA_Screen& operator=(const CGA_Screen&) = delete;
	int x_pos;
	int y_pos;
	int from_col;
	int to_col;
	int from_row;
	int to_row;
	int col_span;
	int row_span;

	bool use_cursor;

public:

	CGA_Screen(int from_col, int to_col, int from_row, int to_row, bool use_cursor=false);


	static const int ROWS = 25;
	static const int COLUMNS = 80;


	enum Color {
		BLACK, BLUE, GREEN, CYAN, RED, MAGENTA, BROWN, LIGHT_GREY,
		DARK_GREY, LIGHT_BLUE, LIGHT_GREEN, LIGHT_CYAN, LIGHT_RED,
		LIGHT_MAGENTA, YELLOW, WHITE
	};


	struct Attribute {
		uint8_t foreground : 4;
		uint8_t background : 3;
		uint8_t blink      : 1;


		Attribute(Color foreground = LIGHT_GREY, Color background = BLACK, bool blink = false)
			:	foreground(foreground),
				background(background <= DARK_GREY ? background : BLACK),
				blink(blink)
			{
			};
	} __attribute__((packed));


	void setpos (int x, int y);
	void getpos (int& x, int& y);
	void print (char* string, int length, Attribute attrib = Attribute());
	void reset(char character=' ', Attribute attrib = Attribute());
	static void show (int x, int y, char character, Attribute attrib = Attribute());
	void printC (int pos, char character, Attribute attribute = Attribute());
	void checkBounce (int x, int y, char character, Attribute attribute = Attribute());
<<<<<<< HEAD
	void writeToConsole (int z, int y, Attribute attribute);
=======
    	void writeToConsole ();
>>>>>>> 8527cf63cc1172c1d454181d2c0df1740f71a677

private:
	struct Cell {
		char character;
		Attribute attribute;
	} __attribute__((packed));
	static Cell * const CGA_START;
};
