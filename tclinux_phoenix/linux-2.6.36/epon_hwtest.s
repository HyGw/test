	.file	1 "epon_hwtest.c"
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
	.globl	eponRegTestPattern
	.data
	.align	2
	.type	eponRegTestPattern, @object
	.size	eponRegTestPattern, 32
eponRegTestPattern:
	.word	-1
	.word	0
	.word	1431655765
	.word	858993459
	.word	1768515945
	.word	1515870810
	.word	-858993460
	.word	-1431655766
	.rdata
	.align	2
$LC0:
	.ascii	"pattern=0x%x\000"
	.align	2
$LC1:
	.ascii	"\015\012 eponRegPatternTest ! register addr= 0x%x , rwma"
	.ascii	"sk= 0x%x, pattern = 0x%x ,  value=0x%x\000"
	.align	2
$LC2:
	.ascii	"\015\012 eponRegPatternTest fail!! register addr= 0x%x ,"
	.ascii	" rwmask= 0x%x, pattern = 0x%x ,  value=0x%x\000"
	.section	.text.eponRegPatternTest,"ax",@progbits
	.align	2
	.globl	eponRegPatternTest
	.set	nomips16
	.set	nomicromips
	.ent	eponRegPatternTest
	.type	eponRegPatternTest, @function
eponRegPatternTest:
	.frame	$sp,48,$31		# vars= 16, regs= 1/0, args= 24, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-48
	sw	$31,44($sp)
	sw	$4,48($sp)
	sw	$5,52($sp)
	sw	$6,56($sp)
	sw	$0,24($sp)
	sw	$0,28($sp)
	sw	$0,32($sp)
	li	$4,4			# 0x4
	lui	$2,%hi($LC0)
	addiu	$5,$2,%lo($LC0)
	lw	$6,48($sp)
	lui	$2,%hi(eponDbgPrint)
	addiu	$2,$2,%lo(eponDbgPrint)
	jalr	$2
	nop

	sw	$0,28($sp)
	j	$L2
	nop

$L5:
	lw	$2,28($sp)
	sll	$2,$2,2
	sll	$3,$2,2
	subu	$2,$3,$2
	lw	$3,52($sp)
	addu	$2,$3,$2
	lw	$6,0($2)
	lw	$2,28($sp)
	sll	$2,$2,2
	sll	$3,$2,2
	subu	$2,$3,$2
	lw	$3,52($sp)
	addu	$2,$3,$2
	lw	$3,8($2)
	lw	$2,48($sp)
	sw	$2,16($sp)
	lw	$2,32($sp)
	sw	$2,20($sp)
	li	$4,4			# 0x4
	lui	$2,%hi($LC1)
	addiu	$5,$2,%lo($LC1)
	move	$7,$3
	lui	$2,%hi(eponDbgPrint)
	addiu	$2,$2,%lo(eponDbgPrint)
	jalr	$2
	nop

	lw	$2,28($sp)
	sll	$2,$2,2
	sll	$3,$2,2
	subu	$2,$3,$2
	lw	$3,52($sp)
	addu	$2,$3,$2
	lw	$3,8($2)
	lw	$2,48($sp)
	and	$4,$3,$2
	lw	$2,28($sp)
	sll	$2,$2,2
	sll	$3,$2,2
	subu	$2,$3,$2
	lw	$3,52($sp)
	addu	$2,$3,$2
	lw	$2,0($2)
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	lw	$2,28($sp)
	sll	$2,$2,2
	sll	$3,$2,2
	subu	$2,$3,$2
	lw	$3,52($sp)
	addu	$2,$3,$2
	lw	$2,0($2)
	move	$3,$2
	lui	$2,%hi(g_EPON_MAC_BASE)
	lw	$2,%lo(g_EPON_MAC_BASE)($2)
	addiu	$2,$2,24788
	bne	$3,$2,$L3
	nop

	lui	$2,%hi(g_EPON_MAC_BASE)
	lw	$2,%lo(g_EPON_MAC_BASE)($2)
	addiu	$2,$2,24576
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	lui	$2,%hi(g_EPON_MAC_BASE)
	lw	$2,%lo(g_EPON_MAC_BASE)($2)
	addiu	$2,$2,24576
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	lui	$2,%hi(g_EPON_MAC_BASE)
	lw	$2,%lo(g_EPON_MAC_BASE)($2)
	addiu	$2,$2,24576
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	lui	$2,%hi(g_EPON_MAC_BASE)
	lw	$2,%lo(g_EPON_MAC_BASE)($2)
	addiu	$2,$2,24576
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	lui	$2,%hi(g_EPON_MAC_BASE)
	lw	$2,%lo(g_EPON_MAC_BASE)($2)
	addiu	$2,$2,24576
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

