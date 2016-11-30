	.file	1 "qdma_dev.c"
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
	.globl	dma_busy_timer
	.section	.bss,"aw",@nobits
	.align	2
	.type	dma_busy_timer, @object
	.size	dma_busy_timer, 28
dma_busy_timer:
	.space	28
	.globl	dma_busy_expires
	.data
	.align	2
	.type	dma_busy_expires, @object
	.size	dma_busy_expires, 4
dma_busy_expires:
	.word	10
	.globl	dma_busy_round_cnt
	.section	.bss
	.align	2
	.type	dma_busy_round_cnt, @object
	.size	dma_busy_round_cnt, 4
dma_busy_round_cnt:
	.space	4
	.globl	tx_dma_busy_enable_cnt
	.align	2
	.type	tx_dma_busy_enable_cnt, @object
	.size	tx_dma_busy_enable_cnt, 4
tx_dma_busy_enable_cnt:
	.space	4
	.globl	rx_dma_busy_enable_cnt
	.align	2
	.type	rx_dma_busy_enable_cnt, @object
	.size	rx_dma_busy_enable_cnt, 4
rx_dma_busy_enable_cnt:
	.space	4
	.globl	reg_polling_timer
	.align	2
	.type	reg_polling_timer, @object
	.size	reg_polling_timer, 28
reg_polling_timer:
	.space	28
	.globl	reg_polling_expires
	.data
	.align	2
	.type	reg_polling_expires, @object
	.size	reg_polling_expires, 4
reg_polling_expires:
	.word	10
	.globl	reg_polling_round_cnt
	.section	.bss
	.align	2
	.type	reg_polling_round_cnt, @object
	.size	reg_polling_round_cnt, 4
reg_polling_round_cnt:
	.space	4
	.globl	reg_polling_offset
	.align	2
	.type	reg_polling_offset, @object
	.size	reg_polling_offset, 4
reg_polling_offset:
	.space	4
	.globl	qdmaRegValBuff
	.align	2
	.type	qdmaRegValBuff, @object
	.size	qdmaRegValBuff, 400
qdmaRegValBuff:
	.space	400
	.globl	channel_limit_threshold
	.align	2
	.type	channel_limit_threshold, @object
	.size	channel_limit_threshold, 4
channel_limit_threshold:
	.space	4
	.globl	queue_limit_threshold
	.align	2
	.type	queue_limit_threshold, @object
	.size	queue_limit_threshold, 4
queue_limit_threshold:
	.space	4
	.globl	gpQdmaDev
	.align	2
	.type	gpQdmaDev, @object
	.size	gpQdmaDev, 4
gpQdmaDev:
	.space	4
	.globl	qdma_vip_num
	.type	qdma_vip_num, @object
	.size	qdma_vip_num, 1
qdma_vip_num:
	.space	1
	.globl	qdma_vip_info
	.align	2
	.type	qdma_vip_info, @object
	.size	qdma_vip_info, 192
qdma_vip_info:
	.space	192
	.globl	priPktChkLen
	.data
	.align	2
	.type	priPktChkLen, @object
	.size	priPktChkLen, 4
priPktChkLen:
	.word	100
	.local	__param_perm_check_priPktChkLen
	.comm	__param_perm_check_priPktChkLen,4,4
	.rdata
	.align	2
	.type	__param_str_priPktChkLen, @object
	.size	__param_str_priPktChkLen, 13
__param_str_priPktChkLen:
	.ascii	"priPktChkLen\000"
	.section	__param,"a",@progbits
	.align	2
	.type	__param_priPktChkLen, @object
	.size	__param_priPktChkLen, 16
__param_priPktChkLen:
	.word	__param_str_priPktChkLen
	.word	param_ops_int
	.half	0
	.half	2
	.word	priPktChkLen
	.section	.modinfo,"a",@progbits
	.align	2
	.type	__mod_priPktChkLentype136, @object
	.size	__mod_priPktChkLentype136, 26
__mod_priPktChkLentype136:
	.ascii	"parmtype=priPktChkLen:int\000"
	.globl	priPktChk
	.data
	.align	2
	.type	priPktChk, @object
	.size	priPktChk, 4
priPktChk:
	.word	1
	.local	__param_perm_check_priPktChk
	.comm	__param_perm_check_priPktChk,4,4
	.rdata
	.align	2
	.type	__param_str_priPktChk, @object
	.size	__param_str_priPktChk, 10
__param_str_priPktChk:
	.ascii	"priPktChk\000"
	.section	__param
	.align	2
	.type	__param_priPktChk, @object
	.size	__param_priPktChk, 16
__param_priPktChk:
	.word	__param_str_priPktChk
	.word	param_ops_int
	.half	0
	.half	2
	.word	priPktChk
	.section	.modinfo
	.align	2
	.type	__mod_priPktChktype143, @object
	.size	__mod_priPktChktype143, 23
__mod_priPktChktype143:
	.ascii	"parmtype=priPktChk:int\000"
	.section	.text.__pseChannelRelease,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	__pseChannelRelease
	.type	__pseChannelRelease, @function
__pseChannelRelease:
	.frame	$sp,56,$31		# vars= 32, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-56
	sw	$31,52($sp)
	move	$2,$4
	sb	$2,56($sp)
	li	$2,-1078657024			# 0xffffffffbfb50000
	ori	$2,$2,0x1528
	sw	$2,20($sp)
	sw	$0,24($sp)
	li	$2,3			# 0x3
	sw	$2,16($sp)
	lbu	$2,56($sp)
	sll	$2,$2,4
	andi	$2,$2,0xff
	ori	$2,$2,0x1
	sw	$2,24($sp)
	lw	$2,20($sp)
	sw	$2,28($sp)
	lw	$2,24($sp)
	sw	$2,32($sp)
	lw	$2,28($sp)
	lw	$3,32($sp)
	sw	$3,0($2)
	j	$L2
	nop

$L6:
	lw	$2,20($sp)
	sw	$2,36($sp)
	lw	$2,36($sp)
	lw	$2,0($2)
	sw	$2,24($sp)
	lw	$2,24($sp)
	andi	$2,$2,0x2
	beq	$2,$0,$L4
	nop

	lbu	$2,56($sp)
	sll	$2,$2,4
	andi	$2,$2,0xff
	sw	$2,24($sp)
	lw	$2,20($sp)
	sw	$2,40($sp)
	lw	$2,24($sp)
	sw	$2,44($sp)
	lw	$2,40($sp)
	lw	$3,44($sp)
	sw	$3,0($2)
	j	$L5
	nop

$L4:
	li	$4,5000			# 0x1388
	lui	$2,%hi(__udelay)
	addiu	$2,$2,%lo(__udelay)
	jalr	$2
	nop

$L2:
	lw	$2,16($sp)
	addiu	$3,$2,-1
	sw	$3,16($sp)
	bne	$2,$0,$L6
	nop

$L5:
	lw	$2,16($sp)
	bgez	$2,$L7
	nop

	li	$2,-62			# 0xffffffffffffffc2
	j	$L8
	nop

$L7:
	move	$2,$0
$L8:
	lw	$31,52($sp)
	addiu	$sp,$sp,56
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	__pseChannelRelease
	.size	__pseChannelRelease, .-__pseChannelRelease
	.section	.text.pseSetWanPcpConfig,"ax",@progbits
	.align	2
	.globl	pseSetWanPcpConfig
	.set	nomips16
	.set	nomicromips
	.ent	pseSetWanPcpConfig
	.type	pseSetWanPcpConfig, @function
pseSetWanPcpConfig:
	.frame	$sp,56,$31		# vars= 56, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-56
	move	$3,$4
	move	$2,$5
	sb	$3,56($sp)
	sb	$2,60($sp)
	li	$2,-1078657024			# 0xffffffffbfb50000
	ori	$2,$2,0x1514
	sw	$2,0($sp)
	lbu	$2,56($sp)
	bne	$2,$0,$L10
	nop

	lw	$2,0($sp)
	sw	$2,16($sp)
	lw	$2,16($sp)
	lw	$2,0($2)
	sw	$2,4($sp)
	lw	$3,4($sp)
	li	$2,-16			# 0xfffffffffffffff0
	and	$3,$3,$2
	lbu	$2,60($sp)
	andi	$2,$2,0xf
	or	$2,$3,$2
	lw	$3,0($sp)
	sw	$3,20($sp)
	sw	$2,24($sp)
	lw	$2,20($sp)
	lw	$3,24($sp)
	sw	$3,0($2)
	j	$L12
	nop

$L10:
	lbu	$3,56($sp)
	li	$2,1			# 0x1
	bne	$3,$2,$L13
	nop

	lw	$2,0($sp)
	sw	$2,28($sp)
	lw	$2,28($sp)
	lw	$2,0($2)
	sw	$2,8($sp)
	lw	$3,8($sp)
	li	$2,-241			# 0xffffffffffffff0f
	and	$3,$3,$2
	lbu	$2,60($sp)
	sll	$2,$2,4
	andi	$2,$2,0xff
	or	$2,$3,$2
	lw	$3,0($sp)
	sw	$3,32($sp)
	sw	$2,36($sp)
	lw	$2,32($sp)
	lw	$3,36($sp)
	sw	$3,0($2)
	j	$L12
	nop

$L13:
	lbu	$3,56($sp)
	li	$2,2			# 0x2
	bne	$3,$2,$L12
	nop

	lw	$2,0($sp)
	sw	$2,40($sp)
	lw	$2,40($sp)
	lw	$2,0($2)
	sw	$2,12($sp)
	lw	$3,12($sp)
	li	$2,-3841			# 0xfffffffffffff0ff
	and	$3,$3,$2
	lbu	$2,60($sp)
	sll	$2,$2,8
	andi	$2,$2,0xf00
	or	$2,$3,$2
	lw	$3,0($sp)
	sw	$3,44($sp)
	sw	$2,48($sp)
	lw	$2,44($sp)
	lw	$3,48($sp)
	sw	$3,0($2)
$L12:
	move	$2,$0
	addiu	$sp,$sp,56
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	pseSetWanPcpConfig
	.size	pseSetWanPcpConfig, .-pseSetWanPcpConfig
	.section	.text.pseGetWanPcpConfig,"ax",@progbits
	.align	2
	.globl	pseGetWanPcpConfig
	.set	nomips16
	.set	nomicromips
	.ent	pseGetWanPcpConfig
	.type	pseGetWanPcpConfig, @function
pseGetWanPcpConfig:
	.frame	$sp,24,$31		# vars= 24, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-24
	move	$2,$4
	sb	$2,24($sp)
	li	$2,-1078657024			# 0xffffffffbfb50000
	ori	$2,$2,0x1514
	sw	$2,4($sp)
	sw	$0,0($sp)
	lbu	$2,24($sp)
	bne	$2,$0,$L18
	nop

	lw	$2,4($sp)
	sw	$2,8($sp)
	lw	$2,8($sp)
	lw	$2,0($2)
	andi	$2,$2,0xf
	sw	$2,0($sp)
	j	$L20
	nop

$L18:
	lbu	$3,24($sp)
	li	$2,1			# 0x1
	bne	$3,$2,$L21
	nop

	lw	$2,4($sp)
	sw	$2,12($sp)
	lw	$2,12($sp)
	lw	$2,0($2)
	andi	$2,$2,0xf0
	srl	$2,$2,4
	sw	$2,0($sp)
	j	$L20
	nop

$L21:
	lbu	$3,24($sp)
	li	$2,2			# 0x2
	bne	$3,$2,$L20
	nop

	lw	$2,4($sp)
	sw	$2,16($sp)
	lw	$2,16($sp)
	lw	$2,0($2)
	andi	$2,$2,0xf00
	srl	$2,$2,8
	sw	$2,0($sp)
$L20:
	lw	$2,0($sp)
	addiu	$sp,$sp,24
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	pseGetWanPcpConfig
	.size	pseGetWanPcpConfig, .-pseGetWanPcpConfig
	.section	.text.qdmaSetChannelRetire,"ax",@progbits
	.align	2
	.globl	qdmaSetChannelRetire
	.set	nomips16
	.set	nomicromips
	.ent	qdmaSetChannelRetire
	.type	qdmaSetChannelRetire, @function
qdmaSetChannelRetire:
	.frame	$sp,136,$31		# vars= 112, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-136
	sw	$31,132($sp)
	move	$2,$4
	sb	$2,136($sp)
	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lw	$2,0($2)
	sw	$2,32($sp)
	sw	$0,36($sp)
	sw	$0,16($sp)
	li	$2,3			# 0x3
	sw	$2,20($sp)
	sw	$0,24($sp)
	lw	$2,32($sp)
	addiu	$2,$2,4
	sw	$2,56($sp)
	lw	$2,56($sp)
	lw	$2,0($2)
	sw	$2,40($sp)
	lw	$2,32($sp)
	addiu	$3,$2,4
	lw	$4,40($sp)
	li	$2,-2			# 0xfffffffffffffffe
	and	$2,$4,$2
	sw	$3,60($sp)
	sw	$2,64($sp)
	lw	$2,60($sp)
	lw	$3,64($sp)
	sw	$3,0($2)
	lw	$2,32($sp)
	addiu	$2,$2,4
	sw	$2,68($sp)
	lw	$2,68($sp)
	lw	$2,0($2)
	andi	$2,$2,0x2
	bne	$2,$0,$L28
	nop

	lw	$2,32($sp)
	addiu	$2,$2,4
	sw	$2,72($sp)
	lw	$2,72($sp)
	lw	$2,0($2)
	sw	$2,44($sp)
	lw	$2,32($sp)
	addiu	$3,$2,4
	lw	$4,44($sp)
	li	$2,-2			# 0xfffffffffffffffe
	and	$2,$4,$2
	sw	$3,76($sp)
	sw	$2,80($sp)
	lw	$2,76($sp)
	lw	$3,80($sp)
	sw	$3,0($2)
	sw	$0,24($sp)
	j	$L30
	nop

$L33:
	lbu	$2,136($sp)
	move	$4,$2
	lui	$2,%hi(__pseChannelRelease)
	addiu	$2,$2,%lo(__pseChannelRelease)
	jalr	$2
	nop

	sw	$2,16($sp)
	li	$2,10			# 0xa
	sw	$2,28($sp)
	j	$L31
	nop

$L32:
	li	$4,1000			# 0x3e8
	lui	$2,%hi(__udelay)
	addiu	$2,$2,%lo(__udelay)
	jalr	$2
	nop

$L31:
	lw	$2,28($sp)
	addiu	$3,$2,-1
	sw	$3,28($sp)
	bne	$2,$0,$L32
	nop

	lw	$2,24($sp)
	addiu	$2,$2,1
	sw	$2,24($sp)
$L30:
	lw	$2,24($sp)
	slt	$2,$2,2
	bne	$2,$0,$L33
	nop

	lbu	$2,136($sp)
	andi	$3,$2,0x1f
	li	$2,-2147483648			# 0xffffffff80000000
	or	$2,$3,$2
	sw	$2,36($sp)
	lw	$2,32($sp)
	addiu	$2,$2,64
	sw	$2,84($sp)
	lw	$2,36($sp)
	sw	$2,88($sp)
	lw	$2,84($sp)
	lw	$3,88($sp)
	sw	$3,0($2)
	j	$L34
	nop

$L38:
	lw	$2,32($sp)
	addiu	$2,$2,64
	sw	$2,92($sp)
	lw	$2,92($sp)
	lw	$2,0($2)
	sw	$2,36($sp)
	lw	$3,36($sp)
	li	$2,1073741824			# 0x40000000
	and	$2,$3,$2
	beq	$2,$0,$L36
	nop

	lbu	$2,136($sp)
	andi	$2,$2,0x1f
	sw	$2,36($sp)
	lw	$2,32($sp)
	addiu	$2,$2,64
	sw	$2,96($sp)
	lw	$2,36($sp)
	sw	$2,100($sp)
	lw	$2,96($sp)
	lw	$3,100($sp)
	sw	$3,0($2)
	j	$L37
	nop

$L36:
	li	$4,5000			# 0x1388
	lui	$2,%hi(__udelay)
	addiu	$2,$2,%lo(__udelay)
	jalr	$2
	nop

$L34:
	lw	$2,20($sp)
	addiu	$3,$2,-1
	sw	$3,20($sp)
	bne	$2,$0,$L38
	nop

$L37:
	lw	$2,20($sp)
	bgez	$2,$L39
	nop

	li	$2,-62			# 0xffffffffffffffc2
	sw	$2,16($sp)
$L39:
	lw	$2,32($sp)
	addiu	$2,$2,4
	sw	$2,104($sp)
	lw	$2,104($sp)
	lw	$2,0($2)
	sw	$2,48($sp)
	lw	$2,32($sp)
	addiu	$3,$2,4
	lw	$2,48($sp)
	ori	$2,$2,0x1
	sw	$3,108($sp)
	sw	$2,112($sp)
	lw	$2,108($sp)
	lw	$3,112($sp)
	sw	$3,0($2)
	j	$L41
	nop

$L28:
	li	$2,-68			# 0xffffffffffffffbc
	sw	$2,16($sp)
$L41:
	lw	$2,32($sp)
	addiu	$2,$2,4
	sw	$2,116($sp)
	lw	$2,116($sp)
	lw	$2,0($2)
	sw	$2,52($sp)
	lw	$2,32($sp)
	addiu	$3,$2,4
	lw	$2,52($sp)
	ori	$2,$2,0x1
	sw	$3,120($sp)
	sw	$2,124($sp)
	lw	$2,120($sp)
	lw	$3,124($sp)
	sw	$3,0($2)
	lw	$2,16($sp)
	lw	$31,132($sp)
	addiu	$sp,$sp,136
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	qdmaSetChannelRetire
	.size	qdmaSetChannelRetire, .-qdmaSetChannelRetire
	.section	.text.qdmaSetRetire,"ax",@progbits
	.align	2
	.globl	qdmaSetRetire
	.set	nomips16
	.set	nomicromips
	.ent	qdmaSetRetire
	.type	qdmaSetRetire, @function
