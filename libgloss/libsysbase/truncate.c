#include "config.h"
#include <_ansi.h>
#include <_syslist.h>
#include <stdio.h>
#include <errno.h>
#include <fcntl.h>

#include "handle_manager.h"

int _DEFUN (truncate, (file, len),
			const char *file _AND
			off_t len)
{
				
	__handle *handle;
	int dev, fd, ret;

	struct _reent * r = _REENT;

	dev = FindDevice(file);
		
	fd = -1;
	if(dev!=-1 && devoptab_list[dev]->open_r && devoptab_list[dev]->close_r && 
	   devoptab_list[dev]->ftruncate_r) 
	{
			
		fd = __alloc_handle(sizeof(__handle) + devoptab_list[dev]->structSize );
			
		if ( -1 != fd ) {
			handle = __get_handle(fd);
			handle->device = dev;
			handle->fileStruct = ((void *)handle) + sizeof(__handle);
				
			ret = devoptab_list[dev]->open_r(r, handle->fileStruct, file, O_WRONLY, 0);
				
			if ( ret < 0 ) {
				__release_handle(fd);
				return ret;
			}
				
			ret = devoptab_list[dev]->ftruncate_r(r, fd, len);
				
			devoptab_list[dev]->close_r(r, fd);
				
			if (ret >= 0) {
				ret = devoptab_list[dev]->close_r(r, fd);
			} else {
				// Close it anyway, we don't want to leak memory
				devoptab_list[dev]->close_r(r, fd);
			}
		} else {
			r->_errno = ENOSR;
		}
	} else {
		r->_errno = ENOSYS;
	}

	return ret;
}