$L3:
	lw	$2,28($sp)
	sll	$2,$2,2
	sll	$3,$2,2
	subu	$2,$3,$2
	lw	$3,52($sp)
	addu	$2,$3,$2
	lw	$2,0($2)
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	sw	$2,32($sp)
	lw	$3,48($sp)
	lw	$2,32($sp)
	xor	$3,$3,$2
	lw	$2,28($sp)
	sll	$2,$2,2
	sll	$4,$2,2
	subu	$2,$4,$2
	lw	$4,52($sp)
	addu	$2,$4,$2
	lw	$2,8($2)
	and	$2,$3,$2
	beq	$2,$0,$L4
	nop

	lw	$2,28($sp)
	sll	$2,$2,2
	sll	$3,$2,2
	subu	$2,$3,$2
	lw	$3,52($sp)
	addu	$2,$3,$2
	lw	$6,0($2)
	lw	$2,28($sp)
	sll	$2,$2,2
	sll	$3,$2,2
	subu	$2,$3,$2
	lw	$3,52($sp)
	addu	$2,$3,$2
	lw	$3,8($2)
	lw	$2,48($sp)
	sw	$2,16($sp)
	lw	$2,32($sp)
	sw	$2,20($sp)
	li	$4,1			# 0x1
	lui	$2,%hi($LC2)
	addiu	$5,$2,%lo($LC2)
	move	$7,$3
	lui	$2,%hi(eponDbgPrint)
	addiu	$2,$2,%lo(eponDbgPrint)
	jalr	$2
	nop

	li	$2,-1			# 0xffffffffffffffff
	sw	$2,24($sp)
$L4:
	lw	$2,28($sp)
	addiu	$2,$2,1
	sw	$2,28($sp)
$L2:
	lw	$3,28($sp)
	lw	$2,56($sp)
	sltu	$2,$3,$2
	bne	$2,$0,$L5
	nop

	lw	$2,24($sp)
	lw	$31,44($sp)
	addiu	$sp,$sp,48
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	eponRegPatternTest
	.size	eponRegPatternTest, .-eponRegPatternTest
	.section	.text.eponMacRegTest,"ax",@progbits
	.align	2
	.globl	eponMacRegTest
	.set	nomips16
	.set	nomicromips
	.ent	eponMacRegTest
	.type	eponMacRegTest, @function
eponMacRegTest:
	.frame	$sp,48,$31		# vars= 16, regs= 3/0, args= 16, gp= 0
	.mask	0x80030000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-48
	sw	$31,44($sp)
	sw	$17,40($sp)
	sw	$16,36($sp)
	sw	$4,48($sp)
	sw	$0,16($sp)
	sw	$0,20($sp)
	sw	$0,28($sp)
	sw	$0,24($sp)
	sw	$0,20($sp)
	j	$L8
	nop

$L14:
	sw	$0,16($sp)
	j	$L9
	nop

$L11:
	lui	$2,%hi(eponRegTestPattern)
	lw	$3,16($sp)
	sll	$3,$3,2
	addiu	$2,$2,%lo(eponRegTestPattern)
	addu	$2,$3,$2
	lw	$17,0($2)
	lui	$2,%hi(eponMacRegTable)
	lw	$16,%lo(eponMacRegTable)($2)
	lui	$2,%hi(eponMacGetRegTblSize)
	addiu	$2,$2,%lo(eponMacGetRegTblSize)
	jalr	$2
	nop

	move	$4,$17
	move	$5,$16
	move	$6,$2
	lui	$2,%hi(eponRegPatternTest)
	addiu	$2,$2,%lo(eponRegPatternTest)
	jalr	$2
	nop

	sw	$2,28($sp)
	lw	$2,28($sp)
	bgez	$2,$L10
	nop

	li	$2,-1			# 0xffffffffffffffff
	sw	$2,24($sp)
$L10:
	lw	$2,16($sp)
	addiu	$2,$2,1
	sw	$2,16($sp)
$L9:
	lw	$2,16($sp)
	sltu	$2,$2,8
	bne	$2,$0,$L11
	nop

	lw	$2,24($sp)
	bgez	$2,$L12
	nop

	lw	$2,24($sp)
	j	$L13
	nop

$L12:
	lw	$2,20($sp)
	addiu	$2,$2,1
	sw	$2,20($sp)
$L8:
	lw	$3,20($sp)
	lw	$2,48($sp)
	sltu	$2,$3,$2
	bne	$2,$0,$L14
	nop

	lw	$2,24($sp)
$L13:
	lw	$31,44($sp)
	lw	$17,40($sp)
	lw	$16,36($sp)
	addiu	$sp,$sp,48
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	eponMacRegTest
	.size	eponMacRegTest, .-eponMacRegTest
	.rdata
	.align	2
