//
// Created by tal on 14/11/17.
//


using namespace std;

#include <string>
#include <algorithm>
#include <iostream>
#include "../include/Environment.h"

// Environment
Environment::Environment() : commandsHistory(), fs() {}

void Environment::start() {}

void Environment::addToHistory(BaseCommand *command) {
    commandsHistory.push_back(command);
}

FileSystem& Environment::getFileSystem() const {
    return const_cast<FileSystem &>(fs);
}

const vector<BaseCommand*>& Environment::getHistory() const {
    return commandsHistory;
}