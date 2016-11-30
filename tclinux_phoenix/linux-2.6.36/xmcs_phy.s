	.file	1 "xmcs_phy.c"
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
	.ascii	"[%lu0ms]Send PHY transceiver alarm, %x\012\000"
#NO_APP
	.section	.text.xmcs_detect_phy_trans_alarm,"ax",@progbits
	.align	2
	.globl	xmcs_detect_phy_trans_alarm
	.set	nomips16
	.set	nomicromips
	.ent	xmcs_detect_phy_trans_alarm
	.type	xmcs_detect_phy_trans_alarm, @function
xmcs_detect_phy_trans_alarm:
	.frame	$sp,32,$31		# vars= 8, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-32
	sw	$31,28($sp)
	sw	$4,32($sp)
	sw	$0,16($sp)
	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	lhu	$2,-30816($2)
	andi	$2,$2,0x40
	beq	$2,$0,$L2
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

$L2:
	lw	$2,32($sp)
	andi	$2,$2,0x1
	lw	$3,16($sp)
	or	$2,$3,$2
	sw	$2,16($sp)
	lw	$2,32($sp)
	andi	$2,$2,0x2
	lw	$3,16($sp)
	or	$2,$3,$2
	sw	$2,16($sp)
	lw	$2,32($sp)
	andi	$2,$2,0x4
	lw	$3,16($sp)
	or	$2,$3,$2
	sw	$2,16($sp)
	lw	$2,32($sp)
	andi	$2,$2,0x8
	lw	$3,16($sp)
	or	$2,$3,$2
	sw	$2,16($sp)
	lw	$2,32($sp)
	andi	$2,$2,0x10
	lw	$3,16($sp)
	or	$2,$3,$2
	sw	$2,16($sp)
	lw	$2,32($sp)
	andi	$2,$2,0x20
	lw	$3,16($sp)
	or	$2,$3,$2
	sw	$2,16($sp)
	lui	$2,%hi(gpPhyData)
	lw	$2,%lo(gpPhyData)($2)
	lw	$2,60($2)
	ext	$2,$2,25,2
	andi	$2,$2,0x00ff
	beq	$2,$0,$L4
	nop

	li	$3,1			# 0x1
	beq	$2,$3,$L5
	nop

	j	$L1
	nop

$L5:
	li	$4,1			# 0x1
	li	$5,6			# 0x6
	lw	$6,16($sp)
	lui	$2,%hi(xmcs_report_event)
	addiu	$2,$2,%lo(xmcs_report_event)
	jalr	$2
	nop

	j	$L1
	nop

$L4:
	li	$4,2			# 0x2
	li	$5,6			# 0x6
	lw	$6,16($sp)
	lui	$2,%hi(xmcs_report_event)
	addiu	$2,$2,%lo(xmcs_report_event)
	jalr	$2
	nop

	nop
$L1:
	lw	$31,28($sp)
	addiu	$sp,$sp,32
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	xmcs_detect_phy_trans_alarm
	.size	xmcs_detect_phy_trans_alarm, .-xmcs_detect_phy_trans_alarm
	.rdata
	.align	2
$LC1:
	.ascii	"ECNT_HOOK_ERROR occur with cmd_id:0x%x\012\000"
	.align	2
$LC2:
	.ascii	"No such API with type:%d, cmd_id:0x%x\012\000"
	.section	.text.xmcs_set_fec_mode,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	xmcs_set_fec_mode
	.type	xmcs_set_fec_mode, @function
xmcs_set_fec_mode:
	.frame	$sp,120,$31		# vars= 96, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	addiu	$sp,$sp,-120
	sw	$31,116($sp)
	sw	$4,120($sp)
	sw	$0,60($sp)
	sw	$0,64($sp)
	sw	$0,68($sp)
	sw	$0,72($sp)
	sw	$0,76($sp)
	sw	$0,80($sp)
	sw	$0,84($sp)
	sw	$0,88($sp)
	sw	$0,16($sp)
	addiu	$2,$sp,60
	sw	$2,24($sp)
	lw	$2,120($sp)
	sw	$2,28($sp)
	li	$2,32			# 0x20
	sw	$2,20($sp)
	lw	$2,28($sp)
	sw	$2,32($sp)
	lw	$2,20($sp)
	sw	$2,36($sp)
	move	$2,$28
	lw	$2,24($2)
	sw	$2,40($sp)
	lw	$3,32($sp)
	lw	$2,36($sp)
	addu	$3,$3,$2
	lw	$2,32($sp)
	or	$3,$3,$2
	lw	$2,36($sp)
	or	$3,$3,$2
	lw	$2,40($sp)
	and	$2,$3,$2
	sw	$2,44($sp)
	lw	$2,44($sp)
	sltu	$2,$2,1
	andi	$2,$2,0x00ff
	sltu	$2,$0,$2
	andi	$2,$2,0x00ff
	beq	$2,$0,$L8
	lw	$4,24($sp)
	lw	$5,28($sp)
	lw	$6,20($sp)
#APP
 # 68 "/opt/tclinux_phoenix/modules/private/xpon/src/xmcs/xmcs_phy.c" 1
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
	sw	$2,20($sp)
$L8:
	lw	$2,20($sp)
	bne	$2,$0,$L9
	lw	$2,60($sp)
	xori	$2,$2,0x1
	sltu	$2,$2,1
	andi	$2,$2,0x00ff
	sw	$2,92($sp)
	li	$2,20			# 0x14
	sw	$2,48($sp)
	addiu	$2,$sp,92
	sw	$2,52($sp)
	sw	$0,96($sp)
	sw	$0,100($sp)
	sw	$0,104($sp)
	sw	$0,108($sp)
	li	$2,1			# 0x1
	sw	$2,96($sp)
	lw	$2,48($sp)
	sw	$2,100($sp)
	lw	$2,52($sp)
	sw	$2,108($sp)
	addiu	$2,$sp,96
	sw	$2,56($sp)
	li	$4,25			# 0x19
	move	$5,$0
	lw	$6,56($sp)
	lui	$2,%hi(__ECNT_HOOK)
	addiu	$2,$2,%lo(__ECNT_HOOK)
	jalr	$2
	move	$3,$2
	li	$2,-1			# 0xffffffffffffffff
	bne	$3,$2,$L10
	lw	$2,56($sp)
	lw	$3,4($2)
	lui	$2,%hi($LC1)
	addiu	$4,$2,%lo($LC1)
	move	$5,$3
	lui	$2,%hi(panic)
	addiu	$2,$2,%lo(panic)
	jalr	$2
$L10:
	lw	$2,56($sp)
	lw	$3,8($2)
	li	$2,-1			# 0xffffffffffffffff
	bne	$3,$2,$L11
	lw	$2,56($sp)
	lw	$5,0($2)
	lw	$2,56($sp)
	lw	$3,4($2)
	lui	$2,%hi($LC2)
	addiu	$4,$2,%lo($LC2)
	move	$6,$3
	lui	$2,%hi(panic)
	addiu	$2,$2,%lo(panic)
	jalr	$2
$L11:
	lw	$2,56($sp)
	lw	$2,8($2)
	sw	$2,16($sp)
	lw	$2,16($sp)
	bne	$2,$0,$L15
	move	$2,$0
	j	$L16
$L15:
	li	$2,-22			# 0xffffffffffffffea
$L16:
	sw	$2,16($sp)
	j	$L17
$L9:
	li	$2,-14			# 0xfffffffffffffff2
	sw	$2,16($sp)
$L17:
	lw	$2,16($sp)
	lw	$31,116($sp)
	addiu	$sp,$sp,120
	j	$31
	.end	xmcs_set_fec_mode
	.size	xmcs_set_fec_mode, .-xmcs_set_fec_mode
	.section	.text.xmcs_get_fec_status,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	xmcs_get_fec_status
	.type	xmcs_get_fec_status, @function
