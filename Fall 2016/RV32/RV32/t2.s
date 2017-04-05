main:
	li	x1, 0xBBAA
	li	x2, 0xDDCC
	sll x2, x2, 16
    or  x3, x2, x1
    andi x4, x2, 0xff
    li	a7, 1
	mv	a0, x4
	scall
	srl x3, x3, 8
    andi x4, x2, 0xff
    li	a7, 1
	mv	a0, x4
	scall
	srl x3, x3, 8
    andi x4, x2, 0xff
    li	a7, 1
	mv	a0, x4
	scall
	srl x3, x3, 8
    andi x4, x2, 0xff
    li	a7, 1
	mv	a0, x4
	scall
    
	li	a7, 10			 # calls exit command (code 10)
    SCALL				     # end of program


