	.file	1 "gpon_recovery.c"
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
	.globl	gponRecovery
	.section	.bss,"aw",@nobits
	.align	2
	.type	gponRecovery, @object
	.size	gponRecovery, 2188
gponRecovery:
	.space	2188
	.section	.text.gpon_recovery_init,"ax",@progbits
	.align	2
	.globl	gpon_recovery_init
	.set	nomips16
	.set	nomicromips
	.ent	gpon_recovery_init
	.type	gpon_recovery_init, @function
gpon_recovery_init:
	.frame	$sp,8,$31		# vars= 8, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-8
	lui	$2,%hi(gponRecovery)
	addiu	$2,$2,%lo(gponRecovery)
	sw	$0,2180($2)
	lui	$2,%hi(gponRecovery)
	addiu	$2,$2,%lo(gponRecovery)
	sw	$0,2176($2)
	sw	$0,0($sp)
	j	$L2
	nop

$L3:
	lui	$2,%hi(gponRecovery)
	lw	$3,0($sp)
	sll	$3,$3,2
	addiu	$2,$2,%lo(gponRecovery)
	addu	$2,$3,$2
	li	$3,255			# 0xff
	sw	$3,0($2)
	lw	$2,0($sp)
	addiu	$2,$2,1
	sw	$2,0($sp)
$L2:
	lw	$2,0($sp)
	slt	$2,$2,32
	bne	$2,$0,$L3
	nop

	lui	$2,%hi(gponRecovery)
	addiu	$2,$2,%lo(gponRecovery)
	sb	$0,2184($2)
	addiu	$sp,$sp,8
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	gpon_recovery_init
	.size	gpon_recovery_init, .-gpon_recovery_init
	.rdata
	.align	2
$LC0:
	.ascii	"%s [%d]: \000"
	.align	2
$LC1:
	.ascii	"allocId[%d] = %d\012\000"
	.section	.text.gpon_recover_backup_allocId,"ax",@progbits
	.align	2
	.globl	gpon_recover_backup_allocId
	.set	nomips16
	.set	nomicromips
	.ent	gpon_recover_backup_allocId
	.type	gpon_recover_backup_allocId, @function
gpon_recover_backup_allocId:
	.frame	$sp,40,$31		# vars= 16, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-40
	sw	$31,36($sp)
	move	$2,$4
	sh	$2,40($sp)
	lhu	$2,40($sp)
	sh	$2,16($sp)
	sw	$0,20($sp)
	sw	$0,24($sp)
	sw	$0,20($sp)
	j	$L5
	nop

$L8:
	lui	$2,%hi(gponRecovery)
	lw	$3,20($sp)
	sll	$3,$3,2
	addiu	$2,$2,%lo(gponRecovery)
	addu	$2,$3,$2
	lw	$3,0($2)
	lhu	$2,16($sp)
	bne	$3,$2,$L6
	nop

	lw	$2,20($sp)
	sll	$3,$2,2
	lui	$2,%hi(gponRecovery)
	addiu	$2,$2,%lo(gponRecovery)
	addu	$2,$3,$2
	sw	$2,24($sp)
	j	$L7
	nop

$L6:
	lw	$2,20($sp)
	addiu	$2,$2,1
	sw	$2,20($sp)
$L5:
	lui	$2,%hi(gponRecovery)
	addiu	$2,$2,%lo(gponRecovery)
	lw	$2,2180($2)
	lw	$3,20($sp)
	slt	$2,$3,$2
	bne	$2,$0,$L8
	nop

$L7:
	lw	$2,24($sp)
	beq	$2,$0,$L10
	nop

	li	$2,-1			# 0xffffffffffffffff
	j	$L11
	nop

$L10:
	lui	$2,%hi(gponRecovery)
	addiu	$2,$2,%lo(gponRecovery)
	lw	$4,2180($2)
	lhu	$3,40($sp)
	lui	$2,%hi(gponRecovery)
	sll	$4,$4,2
	addiu	$2,$2,%lo(gponRecovery)
	addu	$2,$4,$2
	sw	$3,0($2)
	lui	$2,%hi(gponRecovery)
	addiu	$2,$2,%lo(gponRecovery)
	lbu	$2,2184($2)
	beq	$2,$0,$L12
	nop

	lui	$2,%hi($LC0)
	addiu	$4,$2,%lo($LC0)
	lui	$2,%hi(__func__.24904)
	addiu	$5,$2,%lo(__func__.24904)
	li	$6,89			# 0x59
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi(gponRecovery)
	addiu	$2,$2,%lo(gponRecovery)
	lw	$5,2180($2)
	lhu	$3,40($sp)
	lui	$2,%hi($LC1)
	addiu	$4,$2,%lo($LC1)
	move	$6,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L12:
	lui	$2,%hi(gponRecovery)
	addiu	$2,$2,%lo(gponRecovery)
	lw	$2,2180($2)
	addiu	$3,$2,1
	lui	$2,%hi(gponRecovery)
	addiu	$2,$2,%lo(gponRecovery)
	sw	$3,2180($2)
	move	$2,$0
