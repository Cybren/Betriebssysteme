#include "machine/cga_screen.h"
#include "machine/io_port.h"
#include "device/cga_stream.h"
//extern CGA_Stream kout;

static IO_Port index_port (0x3d4);

static IO_Port data_port (0x3d5);

CGA_Screen::Cell * const CGA_Screen::CGA_START = (CGA_Screen::Cell *)(0xb8000);

CGA_Screen::CGA_Screen(int from_col, int to_col, int from_row, int to_row, bool use_cursor)
	: x_pos(from_col), y_pos(from_row),from_col(from_col), to_col(to_col),
	  from_row(from_row), to_row(to_row),
	  col_span(to_col - from_col + 1), row_span(to_row - from_row + 1), use_cursor(use_cursor)
{
	bool err = false;
	if (col_span < 1 || row_span < 1)
		err = true;
	reset();//damit komische Fragmente verschwinden
	if (err)
		checkBounce(79, 24, '-', Attribute(CGA_Screen::RED, CGA_Screen::BLACK, true));
	else
		reset();
}

void CGA_Screen::setpos (int x, int y)
{
	if (x < 0)
		x += to_col;
	if (y < 0)
		y += to_row;

	if (x >= from_col && x <= to_col && y >= from_row && y <= to_row) {
		if (use_cursor) {
			int pos;
			int high, low;

			if (x < 0 || x >= COLUMNS || y < 0 || y > ROWS)
				return;

			pos  = y * COLUMNS + x;
			low  = pos & 0xff;
			high = (pos & 0x3f00) >> 8;

			index_port.outb (14);
			data_port.outb (high);
			index_port.outb (15);
			data_port.outb (low);
		}

		x_pos = x;
		y_pos = y;
	}
}


void CGA_Screen::getpos (int& x, int& y)
{
	if (use_cursor) {
		int high, low;
		int pos;

		index_port.outb (14);
		high = data_port.inb ();
		index_port.outb (15);
		low = data_port.inb ();

		pos = (high & 0x3f) << 8 | (low & 0xff);
		x  = pos % COLUMNS;
		y  = pos / COLUMNS;
	} else {
		x = x_pos;
		y = y_pos;
	}
}

void CGA_Screen::print (char* string, int length, Attribute attribute) {
	int posX;
	int posY;
	char lastchar = '\n';
	getpos(posX, posY);
	for (int i = 0; i < length; i++){
		if(i != 0 && !(lastchar == '\n')){
			getpos(posX, posY);
			checkBounce(posX, posY, *(string+i), attribute);
			getpos(posX, posY);
		}
		if(*(string+i) != '\n'){
			printC(posY * CGA_Screen::COLUMNS + posX, *(string+i), attribute);
		}
		lastchar = *(string+i);
	}
}


void CGA_Screen::writeToConsole() {
	setpos(37, 0);
	for (int y = 1; y < CGA_Screen::ROWS-1; y++){
		for (int x = 0; x < CGA_Screen::COLUMNS; x++){
			setpos(x, y);
			Cell cell = CGA_START[y * CGA_Screen::COLUMNS + x];
			printC((y-1) * CGA_Screen::COLUMNS + x, cell.character, cell.attribute);
		}
	}
	for (int x = 0; x < CGA_Screen::COLUMNS; x++){
		printC( (CGA_Screen::ROWS-2) * CGA_Screen::COLUMNS + x, ' ');
	}
}

void CGA_Screen::reset (char character, Attribute attribute)
{
	for(int y = from_row; y <= to_row; ++y)
		for(int x = from_col; x <= to_col; ++x)
			checkBounce(x, y, character, attribute);
	setpos(from_col, from_row);
}

void CGA_Screen::checkBounce(int x, int y, char character, Attribute attribute) {
	if(x+1 == CGA_Screen::COLUMNS || character == '\n'){
		if(y+1 == to_row){
			writeToConsole();
			setpos(0, y);
		}else{
			setpos(0, y+1);
		}
	}else{
		setpos(x+1, y);
	}
}

void CGA_Screen::printC (int pos, char character, Attribute attribute) {
	CGA_START[pos] = (Cell) {character, attribute};
}