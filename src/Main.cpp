#include <iostream>
#include "../include/Files.h"
#include "../include/Commands.h"

using namespace std;
unsigned int verbose = 0;

int main(int, char **) {

    FileSystem *fs = new FileSystem();
    File *file1 = new File("file1", 5);
    File *file2 = new File("file2", 4);

    Directory &rootDir = fs->getRootDirectory();
    rootDir.addFile(file1);
    rootDir.addFile(file2);

    Directory *dir1 = new Directory("dir1", &rootDir);
    rootDir.addFile(dir1);

    File *file3 = new File("file3", 1);
    dir1->addFile(file3);
    File *file4 = new File("file4", 2);
    dir1->addFile(file4);

    cout << rootDir.getSize() << endl;

    CdCommand command("dir1");
    command.execute(*fs);

    return 0;
}