$L11:
	lw	$31,36($sp)
	addiu	$sp,$sp,40
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	gpon_recover_backup_allocId
	.size	gpon_recover_backup_allocId, .-gpon_recover_backup_allocId
	.section	.text.gpon_recover_create_allocId,"ax",@progbits
	.align	2
	.globl	gpon_recover_create_allocId
	.set	nomips16
	.set	nomicromips
	.ent	gpon_recover_create_allocId
	.type	gpon_recover_create_allocId, @function
gpon_recover_create_allocId:
	.frame	$sp,32,$31		# vars= 8, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-32
	sw	$31,28($sp)
	sw	$0,16($sp)
	sw	$0,20($sp)
	sw	$0,16($sp)
	j	$L14
	nop

$L17:
	lui	$2,%hi(gponRecovery)
	lw	$3,16($sp)
	sll	$3,$3,2
	addiu	$2,$2,%lo(gponRecovery)
	addu	$2,$3,$2
	lw	$2,0($2)
	andi	$2,$2,0xffff
	move	$4,$2
	lui	$2,%hi(gwan_create_new_tcont)
	addiu	$2,$2,%lo(gwan_create_new_tcont)
	jalr	$2
	nop

	beq	$2,$0,$L15
	nop

	li	$2,-1			# 0xffffffffffffffff
	sw	$2,20($sp)
	j	$L16
	nop

$L15:
	lui	$2,%hi(gponRecovery)
	addiu	$2,$2,%lo(gponRecovery)
	lbu	$2,2184($2)
	beq	$2,$0,$L16
	nop

	lui	$2,%hi($LC0)
	addiu	$4,$2,%lo($LC0)
	lui	$2,%hi(__func__.24911)
	addiu	$5,$2,%lo(__func__.24911)
	li	$6,117			# 0x75
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi(gponRecovery)
	lw	$3,16($sp)
	sll	$3,$3,2
	addiu	$2,$2,%lo(gponRecovery)
	addu	$2,$3,$2
	lw	$3,0($2)
	lui	$2,%hi($LC1)
	addiu	$4,$2,%lo($LC1)
	lw	$5,16($sp)
	move	$6,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L16:
	lw	$2,16($sp)
	addiu	$2,$2,1
	sw	$2,16($sp)
$L14:
	lui	$2,%hi(gponRecovery)
	addiu	$2,$2,%lo(gponRecovery)
	lw	$3,2180($2)
	lw	$2,16($sp)
	slt	$2,$2,$3
	bne	$2,$0,$L17
	nop

	lw	$2,20($sp)
	lw	$31,28($sp)
	addiu	$sp,$sp,32
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	gpon_recover_create_allocId
	.size	gpon_recover_create_allocId, .-gpon_recover_create_allocId
	.rdata
	.align	2
$LC2:
	.ascii	"Delete gemport success\012\000"
	.align	2
$LC3:
	.ascii	"allocId = %d\012\000"
	.section	.text.gpon_recover_delete_allocId,"ax",@progbits
	.align	2
	.globl	gpon_recover_delete_allocId
	.set	nomips16
	.set	nomicromips
	.ent	gpon_recover_delete_allocId
	.type	gpon_recover_delete_allocId, @function
gpon_recover_delete_allocId:
	.frame	$sp,56,$31		# vars= 32, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-56
	sw	$31,52($sp)
	move	$2,$4
	sh	$2,56($sp)
	sw	$0,16($sp)
	lui	$2,%hi(gponRecovery+128)
	addiu	$2,$2,%lo(gponRecovery+128)
	sw	$2,20($sp)
	sw	$0,24($sp)
	lhu	$2,56($sp)
	sh	$2,28($sp)
	sw	$0,32($sp)
	sw	$0,36($sp)
	sw	$0,32($sp)
	j	$L20
	nop

$L23:
	lui	$2,%hi(gponRecovery)
	lw	$3,32($sp)
	sll	$3,$3,2
	addiu	$2,$2,%lo(gponRecovery)
	addu	$2,$3,$2
	lw	$3,0($2)
	lhu	$2,28($sp)
	bne	$3,$2,$L21
	nop

	lw	$2,32($sp)
	sll	$3,$2,2
	lui	$2,%hi(gponRecovery)
	addiu	$2,$2,%lo(gponRecovery)
	addu	$2,$3,$2
	sw	$2,36($sp)
	j	$L22
	nop

$L21:
	lw	$2,32($sp)
	addiu	$2,$2,1
	sw	$2,32($sp)
