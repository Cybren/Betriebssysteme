#include "user/app2/kappltest.h"

#include "device/cga_stream.h"
extern CGA_Stream kout;
#include "syscall/guarded_keyboard.h"
extern Guarded_Keyboard keyboard;
#include "syscall/guarded_semaphore.h"
extern Guarded_Semaphore koutsem;

#include "syscall/guarded_bell.h"

#include "user/app2/World.h"

void KeyboardApplicationTest::action(){
    int length = 2;
    int width = 2;
    World* world = new World(length, width);

    if (world->handleKeyboardInput(1, 1, 1, 1) == -1){
        kout << "test 1 failed"<< endl;
    } else{
        kout << "test 1 succeeded" << endl;
    }
    if(  world->handleKeyboardInput(1, 1, 1, 1) == -1){
        kout << "test 2 failed" << endl;
    }else{
        kout << "test 2 succeeded" << endl;
    }
    if(  world->handleKeyboardInput(1, 2, 0, 0) == -1){
        kout << "test 3 failed" << endl;
    }else{
        kout << "test 3 succeeded" << endl;
    }
    if( world->handleKeyboardInput(1, 1, 0, 0) == -1){
        kout << "test 4 failed"<< endl;
    }else{
        kout << "test 4 succeeded" << endl;
    }
    if(world->handleKeyboardInput(2, 2, 0, 1) == -1){
        kout << "test 5 failed" << endl;
    }else{
        kout << "test 5 succeeded" << endl;
    }
    if(world->handleKeyboardInput(1, 1, 1, 0) == -1){
        kout << "test 6 failed" << endl;
    }else{
        kout << "test 6 succeeded" << endl;
    }
    if(world->handleKeyboardInput(1, 1, 1, 0) == -1){
        kout << "test 6 failed" << endl;
    }else{
        kout << "test 6 succeeded" << endl;
    }
    if(world->handleKeyboardInput(1, 1, 1, 0) == -1){
        kout << "test 6 failed" << endl;
    }else{
        kout << "test 6 succeeded" << endl;
    }

    world->printField();


    delete world;

    while (true) {
    }
}