qdmaSetRetire:
	.frame	$sp,128,$31		# vars= 104, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-128
	sw	$31,124($sp)
	move	$2,$4
	sb	$2,128($sp)
	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lw	$2,0($2)
	sw	$2,32($sp)
	sw	$0,36($sp)
	sw	$0,16($sp)
	li	$2,3			# 0x3
	sw	$2,20($sp)
	sw	$0,24($sp)
	sb	$0,28($sp)
	sw	$0,40($sp)
	lw	$2,32($sp)
	addiu	$2,$2,4
	sw	$2,52($sp)
	lw	$2,52($sp)
	lw	$2,0($2)
	sw	$2,44($sp)
	lw	$2,32($sp)
	addiu	$3,$2,4
	lw	$4,44($sp)
	li	$2,-2			# 0xfffffffffffffffe
	and	$2,$4,$2
	sw	$3,56($sp)
	sw	$2,60($sp)
	lw	$2,56($sp)
	lw	$3,60($sp)
	sw	$3,0($2)
	li	$2,-1078657024			# 0xffffffffbfb50000
	ori	$2,$2,0x1844
	sw	$2,64($sp)
	lw	$2,64($sp)
	lw	$2,0($2)
	sw	$2,40($sp)
	lw	$2,40($sp)
	ori	$2,$2,0xffff
	li	$3,-1078657024			# 0xffffffffbfb50000
	ori	$3,$3,0x1844
	sw	$3,68($sp)
	sw	$2,72($sp)
	lw	$2,68($sp)
	lw	$3,72($sp)
	sw	$3,0($2)
	sw	$0,24($sp)
	j	$L47
	nop

$L50:
	sb	$0,28($sp)
	j	$L48
	nop

$L49:
	lbu	$2,28($sp)
	move	$4,$2
	lui	$2,%hi(__pseChannelRelease)
	addiu	$2,$2,%lo(__pseChannelRelease)
	jalr	$2
	nop

	sw	$2,16($sp)
	li	$4,5000			# 0x1388
	lui	$2,%hi(__udelay)
	addiu	$2,$2,%lo(__udelay)
	jalr	$2
	nop

	lbu	$2,28($sp)
	addiu	$2,$2,1
	sb	$2,28($sp)
$L48:
	lbu	$3,28($sp)
	lbu	$2,128($sp)
	sltu	$2,$3,$2
	bne	$2,$0,$L49
	nop

	lw	$2,24($sp)
	addiu	$2,$2,1
	sw	$2,24($sp)
$L47:
	lw	$2,24($sp)
	slt	$2,$2,2
	bne	$2,$0,$L50
	nop

	sb	$0,28($sp)
	j	$L51
	nop

$L58:
	lbu	$2,28($sp)
	andi	$3,$2,0x1f
	li	$2,-2147483648			# 0xffffffff80000000
	or	$2,$3,$2
	sw	$2,36($sp)
	lw	$2,32($sp)
	addiu	$2,$2,64
	sw	$2,76($sp)
	lw	$2,36($sp)
	sw	$2,80($sp)
	lw	$2,76($sp)
	lw	$3,80($sp)
	sw	$3,0($2)
	li	$2,3			# 0x3
	sw	$2,20($sp)
	j	$L52
	nop

$L56:
	lw	$2,32($sp)
	addiu	$2,$2,64
	sw	$2,84($sp)
	lw	$2,84($sp)
	lw	$2,0($2)
	sw	$2,36($sp)
	lw	$3,36($sp)
	li	$2,1073741824			# 0x40000000
	and	$2,$3,$2
	beq	$2,$0,$L54
	nop

	lbu	$2,28($sp)
	andi	$2,$2,0x1f
	sw	$2,36($sp)
	lw	$2,32($sp)
	addiu	$2,$2,64
	sw	$2,88($sp)
	lw	$2,36($sp)
	sw	$2,92($sp)
	lw	$2,88($sp)
	lw	$3,92($sp)
	sw	$3,0($2)
	j	$L55
	nop

$L54:
	li	$4,5000			# 0x1388
	lui	$2,%hi(__udelay)
	addiu	$2,$2,%lo(__udelay)
	jalr	$2
	nop

$L52:
	lw	$2,20($sp)
	addiu	$3,$2,-1
	sw	$3,20($sp)
	bne	$2,$0,$L56
	nop

$L55:
	lw	$2,20($sp)
	bgez	$2,$L57
	nop

	li	$2,-62			# 0xffffffffffffffc2
	sw	$2,16($sp)
$L57:
	lbu	$2,28($sp)
	addiu	$2,$2,1
	sb	$2,28($sp)
$L51:
	lbu	$3,28($sp)
	lbu	$2,128($sp)
	sltu	$2,$3,$2
	bne	$2,$0,$L58
	nop

	li	$2,-1078657024			# 0xffffffffbfb50000
	ori	$2,$2,0x1844
	sw	$2,96($sp)
	lw	$2,40($sp)
	sw	$2,100($sp)
	lw	$2,96($sp)
	lw	$3,100($sp)
	sw	$3,0($2)
	lw	$2,32($sp)
	addiu	$2,$2,4
	sw	$2,104($sp)
	lw	$2,104($sp)
	lw	$2,0($2)
	sw	$2,48($sp)
	lw	$2,32($sp)
	addiu	$3,$2,4
	lw	$2,48($sp)
	ori	$2,$2,0x1
	sw	$3,108($sp)
	sw	$2,112($sp)
	lw	$2,108($sp)
	lw	$3,112($sp)
	sw	$3,0($2)
	lw	$2,16($sp)
	lw	$31,124($sp)
	addiu	$sp,$sp,128
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	qdmaSetRetire
	.size	qdmaSetRetire, .-qdmaSetRetire
	.section	.text.qdmaSetTxQosScheduler,"ax",@progbits
	.align	2
	.globl	qdmaSetTxQosScheduler
	.set	nomips16
	.set	nomicromips
	.ent	qdmaSetTxQosScheduler
	.type	qdmaSetTxQosScheduler, @function
qdmaSetTxQosScheduler:
	.frame	$sp,80,$31		# vars= 56, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-80
	sw	$31,76($sp)
	move	$3,$4
	move	$2,$5
	sw	$6,88($sp)
	sb	$3,80($sp)
	sb	$2,84($sp)
	sw	$0,16($sp)
	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lw	$2,0($2)
	sw	$2,20($sp)
	sw	$0,24($sp)
	lbu	$2,80($sp)
	srl	$2,$2,3
	andi	$2,$2,0x00ff
	sll	$2,$2,2
	move	$3,$2
	lw	$2,20($sp)
	addu	$2,$3,$2
	addiu	$2,$2,384
	sw	$2,32($sp)
	lw	$2,32($sp)
	lw	$2,0($2)
	sw	$2,28($sp)
	lbu	$2,80($sp)
	srl	$2,$2,3
	andi	$2,$2,0x00ff
	sll	$2,$2,2
	move	$3,$2
	lw	$2,20($sp)
	addu	$2,$3,$2
	addiu	$3,$2,384
	lbu	$2,80($sp)
	andi	$2,$2,0x7
	sll	$2,$2,2
	li	$4,7			# 0x7
	sll	$2,$4,$2
	nor	$2,$0,$2
	move	$4,$2
	lw	$2,28($sp)
	and	$2,$4,$2
	lbu	$5,84($sp)
	lbu	$4,80($sp)
	andi	$4,$4,0x7
	sll	$4,$4,2
	sll	$5,$5,$4
	lbu	$4,80($sp)
	andi	$4,$4,0x7
	sll	$4,$4,2
	li	$6,7			# 0x7
	sll	$4,$6,$4
	and	$4,$5,$4
	or	$2,$2,$4
	sw	$3,36($sp)
	sw	$2,40($sp)
	lw	$2,36($sp)
	lw	$3,40($sp)
	sw	$3,0($2)
	sw	$0,16($sp)
	j	$L63
	nop

$L72:
	lw	$2,16($sp)
	lw	$3,88($sp)
	addu	$2,$3,$2
	lbu	$3,0($2)
	li	$2,255			# 0xff
	beq	$3,$2,$L64
	nop

	lw	$2,16($sp)
	lw	$3,88($sp)
	addu	$2,$3,$2
	lbu	$2,0($2)
	move	$3,$2
	li	$2,-2147483648			# 0xffffffff80000000
	or	$3,$3,$2
	lbu	$2,80($sp)
	sll	$4,$2,19
	li	$2,16252928			# 0xf80000
	and	$2,$4,$2
	or	$3,$3,$2
	lw	$2,16($sp)
	sll	$4,$2,16
	li	$2,458752			# 0x70000
	and	$2,$4,$2
	or	$2,$3,$2
	sw	$2,24($sp)
	lw	$2,20($sp)
	addiu	$2,$2,136
	sw	$2,44($sp)
	lw	$2,24($sp)
	sw	$2,48($sp)
	lw	$2,44($sp)
	lw	$3,48($sp)
	sw	$3,0($2)
	lw	$2,20($sp)
	addiu	$2,$2,136
	sw	$2,52($sp)
	li	$2,1073741824			# 0x40000000
	sw	$2,56($sp)
	li	$2,3			# 0x3
	sw	$2,60($sp)
	sw	$0,68($sp)
	j	$L65
	nop

$L68:
	lw	$2,52($sp)
	sw	$2,64($sp)
	lw	$2,64($sp)
	lw	$2,0($2)
	sw	$2,68($sp)
	lw	$3,68($sp)
	lw	$2,56($sp)
	and	$2,$3,$2
	bne	$2,$0,$L67
	nop

	li	$4,1000			# 0x3e8
	lui	$2,%hi(__udelay)
	addiu	$2,$2,%lo(__udelay)
	jalr	$2
	nop

$L65:
	lw	$2,60($sp)
	addiu	$3,$2,-1
	sw	$3,60($sp)
	bne	$2,$0,$L68
	nop

$L67:
	lw	$2,60($sp)
	bgez	$2,$L69
	nop

	li	$2,-62			# 0xffffffffffffffc2
	j	$L70
	nop

$L69:
	move	$2,$0
$L70:
	bgez	$2,$L64
	nop

	li	$2,-62			# 0xffffffffffffffc2
	j	$L71
	nop

$L64:
	lw	$2,16($sp)
	addiu	$2,$2,1
	sw	$2,16($sp)
$L63:
	lw	$2,16($sp)
	slt	$2,$2,8
	bne	$2,$0,$L72
	nop

	move	$2,$0
$L71:
	lw	$31,76($sp)
	addiu	$sp,$sp,80
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	qdmaSetTxQosScheduler
	.size	qdmaSetTxQosScheduler, .-qdmaSetTxQosScheduler
	.section	.text.qdmaGetTxQosScheduler,"ax",@progbits
	.align	2
	.globl	qdmaGetTxQosScheduler
	.set	nomips16
	.set	nomicromips
	.ent	qdmaGetTxQosScheduler
	.type	qdmaGetTxQosScheduler, @function
qdmaGetTxQosScheduler:
	.frame	$sp,72,$31		# vars= 48, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-72
	sw	$31,68($sp)
	move	$2,$4
	sw	$5,76($sp)
	sw	$6,80($sp)
	sb	$2,72($sp)
	sw	$0,16($sp)
	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lw	$2,0($2)
	sw	$2,20($sp)
	sw	$0,24($sp)
	lbu	$2,72($sp)
	srl	$2,$2,3
	andi	$2,$2,0x00ff
	sll	$2,$2,2
	move	$3,$2
	lw	$2,20($sp)
	addu	$2,$3,$2
	addiu	$2,$2,384
	sw	$2,28($sp)
	lw	$2,28($sp)
	lw	$2,0($2)
	lbu	$3,72($sp)
	andi	$3,$3,0x7
	sll	$3,$3,2
	li	$4,7			# 0x7
	sll	$3,$4,$3
	and	$3,$2,$3
	lbu	$2,72($sp)
	andi	$2,$2,0x7
	sll	$2,$2,2
	srl	$2,$3,$2
	andi	$3,$2,0x00ff
	lw	$2,76($sp)
	sb	$3,0($2)
	sw	$0,16($sp)
	j	$L75
	nop

$L85:
	lbu	$2,72($sp)
	sll	$3,$2,19
	li	$2,16252928			# 0xf80000
	and	$3,$3,$2
	lw	$2,16($sp)
	sll	$4,$2,16
	li	$2,458752			# 0x70000
	and	$2,$4,$2
	or	$2,$3,$2
	sw	$2,24($sp)
	lw	$2,20($sp)
	addiu	$2,$2,136
	sw	$2,32($sp)
	lw	$2,24($sp)
	sw	$2,36($sp)
	lw	$2,32($sp)
	lw	$3,36($sp)
	sw	$3,0($2)
	lw	$2,20($sp)
	addiu	$2,$2,136
	sw	$2,40($sp)
	li	$2,1073741824			# 0x40000000
	sw	$2,44($sp)
	li	$2,3			# 0x3
	sw	$2,48($sp)
	sw	$0,60($sp)
	j	$L76
	nop

$L79:
	lw	$2,40($sp)
	sw	$2,52($sp)
	lw	$2,52($sp)
	lw	$2,0($2)
	sw	$2,60($sp)
	lw	$3,60($sp)
	lw	$2,44($sp)
	and	$2,$3,$2
	bne	$2,$0,$L78
	nop

	li	$4,1000			# 0x3e8
	lui	$2,%hi(__udelay)
	addiu	$2,$2,%lo(__udelay)
	jalr	$2
	nop

$L76:
	lw	$2,48($sp)
	addiu	$3,$2,-1
	sw	$3,48($sp)
	bne	$2,$0,$L79
	nop

$L78:
	lw	$2,48($sp)
	bgez	$2,$L80
	nop

	li	$2,-62			# 0xffffffffffffffc2
	j	$L81
	nop

$L80:
	move	$2,$0
$L81:
	bgez	$2,$L82
	nop

	li	$2,-62			# 0xffffffffffffffc2
	j	$L83
	nop

$L82:
	lw	$2,20($sp)
	addiu	$2,$2,136
	sw	$2,56($sp)
	lw	$2,56($sp)
	lw	$2,0($2)
	sw	$2,24($sp)
	lw	$2,16($sp)
	lw	$3,80($sp)
	addu	$2,$3,$2
	lw	$3,24($sp)
	andi	$3,$3,0x00ff
	sb	$3,0($2)
	lw	$2,16($sp)
	addiu	$2,$2,1
	sw	$2,16($sp)
$L75:
	lw	$2,16($sp)
	slt	$2,$2,8
	bne	$2,$0,$L85
	nop

	move	$2,$0
$L83:
	lw	$31,68($sp)
	addiu	$sp,$sp,72
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	qdmaGetTxQosScheduler
	.size	qdmaGetTxQosScheduler, .-qdmaGetTxQosScheduler
	.section	.text.qdmaSetRxRateLimitConfig,"ax",@progbits
	.align	2
	.globl	qdmaSetRxRateLimitConfig
	.set	nomips16
	.set	nomicromips
	.ent	qdmaSetRxRateLimitConfig
	.type	qdmaSetRxRateLimitConfig, @function
qdmaSetRxRateLimitConfig:
	.frame	$sp,64,$31		# vars= 40, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-64
	sw	$31,60($sp)
	move	$3,$5
	move	$2,$6
	sb	$4,64($sp)
	sb	$3,68($sp)
	sh	$2,72($sp)
	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lw	$2,0($2)
	sw	$2,16($sp)
	sw	$0,20($sp)
	lhu	$3,72($sp)
	li	$2,-2147483648			# 0xffffffff80000000
	or	$3,$3,$2
	lbu	$2,64($sp)
	beq	$2,$0,$L87
	nop

	li	$2,65536			# 0x10000
	j	$L88
	nop

$L87:
	move	$2,$0
$L88:
	or	$3,$3,$2
	lbu	$2,68($sp)
	beq	$2,$0,$L89
	nop

	li	$2,536870912			# 0x20000000
	j	$L90
	nop

$L89:
	move	$2,$0
$L90:
	or	$2,$3,$2
	sw	$2,20($sp)
	lw	$2,16($sp)
	addiu	$2,$2,292
	sw	$2,24($sp)
	lw	$2,20($sp)
	sw	$2,28($sp)
	lw	$2,24($sp)
	lw	$3,28($sp)
	sw	$3,0($2)
	lw	$2,16($sp)
	addiu	$2,$2,292
	sw	$2,32($sp)
	li	$2,1073741824			# 0x40000000
	sw	$2,36($sp)
	li	$2,3			# 0x3
	sw	$2,40($sp)
	sw	$0,48($sp)
	j	$L91
	nop

$L94:
	lw	$2,32($sp)
	sw	$2,44($sp)
	lw	$2,44($sp)
	lw	$2,0($2)
	sw	$2,48($sp)
	lw	$3,48($sp)
	lw	$2,36($sp)
	and	$2,$3,$2
	bne	$2,$0,$L93
	nop

	li	$4,1000			# 0x3e8
	lui	$2,%hi(__udelay)
	addiu	$2,$2,%lo(__udelay)
	jalr	$2
	nop

$L91:
	lw	$2,40($sp)
	addiu	$3,$2,-1
	sw	$3,40($sp)
	bne	$2,$0,$L94
	nop

$L93:
	lw	$2,40($sp)
	bgez	$2,$L95
	nop

	li	$2,-62			# 0xffffffffffffffc2
	j	$L96
	nop

$L95:
	move	$2,$0
$L96:
	bgez	$2,$L97
	nop

	li	$2,-62			# 0xffffffffffffffc2
	j	$L98
	nop

$L97:
	move	$2,$0
