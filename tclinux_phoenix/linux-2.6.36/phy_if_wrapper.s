	.file	1 "phy_if_wrapper.c"
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
	.globl	gpPhyData
	.section	.bss,"aw",@nobits
	.align	2
	.type	gpPhyData, @object
	.size	gpPhyData, 4
gpPhyData:
	.space	4
	.rdata
	.align	2
$LC0:
	.ascii	"%s:%d phy_mode:%d\012\000"
	.align	2
$LC1:
	.ascii	"ECNT_HOOK_ERROR occur with cmd_id:0x%x\012\000"
	.align	2
$LC2:
	.ascii	"No such API with type:%d, cmd_id:0x%x\012\000"
	.section	.text.XPON_PHY_SET_MODE,"ax",@progbits
	.align	2
	.globl	XPON_PHY_SET_MODE
	.set	nomips16
	.set	nomicromips
	.ent	XPON_PHY_SET_MODE
	.type	XPON_PHY_SET_MODE, @function
XPON_PHY_SET_MODE:
	.frame	$sp,96,$31		# vars= 72, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-96
	sw	$31,92($sp)
	sw	$4,96($sp)
	lw	$2,96($sp)
	sw	$2,44($sp)
	li	$2,1			# 0x1
	sw	$2,48($sp)
	lui	$2,%hi(xpon_mac_print_open)
	lw	$2,%lo(xpon_mac_print_open)($2)
	beq	$2,$0,$L2
	nop

	lui	$2,%hi($LC0)
	addiu	$4,$2,%lo($LC0)
	lui	$2,%hi(__func__.32874)
	addiu	$5,$2,%lo(__func__.32874)
	li	$6,52			# 0x34
	lw	$7,96($sp)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L2:
	lw	$3,96($sp)
	li	$2,1			# 0x1
	bne	$3,$2,$L3
	nop

	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lw	$3,464($2)
	li	$2,536870912			# 0x20000000
	and	$2,$3,$2
	beq	$2,$0,$L3
	nop

	sw	$0,48($sp)
$L3:
	li	$2,7			# 0x7
	sw	$2,16($sp)
	addiu	$2,$sp,44
	sw	$2,20($sp)
	sw	$0,68($sp)
	sw	$0,72($sp)
	sw	$0,76($sp)
	sw	$0,80($sp)
	li	$2,1			# 0x1
	sw	$2,68($sp)
	lw	$2,16($sp)
	sw	$2,72($sp)
	lw	$2,20($sp)
	sw	$2,80($sp)
	addiu	$2,$sp,68
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
	bne	$3,$2,$L4
	nop

	lw	$2,24($sp)
	lw	$3,4($2)
	lui	$2,%hi($LC1)
	addiu	$4,$2,%lo($LC1)
	move	$5,$3
	lui	$2,%hi(panic)
	addiu	$2,$2,%lo(panic)
	jalr	$2
	nop

$L4:
	lw	$2,24($sp)
	lw	$3,8($2)
	li	$2,-1			# 0xffffffffffffffff
	bne	$3,$2,$L5
	nop

	lw	$2,24($sp)
	lw	$5,0($2)
	lw	$2,24($sp)
	lw	$3,4($2)
	lui	$2,%hi($LC2)
	addiu	$4,$2,%lo($LC2)
	move	$6,$3
	lui	$2,%hi(panic)
	addiu	$2,$2,%lo(panic)
	jalr	$2
	nop

