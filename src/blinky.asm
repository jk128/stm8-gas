.START
ld A, 0x00

.LOOP2
ldw X, 0x0000

.LOOP1
incw X ;; 2 cycle
cpw X, 0xFFFF ;; X < 0xFF => C=1 2 cycle
jrnc 4 ;; 1/2 cycle
jpf LOOP1 ;; 2cycle

inc A
cp A, 0x40
jrnc 4
jpf LOOP2

jpf START