xmcs_get_fec_status:
	.frame	$sp,152,$31		# vars= 128, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	addiu	$sp,$sp,-152
	sw	$31,148($sp)
	sw	$4,152($sp)
	sw	$0,76($sp)
	sw	$0,80($sp)
	sw	$0,84($sp)
	sw	$0,88($sp)
	sw	$0,92($sp)
	sw	$0,96($sp)
	sw	$0,100($sp)
	sw	$0,104($sp)
	li	$2,32791			# 0x8017
	sw	$2,44($sp)
	lw	$2,44($sp)
	sw	$2,48($sp)
	sw	$0,52($sp)
	sw	$0,124($sp)
	sw	$0,128($sp)
	sw	$0,132($sp)
	sw	$0,136($sp)
	sw	$0,124($sp)
	lw	$2,48($sp)
	sw	$2,128($sp)
	lw	$2,52($sp)
	sw	$2,136($sp)
	addiu	$2,$sp,124
	sw	$2,56($sp)
	li	$4,25			# 0x19
	move	$5,$0
	lw	$6,56($sp)
	lui	$2,%hi(__ECNT_HOOK)
	addiu	$2,$2,%lo(__ECNT_HOOK)
	jalr	$2
	move	$3,$2
	li	$2,-1			# 0xffffffffffffffff
	bne	$3,$2,$L20
	lw	$2,56($sp)
	lw	$3,4($2)
	lui	$2,%hi($LC1)
	addiu	$4,$2,%lo($LC1)
	move	$5,$3
	lui	$2,%hi(panic)
	addiu	$2,$2,%lo(panic)
	jalr	$2
$L20:
	lw	$2,56($sp)
	lw	$3,8($2)
	li	$2,-1			# 0xffffffffffffffff
	bne	$3,$2,$L21
	lw	$2,56($sp)
	lw	$5,0($2)
	lw	$2,56($sp)
	lw	$3,4($2)
	lui	$2,%hi($LC2)
	addiu	$4,$2,%lo($LC2)
	move	$6,$3
	lui	$2,%hi(panic)
	addiu	$2,$2,%lo(panic)
	jalr	$2
$L21:
	lw	$2,56($sp)
	lw	$2,8($2)
	sltu	$2,$0,$2
	andi	$2,$2,0x00ff
	sw	$2,76($sp)
	li	$2,32790			# 0x8016
	sw	$2,60($sp)
	lw	$2,60($sp)
	sw	$2,64($sp)
	sw	$0,68($sp)
	sw	$0,108($sp)
	sw	$0,112($sp)
	sw	$0,116($sp)
	sw	$0,120($sp)
	sw	$0,108($sp)
	lw	$2,64($sp)
	sw	$2,112($sp)
	lw	$2,68($sp)
	sw	$2,120($sp)
	addiu	$2,$sp,108
	sw	$2,72($sp)
	li	$4,25			# 0x19
	move	$5,$0
	lw	$6,72($sp)
	lui	$2,%hi(__ECNT_HOOK)
	addiu	$2,$2,%lo(__ECNT_HOOK)
	jalr	$2
	move	$3,$2
	li	$2,-1			# 0xffffffffffffffff
	bne	$3,$2,$L25
	lw	$2,72($sp)
	lw	$3,4($2)
	lui	$2,%hi($LC1)
	addiu	$4,$2,%lo($LC1)
	move	$5,$3
	lui	$2,%hi(panic)
	addiu	$2,$2,%lo(panic)
	jalr	$2
$L25:
	lw	$2,72($sp)
	lw	$3,8($2)
	li	$2,-1			# 0xffffffffffffffff
	bne	$3,$2,$L26
	lw	$2,72($sp)
	lw	$5,0($2)
	lw	$2,72($sp)
	lw	$3,4($2)
	lui	$2,%hi($LC2)
	addiu	$4,$2,%lo($LC2)
	move	$6,$3
	lui	$2,%hi(panic)
	addiu	$2,$2,%lo(panic)
	jalr	$2
$L26:
	lw	$2,72($sp)
	lw	$2,8($2)
	xori	$2,$2,0x1
	sltu	$2,$2,1
	andi	$2,$2,0x00ff
	sw	$2,80($sp)
	lw	$2,152($sp)
	sw	$2,20($sp)
	addiu	$2,$sp,76
	sw	$2,24($sp)
	li	$2,32			# 0x20
	sw	$2,16($sp)
	lw	$2,20($sp)
	sw	$2,28($sp)
	lw	$2,16($sp)
	sw	$2,32($sp)
	move	$2,$28
	lw	$2,24($2)
	sw	$2,36($sp)
	lw	$3,28($sp)
	lw	$2,32($sp)
	addu	$3,$3,$2
	lw	$2,28($sp)
	or	$3,$3,$2
	lw	$2,32($sp)
	or	$3,$3,$2
	lw	$2,36($sp)
	and	$2,$3,$2
	sw	$2,40($sp)
	lw	$2,40($sp)
	sltu	$2,$2,1
	andi	$2,$2,0x00ff
	sltu	$2,$0,$2
	andi	$2,$2,0x00ff
	beq	$2,$0,$L30
	lw	$4,20($sp)
	lw	$5,24($sp)
	lw	$6,16($sp)
#APP
 # 100 "/opt/tclinux_phoenix/modules/private/xpon/src/xmcs/xmcs_phy.c" 1
	.set	noat
	la	$1, __copy_user
	jalr	$1
	.set	at
	
 # 0 "" 2
#NO_APP
	move	$2,$6
	sw	$2,16($sp)
$L30:
	lw	$2,16($sp)
	beq	$2,$0,$L31
	li	$2,-14			# 0xfffffffffffffff2
	j	$L33
$L31:
	move	$2,$0
$L33:
	lw	$31,148($sp)
	addiu	$sp,$sp,152
	j	$31
	.end	xmcs_get_fec_status
	.size	xmcs_get_fec_status, .-xmcs_get_fec_status
	.section	.text.xmcs_set_clear_fec_counter,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	xmcs_set_clear_fec_counter
	.type	xmcs_set_clear_fec_counter, @function
xmcs_set_clear_fec_counter:
	.frame	$sp,56,$31		# vars= 32, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-56
	sw	$31,52($sp)
	sw	$4,56($sp)
	lw	$2,56($sp)
	lw	$3,8($2)
	li	$2,1			# 0x1
	bne	$3,$2,$L35
	nop

	li	$2,1			# 0x1
	sw	$2,28($sp)
	li	$2,9			# 0x9
	sw	$2,16($sp)
	addiu	$2,$sp,28
	sw	$2,20($sp)
	sw	$0,32($sp)
	sw	$0,36($sp)
	sw	$0,40($sp)
	sw	$0,44($sp)
	li	$2,1			# 0x1
	sw	$2,32($sp)
	lw	$2,16($sp)
	sw	$2,36($sp)
	lw	$2,20($sp)
	sw	$2,44($sp)
	addiu	$2,$sp,32
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
	bne	$3,$2,$L36
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

$L36:
	lw	$2,24($sp)
	lw	$3,8($2)
	li	$2,-1			# 0xffffffffffffffff
	bne	$3,$2,$L35
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

$L35:
	move	$2,$0
	lw	$31,52($sp)
	addiu	$sp,$sp,56
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	xmcs_set_clear_fec_counter
	.size	xmcs_set_clear_fec_counter, .-xmcs_set_clear_fec_counter
	.section	.text.xmcs_get_fec_counter,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	xmcs_get_fec_counter
	.type	xmcs_get_fec_counter, @function
