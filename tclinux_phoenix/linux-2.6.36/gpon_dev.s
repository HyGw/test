	.file	1 "gpon_dev.c"
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
	.ascii	"[%lu0ms]WAN Configuration Error. mode:%d\012\000"
#NO_APP
	.section	.text.select_xpon_wan,"ax",@progbits
	.align	2
	.globl	select_xpon_wan
	.set	nomips16
	.set	nomicromips
	.ent	select_xpon_wan
	.type	select_xpon_wan, @function
select_xpon_wan:
	.frame	$sp,32,$31		# vars= 8, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-32
	sw	$31,28($sp)
	sw	$4,32($sp)
	li	$2,-1078984704			# 0xffffffffbfb00000
	ori	$4,$2,0x70
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	sw	$2,16($sp)
	lw	$2,32($sp)
	beq	$2,$0,$L3
	nop

	li	$3,1			# 0x1
	bne	$2,$3,$L8
	nop

	lw	$3,16($sp)
	li	$2,-8			# 0xfffffffffffffff8
	and	$2,$3,$2
	sw	$2,16($sp)
	j	$L5
	nop

$L3:
	lw	$3,16($sp)
	li	$2,-8			# 0xfffffffffffffff8
	and	$2,$3,$2
	sw	$2,16($sp)
	lw	$2,16($sp)
	ori	$2,$2,0x1
	sw	$2,16($sp)
	j	$L5
	nop

$L8:
	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	lhu	$2,-30816($2)
	andi	$2,$2,0x1
	beq	$2,$0,$L6
	nop

	lui	$2,%hi(jiffies)
	lw	$3,%lo(jiffies)($2)
	lui	$2,%hi($LC0)
	addiu	$4,$2,%lo($LC0)
	move	$5,$3
	lw	$6,32($sp)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	j	$L1
	nop

$L6:
	j	$L1
	nop

$L5:
	lw	$4,16($sp)
	li	$2,-1078984704			# 0xffffffffbfb00000
	ori	$5,$2,0x70
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

$L1:
	lw	$31,28($sp)
	addiu	$sp,$sp,32
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	select_xpon_wan
	.size	select_xpon_wan, .-select_xpon_wan
	.section	.text.gponDevGetPloamMsg,"ax",@progbits
	.align	2
	.globl	gponDevGetPloamMsg
	.set	nomips16
	.set	nomicromips
	.ent	gponDevGetPloamMsg
	.type	gponDevGetPloamMsg, @function
gponDevGetPloamMsg:
	.frame	$sp,32,$31		# vars= 8, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-32
	sw	$31,28($sp)
	sw	$4,32($sp)
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16472
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	sw	$2,20($sp)
	lbu	$2,23($sp)
	bne	$2,$0,$L10
	nop

	j	$L11
	nop

$L10:
	lbu	$2,23($sp)
	sltu	$2,$2,3
	bne	$2,$0,$L12
	nop

	sw	$0,16($sp)
	j	$L13
	nop

$L14:
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16476
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	move	$4,$2
	lw	$3,32($sp)
	lw	$2,16($sp)
	sll	$2,$2,2
	addu	$2,$3,$2
	sw	$4,0($2)
	lw	$2,16($sp)
	addiu	$2,$2,1
	sw	$2,16($sp)
$L13:
	lw	$2,16($sp)
	slt	$2,$2,3
	bne	$2,$0,$L14
	nop

	j	$L11
	nop

$L12:
	li	$2,-14			# 0xfffffffffffffff2
	j	$L16
	nop

$L11:
	lbu	$2,23($sp)
$L16:
	lw	$31,28($sp)
	addiu	$sp,$sp,32
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	gponDevGetPloamMsg
	.size	gponDevGetPloamMsg, .-gponDevGetPloamMsg
	.section	.text.gponDevSendPloamMsg,"ax",@progbits
	.align	2
	.globl	gponDevSendPloamMsg
	.set	nomips16
	.set	nomicromips
	.ent	gponDevSendPloamMsg
	.type	gponDevSendPloamMsg, @function
gponDevSendPloamMsg:
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
	j	$L18
	nop

$L21:
	sw	$0,20($sp)
	j	$L19
	nop

$L20:
	lw	$3,32($sp)
	lw	$2,20($sp)
	sll	$2,$2,2
	addu	$2,$3,$2
	lw	$3,0($2)
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16468
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	lw	$2,20($sp)
	addiu	$2,$2,1
	sw	$2,20($sp)
$L19:
	lw	$2,20($sp)
	slt	$2,$2,3
	bne	$2,$0,$L20
	nop

	lw	$2,16($sp)
	addiu	$2,$2,1
	sw	$2,16($sp)
$L18:
	lw	$3,16($sp)
	lw	$2,36($sp)
	sltu	$2,$3,$2
	bne	$2,$0,$L21
	nop

	move	$2,$0
	lw	$31,28($sp)
	addiu	$sp,$sp,32
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	gponDevSendPloamMsg
	.size	gponDevSendPloamMsg, .-gponDevSendPloamMsg
	.section	.text.gponDevSet1PPSInt,"ax",@progbits
	.align	2
	.globl	gponDevSet1PPSInt
	.set	nomips16
	.set	nomicromips
	.ent	gponDevSet1PPSInt
	.type	gponDevSet1PPSInt, @function
gponDevSet1PPSInt:
	.frame	$sp,40,$31		# vars= 16, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-40
	sw	$31,36($sp)
	sw	$4,40($sp)
	lw	$2,40($sp)
	xori	$2,$2,0x1
	sltu	$2,$2,1
	andi	$2,$2,0x00ff
	sw	$2,16($sp)
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16392
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	sw	$2,24($sp)
	lw	$2,16($sp)
	andi	$2,$2,0x00ff
	andi	$2,$2,0x1
	andi	$3,$2,0x00ff
	lw	$2,24($sp)
	ins	$2,$3,10,1
	sw	$2,24($sp)
	lw	$3,24($sp)
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16392
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16396
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	sw	$2,20($sp)
	lw	$2,16($sp)
	andi	$2,$2,0x00ff
	andi	$2,$2,0x1
	andi	$3,$2,0x00ff
	lw	$2,20($sp)
	ins	$2,$3,10,1
	sw	$2,20($sp)
	lw	$3,20($sp)
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16396
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	move	$2,$0
	lw	$31,36($sp)
	addiu	$sp,$sp,40
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	gponDevSet1PPSInt
	.size	gponDevSet1PPSInt, .-gponDevSet1PPSInt
	.rdata
	.align	2
$LC5:
	.ascii	"[%lu0ms]Before gponDevSetNewTod sec = %x, nanosec = %x\012"
	.ascii	"\000"
	.globl	__floatunsisf
	.globl	__divsf3
	.globl	__addsf3
	.globl	__mulsf3
	.globl	__fixunssfsi
	.align	2
$LC6:
	.ascii	"[%lu0ms]gponDevSetNewTod:  delta = %d, eqd = %d\012\000"
	.align	2
$LC7:
	.ascii	"[%lu0ms]After gponDevSetNewTod: sec = %x, nanosec = %x\012"
	.ascii	"\000"
	.section	.text.gponDevSetNewTod,"ax",@progbits
	.align	2
	.globl	gponDevSetNewTod
	.set	nomips16
	.set	nomicromips
	.ent	gponDevSetNewTod
	.type	gponDevSetNewTod, @function
gponDevSetNewTod:
	.frame	$sp,56,$31		# vars= 32, regs= 2/0, args= 16, gp= 0
	.mask	0x80010000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-56
	sw	$31,52($sp)
	sw	$16,48($sp)
	sw	$4,56($sp)
	sw	$5,60($sp)
	sw	$6,64($sp)
	lui	$2,%hi($LC1)
	lw	$2,%lo($LC1)($2)
	sw	$2,16($sp)
	lui	$2,%hi($LC2)
	lw	$2,%lo($LC2)($2)
	sw	$2,20($sp)
	lui	$2,%hi($LC3)
	lw	$2,%lo($LC3)($2)
	sw	$2,24($sp)
	lui	$2,%hi($LC4)
	lw	$2,%lo($LC4)($2)
	sw	$2,28($sp)
	sw	$0,32($sp)
	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	lhu	$2,-30816($2)
	andi	$2,$2,0x100
	beq	$2,$0,$L26
	nop

	lui	$2,%hi(jiffies)
	lw	$3,%lo(jiffies)($2)
	lui	$2,%hi($LC5)
	addiu	$4,$2,%lo($LC5)
	move	$5,$3
	lw	$6,60($sp)
	lw	$7,64($sp)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L26:
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lw	$3,268($2)
	lui	$2,%hi(__floatunsisf)
	addiu	$2,$2,%lo(__floatunsisf)
	move	$4,$3
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(__divsf3)
	addiu	$2,$2,%lo(__divsf3)
	move	$4,$3
	lw	$5,28($sp)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(__addsf3)
	addiu	$2,$2,%lo(__addsf3)
	move	$4,$3
	lw	$5,24($sp)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(__mulsf3)
	addiu	$2,$2,%lo(__mulsf3)
	move	$4,$3
	lw	$5,20($sp)
	jalr	$2
	nop

	move	$16,$2
	lui	$2,%hi(__addsf3)
	addiu	$2,$2,%lo(__addsf3)
	lw	$4,16($sp)
	lw	$5,20($sp)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(__divsf3)
	addiu	$2,$2,%lo(__divsf3)
	move	$4,$16
	move	$5,$3
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(__fixunssfsi)
	addiu	$2,$2,%lo(__fixunssfsi)
	move	$4,$3
	jalr	$2
	nop

	sw	$2,32($sp)
	lw	$3,32($sp)
	lw	$2,64($sp)
	sltu	$2,$2,$3
	beq	$2,$0,$L27
	nop

	lw	$2,60($sp)
	addiu	$2,$2,-1
	sw	$2,60($sp)
	lw	$3,64($sp)
	lw	$2,32($sp)
	subu	$2,$3,$2
	sw	$2,64($sp)
	j	$L28
	nop

$L27:
	lw	$3,64($sp)
	lw	$2,32($sp)
	subu	$2,$3,$2
	sw	$2,64($sp)
$L28:
	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	lhu	$2,-30816($2)
	andi	$2,$2,0x100
	beq	$2,$0,$L29
	nop

	lui	$2,%hi(jiffies)
	lw	$5,%lo(jiffies)($2)
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lw	$3,268($2)
	lui	$2,%hi($LC6)
	addiu	$4,$2,%lo($LC6)
	lw	$6,32($sp)
	move	$7,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L29:
	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	lhu	$2,-30816($2)
	andi	$2,$2,0x100
	beq	$2,$0,$L30
	nop

	lui	$2,%hi(jiffies)
	lw	$3,%lo(jiffies)($2)
	lui	$2,%hi($LC7)
	addiu	$4,$2,%lo($LC7)
	move	$5,$3
	lw	$6,60($sp)
	lw	$7,64($sp)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L30:
	lw	$2,56($sp)
	ext	$3,$2,0,30
	lw	$2,36($sp)
	ins	$2,$3,0,30
	sw	$2,36($sp)
	lw	$2,60($sp)
	sw	$2,40($sp)
	lw	$2,64($sp)
	sw	$2,44($sp)
	lw	$3,36($sp)
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16592
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	lw	$3,40($sp)
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16596
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	lw	$3,44($sp)
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16600
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	move	$2,$0
	lw	$31,52($sp)
	lw	$16,48($sp)
	addiu	$sp,$sp,56
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	gponDevSetNewTod
	.size	gponDevSetNewTod, .-gponDevSetNewTod
	.section	.text.gponDevGetCurrentTod,"ax",@progbits
	.align	2
	.globl	gponDevGetCurrentTod
	.set	nomips16
	.set	nomicromips
	.ent	gponDevGetCurrentTod
	.type	gponDevGetCurrentTod, @function
gponDevGetCurrentTod:
	.frame	$sp,32,$31		# vars= 8, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-32
	sw	$31,28($sp)
	sw	$4,32($sp)
	sw	$5,36($sp)
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16604
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	sw	$2,16($sp)
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16608
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	sw	$2,20($sp)
	lw	$3,16($sp)
	lw	$2,32($sp)
	sw	$3,0($2)
	lw	$3,20($sp)
	lw	$2,36($sp)
	sw	$3,0($2)
	move	$2,$0
	lw	$31,28($sp)
	addiu	$sp,$sp,32
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	gponDevGetCurrentTod
	.size	gponDevGetCurrentTod, .-gponDevGetCurrentTod
	.section	.text.gponDevGetNewTod,"ax",@progbits
	.align	2
	.globl	gponDevGetNewTod
	.set	nomips16
	.set	nomicromips
	.ent	gponDevGetNewTod
	.type	gponDevGetNewTod, @function
gponDevGetNewTod:
	.frame	$sp,32,$31		# vars= 8, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-32
	sw	$31,28($sp)
	sw	$4,32($sp)
	sw	$5,36($sp)
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16596
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	sw	$2,16($sp)
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16600
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	sw	$2,20($sp)
	lw	$3,16($sp)
	lw	$2,32($sp)
	sw	$3,0($2)
	lw	$3,20($sp)
	lw	$2,36($sp)
	sw	$3,0($2)
	move	$2,$0
	lw	$31,28($sp)
	addiu	$sp,$sp,32
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	gponDevGetNewTod
	.size	gponDevGetNewTod, .-gponDevGetNewTod
	.section	.text.gponDevGetTodSwitchTime,"ax",@progbits
	.align	2
	.globl	gponDevGetTodSwitchTime
	.set	nomips16
	.set	nomicromips
	.ent	gponDevGetTodSwitchTime
	.type	gponDevGetTodSwitchTime, @function
gponDevGetTodSwitchTime:
	.frame	$sp,32,$31		# vars= 8, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-32
	sw	$31,28($sp)
	sw	$4,32($sp)
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16592
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	sw	$2,16($sp)
	lw	$2,16($sp)
	ext	$2,$2,0,30
	move	$3,$2
	lw	$2,32($sp)
	sw	$3,0($2)
	move	$2,$0
	lw	$31,28($sp)
	addiu	$sp,$sp,32
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	gponDevGetTodSwitchTime
	.size	gponDevGetTodSwitchTime, .-gponDevGetTodSwitchTime
	.section	.text.gponDevSetSerialNumber,"ax",@progbits
	.align	2
	.globl	gponDevSetSerialNumber
	.set	nomips16
	.set	nomicromips
	.ent	gponDevSetSerialNumber
	.type	gponDevSetSerialNumber, @function
gponDevSetSerialNumber:
	.frame	$sp,32,$31		# vars= 8, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-32
	sw	$31,28($sp)
	sw	$4,32($sp)
	lw	$2,32($sp)
	lbu	$2,0($2)
	sll	$3,$2,24
	lw	$2,32($sp)
	addiu	$2,$2,1
	lbu	$2,0($2)
	sll	$2,$2,16
	or	$3,$3,$2
	lw	$2,32($sp)
	addiu	$2,$2,2
	lbu	$2,0($2)
	sll	$2,$2,8
	or	$2,$3,$2
	lw	$3,32($sp)
	addiu	$3,$3,3
	lbu	$3,0($3)
	or	$2,$2,$3
	sw	$2,16($sp)
	lw	$3,16($sp)
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16560
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	lw	$2,32($sp)
	addiu	$2,$2,4
	lbu	$2,0($2)
	sll	$3,$2,24
	lw	$2,32($sp)
	addiu	$2,$2,5
	lbu	$2,0($2)
	sll	$2,$2,16
	or	$3,$3,$2
	lw	$2,32($sp)
	addiu	$2,$2,6
	lbu	$2,0($2)
	sll	$2,$2,8
	or	$2,$3,$2
	lw	$3,32($sp)
	addiu	$3,$3,7
	lbu	$3,0($3)
	or	$2,$2,$3
	sw	$2,20($sp)
	lw	$3,20($sp)
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16564
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	move	$2,$0
	lw	$31,28($sp)
	addiu	$sp,$sp,32
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	gponDevSetSerialNumber
	.size	gponDevSetSerialNumber, .-gponDevSetSerialNumber
	.section	.text.gponDevResetGemInfo,"ax",@progbits
	.align	2
	.globl	gponDevResetGemInfo
	.set	nomips16
	.set	nomicromips
	.ent	gponDevResetGemInfo
	.type	gponDevResetGemInfo, @function
gponDevResetGemInfo:
	.frame	$sp,40,$31		# vars= 16, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-40
	sw	$31,36($sp)
	li	$2,3			# 0x3
	sw	$2,20($sp)
	sw	$0,16($sp)
	j	$L41
	nop

$L46:
	sw	$0,24($sp)
	lw	$2,24($sp)
	li	$3,1			# 0x1
	ins	$2,$3,31,1
	sw	$2,24($sp)
	lw	$2,24($sp)
	ins	$2,$0,16,1
	sw	$2,24($sp)
	lw	$2,24($sp)
	ins	$2,$0,17,1
	sw	$2,24($sp)
	lw	$2,16($sp)
	andi	$2,$2,0xffff
	andi	$2,$2,0xfff
	andi	$3,$2,0xffff
	lw	$2,24($sp)
	ins	$2,$3,0,12
	sw	$2,24($sp)
	lw	$3,24($sp)
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16448
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	li	$2,3			# 0x3
	sw	$2,20($sp)
	j	$L42
	nop

$L45:
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16452
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	sw	$2,28($sp)
	lw	$3,28($sp)
	li	$2,-2147483648			# 0xffffffff80000000
	and	$2,$3,$2
	beq	$2,$0,$L43
	nop

	j	$L44
	nop

$L43:
	li	$4,1000			# 0x3e8
	lui	$2,%hi(__udelay)
	addiu	$2,$2,%lo(__udelay)
	jalr	$2
	nop

$L42:
	lw	$2,20($sp)
	addiu	$3,$2,-1
	sw	$3,20($sp)
	bne	$2,$0,$L45
	nop

$L44:
	lw	$2,16($sp)
	addiu	$2,$2,1
	sw	$2,16($sp)
$L41:
	lw	$2,16($sp)
	slt	$2,$2,4096
	bne	$2,$0,$L46
	nop

	nop
	lw	$31,36($sp)
	addiu	$sp,$sp,40
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	gponDevResetGemInfo
	.size	gponDevResetGemInfo, .-gponDevResetGemInfo
	.rdata
	.align	2
$LC8:
	.ascii	"[%lu0ms]gponDevGetSuperframe: super frame counter = %x\012"
	.ascii	"\000"
	.section	.text.gponDevGetSuperframe,"ax",@progbits
	.align	2
	.globl	gponDevGetSuperframe
	.set	nomips16
	.set	nomicromips
	.ent	gponDevGetSuperframe
	.type	gponDevGetSuperframe, @function
gponDevGetSuperframe:
	.frame	$sp,32,$31		# vars= 8, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-32
	sw	$31,28($sp)
	sw	$4,32($sp)
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,17240
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	sw	$2,16($sp)
	lw	$2,16($sp)
	ext	$2,$2,0,30
	move	$3,$2
	lw	$2,32($sp)
	sw	$3,0($2)
	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	lhu	$2,-30816($2)
	andi	$2,$2,0x100
	beq	$2,$0,$L49
	nop

	lui	$2,%hi(jiffies)
	lw	$5,%lo(jiffies)($2)
	lw	$2,32($sp)
	lw	$3,0($2)
	lui	$2,%hi($LC8)
	addiu	$4,$2,%lo($LC8)
	move	$6,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L49:
	move	$2,$0
	lw	$31,28($sp)
	addiu	$sp,$sp,32
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	gponDevGetSuperframe
	.size	gponDevGetSuperframe, .-gponDevGetSuperframe
	.section	.text.gponDevGetGemInfo,"ax",@progbits
	.align	2
	.globl	gponDevGetGemInfo
	.set	nomips16
	.set	nomicromips
	.ent	gponDevGetGemInfo
	.type	gponDevGetGemInfo, @function
gponDevGetGemInfo:
	.frame	$sp,40,$31		# vars= 16, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-40
	sw	$31,36($sp)
	move	$2,$4
	sw	$5,44($sp)
	sw	$6,48($sp)
	sh	$2,40($sp)
	li	$2,3			# 0x3
	sw	$2,16($sp)
	lhu	$3,40($sp)
	li	$2,-4096			# 0xfffffffffffff000
	and	$2,$3,$2
	beq	$2,$0,$L52
	nop

	li	$2,-14			# 0xfffffffffffffff2
	j	$L57
	nop

$L52:
	lw	$2,20($sp)
	ins	$2,$0,31,1
	sw	$2,20($sp)
	lhu	$2,40($sp)
	andi	$2,$2,0xfff
	andi	$3,$2,0xffff
	lw	$2,20($sp)
	ins	$2,$3,0,12
	sw	$2,20($sp)
	lw	$3,20($sp)
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16448
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	j	$L54
	nop

$L56:
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16452
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	sw	$2,24($sp)
	lw	$3,24($sp)
	li	$2,-2147483648			# 0xffffffff80000000
	and	$2,$3,$2
	beq	$2,$0,$L55
	nop

	lw	$2,24($sp)
	andi	$2,$2,0x1
	sltu	$2,$0,$2
	andi	$2,$2,0x00ff
	move	$3,$2
	lw	$2,44($sp)
	sb	$3,0($2)
	lw	$2,24($sp)
	andi	$2,$2,0x2
	sltu	$2,$0,$2
	andi	$2,$2,0x00ff
	move	$3,$2
	lw	$2,48($sp)
	sb	$3,0($2)
	move	$2,$0
	j	$L57
	nop

$L55:
	li	$4,1000			# 0x3e8
	lui	$2,%hi(__udelay)
	addiu	$2,$2,%lo(__udelay)
	jalr	$2
	nop

$L54:
	lw	$2,16($sp)
	addiu	$3,$2,-1
	sw	$3,16($sp)
	bne	$2,$0,$L56
	nop

	li	$2,-62			# 0xffffffffffffffc2
$L57:
	lw	$31,36($sp)
	addiu	$sp,$sp,40
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	gponDevGetGemInfo
	.size	gponDevGetGemInfo, .-gponDevGetGemInfo
	.rdata
	.align	2
$LC9:
	.ascii	"Get GEM port ID:%d failed\012\000"
	.align	2
$LC10:
	.ascii	"Valid\000"
	.align	2
$LC11:
	.ascii	"Invalid\000"
	.align	2
$LC12:
	.ascii	"Encrypted\000"
	.align	2
$LC13:
	.ascii	"Not encrypted\000"
	.align	2
$LC14:
	.ascii	"GEM ID:%04d, Valid:%s,\011Encryption:%s\012\000"
	.section	.text.gponDevDumpGemInfo,"ax",@progbits
	.align	2
	.globl	gponDevDumpGemInfo
	.set	nomips16
	.set	nomicromips
	.ent	gponDevDumpGemInfo
	.type	gponDevDumpGemInfo, @function
gponDevDumpGemInfo:
	.frame	$sp,40,$31		# vars= 16, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-40
	sw	$31,36($sp)
	sw	$0,16($sp)
	j	$L59
	nop

$L66:
	lw	$2,16($sp)
	andi	$2,$2,0xffff
	move	$4,$2
	addiu	$3,$sp,24
	addiu	$2,$sp,25
	move	$5,$3
	move	$6,$2
	lui	$2,%hi(gponDevGetGemInfo)
	addiu	$2,$2,%lo(gponDevGetGemInfo)
	jalr	$2
	nop

	sw	$2,20($sp)
	lw	$2,20($sp)
	beq	$2,$0,$L60
	nop

	lui	$2,%hi($LC9)
	addiu	$4,$2,%lo($LC9)
	lw	$5,16($sp)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	li	$2,-14			# 0xfffffffffffffff2
	j	$L67
	nop

$L60:
	lbu	$3,24($sp)
	li	$2,1			# 0x1
	bne	$3,$2,$L62
	nop

	lui	$2,%hi($LC10)
	addiu	$3,$2,%lo($LC10)
	j	$L63
	nop

$L62:
	lui	$2,%hi($LC11)
	addiu	$3,$2,%lo($LC11)
$L63:
	lbu	$4,25($sp)
	li	$2,1			# 0x1
	bne	$4,$2,$L64
	nop

	lui	$2,%hi($LC12)
	addiu	$2,$2,%lo($LC12)
	j	$L65
	nop

$L64:
	lui	$2,%hi($LC13)
	addiu	$2,$2,%lo($LC13)
$L65:
	lui	$4,%hi($LC14)
	addiu	$4,$4,%lo($LC14)
	lw	$5,16($sp)
	move	$6,$3
	move	$7,$2
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lw	$2,16($sp)
	addiu	$2,$2,1
	sw	$2,16($sp)
$L59:
	lw	$2,16($sp)
	slt	$2,$2,4096
	bne	$2,$0,$L66
	nop

	move	$2,$0
$L67:
	lw	$31,36($sp)
	addiu	$sp,$sp,40
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	gponDevDumpGemInfo
	.size	gponDevDumpGemInfo, .-gponDevDumpGemInfo
	.section	.text.gponDevSetGemInfo,"ax",@progbits
	.align	2
	.globl	gponDevSetGemInfo
	.set	nomips16
	.set	nomicromips
	.ent	gponDevSetGemInfo
	.type	gponDevSetGemInfo, @function
gponDevSetGemInfo:
	.frame	$sp,40,$31		# vars= 16, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-40
	sw	$31,36($sp)
	move	$3,$5
	move	$2,$6
	sh	$4,40($sp)
	sb	$3,44($sp)
	sb	$2,48($sp)
	li	$2,3000			# 0xbb8
	sw	$2,16($sp)
	lhu	$3,40($sp)
	li	$2,-4096			# 0xfffffffffffff000
	and	$2,$3,$2
	beq	$2,$0,$L69
	nop

	li	$2,-14			# 0xfffffffffffffff2
	j	$L74
	nop

