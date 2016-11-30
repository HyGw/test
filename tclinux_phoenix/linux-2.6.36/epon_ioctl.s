	.file	1 "epon_ioctl.c"
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

	.rdata
	.align	2
$LC0:
	.ascii	"\015\012eponSetLlidKey param error\000"
	.align	2
$LC1:
	.ascii	"eponSetLlidKey keyValue = 0x%x\000"
	.align	2
$LC2:
	.ascii	"eponSetLlidKey eponSecurityKeyCfgReg = 0x%x\000"
	.align	2
$LC3:
	.ascii	"\015\012 eponSetLlidKey set  timers == 100\000"
#NO_APP
	.section	.text.eponSetLlidKey,"ax",@progbits
	.align	2
	.globl	eponSetLlidKey
	.set	nomips16
	.set	nomicromips
	.ent	eponSetLlidKey
	.type	eponSetLlidKey, @function
eponSetLlidKey:
	.frame	$sp,40,$31		# vars= 16, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-40
	sw	$31,36($sp)
	move	$3,$4
	move	$2,$5
	sw	$6,48($sp)
	sb	$3,40($sp)
	sb	$2,44($sp)
	sw	$0,28($sp)
	sw	$0,16($sp)
	sw	$0,20($sp)
	addiu	$2,$sp,24
	move	$4,$2
	move	$5,$0
	li	$6,4			# 0x4
	lui	$2,%hi(memset)
	addiu	$2,$2,%lo(memset)
	jalr	$2
	nop

	lbu	$2,40($sp)
	sltu	$2,$2,8
	beq	$2,$0,$L2
	nop

	lbu	$2,44($sp)
	sltu	$2,$2,2
	beq	$2,$0,$L2
	nop

	lw	$2,48($sp)
	bne	$2,$0,$L3
	nop

$L2:
	li	$4,1			# 0x1
	lui	$2,%hi($LC0)
	addiu	$5,$2,%lo($LC0)
	lui	$2,%hi(eponDbgPrint)
	addiu	$2,$2,%lo(eponDbgPrint)
	jalr	$2
	nop

	li	$2,-1			# 0xffffffffffffffff
	j	$L10
	nop

$L3:
	addiu	$2,$sp,28
	sw	$2,20($sp)
	lw	$2,20($sp)
	addiu	$2,$2,1
	sw	$2,20($sp)
	lw	$4,20($sp)
	lw	$5,48($sp)
	li	$6,3			# 0x3
	lui	$2,%hi(memcpy)
	addiu	$2,$2,%lo(memcpy)
	jalr	$2
	nop

	lw	$3,28($sp)
	li	$4,4			# 0x4
	lui	$2,%hi($LC1)
	addiu	$5,$2,%lo($LC1)
	move	$6,$3
	lui	$2,%hi(eponDbgPrint)
	addiu	$2,$2,%lo(eponDbgPrint)
	jalr	$2
	nop

	lw	$3,28($sp)
	lui	$2,%hi(g_EPON_MAC_BASE)
	lw	$2,%lo(g_EPON_MAC_BASE)($2)
	addiu	$2,$2,24668
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	lw	$2,24($sp)
	li	$3,1			# 0x1
	ins	$2,$3,31,1
	sw	$2,24($sp)
	lbu	$2,40($sp)
	andi	$2,$2,0x7
	andi	$3,$2,0x00ff
	lw	$2,24($sp)
	ins	$2,$3,3,3
	sw	$2,24($sp)
	lbu	$2,44($sp)
	andi	$2,$2,0x1
	andi	$3,$2,0x00ff
	lw	$2,24($sp)
	ins	$2,$3,2,1
	sw	$2,24($sp)
	lw	$3,24($sp)
	li	$4,4			# 0x4
	lui	$2,%hi($LC2)
	addiu	$5,$2,%lo($LC2)
	move	$6,$3
	lui	$2,%hi(eponDbgPrint)
	addiu	$2,$2,%lo(eponDbgPrint)
	jalr	$2
	nop

	lw	$3,24($sp)
	lui	$2,%hi(g_EPON_MAC_BASE)
	lw	$2,%lo(g_EPON_MAC_BASE)($2)
	addiu	$2,$2,24664
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	sw	$0,16($sp)
	j	$L5
	nop

$L8:
	lui	$2,%hi(g_EPON_MAC_BASE)
	lw	$2,%lo(g_EPON_MAC_BASE)($2)
	addiu	$2,$2,24664
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	sw	$2,24($sp)
	lw	$3,24($sp)
	li	$2,65536			# 0x10000
	and	$2,$3,$2
	beq	$2,$0,$L6
	nop

	j	$L7
	nop

$L6:
	lw	$2,16($sp)
	addiu	$2,$2,1
	sw	$2,16($sp)
$L5:
	lw	$2,16($sp)
	sltu	$2,$2,100
	bne	$2,$0,$L8
	nop

$L7:
	lw	$3,16($sp)
	li	$2,100			# 0x64
	bne	$3,$2,$L9
	nop

	li	$4,1			# 0x1
	lui	$2,%hi($LC3)
	addiu	$5,$2,%lo($LC3)
	lui	$2,%hi(eponDbgPrint)
	addiu	$2,$2,%lo(eponDbgPrint)
	jalr	$2
	nop

	li	$2,-1			# 0xffffffffffffffff
	j	$L10
	nop

$L9:
	move	$2,$0
$L10:
	lw	$31,36($sp)
	addiu	$sp,$sp,40
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	eponSetLlidKey
	.size	eponSetLlidKey, .-eponSetLlidKey
	.rdata
	.align	2
$LC4:
	.ascii	"\015\012 eponSetLlidKey get  timers == 100\000"
	.align	2
$LC5:
	.ascii	"eponSetLlidKey key=0x%x\000"
	.section	.text.eponGetLlidKey,"ax",@progbits
	.align	2
	.globl	eponGetLlidKey
	.set	nomips16
	.set	nomicromips
	.ent	eponGetLlidKey
	.type	eponGetLlidKey, @function
eponGetLlidKey:
	.frame	$sp,40,$31		# vars= 16, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-40
	sw	$31,36($sp)
	move	$3,$4
	move	$2,$5
	sw	$6,48($sp)
	sb	$3,40($sp)
	sb	$2,44($sp)
	sw	$0,28($sp)
	sw	$0,16($sp)
	sw	$0,20($sp)
	addiu	$2,$sp,24
	move	$4,$2
	move	$5,$0
	li	$6,4			# 0x4
	lui	$2,%hi(memset)
	addiu	$2,$2,%lo(memset)
	jalr	$2
	nop

	lbu	$2,40($sp)
	sltu	$2,$2,8
	beq	$2,$0,$L12
	nop

	lbu	$2,44($sp)
	sltu	$2,$2,2
	beq	$2,$0,$L12
	nop

	lw	$2,48($sp)
	bne	$2,$0,$L13
	nop

$L12:
	li	$4,1			# 0x1
	lui	$2,%hi($LC0)
	addiu	$5,$2,%lo($LC0)
	lui	$2,%hi(eponDbgPrint)
	addiu	$2,$2,%lo(eponDbgPrint)
	jalr	$2
	nop

	li	$2,-1			# 0xffffffffffffffff
	j	$L20
	nop

$L13:
	lw	$2,24($sp)
	ins	$2,$0,31,1
	sw	$2,24($sp)
	lbu	$2,40($sp)
	andi	$2,$2,0x7
	andi	$3,$2,0x00ff
	lw	$2,24($sp)
	ins	$2,$3,3,3
	sw	$2,24($sp)
	lbu	$2,44($sp)
	andi	$2,$2,0x1
	andi	$3,$2,0x00ff
	lw	$2,24($sp)
	ins	$2,$3,2,1
	sw	$2,24($sp)
	lw	$3,24($sp)
	lui	$2,%hi(g_EPON_MAC_BASE)
	lw	$2,%lo(g_EPON_MAC_BASE)($2)
	addiu	$2,$2,24664
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	sw	$0,16($sp)
	j	$L15
	nop

$L18:
	lui	$2,%hi(g_EPON_MAC_BASE)
	lw	$2,%lo(g_EPON_MAC_BASE)($2)
	addiu	$2,$2,24664
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	sw	$2,24($sp)
	lw	$3,24($sp)
	li	$2,65536			# 0x10000
	and	$2,$3,$2
	beq	$2,$0,$L16
	nop

	j	$L17
	nop

$L16:
	lw	$2,16($sp)
	addiu	$2,$2,1
	sw	$2,16($sp)
$L15:
	lw	$2,16($sp)
	sltu	$2,$2,100
	bne	$2,$0,$L18
	nop

$L17:
	lw	$3,16($sp)
	li	$2,100			# 0x64
	bne	$3,$2,$L19
	nop

	li	$4,1			# 0x1
	lui	$2,%hi($LC4)
	addiu	$5,$2,%lo($LC4)
	lui	$2,%hi(eponDbgPrint)
	addiu	$2,$2,%lo(eponDbgPrint)
	jalr	$2
	nop

	li	$2,-1			# 0xffffffffffffffff
	j	$L20
	nop

$L19:
	lui	$2,%hi(g_EPON_MAC_BASE)
	lw	$2,%lo(g_EPON_MAC_BASE)($2)
	addiu	$2,$2,24668
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	sw	$2,28($sp)
	addiu	$2,$sp,28
	sw	$2,20($sp)
	lw	$2,20($sp)
	addiu	$2,$2,1
	sw	$2,20($sp)
	lw	$4,48($sp)
	lw	$5,20($sp)
	li	$6,3			# 0x3
	lui	$2,%hi(memcpy)
	addiu	$2,$2,%lo(memcpy)
	jalr	$2
	nop

	lw	$3,28($sp)
	li	$4,4			# 0x4
	lui	$2,%hi($LC5)
	addiu	$5,$2,%lo($LC5)
	move	$6,$3
	lui	$2,%hi(eponDbgPrint)
	addiu	$2,$2,%lo(eponDbgPrint)
	jalr	$2
	nop

	move	$2,$0
$L20:
	lw	$31,36($sp)
	addiu	$sp,$sp,40
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	eponGetLlidKey
	.size	eponGetLlidKey, .-eponGetLlidKey
	.rdata
	.align	2
$LC6:
	.ascii	"\015\012enter eponSetLlidFec\000"
	.align	2
$LC7:
	.ascii	"\015\012eponSetLlidFec param error\000"
	.section	.text.eponSetLlidFec,"ax",@progbits
	.align	2
	.globl	eponSetLlidFec
	.set	nomips16
	.set	nomicromips
	.ent	eponSetLlidFec
	.type	eponSetLlidFec, @function
eponSetLlidFec:
	.frame	$sp,40,$31		# vars= 16, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-40
	sw	$31,36($sp)
	move	$3,$4
	move	$2,$5
	sb	$3,40($sp)
	sb	$2,44($sp)
	sw	$0,16($sp)
	sw	$0,20($sp)
	sb	$0,24($sp)
	li	$4,4			# 0x4
	lui	$2,%hi($LC6)
	addiu	$5,$2,%lo($LC6)
	lui	$2,%hi(eponDbgPrint)
	addiu	$2,$2,%lo(eponDbgPrint)
	jalr	$2
	nop

	lbu	$2,40($sp)
	sltu	$2,$2,8
	bne	$2,$0,$L22
	nop

	li	$4,1			# 0x1
	lui	$2,%hi($LC7)
	addiu	$5,$2,%lo($LC7)
	lui	$2,%hi(eponDbgPrint)
	addiu	$2,$2,%lo(eponDbgPrint)
	jalr	$2
	nop

	li	$2,-1			# 0xffffffffffffffff
	j	$L23
	nop

$L22:
	lbu	$2,40($sp)
	sltu	$2,$2,4
	beq	$2,$0,$L24
	nop

	lui	$2,%hi(g_EPON_MAC_BASE)
	lw	$2,%lo(g_EPON_MAC_BASE)($2)
	addiu	$2,$2,24608
	sw	$2,16($sp)
	lbu	$2,40($sp)
	sll	$2,$2,3
	andi	$2,$2,0x00ff
	addiu	$2,$2,3
	sb	$2,24($sp)
	j	$L25
	nop

$L24:
	lui	$2,%hi(g_EPON_MAC_BASE)
	lw	$2,%lo(g_EPON_MAC_BASE)($2)
	addiu	$2,$2,24612
	sw	$2,16($sp)
	lbu	$2,40($sp)
	addiu	$2,$2,-4
	andi	$2,$2,0x00ff
	sll	$2,$2,3
	andi	$2,$2,0x00ff
	addiu	$2,$2,3
	sb	$2,24($sp)
$L25:
	lw	$2,16($sp)
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	sw	$2,20($sp)
	lbu	$3,44($sp)
	li	$2,1			# 0x1
	bne	$3,$2,$L26
	nop

	lbu	$2,24($sp)
	li	$3,1			# 0x1
	sll	$2,$3,$2
	move	$3,$2
	lw	$2,20($sp)
	or	$2,$2,$3
	sw	$2,20($sp)
	j	$L27
	nop

$L26:
	lbu	$2,24($sp)
	li	$3,1			# 0x1
	sll	$2,$3,$2
	nor	$2,$0,$2
	move	$3,$2
	lw	$2,20($sp)
	and	$2,$2,$3
	sw	$2,20($sp)