xmcs_get_fec_counter:
	.frame	$sp,136,$31		# vars= 112, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	addiu	$sp,$sp,-136
	sw	$31,132($sp)
	sw	$4,136($sp)
	sw	$0,56($sp)
	sw	$0,60($sp)
	sw	$0,64($sp)
	sw	$0,68($sp)
	sw	$0,72($sp)
	sw	$0,76($sp)
	sw	$0,80($sp)
	sw	$0,84($sp)
	sw	$0,88($sp)
	sw	$0,92($sp)
	sw	$0,96($sp)
	sw	$0,100($sp)
	sw	$0,104($sp)
	li	$2,32781			# 0x800d
	sw	$2,44($sp)
	addiu	$2,$sp,88
	sw	$2,48($sp)
	sw	$0,108($sp)
	sw	$0,112($sp)
	sw	$0,116($sp)
	sw	$0,120($sp)
	sw	$0,108($sp)
	lw	$2,44($sp)
	sw	$2,112($sp)
	lw	$2,48($sp)
	sw	$2,120($sp)
	addiu	$2,$sp,108
	sw	$2,52($sp)
	li	$4,25			# 0x19
	move	$5,$0
	lw	$6,52($sp)
	lui	$2,%hi(__ECNT_HOOK)
	addiu	$2,$2,%lo(__ECNT_HOOK)
	jalr	$2
	move	$3,$2
	li	$2,-1			# 0xffffffffffffffff
	bne	$3,$2,$L42
	lw	$2,52($sp)
	lw	$3,4($2)
	lui	$2,%hi($LC1)
	addiu	$4,$2,%lo($LC1)
	move	$5,$3
	lui	$2,%hi(panic)
	addiu	$2,$2,%lo(panic)
	jalr	$2
$L42:
	lw	$2,52($sp)
	lw	$3,8($2)
	li	$2,-1			# 0xffffffffffffffff
	bne	$3,$2,$L43
	lw	$2,52($sp)
	lw	$5,0($2)
	lw	$2,52($sp)
	lw	$3,4($2)
	lui	$2,%hi($LC2)
	addiu	$4,$2,%lo($LC2)
	move	$6,$3
	lui	$2,%hi(panic)
	addiu	$2,$2,%lo(panic)
	jalr	$2
$L43:
	lw	$2,88($sp)
	sw	$2,68($sp)
	lw	$2,92($sp)
	sw	$2,72($sp)
	lw	$2,96($sp)
	sw	$2,76($sp)
	lw	$2,100($sp)
	sw	$2,80($sp)
	lw	$2,104($sp)
	sw	$2,84($sp)
	lw	$2,136($sp)
	sw	$2,20($sp)
	addiu	$2,$sp,56
	sw	$2,24($sp)
	li	$2,32			# 0x20
	sw	$2,16($sp)
	lw	$2,20($sp)
	sw	$2,28($sp)
	lw	$2,16($sp)
	sw	$2,32($sp)
	move	$2,$28
	lw	$2,24($2)
	sw	$2,36($sp)
	lw	$3,28($sp)
	lw	$2,32($sp)
	addu	$3,$3,$2
	lw	$2,28($sp)
	or	$3,$3,$2
	lw	$2,32($sp)
	or	$3,$3,$2
	lw	$2,36($sp)
	and	$2,$3,$2
	sw	$2,40($sp)
	lw	$2,40($sp)
	sltu	$2,$2,1
	andi	$2,$2,0x00ff
	sltu	$2,$0,$2
	andi	$2,$2,0x00ff
	beq	$2,$0,$L46
	lw	$4,20($sp)
	lw	$5,24($sp)
	lw	$6,16($sp)
#APP
 # 165 "/opt/tclinux_phoenix/modules/private/xpon/src/xmcs/xmcs_phy.c" 1
	.set	noat
	la	$1, __copy_user
	jalr	$1
	.set	at
	
 # 0 "" 2
#NO_APP
	move	$2,$6
	sw	$2,16($sp)
$L46:
	move	$2,$0
	lw	$31,132($sp)
	addiu	$sp,$sp,136
	j	$31
	.end	xmcs_get_fec_counter
	.size	xmcs_get_fec_counter, .-xmcs_get_fec_counter
	.section	.text.xmcs_set_clear_frame_counter,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	xmcs_set_clear_frame_counter
	.type	xmcs_set_clear_frame_counter, @function
xmcs_set_clear_frame_counter:
	.frame	$sp,56,$31		# vars= 32, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-56
	sw	$31,52($sp)
	sw	$4,56($sp)
	lw	$2,56($sp)
	lw	$3,0($2)
	li	$2,4			# 0x4
	bne	$3,$2,$L49
	nop

	li	$2,1			# 0x1
	sw	$2,28($sp)
	li	$2,9			# 0x9
	sw	$2,16($sp)
	addiu	$2,$sp,28
	sw	$2,20($sp)
	sw	$0,32($sp)
	sw	$0,36($sp)
	sw	$0,40($sp)
	sw	$0,44($sp)
	li	$2,1			# 0x1
	sw	$2,32($sp)
	lw	$2,16($sp)
	sw	$2,36($sp)
	lw	$2,20($sp)
	sw	$2,44($sp)
	addiu	$2,$sp,32
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
	bne	$3,$2,$L50
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

$L50:
	lw	$2,24($sp)
	lw	$3,8($2)
	li	$2,-1			# 0xffffffffffffffff
	bne	$3,$2,$L49
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

$L49:
	move	$2,$0
	lw	$31,52($sp)
	addiu	$sp,$sp,56
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	xmcs_set_clear_frame_counter
	.size	xmcs_set_clear_frame_counter, .-xmcs_set_clear_frame_counter
	.section	.text.xmcs_get_frame_counter,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	xmcs_get_frame_counter
	.type	xmcs_get_frame_counter, @function
xmcs_get_frame_counter:
	.frame	$sp,112,$31		# vars= 88, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	addiu	$sp,$sp,-112
	sw	$31,108($sp)
	sw	$4,112($sp)
	sw	$0,56($sp)
	sw	$0,60($sp)
	sw	$0,64($sp)
	sw	$0,68($sp)
	sw	$0,72($sp)
	sw	$0,76($sp)
	sw	$0,80($sp)
	li	$2,32787			# 0x8013
	sw	$2,44($sp)
	addiu	$2,$sp,56
	sw	$2,48($sp)
	sw	$0,84($sp)
	sw	$0,88($sp)
	sw	$0,92($sp)
	sw	$0,96($sp)
	sw	$0,84($sp)
	lw	$2,44($sp)
	sw	$2,88($sp)
	lw	$2,48($sp)
	sw	$2,96($sp)
	addiu	$2,$sp,84
	sw	$2,52($sp)
	li	$4,25			# 0x19
	move	$5,$0
	lw	$6,52($sp)
	lui	$2,%hi(__ECNT_HOOK)
	addiu	$2,$2,%lo(__ECNT_HOOK)
	jalr	$2
	move	$3,$2
	li	$2,-1			# 0xffffffffffffffff
	bne	$3,$2,$L56
	lw	$2,52($sp)
	lw	$3,4($2)
	lui	$2,%hi($LC1)
	addiu	$4,$2,%lo($LC1)
	move	$5,$3
	lui	$2,%hi(panic)
	addiu	$2,$2,%lo(panic)
	jalr	$2
$L56:
	lw	$2,52($sp)
	lw	$3,8($2)
	li	$2,-1			# 0xffffffffffffffff
	bne	$3,$2,$L57
	lw	$2,52($sp)
	lw	$5,0($2)
	lw	$2,52($sp)
	lw	$3,4($2)
	lui	$2,%hi($LC2)
	addiu	$4,$2,%lo($LC2)
	move	$6,$3
	lui	$2,%hi(panic)
	addiu	$2,$2,%lo(panic)
	jalr	$2
