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
    string input = "";
    string command;
    string args;
    while (input != "exit") {
        getline(cin, input);
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
        baseCommand->execute(fs);
        commandsHistory.push_back(baseCommand);
        if (verbose == 2 || verbose == 3)
            cout << input << endl;
    }

}

void Environment::addToHistory(BaseCommand *command) {
    commandsHistory.push_back(command);
}

FileSystem &Environment::getFileSystem() const {
    return (FileSystem &) fs;
}

const vector<BaseCommand *> &Environment::getHistory() const {
    return commandsHistory;
}