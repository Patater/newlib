// XXX There is a more functional sbrk.c in the
// dkarm-eabi/patches/newlib-1.20.0.patch file. This version comes from
// dkppc/patches/newlib-1.20.0.patch.
// We should talk to Dave Murphy (WinterMute) about this later. Perhaps
// devkitARM shouldn't do such linker-specific stuff and devkitPPC is
// doing the right thing.

#include "config.h"
#include <sys/iosupport.h>
#include <sys/errno.h>

#ifdef REENTRANT_SYSCALLS_PROVIDED
//---------------------------------------------------------------------------------
void * _sbrk_r (struct _reent *ptr, ptrdiff_t incr) {
//---------------------------------------------------------------------------------
#else
//---------------------------------------------------------------------------------
caddr_t _sbrk (int incr) {
//---------------------------------------------------------------------------------
	struct _reent *ptr = _REENT;
#endif


	if ( __syscalls.sbrk_r ) {
		return __syscalls.sbrk_r(ptr, incr);
	} else {

		ptr->_errno = ENOMEM;
		return (caddr_t) -1;

	}

}
