;; Compilation and flashing under Linux:
;;  stm8-as blinky.s -o blinky
;;  objcopy blinky -O binary -I elf32-little
;;  stm8flash -s8000:w:a.out
;; Estimated frequency (when running on internal 128KHz LSI oscillator):
;;  2 * 128000 / (4 * 2^16) ~= 1 Hz

start:
	;; loop1
	ldw X, 0xFFFF
	.L1:
	decw X
	cpw X, 0x0001
	jrnc .L1

	;; Toggling LED
	ld A, 0x10
	xor A, $501B
	ld $501B, A
	jpf start
