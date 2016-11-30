	.file	1 "gpon.c"
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
	.globl	gEmergenceRecord
	.section	.bss,"aw",@nobits
	.align	3
	.type	gEmergenceRecord, @object
	.size	gEmergenceRecord, 160
gEmergenceRecord:
	.space	160
	.globl	gEmerStateindex
	.align	2
	.type	gEmerStateindex, @object
	.size	gEmerStateindex, 4
gEmerStateindex:
	.space	4
	.section	.text.prepare_gpon,"ax",@progbits
	.align	2
	.globl	prepare_gpon
	.set	nomips16
	.set	nomicromips
	.ent	prepare_gpon
	.type	prepare_gpon, @function
prepare_gpon:
	.frame	$sp,24,$31		# vars= 0, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-24
	sw	$31,20($sp)
	li	$4,1			# 0x1
	lui	$2,%hi(xpon_set_qdma_qos)
	addiu	$2,$2,%lo(xpon_set_qdma_qos)
	jalr	$2
	nop

	li	$4,1			# 0x1
	lui	$2,%hi(select_xpon_wan)
	addiu	$2,$2,%lo(select_xpon_wan)
	jalr	$2
	nop

	li	$4,1			# 0x1
	lui	$2,%hi(XPON_PHY_SET_MODE)
	addiu	$2,$2,%lo(XPON_PHY_SET_MODE)
	jalr	$2
	nop

	move	$4,$0
	lui	$2,%hi(gpon_disable_with_option)
	addiu	$2,$2,%lo(gpon_disable_with_option)
	jalr	$2
	nop

	lw	$31,20($sp)
	addiu	$sp,$sp,24
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	prepare_gpon
	.size	prepare_gpon, .-prepare_gpon
	.rdata
	.align	2
$LC0:
	.ascii	"%s:%d #######About to do GPON MAC and XPON PHY Reset!###"
	.ascii	"####\012\000"
	.section	.text.schedule_gpon_dev_reset,"ax",@progbits
	.align	2
	.globl	schedule_gpon_dev_reset
	.set	nomips16
	.set	nomicromips
	.ent	schedule_gpon_dev_reset
	.type	schedule_gpon_dev_reset, @function
schedule_gpon_dev_reset:
	.frame	$sp,40,$31		# vars= 16, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-40
	sw	$31,36($sp)
	sw	$4,40($sp)
	lui	$2,%hi(gpPhyData)
	lw	$2,%lo(gpPhyData)($2)
	lw	$3,60($2)
	li	$2,14680064			# 0xe00000
	and	$3,$3,$2
	li	$2,2097152			# 0x200000
	bne	$3,$2,$L4
	nop

	li	$4,2			# 0x2
	lui	$2,%hi(gpon_act_change_gpon_state)
	addiu	$2,$2,%lo(gpon_act_change_gpon_state)
	jalr	$2
	nop

	j	$L5
	nop

$L4:
	li	$4,1			# 0x1
	lui	$2,%hi(gpon_act_change_gpon_state)
	addiu	$2,$2,%lo(gpon_act_change_gpon_state)
	jalr	$2
	nop

$L5:
	lui	$2,%hi(xpon_mac_print_open)
	lw	$2,%lo(xpon_mac_print_open)($2)
	beq	$2,$0,$L6
	nop

	lui	$2,%hi($LC0)
	addiu	$4,$2,%lo($LC0)
	lui	$2,%hi(__func__.32912)
	addiu	$5,$2,%lo(__func__.32912)
	li	$6,56			# 0x38
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L6:
	li	$2,1			# 0x1
	sw	$2,16($sp)
	li	$2,1			# 0x1
	sw	$2,20($sp)
	addiu	$2,$sp,16
	move	$4,$2
	lui	$2,%hi(xpon_daemon_job_enqueue)
	addiu	$2,$2,%lo(xpon_daemon_job_enqueue)
	jalr	$2
	nop

	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,34832			# 0x8810
	addu	$2,$3,$2
	move	$4,$2
	li	$5,1			# 0x1
	li	$6,1			# 0x1
	move	$7,$0
	lui	$2,%hi(__wake_up)
	addiu	$2,$2,%lo(__wake_up)
	jalr	$2
	nop

	lw	$31,36($sp)
	addiu	$sp,$sp,40
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	schedule_gpon_dev_reset
	.size	schedule_gpon_dev_reset, .-schedule_gpon_dev_reset
	.rdata
	.align	2
$LC1:
	.ascii	"[%lu0ms]GPON QoS deinitialization failed.\012\000"
	.section	.text.gpon_disable_with_option,"ax",@progbits
	.align	2
	.globl	gpon_disable_with_option
	.set	nomips16
	.set	nomicromips
	.ent	gpon_disable_with_option
	.type	gpon_disable_with_option, @function
gpon_disable_with_option:
	.frame	$sp,48,$31		# vars= 24, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-48
	sw	$31,44($sp)
	sw	$4,48($sp)
	sw	$0,16($sp)
	lui	$2,%hi(unregister_gpon_isr)
	addiu	$2,$2,%lo(unregister_gpon_isr)
	jalr	$2
	nop

	lui	$2,%hi(gpon_qos_deinit)
	addiu	$2,$2,%lo(gpon_qos_deinit)
	jalr	$2
	nop

	sw	$2,16($sp)
	lw	$2,16($sp)
	beq	$2,$0,$L8
	nop

	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	lhu	$2,-30816($2)
	andi	$2,$2,0x1
	beq	$2,$0,$L9
	nop

	lui	$2,%hi(jiffies)
	lw	$3,%lo(jiffies)($2)
	lui	$2,%hi($LC1)
	addiu	$4,$2,%lo($LC1)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L9:
	lw	$2,16($sp)
	j	$L12
	nop

$L8:
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	li	$3,-1			# 0xffffffffffffffff
	sb	$3,204($2)
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	li	$3,-1			# 0xffffffffffffffff
	sh	$3,252($2)
	lui	$2,%hi(gpGponPriv)
	lw	$3,%lo(gpGponPriv)($2)
	lhu	$2,448($3)
	ins	$2,$0,15,1
	sh	$2,448($3)
	lui	$2,%hi(gpGponPriv)
	lw	$3,%lo(gpGponPriv)($2)
	lbu	$2,464($3)
	ins	$2,$0,7,1
	sb	$2,464($3)
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16388
	sw	$2,20($sp)
	lw	$2,20($sp)
	lw	$2,0($2)
	sw	$2,32($sp)
	lw	$2,32($sp)
	ins	$2,$0,16,1
	sw	$2,32($sp)
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16388
	move	$3,$2
	lw	$2,32($sp)
	sw	$3,24($sp)
	sw	$2,28($sp)
	lw	$2,24($sp)
	lw	$3,28($sp)
	sw	$3,0($2)
	lui	$2,%hi(gwan_remove_all_tcont)
	addiu	$2,$2,%lo(gwan_remove_all_tcont)
	jalr	$2
	nop

	lui	$2,%hi(gwan_remove_all_gemport_for_disable)
	addiu	$2,$2,%lo(gwan_remove_all_gemport_for_disable)
	jalr	$2
	nop

	lw	$4,48($sp)
	lui	$2,%hi(schedule_gpon_dev_reset)
	addiu	$2,$2,%lo(schedule_gpon_dev_reset)
	jalr	$2
	nop

	move	$2,$0
$L12:
	lw	$31,44($sp)
	addiu	$sp,$sp,48
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	gpon_disable_with_option
	.size	gpon_disable_with_option, .-gpon_disable_with_option
	.rdata
	.align	2
$LC2:
	.ascii	"[%lu0ms]GPON device initialization failed.\012\000"
	.align	2
$LC3:
	.ascii	"[%lu0ms]GPON QoS initialization failed.\012\000"
	.align	2
$LC4:
	.ascii	"[%lu0ms]Register GPON ISR handler function failed.\012\000"
	.section	.text.gpon_enable,"ax",@progbits
	.align	2
	.globl	gpon_enable
	.set	nomips16
	.set	nomicromips
	.ent	gpon_enable
	.type	gpon_enable, @function
gpon_enable:
	.frame	$sp,32,$31		# vars= 8, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-32
	sw	$31,28($sp)
	sw	$0,16($sp)
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lw	$3,4($2)
	li	$2,1			# 0x1
	beq	$3,$2,$L14
	nop

	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lw	$3,4($2)
	li	$2,2			# 0x2
	beq	$3,$2,$L14
	nop

	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lw	$3,4($2)
	li	$2,7			# 0x7
	beq	$3,$2,$L14
	nop

	li	$2,-14			# 0xfffffffffffffff2
	j	$L15
	nop

$L14:
	lui	$2,%hi(gpon_dev_init)
	addiu	$2,$2,%lo(gpon_dev_init)
	jalr	$2
	nop

	sw	$2,16($sp)
	lw	$2,16($sp)
	beq	$2,$0,$L16
	nop

	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	lhu	$2,-30816($2)
	andi	$2,$2,0x1
	beq	$2,$0,$L17
	nop

	lui	$2,%hi(jiffies)
	lw	$3,%lo(jiffies)($2)
	lui	$2,%hi($LC2)
	addiu	$4,$2,%lo($LC2)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L17:
	lw	$2,16($sp)
	j	$L15
	nop

$L16:
	lui	$2,%hi(gpon_qos_init)
	addiu	$2,$2,%lo(gpon_qos_init)
	jalr	$2
	nop

	sw	$2,16($sp)
	lw	$2,16($sp)
	beq	$2,$0,$L18
	nop

	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	lhu	$2,-30816($2)
	andi	$2,$2,0x1
	beq	$2,$0,$L19
	nop

	lui	$2,%hi(jiffies)
	lw	$3,%lo(jiffies)($2)
	lui	$2,%hi($LC3)
	addiu	$4,$2,%lo($LC3)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L19:
	lw	$2,16($sp)
	j	$L15
	nop

$L18:
	lui	$2,%hi(register_gpon_isr)
	addiu	$2,$2,%lo(register_gpon_isr)
	jalr	$2
	nop

	sw	$2,16($sp)
	lw	$2,16($sp)
	beq	$2,$0,$L20
	nop

	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	lhu	$2,-30816($2)
	andi	$2,$2,0x1
	beq	$2,$0,$L21
	nop

	lui	$2,%hi(jiffies)
	lw	$3,%lo(jiffies)($2)
	lui	$2,%hi($LC4)
	addiu	$4,$2,%lo($LC4)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L21:
	lw	$2,16($sp)
	j	$L15
	nop