$L27:
	lw	$2,16($sp)
	lw	$4,20($sp)
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	move	$2,$0
$L23:
	lw	$31,36($sp)
	addiu	$sp,$sp,40
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	eponSetLlidFec
	.size	eponSetLlidFec, .-eponSetLlidFec
	.section	.text.eponGetLlidFec,"ax",@progbits
	.align	2
	.globl	eponGetLlidFec
	.set	nomips16
	.set	nomicromips
	.ent	eponGetLlidFec
	.type	eponGetLlidFec, @function
eponGetLlidFec:
	.frame	$sp,40,$31		# vars= 16, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-40
	sw	$31,36($sp)
	move	$2,$4
	sw	$5,44($sp)
	sb	$2,40($sp)
	sw	$0,16($sp)
	sw	$0,24($sp)
	sb	$0,20($sp)
	lbu	$2,40($sp)
	sltu	$2,$2,8
	bne	$2,$0,$L29
	nop

	li	$4,1			# 0x1
	lui	$2,%hi($LC7)
	addiu	$5,$2,%lo($LC7)
	lui	$2,%hi(eponDbgPrint)
	addiu	$2,$2,%lo(eponDbgPrint)
	jalr	$2
	nop

	li	$2,-1			# 0xffffffffffffffff
	j	$L30
	nop

$L29:
	lbu	$2,40($sp)
	sltu	$2,$2,4
	beq	$2,$0,$L31
	nop

	lui	$2,%hi(g_EPON_MAC_BASE)
	lw	$2,%lo(g_EPON_MAC_BASE)($2)
	addiu	$2,$2,24608
	sw	$2,16($sp)
	lbu	$2,40($sp)
	sll	$2,$2,3
	andi	$2,$2,0x00ff
	addiu	$2,$2,3
	sb	$2,20($sp)
	j	$L32
	nop

$L31:
	lui	$2,%hi(g_EPON_MAC_BASE)
	lw	$2,%lo(g_EPON_MAC_BASE)($2)
	addiu	$2,$2,24612
	sw	$2,16($sp)
	lbu	$2,40($sp)
	addiu	$2,$2,-4
	andi	$2,$2,0x00ff
	sll	$2,$2,3
	andi	$2,$2,0x00ff
	addiu	$2,$2,3
	sb	$2,20($sp)
$L32:
	lw	$2,16($sp)
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	sw	$2,24($sp)
	lbu	$2,20($sp)
	li	$3,1			# 0x1
	sll	$2,$3,$2
	move	$3,$2
	lw	$2,24($sp)
	and	$2,$3,$2
	beq	$2,$0,$L33
	nop

	lw	$2,44($sp)
	li	$3,1			# 0x1
	sw	$3,0($2)
	j	$L34
	nop

$L33:
	lw	$2,44($sp)
	sw	$0,0($2)
$L34:
	move	$2,$0
$L30:
	lw	$31,36($sp)
	addiu	$sp,$sp,40
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	eponGetLlidFec
	.size	eponGetLlidFec, .-eponGetLlidFec
	.rdata
	.align	2
$LC8:
	.ascii	"\015\012enter eponSetLlidThrshldNum llidIndex=%d ,num=%d"
	.ascii	" \000"
	.align	2
$LC9:
	.ascii	"\015\012eponSetLlidThrshldNum param error\000"
	.align	2
$LC10:
	.ascii	"eponSetLlidThrshldNum eponRptCfgReg.Raw=%x\000"
	.section	.text.eponSetLlidThrshldNum,"ax",@progbits
	.align	2
	.globl	eponSetLlidThrshldNum
	.set	nomips16
	.set	nomicromips
	.ent	eponSetLlidThrshldNum
	.type	eponSetLlidThrshldNum, @function
eponSetLlidThrshldNum:
	.frame	$sp,40,$31		# vars= 16, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-40
	sw	$31,36($sp)
	move	$3,$4
	move	$2,$5
	sb	$3,40($sp)
	sb	$2,44($sp)
	lui	$2,%hi(g_EPON_MAC_BASE)
	lw	$2,%lo(g_EPON_MAC_BASE)($2)
	addiu	$2,$2,24680
	sw	$2,16($sp)
	sw	$0,20($sp)
	sw	$0,24($sp)
	lbu	$6,40($sp)
	lbu	$3,44($sp)
	li	$4,4			# 0x4
	lui	$2,%hi($LC8)
	addiu	$5,$2,%lo($LC8)
	move	$7,$3
	lui	$2,%hi(eponDbgPrint)
	addiu	$2,$2,%lo(eponDbgPrint)
	jalr	$2
	nop

	lbu	$2,40($sp)
	sltu	$2,$2,8
	beq	$2,$0,$L36
	nop

	lbu	$2,44($sp)
	sltu	$2,$2,4
	bne	$2,$0,$L37
	nop

$L36:
	li	$4,1			# 0x1
	lui	$2,%hi($LC9)
	addiu	$5,$2,%lo($LC9)
	lui	$2,%hi(eponDbgPrint)
	addiu	$2,$2,%lo(eponDbgPrint)
	jalr	$2
	nop

	li	$2,-1			# 0xffffffffffffffff
	j	$L39
	nop

$L37:
	lbu	$2,44($sp)
	sw	$2,20($sp)
	lbu	$2,40($sp)
	sll	$2,$2,1
	lw	$3,20($sp)
	sll	$2,$3,$2
	sw	$2,20($sp)
	lw	$2,16($sp)
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	sw	$2,24($sp)
	lw	$2,24($sp)
	lbu	$3,40($sp)
	sll	$3,$3,1
	li	$4,3			# 0x3
	sll	$3,$4,$3
	nor	$3,$0,$3
	and	$2,$2,$3
	sw	$2,24($sp)
	lw	$3,24($sp)
	lw	$2,20($sp)
	or	$2,$3,$2
	sw	$2,24($sp)
	lw	$3,24($sp)
	li	$4,4			# 0x4
	lui	$2,%hi($LC10)
	addiu	$5,$2,%lo($LC10)
	move	$6,$3
	lui	$2,%hi(eponDbgPrint)
	addiu	$2,$2,%lo(eponDbgPrint)
	jalr	$2
	nop

	lw	$3,24($sp)
	lw	$2,16($sp)
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	move	$2,$0
$L39:
	lw	$31,36($sp)
	addiu	$sp,$sp,40
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	eponSetLlidThrshldNum
	.size	eponSetLlidThrshldNum, .-eponSetLlidThrshldNum
	.rdata
	.align	2
$LC11:
	.ascii	"\015\012enter eponGetLlidQueNum\000"
	.align	2
$LC12:
	.ascii	"\015\012eponGetLlidQueNum param error\000"
	.align	2
$LC13:
	.ascii	"eponGetLlidQueNum eponRptCfgReg.Raw=%x\000"
	.section	.text.eponGetLlidThrshldNum,"ax",@progbits
	.align	2
	.globl	eponGetLlidThrshldNum
	.set	nomips16
	.set	nomicromips
	.ent	eponGetLlidThrshldNum
	.type	eponGetLlidThrshldNum, @function
eponGetLlidThrshldNum:
	.frame	$sp,32,$31		# vars= 8, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-32
	sw	$31,28($sp)
	move	$2,$4
	sw	$5,36($sp)
	sb	$2,32($sp)
	lui	$2,%hi(g_EPON_MAC_BASE)
	lw	$2,%lo(g_EPON_MAC_BASE)($2)
	addiu	$2,$2,24680
	sw	$2,16($sp)
	sw	$0,20($sp)
	li	$4,4			# 0x4
	lui	$2,%hi($LC11)
	addiu	$5,$2,%lo($LC11)
	lui	$2,%hi(eponDbgPrint)
	addiu	$2,$2,%lo(eponDbgPrint)
	jalr	$2
	nop

	lbu	$2,32($sp)
	sltu	$2,$2,8
	bne	$2,$0,$L41
	nop

	li	$4,1			# 0x1
	lui	$2,%hi($LC12)
	addiu	$5,$2,%lo($LC12)
	lui	$2,%hi(eponDbgPrint)
	addiu	$2,$2,%lo(eponDbgPrint)
	jalr	$2
	nop

	li	$2,-1			# 0xffffffffffffffff
	j	$L43
	nop

$L41:
	lw	$2,16($sp)
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	sw	$2,20($sp)
	lw	$2,20($sp)
	lbu	$3,32($sp)
	sll	$3,$3,1
	li	$4,3			# 0x3
	sll	$3,$4,$3
	and	$2,$2,$3
	sw	$2,20($sp)
	lw	$3,20($sp)
	lbu	$2,32($sp)
	sll	$2,$2,1
	li	$4,3			# 0x3
	sll	$2,$4,$2
	srl	$2,$3,$2
	sw	$2,20($sp)
	lw	$2,20($sp)
	andi	$3,$2,0x00ff
	lw	$2,36($sp)
	sb	$3,0($2)
	lw	$2,36($sp)
	lbu	$2,0($2)
	move	$3,$2
	li	$4,4			# 0x4
	lui	$2,%hi($LC13)
	addiu	$5,$2,%lo($LC13)
	move	$6,$3
	lui	$2,%hi(eponDbgPrint)
	addiu	$2,$2,%lo(eponDbgPrint)
	jalr	$2
	nop

	move	$2,$0
$L43:
	lw	$31,28($sp)
	addiu	$sp,$sp,32
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	eponGetLlidThrshldNum
	.size	eponGetLlidThrshldNum, .-eponGetLlidThrshldNum
	.rdata
	.align	2
$LC14:
	.ascii	"\015\012eponSetLlidQueThrod error\000"
	.section	.text.eponSetLlidQueThrod,"ax",@progbits
	.align	2
	.globl	eponSetLlidQueThrod
	.set	nomips16
	.set	nomicromips
	.ent	eponSetLlidQueThrod
	.type	eponSetLlidQueThrod, @function
eponSetLlidQueThrod:
	.frame	$sp,40,$31		# vars= 16, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-40
	sw	$31,36($sp)
	move	$3,$5
	move	$2,$6
	sw	$7,52($sp)
	sb	$4,40($sp)
	sb	$3,44($sp)
	sb	$2,48($sp)
	sb	$0,16($sp)
	sw	$0,20($sp)
	sb	$0,16($sp)
	j	$L45
	nop

$L48:
	lbu	$3,48($sp)
	lbu	$2,16($sp)
	sra	$2,$3,$2
	andi	$2,$2,0x1
	beq	$2,$0,$L46
	nop

	lbu	$2,40($sp)
	sb	$2,24($sp)
	lbu	$2,16($sp)
	sb	$2,25($sp)
	lbu	$2,44($sp)
	sb	$2,26($sp)
	lbu	$2,16($sp)
	sll	$2,$2,1
	lw	$3,52($sp)
	addu	$2,$3,$2
	lhu	$2,0($2)
	sh	$2,28($sp)
	addiu	$2,$sp,24
	move	$4,$2
	lui	$2,%hi(epon_set_queue_threshold)
	addiu	$2,$2,%lo(epon_set_queue_threshold)
	jalr	$2
	nop

	sw	$2,20($sp)
	lw	$2,20($sp)
	bgez	$2,$L46
	nop

	li	$4,1			# 0x1
	lui	$2,%hi($LC14)
	addiu	$5,$2,%lo($LC14)
	lui	$2,%hi(eponDbgPrint)
	addiu	$2,$2,%lo(eponDbgPrint)
	jalr	$2
	nop

	li	$2,-1			# 0xffffffffffffffff
	j	$L49
	nop

$L46:
	lbu	$2,16($sp)
	addiu	$2,$2,1
	sb	$2,16($sp)
$L45:
	lbu	$2,16($sp)
	sltu	$2,$2,8
	bne	$2,$0,$L48
	nop

	move	$2,$0
$L49:
	lw	$31,36($sp)
	addiu	$sp,$sp,40
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	eponSetLlidQueThrod
	.size	eponSetLlidQueThrod, .-eponSetLlidQueThrod
	.rdata
	.align	2
$LC15:
	.ascii	"\015\012eponGetLlidQueThrod error\000"
	.section	.text.eponGetLlidQueThrod,"ax",@progbits
	.align	2
	.globl	eponGetLlidQueThrod
	.set	nomips16
	.set	nomicromips
	.ent	eponGetLlidQueThrod
	.type	eponGetLlidQueThrod, @function
eponGetLlidQueThrod:
	.frame	$sp,40,$31		# vars= 16, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-40
	sw	$31,36($sp)
	move	$3,$5
	move	$2,$6
	sw	$7,52($sp)
	sb	$4,40($sp)
	sb	$3,44($sp)
	sb	$2,48($sp)
	sb	$0,16($sp)
	sw	$0,20($sp)
	sb	$0,16($sp)
	j	$L51
	nop