$L69:
	sw	$0,20($sp)
	lw	$2,20($sp)
	li	$3,1			# 0x1
	ins	$2,$3,31,1
	sw	$2,20($sp)
	lbu	$2,44($sp)
	xori	$2,$2,0x1
	sltu	$2,$2,1
	andi	$3,$2,0x00ff
	lw	$2,20($sp)
	ins	$2,$3,16,1
	sw	$2,20($sp)
	lbu	$2,48($sp)
	xori	$2,$2,0x1
	sltu	$2,$2,1
	andi	$3,$2,0x00ff
	lw	$2,20($sp)
	ins	$2,$3,17,1
	sw	$2,20($sp)
	lhu	$2,40($sp)
	andi	$2,$2,0xfff
	andi	$3,$2,0xffff
	lw	$2,20($sp)
	ins	$2,$3,0,12
	sw	$2,20($sp)
	lw	$3,20($sp)
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16448
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	j	$L71
	nop

$L73:
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16452
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	sw	$2,24($sp)
	lw	$3,24($sp)
	li	$2,-2147483648			# 0xffffffff80000000
	and	$2,$3,$2
	beq	$2,$0,$L72
	nop

	move	$2,$0
	j	$L74
	nop

$L72:
	li	$4,1			# 0x1
	lui	$2,%hi(__udelay)
	addiu	$2,$2,%lo(__udelay)
	jalr	$2
	nop

$L71:
	lw	$2,16($sp)
	addiu	$3,$2,-1
	sw	$3,16($sp)
	bne	$2,$0,$L73
	nop

	li	$2,-62			# 0xffffffffffffffc2
$L74:
	lw	$31,36($sp)
	addiu	$sp,$sp,40
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	gponDevSetGemInfo
	.size	gponDevSetGemInfo, .-gponDevSetGemInfo
	.section	.text.gponDevGemMibTablesInit,"ax",@progbits
	.align	2
	.globl	gponDevGemMibTablesInit
	.set	nomips16
	.set	nomicromips
	.ent	gponDevGemMibTablesInit
	.type	gponDevGemMibTablesInit, @function
gponDevGemMibTablesInit:
	.frame	$sp,40,$31		# vars= 16, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-40
	sw	$31,36($sp)
	li	$2,3			# 0x3
	sw	$2,16($sp)
	sw	$0,20($sp)
	lw	$2,20($sp)
	li	$3,1			# 0x1
	ins	$2,$3,0,1
	sw	$2,20($sp)
	lw	$3,20($sp)
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16692
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	sw	$0,24($sp)
	lw	$2,24($sp)
	li	$3,1			# 0x1
	ins	$2,$3,0,1
	sw	$2,24($sp)
	lw	$3,24($sp)
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16712
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	j	$L76
	nop

$L79:
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16692
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	sw	$2,20($sp)
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16448
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	sw	$2,24($sp)
	lw	$2,20($sp)
	andi	$2,$2,0x100
	beq	$2,$0,$L77
	nop

	lw	$2,24($sp)
	andi	$2,$2,0x100
	beq	$2,$0,$L77
	nop

	move	$2,$0
	j	$L80
	nop

$L77:
	li	$4,1000			# 0x3e8
	lui	$2,%hi(__udelay)
	addiu	$2,$2,%lo(__udelay)
	jalr	$2
	nop

$L76:
	lw	$2,16($sp)
	addiu	$3,$2,-1
	sw	$3,16($sp)
	bne	$2,$0,$L79
	nop

	li	$2,-62			# 0xffffffffffffffc2
$L80:
	lw	$31,36($sp)
	addiu	$sp,$sp,40
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	gponDevGemMibTablesInit
	.size	gponDevGemMibTablesInit, .-gponDevGemMibTablesInit
	.section	.text.gponDevUpdateGemMibIdxTable,"ax",@progbits
	.align	2
	.globl	gponDevUpdateGemMibIdxTable
	.set	nomips16
	.set	nomicromips
	.ent	gponDevUpdateGemMibIdxTable
	.type	gponDevUpdateGemMibIdxTable, @function
gponDevUpdateGemMibIdxTable:
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
	sh	$2,44($sp)
	li	$2,3			# 0x3
	sw	$2,16($sp)
	sw	$0,20($sp)
	lw	$2,20($sp)
	li	$3,1			# 0x1
	ins	$2,$3,31,1
	sw	$2,20($sp)
	lbu	$2,40($sp)
	sb	$2,21($sp)
	lhu	$2,44($sp)
	andi	$2,$2,0x1ff
	andi	$3,$2,0xffff
	lw	$2,20($sp)
	ins	$2,$3,0,9
	sw	$2,20($sp)
	lw	$3,20($sp)
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16704
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	j	$L82
	nop

$L85:
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16708
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	sw	$2,24($sp)
	lw	$3,24($sp)
	li	$2,-2147483648			# 0xffffffff80000000
	and	$2,$3,$2
	beq	$2,$0,$L83
	nop

	move	$2,$0
	j	$L86
	nop

$L83:
	li	$4,1000			# 0x3e8
	lui	$2,%hi(__udelay)
	addiu	$2,$2,%lo(__udelay)
	jalr	$2
	nop

$L82:
	lw	$2,16($sp)
	addiu	$3,$2,-1
	sw	$3,16($sp)
	bne	$2,$0,$L85
	nop

	li	$2,-62			# 0xffffffffffffffc2
$L86:
	lw	$31,36($sp)
	addiu	$sp,$sp,40
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	gponDevUpdateGemMibIdxTable
	.size	gponDevUpdateGemMibIdxTable, .-gponDevUpdateGemMibIdxTable
	.section	.text.__gponDevGetGemPortCounter,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	__gponDevGetGemPortCounter
	.type	__gponDevGetGemPortCounter, @function
__gponDevGetGemPortCounter:
	.frame	$sp,40,$31		# vars= 16, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-40
	sw	$31,36($sp)
	move	$3,$4
	sw	$5,44($sp)
	sw	$6,48($sp)
	sw	$7,52($sp)
	lw	$2,56($sp)
	sh	$3,40($sp)
	sb	$2,24($sp)
	li	$2,3			# 0x3
	sw	$2,16($sp)
	lhu	$3,40($sp)
	li	$2,-4096			# 0xfffffffffffff000
	and	$2,$3,$2
	beq	$2,$0,$L88
	nop

	li	$2,-14			# 0xfffffffffffffff2
	j	$L93
	nop

$L88:
	sw	$0,20($sp)
	lw	$2,20($sp)
	ins	$2,$0,31,1
	sw	$2,20($sp)
	lbu	$2,24($sp)
	andi	$2,$2,0x1
	andi	$3,$2,0x00ff
	lw	$2,20($sp)
	ins	$2,$3,24,1
	sw	$2,20($sp)
	lw	$2,44($sp)
	andi	$2,$2,0x00ff
	andi	$2,$2,0x7
	andi	$3,$2,0x00ff
	lw	$2,20($sp)
	ins	$2,$3,16,3
	sw	$2,20($sp)
	lhu	$2,40($sp)
	andi	$2,$2,0xfff
	andi	$3,$2,0xffff
	lw	$2,20($sp)
	ins	$2,$3,0,12
	sw	$2,20($sp)
	lw	$3,20($sp)
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16672
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	j	$L90
	nop

$L92:
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16672
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	sw	$2,20($sp)
	lw	$3,20($sp)
	li	$2,1073741824			# 0x40000000
	and	$2,$3,$2
	beq	$2,$0,$L91
	nop

	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16680
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	move	$3,$2
	lw	$2,48($sp)
	sw	$3,0($2)
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16676
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	move	$3,$2
	lw	$2,52($sp)
	sw	$3,0($2)
	move	$2,$0
	j	$L93
	nop

$L91:
	li	$4,1000			# 0x3e8
	lui	$2,%hi(__udelay)
	addiu	$2,$2,%lo(__udelay)
	jalr	$2
	nop

$L90:
	lw	$2,16($sp)
	addiu	$3,$2,-1
	sw	$3,16($sp)
	bne	$2,$0,$L92
	nop

	li	$2,-62			# 0xffffffffffffffc2
$L93:
	lw	$31,36($sp)
	addiu	$sp,$sp,40
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	__gponDevGetGemPortCounter
	.size	__gponDevGetGemPortCounter, .-__gponDevGetGemPortCounter
	.section	.text.gponDevClearGemPortCounter,"ax",@progbits
	.align	2
	.globl	gponDevClearGemPortCounter
	.set	nomips16
	.set	nomicromips
	.ent	gponDevClearGemPortCounter
	.type	gponDevClearGemPortCounter, @function
gponDevClearGemPortCounter:
	.frame	$sp,40,$31		# vars= 8, regs= 1/0, args= 24, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-40
	sw	$31,36($sp)
	move	$2,$4
	sh	$2,40($sp)
	lhu	$4,40($sp)
	addiu	$3,$sp,28
	li	$2,1			# 0x1
	sw	$2,16($sp)
	move	$5,$0
	addiu	$2,$sp,24
	move	$6,$2
	move	$7,$3
	lui	$2,%hi(__gponDevGetGemPortCounter)
	addiu	$2,$2,%lo(__gponDevGetGemPortCounter)
	jalr	$2
	nop

	beq	$2,$0,$L95
	nop

	li	$2,-1			# 0xffffffffffffffff
	j	$L100
	nop

$L95:
	lhu	$4,40($sp)
	addiu	$3,$sp,28
	li	$2,1			# 0x1
	sw	$2,16($sp)
	li	$5,1			# 0x1
	addiu	$2,$sp,24
	move	$6,$2
	move	$7,$3
	lui	$2,%hi(__gponDevGetGemPortCounter)
	addiu	$2,$2,%lo(__gponDevGetGemPortCounter)
	jalr	$2
	nop

	beq	$2,$0,$L97
	nop

	li	$2,-1			# 0xffffffffffffffff
	j	$L100
	nop

$L97:
	lhu	$4,40($sp)
	addiu	$3,$sp,28
	li	$2,1			# 0x1
	sw	$2,16($sp)
	li	$5,2			# 0x2
	addiu	$2,$sp,24
	move	$6,$2
	move	$7,$3
	lui	$2,%hi(__gponDevGetGemPortCounter)
	addiu	$2,$2,%lo(__gponDevGetGemPortCounter)
	jalr	$2
	nop

	beq	$2,$0,$L98
	nop

	li	$2,-1			# 0xffffffffffffffff
	j	$L100
	nop

$L98:
	lhu	$4,40($sp)
	addiu	$3,$sp,28
	li	$2,1			# 0x1
	sw	$2,16($sp)
	li	$5,3			# 0x3
	addiu	$2,$sp,24
	move	$6,$2
	move	$7,$3
	lui	$2,%hi(__gponDevGetGemPortCounter)
	addiu	$2,$2,%lo(__gponDevGetGemPortCounter)
	jalr	$2
	nop

	beq	$2,$0,$L99
	nop

	li	$2,-1			# 0xffffffffffffffff
	j	$L100
	nop

$L99:
	move	$2,$0
$L100:
	lw	$31,36($sp)
	addiu	$sp,$sp,40
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	gponDevClearGemPortCounter
	.size	gponDevClearGemPortCounter, .-gponDevClearGemPortCounter
	.section	.text.gponDevGetGemPortCounter,"ax",@progbits
	.align	2
	.globl	gponDevGetGemPortCounter
	.set	nomips16
	.set	nomicromips
	.ent	gponDevGetGemPortCounter
	.type	gponDevGetGemPortCounter, @function
gponDevGetGemPortCounter:
	.frame	$sp,32,$31		# vars= 0, regs= 1/0, args= 24, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-32
	sw	$31,28($sp)
	move	$2,$4
	sw	$5,36($sp)
	sw	$6,40($sp)
	sw	$7,44($sp)
	sh	$2,32($sp)
	lhu	$2,32($sp)
	sw	$0,16($sp)
	move	$4,$2
	lw	$5,36($sp)
	lw	$6,40($sp)
	lw	$7,44($sp)
	lui	$2,%hi(__gponDevGetGemPortCounter)
	addiu	$2,$2,%lo(__gponDevGetGemPortCounter)
	jalr	$2
	nop

	lw	$31,28($sp)
	addiu	$sp,$sp,32
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	gponDevGetGemPortCounter
	.size	gponDevGetGemPortCounter, .-gponDevGetGemPortCounter
	.section	.text.gponDevResetAllocId,"ax",@progbits
	.align	2
	.globl	gponDevResetAllocId
	.set	nomips16
	.set	nomicromips
	.ent	gponDevResetAllocId
	.type	gponDevResetAllocId, @function
gponDevResetAllocId:
	.frame	$sp,32,$31		# vars= 8, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-32
	sw	$31,28($sp)
	sw	$0,16($sp)
	lw	$2,16($sp)
	li	$3,255			# 0xff
	ins	$2,$3,0,12
	sw	$2,16($sp)
	lw	$2,16($sp)
	li	$3,255			# 0xff
	ins	$2,$3,16,12
	sw	$2,16($sp)
	lw	$3,16($sp)
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16416
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	lw	$3,16($sp)
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16420
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	lw	$3,16($sp)
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16424
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	lw	$3,16($sp)
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16428
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	lw	$3,16($sp)
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16432
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	lw	$3,16($sp)
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16436
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	lw	$3,16($sp)
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16440
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	lw	$3,16($sp)
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16444
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	lw	$31,28($sp)
	addiu	$sp,$sp,32
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	gponDevResetAllocId
	.size	gponDevResetAllocId, .-gponDevResetAllocId
	.section	.text.gponDevIsChannelValid,"ax",@progbits
	.align	2
	.globl	gponDevIsChannelValid
	.set	nomips16
	.set	nomicromips
	.ent	gponDevIsChannelValid
	.type	gponDevIsChannelValid, @function
gponDevIsChannelValid:
	.frame	$sp,56,$31		# vars= 32, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-56
	sw	$31,52($sp)
	move	$2,$4
	sb	$2,56($sp)
	lbu	$2,56($sp)
	sltu	$3,$2,16
	beq	$3,$0,$L105
	nop

	sll	$3,$2,2
	lui	$2,%hi($L107)
	addiu	$2,$2,%lo($L107)
	addu	$2,$3,$2
	lw	$2,0($2)
	j	$2
	nop

	.rdata
	.align	2
	.align	2
$L107:
	.word	$L106
	.word	$L108
	.word	$L109
	.word	$L110
	.word	$L111
	.word	$L112
	.word	$L113
	.word	$L114
	.word	$L115
	.word	$L116
	.word	$L117
	.word	$L118
	.word	$L119
	.word	$L120
	.word	$L121
	.word	$L122
	.section	.text.gponDevIsChannelValid
$L106:
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16416
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	sw	$2,16($sp)
	lw	$2,16($sp)
	ext	$2,$2,15,1
	andi	$2,$2,0x00ff
	j	$L124
	nop

$L108:
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16416
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	sw	$2,16($sp)
	lw	$2,16($sp)
	ext	$2,$2,31,1
	andi	$2,$2,0x00ff
	j	$L124
	nop

$L109:
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16420
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	sw	$2,20($sp)
	lw	$2,20($sp)
	ext	$2,$2,15,1
	andi	$2,$2,0x00ff
	j	$L124
	nop

$L110:
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16420
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	sw	$2,20($sp)
	lw	$2,20($sp)
	ext	$2,$2,31,1
	andi	$2,$2,0x00ff
	j	$L124
	nop

$L111:
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16424
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	sw	$2,24($sp)
	lw	$2,24($sp)
	ext	$2,$2,15,1
	andi	$2,$2,0x00ff
	j	$L124
	nop

$L112:
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16424
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	sw	$2,24($sp)
	lw	$2,24($sp)
	ext	$2,$2,31,1
	andi	$2,$2,0x00ff
	j	$L124
	nop

$L113:
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16428
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	sw	$2,28($sp)
	lw	$2,28($sp)
	ext	$2,$2,15,1
	andi	$2,$2,0x00ff
	j	$L124
	nop

$L114:
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16428
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	sw	$2,28($sp)
	lw	$2,28($sp)
	ext	$2,$2,31,1
	andi	$2,$2,0x00ff
	j	$L124
	nop

$L115:
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16432
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	sw	$2,32($sp)
	lw	$2,32($sp)
	ext	$2,$2,15,1
	andi	$2,$2,0x00ff
	j	$L124
	nop

$L116:
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16428
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	sw	$2,28($sp)
	lw	$2,32($sp)
	ext	$2,$2,31,1
	andi	$2,$2,0x00ff
	j	$L124
	nop

$L117:
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16436
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	sw	$2,36($sp)
	lw	$2,36($sp)
	ext	$2,$2,15,1
	andi	$2,$2,0x00ff
	j	$L124
	nop

$L118:
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16428
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	sw	$2,28($sp)
	lw	$2,36($sp)
	ext	$2,$2,31,1
	andi	$2,$2,0x00ff
	j	$L124
	nop

$L119:
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16440
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	sw	$2,40($sp)
	lw	$2,40($sp)
	ext	$2,$2,15,1
	andi	$2,$2,0x00ff
	j	$L124
	nop

$L120:
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16428
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	sw	$2,28($sp)
	lw	$2,40($sp)
	ext	$2,$2,31,1
	andi	$2,$2,0x00ff
	j	$L124
	nop

$L121:
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16444
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	sw	$2,44($sp)
	lw	$2,44($sp)
	ext	$2,$2,15,1
	andi	$2,$2,0x00ff
	j	$L124
	nop

$L122:
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16428
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	sw	$2,28($sp)
	lw	$2,44($sp)
	ext	$2,$2,31,1
	andi	$2,$2,0x00ff
	j	$L124
	nop

$L105:
	move	$2,$0
$L124:
	lw	$31,52($sp)
	addiu	$sp,$sp,56
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	gponDevIsChannelValid
	.size	gponDevIsChannelValid, .-gponDevIsChannelValid
	.section	.text.gponDevEnableSniffMode,"ax",@progbits
	.align	2
	.globl	gponDevEnableSniffMode
	.set	nomips16
	.set	nomicromips
	.ent	gponDevEnableSniffMode
	.type	gponDevEnableSniffMode, @function
gponDevEnableSniffMode:
	.frame	$sp,48,$31		# vars= 24, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-48
	sw	$31,44($sp)
	sw	$4,48($sp)
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,17256
	sw	$2,16($sp)
	li	$2,1			# 0x1
	sw	$2,20($sp)
	sw	$0,24($sp)
	lw	$2,48($sp)
	sw	$2,28($sp)
	lw	$2,16($sp)
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	sw	$2,32($sp)
	lw	$2,24($sp)
	lw	$3,20($sp)
	sll	$2,$3,$2
	nor	$3,$0,$2
	lw	$2,32($sp)
	and	$3,$3,$2
	lw	$4,20($sp)
	lw	$2,28($sp)
	and	$4,$4,$2
	lw	$2,24($sp)
	sll	$2,$4,$2
	or	$2,$3,$2
	sw	$2,32($sp)
	lw	$2,16($sp)
	lw	$4,32($sp)
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	lw	$31,44($sp)
	addiu	$sp,$sp,48
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	gponDevEnableSniffMode
	.size	gponDevEnableSniffMode, .-gponDevEnableSniffMode
	.section	.text.gponDevSetSniffMode,"ax",@progbits
	.align	2
	.globl	gponDevSetSniffMode
	.set	nomips16
	.set	nomicromips
	.ent	gponDevSetSniffMode
	.type	gponDevSetSniffMode, @function
gponDevSetSniffMode:
	.frame	$sp,48,$31		# vars= 24, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-48
	sw	$31,44($sp)
	sw	$4,48($sp)
	lw	$2,48($sp)
	lw	$2,28($2)
	beq	$2,$0,$L127
	nop

	lw	$2,48($sp)
	lw	$3,28($2)
	li	$2,1			# 0x1
	beq	$3,$2,$L127
	nop

	li	$2,-1			# 0xffffffffffffffff
	j	$L131
	nop

$L127:
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,17256
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	sw	$2,16($sp)
	lw	$2,48($sp)
	lw	$2,28($2)
	andi	$2,$2,0x00ff
	andi	$2,$2,0x1
	andi	$3,$2,0x00ff
	lw	$2,16($sp)
	ins	$2,$3,0,1
	sw	$2,16($sp)
	lw	$2,48($sp)
	lw	$3,28($2)
	li	$2,1			# 0x1
	bne	$3,$2,$L129
	nop

	lw	$2,48($sp)
	lw	$3,0($2)
	li	$2,32769			# 0x8001
	beq	$3,$2,$L130
	nop

	lw	$2,48($sp)
	lw	$3,0($2)
	li	$2,32770			# 0x8002
	beq	$3,$2,$L130
	nop

	lw	$2,48($sp)
	lw	$3,0($2)
	li	$2,32772			# 0x8004
	beq	$3,$2,$L130
	nop

	lw	$2,48($sp)
	lw	$3,0($2)
	li	$2,32776			# 0x8008
	beq	$3,$2,$L130
	nop

	li	$2,-1			# 0xffffffffffffffff
	j	$L131
	nop

$L130:
	lw	$2,48($sp)
	lhu	$2,4($2)
	sh	$2,28($sp)
	lw	$2,48($sp)
	lhu	$2,6($2)
	sh	$2,30($sp)
	lw	$3,28($sp)
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,17292
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	lw	$2,48($sp)
	lhu	$2,8($2)
	sh	$2,24($sp)
	lw	$2,48($sp)
	lhu	$2,10($2)
	sh	$2,26($sp)
	lw	$3,24($sp)
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,17264
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	lw	$2,48($sp)
	lhu	$2,14($2)
	sh	$2,32($sp)
	lw	$2,48($sp)
	lhu	$2,16($2)
	sh	$2,34($sp)
	lw	$3,32($sp)
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,17296
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	lw	$2,48($sp)
	lhu	$2,18($2)
	sh	$2,20($sp)
	lw	$2,48($sp)
	lhu	$2,20($2)
	sh	$2,22($sp)
	lw	$3,20($sp)
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,17260
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	lw	$2,48($sp)
	lhu	$2,22($2)
	sh	$2,36($sp)
	lw	$2,48($sp)
	lhu	$2,12($2)
	sh	$2,38($sp)
	lw	$3,36($sp)
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,17308
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	lw	$2,48($sp)
	lw	$2,24($2)
	andi	$2,$2,0x00ff
	andi	$2,$2,0x1
	andi	$3,$2,0x00ff
	lw	$2,16($sp)
	ins	$2,$3,8,1
	sw	$2,16($sp)
$L129:
	lw	$3,16($sp)
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,17256
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	move	$2,$0
$L131:
	lw	$31,44($sp)
	addiu	$sp,$sp,48
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	gponDevSetSniffMode
	.size	gponDevSetSniffMode, .-gponDevSetSniffMode
	.section	.text.gponDevGetSniffMode,"ax",@progbits
	.align	2
	.globl	gponDevGetSniffMode
	.set	nomips16
	.set	nomicromips
	.ent	gponDevGetSniffMode
	.type	gponDevGetSniffMode, @function
gponDevGetSniffMode:
	.frame	$sp,48,$31		# vars= 24, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-48
	sw	$31,44($sp)
	sw	$4,48($sp)
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,17256
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	sw	$2,16($sp)
	lw	$2,16($sp)
	ext	$2,$2,0,1
	andi	$2,$2,0x00ff
	move	$3,$2
	lw	$2,48($sp)
	sw	$3,28($2)
	lw	$2,16($sp)
	ext	$2,$2,8,1
	andi	$2,$2,0x00ff
	move	$3,$2
	lw	$2,48($sp)
	sw	$3,24($2)
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,17292
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	sw	$2,28($sp)
	lhu	$3,28($sp)
	lw	$2,48($sp)
	sh	$3,4($2)
	lhu	$3,30($sp)
	lw	$2,48($sp)
	sh	$3,6($2)
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,17264
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	sw	$2,24($sp)
	lhu	$3,24($sp)
	lw	$2,48($sp)
	sh	$3,8($2)
	lhu	$3,26($sp)
	lw	$2,48($sp)
	sh	$3,10($2)
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,17296
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	sw	$2,32($sp)
	lhu	$3,32($sp)
	lw	$2,48($sp)
	sh	$3,14($2)
	lhu	$3,34($sp)
	lw	$2,48($sp)
	sh	$3,16($2)
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,17260
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	sw	$2,20($sp)
	lhu	$3,20($sp)
	lw	$2,48($sp)
	sh	$3,18($2)
	lhu	$3,22($sp)
	lw	$2,48($sp)
	sh	$3,20($2)
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,17308
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	sw	$2,36($sp)
	lhu	$3,36($sp)
	lw	$2,48($sp)
	sh	$3,22($2)
	lhu	$3,38($sp)
	lw	$2,48($sp)
	sh	$3,12($2)
	lw	$2,48($sp)
	lhu	$2,12($2)
	move	$3,$2
	lw	$2,48($sp)
	sw	$3,0($2)
	move	$2,$0
	lw	$31,44($sp)
	addiu	$sp,$sp,48
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	gponDevGetSniffMode
	.size	gponDevGetSniffMode, .-gponDevGetSniffMode
	.rdata
	.align	2
$LC15:
	.ascii	"/opt/tclinux_phoenix/modules/private/xpon/src/gpon/gpon_"
	.ascii	"dev.c\000"
	.align	2
$LC16:
	.ascii	"[%lu0ms][%s:%s]\012  Fail to set AES key switch by sw.\012"
	.ascii	"\000"
	.section	.text.gponDevSetAesKeySwitchBySw,"ax",@progbits
	.align	2
	.globl	gponDevSetAesKeySwitchBySw
	.set	nomips16
	.set	nomicromips
	.ent	gponDevSetAesKeySwitchBySw
	.type	gponDevSetAesKeySwitchBySw, @function
