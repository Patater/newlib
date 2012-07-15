#include <stdlib.h>
#include <stdio.h>

#include "handle_manager.h"

#define MAX_HANDLES 1024

static __handle* handles[MAX_HANDLES];

__LOCK_INIT_RECURSIVE(static, __hndl_lock);

void __release_handle(int fd) {

	if ( fd < 3 || fd >= MAX_HANDLES + 3 ) return;

	fd -= 3;
	__handle* handle = handles[fd];
	if ( NULL != handle ) {
		free(handle);
		handles[fd] = NULL;
	}

}

int  __alloc_handle(int size) {

	int i, ret = -1;

	__lock_acquire_recursive (__hndl_lock);
	for ( i = 0; i < MAX_HANDLES; i++ ) {
		if ( handles[i] == NULL ) break;
	}
	
	if ( i < MAX_HANDLES ) {
		handles[i] = malloc(size);
		if ( NULL != handles[i] ) ret = i+3;
	}
	__lock_release_recursive (__hndl_lock);
	
	return ret; 
}

__handle *__get_handle(int fd) {

	if ( fd < 3 || fd > MAX_HANDLES + 3 ) return NULL;
	
	return handles[fd-3];

}