$L54:
	addiu	$2,$sp,24
	move	$4,$2
	move	$5,$0
	li	$6,6			# 0x6
	lui	$2,%hi(memset)
	addiu	$2,$2,%lo(memset)
	jalr	$2
	nop

	lbu	$3,48($sp)
	lbu	$2,16($sp)
	sra	$2,$3,$2
	andi	$2,$2,0x1
	beq	$2,$0,$L52
	nop

	lbu	$2,40($sp)
	sb	$2,24($sp)
	lbu	$2,16($sp)
	sb	$2,25($sp)
	lbu	$2,44($sp)
	sb	$2,26($sp)
	addiu	$2,$sp,24
	move	$4,$2
	lui	$2,%hi(epon_get_queue_threshold)
	addiu	$2,$2,%lo(epon_get_queue_threshold)
	jalr	$2
	nop

	sw	$2,20($sp)
	lw	$2,20($sp)
	bgez	$2,$L52
	nop

	li	$4,1			# 0x1
	lui	$2,%hi($LC15)
	addiu	$5,$2,%lo($LC15)
	lui	$2,%hi(eponDbgPrint)
	addiu	$2,$2,%lo(eponDbgPrint)
	jalr	$2
	nop

	li	$2,-1			# 0xffffffffffffffff
	j	$L55
	nop

$L52:
	lbu	$2,16($sp)
	addiu	$2,$2,1
	sb	$2,16($sp)
$L51:
	lbu	$2,16($sp)
	sltu	$2,$2,8
	bne	$2,$0,$L54
	nop

	move	$2,$0
$L55:
	lw	$31,36($sp)
	addiu	$sp,$sp,40
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	eponGetLlidQueThrod
	.size	eponGetLlidQueThrod, .-eponGetLlidQueThrod
	.rdata
	.align	2
$LC16:
	.ascii	"\015\012eponSetMpcpTime time is too big\000"
	.align	2
$LC17:
	.ascii	"eponSetMpcpTime time = %d\000"
	.section	.text.eponSetMpcpTime,"ax",@progbits
	.align	2
	.globl	eponSetMpcpTime
	.set	nomips16
	.set	nomicromips
	.ent	eponSetMpcpTime
	.type	eponSetMpcpTime, @function
eponSetMpcpTime:
	.frame	$sp,32,$31		# vars= 8, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-32
	sw	$31,28($sp)
	move	$2,$4
	sh	$2,32($sp)
	sw	$0,16($sp)
	sw	$0,20($sp)
	lhu	$2,32($sp)
	srl	$2,$2,1
	andi	$2,$2,0xffff
	sw	$2,16($sp)
	lw	$2,16($sp)
	sltu	$2,$2,1024
	bne	$2,$0,$L57
	nop

	li	$4,1			# 0x1
	lui	$2,%hi($LC16)
	addiu	$5,$2,%lo($LC16)
	lui	$2,%hi(eponDbgPrint)
	addiu	$2,$2,%lo(eponDbgPrint)
	jalr	$2
	nop

	li	$2,-1			# 0xffffffffffffffff
	j	$L58
	nop

$L57:
	lw	$2,16($sp)
	sltu	$2,$2,7
	beq	$2,$0,$L59
	nop

	li	$2,7			# 0x7
	sw	$2,16($sp)
$L59:
	lw	$2,16($sp)
	andi	$2,$2,0x3ff
	sw	$2,16($sp)
	lui	$2,%hi(g_EPON_MAC_BASE)
	lw	$2,%lo(g_EPON_MAC_BASE)($2)
	addiu	$2,$2,24804
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	sw	$2,20($sp)
	lw	$3,20($sp)
	li	$2,-1024			# 0xfffffffffffffc00
	and	$2,$3,$2
	sw	$2,20($sp)
	lw	$3,20($sp)
	lw	$2,16($sp)
	or	$2,$3,$2
	sw	$2,20($sp)
	lhu	$3,32($sp)
	li	$4,4			# 0x4
	lui	$2,%hi($LC17)
	addiu	$5,$2,%lo($LC17)
	move	$6,$3
	lui	$2,%hi(eponDbgPrint)
	addiu	$2,$2,%lo(eponDbgPrint)
	jalr	$2
	nop

	lui	$2,%hi(g_EPON_MAC_BASE)
	lw	$2,%lo(g_EPON_MAC_BASE)($2)
	addiu	$2,$2,24804
	lw	$4,20($sp)
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	move	$2,$0
$L58:
	lw	$31,28($sp)
	addiu	$sp,$sp,32
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	eponSetMpcpTime
	.size	eponSetMpcpTime, .-eponSetMpcpTime
	.rdata
	.align	2
$LC18:
	.ascii	"eponGetMpcpTime time = %d\000"
	.section	.text.eponGetMpcpTime,"ax",@progbits
	.align	2
	.globl	eponGetMpcpTime
	.set	nomips16
	.set	nomicromips
	.ent	eponGetMpcpTime
	.type	eponGetMpcpTime, @function
eponGetMpcpTime:
	.frame	$sp,32,$31		# vars= 8, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-32
	sw	$31,28($sp)
	sw	$4,32($sp)
	sw	$0,16($sp)
	lui	$2,%hi(g_EPON_MAC_BASE)
	lw	$2,%lo(g_EPON_MAC_BASE)($2)
	addiu	$2,$2,24804
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	sw	$2,16($sp)
	lw	$2,16($sp)
	andi	$2,$2,0x3ff
	sw	$2,16($sp)
	lw	$2,16($sp)
	sll	$2,$2,1
	sw	$2,16($sp)
	lw	$2,16($sp)
	andi	$3,$2,0xffff
	lw	$2,32($sp)
	sh	$3,0($2)
	lw	$2,32($sp)
	lhu	$2,0($2)
	move	$3,$2
	li	$4,4			# 0x4
	lui	$2,%hi($LC18)
	addiu	$5,$2,%lo($LC18)
	move	$6,$3
	lui	$2,%hi(eponDbgPrint)
	addiu	$2,$2,%lo(eponDbgPrint)
	jalr	$2
	nop

	move	$2,$0
	lw	$31,28($sp)
	addiu	$sp,$sp,32
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	eponGetMpcpTime
	.size	eponGetMpcpTime, .-eponGetMpcpTime
	.rdata
	.align	2
$LC19:
	.ascii	"eponSetHoldoverCfg enable = %d, time=%d\000"
	.section	.text.eponSetHoldoverCfg,"ax",@progbits
	.align	2
	.globl	eponSetHoldoverCfg
	.set	nomips16
	.set	nomicromips
	.ent	eponSetHoldoverCfg
	.type	eponSetHoldoverCfg, @function
eponSetHoldoverCfg:
	.frame	$sp,24,$31		# vars= 0, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-24
	sw	$31,20($sp)
	move	$3,$4
	move	$2,$5
	sb	$3,24($sp)
	sh	$2,28($sp)
	lbu	$6,24($sp)
	lhu	$3,28($sp)
	li	$4,4			# 0x4
	lui	$2,%hi($LC19)
	addiu	$5,$2,%lo($LC19)
	move	$7,$3
	lui	$2,%hi(eponDbgPrint)
	addiu	$2,$2,%lo(eponDbgPrint)
	jalr	$2
	nop

	lui	$2,%hi(eponDrv)
	addiu	$2,$2,%lo(eponDrv)
	lbu	$3,24($sp)
	sb	$3,4($2)
	lui	$2,%hi(eponDrv)
	addiu	$2,$2,%lo(eponDrv)
	lhu	$3,28($sp)
	sh	$3,6($2)
	move	$2,$0
	lw	$31,20($sp)
	addiu	$sp,$sp,24
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	eponSetHoldoverCfg
	.size	eponSetHoldoverCfg, .-eponSetHoldoverCfg
	.rdata
	.align	2
$LC20:
	.ascii	"eponGetHoldoverCfg enable = %d, time=%d\000"
	.section	.text.eponGetHoldoverCfg,"ax",@progbits
	.align	2
	.globl	eponGetHoldoverCfg
	.set	nomips16
	.set	nomicromips
	.ent	eponGetHoldoverCfg
	.type	eponGetHoldoverCfg, @function
eponGetHoldoverCfg:
	.frame	$sp,24,$31		# vars= 0, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-24
	sw	$31,20($sp)
	sw	$4,24($sp)
	sw	$5,28($sp)
	lui	$2,%hi(eponDrv)
	addiu	$2,$2,%lo(eponDrv)
	lbu	$2,4($2)
	move	$6,$2
	lui	$2,%hi(eponDrv)
	addiu	$2,$2,%lo(eponDrv)
	lhu	$2,6($2)
	move	$3,$2
	li	$4,4			# 0x4
	lui	$2,%hi($LC20)
	addiu	$5,$2,%lo($LC20)
	move	$7,$3
	lui	$2,%hi(eponDbgPrint)
	addiu	$2,$2,%lo(eponDbgPrint)
	jalr	$2
	nop

	lui	$2,%hi(eponDrv)
	addiu	$2,$2,%lo(eponDrv)
	lbu	$3,4($2)
	lw	$2,24($sp)
	sb	$3,0($2)
	lui	$2,%hi(eponDrv)
	addiu	$2,$2,%lo(eponDrv)
	lhu	$3,6($2)
	lw	$2,28($sp)
	sh	$3,0($2)
	move	$2,$0
	lw	$31,20($sp)
	addiu	$sp,$sp,24
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	eponGetHoldoverCfg
	.size	eponGetHoldoverCfg, .-eponGetHoldoverCfg
	.rdata
	.align	2
$LC21:
	.ascii	"eponGetDataTxCounter\000"
	.align	2
$LC22:
	.ascii	"eponGetDataTxCounter txCnt == NULL\000"
	.section	.text.eponGetDataTxCounter,"ax",@progbits
	.align	2
	.globl	eponGetDataTxCounter
	.set	nomips16
	.set	nomicromips
	.ent	eponGetDataTxCounter
	.type	eponGetDataTxCounter, @function
eponGetDataTxCounter:
	.frame	$sp,216,$31		# vars= 192, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-216
	sw	$31,212($sp)
	sw	$4,216($sp)
	li	$4,4			# 0x4
	lui	$2,%hi($LC21)
	addiu	$5,$2,%lo($LC21)
	lui	$2,%hi(eponDbgPrint)
	addiu	$2,$2,%lo(eponDbgPrint)
	jalr	$2
	nop

	lw	$2,216($sp)
	bne	$2,$0,$L67
	nop

	li	$4,1			# 0x1
	lui	$2,%hi($LC22)
	addiu	$5,$2,%lo($LC22)
	lui	$2,%hi(eponDbgPrint)
	addiu	$2,$2,%lo(eponDbgPrint)
	jalr	$2
	nop

	li	$2,-1			# 0xffffffffffffffff
	j	$L72
	nop

$L67:
	addiu	$2,$sp,24
	sw	$2,16($sp)
	sw	$0,20($sp)
	li	$2,40			# 0x28
	sw	$2,76($sp)
	addiu	$2,$sp,76
	li	$4,18			# 0x12
	move	$5,$0
	move	$6,$2
	lui	$2,%hi(__ECNT_HOOK)
	addiu	$2,$2,%lo(__ECNT_HOOK)
	jalr	$2
	nop

	sw	$2,20($sp)
	lw	$2,16($sp)
	move	$4,$2
	addiu	$3,$sp,128
	li	$2,52			# 0x34
	move	$5,$3
	move	$6,$2
	lui	$2,%hi(memcpy)
	addiu	$2,$2,%lo(memcpy)
	jalr	$2
	nop

	lw	$3,24($sp)
	lw	$2,216($sp)
	sw	$3,0($2)
	lw	$3,28($sp)
	lw	$2,216($sp)
	sw	$3,4($2)
	lw	$3,32($sp)
	lw	$2,216($sp)
	sw	$3,8($2)
	lw	$3,36($sp)
	lw	$2,216($sp)
	sw	$3,12($2)
	lw	$3,40($sp)
	lw	$2,216($sp)
	sw	$3,16($2)
	lw	$3,44($sp)
	lw	$2,216($sp)
	sw	$3,20($2)
	lw	$3,48($sp)
	lw	$2,216($sp)
	sw	$3,24($2)
	lw	$3,52($sp)
	lw	$2,216($sp)
	sw	$3,28($2)
	lw	$3,56($sp)
	lw	$2,216($sp)
	sw	$3,32($2)
	lw	$3,60($sp)
	lw	$2,216($sp)
	sw	$3,36($2)
	lw	$3,64($sp)
	lw	$2,216($sp)
	sw	$3,40($2)
	lw	$3,68($sp)
	lw	$2,216($sp)
	sw	$3,44($2)
	lw	$3,72($sp)
	lw	$2,216($sp)
	sw	$3,48($2)
	move	$2,$0
$L72:
	lw	$31,212($sp)
	addiu	$sp,$sp,216
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	eponGetDataTxCounter
	.size	eponGetDataTxCounter, .-eponGetDataTxCounter
	.rdata
	.align	2
$LC23:
	.ascii	"eponGetDataRxCounter\000"
	.align	2
$LC24:
	.ascii	"\015\012eponGetDataRxCounter rxCnt == NULL\000"
	.section	.text.eponGetDataRxCounter,"ax",@progbits
	.align	2
	.globl	eponGetDataRxCounter
	.set	nomips16
	.set	nomicromips
	.ent	eponGetDataRxCounter
	.type	eponGetDataRxCounter, @function
