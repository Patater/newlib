#include "config.h"
#include <_ansi.h>
#include <_syslist.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/iosupport.h>
#include <errno.h>

#include "handle_manager.h"

int fchmod(int fd, mode_t mode) {
	int	ret = -1, dev;
	struct _reent *r = _REENT;

	if(fd!=-1) {

		__handle *handle = __get_handle(fd);

		if ( handle != NULL) {

			dev = handle->device;
			fd = (unsigned int)handle->fileStruct;

			if(devoptab_list[dev]->fchmod_r)
				ret = devoptab_list[dev]->fchmod_r(r,fd,mode);

		}
	}
	return ret;
}
