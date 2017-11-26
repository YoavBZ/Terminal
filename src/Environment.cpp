//
// Created by tal on 14/11/17.
//


using namespace std;

#include <string>
#include <algorithm>
#include <iostream>
#include "../include/Environment.h"
#include "../include/GlobalVariables.h"


// Environment
Environment::Environment() : commandsHistory(), fs() {}

void Environment::start() {
    string input;
    string command;
    string args;
    cout << fs.getWorkingDirectory().getAbsolutePath() + '>';
    getline(cin, input);
    while (input != "exit") {
        int firstSpace = input.find(' ');
        if (firstSpace != -1) {
            command = input.substr(0, firstSpace);
            args = input.substr(firstSpace + 1, input.size() - firstSpace - 1);
        } else {
            command = input;
            args = "";
        }
        BaseCommand *baseCommand;
        if (command == "pwd")
            baseCommand = new PwdCommand(args);
        else if (command == "cd")
            baseCommand = new CdCommand(args);
        else if (command == "ls")
            baseCommand = new LsCommand(args);
        else if (command == "mkdir")
            baseCommand = new MkdirCommand(args);
        else if (command == "mkfile")
            baseCommand = new MkfileCommand(args);
        else if (command == "cp")
            baseCommand = new CpCommand(args);
        else if (command == "mv")
            baseCommand = new MvCommand(args);
        else if (command == "rename")
            baseCommand = new RenameCommand(args);
        else if (command == "rm")
            baseCommand = new RmCommand(args);
        else if (command == "history")
            baseCommand = new HistoryCommand(args, commandsHistory);
        else if (command == "verbose")
            baseCommand = new VerboseCommand(args);
        else if (command == "exec")
            baseCommand = new ExecCommand(args, commandsHistory);
        else
            baseCommand = new ErrorCommand(input);
        if (verbose == 2 || verbose == 3) {
            cout << input << endl;
        }
        baseCommand->execute(fs);
        addToHistory(baseCommand);
        cout << fs.getWorkingDirectory().getAbsolutePath() + '>';
        getline(cin, input);
    }
}

void Environment::addToHistory(BaseCommand *command) {
    commandsHistory.push_back(command);
}

FileSystem &Environment::getFileSystem() {
    return (FileSystem &) fs;
}

const vector<BaseCommand *> &Environment::getHistory() const {
    return commandsHistory;
}

Environment::Environment(const Environment &other) : commandsHistory(), fs(other.fs) {
    if (verbose == 1 || verbose == 3)
        cout << "Environment::Environment(const Environment &other)" << endl;
    copyCommands(other);
}

void Environment::copyCommands(const Environment &other) {
    for (BaseCommand *baseCommand:other.commandsHistory) {
        string tostring = baseCommand->toString();
        if (tostring == "pwd")
            commandsHistory.push_back(new PwdCommand(baseCommand->getArgs()));
        else if (tostring == "cd")
            commandsHistory.push_back(new CdCommand(baseCommand->getArgs()));
        else if (tostring == "ls")
            commandsHistory.push_back(new LsCommand(baseCommand->getArgs()));
        else if (tostring == "mkdir")
            commandsHistory.push_back(new MkdirCommand(baseCommand->getArgs()));
        else if (tostring == "mkfile")
            commandsHistory.push_back(new MkfileCommand(baseCommand->getArgs()));
        else if (tostring == "cp")
            commandsHistory.push_back(new CpCommand(baseCommand->getArgs()));
        else if (tostring == "mv")
            commandsHistory.push_back(new MvCommand(baseCommand->getArgs()));
        else if (tostring == "rename")
            commandsHistory.push_back(new RenameCommand(baseCommand->getArgs()));
        else if (tostring == "rm")
            commandsHistory.push_back(new RmCommand(baseCommand->getArgs()));
        else if (tostring == "history")
            commandsHistory.push_back(new HistoryCommand(baseCommand->getArgs(), other.commandsHistory));
        else if (tostring == "verbose")
            commandsHistory.push_back(new VerboseCommand(baseCommand->getArgs()));
        else if (tostring == "exec")
            commandsHistory.push_back(new ExecCommand(baseCommand->getArgs(), other.commandsHistory));
        else
            commandsHistory.push_back(new ErrorCommand(baseCommand->getArgs()));

    }
}

Environment::Environment(Environment &&other) : commandsHistory(), fs(other.fs) {
    if (verbose == 1 || verbose == 3)
        cout << "Environment::Environment(Environment &&other)" << endl;
    steal(other);
}

Environment &Environment::operator=(const Environment &other) {
    if (verbose == 1 || verbose == 3)
        cout << "Environment& Environment::operator=(const Environment &other)" << endl;
    if (this != &other) {
        fs = other.fs;
        clean();
        copyCommands(other);
    }
    return *this;

}

Environment &Environment::operator=(Environment &&other) {
    if (verbose == 1 || verbose == 3)
        cout << "Environment& Environment::operator=(Environment &&other)" << endl;
    if (this != &other) {
        fs = other.getFileSystem();
        clean();
        steal(other);
    }
    return *this;
}

void Environment::steal(Environment &other) {
    for (BaseCommand *baseCommand: other.commandsHistory) {
        commandsHistory.push_back(baseCommand);
    }
    other.commandsHistory.clear();
}

void Environment::clean() {
    for (BaseCommand *baseCommand:commandsHistory) {
        delete baseCommand;
    }
    commandsHistory.clear();
}

Environment::~Environment() {
    if (verbose == 1 || verbose == 3)
        cout << "Environment::~Environment()" << endl;
    clean();
}