eponGetDataRxCounter:
	.frame	$sp,224,$31		# vars= 200, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-224
	sw	$31,220($sp)
	sw	$4,224($sp)
	li	$4,4			# 0x4
	lui	$2,%hi($LC23)
	addiu	$5,$2,%lo($LC23)
	lui	$2,%hi(eponDbgPrint)
	addiu	$2,$2,%lo(eponDbgPrint)
	jalr	$2
	nop

	lw	$2,224($sp)
	bne	$2,$0,$L74
	nop

	li	$4,1			# 0x1
	lui	$2,%hi($LC24)
	addiu	$5,$2,%lo($LC24)
	lui	$2,%hi(eponDbgPrint)
	addiu	$2,$2,%lo(eponDbgPrint)
	jalr	$2
	nop

	li	$2,-1			# 0xffffffffffffffff
	j	$L79
	nop

$L74:
	addiu	$2,$sp,24
	sw	$2,16($sp)
	sw	$0,20($sp)
	li	$2,39			# 0x27
	sw	$2,88($sp)
	addiu	$2,$sp,88
	li	$4,18			# 0x12
	move	$5,$0
	move	$6,$2
	lui	$2,%hi(__ECNT_HOOK)
	addiu	$2,$2,%lo(__ECNT_HOOK)
	jalr	$2
	nop

	sw	$2,20($sp)
	lw	$2,16($sp)
	move	$4,$2
	addiu	$3,$sp,140
	li	$2,64			# 0x40
	move	$5,$3
	move	$6,$2
	lui	$2,%hi(memcpy)
	addiu	$2,$2,%lo(memcpy)
	jalr	$2
	nop

	lw	$3,24($sp)
	lw	$2,224($sp)
	sw	$3,0($2)
	lw	$3,28($sp)
	lw	$2,224($sp)
	sw	$3,4($2)
	lw	$3,32($sp)
	lw	$2,224($sp)
	sw	$3,8($2)
	lw	$3,36($sp)
	lw	$2,224($sp)
	sw	$3,12($2)
	lw	$3,40($sp)
	lw	$2,224($sp)
	sw	$3,16($2)
	lw	$3,44($sp)
	lw	$2,224($sp)
	sw	$3,20($2)
	lw	$3,48($sp)
	lw	$2,224($sp)
	sw	$3,24($2)
	lw	$3,52($sp)
	lw	$2,224($sp)
	sw	$3,28($2)
	lw	$3,56($sp)
	lw	$2,224($sp)
	sw	$3,32($2)
	lw	$3,60($sp)
	lw	$2,224($sp)
	sw	$3,36($2)
	lw	$3,64($sp)
	lw	$2,224($sp)
	sw	$3,40($2)
	lw	$3,68($sp)
	lw	$2,224($sp)
	sw	$3,44($2)
	lw	$3,72($sp)
	lw	$2,224($sp)
	sw	$3,48($2)
	lw	$3,76($sp)
	lw	$2,224($sp)
	sw	$3,52($2)
	lw	$3,80($sp)
	lw	$2,224($sp)
	sw	$3,56($2)
	lw	$3,84($sp)
	lw	$2,224($sp)
	sw	$3,60($2)
	move	$2,$0
$L79:
	lw	$31,220($sp)
	addiu	$sp,$sp,224
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	eponGetDataRxCounter
	.size	eponGetDataRxCounter, .-eponGetDataRxCounter
	.section	.text.eponClearGdm2MIB,"ax",@progbits
	.align	2
	.globl	eponClearGdm2MIB
	.set	nomips16
	.set	nomicromips
	.ent	eponClearGdm2MIB
	.type	eponClearGdm2MIB, @function
eponClearGdm2MIB:
	.frame	$sp,184,$31		# vars= 160, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-184
	sw	$31,180($sp)
	move	$3,$4
	move	$2,$5
	sb	$3,184($sp)
	sb	$2,188($sp)
	lbu	$2,188($sp)
	andi	$2,$2,0x1
	beq	$2,$0,$L81
	nop

	li	$2,1			# 0x1
	sw	$2,16($sp)
	sw	$0,20($sp)
	sw	$0,24($sp)
	li	$2,43			# 0x2b
	sw	$2,40($sp)
	lw	$2,16($sp)
	sw	$2,44($sp)
	lw	$2,20($sp)
	sw	$2,60($sp)
	addiu	$2,$sp,40
	li	$4,18			# 0x12
	move	$5,$0
	move	$6,$2
	lui	$2,%hi(__ECNT_HOOK)
	addiu	$2,$2,%lo(__ECNT_HOOK)
	jalr	$2
	nop

	sw	$2,24($sp)
	j	$L85
	nop

$L81:
	lbu	$2,184($sp)
	andi	$2,$2,0x1
	beq	$2,$0,$L85
	nop

	li	$2,1			# 0x1
	sw	$2,28($sp)
	li	$2,1			# 0x1
	sw	$2,32($sp)
	sw	$0,36($sp)
	li	$2,43			# 0x2b
	sw	$2,40($sp)
	lw	$2,28($sp)
	sw	$2,44($sp)
	lw	$2,32($sp)
	sw	$2,60($sp)
	addiu	$2,$sp,40
	li	$4,18			# 0x12
	move	$5,$0
	move	$6,$2
	lui	$2,%hi(__ECNT_HOOK)
	addiu	$2,$2,%lo(__ECNT_HOOK)
	jalr	$2
	nop

	sw	$2,36($sp)
$L85:
	move	$2,$0
	lw	$31,180($sp)
	addiu	$sp,$sp,184
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	eponClearGdm2MIB
	.size	eponClearGdm2MIB, .-eponClearGdm2MIB
	.section	.text.eponMacSetTxbufUsage,"ax",@progbits
	.align	2
	.globl	eponMacSetTxbufUsage
	.set	nomips16
	.set	nomicromips
	.ent	eponMacSetTxbufUsage
	.type	eponMacSetTxbufUsage, @function
eponMacSetTxbufUsage:
	.frame	$sp,32,$31		# vars= 8, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-32
	sw	$31,28($sp)
	sw	$4,32($sp)
	sb	$0,16($sp)
	sb	$0,17($sp)
	sw	$0,20($sp)
	sb	$0,16($sp)
	j	$L91
	nop

$L93:
	lbu	$2,16($sp)
	li	$3,1			# 0x1
	sll	$2,$3,$2
	move	$3,$2
	lw	$2,32($sp)
	and	$2,$3,$2
	beq	$2,$0,$L92
	nop

	lbu	$2,17($sp)
	addiu	$2,$2,1
	sb	$2,17($sp)
$L92:
	lbu	$2,16($sp)
	addiu	$2,$2,1
	sb	$2,16($sp)
$L91:
	lbu	$2,16($sp)
	sltu	$2,$2,8
	bne	$2,$0,$L93
	nop

	lui	$2,%hi(gpWanPriv)
	lw	$2,%lo(gpWanPriv)($2)
	lbu	$3,17($sp)
	sw	$3,20($2)
	lbu	$2,17($sp)
	sltu	$2,$2,2
	beq	$2,$0,$L94
	nop

	lui	$2,%hi(gpWanPriv)
	lw	$3,%lo(gpWanPriv)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	lw	$2,-31788($2)
	andi	$2,$2,0x4000
	beq	$2,$0,$L95
	nop

	li	$2,-2147483648			# 0xffffffff80000000
	ori	$4,$2,0x20c0
	li	$2,-1078657024			# 0xffffffffbfb50000
	ori	$5,$2,0x1890
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	j	$L90
	nop

$L95:
	move	$4,$0
	li	$2,-1078657024			# 0xffffffffbfb50000
	ori	$5,$2,0x1890
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	j	$L90
	nop

$L94:
	lbu	$2,17($sp)
	li	$3,128			# 0x80
	div	$0,$3,$2
	mfhi	$3
	mflo	$2
	sw	$2,20($sp)
	lui	$2,%hi(gpWanPriv)
	lw	$3,%lo(gpWanPriv)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	lw	$2,-31788($2)
	andi	$2,$2,0x4000
	beq	$2,$0,$L98
	nop

	lw	$2,20($sp)
	sltu	$2,$2,33
	bne	$2,$0,$L98
	nop

	li	$2,32			# 0x20
	sw	$2,20($sp)
$L98:
	lw	$2,20($sp)
	sll	$2,$2,8
	sw	$2,20($sp)
	lw	$3,20($sp)
	li	$2,-2147483648			# 0xffffffff80000000
	ori	$2,$2,0x80
	or	$2,$3,$2
	sw	$2,20($sp)
	lw	$4,20($sp)
	li	$2,-1078657024			# 0xffffffffbfb50000
	ori	$5,$2,0x1890
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

$L90:
	lw	$31,28($sp)
	addiu	$sp,$sp,32
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	eponMacSetTxbufUsage
	.size	eponMacSetTxbufUsage, .-eponMacSetTxbufUsage
	.rdata
	.align	2
$LC25:
	.ascii	"\015\012enter eponMacSetLlidEnableMask = %x\000"
	.align	2
$LC26:
	.ascii	"LLID mask is the same\000"
	.section	.text.eponMacSetLlidEnableMask,"ax",@progbits
	.align	2
	.globl	eponMacSetLlidEnableMask
	.set	nomips16
	.set	nomicromips
	.ent	eponMacSetLlidEnableMask
	.type	eponMacSetLlidEnableMask, @function
eponMacSetLlidEnableMask:
	.frame	$sp,32,$31		# vars= 8, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-32
	sw	$31,28($sp)
	sw	$4,32($sp)
	sb	$0,16($sp)
	li	$4,1			# 0x1
	lui	$2,%hi($LC25)
	addiu	$5,$2,%lo($LC25)
	lw	$6,32($sp)
	lui	$2,%hi(eponDbgPrint)
	addiu	$2,$2,%lo(eponDbgPrint)
	jalr	$2
	nop

	lw	$4,32($sp)
	lui	$2,%hi(eponMacSetTxbufUsage)
	addiu	$2,$2,%lo(eponMacSetTxbufUsage)
	jalr	$2
	nop

	lui	$2,%hi(eponDrv)
	lw	$3,%lo(eponDrv)($2)
	lw	$2,32($sp)
	bne	$3,$2,$L100
	nop

	li	$4,4			# 0x4
	lui	$2,%hi($LC26)
	addiu	$5,$2,%lo($LC26)
	lui	$2,%hi(eponDbgPrint)
	addiu	$2,$2,%lo(eponDbgPrint)
	jalr	$2
	nop

	move	$2,$0
	j	$L101
	nop

$L100:
	sb	$0,16($sp)
	j	$L102
	nop

$L105:
	lbu	$2,16($sp)
	li	$3,1			# 0x1
	sll	$2,$3,$2
	move	$3,$2
	lui	$2,%hi(eponDrv)
	lw	$2,%lo(eponDrv)($2)
	and	$2,$3,$2
	bne	$2,$0,$L103
	nop

	lbu	$2,16($sp)
	li	$3,1			# 0x1
	sll	$2,$3,$2
	move	$3,$2
	lw	$2,32($sp)
	and	$2,$3,$2
	beq	$2,$0,$L103
	nop

	lbu	$2,16($sp)
	move	$4,$2
	lui	$2,%hi(eponLlidEnable)
	addiu	$2,$2,%lo(eponLlidEnable)
	jalr	$2
	nop

$L103:
	lbu	$2,16($sp)
	li	$3,1			# 0x1
	sll	$2,$3,$2
	move	$3,$2
	lui	$2,%hi(eponDrv)
	lw	$2,%lo(eponDrv)($2)
	and	$2,$3,$2
	beq	$2,$0,$L104
	nop

	lbu	$2,16($sp)
	li	$3,1			# 0x1
	sll	$2,$3,$2
	move	$3,$2
	lw	$2,32($sp)
	and	$2,$3,$2
	bne	$2,$0,$L104
	nop

	lbu	$2,16($sp)
	move	$4,$2
	lui	$2,%hi(eponLlidDisable)
	addiu	$2,$2,%lo(eponLlidDisable)
	jalr	$2
	nop

$L104:
	lbu	$2,16($sp)
	addiu	$2,$2,1
	sb	$2,16($sp)
$L102:
	lbu	$2,16($sp)
	sltu	$2,$2,8
	bne	$2,$0,$L105
	nop

	lui	$2,%hi(eponDrv)
	lw	$3,32($sp)
	sw	$3,%lo(eponDrv)($2)
	move	$2,$0
$L101:
	lw	$31,28($sp)
	addiu	$sp,$sp,32
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	eponMacSetLlidEnableMask
	.size	eponMacSetLlidEnableMask, .-eponMacSetLlidEnableMask
	.section	.text.PhyTxLedConf,"ax",@progbits
	.align	2
	.globl	PhyTxLedConf
	.set	nomips16
	.set	nomicromips
	.ent	PhyTxLedConf
	.type	PhyTxLedConf, @function
PhyTxLedConf:
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	sw	$4,0($sp)
	nop
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	PhyTxLedConf
	.size	PhyTxLedConf, .-PhyTxLedConf
	.globl	eponTxPwrDwnTmr
	.section	.bss,"aw",@nobits
	.align	2
	.type	eponTxPwrDwnTmr, @object
	.size	eponTxPwrDwnTmr, 28
eponTxPwrDwnTmr:
	.space	28
	.rdata
	.align	2
