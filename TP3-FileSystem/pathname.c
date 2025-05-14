#include "pathname.h"
#include "directory.h"
#include "inode.h"
#include "diskimg.h"
#include <stdio.h>
#include <string.h>
#include <assert.h>

#define DIR_MAX_LEN 14


int pathname_lookup(struct unixfilesystem *fs, const char *pathname) {
	if (strcmp(pathname, "/") == 0) {
		return ROOT_INUMBER;
	} 
    
    const char* path_without_root = pathname + 1;  // Skip leading '/'
	return path_resolve(fs, ROOT_INUMBER, path_without_root);
}

int path_resolve(struct unixfilesystem *fs, int parent_inode, const char* subpath) {
	char* next_slash = strchr(subpath, '/');
	
	if (next_slash == NULL) {
		struct direntv6 dir_entry;
		if (directory_findname(fs, subpath, parent_inode, &dir_entry) < 0) {
			return -1;
		}
		return dir_entry.d_inumber;
	} 
	
	int segment_len = next_slash - subpath;
	char segment[DIR_MAX_LEN];
	strncpy(segment, subpath, segment_len);
	segment[segment_len] = '\0';
	
	struct direntv6 dir_entry;
	if (directory_findname(fs, segment, parent_inode, &dir_entry) < 0) {
		return -1;
	}
	
	const char* remaining_path = next_slash + 1;
	return path_resolve(fs, dir_entry.d_inumber, remaining_path);
}