$L20:
	lui	$2,%hi(gponRecovery)
	addiu	$2,$2,%lo(gponRecovery)
	lw	$2,2180($2)
	lw	$3,32($sp)
	slt	$2,$3,$2
	bne	$2,$0,$L23
	nop

$L22:
	lw	$2,36($sp)
	sw	$2,24($sp)
	lw	$2,24($sp)
	bne	$2,$0,$L25
	nop

	li	$2,-1			# 0xffffffffffffffff
	j	$L26
	nop

$L25:
	sw	$0,16($sp)
	j	$L27
	nop

$L31:
	lw	$2,16($sp)
	sll	$2,$2,3
	lw	$3,20($sp)
	addu	$2,$3,$2
	lhu	$2,2($2)
	lhu	$3,56($sp)
	bne	$3,$2,$L28
	nop

	lw	$2,16($sp)
	sll	$2,$2,3
	lw	$3,20($sp)
	addu	$2,$3,$2
	lhu	$2,0($2)
	move	$4,$2
	lui	$2,%hi(gwan_remove_gemport)
	addiu	$2,$2,%lo(gwan_remove_gemport)
	jalr	$2
	nop

	bne	$2,$0,$L28
	nop

	lw	$2,16($sp)
	sll	$2,$2,3
	lw	$3,20($sp)
	addu	$4,$3,$2
	lui	$2,%hi(gponRecovery)
	addiu	$2,$2,%lo(gponRecovery)
	lw	$2,2176($2)
	move	$3,$2
	li	$2,536805376			# 0x1fff0000
	ori	$2,$2,0xffff
	addu	$2,$3,$2
	sll	$2,$2,3
	lw	$3,20($sp)
	addu	$2,$3,$2
	move	$5,$2
	li	$6,2188			# 0x88c
	lui	$2,%hi(memcpy)
	addiu	$2,$2,%lo(memcpy)
	jalr	$2
	nop

	lui	$2,%hi(gponRecovery)
	addiu	$2,$2,%lo(gponRecovery)
	lw	$2,2176($2)
	move	$3,$2
	li	$2,536805376			# 0x1fff0000
	ori	$2,$2,0xffff
	addu	$2,$3,$2
	sll	$2,$2,3
	lw	$3,20($sp)
	addu	$2,$3,$2
	sw	$2,40($sp)
	lw	$2,40($sp)
	li	$3,255			# 0xff
	sh	$3,2($2)
	lw	$2,40($sp)
	li	$3,-1			# 0xffffffffffffffff
	sh	$3,0($2)
	lui	$2,%hi(gponRecovery)
	addiu	$2,$2,%lo(gponRecovery)
	lbu	$2,2184($2)
	beq	$2,$0,$L29
	nop

	lui	$2,%hi($LC0)
	addiu	$4,$2,%lo($LC0)
	lui	$2,%hi(__func__.24860)
	addiu	$5,$2,%lo(__func__.24860)
	li	$6,90			# 0x5a
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi($LC2)
	addiu	$4,$2,%lo($LC2)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L29:
	lui	$2,%hi(gponRecovery)
	addiu	$2,$2,%lo(gponRecovery)
	lw	$2,2176($2)
	addiu	$3,$2,-1
	lui	$2,%hi(gponRecovery)
	addiu	$2,$2,%lo(gponRecovery)
	sw	$3,2176($2)
	lw	$2,16($sp)
	addiu	$2,$2,-1
	sw	$2,16($sp)
$L28:
	lw	$2,16($sp)
	addiu	$2,$2,1
	sw	$2,16($sp)
$L27:
	lui	$2,%hi(gponRecovery)
	addiu	$2,$2,%lo(gponRecovery)
	lw	$3,2176($2)
	lw	$2,16($sp)
	slt	$2,$2,$3
	bne	$2,$0,$L31
	nop

	lui	$2,%hi(gponRecovery)
	addiu	$2,$2,%lo(gponRecovery)
	lbu	$2,2184($2)
	beq	$2,$0,$L32
	nop

	lui	$2,%hi($LC0)
	addiu	$4,$2,%lo($LC0)
	lui	$2,%hi(__func__.24924)
	addiu	$5,$2,%lo(__func__.24924)
	li	$6,162			# 0xa2
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi($LC3)
	addiu	$4,$2,%lo($LC3)
	lw	$5,24($sp)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L32:
	lui	$2,%hi(gponRecovery)
	addiu	$2,$2,%lo(gponRecovery)
	lw	$2,2180($2)
	addiu	$3,$2,-1
	lui	$2,%hi(gponRecovery)
	sll	$3,$3,2
	addiu	$2,$2,%lo(gponRecovery)
	addu	$2,$3,$2
	lw	$3,0($2)
	lw	$2,24($sp)
	sw	$3,0($2)
	lui	$2,%hi(gponRecovery)
	addiu	$2,$2,%lo(gponRecovery)
	lw	$2,2180($2)
	addiu	$3,$2,-1
	lui	$2,%hi(gponRecovery)
	sll	$3,$3,2
	addiu	$2,$2,%lo(gponRecovery)
	addu	$2,$3,$2
	li	$3,255			# 0xff
	sw	$3,0($2)
	lui	$2,%hi(gponRecovery)
	addiu	$2,$2,%lo(gponRecovery)
	lw	$2,2180($2)
	addiu	$3,$2,-1
	lui	$2,%hi(gponRecovery)
	addiu	$2,$2,%lo(gponRecovery)
	sw	$3,2180($2)
	move	$2,$0