$LC27:
	.ascii	"eponPhyTxPwrUp timer done, restart phy power\012\000"
	.align	2
$LC28:
	.ascii	"ECNT_HOOK_ERROR occur with cmd_id:0x%x\012\000"
	.align	2
$LC29:
	.ascii	"No such API with type:%d, cmd_id:0x%x\012\000"
	.align	2
$LC30:
	.ascii	"\015\012 Epon set PHY Tx Power Enable failed!\000"
	.section	.text.eponPhyTxPwrUp,"ax",@progbits
	.align	2
	.globl	eponPhyTxPwrUp
	.set	nomips16
	.set	nomicromips
	.ent	eponPhyTxPwrUp
	.type	eponPhyTxPwrUp, @function
eponPhyTxPwrUp:
	.frame	$sp,88,$31		# vars= 64, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-88
	sw	$31,84($sp)
	sw	$4,88($sp)
	li	$4,1			# 0x1
	lui	$2,%hi($LC27)
	addiu	$5,$2,%lo($LC27)
	lui	$2,%hi(eponDbgPrint)
	addiu	$2,$2,%lo(eponDbgPrint)
	jalr	$2
	nop

	li	$4,1			# 0x1
	lui	$2,%hi(PhyTxLedConf)
	addiu	$2,$2,%lo(PhyTxLedConf)
	jalr	$2
	nop

	li	$2,1			# 0x1
	sw	$2,60($sp)
	li	$2,11			# 0xb
	sw	$2,16($sp)
	addiu	$2,$sp,60
	sw	$2,20($sp)
	sw	$0,64($sp)
	sw	$0,68($sp)
	sw	$0,72($sp)
	sw	$0,76($sp)
	li	$2,1			# 0x1
	sw	$2,64($sp)
	lw	$2,16($sp)
	sw	$2,68($sp)
	lw	$2,20($sp)
	sw	$2,76($sp)
	addiu	$2,$sp,64
	sw	$2,24($sp)
	li	$4,25			# 0x19
	move	$5,$0
	lw	$6,24($sp)
	lui	$2,%hi(__ECNT_HOOK)
	addiu	$2,$2,%lo(__ECNT_HOOK)
	jalr	$2
	nop

	move	$3,$2
	li	$2,-1			# 0xffffffffffffffff
	bne	$3,$2,$L109
	nop

	lw	$2,24($sp)
	lw	$3,4($2)
	lui	$2,%hi($LC28)
	addiu	$4,$2,%lo($LC28)
	move	$5,$3
	lui	$2,%hi(panic)
	addiu	$2,$2,%lo(panic)
	jalr	$2
	nop

$L109:
	lw	$2,24($sp)
	lw	$3,8($2)
	li	$2,-1			# 0xffffffffffffffff
	bne	$3,$2,$L110
	nop

	lw	$2,24($sp)
	lw	$5,0($2)
	lw	$2,24($sp)
	lw	$3,4($2)
	lui	$2,%hi($LC29)
	addiu	$4,$2,%lo($LC29)
	move	$6,$3
	lui	$2,%hi(panic)
	addiu	$2,$2,%lo(panic)
	jalr	$2
	nop

$L110:
	li	$2,1			# 0x1
	sw	$2,40($sp)
	li	$2,12			# 0xc
	sw	$2,28($sp)
	addiu	$2,$sp,40
	sw	$2,32($sp)
	sw	$0,44($sp)
	sw	$0,48($sp)
	sw	$0,52($sp)
	sw	$0,56($sp)
	li	$2,1			# 0x1
	sw	$2,44($sp)
	lw	$2,28($sp)
	sw	$2,48($sp)
	lw	$2,32($sp)
	sw	$2,56($sp)
	addiu	$2,$sp,44
	sw	$2,36($sp)
	li	$4,25			# 0x19
	move	$5,$0
	lw	$6,36($sp)
	lui	$2,%hi(__ECNT_HOOK)
	addiu	$2,$2,%lo(__ECNT_HOOK)
	jalr	$2
	nop

	move	$3,$2
	li	$2,-1			# 0xffffffffffffffff
	bne	$3,$2,$L113
	nop

	lw	$2,36($sp)
	lw	$3,4($2)
	lui	$2,%hi($LC28)
	addiu	$4,$2,%lo($LC28)
	move	$5,$3
	lui	$2,%hi(panic)
	addiu	$2,$2,%lo(panic)
	jalr	$2
	nop

$L113:
	lw	$2,36($sp)
	lw	$3,8($2)
	li	$2,-1			# 0xffffffffffffffff
	bne	$3,$2,$L114
	nop

	lw	$2,36($sp)
	lw	$5,0($2)
	lw	$2,36($sp)
	lw	$3,4($2)
	lui	$2,%hi($LC29)
	addiu	$4,$2,%lo($LC29)
	move	$6,$3
	lui	$2,%hi(panic)
	addiu	$2,$2,%lo(panic)
	jalr	$2
	nop

$L114:
	lw	$2,36($sp)
	lw	$3,8($2)
	li	$2,1			# 0x1
	bne	$3,$2,$L108
	nop

	li	$4,1			# 0x1
	lui	$2,%hi($LC30)
	addiu	$5,$2,%lo($LC30)
	lui	$2,%hi(eponDbgPrint)
	addiu	$2,$2,%lo(eponDbgPrint)
	jalr	$2
	nop

$L108:
	lw	$31,84($sp)
	addiu	$sp,$sp,88
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	eponPhyTxPwrUp
	.size	eponPhyTxPwrUp, .-eponPhyTxPwrUp
	.rdata
	.align	2
$LC31:
	.ascii	"Epon open phy power!\012\000"
	.align	2
$LC32:
	.ascii	"Epon close phy power forever!\012\000"
	.align	2
$LC33:
	.ascii	"\015\012 Epon close phy power %d sec!\000"
	.section	.text.eponPhyTxPowerCtl,"ax",@progbits
	.align	2
	.globl	eponPhyTxPowerCtl
	.set	nomips16
	.set	nomicromips
	.ent	eponPhyTxPowerCtl
	.type	eponPhyTxPowerCtl, @function
eponPhyTxPowerCtl:
	.frame	$sp,264,$31		# vars= 240, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-264
	sw	$31,260($sp)
	sw	$4,264($sp)
	sw	$0,16($sp)
	sw	$0,20($sp)
	lw	$2,264($sp)
	bne	$2,$0,$L120
	nop

	li	$2,32795			# 0x801b
	sw	$2,24($sp)
	lw	$2,24($sp)
	sw	$2,28($sp)
	sw	$0,32($sp)
	sw	$0,236($sp)
	sw	$0,240($sp)
	sw	$0,244($sp)
	sw	$0,248($sp)
	sw	$0,236($sp)
	lw	$2,28($sp)
	sw	$2,240($sp)
	lw	$2,32($sp)
	sw	$2,248($sp)
	addiu	$2,$sp,236
	sw	$2,36($sp)
	li	$4,25			# 0x19
	move	$5,$0
	lw	$6,36($sp)
	lui	$2,%hi(__ECNT_HOOK)
	addiu	$2,$2,%lo(__ECNT_HOOK)
	jalr	$2
	nop

	move	$3,$2
	li	$2,-1			# 0xffffffffffffffff
	bne	$3,$2,$L121
	nop

	lw	$2,36($sp)
	lw	$3,4($2)
	lui	$2,%hi($LC28)
	addiu	$4,$2,%lo($LC28)
	move	$5,$3
	lui	$2,%hi(panic)
	addiu	$2,$2,%lo(panic)
	jalr	$2
	nop

$L121:
	lw	$2,36($sp)
	lw	$3,8($2)
	li	$2,-1			# 0xffffffffffffffff
	bne	$3,$2,$L122
	nop

	lw	$2,36($sp)
	lw	$5,0($2)
	lw	$2,36($sp)
	lw	$3,4($2)
	lui	$2,%hi($LC29)
	addiu	$4,$2,%lo($LC29)
	move	$6,$3
	lui	$2,%hi(panic)
	addiu	$2,$2,%lo(panic)
	jalr	$2
	nop

$L122:
	lw	$2,36($sp)
	lw	$2,8($2)
	bne	$2,$0,$L136
	nop

	li	$4,1			# 0x1
	lui	$2,%hi($LC31)
	addiu	$5,$2,%lo($LC31)
	lui	$2,%hi(eponDbgPrint)
	addiu	$2,$2,%lo(eponDbgPrint)
	jalr	$2
	nop

	li	$4,1			# 0x1
	lui	$2,%hi(PhyTxLedConf)
	addiu	$2,$2,%lo(PhyTxLedConf)
	jalr	$2
	nop

	li	$2,1			# 0x1
	sw	$2,216($sp)
	li	$2,11			# 0xb
	sw	$2,40($sp)
	addiu	$2,$sp,216
	sw	$2,44($sp)
	sw	$0,220($sp)
	sw	$0,224($sp)
	sw	$0,228($sp)
	sw	$0,232($sp)
	li	$2,1			# 0x1
	sw	$2,220($sp)
	lw	$2,40($sp)
	sw	$2,224($sp)
	lw	$2,44($sp)
	sw	$2,232($sp)
	addiu	$2,$sp,220
	sw	$2,48($sp)
	li	$4,25			# 0x19
	move	$5,$0
	lw	$6,48($sp)
	lui	$2,%hi(__ECNT_HOOK)
	addiu	$2,$2,%lo(__ECNT_HOOK)
	jalr	$2
	nop

	move	$3,$2
	li	$2,-1			# 0xffffffffffffffff
	bne	$3,$2,$L127
	nop

	lw	$2,48($sp)
	lw	$3,4($2)
	lui	$2,%hi($LC28)
	addiu	$4,$2,%lo($LC28)
	move	$5,$3
	lui	$2,%hi(panic)
	addiu	$2,$2,%lo(panic)
	jalr	$2
	nop

$L127:
	lw	$2,48($sp)
	lw	$3,8($2)
	li	$2,-1			# 0xffffffffffffffff
	bne	$3,$2,$L128
	nop

	lw	$2,48($sp)
	lw	$5,0($2)
	lw	$2,48($sp)
	lw	$3,4($2)
	lui	$2,%hi($LC29)
	addiu	$4,$2,%lo($LC29)
	move	$6,$3
	lui	$2,%hi(panic)
	addiu	$2,$2,%lo(panic)
	jalr	$2
	nop

$L128:
	li	$2,1			# 0x1
	sw	$2,196($sp)
	li	$2,12			# 0xc
	sw	$2,52($sp)
	addiu	$2,$sp,196
	sw	$2,56($sp)
	sw	$0,200($sp)
	sw	$0,204($sp)
	sw	$0,208($sp)
	sw	$0,212($sp)
	li	$2,1			# 0x1
	sw	$2,200($sp)
	lw	$2,52($sp)
	sw	$2,204($sp)
	lw	$2,56($sp)
	sw	$2,212($sp)
	addiu	$2,$sp,200
	sw	$2,60($sp)
	li	$4,25			# 0x19
	move	$5,$0
	lw	$6,60($sp)
	lui	$2,%hi(__ECNT_HOOK)
	addiu	$2,$2,%lo(__ECNT_HOOK)
	jalr	$2
	nop

	move	$3,$2
	li	$2,-1			# 0xffffffffffffffff
	bne	$3,$2,$L131
	nop

	lw	$2,60($sp)
	lw	$3,4($2)
	lui	$2,%hi($LC28)
	addiu	$4,$2,%lo($LC28)
	move	$5,$3
	lui	$2,%hi(panic)
	addiu	$2,$2,%lo(panic)
	jalr	$2
	nop

$L131:
	lw	$2,60($sp)
	lw	$3,8($2)
	li	$2,-1			# 0xffffffffffffffff
	bne	$3,$2,$L136
	nop

	lw	$2,60($sp)
	lw	$5,0($2)
	lw	$2,60($sp)
	lw	$3,4($2)
	lui	$2,%hi($LC29)
	addiu	$4,$2,%lo($LC29)
	move	$6,$3
	lui	$2,%hi(panic)
	addiu	$2,$2,%lo(panic)
	jalr	$2
	nop

$L120:
	lw	$3,264($sp)
	li	$2,65535			# 0xffff
	bne	$3,$2,$L137
	nop

	li	$4,1			# 0x1
	lui	$2,%hi($LC32)
	addiu	$5,$2,%lo($LC32)
	lui	$2,%hi(eponDbgPrint)
	addiu	$2,$2,%lo(eponDbgPrint)
	jalr	$2
	nop

	lui	$2,%hi(eponTxPwrDwnTmr)
	addiu	$2,$2,%lo(eponTxPwrDwnTmr)
	sw	$2,64($sp)
	lw	$2,64($sp)
	lw	$2,0($2)
	sltu	$2,$0,$2
	andi	$2,$2,0x00ff
	beq	$2,$0,$L139
	nop

	lui	$2,%hi(eponTxPwrDwnTmr)
	addiu	$4,$2,%lo(eponTxPwrDwnTmr)
	lui	$2,%hi(del_timer)
	addiu	$2,$2,%lo(del_timer)
	jalr	$2
	nop

