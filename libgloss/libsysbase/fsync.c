#include "config.h"
#include <_ansi.h>
#include <_syslist.h>
#include <stdio.h>
#include <errno.h>
#include <fcntl.h>

#include "handle_manager.h"

int _DEFUN (fsync,(fileDesc),
			int   fileDesc ) {
	int ret = -1;
	unsigned int dev = 0;
	unsigned int fd = -1;
	
	__handle * handle;
	
	if ( fileDesc < 3) {
		errno = EINVAL;
	} else {
		handle = __get_handle(fileDesc);
	
		if ( NULL == handle ) {
			errno = EINVAL;
			return ret;
		}
				
		dev = handle->device;
		fd = (int)handle->fileStruct;

		if(devoptab_list[dev]->fsync_r)
			ret = devoptab_list[dev]->fsync_r( _REENT, fd);
	}
		
	return ret;
}
