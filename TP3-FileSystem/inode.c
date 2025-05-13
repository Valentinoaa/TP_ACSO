#include <stdio.h>
#include <assert.h>

#include "inode.h"
#include "diskimg.h"

#define INDIR_ADDR 7


int inode_iget(struct unixfilesystem *fs, int inumber, struct inode *inp) {
	inumber = inumber - 1;	
	int inode_num = DISKIMG_SECTOR_SIZE / sizeof(struct inode);
	int sector_offset = inumber / inode_num;
	int inumber_offset = inumber % inode_num;

	int fd = fs->dfd;
	struct inode inodes[inode_num];
	int err = diskimg_readsector(fd, INODE_START_SECTOR + sector_offset, inodes);
	if(err < 0) return -1;
	
	*inp = inodes[inumber_offset];

	return 0;	
}


int inode_indexlookup(struct unixfilesystem *fs, struct inode *inp, int blockNum) {
	int fd = fs->dfd;
	int is_small_file = ((inp->i_mode & ILARG) == 0);

	if(is_small_file) {
		return inp->i_addr[blockNum];
	}	
    
	int addr_num = DISKIMG_SECTOR_SIZE / sizeof(uint16_t);
	int indir_addr_num = addr_num * INDIR_ADDR;
	if(blockNum < indir_addr_num) {		// if it only uses INDIR_ADDR
		int sector_offset = blockNum / addr_num;
		int addr_offset = blockNum % addr_num;
		uint16_t addrs[addr_num];
		int err = diskimg_readsector(fd, inp->i_addr[sector_offset], addrs);
		if(err < 0) return -1;	
		return addrs[addr_offset];
	} else {							// if it also uses the DOUBLE_INDIR_ADDR
		// the first layer
		int blockNum_in_double = blockNum - indir_addr_num;
		int sector_offset_1 = INDIR_ADDR;
		int addr_offset_1 = blockNum_in_double / addr_num;
		uint16_t addrs_1[addr_num];
		int err_1 = diskimg_readsector(fd, inp->i_addr[sector_offset_1], addrs_1);
		if(err_1 < 0) return -1;

		// the second layer
		int sector_2 = addrs_1[addr_offset_1];
		int addr_offset_2 = blockNum_in_double % addr_num;
		uint16_t addrs_2[addr_num];
		int err_2 = diskimg_readsector(fd, sector_2, addrs_2);
		if(err_2 < 0) return -1;
		return addrs_2[addr_offset_2];
	}	
}


/**
 * Computes the size in bytes of the file identified by the given inode
 */
int inode_getsize(struct inode *inp) {
  return ((inp->i_size0 << 16) | inp->i_size1); 
}