$L139:
	sw	$0,176($sp)
	li	$2,12			# 0xc
	sw	$2,68($sp)
	addiu	$2,$sp,176
	sw	$2,72($sp)
	sw	$0,180($sp)
	sw	$0,184($sp)
	sw	$0,188($sp)
	sw	$0,192($sp)
	li	$2,1			# 0x1
	sw	$2,180($sp)
	lw	$2,68($sp)
	sw	$2,184($sp)
	lw	$2,72($sp)
	sw	$2,192($sp)
	addiu	$2,$sp,180
	sw	$2,76($sp)
	li	$4,25			# 0x19
	move	$5,$0
	lw	$6,76($sp)
	lui	$2,%hi(__ECNT_HOOK)
	addiu	$2,$2,%lo(__ECNT_HOOK)
	jalr	$2
	nop

	move	$3,$2
	li	$2,-1			# 0xffffffffffffffff
	bne	$3,$2,$L140
	nop

	lw	$2,76($sp)
	lw	$3,4($2)
	lui	$2,%hi($LC28)
	addiu	$4,$2,%lo($LC28)
	move	$5,$3
	lui	$2,%hi(panic)
	addiu	$2,$2,%lo(panic)
	jalr	$2
	nop

$L140:
	lw	$2,76($sp)
	lw	$3,8($2)
	li	$2,-1			# 0xffffffffffffffff
	bne	$3,$2,$L141
	nop

	lw	$2,76($sp)
	lw	$5,0($2)
	lw	$2,76($sp)
	lw	$3,4($2)
	lui	$2,%hi($LC29)
	addiu	$4,$2,%lo($LC29)
	move	$6,$3
	lui	$2,%hi(panic)
	addiu	$2,$2,%lo(panic)
	jalr	$2
	nop

$L141:
	sw	$0,156($sp)
	li	$2,11			# 0xb
	sw	$2,80($sp)
	addiu	$2,$sp,156
	sw	$2,84($sp)
	sw	$0,160($sp)
	sw	$0,164($sp)
	sw	$0,168($sp)
	sw	$0,172($sp)
	li	$2,1			# 0x1
	sw	$2,160($sp)
	lw	$2,80($sp)
	sw	$2,164($sp)
	lw	$2,84($sp)
	sw	$2,172($sp)
	addiu	$2,$sp,160
	sw	$2,88($sp)
	li	$4,25			# 0x19
	move	$5,$0
	lw	$6,88($sp)
	lui	$2,%hi(__ECNT_HOOK)
	addiu	$2,$2,%lo(__ECNT_HOOK)
	jalr	$2
	nop

	move	$3,$2
	li	$2,-1			# 0xffffffffffffffff
	bne	$3,$2,$L145
	nop

	lw	$2,88($sp)
	lw	$3,4($2)
	lui	$2,%hi($LC28)
	addiu	$4,$2,%lo($LC28)
	move	$5,$3
	lui	$2,%hi(panic)
	addiu	$2,$2,%lo(panic)
	jalr	$2
	nop

$L145:
	lw	$2,88($sp)
	lw	$3,8($2)
	li	$2,-1			# 0xffffffffffffffff
	bne	$3,$2,$L146
	nop

	lw	$2,88($sp)
	lw	$5,0($2)
	lw	$2,88($sp)
	lw	$3,4($2)
	lui	$2,%hi($LC29)
	addiu	$4,$2,%lo($LC29)
	move	$6,$3
	lui	$2,%hi(panic)
	addiu	$2,$2,%lo(panic)
	jalr	$2
	nop

$L146:
	move	$4,$0
	lui	$2,%hi(PhyTxLedConf)
	addiu	$2,$2,%lo(PhyTxLedConf)
	jalr	$2
	nop

	j	$L136
	nop

$L137:
	lw	$2,264($sp)
	beq	$2,$0,$L136
	nop

	lw	$3,264($sp)
	li	$2,65535			# 0xffff
	sltu	$2,$3,$2
	beq	$2,$0,$L136
	nop

	li	$4,1			# 0x1
	lui	$2,%hi($LC33)
	addiu	$5,$2,%lo($LC33)
	lw	$6,264($sp)
	lui	$2,%hi(eponDbgPrint)
	addiu	$2,$2,%lo(eponDbgPrint)
	jalr	$2
	nop

	sw	$0,136($sp)
	li	$2,12			# 0xc
	sw	$2,92($sp)
	addiu	$2,$sp,136
	sw	$2,96($sp)
	sw	$0,140($sp)
	sw	$0,144($sp)
	sw	$0,148($sp)
	sw	$0,152($sp)
	li	$2,1			# 0x1
	sw	$2,140($sp)
	lw	$2,92($sp)
	sw	$2,144($sp)
	lw	$2,96($sp)
	sw	$2,152($sp)
	addiu	$2,$sp,140
	sw	$2,100($sp)
	li	$4,25			# 0x19
	move	$5,$0
	lw	$6,100($sp)
	lui	$2,%hi(__ECNT_HOOK)
	addiu	$2,$2,%lo(__ECNT_HOOK)
	jalr	$2
	nop

	move	$3,$2
	li	$2,-1			# 0xffffffffffffffff
	bne	$3,$2,$L149
	nop

	lw	$2,100($sp)
	lw	$3,4($2)
	lui	$2,%hi($LC28)
	addiu	$4,$2,%lo($LC28)
	move	$5,$3
	lui	$2,%hi(panic)
	addiu	$2,$2,%lo(panic)
	jalr	$2
	nop

$L149:
	lw	$2,100($sp)
	lw	$3,8($2)
	li	$2,-1			# 0xffffffffffffffff
	bne	$3,$2,$L150
	nop

	lw	$2,100($sp)
	lw	$5,0($2)
	lw	$2,100($sp)
	lw	$3,4($2)
	lui	$2,%hi($LC29)
	addiu	$4,$2,%lo($LC29)
	move	$6,$3
	lui	$2,%hi(panic)
	addiu	$2,$2,%lo(panic)
	jalr	$2
	nop

$L150:
	sw	$0,116($sp)
	li	$2,11			# 0xb
	sw	$2,104($sp)
	addiu	$2,$sp,116
	sw	$2,108($sp)
	sw	$0,120($sp)
	sw	$0,124($sp)
	sw	$0,128($sp)
	sw	$0,132($sp)
	li	$2,1			# 0x1
	sw	$2,120($sp)
	lw	$2,104($sp)
	sw	$2,124($sp)
	lw	$2,108($sp)
	sw	$2,132($sp)
	addiu	$2,$sp,120
	sw	$2,112($sp)
	li	$4,25			# 0x19
	move	$5,$0
	lw	$6,112($sp)
	lui	$2,%hi(__ECNT_HOOK)
	addiu	$2,$2,%lo(__ECNT_HOOK)
	jalr	$2
	nop

	move	$3,$2
	li	$2,-1			# 0xffffffffffffffff
	bne	$3,$2,$L154
	nop

	lw	$2,112($sp)
	lw	$3,4($2)
	lui	$2,%hi($LC28)
	addiu	$4,$2,%lo($LC28)
	move	$5,$3
	lui	$2,%hi(panic)
	addiu	$2,$2,%lo(panic)
	jalr	$2
	nop

$L154:
	lw	$2,112($sp)
	lw	$3,8($2)
	li	$2,-1			# 0xffffffffffffffff
	bne	$3,$2,$L155
	nop

	lw	$2,112($sp)
	lw	$5,0($2)
	lw	$2,112($sp)
	lw	$3,4($2)
	lui	$2,%hi($LC29)
	addiu	$4,$2,%lo($LC29)
	move	$6,$3
	lui	$2,%hi(panic)
	addiu	$2,$2,%lo(panic)
	jalr	$2
	nop

$L155:
	move	$4,$0
	lui	$2,%hi(PhyTxLedConf)
	addiu	$2,$2,%lo(PhyTxLedConf)
	jalr	$2
	nop

	lui	$2,%hi(eponTxPwrDwnTmr)
	addiu	$4,$2,%lo(eponTxPwrDwnTmr)
	move	$5,$0
	move	$6,$0
	lui	$2,%hi(init_timer_key)
	addiu	$2,$2,%lo(init_timer_key)
	jalr	$2
	nop

	lw	$2,264($sp)
	sll	$2,$2,2
	sll	$3,$2,2
	addu	$2,$2,$3
	sll	$3,$2,2
	addu	$3,$2,$3
	lui	$2,%hi(jiffies)
	lw	$2,%lo(jiffies)($2)
	addu	$3,$3,$2
	lui	$2,%hi(eponTxPwrDwnTmr)
	addiu	$2,$2,%lo(eponTxPwrDwnTmr)
	sw	$3,8($2)
	lui	$2,%hi(eponTxPwrDwnTmr)
	addiu	$2,$2,%lo(eponTxPwrDwnTmr)
	lui	$3,%hi(eponPhyTxPwrUp)
	addiu	$3,$3,%lo(eponPhyTxPwrUp)
	sw	$3,16($2)
	lui	$2,%hi(eponTxPwrDwnTmr)
	addiu	$2,$2,%lo(eponTxPwrDwnTmr)
	sw	$0,20($2)
	lw	$2,264($sp)
	sll	$2,$2,2
	sll	$3,$2,2
	addu	$2,$2,$3
	sll	$3,$2,2
	addu	$3,$2,$3
	lui	$2,%hi(jiffies)
	lw	$2,%lo(jiffies)($2)
	addu	$3,$3,$2
	lui	$2,%hi(eponTxPwrDwnTmr)
	addiu	$4,$2,%lo(eponTxPwrDwnTmr)
	move	$5,$3
	lui	$2,%hi(mod_timer)
	addiu	$2,$2,%lo(mod_timer)
	jalr	$2
	nop

$L136:
	move	$2,$0
	lw	$31,260($sp)
	addiu	$sp,$sp,264
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	eponPhyTxPowerCtl
	.size	eponPhyTxPowerCtl, .-eponPhyTxPowerCtl
	.rdata
	.align	2
$LC34:
	.ascii	"cmd = %d\000"
	.align	2
$LC35:
	.ascii	"eponMacIoctl = EPON_IOCTL_SET_LLID_ENABLE_MASK\000"
	.align	2
$LC36:
	.ascii	"eponMacIoctl = EPON_IOCTL_LLID_DEREGISTER\000"
	.align	2
$LC37:
	.ascii	"eponMacIoctl = EPON_IOCTL_LLID_DISCV_RGSTACK\000"
	.align	2
$LC38:
	.ascii	"eponMacIoctl = EPON_IOCTL_SET_LLID_KEY\000"
	.align	2
$LC39:
	.ascii	"eponMacIoctl = EPON_IOCTL_GET_LLID_KEY\000"
	.align	2
$LC40:
	.ascii	"eponMacIoctl = EPON_IOCTL_SET_LLID_FEC\000"
	.align	2
$LC41:
	.ascii	"eponMacIoctl = EPON_IOCTL_GET_LLID_FEC\000"
	.align	2
$LC42:
	.ascii	"eponMacIoctl = EPON_IOCTL_MAC_RST\000"
	.align	2
$LC43:
	.ascii	"eponMacIoctl = EPON_IOCTL_SET_LLID_DBA_QUE_NUM\000"
	.align	2
$LC44:
	.ascii	"eponMacIoctl = EPON_IOCTL_GET_LLID_DBA_QUE_NUM\000"
	.align	2
$LC45:
	.ascii	"eponMacIoctl = EPON_IOCTL_SET_LLID_DBA_THROD\000"
	.align	2
$LC46:
	.ascii	"eponMacIoctl = EPON_IOCTL_GET_LLID_DBA_THROD\000"
	.align	2
$LC47:
	.ascii	"eponMacIoctl = EPON_IOCTL_SET_MPCP_TIME\000"
	.align	2
$LC48:
	.ascii	"eponMacIoctl = EPON_IOCTL_GET_MPCP_TIME\000"
	.align	2
$LC49:
	.ascii	"eponMacIoctl = EPON_IOCTL_SET_HOLDOVER_CFG\000"
	.align	2
$LC50:
	.ascii	"eponMacIoctl = EPON_IOCTL_GET_HOLDOVER_CFG\000"
	.align	2
$LC51:
	.ascii	"eponMacIoctl = EPON_IOCTL_GET_TX_ETH_MIB\000"
	.align	2
$LC52:
	.ascii	"eponMacIoctl = EPON_IOCTL_GET_RX_ETH_MIB\000"
	.align	2
$LC53:
	.ascii	"eponMacIoctl = EPON_IOCTL_CLR_TX_RX_MIB\000"
	.align	2
$LC54:
	.ascii	"eponMacIoctl = EPON_IOCTL_POWER_CTL\000"
	.align	2
$LC55:
	.ascii	"eponMacIoctl = EPON_IOCTL_AUTH_FAIL_CTL\000"
	.align	2
$LC56:
	.ascii	"eponMacIoctl = EPON_IOCTL_SET_DEBUG_LEVEL\000"
	.align	2
$LC57:
	.ascii	"eponMacIoctl = EPON_IOCTL_SET_PHY_BURST_EN\000"
	.align	2
$LC58:
	.ascii	"eponMacIoctl = EPON_IOCTL_SET_DYING_GASP_MODE\000"
	.align	2
$LC59:
	.ascii	"eponMacIoctl = EPON_IOCTL_GET_DYING_GASP_MODE\000"
	.align	2