$L20:
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	addiu	$2,$2,205
	move	$4,$2
	lui	$2,%hi(gponDevSetSerialNumber)
	addiu	$2,$2,%lo(gponDevSetSerialNumber)
	jalr	$2
	nop

	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	addiu	$2,$2,227
	move	$4,$2
	lui	$2,%hi(gponDevSetEncryptKey)
	addiu	$2,$2,%lo(gponDevSetEncryptKey)
	jalr	$2
	nop

	move	$2,$0
$L15:
	lw	$31,28($sp)
	addiu	$sp,$sp,32
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	gpon_enable
	.size	gpon_enable, .-gpon_enable
	.rdata
	.align	2
$LC5:
	.ascii	"[%lu0ms]Adjust EqD: first psync_to_sof_delay 0x%x\012\000"
	.align	2
$LC6:
	.ascii	"[%lu0ms]Adjust EqD: psync_to_sof_delay_diff d'%d\012\000"
	.align	2
$LC7:
	.ascii	"[%lu0ms]Adjust EqD: internal delay fine tune 0x%x\012\000"
	.section	.text.adjust_mac_internal_delay_fine_tune,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	adjust_mac_internal_delay_fine_tune
	.type	adjust_mac_internal_delay_fine_tune, @function
adjust_mac_internal_delay_fine_tune:
	.frame	$sp,64,$31		# vars= 40, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-64
	sw	$31,60($sp)
	li	$2,10			# 0xa
	sw	$2,32($sp)
	sw	$0,28($sp)
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lw	$3,308($2)
	li	$2,-1			# 0xffffffffffffffff
	beq	$3,$2,$L22
	nop

	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	lhu	$2,-30816($2)
	andi	$2,$2,0x100
	beq	$2,$0,$L24
	nop

	lui	$2,%hi(jiffies)
	lw	$5,%lo(jiffies)($2)
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lw	$3,308($2)
	lui	$2,%hi($LC5)
	addiu	$4,$2,%lo($LC5)
	move	$6,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L24:
	sw	$0,24($sp)
	j	$L25
	nop

$L32:
	li	$4,1000			# 0x3e8
	lui	$2,%hi(__udelay)
	addiu	$2,$2,%lo(__udelay)
	jalr	$2
	nop

	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$2,$2,0x2d4
	sw	$2,48($sp)
	lw	$2,48($sp)
	lw	$2,0($2)
	andi	$2,$2,0xffff
	sw	$2,16($sp)
	lw	$2,24($sp)
	bne	$2,$0,$L27
	nop

	lw	$2,16($sp)
	sw	$2,20($sp)
	j	$L28
	nop

$L27:
	lw	$3,20($sp)
	lw	$2,16($sp)
	bne	$3,$2,$L29
	nop

	lw	$2,28($sp)
	addiu	$2,$2,1
	sw	$2,28($sp)
	j	$L28
	nop

$L29:
	lw	$2,16($sp)
	sw	$2,20($sp)
	sw	$0,28($sp)
$L28:
	lw	$3,28($sp)
	li	$2,3			# 0x3
	bne	$3,$2,$L30
	nop

	j	$L31
	nop

$L30:
	lw	$2,24($sp)
	addiu	$2,$2,1
	sw	$2,24($sp)
$L25:
	lw	$3,24($sp)
	lw	$2,32($sp)
	slt	$2,$3,$2
	bne	$2,$0,$L32
	nop

$L31:
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lw	$2,308($2)
	lw	$3,16($sp)
	subu	$2,$3,$2
	sw	$2,36($sp)
	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	lhu	$2,-30816($2)
	andi	$2,$2,0x100
	beq	$2,$0,$L33
	nop

	lui	$2,%hi(jiffies)
	lw	$3,%lo(jiffies)($2)
	lui	$2,%hi($LC6)
	addiu	$4,$2,%lo($LC6)
	move	$5,$3
	lw	$6,36($sp)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L33:
	lw	$2,36($sp)
	sw	$2,40($sp)
	lw	$2,40($sp)
	bgez	$2,$L34
	nop

	subu	$2,$0,$2
$L34:
	slt	$2,$2,16
	beq	$2,$0,$L22
	nop

	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lbu	$2,294($2)
	move	$4,$2
	lw	$2,36($sp)
	srl	$3,$2,31
	addu	$2,$3,$2
	sra	$2,$2,1
	addu	$2,$4,$2
	sw	$2,44($sp)
	lw	$2,44($sp)
	andi	$2,$2,0x00ff
	move	$4,$2
	lui	$2,%hi(gponDevSetInternalDelayFineTune)
	addiu	$2,$2,%lo(gponDevSetInternalDelayFineTune)
	jalr	$2
	nop

	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	lhu	$2,-30816($2)
	andi	$2,$2,0x100
	beq	$2,$0,$L22
	nop

	lui	$2,%hi(jiffies)
	lw	$3,%lo(jiffies)($2)
	lui	$2,%hi($LC7)
	addiu	$4,$2,%lo($LC7)
	move	$5,$3
	lw	$6,44($sp)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L22:
	lw	$31,60($sp)
	addiu	$sp,$sp,64
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	adjust_mac_internal_delay_fine_tune
	.size	adjust_mac_internal_delay_fine_tune, .-adjust_mac_internal_delay_fine_tune
	.rdata
	.align	2
$LC8:
	.ascii	"[%lu0ms]GPON IRQ: gpon detect phy ready.\012\000"
	.align	2
$LC9:
	.ascii	"ECNT_HOOK_ERROR occur with cmd_id:0x%x\012\000"
	.align	2
$LC10:
	.ascii	"No such API with type:%d, cmd_id:0x%x\012\000"
	.section	.text.gpon_detect_phy_ready,"ax",@progbits
	.align	2
	.globl	gpon_detect_phy_ready
	.set	nomips16
	.set	nomicromips
	.ent	gpon_detect_phy_ready
	.type	gpon_detect_phy_ready, @function
gpon_detect_phy_ready:
	.frame	$sp,64,$31		# vars= 40, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-64
	sw	$31,60($sp)
	li	$2,-1078984704			# 0xffffffffbfb00000
	ori	$2,$2,0x64
	lw	$3,0($2)
	li	$2,-65536			# 0xffffffffffff0000
	and	$3,$3,$2
	li	$2,524288			# 0x80000
	bne	$3,$2,$L36
	nop

	li	$2,-1078984704			# 0xffffffffbfb00000
	ori	$2,$2,0x9c
	lw	$2,0($2)
	andi	$2,$2,0x1
	sltu	$2,$0,$2
	andi	$2,$2,0x00ff
	j	$L37
	nop

$L36:
	li	$2,-1078984704			# 0xffffffffbfb00000
	ori	$2,$2,0x64
	lw	$3,0($2)
	li	$2,-65536			# 0xffffffffffff0000
	and	$3,$3,$2
	li	$2,458752			# 0x70000
	beq	$3,$2,$L38
	nop

	li	$2,-1078984704			# 0xffffffffbfb00000
	ori	$2,$2,0x64
	lw	$3,0($2)
	li	$2,-65536			# 0xffffffffffff0000
	and	$3,$3,$2
	li	$2,524288			# 0x80000
	bne	$3,$2,$L39
	nop

$L38:
	li	$2,-1078984704			# 0xffffffffbfb00000
	ori	$2,$2,0x8c
	sw	$2,16($sp)
	lw	$2,16($sp)
	lw	$3,0($2)
	li	$2,536870912			# 0x20000000
	and	$2,$3,$2
	sltu	$2,$0,$2
	andi	$2,$2,0x00ff
	j	$L37
	nop

$L39:
	li	$2,-1078984704			# 0xffffffffbfb00000
	ori	$2,$2,0x8c
	sw	$2,20($sp)
	lw	$2,20($sp)
	lw	$2,0($2)
	nor	$2,$0,$2
	srl	$2,$2,31
	andi	$2,$2,0x00ff
$L37:
	beq	$2,$0,$L43
	nop

	lui	$2,%hi(adjust_mac_internal_delay_fine_tune)
	addiu	$2,$2,%lo(adjust_mac_internal_delay_fine_tune)
	jalr	$2
	nop

$L43:
	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	lhu	$2,-30816($2)
	andi	$2,$2,0x4
	beq	$2,$0,$L44
	nop

	lui	$2,%hi(jiffies)
	lw	$3,%lo(jiffies)($2)
	lui	$2,%hi($LC8)
	addiu	$4,$2,%lo($LC8)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L44:
	lui	$2,%hi(gpPonSysData)
	lw	$2,%lo(gpPonSysData)($2)
	lw	$3,4($2)
	li	$2,2			# 0x2
	bne	$3,$2,$L45
	nop

	li	$4,2			# 0x2
	li	$5,5			# 0x5
	move	$6,$0
	lui	$2,%hi(xmcs_report_event)
	addiu	$2,$2,%lo(xmcs_report_event)
	jalr	$2
	nop

$L45:
	li	$2,32797			# 0x801d
	sw	$2,24($sp)
	lw	$2,24($sp)
	sw	$2,28($sp)
	sw	$0,32($sp)
	sw	$0,40($sp)
	sw	$0,44($sp)
	sw	$0,48($sp)
	sw	$0,52($sp)
	sw	$0,40($sp)
	lw	$2,28($sp)
	sw	$2,44($sp)
	lw	$2,32($sp)
	sw	$2,52($sp)
	addiu	$2,$sp,40
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
	bne	$3,$2,$L46
	nop

	lw	$2,36($sp)
	lw	$3,4($2)
	lui	$2,%hi($LC9)
	addiu	$4,$2,%lo($LC9)
	move	$5,$3
	lui	$2,%hi(panic)
	addiu	$2,$2,%lo(panic)
	jalr	$2
	nop

$L46:
	lw	$2,36($sp)
	lw	$3,8($2)
	li	$2,-1			# 0xffffffffffffffff
	bne	$3,$2,$L47
	nop

	lw	$2,36($sp)
	lw	$5,0($2)
	lw	$2,36($sp)
	lw	$3,4($2)
	lui	$2,%hi($LC10)
	addiu	$4,$2,%lo($LC10)
	move	$6,$3
	lui	$2,%hi(panic)
	addiu	$2,$2,%lo(panic)
	jalr	$2
	nop

