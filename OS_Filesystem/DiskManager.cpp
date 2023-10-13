#include "DiskManager.h"

DiskManager::DiskManager()
{
	format();
	reboot();
}

DiskManager::~DiskManager()
{
	FILE* fp;
	errno_t err = fopen_s(&fp, SYSTEM_FILE.c_str(), "wb");
	fwrite(system_addr, sizeof(char), SYSTEM_SIZE, fp);
	fclose(fp);
	free(system_addr);
}

void DiskManager::format()
{
	// allocate space for entire system
	system_addr = (char*)malloc(SYSTEM_SIZE * sizeof(char));

	// allocate address for super block
	super_block = (SuperBlock*)system_addr;
	super_block->setFreeBlocks(DBLOCK_NUM);

	// allocate space for inode bitmap
	inode_bitmap = system_addr + OFFSET_INODE_BITMAP * BLOCK_SIZE;
	char* tmp_inode_bitmap = new char[NUM_BLOCK_INODE_BITMAP];
	memcpy(inode_bitmap, tmp_inode_bitmap, NUM_BLOCK_INODE_BITMAP * sizeof(char));
	memset(inode_bitmap, '0', NUM_BLOCK_INODE_BITMAP * sizeof(char));
	delete[] tmp_inode_bitmap;

	// allocate space for data block bitmap
	data_bitmap = system_addr + OFFSET_DBLOCK_BITMAP * BLOCK_SIZE;
	char* tmp_data_bitmap = new char[NUM_BLOCK_DATA_BITMAP];
	memcpy(data_bitmap, tmp_data_bitmap, NUM_BLOCK_DATA_BITMAP * sizeof(char));
	memset(data_bitmap, '0', NUM_BLOCK_DATA_BITMAP * sizeof(char));
	delete[] tmp_data_bitmap;


	// initialze inodes
	inodes = (INODE*)(system_addr + OFFSET_INODE * BLOCK_SIZE);
	INODE* tmp_inodes = new INODE[INODE_NUM];
	memcpy(inodes, tmp_inodes, INODE_NUM * sizeof(INODE));
	delete[] tmp_inodes;

	// set data block start address
	data_addr = system_addr + OFFSET_DBLOCK * BLOCK_SIZE;

	// initialize root inode
	int root_inode_id = getFreeInodeID();
	setInodeBitmap(root_inode_id, 1);
	INODE& root = inodes[root_inode_id];
	root = INODE("DIR", -1, getCurrTime().c_str());
}

char* DiskManager::getSystemAddr()
{
	return system_addr;
}

SuperBlock* DiskManager::getSuperBlock()
{
	return super_block;
}

char* DiskManager::getDBlockAddr(int block_id)
{
	return data_addr + (block_id * BLOCK_SIZE);
}

void DiskManager::setDBlockBitmap(const int block_id, const int value) {
	if (value==1)
		data_bitmap[block_id] = '1';
	else
		data_bitmap[block_id] = '0';
}

void DiskManager::setInodeBitmap(const int inode_id, const int value) {
	if (value==1)
		inode_bitmap[inode_id] = '1';
	else
		inode_bitmap[inode_id] = '0';
}

int DiskManager::getDBlockBitmap(const int id) {
	if (data_bitmap[id] == '1')
		return 1;
	else
		return 0;
}

int DiskManager::getInodeBitmap(const int inode_id) {
	if (inode_bitmap[inode_id] == '1')
		return 1;
	else
		return 0;
}


int DiskManager::getFreeInodeID()
{
	int free_node_id = -1;
	for (int i = 0; i < INODE_NUM; ++i) {
		if (getInodeBitmap(i) == 0) {
			free_node_id = i;
			break;
		}
	}
	return free_node_id;
}

int DiskManager::getFreeDBlockID()
{
	int free_data_block_id = -1;
	for (int i = 0; i < DBLOCK_NUM; ++i) {
		if (getDBlockBitmap(i) == 0) {
			free_data_block_id = i;
			break;
		}
	}
	return free_data_block_id;
}

void DiskManager::reboot()
{
	FILE* fp;
	errno_t err = fopen_s(&fp, SYSTEM_FILE.c_str(), "rb");
	if (err == 0) {
		fread(system_addr, sizeof(char), SYSTEM_SIZE, fp);
		fclose(fp);
	}
}