$L57:
	lw	$2,56($sp)
	sw	$2,72($sp)
	lw	$2,60($sp)
	sw	$2,76($sp)
	lw	$2,64($sp)
	sw	$2,80($sp)
	lw	$2,112($sp)
	sw	$2,20($sp)
	addiu	$2,$sp,68
	sw	$2,24($sp)
	li	$2,16			# 0x10
	sw	$2,16($sp)
	lw	$2,20($sp)
	sw	$2,28($sp)
	lw	$2,16($sp)
	sw	$2,32($sp)
	move	$2,$28
	lw	$2,24($2)
	sw	$2,36($sp)
	lw	$3,28($sp)
	lw	$2,32($sp)
	addu	$3,$3,$2
	lw	$2,28($sp)
	or	$3,$3,$2
	lw	$2,32($sp)
	or	$3,$3,$2
	lw	$2,36($sp)
	and	$2,$3,$2
	sw	$2,40($sp)
	lw	$2,40($sp)
	sltu	$2,$2,1
	andi	$2,$2,0x00ff
	sltu	$2,$0,$2
	andi	$2,$2,0x00ff
	beq	$2,$0,$L60
	lw	$4,20($sp)
	lw	$5,24($sp)
	lw	$6,16($sp)
#APP
 # 216 "/opt/tclinux_phoenix/modules/private/xpon/src/xmcs/xmcs_phy.c" 1
	.set	noat
	la	$1, __copy_user
	jalr	$1
	.set	at
	
 # 0 "" 2
#NO_APP
	move	$2,$6
	sw	$2,16($sp)
$L60:
	move	$2,$0
	lw	$31,108($sp)
	addiu	$sp,$sp,112
	j	$31
	.end	xmcs_get_frame_counter
	.size	xmcs_get_frame_counter, .-xmcs_get_frame_counter
	.section	.text.xmcs_set_transceiver_config,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	xmcs_set_transceiver_config
	.type	xmcs_set_transceiver_config, @function
xmcs_set_transceiver_config:
	.frame	$sp,136,$31		# vars= 112, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	addiu	$sp,$sp,-136
	sw	$31,132($sp)
	sw	$4,136($sp)
	sw	$0,68($sp)
	sw	$0,72($sp)
	sw	$0,76($sp)
	sw	$0,80($sp)
	addiu	$2,$sp,68
	sw	$2,20($sp)
	lw	$2,136($sp)
	sw	$2,24($sp)
	li	$2,16			# 0x10
	sw	$2,16($sp)
	lw	$2,24($sp)
	sw	$2,28($sp)
	lw	$2,16($sp)
	sw	$2,32($sp)
	move	$2,$28
	lw	$2,24($2)
	sw	$2,36($sp)
	lw	$3,28($sp)
	lw	$2,32($sp)
	addu	$3,$3,$2
	lw	$2,28($sp)
	or	$3,$3,$2
	lw	$2,32($sp)
	or	$3,$3,$2
	lw	$2,36($sp)
	and	$2,$3,$2
	sw	$2,40($sp)
	lw	$2,40($sp)
	sltu	$2,$2,1
	andi	$2,$2,0x00ff
	sltu	$2,$0,$2
	andi	$2,$2,0x00ff
	beq	$2,$0,$L63
	lw	$4,20($sp)
	lw	$5,24($sp)
	lw	$6,16($sp)
#APP
 # 241 "/opt/tclinux_phoenix/modules/private/xpon/src/xmcs/xmcs_phy.c" 1
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
$L63:
	lw	$2,16($sp)
	beq	$2,$0,$L64
	li	$2,-14			# 0xfffffffffffffff2
	j	$L77
$L64:
	lw	$2,68($sp)
	xori	$2,$2,0x1
	sltu	$2,$2,1
	andi	$2,$2,0x00ff
	sb	$2,84($sp)
	lw	$2,76($sp)
	xori	$2,$2,0x1
	sltu	$2,$2,1
	andi	$2,$2,0x00ff
	sb	$2,85($sp)
	lw	$2,72($sp)
	xori	$2,$2,0x1
	sltu	$2,$2,1
	andi	$2,$2,0x00ff
	sb	$2,86($sp)
	li	$2,16			# 0x10
	sw	$2,44($sp)
	addiu	$2,$sp,84
	sw	$2,48($sp)
	sw	$0,108($sp)
	sw	$0,112($sp)
	sw	$0,116($sp)
	sw	$0,120($sp)
	li	$2,1			# 0x1
	sw	$2,108($sp)
	lw	$2,44($sp)
	sw	$2,112($sp)
	lw	$2,48($sp)
	sw	$2,120($sp)
	addiu	$2,$sp,108
	sw	$2,52($sp)
	li	$4,25			# 0x19
	move	$5,$0
	lw	$6,52($sp)
	lui	$2,%hi(__ECNT_HOOK)
	addiu	$2,$2,%lo(__ECNT_HOOK)
	jalr	$2
	move	$3,$2
	li	$2,-1			# 0xffffffffffffffff
	bne	$3,$2,$L66
	lw	$2,52($sp)
	lw	$3,4($2)
	lui	$2,%hi($LC1)
	addiu	$4,$2,%lo($LC1)
	move	$5,$3
	lui	$2,%hi(panic)
	addiu	$2,$2,%lo(panic)
	jalr	$2
$L66:
	lw	$2,52($sp)
	lw	$3,8($2)
	li	$2,-1			# 0xffffffffffffffff
	bne	$3,$2,$L67
	lw	$2,52($sp)
	lw	$5,0($2)
	lw	$2,52($sp)
	lw	$3,4($2)
	lui	$2,%hi($LC2)
	addiu	$4,$2,%lo($LC2)
	move	$6,$3
	lui	$2,%hi(panic)
	addiu	$2,$2,%lo(panic)
	jalr	$2
$L67:
	lw	$2,52($sp)
	lw	$2,8($2)
	beq	$2,$0,$L70
	li	$2,-22			# 0xffffffffffffffea
	j	$L77
$L70:
	lw	$2,136($sp)
	lw	$2,12($2)
	xori	$2,$2,0x1
	sltu	$2,$2,1
	andi	$2,$2,0x00ff
	sw	$2,88($sp)
	li	$2,21			# 0x15
	sw	$2,56($sp)
	addiu	$2,$sp,88
	sw	$2,60($sp)
	sw	$0,92($sp)
	sw	$0,96($sp)
	sw	$0,100($sp)
	sw	$0,104($sp)
	li	$2,1			# 0x1
	sw	$2,92($sp)
	lw	$2,56($sp)
	sw	$2,96($sp)
	lw	$2,60($sp)
	sw	$2,104($sp)
	addiu	$2,$sp,92
	sw	$2,64($sp)
	li	$4,25			# 0x19
	move	$5,$0
	lw	$6,64($sp)
	lui	$2,%hi(__ECNT_HOOK)
	addiu	$2,$2,%lo(__ECNT_HOOK)
	jalr	$2
	move	$3,$2
	li	$2,-1			# 0xffffffffffffffff
	bne	$3,$2,$L71
	lw	$2,64($sp)
	lw	$3,4($2)
	lui	$2,%hi($LC1)
	addiu	$4,$2,%lo($LC1)
	move	$5,$3
	lui	$2,%hi(panic)
	addiu	$2,$2,%lo(panic)
	jalr	$2
$L71:
	lw	$2,64($sp)
	lw	$3,8($2)
	li	$2,-1			# 0xffffffffffffffff
	bne	$3,$2,$L72
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
$L72:
	lw	$2,64($sp)
	lw	$2,8($2)
	beq	$2,$0,$L76
	li	$2,-22			# 0xffffffffffffffea
	j	$L77
$L76:
	move	$2,$0
$L77:
	lw	$31,132($sp)
	addiu	$sp,$sp,136
	j	$31
	.end	xmcs_set_transceiver_config
	.size	xmcs_set_transceiver_config, .-xmcs_set_transceiver_config
	.section	.text.xmcs_get_transceiver_config,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	xmcs_get_transceiver_config
	.type	xmcs_get_transceiver_config, @function
