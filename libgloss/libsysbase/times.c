// XXX This file doesn't exist in dkppc/patches/newlib-1.20.0.patch
// I'm not sure why not.
#include <_ansi.h>
#include <_syslist.h>
#include <sys/times.h>
#include <errno.h>

//---------------------------------------------------------------------------------
clock_t _DEFUN (_times, (buf),
        struct tms *buf) {
//---------------------------------------------------------------------------------
  errno = ENOSYS;
  return -1;
}

#include <reent.h>
#include <time.h>
#include <sys/time.h>
#include <sys/times.h>
#include <_syslist.h>

time_t *punixTime;

clock_t
_DEFUN (_times_r, (ptr, ptms),
     struct _reent *ptr _AND
     struct tms *ptms)
{
  return (clock_t)-1;
}


int
_DEFUN (_gettimeofday_r, (ptr, ptimeval, ptimezone),
     struct _reent *ptr _AND
     struct timeval *ptimeval _AND
     void *ptimezone)
{

	time_t ret = -1;
	if ( ptimeval && punixTime ) {
		ret = 0;
		ptimeval->tv_sec = *punixTime;
		ptimeval->tv_usec = 0;
	}

	ptr->_errno = ret;
	return ret;
}