$LC60:
	.ascii	"eponMacIoctl = EPON_IOCTL_SET_DYING_GASP_NUM\000"
	.align	2
$LC61:
	.ascii	"eponMacIoctl = EPON_IOCTL_GET_DYING_GASP_NUM\000"
	.align	2
$LC62:
	.ascii	"eponMacIoctl = EPON_IOCTL_SET_SNIFFER_MODE\000"
	.section	.text.eponMacIoctl,"ax",@progbits
	.align	2
	.globl	eponMacIoctl
	.set	nomips16
	.set	nomicromips
	.ent	eponMacIoctl
	.type	eponMacIoctl, @function
eponMacIoctl:
	.frame	$sp,64,$31		# vars= 40, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-64
	sw	$31,60($sp)
	sw	$4,64($sp)
	sw	$5,68($sp)
	sw	$6,72($sp)
	sw	$0,16($sp)
	lw	$2,72($sp)
	sw	$2,20($sp)
	li	$4,4			# 0x4
	lui	$2,%hi($LC34)
	addiu	$5,$2,%lo($LC34)
	lw	$6,68($sp)
	lui	$2,%hi(eponDbgPrint)
	addiu	$2,$2,%lo(eponDbgPrint)
	jalr	$2
	nop

	lw	$2,68($sp)
	li	$3,-2147221504			# 0xffffffff80040000
	ori	$3,$3,0x6a03
	beq	$2,$3,$L161
	nop

	li	$3,-2147221504			# 0xffffffff80040000
	ori	$3,$3,0x6a04
	sltu	$3,$2,$3
	beq	$3,$0,$L162
	nop

	li	$3,1074003968			# 0x40040000
	ori	$3,$3,0x6a10
	beq	$2,$3,$L163
	nop

	li	$3,1074003968			# 0x40040000
	ori	$3,$3,0x6a11
	sltu	$3,$2,$3
	beq	$3,$0,$L164
	nop

	li	$3,1074003968			# 0x40040000
	ori	$3,$3,0x6a09
	beq	$2,$3,$L165
	nop

	li	$3,1074003968			# 0x40040000
	ori	$3,$3,0x6a0a
	sltu	$3,$2,$3
	beq	$3,$0,$L166
	nop

	li	$3,1074003968			# 0x40040000
	ori	$3,$3,0x6a06
	beq	$2,$3,$L167
	nop

	li	$3,1074003968			# 0x40040000
	ori	$3,$3,0x6a07
	beq	$2,$3,$L168
	nop

	j	$L197
	nop

$L166:
	li	$3,1074003968			# 0x40040000
	ori	$3,$3,0x6a0d
	beq	$2,$3,$L169
	nop

	li	$3,1074003968			# 0x40040000
	ori	$3,$3,0x6a0f
	beq	$2,$3,$L170
	nop

	li	$3,1074003968			# 0x40040000
	ori	$3,$3,0x6a0b
	beq	$2,$3,$L171
	nop

	j	$L197
	nop

$L164:
	li	$3,-2147418112			# 0xffffffff80010000
	ori	$3,$3,0x6a1e
	beq	$2,$3,$L172
	nop

	li	$3,-2147418112			# 0xffffffff80010000
	ori	$3,$3,0x6a1f
	sltu	$3,$2,$3
	beq	$3,$0,$L173
	nop

	li	$3,-2147418112			# 0xffffffff80010000
	ori	$3,$3,0x6a1b
	beq	$2,$3,$L174
	nop

	li	$3,-2147418112			# 0xffffffff80010000
	ori	$3,$3,0x6a1d
	beq	$2,$3,$L175
	nop

	li	$3,1074003968			# 0x40040000
	ori	$3,$3,0x6a11
	beq	$2,$3,$L176
	nop

	j	$L197
	nop

$L173:
	li	$3,-2147221504			# 0xffffffff80040000
	ori	$3,$3,0x6a01
	beq	$2,$3,$L177
	nop

	li	$3,-2147221504			# 0xffffffff80040000
	ori	$3,$3,0x6a02
	sltu	$3,$2,$3
	beq	$3,$0,$L178
	nop

	li	$3,-2147221504			# 0xffffffff80040000
	ori	$3,$3,0x6a00
	beq	$2,$3,$L179
	nop

	j	$L197
	nop

$L162:
	li	$3,-2147221504			# 0xffffffff80040000
	ori	$3,$3,0x6a12
	beq	$2,$3,$L180
	nop

	li	$3,-2147221504			# 0xffffffff80040000
	ori	$3,$3,0x6a13
	sltu	$3,$2,$3
	beq	$3,$0,$L181
	nop

	li	$3,-2147221504			# 0xffffffff80040000
	ori	$3,$3,0x6a08
	beq	$2,$3,$L182
	nop

	li	$3,-2147221504			# 0xffffffff80040000
	ori	$3,$3,0x6a09
	sltu	$3,$2,$3
	beq	$3,$0,$L183
	nop

	li	$3,-2147221504			# 0xffffffff80040000
	ori	$3,$3,0x6a04
	beq	$2,$3,$L184
	nop

	li	$3,-2147221504			# 0xffffffff80040000
	ori	$3,$3,0x6a05
	beq	$2,$3,$L185
	nop

	j	$L197
	nop

$L183:
	li	$3,-2147221504			# 0xffffffff80040000
	ori	$3,$3,0x6a0c
	beq	$2,$3,$L186
	nop

	li	$3,-2147221504			# 0xffffffff80040000
	ori	$3,$3,0x6a0e
	beq	$2,$3,$L187
	nop

	li	$3,-2147221504			# 0xffffffff80040000
	ori	$3,$3,0x6a0a
	beq	$2,$3,$L188
	nop

	j	$L197
	nop

$L181:
	li	$3,-2147221504			# 0xffffffff80040000
	ori	$3,$3,0x6a1c
	beq	$2,$3,$L189
	nop

	li	$3,-2147221504			# 0xffffffff80040000
	ori	$3,$3,0x6a1d
	sltu	$3,$2,$3
	beq	$3,$0,$L190
	nop

	li	$3,-2147221504			# 0xffffffff80040000
	ori	$3,$3,0x6a15
	beq	$2,$3,$L191
	nop

	li	$3,-2147221504			# 0xffffffff80040000
	ori	$3,$3,0x6a1a
	beq	$2,$3,$L192
	nop

	li	$3,-2147221504			# 0xffffffff80040000
	ori	$3,$3,0x6a13
	beq	$2,$3,$L193
	nop

	j	$L197
	nop

$L190:
	li	$3,-2147221504			# 0xffffffff80040000
	ori	$3,$3,0x6a20
	beq	$2,$3,$L194
	nop

	li	$3,-2147221504			# 0xffffffff80040000
	ori	$3,$3,0x6a22
	beq	$2,$3,$L195
	nop

	li	$3,-2147221504			# 0xffffffff80040000
	ori	$3,$3,0x6a1f
	beq	$2,$3,$L196
	nop

	j	$L197
	nop

$L179:
	li	$4,4			# 0x4
	lui	$2,%hi($LC35)
	addiu	$5,$2,%lo($LC35)
	lui	$2,%hi(eponDbgPrint)
	addiu	$2,$2,%lo(eponDbgPrint)
	jalr	$2
	nop

	lw	$2,20($sp)
	lw	$2,4($2)
	move	$4,$2
	lui	$2,%hi(eponMacSetLlidEnableMask)
	addiu	$2,$2,%lo(eponMacSetLlidEnableMask)
	jalr	$2
	nop

	sw	$2,16($sp)
	j	$L197
	nop

$L177:
	li	$4,4			# 0x4
	lui	$2,%hi($LC36)
	addiu	$5,$2,%lo($LC36)
	lui	$2,%hi(eponDbgPrint)
	addiu	$2,$2,%lo(eponDbgPrint)
	jalr	$2
	nop

	lw	$2,20($sp)
	lbu	$2,0($2)
	move	$4,$2
	lui	$2,%hi(eponMpcpLocalDergstr)
	addiu	$2,$2,%lo(eponMpcpLocalDergstr)
	jalr	$2
	nop

	sw	$2,16($sp)
	j	$L197
	nop

$L178:
	li	$4,4			# 0x4
	lui	$2,%hi($LC37)
	addiu	$5,$2,%lo($LC37)
	lui	$2,%hi(eponDbgPrint)
	addiu	$2,$2,%lo(eponDbgPrint)
	jalr	$2
	nop

	lw	$2,20($sp)
	lbu	$2,1($2)
	move	$3,$2
	lw	$2,20($sp)
	lbu	$2,8($2)
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(eponMpcpSetDiscvRgstAck)
	addiu	$2,$2,%lo(eponMpcpSetDiscvRgstAck)
	jalr	$2
	nop

	sw	$2,16($sp)
	j	$L197
	nop

$L161:
	li	$4,4			# 0x4
	lui	$2,%hi($LC38)
	addiu	$5,$2,%lo($LC38)
	lui	$2,%hi(eponDbgPrint)
	addiu	$2,$2,%lo(eponDbgPrint)
	jalr	$2
	nop

	lw	$2,20($sp)
	lbu	$2,0($2)
	move	$4,$2
	lw	$2,20($sp)
	lbu	$2,1($2)
	move	$3,$2
	lw	$2,20($sp)
	addiu	$2,$2,8
	move	$5,$3
	move	$6,$2
	lui	$2,%hi(eponSetLlidKey)
	addiu	$2,$2,%lo(eponSetLlidKey)
	jalr	$2
	nop

	sw	$2,16($sp)
	j	$L197
	nop

$L184:
	li	$4,4			# 0x4
	lui	$2,%hi($LC39)
	addiu	$5,$2,%lo($LC39)
	lui	$2,%hi(eponDbgPrint)
	addiu	$2,$2,%lo(eponDbgPrint)
	jalr	$2
	nop

	lw	$2,20($sp)
	lbu	$2,0($2)
	move	$4,$2
	lw	$2,20($sp)
	lbu	$2,1($2)
	move	$3,$2
	lw	$2,20($sp)
	addiu	$2,$2,8
	move	$5,$3
	move	$6,$2
	lui	$2,%hi(eponGetLlidKey)
	addiu	$2,$2,%lo(eponGetLlidKey)
	jalr	$2
	nop

	sw	$2,16($sp)
	j	$L197
	nop

$L185:
	li	$4,4			# 0x4
	lui	$2,%hi($LC40)
	addiu	$5,$2,%lo($LC40)
	lui	$2,%hi(eponDbgPrint)
	addiu	$2,$2,%lo(eponDbgPrint)
	jalr	$2
	nop

	lw	$2,20($sp)
	lbu	$2,0($2)
	move	$3,$2
	lw	$2,20($sp)
	lbu	$2,8($2)
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(eponSetLlidFec)
	addiu	$2,$2,%lo(eponSetLlidFec)
	jalr	$2
	nop

	sw	$2,16($sp)
	j	$L197
	nop

$L167:
	li	$4,4			# 0x4
	lui	$2,%hi($LC41)
	addiu	$5,$2,%lo($LC41)
	lui	$2,%hi(eponDbgPrint)
	addiu	$2,$2,%lo(eponDbgPrint)
	jalr	$2
	nop

	lw	$2,20($sp)
	lbu	$2,0($2)
	move	$3,$2
	lw	$2,20($sp)
	addiu	$2,$2,4
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(eponGetLlidFec)
	addiu	$2,$2,%lo(eponGetLlidFec)
	jalr	$2
	nop

	sw	$2,16($sp)
	j	$L197
	nop

$L168:
	lui	$2,%hi(eponWanResetWithChannelRetire)
	addiu	$2,$2,%lo(eponWanResetWithChannelRetire)
	jalr	$2
	nop

	li	$4,4			# 0x4
	lui	$2,%hi($LC42)
	addiu	$5,$2,%lo($LC42)
	lui	$2,%hi(eponDbgPrint)
	addiu	$2,$2,%lo(eponDbgPrint)
	jalr	$2
	nop

	j	$L197
	nop

$L182:
	li	$4,4			# 0x4
	lui	$2,%hi($LC43)
	addiu	$5,$2,%lo($LC43)
	lui	$2,%hi(eponDbgPrint)
	addiu	$2,$2,%lo(eponDbgPrint)
	jalr	$2
	nop

	lw	$2,20($sp)
	lbu	$2,0($2)
	move	$3,$2
	lw	$2,20($sp)
	lbu	$2,1($2)
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(eponSetLlidThrshldNum)
	addiu	$2,$2,%lo(eponSetLlidThrshldNum)
	jalr	$2
	nop

	sw	$2,16($sp)
	j	$L197
	nop

$L165:
	li	$4,4			# 0x4
	lui	$2,%hi($LC44)
	addiu	$5,$2,%lo($LC44)
	lui	$2,%hi(eponDbgPrint)
	addiu	$2,$2,%lo(eponDbgPrint)
	jalr	$2
	nop

	lw	$2,20($sp)
	lbu	$2,0($2)
	move	$3,$2
	lw	$2,20($sp)
	addiu	$2,$2,8
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(eponGetLlidThrshldNum)
	addiu	$2,$2,%lo(eponGetLlidThrshldNum)
	jalr	$2
	nop

	sw	$2,16($sp)
	j	$L197
	nop