$L98:
	lw	$31,60($sp)
	addiu	$sp,$sp,64
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	qdmaSetRxRateLimitConfig
	.size	qdmaSetRxRateLimitConfig, .-qdmaSetRxRateLimitConfig
	.section	.text.qdmaGetRxRateLimitConfig,"ax",@progbits
	.align	2
	.globl	qdmaGetRxRateLimitConfig
	.set	nomips16
	.set	nomicromips
	.ent	qdmaGetRxRateLimitConfig
	.type	qdmaGetRxRateLimitConfig, @function
qdmaGetRxRateLimitConfig:
	.frame	$sp,64,$31		# vars= 40, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-64
	sw	$31,60($sp)
	move	$3,$4
	move	$2,$5
	sb	$3,64($sp)
	sb	$2,68($sp)
	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lw	$2,0($2)
	sw	$2,16($sp)
	sw	$0,20($sp)
	lbu	$2,64($sp)
	beq	$2,$0,$L100
	nop

	li	$3,65536			# 0x10000
	j	$L101
	nop

$L100:
	move	$3,$0
$L101:
	lbu	$2,68($sp)
	beq	$2,$0,$L102
	nop

	li	$2,536870912			# 0x20000000
	j	$L103
	nop

$L102:
	move	$2,$0
$L103:
	or	$2,$3,$2
	sw	$2,20($sp)
	lw	$2,16($sp)
	addiu	$2,$2,292
	sw	$2,24($sp)
	lw	$2,20($sp)
	sw	$2,28($sp)
	lw	$2,24($sp)
	lw	$3,28($sp)
	sw	$3,0($2)
	lw	$2,16($sp)
	addiu	$2,$2,292
	sw	$2,32($sp)
	li	$2,1073741824			# 0x40000000
	sw	$2,36($sp)
	li	$2,3			# 0x3
	sw	$2,40($sp)
	sw	$0,52($sp)
	j	$L104
	nop

$L107:
	lw	$2,32($sp)
	sw	$2,44($sp)
	lw	$2,44($sp)
	lw	$2,0($2)
	sw	$2,52($sp)
	lw	$3,52($sp)
	lw	$2,36($sp)
	and	$2,$3,$2
	bne	$2,$0,$L106
	nop

	li	$4,1000			# 0x3e8
	lui	$2,%hi(__udelay)
	addiu	$2,$2,%lo(__udelay)
	jalr	$2
	nop

$L104:
	lw	$2,40($sp)
	addiu	$3,$2,-1
	sw	$3,40($sp)
	bne	$2,$0,$L107
	nop

$L106:
	lw	$2,40($sp)
	bgez	$2,$L108
	nop

	li	$2,-62			# 0xffffffffffffffc2
	j	$L109
	nop

$L108:
	move	$2,$0
$L109:
	bgez	$2,$L110
	nop

	li	$2,-62			# 0xffffffffffffffc2
	j	$L111
	nop

$L110:
	lw	$2,16($sp)
	addiu	$2,$2,292
	sw	$2,48($sp)
	lw	$2,48($sp)
	lw	$2,0($2)
	sw	$2,20($sp)
	lw	$2,20($sp)
	andi	$2,$2,0xffff
$L111:
	lw	$31,60($sp)
	addiu	$sp,$sp,64
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	qdmaGetRxRateLimitConfig
	.size	qdmaGetRxRateLimitConfig, .-qdmaGetRxRateLimitConfig
	.section	.text.qdmaSetTxRateLimitConfig,"ax",@progbits
	.align	2
	.globl	qdmaSetTxRateLimitConfig
	.set	nomips16
	.set	nomicromips
	.ent	qdmaSetTxRateLimitConfig
	.type	qdmaSetTxRateLimitConfig, @function
qdmaSetTxRateLimitConfig:
	.frame	$sp,64,$31		# vars= 40, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-64
	sw	$31,60($sp)
	move	$3,$5
	move	$2,$6
	sb	$4,64($sp)
	sb	$3,68($sp)
	sh	$2,72($sp)
	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lw	$2,0($2)
	sw	$2,16($sp)
	sw	$0,20($sp)
	lhu	$3,72($sp)
	li	$2,-2147483648			# 0xffffffff80000000
	or	$3,$3,$2
	lbu	$2,64($sp)
	sll	$4,$2,16
	li	$2,2031616			# 0x1f0000
	and	$2,$4,$2
	or	$3,$3,$2
	lbu	$2,68($sp)
	sll	$4,$2,28
	li	$2,805306368			# 0x30000000
	and	$2,$4,$2
	or	$2,$3,$2
	sw	$2,20($sp)
	lw	$2,16($sp)
	addiu	$2,$2,156
	sw	$2,24($sp)
	lw	$2,20($sp)
	sw	$2,28($sp)
	lw	$2,24($sp)
	lw	$3,28($sp)
	sw	$3,0($2)
	lw	$2,16($sp)
	addiu	$2,$2,156
	sw	$2,32($sp)
	li	$2,1073741824			# 0x40000000
	sw	$2,36($sp)
	li	$2,3			# 0x3
	sw	$2,40($sp)
	sw	$0,48($sp)
	j	$L114
	nop

$L117:
	lw	$2,32($sp)
	sw	$2,44($sp)
	lw	$2,44($sp)
	lw	$2,0($2)
	sw	$2,48($sp)
	lw	$3,48($sp)
	lw	$2,36($sp)
	and	$2,$3,$2
	bne	$2,$0,$L116
	nop

	li	$4,1000			# 0x3e8
	lui	$2,%hi(__udelay)
	addiu	$2,$2,%lo(__udelay)
	jalr	$2
	nop

$L114:
	lw	$2,40($sp)
	addiu	$3,$2,-1
	sw	$3,40($sp)
	bne	$2,$0,$L117
	nop

$L116:
	lw	$2,40($sp)
	bgez	$2,$L118
	nop

	li	$2,-62			# 0xffffffffffffffc2
	j	$L119
	nop

$L118:
	move	$2,$0
$L119:
	bgez	$2,$L120
	nop

	li	$2,-62			# 0xffffffffffffffc2
	j	$L121
	nop

$L120:
	move	$2,$0
$L121:
	lw	$31,60($sp)
	addiu	$sp,$sp,64
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	qdmaSetTxRateLimitConfig
	.size	qdmaSetTxRateLimitConfig, .-qdmaSetTxRateLimitConfig
	.section	.text.qdmaGetTxRateLimitConfig,"ax",@progbits
	.align	2
	.globl	qdmaGetTxRateLimitConfig
	.set	nomips16
	.set	nomicromips
	.ent	qdmaGetTxRateLimitConfig
	.type	qdmaGetTxRateLimitConfig, @function
qdmaGetTxRateLimitConfig:
	.frame	$sp,64,$31		# vars= 40, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-64
	sw	$31,60($sp)
	move	$3,$4
	move	$2,$5
	sb	$3,64($sp)
	sb	$2,68($sp)
	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lw	$2,0($2)
	sw	$2,16($sp)
	sw	$0,20($sp)
	lbu	$2,64($sp)
	sll	$3,$2,16
	li	$2,2031616			# 0x1f0000
	and	$3,$3,$2
	lbu	$2,68($sp)
	sll	$4,$2,28
	li	$2,805306368			# 0x30000000
	and	$2,$4,$2
	or	$2,$3,$2
	sw	$2,20($sp)
	lw	$2,16($sp)
	addiu	$2,$2,156
	sw	$2,24($sp)
	lw	$2,20($sp)
	sw	$2,28($sp)
	lw	$2,24($sp)
	lw	$3,28($sp)
	sw	$3,0($2)
	lw	$2,16($sp)
	addiu	$2,$2,156
	sw	$2,32($sp)
	li	$2,1073741824			# 0x40000000
	sw	$2,36($sp)
	li	$2,3			# 0x3
	sw	$2,40($sp)
	sw	$0,52($sp)
	j	$L123
	nop

$L126:
	lw	$2,32($sp)
	sw	$2,44($sp)
	lw	$2,44($sp)
	lw	$2,0($2)
	sw	$2,52($sp)
	lw	$3,52($sp)
	lw	$2,36($sp)
	and	$2,$3,$2
	bne	$2,$0,$L125
	nop

	li	$4,1000			# 0x3e8
	lui	$2,%hi(__udelay)
	addiu	$2,$2,%lo(__udelay)
	jalr	$2
	nop

$L123:
	lw	$2,40($sp)
	addiu	$3,$2,-1
	sw	$3,40($sp)
	bne	$2,$0,$L126
	nop

$L125:
	lw	$2,40($sp)
	bgez	$2,$L127
	nop

	li	$2,-62			# 0xffffffffffffffc2
	j	$L128
	nop

$L127:
	move	$2,$0
$L128:
	bgez	$2,$L129
	nop

	li	$2,-62			# 0xffffffffffffffc2
	j	$L130
	nop

$L129:
	lw	$2,16($sp)
	addiu	$2,$2,156
	sw	$2,48($sp)
	lw	$2,48($sp)
	lw	$2,0($2)
	sw	$2,20($sp)
	lw	$2,20($sp)
	andi	$2,$2,0xffff
$L130:
	lw	$31,60($sp)
	addiu	$sp,$sp,64
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	qdmaGetTxRateLimitConfig
	.size	qdmaGetTxRateLimitConfig, .-qdmaGetTxRateLimitConfig
	.section	.text.qdmaSetVirtualChannelQos,"ax",@progbits
	.align	2
	.globl	qdmaSetVirtualChannelQos
	.set	nomips16
	.set	nomicromips
	.ent	qdmaSetVirtualChannelQos
	.type	qdmaSetVirtualChannelQos, @function
qdmaSetVirtualChannelQos:
	.frame	$sp,80,$31		# vars= 56, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-80
	sw	$31,76($sp)
	move	$3,$5
	move	$2,$6
	sw	$7,92($sp)
	sb	$4,80($sp)
	sb	$3,84($sp)
	sb	$2,88($sp)
	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lw	$2,0($2)
	sw	$2,20($sp)
	sw	$0,24($sp)
	sw	$0,16($sp)
	lw	$2,20($sp)
	addiu	$2,$2,320
	sw	$2,32($sp)
	lw	$2,32($sp)
	lw	$2,0($2)
	sw	$2,28($sp)
	lw	$2,20($sp)
	addiu	$3,$2,320
	lbu	$2,80($sp)
	andi	$2,$2,0xf
	sll	$2,$2,1
	li	$4,3			# 0x3
	sll	$2,$4,$2
	nor	$2,$0,$2
	move	$4,$2
	lw	$2,28($sp)
	and	$2,$4,$2
	lbu	$5,88($sp)
	lbu	$4,80($sp)
	andi	$4,$4,0xf
	sll	$4,$4,1
	sll	$5,$5,$4
	lbu	$4,80($sp)
	andi	$4,$4,0xf
	sll	$4,$4,1
	li	$6,3			# 0x3
	sll	$4,$6,$4
	and	$4,$5,$4
	or	$2,$2,$4
	sw	$3,36($sp)
	sw	$2,40($sp)
	lw	$2,36($sp)
	lw	$3,40($sp)
	sw	$3,0($2)
	sw	$0,16($sp)
	j	$L134
	nop

$L143:
	lw	$2,16($sp)
	lw	$3,92($sp)
	addu	$2,$3,$2
	lbu	$3,0($2)
	li	$2,255			# 0xff
	beq	$3,$2,$L135
	nop

	lw	$2,16($sp)
	lw	$3,92($sp)
	addu	$2,$3,$2
	lbu	$2,0($2)
	move	$3,$2
	li	$2,-2147483648			# 0xffffffff80000000
	or	$3,$3,$2
	lbu	$2,80($sp)
	sll	$4,$2,18
	li	$2,3932160			# 0x3c0000
	and	$2,$4,$2
	or	$3,$3,$2
	lw	$2,16($sp)
	sll	$4,$2,16
	li	$2,196608			# 0x30000
	and	$2,$4,$2
	or	$2,$3,$2
	sw	$2,24($sp)
	lw	$2,20($sp)
	addiu	$2,$2,316
	sw	$2,44($sp)
	lw	$2,24($sp)
	sw	$2,48($sp)
	lw	$2,44($sp)
	lw	$3,48($sp)
	sw	$3,0($2)
	lw	$2,20($sp)
	addiu	$2,$2,316
	sw	$2,52($sp)
	li	$2,1073741824			# 0x40000000
	sw	$2,56($sp)
	li	$2,3			# 0x3
	sw	$2,60($sp)
	sw	$0,68($sp)
	j	$L136
	nop

$L139:
	lw	$2,52($sp)
	sw	$2,64($sp)
	lw	$2,64($sp)
	lw	$2,0($2)
	sw	$2,68($sp)
	lw	$3,68($sp)
	lw	$2,56($sp)
	and	$2,$3,$2
	bne	$2,$0,$L138
	nop

	li	$4,1000			# 0x3e8
	lui	$2,%hi(__udelay)
	addiu	$2,$2,%lo(__udelay)
	jalr	$2
	nop

$L136:
	lw	$2,60($sp)
	addiu	$3,$2,-1
	sw	$3,60($sp)
	bne	$2,$0,$L139
	nop

$L138:
	lw	$2,60($sp)
	bgez	$2,$L140
	nop

	li	$2,-62			# 0xffffffffffffffc2
	j	$L141
	nop

$L140:
	move	$2,$0
$L141:
	bgez	$2,$L135
	nop

	li	$2,-62			# 0xffffffffffffffc2
	j	$L142
	nop

$L135:
	lw	$2,16($sp)
	addiu	$2,$2,1
	sw	$2,16($sp)
$L134:
	lbu	$3,84($sp)
	lw	$2,16($sp)
	slt	$2,$2,$3
	bne	$2,$0,$L143
	nop

	move	$2,$0
$L142:
	lw	$31,76($sp)
	addiu	$sp,$sp,80
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	qdmaSetVirtualChannelQos
	.size	qdmaSetVirtualChannelQos, .-qdmaSetVirtualChannelQos
	.section	.text.qdmaGetVirtualChannelQos,"ax",@progbits
	.align	2
	.globl	qdmaGetVirtualChannelQos
	.set	nomips16
	.set	nomicromips
	.ent	qdmaGetVirtualChannelQos
	.type	qdmaGetVirtualChannelQos, @function
qdmaGetVirtualChannelQos:
	.frame	$sp,72,$31		# vars= 48, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-72
	sw	$31,68($sp)
	move	$3,$4
	move	$2,$5
	sw	$6,80($sp)
	sw	$7,84($sp)
	sb	$3,72($sp)
	sb	$2,76($sp)
	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lw	$2,0($2)
	sw	$2,20($sp)
	sw	$0,24($sp)
	sw	$0,16($sp)
	lw	$2,20($sp)
	addiu	$2,$2,320
	sw	$2,28($sp)
	lw	$2,28($sp)
	lw	$2,0($2)
	lbu	$3,72($sp)
	andi	$3,$3,0xf
	sll	$3,$3,1
	li	$4,3			# 0x3
	sll	$3,$4,$3
	and	$3,$2,$3
	lbu	$2,72($sp)
	andi	$2,$2,0xf
	sll	$2,$2,1
	srl	$2,$3,$2
	andi	$3,$2,0x00ff
	lw	$2,80($sp)
	sb	$3,0($2)
	sw	$0,16($sp)
	j	$L146
	nop

$L156:
	lbu	$2,72($sp)
	sll	$3,$2,18
	li	$2,3932160			# 0x3c0000
	and	$3,$3,$2
	lw	$2,16($sp)
	sll	$4,$2,16
	li	$2,196608			# 0x30000
	and	$2,$4,$2
	or	$2,$3,$2
	sw	$2,24($sp)
	lw	$2,20($sp)
	addiu	$2,$2,316
	sw	$2,32($sp)
	lw	$2,24($sp)
	sw	$2,36($sp)
	lw	$2,32($sp)
	lw	$3,36($sp)
	sw	$3,0($2)
	lw	$2,20($sp)
	addiu	$2,$2,316
	sw	$2,40($sp)
	li	$2,1073741824			# 0x40000000
	sw	$2,44($sp)
	li	$2,3			# 0x3
	sw	$2,48($sp)
	sw	$0,60($sp)
	j	$L147
	nop

$L150:
	lw	$2,40($sp)
	sw	$2,52($sp)
	lw	$2,52($sp)
	lw	$2,0($2)
	sw	$2,60($sp)
	lw	$3,60($sp)
	lw	$2,44($sp)
	and	$2,$3,$2
	bne	$2,$0,$L149
	nop

	li	$4,1000			# 0x3e8
	lui	$2,%hi(__udelay)
	addiu	$2,$2,%lo(__udelay)
	jalr	$2
	nop

$L147:
	lw	$2,48($sp)
	addiu	$3,$2,-1
	sw	$3,48($sp)
	bne	$2,$0,$L150
	nop

$L149:
	lw	$2,48($sp)
	bgez	$2,$L151
	nop

	li	$2,-62			# 0xffffffffffffffc2
	j	$L152
	nop

$L151:
	move	$2,$0
$L152:
	bgez	$2,$L153
	nop

	li	$2,-62			# 0xffffffffffffffc2
	j	$L154
	nop

$L153:
	lw	$2,20($sp)
	addiu	$2,$2,316
	sw	$2,56($sp)
	lw	$2,56($sp)
	lw	$2,0($2)
	sw	$2,24($sp)
	lw	$2,16($sp)
	lw	$3,84($sp)
	addu	$2,$3,$2
	lw	$3,24($sp)
	andi	$3,$3,0x00ff
	sb	$3,0($2)
	lw	$2,16($sp)
	addiu	$2,$2,1
	sw	$2,16($sp)
$L146:
	lbu	$3,76($sp)
	lw	$2,16($sp)
	slt	$2,$2,$3
	bne	$2,$0,$L156
	nop

	move	$2,$0
