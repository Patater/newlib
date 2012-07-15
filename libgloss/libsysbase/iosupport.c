#include <stdlib.h>
#include <string.h>
#include <sys/iosupport.h>

static int defaultDevice = -1;

//---------------------------------------------------------------------------------
void setDefaultDevice( int device ) {
//---------------------------------------------------------------------------------

	if ( device >2 && device <= STD_MAX)
		defaultDevice = device;
}

//---------------------------------------------------------------------------------
const devoptab_t dotab_stdnull = {
//---------------------------------------------------------------------------------
	"stdnull",	// device name
	0,			// size of file structure
	NULL,		// device open
	NULL,		// device close
	NULL,		// device write
	NULL,		// device read
	NULL,		// device seek
	NULL,		// device fstat
	NULL,		// device stat
	NULL,		// device link
	NULL,		// device unlink
	NULL,		// device chdir
	NULL,		// device rename
	NULL,		// device mkdir
	0,			// dirStateSize
	NULL,		// device diropen_r
	NULL,		// device dirreset_r
	NULL,		// device dirnext_r
	NULL,		// device dirclose_r
	NULL,		// device statvfs_r
	NULL,		// device ftruncate_r
	NULL,		// device fsync_r
	NULL		// deviceData
};

//---------------------------------------------------------------------------------
const devoptab_t *devoptab_list[STD_MAX] = {
//---------------------------------------------------------------------------------
	&dotab_stdnull, &dotab_stdnull, &dotab_stdnull, &dotab_stdnull,
	&dotab_stdnull, &dotab_stdnull, &dotab_stdnull, &dotab_stdnull,
	&dotab_stdnull, &dotab_stdnull, &dotab_stdnull, &dotab_stdnull,
	&dotab_stdnull, &dotab_stdnull, &dotab_stdnull, &dotab_stdnull
};

//---------------------------------------------------------------------------------
int FindDevice(const char* name) {
//---------------------------------------------------------------------------------
	int i = 0, namelen, dev = -1;

	if (strchr(name, ':') == NULL) return defaultDevice;

	while(i<STD_MAX) {
		if(devoptab_list[i]) {
			namelen = strlen(devoptab_list[i]->name);
			if(strncmp(devoptab_list[i]->name,name,namelen)==0 ) {
				if ( name[namelen] == ':' || (isdigit(name[namelen]) && name[namelen+1] ==':' )) {
					dev = i;
					break;
				}
			}
		}
		i++;
	}

	return dev;
}

//---------------------------------------------------------------------------------
int RemoveDevice( const char* name) {
//---------------------------------------------------------------------------------
	int dev = FindDevice(name);

	if ( -1 != dev ) {
		devoptab_list[dev] = &dotab_stdnull;
		return 0;
	}

	return -1;

}

//---------------------------------------------------------------------------------
int AddDevice( const devoptab_t* device) {
//---------------------------------------------------------------------------------

	int devnum;

	for ( devnum = 3;devnum <STD_MAX; devnum++ ) {

		if ( (!strcmp(devoptab_list[devnum]->name, device->name) &&
					strlen(devoptab_list[devnum]->name) == strlen(device->name) ) ||
			 		!strcmp(devoptab_list[devnum]->name, "stdnull")
			 )
			 break;
	}

	if ( devnum == STD_MAX ) {
		devnum = -1;
	} else {
		devoptab_list[devnum] = device;
	}
	return devnum;
}

//---------------------------------------------------------------------------------
const devoptab_t* GetDeviceOpTab (const char *name) {
//---------------------------------------------------------------------------------
	int dev = FindDevice(name);
	if (dev >= 0 && dev < STD_MAX) {
		return devoptab_list[dev];
	} else {
		return NULL;
	}
}