$L47:
	lw	$2,36($sp)
	lw	$3,8($2)
	li	$2,1			# 0x1
	bne	$3,$2,$L51
	nop

	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	lw	$2,-30416($2)
	bne	$2,$0,$L51
	nop

	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lw	$3,4($2)
	li	$2,7			# 0x7
	beq	$3,$2,$L52
	nop

	move	$4,$0
	lui	$2,%hi(change_alarm_led_status)
	addiu	$2,$2,%lo(change_alarm_led_status)
	jalr	$2
	nop

	j	$L51
	nop

$L52:
	li	$4,1			# 0x1
	lui	$2,%hi(change_alarm_led_status)
	addiu	$2,$2,%lo(change_alarm_led_status)
	jalr	$2
	nop

$L51:
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lw	$3,4($2)
	li	$2,1			# 0x1
	bne	$3,$2,$L53
	nop

	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lw	$3,464($2)
	li	$2,536870912			# 0x20000000
	and	$2,$3,$2
	beq	$2,$0,$L54
	nop

	li	$4,7			# 0x7
	lui	$2,%hi(gpon_act_change_gpon_state)
	addiu	$2,$2,%lo(gpon_act_change_gpon_state)
	jalr	$2
	nop

	li	$4,2			# 0x2
	lui	$2,%hi(gpon_record_emergence_info)
	addiu	$2,$2,%lo(gpon_record_emergence_info)
	jalr	$2
	nop

	j	$L53
	nop

$L54:
	li	$4,2			# 0x2
	lui	$2,%hi(gpon_act_change_gpon_state)
	addiu	$2,$2,%lo(gpon_act_change_gpon_state)
	jalr	$2
	nop

$L53:
	lui	$2,%hi(gpPonSysData)
	lw	$2,%lo(gpPonSysData)($2)
	lw	$2,8($2)
	bne	$2,$0,$L55
	nop

	lui	$2,%hi(gpPonSysData)
	lw	$2,%lo(gpPonSysData)($2)
	li	$3,1			# 0x1
	sw	$3,0($2)
	lui	$2,%hi(gpon_enable)
	addiu	$2,$2,%lo(gpon_enable)
	jalr	$2
	nop

$L55:
	li	$4,1			# 0x1
	li	$5,1			# 0x1
	move	$6,$0
	lui	$2,%hi(xmcs_report_event)
	addiu	$2,$2,%lo(xmcs_report_event)
	jalr	$2
	nop

	lw	$31,60($sp)
	addiu	$sp,$sp,64
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	gpon_detect_phy_ready
	.size	gpon_detect_phy_ready, .-gpon_detect_phy_ready
	.section	.text.disable_sniffer_mode,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	disable_sniffer_mode
	.type	disable_sniffer_mode, @function
disable_sniffer_mode:
	.frame	$sp,24,$31		# vars= 0, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-24
	sw	$31,20($sp)
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	addiu	$2,$2,312
	move	$4,$2
	lui	$2,%hi(gponDevGetSniffMode)
	addiu	$2,$2,%lo(gponDevGetSniffMode)
	jalr	$2
	nop

	move	$4,$0
	lui	$2,%hi(gponDevEnableSniffMode)
	addiu	$2,$2,%lo(gponDevEnableSniffMode)
	jalr	$2
	nop

	lw	$31,20($sp)
	addiu	$sp,$sp,24
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	disable_sniffer_mode
	.size	disable_sniffer_mode, .-disable_sniffer_mode
	.rdata
	.align	2
$LC11:
	.ascii	"[%lu0ms]GPON IRQ: gpon detect los lof.\012\000"
	.align	2
$LC12:
	.ascii	"[%lu0ms]one bit err cnt:0x%08x\012\000"
	.align	2
$LC13:
	.ascii	"[%lu0ms]two bits err cnt:0x%08x\012\000"
	.align	2
$LC14:
	.ascii	"[%lu0ms]uncorrectable err cnt:0x%08x\012\000"
	.section	.text.gpon_detect_los_lof,"ax",@progbits
	.align	2
	.globl	gpon_detect_los_lof
	.set	nomips16
	.set	nomicromips
	.ent	gpon_detect_los_lof
	.type	gpon_detect_los_lof, @function
gpon_detect_los_lof:
	.frame	$sp,40,$31		# vars= 16, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-40
	sw	$31,36($sp)
	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	lhu	$2,-30816($2)
	andi	$2,$2,0x4
	beq	$2,$0,$L58
	nop

	lui	$2,%hi(jiffies)
	lw	$3,%lo(jiffies)($2)
	lui	$2,%hi($LC11)
	addiu	$4,$2,%lo($LC11)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L58:
	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	lhu	$2,-30816($2)
	andi	$2,$2,0x1
	beq	$2,$0,$L59
	nop

	lui	$2,%hi(jiffies)
	lw	$5,%lo(jiffies)($2)
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,17200
	sw	$2,16($sp)
	lw	$2,16($sp)
	lw	$3,0($2)
	lui	$2,%hi($LC12)
	addiu	$4,$2,%lo($LC12)
	move	$6,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L59:
	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	lhu	$2,-30816($2)
	andi	$2,$2,0x1
	beq	$2,$0,$L61
	nop

	lui	$2,%hi(jiffies)
	lw	$5,%lo(jiffies)($2)
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,17204
	sw	$2,20($sp)
	lw	$2,20($sp)
	lw	$3,0($2)
	lui	$2,%hi($LC13)
	addiu	$4,$2,%lo($LC13)
	move	$6,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L61:
	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	lhu	$2,-30816($2)
	andi	$2,$2,0x1
	beq	$2,$0,$L63
	nop

	lui	$2,%hi(jiffies)
	lw	$5,%lo(jiffies)($2)
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,17208
	sw	$2,24($sp)
	lw	$2,24($sp)
	lw	$3,0($2)
	lui	$2,%hi($LC14)
	addiu	$4,$2,%lo($LC14)
	move	$6,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L63:
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lw	$3,4($2)
	li	$2,7			# 0x7
	bne	$3,$2,$L65
	nop

	li	$4,1			# 0x1
	lui	$2,%hi(change_alarm_led_status)
	addiu	$2,$2,%lo(change_alarm_led_status)
	jalr	$2
	nop

$L65:
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lw	$3,4($2)
	li	$2,2			# 0x2
	beq	$3,$2,$L66
	nop

	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lw	$3,4($2)
	li	$2,3			# 0x3
	beq	$3,$2,$L66
	nop

	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lw	$3,4($2)
	li	$2,4			# 0x4
	bne	$3,$2,$L67
	nop

$L66:
	lui	$2,%hi(gpPonSysData)
	lw	$2,%lo(gpPonSysData)($2)
	sw	$0,0($2)
	lui	$2,%hi(gpPhyData)
	lw	$3,%lo(gpPhyData)($2)
	lhu	$2,60($3)
	ins	$2,$0,5,3
	sh	$2,60($3)
	li	$4,1			# 0x1
	move	$5,$0
	move	$6,$0
	lui	$2,%hi(xmcs_report_event)
	addiu	$2,$2,%lo(xmcs_report_event)
	jalr	$2
	nop

	move	$4,$0
	lui	$2,%hi(gpon_disable_with_option)
	addiu	$2,$2,%lo(gpon_disable_with_option)
	jalr	$2
	nop

	j	$L69
	nop

$L67:
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lw	$3,4($2)
	li	$2,5			# 0x5
	bne	$3,$2,$L69
	nop

	lui	$2,%hi(is_hwnat_dont_clean)
	li	$3,1			# 0x1
	sw	$3,%lo(is_hwnat_dont_clean)($2)
	lui	$2,%hi(disable_sniffer_mode)
	addiu	$2,$2,%lo(disable_sniffer_mode)
	jalr	$2
	nop

	li	$4,6			# 0x6
	lui	$2,%hi(gpon_act_change_gpon_state)
	addiu	$2,$2,%lo(gpon_act_change_gpon_state)
	jalr	$2
	nop

$L69:
	lui	$2,%hi(gpGponPriv)
	lw	$3,%lo(gpGponPriv)($2)
	lhu	$2,448($3)
	ins	$2,$0,13,1
	sh	$2,448($3)
	lw	$31,36($sp)
	addiu	$sp,$sp,40
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	gpon_detect_los_lof
	.size	gpon_detect_los_lof, .-gpon_detect_los_lof
	.section	.text.gpon_detect_dying_gasp,"ax",@progbits
	.align	2
	.globl	gpon_detect_dying_gasp
	.set	nomips16
	.set	nomicromips
	.ent	gpon_detect_dying_gasp
	.type	gpon_detect_dying_gasp, @function
gpon_detect_dying_gasp:
	.frame	$sp,32,$31		# vars= 8, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-32
	sw	$31,28($sp)
	sw	$0,16($sp)
	j	$L71
	nop

$L72:
	lui	$2,%hi(ploam_send_dying_gasp)
	addiu	$2,$2,%lo(ploam_send_dying_gasp)
	jalr	$2
	nop

	lw	$2,16($sp)
	addiu	$2,$2,1
	sw	$2,16($sp)
$L71:
	lw	$2,16($sp)
	slt	$2,$2,3
	bne	$2,$0,$L72
	nop

	lw	$31,28($sp)
	addiu	$sp,$sp,32
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	gpon_detect_dying_gasp
	.size	gpon_detect_dying_gasp, .-gpon_detect_dying_gasp
	.section	.text.gpon_sn_request_threshold_crossed,"ax",@progbits
	.align	2
	.globl	gpon_sn_request_threshold_crossed
	.set	nomips16
	.set	nomicromips
	.ent	gpon_sn_request_threshold_crossed
	.type	gpon_sn_request_threshold_crossed, @function
gpon_sn_request_threshold_crossed:
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	gpon_sn_request_threshold_crossed
	.size	gpon_sn_request_threshold_crossed, .-gpon_sn_request_threshold_crossed
	.section	.text.gpon_ber_interval_expires,"ax",@progbits
	.align	2
	.globl	gpon_ber_interval_expires
	.set	nomips16
	.set	nomicromips
	.ent	gpon_ber_interval_expires
	.type	gpon_ber_interval_expires, @function