gponDevSetAesKeySwitchBySw:
	.frame	$sp,32,$31		# vars= 8, regs= 2/0, args= 16, gp= 0
	.mask	0x80010000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-32
	sw	$31,28($sp)
	sw	$16,24($sp)
	li	$2,3000			# 0xbb8
	sw	$2,16($sp)
	lw	$2,20($sp)
	li	$3,1			# 0x1
	ins	$2,$3,0,1
	sw	$2,20($sp)
	lw	$3,20($sp)
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16516
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	j	$L135
	nop

$L138:
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16516
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	sw	$2,20($sp)
	lw	$3,20($sp)
	li	$2,-2147483648			# 0xffffffff80000000
	and	$2,$3,$2
	beq	$2,$0,$L136
	nop

	li	$2,1			# 0x1
	j	$L140
	nop

$L136:
	li	$4,1			# 0x1
	lui	$2,%hi(__udelay)
	addiu	$2,$2,%lo(__udelay)
	jalr	$2
	nop

$L135:
	lw	$2,16($sp)
	addiu	$3,$2,-1
	sw	$3,16($sp)
	bne	$2,$0,$L138
	nop

	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	lhu	$2,-30816($2)
	andi	$2,$2,0x1
	beq	$2,$0,$L139
	nop

	lui	$2,%hi(jiffies)
	lw	$16,%lo(jiffies)($2)
	lui	$2,%hi($LC15)
	addiu	$4,$2,%lo($LC15)
	li	$5,47			# 0x2f
	lui	$2,%hi(strrchr)
	addiu	$2,$2,%lo(strrchr)
	jalr	$2
	nop

	addiu	$3,$2,1
	lui	$2,%hi($LC16)
	addiu	$4,$2,%lo($LC16)
	move	$5,$16
	move	$6,$3
	lui	$2,%hi(__func__.31792)
	addiu	$7,$2,%lo(__func__.31792)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L139:
	move	$2,$0
$L140:
	lw	$31,28($sp)
	lw	$16,24($sp)
	addiu	$sp,$sp,32
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	gponDevSetAesKeySwitchBySw
	.size	gponDevSetAesKeySwitchBySw, .-gponDevSetAesKeySwitchBySw
	.section	.text.gponDevSwReset,"ax",@progbits
	.align	2
	.globl	gponDevSwReset
	.set	nomips16
	.set	nomicromips
	.ent	gponDevSwReset
	.type	gponDevSwReset, @function
gponDevSwReset:
	.frame	$sp,64,$31		# vars= 40, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-64
	sw	$31,60($sp)
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	addiu	$2,$2,460
	sw	$2,20($sp)
	lw	$2,20($sp)
	move	$4,$2
	lui	$2,%hi(_raw_spin_lock_irqsave)
	addiu	$2,$2,%lo(_raw_spin_lock_irqsave)
	jalr	$2
	nop

	sw	$2,16($sp)
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,17312
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	sw	$2,32($sp)
	lw	$2,32($sp)
	ins	$2,$0,0,1
	sw	$2,32($sp)
	lw	$3,32($sp)
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,17312
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	li	$4,1			# 0x1
	lui	$2,%hi(__udelay)
	addiu	$2,$2,%lo(__udelay)
	jalr	$2
	nop

	lw	$2,32($sp)
	li	$3,1			# 0x1
	ins	$2,$3,0,1
	sw	$2,32($sp)
	lw	$3,32($sp)
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,17312
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	addiu	$2,$2,460
	sw	$2,24($sp)
	lw	$2,16($sp)
	sw	$2,28($sp)
	lw	$2,24($sp)
	move	$4,$2
	lw	$5,28($sp)
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
	.end	gponDevSwReset
	.size	gponDevSwReset, .-gponDevSwReset
	.section	.text.gponDevSwResync,"ax",@progbits
	.align	2
	.globl	gponDevSwResync
	.set	nomips16
	.set	nomicromips
	.ent	gponDevSwResync
	.type	gponDevSwResync, @function
gponDevSwResync:
	.frame	$sp,32,$31		# vars= 8, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-32
	sw	$31,28($sp)
	lw	$2,16($sp)
	li	$3,1			# 0x1
	ins	$2,$3,31,1
	sw	$2,16($sp)
	lw	$2,16($sp)
	li	$3,1			# 0x1
	ins	$2,$3,0,1
	sw	$2,16($sp)
	lw	$3,16($sp)
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,17252
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	li	$4,1			# 0x1
	lui	$2,%hi(__udelay)
	addiu	$2,$2,%lo(__udelay)
	jalr	$2
	nop

	lw	$2,16($sp)
	ins	$2,$0,31,1
	sw	$2,16($sp)
	lw	$2,16($sp)
	ins	$2,$0,0,1
	sw	$2,16($sp)
	lw	$3,16($sp)
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,17252
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	move	$2,$0
	lw	$31,28($sp)
	addiu	$sp,$sp,32
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	gponDevSwResync
	.size	gponDevSwResync, .-gponDevSwResync
	.section	.text.gponDevSetDbaBackdoor,"ax",@progbits
	.align	2
	.globl	gponDevSetDbaBackdoor
	.set	nomips16
	.set	nomicromips
	.ent	gponDevSetDbaBackdoor
	.type	gponDevSetDbaBackdoor, @function
gponDevSetDbaBackdoor:
	.frame	$sp,32,$31		# vars= 8, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-32
	sw	$31,28($sp)
	sw	$4,32($sp)
	lw	$2,32($sp)
	beq	$2,$0,$L147
	nop

	lw	$3,32($sp)
	li	$2,1			# 0x1
	beq	$3,$2,$L147
	nop

	li	$2,-1			# 0xffffffffffffffff
	j	$L149
	nop

$L147:
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,17216
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	sw	$2,16($sp)
	lw	$2,32($sp)
	andi	$2,$2,0x00ff
	andi	$2,$2,0x1
	andi	$3,$2,0x00ff
	lw	$2,16($sp)
	ins	$2,$3,31,1
	sw	$2,16($sp)
	lw	$3,16($sp)
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,17216
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	move	$2,$0
$L149:
	lw	$31,28($sp)
	addiu	$sp,$sp,32
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	gponDevSetDbaBackdoor
	.size	gponDevSetDbaBackdoor, .-gponDevSetDbaBackdoor
	.section	.text.gponDevSetDbaBackdoorTotal,"ax",@progbits
	.align	2
	.globl	gponDevSetDbaBackdoorTotal
	.set	nomips16
	.set	nomicromips
	.ent	gponDevSetDbaBackdoorTotal
	.type	gponDevSetDbaBackdoorTotal, @function
gponDevSetDbaBackdoorTotal:
	.frame	$sp,32,$31		# vars= 8, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-32
	sw	$31,28($sp)
	sw	$4,32($sp)
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,17220
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	sw	$2,16($sp)
	lw	$2,32($sp)
	sw	$2,16($sp)
	lw	$3,16($sp)
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,17220
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	move	$2,$0
	lw	$31,28($sp)
	addiu	$sp,$sp,32
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	gponDevSetDbaBackdoorTotal
	.size	gponDevSetDbaBackdoorTotal, .-gponDevSetDbaBackdoorTotal
	.section	.text.gponDevSetDbaBackdoorGreen,"ax",@progbits
	.align	2
	.globl	gponDevSetDbaBackdoorGreen
	.set	nomips16
	.set	nomicromips
	.ent	gponDevSetDbaBackdoorGreen
	.type	gponDevSetDbaBackdoorGreen, @function
gponDevSetDbaBackdoorGreen:
	.frame	$sp,32,$31		# vars= 8, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-32
	sw	$31,28($sp)
	sw	$4,32($sp)
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,17224
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	sw	$2,16($sp)
	lw	$2,32($sp)
	sw	$2,16($sp)
	lw	$3,16($sp)
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,17224
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	move	$2,$0
	lw	$31,28($sp)
	addiu	$sp,$sp,32
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	gponDevSetDbaBackdoorGreen
	.size	gponDevSetDbaBackdoorGreen, .-gponDevSetDbaBackdoorGreen
	.section	.text.gponDevSetDbaBackdoorYellow,"ax",@progbits
	.align	2
	.globl	gponDevSetDbaBackdoorYellow
	.set	nomips16
	.set	nomicromips
	.ent	gponDevSetDbaBackdoorYellow
	.type	gponDevSetDbaBackdoorYellow, @function
gponDevSetDbaBackdoorYellow:
	.frame	$sp,32,$31		# vars= 8, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-32
	sw	$31,28($sp)
	sw	$4,32($sp)
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,17228
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	sw	$2,16($sp)
	lw	$2,32($sp)
	sw	$2,16($sp)
	lw	$3,16($sp)
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,17228
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	move	$2,$0
	lw	$31,28($sp)
	addiu	$sp,$sp,32
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	gponDevSetDbaBackdoorYellow
	.size	gponDevSetDbaBackdoorYellow, .-gponDevSetDbaBackdoorYellow
	.section	.text.gponDevGetDbaBackdoor,"ax",@progbits
	.align	2
	.globl	gponDevGetDbaBackdoor
	.set	nomips16
	.set	nomicromips
	.ent	gponDevGetDbaBackdoor
	.type	gponDevGetDbaBackdoor, @function
gponDevGetDbaBackdoor:
	.frame	$sp,40,$31		# vars= 16, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-40
	sw	$31,36($sp)
	sw	$4,40($sp)
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,17216
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	sw	$2,16($sp)
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,17220
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	sw	$2,20($sp)
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,17224
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	sw	$2,24($sp)
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,17228
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	sw	$2,28($sp)
	lw	$2,16($sp)
	ext	$2,$2,31,1
	andi	$2,$2,0x00ff
	move	$3,$2
	lw	$2,40($sp)
	sw	$3,12($2)
	lw	$3,20($sp)
	lw	$2,40($sp)
	sw	$3,0($2)
	lw	$3,24($sp)
	lw	$2,40($sp)
	sw	$3,4($2)
	lw	$3,28($sp)
	lw	$2,40($sp)
	sw	$3,8($2)
	move	$2,$0
	lw	$31,36($sp)
	addiu	$sp,$sp,40
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	gponDevGetDbaBackdoor
	.size	gponDevGetDbaBackdoor, .-gponDevGetDbaBackdoor
	.section	.text.gponDevSetDbaSlightModify,"ax",@progbits
	.align	2
	.globl	gponDevSetDbaSlightModify
	.set	nomips16
	.set	nomicromips
	.ent	gponDevSetDbaSlightModify
	.type	gponDevSetDbaSlightModify, @function
gponDevSetDbaSlightModify:
	.frame	$sp,32,$31		# vars= 8, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-32
	sw	$31,28($sp)
	sw	$4,32($sp)
	lw	$2,32($sp)
	beq	$2,$0,$L159
	nop

	lw	$3,32($sp)
	li	$2,1			# 0x1
	beq	$3,$2,$L159
	nop

	li	$2,-1			# 0xffffffffffffffff
	j	$L161
	nop

$L159:
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,17216
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	sw	$2,16($sp)
	lw	$2,32($sp)
	andi	$2,$2,0x00ff
	andi	$2,$2,0x1
	andi	$3,$2,0x00ff
	lw	$2,16($sp)
	ins	$2,$3,27,1
	sw	$2,16($sp)
	lw	$3,16($sp)
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,17216
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	move	$2,$0
$L161:
	lw	$31,28($sp)
	addiu	$sp,$sp,32
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	gponDevSetDbaSlightModify
	.size	gponDevSetDbaSlightModify, .-gponDevSetDbaSlightModify
	.section	.text.gponDevSetDbaSlightModifyTotal,"ax",@progbits
	.align	2
	.globl	gponDevSetDbaSlightModifyTotal
	.set	nomips16
	.set	nomicromips
	.ent	gponDevSetDbaSlightModifyTotal
	.type	gponDevSetDbaSlightModifyTotal, @function
gponDevSetDbaSlightModifyTotal:
	.frame	$sp,32,$31		# vars= 8, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-32
	sw	$31,28($sp)
	move	$2,$4
	sh	$2,32($sp)
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,17236
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	sw	$2,16($sp)
	lhu	$2,32($sp)
	sh	$2,18($sp)
	lw	$3,16($sp)
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,17236
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	move	$2,$0
	lw	$31,28($sp)
	addiu	$sp,$sp,32
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	gponDevSetDbaSlightModifyTotal
	.size	gponDevSetDbaSlightModifyTotal, .-gponDevSetDbaSlightModifyTotal
	.section	.text.gponDevSetDbaSlightModifyGreen,"ax",@progbits
	.align	2
	.globl	gponDevSetDbaSlightModifyGreen
	.set	nomips16
	.set	nomicromips
	.ent	gponDevSetDbaSlightModifyGreen
	.type	gponDevSetDbaSlightModifyGreen, @function
gponDevSetDbaSlightModifyGreen:
	.frame	$sp,32,$31		# vars= 8, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-32
	sw	$31,28($sp)
	move	$2,$4
	sh	$2,32($sp)
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,17232
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	sw	$2,16($sp)
	lhu	$2,32($sp)
	sh	$2,18($sp)
	lw	$3,16($sp)
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,17232
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	move	$2,$0
	lw	$31,28($sp)
	addiu	$sp,$sp,32
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	gponDevSetDbaSlightModifyGreen
	.size	gponDevSetDbaSlightModifyGreen, .-gponDevSetDbaSlightModifyGreen
	.section	.text.gponDevSetDbaSlightModifyYellow,"ax",@progbits
	.align	2
	.globl	gponDevSetDbaSlightModifyYellow
	.set	nomips16
	.set	nomicromips
	.ent	gponDevSetDbaSlightModifyYellow
	.type	gponDevSetDbaSlightModifyYellow, @function
gponDevSetDbaSlightModifyYellow:
	.frame	$sp,32,$31		# vars= 8, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-32
	sw	$31,28($sp)
	move	$2,$4
	sh	$2,32($sp)
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,17232
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	sw	$2,16($sp)
	lhu	$2,32($sp)
	sh	$2,16($sp)
	lw	$3,16($sp)
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,17232
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	move	$2,$0
	lw	$31,28($sp)
	addiu	$sp,$sp,32
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	gponDevSetDbaSlightModifyYellow
	.size	gponDevSetDbaSlightModifyYellow, .-gponDevSetDbaSlightModifyYellow
	.section	.text.gponDevGetDbaSlightModify,"ax",@progbits
	.align	2
	.globl	gponDevGetDbaSlightModify
	.set	nomips16
	.set	nomicromips
	.ent	gponDevGetDbaSlightModify
	.type	gponDevGetDbaSlightModify, @function
gponDevGetDbaSlightModify:
	.frame	$sp,40,$31		# vars= 16, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-40
	sw	$31,36($sp)
	sw	$4,40($sp)
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,17216
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	sw	$2,16($sp)
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,17232
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	sw	$2,20($sp)
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,17236
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	sw	$2,24($sp)
	lw	$2,16($sp)
	ext	$2,$2,27,1
	andi	$2,$2,0x00ff
	move	$3,$2
	lw	$2,40($sp)
	sw	$3,8($2)
	lhu	$3,26($sp)
	lw	$2,40($sp)
	sh	$3,0($2)
	lhu	$3,22($sp)
	lw	$2,40($sp)
	sh	$3,2($2)
	lhu	$3,20($sp)
	lw	$2,40($sp)
	sh	$3,4($2)
	move	$2,$0
	lw	$31,36($sp)
	addiu	$sp,$sp,40
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	gponDevGetDbaSlightModify
	.size	gponDevGetDbaSlightModify, .-gponDevGetDbaSlightModify
	.section	.text.gponDevSetBwmStopTimeInt,"ax",@progbits
	.align	2
	.globl	gponDevSetBwmStopTimeInt
	.set	nomips16
	.set	nomicromips
	.ent	gponDevSetBwmStopTimeInt
	.type	gponDevSetBwmStopTimeInt, @function
gponDevSetBwmStopTimeInt:
	.frame	$sp,32,$31		# vars= 8, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-32
	sw	$31,28($sp)
	sw	$4,32($sp)
	lw	$2,32($sp)
	beq	$2,$0,$L171
	nop

	lw	$3,32($sp)
	li	$2,1			# 0x1
	beq	$3,$2,$L171
	nop

	li	$2,-1			# 0xffffffffffffffff
	j	$L174
	nop

$L171:
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16396
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	sw	$2,16($sp)
	lw	$3,32($sp)
	li	$2,1			# 0x1
	bne	$3,$2,$L173
	nop

	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16392
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	sw	$2,20($sp)
	lw	$2,20($sp)
	li	$3,1			# 0x1
	ins	$2,$3,30,1
	sw	$2,20($sp)
	lw	$3,20($sp)
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16392
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

$L173:
	lw	$2,32($sp)
	andi	$2,$2,0x00ff
	andi	$2,$2,0x1
	andi	$3,$2,0x00ff
	lw	$2,16($sp)
	ins	$2,$3,30,1
	sw	$2,16($sp)
	lw	$3,16($sp)
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16396
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	move	$2,$0
$L174:
	lw	$31,28($sp)
	addiu	$sp,$sp,32
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	gponDevSetBwmStopTimeInt
	.size	gponDevSetBwmStopTimeInt, .-gponDevSetBwmStopTimeInt
	.section	.text.gponDevSetTx4bytesAlign,"ax",@progbits
	.align	2
	.globl	gponDevSetTx4bytesAlign
	.set	nomips16
	.set	nomicromips
	.ent	gponDevSetTx4bytesAlign
	.type	gponDevSetTx4bytesAlign, @function
gponDevSetTx4bytesAlign:
	.frame	$sp,32,$31		# vars= 8, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-32
	sw	$31,28($sp)
	sw	$4,32($sp)
	lw	$2,32($sp)
	beq	$2,$0,$L176
	nop

	lw	$3,32($sp)
	li	$2,1			# 0x1
	beq	$3,$2,$L176
	nop

	li	$2,-1			# 0xffffffffffffffff
	j	$L178
	nop

$L176:
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,17312
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	sw	$2,16($sp)
	lw	$2,32($sp)
	andi	$2,$2,0x00ff
	andi	$2,$2,0x1
	andi	$3,$2,0x00ff
	lw	$2,16($sp)
	ins	$2,$3,31,1
	sw	$2,16($sp)
	lw	$3,16($sp)
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,17312
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	move	$2,$0
$L178:
	lw	$31,28($sp)
	addiu	$sp,$sp,32
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	gponDevSetTx4bytesAlign
	.size	gponDevSetTx4bytesAlign, .-gponDevSetTx4bytesAlign
	.section	.text.gponDevGetTx4bytesAlign,"ax",@progbits
	.align	2
	.globl	gponDevGetTx4bytesAlign
	.set	nomips16
	.set	nomicromips
	.ent	gponDevGetTx4bytesAlign
	.type	gponDevGetTx4bytesAlign, @function
gponDevGetTx4bytesAlign:
	.frame	$sp,32,$31		# vars= 8, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-32
	sw	$31,28($sp)
	sw	$4,32($sp)
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,17312
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	sw	$2,16($sp)
	lw	$2,16($sp)
	ext	$2,$2,31,1
	andi	$2,$2,0x00ff
	move	$3,$2
	lw	$2,32($sp)
	sw	$3,0($2)
	move	$2,$0
	lw	$31,28($sp)
	addiu	$sp,$sp,32
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	gponDevGetTx4bytesAlign
	.size	gponDevGetTx4bytesAlign, .-gponDevGetTx4bytesAlign
	.section	.text.gponDevGetTxSyncOffset,"ax",@progbits
	.align	2
	.globl	gponDevGetTxSyncOffset
	.set	nomips16
	.set	nomicromips
	.ent	gponDevGetTxSyncOffset
	.type	gponDevGetTxSyncOffset, @function
gponDevGetTxSyncOffset:
	.frame	$sp,32,$31		# vars= 8, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-32
	sw	$31,28($sp)
	sw	$4,32($sp)
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,17244
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	sw	$2,16($sp)
	lw	$2,16($sp)
	ext	$2,$2,0,2
	andi	$2,$2,0x00ff
	move	$3,$2
	lw	$2,32($sp)
	sb	$3,0($2)
	move	$2,$0
	lw	$31,28($sp)
	addiu	$sp,$sp,32
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	gponDevGetTxSyncOffset
	.size	gponDevGetTxSyncOffset, .-gponDevGetTxSyncOffset
	.section	.text.gponDevGetO3O4PloamCtrl,"ax",@progbits
	.align	2
	.globl	gponDevGetO3O4PloamCtrl
	.set	nomips16
	.set	nomicromips
	.ent	gponDevGetO3O4PloamCtrl
	.type	gponDevGetO3O4PloamCtrl, @function
gponDevGetO3O4PloamCtrl:
	.frame	$sp,32,$31		# vars= 8, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-32
	sw	$31,28($sp)
	sw	$4,32($sp)
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,17348
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	sw	$2,16($sp)
	lw	$2,16($sp)
	andi	$2,$2,0x1
	sltu	$2,$2,1
	andi	$2,$2,0x00ff
	move	$3,$2
	lw	$2,32($sp)
	sw	$3,0($2)
	move	$2,$0
	lw	$31,28($sp)
	addiu	$sp,$sp,32
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	gponDevGetO3O4PloamCtrl
	.size	gponDevGetO3O4PloamCtrl, .-gponDevGetO3O4PloamCtrl
	.section	.text.gponDevSetO3O4PloamCtrl,"ax",@progbits
	.align	2
	.globl	gponDevSetO3O4PloamCtrl
	.set	nomips16
	.set	nomicromips
	.ent	gponDevSetO3O4PloamCtrl
	.type	gponDevSetO3O4PloamCtrl, @function
gponDevSetO3O4PloamCtrl:
	.frame	$sp,32,$31		# vars= 8, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-32
	sw	$31,28($sp)
	sw	$4,32($sp)
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,17348
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	sw	$2,16($sp)
	lw	$2,32($sp)
	xori	$2,$2,0x1
	sltu	$2,$0,$2
	andi	$3,$2,0x00ff
	lw	$2,16($sp)
	ins	$2,$3,0,1
	sw	$2,16($sp)
	lw	$3,16($sp)
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,17348
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	move	$2,$0
	lw	$31,28($sp)
	addiu	$sp,$sp,32
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	gponDevSetO3O4PloamCtrl
	.size	gponDevSetO3O4PloamCtrl, .-gponDevSetO3O4PloamCtrl
	.rdata
	.align	2
$LC17:
	.ascii	"[%lu0ms][%s:%s]\012  Fail to set tcont:%d.\000"
	.section	.text.gponDevSetTCont16_31,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	gponDevSetTCont16_31
	.type	gponDevSetTCont16_31, @function
gponDevSetTCont16_31:
	.frame	$sp,48,$31		# vars= 16, regs= 2/0, args= 24, gp= 0
	.mask	0x80010000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-48
	sw	$31,44($sp)
	sw	$16,40($sp)
	sw	$4,48($sp)
	sw	$5,52($sp)
	move	$2,$6
	sh	$2,56($sp)
	li	$2,3000			# 0xbb8
	sw	$2,24($sp)
	lw	$2,52($sp)
	slt	$2,$2,16
	bne	$2,$0,$L188
	nop

	lw	$2,52($sp)
	slt	$2,$2,32
	bne	$2,$0,$L189
	nop

$L188:
	li	$2,1			# 0x1
	j	$L195
	nop

$L189:
	lw	$2,28($sp)
	li	$3,1			# 0x1
	ins	$2,$3,31,1
	sw	$2,28($sp)
	lw	$2,48($sp)
	andi	$2,$2,0x00ff
	andi	$2,$2,0x1
	andi	$3,$2,0x00ff
	lw	$2,28($sp)
	ins	$2,$3,27,1
	sw	$2,28($sp)
	lw	$2,52($sp)
	andi	$2,$2,0x00ff
	addiu	$2,$2,-16
	andi	$2,$2,0x00ff
	andi	$2,$2,0xf
	andi	$3,$2,0x00ff
	lw	$2,28($sp)
	ins	$2,$3,16,4
	sw	$2,28($sp)
	lhu	$2,56($sp)
	andi	$2,$2,0xfff
	andi	$3,$2,0xffff
	lw	$2,28($sp)
	ins	$2,$3,0,12
	sw	$2,28($sp)
	lw	$3,28($sp)
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16768
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	j	$L191
	nop

$L193:
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16772
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	sw	$2,32($sp)
	lw	$3,32($sp)
	li	$2,-2147483648			# 0xffffffff80000000
	and	$2,$3,$2
	beq	$2,$0,$L192
	nop

	move	$2,$0
	j	$L195
	nop

$L192:
	li	$4,1			# 0x1
	lui	$2,%hi(__udelay)
	addiu	$2,$2,%lo(__udelay)
	jalr	$2
	nop

$L191:
	lw	$2,24($sp)
	addiu	$3,$2,-1
	sw	$3,24($sp)
	bne	$2,$0,$L193
	nop

	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	lhu	$2,-30816($2)
	andi	$2,$2,0x1
	beq	$2,$0,$L194
	nop

	lui	$2,%hi(jiffies)
	lw	$16,%lo(jiffies)($2)
	lui	$2,%hi($LC15)
	addiu	$4,$2,%lo($LC15)
	li	$5,47			# 0x2f
	lui	$2,%hi(strrchr)
	addiu	$2,$2,%lo(strrchr)
	jalr	$2
	nop

	addiu	$3,$2,1
	lw	$2,52($sp)
	sw	$2,16($sp)
	lui	$2,%hi($LC17)
	addiu	$4,$2,%lo($LC17)
	move	$5,$16
	move	$6,$3
	lui	$2,%hi(__func__.31886)
	addiu	$7,$2,%lo(__func__.31886)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L194:
	li	$2,1			# 0x1
$L195:
	lw	$31,44($sp)
	lw	$16,40($sp)
	addiu	$sp,$sp,48
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	gponDevSetTCont16_31
	.size	gponDevSetTCont16_31, .-gponDevSetTCont16_31
	.rdata
	.align	2
