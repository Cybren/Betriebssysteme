#include <iostream>
#include <sys/stat.h>
#include <fstream>
#include <unistd.h>

int main() {

    mode_t mode = 0000700;
    mkdir("file", mode);
    chdir("file/");
    std::ofstream file { "test.txt" };
    std::fstream f;
    f.open("test.txt");
    f << "123" << std::endl;
    f.close();
    
    return 0;
}