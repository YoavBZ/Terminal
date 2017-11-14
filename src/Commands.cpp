using namespace std;

#include <string>

#include <algorithm>
#include <iostream>
#include "../include/Commands.h"

// BaseCommand
BaseCommand::BaseCommand(string args) : args(args) {}

string BaseCommand::getArgs() { return args; }

vector<string> BaseCommand::parseArgs(string args) {
    vector<string> parsed;
    size_t found = args.find(' ');
    while (found != string::npos) {
        parsed.push_back(args.substr(0, found));
        args = args.substr(found + 1, args.size() - found - 1);
        found = args.find(' ');
    }
    parsed.push_back(args);
    return parsed;
}

Directory *BaseCommand::getPath(Directory *rootDir, Directory *currentDir, string path) {
    if (path.empty())
        return nullptr;
    if (path[0] != '/') {
        // Relative path
        size_t found = path.find('/');
        if (found == string::npos) {
            for (BaseFile *baseFile:currentDir->getChildren()) {
                if (baseFile->getName() == path.substr(0, found))
                    currentDir = getPath(rootDir, (Directory *) baseFile, path.substr(found, path.size() - found - 1));
            }
        }
    } else {
        // Absolute path
        currentDir = getPath(rootDir, rootDir, path.substr(1, path.size() - 1));
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
    vector<string> parse = parseArgs(getArgs());
    fs.setWorkingDirectory(getPath(&fs.getRootDirectory(), &fs.getWorkingDirectory(), parse[0]));
}

string CdCommand::toString() { return "cd"; }

// LsCommand
LsCommand::LsCommand(string args) : BaseCommand(args) {}

void LsCommand::execute(FileSystem &fs) {}

string LsCommand::toString() { return "ls"; }

// MkdirCommand
MkdirCommand::MkdirCommand(string args) : BaseCommand(args) {}

void MkdirCommand::execute(FileSystem &fs) {}

string MkdirCommand::toString() { return "mkdir"; }

// MkfileCommand
MkfileCommand::MkfileCommand(string args) : BaseCommand(args) {}

void MkfileCommand::execute(FileSystem &fs) {
    vector<string> arguments = parseArgs(getArgs());
    for (string s : arguments) {
        cout << s << endl;
    }
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