$L154:
	lw	$31,68($sp)
	addiu	$sp,$sp,72
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	qdmaGetVirtualChannelQos
	.size	qdmaGetVirtualChannelQos, .-qdmaGetVirtualChannelQos
	.section	.text.qdmaGetLimitRateMax,"ax",@progbits
	.align	2
	.globl	qdmaGetLimitRateMax
	.set	nomips16
	.set	nomicromips
	.ent	qdmaGetLimitRateMax
	.type	qdmaGetLimitRateMax, @function
qdmaGetLimitRateMax:
	.frame	$sp,48,$31		# vars= 24, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-48
	sw	$31,44($sp)
	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lw	$2,0($2)
	sw	$2,24($sp)
	sw	$0,16($sp)
	sw	$0,28($sp)
	sw	$0,20($sp)
	sw	$0,16($sp)
	j	$L158
	nop

$L163:
	lw	$2,24($sp)
	addiu	$2,$2,352
	sw	$2,32($sp)
	lw	$2,32($sp)
	lw	$2,0($2)
	li	$4,1			# 0x1
	lw	$3,16($sp)
	sll	$3,$4,$3
	and	$2,$2,$3
	beq	$2,$0,$L160
	nop

	lw	$2,16($sp)
	andi	$2,$2,0x00ff
	move	$4,$2
	li	$5,2			# 0x2
	lui	$2,%hi(qdmaGetTxRateLimitConfig)
	addiu	$2,$2,%lo(qdmaGetTxRateLimitConfig)
	jalr	$2
	nop

	sw	$2,28($sp)
	lw	$2,28($sp)
	bgez	$2,$L161
	nop

	lw	$2,28($sp)
	j	$L162
	nop

$L161:
	lw	$3,28($sp)
	lw	$2,20($sp)
	slt	$2,$2,$3
	beq	$2,$0,$L160
	nop

	lw	$2,28($sp)
	sw	$2,20($sp)
$L160:
	lw	$2,16($sp)
	addiu	$2,$2,1
	sw	$2,16($sp)
$L158:
	lw	$2,16($sp)
	slt	$2,$2,8
	bne	$2,$0,$L163
	nop

	lw	$2,20($sp)
$L162:
	lw	$31,44($sp)
	addiu	$sp,$sp,48
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	qdmaGetLimitRateMax
	.size	qdmaGetLimitRateMax, .-qdmaGetLimitRateMax
	.section	.text.qdmaUpdateAllTxRateLimitValue,"ax",@progbits
	.align	2
	.globl	qdmaUpdateAllTxRateLimitValue
	.set	nomips16
	.set	nomicromips
	.ent	qdmaUpdateAllTxRateLimitValue
	.type	qdmaUpdateAllTxRateLimitValue, @function
qdmaUpdateAllTxRateLimitValue:
	.frame	$sp,48,$31		# vars= 24, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-48
	sw	$31,44($sp)
	move	$3,$4
	move	$2,$5
	sh	$3,48($sp)
	sh	$2,52($sp)
	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lw	$2,0($2)
	sw	$2,20($sp)
	sw	$0,24($sp)
	sw	$0,16($sp)
	sw	$0,28($sp)
	sw	$0,32($sp)
	lhu	$2,52($sp)
	bne	$2,$0,$L165
	nop

	li	$2,-1			# 0xffffffffffffffff
	j	$L166
	nop

$L165:
	sw	$0,16($sp)
	j	$L167
	nop

$L170:
	lw	$2,16($sp)
	andi	$2,$2,0x00ff
	move	$4,$2
	li	$5,2			# 0x2
	lui	$2,%hi(qdmaGetTxRateLimitConfig)
	addiu	$2,$2,%lo(qdmaGetTxRateLimitConfig)
	jalr	$2
	nop

	sw	$2,28($sp)
	lw	$2,28($sp)
	bgez	$2,$L168
	nop

	lw	$2,28($sp)
	j	$L166
	nop

$L168:
	lhu	$3,48($sp)
	lw	$2,28($sp)
	mul	$3,$3,$2
	lhu	$2,52($sp)
	div	$0,$3,$2
	mfhi	$3
	mflo	$2
	sw	$2,32($sp)
	lw	$2,16($sp)
	andi	$2,$2,0x00ff
	move	$3,$2
	lw	$2,32($sp)
	andi	$2,$2,0xffff
	move	$4,$3
	li	$5,2			# 0x2
	move	$6,$2
	lui	$2,%hi(qdmaSetTxRateLimitConfig)
	addiu	$2,$2,%lo(qdmaSetTxRateLimitConfig)
	jalr	$2
	nop

	sw	$2,24($sp)
	lw	$2,24($sp)
	bgez	$2,$L169
	nop

	lw	$2,24($sp)
	j	$L166
	nop

$L169:
	lw	$2,16($sp)
	addiu	$2,$2,1
	sw	$2,16($sp)
$L167:
	lw	$2,16($sp)
	slt	$2,$2,8
	bne	$2,$0,$L170
	nop

	move	$2,$0
$L166:
	lw	$31,44($sp)
	addiu	$sp,$sp,48
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	qdmaUpdateAllTxRateLimitValue
	.size	qdmaUpdateAllTxRateLimitValue, .-qdmaUpdateAllTxRateLimitValue
	.section	.text.qdmaUpdateAllRxRateLimitValue,"ax",@progbits
	.align	2
	.globl	qdmaUpdateAllRxRateLimitValue
	.set	nomips16
	.set	nomicromips
	.ent	qdmaUpdateAllRxRateLimitValue
	.type	qdmaUpdateAllRxRateLimitValue, @function
qdmaUpdateAllRxRateLimitValue:
	.frame	$sp,40,$31		# vars= 16, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-40
	sw	$31,36($sp)
	move	$3,$4
	move	$2,$5
	sh	$3,40($sp)
	sh	$2,44($sp)
	sw	$0,20($sp)
	sw	$0,16($sp)
	sw	$0,24($sp)
	sw	$0,28($sp)
	lhu	$2,44($sp)
	bne	$2,$0,$L172
	nop

	li	$2,-1			# 0xffffffffffffffff
	j	$L173
	nop

$L172:
	sw	$0,16($sp)
	j	$L174
	nop

$L177:
	lw	$2,16($sp)
	andi	$2,$2,0x00ff
	move	$4,$2
	move	$5,$0
	lui	$2,%hi(qdmaGetRxRateLimitConfig)
	addiu	$2,$2,%lo(qdmaGetRxRateLimitConfig)
	jalr	$2
	nop

	sw	$2,24($sp)
	lw	$2,24($sp)
	bgez	$2,$L175
	nop

	lw	$2,24($sp)
	j	$L173
	nop

$L175:
	lhu	$3,40($sp)
	lw	$2,24($sp)
	mul	$3,$3,$2
	lhu	$2,44($sp)
	div	$0,$3,$2
	mfhi	$3
	mflo	$2
	sw	$2,28($sp)
	lw	$2,16($sp)
	andi	$2,$2,0x00ff
	move	$3,$2
	lw	$2,28($sp)
	andi	$2,$2,0xffff
	move	$4,$3
	move	$5,$0
	move	$6,$2
	lui	$2,%hi(qdmaSetRxRateLimitConfig)
	addiu	$2,$2,%lo(qdmaSetRxRateLimitConfig)
	jalr	$2
	nop

	sw	$2,20($sp)
	lw	$2,20($sp)
	bgez	$2,$L176
	nop

	lw	$2,20($sp)
	j	$L173
	nop

$L176:
	lw	$2,16($sp)
	addiu	$2,$2,1
	sw	$2,16($sp)
$L174:
	lw	$2,16($sp)
	slt	$2,$2,2
	bne	$2,$0,$L177
	nop

	move	$2,$0
$L173:
	lw	$31,36($sp)
	addiu	$sp,$sp,40
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	qdmaUpdateAllRxRateLimitValue
	.size	qdmaUpdateAllRxRateLimitValue, .-qdmaUpdateAllRxRateLimitValue
	.section	.text.qdma_clear_and_set_dbg_cntr_info,"ax",@progbits
	.align	2
	.globl	qdma_clear_and_set_dbg_cntr_info
	.set	nomips16
	.set	nomicromips
	.ent	qdma_clear_and_set_dbg_cntr_info
	.type	qdma_clear_and_set_dbg_cntr_info, @function
qdma_clear_and_set_dbg_cntr_info:
	.frame	$sp,208,$31		# vars= 208, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-208
	sw	$4,208($sp)
	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lw	$2,0($2)
	sw	$2,0($sp)
	lw	$2,208($sp)
	lbu	$2,0($2)
	sb	$2,4($sp)
	lw	$2,208($sp)
	lbu	$2,0($2)
	sltu	$2,$2,40
	bne	$2,$0,$L179
	nop

	li	$2,-22			# 0xffffffffffffffea
	j	$L180
	nop

$L179:
	lw	$2,208($sp)
	lw	$2,4($2)
	sltu	$2,$2,4
	bne	$2,$0,$L181
	nop

	li	$2,-22			# 0xffffffffffffffea
	j	$L180
	nop

$L181:
	lw	$2,208($sp)
	lbu	$2,8($2)
	bne	$2,$0,$L182
	nop

	lw	$2,208($sp)
	lbu	$2,11($2)
	sltu	$2,$2,8
	bne	$2,$0,$L182
	nop

	li	$2,-22			# 0xffffffffffffffea
	j	$L180
	nop

$L182:
	lw	$2,208($sp)
	lbu	$2,9($2)
	bne	$2,$0,$L183
	nop

	lw	$2,208($sp)
	lbu	$2,12($2)
	sltu	$2,$2,8
	bne	$2,$0,$L183
	nop

	li	$2,-22			# 0xffffffffffffffea
	j	$L180
	nop

$L183:
	lw	$2,208($sp)
	lbu	$2,10($2)
	bne	$2,$0,$L184
	nop

	lw	$2,208($sp)
	lbu	$2,13($2)
	sltu	$2,$2,2
	bne	$2,$0,$L184
	nop

	li	$2,-22			# 0xffffffffffffffea
	j	$L180
	nop

$L184:
	lbu	$2,4($sp)
	sll	$2,$2,3
	move	$3,$2
	lw	$2,0($sp)
	addu	$2,$3,$2
	addiu	$2,$2,768
	sw	$2,56($sp)
	lw	$2,56($sp)
	lw	$2,0($2)
	sw	$2,8($sp)
	lbu	$2,4($sp)
	sll	$2,$2,3
	move	$3,$2
	lw	$2,0($sp)
	addu	$2,$3,$2
	addiu	$3,$2,768
	lw	$4,8($sp)
	li	$2,-1879113728			# 0xffffffff8fff0000
	ori	$2,$2,0xffff
	and	$4,$4,$2
	lw	$2,208($sp)
	lw	$2,4($2)
	sll	$5,$2,28
	li	$2,1879048192			# 0x70000000
	and	$2,$5,$2
	or	$2,$4,$2
	sw	$3,60($sp)
	sw	$2,64($sp)
	lw	$2,60($sp)
	lw	$3,64($sp)
	sw	$3,0($2)
	lbu	$2,4($sp)
	sll	$2,$2,3
	move	$3,$2
	lw	$2,0($sp)
	addu	$2,$3,$2
	addiu	$2,$2,768
	sw	$2,68($sp)
	lw	$2,68($sp)
	lw	$2,0($2)
	sw	$2,12($sp)
	lbu	$2,4($sp)
	sll	$2,$2,3
	move	$3,$2
	lw	$2,0($sp)
	addu	$2,$3,$2
	addiu	$3,$2,768
	lw	$4,12($sp)
	li	$2,-249			# 0xffffffffffffff07
	and	$4,$4,$2
	lw	$2,208($sp)
	lbu	$2,11($2)
	sll	$2,$2,3
	andi	$2,$2,0xff
	or	$2,$4,$2
	sw	$3,72($sp)
	sw	$2,76($sp)
	lw	$2,72($sp)
	lw	$3,76($sp)
	sw	$3,0($2)
	lbu	$2,4($sp)
	sll	$2,$2,3
	move	$3,$2
	lw	$2,0($sp)
	addu	$2,$3,$2
	addiu	$2,$2,768
	sw	$2,80($sp)
	lw	$2,80($sp)
	lw	$2,0($2)
	sw	$2,16($sp)
	lbu	$2,4($sp)
	sll	$2,$2,3
	move	$3,$2
	lw	$2,0($sp)
	addu	$2,$3,$2
	addiu	$3,$2,768
	lw	$4,16($sp)
	li	$2,-8			# 0xfffffffffffffff8
	and	$4,$4,$2
	lw	$2,208($sp)
	lbu	$2,12($2)
	andi	$2,$2,0x7
	or	$2,$4,$2
	sw	$3,84($sp)
	sw	$2,88($sp)
	lw	$2,84($sp)
	lw	$3,88($sp)
	sw	$3,0($2)
	lbu	$2,4($sp)
	sll	$2,$2,3
	move	$3,$2
	lw	$2,0($sp)
	addu	$2,$3,$2
	addiu	$2,$2,768
	sw	$2,92($sp)
	lw	$2,92($sp)
	lw	$2,0($2)
	sw	$2,20($sp)
	lbu	$2,4($sp)
	sll	$2,$2,3
	move	$3,$2
	lw	$2,0($sp)
	addu	$2,$3,$2
	addiu	$3,$2,768
	lw	$4,20($sp)
	li	$2,-2097152			# 0xffffffffffe00000
	ori	$2,$2,0xffff
	and	$4,$4,$2
	lw	$2,208($sp)
	lbu	$2,13($2)
	sll	$2,$2,16
	move	$5,$2
	li	$2,2031616			# 0x1f0000
	and	$2,$5,$2
	or	$2,$4,$2
	sw	$3,96($sp)
	sw	$2,100($sp)
	lw	$2,96($sp)
	lw	$3,100($sp)
	sw	$3,0($2)
	lw	$2,208($sp)
	lbu	$2,8($2)
	bne	$2,$0,$L189
	nop

	lbu	$2,4($sp)
	sll	$2,$2,3
	move	$3,$2
	lw	$2,0($sp)
	addu	$2,$3,$2
	addiu	$2,$2,768
	sw	$2,104($sp)
	lw	$2,104($sp)
	lw	$2,0($2)
	sw	$2,24($sp)
	lbu	$2,4($sp)
	sll	$2,$2,3
	move	$3,$2
	lw	$2,0($sp)
	addu	$2,$3,$2
	addiu	$3,$2,768
	lw	$4,24($sp)
	li	$2,-134283264			# 0xfffffffff7ff0000
	ori	$2,$2,0xffff
	and	$2,$4,$2
	sw	$3,108($sp)
	sw	$2,112($sp)
	lw	$2,108($sp)
	lw	$3,112($sp)
	sw	$3,0($2)
	j	$L191
	nop

$L189:
	lbu	$2,4($sp)
	sll	$2,$2,3
	move	$3,$2
	lw	$2,0($sp)
	addu	$2,$3,$2
	addiu	$2,$2,768
	sw	$2,116($sp)
	lw	$2,116($sp)
	lw	$2,0($2)
	sw	$2,28($sp)
	lbu	$2,4($sp)
	sll	$2,$2,3
	move	$3,$2
	lw	$2,0($sp)
	addu	$2,$3,$2
	addiu	$3,$2,768
	lw	$4,28($sp)
	li	$2,134217728			# 0x8000000
	or	$2,$4,$2
	sw	$3,120($sp)
	sw	$2,124($sp)
	lw	$2,120($sp)
	lw	$3,124($sp)
	sw	$3,0($2)
$L191:
	lw	$2,208($sp)
	lbu	$2,9($2)
	bne	$2,$0,$L193
	nop

	lbu	$2,4($sp)
	sll	$2,$2,3
	move	$3,$2
	lw	$2,0($sp)
	addu	$2,$3,$2
	addiu	$2,$2,768
	sw	$2,128($sp)
	lw	$2,128($sp)
	lw	$2,0($2)
	sw	$2,32($sp)
	lbu	$2,4($sp)
	sll	$2,$2,3
	move	$3,$2
	lw	$2,0($sp)
	addu	$2,$3,$2
	addiu	$3,$2,768
	lw	$4,32($sp)
	li	$2,-67174400			# 0xfffffffffbff0000
	ori	$2,$2,0xffff
	and	$2,$4,$2
	sw	$3,132($sp)
	sw	$2,136($sp)
	lw	$2,132($sp)
	lw	$3,136($sp)
	sw	$3,0($2)
	j	$L195
	nop

$L193:
	lbu	$2,4($sp)
	sll	$2,$2,3
	move	$3,$2
	lw	$2,0($sp)
	addu	$2,$3,$2
	addiu	$2,$2,768
	sw	$2,140($sp)
	lw	$2,140($sp)
	lw	$2,0($2)
	sw	$2,36($sp)
	lbu	$2,4($sp)
	sll	$2,$2,3
	move	$3,$2
	lw	$2,0($sp)
	addu	$2,$3,$2
	addiu	$3,$2,768
	lw	$4,36($sp)
	li	$2,67108864			# 0x4000000
	or	$2,$4,$2
	sw	$3,144($sp)
	sw	$2,148($sp)
	lw	$2,144($sp)
	lw	$3,148($sp)
	sw	$3,0($2)
$L195:
	lw	$2,208($sp)
	lbu	$2,10($2)
	bne	$2,$0,$L197
	nop

	lbu	$2,4($sp)
	sll	$2,$2,3
	move	$3,$2
	lw	$2,0($sp)
	addu	$2,$3,$2
	addiu	$2,$2,768
	sw	$2,152($sp)
	lw	$2,152($sp)
	lw	$2,0($2)
	sw	$2,40($sp)
	lbu	$2,4($sp)
	sll	$2,$2,3
	move	$3,$2
	lw	$2,0($sp)
	addu	$2,$3,$2
	addiu	$3,$2,768
	lw	$4,40($sp)
	li	$2,-33619968			# 0xfffffffffdff0000
	ori	$2,$2,0xffff
	and	$2,$4,$2
	sw	$3,156($sp)
	sw	$2,160($sp)
	lw	$2,156($sp)
	lw	$3,160($sp)
	sw	$3,0($2)
	j	$L199
	nop

