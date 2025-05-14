#include "directory.h"
#include "inode.h"
#include "diskimg.h"
#include "file.h"
#include <stdio.h>
#include <string.h>
#include <assert.h>


int directory_findname(struct unixfilesystem *fs, const char *name, int dir_number, struct direntv6 *dirEnt) {
	struct inode current_inode;
	if (inode_iget(fs, dir_number, &current_inode) < 0) {
		return -1;
	}

	if (((current_inode.i_mode & IFMT) != IFDIR)) {
		return -1;
	}

	int directory_size = inode_getsize(&current_inode);
	if (directory_size <= 0) {
		return -1;
	}

	int blocks_to_read = (directory_size + DISKIMG_SECTOR_SIZE - 1) / DISKIMG_SECTOR_SIZE;
	
	for (int block_idx = 0; block_idx < blocks_to_read; block_idx++) {		
		struct direntv6 dir_entries[DISKIMG_SECTOR_SIZE / sizeof(struct direntv6)];
		
		int bytes_read = file_getblock(fs, dir_number, block_idx, dir_entries);
		if (bytes_read < 0) {
			return -1;
		}
		
		int entries_count = bytes_read / sizeof(struct direntv6);
		
		for (int entry_idx = 0; entry_idx < entries_count; entry_idx++) {	
			if (strcmp(dir_entries[entry_idx].d_name, name) == 0) {
				*dirEnt = dir_entries[entry_idx];
				return 0;
			}
		}
	}

	return -1;
}