$L5:
	li	$2,22			# 0x16
	sw	$2,28($sp)
	lw	$2,28($sp)
	sw	$2,32($sp)
	sw	$0,36($sp)
	sw	$0,52($sp)
	sw	$0,56($sp)
	sw	$0,60($sp)
	sw	$0,64($sp)
	li	$2,1			# 0x1
	sw	$2,52($sp)
	lw	$2,32($sp)
	sw	$2,56($sp)
	lw	$2,36($sp)
	sw	$2,64($sp)
	addiu	$2,$sp,52
	sw	$2,40($sp)
	li	$4,25			# 0x19
	move	$5,$0
	lw	$6,40($sp)
	lui	$2,%hi(__ECNT_HOOK)
	addiu	$2,$2,%lo(__ECNT_HOOK)
	jalr	$2
	nop

	move	$3,$2
	li	$2,-1			# 0xffffffffffffffff
	bne	$3,$2,$L8
	nop

	lw	$2,40($sp)
	lw	$3,4($2)
	lui	$2,%hi($LC1)
	addiu	$4,$2,%lo($LC1)
	move	$5,$3
	lui	$2,%hi(panic)
	addiu	$2,$2,%lo(panic)
	jalr	$2
	nop

$L8:
	lw	$2,40($sp)
	lw	$3,8($2)
	li	$2,-1			# 0xffffffffffffffff
	bne	$3,$2,$L9
	nop

	lw	$2,40($sp)
	lw	$5,0($2)
	lw	$2,40($sp)
	lw	$3,4($2)
	lui	$2,%hi($LC2)
	addiu	$4,$2,%lo($LC2)
	move	$6,$3
	lui	$2,%hi(panic)
	addiu	$2,$2,%lo(panic)
	jalr	$2
	nop

$L9:
	lw	$31,92($sp)
	addiu	$sp,$sp,96
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	XPON_PHY_SET_MODE
	.size	XPON_PHY_SET_MODE, .-XPON_PHY_SET_MODE
	.rdata
	.align	2
$LC3:
	.ascii	"GPON\000"
	.align	2
$LC4:
	.ascii	"EPON\000"
	.align	2
$LC5:
	.ascii	"XPON PHY sync on %s mode\012\000"
	.section	.text.check_sync_result,"ax",@progbits
	.align	2
	.globl	check_sync_result
	.set	nomips16
	.set	nomicromips
	.ent	check_sync_result
	.type	check_sync_result, @function
check_sync_result:
	.frame	$sp,128,$31		# vars= 104, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-128
	sw	$31,124($sp)
	sw	$4,128($sp)
	li	$2,2			# 0x2
	sw	$2,16($sp)
	lui	$2,%hi(gpPhyData)
	lw	$2,%lo(gpPhyData)($2)
	lw	$3,60($2)
	li	$2,402653184			# 0x18000000
	and	$3,$3,$2
	li	$2,268435456			# 0x10000000
	beq	$3,$2,$L14
	nop

	lui	$2,%hi(gpPhyData)
	lw	$2,%lo(gpPhyData)($2)
	lw	$3,60($2)
	li	$2,402653184			# 0x18000000
	and	$3,$3,$2
	li	$2,402653184			# 0x18000000
	bne	$3,$2,$L15
	nop

$L14:
	li	$2,1			# 0x1
	j	$L17
	nop

$L15:
	move	$2,$0
$L17:
	beq	$2,$0,$L18
	nop

	j	$L13
	nop

$L18:
	li	$2,32769			# 0x8001
	sw	$2,20($sp)
	lw	$2,20($sp)
	sw	$2,24($sp)
	sw	$0,28($sp)
	sw	$0,100($sp)
	sw	$0,104($sp)
	sw	$0,108($sp)
	sw	$0,112($sp)
	sw	$0,100($sp)
	lw	$2,24($sp)
	sw	$2,104($sp)
	lw	$2,28($sp)
	sw	$2,112($sp)
	addiu	$2,$sp,100
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
	bne	$3,$2,$L20
	nop

	lw	$2,32($sp)
	lw	$3,4($2)
	lui	$2,%hi($LC1)
	addiu	$4,$2,%lo($LC1)
	move	$5,$3
	lui	$2,%hi(panic)
	addiu	$2,$2,%lo(panic)
	jalr	$2
	nop

