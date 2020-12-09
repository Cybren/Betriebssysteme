

#pragma once


class Stringbuffer {

	Stringbuffer(const Stringbuffer&)            = delete;
	Stringbuffer& operator=(const Stringbuffer&) = delete;






protected:

	char buffer[80];

	int pos;


	Stringbuffer()
		: pos(0)
	{ }


	void put(char c);


	virtual void flush() = 0;
public:

	virtual ~Stringbuffer() { }
};
