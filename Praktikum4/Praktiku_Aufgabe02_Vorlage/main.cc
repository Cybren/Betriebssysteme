#include "device/cga_stream.h"
#include "debug/output.h"
#include "user/app1/appl.h"

CGA_Stream kout(0, 79, 0, 16, true);
CGA_Stream dout(0, 79, 17, 24);

extern "C" int main() {
	kout.setpos(37, 0);
	kout << "Testausgabe BS" << endl;

	DBG << "[DBG] CPU ready for App" << endl;

	Application application(0);

	application.action();

	return 0;
}