xmcs_get_transceiver_config:
	.frame	$sp,136,$31		# vars= 112, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	addiu	$sp,$sp,-136
	sw	$31,132($sp)
	sw	$4,136($sp)
	li	$2,32778			# 0x800a
	sw	$2,44($sp)
	addiu	$2,$sp,72
	sw	$2,48($sp)
	sw	$0,108($sp)
	sw	$0,112($sp)
	sw	$0,116($sp)
	sw	$0,120($sp)
	sw	$0,108($sp)
	lw	$2,44($sp)
	sw	$2,112($sp)
	lw	$2,48($sp)
	sw	$2,120($sp)
	addiu	$2,$sp,108
	sw	$2,52($sp)
	li	$4,25			# 0x19
	move	$5,$0
	lw	$6,52($sp)
	lui	$2,%hi(__ECNT_HOOK)
	addiu	$2,$2,%lo(__ECNT_HOOK)
	jalr	$2
	move	$3,$2
	li	$2,-1			# 0xffffffffffffffff
	bne	$3,$2,$L79
	lw	$2,52($sp)
	lw	$3,4($2)
	lui	$2,%hi($LC1)
	addiu	$4,$2,%lo($LC1)
	move	$5,$3
	lui	$2,%hi(panic)
	addiu	$2,$2,%lo(panic)
	jalr	$2
$L79:
	lw	$2,52($sp)
	lw	$3,8($2)
	li	$2,-1			# 0xffffffffffffffff
	bne	$3,$2,$L80
	lw	$2,52($sp)
	lw	$5,0($2)
	lw	$2,52($sp)
	lw	$3,4($2)
	lui	$2,%hi($LC2)
	addiu	$4,$2,%lo($LC2)
	move	$6,$3
	lui	$2,%hi(panic)
	addiu	$2,$2,%lo(panic)
	jalr	$2
$L80:
	lbu	$2,72($sp)
	xori	$2,$2,0x1
	sltu	$2,$2,1
	andi	$2,$2,0x00ff
	sw	$2,76($sp)
	lbu	$2,73($sp)
	xori	$2,$2,0x1
	sltu	$2,$2,1
	andi	$2,$2,0x00ff
	sw	$2,80($sp)
	lbu	$2,74($sp)
	xori	$2,$2,0x1
	sltu	$2,$2,1
	andi	$2,$2,0x00ff
	sw	$2,84($sp)
	li	$2,32792			# 0x8018
	sw	$2,56($sp)
	lw	$2,56($sp)
	sw	$2,60($sp)
	sw	$0,64($sp)
	sw	$0,92($sp)
	sw	$0,96($sp)
	sw	$0,100($sp)
	sw	$0,104($sp)
	sw	$0,92($sp)
	lw	$2,60($sp)
	sw	$2,96($sp)
	lw	$2,64($sp)
	sw	$2,104($sp)
	addiu	$2,$sp,92
	sw	$2,68($sp)
	li	$4,25			# 0x19
	move	$5,$0
	lw	$6,68($sp)
	lui	$2,%hi(__ECNT_HOOK)
	addiu	$2,$2,%lo(__ECNT_HOOK)
	jalr	$2
	move	$3,$2
	li	$2,-1			# 0xffffffffffffffff
	bne	$3,$2,$L83
	lw	$2,68($sp)
	lw	$3,4($2)
	lui	$2,%hi($LC1)
	addiu	$4,$2,%lo($LC1)
	move	$5,$3
	lui	$2,%hi(panic)
	addiu	$2,$2,%lo(panic)
	jalr	$2
$L83:
	lw	$2,68($sp)
	lw	$3,8($2)
	li	$2,-1			# 0xffffffffffffffff
	bne	$3,$2,$L84
	lw	$2,68($sp)
	lw	$5,0($2)
	lw	$2,68($sp)
	lw	$3,4($2)
	lui	$2,%hi($LC2)
	addiu	$4,$2,%lo($LC2)
	move	$6,$3
	lui	$2,%hi(panic)
	addiu	$2,$2,%lo(panic)
	jalr	$2
$L84:
	lw	$2,68($sp)
	lw	$2,8($2)
	xori	$2,$2,0x1
	sltu	$2,$2,1
	andi	$2,$2,0x00ff
	sw	$2,88($sp)
	lw	$2,136($sp)
	sw	$2,20($sp)
	addiu	$2,$sp,76
	sw	$2,24($sp)
	li	$2,16			# 0x10
	sw	$2,16($sp)
	lw	$2,20($sp)
	sw	$2,28($sp)
	lw	$2,16($sp)
	sw	$2,32($sp)
	move	$2,$28
	lw	$2,24($2)
	sw	$2,36($sp)
	lw	$3,28($sp)
	lw	$2,32($sp)
	addu	$3,$3,$2
	lw	$2,28($sp)
	or	$3,$3,$2
	lw	$2,32($sp)
	or	$3,$3,$2
	lw	$2,36($sp)
	and	$2,$3,$2
	sw	$2,40($sp)
	lw	$2,40($sp)
	sltu	$2,$2,1
	andi	$2,$2,0x00ff
	sltu	$2,$0,$2
	andi	$2,$2,0x00ff
	beq	$2,$0,$L88
	lw	$4,20($sp)
	lw	$5,24($sp)
	lw	$6,16($sp)
#APP
 # 286 "/opt/tclinux_phoenix/modules/private/xpon/src/xmcs/xmcs_phy.c" 1
	.set	noat
	la	$1, __copy_user
	jalr	$1
	.set	at
	
 # 0 "" 2
#NO_APP
	move	$2,$6
	sw	$2,16($sp)
$L88:
	lw	$2,16($sp)
	beq	$2,$0,$L89
	li	$2,-14			# 0xfffffffffffffff2
	j	$L91
$L89:
	move	$2,$0
$L91:
	lw	$31,132($sp)
	addiu	$sp,$sp,136
	j	$31
	.end	xmcs_get_transceiver_config
	.size	xmcs_get_transceiver_config, .-xmcs_get_transceiver_config
	.section	.text.xmcs_get_transceiver_params,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	xmcs_get_transceiver_params
	.type	xmcs_get_transceiver_params, @function
xmcs_get_transceiver_params:
	.frame	$sp,88,$31		# vars= 64, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	addiu	$sp,$sp,-88
	sw	$31,84($sp)
	sw	$4,88($sp)
	lui	$2,%hi(gpPhyData)
	lw	$2,%lo(gpPhyData)($2)
	addiu	$2,$2,92
	sw	$2,44($sp)
	lw	$2,44($sp)
	move	$4,$2
	lui	$2,%hi(_raw_spin_lock)
	addiu	$2,$2,%lo(_raw_spin_lock)
	jalr	$2
	lui	$2,%hi(gpPhyData)
	lw	$2,%lo(gpPhyData)($2)
	lhu	$2,96($2)
	sh	$2,68($sp)
	lui	$2,%hi(gpPhyData)
	lw	$2,%lo(gpPhyData)($2)
	lhu	$2,98($2)
	sh	$2,70($sp)
	lui	$2,%hi(gpPhyData)
	lw	$2,%lo(gpPhyData)($2)
	lhu	$2,100($2)
	sh	$2,72($sp)
	lui	$2,%hi(gpPhyData)
	lw	$2,%lo(gpPhyData)($2)
	lhu	$2,102($2)
	sh	$2,74($sp)
	lui	$2,%hi(gpPhyData)
	lw	$2,%lo(gpPhyData)($2)
	lhu	$2,104($2)
	sh	$2,76($sp)
	lui	$2,%hi(gpPhyData)
	lw	$2,%lo(gpPhyData)($2)
	addiu	$2,$2,92
	sw	$2,48($sp)
	lw	$2,48($sp)
	sw	$2,52($sp)
	lw	$2,52($sp)
	sw	$2,56($sp)
	lw	$2,56($sp)
	sw	$2,60($sp)
	lw	$2,60($sp)
	lhu	$2,2($2)
	addiu	$2,$2,1
	sw	$2,64($sp)
