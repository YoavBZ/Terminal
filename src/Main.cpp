#include <iostream>
#include "../include/Files.h"
#include "../include/Commands.h"
#include "../include/Environment.h"

using namespace std;
unsigned int verbose = 0;
Environment geten(){
    Environment e;
    e.addToHistory(new CdCommand("d1"));
    return e;
}

Directory getdir(){
    Directory d=Directory("d", nullptr);
    d.addFile(new File("file",7));
    return d;
}

FileSystem getfs(){
    FileSystem fs;
    fs.getRootDirectory().addFile(new File("new",1));
    return fs;
}

int main(int, char **) {

    Environment e;

    FileSystem &fs = e.getFileSystem();
    const vector<BaseCommand *> &history = e.getHistory();
    fs.getRootDirectory().addFile(new Directory("dir1", &fs.getWorkingDirectory()));
    fs.getRootDirectory().addFile(new Directory("dir2", &fs.getWorkingDirectory()));
    fs.getRootDirectory().addFile(new Directory("dir3", &fs.getWorkingDirectory()));

    e.addToHistory(new CdCommand("dir1"));
    history[0]->execute(fs);
    fs.getWorkingDirectory().addFile(new Directory("dir11", &fs.getWorkingDirectory()));
    fs.getRootDirectory().addFile(new File("file12", 6));

    e.addToHistory(new MkdirCommand("/dir2/dir21/dir211"));
    history[1]->execute(fs);

    e.addToHistory(new MkfileCommand("/dir2/dir21/file212 3"));
    e.addToHistory(new MkfileCommand("/dir2/dir21/dir211/file2111 4"));
    e.addToHistory(new MkfileCommand("/dir3/file31 5"));
    history[2]->execute(fs);
    history[3]->execute(fs);
    history[4]->execute(fs);

    e.addToHistory(new MvCommand("/dir2/dir21/dir211 ../dir3"));
    history[5]->execute(fs);

    e.addToHistory(new PwdCommand(""));
    history[6]->execute(fs);

    e.addToHistory(new LsCommand("-s /"));
    history[7]->execute(fs);

    e.addToHistory(new RenameCommand("/dir1 dir1new"));
    history[8]->execute(fs);

    e.addToHistory(new RmCommand("/dir3"));
    history[9]->execute(fs);

    e.addToHistory(new VerboseCommand("3"));
    history[10]->execute(fs);

    e.addToHistory(new HistoryCommand("", history));
    history[11]->execute(fs);

    e.addToHistory(new ExecCommand("7", history));
    history[12]->execute(fs);

    e.addToHistory(new ErrorCommand("asdf asdf asdf"));
    history[13]->execute(fs);

    FileSystem fsCopy = fs;
    FileSystem f;
    f.getWorkingDirectory().addFile(new File("file", 4));
    f = fs;
    f = getfs();
    FileSystem *fss = new FileSystem(getfs());


    Environment envCopy = e;
    Environment en;
    en.getFileSystem().getRootDirectory().addFile(new File("file", 5));
    en = e;
    en = geten();
    Environment* e1 = new Environment(geten());

    Directory *d2 = new Directory("tal", nullptr);
    Directory *d3 = new Directory("y", nullptr);
    d3->addFile(new Directory("o", d3));
    *d2 = getdir();
    Directory *d1 = new Directory(getdir());

    delete d2;
    delete d3;
    delete d1;
    delete fss;
    delete e1;

//    File *file1 = new File("file1", 5);
//    File *file2 = new File("file2", 4);
//
//    Directory &rootDir = fs->getRootDirectory();
//    rootDir.addFile(file1);
//    rootDir.addFile(file2);
//
//    Directory *dir1 = new Directory("dir1", &rootDir);
//    rootDir.addFile(dir1);
//
//    File *file3 = new File("file3", 1);
//    dir1->addFile(file3);
//    File *file4 = new File("file4", 2);
//    dir1->addFile(file4);
//
//    Directory *dir2 = new Directory("dir2", dir1);
//    dir1->addFile(dir2);
//
//    rootDir.removeChildPointer(1);

    return 0;
}