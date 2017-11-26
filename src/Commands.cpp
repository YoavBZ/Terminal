using namespace std;

#include <string>
#include "../include/GlobalVariables.h"
#include <algorithm>
#include <iostream>
#include "../include/Commands.h"

// BaseCommand
BaseCommand::BaseCommand(string args) : args(args) {}

string BaseCommand::getArgs() { return args; }

vector<string> BaseCommand::split(string str, char delimiter) {//return vector represent the splited string according to the delimiter recived
    vector<string> parsed;
    size_t found = str.find(delimiter);
    while (found != string::npos) {
        parsed.push_back(str.substr(0, found));
        str = str.substr(found + 1, str.size() - found - 1);
        found = str.find(delimiter);
    }
    parsed.push_back(str);
    return parsed;
}

BaseFile *BaseCommand::getPointer(Directory *rootDir, Directory *currentDir, string path) { //return a pointer to the basefile which the path recived represent
    vector<string> parsePath = split(path, '/');
    unsigned int i = 0;
    if (parsePath[0] == "") {
        if (parsePath.size() == 1)
            return currentDir;
        if (parsePath[1] == "" && parsePath.size() == 2)
            return rootDir;
        i++;
        currentDir = rootDir;
    }
    for (; i < parsePath.size(); i++) {
        if (parsePath[i] == "..") {
            if (currentDir->getParent() == nullptr)
                return nullptr;
            else
                currentDir = currentDir->getParent();
        } else {
            bool found = false;
            for (BaseFile *baseFile:currentDir->getChildren()) {
                if (baseFile->getName() == parsePath[i]) {
                    if (!baseFile->isFile()) {
                        currentDir = (Directory *) baseFile;
                        found = true;
                        break;
                    } else if (i == parsePath.size() - 1)
                        return baseFile;
                }
            }
            if (!found)
                return nullptr;
        }
    }
    return currentDir;
}

BaseCommand::~BaseCommand() = default;

// PwdCommand
PwdCommand::PwdCommand(string args) : BaseCommand(args) {}

void PwdCommand::execute(FileSystem &fs) {
    cout << fs.getWorkingDirectory().getAbsolutePath() << endl;
}

string PwdCommand::toString() { return "pwd"; }

// CdCommand
CdCommand::CdCommand(string args) : BaseCommand(args) {}

void CdCommand::execute(FileSystem &fs) {
    BaseFile *newWorkingDirectory = getPointer(&fs.getRootDirectory(), &fs.getWorkingDirectory(), getArgs());
    if (newWorkingDirectory != nullptr && !newWorkingDirectory->isFile())
        fs.setWorkingDirectory((Directory *) newWorkingDirectory);
    else
        cout << "The system cannot find the path specified" << endl;
}

string CdCommand::toString() { return "cd"; }

// LsCommand
LsCommand::LsCommand(string args) : BaseCommand(args) {
}

void LsCommand::execute(FileSystem &fs) {
    vector<string> parse = split(getArgs(), ' ');
    BaseFile *dir;
    if ((parse.size() == 1) && (parse[0] == "-s" || parse[0].empty()))
        dir = &fs.getWorkingDirectory();
    else
        dir = getPointer(&fs.getRootDirectory(), &fs.getWorkingDirectory(), parse.back());

    if (dir == nullptr || dir->isFile()) {
        cout << "The system cannot find the path specified" << endl;
        return;
    }
    ((Directory *) dir)->sortByName();
    if (parse[0] == "-s") {
        ((Directory *) dir)->sortBySize();
    }
    for (BaseFile *baseFile: ((Directory *) dir)->getChildren()) {
        cout << baseFile->toString() << endl;
    }
}

string LsCommand::toString() { return "ls"; }

// MkdirCommand
MkdirCommand::MkdirCommand(string args) : BaseCommand(args) {}

void MkdirCommand::execute(FileSystem &fs) {
    vector<string> path = split(getArgs(), '/');

    unsigned int i = 0;
    Directory *currentDir = &fs.getWorkingDirectory();
    if (path[0] == "") {
        if (path.size() == 1) {
            cout << "The system cannot find the path specified" << endl;
            return;
        }
        if (path[1] == "" && path.size() == 2) {
            cout << "The directory already exists" << endl;
            return;
        }
        i++;
        currentDir = &fs.getRootDirectory();
    }
    for (; i < path.size(); i++) {
        if (path[i] == "..") {
            if (currentDir->getParent() == nullptr) {
                cout << "The system cannot find the path specified" << endl;
                return;
            } else
                currentDir = currentDir->getParent();
        } else {
            bool found = false;
            for (BaseFile *baseFile:currentDir->getChildren()) {
                if (baseFile->getName() == path[i]) {
                    if (!baseFile->isFile()) {
                        currentDir = (Directory *) baseFile;
                        found = true;
                        break;
                    } else {
                        cout << "The directory already exists" << endl;
                        return;
                    }
                }
            }
            if (!found)
                break;
        }
    }
    if (i == path.size()) {
        cout << "The directory already exists" << endl;
        return;
    }
    for (; i < path.size(); i++) {
        if (path[i] == "..") {
            cout << "The system cannot find the path specified" << endl;
            return;
        }
        Directory *newDir = new Directory(path[i], currentDir);
        currentDir->addFile(newDir);
        currentDir = newDir;
    }
}

