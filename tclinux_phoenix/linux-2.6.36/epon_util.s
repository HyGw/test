	.file	1 "epon_util.c"
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
	.local	msg
	.comm	msg,256,4
	.rdata
	.align	2
$LC0:
	.ascii	"[%d]\000"
	.section	.text.eponDbgPrint,"ax",@progbits
	.align	2
	.globl	eponDbgPrint
	.set	nomips16
	.set	nomicromips
	.ent	eponDbgPrint
	.type	eponDbgPrint, @function
eponDbgPrint:
	.frame	$sp,32,$31		# vars= 8, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-32
	sw	$31,28($sp)
	sw	$4,32($sp)
	sw	$6,40($sp)
	sw	$7,44($sp)
	sw	$5,36($sp)
	lui	$2,%hi(eponDebugLevel)
	lw	$3,%lo(eponDebugLevel)($2)
	lw	$2,32($sp)
	sltu	$2,$3,$2
	bne	$2,$0,$L2
	nop

	addiu	$2,$sp,40
	sw	$2,16($sp)
	lw	$3,16($sp)
	lui	$2,%hi(msg)
	addiu	$4,$2,%lo(msg)
	li	$5,256			# 0x100
	lw	$6,36($sp)
	move	$7,$3
	lui	$2,%hi(vsnprintf)
	addiu	$2,$2,%lo(vsnprintf)
	jalr	$2
	nop

	lui	$2,%hi(msg)
	addiu	$4,$2,%lo(msg)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi(jiffies)
	lw	$3,%lo(jiffies)($2)
	lui	$2,%hi($LC0)
	addiu	$4,$2,%lo($LC0)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L2:
	nop
	lw	$31,28($sp)
	addiu	$sp,$sp,32
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	eponDbgPrint
	.size	eponDbgPrint, .-eponDbgPrint
	.rdata
	.align	2
$LC1:
	.ascii	"%02x:%02x:%02x:%02x:%02x:%02x\000"
	.section	.text.strToMacNum,"ax",@progbits
	.align	2
	.globl	strToMacNum
	.set	nomips16
	.set	nomicromips
	.ent	strToMacNum
	.type	strToMacNum, @function
strToMacNum:
	.frame	$sp,72,$31		# vars= 32, regs= 1/0, args= 32, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-72
	sw	$31,68($sp)
	sw	$4,72($sp)
	sw	$5,76($sp)
	addiu	$6,$sp,36
	addiu	$2,$sp,36
	addiu	$3,$2,4
	addiu	$2,$sp,36
	addiu	$2,$2,8
	sw	$2,16($sp)
	addiu	$2,$sp,36
	addiu	$2,$2,12
	sw	$2,20($sp)
	addiu	$2,$sp,36
	addiu	$2,$2,16
	sw	$2,24($sp)
	addiu	$2,$sp,36
	addiu	$2,$2,20
	sw	$2,28($sp)
	lw	$4,72($sp)
	lui	$2,%hi($LC1)
	addiu	$5,$2,%lo($LC1)
	move	$7,$3
	lui	$2,%hi(sscanf)
	addiu	$2,$2,%lo(sscanf)
	jalr	$2
	nop

	sw	$0,32($sp)
	j	$L5
	nop

$L6:
	lw	$2,32($sp)
	lw	$3,76($sp)
	addu	$3,$3,$2
	lw	$2,32($sp)
	sll	$2,$2,2
	addiu	$4,$sp,32
	addu	$2,$4,$2
	lw	$2,4($2)
	andi	$2,$2,0x00ff
	sb	$2,0($3)
	lw	$2,32($sp)
	addiu	$2,$2,1
	sw	$2,32($sp)
$L5:
	lw	$2,32($sp)
	slt	$2,$2,6
	bne	$2,$0,$L6
	nop

	move	$2,$0
	lw	$31,68($sp)
	addiu	$sp,$sp,72
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	strToMacNum
	.size	strToMacNum, .-strToMacNum
	.section	.text.get16,"ax",@progbits
	.align	2
	.globl	get16
	.set	nomips16
	.set	nomicromips
	.ent	get16
	.type	get16, @function
