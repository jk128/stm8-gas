#ifndef TC_ST8
#define TC_ST8
#include "../as.h"

extern const char md_short_jump_size;
extern const char md_long_jump_size;
extern const pseudo_typeS md_pseudo_table[];

typedef enum {
	ST8_END = 0,
	ST8_REG_X,
	ST8_REG_Y,
	ST8_BYTE,
	ST8_WORD,
	ST8_SHORTMEM,
	ST8_LONGMEM,
	ST8_EXTMEM,
	ST8_REG_A
} stm8_arg_t;

#endif
