#ifndef FLASH_H_
#define FLASH_H_



#include "flash_lld.h"
#include "ucos_ii.h"


INT8U init_ucos2_lib_flash(void);

enum fsh_op_err write_flash_protected(int addr, char *data_buff, int buff_size);

enum fsh_op_err read_flash(int addr, int size, char *container);



#endif 
