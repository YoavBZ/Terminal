#include <iostream>
#include "../include/Files.h"

using namespace std;
unsigned int verbose = 0;

int main(int, char **) {
    File file1("file1", 5);
    File file2("file2",4);

    Directory rootDir("root", nullptr);
    rootDir.addFile(&file1);
    rootDir.addFile(&file2);

    Directory dir1("dir1", &rootDir);
    rootDir.addFile(&dir1);

    File file3("file3", 1);
    dir1.addFile(&file3);

    cout << rootDir.getSize() << endl;
    cout << dir1.getAbsolutePath() << endl;

    dir1.removeFile("file3");
    cout << rootDir.getSize() << endl;

    rootDir.sortByName();
    cout << rootDir.getSize() << endl;
    return 0;
}