gpon_ber_interval_expires:
	.frame	$sp,64,$31		# vars= 40, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-64
	sw	$31,60($sp)
	sw	$4,64($sp)
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lw	$3,4($2)
	li	$2,5			# 0x5
	bne	$3,$2,$L74
	nop

	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	addiu	$4,$2,64
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lw	$2,84($2)
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

	li	$2,32786			# 0x8012
	sw	$2,20($sp)
	lw	$2,20($sp)
	sw	$2,24($sp)
	sw	$0,28($sp)
	sw	$0,36($sp)
	sw	$0,40($sp)
	sw	$0,44($sp)
	sw	$0,48($sp)
	sw	$0,36($sp)
	lw	$2,24($sp)
	sw	$2,40($sp)
	lw	$2,28($sp)
	sw	$2,48($sp)
	addiu	$2,$sp,36
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
	bne	$3,$2,$L76
	nop

	lw	$2,32($sp)
	lw	$3,4($2)
	lui	$2,%hi($LC9)
	addiu	$4,$2,%lo($LC9)
	move	$5,$3
	lui	$2,%hi(panic)
	addiu	$2,$2,%lo(panic)
	jalr	$2
	nop

$L76:
	lw	$2,32($sp)
	lw	$3,8($2)
	li	$2,-1			# 0xffffffffffffffff
	bne	$3,$2,$L77
	nop

	lw	$2,32($sp)
	lw	$5,0($2)
	lw	$2,32($sp)
	lw	$3,4($2)
	lui	$2,%hi($LC10)
	addiu	$4,$2,%lo($LC10)
	move	$6,$3
	lui	$2,%hi(panic)
	addiu	$2,$2,%lo(panic)
	jalr	$2
	nop

$L77:
	lw	$2,32($sp)
	lw	$2,8($2)
	sw	$2,16($sp)
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lui	$3,%hi(gpGponPriv)
	lw	$3,%lo(gpGponPriv)($3)
	lw	$4,0($3)
	lw	$3,16($sp)
	addu	$3,$4,$3
	sw	$3,0($2)
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	addiu	$2,$2,254
	lw	$4,16($sp)
	move	$5,$2
	lui	$2,%hi(ploam_send_rei_msg)
	addiu	$2,$2,%lo(ploam_send_rei_msg)
	jalr	$2
	nop

$L74:
	lw	$31,60($sp)
	addiu	$sp,$sp,64
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	gpon_ber_interval_expires
	.size	gpon_ber_interval_expires, .-gpon_ber_interval_expires
	.rdata
	.align	2
$LC15:
	.ascii	"[%lu0ms]Receive PLOAM message failed.\012\000"
	.align	2
$LC16:
	.ascii	"[%lu0ms]WAN not start, ignore ploam msg:%8X\012\000"
	.align	2
$LC17:
	.ascii	"[%lu0ms]Receive the PLOAM message same:%8X\012\000"
	.section	.text.gpon_recv_ploam_message,"ax",@progbits
	.align	2
	.globl	gpon_recv_ploam_message
	.set	nomips16
	.set	nomicromips
	.ent	gpon_recv_ploam_message
	.type	gpon_recv_ploam_message, @function
gpon_recv_ploam_message:
	.frame	$sp,56,$31		# vars= 32, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-56
	sw	$31,52($sp)
	sw	$0,16($sp)
$L95:
	addiu	$2,$sp,32
	move	$4,$2
	lui	$2,%hi(gponDevGetPloamMsg)
	addiu	$2,$2,%lo(gponDevGetPloamMsg)
	jalr	$2
	nop

	sw	$2,16($sp)
	lw	$2,16($sp)
	bgtz	$2,$L82
	nop

	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	lhu	$2,-30816($2)
	andi	$2,$2,0x1
	beq	$2,$0,$L81
	nop

	lui	$2,%hi(jiffies)
	lw	$3,%lo(jiffies)($2)
	lui	$2,%hi($LC15)
	addiu	$4,$2,%lo($LC15)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	j	$L81
	nop

$L82:
	addiu	$2,$sp,32
	sw	$2,20($sp)
	sw	$0,24($sp)
	lui	$2,%hi(gpPonSysData)
	lw	$2,%lo(gpPonSysData)($2)
	lw	$2,8($2)
	beq	$2,$0,$L85
	nop

	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	lhu	$2,-30816($2)
	andi	$2,$2,0x10
	beq	$2,$0,$L86
	nop

	lui	$2,%hi(jiffies)
	lw	$5,%lo(jiffies)($2)
	lw	$2,20($sp)
	lw	$3,0($2)
	lui	$2,%hi($LC16)
	addiu	$4,$2,%lo($LC16)
	move	$6,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L86:
	li	$2,1			# 0x1
	j	$L87
	nop

$L85:
	lw	$2,20($sp)
	sw	$2,28($sp)
	lw	$2,28($sp)
	lbu	$2,1($2)
	move	$3,$2
	li	$2,4			# 0x4
	bne	$3,$2,$L96
	nop

	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	addiu	$2,$2,192
	move	$4,$2
	lw	$5,28($sp)
	li	$6,7			# 0x7
	lui	$2,%hi(memcmp)
	addiu	$2,$2,%lo(memcmp)
	jalr	$2
	nop

	sltu	$2,$2,1
	andi	$2,$2,0x00ff
	j	$L90
	nop

$L96:
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	addiu	$2,$2,192
	move	$4,$2
	lw	$5,28($sp)
	li	$6,12			# 0xc
	lui	$2,%hi(memcmp)
	addiu	$2,$2,%lo(memcmp)
	jalr	$2
	nop

	sltu	$2,$2,1
	andi	$2,$2,0x00ff
$L90:
	sw	$2,24($sp)
	lw	$2,24($sp)
	beq	$2,$0,$L91
	nop

	lui	$2,%hi(same_cnt.32985)
	lw	$2,%lo(same_cnt.32985)($2)
	addiu	$3,$2,1
	lui	$2,%hi(same_cnt.32985)
	sw	$3,%lo(same_cnt.32985)($2)
$L91:
	lw	$2,24($sp)
	beq	$2,$0,$L92
	nop

	lui	$2,%hi(same_cnt.32985)
	lw	$4,%lo(same_cnt.32985)($2)
	li	$2,1431633920			# 0x55550000
	ori	$2,$2,0x5556
	mult	$4,$2
	mfhi	$3
	sra	$2,$4,31
	subu	$3,$3,$2
	move	$2,$3
	sll	$2,$2,1
	addu	$2,$2,$3
	subu	$3,$4,$2
	beq	$3,$0,$L92
	nop

	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	lhu	$2,-30816($2)
	andi	$2,$2,0x10
	beq	$2,$0,$L93
	nop

	lui	$2,%hi(jiffies)
	lw	$5,%lo(jiffies)($2)
	lw	$2,20($sp)
	lw	$3,0($2)
	lui	$2,%hi($LC17)
	addiu	$4,$2,%lo($LC17)
	move	$6,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L93:
	li	$2,1			# 0x1
	j	$L87
	nop

$L92:
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	addiu	$2,$2,192
	move	$4,$2
	lw	$5,20($sp)
	li	$6,12			# 0xc
	lui	$2,%hi(memcpy)
	addiu	$2,$2,%lo(memcpy)
	jalr	$2
	nop

	lui	$2,%hi(same_cnt.32985)
	sw	$0,%lo(same_cnt.32985)($2)
	move	$2,$0
$L87:
	bne	$2,$0,$L94
	nop

	addiu	$2,$sp,32
	move	$4,$2
	lui	$2,%hi(ploam_parser_down_message)
	addiu	$2,$2,%lo(ploam_parser_down_message)
	jalr	$2
	nop

$L94:
	lw	$2,16($sp)
	slt	$2,$2,4
	beq	$2,$0,$L95
	nop

$L81:
	lw	$31,52($sp)
	addiu	$sp,$sp,56
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	gpon_recv_ploam_message
	.size	gpon_recv_ploam_message, .-gpon_recv_ploam_message
	.rdata
	.align	2
$LC18:
	.ascii	"Update the TOD to switch. sec:%d, nanosec:%d.\012\012\000"
	.section	.text.gpon_update_time_to_switch,"ax",@progbits
	.align	2
	.globl	gpon_update_time_to_switch
	.set	nomips16
	.set	nomicromips
	.ent	gpon_update_time_to_switch
	.type	gpon_update_time_to_switch, @function
gpon_update_time_to_switch:
	.frame	$sp,32,$31		# vars= 8, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-32
	sw	$31,28($sp)
	addiu	$2,$sp,20
	addiu	$3,$sp,16
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(gponDevGetCurrentTod)
	addiu	$2,$2,%lo(gponDevGetCurrentTod)
	jalr	$2
	nop

	lw	$5,16($sp)
	lw	$3,20($sp)
	lui	$2,%hi($LC18)
	addiu	$4,$2,%lo($LC18)
	move	$6,$3
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
	.end	gpon_update_time_to_switch
	.size	gpon_update_time_to_switch, .-gpon_update_time_to_switch
	.rdata
	.align	2
$LC19:
	.ascii	"[%lu0ms]GPON IRQ: send a dying gasp ploam message interr"
	.ascii	"upt.\012\000"
	.align	2
$LC20:
	.ascii	"[%lu0ms]GPON IRQ: AES key switch done interrupt.\012\000"
	.align	2
$LC21:
	.ascii	"[%lu0ms]GPON IRQ: Ranging_Request received interrupt.\012"
	.ascii	"\000"
	.align	2
$LC22:
	.ascii	"[%lu0ms]GPON IRQ: SN_ONU send in O4 state interrupt.\012"
	.ascii	"\000"
	.align	2
$LC23:
	.ascii	"[%lu0ms]GPON IRQ: SN_Request received interrupt.\012\000"
	.align	2
$LC24:
	.ascii	"[%lu0ms]GPON IRQ: SN_ONU sent in O3 state interrupt.\012"
	.ascii	"\000"
	.align	2
$LC25:
	.ascii	"[%lu0ms]GPON IRQ: EqD adjustment done interrupt in O5 st"
	.ascii	"ate interrupt.\012\000"
	.align	2
$LC26:
	.ascii	"[%lu0ms]GPON IRQ: DS fec change interrupt.\012\000"
	.align	2
$LC27:
	.ascii	"[%lu0ms]GPON IRQ: US fec change interrupt.\012\000"
	.align	2
$LC28:
	.ascii	"[%lu0ms]GPON IRQ: LWI interrupt.\012\000"
	.align	2
$LC29:
	.ascii	"[%lu0ms]GPON IRQ: FWI interrupt.\012\000"
	.align	2
$LC30:
	.ascii	"[%lu0ms]GPON IRQ: POP_UP receive in O6 interrupt.\012\000"
	.align	2
$LC31:
	.ascii	"[%lu0ms]GPON IRQ: ToD 1PPS interrupt.\012\000"
	.align	2
