#include "FileManager.h"
#include <iostream>
#include <cstdio>
#include <sstream>
#include <cstdlib>

bool checkCommandLength(vector<string>& input, const int expected_length)
{
	const int input_length = input.size();
	if (input_length != expected_length)
	{
		cout << input[0] << ": Expected " << expected_length - 1 << " parameters, but given " << input_length - 1 << endl;
		return false;
	}
	return true;
}
bool checkshort(string opname, string target)
{
	if (opname.size() < 4)
		return 0;
	if (opname.substr(opname.size() - 3, 3) == "Dir" || opname.substr(opname.size() - 3, 3) == "dir") {
		if (opname[0] == 'c' && opname[1] == 'r')
			return "createDir" == target;
		else if (opname[0] == 'c' && opname[1] == 'h')
			return "changeDir" == target;
		else if (opname[0] == 'd')
			return "deleteDir" == target;
	}
	if (opname.substr(opname.size() - 4, 4) == "File" || opname.substr(opname.size() - 4, 4) == "file") {
		if (opname[0] == 'c')
			return "createFile" == target;
		else if (opname[0] == 'd')
			return "deleteFile" == target;
	}
	return 0;
}

void printInterface()
{
	cout << "+-----------------------------------------------------------------------+" << endl;
	cout << "|                      Unix-Style File System                           |" << endl;
	cout << "|                             Group 7                                   |" << endl;
	cout << "|                      Wen Yufei  201930034695                          |" << endl;
	cout << "|                      Yu Beiming 201930640162                          |" << endl;
	cout << "|                      Wang Xu    201930034466                          |" << endl;
	cout << "|-----------------------------------------------------------------------|" << endl;
	cout << "|            1. Create a file: createFile <fileName> <fileSize>         |" << endl;
	cout << "|            2. Delete a file: deleteFile <fileName>                    |" << endl;
	cout << "|            3. Create a directory: createDir <dirName>                 |" << endl;
	cout << "|            4. Delete a directory: deleteDir <dirName>                 |" << endl;
	cout << "|            5. Change directory: changeDir <dirName>                   |" << endl;
	cout << "|            6. Back to parent directory: ..                            |" << endl;
	cout << "|            7. List all the files: dir                                 |" << endl;
	cout << "|            8. Copy a file: cp <exsitedFile> <newFile>                 |" << endl;
	cout << "|            9. Display space usage: sum                                |" << endl;
	cout << "|            10. Print file content: cat <filename>                     |" << endl;
	cout << "|            11. Format the file system: format                         |" << endl;
	cout << "|            12. Show command menu: help                                |" << endl;
	cout << "|            13. Exit the system: exit                                  |" << endl;
	cout << "+-----------------------------------------------------------------------+" << endl;
}

int main()
{
	srand(time(0));
	FileManager fm;
	printInterface();
	vector<string> opt;
	string input;
	string currentDir = "/";
	cout << endl;
	while (true) {
		stack<string> path = fm.getCurrentPath();
		currentDir = path.empty() ? "/" : path.top();
		vector<string> currentpath;
		while (!path.empty()) {
			currentpath.push_back(path.top());
			path.pop();
		}
		cout << "[root@MyFilesystem] " << "/";
		for (int i = 0; i < currentpath.size(); i++) cout << currentpath[i] << "/";
		cout << " $ ";
		getline(cin, input);
		istringstream input_stream(input);
		string item;
		while (input_stream >> item) opt.push_back(item);
		if (opt.empty()) continue;
		else if (opt[0] == "help") {
			printInterface();
		}
		else if (opt[0] == "exit") {
			cout << "GoodBye!" << endl;
			break;
		}
		else if (opt[0] == "dir") {
			fm.dir();
		}
		else if (opt[0] == "..")
		{
			stack<string> currentPath = fm.getCurrentPath();
			if (currentPath.empty()) {
				cout << " parent directory does not exist " << endl;
			}
			else if (currentPath.size() == 1) {
				currentDir = "/";
				fm.changeDir(currentDir.data());
			}
			else {
				string parentpath = "/";
				string curDir;
				while (currentPath.size() > 2) {
					parentpath += currentPath.top() + "/";
					currentPath.pop();
				}
				parentpath += currentPath.top();
				fm.changeDir(parentpath.data());
				currentDir = currentPath.top();
			}
		}
		else if (checkshort(opt[0], "changeDir")) {
			if (checkCommandLength(opt, 2))
			{
				fm.changeDir(opt[1].data());
				stack<string> currentPath = fm.getCurrentPath();
				string curDir;
				if (currentPath.empty())
				{
					curDir = "/";
				}
				else
				{
					while (!currentPath.empty())
					{
						curDir = currentPath.top();
						currentPath.pop();
					}
				}
				currentDir = curDir;
			}
		}
		else if (checkshort(opt[0], "createFile")) {
			if (checkCommandLength(opt, 3))
			{
				if (opt[1] == ".." || opt[1] == "/..")
					cout << " .. cannot be file name!";
				else
					fm.createFile(opt[1].data(), atoi(opt[2].data()));
			}
		}
		else if (checkshort(opt[0], "deleteFile")) {
			if (checkCommandLength(opt, 2)) {
				fm.deleteFile(opt[1].data());
			}
		}
		else if (checkshort(opt[0], "createDir")) {
			if (checkCommandLength(opt, 2)) {
				if (opt[1] == ".." || opt[1] == "/..")
					cout << " .. cannot be directory name!";
				else
					fm.createDir(opt[1].data());
			}
		}
		else if (checkshort(opt[0], "deleteDir")) {
			if (checkCommandLength(opt, 2))
				fm.deleteDir(opt[1].data());
		}
		else if (opt[0] == "cp") {
			if (checkCommandLength(opt, 3))
				fm.copyFile(opt[1].data(), opt[2].data());
		}
		else if (opt[0] == "sum") {
			fm.sum();
		}
		else if (opt[0] == "cat") {
			if (checkCommandLength(opt, 2))
				fm.printFile(opt[1].data());
		}
		else if (opt[0] == "format") {
			cout << "All data will be wiped after formatting, continue? [y/n] ";
			char yn[2];
			cin.getline(yn, 2);
			if (yn[0] == 'y') {
				fm.format();
				cout << "Formatting is done.";
			}
		}
		else {
			cout << "-bash: " << opt[0] << ": Command not found" << endl;
		}
		opt.clear();
		cout << endl;
	}
	return 0;
}