$L26:
	lw	$31,52($sp)
	addiu	$sp,$sp,56
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	gpon_recover_delete_allocId
	.size	gpon_recover_delete_allocId, .-gpon_recover_delete_allocId
	.rdata
	.align	2
$LC4:
	.ascii	"gemPort[%d].allocId = %d\012\000"
	.align	2
$LC5:
	.ascii	"gemPort[%d].gemPortId = %d\012\000"
	.align	2
$LC6:
	.ascii	"gemPort[%d].channel = %d\012\000"
	.align	2
$LC7:
	.ascii	"gemPort[%d].encryption = %d\012\012\000"
	.section	.text.gpon_recover_backup_gemport,"ax",@progbits
	.align	2
	.globl	gpon_recover_backup_gemport
	.set	nomips16
	.set	nomicromips
	.ent	gpon_recover_backup_gemport
	.type	gpon_recover_backup_gemport, @function
gpon_recover_backup_gemport:
	.frame	$sp,40,$31		# vars= 16, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-40
	sw	$31,36($sp)
	sw	$4,40($sp)
	lw	$2,40($sp)
	lhu	$2,0($2)
	sh	$2,16($sp)
	sw	$0,20($sp)
	sw	$0,24($sp)
	sw	$0,20($sp)
	j	$L34
	nop

$L37:
	lui	$2,%hi(gponRecovery)
	lw	$3,20($sp)
	addiu	$3,$3,16
	sll	$3,$3,3
	addiu	$2,$2,%lo(gponRecovery)
	addu	$2,$3,$2
	lhu	$2,0($2)
	lhu	$3,16($sp)
	bne	$3,$2,$L35
	nop

	lw	$2,20($sp)
	addiu	$2,$2,16
	sll	$3,$2,3
	lui	$2,%hi(gponRecovery)
	addiu	$2,$2,%lo(gponRecovery)
	addu	$2,$3,$2
	sw	$2,24($sp)
	j	$L36
	nop

$L35:
	lw	$2,20($sp)
	addiu	$2,$2,1
	sw	$2,20($sp)
$L34:
	lui	$2,%hi(gponRecovery)
	addiu	$2,$2,%lo(gponRecovery)
	lw	$2,2176($2)
	lw	$3,20($sp)
	slt	$2,$3,$2
	bne	$2,$0,$L37
	nop

$L36:
	lw	$2,24($sp)
	beq	$2,$0,$L39
	nop

	li	$2,-1			# 0xffffffffffffffff
	j	$L40
	nop