#APP
 # 139 "/opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/spinlock.h" 1
	.set	push
	.set	noreorder
	.set	mips2
	sync
	.set	pop
 # 0 "" 2
#NO_APP
	lw	$2,64($sp)
	andi	$3,$2,0xffff
	lw	$2,60($sp)
	sh	$3,2($2)
#APP
 # 141 "/opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/spinlock.h" 1
	.set	push
	.set	noreorder
	.set	mips2
	sync
	.set	pop
 # 0 "" 2
#NO_APP
	lw	$2,88($sp)
	sw	$2,20($sp)
	addiu	$2,$sp,68
	sw	$2,24($sp)
	li	$2,10			# 0xa
	sw	$2,16($sp)
	lw	$2,20($sp)
	sw	$2,28($sp)
	lw	$2,16($sp)
	sw	$2,32($sp)
	move	$2,$28
	lw	$2,24($2)
	sw	$2,36($sp)
	lw	$3,28($sp)
	lw	$2,32($sp)
	addu	$3,$3,$2
	lw	$2,28($sp)
	or	$3,$3,$2
	lw	$2,32($sp)
	or	$3,$3,$2
	lw	$2,36($sp)
	and	$2,$3,$2
	sw	$2,40($sp)
	lw	$2,40($sp)
	sltu	$2,$2,1
	andi	$2,$2,0x00ff
	sltu	$2,$0,$2
	andi	$2,$2,0x00ff
	beq	$2,$0,$L93
	lw	$4,20($sp)
	lw	$5,24($sp)
	lw	$6,16($sp)
#APP
 # 323 "/opt/tclinux_phoenix/modules/private/xpon/src/xmcs/xmcs_phy.c" 1
	.set	noat
	la	$1, __copy_user
	jalr	$1
	.set	at
	
 # 0 "" 2
#NO_APP
	move	$2,$6
	sw	$2,16($sp)
$L93:
	move	$2,$0
	lw	$31,84($sp)
	addiu	$sp,$sp,88
	j	$31
	.end	xmcs_get_transceiver_params
	.size	xmcs_get_transceiver_params, .-xmcs_get_transceiver_params
	.section	.text.xmcs_set_burst_config,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	xmcs_set_burst_config
	.type	xmcs_set_burst_config, @function
xmcs_set_burst_config:
	.frame	$sp,96,$31		# vars= 72, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	addiu	$sp,$sp,-96
	sw	$31,92($sp)
	sw	$4,96($sp)
	sw	$0,60($sp)
	sw	$0,20($sp)
	addiu	$2,$sp,60
	sw	$2,24($sp)
	lw	$2,96($sp)
	sw	$2,28($sp)
	li	$2,4			# 0x4
	sw	$2,16($sp)
	lw	$2,28($sp)
	sw	$2,32($sp)
	lw	$2,16($sp)
	sw	$2,36($sp)
	move	$2,$28
	lw	$2,24($2)
	sw	$2,40($sp)
	lw	$3,32($sp)
	lw	$2,36($sp)
	addu	$3,$3,$2
	lw	$2,32($sp)
	or	$3,$3,$2
	lw	$2,36($sp)
	or	$3,$3,$2
	lw	$2,40($sp)
	and	$2,$3,$2
	sw	$2,44($sp)
	lw	$2,44($sp)
	sltu	$2,$2,1
	andi	$2,$2,0x00ff
	sltu	$2,$0,$2
	andi	$2,$2,0x00ff
	beq	$2,$0,$L96
	lw	$4,24($sp)
	lw	$5,28($sp)
	lw	$6,16($sp)
#APP
 # 343 "/opt/tclinux_phoenix/modules/private/xpon/src/xmcs/xmcs_phy.c" 1
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
$L96:
	lw	$2,16($sp)
	beq	$2,$0,$L97
	li	$2,-14			# 0xfffffffffffffff2
	j	$L105
$L97:
	lw	$2,60($sp)
	xori	$2,$2,0x1
	sltu	$2,$2,1
	andi	$2,$2,0x00ff
	sw	$2,20($sp)
	lw	$2,20($sp)
	sw	$2,64($sp)
	li	$2,13			# 0xd
	sw	$2,48($sp)
	addiu	$2,$sp,64
	sw	$2,52($sp)
	sw	$0,68($sp)
	sw	$0,72($sp)
	sw	$0,76($sp)
	sw	$0,80($sp)
	li	$2,1			# 0x1
	sw	$2,68($sp)
	lw	$2,48($sp)
	sw	$2,72($sp)
	lw	$2,52($sp)
	sw	$2,80($sp)
	addiu	$2,$sp,68
	sw	$2,56($sp)
	li	$4,25			# 0x19
	move	$5,$0
	lw	$6,56($sp)
	lui	$2,%hi(__ECNT_HOOK)
	addiu	$2,$2,%lo(__ECNT_HOOK)
	jalr	$2
	move	$3,$2
	li	$2,-1			# 0xffffffffffffffff
	bne	$3,$2,$L99
	lw	$2,56($sp)
	lw	$3,4($2)
	lui	$2,%hi($LC1)
	addiu	$4,$2,%lo($LC1)
	move	$5,$3
	lui	$2,%hi(panic)
	addiu	$2,$2,%lo(panic)
	jalr	$2
$L99:
	lw	$2,56($sp)
	lw	$3,8($2)
	li	$2,-1			# 0xffffffffffffffff
	bne	$3,$2,$L100
	lw	$2,56($sp)
	lw	$5,0($2)
	lw	$2,56($sp)
	lw	$3,4($2)
	lui	$2,%hi($LC2)
	addiu	$4,$2,%lo($LC2)
	move	$6,$3
	lui	$2,%hi(panic)
	addiu	$2,$2,%lo(panic)
	jalr	$2
$L100:
	lw	$2,56($sp)
	lw	$2,8($2)
	beq	$2,$0,$L104
	li	$2,-22			# 0xffffffffffffffea
	j	$L105
$L104:
	move	$2,$0
$L105:
	lw	$31,92($sp)
	addiu	$sp,$sp,96
	j	$31
	.end	xmcs_set_burst_config
	.size	xmcs_set_burst_config, .-xmcs_set_burst_config
	.section	.text.xmcs_get_burst_config,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	xmcs_get_burst_config
	.type	xmcs_get_burst_config, @function
xmcs_get_burst_config:
	.frame	$sp,96,$31		# vars= 72, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	addiu	$sp,$sp,-96
	sw	$31,92($sp)
	sw	$4,96($sp)
	sw	$0,64($sp)
	li	$2,32777			# 0x8009
	sw	$2,48($sp)
	lw	$2,48($sp)
	sw	$2,52($sp)
	sw	$0,56($sp)
	sw	$0,68($sp)
	sw	$0,72($sp)
	sw	$0,76($sp)
	sw	$0,80($sp)
	sw	$0,68($sp)
	lw	$2,52($sp)
	sw	$2,72($sp)
	lw	$2,56($sp)
	sw	$2,80($sp)
	addiu	$2,$sp,68
	sw	$2,60($sp)
	li	$4,25			# 0x19
	move	$5,$0
	lw	$6,60($sp)
	lui	$2,%hi(__ECNT_HOOK)
	addiu	$2,$2,%lo(__ECNT_HOOK)
	jalr	$2
	move	$3,$2
	li	$2,-1			# 0xffffffffffffffff
	bne	$3,$2,$L107
	lw	$2,60($sp)
	lw	$3,4($2)
	lui	$2,%hi($LC1)
	addiu	$4,$2,%lo($LC1)
	move	$5,$3
	lui	$2,%hi(panic)
	addiu	$2,$2,%lo(panic)
	jalr	$2