string MkdirCommand::toString() { return "mkdir"; }

// MkfileCommand
MkfileCommand::MkfileCommand(string args) : BaseCommand(args) {}

void MkfileCommand::execute(FileSystem &fs) {
    vector<string> arguments = split(getArgs(), ' ');
    vector<string> path = split(arguments[0], '/');
    string name = path[path.size() - 1];
    string dirPath = arguments[0].substr(0, arguments[0].size() - name.size());
    BaseFile *dir;
    if (dirPath == "")
        dir = getPointer(&fs.getRootDirectory(), &fs.getWorkingDirectory(),
                         dirPath.substr(0, dirPath.size()));
    else
        dir = getPointer(&fs.getRootDirectory(), &fs.getWorkingDirectory(),
                         dirPath.substr(0, dirPath.size() - 1));
    if (dir == nullptr || dir->isFile()) {
        cout << "The system cannot find the path specified" << endl;
        return;
    }
    for (BaseFile *baseFile : ((Directory *) dir)->getChildren()) {
        if (baseFile->getName() == name) {
            cout << "File already exists" << endl;
            return;
        }
    }
    ((Directory *) dir)->addFile(new File(name, stoi(arguments[1])));
}

string MkfileCommand::toString() { return "mkfile"; }

// CpCommand
CpCommand::CpCommand(string args) : BaseCommand(args) {}

void CpCommand::execute(FileSystem &fs) {
    vector<string> parse = split(getArgs(), ' ');
    BaseFile *destinationPath = getPointer(&fs.getRootDirectory(), &fs.getWorkingDirectory(), parse[1]);
    if (destinationPath == nullptr || destinationPath->isFile()) {
        cout << "No such file or directory" << endl;
        return;
    }
    BaseFile *toCopy = getPointer(&fs.getRootDirectory(), &fs.getWorkingDirectory(), parse[0]);
    if (toCopy == nullptr) {
        cout << "No such file or directory" << endl;
        return;
    }
    for (BaseFile *baseFile: ((Directory *) destinationPath)->getChildren()) {
        if (baseFile->getName() == toCopy->getName()) {
            return;
        }
    }
    if (toCopy->isFile())
        ((Directory *) destinationPath)->addFile(new File((File &) *toCopy));
    else {
        ((Directory *) destinationPath)->addFile(new Directory((Directory &) *toCopy));
        ((Directory *) toCopy)->setParent((Directory *) destinationPath);
    }

}

string CpCommand::toString() { return "cp"; }

// MvCommand
MvCommand::MvCommand(string args) : BaseCommand(args) {}

void MvCommand::execute(FileSystem &fs) {
    vector<string> parse = split(getArgs(), ' ');
    Directory *root = &fs.getRootDirectory();
    Directory *workingDirectory = &fs.getWorkingDirectory();
    BaseFile *destinationPath = getPointer(root, workingDirectory, parse[1]);
    if (destinationPath == nullptr || destinationPath->isFile()) {
        cout << "No such file or directory" << endl;
        return;
    }
    BaseFile *toMove = getPointer(root, workingDirectory, parse[0]);
    if (toMove == nullptr) {
        cout << "No such file or directory" << endl;
        return;
    }
    if (!toMove->isFile() && workingDirectory->getAbsolutePath().find(((Directory *) toMove)->getAbsolutePath()) == 0) {
        // Checks whether toMove is pointed workingDirectory or any parent of workingDirectory (including root)
        cout << "Can't move directory" << endl;
        return;
    }
    string toMoveParentPath = parse[0].substr(0, parse[0].size() - toMove->getName().size());
    BaseFile *parent;
    if (toMoveParentPath == "/")
        parent = getPointer(root, workingDirectory, toMoveParentPath.substr(0, toMoveParentPath.size()));
    else
        parent = getPointer(root, workingDirectory, toMoveParentPath.substr(0, toMoveParentPath.size() - 1));
    int i = 0;
    for (BaseFile *baseFile:((Directory *) parent)->getChildren()) {
        if (baseFile == toMove)
            break;
        i++;
    }
    for (BaseFile *baseFile: ((Directory *) destinationPath)->getChildren()) {
        if (baseFile->getName() == toMove->getName()) {
            return;
        }
    }
    if (!toMove->isFile())
        ((Directory *) toMove)->setParent(((Directory *) destinationPath));
    ((Directory *) destinationPath)->addFile(toMove);
    ((Directory *) parent)->removeChildPointer(i);

}