$L20:
	lw	$2,32($sp)
	lw	$3,8($2)
	li	$2,-1			# 0xffffffffffffffff
	bne	$3,$2,$L21
	nop

	lw	$2,32($sp)
	lw	$5,0($2)
	lw	$2,32($sp)
	lw	$3,4($2)
	lui	$2,%hi($LC2)
	addiu	$4,$2,%lo($LC2)
	move	$6,$3
	lui	$2,%hi(panic)
	addiu	$2,$2,%lo(panic)
	jalr	$2
	nop

$L21:
	lw	$2,32($sp)
	lw	$3,8($2)
	li	$2,1			# 0x1
	bne	$3,$2,$L25
	nop

	lui	$2,%hi(gpPhyData)
	lw	$2,%lo(gpPhyData)($2)
	addiu	$4,$2,32
	lui	$2,%hi(gpPhyData)
	lw	$2,%lo(gpPhyData)($2)
	lw	$2,52($2)
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
	addu	$2,$3,$2
	move	$5,$2
	lui	$2,%hi(mod_timer)
	addiu	$2,$2,%lo(mod_timer)
	jalr	$2
	nop

	j	$L13
	nop

$L25:
	li	$2,32794			# 0x801a
	sw	$2,36($sp)
	lw	$2,36($sp)
	sw	$2,40($sp)
	sw	$0,44($sp)
	sw	$0,84($sp)
	sw	$0,88($sp)
	sw	$0,92($sp)
	sw	$0,96($sp)
	sw	$0,84($sp)
	lw	$2,40($sp)
	sw	$2,88($sp)
	lw	$2,44($sp)
	sw	$2,96($sp)
	addiu	$2,$sp,84
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
	bne	$3,$2,$L26
	nop

	lw	$2,48($sp)
	lw	$3,4($2)
	lui	$2,%hi($LC1)
	addiu	$4,$2,%lo($LC1)
	move	$5,$3
	lui	$2,%hi(panic)
	addiu	$2,$2,%lo(panic)
	jalr	$2
	nop

$L26:
	lw	$2,48($sp)
	lw	$3,8($2)
	li	$2,-1			# 0xffffffffffffffff
	bne	$3,$2,$L27
	nop

	lw	$2,48($sp)
	lw	$5,0($2)
	lw	$2,48($sp)
	lw	$3,4($2)
	lui	$2,%hi($LC2)
	addiu	$4,$2,%lo($LC2)
	move	$6,$3
	lui	$2,%hi(panic)
	addiu	$2,$2,%lo(panic)
	jalr	$2
	nop

$L27:
	lw	$2,48($sp)
	lw	$2,8($2)
	sw	$2,16($sp)
	li	$2,32793			# 0x8019
	sw	$2,52($sp)
	lw	$2,52($sp)
	sw	$2,56($sp)
	sw	$0,60($sp)
	sw	$0,68($sp)
	sw	$0,72($sp)
	sw	$0,76($sp)
	sw	$0,80($sp)
	sw	$0,68($sp)
	lw	$2,56($sp)
	sw	$2,72($sp)
	lw	$2,60($sp)
	sw	$2,80($sp)
	addiu	$2,$sp,68
	sw	$2,64($sp)
	li	$4,25			# 0x19
	move	$5,$0
	lw	$6,64($sp)
	lui	$2,%hi(__ECNT_HOOK)
	addiu	$2,$2,%lo(__ECNT_HOOK)
	jalr	$2
	nop

	move	$3,$2
	li	$2,-1			# 0xffffffffffffffff
	bne	$3,$2,$L31
	nop

	lw	$2,64($sp)
	lw	$3,4($2)
	lui	$2,%hi($LC1)
	addiu	$4,$2,%lo($LC1)
	move	$5,$3
	lui	$2,%hi(panic)
	addiu	$2,$2,%lo(panic)
	jalr	$2
	nop

