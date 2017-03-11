#ifndef __FLASH_LIB__
#define __FLASH_LIB__

#include "MK70F15.h"


#define PHRASE_LEN			  8		
#define SECT_SIZE		    	  4096

#define FLASH_OP_ERR_PENDING              FTFE_FSTAT &  (FTFE_FSTAT_ACCERR_MASK | FTFE_FSTAT_FPVIOL_MASK)								 
#define FLASH_OP_ERR_CLEAR()		  FTFE_FSTAT |= (FTFE_FSTAT_ACCERR_MASK | FTFE_FSTAT_RDCOLERR_MASK)
#define RUN_LOADED_FLASH_CMD() 		  FTFE_FSTAT |= FTFE_FSTAT_CCIF_MASK                     

#define OPERATION_PENDING 		  (0 == (FTFE_FSTAT & FTFE_FSTAT_CCIF_MASK))
#define SECT_NOT_ERASED 		  (0 != (FTFE_FSTAT & FTFE_FSTAT_MGSTAT0_MASK))


enum flash_cmd {

    ERASE_SECT		 	= 0x08,
    VERIFY_ERASE_SECT 		= 0x01,			
    PROGRAM_PHRASE 		= 0x07
};

enum flash_op_res {

    ERR_BOUNDARY,       	/* Commands may not span several sectors */
    ERR_SECT_ALIGN,             /* Data must be aligned on longword (two LSBs zero) */
    ERR_PROT,                   /* Flash sector is protected */
    ERR_NOT_PREV_ERASED,
    OP_SUCCESSFUL				
};

enum flash_op_res
write_flash(char* addr, char *data, int len); 


#endif