$L197:
	lbu	$2,4($sp)
	sll	$2,$2,3
	move	$3,$2
	lw	$2,0($sp)
	addu	$2,$3,$2
	addiu	$2,$2,768
	sw	$2,164($sp)
	lw	$2,164($sp)
	lw	$2,0($2)
	sw	$2,44($sp)
	lbu	$2,4($sp)
	sll	$2,$2,3
	move	$3,$2
	lw	$2,0($sp)
	addu	$2,$3,$2
	addiu	$3,$2,768
	lw	$4,44($sp)
	li	$2,33554432			# 0x2000000
	or	$2,$4,$2
	sw	$3,168($sp)
	sw	$2,172($sp)
	lw	$2,168($sp)
	lw	$3,172($sp)
	sw	$3,0($2)
$L199:
	lbu	$2,4($sp)
	sll	$2,$2,3
	move	$3,$2
	lw	$2,0($sp)
	addu	$2,$3,$2
	addiu	$2,$2,772
	sw	$2,176($sp)
	sw	$0,180($sp)
	lw	$2,176($sp)
	lw	$3,180($sp)
	sw	$3,0($2)
	lw	$2,208($sp)
	lbu	$2,1($2)
	bne	$2,$0,$L201
	nop

	lbu	$2,4($sp)
	sll	$2,$2,3
	move	$3,$2
	lw	$2,0($sp)
	addu	$2,$3,$2
	addiu	$2,$2,768
	sw	$2,184($sp)
	lw	$2,184($sp)
	lw	$2,0($2)
	sw	$2,48($sp)
	lbu	$2,4($sp)
	sll	$2,$2,3
	move	$3,$2
	lw	$2,0($sp)
	addu	$2,$3,$2
	addiu	$3,$2,768
	lw	$2,48($sp)
	ext	$2,$2,0,31
	sw	$3,188($sp)
	sw	$2,192($sp)
	lw	$2,188($sp)
	lw	$3,192($sp)
	sw	$3,0($2)
	j	$L203
	nop

$L201:
	lbu	$2,4($sp)
	sll	$2,$2,3
	move	$3,$2
	lw	$2,0($sp)
	addu	$2,$3,$2
	addiu	$2,$2,768
	sw	$2,196($sp)
	lw	$2,196($sp)
	lw	$2,0($2)
	sw	$2,52($sp)
	lbu	$2,4($sp)
	sll	$2,$2,3
	move	$3,$2
	lw	$2,0($sp)
	addu	$2,$3,$2
	addiu	$3,$2,768
	lw	$4,52($sp)
	li	$2,-2147483648			# 0xffffffff80000000
	or	$2,$4,$2
	sw	$3,200($sp)
	sw	$2,204($sp)
	lw	$2,200($sp)
	lw	$3,204($sp)
	sw	$3,0($2)
$L203:
	move	$2,$0
$L180:
	addiu	$sp,$sp,208
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	qdma_clear_and_set_dbg_cntr_info
	.size	qdma_clear_and_set_dbg_cntr_info, .-qdma_clear_and_set_dbg_cntr_info
	.section	.text.qdma_get_dbg_cntr_info,"ax",@progbits
	.align	2
	.globl	qdma_get_dbg_cntr_info
	.set	nomips16
	.set	nomicromips
	.ent	qdma_get_dbg_cntr_info
	.type	qdma_get_dbg_cntr_info, @function
qdma_get_dbg_cntr_info:
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
	lbu	$2,0($2)
	sb	$2,4($sp)
	lw	$2,48($sp)
	lbu	$2,0($2)
	sltu	$2,$2,40
	bne	$2,$0,$L206
	nop

	li	$2,-22			# 0xffffffffffffffea
	j	$L207
	nop

$L206:
	lbu	$2,4($sp)
	sll	$2,$2,3
	move	$3,$2
	lw	$2,0($sp)
	addu	$2,$3,$2
	addiu	$2,$2,768
	sw	$2,8($sp)
	lw	$2,8($sp)
	lw	$2,0($2)
	lw	$2,48($sp)
	sb	$0,1($2)
	lbu	$2,4($sp)
	sll	$2,$2,3
	move	$3,$2
	lw	$2,0($sp)
	addu	$2,$3,$2
	addiu	$2,$2,768
	sw	$2,12($sp)
	lw	$2,12($sp)
	lw	$3,0($2)
	li	$2,1879048192			# 0x70000000
	and	$2,$3,$2
	srl	$3,$2,28
	lw	$2,48($sp)
	sw	$3,4($2)
	lbu	$2,4($sp)
	sll	$2,$2,3
	move	$3,$2
	lw	$2,0($sp)
	addu	$2,$3,$2
	addiu	$2,$2,768
	sw	$2,16($sp)
	lw	$2,16($sp)
	lw	$2,0($2)
	andi	$2,$2,0xf8
	srl	$2,$2,3
	andi	$3,$2,0x00ff
	lw	$2,48($sp)
	sb	$3,11($2)
	lbu	$2,4($sp)
	sll	$2,$2,3
	move	$3,$2
	lw	$2,0($sp)
	addu	$2,$3,$2
	addiu	$2,$2,768
	sw	$2,20($sp)
	lw	$2,20($sp)
	lw	$2,0($2)
	andi	$2,$2,0x00ff
	andi	$2,$2,0x7
	andi	$3,$2,0x00ff
	lw	$2,48($sp)
	sb	$3,12($2)
	lbu	$2,4($sp)
	sll	$2,$2,3
	move	$3,$2
	lw	$2,0($sp)
	addu	$2,$3,$2
	addiu	$2,$2,768
	sw	$2,24($sp)
	lw	$2,24($sp)
	lw	$3,0($2)
	li	$2,2031616			# 0x1f0000
	and	$2,$3,$2
	srl	$2,$2,16
	andi	$3,$2,0x00ff
	lw	$2,48($sp)
	sb	$3,13($2)
	lbu	$2,4($sp)
	sll	$2,$2,3
	move	$3,$2
	lw	$2,0($sp)
	addu	$2,$3,$2
	addiu	$2,$2,768
	sw	$2,28($sp)
	lw	$2,28($sp)
	lw	$2,0($2)
	lw	$2,48($sp)
	sb	$0,8($2)
	lbu	$2,4($sp)
	sll	$2,$2,3
	move	$3,$2
	lw	$2,0($sp)
	addu	$2,$3,$2
	addiu	$2,$2,768
	sw	$2,32($sp)
	lw	$2,32($sp)
	lw	$2,0($2)
	lw	$2,48($sp)
	sb	$0,9($2)
	lbu	$2,4($sp)
	sll	$2,$2,3
	move	$3,$2
	lw	$2,0($sp)
	addu	$2,$3,$2
	addiu	$2,$2,768
	sw	$2,36($sp)
	lw	$2,36($sp)
	lw	$2,0($2)
	lw	$2,48($sp)
	sb	$0,10($2)
	lbu	$2,4($sp)
	sll	$2,$2,3
	move	$3,$2
	lw	$2,0($sp)
	addu	$2,$3,$2
	addiu	$2,$2,772
	sw	$2,40($sp)
	lw	$2,40($sp)
	lw	$3,0($2)
	lw	$2,48($sp)
	sw	$3,16($2)
	move	$2,$0
$L207:
	addiu	$sp,$sp,48
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	qdma_get_dbg_cntr_info
	.size	qdma_get_dbg_cntr_info, .-qdma_get_dbg_cntr_info
	.section	.text.qdma_set_dbg_cntr_default_config,"ax",@progbits
	.align	2
	.globl	qdma_set_dbg_cntr_default_config
	.set	nomips16
	.set	nomicromips
	.ent	qdma_set_dbg_cntr_default_config
	.type	qdma_set_dbg_cntr_default_config, @function
qdma_set_dbg_cntr_default_config:
	.frame	$sp,48,$31		# vars= 24, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-48
	sw	$31,44($sp)
	sw	$0,16($sp)
	li	$2,1			# 0x1
	sb	$2,21($sp)
	li	$2,1			# 0x1
	sb	$2,28($sp)
	li	$2,1			# 0x1
	sb	$2,29($sp)
	li	$2,1			# 0x1
	sb	$2,30($sp)
	sb	$0,32($sp)
	sb	$0,31($sp)
	sb	$0,33($sp)
	sw	$0,16($sp)
	j	$L218
	nop

$L229:
	lw	$2,16($sp)
	sltu	$2,$2,8
	beq	$2,$0,$L219
	nop

	lw	$2,16($sp)
	sll	$3,$2,2
	lui	$2,%hi($L221)
	addiu	$2,$2,%lo($L221)
	addu	$2,$3,$2
	lw	$2,0($2)
	j	$2
	nop

	.rdata
	.align	2
	.align	2
$L221:
	.word	$L220
	.word	$L222
	.word	$L223
	.word	$L224
	.word	$L225
	.word	$L226
	.word	$L227
	.word	$L228
	.section	.text.qdma_set_dbg_cntr_default_config
$L220:
	sw	$0,24($sp)
	j	$L219
	nop

$L222:
	li	$2,1			# 0x1
	sw	$2,24($sp)
	j	$L219
	nop

$L223:
	li	$2,2			# 0x2
	sw	$2,24($sp)
	j	$L219
	nop

$L224:
	li	$2,3			# 0x3
	sw	$2,24($sp)
	j	$L219
	nop

$L225:
	sb	$0,30($sp)
	sb	$0,33($sp)
	sw	$0,24($sp)
	j	$L219
	nop

$L226:
	sb	$0,30($sp)
	li	$2,1			# 0x1
	sb	$2,33($sp)
	sw	$0,24($sp)
	j	$L219
	nop

$L227:
	sb	$0,30($sp)
	sb	$0,33($sp)
	li	$2,2			# 0x2
	sw	$2,24($sp)
	j	$L219
	nop

$L228:
	sb	$0,30($sp)
	li	$2,1			# 0x1
	sb	$2,33($sp)
	li	$2,2			# 0x2
	sw	$2,24($sp)
	nop
$L219:
	lw	$2,16($sp)
	andi	$2,$2,0x00ff
	addiu	$2,$2,32
	andi	$2,$2,0x00ff
	sb	$2,20($sp)
	addiu	$2,$sp,20
	move	$4,$2
	lui	$2,%hi(qdma_clear_and_set_dbg_cntr_info)
	addiu	$2,$2,%lo(qdma_clear_and_set_dbg_cntr_info)
	jalr	$2
	nop

	lw	$2,16($sp)
	addiu	$2,$2,1
	sw	$2,16($sp)
$L218:
	lw	$2,16($sp)
	slt	$2,$2,8
	bne	$2,$0,$L229
	nop

	move	$2,$0
	lw	$31,44($sp)
	addiu	$sp,$sp,48
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	qdma_set_dbg_cntr_default_config
	.size	qdma_set_dbg_cntr_default_config, .-qdma_set_dbg_cntr_default_config
	.rdata
	.align	2
$LC0:
	.ascii	"dbgMemXsCfg is 0x%08x, valueLow:0x%08x, valueHigh:0x%08x"
	.ascii	".\012\000"
	.section	.text.qdmaSetDbgMemXsConfig,"ax",@progbits
	.align	2
	.globl	qdmaSetDbgMemXsConfig
	.set	nomips16
	.set	nomicromips
	.ent	qdmaSetDbgMemXsConfig
	.type	qdmaSetDbgMemXsConfig, @function
qdmaSetDbgMemXsConfig:
	.frame	$sp,80,$31		# vars= 56, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-80
	sw	$31,76($sp)
	sw	$4,80($sp)
	move	$3,$5
	move	$2,$6
	sw	$7,92($sp)
	sb	$3,84($sp)
	sh	$2,88($sp)
	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lw	$2,0($2)
	sw	$2,16($sp)
	sw	$0,20($sp)
	lw	$2,16($sp)
	addiu	$2,$2,228
	sw	$2,24($sp)
	lw	$2,92($sp)
	sw	$2,28($sp)
	lw	$2,24($sp)
	lw	$3,28($sp)
	sw	$3,0($2)
	lw	$2,16($sp)
	addiu	$2,$2,232
	sw	$2,32($sp)
	lw	$2,96($sp)
	sw	$2,36($sp)
	lw	$2,32($sp)
	lw	$3,36($sp)
	sw	$3,0($2)
	lw	$2,80($sp)
	sll	$3,$2,24
	li	$2,117440512			# 0x7000000
	and	$3,$3,$2
	lbu	$2,84($sp)
	sll	$2,$2,16
	move	$4,$2
	li	$2,2031616			# 0x1f0000
	and	$2,$4,$2
	or	$3,$3,$2
	lhu	$2,88($sp)
	or	$3,$3,$2
	li	$2,-2147483648			# 0xffffffff80000000
	or	$2,$3,$2
	sw	$2,20($sp)
	lui	$2,%hi($LC0)
	addiu	$4,$2,%lo($LC0)
	lw	$5,20($sp)
	lw	$6,92($sp)
	lw	$7,96($sp)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lw	$2,16($sp)
	addiu	$2,$2,224
	sw	$2,40($sp)
	lw	$2,20($sp)
	sw	$2,44($sp)
	lw	$2,40($sp)
	lw	$3,44($sp)
	sw	$3,0($2)
	lw	$2,16($sp)
	addiu	$2,$2,224
	sw	$2,48($sp)
	li	$2,1073741824			# 0x40000000
	sw	$2,52($sp)
	li	$2,3			# 0x3
	sw	$2,56($sp)
	sw	$0,64($sp)
	j	$L232
	nop

$L235:
	lw	$2,48($sp)
	sw	$2,60($sp)
	lw	$2,60($sp)
	lw	$2,0($2)
	sw	$2,64($sp)
	lw	$3,64($sp)
	lw	$2,52($sp)
	and	$2,$3,$2
	bne	$2,$0,$L234
	nop

	li	$4,1000			# 0x3e8
	lui	$2,%hi(__udelay)
	addiu	$2,$2,%lo(__udelay)
	jalr	$2
	nop

$L232:
	lw	$2,56($sp)
	addiu	$3,$2,-1
	sw	$3,56($sp)
	bne	$2,$0,$L235
	nop

$L234:
	lw	$2,56($sp)
	bgez	$2,$L236
	nop

	li	$2,-62			# 0xffffffffffffffc2
	j	$L237
	nop

$L236:
	move	$2,$0
$L237:
	bgez	$2,$L238
	nop

	li	$2,-62			# 0xffffffffffffffc2
	j	$L239
	nop

$L238:
	move	$2,$0
$L239:
	lw	$31,76($sp)
	addiu	$sp,$sp,80
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	qdmaSetDbgMemXsConfig
	.size	qdmaSetDbgMemXsConfig, .-qdmaSetDbgMemXsConfig
	.section	.text.qdmaGetDbgMemXsConfig,"ax",@progbits
	.align	2
	.globl	qdmaGetDbgMemXsConfig
	.set	nomips16
	.set	nomicromips
	.ent	qdmaGetDbgMemXsConfig
	.type	qdmaGetDbgMemXsConfig, @function
qdmaGetDbgMemXsConfig:
	.frame	$sp,64,$31		# vars= 40, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-64
	sw	$31,60($sp)
	sw	$4,64($sp)
	move	$3,$5
	move	$2,$6
	sw	$7,76($sp)
	sb	$3,68($sp)
	sh	$2,72($sp)
	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lw	$2,0($2)
	sw	$2,16($sp)
	sw	$0,20($sp)
	lw	$2,64($sp)
	sll	$3,$2,24
	li	$2,117440512			# 0x7000000
	and	$3,$3,$2
	lbu	$2,68($sp)
	sll	$2,$2,16
	move	$4,$2
	li	$2,2031616			# 0x1f0000
	and	$2,$4,$2
	or	$3,$3,$2
	lhu	$2,72($sp)
	or	$2,$3,$2
	sw	$2,20($sp)
	lw	$2,16($sp)
	addiu	$2,$2,224
	sw	$2,24($sp)
	lw	$2,20($sp)
	sw	$2,28($sp)
	lw	$2,24($sp)
	lw	$3,28($sp)
	sw	$3,0($2)
	lw	$2,16($sp)
	addiu	$2,$2,224
	sw	$2,32($sp)
	li	$2,1073741824			# 0x40000000
	sw	$2,36($sp)
	li	$2,3			# 0x3
	sw	$2,40($sp)
	sw	$0,52($sp)
	j	$L241
	nop

$L244:
	lw	$2,32($sp)
	sw	$2,44($sp)
	lw	$2,44($sp)
	lw	$2,0($2)
	sw	$2,52($sp)
	lw	$3,52($sp)
	lw	$2,36($sp)
	and	$2,$3,$2
	bne	$2,$0,$L243
	nop

	li	$4,1000			# 0x3e8
	lui	$2,%hi(__udelay)
	addiu	$2,$2,%lo(__udelay)
	jalr	$2
	nop

$L241:
	lw	$2,40($sp)
	addiu	$3,$2,-1
	sw	$3,40($sp)
	bne	$2,$0,$L244
	nop

$L243:
	lw	$2,40($sp)
	bgez	$2,$L245
	nop

	li	$2,-62			# 0xffffffffffffffc2
	j	$L246
	nop

$L245:
	move	$2,$0
$L246:
	bgez	$2,$L247
	nop

	li	$2,-62			# 0xffffffffffffffc2
	j	$L248
	nop