$LC32:
	.ascii	"Get the current TOD from GPON MAC. sec:%d, nanosec:%d, i"
	.ascii	"nterval:%d\012\000"
	.align	2
$LC33:
	.ascii	"[%lu0ms]GPON IRQ: ToD updated done interrupt.\012\000"
	.align	2
$LC34:
	.ascii	"[%lu0ms]GPON IRQ: A PLOAM message sent interrupt.\012\000"
	.align	2
$LC35:
	.ascii	"[%lu0ms]GPON IRQ: A PLOAM message received interrupt.\012"
	.ascii	"\000"
	.align	2
$LC36:
	.ascii	"[%lu0ms]GPON IRQ: Loss of GEM delineation interrupt.\012"
	.ascii	"\000"
	.align	2
$LC37:
	.ascii	"[%lu0ms]GPON IRQ: SN_Request_Threshold crossed interrupt"
	.ascii	".\012\000"
	.align	2
$LC38:
	.ascii	"[%lu0ms]GPON IRQ: PHY_RX_EOF signal error interrupt.\012"
	.ascii	"\000"
	.align	2
$LC39:
	.ascii	"[%lu0ms]GPON IRQ: Tx local time late interrupt.\012\000"
	.align	2
$LC40:
	.ascii	"[%lu0ms]GPON IRQ: Burst and single grant buffer mismatch"
	.ascii	" interrupt.\012\000"
	.align	2
$LC41:
	.ascii	"[%lu0ms]GPON IRQ: Superframe counter:%08x\012\000"
	.align	2
$LC42:
	.ascii	"GPON IRQ: FIFO error interrupt.\012\000"
	.align	2
$LC43:
	.ascii	"[%lu0ms]GPON IRQ: OLT assigned stop time error when US F"
	.ascii	"EC on interrupt.\012\000"
	.align	2
$LC44:
	.ascii	"[%lu0ms]GPON IRQ: Rx more than two interleaved GEM fragm"
	.ascii	"ent interrupt.\012\000"
	.align	2
$LC45:
	.ascii	"[%lu0ms]GPON IRQ: bfifo full interrupt.\012\000"
	.align	2
$LC46:
	.ascii	"[%lu0ms]GPON IRQ: sfifo full interrupt.\012\000"
	.align	2
$LC47:
	.ascii	"[%lu0ms]GPON IRQ: Rx error interrupt.\012\000"
	.align	2
$LC48:
	.ascii	"[%lu0ms][4220:%.8x],[4224:%.8x],[4228:%.8x],[422C:%.8x],"
	.ascii	"[4240:%.8x]\012\000"
	.align	2
$LC49:
	.ascii	"[%lu0ms][4300:%.8x],[4304:%.8x],[4308:%.8x],[430C:%.8x],"
	.ascii	"[4310:%.8x]\012\012\000"
	.align	2
$LC50:
	.ascii	"[%lu0ms]GPON IRQ: NULL GPON ISR, Status:%.8x, Mask:%.8x\012"
	.ascii	"\000"
	.section	.text.gpon_isr,"ax",@progbits
	.align	2
	.globl	gpon_isr
	.set	nomips16
	.set	nomicromips
	.ent	gpon_isr
	.type	gpon_isr, @function
gpon_isr:
	.frame	$sp,112,$31		# vars= 72, regs= 1/0, args= 32, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-112
	sw	$31,108($sp)
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16392
	sw	$2,40($sp)
	lw	$2,40($sp)
	lw	$2,0($2)
	sw	$2,80($sp)
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16396
	sw	$2,44($sp)
	lw	$2,44($sp)
	lw	$2,0($2)
	sw	$2,84($sp)
	lw	$3,80($sp)
	lw	$2,84($sp)
	and	$2,$3,$2
	sw	$2,80($sp)
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16392
	move	$3,$2
	lw	$2,80($sp)
	sw	$3,48($sp)
	sw	$2,52($sp)
	lw	$2,48($sp)
	lw	$3,52($sp)
	sw	$3,0($2)
	lw	$3,80($sp)
	lw	$2,84($sp)
	and	$2,$3,$2
	beq	$2,$0,$L102
	nop

	lw	$3,80($sp)
	li	$2,1056964608			# 0x3f000000
	ori	$2,$2,0x93c
	and	$2,$3,$2
	beq	$2,$0,$L103
	nop

	lw	$2,80($sp)
	andi	$2,$2,0x800
	beq	$2,$0,$L104
	nop

	li	$2,-1078984704			# 0xffffffffbfb00000
	ori	$2,$2,0x84
	sw	$2,56($sp)
	lw	$2,56($sp)
	lw	$2,0($2)
	sw	$2,32($sp)
	lw	$3,32($sp)
	li	$2,65536			# 0x10000
	or	$2,$3,$2
	li	$3,-1078984704			# 0xffffffffbfb00000
	ori	$3,$3,0x84
	sw	$3,60($sp)
	sw	$2,64($sp)
	lw	$2,60($sp)
	lw	$3,64($sp)
	sw	$3,0($2)
	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	lhu	$2,-30816($2)
	andi	$2,$2,0x4
	beq	$2,$0,$L104
	nop

	lui	$2,%hi(jiffies)
	lw	$3,%lo(jiffies)($2)
	lui	$2,%hi($LC19)
	addiu	$4,$2,%lo($LC19)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L104:
	lw	$2,80($sp)
	andi	$2,$2,0x100
	beq	$2,$0,$L106
	nop

	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	lhu	$2,-30816($2)
	andi	$2,$2,0x4
	beq	$2,$0,$L106
	nop

	lui	$2,%hi(jiffies)
	lw	$3,%lo(jiffies)($2)
	lui	$2,%hi($LC20)
	addiu	$4,$2,%lo($LC20)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L106:
	lw	$2,80($sp)
	andi	$2,$2,0x10
	beq	$2,$0,$L107
	nop

	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	lhu	$2,-30816($2)
	andi	$2,$2,0x4
	beq	$2,$0,$L107
	nop

	lui	$2,%hi(jiffies)
	lw	$3,%lo(jiffies)($2)
	lui	$2,%hi($LC21)
	addiu	$4,$2,%lo($LC21)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L107:
	lw	$2,80($sp)
	andi	$2,$2,0x20
	beq	$2,$0,$L108
	nop

	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	lhu	$2,-30816($2)
	andi	$2,$2,0x4
	beq	$2,$0,$L108
	nop

	lui	$2,%hi(jiffies)
	lw	$3,%lo(jiffies)($2)
	lui	$2,%hi($LC22)
	addiu	$4,$2,%lo($LC22)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L108:
	lw	$2,80($sp)
	andi	$2,$2,0x4
	beq	$2,$0,$L109
	nop

	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	lhu	$2,-30816($2)
	andi	$2,$2,0x4
	beq	$2,$0,$L109
	nop

	lui	$2,%hi(jiffies)
	lw	$3,%lo(jiffies)($2)
	lui	$2,%hi($LC23)
	addiu	$4,$2,%lo($LC23)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L109:
	lw	$2,80($sp)
	andi	$2,$2,0x8
	beq	$2,$0,$L110
	nop

	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	lhu	$2,-30816($2)
	andi	$2,$2,0x4
	beq	$2,$0,$L110
	nop

	lui	$2,%hi(jiffies)
	lw	$3,%lo(jiffies)($2)
	lui	$2,%hi($LC24)
	addiu	$4,$2,%lo($LC24)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L110:
	lw	$3,80($sp)
	li	$2,16777216			# 0x1000000
	and	$2,$3,$2
	beq	$2,$0,$L111
	nop

	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	lhu	$2,-30816($2)
	andi	$2,$2,0x4
	beq	$2,$0,$L111
	nop

	lui	$2,%hi(jiffies)
	lw	$3,%lo(jiffies)($2)
	lui	$2,%hi($LC25)
	addiu	$4,$2,%lo($LC25)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L111:
	lw	$3,80($sp)
	li	$2,33554432			# 0x2000000
	and	$2,$3,$2
	beq	$2,$0,$L112
	nop

	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	lhu	$2,-30816($2)
	andi	$2,$2,0x4
	beq	$2,$0,$L112
	nop

	lui	$2,%hi(jiffies)
	lw	$3,%lo(jiffies)($2)
	lui	$2,%hi($LC26)
	addiu	$4,$2,%lo($LC26)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L112:
	lw	$3,80($sp)
	li	$2,67108864			# 0x4000000
	and	$2,$3,$2
	beq	$2,$0,$L113
	nop

	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	lhu	$2,-30816($2)
	andi	$2,$2,0x4
	beq	$2,$0,$L113
	nop

	lui	$2,%hi(jiffies)
	lw	$3,%lo(jiffies)($2)
	lui	$2,%hi($LC27)
	addiu	$4,$2,%lo($LC27)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L113:
	lw	$3,80($sp)
	li	$2,536870912			# 0x20000000
	and	$2,$3,$2
	beq	$2,$0,$L114
	nop

	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	lhu	$2,-30816($2)
	andi	$2,$2,0x4
	beq	$2,$0,$L114
	nop

	lui	$2,%hi(jiffies)
	lw	$3,%lo(jiffies)($2)
	lui	$2,%hi($LC28)
	addiu	$4,$2,%lo($LC28)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L114:
	lw	$3,80($sp)
	li	$2,268435456			# 0x10000000
	and	$2,$3,$2
	beq	$2,$0,$L115
	nop

	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	lhu	$2,-30816($2)
	andi	$2,$2,0x4
	beq	$2,$0,$L115
	nop

	lui	$2,%hi(jiffies)
	lw	$3,%lo(jiffies)($2)
	lui	$2,%hi($LC29)
	addiu	$4,$2,%lo($LC29)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L115:
	lw	$3,80($sp)
	li	$2,134217728			# 0x8000000
	and	$2,$3,$2
	beq	$2,$0,$L103
	nop

	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	lhu	$2,-30816($2)
	andi	$2,$2,0x4
	beq	$2,$0,$L103
	nop

	lui	$2,%hi(jiffies)
	lw	$3,%lo(jiffies)($2)
	lui	$2,%hi($LC30)
	addiu	$4,$2,%lo($LC30)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L103:
	lw	$2,80($sp)
	andi	$2,$2,0x600
	beq	$2,$0,$L116
	nop

	lw	$2,80($sp)
	andi	$2,$2,0x400
	beq	$2,$0,$L117
	nop

	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lw	$3,448($2)
	li	$2,1073741824			# 0x40000000
	and	$2,$3,$2
	beq	$2,$0,$L118
	nop

	lui	$2,%hi(gpGponPriv)
	lw	$3,%lo(gpGponPriv)($2)
	lhu	$2,448($3)
	ins	$2,$0,14,1
	sh	$2,448($3)
	lui	$2,%hi(gpon_update_time_to_switch)
	addiu	$2,$2,%lo(gpon_update_time_to_switch)
	jalr	$2
	nop

	move	$4,$0
	lui	$2,%hi(gponDevSet1PPSInt)
	addiu	$2,$2,%lo(gponDevSet1PPSInt)
	jalr	$2
	nop