$LC3:
	.ascii	"error: add:%x default: 0x%08lx real:0x%08lx\012\000"
	.section	.text.eponRegDefCheck,"ax",@progbits
	.align	2
	.globl	eponRegDefCheck
	.set	nomips16
	.set	nomicromips
	.ent	eponRegDefCheck
	.type	eponRegDefCheck, @function
eponRegDefCheck:
	.frame	$sp,40,$31		# vars= 16, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-40
	sw	$31,36($sp)
	sw	$4,40($sp)
	sw	$5,44($sp)
	sw	$0,16($sp)
	sw	$0,20($sp)
	sw	$0,24($sp)
	sw	$0,20($sp)
	j	$L16
	nop

$L20:
	lw	$2,20($sp)
	sll	$2,$2,2
	sll	$3,$2,2
	subu	$2,$3,$2
	lw	$3,40($sp)
	addu	$2,$3,$2
	lw	$2,0($2)
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	sw	$2,24($sp)
	lw	$2,20($sp)
	sll	$2,$2,2
	sll	$3,$2,2
	subu	$2,$3,$2
	lw	$3,40($sp)
	addu	$2,$3,$2
	lw	$2,0($2)
	move	$3,$2
	lui	$2,%hi(g_EPON_MAC_BASE)
	lw	$2,%lo(g_EPON_MAC_BASE)($2)
	addiu	$2,$2,24704
	beq	$3,$2,$L17
	nop

	lw	$2,20($sp)
	sll	$2,$2,2
	sll	$3,$2,2
	subu	$2,$3,$2
	lw	$3,40($sp)
	addu	$2,$3,$2
	lw	$2,0($2)
	move	$3,$2
	lui	$2,%hi(g_EPON_MAC_BASE)
	lw	$2,%lo(g_EPON_MAC_BASE)($2)
	addiu	$2,$2,24672
	beq	$3,$2,$L17
	nop

	lw	$2,20($sp)
	sll	$2,$2,2
	sll	$3,$2,2
	subu	$2,$3,$2
	lw	$3,40($sp)
	addu	$2,$3,$2
	lw	$2,0($2)
	move	$3,$2
	lui	$2,%hi(g_EPON_MAC_BASE)
	lw	$2,%lo(g_EPON_MAC_BASE)($2)
	addiu	$2,$2,24676
	beq	$3,$2,$L17
	nop

	lw	$2,20($sp)
	sll	$2,$2,2
	sll	$3,$2,2
	subu	$2,$3,$2
	lw	$3,40($sp)
	addu	$2,$3,$2
	lw	$2,0($2)
	move	$3,$2
	lui	$2,%hi(g_EPON_MAC_BASE)
	lw	$2,%lo(g_EPON_MAC_BASE)($2)
	addiu	$2,$2,24836
	beq	$3,$2,$L17
	nop

	lw	$2,20($sp)
	sll	$2,$2,2
	sll	$3,$2,2
	subu	$2,$3,$2
	lw	$3,40($sp)
	addu	$2,$3,$2
	lw	$2,0($2)
	move	$3,$2
	lui	$2,%hi(g_EPON_MAC_BASE)
	lw	$2,%lo(g_EPON_MAC_BASE)($2)
	addiu	$2,$2,24840
	bne	$3,$2,$L18
	nop

$L17:
	j	$L19
	nop

$L18:
	lw	$2,20($sp)
	sll	$2,$2,2
	sll	$3,$2,2
	subu	$2,$3,$2
	lw	$3,40($sp)
	addu	$2,$3,$2
	lw	$3,4($2)
	lw	$2,24($sp)
	beq	$3,$2,$L19
	nop

	lw	$2,20($sp)
	sll	$2,$2,2
	sll	$3,$2,2
	subu	$2,$3,$2
	lw	$3,40($sp)
	addu	$2,$3,$2
	lw	$5,0($2)
	lw	$2,20($sp)
	sll	$2,$2,2
	sll	$3,$2,2
	subu	$2,$3,$2
	lw	$3,40($sp)
	addu	$2,$3,$2
	lw	$3,4($2)
	lui	$2,%hi($LC3)
	addiu	$4,$2,%lo($LC3)
	move	$6,$3
	lw	$7,24($sp)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	li	$2,-1			# 0xffffffffffffffff
	sw	$2,16($sp)
$L19:
	lw	$2,20($sp)
	addiu	$2,$2,1
	sw	$2,20($sp)
$L16:
	lw	$3,20($sp)
	lw	$2,44($sp)
	sltu	$2,$3,$2
	bne	$2,$0,$L20
	nop

	lw	$2,16($sp)
	lw	$31,36($sp)
	addiu	$sp,$sp,40
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	eponRegDefCheck
	.size	eponRegDefCheck, .-eponRegDefCheck
	.ident	"GCC: (Buildroot 2015.08.1) 4.9.3"