$L39:
	lui	$2,%hi(gponRecovery)
	addiu	$2,$2,%lo(gponRecovery)
	lw	$3,2176($2)
	lui	$2,%hi(gponRecovery)
	addiu	$3,$3,16
	sll	$3,$3,3
	addiu	$2,$2,%lo(gponRecovery)
	addu	$3,$3,$2
	lw	$2,40($sp)
	lwl	$4,0($2)
	lwr	$4,3($2)
	lwl	$5,4($2)
	move	$6,$5
	lwr	$6,7($2)
	move	$2,$6
	sw	$4,0($3)
	sw	$2,4($3)
	lui	$2,%hi(gponRecovery)
	addiu	$2,$2,%lo(gponRecovery)
	lbu	$2,2184($2)
	beq	$2,$0,$L41
	nop

	lui	$2,%hi($LC0)
	addiu	$4,$2,%lo($LC0)
	lui	$2,%hi(__func__.24928)
	addiu	$5,$2,%lo(__func__.24928)
	li	$6,186			# 0xba
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi(gponRecovery)
	addiu	$2,$2,%lo(gponRecovery)
	lw	$5,2176($2)
	lui	$2,%hi(gponRecovery)
	addiu	$2,$2,%lo(gponRecovery)
	lw	$3,2176($2)
	lui	$2,%hi(gponRecovery)
	addiu	$3,$3,16
	sll	$3,$3,3
	addiu	$2,$2,%lo(gponRecovery)
	addu	$2,$3,$2
	lhu	$2,2($2)
	move	$3,$2
	lui	$2,%hi($LC4)
	addiu	$4,$2,%lo($LC4)
	move	$6,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L41:
	lui	$2,%hi(gponRecovery)
	addiu	$2,$2,%lo(gponRecovery)
	lbu	$2,2184($2)
	beq	$2,$0,$L42
	nop

	lui	$2,%hi($LC0)
	addiu	$4,$2,%lo($LC0)
	lui	$2,%hi(__func__.24928)
	addiu	$5,$2,%lo(__func__.24928)
	li	$6,186			# 0xba
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi(gponRecovery)
	addiu	$2,$2,%lo(gponRecovery)
	lw	$5,2176($2)
	lui	$2,%hi(gponRecovery)
	addiu	$2,$2,%lo(gponRecovery)
	lw	$3,2176($2)
	lui	$2,%hi(gponRecovery)
	addiu	$3,$3,16
	sll	$3,$3,3
	addiu	$2,$2,%lo(gponRecovery)
	addu	$2,$3,$2
	lhu	$2,0($2)
	move	$3,$2
	lui	$2,%hi($LC5)
	addiu	$4,$2,%lo($LC5)
	move	$6,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L42:
	lui	$2,%hi(gponRecovery)
	addiu	$2,$2,%lo(gponRecovery)
	lbu	$2,2184($2)
	beq	$2,$0,$L43
	nop

	lui	$2,%hi($LC0)
	addiu	$4,$2,%lo($LC0)
	lui	$2,%hi(__func__.24928)
	addiu	$5,$2,%lo(__func__.24928)
	li	$6,186			# 0xba
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi(gponRecovery)
	addiu	$2,$2,%lo(gponRecovery)
	lw	$5,2176($2)
	lui	$2,%hi(gponRecovery)
	addiu	$2,$2,%lo(gponRecovery)
	lw	$3,2176($2)
	lui	$2,%hi(gponRecovery)
	addiu	$3,$3,16
	sll	$3,$3,3
	addiu	$2,$2,%lo(gponRecovery)
	addu	$2,$3,$2
	lbu	$2,4($2)
	move	$3,$2
	lui	$2,%hi($LC6)
	addiu	$4,$2,%lo($LC6)
	move	$6,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L43:
	lui	$2,%hi(gponRecovery)
	addiu	$2,$2,%lo(gponRecovery)
	lbu	$2,2184($2)
	beq	$2,$0,$L44
	nop

	lui	$2,%hi($LC0)
	addiu	$4,$2,%lo($LC0)
	lui	$2,%hi(__func__.24928)
	addiu	$5,$2,%lo(__func__.24928)
	li	$6,186			# 0xba
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi(gponRecovery)
	addiu	$2,$2,%lo(gponRecovery)
	lw	$5,2176($2)
	lui	$2,%hi(gponRecovery)
	addiu	$2,$2,%lo(gponRecovery)
	lw	$3,2176($2)
	lui	$2,%hi(gponRecovery)
	addiu	$3,$3,16
	sll	$3,$3,3
	addiu	$2,$2,%lo(gponRecovery)
	addu	$2,$3,$2
	lbu	$2,5($2)
	move	$3,$2
	lui	$2,%hi($LC7)
	addiu	$4,$2,%lo($LC7)
	move	$6,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L44:
	lui	$2,%hi(gponRecovery)
	addiu	$2,$2,%lo(gponRecovery)
	lw	$2,2176($2)
	addiu	$3,$2,1
	lui	$2,%hi(gponRecovery)
	addiu	$2,$2,%lo(gponRecovery)
	sw	$3,2176($2)
	move	$2,$0
$L40:
	lw	$31,36($sp)
	addiu	$sp,$sp,40
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	gpon_recover_backup_gemport
	.size	gpon_recover_backup_gemport, .-gpon_recover_backup_gemport
	.section	.text.gpon_recover_get_real_channel,"ax",@progbits
	.align	2
	.globl	gpon_recover_get_real_channel
	.set	nomips16
	.set	nomicromips
	.ent	gpon_recover_get_real_channel
	.type	gpon_recover_get_real_channel, @function
gpon_recover_get_real_channel:
	.frame	$sp,8,$31		# vars= 8, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-8
	move	$2,$4
	sh	$2,8($sp)
	lhu	$3,8($sp)
	li	$2,255			# 0xff
	bne	$3,$2,$L46
	nop

	li	$2,32			# 0x20
	j	$L47
	nop

$L46:
	sw	$0,0($sp)
	j	$L48
	nop

$L50:
	lui	$2,%hi(gpWanPriv)
	lw	$3,%lo(gpWanPriv)($2)
	lw	$2,0($sp)
	addiu	$2,$2,12
	sll	$2,$2,1
	addu	$2,$3,$2
	lhu	$2,4($2)
	lhu	$3,8($sp)
	bne	$3,$2,$L49
	nop

	lw	$2,0($sp)
	j	$L47
	nop

$L49:
	lw	$2,0($sp)
	addiu	$2,$2,1
	sw	$2,0($sp)