$LC18:
	.ascii	"[%lu0ms][%s:%s]\012  Fail to get tcont:%d.\000"
	.section	.text.gponDevGetTCont16_31,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	gponDevGetTCont16_31
	.type	gponDevGetTCont16_31, @function
gponDevGetTCont16_31:
	.frame	$sp,48,$31		# vars= 16, regs= 2/0, args= 24, gp= 0
	.mask	0x80010000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-48
	sw	$31,44($sp)
	sw	$16,40($sp)
	sw	$4,48($sp)
	sw	$5,52($sp)
	sw	$6,56($sp)
	li	$2,3000			# 0xbb8
	sw	$2,24($sp)
	lw	$2,52($sp)
	slt	$2,$2,16
	bne	$2,$0,$L197
	nop

	lw	$2,52($sp)
	slt	$2,$2,32
	bne	$2,$0,$L198
	nop

$L197:
	li	$2,1			# 0x1
	j	$L204
	nop

$L198:
	lw	$2,28($sp)
	ins	$2,$0,31,1
	sw	$2,28($sp)
	lw	$2,52($sp)
	andi	$2,$2,0x00ff
	addiu	$2,$2,-16
	andi	$2,$2,0x00ff
	andi	$2,$2,0xf
	andi	$3,$2,0x00ff
	lw	$2,28($sp)
	ins	$2,$3,16,4
	sw	$2,28($sp)
	lw	$3,28($sp)
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16768
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	j	$L200
	nop

$L202:
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16772
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	sw	$2,32($sp)
	lw	$3,32($sp)
	li	$2,-2147483648			# 0xffffffff80000000
	and	$2,$3,$2
	beq	$2,$0,$L201
	nop

	lw	$2,32($sp)
	ext	$2,$2,16,1
	andi	$2,$2,0x00ff
	move	$3,$2
	lw	$2,48($sp)
	sw	$3,0($2)
	lw	$2,32($sp)
	ext	$2,$2,0,12
	andi	$2,$2,0xffff
	move	$3,$2
	lw	$2,56($sp)
	sh	$3,0($2)
	move	$2,$0
	j	$L204
	nop

$L201:
	li	$4,1			# 0x1
	lui	$2,%hi(__udelay)
	addiu	$2,$2,%lo(__udelay)
	jalr	$2
	nop

$L200:
	lw	$2,24($sp)
	addiu	$3,$2,-1
	sw	$3,24($sp)
	bne	$2,$0,$L202
	nop

	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	lhu	$2,-30816($2)
	andi	$2,$2,0x1
	beq	$2,$0,$L203
	nop

	lui	$2,%hi(jiffies)
	lw	$16,%lo(jiffies)($2)
	lui	$2,%hi($LC15)
	addiu	$4,$2,%lo($LC15)
	li	$5,47			# 0x2f
	lui	$2,%hi(strrchr)
	addiu	$2,$2,%lo(strrchr)
	jalr	$2
	nop

	addiu	$3,$2,1
	lw	$2,52($sp)
	sw	$2,16($sp)
	lui	$2,%hi($LC18)
	addiu	$4,$2,%lo($LC18)
	move	$5,$16
	move	$6,$3
	lui	$2,%hi(__func__.31898)
	addiu	$7,$2,%lo(__func__.31898)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L203:
	li	$2,1			# 0x1
$L204:
	lw	$31,44($sp)
	lw	$16,40($sp)
	addiu	$sp,$sp,48
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	gponDevGetTCont16_31
	.size	gponDevGetTCont16_31, .-gponDevGetTCont16_31
	.section	.text.gponDevSetFilterUpstreamOverheadPLOAM,"ax",@progbits
	.align	2
	.globl	gponDevSetFilterUpstreamOverheadPLOAM
	.set	nomips16
	.set	nomicromips
	.ent	gponDevSetFilterUpstreamOverheadPLOAM
	.type	gponDevSetFilterUpstreamOverheadPLOAM, @function
gponDevSetFilterUpstreamOverheadPLOAM:
	.frame	$sp,32,$31		# vars= 8, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-32
	sw	$31,28($sp)
	sw	$4,32($sp)
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,17248
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	sw	$2,16($sp)
	lw	$2,32($sp)
	xori	$2,$2,0x1
	sltu	$2,$2,1
	andi	$3,$2,0x00ff
	lw	$2,16($sp)
	ins	$2,$3,0,1
	sw	$2,16($sp)
	lw	$3,16($sp)
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,17248
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	move	$2,$0
	lw	$31,28($sp)
	addiu	$sp,$sp,32
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	gponDevSetFilterUpstreamOverheadPLOAM
	.size	gponDevSetFilterUpstreamOverheadPLOAM, .-gponDevSetFilterUpstreamOverheadPLOAM
	.section	.text.gponDevGetFilterUpstreamOverheadPLOAM,"ax",@progbits
	.align	2
	.globl	gponDevGetFilterUpstreamOverheadPLOAM
	.set	nomips16
	.set	nomicromips
	.ent	gponDevGetFilterUpstreamOverheadPLOAM
	.type	gponDevGetFilterUpstreamOverheadPLOAM, @function
gponDevGetFilterUpstreamOverheadPLOAM:
	.frame	$sp,32,$31		# vars= 8, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-32
	sw	$31,28($sp)
	sw	$4,32($sp)
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,17248
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	sw	$2,16($sp)
	lw	$2,16($sp)
	ext	$2,$2,0,1
	andi	$2,$2,0x00ff
	move	$3,$2
	lw	$2,32($sp)
	sw	$3,0($2)
	move	$2,$0
	lw	$31,28($sp)
	addiu	$sp,$sp,32
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	gponDevGetFilterUpstreamOverheadPLOAM
	.size	gponDevGetFilterUpstreamOverheadPLOAM, .-gponDevGetFilterUpstreamOverheadPLOAM
	.section	.text.gponDevSetFilterExtBurstLengthPLOAM,"ax",@progbits
	.align	2
	.globl	gponDevSetFilterExtBurstLengthPLOAM
	.set	nomips16
	.set	nomicromips
	.ent	gponDevSetFilterExtBurstLengthPLOAM
	.type	gponDevSetFilterExtBurstLengthPLOAM, @function
gponDevSetFilterExtBurstLengthPLOAM:
	.frame	$sp,32,$31		# vars= 8, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-32
	sw	$31,28($sp)
	sw	$4,32($sp)
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,17248
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	sw	$2,16($sp)
	lw	$2,32($sp)
	xori	$2,$2,0x1
	sltu	$2,$2,1
	andi	$3,$2,0x00ff
	lw	$2,16($sp)
	ins	$2,$3,8,1
	sw	$2,16($sp)
	lw	$3,16($sp)
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,17248
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	move	$2,$0
	lw	$31,28($sp)
	addiu	$sp,$sp,32
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	gponDevSetFilterExtBurstLengthPLOAM
	.size	gponDevSetFilterExtBurstLengthPLOAM, .-gponDevSetFilterExtBurstLengthPLOAM
	.section	.text.gponDevGetFilterExtBurstLengthPLOAM,"ax",@progbits
	.align	2
	.globl	gponDevGetFilterExtBurstLengthPLOAM
	.set	nomips16
	.set	nomicromips
	.ent	gponDevGetFilterExtBurstLengthPLOAM
	.type	gponDevGetFilterExtBurstLengthPLOAM, @function
gponDevGetFilterExtBurstLengthPLOAM:
	.frame	$sp,32,$31		# vars= 8, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-32
	sw	$31,28($sp)
	sw	$4,32($sp)
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,17248
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	sw	$2,16($sp)
	lw	$2,16($sp)
	ext	$2,$2,8,1
	andi	$2,$2,0x00ff
	move	$3,$2
	lw	$2,32($sp)
	sw	$3,0($2)
	move	$2,$0
	lw	$31,28($sp)
	addiu	$sp,$sp,32
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	gponDevGetFilterExtBurstLengthPLOAM
	.size	gponDevGetFilterExtBurstLengthPLOAM, .-gponDevGetFilterExtBurstLengthPLOAM
	.section	.text.gponDevSet1ppsHighWidth,"ax",@progbits
	.align	2
	.globl	gponDevSet1ppsHighWidth
	.set	nomips16
	.set	nomicromips
	.ent	gponDevSet1ppsHighWidth
	.type	gponDevSet1ppsHighWidth, @function
gponDevSet1ppsHighWidth:
	.frame	$sp,32,$31		# vars= 8, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-32
	sw	$31,28($sp)
	sw	$4,32($sp)
	li	$2,-1078984704			# 0xffffffffbfb00000
	ori	$2,$2,0x64
	lw	$3,0($2)
	li	$2,-65536			# 0xffffffffffff0000
	and	$3,$3,$2
	li	$2,524288			# 0x80000
	beq	$3,$2,$L214
	nop

	li	$2,-1			# 0xffffffffffffffff
	j	$L217
	nop

$L214:
	lw	$3,32($sp)
	li	$2,983040			# 0xf0000
	ori	$2,$2,0x4240
	sltu	$2,$3,$2
	bne	$2,$0,$L216
	nop

	li	$2,-1			# 0xffffffffffffffff
	j	$L217
	nop

$L216:
	lw	$4,32($sp)
	move	$3,$4
	sll	$2,$3,2
	move	$3,$2
	sll	$2,$3,5
	subu	$2,$2,$3
	addu	$2,$2,$4
	sll	$2,$2,3
	sw	$2,32($sp)
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16612
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	sw	$2,16($sp)
	lbu	$2,19($sp)
	move	$3,$2
	lw	$2,32($sp)
	divu	$0,$2,$3
	mfhi	$3
	mflo	$2
	sw	$2,20($sp)
	lw	$3,20($sp)
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,17352
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	move	$2,$0
$L217:
	lw	$31,28($sp)
	addiu	$sp,$sp,32
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	gponDevSet1ppsHighWidth
	.size	gponDevSet1ppsHighWidth, .-gponDevSet1ppsHighWidth
	.section	.text.gponDevGet1ppsHighWidth,"ax",@progbits
	.align	2
	.globl	gponDevGet1ppsHighWidth
	.set	nomips16
	.set	nomicromips
	.ent	gponDevGet1ppsHighWidth
	.type	gponDevGet1ppsHighWidth, @function
gponDevGet1ppsHighWidth:
	.frame	$sp,32,$31		# vars= 8, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-32
	sw	$31,28($sp)
	sw	$4,32($sp)
	li	$2,-1078984704			# 0xffffffffbfb00000
	ori	$2,$2,0x64
	lw	$3,0($2)
	li	$2,-65536			# 0xffffffffffff0000
	and	$3,$3,$2
	li	$2,524288			# 0x80000
	beq	$3,$2,$L219
	nop

	move	$2,$0
	j	$L221
	nop

$L219:
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16612
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	sw	$2,16($sp)
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,17352
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	sw	$2,20($sp)
	lw	$2,20($sp)
	lbu	$3,19($sp)
	mul	$3,$2,$3
	lw	$2,32($sp)
	sw	$3,0($2)
	lw	$2,32($sp)
	lw	$3,0($2)
	li	$2,274857984			# 0x10620000
	ori	$2,$2,0x4dd3
	multu	$3,$2
	mfhi	$2
	srl	$3,$2,6
	lw	$2,32($sp)
	sw	$3,0($2)
	move	$2,$0
$L221:
	lw	$31,28($sp)
	addiu	$sp,$sp,32
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	gponDevGet1ppsHighWidth
	.size	gponDevGet1ppsHighWidth, .-gponDevGet1ppsHighWidth
	.section	.text.gponDevSetSendPloamuWaitMode,"ax",@progbits
	.align	2
	.globl	gponDevSetSendPloamuWaitMode
	.set	nomips16
	.set	nomicromips
	.ent	gponDevSetSendPloamuWaitMode
	.type	gponDevSetSendPloamuWaitMode, @function
gponDevSetSendPloamuWaitMode:
	.frame	$sp,32,$31		# vars= 8, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-32
	sw	$31,28($sp)
	sw	$4,32($sp)
	li	$2,-1078984704			# 0xffffffffbfb00000
	ori	$2,$2,0x64
	lw	$3,0($2)
	li	$2,-65536			# 0xffffffffffff0000
	and	$3,$3,$2
	li	$2,524288			# 0x80000
	beq	$3,$2,$L223
	nop

	move	$2,$0
	j	$L225
	nop

$L223:
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,17348
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	sw	$2,16($sp)
	lw	$2,32($sp)
	andi	$2,$2,0x00ff
	andi	$2,$2,0x1
	andi	$3,$2,0x00ff
	lw	$2,16($sp)
	ins	$2,$3,8,1
	sw	$2,16($sp)
	lw	$3,16($sp)
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,17348
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	move	$2,$0
$L225:
	lw	$31,28($sp)
	addiu	$sp,$sp,32
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	gponDevSetSendPloamuWaitMode
	.size	gponDevSetSendPloamuWaitMode, .-gponDevSetSendPloamuWaitMode
	.section	.text.gponDevGetSendPloamuWaitMode,"ax",@progbits
	.align	2
	.globl	gponDevGetSendPloamuWaitMode
	.set	nomips16
	.set	nomicromips
	.ent	gponDevGetSendPloamuWaitMode
	.type	gponDevGetSendPloamuWaitMode, @function
gponDevGetSendPloamuWaitMode:
	.frame	$sp,32,$31		# vars= 8, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-32
	sw	$31,28($sp)
	sw	$4,32($sp)
	li	$2,-1078984704			# 0xffffffffbfb00000
	ori	$2,$2,0x64
	lw	$3,0($2)
	li	$2,-65536			# 0xffffffffffff0000
	and	$3,$3,$2
	li	$2,524288			# 0x80000
	beq	$3,$2,$L227
	nop

	move	$2,$0
	j	$L229
	nop

$L227:
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,17348
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	sw	$2,16($sp)
	lw	$2,16($sp)
	ext	$2,$2,8,1
	andi	$2,$2,0x00ff
	move	$3,$2
	lw	$2,32($sp)
	sw	$3,0($2)
	move	$2,$0
$L229:
	lw	$31,28($sp)
	addiu	$sp,$sp,32
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	gponDevGetSendPloamuWaitMode
	.size	gponDevGetSendPloamuWaitMode, .-gponDevGetSendPloamuWaitMode
	.section	.text.gponDevSetTodClkPeriod,"ax",@progbits
	.align	2
	.globl	gponDevSetTodClkPeriod
	.set	nomips16
	.set	nomicromips
	.ent	gponDevSetTodClkPeriod
	.type	gponDevSetTodClkPeriod, @function
gponDevSetTodClkPeriod:
	.frame	$sp,32,$31		# vars= 8, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-32
	sw	$31,28($sp)
	move	$2,$4
	sh	$2,32($sp)
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16612
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	sw	$2,16($sp)
	lhu	$2,32($sp)
	andi	$2,$2,0x00ff
	sb	$2,19($sp)
	lw	$3,16($sp)
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16612
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	move	$2,$0
	lw	$31,28($sp)
	addiu	$sp,$sp,32
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	gponDevSetTodClkPeriod
	.size	gponDevSetTodClkPeriod, .-gponDevSetTodClkPeriod
	.section	.text.gponDevSetIntMask,"ax",@progbits
	.align	2
	.globl	gponDevSetIntMask
	.set	nomips16
	.set	nomicromips
	.ent	gponDevSetIntMask
	.type	gponDevSetIntMask, @function
gponDevSetIntMask:
	.frame	$sp,24,$31		# vars= 0, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-24
	sw	$31,20($sp)
	sw	$4,24($sp)
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16396
	lw	$4,24($sp)
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	move	$2,$0
	lw	$31,20($sp)
	addiu	$sp,$sp,24
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	gponDevSetIntMask
	.size	gponDevSetIntMask, .-gponDevSetIntMask
	.section	.text.gponDevGetIntMask,"ax",@progbits
	.align	2
	.globl	gponDevGetIntMask
	.set	nomips16
	.set	nomicromips
	.ent	gponDevGetIntMask
	.type	gponDevGetIntMask, @function
gponDevGetIntMask:
	.frame	$sp,32,$31		# vars= 8, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-32
	sw	$31,28($sp)
	sw	$4,32($sp)
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16396
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	sw	$2,16($sp)
	lw	$3,16($sp)
	lw	$2,32($sp)
	sw	$3,0($2)
	move	$2,$0
	lw	$31,28($sp)
	addiu	$sp,$sp,32
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	gponDevGetIntMask
	.size	gponDevGetIntMask, .-gponDevGetIntMask
	.section	.text.gponDevSetTCont0_15,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	gponDevSetTCont0_15
	.type	gponDevSetTCont0_15, @function
gponDevSetTCont0_15:
	.frame	$sp,56,$31		# vars= 32, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-56
	sw	$31,52($sp)
	sw	$4,56($sp)
	sw	$5,60($sp)
	move	$2,$6
	sh	$2,64($sp)
	lw	$2,60($sp)
	bltz	$2,$L237
	nop

	lw	$2,60($sp)
	slt	$2,$2,16
	bne	$2,$0,$L238
	nop

$L237:
	li	$2,1			# 0x1
	j	$L258
	nop

$L238:
	lw	$2,60($sp)
	sltu	$2,$2,16
	beq	$2,$0,$L240
	nop

	lw	$2,60($sp)
	sll	$3,$2,2
	lui	$2,%hi($L242)
	addiu	$2,$2,%lo($L242)
	addu	$2,$3,$2
	lw	$2,0($2)
	j	$2
	nop

	.rdata
	.align	2
	.align	2
$L242:
	.word	$L241
	.word	$L243
	.word	$L244
	.word	$L245
	.word	$L246
	.word	$L247
	.word	$L248
	.word	$L249
	.word	$L250
	.word	$L251
	.word	$L252
	.word	$L253
	.word	$L254
	.word	$L255
	.word	$L256
	.word	$L257
	.section	.text.gponDevSetTCont0_15
$L241:
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16416
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	sw	$2,16($sp)
	lw	$2,56($sp)
	andi	$2,$2,0x00ff
	andi	$2,$2,0x1
	andi	$3,$2,0x00ff
	lw	$2,16($sp)
	ins	$2,$3,15,1
	sw	$2,16($sp)
	lhu	$2,64($sp)
	andi	$2,$2,0xfff
	andi	$3,$2,0xffff
	lw	$2,16($sp)
	ins	$2,$3,0,12
	sw	$2,16($sp)
	lw	$3,16($sp)
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16416
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	j	$L240
	nop

$L243:
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16416
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	sw	$2,16($sp)
	lw	$2,56($sp)
	andi	$2,$2,0x00ff
	andi	$2,$2,0x1
	andi	$3,$2,0x00ff
	lw	$2,16($sp)
	ins	$2,$3,31,1
	sw	$2,16($sp)
	lhu	$2,64($sp)
	andi	$2,$2,0xfff
	andi	$3,$2,0xffff
	lw	$2,16($sp)
	ins	$2,$3,16,12
	sw	$2,16($sp)
	lw	$3,16($sp)
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16416
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	j	$L240
	nop

$L244:
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16420
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	sw	$2,20($sp)
	lw	$2,56($sp)
	andi	$2,$2,0x00ff
	andi	$2,$2,0x1
	andi	$3,$2,0x00ff
	lw	$2,20($sp)
	ins	$2,$3,15,1
	sw	$2,20($sp)
	lhu	$2,64($sp)
	andi	$2,$2,0xfff
	andi	$3,$2,0xffff
	lw	$2,20($sp)
	ins	$2,$3,0,12
	sw	$2,20($sp)
	lw	$3,20($sp)
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16420
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	j	$L240
	nop

$L245:
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16420
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	sw	$2,20($sp)
	lw	$2,56($sp)
	andi	$2,$2,0x00ff
	andi	$2,$2,0x1
	andi	$3,$2,0x00ff
	lw	$2,20($sp)
	ins	$2,$3,31,1
	sw	$2,20($sp)
	lhu	$2,64($sp)
	andi	$2,$2,0xfff
	andi	$3,$2,0xffff
	lw	$2,20($sp)
	ins	$2,$3,16,12
	sw	$2,20($sp)
	lw	$3,20($sp)
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16420
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	j	$L240
	nop

$L246:
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16424
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	sw	$2,24($sp)
	lw	$2,56($sp)
	andi	$2,$2,0x00ff
	andi	$2,$2,0x1
	andi	$3,$2,0x00ff
	lw	$2,24($sp)
	ins	$2,$3,15,1
	sw	$2,24($sp)
	lhu	$2,64($sp)
	andi	$2,$2,0xfff
	andi	$3,$2,0xffff
	lw	$2,24($sp)
	ins	$2,$3,0,12
	sw	$2,24($sp)
	lw	$3,24($sp)
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16424
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	j	$L240
	nop

$L247:
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16424
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	sw	$2,24($sp)
	lw	$2,56($sp)
	andi	$2,$2,0x00ff
	andi	$2,$2,0x1
	andi	$3,$2,0x00ff
	lw	$2,24($sp)
	ins	$2,$3,31,1
	sw	$2,24($sp)
	lhu	$2,64($sp)
	andi	$2,$2,0xfff
	andi	$3,$2,0xffff
	lw	$2,24($sp)
	ins	$2,$3,16,12
	sw	$2,24($sp)
	lw	$3,24($sp)
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16424
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	j	$L240
	nop

$L248:
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16428
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	sw	$2,28($sp)
	lw	$2,56($sp)
	andi	$2,$2,0x00ff
	andi	$2,$2,0x1
	andi	$3,$2,0x00ff
	lw	$2,28($sp)
	ins	$2,$3,15,1
	sw	$2,28($sp)
	lhu	$2,64($sp)
	andi	$2,$2,0xfff
	andi	$3,$2,0xffff
	lw	$2,28($sp)
	ins	$2,$3,0,12
	sw	$2,28($sp)
	lw	$3,28($sp)
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16428
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	j	$L240
	nop

$L249:
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16428
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	sw	$2,28($sp)
	lw	$2,56($sp)
	andi	$2,$2,0x00ff
	andi	$2,$2,0x1
	andi	$3,$2,0x00ff
	lw	$2,28($sp)
	ins	$2,$3,31,1
	sw	$2,28($sp)
	lhu	$2,64($sp)
	andi	$2,$2,0xfff
	andi	$3,$2,0xffff
	lw	$2,28($sp)
	ins	$2,$3,16,12
	sw	$2,28($sp)
	lw	$3,28($sp)
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16428
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	j	$L240
	nop

$L250:
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16432
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	sw	$2,32($sp)
	lw	$2,56($sp)
	andi	$2,$2,0x00ff
	andi	$2,$2,0x1
	andi	$3,$2,0x00ff
	lw	$2,32($sp)
	ins	$2,$3,15,1
	sw	$2,32($sp)
	lhu	$2,64($sp)
	andi	$2,$2,0xfff
	andi	$3,$2,0xffff
	lw	$2,32($sp)
	ins	$2,$3,0,12
	sw	$2,32($sp)
	lw	$3,32($sp)
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16432
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	j	$L240
	nop

$L251:
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16432
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	sw	$2,32($sp)
	lw	$2,56($sp)
	andi	$2,$2,0x00ff
	andi	$2,$2,0x1
	andi	$3,$2,0x00ff
	lw	$2,32($sp)
	ins	$2,$3,31,1
	sw	$2,32($sp)
	lhu	$2,64($sp)
	andi	$2,$2,0xfff
	andi	$3,$2,0xffff
	lw	$2,32($sp)
	ins	$2,$3,16,12
	sw	$2,32($sp)
	lw	$3,32($sp)
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16432
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	j	$L240
	nop

$L252:
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16436
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	sw	$2,36($sp)
	lw	$2,56($sp)
	andi	$2,$2,0x00ff
	andi	$2,$2,0x1
	andi	$3,$2,0x00ff
	lw	$2,36($sp)
	ins	$2,$3,15,1
	sw	$2,36($sp)
	lhu	$2,64($sp)
	andi	$2,$2,0xfff
	andi	$3,$2,0xffff
	lw	$2,36($sp)
	ins	$2,$3,0,12
	sw	$2,36($sp)
	lw	$3,36($sp)
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16436
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	j	$L240
	nop

$L253:
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16436
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	sw	$2,36($sp)
	lw	$2,56($sp)
	andi	$2,$2,0x00ff
	andi	$2,$2,0x1
	andi	$3,$2,0x00ff
	lw	$2,36($sp)
	ins	$2,$3,31,1
	sw	$2,36($sp)
	lhu	$2,64($sp)
	andi	$2,$2,0xfff
	andi	$3,$2,0xffff
	lw	$2,36($sp)
	ins	$2,$3,16,12
	sw	$2,36($sp)
	lw	$3,36($sp)
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16436
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	j	$L240
	nop

$L254:
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16440
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	sw	$2,40($sp)
	lw	$2,56($sp)
	andi	$2,$2,0x00ff
	andi	$2,$2,0x1
	andi	$3,$2,0x00ff
	lw	$2,40($sp)
	ins	$2,$3,15,1
	sw	$2,40($sp)
	lhu	$2,64($sp)
	andi	$2,$2,0xfff
	andi	$3,$2,0xffff
	lw	$2,40($sp)
	ins	$2,$3,0,12
	sw	$2,40($sp)
	lw	$3,40($sp)
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16440
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	j	$L240
	nop

$L255:
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16440
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	sw	$2,40($sp)
	lw	$2,56($sp)
	andi	$2,$2,0x00ff
	andi	$2,$2,0x1
	andi	$3,$2,0x00ff
	lw	$2,40($sp)
	ins	$2,$3,31,1
	sw	$2,40($sp)
	lhu	$2,64($sp)
	andi	$2,$2,0xfff
	andi	$3,$2,0xffff
	lw	$2,40($sp)
	ins	$2,$3,16,12
	sw	$2,40($sp)
	lw	$3,40($sp)
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16440
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	j	$L240
	nop

