//
// Created by tal on 14/11/17.
//

using namespace std;

#include <string>

#include <algorithm>
#include <iostream>
#include "../include/FileSystem.h"

// FileSystem
FileSystem::FileSystem() : rootDirectory(new Directory("root", nullptr)), workingDirectory(rootDirectory) {}

Directory &FileSystem::getRootDirectory() const { return *rootDirectory; }

Directory &FileSystem::getWorkingDirectory() const { return *workingDirectory; }

void FileSystem::setWorkingDirectory(Directory *newWorkingDirectory) {
    workingDirectory = newWorkingDirectory;
}