$L31:
	lw	$2,64($sp)
	lw	$3,8($2)
	li	$2,-1			# 0xffffffffffffffff
	bne	$3,$2,$L32
	nop

	lw	$2,64($sp)
	lw	$5,0($2)
	lw	$2,64($sp)
	lw	$3,4($2)
	lui	$2,%hi($LC2)
	addiu	$4,$2,%lo($LC2)
	move	$6,$3
	lui	$2,%hi(panic)
	addiu	$2,$2,%lo(panic)
	jalr	$2
	nop

$L32:
	lw	$2,64($sp)
	lw	$2,8($2)
	beq	$2,$0,$L36
	nop

	lui	$2,%hi(gpPhyData)
	lw	$3,%lo(gpPhyData)($2)
	lw	$2,16($sp)
	andi	$2,$2,0x00ff
	andi	$2,$2,0x3
	andi	$4,$2,0x00ff
	lhu	$2,60($3)
	ins	$2,$4,9,2
	sh	$2,60($3)
	lui	$2,%hi(gpPhyData)
	lw	$3,%lo(gpPhyData)($2)
	lhu	$2,60($3)
	li	$4,2			# 0x2
	ins	$2,$4,11,2
	sh	$2,60($3)
	lw	$3,16($sp)
	li	$2,1			# 0x1
	bne	$3,$2,$L37
	nop

	lui	$2,%hi($LC3)
	addiu	$2,$2,%lo($LC3)
	j	$L38
	nop

$L37:
	lui	$2,%hi($LC4)
	addiu	$2,$2,%lo($LC4)
$L38:
	lui	$3,%hi($LC5)
	addiu	$4,$3,%lo($LC5)
	move	$5,$2
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	j	$L13
	nop

$L36:
	move	$4,$0
	lui	$2,%hi(try_set_phy_mode)
	addiu	$2,$2,%lo(try_set_phy_mode)
	jalr	$2
	nop

$L13:
	lw	$31,124($sp)
	addiu	$sp,$sp,128
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	check_sync_result
	.size	check_sync_result, .-check_sync_result
	.rdata
	.align	2
$LC6:
	.ascii	"%s:%d [%s]detect_mode: %d\012\000"
	.section	.text.try_set_phy_mode,"ax",@progbits
	.align	2
	.globl	try_set_phy_mode
	.set	nomips16
	.set	nomicromips
	.ent	try_set_phy_mode
	.type	try_set_phy_mode, @function
try_set_phy_mode:
	.frame	$sp,72,$31		# vars= 40, regs= 1/0, args= 24, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-72
	sw	$31,68($sp)
	sw	$4,72($sp)
	sw	$0,24($sp)
	lui	$2,%hi(gpPhyData)
	lw	$2,%lo(gpPhyData)($2)
	lw	$3,60($2)
	li	$2,402653184			# 0x18000000
	and	$3,$3,$2
	li	$2,268435456			# 0x10000000
	beq	$3,$2,$L40
	nop

	lui	$2,%hi(gpPhyData)
	lw	$2,%lo(gpPhyData)($2)
	lw	$3,60($2)
	li	$2,402653184			# 0x18000000
	and	$3,$3,$2
	li	$2,402653184			# 0x18000000
	bne	$3,$2,$L41
	nop

$L40:
	li	$2,1			# 0x1
	j	$L43
	nop

$L41:
	move	$2,$0
$L43:
	beq	$2,$0,$L44
	nop

	j	$L39
	nop

$L44:
	li	$2,32794			# 0x801a
	sw	$2,32($sp)
	lw	$2,32($sp)
	sw	$2,36($sp)
	sw	$0,40($sp)
	sw	$0,48($sp)
	sw	$0,52($sp)
	sw	$0,56($sp)
	sw	$0,60($sp)
	sw	$0,48($sp)
	lw	$2,36($sp)
	sw	$2,52($sp)
	lw	$2,40($sp)
	sw	$2,60($sp)
	addiu	$2,$sp,48
	sw	$2,44($sp)
	li	$4,25			# 0x19
	move	$5,$0
	lw	$6,44($sp)
	lui	$2,%hi(__ECNT_HOOK)
	addiu	$2,$2,%lo(__ECNT_HOOK)
	jalr	$2
	nop

	move	$3,$2
	li	$2,-1			# 0xffffffffffffffff
	bne	$3,$2,$L46
	nop

	lw	$2,44($sp)
	lw	$3,4($2)
	lui	$2,%hi($LC1)
	addiu	$4,$2,%lo($LC1)
	move	$5,$3
	lui	$2,%hi(panic)
	addiu	$2,$2,%lo(panic)
	jalr	$2
	nop