$L256:
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16444
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	sw	$2,44($sp)
	lw	$2,56($sp)
	andi	$2,$2,0x00ff
	andi	$2,$2,0x1
	andi	$3,$2,0x00ff
	lw	$2,44($sp)
	ins	$2,$3,15,1
	sw	$2,44($sp)
	lhu	$2,64($sp)
	andi	$2,$2,0xfff
	andi	$3,$2,0xffff
	lw	$2,44($sp)
	ins	$2,$3,0,12
	sw	$2,44($sp)
	lw	$3,44($sp)
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16444
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	j	$L240
	nop

$L257:
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16444
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	sw	$2,44($sp)
	lw	$2,56($sp)
	andi	$2,$2,0x00ff
	andi	$2,$2,0x1
	andi	$3,$2,0x00ff
	lw	$2,44($sp)
	ins	$2,$3,31,1
	sw	$2,44($sp)
	lhu	$2,64($sp)
	andi	$2,$2,0xfff
	andi	$3,$2,0xffff
	lw	$2,44($sp)
	ins	$2,$3,16,12
	sw	$2,44($sp)
	lw	$3,44($sp)
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16444
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	nop
$L240:
	move	$2,$0
$L258:
	lw	$31,52($sp)
	addiu	$sp,$sp,56
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	gponDevSetTCont0_15
	.size	gponDevSetTCont0_15, .-gponDevSetTCont0_15
	.section	.text.gponDevGetTCont0_15,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	gponDevGetTCont0_15
	.type	gponDevGetTCont0_15, @function
gponDevGetTCont0_15:
	.frame	$sp,56,$31		# vars= 32, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-56
	sw	$31,52($sp)
	sw	$4,56($sp)
	sw	$5,60($sp)
	sw	$6,64($sp)
	lw	$2,60($sp)
	bltz	$2,$L260
	nop

	lw	$2,60($sp)
	slt	$2,$2,16
	bne	$2,$0,$L261
	nop

$L260:
	li	$2,1			# 0x1
	j	$L281
	nop

$L261:
	lw	$2,60($sp)
	sltu	$2,$2,16
	beq	$2,$0,$L263
	nop

	lw	$2,60($sp)
	sll	$3,$2,2
	lui	$2,%hi($L265)
	addiu	$2,$2,%lo($L265)
	addu	$2,$3,$2
	lw	$2,0($2)
	j	$2
	nop

	.rdata
	.align	2
	.align	2
$L265:
	.word	$L264
	.word	$L266
	.word	$L267
	.word	$L268
	.word	$L269
	.word	$L270
	.word	$L271
	.word	$L272
	.word	$L273
	.word	$L274
	.word	$L275
	.word	$L276
	.word	$L277
	.word	$L278
	.word	$L279
	.word	$L280
	.section	.text.gponDevGetTCont0_15
$L264:
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16416
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	sw	$2,16($sp)
	lw	$2,16($sp)
	ext	$2,$2,15,1
	andi	$2,$2,0x00ff
	move	$3,$2
	lw	$2,56($sp)
	sw	$3,0($2)
	lw	$2,16($sp)
	ext	$2,$2,0,12
	andi	$2,$2,0xffff
	move	$3,$2
	lw	$2,64($sp)
	sh	$3,0($2)
	j	$L263
	nop

$L266:
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16416
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	sw	$2,16($sp)
	lw	$2,16($sp)
	ext	$2,$2,31,1
	andi	$2,$2,0x00ff
	move	$3,$2
	lw	$2,56($sp)
	sw	$3,0($2)
	lw	$2,16($sp)
	ext	$2,$2,16,12
	andi	$2,$2,0xffff
	move	$3,$2
	lw	$2,64($sp)
	sh	$3,0($2)
	j	$L263
	nop

$L267:
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16420
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	sw	$2,20($sp)
	lw	$2,20($sp)
	ext	$2,$2,15,1
	andi	$2,$2,0x00ff
	move	$3,$2
	lw	$2,56($sp)
	sw	$3,0($2)
	lw	$2,20($sp)
	ext	$2,$2,0,12
	andi	$2,$2,0xffff
	move	$3,$2
	lw	$2,64($sp)
	sh	$3,0($2)
	j	$L263
	nop

$L268:
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16420
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	sw	$2,20($sp)
	lw	$2,20($sp)
	ext	$2,$2,31,1
	andi	$2,$2,0x00ff
	move	$3,$2
	lw	$2,56($sp)
	sw	$3,0($2)
	lw	$2,20($sp)
	ext	$2,$2,16,12
	andi	$2,$2,0xffff
	move	$3,$2
	lw	$2,64($sp)
	sh	$3,0($2)
	j	$L263
	nop

$L269:
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16424
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	sw	$2,24($sp)
	lw	$2,24($sp)
	ext	$2,$2,15,1
	andi	$2,$2,0x00ff
	move	$3,$2
	lw	$2,56($sp)
	sw	$3,0($2)
	lw	$2,24($sp)
	ext	$2,$2,0,12
	andi	$2,$2,0xffff
	move	$3,$2
	lw	$2,64($sp)
	sh	$3,0($2)
	j	$L263
	nop

$L270:
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16424
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	sw	$2,24($sp)
	lw	$2,24($sp)
	ext	$2,$2,31,1
	andi	$2,$2,0x00ff
	move	$3,$2
	lw	$2,56($sp)
	sw	$3,0($2)
	lw	$2,24($sp)
	ext	$2,$2,16,12
	andi	$2,$2,0xffff
	move	$3,$2
	lw	$2,64($sp)
	sh	$3,0($2)
	j	$L263
	nop

$L271:
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16428
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	sw	$2,28($sp)
	lw	$2,28($sp)
	ext	$2,$2,15,1
	andi	$2,$2,0x00ff
	move	$3,$2
	lw	$2,56($sp)
	sw	$3,0($2)
	lw	$2,28($sp)
	ext	$2,$2,0,12
	andi	$2,$2,0xffff
	move	$3,$2
	lw	$2,64($sp)
	sh	$3,0($2)
	j	$L263
	nop

$L272:
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16428
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	sw	$2,28($sp)
	lw	$2,28($sp)
	ext	$2,$2,31,1
	andi	$2,$2,0x00ff
	move	$3,$2
	lw	$2,56($sp)
	sw	$3,0($2)
	lw	$2,28($sp)
	ext	$2,$2,16,12
	andi	$2,$2,0xffff
	move	$3,$2
	lw	$2,64($sp)
	sh	$3,0($2)
	j	$L263
	nop

$L273:
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16432
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	sw	$2,32($sp)
	lw	$2,32($sp)
	ext	$2,$2,15,1
	andi	$2,$2,0x00ff
	move	$3,$2
	lw	$2,56($sp)
	sw	$3,0($2)
	lw	$2,32($sp)
	ext	$2,$2,0,12
	andi	$2,$2,0xffff
	move	$3,$2
	lw	$2,64($sp)
	sh	$3,0($2)
	j	$L263
	nop

$L274:
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16432
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	sw	$2,32($sp)
	lw	$2,32($sp)
	ext	$2,$2,31,1
	andi	$2,$2,0x00ff
	move	$3,$2
	lw	$2,56($sp)
	sw	$3,0($2)
	lw	$2,32($sp)
	ext	$2,$2,16,12
	andi	$2,$2,0xffff
	move	$3,$2
	lw	$2,64($sp)
	sh	$3,0($2)
	j	$L263
	nop

$L275:
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16436
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	sw	$2,36($sp)
	lw	$2,36($sp)
	ext	$2,$2,15,1
	andi	$2,$2,0x00ff
	move	$3,$2
	lw	$2,56($sp)
	sw	$3,0($2)
	lw	$2,36($sp)
	ext	$2,$2,0,12
	andi	$2,$2,0xffff
	move	$3,$2
	lw	$2,64($sp)
	sh	$3,0($2)
	j	$L263
	nop

$L276:
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16436
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	sw	$2,36($sp)
	lw	$2,36($sp)
	ext	$2,$2,31,1
	andi	$2,$2,0x00ff
	move	$3,$2
	lw	$2,56($sp)
	sw	$3,0($2)
	lw	$2,36($sp)
	ext	$2,$2,16,12
	andi	$2,$2,0xffff
	move	$3,$2
	lw	$2,64($sp)
	sh	$3,0($2)
	j	$L263
	nop

$L277:
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16440
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	sw	$2,40($sp)
	lw	$2,40($sp)
	ext	$2,$2,15,1
	andi	$2,$2,0x00ff
	move	$3,$2
	lw	$2,56($sp)
	sw	$3,0($2)
	lw	$2,40($sp)
	ext	$2,$2,0,12
	andi	$2,$2,0xffff
	move	$3,$2
	lw	$2,64($sp)
	sh	$3,0($2)
	j	$L263
	nop

$L278:
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16440
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	sw	$2,40($sp)
	lw	$2,40($sp)
	ext	$2,$2,31,1
	andi	$2,$2,0x00ff
	move	$3,$2
	lw	$2,56($sp)
	sw	$3,0($2)
	lw	$2,40($sp)
	ext	$2,$2,16,12
	andi	$2,$2,0xffff
	move	$3,$2
	lw	$2,64($sp)
	sh	$3,0($2)
	j	$L263
	nop

$L279:
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16444
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	sw	$2,44($sp)
	lw	$2,44($sp)
	ext	$2,$2,15,1
	andi	$2,$2,0x00ff
	move	$3,$2
	lw	$2,56($sp)
	sw	$3,0($2)
	lw	$2,44($sp)
	ext	$2,$2,0,12
	andi	$2,$2,0xffff
	move	$3,$2
	lw	$2,64($sp)
	sh	$3,0($2)
	j	$L263
	nop

$L280:
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16444
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	sw	$2,44($sp)
	lw	$2,44($sp)
	ext	$2,$2,31,1
	andi	$2,$2,0x00ff
	move	$3,$2
	lw	$2,56($sp)
	sw	$3,0($2)
	lw	$2,44($sp)
	ext	$2,$2,16,12
	andi	$2,$2,0xffff
	move	$3,$2
	lw	$2,64($sp)
	sh	$3,0($2)
	nop
$L263:
	move	$2,$0
$L281:
	lw	$31,52($sp)
	addiu	$sp,$sp,56
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	gponDevGetTCont0_15
	.size	gponDevGetTCont0_15, .-gponDevGetTCont0_15
	.section	.text.gponDevSetTCont,"ax",@progbits
	.align	2
	.globl	gponDevSetTCont
	.set	nomips16
	.set	nomicromips
	.ent	gponDevSetTCont
	.type	gponDevSetTCont, @function
gponDevSetTCont:
	.frame	$sp,32,$31		# vars= 8, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-32
	sw	$31,28($sp)
	sw	$4,32($sp)
	sw	$5,36($sp)
	move	$2,$6
	sh	$2,40($sp)
	li	$2,1			# 0x1
	sw	$2,16($sp)
	lw	$2,36($sp)
	bltz	$2,$L283
	nop

	lw	$2,36($sp)
	slt	$2,$2,32
	bne	$2,$0,$L284
	nop

$L283:
	li	$2,1			# 0x1
	j	$L285
	nop

$L284:
	lw	$2,36($sp)
	slt	$2,$2,16
	beq	$2,$0,$L286
	nop

	lhu	$2,40($sp)
	lw	$4,32($sp)
	lw	$5,36($sp)
	move	$6,$2
	lui	$2,%hi(gponDevSetTCont0_15)
	addiu	$2,$2,%lo(gponDevSetTCont0_15)
	jalr	$2
	nop

	sw	$2,16($sp)
	j	$L287
	nop

$L286:
	lhu	$2,40($sp)
	lw	$4,32($sp)
	lw	$5,36($sp)
	move	$6,$2
	lui	$2,%hi(gponDevSetTCont16_31)
	addiu	$2,$2,%lo(gponDevSetTCont16_31)
	jalr	$2
	nop

	sw	$2,16($sp)
$L287:
	lw	$2,16($sp)
$L285:
	lw	$31,28($sp)
	addiu	$sp,$sp,32
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	gponDevSetTCont
	.size	gponDevSetTCont, .-gponDevSetTCont
	.section	.text.gponDevGetTCont,"ax",@progbits
	.align	2
	.globl	gponDevGetTCont
	.set	nomips16
	.set	nomicromips
	.ent	gponDevGetTCont
	.type	gponDevGetTCont, @function
gponDevGetTCont:
	.frame	$sp,32,$31		# vars= 8, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-32
	sw	$31,28($sp)
	sw	$4,32($sp)
	sw	$5,36($sp)
	sw	$6,40($sp)
	li	$2,1			# 0x1
	sw	$2,16($sp)
	lw	$2,36($sp)
	bltz	$2,$L289
	nop

	lw	$2,36($sp)
	slt	$2,$2,32
	bne	$2,$0,$L290
	nop

$L289:
	li	$2,1			# 0x1
	j	$L291
	nop

$L290:
	lw	$2,36($sp)
	slt	$2,$2,16
	beq	$2,$0,$L292
	nop

	lw	$4,32($sp)
	lw	$5,36($sp)
	lw	$6,40($sp)
	lui	$2,%hi(gponDevGetTCont0_15)
	addiu	$2,$2,%lo(gponDevGetTCont0_15)
	jalr	$2
	nop

	sw	$2,16($sp)
	j	$L293
	nop

$L292:
	lw	$4,32($sp)
	lw	$5,36($sp)
	lw	$6,40($sp)
	lui	$2,%hi(gponDevGetTCont16_31)
	addiu	$2,$2,%lo(gponDevGetTCont16_31)
	jalr	$2
	nop

	sw	$2,16($sp)
$L293:
	lw	$2,16($sp)
$L291:
	lw	$31,28($sp)
	addiu	$sp,$sp,32
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	gponDevGetTCont
	.size	gponDevGetTCont, .-gponDevGetTCont
	.section	.text.gponDevDisableTCont,"ax",@progbits
	.align	2
	.globl	gponDevDisableTCont
	.set	nomips16
	.set	nomicromips
	.ent	gponDevDisableTCont
	.type	gponDevDisableTCont, @function
gponDevDisableTCont:
	.frame	$sp,40,$31		# vars= 16, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-40
	sw	$31,36($sp)
	move	$2,$4
	sh	$2,40($sp)
	sw	$0,16($sp)
	j	$L295
	nop

$L300:
	addiu	$3,$sp,24
	addiu	$2,$sp,28
	move	$4,$3
	lw	$5,16($sp)
	move	$6,$2
	lui	$2,%hi(gponDevGetTCont)
	addiu	$2,$2,%lo(gponDevGetTCont)
	jalr	$2
	nop

	sw	$2,20($sp)
	lw	$2,20($sp)
	bne	$2,$0,$L296
	nop

	lhu	$2,28($sp)
	lhu	$3,40($sp)
	bne	$3,$2,$L297
	nop

	move	$4,$0
	lw	$5,16($sp)
	li	$6,255			# 0xff
	lui	$2,%hi(gponDevSetTCont)
	addiu	$2,$2,%lo(gponDevSetTCont)
	jalr	$2
	nop

	sw	$2,20($sp)
	lw	$2,20($sp)
	bne	$2,$0,$L298
	nop

	lw	$2,16($sp)
	j	$L301
	nop

$L298:
	li	$2,-14			# 0xfffffffffffffff2
	j	$L301
	nop

$L296:
	li	$2,-14			# 0xfffffffffffffff2
	j	$L301
	nop

$L297:
	lw	$2,16($sp)
	addiu	$2,$2,1
	sw	$2,16($sp)
$L295:
	lw	$2,16($sp)
	slt	$2,$2,32
	bne	$2,$0,$L300
	nop

	li	$2,-14			# 0xfffffffffffffff2
$L301:
	lw	$31,36($sp)
	addiu	$sp,$sp,40
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	gponDevDisableTCont
	.size	gponDevDisableTCont, .-gponDevDisableTCont
	.section	.text.gponDevEnableTCont,"ax",@progbits
	.align	2
	.globl	gponDevEnableTCont
	.set	nomips16
	.set	nomicromips
	.ent	gponDevEnableTCont
	.type	gponDevEnableTCont, @function
gponDevEnableTCont:
	.frame	$sp,40,$31		# vars= 16, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-40
	sw	$31,36($sp)
	move	$2,$4
	sh	$2,40($sp)
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lbu	$2,204($2)
	andi	$2,$2,0xffff
	lhu	$3,40($sp)
	bne	$3,$2,$L303
	nop

	move	$2,$0
	j	$L310
	nop

$L303:
	lhu	$2,40($sp)
	move	$4,$2
	lui	$2,%hi(gponDevDisableTCont)
	addiu	$2,$2,%lo(gponDevDisableTCont)
	jalr	$2
	nop

	li	$2,1			# 0x1
	sw	$2,16($sp)
	j	$L305
	nop

$L309:
	addiu	$3,$sp,24
	addiu	$2,$sp,28
	move	$4,$3
	lw	$5,16($sp)
	move	$6,$2
	lui	$2,%hi(gponDevGetTCont)
	addiu	$2,$2,%lo(gponDevGetTCont)
	jalr	$2
	nop

	sw	$2,20($sp)
	lw	$2,20($sp)
	bne	$2,$0,$L306
	nop

	lw	$2,24($sp)
	bne	$2,$0,$L307
	nop

	lhu	$2,40($sp)
	li	$4,1			# 0x1
	lw	$5,16($sp)
	move	$6,$2
	lui	$2,%hi(gponDevSetTCont)
	addiu	$2,$2,%lo(gponDevSetTCont)
	jalr	$2
	nop

	sw	$2,20($sp)
	lw	$2,20($sp)
	bne	$2,$0,$L308
	nop

	lw	$2,16($sp)
	j	$L310
	nop

$L308:
	li	$2,-14			# 0xfffffffffffffff2
	j	$L310
	nop

$L306:
	li	$2,-14			# 0xfffffffffffffff2
	j	$L310
	nop

$L307:
	lw	$2,16($sp)
	addiu	$2,$2,1
	sw	$2,16($sp)
$L305:
	lw	$2,16($sp)
	slt	$2,$2,32
	bne	$2,$0,$L309
	nop

	li	$2,-14			# 0xfffffffffffffff2
$L310:
	lw	$31,36($sp)
	addiu	$sp,$sp,40
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	gponDevEnableTCont
	.size	gponDevEnableTCont, .-gponDevEnableTCont
	.rdata
	.align	2
$LC19:
	.ascii	"0x4000: ONU ID                                          "
	.ascii	"              :0x%.8x\012\000"
	.align	2
$LC20:
	.ascii	"0x4004: ONU GLOBAL CONFIGURATION REGISTER               "
	.ascii	"              :0x%.8x\012\000"
	.align	2
$LC21:
	.ascii	"0x4008: GPON MAC INTERRUPTS STATUS                      "
	.ascii	"              :0x%.8x\012\000"
	.align	2
$LC22:
	.ascii	"0x400C: GPON MAC INTERRUPTS ENABLE                      "
	.ascii	"              :0x%.8x\012\000"
	.align	2
$LC23:
	.ascii	"0x4020: The Alloc-ID of T-CONT0 and T-CONT1             "
	.ascii	"              :0x%.8x\012\000"
	.align	2
$LC24:
	.ascii	"0x4024: The Alloc-ID of T-CONT2 and T-CONT3             "
	.ascii	"              :0x%.8x\012\000"
	.align	2
$LC25:
	.ascii	"0x4028: The Alloc-ID of T-CONT4 and T-CONT5             "
	.ascii	"              :0x%.8x\012\000"
	.align	2
$LC26:
	.ascii	"0x402C: The Alloc-ID of T-CONT6 and T-CONT7             "
	.ascii	"              :0x%.8x\012\000"
	.align	2
$LC27:
	.ascii	"0x4030: The Alloc-ID of T-CONT8 and T-CONT9             "
	.ascii	"              :0x%.8x\012\000"
	.align	2
$LC28:
	.ascii	"0x4034: The Alloc-ID of T-CONT10 and T-CONT11           "
	.ascii	"              :0x%.8x\012\000"
	.align	2
$LC29:
	.ascii	"0x4038: The Alloc-ID of T-CONT12 and T-CONT13           "
	.ascii	"              :0x%.8x\012\000"
	.align	2
$LC30:
	.ascii	"0x403C: The Alloc-ID of T-CONT14 and T-CONT15           "
	.ascii	"              :0x%.8x\012\000"
	.align	2
$LC31:
	.ascii	"0x4048: OMCI GEM PORT ID                                "
	.ascii	"              :0x%.8x\012\000"
	.align	2
$LC32:
	.ascii	"0x404C: GEM TABLE INITIALIZATION CONTROL                "
	.ascii	"              :0x%.8x\012\000"
	.align	2
$LC33:
	.ascii	"0x4050: PLOAMu FIFO STATUS                              "
	.ascii	"              :0x%.8x\012\000"
	.align	2
$LC34:
	.ascii	"0x4058: PLOAMd FIFO STATUS                              "
	.ascii	"              :0x%.8x\012\000"
	.align	2
$LC35:
	.ascii	"0x4060: SUPER FRAME COUNTER FOR AES KEY CHANGE          "
	.ascii	"              :0x%.8x\012\000"
	.align	2
$LC36:
	.ascii	"0x4064: ACTIVE AES KEY0                                 "
	.ascii	"              :0x%.8x\012\000"
	.align	2
$LC37:
	.ascii	"0x4068: ACTIVE AES KEY1                                 "
	.ascii	"              :0x%.8x\012\000"
	.align	2
$LC38:
	.ascii	"0x406C: ACTIVE AES KEY2                                 "
	.ascii	"              :0x%.8x\012\000"
	.align	2
$LC39:
	.ascii	"0x4070: ACTIVE AES KEY3                                 "
	.ascii	"              :0x%.8x\012\000"
	.align	2
$LC40:
	.ascii	"0x4074: AES KEY in SHADOW REGISTER0                     "
	.ascii	"              :0x%.8x\012\000"
	.align	2
$LC41:
	.ascii	"0x4078: AES KEY in SHADOW REGISTER1                     "
	.ascii	"              :0x%.8x\012\000"
	.align	2
$LC42:
	.ascii	"0x407C: AES KEY in SHADOW REGISTER2                     "
	.ascii	"              :0x%.8x\012\000"
	.align	2
$LC43:
	.ascii	"0x4080: AES KEY in SHADOW REGISTER3                     "
	.ascii	"              :0x%.8x\012\000"
	.align	2
$LC44:
	.ascii	"0x4084: AES KEY SWITCH BY SOFTWARE                      "
	.ascii	"              :0x%.8x\012\000"
	.align	2
$LC45:
	.ascii	"0x4090: PHYSICAL LAYER OVERHEAD TIME                    "
	.ascii	"              :0x%.8x\012\000"
	.align	2
$LC46:
	.ascii	"0x4094: NUMBER OF GUARD BITS                            "
	.ascii	"              :0x%.8x\012\000"
	.align	2
$LC47:
	.ascii	"0x4098: NUMBER OF TYPE1 and TYPE2 PREAMBLE BITS         "
	.ascii	"              :0x%.8x\012\000"
	.align	2
$LC48:
	.ascii	"0x409C: NUNMBER OF TYPE3                                "
	.ascii	"              :0x%.8x\012\000"
	.align	2
$LC49:
	.ascii	"0x40A0: NUMBER OF DELIMETER BITS                        "
	.ascii	"              :0x%.8x\012\000"
	.align	2
$LC50:
	.ascii	"0x40A4: PRE-ASSIGNED DELAY                              "
	.ascii	"              :0x%.8x\012\000"
	.align	2
$LC51:
	.ascii	"0x40A8: MAIN PATH EQUALIZATION DELAY                    "
	.ascii	"              :0x%.8x\012\000"
	.align	2
$LC52:
	.ascii	"0x40AC: ONU ACTIVATION PARAMETER                        "
	.ascii	"              :0x%.8x\012\000"
	.align	2
$LC53:
	.ascii	"0x40B0: Vendor ID                                       "
	.ascii	"              :0x%.8x\012\000"
	.align	2
$LC54:
	.ascii	"0x40B4: Vendor-specific serial number                   "
	.ascii	"              :0x%.8x\012\000"
	.align	2
$LC55:
	.ascii	"0x40B8: SN_ONU MESSAGE INFORMATION                      "
	.ascii	"              :0x%.8x\012\000"
	.align	2
$LC56:
	.ascii	"0x40BC: ONU STATE IN ACTIVATION                         "
	.ascii	"              :0x%.8x\012\000"
	.align	2
$LC57:
	.ascii	"0x40D0: SUPER FRAME COUNTER FOR TIME OF DAY CONFIGURATIO"
	.ascii	"N             :0x%.8x\012\000"
	.align	2
$LC58:
	.ascii	"0x40D4: NEW ToD SECOND LOW 32 BITS                      "
	.ascii	"              :0x%.8x\012\000"
	.align	2
$LC59:
	.ascii	"0x40D8: NEW ToD NANO SECOND                             "
	.ascii	"              :0x%.8x\012\000"
	.align	2
$LC60:
	.ascii	"0x40DC: CURRENT ToD SECOND LOW 32 BITS                  "
	.ascii	"              :0x%.8x\012\000"
	.align	2
$LC61:
	.ascii	"0x40E0: CURRENT ToD NANO SECOND                         "
	.ascii	"              :0x%.8x\012\000"
	.align	2
$LC62:
	.ascii	"0x40E4: TOD CLOCK PERIOD                                "
	.ascii	"              :0x%.8x\012\000"
	.align	2
$LC63:
	.ascii	"0x4100: TX FCS TABLE INITIALIZATION CONTROL             "
	.ascii	"              :0x%.8x\012\000"
	.align	2
$LC64:
	.ascii	"0x4120: MIB CONTROL AND STATUS                          "
	.ascii	"              :0x%.8x\012\000"
	.align	2
$LC65:
	.ascii	"0x4124: LOW 32 BITS of MIB READ DATA                    "
	.ascii	"              :0x%.8x\012\000"
	.align	2
