#include "config.h"
#include <_ansi.h>
#include <_syslist.h>
#include <reent.h>
#include <errno.h>
#include <sys/iosupport.h>

#ifdef REENTRANT_SYSCALLS_PROVIDED
//---------------------------------------------------------------------------------
int _DEFUN (_unlink_r, (r, name),
		struct _reent * r _AND
		const char *name ) {
//---------------------------------------------------------------------------------
#else
//---------------------------------------------------------------------------------
int _DEFUN (_unlink, (name),
        const char *name) {
//---------------------------------------------------------------------------------
	struct _reent *r = _REENT;
#endif
	int dev,ret;

	dev = FindDevice(name);
	if(dev!=-1 && devoptab_list[dev]->unlink_r) {
		ret = devoptab_list[dev]->unlink_r(r,name);
	} else {
		ret = -1;
		r->_errno = ENODEV;
	}

	return ret;
}
