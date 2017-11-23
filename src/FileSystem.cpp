//
// Created by tal on 14/11/17.
//

using namespace std;

#include <string>
#include "../include/GlobalVariables.h"
#include <iostream>
#include "../include/FileSystem.h"
#include "../include/Commands.h"

// FileSystem
FileSystem::FileSystem() : rootDirectory(new Directory("/", nullptr)), workingDirectory(rootDirectory) {}

Directory &FileSystem::getRootDirectory() const { return *rootDirectory; }

Directory &FileSystem::getWorkingDirectory() const { return *workingDirectory; }

void FileSystem::setWorkingDirectory(Directory *newWorkingDirectory) {
    workingDirectory = newWorkingDirectory;
}

FileSystem::FileSystem(const FileSystem &other) : rootDirectory(new Directory(*other.rootDirectory)), workingDirectory(
        nullptr) {
    workingDirectory = (Directory *) BaseCommand::getPointer(rootDirectory, nullptr,
                                                             other.workingDirectory->getRecursivePath());
    if (verbose == 1 || verbose == 3)
        cout << "FileSystem::FileSystem(const FileSystem &other)" << endl;
}

FileSystem::FileSystem(FileSystem &&other) : rootDirectory(other.rootDirectory),
                                             workingDirectory(other.workingDirectory) {
    if (verbose == 1 || verbose == 3)
        cout << "FileSystem::FileSystem(FileSystem &&other)" << endl;
    other.workingDirectory = nullptr;
    other.rootDirectory = nullptr;
}

FileSystem &FileSystem::operator=(const FileSystem &other) {
    if (verbose == 1 || verbose == 3)
        cout << "FileSystem& FileSystem::operator=(const FileSystem &other)" << endl;
    if (this != &other) {
        *rootDirectory = *other.rootDirectory;
        workingDirectory = (Directory *) BaseCommand::getPointer(rootDirectory, nullptr,
                                                                 other.workingDirectory->getRecursivePath());
    }
    return *this;
}

FileSystem &FileSystem::operator=(FileSystem &&other) {
    if (verbose == 1 || verbose == 3)
        cout << "FileSystem& FileSystem::operator=(FileSystem &&other)" << endl;
    if (this != &other) {
        delete rootDirectory;
        rootDirectory = nullptr;
        workingDirectory = nullptr;
        rootDirectory = other.rootDirectory;
        workingDirectory = other.workingDirectory;
        other.workingDirectory = nullptr;
        other.rootDirectory = nullptr;
    }
    return *this;
}

FileSystem::~FileSystem() {
    if (verbose == 1 || verbose == 3)
        cout << "FileSystem::~FileSystem()" << endl;
    delete rootDirectory;
}