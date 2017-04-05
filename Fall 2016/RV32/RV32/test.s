	.file	"test.c"
	.section	.rodata
	.align	2
.LC0:
	.string	"hello there"
	.text
	.align	2
	.globl	main
	.type	main, @function
main:
	add	sp,sp,-32
	sw	s0,28(sp)
	add	s0,sp,32
	lui	a5,%hi(.LC0)
	add	a5,a5,%lo(.LC0)
	sw	a5,-20(s0)
	lw	a5,-20(s0)
	sw	a5,-24(s0)
	j	.L2
.L3:
	lw	a5,-24(s0)
	lbu	a5,0(a5)
	sb	a5,-25(s0)
	lbu	a4,-25(s0)
	li	a5,96
	bleu	a4,a5,.L2
	lbu	a4,-25(s0)
	li	a5,122
	bgtu	a4,a5,.L2
	lbu	a5,-25(s0)
	add	a5,a5,-32
	and	a4,a5,0xff
	lw	a5,-24(s0)
	sb	a4,0(a5)
.L2:
	lw	a5,-24(s0)
	lbu	a5,0(a5)
	bnez	a5,.L3
	nop
	lw	s0,28(sp)
	add	sp,sp,32
	jr	ra
	.size	main, .-main
	.ident	"GCC: (GNU) 6.1.0"