$L46:
	lw	$2,44($sp)
	lw	$3,8($2)
	li	$2,-1			# 0xffffffffffffffff
	bne	$3,$2,$L47
	nop

	lw	$2,44($sp)
	lw	$5,0($2)
	lw	$2,44($sp)
	lw	$3,4($2)
	lui	$2,%hi($LC2)
	addiu	$4,$2,%lo($LC2)
	move	$6,$3
	lui	$2,%hi(panic)
	addiu	$2,$2,%lo(panic)
	jalr	$2
	nop

$L47:
	lw	$2,44($sp)
	lw	$2,8($2)
	sw	$2,28($sp)
	lw	$2,28($sp)
	xori	$2,$2,0x1
	sltu	$2,$0,$2
	andi	$2,$2,0x00ff
	sw	$2,24($sp)
	lui	$2,%hi(gpPhyData)
	lw	$3,%lo(gpPhyData)($2)
	lw	$2,24($sp)
	andi	$2,$2,0x00ff
	andi	$2,$2,0x3
	andi	$4,$2,0x00ff
	lhu	$2,60($3)
	ins	$2,$4,13,2
	sh	$2,60($3)
	lui	$2,%hi(gpPhyData)
	lw	$3,%lo(gpPhyData)($2)
	lhu	$2,60($3)
	li	$4,1			# 0x1
	ins	$2,$4,11,2
	sh	$2,60($3)
	lui	$2,%hi(xpon_mac_print_open)
	lw	$2,%lo(xpon_mac_print_open)($2)
	beq	$2,$0,$L51
	nop

	lw	$2,24($sp)
	sw	$2,16($sp)
	lui	$2,%hi($LC6)
	addiu	$4,$2,%lo($LC6)
	lui	$2,%hi(__func__.32885)
	addiu	$5,$2,%lo(__func__.32885)
	li	$6,106			# 0x6a
	lui	$2,%hi(__func__.32885)
	addiu	$7,$2,%lo(__func__.32885)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L51:
	lw	$3,24($sp)
	li	$2,1			# 0x1
	bne	$3,$2,$L52
	nop

	lui	$2,%hi(prepare_gpon)
	addiu	$2,$2,%lo(prepare_gpon)
	jalr	$2
	nop

	j	$L53
	nop

$L52:
	lui	$2,%hi(prepare_epon)
	addiu	$2,$2,%lo(prepare_epon)
	jalr	$2
	nop

$L53:
	lui	$2,%hi(gpPhyData)
	lw	$2,%lo(gpPhyData)($2)
	addiu	$4,$2,32
	lui	$2,%hi(gpPhyData)
	lw	$2,%lo(gpPhyData)($2)
	lw	$2,52($2)
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
	addu	$2,$3,$2
	move	$5,$2
	lui	$2,%hi(mod_timer)
	addiu	$2,$2,%lo(mod_timer)
	jalr	$2
	nop

$L39:
	lw	$31,68($sp)
	addiu	$sp,$sp,72
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	try_set_phy_mode
	.size	try_set_phy_mode, .-try_set_phy_mode
	.rdata
	.align	2
	.type	__func__.32874, @object
	.size	__func__.32874, 18
__func__.32874:
	.ascii	"XPON_PHY_SET_MODE\000"
	.align	2
	.type	__func__.32885, @object
	.size	__func__.32885, 17
__func__.32885:
	.ascii	"try_set_phy_mode\000"
	.ident	"GCC: (Buildroot 2015.08.1) 4.9.3"