$L107:
	lw	$2,60($sp)
	lw	$3,8($2)
	li	$2,-1			# 0xffffffffffffffff
	bne	$3,$2,$L108
	lw	$2,60($sp)
	lw	$5,0($2)
	lw	$2,60($sp)
	lw	$3,4($2)
	lui	$2,%hi($LC2)
	addiu	$4,$2,%lo($LC2)
	move	$6,$3
	lui	$2,%hi(panic)
	addiu	$2,$2,%lo(panic)
	jalr	$2
$L108:
	lw	$2,60($sp)
	lw	$2,8($2)
	sw	$2,20($sp)
	lw	$2,20($sp)
	xori	$2,$2,0x1
	sltu	$2,$2,1
	andi	$2,$2,0x00ff
	sw	$2,64($sp)
	lw	$2,96($sp)
	sw	$2,24($sp)
	addiu	$2,$sp,64
	sw	$2,28($sp)
	li	$2,4			# 0x4
	sw	$2,16($sp)
	lw	$2,24($sp)
	sw	$2,32($sp)
	lw	$2,16($sp)
	sw	$2,36($sp)
	move	$2,$28
	lw	$2,24($2)
	sw	$2,40($sp)
	lw	$3,32($sp)
	lw	$2,36($sp)
	addu	$3,$3,$2
	lw	$2,32($sp)
	or	$3,$3,$2
	lw	$2,36($sp)
	or	$3,$3,$2
	lw	$2,40($sp)
	and	$2,$3,$2
	sw	$2,44($sp)
	lw	$2,44($sp)
	sltu	$2,$2,1
	andi	$2,$2,0x00ff
	sltu	$2,$0,$2
	andi	$2,$2,0x00ff
	beq	$2,$0,$L112
	lw	$4,24($sp)
	lw	$5,28($sp)
	lw	$6,16($sp)
#APP
 # 364 "/opt/tclinux_phoenix/modules/private/xpon/src/xmcs/xmcs_phy.c" 1
	.set	noat
	la	$1, __copy_user
	jalr	$1
	.set	at
	
 # 0 "" 2
#NO_APP
	move	$2,$6
	sw	$2,16($sp)
$L112:
	lw	$2,16($sp)
	beq	$2,$0,$L113
	li	$2,-14			# 0xfffffffffffffff2
	j	$L115
$L113:
	move	$2,$0
$L115:
	lw	$31,92($sp)
	addiu	$sp,$sp,96
	j	$31
	.end	xmcs_get_burst_config
	.size	xmcs_get_burst_config, .-xmcs_get_burst_config
	.section	.text.xmcs_get_phy_fec_status,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	xmcs_get_phy_fec_status
	.type	xmcs_get_phy_fec_status, @function
xmcs_get_phy_fec_status:
	.frame	$sp,128,$31		# vars= 104, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	addiu	$sp,$sp,-128
	sw	$31,124($sp)
	sw	$4,128($sp)
	sw	$0,76($sp)
	sw	$0,80($sp)
	li	$2,32790			# 0x8016
	sw	$2,44($sp)
	lw	$2,44($sp)
	sw	$2,48($sp)
	sw	$0,52($sp)
	sw	$0,100($sp)
	sw	$0,104($sp)
	sw	$0,108($sp)
	sw	$0,112($sp)
	sw	$0,100($sp)
	lw	$2,48($sp)
	sw	$2,104($sp)
	lw	$2,52($sp)
	sw	$2,112($sp)
	addiu	$2,$sp,100
	sw	$2,56($sp)
	li	$4,25			# 0x19
	move	$5,$0
	lw	$6,56($sp)
	lui	$2,%hi(__ECNT_HOOK)
	addiu	$2,$2,%lo(__ECNT_HOOK)
	jalr	$2
	move	$3,$2
	li	$2,-1			# 0xffffffffffffffff
	bne	$3,$2,$L117
	lw	$2,56($sp)
	lw	$3,4($2)
	lui	$2,%hi($LC1)
	addiu	$4,$2,%lo($LC1)
	move	$5,$3
	lui	$2,%hi(panic)
	addiu	$2,$2,%lo(panic)
	jalr	$2
$L117:
	lw	$2,56($sp)
	lw	$3,8($2)
	li	$2,-1			# 0xffffffffffffffff
	bne	$3,$2,$L118
	lw	$2,56($sp)
	lw	$5,0($2)
	lw	$2,56($sp)
	lw	$3,4($2)
	lui	$2,%hi($LC2)
	addiu	$4,$2,%lo($LC2)
	move	$6,$3
	lui	$2,%hi(panic)
	addiu	$2,$2,%lo(panic)
	jalr	$2
$L118:
	lw	$2,56($sp)
	lw	$2,8($2)
	sw	$2,76($sp)
	li	$2,32776			# 0x8008
	sw	$2,60($sp)
	lw	$2,60($sp)
	sw	$2,64($sp)
	sw	$0,68($sp)
	sw	$0,84($sp)
	sw	$0,88($sp)
	sw	$0,92($sp)
	sw	$0,96($sp)
	sw	$0,84($sp)
	lw	$2,64($sp)
	sw	$2,88($sp)
	lw	$2,68($sp)
	sw	$2,96($sp)
	addiu	$2,$sp,84
	sw	$2,72($sp)
	li	$4,25			# 0x19
	move	$5,$0
	lw	$6,72($sp)
	lui	$2,%hi(__ECNT_HOOK)
	addiu	$2,$2,%lo(__ECNT_HOOK)
	jalr	$2
	move	$3,$2
	li	$2,-1			# 0xffffffffffffffff
	bne	$3,$2,$L122
	lw	$2,72($sp)
	lw	$3,4($2)
	lui	$2,%hi($LC1)
	addiu	$4,$2,%lo($LC1)
	move	$5,$3
	lui	$2,%hi(panic)
	addiu	$2,$2,%lo(panic)
	jalr	$2
$L122:
	lw	$2,72($sp)
	lw	$3,8($2)
	li	$2,-1			# 0xffffffffffffffff
	bne	$3,$2,$L123
	lw	$2,72($sp)
	lw	$5,0($2)
	lw	$2,72($sp)
	lw	$3,4($2)
	lui	$2,%hi($LC2)
	addiu	$4,$2,%lo($LC2)
	move	$6,$3
	lui	$2,%hi(panic)
	addiu	$2,$2,%lo(panic)
	jalr	$2
$L123:
	lw	$2,72($sp)
	lw	$2,8($2)
	sw	$2,80($sp)
	lw	$2,128($sp)
	sw	$2,20($sp)
	addiu	$2,$sp,76
	sw	$2,24($sp)
	li	$2,8			# 0x8
	sw	$2,16($sp)
	lw	$2,20($sp)
	sw	$2,28($sp)
	lw	$2,16($sp)
	sw	$2,32($sp)
	move	$2,$28
	lw	$2,24($2)
	sw	$2,36($sp)
	lw	$3,28($sp)
	lw	$2,32($sp)
	addu	$3,$3,$2
	lw	$2,28($sp)
	or	$3,$3,$2
	lw	$2,32($sp)
	or	$3,$3,$2
	lw	$2,36($sp)
	and	$2,$3,$2
	sw	$2,40($sp)
	lw	$2,40($sp)
	sltu	$2,$2,1
	andi	$2,$2,0x00ff
	sltu	$2,$0,$2
	andi	$2,$2,0x00ff
	beq	$2,$0,$L127
	lw	$4,20($sp)
	lw	$5,24($sp)
	lw	$6,16($sp)