$L247:
	lw	$2,16($sp)
	addiu	$2,$2,228
	sw	$2,48($sp)
	lw	$2,48($sp)
	lw	$3,0($2)
	lw	$2,76($sp)
	sw	$3,0($2)
	move	$2,$0
$L248:
	lw	$31,60($sp)
	addiu	$sp,$sp,64
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	qdmaGetDbgMemXsConfig
	.size	qdmaGetDbgMemXsConfig, .-qdmaGetDbgMemXsConfig
	.section	.text.isDefaultQueue,"ax",@progbits
	.align	2
	.globl	isDefaultQueue
	.set	nomips16
	.set	nomicromips
	.ent	isDefaultQueue
	.type	isDefaultQueue, @function
isDefaultQueue:
	.frame	$sp,8,$31		# vars= 8, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-8
	sw	$4,8($sp)
	sw	$0,0($sp)
	lw	$2,8($sp)
	lw	$2,148($2)
	andi	$2,$2,0xf0
	sw	$2,0($sp)
	lw	$2,0($sp)
	bne	$2,$0,$L251
	nop

	li	$2,1			# 0x1
	j	$L252
	nop

$L251:
	lui	$2,%hi(gQueueMask)
	lw	$3,%lo(gQueueMask)($2)
	lw	$2,0($sp)
	sra	$2,$2,4
	addiu	$2,$2,-1
	sra	$2,$3,$2
	andi	$2,$2,0x1
	beq	$2,$0,$L253
	nop

	move	$2,$0
	j	$L252
	nop

$L253:
	li	$2,1			# 0x1
$L252:
	addiu	$sp,$sp,8
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	isDefaultQueue
	.size	isDefaultQueue, .-isDefaultQueue
	.section	.text.isWeigt0,"ax",@progbits
	.align	2
	.globl	isWeigt0
	.set	nomips16
	.set	nomicromips
	.ent	isWeigt0
	.type	isWeigt0, @function
isWeigt0:
	.frame	$sp,8,$31		# vars= 8, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-8
	sw	$4,8($sp)
	sw	$0,0($sp)
	lw	$2,8($sp)
	lw	$2,148($2)
	andi	$2,$2,0xf0
	srl	$2,$2,4
	sw	$2,0($sp)
	lw	$2,0($sp)
	blez	$2,$L255
	nop

	lw	$2,0($sp)
	slt	$2,$2,5
	bne	$2,$0,$L256
	nop

$L255:
	move	$2,$0
	j	$L257
	nop

$L256:
	lui	$2,%hi(qos_wrr_info)
	lw	$3,0($sp)
	sll	$3,$3,2
	addiu	$2,$2,%lo(qos_wrr_info)
	addu	$2,$3,$2
	lw	$2,0($2)
	beq	$2,$0,$L258
	nop

	move	$2,$0
	j	$L257
	nop

$L258:
	li	$2,1			# 0x1
$L257:
	addiu	$sp,$sp,8
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	isWeigt0
	.size	isWeigt0, .-isWeigt0
	.section	.text.isPriorityPkt,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	isPriorityPkt
	.type	isPriorityPkt, @function
isPriorityPkt:
	.frame	$sp,16,$31		# vars= 16, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-16
	sw	$4,16($sp)
	sw	$5,20($sp)
	lw	$2,16($sp)
	bne	$2,$0,$L260
	nop

	move	$2,$0
	j	$L261
	nop

$L260:
	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lbu	$3,142($2)
	li	$2,3			# 0x3
	bne	$3,$2,$L262
	nop

	lw	$2,20($sp)
	lw	$2,0($2)
	andi	$2,$2,0x800
	bne	$2,$0,$L263
	nop

	lw	$2,20($sp)
	lw	$2,0($2)
	andi	$2,$2,0x400
	bne	$2,$0,$L263
	nop

$L262:
	lw	$2,16($sp)
	addiu	$2,$2,12
	sw	$2,16($sp)
	lw	$2,16($sp)
	lhu	$2,0($2)
	sh	$2,0($sp)
	lw	$2,16($sp)
	addiu	$2,$2,2
	sw	$2,16($sp)
	lhu	$3,0($sp)
	li	$2,33024			# 0x8100
	bne	$3,$2,$L264
	nop

	lw	$2,16($sp)
	addiu	$2,$2,2
	sw	$2,16($sp)
	lw	$2,16($sp)
	lhu	$2,0($2)
	sh	$2,0($sp)
	lw	$2,16($sp)
	addiu	$2,$2,2
	sw	$2,16($sp)
$L264:
	lhu	$3,0($sp)
	li	$2,34916			# 0x8864
	bne	$3,$2,$L265
	nop

	lw	$2,16($sp)
	addiu	$2,$2,6
	sw	$2,16($sp)
	lw	$2,16($sp)
	lhu	$2,0($2)
	sh	$2,2($sp)
	lhu	$3,2($sp)
	li	$2,49185			# 0xc021
	bne	$3,$2,$L266
	nop

	li	$2,1			# 0x1
	j	$L261
	nop

$L266:
	lhu	$3,2($sp)
	li	$2,33			# 0x21
	beq	$3,$2,$L267
	nop

	move	$2,$0
	j	$L261
	nop

$L267:
	lw	$2,16($sp)
	addiu	$2,$2,2
	sw	$2,16($sp)
	j	$L263
	nop

$L265:
	lhu	$3,0($sp)
	li	$2,34915			# 0x8863
	bne	$3,$2,$L268
	nop

	li	$2,1			# 0x1
	j	$L261
	nop

$L268:
	lhu	$3,0($sp)
	li	$2,2054			# 0x806
	bne	$3,$2,$L269
	nop

	li	$2,1			# 0x1
	j	$L261
	nop

$L269:
	lhu	$3,0($sp)
	li	$2,34525			# 0x86dd
	bne	$3,$2,$L270
	nop

	lw	$2,16($sp)
	addiu	$2,$2,6
	sw	$2,16($sp)
	lw	$2,16($sp)
	lbu	$2,0($2)
	sb	$2,4($sp)
	lbu	$3,4($sp)
	li	$2,58			# 0x3a
	bne	$3,$2,$L263
	nop

	lw	$2,16($sp)
	addiu	$2,$2,34
	sw	$2,16($sp)
	lw	$2,16($sp)
	lbu	$2,0($2)
	sb	$2,5($sp)
	lbu	$3,5($sp)
	li	$2,135			# 0x87
	bne	$3,$2,$L263
	nop

	li	$2,1			# 0x1
	j	$L261
	nop

$L270:
	lhu	$3,0($sp)
	li	$2,2048			# 0x800
	beq	$3,$2,$L263
	nop

	move	$2,$0
	j	$L261
	nop

$L263:
	lw	$2,16($sp)
	lhu	$2,0($2)
	sh	$2,2($sp)
	lhu	$3,2($sp)
	li	$2,49185			# 0xc021
	bne	$3,$2,$L271
	nop

	li	$2,1			# 0x1
	j	$L261
	nop

$L271:
	lhu	$3,2($sp)
	li	$2,33			# 0x21
	bne	$3,$2,$L272
	nop

	lw	$2,16($sp)
	addiu	$2,$2,2
	sw	$2,16($sp)
$L272:
	lw	$2,16($sp)
	lbu	$2,0($2)
	sb	$2,6($sp)
	lbu	$2,6($sp)
	andi	$3,$2,0xf0
	li	$2,64			# 0x40
	beq	$3,$2,$L273
	nop

	move	$2,$0
	j	$L261
	nop

$L273:
	lw	$2,16($sp)
	lbu	$2,9($2)
	sb	$2,7($sp)
	lbu	$3,7($sp)
	li	$2,1			# 0x1
	beq	$3,$2,$L274
	nop

	lbu	$3,7($sp)
	li	$2,2			# 0x2
	bne	$3,$2,$L275
	nop

$L274:
	li	$2,1			# 0x1
	j	$L261
	nop

$L275:
	lbu	$3,7($sp)
	li	$2,6			# 0x6
	beq	$3,$2,$L276
	nop

	move	$2,$0
	j	$L261
	nop

$L276:
	lbu	$2,6($sp)
	andi	$2,$2,0xf
	sll	$2,$2,2
	move	$3,$2
	lw	$2,16($sp)
	addu	$2,$2,$3
	sw	$2,16($sp)
	lw	$2,16($sp)
	lbu	$2,13($2)
	sb	$2,8($sp)
	lbu	$2,8($sp)
	andi	$2,$2,0x10
	bne	$2,$0,$L277
	nop

	lbu	$3,8($sp)
	li	$2,2			# 0x2
	bne	$3,$2,$L278
	nop

$L277:
	li	$2,1			# 0x1
	j	$L261
	nop

$L278:
	move	$2,$0
$L261:
	addiu	$sp,$sp,16
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	isPriorityPkt
	.size	isPriorityPkt, .-isPriorityPkt
	.section	.text.qdma_get_ringIdx,"ax",@progbits
	.align	2
	.globl	qdma_get_ringIdx
	.set	nomips16
	.set	nomicromips
	.ent	qdma_get_ringIdx
	.type	qdma_get_ringIdx, @function
qdma_get_ringIdx:
	.frame	$sp,32,$31		# vars= 8, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-32
	sw	$31,28($sp)
	sw	$4,32($sp)
	sw	$5,36($sp)
	sw	$0,16($sp)
	lui	$2,%hi(priPktChk)
	lw	$2,%lo(priPktChk)($2)
	beq	$2,$0,$L280
	nop

	lw	$2,32($sp)
	lw	$2,80($2)
	lui	$3,%hi(priPktChkLen)
	lw	$3,%lo(priPktChkLen)($3)
	sltu	$2,$2,$3
	beq	$2,$0,$L280
	nop

	lw	$2,32($sp)
	lw	$2,256($2)
	move	$4,$2
	lw	$5,36($sp)
	lui	$2,%hi(isPriorityPkt)
	addiu	$2,$2,%lo(isPriorityPkt)
	jalr	$2
	nop

	beq	$2,$0,$L280
	nop

	li	$2,1			# 0x1
	sw	$2,16($sp)
$L280:
	lw	$2,16($sp)
	lw	$31,28($sp)
	addiu	$sp,$sp,32
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	qdma_get_ringIdx
	.size	qdma_get_ringIdx, .-qdma_get_ringIdx
	.section	.imem_text,"ax",@progbits
	.align	2
	.globl	macResourceLimit
	.set	nomips16
	.set	nomicromips
	.ent	macResourceLimit
	.type	macResourceLimit, @function
macResourceLimit:
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	sw	$4,0($sp)
	sw	$5,4($sp)
	sw	$6,8($sp)
	lw	$2,4($sp)
	sll	$3,$2,2
	lui	$2,%hi(chnlLimit)
	addiu	$2,$2,%lo(chnlLimit)
	addu	$2,$3,$2
	lw	$2,0($2)
	move	$3,$2
	lui	$2,%hi(channel_limit_threshold)
	lw	$2,%lo(channel_limit_threshold)($2)
	sltu	$2,$3,$2
	beq	$2,$0,$L283
	nop

	lw	$2,4($sp)
	sll	$3,$2,3
	lw	$2,8($sp)
	addu	$2,$3,$2
	sll	$3,$2,2
	lui	$2,%hi(queueLimit)
	addiu	$2,$2,%lo(queueLimit)
	addu	$2,$3,$2
	lw	$2,0($2)
	move	$3,$2
	lui	$2,%hi(queue_limit_threshold)
	lw	$2,%lo(queue_limit_threshold)($2)
	sltu	$2,$3,$2
	bne	$2,$0,$L284
	nop

$L283:
	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lhu	$3,192($2)
	addiu	$3,$3,1
	andi	$3,$3,0xffff
	sh	$3,192($2)
	li	$2,-14			# 0xfffffffffffffff2
	j	$L285
	nop

$L284:
	move	$2,$0
$L285:
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	macResourceLimit
	.size	macResourceLimit, .-macResourceLimit
	.rdata
	.align	2
$LC1:
	.ascii	"########WARNING: Enable INT: Get INT_ENABLE:%.8x, RETRY:"
	.ascii	"%d\012\000"
	.section	.imem_text
	.align	2
	.globl	qdmaEnableInt
	.set	nomips16
	.set	nomicromips
	.ent	qdmaEnableInt
	.type	qdmaEnableInt, @function
qdmaEnableInt:
	.frame	$sp,80,$31		# vars= 56, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-80
	sw	$31,76($sp)
	sw	$4,80($sp)
	sw	$5,84($sp)
	sw	$0,20($sp)
	li	$2,3			# 0x3
	sw	$2,16($sp)
	sw	$0,24($sp)
	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	addiu	$2,$2,12
	sw	$2,28($sp)
	lw	$2,28($sp)
	move	$4,$2
	lui	$2,%hi(_raw_spin_lock_irqsave)
	addiu	$2,$2,%lo(_raw_spin_lock_irqsave)
	jalr	$2
	nop

	sw	$2,20($sp)
	j	$L288
	nop

$L292:
	lw	$2,80($sp)
	addiu	$2,$2,84
	sw	$2,32($sp)
	lw	$2,32($sp)
	lw	$2,0($2)
	sw	$2,24($sp)
	lw	$2,24($sp)
	bne	$2,$0,$L290
	nop

	li	$3,3			# 0x3
	lw	$2,16($sp)
	subu	$3,$3,$2
	lui	$2,%hi($LC1)
	addiu	$4,$2,%lo($LC1)
	lw	$5,24($sp)
	move	$6,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	j	$L288
	nop

$L290:
	lw	$2,80($sp)
	addiu	$3,$2,84
	lw	$4,24($sp)
	lw	$2,84($sp)
	or	$2,$4,$2
	sw	$3,36($sp)
	sw	$2,40($sp)
	lw	$2,36($sp)
	lw	$3,40($sp)
	sw	$3,0($2)
	j	$L291
	nop

$L288:
	lw	$2,16($sp)
	addiu	$3,$2,-1
	sw	$3,16($sp)
	bne	$2,$0,$L292
	nop

$L291:
	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	addiu	$2,$2,12
	sw	$2,44($sp)
	lw	$2,20($sp)
	sw	$2,48($sp)
	lw	$2,44($sp)
	move	$4,$2
	lw	$5,48($sp)
	lui	$2,%hi(_raw_spin_unlock_irqrestore)
	addiu	$2,$2,%lo(_raw_spin_unlock_irqrestore)
	jalr	$2
	nop

	move	$2,$0
	lw	$31,76($sp)
	addiu	$sp,$sp,80
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	qdmaEnableInt
	.size	qdmaEnableInt, .-qdmaEnableInt
	.rdata
	.align	2
$LC2:
	.ascii	"########WARNING: Disable INT: Get INT_ENABLE:%.8x, RETRY"
	.ascii	":%d\012\000"
	.section	.imem_text
	.align	2
	.globl	qdmaDisableInt
	.set	nomips16
	.set	nomicromips
	.ent	qdmaDisableInt
	.type	qdmaDisableInt, @function
qdmaDisableInt:
	.frame	$sp,80,$31		# vars= 56, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-80
	sw	$31,76($sp)
	sw	$4,80($sp)
	sw	$5,84($sp)
	sw	$0,20($sp)
	li	$2,3			# 0x3
	sw	$2,16($sp)
	sw	$0,24($sp)
	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	addiu	$2,$2,12
	sw	$2,28($sp)
	lw	$2,28($sp)
	move	$4,$2
	lui	$2,%hi(_raw_spin_lock_irqsave)
	addiu	$2,$2,%lo(_raw_spin_lock_irqsave)
	jalr	$2
	nop

	sw	$2,20($sp)
	j	$L296
	nop

$L300:
	lw	$2,80($sp)
	addiu	$2,$2,84
	sw	$2,32($sp)
	lw	$2,32($sp)
	lw	$2,0($2)
	sw	$2,24($sp)
	lw	$2,24($sp)
	bne	$2,$0,$L298
	nop

	li	$3,3			# 0x3
	lw	$2,16($sp)
	subu	$3,$3,$2
	lui	$2,%hi($LC2)
	addiu	$4,$2,%lo($LC2)
	lw	$5,24($sp)
	move	$6,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	j	$L296
	nop

$L298:
	lw	$2,80($sp)
	addiu	$3,$2,84
	lw	$2,84($sp)
	nor	$4,$0,$2
	lw	$2,24($sp)
	and	$2,$4,$2
	sw	$3,36($sp)
	sw	$2,40($sp)
	lw	$2,36($sp)
	lw	$3,40($sp)
	sw	$3,0($2)
	j	$L299
	nop

$L296:
	lw	$2,16($sp)
	addiu	$3,$2,-1
	sw	$3,16($sp)
	bne	$2,$0,$L300
	nop

$L299:
	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	addiu	$2,$2,12
	sw	$2,44($sp)
	lw	$2,20($sp)
	sw	$2,48($sp)
	lw	$2,44($sp)
	move	$4,$2
	lw	$5,48($sp)
	lui	$2,%hi(_raw_spin_unlock_irqrestore)
	addiu	$2,$2,%lo(_raw_spin_unlock_irqrestore)
	jalr	$2
	nop

	move	$2,$0
	lw	$31,76($sp)
	addiu	$sp,$sp,80
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	qdmaDisableInt
	.size	qdmaDisableInt, .-qdmaDisableInt
	.section	.text.qdmaSetIntMask,"ax",@progbits
	.align	2
	.globl	qdmaSetIntMask
	.set	nomips16
	.set	nomicromips
	.ent	qdmaSetIntMask
	.type	qdmaSetIntMask, @function
