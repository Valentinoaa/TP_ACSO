#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "file.h"
#include "inode.h"
#include "diskimg.h"


int file_getblock(struct unixfilesystem *fs, int inumber, int blockNum, void *buf) {
	struct inode node_data;
	if (inode_iget(fs, inumber, &node_data) < 0) {
		return -1;
	}

	if ((node_data.i_mode & IALLOC) == 0) {
		return -1;
	}

	int block_sector = inode_indexlookup(fs, &node_data, blockNum);
	if (block_sector < 0) {
		return -1;
	}

	if (block_sector == 0) {
		memset(buf, 0, DISKIMG_SECTOR_SIZE);
	} else {
		if (diskimg_readsector(fs->dfd, block_sector, buf) < 0) {
			return -1;
		}
	}

	int file_size = inode_getsize(&node_data);
	if (file_size < 0) {
		return -1;
	}
	
	int full_blocks = file_size / DISKIMG_SECTOR_SIZE;
	
	if (blockNum == full_blocks) {
		return file_size % DISKIMG_SECTOR_SIZE;
	} else {
		return DISKIMG_SECTOR_SIZE;
	}
}