$LC66:
	.ascii	"0x4128: HIGH 32 BITS of MIB READ DATA                   "
	.ascii	"              :0x%.8x\012\000"
	.align	2
$LC67:
	.ascii	"0x412C: LOW 32 BITS of MIB WRITE DATA                   "
	.ascii	"              :0x%.8x\012\000"
	.align	2
$LC68:
	.ascii	"0x4130: HIGH 32 BITS of MIB WRITE DATA                  "
	.ascii	"              :0x%.8x\012\000"
	.align	2
$LC69:
	.ascii	"0x4134: MIB TABLE INITIALIZATION CONTROL                "
	.ascii	"              :0x%.8x\012\000"
	.align	2
$LC70:
	.ascii	"0x4140: GEM PORT INDEX TABLE ACCESS CONTROL FOR MIB     "
	.ascii	"              :0x%.8x\012\000"
	.align	2
$LC71:
	.ascii	"0x4144: GEM PORT INDEX TABLE ACCESS STATUS              "
	.ascii	"              :0x%.8x\012\000"
	.align	2
$LC72:
	.ascii	"0x4148: GEM PORT INDEX TABLE INITIALIZATION CONTROL     "
	.ascii	"              :0x%.8x\012\000"
	.align	2
$LC73:
	.ascii	"0x4160: MAC AND BM INTERFACE STOP                       "
	.ascii	"              :0x%.8x\012\000"
	.align	2
$LC74:
	.ascii	"0x4200: FLEXIBLE ONU CAPABILITY SETTING                 "
	.ascii	"              :0x%.8x\012\000"
	.align	2
$LC75:
	.ascii	"0x4204: UPSTREAM PREFETCH DATA OFFSET                   "
	.ascii	"              :0x%.8x\012\000"
	.align	2
$LC76:
	.ascii	"0x4208: INTERNAL DELAY                                  "
	.ascii	"              :0x%.8x\012\000"
	.align	2
$LC77:
	.ascii	"0x420C: THRESHOLD of INSERT IDLE GEM                    "
	.ascii	"              :0x%.8x\012\000"
	.align	2
$LC78:
	.ascii	"0x4210: UPSTREAM NO MESSAGE 0                           "
	.ascii	"              :0x%.8x\012\000"
	.align	2
$LC79:
	.ascii	"0x4214: UPSTREAM NO MESSAGE 1                           "
	.ascii	"              :0x%.8x\012\000"
	.align	2
$LC80:
	.ascii	"0x4218: UPSTREAM NO MESSAGE 2                           "
	.ascii	"              :0x%.8x\012\000"
	.align	2
$LC81:
	.ascii	"0x421C: UPSTREAM DYING GASP CONTROL                     "
	.ascii	"              :0x%.8x\012\000"
	.align	2
$LC82:
	.ascii	"0x4220: BW MAP FITLER CONTROL                           "
	.ascii	"              :0x%.8x\012\000"
	.align	2
$LC83:
	.ascii	"0x4224: BW MAP SFIFO STATUS                             "
	.ascii	"              :0x%.8x\012\000"
	.align	2
$LC84:
	.ascii	"0x4228: DEBUG SIGNAL GROUP 0                            "
	.ascii	"              :0x%.8x\012\000"
	.align	2
$LC85:
	.ascii	"0x422C: DEBUG SIGNAL GROUP 1                            "
	.ascii	"              :0x%.8x\012\000"
	.align	2
$LC86:
	.ascii	"0x4230: THE MAXIMUM USED ENTRIES OF TX FIFO             "
	.ascii	"              :0x%.8x\012\000"
	.align	2
$LC87:
	.ascii	"0x4234: THE RECEIVED BWM GRANT STATUS                   "
	.ascii	"              :0x%.8x\012\000"
	.align	2
$LC88:
	.ascii	"0x4250: BW MAP BFIFO STATUS                             "
	.ascii	"              :0x%.8x\012\000"
	.align	2
$LC89:
	.ascii	"0x4260: ERROR CONTROL                                   "
	.ascii	"              :0x%.8x\012\000"
	.align	2
$LC90:
	.ascii	"0x4264: SOFTWARE RDI CONTROL                            "
	.ascii	"              :0x%.8x\012\000"
	.align	2
$LC91:
	.ascii	"0x4300: RX GEM COUNTER                                  "
	.ascii	"              :0x%.8x\012\000"
	.align	2
$LC92:
	.ascii	"0x4304: RX ETHERNET FRAME WITH ERROR CRC COUNTER        "
	.ascii	"              :0x%.8x\012\000"
	.align	2
$LC93:
	.ascii	"0x4308: RX GTC COUNTER                                  "
	.ascii	"              :0x%.8x\012\000"
	.align	2
$LC94:
	.ascii	"0x430C: TX GEM COUNTER                                  "
	.ascii	"              :0x%.8x\012\000"
	.align	2
$LC95:
	.ascii	"0x4310: TX BST COUNTER                                  "
	.ascii	"              :0x%.8x\012\000"
	.align	2
$LC96:
	.ascii	"0x4330: RX GEM HEADER ONE BIT ERROR COUNTER             "
	.ascii	"              :0x%.8x\012\000"
	.align	2
$LC97:
	.ascii	"0x4334: RX GEM HEADER TWO BIT ERROR COUNTER             "
	.ascii	"              :0x%.8x\012\000"
	.align	2
$LC98:
	.ascii	"0x4338: RX GEM HEADER UNCORRECTABLE ERROR COUNTER       "
	.ascii	"              :0x%.8x\012\000"
	.align	2
$LC99:
	.ascii	"0x4340: DBA_BACK_DOOR_FOR_SW                            "
	.ascii	"              :0x%.8x\012\000"
	.align	2
$LC100:
	.ascii	"0x4344: DBA_TOTAL_BUF_SIZE                              "
	.ascii	"              :0x%.8x\012\000"
	.align	2
$LC101:
	.ascii	"0x4348: DBA_GREEN_SIZE                                  "
	.ascii	"              :0x%.8x\012\000"
	.align	2
$LC102:
	.ascii	"0x434C: DBA_YELLOW_SIZE                                 "
	.ascii	"              :0x%.8x\012\000"
	.align	2
$LC103:
	.ascii	"0x4350: DBG_SLIGHT_MODIFY_SIZE_1                        "
	.ascii	"              :0x%.8x\012\000"
	.align	2
$LC104:
	.ascii	"0x4354: DBG_SLIGHT_MODIFY_SIZE_2                        "
	.ascii	"              :0x%.8x\012\000"
	.align	2
$LC105:
	.ascii	"0x4358: DS_SPF_CNT                                      "
	.ascii	"              :0x%.8x\012\000"
	.align	2
$LC106:
	.ascii	"0x435C: TX_SYNC_OFFSET                                  "
	.ascii	"              :0x%.8x\012\000"
	.align	2
$LC107:
	.ascii	"0x4360: PLOAMD_FILTER_IN_O5                             "
	.ascii	"              :0x%.8x\012\000"
	.align	2
$LC108:
	.ascii	"0x4364: SW_RESYNC                                       "
	.ascii	"              :0x%.8x\012\000"
	.align	2
$LC109:
	.ascii	"0x4368: GTC_ETH_EXTR                                    "
	.ascii	"              :0x%.8x\012\000"
	.align	2
$LC110:
	.ascii	"0x436C: DS_GTC_EXTR_ETH_HDR                             "
	.ascii	"              :0x%.8x\012\000"
	.align	2
$LC111:
	.ascii	"0x4370: US_GTC_EXTR_ETH_HDR                             "
	.ascii	"              :0x%.8x\012\000"
	.align	2
$LC112:
	.ascii	"0x4374: DS_GTC_EXTR_ETH_CNT                             "
	.ascii	"              :0x%.8x\012\000"
	.align	2
$LC113:
	.ascii	"0x4378: US_GTC_EXTR_ETH_CNT                             "
	.ascii	"              :0x%.8x\012\000"
	.align	2
$LC114:
	.ascii	"0x437C: SNIFF_GTC_INVLD_GEM_BYTE                        "
	.ascii	"              :0x%.8x\012\000"
	.align	2
$LC115:
	.ascii	"0x4380: SNIFF_DBG_REG                                   "
	.ascii	"              :0x%.8x\012\000"
	.align	2
$LC116:
	.ascii	"0x4384: SNIFF_US_INFO_FIFO                              "
	.ascii	"              :0x%.8x\012\000"
	.align	2
$LC117:
	.ascii	"0x4388: Sniff mode Up stream header transmit time window"
	.ascii	" of stop time :0x%.8x\012\000"
	.align	2
$LC118:
	.ascii	"0x438C: Sniff TX DA SA adderess                         "
	.ascii	"              :0x%.8x\012\000"
	.align	2
$LC119:
	.ascii	"0x4390: Sniff RX DA SA adderess                         "
	.ascii	"              :0x%.8x\012\000"
	.align	2
$LC120:
	.ascii	"0x4394: Sniff TX RX PID                                 "
	.ascii	"              :0x%.8x\012\000"
	.align	2
$LC121:
	.ascii	"0x4398: Sniff mode GEM port counter enable              "
	.ascii	"              :0x%.8x\012\000"
	.align	2
$LC122:
	.ascii	"0x439C: Sniff RX TX TPID                                "
	.ascii	"              :0x%.8x\012\000"
	.align	2
$LC123:
	.ascii	"0x43A0: GPON_MAC_SET                                    "
	.ascii	"              :0x%.8x\012\000"
	.align	2
$LC124:
	.ascii	"0x43A4: power management sleep mode configuration       "
	.ascii	"              :0x%.8x\012\000"
	.align	2
$LC125:
	.ascii	"0x43A8: Power management sleep counter                  "
	.ascii	"              :0x%.8x\012\000"
	.align	2
$LC126:
	.ascii	"0x43C0: Tx sync option                                  "
	.ascii	"              :0x%.8x\012\000"
	.align	2
$LC127:
	.ascii	"0x43C4: O3 and O4 US PLOAM CONTROL                      "
	.ascii	"              :0x%.8x\012\000"
	.align	2
$LC128:
	.ascii	"0x43C8: TOD 1PPS PLUSE WIDTH CONTROL\011\011\011\011\011"
	.ascii	"\011\011\011  :0x%.8x\012\000"
	.section	.text.gponDevDumpCsr,"ax",@progbits
	.align	2
	.globl	gponDevDumpCsr
	.set	nomips16
	.set	nomicromips
	.ent	gponDevDumpCsr
	.type	gponDevDumpCsr, @function
gponDevDumpCsr:
	.frame	$sp,24,$31		# vars= 0, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-24
	sw	$31,20($sp)
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16384
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi($LC19)
	addiu	$4,$2,%lo($LC19)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16388
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi($LC20)
	addiu	$4,$2,%lo($LC20)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16392
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi($LC21)
	addiu	$4,$2,%lo($LC21)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16396
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi($LC22)
	addiu	$4,$2,%lo($LC22)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16416
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi($LC23)
	addiu	$4,$2,%lo($LC23)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16420
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi($LC24)
	addiu	$4,$2,%lo($LC24)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16424
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi($LC25)
	addiu	$4,$2,%lo($LC25)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16428
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi($LC26)
	addiu	$4,$2,%lo($LC26)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16432
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi($LC27)
	addiu	$4,$2,%lo($LC27)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16436
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi($LC28)
	addiu	$4,$2,%lo($LC28)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16440
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi($LC29)
	addiu	$4,$2,%lo($LC29)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16444
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi($LC30)
	addiu	$4,$2,%lo($LC30)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16456
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi($LC31)
	addiu	$4,$2,%lo($LC31)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16460
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi($LC32)
	addiu	$4,$2,%lo($LC32)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16464
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi($LC33)
	addiu	$4,$2,%lo($LC33)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16472
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi($LC34)
	addiu	$4,$2,%lo($LC34)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16480
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi($LC35)
	addiu	$4,$2,%lo($LC35)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16484
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi($LC36)
	addiu	$4,$2,%lo($LC36)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16488
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi($LC37)
	addiu	$4,$2,%lo($LC37)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16492
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi($LC38)
	addiu	$4,$2,%lo($LC38)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16496
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi($LC39)
	addiu	$4,$2,%lo($LC39)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16500
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi($LC40)
	addiu	$4,$2,%lo($LC40)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16504
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi($LC41)
	addiu	$4,$2,%lo($LC41)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16508
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi($LC42)
	addiu	$4,$2,%lo($LC42)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16512
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi($LC43)
	addiu	$4,$2,%lo($LC43)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16516
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi($LC44)
	addiu	$4,$2,%lo($LC44)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16528
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi($LC45)
	addiu	$4,$2,%lo($LC45)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16532
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi($LC46)
	addiu	$4,$2,%lo($LC46)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16536
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi($LC47)
	addiu	$4,$2,%lo($LC47)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16540
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi($LC48)
	addiu	$4,$2,%lo($LC48)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16544
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi($LC49)
	addiu	$4,$2,%lo($LC49)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16548
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi($LC50)
	addiu	$4,$2,%lo($LC50)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16552
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi($LC51)
	addiu	$4,$2,%lo($LC51)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16556
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi($LC52)
	addiu	$4,$2,%lo($LC52)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16560
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi($LC53)
	addiu	$4,$2,%lo($LC53)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16564
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi($LC54)
	addiu	$4,$2,%lo($LC54)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16568
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi($LC55)
	addiu	$4,$2,%lo($LC55)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16572
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi($LC56)
	addiu	$4,$2,%lo($LC56)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16592
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi($LC57)
	addiu	$4,$2,%lo($LC57)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16596
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi($LC58)
	addiu	$4,$2,%lo($LC58)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16600
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi($LC59)
	addiu	$4,$2,%lo($LC59)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16604
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi($LC60)
	addiu	$4,$2,%lo($LC60)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16608
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi($LC61)
	addiu	$4,$2,%lo($LC61)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16612
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi($LC62)
	addiu	$4,$2,%lo($LC62)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16640
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi($LC63)
	addiu	$4,$2,%lo($LC63)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16672
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi($LC64)
	addiu	$4,$2,%lo($LC64)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16676
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi($LC65)
	addiu	$4,$2,%lo($LC65)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16680
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi($LC66)
	addiu	$4,$2,%lo($LC66)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16684
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi($LC67)
	addiu	$4,$2,%lo($LC67)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16688
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi($LC68)
	addiu	$4,$2,%lo($LC68)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16692
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi($LC69)
	addiu	$4,$2,%lo($LC69)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16704
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi($LC70)
	addiu	$4,$2,%lo($LC70)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16708
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi($LC71)
	addiu	$4,$2,%lo($LC71)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16712
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi($LC72)
	addiu	$4,$2,%lo($LC72)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16736
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi($LC73)
	addiu	$4,$2,%lo($LC73)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16896
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi($LC74)
	addiu	$4,$2,%lo($LC74)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16900
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi($LC75)
	addiu	$4,$2,%lo($LC75)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16904
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi($LC76)
	addiu	$4,$2,%lo($LC76)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16908
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi($LC77)
	addiu	$4,$2,%lo($LC77)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16912
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi($LC78)
	addiu	$4,$2,%lo($LC78)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16916
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi($LC79)
	addiu	$4,$2,%lo($LC79)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16920
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi($LC80)
	addiu	$4,$2,%lo($LC80)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16924
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi($LC81)
	addiu	$4,$2,%lo($LC81)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16928
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi($LC82)
	addiu	$4,$2,%lo($LC82)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16932
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi($LC83)
	addiu	$4,$2,%lo($LC83)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16936
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi($LC84)
	addiu	$4,$2,%lo($LC84)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16940
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi($LC85)
	addiu	$4,$2,%lo($LC85)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16944
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi($LC86)
	addiu	$4,$2,%lo($LC86)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16948
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi($LC87)
	addiu	$4,$2,%lo($LC87)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16976
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi($LC88)
	addiu	$4,$2,%lo($LC88)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16992
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi($LC89)
	addiu	$4,$2,%lo($LC89)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16996
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi($LC90)
	addiu	$4,$2,%lo($LC90)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,17152
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi($LC91)
	addiu	$4,$2,%lo($LC91)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,17156
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi($LC92)
	addiu	$4,$2,%lo($LC92)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,17160
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi($LC93)
	addiu	$4,$2,%lo($LC93)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,17164
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi($LC94)
	addiu	$4,$2,%lo($LC94)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,17168
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi($LC95)
	addiu	$4,$2,%lo($LC95)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,17200
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi($LC96)
	addiu	$4,$2,%lo($LC96)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,17204
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi($LC97)
	addiu	$4,$2,%lo($LC97)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,17208
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi($LC98)
	addiu	$4,$2,%lo($LC98)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,17216
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi($LC99)
	addiu	$4,$2,%lo($LC99)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,17220
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi($LC100)
	addiu	$4,$2,%lo($LC100)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,17224
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi($LC101)
	addiu	$4,$2,%lo($LC101)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,17228
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi($LC102)
	addiu	$4,$2,%lo($LC102)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,17232
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi($LC103)
	addiu	$4,$2,%lo($LC103)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,17236
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi($LC104)
	addiu	$4,$2,%lo($LC104)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,17240
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi($LC105)
	addiu	$4,$2,%lo($LC105)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,17244
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi($LC106)
	addiu	$4,$2,%lo($LC106)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,17248
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi($LC107)
	addiu	$4,$2,%lo($LC107)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,17252
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi($LC108)
	addiu	$4,$2,%lo($LC108)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,17256
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi($LC109)
	addiu	$4,$2,%lo($LC109)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,17260
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi($LC110)
	addiu	$4,$2,%lo($LC110)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,17264
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi($LC111)
	addiu	$4,$2,%lo($LC111)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,17268
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi($LC112)
	addiu	$4,$2,%lo($LC112)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,17272
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi($LC113)
	addiu	$4,$2,%lo($LC113)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,17276
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi($LC114)
	addiu	$4,$2,%lo($LC114)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,17280
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi($LC115)
	addiu	$4,$2,%lo($LC115)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,17284
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi($LC116)
	addiu	$4,$2,%lo($LC116)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,17288
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi($LC117)
	addiu	$4,$2,%lo($LC117)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,17292
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi($LC118)
	addiu	$4,$2,%lo($LC118)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,17296
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi($LC119)
	addiu	$4,$2,%lo($LC119)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,17300
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi($LC120)
	addiu	$4,$2,%lo($LC120)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,17304
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi($LC121)
	addiu	$4,$2,%lo($LC121)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,17308
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi($LC122)
	addiu	$4,$2,%lo($LC122)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,17312
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi($LC123)
	addiu	$4,$2,%lo($LC123)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,17316
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi($LC124)
	addiu	$4,$2,%lo($LC124)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,17320
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi($LC125)
	addiu	$4,$2,%lo($LC125)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,17344
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi($LC126)
	addiu	$4,$2,%lo($LC126)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,17348
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi($LC127)
	addiu	$4,$2,%lo($LC127)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	li	$2,-1078984704			# 0xffffffffbfb00000
	ori	$2,$2,0x64
	lw	$3,0($2)
	li	$2,-65536			# 0xffffffffffff0000
	and	$3,$3,$2
	li	$2,524288			# 0x80000
	bne	$3,$2,$L312
	nop

	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,17352
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi($LC128)
	addiu	$4,$2,%lo($LC128)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L312:
	move	$2,$0
	lw	$31,20($sp)
	addiu	$sp,$sp,24
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	gponDevDumpCsr
	.size	gponDevDumpCsr, .-gponDevDumpCsr
	.rdata
	.align	2
$LC129:
	.ascii	"TCONT%d ID:%d, Valid:%s\012\000"
	.section	.text.gponDevDumpTcontInfo,"ax",@progbits
	.align	2
	.globl	gponDevDumpTcontInfo
	.set	nomips16
	.set	nomicromips
	.ent	gponDevDumpTcontInfo
	.type	gponDevDumpTcontInfo, @function
gponDevDumpTcontInfo:
	.frame	$sp,40,$31		# vars= 16, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-40
	sw	$31,36($sp)
	sw	$0,16($sp)
	j	$L315
	nop

$L321:
	addiu	$3,$sp,24
	addiu	$2,$sp,28
	move	$4,$3
	lw	$5,16($sp)
	move	$6,$2
	lui	$2,%hi(gponDevGetTCont)
	addiu	$2,$2,%lo(gponDevGetTCont)
	jalr	$2
	nop

	sw	$2,20($sp)
	lw	$2,20($sp)
	bne	$2,$0,$L316
	nop

	lhu	$2,28($sp)
	move	$6,$2
	lw	$3,24($sp)
	li	$2,1			# 0x1
	bne	$3,$2,$L317
	nop

	lui	$2,%hi($LC10)
	addiu	$2,$2,%lo($LC10)
	j	$L318
	nop

$L317:
	lui	$2,%hi($LC11)
	addiu	$2,$2,%lo($LC11)
$L318:
	lui	$3,%hi($LC129)
	addiu	$4,$3,%lo($LC129)
	lw	$5,16($sp)
	move	$7,$2
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	j	$L323
	nop

$L316:
	li	$2,-1			# 0xffffffffffffffff
	j	$L322
	nop

$L323:
	lw	$2,16($sp)
	addiu	$2,$2,1
	sw	$2,16($sp)
$L315:
	lw	$2,16($sp)
	slt	$2,$2,32
	bne	$2,$0,$L321
	nop

	move	$2,$0
$L322:
	lw	$31,36($sp)
	addiu	$sp,$sp,40
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	gponDevDumpTcontInfo
	.size	gponDevDumpTcontInfo, .-gponDevDumpTcontInfo
	.rdata
	.align	2
$LC130:
	.ascii	"ECNT_HOOK_ERROR occur with cmd_id:0x%x\012\000"
	.align	2
$LC131:
	.ascii	"No such API with type:%d, cmd_id:0x%x\012\000"
	.align	2
$LC132:
	.ascii	"%s:phy device initialization failed.\012\000"
	.section	.text.gpon_dev_init_reset,"ax",@progbits
	.align	2
	.globl	gpon_dev_init_reset
	.set	nomips16
	.set	nomicromips
	.ent	gpon_dev_init_reset
	.type	gpon_dev_init_reset, @function
gpon_dev_init_reset:
	.frame	$sp,160,$31		# vars= 136, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-160
	sw	$31,156($sp)
	li	$4,1			# 0x1
	lui	$2,%hi(gponDevMbiStop)
	addiu	$2,$2,%lo(gponDevMbiStop)
	jalr	$2
	nop

	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	lw	$3,-30416($2)
	li	$2,1			# 0x1
	bne	$3,$2,$L325
	nop

	li	$2,-1078984704			# 0xffffffffbfb00000
	ori	$2,$2,0x834
	sw	$2,16($sp)
	li	$2,-2147483648			# 0xffffffff80000000
	sw	$2,20($sp)
	lw	$2,16($sp)
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	sw	$2,24($sp)
	lw	$3,24($sp)
	lw	$2,20($sp)
	or	$2,$3,$2
	sw	$2,24($sp)
	lw	$2,16($sp)
	lw	$4,24($sp)
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	li	$4,1000			# 0x3e8
	lui	$2,%hi(__udelay)
	addiu	$2,$2,%lo(__udelay)
	jalr	$2
	nop

	li	$2,-1078984704			# 0xffffffffbfb00000
	ori	$2,$2,0x834
	sw	$2,28($sp)
	li	$2,-2147483648			# 0xffffffff80000000
	sw	$2,32($sp)
	lw	$2,28($sp)
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	sw	$2,36($sp)
	lw	$2,32($sp)
	nor	$2,$0,$2
	lw	$3,36($sp)
	and	$2,$3,$2
	sw	$2,36($sp)
	lw	$2,28($sp)
	lw	$4,36($sp)
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	j	$L326
	nop

$L325:
	li	$2,-1078984704			# 0xffffffffbfb00000
	ori	$2,$2,0x834
	sw	$2,40($sp)
	li	$2,-2147483648			# 0xffffffff80000000
	sw	$2,44($sp)
	lw	$2,40($sp)
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	sw	$2,48($sp)
	lw	$3,48($sp)
	lw	$2,44($sp)
	or	$2,$3,$2
	sw	$2,48($sp)
	lw	$2,40($sp)
	lw	$4,48($sp)
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	li	$2,-1078984704			# 0xffffffffbfb00000
	ori	$2,$2,0x380
	sw	$2,52($sp)
	li	$2,33554432			# 0x2000000
	sw	$2,56($sp)
	lw	$2,52($sp)
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	sw	$2,60($sp)
	lw	$2,56($sp)
	nor	$2,$0,$2
	lw	$3,60($sp)
	and	$2,$3,$2
	sw	$2,60($sp)
	lw	$2,52($sp)
	lw	$4,60($sp)
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	li	$4,1000			# 0x3e8
	lui	$2,%hi(__udelay)
	addiu	$2,$2,%lo(__udelay)
	jalr	$2
	nop

	li	$2,-1078984704			# 0xffffffffbfb00000
	ori	$2,$2,0x834
	sw	$2,64($sp)
	li	$2,-2147483648			# 0xffffffff80000000
	sw	$2,68($sp)
	lw	$2,64($sp)
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	sw	$2,72($sp)
	lw	$2,68($sp)
	nor	$2,$0,$2
	lw	$3,72($sp)
	and	$2,$3,$2
	sw	$2,72($sp)
	lw	$2,64($sp)
	lw	$4,72($sp)
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	li	$2,-1078984704			# 0xffffffffbfb00000
	ori	$2,$2,0x380
	sw	$2,76($sp)
	li	$2,33554432			# 0x2000000
	sw	$2,80($sp)
	lw	$2,76($sp)
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	sw	$2,84($sp)
	lw	$3,84($sp)
	lw	$2,80($sp)
	or	$2,$3,$2
	sw	$2,84($sp)
	lw	$2,76($sp)
	lw	$4,84($sp)
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	li	$2,4			# 0x4
	sw	$2,88($sp)
	lw	$2,88($sp)
	sw	$2,92($sp)
	sw	$0,96($sp)
	sw	$0,136($sp)
	sw	$0,140($sp)
	sw	$0,144($sp)
	sw	$0,148($sp)
	li	$2,1			# 0x1
	sw	$2,136($sp)
	lw	$2,92($sp)
	sw	$2,140($sp)
	lw	$2,96($sp)
	sw	$2,148($sp)
	addiu	$2,$sp,136
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
	bne	$3,$2,$L327
	nop

	lw	$2,100($sp)
	lw	$3,4($2)
	lui	$2,%hi($LC130)
	addiu	$4,$2,%lo($LC130)
	move	$5,$3
	lui	$2,%hi(panic)
	addiu	$2,$2,%lo(panic)
	jalr	$2
	nop

