#include "ucos_ii.h"
#include "flash.h"

static OS_EVENT    *flash_protect_mutex;
static INT8U        mutex_creation_err;
static INT8U        mutex_generic_err;

INT8U
init_ucos2_lib_flash(void)
{
	flash_protect_mutex = OSMutexCreate(0, &mutex_creation_err);
}

enum fsh_op_err
protected_flash_write(int addr, char* data, int len)
{
	enum flash_op_res res;

	OSMutexPend(flash_protect_mutex, 0, &mutex_generic_err);
	res = write_flash(addr, data, len);
	OSMutexPost(flash_protect_mutex);

	return res;
}

enum flash_op_res
read_flash(char* addr, int len, char* dest)
{
	int i;
	for(i = 0; i < len; i++) {

		dest[i] = addr[i]; 
	}
}

