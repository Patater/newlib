#include "config.h"
#include <_ansi.h>
#include <_syslist.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <errno.h>

#include "handle_manager.h"

#ifdef REENTRANT_SYSCALLS_PROVIDED
//---------------------------------------------------------------------------------
int _DEFUN (_open_r, (r, file, flags, mode),
			struct _reent * r _AND
			const char *file  _AND
			int   flags _AND
			int   mode) {
//---------------------------------------------------------------------------------
#else
//---------------------------------------------------------------------------------
int _DEFUN (_open, (file, flags, mode),
        const char *file  _AND
        int   flags _AND
        int   mode) {
//---------------------------------------------------------------------------------
	struct _reent *r = _REENT;
#endif
	__handle *handle;
	int dev, fd, ret;

	dev = FindDevice(file);

	fd = -1;
	if(dev!=-1 && devoptab_list[dev]->open_r) {

		fd = __alloc_handle(sizeof(__handle) + devoptab_list[dev]->structSize );

		if ( -1 != fd ) {
			handle = __get_handle(fd);
			handle->device = dev;
			handle->fileStruct = ((void *)handle) + sizeof(__handle);

			ret = devoptab_list[dev]->open_r(r, handle->fileStruct, file, flags, mode);

			if ( ret == -1 ) {
				__release_handle(fd);
				fd = -1;
			}
		} else {
			r->_errno = ENOSR;
		}
	} else {
		r->_errno = ENOSYS;
	}

	return fd;
}