$L118:
	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	lhu	$2,-30816($2)
	andi	$2,$2,0x4
	beq	$2,$0,$L117
	nop

	lui	$2,%hi(jiffies)
	lw	$3,%lo(jiffies)($2)
	lui	$2,%hi($LC31)
	addiu	$4,$2,%lo($LC31)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L117:
	lw	$2,80($sp)
	andi	$2,$2,0x200
	beq	$2,$0,$L116
	nop

	addiu	$3,$sp,88
	addiu	$2,$sp,92
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(gponDevGetCurrentTod)
	addiu	$2,$2,%lo(gponDevGetCurrentTod)
	jalr	$2
	nop

	lw	$5,88($sp)
	lw	$6,92($sp)
	lw	$3,88($sp)
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	lw	$2,16596($2)
	subu	$3,$3,$2
	li	$2,999948288			# 0x3b9a0000
	ori	$2,$2,0xca00
	mul	$3,$3,$2
	lw	$4,92($sp)
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	lw	$2,16600($2)
	subu	$2,$4,$2
	addu	$3,$3,$2
	lui	$2,%hi($LC32)
	addiu	$4,$2,%lo($LC32)
	move	$7,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi(gpGponPriv)
	lw	$3,%lo(gpGponPriv)($2)
	lhu	$2,448($3)
	li	$4,1			# 0x1
	ins	$2,$4,14,1
	sh	$2,448($3)
	li	$4,1			# 0x1
	lui	$2,%hi(gponDevSet1PPSInt)
	addiu	$2,$2,%lo(gponDevSet1PPSInt)
	jalr	$2
	nop

	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	lhu	$2,-30816($2)
	andi	$2,$2,0x4
	beq	$2,$0,$L116
	nop

	lui	$2,%hi(jiffies)
	lw	$3,%lo(jiffies)($2)
	lui	$2,%hi($LC33)
	addiu	$4,$2,%lo($LC33)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L116:
	lw	$2,80($sp)
	andi	$2,$2,0x3
	beq	$2,$0,$L119
	nop

	lw	$2,80($sp)
	andi	$2,$2,0x2
	beq	$2,$0,$L120
	nop

	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	lhu	$2,-30816($2)
	andi	$2,$2,0x4
	beq	$2,$0,$L120
	nop

	lui	$2,%hi(jiffies)
	lw	$3,%lo(jiffies)($2)
	lui	$2,%hi($LC34)
	addiu	$4,$2,%lo($LC34)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L120:
	lw	$2,80($sp)
	andi	$2,$2,0x1
	beq	$2,$0,$L119
	nop

	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	lhu	$2,-30816($2)
	andi	$2,$2,0x4
	beq	$2,$0,$L121
	nop

	lui	$2,%hi(jiffies)
	lw	$3,%lo(jiffies)($2)
	lui	$2,%hi($LC35)
	addiu	$4,$2,%lo($LC35)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L121:
	lui	$2,%hi(gpon_recv_ploam_message)
	addiu	$2,$2,%lo(gpon_recv_ploam_message)
	jalr	$2
	nop

$L119:
	lw	$3,80($sp)
	li	$2,1090453504			# 0x40ff0000
	ori	$2,$2,0xc0
	and	$2,$3,$2
	beq	$2,$0,$L122
	nop

	lw	$2,80($sp)
	andi	$2,$2,0x80
	beq	$2,$0,$L123
	nop

	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	lhu	$2,-30816($2)
	andi	$2,$2,0x5
	beq	$2,$0,$L123
	nop

	lui	$2,%hi(jiffies)
	lw	$3,%lo(jiffies)($2)
	lui	$2,%hi($LC36)
	addiu	$4,$2,%lo($LC36)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L123:
	lw	$2,80($sp)
	andi	$2,$2,0x40
	beq	$2,$0,$L124
	nop

	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	lhu	$2,-30816($2)
	andi	$2,$2,0x5
	beq	$2,$0,$L124
	nop

	lui	$2,%hi(jiffies)
	lw	$3,%lo(jiffies)($2)
	lui	$2,%hi($LC37)
	addiu	$4,$2,%lo($LC37)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L124:
	lw	$3,80($sp)
	li	$2,1048576			# 0x100000
	and	$2,$3,$2
	beq	$2,$0,$L125
	nop

	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	lhu	$2,-30816($2)
	andi	$2,$2,0x5
	beq	$2,$0,$L125
	nop

	lui	$2,%hi(jiffies)
	lw	$3,%lo(jiffies)($2)
	lui	$2,%hi($LC38)
	addiu	$4,$2,%lo($LC38)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L125:
	lw	$3,80($sp)
	li	$2,524288			# 0x80000
	and	$2,$3,$2
	beq	$2,$0,$L126
	nop

	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	lhu	$2,-30816($2)
	andi	$2,$2,0x5
	beq	$2,$0,$L126
	nop

	lui	$2,%hi(jiffies)
	lw	$3,%lo(jiffies)($2)
	lui	$2,%hi($LC39)
	addiu	$4,$2,%lo($LC39)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L126:
	lw	$3,80($sp)
	li	$2,262144			# 0x40000
	and	$2,$3,$2
	beq	$2,$0,$L127
	nop

	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	lhu	$2,-30816($2)
	andi	$2,$2,0x5
	beq	$2,$0,$L128
	nop

	lui	$2,%hi(jiffies)
	lw	$3,%lo(jiffies)($2)
	lui	$2,%hi($LC40)
	addiu	$4,$2,%lo($LC40)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L128:
	addiu	$2,$sp,96
	move	$4,$2
	lui	$2,%hi(gponDevGetSuperframe)
	addiu	$2,$2,%lo(gponDevGetSuperframe)
	jalr	$2
	nop

	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	lhu	$2,-30816($2)
	andi	$2,$2,0x5
	beq	$2,$0,$L127
	nop

	lui	$2,%hi(jiffies)
	lw	$5,%lo(jiffies)($2)
	lw	$3,96($sp)
	lui	$2,%hi($LC41)
	addiu	$4,$2,%lo($LC41)
	move	$6,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L127:
	lw	$3,80($sp)
	li	$2,131072			# 0x20000
	and	$2,$3,$2
	beq	$2,$0,$L129
	nop

	move	$4,$0
	lui	$2,%hi($LC42)
	addiu	$5,$2,%lo($LC42)
	lui	$2,%hi(dbgtoMem)
	addiu	$2,$2,%lo(dbgtoMem)
	jalr	$2
	nop

$L129:
	lw	$3,80($sp)
	li	$2,1073741824			# 0x40000000
	and	$2,$3,$2
	beq	$2,$0,$L130
	nop

	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lw	$3,448($2)
	li	$2,8388608			# 0x800000
	and	$2,$3,$2
	bne	$2,$0,$L131
	nop

	move	$4,$0
	lui	$2,%hi(gponDevSetBwmStopTimeInt)
	addiu	$2,$2,%lo(gponDevSetBwmStopTimeInt)
	jalr	$2
	nop

$L131:
	addiu	$2,$sp,96
	move	$4,$2
	lui	$2,%hi(gponDevGetSuperframe)
	addiu	$2,$2,%lo(gponDevGetSuperframe)
	jalr	$2
	nop

	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	lhu	$2,-30816($2)
	andi	$2,$2,0x5
	beq	$2,$0,$L132
	nop

	lui	$2,%hi(jiffies)
	lw	$3,%lo(jiffies)($2)
	lui	$2,%hi($LC43)
	addiu	$4,$2,%lo($LC43)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L132:
	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	lhu	$2,-30816($2)
	andi	$2,$2,0x5
	beq	$2,$0,$L130
	nop

	lui	$2,%hi(jiffies)
	lw	$5,%lo(jiffies)($2)
	lw	$3,96($sp)
	lui	$2,%hi($LC41)
	addiu	$4,$2,%lo($LC41)
	move	$6,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L130:
	lw	$3,80($sp)
	li	$2,2097152			# 0x200000
	and	$2,$3,$2
	beq	$2,$0,$L133
	nop

	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	lhu	$2,-30816($2)
	andi	$2,$2,0x5
	beq	$2,$0,$L133
	nop

	lui	$2,%hi(jiffies)
	lw	$3,%lo(jiffies)($2)
	lui	$2,%hi($LC44)
	addiu	$4,$2,%lo($LC44)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L133:
	lw	$3,80($sp)
	li	$2,4194304			# 0x400000
	and	$2,$3,$2
	beq	$2,$0,$L134
	nop

	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	lhu	$2,-30816($2)
	andi	$2,$2,0x5
	beq	$2,$0,$L134
	nop

	lui	$2,%hi(jiffies)
	lw	$3,%lo(jiffies)($2)
	lui	$2,%hi($LC45)
	addiu	$4,$2,%lo($LC45)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L134:
	lw	$3,80($sp)
	li	$2,8388608			# 0x800000
	and	$2,$3,$2
	beq	$2,$0,$L135
	nop

	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	lhu	$2,-30816($2)
	andi	$2,$2,0x5
	beq	$2,$0,$L135
	nop

	lui	$2,%hi(jiffies)
	lw	$3,%lo(jiffies)($2)
	lui	$2,%hi($LC46)
	addiu	$4,$2,%lo($LC46)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L135:
	lw	$3,80($sp)
	li	$2,65536			# 0x10000
	and	$2,$3,$2
	beq	$2,$0,$L122
	nop

	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	lhu	$2,-30816($2)
	andi	$2,$2,0x5
	beq	$2,$0,$L122
	nop

	lui	$2,%hi(jiffies)
	lw	$3,%lo(jiffies)($2)
	lui	$2,%hi($LC47)
	addiu	$4,$2,%lo($LC47)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L122:
	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	lhu	$2,-30816($2)
	andi	$2,$2,0x100
	beq	$2,$0,$L99
	nop

	lw	$3,80($sp)
	li	$2,1090453504			# 0x40ff0000
	ori	$2,$2,0xc0
	and	$2,$3,$2
	beq	$2,$0,$L99
	nop

	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	lhu	$2,-30816($2)
	andi	$2,$2,0x100
	beq	$2,$0,$L137
	nop

	lui	$2,%hi(jiffies)
	lw	$5,%lo(jiffies)($2)
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	lw	$6,16928($2)
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	lw	$7,16932($2)
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	lw	$4,16936($2)
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	lw	$3,16940($2)
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	lw	$2,16960($2)
	sw	$4,16($sp)
	sw	$3,20($sp)
	sw	$2,24($sp)
	lui	$2,%hi($LC48)
	addiu	$4,$2,%lo($LC48)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L137:
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16928
	sw	$2,68($sp)
	lw	$2,68($sp)
	lw	$2,0($2)
	sw	$2,36($sp)
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16928
	move	$4,$2
	lw	$3,36($sp)
	li	$2,-536870912			# 0xffffffffe0000000
	or	$2,$3,$2
	sw	$4,72($sp)
	sw	$2,76($sp)
	lw	$2,72($sp)
	lw	$3,76($sp)
	sw	$3,0($2)
	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	lhu	$2,-30816($2)
	andi	$2,$2,0x100
	beq	$2,$0,$L139
	nop

	lui	$2,%hi(jiffies)
	lw	$5,%lo(jiffies)($2)
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	lw	$6,17152($2)
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	lw	$7,17156($2)
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	lw	$4,17160($2)
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	lw	$3,17164($2)
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	lw	$2,17168($2)
	sw	$4,16($sp)
	sw	$3,20($sp)
	sw	$2,24($sp)
	lui	$2,%hi($LC49)
	addiu	$4,$2,%lo($LC49)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L139:
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	li	$3,-1			# 0xffffffffffffffff
	sw	$3,16936($2)
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	li	$3,-1			# 0xffffffffffffffff
	sw	$3,16940($2)
	j	$L99
	nop

