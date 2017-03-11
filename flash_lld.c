#include "flash.h"
#include "MK70F15.h"


static enum flash_op_res 
sect_erased(char* addr);

static enum flash_op_res
erase_sect(char* addr);

static enum flash_op_res
copy_sect(char* sect_start, char *tmp);

static char*
get_sect_start(char* addr);


/*
* DESC	erase sect 
*
* IN	addr: arbitrary address inside a sector 
*/
static enum flash_op_res
erase_sect(char* addr) 
{
	if (FLASH_OP_ERR_PENDING) {

		FLASH_OP_ERR_CLEAR();
	}    

	FTFE_FCCOB0 = (enum flash_cmd )ERASE_SECT;               
	FTFE_FCCOB1 = (addr >> 16);                    
	FTFE_FCCOB2 = (addr >> 8);	                
	FTFE_FCCOB3 = (addr);              

	while (OPERATION_PENDING) /* wait */; 	

	RUN_LOADED_FLASH_CMD();
}

/*
* DESC	 copy flash sector into ram 
*
* IN	 start: sector start  
* 	 tmp: 	pointer to temporary buffer which will store sector copy
*/
static enum flash_op_res 
copy_sect(char* start, char* tmp)
{
	int i;
	for(i = 0; i < SECT_SIZE; i++) {

		tmp[i] = start[i];
	}

	return OP_SUCCESSFUL; 
}

/*
* DESC	 calculate the start address of a sector based on an arbitrary address inside it 
*
* IN	 addr: arbitrary address inside the sector  
*/
static int 
get_sect_start(char* addr)
{
	return (addr - (addr % SECT_SIZE));  
}

/*
* DESC	 check flash sector erase + write data into it     
*
* IN	 addr:    arbitrary address inside the sector  
*        data:    data to write 
*        len:     data length  
*/
enum flash_op_res
write_flash(char* addr, char* data, int len)
{	
	/* 
	 * we cannot directly write/modify a flash sector, so we need a temporary buffer 
	 * to store and modify sector content for later writing into flash 
	 */
	char tmp[SECT_SIZE];  	

	int off, start, i;

	start = get_sect_start(addr);

	if (!sect_erased(start)) { 

		erase_sect(start);
	}

	off = addr - start;

	copy_sect(start, tmp);

	for (i = 0; i < len; i++) {

		tmp[off + i] = data[i];
	}

	for (i = 0; i < SECT_SIZE; i += PHRASE_LEN) {

		if (FLASH_OP_ERR_PENDING) {

			FLASH_OP_ERR_CLEAR();
		}    

		FTFE_FCCOB0 = (enum flash_cmd )PROGRAM_PHRASE;                        

		FTFE_FCCOB1 = (start + i) >> 16;                        
		FTFE_FCCOB2 = (start + i) >> 8;                           
		FTFE_FCCOB3 = (start + i);  

		FTFE_FCCOB7 = tmp[i];
		FTFE_FCCOB6 = tmp[i + 1];
		FTFE_FCCOB5 = tmp[i + 2];
		FTFE_FCCOB4 = tmp[i + 3];
		FTFE_FCCOBB = tmp[i + 4];					
		FTFE_FCCOBA = tmp[i + 5];
		FTFE_FCCOB9 = tmp[i + 6];
		FTFE_FCCOB8 = tmp[i + (PHRASE_LEN - 1)];

		while (OPERATION_PENDING) /* wait */;

		RUN_LOADED_FLASH_CMD();
	}

	return OP_SUCCESSFUL;
} 

/*
* DESC	 check if sector is erased 
*
* IN	 addr: arbitrary address inside the sector  
*/
static enum flash_op_res
sect_erased(char* addr)
{
	enum flash_op_res res;
	
	if (FLASH_OP_ERR_PENDING) {

		FLASH_OP_ERR_CLEAR();
	}    

	FTFE_FCCOB0 = (enum flash_cmd )VERIFY_SECT_ERASE;
	FTFE_FCCOB1 = (addr >> 16);
	FTFE_FCCOB2 = (addr >> 8);
	FTFE_FCCOB3 = (addr);
	FTFE_FCCOB4 = 1; 					/* not sure about what it does...but it works! */

	while (OPERATION_PENDING) /* wait */; 	

	RUN_LOADED_FLASH_CMD();

	if (SECT_NOT_ERASED) {	

		ret = ERR_NOT_PREV_ERASED;
	} else { 

		ret = OP_SUCCESSFUL;
	}

	return ret;
}