$L48:
	lw	$2,0($sp)
	slt	$2,$2,32
	bne	$2,$0,$L50
	nop

	li	$2,-1			# 0xffffffffffffffff
$L47:
	addiu	$sp,$sp,8
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	gpon_recover_get_real_channel
	.size	gpon_recover_get_real_channel, .-gpon_recover_get_real_channel
	.rdata
	.align	2
$LC8:
	.ascii	"create_gemport err\012\000"
	.align	2
$LC9:
	.ascii	"config_gemport err\012\000"
	.section	.text.gpon_recover_create_gemport,"ax",@progbits
	.align	2
	.globl	gpon_recover_create_gemport
	.set	nomips16
	.set	nomicromips
	.ent	gpon_recover_create_gemport
	.type	gpon_recover_create_gemport, @function
gpon_recover_create_gemport:
	.frame	$sp,32,$31		# vars= 8, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-32
	sw	$31,28($sp)
	sw	$0,16($sp)
	sw	$0,20($sp)
	sw	$0,16($sp)
	j	$L52
	nop

$L61:
	lw	$2,16($sp)
	addiu	$2,$2,16
	sll	$3,$2,3
	lui	$2,%hi(gponRecovery)
	addiu	$2,$2,%lo(gponRecovery)
	addu	$2,$3,$2
	sw	$2,20($sp)
	lw	$2,20($sp)
	lhu	$2,2($2)
	move	$4,$2
	lui	$2,%hi(gpon_recover_get_real_channel)
	addiu	$2,$2,%lo(gpon_recover_get_real_channel)
	jalr	$2
	nop

	andi	$3,$2,0x00ff
	lw	$2,20($sp)
	sb	$3,4($2)
	lw	$2,20($sp)
	lhu	$2,0($2)
	move	$4,$2
	lw	$2,20($sp)
	lbu	$2,4($2)
	move	$3,$2
	lw	$2,20($sp)
	lbu	$2,5($2)
	move	$5,$3
	move	$6,$2
	lui	$2,%hi(gwan_create_new_gemport)
	addiu	$2,$2,%lo(gwan_create_new_gemport)
	jalr	$2
	nop

	beq	$2,$0,$L53
	nop

	lui	$2,%hi(gponRecovery)
	addiu	$2,$2,%lo(gponRecovery)
	lbu	$2,2184($2)
	beq	$2,$0,$L54
	nop

	lui	$2,%hi($LC0)
	addiu	$4,$2,%lo($LC0)
	lui	$2,%hi(__func__.24942)
	addiu	$5,$2,%lo(__func__.24942)
	li	$6,246			# 0xf6
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi($LC8)
	addiu	$4,$2,%lo($LC8)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	j	$L55
	nop

$L54:
	j	$L55
	nop

$L53:
	lw	$2,20($sp)
	lhu	$2,0($2)
	move	$4,$2
	li	$5,1			# 0x1
	li	$6,1			# 0x1
	lui	$2,%hi(gwan_config_gemport)
	addiu	$2,$2,%lo(gwan_config_gemport)
	jalr	$2
	nop

	beq	$2,$0,$L56
	nop

	lui	$2,%hi(gponRecovery)
	addiu	$2,$2,%lo(gponRecovery)
	lbu	$2,2184($2)
	beq	$2,$0,$L57
	nop

	lui	$2,%hi($LC0)
	addiu	$4,$2,%lo($LC0)
	lui	$2,%hi(__func__.24942)
	addiu	$5,$2,%lo(__func__.24942)
	li	$6,251			# 0xfb
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi($LC9)
	addiu	$4,$2,%lo($LC9)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	j	$L55
	nop

$L57:
	j	$L55
	nop

