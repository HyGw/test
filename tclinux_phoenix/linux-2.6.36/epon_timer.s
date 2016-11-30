	.file	1 "epon_timer.c"
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
	.ascii	"\015\012enter eponMacAddTimer\000"
	.align	2
$LC1:
	.ascii	"\015\012eponMacAddTimer param error\000"
#NO_APP
	.section	.text.eponMacAddTimer,"ax",@progbits
	.align	2
	.globl	eponMacAddTimer
	.set	nomips16
	.set	nomicromips
	.ent	eponMacAddTimer
	.type	eponMacAddTimer, @function
eponMacAddTimer:
	.frame	$sp,24,$31		# vars= 0, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-24
	sw	$31,20($sp)
	sw	$4,24($sp)
	sw	$5,28($sp)
	sw	$6,32($sp)
	sw	$7,36($sp)
	li	$4,4			# 0x4
	lui	$2,%hi($LC0)
	addiu	$5,$2,%lo($LC0)
	lui	$2,%hi(eponDbgPrint)
	addiu	$2,$2,%lo(eponDbgPrint)
	jalr	$2
	nop

	lw	$2,24($sp)
	beq	$2,$0,$L2
	nop

	lw	$2,32($sp)
	bne	$2,$0,$L3
	nop

$L2:
	li	$4,4			# 0x4
	lui	$2,%hi($LC1)
	addiu	$5,$2,%lo($LC1)
	lui	$2,%hi(eponDbgPrint)
	addiu	$2,$2,%lo(eponDbgPrint)
	jalr	$2
	nop

	li	$2,-1			# 0xffffffffffffffff
	j	$L4
	nop

$L3:
	lw	$4,24($sp)
	move	$5,$0
	move	$6,$0
	lui	$2,%hi(init_timer_key)
	addiu	$2,$2,%lo(init_timer_key)
	jalr	$2
	nop

	li	$3,100			# 0x64
	lw	$2,28($sp)
	divu	$0,$3,$2
	mfhi	$3
	mflo	$2
	move	$3,$2
	lui	$2,%hi(jiffies)
	lw	$2,%lo(jiffies)($2)
	addu	$3,$3,$2
	lw	$2,24($sp)
	sw	$3,8($2)
	lw	$2,24($sp)
	lw	$3,32($sp)
	sw	$3,16($2)
	lw	$2,24($sp)
	lw	$3,36($sp)
	sw	$3,20($2)
	lw	$4,24($sp)
	lui	$2,%hi(add_timer)
	addiu	$2,$2,%lo(add_timer)
	jalr	$2
	nop

	move	$2,$0
$L4:
	lw	$31,20($sp)
	addiu	$sp,$sp,24
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	eponMacAddTimer
	.size	eponMacAddTimer, .-eponMacAddTimer
	.section	.text.eponMacDelTimer,"ax",@progbits
	.align	2
	.globl	eponMacDelTimer
	.set	nomips16
	.set	nomicromips
	.ent	eponMacDelTimer
	.type	eponMacDelTimer, @function
eponMacDelTimer:
	.frame	$sp,24,$31		# vars= 0, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-24
	sw	$31,20($sp)
	sw	$4,24($sp)
	lw	$4,24($sp)
	lui	$2,%hi(del_timer_sync)
	addiu	$2,$2,%lo(del_timer_sync)
	jalr	$2
	nop

	lw	$31,20($sp)
	addiu	$sp,$sp,24
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	eponMacDelTimer
	.size	eponMacDelTimer, .-eponMacDelTimer
	.ident	"GCC: (Buildroot 2015.08.1) 4.9.3"
