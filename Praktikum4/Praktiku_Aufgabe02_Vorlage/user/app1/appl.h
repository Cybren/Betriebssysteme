

#pragma once


class Application
{

	Application(const Application&)            = delete;
	Application& operator=(const Application&) = delete;

private:
	int id;
public:

	Application(int i = 0) : id(i) {}


	void setID(int i) {
		id = i;
	}


	void action ();
};
