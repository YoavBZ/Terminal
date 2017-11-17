#ifndef FILES_H_
#define FILES_H_

#include <string>
#include <vector>

using namespace std;

class BaseFile {
private:
    string name;

public:
    BaseFile(string name);

    string getName() const;

    void setName(string newName);

    virtual int getSize() const = 0;

    virtual bool isFile() =0; //added
    virtual ~BaseFile();
    virtual string toString()=0;
};

class File : public BaseFile {
private:
    int size;

public:
    File(string name, int size); // Constructor
    int getSize() const; // Return the size of the file
    bool isFile(); // added
    virtual string toString();
};

class Directory : public BaseFile {
private:
    vector<BaseFile *> children;
    Directory *parent;

    //clean
    void clean();

    void copyChildren(const Directory &other); //deep copy of children

public:
    Directory(string name, Directory *parent); // Constructor
    Directory *getParent() const; // Return a pointer to the parent of this directory
    void setParent(Directory *newParent); // Change the parent of this directory
    void addFile(BaseFile *file); // Add the file to children
    void removeFile(string name); // Remove the file with the specified name from children
    void removeFile(BaseFile *file); // Remove the file from children
    void removeChildPointer(int i);
    void sortByName(); // Sort children by name alphabetically (not recursively)
    void sortBySize(); // Sort children by size (not recursively)
    vector<BaseFile *> getChildren(); // Return children
    int getSize() const; // Return the size of the directory (recursively)
    string getAbsolutePath();//Return the path from the root to this
    string getAbsolutePathR();
    virtual  string toString();
    // Rule of 5
    virtual ~Directory(); // Distructor
    Directory(const Directory &other); // Copy constructor
    Directory(Directory &&other); // Move constructor
    Directory &operator=(const Directory &other); // Copy assignment operator
    Directory &operator=(Directory &&other); // Move assignment operator
    bool isFile(); // added
    bool nameExist(string name);
};

#endif