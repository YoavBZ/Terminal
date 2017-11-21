#include "../include/Environment.h"

// ... You may not change this file

unsigned int verbose = 0;

Directory& getDir() {
     Directory d("name", nullptr);
    return d;
}

int main(int, char **) {
//    Environment env;
//    env.start();
    verbose = 1;
    Directory *dir1= new Directory("dir1", nullptr);
//    Directory *par2 = new Directory("par2", nullptr);
    Directory *dir2 = new Directory("dir2", nullptr);
    *dir1 = std::move(*dir2);
//    Directory *dir3 = new Directory(*dir2);
//     Directory dir4 ("name", nullptr);
//    Directory *dir5 = new Directory("dir5", nullptr);
//    *dir5 = getDir("dir6");
//    delete dir1;
//    delete dir2;
//    delete dir3;
//    delete dir4;
//    delete dir5;
//    delete par2;
    return 0;
}