$L56:
	lui	$2,%hi(gponRecovery)
	addiu	$2,$2,%lo(gponRecovery)
	lbu	$2,2184($2)
	beq	$2,$0,$L58
	nop

	lui	$2,%hi($LC0)
	addiu	$4,$2,%lo($LC0)
	lui	$2,%hi(__func__.24942)
	addiu	$5,$2,%lo(__func__.24942)
	li	$6,255			# 0xff
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi(gponRecovery)
	lw	$3,16($sp)
	addiu	$3,$3,16
	sll	$3,$3,3
	addiu	$2,$2,%lo(gponRecovery)
	addu	$2,$3,$2
	lhu	$2,2($2)
	move	$3,$2
	lui	$2,%hi($LC4)
	addiu	$4,$2,%lo($LC4)
	lw	$5,16($sp)
	move	$6,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L58:
	lui	$2,%hi(gponRecovery)
	addiu	$2,$2,%lo(gponRecovery)
	lbu	$2,2184($2)
	beq	$2,$0,$L59
	nop

	lui	$2,%hi($LC0)
	addiu	$4,$2,%lo($LC0)
	lui	$2,%hi(__func__.24942)
	addiu	$5,$2,%lo(__func__.24942)
	li	$6,255			# 0xff
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi(gponRecovery)
	lw	$3,16($sp)
	addiu	$3,$3,16
	sll	$3,$3,3
	addiu	$2,$2,%lo(gponRecovery)
	addu	$2,$3,$2
	lhu	$2,0($2)
	move	$3,$2
	lui	$2,%hi($LC5)
	addiu	$4,$2,%lo($LC5)
	lw	$5,16($sp)
	move	$6,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L59:
	lui	$2,%hi(gponRecovery)
	addiu	$2,$2,%lo(gponRecovery)
	lbu	$2,2184($2)
	beq	$2,$0,$L60
	nop

	lui	$2,%hi($LC0)
	addiu	$4,$2,%lo($LC0)
	lui	$2,%hi(__func__.24942)
	addiu	$5,$2,%lo(__func__.24942)
	li	$6,255			# 0xff
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi(gponRecovery)
	lw	$3,16($sp)
	addiu	$3,$3,16
	sll	$3,$3,3
	addiu	$2,$2,%lo(gponRecovery)
	addu	$2,$3,$2
	lbu	$2,4($2)
	move	$3,$2
	lui	$2,%hi($LC6)
	addiu	$4,$2,%lo($LC6)
	lw	$5,16($sp)
	move	$6,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L60:
	lui	$2,%hi(gponRecovery)
	addiu	$2,$2,%lo(gponRecovery)
	lbu	$2,2184($2)
	beq	$2,$0,$L55
	nop

	lui	$2,%hi($LC0)
	addiu	$4,$2,%lo($LC0)
	lui	$2,%hi(__func__.24942)
	addiu	$5,$2,%lo(__func__.24942)
	li	$6,255			# 0xff
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi(gponRecovery)
	lw	$3,16($sp)
	addiu	$3,$3,16
	sll	$3,$3,3
	addiu	$2,$2,%lo(gponRecovery)
	addu	$2,$3,$2
	lbu	$2,5($2)
	move	$3,$2
	lui	$2,%hi($LC7)
	addiu	$4,$2,%lo($LC7)
	lw	$5,16($sp)
	move	$6,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L55:
	lw	$2,16($sp)
	addiu	$2,$2,1
	sw	$2,16($sp)
$L52:
	lui	$2,%hi(gponRecovery)
	addiu	$2,$2,%lo(gponRecovery)
	lw	$3,2176($2)
	lw	$2,16($sp)
	slt	$2,$2,$3
	bne	$2,$0,$L61
	nop

	move	$2,$0
	lw	$31,28($sp)
	addiu	$sp,$sp,32
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	gpon_recover_create_gemport
	.size	gpon_recover_create_gemport, .-gpon_recover_create_gemport
	.rdata
	.align	2
$LC10:
	.ascii	"gemPort = %d\012\000"
	.section	.text.gpon_recover_delete_gemport,"ax",@progbits
	.align	2
	.globl	gpon_recover_delete_gemport
	.set	nomips16
	.set	nomicromips
	.ent	gpon_recover_delete_gemport
	.type	gpon_recover_delete_gemport, @function
gpon_recover_delete_gemport:
	.frame	$sp,56,$31		# vars= 32, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-56
	sw	$31,52($sp)
	move	$2,$4
	sh	$2,56($sp)
	sw	$0,16($sp)
	lui	$2,%hi(gponRecovery+128)
	addiu	$2,$2,%lo(gponRecovery+128)
	sw	$2,20($sp)
	sw	$0,24($sp)
	lhu	$2,56($sp)
	sh	$2,28($sp)
	sw	$0,32($sp)
	sw	$0,36($sp)
	sw	$0,32($sp)
	j	$L64
	nop

$L67:
	lui	$2,%hi(gponRecovery)
	lw	$3,32($sp)
	addiu	$3,$3,16
	sll	$3,$3,3
	addiu	$2,$2,%lo(gponRecovery)
	addu	$2,$3,$2
	lhu	$2,0($2)
	lhu	$3,28($sp)
	bne	$3,$2,$L65
	nop

	lw	$2,32($sp)
	addiu	$2,$2,16
	sll	$3,$2,3
	lui	$2,%hi(gponRecovery)
	addiu	$2,$2,%lo(gponRecovery)
	addu	$2,$3,$2
	sw	$2,36($sp)
	j	$L66
	nop

$L65:
	lw	$2,32($sp)
	addiu	$2,$2,1
	sw	$2,32($sp)
$L64:
	lui	$2,%hi(gponRecovery)
	addiu	$2,$2,%lo(gponRecovery)
	lw	$2,2176($2)
	lw	$3,32($sp)
	slt	$2,$3,$2
	bne	$2,$0,$L67
	nop

