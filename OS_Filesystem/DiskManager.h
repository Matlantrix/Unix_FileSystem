#pragma once
#include "util.h"
using namespace std;

class DiskManager {
private:
	char* system_addr;									  // start address of system
	SuperBlock* super_block;						 	  // start address of super block
	char* inode_bitmap;									  // start address of inode bitmap
	char* data_bitmap;								 	  // start address of data block bitmap
	char* data_addr;	    							  // start address of data blocks
public:
	DiskManager();
	~DiskManager();
	INODE* inodes;                                        // store the inodes
	void format();                                        // format the disk
	SuperBlock* getSuperBlock();                          // get the SuperBlock
	char* getSystemAddr();                                // get the start address of system
	char* getDBlockAddr(int block_id);	                  // get address of a data block by id
	void setInodeBitmap(const int id, const int value);   // set the bitmap value of an inode by id
	int getInodeBitmap(const int id);	                  // get the bitmap value of an inode by id
	void setDBlockBitmap(const int id, const int value);  // set the bitmap value of a data block by id
	int getDBlockBitmap(const int id);	                  // get the bitmap value of a data block by id
	int getFreeInodeID();	                              // return id of a free inode
	int getFreeDBlockID();                                // return id of a free data block
	void reboot();                                        // relaod data from local storage file
};