get16:
	.frame	$sp,8,$31		# vars= 0, regs= 1/0, args= 0, gp= 0
	.mask	0x00010000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-8
	sw	$16,4($sp)
	sw	$4,8($sp)
	lw	$2,8($sp)
	addiu	$3,$2,1
	sw	$3,8($sp)
	lbu	$2,0($2)
	andi	$16,$2,0xffff
	sll	$2,$16,8
	andi	$16,$2,0xffff
	lw	$2,8($sp)
	lbu	$2,0($2)
	andi	$2,$2,0xffff
	or	$2,$2,$16
	andi	$16,$2,0xffff
	move	$2,$16
	lw	$16,4($sp)
	addiu	$sp,$sp,8
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	get16
	.size	get16, .-get16
	.section	.text.get32,"ax",@progbits
	.align	2
	.globl	get32
	.set	nomips16
	.set	nomicromips
	.ent	get32
	.type	get32, @function
get32:
	.frame	$sp,8,$31		# vars= 8, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-8
	sw	$4,8($sp)
	lw	$2,8($sp)
	addiu	$3,$2,1
	sw	$3,8($sp)
	lbu	$2,0($2)
	sw	$2,0($sp)
	lw	$2,0($sp)
	sll	$2,$2,8
	sw	$2,0($sp)
	lw	$2,8($sp)
	addiu	$3,$2,1
	sw	$3,8($sp)
	lbu	$2,0($2)
	move	$3,$2
	lw	$2,0($sp)
	or	$2,$2,$3
	sw	$2,0($sp)
	lw	$2,0($sp)
	sll	$2,$2,8
	sw	$2,0($sp)
	lw	$2,8($sp)
	addiu	$3,$2,1
	sw	$3,8($sp)
	lbu	$2,0($2)
	move	$3,$2
	lw	$2,0($sp)
	or	$2,$2,$3
	sw	$2,0($sp)
	lw	$2,0($sp)
	sll	$2,$2,8
	sw	$2,0($sp)
	lw	$2,8($sp)
	lbu	$2,0($2)
	move	$3,$2
	lw	$2,0($sp)
	or	$2,$2,$3
	sw	$2,0($sp)
	lw	$2,0($sp)
	addiu	$sp,$sp,8
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	get32
	.size	get32, .-get32
	.section	.text.put32,"ax",@progbits
	.align	2
	.globl	put32
	.set	nomips16
	.set	nomicromips
	.ent	put32
	.type	put32, @function
put32:
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	sw	$4,0($sp)
	sw	$5,4($sp)
	lw	$2,0($sp)
	addiu	$3,$2,1
	sw	$3,0($sp)
	lw	$3,4($sp)
	srl	$3,$3,24
	andi	$3,$3,0x00ff
	sb	$3,0($2)
	lw	$2,0($sp)
	addiu	$3,$2,1
	sw	$3,0($sp)
	lw	$3,4($sp)
	srl	$3,$3,16
	andi	$3,$3,0x00ff
	sb	$3,0($2)
	lw	$2,0($sp)
	addiu	$3,$2,1
	sw	$3,0($sp)
	lw	$3,4($sp)
	srl	$3,$3,8
	andi	$3,$3,0x00ff
	sb	$3,0($2)
	lw	$2,0($sp)
	addiu	$3,$2,1
	sw	$3,0($sp)
	lw	$3,4($sp)
	andi	$3,$3,0x00ff
	sb	$3,0($2)
	lw	$2,0($sp)
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	put32
	.size	put32, .-put32
	.section	.text.put16,"ax",@progbits
	.align	2
	.globl	put16
	.set	nomips16
	.set	nomicromips
	.ent	put16
	.type	put16, @function
put16:
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	sw	$4,0($sp)
	move	$2,$5
	sh	$2,4($sp)
	lw	$2,0($sp)
	addiu	$3,$2,1
	sw	$3,0($sp)
	lhu	$3,4($sp)
	srl	$3,$3,8
	andi	$3,$3,0xffff
	andi	$3,$3,0x00ff
	sb	$3,0($2)
	lw	$2,0($sp)
	addiu	$3,$2,1
	sw	$3,0($sp)
	lhu	$3,4($sp)
	andi	$3,$3,0x00ff
	sb	$3,0($2)
	lw	$2,0($sp)
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	put16
	.size	put16, .-put16
	.ident	"GCC: (Buildroot 2015.08.1) 4.9.3"
