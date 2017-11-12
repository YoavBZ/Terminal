
using namespace std;

#include <string>
#include <algorithm>
#include <iostream>
#include "../include/Files.h"

// ** BaseFile **
BaseFile::BaseFile(string name) : name(std::move(name)) {}

string BaseFile::getName() const {
    return name;
}

void BaseFile::setName(string newName) {
    name = newName;
}

// ** File **
File::File(string name, int size) : BaseFile(name), size(size) {}

int File::getSize() {
    cout << "getSize() of " + getName() + " = " << size << endl;
    return size;
}

// ** Directory **
Directory::Directory(string name, Directory *parent) : BaseFile(name), parent(parent), children() {}

Directory *Directory::getParent() const { return parent; }

void Directory::setParent(Directory *newParent) {
    parent = newParent;
}

vector<BaseFile *> Directory::getChildren() { return children; }

int Directory::getSize() {
    int totalSize = 0;
    for (BaseFile *baseFile : children) {
        totalSize += baseFile->getSize();
    }
    return totalSize;
}

void Directory::addFile(BaseFile *file) {
    children.push_back(file);
}

void Directory::removeFile(string name) {
    for (BaseFile *baseFile : children) {
        if (baseFile->getName() == name) {
            removeFile(baseFile);
            break;
        }
    }
}

void Directory::removeFile(BaseFile *file) {
    children.erase(std::remove(children.begin(), children.end(), file), children.end());
}

string Directory::getAbsolutePath() {
    if (!parent) {
        return "";
    }
    return parent->getAbsolutePath() + "/" + getName();
}

void Directory::sortByName() {
    sort(children.begin(), children.end(), greater<string>());
};

