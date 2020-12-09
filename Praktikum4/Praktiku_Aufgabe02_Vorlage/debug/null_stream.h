

#pragma once


class Null_Stream {
public:


	Null_Stream(){};


	template <typename T>
	Null_Stream& operator << (__attribute__((unused)) T value){
		return *this;
	};


	template <typename T>
	Null_Stream& operator <<(__attribute__((unused)) T& (*f) (T&)){
		return *this;
	}
};

extern Null_Stream nullstream;