$L327:
	lw	$2,100($sp)
	lw	$3,8($2)
	li	$2,-1			# 0xffffffffffffffff
	bne	$3,$2,$L328
	nop

	lw	$2,100($sp)
	lw	$5,0($2)
	lw	$2,100($sp)
	lw	$3,4($2)
	lui	$2,%hi($LC131)
	addiu	$4,$2,%lo($LC131)
	move	$6,$3
	lui	$2,%hi(panic)
	addiu	$2,$2,%lo(panic)
	jalr	$2
	nop

$L328:
	lw	$2,100($sp)
	lw	$2,8($2)
	beq	$2,$0,$L332
	nop

	lui	$2,%hi(dump_stack)
	addiu	$2,$2,%lo(dump_stack)
	jalr	$2
	nop

	lui	$2,%hi($LC132)
	addiu	$4,$2,%lo($LC132)
	lui	$2,%hi(__func__.32060)
	addiu	$5,$2,%lo(__func__.32060)
	lui	$2,%hi(panic)
	addiu	$2,$2,%lo(panic)
	jalr	$2
	nop

$L332:
	li	$2,8			# 0x8
	sw	$2,104($sp)
	lw	$2,104($sp)
	sw	$2,108($sp)
	sw	$0,112($sp)
	sw	$0,120($sp)
	sw	$0,124($sp)
	sw	$0,128($sp)
	sw	$0,132($sp)
	li	$2,1			# 0x1
	sw	$2,120($sp)
	lw	$2,108($sp)
	sw	$2,124($sp)
	lw	$2,112($sp)
	sw	$2,132($sp)
	addiu	$2,$sp,120
	sw	$2,116($sp)
	li	$4,25			# 0x19
	move	$5,$0
	lw	$6,116($sp)
	lui	$2,%hi(__ECNT_HOOK)
	addiu	$2,$2,%lo(__ECNT_HOOK)
	jalr	$2
	nop

	move	$3,$2
	li	$2,-1			# 0xffffffffffffffff
	bne	$3,$2,$L333
	nop

	lw	$2,116($sp)
	lw	$3,4($2)
	lui	$2,%hi($LC130)
	addiu	$4,$2,%lo($LC130)
	move	$5,$3
	lui	$2,%hi(panic)
	addiu	$2,$2,%lo(panic)
	jalr	$2
	nop

$L333:
	lw	$2,116($sp)
	lw	$3,8($2)
	li	$2,-1			# 0xffffffffffffffff
	bne	$3,$2,$L326
	nop

	lw	$2,116($sp)
	lw	$5,0($2)
	lw	$2,116($sp)
	lw	$3,4($2)
	lui	$2,%hi($LC131)
	addiu	$4,$2,%lo($LC131)
	move	$6,$3
	lui	$2,%hi(panic)
	addiu	$2,$2,%lo(panic)
	jalr	$2
	nop

$L326:
	move	$4,$0
	lui	$2,%hi(gponDevMbiStop)
	addiu	$2,$2,%lo(gponDevMbiStop)
	jalr	$2
	nop

	lw	$31,156($sp)
	addiu	$sp,$sp,160
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	gpon_dev_init_reset
	.size	gpon_dev_init_reset, .-gpon_dev_init_reset
	.section	.text.gpon_dev_reset_GPON_MAC,"ax",@progbits
	.align	2
	.globl	gpon_dev_reset_GPON_MAC
	.set	nomips16
	.set	nomicromips
	.ent	gpon_dev_reset_GPON_MAC
	.type	gpon_dev_reset_GPON_MAC, @function
gpon_dev_reset_GPON_MAC:
	.frame	$sp,80,$31		# vars= 56, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-80
	sw	$31,76($sp)
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	addiu	$2,$2,456
	sw	$2,20($sp)
	lw	$2,20($sp)
	move	$4,$2
	lui	$2,%hi(_raw_spin_lock_irqsave)
	addiu	$2,$2,%lo(_raw_spin_lock_irqsave)
	jalr	$2
	nop

	sw	$2,16($sp)
	li	$2,-1078984704			# 0xffffffffbfb00000
	ori	$2,$2,0x834
	sw	$2,24($sp)
	li	$2,-2147483648			# 0xffffffff80000000
	sw	$2,28($sp)
	lw	$2,24($sp)
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	sw	$2,32($sp)
	lw	$3,32($sp)
	lw	$2,28($sp)
	or	$2,$3,$2
	sw	$2,32($sp)
	lw	$2,24($sp)
	lw	$4,32($sp)
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	li	$4,1			# 0x1
	lui	$2,%hi(__udelay)
	addiu	$2,$2,%lo(__udelay)
	jalr	$2
	nop

	li	$2,-1078984704			# 0xffffffffbfb00000
	ori	$2,$2,0x834
	sw	$2,36($sp)
	li	$2,-2147483648			# 0xffffffff80000000
	sw	$2,40($sp)
	lw	$2,36($sp)
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	sw	$2,44($sp)
	lw	$2,40($sp)
	nor	$2,$0,$2
	lw	$3,44($sp)
	and	$2,$3,$2
	sw	$2,44($sp)
	lw	$2,36($sp)
	lw	$4,44($sp)
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	addiu	$2,$2,456
	sw	$2,48($sp)
	lw	$2,16($sp)
	sw	$2,52($sp)
	lw	$2,48($sp)
	move	$4,$2
	lw	$5,52($sp)
	lui	$2,%hi(_raw_spin_unlock_irqrestore)
	addiu	$2,$2,%lo(_raw_spin_unlock_irqrestore)
	jalr	$2
	nop

	li	$4,1			# 0x1
	lui	$2,%hi(gponDevMbiStop)
	addiu	$2,$2,%lo(gponDevMbiStop)
	jalr	$2
	nop

	lw	$31,76($sp)
	addiu	$sp,$sp,80
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	gpon_dev_reset_GPON_MAC
	.size	gpon_dev_reset_GPON_MAC, .-gpon_dev_reset_GPON_MAC
	.section	.text.gponDevDeactiveOnu,"ax",@progbits
	.align	2
	.globl	gponDevDeactiveOnu
	.set	nomips16
	.set	nomicromips
	.ent	gponDevDeactiveOnu
	.type	gponDevDeactiveOnu, @function
gponDevDeactiveOnu:
	.frame	$sp,32,$31		# vars= 8, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-32
	sw	$31,28($sp)
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16384
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	sw	$2,16($sp)
	lw	$2,16($sp)
	ins	$2,$0,15,1
	sw	$2,16($sp)
	li	$2,-1			# 0xffffffffffffffff
	sb	$2,19($sp)
	lw	$3,16($sp)
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16384
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16388
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	sw	$2,20($sp)
	lw	$2,20($sp)
	ins	$2,$0,16,1
	sw	$2,20($sp)
	lw	$3,20($sp)
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16388
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	lui	$2,%hi(gponDevResetGemInfo)
	addiu	$2,$2,%lo(gponDevResetGemInfo)
	jalr	$2
	nop

	move	$2,$0
	lw	$31,28($sp)
	addiu	$sp,$sp,32
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	gponDevDeactiveOnu
	.size	gponDevDeactiveOnu, .-gponDevDeactiveOnu
	.section	.text.gponDevSetEncryptKey,"ax",@progbits
	.align	2
	.globl	gponDevSetEncryptKey
	.set	nomips16
	.set	nomicromips
	.ent	gponDevSetEncryptKey
	.type	gponDevSetEncryptKey, @function
gponDevSetEncryptKey:
	.frame	$sp,40,$31		# vars= 16, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-40
	sw	$31,36($sp)
	sw	$4,40($sp)
	lw	$2,40($sp)
	lbu	$2,0($2)
	sll	$3,$2,24
	lw	$2,40($sp)
	addiu	$2,$2,1
	lbu	$2,0($2)
	sll	$2,$2,16
	or	$3,$3,$2
	lw	$2,40($sp)
	addiu	$2,$2,2
	lbu	$2,0($2)
	sll	$2,$2,8
	or	$2,$3,$2
	lw	$3,40($sp)
	addiu	$3,$3,3
	lbu	$3,0($3)
	or	$2,$2,$3
	sw	$2,28($sp)
	lw	$2,40($sp)
	addiu	$2,$2,4
	lbu	$2,0($2)
	sll	$3,$2,24
	lw	$2,40($sp)
	addiu	$2,$2,5
	lbu	$2,0($2)
	sll	$2,$2,16
	or	$3,$3,$2
	lw	$2,40($sp)
	addiu	$2,$2,6
	lbu	$2,0($2)
	sll	$2,$2,8
	or	$2,$3,$2
	lw	$3,40($sp)
	addiu	$3,$3,7
	lbu	$3,0($3)
	or	$2,$2,$3
	sw	$2,24($sp)
	lw	$2,40($sp)
	addiu	$2,$2,8
	lbu	$2,0($2)
	sll	$3,$2,24
	lw	$2,40($sp)
	addiu	$2,$2,9
	lbu	$2,0($2)
	sll	$2,$2,16
	or	$3,$3,$2
	lw	$2,40($sp)
	addiu	$2,$2,10
	lbu	$2,0($2)
	sll	$2,$2,8
	or	$2,$3,$2
	lw	$3,40($sp)
	addiu	$3,$3,11
	lbu	$3,0($3)
	or	$2,$2,$3
	sw	$2,20($sp)
	lw	$2,40($sp)
	addiu	$2,$2,12
	lbu	$2,0($2)
	sll	$3,$2,24
	lw	$2,40($sp)
	addiu	$2,$2,13
	lbu	$2,0($2)
	sll	$2,$2,16
	or	$3,$3,$2
	lw	$2,40($sp)
	addiu	$2,$2,14
	lbu	$2,0($2)
	sll	$2,$2,8
	or	$2,$3,$2
	lw	$3,40($sp)
	addiu	$3,$3,15
	lbu	$3,0($3)
	or	$2,$2,$3
	sw	$2,16($sp)
	lw	$3,16($sp)
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16500
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	lw	$3,20($sp)
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16504
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	lw	$3,24($sp)
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16508
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	lw	$3,28($sp)
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16512
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	move	$2,$0
	lw	$31,36($sp)
	addiu	$sp,$sp,40
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	gponDevSetEncryptKey
	.size	gponDevSetEncryptKey, .-gponDevSetEncryptKey
	.section	.text.gponDevGetEncryptKey,"ax",@progbits
	.align	2
	.globl	gponDevGetEncryptKey
	.set	nomips16
	.set	nomicromips
	.ent	gponDevGetEncryptKey
	.type	gponDevGetEncryptKey, @function
gponDevGetEncryptKey:
	.frame	$sp,32,$31		# vars= 8, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-32
	sw	$31,28($sp)
	sw	$4,32($sp)
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16480
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	sw	$2,16($sp)
	lw	$2,16($sp)
	ext	$2,$2,0,30
	move	$3,$2
	lw	$2,32($sp)
	sw	$3,0($2)
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16500
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	move	$3,$2
	lw	$2,32($sp)
	sw	$3,20($2)
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16504
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	move	$3,$2
	lw	$2,32($sp)
	sw	$3,24($2)
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16508
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	move	$3,$2
	lw	$2,32($sp)
	sw	$3,28($2)
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16512
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	move	$3,$2
	lw	$2,32($sp)
	sw	$3,32($2)
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16484
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	move	$3,$2
	lw	$2,32($sp)
	sw	$3,4($2)
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16488
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	move	$3,$2
	lw	$2,32($sp)
	sw	$3,8($2)
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16492
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	move	$3,$2
	lw	$2,32($sp)
	sw	$3,12($2)
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16496
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	move	$3,$2
	lw	$2,32($sp)
	sw	$3,16($2)
	move	$2,$0
	lw	$31,28($sp)
	addiu	$sp,$sp,32
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	gponDevGetEncryptKey
	.size	gponDevGetEncryptKey, .-gponDevGetEncryptKey
	.rdata
	.align	2
$LC133:
	.ascii	"[%lu0ms]There is something wrong with set AES key switch"
	.ascii	" by SW\012\000"
	.section	.text.gponDevSetKeySwithTime,"ax",@progbits
	.align	2
	.globl	gponDevSetKeySwithTime
	.set	nomips16
	.set	nomicromips
	.ent	gponDevSetKeySwithTime
	.type	gponDevSetKeySwithTime, @function
gponDevSetKeySwithTime:
	.frame	$sp,32,$31		# vars= 8, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-32
	sw	$31,28($sp)
	sw	$4,32($sp)
	lw	$2,32($sp)
	bne	$2,$0,$L347
	nop

	lui	$2,%hi(gponDevSetAesKeySwitchBySw)
	addiu	$2,$2,%lo(gponDevSetAesKeySwitchBySw)
	jalr	$2
	nop

	sw	$2,16($sp)
	lw	$2,16($sp)
	bne	$2,$0,$L348
	nop

	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	lhu	$2,-30816($2)
	andi	$2,$2,0x1
	beq	$2,$0,$L348
	nop

	lui	$2,%hi(jiffies)
	lw	$3,%lo(jiffies)($2)
	lui	$2,%hi($LC133)
	addiu	$4,$2,%lo($LC133)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L348:
	move	$2,$0
	j	$L350
	nop

$L347:
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16480
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	sw	$2,20($sp)
	lw	$2,32($sp)
	ext	$3,$2,0,30
	lw	$2,20($sp)
	ins	$2,$3,0,30
	sw	$2,20($sp)
	lw	$3,20($sp)
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16480
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	move	$2,$0
$L350:
	lw	$31,28($sp)
	addiu	$sp,$sp,32
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	gponDevSetKeySwithTime
	.size	gponDevSetKeySwithTime, .-gponDevSetKeySwithTime
	.section	.text.gponDevSetDBABlockSize,"ax",@progbits
	.align	2
	.globl	gponDevSetDBABlockSize
	.set	nomips16
	.set	nomicromips
	.ent	gponDevSetDBABlockSize
	.type	gponDevSetDBABlockSize, @function
gponDevSetDBABlockSize:
	.frame	$sp,32,$31		# vars= 8, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-32
	sw	$31,28($sp)
	move	$2,$4
	sh	$2,32($sp)
	sb	$0,16($sp)
	lhu	$2,32($sp)
	sltu	$2,$2,2048
	bne	$2,$0,$L352
	nop

	li	$2,-128			# 0xffffffffffffff80
	sb	$2,16($sp)
	j	$L353
	nop

$L352:
	lhu	$2,32($sp)
	li	$3,2048			# 0x800
	div	$0,$3,$2
	mfhi	$3
	mflo	$2
	andi	$2,$2,0x00ff
	lhu	$3,32($sp)
	li	$4,2048			# 0x800
	div	$0,$4,$3
	mfhi	$3
	move	$4,$3
	lhu	$3,32($sp)
	srl	$3,$3,1
	andi	$3,$3,0xffff
	slt	$3,$4,$3
	xori	$3,$3,0x1
	andi	$3,$3,0x00ff
	addu	$2,$2,$3
	sb	$2,18($sp)
	sb	$0,17($sp)
	j	$L354
	nop

$L357:
	lbu	$2,18($sp)
	nor	$3,$0,$2
	lbu	$2,17($sp)
	sra	$2,$3,$2
	andi	$2,$2,0x1
	bne	$2,$0,$L355
	nop

	lbu	$2,17($sp)
	li	$3,7			# 0x7
	subu	$2,$3,$2
	li	$3,1			# 0x1
	sll	$2,$3,$2
	andi	$2,$2,0x00ff
	j	$L356
	nop

$L355:
	move	$2,$0
$L356:
	lbu	$3,16($sp)
	addu	$2,$2,$3
	sb	$2,16($sp)
	lbu	$2,17($sp)
	addiu	$2,$2,1
	sb	$2,17($sp)
$L354:
	lbu	$2,17($sp)
	sltu	$2,$2,8
	bne	$2,$0,$L357
	nop

$L353:
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lbu	$3,16($sp)
	sb	$3,392($2)
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16388
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	sw	$2,20($sp)
	lbu	$2,16($sp)
	sb	$2,23($sp)
	lw	$3,20($sp)
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16388
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	move	$2,$0
	lw	$31,28($sp)
	addiu	$sp,$sp,32
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	gponDevSetDBABlockSize
	.size	gponDevSetDBABlockSize, .-gponDevSetDBABlockSize
	.section	.text.gponDevGetDBABlockSize,"ax",@progbits
	.align	2
	.globl	gponDevGetDBABlockSize
	.set	nomips16
	.set	nomicromips
	.ent	gponDevGetDBABlockSize
	.type	gponDevGetDBABlockSize, @function
gponDevGetDBABlockSize:
	.frame	$sp,32,$31		# vars= 8, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-32
	sw	$31,28($sp)
	sw	$4,32($sp)
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16388
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	sw	$2,16($sp)
	lbu	$2,19($sp)
	andi	$3,$2,0xffff
	lw	$2,32($sp)
	sh	$3,0($2)
	move	$2,$0
	lw	$31,28($sp)
	addiu	$sp,$sp,32
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	gponDevGetDBABlockSize
	.size	gponDevGetDBABlockSize, .-gponDevGetDBABlockSize
	.section	.text.gponDevHardwareDyingGasp,"ax",@progbits
	.align	2
	.globl	gponDevHardwareDyingGasp
	.set	nomips16
	.set	nomicromips
	.ent	gponDevHardwareDyingGasp
	.type	gponDevHardwareDyingGasp, @function
gponDevHardwareDyingGasp:
	.frame	$sp,32,$31		# vars= 8, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-32
	sw	$31,28($sp)
	sw	$4,32($sp)
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16924
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	sw	$2,16($sp)
	lw	$2,32($sp)
	andi	$2,$2,0x00ff
	andi	$2,$2,0x1
	andi	$3,$2,0x00ff
	lw	$2,16($sp)
	ins	$2,$3,8,1
	sw	$2,16($sp)
	lw	$3,16($sp)
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16924
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	move	$2,$0
	lw	$31,28($sp)
	addiu	$sp,$sp,32
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	gponDevHardwareDyingGasp
	.size	gponDevHardwareDyingGasp, .-gponDevHardwareDyingGasp
	.section	.text.gponDevGetDyingGaspMode,"ax",@progbits
	.align	2
	.globl	gponDevGetDyingGaspMode
	.set	nomips16
	.set	nomicromips
	.ent	gponDevGetDyingGaspMode
	.type	gponDevGetDyingGaspMode, @function
gponDevGetDyingGaspMode:
	.frame	$sp,32,$31		# vars= 8, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-32
	sw	$31,28($sp)
	sw	$4,32($sp)
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16924
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	sw	$2,16($sp)
	lw	$2,16($sp)
	ext	$2,$2,8,1
	andi	$2,$2,0x00ff
	move	$3,$2
	lw	$2,32($sp)
	sw	$3,0($2)
	move	$2,$0
	lw	$31,28($sp)
	addiu	$sp,$sp,32
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	gponDevGetDyingGaspMode
	.size	gponDevGetDyingGaspMode, .-gponDevGetDyingGaspMode
	.section	.text.gponDevSetDyingGaspNum,"ax",@progbits
	.align	2
	.globl	gponDevSetDyingGaspNum
	.set	nomips16
	.set	nomicromips
	.ent	gponDevSetDyingGaspNum
	.type	gponDevSetDyingGaspNum, @function
gponDevSetDyingGaspNum:
	.frame	$sp,32,$31		# vars= 8, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-32
	sw	$31,28($sp)
	sw	$4,32($sp)
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16924
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	sw	$2,16($sp)
	lw	$2,32($sp)
	andi	$2,$2,0x00ff
	andi	$2,$2,0xf
	andi	$3,$2,0x00ff
	lw	$2,16($sp)
	ins	$2,$3,24,4
	sw	$2,16($sp)
	lw	$3,16($sp)
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16924
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	move	$2,$0
	lw	$31,28($sp)
	addiu	$sp,$sp,32
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	gponDevSetDyingGaspNum
	.size	gponDevSetDyingGaspNum, .-gponDevSetDyingGaspNum
	.section	.text.gponDevGetDyingGaspNum,"ax",@progbits
	.align	2
	.globl	gponDevGetDyingGaspNum
	.set	nomips16
	.set	nomicromips
	.ent	gponDevGetDyingGaspNum
	.type	gponDevGetDyingGaspNum, @function
gponDevGetDyingGaspNum:
	.frame	$sp,32,$31		# vars= 8, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-32
	sw	$31,28($sp)
	sw	$4,32($sp)
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16924
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	sw	$2,16($sp)
	lw	$2,16($sp)
	ext	$2,$2,24,4
	andi	$2,$2,0x00ff
	move	$3,$2
	lw	$2,32($sp)
	sw	$3,0($2)
	move	$2,$0
	lw	$31,28($sp)
	addiu	$sp,$sp,32
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	gponDevGetDyingGaspNum
	.size	gponDevGetDyingGaspNum, .-gponDevGetDyingGaspNum
	.section	.text.gponDevSetIdleGemThreshold,"ax",@progbits
	.align	2
	.globl	gponDevSetIdleGemThreshold
	.set	nomips16
	.set	nomicromips
	.ent	gponDevSetIdleGemThreshold
	.type	gponDevSetIdleGemThreshold, @function
gponDevSetIdleGemThreshold:
	.frame	$sp,32,$31		# vars= 8, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-32
	sw	$31,28($sp)
	move	$2,$4
	sh	$2,32($sp)
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16908
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	sw	$2,16($sp)
	lhu	$2,32($sp)
	sh	$2,18($sp)
	lw	$3,16($sp)
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16908
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	move	$2,$0
	lw	$31,28($sp)
	addiu	$sp,$sp,32
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	gponDevSetIdleGemThreshold
	.size	gponDevSetIdleGemThreshold, .-gponDevSetIdleGemThreshold
	.section	.text.gponDevGetIdleGemThreshold,"ax",@progbits
	.align	2
	.globl	gponDevGetIdleGemThreshold
	.set	nomips16
	.set	nomicromips
	.ent	gponDevGetIdleGemThreshold
	.type	gponDevGetIdleGemThreshold, @function
gponDevGetIdleGemThreshold:
	.frame	$sp,32,$31		# vars= 8, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-32
	sw	$31,28($sp)
	sw	$4,32($sp)
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16908
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	sw	$2,16($sp)
	lhu	$3,18($sp)
	lw	$2,32($sp)
	sh	$3,0($2)
	move	$2,$0
	lw	$31,28($sp)
	addiu	$sp,$sp,32
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	gponDevGetIdleGemThreshold
	.size	gponDevGetIdleGemThreshold, .-gponDevGetIdleGemThreshold
	.section	.text.gponDevSetCounterType,"ax",@progbits
	.align	2
	.globl	gponDevSetCounterType
	.set	nomips16
	.set	nomicromips
	.ent	gponDevSetCounterType
	.type	gponDevSetCounterType, @function
gponDevSetCounterType:
	.frame	$sp,32,$31		# vars= 8, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-32
	sw	$31,28($sp)
	sw	$4,32($sp)
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16896
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	sw	$2,16($sp)
	lw	$2,32($sp)
	andi	$2,$2,0x00ff
	andi	$2,$2,0x1
	andi	$3,$2,0x00ff
	lw	$2,16($sp)
	ins	$2,$3,26,1
	sw	$2,16($sp)
	lw	$3,16($sp)
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16896
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	move	$2,$0
	lw	$31,28($sp)
	addiu	$sp,$sp,32
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	gponDevSetCounterType
	.size	gponDevSetCounterType, .-gponDevSetCounterType
	.section	.text.gponDevGetCounterType,"ax",@progbits
	.align	2
	.globl	gponDevGetCounterType
	.set	nomips16
	.set	nomicromips
	.ent	gponDevGetCounterType
	.type	gponDevGetCounterType, @function
gponDevGetCounterType:
	.frame	$sp,32,$31		# vars= 8, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-32
	sw	$31,28($sp)
	sw	$4,32($sp)
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16896
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	sw	$2,16($sp)
	lw	$2,16($sp)
	ext	$2,$2,26,1
	andi	$2,$2,0x00ff
	move	$3,$2
	lw	$2,32($sp)
	sw	$3,0($2)
	move	$2,$0
	lw	$31,28($sp)
	addiu	$sp,$sp,32
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	gponDevGetCounterType
	.size	gponDevGetCounterType, .-gponDevGetCounterType
	.section	.text.gponDevSetResponseTime,"ax",@progbits
	.align	2
	.globl	gponDevSetResponseTime
	.set	nomips16
	.set	nomicromips
	.ent	gponDevSetResponseTime
	.type	gponDevSetResponseTime, @function
gponDevSetResponseTime:
	.frame	$sp,32,$31		# vars= 8, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-32
	sw	$31,28($sp)
	move	$2,$4
	sh	$2,32($sp)
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16556
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	sw	$2,16($sp)
	lhu	$2,32($sp)
	sh	$2,18($sp)
	lw	$3,16($sp)
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16556
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	move	$2,$0
	lw	$31,28($sp)
	addiu	$sp,$sp,32
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	gponDevSetResponseTime
	.size	gponDevSetResponseTime, .-gponDevSetResponseTime
	.section	.text.gponDevGetResponseTime,"ax",@progbits
	.align	2
	.globl	gponDevGetResponseTime
	.set	nomips16
	.set	nomicromips
	.ent	gponDevGetResponseTime
	.type	gponDevGetResponseTime, @function
