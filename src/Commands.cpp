using namespace std;

#include <string>

#include <algorithm>
#include <iostream>
#include "../include/Commands.h"

// BaseCommand
BaseCommand::BaseCommand(string args) : args(args) {}

string BaseCommand::getArgs() { return args; }

vector<string> BaseCommand::parseArgs(string args, char delimiter) {
    vector<string> parsed;
    size_t found = args.find(delimiter);
    while (found != string::npos) {
        parsed.push_back(args.substr(0, found));
        args = args.substr(found + 1, args.size() - found - 1);
        found = args.find(delimiter);
    }
    parsed.push_back(args);
    return parsed;
}

Directory *BaseCommand::getPath(Directory *rootDir, Directory *currentDir, string path) {
    vector<string> parsePath = parseArgs(path, '/');
    int i = 0;
    if (parsePath[0] == "") {
        if (parsePath.size() == 1)
            return nullptr;
        if (parsePath[1] == "" && parsePath.size() == 2)
            return rootDir;
        i++;
        currentDir = rootDir;
    }
    for (i; i < parsePath.size(); i++) {
        if (parsePath[i] == "..") {
            if (currentDir->getParent() == nullptr)
                return nullptr;
            else
                currentDir = currentDir->getParent();
        } else {
            bool found = false;
            for (BaseFile *baseFile:currentDir->getChildren()) {
                if (baseFile->getName() == parsePath[i] && !baseFile->isFile()) {
                    currentDir = (Directory *) baseFile;
                    found = true;
                    break;
                }
            }
            if (!found)
                return nullptr;
        }
    }
    return currentDir;
}

// PwdCommand
PwdCommand::PwdCommand(string args) : BaseCommand(args) {}

void PwdCommand::execute(FileSystem &fs) {
    cout << fs.getWorkingDirectory().getAbsolutePath() << endl;
}

string PwdCommand::toString() { return "pwd"; }

// CdCommand
CdCommand::CdCommand(string args) : BaseCommand(args) {}

void CdCommand::execute(FileSystem &fs) {
    vector<string> parse = parseArgs(getArgs(), ' ');
    Directory *newWorkingDirectory = getPath(&fs.getRootDirectory(), &fs.getWorkingDirectory(), parse[0]);
    if (newWorkingDirectory != nullptr)
        fs.setWorkingDirectory(newWorkingDirectory);
    else
        cout << "The system cannot find the path specified" << endl;
}

string CdCommand::toString() { return "cd"; }

// LsCommand
LsCommand::LsCommand(string args) : BaseCommand(args) {
}

void LsCommand::execute(FileSystem &fs) {
    vector<string> parse = parseArgs(getArgs(), ' ');
    Directory *dir = getPath(&fs.getRootDirectory(), &fs.getWorkingDirectory(), parse.back());
    if (dir == nullptr) {
        cout << "The system cannot find the path specified" << endl;
        return;
    }
    if (parse[0] == "-s")
        dir->sortBySize();
    else
        dir->sortByName();
    for (BaseFile *baseFile: dir->getChildren()) {
        cout << baseFile->toString() << endl;
    }
}

string LsCommand::toString() { return "ls"; }

// MkdirCommand
MkdirCommand::MkdirCommand(string args) : BaseCommand(args) {}

void MkdirCommand::execute(FileSystem &fs) {
    vector<string> path = parseArgs(getArgs(), '/');

    int i = 0;
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
    for (i; i < path.size(); i++) {
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
                        cout << "The system cannot find the path specified" << endl;
                        return;
                    }
                }
            }
            if (!found)
                break;
        }
    }
    if (i==path.size()){
        cout << "The directory already exists" << endl;
        return;
    }
    for (i; i < path.size(); i++) {
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
    vector<string> arguments = parseArgs(getArgs(), ' ');
    vector<string> path = parseArgs(arguments[0], '/');
    string name = path[path.size() - 1];
    string dirPath = arguments[0].substr(0,arguments[0].find(name) - 1);
    Directory *dir = getPath(&fs.getRootDirectory(), &fs.getWorkingDirectory(), dirPath);
    if (dir == nullptr) {
        cout << "The system cannot find the path specified" << endl;
        return;
    }
    for (BaseFile *baseFile : dir->getChildren()) {
        if (baseFile->getName() == name) {
            cout << "File already exists" << endl;
            return;
        }
    }
    dir->addFile(new File(name, stoi(arguments[1])));
}

string MkfileCommand::toString() { return "mkfile"; }

// CpCommand
CpCommand::CpCommand(string args) : BaseCommand(args) {}

void CpCommand::execute(FileSystem &fs) {}

string CpCommand::toString() { return "cp"; }

// MvCommand
MvCommand::MvCommand(string args) : BaseCommand(args) {}

void MvCommand::execute(FileSystem &fs) {}

string MvCommand::toString() { return "mv"; }

// RenameCommand
RenameCommand::RenameCommand(string args) : BaseCommand(args) {}

void RenameCommand::execute(FileSystem &fs) {}

string RenameCommand::toString() { return "rename"; }

// RmCommand
RmCommand::RmCommand(string args) : BaseCommand(args) {}

void RmCommand::execute(FileSystem &fs) {}

string RmCommand::toString() { return "rm"; }

// HistoryCommand
HistoryCommand::HistoryCommand(string args, const vector<BaseCommand *> &history) : BaseCommand(args),
                                                                                    history(history) {}

void HistoryCommand::execute(FileSystem &fs) {}

string HistoryCommand::toString() { return "history"; }

// VerboseCommand
VerboseCommand::VerboseCommand(string args) : BaseCommand(args) {}

void VerboseCommand::execute(FileSystem &fs) {}

string VerboseCommand::toString() { return "verbose"; }

// ErrorCommand
ErrorCommand::ErrorCommand(string args) : BaseCommand(args) {}

void ErrorCommand::execute(FileSystem &fs) {}

string ErrorCommand::toString() { return ""; }

// ExecCommand
ExecCommand::ExecCommand(string args, const vector<BaseCommand *> &history) : BaseCommand(args), history(history) {}

void ExecCommand::execute(FileSystem &fs) {}

string ExecCommand::toString() { return "exec"; }