string MvCommand::toString() { return "mv"; }

// RenameCommand
RenameCommand::RenameCommand(string args) : BaseCommand(args) {}

void RenameCommand::execute(FileSystem &fs) {
    vector<string> parse = split(getArgs(), ' ');
    BaseFile *oldNamePointer = getPointer(&fs.getRootDirectory(), &fs.getWorkingDirectory(), parse[0]);
    if (oldNamePointer == nullptr) {
        cout << "No such file or directory" << endl;
        return;
    }
    if (oldNamePointer == &fs.getWorkingDirectory()) {
        cout << "Can't rename the working directory" << endl;
        return;
    }
    if ( oldNamePointer == &fs.getRootDirectory()){
        cout << "Can't rename the root directory" << endl;
        return;
    }
    Directory *parent;
    if (!oldNamePointer->isFile())
        parent = ((Directory *) oldNamePointer)->getParent();
    else {
        string childPath = parse[0].substr(0, parse[0].size() - oldNamePointer->getName().size());
        if (childPath == "/")
            parent = (Directory *) getPointer(&fs.getRootDirectory(), &fs.getWorkingDirectory(),
                                              childPath.substr(0, childPath.size()));
        else
            parent = (Directory *) getPointer(&fs.getRootDirectory(), &fs.getWorkingDirectory(),
                                              childPath.substr(0, childPath.size() - 1));
    }
    if (parent->nameExist(parse[1]))
        return;

    oldNamePointer->setName(parse[1]);
}

string RenameCommand::toString() { return "rename"; }

// RmCommand
RmCommand::RmCommand(string args) : BaseCommand(args) {}

void RmCommand::execute(FileSystem &fs) {
    vector<string> parse = split(getArgs(), ' ');
    Directory *root = &fs.getRootDirectory();
    Directory *workingDirectory = &fs.getWorkingDirectory();
    BaseFile *toRemove = getPointer(root, workingDirectory, parse[0]);
    if (toRemove == nullptr) {
        cout << "No such file or directory" << endl;
        return;
    }
    if (!toRemove->isFile() &&
        workingDirectory->getAbsolutePath().find(((Directory *) toRemove)->getAbsolutePath()) == 0) {
        cout << "Can't remove directory" << endl;
        return;
    }
    Directory *parent;
    if (!toRemove->isFile())
        parent = ((Directory *) toRemove)->getParent();
    else {
        string childPath = parse[0].substr(0, parse[0].size() - toRemove->getName().size());
        if (childPath == "/")
            parent = (Directory *) getPointer(root, workingDirectory, childPath.substr(0, childPath.size()));
        else
            parent = (Directory *) getPointer(root, workingDirectory, childPath.substr(0, childPath.size() - 1));
    }
    parent->removeFile(toRemove);
}

string RmCommand::toString() { return "rm"; }

// HistoryCommand
HistoryCommand::HistoryCommand(string args, const vector<BaseCommand *> &history) : BaseCommand(args),
                                                                                    history(history) {}

void HistoryCommand::execute(FileSystem &fs) {
    int i = 0;
    for (BaseCommand *baseCommand: history) {
        if (baseCommand->toString() == "error")
            cout << to_string(i) + "\t" + baseCommand->getArgs() << endl;
        else
            cout << to_string(i) + "\t" + baseCommand->toString() + " " + baseCommand->getArgs() << endl;
        i++;
    }

}

string HistoryCommand::toString() { return "history"; }

// VerboseCommand
VerboseCommand::VerboseCommand(string args) : BaseCommand(args) {}

void VerboseCommand::execute(FileSystem &fs) {
    if (getArgs() == "0")
        verbose = 0;
    else if (getArgs() == "1")
        verbose = 1;
    else if (getArgs() == "2")
        verbose = 2;
    else if (getArgs() == "3")
        verbose = 3;
    else
        cout << "Wrong verbose input" << endl;
}

string VerboseCommand::toString() { return "verbose"; }

// ErrorCommand
ErrorCommand::ErrorCommand(string args) : BaseCommand(args) {}

void ErrorCommand::execute(FileSystem &fs) {
    cout << getArgs().substr(0, getArgs().find(' ')) + ": " + "Unknown command" << endl;
}

string ErrorCommand::toString() {
    return "error";
}

// ExecCommand
ExecCommand::ExecCommand(string args, const vector<BaseCommand *> &history) : BaseCommand(args), history(history) {}

void ExecCommand::execute(FileSystem &fs) {
    int i = 0;
    for (BaseCommand *baseCommand: history) {
        if (i == stoi(getArgs())) {
            baseCommand->execute(fs);
            return;
        }
        i++;
    }
    cout << "Command not found" << endl;
}

string ExecCommand::toString() { return "exec"; }
