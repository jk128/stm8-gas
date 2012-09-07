{"nop", {ST8_END}, 1, 0x9D},
{"incw", {ST8_REG_X, ST8_END}, 1, 0x5C},
{"incw", {ST8_REG_Y, ST8_END}, 2, 0x905C},
/* logical operators */
{"or", {ST8_REG_A, ST8_BYTE, ST8_END}, 2, 0xAA},
{"or", {ST8_REG_A, ST8_SPREL, ST8_END}, 2, 0xBA},
{"or", {ST8_REG_A, ST8_LONGMEM, ST8_END}, 2, 0xCA},
/* ld / mov */
{"ld", {ST8_REG_A, ST8_BYTE, ST8_END}, 2, 0xA6},
{"ldw", {ST8_REG_X, ST8_WORD, ST8_END}, 2, 0xAE},
/* comparison / jumps */
{"cp", {ST8_REG_A, ST8_BYTE, ST8_END}, 2, 0xA1},
{"cpw", {ST8_REG_X, ST8_WORD, ST8_END}, 2, 0xA3},
{"jrnc", {ST8_SPREL, ST8_END}, 1, 0x24},
{"jpf", {ST8_EXTMEM, ST8_END}, 1, 0xAC},
