#pragma once
#include<unordered_map>
#include"DiskManager.h"

using namespace std;

class FileManager {
private:
	DiskManager disk;
	int curr_inode;                                                                     // Current directory inode id
	vector<string> processPath(string path);											// Split path into vector
	bool getInodeMap(int dir_inode_id, unordered_map<string, int>& inode_map);			// Get inode map of directory
	bool setInodeMap(int dir_inode_id, const unordered_map<string, int>& inode_map);	// Set inode map of directory
	void addEntry(int dir_inode_id, const char* name, int inode_id);					// Add directory entry
	void removeEntry(int dir_inode_id, const char* name);								// Remove directory entry
	string getInodeData(int inode_id);													// Get data of a block			
	void releaseBlocks(int inode_id);													// Release a inode ans its blocks
	bool setInodeBlocks(int inode_id, const vector<int>& blocks_id);					// Set all data blocks of a inode
	vector<int> getInodeBlocks(int inode_id);											// Get all data blocks of a inode
	void randFillBlock(int block_id, int size);											// Random fill a block

public:
	FileManager();
	~FileManager();
	void format();                                                                      // Format the whole file system
	int getInode(const vector<string>& path);                                           // Get inode of path
	stack<string> getCurrentPath();                                                     // Get current working absolute path
	void createFile(const char* file_name, const int file_size);                        // Create new file
	void deleteFile(const char* file_name);                                             // Delete a file 
	void createDir(const char* dir_name);                                               // Create new directory
	void deleteDir(const char* dir_name);                                               // Delete a directory
	void changeDir(const char* dir_name);                                               // Change the working directory
	void dir();                                                                         // List all the files and sub-directories under current working directory
	void copyFile(const char* file_1, const char* file_2);                              // Copy to a file from another
	void sum();                                                                         // Display the usage of storage space
	void printFile(const char* file_name);                                              // Print out file contents
};