$L66:
	lw	$2,36($sp)
	sw	$2,24($sp)
	lw	$2,24($sp)
	bne	$2,$0,$L69
	nop

	li	$2,-1			# 0xffffffffffffffff
	j	$L70
	nop

$L69:
	lw	$3,24($sp)
	lui	$2,%hi(gponRecovery+128)
	addiu	$2,$2,%lo(gponRecovery+128)
	subu	$2,$3,$2
	sra	$2,$2,3
	move	$3,$2
	lui	$2,%hi(gponRecovery)
	addiu	$2,$2,%lo(gponRecovery)
	lw	$2,2176($2)
	addiu	$2,$2,-1
	beq	$3,$2,$L71
	nop

	lui	$2,%hi(gponRecovery)
	addiu	$2,$2,%lo(gponRecovery)
	lbu	$2,2184($2)
	beq	$2,$0,$L72
	nop

	lui	$2,%hi($LC0)
	addiu	$4,$2,%lo($LC0)
	lui	$2,%hi(__func__.24953)
	addiu	$5,$2,%lo(__func__.24953)
	li	$6,284			# 0x11c
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lw	$2,24($sp)
	lhu	$2,0($2)
	move	$3,$2
	lui	$2,%hi($LC10)
	addiu	$4,$2,%lo($LC10)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L72:
	lui	$2,%hi(gponRecovery)
	addiu	$2,$2,%lo(gponRecovery)
	lw	$2,2176($2)
	move	$3,$2
	li	$2,536805376			# 0x1fff0000
	ori	$2,$2,0xffff
	addu	$2,$3,$2
	sll	$2,$2,3
	lw	$3,20($sp)
	addu	$2,$3,$2
	lw	$4,24($sp)
	move	$5,$2
	li	$6,8			# 0x8
	lui	$2,%hi(memcpy)
	addiu	$2,$2,%lo(memcpy)
	jalr	$2
	nop

$L71:
	lui	$2,%hi(gponRecovery)
	addiu	$2,$2,%lo(gponRecovery)
	lw	$2,2176($2)
	move	$3,$2
	li	$2,536805376			# 0x1fff0000
	ori	$2,$2,0xffff
	addu	$2,$3,$2
	sll	$2,$2,3
	lw	$3,20($sp)
	addu	$2,$3,$2
	sw	$2,40($sp)
	lw	$2,40($sp)
	li	$3,255			# 0xff
	sh	$3,2($2)
	lw	$2,40($sp)
	li	$3,-1			# 0xffffffffffffffff
	sh	$3,0($2)
	lui	$2,%hi(gponRecovery)
	addiu	$2,$2,%lo(gponRecovery)
	lbu	$2,2184($2)
	beq	$2,$0,$L73
	nop

	lui	$2,%hi($LC0)
	addiu	$4,$2,%lo($LC0)
	lui	$2,%hi(__func__.24860)
	addiu	$5,$2,%lo(__func__.24860)
	li	$6,90			# 0x5a
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi($LC2)
	addiu	$4,$2,%lo($LC2)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L73:
	lui	$2,%hi(gponRecovery)
	addiu	$2,$2,%lo(gponRecovery)
	lw	$2,2176($2)
	addiu	$3,$2,-1
	lui	$2,%hi(gponRecovery)
	addiu	$2,$2,%lo(gponRecovery)
	sw	$3,2176($2)
	move	$2,$0
$L70:
	lw	$31,52($sp)
	addiu	$sp,$sp,56
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	gpon_recover_delete_gemport
	.size	gpon_recover_delete_gemport, .-gpon_recover_delete_gemport
	.rdata
	.align	2
	.type	__func__.24904, @object
	.size	__func__.24904, 28
__func__.24904:
	.ascii	"gpon_recover_backup_allocId\000"
	.align	2
	.type	__func__.24911, @object
	.size	__func__.24911, 28
__func__.24911:
	.ascii	"gpon_recover_create_allocId\000"
	.align	2
	.type	__func__.24924, @object
	.size	__func__.24924, 28
__func__.24924:
	.ascii	"gpon_recover_delete_allocId\000"
	.align	2
	.type	__func__.24860, @object
	.size	__func__.24860, 19
__func__.24860:
	.ascii	"clear_gemport_info\000"
	.align	2
	.type	__func__.24928, @object
	.size	__func__.24928, 28
__func__.24928:
	.ascii	"gpon_recover_backup_gemport\000"
	.align	2
	.type	__func__.24942, @object
	.size	__func__.24942, 28
__func__.24942:
	.ascii	"gpon_recover_create_gemport\000"
	.align	2
	.type	__func__.24953, @object
	.size	__func__.24953, 28
__func__.24953:
	.ascii	"gpon_recover_delete_gemport\000"
	.ident	"GCC: (Buildroot 2015.08.1) 4.9.3"