$L102:
	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	lhu	$2,-30816($2)
	andi	$2,$2,0x4
	beq	$2,$0,$L99
	nop

	lui	$2,%hi(jiffies)
	lw	$5,%lo(jiffies)($2)
	lw	$6,80($sp)
	lw	$3,84($sp)
	lui	$2,%hi($LC50)
	addiu	$4,$2,%lo($LC50)
	move	$7,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L99:
	lw	$31,108($sp)
	addiu	$sp,$sp,112
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	gpon_isr
	.size	gpon_isr, .-gpon_isr
	.rdata
	.align	2
$LC51:
	.ascii	"[%lu0ms]%s event exceed max num\012\000"
	.align	2
$LC52:
	.ascii	"[%lu0ms]gEmerStateindex exceed max num set 0\012\000"
	.align	2
$LC53:
	.ascii	"[%lu0ms]record gEmerStateindex %d \012\000"
	.section	.text.gpon_record_emergence_info,"ax",@progbits
	.align	2
	.globl	gpon_record_emergence_info
	.set	nomips16
	.set	nomicromips
	.ent	gpon_record_emergence_info
	.type	gpon_record_emergence_info, @function
gpon_record_emergence_info:
	.frame	$sp,48,$31		# vars= 24, regs= 2/0, args= 16, gp= 0
	.mask	0x80010000,-4
	.fmask	0x00000000,0
	addiu	$sp,$sp,-48
	sw	$31,44($sp)
	sw	$16,40($sp)
	move	$2,$4
	sb	$2,48($sp)
	sw	$0,16($sp)
	lbu	$2,48($sp)
	sltu	$2,$2,10
	bne	$2,$0,$L142
	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	lhu	$2,-30816($2)
	andi	$2,$2,0x1
	beq	$2,$0,$L143
	lui	$2,%hi(jiffies)
	lw	$3,%lo(jiffies)($2)
	lui	$2,%hi($LC51)
	addiu	$4,$2,%lo($LC51)
	move	$5,$3
	lui	$2,%hi(__func__.33013)
	addiu	$6,$2,%lo(__func__.33013)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	j	$L141
$L143:
	j	$L141
$L142:
	lui	$2,%hi(gEmerStateindex)
	addiu	$2,$2,%lo(gEmerStateindex)
	lw	$2,0($2)
	slt	$2,$2,10
	bne	$2,$0,$L145
	lui	$2,%hi(gEmerStateindex)
	sw	$0,%lo(gEmerStateindex)($2)
	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	lhu	$2,-30816($2)
	andi	$2,$2,0x100
	beq	$2,$0,$L145
	lui	$2,%hi(jiffies)
	lw	$3,%lo(jiffies)($2)
	lui	$2,%hi($LC52)
	addiu	$4,$2,%lo($LC52)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
$L145:
	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	lhu	$2,-30816($2)
	andi	$2,$2,0x100
	beq	$2,$0,$L146
	lui	$2,%hi(jiffies)
	lw	$5,%lo(jiffies)($2)
	lui	$2,%hi(gEmerStateindex)
	addiu	$2,$2,%lo(gEmerStateindex)
	lw	$3,0($2)
	lui	$2,%hi($LC53)
	addiu	$4,$2,%lo($LC53)
	move	$6,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
$L146:
	lui	$2,%hi(gEmerStateindex)
	addiu	$2,$2,%lo(gEmerStateindex)
	lw	$2,0($2)
	sll	$3,$2,4
	lui	$2,%hi(gEmergenceRecord)
	addiu	$2,$2,%lo(gEmergenceRecord)
	addu	$2,$3,$2
	sw	$2,16($sp)
	lw	$4,16($sp)
	move	$5,$0
	li	$6,16			# 0x10
	lui	$2,%hi(memset)
	addiu	$2,$2,%lo(memset)
	jalr	$2
	lw	$2,16($sp)
	li	$3,1			# 0x1
	sb	$3,8($2)
	lw	$16,16($sp)
	addiu	$2,$sp,32
	move	$4,$2
	lui	$2,%hi(ktime_get)
	addiu	$2,$2,%lo(ktime_get)
	jalr	$2
	lw	$3,36($sp)
	lw	$2,32($sp)
	sw	$3,4($16)
	sw	$2,0($16)
	lw	$2,16($sp)
	lbu	$3,48($sp)
	sb	$3,9($2)
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lbu	$3,215($2)
	lw	$2,16($sp)
	sb	$3,10($2)
	li	$2,1			# 0x1
	sw	$2,20($sp)
	lui	$2,%hi(gEmerStateindex)
	addiu	$2,$2,%lo(gEmerStateindex)
	sw	$2,24($sp)
	lw	$4,24($sp)
	lw	$3,20($sp)
	lw	$2,24($sp)
#APP
 # 67 "/opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/atomic.h" 1
		.set	mips3					
1:	ll	$5, 0($4)		# atomic_add		
	addu	$5, $3					
	sc	$5, 0($4)					
	beqz	$5, 2f					
	.subsection 2					
2:	b	1b					
	.previous					
	.set	mips0					

 # 0 "" 2
#NO_APP
	move	$2,$5
	sw	$2,28($sp)
$L141:
	lw	$31,44($sp)
	lw	$16,40($sp)
	addiu	$sp,$sp,48
	j	$31
	.end	gpon_record_emergence_info
	.size	gpon_record_emergence_info, .-gpon_record_emergence_info
	.rdata
	.align	2
$LC54:
	.ascii	"[%lu0ms]show printId %d, event %u ishappen %u\012\000"
	.align	2
$LC55:
	.ascii	"[%03dmin : %03ds : %03dms: %03dus : %03dns] \000"
	.align	2
$LC56:
	.ascii	"GPON RCV DISABLE_SN REPORTO7          \000"
	.align	2
$LC57:
	.ascii	"GPON RCV DISABLE_SN SET_EMERGNCE_STATE\000"
	.align	2
$LC58:
	.ascii	"GPON OMCI SET EMERGNCE_STATE          \000"
	.align	2
$LC59:
	.ascii	"GPON PHY READY REPORT_O7              \000"
	.align	2
$LC60:
	.ascii	"GPON SET CONNECTION START REPORT_O7   \000"
	.align	2
$LC61:
	.ascii	"No Info   \000"
	.align	2
$LC62:
	.ascii	"ON\000"
	.align	2
$LC63:
	.ascii	"OFF\000"
	.align	2
$LC64:
	.ascii	"  emergence state %s\012\000"
	.section	.text.gpon_show_emergence_info,"ax",@progbits
	.align	2
	.globl	gpon_show_emergence_info
	.set	nomips16
	.set	nomicromips
	.ent	gpon_show_emergence_info
	.type	gpon_show_emergence_info, @function
gpon_show_emergence_info:
	.frame	$sp,72,$31		# vars= 40, regs= 1/0, args= 24, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-72
	sw	$31,68($sp)
	sw	$0,24($sp)
	sw	$0,28($sp)
	sw	$0,32($sp)
	sw	$0,24($sp)
	j	$L148
	nop

