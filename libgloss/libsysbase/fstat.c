
#include "config.h"
#include <_ansi.h>
#include <_syslist.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <sys/iosupport.h>

// XXX This was not present in dkarm-eabi/patches/newlib-1.20.0.patch
// It is present in dkppc/patches/newlib-1.20.0.patch
#include "handle_manager.h"

#ifdef REENTRANT_SYSCALLS_PROVIDED
//---------------------------------------------------------------------------------
int _DEFUN (_fstat_r,(r,fileDesc, st),
			struct _reent * r _AND
			int fileDesc _AND
			struct stat *st) {
//---------------------------------------------------------------------------------
#else
//---------------------------------------------------------------------------------
int _DEFUN (_fstat,(fileDesc, st),
			int fileDesc _AND
			struct stat *st) {
//---------------------------------------------------------------------------------
	struct _reent *r = _REENT;
#endif
	int ret = -1;
	unsigned int dev = 0;
	unsigned int fd = -1;

	__handle * handle = NULL;

	if(fileDesc!=-1) {
		if ( fileDesc < 3) {
			dev = fileDesc;
		} else {
			handle = __get_handle(fileDesc);

			if ( NULL == handle ) return ret;

			dev = handle->device;
			fd = (int)handle->fileStruct;
		}
		if(devoptab_list[dev]->fstat_r) {
			ret = devoptab_list[dev]->fstat_r(r,fd,st);
		} else {
			r->_errno = ENOSYS;
		}
	}
	return ret;
}
