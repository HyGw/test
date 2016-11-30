	.file	1 "qdma_api.c"
	.section .mdebug.abi32
	.previous
	.nan	legacy
	.gnu_attribute 4, 3
#APP
	.macro _ssnop; sll $0, $0, 1; .endm
	.macro _ehb; sll $0, $0, 3; .endm
	.macro mtc0_tlbw_hazard; _ehb; .endm
	.macro tlbw_use_hazard; _ehb; .endm
	.macro tlb_probe_hazard; _ehb; .endm
	.macro irq_enable_hazard; _ehb; .endm
	.macro irq_disable_hazard; _ehb; .endm
	.macro back_to_back_c0_hazard; _ehb; .endm
	.macro enable_fpu_hazard; _ehb; .endm
	.macro disable_fpu_hazard; _ehb; .endm
	.macro	raw_local_irq_enable				
	.set	push						
	.set	reorder						
	.set	noat						
	mfc0	$1, $2, 1	# SMTC - clear TCStatus.IXMT	
	ori	$1, 0x400					
	xori	$1, 0x400					
	mtc0	$1, $2, 1					
	irq_enable_hazard					
	.set	pop						
	.endm
	.macro	raw_local_irq_disable
	.set	push						
	.set	noat						
	mfc0	$1, $2, 1					
	ori	$1, 0x400					
	.set	noreorder					
	mtc0	$1, $2, 1					
	irq_disable_hazard					
	.set	pop						
	.endm							

	.macro	raw_local_save_flags flags			
	.set	push						
	.set	reorder						
	mfc0	\flags, $2, 1					
	.set	pop						
	.endm							

	.macro	raw_local_irq_save result			
	.set	push						
	.set	reorder						
	.set	noat						
	mfc0	\result, $2, 1					
	ori	$1, \result, 0x400				
	.set	noreorder					
	mtc0	$1, $2, 1					
	andi	\result, \result, 0x400			
	irq_disable_hazard					
	.set	pop						
	.endm							

	.macro	raw_local_irq_restore flags			
	.set	push						
	.set	noreorder					
	.set	noat						
mfc0	$1, $2, 1						
andi	\flags, 0x400						
ori	$1, 0x400						
xori	$1, 0x400						
or	\flags, $1						
mtc0	\flags, $2, 1						
	irq_disable_hazard					
	.set	pop						
	.endm							

#NO_APP
	.globl	isForceRX1
	.section	.bss,"aw",@nobits
	.align	2
	.type	isForceRX1, @object
	.size	isForceRX1, 4
isForceRX1:
	.space	4
	.globl	chnlLimit
	.align	2
	.type	chnlLimit, @object
	.size	chnlLimit, 32
chnlLimit:
	.space	32
	.globl	queueLimit
	.align	2
	.type	queueLimit, @object
	.size	queueLimit, 256
queueLimit:
	.space	256
	.data
	.align	2
	.type	qosTypeStr, @object
	.size	qosTypeStr, 128
qosTypeStr:
	.ascii	"WRR\000"
	.space	12
	.ascii	"SP\000"
	.space	13
	.ascii	"SPWRR7\000"
	.space	9
	.ascii	"SPWRR6\000"
	.space	9
	.ascii	"SPWRR5\000"
	.space	9
	.ascii	"SPWRR4\000"
	.space	9
	.ascii	"SPWRR3\000"
	.space	9
	.ascii	"SPWRR2\000"
	.space	9
	.align	2
	.type	vChnlQosTypeStr, @object
	.size	vChnlQosTypeStr, 64
vChnlQosTypeStr:
	.ascii	"WRR\000"
	.space	12
	.ascii	"SP\000"
	.space	13
	.ascii	"SPWRR3\000"
	.space	9
	.ascii	"SPWRR2\000"
	.space	9
	.section	.text.qdma_init,"ax",@progbits
	.align	2
	.globl	qdma_init
	.set	nomips16
	.set	nomicromips
	.ent	qdma_init
	.type	qdma_init, @function
qdma_init:
	.frame	$sp,32,$31		# vars= 8, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-32
	sw	$31,28($sp)
	sw	$4,32($sp)
	lw	$2,32($sp)
	lw	$2,8($2)
	sw	$2,16($sp)
	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lw	$2,0($2)
	sw	$2,20($sp)
	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lw	$3,16($sp)
	lw	$3,0($3)
	andi	$3,$3,0x00ff
	sb	$3,142($2)
	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lw	$3,140($2)
	li	$2,-2147483648			# 0xffffffff80000000
	and	$2,$3,$2
	beq	$2,$0,$L2
	nop

	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lw	$3,16($sp)
	lbu	$3,4($3)
	sb	$3,141($2)
$L2:
	lw	$2,32($sp)
	sw	$0,8($2)
	lw	$2,16($sp)
	lw	$3,8($2)
	lw	$2,32($sp)
	sw	$3,12($2)
	lw	$4,32($sp)
	lui	$2,%hi(qdma_register_callback_function)
	addiu	$2,$2,%lo(qdma_register_callback_function)
	jalr	$2
	nop

	lw	$2,32($sp)
	li	$3,1			# 0x1
	sw	$3,8($2)
	lw	$2,16($sp)
	lw	$3,12($2)
	lw	$2,32($sp)
	sw	$3,12($2)
	lw	$4,32($sp)
	lui	$2,%hi(qdma_register_callback_function)
	addiu	$2,$2,%lo(qdma_register_callback_function)
	jalr	$2
	nop

	lw	$2,32($sp)
	li	$3,2			# 0x2
	sw	$3,8($2)
	lw	$2,16($sp)
	lw	$3,16($2)
	lw	$2,32($sp)
	sw	$3,12($2)
	lw	$4,32($sp)
	lui	$2,%hi(qdma_register_callback_function)
	addiu	$2,$2,%lo(qdma_register_callback_function)
	jalr	$2
	nop

	lw	$2,32($sp)
	li	$3,3			# 0x3
	sw	$3,8($2)
	lw	$2,16($sp)
	lw	$3,20($2)
	lw	$2,32($sp)
	sw	$3,12($2)
	lw	$4,32($sp)
	lui	$2,%hi(qdma_register_callback_function)
	addiu	$2,$2,%lo(qdma_register_callback_function)
	jalr	$2
	nop

	lw	$2,32($sp)
	li	$3,4			# 0x4
	sw	$3,8($2)
	lw	$2,16($sp)
	lw	$3,24($2)
	lw	$2,32($sp)
	sw	$3,12($2)
	lw	$4,32($sp)
	lui	$2,%hi(qdma_register_callback_function)
	addiu	$2,$2,%lo(qdma_register_callback_function)
	jalr	$2
	nop

	lw	$2,32($sp)
	li	$3,5			# 0x5
	sw	$3,8($2)
	lw	$2,16($sp)
	lw	$3,28($2)
	lw	$2,32($sp)
	sw	$3,12($2)
	lw	$4,32($sp)
	lui	$2,%hi(qdma_register_callback_function)
	addiu	$2,$2,%lo(qdma_register_callback_function)
	jalr	$2
	nop

	move	$2,$0
	lw	$31,28($sp)
	addiu	$sp,$sp,32
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	qdma_init
	.size	qdma_init, .-qdma_init
	.section	.text.qdma_deinit,"ax",@progbits
	.align	2
	.globl	qdma_deinit
	.set	nomips16
	.set	nomicromips
	.ent	qdma_deinit
	.type	qdma_deinit, @function
qdma_deinit:
	.frame	$sp,24,$31		# vars= 0, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-24
	sw	$31,20($sp)
	sw	$4,24($sp)
	lw	$2,24($sp)
	sw	$0,8($2)
	lw	$4,24($sp)
	lui	$2,%hi(qdma_unregister_callback_function)
	addiu	$2,$2,%lo(qdma_unregister_callback_function)
	jalr	$2
	nop

	lw	$2,24($sp)
	li	$3,1			# 0x1
	sw	$3,8($2)
	lw	$4,24($sp)
	lui	$2,%hi(qdma_unregister_callback_function)
	addiu	$2,$2,%lo(qdma_unregister_callback_function)
	jalr	$2
	nop

	lw	$2,24($sp)
	li	$3,2			# 0x2
	sw	$3,8($2)
	lw	$4,24($sp)
	lui	$2,%hi(qdma_unregister_callback_function)
	addiu	$2,$2,%lo(qdma_unregister_callback_function)
	jalr	$2
	nop

	lw	$2,24($sp)
	li	$3,3			# 0x3
	sw	$3,8($2)
	lw	$4,24($sp)
	lui	$2,%hi(qdma_unregister_callback_function)
	addiu	$2,$2,%lo(qdma_unregister_callback_function)
	jalr	$2
	nop

	lw	$2,24($sp)
	li	$3,4			# 0x4
	sw	$3,8($2)
	lw	$4,24($sp)
	lui	$2,%hi(qdma_unregister_callback_function)
	addiu	$2,$2,%lo(qdma_unregister_callback_function)
	jalr	$2
	nop

	lw	$2,24($sp)
	li	$3,5			# 0x5
	sw	$3,8($2)
	lw	$4,24($sp)
	lui	$2,%hi(qdma_unregister_callback_function)
	addiu	$2,$2,%lo(qdma_unregister_callback_function)
	jalr	$2
	nop

	move	$2,$0
	lw	$31,20($sp)
	addiu	$sp,$sp,24
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	qdma_deinit
	.size	qdma_deinit, .-qdma_deinit
	.section	.text.qdma_tx_dma_mode,"ax",@progbits
	.align	2
	.globl	qdma_tx_dma_mode
	.set	nomips16
	.set	nomicromips
	.ent	qdma_tx_dma_mode
	.type	qdma_tx_dma_mode, @function
qdma_tx_dma_mode:
	.frame	$sp,40,$31		# vars= 40, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-40
	sw	$4,40($sp)
	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lw	$2,0($2)
	sw	$2,0($sp)
	lw	$2,40($sp)
	lw	$2,8($2)
	sw	$2,4($sp)
	lw	$3,4($sp)
	li	$2,1			# 0x1
	bne	$3,$2,$L7
	nop

	lw	$2,0($sp)
	addiu	$2,$2,4
	sw	$2,16($sp)
	lw	$2,16($sp)
	lw	$2,0($2)
	sw	$2,8($sp)
	lw	$2,0($sp)
	addiu	$3,$2,4
	lw	$2,8($sp)
	ori	$2,$2,0x1
	sw	$3,20($sp)
	sw	$2,24($sp)
	lw	$2,20($sp)
	lw	$3,24($sp)
	sw	$3,0($2)
	j	$L9
	nop

$L7:
	lw	$2,0($sp)
	addiu	$2,$2,4
	sw	$2,28($sp)
	lw	$2,28($sp)
	lw	$2,0($2)
	sw	$2,12($sp)
	lw	$2,0($sp)
	addiu	$3,$2,4
	lw	$4,12($sp)
	li	$2,-2			# 0xfffffffffffffffe
	and	$2,$4,$2
	sw	$3,32($sp)
	sw	$2,36($sp)
	lw	$2,32($sp)
	lw	$3,36($sp)
	sw	$3,0($2)
$L9:
	move	$2,$0
	addiu	$sp,$sp,40
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	qdma_tx_dma_mode
	.size	qdma_tx_dma_mode, .-qdma_tx_dma_mode
	.section	.text.qdma_rx_dma_mode,"ax",@progbits
	.align	2
	.globl	qdma_rx_dma_mode
	.set	nomips16
	.set	nomicromips
	.ent	qdma_rx_dma_mode
	.type	qdma_rx_dma_mode, @function
qdma_rx_dma_mode:
	.frame	$sp,40,$31		# vars= 40, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-40
	sw	$4,40($sp)
	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lw	$2,0($2)
	sw	$2,0($sp)
	lw	$2,40($sp)
	lw	$2,8($2)
	sw	$2,4($sp)
	lw	$3,4($sp)
	li	$2,1			# 0x1
	bne	$3,$2,$L13
	nop

	lw	$2,0($sp)
	addiu	$2,$2,4
	sw	$2,16($sp)
	lw	$2,16($sp)
	lw	$2,0($2)
	sw	$2,8($sp)
	lw	$2,0($sp)
	addiu	$3,$2,4
	lw	$2,8($sp)
	ori	$2,$2,0x4
	sw	$3,20($sp)
	sw	$2,24($sp)
	lw	$2,20($sp)
	lw	$3,24($sp)
	sw	$3,0($2)
	j	$L15
	nop

$L13:
	lw	$2,0($sp)
	addiu	$2,$2,4
	sw	$2,28($sp)
	lw	$2,28($sp)
	lw	$2,0($2)
	sw	$2,12($sp)
	lw	$2,0($sp)
	addiu	$3,$2,4
	lw	$4,12($sp)
	li	$2,-5			# 0xfffffffffffffffb
	and	$2,$4,$2
	sw	$3,32($sp)
	sw	$2,36($sp)
	lw	$2,32($sp)
	lw	$3,36($sp)
	sw	$3,0($2)
$L15:
	move	$2,$0
	addiu	$sp,$sp,40
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	qdma_rx_dma_mode
	.size	qdma_rx_dma_mode, .-qdma_rx_dma_mode
	.section	.text.qdma_loopback_mode,"ax",@progbits
	.align	2
	.globl	qdma_loopback_mode
	.set	nomips16
	.set	nomicromips
	.ent	qdma_loopback_mode
	.type	qdma_loopback_mode, @function
qdma_loopback_mode:
	.frame	$sp,104,$31		# vars= 104, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-104
	sw	$4,104($sp)
	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lw	$2,0($2)
	sw	$2,0($sp)
	lw	$2,104($sp)
	lw	$2,8($2)
	sw	$2,4($sp)
	lw	$2,4($sp)
	bne	$2,$0,$L19
	nop

	lw	$2,0($sp)
	addiu	$2,$2,4
	sw	$2,32($sp)
	lw	$2,32($sp)
	lw	$2,0($2)
	sw	$2,8($sp)
	lw	$2,0($sp)
	addiu	$3,$2,4
	lw	$4,8($sp)
	li	$2,-196608			# 0xfffffffffffd0000
	ori	$2,$2,0xffff
	and	$2,$4,$2
	sw	$3,36($sp)
	sw	$2,40($sp)
	lw	$2,36($sp)
	lw	$3,40($sp)
	sw	$3,0($2)
	lw	$2,0($sp)
	addiu	$2,$2,4
	sw	$2,44($sp)
	lw	$2,44($sp)
	lw	$2,0($2)
	sw	$2,12($sp)
	lw	$2,0($sp)
	addiu	$3,$2,4
	lw	$4,12($sp)
	li	$2,-131072			# 0xfffffffffffe0000
	ori	$2,$2,0xffff
	and	$2,$4,$2
	sw	$3,48($sp)
	sw	$2,52($sp)
	lw	$2,48($sp)
	lw	$3,52($sp)
	sw	$3,0($2)
	j	$L22
	nop

$L19:
	lw	$3,4($sp)
	li	$2,1			# 0x1
	bne	$3,$2,$L23
	nop

	lw	$2,0($sp)
	addiu	$2,$2,4
	sw	$2,56($sp)
	lw	$2,56($sp)
	lw	$2,0($2)
	sw	$2,16($sp)
	lw	$2,0($sp)
	addiu	$3,$2,4
	lw	$4,16($sp)
	li	$2,-196608			# 0xfffffffffffd0000
	ori	$2,$2,0xffff
	and	$2,$4,$2
	sw	$3,60($sp)
	sw	$2,64($sp)
	lw	$2,60($sp)
	lw	$3,64($sp)
	sw	$3,0($2)
	lw	$2,0($sp)
	addiu	$2,$2,4
	sw	$2,68($sp)
	lw	$2,68($sp)
	lw	$2,0($2)
	sw	$2,20($sp)
	lw	$2,0($sp)
	addiu	$3,$2,4
	lw	$4,20($sp)
	li	$2,65536			# 0x10000
	or	$2,$4,$2
	sw	$3,72($sp)
	sw	$2,76($sp)
	lw	$2,72($sp)
	lw	$3,76($sp)
	sw	$3,0($2)
	j	$L22
	nop

$L23:
	lw	$3,4($sp)
	li	$2,2			# 0x2
	bne	$3,$2,$L22
	nop

	lw	$2,0($sp)
	addiu	$2,$2,4
	sw	$2,80($sp)
	lw	$2,80($sp)
	lw	$2,0($2)
	sw	$2,24($sp)
	lw	$2,0($sp)
	addiu	$3,$2,4
	lw	$4,24($sp)
	li	$2,-131072			# 0xfffffffffffe0000
	ori	$2,$2,0xffff
	and	$2,$4,$2
	sw	$3,84($sp)
	sw	$2,88($sp)
	lw	$2,84($sp)
	lw	$3,88($sp)
	sw	$3,0($2)
	lw	$2,0($sp)
	addiu	$2,$2,4
	sw	$2,92($sp)
	lw	$2,92($sp)
	lw	$2,0($2)
	sw	$2,28($sp)
	lw	$2,0($sp)
	addiu	$3,$2,4
	lw	$4,28($sp)
	li	$2,131072			# 0x20000
	or	$2,$4,$2
	sw	$3,96($sp)
	sw	$2,100($sp)
	lw	$2,96($sp)
	lw	$3,100($sp)
	sw	$3,0($2)
$L22:
	move	$2,$0
	addiu	$sp,$sp,104
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	qdma_loopback_mode
	.size	qdma_loopback_mode, .-qdma_loopback_mode
	.section	.text.qdma_register_callback_function,"ax",@progbits
	.align	2
	.globl	qdma_register_callback_function
	.set	nomips16
	.set	nomicromips
	.ent	qdma_register_callback_function
	.type	qdma_register_callback_function, @function
qdma_register_callback_function:
	.frame	$sp,40,$31		# vars= 16, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-40
	sw	$31,36($sp)
	sw	$4,40($sp)
	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lw	$2,0($2)
	sw	$2,16($sp)
	lw	$2,40($sp)
	lw	$2,8($2)
	sw	$2,20($sp)
	lw	$2,40($sp)
	lw	$2,12($2)
	sw	$2,24($sp)
	lw	$2,24($sp)
	bne	$2,$0,$L30
	nop

	li	$2,-22			# 0xffffffffffffffea
	j	$L31
	nop

$L30:
	lw	$2,20($sp)
	sltu	$2,$2,6
	beq	$2,$0,$L32
	nop

	lw	$2,20($sp)
	sll	$3,$2,2
	lui	$2,%hi($L34)
	addiu	$2,$2,%lo($L34)
	addu	$2,$3,$2
	lw	$2,0($2)
	j	$2
	nop

	.rdata
	.align	2
	.align	2
$L34:
	.word	$L33
	.word	$L35
	.word	$L36
	.word	$L37
	.word	$L38
	.word	$L39
	.section	.text.qdma_register_callback_function
$L33:
	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lw	$3,24($sp)
	sw	$3,148($2)
	j	$L40
	nop

$L35:
	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lw	$3,24($sp)
	sw	$3,152($2)
	j	$L40
	nop

$L36:
	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lw	$3,24($sp)
	sw	$3,156($2)
	j	$L40
	nop

$L37:
	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lw	$3,24($sp)
	sw	$3,160($2)
	lw	$4,16($sp)
	li	$5,65536			# 0x10000
	lui	$2,%hi(qdmaEnableInt)
	addiu	$2,$2,%lo(qdmaEnableInt)
	jalr	$2
	nop

	j	$L40
	nop

$L38:
	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lw	$3,24($sp)
	sw	$3,164($2)
	lw	$4,16($sp)
	li	$5,131072			# 0x20000
	lui	$2,%hi(qdmaEnableInt)
	addiu	$2,$2,%lo(qdmaEnableInt)
	jalr	$2
	nop

	j	$L40
	nop

$L39:
	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lw	$3,24($sp)
	sw	$3,168($2)
	lw	$4,16($sp)
	li	$5,16777216			# 0x1000000
	lui	$2,%hi(qdmaEnableInt)
	addiu	$2,$2,%lo(qdmaEnableInt)
	jalr	$2
	nop

	j	$L40
	nop

$L32:
	li	$2,-14			# 0xfffffffffffffff2
	j	$L31
	nop

$L40:
	move	$2,$0
$L31:
	lw	$31,36($sp)
	addiu	$sp,$sp,40
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	qdma_register_callback_function
	.size	qdma_register_callback_function, .-qdma_register_callback_function
	.section	.text.qdma_unregister_callback_function,"ax",@progbits
	.align	2
	.globl	qdma_unregister_callback_function
	.set	nomips16
	.set	nomicromips
	.ent	qdma_unregister_callback_function
	.type	qdma_unregister_callback_function, @function
qdma_unregister_callback_function:
	.frame	$sp,32,$31		# vars= 8, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-32
	sw	$31,28($sp)
	sw	$4,32($sp)
	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lw	$2,0($2)
	sw	$2,16($sp)
	lw	$2,32($sp)
	lw	$2,8($2)
	sw	$2,20($sp)
	lw	$2,20($sp)
	sltu	$2,$2,6
	beq	$2,$0,$L42
	nop

	lw	$2,20($sp)
	sll	$3,$2,2
	lui	$2,%hi($L44)
	addiu	$2,$2,%lo($L44)
	addu	$2,$3,$2
	lw	$2,0($2)
	j	$2
	nop

	.rdata
	.align	2
	.align	2
$L44:
	.word	$L43
	.word	$L45
	.word	$L46
	.word	$L47
	.word	$L48
	.word	$L49
	.section	.text.qdma_unregister_callback_function
$L43:
	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	sw	$0,148($2)
	j	$L50
	nop

$L45:
	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	sw	$0,152($2)
	j	$L50
	nop

$L46:
	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	sw	$0,156($2)
	j	$L50
	nop

$L47:
	lw	$4,16($sp)
	li	$5,65536			# 0x10000
	lui	$2,%hi(qdmaDisableInt)
	addiu	$2,$2,%lo(qdmaDisableInt)
	jalr	$2
	nop

	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	sw	$0,160($2)
	j	$L50
	nop

$L48:
	lw	$4,16($sp)
	li	$5,131072			# 0x20000
	lui	$2,%hi(qdmaDisableInt)
	addiu	$2,$2,%lo(qdmaDisableInt)
	jalr	$2
	nop

	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	sw	$0,164($2)
	j	$L50
	nop

$L49:
	lw	$4,16($sp)
	li	$5,16777216			# 0x1000000
	lui	$2,%hi(qdmaDisableInt)
	addiu	$2,$2,%lo(qdmaDisableInt)
	jalr	$2
	nop

	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	sw	$0,168($2)
	j	$L50
	nop

$L42:
	li	$2,-14			# 0xfffffffffffffff2
	j	$L51
	nop

$L50:
	move	$2,$0
$L51:
	lw	$31,28($sp)
	addiu	$sp,$sp,32
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	qdma_unregister_callback_function
	.size	qdma_unregister_callback_function, .-qdma_unregister_callback_function
	.section	.imem_text,"ax",@progbits
	.align	2
	.globl	qdma_enable_rxpkt_int
	.set	nomips16
	.set	nomicromips
	.ent	qdma_enable_rxpkt_int
	.type	qdma_enable_rxpkt_int, @function
qdma_enable_rxpkt_int:
	.frame	$sp,32,$31		# vars= 8, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-32
	sw	$31,28($sp)
	sw	$4,32($sp)
	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lw	$2,0($2)
	sw	$2,16($sp)
	lw	$4,16($sp)
	li	$5,170			# 0xaa
	lui	$2,%hi(qdmaEnableInt)
	addiu	$2,$2,%lo(qdmaEnableInt)
	jalr	$2
	nop

	move	$2,$0
	lw	$31,28($sp)
	addiu	$sp,$sp,32
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	qdma_enable_rxpkt_int
	.size	qdma_enable_rxpkt_int, .-qdma_enable_rxpkt_int
	.align	2
	.globl	qdma_disable_rxpkt_int
	.set	nomips16
	.set	nomicromips
	.ent	qdma_disable_rxpkt_int
	.type	qdma_disable_rxpkt_int, @function
qdma_disable_rxpkt_int:
	.frame	$sp,32,$31		# vars= 8, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-32
	sw	$31,28($sp)
	sw	$4,32($sp)
	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lw	$2,0($2)
	sw	$2,16($sp)
	lw	$4,16($sp)
	li	$5,170			# 0xaa
	lui	$2,%hi(qdmaDisableInt)
	addiu	$2,$2,%lo(qdmaDisableInt)
	jalr	$2
	nop

	move	$2,$0
	lw	$31,28($sp)
	addiu	$sp,$sp,32
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	qdma_disable_rxpkt_int
	.size	qdma_disable_rxpkt_int, .-qdma_disable_rxpkt_int
	.align	2
	.globl	qdma_receive_packets
	.set	nomips16
	.set	nomicromips
	.ent	qdma_receive_packets
	.type	qdma_receive_packets, @function
qdma_receive_packets:
	.frame	$sp,64,$31		# vars= 40, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-64
	sw	$31,60($sp)
	sw	$4,64($sp)
	sw	$0,16($sp)
	sw	$0,32($sp)
	sw	$0,36($sp)
	sw	$0,20($sp)
	sw	$0,24($sp)
	lw	$2,64($sp)
	lw	$2,8($2)
	sw	$2,28($sp)
	lw	$2,28($sp)
	bne	$2,$0,$L57
	nop

	li	$2,640			# 0x280
	sw	$2,28($sp)
$L57:
	sw	$0,24($sp)
	lw	$2,24($sp)
	sw	$2,20($sp)
	lw	$3,28($sp)
	lw	$2,16($sp)
	subu	$2,$3,$2
	sw	$2,40($sp)
	li	$2,5			# 0x5
	sw	$2,44($sp)
	lw	$3,44($sp)
	lw	$2,40($sp)
	slt	$4,$2,$3
	movz	$2,$3,$4
	sw	$2,36($sp)
	lw	$2,36($sp)
	beq	$2,$0,$L58
	nop

	lw	$2,36($sp)
	move	$4,$2
	li	$5,1			# 0x1
	lui	$2,%hi(qdma_bm_receive_packets)
	addiu	$2,$2,%lo(qdma_bm_receive_packets)
	jalr	$2
	nop

	sw	$2,32($sp)
	lw	$3,32($sp)
	lw	$2,36($sp)
	slt	$2,$3,$2
	beq	$2,$0,$L66
	nop

	li	$2,1			# 0x1
	sw	$2,20($sp)
$L66:
	lw	$3,16($sp)
	lw	$2,32($sp)
	addu	$2,$3,$2
	sw	$2,16($sp)
$L58:
	lui	$2,%hi(isForceRX1)
	lw	$3,%lo(isForceRX1)($2)
	li	$2,1			# 0x1
	bne	$3,$2,$L60
	nop

	lw	$2,20($sp)
	bne	$2,$0,$L60
	nop

	j	$L61
	nop

$L60:
	lw	$3,28($sp)
	lw	$2,16($sp)
	subu	$2,$3,$2
	sw	$2,48($sp)
	li	$2,2			# 0x2
	sw	$2,52($sp)
	lw	$3,52($sp)
	lw	$2,48($sp)
	slt	$4,$2,$3
	movz	$2,$3,$4
	sw	$2,36($sp)
	lw	$2,36($sp)
	beq	$2,$0,$L62
	nop

	lw	$2,36($sp)
	move	$4,$2
	move	$5,$0
	lui	$2,%hi(qdma_bm_receive_packets)
	addiu	$2,$2,%lo(qdma_bm_receive_packets)
	jalr	$2
	nop

	sw	$2,32($sp)
	lw	$3,32($sp)
	lw	$2,36($sp)
	slt	$2,$3,$2
	beq	$2,$0,$L67
	nop

	li	$2,1			# 0x1
	sw	$2,24($sp)
$L67:
	lw	$3,16($sp)
	lw	$2,32($sp)
	addu	$2,$3,$2
	sw	$2,16($sp)
$L62:
	lw	$3,20($sp)
	li	$2,1			# 0x1
	bne	$3,$2,$L61
	nop

	lw	$3,24($sp)
	li	$2,1			# 0x1
	bne	$3,$2,$L61
	nop

	j	$L64
	nop

$L61:
	lw	$3,16($sp)
	lw	$2,28($sp)
	slt	$2,$3,$2
	bne	$2,$0,$L57
	nop

$L64:
	lw	$2,16($sp)
	lw	$31,60($sp)
	addiu	$sp,$sp,64
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	qdma_receive_packets
	.size	qdma_receive_packets, .-qdma_receive_packets
	.align	2
	.globl	qdma_transmit_packet
	.set	nomips16
	.set	nomicromips
	.ent	qdma_transmit_packet
	.type	qdma_transmit_packet, @function
qdma_transmit_packet:
	.frame	$sp,96,$31		# vars= 72, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-96
	sw	$31,92($sp)
	sw	$4,96($sp)
	lw	$2,96($sp)
	lw	$2,8($2)
	sw	$2,20($sp)
	lw	$2,96($sp)
	lw	$2,12($2)
	sw	$2,68($sp)
	lw	$2,96($sp)
	lw	$2,16($2)
	sw	$2,72($sp)
	lw	$2,96($sp)
	lw	$2,20($2)
	sw	$2,24($sp)
	sw	$0,16($sp)
	sw	$0,28($sp)
	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lw	$2,0($2)
	sw	$2,32($sp)
	addiu	$2,$sp,76
	sw	$2,36($sp)
	sw	$0,40($sp)
	sw	$0,44($sp)
	lw	$2,36($sp)
	addiu	$3,$sp,68
	sw	$3,0($2)
	lw	$2,36($sp)
	addiu	$3,$sp,72
	sw	$3,4($2)
	lw	$2,20($sp)
	bne	$2,$0,$L69
	nop

	li	$2,-14			# 0xfffffffffffffff2
	j	$L81
	nop

$L69:
	lw	$2,24($sp)
	beq	$2,$0,$L71
	nop

	lw	$2,24($sp)
	lw	$2,0($2)
	andi	$2,$2,0x10
	beq	$2,$0,$L71
	nop

	lw	$2,36($sp)
	lw	$3,0($2)
	lw	$2,24($sp)
	lw	$2,0($2)
	ext	$2,$2,28,4
	andi	$2,$2,0x00ff
	andi	$2,$2,0x7
	andi	$4,$2,0x00ff
	lw	$2,0($3)
	ins	$2,$4,0,3
	sw	$2,0($3)
	j	$L72
	nop

$L71:
	sw	$0,16($sp)
	lui	$2,%hi(TCSUPPORT_FWC_QOS_VAL)
	lw	$2,%lo(TCSUPPORT_FWC_QOS_VAL)($2)
	beq	$2,$0,$L73
	nop

	lw	$2,20($sp)
	sw	$2,48($sp)
	sw	$0,52($sp)
	sw	$0,56($sp)
	sb	$0,60($sp)
	lw	$2,48($sp)
	lw	$3,148($2)
	li	$2,65536			# 0x10000
	and	$2,$3,$2
	beq	$2,$0,$L74
	nop

	lw	$2,48($sp)
	lw	$2,148($2)
	srl	$2,$2,11
	andi	$2,$2,0x00ff
	andi	$2,$2,0x7
	sb	$2,60($sp)
	j	$L75
	nop

$L74:
	lw	$2,48($sp)
	lw	$2,256($2)
	addiu	$2,$2,12
	sw	$2,56($sp)
	lw	$2,56($sp)
	lhu	$3,0($2)
	li	$2,33024			# 0x8100
	bne	$3,$2,$L75
	nop

	lw	$2,56($sp)
	addiu	$2,$2,2
	sw	$2,52($sp)
	lw	$2,52($sp)
	lhu	$2,0($2)
	srl	$2,$2,13
	andi	$2,$2,0xffff
	andi	$2,$2,0x00ff
	andi	$2,$2,0x7
	sb	$2,60($sp)
$L75:
	lbu	$2,60($sp)
	sw	$2,28($sp)
	lw	$2,36($sp)
	lw	$3,0($2)
	lw	$2,28($sp)
	andi	$2,$2,0x00ff
	andi	$2,$2,0x7
	andi	$4,$2,0x00ff
	lw	$2,0($3)
	ins	$2,$4,0,3
	sw	$2,0($3)
	j	$L72
	nop

$L73:
	lw	$2,36($sp)
	lw	$3,0($2)
	lw	$2,0($3)
	ins	$2,$0,0,3
	sw	$2,0($3)
$L72:
	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lw	$3,140($2)
	li	$2,-2147483648			# 0xffffffff80000000
	and	$2,$3,$2
	beq	$2,$0,$L77
	nop

	lw	$2,32($sp)
	addiu	$2,$2,108
	sw	$2,64($sp)
	lw	$2,64($sp)
	lw	$3,0($2)
	li	$2,268369920			# 0xfff0000
	and	$2,$3,$2
	srl	$2,$2,16
	lui	$3,%hi(gpQdmaPriv)
	lw	$3,%lo(gpQdmaPriv)($3)
	lbu	$3,141($3)
	sltu	$2,$2,$3
	bne	$2,$0,$L77
	nop

	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lbu	$2,141($2)
	sll	$2,$2,2
	move	$4,$2
	lui	$2,%hi(qdma_bm_transmit_done)
	addiu	$2,$2,%lo(qdma_bm_transmit_done)
	jalr	$2
	nop

$L77:
	lw	$2,36($sp)
	lw	$2,0($2)
	lw	$2,0($2)
	ext	$2,$2,3,8
	andi	$2,$2,0x00ff
	sw	$2,40($sp)
	lw	$2,36($sp)
	lw	$2,0($2)
	lw	$2,0($2)
	ext	$2,$2,0,3
	andi	$2,$2,0x00ff
	sw	$2,44($sp)
	lw	$4,20($sp)
	lw	$5,40($sp)
	lw	$6,44($sp)
	lui	$2,%hi(macResourceLimit)
	addiu	$2,$2,%lo(macResourceLimit)
	jalr	$2
	nop

	beq	$2,$0,$L79
	nop

	li	$2,-14			# 0xfffffffffffffff2
	j	$L81
	nop

$L79:
	lw	$2,24($sp)
	beq	$2,$0,$L80
	nop

	lui	$2,%hi(ra_sw_nat_hook_tx)
	lw	$2,%lo(ra_sw_nat_hook_tx)($2)
	beq	$2,$0,$L80
	nop

	lw	$2,36($sp)
	lw	$2,0($2)
	lw	$2,0($2)
	ext	$2,$2,0,3
	andi	$2,$2,0x00ff
	andi	$2,$2,0xf
	andi	$4,$2,0x00ff
	lw	$3,24($sp)
	lw	$2,0($3)
	ins	$2,$4,28,4
	sw	$2,0($3)
	lui	$2,%hi(ra_sw_nat_hook_tx)
	lw	$2,%lo(ra_sw_nat_hook_tx)($2)
	lw	$3,24($sp)
	lhu	$3,6($3)
	lw	$4,20($sp)
	lw	$5,24($sp)
	move	$6,$3
	jalr	$2
	nop

	bne	$2,$0,$L80
	nop

	li	$2,-51			# 0xffffffffffffffcd
	j	$L81
	nop

$L80:
	lw	$3,68($sp)
	lw	$2,72($sp)
	lw	$4,20($sp)
	lw	$5,16($sp)
	move	$6,$3
	move	$7,$2
	lui	$2,%hi(qdma_bm_transmit_packet)
	addiu	$2,$2,%lo(qdma_bm_transmit_packet)
	jalr	$2
	nop

$L81:
	lw	$31,92($sp)
	addiu	$sp,$sp,96
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	qdma_transmit_packet
	.size	qdma_transmit_packet, .-qdma_transmit_packet
	.section	.text.qdma_set_tx_qos_weight,"ax",@progbits
	.align	2
	.globl	qdma_set_tx_qos_weight
	.set	nomips16
	.set	nomicromips
	.ent	qdma_set_tx_qos_weight
	.type	qdma_set_tx_qos_weight, @function
qdma_set_tx_qos_weight:
	.frame	$sp,80,$31		# vars= 80, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-80
	sw	$4,80($sp)
	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lw	$2,0($2)
	sw	$2,0($sp)
	lw	$2,80($sp)
	lw	$2,8($2)
	sw	$2,4($sp)
	lw	$2,80($sp)
	lw	$2,12($2)
	sw	$2,8($sp)
	lw	$2,4($sp)
	bne	$2,$0,$L83
	nop

	lw	$2,0($sp)
	addiu	$2,$2,128
	sw	$2,28($sp)
	lw	$2,28($sp)
	lw	$2,0($2)
	sw	$2,24($sp)
	lw	$2,0($sp)
	addiu	$3,$2,128
	lw	$4,24($sp)
	li	$2,-9			# 0xfffffffffffffff7
	and	$2,$4,$2
	sw	$3,32($sp)
	sw	$2,36($sp)
	lw	$2,32($sp)
	lw	$3,36($sp)
	sw	$3,0($2)
	j	$L85
	nop

$L83:
	lw	$3,4($sp)
	li	$2,1			# 0x1
	bne	$3,$2,$L86
	nop

	lw	$2,0($sp)
	addiu	$2,$2,128
	sw	$2,40($sp)
	lw	$2,40($sp)
	lw	$2,0($2)
	sw	$2,12($sp)
	lw	$2,0($sp)
	addiu	$3,$2,128
	lw	$2,12($sp)
	ori	$2,$2,0x8
	sw	$3,44($sp)
	sw	$2,48($sp)
	lw	$2,44($sp)
	lw	$3,48($sp)
	sw	$3,0($2)
	lw	$2,8($sp)
	bne	$2,$0,$L88
	nop

	lw	$2,0($sp)
	addiu	$2,$2,128
	sw	$2,52($sp)
	lw	$2,52($sp)
	lw	$2,0($2)
	sw	$2,20($sp)
	lw	$2,0($sp)
	addiu	$3,$2,128
	lw	$2,20($sp)
	ext	$2,$2,0,31
	sw	$3,56($sp)
	sw	$2,60($sp)
	lw	$2,56($sp)
	lw	$3,60($sp)
	sw	$3,0($2)
	j	$L85
	nop

$L88:
	lw	$3,8($sp)
	li	$2,1			# 0x1
	bne	$3,$2,$L90
	nop

	lw	$2,0($sp)
	addiu	$2,$2,128
	sw	$2,64($sp)
	lw	$2,64($sp)
	lw	$2,0($2)
	sw	$2,16($sp)
	lw	$2,0($sp)
	addiu	$3,$2,128
	lw	$4,16($sp)
	li	$2,-2147483648			# 0xffffffff80000000
	or	$2,$4,$2
	sw	$3,68($sp)
	sw	$2,72($sp)
	lw	$2,68($sp)
	lw	$3,72($sp)
	sw	$3,0($2)
	j	$L85
	nop

$L90:
	li	$2,-22			# 0xffffffffffffffea
	j	$L92
	nop

$L86:
	li	$2,-22			# 0xffffffffffffffea
	j	$L92
	nop

$L85:
	move	$2,$0
$L92:
	addiu	$sp,$sp,80
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	qdma_set_tx_qos_weight
	.size	qdma_set_tx_qos_weight, .-qdma_set_tx_qos_weight
	.section	.text.qdma_get_tx_qos_weight,"ax",@progbits
	.align	2
	.globl	qdma_get_tx_qos_weight
	.set	nomips16
	.set	nomicromips
	.ent	qdma_get_tx_qos_weight
	.type	qdma_get_tx_qos_weight, @function
qdma_get_tx_qos_weight:
	.frame	$sp,16,$31		# vars= 16, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-16
	sw	$4,16($sp)
	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lw	$2,0($2)
	sw	$2,0($sp)
	lw	$2,0($sp)
	addiu	$2,$2,128
	sw	$2,4($sp)
	lw	$2,4($sp)
	lw	$2,0($2)
	andi	$2,$2,0x8
	beq	$2,$0,$L95
	nop

	lw	$2,16($sp)
	li	$3,1			# 0x1
	sw	$3,8($2)
	j	$L96
	nop

$L95:
	lw	$2,16($sp)
	sw	$0,8($2)
$L96:
	lw	$2,0($sp)
	addiu	$2,$2,128
	sw	$2,8($sp)
	lw	$2,8($sp)
	lw	$2,0($2)
	bgez	$2,$L98
	nop

	lw	$2,16($sp)
	li	$3,1			# 0x1
	sw	$3,12($2)
	j	$L99
	nop

$L98:
	lw	$2,16($sp)
	sw	$0,12($2)
$L99:
	move	$2,$0
	addiu	$sp,$sp,16
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	qdma_get_tx_qos_weight
	.size	qdma_get_tx_qos_weight, .-qdma_get_tx_qos_weight
	.rdata
	.align	2
$LC0:
	.ascii	"channel is %d, old qosType is %d, new qosType is %d, Q0:"
	.ascii	"Q1:...:Q7=\000"
	.align	2
$LC1:
	.ascii	"%d \000"
	.align	2
$LC2:
	.ascii	"\012\000"
	.section	.text.qdma_set_tx_qos,"ax",@progbits
	.align	2
	.globl	qdma_set_tx_qos
	.set	nomips16
	.set	nomicromips
	.ent	qdma_set_tx_qos
	.type	qdma_set_tx_qos, @function
qdma_set_tx_qos:
	.frame	$sp,232,$31		# vars= 200, regs= 1/0, args= 24, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-232
	sw	$31,228($sp)
	sw	$4,232($sp)
	sw	$0,28($sp)
	sw	$0,24($sp)
	sb	$0,36($sp)
	li	$2,8			# 0x8
	sw	$2,192($sp)
	sw	$0,196($sp)
	li	$2,7			# 0x7
	sw	$2,200($sp)
	li	$2,6			# 0x6
	sw	$2,204($sp)
	li	$2,5			# 0x5
	sw	$2,208($sp)
	li	$2,4			# 0x4
	sw	$2,212($sp)
	li	$2,3			# 0x3
	sw	$2,216($sp)
	li	$2,2			# 0x2
	sw	$2,220($sp)
	lw	$2,232($sp)
	lw	$2,16($2)
	sw	$2,32($sp)
	lw	$2,32($sp)
	lbu	$2,0($2)
	sltu	$2,$2,8
	bne	$2,$0,$L102
	nop

	li	$2,-22			# 0xffffffffffffffea
	j	$L117
	nop

$L102:
	lw	$2,32($sp)
	lw	$2,4($2)
	sltu	$2,$2,8
	bne	$2,$0,$L104
	nop

	li	$2,-22			# 0xffffffffffffffea
	j	$L117
	nop

$L104:
	lw	$2,32($sp)
	lw	$2,4($2)
	andi	$2,$2,0x00ff
	sb	$2,36($sp)
	sw	$0,24($sp)
	j	$L105
	nop

$L113:
	lw	$3,32($sp)
	lw	$2,24($sp)
	addu	$2,$3,$2
	lbu	$2,8($2)
	bne	$2,$0,$L106
	nop

	lw	$2,32($sp)
	lbu	$2,0($2)
	move	$4,$2
	addiu	$3,$sp,36
	addiu	$2,$sp,40
	move	$5,$3
	move	$6,$2
	lui	$2,%hi(qdmaGetTxQosScheduler)
	addiu	$2,$2,%lo(qdmaGetTxQosScheduler)
	jalr	$2
	nop

	sw	$2,28($sp)
	lw	$2,28($sp)
	bgez	$2,$L107
	nop

	li	$2,-14			# 0xfffffffffffffff2
	j	$L117
	nop

$L107:
	sw	$0,24($sp)
	j	$L108
	nop

$L110:
	lw	$3,32($sp)
	lw	$2,24($sp)
	addu	$2,$3,$2
	lbu	$2,8($2)
	bne	$2,$0,$L109
	nop

	li	$2,-14			# 0xfffffffffffffff2
	j	$L117
	nop

$L109:
	lw	$2,24($sp)
	addiu	$2,$2,1
	sw	$2,24($sp)
$L108:
	lbu	$2,36($sp)
	sll	$2,$2,2
	addiu	$3,$sp,24
	addu	$2,$3,$2
	lw	$3,168($2)
	lw	$2,24($sp)
	slt	$2,$2,$3
	bne	$2,$0,$L110
	nop

	lw	$2,32($sp)
	lbu	$2,0($2)
	move	$6,$2
	lbu	$2,36($sp)
	move	$7,$2
	lw	$2,32($sp)
	lw	$2,4($2)
	addiu	$3,$sp,48
	sw	$2,16($sp)
	move	$4,$3
	lui	$2,%hi($LC0)
	addiu	$5,$2,%lo($LC0)
	lui	$2,%hi(sprintf)
	addiu	$2,$2,%lo(sprintf)
	jalr	$2
	nop

	sw	$0,24($sp)
	j	$L111
	nop

$L112:
	lw	$3,32($sp)
	lw	$2,24($sp)
	addu	$2,$3,$2
	lbu	$2,8($2)
	move	$3,$2
	addiu	$2,$sp,176
	move	$4,$2
	lui	$2,%hi($LC1)
	addiu	$5,$2,%lo($LC1)
	move	$6,$3
	lui	$2,%hi(sprintf)
	addiu	$2,$2,%lo(sprintf)
	jalr	$2
	nop

	addiu	$3,$sp,48
	addiu	$2,$sp,176
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(strcat)
	addiu	$2,$2,%lo(strcat)
	jalr	$2
	nop

	lw	$2,24($sp)
	addiu	$2,$2,1
	sw	$2,24($sp)
$L111:
	lw	$2,24($sp)
	slt	$2,$2,8
	bne	$2,$0,$L112
	nop

	addiu	$2,$sp,48
	move	$4,$2
	lui	$2,%hi($LC2)
	addiu	$5,$2,%lo($LC2)
	lui	$2,%hi(strcat)
	addiu	$2,$2,%lo(strcat)
	jalr	$2
	nop

$L106:
	lw	$2,24($sp)
	addiu	$2,$2,1
	sw	$2,24($sp)
$L105:
	lw	$2,32($sp)
	lw	$2,4($2)
	sll	$2,$2,2
	addiu	$3,$sp,24
	addu	$2,$3,$2
	lw	$3,168($2)
	lw	$2,24($sp)
	slt	$2,$2,$3
	bne	$2,$0,$L113
	nop

	sw	$0,24($sp)
	j	$L114
	nop

$L116:
	lw	$3,32($sp)
	lw	$2,24($sp)
	addu	$2,$3,$2
	lbu	$2,8($2)
	sltu	$2,$2,101
	bne	$2,$0,$L115
	nop

	lw	$3,32($sp)
	lw	$2,24($sp)
	addu	$2,$3,$2
	lbu	$3,8($2)
	li	$2,255			# 0xff
	beq	$3,$2,$L115
	nop

	li	$2,-22			# 0xffffffffffffffea
	j	$L117
	nop

$L115:
	lw	$3,32($sp)
	lw	$2,24($sp)
	addu	$2,$3,$2
	lbu	$3,8($2)
	lw	$2,24($sp)
	addiu	$4,$sp,24
	addu	$2,$4,$2
	sb	$3,16($2)
	lw	$2,24($sp)
	addiu	$2,$2,1
	sw	$2,24($sp)
$L114:
	lw	$2,24($sp)
	slt	$2,$2,8
	bne	$2,$0,$L116
	nop

	lw	$2,32($sp)
	lbu	$2,0($2)
	move	$4,$2
	lbu	$2,36($sp)
	move	$3,$2
	addiu	$2,$sp,40
	move	$5,$3
	move	$6,$2
	lui	$2,%hi(qdmaSetTxQosScheduler)
	addiu	$2,$2,%lo(qdmaSetTxQosScheduler)
	jalr	$2
	nop

$L117:
	lw	$31,228($sp)
	addiu	$sp,$sp,232
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	qdma_set_tx_qos
	.size	qdma_set_tx_qos, .-qdma_set_tx_qos
	.section	.text.qdma_get_tx_qos,"ax",@progbits
	.align	2
	.globl	qdma_get_tx_qos
	.set	nomips16
	.set	nomicromips
	.ent	qdma_get_tx_qos
	.type	qdma_get_tx_qos, @function
qdma_get_tx_qos:
	.frame	$sp,48,$31		# vars= 24, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-48
	sw	$31,44($sp)
	sw	$4,48($sp)
	sw	$0,20($sp)
	sw	$0,16($sp)
	sb	$0,28($sp)
	lw	$2,48($sp)
	lw	$2,16($2)
	sw	$2,24($sp)
	lw	$2,24($sp)
	lbu	$2,0($2)
	sltu	$2,$2,8
	bne	$2,$0,$L119
	nop

	li	$2,-22			# 0xffffffffffffffea
	j	$L124
	nop

$L119:
	lw	$2,24($sp)
	lbu	$2,0($2)
	move	$4,$2
	addiu	$3,$sp,28
	addiu	$2,$sp,32
	move	$5,$3
	move	$6,$2
	lui	$2,%hi(qdmaGetTxQosScheduler)
	addiu	$2,$2,%lo(qdmaGetTxQosScheduler)
	jalr	$2
	nop

	sw	$2,20($sp)
	lw	$2,20($sp)
	bgez	$2,$L121
	nop

	li	$2,-14			# 0xfffffffffffffff2
	j	$L124
	nop

$L121:
	lbu	$2,28($sp)
	move	$3,$2
	lw	$2,24($sp)
	sw	$3,4($2)
	sw	$0,16($sp)
	j	$L122
	nop

$L123:
	lw	$2,16($sp)
	addiu	$3,$sp,16
	addu	$2,$3,$2
	lbu	$3,16($2)
	lw	$4,24($sp)
	lw	$2,16($sp)
	addu	$2,$4,$2
	sb	$3,8($2)
	lw	$2,16($sp)
	addiu	$2,$2,1
	sw	$2,16($sp)
$L122:
	lw	$2,16($sp)
	slt	$2,$2,8
	bne	$2,$0,$L123
	nop

	move	$2,$0
$L124:
	lw	$31,44($sp)
	addiu	$sp,$sp,48
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	qdma_get_tx_qos
	.size	qdma_get_tx_qos, .-qdma_get_tx_qos
	.section	.text.qdma_set_mac_limit_threshold,"ax",@progbits
	.align	2
	.globl	qdma_set_mac_limit_threshold
	.set	nomips16
	.set	nomicromips
	.ent	qdma_set_mac_limit_threshold
	.type	qdma_set_mac_limit_threshold, @function
qdma_set_mac_limit_threshold:
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	sw	$4,0($sp)
	lw	$2,0($sp)
	lw	$2,8($2)
	lw	$3,0($2)
	lui	$2,%hi(channel_limit_threshold)
	sw	$3,%lo(channel_limit_threshold)($2)
	lw	$2,0($sp)
	lw	$2,8($2)
	lw	$3,4($2)
	lui	$2,%hi(queue_limit_threshold)
	sw	$3,%lo(queue_limit_threshold)($2)
	move	$2,$0
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	qdma_set_mac_limit_threshold
	.size	qdma_set_mac_limit_threshold, .-qdma_set_mac_limit_threshold
	.section	.text.qdma_get_mac_limit_threshold,"ax",@progbits
	.align	2
	.globl	qdma_get_mac_limit_threshold
	.set	nomips16
	.set	nomicromips
	.ent	qdma_get_mac_limit_threshold
	.type	qdma_get_mac_limit_threshold, @function
qdma_get_mac_limit_threshold:
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	sw	$4,0($sp)
	lw	$2,0($sp)
	lw	$2,8($2)
	lui	$3,%hi(channel_limit_threshold)
	lw	$3,%lo(channel_limit_threshold)($3)
	sw	$3,0($2)
	lw	$2,0($sp)
	lw	$2,8($2)
	lui	$3,%hi(queue_limit_threshold)
	lw	$3,%lo(queue_limit_threshold)($3)
	sw	$3,4($2)
	move	$2,$0
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	qdma_get_mac_limit_threshold
	.size	qdma_get_mac_limit_threshold, .-qdma_get_mac_limit_threshold
	.section	.text.qdma_set_txbuf_threshold,"ax",@progbits
	.align	2
	.globl	qdma_set_txbuf_threshold
	.set	nomips16
	.set	nomicromips
	.ent	qdma_set_txbuf_threshold
	.type	qdma_set_txbuf_threshold, @function
qdma_set_txbuf_threshold:
	.frame	$sp,72,$31		# vars= 72, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-72
	sw	$4,72($sp)
	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lw	$2,0($2)
	sw	$2,0($sp)
	lw	$2,72($sp)
	lw	$2,8($2)
	sw	$2,4($sp)
	lw	$2,4($sp)
	lw	$3,0($2)
	li	$2,1			# 0x1
	bne	$3,$2,$L130
	nop

	lw	$2,0($sp)
	addiu	$2,$2,144
	sw	$2,24($sp)
	lw	$2,24($sp)
	lw	$2,0($2)
	sw	$2,12($sp)
	lw	$2,0($sp)
	addiu	$3,$2,144
	lw	$4,12($sp)
	li	$2,-2147483648			# 0xffffffff80000000
	or	$2,$4,$2
	sw	$3,28($sp)
	sw	$2,32($sp)
	lw	$2,28($sp)
	lw	$3,32($sp)
	sw	$3,0($2)
	lw	$2,0($sp)
	addiu	$2,$2,144
	sw	$2,36($sp)
	lw	$2,36($sp)
	lw	$2,0($2)
	sw	$2,16($sp)
	lw	$2,0($sp)
	addiu	$3,$2,144
	lw	$4,16($sp)
	li	$2,-16777216			# 0xffffffffff000000
	ori	$2,$2,0xffff
	and	$4,$4,$2
	lw	$2,4($sp)
	lbu	$2,4($2)
	sll	$2,$2,16
	move	$5,$2
	li	$2,16711680			# 0xff0000
	and	$2,$5,$2
	or	$2,$4,$2
	sw	$3,40($sp)
	sw	$2,44($sp)
	lw	$2,40($sp)
	lw	$3,44($sp)
	sw	$3,0($2)
	lw	$2,0($sp)
	addiu	$2,$2,144
	sw	$2,48($sp)
	lw	$2,48($sp)
	lw	$2,0($2)
	sw	$2,20($sp)
	lw	$2,0($sp)
	addiu	$3,$2,144
	lw	$4,20($sp)
	li	$2,-256			# 0xffffffffffffff00
	and	$2,$4,$2
	lw	$4,4($sp)
	lbu	$4,5($4)
	or	$2,$2,$4
	sw	$3,52($sp)
	sw	$2,56($sp)
	lw	$2,52($sp)
	lw	$3,56($sp)
	sw	$3,0($2)
	j	$L134
	nop

$L130:
	lw	$2,4($sp)
	lw	$2,0($2)
	bne	$2,$0,$L135
	nop

	lw	$2,0($sp)
	addiu	$2,$2,144
	sw	$2,60($sp)
	lw	$2,60($sp)
	lw	$2,0($2)
	sw	$2,8($sp)
	lw	$2,0($sp)
	addiu	$3,$2,144
	lw	$2,8($sp)
	ext	$2,$2,0,31
	sw	$3,64($sp)
	sw	$2,68($sp)
	lw	$2,64($sp)
	lw	$3,68($sp)
	sw	$3,0($2)
	j	$L134
	nop

$L135:
	li	$2,-22			# 0xffffffffffffffea
	j	$L137
	nop

$L134:
	move	$2,$0
$L137:
	addiu	$sp,$sp,72
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	qdma_set_txbuf_threshold
	.size	qdma_set_txbuf_threshold, .-qdma_set_txbuf_threshold
	.section	.text.qdma_get_txbuf_threshold,"ax",@progbits
	.align	2
	.globl	qdma_get_txbuf_threshold
	.set	nomips16
	.set	nomicromips
	.ent	qdma_get_txbuf_threshold
	.type	qdma_get_txbuf_threshold, @function
qdma_get_txbuf_threshold:
	.frame	$sp,24,$31		# vars= 24, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-24
	sw	$4,24($sp)
	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lw	$2,0($2)
	sw	$2,0($sp)
	lw	$2,24($sp)
	lw	$2,8($2)
	sw	$2,4($sp)
	lw	$2,0($sp)
	addiu	$2,$2,144
	sw	$2,8($sp)
	lw	$2,8($sp)
	lw	$2,0($2)
	srl	$2,$2,31
	andi	$2,$2,0x00ff
	move	$3,$2
	lw	$2,4($sp)
	sw	$3,0($2)
	lw	$2,0($sp)
	addiu	$2,$2,144
	sw	$2,12($sp)
	lw	$2,12($sp)
	lw	$3,0($2)
	li	$2,16711680			# 0xff0000
	and	$2,$3,$2
	srl	$2,$2,16
	andi	$3,$2,0x00ff
	lw	$2,4($sp)
	sb	$3,4($2)
	lw	$2,0($sp)
	addiu	$2,$2,144
	sw	$2,16($sp)
	lw	$2,16($sp)
	lw	$2,0($2)
	andi	$3,$2,0x00ff
	lw	$2,4($sp)
	sb	$3,5($2)
	move	$2,$0
	addiu	$sp,$sp,24
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	qdma_get_txbuf_threshold
	.size	qdma_get_txbuf_threshold, .-qdma_get_txbuf_threshold
	.section	.text.qdma_set_prefetch_mode,"ax",@progbits
	.align	2
	.globl	qdma_set_prefetch_mode
	.set	nomips16
	.set	nomicromips
	.ent	qdma_set_prefetch_mode
	.type	qdma_set_prefetch_mode, @function
qdma_set_prefetch_mode:
	.frame	$sp,88,$31		# vars= 88, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-88
	sw	$4,88($sp)
	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lw	$2,0($2)
	sw	$2,0($sp)
	lw	$2,88($sp)
	lw	$2,8($2)
	sw	$2,4($sp)
	lw	$2,4($sp)
	lw	$2,0($2)
	bne	$2,$0,$L144
	nop

	lw	$2,0($sp)
	addiu	$2,$2,144
	sw	$2,28($sp)
	lw	$2,28($sp)
	lw	$2,0($2)
	sw	$2,8($sp)
	lw	$2,0($sp)
	addiu	$3,$2,144
	lw	$4,8($sp)
	li	$2,-1073807360			# 0xffffffffbfff0000
	ori	$2,$2,0xffff
	and	$2,$4,$2
	sw	$3,32($sp)
	sw	$2,36($sp)
	lw	$2,32($sp)
	lw	$3,36($sp)
	sw	$3,0($2)
	j	$L146
	nop

$L144:
	lw	$2,0($sp)
	addiu	$2,$2,144
	sw	$2,40($sp)
	lw	$2,40($sp)
	lw	$2,0($2)
	sw	$2,12($sp)
	lw	$2,0($sp)
	addiu	$3,$2,144
	lw	$4,12($sp)
	li	$2,1073741824			# 0x40000000
	or	$2,$4,$2
	sw	$3,44($sp)
	sw	$2,48($sp)
	lw	$2,44($sp)
	lw	$3,48($sp)
	sw	$3,0($2)
$L146:
	lw	$2,4($sp)
	lw	$2,4($2)
	bne	$2,$0,$L148
	nop

	lw	$2,0($sp)
	addiu	$2,$2,140
	sw	$2,52($sp)
	lw	$2,52($sp)
	lw	$2,0($2)
	sw	$2,16($sp)
	lw	$2,0($sp)
	addiu	$3,$2,140
	lw	$4,16($sp)
	li	$2,-65536			# 0xffffffffffff0000
	ori	$2,$2,0x7fff
	and	$2,$4,$2
	sw	$3,56($sp)
	sw	$2,60($sp)
	lw	$2,56($sp)
	lw	$3,60($sp)
	sw	$3,0($2)
	j	$L150
	nop

$L148:
	lw	$2,0($sp)
	addiu	$2,$2,140
	sw	$2,64($sp)
	lw	$2,64($sp)
	lw	$2,0($2)
	sw	$2,20($sp)
	lw	$2,0($sp)
	addiu	$3,$2,140
	lw	$2,20($sp)
	ori	$2,$2,0x8000
	sw	$3,68($sp)
	sw	$2,72($sp)
	lw	$2,68($sp)
	lw	$3,72($sp)
	sw	$3,0($2)
$L150:
	lw	$2,0($sp)
	addiu	$2,$2,140
	sw	$2,76($sp)
	lw	$2,76($sp)
	lw	$2,0($2)
	sw	$2,24($sp)
	lw	$2,0($sp)
	addiu	$3,$2,140
	lw	$4,24($sp)
	li	$2,-32768			# 0xffffffffffff8000
	and	$4,$4,$2
	lw	$2,4($sp)
	lw	$2,8($2)
	andi	$2,$2,0x7fff
	or	$2,$4,$2
	sw	$3,80($sp)
	sw	$2,84($sp)
	lw	$2,80($sp)
	lw	$3,84($sp)
	sw	$3,0($2)
	move	$2,$0
	addiu	$sp,$sp,88
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	qdma_set_prefetch_mode
	.size	qdma_set_prefetch_mode, .-qdma_set_prefetch_mode
	.section	.text.qdma_set_pktsize_overhead_en,"ax",@progbits
	.align	2
	.globl	qdma_set_pktsize_overhead_en
	.set	nomips16
	.set	nomicromips
	.ent	qdma_set_pktsize_overhead_en
	.type	qdma_set_pktsize_overhead_en, @function
qdma_set_pktsize_overhead_en:
	.frame	$sp,40,$31		# vars= 40, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-40
	sw	$4,40($sp)
	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lw	$2,0($2)
	sw	$2,0($sp)
	lw	$2,40($sp)
	lw	$2,8($2)
	sw	$2,4($sp)
	lw	$2,4($sp)
	bne	$2,$0,$L155
	nop

	lw	$2,0($sp)
	addiu	$2,$2,48
	sw	$2,16($sp)
	lw	$2,16($sp)
	lw	$2,0($2)
	sw	$2,8($sp)
	lw	$2,0($sp)
	addiu	$3,$2,48
	lw	$4,8($sp)
	li	$2,-16842752			# 0xfffffffffeff0000
	ori	$2,$2,0xffff
	and	$2,$4,$2
	sw	$3,20($sp)
	sw	$2,24($sp)
	lw	$2,20($sp)
	lw	$3,24($sp)
	sw	$3,0($2)
	j	$L157
	nop

$L155:
	lw	$2,0($sp)
	addiu	$2,$2,48
	sw	$2,28($sp)
	lw	$2,28($sp)
	lw	$2,0($2)
	sw	$2,12($sp)
	lw	$2,0($sp)
	addiu	$3,$2,48
	lw	$4,12($sp)
	li	$2,16777216			# 0x1000000
	or	$2,$4,$2
	sw	$3,32($sp)
	sw	$2,36($sp)
	lw	$2,32($sp)
	lw	$3,36($sp)
	sw	$3,0($2)
$L157:
	move	$2,$0
	addiu	$sp,$sp,40
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	qdma_set_pktsize_overhead_en
	.size	qdma_set_pktsize_overhead_en, .-qdma_set_pktsize_overhead_en
	.section	.text.qdma_get_pktsize_overhead_en,"ax",@progbits
	.align	2
	.globl	qdma_get_pktsize_overhead_en
	.set	nomips16
	.set	nomicromips
	.ent	qdma_get_pktsize_overhead_en
	.type	qdma_get_pktsize_overhead_en, @function
qdma_get_pktsize_overhead_en:
	.frame	$sp,8,$31		# vars= 8, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-8
	sw	$4,8($sp)
	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lw	$2,0($2)
	sw	$2,0($sp)
	lw	$2,0($sp)
	addiu	$2,$2,48
	sw	$2,4($sp)
	lw	$2,4($sp)
	lw	$3,0($2)
	li	$2,16777216			# 0x1000000
	and	$2,$3,$2
	beq	$2,$0,$L162
	nop

	lw	$2,8($sp)
	li	$3,1			# 0x1
	sw	$3,8($2)
	j	$L163
	nop

$L162:
	lw	$2,8($sp)
	sw	$0,8($2)
$L163:
	lw	$2,8($sp)
	lw	$2,8($2)
	addiu	$sp,$sp,8
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	qdma_get_pktsize_overhead_en
	.size	qdma_get_pktsize_overhead_en, .-qdma_get_pktsize_overhead_en
	.section	.text.qdma_set_pktsize_overhead_value,"ax",@progbits
	.align	2
	.globl	qdma_set_pktsize_overhead_value
	.set	nomips16
	.set	nomicromips
	.ent	qdma_set_pktsize_overhead_value
	.type	qdma_set_pktsize_overhead_value, @function
qdma_set_pktsize_overhead_value:
	.frame	$sp,24,$31		# vars= 24, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-24
	sw	$4,24($sp)
	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lw	$2,0($2)
	sw	$2,0($sp)
	lw	$2,24($sp)
	lw	$2,8($2)
	sw	$2,4($sp)
	lw	$2,0($sp)
	addiu	$2,$2,48
	sw	$2,12($sp)
	lw	$2,12($sp)
	lw	$2,0($2)
	sw	$2,8($sp)
	lw	$2,0($sp)
	addiu	$3,$2,48
	lw	$4,8($sp)
	li	$2,-16777216			# 0xffffffffff000000
	ori	$2,$2,0xffff
	and	$4,$4,$2
	lw	$2,4($sp)
	sll	$5,$2,16
	li	$2,16711680			# 0xff0000
	and	$2,$5,$2
	or	$2,$4,$2
	sw	$3,16($sp)
	sw	$2,20($sp)
	lw	$2,16($sp)
	lw	$3,20($sp)
	sw	$3,0($2)
	move	$2,$0
	addiu	$sp,$sp,24
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	qdma_set_pktsize_overhead_value
	.size	qdma_set_pktsize_overhead_value, .-qdma_set_pktsize_overhead_value
	.section	.text.qdma_get_pktsize_overhead_value,"ax",@progbits
	.align	2
	.globl	qdma_get_pktsize_overhead_value
	.set	nomips16
	.set	nomicromips
	.ent	qdma_get_pktsize_overhead_value
	.type	qdma_get_pktsize_overhead_value, @function
qdma_get_pktsize_overhead_value:
	.frame	$sp,8,$31		# vars= 8, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-8
	sw	$4,8($sp)
	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lw	$2,0($2)
	sw	$2,0($sp)
	lw	$2,0($sp)
	addiu	$2,$2,48
	sw	$2,4($sp)
	lw	$2,4($sp)
	lw	$3,0($2)
	li	$2,16711680			# 0xff0000
	and	$2,$3,$2
	srl	$2,$2,16
	move	$3,$2
	lw	$2,8($sp)
	sw	$3,8($2)
	lw	$2,8($sp)
	lw	$2,8($2)
	addiu	$sp,$sp,8
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	qdma_get_pktsize_overhead_value
	.size	qdma_get_pktsize_overhead_value, .-qdma_get_pktsize_overhead_value
	.section	.text.qdma_set_lmgr_low_threshold,"ax",@progbits
	.align	2
	.globl	qdma_set_lmgr_low_threshold
	.set	nomips16
	.set	nomicromips
	.ent	qdma_set_lmgr_low_threshold
	.type	qdma_set_lmgr_low_threshold, @function
qdma_set_lmgr_low_threshold:
	.frame	$sp,24,$31		# vars= 24, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-24
	sw	$4,24($sp)
	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lw	$2,0($2)
	sw	$2,0($sp)
	lw	$2,24($sp)
	lw	$2,8($2)
	sw	$2,4($sp)
	lw	$2,0($sp)
	addiu	$2,$2,40
	sw	$2,12($sp)
	lw	$2,12($sp)
	lw	$2,0($2)
	sw	$2,8($sp)
	lw	$2,0($sp)
	addiu	$3,$2,40
	lw	$4,8($sp)
	li	$2,-8192			# 0xffffffffffffe000
	and	$4,$4,$2
	lw	$2,4($sp)
	andi	$2,$2,0x1fff
	or	$2,$4,$2
	sw	$3,16($sp)
	sw	$2,20($sp)
	lw	$2,16($sp)
	lw	$3,20($sp)
	sw	$3,0($2)
	move	$2,$0
	addiu	$sp,$sp,24
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	qdma_set_lmgr_low_threshold
	.size	qdma_set_lmgr_low_threshold, .-qdma_set_lmgr_low_threshold
	.section	.text.qdma_get_lmgr_low_threshold,"ax",@progbits
	.align	2
	.globl	qdma_get_lmgr_low_threshold
	.set	nomips16
	.set	nomicromips
	.ent	qdma_get_lmgr_low_threshold
	.type	qdma_get_lmgr_low_threshold, @function
qdma_get_lmgr_low_threshold:
	.frame	$sp,8,$31		# vars= 8, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-8
	sw	$4,8($sp)
	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lw	$2,0($2)
	sw	$2,0($sp)
	lw	$2,0($sp)
	addiu	$2,$2,40
	sw	$2,4($sp)
	lw	$2,4($sp)
	lw	$2,0($2)
	andi	$3,$2,0x1fff
	lw	$2,8($sp)
	sw	$3,8($2)
	lw	$2,8($sp)
	lw	$2,8($2)
	addiu	$sp,$sp,8
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	qdma_get_lmgr_low_threshold
	.size	qdma_get_lmgr_low_threshold, .-qdma_get_lmgr_low_threshold
	.section	.text.qdma_get_lmgr_status,"ax",@progbits
	.align	2
	.globl	qdma_get_lmgr_status
	.set	nomips16
	.set	nomicromips
	.ent	qdma_get_lmgr_status
	.type	qdma_get_lmgr_status, @function
qdma_get_lmgr_status:
	.frame	$sp,16,$31		# vars= 16, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-16
	sw	$4,16($sp)
	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lw	$2,0($2)
	sw	$2,0($sp)
	lw	$2,0($sp)
	addiu	$2,$2,240
	sw	$2,8($sp)
	lw	$2,8($sp)
	lw	$2,0($2)
	andi	$2,$2,0x1fff
	sw	$2,4($sp)
	lw	$2,16($sp)
	lw	$2,8($2)
	lw	$3,4($sp)
	sw	$3,0($2)
	lw	$2,16($sp)
	lw	$2,8($2)
	li	$4,1024			# 0x400
	lw	$3,4($sp)
	subu	$3,$4,$3
	sw	$3,4($2)
	lw	$2,16($sp)
	lw	$2,8($2)
	lw	$3,0($sp)
	addiu	$3,$3,244
	sw	$3,12($sp)
	lw	$3,12($sp)
	lw	$3,0($3)
	ext	$3,$3,0,24
	sw	$3,8($2)
	move	$2,$0
	addiu	$sp,$sp,16
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	qdma_get_lmgr_status
	.size	qdma_get_lmgr_status, .-qdma_get_lmgr_status
	.section	.text.qdma_clear_cpu_counters,"ax",@progbits
	.align	2
	.globl	qdma_clear_cpu_counters
	.set	nomips16
	.set	nomicromips
	.ent	qdma_clear_cpu_counters
	.type	qdma_clear_cpu_counters, @function
qdma_clear_cpu_counters:
	.frame	$sp,24,$31		# vars= 0, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-24
	sw	$31,20($sp)
	sw	$4,24($sp)
	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	addiu	$2,$2,172
	move	$4,$2
	move	$5,$0
	li	$6,132			# 0x84
	lui	$2,%hi(memset)
	addiu	$2,$2,%lo(memset)
	jalr	$2
	nop

	move	$2,$0
	lw	$31,20($sp)
	addiu	$sp,$sp,24
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	qdma_clear_cpu_counters
	.size	qdma_clear_cpu_counters, .-qdma_clear_cpu_counters
	.rdata
	.align	2
$LC3:
	.ascii	"IRQ Queue Asynchronous             %u\012\000"
	.align	2
$LC4:
	.ascii	"IRQ Queue Index Errors:            %u\012\000"
	.align	2
$LC5:
	.ascii	"IRQ Queue Full Interrupt:          %u\012\000"
	.align	2
$LC6:
	.ascii	"IRQ Queue Error Number:            %u\012\000"
	.align	2
$LC7:
	.ascii	"RX Allocate Fail Errors:           %u\012\000"
	.align	2
$LC8:
	.ascii	"No Link DSCP Interrupt:            %u\012\000"
	.align	2
$LC9:
	.ascii	"Low Link DSCP Interrupt:           %u\012\000"
	.align	2
$LC10:
	.ascii	"--------------------------------------------\012\000"
	.align	2
$LC11:
	.ascii	"Tx%d DSCP Counts:                    %u\012\000"
	.align	2
$LC12:
	.ascii	"Tx%d DSCP Recycle Counts:            %u\012\000"
	.align	2
$LC13:
	.ascii	"Tx%d DSCP Content Incorrect:         %u\012\000"
	.align	2
$LC14:
	.ascii	"Tx%d Done Bit Errors:                %u\012\000"
	.align	2
$LC15:
	.ascii	"No Tx%d DSCP Errors:                 %u\012\000"
	.align	2
$LC16:
	.ascii	"Tx%d Chorent Interrupt:              %u\012\000"
	.align	2
$LC17:
	.ascii	"No Tx%d Buffer Interrupt:            %u\012\000"
	.align	2
$LC18:
	.ascii	"No Tx%d DSCP Interrupt:              %u\012\000"
	.align	2
$LC19:
	.ascii	"Tx%d Done Interrupt:                 %u\012\000"
	.align	2
$LC20:
	.ascii	"Rx%d DSCP Counts:                    %u\012\000"
	.align	2
$LC21:
	.ascii	"Rx%d DSCP Content Incorrect:         %u\012\000"
	.align	2
$LC22:
	.ascii	"Rx%d Packet Errors:                  %u\012\000"
	.align	2
$LC23:
	.ascii	"No Rx%d Callback Errors:             %u\012\000"
	.align	2
$LC24:
	.ascii	"No Rx%d DSCP Errors:                 %u\012\000"
	.align	2
$LC25:
	.ascii	"Rx%d Allocate Fail Drop Errors:      %u\012\000"
	.align	2
$LC26:
	.ascii	"Rx%d Chorent Interrupt:              %u\012\000"
	.align	2
$LC27:
	.ascii	"No Rx%d Buffer Interrupt:            %u\012\000"
	.align	2
$LC28:
	.ascii	"No Rx%d DSCP Interrupt:              %u\012\000"
	.align	2
$LC29:
	.ascii	"Rx%d Broadcast Interrupt:            %u\012\000"
	.align	2
$LC30:
	.ascii	"Rx%d Unknown Unicast Interrupt:      %u\012\000"
	.align	2
$LC31:
	.ascii	"Rx%d Multicast Interrupt:            %u\012\000"
	.align	2
$LC32:
	.ascii	"Rx%d Done Interrupt:                 %u\012\000"
	.section	.text.qdma_dump_cpu_counters,"ax",@progbits
	.align	2
	.globl	qdma_dump_cpu_counters
	.set	nomips16
	.set	nomicromips
	.ent	qdma_dump_cpu_counters
	.type	qdma_dump_cpu_counters, @function
qdma_dump_cpu_counters:
	.frame	$sp,32,$31		# vars= 8, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-32
	sw	$31,28($sp)
	sw	$4,32($sp)
	sw	$0,16($sp)
	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lhu	$2,172($2)
	move	$3,$2
	lui	$2,%hi($LC3)
	addiu	$4,$2,%lo($LC3)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lhu	$2,174($2)
	move	$3,$2
	lui	$2,%hi($LC4)
	addiu	$4,$2,%lo($LC4)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lhu	$2,180($2)
	move	$3,$2
	lui	$2,%hi($LC5)
	addiu	$4,$2,%lo($LC5)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lhu	$2,182($2)
	move	$3,$2
	lui	$2,%hi($LC6)
	addiu	$4,$2,%lo($LC6)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lw	$3,176($2)
	lui	$2,%hi($LC7)
	addiu	$4,$2,%lo($LC7)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lhu	$2,184($2)
	move	$3,$2
	lui	$2,%hi($LC8)
	addiu	$4,$2,%lo($LC8)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lhu	$2,186($2)
	move	$3,$2
	lui	$2,%hi($LC9)
	addiu	$4,$2,%lo($LC9)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi($LC10)
	addiu	$4,$2,%lo($LC10)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	sw	$0,16($sp)
	j	$L184
	nop

$L185:
	lui	$2,%hi(gpQdmaPriv)
	lw	$3,%lo(gpQdmaPriv)($2)
	lw	$2,16($sp)
	addiu	$2,$2,48
	sll	$2,$2,2
	addu	$2,$3,$2
	lw	$3,4($2)
	lui	$2,%hi($LC11)
	addiu	$4,$2,%lo($LC11)
	lw	$5,16($sp)
	move	$6,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi(gpQdmaPriv)
	lw	$3,%lo(gpQdmaPriv)($2)
	lw	$2,16($sp)
	addiu	$2,$2,50
	sll	$2,$2,2
	addu	$2,$3,$2
	lw	$3,4($2)
	lui	$2,%hi($LC12)
	addiu	$4,$2,%lo($LC12)
	lw	$5,16($sp)
	move	$6,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi(gpQdmaPriv)
	lw	$3,%lo(gpQdmaPriv)($2)
	lw	$2,16($sp)
	addiu	$2,$2,108
	sll	$2,$2,1
	addu	$2,$3,$2
	lhu	$2,4($2)
	move	$3,$2
	lui	$2,%hi($LC13)
	addiu	$4,$2,%lo($LC13)
	lw	$5,16($sp)
	move	$6,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi(gpQdmaPriv)
	lw	$3,%lo(gpQdmaPriv)($2)
	lw	$2,16($sp)
	addiu	$2,$2,108
	sll	$2,$2,1
	addu	$2,$3,$2
	lhu	$2,8($2)
	move	$3,$2
	lui	$2,%hi($LC14)
	addiu	$4,$2,%lo($LC14)
	lw	$5,16($sp)
	move	$6,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi(gpQdmaPriv)
	lw	$3,%lo(gpQdmaPriv)($2)
	lw	$2,16($sp)
	addiu	$2,$2,116
	sll	$2,$2,1
	addu	$2,$3,$2
	lhu	$2,4($2)
	move	$3,$2
	lui	$2,%hi($LC15)
	addiu	$4,$2,%lo($LC15)
	lw	$5,16($sp)
	move	$6,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi(gpQdmaPriv)
	lw	$3,%lo(gpQdmaPriv)($2)
	lw	$2,16($sp)
	addiu	$2,$2,124
	sll	$2,$2,1
	addu	$2,$3,$2
	lhu	$2,8($2)
	move	$3,$2
	lui	$2,%hi($LC16)
	addiu	$4,$2,%lo($LC16)
	lw	$5,16($sp)
	move	$6,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi(gpQdmaPriv)
	lw	$3,%lo(gpQdmaPriv)($2)
	lw	$2,16($sp)
	addiu	$2,$2,128
	sll	$2,$2,1
	addu	$2,$3,$2
	lhu	$2,8($2)
	move	$3,$2
	lui	$2,%hi($LC17)
	addiu	$4,$2,%lo($LC17)
	lw	$5,16($sp)
	move	$6,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi(gpQdmaPriv)
	lw	$3,%lo(gpQdmaPriv)($2)
	lw	$2,16($sp)
	addiu	$2,$2,132
	sll	$2,$2,1
	addu	$2,$3,$2
	lhu	$2,8($2)
	move	$3,$2
	lui	$2,%hi($LC18)
	addiu	$4,$2,%lo($LC18)
	lw	$5,16($sp)
	move	$6,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi(gpQdmaPriv)
	lw	$3,%lo(gpQdmaPriv)($2)
	lw	$2,16($sp)
	addiu	$2,$2,72
	sll	$2,$2,2
	addu	$2,$3,$2
	lw	$3,8($2)
	lui	$2,%hi($LC19)
	addiu	$4,$2,%lo($LC19)
	lw	$5,16($sp)
	move	$6,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi($LC10)
	addiu	$4,$2,%lo($LC10)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lw	$2,16($sp)
	addiu	$2,$2,1
	sw	$2,16($sp)
$L184:
	lw	$2,16($sp)
	slt	$2,$2,2
	bne	$2,$0,$L185
	nop

	sw	$0,16($sp)
	j	$L186
	nop

$L187:
	lui	$2,%hi(gpQdmaPriv)
	lw	$3,%lo(gpQdmaPriv)($2)
	lw	$2,16($sp)
	addiu	$2,$2,52
	sll	$2,$2,2
	addu	$2,$3,$2
	lw	$3,4($2)
	lui	$2,%hi($LC20)
	addiu	$4,$2,%lo($LC20)
	lw	$5,16($sp)
	move	$6,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi(gpQdmaPriv)
	lw	$3,%lo(gpQdmaPriv)($2)
	lw	$2,16($sp)
	addiu	$2,$2,112
	sll	$2,$2,1
	addu	$2,$3,$2
	lhu	$2,4($2)
	move	$3,$2
	lui	$2,%hi($LC21)
	addiu	$4,$2,%lo($LC21)
	lw	$5,16($sp)
	move	$6,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi(gpQdmaPriv)
	lw	$3,%lo(gpQdmaPriv)($2)
	lw	$2,16($sp)
	addiu	$2,$2,112
	sll	$2,$2,1
	addu	$2,$3,$2
	lhu	$2,8($2)
	move	$3,$2
	lui	$2,%hi($LC22)
	addiu	$4,$2,%lo($LC22)
	lw	$5,16($sp)
	move	$6,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi(gpQdmaPriv)
	lw	$3,%lo(gpQdmaPriv)($2)
	lw	$2,16($sp)
	addiu	$2,$2,116
	sll	$2,$2,1
	addu	$2,$3,$2
	lhu	$2,8($2)
	move	$3,$2
	lui	$2,%hi($LC23)
	addiu	$4,$2,%lo($LC23)
	lw	$5,16($sp)
	move	$6,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi(gpQdmaPriv)
	lw	$3,%lo(gpQdmaPriv)($2)
	lw	$2,16($sp)
	addiu	$2,$2,120
	sll	$2,$2,1
	addu	$2,$3,$2
	lhu	$2,4($2)
	move	$3,$2
	lui	$2,%hi($LC24)
	addiu	$4,$2,%lo($LC24)
	lw	$5,16($sp)
	move	$6,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi(gpQdmaPriv)
	lw	$3,%lo(gpQdmaPriv)($2)
	lw	$2,16($sp)
	addiu	$2,$2,120
	sll	$2,$2,1
	addu	$2,$3,$2
	lhu	$2,8($2)
	move	$3,$2
	lui	$2,%hi($LC25)
	addiu	$4,$2,%lo($LC25)
	lw	$5,16($sp)
	move	$6,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi(gpQdmaPriv)
	lw	$3,%lo(gpQdmaPriv)($2)
	lw	$2,16($sp)
	addiu	$2,$2,124
	sll	$2,$2,1
	addu	$2,$3,$2
	lhu	$2,4($2)
	move	$3,$2
	lui	$2,%hi($LC26)
	addiu	$4,$2,%lo($LC26)
	lw	$5,16($sp)
	move	$6,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi(gpQdmaPriv)
	lw	$3,%lo(gpQdmaPriv)($2)
	lw	$2,16($sp)
	addiu	$2,$2,128
	sll	$2,$2,1
	addu	$2,$3,$2
	lhu	$2,4($2)
	move	$3,$2
	lui	$2,%hi($LC27)
	addiu	$4,$2,%lo($LC27)
	lw	$5,16($sp)
	move	$6,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi(gpQdmaPriv)
	lw	$3,%lo(gpQdmaPriv)($2)
	lw	$2,16($sp)
	addiu	$2,$2,132
	sll	$2,$2,1
	addu	$2,$3,$2
	lhu	$2,4($2)
	move	$3,$2
	lui	$2,%hi($LC28)
	addiu	$4,$2,%lo($LC28)
	lw	$5,16($sp)
	move	$6,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi(gpQdmaPriv)
	lw	$3,%lo(gpQdmaPriv)($2)
	lw	$2,16($sp)
	addiu	$2,$2,136
	sll	$2,$2,1
	addu	$2,$3,$2
	lhu	$2,4($2)
	move	$3,$2
	lui	$2,%hi($LC29)
	addiu	$4,$2,%lo($LC29)
	lw	$5,16($sp)
	move	$6,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi(gpQdmaPriv)
	lw	$3,%lo(gpQdmaPriv)($2)
	lw	$2,16($sp)
	addiu	$2,$2,136
	sll	$2,$2,1
	addu	$2,$3,$2
	lhu	$2,8($2)
	move	$3,$2
	lui	$2,%hi($LC30)
	addiu	$4,$2,%lo($LC30)
	lw	$5,16($sp)
	move	$6,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi(gpQdmaPriv)
	lw	$3,%lo(gpQdmaPriv)($2)
	lw	$2,16($sp)
	addiu	$2,$2,140
	sll	$2,$2,1
	addu	$2,$3,$2
	lhu	$2,4($2)
	move	$3,$2
	lui	$2,%hi($LC31)
	addiu	$4,$2,%lo($LC31)
	lw	$5,16($sp)
	move	$6,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi(gpQdmaPriv)
	lw	$3,%lo(gpQdmaPriv)($2)
	lw	$2,16($sp)
	addiu	$2,$2,70
	sll	$2,$2,2
	addu	$2,$3,$2
	lw	$3,8($2)
	lui	$2,%hi($LC32)
	addiu	$4,$2,%lo($LC32)
	lw	$5,16($sp)
	move	$6,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi($LC10)
	addiu	$4,$2,%lo($LC10)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lw	$2,16($sp)
	addiu	$2,$2,1
	sw	$2,16($sp)
$L186:
	lw	$2,16($sp)
	slt	$2,$2,2
	bne	$2,$0,$L187
	nop

	move	$2,$0
	lw	$31,28($sp)
	addiu	$sp,$sp,32
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	qdma_dump_cpu_counters
	.size	qdma_dump_cpu_counters, .-qdma_dump_cpu_counters
	.rdata
	.align	2
$LC33:
	.ascii	"Polling\000"
	.align	2
$LC34:
	.ascii	"Interrupt\000"
	.align	2
$LC35:
	.ascii	"Tx Mode %s \012\000"
	.align	2
$LC36:
	.ascii	"Napi\000"
	.align	2
$LC37:
	.ascii	"Rx Mode %s \012\000"
	.align	2
$LC38:
	.ascii	"gpQdmaPriv->csrBaseAddr:\011\011\011\011\011 0x%08x\012\000"
	.align	2
$LC39:
	.ascii	"gpQdmaPriv->hwFwdDscpNum:\011\011\011\011\011 %u\012\000"
	.align	2
$LC40:
	.ascii	"gpQdmaPriv->irqDepth:\011\011\011\011\011\011 %u\012\000"
	.align	2
$LC41:
	.ascii	"gpQdmaPriv->hwPktSize:\011\011\011\011\011\011 %u\012\000"
	.align	2
$LC42:
	.ascii	"gpQdmaPriv->dscpInfoAddr:\011\011\011\011\011 0x%08x\012"
	.ascii	"\000"
	.align	2
$LC43:
	.ascii	"gpQdmaPriv->irqQueueAddr:\011\011\011\011\011 0x%08x\012"
	.ascii	"\000"
	.align	2
$LC44:
	.ascii	"gpQdmaPriv->hwFwdBaseAddr:\011\011\011\011\011 0x%08x\012"
	.ascii	"\000"
	.align	2
$LC45:
	.ascii	"gpQdmaPriv->hwFwdBuffAddr:\011\011\011\011\011 0x%08x\012"
	.ascii	"\000"
	.align	2
$LC46:
	.ascii	"gpQdmaPriv->hwFwdPayloadSize:\011\011\011\011 %u\012\012"
	.ascii	"\000"
	.align	2
$LC47:
	.ascii	"gpQdmaPriv->txDscpNum[%d]:\011\011\011\011\011  %u\012\000"
	.align	2
$LC48:
	.ascii	"gpQdmaPriv->txBaseAddr[%d]: \011\011\011\011  0x%08x\012"
	.ascii	"\000"
	.align	2
$LC49:
	.ascii	"gpQdmaPriv->txHeadPtr[%d]->dscpIdx: \011\011  %d\012\000"
	.align	2
$LC50:
	.ascii	"gpQdmaPriv->txTailPtr[%d]->dscpIdx: \011\011  %d\012\000"
	.align	2
$LC51:
	.ascii	"gpQdmaPriv->txUsingPtr[%d]->dscpIdx:\011\011  %d\012\012"
	.ascii	"\000"
	.align	2
$LC52:
	.ascii	"gpQdmaPriv->rxDscpNum[%d]:\011\011\011\011\011  %u\012\000"
	.align	2
$LC53:
	.ascii	"gpQdmaPriv->rxBaseAddr[%d]: \011\011\011\011  0x%08x\012"
	.ascii	"\000"
	.align	2
$LC54:
	.ascii	"gpQdmaPriv->rxStartPtr[%d]->dscpIdx:\011\011  %d\012\000"
	.align	2
$LC55:
	.ascii	"gpQdmaPriv->rxEndPtr[%d]->dscpIdx:\011\011\011  %d\012\000"
	.align	2
$LC56:
	.ascii	"gpQdmaPriv->rxUsingPtr[%d]->dscpIdx:\011\011  %d\012\012"
	.ascii	"\000"
	.align	2
$LC57:
	.ascii	"0x0000: QDMA_CSR_INFO                           :%.8x\012"
	.ascii	"\000"
	.align	2
$LC58:
	.ascii	"0x0004: QDMA_CSR_GLB_CFG                        :%.8x\012"
	.ascii	"\000"
	.align	2
$LC59:
	.ascii	"0x%04X: QDMA_CSR_TX_DSCP_BASE[%d]                :%.8x\012"
	.ascii	"\000"
	.align	2
$LC60:
	.ascii	"0x%04X: QDMA_CSR_TX_CPU_IDX[%d]                  :%.8x\012"
	.ascii	"\000"
	.align	2
$LC61:
	.ascii	"0x%04X: QDMA_CSR_TX_DMA_IDX[%d]                  :%.8x\012"
	.ascii	"\000"
	.align	2
$LC62:
	.ascii	"0x%04X: QDMA_CSR_RX_DSCP_BASE[%d]                :%.8x\012"
	.ascii	"\000"
	.align	2
$LC63:
	.ascii	"0x%04X: QDMA_CSR_RX_CPU_IDX[%d]                  :%.8x\012"
	.ascii	"\000"
	.align	2
$LC64:
	.ascii	"0x%04X: QDMA_CSR_RX_DMA_IDX[%d]                  :%.8x\012"
	.ascii	"\000"
	.align	2
$LC65:
	.ascii	"0x0020: QDMA_CSR_HWFWD_DSCP_BASE                :%.8x\012"
	.ascii	"\000"
	.align	2
$LC66:
	.ascii	"0x0024: QDMA_CSR_HWFWD_BUFF_BASE                :%.8x\012"
	.ascii	"\000"
	.align	2
$LC67:
	.ascii	"0x0028: QDMA_CSR_HWFWD_DSCP_CFG                 :%.8x\012"
	.ascii	"\000"
	.align	2
$LC68:
	.ascii	"0x0030: QDMA_CSR_LMGR_INIT_CFG                  :%.8x\012"
	.ascii	"\000"
	.align	2
$LC69:
	.ascii	"0x0040: QDMA_CSR_LMGR_CHNL_RETIRE               :%.8x\012"
	.ascii	"\000"
	.align	2
$LC70:
	.ascii	"0x0050: QDMA_CSR_INT_STATUS                     :%.8x\012"
	.ascii	"\000"
	.align	2
$LC71:
	.ascii	"0x0054: QDMA_CSR_INT_ENABLE                     :%.8x\012"
	.ascii	"\000"
	.align	2
$LC72:
	.ascii	"0x0058: QDMA_CSR_TX_DELAY_INT_CFG               :%.8x\012"
	.ascii	"\000"
	.align	2
$LC73:
	.ascii	"0x005C: QDMA_CSR_RX_DELAY_INT_CFG               :%.8x\012"
	.ascii	"\000"
	.align	2
$LC74:
	.ascii	"0x0060: QDMA_CSR_IRQ_BASE                       :%.8x\012"
	.ascii	"\000"
	.align	2
$LC75:
	.ascii	"0x0064: QDMA_CSR_IRQ_CFG                        :%.8x\012"
	.ascii	"\000"
	.align	2
$LC76:
	.ascii	"0x0068: QDMA_CSR_IRQ_CLEAR_LEN                  :%.8x\012"
	.ascii	"\000"
	.align	2
$LC77:
	.ascii	"0x006C: QDMA_CSR_IRQ_STATUS                     :%.8x\012"
	.ascii	"\000"
	.align	2
$LC78:
	.ascii	"0x0070: QDMA_CSR_IRQ_PTIME                      :%.8x\012"
	.ascii	"\000"
	.align	2
$LC79:
	.ascii	"0x0080: QDMA_CSR_TXWRR_MODE_CFG                 :%.8x\012"
	.ascii	"\000"
	.align	2
$LC80:
	.ascii	"0x0088: QDMA_CSR_TXWRR_WEIGHT_CFG               :%.8x\012"
	.ascii	"\000"
	.align	2
$LC81:
	.ascii	"0x0090: QDMA_CSR_PSE_BUF_USAGE_CFG              :%.8x\012"
	.ascii	"\000"
	.align	2
$LC82:
	.ascii	"0x0094: QDMA_CSR_EGRESS_RATEMETER_CFG           :%.8x\012"
	.ascii	"\000"
	.align	2
$LC83:
	.ascii	"0x0098: QDMA_CSR_EGRESS_RATELIMIT_CFG           :%.8x\012"
	.ascii	"\000"
	.align	2
$LC84:
	.ascii	"0x009C: QDMA_CSR_RATELIMIT_PARAMETER_CFG        :%.8x\012"
	.ascii	"\000"
	.align	2
$LC85:
	.ascii	"0x00A0: QDMA_CSR_TXQ_CNGST_CFG                  :%.8x\012"
	.ascii	"\000"
	.align	2
$LC86:
	.ascii	"0x00A4: QDMA_CSR_TXQ_DYN_TOTALTHR               :%.8x\012"
	.ascii	"\000"
	.align	2
$LC87:
	.ascii	"0x00A8: QDMA_CSR_TXQ_DYN_CHNLTHR_CFG            :%.8x\012"
	.ascii	"\000"
	.align	2
$LC88:
	.ascii	"0x00AC: QDMA_CSR_TXQ_DYN_QUEUETHR_CFG           :%.8x\012"
	.ascii	"\000"
	.align	2
$LC89:
	.ascii	"0x%04X: QDMA_CSR_STATIC_QUEUE_THR_%d             :%.8x\012"
	.ascii	"\000"
	.align	2
$LC90:
	.ascii	"0x00F0: QDMA_CSR_DBG_LMGR_STATUS                :%.8x\012"
	.ascii	"\000"
	.align	2
$LC91:
	.ascii	"0x00F4: QDMA_CSR_DBG_LMGR_USAGE                 :%.8x\012"
	.ascii	"\000"
	.align	2
$LC92:
	.ascii	"0x00F8: QDMA_CSR_DBG_QDMA_PROBE_LO              :%.8x\012"
	.ascii	"\000"
	.align	2
$LC93:
	.ascii	"0x00FC: QDMA_CSR_DBG_QDMA_PROBE_HI              :%.8x\012"
	.ascii	"\000"
	.align	2
$LC94:
	.ascii	"0x0100: QDMA_CSR_RX_RING_CFG                    :%.8x\012"
	.ascii	"\000"
	.align	2
$LC95:
	.ascii	"0x0104: QDMA_CSR_RX_RING_THR                    :%.8x\012"
	.ascii	"\000"
	.align	2
$LC96:
	.ascii	"0x0120: QDMA_CSR_RX_PROTECT_CFG                 :%.8x\012"
	.ascii	"\000"
	.align	2
$LC97:
	.ascii	"0x0124: QDMA_CSR_RX_RATELIMIT_CFG               :%.8x\012"
	.ascii	"\000"
	.align	2
$LC98:
	.ascii	"0x%04x: QDMA_CSR_PERCHNL_QOS_MODE_%02d_%02d         :%.8"
	.ascii	"x\012\000"
	.align	2
$LC99:
	.ascii	"0x%04x: QDMA_CSR_PERCHNL_DATARATE_%02d_%02d         :%.8"
	.ascii	"x\012\000"
	.align	2
$LC100:
	.ascii	"0x%04x: QDMA_CSR_PERCHNL_QUEUE_NORM_DROP_%02d_%02d  :%.8"
	.ascii	"x\012\000"
	.align	2
$LC101:
	.ascii	"0x%04x: QDMA_CSR_PERCHNL_QUEUE_DEI_DROP_%02d_%02d   :%.8"
	.ascii	"x\012\000"
	.section	.text.qdma_dump_register_value,"ax",@progbits
	.align	2
	.globl	qdma_dump_register_value
	.set	nomips16
	.set	nomicromips
	.ent	qdma_dump_register_value
	.type	qdma_dump_register_value, @function
qdma_dump_register_value:
	.frame	$sp,224,$31		# vars= 192, regs= 1/0, args= 24, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-224
	sw	$31,220($sp)
	sw	$4,224($sp)
	sw	$0,24($sp)
	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lw	$2,0($2)
	sw	$2,28($sp)
	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lw	$3,140($2)
	li	$2,-2147483648			# 0xffffffff80000000
	and	$2,$3,$2
	beq	$2,$0,$L190
	nop

	lui	$2,%hi($LC33)
	addiu	$2,$2,%lo($LC33)
	j	$L191
	nop

$L190:
	lui	$2,%hi($LC34)
	addiu	$2,$2,%lo($LC34)
$L191:
	lui	$3,%hi($LC35)
	addiu	$4,$3,%lo($LC35)
	move	$5,$2
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lw	$3,140($2)
	li	$2,1073741824			# 0x40000000
	and	$2,$3,$2
	bne	$2,$0,$L192
	nop

	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lw	$3,140($2)
	li	$2,536870912			# 0x20000000
	and	$2,$3,$2
	beq	$2,$0,$L193
	nop

	lui	$2,%hi($LC36)
	addiu	$2,$2,%lo($LC36)
	j	$L195
	nop

$L193:
	lui	$2,%hi($LC34)
	addiu	$2,$2,%lo($LC34)
	j	$L195
	nop

$L192:
	lui	$2,%hi($LC33)
	addiu	$2,$2,%lo($LC33)
$L195:
	lui	$3,%hi($LC37)
	addiu	$4,$3,%lo($LC37)
	move	$5,$2
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lw	$3,0($2)
	lui	$2,%hi($LC38)
	addiu	$4,$2,%lo($LC38)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lhu	$2,28($2)
	move	$3,$2
	lui	$2,%hi($LC39)
	addiu	$4,$2,%lo($LC39)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lhu	$2,30($2)
	move	$3,$2
	lui	$2,%hi($LC40)
	addiu	$4,$2,%lo($LC40)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lhu	$2,32($2)
	move	$3,$2
	lui	$2,%hi($LC41)
	addiu	$4,$2,%lo($LC41)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lw	$3,36($2)
	lui	$2,%hi($LC42)
	addiu	$4,$2,%lo($LC42)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lw	$3,56($2)
	lui	$2,%hi($LC43)
	addiu	$4,$2,%lo($LC43)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lw	$3,60($2)
	lui	$2,%hi($LC44)
	addiu	$4,$2,%lo($LC44)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lw	$3,64($2)
	lui	$2,%hi($LC45)
	addiu	$4,$2,%lo($LC45)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lw	$3,68($2)
	lui	$2,%hi($LC46)
	addiu	$4,$2,%lo($LC46)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	sw	$0,24($sp)
	j	$L196
	nop

$L203:
	lui	$2,%hi(gpQdmaPriv)
	lw	$3,%lo(gpQdmaPriv)($2)
	lw	$2,24($sp)
	addiu	$2,$2,8
	sll	$2,$2,1
	addu	$2,$3,$2
	lhu	$2,4($2)
	move	$3,$2
	lui	$2,%hi($LC47)
	addiu	$4,$2,%lo($LC47)
	lw	$5,24($sp)
	move	$6,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi(gpQdmaPriv)
	lw	$3,%lo(gpQdmaPriv)($2)
	lw	$2,24($sp)
	addiu	$2,$2,10
	sll	$2,$2,2
	addu	$2,$3,$2
	lw	$3,0($2)
	lui	$2,%hi($LC48)
	addiu	$4,$2,%lo($LC48)
	lw	$5,24($sp)
	move	$6,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi(gpQdmaPriv)
	lw	$3,%lo(gpQdmaPriv)($2)
	lw	$2,24($sp)
	addiu	$2,$2,18
	sll	$2,$2,2
	addu	$2,$3,$2
	lw	$2,0($2)
	beq	$2,$0,$L197
	nop

	lui	$2,%hi(gpQdmaPriv)
	lw	$3,%lo(gpQdmaPriv)($2)
	lw	$2,24($sp)
	addiu	$2,$2,18
	sll	$2,$2,2
	addu	$2,$3,$2
	lw	$2,0($2)
	lw	$2,4($2)
	j	$L198
	nop

$L197:
	li	$2,-1			# 0xffffffffffffffff
$L198:
	lui	$3,%hi($LC49)
	addiu	$4,$3,%lo($LC49)
	lw	$5,24($sp)
	move	$6,$2
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi(gpQdmaPriv)
	lw	$3,%lo(gpQdmaPriv)($2)
	lw	$2,24($sp)
	addiu	$2,$2,20
	sll	$2,$2,2
	addu	$2,$3,$2
	lw	$2,0($2)
	beq	$2,$0,$L199
	nop

	lui	$2,%hi(gpQdmaPriv)
	lw	$3,%lo(gpQdmaPriv)($2)
	lw	$2,24($sp)
	addiu	$2,$2,20
	sll	$2,$2,2
	addu	$2,$3,$2
	lw	$2,0($2)
	lw	$2,4($2)
	j	$L200
	nop

$L199:
	li	$2,-1			# 0xffffffffffffffff
$L200:
	lui	$3,%hi($LC50)
	addiu	$4,$3,%lo($LC50)
	lw	$5,24($sp)
	move	$6,$2
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi(gpQdmaPriv)
	lw	$3,%lo(gpQdmaPriv)($2)
	lw	$2,24($sp)
	addiu	$2,$2,22
	sll	$2,$2,2
	addu	$2,$3,$2
	lw	$2,0($2)
	beq	$2,$0,$L201
	nop

	lui	$2,%hi(gpQdmaPriv)
	lw	$3,%lo(gpQdmaPriv)($2)
	lw	$2,24($sp)
	addiu	$2,$2,22
	sll	$2,$2,2
	addu	$2,$3,$2
	lw	$2,0($2)
	lw	$2,4($2)
	j	$L202
	nop

$L201:
	li	$2,-1			# 0xffffffffffffffff
$L202:
	lui	$3,%hi($LC51)
	addiu	$4,$3,%lo($LC51)
	lw	$5,24($sp)
	move	$6,$2
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lw	$2,24($sp)
	addiu	$2,$2,1
	sw	$2,24($sp)
$L196:
	lw	$2,24($sp)
	slt	$2,$2,2
	bne	$2,$0,$L203
	nop

	sw	$0,24($sp)
	j	$L204
	nop

$L211:
	lui	$2,%hi(gpQdmaPriv)
	lw	$3,%lo(gpQdmaPriv)($2)
	lw	$2,24($sp)
	addiu	$2,$2,12
	sll	$2,$2,1
	addu	$2,$3,$2
	lhu	$2,0($2)
	move	$3,$2
	lui	$2,%hi($LC52)
	addiu	$4,$2,%lo($LC52)
	lw	$5,24($sp)
	move	$6,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi(gpQdmaPriv)
	lw	$3,%lo(gpQdmaPriv)($2)
	lw	$2,24($sp)
	addiu	$2,$2,12
	sll	$2,$2,2
	addu	$2,$3,$2
	lw	$3,0($2)
	lui	$2,%hi($LC53)
	addiu	$4,$2,%lo($LC53)
	lw	$5,24($sp)
	move	$6,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi(gpQdmaPriv)
	lw	$3,%lo(gpQdmaPriv)($2)
	lw	$2,24($sp)
	addiu	$2,$2,24
	sll	$2,$2,2
	addu	$2,$3,$2
	lw	$2,0($2)
	beq	$2,$0,$L205
	nop

	lui	$2,%hi(gpQdmaPriv)
	lw	$3,%lo(gpQdmaPriv)($2)
	lw	$2,24($sp)
	addiu	$2,$2,24
	sll	$2,$2,2
	addu	$2,$3,$2
	lw	$2,0($2)
	lw	$2,4($2)
	j	$L206
	nop

$L205:
	li	$2,-1			# 0xffffffffffffffff
$L206:
	lui	$3,%hi($LC54)
	addiu	$4,$3,%lo($LC54)
	lw	$5,24($sp)
	move	$6,$2
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi(gpQdmaPriv)
	lw	$3,%lo(gpQdmaPriv)($2)
	lw	$2,24($sp)
	addiu	$2,$2,26
	sll	$2,$2,2
	addu	$2,$3,$2
	lw	$2,0($2)
	beq	$2,$0,$L207
	nop

	lui	$2,%hi(gpQdmaPriv)
	lw	$3,%lo(gpQdmaPriv)($2)
	lw	$2,24($sp)
	addiu	$2,$2,26
	sll	$2,$2,2
	addu	$2,$3,$2
	lw	$2,0($2)
	lw	$2,4($2)
	j	$L208
	nop

$L207:
	li	$2,-1			# 0xffffffffffffffff
$L208:
	lui	$3,%hi($LC55)
	addiu	$4,$3,%lo($LC55)
	lw	$5,24($sp)
	move	$6,$2
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi(gpQdmaPriv)
	lw	$3,%lo(gpQdmaPriv)($2)
	lw	$2,24($sp)
	addiu	$2,$2,28
	sll	$2,$2,2
	addu	$2,$3,$2
	lw	$2,0($2)
	beq	$2,$0,$L209
	nop

	lui	$2,%hi(gpQdmaPriv)
	lw	$3,%lo(gpQdmaPriv)($2)
	lw	$2,24($sp)
	addiu	$2,$2,28
	sll	$2,$2,2
	addu	$2,$3,$2
	lw	$2,0($2)
	lw	$2,4($2)
	j	$L210
	nop

$L209:
	li	$2,-1			# 0xffffffffffffffff
$L210:
	lui	$3,%hi($LC56)
	addiu	$4,$3,%lo($LC56)
	lw	$5,24($sp)
	move	$6,$2
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lw	$2,24($sp)
	addiu	$2,$2,1
	sw	$2,24($sp)
$L204:
	lw	$2,24($sp)
	slt	$2,$2,2
	bne	$2,$0,$L211
	nop

	lw	$2,28($sp)
	sw	$2,32($sp)
	lw	$2,32($sp)
	lw	$3,0($2)
	lui	$2,%hi($LC57)
	addiu	$4,$2,%lo($LC57)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lw	$2,28($sp)
	addiu	$2,$2,4
	sw	$2,36($sp)
	lw	$2,36($sp)
	lw	$3,0($2)
	lui	$2,%hi($LC58)
	addiu	$4,$2,%lo($LC58)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	sw	$0,24($sp)
	j	$L214
	nop

$L224:
	lw	$2,24($sp)
	beq	$2,$0,$L215
	nop

	li	$2,264			# 0x108
	j	$L216
	nop

$L215:
	li	$2,8			# 0x8
$L216:
	lw	$3,24($sp)
	sll	$3,$3,8
	move	$4,$3
	lw	$3,28($sp)
	addu	$3,$4,$3
	addiu	$3,$3,8
	sw	$3,40($sp)
	lw	$3,40($sp)
	lw	$7,0($3)
	lui	$3,%hi($LC59)
	addiu	$4,$3,%lo($LC59)
	move	$5,$2
	lw	$6,24($sp)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lw	$2,24($sp)
	beq	$2,$0,$L218
	nop

	li	$2,272			# 0x110
	j	$L219
	nop

$L218:
	li	$2,16			# 0x10
$L219:
	lw	$3,24($sp)
	sll	$3,$3,8
	move	$4,$3
	lw	$3,28($sp)
	addu	$3,$4,$3
	addiu	$3,$3,16
	sw	$3,44($sp)
	lw	$3,44($sp)
	lw	$7,0($3)
	lui	$3,%hi($LC60)
	addiu	$4,$3,%lo($LC60)
	move	$5,$2
	lw	$6,24($sp)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lw	$2,24($sp)
	beq	$2,$0,$L221
	nop

	li	$2,276			# 0x114
	j	$L222
	nop

$L221:
	li	$2,20			# 0x14
$L222:
	lw	$3,24($sp)
	sll	$3,$3,8
	move	$4,$3
	lw	$3,28($sp)
	addu	$3,$4,$3
	addiu	$3,$3,20
	sw	$3,48($sp)
	lw	$3,48($sp)
	lw	$7,0($3)
	lui	$3,%hi($LC61)
	addiu	$4,$3,%lo($LC61)
	move	$5,$2
	lw	$6,24($sp)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lw	$2,24($sp)
	addiu	$2,$2,1
	sw	$2,24($sp)
$L214:
	lw	$2,24($sp)
	slt	$2,$2,2
	bne	$2,$0,$L224
	nop

	sw	$0,24($sp)
	j	$L225
	nop

$L235:
	lw	$2,24($sp)
	beq	$2,$0,$L226
	nop

	li	$2,268			# 0x10c
	j	$L227
	nop

$L226:
	li	$2,12			# 0xc
$L227:
	lw	$3,24($sp)
	sll	$3,$3,8
	move	$4,$3
	lw	$3,28($sp)
	addu	$3,$4,$3
	addiu	$3,$3,12
	sw	$3,52($sp)
	lw	$3,52($sp)
	lw	$7,0($3)
	lui	$3,%hi($LC62)
	addiu	$4,$3,%lo($LC62)
	move	$5,$2
	lw	$6,24($sp)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lw	$2,24($sp)
	beq	$2,$0,$L229
	nop

	li	$2,280			# 0x118
	j	$L230
	nop

$L229:
	li	$2,24			# 0x18
$L230:
	lw	$3,24($sp)
	sll	$3,$3,8
	move	$4,$3
	lw	$3,28($sp)
	addu	$3,$4,$3
	addiu	$3,$3,24
	sw	$3,56($sp)
	lw	$3,56($sp)
	lw	$7,0($3)
	lui	$3,%hi($LC63)
	addiu	$4,$3,%lo($LC63)
	move	$5,$2
	lw	$6,24($sp)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lw	$2,24($sp)
	beq	$2,$0,$L232
	nop

	li	$2,284			# 0x11c
	j	$L233
	nop

$L232:
	li	$2,28			# 0x1c
$L233:
	lw	$3,24($sp)
	sll	$3,$3,8
	move	$4,$3
	lw	$3,28($sp)
	addu	$3,$4,$3
	addiu	$3,$3,28
	sw	$3,60($sp)
	lw	$3,60($sp)
	lw	$7,0($3)
	lui	$3,%hi($LC64)
	addiu	$4,$3,%lo($LC64)
	move	$5,$2
	lw	$6,24($sp)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lw	$2,24($sp)
	addiu	$2,$2,1
	sw	$2,24($sp)
$L225:
	lw	$2,24($sp)
	slt	$2,$2,2
	bne	$2,$0,$L235
	nop

	lw	$2,28($sp)
	addiu	$2,$2,32
	sw	$2,64($sp)
	lw	$2,64($sp)
	lw	$3,0($2)
	lui	$2,%hi($LC65)
	addiu	$4,$2,%lo($LC65)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lw	$2,28($sp)
	addiu	$2,$2,36
	sw	$2,68($sp)
	lw	$2,68($sp)
	lw	$3,0($2)
	lui	$2,%hi($LC66)
	addiu	$4,$2,%lo($LC66)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lw	$2,28($sp)
	addiu	$2,$2,40
	sw	$2,72($sp)
	lw	$2,72($sp)
	lw	$3,0($2)
	lui	$2,%hi($LC67)
	addiu	$4,$2,%lo($LC67)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lw	$2,28($sp)
	addiu	$2,$2,48
	sw	$2,76($sp)
	lw	$2,76($sp)
	lw	$3,0($2)
	lui	$2,%hi($LC68)
	addiu	$4,$2,%lo($LC68)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lw	$2,28($sp)
	addiu	$2,$2,64
	sw	$2,80($sp)
	lw	$2,80($sp)
	lw	$3,0($2)
	lui	$2,%hi($LC69)
	addiu	$4,$2,%lo($LC69)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lw	$2,28($sp)
	addiu	$2,$2,80
	sw	$2,84($sp)
	lw	$2,84($sp)
	lw	$3,0($2)
	lui	$2,%hi($LC70)
	addiu	$4,$2,%lo($LC70)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lw	$2,28($sp)
	addiu	$2,$2,84
	sw	$2,88($sp)
	lw	$2,88($sp)
	lw	$3,0($2)
	lui	$2,%hi($LC71)
	addiu	$4,$2,%lo($LC71)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lw	$2,28($sp)
	addiu	$2,$2,88
	sw	$2,92($sp)
	lw	$2,92($sp)
	lw	$3,0($2)
	lui	$2,%hi($LC72)
	addiu	$4,$2,%lo($LC72)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lw	$2,28($sp)
	addiu	$2,$2,92
	sw	$2,96($sp)
	lw	$2,96($sp)
	lw	$3,0($2)
	lui	$2,%hi($LC73)
	addiu	$4,$2,%lo($LC73)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lw	$2,28($sp)
	addiu	$2,$2,96
	sw	$2,100($sp)
	lw	$2,100($sp)
	lw	$3,0($2)
	lui	$2,%hi($LC74)
	addiu	$4,$2,%lo($LC74)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lw	$2,28($sp)
	addiu	$2,$2,100
	sw	$2,104($sp)
	lw	$2,104($sp)
	lw	$3,0($2)
	lui	$2,%hi($LC75)
	addiu	$4,$2,%lo($LC75)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lw	$2,28($sp)
	addiu	$2,$2,104
	sw	$2,108($sp)
	lw	$2,108($sp)
	lw	$3,0($2)
	lui	$2,%hi($LC76)
	addiu	$4,$2,%lo($LC76)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lw	$2,28($sp)
	addiu	$2,$2,108
	sw	$2,112($sp)
	lw	$2,112($sp)
	lw	$3,0($2)
	lui	$2,%hi($LC77)
	addiu	$4,$2,%lo($LC77)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lw	$2,28($sp)
	addiu	$2,$2,112
	sw	$2,116($sp)
	lw	$2,116($sp)
	lw	$3,0($2)
	lui	$2,%hi($LC78)
	addiu	$4,$2,%lo($LC78)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lw	$2,28($sp)
	addiu	$2,$2,128
	sw	$2,120($sp)
	lw	$2,120($sp)
	lw	$3,0($2)
	lui	$2,%hi($LC79)
	addiu	$4,$2,%lo($LC79)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lw	$2,28($sp)
	addiu	$2,$2,136
	sw	$2,124($sp)
	lw	$2,124($sp)
	lw	$3,0($2)
	lui	$2,%hi($LC80)
	addiu	$4,$2,%lo($LC80)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lw	$2,28($sp)
	addiu	$2,$2,144
	sw	$2,128($sp)
	lw	$2,128($sp)
	lw	$3,0($2)
	lui	$2,%hi($LC81)
	addiu	$4,$2,%lo($LC81)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lw	$2,28($sp)
	addiu	$2,$2,148
	sw	$2,132($sp)
	lw	$2,132($sp)
	lw	$3,0($2)
	lui	$2,%hi($LC82)
	addiu	$4,$2,%lo($LC82)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lw	$2,28($sp)
	addiu	$2,$2,152
	sw	$2,136($sp)
	lw	$2,136($sp)
	lw	$3,0($2)
	lui	$2,%hi($LC83)
	addiu	$4,$2,%lo($LC83)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lw	$2,28($sp)
	addiu	$2,$2,156
	sw	$2,140($sp)
	lw	$2,140($sp)
	lw	$3,0($2)
	lui	$2,%hi($LC84)
	addiu	$4,$2,%lo($LC84)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lw	$2,28($sp)
	addiu	$2,$2,160
	sw	$2,144($sp)
	lw	$2,144($sp)
	lw	$3,0($2)
	lui	$2,%hi($LC85)
	addiu	$4,$2,%lo($LC85)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lw	$2,28($sp)
	addiu	$2,$2,164
	sw	$2,148($sp)
	lw	$2,148($sp)
	lw	$3,0($2)
	lui	$2,%hi($LC86)
	addiu	$4,$2,%lo($LC86)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lw	$2,28($sp)
	addiu	$2,$2,168
	sw	$2,152($sp)
	lw	$2,152($sp)
	lw	$3,0($2)
	lui	$2,%hi($LC87)
	addiu	$4,$2,%lo($LC87)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lw	$2,28($sp)
	addiu	$2,$2,172
	sw	$2,156($sp)
	lw	$2,156($sp)
	lw	$3,0($2)
	lui	$2,%hi($LC88)
	addiu	$4,$2,%lo($LC88)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	sw	$0,24($sp)
	j	$L260
	nop

$L262:
	lw	$2,24($sp)
	sll	$2,$2,2
	addiu	$5,$2,176
	lw	$2,24($sp)
	sll	$2,$2,2
	move	$3,$2
	lw	$2,28($sp)
	addu	$2,$3,$2
	addiu	$2,$2,176
	sw	$2,160($sp)
	lw	$2,160($sp)
	lw	$3,0($2)
	lui	$2,%hi($LC89)
	addiu	$4,$2,%lo($LC89)
	lw	$6,24($sp)
	move	$7,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lw	$2,24($sp)
	addiu	$2,$2,1
	sw	$2,24($sp)
$L260:
	lw	$2,24($sp)
	slt	$2,$2,8
	bne	$2,$0,$L262
	nop

	lw	$2,28($sp)
	addiu	$2,$2,240
	sw	$2,164($sp)
	lw	$2,164($sp)
	lw	$3,0($2)
	lui	$2,%hi($LC90)
	addiu	$4,$2,%lo($LC90)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lw	$2,28($sp)
	addiu	$2,$2,244
	sw	$2,168($sp)
	lw	$2,168($sp)
	lw	$3,0($2)
	lui	$2,%hi($LC91)
	addiu	$4,$2,%lo($LC91)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lw	$2,28($sp)
	addiu	$2,$2,248
	sw	$2,172($sp)
	lw	$2,172($sp)
	lw	$3,0($2)
	lui	$2,%hi($LC92)
	addiu	$4,$2,%lo($LC92)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lw	$2,28($sp)
	addiu	$2,$2,252
	sw	$2,176($sp)
	lw	$2,176($sp)
	lw	$3,0($2)
	lui	$2,%hi($LC93)
	addiu	$4,$2,%lo($LC93)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lw	$2,28($sp)
	addiu	$2,$2,256
	sw	$2,180($sp)
	lw	$2,180($sp)
	lw	$3,0($2)
	lui	$2,%hi($LC94)
	addiu	$4,$2,%lo($LC94)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lw	$2,28($sp)
	addiu	$2,$2,260
	sw	$2,184($sp)
	lw	$2,184($sp)
	lw	$3,0($2)
	lui	$2,%hi($LC95)
	addiu	$4,$2,%lo($LC95)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lw	$2,28($sp)
	addiu	$2,$2,288
	sw	$2,188($sp)
	lw	$2,188($sp)
	lw	$3,0($2)
	lui	$2,%hi($LC96)
	addiu	$4,$2,%lo($LC96)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lw	$2,28($sp)
	addiu	$2,$2,292
	sw	$2,192($sp)
	lw	$2,192($sp)
	lw	$3,0($2)
	lui	$2,%hi($LC97)
	addiu	$4,$2,%lo($LC97)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	sw	$0,24($sp)
	j	$L271
	nop

$L273:
	lw	$2,24($sp)
	sll	$2,$2,2
	addiu	$5,$2,384
	lw	$2,24($sp)
	sll	$2,$2,3
	addiu	$6,$2,7
	lw	$2,24($sp)
	sll	$3,$2,3
	lw	$2,24($sp)
	sll	$2,$2,2
	move	$4,$2
	lw	$2,28($sp)
	addu	$2,$4,$2
	addiu	$2,$2,384
	sw	$2,196($sp)
	lw	$2,196($sp)
	lw	$2,0($2)
	sw	$2,16($sp)
	lui	$2,%hi($LC98)
	addiu	$4,$2,%lo($LC98)
	move	$7,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lw	$2,24($sp)
	addiu	$2,$2,1
	sw	$2,24($sp)
$L271:
	lw	$2,24($sp)
	blez	$2,$L273
	nop

	sw	$0,24($sp)
	j	$L274
	nop

$L276:
	lw	$2,24($sp)
	sll	$2,$2,2
	addiu	$5,$2,512
	lw	$2,24($sp)
	sll	$2,$2,1
	addiu	$6,$2,1
	lw	$2,24($sp)
	sll	$3,$2,1
	lw	$2,24($sp)
	sll	$2,$2,2
	move	$4,$2
	lw	$2,28($sp)
	addu	$2,$4,$2
	addiu	$2,$2,512
	sw	$2,200($sp)
	lw	$2,200($sp)
	lw	$2,0($2)
	sw	$2,16($sp)
	lui	$2,%hi($LC99)
	addiu	$4,$2,%lo($LC99)
	move	$7,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lw	$2,24($sp)
	addiu	$2,$2,1
	sw	$2,24($sp)
$L274:
	lw	$2,24($sp)
	slt	$2,$2,4
	bne	$2,$0,$L276
	nop

	sw	$0,24($sp)
	j	$L277
	nop

$L279:
	lw	$2,24($sp)
	sll	$2,$2,2
	addiu	$5,$2,640
	lw	$2,24($sp)
	sll	$2,$2,2
	addiu	$6,$2,3
	lw	$2,24($sp)
	sll	$3,$2,2
	lw	$2,24($sp)
	sll	$2,$2,2
	move	$4,$2
	lw	$2,28($sp)
	addu	$2,$4,$2
	addiu	$2,$2,640
	sw	$2,204($sp)
	lw	$2,204($sp)
	lw	$2,0($2)
	sw	$2,16($sp)
	lui	$2,%hi($LC100)
	addiu	$4,$2,%lo($LC100)
	move	$7,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lw	$2,24($sp)
	addiu	$2,$2,1
	sw	$2,24($sp)
$L277:
	lw	$2,24($sp)
	slt	$2,$2,2
	bne	$2,$0,$L279
	nop

	sw	$0,24($sp)
	j	$L280
	nop

$L282:
	lw	$2,24($sp)
	sll	$2,$2,2
	addiu	$5,$2,704
	lw	$2,24($sp)
	sll	$2,$2,2
	addiu	$6,$2,3
	lw	$2,24($sp)
	sll	$3,$2,2
	lw	$2,24($sp)
	sll	$2,$2,2
	move	$4,$2
	lw	$2,28($sp)
	addu	$2,$4,$2
	addiu	$2,$2,704
	sw	$2,208($sp)
	lw	$2,208($sp)
	lw	$2,0($2)
	sw	$2,16($sp)
	lui	$2,%hi($LC101)
	addiu	$4,$2,%lo($LC101)
	move	$7,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lw	$2,24($sp)
	addiu	$2,$2,1
	sw	$2,24($sp)
$L280:
	lw	$2,24($sp)
	slt	$2,$2,2
	bne	$2,$0,$L282
	nop

	move	$2,$0
	lw	$31,220($sp)
	addiu	$sp,$sp,224
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	qdma_dump_register_value
	.size	qdma_dump_register_value, .-qdma_dump_register_value
	.rdata
	.align	2
$LC102:
	.ascii	"Unused Tx%d DSCP Link List:\012\000"
	.align	2
$LC103:
	.ascii	"%d: DSCP Idx:%d, DSCP Ptr:%.8x, Done Bit:%d\012\000"
	.align	2
$LC104:
	.ascii	"\012Using Tx%d DSCP Set:\012\000"
	.align	2
$LC105:
	.ascii	"%d: DSCP Idx:%d, DSCP Ptr:%.8x, Done Bit:%d, PktLen:%d, "
	.ascii	"PktAddr:%.8x(%.8x), Next Idx:%d\012\000"
	.align	2
$LC106:
	.ascii	"\012Rx%d DSCP Ring: RxStartIdx:%d, RxEndIdx:%d\012\000"
	.section	.text.qdma_dump_descriptor_info,"ax",@progbits
	.align	2
	.globl	qdma_dump_descriptor_info
	.set	nomips16
	.set	nomicromips
	.ent	qdma_dump_descriptor_info
	.type	qdma_dump_descriptor_info, @function
qdma_dump_descriptor_info:
	.frame	$sp,64,$31		# vars= 16, regs= 1/0, args= 40, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-64
	sw	$31,60($sp)
	sw	$4,64($sp)
	sw	$0,40($sp)
	sw	$0,44($sp)
	li	$2,1			# 0x1
	sw	$2,48($sp)
	sw	$0,52($sp)
	sw	$0,52($sp)
	j	$L285
	nop

$L292:
	li	$2,1			# 0x1
	sw	$2,48($sp)
	lui	$2,%hi(gpQdmaPriv)
	lw	$3,%lo(gpQdmaPriv)($2)
	lw	$2,52($sp)
	addiu	$2,$2,18
	sll	$2,$2,2
	addu	$2,$3,$2
	lw	$2,0($2)
	sw	$2,40($sp)
	lui	$2,%hi($LC102)
	addiu	$4,$2,%lo($LC102)
	lw	$5,52($sp)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	j	$L286
	nop

$L287:
	lw	$2,40($sp)
	lw	$3,4($2)
	lw	$2,40($sp)
	lw	$2,0($2)
	move	$7,$2
	lw	$2,40($sp)
	lw	$2,0($2)
	lw	$2,4($2)
	ext	$2,$2,31,1
	andi	$2,$2,0x00ff
	sw	$2,16($sp)
	lui	$2,%hi($LC103)
	addiu	$4,$2,%lo($LC103)
	lw	$5,48($sp)
	move	$6,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lw	$2,40($sp)
	lw	$2,12($2)
	sw	$2,40($sp)
	lw	$2,48($sp)
	addiu	$2,$2,1
	sw	$2,48($sp)
$L286:
	lw	$2,40($sp)
	bne	$2,$0,$L287
	nop

	li	$2,1			# 0x1
	sw	$2,48($sp)
	lui	$2,%hi($LC104)
	addiu	$4,$2,%lo($LC104)
	lw	$5,52($sp)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	sw	$0,44($sp)
	j	$L288
	nop

$L291:
	lw	$2,44($sp)
	sll	$3,$2,4
	lui	$2,%hi(gpQdmaPriv)
	lw	$4,%lo(gpQdmaPriv)($2)
	lw	$2,52($sp)
	addiu	$2,$2,10
	sll	$2,$2,2
	addu	$2,$4,$2
	lw	$2,0($2)
	addu	$2,$3,$2
	sw	$2,40($sp)
	lw	$2,40($sp)
	lw	$2,12($2)
	bne	$2,$0,$L289
	nop

	lui	$2,%hi(gpQdmaPriv)
	lw	$3,%lo(gpQdmaPriv)($2)
	lw	$2,52($sp)
	addiu	$2,$2,20
	sll	$2,$2,2
	addu	$2,$3,$2
	lw	$3,0($2)
	lw	$2,40($sp)
	beq	$3,$2,$L289
	nop

	lw	$2,40($sp)
	lw	$6,4($2)
	lw	$2,40($sp)
	lw	$2,0($2)
	move	$8,$2
	lw	$2,40($sp)
	lw	$2,0($2)
	lw	$2,4($2)
	ext	$2,$2,31,1
	andi	$2,$2,0x00ff
	move	$7,$2
	lw	$2,40($sp)
	lw	$2,0($2)
	lhu	$2,6($2)
	move	$5,$2
	lw	$2,40($sp)
	lw	$2,8($2)
	move	$4,$2
	lw	$2,40($sp)
	lw	$2,0($2)
	lw	$2,8($2)
	lw	$3,40($sp)
	lw	$3,0($3)
	lw	$3,12($3)
	ext	$3,$3,0,12
	andi	$3,$3,0xffff
	sw	$7,16($sp)
	sw	$5,20($sp)
	sw	$4,24($sp)
	sw	$2,28($sp)
	sw	$3,32($sp)
	lui	$2,%hi($LC105)
	addiu	$4,$2,%lo($LC105)
	lw	$5,48($sp)
	move	$7,$8
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lw	$2,48($sp)
	addiu	$2,$2,1
	sw	$2,48($sp)
$L289:
	lw	$2,44($sp)
	andi	$3,$2,0xff
	li	$2,255			# 0xff
	bne	$3,$2,$L290
	nop

	move	$4,$0
	lui	$2,%hi(msleep)
	addiu	$2,$2,%lo(msleep)
	jalr	$2
	nop

$L290:
	lw	$2,44($sp)
	addiu	$2,$2,1
	sw	$2,44($sp)
$L288:
	lui	$2,%hi(gpQdmaPriv)
	lw	$3,%lo(gpQdmaPriv)($2)
	lw	$2,52($sp)
	addiu	$2,$2,8
	sll	$2,$2,1
	addu	$2,$3,$2
	lhu	$2,4($2)
	move	$3,$2
	lw	$2,44($sp)
	slt	$2,$2,$3
	bne	$2,$0,$L291
	nop

	lw	$2,52($sp)
	addiu	$2,$2,1
	sw	$2,52($sp)
$L285:
	lw	$2,52($sp)
	slt	$2,$2,2
	bne	$2,$0,$L292
	nop

	sw	$0,52($sp)
	j	$L293
	nop

$L297:
	li	$2,1			# 0x1
	sw	$2,48($sp)
	lui	$2,%hi(gpQdmaPriv)
	lw	$3,%lo(gpQdmaPriv)($2)
	lw	$2,52($sp)
	addiu	$2,$2,24
	sll	$2,$2,2
	addu	$2,$3,$2
	lw	$2,0($2)
	sw	$2,40($sp)
	lui	$2,%hi(gpQdmaPriv)
	lw	$3,%lo(gpQdmaPriv)($2)
	lw	$2,52($sp)
	addiu	$2,$2,24
	sll	$2,$2,2
	addu	$2,$3,$2
	lw	$2,0($2)
	lw	$6,4($2)
	lui	$2,%hi(gpQdmaPriv)
	lw	$3,%lo(gpQdmaPriv)($2)
	lw	$2,52($sp)
	addiu	$2,$2,26
	sll	$2,$2,2
	addu	$2,$3,$2
	lw	$2,0($2)
	lw	$3,4($2)
	lui	$2,%hi($LC106)
	addiu	$4,$2,%lo($LC106)
	lw	$5,52($sp)
	move	$7,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L296:
	lw	$2,40($sp)
	beq	$2,$0,$L294
	nop

	lw	$2,40($sp)
	lw	$6,4($2)
	lw	$2,40($sp)
	lw	$2,0($2)
	move	$8,$2
	lw	$2,40($sp)
	lw	$2,0($2)
	lw	$2,4($2)
	ext	$2,$2,31,1
	andi	$2,$2,0x00ff
	move	$7,$2
	lw	$2,40($sp)
	lw	$2,0($2)
	lhu	$2,6($2)
	move	$5,$2
	lw	$2,40($sp)
	lw	$2,8($2)
	move	$4,$2
	lw	$2,40($sp)
	lw	$2,0($2)
	lw	$2,8($2)
	lw	$3,40($sp)
	lw	$3,0($3)
	lw	$3,12($3)
	ext	$3,$3,0,12
	andi	$3,$3,0xffff
	sw	$7,16($sp)
	sw	$5,20($sp)
	sw	$4,24($sp)
	sw	$2,28($sp)
	sw	$3,32($sp)
	lui	$2,%hi($LC105)
	addiu	$4,$2,%lo($LC105)
	lw	$5,48($sp)
	move	$7,$8
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lw	$2,40($sp)
	lw	$2,12($2)
	sw	$2,40($sp)
	lw	$2,48($sp)
	addiu	$2,$2,1
	sw	$2,48($sp)
$L294:
	lw	$2,40($sp)
	beq	$2,$0,$L295
	nop

	lui	$2,%hi(gpQdmaPriv)
	lw	$3,%lo(gpQdmaPriv)($2)
	lw	$2,52($sp)
	addiu	$2,$2,24
	sll	$2,$2,2
	addu	$2,$3,$2
	lw	$3,0($2)
	lw	$2,40($sp)
	bne	$3,$2,$L296
	nop

$L295:
	lw	$2,52($sp)
	addiu	$2,$2,1
	sw	$2,52($sp)
$L293:
	lw	$2,52($sp)
	slt	$2,$2,2
	bne	$2,$0,$L297
	nop

	move	$2,$0
	lw	$31,60($sp)
	addiu	$sp,$sp,64
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	qdma_dump_descriptor_info
	.size	qdma_dump_descriptor_info, .-qdma_dump_descriptor_info
	.rdata
	.align	2
$LC107:
	.ascii	"IRQ Queue:%.4x,     Content:%.8x\012\000"
	.section	.text.qdma_dump_irq_info,"ax",@progbits
	.align	2
	.globl	qdma_dump_irq_info
	.set	nomips16
	.set	nomicromips
	.ent	qdma_dump_irq_info
	.type	qdma_dump_irq_info, @function
qdma_dump_irq_info:
	.frame	$sp,40,$31		# vars= 16, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-40
	sw	$31,36($sp)
	sw	$4,40($sp)
	sw	$0,16($sp)
	sw	$0,20($sp)
	sw	$0,24($sp)
	sw	$0,16($sp)
	j	$L300
	nop

$L302:
	lw	$2,16($sp)
	sll	$3,$2,2
	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lw	$2,56($2)
	addu	$2,$3,$2
	sw	$2,20($sp)
	lw	$2,20($sp)
	lw	$2,0($2)
	sw	$2,24($sp)
	lui	$2,%hi($LC107)
	addiu	$4,$2,%lo($LC107)
	lw	$5,16($sp)
	lw	$6,24($sp)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lw	$2,16($sp)
	andi	$3,$2,0xff
	li	$2,255			# 0xff
	bne	$3,$2,$L301
	nop

	move	$4,$0
	lui	$2,%hi(msleep)
	addiu	$2,$2,%lo(msleep)
	jalr	$2
	nop

$L301:
	lw	$2,16($sp)
	addiu	$2,$2,1
	sw	$2,16($sp)
$L300:
	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lhu	$2,30($2)
	move	$3,$2
	lw	$2,16($sp)
	slt	$2,$2,$3
	bne	$2,$0,$L302
	nop

	move	$2,$0
	lw	$31,36($sp)
	addiu	$sp,$sp,40
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	qdma_dump_irq_info
	.size	qdma_dump_irq_info, .-qdma_dump_irq_info
	.rdata
	.align	2
$LC108:
	.ascii	"Hardware Forwarding DSCP Link List:\012\000"
	.align	2
$LC109:
	.ascii	"HWFWD\000"
	.align	2
$LC110:
	.ascii	"CPU\000"
	.align	2
$LC111:
	.ascii	"%.4d: DSCP:%.8x(%.8x), ctx:%s, ctx_idx:%d, MsgAddr:%.8x,"
	.ascii	" PktLen:%d, PktAddr:%.8x(%.8x)\012\000"
	.align	2
$LC112:
	.ascii	"      Msg Content:%.8x, %.8x\012\000"
	.section	.text.qdma_dump_hwfwd_info,"ax",@progbits
	.align	2
	.globl	qdma_dump_hwfwd_info
	.set	nomips16
	.set	nomicromips
	.ent	qdma_dump_hwfwd_info
	.type	qdma_dump_hwfwd_info, @function
qdma_dump_hwfwd_info:
	.frame	$sp,72,$31		# vars= 24, regs= 1/0, args= 40, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-72
	sw	$31,68($sp)
	sw	$4,72($sp)
	sw	$0,40($sp)
	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lw	$2,0($2)
	sw	$2,44($sp)
	sw	$0,48($sp)
	sw	$0,52($sp)
	sw	$0,56($sp)
	lui	$2,%hi($LC108)
	addiu	$4,$2,%lo($LC108)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	sw	$0,40($sp)
	j	$L305
	nop

$L310:
	lw	$2,40($sp)
	sll	$3,$2,4
	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lw	$2,60($2)
	addu	$2,$3,$2
	sw	$2,48($sp)
	lw	$2,48($sp)
	addiu	$2,$2,8
	sw	$2,52($sp)
	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lw	$2,68($2)
	srl	$3,$2,2
	lw	$2,40($sp)
	mul	$2,$3,$2
	sll	$3,$2,2
	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lw	$2,64($2)
	addu	$2,$3,$2
	sw	$2,56($sp)
	lw	$6,48($sp)
	lw	$2,44($sp)
	addiu	$2,$2,32
	sw	$2,60($sp)
	lw	$2,60($sp)
	lw	$3,0($2)
	lw	$2,40($sp)
	sll	$2,$2,4
	addu	$7,$3,$2
	lw	$2,48($sp)
	lw	$3,4($2)
	li	$2,-2147483648			# 0xffffffff80000000
	and	$2,$3,$2
	beq	$2,$0,$L307
	nop

	lui	$2,%hi($LC109)
	addiu	$2,$2,%lo($LC109)
	j	$L308
	nop

$L307:
	lui	$2,%hi($LC110)
	addiu	$2,$2,%lo($LC110)
$L308:
	lw	$3,48($sp)
	lw	$3,4($3)
	ext	$3,$3,16,12
	andi	$3,$3,0xffff
	move	$9,$3
	lw	$5,52($sp)
	lw	$3,48($sp)
	lhu	$3,6($3)
	move	$8,$3
	lw	$4,56($sp)
	lw	$3,48($sp)
	lw	$3,0($3)
	sw	$2,16($sp)
	sw	$9,20($sp)
	sw	$5,24($sp)
	sw	$8,28($sp)
	sw	$4,32($sp)
	sw	$3,36($sp)
	lui	$2,%hi($LC111)
	addiu	$4,$2,%lo($LC111)
	lw	$5,40($sp)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lw	$2,52($sp)
	lw	$5,0($2)
	lw	$2,52($sp)
	addiu	$2,$2,4
	lw	$3,0($2)
	lui	$2,%hi($LC112)
	addiu	$4,$2,%lo($LC112)
	move	$6,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lw	$2,40($sp)
	andi	$3,$2,0xff
	li	$2,255			# 0xff
	bne	$3,$2,$L309
	nop

	move	$4,$0
	lui	$2,%hi(msleep)
	addiu	$2,$2,%lo(msleep)
	jalr	$2
	nop

$L309:
	lw	$2,40($sp)
	addiu	$2,$2,1
	sw	$2,40($sp)
$L305:
	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lhu	$2,28($2)
	move	$3,$2
	lw	$2,40($sp)
	slt	$2,$2,$3
	bne	$2,$0,$L310
	nop

	move	$2,$0
	lw	$31,68($sp)
	addiu	$sp,$sp,72
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	qdma_dump_hwfwd_info
	.size	qdma_dump_hwfwd_info, .-qdma_dump_hwfwd_info
	.section	.text.qdma_set_dbg_level,"ax",@progbits
	.align	2
	.globl	qdma_set_dbg_level
	.set	nomips16
	.set	nomicromips
	.ent	qdma_set_dbg_level
	.type	qdma_set_dbg_level, @function
qdma_set_dbg_level:
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	sw	$4,0($sp)
	move	$2,$0
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	qdma_set_dbg_level
	.size	qdma_set_dbg_level, .-qdma_set_dbg_level
	.rdata
	.align	2
$LC113:
	.ascii	"dma busy duration force setting to %d seconds.\000"
	.section	.text.qdma_dump_dma_busy,"ax",@progbits
	.align	2
	.globl	qdma_dump_dma_busy
	.set	nomips16
	.set	nomicromips
	.ent	qdma_dump_dma_busy
	.type	qdma_dump_dma_busy, @function
qdma_dump_dma_busy:
	.frame	$sp,32,$31		# vars= 8, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-32
	sw	$31,28($sp)
	sw	$4,32($sp)
	lw	$2,32($sp)
	lw	$2,12($2)
	sw	$2,16($sp)
	lw	$2,16($sp)
	sltu	$2,$2,61
	bne	$2,$0,$L315
	nop

	lui	$2,%hi($LC113)
	addiu	$4,$2,%lo($LC113)
	li	$5,60			# 0x3c
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	li	$2,60			# 0x3c
	sw	$2,16($sp)
$L315:
	lw	$2,16($sp)
	sll	$2,$2,2
	sll	$3,$2,2
	addu	$2,$2,$3
	sll	$3,$2,2
	addu	$2,$2,$3
	move	$3,$2
	lui	$2,%hi(dma_busy_round_cnt)
	sw	$3,%lo(dma_busy_round_cnt)($2)
	lui	$2,%hi(tx_dma_busy_enable_cnt)
	sw	$0,%lo(tx_dma_busy_enable_cnt)($2)
	lui	$2,%hi(rx_dma_busy_enable_cnt)
	sw	$0,%lo(rx_dma_busy_enable_cnt)($2)
	lui	$2,%hi(dma_busy_timer)
	addiu	$4,$2,%lo(dma_busy_timer)
	move	$5,$0
	move	$6,$0
	lui	$2,%hi(init_timer_key)
	addiu	$2,$2,%lo(init_timer_key)
	jalr	$2
	nop

	lui	$2,%hi(dma_busy_expires)
	lw	$2,%lo(dma_busy_expires)($2)
	move	$3,$2
	lui	$2,%hi(dma_busy_timer)
	addiu	$2,$2,%lo(dma_busy_timer)
	sw	$3,20($2)
	lui	$2,%hi(dma_busy_timer)
	addiu	$2,$2,%lo(dma_busy_timer)
	lui	$3,%hi(qdma_dma_busy_timer)
	addiu	$3,$3,%lo(qdma_dma_busy_timer)
	sw	$3,16($2)
	lui	$2,%hi(dma_busy_timer)
	addiu	$2,$2,%lo(dma_busy_timer)
	lw	$2,20($2)
	sll	$2,$2,2
	sll	$3,$2,2
	addu	$2,$2,$3
	sll	$3,$2,2
	addu	$3,$2,$3
	li	$2,274857984			# 0x10620000
	ori	$2,$2,0x4dd3
	multu	$3,$2
	mfhi	$2
	srl	$3,$2,6
	lui	$2,%hi(jiffies)
	lw	$2,%lo(jiffies)($2)
	addu	$3,$3,$2
	lui	$2,%hi(dma_busy_timer)
	addiu	$4,$2,%lo(dma_busy_timer)
	move	$5,$3
	lui	$2,%hi(mod_timer)
	addiu	$2,$2,%lo(mod_timer)
	jalr	$2
	nop

	move	$2,$0
	lw	$31,28($sp)
	addiu	$sp,$sp,32
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	qdma_dump_dma_busy
	.size	qdma_dump_dma_busy, .-qdma_dump_dma_busy
	.rdata
	.align	2
$LC114:
	.ascii	"register offset is %u, which should be an integral multi"
	.ascii	"ple of 4.\000"
	.align	2
$LC115:
	.ascii	"register polling duration force setting to %d seconds.\000"
	.section	.text.qdma_dump_reg_polling,"ax",@progbits
	.align	2
	.globl	qdma_dump_reg_polling
	.set	nomips16
	.set	nomicromips
	.ent	qdma_dump_reg_polling
	.type	qdma_dump_reg_polling, @function
qdma_dump_reg_polling:
	.frame	$sp,32,$31		# vars= 8, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-32
	sw	$31,28($sp)
	sw	$4,32($sp)
	lw	$2,32($sp)
	lw	$2,16($2)
	sw	$2,20($sp)
	lw	$2,32($sp)
	lw	$2,20($2)
	sw	$2,16($sp)
	lw	$2,20($sp)
	andi	$2,$2,0x3
	beq	$2,$0,$L318
	nop

	lui	$2,%hi($LC114)
	addiu	$4,$2,%lo($LC114)
	lw	$5,20($sp)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	li	$2,-22			# 0xffffffffffffffea
	j	$L319
	nop

$L318:
	lw	$2,16($sp)
	sltu	$2,$2,61
	bne	$2,$0,$L320
	nop

	lui	$2,%hi($LC115)
	addiu	$4,$2,%lo($LC115)
	li	$5,60			# 0x3c
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	li	$2,60			# 0x3c
	sw	$2,16($sp)
$L320:
	lw	$2,16($sp)
	sll	$2,$2,2
	sll	$3,$2,2
	addu	$2,$2,$3
	sll	$3,$2,2
	addu	$2,$2,$3
	move	$3,$2
	lui	$2,%hi(reg_polling_round_cnt)
	sw	$3,%lo(reg_polling_round_cnt)($2)
	lw	$3,20($sp)
	lui	$2,%hi(reg_polling_offset)
	sw	$3,%lo(reg_polling_offset)($2)
	lui	$2,%hi(reg_polling_timer)
	addiu	$4,$2,%lo(reg_polling_timer)
	move	$5,$0
	move	$6,$0
	lui	$2,%hi(init_timer_key)
	addiu	$2,$2,%lo(init_timer_key)
	jalr	$2
	nop

	lui	$2,%hi(reg_polling_expires)
	lw	$2,%lo(reg_polling_expires)($2)
	move	$3,$2
	lui	$2,%hi(reg_polling_timer)
	addiu	$2,$2,%lo(reg_polling_timer)
	sw	$3,20($2)
	lui	$2,%hi(reg_polling_timer)
	addiu	$2,$2,%lo(reg_polling_timer)
	lui	$3,%hi(qdma_reg_polling_timer)
	addiu	$3,$3,%lo(qdma_reg_polling_timer)
	sw	$3,16($2)
	lui	$2,%hi(reg_polling_timer)
	addiu	$2,$2,%lo(reg_polling_timer)
	lw	$2,20($2)
	sll	$2,$2,2
	sll	$3,$2,2
	addu	$2,$2,$3
	sll	$3,$2,2
	addu	$3,$2,$3
	li	$2,274857984			# 0x10620000
	ori	$2,$2,0x4dd3
	multu	$3,$2
	mfhi	$2
	srl	$3,$2,6
	lui	$2,%hi(jiffies)
	lw	$2,%lo(jiffies)($2)
	addu	$3,$3,$2
	lui	$2,%hi(reg_polling_timer)
	addiu	$4,$2,%lo(reg_polling_timer)
	move	$5,$3
	lui	$2,%hi(mod_timer)
	addiu	$2,$2,%lo(mod_timer)
	jalr	$2
	nop

	move	$2,$0
$L319:
	lw	$31,28($sp)
	addiu	$sp,$sp,32
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	qdma_dump_reg_polling
	.size	qdma_dump_reg_polling, .-qdma_dump_reg_polling
	.rdata
	.align	2
$LC116:
	.ascii	"isForceRX1: %d\012\000"
	.section	.text.qdma_set_force_receive_rx_ring1,"ax",@progbits
	.align	2
	.globl	qdma_set_force_receive_rx_ring1
	.set	nomips16
	.set	nomicromips
	.ent	qdma_set_force_receive_rx_ring1
	.type	qdma_set_force_receive_rx_ring1, @function
qdma_set_force_receive_rx_ring1:
	.frame	$sp,32,$31		# vars= 8, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-32
	sw	$31,28($sp)
	sw	$4,32($sp)
	lw	$2,32($sp)
	lw	$2,24($2)
	sw	$2,16($sp)
	lw	$2,16($sp)
	sltu	$2,$0,$2
	andi	$2,$2,0x00ff
	move	$3,$2
	lui	$2,%hi(isForceRX1)
	sw	$3,%lo(isForceRX1)($2)
	lui	$2,%hi(isForceRX1)
	lw	$3,%lo(isForceRX1)($2)
	lui	$2,%hi($LC116)
	addiu	$4,$2,%lo($LC116)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	move	$2,$0
	lw	$31,28($sp)
	addiu	$sp,$sp,32
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	qdma_set_force_receive_rx_ring1
	.size	qdma_set_force_receive_rx_ring1, .-qdma_set_force_receive_rx_ring1
	.section	.text.qdma_set_tx_drop_en,"ax",@progbits
	.align	2
	.globl	qdma_set_tx_drop_en
	.set	nomips16
	.set	nomicromips
	.ent	qdma_set_tx_drop_en
	.type	qdma_set_tx_drop_en, @function
qdma_set_tx_drop_en:
	.frame	$sp,40,$31		# vars= 40, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-40
	sw	$4,40($sp)
	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lw	$2,0($2)
	sw	$2,0($sp)
	lw	$2,40($sp)
	lw	$2,8($2)
	sw	$2,4($sp)
	lw	$2,4($sp)
	bne	$2,$0,$L324
	nop

	lw	$2,0($sp)
	addiu	$2,$2,160
	sw	$2,16($sp)
	lw	$2,16($sp)
	lw	$2,0($2)
	sw	$2,8($sp)
	lw	$2,0($sp)
	addiu	$3,$2,160
	lw	$2,8($sp)
	ext	$2,$2,0,31
	sw	$3,20($sp)
	sw	$2,24($sp)
	lw	$2,20($sp)
	lw	$3,24($sp)
	sw	$3,0($2)
	j	$L326
	nop

$L324:
	lw	$2,0($sp)
	addiu	$2,$2,160
	sw	$2,28($sp)
	lw	$2,28($sp)
	lw	$2,0($2)
	sw	$2,12($sp)
	lw	$2,0($sp)
	addiu	$3,$2,160
	lw	$4,12($sp)
	li	$2,-2147483648			# 0xffffffff80000000
	or	$2,$4,$2
	sw	$3,32($sp)
	sw	$2,36($sp)
	lw	$2,32($sp)
	lw	$3,36($sp)
	sw	$3,0($2)
$L326:
	move	$2,$0
	addiu	$sp,$sp,40
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	qdma_set_tx_drop_en
	.size	qdma_set_tx_drop_en, .-qdma_set_tx_drop_en
	.section	.text.qdma_get_tx_drop_en,"ax",@progbits
	.align	2
	.globl	qdma_get_tx_drop_en
	.set	nomips16
	.set	nomicromips
	.ent	qdma_get_tx_drop_en
	.type	qdma_get_tx_drop_en, @function
qdma_get_tx_drop_en:
	.frame	$sp,8,$31		# vars= 8, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-8
	sw	$4,8($sp)
	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lw	$2,0($2)
	sw	$2,0($sp)
	lw	$2,0($sp)
	addiu	$2,$2,160
	sw	$2,4($sp)
	lw	$2,4($sp)
	lw	$2,0($2)
	bgez	$2,$L331
	nop

	lw	$2,8($sp)
	li	$3,1			# 0x1
	sw	$3,8($2)
	j	$L332
	nop

$L331:
	lw	$2,8($sp)
	sw	$0,8($2)
$L332:
	lw	$2,8($sp)
	lw	$2,8($2)
	addiu	$sp,$sp,8
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	qdma_get_tx_drop_en
	.size	qdma_get_tx_drop_en, .-qdma_get_tx_drop_en
	.section	.text.qdma_set_tx_ratemeter,"ax",@progbits
	.align	2
	.globl	qdma_set_tx_ratemeter
	.set	nomips16
	.set	nomicromips
	.ent	qdma_set_tx_ratemeter
	.type	qdma_set_tx_ratemeter, @function
qdma_set_tx_ratemeter:
	.frame	$sp,40,$31		# vars= 40, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-40
	sw	$4,40($sp)
	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lw	$2,0($2)
	sw	$2,0($sp)
	lw	$2,40($sp)
	lw	$2,8($2)
	sw	$2,4($sp)
	lw	$2,4($sp)
	lw	$2,4($2)
	sltu	$2,$2,4
	bne	$2,$0,$L335
	nop

	li	$2,-22			# 0xffffffffffffffea
	j	$L336
	nop

$L335:
	lw	$2,0($sp)
	addiu	$2,$2,148
	sw	$2,16($sp)
	lw	$2,16($sp)
	lw	$2,0($2)
	sw	$2,8($sp)
	lw	$2,0($sp)
	addiu	$3,$2,148
	lw	$4,8($sp)
	li	$2,-262144			# 0xfffffffffffc0000
	ori	$2,$2,0xffff
	and	$4,$4,$2
	lw	$2,4($sp)
	lw	$2,4($2)
	sll	$5,$2,16
	li	$2,196608			# 0x30000
	and	$2,$5,$2
	or	$2,$4,$2
	sw	$3,20($sp)
	sw	$2,24($sp)
	lw	$2,20($sp)
	lw	$3,24($sp)
	sw	$3,0($2)
	lw	$2,0($sp)
	addiu	$2,$2,148
	sw	$2,28($sp)
	lw	$2,28($sp)
	lw	$2,0($2)
	sw	$2,12($sp)
	lw	$2,0($sp)
	addiu	$3,$2,148
	lw	$4,12($sp)
	li	$2,-65536			# 0xffffffffffff0000
	and	$2,$4,$2
	lw	$4,4($sp)
	lhu	$4,0($4)
	or	$2,$2,$4
	sw	$3,32($sp)
	sw	$2,36($sp)
	lw	$2,32($sp)
	lw	$3,36($sp)
	sw	$3,0($2)
	move	$2,$0
$L336:
	addiu	$sp,$sp,40
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	qdma_set_tx_ratemeter
	.size	qdma_set_tx_ratemeter, .-qdma_set_tx_ratemeter
	.section	.text.qdma_get_tx_ratemeter,"ax",@progbits
	.align	2
	.globl	qdma_get_tx_ratemeter
	.set	nomips16
	.set	nomicromips
	.ent	qdma_get_tx_ratemeter
	.type	qdma_get_tx_ratemeter, @function
qdma_get_tx_ratemeter:
	.frame	$sp,16,$31		# vars= 16, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-16
	sw	$4,16($sp)
	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lw	$2,0($2)
	sw	$2,0($sp)
	lw	$2,16($sp)
	lw	$2,8($2)
	sw	$2,4($sp)
	lw	$2,0($sp)
	addiu	$2,$2,148
	sw	$2,8($sp)
	lw	$2,8($sp)
	lw	$3,0($2)
	li	$2,196608			# 0x30000
	and	$2,$3,$2
	srl	$3,$2,16
	lw	$2,4($sp)
	sw	$3,4($2)
	lw	$2,0($sp)
	addiu	$2,$2,148
	sw	$2,12($sp)
	lw	$2,12($sp)
	lw	$2,0($2)
	andi	$3,$2,0xffff
	lw	$2,4($sp)
	sh	$3,0($2)
	move	$2,$0
	addiu	$sp,$sp,16
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	qdma_get_tx_ratemeter
	.size	qdma_get_tx_ratemeter, .-qdma_get_tx_ratemeter
	.section	.text.qdma_get_tx_channel_datarate,"ax",@progbits
	.align	2
	.globl	qdma_get_tx_channel_datarate
	.set	nomips16
	.set	nomicromips
	.ent	qdma_get_tx_channel_datarate
	.type	qdma_get_tx_channel_datarate, @function
qdma_get_tx_channel_datarate:
	.frame	$sp,48,$31		# vars= 24, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-48
	sw	$31,44($sp)
	sw	$4,48($sp)
	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lw	$2,0($2)
	sw	$2,20($sp)
	sw	$0,16($sp)
	lw	$2,48($sp)
	lw	$2,8($2)
	sw	$2,24($sp)
	lw	$2,24($sp)
	sra	$2,$2,1
	sll	$2,$2,2
	move	$3,$2
	lw	$2,20($sp)
	addu	$2,$3,$2
	addiu	$2,$2,512
	sw	$2,28($sp)
	lw	$2,28($sp)
	lw	$2,0($2)
	lw	$3,24($sp)
	andi	$3,$3,0x1
	sll	$3,$3,4
	li	$4,65535			# 0xffff
	sll	$3,$4,$3
	and	$3,$2,$3
	lw	$2,24($sp)
	andi	$2,$2,0x1
	sll	$2,$2,4
	srl	$2,$3,$2
	sw	$2,16($sp)
	lw	$2,48($sp)
	addiu	$3,$sp,32
	sw	$3,8($2)
	lw	$4,48($sp)
	lui	$2,%hi(qdma_get_tx_ratemeter)
	addiu	$2,$2,%lo(qdma_get_tx_ratemeter)
	jalr	$2
	nop

	lhu	$2,32($sp)
	beq	$2,$0,$L345
	nop

	lw	$2,16($sp)
	sll	$3,$2,7
	li	$2,983040			# 0xf0000
	ori	$2,$2,0x4240
	mul	$2,$3,$2
	lw	$3,36($sp)
	sll	$2,$2,$3
	lhu	$3,32($sp)
	div	$0,$2,$3
	mfhi	$3
	mflo	$2
	sw	$2,16($sp)
	j	$L346
	nop

$L345:
	li	$2,-1			# 0xffffffffffffffff
	sw	$2,16($sp)
$L346:
	lw	$2,16($sp)
	lw	$31,44($sp)
	addiu	$sp,$sp,48
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	qdma_get_tx_channel_datarate
	.size	qdma_get_tx_channel_datarate, .-qdma_get_tx_channel_datarate
	.section	.text.qdma_enable_tx_ratelimit,"ax",@progbits
	.align	2
	.globl	qdma_enable_tx_ratelimit
	.set	nomips16
	.set	nomicromips
	.ent	qdma_enable_tx_ratelimit
	.type	qdma_enable_tx_ratelimit, @function
qdma_enable_tx_ratelimit:
	.frame	$sp,24,$31		# vars= 24, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-24
	sw	$4,24($sp)
	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lw	$2,0($2)
	sw	$2,0($sp)
	lw	$2,0($sp)
	addiu	$2,$2,152
	sw	$2,8($sp)
	lw	$2,8($sp)
	lw	$2,0($2)
	sw	$2,4($sp)
	lw	$2,0($sp)
	addiu	$3,$2,152
	lw	$4,4($sp)
	li	$2,-2147483648			# 0xffffffff80000000
	or	$2,$4,$2
	sw	$3,12($sp)
	sw	$2,16($sp)
	lw	$2,12($sp)
	lw	$3,16($sp)
	sw	$3,0($2)
	move	$2,$0
	addiu	$sp,$sp,24
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	qdma_enable_tx_ratelimit
	.size	qdma_enable_tx_ratelimit, .-qdma_enable_tx_ratelimit
	.section	.text.qdma_read_vip_info,"ax",@progbits
	.align	2
	.globl	qdma_read_vip_info
	.set	nomips16
	.set	nomicromips
	.ent	qdma_read_vip_info
	.type	qdma_read_vip_info, @function
qdma_read_vip_info:
	.frame	$sp,48,$31		# vars= 24, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-48
	sw	$31,44($sp)
	lui	$2,%hi(qdma_vip_num)
	sb	$0,%lo(qdma_vip_num)($2)
	lui	$2,%hi(qdma_vip_info)
	addiu	$4,$2,%lo(qdma_vip_info)
	move	$5,$0
	li	$6,192			# 0xc0
	lui	$2,%hi(memset)
	addiu	$2,$2,%lo(memset)
	jalr	$2
	nop

	sw	$0,16($sp)
	j	$L352
	nop

$L360:
	lw	$2,16($sp)
	sll	$2,$2,3
	move	$3,$2
	li	$2,-1078657024			# 0xffffffffbfb50000
	ori	$2,$2,0x300
	addu	$2,$3,$2
	sw	$2,28($sp)
	lw	$2,28($sp)
	lw	$2,0($2)
	sw	$2,20($sp)
	lw	$2,20($sp)
	andi	$2,$2,0x1
	bne	$2,$0,$L354
	nop

	j	$L355
	nop

$L354:
	lw	$2,20($sp)
	srl	$2,$2,1
	andi	$2,$2,0x7
	sltu	$2,$2,3
	beq	$2,$0,$L356
	nop

	j	$L355
	nop

$L356:
	lui	$2,%hi(qdma_vip_num)
	lbu	$2,%lo(qdma_vip_num)($2)
	move	$6,$2
	lw	$2,20($sp)
	srl	$2,$2,1
	andi	$3,$2,0x7
	li	$2,3			# 0x3
	bne	$3,$2,$L357
	nop

	li	$4,6			# 0x6
	j	$L358
	nop

$L357:
	li	$4,17			# 0x11
$L358:
	lui	$5,%hi(qdma_vip_info)
	move	$3,$6
	sll	$2,$3,1
	move	$3,$2
	sll	$2,$3,2
	subu	$2,$2,$3
	addiu	$3,$5,%lo(qdma_vip_info)
	addu	$2,$2,$3
	sb	$4,5($2)
	lui	$2,%hi(qdma_vip_num)
	lbu	$2,%lo(qdma_vip_num)($2)
	move	$3,$2
	lw	$2,20($sp)
	srl	$2,$2,5
	andi	$2,$2,0x00ff
	andi	$2,$2,0x1
	andi	$4,$2,0x00ff
	lui	$5,%hi(qdma_vip_info)
	sll	$2,$3,1
	move	$3,$2
	sll	$2,$3,2
	subu	$2,$2,$3
	addiu	$3,$5,%lo(qdma_vip_info)
	addu	$3,$2,$3
	lbu	$2,4($3)
	ins	$2,$4,7,1
	sb	$2,4($3)
	lui	$2,%hi(qdma_vip_num)
	lbu	$2,%lo(qdma_vip_num)($2)
	move	$3,$2
	lw	$2,20($sp)
	srl	$2,$2,4
	andi	$2,$2,0x00ff
	andi	$2,$2,0x1
	andi	$4,$2,0x00ff
	lui	$5,%hi(qdma_vip_info)
	sll	$2,$3,1
	move	$3,$2
	sll	$2,$3,2
	subu	$2,$2,$3
	addiu	$3,$5,%lo(qdma_vip_info)
	addu	$3,$2,$3
	lbu	$2,4($3)
	ins	$2,$4,6,1
	sb	$2,4($3)
	lui	$2,%hi(qdma_vip_num)
	lbu	$2,%lo(qdma_vip_num)($2)
	move	$3,$2
	lw	$2,20($sp)
	srl	$2,$2,6
	andi	$2,$2,0x00ff
	andi	$2,$2,0x1
	andi	$4,$2,0x00ff
	lui	$5,%hi(qdma_vip_info)
	sll	$2,$3,1
	move	$3,$2
	sll	$2,$3,2
	subu	$2,$2,$3
	addiu	$3,$5,%lo(qdma_vip_info)
	addu	$3,$2,$3
	lbu	$2,4($3)
	ins	$2,$4,5,1
	sb	$2,4($3)
	lw	$2,16($sp)
	sll	$2,$2,3
	move	$3,$2
	li	$2,-1078657024			# 0xffffffffbfb50000
	ori	$2,$2,0x304
	addu	$2,$3,$2
	sw	$2,32($sp)
	lw	$2,32($sp)
	lw	$2,0($2)
	sw	$2,24($sp)
	lui	$2,%hi(qdma_vip_num)
	lbu	$2,%lo(qdma_vip_num)($2)
	move	$3,$2
	lw	$2,24($sp)
	srl	$2,$2,16
	andi	$4,$2,0xffff
	lui	$5,%hi(qdma_vip_info)
	sll	$2,$3,1
	move	$3,$2
	sll	$2,$3,2
	subu	$2,$2,$3
	addiu	$3,$5,%lo(qdma_vip_info)
	addu	$2,$2,$3
	sh	$4,0($2)
	lui	$2,%hi(qdma_vip_num)
	lbu	$2,%lo(qdma_vip_num)($2)
	move	$3,$2
	lw	$2,24($sp)
	andi	$4,$2,0xffff
	lui	$5,%hi(qdma_vip_info)
	sll	$2,$3,1
	move	$3,$2
	sll	$2,$3,2
	subu	$2,$2,$3
	addiu	$3,$5,%lo(qdma_vip_info)
	addu	$2,$2,$3
	sh	$4,2($2)
	lui	$2,%hi(qdma_vip_num)
	lbu	$2,%lo(qdma_vip_num)($2)
	addiu	$2,$2,1
	andi	$3,$2,0x00ff
	lui	$2,%hi(qdma_vip_num)
	sb	$3,%lo(qdma_vip_num)($2)
$L355:
	lw	$2,16($sp)
	addiu	$2,$2,1
	sw	$2,16($sp)
$L352:
	lw	$2,16($sp)
	slt	$2,$2,32
	bne	$2,$0,$L360
	nop

	move	$2,$0
	lw	$31,44($sp)
	addiu	$sp,$sp,48
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	qdma_read_vip_info
	.size	qdma_read_vip_info, .-qdma_read_vip_info
	.section	.text.qdma_set_tx_ratelimit_cfg,"ax",@progbits
	.align	2
	.globl	qdma_set_tx_ratelimit_cfg
	.set	nomips16
	.set	nomicromips
	.ent	qdma_set_tx_ratelimit_cfg
	.type	qdma_set_tx_ratelimit_cfg, @function
qdma_set_tx_ratelimit_cfg:
	.frame	$sp,88,$31		# vars= 64, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-88
	sw	$31,84($sp)
	sw	$4,88($sp)
	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lw	$2,0($2)
	sw	$2,20($sp)
	sw	$0,24($sp)
	sw	$0,16($sp)
	sw	$0,28($sp)
	sw	$0,32($sp)
	lw	$2,88($sp)
	lw	$2,8($2)
	sw	$2,36($sp)
	lw	$2,36($sp)
	lw	$2,4($2)
	sltu	$2,$2,16
	bne	$2,$0,$L363
	nop

	li	$2,-22			# 0xffffffffffffffea
	j	$L364
	nop

$L363:
	lw	$2,36($sp)
	lhu	$2,0($2)
	beq	$2,$0,$L365
	nop

	lw	$2,36($sp)
	lhu	$2,0($2)
	sltu	$2,$2,66
	bne	$2,$0,$L366
	nop

$L365:
	li	$2,-22			# 0xffffffffffffffea
	j	$L364
	nop

$L366:
	lw	$2,20($sp)
	addiu	$2,$2,152
	sw	$2,48($sp)
	lw	$2,48($sp)
	lw	$2,0($2)
	andi	$2,$2,0xffff
	sw	$2,28($sp)
	lw	$2,28($sp)
	bne	$2,$0,$L368
	nop

	sw	$0,16($sp)
	j	$L369
	nop

$L368:
	li	$3,8000			# 0x1f40
	lw	$2,28($sp)
	div	$0,$3,$2
	mfhi	$3
	mflo	$2
	sw	$2,16($sp)
$L369:
	lw	$2,36($sp)
	lhu	$2,0($2)
	move	$3,$2
	lw	$2,16($sp)
	beq	$3,$2,$L370
	nop

	lui	$2,%hi(qdmaGetLimitRateMax)
	addiu	$2,$2,%lo(qdmaGetLimitRateMax)
	jalr	$2
	nop

	sw	$2,24($sp)
	lw	$2,24($sp)
	bgez	$2,$L371
	nop

	li	$2,-22			# 0xffffffffffffffea
	j	$L364
	nop

$L371:
	lw	$3,24($sp)
	lw	$2,16($sp)
	mul	$2,$3,$2
	lw	$3,36($sp)
	lhu	$3,0($3)
	div	$0,$2,$3
	mfhi	$3
	mflo	$2
	sw	$2,32($sp)
	lw	$3,32($sp)
	li	$2,65536			# 0x10000
	slt	$2,$3,$2
	bne	$2,$0,$L372
	nop

	li	$2,-22			# 0xffffffffffffffea
	j	$L364
	nop

$L372:
	lw	$2,16($sp)
	andi	$2,$2,0xffff
	move	$3,$2
	lw	$2,36($sp)
	lhu	$2,0($2)
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(qdmaUpdateAllTxRateLimitValue)
	addiu	$2,$2,%lo(qdmaUpdateAllTxRateLimitValue)
	jalr	$2
	nop

$L370:
	lw	$2,20($sp)
	addiu	$2,$2,152
	sw	$2,52($sp)
	lw	$2,52($sp)
	lw	$2,0($2)
	sw	$2,40($sp)
	lw	$2,20($sp)
	addiu	$3,$2,152
	lw	$4,40($sp)
	li	$2,-1048576			# 0xfffffffffff00000
	ori	$2,$2,0xffff
	and	$4,$4,$2
	lw	$2,36($sp)
	lw	$2,4($2)
	sll	$5,$2,16
	li	$2,983040			# 0xf0000
	and	$2,$5,$2
	or	$2,$4,$2
	sw	$3,56($sp)
	sw	$2,60($sp)
	lw	$2,56($sp)
	lw	$3,60($sp)
	sw	$3,0($2)
	lw	$2,36($sp)
	lhu	$2,0($2)
	beq	$2,$0,$L374
	nop

	lw	$2,20($sp)
	addiu	$2,$2,152
	sw	$2,64($sp)
	lw	$2,64($sp)
	lw	$2,0($2)
	sw	$2,44($sp)
	lw	$2,20($sp)
	addiu	$3,$2,152
	lw	$4,44($sp)
	li	$2,-65536			# 0xffffffffffff0000
	and	$4,$4,$2
	lw	$2,36($sp)
	lhu	$2,0($2)
	move	$5,$2
	li	$2,8000			# 0x1f40
	div	$0,$2,$5
	mfhi	$5
	mflo	$2
	andi	$2,$2,0xffff
	or	$2,$4,$2
	sw	$3,68($sp)
	sw	$2,72($sp)
	lw	$2,68($sp)
	lw	$3,72($sp)
	sw	$3,0($2)
$L374:
	move	$2,$0
$L364:
	lw	$31,84($sp)
	addiu	$sp,$sp,88
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	qdma_set_tx_ratelimit_cfg
	.size	qdma_set_tx_ratelimit_cfg, .-qdma_set_tx_ratelimit_cfg
	.section	.text.qdma_get_tx_ratelimit_cfg,"ax",@progbits
	.align	2
	.globl	qdma_get_tx_ratelimit_cfg
	.set	nomips16
	.set	nomicromips
	.ent	qdma_get_tx_ratelimit_cfg
	.type	qdma_get_tx_ratelimit_cfg, @function
qdma_get_tx_ratelimit_cfg:
	.frame	$sp,24,$31		# vars= 24, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-24
	sw	$4,24($sp)
	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lw	$2,0($2)
	sw	$2,0($sp)
	sw	$0,4($sp)
	lw	$2,24($sp)
	lw	$2,8($2)
	sw	$2,8($sp)
	lw	$2,0($sp)
	addiu	$2,$2,152
	sw	$2,12($sp)
	lw	$2,12($sp)
	lw	$2,0($2)
	andi	$2,$2,0xffff
	sw	$2,4($sp)
	lw	$2,4($sp)
	bne	$2,$0,$L378
	nop

	lw	$2,8($sp)
	sh	$0,0($2)
	j	$L379
	nop

$L378:
	li	$3,8000			# 0x1f40
	lw	$2,4($sp)
	div	$0,$3,$2
	mfhi	$3
	mflo	$2
	andi	$3,$2,0xffff
	lw	$2,8($sp)
	sh	$3,0($2)
$L379:
	lw	$2,0($sp)
	addiu	$2,$2,152
	sw	$2,16($sp)
	lw	$2,16($sp)
	lw	$3,0($2)
	li	$2,983040			# 0xf0000
	and	$2,$3,$2
	srl	$3,$2,16
	lw	$2,8($sp)
	sw	$3,4($2)
	move	$2,$0
	addiu	$sp,$sp,24
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	qdma_get_tx_ratelimit_cfg
	.size	qdma_get_tx_ratelimit_cfg, .-qdma_get_tx_ratelimit_cfg
	.section	.text.qdma_set_tx_ratelimit,"ax",@progbits
	.align	2
	.globl	qdma_set_tx_ratelimit
	.set	nomips16
	.set	nomicromips
	.ent	qdma_set_tx_ratelimit
	.type	qdma_set_tx_ratelimit, @function
qdma_set_tx_ratelimit:
	.frame	$sp,232,$31		# vars= 208, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-232
	sw	$31,228($sp)
	sw	$4,232($sp)
	sw	$0,28($sp)
	sw	$0,32($sp)
	sw	$0,36($sp)
	sw	$0,16($sp)
	sw	$0,40($sp)
	sw	$0,44($sp)
	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lw	$2,0($2)
	sw	$2,48($sp)
	lw	$2,232($sp)
	lw	$2,8($2)
	sw	$2,52($sp)
	sw	$0,56($sp)
	sw	$0,20($sp)
	lw	$2,52($sp)
	lbu	$2,0($2)
	sltu	$2,$2,8
	bne	$2,$0,$L383
	nop

	li	$2,-22			# 0xffffffffffffffea
	j	$L384
	nop

$L383:
	lui	$2,%hi(TCSUPPORT_FWC_ENV_VAL)
	lw	$2,%lo(TCSUPPORT_FWC_ENV_VAL)($2)
	beq	$2,$0,$L385
	nop

	lw	$2,52($sp)
	lbu	$2,0($2)
	bne	$2,$0,$L386
	nop

	lw	$2,52($sp)
	lbu	$3,1($2)
	li	$2,1			# 0x1
	bne	$3,$2,$L387
	nop

	lw	$2,52($sp)
	lw	$3,4($2)
	li	$2,262144			# 0x40000
	ori	$2,$2,0x93e0
	sltu	$2,$3,$2
	beq	$2,$0,$L387
	nop

	li	$2,1			# 0x1
	sb	$2,80($sp)
	sw	$0,84($sp)
	li	$2,7			# 0x7
	sw	$2,188($sp)
	lbu	$2,80($sp)
	sb	$2,196($sp)
	addiu	$2,$sp,188
	li	$4,22			# 0x16
	move	$5,$0
	move	$6,$2
	lui	$2,%hi(__ECNT_HOOK)
	addiu	$2,$2,%lo(__ECNT_HOOK)
	jalr	$2
	nop

	sw	$2,84($sp)
	j	$L386
	nop

$L387:
	sb	$0,88($sp)
	sw	$0,92($sp)
	li	$2,7			# 0x7
	sw	$2,168($sp)
	lbu	$2,88($sp)
	sb	$2,176($sp)
	addiu	$2,$sp,168
	li	$4,22			# 0x16
	move	$5,$0
	move	$6,$2
	lui	$2,%hi(__ECNT_HOOK)
	addiu	$2,$2,%lo(__ECNT_HOOK)
	jalr	$2
	nop

	sw	$2,92($sp)
$L386:
	lw	$2,52($sp)
	lbu	$2,0($2)
	beq	$2,$0,$L385
	nop

	lw	$2,52($sp)
	lbu	$2,0($2)
	addiu	$2,$2,-1
	sw	$2,56($sp)
	lui	$2,%hi(MT7530LanPortMap2Switch_hook)
	lw	$2,%lo(MT7530LanPortMap2Switch_hook)($2)
	beq	$2,$0,$L394
	nop

	lui	$2,%hi(MT7530LanPortMap2Switch_hook)
	lw	$2,%lo(MT7530LanPortMap2Switch_hook)($2)
	lw	$4,56($sp)
	jalr	$2
	nop

	sw	$2,20($sp)
$L394:
	lw	$2,20($sp)
	andi	$3,$2,0x00ff
	lw	$2,52($sp)
	sb	$3,0($2)
$L385:
	lw	$2,52($sp)
	lbu	$2,1($2)
	bne	$2,$0,$L395
	nop

	lw	$2,48($sp)
	addiu	$2,$2,352
	sw	$2,96($sp)
	lw	$2,96($sp)
	lw	$2,0($2)
	sw	$2,60($sp)
	lw	$2,48($sp)
	addiu	$3,$2,352
	lw	$2,52($sp)
	lbu	$2,0($2)
	move	$4,$2
	li	$2,1			# 0x1
	sll	$2,$2,$4
	nor	$2,$0,$2
	move	$4,$2
	lw	$2,60($sp)
	and	$2,$4,$2
	sw	$3,100($sp)
	sw	$2,104($sp)
	lw	$2,100($sp)
	lw	$3,104($sp)
	sw	$3,0($2)
	j	$L397
	nop

$L395:
	lw	$2,48($sp)
	addiu	$2,$2,352
	sw	$2,108($sp)
	lw	$2,108($sp)
	lw	$2,0($2)
	lw	$3,52($sp)
	lbu	$3,0($3)
	move	$4,$3
	li	$3,1			# 0x1
	sll	$3,$3,$4
	and	$2,$2,$3
	sw	$2,32($sp)
	lw	$2,48($sp)
	addiu	$2,$2,352
	sw	$2,112($sp)
	lw	$2,112($sp)
	lw	$2,0($2)
	sw	$2,64($sp)
	lw	$2,48($sp)
	addiu	$3,$2,352
	lw	$2,52($sp)
	lbu	$2,0($2)
	move	$4,$2
	li	$2,1			# 0x1
	sll	$2,$2,$4
	move	$4,$2
	lw	$2,64($sp)
	or	$2,$4,$2
	sw	$3,116($sp)
	sw	$2,120($sp)
	lw	$2,116($sp)
	lw	$3,120($sp)
	sw	$3,0($2)
	lw	$2,32($sp)
	bne	$2,$0,$L397
	nop

	lw	$2,48($sp)
	addiu	$2,$2,152
	sw	$2,124($sp)
	lw	$2,124($sp)
	lw	$2,0($2)
	andi	$2,$2,0xffff
	sw	$2,36($sp)
	lw	$2,48($sp)
	addiu	$2,$2,152
	sw	$2,128($sp)
	lw	$2,128($sp)
	lw	$2,0($2)
	sw	$2,68($sp)
	lw	$2,48($sp)
	addiu	$3,$2,152
	lw	$4,68($sp)
	li	$2,-65536			# 0xffffffffffff0000
	and	$2,$4,$2
	ori	$2,$2,0x1
	sw	$3,132($sp)
	sw	$2,136($sp)
	lw	$2,132($sp)
	lw	$3,136($sp)
	sw	$3,0($2)
	lw	$2,52($sp)
	lbu	$2,0($2)
	move	$4,$2
	li	$5,2			# 0x2
	li	$6,65535			# 0xffff
	lui	$2,%hi(qdmaSetTxRateLimitConfig)
	addiu	$2,$2,%lo(qdmaSetTxRateLimitConfig)
	jalr	$2
	nop

	li	$2,100			# 0x64
	sw	$2,24($sp)
	j	$L402
	nop

$L403:
	li	$4,1000			# 0x3e8
	lui	$2,%hi(__udelay)
	addiu	$2,$2,%lo(__udelay)
	jalr	$2
	nop

$L402:
	lw	$2,24($sp)
	addiu	$3,$2,-1
	sw	$3,24($sp)
	bne	$2,$0,$L403
	nop

	lw	$2,48($sp)
	addiu	$2,$2,152
	sw	$2,140($sp)
	lw	$2,140($sp)
	lw	$2,0($2)
	sw	$2,72($sp)
	lw	$2,48($sp)
	addiu	$3,$2,152
	lw	$4,72($sp)
	li	$2,-65536			# 0xffffffffffff0000
	and	$4,$4,$2
	lw	$2,36($sp)
	andi	$2,$2,0xffff
	or	$2,$4,$2
	sw	$3,144($sp)
	sw	$2,148($sp)
	lw	$2,144($sp)
	lw	$3,148($sp)
	sw	$3,0($2)
$L397:
	lw	$2,52($sp)
	lw	$3,4($2)
	li	$2,-2147483648			# 0xffffffff80000000
	ori	$2,$2,0x8001
	multu	$3,$2
	mfhi	$2
	srl	$2,$2,15
	addiu	$2,$2,1
	sw	$2,40($sp)
	lw	$2,52($sp)
	lw	$3,4($2)
	li	$2,-776536064			# 0xffffffffd1b70000
	ori	$2,$2,0x1759
	multu	$3,$2
	mfhi	$2
	srl	$2,$2,15
	sw	$2,44($sp)
	lw	$2,40($sp)
	lw	$4,44($sp)
	lw	$3,44($sp)
	slt	$4,$4,$2
	movz	$2,$3,$4
	sw	$2,40($sp)
	lw	$2,48($sp)
	addiu	$2,$2,152
	sw	$2,152($sp)
	lw	$2,152($sp)
	lw	$2,0($2)
	andi	$2,$2,0xffff
	sw	$2,36($sp)
	lw	$2,36($sp)
	bne	$2,$0,$L406
	nop

	sw	$0,16($sp)
	j	$L407
	nop

$L406:
	li	$3,8000			# 0x1f40
	lw	$2,36($sp)
	div	$0,$3,$2
	mfhi	$3
	mflo	$2
	sw	$2,16($sp)
$L407:
	lw	$3,40($sp)
	lw	$2,16($sp)
	slt	$2,$2,$3
	beq	$2,$0,$L408
	nop

	li	$3,8000			# 0x1f40
	lw	$2,40($sp)
	div	$0,$3,$2
	mfhi	$3
	mflo	$2
	sw	$2,36($sp)
	lw	$2,48($sp)
	addiu	$2,$2,152
	sw	$2,156($sp)
	lw	$2,156($sp)
	lw	$2,0($2)
	sw	$2,76($sp)
	lw	$2,48($sp)
	addiu	$3,$2,152
	lw	$4,76($sp)
	li	$2,-65536			# 0xffffffffffff0000
	and	$4,$4,$2
	lw	$2,36($sp)
	andi	$2,$2,0xffff
	or	$2,$4,$2
	sw	$3,160($sp)
	sw	$2,164($sp)
	lw	$2,160($sp)
	lw	$3,164($sp)
	sw	$3,0($2)
	lw	$2,16($sp)
	andi	$2,$2,0xffff
	move	$3,$2
	lw	$2,40($sp)
	andi	$2,$2,0xffff
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(qdmaUpdateAllTxRateLimitValue)
	addiu	$2,$2,%lo(qdmaUpdateAllTxRateLimitValue)
	jalr	$2
	nop

	lw	$2,40($sp)
	sw	$2,16($sp)
$L408:
	lw	$2,52($sp)
	lbu	$2,0($2)
	move	$4,$2
	lw	$2,52($sp)
	lw	$3,4($2)
	lw	$2,16($sp)
	divu	$0,$3,$2
	mfhi	$3
	mflo	$2
	andi	$2,$2,0xffff
	li	$5,2			# 0x2
	move	$6,$2
	lui	$2,%hi(qdmaSetTxRateLimitConfig)
	addiu	$2,$2,%lo(qdmaSetTxRateLimitConfig)
	jalr	$2
	nop

	sw	$2,28($sp)
	lw	$2,28($sp)
	bgez	$2,$L410
	nop

	lw	$2,28($sp)
	j	$L384
	nop

$L410:
	lw	$2,52($sp)
	lbu	$2,0($2)
	move	$4,$2
	lw	$2,52($sp)
	lw	$2,4($2)
	srl	$3,$2,1
	lw	$2,52($sp)
	lw	$2,4($2)
	srl	$2,$2,3
	addu	$3,$3,$2
	lw	$2,16($sp)
	divu	$0,$3,$2
	mfhi	$3
	mflo	$2
	andi	$2,$2,0xffff
	li	$5,3			# 0x3
	move	$6,$2
	lui	$2,%hi(qdmaSetTxRateLimitConfig)
	addiu	$2,$2,%lo(qdmaSetTxRateLimitConfig)
	jalr	$2
	nop

	sw	$2,28($sp)
	lw	$2,28($sp)
	bgez	$2,$L411
	nop

	lw	$2,28($sp)
	j	$L384
	nop

$L411:
	move	$2,$0
$L384:
	lw	$31,228($sp)
	addiu	$sp,$sp,232
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	qdma_set_tx_ratelimit
	.size	qdma_set_tx_ratelimit, .-qdma_set_tx_ratelimit
	.section	.text.qdma_get_tx_ratelimit,"ax",@progbits
	.align	2
	.globl	qdma_get_tx_ratelimit
	.set	nomips16
	.set	nomicromips
	.ent	qdma_get_tx_ratelimit
	.type	qdma_get_tx_ratelimit, @function
qdma_get_tx_ratelimit:
	.frame	$sp,64,$31		# vars= 40, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-64
	sw	$31,60($sp)
	sw	$4,64($sp)
	sw	$0,24($sp)
	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lw	$2,0($2)
	sw	$2,28($sp)
	sw	$0,32($sp)
	sw	$0,16($sp)
	lw	$2,64($sp)
	lw	$2,8($2)
	sw	$2,36($sp)
	sw	$0,40($sp)
	sw	$0,20($sp)
	lw	$2,36($sp)
	lbu	$2,0($2)
	sltu	$2,$2,8
	bne	$2,$0,$L413
	nop

	li	$2,-22			# 0xffffffffffffffea
	j	$L414
	nop

$L413:
	lui	$2,%hi(TCSUPPORT_FWC_ENV_VAL)
	lw	$2,%lo(TCSUPPORT_FWC_ENV_VAL)($2)
	beq	$2,$0,$L415
	nop

	lw	$2,36($sp)
	lbu	$2,0($2)
	beq	$2,$0,$L415
	nop

	lw	$2,36($sp)
	lbu	$2,0($2)
	addiu	$2,$2,-1
	sw	$2,40($sp)
	lui	$2,%hi(MT7530LanPortMap2Switch_hook)
	lw	$2,%lo(MT7530LanPortMap2Switch_hook)($2)
	beq	$2,$0,$L416
	nop

	lui	$2,%hi(MT7530LanPortMap2Switch_hook)
	lw	$2,%lo(MT7530LanPortMap2Switch_hook)($2)
	lw	$4,40($sp)
	jalr	$2
	nop

	sw	$2,20($sp)
$L416:
	lw	$2,20($sp)
	andi	$3,$2,0x00ff
	lw	$2,36($sp)
	sb	$3,0($2)
$L415:
	lw	$2,28($sp)
	addiu	$2,$2,152
	sw	$2,44($sp)
	lw	$2,44($sp)
	lw	$2,0($2)
	andi	$2,$2,0xffff
	sw	$2,32($sp)
	lw	$2,32($sp)
	bne	$2,$0,$L418
	nop

	sw	$0,16($sp)
	j	$L419
	nop

$L418:
	li	$3,8000			# 0x1f40
	lw	$2,32($sp)
	div	$0,$3,$2
	mfhi	$3
	mflo	$2
	sw	$2,16($sp)
$L419:
	lw	$2,28($sp)
	addiu	$2,$2,352
	sw	$2,48($sp)
	lw	$2,48($sp)
	lw	$2,0($2)
	lw	$3,36($sp)
	lbu	$3,0($3)
	move	$4,$3
	li	$3,1			# 0x1
	sll	$3,$3,$4
	and	$2,$2,$3
	beq	$2,$0,$L421
	nop

	lw	$2,36($sp)
	li	$3,1			# 0x1
	sb	$3,1($2)
	j	$L422
	nop

$L421:
	lw	$2,36($sp)
	sb	$0,1($2)
$L422:
	lw	$2,36($sp)
	lbu	$2,0($2)
	move	$4,$2
	li	$5,2			# 0x2
	lui	$2,%hi(qdmaGetTxRateLimitConfig)
	addiu	$2,$2,%lo(qdmaGetTxRateLimitConfig)
	jalr	$2
	nop

	sw	$2,24($sp)
	lw	$2,24($sp)
	bgez	$2,$L423
	nop

	lw	$2,24($sp)
	j	$L414
	nop

$L423:
	lw	$3,24($sp)
	lw	$2,16($sp)
	mul	$2,$3,$2
	move	$3,$2
	lw	$2,36($sp)
	sw	$3,4($2)
	lw	$2,36($sp)
	lbu	$2,0($2)
	move	$4,$2
	li	$5,3			# 0x3
	lui	$2,%hi(qdmaGetTxRateLimitConfig)
	addiu	$2,$2,%lo(qdmaGetTxRateLimitConfig)
	jalr	$2
	nop

	sw	$2,24($sp)
	lw	$2,24($sp)
	bgez	$2,$L424
	nop

	lw	$2,24($sp)
	j	$L414
	nop

$L424:
	lw	$2,28($sp)
	addiu	$2,$2,152
	sw	$2,52($sp)
	lw	$2,52($sp)
	lw	$3,0($2)
	li	$2,983040			# 0xf0000
	and	$2,$3,$2
	srl	$2,$2,16
	move	$3,$2
	lw	$2,24($sp)
	sll	$2,$2,$3
	move	$3,$2
	lw	$2,36($sp)
	sw	$3,8($2)
	move	$2,$0
$L414:
	lw	$31,60($sp)
	addiu	$sp,$sp,64
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	qdma_get_tx_ratelimit
	.size	qdma_get_tx_ratelimit, .-qdma_get_tx_ratelimit
	.section	.text.qdma_set_tx_dba_report,"ax",@progbits
	.align	2
	.globl	qdma_set_tx_dba_report
	.set	nomips16
	.set	nomicromips
	.ent	qdma_set_tx_dba_report
	.type	qdma_set_tx_dba_report, @function
qdma_set_tx_dba_report:
	.frame	$sp,80,$31		# vars= 56, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-80
	sw	$31,76($sp)
	sw	$4,80($sp)
	sw	$0,24($sp)
	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lw	$2,0($2)
	sw	$2,28($sp)
	sw	$0,16($sp)
	sw	$0,32($sp)
	sw	$0,20($sp)
	sw	$0,36($sp)
	lw	$2,80($sp)
	lw	$2,8($2)
	sw	$2,40($sp)
	lw	$2,40($sp)
	lbu	$2,0($2)
	sltu	$2,$2,8
	bne	$2,$0,$L427
	nop

	li	$2,-22			# 0xffffffffffffffea
	j	$L428
	nop

$L427:
	lw	$2,40($sp)
	lw	$3,12($2)
	lw	$2,40($sp)
	lw	$2,4($2)
	sltu	$2,$2,$3
	beq	$2,$0,$L429
	nop

	lw	$2,40($sp)
	lw	$2,12($2)
	sw	$2,16($sp)
	j	$L430
	nop

$L429:
	lw	$2,40($sp)
	lw	$2,4($2)
	sw	$2,16($sp)
$L430:
	lw	$3,16($sp)
	li	$2,-2147483648			# 0xffffffff80000000
	ori	$2,$2,0x8001
	multu	$3,$2
	mfhi	$2
	srl	$2,$2,15
	addiu	$2,$2,1
	sw	$2,36($sp)
	lw	$2,28($sp)
	addiu	$2,$2,152
	sw	$2,48($sp)
	lw	$2,48($sp)
	lw	$2,0($2)
	andi	$2,$2,0xffff
	sw	$2,32($sp)
	lw	$2,32($sp)
	bne	$2,$0,$L432
	nop

	sw	$0,20($sp)
	j	$L433
	nop

$L432:
	li	$3,8000			# 0x1f40
	lw	$2,32($sp)
	div	$0,$3,$2
	mfhi	$3
	mflo	$2
	sw	$2,20($sp)
$L433:
	lw	$3,36($sp)
	lw	$2,20($sp)
	slt	$2,$2,$3
	beq	$2,$0,$L434
	nop

	li	$3,8000			# 0x1f40
	lw	$2,36($sp)
	div	$0,$3,$2
	mfhi	$3
	mflo	$2
	sw	$2,32($sp)
	lw	$2,28($sp)
	addiu	$2,$2,152
	sw	$2,52($sp)
	lw	$2,52($sp)
	lw	$2,0($2)
	sw	$2,44($sp)
	lw	$2,28($sp)
	addiu	$3,$2,152
	lw	$4,44($sp)
	li	$2,-65536			# 0xffffffffffff0000
	and	$4,$4,$2
	lw	$2,32($sp)
	andi	$2,$2,0xffff
	or	$2,$4,$2
	sw	$3,56($sp)
	sw	$2,60($sp)
	lw	$2,56($sp)
	lw	$3,60($sp)
	sw	$3,0($2)
	lw	$2,20($sp)
	andi	$2,$2,0xffff
	move	$3,$2
	lw	$2,36($sp)
	andi	$2,$2,0xffff
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(qdmaUpdateAllTxRateLimitValue)
	addiu	$2,$2,%lo(qdmaUpdateAllTxRateLimitValue)
	jalr	$2
	nop

	lw	$2,36($sp)
	sw	$2,20($sp)
$L434:
	lw	$2,40($sp)
	lbu	$2,0($2)
	move	$4,$2
	lw	$2,40($sp)
	lw	$3,4($2)
	lw	$2,20($sp)
	divu	$0,$3,$2
	mfhi	$3
	mflo	$2
	andi	$2,$2,0xffff
	move	$5,$0
	move	$6,$2
	lui	$2,%hi(qdmaSetTxRateLimitConfig)
	addiu	$2,$2,%lo(qdmaSetTxRateLimitConfig)
	jalr	$2
	nop

	sw	$2,24($sp)
	lw	$2,24($sp)
	bgez	$2,$L436
	nop

	lw	$2,24($sp)
	j	$L428
	nop

$L436:
	lw	$2,40($sp)
	lbu	$2,0($2)
	move	$5,$2
	lw	$2,40($sp)
	lw	$2,8($2)
	lw	$3,28($sp)
	addiu	$3,$3,152
	sw	$3,64($sp)
	lw	$3,64($sp)
	lw	$4,0($3)
	li	$3,983040			# 0xf0000
	and	$3,$4,$3
	srl	$3,$3,16
	srl	$2,$2,$3
	andi	$2,$2,0xffff
	move	$4,$5
	li	$5,1			# 0x1
	move	$6,$2
	lui	$2,%hi(qdmaSetTxRateLimitConfig)
	addiu	$2,$2,%lo(qdmaSetTxRateLimitConfig)
	jalr	$2
	nop

	sw	$2,24($sp)
	lw	$2,24($sp)
	bgez	$2,$L438
	nop

	lw	$2,24($sp)
	j	$L428
	nop

$L438:
	lw	$2,40($sp)
	lbu	$2,0($2)
	move	$4,$2
	lw	$2,40($sp)
	lw	$3,12($2)
	lw	$2,20($sp)
	divu	$0,$3,$2
	mfhi	$3
	mflo	$2
	andi	$2,$2,0xffff
	li	$5,2			# 0x2
	move	$6,$2
	lui	$2,%hi(qdmaSetTxRateLimitConfig)
	addiu	$2,$2,%lo(qdmaSetTxRateLimitConfig)
	jalr	$2
	nop

	sw	$2,24($sp)
	lw	$2,24($sp)
	bgez	$2,$L439
	nop

	lw	$2,24($sp)
	j	$L428
	nop

$L439:
	lw	$2,40($sp)
	lbu	$2,0($2)
	move	$3,$2
	lw	$2,40($sp)
	lw	$2,16($2)
	andi	$2,$2,0xffff
	move	$4,$3
	li	$5,3			# 0x3
	move	$6,$2
	lui	$2,%hi(qdmaSetTxRateLimitConfig)
	addiu	$2,$2,%lo(qdmaSetTxRateLimitConfig)
	jalr	$2
	nop

	move	$4,$2
	lw	$2,28($sp)
	addiu	$2,$2,152
	sw	$2,68($sp)
	lw	$2,68($sp)
	lw	$3,0($2)
	li	$2,983040			# 0xf0000
	and	$2,$3,$2
	srl	$2,$2,16
	move	$3,$2
	li	$2,1			# 0x1
	sll	$2,$2,$3
	div	$0,$4,$2
	mfhi	$3
	mflo	$2
	sw	$2,24($sp)
	lw	$2,24($sp)
	bgez	$2,$L441
	nop

	lw	$2,24($sp)
	j	$L428
	nop

$L441:
	move	$2,$0
$L428:
	lw	$31,76($sp)
	addiu	$sp,$sp,80
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	qdma_set_tx_dba_report
	.size	qdma_set_tx_dba_report, .-qdma_set_tx_dba_report
	.section	.text.qdma_get_tx_dba_report,"ax",@progbits
	.align	2
	.globl	qdma_get_tx_dba_report
	.set	nomips16
	.set	nomicromips
	.ent	qdma_get_tx_dba_report
	.type	qdma_get_tx_dba_report, @function
qdma_get_tx_dba_report:
	.frame	$sp,56,$31		# vars= 32, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-56
	sw	$31,52($sp)
	sw	$4,56($sp)
	sw	$0,20($sp)
	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lw	$2,0($2)
	sw	$2,24($sp)
	sw	$0,28($sp)
	sw	$0,16($sp)
	lw	$2,56($sp)
	lw	$2,8($2)
	sw	$2,32($sp)
	lw	$2,32($sp)
	lbu	$2,0($2)
	sltu	$2,$2,8
	bne	$2,$0,$L443
	nop

	li	$2,-22			# 0xffffffffffffffea
	j	$L444
	nop

$L443:
	lw	$2,24($sp)
	addiu	$2,$2,152
	sw	$2,36($sp)
	lw	$2,36($sp)
	lw	$2,0($2)
	andi	$2,$2,0xffff
	sw	$2,28($sp)
	lw	$2,28($sp)
	bne	$2,$0,$L446
	nop

	sw	$0,16($sp)
	j	$L447
	nop

$L446:
	li	$3,8000			# 0x1f40
	lw	$2,28($sp)
	div	$0,$3,$2
	mfhi	$3
	mflo	$2
	sw	$2,16($sp)
$L447:
	lw	$2,32($sp)
	lbu	$2,0($2)
	move	$4,$2
	move	$5,$0
	lui	$2,%hi(qdmaGetTxRateLimitConfig)
	addiu	$2,$2,%lo(qdmaGetTxRateLimitConfig)
	jalr	$2
	nop

	sw	$2,20($sp)
	lw	$2,20($sp)
	bgez	$2,$L448
	nop

	lw	$2,20($sp)
	j	$L444
	nop

$L448:
	lw	$3,20($sp)
	lw	$2,16($sp)
	mul	$2,$3,$2
	move	$3,$2
	lw	$2,32($sp)
	sw	$3,4($2)
	lw	$2,32($sp)
	lbu	$2,0($2)
	move	$4,$2
	li	$5,1			# 0x1
	lui	$2,%hi(qdmaGetTxRateLimitConfig)
	addiu	$2,$2,%lo(qdmaGetTxRateLimitConfig)
	jalr	$2
	nop

	sw	$2,20($sp)
	lw	$2,20($sp)
	bgez	$2,$L449
	nop

	lw	$2,20($sp)
	j	$L444
	nop

$L449:
	lw	$2,24($sp)
	addiu	$2,$2,152
	sw	$2,40($sp)
	lw	$2,40($sp)
	lw	$3,0($2)
	li	$2,983040			# 0xf0000
	and	$2,$3,$2
	srl	$2,$2,16
	move	$3,$2
	lw	$2,20($sp)
	sll	$2,$2,$3
	move	$3,$2
	lw	$2,32($sp)
	sw	$3,8($2)
	lw	$2,32($sp)
	lbu	$2,0($2)
	move	$4,$2
	li	$5,2			# 0x2
	lui	$2,%hi(qdmaGetTxRateLimitConfig)
	addiu	$2,$2,%lo(qdmaGetTxRateLimitConfig)
	jalr	$2
	nop

	sw	$2,20($sp)
	lw	$2,20($sp)
	bgez	$2,$L451
	nop

	lw	$2,20($sp)
	j	$L444
	nop

$L451:
	lw	$3,20($sp)
	lw	$2,16($sp)
	mul	$2,$3,$2
	move	$3,$2
	lw	$2,32($sp)
	sw	$3,12($2)
	lw	$2,32($sp)
	lbu	$2,0($2)
	move	$4,$2
	li	$5,3			# 0x3
	lui	$2,%hi(qdmaGetTxRateLimitConfig)
	addiu	$2,$2,%lo(qdmaGetTxRateLimitConfig)
	jalr	$2
	nop

	sw	$2,20($sp)
	lw	$2,20($sp)
	bgez	$2,$L452
	nop

	lw	$2,20($sp)
	j	$L444
	nop

$L452:
	lw	$2,24($sp)
	addiu	$2,$2,152
	sw	$2,44($sp)
	lw	$2,44($sp)
	lw	$3,0($2)
	li	$2,983040			# 0xf0000
	and	$2,$3,$2
	srl	$2,$2,16
	move	$3,$2
	lw	$2,20($sp)
	sll	$2,$2,$3
	move	$3,$2
	lw	$2,32($sp)
	sw	$3,16($2)
	move	$2,$0
$L444:
	lw	$31,52($sp)
	addiu	$sp,$sp,56
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	qdma_get_tx_dba_report
	.size	qdma_get_tx_dba_report, .-qdma_get_tx_dba_report
	.section	.text.qdma_set_rx_protect_en,"ax",@progbits
	.align	2
	.globl	qdma_set_rx_protect_en
	.set	nomips16
	.set	nomicromips
	.ent	qdma_set_rx_protect_en
	.type	qdma_set_rx_protect_en, @function
qdma_set_rx_protect_en:
	.frame	$sp,40,$31		# vars= 40, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-40
	sw	$4,40($sp)
	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lw	$2,0($2)
	sw	$2,0($sp)
	lw	$2,40($sp)
	lw	$2,8($2)
	sw	$2,4($sp)
	lw	$3,4($sp)
	li	$2,1			# 0x1
	bne	$3,$2,$L455
	nop

	lw	$2,0($sp)
	addiu	$2,$2,288
	sw	$2,16($sp)
	lw	$2,16($sp)
	lw	$2,0($2)
	sw	$2,8($sp)
	lw	$2,0($sp)
	addiu	$3,$2,288
	lw	$4,8($sp)
	li	$2,-2147483648			# 0xffffffff80000000
	or	$2,$4,$2
	sw	$3,20($sp)
	sw	$2,24($sp)
	lw	$2,20($sp)
	lw	$3,24($sp)
	sw	$3,0($2)
	j	$L457
	nop

$L455:
	lw	$2,0($sp)
	addiu	$2,$2,288
	sw	$2,28($sp)
	lw	$2,28($sp)
	lw	$2,0($2)
	sw	$2,12($sp)
	lw	$2,0($sp)
	addiu	$3,$2,288
	lw	$2,12($sp)
	ext	$2,$2,0,31
	sw	$3,32($sp)
	sw	$2,36($sp)
	lw	$2,32($sp)
	lw	$3,36($sp)
	sw	$3,0($2)
$L457:
	move	$2,$0
	addiu	$sp,$sp,40
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	qdma_set_rx_protect_en
	.size	qdma_set_rx_protect_en, .-qdma_set_rx_protect_en
	.section	.text.qdma_get_rx_protect_en,"ax",@progbits
	.align	2
	.globl	qdma_get_rx_protect_en
	.set	nomips16
	.set	nomicromips
	.ent	qdma_get_rx_protect_en
	.type	qdma_get_rx_protect_en, @function
qdma_get_rx_protect_en:
	.frame	$sp,8,$31		# vars= 8, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-8
	sw	$4,8($sp)
	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lw	$2,0($2)
	sw	$2,0($sp)
	lw	$2,0($sp)
	addiu	$2,$2,288
	sw	$2,4($sp)
	lw	$2,4($sp)
	lw	$2,0($2)
	bgez	$2,$L462
	nop

	lw	$2,8($sp)
	li	$3,1			# 0x1
	sw	$3,8($2)
	j	$L463
	nop

$L462:
	lw	$2,8($sp)
	sw	$0,8($2)
$L463:
	lw	$2,8($sp)
	lw	$2,8($2)
	addiu	$sp,$sp,8
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	qdma_get_rx_protect_en
	.size	qdma_get_rx_protect_en, .-qdma_get_rx_protect_en
	.section	.text.qdma_set_rx_low_threshold,"ax",@progbits
	.align	2
	.globl	qdma_set_rx_low_threshold
	.set	nomips16
	.set	nomicromips
	.ent	qdma_set_rx_low_threshold
	.type	qdma_set_rx_low_threshold, @function
qdma_set_rx_low_threshold:
	.frame	$sp,48,$31		# vars= 48, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-48
	sw	$4,48($sp)
	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lw	$2,0($2)
	sw	$2,0($sp)
	lw	$2,48($sp)
	lw	$2,8($2)
	sw	$2,4($sp)
	lw	$2,4($sp)
	lw	$3,0($2)
	lw	$2,0($sp)
	addiu	$2,$2,256
	sw	$2,16($sp)
	lw	$2,16($sp)
	lw	$2,0($2)
	andi	$2,$2,0xfff
	sltu	$2,$2,$3
	bne	$2,$0,$L467
	nop

	lw	$2,4($sp)
	lw	$3,4($2)
	lw	$2,0($sp)
	addiu	$2,$2,256
	sw	$2,20($sp)
	lw	$2,20($sp)
	lw	$4,0($2)
	li	$2,268369920			# 0xfff0000
	and	$2,$4,$2
	srl	$2,$2,16
	sltu	$2,$2,$3
	beq	$2,$0,$L469
	nop

$L467:
	li	$2,-22			# 0xffffffffffffffea
	j	$L470
	nop

$L469:
	lw	$2,0($sp)
	addiu	$2,$2,260
	sw	$2,24($sp)
	lw	$2,24($sp)
	lw	$2,0($2)
	sw	$2,8($sp)
	lw	$2,0($sp)
	addiu	$3,$2,260
	lw	$4,8($sp)
	li	$2,-4096			# 0xfffffffffffff000
	and	$4,$4,$2
	lw	$2,4($sp)
	lw	$2,0($2)
	andi	$2,$2,0xfff
	or	$2,$4,$2
	sw	$3,28($sp)
	sw	$2,32($sp)
	lw	$2,28($sp)
	lw	$3,32($sp)
	sw	$3,0($2)
	lw	$2,0($sp)
	addiu	$2,$2,260
	sw	$2,36($sp)
	lw	$2,36($sp)
	lw	$2,0($2)
	sw	$2,12($sp)
	lw	$2,0($sp)
	addiu	$3,$2,260
	lw	$4,12($sp)
	li	$2,-268435456			# 0xfffffffff0000000
	ori	$2,$2,0xffff
	and	$4,$4,$2
	lw	$2,4($sp)
	lw	$2,4($2)
	sll	$5,$2,16
	li	$2,268369920			# 0xfff0000
	and	$2,$5,$2
	or	$2,$4,$2
	sw	$3,40($sp)
	sw	$2,44($sp)
	lw	$2,40($sp)
	lw	$3,44($sp)
	sw	$3,0($2)
	move	$2,$0
$L470:
	addiu	$sp,$sp,48
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	qdma_set_rx_low_threshold
	.size	qdma_set_rx_low_threshold, .-qdma_set_rx_low_threshold
	.section	.text.qdma_get_rx_low_threshold,"ax",@progbits
	.align	2
	.globl	qdma_get_rx_low_threshold
	.set	nomips16
	.set	nomicromips
	.ent	qdma_get_rx_low_threshold
	.type	qdma_get_rx_low_threshold, @function
qdma_get_rx_low_threshold:
	.frame	$sp,16,$31		# vars= 16, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-16
	sw	$4,16($sp)
	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lw	$2,0($2)
	sw	$2,0($sp)
	lw	$2,16($sp)
	lw	$2,8($2)
	sw	$2,4($sp)
	lw	$2,0($sp)
	addiu	$2,$2,260
	sw	$2,8($sp)
	lw	$2,8($sp)
	lw	$2,0($2)
	andi	$3,$2,0xfff
	lw	$2,4($sp)
	sw	$3,0($2)
	lw	$2,0($sp)
	addiu	$2,$2,260
	sw	$2,12($sp)
	lw	$2,12($sp)
	lw	$3,0($2)
	li	$2,268369920			# 0xfff0000
	and	$2,$3,$2
	srl	$3,$2,16
	lw	$2,4($sp)
	sw	$3,4($2)
	move	$2,$0
	addiu	$sp,$sp,16
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	qdma_get_rx_low_threshold
	.size	qdma_get_rx_low_threshold, .-qdma_get_rx_low_threshold
	.section	.text.qdma_set_rx_ratelimit_en,"ax",@progbits
	.align	2
	.globl	qdma_set_rx_ratelimit_en
	.set	nomips16
	.set	nomicromips
	.ent	qdma_set_rx_ratelimit_en
	.type	qdma_set_rx_ratelimit_en, @function
qdma_set_rx_ratelimit_en:
	.frame	$sp,128,$31		# vars= 104, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-128
	sw	$31,124($sp)
	sw	$4,128($sp)
	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lw	$2,0($2)
	sw	$2,20($sp)
	sw	$0,24($sp)
	sw	$0,16($sp)
	lw	$2,128($sp)
	lw	$2,8($2)
	sw	$2,28($sp)
	lw	$2,28($sp)
	bne	$2,$0,$L478
	nop

	lw	$2,20($sp)
	addiu	$2,$2,288
	sw	$2,48($sp)
	lw	$2,48($sp)
	lw	$2,0($2)
	andi	$2,$2,0xffff
	sw	$2,24($sp)
	lw	$2,20($sp)
	addiu	$2,$2,288
	sw	$2,52($sp)
	lw	$2,52($sp)
	lw	$2,0($2)
	sw	$2,32($sp)
	lw	$2,20($sp)
	addiu	$3,$2,288
	lw	$4,32($sp)
	li	$2,-65536			# 0xffffffffffff0000
	and	$2,$4,$2
	ori	$2,$2,0x1f4
	sw	$3,56($sp)
	sw	$2,60($sp)
	lw	$2,56($sp)
	lw	$3,60($sp)
	sw	$3,0($2)
	sw	$0,16($sp)
	j	$L481
	nop

$L482:
	lw	$2,16($sp)
	andi	$2,$2,0x00ff
	move	$4,$2
	move	$5,$0
	lui	$2,%hi(qdmaGetRxRateLimitConfig)
	addiu	$2,$2,%lo(qdmaGetRxRateLimitConfig)
	jalr	$2
	nop

	move	$4,$2
	lw	$2,16($sp)
	sll	$2,$2,2
	addiu	$3,$sp,16
	addu	$2,$3,$2
	sw	$4,84($2)
	lw	$2,16($sp)
	andi	$2,$2,0x00ff
	move	$4,$2
	li	$5,1			# 0x1
	lui	$2,%hi(qdmaGetRxRateLimitConfig)
	addiu	$2,$2,%lo(qdmaGetRxRateLimitConfig)
	jalr	$2
	nop

	move	$4,$2
	lw	$2,16($sp)
	sll	$2,$2,2
	addiu	$3,$sp,16
	addu	$2,$3,$2
	sw	$4,92($2)
	lw	$2,16($sp)
	andi	$2,$2,0x00ff
	move	$4,$2
	move	$5,$0
	li	$6,65535			# 0xffff
	lui	$2,%hi(qdmaSetRxRateLimitConfig)
	addiu	$2,$2,%lo(qdmaSetRxRateLimitConfig)
	jalr	$2
	nop

	lw	$2,16($sp)
	andi	$2,$2,0x00ff
	move	$4,$2
	li	$5,1			# 0x1
	li	$6,65535			# 0xffff
	lui	$2,%hi(qdmaSetRxRateLimitConfig)
	addiu	$2,$2,%lo(qdmaSetRxRateLimitConfig)
	jalr	$2
	nop

	lw	$2,16($sp)
	addiu	$2,$2,1
	sw	$2,16($sp)
$L481:
	lw	$2,16($sp)
	slt	$2,$2,2
	bne	$2,$0,$L482
	nop

	li	$4,2000			# 0x7d0
	lui	$2,%hi(__udelay)
	addiu	$2,$2,%lo(__udelay)
	jalr	$2
	nop

	lw	$2,20($sp)
	addiu	$2,$2,288
	sw	$2,64($sp)
	lw	$2,64($sp)
	lw	$2,0($2)
	sw	$2,36($sp)
	lw	$2,20($sp)
	addiu	$3,$2,288
	lw	$4,36($sp)
	li	$2,-1073807360			# 0xffffffffbfff0000
	ori	$2,$2,0xffff
	and	$2,$4,$2
	sw	$3,68($sp)
	sw	$2,72($sp)
	lw	$2,68($sp)
	lw	$3,72($sp)
	sw	$3,0($2)
	sw	$0,16($sp)
	j	$L484
	nop

$L485:
	lw	$2,16($sp)
	andi	$2,$2,0x00ff
	move	$4,$2
	lw	$2,16($sp)
	sll	$2,$2,2
	addiu	$3,$sp,16
	addu	$2,$3,$2
	lw	$2,84($2)
	andi	$2,$2,0xffff
	move	$5,$0
	move	$6,$2
	lui	$2,%hi(qdmaSetRxRateLimitConfig)
	addiu	$2,$2,%lo(qdmaSetRxRateLimitConfig)
	jalr	$2
	nop

	lw	$2,16($sp)
	andi	$2,$2,0x00ff
	move	$4,$2
	lw	$2,16($sp)
	sll	$2,$2,2
	addiu	$3,$sp,16
	addu	$2,$3,$2
	lw	$2,92($2)
	andi	$2,$2,0xffff
	li	$5,1			# 0x1
	move	$6,$2
	lui	$2,%hi(qdmaSetRxRateLimitConfig)
	addiu	$2,$2,%lo(qdmaSetRxRateLimitConfig)
	jalr	$2
	nop

	lw	$2,16($sp)
	addiu	$2,$2,1
	sw	$2,16($sp)
$L484:
	lw	$2,16($sp)
	slt	$2,$2,2
	bne	$2,$0,$L485
	nop

	lw	$2,20($sp)
	addiu	$2,$2,288
	sw	$2,76($sp)
	lw	$2,76($sp)
	lw	$2,0($2)
	sw	$2,40($sp)
	lw	$2,20($sp)
	addiu	$3,$2,288
	lw	$4,40($sp)
	li	$2,-65536			# 0xffffffffffff0000
	and	$4,$4,$2
	lw	$2,24($sp)
	andi	$2,$2,0xffff
	or	$2,$4,$2
	sw	$3,80($sp)
	sw	$2,84($sp)
	lw	$2,80($sp)
	lw	$3,84($sp)
	sw	$3,0($2)
	j	$L487
	nop

$L478:
	lw	$2,20($sp)
	addiu	$2,$2,288
	sw	$2,88($sp)
	lw	$2,88($sp)
	lw	$2,0($2)
	sw	$2,44($sp)
	lw	$2,20($sp)
	addiu	$3,$2,288
	lw	$4,44($sp)
	li	$2,1073741824			# 0x40000000
	or	$2,$4,$2
	sw	$3,92($sp)
	sw	$2,96($sp)
	lw	$2,92($sp)
	lw	$3,96($sp)
	sw	$3,0($2)
$L487:
	move	$2,$0
	lw	$31,124($sp)
	addiu	$sp,$sp,128
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	qdma_set_rx_ratelimit_en
	.size	qdma_set_rx_ratelimit_en, .-qdma_set_rx_ratelimit_en
	.section	.text.qdma_set_rx_ratelimit_pkt_mode,"ax",@progbits
	.align	2
	.globl	qdma_set_rx_ratelimit_pkt_mode
	.set	nomips16
	.set	nomicromips
	.ent	qdma_set_rx_ratelimit_pkt_mode
	.type	qdma_set_rx_ratelimit_pkt_mode, @function
qdma_set_rx_ratelimit_pkt_mode:
	.frame	$sp,72,$31		# vars= 72, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-72
	sw	$4,72($sp)
	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lw	$2,0($2)
	sw	$2,0($sp)
	lw	$2,72($sp)
	lw	$2,8($2)
	sw	$2,4($sp)
	lw	$3,4($sp)
	li	$2,1			# 0x1
	bne	$3,$2,$L491
	nop

	lw	$2,0($sp)
	addiu	$2,$2,288
	sw	$2,24($sp)
	lw	$2,24($sp)
	lw	$2,0($2)
	sw	$2,8($sp)
	lw	$2,0($sp)
	addiu	$3,$2,288
	lw	$4,8($sp)
	li	$2,536870912			# 0x20000000
	or	$2,$4,$2
	sw	$3,28($sp)
	sw	$2,32($sp)
	lw	$2,28($sp)
	lw	$3,32($sp)
	sw	$3,0($2)
	lw	$2,0($sp)
	addiu	$2,$2,288
	sw	$2,36($sp)
	lw	$2,36($sp)
	lw	$2,0($2)
	sw	$2,12($sp)
	lw	$2,0($sp)
	addiu	$3,$2,288
	lw	$4,12($sp)
	li	$2,-65536			# 0xffffffffffff0000
	and	$2,$4,$2
	ori	$2,$2,0xf424
	sw	$3,40($sp)
	sw	$2,44($sp)
	lw	$2,40($sp)
	lw	$3,44($sp)
	sw	$3,0($2)
	j	$L494
	nop

$L491:
	lw	$2,0($sp)
	addiu	$2,$2,288
	sw	$2,48($sp)
	lw	$2,48($sp)
	lw	$2,0($2)
	sw	$2,16($sp)
	lw	$2,0($sp)
	addiu	$3,$2,288
	lw	$4,16($sp)
	li	$2,-536936448			# 0xffffffffdfff0000
	ori	$2,$2,0xffff
	and	$2,$4,$2
	sw	$3,52($sp)
	sw	$2,56($sp)
	lw	$2,52($sp)
	lw	$3,56($sp)
	sw	$3,0($2)
	lw	$2,0($sp)
	addiu	$2,$2,288
	sw	$2,60($sp)
	lw	$2,60($sp)
	lw	$2,0($2)
	sw	$2,20($sp)
	lw	$2,0($sp)
	addiu	$3,$2,288
	lw	$4,20($sp)
	li	$2,-65536			# 0xffffffffffff0000
	and	$2,$4,$2
	ori	$2,$2,0x3e8
	sw	$3,64($sp)
	sw	$2,68($sp)
	lw	$2,64($sp)
	lw	$3,68($sp)
	sw	$3,0($2)
$L494:
	move	$2,$0
	addiu	$sp,$sp,72
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	qdma_set_rx_ratelimit_pkt_mode
	.size	qdma_set_rx_ratelimit_pkt_mode, .-qdma_set_rx_ratelimit_pkt_mode
	.section	.text.qdma_get_rx_ratelimit_cfg,"ax",@progbits
	.align	2
	.globl	qdma_get_rx_ratelimit_cfg
	.set	nomips16
	.set	nomicromips
	.ent	qdma_get_rx_ratelimit_cfg
	.type	qdma_get_rx_ratelimit_cfg, @function
qdma_get_rx_ratelimit_cfg:
	.frame	$sp,32,$31		# vars= 32, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-32
	sw	$4,32($sp)
	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lw	$2,0($2)
	sw	$2,4($sp)
	sw	$0,8($sp)
	sw	$0,0($sp)
	lw	$2,32($sp)
	lw	$2,8($2)
	sw	$2,12($sp)
	lw	$2,4($sp)
	addiu	$2,$2,288
	sw	$2,16($sp)
	lw	$2,16($sp)
	lw	$3,0($2)
	li	$2,1073741824			# 0x40000000
	and	$2,$3,$2
	beq	$2,$0,$L500
	nop

	lw	$2,12($sp)
	li	$3,1			# 0x1
	sb	$3,0($2)
	j	$L501
	nop

$L500:
	lw	$2,12($sp)
	sb	$0,0($2)
$L501:
	lw	$2,4($sp)
	addiu	$2,$2,288
	sw	$2,20($sp)
	lw	$2,20($sp)
	lw	$3,0($2)
	li	$2,536870912			# 0x20000000
	and	$2,$3,$2
	beq	$2,$0,$L503
	nop

	lw	$2,12($sp)
	li	$3,1			# 0x1
	sw	$3,4($2)
	li	$2,983040			# 0xf0000
	ori	$2,$2,0x4240
	sw	$2,0($sp)
	j	$L504
	nop

$L503:
	lw	$2,12($sp)
	sw	$0,4($2)
	li	$2,8000			# 0x1f40
	sw	$2,0($sp)
$L504:
	lw	$2,4($sp)
	addiu	$2,$2,288
	sw	$2,24($sp)
	lw	$2,24($sp)
	lw	$2,0($2)
	andi	$2,$2,0xffff
	sw	$2,8($sp)
	lw	$2,8($sp)
	bne	$2,$0,$L506
	nop

	lw	$2,12($sp)
	sh	$0,8($2)
	j	$L507
	nop

$L506:
	lw	$3,0($sp)
	lw	$2,8($sp)
	div	$0,$3,$2
	mfhi	$3
	mflo	$2
	andi	$3,$2,0xffff
	lw	$2,12($sp)
	sh	$3,8($2)
$L507:
	lw	$2,4($sp)
	addiu	$2,$2,288
	sw	$2,28($sp)
	lw	$2,28($sp)
	lw	$3,0($2)
	li	$2,251658240			# 0xf000000
	and	$2,$3,$2
	srl	$3,$2,24
	lw	$2,12($sp)
	sw	$3,12($2)
	move	$2,$0
	addiu	$sp,$sp,32
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	qdma_get_rx_ratelimit_cfg
	.size	qdma_get_rx_ratelimit_cfg, .-qdma_get_rx_ratelimit_cfg
	.section	.text.qdma_set_rx_ratelimit,"ax",@progbits
	.align	2
	.globl	qdma_set_rx_ratelimit
	.set	nomips16
	.set	nomicromips
	.ent	qdma_set_rx_ratelimit
	.type	qdma_set_rx_ratelimit, @function
qdma_set_rx_ratelimit:
	.frame	$sp,80,$31		# vars= 56, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-80
	sw	$31,76($sp)
	sw	$4,80($sp)
	sw	$0,24($sp)
	sw	$0,16($sp)
	sw	$0,28($sp)
	sw	$0,20($sp)
	sw	$0,32($sp)
	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lw	$2,0($2)
	sw	$2,36($sp)
	lw	$2,80($sp)
	lw	$2,8($2)
	sw	$2,40($sp)
	lw	$2,40($sp)
	lbu	$2,0($2)
	sltu	$2,$2,2
	bne	$2,$0,$L511
	nop

	li	$2,-22			# 0xffffffffffffffea
	j	$L512
	nop

$L511:
	lw	$2,36($sp)
	addiu	$2,$2,288
	sw	$2,48($sp)
	lw	$2,48($sp)
	lw	$3,0($2)
	li	$2,536870912			# 0x20000000
	and	$2,$3,$2
	beq	$2,$0,$L514
	nop

	li	$2,983040			# 0xf0000
	ori	$2,$2,0x4240
	sw	$2,16($sp)
	j	$L515
	nop

$L514:
	li	$2,8000			# 0x1f40
	sw	$2,16($sp)
$L515:
	lw	$2,40($sp)
	lw	$3,4($2)
	li	$2,-2147483648			# 0xffffffff80000000
	ori	$2,$2,0x8001
	multu	$3,$2
	mfhi	$2
	srl	$2,$2,15
	addiu	$2,$2,1
	sw	$2,32($sp)
	lw	$2,36($sp)
	addiu	$2,$2,288
	sw	$2,52($sp)
	lw	$2,52($sp)
	lw	$2,0($2)
	andi	$2,$2,0xffff
	sw	$2,28($sp)
	lw	$2,28($sp)
	bne	$2,$0,$L517
	nop

	sw	$0,20($sp)
	j	$L518
	nop

$L517:
	lw	$3,16($sp)
	lw	$2,28($sp)
	div	$0,$3,$2
	mfhi	$3
	mflo	$2
	sw	$2,20($sp)
$L518:
	lw	$3,32($sp)
	lw	$2,20($sp)
	slt	$2,$2,$3
	beq	$2,$0,$L519
	nop

	lw	$3,16($sp)
	lw	$2,32($sp)
	div	$0,$3,$2
	mfhi	$3
	mflo	$2
	sw	$2,28($sp)
	lw	$2,36($sp)
	addiu	$2,$2,288
	sw	$2,56($sp)
	lw	$2,56($sp)
	lw	$2,0($2)
	sw	$2,44($sp)
	lw	$2,36($sp)
	addiu	$3,$2,288
	lw	$4,44($sp)
	li	$2,-65536			# 0xffffffffffff0000
	and	$4,$4,$2
	lw	$2,28($sp)
	andi	$2,$2,0xffff
	or	$2,$4,$2
	sw	$3,60($sp)
	sw	$2,64($sp)
	lw	$2,60($sp)
	lw	$3,64($sp)
	sw	$3,0($2)
	lw	$2,20($sp)
	andi	$2,$2,0xffff
	move	$3,$2
	lw	$2,32($sp)
	andi	$2,$2,0xffff
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(qdmaUpdateAllRxRateLimitValue)
	addiu	$2,$2,%lo(qdmaUpdateAllRxRateLimitValue)
	jalr	$2
	nop

	lw	$2,32($sp)
	sw	$2,20($sp)
$L519:
	lw	$2,40($sp)
	lbu	$2,0($2)
	move	$4,$2
	lw	$2,40($sp)
	lw	$3,4($2)
	lw	$2,20($sp)
	divu	$0,$3,$2
	mfhi	$3
	mflo	$2
	andi	$2,$2,0xffff
	move	$5,$0
	move	$6,$2
	lui	$2,%hi(qdmaSetRxRateLimitConfig)
	addiu	$2,$2,%lo(qdmaSetRxRateLimitConfig)
	jalr	$2
	nop

	sw	$2,24($sp)
	lw	$2,24($sp)
	bgez	$2,$L521
	nop

	lw	$2,24($sp)
	j	$L512
	nop

$L521:
	lw	$2,40($sp)
	lbu	$2,0($2)
	move	$4,$2
	lw	$2,40($sp)
	lw	$3,4($2)
	lw	$2,20($sp)
	divu	$0,$3,$2
	mfhi	$3
	mflo	$2
	andi	$2,$2,0xffff
	li	$5,1			# 0x1
	move	$6,$2
	lui	$2,%hi(qdmaSetRxRateLimitConfig)
	addiu	$2,$2,%lo(qdmaSetRxRateLimitConfig)
	jalr	$2
	nop

	sw	$2,24($sp)
	lw	$2,24($sp)
	bgez	$2,$L522
	nop

	lw	$2,24($sp)
	j	$L512
	nop

$L522:
	move	$2,$0
$L512:
	lw	$31,76($sp)
	addiu	$sp,$sp,80
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	qdma_set_rx_ratelimit
	.size	qdma_set_rx_ratelimit, .-qdma_set_rx_ratelimit
	.section	.text.qdma_get_rx_ratelimit,"ax",@progbits
	.align	2
	.globl	qdma_get_rx_ratelimit
	.set	nomips16
	.set	nomicromips
	.ent	qdma_get_rx_ratelimit
	.type	qdma_get_rx_ratelimit, @function
qdma_get_rx_ratelimit:
	.frame	$sp,64,$31		# vars= 40, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-64
	sw	$31,60($sp)
	sw	$4,64($sp)
	sw	$0,24($sp)
	sw	$0,16($sp)
	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lw	$2,0($2)
	sw	$2,28($sp)
	sw	$0,32($sp)
	sw	$0,20($sp)
	lw	$2,64($sp)
	lw	$2,8($2)
	sw	$2,36($sp)
	lw	$2,36($sp)
	lbu	$2,0($2)
	sltu	$2,$2,2
	bne	$2,$0,$L524
	nop

	li	$2,-22			# 0xffffffffffffffea
	j	$L525
	nop

$L524:
	lw	$2,28($sp)
	addiu	$2,$2,288
	sw	$2,40($sp)
	lw	$2,40($sp)
	lw	$3,0($2)
	li	$2,536870912			# 0x20000000
	and	$2,$3,$2
	beq	$2,$0,$L527
	nop

	li	$2,983040			# 0xf0000
	ori	$2,$2,0x4240
	sw	$2,16($sp)
	j	$L528
	nop

$L527:
	li	$2,8000			# 0x1f40
	sw	$2,16($sp)
$L528:
	lw	$2,28($sp)
	addiu	$2,$2,288
	sw	$2,44($sp)
	lw	$2,44($sp)
	lw	$2,0($2)
	andi	$2,$2,0xffff
	sw	$2,32($sp)
	lw	$2,32($sp)
	bne	$2,$0,$L530
	nop

	sw	$0,20($sp)
	j	$L531
	nop

$L530:
	lw	$3,16($sp)
	lw	$2,32($sp)
	div	$0,$3,$2
	mfhi	$3
	mflo	$2
	sw	$2,20($sp)
$L531:
	lw	$2,36($sp)
	lbu	$2,0($2)
	move	$4,$2
	move	$5,$0
	lui	$2,%hi(qdmaGetRxRateLimitConfig)
	addiu	$2,$2,%lo(qdmaGetRxRateLimitConfig)
	jalr	$2
	nop

	sw	$2,24($sp)
	lw	$2,24($sp)
	bgez	$2,$L532
	nop

	lw	$2,24($sp)
	j	$L525
	nop

$L532:
	lw	$3,24($sp)
	lw	$2,20($sp)
	mul	$2,$3,$2
	move	$3,$2
	lw	$2,36($sp)
	sw	$3,4($2)
	lw	$2,36($sp)
	lbu	$2,0($2)
	move	$4,$2
	li	$5,1			# 0x1
	lui	$2,%hi(qdmaGetRxRateLimitConfig)
	addiu	$2,$2,%lo(qdmaGetRxRateLimitConfig)
	jalr	$2
	nop

	sw	$2,24($sp)
	lw	$2,24($sp)
	bgez	$2,$L533
	nop

	lw	$2,24($sp)
	j	$L525
	nop

$L533:
	lw	$2,28($sp)
	addiu	$2,$2,288
	sw	$2,48($sp)
	lw	$2,48($sp)
	lw	$3,0($2)
	li	$2,251658240			# 0xf000000
	and	$2,$3,$2
	srl	$2,$2,24
	move	$3,$2
	lw	$2,24($sp)
	sll	$2,$2,$3
	move	$3,$2
	lw	$2,36($sp)
	sw	$3,8($2)
	move	$2,$0
$L525:
	lw	$31,60($sp)
	addiu	$sp,$sp,64
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	qdma_get_rx_ratelimit
	.size	qdma_get_rx_ratelimit, .-qdma_get_rx_ratelimit
	.section	.text.qdma_set_txq_dei_drop_mode,"ax",@progbits
	.align	2
	.globl	qdma_set_txq_dei_drop_mode
	.set	nomips16
	.set	nomicromips
	.ent	qdma_set_txq_dei_drop_mode
	.type	qdma_set_txq_dei_drop_mode, @function
qdma_set_txq_dei_drop_mode:
	.frame	$sp,40,$31		# vars= 40, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-40
	sw	$4,40($sp)
	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lw	$2,0($2)
	sw	$2,0($sp)
	lw	$2,40($sp)
	lw	$2,8($2)
	sw	$2,4($sp)
	lw	$3,4($sp)
	li	$2,1			# 0x1
	bne	$3,$2,$L536
	nop

	lw	$2,0($sp)
	addiu	$2,$2,160
	sw	$2,16($sp)
	lw	$2,16($sp)
	lw	$2,0($2)
	sw	$2,8($sp)
	lw	$2,0($sp)
	addiu	$3,$2,160
	lw	$4,8($sp)
	li	$2,1073741824			# 0x40000000
	or	$2,$4,$2
	sw	$3,20($sp)
	sw	$2,24($sp)
	lw	$2,20($sp)
	lw	$3,24($sp)
	sw	$3,0($2)
	j	$L538
	nop

$L536:
	lw	$2,0($sp)
	addiu	$2,$2,160
	sw	$2,28($sp)
	lw	$2,28($sp)
	lw	$2,0($2)
	sw	$2,12($sp)
	lw	$2,0($sp)
	addiu	$3,$2,160
	lw	$4,12($sp)
	li	$2,-1073807360			# 0xffffffffbfff0000
	ori	$2,$2,0xffff
	and	$2,$4,$2
	sw	$3,32($sp)
	sw	$2,36($sp)
	lw	$2,32($sp)
	lw	$3,36($sp)
	sw	$3,0($2)
$L538:
	move	$2,$0
	addiu	$sp,$sp,40
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	qdma_set_txq_dei_drop_mode
	.size	qdma_set_txq_dei_drop_mode, .-qdma_set_txq_dei_drop_mode
	.section	.text.qdma_get_txq_dei_drop_mode,"ax",@progbits
	.align	2
	.globl	qdma_get_txq_dei_drop_mode
	.set	nomips16
	.set	nomicromips
	.ent	qdma_get_txq_dei_drop_mode
	.type	qdma_get_txq_dei_drop_mode, @function
qdma_get_txq_dei_drop_mode:
	.frame	$sp,8,$31		# vars= 8, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-8
	sw	$4,8($sp)
	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lw	$2,0($2)
	sw	$2,0($sp)
	lw	$2,0($sp)
	addiu	$2,$2,160
	sw	$2,4($sp)
	lw	$2,4($sp)
	lw	$3,0($2)
	li	$2,1073741824			# 0x40000000
	and	$2,$3,$2
	beq	$2,$0,$L543
	nop

	lw	$2,8($sp)
	li	$3,1			# 0x1
	sw	$3,8($2)
	j	$L544
	nop

$L543:
	lw	$2,8($sp)
	sw	$0,8($2)
$L544:
	lw	$2,8($sp)
	lw	$2,8($2)
	addiu	$sp,$sp,8
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	qdma_get_txq_dei_drop_mode
	.size	qdma_get_txq_dei_drop_mode, .-qdma_get_txq_dei_drop_mode
	.section	.text.qdma_set_txq_cngst_mode,"ax",@progbits
	.align	2
	.globl	qdma_set_txq_cngst_mode
	.set	nomips16
	.set	nomicromips
	.ent	qdma_set_txq_cngst_mode
	.type	qdma_set_txq_cngst_mode, @function
qdma_set_txq_cngst_mode:
	.frame	$sp,40,$31		# vars= 40, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-40
	sw	$4,40($sp)
	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lw	$2,0($2)
	sw	$2,0($sp)
	lw	$2,40($sp)
	lw	$2,8($2)
	sw	$2,4($sp)
	lw	$3,4($sp)
	li	$2,1			# 0x1
	bne	$3,$2,$L547
	nop

	lw	$2,0($sp)
	addiu	$2,$2,160
	sw	$2,16($sp)
	lw	$2,16($sp)
	lw	$2,0($2)
	sw	$2,8($sp)
	lw	$2,0($sp)
	addiu	$3,$2,160
	lw	$4,8($sp)
	li	$2,536870912			# 0x20000000
	or	$2,$4,$2
	sw	$3,20($sp)
	sw	$2,24($sp)
	lw	$2,20($sp)
	lw	$3,24($sp)
	sw	$3,0($2)
	j	$L549
	nop

$L547:
	lw	$2,0($sp)
	addiu	$2,$2,160
	sw	$2,28($sp)
	lw	$2,28($sp)
	lw	$2,0($2)
	sw	$2,12($sp)
	lw	$2,0($sp)
	addiu	$3,$2,160
	lw	$4,12($sp)
	li	$2,-536936448			# 0xffffffffdfff0000
	ori	$2,$2,0xffff
	and	$2,$4,$2
	sw	$3,32($sp)
	sw	$2,36($sp)
	lw	$2,32($sp)
	lw	$3,36($sp)
	sw	$3,0($2)
$L549:
	move	$2,$0
	addiu	$sp,$sp,40
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	qdma_set_txq_cngst_mode
	.size	qdma_set_txq_cngst_mode, .-qdma_set_txq_cngst_mode
	.section	.text.qdma_get_txq_cngst_mode,"ax",@progbits
	.align	2
	.globl	qdma_get_txq_cngst_mode
	.set	nomips16
	.set	nomicromips
	.ent	qdma_get_txq_cngst_mode
	.type	qdma_get_txq_cngst_mode, @function
qdma_get_txq_cngst_mode:
	.frame	$sp,8,$31		# vars= 8, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-8
	sw	$4,8($sp)
	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lw	$2,0($2)
	sw	$2,0($sp)
	lw	$2,0($sp)
	addiu	$2,$2,160
	sw	$2,4($sp)
	lw	$2,4($sp)
	lw	$3,0($2)
	li	$2,536870912			# 0x20000000
	and	$2,$3,$2
	beq	$2,$0,$L554
	nop

	lw	$2,8($sp)
	li	$3,1			# 0x1
	sw	$3,8($2)
	j	$L555
	nop

$L554:
	lw	$2,8($sp)
	sw	$0,8($2)
$L555:
	lw	$2,8($sp)
	lw	$2,8($2)
	addiu	$sp,$sp,8
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	qdma_get_txq_cngst_mode
	.size	qdma_get_txq_cngst_mode, .-qdma_get_txq_cngst_mode
	.section	.text.qdma_set_txq_dei_threshold_scale,"ax",@progbits
	.align	2
	.globl	qdma_set_txq_dei_threshold_scale
	.set	nomips16
	.set	nomicromips
	.ent	qdma_set_txq_dei_threshold_scale
	.type	qdma_set_txq_dei_threshold_scale, @function
qdma_set_txq_dei_threshold_scale:
	.frame	$sp,24,$31		# vars= 24, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-24
	sw	$4,24($sp)
	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lw	$2,0($2)
	sw	$2,0($sp)
	lw	$2,24($sp)
	lw	$2,8($2)
	sw	$2,4($sp)
	lw	$2,0($sp)
	addiu	$2,$2,160
	sw	$2,12($sp)
	lw	$2,12($sp)
	lw	$2,0($2)
	sw	$2,8($sp)
	lw	$2,0($sp)
	addiu	$3,$2,160
	lw	$4,8($sp)
	li	$2,-3211264			# 0xffffffffffcf0000
	ori	$2,$2,0xffff
	and	$4,$4,$2
	lw	$2,4($sp)
	sll	$5,$2,20
	li	$2,3145728			# 0x300000
	and	$2,$5,$2
	or	$2,$4,$2
	sw	$3,16($sp)
	sw	$2,20($sp)
	lw	$2,16($sp)
	lw	$3,20($sp)
	sw	$3,0($2)
	move	$2,$0
	addiu	$sp,$sp,24
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	qdma_set_txq_dei_threshold_scale
	.size	qdma_set_txq_dei_threshold_scale, .-qdma_set_txq_dei_threshold_scale
	.section	.text.qdma_get_txq_dei_threshold_scale,"ax",@progbits
	.align	2
	.globl	qdma_get_txq_dei_threshold_scale
	.set	nomips16
	.set	nomicromips
	.ent	qdma_get_txq_dei_threshold_scale
	.type	qdma_get_txq_dei_threshold_scale, @function
qdma_get_txq_dei_threshold_scale:
	.frame	$sp,8,$31		# vars= 8, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-8
	sw	$4,8($sp)
	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lw	$2,0($2)
	sw	$2,0($sp)
	lw	$2,0($sp)
	addiu	$2,$2,160
	sw	$2,4($sp)
	lw	$2,4($sp)
	lw	$3,0($2)
	li	$2,3145728			# 0x300000
	and	$2,$3,$2
	srl	$3,$2,20
	lw	$2,8($sp)
	sw	$3,8($2)
	lw	$2,8($sp)
	lw	$2,8($2)
	addiu	$sp,$sp,8
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	qdma_get_txq_dei_threshold_scale
	.size	qdma_get_txq_dei_threshold_scale, .-qdma_get_txq_dei_threshold_scale
	.section	.text.qdma_set_txq_cngst_auto_config,"ax",@progbits
	.align	2
	.globl	qdma_set_txq_cngst_auto_config
	.set	nomips16
	.set	nomicromips
	.ent	qdma_set_txq_cngst_auto_config
	.type	qdma_set_txq_cngst_auto_config, @function
qdma_set_txq_cngst_auto_config:
	.frame	$sp,304,$31		# vars= 280, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-304
	sw	$31,300($sp)
	sw	$4,304($sp)
	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lw	$2,0($2)
	sw	$2,16($sp)
	lw	$2,304($sp)
	lw	$2,8($2)
	sw	$2,20($sp)
	lw	$2,16($sp)
	addiu	$2,$2,160
	sw	$2,88($sp)
	lw	$2,88($sp)
	lw	$2,0($2)
	sw	$2,24($sp)
	lw	$2,16($sp)
	addiu	$3,$2,160
	lw	$4,24($sp)
	li	$2,-134283264			# 0xfffffffff7ff0000
	ori	$2,$2,0xffff
	and	$2,$4,$2
	sw	$3,92($sp)
	sw	$2,96($sp)
	lw	$2,92($sp)
	lw	$3,96($sp)
	sw	$3,0($2)
	lw	$2,16($sp)
	addiu	$2,$2,160
	sw	$2,100($sp)
	lw	$2,100($sp)
	lw	$2,0($2)
	sw	$2,28($sp)
	lw	$2,16($sp)
	addiu	$3,$2,160
	lw	$4,28($sp)
	li	$2,-67174400			# 0xfffffffffbff0000
	ori	$2,$2,0xffff
	and	$2,$4,$2
	sw	$3,104($sp)
	sw	$2,108($sp)
	lw	$2,104($sp)
	lw	$3,108($sp)
	sw	$3,0($2)
	lw	$2,16($sp)
	addiu	$2,$2,160
	sw	$2,112($sp)
	lw	$2,112($sp)
	lw	$2,0($2)
	sw	$2,32($sp)
	lw	$2,16($sp)
	addiu	$3,$2,160
	lw	$4,32($sp)
	li	$2,-33619968			# 0xfffffffffdff0000
	ori	$2,$2,0xffff
	and	$2,$4,$2
	sw	$3,116($sp)
	sw	$2,120($sp)
	lw	$2,116($sp)
	lw	$3,120($sp)
	sw	$3,0($2)
	lw	$2,16($sp)
	addiu	$2,$2,160
	sw	$2,124($sp)
	lw	$2,124($sp)
	lw	$2,0($2)
	sw	$2,36($sp)
	lw	$2,16($sp)
	addiu	$3,$2,160
	lw	$4,36($sp)
	li	$2,-16842752			# 0xfffffffffeff0000
	ori	$2,$2,0xffff
	and	$2,$4,$2
	sw	$3,128($sp)
	sw	$2,132($sp)
	lw	$2,128($sp)
	lw	$3,132($sp)
	sw	$3,0($2)
	lw	$2,16($sp)
	addiu	$2,$2,160
	sw	$2,136($sp)
	lw	$2,136($sp)
	lw	$2,0($2)
	sw	$2,40($sp)
	lw	$2,16($sp)
	addiu	$3,$2,160
	lw	$4,40($sp)
	li	$2,262144			# 0x40000
	or	$2,$4,$2
	sw	$3,140($sp)
	sw	$2,144($sp)
	lw	$2,140($sp)
	lw	$3,144($sp)
	sw	$3,0($2)
	lw	$2,16($sp)
	addiu	$2,$2,160
	sw	$2,148($sp)
	lw	$2,148($sp)
	lw	$2,0($2)
	sw	$2,44($sp)
	lw	$2,16($sp)
	addiu	$3,$2,160
	lw	$4,44($sp)
	li	$2,131072			# 0x20000
	or	$2,$4,$2
	sw	$3,152($sp)
	sw	$2,156($sp)
	lw	$2,152($sp)
	lw	$3,156($sp)
	sw	$3,0($2)
	lw	$2,16($sp)
	addiu	$2,$2,160
	sw	$2,160($sp)
	lw	$2,160($sp)
	lw	$2,0($2)
	sw	$2,48($sp)
	lw	$2,16($sp)
	addiu	$3,$2,160
	lw	$4,48($sp)
	li	$2,65536			# 0x10000
	or	$2,$4,$2
	sw	$3,164($sp)
	sw	$2,168($sp)
	lw	$2,164($sp)
	lw	$3,168($sp)
	sw	$3,0($2)
	lw	$2,16($sp)
	addiu	$2,$2,148
	sw	$2,172($sp)
	lw	$2,172($sp)
	lw	$2,0($2)
	sw	$2,52($sp)
	lw	$2,16($sp)
	addiu	$3,$2,148
	lw	$4,52($sp)
	li	$2,-2147483648			# 0xffffffff80000000
	or	$2,$4,$2
	sw	$3,176($sp)
	sw	$2,180($sp)
	lw	$2,176($sp)
	lw	$3,180($sp)
	sw	$3,0($2)
	li	$2,204			# 0xcc
	sh	$2,282($sp)
	li	$2,73			# 0x49
	sh	$2,286($sp)
	li	$2,6			# 0x6
	sh	$2,290($sp)
	lui	$2,%hi(physical_size)
	lw	$3,%lo(physical_size)($2)
	lui	$2,%hi(physical_size)
	lw	$2,%lo(physical_size)($2)
	srl	$2,$2,4
	subu	$2,$3,$2
	srl	$2,$2,8
	andi	$2,$2,0xffff
	sh	$2,280($sp)
	lhu	$2,280($sp)
	sh	$2,284($sp)
	lhu	$2,282($sp)
	sh	$2,288($sp)
	lw	$2,20($sp)
	lw	$3,0($2)
	li	$2,2			# 0x2
	bne	$3,$2,$L572
	nop

	lw	$2,16($sp)
	addiu	$2,$2,160
	sw	$2,184($sp)
	lw	$2,184($sp)
	lw	$2,0($2)
	sw	$2,56($sp)
	lw	$2,16($sp)
	addiu	$3,$2,160
	lw	$4,56($sp)
	li	$2,-536936448			# 0xffffffffdfff0000
	ori	$2,$2,0xffff
	and	$2,$4,$2
	sw	$3,188($sp)
	sw	$2,192($sp)
	lw	$2,188($sp)
	lw	$3,192($sp)
	sw	$3,0($2)
	lw	$2,16($sp)
	addiu	$2,$2,148
	sw	$2,196($sp)
	lw	$2,196($sp)
	lw	$2,0($2)
	sw	$2,60($sp)
	lw	$2,16($sp)
	addiu	$3,$2,148
	lw	$4,60($sp)
	li	$2,-1073807360			# 0xffffffffbfff0000
	ori	$2,$2,0xffff
	and	$2,$4,$2
	sw	$3,200($sp)
	sw	$2,204($sp)
	lw	$2,200($sp)
	lw	$3,204($sp)
	sw	$3,0($2)
	j	$L575
	nop

$L572:
	lw	$2,20($sp)
	lw	$2,0($2)
	bne	$2,$0,$L576
	nop

	lw	$2,16($sp)
	addiu	$2,$2,160
	sw	$2,208($sp)
	lw	$2,208($sp)
	lw	$2,0($2)
	sw	$2,64($sp)
	lw	$2,16($sp)
	addiu	$3,$2,160
	lw	$4,64($sp)
	li	$2,536870912			# 0x20000000
	or	$2,$4,$2
	sw	$3,212($sp)
	sw	$2,216($sp)
	lw	$2,212($sp)
	lw	$3,216($sp)
	sw	$3,0($2)
	lw	$2,16($sp)
	addiu	$2,$2,148
	sw	$2,220($sp)
	lw	$2,220($sp)
	lw	$2,0($2)
	sw	$2,68($sp)
	lw	$2,16($sp)
	addiu	$3,$2,148
	lw	$4,68($sp)
	li	$2,-1073807360			# 0xffffffffbfff0000
	ori	$2,$2,0xffff
	and	$2,$4,$2
	sw	$3,224($sp)
	sw	$2,228($sp)
	lw	$2,224($sp)
	lw	$3,228($sp)
	sw	$3,0($2)
	lw	$2,304($sp)
	addiu	$3,$sp,280
	sw	$3,8($2)
	lw	$4,304($sp)
	lui	$2,%hi(qdma_set_txq_cngst_dynamic_threshold)
	addiu	$2,$2,%lo(qdma_set_txq_cngst_dynamic_threshold)
	jalr	$2
	nop

	j	$L575
	nop

$L576:
	lw	$2,20($sp)
	lw	$3,0($2)
	li	$2,1			# 0x1
	bne	$3,$2,$L575
	nop

	lw	$2,16($sp)
	addiu	$2,$2,160
	sw	$2,232($sp)
	lw	$2,232($sp)
	lw	$2,0($2)
	sw	$2,72($sp)
	lw	$2,16($sp)
	addiu	$3,$2,160
	lw	$4,72($sp)
	li	$2,536870912			# 0x20000000
	or	$2,$4,$2
	sw	$3,236($sp)
	sw	$2,240($sp)
	lw	$2,236($sp)
	lw	$3,240($sp)
	sw	$3,0($2)
	lw	$2,16($sp)
	addiu	$2,$2,148
	sw	$2,244($sp)
	lw	$2,244($sp)
	lw	$2,0($2)
	sw	$2,76($sp)
	lw	$2,16($sp)
	addiu	$3,$2,148
	lw	$4,76($sp)
	li	$2,1073741824			# 0x40000000
	or	$2,$4,$2
	sw	$3,248($sp)
	sw	$2,252($sp)
	lw	$2,248($sp)
	lw	$3,252($sp)
	sw	$3,0($2)
	lw	$2,16($sp)
	addiu	$2,$2,160
	sw	$2,256($sp)
	lw	$2,256($sp)
	lw	$2,0($2)
	sw	$2,80($sp)
	lw	$2,16($sp)
	addiu	$3,$2,160
	lw	$4,80($sp)
	li	$2,-12648448			# 0xffffffffff3f0000
	ori	$2,$2,0xffff
	and	$4,$4,$2
	lw	$2,20($sp)
	lw	$2,4($2)
	sll	$5,$2,22
	li	$2,12582912			# 0xc00000
	and	$2,$5,$2
	or	$2,$4,$2
	sw	$3,260($sp)
	sw	$2,264($sp)
	lw	$2,260($sp)
	lw	$3,264($sp)
	sw	$3,0($2)
	lw	$2,16($sp)
	addiu	$2,$2,148
	sw	$2,268($sp)
	lw	$2,268($sp)
	lw	$2,0($2)
	sw	$2,84($sp)
	lw	$2,16($sp)
	addiu	$3,$2,148
	lw	$4,84($sp)
	li	$2,-267452416			# 0xfffffffff00f0000
	ori	$2,$2,0xffff
	and	$4,$4,$2
	lw	$2,20($sp)
	lw	$2,8($2)
	srl	$2,$2,4
	sll	$5,$2,20
	li	$2,267386880			# 0xff00000
	and	$2,$5,$2
	or	$2,$4,$2
	sw	$3,272($sp)
	sw	$2,276($sp)
	lw	$2,272($sp)
	lw	$3,276($sp)
	sw	$3,0($2)
	lw	$2,304($sp)
	addiu	$3,$sp,280
	sw	$3,8($2)
	lw	$4,304($sp)
	lui	$2,%hi(qdma_set_txq_cngst_dynamic_threshold)
	addiu	$2,$2,%lo(qdma_set_txq_cngst_dynamic_threshold)
	jalr	$2
	nop

$L575:
	move	$2,$0
	lw	$31,300($sp)
	addiu	$sp,$sp,304
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	qdma_set_txq_cngst_auto_config
	.size	qdma_set_txq_cngst_auto_config, .-qdma_set_txq_cngst_auto_config
	.section	.text.qdma_get_txq_cngst_auto_config,"ax",@progbits
	.align	2
	.globl	qdma_get_txq_cngst_auto_config
	.set	nomips16
	.set	nomicromips
	.ent	qdma_get_txq_cngst_auto_config
	.type	qdma_get_txq_cngst_auto_config, @function
qdma_get_txq_cngst_auto_config:
	.frame	$sp,24,$31		# vars= 24, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-24
	sw	$4,24($sp)
	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lw	$2,0($2)
	sw	$2,0($sp)
	lw	$2,24($sp)
	lw	$2,8($2)
	sw	$2,4($sp)
	lw	$2,0($sp)
	addiu	$2,$2,160
	sw	$2,8($sp)
	lw	$2,8($sp)
	lw	$3,0($2)
	li	$2,536870912			# 0x20000000
	and	$2,$3,$2
	beq	$2,$0,$L586
	nop

	lw	$2,0($sp)
	addiu	$2,$2,148
	sw	$2,12($sp)
	lw	$2,12($sp)
	lw	$3,0($2)
	li	$2,1073741824			# 0x40000000
	and	$2,$3,$2
	beq	$2,$0,$L588
	nop

	lw	$2,4($sp)
	li	$3,1			# 0x1
	sw	$3,0($2)
	lw	$2,0($sp)
	addiu	$2,$2,148
	sw	$2,16($sp)
	lw	$2,16($sp)
	lw	$3,0($2)
	li	$2,267386880			# 0xff00000
	and	$2,$3,$2
	srl	$3,$2,20
	lw	$2,4($sp)
	sw	$3,8($2)
	lw	$2,0($sp)
	addiu	$2,$2,160
	sw	$2,20($sp)
	lw	$2,20($sp)
	lw	$3,0($2)
	li	$2,12582912			# 0xc00000
	and	$2,$3,$2
	srl	$3,$2,22
	lw	$2,4($sp)
	sw	$3,4($2)
	j	$L592
	nop

$L588:
	lw	$2,4($sp)
	sw	$0,0($2)
	j	$L592
	nop

$L586:
	lw	$2,4($sp)
	li	$3,2			# 0x2
	sw	$3,0($2)
$L592:
	move	$2,$0
	addiu	$sp,$sp,24
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	qdma_get_txq_cngst_auto_config
	.size	qdma_get_txq_cngst_auto_config, .-qdma_get_txq_cngst_auto_config
	.section	.text.qdma_set_txq_cngst_dynamic_threshold,"ax",@progbits
	.align	2
	.globl	qdma_set_txq_cngst_dynamic_threshold
	.set	nomips16
	.set	nomicromips
	.ent	qdma_set_txq_cngst_dynamic_threshold
	.type	qdma_set_txq_cngst_dynamic_threshold, @function
qdma_set_txq_cngst_dynamic_threshold:
	.frame	$sp,104,$31		# vars= 104, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-104
	sw	$4,104($sp)
	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lw	$2,0($2)
	sw	$2,0($sp)
	lw	$2,104($sp)
	lw	$2,8($2)
	sw	$2,4($sp)
	lw	$2,4($sp)
	lhu	$3,2($2)
	lw	$2,4($sp)
	lhu	$2,0($2)
	sltu	$2,$2,$3
	beq	$2,$0,$L595
	nop

	li	$2,-22			# 0xffffffffffffffea
	j	$L596
	nop

$L595:
	lw	$2,4($sp)
	lhu	$3,6($2)
	lw	$2,4($sp)
	lhu	$2,4($2)
	sltu	$2,$2,$3
	beq	$2,$0,$L597
	nop

	li	$2,-22			# 0xffffffffffffffea
	j	$L596
	nop

$L597:
	lw	$2,4($sp)
	lhu	$3,10($2)
	lw	$2,4($sp)
	lhu	$2,8($2)
	sltu	$2,$2,$3
	beq	$2,$0,$L598
	nop

	li	$2,-22			# 0xffffffffffffffea
	j	$L596
	nop

$L598:
	lw	$2,0($sp)
	addiu	$2,$2,164
	sw	$2,32($sp)
	lw	$2,32($sp)
	lw	$2,0($2)
	sw	$2,8($sp)
	lw	$2,0($sp)
	addiu	$3,$2,164
	lw	$2,8($sp)
	andi	$2,$2,0xffff
	lw	$4,4($sp)
	lhu	$4,0($4)
	sll	$4,$4,16
	or	$2,$2,$4
	sw	$3,36($sp)
	sw	$2,40($sp)
	lw	$2,36($sp)
	lw	$3,40($sp)
	sw	$3,0($2)
	lw	$2,0($sp)
	addiu	$2,$2,164
	sw	$2,44($sp)
	lw	$2,44($sp)
	lw	$2,0($2)
	sw	$2,12($sp)
	lw	$2,0($sp)
	addiu	$3,$2,164
	lw	$4,12($sp)
	li	$2,-65536			# 0xffffffffffff0000
	and	$2,$4,$2
	lw	$4,4($sp)
	lhu	$4,2($4)
	or	$2,$2,$4
	sw	$3,48($sp)
	sw	$2,52($sp)
	lw	$2,48($sp)
	lw	$3,52($sp)
	sw	$3,0($2)
	lw	$2,0($sp)
	addiu	$2,$2,168
	sw	$2,56($sp)
	lw	$2,56($sp)
	lw	$2,0($2)
	sw	$2,16($sp)
	lw	$2,0($sp)
	addiu	$3,$2,168
	lw	$2,16($sp)
	andi	$2,$2,0xffff
	lw	$4,4($sp)
	lhu	$4,4($4)
	sll	$4,$4,16
	or	$2,$2,$4
	sw	$3,60($sp)
	sw	$2,64($sp)
	lw	$2,60($sp)
	lw	$3,64($sp)
	sw	$3,0($2)
	lw	$2,0($sp)
	addiu	$2,$2,168
	sw	$2,68($sp)
	lw	$2,68($sp)
	lw	$2,0($2)
	sw	$2,20($sp)
	lw	$2,0($sp)
	addiu	$3,$2,168
	lw	$4,20($sp)
	li	$2,-65536			# 0xffffffffffff0000
	and	$2,$4,$2
	lw	$4,4($sp)
	lhu	$4,6($4)
	or	$2,$2,$4
	sw	$3,72($sp)
	sw	$2,76($sp)
	lw	$2,72($sp)
	lw	$3,76($sp)
	sw	$3,0($2)
	lw	$2,0($sp)
	addiu	$2,$2,172
	sw	$2,80($sp)
	lw	$2,80($sp)
	lw	$2,0($2)
	sw	$2,24($sp)
	lw	$2,0($sp)
	addiu	$3,$2,172
	lw	$2,24($sp)
	andi	$2,$2,0xffff
	lw	$4,4($sp)
	lhu	$4,8($4)
	sll	$4,$4,16
	or	$2,$2,$4
	sw	$3,84($sp)
	sw	$2,88($sp)
	lw	$2,84($sp)
	lw	$3,88($sp)
	sw	$3,0($2)
	lw	$2,0($sp)
	addiu	$2,$2,172
	sw	$2,92($sp)
	lw	$2,92($sp)
	lw	$2,0($2)
	sw	$2,28($sp)
	lw	$2,0($sp)
	addiu	$3,$2,172
	lw	$4,28($sp)
	li	$2,-65536			# 0xffffffffffff0000
	and	$2,$4,$2
	lw	$4,4($sp)
	lhu	$4,10($4)
	or	$2,$2,$4
	sw	$3,96($sp)
	sw	$2,100($sp)
	lw	$2,96($sp)
	lw	$3,100($sp)
	sw	$3,0($2)
	move	$2,$0
$L596:
	addiu	$sp,$sp,104
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	qdma_set_txq_cngst_dynamic_threshold
	.size	qdma_set_txq_cngst_dynamic_threshold, .-qdma_set_txq_cngst_dynamic_threshold
	.section	.text.qdma_get_txq_cngst_dynamic_threshold,"ax",@progbits
	.align	2
	.globl	qdma_get_txq_cngst_dynamic_threshold
	.set	nomips16
	.set	nomicromips
	.ent	qdma_get_txq_cngst_dynamic_threshold
	.type	qdma_get_txq_cngst_dynamic_threshold, @function
qdma_get_txq_cngst_dynamic_threshold:
	.frame	$sp,32,$31		# vars= 32, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-32
	sw	$4,32($sp)
	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lw	$2,0($2)
	sw	$2,0($sp)
	lw	$2,32($sp)
	lw	$2,8($2)
	sw	$2,4($sp)
	lw	$2,0($sp)
	addiu	$2,$2,164
	sw	$2,8($sp)
	lw	$2,8($sp)
	lw	$2,0($2)
	srl	$2,$2,16
	andi	$3,$2,0xffff
	lw	$2,4($sp)
	sh	$3,0($2)
	lw	$2,0($sp)
	addiu	$2,$2,164
	sw	$2,12($sp)
	lw	$2,12($sp)
	lw	$2,0($2)
	andi	$3,$2,0xffff
	lw	$2,4($sp)
	sh	$3,2($2)
	lw	$2,0($sp)
	addiu	$2,$2,168
	sw	$2,16($sp)
	lw	$2,16($sp)
	lw	$2,0($2)
	srl	$2,$2,16
	andi	$3,$2,0xffff
	lw	$2,4($sp)
	sh	$3,4($2)
	lw	$2,0($sp)
	addiu	$2,$2,168
	sw	$2,20($sp)
	lw	$2,20($sp)
	lw	$2,0($2)
	andi	$3,$2,0xffff
	lw	$2,4($sp)
	sh	$3,6($2)
	lw	$2,0($sp)
	addiu	$2,$2,172
	sw	$2,24($sp)
	lw	$2,24($sp)
	lw	$2,0($2)
	srl	$2,$2,16
	andi	$3,$2,0xffff
	lw	$2,4($sp)
	sh	$3,8($2)
	lw	$2,0($sp)
	addiu	$2,$2,172
	sw	$2,28($sp)
	lw	$2,28($sp)
	lw	$2,0($2)
	andi	$3,$2,0xffff
	lw	$2,4($sp)
	sh	$3,10($2)
	move	$2,$0
	addiu	$sp,$sp,32
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	qdma_get_txq_cngst_dynamic_threshold
	.size	qdma_get_txq_cngst_dynamic_threshold, .-qdma_get_txq_cngst_dynamic_threshold
	.section	.text.qdma_set_txq_cngst_total_threshold,"ax",@progbits
	.align	2
	.globl	qdma_set_txq_cngst_total_threshold
	.set	nomips16
	.set	nomicromips
	.ent	qdma_set_txq_cngst_total_threshold
	.type	qdma_set_txq_cngst_total_threshold, @function
qdma_set_txq_cngst_total_threshold:
	.frame	$sp,40,$31		# vars= 40, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-40
	sw	$4,40($sp)
	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lw	$2,0($2)
	sw	$2,0($sp)
	lw	$2,40($sp)
	lw	$2,8($2)
	sw	$2,4($sp)
	lw	$2,4($sp)
	lw	$3,4($2)
	lw	$2,4($sp)
	lw	$2,0($2)
	sltu	$2,$2,$3
	beq	$2,$0,$L614
	nop

	li	$2,-22			# 0xffffffffffffffea
	j	$L615
	nop

$L614:
	lw	$2,0($sp)
	addiu	$2,$2,164
	sw	$2,16($sp)
	lw	$2,16($sp)
	lw	$2,0($2)
	sw	$2,8($sp)
	lw	$2,0($sp)
	addiu	$3,$2,164
	lw	$2,8($sp)
	andi	$4,$2,0xffff
	lw	$2,4($sp)
	lw	$2,0($2)
	sll	$2,$2,18
	or	$2,$4,$2
	sw	$3,20($sp)
	sw	$2,24($sp)
	lw	$2,20($sp)
	lw	$3,24($sp)
	sw	$3,0($2)
	lw	$2,0($sp)
	addiu	$2,$2,164
	sw	$2,28($sp)
	lw	$2,28($sp)
	lw	$2,0($2)
	sw	$2,12($sp)
	lw	$2,0($sp)
	addiu	$3,$2,164
	lw	$4,12($sp)
	li	$2,-65536			# 0xffffffffffff0000
	and	$4,$4,$2
	lw	$2,4($sp)
	lw	$2,4($2)
	sll	$2,$2,2
	andi	$2,$2,0xffff
	or	$2,$4,$2
	sw	$3,32($sp)
	sw	$2,36($sp)
	lw	$2,32($sp)
	lw	$3,36($sp)
	sw	$3,0($2)
	move	$2,$0
$L615:
	addiu	$sp,$sp,40
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	qdma_set_txq_cngst_total_threshold
	.size	qdma_set_txq_cngst_total_threshold, .-qdma_set_txq_cngst_total_threshold
	.section	.text.qdma_get_txq_cngst_total_threshold,"ax",@progbits
	.align	2
	.globl	qdma_get_txq_cngst_total_threshold
	.set	nomips16
	.set	nomicromips
	.ent	qdma_get_txq_cngst_total_threshold
	.type	qdma_get_txq_cngst_total_threshold, @function
qdma_get_txq_cngst_total_threshold:
	.frame	$sp,16,$31		# vars= 16, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-16
	sw	$4,16($sp)
	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lw	$2,0($2)
	sw	$2,0($sp)
	lw	$2,16($sp)
	lw	$2,8($2)
	sw	$2,4($sp)
	lw	$2,0($sp)
	addiu	$2,$2,164
	sw	$2,8($sp)
	lw	$2,8($sp)
	lw	$2,0($2)
	srl	$3,$2,18
	lw	$2,4($sp)
	sw	$3,0($2)
	lw	$2,0($sp)
	addiu	$2,$2,164
	sw	$2,12($sp)
	lw	$2,12($sp)
	lw	$2,0($2)
	andi	$2,$2,0xffff
	srl	$3,$2,2
	lw	$2,4($sp)
	sw	$3,4($2)
	move	$2,$0
	addiu	$sp,$sp,16
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	qdma_get_txq_cngst_total_threshold
	.size	qdma_get_txq_cngst_total_threshold, .-qdma_get_txq_cngst_total_threshold
	.section	.text.qdma_set_txq_cngst_channel_threshold,"ax",@progbits
	.align	2
	.globl	qdma_set_txq_cngst_channel_threshold
	.set	nomips16
	.set	nomicromips
	.ent	qdma_set_txq_cngst_channel_threshold
	.type	qdma_set_txq_cngst_channel_threshold, @function
qdma_set_txq_cngst_channel_threshold:
	.frame	$sp,40,$31		# vars= 40, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-40
	sw	$4,40($sp)
	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lw	$2,0($2)
	sw	$2,0($sp)
	lw	$2,40($sp)
	lw	$2,8($2)
	sw	$2,4($sp)
	lw	$2,4($sp)
	lw	$3,4($2)
	lw	$2,4($sp)
	lw	$2,0($2)
	sltu	$2,$2,$3
	beq	$2,$0,$L623
	nop

	li	$2,-22			# 0xffffffffffffffea
	j	$L624
	nop

$L623:
	lw	$2,0($sp)
	addiu	$2,$2,168
	sw	$2,16($sp)
	lw	$2,16($sp)
	lw	$2,0($2)
	sw	$2,8($sp)
	lw	$2,0($sp)
	addiu	$3,$2,168
	lw	$2,8($sp)
	andi	$4,$2,0xffff
	lw	$2,4($sp)
	lw	$2,0($2)
	sll	$2,$2,18
	or	$2,$4,$2
	sw	$3,20($sp)
	sw	$2,24($sp)
	lw	$2,20($sp)
	lw	$3,24($sp)
	sw	$3,0($2)
	lw	$2,0($sp)
	addiu	$2,$2,168
	sw	$2,28($sp)
	lw	$2,28($sp)
	lw	$2,0($2)
	sw	$2,12($sp)
	lw	$2,0($sp)
	addiu	$3,$2,168
	lw	$4,12($sp)
	li	$2,-65536			# 0xffffffffffff0000
	and	$4,$4,$2
	lw	$2,4($sp)
	lw	$2,4($2)
	sll	$2,$2,2
	andi	$2,$2,0xffff
	or	$2,$4,$2
	sw	$3,32($sp)
	sw	$2,36($sp)
	lw	$2,32($sp)
	lw	$3,36($sp)
	sw	$3,0($2)
	move	$2,$0
$L624:
	addiu	$sp,$sp,40
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	qdma_set_txq_cngst_channel_threshold
	.size	qdma_set_txq_cngst_channel_threshold, .-qdma_set_txq_cngst_channel_threshold
	.section	.text.qdma_get_txq_cngst_channel_threshold,"ax",@progbits
	.align	2
	.globl	qdma_get_txq_cngst_channel_threshold
	.set	nomips16
	.set	nomicromips
	.ent	qdma_get_txq_cngst_channel_threshold
	.type	qdma_get_txq_cngst_channel_threshold, @function
qdma_get_txq_cngst_channel_threshold:
	.frame	$sp,16,$31		# vars= 16, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-16
	sw	$4,16($sp)
	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lw	$2,0($2)
	sw	$2,0($sp)
	lw	$2,16($sp)
	lw	$2,8($2)
	sw	$2,4($sp)
	lw	$2,0($sp)
	addiu	$2,$2,168
	sw	$2,8($sp)
	lw	$2,8($sp)
	lw	$2,0($2)
	srl	$3,$2,18
	lw	$2,4($sp)
	sw	$3,0($2)
	lw	$2,0($sp)
	addiu	$2,$2,168
	sw	$2,12($sp)
	lw	$2,12($sp)
	lw	$2,0($2)
	andi	$2,$2,0xffff
	srl	$3,$2,2
	lw	$2,4($sp)
	sw	$3,4($2)
	move	$2,$0
	addiu	$sp,$sp,16
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	qdma_get_txq_cngst_channel_threshold
	.size	qdma_get_txq_cngst_channel_threshold, .-qdma_get_txq_cngst_channel_threshold
	.section	.text.qdma_set_txq_cngst_queue_threshold,"ax",@progbits
	.align	2
	.globl	qdma_set_txq_cngst_queue_threshold
	.set	nomips16
	.set	nomicromips
	.ent	qdma_set_txq_cngst_queue_threshold
	.type	qdma_set_txq_cngst_queue_threshold, @function
qdma_set_txq_cngst_queue_threshold:
	.frame	$sp,40,$31		# vars= 40, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-40
	sw	$4,40($sp)
	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lw	$2,0($2)
	sw	$2,0($sp)
	lw	$2,40($sp)
	lw	$2,8($2)
	sw	$2,4($sp)
	lw	$2,4($sp)
	lw	$3,4($2)
	lw	$2,4($sp)
	lw	$2,0($2)
	sltu	$2,$2,$3
	beq	$2,$0,$L632
	nop

	li	$2,-22			# 0xffffffffffffffea
	j	$L633
	nop

$L632:
	lw	$2,0($sp)
	addiu	$2,$2,172
	sw	$2,16($sp)
	lw	$2,16($sp)
	lw	$2,0($2)
	sw	$2,8($sp)
	lw	$2,0($sp)
	addiu	$3,$2,172
	lw	$2,8($sp)
	andi	$4,$2,0xffff
	lw	$2,4($sp)
	lw	$2,0($2)
	sll	$2,$2,18
	or	$2,$4,$2
	sw	$3,20($sp)
	sw	$2,24($sp)
	lw	$2,20($sp)
	lw	$3,24($sp)
	sw	$3,0($2)
	lw	$2,0($sp)
	addiu	$2,$2,172
	sw	$2,28($sp)
	lw	$2,28($sp)
	lw	$2,0($2)
	sw	$2,12($sp)
	lw	$2,0($sp)
	addiu	$3,$2,172
	lw	$4,12($sp)
	li	$2,-65536			# 0xffffffffffff0000
	and	$4,$4,$2
	lw	$2,4($sp)
	lw	$2,4($2)
	sll	$2,$2,2
	andi	$2,$2,0xffff
	or	$2,$4,$2
	sw	$3,32($sp)
	sw	$2,36($sp)
	lw	$2,32($sp)
	lw	$3,36($sp)
	sw	$3,0($2)
	move	$2,$0
$L633:
	addiu	$sp,$sp,40
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	qdma_set_txq_cngst_queue_threshold
	.size	qdma_set_txq_cngst_queue_threshold, .-qdma_set_txq_cngst_queue_threshold
	.section	.text.qdma_get_txq_cngst_queue_threshold,"ax",@progbits
	.align	2
	.globl	qdma_get_txq_cngst_queue_threshold
	.set	nomips16
	.set	nomicromips
	.ent	qdma_get_txq_cngst_queue_threshold
	.type	qdma_get_txq_cngst_queue_threshold, @function
qdma_get_txq_cngst_queue_threshold:
	.frame	$sp,16,$31		# vars= 16, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-16
	sw	$4,16($sp)
	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lw	$2,0($2)
	sw	$2,0($sp)
	lw	$2,16($sp)
	lw	$2,8($2)
	sw	$2,4($sp)
	lw	$2,0($sp)
	addiu	$2,$2,172
	sw	$2,8($sp)
	lw	$2,8($sp)
	lw	$2,0($2)
	srl	$3,$2,18
	lw	$2,4($sp)
	sw	$3,0($2)
	lw	$2,0($sp)
	addiu	$2,$2,172
	sw	$2,12($sp)
	lw	$2,12($sp)
	lw	$2,0($2)
	andi	$2,$2,0xffff
	srl	$3,$2,2
	lw	$2,4($sp)
	sw	$3,4($2)
	move	$2,$0
	addiu	$sp,$sp,16
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	qdma_get_txq_cngst_queue_threshold
	.size	qdma_get_txq_cngst_queue_threshold, .-qdma_get_txq_cngst_queue_threshold
	.section	.text.qdma_set_txq_cngst_queue_nonblocking,"ax",@progbits
	.align	2
	.globl	qdma_set_txq_cngst_queue_nonblocking
	.set	nomips16
	.set	nomicromips
	.ent	qdma_set_txq_cngst_queue_nonblocking
	.type	qdma_set_txq_cngst_queue_nonblocking, @function
qdma_set_txq_cngst_queue_nonblocking:
	.frame	$sp,40,$31		# vars= 40, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-40
	sw	$4,40($sp)
	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lw	$2,0($2)
	sw	$2,0($sp)
	lw	$2,40($sp)
	lw	$2,8($2)
	sw	$2,4($sp)
	lw	$2,4($sp)
	lw	$2,0($2)
	bne	$2,$0,$L641
	nop

	lw	$2,0($sp)
	addiu	$2,$2,1596
	sw	$2,16($sp)
	lw	$2,16($sp)
	lw	$2,0($2)
	sw	$2,8($sp)
	lw	$2,0($sp)
	addiu	$3,$2,1596
	lw	$2,4($sp)
	lw	$2,4($2)
	move	$4,$2
	li	$2,1			# 0x1
	sll	$2,$2,$4
	nor	$2,$0,$2
	move	$4,$2
	lw	$2,8($sp)
	and	$2,$4,$2
	sw	$3,20($sp)
	sw	$2,24($sp)
	lw	$2,20($sp)
	lw	$3,24($sp)
	sw	$3,0($2)
	j	$L643
	nop

$L641:
	lw	$2,0($sp)
	addiu	$2,$2,1596
	sw	$2,28($sp)
	lw	$2,28($sp)
	lw	$2,0($2)
	sw	$2,12($sp)
	lw	$2,0($sp)
	addiu	$3,$2,1596
	lw	$2,4($sp)
	lw	$2,4($2)
	move	$4,$2
	li	$2,1			# 0x1
	sll	$2,$2,$4
	move	$4,$2
	lw	$2,12($sp)
	or	$2,$4,$2
	sw	$3,32($sp)
	sw	$2,36($sp)
	lw	$2,32($sp)
	lw	$3,36($sp)
	sw	$3,0($2)
$L643:
	move	$2,$0
	addiu	$sp,$sp,40
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	qdma_set_txq_cngst_queue_nonblocking
	.size	qdma_set_txq_cngst_queue_nonblocking, .-qdma_set_txq_cngst_queue_nonblocking
	.section	.text.qdma_get_txq_cngst_queue_nonblocking,"ax",@progbits
	.align	2
	.globl	qdma_get_txq_cngst_queue_nonblocking
	.set	nomips16
	.set	nomicromips
	.ent	qdma_get_txq_cngst_queue_nonblocking
	.type	qdma_get_txq_cngst_queue_nonblocking, @function
qdma_get_txq_cngst_queue_nonblocking:
	.frame	$sp,16,$31		# vars= 16, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-16
	sw	$4,16($sp)
	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lw	$2,0($2)
	sw	$2,0($sp)
	lw	$2,16($sp)
	lw	$2,8($2)
	sw	$2,4($sp)
	lw	$2,0($sp)
	addiu	$2,$2,1596
	sw	$2,8($sp)
	lw	$2,8($sp)
	lw	$2,0($2)
	lw	$3,4($sp)
	lw	$3,4($3)
	move	$4,$3
	li	$3,1			# 0x1
	sll	$3,$3,$4
	and	$2,$2,$3
	beq	$2,$0,$L648
	nop

	lw	$2,4($sp)
	li	$3,1			# 0x1
	sw	$3,0($2)
	j	$L649
	nop

$L648:
	lw	$2,4($sp)
	sw	$0,0($2)
$L649:
	move	$2,$0
	addiu	$sp,$sp,16
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	qdma_get_txq_cngst_queue_nonblocking
	.size	qdma_get_txq_cngst_queue_nonblocking, .-qdma_get_txq_cngst_queue_nonblocking
	.section	.text.qdma_set_txq_cngst_channel_nonblocking,"ax",@progbits
	.align	2
	.globl	qdma_set_txq_cngst_channel_nonblocking
	.set	nomips16
	.set	nomicromips
	.ent	qdma_set_txq_cngst_channel_nonblocking
	.type	qdma_set_txq_cngst_channel_nonblocking, @function
qdma_set_txq_cngst_channel_nonblocking:
	.frame	$sp,40,$31		# vars= 40, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-40
	sw	$4,40($sp)
	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lw	$2,0($2)
	sw	$2,0($sp)
	lw	$2,40($sp)
	lw	$2,8($2)
	sw	$2,4($sp)
	lw	$2,4($sp)
	lw	$2,0($2)
	bne	$2,$0,$L652
	nop

	lw	$2,0($sp)
	addiu	$2,$2,1600
	sw	$2,16($sp)
	lw	$2,16($sp)
	lw	$2,0($2)
	sw	$2,8($sp)
	lw	$2,0($sp)
	addiu	$3,$2,1600
	lw	$2,4($sp)
	lw	$2,4($2)
	move	$4,$2
	li	$2,1			# 0x1
	sll	$2,$2,$4
	nor	$2,$0,$2
	move	$4,$2
	lw	$2,8($sp)
	and	$2,$4,$2
	sw	$3,20($sp)
	sw	$2,24($sp)
	lw	$2,20($sp)
	lw	$3,24($sp)
	sw	$3,0($2)
	j	$L654
	nop

$L652:
	lw	$2,0($sp)
	addiu	$2,$2,1600
	sw	$2,28($sp)
	lw	$2,28($sp)
	lw	$2,0($2)
	sw	$2,12($sp)
	lw	$2,0($sp)
	addiu	$3,$2,1600
	lw	$2,4($sp)
	lw	$2,4($2)
	move	$4,$2
	li	$2,1			# 0x1
	sll	$2,$2,$4
	move	$4,$2
	lw	$2,12($sp)
	or	$2,$4,$2
	sw	$3,32($sp)
	sw	$2,36($sp)
	lw	$2,32($sp)
	lw	$3,36($sp)
	sw	$3,0($2)
$L654:
	move	$2,$0
	addiu	$sp,$sp,40
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	qdma_set_txq_cngst_channel_nonblocking
	.size	qdma_set_txq_cngst_channel_nonblocking, .-qdma_set_txq_cngst_channel_nonblocking
	.section	.text.qdma_get_txq_cngst_channel_nonblocking,"ax",@progbits
	.align	2
	.globl	qdma_get_txq_cngst_channel_nonblocking
	.set	nomips16
	.set	nomicromips
	.ent	qdma_get_txq_cngst_channel_nonblocking
	.type	qdma_get_txq_cngst_channel_nonblocking, @function
qdma_get_txq_cngst_channel_nonblocking:
	.frame	$sp,16,$31		# vars= 16, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-16
	sw	$4,16($sp)
	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lw	$2,0($2)
	sw	$2,0($sp)
	lw	$2,16($sp)
	lw	$2,8($2)
	sw	$2,4($sp)
	lw	$2,0($sp)
	addiu	$2,$2,1600
	sw	$2,8($sp)
	lw	$2,8($sp)
	lw	$2,0($2)
	lw	$3,4($sp)
	lw	$3,4($3)
	move	$4,$3
	li	$3,1			# 0x1
	sll	$3,$3,$4
	and	$2,$2,$3
	beq	$2,$0,$L659
	nop

	lw	$2,4($sp)
	li	$3,1			# 0x1
	sw	$3,0($2)
	j	$L660
	nop

$L659:
	lw	$2,4($sp)
	sw	$0,0($2)
$L660:
	move	$2,$0
	addiu	$sp,$sp,16
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	qdma_get_txq_cngst_channel_nonblocking
	.size	qdma_get_txq_cngst_channel_nonblocking, .-qdma_get_txq_cngst_channel_nonblocking
	.section	.text.qdma_set_txq_peekrate_params,"ax",@progbits
	.align	2
	.globl	qdma_set_txq_peekrate_params
	.set	nomips16
	.set	nomicromips
	.ent	qdma_set_txq_peekrate_params
	.type	qdma_set_txq_peekrate_params, @function
qdma_set_txq_peekrate_params:
	.frame	$sp,72,$31		# vars= 72, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-72
	sw	$4,72($sp)
	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lw	$2,0($2)
	sw	$2,0($sp)
	lw	$2,72($sp)
	lw	$2,8($2)
	sw	$2,4($sp)
	lw	$2,4($sp)
	lw	$3,0($2)
	li	$2,1			# 0x1
	bne	$3,$2,$L663
	nop

	lw	$2,0($sp)
	addiu	$2,$2,148
	sw	$2,24($sp)
	lw	$2,24($sp)
	lw	$2,0($2)
	sw	$2,8($sp)
	lw	$2,0($sp)
	addiu	$3,$2,148
	lw	$4,8($sp)
	li	$2,1073741824			# 0x40000000
	or	$2,$4,$2
	sw	$3,28($sp)
	sw	$2,32($sp)
	lw	$2,28($sp)
	lw	$3,32($sp)
	sw	$3,0($2)
	j	$L665
	nop

$L663:
	lw	$2,0($sp)
	addiu	$2,$2,148
	sw	$2,36($sp)
	lw	$2,36($sp)
	lw	$2,0($2)
	sw	$2,12($sp)
	lw	$2,0($sp)
	addiu	$3,$2,148
	lw	$4,12($sp)
	li	$2,-1073807360			# 0xffffffffbfff0000
	ori	$2,$2,0xffff
	and	$2,$4,$2
	sw	$3,40($sp)
	sw	$2,44($sp)
	lw	$2,40($sp)
	lw	$3,44($sp)
	sw	$3,0($2)
$L665:
	lw	$2,0($sp)
	addiu	$2,$2,160
	sw	$2,48($sp)
	lw	$2,48($sp)
	lw	$2,0($2)
	sw	$2,16($sp)
	lw	$2,0($sp)
	addiu	$3,$2,160
	lw	$4,16($sp)
	li	$2,-12648448			# 0xffffffffff3f0000
	ori	$2,$2,0xffff
	and	$4,$4,$2
	lw	$2,4($sp)
	lw	$2,4($2)
	sll	$5,$2,22
	li	$2,12582912			# 0xc00000
	and	$2,$5,$2
	or	$2,$4,$2
	sw	$3,52($sp)
	sw	$2,56($sp)
	lw	$2,52($sp)
	lw	$3,56($sp)
	sw	$3,0($2)
	lw	$2,0($sp)
	addiu	$2,$2,148
	sw	$2,60($sp)
	lw	$2,60($sp)
	lw	$2,0($2)
	sw	$2,20($sp)
	lw	$2,0($sp)
	addiu	$3,$2,148
	lw	$4,20($sp)
	li	$2,-267452416			# 0xfffffffff00f0000
	ori	$2,$2,0xffff
	and	$4,$4,$2
	lw	$2,4($sp)
	lw	$2,8($2)
	srl	$2,$2,4
	sll	$5,$2,20
	li	$2,267386880			# 0xff00000
	and	$2,$5,$2
	or	$2,$4,$2
	sw	$3,64($sp)
	sw	$2,68($sp)
	lw	$2,64($sp)
	lw	$3,68($sp)
	sw	$3,0($2)
	move	$2,$0
	addiu	$sp,$sp,72
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	qdma_set_txq_peekrate_params
	.size	qdma_set_txq_peekrate_params, .-qdma_set_txq_peekrate_params
	.section	.text.qdma_get_txq_peekrate_params,"ax",@progbits
	.align	2
	.globl	qdma_get_txq_peekrate_params
	.set	nomips16
	.set	nomicromips
	.ent	qdma_get_txq_peekrate_params
	.type	qdma_get_txq_peekrate_params, @function
qdma_get_txq_peekrate_params:
	.frame	$sp,24,$31		# vars= 24, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-24
	sw	$4,24($sp)
	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lw	$2,0($2)
	sw	$2,0($sp)
	lw	$2,24($sp)
	lw	$2,8($2)
	sw	$2,4($sp)
	lw	$2,0($sp)
	addiu	$2,$2,148
	sw	$2,8($sp)
	lw	$2,8($sp)
	lw	$3,0($2)
	li	$2,1073741824			# 0x40000000
	and	$2,$3,$2
	beq	$2,$0,$L672
	nop

	lw	$2,4($sp)
	li	$3,1			# 0x1
	sw	$3,0($2)
	j	$L673
	nop

$L672:
	lw	$2,4($sp)
	sw	$0,0($2)
$L673:
	lw	$2,0($sp)
	addiu	$2,$2,160
	sw	$2,12($sp)
	lw	$2,12($sp)
	lw	$3,0($2)
	li	$2,12582912			# 0xc00000
	and	$2,$3,$2
	srl	$3,$2,22
	lw	$2,4($sp)
	sw	$3,4($2)
	lw	$2,0($sp)
	addiu	$2,$2,148
	sw	$2,16($sp)
	lw	$2,16($sp)
	lw	$3,0($2)
	li	$2,267386880			# 0xff00000
	and	$2,$3,$2
	srl	$2,$2,20
	sll	$3,$2,4
	lw	$2,4($sp)
	sw	$3,8($2)
	move	$2,$0
	addiu	$sp,$sp,24
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	qdma_get_txq_peekrate_params
	.size	qdma_get_txq_peekrate_params, .-qdma_get_txq_peekrate_params
	.section	.text.qdma_set_txq_cngst_static_queue_normal_threshold,"ax",@progbits
	.align	2
	.globl	qdma_set_txq_cngst_static_queue_normal_threshold
	.set	nomips16
	.set	nomicromips
	.ent	qdma_set_txq_cngst_static_queue_normal_threshold
	.type	qdma_set_txq_cngst_static_queue_normal_threshold, @function
qdma_set_txq_cngst_static_queue_normal_threshold:
	.frame	$sp,32,$31		# vars= 32, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-32
	sw	$4,32($sp)
	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lw	$2,0($2)
	sw	$2,4($sp)
	sw	$0,0($sp)
	lw	$2,32($sp)
	lw	$2,8($2)
	sw	$2,8($sp)
	sw	$0,0($sp)
	j	$L678
	nop

$L680:
	lw	$2,0($sp)
	sll	$2,$2,2
	move	$3,$2
	lw	$2,4($sp)
	addu	$2,$3,$2
	addiu	$2,$2,176
	sw	$2,16($sp)
	lw	$2,16($sp)
	lw	$2,0($2)
	sw	$2,12($sp)
	lw	$2,0($sp)
	sll	$2,$2,2
	move	$3,$2
	lw	$2,4($sp)
	addu	$2,$3,$2
	addiu	$3,$2,176
	lw	$2,12($sp)
	andi	$4,$2,0xffff
	lw	$5,8($sp)
	lw	$2,0($sp)
	sll	$2,$2,2
	addu	$2,$5,$2
	lw	$2,0($2)
	sll	$2,$2,18
	or	$2,$4,$2
	sw	$3,20($sp)
	sw	$2,24($sp)
	lw	$2,20($sp)
	lw	$3,24($sp)
	sw	$3,0($2)
	lw	$2,0($sp)
	addiu	$2,$2,1
	sw	$2,0($sp)
$L678:
	lw	$2,0($sp)
	slt	$2,$2,8
	bne	$2,$0,$L680
	nop

	move	$2,$0
	addiu	$sp,$sp,32
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	qdma_set_txq_cngst_static_queue_normal_threshold
	.size	qdma_set_txq_cngst_static_queue_normal_threshold, .-qdma_set_txq_cngst_static_queue_normal_threshold
	.section	.text.qdma_set_txq_cngst_static_queue_dei_threshold,"ax",@progbits
	.align	2
	.globl	qdma_set_txq_cngst_static_queue_dei_threshold
	.set	nomips16
	.set	nomicromips
	.ent	qdma_set_txq_cngst_static_queue_dei_threshold
	.type	qdma_set_txq_cngst_static_queue_dei_threshold, @function
qdma_set_txq_cngst_static_queue_dei_threshold:
	.frame	$sp,32,$31		# vars= 32, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-32
	sw	$4,32($sp)
	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lw	$2,0($2)
	sw	$2,4($sp)
	sw	$0,0($sp)
	lw	$2,32($sp)
	lw	$2,8($2)
	sw	$2,8($sp)
	sw	$0,0($sp)
	j	$L683
	nop

$L685:
	lw	$2,0($sp)
	sll	$2,$2,2
	move	$3,$2
	lw	$2,4($sp)
	addu	$2,$3,$2
	addiu	$2,$2,176
	sw	$2,16($sp)
	lw	$2,16($sp)
	lw	$2,0($2)
	sw	$2,12($sp)
	lw	$2,0($sp)
	sll	$2,$2,2
	move	$3,$2
	lw	$2,4($sp)
	addu	$2,$3,$2
	addiu	$3,$2,176
	lw	$2,12($sp)
	andi	$4,$2,0xffff
	lw	$5,8($sp)
	lw	$2,0($sp)
	sll	$2,$2,2
	addu	$2,$5,$2
	lw	$2,0($2)
	sll	$2,$2,18
	or	$2,$4,$2
	sw	$3,20($sp)
	sw	$2,24($sp)
	lw	$2,20($sp)
	lw	$3,24($sp)
	sw	$3,0($2)
	lw	$2,0($sp)
	addiu	$2,$2,1
	sw	$2,0($sp)
$L683:
	lw	$2,0($sp)
	slt	$2,$2,8
	bne	$2,$0,$L685
	nop

	move	$2,$0
	addiu	$sp,$sp,32
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	qdma_set_txq_cngst_static_queue_dei_threshold
	.size	qdma_set_txq_cngst_static_queue_dei_threshold, .-qdma_set_txq_cngst_static_queue_dei_threshold
	.section	.text.qdma_get_txq_cngst_dynamic_info,"ax",@progbits
	.align	2
	.globl	qdma_get_txq_cngst_dynamic_info
	.set	nomips16
	.set	nomicromips
	.ent	qdma_get_txq_cngst_dynamic_info
	.type	qdma_get_txq_cngst_dynamic_info, @function
qdma_get_txq_cngst_dynamic_info:
	.frame	$sp,56,$31		# vars= 56, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-56
	sw	$4,56($sp)
	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lw	$2,0($2)
	sw	$2,0($sp)
	lw	$2,56($sp)
	lw	$2,8($2)
	lw	$2,0($2)
	sw	$2,4($sp)
	lw	$2,56($sp)
	lw	$2,8($2)
	lw	$2,4($2)
	sw	$2,8($sp)
	lw	$2,56($sp)
	lw	$2,8($2)
	lw	$2,8($2)
	sw	$2,12($sp)
	lw	$2,56($sp)
	lw	$2,8($2)
	lw	$2,12($2)
	sw	$2,16($sp)
	lw	$2,0($sp)
	addiu	$2,$2,164
	sw	$2,20($sp)
	lw	$2,20($sp)
	lw	$2,0($2)
	srl	$3,$2,18
	lw	$2,4($sp)
	sw	$3,0($2)
	lw	$2,0($sp)
	addiu	$2,$2,164
	sw	$2,24($sp)
	lw	$2,24($sp)
	lw	$2,0($2)
	andi	$2,$2,0xffff
	srl	$3,$2,2
	lw	$2,4($sp)
	sw	$3,4($2)
	lw	$2,0($sp)
	addiu	$2,$2,168
	sw	$2,28($sp)
	lw	$2,28($sp)
	lw	$2,0($2)
	srl	$3,$2,18
	lw	$2,8($sp)
	sw	$3,0($2)
	lw	$2,0($sp)
	addiu	$2,$2,168
	sw	$2,32($sp)
	lw	$2,32($sp)
	lw	$2,0($2)
	andi	$2,$2,0xffff
	srl	$3,$2,2
	lw	$2,8($sp)
	sw	$3,4($2)
	lw	$2,0($sp)
	addiu	$2,$2,172
	sw	$2,36($sp)
	lw	$2,36($sp)
	lw	$2,0($2)
	srl	$3,$2,18
	lw	$2,12($sp)
	sw	$3,0($2)
	lw	$2,0($sp)
	addiu	$2,$2,172
	sw	$2,40($sp)
	lw	$2,40($sp)
	lw	$2,0($2)
	andi	$2,$2,0xffff
	srl	$3,$2,2
	lw	$2,12($sp)
	sw	$3,4($2)
	lw	$2,0($sp)
	addiu	$2,$2,148
	sw	$2,44($sp)
	lw	$2,44($sp)
	lw	$3,0($2)
	li	$2,1073741824			# 0x40000000
	and	$2,$3,$2
	beq	$2,$0,$L695
	nop

	lw	$2,16($sp)
	li	$3,1			# 0x1
	sw	$3,0($2)
	j	$L696
	nop

$L695:
	lw	$2,16($sp)
	sw	$0,0($2)
$L696:
	lw	$2,0($sp)
	addiu	$2,$2,160
	sw	$2,48($sp)
	lw	$2,48($sp)
	lw	$3,0($2)
	li	$2,12582912			# 0xc00000
	and	$2,$3,$2
	srl	$3,$2,22
	lw	$2,16($sp)
	sw	$3,4($2)
	lw	$2,0($sp)
	addiu	$2,$2,148
	sw	$2,52($sp)
	lw	$2,52($sp)
	lw	$3,0($2)
	li	$2,267386880			# 0xff00000
	and	$2,$3,$2
	srl	$2,$2,20
	sll	$3,$2,4
	lw	$2,16($sp)
	sw	$3,8($2)
	move	$2,$0
	addiu	$sp,$sp,56
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	qdma_get_txq_cngst_dynamic_info
	.size	qdma_get_txq_cngst_dynamic_info, .-qdma_get_txq_cngst_dynamic_info
	.section	.text.qdma_get_txq_cngst_static_info,"ax",@progbits
	.align	2
	.globl	qdma_get_txq_cngst_static_info
	.set	nomips16
	.set	nomicromips
	.ent	qdma_get_txq_cngst_static_info
	.type	qdma_get_txq_cngst_static_info, @function
qdma_get_txq_cngst_static_info:
	.frame	$sp,40,$31		# vars= 40, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-40
	sw	$4,40($sp)
	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lw	$2,0($2)
	sw	$2,4($sp)
	sw	$0,0($sp)
	lw	$2,40($sp)
	lw	$2,8($2)
	lw	$2,0($2)
	sw	$2,8($sp)
	lw	$2,40($sp)
	lw	$2,8($2)
	lw	$2,8($2)
	sw	$2,12($sp)
	lw	$2,40($sp)
	lw	$2,8($2)
	lw	$2,4($2)
	sw	$2,16($sp)
	lw	$2,4($sp)
	addiu	$2,$2,164
	sw	$2,20($sp)
	lw	$2,20($sp)
	lw	$2,0($2)
	srl	$3,$2,18
	lw	$2,8($sp)
	sw	$3,0($2)
	lw	$2,4($sp)
	addiu	$2,$2,164
	sw	$2,24($sp)
	lw	$2,24($sp)
	lw	$2,0($2)
	andi	$2,$2,0xffff
	srl	$3,$2,2
	lw	$2,8($sp)
	sw	$3,4($2)
	sw	$0,0($sp)
	j	$L703
	nop

$L705:
	lw	$2,0($sp)
	sll	$2,$2,2
	move	$3,$2
	lw	$2,4($sp)
	addu	$2,$3,$2
	addiu	$2,$2,176
	sw	$2,28($sp)
	lw	$2,28($sp)
	lw	$2,0($2)
	srl	$3,$2,18
	lw	$4,12($sp)
	lw	$2,0($sp)
	sll	$2,$2,2
	addu	$2,$4,$2
	sw	$3,0($2)
	lw	$2,0($sp)
	addiu	$2,$2,1
	sw	$2,0($sp)
$L703:
	lw	$2,0($sp)
	slt	$2,$2,8
	bne	$2,$0,$L705
	nop

	sw	$0,0($sp)
	j	$L706
	nop

$L708:
	lw	$2,0($sp)
	sll	$2,$2,2
	move	$3,$2
	lw	$2,4($sp)
	addu	$2,$3,$2
	addiu	$2,$2,176
	sw	$2,32($sp)
	lw	$2,32($sp)
	lw	$2,0($2)
	andi	$2,$2,0xffff
	srl	$3,$2,2
	lw	$4,16($sp)
	lw	$2,0($sp)
	sll	$2,$2,2
	addu	$2,$4,$2
	sw	$3,0($2)
	lw	$2,0($sp)
	addiu	$2,$2,1
	sw	$2,0($sp)
$L706:
	lw	$2,0($sp)
	slt	$2,$2,8
	bne	$2,$0,$L708
	nop

	move	$2,$0
	addiu	$sp,$sp,40
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	qdma_get_txq_cngst_static_info
	.size	qdma_get_txq_cngst_static_info, .-qdma_get_txq_cngst_static_info
	.section	.text.qdma_set_txq_cngst_cfg,"ax",@progbits
	.align	2
	.globl	qdma_set_txq_cngst_cfg
	.set	nomips16
	.set	nomicromips
	.ent	qdma_set_txq_cngst_cfg
	.type	qdma_set_txq_cngst_cfg, @function
qdma_set_txq_cngst_cfg:
	.frame	$sp,360,$31		# vars= 360, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-360
	sw	$4,360($sp)
	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lw	$2,0($2)
	sw	$2,0($sp)
	lw	$2,360($sp)
	lw	$2,12($2)
	sltu	$2,$2,4
	bne	$2,$0,$L711
	nop

	li	$2,-22			# 0xffffffffffffffea
	j	$L712
	nop

$L711:
	lw	$2,360($sp)
	lbu	$2,0($2)
	bne	$2,$0,$L713
	nop

	lw	$2,0($sp)
	addiu	$2,$2,160
	sw	$2,92($sp)
	lw	$2,92($sp)
	lw	$2,0($2)
	sw	$2,4($sp)
	lw	$2,0($sp)
	addiu	$3,$2,160
	lw	$2,4($sp)
	ext	$2,$2,0,31
	sw	$3,96($sp)
	sw	$2,100($sp)
	lw	$2,96($sp)
	lw	$3,100($sp)
	sw	$3,0($2)
	j	$L715
	nop

$L713:
	lw	$2,0($sp)
	addiu	$2,$2,160
	sw	$2,104($sp)
	lw	$2,104($sp)
	lw	$2,0($2)
	sw	$2,8($sp)
	lw	$2,0($sp)
	addiu	$3,$2,160
	lw	$4,8($sp)
	li	$2,-2147483648			# 0xffffffff80000000
	or	$2,$4,$2
	sw	$3,108($sp)
	sw	$2,112($sp)
	lw	$2,108($sp)
	lw	$3,112($sp)
	sw	$3,0($2)
$L715:
	lw	$2,360($sp)
	lbu	$2,1($2)
	bne	$2,$0,$L717
	nop

	lw	$2,0($sp)
	addiu	$2,$2,160
	sw	$2,116($sp)
	lw	$2,116($sp)
	lw	$2,0($2)
	sw	$2,12($sp)
	lw	$2,0($sp)
	addiu	$3,$2,160
	lw	$4,12($sp)
	li	$2,-1073807360			# 0xffffffffbfff0000
	ori	$2,$2,0xffff
	and	$2,$4,$2
	sw	$3,120($sp)
	sw	$2,124($sp)
	lw	$2,120($sp)
	lw	$3,124($sp)
	sw	$3,0($2)
	j	$L719
	nop

$L717:
	lw	$2,0($sp)
	addiu	$2,$2,160
	sw	$2,128($sp)
	lw	$2,128($sp)
	lw	$2,0($2)
	sw	$2,16($sp)
	lw	$2,0($sp)
	addiu	$3,$2,160
	lw	$4,16($sp)
	li	$2,1073741824			# 0x40000000
	or	$2,$4,$2
	sw	$3,132($sp)
	sw	$2,136($sp)
	lw	$2,132($sp)
	lw	$3,136($sp)
	sw	$3,0($2)
$L719:
	lw	$2,360($sp)
	lbu	$2,2($2)
	bne	$2,$0,$L721
	nop

	lw	$2,0($sp)
	addiu	$2,$2,160
	sw	$2,140($sp)
	lw	$2,140($sp)
	lw	$2,0($2)
	sw	$2,20($sp)
	lw	$2,0($sp)
	addiu	$3,$2,160
	lw	$4,20($sp)
	li	$2,-536936448			# 0xffffffffdfff0000
	ori	$2,$2,0xffff
	and	$2,$4,$2
	sw	$3,144($sp)
	sw	$2,148($sp)
	lw	$2,144($sp)
	lw	$3,148($sp)
	sw	$3,0($2)
	j	$L723
	nop

$L721:
	lw	$2,0($sp)
	addiu	$2,$2,160
	sw	$2,152($sp)
	lw	$2,152($sp)
	lw	$2,0($2)
	sw	$2,24($sp)
	lw	$2,0($sp)
	addiu	$3,$2,160
	lw	$4,24($sp)
	li	$2,536870912			# 0x20000000
	or	$2,$4,$2
	sw	$3,156($sp)
	sw	$2,160($sp)
	lw	$2,156($sp)
	lw	$3,160($sp)
	sw	$3,0($2)
$L723:
	lw	$2,360($sp)
	lbu	$2,3($2)
	bne	$2,$0,$L725
	nop

	lw	$2,0($sp)
	addiu	$2,$2,160
	sw	$2,164($sp)
	lw	$2,164($sp)
	lw	$2,0($2)
	sw	$2,28($sp)
	lw	$2,0($sp)
	addiu	$3,$2,160
	lw	$4,28($sp)
	li	$2,-134283264			# 0xfffffffff7ff0000
	ori	$2,$2,0xffff
	and	$2,$4,$2
	sw	$3,168($sp)
	sw	$2,172($sp)
	lw	$2,168($sp)
	lw	$3,172($sp)
	sw	$3,0($2)
	j	$L727
	nop

$L725:
	lw	$2,0($sp)
	addiu	$2,$2,160
	sw	$2,176($sp)
	lw	$2,176($sp)
	lw	$2,0($2)
	sw	$2,32($sp)
	lw	$2,0($sp)
	addiu	$3,$2,160
	lw	$4,32($sp)
	li	$2,134217728			# 0x8000000
	or	$2,$4,$2
	sw	$3,180($sp)
	sw	$2,184($sp)
	lw	$2,180($sp)
	lw	$3,184($sp)
	sw	$3,0($2)
$L727:
	lw	$2,360($sp)
	lbu	$2,4($2)
	bne	$2,$0,$L729
	nop

	lw	$2,0($sp)
	addiu	$2,$2,160
	sw	$2,188($sp)
	lw	$2,188($sp)
	lw	$2,0($2)
	sw	$2,36($sp)
	lw	$2,0($sp)
	addiu	$3,$2,160
	lw	$4,36($sp)
	li	$2,-67174400			# 0xfffffffffbff0000
	ori	$2,$2,0xffff
	and	$2,$4,$2
	sw	$3,192($sp)
	sw	$2,196($sp)
	lw	$2,192($sp)
	lw	$3,196($sp)
	sw	$3,0($2)
	j	$L731
	nop

$L729:
	lw	$2,0($sp)
	addiu	$2,$2,160
	sw	$2,200($sp)
	lw	$2,200($sp)
	lw	$2,0($2)
	sw	$2,40($sp)
	lw	$2,0($sp)
	addiu	$3,$2,160
	lw	$4,40($sp)
	li	$2,67108864			# 0x4000000
	or	$2,$4,$2
	sw	$3,204($sp)
	sw	$2,208($sp)
	lw	$2,204($sp)
	lw	$3,208($sp)
	sw	$3,0($2)
$L731:
	lw	$2,360($sp)
	lbu	$2,5($2)
	bne	$2,$0,$L733
	nop

	lw	$2,0($sp)
	addiu	$2,$2,160
	sw	$2,212($sp)
	lw	$2,212($sp)
	lw	$2,0($2)
	sw	$2,44($sp)
	lw	$2,0($sp)
	addiu	$3,$2,160
	lw	$4,44($sp)
	li	$2,-33619968			# 0xfffffffffdff0000
	ori	$2,$2,0xffff
	and	$2,$4,$2
	sw	$3,216($sp)
	sw	$2,220($sp)
	lw	$2,216($sp)
	lw	$3,220($sp)
	sw	$3,0($2)
	j	$L735
	nop

$L733:
	lw	$2,0($sp)
	addiu	$2,$2,160
	sw	$2,224($sp)
	lw	$2,224($sp)
	lw	$2,0($2)
	sw	$2,48($sp)
	lw	$2,0($sp)
	addiu	$3,$2,160
	lw	$4,48($sp)
	li	$2,33554432			# 0x2000000
	or	$2,$4,$2
	sw	$3,228($sp)
	sw	$2,232($sp)
	lw	$2,228($sp)
	lw	$3,232($sp)
	sw	$3,0($2)
$L735:
	lw	$2,360($sp)
	lbu	$2,6($2)
	bne	$2,$0,$L737
	nop

	lw	$2,0($sp)
	addiu	$2,$2,160
	sw	$2,236($sp)
	lw	$2,236($sp)
	lw	$2,0($2)
	sw	$2,52($sp)
	lw	$2,0($sp)
	addiu	$3,$2,160
	lw	$4,52($sp)
	li	$2,-16842752			# 0xfffffffffeff0000
	ori	$2,$2,0xffff
	and	$2,$4,$2
	sw	$3,240($sp)
	sw	$2,244($sp)
	lw	$2,240($sp)
	lw	$3,244($sp)
	sw	$3,0($2)
	j	$L739
	nop

$L737:
	lw	$2,0($sp)
	addiu	$2,$2,160
	sw	$2,248($sp)
	lw	$2,248($sp)
	lw	$2,0($2)
	sw	$2,56($sp)
	lw	$2,0($sp)
	addiu	$3,$2,160
	lw	$4,56($sp)
	li	$2,16777216			# 0x1000000
	or	$2,$4,$2
	sw	$3,252($sp)
	sw	$2,256($sp)
	lw	$2,252($sp)
	lw	$3,256($sp)
	sw	$3,0($2)
$L739:
	lw	$2,360($sp)
	lbu	$2,7($2)
	bne	$2,$0,$L741
	nop

	lw	$2,0($sp)
	addiu	$2,$2,160
	sw	$2,260($sp)
	lw	$2,260($sp)
	lw	$2,0($2)
	sw	$2,60($sp)
	lw	$2,0($sp)
	addiu	$3,$2,160
	lw	$4,60($sp)
	li	$2,-327680			# 0xfffffffffffb0000
	ori	$2,$2,0xffff
	and	$2,$4,$2
	sw	$3,264($sp)
	sw	$2,268($sp)
	lw	$2,264($sp)
	lw	$3,268($sp)
	sw	$3,0($2)
	j	$L743
	nop

$L741:
	lw	$2,0($sp)
	addiu	$2,$2,160
	sw	$2,272($sp)
	lw	$2,272($sp)
	lw	$2,0($2)
	sw	$2,64($sp)
	lw	$2,0($sp)
	addiu	$3,$2,160
	lw	$4,64($sp)
	li	$2,262144			# 0x40000
	or	$2,$4,$2
	sw	$3,276($sp)
	sw	$2,280($sp)
	lw	$2,276($sp)
	lw	$3,280($sp)
	sw	$3,0($2)
$L743:
	lw	$2,360($sp)
	lbu	$2,8($2)
	bne	$2,$0,$L745
	nop

	lw	$2,0($sp)
	addiu	$2,$2,160
	sw	$2,284($sp)
	lw	$2,284($sp)
	lw	$2,0($2)
	sw	$2,68($sp)
	lw	$2,0($sp)
	addiu	$3,$2,160
	lw	$4,68($sp)
	li	$2,-196608			# 0xfffffffffffd0000
	ori	$2,$2,0xffff
	and	$2,$4,$2
	sw	$3,288($sp)
	sw	$2,292($sp)
	lw	$2,288($sp)
	lw	$3,292($sp)
	sw	$3,0($2)
	j	$L747
	nop

$L745:
	lw	$2,0($sp)
	addiu	$2,$2,160
	sw	$2,296($sp)
	lw	$2,296($sp)
	lw	$2,0($2)
	sw	$2,72($sp)
	lw	$2,0($sp)
	addiu	$3,$2,160
	lw	$4,72($sp)
	li	$2,131072			# 0x20000
	or	$2,$4,$2
	sw	$3,300($sp)
	sw	$2,304($sp)
	lw	$2,300($sp)
	lw	$3,304($sp)
	sw	$3,0($2)
$L747:
	lw	$2,360($sp)
	lbu	$2,9($2)
	bne	$2,$0,$L749
	nop

	lw	$2,0($sp)
	addiu	$2,$2,160
	sw	$2,308($sp)
	lw	$2,308($sp)
	lw	$2,0($2)
	sw	$2,76($sp)
	lw	$2,0($sp)
	addiu	$3,$2,160
	lw	$4,76($sp)
	li	$2,-131072			# 0xfffffffffffe0000
	ori	$2,$2,0xffff
	and	$2,$4,$2
	sw	$3,312($sp)
	sw	$2,316($sp)
	lw	$2,312($sp)
	lw	$3,316($sp)
	sw	$3,0($2)
	j	$L751
	nop

$L749:
	lw	$2,0($sp)
	addiu	$2,$2,160
	sw	$2,320($sp)
	lw	$2,320($sp)
	lw	$2,0($2)
	sw	$2,80($sp)
	lw	$2,0($sp)
	addiu	$3,$2,160
	lw	$4,80($sp)
	li	$2,65536			# 0x10000
	or	$2,$4,$2
	sw	$3,324($sp)
	sw	$2,328($sp)
	lw	$2,324($sp)
	lw	$3,328($sp)
	sw	$3,0($2)
$L751:
	lw	$2,0($sp)
	addiu	$2,$2,160
	sw	$2,332($sp)
	lw	$2,332($sp)
	lw	$2,0($2)
	sw	$2,84($sp)
	lw	$2,0($sp)
	addiu	$3,$2,160
	lw	$4,84($sp)
	li	$2,-3211264			# 0xffffffffffcf0000
	ori	$2,$2,0xffff
	and	$4,$4,$2
	lw	$2,360($sp)
	lw	$2,12($2)
	sll	$5,$2,20
	li	$2,3145728			# 0x300000
	and	$2,$5,$2
	or	$2,$4,$2
	sw	$3,336($sp)
	sw	$2,340($sp)
	lw	$2,336($sp)
	lw	$3,340($sp)
	sw	$3,0($2)
	lw	$2,0($sp)
	addiu	$2,$2,160
	sw	$2,344($sp)
	lw	$2,344($sp)
	lw	$2,0($2)
	sw	$2,88($sp)
	lw	$2,0($sp)
	addiu	$3,$2,160
	lw	$4,88($sp)
	li	$2,-65536			# 0xffffffffffff0000
	and	$2,$4,$2
	lw	$4,360($sp)
	lhu	$4,10($4)
	or	$2,$2,$4
	sw	$3,348($sp)
	sw	$2,352($sp)
	lw	$2,348($sp)
	lw	$3,352($sp)
	sw	$3,0($2)
	move	$2,$0
$L712:
	addiu	$sp,$sp,360
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	qdma_set_txq_cngst_cfg
	.size	qdma_set_txq_cngst_cfg, .-qdma_set_txq_cngst_cfg
	.section	.text.qdma_get_txq_cngst_cfg,"ax",@progbits
	.align	2
	.globl	qdma_get_txq_cngst_cfg
	.set	nomips16
	.set	nomicromips
	.ent	qdma_get_txq_cngst_cfg
	.type	qdma_get_txq_cngst_cfg, @function
qdma_get_txq_cngst_cfg:
	.frame	$sp,56,$31		# vars= 56, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-56
	sw	$4,56($sp)
	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lw	$2,0($2)
	sw	$2,0($sp)
	lw	$2,0($sp)
	addiu	$2,$2,160
	sw	$2,4($sp)
	lw	$2,4($sp)
	lw	$2,0($2)
	bgez	$2,$L757
	nop

	lw	$2,56($sp)
	li	$3,1			# 0x1
	sb	$3,0($2)
	j	$L758
	nop

$L757:
	lw	$2,56($sp)
	sb	$0,0($2)
$L758:
	lw	$2,0($sp)
	addiu	$2,$2,160
	sw	$2,8($sp)
	lw	$2,8($sp)
	lw	$3,0($2)
	li	$2,1073741824			# 0x40000000
	and	$2,$3,$2
	beq	$2,$0,$L760
	nop

	lw	$2,56($sp)
	li	$3,1			# 0x1
	sb	$3,1($2)
	j	$L761
	nop

$L760:
	lw	$2,56($sp)
	sb	$0,1($2)
$L761:
	lw	$2,0($sp)
	addiu	$2,$2,160
	sw	$2,12($sp)
	lw	$2,12($sp)
	lw	$3,0($2)
	li	$2,536870912			# 0x20000000
	and	$2,$3,$2
	beq	$2,$0,$L763
	nop

	lw	$2,56($sp)
	li	$3,1			# 0x1
	sb	$3,2($2)
	j	$L764
	nop

$L763:
	lw	$2,56($sp)
	sb	$0,2($2)
$L764:
	lw	$2,0($sp)
	addiu	$2,$2,160
	sw	$2,16($sp)
	lw	$2,16($sp)
	lw	$3,0($2)
	li	$2,134217728			# 0x8000000
	and	$2,$3,$2
	beq	$2,$0,$L766
	nop

	lw	$2,56($sp)
	li	$3,1			# 0x1
	sb	$3,3($2)
	j	$L767
	nop

$L766:
	lw	$2,56($sp)
	sb	$0,3($2)
$L767:
	lw	$2,0($sp)
	addiu	$2,$2,160
	sw	$2,20($sp)
	lw	$2,20($sp)
	lw	$3,0($2)
	li	$2,67108864			# 0x4000000
	and	$2,$3,$2
	beq	$2,$0,$L769
	nop

	lw	$2,56($sp)
	li	$3,1			# 0x1
	sb	$3,4($2)
	j	$L770
	nop

$L769:
	lw	$2,56($sp)
	sb	$0,4($2)
$L770:
	lw	$2,0($sp)
	addiu	$2,$2,160
	sw	$2,24($sp)
	lw	$2,24($sp)
	lw	$3,0($2)
	li	$2,33554432			# 0x2000000
	and	$2,$3,$2
	beq	$2,$0,$L772
	nop

	lw	$2,56($sp)
	li	$3,1			# 0x1
	sb	$3,5($2)
	j	$L773
	nop

$L772:
	lw	$2,56($sp)
	sb	$0,5($2)
$L773:
	lw	$2,0($sp)
	addiu	$2,$2,160
	sw	$2,28($sp)
	lw	$2,28($sp)
	lw	$3,0($2)
	li	$2,16777216			# 0x1000000
	and	$2,$3,$2
	beq	$2,$0,$L775
	nop

	lw	$2,56($sp)
	li	$3,1			# 0x1
	sb	$3,6($2)
	j	$L776
	nop

$L775:
	lw	$2,56($sp)
	sb	$0,6($2)
$L776:
	lw	$2,0($sp)
	addiu	$2,$2,160
	sw	$2,32($sp)
	lw	$2,32($sp)
	lw	$3,0($2)
	li	$2,262144			# 0x40000
	and	$2,$3,$2
	beq	$2,$0,$L778
	nop

	lw	$2,56($sp)
	li	$3,1			# 0x1
	sb	$3,7($2)
	j	$L779
	nop

$L778:
	lw	$2,56($sp)
	sb	$0,7($2)
$L779:
	lw	$2,0($sp)
	addiu	$2,$2,160
	sw	$2,36($sp)
	lw	$2,36($sp)
	lw	$3,0($2)
	li	$2,131072			# 0x20000
	and	$2,$3,$2
	beq	$2,$0,$L781
	nop

	lw	$2,56($sp)
	li	$3,1			# 0x1
	sb	$3,8($2)
	j	$L782
	nop

$L781:
	lw	$2,56($sp)
	sb	$0,8($2)
$L782:
	lw	$2,0($sp)
	addiu	$2,$2,160
	sw	$2,40($sp)
	lw	$2,40($sp)
	lw	$3,0($2)
	li	$2,65536			# 0x10000
	and	$2,$3,$2
	beq	$2,$0,$L784
	nop

	lw	$2,56($sp)
	li	$3,1			# 0x1
	sb	$3,9($2)
	j	$L785
	nop

$L784:
	lw	$2,56($sp)
	sb	$0,9($2)
$L785:
	lw	$2,0($sp)
	addiu	$2,$2,160
	sw	$2,44($sp)
	lw	$2,44($sp)
	lw	$3,0($2)
	li	$2,3145728			# 0x300000
	and	$2,$3,$2
	srl	$3,$2,20
	lw	$2,56($sp)
	sw	$3,12($2)
	lw	$2,0($sp)
	addiu	$2,$2,160
	sw	$2,48($sp)
	lw	$2,48($sp)
	lw	$2,0($2)
	andi	$3,$2,0xffff
	lw	$2,56($sp)
	sh	$3,10($2)
	move	$2,$0
	addiu	$sp,$sp,56
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	qdma_get_txq_cngst_cfg
	.size	qdma_get_txq_cngst_cfg, .-qdma_get_txq_cngst_cfg
	.section	.text.qdma_set_tx_cngst_mode,"ax",@progbits
	.align	2
	.globl	qdma_set_tx_cngst_mode
	.set	nomips16
	.set	nomicromips
	.ent	qdma_set_tx_cngst_mode
	.type	qdma_set_tx_cngst_mode, @function
qdma_set_tx_cngst_mode:
	.frame	$sp,304,$31		# vars= 280, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-304
	sw	$31,300($sp)
	sw	$4,304($sp)
	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lw	$2,0($2)
	sw	$2,16($sp)
	lw	$2,304($sp)
	lw	$2,8($2)
	sw	$2,20($sp)
	lw	$2,16($sp)
	addiu	$2,$2,160
	sw	$2,88($sp)
	lw	$2,88($sp)
	lw	$2,0($2)
	sw	$2,24($sp)
	lw	$2,16($sp)
	addiu	$3,$2,160
	lw	$4,24($sp)
	li	$2,-134283264			# 0xfffffffff7ff0000
	ori	$2,$2,0xffff
	and	$2,$4,$2
	sw	$3,92($sp)
	sw	$2,96($sp)
	lw	$2,92($sp)
	lw	$3,96($sp)
	sw	$3,0($2)
	lw	$2,16($sp)
	addiu	$2,$2,160
	sw	$2,100($sp)
	lw	$2,100($sp)
	lw	$2,0($2)
	sw	$2,28($sp)
	lw	$2,16($sp)
	addiu	$3,$2,160
	lw	$4,28($sp)
	li	$2,-67174400			# 0xfffffffffbff0000
	ori	$2,$2,0xffff
	and	$2,$4,$2
	sw	$3,104($sp)
	sw	$2,108($sp)
	lw	$2,104($sp)
	lw	$3,108($sp)
	sw	$3,0($2)
	lw	$2,16($sp)
	addiu	$2,$2,160
	sw	$2,112($sp)
	lw	$2,112($sp)
	lw	$2,0($2)
	sw	$2,32($sp)
	lw	$2,16($sp)
	addiu	$3,$2,160
	lw	$4,32($sp)
	li	$2,-33619968			# 0xfffffffffdff0000
	ori	$2,$2,0xffff
	and	$2,$4,$2
	sw	$3,116($sp)
	sw	$2,120($sp)
	lw	$2,116($sp)
	lw	$3,120($sp)
	sw	$3,0($2)
	lw	$2,16($sp)
	addiu	$2,$2,160
	sw	$2,124($sp)
	lw	$2,124($sp)
	lw	$2,0($2)
	sw	$2,36($sp)
	lw	$2,16($sp)
	addiu	$3,$2,160
	lw	$4,36($sp)
	li	$2,-16842752			# 0xfffffffffeff0000
	ori	$2,$2,0xffff
	and	$2,$4,$2
	sw	$3,128($sp)
	sw	$2,132($sp)
	lw	$2,128($sp)
	lw	$3,132($sp)
	sw	$3,0($2)
	lw	$2,16($sp)
	addiu	$2,$2,160
	sw	$2,136($sp)
	lw	$2,136($sp)
	lw	$2,0($2)
	sw	$2,40($sp)
	lw	$2,16($sp)
	addiu	$3,$2,160
	lw	$4,40($sp)
	li	$2,262144			# 0x40000
	or	$2,$4,$2
	sw	$3,140($sp)
	sw	$2,144($sp)
	lw	$2,140($sp)
	lw	$3,144($sp)
	sw	$3,0($2)
	lw	$2,16($sp)
	addiu	$2,$2,160
	sw	$2,148($sp)
	lw	$2,148($sp)
	lw	$2,0($2)
	sw	$2,44($sp)
	lw	$2,16($sp)
	addiu	$3,$2,160
	lw	$4,44($sp)
	li	$2,131072			# 0x20000
	or	$2,$4,$2
	sw	$3,152($sp)
	sw	$2,156($sp)
	lw	$2,152($sp)
	lw	$3,156($sp)
	sw	$3,0($2)
	lw	$2,16($sp)
	addiu	$2,$2,160
	sw	$2,160($sp)
	lw	$2,160($sp)
	lw	$2,0($2)
	sw	$2,48($sp)
	lw	$2,16($sp)
	addiu	$3,$2,160
	lw	$4,48($sp)
	li	$2,65536			# 0x10000
	or	$2,$4,$2
	sw	$3,164($sp)
	sw	$2,168($sp)
	lw	$2,164($sp)
	lw	$3,168($sp)
	sw	$3,0($2)
	lw	$2,16($sp)
	addiu	$2,$2,148
	sw	$2,172($sp)
	lw	$2,172($sp)
	lw	$2,0($2)
	sw	$2,52($sp)
	lw	$2,16($sp)
	addiu	$3,$2,148
	lw	$4,52($sp)
	li	$2,-2147483648			# 0xffffffff80000000
	or	$2,$4,$2
	sw	$3,176($sp)
	sw	$2,180($sp)
	lw	$2,176($sp)
	lw	$3,180($sp)
	sw	$3,0($2)
	li	$2,204			# 0xcc
	sh	$2,282($sp)
	li	$2,73			# 0x49
	sh	$2,286($sp)
	li	$2,6			# 0x6
	sh	$2,290($sp)
	lui	$2,%hi(physical_size)
	lw	$3,%lo(physical_size)($2)
	lui	$2,%hi(physical_size)
	lw	$2,%lo(physical_size)($2)
	srl	$2,$2,4
	subu	$2,$3,$2
	srl	$2,$2,8
	andi	$2,$2,0xffff
	sh	$2,280($sp)
	lhu	$2,280($sp)
	sh	$2,284($sp)
	lhu	$2,282($sp)
	sh	$2,288($sp)
	lw	$2,20($sp)
	lw	$3,0($2)
	li	$2,2			# 0x2
	bne	$3,$2,$L798
	nop

	lw	$2,16($sp)
	addiu	$2,$2,160
	sw	$2,184($sp)
	lw	$2,184($sp)
	lw	$2,0($2)
	sw	$2,56($sp)
	lw	$2,16($sp)
	addiu	$3,$2,160
	lw	$4,56($sp)
	li	$2,-536936448			# 0xffffffffdfff0000
	ori	$2,$2,0xffff
	and	$2,$4,$2
	sw	$3,188($sp)
	sw	$2,192($sp)
	lw	$2,188($sp)
	lw	$3,192($sp)
	sw	$3,0($2)
	lw	$2,16($sp)
	addiu	$2,$2,148
	sw	$2,196($sp)
	lw	$2,196($sp)
	lw	$2,0($2)
	sw	$2,60($sp)
	lw	$2,16($sp)
	addiu	$3,$2,148
	lw	$4,60($sp)
	li	$2,-1073807360			# 0xffffffffbfff0000
	ori	$2,$2,0xffff
	and	$2,$4,$2
	sw	$3,200($sp)
	sw	$2,204($sp)
	lw	$2,200($sp)
	lw	$3,204($sp)
	sw	$3,0($2)
	j	$L801
	nop

$L798:
	lw	$2,20($sp)
	lw	$2,0($2)
	bne	$2,$0,$L802
	nop

	lw	$2,16($sp)
	addiu	$2,$2,160
	sw	$2,208($sp)
	lw	$2,208($sp)
	lw	$2,0($2)
	sw	$2,64($sp)
	lw	$2,16($sp)
	addiu	$3,$2,160
	lw	$4,64($sp)
	li	$2,536870912			# 0x20000000
	or	$2,$4,$2
	sw	$3,212($sp)
	sw	$2,216($sp)
	lw	$2,212($sp)
	lw	$3,216($sp)
	sw	$3,0($2)
	lw	$2,16($sp)
	addiu	$2,$2,148
	sw	$2,220($sp)
	lw	$2,220($sp)
	lw	$2,0($2)
	sw	$2,68($sp)
	lw	$2,16($sp)
	addiu	$3,$2,148
	lw	$4,68($sp)
	li	$2,-1073807360			# 0xffffffffbfff0000
	ori	$2,$2,0xffff
	and	$2,$4,$2
	sw	$3,224($sp)
	sw	$2,228($sp)
	lw	$2,224($sp)
	lw	$3,228($sp)
	sw	$3,0($2)
	lw	$2,304($sp)
	addiu	$3,$sp,280
	sw	$3,8($2)
	lw	$4,304($sp)
	lui	$2,%hi(qdma_set_txq_cngst_dynamic_threshold)
	addiu	$2,$2,%lo(qdma_set_txq_cngst_dynamic_threshold)
	jalr	$2
	nop

	j	$L801
	nop

$L802:
	lw	$2,20($sp)
	lw	$3,0($2)
	li	$2,1			# 0x1
	bne	$3,$2,$L801
	nop

	lw	$2,16($sp)
	addiu	$2,$2,160
	sw	$2,232($sp)
	lw	$2,232($sp)
	lw	$2,0($2)
	sw	$2,72($sp)
	lw	$2,16($sp)
	addiu	$3,$2,160
	lw	$4,72($sp)
	li	$2,536870912			# 0x20000000
	or	$2,$4,$2
	sw	$3,236($sp)
	sw	$2,240($sp)
	lw	$2,236($sp)
	lw	$3,240($sp)
	sw	$3,0($2)
	lw	$2,16($sp)
	addiu	$2,$2,148
	sw	$2,244($sp)
	lw	$2,244($sp)
	lw	$2,0($2)
	sw	$2,76($sp)
	lw	$2,16($sp)
	addiu	$3,$2,148
	lw	$4,76($sp)
	li	$2,1073741824			# 0x40000000
	or	$2,$4,$2
	sw	$3,248($sp)
	sw	$2,252($sp)
	lw	$2,248($sp)
	lw	$3,252($sp)
	sw	$3,0($2)
	lw	$2,16($sp)
	addiu	$2,$2,148
	sw	$2,256($sp)
	lw	$2,256($sp)
	lw	$2,0($2)
	sw	$2,80($sp)
	lw	$2,16($sp)
	addiu	$3,$2,148
	lw	$4,80($sp)
	li	$2,-267452416			# 0xfffffffff00f0000
	ori	$2,$2,0xffff
	and	$4,$4,$2
	lw	$2,20($sp)
	lw	$2,8($2)
	srl	$2,$2,4
	sll	$5,$2,20
	li	$2,267386880			# 0xff00000
	and	$2,$5,$2
	or	$2,$4,$2
	sw	$3,260($sp)
	sw	$2,264($sp)
	lw	$2,260($sp)
	lw	$3,264($sp)
	sw	$3,0($2)
	lw	$2,16($sp)
	addiu	$2,$2,160
	sw	$2,268($sp)
	lw	$2,268($sp)
	lw	$2,0($2)
	sw	$2,84($sp)
	lw	$2,16($sp)
	addiu	$3,$2,160
	lw	$4,84($sp)
	li	$2,-12648448			# 0xffffffffff3f0000
	ori	$2,$2,0xffff
	and	$4,$4,$2
	lw	$2,20($sp)
	lw	$2,4($2)
	sll	$5,$2,22
	li	$2,12582912			# 0xc00000
	and	$2,$5,$2
	or	$2,$4,$2
	sw	$3,272($sp)
	sw	$2,276($sp)
	lw	$2,272($sp)
	lw	$3,276($sp)
	sw	$3,0($2)
	lw	$2,304($sp)
	addiu	$3,$sp,280
	sw	$3,8($2)
	lw	$4,304($sp)
	lui	$2,%hi(qdma_set_txq_cngst_dynamic_threshold)
	addiu	$2,$2,%lo(qdma_set_txq_cngst_dynamic_threshold)
	jalr	$2
	nop

$L801:
	move	$2,$0
	lw	$31,300($sp)
	addiu	$sp,$sp,304
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	qdma_set_tx_cngst_mode
	.size	qdma_set_tx_cngst_mode, .-qdma_set_tx_cngst_mode
	.section	.text.qdma_get_tx_cngst_mode,"ax",@progbits
	.align	2
	.globl	qdma_get_tx_cngst_mode
	.set	nomips16
	.set	nomicromips
	.ent	qdma_get_tx_cngst_mode
	.type	qdma_get_tx_cngst_mode, @function
qdma_get_tx_cngst_mode:
	.frame	$sp,24,$31		# vars= 24, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-24
	sw	$4,24($sp)
	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lw	$2,0($2)
	sw	$2,0($sp)
	lw	$2,24($sp)
	lw	$2,8($2)
	sw	$2,4($sp)
	lw	$2,0($sp)
	addiu	$2,$2,160
	sw	$2,8($sp)
	lw	$2,8($sp)
	lw	$3,0($2)
	li	$2,536870912			# 0x20000000
	and	$2,$3,$2
	beq	$2,$0,$L812
	nop

	lw	$2,0($sp)
	addiu	$2,$2,148
	sw	$2,12($sp)
	lw	$2,12($sp)
	lw	$3,0($2)
	li	$2,1073741824			# 0x40000000
	and	$2,$3,$2
	beq	$2,$0,$L814
	nop

	lw	$2,4($sp)
	li	$3,1			# 0x1
	sw	$3,0($2)
	lw	$2,0($sp)
	addiu	$2,$2,148
	sw	$2,16($sp)
	lw	$2,16($sp)
	lw	$3,0($2)
	li	$2,267386880			# 0xff00000
	and	$2,$3,$2
	srl	$3,$2,20
	lw	$2,4($sp)
	sw	$3,8($2)
	lw	$2,0($sp)
	addiu	$2,$2,160
	sw	$2,20($sp)
	lw	$2,20($sp)
	lw	$3,0($2)
	li	$2,12582912			# 0xc00000
	and	$2,$3,$2
	srl	$3,$2,22
	lw	$2,4($sp)
	sw	$3,4($2)
	j	$L818
	nop

$L814:
	lw	$2,4($sp)
	sw	$0,0($2)
	j	$L818
	nop

$L812:
	lw	$2,4($sp)
	li	$3,2			# 0x2
	sw	$3,0($2)
$L818:
	move	$2,$0
	addiu	$sp,$sp,24
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	qdma_get_tx_cngst_mode
	.size	qdma_get_tx_cngst_mode, .-qdma_get_tx_cngst_mode
	.section	.text.qdma_set_virtual_channel_mode,"ax",@progbits
	.align	2
	.globl	qdma_set_virtual_channel_mode
	.set	nomips16
	.set	nomicromips
	.ent	qdma_set_virtual_channel_mode
	.type	qdma_set_virtual_channel_mode, @function
qdma_set_virtual_channel_mode:
	.frame	$sp,72,$31		# vars= 72, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-72
	sw	$4,72($sp)
	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lw	$2,0($2)
	sw	$2,0($sp)
	lw	$2,72($sp)
	lw	$2,8($2)
	sw	$2,4($sp)
	lw	$2,4($sp)
	lw	$3,0($2)
	li	$2,1			# 0x1
	bne	$3,$2,$L821
	nop

	lw	$2,0($sp)
	addiu	$2,$2,4
	sw	$2,24($sp)
	lw	$2,24($sp)
	lw	$2,0($2)
	sw	$2,8($sp)
	lw	$2,0($sp)
	addiu	$3,$2,4
	lw	$4,8($sp)
	li	$2,33554432			# 0x2000000
	or	$2,$4,$2
	sw	$3,28($sp)
	sw	$2,32($sp)
	lw	$2,28($sp)
	lw	$3,32($sp)
	sw	$3,0($2)
	j	$L823
	nop

$L821:
	lw	$2,0($sp)
	addiu	$2,$2,4
	sw	$2,36($sp)
	lw	$2,36($sp)
	lw	$2,0($2)
	sw	$2,12($sp)
	lw	$2,0($sp)
	addiu	$3,$2,4
	lw	$4,12($sp)
	li	$2,-33619968			# 0xfffffffffdff0000
	ori	$2,$2,0xffff
	and	$2,$4,$2
	sw	$3,40($sp)
	sw	$2,44($sp)
	lw	$2,40($sp)
	lw	$3,44($sp)
	sw	$3,0($2)
$L823:
	lw	$2,4($sp)
	lw	$3,4($2)
	li	$2,1			# 0x1
	bne	$3,$2,$L825
	nop

	lw	$2,0($sp)
	addiu	$2,$2,4
	sw	$2,48($sp)
	lw	$2,48($sp)
	lw	$2,0($2)
	sw	$2,16($sp)
	lw	$2,0($sp)
	addiu	$3,$2,4
	lw	$4,16($sp)
	li	$2,16777216			# 0x1000000
	or	$2,$4,$2
	sw	$3,52($sp)
	sw	$2,56($sp)
	lw	$2,52($sp)
	lw	$3,56($sp)
	sw	$3,0($2)
	j	$L827
	nop

$L825:
	lw	$2,0($sp)
	addiu	$2,$2,4
	sw	$2,60($sp)
	lw	$2,60($sp)
	lw	$2,0($2)
	sw	$2,20($sp)
	lw	$2,0($sp)
	addiu	$3,$2,4
	lw	$4,20($sp)
	li	$2,-16842752			# 0xfffffffffeff0000
	ori	$2,$2,0xffff
	and	$2,$4,$2
	sw	$3,64($sp)
	sw	$2,68($sp)
	lw	$2,64($sp)
	lw	$3,68($sp)
	sw	$3,0($2)
$L827:
	move	$2,$0
	addiu	$sp,$sp,72
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	qdma_set_virtual_channel_mode
	.size	qdma_set_virtual_channel_mode, .-qdma_set_virtual_channel_mode
	.section	.text.qdma_get_virtual_channel_mode,"ax",@progbits
	.align	2
	.globl	qdma_get_virtual_channel_mode
	.set	nomips16
	.set	nomicromips
	.ent	qdma_get_virtual_channel_mode
	.type	qdma_get_virtual_channel_mode, @function
qdma_get_virtual_channel_mode:
	.frame	$sp,16,$31		# vars= 16, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-16
	sw	$4,16($sp)
	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lw	$2,0($2)
	sw	$2,0($sp)
	lw	$2,16($sp)
	lw	$2,8($2)
	sw	$2,4($sp)
	lw	$2,0($sp)
	addiu	$2,$2,4
	sw	$2,8($sp)
	lw	$2,8($sp)
	lw	$3,0($2)
	li	$2,33554432			# 0x2000000
	and	$2,$3,$2
	beq	$2,$0,$L832
	nop

	lw	$2,4($sp)
	li	$3,1			# 0x1
	sw	$3,0($2)
	j	$L833
	nop

$L832:
	lw	$2,4($sp)
	sw	$0,0($2)
$L833:
	lw	$2,0($sp)
	addiu	$2,$2,4
	sw	$2,12($sp)
	lw	$2,12($sp)
	lw	$3,0($2)
	li	$2,16777216			# 0x1000000
	and	$2,$3,$2
	beq	$2,$0,$L835
	nop

	lw	$2,4($sp)
	li	$3,1			# 0x1
	sw	$3,4($2)
	j	$L836
	nop

$L835:
	lw	$2,4($sp)
	sw	$0,4($2)
$L836:
	move	$2,$0
	addiu	$sp,$sp,16
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	qdma_get_virtual_channel_mode
	.size	qdma_get_virtual_channel_mode, .-qdma_get_virtual_channel_mode
	.section	.text.qdma_set_virtual_channel_qos,"ax",@progbits
	.align	2
	.globl	qdma_set_virtual_channel_qos
	.set	nomips16
	.set	nomicromips
	.ent	qdma_set_virtual_channel_qos
	.type	qdma_set_virtual_channel_qos, @function
qdma_set_virtual_channel_qos:
	.frame	$sp,64,$31		# vars= 40, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-64
	sw	$31,60($sp)
	sw	$4,64($sp)
	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lw	$2,0($2)
	sw	$2,24($sp)
	sw	$0,16($sp)
	sw	$0,28($sp)
	sw	$0,20($sp)
	lw	$2,64($sp)
	lw	$2,8($2)
	sw	$2,32($sp)
	lw	$2,24($sp)
	addiu	$2,$2,4
	sw	$2,36($sp)
	lw	$2,36($sp)
	lw	$3,0($2)
	li	$2,33554432			# 0x2000000
	and	$2,$3,$2
	bne	$2,$0,$L840
	nop

	li	$2,-22			# 0xffffffffffffffea
	j	$L850
	nop

$L840:
	lw	$2,24($sp)
	addiu	$2,$2,4
	sw	$2,40($sp)
	lw	$2,40($sp)
	lw	$3,0($2)
	li	$2,16777216			# 0x1000000
	and	$2,$3,$2
	beq	$2,$0,$L843
	nop

	li	$2,4			# 0x4
	sw	$2,20($sp)
	j	$L844
	nop

$L843:
	li	$2,2			# 0x2
	sw	$2,20($sp)
$L844:
	li	$3,8			# 0x8
	lw	$2,20($sp)
	div	$0,$3,$2
	mfhi	$3
	mflo	$2
	sw	$2,28($sp)
	lw	$2,32($sp)
	lbu	$2,0($2)
	move	$3,$2
	lw	$2,28($sp)
	slt	$2,$3,$2
	beq	$2,$0,$L845
	nop

	lw	$2,32($sp)
	lw	$3,4($2)
	lw	$2,20($sp)
	sltu	$2,$3,$2
	bne	$2,$0,$L846
	nop

$L845:
	li	$2,-22			# 0xffffffffffffffea
	j	$L850
	nop

$L846:
	sw	$0,16($sp)
	j	$L847
	nop

$L849:
	lw	$3,32($sp)
	lw	$2,16($sp)
	addu	$2,$3,$2
	lbu	$2,8($2)
	sltu	$2,$2,101
	bne	$2,$0,$L848
	nop

	lw	$3,32($sp)
	lw	$2,16($sp)
	addu	$2,$3,$2
	lbu	$3,8($2)
	li	$2,255			# 0xff
	beq	$3,$2,$L848
	nop

	li	$2,-22			# 0xffffffffffffffea
	j	$L850
	nop

$L848:
	lw	$3,32($sp)
	lw	$2,16($sp)
	addu	$2,$3,$2
	lbu	$3,8($2)
	lw	$2,16($sp)
	addiu	$4,$sp,16
	addu	$2,$4,$2
	sb	$3,28($2)
	lw	$2,16($sp)
	addiu	$2,$2,1
	sw	$2,16($sp)
$L847:
	lw	$3,16($sp)
	lw	$2,20($sp)
	slt	$2,$3,$2
	bne	$2,$0,$L849
	nop

	lw	$2,32($sp)
	lbu	$2,0($2)
	move	$4,$2
	lw	$2,20($sp)
	andi	$2,$2,0x00ff
	move	$5,$2
	lw	$2,32($sp)
	lw	$2,4($2)
	andi	$2,$2,0x00ff
	move	$3,$2
	addiu	$2,$sp,44
	move	$6,$3
	move	$7,$2
	lui	$2,%hi(qdmaSetVirtualChannelQos)
	addiu	$2,$2,%lo(qdmaSetVirtualChannelQos)
	jalr	$2
	nop

$L850:
	lw	$31,60($sp)
	addiu	$sp,$sp,64
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	qdma_set_virtual_channel_qos
	.size	qdma_set_virtual_channel_qos, .-qdma_set_virtual_channel_qos
	.section	.text.qdma_get_virtual_channel_qos,"ax",@progbits
	.align	2
	.globl	qdma_get_virtual_channel_qos
	.set	nomips16
	.set	nomicromips
	.ent	qdma_get_virtual_channel_qos
	.type	qdma_get_virtual_channel_qos, @function
qdma_get_virtual_channel_qos:
	.frame	$sp,72,$31		# vars= 48, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-72
	sw	$31,68($sp)
	sw	$4,72($sp)
	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lw	$2,0($2)
	sw	$2,24($sp)
	lw	$2,72($sp)
	lw	$2,8($2)
	sw	$2,28($sp)
	sw	$0,32($sp)
	sw	$0,16($sp)
	sw	$0,36($sp)
	sw	$0,20($sp)
	sb	$0,48($sp)
	lw	$2,24($sp)
	addiu	$2,$2,4
	sw	$2,40($sp)
	lw	$2,40($sp)
	lw	$3,0($2)
	li	$2,33554432			# 0x2000000
	and	$2,$3,$2
	bne	$2,$0,$L853
	nop

	li	$2,-22			# 0xffffffffffffffea
	j	$L862
	nop

$L853:
	lw	$2,24($sp)
	addiu	$2,$2,4
	sw	$2,44($sp)
	lw	$2,44($sp)
	lw	$3,0($2)
	li	$2,16777216			# 0x1000000
	and	$2,$3,$2
	beq	$2,$0,$L856
	nop

	li	$2,4			# 0x4
	sw	$2,20($sp)
	j	$L857
	nop

$L856:
	li	$2,2			# 0x2
	sw	$2,20($sp)
$L857:
	li	$3,8			# 0x8
	lw	$2,20($sp)
	div	$0,$3,$2
	mfhi	$3
	mflo	$2
	sw	$2,36($sp)
	lw	$2,28($sp)
	lbu	$2,0($2)
	move	$3,$2
	lw	$2,36($sp)
	slt	$2,$3,$2
	bne	$2,$0,$L858
	nop

	li	$2,-22			# 0xffffffffffffffea
	j	$L862
	nop

$L858:
	lw	$2,28($sp)
	lbu	$2,0($2)
	move	$4,$2
	lw	$2,20($sp)
	andi	$2,$2,0x00ff
	move	$5,$2
	addiu	$3,$sp,48
	addiu	$2,$sp,52
	move	$6,$3
	move	$7,$2
	lui	$2,%hi(qdmaGetVirtualChannelQos)
	addiu	$2,$2,%lo(qdmaGetVirtualChannelQos)
	jalr	$2
	nop

	sw	$2,32($sp)
	lw	$2,32($sp)
	bgez	$2,$L859
	nop

	li	$2,-14			# 0xfffffffffffffff2
	j	$L862
	nop

$L859:
	lbu	$2,48($sp)
	move	$3,$2
	lw	$2,28($sp)
	sw	$3,4($2)
	sw	$0,16($sp)
	j	$L860
	nop

$L861:
	lw	$2,16($sp)
	addiu	$3,$sp,16
	addu	$2,$3,$2
	lbu	$3,36($2)
	lw	$4,28($sp)
	lw	$2,16($sp)
	addu	$2,$4,$2
	sb	$3,8($2)
	lw	$2,16($sp)
	addiu	$2,$2,1
	sw	$2,16($sp)
$L860:
	lw	$3,16($sp)
	lw	$2,20($sp)
	slt	$2,$3,$2
	bne	$2,$0,$L861
	nop

	move	$2,$0
$L862:
	lw	$31,68($sp)
	addiu	$sp,$sp,72
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	qdma_get_virtual_channel_qos
	.size	qdma_get_virtual_channel_qos, .-qdma_get_virtual_channel_qos
	.section	.text.qdma_clear_dbg_cntr_value_all,"ax",@progbits
	.align	2
	.globl	qdma_clear_dbg_cntr_value_all
	.set	nomips16
	.set	nomicromips
	.ent	qdma_clear_dbg_cntr_value_all
	.type	qdma_clear_dbg_cntr_value_all, @function
qdma_clear_dbg_cntr_value_all:
	.frame	$sp,16,$31		# vars= 16, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-16
	sw	$4,16($sp)
	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lw	$2,0($2)
	sw	$2,4($sp)
	sw	$0,0($sp)
	sw	$0,0($sp)
	j	$L864
	nop

$L865:
	lw	$2,0($sp)
	sll	$2,$2,3
	move	$3,$2
	lw	$2,4($sp)
	addu	$2,$3,$2
	addiu	$2,$2,772
	sw	$2,8($sp)
	sw	$0,12($sp)
	lw	$2,8($sp)
	lw	$3,12($sp)
	sw	$3,0($2)
	lw	$2,0($sp)
	addiu	$2,$2,1
	sw	$2,0($sp)
$L864:
	lw	$2,0($sp)
	slt	$2,$2,40
	bne	$2,$0,$L865
	nop

	move	$2,$0
	addiu	$sp,$sp,16
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	qdma_clear_dbg_cntr_value_all
	.size	qdma_clear_dbg_cntr_value_all, .-qdma_clear_dbg_cntr_value_all
	.section	.text.qdma_clear_and_set_dbg_cntr_channel_group,"ax",@progbits
	.align	2
	.globl	qdma_clear_and_set_dbg_cntr_channel_group
	.set	nomips16
	.set	nomicromips
	.ent	qdma_clear_and_set_dbg_cntr_channel_group
	.type	qdma_clear_and_set_dbg_cntr_channel_group, @function
qdma_clear_and_set_dbg_cntr_channel_group:
	.frame	$sp,56,$31		# vars= 32, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-56
	sw	$31,52($sp)
	sw	$4,56($sp)
	sw	$0,16($sp)
	sw	$0,20($sp)
	lw	$2,56($sp)
	lw	$2,8($2)
	sw	$2,24($sp)
	li	$2,1			# 0x1
	sb	$2,29($sp)
	sb	$0,36($sp)
	li	$2,1			# 0x1
	sb	$2,38($sp)
	li	$2,1			# 0x1
	sb	$2,37($sp)
	sb	$0,40($sp)
	sb	$0,41($sp)
	lw	$2,24($sp)
	sltu	$2,$2,3
	bne	$2,$0,$L868
	nop

	li	$2,-22			# 0xffffffffffffffea
	j	$L872
	nop

$L868:
	sw	$0,16($sp)
	j	$L870
	nop

$L871:
	lw	$2,16($sp)
	andi	$2,$2,0x00ff
	sb	$2,39($sp)
	lw	$2,16($sp)
	andi	$2,$2,0x00ff
	sb	$2,28($sp)
	sw	$0,32($sp)
	addiu	$2,$sp,28
	move	$4,$2
	lui	$2,%hi(qdma_clear_and_set_dbg_cntr_info)
	addiu	$2,$2,%lo(qdma_clear_and_set_dbg_cntr_info)
	jalr	$2
	nop

	lw	$2,16($sp)
	andi	$2,$2,0x00ff
	addiu	$2,$2,8
	andi	$2,$2,0x00ff
	sb	$2,28($sp)
	li	$2,1			# 0x1
	sw	$2,32($sp)
	addiu	$2,$sp,28
	move	$4,$2
	lui	$2,%hi(qdma_clear_and_set_dbg_cntr_info)
	addiu	$2,$2,%lo(qdma_clear_and_set_dbg_cntr_info)
	jalr	$2
	nop

	lw	$2,16($sp)
	addiu	$2,$2,1
	sw	$2,16($sp)
$L870:
	lw	$2,16($sp)
	slt	$2,$2,8
	bne	$2,$0,$L871
	nop

	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lw	$3,24($sp)
	sw	$3,336($2)
	move	$2,$0
$L872:
	lw	$31,52($sp)
	addiu	$sp,$sp,56
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	qdma_clear_and_set_dbg_cntr_channel_group
	.size	qdma_clear_and_set_dbg_cntr_channel_group, .-qdma_clear_and_set_dbg_cntr_channel_group
	.section	.text.qdma_clear_and_set_dbg_cntr_queue_group,"ax",@progbits
	.align	2
	.globl	qdma_clear_and_set_dbg_cntr_queue_group
	.set	nomips16
	.set	nomicromips
	.ent	qdma_clear_and_set_dbg_cntr_queue_group
	.type	qdma_clear_and_set_dbg_cntr_queue_group, @function
qdma_clear_and_set_dbg_cntr_queue_group:
	.frame	$sp,48,$31		# vars= 24, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-48
	sw	$31,44($sp)
	sw	$4,48($sp)
	sw	$0,16($sp)
	li	$2,1			# 0x1
	sb	$2,21($sp)
	li	$2,1			# 0x1
	sb	$2,28($sp)
	li	$2,1			# 0x1
	sb	$2,30($sp)
	sb	$0,29($sp)
	sb	$0,31($sp)
	sb	$0,33($sp)
	sw	$0,16($sp)
	j	$L874
	nop

$L875:
	lw	$2,16($sp)
	andi	$2,$2,0x00ff
	sb	$2,32($sp)
	lw	$2,16($sp)
	andi	$2,$2,0x00ff
	sb	$2,20($sp)
	sw	$0,24($sp)
	addiu	$2,$sp,20
	move	$4,$2
	lui	$2,%hi(qdma_clear_and_set_dbg_cntr_info)
	addiu	$2,$2,%lo(qdma_clear_and_set_dbg_cntr_info)
	jalr	$2
	nop

	lw	$2,16($sp)
	andi	$2,$2,0x00ff
	addiu	$2,$2,8
	andi	$2,$2,0x00ff
	sb	$2,20($sp)
	li	$2,1			# 0x1
	sw	$2,24($sp)
	addiu	$2,$sp,20
	move	$4,$2
	lui	$2,%hi(qdma_clear_and_set_dbg_cntr_info)
	addiu	$2,$2,%lo(qdma_clear_and_set_dbg_cntr_info)
	jalr	$2
	nop

	lw	$2,16($sp)
	addiu	$2,$2,1
	sw	$2,16($sp)
$L874:
	lw	$2,16($sp)
	slt	$2,$2,8
	bne	$2,$0,$L875
	nop

	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	li	$3,3			# 0x3
	sw	$3,336($2)
	move	$2,$0
	lw	$31,44($sp)
	addiu	$sp,$sp,48
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	qdma_clear_and_set_dbg_cntr_queue_group
	.size	qdma_clear_and_set_dbg_cntr_queue_group, .-qdma_clear_and_set_dbg_cntr_queue_group
	.rdata
	.align	2
$LC117:
	.ascii	"Dump Queue %d~%d TX CPU & FWD Counter:\012\000"
	.align	2
$LC118:
	.ascii	"queue_%d: tx cpu cnt:%x, tx fwd cnt:%u\012\000"
	.align	2
$LC119:
	.ascii	"Dump Channel %d~%d TX CPU & FWD Counter:\012\000"
	.align	2
$LC120:
	.ascii	"channel_%d: tx cpu cnt:%x, tx fwd cnt:%u\012\000"
	.align	2
$LC121:
	.ascii	"\012TX count: cpu-path:%u, fwd-path:%u, total:%u\012\000"
	.align	2
$LC122:
	.ascii	"TX cpu-path count: Ring0:%u, Ring1:%u\012\000"
	.align	2
$LC123:
	.ascii	"RX count: cpu-path:%u, fwd-path:%u, total:%u\012\000"
	.align	2
$LC124:
	.ascii	"RX cpu-path count: Ring0:%u, Ring1:%u\012\000"
	.section	.text.qdma_dump_dbg_cntr_value,"ax",@progbits
	.align	2
	.globl	qdma_dump_dbg_cntr_value
	.set	nomips16
	.set	nomicromips
	.ent	qdma_dump_dbg_cntr_value
	.type	qdma_dump_dbg_cntr_value, @function
qdma_dump_dbg_cntr_value:
	.frame	$sp,152,$31		# vars= 128, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-152
	sw	$31,148($sp)
	sw	$4,152($sp)
	sw	$0,16($sp)
	sw	$0,20($sp)
	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lw	$2,336($2)
	sltu	$2,$2,4
	bne	$2,$0,$L878
	nop

	li	$2,-22			# 0xffffffffffffffea
	j	$L900
	nop

$L878:
	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lw	$3,336($2)
	li	$2,3			# 0x3
	bne	$3,$2,$L880
	nop

	lui	$2,%hi($LC117)
	addiu	$4,$2,%lo($LC117)
	move	$5,$0
	li	$6,7			# 0x7
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	sw	$0,16($sp)
	j	$L881
	nop

$L884:
	lw	$2,16($sp)
	andi	$2,$2,0x00ff
	sb	$2,120($sp)
	addiu	$2,$sp,120
	move	$4,$2
	lui	$2,%hi(qdma_get_dbg_cntr_info)
	addiu	$2,$2,%lo(qdma_get_dbg_cntr_info)
	jalr	$2
	nop

	lw	$2,124($sp)
	bne	$2,$0,$L882
	nop

	lbu	$3,129($sp)
	li	$2,1			# 0x1
	beq	$3,$2,$L882
	nop

	lbu	$2,132($sp)
	move	$3,$2
	lw	$2,16($sp)
	beq	$3,$2,$L883
	nop

$L882:
	li	$2,-22			# 0xffffffffffffffea
	j	$L900
	nop

$L883:
	lw	$3,136($sp)
	lw	$2,16($sp)
	sll	$2,$2,2
	addiu	$4,$sp,16
	addu	$2,$4,$2
	sw	$3,8($2)
	lw	$2,16($sp)
	addiu	$2,$2,1
	sw	$2,16($sp)
$L881:
	lw	$2,16($sp)
	slt	$2,$2,8
	bne	$2,$0,$L884
	nop

	sw	$0,16($sp)
	j	$L885
	nop

$L888:
	lw	$2,16($sp)
	andi	$2,$2,0x00ff
	addiu	$2,$2,8
	andi	$2,$2,0x00ff
	sb	$2,120($sp)
	addiu	$2,$sp,120
	move	$4,$2
	lui	$2,%hi(qdma_get_dbg_cntr_info)
	addiu	$2,$2,%lo(qdma_get_dbg_cntr_info)
	jalr	$2
	nop

	lw	$3,124($sp)
	li	$2,1			# 0x1
	bne	$3,$2,$L886
	nop

	lbu	$3,129($sp)
	li	$2,1			# 0x1
	beq	$3,$2,$L886
	nop

	lbu	$2,132($sp)
	move	$3,$2
	lw	$2,16($sp)
	beq	$3,$2,$L887
	nop

$L886:
	li	$2,-22			# 0xffffffffffffffea
	j	$L900
	nop

$L887:
	lw	$2,16($sp)
	sll	$2,$2,2
	addiu	$3,$sp,16
	addu	$2,$3,$2
	lw	$6,8($2)
	lw	$3,136($sp)
	lw	$2,16($sp)
	sll	$2,$2,2
	addiu	$4,$sp,16
	addu	$2,$4,$2
	lw	$2,8($2)
	subu	$3,$3,$2
	lui	$2,%hi($LC118)
	addiu	$4,$2,%lo($LC118)
	lw	$5,16($sp)
	move	$7,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lw	$2,16($sp)
	addiu	$2,$2,1
	sw	$2,16($sp)
$L885:
	lw	$2,16($sp)
	slt	$2,$2,8
	bne	$2,$0,$L888
	nop

	j	$L889
	nop

$L880:
	lui	$2,%hi($LC119)
	addiu	$4,$2,%lo($LC119)
	move	$5,$0
	li	$6,7			# 0x7
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	sw	$0,16($sp)
	j	$L890
	nop

$L893:
	lw	$2,16($sp)
	andi	$2,$2,0x00ff
	sb	$2,120($sp)
	addiu	$2,$sp,120
	move	$4,$2
	lui	$2,%hi(qdma_get_dbg_cntr_info)
	addiu	$2,$2,%lo(qdma_get_dbg_cntr_info)
	jalr	$2
	nop

	lw	$2,124($sp)
	bne	$2,$0,$L891
	nop

	lbu	$3,128($sp)
	li	$2,1			# 0x1
	beq	$3,$2,$L891
	nop

	lbu	$2,131($sp)
	move	$3,$2
	lw	$2,16($sp)
	beq	$3,$2,$L892
	nop

$L891:
	li	$2,-22			# 0xffffffffffffffea
	j	$L900
	nop

$L892:
	lw	$3,136($sp)
	lw	$2,16($sp)
	sll	$2,$2,2
	addiu	$4,$sp,16
	addu	$2,$4,$2
	sw	$3,40($2)
	lw	$2,16($sp)
	addiu	$2,$2,1
	sw	$2,16($sp)
$L890:
	lw	$2,16($sp)
	slt	$2,$2,8
	bne	$2,$0,$L893
	nop

	sw	$0,16($sp)
	j	$L894
	nop

$L897:
	lw	$2,16($sp)
	andi	$2,$2,0x00ff
	addiu	$2,$2,8
	andi	$2,$2,0x00ff
	sb	$2,120($sp)
	addiu	$2,$sp,120
	move	$4,$2
	lui	$2,%hi(qdma_get_dbg_cntr_info)
	addiu	$2,$2,%lo(qdma_get_dbg_cntr_info)
	jalr	$2
	nop

	lw	$3,124($sp)
	li	$2,1			# 0x1
	bne	$3,$2,$L895
	nop

	lbu	$3,128($sp)
	li	$2,1			# 0x1
	beq	$3,$2,$L895
	nop

	lbu	$2,131($sp)
	move	$3,$2
	lw	$2,16($sp)
	beq	$3,$2,$L896
	nop

$L895:
	li	$2,-22			# 0xffffffffffffffea
	j	$L900
	nop

$L896:
	lw	$2,16($sp)
	sll	$2,$2,2
	addiu	$3,$sp,16
	addu	$2,$3,$2
	lw	$6,40($2)
	lw	$3,136($sp)
	lw	$2,16($sp)
	sll	$2,$2,2
	addiu	$4,$sp,16
	addu	$2,$4,$2
	lw	$2,40($2)
	subu	$3,$3,$2
	lui	$2,%hi($LC120)
	addiu	$4,$2,%lo($LC120)
	lw	$5,16($sp)
	move	$7,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lw	$2,16($sp)
	addiu	$2,$2,1
	sw	$2,16($sp)
$L894:
	lw	$2,16($sp)
	slt	$2,$2,8
	bne	$2,$0,$L897
	nop

$L889:
	sw	$0,16($sp)
	j	$L898
	nop

$L899:
	lw	$2,16($sp)
	andi	$2,$2,0x00ff
	addiu	$2,$2,32
	andi	$2,$2,0x00ff
	sb	$2,120($sp)
	addiu	$2,$sp,120
	move	$4,$2
	lui	$2,%hi(qdma_get_dbg_cntr_info)
	addiu	$2,$2,%lo(qdma_get_dbg_cntr_info)
	jalr	$2
	nop

	lw	$3,136($sp)
	lw	$2,16($sp)
	sll	$2,$2,2
	addiu	$4,$sp,16
	addu	$2,$4,$2
	sw	$3,72($2)
	lw	$2,16($sp)
	addiu	$2,$2,1
	sw	$2,16($sp)
$L898:
	lw	$2,16($sp)
	slt	$2,$2,8
	bne	$2,$0,$L899
	nop

	lw	$5,88($sp)
	lw	$3,92($sp)
	lw	$2,88($sp)
	subu	$6,$3,$2
	lw	$3,92($sp)
	lui	$2,%hi($LC121)
	addiu	$4,$2,%lo($LC121)
	move	$7,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lw	$5,104($sp)
	lw	$3,108($sp)
	lui	$2,%hi($LC122)
	addiu	$4,$2,%lo($LC122)
	move	$6,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lw	$5,96($sp)
	lw	$6,100($sp)
	lw	$3,96($sp)
	lw	$2,100($sp)
	addu	$3,$3,$2
	lui	$2,%hi($LC123)
	addiu	$4,$2,%lo($LC123)
	move	$7,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lw	$5,112($sp)
	lw	$3,116($sp)
	lui	$2,%hi($LC124)
	addiu	$4,$2,%lo($LC124)
	move	$6,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	move	$2,$0
$L900:
	lw	$31,148($sp)
	addiu	$sp,$sp,152
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	qdma_dump_dbg_cntr_value
	.size	qdma_dump_dbg_cntr_value, .-qdma_dump_dbg_cntr_value
	.rdata
	.align	2
$LC125:
	.ascii	"packet mode\000"
	.align	2
$LC126:
	.ascii	"byte mode\000"
	.align	2
$LC127:
	.ascii	"64Byte\000"
	.align	2
$LC128:
	.ascii	"16Byte\000"
	.align	2
$LC129:
	.ascii	"weightBase is %s, weightScale is %s\012\000"
	.align	2
$LC130:
	.ascii	"channel-%d: qosType is %s\000"
	.align	2
$LC131:
	.ascii	", q7:q6:q5:q4:q3:q2:q1:q0=%d:%d:%d:%d:%d:%d:%d:%d\015\012"
	.ascii	"\000"
	.align	2
$LC132:
	.ascii	"\015\012\000"
	.align	2
$LC133:
	.ascii	", q6:q5:q4:q3:q2:q1:q0=%d:%d:%d:%d:%d:%d:%d\015\012\000"
	.align	2
$LC134:
	.ascii	", q5:q4:q3:q2:q1:q0=%d:%d:%d:%d:%d:%d\015\012\000"
	.align	2
$LC135:
	.ascii	", q4:q3:q2:q1:q0=%d:%d:%d:%d:%d\015\012\000"
	.align	2
$LC136:
	.ascii	", q3:q2:q1:q0=%d:%d:%d:%d\015\012\000"
	.align	2
$LC137:
	.ascii	", q2:q1:q0=%d:%d:%d\015\012\000"
	.align	2
$LC138:
	.ascii	", q1:q0=%d:%d\015\012\000"
	.section	.text.qdma_dump_tx_qos,"ax",@progbits
	.align	2
	.globl	qdma_dump_tx_qos
	.set	nomips16
	.set	nomicromips
	.ent	qdma_dump_tx_qos
	.type	qdma_dump_tx_qos, @function
qdma_dump_tx_qos:
	.frame	$sp,80,$31		# vars= 32, regs= 1/0, args= 40, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-80
	sw	$31,76($sp)
	sw	$4,80($sp)
	sw	$0,44($sp)
	sb	$0,40($sp)
	sb	$0,60($sp)
	sw	$0,48($sp)
	sw	$0,52($sp)
	lw	$2,80($sp)
	lw	$2,8($2)
	sw	$2,56($sp)
	lw	$4,80($sp)
	lui	$2,%hi(qdma_get_tx_qos_weight)
	addiu	$2,$2,%lo(qdma_get_tx_qos_weight)
	jalr	$2
	nop

	sw	$2,44($sp)
	lw	$2,80($sp)
	lw	$2,8($2)
	sw	$2,48($sp)
	lw	$2,80($sp)
	lw	$2,12($2)
	sw	$2,52($sp)
	lw	$2,48($sp)
	bne	$2,$0,$L902
	nop

	lui	$2,%hi($LC125)
	addiu	$3,$2,%lo($LC125)
	j	$L903
	nop

$L902:
	lui	$2,%hi($LC126)
	addiu	$3,$2,%lo($LC126)
$L903:
	lw	$2,52($sp)
	bne	$2,$0,$L904
	nop

	lui	$2,%hi($LC127)
	addiu	$2,$2,%lo($LC127)
	j	$L905
	nop

$L904:
	lui	$2,%hi($LC128)
	addiu	$2,$2,%lo($LC128)
$L905:
	lui	$4,%hi($LC129)
	addiu	$4,$4,%lo($LC129)
	move	$5,$3
	move	$6,$2
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	sb	$0,40($sp)
	j	$L906
	nop

$L922:
	lw	$3,56($sp)
	li	$2,8			# 0x8
	beq	$3,$2,$L907
	nop

	lbu	$3,40($sp)
	lw	$2,56($sp)
	beq	$3,$2,$L907
	nop

	j	$L908
	nop

$L907:
	lbu	$4,40($sp)
	addiu	$3,$sp,60
	addiu	$2,$sp,64
	move	$5,$3
	move	$6,$2
	lui	$2,%hi(qdmaGetTxQosScheduler)
	addiu	$2,$2,%lo(qdmaGetTxQosScheduler)
	jalr	$2
	nop

	sw	$2,44($sp)
	lw	$2,44($sp)
	bltz	$2,$L909
	nop

	lbu	$2,60($sp)
	sltu	$2,$2,9
	bne	$2,$0,$L910
	nop

$L909:
	li	$2,-14			# 0xfffffffffffffff2
	j	$L923
	nop

$L910:
	lbu	$5,40($sp)
	lbu	$2,60($sp)
	sll	$3,$2,4
	lui	$2,%hi(qosTypeStr)
	addiu	$2,$2,%lo(qosTypeStr)
	addu	$3,$3,$2
	lui	$2,%hi($LC130)
	addiu	$4,$2,%lo($LC130)
	move	$6,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lbu	$2,60($sp)
	sltu	$3,$2,8
	beq	$3,$0,$L912
	nop

	sll	$3,$2,2
	lui	$2,%hi($L914)
	addiu	$2,$2,%lo($L914)
	addu	$2,$3,$2
	lw	$2,0($2)
	j	$2
	nop

	.rdata
	.align	2
	.align	2
$L914:
	.word	$L913
	.word	$L915
	.word	$L916
	.word	$L917
	.word	$L918
	.word	$L919
	.word	$L920
	.word	$L921
	.section	.text.qdma_dump_tx_qos
$L913:
	lbu	$2,71($sp)
	move	$9,$2
	lbu	$2,70($sp)
	move	$8,$2
	lbu	$2,69($sp)
	move	$7,$2
	lbu	$2,68($sp)
	move	$6,$2
	lbu	$2,67($sp)
	move	$5,$2
	lbu	$2,66($sp)
	move	$4,$2
	lbu	$2,65($sp)
	move	$3,$2
	lbu	$2,64($sp)
	sw	$6,16($sp)
	sw	$5,20($sp)
	sw	$4,24($sp)
	sw	$3,28($sp)
	sw	$2,32($sp)
	lui	$2,%hi($LC131)
	addiu	$4,$2,%lo($LC131)
	move	$5,$9
	move	$6,$8
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	j	$L908
	nop

$L915:
	lui	$2,%hi($LC132)
	addiu	$4,$2,%lo($LC132)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	j	$L908
	nop

$L916:
	lbu	$2,70($sp)
	move	$8,$2
	lbu	$2,69($sp)
	move	$6,$2
	lbu	$2,68($sp)
	move	$7,$2
	lbu	$2,67($sp)
	move	$5,$2
	lbu	$2,66($sp)
	move	$4,$2
	lbu	$2,65($sp)
	move	$3,$2
	lbu	$2,64($sp)
	sw	$5,16($sp)
	sw	$4,20($sp)
	sw	$3,24($sp)
	sw	$2,28($sp)
	lui	$2,%hi($LC133)
	addiu	$4,$2,%lo($LC133)
	move	$5,$8
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	j	$L908
	nop

$L917:
	lbu	$2,69($sp)
	move	$5,$2
	lbu	$2,68($sp)
	move	$6,$2
	lbu	$2,67($sp)
	move	$7,$2
	lbu	$2,66($sp)
	move	$4,$2
	lbu	$2,65($sp)
	move	$3,$2
	lbu	$2,64($sp)
	sw	$4,16($sp)
	sw	$3,20($sp)
	sw	$2,24($sp)
	lui	$2,%hi($LC134)
	addiu	$4,$2,%lo($LC134)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	j	$L908
	nop

$L918:
	lbu	$2,68($sp)
	move	$5,$2
	lbu	$2,67($sp)
	move	$6,$2
	lbu	$2,66($sp)
	move	$7,$2
	lbu	$2,65($sp)
	move	$3,$2
	lbu	$2,64($sp)
	sw	$3,16($sp)
	sw	$2,20($sp)
	lui	$2,%hi($LC135)
	addiu	$4,$2,%lo($LC135)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	j	$L908
	nop

$L919:
	lbu	$2,67($sp)
	move	$5,$2
	lbu	$2,66($sp)
	move	$6,$2
	lbu	$2,65($sp)
	move	$3,$2
	lbu	$2,64($sp)
	sw	$2,16($sp)
	lui	$2,%hi($LC136)
	addiu	$4,$2,%lo($LC136)
	move	$7,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	j	$L908
	nop

$L920:
	lbu	$2,66($sp)
	move	$5,$2
	lbu	$2,65($sp)
	move	$6,$2
	lbu	$2,64($sp)
	move	$3,$2
	lui	$2,%hi($LC137)
	addiu	$4,$2,%lo($LC137)
	move	$7,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	j	$L908
	nop

$L921:
	lbu	$2,65($sp)
	move	$5,$2
	lbu	$2,64($sp)
	move	$3,$2
	lui	$2,%hi($LC138)
	addiu	$4,$2,%lo($LC138)
	move	$6,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	j	$L908
	nop

$L912:
	li	$2,-14			# 0xfffffffffffffff2
	j	$L923
	nop

$L908:
	lbu	$2,40($sp)
	addiu	$2,$2,1
	sb	$2,40($sp)
$L906:
	lbu	$2,40($sp)
	sltu	$2,$2,8
	bne	$2,$0,$L922
	nop

	move	$2,$0
$L923:
	lw	$31,76($sp)
	addiu	$sp,$sp,80
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	qdma_dump_tx_qos
	.size	qdma_dump_tx_qos, .-qdma_dump_tx_qos
	.rdata
	.align	2
$LC139:
	.ascii	"virtual channel mapping mode is disable.\012\000"
	.align	2
$LC140:
	.ascii	"32Queue\000"
	.align	2
$LC141:
	.ascii	"16Queue\000"
	.align	2
$LC142:
	.ascii	"virtual channel mapping mode is enable, virChnlMode is %"
	.ascii	"s.\012\000"
	.align	2
$LC143:
	.ascii	"physical channel-%d: qosType:%s\000"
	.align	2
$LC144:
	.ascii	", vChnl3:vChnl2:vChnl1:vChnl0=%d:%d:%d:%d\015\012\000"
	.align	2
$LC145:
	.ascii	", vChnl1:vChnl0=%d:%d\015\012\000"
	.align	2
$LC146:
	.ascii	", vChnl2:vChnl1:vChnl0=%d:%d:%d\015\012\000"
	.section	.text.qdma_dump_virtual_channel_qos,"ax",@progbits
	.align	2
	.globl	qdma_dump_virtual_channel_qos
	.set	nomips16
	.set	nomicromips
	.ent	qdma_dump_virtual_channel_qos
	.type	qdma_dump_virtual_channel_qos, @function
qdma_dump_virtual_channel_qos:
	.frame	$sp,80,$31		# vars= 48, regs= 1/0, args= 24, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-80
	sw	$31,76($sp)
	sw	$4,80($sp)
	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lw	$2,0($2)
	sw	$2,32($sp)
	sw	$0,36($sp)
	sb	$0,56($sp)
	sw	$0,24($sp)
	sw	$0,40($sp)
	sw	$0,28($sp)
	lw	$2,80($sp)
	lw	$2,8($2)
	sw	$2,44($sp)
	lw	$2,32($sp)
	addiu	$2,$2,4
	sw	$2,48($sp)
	lw	$2,48($sp)
	lw	$3,0($2)
	li	$2,33554432			# 0x2000000
	and	$2,$3,$2
	bne	$2,$0,$L926
	nop

	lui	$2,%hi($LC139)
	addiu	$4,$2,%lo($LC139)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	move	$2,$0
	j	$L947
	nop

$L926:
	lw	$2,32($sp)
	addiu	$2,$2,4
	sw	$2,52($sp)
	lw	$2,52($sp)
	lw	$3,0($2)
	li	$2,16777216			# 0x1000000
	and	$2,$3,$2
	beq	$2,$0,$L929
	nop

	li	$2,4			# 0x4
	sw	$2,28($sp)
	j	$L930
	nop

$L929:
	li	$2,2			# 0x2
	sw	$2,28($sp)
$L930:
	li	$3,8			# 0x8
	lw	$2,28($sp)
	div	$0,$3,$2
	mfhi	$3
	mflo	$2
	sw	$2,40($sp)
	lw	$3,28($sp)
	li	$2,4			# 0x4
	bne	$3,$2,$L931
	nop

	lui	$2,%hi($LC140)
	addiu	$2,$2,%lo($LC140)
	j	$L932
	nop

$L931:
	lui	$2,%hi($LC141)
	addiu	$2,$2,%lo($LC141)
$L932:
	lui	$3,%hi($LC142)
	addiu	$4,$3,%lo($LC142)
	move	$5,$2
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	sw	$0,24($sp)
	j	$L933
	nop

$L946:
	lw	$3,44($sp)
	li	$2,8			# 0x8
	beq	$3,$2,$L934
	nop

	lw	$3,44($sp)
	lw	$2,24($sp)
	beq	$3,$2,$L934
	nop

	j	$L935
	nop

$L934:
	lw	$2,24($sp)
	andi	$2,$2,0x00ff
	move	$4,$2
	lw	$2,28($sp)
	andi	$2,$2,0x00ff
	move	$5,$2
	addiu	$3,$sp,56
	addiu	$2,$sp,60
	move	$6,$3
	move	$7,$2
	lui	$2,%hi(qdmaGetVirtualChannelQos)
	addiu	$2,$2,%lo(qdmaGetVirtualChannelQos)
	jalr	$2
	nop

	sw	$2,36($sp)
	lw	$2,36($sp)
	bltz	$2,$L936
	nop

	lbu	$2,56($sp)
	move	$3,$2
	lw	$2,28($sp)
	slt	$2,$2,$3
	beq	$2,$0,$L937
	nop

$L936:
	li	$2,-14			# 0xfffffffffffffff2
	j	$L947
	nop

$L937:
	lbu	$2,56($sp)
	sll	$3,$2,4
	lui	$2,%hi(vChnlQosTypeStr)
	addiu	$2,$2,%lo(vChnlQosTypeStr)
	addu	$3,$3,$2
	lui	$2,%hi($LC143)
	addiu	$4,$2,%lo($LC143)
	lw	$5,24($sp)
	move	$6,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lbu	$2,56($sp)
	li	$3,1			# 0x1
	beq	$2,$3,$L939
	nop

	slt	$3,$2,2
	beq	$3,$0,$L940
	nop

	beq	$2,$0,$L941
	nop

	j	$L938
	nop

$L940:
	li	$3,2			# 0x2
	beq	$2,$3,$L942
	nop

	li	$3,3			# 0x3
	beq	$2,$3,$L943
	nop

	j	$L938
	nop

$L941:
	lw	$3,28($sp)
	li	$2,4			# 0x4
	bne	$3,$2,$L944
	nop

	lbu	$2,63($sp)
	move	$5,$2
	lbu	$2,62($sp)
	move	$6,$2
	lbu	$2,61($sp)
	move	$3,$2
	lbu	$2,60($sp)
	sw	$2,16($sp)
	lui	$2,%hi($LC144)
	addiu	$4,$2,%lo($LC144)
	move	$7,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	j	$L935
	nop

$L944:
	lbu	$2,61($sp)
	move	$5,$2
	lbu	$2,60($sp)
	move	$3,$2
	lui	$2,%hi($LC145)
	addiu	$4,$2,%lo($LC145)
	move	$6,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	j	$L935
	nop

$L939:
	lui	$2,%hi($LC132)
	addiu	$4,$2,%lo($LC132)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	j	$L935
	nop

$L942:
	lbu	$2,62($sp)
	move	$5,$2
	lbu	$2,61($sp)
	move	$6,$2
	lbu	$2,60($sp)
	move	$3,$2
	lui	$2,%hi($LC146)
	addiu	$4,$2,%lo($LC146)
	move	$7,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	j	$L935
	nop

$L943:
	lbu	$2,61($sp)
	move	$5,$2
	lbu	$2,60($sp)
	move	$3,$2
	lui	$2,%hi($LC145)
	addiu	$4,$2,%lo($LC145)
	move	$6,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	j	$L935
	nop

$L938:
	li	$2,-14			# 0xfffffffffffffff2
	j	$L947
	nop

$L935:
	lw	$2,24($sp)
	addiu	$2,$2,1
	sw	$2,24($sp)
$L933:
	lw	$3,24($sp)
	lw	$2,40($sp)
	slt	$2,$3,$2
	bne	$2,$0,$L946
	nop

	move	$2,$0
$L947:
	lw	$31,76($sp)
	addiu	$sp,$sp,80
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	qdma_dump_virtual_channel_qos
	.size	qdma_dump_virtual_channel_qos, .-qdma_dump_virtual_channel_qos
	.rdata
	.align	2
$LC147:
	.ascii	"Enable\000"
	.align	2
$LC148:
	.ascii	"Disable\000"
	.align	2
$LC149:
	.ascii	"TX-RateLimit: mode:%s, tickSel:%dus, unit:%dkbps, buckse"
	.ascii	"tScale:%uByte\015\012\000"
	.align	2
$LC150:
	.ascii	"\015\012Channel-%d: mode:%s\000"
	.align	2
$LC151:
	.ascii	", ratelimit value:%ukbps, PBS:%uByte\000"
	.section	.text.qdma_dump_tx_ratelimit,"ax",@progbits
	.align	2
	.globl	qdma_dump_tx_ratelimit
	.set	nomips16
	.set	nomicromips
	.ent	qdma_dump_tx_ratelimit
	.type	qdma_dump_tx_ratelimit, @function
qdma_dump_tx_ratelimit:
	.frame	$sp,88,$31		# vars= 56, regs= 1/0, args= 24, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-88
	sw	$31,84($sp)
	sw	$4,88($sp)
	sw	$0,32($sp)
	sw	$0,24($sp)
	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lw	$2,0($2)
	sw	$2,36($sp)
	sw	$0,40($sp)
	sw	$0,28($sp)
	sw	$0,44($sp)
	sw	$0,48($sp)
	sw	$0,52($sp)
	sw	$0,56($sp)
	lw	$2,88($sp)
	lw	$2,8($2)
	sw	$2,60($sp)
	lw	$2,36($sp)
	addiu	$2,$2,152
	sw	$2,64($sp)
	lw	$2,64($sp)
	lw	$2,0($2)
	andi	$2,$2,0xffff
	sw	$2,40($sp)
	lw	$2,40($sp)
	bne	$2,$0,$L950
	nop

	sw	$0,28($sp)
	j	$L951
	nop

$L950:
	li	$3,8000			# 0x1f40
	lw	$2,40($sp)
	div	$0,$3,$2
	mfhi	$3
	mflo	$2
	sw	$2,28($sp)
$L951:
	lw	$2,36($sp)
	addiu	$2,$2,152
	sw	$2,68($sp)
	lw	$2,68($sp)
	lw	$2,0($2)
	move	$3,$2
	li	$2,-2147483648			# 0xffffffff80000000
	and	$2,$3,$2
	sw	$2,44($sp)
	lw	$2,36($sp)
	addiu	$2,$2,152
	sw	$2,72($sp)
	lw	$2,72($sp)
	lw	$3,0($2)
	li	$2,983040			# 0xf0000
	and	$2,$3,$2
	srl	$2,$2,16
	sw	$2,56($sp)
	lw	$2,44($sp)
	beq	$2,$0,$L954
	nop

	lui	$2,%hi($LC147)
	addiu	$2,$2,%lo($LC147)
	j	$L955
	nop

$L954:
	lui	$2,%hi($LC148)
	addiu	$2,$2,%lo($LC148)
$L955:
	lw	$3,56($sp)
	li	$4,1			# 0x1
	sll	$3,$4,$3
	sw	$3,16($sp)
	lui	$3,%hi($LC149)
	addiu	$4,$3,%lo($LC149)
	move	$5,$2
	lw	$6,40($sp)
	lw	$7,28($sp)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lw	$2,44($sp)
	bne	$2,$0,$L956
	nop

	move	$2,$0
	j	$L957
	nop

$L956:
	sw	$0,24($sp)
	j	$L958
	nop

$L966:
	lw	$3,60($sp)
	li	$2,8			# 0x8
	beq	$3,$2,$L959
	nop

	lw	$3,60($sp)
	lw	$2,24($sp)
	beq	$3,$2,$L959
	nop

	j	$L960
	nop

$L959:
	lw	$2,36($sp)
	addiu	$2,$2,352
	sw	$2,76($sp)
	lw	$2,76($sp)
	lw	$2,0($2)
	li	$4,1			# 0x1
	lw	$3,24($sp)
	sll	$3,$4,$3
	and	$2,$2,$3
	sw	$2,44($sp)
	lw	$2,24($sp)
	andi	$2,$2,0x00ff
	move	$4,$2
	li	$5,2			# 0x2
	lui	$2,%hi(qdmaGetTxRateLimitConfig)
	addiu	$2,$2,%lo(qdmaGetTxRateLimitConfig)
	jalr	$2
	nop

	sw	$2,32($sp)
	lw	$2,32($sp)
	bgez	$2,$L962
	nop

	lw	$2,32($sp)
	j	$L957
	nop

$L962:
	lw	$3,32($sp)
	lw	$2,28($sp)
	mul	$2,$3,$2
	sw	$2,48($sp)
	lw	$2,24($sp)
	andi	$2,$2,0x00ff
	move	$4,$2
	li	$5,3			# 0x3
	lui	$2,%hi(qdmaGetTxRateLimitConfig)
	addiu	$2,$2,%lo(qdmaGetTxRateLimitConfig)
	jalr	$2
	nop

	sw	$2,32($sp)
	lw	$2,32($sp)
	bgez	$2,$L963
	nop

	lw	$2,32($sp)
	j	$L957
	nop

$L963:
	lw	$2,56($sp)
	lw	$3,32($sp)
	sll	$2,$3,$2
	sw	$2,52($sp)
	lw	$2,44($sp)
	beq	$2,$0,$L964
	nop

	lui	$2,%hi($LC147)
	addiu	$2,$2,%lo($LC147)
	j	$L965
	nop

$L964:
	lui	$2,%hi($LC148)
	addiu	$2,$2,%lo($LC148)
$L965:
	lui	$3,%hi($LC150)
	addiu	$4,$3,%lo($LC150)
	lw	$5,24($sp)
	move	$6,$2
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lw	$2,44($sp)
	beq	$2,$0,$L960
	nop

	lui	$2,%hi($LC151)
	addiu	$4,$2,%lo($LC151)
	lw	$5,48($sp)
	lw	$6,52($sp)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L960:
	lw	$2,24($sp)
	addiu	$2,$2,1
	sw	$2,24($sp)
$L958:
	lw	$2,24($sp)
	slt	$2,$2,8
	bne	$2,$0,$L966
	nop

	lui	$2,%hi($LC132)
	addiu	$4,$2,%lo($LC132)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	move	$2,$0
$L957:
	lw	$31,84($sp)
	addiu	$sp,$sp,88
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	qdma_dump_tx_ratelimit
	.size	qdma_dump_tx_ratelimit, .-qdma_dump_tx_ratelimit
	.rdata
	.align	2
$LC152:
	.ascii	"packet\000"
	.align	2
$LC153:
	.ascii	"byte\000"
	.align	2
$LC154:
	.ascii	"pps\000"
	.align	2
$LC155:
	.ascii	"kbps\000"
	.align	2
$LC156:
	.ascii	"RX-RateLimit: enMode:%s, pktMode:%s, tickSel:%dus, unit:"
	.ascii	"%d%s, bucksetScale:%uByte\015\012\000"
	.align	2
$LC157:
	.ascii	"RingIdx-%d: ratelimit value:%u%s, PBS:%uByte\015\012\000"
	.section	.text.qdma_dump_rx_ratelimit,"ax",@progbits
	.align	2
	.globl	qdma_dump_rx_ratelimit
	.set	nomips16
	.set	nomicromips
	.ent	qdma_dump_rx_ratelimit
	.type	qdma_dump_rx_ratelimit, @function
qdma_dump_rx_ratelimit:
	.frame	$sp,104,$31		# vars= 64, regs= 1/0, args= 32, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-104
	sw	$31,100($sp)
	sw	$4,104($sp)
	sw	$0,44($sp)
	sw	$0,32($sp)
	sw	$0,36($sp)
	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lw	$2,0($2)
	sw	$2,48($sp)
	sw	$0,52($sp)
	sw	$0,40($sp)
	sw	$0,56($sp)
	sw	$0,60($sp)
	sw	$0,64($sp)
	sw	$0,68($sp)
	sw	$0,72($sp)
	lw	$2,104($sp)
	lw	$2,8($2)
	sw	$2,76($sp)
	lw	$2,48($sp)
	addiu	$2,$2,288
	sw	$2,80($sp)
	lw	$2,80($sp)
	lw	$3,0($2)
	li	$2,1073741824			# 0x40000000
	and	$2,$3,$2
	sw	$2,56($sp)
	lw	$2,48($sp)
	addiu	$2,$2,288
	sw	$2,84($sp)
	lw	$2,84($sp)
	lw	$3,0($2)
	li	$2,251658240			# 0xf000000
	and	$2,$3,$2
	srl	$2,$2,24
	sw	$2,72($sp)
	lw	$2,48($sp)
	addiu	$2,$2,288
	sw	$2,88($sp)
	lw	$2,88($sp)
	lw	$3,0($2)
	li	$2,536870912			# 0x20000000
	and	$2,$3,$2
	sw	$2,60($sp)
	lw	$2,60($sp)
	beq	$2,$0,$L971
	nop

	li	$2,983040			# 0xf0000
	ori	$2,$2,0x4240
	sw	$2,36($sp)
	j	$L972
	nop

$L971:
	li	$2,8000			# 0x1f40
	sw	$2,36($sp)
$L972:
	lw	$2,48($sp)
	addiu	$2,$2,288
	sw	$2,92($sp)
	lw	$2,92($sp)
	lw	$2,0($2)
	andi	$2,$2,0xffff
	sw	$2,52($sp)
	lw	$2,52($sp)
	bne	$2,$0,$L974
	nop

	sw	$0,40($sp)
	j	$L975
	nop

$L974:
	lw	$3,36($sp)
	lw	$2,52($sp)
	div	$0,$3,$2
	mfhi	$3
	mflo	$2
	sw	$2,40($sp)
$L975:
	lw	$2,56($sp)
	beq	$2,$0,$L976
	nop

	lui	$2,%hi($LC147)
	addiu	$5,$2,%lo($LC147)
	j	$L977
	nop

$L976:
	lui	$2,%hi($LC148)
	addiu	$5,$2,%lo($LC148)
$L977:
	lw	$2,60($sp)
	beq	$2,$0,$L978
	nop

	lui	$2,%hi($LC152)
	addiu	$3,$2,%lo($LC152)
	j	$L979
	nop

$L978:
	lui	$2,%hi($LC153)
	addiu	$3,$2,%lo($LC153)
$L979:
	lw	$2,60($sp)
	beq	$2,$0,$L980
	nop

	lui	$2,%hi($LC154)
	addiu	$2,$2,%lo($LC154)
	j	$L981
	nop

$L980:
	lui	$2,%hi($LC155)
	addiu	$2,$2,%lo($LC155)
$L981:
	lw	$4,72($sp)
	li	$6,1			# 0x1
	sll	$4,$6,$4
	lw	$6,40($sp)
	sw	$6,16($sp)
	sw	$2,20($sp)
	sw	$4,24($sp)
	lui	$2,%hi($LC156)
	addiu	$4,$2,%lo($LC156)
	move	$6,$3
	lw	$7,52($sp)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lw	$2,56($sp)
	bne	$2,$0,$L982
	nop

	move	$2,$0
	j	$L983
	nop

$L982:
	sw	$0,32($sp)
	j	$L984
	nop

$L991:
	lw	$3,76($sp)
	li	$2,2			# 0x2
	beq	$3,$2,$L985
	nop

	lw	$3,76($sp)
	lw	$2,32($sp)
	beq	$3,$2,$L985
	nop

	j	$L986
	nop

$L985:
	lw	$2,32($sp)
	andi	$2,$2,0x00ff
	move	$4,$2
	move	$5,$0
	lui	$2,%hi(qdmaGetRxRateLimitConfig)
	addiu	$2,$2,%lo(qdmaGetRxRateLimitConfig)
	jalr	$2
	nop

	sw	$2,44($sp)
	lw	$2,44($sp)
	bgez	$2,$L987
	nop

	lw	$2,44($sp)
	j	$L983
	nop

$L987:
	lw	$3,44($sp)
	lw	$2,40($sp)
	mul	$2,$3,$2
	sw	$2,64($sp)
	lw	$2,32($sp)
	andi	$2,$2,0x00ff
	move	$4,$2
	li	$5,1			# 0x1
	lui	$2,%hi(qdmaGetRxRateLimitConfig)
	addiu	$2,$2,%lo(qdmaGetRxRateLimitConfig)
	jalr	$2
	nop

	sw	$2,44($sp)
	lw	$2,44($sp)
	bgez	$2,$L988
	nop

	lw	$2,44($sp)
	j	$L983
	nop

$L988:
	lw	$2,72($sp)
	lw	$3,44($sp)
	sll	$2,$3,$2
	sw	$2,68($sp)
	lw	$2,60($sp)
	beq	$2,$0,$L989
	nop

	lui	$2,%hi($LC154)
	addiu	$2,$2,%lo($LC154)
	j	$L990
	nop

$L989:
	lui	$2,%hi($LC155)
	addiu	$2,$2,%lo($LC155)
$L990:
	lw	$3,68($sp)
	sw	$3,16($sp)
	lui	$3,%hi($LC157)
	addiu	$4,$3,%lo($LC157)
	lw	$5,32($sp)
	lw	$6,64($sp)
	move	$7,$2
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L986:
	lw	$2,32($sp)
	addiu	$2,$2,1
	sw	$2,32($sp)
$L984:
	lw	$2,32($sp)
	slt	$2,$2,2
	bne	$2,$0,$L991
	nop

	move	$2,$0
$L983:
	lw	$31,100($sp)
	addiu	$sp,$sp,104
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	qdma_dump_rx_ratelimit
	.size	qdma_dump_rx_ratelimit, .-qdma_dump_rx_ratelimit
	.rdata
	.align	2
$LC158:
	.ascii	"TX-DBA Report: tickSel:%dus, unit:%dkbps, bucksetScale:%"
	.ascii	"uByte\015\012\000"
	.align	2
$LC159:
	.ascii	"Channel-%d: CIR:%ukbps, CBS:%uByte, PIR:%ukbps, PBS:%uBy"
	.ascii	"te\015\012\000"
	.section	.text.qdma_dump_tx_dba_report,"ax",@progbits
	.align	2
	.globl	qdma_dump_tx_dba_report
	.set	nomips16
	.set	nomicromips
	.ent	qdma_dump_tx_dba_report
	.type	qdma_dump_tx_dba_report, @function
qdma_dump_tx_dba_report:
	.frame	$sp,88,$31		# vars= 56, regs= 1/0, args= 24, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-88
	sw	$31,84($sp)
	sw	$4,88($sp)
	sw	$0,32($sp)
	sw	$0,24($sp)
	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lw	$2,0($2)
	sw	$2,36($sp)
	sw	$0,40($sp)
	sw	$0,28($sp)
	sw	$0,44($sp)
	sw	$0,48($sp)
	sw	$0,52($sp)
	sw	$0,56($sp)
	sw	$0,60($sp)
	lw	$2,88($sp)
	lw	$2,8($2)
	sw	$2,64($sp)
	lw	$2,36($sp)
	addiu	$2,$2,152
	sw	$2,68($sp)
	lw	$2,68($sp)
	lw	$2,0($2)
	andi	$2,$2,0xffff
	sw	$2,40($sp)
	lw	$2,40($sp)
	bne	$2,$0,$L994
	nop

	sw	$0,28($sp)
	j	$L995
	nop

$L994:
	li	$3,8000			# 0x1f40
	lw	$2,40($sp)
	div	$0,$3,$2
	mfhi	$3
	mflo	$2
	sw	$2,28($sp)
$L995:
	lw	$2,36($sp)
	addiu	$2,$2,152
	sw	$2,72($sp)
	lw	$2,72($sp)
	lw	$3,0($2)
	li	$2,983040			# 0xf0000
	and	$2,$3,$2
	srl	$2,$2,16
	sw	$2,60($sp)
	lw	$2,60($sp)
	li	$3,1			# 0x1
	sll	$3,$3,$2
	lui	$2,%hi($LC158)
	addiu	$4,$2,%lo($LC158)
	lw	$5,40($sp)
	lw	$6,28($sp)
	move	$7,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	sw	$0,24($sp)
	j	$L997
	nop

$L1005:
	lw	$3,64($sp)
	li	$2,8			# 0x8
	beq	$3,$2,$L998
	nop

	lw	$3,64($sp)
	lw	$2,24($sp)
	beq	$3,$2,$L998
	nop

	j	$L999
	nop

$L998:
	lw	$2,24($sp)
	andi	$2,$2,0x00ff
	move	$4,$2
	li	$5,2			# 0x2
	lui	$2,%hi(qdmaGetTxRateLimitConfig)
	addiu	$2,$2,%lo(qdmaGetTxRateLimitConfig)
	jalr	$2
	nop

	sw	$2,32($sp)
	lw	$2,32($sp)
	bgez	$2,$L1000
	nop

	lw	$2,32($sp)
	j	$L1001
	nop

$L1000:
	lw	$3,32($sp)
	lw	$2,28($sp)
	mul	$2,$3,$2
	sw	$2,44($sp)
	lw	$2,24($sp)
	andi	$2,$2,0x00ff
	move	$4,$2
	li	$5,3			# 0x3
	lui	$2,%hi(qdmaGetTxRateLimitConfig)
	addiu	$2,$2,%lo(qdmaGetTxRateLimitConfig)
	jalr	$2
	nop

	sw	$2,32($sp)
	lw	$2,32($sp)
	bgez	$2,$L1002
	nop

	lw	$2,32($sp)
	j	$L1001
	nop

$L1002:
	lw	$2,60($sp)
	lw	$3,32($sp)
	sll	$2,$3,$2
	sw	$2,48($sp)
	lw	$2,24($sp)
	andi	$2,$2,0x00ff
	move	$4,$2
	move	$5,$0
	lui	$2,%hi(qdmaGetTxRateLimitConfig)
	addiu	$2,$2,%lo(qdmaGetTxRateLimitConfig)
	jalr	$2
	nop

	sw	$2,32($sp)
	lw	$2,32($sp)
	bgez	$2,$L1003
	nop

	lw	$2,32($sp)
	j	$L1001
	nop

$L1003:
	lw	$3,32($sp)
	lw	$2,28($sp)
	mul	$2,$3,$2
	sw	$2,52($sp)
	lw	$2,24($sp)
	andi	$2,$2,0x00ff
	move	$4,$2
	li	$5,1			# 0x1
	lui	$2,%hi(qdmaGetTxRateLimitConfig)
	addiu	$2,$2,%lo(qdmaGetTxRateLimitConfig)
	jalr	$2
	nop

	sw	$2,32($sp)
	lw	$2,32($sp)
	bgez	$2,$L1004
	nop

	lw	$2,32($sp)
	j	$L1001
	nop

$L1004:
	lw	$2,60($sp)
	lw	$3,32($sp)
	sll	$2,$3,$2
	sw	$2,56($sp)
	lw	$2,44($sp)
	sw	$2,16($sp)
	lw	$2,48($sp)
	sw	$2,20($sp)
	lui	$2,%hi($LC159)
	addiu	$4,$2,%lo($LC159)
	lw	$5,24($sp)
	lw	$6,52($sp)
	lw	$7,56($sp)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L999:
	lw	$2,24($sp)
	addiu	$2,$2,1
	sw	$2,24($sp)
$L997:
	lw	$2,24($sp)
	slt	$2,$2,8
	bne	$2,$0,$L1005
	nop

	move	$2,$0
$L1001:
	lw	$31,84($sp)
	addiu	$sp,$sp,88
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	qdma_dump_tx_dba_report
	.size	qdma_dump_tx_dba_report, .-qdma_dump_tx_dba_report
	.rdata
	.align	2
$LC160:
	.ascii	"/*************************************** TXQ CNGST BEGIN"
	.ascii	" ************************************/\012\000"
	.align	2
$LC161:
	.ascii	"txq cngst drop is %s\012\000"
	.align	2
$LC162:
	.ascii	"txq cngst dei drop is %s\012\000"
	.align	2
$LC163:
	.ascii	"when total tx buffer usage greater than total_max_thresh"
	.ascii	"old, tx ring1 will be blocked:  %s\012\000"
	.align	2
$LC164:
	.ascii	"when total tx buffer usage greater than total_min_thresh"
	.ascii	"old, tx ring1 will be blocked:  %s\012\000"
	.align	2
$LC165:
	.ascii	"when total tx buffer usage greater than total_max_thresh"
	.ascii	"old, tx ring0 will be blocked:  %s\012\000"
	.align	2
$LC166:
	.ascii	"when total tx buffer usage greater than total_min_thresh"
	.ascii	"old, tx ring0 will be blocked:  %s\012\000"
	.align	2
$LC167:
	.ascii	"\012txq cngst mode is dynamic\012\000"
	.align	2
$LC168:
	.ascii	"total max threshold: %ukByte\012\000"
	.align	2
$LC169:
	.ascii	"total min threshold: %ukByte\012\000"
	.align	2
$LC170:
	.ascii	"channel max threshold: %ukByte\012\000"
	.align	2
$LC171:
	.ascii	"channel min threshold: %ukByte\012\000"
	.align	2
$LC172:
	.ascii	"queue max threshold: %ukByte\012\000"
	.align	2
$LC173:
	.ascii	"queue min threshold: %ukByte\012\000"
	.align	2
$LC174:
	.ascii	"DEI packet threshold equal 1/%d normal packet\012\000"
	.align	2
$LC175:
	.ascii	"dynamic congetion threshold will be updateed after WRR w"
	.ascii	"eight changed:  %s\012\000"
	.align	2
$LC176:
	.ascii	"dynamic congetion threshold will be updateed after packe"
	.ascii	"t receive or transmit:  %s\012\000"
	.align	2
$LC177:
	.ascii	"dynamic congetion threshold will be updateed after ticks"
	.ascii	"el %sus arrived:  %s\012\000"
	.align	2
$LC178:
	.ascii	"Peek Rate Function is Enable, Duration is %ums, Margin i"
	.ascii	"s %d\012\000"
	.align	2
$LC179:
	.ascii	"Peek Rate Function is Disable\012\000"
	.align	2
$LC180:
	.ascii	"\012txq cngst mode is static\012\000"
	.align	2
$LC181:
	.ascii	"\011Normal Threshold\011DEI Threshold\012\000"
	.align	2
$LC182:
	.ascii	"Q%d\011%ukByte\011\011\011%ukByte\012\000"
	.align	2
$LC183:
	.ascii	"/**************************************** TXQ CNGST END "
	.ascii	"*************************************/\012\000"
	.section	.text.qdma_dump_txq_cngst,"ax",@progbits
	.align	2
	.globl	qdma_dump_txq_cngst
	.set	nomips16
	.set	nomicromips
	.ent	qdma_dump_txq_cngst
	.type	qdma_dump_txq_cngst, @function
qdma_dump_txq_cngst:
	.frame	$sp,152,$31		# vars= 128, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-152
	sw	$31,148($sp)
	sw	$4,152($sp)
	sw	$0,16($sp)
	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lw	$2,0($2)
	sw	$2,20($sp)
	addiu	$2,$sp,28
	move	$4,$2
	lui	$2,%hi(qdma_get_txq_cngst_cfg)
	addiu	$2,$2,%lo(qdma_get_txq_cngst_cfg)
	jalr	$2
	nop

	lui	$2,%hi($LC160)
	addiu	$4,$2,%lo($LC160)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lbu	$2,28($sp)
	beq	$2,$0,$L1007
	nop

	lui	$2,%hi($LC147)
	addiu	$2,$2,%lo($LC147)
	j	$L1008
	nop

$L1007:
	lui	$2,%hi($LC148)
	addiu	$2,$2,%lo($LC148)
$L1008:
	lui	$3,%hi($LC161)
	addiu	$4,$3,%lo($LC161)
	move	$5,$2
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lbu	$2,29($sp)
	beq	$2,$0,$L1009
	nop

	lui	$2,%hi($LC147)
	addiu	$2,$2,%lo($LC147)
	j	$L1010
	nop

$L1009:
	lui	$2,%hi($LC148)
	addiu	$2,$2,%lo($LC148)
$L1010:
	lui	$3,%hi($LC162)
	addiu	$4,$3,%lo($LC162)
	move	$5,$2
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lbu	$2,31($sp)
	beq	$2,$0,$L1011
	nop

	lui	$2,%hi($LC147)
	addiu	$2,$2,%lo($LC147)
	j	$L1012
	nop

$L1011:
	lui	$2,%hi($LC148)
	addiu	$2,$2,%lo($LC148)
$L1012:
	lui	$3,%hi($LC163)
	addiu	$4,$3,%lo($LC163)
	move	$5,$2
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lbu	$2,32($sp)
	beq	$2,$0,$L1013
	nop

	lui	$2,%hi($LC147)
	addiu	$2,$2,%lo($LC147)
	j	$L1014
	nop

$L1013:
	lui	$2,%hi($LC148)
	addiu	$2,$2,%lo($LC148)
$L1014:
	lui	$3,%hi($LC164)
	addiu	$4,$3,%lo($LC164)
	move	$5,$2
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lbu	$2,33($sp)
	beq	$2,$0,$L1015
	nop

	lui	$2,%hi($LC147)
	addiu	$2,$2,%lo($LC147)
	j	$L1016
	nop

$L1015:
	lui	$2,%hi($LC148)
	addiu	$2,$2,%lo($LC148)
$L1016:
	lui	$3,%hi($LC165)
	addiu	$4,$3,%lo($LC165)
	move	$5,$2
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lbu	$2,34($sp)
	beq	$2,$0,$L1017
	nop

	lui	$2,%hi($LC147)
	addiu	$2,$2,%lo($LC147)
	j	$L1018
	nop

$L1017:
	lui	$2,%hi($LC148)
	addiu	$2,$2,%lo($LC148)
$L1018:
	lui	$3,%hi($LC166)
	addiu	$4,$3,%lo($LC166)
	move	$5,$2
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lw	$2,20($sp)
	addiu	$2,$2,160
	sw	$2,24($sp)
	lw	$2,24($sp)
	lw	$3,0($2)
	li	$2,536870912			# 0x20000000
	and	$2,$3,$2
	beq	$2,$0,$L1020
	nop

	lw	$2,152($sp)
	lw	$2,8($2)
	addiu	$3,$sp,44
	sw	$3,0($2)
	lw	$2,152($sp)
	lw	$2,8($2)
	addiu	$3,$sp,52
	sw	$3,4($2)
	lw	$2,152($sp)
	lw	$2,8($2)
	addiu	$3,$sp,60
	sw	$3,8($2)
	lw	$2,152($sp)
	lw	$2,8($2)
	addiu	$3,$sp,68
	sw	$3,12($2)
	lw	$4,152($sp)
	lui	$2,%hi(qdma_get_txq_cngst_dynamic_info)
	addiu	$2,$2,%lo(qdma_get_txq_cngst_dynamic_info)
	jalr	$2
	nop

	lui	$2,%hi($LC167)
	addiu	$4,$2,%lo($LC167)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lw	$3,44($sp)
	lui	$2,%hi($LC168)
	addiu	$4,$2,%lo($LC168)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lw	$3,48($sp)
	lui	$2,%hi($LC169)
	addiu	$4,$2,%lo($LC169)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lw	$3,52($sp)
	lui	$2,%hi($LC170)
	addiu	$4,$2,%lo($LC170)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lw	$3,56($sp)
	lui	$2,%hi($LC171)
	addiu	$4,$2,%lo($LC171)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lw	$3,60($sp)
	lui	$2,%hi($LC172)
	addiu	$4,$2,%lo($LC172)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lw	$3,64($sp)
	lui	$2,%hi($LC173)
	addiu	$4,$2,%lo($LC173)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lw	$2,40($sp)
	addiu	$2,$2,1
	move	$3,$2
	li	$2,1			# 0x1
	sll	$3,$2,$3
	lui	$2,%hi($LC174)
	addiu	$4,$2,%lo($LC174)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lbu	$2,35($sp)
	beq	$2,$0,$L1021
	nop

	lui	$2,%hi($LC147)
	addiu	$2,$2,%lo($LC147)
	j	$L1022
	nop

$L1021:
	lui	$2,%hi($LC148)
	addiu	$2,$2,%lo($LC148)
$L1022:
	lui	$3,%hi($LC175)
	addiu	$4,$3,%lo($LC175)
	move	$5,$2
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lbu	$2,36($sp)
	beq	$2,$0,$L1023
	nop

	lui	$2,%hi($LC147)
	addiu	$2,$2,%lo($LC147)
	j	$L1024
	nop

$L1023:
	lui	$2,%hi($LC148)
	addiu	$2,$2,%lo($LC148)
$L1024:
	lui	$3,%hi($LC176)
	addiu	$4,$3,%lo($LC176)
	move	$5,$2
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lhu	$2,38($sp)
	move	$5,$2
	lbu	$2,37($sp)
	beq	$2,$0,$L1025
	nop

	lui	$2,%hi($LC147)
	addiu	$2,$2,%lo($LC147)
	j	$L1026
	nop

$L1025:
	lui	$2,%hi($LC148)
	addiu	$2,$2,%lo($LC148)
$L1026:
	lui	$3,%hi($LC177)
	addiu	$4,$3,%lo($LC177)
	move	$6,$2
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lw	$3,68($sp)
	li	$2,1			# 0x1
	bne	$3,$2,$L1027
	nop

	lw	$5,76($sp)
	lw	$3,72($sp)
	lui	$2,%hi($LC178)
	addiu	$4,$2,%lo($LC178)
	move	$6,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	j	$L1029
	nop

$L1027:
	lui	$2,%hi($LC179)
	addiu	$4,$2,%lo($LC179)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	j	$L1029
	nop

$L1020:
	lw	$2,152($sp)
	lw	$2,8($2)
	addiu	$3,$sp,44
	sw	$3,0($2)
	lw	$2,152($sp)
	lw	$2,8($2)
	addiu	$3,$sp,80
	sw	$3,8($2)
	lw	$2,152($sp)
	lw	$2,8($2)
	addiu	$3,$sp,112
	sw	$3,4($2)
	lw	$4,152($sp)
	lui	$2,%hi(qdma_get_txq_cngst_static_info)
	addiu	$2,$2,%lo(qdma_get_txq_cngst_static_info)
	jalr	$2
	nop

	lui	$2,%hi($LC180)
	addiu	$4,$2,%lo($LC180)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lw	$3,44($sp)
	lui	$2,%hi($LC168)
	addiu	$4,$2,%lo($LC168)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lw	$3,48($sp)
	lui	$2,%hi($LC169)
	addiu	$4,$2,%lo($LC169)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi($LC181)
	addiu	$4,$2,%lo($LC181)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	sw	$0,16($sp)
	j	$L1030
	nop

$L1031:
	lw	$2,16($sp)
	sll	$2,$2,2
	addiu	$3,$sp,16
	addu	$2,$3,$2
	lw	$6,96($2)
	lw	$2,16($sp)
	sll	$2,$2,2
	addiu	$3,$sp,16
	addu	$2,$3,$2
	lw	$3,64($2)
	lui	$2,%hi($LC182)
	addiu	$4,$2,%lo($LC182)
	lw	$5,16($sp)
	move	$7,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lw	$2,16($sp)
	addiu	$2,$2,1
	sw	$2,16($sp)
$L1030:
	lw	$2,16($sp)
	slt	$2,$2,8
	bne	$2,$0,$L1031
	nop

$L1029:
	lui	$2,%hi($LC183)
	addiu	$4,$2,%lo($LC183)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	move	$2,$0
	lw	$31,148($sp)
	addiu	$sp,$sp,152
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	qdma_dump_txq_cngst
	.size	qdma_dump_txq_cngst, .-qdma_dump_txq_cngst
	.rdata
	.align	2
$LC184:
	.ascii	"############ dump qdma all information begin ###########"
	.ascii	"#\015\012\000"
	.align	2
$LC185:
	.ascii	"\015\012\015\012\015\012************ dump register value"
	.ascii	" ************\015\012\015\012\000"
	.align	2
$LC186:
	.ascii	"\015\012\015\012\015\012************ dump tx/rx descript"
	.ascii	"or ************\015\012\015\012\000"
	.align	2
$LC187:
	.ascii	"\015\012\015\012\015\012************ dump hwfwd descript"
	.ascii	"or ************\015\012\015\012\000"
	.align	2
$LC188:
	.ascii	"\015\012\015\012\015\012************ dump irq informatio"
	.ascii	"n ************\015\012\015\012\000"
	.align	2
$LC189:
	.ascii	"\015\012\015\012\015\012************ dump cpu counters *"
	.ascii	"***********\015\012\015\012\000"
	.align	2
$LC190:
	.ascii	"\015\012\015\012\015\012************ dump tx qos *******"
	.ascii	"*****\015\012\015\012\000"
	.align	2
$LC191:
	.ascii	"\015\012\015\012\015\012************ dump virtual channe"
	.ascii	"l qos ************\015\012\015\012\000"
	.align	2
$LC192:
	.ascii	"\015\012\015\012\015\012************ dump tx ratelimit *"
	.ascii	"***********\015\012\015\012\000"
	.align	2
$LC193:
	.ascii	"\015\012\015\012\015\012************ dump rx ratelimit *"
	.ascii	"***********\015\012\015\012\000"
	.align	2
$LC194:
	.ascii	"\015\012\015\012\015\012************ dump tx dba report "
	.ascii	"************\015\012\015\012\000"
	.align	2
$LC195:
	.ascii	"\015\012\015\012############ dump qdma all information e"
	.ascii	"nd ############\015\012\000"
	.section	.text.qdma_dump_info_all,"ax",@progbits
	.align	2
	.globl	qdma_dump_info_all
	.set	nomips16
	.set	nomicromips
	.ent	qdma_dump_info_all
	.type	qdma_dump_info_all, @function
qdma_dump_info_all:
	.frame	$sp,32,$31		# vars= 8, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-32
	sw	$31,28($sp)
	sw	$4,32($sp)
	sw	$0,16($sp)
	lui	$2,%hi($LC184)
	addiu	$4,$2,%lo($LC184)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi($LC185)
	addiu	$4,$2,%lo($LC185)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lw	$4,32($sp)
	lui	$2,%hi(qdma_dump_register_value)
	addiu	$2,$2,%lo(qdma_dump_register_value)
	jalr	$2
	nop

	move	$3,$2
	lw	$2,16($sp)
	or	$2,$2,$3
	sw	$2,16($sp)
	lui	$2,%hi($LC186)
	addiu	$4,$2,%lo($LC186)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lw	$4,32($sp)
	lui	$2,%hi(qdma_dump_descriptor_info)
	addiu	$2,$2,%lo(qdma_dump_descriptor_info)
	jalr	$2
	nop

	move	$3,$2
	lw	$2,16($sp)
	or	$2,$2,$3
	sw	$2,16($sp)
	lui	$2,%hi($LC187)
	addiu	$4,$2,%lo($LC187)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lw	$4,32($sp)
	lui	$2,%hi(qdma_dump_hwfwd_info)
	addiu	$2,$2,%lo(qdma_dump_hwfwd_info)
	jalr	$2
	nop

	move	$3,$2
	lw	$2,16($sp)
	or	$2,$2,$3
	sw	$2,16($sp)
	lui	$2,%hi($LC188)
	addiu	$4,$2,%lo($LC188)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lw	$4,32($sp)
	lui	$2,%hi(qdma_dump_irq_info)
	addiu	$2,$2,%lo(qdma_dump_irq_info)
	jalr	$2
	nop

	move	$3,$2
	lw	$2,16($sp)
	or	$2,$2,$3
	sw	$2,16($sp)
	lui	$2,%hi($LC189)
	addiu	$4,$2,%lo($LC189)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lw	$4,32($sp)
	lui	$2,%hi(qdma_dump_cpu_counters)
	addiu	$2,$2,%lo(qdma_dump_cpu_counters)
	jalr	$2
	nop

	move	$3,$2
	lw	$2,16($sp)
	or	$2,$2,$3
	sw	$2,16($sp)
	lui	$2,%hi($LC190)
	addiu	$4,$2,%lo($LC190)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lw	$2,32($sp)
	li	$3,8			# 0x8
	sw	$3,8($2)
	lw	$4,32($sp)
	lui	$2,%hi(qdma_dump_tx_qos)
	addiu	$2,$2,%lo(qdma_dump_tx_qos)
	jalr	$2
	nop

	move	$3,$2
	lw	$2,16($sp)
	or	$2,$2,$3
	sw	$2,16($sp)
	lui	$2,%hi($LC191)
	addiu	$4,$2,%lo($LC191)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lw	$2,32($sp)
	li	$3,8			# 0x8
	sw	$3,8($2)
	lw	$4,32($sp)
	lui	$2,%hi(qdma_dump_virtual_channel_qos)
	addiu	$2,$2,%lo(qdma_dump_virtual_channel_qos)
	jalr	$2
	nop

	move	$3,$2
	lw	$2,16($sp)
	or	$2,$2,$3
	sw	$2,16($sp)
	lui	$2,%hi($LC192)
	addiu	$4,$2,%lo($LC192)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lw	$2,32($sp)
	li	$3,8			# 0x8
	sw	$3,8($2)
	lw	$4,32($sp)
	lui	$2,%hi(qdma_dump_tx_ratelimit)
	addiu	$2,$2,%lo(qdma_dump_tx_ratelimit)
	jalr	$2
	nop

	move	$3,$2
	lw	$2,16($sp)
	or	$2,$2,$3
	sw	$2,16($sp)
	lui	$2,%hi($LC193)
	addiu	$4,$2,%lo($LC193)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lw	$2,32($sp)
	li	$3,2			# 0x2
	sw	$3,8($2)
	lw	$4,32($sp)
	lui	$2,%hi(qdma_dump_rx_ratelimit)
	addiu	$2,$2,%lo(qdma_dump_rx_ratelimit)
	jalr	$2
	nop

	move	$3,$2
	lw	$2,16($sp)
	or	$2,$2,$3
	sw	$2,16($sp)
	lui	$2,%hi($LC194)
	addiu	$4,$2,%lo($LC194)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lw	$2,32($sp)
	li	$3,8			# 0x8
	sw	$3,8($2)
	lw	$4,32($sp)
	lui	$2,%hi(qdma_dump_tx_dba_report)
	addiu	$2,$2,%lo(qdma_dump_tx_dba_report)
	jalr	$2
	nop

	move	$3,$2
	lw	$2,16($sp)
	or	$2,$2,$3
	sw	$2,16($sp)
	lui	$2,%hi($LC195)
	addiu	$4,$2,%lo($LC195)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lw	$2,16($sp)
	lw	$31,28($sp)
	addiu	$sp,$sp,32
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	qdma_dump_info_all
	.size	qdma_dump_info_all, .-qdma_dump_info_all
	.ident	"GCC: (Buildroot 2015.08.1) 4.9.3"
