#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

#include "inode.h"
#include "diskimg.h"

#define INDIR_ADDR 7
#define INODES_PER_SECTOR 16
#define ADDRS_PER_BLOCK 256  // 512 bytes / 2 bytes por dirección


int inode_iget(struct unixfilesystem *fs, int inumber, struct inode *inp) {
	if (inumber < 1) {
		return -1;
	}

	int adjusted_inumber = inumber - 1;	
	int sector_num = adjusted_inumber / INODES_PER_SECTOR;
	int inode_index = adjusted_inumber % INODES_PER_SECTOR;

	struct inode sector_inodes[INODES_PER_SECTOR];
	if (diskimg_readsector(fs->dfd, INODE_START_SECTOR + sector_num, sector_inodes) < 0) {
		return -1;
	}
	
	*inp = sector_inodes[inode_index];
	return 0;	
}


int inode_indexlookup(struct unixfilesystem *fs, struct inode *inp, int blockNum) {
	if ((inp->i_mode & IALLOC) == 0) {
		return -1;
	}

	int is_regular_file = ((inp->i_mode & ILARG) == 0);

	if (is_regular_file) {
		if (blockNum < 0 || blockNum >= 8) {
			return -1;
		}
		return inp->i_addr[blockNum];
	}	
    
	int total_indirect_blocks = ADDRS_PER_BLOCK * INDIR_ADDR;
	
	if (blockNum < total_indirect_blocks) {
		int indirect_level = blockNum / ADDRS_PER_BLOCK;
		int block_offset = blockNum % ADDRS_PER_BLOCK;
		
		uint16_t addr_table[ADDRS_PER_BLOCK];
		if (diskimg_readsector(fs->dfd, inp->i_addr[indirect_level], addr_table) < 0) {
			return -1;
		}
		
		return addr_table[block_offset];
	} else {
		int remaining_blocks = blockNum - total_indirect_blocks;
		int double_indirect_idx = INDIR_ADDR;
		int first_level_offset = remaining_blocks / ADDRS_PER_BLOCK;
		
		uint16_t first_level_table[ADDRS_PER_BLOCK];
		if (diskimg_readsector(fs->dfd, inp->i_addr[double_indirect_idx], first_level_table) < 0) {
			return -1;
		}

		int second_level_block = first_level_table[first_level_offset];
		int second_level_offset = remaining_blocks % ADDRS_PER_BLOCK;
		
		uint16_t second_level_table[ADDRS_PER_BLOCK];
		if (diskimg_readsector(fs->dfd, second_level_block, second_level_table) < 0) {
			return -1;
		}
		
		return second_level_table[second_level_offset];
	}	
}


int inode_getsize(struct inode *inp) {
  return ((inp->i_size0 << 16) | inp->i_size1); 
}