qdmaSetIntMask:
	.frame	$sp,64,$31		# vars= 40, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-64
	sw	$31,60($sp)
	sw	$4,64($sp)
	sw	$5,68($sp)
	sw	$0,16($sp)
	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	addiu	$2,$2,12
	sw	$2,20($sp)
	lw	$2,20($sp)
	move	$4,$2
	lui	$2,%hi(_raw_spin_lock_irqsave)
	addiu	$2,$2,%lo(_raw_spin_lock_irqsave)
	jalr	$2
	nop

	sw	$2,16($sp)
	lw	$2,64($sp)
	addiu	$2,$2,84
	sw	$2,24($sp)
	lw	$2,68($sp)
	sw	$2,28($sp)
	lw	$2,24($sp)
	lw	$3,28($sp)
	sw	$3,0($2)
	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	addiu	$2,$2,12
	sw	$2,32($sp)
	lw	$2,16($sp)
	sw	$2,36($sp)
	lw	$2,32($sp)
	move	$4,$2
	lw	$5,36($sp)
	lui	$2,%hi(_raw_spin_unlock_irqrestore)
	addiu	$2,$2,%lo(_raw_spin_unlock_irqrestore)
	jalr	$2
	nop

	move	$2,$0
	lw	$31,60($sp)
	addiu	$sp,$sp,64
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	qdmaSetIntMask
	.size	qdmaSetIntMask, .-qdmaSetIntMask
	.section	.imem_text
	.align	2
	.globl	qdmaGetIntMask
	.set	nomips16
	.set	nomicromips
	.ent	qdmaGetIntMask
	.type	qdmaGetIntMask, @function
qdmaGetIntMask:
	.frame	$sp,64,$31		# vars= 40, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-64
	sw	$31,60($sp)
	sw	$4,64($sp)
	sw	$0,16($sp)
	sw	$0,20($sp)
	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	addiu	$2,$2,12
	sw	$2,24($sp)
	lw	$2,24($sp)
	move	$4,$2
	lui	$2,%hi(_raw_spin_lock_irqsave)
	addiu	$2,$2,%lo(_raw_spin_lock_irqsave)
	jalr	$2
	nop

	sw	$2,16($sp)
	lw	$2,64($sp)
	addiu	$2,$2,84
	sw	$2,28($sp)
	lw	$2,28($sp)
	lw	$2,0($2)
	sw	$2,20($sp)
	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	addiu	$2,$2,12
	sw	$2,32($sp)
	lw	$2,16($sp)
	sw	$2,36($sp)
	lw	$2,32($sp)
	move	$4,$2
	lw	$5,36($sp)
	lui	$2,%hi(_raw_spin_unlock_irqrestore)
	addiu	$2,$2,%lo(_raw_spin_unlock_irqrestore)
	jalr	$2
	nop

	lw	$2,20($sp)
	lw	$31,60($sp)
	addiu	$sp,$sp,64
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	qdmaGetIntMask
	.size	qdmaGetIntMask, .-qdmaGetIntMask
	.rdata
	.align	2
$LC3:
	.ascii	"%04x: dma busy register is 0x%08x\015\012\000"
	.align	2
$LC4:
	.ascii	"\015\012tx_busy enable cnt is %d, rx_busy enable cnt is "
	.ascii	"%d\015\012\000"
	.section	.text.qdma_dma_busy_timer,"ax",@progbits
	.align	2
	.globl	qdma_dma_busy_timer
	.set	nomips16
	.set	nomicromips
	.ent	qdma_dma_busy_timer
	.type	qdma_dma_busy_timer, @function
qdma_dma_busy_timer:
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
	lw	$2,16($sp)
	addiu	$2,$2,4
	sw	$2,24($sp)
	lw	$2,24($sp)
	lw	$2,0($2)
	sw	$2,20($sp)
	lui	$2,%hi(dma_busy_round_cnt)
	lw	$3,%lo(dma_busy_round_cnt)($2)
	lui	$2,%hi($LC3)
	addiu	$4,$2,%lo($LC3)
	move	$5,$3
	lw	$6,20($sp)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
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
	lw	$2,20($sp)
	andi	$2,$2,0x2
	beq	$2,$0,$L311
	nop

	lui	$2,%hi(tx_dma_busy_enable_cnt)
	lw	$2,%lo(tx_dma_busy_enable_cnt)($2)
	addiu	$3,$2,1
	lui	$2,%hi(tx_dma_busy_enable_cnt)
	sw	$3,%lo(tx_dma_busy_enable_cnt)($2)
$L311:
	lw	$2,20($sp)
	andi	$2,$2,0x8
	beq	$2,$0,$L312
	nop

	lui	$2,%hi(rx_dma_busy_enable_cnt)
	lw	$2,%lo(rx_dma_busy_enable_cnt)($2)
	addiu	$3,$2,1
	lui	$2,%hi(rx_dma_busy_enable_cnt)
	sw	$3,%lo(rx_dma_busy_enable_cnt)($2)
$L312:
	lui	$2,%hi(dma_busy_round_cnt)
	lw	$2,%lo(dma_busy_round_cnt)($2)
	addiu	$4,$2,-1
	lui	$3,%hi(dma_busy_round_cnt)
	sw	$4,%lo(dma_busy_round_cnt)($3)
	beq	$2,$0,$L313
	nop

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

	j	$L309
	nop

$L313:
	lui	$2,%hi(tx_dma_busy_enable_cnt)
	lw	$5,%lo(tx_dma_busy_enable_cnt)($2)
	lui	$2,%hi(rx_dma_busy_enable_cnt)
	lw	$3,%lo(rx_dma_busy_enable_cnt)($2)
	lui	$2,%hi($LC4)
	addiu	$4,$2,%lo($LC4)
	move	$6,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L309:
	lw	$31,36($sp)
	addiu	$sp,$sp,40
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	qdma_dma_busy_timer
	.size	qdma_dma_busy_timer, .-qdma_dma_busy_timer
	.rdata
	.align	2
$LC5:
	.ascii	"\015\012%04x: qdma_reg_value[%x]: 0x%08x\000"
	.section	.text.qdma_reg_polling_timer,"ax",@progbits
	.align	2
	.globl	qdma_reg_polling_timer
	.set	nomips16
	.set	nomicromips
	.ent	qdma_reg_polling_timer
	.type	qdma_reg_polling_timer, @function
qdma_reg_polling_timer:
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
	lui	$2,%hi(reg_polling_offset)
	lw	$2,%lo(reg_polling_offset)($2)
	move	$3,$2
	lw	$2,16($sp)
	addu	$2,$3,$2
	sw	$2,24($sp)
	lw	$2,24($sp)
	lw	$2,0($2)
	sw	$2,20($sp)
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
	lui	$2,%hi(i.26484)
	lw	$2,%lo(i.26484)($2)
	addiu	$4,$2,1
	lui	$3,%hi(i.26484)
	sw	$4,%lo(i.26484)($3)
	lui	$4,%hi(qdmaRegValBuff)
	sll	$3,$2,2
	addiu	$2,$4,%lo(qdmaRegValBuff)
	addu	$2,$3,$2
	lw	$3,20($sp)
	sw	$3,0($2)
	lui	$2,%hi(reg_polling_round_cnt)
	lw	$2,%lo(reg_polling_round_cnt)($2)
	addiu	$3,$2,-1
	lui	$2,%hi(reg_polling_round_cnt)
	sw	$3,%lo(reg_polling_round_cnt)($2)
	lui	$2,%hi(reg_polling_round_cnt)
	lw	$2,%lo(reg_polling_round_cnt)($2)
	beq	$2,$0,$L317
	nop

	lui	$2,%hi(i.26484)
	lw	$3,%lo(i.26484)($2)
	li	$2,100			# 0x64
	bne	$3,$2,$L318
	nop

	j	$L319
	nop

$L320:
	lui	$2,%hi(i.26484)
	lw	$2,%lo(i.26484)($2)
	addiu	$3,$2,-1
	lui	$2,%hi(i.26484)
	sw	$3,%lo(i.26484)($2)
	lui	$2,%hi(reg_polling_round_cnt)
	lw	$3,%lo(reg_polling_round_cnt)($2)
	lui	$2,%hi(i.26484)
	lw	$2,%lo(i.26484)($2)
	addu	$5,$3,$2
	lui	$2,%hi(reg_polling_offset)
	lw	$2,%lo(reg_polling_offset)($2)
	move	$3,$2
	lw	$2,16($sp)
	addu	$6,$3,$2
	lui	$2,%hi(i.26484)
	lw	$3,%lo(i.26484)($2)
	lui	$2,%hi(qdmaRegValBuff)
	sll	$3,$3,2
	addiu	$2,$2,%lo(qdmaRegValBuff)
	addu	$2,$3,$2
	lw	$3,0($2)
	lui	$2,%hi($LC5)
	addiu	$4,$2,%lo($LC5)
	move	$7,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L319:
	lui	$2,%hi(i.26484)
	lw	$2,%lo(i.26484)($2)
	bgtz	$2,$L320
	nop

$L318:
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

	j	$L315
	nop

$L317:
	j	$L322
	nop

$L323:
	lui	$2,%hi(i.26484)
	lw	$2,%lo(i.26484)($2)
	addiu	$3,$2,-1
	lui	$2,%hi(i.26484)
	sw	$3,%lo(i.26484)($2)
	lui	$2,%hi(reg_polling_round_cnt)
	lw	$3,%lo(reg_polling_round_cnt)($2)
	lui	$2,%hi(i.26484)
	lw	$2,%lo(i.26484)($2)
	addu	$5,$3,$2
	lui	$2,%hi(reg_polling_offset)
	lw	$2,%lo(reg_polling_offset)($2)
	move	$3,$2
	lw	$2,16($sp)
	addu	$6,$3,$2
	lui	$2,%hi(i.26484)
	lw	$3,%lo(i.26484)($2)
	lui	$2,%hi(qdmaRegValBuff)
	sll	$3,$3,2
	addiu	$2,$2,%lo(qdmaRegValBuff)
	addu	$2,$3,$2
	lw	$3,0($2)
	lui	$2,%hi($LC5)
	addiu	$4,$2,%lo($LC5)
	move	$7,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L322:
	lui	$2,%hi(i.26484)
	lw	$2,%lo(i.26484)($2)
	bgtz	$2,$L323
	nop

$L315:
	lw	$31,36($sp)
	addiu	$sp,$sp,40
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	qdma_reg_polling_timer
	.size	qdma_reg_polling_timer, .-qdma_reg_polling_timer
	.section	.text.qdma_ioctl,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	qdma_ioctl
	.type	qdma_ioctl, @function
qdma_ioctl:
	.frame	$sp,112,$31		# vars= 88, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	addiu	$sp,$sp,-112
	sw	$31,108($sp)
	sw	$4,112($sp)
	sw	$5,116($sp)
	sw	$6,120($sp)
	sw	$0,24($sp)
	addiu	$2,$sp,76
	sw	$2,28($sp)
	lw	$2,120($sp)
	sw	$2,32($sp)
	li	$2,28			# 0x1c
	sw	$2,16($sp)
	lw	$2,32($sp)
	sw	$2,36($sp)
	lw	$2,16($sp)
	sw	$2,40($sp)
	move	$2,$28
	lw	$2,24($2)
	sw	$2,44($sp)
	lw	$3,36($sp)
	lw	$2,40($sp)
	addu	$3,$3,$2
	lw	$2,36($sp)
	or	$3,$3,$2
	lw	$2,40($sp)
	or	$3,$3,$2
	lw	$2,44($sp)
	and	$2,$3,$2
	sw	$2,48($sp)
	lw	$2,48($sp)
	sltu	$2,$2,1
	andi	$2,$2,0x00ff
	sltu	$2,$0,$2
	andi	$2,$2,0x00ff
	beq	$2,$0,$L325
	lw	$4,28($sp)
	lw	$5,32($sp)
	lw	$6,16($sp)
#APP
 # 1779 "/opt/tclinux_phoenix/modules/private/qdma/EN7512/qdma_dev.c" 1
	.set	noreorder
	.set	noat
	la	$1, __copy_user
	jalr	$1
	.set	at
	.set	noat
	addu	$1, $5, $6
	.set	at
	.set	reorder
 # 0 "" 2
#NO_APP
	move	$2,$6
	sw	$2,16($sp)
$L325:
	lw	$2,16($sp)
	beq	$2,$0,$L326
	li	$2,-14			# 0xfffffffffffffff2
	j	$L330
$L326:
	lw	$3,76($sp)
	lui	$2,%hi(qdma_operation)
	sll	$3,$3,2
	addiu	$2,$2,%lo(qdma_operation)
	addu	$2,$3,$2
	lw	$2,0($2)
	addiu	$3,$sp,76
	move	$4,$3
	jalr	$2
	sw	$2,24($sp)
	lw	$2,120($sp)
	sw	$2,52($sp)
	addiu	$2,$sp,76
	sw	$2,56($sp)
	li	$2,28			# 0x1c
	sw	$2,20($sp)
	lw	$2,52($sp)
	sw	$2,60($sp)
	lw	$2,20($sp)
	sw	$2,64($sp)
	move	$2,$28
	lw	$2,24($2)
	sw	$2,68($sp)
	lw	$3,60($sp)
	lw	$2,64($sp)
	addu	$3,$3,$2
	lw	$2,60($sp)
	or	$3,$3,$2
	lw	$2,64($sp)
	or	$3,$3,$2
	lw	$2,68($sp)
	and	$2,$3,$2
	sw	$2,72($sp)
	lw	$2,72($sp)
	sltu	$2,$2,1
	andi	$2,$2,0x00ff
	sltu	$2,$0,$2
	andi	$2,$2,0x00ff
	beq	$2,$0,$L328
	lw	$4,52($sp)
	lw	$5,56($sp)
	lw	$6,20($sp)
#APP
 # 1786 "/opt/tclinux_phoenix/modules/private/qdma/EN7512/qdma_dev.c" 1
	.set	noat
	la	$1, __copy_user
	jalr	$1
	.set	at
	
 # 0 "" 2
#NO_APP
	move	$2,$6
	sw	$2,20($sp)
$L328:
	lw	$2,20($sp)
	beq	$2,$0,$L329
	li	$2,-14			# 0xfffffffffffffff2
	j	$L330
$L329:
	lw	$2,24($sp)
$L330:
	lw	$31,108($sp)
	addiu	$sp,$sp,112
	j	$31
	.end	qdma_ioctl
	.size	qdma_ioctl, .-qdma_ioctl
	.globl	qdma_fops
	.data
	.align	2
	.type	qdma_fops, @object
	.size	qdma_fops, 100
qdma_fops:
	.word	__this_module
	.space	28
	.word	qdma_ioctl
	.space	64
	.section	.text.qdma_dev_destroy,"ax",@progbits
	.align	2
	.globl	qdma_dev_destroy
	.set	nomips16
	.set	nomicromips
	.ent	qdma_dev_destroy
	.type	qdma_dev_destroy, @function
qdma_dev_destroy:
	.frame	$sp,32,$31		# vars= 8, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-32
	sw	$31,28($sp)
	lui	$2,%hi(gpQdmaDev)
	lw	$2,%lo(gpQdmaDev)($2)
	lw	$2,0($2)
	beq	$2,$0,$L332
	nop

	lui	$2,%hi(gpQdmaDev)
	lw	$2,%lo(gpQdmaDev)($2)
	lw	$2,0($2)
	move	$4,$2
	lui	$2,%hi(cdev_del)
	addiu	$2,$2,%lo(cdev_del)
	jalr	$2
	nop

	lui	$2,%hi(gpQdmaDev)
	lw	$2,%lo(gpQdmaDev)($2)
	sw	$0,0($2)
$L332:
	li	$2,124780544			# 0x7700000
	sw	$2,16($sp)
	lw	$4,16($sp)
	li	$5,1			# 0x1
	lui	$2,%hi(unregister_chrdev_region)
	addiu	$2,$2,%lo(unregister_chrdev_region)
	jalr	$2
	nop

	lw	$31,28($sp)
	addiu	$sp,$sp,32
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	qdma_dev_destroy
	.size	qdma_dev_destroy, .-qdma_dev_destroy
	.rdata
	.align	2
$LC6:
	.ascii	"qdma_lan\000"
	.section	.text.qdma_dev_init,"ax",@progbits
	.align	2
	.globl	qdma_dev_init
	.set	nomips16
	.set	nomicromips
	.ent	qdma_dev_init
	.type	qdma_dev_init, @function
qdma_dev_init:
	.frame	$sp,456,$31		# vars= 432, regs= 2/0, args= 16, gp= 0
	.mask	0x80010000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-456
	sw	$31,452($sp)
	sw	$16,448($sp)
	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lw	$2,0($2)
	sw	$2,32($sp)
	sw	$0,16($sp)
	sw	$0,36($sp)
	sw	$0,20($sp)
	sw	$0,24($sp)
	sw	$0,28($sp)
	lui	$2,%hi(qdma_vip_info)
	addiu	$4,$2,%lo(qdma_vip_info)
	move	$5,$0
	li	$6,192			# 0xc0
	lui	$2,%hi(memset)
	addiu	$2,$2,%lo(memset)
	jalr	$2
	nop

	lw	$2,32($sp)
	addiu	$2,$2,80
	sw	$2,128($sp)
	li	$2,-1			# 0xffffffffffffffff
	sw	$2,132($sp)
	lw	$2,128($sp)
	lw	$3,132($sp)
	sw	$3,0($2)
	li	$2,64187			# 0xfabb
	sw	$2,36($sp)
	lw	$4,32($sp)
	lw	$5,36($sp)
	lui	$2,%hi(qdmaSetIntMask)
	addiu	$2,$2,%lo(qdmaSetIntMask)
	jalr	$2
	nop

	lw	$3,16($sp)
	li	$2,469762048			# 0x1c000000
	ori	$2,$2,0x30
	or	$2,$3,$2
	sw	$2,16($sp)
	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lhu	$2,30($2)
	beq	$2,$0,$L334
	nop

	lw	$3,16($sp)
	li	$2,524288			# 0x80000
	or	$2,$3,$2
	sw	$2,16($sp)
