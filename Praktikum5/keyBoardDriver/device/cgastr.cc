
#include "device/cgastr.h"

void CGA_Stream::flush() {
    print(buffer, pos
    );
    pos = 0;
}