$L188:
	li	$4,4			# 0x4
	lui	$2,%hi($LC45)
	addiu	$5,$2,%lo($LC45)
	lui	$2,%hi(eponDbgPrint)
	addiu	$2,$2,%lo(eponDbgPrint)
	jalr	$2
	nop

	lw	$2,20($sp)
	lbu	$2,0($2)
	move	$4,$2
	lw	$2,20($sp)
	lbu	$2,1($2)
	move	$5,$2
	lw	$2,20($sp)
	lbu	$2,8($2)
	move	$3,$2
	lw	$2,20($sp)
	addiu	$2,$2,9
	move	$6,$3
	move	$7,$2
	lui	$2,%hi(eponSetLlidQueThrod)
	addiu	$2,$2,%lo(eponSetLlidQueThrod)
	jalr	$2
	nop

	sw	$2,16($sp)
	j	$L197
	nop

$L171:
	li	$4,4			# 0x4
	lui	$2,%hi($LC46)
	addiu	$5,$2,%lo($LC46)
	lui	$2,%hi(eponDbgPrint)
	addiu	$2,$2,%lo(eponDbgPrint)
	jalr	$2
	nop

	lw	$2,20($sp)
	lbu	$2,0($2)
	move	$4,$2
	lw	$2,20($sp)
	lbu	$2,1($2)
	move	$5,$2
	lw	$2,20($sp)
	lbu	$2,8($2)
	move	$3,$2
	lw	$2,20($sp)
	addiu	$2,$2,9
	move	$6,$3
	move	$7,$2
	lui	$2,%hi(eponGetLlidQueThrod)
	addiu	$2,$2,%lo(eponGetLlidQueThrod)
	jalr	$2
	nop

	sw	$2,16($sp)
	j	$L197
	nop

$L186:
	li	$4,4			# 0x4
	lui	$2,%hi($LC47)
	addiu	$5,$2,%lo($LC47)
	lui	$2,%hi(eponDbgPrint)
	addiu	$2,$2,%lo(eponDbgPrint)
	jalr	$2
	nop

	lw	$2,20($sp)
	lhu	$2,2($2)
	move	$4,$2
	lui	$2,%hi(eponSetMpcpTime)
	addiu	$2,$2,%lo(eponSetMpcpTime)
	jalr	$2
	nop

	sw	$2,16($sp)
	j	$L197
	nop

$L169:
	li	$4,4			# 0x4
	lui	$2,%hi($LC48)
	addiu	$5,$2,%lo($LC48)
	lui	$2,%hi(eponDbgPrint)
	addiu	$2,$2,%lo(eponDbgPrint)
	jalr	$2
	nop

	lw	$2,20($sp)
	addiu	$2,$2,2
	move	$4,$2
	lui	$2,%hi(eponGetMpcpTime)
	addiu	$2,$2,%lo(eponGetMpcpTime)
	jalr	$2
	nop

	sw	$2,16($sp)
	j	$L197
	nop

$L187:
	li	$4,4			# 0x4
	lui	$2,%hi($LC49)
	addiu	$5,$2,%lo($LC49)
	lui	$2,%hi(eponDbgPrint)
	addiu	$2,$2,%lo(eponDbgPrint)
	jalr	$2
	nop

	lw	$2,20($sp)
	lbu	$2,1($2)
	move	$3,$2
	lw	$2,20($sp)
	lhu	$2,2($2)
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(eponSetHoldoverCfg)
	addiu	$2,$2,%lo(eponSetHoldoverCfg)
	jalr	$2
	nop

	sw	$2,16($sp)
	j	$L197
	nop

$L170:
	li	$4,4			# 0x4
	lui	$2,%hi($LC50)
	addiu	$5,$2,%lo($LC50)
	lui	$2,%hi(eponDbgPrint)
	addiu	$2,$2,%lo(eponDbgPrint)
	jalr	$2
	nop

	lw	$2,20($sp)
	addiu	$3,$2,1
	lw	$2,20($sp)
	addiu	$2,$2,2
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(eponGetHoldoverCfg)
	addiu	$2,$2,%lo(eponGetHoldoverCfg)
	jalr	$2
	nop

	sw	$2,16($sp)
	j	$L197
	nop

$L163:
	li	$4,4			# 0x4
	lui	$2,%hi($LC51)
	addiu	$5,$2,%lo($LC51)
	lui	$2,%hi(eponDbgPrint)
	addiu	$2,$2,%lo(eponDbgPrint)
	jalr	$2
	nop

	lw	$2,20($sp)
	addiu	$2,$2,8
	move	$4,$2
	lui	$2,%hi(eponGetDataTxCounter)
	addiu	$2,$2,%lo(eponGetDataTxCounter)
	jalr	$2
	nop

	sw	$2,16($sp)
	j	$L197
	nop

$L176:
	li	$4,4			# 0x4
	lui	$2,%hi($LC52)
	addiu	$5,$2,%lo($LC52)
	lui	$2,%hi(eponDbgPrint)
	addiu	$2,$2,%lo(eponDbgPrint)
	jalr	$2
	nop

	lw	$2,20($sp)
	addiu	$2,$2,8
	move	$4,$2
	lui	$2,%hi(eponGetDataRxCounter)
	addiu	$2,$2,%lo(eponGetDataRxCounter)
	jalr	$2
	nop

	sw	$2,16($sp)
	j	$L197
	nop

$L180:
	li	$4,4			# 0x4
	lui	$2,%hi($LC53)
	addiu	$5,$2,%lo($LC53)
	lui	$2,%hi(eponDbgPrint)
	addiu	$2,$2,%lo(eponDbgPrint)
	jalr	$2
	nop

	lw	$2,20($sp)
	lbu	$2,8($2)
	move	$3,$2
	lw	$2,20($sp)
	lbu	$2,9($2)
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(eponClearGdm2MIB)
	addiu	$2,$2,%lo(eponClearGdm2MIB)
	jalr	$2
	nop

	sw	$2,16($sp)
	j	$L197
	nop

$L193:
	li	$4,4			# 0x4
	lui	$2,%hi($LC54)
	addiu	$5,$2,%lo($LC54)
	lui	$2,%hi(eponDbgPrint)
	addiu	$2,$2,%lo(eponDbgPrint)
	jalr	$2
	nop

	lw	$2,20($sp)
	lw	$2,4($2)
	move	$4,$2
	lui	$2,%hi(eponPhyTxPowerCtl)
	addiu	$2,$2,%lo(eponPhyTxPowerCtl)
	jalr	$2
	nop

	sw	$2,16($sp)
	j	$L197
	nop

$L191:
	li	$4,4			# 0x4
	lui	$2,%hi($LC55)
	addiu	$5,$2,%lo($LC55)
	lui	$2,%hi(eponDbgPrint)
	addiu	$2,$2,%lo(eponDbgPrint)
	jalr	$2
	nop

	lw	$2,20($sp)
	lbu	$2,1($2)
	move	$3,$2
	lui	$2,%hi(isAuthFail)
	sw	$3,%lo(isAuthFail)($2)
	j	$L197
	nop

$L192:
	lw	$2,20($sp)
	lbu	$3,1($2)
	lui	$2,%hi(g_silence_time)
	sb	$3,%lo(g_silence_time)($2)
	j	$L197
	nop

$L174:
	li	$4,4			# 0x4
	lui	$2,%hi($LC56)
	addiu	$5,$2,%lo($LC56)
	lui	$2,%hi(eponDbgPrint)
	addiu	$2,$2,%lo(eponDbgPrint)
	jalr	$2
	nop

	lw	$2,20($sp)
	lbu	$2,1($2)
	move	$3,$2
	lui	$2,%hi(eponDebugLevel)
	sw	$3,%lo(eponDebugLevel)($2)
	j	$L197
	nop

$L189:
	li	$4,4			# 0x4
	lui	$2,%hi($LC57)
	addiu	$5,$2,%lo($LC57)
	lui	$2,%hi(eponDbgPrint)
	addiu	$2,$2,%lo(eponDbgPrint)
	jalr	$2
	nop

	lw	$2,20($sp)
	lw	$2,4($2)
	sw	$2,36($sp)
	li	$2,23			# 0x17
	sw	$2,24($sp)
	addiu	$2,$sp,36
	sw	$2,28($sp)
	sw	$0,40($sp)
	sw	$0,44($sp)
	sw	$0,48($sp)
	sw	$0,52($sp)
	li	$2,1			# 0x1
	sw	$2,40($sp)
	lw	$2,24($sp)
	sw	$2,44($sp)
	lw	$2,28($sp)
	sw	$2,52($sp)
	addiu	$2,$sp,40
	sw	$2,32($sp)
	li	$4,25			# 0x19
	move	$5,$0
	lw	$6,32($sp)
	lui	$2,%hi(__ECNT_HOOK)
	addiu	$2,$2,%lo(__ECNT_HOOK)
	jalr	$2
	nop

	move	$3,$2
	li	$2,-1			# 0xffffffffffffffff
	bne	$3,$2,$L198
	nop

	lw	$2,32($sp)
	lw	$3,4($2)
	lui	$2,%hi($LC28)
	addiu	$4,$2,%lo($LC28)
	move	$5,$3
	lui	$2,%hi(panic)
	addiu	$2,$2,%lo(panic)
	jalr	$2
	nop

$L198:
	lw	$2,32($sp)
	lw	$3,8($2)
	li	$2,-1			# 0xffffffffffffffff
	bne	$3,$2,$L199
	nop

	lw	$2,32($sp)
	lw	$5,0($2)
	lw	$2,32($sp)
	lw	$3,4($2)
	lui	$2,%hi($LC29)
	addiu	$4,$2,%lo($LC29)
	move	$6,$3
	lui	$2,%hi(panic)
	addiu	$2,$2,%lo(panic)
	jalr	$2
	nop

$L199:
	lw	$2,32($sp)
	lw	$2,8($2)
	sw	$2,16($sp)
	j	$L197
	nop

$L175:
	li	$4,4			# 0x4
	lui	$2,%hi($LC58)
	addiu	$5,$2,%lo($LC58)
	lui	$2,%hi(eponDbgPrint)
	addiu	$2,$2,%lo(eponDbgPrint)
	jalr	$2
	nop

	lw	$2,20($sp)
	lbu	$2,1($2)
	move	$4,$2
	lui	$2,%hi(eponHwDygaspCtrl)
	addiu	$2,$2,%lo(eponHwDygaspCtrl)
	jalr	$2
	nop

	sw	$2,16($sp)
	j	$L197
	nop

$L172:
	li	$4,4			# 0x4
	lui	$2,%hi($LC59)
	addiu	$5,$2,%lo($LC59)
	lui	$2,%hi(eponDbgPrint)
	addiu	$2,$2,%lo(eponDbgPrint)
	jalr	$2
	nop

	lw	$2,20($sp)
	addiu	$2,$2,1
	move	$4,$2
	lui	$2,%hi(eponDevGetDyingGaspMode)
	addiu	$2,$2,%lo(eponDevGetDyingGaspMode)
	jalr	$2
	nop

	sw	$2,16($sp)
	j	$L197
	nop

$L196:
	li	$4,4			# 0x4
	lui	$2,%hi($LC60)
	addiu	$5,$2,%lo($LC60)
	lui	$2,%hi(eponDbgPrint)
	addiu	$2,$2,%lo(eponDbgPrint)
	jalr	$2
	nop

	lw	$2,20($sp)
	lw	$2,4($2)
	move	$4,$2
	lui	$2,%hi(eponDevSetDyingGaspNum)
	addiu	$2,$2,%lo(eponDevSetDyingGaspNum)
	jalr	$2
	nop

	sw	$2,16($sp)
	j	$L197
	nop

$L194:
	li	$4,4			# 0x4
	lui	$2,%hi($LC61)
	addiu	$5,$2,%lo($LC61)
	lui	$2,%hi(eponDbgPrint)
	addiu	$2,$2,%lo(eponDbgPrint)
	jalr	$2
	nop

	lw	$2,20($sp)
	addiu	$2,$2,4
	move	$4,$2
	lui	$2,%hi(eponDevGetDyingGaspNum)
	addiu	$2,$2,%lo(eponDevGetDyingGaspNum)
	jalr	$2
	nop

	sw	$2,16($sp)
	j	$L197
	nop

$L195:
	li	$4,4			# 0x4
	lui	$2,%hi($LC62)
	addiu	$5,$2,%lo($LC62)
	lui	$2,%hi(eponDbgPrint)
	addiu	$2,$2,%lo(eponDbgPrint)
	jalr	$2
	nop

	lw	$2,20($sp)
	lbu	$2,1($2)
	move	$3,$2
	lw	$2,20($sp)
	lhu	$2,2($2)
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(eponSetSnifferModeConfig)
	addiu	$2,$2,%lo(eponSetSnifferModeConfig)
	jalr	$2
	nop

	sw	$2,16($sp)
	nop
$L197:
	lw	$2,16($sp)
	lw	$31,60($sp)
	addiu	$sp,$sp,64
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	eponMacIoctl
	.size	eponMacIoctl, .-eponMacIoctl
	.ident	"GCC: (Buildroot 2015.08.1) 4.9.3"