$L334:
	lw	$3,16($sp)
	li	$2,-2147483648			# 0xffffffff80000000
	or	$2,$3,$2
	sw	$2,16($sp)
	lw	$2,16($sp)
	ori	$2,$2,0x40
	sw	$2,16($sp)
	lw	$3,16($sp)
	li	$2,1048576			# 0x100000
	or	$2,$3,$2
	sw	$2,16($sp)
	lw	$2,32($sp)
	addiu	$2,$2,4
	sw	$2,136($sp)
	lw	$2,16($sp)
	sw	$2,140($sp)
	lw	$2,136($sp)
	lw	$3,140($sp)
	sw	$3,0($2)
	lw	$2,32($sp)
	addiu	$2,$2,128
	sw	$2,144($sp)
	lw	$2,144($sp)
	lw	$2,0($2)
	sw	$2,40($sp)
	lw	$2,32($sp)
	addiu	$3,$2,128
	lw	$4,40($sp)
	li	$2,-2147483648			# 0xffffffff80000000
	or	$2,$4,$2
	sw	$3,148($sp)
	sw	$2,152($sp)
	lw	$2,148($sp)
	lw	$3,152($sp)
	sw	$3,0($2)
	lw	$2,32($sp)
	addiu	$2,$2,128
	sw	$2,156($sp)
	lw	$2,156($sp)
	lw	$2,0($2)
	sw	$2,44($sp)
	lw	$2,32($sp)
	addiu	$3,$2,128
	lw	$2,44($sp)
	ori	$2,$2,0x8
	sw	$3,160($sp)
	sw	$2,164($sp)
	lw	$2,160($sp)
	lw	$3,164($sp)
	sw	$3,0($2)
	lw	$2,32($sp)
	addiu	$2,$2,144
	sw	$2,168($sp)
	lw	$2,168($sp)
	lw	$2,0($2)
	sw	$2,48($sp)
	lw	$2,32($sp)
	addiu	$3,$2,144
	lw	$4,48($sp)
	li	$2,-536936448			# 0xffffffffdfff0000
	ori	$2,$2,0xffff
	and	$2,$4,$2
	sw	$3,172($sp)
	sw	$2,176($sp)
	lw	$2,172($sp)
	lw	$3,176($sp)
	sw	$3,0($2)
	lw	$2,32($sp)
	addiu	$2,$2,148
	sw	$2,180($sp)
	lw	$2,180($sp)
	lw	$2,0($2)
	sw	$2,52($sp)
	lw	$2,32($sp)
	addiu	$3,$2,148
	lw	$4,52($sp)
	li	$2,-2147483648			# 0xffffffff80000000
	or	$2,$4,$2
	sw	$3,184($sp)
	sw	$2,188($sp)
	lw	$2,184($sp)
	lw	$3,188($sp)
	sw	$3,0($2)
	lw	$2,32($sp)
	addiu	$2,$2,148
	sw	$2,192($sp)
	lw	$2,192($sp)
	lw	$2,0($2)
	sw	$2,56($sp)
	lw	$2,32($sp)
	addiu	$3,$2,148
	lw	$4,56($sp)
	li	$2,-262144			# 0xfffffffffffc0000
	ori	$2,$2,0xffff
	and	$4,$4,$2
	li	$2,131072			# 0x20000
	or	$2,$4,$2
	sw	$3,196($sp)
	sw	$2,200($sp)
	lw	$2,196($sp)
	lw	$3,200($sp)
	sw	$3,0($2)
	lw	$2,32($sp)
	addiu	$2,$2,148
	sw	$2,204($sp)
	lw	$2,204($sp)
	lw	$2,0($2)
	sw	$2,60($sp)
	lw	$2,32($sp)
	addiu	$3,$2,148
	lw	$4,60($sp)
	li	$2,-65536			# 0xffffffffffff0000
	and	$2,$4,$2
	ori	$2,$2,0xfa0
	sw	$3,208($sp)
	sw	$2,212($sp)
	lw	$2,208($sp)
	lw	$3,212($sp)
	sw	$3,0($2)
	lw	$2,32($sp)
	addiu	$2,$2,152
	sw	$2,216($sp)
	lw	$2,216($sp)
	lw	$2,0($2)
	sw	$2,64($sp)
	lw	$2,32($sp)
	addiu	$3,$2,152
	lw	$4,64($sp)
	li	$2,-1048576			# 0xfffffffffff00000
	ori	$2,$2,0xffff
	and	$4,$4,$2
	li	$2,65536			# 0x10000
	or	$2,$4,$2
	sw	$3,220($sp)
	sw	$2,224($sp)
	lw	$2,220($sp)
	lw	$3,224($sp)
	sw	$3,0($2)
	lw	$2,32($sp)
	addiu	$2,$2,152
	sw	$2,228($sp)
	lw	$2,228($sp)
	lw	$2,0($2)
	sw	$2,68($sp)
	lw	$2,32($sp)
	addiu	$3,$2,152
	lw	$4,68($sp)
	li	$2,-65536			# 0xffffffffffff0000
	and	$2,$4,$2
	ori	$2,$2,0x3e8
	sw	$3,232($sp)
	sw	$2,236($sp)
	lw	$2,232($sp)
	lw	$3,236($sp)
	sw	$3,0($2)
	lw	$2,32($sp)
	addiu	$2,$2,160
	sw	$2,240($sp)
	lw	$2,240($sp)
	lw	$2,0($2)
	sw	$2,72($sp)
	lw	$2,32($sp)
	addiu	$3,$2,160
	lw	$4,72($sp)
	li	$2,-134283264			# 0xfffffffff7ff0000
	ori	$2,$2,0xffff
	and	$2,$4,$2
	sw	$3,244($sp)
	sw	$2,248($sp)
	lw	$2,244($sp)
	lw	$3,248($sp)
	sw	$3,0($2)
	lw	$2,32($sp)
	addiu	$2,$2,160
	sw	$2,252($sp)
	lw	$2,252($sp)
	lw	$2,0($2)
	sw	$2,76($sp)
	lw	$2,32($sp)
	addiu	$3,$2,160
	lw	$4,76($sp)
	li	$2,-67174400			# 0xfffffffffbff0000
	ori	$2,$2,0xffff
	and	$2,$4,$2
	sw	$3,256($sp)
	sw	$2,260($sp)
	lw	$2,256($sp)
	lw	$3,260($sp)
	sw	$3,0($2)
	lw	$2,32($sp)
	addiu	$2,$2,160
	sw	$2,264($sp)
	lw	$2,264($sp)
	lw	$2,0($2)
	sw	$2,80($sp)
	lw	$2,32($sp)
	addiu	$3,$2,160
	lw	$4,80($sp)
	li	$2,-33619968			# 0xfffffffffdff0000
	ori	$2,$2,0xffff
	and	$2,$4,$2
	sw	$3,268($sp)
	sw	$2,272($sp)
	lw	$2,268($sp)
	lw	$3,272($sp)
	sw	$3,0($2)
	lw	$2,32($sp)
	addiu	$2,$2,160
	sw	$2,276($sp)
	lw	$2,276($sp)
	lw	$2,0($2)
	sw	$2,84($sp)
	lw	$2,32($sp)
	addiu	$3,$2,160
	lw	$4,84($sp)
	li	$2,-16842752			# 0xfffffffffeff0000
	ori	$2,$2,0xffff
	and	$2,$4,$2
	sw	$3,280($sp)
	sw	$2,284($sp)
	lw	$2,280($sp)
	lw	$3,284($sp)
	sw	$3,0($2)
	sw	$0,432($sp)
	addiu	$2,$sp,432
	sw	$2,412($sp)
	addiu	$2,$sp,404
	move	$4,$2
	lui	$2,%hi(qdma_set_txq_cngst_auto_config)
	addiu	$2,$2,%lo(qdma_set_txq_cngst_auto_config)
	jalr	$2
	nop

	lw	$2,32($sp)
	addiu	$2,$2,160
	sw	$2,288($sp)
	lw	$2,288($sp)
	lw	$2,0($2)
	sw	$2,88($sp)
	lw	$2,32($sp)
	addiu	$3,$2,160
	lw	$4,88($sp)
	li	$2,262144			# 0x40000
	or	$2,$4,$2
	sw	$3,292($sp)
	sw	$2,296($sp)
	lw	$2,292($sp)
	lw	$3,296($sp)
	sw	$3,0($2)
	lw	$2,32($sp)
	addiu	$2,$2,160
	sw	$2,300($sp)
	lw	$2,300($sp)
	lw	$2,0($2)
	sw	$2,92($sp)
	lw	$2,32($sp)
	addiu	$3,$2,160
	lw	$4,92($sp)
	li	$2,131072			# 0x20000
	or	$2,$4,$2
	sw	$3,304($sp)
	sw	$2,308($sp)
	lw	$2,304($sp)
	lw	$3,308($sp)
	sw	$3,0($2)
	lw	$2,32($sp)
	addiu	$2,$2,160
	sw	$2,312($sp)
	lw	$2,312($sp)
	lw	$2,0($2)
	sw	$2,96($sp)
	lw	$2,32($sp)
	addiu	$3,$2,160
	lw	$4,96($sp)
	li	$2,65536			# 0x10000
	or	$2,$4,$2
	sw	$3,316($sp)
	sw	$2,320($sp)
	lw	$2,316($sp)
	lw	$3,320($sp)
	sw	$3,0($2)
	lw	$2,32($sp)
	addiu	$2,$2,160
	sw	$2,324($sp)
	lw	$2,324($sp)
	lw	$2,0($2)
	sw	$2,100($sp)
	lw	$2,32($sp)
	addiu	$3,$2,160
	lw	$4,100($sp)
	li	$2,-65536			# 0xffffffffffff0000
	and	$2,$4,$2
	ori	$2,$2,0xfa
	sw	$3,328($sp)
	sw	$2,332($sp)
	lw	$2,328($sp)
	lw	$3,332($sp)
	sw	$3,0($2)
	lw	$2,32($sp)
	addiu	$2,$2,160
	sw	$2,336($sp)
	lw	$2,336($sp)
	lw	$2,0($2)
	sw	$2,104($sp)
	lw	$2,32($sp)
	addiu	$3,$2,160
	lw	$4,104($sp)
	li	$2,-3211264			# 0xffffffffffcf0000
	ori	$2,$2,0xffff
	and	$2,$4,$2
	sw	$3,340($sp)
	sw	$2,344($sp)
	lw	$2,340($sp)
	lw	$3,344($sp)
	sw	$3,0($2)
	lw	$2,32($sp)
	addiu	$2,$2,152
	sw	$2,348($sp)
	lw	$2,348($sp)
	lw	$2,0($2)
	sw	$2,108($sp)
	lw	$2,32($sp)
	addiu	$3,$2,152
	lw	$4,108($sp)
	li	$2,-2147483648			# 0xffffffff80000000
	or	$2,$4,$2
	sw	$3,352($sp)
	sw	$2,356($sp)
	lw	$2,352($sp)
	lw	$3,356($sp)
	sw	$3,0($2)
	lw	$2,32($sp)
	addiu	$2,$2,288
	sw	$2,360($sp)
	lw	$2,360($sp)
	lw	$2,0($2)
	sw	$2,112($sp)
	lw	$2,32($sp)
	addiu	$3,$2,288
	lw	$4,112($sp)
	li	$2,-251723776			# 0xfffffffff0ff0000
	ori	$2,$2,0xffff
	and	$4,$4,$2
	li	$2,16777216			# 0x1000000
	or	$2,$4,$2
	sw	$3,364($sp)
	sw	$2,368($sp)
	lw	$2,364($sp)
	lw	$3,368($sp)
	sw	$3,0($2)
	lw	$2,32($sp)
	addiu	$2,$2,288
	sw	$2,372($sp)
	lw	$2,372($sp)
	lw	$2,0($2)
	sw	$2,116($sp)
	lw	$2,32($sp)
	addiu	$3,$2,288
	lw	$4,116($sp)
	li	$2,536870912			# 0x20000000
	or	$2,$4,$2
	sw	$3,376($sp)
	sw	$2,380($sp)
	lw	$2,376($sp)
	lw	$3,380($sp)
	sw	$3,0($2)
	lw	$2,32($sp)
	addiu	$2,$2,288
	sw	$2,384($sp)
	lw	$2,384($sp)
	lw	$2,0($2)
	sw	$2,120($sp)
	lw	$2,32($sp)
	addiu	$3,$2,288
	lw	$4,120($sp)
	li	$2,-65536			# 0xffffffffffff0000
	and	$2,$4,$2
	ori	$2,$2,0xf424
	sw	$3,388($sp)
	sw	$2,392($sp)
	lw	$2,388($sp)
	lw	$3,392($sp)
	sw	$3,0($2)
	lui	$2,%hi(qdma_set_dbg_cntr_default_config)
	addiu	$2,$2,%lo(qdma_set_dbg_cntr_default_config)
	jalr	$2
	nop

	lui	$2,%hi(channel_limit_threshold)
	li	$3,65536			# 0x10000
	sw	$3,%lo(channel_limit_threshold)($2)
	lui	$2,%hi(queue_limit_threshold)
	li	$3,65536			# 0x10000
	sw	$3,%lo(queue_limit_threshold)($2)
	sw	$0,20($sp)
	j	$L356
	nop

$L359:
	lui	$2,%hi(chnlLimit)
	lw	$3,20($sp)
	sll	$3,$3,2
	addiu	$2,$2,%lo(chnlLimit)
	addu	$2,$3,$2
	sw	$0,0($2)
	sw	$0,24($sp)
	j	$L357
	nop

$L358:
	lui	$2,%hi(queueLimit)
	lw	$3,20($sp)
	sll	$4,$3,3
	lw	$3,24($sp)
	addu	$3,$4,$3
	sll	$3,$3,2
	addiu	$2,$2,%lo(queueLimit)
	addu	$2,$3,$2
	sw	$0,0($2)
	lw	$2,24($sp)
	addiu	$2,$2,1
	sw	$2,24($sp)
$L357:
	lw	$2,24($sp)
	sltu	$2,$2,8
	bne	$2,$0,$L358
	nop

	lw	$2,20($sp)
	addiu	$2,$2,1
	sw	$2,20($sp)
$L356:
	lw	$2,20($sp)
	sltu	$2,$2,8
	bne	$2,$0,$L359
	nop

	lw	$2,32($sp)
	addiu	$2,$2,92
	sw	$2,396($sp)
	sw	$0,400($sp)
	lw	$2,396($sp)
	lw	$3,400($sp)
	sw	$3,0($2)
	li	$2,124780544			# 0x7700000
	sw	$2,124($sp)
	lw	$4,124($sp)
	li	$5,1			# 0x1
	lui	$2,%hi($LC6)
	addiu	$6,$2,%lo($LC6)
	lui	$2,%hi(register_chrdev_region)
	addiu	$2,$2,%lo(register_chrdev_region)
	jalr	$2
	nop

	sw	$2,28($sp)
	lw	$2,28($sp)
	bgez	$2,$L360
	nop

	lw	$2,28($sp)
	j	$L365
	nop

$L360:
	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	addiu	$3,$2,332
	lui	$2,%hi(gpQdmaDev)
	sw	$3,%lo(gpQdmaDev)($2)
	lui	$2,%hi(gpQdmaDev)
	lw	$16,%lo(gpQdmaDev)($2)
	lui	$2,%hi(cdev_alloc)
	addiu	$2,$2,%lo(cdev_alloc)
	jalr	$2
	nop

	sw	$2,0($16)
	lui	$2,%hi(gpQdmaDev)
	lw	$2,%lo(gpQdmaDev)($2)
	lw	$2,0($2)
	bne	$2,$0,$L362
	nop

	li	$2,-12			# 0xfffffffffffffff4
	sw	$2,28($sp)
	j	$L363
	nop

$L362:
	lui	$2,%hi(gpQdmaDev)
	lw	$2,%lo(gpQdmaDev)($2)
	lw	$2,0($2)
	move	$4,$2
	lui	$2,%hi(qdma_fops)
	addiu	$5,$2,%lo(qdma_fops)
	lui	$2,%hi(cdev_init)
	addiu	$2,$2,%lo(cdev_init)
	jalr	$2
	nop

	lui	$2,%hi(gpQdmaDev)
	lw	$2,%lo(gpQdmaDev)($2)
	lw	$2,0($2)
	lui	$3,%hi(__this_module)
	addiu	$3,$3,%lo(__this_module)
	sw	$3,36($2)
	lui	$2,%hi(gpQdmaDev)
	lw	$2,%lo(gpQdmaDev)($2)
	lw	$2,0($2)
	lui	$3,%hi(qdma_fops)
	addiu	$3,$3,%lo(qdma_fops)
	sw	$3,40($2)
	lui	$2,%hi(gpQdmaDev)
	lw	$2,%lo(gpQdmaDev)($2)
	lw	$2,0($2)
	move	$4,$2
	lw	$5,124($sp)
	li	$6,1			# 0x1
	lui	$2,%hi(cdev_add)
	addiu	$2,$2,%lo(cdev_add)
	jalr	$2
	nop

	sw	$2,28($sp)
	lw	$2,28($sp)
	bgez	$2,$L364
	nop

	j	$L363
	nop

$L364:
	move	$2,$0
	j	$L365
	nop

$L363:
	lui	$2,%hi(qdma_dev_destroy)
	addiu	$2,$2,%lo(qdma_dev_destroy)
	jalr	$2
	nop

	lw	$2,28($sp)
$L365:
	lw	$31,452($sp)
	lw	$16,448($sp)
	addiu	$sp,$sp,456
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	qdma_dev_init
	.size	qdma_dev_init, .-qdma_dev_init
	.local	i.26484
	.comm	i.26484,4,4
	.ident	"GCC: (Buildroot 2015.08.1) 4.9.3"