gponDevGetResponseTime:
	.frame	$sp,32,$31		# vars= 8, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-32
	sw	$31,28($sp)
	sw	$4,32($sp)
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16556
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	sw	$2,16($sp)
	lhu	$3,18($sp)
	lw	$2,32($sp)
	sh	$3,0($2)
	move	$2,$0
	lw	$31,28($sp)
	addiu	$sp,$sp,32
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	gponDevGetResponseTime
	.size	gponDevGetResponseTime, .-gponDevGetResponseTime
	.section	.text.gponDevSetInternalDelayFineTune,"ax",@progbits
	.align	2
	.globl	gponDevSetInternalDelayFineTune
	.set	nomips16
	.set	nomicromips
	.ent	gponDevSetInternalDelayFineTune
	.type	gponDevSetInternalDelayFineTune, @function
gponDevSetInternalDelayFineTune:
	.frame	$sp,32,$31		# vars= 8, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-32
	sw	$31,28($sp)
	move	$2,$4
	sb	$2,32($sp)
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16904
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	sw	$2,16($sp)
	lbu	$2,32($sp)
	sb	$2,18($sp)
	lw	$3,16($sp)
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16904
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	move	$2,$0
	lw	$31,28($sp)
	addiu	$sp,$sp,32
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	gponDevSetInternalDelayFineTune
	.size	gponDevSetInternalDelayFineTune, .-gponDevSetInternalDelayFineTune
	.section	.text.gponDevGetInternalDelayFineTune,"ax",@progbits
	.align	2
	.globl	gponDevGetInternalDelayFineTune
	.set	nomips16
	.set	nomicromips
	.ent	gponDevGetInternalDelayFineTune
	.type	gponDevGetInternalDelayFineTune, @function
gponDevGetInternalDelayFineTune:
	.frame	$sp,32,$31		# vars= 8, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-32
	sw	$31,28($sp)
	sw	$4,32($sp)
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16904
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	sw	$2,16($sp)
	lbu	$3,18($sp)
	lw	$2,32($sp)
	sb	$3,0($2)
	move	$2,$0
	lw	$31,28($sp)
	addiu	$sp,$sp,32
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	gponDevGetInternalDelayFineTune
	.size	gponDevGetInternalDelayFineTune, .-gponDevGetInternalDelayFineTune
	.section	.text.gponDevClearSwCounter,"ax",@progbits
	.align	2
	.globl	gponDevClearSwCounter
	.set	nomips16
	.set	nomicromips
	.ent	gponDevClearSwCounter
	.type	gponDevClearSwCounter, @function
gponDevClearSwCounter:
	.frame	$sp,32,$31		# vars= 8, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-32
	sw	$31,28($sp)
	sw	$0,16($sp)
	j	$L386
	nop

$L388:
	lui	$2,%hi(gpWanPriv)
	lw	$3,%lo(gpWanPriv)($2)
	lw	$2,16($sp)
	sll	$2,$2,5
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	lw	$2,8284($2)
	ext	$2,$2,2,1
	andi	$2,$2,0x00ff
	beq	$2,$0,$L387
	nop

	lui	$2,%hi(gpWanPriv)
	lw	$4,%lo(gpWanPriv)($2)
	lw	$2,16($sp)
	move	$3,$2
	sll	$2,$3,5
	move	$3,$2
	sll	$2,$3,2
	subu	$2,$2,$3
	addiu	$2,$2,8280
	addu	$2,$4,$2
	addiu	$2,$2,8
	move	$4,$2
	move	$5,$0
	li	$6,92			# 0x5c
	lui	$2,%hi(memset)
	addiu	$2,$2,%lo(memset)
	jalr	$2
	nop

$L387:
	lw	$2,16($sp)
	addiu	$2,$2,1
	sw	$2,16($sp)
$L386:
	lw	$2,16($sp)
	slt	$2,$2,256
	bne	$2,$0,$L388
	nop

	move	$2,$0
	lw	$31,28($sp)
	addiu	$sp,$sp,32
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	gponDevClearSwCounter
	.size	gponDevClearSwCounter, .-gponDevClearSwCounter
	.section	.text.gponDevClearHwCounter,"ax",@progbits
	.align	2
	.globl	gponDevClearHwCounter
	.set	nomips16
	.set	nomicromips
	.ent	gponDevClearHwCounter
	.type	gponDevClearHwCounter, @function
gponDevClearHwCounter:
	.frame	$sp,40,$31		# vars= 16, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-40
	sw	$31,36($sp)
	sw	$0,16($sp)
	lui	$2,%hi(gponDevGemMibTablesInit)
	addiu	$2,$2,%lo(gponDevGemMibTablesInit)
	jalr	$2
	nop

	move	$4,$0
	move	$5,$0
	lui	$2,%hi(gponDevUpdateGemMibIdxTable)
	addiu	$2,$2,%lo(gponDevUpdateGemMibIdxTable)
	jalr	$2
	nop

	li	$2,1			# 0x1
	sw	$2,24($sp)
	sw	$0,20($sp)
	j	$L391
	nop

$L394:
	lui	$2,%hi(gpWanPriv)
	lw	$3,%lo(gpWanPriv)($2)
	lw	$2,20($sp)
	addiu	$2,$2,44
	sll	$2,$2,1
	addu	$2,$3,$2
	lhu	$2,4($2)
	sw	$2,28($sp)
	lui	$2,%hi(gpWanPriv)
	lw	$3,%lo(gpWanPriv)($2)
	lw	$2,28($sp)
	sll	$2,$2,5
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	lw	$2,8284($2)
	ext	$2,$2,2,1
	andi	$2,$2,0x00ff
	beq	$2,$0,$L392
	nop

	lw	$2,16($sp)
	addiu	$2,$2,1
	sw	$2,16($sp)
$L392:
	lw	$2,20($sp)
	andi	$3,$2,0xf
	li	$2,15			# 0xf
	bne	$3,$2,$L393
	nop

	lw	$3,24($sp)
	li	$2,256			# 0x100
	beq	$3,$2,$L393
	nop

	lw	$2,24($sp)
	andi	$2,$2,0x00ff
	move	$3,$2
	lw	$2,16($sp)
	andi	$2,$2,0xffff
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(gponDevUpdateGemMibIdxTable)
	addiu	$2,$2,%lo(gponDevUpdateGemMibIdxTable)
	jalr	$2
	nop

	lw	$2,24($sp)
	addiu	$2,$2,1
	sw	$2,24($sp)
$L393:
	lw	$2,20($sp)
	addiu	$2,$2,1
	sw	$2,20($sp)
$L391:
	lw	$2,20($sp)
	slt	$2,$2,4096
	bne	$2,$0,$L394
	nop

	move	$2,$0
	lw	$31,36($sp)
	addiu	$sp,$sp,40
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	gponDevClearHwCounter
	.size	gponDevClearHwCounter, .-gponDevClearHwCounter
	.section	.text.feDevGdm2Cdm2Stop,"ax",@progbits
	.align	2
	.globl	feDevGdm2Cdm2Stop
	.set	nomips16
	.set	nomicromips
	.ent	feDevGdm2Cdm2Stop
	.type	feDevGdm2Cdm2Stop, @function
feDevGdm2Cdm2Stop:
	.frame	$sp,256,$31		# vars= 232, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-256
	sw	$31,252($sp)
	sw	$4,256($sp)
	lw	$2,256($sp)
	beq	$2,$0,$L397
	nop

	lw	$3,256($sp)
	li	$2,1			# 0x1
	beq	$3,$2,$L397
	nop

	li	$2,-1			# 0xffffffffffffffff
	j	$L398
	nop

$L397:
	lw	$3,256($sp)
	li	$2,1			# 0x1
	bne	$3,$2,$L399
	nop

	sw	$0,16($sp)
	j	$L400
	nop

$L404:
	lw	$2,16($sp)
	andi	$2,$2,0x00ff
	move	$3,$2
	li	$2,1			# 0x1
	sw	$2,20($sp)
	sw	$0,24($sp)
	sb	$3,28($sp)
	sw	$0,32($sp)
	sw	$0,36($sp)
	li	$2,1			# 0x1
	sw	$2,96($sp)
	lw	$2,20($sp)
	sw	$2,100($sp)
	lw	$2,24($sp)
	sw	$2,116($sp)
	lbu	$2,28($sp)
	sw	$2,124($sp)
	lw	$2,32($sp)
	sw	$2,148($sp)
	addiu	$2,$sp,96
	li	$4,18			# 0x12
	move	$5,$0
	move	$6,$2
	lui	$2,%hi(__ECNT_HOOK)
	addiu	$2,$2,%lo(__ECNT_HOOK)
	jalr	$2
	nop

	sw	$2,36($sp)
	lw	$2,16($sp)
	addiu	$2,$2,1
	sw	$2,16($sp)
$L400:
	lw	$2,16($sp)
	slt	$2,$2,32
	bne	$2,$0,$L404
	nop

	sw	$0,16($sp)
	j	$L405
	nop

$L409:
	lw	$2,16($sp)
	andi	$2,$2,0x00ff
	move	$3,$2
	li	$2,1			# 0x1
	sw	$2,40($sp)
	li	$2,1			# 0x1
	sw	$2,44($sp)
	sb	$3,48($sp)
	sw	$0,52($sp)
	sw	$0,56($sp)
	li	$2,1			# 0x1
	sw	$2,96($sp)
	lw	$2,40($sp)
	sw	$2,100($sp)
	lw	$2,44($sp)
	sw	$2,116($sp)
	lbu	$2,48($sp)
	sw	$2,124($sp)
	lw	$2,52($sp)
	sw	$2,148($sp)
	addiu	$2,$sp,96
	li	$4,18			# 0x12
	move	$5,$0
	move	$6,$2
	lui	$2,%hi(__ECNT_HOOK)
	addiu	$2,$2,%lo(__ECNT_HOOK)
	jalr	$2
	nop

	sw	$2,56($sp)
	lw	$2,16($sp)
	addiu	$2,$2,1
	sw	$2,16($sp)
$L405:
	lw	$2,16($sp)
	slt	$2,$2,16
	bne	$2,$0,$L409
	nop

	sw	$0,16($sp)
	j	$L410
	nop

$L414:
	lw	$2,16($sp)
	andi	$2,$2,0x00ff
	move	$3,$2
	li	$2,1			# 0x1
	sw	$2,60($sp)
	sb	$3,64($sp)
	sw	$0,68($sp)
	sw	$0,72($sp)
	li	$2,3			# 0x3
	sw	$2,96($sp)
	lw	$2,60($sp)
	sw	$2,104($sp)
	lbu	$2,64($sp)
	sw	$2,124($sp)
	lw	$2,68($sp)
	sw	$2,148($sp)
	addiu	$2,$sp,96
	li	$4,18			# 0x12
	move	$5,$0
	move	$6,$2
	lui	$2,%hi(__ECNT_HOOK)
	addiu	$2,$2,%lo(__ECNT_HOOK)
	jalr	$2
	nop

	sw	$2,72($sp)
	lw	$2,16($sp)
	addiu	$2,$2,1
	sw	$2,16($sp)
$L410:
	lw	$2,16($sp)
	slt	$2,$2,32
	bne	$2,$0,$L414
	nop

	j	$L415
	nop

$L399:
	sw	$0,16($sp)
	j	$L416
	nop

$L420:
	lw	$2,16($sp)
	andi	$2,$2,0x00ff
	move	$3,$2
	li	$2,1			# 0x1
	sw	$2,76($sp)
	li	$2,1			# 0x1
	sw	$2,80($sp)
	sb	$3,84($sp)
	li	$2,1			# 0x1
	sw	$2,88($sp)
	sw	$0,92($sp)
	li	$2,1			# 0x1
	sw	$2,96($sp)
	lw	$2,76($sp)
	sw	$2,100($sp)
	lw	$2,80($sp)
	sw	$2,116($sp)
	lbu	$2,84($sp)
	sw	$2,124($sp)
	lw	$2,88($sp)
	sw	$2,148($sp)
	addiu	$2,$sp,96
	li	$4,18			# 0x12
	move	$5,$0
	move	$6,$2
	lui	$2,%hi(__ECNT_HOOK)
	addiu	$2,$2,%lo(__ECNT_HOOK)
	jalr	$2
	nop

	sw	$2,92($sp)
	lw	$2,16($sp)
	addiu	$2,$2,1
	sw	$2,16($sp)
$L416:
	lw	$2,16($sp)
	slt	$2,$2,2
	bne	$2,$0,$L420
	nop

$L415:
	move	$2,$0
$L398:
	lw	$31,252($sp)
	addiu	$sp,$sp,256
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	feDevGdm2Cdm2Stop
	.size	feDevGdm2Cdm2Stop, .-feDevGdm2Cdm2Stop
	.section	.text.gponDevMbiStop,"ax",@progbits
	.align	2
	.globl	gponDevMbiStop
	.set	nomips16
	.set	nomicromips
	.ent	gponDevMbiStop
	.type	gponDevMbiStop, @function
gponDevMbiStop:
	.frame	$sp,32,$31		# vars= 8, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-32
	sw	$31,28($sp)
	sw	$4,32($sp)
	lw	$2,32($sp)
	beq	$2,$0,$L426
	nop

	lw	$3,32($sp)
	li	$2,1			# 0x1
	beq	$3,$2,$L426
	nop

	li	$2,-1			# 0xffffffffffffffff
	j	$L428
	nop

$L426:
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16736
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	sw	$2,16($sp)
	lw	$2,32($sp)
	sltu	$2,$0,$2
	andi	$3,$2,0x00ff
	lw	$2,16($sp)
	ins	$2,$3,0,1
	sw	$2,16($sp)
	lw	$2,32($sp)
	sltu	$2,$0,$2
	andi	$3,$2,0x00ff
	lw	$2,16($sp)
	ins	$2,$3,8,1
	sw	$2,16($sp)
	lw	$3,16($sp)
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16736
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	li	$4,1000			# 0x3e8
	lui	$2,%hi(__udelay)
	addiu	$2,$2,%lo(__udelay)
	jalr	$2
	nop

	lw	$4,32($sp)
	lui	$2,%hi(feDevGdm2Cdm2Stop)
	addiu	$2,$2,%lo(feDevGdm2Cdm2Stop)
	jalr	$2
	nop

	li	$4,1000			# 0x3e8
	lui	$2,%hi(__udelay)
	addiu	$2,$2,%lo(__udelay)
	jalr	$2
	nop

	move	$2,$0
$L428:
	lw	$31,28($sp)
	addiu	$sp,$sp,32
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	gponDevMbiStop
	.size	gponDevMbiStop, .-gponDevMbiStop
	.section	.text.gpon_INT_init,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	gpon_INT_init
	.type	gpon_INT_init, @function
gpon_INT_init:
	.frame	$sp,32,$31		# vars= 8, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-32
	sw	$31,28($sp)
	sw	$0,16($sp)
	lw	$2,16($sp)
	li	$3,1			# 0x1
	ins	$2,$3,30,1
	sw	$2,16($sp)
	lw	$2,16($sp)
	li	$3,1			# 0x1
	ins	$2,$3,29,1
	sw	$2,16($sp)
	lw	$2,16($sp)
	li	$3,1			# 0x1
	ins	$2,$3,28,1
	sw	$2,16($sp)
	lw	$2,16($sp)
	li	$3,1			# 0x1
	ins	$2,$3,26,1
	sw	$2,16($sp)
	lw	$2,16($sp)
	li	$3,1			# 0x1
	ins	$2,$3,25,1
	sw	$2,16($sp)
	lw	$2,16($sp)
	li	$3,1			# 0x1
	ins	$2,$3,24,1
	sw	$2,16($sp)
	lw	$2,16($sp)
	li	$3,1			# 0x1
	ins	$2,$3,23,1
	sw	$2,16($sp)
	lw	$2,16($sp)
	li	$3,1			# 0x1
	ins	$2,$3,22,1
	sw	$2,16($sp)
	lw	$2,16($sp)
	li	$3,1			# 0x1
	ins	$2,$3,21,1
	sw	$2,16($sp)
	lw	$2,16($sp)
	li	$3,1			# 0x1
	ins	$2,$3,20,1
	sw	$2,16($sp)
	lw	$2,16($sp)
	li	$3,1			# 0x1
	ins	$2,$3,19,1
	sw	$2,16($sp)
	lw	$2,16($sp)
	li	$3,1			# 0x1
	ins	$2,$3,18,1
	sw	$2,16($sp)
	lw	$2,16($sp)
	li	$3,1			# 0x1
	ins	$2,$3,17,1
	sw	$2,16($sp)
	lw	$2,16($sp)
	li	$3,1			# 0x1
	ins	$2,$3,16,1
	sw	$2,16($sp)
	lw	$2,16($sp)
	li	$3,1			# 0x1
	ins	$2,$3,11,1
	sw	$2,16($sp)
	lw	$2,16($sp)
	ins	$2,$0,10,1
	sw	$2,16($sp)
	lw	$2,16($sp)
	li	$3,1			# 0x1
	ins	$2,$3,9,1
	sw	$2,16($sp)
	lw	$2,16($sp)
	li	$3,1			# 0x1
	ins	$2,$3,8,1
	sw	$2,16($sp)
	lw	$2,16($sp)
	li	$3,1			# 0x1
	ins	$2,$3,7,1
	sw	$2,16($sp)
	lw	$2,16($sp)
	li	$3,1			# 0x1
	ins	$2,$3,6,1
	sw	$2,16($sp)
	lw	$2,16($sp)
	li	$3,1			# 0x1
	ins	$2,$3,5,1
	sw	$2,16($sp)
	lw	$2,16($sp)
	li	$3,1			# 0x1
	ins	$2,$3,4,1
	sw	$2,16($sp)
	lw	$2,16($sp)
	li	$3,1			# 0x1
	ins	$2,$3,3,1
	sw	$2,16($sp)
	lw	$2,16($sp)
	li	$3,1			# 0x1
	ins	$2,$3,2,1
	sw	$2,16($sp)
	lw	$2,16($sp)
	ins	$2,$0,1,1
	sw	$2,16($sp)
	lw	$2,16($sp)
	li	$3,1			# 0x1
	ins	$2,$3,0,1
	sw	$2,16($sp)
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16392
	li	$4,-1			# 0xffffffffffffffff
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	lw	$3,16($sp)
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16396
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	lw	$31,28($sp)
	addiu	$sp,$sp,32
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	gpon_INT_init
	.size	gpon_INT_init, .-gpon_INT_init
	.section	.text.gpon_dev_init,"ax",@progbits
	.align	2
	.globl	gpon_dev_init
	.set	nomips16
	.set	nomicromips
	.ent	gpon_dev_init
	.type	gpon_dev_init, @function
gpon_dev_init:
	.frame	$sp,32,$31		# vars= 8, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-32
	sw	$31,28($sp)
	sw	$0,16($sp)
	li	$4,1			# 0x1
	lui	$2,%hi(select_xpon_wan)
	addiu	$2,$2,%lo(select_xpon_wan)
	jalr	$2
	nop

	lui	$2,%hi(gpon_INT_init)
	addiu	$2,$2,%lo(gpon_INT_init)
	jalr	$2
	nop

	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	lw	$3,-30828($2)
	li	$2,-2147483648			# 0xffffffff80000000
	and	$2,$3,$2
	beq	$2,$0,$L431
	nop

	li	$4,1			# 0x1
	lui	$2,%hi(gponDevHardwareDyingGasp)
	addiu	$2,$2,%lo(gponDevHardwareDyingGasp)
	jalr	$2
	nop

	j	$L432
	nop

$L431:
	move	$4,$0
	lui	$2,%hi(gponDevHardwareDyingGasp)
	addiu	$2,$2,%lo(gponDevHardwareDyingGasp)
	jalr	$2
	nop

$L432:
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lhu	$2,296($2)
	move	$4,$2
	lui	$2,%hi(gponDevSetIdleGemThreshold)
	addiu	$2,$2,%lo(gponDevSetIdleGemThreshold)
	jalr	$2
	nop

	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lw	$2,300($2)
	move	$4,$2
	lui	$2,%hi(gponDevSetCounterType)
	addiu	$2,$2,%lo(gponDevSetCounterType)
	jalr	$2
	nop

	li	$2,-1078984704			# 0xffffffffbfb00000
	ori	$2,$2,0x64
	lw	$3,0($2)
	li	$2,-65536			# 0xffffffffffff0000
	and	$3,$3,$2
	li	$2,524288			# 0x80000
	bne	$3,$2,$L433
	nop

	li	$4,48			# 0x30
	lui	$2,%hi(gponDevSetDBABlockSize)
	addiu	$2,$2,%lo(gponDevSetDBABlockSize)
	jalr	$2
	nop

$L433:
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lw	$2,304($2)
	move	$4,$2
	lui	$2,%hi(modify_sleep_count)
	addiu	$2,$2,%lo(modify_sleep_count)
	jalr	$2
	nop

	li	$4,10			# 0xa
	lui	$2,%hi(gponDevSetTodClkPeriod)
	addiu	$2,$2,%lo(gponDevSetTodClkPeriod)
	jalr	$2
	nop

	move	$2,$0
	lw	$31,28($sp)
	addiu	$sp,$sp,32
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	gpon_dev_init
	.size	gpon_dev_init, .-gpon_dev_init
	.rdata
	.align	2
$LC134:
	.ascii	"ERROR: Input Para wrong, channelId: %d  allocId: %d\012\000"
	.align	2
$LC135:
	.ascii	"ERROR: Tcont info in reg is wrong,channelId %d, valid %d"
	.ascii	", alloc %d\012\000"
	.section	.text.gponDevCheckTContReg,"ax",@progbits
	.align	2
	.globl	gponDevCheckTContReg
	.set	nomips16
	.set	nomicromips
	.ent	gponDevCheckTContReg
	.type	gponDevCheckTContReg, @function
gponDevCheckTContReg:
	.frame	$sp,32,$31		# vars= 8, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-32
	sw	$31,28($sp)
	move	$3,$4
	move	$2,$5
	sh	$3,32($sp)
	sh	$2,36($sp)
	sw	$0,16($sp)
	lhu	$2,32($sp)
	sltu	$2,$2,32
	beq	$2,$0,$L436
	nop

	lhu	$2,36($sp)
	sltu	$2,$2,4096
	bne	$2,$0,$L437
	nop

$L436:
	lhu	$5,32($sp)
	lhu	$3,36($sp)
	lui	$2,%hi($LC134)
	addiu	$4,$2,%lo($LC134)
	move	$6,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	move	$2,$0
	j	$L442
	nop

$L437:
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$3,$2,16416
	lhu	$2,32($sp)
	srl	$2,$2,1
	andi	$2,$2,0xffff
	sll	$2,$2,2
	addu	$2,$3,$2
	sw	$2,16($sp)
	lw	$2,16($sp)
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	sw	$2,20($sp)
	lhu	$2,32($sp)
	andi	$2,$2,0x1
	andi	$2,$2,0xffff
	bne	$2,$0,$L439
	nop

	lw	$2,20($sp)
	andi	$2,$2,0x8000
	beq	$2,$0,$L440
	nop

	lw	$2,20($sp)
	ext	$2,$2,0,12
	andi	$2,$2,0xffff
	move	$3,$2
	lhu	$2,36($sp)
	bne	$3,$2,$L440
	nop

	li	$2,1			# 0x1
	j	$L442
	nop

$L440:
	lhu	$3,32($sp)
	lw	$2,20($sp)
	ext	$2,$2,15,1
	andi	$2,$2,0x00ff
	move	$6,$2
	lw	$2,20($sp)
	ext	$2,$2,0,12
	andi	$2,$2,0xffff
	move	$7,$2
	lui	$2,%hi($LC135)
	addiu	$4,$2,%lo($LC135)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	move	$2,$0
	j	$L442
	nop

$L439:
	lw	$3,20($sp)
	li	$2,-2147483648			# 0xffffffff80000000
	and	$2,$3,$2
	beq	$2,$0,$L441
	nop

	lw	$2,20($sp)
	ext	$2,$2,16,12
	andi	$2,$2,0xffff
	move	$3,$2
	lhu	$2,36($sp)
	bne	$3,$2,$L441
	nop

	li	$2,1			# 0x1
	j	$L442
	nop

$L441:
	lhu	$3,32($sp)
	lw	$2,20($sp)
	ext	$2,$2,31,1
	andi	$2,$2,0x00ff
	move	$6,$2
	lw	$2,20($sp)
	ext	$2,$2,16,12
	andi	$2,$2,0xffff
	move	$7,$2
	lui	$2,%hi($LC135)
	addiu	$4,$2,%lo($LC135)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	move	$2,$0
$L442:
	lw	$31,28($sp)
	addiu	$sp,$sp,32
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	gponDevCheckTContReg
	.size	gponDevCheckTContReg, .-gponDevCheckTContReg
	.rdata
	.align	2
	.type	__func__.31792, @object
	.size	__func__.31792, 27
__func__.31792:
	.ascii	"gponDevSetAesKeySwitchBySw\000"
	.align	2
	.type	__func__.31886, @object
	.size	__func__.31886, 21
__func__.31886:
	.ascii	"gponDevSetTCont16_31\000"
	.align	2
	.type	__func__.31898, @object
	.size	__func__.31898, 21
__func__.31898:
	.ascii	"gponDevGetTCont16_31\000"
	.align	2
	.type	__func__.32060, @object
	.size	__func__.32060, 20
__func__.32060:
	.ascii	"gpon_dev_init_reset\000"
	.align	2
$LC1:
	.word	1069276568
	.align	2
$LC2:
	.word	1069280762
	.align	2
$LC3:
	.word	1191753728
	.align	2
$LC4:
	.word	1067401379
	.ident	"GCC: (Buildroot 2015.08.1) 4.9.3"
