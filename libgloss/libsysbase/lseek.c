#include "config.h"
#include <_ansi.h>
#include <_syslist.h>
#include <errno.h>

#include <sys/iosupport.h>

#include "handle_manager.h"

//---------------------------------------------------------------------------------
#ifdef REENTRANT_SYSCALLS_PROVIDED
//---------------------------------------------------------------------------------
_off_t _DEFUN (_lseek_r, (r, fileDesc, pos, dir),
           struct _reent * r _AND
           int   fileDesc  _AND
		   _off_t   pos   _AND
           int   dir) {
//---------------------------------------------------------------------------------
#else
//---------------------------------------------------------------------------------
_off_t _DEFUN (_lseek,(fileDesc, pos, dir),
           int   fileDesc  _AND
		   _off_t   pos   _AND
           int   dir) {
//---------------------------------------------------------------------------------
	struct _reent *r = _REENT;
//---------------------------------------------------------------------------------
#endif
//---------------------------------------------------------------------------------
	_off_t ret = -1;
	unsigned int dev = 0;
	unsigned int fd = -1;

	__handle * handle;

	if(fileDesc!=-1) {

		if ( fileDesc < 3) {
			dev = fileDesc;
		} else {
			handle = __get_handle(fileDesc);

			if ( NULL == handle ) return ret;

			dev = handle->device;
			fd = (int)handle->fileStruct;
		}

		if(devoptab_list[dev]->seek_r)
			ret = devoptab_list[dev]->seek_r( r, fd, pos, dir);

	}
	return ret;

}
