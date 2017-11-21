#ifndef ENVIRONMENT_H_
#define ENVIRONMENT_H_

#include "Files.h"
#include "Commands.h"

#include <string>
#include <vector>

using namespace std;

class Environment {
private:
	vector<BaseCommand*> commandsHistory;
	FileSystem fs;
	void copyCommands(const Environment &other);
	void clean();
	void steal(Environment &other);

public:
	Environment();
	void start();
	FileSystem& getFileSystem() const; // Get a reference to the file system
	void addToHistory(BaseCommand *command); // Add a new command to the history
	const vector<BaseCommand*>& getHistory() const; // Return a reference to the history of commands
	Environment (const Environment& other);// Copy constructor
	Environment (Environment &&other);// Move constructor
	Environment &operator=(const Environment &other);// Copy assignment operator
	Environment &operator=(Environment &&other);// Move assignment operator
	~Environment();
};

#endif