#APP
 # 380 "/opt/tclinux_phoenix/modules/private/xpon/src/xmcs/xmcs_phy.c" 1
	.set	noat
	la	$1, __copy_user
	jalr	$1
	.set	at
	
 # 0 "" 2
#NO_APP
	move	$2,$6
	sw	$2,16($sp)
$L127:
	lw	$2,16($sp)
	beq	$2,$0,$L128
	li	$2,-14			# 0xfffffffffffffff2
	j	$L130
$L128:
	move	$2,$0
$L130:
	lw	$31,124($sp)
	addiu	$sp,$sp,128
	j	$31
	.end	xmcs_get_phy_fec_status
	.size	xmcs_get_phy_fec_status, .-xmcs_get_phy_fec_status
	.rdata
	.align	2
$LC3:
	.ascii	"[%lu0ms]No such I/O command, cmd: %x\012\000"
	.section	.text.phy_cmd_proc,"ax",@progbits
	.align	2
	.globl	phy_cmd_proc
	.set	nomips16
	.set	nomicromips
	.ent	phy_cmd_proc
	.type	phy_cmd_proc, @function
phy_cmd_proc:
	.frame	$sp,32,$31		# vars= 8, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-32
	sw	$31,28($sp)
	sw	$4,32($sp)
	sw	$5,36($sp)
	lw	$2,32($sp)
	li	$3,1074003968			# 0x40040000
	ori	$3,$3,0xd706
	beq	$2,$3,$L133
	nop

	li	$3,1074003968			# 0x40040000
	ori	$3,$3,0xd707
	sltu	$3,$2,$3
	beq	$3,$0,$L134
	nop

	li	$3,1074003968			# 0x40040000
	ori	$3,$3,0xd703
	beq	$2,$3,$L135
	nop

	li	$3,1074003968			# 0x40040000
	ori	$3,$3,0xd704
	sltu	$3,$2,$3
	beq	$3,$0,$L136
	nop

	li	$3,1074003968			# 0x40040000
	ori	$3,$3,0xd701
	beq	$2,$3,$L137
	nop

	li	$3,1074003968			# 0x40040000
	ori	$3,$3,0xd702
	beq	$2,$3,$L138
	nop

	j	$L132
	nop

$L136:
	li	$3,1074003968			# 0x40040000
	ori	$3,$3,0xd704
	beq	$2,$3,$L139
	nop

	li	$3,1074003968			# 0x40040000
	ori	$3,$3,0xd705
	beq	$2,$3,$L140
	nop

	j	$L132
	nop

$L134:
	li	$3,-2147221504			# 0xffffffff80040000
	ori	$3,$3,0xd702
	beq	$2,$3,$L141
	nop

	li	$3,-2147221504			# 0xffffffff80040000
	ori	$3,$3,0xd703
	sltu	$3,$2,$3
	beq	$3,$0,$L142
	nop

	li	$3,1074003968			# 0x40040000
	ori	$3,$3,0xd707
	beq	$2,$3,$L143
	nop

	li	$3,-2147221504			# 0xffffffff80040000
	ori	$3,$3,0xd701
	beq	$2,$3,$L144
	nop

	j	$L132
	nop

$L142:
	li	$3,-2147221504			# 0xffffffff80040000
	ori	$3,$3,0xd704
	beq	$2,$3,$L145
	nop

	li	$3,-2147221504			# 0xffffffff80040000
	ori	$3,$3,0xd704
	sltu	$3,$2,$3
	bne	$3,$0,$L146
	nop

	li	$3,-2147221504			# 0xffffffff80040000
	ori	$3,$3,0xd706
	beq	$2,$3,$L147
	nop

	j	$L132
	nop

$L144:
	lw	$2,36($sp)
	move	$4,$2
	lui	$2,%hi(xmcs_set_fec_mode)
	addiu	$2,$2,%lo(xmcs_set_fec_mode)
	jalr	$2
	nop

	sw	$2,16($sp)
	j	$L148
	nop

$L137:
	lw	$2,36($sp)
	move	$4,$2
	lui	$2,%hi(xmcs_get_fec_status)
	addiu	$2,$2,%lo(xmcs_get_fec_status)
	jalr	$2
	nop

	sw	$2,16($sp)
	j	$L148
	nop

$L141:
	lw	$2,36($sp)
	move	$4,$2
	lui	$2,%hi(xmcs_set_clear_fec_counter)
	addiu	$2,$2,%lo(xmcs_set_clear_fec_counter)
	jalr	$2
	nop

	sw	$2,16($sp)
	j	$L148
	nop

$L138:
	lw	$2,36($sp)
	move	$4,$2
	lui	$2,%hi(xmcs_get_fec_counter)
	addiu	$2,$2,%lo(xmcs_get_fec_counter)
	jalr	$2
	nop

	sw	$2,16($sp)
	j	$L148
	nop

$L146:
	lw	$2,36($sp)
	move	$4,$2
	lui	$2,%hi(xmcs_set_clear_frame_counter)
	addiu	$2,$2,%lo(xmcs_set_clear_frame_counter)
	jalr	$2
	nop

	sw	$2,16($sp)
	j	$L148
	nop

$L135:
	lw	$2,36($sp)
	move	$4,$2
	lui	$2,%hi(xmcs_get_frame_counter)
	addiu	$2,$2,%lo(xmcs_get_frame_counter)
	jalr	$2
	nop

	sw	$2,16($sp)
	j	$L148
	nop

$L145:
	lw	$2,36($sp)
	move	$4,$2
	lui	$2,%hi(xmcs_set_transceiver_config)
	addiu	$2,$2,%lo(xmcs_set_transceiver_config)
	jalr	$2
	nop

	sw	$2,16($sp)
	j	$L148
	nop

$L139:
	lw	$2,36($sp)
	move	$4,$2
	lui	$2,%hi(xmcs_get_transceiver_config)
	addiu	$2,$2,%lo(xmcs_get_transceiver_config)
	jalr	$2
	nop

	sw	$2,16($sp)
	j	$L148
	nop

$L140:
	lw	$2,36($sp)
	move	$4,$2
	lui	$2,%hi(xmcs_get_transceiver_params)
	addiu	$2,$2,%lo(xmcs_get_transceiver_params)
	jalr	$2
	nop

	sw	$2,16($sp)
	j	$L148
	nop

$L147:
	lw	$2,36($sp)
	move	$4,$2
	lui	$2,%hi(xmcs_set_burst_config)
	addiu	$2,$2,%lo(xmcs_set_burst_config)
	jalr	$2
	nop

	sw	$2,16($sp)
	j	$L148
	nop

$L133:
	lw	$2,36($sp)
	move	$4,$2
	lui	$2,%hi(xmcs_get_burst_config)
	addiu	$2,$2,%lo(xmcs_get_burst_config)
	jalr	$2
	nop

	sw	$2,16($sp)
	j	$L148
	nop

$L143:
	lw	$2,36($sp)
	move	$4,$2
	lui	$2,%hi(xmcs_get_phy_fec_status)
	addiu	$2,$2,%lo(xmcs_get_phy_fec_status)
	jalr	$2
	nop

	sw	$2,16($sp)
	j	$L148
	nop

$L132:
	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	lhu	$2,-30816($2)
	andi	$2,$2,0x1
	beq	$2,$0,$L149
	nop

	lui	$2,%hi(jiffies)
	lw	$3,%lo(jiffies)($2)
	lui	$2,%hi($LC3)
	addiu	$4,$2,%lo($LC3)
	move	$5,$3
	lw	$6,32($sp)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	j	$L151
	nop

$L149:
$L151:
	nop
$L148:
	lw	$2,16($sp)
	lw	$31,28($sp)
	addiu	$sp,$sp,32
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	phy_cmd_proc
	.size	phy_cmd_proc, .-phy_cmd_proc
	.ident	"GCC: (Buildroot 2015.08.1) 4.9.3"