$L161:
	lui	$2,%hi(gEmerStateindex)
	addiu	$2,$2,%lo(gEmerStateindex)
	lw	$3,0($2)
	lw	$2,24($sp)
	addu	$3,$3,$2
	li	$2,1717960704			# 0x66660000
	ori	$2,$2,0x6667
	mult	$3,$2
	mfhi	$2
	sra	$4,$2,2
	sra	$2,$3,31
	subu	$2,$4,$2
	sll	$2,$2,1
	sll	$4,$2,2
	addu	$2,$2,$4
	subu	$2,$3,$2
	sw	$2,28($sp)
	lw	$2,28($sp)
	sll	$3,$2,4
	lui	$2,%hi(gEmergenceRecord)
	addiu	$2,$2,%lo(gEmergenceRecord)
	addu	$2,$3,$2
	sw	$2,32($sp)
	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	lhu	$2,-30816($2)
	andi	$2,$2,0x100
	beq	$2,$0,$L149
	nop

	lui	$2,%hi(jiffies)
	lw	$3,%lo(jiffies)($2)
	lw	$2,32($sp)
	lbu	$2,9($2)
	move	$7,$2
	lw	$2,32($sp)
	lbu	$2,8($2)
	sw	$2,16($sp)
	lui	$2,%hi($LC54)
	addiu	$4,$2,%lo($LC54)
	move	$5,$3
	lw	$6,28($sp)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L149:
	lw	$2,32($sp)
	lbu	$3,8($2)
	li	$2,1			# 0x1
	bne	$3,$2,$L150
	nop

	lw	$2,32($sp)
	sw	$2,36($sp)
	lw	$2,36($sp)
	lw	$4,4($2)
	li	$2,274857984			# 0x10620000
	ori	$2,$2,0x4dd3
	mult	$4,$2
	mfhi	$2
	sra	$3,$2,6
	sra	$2,$4,31
	subu	$3,$3,$2
	move	$5,$3
	sll	$2,$5,2
	move	$5,$2
	sll	$2,$5,5
	subu	$2,$2,$5
	addu	$2,$2,$3
	sll	$2,$2,3
	subu	$3,$4,$2
	sw	$3,40($sp)
	lw	$2,36($sp)
	lw	$2,4($2)
	li	$3,274857984			# 0x10620000
	ori	$3,$3,0x4dd3
	mult	$2,$3
	mfhi	$3
	sra	$3,$3,6
	sra	$2,$2,31
	subu	$4,$3,$2
	li	$2,274857984			# 0x10620000
	ori	$2,$2,0x4dd3
	mult	$4,$2
	mfhi	$2
	sra	$3,$2,6
	sra	$2,$4,31
	subu	$3,$3,$2
	move	$5,$3
	sll	$2,$5,2
	move	$5,$2
	sll	$2,$5,5
	subu	$2,$2,$5
	addu	$2,$2,$3
	sll	$2,$2,3
	subu	$3,$4,$2
	sw	$3,44($sp)
	lw	$2,36($sp)
	lw	$2,4($2)
	li	$3,1125842944			# 0x431b0000
	ori	$3,$3,0xde83
	mult	$2,$3
	mfhi	$3
	sra	$3,$3,18
	sra	$2,$2,31
	subu	$4,$3,$2
	li	$2,274857984			# 0x10620000
	ori	$2,$2,0x4dd3
	mult	$4,$2
	mfhi	$2
	sra	$3,$2,6
	sra	$2,$4,31
	subu	$3,$3,$2
	move	$5,$3
	sll	$2,$5,2
	move	$5,$2
	sll	$2,$5,5
	subu	$2,$2,$5
	addu	$2,$2,$3
	sll	$2,$2,3
	subu	$3,$4,$2
	sw	$3,48($sp)
	lw	$2,36($sp)
	lw	$2,0($2)
	li	$3,-2004353024			# 0xffffffff88880000
	ori	$3,$3,0x8889
	mult	$2,$3
	mfhi	$3
	addu	$3,$3,$2
	sra	$4,$3,5
	sra	$3,$2,31
	subu	$3,$4,$3
	move	$4,$3
	sll	$3,$4,2
	move	$4,$3
	sll	$3,$4,4
	subu	$3,$3,$4
	subu	$3,$2,$3
	sw	$3,52($sp)
	lw	$2,36($sp)
	lw	$2,0($2)
	li	$3,-2004353024			# 0xffffffff88880000
	ori	$3,$3,0x8889
	mult	$2,$3
	mfhi	$3
	addu	$3,$3,$2
	sra	$3,$3,5
	sra	$2,$2,31
	subu	$2,$3,$2
	sw	$2,56($sp)
	lw	$2,44($sp)
	sw	$2,16($sp)
	lw	$2,40($sp)
	sw	$2,20($sp)
	lui	$2,%hi($LC55)
	addiu	$4,$2,%lo($LC55)
	lw	$5,56($sp)
	lw	$6,52($sp)
	lw	$7,48($sp)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lw	$2,32($sp)
	lbu	$2,9($2)
	sltu	$3,$2,5
	beq	$3,$0,$L151
	nop

	sll	$3,$2,2
	lui	$2,%hi($L153)
	addiu	$2,$2,%lo($L153)
	addu	$2,$3,$2
	lw	$2,0($2)
	j	$2
	nop

	.rdata
	.align	2
	.align	2
$L153:
	.word	$L152
	.word	$L154
	.word	$L155
	.word	$L156
	.word	$L157
	.section	.text.gpon_show_emergence_info
$L152:
	lui	$2,%hi($LC56)
	addiu	$4,$2,%lo($LC56)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	j	$L158
	nop

$L154:
	lui	$2,%hi($LC57)
	addiu	$4,$2,%lo($LC57)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	j	$L158
	nop

$L157:
	lui	$2,%hi($LC58)
	addiu	$4,$2,%lo($LC58)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	j	$L158
	nop

$L155:
	lui	$2,%hi($LC59)
	addiu	$4,$2,%lo($LC59)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	j	$L158
	nop

$L156:
	lui	$2,%hi($LC60)
	addiu	$4,$2,%lo($LC60)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	j	$L158
	nop

$L151:
	lui	$2,%hi($LC61)
	addiu	$4,$2,%lo($LC61)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	nop
$L158:
	lw	$2,32($sp)
	lbu	$2,10($2)
	beq	$2,$0,$L159
	nop

	lui	$2,%hi($LC62)
	addiu	$2,$2,%lo($LC62)
	j	$L160
	nop

$L159:
	lui	$2,%hi($LC63)
	addiu	$2,$2,%lo($LC63)
$L160:
	lui	$3,%hi($LC64)
	addiu	$4,$3,%lo($LC64)
	move	$5,$2
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L150:
	lw	$2,24($sp)
	addiu	$2,$2,1
	sw	$2,24($sp)
$L148:
	lw	$2,24($sp)
	slt	$2,$2,10
	bne	$2,$0,$L161
	nop

	lw	$31,68($sp)
	addiu	$sp,$sp,72
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	gpon_show_emergence_info
	.size	gpon_show_emergence_info, .-gpon_show_emergence_info
	.section	.text.gpon_stop_timer,"ax",@progbits
	.align	2
	.globl	gpon_stop_timer
	.set	nomips16
	.set	nomicromips
	.ent	gpon_stop_timer
	.type	gpon_stop_timer, @function
gpon_stop_timer:
	.frame	$sp,24,$31		# vars= 0, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-24
	sw	$31,20($sp)
	move	$2,$28
	lw	$2,20($2)
	move	$3,$2
	li	$2,134152192			# 0x7ff0000
	ori	$2,$2,0xff00
	and	$2,$3,$2
	beq	$2,$0,$L163
	nop

	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	addiu	$2,$2,8
	move	$4,$2
	lui	$2,%hi(del_timer)
	addiu	$2,$2,%lo(del_timer)
	jalr	$2
	nop

	j	$L164
	nop

$L163:
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	addiu	$2,$2,8
	move	$4,$2
	lui	$2,%hi(del_timer_sync)
	addiu	$2,$2,%lo(del_timer_sync)
	jalr	$2
	nop

$L164:
	move	$2,$28
	lw	$2,20($2)
	move	$3,$2
	li	$2,134152192			# 0x7ff0000
	ori	$2,$2,0xff00
	and	$2,$3,$2
	beq	$2,$0,$L165
	nop

	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	addiu	$2,$2,36
	move	$4,$2
	lui	$2,%hi(del_timer)
	addiu	$2,$2,%lo(del_timer)
	jalr	$2
	nop

	j	$L166
	nop

$L165:
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	addiu	$2,$2,36
	move	$4,$2
	lui	$2,%hi(del_timer_sync)
	addiu	$2,$2,%lo(del_timer_sync)
	jalr	$2
	nop

$L166:
	move	$2,$28
	lw	$2,20($2)
	move	$3,$2
	li	$2,134152192			# 0x7ff0000
	ori	$2,$2,0xff00
	and	$2,$3,$2
	beq	$2,$0,$L167
	nop

	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	addiu	$2,$2,64
	move	$4,$2
	lui	$2,%hi(del_timer)
	addiu	$2,$2,%lo(del_timer)
	jalr	$2
	nop

	j	$L168
	nop

$L167:
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	addiu	$2,$2,64
	move	$4,$2
	lui	$2,%hi(del_timer_sync)
	addiu	$2,$2,%lo(del_timer_sync)
	jalr	$2
	nop

$L168:
	move	$2,$28
	lw	$2,20($2)
	move	$3,$2
	li	$2,134152192			# 0x7ff0000
	ori	$2,$2,0xff00
	and	$2,$3,$2
	beq	$2,$0,$L169
	nop

	lui	$2,%hi(gpWanPriv)
	lw	$3,%lo(gpWanPriv)($2)
	li	$2,32864			# 0x8060
	addu	$2,$3,$2
	move	$4,$2
	lui	$2,%hi(del_timer)
	addiu	$2,$2,%lo(del_timer)
	jalr	$2
	nop

	j	$L170
	nop

$L169:
	lui	$2,%hi(gpWanPriv)
	lw	$3,%lo(gpWanPriv)($2)
	li	$2,32864			# 0x8060
	addu	$2,$3,$2
	move	$4,$2
	lui	$2,%hi(del_timer_sync)
	addiu	$2,$2,%lo(del_timer_sync)
	jalr	$2
	nop

$L170:
	move	$2,$28
	lw	$2,20($2)
	move	$3,$2
	li	$2,134152192			# 0x7ff0000
	ori	$2,$2,0xff00
	and	$2,$3,$2
	beq	$2,$0,$L171
	nop

	lui	$2,%hi(gpWanPriv)
	lw	$3,%lo(gpWanPriv)($2)
	li	$2,33688			# 0x8398
	addu	$2,$3,$2
	move	$4,$2
	lui	$2,%hi(del_timer)
	addiu	$2,$2,%lo(del_timer)
	jalr	$2
	nop

	j	$L172
	nop

$L171:
	lui	$2,%hi(gpWanPriv)
	lw	$3,%lo(gpWanPriv)($2)
	li	$2,33688			# 0x8398
	addu	$2,$3,$2
	move	$4,$2
	lui	$2,%hi(del_timer_sync)
	addiu	$2,$2,%lo(del_timer_sync)
	jalr	$2
	nop

$L172:
	lui	$2,%hi(stop_omci_oam_monitor)
	addiu	$2,$2,%lo(stop_omci_oam_monitor)
	jalr	$2
	nop

	lw	$31,20($sp)
	addiu	$sp,$sp,24
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	gpon_stop_timer
	.size	gpon_stop_timer, .-gpon_stop_timer
	.rdata
	.align	2
	.type	__func__.32912, @object
	.size	__func__.32912, 24
__func__.32912:
	.ascii	"schedule_gpon_dev_reset\000"
	.local	same_cnt.32985
	.comm	same_cnt.32985,4,4
	.align	2
	.type	__func__.33013, @object
	.size	__func__.33013, 27
__func__.33013:
	.ascii	"gpon_record_emergence_info\000"
	.ident	"GCC: (Buildroot 2015.08.1) 4.9.3"
