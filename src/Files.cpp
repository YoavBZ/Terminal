
using namespace std;

#include <string>
#include <algorithm>
#include <iostream>
#include "../include/Files.h"
#include "../include/GlobalVariables.h"

// ** BaseFile **
BaseFile::BaseFile(string name) : name(std::move(name)) {}

string BaseFile::getName() const {
    return name;
}

void BaseFile::setName(string newName) {
    name = newName;
}

BaseFile::~BaseFile() {}

// ** File **
File::File(string name, int size) : BaseFile(name), size(size) {}

int File::getSize() const {
    return size;
}

bool File::isFile() { return true; }

string File::toString() {
    return "FILE\t" + getName() + "\t" + to_string(size);
}


// ** Directory **
Directory::Directory(string name, Directory *parent) : BaseFile(name), children(), parent(parent) {}

Directory *Directory::getParent() const { return parent; }

void Directory::setParent(Directory *newParent) {
    parent = newParent;
}

vector<BaseFile *> Directory::getChildren() { return children; }

int Directory::getSize() const {
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
    delete file;
    int i = 0;
    for (BaseFile *baseFile: children) {
        if (baseFile == file)
            break;
        i++;
    }
    removeChildPointer(i);
}

void Directory::removeChildPointer(int index) {
    children.erase(children.begin() + index);
}

string Directory::getAbsolutePath() {
    string path = getAbsolutePathR();
    if (path.empty())
        return "/";
    return path;
}

string Directory::getAbsolutePathR() {
    if (!parent) {
        return "";
    }
    return parent->getAbsolutePathR() + "/" + getName();
}

bool nameCompare(const BaseFile *a, const BaseFile *b) {
    return a->getName().compare(b->getName()) < 0;
}

bool sizeCompare(BaseFile *a, BaseFile *b) {
    return (a->getSize() < b->getSize());
}

void Directory::sortByName() {
    sort(children.begin(), children.end(), nameCompare);
}

void Directory::sortBySize() {
    sort(children.begin(), children.end(), sizeCompare);
}

bool Directory::isFile() { return false; }

Directory::Directory(const Directory &other) : BaseFile(other.getName()), children(), parent(other.getParent()) {
    if (verbose == 1 || verbose == 3)
        cout << "Directory::Directory(const Directory &other)" << endl;
    copyChildren(other);
}

Directory::Directory(Directory &&other) : BaseFile(other.getName()), children(), parent(other.getParent()) {
    if (verbose == 1 || verbose == 3)
        cout << "Directory::Directory(Directory &&other)" << endl;

    for (BaseFile *baseFile: other.children) {
        addFile(baseFile);
    }
    other.setParent(nullptr);
    other.getChildren().clear();
}


void Directory::clean() {
    parent = nullptr;
    for (BaseFile *baseFile: children) {
        delete baseFile;
    }
    children.clear();

}

void Directory::copyChildren(const Directory &other) {
    for (BaseFile *baseFile: other.children) {
        if (baseFile->isFile()) {
            BaseFile *copy = new File((File &) *baseFile);
            addFile(copy);
        } else {
            BaseFile *copy = new Directory((Directory &) *baseFile);
            addFile(copy);
        }

    }
}

Directory &Directory::operator=(const Directory &other) {
    if (verbose == 1 || verbose == 3)
        cout << "Directory &Directory::operator=(Directory &other)" << endl;
    if (this != &other) {
        clean();
        parent = other.getParent();
        setName(other.getName());
        copyChildren(other);
    }

    return *this;
}

Directory &Directory::operator=(Directory &&other) {
    // verbose
    if (verbose == 1 || verbose == 3)
        cout << "Directory &Directory::operator=(Directory &&other)" << endl;
    if (this != &other) {
        clean();
        setName(other.getName());
        parent = other.getParent();
        for (BaseFile *baseFile: other.children) {
            addFile(baseFile);
        }
        other.setParent(nullptr);
        other.getChildren().clear();
    }

    return *this;
}

Directory::~Directory() {
    // verbose
    if (verbose == 1 || verbose == 3)
        cout << "Directory::~Directory()" << endl;
    clean();
}

string Directory::toString() {
    return "DIR\t" + getName() + "\t" + to_string(getSize());
}

bool Directory::nameExist(string name) {
    for (BaseFile *baseFile: children) {
        if (baseFile->getName() == name)
            return true;
    }
    return false;
}