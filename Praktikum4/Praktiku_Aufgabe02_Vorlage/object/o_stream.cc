#include "o_stream.h"

O_Stream& O_Stream::operator << (char c) {
	//TODO implement me
	put(c);
	return *this;
}

O_Stream& O_Stream::operator << (unsigned char c) {
	//TODO implement me
	return *this << (unsigned long long) c;
}

O_Stream& O_Stream::operator << (const char* string) {
	//TODO implement me
	char c = *string;
	int i = 0;
	while(c != '\0'){
		put(c);
		i++;
		c = *(string+i);
	}
	return *this;
}

O_Stream& O_Stream::operator << (bool b){
	if(b){
		return *this << "true";
	}else{
		return *this << "false";
	}
}

O_Stream& O_Stream::operator << (short ival) {
	return *this << (long long) ival;
}

O_Stream& O_Stream::operator << (unsigned short ival) {
	return *this << (unsigned long long) ival;
}

O_Stream& O_Stream::operator << (int ival) {
	return *this << (long long) ival;
}

O_Stream& O_Stream::operator << (unsigned int ival) {
	return *this << (unsigned long long) ival;
}

O_Stream& O_Stream::operator << (long ival) {
	return *this << (long long) ival;
}

O_Stream& O_Stream::operator << (unsigned long ival) {
	return *this << (unsigned long long) ival;
}

O_Stream& O_Stream::operator << (long long ival) {
	if ((ival < 0) && (base == 10)) {
		put('-');
		ival = -ival;
	}
	return *this << (unsigned long long) ival;
}

O_Stream& O_Stream::operator << (unsigned long long ival) {
	unsigned long long div;
	char digit;

	if(base == 0) {
		base = 16;
	}

	if (base == 2) {
		put('0');
		put('b');
	}
	else if (base == 8) {
		put('0');
	} else if (base == 16) {
		put('0');
		put('x');
	}

	for (div = 1; ival/div >= (unsigned long long) base; div *= base);

	for (; div > 0; div /= (unsigned long long) base) {
		digit = ival / div;
		if (digit < 10)
			put('0' + digit);
		else
			put('a' + digit - 10);
		ival %= div;
	}
	return *this;
}

O_Stream& O_Stream::operator << (const void* ptr) {
	int oldbase = base;
	base = 16;
	*this << (unsigned long long) ptr;
	base = oldbase;
	return *this;
}

O_Stream& O_Stream::operator << (O_Stream& (*f) (O_Stream&)) {
	return f(*this);
}

O_Stream& flush(O_Stream& os) {
	os.flush();
	return os;
}

O_Stream& endl(O_Stream& os) {
	os << '\n';
	os.flush();
	return os;
}

O_Stream& bin(O_Stream& os) {
	os.base = 2;
	return os;
}

O_Stream& oct(O_Stream& os) {
	os.base = 8;
	return os;
}

O_Stream& dec(O_Stream& os) {
	os.base = 10;
	return os;
}

O_Stream& hex(O_Stream& os) {
	os.base = 16;
	return os;
}

