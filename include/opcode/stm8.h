{"nop", {NULL}, 1, 0x9D},
{"incw", {ST8_REG_X, NULL}, 3, 0x5C},
{"incw", {ST8_REG_Y, NULL}, 3, 0x905C},
/* logical operators */
{"or", {ST8_BYTE, NULL}, 2, 0xAA},
{"or", {ST8_SHORTMEM, NULL}, 2, 0xBA},
{"or", {ST8_LONGMEM, NULL}, 3, 0xCA},
/* ld / mov */
{"ld", {ST8_REG_A, ST8_BYTE, NULL}, 3, 0xA6},
{"ldw", {ST8_REG_X, ST8_WORD, NULL}, 3, 0xAE},
/* comparison / jumps */
{"cp", {ST8_REG_A, ST8_BYTE, NULL}, 3, 0xA1},
{"cpw", {ST8_REG_X, ST8_WORD, NULL}, 3, 0xA3},
