	.file	1 "xpondrv.c"
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
	.data
	.align	2
	.type	dying_gasp_oam, @object
	.size	dying_gasp_oam, 58
dying_gasp_oam:
	.byte	0
	.byte	0
	.byte	1
	.byte	-128
	.byte	-62
	.byte	0
	.byte	0
	.byte	2
	.byte	0
	.byte	37
	.byte	18
	.byte	-69
	.byte	37
	.byte	14
	.byte	-120
	.byte	9
	.byte	3
	.byte	0
	.byte	82
	.byte	0
	.byte	1
	.byte	16
	.byte	1
	.byte	0
	.byte	0
	.byte	0
	.byte	15
	.byte	5
	.byte	-18
	.byte	0
	.byte	19
	.byte	37
	.byte	0
	.byte	34
	.byte	1
	.byte	0
	.byte	2
	.byte	16
	.byte	1
	.byte	0
	.byte	0
	.byte	0
	.byte	15
	.byte	5
	.byte	-18
	.byte	0
	.byte	19
	.byte	37
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.globl	pon_mode
	.section	.bss,"aw",@nobits
	.type	pon_mode, @object
	.size	pon_mode, 1
pon_mode:
	.space	1
	.local	epon_proc_cnt
	.comm	epon_proc_cnt,4,4
	.globl	xpon_regs_p
	.align	2
	.type	xpon_regs_p, @object
	.size	xpon_regs_p, 4
xpon_regs_p:
	.space	4
	.local	epon_stat_prev
	.comm	epon_stat_prev,4,4
	.local	xpon_proc_dir
	.comm	xpon_proc_dir,4,4
	.local	gpon_proc_dir
	.comm	gpon_proc_dir,4,4
	.local	epon_proc_dir
	.comm	epon_proc_dir,4,4
	.local	gpon_debug_proc_dir
	.comm	gpon_debug_proc_dir,4,4
	.local	gpon_power_management_proc_dir
	.comm	gpon_power_management_proc_dir,4,4
	.globl	gpPonSysData
	.align	2
	.type	gpPonSysData, @object
	.size	gpPonSysData, 4
gpPonSysData:
	.space	4
	.globl	gpWanPriv
	.align	2
	.type	gpWanPriv, @object
	.size	gpWanPriv, 4
gpWanPriv:
	.space	4
	.globl	gpMcsPriv
	.align	2
	.type	gpMcsPriv, @object
	.size	gpMcsPriv, 4
gpMcsPriv:
	.space	4
	.globl	gpGponPriv
	.align	2
	.type	gpGponPriv, @object
	.size	gpGponPriv, 4
gpGponPriv:
	.space	4
	.globl	gpEponPriv
	.align	2
	.type	gpEponPriv, @object
	.size	gpEponPriv, 4
gpEponPriv:
	.space	4
	.section	.text.trans_status_refresh,"ax",@progbits
	.align	2
	.globl	trans_status_refresh
	.set	nomips16
	.set	nomicromips
	.ent	trans_status_refresh
	.type	trans_status_refresh, @function
trans_status_refresh:
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
	lbu	$2,62($2)
	bne	$2,$0,$L2
	nop

	sw	$0,16($sp)
	sw	$0,20($sp)
	lui	$2,%hi(gpPhyData)
	lw	$2,%lo(gpPhyData)($2)
	li	$3,1			# 0x1
	sb	$3,62($2)
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

$L2:
	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	lw	$3,-30412($2)
	li	$2,1073741824			# 0x40000000
	and	$2,$3,$2
	bne	$2,$0,$L1
	nop

	lui	$2,%hi(gpPhyData)
	lw	$2,%lo(gpPhyData)($2)
	addiu	$4,$2,64
	lui	$2,%hi(gpPhyData)
	lw	$2,%lo(gpPhyData)($2)
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

$L1:
	lw	$31,36($sp)
	addiu	$sp,$sp,40
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	trans_status_refresh
	.size	trans_status_refresh, .-trans_status_refresh
	.section	.text.xpondrv_init_phy_data,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	xpondrv_init_phy_data
	.type	xpondrv_init_phy_data, @function
xpondrv_init_phy_data:
	.frame	$sp,32,$31		# vars= 8, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-32
	sw	$31,28($sp)
	lui	$2,%hi(gpPhyData)
	lw	$3,%lo(gpPhyData)($2)
	lhu	$2,60($3)
	ins	$2,$0,15,1
	sh	$2,60($3)
	lui	$2,%hi(gpPhyData)
	lw	$3,%lo(gpPhyData)($2)
	lhu	$2,60($3)
	ins	$2,$0,8,1
	sh	$2,60($3)
	lui	$2,%hi(gpPhyData)
	lw	$3,%lo(gpPhyData)($2)
	lhu	$2,60($3)
	li	$4,2			# 0x2
	ins	$2,$4,9,2
	sh	$2,60($3)
	lui	$2,%hi(gpPhyData)
	lw	$3,%lo(gpPhyData)($2)
	lhu	$2,60($3)
	li	$4,2			# 0x2
	ins	$2,$4,13,2
	sh	$2,60($3)
	lui	$2,%hi(gpPhyData)
	lw	$3,%lo(gpPhyData)($2)
	lhu	$2,60($3)
	ins	$2,$0,11,2
	sh	$2,60($3)
	lui	$2,%hi(gpPhyData)
	lw	$3,%lo(gpPhyData)($2)
	lhu	$2,60($3)
	li	$4,2			# 0x2
	ins	$2,$4,5,3
	sh	$2,60($3)
	lui	$2,%hi(gpPhyData)
	lw	$2,%lo(gpPhyData)($2)
	addiu	$2,$2,92
	sw	$2,16($sp)
	lui	$2,%hi(gpPhyData)
	lw	$2,%lo(gpPhyData)($2)
	sw	$0,92($2)
	lui	$2,%hi(gpPhyData)
	lw	$2,%lo(gpPhyData)($2)
	addiu	$2,$2,96
	move	$4,$2
	move	$5,$0
	li	$6,10			# 0xa
	lui	$2,%hi(memset)
	addiu	$2,$2,%lo(memset)
	jalr	$2
	nop

	lui	$2,%hi(gpPhyData)
	lw	$2,%lo(gpPhyData)($2)
	addiu	$2,$2,64
	move	$4,$2
	move	$5,$0
	move	$6,$0
	lui	$2,%hi(init_timer_key)
	addiu	$2,$2,%lo(init_timer_key)
	jalr	$2
	nop

	lui	$2,%hi(gpPhyData)
	lw	$2,%lo(gpPhyData)($2)
	li	$3,1000			# 0x3e8
	sw	$3,84($2)
	lui	$2,%hi(gpPhyData)
	lw	$2,%lo(gpPhyData)($2)
	lui	$3,%hi(trans_status_refresh)
	addiu	$3,$3,%lo(trans_status_refresh)
	sw	$3,80($2)
	lui	$2,%hi(gpPhyData)
	lw	$2,%lo(gpPhyData)($2)
	sb	$0,62($2)
	lui	$2,%hi(gpPhyData)
	lw	$2,%lo(gpPhyData)($2)
	addiu	$2,$2,4
	move	$4,$2
	move	$5,$0
	move	$6,$0
	lui	$2,%hi(init_timer_key)
	addiu	$2,$2,%lo(init_timer_key)
	jalr	$2
	nop

	lui	$2,%hi(gpPhyData)
	lw	$2,%lo(gpPhyData)($2)
	li	$3,3000			# 0xbb8
	sw	$3,24($2)
	lui	$2,%hi(gpPhyData)
	lw	$2,%lo(gpPhyData)($2)
	lui	$3,%hi(try_set_phy_mode)
	addiu	$3,$3,%lo(try_set_phy_mode)
	sw	$3,20($2)
	lui	$2,%hi(gpPhyData)
	lw	$2,%lo(gpPhyData)($2)
	addiu	$2,$2,32
	move	$4,$2
	move	$5,$0
	move	$6,$0
	lui	$2,%hi(init_timer_key)
	addiu	$2,$2,%lo(init_timer_key)
	jalr	$2
	nop

	lui	$2,%hi(gpPhyData)
	lw	$2,%lo(gpPhyData)($2)
	li	$3,1000			# 0x3e8
	sw	$3,52($2)
	lui	$2,%hi(gpPhyData)
	lw	$2,%lo(gpPhyData)($2)
	lui	$3,%hi(check_sync_result)
	addiu	$3,$3,%lo(check_sync_result)
	sw	$3,48($2)
	lw	$31,28($sp)
	addiu	$sp,$sp,32
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	xpondrv_init_phy_data
	.size	xpondrv_init_phy_data, .-xpondrv_init_phy_data
	.rdata
	.align	2
$LC0:
	.ascii	"xpon_daemon\000"
	.align	2
$LC1:
	.ascii	"@%s>>%d--xpon_daemon init failed\012\000"
	.section	.text.xpondrv_init_global_data,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	xpondrv_init_global_data
	.type	xpondrv_init_global_data, @function
xpondrv_init_global_data:
	.frame	$sp,168,$31		# vars= 144, regs= 2/0, args= 16, gp= 0
	.mask	0x80010000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-168
	sw	$31,164($sp)
	sw	$16,160($sp)
	lui	$2,%hi(gpPonSysData)
	lw	$16,%lo(gpPonSysData)($2)
	li	$2,-1079967744			# 0xffffffffbfa10000
	ori	$2,$2,0x114
	lw	$2,0($2)
	andi	$2,$2,0x2
	beq	$2,$0,$L7
	nop

	lui	$2,%hi(ranand_read_byte)
	lw	$2,%lo(ranand_read_byte)($2)
	beq	$2,$0,$L8
	nop

	lui	$2,%hi(ranand_read_byte)
	lw	$2,%lo(ranand_read_byte)($2)
	lui	$3,%hi(flash_base)
	lw	$4,%lo(flash_base)($3)
	li	$3,65436			# 0xff9c
	addu	$3,$4,$3
	move	$7,$3
	move	$6,$0
	move	$5,$7
	move	$4,$6
	jalr	$2
	nop

	j	$L10
	nop

$L8:
	li	$2,-1			# 0xffffffffffffffff
	j	$L10
	nop

$L7:
	lui	$2,%hi(flash_base)
	lw	$3,%lo(flash_base)($2)
	li	$2,65436			# 0xff9c
	addu	$2,$3,$2
	move	$4,$2
	lui	$2,%hi(ReadSPIByte)
	addiu	$2,$2,%lo(ReadSPIByte)
	jalr	$2
	nop

$L10:
	li	$3,65536			# 0x10000
	addu	$3,$16,$3
	sw	$2,-30824($3)
	lui	$2,%hi(gpPonSysData)
	lw	$2,%lo(gpPonSysData)($2)
	sw	$0,0($2)
	lui	$2,%hi(gpPonSysData)
	lw	$2,%lo(gpPonSysData)($2)
	sw	$0,4($2)
	lui	$2,%hi(gpPonSysData)
	lw	$2,%lo(gpPonSysData)($2)
	li	$3,1			# 0x1
	sw	$3,8($2)
	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,34664			# 0x8768
	addu	$3,$3,$2
	lui	$2,%hi(gpEponPriv)
	sw	$3,%lo(gpEponPriv)($2)
	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,34180			# 0x8584
	addu	$3,$3,$2
	lui	$2,%hi(gpGponPriv)
	sw	$3,%lo(gpGponPriv)($2)
	lui	$2,%hi(gpPonSysData)
	lw	$2,%lo(gpPonSysData)($2)
	addiu	$3,$2,12
	lui	$2,%hi(gpWanPriv)
	sw	$3,%lo(gpWanPriv)($2)
	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,33768			# 0x83e8
	addu	$3,$3,$2
	lui	$2,%hi(gpMcsPriv)
	sw	$3,%lo(gpMcsPriv)($2)
	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,34724			# 0x87a4
	addu	$3,$3,$2
	lui	$2,%hi(gpPhyData)
	sw	$3,%lo(gpPhyData)($2)
	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	sh	$0,-30816($2)
	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	lw	$3,-30824($2)
	li	$2,2			# 0x2
	beq	$3,$2,$L11
	nop

	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	li	$3,1			# 0x1
	sw	$3,-30824($2)
	j	$L12
	nop

$L11:
	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	li	$3,2			# 0x2
	sw	$3,-30824($2)
$L12:
	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$3,$3,$2
	lbu	$2,-30828($3)
	li	$4,1			# 0x1
	ins	$2,$4,6,1
	sb	$2,-30828($3)
	li	$2,-1078984704			# 0xffffffffbfb00000
	ori	$2,$2,0x64
	sw	$2,20($sp)
	lw	$2,20($sp)
	lw	$2,0($2)
	andi	$2,$2,0xff
	sltu	$2,$2,3
	beq	$2,$0,$L14
	nop

	li	$2,-1078984704			# 0xffffffffbfb00000
	ori	$2,$2,0xf8
	sw	$2,24($sp)
	lw	$2,24($sp)
	lw	$2,0($2)
	andi	$3,$2,0x3
	li	$2,2			# 0x2
	bne	$3,$2,$L16
	nop

	li	$2,-1078984704			# 0xffffffffbfb00000
	ori	$2,$2,0x64
	sw	$2,28($sp)
	lw	$2,28($sp)
	lw	$3,0($2)
	li	$2,-65536			# 0xffffffffffff0000
	and	$3,$3,$2
	li	$2,327680			# 0x50000
	bne	$3,$2,$L16
	nop

	li	$2,-1078001664			# 0xffffffffbfbf0000
	ori	$2,$2,0x8214
	sw	$2,32($sp)
	lw	$2,32($sp)
	lw	$2,0($2)
	andi	$2,$2,0x8
	beq	$2,$0,$L19
	nop

	li	$2,-1078001664			# 0xffffffffbfbf0000
	ori	$2,$2,0x8214
	sw	$2,36($sp)
	lw	$2,36($sp)
	lw	$2,0($2)
	andi	$2,$2,0x20
	bne	$2,$0,$L21
	nop

	j	$L16
	nop

$L19:
	li	$2,-1078001664			# 0xffffffffbfbf0000
	ori	$2,$2,0x8214
	sw	$2,40($sp)
	lw	$2,40($sp)
	lw	$2,0($2)
	andi	$2,$2,0x2
	beq	$2,$0,$L16
	nop

$L21:
	li	$2,-1078001664			# 0xffffffffbfbf0000
	ori	$2,$2,0x8214
	sw	$2,44($sp)
	lw	$2,44($sp)
	lw	$2,0($2)
	andi	$2,$2,0x8
	beq	$2,$0,$L24
	nop

	li	$2,-1078001664			# 0xffffffffbfbf0000
	ori	$2,$2,0x8214
	sw	$2,48($sp)
	lw	$2,48($sp)
	lw	$2,0($2)
	andi	$2,$2,0x40
	beq	$2,$0,$L26
	nop

	j	$L16
	nop

$L24:
	li	$2,-1078001664			# 0xffffffffbfbf0000
	ori	$2,$2,0x8214
	sw	$2,52($sp)
	lw	$2,52($sp)
	lw	$2,0($2)
	andi	$2,$2,0x4
	beq	$2,$0,$L26
	nop

$L16:
	li	$2,-1078984704			# 0xffffffffbfb00000
	ori	$2,$2,0xf8
	sw	$2,56($sp)
	lw	$2,56($sp)
	lw	$2,0($2)
	andi	$3,$2,0x3
	li	$2,3			# 0x3
	bne	$3,$2,$L29
	nop

	li	$2,-1078984704			# 0xffffffffbfb00000
	ori	$2,$2,0x64
	sw	$2,60($sp)
	lw	$2,60($sp)
	lw	$3,0($2)
	li	$2,-65536			# 0xffffffffffff0000
	and	$3,$3,$2
	li	$2,327680			# 0x50000
	bne	$3,$2,$L29
	nop

	li	$2,-1078001664			# 0xffffffffbfbf0000
	ori	$2,$2,0x8214
	sw	$2,64($sp)
	lw	$2,64($sp)
	lw	$2,0($2)
	andi	$2,$2,0x8
	beq	$2,$0,$L32
	nop

	li	$2,-1078001664			# 0xffffffffbfbf0000
	ori	$2,$2,0x8214
	sw	$2,68($sp)
	lw	$2,68($sp)
	lw	$2,0($2)
	andi	$2,$2,0x20
	bne	$2,$0,$L26
	nop

	j	$L29
	nop

$L32:
	li	$2,-1078001664			# 0xffffffffbfbf0000
	ori	$2,$2,0x8214
	sw	$2,72($sp)
	lw	$2,72($sp)
	lw	$2,0($2)
	andi	$2,$2,0x2
	bne	$2,$0,$L26
	nop

$L29:
	li	$2,-1078984704			# 0xffffffffbfb00000
	ori	$2,$2,0xf8
	sw	$2,76($sp)
	lw	$2,76($sp)
	lw	$2,0($2)
	andi	$3,$2,0x3
	li	$2,2			# 0x2
	bne	$3,$2,$L36
	nop

	li	$2,-1078984704			# 0xffffffffbfb00000
	ori	$2,$2,0x64
	sw	$2,80($sp)
	lw	$2,80($sp)
	lw	$3,0($2)
	li	$2,-65536			# 0xffffffffffff0000
	and	$3,$3,$2
	li	$2,327680			# 0x50000
	bne	$3,$2,$L36
	nop

	li	$2,-1078001664			# 0xffffffffbfbf0000
	ori	$2,$2,0x8214
	sw	$2,84($sp)
	lw	$2,84($sp)
	lw	$2,0($2)
	andi	$2,$2,0x8
	beq	$2,$0,$L39
	nop

	li	$2,-1078001664			# 0xffffffffbfbf0000
	ori	$2,$2,0x8214
	sw	$2,88($sp)
	lw	$2,88($sp)
	lw	$2,0($2)
	andi	$2,$2,0x20
	beq	$2,$0,$L41
	nop

	j	$L36
	nop

$L39:
	li	$2,-1078001664			# 0xffffffffbfbf0000
	ori	$2,$2,0x8214
	sw	$2,92($sp)
	lw	$2,92($sp)
	lw	$2,0($2)
	andi	$2,$2,0x2
	bne	$2,$0,$L36
	nop

$L41:
	li	$2,-1078001664			# 0xffffffffbfbf0000
	ori	$2,$2,0x8214
	sw	$2,96($sp)
	lw	$2,96($sp)
	lw	$2,0($2)
	andi	$2,$2,0x8
	beq	$2,$0,$L44
	nop

	li	$2,-1078001664			# 0xffffffffbfbf0000
	ori	$2,$2,0x8214
	sw	$2,100($sp)
	lw	$2,100($sp)
	lw	$2,0($2)
	andi	$2,$2,0x40
	beq	$2,$0,$L26
	nop

	j	$L36
	nop

$L44:
	li	$2,-1078001664			# 0xffffffffbfbf0000
	ori	$2,$2,0x8214
	sw	$2,104($sp)
	lw	$2,104($sp)
	lw	$2,0($2)
	andi	$2,$2,0x4
	beq	$2,$0,$L26
	nop

$L36:
	li	$2,-1078984704			# 0xffffffffbfb00000
	ori	$2,$2,0xf8
	sw	$2,108($sp)
	lw	$2,108($sp)
	lw	$2,0($2)
	andi	$3,$2,0x3
	li	$2,3			# 0x3
	bne	$3,$2,$L48
	nop

	li	$2,-1078984704			# 0xffffffffbfb00000
	ori	$2,$2,0x64
	sw	$2,112($sp)
	lw	$2,112($sp)
	lw	$3,0($2)
	li	$2,-65536			# 0xffffffffffff0000
	and	$3,$3,$2
	li	$2,327680			# 0x50000
	bne	$3,$2,$L48
	nop

	li	$2,-1078001664			# 0xffffffffbfbf0000
	ori	$2,$2,0x8214
	sw	$2,116($sp)
	lw	$2,116($sp)
	lw	$2,0($2)
	andi	$2,$2,0x8
	beq	$2,$0,$L51
	nop

	li	$2,-1078001664			# 0xffffffffbfbf0000
	ori	$2,$2,0x8214
	sw	$2,120($sp)
	lw	$2,120($sp)
	lw	$2,0($2)
	andi	$2,$2,0x20
	beq	$2,$0,$L26
	nop

	j	$L48
	nop

$L51:
	li	$2,-1078001664			# 0xffffffffbfbf0000
	ori	$2,$2,0x8214
	sw	$2,124($sp)
	lw	$2,124($sp)
	lw	$2,0($2)
	andi	$2,$2,0x2
	beq	$2,$0,$L26
	nop

$L48:
	li	$2,-1078984704			# 0xffffffffbfb00000
	ori	$2,$2,0xf8
	sw	$2,128($sp)
	lw	$2,128($sp)
	lw	$2,0($2)
	andi	$3,$2,0x3
	li	$2,2			# 0x2
	bne	$3,$2,$L14
	nop

	li	$2,-1078984704			# 0xffffffffbfb00000
	ori	$2,$2,0x64
	sw	$2,132($sp)
	lw	$2,132($sp)
	lw	$3,0($2)
	li	$2,-65536			# 0xffffffffffff0000
	and	$3,$3,$2
	li	$2,327680			# 0x50000
	bne	$3,$2,$L14
	nop

	li	$2,-1078001664			# 0xffffffffbfbf0000
	ori	$2,$2,0x8214
	sw	$2,136($sp)
	lw	$2,136($sp)
	lw	$2,0($2)
	andi	$2,$2,0x8
	beq	$2,$0,$L57
	nop

	li	$2,-1078001664			# 0xffffffffbfbf0000
	ori	$2,$2,0x8214
	sw	$2,140($sp)
	lw	$2,140($sp)
	lw	$2,0($2)
	andi	$2,$2,0x40
	bne	$2,$0,$L26
	nop

	j	$L14
	nop

$L57:
	li	$2,-1078001664			# 0xffffffffbfbf0000
	ori	$2,$2,0x8214
	sw	$2,144($sp)
	lw	$2,144($sp)
	lw	$2,0($2)
	andi	$2,$2,0x4
	beq	$2,$0,$L14
	nop

$L26:
	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$3,$3,$2
	lbu	$2,-30828($3)
	ins	$2,$0,7,1
	sb	$2,-30828($3)
	j	$L60
	nop

$L14:
	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$3,$3,$2
	lbu	$2,-30828($3)
	li	$4,1			# 0x1
	ins	$2,$4,7,1
	sb	$2,-30828($3)
$L60:
	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	sw	$0,-30820($2)
	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,34832			# 0x8810
	addu	$2,$3,$2
	move	$4,$2
	lui	$2,%hi(__key.33752)
	addiu	$5,$2,%lo(__key.33752)
	lui	$2,%hi(__init_waitqueue_head)
	addiu	$2,$2,%lo(__init_waitqueue_head)
	jalr	$2
	nop

	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,34848			# 0x8820
	addu	$2,$3,$2
	move	$4,$2
	move	$5,$0
	li	$6,272			# 0x110
	lui	$2,%hi(memset)
	addiu	$2,$2,%lo(memset)
	jalr	$2
	nop

	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,35104			# 0x8920
	addu	$2,$3,$2
	sw	$2,148($sp)
	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	sw	$0,-30432($2)
	lui	$2,%hi(gpPonSysData)
	lw	$16,%lo(gpPonSysData)($2)
	lui	$2,%hi(xpon_daemon)
	addiu	$4,$2,%lo(xpon_daemon)
	move	$5,$0
	lui	$2,%hi($LC0)
	addiu	$6,$2,%lo($LC0)
	lui	$2,%hi(kthread_create)
	addiu	$2,$2,%lo(kthread_create)
	jalr	$2
	nop

	sw	$2,16($sp)
	lw	$2,16($sp)
	sw	$2,152($sp)
	lw	$2,152($sp)
	sltu	$2,$2,-4095
	xori	$2,$2,0x1
	andi	$2,$2,0x00ff
	bne	$2,$0,$L63
	nop

	lw	$4,16($sp)
	lui	$2,%hi(wake_up_process)
	addiu	$2,$2,%lo(wake_up_process)
	jalr	$2
	nop

$L63:
	lw	$3,16($sp)
	li	$2,65536			# 0x10000
	addu	$2,$16,$2
	sw	$3,-30692($2)
	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	lw	$2,-30692($2)
	sw	$2,156($sp)
	lw	$2,156($sp)
	sltu	$2,$2,-4095
	xori	$2,$2,0x1
	andi	$2,$2,0x00ff
	beq	$2,$0,$L65
	nop

	lui	$2,%hi($LC1)
	addiu	$4,$2,%lo($LC1)
	lui	$2,%hi(__func__.33756)
	addiu	$5,$2,%lo(__func__.33756)
	li	$6,174			# 0xae
	lui	$2,%hi(panic)
	addiu	$2,$2,%lo(panic)
	jalr	$2
	nop

$L65:
	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	sw	$0,-30416($2)
	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$3,$3,$2
	lbu	$2,-30412($3)
	ins	$2,$0,7,1
	sb	$2,-30412($3)
	lui	$2,%hi(xpondrv_init_phy_data)
	addiu	$2,$2,%lo(xpondrv_init_phy_data)
	jalr	$2
	nop

	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$3,$3,$2
	lbu	$2,-30412($3)
	ins	$2,$0,5,1
	sb	$2,-30412($3)
	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$3,$3,$2
	lbu	$2,-30412($3)
	ins	$2,$0,4,1
	sb	$2,-30412($3)
	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$3,$3,$2
	lbu	$2,-30412($3)
	ins	$2,$0,6,1
	sb	$2,-30412($3)
	lw	$31,164($sp)
	lw	$16,160($sp)
	addiu	$sp,$sp,168
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	xpondrv_init_global_data
	.size	xpondrv_init_global_data, .-xpondrv_init_global_data
	.rdata
	.align	2
$LC2:
	.ascii	"%d\012\000"
	.section	.text.gpon_debug_read_proc,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	gpon_debug_read_proc
	.type	gpon_debug_read_proc, @function
gpon_debug_read_proc:
	.frame	$sp,320,$31		# vars= 296, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-320
	sw	$31,316($sp)
	sw	$4,320($sp)
	sw	$5,324($sp)
	sw	$6,328($sp)
	sw	$7,332($sp)
	sw	$0,16($sp)
	sw	$0,20($sp)
	li	$2,1			# 0x1
	sw	$2,212($sp)
	li	$2,-1078984704			# 0xffffffffbfb00000
	ori	$2,$2,0xf8
	sw	$2,28($sp)
	lw	$2,28($sp)
	lw	$2,0($2)
	andi	$3,$2,0x3
	li	$2,2			# 0x2
	bne	$3,$2,$L68
	nop

	li	$2,-1078984704			# 0xffffffffbfb00000
	ori	$2,$2,0x64
	sw	$2,32($sp)
	lw	$2,32($sp)
	lw	$3,0($2)
	li	$2,-65536			# 0xffffffffffff0000
	and	$3,$3,$2
	li	$2,327680			# 0x50000
	bne	$3,$2,$L68
	nop

	li	$2,-1078001664			# 0xffffffffbfbf0000
	ori	$2,$2,0x8214
	sw	$2,36($sp)
	lw	$2,36($sp)
	lw	$2,0($2)
	andi	$2,$2,0x8
	beq	$2,$0,$L71
	nop

	li	$2,-1078001664			# 0xffffffffbfbf0000
	ori	$2,$2,0x8214
	sw	$2,40($sp)
	lw	$2,40($sp)
	lw	$2,0($2)
	andi	$2,$2,0x40
	bne	$2,$0,$L73
	nop

	j	$L68
	nop

$L71:
	li	$2,-1078001664			# 0xffffffffbfbf0000
	ori	$2,$2,0x8214
	sw	$2,44($sp)
	lw	$2,44($sp)
	lw	$2,0($2)
	andi	$2,$2,0x4
	beq	$2,$0,$L68
	nop

$L73:
	li	$2,1			# 0x1
	j	$L75
	nop

$L68:
	move	$2,$0
$L75:
	sw	$2,216($sp)
	li	$2,2			# 0x2
	sw	$2,220($sp)
	li	$2,-1078984704			# 0xffffffffbfb00000
	ori	$2,$2,0xf8
	sw	$2,48($sp)
	lw	$2,48($sp)
	lw	$2,0($2)
	andi	$3,$2,0x3
	li	$2,2			# 0x2
	bne	$3,$2,$L77
	nop

	li	$2,-1078984704			# 0xffffffffbfb00000
	ori	$2,$2,0x64
	sw	$2,52($sp)
	lw	$2,52($sp)
	lw	$3,0($2)
	li	$2,-65536			# 0xffffffffffff0000
	and	$3,$3,$2
	li	$2,327680			# 0x50000
	bne	$3,$2,$L77
	nop

	li	$2,-1078001664			# 0xffffffffbfbf0000
	ori	$2,$2,0x8214
	sw	$2,56($sp)
	lw	$2,56($sp)
	lw	$2,0($2)
	andi	$2,$2,0x8
	beq	$2,$0,$L80
	nop

	li	$2,-1078001664			# 0xffffffffbfbf0000
	ori	$2,$2,0x8214
	sw	$2,60($sp)
	lw	$2,60($sp)
	lw	$2,0($2)
	andi	$2,$2,0x20
	bne	$2,$0,$L82
	nop

	j	$L77
	nop

$L80:
	li	$2,-1078001664			# 0xffffffffbfbf0000
	ori	$2,$2,0x8214
	sw	$2,64($sp)
	lw	$2,64($sp)
	lw	$2,0($2)
	andi	$2,$2,0x2
	beq	$2,$0,$L77
	nop

$L82:
	li	$2,-1078001664			# 0xffffffffbfbf0000
	ori	$2,$2,0x8214
	sw	$2,68($sp)
	lw	$2,68($sp)
	lw	$2,0($2)
	andi	$2,$2,0x8
	beq	$2,$0,$L85
	nop

	li	$2,-1078001664			# 0xffffffffbfbf0000
	ori	$2,$2,0x8214
	sw	$2,72($sp)
	lw	$2,72($sp)
	lw	$2,0($2)
	andi	$2,$2,0x40
	beq	$2,$0,$L87
	nop

	j	$L77
	nop

$L85:
	li	$2,-1078001664			# 0xffffffffbfbf0000
	ori	$2,$2,0x8214
	sw	$2,76($sp)
	lw	$2,76($sp)
	lw	$2,0($2)
	andi	$2,$2,0x4
	bne	$2,$0,$L77
	nop

$L87:
	li	$2,1			# 0x1
	j	$L89
	nop

$L77:
	move	$2,$0
$L89:
	sw	$2,224($sp)
	li	$2,3			# 0x3
	sw	$2,228($sp)
	li	$2,-1078984704			# 0xffffffffbfb00000
	ori	$2,$2,0xf8
	sw	$2,80($sp)
	lw	$2,80($sp)
	lw	$2,0($2)
	andi	$3,$2,0x3
	li	$2,3			# 0x3
	bne	$3,$2,$L91
	nop

	li	$2,-1078984704			# 0xffffffffbfb00000
	ori	$2,$2,0x64
	sw	$2,84($sp)
	lw	$2,84($sp)
	lw	$3,0($2)
	li	$2,-65536			# 0xffffffffffff0000
	and	$3,$3,$2
	li	$2,327680			# 0x50000
	bne	$3,$2,$L91
	nop

	li	$2,-1078001664			# 0xffffffffbfbf0000
	ori	$2,$2,0x8214
	sw	$2,88($sp)
	lw	$2,88($sp)
	lw	$2,0($2)
	andi	$2,$2,0x8
	beq	$2,$0,$L94
	nop

	li	$2,-1078001664			# 0xffffffffbfbf0000
	ori	$2,$2,0x8214
	sw	$2,92($sp)
	lw	$2,92($sp)
	lw	$2,0($2)
	andi	$2,$2,0x20
	bne	$2,$0,$L96
	nop

	j	$L91
	nop

$L94:
	li	$2,-1078001664			# 0xffffffffbfbf0000
	ori	$2,$2,0x8214
	sw	$2,96($sp)
	lw	$2,96($sp)
	lw	$2,0($2)
	andi	$2,$2,0x2
	beq	$2,$0,$L91
	nop

$L96:
	li	$2,1			# 0x1
	j	$L98
	nop

$L91:
	move	$2,$0
$L98:
	sw	$2,232($sp)
	li	$2,4			# 0x4
	sw	$2,236($sp)
	li	$2,-1078984704			# 0xffffffffbfb00000
	ori	$2,$2,0xf8
	sw	$2,100($sp)
	lw	$2,100($sp)
	lw	$2,0($2)
	andi	$3,$2,0x3
	li	$2,2			# 0x2
	bne	$3,$2,$L100
	nop

	li	$2,-1078984704			# 0xffffffffbfb00000
	ori	$2,$2,0x64
	sw	$2,104($sp)
	lw	$2,104($sp)
	lw	$3,0($2)
	li	$2,-65536			# 0xffffffffffff0000
	and	$3,$3,$2
	li	$2,327680			# 0x50000
	bne	$3,$2,$L100
	nop

	li	$2,-1078001664			# 0xffffffffbfbf0000
	ori	$2,$2,0x8214
	sw	$2,108($sp)
	lw	$2,108($sp)
	lw	$2,0($2)
	andi	$2,$2,0x8
	beq	$2,$0,$L103
	nop

	li	$2,-1078001664			# 0xffffffffbfbf0000
	ori	$2,$2,0x8214
	sw	$2,112($sp)
	lw	$2,112($sp)
	lw	$2,0($2)
	andi	$2,$2,0x20
	beq	$2,$0,$L105
	nop

	j	$L100
	nop

$L103:
	li	$2,-1078001664			# 0xffffffffbfbf0000
	ori	$2,$2,0x8214
	sw	$2,116($sp)
	lw	$2,116($sp)
	lw	$2,0($2)
	andi	$2,$2,0x2
	bne	$2,$0,$L100
	nop

$L105:
	li	$2,-1078001664			# 0xffffffffbfbf0000
	ori	$2,$2,0x8214
	sw	$2,120($sp)
	lw	$2,120($sp)
	lw	$2,0($2)
	andi	$2,$2,0x8
	beq	$2,$0,$L108
	nop

	li	$2,-1078001664			# 0xffffffffbfbf0000
	ori	$2,$2,0x8214
	sw	$2,124($sp)
	lw	$2,124($sp)
	lw	$2,0($2)
	andi	$2,$2,0x40
	beq	$2,$0,$L110
	nop

	j	$L100
	nop

$L108:
	li	$2,-1078001664			# 0xffffffffbfbf0000
	ori	$2,$2,0x8214
	sw	$2,128($sp)
	lw	$2,128($sp)
	lw	$2,0($2)
	andi	$2,$2,0x4
	bne	$2,$0,$L100
	nop

$L110:
	li	$2,1			# 0x1
	j	$L112
	nop

$L100:
	move	$2,$0
$L112:
	sw	$2,240($sp)
	li	$2,5			# 0x5
	sw	$2,244($sp)
	li	$2,-1078984704			# 0xffffffffbfb00000
	ori	$2,$2,0xf8
	sw	$2,132($sp)
	lw	$2,132($sp)
	lw	$2,0($2)
	andi	$3,$2,0x3
	li	$2,3			# 0x3
	bne	$3,$2,$L114
	nop

	li	$2,-1078984704			# 0xffffffffbfb00000
	ori	$2,$2,0x64
	sw	$2,136($sp)
	lw	$2,136($sp)
	lw	$3,0($2)
	li	$2,-65536			# 0xffffffffffff0000
	and	$3,$3,$2
	li	$2,327680			# 0x50000
	bne	$3,$2,$L114
	nop

	li	$2,-1078001664			# 0xffffffffbfbf0000
	ori	$2,$2,0x8214
	sw	$2,140($sp)
	lw	$2,140($sp)
	lw	$2,0($2)
	andi	$2,$2,0x8
	beq	$2,$0,$L117
	nop

	li	$2,-1078001664			# 0xffffffffbfbf0000
	ori	$2,$2,0x8214
	sw	$2,144($sp)
	lw	$2,144($sp)
	lw	$2,0($2)
	andi	$2,$2,0x20
	beq	$2,$0,$L119
	nop

	j	$L114
	nop

$L117:
	li	$2,-1078001664			# 0xffffffffbfbf0000
	ori	$2,$2,0x8214
	sw	$2,148($sp)
	lw	$2,148($sp)
	lw	$2,0($2)
	andi	$2,$2,0x2
	bne	$2,$0,$L114
	nop

$L119:
	li	$2,1			# 0x1
	j	$L121
	nop

$L114:
	move	$2,$0
$L121:
	sw	$2,248($sp)
	li	$2,6			# 0x6
	sw	$2,252($sp)
	li	$2,-1078984704			# 0xffffffffbfb00000
	ori	$2,$2,0x64
	lw	$3,0($2)
	li	$2,-65536			# 0xffffffffffff0000
	and	$3,$3,$2
	li	$2,458752			# 0x70000
	beq	$3,$2,$L122
	nop

	li	$2,-1078984704			# 0xffffffffbfb00000
	ori	$2,$2,0x64
	lw	$3,0($2)
	li	$2,-65536			# 0xffffffffffff0000
	and	$3,$3,$2
	li	$2,524288			# 0x80000
	bne	$3,$2,$L123
	nop

$L122:
	li	$2,-1078001664			# 0xffffffffbfbf0000
	ori	$2,$2,0x8214
	sw	$2,152($sp)
	lw	$2,152($sp)
	lw	$2,0($2)
	andi	$2,$2,0x40
	beq	$2,$0,$L125
	nop

	li	$2,-1078001664			# 0xffffffffbfbf0000
	ori	$2,$2,0x8214
	sw	$2,156($sp)
	lw	$2,156($sp)
	lw	$2,0($2)
	srl	$2,$2,7
	andi	$3,$2,0x3f
	li	$2,32			# 0x20
	beq	$3,$2,$L127
	nop

	j	$L123
	nop

$L125:
	li	$2,-1078001664			# 0xffffffffbfbf0000
	ori	$2,$2,0x8214
	sw	$2,160($sp)
	lw	$2,160($sp)
	lw	$2,0($2)
	andi	$3,$2,0x3f
	li	$2,32			# 0x20
	bne	$3,$2,$L123
	nop

$L127:
	li	$2,1			# 0x1
	j	$L129
	nop

$L123:
	move	$2,$0
$L129:
	sw	$2,256($sp)
	li	$2,7			# 0x7
	sw	$2,260($sp)
	li	$2,-1078984704			# 0xffffffffbfb00000
	ori	$2,$2,0x64
	lw	$3,0($2)
	li	$2,-65536			# 0xffffffffffff0000
	and	$3,$3,$2
	li	$2,458752			# 0x70000
	beq	$3,$2,$L130
	nop

	li	$2,-1078984704			# 0xffffffffbfb00000
	ori	$2,$2,0x64
	lw	$3,0($2)
	li	$2,-65536			# 0xffffffffffff0000
	and	$3,$3,$2
	li	$2,524288			# 0x80000
	bne	$3,$2,$L131
	nop

$L130:
	li	$2,-1078001664			# 0xffffffffbfbf0000
	ori	$2,$2,0x8214
	sw	$2,164($sp)
	lw	$2,164($sp)
	lw	$2,0($2)
	andi	$2,$2,0x40
	beq	$2,$0,$L133
	nop

	li	$2,-1078001664			# 0xffffffffbfbf0000
	ori	$2,$2,0x8214
	sw	$2,168($sp)
	lw	$2,168($sp)
	lw	$2,0($2)
	srl	$2,$2,7
	andi	$3,$2,0x3f
	li	$2,16			# 0x10
	beq	$3,$2,$L135
	nop

	j	$L131
	nop

$L133:
	li	$2,-1078001664			# 0xffffffffbfbf0000
	ori	$2,$2,0x8214
	sw	$2,172($sp)
	lw	$2,172($sp)
	lw	$2,0($2)
	andi	$3,$2,0x3f
	li	$2,16			# 0x10
	bne	$3,$2,$L131
	nop

$L135:
	li	$2,1			# 0x1
	j	$L137
	nop

$L131:
	move	$2,$0
$L137:
	sw	$2,264($sp)
	li	$2,8			# 0x8
	sw	$2,268($sp)
	li	$2,-1078984704			# 0xffffffffbfb00000
	ori	$2,$2,0x64
	lw	$3,0($2)
	li	$2,-65536			# 0xffffffffffff0000
	and	$3,$3,$2
	li	$2,458752			# 0x70000
	beq	$3,$2,$L138
	nop

	li	$2,-1078984704			# 0xffffffffbfb00000
	ori	$2,$2,0x64
	lw	$3,0($2)
	li	$2,-65536			# 0xffffffffffff0000
	and	$3,$3,$2
	li	$2,524288			# 0x80000
	bne	$3,$2,$L139
	nop

$L138:
	li	$2,-1078001664			# 0xffffffffbfbf0000
	ori	$2,$2,0x8214
	sw	$2,176($sp)
	lw	$2,176($sp)
	lw	$2,0($2)
	andi	$2,$2,0x40
	beq	$2,$0,$L141
	nop

	li	$2,-1078001664			# 0xffffffffbfbf0000
	ori	$2,$2,0x8214
	sw	$2,180($sp)
	lw	$2,180($sp)
	lw	$2,0($2)
	srl	$2,$2,7
	andi	$2,$2,0x3f
	beq	$2,$0,$L143
	nop

	j	$L139
	nop

$L141:
	li	$2,-1078001664			# 0xffffffffbfbf0000
	ori	$2,$2,0x8214
	sw	$2,184($sp)
	lw	$2,184($sp)
	lw	$2,0($2)
	andi	$2,$2,0x3f
	bne	$2,$0,$L139
	nop

$L143:
	li	$2,1			# 0x1
	j	$L145
	nop

$L139:
	move	$2,$0
$L145:
	sw	$2,272($sp)
	li	$2,9			# 0x9
	sw	$2,276($sp)
	li	$2,-1078984704			# 0xffffffffbfb00000
	ori	$2,$2,0x64
	lw	$3,0($2)
	li	$2,-65536			# 0xffffffffffff0000
	and	$3,$3,$2
	li	$2,458752			# 0x70000
	beq	$3,$2,$L146
	nop

	li	$2,-1078984704			# 0xffffffffbfb00000
	ori	$2,$2,0x64
	lw	$3,0($2)
	li	$2,-65536			# 0xffffffffffff0000
	and	$3,$3,$2
	li	$2,524288			# 0x80000
	bne	$3,$2,$L147
	nop

$L146:
	li	$2,-1078001664			# 0xffffffffbfbf0000
	ori	$2,$2,0x8214
	sw	$2,188($sp)
	lw	$2,188($sp)
	lw	$2,0($2)
	andi	$2,$2,0x40
	beq	$2,$0,$L149
	nop

	li	$2,-1078001664			# 0xffffffffbfbf0000
	ori	$2,$2,0x8214
	sw	$2,192($sp)
	lw	$2,192($sp)
	lw	$2,0($2)
	srl	$2,$2,7
	andi	$3,$2,0x3f
	li	$2,1			# 0x1
	beq	$3,$2,$L151
	nop

	j	$L147
	nop

$L149:
	li	$2,-1078001664			# 0xffffffffbfbf0000
	ori	$2,$2,0x8214
	sw	$2,196($sp)
	lw	$2,196($sp)
	lw	$2,0($2)
	andi	$3,$2,0x3f
	li	$2,1			# 0x1
	bne	$3,$2,$L147
	nop

$L151:
	li	$2,1			# 0x1
	j	$L153
	nop

$L147:
	move	$2,$0
$L153:
	sw	$2,280($sp)
	li	$2,10			# 0xa
	sw	$2,284($sp)
	li	$2,-1078984704			# 0xffffffffbfb00000
	ori	$2,$2,0x64
	lw	$3,0($2)
	li	$2,-65536			# 0xffffffffffff0000
	and	$3,$3,$2
	li	$2,458752			# 0x70000
	beq	$3,$2,$L154
	nop

	li	$2,-1078984704			# 0xffffffffbfb00000
	ori	$2,$2,0x64
	lw	$3,0($2)
	li	$2,-65536			# 0xffffffffffff0000
	and	$3,$3,$2
	li	$2,524288			# 0x80000
	bne	$3,$2,$L155
	nop

$L154:
	li	$2,-1078001664			# 0xffffffffbfbf0000
	ori	$2,$2,0x8214
	sw	$2,200($sp)
	lw	$2,200($sp)
	lw	$2,0($2)
	andi	$2,$2,0x40
	beq	$2,$0,$L157
	nop

	li	$2,-1078001664			# 0xffffffffbfbf0000
	ori	$2,$2,0x8214
	sw	$2,204($sp)
	lw	$2,204($sp)
	lw	$2,0($2)
	srl	$2,$2,7
	andi	$3,$2,0x3f
	li	$2,2			# 0x2
	beq	$3,$2,$L159
	nop

	j	$L155
	nop

$L157:
	li	$2,-1078001664			# 0xffffffffbfbf0000
	ori	$2,$2,0x8214
	sw	$2,208($sp)
	lw	$2,208($sp)
	lw	$2,0($2)
	andi	$3,$2,0x3f
	li	$2,2			# 0x2
	bne	$3,$2,$L155
	nop

$L159:
	li	$2,1			# 0x1
	j	$L161
	nop

$L155:
	move	$2,$0
$L161:
	sw	$2,288($sp)
	li	$2,11			# 0xb
	sw	$2,292($sp)
	li	$2,-1078984704			# 0xffffffffbfb00000
	ori	$2,$2,0x64
	lw	$3,0($2)
	li	$2,-65536			# 0xffffffffffff0000
	and	$3,$3,$2
	li	$2,458752			# 0x70000
	beq	$3,$2,$L162
	nop

	li	$2,-1078984704			# 0xffffffffbfb00000
	ori	$2,$2,0x64
	lw	$3,0($2)
	li	$2,-65536			# 0xffffffffffff0000
	and	$3,$3,$2
	li	$2,524288			# 0x80000
	bne	$3,$2,$L163
	nop

$L162:
	li	$2,1			# 0x1
	j	$L164
	nop

$L163:
	move	$2,$0
$L164:
	sw	$2,296($sp)
	sw	$0,300($sp)
	sw	$0,304($sp)
	sw	$0,24($sp)
	j	$L165
	nop

$L168:
	lw	$2,24($sp)
	sll	$2,$2,3
	addiu	$3,$sp,16
	addu	$2,$3,$2
	lw	$2,200($2)
	beq	$2,$0,$L166
	nop

	lw	$2,24($sp)
	sll	$2,$2,3
	addiu	$3,$sp,16
	addu	$2,$3,$2
	lw	$2,196($2)
	sw	$2,20($sp)
	j	$L167
	nop

$L166:
	lw	$2,24($sp)
	addiu	$2,$2,1
	sw	$2,24($sp)
$L165:
	lw	$2,24($sp)
	sltu	$2,$2,12
	bne	$2,$0,$L168
	nop

$L167:
	lw	$4,320($sp)
	lui	$2,%hi($LC2)
	addiu	$5,$2,%lo($LC2)
	lw	$6,20($sp)
	lui	$2,%hi(sprintf)
	addiu	$2,$2,%lo(sprintf)
	jalr	$2
	nop

	sw	$2,16($sp)
	lw	$3,16($sp)
	lw	$2,328($sp)
	subu	$2,$3,$2
	sw	$2,16($sp)
	lw	$2,328($sp)
	lw	$3,320($sp)
	addu	$3,$3,$2
	lw	$2,324($sp)
	sw	$3,0($2)
	lw	$3,16($sp)
	lw	$2,332($sp)
	slt	$2,$2,$3
	beq	$2,$0,$L169
	nop

	lw	$2,332($sp)
	sw	$2,16($sp)
	j	$L170
	nop

$L169:
	lw	$2,336($sp)
	li	$3,1			# 0x1
	sw	$3,0($2)
$L170:
	lw	$2,16($sp)
	bgez	$2,$L171
	nop

	sw	$0,16($sp)
$L171:
	lw	$2,16($sp)
	lw	$31,316($sp)
	addiu	$sp,$sp,320
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	gpon_debug_read_proc
	.size	gpon_debug_read_proc, .-gpon_debug_read_proc
	.globl	reiSeq
	.section	.bss
	.type	reiSeq, @object
	.size	reiSeq, 1
reiSeq:
	.space	1
	.globl	attackEnhance
	.type	attackEnhance, @object
	.size	attackEnhance, 1
attackEnhance:
	.space	1
	.globl	dropThred
	.data
	.align	2
	.type	dropThred, @object
	.size	dropThred, 4
dropThred:
	.word	500
	.globl	rxDropCheckTimer
	.type	rxDropCheckTimer, @object
	.size	rxDropCheckTimer, 1
rxDropCheckTimer:
	.byte	5
	.rdata
	.align	2
$LC3:
	.ascii	"\015\012s is NULL ====> atoi in xpon drv\000"
	.section	.text.atoi_temp,"ax",@progbits
	.align	2
	.globl	atoi_temp
	.set	nomips16
	.set	nomicromips
	.ent	atoi_temp
	.type	atoi_temp, @function
atoi_temp:
	.frame	$sp,32,$31		# vars= 8, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-32
	sw	$31,28($sp)
	sw	$4,32($sp)
	sw	$0,16($sp)
	lw	$2,32($sp)
	bne	$2,$0,$L174
	nop

	lui	$2,%hi($LC3)
	addiu	$4,$2,%lo($LC3)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	li	$2,-1			# 0xffffffffffffffff
	j	$L175
	nop

$L174:
	j	$L176
	nop

$L178:
	lw	$2,16($sp)
	sll	$2,$2,1
	sll	$3,$2,2
	addu	$3,$2,$3
	lw	$2,32($sp)
	addiu	$4,$2,1
	sw	$4,32($sp)
	lb	$2,0($2)
	addu	$2,$3,$2
	addiu	$2,$2,-48
	sw	$2,16($sp)
$L176:
	lw	$2,32($sp)
	lb	$2,0($2)
	slt	$2,$2,48
	bne	$2,$0,$L177
	nop

	lw	$2,32($sp)
	lb	$2,0($2)
	slt	$2,$2,58
	bne	$2,$0,$L178
	nop

$L177:
	lw	$2,16($sp)
$L175:
	lw	$31,28($sp)
	addiu	$sp,$sp,32
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	atoi_temp
	.size	atoi_temp, .-atoi_temp
	.rdata
	.align	2
$LC4:
	.ascii	"oam\000"
	.section	.text.xmit_dying_gasp_oam,"ax",@progbits
	.align	2
	.globl	xmit_dying_gasp_oam
	.set	nomips16
	.set	nomicromips
	.ent	xmit_dying_gasp_oam
	.type	xmit_dying_gasp_oam, @function
xmit_dying_gasp_oam:
	.frame	$sp,48,$31		# vars= 24, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	addiu	$sp,$sp,-48
	sw	$31,44($sp)
	sw	$0,16($sp)
	sw	$0,20($sp)
	lui	$2,%hi(init_net)
	addiu	$4,$2,%lo(init_net)
	lui	$2,%hi($LC4)
	addiu	$5,$2,%lo($LC4)
	lui	$2,%hi(dev_get_by_name)
	addiu	$2,$2,%lo(dev_get_by_name)
	jalr	$2
	sw	$2,20($sp)
	lw	$2,20($sp)
	bne	$2,$0,$L180
	j	$L181
$L180:
	lui	$2,%hi(skbmgr_alloc_skb2k)
	addiu	$2,$2,%lo(skbmgr_alloc_skb2k)
	jalr	$2
	sw	$2,16($sp)
	lw	$2,16($sp)
	sltu	$2,$2,1
	andi	$2,$2,0x00ff
	beq	$2,$0,$L182
	j	$L181
$L182:
	lui	$2,%hi(GetMacAddr)
	addiu	$2,$2,%lo(GetMacAddr)
	jalr	$2
	move	$3,$2
	lui	$2,%hi(dying_gasp_oam+8)
	addiu	$4,$2,%lo(dying_gasp_oam+8)
	move	$5,$3
	li	$6,6			# 0x6
	lui	$2,%hi(memcpy)
	addiu	$2,$2,%lo(memcpy)
	jalr	$2
	lw	$2,16($sp)
	lw	$2,256($2)
	move	$4,$2
	lui	$2,%hi(dying_gasp_oam)
	addiu	$5,$2,%lo(dying_gasp_oam)
	li	$6,58			# 0x3a
	lui	$2,%hi(memcpy)
	addiu	$2,$2,%lo(memcpy)
	jalr	$2
	lw	$4,16($sp)
	li	$5,62			# 0x3e
	lui	$2,%hi(skb_put)
	addiu	$2,$2,%lo(skb_put)
	jalr	$2
	lw	$4,16($sp)
	lw	$5,20($sp)
	lui	$2,%hi(pwan_net_start_xmit)
	addiu	$2,$2,%lo(pwan_net_start_xmit)
	jalr	$2
	lw	$2,20($sp)
	sw	$2,24($sp)
	lw	$2,24($sp)
	addiu	$2,$2,608
	li	$3,1			# 0x1
	sw	$3,28($sp)
	sw	$2,32($sp)
	lw	$4,32($sp)
	lw	$3,28($sp)
	lw	$2,32($sp)
#APP
 # 112 "/opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/atomic.h" 1
		.set	mips3					
1:	ll	$5, 0($4)		# atomic_sub		
	subu	$5, $3					
	sc	$5, 0($4)					
	beqz	$5, 2f					
	.subsection 2					
2:	b	1b					
	.previous					
	.set	mips0					

 # 0 "" 2
#NO_APP
	move	$2,$5
	sw	$2,36($sp)
$L181:
	.set	noreorder
	nop
	.set	reorder
	lw	$31,44($sp)
	addiu	$sp,$sp,48
	j	$31
	.end	xmit_dying_gasp_oam
	.size	xmit_dying_gasp_oam, .-xmit_dying_gasp_oam
	.globl	drop_print_flag
	.section	.bss
	.align	2
	.type	drop_print_flag, @object
	.size	drop_print_flag, 4
drop_print_flag:
	.space	4
	.globl	xpon_mac_print_open
	.align	2
	.type	xpon_mac_print_open, @object
	.size	xpon_mac_print_open, 4
xpon_mac_print_open:
	.space	4
	.rdata
	.align	2
$LC5:
	.ascii	"%s %s %x\000"
	.align	2
$LC6:
	.ascii	"show_flag\000"
	.align	2
$LC7:
	.ascii	"sysLinkStatus:\011 %d\012\000"
	.align	2
$LC8:
	.ascii	"sysStartup:\011 %d\012\000"
	.align	2
$LC9:
	.ascii	"drop_print_flag:\011 %d\012\000"
	.align	2
$LC10:
	.ascii	"xpon_mac_print_open:\011 %d\012\000"
	.align	2
$LC11:
	.ascii	"set\000"
	.align	2
$LC12:
	.ascii	"rx_ratelimit\000"
	.align	2
$LC13:
	.ascii	"set rx ratelimit %d FPS success\012\000"
	.align	2
$LC14:
	.ascii	"gpon_recovery_dbg\000"
	.align	2
$LC15:
	.ascii	"1\000"
	.align	2
$LC16:
	.ascii	"olttype\000"
	.align	2
$LC17:
	.ascii	"set olt type: %d\012\000"
	.align	2
$LC18:
	.ascii	"get olt type: %d\012\000"
	.align	2
$LC19:
	.ascii	"pwan\000"
	.align	2
$LC20:
	.ascii	"qos\000"
	.align	2
$LC21:
	.ascii	"mapqos\000"
	.align	2
$LC22:
	.ascii	"xPon: Qos is aleady up.\012\000"
	.align	2
$LC23:
	.ascii	"drop\000"
	.align	2
$LC24:
	.ascii	"Set TxDropTimer = %u\012\000"
	.align	2
$LC25:
	.ascii	"epon\000"
	.align	2
$LC26:
	.ascii	"staticrpt\000"
	.align	2
$LC27:
	.ascii	"Disable\000"
	.align	2
$LC28:
	.ascii	"Enable\000"
	.align	2
$LC29:
	.ascii	"%s epon static report!\012\000"
	.align	2
$LC30:
	.ascii	"dying\000"
	.align	2
$LC31:
	.ascii	"The dying gasp message will be send by software\012\000"
	.align	2
$LC32:
	.ascii	"The dying gasp message will be send by hardware\012\000"
	.align	2
$LC33:
	.ascii	"txoamfavor\000"
	.align	2
$LC34:
	.ascii	"Tx OAM Favor mode is disabled\012\000"
	.align	2
$LC35:
	.ascii	"Tx OAM Favor mode is enabled\012\000"
	.align	2
$LC36:
	.ascii	"Enable the attack enhance function!\012\000"
	.align	2
$LC37:
	.ascii	"Disable the attack enhance function!\012\000"
	.align	2
$LC38:
	.ascii	"dropThreld\000"
	.align	2
$LC39:
	.ascii	"The threld for drop packets is 0x%08lx\000"
	.align	2
$LC40:
	.ascii	"dropTimer\000"
	.align	2
$LC41:
	.ascii	"The rx drop check timer is 0x%08lx\000"
	.align	2
$LC42:
	.ascii	"msg\000"
	.align	2
$LC43:
	.ascii	"act\000"
	.align	2
$LC44:
	.ascii	"omci\000"
	.align	2
$LC45:
	.ascii	"int\000"
	.align	2
$LC46:
	.ascii	"trace\000"
	.align	2
$LC47:
	.ascii	"warning\000"
	.align	2
$LC48:
	.ascii	"content\000"
	.align	2
$LC49:
	.ascii	"debug\000"
	.align	2
$LC50:
	.ascii	"err\000"
	.align	2
$LC51:
	.ascii	"eqd\000"
	.align	2
$LC52:
	.ascii	"xmcs\000"
	.align	2
$LC53:
	.ascii	"Debug Level: %x\012\000"
	.align	2
$LC54:
	.ascii	"csr\000"
	.align	2
$LC55:
	.ascii	"gem\000"
	.align	2
$LC56:
	.ascii	"tcont\000"
	.align	2
$LC57:
	.ascii	"Channel 0 valid, ID:%d\012\000"
	.align	2
$LC58:
	.ascii	"Channel 1 valid, ID:%d\012\000"
	.align	2
$LC59:
	.ascii	"Channel 2 valid, ID:%d\012\000"
	.align	2
$LC60:
	.ascii	"Channel 3 valid, ID:%d\012\000"
	.align	2
$LC61:
	.ascii	"Channel 4 valid, ID:%d\012\000"
	.align	2
$LC62:
	.ascii	"Channel 5 valid, ID:%d\012\000"
	.align	2
$LC63:
	.ascii	"Channel 6 valid, ID:%d\012\000"
	.align	2
$LC64:
	.ascii	"Channel 7 valid, ID:%d\012\000"
	.align	2
$LC65:
	.ascii	"Channel 8 valid, ID:%d\012\000"
	.align	2
$LC66:
	.ascii	"Channel 9 valid, ID:%d\012\000"
	.align	2
$LC67:
	.ascii	"Channel 10 valid, ID:%d\012\000"
	.align	2
$LC68:
	.ascii	"Channel 11 valid, ID:%d\012\000"
	.align	2
$LC69:
	.ascii	"Channel 12 valid, ID:%d\012\000"
	.align	2
$LC70:
	.ascii	"Channel 13 valid, ID:%d\012\000"
	.align	2
$LC71:
	.ascii	"Channel 14 valid, ID:%d\012\000"
	.align	2
$LC72:
	.ascii	"Channel 15 valid, ID:%d\012\000"
	.align	2
$LC73:
	.ascii	"test\000"
	.align	2
$LC74:
	.ascii	"loopback\000"
	.align	2
$LC75:
	.ascii	"Test Loopback Mode: %d\012\000"
	.align	2
$LC76:
	.ascii	"gpon\000"
	.align	2
$LC77:
	.ascii	"Modify the EqD to %d in O5 state\012\000"
	.align	2
$LC78:
	.ascii	"eqd_dis\000"
	.align	2
$LC79:
	.ascii	"\012Disable Ranging in O5!\012\000"
	.align	2
$LC80:
	.ascii	"\012Enable Ranging in O5!\012\000"
	.align	2
$LC81:
	.ascii	"report\000"
	.align	2
$LC82:
	.ascii	"eqdOffsetFlag\000"
	.align	2
$LC83:
	.ascii	"Subtract\000"
	.align	2
$LC84:
	.ascii	"Add\000"
	.align	2
$LC85:
	.ascii	"from\000"
	.align	2
$LC86:
	.ascii	"to\000"
	.align	2
$LC87:
	.ascii	"%s eqdOffset %s the EqD\012\000"
	.align	2
$LC88:
	.ascii	"eqdO4Offset\000"
	.align	2
$LC89:
	.ascii	"Modify the EqD offset = %x at ranging_time PLOAM in O4\012"
	.ascii	"\000"
	.align	2
$LC90:
	.ascii	"eqdO5Offset\000"
	.align	2
$LC91:
	.ascii	"Modify the EqD offset = %x in O5\012\000"
	.align	2
$LC92:
	.ascii	"connection\000"
	.align	2
$LC93:
	.ascii	"Finish to test the gpon ONU connection.\012\000"
	.align	2
$LC94:
	.ascii	"hotplug\000"
	.align	2
$LC95:
	.ascii	"Finish to test the gpon ONU hotplug connection.\012\000"
	.align	2
$LC96:
	.ascii	"ploam\000"
	.align	2
$LC97:
	.ascii	"sn\000"
	.align	2
$LC98:
	.ascii	"passwd\000"
	.align	2
$LC99:
	.ascii	"Password: %s\012\000"
	.align	2
$LC100:
	.ascii	"show\000"
	.align	2
$LC101:
	.ascii	"xpon_print\000"
	.align	2
$LC102:
	.ascii	"pwan_drop\000"
	.align	2
$LC103:
	.ascii	"exec failed\000"
	.align	2
$LC104:
	.ascii	"ENABLE\000"
	.align	2
$LC105:
	.ascii	"DISABLE\000"
	.align	2
$LC106:
	.ascii	"GEM ID:%d, Alloc ID:%d, If Idx:%d, Loopback:%s, Encrypti"
	.ascii	"on:%s, valid is %d, chn:%d\012\000"
	.align	2
$LC107:
	.ascii	"llid\000"
	.align	2
$LC108:
	.ascii	"idx = %d, llid = %d, Channel = %d, rxMode = %d, txMode ="
	.ascii	" %d.\012\000"
	.align	2
$LC109:
	.ascii	"TCONT ID:%d, Channel:%d\012\000"
	.align	2
$LC110:
	.ascii	"superframe\000"
	.align	2
$LC111:
	.ascii	"Super frame Counter = %x\012\000"
	.align	2
$LC112:
	.ascii	"status\000"
	.align	2
$LC113:
	.ascii	"ONU Info:\012\000"
	.align	2
$LC114:
	.ascii	"ONU ID: \011%d\012\000"
	.align	2
$LC115:
	.ascii	"ONU State: \011%d\012\000"
	.align	2
$LC116:
	.ascii	"SN: \011\011\000"
	.align	2
$LC117:
	.ascii	"%c\000"
	.align	2
$LC118:
	.ascii	"%.2x\000"
	.align	2
$LC119:
	.ascii	"\012PASSWD: \011%s\012\000"
	.align	2
$LC120:
	.ascii	"Key Idx: \011%d\012\000"
	.align	2
$LC121:
	.ascii	"Key: \011\011\000"
	.align	2
$LC122:
	.ascii	"\012O1 Timer: \011%d\012\000"
	.align	2
$LC123:
	.ascii	"O2 Timer: \011%d\012\000"
	.align	2
$LC124:
	.ascii	"OMCC ID: \011%d\012\000"
	.align	2
$LC125:
	.ascii	"counter\000"
	.align	2
$LC126:
	.ascii	"Gem Port:%d, SW Tx:%u, SW Rx:%u, HW Tx:%u, HW Rx:%u\012\000"
	.align	2
$LC127:
	.ascii	"gemcounter\000"
	.align	2
$LC128:
	.ascii	"Gem Port:%d\012\000"
	.align	2
$LC129:
	.ascii	"GemFrame Rx High:%u, GemFrame Rx Low:%u\012\000"
	.align	2
$LC130:
	.ascii	"GemPayload Rx High:%u, GemPayload Rx Low:%u\012\000"
	.align	2
$LC131:
	.ascii	"GemFrame Tx High:%u, GemFrame Tx Low:%u\012\000"
	.align	2
$LC132:
	.ascii	"GemPayload Tx High:%u, GemPayload Tx Low:%u\012\000"
	.align	2
$LC133:
	.ascii	"gpon_recovery_backup\000"
	.align	2
$LC134:
	.ascii	"------ bakAllocId ------\012\000"
	.align	2
$LC135:
	.ascii	"gponRecovery.allocId[%d] = %d\012\000"
	.align	2
$LC136:
	.ascii	"\012\000"
	.align	2
$LC137:
	.ascii	"------ bakGemport ------\012\000"
	.align	2
$LC138:
	.ascii	"gponRecovery.gemPort[%d].allocId = %d\012\000"
	.align	2
$LC139:
	.ascii	"gponRecovery.gemPort[%d].gemPortId = %d\012\000"
	.align	2
$LC140:
	.ascii	"gponRecovery.gemPort[%d].channel = %d\012\000"
	.align	2
$LC141:
	.ascii	"gponRecovery.gemPort[%d].encryption = %d\012\000"
	.align	2
$LC142:
	.ascii	"startup\000"
	.align	2
$LC143:
	.ascii	"Disable GPON ONU\012\000"
	.align	2
$LC144:
	.ascii	"Enable GPON ONU\012\000"
	.align	2
$LC145:
	.ascii	"clear\000"
	.align	2
$LC146:
	.ascii	"Successful to clear the GEM counter\012\000"
	.align	2
$LC147:
	.ascii	"cntreset\000"
	.align	2
$LC148:
	.ascii	"rei\000"
	.align	2
$LC149:
	.ascii	"Send REI PLOAM message successful\012\000"
	.align	2
$LC150:
	.ascii	"responseTime\000"
	.align	2
$LC151:
	.ascii	"Set ONU Response Time to %x\012\000"
	.align	2
$LC152:
	.ascii	"preambleoffset\000"
	.align	2
$LC153:
	.ascii	"Set the O3/O4 type 3 preamble offset %d successful.\012\000"
	.align	2
$LC154:
	.ascii	"preambleFlag\000"
	.align	2
$LC155:
	.ascii	"%s the preambe length over 128 byte\012\000"
	.align	2
$LC156:
	.ascii	"sniffer\000"
	.align	2
$LC157:
	.ascii	"%s sniffer all gemport downstream data\012\000"
	.align	2
$LC158:
	.ascii	"xmit_dying_gasp\000"
	.align	2
$LC159:
	.ascii	"retire\000"
	.align	2
$LC160:
	.ascii	"Successful to set the channel %d retire\012\000"
	.align	2
$LC161:
	.ascii	"Failed to set the channel %d retire\012\000"
	.align	2
$LC162:
	.ascii	"pcp\000"
	.align	2
$LC163:
	.ascii	"auto\000"
	.align	2
$LC164:
	.ascii	"Finished to auto test PCP function\012\000"
	.align	2
$LC165:
	.ascii	"Last PCP check counter is %d, %d\012\000"
	.align	2
$LC166:
	.ascii	"disable\000"
	.align	2
$LC167:
	.ascii	"gdmrx\000"
	.align	2
$LC168:
	.ascii	"cdmrx\000"
	.align	2
$LC169:
	.ascii	"cdmtx\000"
	.align	2
$LC170:
	.ascii	"Successful to set the PCP %.8x\012\000"
	.align	2
$LC171:
	.ascii	"vlan_trans\000"
	.align	2
$LC172:
	.ascii	"laser_time\000"
	.align	2
$LC173:
	.ascii	"epon laser time del %d\012\000"
	.align	2
$LC174:
	.ascii	"set_llid\000"
	.align	2
$LC175:
	.ascii	"epon set llid %d traffic up\012\000"
	.align	2
$LC176:
	.ascii	"silent_time\000"
	.align	2
$LC177:
	.ascii	"%s %s %d\000"
	.align	2
$LC178:
	.ascii	"epon set silent time %d\012\000"
	.align	2
$LC179:
	.ascii	"assign\000"
	.align	2
$LC180:
	.ascii	"gemport\000"
	.align	2
$LC181:
	.ascii	"bip_cnt\000"
	.align	2
$LC182:
	.ascii	"ECNT_HOOK_ERROR occur with cmd_id:0x%x\012\000"
	.align	2
$LC183:
	.ascii	"No such API with type:%d, cmd_id:0x%x\012\000"
	.align	2
$LC184:
	.ascii	"current_bip_cnt_counter = %u, total_bip_cnt_counter = %u"
	.ascii	"\012\000"
	.align	2
$LC185:
	.ascii	"emergence_state\000"
	.align	2
$LC186:
	.ascii	"show GPON emergence_state begin\012\000"
	.align	2
$LC187:
	.ascii	"green_drop\000"
	.align	2
$LC188:
	.ascii	"disable green_drop config \012\000"
	.align	2
$LC189:
	.ascii	"enable\000"
	.align	2
$LC190:
	.ascii	"enable green_drop config \012\000"
	.align	2
$LC191:
	.ascii	"epon_test\000"
	.align	2
$LC192:
	.ascii	"holdoverflag\000"
	.align	2
$LC193:
	.ascii	"hldOverFlag = %d\012\000"
	.align	2
$LC194:
	.ascii	"typeBOnGoing\000"
	.align	2
$LC195:
	.ascii	"typeBOnGoing = %d\012\000"
	.align	2
$LC196:
	.ascii	"time\000"
	.align	2
$LC197:
	.ascii	"hldOverTime = %d\012\000"
	.section	.text.gpon_debug_write_proc,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	gpon_debug_write_proc
	.type	gpon_debug_write_proc, @function
gpon_debug_write_proc:
	.frame	$sp,7032,$31		# vars= 6992, regs= 1/0, args= 32, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	addiu	$sp,$sp,-7032
	sw	$31,7028($sp)
	sw	$4,7032($sp)
	sw	$5,7036($sp)
	sw	$6,7040($sp)
	sw	$7,7044($sp)
	lw	$2,7040($sp)
	sltu	$2,$2,64
	bne	$2,$0,$L185
	li	$2,-22			# 0xffffffffffffffea
	j	$L533
$L185:
	addiu	$2,$sp,1624
	sw	$2,44($sp)
	lw	$2,7036($sp)
	sw	$2,48($sp)
	lw	$2,7040($sp)
	sw	$2,36($sp)
	lw	$2,48($sp)
	sw	$2,52($sp)
	lw	$2,36($sp)
	sw	$2,56($sp)
	move	$2,$28
	lw	$2,24($2)
	sw	$2,60($sp)
	lw	$3,52($sp)
	lw	$2,56($sp)
	addu	$3,$3,$2
	lw	$2,52($sp)
	or	$3,$3,$2
	lw	$2,56($sp)
	or	$3,$3,$2
	lw	$2,60($sp)
	and	$2,$3,$2
	sw	$2,64($sp)
	lw	$2,64($sp)
	sltu	$2,$2,1
	andi	$2,$2,0x00ff
	sltu	$2,$0,$2
	andi	$2,$2,0x00ff
	beq	$2,$0,$L187
	lw	$4,44($sp)
	lw	$5,48($sp)
	lw	$6,36($sp)
#APP
 # 415 "/opt/tclinux_phoenix/modules/private/xpon/src/xpondrv.c" 1
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
	sw	$2,36($sp)
$L187:
	lw	$2,36($sp)
	beq	$2,$0,$L188
	li	$2,-14			# 0xfffffffffffffff2
	j	$L533
$L188:
	addiu	$4,$sp,1624
	addiu	$6,$sp,1688
	addiu	$3,$sp,1720
	addiu	$2,$sp,1752
	sw	$2,16($sp)
	lui	$2,%hi($LC5)
	addiu	$5,$2,%lo($LC5)
	move	$7,$3
	lui	$2,%hi(sscanf)
	addiu	$2,$2,%lo(sscanf)
	jalr	$2
	addiu	$2,$sp,1688
	sw	$2,112($sp)
	lui	$2,%hi($LC6)
	addiu	$2,$2,%lo($LC6)
	sw	$2,116($sp)
	lw	$3,112($sp)
	lw	$2,116($sp)
#APP
 # 77 "/opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/string.h" 1
	.set	noreorder
	.set	noat
	lbu	$4,($3)
1:	lbu	$1,($2)
	addiu	$3,1
	bne	$1,$4,2f
	addiu	$2,1
	bnez	$4,1b
	lbu	$4,($3)
	move	$4,$1
2:	subu	$4,$1
3:	.set	at
	.set	reorder
 # 0 "" 2
#NO_APP
	sw	$3,112($sp)
	sw	$2,116($sp)
	sw	$4,120($sp)
	lw	$2,120($sp)
	bne	$2,$0,$L190
	lui	$2,%hi(gpPonSysData)
	lw	$2,%lo(gpPonSysData)($2)
	lw	$3,0($2)
	lui	$2,%hi($LC7)
	addiu	$4,$2,%lo($LC7)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	lui	$2,%hi(gpPonSysData)
	lw	$2,%lo(gpPonSysData)($2)
	lw	$3,8($2)
	lui	$2,%hi($LC8)
	addiu	$4,$2,%lo($LC8)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	lui	$2,%hi(drop_print_flag)
	lw	$3,%lo(drop_print_flag)($2)
	lui	$2,%hi($LC9)
	addiu	$4,$2,%lo($LC9)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	lui	$2,%hi(xpon_mac_print_open)
	lw	$3,%lo(xpon_mac_print_open)($2)
	lui	$2,%hi($LC10)
	addiu	$4,$2,%lo($LC10)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	j	$L191
$L190:
	addiu	$2,$sp,1688
	sw	$2,124($sp)
	lui	$2,%hi($LC11)
	addiu	$2,$2,%lo($LC11)
	sw	$2,128($sp)
	lw	$3,124($sp)
	lw	$2,128($sp)
#APP
 # 77 "/opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/string.h" 1
	.set	noreorder
	.set	noat
	lbu	$4,($3)
1:	lbu	$1,($2)
	addiu	$3,1
	bne	$1,$4,2f
	addiu	$2,1
	bnez	$4,1b
	lbu	$4,($3)
	move	$4,$1
2:	subu	$4,$1
3:	.set	at
	.set	reorder
 # 0 "" 2
#NO_APP
	sw	$3,124($sp)
	sw	$2,128($sp)
	sw	$4,132($sp)
	lw	$2,132($sp)
	bne	$2,$0,$L193
	addiu	$2,$sp,1720
	sw	$2,136($sp)
	lui	$2,%hi($LC12)
	addiu	$2,$2,%lo($LC12)
	sw	$2,140($sp)
	lw	$3,136($sp)
	lw	$2,140($sp)
#APP
 # 77 "/opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/string.h" 1
	.set	noreorder
	.set	noat
	lbu	$4,($3)
1:	lbu	$1,($2)
	addiu	$3,1
	bne	$1,$4,2f
	addiu	$2,1
	bnez	$4,1b
	lbu	$4,($3)
	move	$4,$1
2:	subu	$4,$1
3:	.set	at
	.set	reorder
 # 0 "" 2
#NO_APP
	sw	$3,136($sp)
	sw	$2,140($sp)
	sw	$4,144($sp)
	lw	$2,144($sp)
	bne	$2,$0,$L191
	lw	$2,1752($sp)
	move	$4,$2
	lui	$2,%hi(gpon_enable_cpu_protection)
	addiu	$2,$2,%lo(gpon_enable_cpu_protection)
	jalr	$2
	lw	$3,1752($sp)
	lui	$2,%hi($LC13)
	addiu	$4,$2,%lo($LC13)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	j	$L191
$L193:
	addiu	$2,$sp,1688
	sw	$2,148($sp)
	lui	$2,%hi($LC14)
	addiu	$2,$2,%lo($LC14)
	sw	$2,152($sp)
	lw	$3,148($sp)
	lw	$2,152($sp)
#APP
 # 77 "/opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/string.h" 1
	.set	noreorder
	.set	noat
	lbu	$4,($3)
1:	lbu	$1,($2)
	addiu	$3,1
	bne	$1,$4,2f
	addiu	$2,1
	bnez	$4,1b
	lbu	$4,($3)
	move	$4,$1
2:	subu	$4,$1
3:	.set	at
	.set	reorder
 # 0 "" 2
#NO_APP
	sw	$3,148($sp)
	sw	$2,152($sp)
	sw	$4,156($sp)
	lw	$2,156($sp)
	bne	$2,$0,$L197
	addiu	$2,$sp,1720
	sw	$2,160($sp)
	lui	$2,%hi($LC15)
	addiu	$2,$2,%lo($LC15)
	sw	$2,164($sp)
	lw	$3,160($sp)
	lw	$2,164($sp)
#APP
 # 77 "/opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/string.h" 1
	.set	noreorder
	.set	noat
	lbu	$4,($3)
1:	lbu	$1,($2)
	addiu	$3,1
	bne	$1,$4,2f
	addiu	$2,1
	bnez	$4,1b
	lbu	$4,($3)
	move	$4,$1
2:	subu	$4,$1
3:	.set	at
	.set	reorder
 # 0 "" 2
#NO_APP
	sw	$3,160($sp)
	sw	$2,164($sp)
	sw	$4,168($sp)
	lw	$2,168($sp)
	bne	$2,$0,$L199
	lui	$2,%hi(gponRecovery)
	addiu	$2,$2,%lo(gponRecovery)
	li	$3,1			# 0x1
	sb	$3,2184($2)
	j	$L191
$L199:
	lui	$2,%hi(gponRecovery)
	addiu	$2,$2,%lo(gponRecovery)
	sb	$0,2184($2)
	j	$L191
$L197:
	addiu	$2,$sp,1688
	sw	$2,172($sp)
	lui	$2,%hi($LC16)
	addiu	$2,$2,%lo($LC16)
	sw	$2,176($sp)
	lw	$3,172($sp)
	lw	$2,176($sp)
#APP
 # 77 "/opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/string.h" 1
	.set	noreorder
	.set	noat
	lbu	$4,($3)
1:	lbu	$1,($2)
	addiu	$3,1
	bne	$1,$4,2f
	addiu	$2,1
	bnez	$4,1b
	lbu	$4,($3)
	move	$4,$1
2:	subu	$4,$1
3:	.set	at
	.set	reorder
 # 0 "" 2
#NO_APP
	sw	$3,172($sp)
	sw	$2,176($sp)
	sw	$4,180($sp)
	lw	$2,180($sp)
	bne	$2,$0,$L202
	addiu	$2,$sp,1720
	sw	$2,184($sp)
	lui	$2,%hi($LC11)
	addiu	$2,$2,%lo($LC11)
	sw	$2,188($sp)
	lw	$3,184($sp)
	lw	$2,188($sp)
#APP
 # 77 "/opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/string.h" 1
	.set	noreorder
	.set	noat
	lbu	$4,($3)
1:	lbu	$1,($2)
	addiu	$3,1
	bne	$1,$4,2f
	addiu	$2,1
	bnez	$4,1b
	lbu	$4,($3)
	move	$4,$1
2:	subu	$4,$1
3:	.set	at
	.set	reorder
 # 0 "" 2
#NO_APP
	sw	$3,184($sp)
	sw	$2,188($sp)
	sw	$4,192($sp)
	lw	$2,192($sp)
	bne	$2,$0,$L204
	lui	$2,%hi(gpPonSysData)
	lw	$4,%lo(gpPonSysData)($2)
	lw	$3,1752($sp)
	li	$2,65536			# 0x10000
	addu	$2,$4,$2
	sw	$3,-30820($2)
	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	lw	$3,-30820($2)
	lui	$2,%hi($LC17)
	addiu	$4,$2,%lo($LC17)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	j	$L191
$L204:
	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	lw	$3,-30820($2)
	lui	$2,%hi($LC18)
	addiu	$4,$2,%lo($LC18)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	j	$L191
$L202:
	addiu	$2,$sp,1688
	sw	$2,196($sp)
	lui	$2,%hi($LC19)
	addiu	$2,$2,%lo($LC19)
	sw	$2,200($sp)
	lw	$3,196($sp)
	lw	$2,200($sp)
#APP
 # 77 "/opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/string.h" 1
	.set	noreorder
	.set	noat
	lbu	$4,($3)
1:	lbu	$1,($2)
	addiu	$3,1
	bne	$1,$4,2f
	addiu	$2,1
	bnez	$4,1b
	lbu	$4,($3)
	move	$4,$1
2:	subu	$4,$1
3:	.set	at
	.set	reorder
 # 0 "" 2
#NO_APP
	sw	$3,196($sp)
	sw	$2,200($sp)
	sw	$4,204($sp)
	lw	$2,204($sp)
	bne	$2,$0,$L207
	addiu	$2,$sp,1720
	sw	$2,208($sp)
	lui	$2,%hi($LC20)
	addiu	$2,$2,%lo($LC20)
	sw	$2,212($sp)
	lw	$3,208($sp)
	lw	$2,212($sp)
#APP
 # 77 "/opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/string.h" 1
	.set	noreorder
	.set	noat
	lbu	$4,($3)
1:	lbu	$1,($2)
	addiu	$3,1
	bne	$1,$4,2f
	addiu	$2,1
	bnez	$4,1b
	lbu	$4,($3)
	move	$4,$1
2:	subu	$4,$1
3:	.set	at
	.set	reorder
 # 0 "" 2
#NO_APP
	sw	$3,208($sp)
	sw	$2,212($sp)
	sw	$4,216($sp)
	lw	$2,216($sp)
	bne	$2,$0,$L209
	lw	$2,1752($sp)
	move	$4,$2
	lui	$2,%hi(xpon_set_qdma_qos)
	addiu	$2,$2,%lo(xpon_set_qdma_qos)
	jalr	$2
	lw	$2,1752($sp)
	sw	$0,220($sp)
	sw	$2,224($sp)
	lw	$2,220($sp)
	addiu	$2,$2,208
	sll	$3,$2,3
	lui	$2,%hi(ecnt_hooks)
	addiu	$2,$2,%lo(ecnt_hooks)
	addu	$2,$3,$2
	sw	$2,228($sp)
	lw	$2,228($sp)
	lw	$3,0($2)
	lw	$2,228($sp)
	xor	$2,$3,$2
	sltu	$2,$2,1
	andi	$2,$2,0x00ff
	bne	$2,$0,$L191
	lw	$2,224($sp)
	sw	$2,1756($sp)
	lw	$3,220($sp)
	addiu	$2,$sp,1756
	li	$4,26			# 0x1a
	move	$5,$3
	move	$6,$2
	lui	$2,%hi(__ECNT_HOOK)
	addiu	$2,$2,%lo(__ECNT_HOOK)
	jalr	$2
	sw	$2,232($sp)
	j	$L191
$L209:
	addiu	$2,$sp,1720
	sw	$2,236($sp)
	lui	$2,%hi($LC21)
	addiu	$2,$2,%lo($LC21)
	sw	$2,240($sp)
	lw	$3,236($sp)
	lw	$2,240($sp)
#APP
 # 77 "/opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/string.h" 1
	.set	noreorder
	.set	noat
	lbu	$4,($3)
1:	lbu	$1,($2)
	addiu	$3,1
	bne	$1,$4,2f
	addiu	$2,1
	bnez	$4,1b
	lbu	$4,($3)
	move	$4,$1
2:	subu	$4,$1
3:	.set	at
	.set	reorder
 # 0 "" 2
#NO_APP
	sw	$3,236($sp)
	sw	$2,240($sp)
	sw	$4,244($sp)
	lw	$2,244($sp)
	bne	$2,$0,$L216
	lw	$3,1752($sp)
	li	$2,1			# 0x1
	bne	$3,$2,$L217
	lui	$2,%hi(gpWanPriv)
	lw	$3,%lo(gpWanPriv)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	lw	$2,-31788($2)
	andi	$2,$2,0x4000
	beq	$2,$0,$L218
	lui	$2,%hi($LC22)
	addiu	$4,$2,%lo($LC22)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	lw	$2,7040($sp)
	j	$L533
$L218:
	li	$4,1			# 0x1
	lui	$2,%hi(xpon_set_qdma_qos)
	addiu	$2,$2,%lo(xpon_set_qdma_qos)
	jalr	$2
	j	$L219
$L217:
	lw	$2,1752($sp)
	bne	$2,$0,$L219
	move	$4,$0
	lui	$2,%hi(xpon_set_qdma_qos)
	addiu	$2,$2,%lo(xpon_set_qdma_qos)
	jalr	$2
$L219:
	addiu	$2,$sp,1760
	move	$4,$2
	move	$5,$0
	li	$6,16			# 0x10
	lui	$2,%hi(memset)
	addiu	$2,$2,%lo(memset)
	jalr	$2
	sw	$0,32($sp)
	j	$L220
$L221:
	lw	$2,32($sp)
	addiu	$3,$sp,32
	addu	$2,$3,$2
	li	$3,-1			# 0xffffffffffffffff
	sb	$3,1736($2)
	lw	$2,32($sp)
	addiu	$2,$2,1
	sw	$2,32($sp)
$L220:
	lw	$2,32($sp)
	slt	$2,$2,8
	bne	$2,$0,$L221
	sw	$0,32($sp)
	j	$L222
$L226:
	lw	$2,32($sp)
	andi	$2,$2,0x00ff
	sb	$2,1760($sp)
	li	$2,1			# 0x1
	sw	$2,1764($sp)
	li	$2,16			# 0x10
	sw	$2,248($sp)
	addiu	$2,$sp,1760
	sw	$2,252($sp)
	sw	$0,256($sp)
	li	$2,13			# 0xd
	sw	$2,1776($sp)
	lw	$2,252($sp)
	sw	$2,1792($sp)
	addiu	$2,$sp,1776
	lw	$4,248($sp)
	move	$5,$0
	move	$6,$2
	lui	$2,%hi(__ECNT_HOOK)
	addiu	$2,$2,%lo(__ECNT_HOOK)
	jalr	$2
	sw	$2,256($sp)
	lw	$2,32($sp)
	addiu	$2,$2,1
	sw	$2,32($sp)
$L222:
	lw	$2,32($sp)
	slt	$2,$2,32
	bne	$2,$0,$L226
	j	$L191
$L216:
	addiu	$2,$sp,1720
	sw	$2,260($sp)
	lui	$2,%hi($LC23)
	addiu	$2,$2,%lo($LC23)
	sw	$2,264($sp)
	lw	$3,260($sp)
	lw	$2,264($sp)
#APP
 # 77 "/opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/string.h" 1
	.set	noreorder
	.set	noat
	lbu	$4,($3)
1:	lbu	$1,($2)
	addiu	$3,1
	bne	$1,$4,2f
	addiu	$2,1
	bnez	$4,1b
	lbu	$4,($3)
	move	$4,$1
2:	subu	$4,$1
3:	.set	at
	.set	reorder
 # 0 "" 2
#NO_APP
	sw	$3,260($sp)
	sw	$2,264($sp)
	sw	$4,268($sp)
	lw	$2,268($sp)
	bne	$2,$0,$L191
	lw	$2,1752($sp)
	sltu	$2,$2,3001
	bne	$2,$0,$L191
	lw	$3,1752($sp)
	lui	$2,%hi($LC24)
	addiu	$4,$2,%lo($LC24)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	lui	$2,%hi(gpWanPriv)
	lw	$4,%lo(gpWanPriv)($2)
	lw	$3,1752($sp)
	li	$2,65536			# 0x10000
	addu	$2,$4,$2
	sw	$3,-31828($2)
	j	$L191
$L207:
	addiu	$2,$sp,1688
	sw	$2,272($sp)
	lui	$2,%hi($LC25)
	addiu	$2,$2,%lo($LC25)
	sw	$2,276($sp)
	lw	$3,272($sp)
	lw	$2,276($sp)
#APP
 # 77 "/opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/string.h" 1
	.set	noreorder
	.set	noat
	lbu	$4,($3)
1:	lbu	$1,($2)
	addiu	$3,1
	bne	$1,$4,2f
	addiu	$2,1
	bnez	$4,1b
	lbu	$4,($3)
	move	$4,$1
2:	subu	$4,$1
3:	.set	at
	.set	reorder
 # 0 "" 2
#NO_APP
	sw	$3,272($sp)
	sw	$2,276($sp)
	sw	$4,280($sp)
	lw	$2,280($sp)
	bne	$2,$0,$L229
	addiu	$2,$sp,1720
	sw	$2,284($sp)
	lui	$2,%hi($LC26)
	addiu	$2,$2,%lo($LC26)
	sw	$2,288($sp)
	lw	$3,284($sp)
	lw	$2,288($sp)
#APP
 # 77 "/opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/string.h" 1
	.set	noreorder
	.set	noat
	lbu	$4,($3)
1:	lbu	$1,($2)
	addiu	$3,1
	bne	$1,$4,2f
	addiu	$2,1
	bnez	$4,1b
	lbu	$4,($3)
	move	$4,$1
2:	subu	$4,$1
3:	.set	at
	.set	reorder
 # 0 "" 2
#NO_APP
	sw	$3,284($sp)
	sw	$2,288($sp)
	sw	$4,292($sp)
	lw	$2,292($sp)
	bne	$2,$0,$L231
	lw	$2,1752($sp)
	sltu	$2,$0,$2
	andi	$2,$2,0x00ff
	move	$3,$2
	lui	$2,%hi(eponStaticRptEnable)
	sb	$3,%lo(eponStaticRptEnable)($2)
	lw	$2,1752($sp)
	bne	$2,$0,$L232
	lui	$2,%hi($LC27)
	addiu	$2,$2,%lo($LC27)
	j	$L233
$L232:
	lui	$2,%hi($LC28)
	addiu	$2,$2,%lo($LC28)
$L233:
	lui	$3,%hi($LC29)
	addiu	$4,$3,%lo($LC29)
	move	$5,$2
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	lui	$2,%hi(gpPonSysData)
	lw	$2,%lo(gpPonSysData)($2)
	lw	$3,0($2)
	li	$2,2			# 0x2
	bne	$3,$2,$L191
	lw	$2,1752($sp)
	bne	$2,$0,$L235
	lui	$2,%hi(g_EPON_MAC_BASE)
	lw	$2,%lo(g_EPON_MAC_BASE)($2)
	addiu	$2,$2,24576
	sw	$2,296($sp)
	li	$2,1			# 0x1
	sw	$2,300($sp)
	li	$2,5			# 0x5
	sw	$2,304($sp)
	sw	$0,308($sp)
	lw	$2,296($sp)
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	sw	$2,312($sp)
	lw	$2,304($sp)
	lw	$3,300($sp)
	sll	$2,$3,$2
	nor	$3,$0,$2
	lw	$2,312($sp)
	and	$3,$3,$2
	lw	$4,300($sp)
	lw	$2,308($sp)
	and	$4,$4,$2
	lw	$2,304($sp)
	sll	$2,$4,$2
	or	$2,$3,$2
	sw	$2,312($sp)
	lw	$2,296($sp)
	lw	$4,312($sp)
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	lui	$2,%hi(g_EPON_MAC_BASE)
	lw	$2,%lo(g_EPON_MAC_BASE)($2)
	addiu	$2,$2,24792
	sw	$2,316($sp)
	li	$2,63			# 0x3f
	sw	$2,320($sp)
	sw	$0,324($sp)
	li	$2,12			# 0xc
	sw	$2,328($sp)
	lw	$2,316($sp)
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	sw	$2,332($sp)
	lw	$2,324($sp)
	lw	$3,320($sp)
	sll	$2,$3,$2
	nor	$3,$0,$2
	lw	$2,332($sp)
	and	$3,$3,$2
	lw	$4,320($sp)
	lw	$2,328($sp)
	and	$4,$4,$2
	lw	$2,324($sp)
	sll	$2,$4,$2
	or	$2,$3,$2
	sw	$2,332($sp)
	lw	$2,316($sp)
	lw	$4,332($sp)
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	j	$L191
$L235:
	lui	$2,%hi(g_EPON_MAC_BASE)
	lw	$2,%lo(g_EPON_MAC_BASE)($2)
	addiu	$2,$2,24676
	sw	$2,336($sp)
	li	$2,1			# 0x1
	sw	$2,340($sp)
	li	$2,8			# 0x8
	sw	$2,344($sp)
	li	$2,1			# 0x1
	sw	$2,348($sp)
	lw	$2,336($sp)
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	sw	$2,352($sp)
	lw	$2,344($sp)
	lw	$3,340($sp)
	sll	$2,$3,$2
	nor	$3,$0,$2
	lw	$2,352($sp)
	and	$3,$3,$2
	lw	$4,340($sp)
	lw	$2,348($sp)
	and	$4,$4,$2
	lw	$2,344($sp)
	sll	$2,$4,$2
	or	$2,$3,$2
	sw	$2,352($sp)
	lw	$2,336($sp)
	lw	$4,352($sp)
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	lui	$2,%hi(g_EPON_MAC_BASE)
	lw	$2,%lo(g_EPON_MAC_BASE)($2)
	addiu	$2,$2,24676
	sw	$2,356($sp)
	li	$2,1			# 0x1
	sw	$2,360($sp)
	li	$2,8			# 0x8
	sw	$2,364($sp)
	lw	$2,356($sp)
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	lui	$2,%hi(g_EPON_MAC_BASE)
	lw	$2,%lo(g_EPON_MAC_BASE)($2)
	addiu	$2,$2,24676
	sw	$2,368($sp)
	li	$2,1			# 0x1
	sw	$2,372($sp)
	li	$2,8			# 0x8
	sw	$2,376($sp)
	lw	$2,368($sp)
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	lui	$2,%hi(g_EPON_MAC_BASE)
	lw	$2,%lo(g_EPON_MAC_BASE)($2)
	addiu	$2,$2,24676
	sw	$2,380($sp)
	li	$2,1			# 0x1
	sw	$2,384($sp)
	li	$2,8			# 0x8
	sw	$2,388($sp)
	lw	$2,380($sp)
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	lui	$2,%hi(g_EPON_MAC_BASE)
	lw	$2,%lo(g_EPON_MAC_BASE)($2)
	addiu	$2,$2,24676
	sw	$2,392($sp)
	li	$2,1			# 0x1
	sw	$2,396($sp)
	li	$2,8			# 0x8
	sw	$2,400($sp)
	lw	$2,392($sp)
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	lui	$2,%hi(g_EPON_MAC_BASE)
	lw	$2,%lo(g_EPON_MAC_BASE)($2)
	addiu	$2,$2,24676
	sw	$2,404($sp)
	li	$2,1			# 0x1
	sw	$2,408($sp)
	li	$2,8			# 0x8
	sw	$2,412($sp)
	sw	$0,416($sp)
	lw	$2,404($sp)
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	sw	$2,420($sp)
	lw	$2,412($sp)
	lw	$3,408($sp)
	sll	$2,$3,$2
	nor	$3,$0,$2
	lw	$2,420($sp)
	and	$3,$3,$2
	lw	$4,408($sp)
	lw	$2,416($sp)
	and	$4,$4,$2
	lw	$2,412($sp)
	sll	$2,$4,$2
	or	$2,$3,$2
	sw	$2,420($sp)
	lw	$2,404($sp)
	lw	$4,420($sp)
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	lui	$2,%hi(g_EPON_MAC_BASE)
	lw	$2,%lo(g_EPON_MAC_BASE)($2)
	addiu	$2,$2,24676
	sw	$2,424($sp)
	li	$2,63			# 0x3f
	sw	$2,428($sp)
	sw	$0,432($sp)
	li	$2,1			# 0x1
	sw	$2,436($sp)
	lw	$2,424($sp)
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	sw	$2,440($sp)
	lw	$2,432($sp)
	lw	$3,428($sp)
	sll	$2,$3,$2
	nor	$3,$0,$2
	lw	$2,440($sp)
	and	$3,$3,$2
	lw	$4,428($sp)
	lw	$2,436($sp)
	and	$4,$4,$2
	lw	$2,432($sp)
	sll	$2,$4,$2
	or	$2,$3,$2
	sw	$2,440($sp)
	lw	$2,424($sp)
	lw	$4,440($sp)
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	lui	$2,%hi(g_EPON_MAC_BASE)
	lw	$2,%lo(g_EPON_MAC_BASE)($2)
	addiu	$2,$2,24672
	sw	$2,444($sp)
	li	$2,-1			# 0xffffffffffffffff
	sw	$2,448($sp)
	sw	$0,452($sp)
	li	$2,16842752			# 0x1010000
	ori	$2,$2,0x8000
	sw	$2,456($sp)
	lw	$2,444($sp)
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	sw	$2,460($sp)
	lw	$2,452($sp)
	lw	$3,448($sp)
	sll	$2,$3,$2
	nor	$3,$0,$2
	lw	$2,460($sp)
	and	$3,$3,$2
	lw	$4,448($sp)
	lw	$2,456($sp)
	and	$4,$4,$2
	lw	$2,452($sp)
	sll	$2,$4,$2
	or	$2,$3,$2
	sw	$2,460($sp)
	lw	$2,444($sp)
	lw	$4,460($sp)
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	lui	$2,%hi(g_EPON_MAC_BASE)
	lw	$2,%lo(g_EPON_MAC_BASE)($2)
	addiu	$2,$2,24672
	sw	$2,464($sp)
	li	$2,-1			# 0xffffffffffffffff
	sw	$2,468($sp)
	sw	$0,472($sp)
	sw	$0,476($sp)
	lw	$2,464($sp)
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	sw	$2,480($sp)
	lw	$2,472($sp)
	lw	$3,468($sp)
	sll	$2,$3,$2
	nor	$3,$0,$2
	lw	$2,480($sp)
	and	$3,$3,$2
	lw	$4,468($sp)
	lw	$2,476($sp)
	and	$4,$4,$2
	lw	$2,472($sp)
	sll	$2,$4,$2
	or	$2,$3,$2
	sw	$2,480($sp)
	lw	$2,464($sp)
	lw	$4,480($sp)
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	lui	$2,%hi(g_EPON_MAC_BASE)
	lw	$2,%lo(g_EPON_MAC_BASE)($2)
	addiu	$2,$2,24576
	sw	$2,484($sp)
	li	$2,1			# 0x1
	sw	$2,488($sp)
	li	$2,5			# 0x5
	sw	$2,492($sp)
	li	$2,1			# 0x1
	sw	$2,496($sp)
	lw	$2,484($sp)
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	sw	$2,500($sp)
	lw	$2,492($sp)
	lw	$3,488($sp)
	sll	$2,$3,$2
	nor	$3,$0,$2
	lw	$2,500($sp)
	and	$3,$3,$2
	lw	$4,488($sp)
	lw	$2,496($sp)
	and	$4,$4,$2
	lw	$2,492($sp)
	sll	$2,$4,$2
	or	$2,$3,$2
	sw	$2,500($sp)
	lw	$2,484($sp)
	lw	$4,500($sp)
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	lui	$2,%hi(g_EPON_MAC_BASE)
	lw	$2,%lo(g_EPON_MAC_BASE)($2)
	addiu	$2,$2,24792
	sw	$2,504($sp)
	li	$2,63			# 0x3f
	sw	$2,508($sp)
	sw	$0,512($sp)
	li	$2,8			# 0x8
	sw	$2,516($sp)
	lw	$2,504($sp)
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	sw	$2,520($sp)
	lw	$2,512($sp)
	lw	$3,508($sp)
	sll	$2,$3,$2
	nor	$3,$0,$2
	lw	$2,520($sp)
	and	$3,$3,$2
	lw	$4,508($sp)
	lw	$2,516($sp)
	and	$4,$4,$2
	lw	$2,512($sp)
	sll	$2,$4,$2
	or	$2,$3,$2
	sw	$2,520($sp)
	lw	$2,504($sp)
	lw	$4,520($sp)
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	j	$L191
$L231:
	addiu	$2,$sp,1720
	sw	$2,524($sp)
	lui	$2,%hi($LC30)
	addiu	$2,$2,%lo($LC30)
	sw	$2,528($sp)
	lw	$3,524($sp)
	lw	$2,528($sp)
#APP
 # 77 "/opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/string.h" 1
	.set	noreorder
	.set	noat
	lbu	$4,($3)
1:	lbu	$1,($2)
	addiu	$3,1
	bne	$1,$4,2f
	addiu	$2,1
	bnez	$4,1b
	lbu	$4,($3)
	move	$4,$1
2:	subu	$4,$1
3:	.set	at
	.set	reorder
 # 0 "" 2
#NO_APP
	sw	$3,524($sp)
	sw	$2,528($sp)
	sw	$4,532($sp)
	lw	$2,532($sp)
	bne	$2,$0,$L242
	lw	$2,1752($sp)
	bne	$2,$0,$L243
	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$3,$3,$2
	lbu	$2,-30828($3)
	ins	$2,$0,6,1
	sb	$2,-30828($3)
	move	$4,$0
	lui	$2,%hi(eponHwDygaspCtrl)
	addiu	$2,$2,%lo(eponHwDygaspCtrl)
	jalr	$2
	lui	$2,%hi($LC31)
	addiu	$4,$2,%lo($LC31)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	j	$L191
$L243:
	lw	$3,1752($sp)
	li	$2,1			# 0x1
	bne	$3,$2,$L191
	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$3,$3,$2
	lbu	$2,-30828($3)
	li	$4,1			# 0x1
	ins	$2,$4,6,1
	sb	$2,-30828($3)
	li	$4,1			# 0x1
	lui	$2,%hi(eponHwDygaspCtrl)
	addiu	$2,$2,%lo(eponHwDygaspCtrl)
	jalr	$2
	lui	$2,%hi($LC32)
	addiu	$4,$2,%lo($LC32)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	j	$L191
$L242:
	addiu	$2,$sp,1720
	sw	$2,536($sp)
	lui	$2,%hi($LC33)
	addiu	$2,$2,%lo($LC33)
	sw	$2,540($sp)
	lw	$3,536($sp)
	lw	$2,540($sp)
#APP
 # 77 "/opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/string.h" 1
	.set	noreorder
	.set	noat
	lbu	$4,($3)
1:	lbu	$1,($2)
	addiu	$3,1
	bne	$1,$4,2f
	addiu	$2,1
	bnez	$4,1b
	lbu	$4,($3)
	move	$4,$1
2:	subu	$4,$1
3:	.set	at
	.set	reorder
 # 0 "" 2
#NO_APP
	sw	$3,536($sp)
	sw	$2,540($sp)
	sw	$4,544($sp)
	lw	$2,544($sp)
	bne	$2,$0,$L191
	li	$2,-1078984704			# 0xffffffffbfb00000
	ori	$2,$2,0x64
	lw	$3,0($2)
	li	$2,-65536			# 0xffffffffffff0000
	and	$3,$3,$2
	li	$2,524288			# 0x80000
	bne	$3,$2,$L191
	lw	$2,1752($sp)
	bne	$2,$0,$L246
	lui	$2,%hi(gpEponPriv)
	lw	$2,%lo(gpEponPriv)($2)
	sb	$0,36($2)
	lui	$2,%hi($LC34)
	addiu	$4,$2,%lo($LC34)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	j	$L191
$L246:
	lw	$3,1752($sp)
	li	$2,1			# 0x1
	bne	$3,$2,$L191
	lui	$2,%hi(gpEponPriv)
	lw	$2,%lo(gpEponPriv)($2)
	li	$3,1			# 0x1
	sb	$3,36($2)
	lui	$2,%hi($LC35)
	addiu	$4,$2,%lo($LC35)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	j	$L191
$L229:
	addiu	$2,$sp,1688
	sw	$2,548($sp)
	lui	$2,%hi($LC23)
	addiu	$2,$2,%lo($LC23)
	sw	$2,552($sp)
	lw	$3,548($sp)
	lw	$2,552($sp)
#APP
 # 77 "/opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/string.h" 1
	.set	noreorder
	.set	noat
	lbu	$4,($3)
1:	lbu	$1,($2)
	addiu	$3,1
	bne	$1,$4,2f
	addiu	$2,1
	bnez	$4,1b
	lbu	$4,($3)
	move	$4,$1
2:	subu	$4,$1
3:	.set	at
	.set	reorder
 # 0 "" 2
#NO_APP
	sw	$3,548($sp)
	sw	$2,552($sp)
	sw	$4,556($sp)
	lw	$2,556($sp)
	bne	$2,$0,$L248
	addiu	$2,$sp,1720
	sw	$2,560($sp)
	lui	$2,%hi($LC15)
	addiu	$2,$2,%lo($LC15)
	sw	$2,564($sp)
	lw	$3,560($sp)
	lw	$2,564($sp)
#APP
 # 77 "/opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/string.h" 1
	.set	noreorder
	.set	noat
	lbu	$4,($3)
1:	lbu	$1,($2)
	addiu	$3,1
	bne	$1,$4,2f
	addiu	$2,1
	bnez	$4,1b
	lbu	$4,($3)
	move	$4,$1
2:	subu	$4,$1
3:	.set	at
	.set	reorder
 # 0 "" 2
#NO_APP
	sw	$3,560($sp)
	sw	$2,564($sp)
	sw	$4,568($sp)
	lw	$2,568($sp)
	bne	$2,$0,$L250
	lui	$2,%hi($LC36)
	addiu	$4,$2,%lo($LC36)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	lui	$2,%hi(attackEnhance)
	li	$3,1			# 0x1
	sb	$3,%lo(attackEnhance)($2)
	j	$L191
$L250:
	lui	$2,%hi($LC37)
	addiu	$4,$2,%lo($LC37)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	lui	$2,%hi(attackEnhance)
	sb	$0,%lo(attackEnhance)($2)
	j	$L191
$L248:
	addiu	$2,$sp,1688
	sw	$2,572($sp)
	lui	$2,%hi($LC38)
	addiu	$2,$2,%lo($LC38)
	sw	$2,576($sp)
	lw	$3,572($sp)
	lw	$2,576($sp)
#APP
 # 77 "/opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/string.h" 1
	.set	noreorder
	.set	noat
	lbu	$4,($3)
1:	lbu	$1,($2)
	addiu	$3,1
	bne	$1,$4,2f
	addiu	$2,1
	bnez	$4,1b
	lbu	$4,($3)
	move	$4,$1
2:	subu	$4,$1
3:	.set	at
	.set	reorder
 # 0 "" 2
#NO_APP
	sw	$3,572($sp)
	sw	$2,576($sp)
	sw	$4,580($sp)
	lw	$2,580($sp)
	bne	$2,$0,$L253
	addiu	$2,$sp,1720
	move	$4,$2
	lui	$2,%hi(atoi_temp)
	addiu	$2,$2,%lo(atoi_temp)
	jalr	$2
	sw	$2,68($sp)
	lw	$2,68($sp)
	blez	$2,$L254
	lw	$2,68($sp)
	slt	$2,$2,4097
	beq	$2,$0,$L254
	lw	$3,68($sp)
	lui	$2,%hi(dropThred)
	sw	$3,%lo(dropThred)($2)
$L254:
	lui	$2,%hi(dropThred)
	lw	$3,%lo(dropThred)($2)
	lui	$2,%hi($LC39)
	addiu	$4,$2,%lo($LC39)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	j	$L191
$L253:
	addiu	$2,$sp,1688
	sw	$2,584($sp)
	lui	$2,%hi($LC40)
	addiu	$2,$2,%lo($LC40)
	sw	$2,588($sp)
	lw	$3,584($sp)
	lw	$2,588($sp)
#APP
 # 77 "/opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/string.h" 1
	.set	noreorder
	.set	noat
	lbu	$4,($3)
1:	lbu	$1,($2)
	addiu	$3,1
	bne	$1,$4,2f
	addiu	$2,1
	bnez	$4,1b
	lbu	$4,($3)
	move	$4,$1
2:	subu	$4,$1
3:	.set	at
	.set	reorder
 # 0 "" 2
#NO_APP
	sw	$3,584($sp)
	sw	$2,588($sp)
	sw	$4,592($sp)
	lw	$2,592($sp)
	bne	$2,$0,$L191
	addiu	$2,$sp,1720
	move	$4,$2
	lui	$2,%hi(atoi_temp)
	addiu	$2,$2,%lo(atoi_temp)
	jalr	$2
	sw	$2,72($sp)
	lw	$2,72($sp)
	blez	$2,$L256
	lw	$2,72($sp)
	slt	$2,$2,61
	beq	$2,$0,$L256
	lw	$2,72($sp)
	andi	$3,$2,0x00ff
	lui	$2,%hi(rxDropCheckTimer)
	sb	$3,%lo(rxDropCheckTimer)($2)
$L256:
	lui	$2,%hi(rxDropCheckTimer)
	lbu	$2,%lo(rxDropCheckTimer)($2)
	move	$3,$2
	lui	$2,%hi($LC41)
	addiu	$4,$2,%lo($LC41)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
$L191:
	addiu	$2,$sp,1688
	sw	$2,596($sp)
	lui	$2,%hi($LC42)
	addiu	$2,$2,%lo($LC42)
	sw	$2,600($sp)
	lw	$3,596($sp)
	lw	$2,600($sp)
#APP
 # 77 "/opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/string.h" 1
	.set	noreorder
	.set	noat
	lbu	$4,($3)
1:	lbu	$1,($2)
	addiu	$3,1
	bne	$1,$4,2f
	addiu	$2,1
	bnez	$4,1b
	lbu	$4,($3)
	move	$4,$1
2:	subu	$4,$1
3:	.set	at
	.set	reorder
 # 0 "" 2
#NO_APP
	sw	$3,596($sp)
	sw	$2,600($sp)
	sw	$4,604($sp)
	lw	$2,604($sp)
	bne	$2,$0,$L258
	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	lhu	$2,-30816($2)
	sw	$2,76($sp)
	addiu	$2,$sp,1720
	sw	$2,608($sp)
	lui	$2,%hi($LC43)
	addiu	$2,$2,%lo($LC43)
	sw	$2,612($sp)
	lw	$3,608($sp)
	lw	$2,612($sp)
#APP
 # 77 "/opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/string.h" 1
	.set	noreorder
	.set	noat
	lbu	$4,($3)
1:	lbu	$1,($2)
	addiu	$3,1
	bne	$1,$4,2f
	addiu	$2,1
	bnez	$4,1b
	lbu	$4,($3)
	move	$4,$1
2:	subu	$4,$1
3:	.set	at
	.set	reorder
 # 0 "" 2
#NO_APP
	sw	$3,608($sp)
	sw	$2,612($sp)
	sw	$4,616($sp)
	lw	$2,616($sp)
	bne	$2,$0,$L260
	lui	$2,%hi(gpPonSysData)
	lw	$4,%lo(gpPonSysData)($2)
	lw	$2,1752($sp)
	beq	$2,$0,$L261
	lw	$2,76($sp)
	seh	$2,$2
	ori	$2,$2,0x8
	seh	$2,$2
	andi	$2,$2,0xffff
	j	$L262
$L261:
	lw	$2,76($sp)
	andi	$3,$2,0xffff
	li	$2,-9			# 0xfffffffffffffff7
	and	$2,$3,$2
	andi	$2,$2,0xffff
$L262:
	li	$3,65536			# 0x10000
	addu	$3,$4,$3
	sh	$2,-30816($3)
	j	$L263
$L260:
	addiu	$2,$sp,1720
	sw	$2,620($sp)
	lui	$2,%hi($LC44)
	addiu	$2,$2,%lo($LC44)
	sw	$2,624($sp)
	lw	$3,620($sp)
	lw	$2,624($sp)
#APP
 # 77 "/opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/string.h" 1
	.set	noreorder
	.set	noat
	lbu	$4,($3)
1:	lbu	$1,($2)
	addiu	$3,1
	bne	$1,$4,2f
	addiu	$2,1
	bnez	$4,1b
	lbu	$4,($3)
	move	$4,$1
2:	subu	$4,$1
3:	.set	at
	.set	reorder
 # 0 "" 2
#NO_APP
	sw	$3,620($sp)
	sw	$2,624($sp)
	sw	$4,628($sp)
	lw	$2,628($sp)
	bne	$2,$0,$L265
	lui	$2,%hi(gpPonSysData)
	lw	$4,%lo(gpPonSysData)($2)
	lw	$2,1752($sp)
	beq	$2,$0,$L266
	lw	$2,76($sp)
	seh	$2,$2
	ori	$2,$2,0x20
	seh	$2,$2
	andi	$2,$2,0xffff
	j	$L267
$L266:
	lw	$2,76($sp)
	andi	$3,$2,0xffff
	li	$2,-33			# 0xffffffffffffffdf
	and	$2,$3,$2
	andi	$2,$2,0xffff
$L267:
	li	$3,65536			# 0x10000
	addu	$3,$4,$3
	sh	$2,-30816($3)
	j	$L263
$L265:
	addiu	$2,$sp,1720
	sw	$2,632($sp)
	lui	$2,%hi($LC4)
	addiu	$2,$2,%lo($LC4)
	sw	$2,636($sp)
	lw	$3,632($sp)
	lw	$2,636($sp)
#APP
 # 77 "/opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/string.h" 1
	.set	noreorder
	.set	noat
	lbu	$4,($3)
1:	lbu	$1,($2)
	addiu	$3,1
	bne	$1,$4,2f
	addiu	$2,1
	bnez	$4,1b
	lbu	$4,($3)
	move	$4,$1
2:	subu	$4,$1
3:	.set	at
	.set	reorder
 # 0 "" 2
#NO_APP
	sw	$3,632($sp)
	sw	$2,636($sp)
	sw	$4,640($sp)
	lw	$2,640($sp)
	bne	$2,$0,$L269
	lui	$2,%hi(gpPonSysData)
	lw	$4,%lo(gpPonSysData)($2)
	lw	$2,1752($sp)
	beq	$2,$0,$L270
	lw	$2,76($sp)
	seh	$2,$2
	ori	$2,$2,0x10
	seh	$2,$2
	andi	$2,$2,0xffff
	j	$L271
$L270:
	lw	$2,76($sp)
	andi	$3,$2,0xffff
	li	$2,-17			# 0xffffffffffffffef
	and	$2,$3,$2
	andi	$2,$2,0xffff
$L271:
	li	$3,65536			# 0x10000
	addu	$3,$4,$3
	sh	$2,-30816($3)
	j	$L263
$L269:
	addiu	$2,$sp,1720
	sw	$2,644($sp)
	lui	$2,%hi($LC45)
	addiu	$2,$2,%lo($LC45)
	sw	$2,648($sp)
	lw	$3,644($sp)
	lw	$2,648($sp)
#APP
 # 77 "/opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/string.h" 1
	.set	noreorder
	.set	noat
	lbu	$4,($3)
1:	lbu	$1,($2)
	addiu	$3,1
	bne	$1,$4,2f
	addiu	$2,1
	bnez	$4,1b
	lbu	$4,($3)
	move	$4,$1
2:	subu	$4,$1
3:	.set	at
	.set	reorder
 # 0 "" 2
#NO_APP
	sw	$3,644($sp)
	sw	$2,648($sp)
	sw	$4,652($sp)
	lw	$2,652($sp)
	bne	$2,$0,$L273
	lui	$2,%hi(gpPonSysData)
	lw	$4,%lo(gpPonSysData)($2)
	lw	$2,1752($sp)
	beq	$2,$0,$L274
	lw	$2,76($sp)
	seh	$2,$2
	ori	$2,$2,0x4
	seh	$2,$2
	andi	$2,$2,0xffff
	j	$L275
$L274:
	lw	$2,76($sp)
	andi	$3,$2,0xffff
	li	$2,-5			# 0xfffffffffffffffb
	and	$2,$3,$2
	andi	$2,$2,0xffff
$L275:
	li	$3,65536			# 0x10000
	addu	$3,$4,$3
	sh	$2,-30816($3)
	j	$L263
$L273:
	addiu	$2,$sp,1720
	sw	$2,656($sp)
	lui	$2,%hi($LC46)
	addiu	$2,$2,%lo($LC46)
	sw	$2,660($sp)
	lw	$3,656($sp)
	lw	$2,660($sp)
#APP
 # 77 "/opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/string.h" 1
	.set	noreorder
	.set	noat
	lbu	$4,($3)
1:	lbu	$1,($2)
	addiu	$3,1
	bne	$1,$4,2f
	addiu	$2,1
	bnez	$4,1b
	lbu	$4,($3)
	move	$4,$1
2:	subu	$4,$1
3:	.set	at
	.set	reorder
 # 0 "" 2
#NO_APP
	sw	$3,656($sp)
	sw	$2,660($sp)
	sw	$4,664($sp)
	lw	$2,664($sp)
	bne	$2,$0,$L277
	lui	$2,%hi(gpPonSysData)
	lw	$4,%lo(gpPonSysData)($2)
	lw	$2,1752($sp)
	beq	$2,$0,$L278
	lw	$2,76($sp)
	seh	$2,$2
	ori	$2,$2,0x40
	seh	$2,$2
	andi	$2,$2,0xffff
	j	$L279
$L278:
	lw	$2,76($sp)
	andi	$3,$2,0xffff
	li	$2,-65			# 0xffffffffffffffbf
	and	$2,$3,$2
	andi	$2,$2,0xffff
$L279:
	li	$3,65536			# 0x10000
	addu	$3,$4,$3
	sh	$2,-30816($3)
	j	$L263
$L277:
	addiu	$2,$sp,1720
	sw	$2,668($sp)
	lui	$2,%hi($LC47)
	addiu	$2,$2,%lo($LC47)
	sw	$2,672($sp)
	lw	$3,668($sp)
	lw	$2,672($sp)
#APP
 # 77 "/opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/string.h" 1
	.set	noreorder
	.set	noat
	lbu	$4,($3)
1:	lbu	$1,($2)
	addiu	$3,1
	bne	$1,$4,2f
	addiu	$2,1
	bnez	$4,1b
	lbu	$4,($3)
	move	$4,$1
2:	subu	$4,$1
3:	.set	at
	.set	reorder
 # 0 "" 2
#NO_APP
	sw	$3,668($sp)
	sw	$2,672($sp)
	sw	$4,676($sp)
	lw	$2,676($sp)
	bne	$2,$0,$L281
	lui	$2,%hi(gpPonSysData)
	lw	$4,%lo(gpPonSysData)($2)
	lw	$2,1752($sp)
	beq	$2,$0,$L282
	lw	$2,76($sp)
	seh	$2,$2
	ori	$2,$2,0x2
	seh	$2,$2
	andi	$2,$2,0xffff
	j	$L283
$L282:
	lw	$2,76($sp)
	andi	$3,$2,0xffff
	li	$2,-3			# 0xfffffffffffffffd
	and	$2,$3,$2
	andi	$2,$2,0xffff
$L283:
	li	$3,65536			# 0x10000
	addu	$3,$4,$3
	sh	$2,-30816($3)
	j	$L263
$L281:
	addiu	$2,$sp,1720
	sw	$2,680($sp)
	lui	$2,%hi($LC48)
	addiu	$2,$2,%lo($LC48)
	sw	$2,684($sp)
	lw	$3,680($sp)
	lw	$2,684($sp)
#APP
 # 77 "/opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/string.h" 1
	.set	noreorder
	.set	noat
	lbu	$4,($3)
1:	lbu	$1,($2)
	addiu	$3,1
	bne	$1,$4,2f
	addiu	$2,1
	bnez	$4,1b
	lbu	$4,($3)
	move	$4,$1
2:	subu	$4,$1
3:	.set	at
	.set	reorder
 # 0 "" 2
#NO_APP
	sw	$3,680($sp)
	sw	$2,684($sp)
	sw	$4,688($sp)
	lw	$2,688($sp)
	bne	$2,$0,$L285
	lui	$2,%hi(gpPonSysData)
	lw	$4,%lo(gpPonSysData)($2)
	lw	$2,1752($sp)
	beq	$2,$0,$L286
	lw	$2,76($sp)
	seh	$2,$2
	ori	$2,$2,0x80
	seh	$2,$2
	andi	$2,$2,0xffff
	j	$L287
$L286:
	lw	$2,76($sp)
	andi	$3,$2,0xffff
	li	$2,-129			# 0xffffffffffffff7f
	and	$2,$3,$2
	andi	$2,$2,0xffff
$L287:
	li	$3,65536			# 0x10000
	addu	$3,$4,$3
	sh	$2,-30816($3)
	j	$L263
$L285:
	addiu	$2,$sp,1720
	sw	$2,692($sp)
	lui	$2,%hi($LC49)
	addiu	$2,$2,%lo($LC49)
	sw	$2,696($sp)
	lw	$3,692($sp)
	lw	$2,696($sp)
#APP
 # 77 "/opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/string.h" 1
	.set	noreorder
	.set	noat
	lbu	$4,($3)
1:	lbu	$1,($2)
	addiu	$3,1
	bne	$1,$4,2f
	addiu	$2,1
	bnez	$4,1b
	lbu	$4,($3)
	move	$4,$1
2:	subu	$4,$1
3:	.set	at
	.set	reorder
 # 0 "" 2
#NO_APP
	sw	$3,692($sp)
	sw	$2,696($sp)
	sw	$4,700($sp)
	lw	$2,700($sp)
	bne	$2,$0,$L289
	lui	$2,%hi(gpPonSysData)
	lw	$4,%lo(gpPonSysData)($2)
	lw	$2,1752($sp)
	beq	$2,$0,$L290
	lw	$2,76($sp)
	seh	$2,$2
	ori	$2,$2,0x100
	seh	$2,$2
	andi	$2,$2,0xffff
	j	$L291
$L290:
	lw	$2,76($sp)
	andi	$3,$2,0xffff
	li	$2,-257			# 0xfffffffffffffeff
	and	$2,$3,$2
	andi	$2,$2,0xffff
$L291:
	li	$3,65536			# 0x10000
	addu	$3,$4,$3
	sh	$2,-30816($3)
	j	$L263
$L289:
	addiu	$2,$sp,1720
	sw	$2,704($sp)
	lui	$2,%hi($LC50)
	addiu	$2,$2,%lo($LC50)
	sw	$2,708($sp)
	lw	$3,704($sp)
	lw	$2,708($sp)
#APP
 # 77 "/opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/string.h" 1
	.set	noreorder
	.set	noat
	lbu	$4,($3)
1:	lbu	$1,($2)
	addiu	$3,1
	bne	$1,$4,2f
	addiu	$2,1
	bnez	$4,1b
	lbu	$4,($3)
	move	$4,$1
2:	subu	$4,$1
3:	.set	at
	.set	reorder
 # 0 "" 2
#NO_APP
	sw	$3,704($sp)
	sw	$2,708($sp)
	sw	$4,712($sp)
	lw	$2,712($sp)
	bne	$2,$0,$L293
	lui	$2,%hi(gpPonSysData)
	lw	$4,%lo(gpPonSysData)($2)
	lw	$2,1752($sp)
	beq	$2,$0,$L294
	lw	$2,76($sp)
	seh	$2,$2
	ori	$2,$2,0x1
	seh	$2,$2
	andi	$2,$2,0xffff
	j	$L295
$L294:
	lw	$2,76($sp)
	andi	$3,$2,0xffff
	li	$2,-2			# 0xfffffffffffffffe
	and	$2,$3,$2
	andi	$2,$2,0xffff
$L295:
	li	$3,65536			# 0x10000
	addu	$3,$4,$3
	sh	$2,-30816($3)
	j	$L263
$L293:
	addiu	$2,$sp,1720
	sw	$2,716($sp)
	lui	$2,%hi($LC51)
	addiu	$2,$2,%lo($LC51)
	sw	$2,720($sp)
	lw	$3,716($sp)
	lw	$2,720($sp)
#APP
 # 77 "/opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/string.h" 1
	.set	noreorder
	.set	noat
	lbu	$4,($3)
1:	lbu	$1,($2)
	addiu	$3,1
	bne	$1,$4,2f
	addiu	$2,1
	bnez	$4,1b
	lbu	$4,($3)
	move	$4,$1
2:	subu	$4,$1
3:	.set	at
	.set	reorder
 # 0 "" 2
#NO_APP
	sw	$3,716($sp)
	sw	$2,720($sp)
	sw	$4,724($sp)
	lw	$2,724($sp)
	bne	$2,$0,$L297
	lui	$2,%hi(gpPonSysData)
	lw	$4,%lo(gpPonSysData)($2)
	lw	$2,1752($sp)
	beq	$2,$0,$L298
	lw	$2,76($sp)
	seh	$2,$2
	ori	$2,$2,0x200
	seh	$2,$2
	andi	$2,$2,0xffff
	j	$L299
$L298:
	lw	$2,76($sp)
	andi	$3,$2,0xffff
	li	$2,-513			# 0xfffffffffffffdff
	and	$2,$3,$2
	andi	$2,$2,0xffff
$L299:
	li	$3,65536			# 0x10000
	addu	$3,$4,$3
	sh	$2,-30816($3)
	j	$L263
$L297:
	addiu	$2,$sp,1720
	sw	$2,728($sp)
	lui	$2,%hi($LC52)
	addiu	$2,$2,%lo($LC52)
	sw	$2,732($sp)
	lw	$3,728($sp)
	lw	$2,732($sp)
#APP
 # 77 "/opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/string.h" 1
	.set	noreorder
	.set	noat
	lbu	$4,($3)
1:	lbu	$1,($2)
	addiu	$3,1
	bne	$1,$4,2f
	addiu	$2,1
	bnez	$4,1b
	lbu	$4,($3)
	move	$4,$1
2:	subu	$4,$1
3:	.set	at
	.set	reorder
 # 0 "" 2
#NO_APP
	sw	$3,728($sp)
	sw	$2,732($sp)
	sw	$4,736($sp)
	lw	$2,736($sp)
	bne	$2,$0,$L263
	lui	$2,%hi(gpPonSysData)
	lw	$4,%lo(gpPonSysData)($2)
	lw	$2,1752($sp)
	beq	$2,$0,$L301
	lw	$2,76($sp)
	seh	$2,$2
	ori	$2,$2,0x400
	seh	$2,$2
	andi	$2,$2,0xffff
	j	$L302
$L301:
	lw	$2,76($sp)
	andi	$3,$2,0xffff
	li	$2,-1025			# 0xfffffffffffffbff
	and	$2,$3,$2
	andi	$2,$2,0xffff
$L302:
	li	$3,65536			# 0x10000
	addu	$3,$4,$3
	sh	$2,-30816($3)
$L263:
	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	lhu	$2,-30816($2)
	move	$3,$2
	lui	$2,%hi($LC53)
	addiu	$4,$2,%lo($LC53)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	j	$L303
$L258:
	addiu	$2,$sp,1688
	sw	$2,740($sp)
	lui	$2,%hi($LC54)
	addiu	$2,$2,%lo($LC54)
	sw	$2,744($sp)
	lw	$3,740($sp)
	lw	$2,744($sp)
#APP
 # 77 "/opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/string.h" 1
	.set	noreorder
	.set	noat
	lbu	$4,($3)
1:	lbu	$1,($2)
	addiu	$3,1
	bne	$1,$4,2f
	addiu	$2,1
	bnez	$4,1b
	lbu	$4,($3)
	move	$4,$1
2:	subu	$4,$1
3:	.set	at
	.set	reorder
 # 0 "" 2
#NO_APP
	sw	$3,740($sp)
	sw	$2,744($sp)
	sw	$4,748($sp)
	lw	$2,748($sp)
	bne	$2,$0,$L305
	addiu	$2,$sp,1720
	sw	$2,752($sp)
	lui	$2,%hi($LC55)
	addiu	$2,$2,%lo($LC55)
	sw	$2,756($sp)
	lw	$3,752($sp)
	lw	$2,756($sp)
#APP
 # 77 "/opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/string.h" 1
	.set	noreorder
	.set	noat
	lbu	$4,($3)
1:	lbu	$1,($2)
	addiu	$3,1
	bne	$1,$4,2f
	addiu	$2,1
	bnez	$4,1b
	lbu	$4,($3)
	move	$4,$1
2:	subu	$4,$1
3:	.set	at
	.set	reorder
 # 0 "" 2
#NO_APP
	sw	$3,752($sp)
	sw	$2,756($sp)
	sw	$4,760($sp)
	lw	$2,760($sp)
	bne	$2,$0,$L307
	lw	$2,1752($sp)
	move	$4,$2
	lui	$2,%hi(gpon_dvt_gemport)
	addiu	$2,$2,%lo(gpon_dvt_gemport)
	jalr	$2
	j	$L303
$L307:
	addiu	$2,$sp,1720
	sw	$2,764($sp)
	lui	$2,%hi($LC56)
	addiu	$2,$2,%lo($LC56)
	sw	$2,768($sp)
	lw	$3,764($sp)
	lw	$2,768($sp)
#APP
 # 77 "/opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/string.h" 1
	.set	noreorder
	.set	noat
	lbu	$4,($3)
1:	lbu	$1,($2)
	addiu	$3,1
	bne	$1,$4,2f
	addiu	$2,1
	bnez	$4,1b
	lbu	$4,($3)
	move	$4,$1
2:	subu	$4,$1
3:	.set	at
	.set	reorder
 # 0 "" 2
#NO_APP
	sw	$3,764($sp)
	sw	$2,768($sp)
	sw	$4,772($sp)
	lw	$2,772($sp)
	bne	$2,$0,$L303
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	lw	$2,16416($2)
	ext	$2,$2,15,1
	andi	$2,$2,0x00ff
	beq	$2,$0,$L310
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	lw	$2,16416($2)
	ext	$2,$2,0,12
	andi	$2,$2,0xffff
	move	$3,$2
	lui	$2,%hi($LC57)
	addiu	$4,$2,%lo($LC57)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
$L310:
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	lw	$2,16416($2)
	ext	$2,$2,31,1
	andi	$2,$2,0x00ff
	beq	$2,$0,$L311
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	lw	$2,16416($2)
	ext	$2,$2,16,12
	andi	$2,$2,0xffff
	move	$3,$2
	lui	$2,%hi($LC58)
	addiu	$4,$2,%lo($LC58)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
$L311:
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	lw	$2,16420($2)
	ext	$2,$2,15,1
	andi	$2,$2,0x00ff
	beq	$2,$0,$L312
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	lw	$2,16420($2)
	ext	$2,$2,0,12
	andi	$2,$2,0xffff
	move	$3,$2
	lui	$2,%hi($LC59)
	addiu	$4,$2,%lo($LC59)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
$L312:
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	lw	$2,16420($2)
	ext	$2,$2,31,1
	andi	$2,$2,0x00ff
	beq	$2,$0,$L313
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	lw	$2,16420($2)
	ext	$2,$2,16,12
	andi	$2,$2,0xffff
	move	$3,$2
	lui	$2,%hi($LC60)
	addiu	$4,$2,%lo($LC60)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
$L313:
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	lw	$2,16424($2)
	ext	$2,$2,15,1
	andi	$2,$2,0x00ff
	beq	$2,$0,$L314
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	lw	$2,16424($2)
	ext	$2,$2,0,12
	andi	$2,$2,0xffff
	move	$3,$2
	lui	$2,%hi($LC61)
	addiu	$4,$2,%lo($LC61)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
$L314:
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	lw	$2,16424($2)
	ext	$2,$2,31,1
	andi	$2,$2,0x00ff
	beq	$2,$0,$L315
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	lw	$2,16424($2)
	ext	$2,$2,16,12
	andi	$2,$2,0xffff
	move	$3,$2
	lui	$2,%hi($LC62)
	addiu	$4,$2,%lo($LC62)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
$L315:
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	lw	$2,16428($2)
	ext	$2,$2,15,1
	andi	$2,$2,0x00ff
	beq	$2,$0,$L316
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	lw	$2,16428($2)
	ext	$2,$2,0,12
	andi	$2,$2,0xffff
	move	$3,$2
	lui	$2,%hi($LC63)
	addiu	$4,$2,%lo($LC63)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
$L316:
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	lw	$2,16428($2)
	ext	$2,$2,31,1
	andi	$2,$2,0x00ff
	beq	$2,$0,$L317
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	lw	$2,16428($2)
	ext	$2,$2,16,12
	andi	$2,$2,0xffff
	move	$3,$2
	lui	$2,%hi($LC64)
	addiu	$4,$2,%lo($LC64)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
$L317:
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	lw	$2,16432($2)
	ext	$2,$2,15,1
	andi	$2,$2,0x00ff
	beq	$2,$0,$L318
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	lw	$2,16432($2)
	ext	$2,$2,0,12
	andi	$2,$2,0xffff
	move	$3,$2
	lui	$2,%hi($LC65)
	addiu	$4,$2,%lo($LC65)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
$L318:
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	lw	$2,16432($2)
	ext	$2,$2,31,1
	andi	$2,$2,0x00ff
	beq	$2,$0,$L319
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	lw	$2,16432($2)
	ext	$2,$2,16,12
	andi	$2,$2,0xffff
	move	$3,$2
	lui	$2,%hi($LC66)
	addiu	$4,$2,%lo($LC66)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
$L319:
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	lw	$2,16436($2)
	ext	$2,$2,15,1
	andi	$2,$2,0x00ff
	beq	$2,$0,$L320
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	lw	$2,16436($2)
	ext	$2,$2,0,12
	andi	$2,$2,0xffff
	move	$3,$2
	lui	$2,%hi($LC67)
	addiu	$4,$2,%lo($LC67)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
$L320:
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	lw	$2,16436($2)
	ext	$2,$2,31,1
	andi	$2,$2,0x00ff
	beq	$2,$0,$L321
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	lw	$2,16436($2)
	ext	$2,$2,16,12
	andi	$2,$2,0xffff
	move	$3,$2
	lui	$2,%hi($LC68)
	addiu	$4,$2,%lo($LC68)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
$L321:
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	lw	$2,16440($2)
	ext	$2,$2,15,1
	andi	$2,$2,0x00ff
	beq	$2,$0,$L322
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	lw	$2,16440($2)
	ext	$2,$2,0,12
	andi	$2,$2,0xffff
	move	$3,$2
	lui	$2,%hi($LC69)
	addiu	$4,$2,%lo($LC69)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
$L322:
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	lw	$2,16440($2)
	ext	$2,$2,31,1
	andi	$2,$2,0x00ff
	beq	$2,$0,$L323
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	lw	$2,16440($2)
	ext	$2,$2,16,12
	andi	$2,$2,0xffff
	move	$3,$2
	lui	$2,%hi($LC70)
	addiu	$4,$2,%lo($LC70)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
$L323:
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	lw	$2,16444($2)
	ext	$2,$2,15,1
	andi	$2,$2,0x00ff
	beq	$2,$0,$L324
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	lw	$2,16444($2)
	ext	$2,$2,0,12
	andi	$2,$2,0xffff
	move	$3,$2
	lui	$2,%hi($LC71)
	addiu	$4,$2,%lo($LC71)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
$L324:
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	lw	$2,16444($2)
	ext	$2,$2,31,1
	andi	$2,$2,0x00ff
	beq	$2,$0,$L303
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	lw	$2,16444($2)
	ext	$2,$2,16,12
	andi	$2,$2,0xffff
	move	$3,$2
	lui	$2,%hi($LC72)
	addiu	$4,$2,%lo($LC72)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	j	$L303
$L305:
	addiu	$2,$sp,1688
	sw	$2,776($sp)
	lui	$2,%hi($LC73)
	addiu	$2,$2,%lo($LC73)
	sw	$2,780($sp)
	lw	$3,776($sp)
	lw	$2,780($sp)
#APP
 # 77 "/opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/string.h" 1
	.set	noreorder
	.set	noat
	lbu	$4,($3)
1:	lbu	$1,($2)
	addiu	$3,1
	bne	$1,$4,2f
	addiu	$2,1
	bnez	$4,1b
	lbu	$4,($3)
	move	$4,$1
2:	subu	$4,$1
3:	.set	at
	.set	reorder
 # 0 "" 2
#NO_APP
	sw	$3,776($sp)
	sw	$2,780($sp)
	sw	$4,784($sp)
	lw	$2,784($sp)
	bne	$2,$0,$L326
	addiu	$2,$sp,1720
	sw	$2,788($sp)
	lui	$2,%hi($LC74)
	addiu	$2,$2,%lo($LC74)
	sw	$2,792($sp)
	lw	$3,788($sp)
	lw	$2,792($sp)
#APP
 # 77 "/opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/string.h" 1
	.set	noreorder
	.set	noat
	lbu	$4,($3)
1:	lbu	$1,($2)
	addiu	$3,1
	bne	$1,$4,2f
	addiu	$2,1
	bnez	$4,1b
	lbu	$4,($3)
	move	$4,$1
2:	subu	$4,$1
3:	.set	at
	.set	reorder
 # 0 "" 2
#NO_APP
	sw	$3,788($sp)
	sw	$2,792($sp)
	sw	$4,796($sp)
	lw	$2,796($sp)
	bne	$2,$0,$L328
	sw	$0,32($sp)
	j	$L329
$L331:
	lui	$2,%hi(gpWanPriv)
	lw	$3,%lo(gpWanPriv)($2)
	lw	$2,32($sp)
	sll	$2,$2,5
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	lw	$2,8284($2)
	ext	$2,$2,2,1
	andi	$2,$2,0x00ff
	beq	$2,$0,$L330
	lui	$2,%hi(gpWanPriv)
	lw	$3,%lo(gpWanPriv)($2)
	lw	$2,32($sp)
	sll	$2,$2,5
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	lw	$2,8284($2)
	ext	$2,$2,20,12
	andi	$2,$2,0xffff
	move	$3,$2
	lw	$2,1752($sp)
	move	$4,$3
	li	$5,4			# 0x4
	move	$6,$2
	lui	$2,%hi(gwan_config_gemport)
	addiu	$2,$2,%lo(gwan_config_gemport)
	jalr	$2
$L330:
	lw	$2,32($sp)
	addiu	$2,$2,1
	sw	$2,32($sp)
$L329:
	lw	$2,32($sp)
	slt	$2,$2,256
	bne	$2,$0,$L331
	lui	$2,%hi(gpWanPriv)
	lw	$3,%lo(gpWanPriv)($2)
	lw	$2,1752($sp)
	xori	$2,$2,0x2
	sltu	$2,$2,1
	andi	$4,$2,0x00ff
	li	$2,65536			# 0x10000
	addu	$3,$3,$2
	lbu	$2,-31786($3)
	ins	$2,$4,7,1
	sb	$2,-31786($3)
	lw	$3,1752($sp)
	lui	$2,%hi($LC75)
	addiu	$4,$2,%lo($LC75)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	j	$L332
$L328:
	addiu	$2,$sp,1720
	sw	$2,800($sp)
	lui	$2,%hi($LC76)
	addiu	$2,$2,%lo($LC76)
	sw	$2,804($sp)
	lw	$3,800($sp)
	lw	$2,804($sp)
#APP
 # 77 "/opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/string.h" 1
	.set	noreorder
	.set	noat
	lbu	$4,($3)
1:	lbu	$1,($2)
	addiu	$3,1
	bne	$1,$4,2f
	addiu	$2,1
	bnez	$4,1b
	lbu	$4,($3)
	move	$4,$1
2:	subu	$4,$1
3:	.set	at
	.set	reorder
 # 0 "" 2
#NO_APP
	sw	$3,800($sp)
	sw	$2,804($sp)
	sw	$4,808($sp)
	lw	$2,808($sp)
	bne	$2,$0,$L334
	lui	$2,%hi(gpPonSysData)
	lw	$2,%lo(gpPonSysData)($2)
	li	$3,1			# 0x1
	sw	$3,0($2)
	j	$L332
$L334:
	addiu	$2,$sp,1720
	sw	$2,812($sp)
	lui	$2,%hi($LC25)
	addiu	$2,$2,%lo($LC25)
	sw	$2,816($sp)
	lw	$3,812($sp)
	lw	$2,816($sp)
#APP
 # 77 "/opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/string.h" 1
	.set	noreorder
	.set	noat
	lbu	$4,($3)
1:	lbu	$1,($2)
	addiu	$3,1
	bne	$1,$4,2f
	addiu	$2,1
	bnez	$4,1b
	lbu	$4,($3)
	move	$4,$1
2:	subu	$4,$1
3:	.set	at
	.set	reorder
 # 0 "" 2
#NO_APP
	sw	$3,812($sp)
	sw	$2,816($sp)
	sw	$4,820($sp)
	lw	$2,820($sp)
	bne	$2,$0,$L336
	lui	$2,%hi(gpPonSysData)
	lw	$2,%lo(gpPonSysData)($2)
	li	$3,2			# 0x2
	sw	$3,0($2)
	j	$L332
$L336:
	addiu	$2,$sp,1720
	sw	$2,824($sp)
	lui	$2,%hi($LC51)
	addiu	$2,$2,%lo($LC51)
	sw	$2,828($sp)
	lw	$3,824($sp)
	lw	$2,828($sp)
#APP
 # 77 "/opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/string.h" 1
	.set	noreorder
	.set	noat
	lbu	$4,($3)
1:	lbu	$1,($2)
	addiu	$3,1
	bne	$1,$4,2f
	addiu	$2,1
	bnez	$4,1b
	lbu	$4,($3)
	move	$4,$1
2:	subu	$4,$1
3:	.set	at
	.set	reorder
 # 0 "" 2
#NO_APP
	sw	$3,824($sp)
	sw	$2,828($sp)
	sw	$4,832($sp)
	lw	$2,832($sp)
	bne	$2,$0,$L338
	lw	$2,1752($sp)
	move	$4,$2
	lui	$2,%hi(gpon_dvt_eqd_adjustment)
	addiu	$2,$2,%lo(gpon_dvt_eqd_adjustment)
	jalr	$2
	lw	$3,1752($sp)
	lui	$2,%hi($LC77)
	addiu	$4,$2,%lo($LC77)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	j	$L332
$L338:
	addiu	$2,$sp,1720
	sw	$2,836($sp)
	lui	$2,%hi($LC78)
	addiu	$2,$2,%lo($LC78)
	sw	$2,840($sp)
	lw	$3,836($sp)
	lw	$2,840($sp)
#APP
 # 77 "/opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/string.h" 1
	.set	noreorder
	.set	noat
	lbu	$4,($3)
1:	lbu	$1,($2)
	addiu	$3,1
	bne	$1,$4,2f
	addiu	$2,1
	bnez	$4,1b
	lbu	$4,($3)
	move	$4,$1
2:	subu	$4,$1
3:	.set	at
	.set	reorder
 # 0 "" 2
#NO_APP
	sw	$3,836($sp)
	sw	$2,840($sp)
	sw	$4,844($sp)
	lw	$2,844($sp)
	bne	$2,$0,$L340
	lw	$3,1752($sp)
	li	$2,1			# 0x1
	bne	$3,$2,$L341
	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	li	$3,1			# 0x1
	sb	$3,-31061($2)
	lui	$2,%hi($LC79)
	addiu	$4,$2,%lo($LC79)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	j	$L332
$L341:
	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	sb	$0,-31061($2)
	lui	$2,%hi($LC80)
	addiu	$4,$2,%lo($LC80)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	j	$L332
$L340:
	addiu	$2,$sp,1720
	sw	$2,848($sp)
	lui	$2,%hi($LC81)
	addiu	$2,$2,%lo($LC81)
	sw	$2,852($sp)
	lw	$3,848($sp)
	lw	$2,852($sp)
#APP
 # 77 "/opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/string.h" 1
	.set	noreorder
	.set	noat
	lbu	$4,($3)
1:	lbu	$1,($2)
	addiu	$3,1
	bne	$1,$4,2f
	addiu	$2,1
	bnez	$4,1b
	lbu	$4,($3)
	move	$4,$1
2:	subu	$4,$1
3:	.set	at
	.set	reorder
 # 0 "" 2
#NO_APP
	sw	$3,848($sp)
	sw	$2,852($sp)
	sw	$4,856($sp)
	lw	$2,856($sp)
	beq	$2,$0,$L332
	addiu	$2,$sp,1720
	sw	$2,860($sp)
	lui	$2,%hi($LC82)
	addiu	$2,$2,%lo($LC82)
	sw	$2,864($sp)
	lw	$3,860($sp)
	lw	$2,864($sp)
#APP
 # 77 "/opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/string.h" 1
	.set	noreorder
	.set	noat
	lbu	$4,($3)
1:	lbu	$1,($2)
	addiu	$3,1
	bne	$1,$4,2f
	addiu	$2,1
	bnez	$4,1b
	lbu	$4,($3)
	move	$4,$1
2:	subu	$4,$1
3:	.set	at
	.set	reorder
 # 0 "" 2
#NO_APP
	sw	$3,860($sp)
	sw	$2,864($sp)
	sw	$4,868($sp)
	lw	$2,868($sp)
	bne	$2,$0,$L345
	lui	$2,%hi(gpGponPriv)
	lw	$3,%lo(gpGponPriv)($2)
	lw	$2,1752($sp)
	sltu	$2,$0,$2
	andi	$4,$2,0x00ff
	lhu	$2,448($3)
	ins	$2,$4,12,1
	sh	$2,448($3)
	lw	$2,1752($sp)
	beq	$2,$0,$L346
	lui	$2,%hi($LC83)
	addiu	$3,$2,%lo($LC83)
	j	$L347
$L346:
	lui	$2,%hi($LC84)
	addiu	$3,$2,%lo($LC84)
$L347:
	lw	$2,1752($sp)
	beq	$2,$0,$L348
	lui	$2,%hi($LC85)
	addiu	$2,$2,%lo($LC85)
	j	$L349
$L348:
	lui	$2,%hi($LC86)
	addiu	$2,$2,%lo($LC86)
$L349:
	lui	$4,%hi($LC87)
	addiu	$4,$4,%lo($LC87)
	move	$5,$3
	move	$6,$2
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	j	$L332
$L345:
	addiu	$2,$sp,1720
	sw	$2,872($sp)
	lui	$2,%hi($LC88)
	addiu	$2,$2,%lo($LC88)
	sw	$2,876($sp)
	lw	$3,872($sp)
	lw	$2,876($sp)
#APP
 # 77 "/opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/string.h" 1
	.set	noreorder
	.set	noat
	lbu	$4,($3)
1:	lbu	$1,($2)
	addiu	$3,1
	bne	$1,$4,2f
	addiu	$2,1
	bnez	$4,1b
	lbu	$4,($3)
	move	$4,$1
2:	subu	$4,$1
3:	.set	at
	.set	reorder
 # 0 "" 2
#NO_APP
	sw	$3,872($sp)
	sw	$2,876($sp)
	sw	$4,880($sp)
	lw	$2,880($sp)
	bne	$2,$0,$L351
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lw	$3,1752($sp)
	andi	$3,$3,0x00ff
	sb	$3,277($2)
	lw	$3,1752($sp)
	lui	$2,%hi($LC89)
	addiu	$4,$2,%lo($LC89)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	j	$L332
$L351:
	addiu	$2,$sp,1720
	sw	$2,884($sp)
	lui	$2,%hi($LC90)
	addiu	$2,$2,%lo($LC90)
	sw	$2,888($sp)
	lw	$3,884($sp)
	lw	$2,888($sp)
#APP
 # 77 "/opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/string.h" 1
	.set	noreorder
	.set	noat
	lbu	$4,($3)
1:	lbu	$1,($2)
	addiu	$3,1
	bne	$1,$4,2f
	addiu	$2,1
	bnez	$4,1b
	lbu	$4,($3)
	move	$4,$1
2:	subu	$4,$1
3:	.set	at
	.set	reorder
 # 0 "" 2
#NO_APP
	sw	$3,884($sp)
	sw	$2,888($sp)
	sw	$4,892($sp)
	lw	$2,892($sp)
	bne	$2,$0,$L332
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lw	$3,1752($sp)
	andi	$3,$3,0x00ff
	sb	$3,278($2)
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lbu	$2,278($2)
	move	$3,$2
	lui	$2,%hi($LC91)
	addiu	$4,$2,%lo($LC91)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lw	$2,268($2)
	move	$4,$2
	lui	$2,%hi(gpon_dvt_eqd_adjustment)
	addiu	$2,$2,%lo(gpon_dvt_eqd_adjustment)
	jalr	$2
$L332:
	addiu	$2,$sp,1720
	sw	$2,896($sp)
	lui	$2,%hi($LC92)
	addiu	$2,$2,%lo($LC92)
	sw	$2,900($sp)
	lw	$3,896($sp)
	lw	$2,900($sp)
#APP
 # 77 "/opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/string.h" 1
	.set	noreorder
	.set	noat
	lbu	$4,($3)
1:	lbu	$1,($2)
	addiu	$3,1
	bne	$1,$4,2f
	addiu	$2,1
	bnez	$4,1b
	lbu	$4,($3)
	move	$4,$1
2:	subu	$4,$1
3:	.set	at
	.set	reorder
 # 0 "" 2
#NO_APP
	sw	$3,896($sp)
	sw	$2,900($sp)
	sw	$4,904($sp)
	lw	$2,904($sp)
	bne	$2,$0,$L354
	lw	$2,1752($sp)
	move	$4,$2
	lui	$2,%hi(gpon_dvt_onu_connection)
	addiu	$2,$2,%lo(gpon_dvt_onu_connection)
	jalr	$2
	lui	$2,%hi($LC93)
	addiu	$4,$2,%lo($LC93)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
$L354:
	addiu	$2,$sp,1720
	sw	$2,908($sp)
	lui	$2,%hi($LC94)
	addiu	$2,$2,%lo($LC94)
	sw	$2,912($sp)
	lw	$3,908($sp)
	lw	$2,912($sp)
#APP
 # 77 "/opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/string.h" 1
	.set	noreorder
	.set	noat
	lbu	$4,($3)
1:	lbu	$1,($2)
	addiu	$3,1
	bne	$1,$4,2f
	addiu	$2,1
	bnez	$4,1b
	lbu	$4,($3)
	move	$4,$1
2:	subu	$4,$1
3:	.set	at
	.set	reorder
 # 0 "" 2
#NO_APP
	sw	$3,908($sp)
	sw	$2,912($sp)
	sw	$4,916($sp)
	lw	$2,916($sp)
	bne	$2,$0,$L303
	lw	$2,1752($sp)
	move	$4,$2
	lui	$2,%hi(gpon_dvt_transceiver_on_off_connection)
	addiu	$2,$2,%lo(gpon_dvt_transceiver_on_off_connection)
	jalr	$2
	lui	$2,%hi($LC95)
	addiu	$4,$2,%lo($LC95)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	j	$L303
$L326:
	addiu	$2,$sp,1688
	sw	$2,920($sp)
	lui	$2,%hi($LC96)
	addiu	$2,$2,%lo($LC96)
	sw	$2,924($sp)
	lw	$3,920($sp)
	lw	$2,924($sp)
#APP
 # 77 "/opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/string.h" 1
	.set	noreorder
	.set	noat
	lbu	$4,($3)
1:	lbu	$1,($2)
	addiu	$3,1
	bne	$1,$4,2f
	addiu	$2,1
	bnez	$4,1b
	lbu	$4,($3)
	move	$4,$1
2:	subu	$4,$1
3:	.set	at
	.set	reorder
 # 0 "" 2
#NO_APP
	sw	$3,920($sp)
	sw	$2,924($sp)
	sw	$4,928($sp)
	lw	$2,928($sp)
	bne	$2,$0,$L358
	addiu	$2,$sp,1720
	sw	$2,932($sp)
	lui	$2,%hi($LC30)
	addiu	$2,$2,%lo($LC30)
	sw	$2,936($sp)
	lw	$3,932($sp)
	lw	$2,936($sp)
#APP
 # 77 "/opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/string.h" 1
	.set	noreorder
	.set	noat
	lbu	$4,($3)
1:	lbu	$1,($2)
	addiu	$3,1
	bne	$1,$4,2f
	addiu	$2,1
	bnez	$4,1b
	lbu	$4,($3)
	move	$4,$1
2:	subu	$4,$1
3:	.set	at
	.set	reorder
 # 0 "" 2
#NO_APP
	sw	$3,932($sp)
	sw	$2,936($sp)
	sw	$4,940($sp)
	lw	$2,940($sp)
	bne	$2,$0,$L303
	lw	$2,1752($sp)
	bne	$2,$0,$L361
	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$3,$3,$2
	lbu	$2,-30828($3)
	ins	$2,$0,7,1
	sb	$2,-30828($3)
	move	$4,$0
	lui	$2,%hi(gponDevHardwareDyingGasp)
	addiu	$2,$2,%lo(gponDevHardwareDyingGasp)
	jalr	$2
	lui	$2,%hi($LC31)
	addiu	$4,$2,%lo($LC31)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	j	$L303
$L361:
	lw	$3,1752($sp)
	li	$2,1			# 0x1
	bne	$3,$2,$L362
	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$3,$3,$2
	lbu	$2,-30828($3)
	li	$4,1			# 0x1
	ins	$2,$4,7,1
	sb	$2,-30828($3)
	li	$4,1			# 0x1
	lui	$2,%hi(gponDevHardwareDyingGasp)
	addiu	$2,$2,%lo(gponDevHardwareDyingGasp)
	jalr	$2
	lui	$2,%hi($LC32)
	addiu	$4,$2,%lo($LC32)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	j	$L303
$L362:
	lw	$3,1752($sp)
	li	$2,16			# 0x10
	bne	$3,$2,$L363
	lui	$2,%hi(ploam_send_dying_gasp)
	addiu	$2,$2,%lo(ploam_send_dying_gasp)
	jalr	$2
	j	$L303
$L363:
	lw	$3,1752($sp)
	li	$2,17			# 0x11
	bne	$3,$2,$L303
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16468
	move	$4,$0
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16468
	move	$4,$0
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16468
	move	$4,$0
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$3,%lo(g_gpon_mac_reg_BASE)($2)
	lw	$2,16924($3)
	li	$4,1			# 0x1
	ins	$2,$4,16,1
	sw	$2,16924($3)
	j	$L303
$L358:
	addiu	$2,$sp,1688
	sw	$2,944($sp)
	lui	$2,%hi($LC97)
	addiu	$2,$2,%lo($LC97)
	sw	$2,948($sp)
	lw	$3,944($sp)
	lw	$2,948($sp)
#APP
 # 77 "/opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/string.h" 1
	.set	noreorder
	.set	noat
	lbu	$4,($3)
1:	lbu	$1,($2)
	addiu	$3,1
	bne	$1,$4,2f
	addiu	$2,1
	bnez	$4,1b
	lbu	$4,($3)
	move	$4,$1
2:	subu	$4,$1
3:	.set	at
	.set	reorder
 # 0 "" 2
#NO_APP
	sw	$3,944($sp)
	sw	$2,948($sp)
	sw	$4,952($sp)
	lw	$2,952($sp)
	bne	$2,$0,$L365
	addiu	$2,$sp,1804
	move	$4,$2
	move	$5,$0
	li	$6,21			# 0x15
	lui	$2,%hi(memset)
	addiu	$2,$2,%lo(memset)
	jalr	$2
	addiu	$3,$sp,1860
	li	$2,1074003968			# 0x40040000
	ori	$4,$2,0xd910
	move	$5,$3
	lui	$2,%hi(gpon_cmd_proc)
	addiu	$2,$2,%lo(gpon_cmd_proc)
	jalr	$2
	addiu	$2,$sp,1804
	addiu	$3,$2,8
	addiu	$2,$sp,1860
	addiu	$2,$2,12
	move	$4,$3
	move	$5,$2
	li	$6,10			# 0xa
	lui	$2,%hi(memcpy)
	addiu	$2,$2,%lo(memcpy)
	jalr	$2
	addiu	$3,$sp,1804
	addiu	$2,$sp,1720
	move	$4,$3
	move	$5,$2
	li	$6,4			# 0x4
	lui	$2,%hi(memcpy)
	addiu	$2,$2,%lo(memcpy)
	jalr	$2
	lw	$2,1752($sp)
	srl	$2,$2,24
	andi	$2,$2,0x00ff
	sb	$2,1808($sp)
	lw	$2,1752($sp)
	srl	$2,$2,16
	andi	$2,$2,0x00ff
	sb	$2,1809($sp)
	lw	$2,1752($sp)
	srl	$2,$2,8
	andi	$2,$2,0x00ff
	sb	$2,1810($sp)
	lw	$2,1752($sp)
	andi	$2,$2,0x00ff
	sb	$2,1811($sp)
	addiu	$3,$sp,1804
	li	$2,-2147221504			# 0xffffffff80040000
	ori	$4,$2,0xd901
	move	$5,$3
	lui	$2,%hi(gpon_cmd_proc)
	addiu	$2,$2,%lo(gpon_cmd_proc)
	jalr	$2
	j	$L303
$L365:
	addiu	$2,$sp,1688
	sw	$2,956($sp)
	lui	$2,%hi($LC98)
	addiu	$2,$2,%lo($LC98)
	sw	$2,960($sp)
	lw	$3,956($sp)
	lw	$2,960($sp)
#APP
 # 77 "/opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/string.h" 1
	.set	noreorder
	.set	noat
	lbu	$4,($3)
1:	lbu	$1,($2)
	addiu	$3,1
	bne	$1,$4,2f
	addiu	$2,1
	bnez	$4,1b
	lbu	$4,($3)
	move	$4,$1
2:	subu	$4,$1
3:	.set	at
	.set	reorder
 # 0 "" 2
#NO_APP
	sw	$3,956($sp)
	sw	$2,960($sp)
	sw	$4,964($sp)
	lw	$2,964($sp)
	bne	$2,$0,$L367
	addiu	$2,$sp,1720
	move	$4,$2
	lui	$2,%hi(strlen)
	addiu	$2,$2,%lo(strlen)
	jalr	$2
	sltu	$2,$2,11
	beq	$2,$0,$L303
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	addiu	$2,$2,216
	move	$4,$2
	move	$5,$0
	li	$6,10			# 0xa
	lui	$2,%hi(memset)
	addiu	$2,$2,%lo(memset)
	jalr	$2
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	addiu	$2,$2,216
	sw	$2,968($sp)
	addiu	$2,$sp,1720
	sw	$2,972($sp)
	lw	$2,968($sp)
	sw	$2,976($sp)
	lw	$3,968($sp)
	lw	$2,972($sp)
#APP
 # 27 "/opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/string.h" 1
	.set	noreorder
	.set	noat
1:	lbu	$1,($2)
	addiu	$2,1
	sb	$1,($3)
	bnez	$1,1b
	addiu	$3,1
	.set	at
	.set	reorder
 # 0 "" 2
#NO_APP
	sw	$3,968($sp)
	sw	$2,972($sp)
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	addiu	$3,$2,216
	lui	$2,%hi($LC99)
	addiu	$4,$2,%lo($LC99)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	j	$L303
$L367:
	addiu	$2,$sp,1688
	sw	$2,980($sp)
	lui	$2,%hi($LC100)
	addiu	$2,$2,%lo($LC100)
	sw	$2,984($sp)
	lw	$3,980($sp)
	lw	$2,984($sp)
#APP
 # 77 "/opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/string.h" 1
	.set	noreorder
	.set	noat
	lbu	$4,($3)
1:	lbu	$1,($2)
	addiu	$3,1
	bne	$1,$4,2f
	addiu	$2,1
	bnez	$4,1b
	lbu	$4,($3)
	move	$4,$1
2:	subu	$4,$1
3:	.set	at
	.set	reorder
 # 0 "" 2
#NO_APP
	sw	$3,980($sp)
	sw	$2,984($sp)
	sw	$4,988($sp)
	lw	$2,988($sp)
	bne	$2,$0,$L371
	addiu	$2,$sp,1720
	sw	$2,992($sp)
	lui	$2,%hi($LC101)
	addiu	$2,$2,%lo($LC101)
	sw	$2,996($sp)
	lw	$3,992($sp)
	lw	$2,996($sp)
#APP
 # 77 "/opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/string.h" 1
	.set	noreorder
	.set	noat
	lbu	$4,($3)
1:	lbu	$1,($2)
	addiu	$3,1
	bne	$1,$4,2f
	addiu	$2,1
	bnez	$4,1b
	lbu	$4,($3)
	move	$4,$1
2:	subu	$4,$1
3:	.set	at
	.set	reorder
 # 0 "" 2
#NO_APP
	sw	$3,992($sp)
	sw	$2,996($sp)
	sw	$4,1000($sp)
	lw	$2,1000($sp)
	bne	$2,$0,$L373
	lw	$2,1752($sp)
	move	$3,$2
	lui	$2,%hi(xpon_mac_print_open)
	sw	$3,%lo(xpon_mac_print_open)($2)
	j	$L303
$L373:
	addiu	$2,$sp,1720
	sw	$2,1004($sp)
	lui	$2,%hi($LC102)
	addiu	$2,$2,%lo($LC102)
	sw	$2,1008($sp)
	lw	$3,1004($sp)
	lw	$2,1008($sp)
#APP
 # 77 "/opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/string.h" 1
	.set	noreorder
	.set	noat
	lbu	$4,($3)
1:	lbu	$1,($2)
	addiu	$3,1
	bne	$1,$4,2f
	addiu	$2,1
	bnez	$4,1b
	lbu	$4,($3)
	move	$4,$1
2:	subu	$4,$1
3:	.set	at
	.set	reorder
 # 0 "" 2
#NO_APP
	sw	$3,1004($sp)
	sw	$2,1008($sp)
	sw	$4,1012($sp)
	lw	$2,1012($sp)
	bne	$2,$0,$L375
	lw	$2,1752($sp)
	move	$3,$2
	lui	$2,%hi(drop_print_flag)
	sw	$3,%lo(drop_print_flag)($2)
	j	$L303
$L375:
	addiu	$2,$sp,1720
	sw	$2,1016($sp)
	lui	$2,%hi($LC55)
	addiu	$2,$2,%lo($LC55)
	sw	$2,1020($sp)
	lw	$3,1016($sp)
	lw	$2,1020($sp)
#APP
 # 77 "/opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/string.h" 1
	.set	noreorder
	.set	noat
	lbu	$4,($3)
1:	lbu	$1,($2)
	addiu	$3,1
	bne	$1,$4,2f
	addiu	$2,1
	bnez	$4,1b
	lbu	$4,($3)
	move	$4,$1
2:	subu	$4,$1
3:	.set	at
	.set	reorder
 # 0 "" 2
#NO_APP
	sw	$3,1016($sp)
	sw	$2,1020($sp)
	sw	$4,1024($sp)
	lw	$2,1024($sp)
	bne	$2,$0,$L377
	addiu	$2,$sp,1860
	move	$4,$2
	lui	$2,%hi(xmcs_get_gem_port_info)
	addiu	$2,$2,%lo(xmcs_get_gem_port_info)
	jalr	$2
	bgez	$2,$L378
	lui	$2,%hi($LC103)
	addiu	$4,$2,%lo($LC103)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	j	$L379
$L378:
	sw	$0,32($sp)
	j	$L380
$L385:
	lw	$2,32($sp)
	sll	$2,$2,2
	sll	$3,$2,2
	addu	$2,$2,$3
	addiu	$3,$sp,32
	addu	$2,$3,$2
	lhu	$2,1832($2)
	sw	$2,80($sp)
	lui	$2,%hi(gpWanPriv)
	lw	$3,%lo(gpWanPriv)($2)
	lw	$2,80($sp)
	addiu	$2,$2,44
	sll	$2,$2,1
	addu	$2,$3,$2
	lhu	$2,4($2)
	andi	$2,$2,0x7fff
	sw	$2,84($sp)
	lw	$2,32($sp)
	sll	$2,$2,2
	sll	$3,$2,2
	addu	$2,$2,$3
	addiu	$3,$sp,32
	addu	$2,$3,$2
	lhu	$2,1832($2)
	move	$10,$2
	lw	$2,32($sp)
	sll	$2,$2,2
	sll	$3,$2,2
	addu	$2,$2,$3
	addiu	$3,$sp,32
	addu	$2,$3,$2
	lhu	$2,1834($2)
	move	$9,$2
	lw	$2,32($sp)
	sll	$2,$2,2
	sll	$3,$2,2
	addu	$2,$2,$3
	addiu	$3,$sp,32
	addu	$2,$3,$2
	lhu	$2,1836($2)
	move	$8,$2
	lw	$2,32($sp)
	sll	$2,$2,2
	sll	$3,$2,2
	addu	$2,$2,$3
	addiu	$3,$sp,32
	addu	$2,$3,$2
	lw	$2,1840($2)
	beq	$2,$0,$L381
	lui	$2,%hi($LC104)
	addiu	$4,$2,%lo($LC104)
	j	$L382
$L381:
	lui	$2,%hi($LC105)
	addiu	$4,$2,%lo($LC105)
$L382:
	lw	$2,32($sp)
	sll	$2,$2,2
	sll	$3,$2,2
	addu	$2,$2,$3
	addiu	$3,$sp,32
	addu	$2,$3,$2
	lw	$2,1844($2)
	beq	$2,$0,$L383
	lui	$2,%hi($LC104)
	addiu	$3,$2,%lo($LC104)
	j	$L384
$L383:
	lui	$2,%hi($LC105)
	addiu	$3,$2,%lo($LC105)
$L384:
	lui	$2,%hi(gpWanPriv)
	lw	$5,%lo(gpWanPriv)($2)
	lw	$2,84($sp)
	sll	$2,$2,5
	sll	$6,$2,2
	subu	$2,$6,$2
	addu	$2,$5,$2
	lw	$2,8284($2)
	ext	$2,$2,2,1
	andi	$2,$2,0x00ff
	move	$7,$2
	lui	$2,%hi(gpWanPriv)
	lw	$5,%lo(gpWanPriv)($2)
	lw	$2,84($sp)
	sll	$2,$2,5
	sll	$6,$2,2
	subu	$2,$6,$2
	addu	$2,$5,$2
	lw	$2,8284($2)
	ext	$2,$2,14,6
	andi	$2,$2,0x00ff
	sw	$4,16($sp)
	sw	$3,20($sp)
	sw	$7,24($sp)
	sw	$2,28($sp)
	lui	$2,%hi($LC106)
	addiu	$4,$2,%lo($LC106)
	move	$5,$10
	move	$6,$9
	move	$7,$8
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	lw	$2,32($sp)
	addiu	$2,$2,1
	sw	$2,32($sp)
$L380:
	lhu	$2,6980($sp)
	move	$3,$2
	lw	$2,32($sp)
	slt	$2,$2,$3
	bne	$2,$0,$L385
$L379:
	j	$L303
$L377:
	addiu	$2,$sp,1720
	sw	$2,1028($sp)
	lui	$2,%hi($LC107)
	addiu	$2,$2,%lo($LC107)
	sw	$2,1032($sp)
	lw	$3,1028($sp)
	lw	$2,1032($sp)
#APP
 # 77 "/opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/string.h" 1
	.set	noreorder
	.set	noat
	lbu	$4,($3)
1:	lbu	$1,($2)
	addiu	$3,1
	bne	$1,$4,2f
	addiu	$2,1
	bnez	$4,1b
	lbu	$4,($3)
	move	$4,$1
2:	subu	$4,$1
3:	.set	at
	.set	reorder
 # 0 "" 2
#NO_APP
	sw	$3,1028($sp)
	sw	$2,1032($sp)
	sw	$4,1036($sp)
	lw	$2,1036($sp)
	bne	$2,$0,$L387
	addiu	$2,$sp,1860
	move	$4,$2
	move	$5,$0
	li	$6,132			# 0x84
	lui	$2,%hi(memset)
	addiu	$2,$2,%lo(memset)
	jalr	$2
	addiu	$2,$sp,1860
	move	$4,$2
	lui	$2,%hi(xmcs_get_llid_info)
	addiu	$2,$2,%lo(xmcs_get_llid_info)
	jalr	$2
	bgez	$2,$L388
	lui	$2,%hi($LC103)
	addiu	$4,$2,%lo($LC103)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	j	$L389
$L388:
	sw	$0,32($sp)
	j	$L390
$L391:
	lw	$2,32($sp)
	sll	$2,$2,4
	addiu	$3,$sp,32
	addu	$2,$3,$2
	lbu	$2,1828($2)
	move	$5,$2
	lw	$2,32($sp)
	sll	$2,$2,4
	addiu	$3,$sp,32
	addu	$2,$3,$2
	lhu	$2,1830($2)
	move	$6,$2
	lw	$2,32($sp)
	sll	$2,$2,4
	addiu	$3,$sp,32
	addu	$2,$3,$2
	lbu	$2,1832($2)
	move	$7,$2
	lw	$2,32($sp)
	sll	$2,$2,4
	addiu	$3,$sp,32
	addu	$2,$3,$2
	lw	$3,1836($2)
	lw	$2,32($sp)
	sll	$2,$2,4
	addiu	$4,$sp,32
	addu	$2,$4,$2
	lw	$2,1840($2)
	sw	$3,16($sp)
	sw	$2,20($sp)
	lui	$2,%hi($LC108)
	addiu	$4,$2,%lo($LC108)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	lw	$2,32($sp)
	addiu	$2,$2,1
	sw	$2,32($sp)
$L390:
	lhu	$2,1988($sp)
	move	$3,$2
	lw	$2,32($sp)
	slt	$2,$2,$3
	bne	$2,$0,$L391
$L389:
	j	$L303
$L387:
	addiu	$2,$sp,1720
	sw	$2,1040($sp)
	lui	$2,%hi($LC56)
	addiu	$2,$2,%lo($LC56)
	sw	$2,1044($sp)
	lw	$3,1040($sp)
	lw	$2,1044($sp)
#APP
 # 77 "/opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/string.h" 1
	.set	noreorder
	.set	noat
	lbu	$4,($3)
1:	lbu	$1,($2)
	addiu	$3,1
	bne	$1,$4,2f
	addiu	$2,1
	bnez	$4,1b
	lbu	$4,($3)
	move	$4,$1
2:	subu	$4,$1
3:	.set	at
	.set	reorder
 # 0 "" 2
#NO_APP
	sw	$3,1040($sp)
	sw	$2,1044($sp)
	sw	$4,1048($sp)
	lw	$2,1048($sp)
	bne	$2,$0,$L393
	addiu	$2,$sp,1860
	move	$4,$2
	lui	$2,%hi(xmcs_get_tcont_info)
	addiu	$2,$2,%lo(xmcs_get_tcont_info)
	jalr	$2
	bgez	$2,$L394
	lui	$2,%hi($LC103)
	addiu	$4,$2,%lo($LC103)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	j	$L395
$L394:
	sw	$0,32($sp)
	j	$L396
$L397:
	lw	$2,32($sp)
	sll	$2,$2,2
	addiu	$3,$sp,32
	addu	$2,$3,$2
	lhu	$2,1828($2)
	move	$5,$2
	lw	$2,32($sp)
	sll	$2,$2,2
	addiu	$3,$sp,32
	addu	$2,$3,$2
	lbu	$2,1830($2)
	move	$3,$2
	lui	$2,%hi($LC109)
	addiu	$4,$2,%lo($LC109)
	move	$6,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	lw	$2,32($sp)
	addiu	$2,$2,1
	sw	$2,32($sp)
$L396:
	lhu	$2,1988($sp)
	move	$3,$2
	lw	$2,32($sp)
	slt	$2,$2,$3
	bne	$2,$0,$L397
$L395:
	j	$L303
$L393:
	addiu	$2,$sp,1720
	sw	$2,1052($sp)
	lui	$2,%hi($LC110)
	addiu	$2,$2,%lo($LC110)
	sw	$2,1056($sp)
	lw	$3,1052($sp)
	lw	$2,1056($sp)
#APP
 # 77 "/opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/string.h" 1
	.set	noreorder
	.set	noat
	lbu	$4,($3)
1:	lbu	$1,($2)
	addiu	$3,1
	bne	$1,$4,2f
	addiu	$2,1
	bnez	$4,1b
	lbu	$4,($3)
	move	$4,$1
2:	subu	$4,$1
3:	.set	at
	.set	reorder
 # 0 "" 2
#NO_APP
	sw	$3,1052($sp)
	sw	$2,1056($sp)
	sw	$4,1060($sp)
	lw	$2,1060($sp)
	bne	$2,$0,$L399
	addiu	$2,$sp,1828
	move	$4,$2
	lui	$2,%hi(gponDevGetSuperframe)
	addiu	$2,$2,%lo(gponDevGetSuperframe)
	jalr	$2
	lw	$3,1828($sp)
	lui	$2,%hi($LC111)
	addiu	$4,$2,%lo($LC111)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	j	$L303
$L399:
	addiu	$2,$sp,1720
	sw	$2,1064($sp)
	lui	$2,%hi($LC112)
	addiu	$2,$2,%lo($LC112)
	sw	$2,1068($sp)
	lw	$3,1064($sp)
	lw	$2,1068($sp)
#APP
 # 77 "/opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/string.h" 1
	.set	noreorder
	.set	noat
	lbu	$4,($3)
1:	lbu	$1,($2)
	addiu	$3,1
	bne	$1,$4,2f
	addiu	$2,1
	bnez	$4,1b
	lbu	$4,($3)
	move	$4,$1
2:	subu	$4,$1
3:	.set	at
	.set	reorder
 # 0 "" 2
#NO_APP
	sw	$3,1064($sp)
	sw	$2,1068($sp)
	sw	$4,1072($sp)
	lw	$2,1072($sp)
	bne	$2,$0,$L401
	addiu	$3,$sp,1860
	li	$2,1074003968			# 0x40040000
	ori	$4,$2,0xd910
	move	$5,$3
	lui	$2,%hi(gpon_cmd_proc)
	addiu	$2,$2,%lo(gpon_cmd_proc)
	jalr	$2
	lui	$2,%hi($LC113)
	addiu	$4,$2,%lo($LC113)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	lbu	$2,1860($sp)
	move	$3,$2
	lui	$2,%hi($LC114)
	addiu	$4,$2,%lo($LC114)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	lbu	$2,1861($sp)
	move	$3,$2
	lui	$2,%hi($LC115)
	addiu	$4,$2,%lo($LC115)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	lui	$2,%hi($LC116)
	addiu	$4,$2,%lo($LC116)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	sw	$0,32($sp)
	j	$L402
$L403:
	lw	$2,32($sp)
	addiu	$3,$sp,32
	addu	$2,$3,$2
	lbu	$2,1830($2)
	move	$3,$2
	lui	$2,%hi($LC117)
	addiu	$4,$2,%lo($LC117)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	lw	$2,32($sp)
	addiu	$2,$2,1
	sw	$2,32($sp)
$L402:
	lw	$2,32($sp)
	slt	$2,$2,4
	bne	$2,$0,$L403
	li	$2,4			# 0x4
	sw	$2,32($sp)
	j	$L404
$L405:
	lw	$2,32($sp)
	addiu	$3,$sp,32
	addu	$2,$3,$2
	lbu	$2,1830($2)
	move	$3,$2
	lui	$2,%hi($LC118)
	addiu	$4,$2,%lo($LC118)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	lw	$2,32($sp)
	addiu	$2,$2,1
	sw	$2,32($sp)
$L404:
	lw	$2,32($sp)
	slt	$2,$2,8
	bne	$2,$0,$L405
	addiu	$2,$sp,1860
	addiu	$3,$2,12
	lui	$2,%hi($LC119)
	addiu	$4,$2,%lo($LC119)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	lbu	$2,1882($sp)
	move	$3,$2
	lui	$2,%hi($LC120)
	addiu	$4,$2,%lo($LC120)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	lui	$2,%hi($LC121)
	addiu	$4,$2,%lo($LC121)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	sw	$0,32($sp)
	j	$L406
$L407:
	lw	$2,32($sp)
	addiu	$3,$sp,32
	addu	$2,$3,$2
	lbu	$2,1851($2)
	move	$3,$2
	lui	$2,%hi($LC118)
	addiu	$4,$2,%lo($LC118)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	lw	$2,32($sp)
	addiu	$2,$2,1
	sw	$2,32($sp)
$L406:
	lw	$2,32($sp)
	slt	$2,$2,16
	bne	$2,$0,$L407
	lw	$3,1908($sp)
	lui	$2,%hi($LC122)
	addiu	$4,$2,%lo($LC122)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	lw	$3,1912($sp)
	lui	$2,%hi($LC123)
	addiu	$4,$2,%lo($LC123)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	lhu	$2,1916($sp)
	move	$3,$2
	lui	$2,%hi($LC124)
	addiu	$4,$2,%lo($LC124)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	j	$L303
$L401:
	addiu	$2,$sp,1720
	sw	$2,1076($sp)
	lui	$2,%hi($LC125)
	addiu	$2,$2,%lo($LC125)
	sw	$2,1080($sp)
	lw	$3,1076($sp)
	lw	$2,1080($sp)
#APP
 # 77 "/opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/string.h" 1
	.set	noreorder
	.set	noat
	lbu	$4,($3)
1:	lbu	$1,($2)
	addiu	$3,1
	bne	$1,$4,2f
	addiu	$2,1
	bnez	$4,1b
	lbu	$4,($3)
	move	$4,$1
2:	subu	$4,$1
3:	.set	at
	.set	reorder
 # 0 "" 2
#NO_APP
	sw	$3,1076($sp)
	sw	$2,1080($sp)
	sw	$4,1084($sp)
	lw	$2,1084($sp)
	bne	$2,$0,$L409
	sw	$0,32($sp)
	j	$L410
$L413:
	lui	$2,%hi(gpWanPriv)
	lw	$3,%lo(gpWanPriv)($2)
	lw	$2,32($sp)
	sll	$2,$2,5
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	lw	$2,8284($2)
	ext	$2,$2,2,1
	andi	$2,$2,0x00ff
	beq	$2,$0,$L411
	lui	$2,%hi(gpWanPriv)
	lw	$3,%lo(gpWanPriv)($2)
	lw	$2,32($sp)
	sll	$2,$2,5
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	lw	$2,8284($2)
	ext	$2,$2,20,12
	andi	$2,$2,0xffff
	move	$3,$2
	lw	$2,1752($sp)
	beq	$3,$2,$L412
	lw	$3,1752($sp)
	li	$2,65536			# 0x10000
	bne	$3,$2,$L411
$L412:
	lui	$2,%hi(gpWanPriv)
	lw	$3,%lo(gpWanPriv)($2)
	lw	$2,32($sp)
	sll	$2,$2,5
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	lw	$2,8284($2)
	ext	$2,$2,20,12
	andi	$2,$2,0xffff
	move	$4,$2
	addiu	$3,$sp,1832
	addiu	$2,$sp,1836
	move	$5,$0
	move	$6,$3
	move	$7,$2
	lui	$2,%hi(gponDevGetGemPortCounter)
	addiu	$2,$2,%lo(gponDevGetGemPortCounter)
	jalr	$2
	lui	$2,%hi(gpWanPriv)
	lw	$3,%lo(gpWanPriv)($2)
	lw	$2,32($sp)
	sll	$2,$2,5
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	lw	$2,8284($2)
	ext	$2,$2,20,12
	andi	$2,$2,0xffff
	move	$4,$2
	addiu	$3,$sp,1832
	addiu	$2,$sp,1840
	li	$5,2			# 0x2
	move	$6,$3
	move	$7,$2
	lui	$2,%hi(gponDevGetGemPortCounter)
	addiu	$2,$2,%lo(gponDevGetGemPortCounter)
	jalr	$2
	lui	$2,%hi(gpWanPriv)
	lw	$3,%lo(gpWanPriv)($2)
	lw	$2,32($sp)
	sll	$2,$2,5
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	lw	$2,8284($2)
	ext	$2,$2,20,12
	andi	$2,$2,0xffff
	move	$5,$2
	lui	$2,%hi(gpWanPriv)
	lw	$3,%lo(gpWanPriv)($2)
	lw	$2,32($sp)
	sll	$2,$2,5
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	lw	$6,8292($2)
	lui	$2,%hi(gpWanPriv)
	lw	$3,%lo(gpWanPriv)($2)
	lw	$2,32($sp)
	sll	$2,$2,5
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	lw	$7,8288($2)
	lw	$3,1840($sp)
	lw	$2,1836($sp)
	sw	$3,16($sp)
	sw	$2,20($sp)
	lui	$2,%hi($LC126)
	addiu	$4,$2,%lo($LC126)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
$L411:
	lw	$2,32($sp)
	addiu	$2,$2,1
	sw	$2,32($sp)
$L410:
	lw	$2,32($sp)
	slt	$2,$2,256
	bne	$2,$0,$L413
	j	$L303
$L409:
	addiu	$2,$sp,1720
	sw	$2,1088($sp)
	lui	$2,%hi($LC127)
	addiu	$2,$2,%lo($LC127)
	sw	$2,1092($sp)
	lw	$3,1088($sp)
	lw	$2,1092($sp)
#APP
 # 77 "/opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/string.h" 1
	.set	noreorder
	.set	noat
	lbu	$4,($3)
1:	lbu	$1,($2)
	addiu	$3,1
	bne	$1,$4,2f
	addiu	$2,1
	bnez	$4,1b
	lbu	$4,($3)
	move	$4,$1
2:	subu	$4,$1
3:	.set	at
	.set	reorder
 # 0 "" 2
#NO_APP
	sw	$3,1088($sp)
	sw	$2,1092($sp)
	sw	$4,1096($sp)
	lw	$2,1096($sp)
	bne	$2,$0,$L415
	addiu	$2,$sp,1860
	move	$4,$2
	move	$5,$0
	li	$6,36			# 0x24
	lui	$2,%hi(memset)
	addiu	$2,$2,%lo(memset)
	jalr	$2
	lw	$2,1752($sp)
	andi	$2,$2,0xffff
	sh	$2,1860($sp)
	addiu	$2,$sp,1860
	move	$4,$2
	lui	$2,%hi(get_counter_from_reg)
	addiu	$2,$2,%lo(get_counter_from_reg)
	jalr	$2
	lhu	$2,1860($sp)
	move	$3,$2
	lui	$2,%hi($LC128)
	addiu	$4,$2,%lo($LC128)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	lw	$5,1864($sp)
	lw	$3,1868($sp)
	lui	$2,%hi($LC129)
	addiu	$4,$2,%lo($LC129)
	move	$6,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	lw	$5,1872($sp)
	lw	$3,1876($sp)
	lui	$2,%hi($LC130)
	addiu	$4,$2,%lo($LC130)
	move	$6,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	lw	$5,1880($sp)
	lw	$3,1884($sp)
	lui	$2,%hi($LC131)
	addiu	$4,$2,%lo($LC131)
	move	$6,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	lw	$5,1888($sp)
	lw	$3,1892($sp)
	lui	$2,%hi($LC132)
	addiu	$4,$2,%lo($LC132)
	move	$6,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	j	$L303
$L415:
	addiu	$2,$sp,1720
	sw	$2,1100($sp)
	lui	$2,%hi($LC133)
	addiu	$2,$2,%lo($LC133)
	sw	$2,1104($sp)
	lw	$3,1100($sp)
	lw	$2,1104($sp)
#APP
 # 77 "/opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/string.h" 1
	.set	noreorder
	.set	noat
	lbu	$4,($3)
1:	lbu	$1,($2)
	addiu	$3,1
	bne	$1,$4,2f
	addiu	$2,1
	bnez	$4,1b
	lbu	$4,($3)
	move	$4,$1
2:	subu	$4,$1
3:	.set	at
	.set	reorder
 # 0 "" 2
#NO_APP
	sw	$3,1100($sp)
	sw	$2,1104($sp)
	sw	$4,1108($sp)
	lw	$2,1108($sp)
	bne	$2,$0,$L303
	lui	$2,%hi($LC134)
	addiu	$4,$2,%lo($LC134)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	sw	$0,32($sp)
	j	$L417
$L418:
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lw	$3,480($2)
	lw	$2,32($sp)
	sll	$2,$2,2
	addu	$2,$3,$2
	lw	$3,0($2)
	lui	$2,%hi($LC135)
	addiu	$4,$2,%lo($LC135)
	lw	$5,32($sp)
	move	$6,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	lw	$2,32($sp)
	addiu	$2,$2,1
	sw	$2,32($sp)
$L417:
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lw	$2,480($2)
	lw	$3,2180($2)
	lw	$2,32($sp)
	slt	$2,$2,$3
	bne	$2,$0,$L418
	lui	$2,%hi($LC136)
	addiu	$4,$2,%lo($LC136)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	lui	$2,%hi($LC137)
	addiu	$4,$2,%lo($LC137)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	sw	$0,32($sp)
	j	$L419
$L420:
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lw	$3,480($2)
	lw	$2,32($sp)
	addiu	$2,$2,16
	sll	$2,$2,3
	addu	$2,$3,$2
	lhu	$2,2($2)
	move	$3,$2
	lui	$2,%hi($LC138)
	addiu	$4,$2,%lo($LC138)
	lw	$5,32($sp)
	move	$6,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lw	$3,480($2)
	lw	$2,32($sp)
	addiu	$2,$2,16
	sll	$2,$2,3
	addu	$2,$3,$2
	lhu	$2,0($2)
	move	$3,$2
	lui	$2,%hi($LC139)
	addiu	$4,$2,%lo($LC139)
	lw	$5,32($sp)
	move	$6,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lw	$3,480($2)
	lw	$2,32($sp)
	addiu	$2,$2,16
	sll	$2,$2,3
	addu	$2,$3,$2
	lbu	$2,4($2)
	move	$3,$2
	lui	$2,%hi($LC140)
	addiu	$4,$2,%lo($LC140)
	lw	$5,32($sp)
	move	$6,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lw	$3,480($2)
	lw	$2,32($sp)
	addiu	$2,$2,16
	sll	$2,$2,3
	addu	$2,$3,$2
	lbu	$2,5($2)
	move	$3,$2
	lui	$2,%hi($LC141)
	addiu	$4,$2,%lo($LC141)
	lw	$5,32($sp)
	move	$6,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	lui	$2,%hi($LC136)
	addiu	$4,$2,%lo($LC136)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	lw	$2,32($sp)
	addiu	$2,$2,1
	sw	$2,32($sp)
$L419:
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lw	$2,480($2)
	lw	$3,2176($2)
	lw	$2,32($sp)
	slt	$2,$2,$3
	bne	$2,$0,$L420
	j	$L303
$L371:
	addiu	$2,$sp,1688
	sw	$2,1112($sp)
	lui	$2,%hi($LC76)
	addiu	$2,$2,%lo($LC76)
	sw	$2,1116($sp)
	lw	$3,1112($sp)
	lw	$2,1116($sp)
#APP
 # 77 "/opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/string.h" 1
	.set	noreorder
	.set	noat
	lbu	$4,($3)
1:	lbu	$1,($2)
	addiu	$3,1
	bne	$1,$4,2f
	addiu	$2,1
	bnez	$4,1b
	lbu	$4,($3)
	move	$4,$1
2:	subu	$4,$1
3:	.set	at
	.set	reorder
 # 0 "" 2
#NO_APP
	sw	$3,1112($sp)
	sw	$2,1116($sp)
	sw	$4,1120($sp)
	lw	$2,1120($sp)
	bne	$2,$0,$L422
	addiu	$2,$sp,1720
	sw	$2,1124($sp)
	lui	$2,%hi($LC142)
	addiu	$2,$2,%lo($LC142)
	sw	$2,1128($sp)
	lw	$3,1124($sp)
	lw	$2,1128($sp)
#APP
 # 77 "/opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/string.h" 1
	.set	noreorder
	.set	noat
	lbu	$4,($3)
1:	lbu	$1,($2)
	addiu	$3,1
	bne	$1,$4,2f
	addiu	$2,1
	bnez	$4,1b
	lbu	$4,($3)
	move	$4,$1
2:	subu	$4,$1
3:	.set	at
	.set	reorder
 # 0 "" 2
#NO_APP
	sw	$3,1124($sp)
	sw	$2,1128($sp)
	sw	$4,1132($sp)
	lw	$2,1132($sp)
	bne	$2,$0,$L424
	lw	$2,1752($sp)
	beq	$2,$0,$L425
	lw	$3,1752($sp)
	li	$2,2			# 0x2
	bne	$3,$2,$L426
$L425:
	lw	$3,1752($sp)
	li	$2,-2147221504			# 0xffffffff80040000
	ori	$4,$2,0xda20
	move	$5,$3
	lui	$2,%hi(if_cmd_proc)
	addiu	$2,$2,%lo(if_cmd_proc)
	jalr	$2
	lui	$2,%hi($LC143)
	addiu	$4,$2,%lo($LC143)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	j	$L303
$L426:
	lw	$3,1752($sp)
	li	$2,-2147221504			# 0xffffffff80040000
	ori	$4,$2,0xda20
	move	$5,$3
	lui	$2,%hi(if_cmd_proc)
	addiu	$2,$2,%lo(if_cmd_proc)
	jalr	$2
	lui	$2,%hi($LC144)
	addiu	$4,$2,%lo($LC144)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	j	$L303
$L424:
	addiu	$2,$sp,1720
	sw	$2,1136($sp)
	lui	$2,%hi($LC145)
	addiu	$2,$2,%lo($LC145)
	sw	$2,1140($sp)
	lw	$3,1136($sp)
	lw	$2,1140($sp)
#APP
 # 77 "/opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/string.h" 1
	.set	noreorder
	.set	noat
	lbu	$4,($3)
1:	lbu	$1,($2)
	addiu	$3,1
	bne	$1,$4,2f
	addiu	$2,1
	bnez	$4,1b
	lbu	$4,($3)
	move	$4,$1
2:	subu	$4,$1
3:	.set	at
	.set	reorder
 # 0 "" 2
#NO_APP
	sw	$3,1136($sp)
	sw	$2,1140($sp)
	sw	$4,1144($sp)
	lw	$2,1144($sp)
	bne	$2,$0,$L429
	lui	$2,%hi(gponDevClearSwCounter)
	addiu	$2,$2,%lo(gponDevClearSwCounter)
	jalr	$2
	lui	$2,%hi($LC146)
	addiu	$4,$2,%lo($LC146)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	j	$L303
$L429:
	addiu	$2,$sp,1720
	sw	$2,1148($sp)
	lui	$2,%hi($LC147)
	addiu	$2,$2,%lo($LC147)
	sw	$2,1152($sp)
	lw	$3,1148($sp)
	lw	$2,1152($sp)
#APP
 # 77 "/opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/string.h" 1
	.set	noreorder
	.set	noat
	lbu	$4,($3)
1:	lbu	$1,($2)
	addiu	$3,1
	bne	$1,$4,2f
	addiu	$2,1
	bnez	$4,1b
	lbu	$4,($3)
	move	$4,$1
2:	subu	$4,$1
3:	.set	at
	.set	reorder
 # 0 "" 2
#NO_APP
	sw	$3,1148($sp)
	sw	$2,1152($sp)
	sw	$4,1156($sp)
	lw	$2,1156($sp)
	bne	$2,$0,$L431
	lui	$2,%hi(gponDevClearHwCounter)
	addiu	$2,$2,%lo(gponDevClearHwCounter)
	jalr	$2
	j	$L303
$L431:
	addiu	$2,$sp,1720
	sw	$2,1160($sp)
	lui	$2,%hi($LC148)
	addiu	$2,$2,%lo($LC148)
	sw	$2,1164($sp)
	lw	$3,1160($sp)
	lw	$2,1164($sp)
#APP
 # 77 "/opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/string.h" 1
	.set	noreorder
	.set	noat
	lbu	$4,($3)
1:	lbu	$1,($2)
	addiu	$3,1
	bne	$1,$4,2f
	addiu	$2,1
	bnez	$4,1b
	lbu	$4,($3)
	move	$4,$1
2:	subu	$4,$1
3:	.set	at
	.set	reorder
 # 0 "" 2
#NO_APP
	sw	$3,1160($sp)
	sw	$2,1164($sp)
	sw	$4,1168($sp)
	lw	$2,1168($sp)
	bne	$2,$0,$L433
	lw	$2,1752($sp)
	sw	$2,96($sp)
	lw	$2,96($sp)
	move	$4,$2
	lui	$2,%hi(reiSeq)
	addiu	$5,$2,%lo(reiSeq)
	lui	$2,%hi(ploam_send_rei_msg)
	addiu	$2,$2,%lo(ploam_send_rei_msg)
	jalr	$2
	lui	$2,%hi($LC149)
	addiu	$4,$2,%lo($LC149)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	j	$L303
$L433:
	addiu	$2,$sp,1720
	sw	$2,1172($sp)
	lui	$2,%hi($LC150)
	addiu	$2,$2,%lo($LC150)
	sw	$2,1176($sp)
	lw	$3,1172($sp)
	lw	$2,1176($sp)
#APP
 # 77 "/opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/string.h" 1
	.set	noreorder
	.set	noat
	lbu	$4,($3)
1:	lbu	$1,($2)
	addiu	$3,1
	bne	$1,$4,2f
	addiu	$2,1
	bnez	$4,1b
	lbu	$4,($3)
	move	$4,$1
2:	subu	$4,$1
3:	.set	at
	.set	reorder
 # 0 "" 2
#NO_APP
	sw	$3,1172($sp)
	sw	$2,1176($sp)
	sw	$4,1180($sp)
	lw	$2,1180($sp)
	bne	$2,$0,$L435
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lw	$3,1752($sp)
	andi	$3,$3,0xffff
	sh	$3,292($2)
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lhu	$2,292($2)
	move	$3,$2
	lui	$2,%hi($LC151)
	addiu	$4,$2,%lo($LC151)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	j	$L303
$L435:
	addiu	$2,$sp,1720
	sw	$2,1184($sp)
	lui	$2,%hi($LC152)
	addiu	$2,$2,%lo($LC152)
	sw	$2,1188($sp)
	lw	$3,1184($sp)
	lw	$2,1188($sp)
#APP
 # 77 "/opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/string.h" 1
	.set	noreorder
	.set	noat
	lbu	$4,($3)
1:	lbu	$1,($2)
	addiu	$3,1
	bne	$1,$4,2f
	addiu	$2,1
	bnez	$4,1b
	lbu	$4,($3)
	move	$4,$1
2:	subu	$4,$1
3:	.set	at
	.set	reorder
 # 0 "" 2
#NO_APP
	sw	$3,1184($sp)
	sw	$2,1188($sp)
	sw	$4,1192($sp)
	lw	$2,1192($sp)
	bne	$2,$0,$L437
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lw	$3,1752($sp)
	andi	$3,$3,0x00ff
	sb	$3,279($2)
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lbu	$2,279($2)
	move	$3,$2
	lui	$2,%hi($LC153)
	addiu	$4,$2,%lo($LC153)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	j	$L303
$L437:
	addiu	$2,$sp,1720
	sw	$2,1196($sp)
	lui	$2,%hi($LC154)
	addiu	$2,$2,%lo($LC154)
	sw	$2,1200($sp)
	lw	$3,1196($sp)
	lw	$2,1200($sp)
#APP
 # 77 "/opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/string.h" 1
	.set	noreorder
	.set	noat
	lbu	$4,($3)
1:	lbu	$1,($2)
	addiu	$3,1
	bne	$1,$4,2f
	addiu	$2,1
	bnez	$4,1b
	lbu	$4,($3)
	move	$4,$1
2:	subu	$4,$1
3:	.set	at
	.set	reorder
 # 0 "" 2
#NO_APP
	sw	$3,1196($sp)
	sw	$2,1200($sp)
	sw	$4,1204($sp)
	lw	$2,1204($sp)
	bne	$2,$0,$L439
	lui	$2,%hi(gpGponPriv)
	lw	$3,%lo(gpGponPriv)($2)
	lw	$2,1752($sp)
	andi	$2,$2,0x00ff
	andi	$2,$2,0x1
	andi	$4,$2,0x00ff
	lhu	$2,448($3)
	ins	$2,$4,11,1
	sh	$2,448($3)
	lw	$2,1752($sp)
	beq	$2,$0,$L440
	lui	$2,%hi($LC28)
	addiu	$2,$2,%lo($LC28)
	j	$L441
$L440:
	lui	$2,%hi($LC27)
	addiu	$2,$2,%lo($LC27)
$L441:
	lui	$3,%hi($LC155)
	addiu	$4,$3,%lo($LC155)
	move	$5,$2
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	j	$L303
$L439:
	addiu	$2,$sp,1720
	sw	$2,1208($sp)
	lui	$2,%hi($LC156)
	addiu	$2,$2,%lo($LC156)
	sw	$2,1212($sp)
	lw	$3,1208($sp)
	lw	$2,1212($sp)
#APP
 # 77 "/opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/string.h" 1
	.set	noreorder
	.set	noat
	lbu	$4,($3)
1:	lbu	$1,($2)
	addiu	$3,1
	bne	$1,$4,2f
	addiu	$2,1
	bnez	$4,1b
	lbu	$4,($3)
	move	$4,$1
2:	subu	$4,$1
3:	.set	at
	.set	reorder
 # 0 "" 2
#NO_APP
	sw	$3,1208($sp)
	sw	$2,1212($sp)
	sw	$4,1216($sp)
	lw	$2,1216($sp)
	bne	$2,$0,$L443
	sw	$0,40($sp)
	sw	$0,88($sp)
	sw	$0,92($sp)
	lui	$2,%hi(gpGponPriv)
	lw	$3,%lo(gpGponPriv)($2)
	lw	$2,1752($sp)
	andi	$2,$2,0x00ff
	andi	$2,$2,0x1
	andi	$4,$2,0x00ff
	lhu	$2,448($3)
	ins	$2,$4,10,1
	sh	$2,448($3)
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lw	$3,448($2)
	li	$2,67108864			# 0x4000000
	and	$2,$3,$2
	beq	$2,$0,$L444
	li	$2,1			# 0x1
	sw	$2,1220($sp)
	sw	$0,1224($sp)
	sw	$0,1228($sp)
	li	$2,14			# 0xe
	sw	$2,1860($sp)
	lw	$2,1220($sp)
	sw	$2,1864($sp)
	lw	$2,1224($sp)
	sw	$2,1912($sp)
	addiu	$2,$sp,1860
	li	$4,18			# 0x12
	move	$5,$0
	move	$6,$2
	lui	$2,%hi(__ECNT_HOOK)
	addiu	$2,$2,%lo(__ECNT_HOOK)
	jalr	$2
	sw	$2,1228($sp)
	li	$2,1			# 0x1
	sw	$2,1232($sp)
	sw	$0,1236($sp)
	sw	$0,1240($sp)
	li	$2,15			# 0xf
	sw	$2,1860($sp)
	lw	$2,1232($sp)
	sw	$2,1864($sp)
	lw	$2,1236($sp)
	sw	$2,1912($sp)
	addiu	$2,$sp,1860
	li	$4,18			# 0x12
	move	$5,$0
	move	$6,$2
	lui	$2,%hi(__ECNT_HOOK)
	addiu	$2,$2,%lo(__ECNT_HOOK)
	jalr	$2
	sw	$2,1240($sp)
	sw	$0,40($sp)
	j	$L451
$L454:
	lui	$2,%hi(gpWanPriv)
	lw	$3,%lo(gpWanPriv)($2)
	lw	$2,40($sp)
	addiu	$2,$2,44
	sll	$2,$2,1
	addu	$2,$3,$2
	lhu	$2,4($2)
	andi	$2,$2,0x7fff
	sw	$2,88($sp)
	lw	$2,88($sp)
	sltu	$2,$2,256
	beq	$2,$0,$L452
	lui	$2,%hi(gpWanPriv)
	lw	$3,%lo(gpWanPriv)($2)
	lw	$2,88($sp)
	sll	$2,$2,5
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	lw	$2,8284($2)
	ext	$2,$2,2,1
	andi	$2,$2,0x00ff
	bne	$2,$0,$L453
$L452:
	lw	$2,40($sp)
	andi	$2,$2,0xffff
	move	$4,$2
	li	$5,1			# 0x1
	move	$6,$0
	lui	$2,%hi(gponDevSetGemInfo)
	addiu	$2,$2,%lo(gponDevSetGemInfo)
	jalr	$2
$L453:
	lw	$2,40($sp)
	addiu	$2,$2,1
	sw	$2,40($sp)
$L451:
	lw	$2,40($sp)
	slt	$2,$2,4096
	bne	$2,$0,$L454
	j	$L455
$L444:
	li	$2,1			# 0x1
	sw	$2,1244($sp)
	li	$2,1			# 0x1
	sw	$2,1248($sp)
	sw	$0,1252($sp)
	li	$2,14			# 0xe
	sw	$2,1860($sp)
	lw	$2,1244($sp)
	sw	$2,1864($sp)
	lw	$2,1248($sp)
	sw	$2,1912($sp)
	addiu	$2,$sp,1860
	li	$4,18			# 0x12
	move	$5,$0
	move	$6,$2
	lui	$2,%hi(__ECNT_HOOK)
	addiu	$2,$2,%lo(__ECNT_HOOK)
	jalr	$2
	sw	$2,1252($sp)
	li	$2,1			# 0x1
	sw	$2,1256($sp)
	li	$2,1			# 0x1
	sw	$2,1260($sp)
	sw	$0,1264($sp)
	li	$2,15			# 0xf
	sw	$2,1860($sp)
	lw	$2,1256($sp)
	sw	$2,1864($sp)
	lw	$2,1260($sp)
	sw	$2,1912($sp)
	addiu	$2,$sp,1860
	li	$4,18			# 0x12
	move	$5,$0
	move	$6,$2
	lui	$2,%hi(__ECNT_HOOK)
	addiu	$2,$2,%lo(__ECNT_HOOK)
	jalr	$2
	sw	$2,1264($sp)
	sw	$0,40($sp)
	j	$L462
$L465:
	lui	$2,%hi(gpWanPriv)
	lw	$3,%lo(gpWanPriv)($2)
	lw	$2,40($sp)
	addiu	$2,$2,44
	sll	$2,$2,1
	addu	$2,$3,$2
	lhu	$2,4($2)
	andi	$2,$2,0x7fff
	sw	$2,88($sp)
	lw	$2,88($sp)
	sltu	$2,$2,256
	beq	$2,$0,$L463
	lui	$2,%hi(gpWanPriv)
	lw	$3,%lo(gpWanPriv)($2)
	lw	$2,88($sp)
	sll	$2,$2,5
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	lw	$2,8284($2)
	ext	$2,$2,2,1
	andi	$2,$2,0x00ff
	bne	$2,$0,$L464
$L463:
	lw	$2,40($sp)
	andi	$2,$2,0xffff
	move	$4,$2
	move	$5,$0
	move	$6,$0
	lui	$2,%hi(gponDevSetGemInfo)
	addiu	$2,$2,%lo(gponDevSetGemInfo)
	jalr	$2
$L464:
	lw	$2,40($sp)
	addiu	$2,$2,1
	sw	$2,40($sp)
$L462:
	lw	$2,40($sp)
	slt	$2,$2,4096
	bne	$2,$0,$L465
$L455:
	lw	$2,1752($sp)
	beq	$2,$0,$L466
	lui	$2,%hi($LC28)
	addiu	$2,$2,%lo($LC28)
	j	$L467
$L466:
	lui	$2,%hi($LC27)
	addiu	$2,$2,%lo($LC27)
$L467:
	lui	$3,%hi($LC157)
	addiu	$4,$3,%lo($LC157)
	move	$5,$2
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	j	$L303
$L443:
	addiu	$2,$sp,1720
	sw	$2,1268($sp)
	lui	$2,%hi($LC158)
	addiu	$2,$2,%lo($LC158)
	sw	$2,1272($sp)
	lw	$3,1268($sp)
	lw	$2,1272($sp)
#APP
 # 77 "/opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/string.h" 1
	.set	noreorder
	.set	noat
	lbu	$4,($3)
1:	lbu	$1,($2)
	addiu	$3,1
	bne	$1,$4,2f
	addiu	$2,1
	bnez	$4,1b
	lbu	$4,($3)
	move	$4,$1
2:	subu	$4,$1
3:	.set	at
	.set	reorder
 # 0 "" 2
#NO_APP
	sw	$3,1268($sp)
	sw	$2,1272($sp)
	sw	$4,1276($sp)
	lw	$2,1276($sp)
	bne	$2,$0,$L303
	lw	$2,1752($sp)
	beq	$2,$0,$L303
	j	$L469
$L470:
	lui	$2,%hi(xmit_dying_gasp_oam)
	addiu	$2,$2,%lo(xmit_dying_gasp_oam)
	jalr	$2
	lw	$2,1752($sp)
	addiu	$2,$2,-1
	sw	$2,1752($sp)
$L469:
	lw	$2,1752($sp)
	bne	$2,$0,$L470
	j	$L303
$L422:
	addiu	$2,$sp,1688
	sw	$2,1280($sp)
	lui	$2,%hi($LC159)
	addiu	$2,$2,%lo($LC159)
	sw	$2,1284($sp)
	lw	$3,1280($sp)
	lw	$2,1284($sp)
#APP
 # 77 "/opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/string.h" 1
	.set	noreorder
	.set	noat
	lbu	$4,($3)
1:	lbu	$1,($2)
	addiu	$3,1
	bne	$1,$4,2f
	addiu	$2,1
	bnez	$4,1b
	lbu	$4,($3)
	move	$4,$1
2:	subu	$4,$1
3:	.set	at
	.set	reorder
 # 0 "" 2
#NO_APP
	sw	$3,1280($sp)
	sw	$2,1284($sp)
	sw	$4,1288($sp)
	lw	$2,1288($sp)
	bne	$2,$0,$L472
	lw	$2,1752($sp)
	andi	$2,$2,0x00ff
	move	$3,$2
	li	$2,1			# 0x1
	sw	$2,1292($sp)
	sb	$3,1296($sp)
	li	$2,1			# 0x1
	sw	$2,1300($sp)
	sw	$0,1304($sp)
	li	$2,4			# 0x4
	sw	$2,1860($sp)
	lw	$2,1292($sp)
	sw	$2,1864($sp)
	lbu	$2,1296($sp)
	sw	$2,1888($sp)
	lw	$2,1300($sp)
	sw	$2,1912($sp)
	addiu	$2,$sp,1860
	li	$4,18			# 0x12
	move	$5,$0
	move	$6,$2
	lui	$2,%hi(__ECNT_HOOK)
	addiu	$2,$2,%lo(__ECNT_HOOK)
	jalr	$2
	sw	$2,1304($sp)
	lw	$3,1304($sp)
	li	$2,-1			# 0xffffffffffffffff
	beq	$3,$2,$L473
	lw	$2,1976($sp)
	j	$L475
$L473:
	li	$2,-1			# 0xffffffffffffffff
$L475:
	sw	$2,100($sp)
	lw	$2,100($sp)
	bne	$2,$0,$L476
	lw	$3,1752($sp)
	lui	$2,%hi($LC160)
	addiu	$4,$2,%lo($LC160)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	j	$L303
$L476:
	lw	$3,1752($sp)
	lui	$2,%hi($LC161)
	addiu	$4,$2,%lo($LC161)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	j	$L303
$L472:
	addiu	$2,$sp,1688
	sw	$2,1308($sp)
	lui	$2,%hi($LC162)
	addiu	$2,$2,%lo($LC162)
	sw	$2,1312($sp)
	lw	$3,1308($sp)
	lw	$2,1312($sp)
#APP
 # 77 "/opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/string.h" 1
	.set	noreorder
	.set	noat
	lbu	$4,($3)
1:	lbu	$1,($2)
	addiu	$3,1
	bne	$1,$4,2f
	addiu	$2,1
	bnez	$4,1b
	lbu	$4,($3)
	move	$4,$1
2:	subu	$4,$1
3:	.set	at
	.set	reorder
 # 0 "" 2
#NO_APP
	sw	$3,1308($sp)
	sw	$2,1312($sp)
	sw	$4,1316($sp)
	lw	$2,1316($sp)
	bne	$2,$0,$L479
	addiu	$2,$sp,1720
	sw	$2,1320($sp)
	lui	$2,%hi($LC163)
	addiu	$2,$2,%lo($LC163)
	sw	$2,1324($sp)
	lw	$3,1320($sp)
	lw	$2,1324($sp)
#APP
 # 77 "/opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/string.h" 1
	.set	noreorder
	.set	noat
	lbu	$4,($3)
1:	lbu	$1,($2)
	addiu	$3,1
	bne	$1,$4,2f
	addiu	$2,1
	bnez	$4,1b
	lbu	$4,($3)
	move	$4,$1
2:	subu	$4,$1
3:	.set	at
	.set	reorder
 # 0 "" 2
#NO_APP
	sw	$3,1320($sp)
	sw	$2,1324($sp)
	sw	$4,1328($sp)
	lw	$2,1328($sp)
	bne	$2,$0,$L481
	lui	$2,%hi(gpon_dvt_pcp_auto_testing)
	addiu	$2,$2,%lo(gpon_dvt_pcp_auto_testing)
	jalr	$2
	lui	$2,%hi($LC164)
	addiu	$4,$2,%lo($LC164)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	j	$L303
$L481:
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lw	$2,284($2)
	beq	$2,$0,$L483
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lw	$5,284($2)
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lw	$3,288($2)
	lui	$2,%hi($LC165)
	addiu	$4,$2,%lo($LC165)
	move	$6,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	sw	$0,284($2)
$L483:
	addiu	$2,$sp,1720
	sw	$2,1332($sp)
	lui	$2,%hi($LC166)
	addiu	$2,$2,%lo($LC166)
	sw	$2,1336($sp)
	lw	$3,1332($sp)
	lw	$2,1336($sp)
#APP
 # 77 "/opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/string.h" 1
	.set	noreorder
	.set	noat
	lbu	$4,($3)
1:	lbu	$1,($2)
	addiu	$3,1
	bne	$1,$4,2f
	addiu	$2,1
	bnez	$4,1b
	lbu	$4,($3)
	move	$4,$1
2:	subu	$4,$1
3:	.set	at
	.set	reorder
 # 0 "" 2
#NO_APP
	sw	$3,1332($sp)
	sw	$2,1336($sp)
	sw	$4,1340($sp)
	lw	$2,1340($sp)
	bne	$2,$0,$L485
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	sb	$0,280($2)
	move	$4,$0
	li	$2,-1078657024			# 0xffffffffbfb50000
	ori	$5,$2,0x1514
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	j	$L486
$L485:
	lw	$3,1752($sp)
	li	$2,1			# 0x1
	beq	$3,$2,$L487
	lw	$3,1752($sp)
	li	$2,2			# 0x2
	beq	$3,$2,$L487
	lw	$3,1752($sp)
	li	$2,4			# 0x4
	beq	$3,$2,$L487
	lw	$3,1752($sp)
	li	$2,8			# 0x8
	bne	$3,$2,$L486
$L487:
	addiu	$2,$sp,1720
	sw	$2,1344($sp)
	lui	$2,%hi($LC167)
	addiu	$2,$2,%lo($LC167)
	sw	$2,1348($sp)
	lw	$3,1344($sp)
	lw	$2,1348($sp)
#APP
 # 77 "/opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/string.h" 1
	.set	noreorder
	.set	noat
	lbu	$4,($3)
1:	lbu	$1,($2)
	addiu	$3,1
	bne	$1,$4,2f
	addiu	$2,1
	bnez	$4,1b
	lbu	$4,($3)
	move	$4,$1
2:	subu	$4,$1
3:	.set	at
	.set	reorder
 # 0 "" 2
#NO_APP
	sw	$3,1344($sp)
	sw	$2,1348($sp)
	sw	$4,1352($sp)
	lw	$2,1352($sp)
	bne	$2,$0,$L489
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lw	$3,1752($sp)
	andi	$3,$3,0x00ff
	ori	$3,$3,0x40
	andi	$3,$3,0x00ff
	sb	$3,280($2)
	lw	$2,1752($sp)
	sll	$2,$2,8
	move	$4,$2
	li	$2,-1078657024			# 0xffffffffbfb50000
	ori	$5,$2,0x1514
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	j	$L486
$L489:
	addiu	$2,$sp,1720
	sw	$2,1356($sp)
	lui	$2,%hi($LC168)
	addiu	$2,$2,%lo($LC168)
	sw	$2,1360($sp)
	lw	$3,1356($sp)
	lw	$2,1360($sp)
#APP
 # 77 "/opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/string.h" 1
	.set	noreorder
	.set	noat
	lbu	$4,($3)
1:	lbu	$1,($2)
	addiu	$3,1
	bne	$1,$4,2f
	addiu	$2,1
	bnez	$4,1b
	lbu	$4,($3)
	move	$4,$1
2:	subu	$4,$1
3:	.set	at
	.set	reorder
 # 0 "" 2
#NO_APP
	sw	$3,1356($sp)
	sw	$2,1360($sp)
	sw	$4,1364($sp)
	lw	$2,1364($sp)
	bne	$2,$0,$L491
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lw	$3,1752($sp)
	andi	$3,$3,0x00ff
	ori	$3,$3,0x20
	andi	$3,$3,0x00ff
	sb	$3,280($2)
	lw	$2,1752($sp)
	sll	$2,$2,4
	move	$4,$2
	li	$2,-1078657024			# 0xffffffffbfb50000
	ori	$5,$2,0x1514
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	j	$L486
$L491:
	addiu	$2,$sp,1720
	sw	$2,1368($sp)
	lui	$2,%hi($LC169)
	addiu	$2,$2,%lo($LC169)
	sw	$2,1372($sp)
	lw	$3,1368($sp)
	lw	$2,1372($sp)
#APP
 # 77 "/opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/string.h" 1
	.set	noreorder
	.set	noat
	lbu	$4,($3)
1:	lbu	$1,($2)
	addiu	$3,1
	bne	$1,$4,2f
	addiu	$2,1
	bnez	$4,1b
	lbu	$4,($3)
	move	$4,$1
2:	subu	$4,$1
3:	.set	at
	.set	reorder
 # 0 "" 2
#NO_APP
	sw	$3,1368($sp)
	sw	$2,1372($sp)
	sw	$4,1376($sp)
	lw	$2,1376($sp)
	bne	$2,$0,$L486
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lw	$3,1752($sp)
	andi	$3,$3,0x00ff
	ori	$3,$3,0x10
	andi	$3,$3,0x00ff
	sb	$3,280($2)
	lw	$2,1752($sp)
	move	$4,$2
	li	$2,-1078657024			# 0xffffffffbfb50000
	ori	$5,$2,0x1514
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
$L486:
	li	$2,-1078657024			# 0xffffffffbfb50000
	ori	$4,$2,0x1514
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	move	$3,$2
	lui	$2,%hi($LC170)
	addiu	$4,$2,%lo($LC170)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	j	$L303
$L479:
	addiu	$2,$sp,1688
	sw	$2,1380($sp)
	lui	$2,%hi($LC171)
	addiu	$2,$2,%lo($LC171)
	sw	$2,1384($sp)
	lw	$3,1380($sp)
	lw	$2,1384($sp)
#APP
 # 77 "/opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/string.h" 1
	.set	noreorder
	.set	noat
	lbu	$4,($3)
1:	lbu	$1,($2)
	addiu	$3,1
	bne	$1,$4,2f
	addiu	$2,1
	bnez	$4,1b
	lbu	$4,($3)
	move	$4,$1
2:	subu	$4,$1
3:	.set	at
	.set	reorder
 # 0 "" 2
#NO_APP
	sw	$3,1380($sp)
	sw	$2,1384($sp)
	sw	$4,1388($sp)
	lw	$2,1388($sp)
	beq	$2,$0,$L303
	addiu	$2,$sp,1688
	sw	$2,1392($sp)
	lui	$2,%hi($LC25)
	addiu	$2,$2,%lo($LC25)
	sw	$2,1396($sp)
	lw	$3,1392($sp)
	lw	$2,1396($sp)
#APP
 # 77 "/opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/string.h" 1
	.set	noreorder
	.set	noat
	lbu	$4,($3)
1:	lbu	$1,($2)
	addiu	$3,1
	bne	$1,$4,2f
	addiu	$2,1
	bnez	$4,1b
	lbu	$4,($3)
	move	$4,$1
2:	subu	$4,$1
3:	.set	at
	.set	reorder
 # 0 "" 2
#NO_APP
	sw	$3,1392($sp)
	sw	$2,1396($sp)
	sw	$4,1400($sp)
	lw	$2,1400($sp)
	bne	$2,$0,$L495
	addiu	$2,$sp,1720
	sw	$2,1404($sp)
	lui	$2,%hi($LC172)
	addiu	$2,$2,%lo($LC172)
	sw	$2,1408($sp)
	lw	$3,1404($sp)
	lw	$2,1408($sp)
#APP
 # 77 "/opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/string.h" 1
	.set	noreorder
	.set	noat
	lbu	$4,($3)
1:	lbu	$1,($2)
	addiu	$3,1
	bne	$1,$4,2f
	addiu	$2,1
	bnez	$4,1b
	lbu	$4,($3)
	move	$4,$1
2:	subu	$4,$1
3:	.set	at
	.set	reorder
 # 0 "" 2
#NO_APP
	sw	$3,1404($sp)
	sw	$2,1408($sp)
	sw	$4,1412($sp)
	lw	$2,1412($sp)
	bne	$2,$0,$L497
	lw	$3,1752($sp)
	lui	$2,%hi(g_EponLaserTime)
	sw	$3,%lo(g_EponLaserTime)($2)
	lui	$2,%hi(g_EponLaserTime)
	lw	$3,%lo(g_EponLaserTime)($2)
	lui	$2,%hi($LC173)
	addiu	$4,$2,%lo($LC173)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	j	$L498
$L497:
	addiu	$2,$sp,1720
	sw	$2,1416($sp)
	lui	$2,%hi($LC174)
	addiu	$2,$2,%lo($LC174)
	sw	$2,1420($sp)
	lw	$3,1416($sp)
	lw	$2,1420($sp)
#APP
 # 77 "/opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/string.h" 1
	.set	noreorder
	.set	noat
	lbu	$4,($3)
1:	lbu	$1,($2)
	addiu	$3,1
	bne	$1,$4,2f
	addiu	$2,1
	bnez	$4,1b
	lbu	$4,($3)
	move	$4,$1
2:	subu	$4,$1
3:	.set	at
	.set	reorder
 # 0 "" 2
#NO_APP
	sw	$3,1416($sp)
	sw	$2,1420($sp)
	sw	$4,1424($sp)
	lw	$2,1424($sp)
	bne	$2,$0,$L498
	lw	$3,1752($sp)
	lui	$2,%hi($LC175)
	addiu	$4,$2,%lo($LC175)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	lw	$2,1752($sp)
	move	$4,$2
	lui	$2,%hi(xmcs_set_epon_llid_config)
	addiu	$2,$2,%lo(xmcs_set_epon_llid_config)
	jalr	$2
$L498:
	addiu	$2,$sp,1720
	sw	$2,1428($sp)
	lui	$2,%hi($LC176)
	addiu	$2,$2,%lo($LC176)
	sw	$2,1432($sp)
	lw	$3,1428($sp)
	lw	$2,1432($sp)
#APP
 # 77 "/opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/string.h" 1
	.set	noreorder
	.set	noat
	lbu	$4,($3)
1:	lbu	$1,($2)
	addiu	$3,1
	bne	$1,$4,2f
	addiu	$2,1
	bnez	$4,1b
	lbu	$4,($3)
	move	$4,$1
2:	subu	$4,$1
3:	.set	at
	.set	reorder
 # 0 "" 2
#NO_APP
	sw	$3,1428($sp)
	sw	$2,1432($sp)
	sw	$4,1436($sp)
	lw	$2,1436($sp)
	bne	$2,$0,$L303
	addiu	$4,$sp,1624
	addiu	$6,$sp,1688
	addiu	$3,$sp,1720
	addiu	$2,$sp,1752
	sw	$2,16($sp)
	lui	$2,%hi($LC177)
	addiu	$5,$2,%lo($LC177)
	move	$7,$3
	lui	$2,%hi(sscanf)
	addiu	$2,$2,%lo(sscanf)
	jalr	$2
	lw	$2,1752($sp)
	andi	$3,$2,0x00ff
	lui	$2,%hi(g_silence_time)
	sb	$3,%lo(g_silence_time)($2)
	lui	$2,%hi(g_silence_time)
	lbu	$2,%lo(g_silence_time)($2)
	move	$3,$2
	lui	$2,%hi($LC178)
	addiu	$4,$2,%lo($LC178)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	j	$L303
$L495:
	addiu	$2,$sp,1688
	sw	$2,1440($sp)
	lui	$2,%hi($LC179)
	addiu	$2,$2,%lo($LC179)
	sw	$2,1444($sp)
	lw	$3,1440($sp)
	lw	$2,1444($sp)
#APP
 # 77 "/opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/string.h" 1
	.set	noreorder
	.set	noat
	lbu	$4,($3)
1:	lbu	$1,($2)
	addiu	$3,1
	bne	$1,$4,2f
	addiu	$2,1
	bnez	$4,1b
	lbu	$4,($3)
	move	$4,$1
2:	subu	$4,$1
3:	.set	at
	.set	reorder
 # 0 "" 2
#NO_APP
	sw	$3,1440($sp)
	sw	$2,1444($sp)
	sw	$4,1448($sp)
	lw	$2,1448($sp)
	bne	$2,$0,$L503
	sh	$0,104($sp)
	addiu	$2,$sp,1720
	sw	$2,1452($sp)
	lui	$2,%hi($LC180)
	addiu	$2,$2,%lo($LC180)
	sw	$2,1456($sp)
	lw	$3,1452($sp)
	lw	$2,1456($sp)
#APP
 # 77 "/opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/string.h" 1
	.set	noreorder
	.set	noat
	lbu	$4,($3)
1:	lbu	$1,($2)
	addiu	$3,1
	bne	$1,$4,2f
	addiu	$2,1
	bnez	$4,1b
	lbu	$4,($3)
	move	$4,$1
2:	subu	$4,$1
3:	.set	at
	.set	reorder
 # 0 "" 2
#NO_APP
	sw	$3,1452($sp)
	sw	$2,1456($sp)
	sw	$4,1460($sp)
	lw	$2,1460($sp)
	bne	$2,$0,$L505
	lw	$2,1752($sp)
	sh	$2,104($sp)
	lhu	$2,104($sp)
	move	$4,$2
	lui	$2,%hi(assignGemportId)
	addiu	$2,$2,%lo(assignGemportId)
	jalr	$2
	j	$L303
$L505:
	j	$L303
$L503:
	addiu	$2,$sp,1688
	sw	$2,1464($sp)
	lui	$2,%hi($LC181)
	addiu	$2,$2,%lo($LC181)
	sw	$2,1468($sp)
	lw	$3,1464($sp)
	lw	$2,1468($sp)
#APP
 # 77 "/opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/string.h" 1
	.set	noreorder
	.set	noat
	lbu	$4,($3)
1:	lbu	$1,($2)
	addiu	$3,1
	bne	$1,$4,2f
	addiu	$2,1
	bnez	$4,1b
	lbu	$4,($3)
	move	$4,$1
2:	subu	$4,$1
3:	.set	at
	.set	reorder
 # 0 "" 2
#NO_APP
	sw	$3,1464($sp)
	sw	$2,1468($sp)
	sw	$4,1472($sp)
	lw	$2,1472($sp)
	bne	$2,$0,$L507
	addiu	$2,$sp,1720
	sw	$2,1476($sp)
	lui	$2,%hi($LC100)
	addiu	$2,$2,%lo($LC100)
	sw	$2,1480($sp)
	lw	$3,1476($sp)
	lw	$2,1480($sp)
#APP
 # 77 "/opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/string.h" 1
	.set	noreorder
	.set	noat
	lbu	$4,($3)
1:	lbu	$1,($2)
	addiu	$3,1
	bne	$1,$4,2f
	addiu	$2,1
	bnez	$4,1b
	lbu	$4,($3)
	move	$4,$1
2:	subu	$4,$1
3:	.set	at
	.set	reorder
 # 0 "" 2
#NO_APP
	sw	$3,1476($sp)
	sw	$2,1480($sp)
	sw	$4,1484($sp)
	lw	$2,1484($sp)
	bne	$2,$0,$L509
	sw	$0,108($sp)
	li	$2,32786			# 0x8012
	sw	$2,1488($sp)
	lw	$2,1488($sp)
	sw	$2,1492($sp)
	sw	$0,1496($sp)
	sw	$0,1844($sp)
	sw	$0,1848($sp)
	sw	$0,1852($sp)
	sw	$0,1856($sp)
	sw	$0,1844($sp)
	lw	$2,1492($sp)
	sw	$2,1848($sp)
	lw	$2,1496($sp)
	sw	$2,1856($sp)
	addiu	$2,$sp,1844
	sw	$2,1500($sp)
	li	$4,25			# 0x19
	move	$5,$0
	lw	$6,1500($sp)
	lui	$2,%hi(__ECNT_HOOK)
	addiu	$2,$2,%lo(__ECNT_HOOK)
	jalr	$2
	move	$3,$2
	li	$2,-1			# 0xffffffffffffffff
	bne	$3,$2,$L510
	lw	$2,1500($sp)
	lw	$3,4($2)
	lui	$2,%hi($LC182)
	addiu	$4,$2,%lo($LC182)
	move	$5,$3
	lui	$2,%hi(panic)
	addiu	$2,$2,%lo(panic)
	jalr	$2
$L510:
	lw	$2,1500($sp)
	lw	$3,8($2)
	li	$2,-1			# 0xffffffffffffffff
	bne	$3,$2,$L511
	lw	$2,1500($sp)
	lw	$5,0($2)
	lw	$2,1500($sp)
	lw	$3,4($2)
	lui	$2,%hi($LC183)
	addiu	$4,$2,%lo($LC183)
	move	$6,$3
	lui	$2,%hi(panic)
	addiu	$2,$2,%lo(panic)
	jalr	$2
$L511:
	lw	$2,1500($sp)
	lw	$2,8($2)
	sw	$2,108($sp)
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lui	$3,%hi(gpGponPriv)
	lw	$3,%lo(gpGponPriv)($3)
	lw	$4,0($3)
	lw	$3,108($sp)
	addu	$3,$4,$3
	sw	$3,0($2)
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lw	$3,0($2)
	lui	$2,%hi($LC184)
	addiu	$4,$2,%lo($LC184)
	lw	$5,108($sp)
	move	$6,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	j	$L303
$L509:
	addiu	$2,$sp,1720
	sw	$2,1504($sp)
	lui	$2,%hi($LC145)
	addiu	$2,$2,%lo($LC145)
	sw	$2,1508($sp)
	lw	$3,1504($sp)
	lw	$2,1508($sp)
#APP
 # 77 "/opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/string.h" 1
	.set	noreorder
	.set	noat
	lbu	$4,($3)
1:	lbu	$1,($2)
	addiu	$3,1
	bne	$1,$4,2f
	addiu	$2,1
	bnez	$4,1b
	lbu	$4,($3)
	move	$4,$1
2:	subu	$4,$1
3:	.set	at
	.set	reorder
 # 0 "" 2
#NO_APP
	sw	$3,1504($sp)
	sw	$2,1508($sp)
	sw	$4,1512($sp)
	lw	$2,1512($sp)
	bne	$2,$0,$L303
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	sw	$0,0($2)
	j	$L303
$L507:
	addiu	$2,$sp,1688
	sw	$2,1516($sp)
	lui	$2,%hi($LC185)
	addiu	$2,$2,%lo($LC185)
	sw	$2,1520($sp)
	lw	$3,1516($sp)
	lw	$2,1520($sp)
#APP
 # 77 "/opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/string.h" 1
	.set	noreorder
	.set	noat
	lbu	$4,($3)
1:	lbu	$1,($2)
	addiu	$3,1
	bne	$1,$4,2f
	addiu	$2,1
	bnez	$4,1b
	lbu	$4,($3)
	move	$4,$1
2:	subu	$4,$1
3:	.set	at
	.set	reorder
 # 0 "" 2
#NO_APP
	sw	$3,1516($sp)
	sw	$2,1520($sp)
	sw	$4,1524($sp)
	lw	$2,1524($sp)
	bne	$2,$0,$L518
	addiu	$2,$sp,1720
	sw	$2,1528($sp)
	lui	$2,%hi($LC100)
	addiu	$2,$2,%lo($LC100)
	sw	$2,1532($sp)
	lw	$3,1528($sp)
	lw	$2,1532($sp)
#APP
 # 77 "/opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/string.h" 1
	.set	noreorder
	.set	noat
	lbu	$4,($3)
1:	lbu	$1,($2)
	addiu	$3,1
	bne	$1,$4,2f
	addiu	$2,1
	bnez	$4,1b
	lbu	$4,($3)
	move	$4,$1
2:	subu	$4,$1
3:	.set	at
	.set	reorder
 # 0 "" 2
#NO_APP
	sw	$3,1528($sp)
	sw	$2,1532($sp)
	sw	$4,1536($sp)
	lw	$2,1536($sp)
	bne	$2,$0,$L303
	lui	$2,%hi($LC186)
	addiu	$4,$2,%lo($LC186)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	lui	$2,%hi(gpon_show_emergence_info)
	addiu	$2,$2,%lo(gpon_show_emergence_info)
	jalr	$2
	j	$L303
$L518:
	addiu	$2,$sp,1688
	sw	$2,1540($sp)
	lui	$2,%hi($LC187)
	addiu	$2,$2,%lo($LC187)
	sw	$2,1544($sp)
	lw	$3,1540($sp)
	lw	$2,1544($sp)
#APP
 # 77 "/opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/string.h" 1
	.set	noreorder
	.set	noat
	lbu	$4,($3)
1:	lbu	$1,($2)
	addiu	$3,1
	bne	$1,$4,2f
	addiu	$2,1
	bnez	$4,1b
	lbu	$4,($3)
	move	$4,$1
2:	subu	$4,$1
3:	.set	at
	.set	reorder
 # 0 "" 2
#NO_APP
	sw	$3,1540($sp)
	sw	$2,1544($sp)
	sw	$4,1548($sp)
	lw	$2,1548($sp)
	bne	$2,$0,$L522
	addiu	$2,$sp,1720
	sw	$2,1552($sp)
	lui	$2,%hi($LC166)
	addiu	$2,$2,%lo($LC166)
	sw	$2,1556($sp)
	lw	$3,1552($sp)
	lw	$2,1556($sp)
#APP
 # 77 "/opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/string.h" 1
	.set	noreorder
	.set	noat
	lbu	$4,($3)
1:	lbu	$1,($2)
	addiu	$3,1
	bne	$1,$4,2f
	addiu	$2,1
	bnez	$4,1b
	lbu	$4,($3)
	move	$4,$1
2:	subu	$4,$1
3:	.set	at
	.set	reorder
 # 0 "" 2
#NO_APP
	sw	$3,1552($sp)
	sw	$2,1556($sp)
	sw	$4,1560($sp)
	lw	$2,1560($sp)
	bne	$2,$0,$L524
	lui	$2,%hi($LC188)
	addiu	$4,$2,%lo($LC188)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	lui	$2,%hi(green_drop_flag)
	sb	$0,%lo(green_drop_flag)($2)
	move	$4,$0
	lui	$2,%hi(xpon_set_qdma_qos)
	addiu	$2,$2,%lo(xpon_set_qdma_qos)
	jalr	$2
	j	$L303
$L524:
	addiu	$2,$sp,1720
	sw	$2,1564($sp)
	lui	$2,%hi($LC189)
	addiu	$2,$2,%lo($LC189)
	sw	$2,1568($sp)
	lw	$3,1564($sp)
	lw	$2,1568($sp)
#APP
 # 77 "/opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/string.h" 1
	.set	noreorder
	.set	noat
	lbu	$4,($3)
1:	lbu	$1,($2)
	addiu	$3,1
	bne	$1,$4,2f
	addiu	$2,1
	bnez	$4,1b
	lbu	$4,($3)
	move	$4,$1
2:	subu	$4,$1
3:	.set	at
	.set	reorder
 # 0 "" 2
#NO_APP
	sw	$3,1564($sp)
	sw	$2,1568($sp)
	sw	$4,1572($sp)
	lw	$2,1572($sp)
	bne	$2,$0,$L303
	lui	$2,%hi($LC190)
	addiu	$4,$2,%lo($LC190)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	lui	$2,%hi(green_drop_flag)
	li	$3,1			# 0x1
	sb	$3,%lo(green_drop_flag)($2)
	li	$4,1			# 0x1
	lui	$2,%hi(xpon_set_qdma_qos)
	addiu	$2,$2,%lo(xpon_set_qdma_qos)
	jalr	$2
	j	$L303
$L522:
	addiu	$2,$sp,1688
	sw	$2,1576($sp)
	lui	$2,%hi($LC191)
	addiu	$2,$2,%lo($LC191)
	sw	$2,1580($sp)
	lw	$3,1576($sp)
	lw	$2,1580($sp)
#APP
 # 77 "/opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/string.h" 1
	.set	noreorder
	.set	noat
	lbu	$4,($3)
1:	lbu	$1,($2)
	addiu	$3,1
	bne	$1,$4,2f
	addiu	$2,1
	bnez	$4,1b
	lbu	$4,($3)
	move	$4,$1
2:	subu	$4,$1
3:	.set	at
	.set	reorder
 # 0 "" 2
#NO_APP
	sw	$3,1576($sp)
	sw	$2,1580($sp)
	sw	$4,1584($sp)
	lw	$2,1584($sp)
	bne	$2,$0,$L303
	addiu	$2,$sp,1720
	sw	$2,1588($sp)
	lui	$2,%hi($LC192)
	addiu	$2,$2,%lo($LC192)
	sw	$2,1592($sp)
	lw	$3,1588($sp)
	lw	$2,1592($sp)
#APP
 # 77 "/opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/string.h" 1
	.set	noreorder
	.set	noat
	lbu	$4,($3)
1:	lbu	$1,($2)
	addiu	$3,1
	bne	$1,$4,2f
	addiu	$2,1
	bnez	$4,1b
	lbu	$4,($3)
	move	$4,$1
2:	subu	$4,$1
3:	.set	at
	.set	reorder
 # 0 "" 2
#NO_APP
	sw	$3,1588($sp)
	sw	$2,1592($sp)
	sw	$4,1596($sp)
	lw	$2,1596($sp)
	bne	$2,$0,$L529
	lw	$2,1752($sp)
	andi	$3,$2,0x00ff
	lui	$2,%hi(eponDrv)
	addiu	$2,$2,%lo(eponDrv)
	sb	$3,4($2)
	lui	$2,%hi(eponDrv)
	addiu	$2,$2,%lo(eponDrv)
	lbu	$2,4($2)
	move	$3,$2
	lui	$2,%hi($LC193)
	addiu	$4,$2,%lo($LC193)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	j	$L303
$L529:
	addiu	$2,$sp,1720
	sw	$2,1600($sp)
	lui	$2,%hi($LC194)
	addiu	$2,$2,%lo($LC194)
	sw	$2,1604($sp)
	lw	$3,1600($sp)
	lw	$2,1604($sp)
#APP
 # 77 "/opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/string.h" 1
	.set	noreorder
	.set	noat
	lbu	$4,($3)
1:	lbu	$1,($2)
	addiu	$3,1
	bne	$1,$4,2f
	addiu	$2,1
	bnez	$4,1b
	lbu	$4,($3)
	move	$4,$1
2:	subu	$4,$1
3:	.set	at
	.set	reorder
 # 0 "" 2
#NO_APP
	sw	$3,1600($sp)
	sw	$2,1604($sp)
	sw	$4,1608($sp)
	lw	$2,1608($sp)
	bne	$2,$0,$L531
	lw	$2,1752($sp)
	andi	$3,$2,0x00ff
	lui	$2,%hi(eponDrv)
	addiu	$2,$2,%lo(eponDrv)
	sb	$3,8($2)
	lui	$2,%hi(eponDrv)
	addiu	$2,$2,%lo(eponDrv)
	lbu	$2,8($2)
	move	$3,$2
	lui	$2,%hi($LC195)
	addiu	$4,$2,%lo($LC195)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	j	$L303
$L531:
	addiu	$2,$sp,1720
	sw	$2,1612($sp)
	lui	$2,%hi($LC196)
	addiu	$2,$2,%lo($LC196)
	sw	$2,1616($sp)
	lw	$3,1612($sp)
	lw	$2,1616($sp)
#APP
 # 77 "/opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/string.h" 1
	.set	noreorder
	.set	noat
	lbu	$4,($3)
1:	lbu	$1,($2)
	addiu	$3,1
	bne	$1,$4,2f
	addiu	$2,1
	bnez	$4,1b
	lbu	$4,($3)
	move	$4,$1
2:	subu	$4,$1
3:	.set	at
	.set	reorder
 # 0 "" 2
#NO_APP
	sw	$3,1612($sp)
	sw	$2,1616($sp)
	sw	$4,1620($sp)
	lw	$2,1620($sp)
	bne	$2,$0,$L303
	lw	$2,1752($sp)
	andi	$3,$2,0xffff
	lui	$2,%hi(eponDrv)
	addiu	$2,$2,%lo(eponDrv)
	sh	$3,6($2)
	lui	$2,%hi(eponDrv)
	addiu	$2,$2,%lo(eponDrv)
	lhu	$2,6($2)
	move	$3,$2
	lui	$2,%hi($LC197)
	addiu	$4,$2,%lo($LC197)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
$L303:
	lw	$2,7040($sp)
$L533:
	lw	$31,7028($sp)
	addiu	$sp,$sp,7032
	j	$31
	.end	gpon_debug_write_proc
	.size	gpon_debug_write_proc, .-gpon_debug_write_proc
	.rdata
	.align	2
$LC198:
	.ascii	"\012FAILED(%s): qdma setting for txBufUsage\012\000"
	.section	.text.qdma_reg_init,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	qdma_reg_init
	.type	qdma_reg_init, @function
qdma_reg_init:
	.frame	$sp,296,$31		# vars= 272, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-296
	sw	$31,292($sp)
	sw	$0,80($sp)
	li	$2,16			# 0x10
	sw	$2,20($sp)
	addiu	$2,$sp,80
	sw	$2,24($sp)
	sw	$0,28($sp)
	li	$2,57			# 0x39
	sw	$2,224($sp)
	lw	$2,24($sp)
	sw	$2,232($sp)
	addiu	$2,$sp,224
	lw	$4,20($sp)
	move	$5,$0
	move	$6,$2
	lui	$2,%hi(__ECNT_HOOK)
	addiu	$2,$2,%lo(__ECNT_HOOK)
	jalr	$2
	nop

	sw	$2,28($sp)
	li	$2,16			# 0x10
	sw	$2,32($sp)
	li	$2,1			# 0x1
	sw	$2,36($sp)
	sw	$0,40($sp)
	li	$2,51			# 0x33
	sw	$2,196($sp)
	lw	$2,36($sp)
	sw	$2,204($sp)
	addiu	$2,$sp,196
	lw	$4,32($sp)
	move	$5,$0
	move	$6,$2
	lui	$2,%hi(__ECNT_HOOK)
	addiu	$2,$2,%lo(__ECNT_HOOK)
	jalr	$2
	nop

	sw	$2,40($sp)
	addiu	$2,$sp,92
	move	$4,$2
	move	$5,$0
	li	$6,12			# 0xc
	lui	$2,%hi(memset)
	addiu	$2,$2,%lo(memset)
	jalr	$2
	nop

	li	$2,16			# 0x10
	sw	$2,44($sp)
	addiu	$2,$sp,92
	sw	$2,48($sp)
	sw	$0,52($sp)
	li	$2,60			# 0x3c
	sw	$2,168($sp)
	lw	$2,48($sp)
	sw	$2,176($sp)
	addiu	$2,$sp,168
	lw	$4,44($sp)
	move	$5,$0
	move	$6,$2
	lui	$2,%hi(__ECNT_HOOK)
	addiu	$2,$2,%lo(__ECNT_HOOK)
	jalr	$2
	nop

	sw	$2,52($sp)
	li	$2,160			# 0xa0
	sh	$2,98($sp)
	li	$2,16			# 0x10
	sw	$2,56($sp)
	addiu	$2,$sp,92
	sw	$2,60($sp)
	sw	$0,64($sp)
	li	$2,59			# 0x3b
	sw	$2,140($sp)
	lw	$2,60($sp)
	sw	$2,148($sp)
	addiu	$2,$sp,140
	lw	$4,56($sp)
	move	$5,$0
	move	$6,$2
	lui	$2,%hi(__ECNT_HOOK)
	addiu	$2,$2,%lo(__ECNT_HOOK)
	jalr	$2
	nop

	sw	$2,64($sp)
	addiu	$2,$sp,104
	move	$4,$2
	move	$5,$0
	li	$6,8			# 0x8
	lui	$2,%hi(memset)
	addiu	$2,$2,%lo(memset)
	jalr	$2
	nop

	li	$2,1			# 0x1
	sw	$2,104($sp)
	li	$2,32			# 0x20
	sb	$2,108($sp)
	li	$2,32			# 0x20
	sb	$2,109($sp)
	li	$2,16			# 0x10
	sw	$2,68($sp)
	addiu	$2,$sp,104
	sw	$2,72($sp)
	sw	$0,76($sp)
	li	$2,17			# 0x11
	sw	$2,112($sp)
	lw	$2,72($sp)
	sw	$2,120($sp)
	addiu	$2,$sp,112
	lw	$4,68($sp)
	move	$5,$0
	move	$6,$2
	lui	$2,%hi(__ECNT_HOOK)
	addiu	$2,$2,%lo(__ECNT_HOOK)
	jalr	$2
	nop

	sw	$2,76($sp)
	lw	$3,76($sp)
	li	$2,-1			# 0xffffffffffffffff
	beq	$3,$2,$L548
	nop

	lw	$2,116($sp)
	j	$L550
	nop

$L548:
	li	$2,-1			# 0xffffffffffffffff
$L550:
	sw	$2,16($sp)
	lw	$2,16($sp)
	beq	$2,$0,$L551
	nop

	lui	$2,%hi($LC198)
	addiu	$4,$2,%lo($LC198)
	lui	$2,%hi(__func__.33911)
	addiu	$5,$2,%lo(__func__.33911)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	li	$2,-1			# 0xffffffffffffffff
	j	$L553
	nop

$L551:
	move	$2,$0
$L553:
	lw	$31,292($sp)
	addiu	$sp,$sp,296
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	qdma_reg_init
	.size	qdma_reg_init, .-qdma_reg_init
	.rdata
	.align	2
$LC199:
	.ascii	"[%lu0ms]Prepare %d receive packet buffers in QDMA init s"
	.ascii	"tage.\012\000"
	.section	.text.xpondrv_qdma_init,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	xpondrv_qdma_init
	.type	xpondrv_qdma_init, @function
xpondrv_qdma_init:
	.frame	$sp,264,$31		# vars= 240, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-264
	sw	$31,260($sp)
	sw	$0,16($sp)
	lui	$2,%hi(qdma_reg_init)
	addiu	$2,$2,%lo(qdma_reg_init)
	jalr	$2
	nop

	addiu	$2,$sp,68
	move	$4,$2
	move	$5,$0
	li	$6,32			# 0x20
	lui	$2,%hi(memset)
	addiu	$2,$2,%lo(memset)
	jalr	$2
	nop

	lui	$2,%hi(pwan_cb_rx_packet)
	addiu	$2,$2,%lo(pwan_cb_rx_packet)
	sw	$2,76($sp)
	lui	$2,%hi(pwan_cb_event_handler)
	addiu	$2,$2,%lo(pwan_cb_event_handler)
	sw	$2,80($sp)
	li	$2,16			# 0x10
	sw	$2,20($sp)
	addiu	$2,$sp,68
	sw	$2,24($sp)
	sw	$0,28($sp)
	sw	$0,192($sp)
	lw	$2,24($sp)
	sw	$2,200($sp)
	addiu	$2,$sp,192
	lw	$4,20($sp)
	move	$5,$0
	move	$6,$2
	lui	$2,%hi(__ECNT_HOOK)
	addiu	$2,$2,%lo(__ECNT_HOOK)
	jalr	$2
	nop

	sw	$2,28($sp)
	li	$2,16			# 0x10
	sw	$2,32($sp)
	li	$2,1			# 0x1
	sw	$2,36($sp)
	sw	$0,40($sp)
	li	$2,2			# 0x2
	sw	$2,164($sp)
	lw	$2,36($sp)
	sw	$2,172($sp)
	addiu	$2,$sp,164
	lw	$4,32($sp)
	move	$5,$0
	move	$6,$2
	lui	$2,%hi(__ECNT_HOOK)
	addiu	$2,$2,%lo(__ECNT_HOOK)
	jalr	$2
	nop

	sw	$2,40($sp)
	li	$2,16			# 0x10
	sw	$2,44($sp)
	li	$2,1			# 0x1
	sw	$2,48($sp)
	sw	$0,52($sp)
	li	$2,3			# 0x3
	sw	$2,136($sp)
	lw	$2,48($sp)
	sw	$2,144($sp)
	addiu	$2,$sp,136
	lw	$4,44($sp)
	move	$5,$0
	move	$6,$2
	lui	$2,%hi(__ECNT_HOOK)
	addiu	$2,$2,%lo(__ECNT_HOOK)
	jalr	$2
	nop

	sw	$2,52($sp)
	li	$2,16			# 0x10
	sw	$2,56($sp)
	sw	$0,60($sp)
	sw	$0,64($sp)
	li	$2,4			# 0x4
	sw	$2,108($sp)
	lw	$2,60($sp)
	sw	$2,116($sp)
	addiu	$2,$sp,108
	lw	$4,56($sp)
	move	$5,$0
	move	$6,$2
	lui	$2,%hi(__ECNT_HOOK)
	addiu	$2,$2,%lo(__ECNT_HOOK)
	jalr	$2
	nop

	sw	$2,64($sp)
	move	$4,$0
	lui	$2,%hi(xpon_set_qdma_qos)
	addiu	$2,$2,%lo(xpon_set_qdma_qos)
	jalr	$2
	nop

	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	lhu	$2,-30816($2)
	andi	$2,$2,0x40
	beq	$2,$0,$L567
	nop

	lui	$2,%hi(jiffies)
	lw	$3,%lo(jiffies)($2)
	lui	$2,%hi($LC199)
	addiu	$4,$2,%lo($LC199)
	move	$5,$3
	lw	$6,16($sp)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L567:
	move	$2,$0
	lw	$31,260($sp)
	addiu	$sp,$sp,264
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	xpondrv_qdma_init
	.size	xpondrv_qdma_init, .-xpondrv_qdma_init
	.section	.text.xpon_dying_gasp_interrupt,"ax",@progbits
	.align	2
	.globl	xpon_dying_gasp_interrupt
	.set	nomips16
	.set	nomicromips
	.ent	xpon_dying_gasp_interrupt
	.type	xpon_dying_gasp_interrupt, @function
xpon_dying_gasp_interrupt:
	.frame	$sp,48,$31		# vars= 24, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-48
	sw	$31,44($sp)
	sw	$4,48($sp)
	sw	$5,52($sp)
	lui	$2,%hi(gpPonSysData)
	lw	$2,%lo(gpPonSysData)($2)
	lw	$3,0($2)
	li	$2,1			# 0x1
	bne	$3,$2,$L570
	nop

	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	lw	$3,-30828($2)
	li	$2,-2147483648			# 0xffffffff80000000
	and	$2,$3,$2
	bne	$2,$0,$L572
	nop

	li	$2,-1078984704			# 0xffffffffbfb00000
	ori	$2,$2,0x84
	sw	$2,16($sp)
	li	$2,65536			# 0x10000
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

	lui	$2,%hi(gpon_detect_dying_gasp)
	addiu	$2,$2,%lo(gpon_detect_dying_gasp)
	jalr	$2
	nop

	j	$L572
	nop

$L570:
	lui	$2,%hi(gpPonSysData)
	lw	$2,%lo(gpPonSysData)($2)
	lw	$3,0($2)
	li	$2,2			# 0x2
	bne	$3,$2,$L572
	nop

	li	$2,-1078984704			# 0xffffffffbfb00000
	ori	$2,$2,0x84
	sw	$2,28($sp)
	li	$2,65536			# 0x10000
	sw	$2,32($sp)
	lw	$2,28($sp)
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	sw	$2,36($sp)
	lw	$3,36($sp)
	lw	$2,32($sp)
	or	$2,$3,$2
	sw	$2,36($sp)
	lw	$2,28($sp)
	lw	$4,36($sp)
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	lw	$3,-30828($2)
	li	$2,1073741824			# 0x40000000
	and	$2,$3,$2
	bne	$2,$0,$L572
	nop

	lui	$2,%hi(xmit_dying_gasp_oam)
	addiu	$2,$2,%lo(xmit_dying_gasp_oam)
	jalr	$2
	nop

$L572:
	li	$2,1			# 0x1
	lw	$31,44($sp)
	addiu	$sp,$sp,48
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	xpon_dying_gasp_interrupt
	.size	xpon_dying_gasp_interrupt, .-xpon_dying_gasp_interrupt
	.rdata
	.align	2
$LC200:
	.ascii	"dying gasp\000"
	.align	2
$LC201:
	.ascii	"Request the interrupt service routine fail, irq:%d.\012\000"
	.section	.text.xpon_dying_gasp_init,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	xpon_dying_gasp_init
	.type	xpon_dying_gasp_init, @function
xpon_dying_gasp_init:
	.frame	$sp,56,$31		# vars= 24, regs= 1/0, args= 24, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-56
	sw	$31,52($sp)
	li	$2,19			# 0x13
	sw	$2,24($sp)
	lui	$2,%hi(xpon_dying_gasp_interrupt)
	addiu	$2,$2,%lo(xpon_dying_gasp_interrupt)
	sw	$2,28($sp)
	sw	$0,32($sp)
	lui	$2,%hi($LC200)
	addiu	$2,$2,%lo($LC200)
	sw	$2,36($sp)
	sw	$0,40($sp)
	lw	$2,36($sp)
	sw	$2,16($sp)
	lw	$2,40($sp)
	sw	$2,20($sp)
	lw	$4,24($sp)
	lw	$5,28($sp)
	move	$6,$0
	lw	$7,32($sp)
	lui	$2,%hi(request_threaded_irq)
	addiu	$2,$2,%lo(request_threaded_irq)
	jalr	$2
	nop

	beq	$2,$0,$L576
	nop

	lui	$2,%hi($LC201)
	addiu	$4,$2,%lo($LC201)
	li	$5,19			# 0x13
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	li	$2,-19			# 0xffffffffffffffed
	j	$L577
	nop

$L576:
	move	$2,$0
$L577:
	lw	$31,52($sp)
	addiu	$sp,$sp,56
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	xpon_dying_gasp_init
	.size	xpon_dying_gasp_init, .-xpon_dying_gasp_init
	.section	.text.schedule_fe_reset,"ax",@progbits
	.align	2
	.globl	schedule_fe_reset
	.set	nomips16
	.set	nomicromips
	.ent	schedule_fe_reset
	.type	schedule_fe_reset, @function
schedule_fe_reset:
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	schedule_fe_reset
	.size	schedule_fe_reset, .-schedule_fe_reset
	.rdata
	.align	2
$LC202:
	.ascii	"to1_timeout_cnt:%d rst:%d\012\000"
	.section	.text.gpon_stage_change_chk_read_proc,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	gpon_stage_change_chk_read_proc
	.type	gpon_stage_change_chk_read_proc, @function
gpon_stage_change_chk_read_proc:
	.frame	$sp,40,$31		# vars= 16, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-40
	sw	$31,36($sp)
	sw	$4,40($sp)
	sw	$5,44($sp)
	sw	$6,48($sp)
	sw	$7,52($sp)
	sw	$0,16($sp)
	sw	$0,24($sp)
	sw	$0,20($sp)
	lw	$2,16($sp)
	lw	$3,40($sp)
	addu	$4,$3,$2
	lui	$2,%hi(to1_timeout_cnt)
	addiu	$2,$2,%lo(to1_timeout_cnt)
	lw	$6,0($2)
	lui	$2,%hi(to1_rst_cnt)
	addiu	$2,$2,%lo(to1_rst_cnt)
	lw	$3,0($2)
	lui	$2,%hi($LC202)
	addiu	$5,$2,%lo($LC202)
	move	$7,$3
	lui	$2,%hi(sprintf)
	addiu	$2,$2,%lo(sprintf)
	jalr	$2
	nop

	move	$3,$2
	lw	$2,16($sp)
	addu	$2,$2,$3
	sw	$2,16($sp)
	lw	$3,20($sp)
	lw	$2,16($sp)
	addu	$2,$3,$2
	sw	$2,24($sp)
	lw	$3,24($sp)
	lw	$2,48($sp)
	slt	$2,$3,$2
	beq	$2,$0,$L580
	nop

	sw	$0,16($sp)
	lw	$2,24($sp)
	sw	$2,20($sp)
$L580:
	lw	$3,48($sp)
	lw	$2,52($sp)
	addu	$3,$3,$2
	lw	$2,24($sp)
	slt	$2,$3,$2
	beq	$2,$0,$L581
	nop

	j	$L582
	nop

$L581:
	lw	$2,56($sp)
	li	$3,1			# 0x1
	sw	$3,0($2)
$L582:
	lw	$3,48($sp)
	lw	$2,20($sp)
	subu	$2,$3,$2
	move	$3,$2
	lw	$2,40($sp)
	addu	$3,$2,$3
	lw	$2,44($sp)
	sw	$3,0($2)
	lw	$3,20($sp)
	lw	$2,48($sp)
	subu	$2,$3,$2
	lw	$3,16($sp)
	addu	$2,$3,$2
	sw	$2,16($sp)
	lw	$2,16($sp)
	bgez	$2,$L583
	nop

	sw	$0,16($sp)
$L583:
	lw	$3,16($sp)
	lw	$2,52($sp)
	slt	$2,$2,$3
	beq	$2,$0,$L584
	nop

	lw	$2,52($sp)
	sw	$2,16($sp)
$L584:
	lw	$2,16($sp)
	lw	$31,36($sp)
	addiu	$sp,$sp,40
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	gpon_stage_change_chk_read_proc
	.size	gpon_stage_change_chk_read_proc, .-gpon_stage_change_chk_read_proc
	.rdata
	.align	2
$LC203:
	.ascii	"%d %d\000"
	.section	.text.gpon_stage_change_chk_write_proc,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	gpon_stage_change_chk_write_proc
	.type	gpon_stage_change_chk_write_proc, @function
gpon_stage_change_chk_write_proc:
	.frame	$sp,128,$31		# vars= 104, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	addiu	$sp,$sp,-128
	sw	$31,124($sp)
	sw	$4,128($sp)
	sw	$5,132($sp)
	sw	$6,136($sp)
	sw	$7,140($sp)
	addiu	$3,$sp,44
	li	$2,64			# 0x40
	move	$4,$3
	move	$5,$0
	move	$6,$2
	lui	$2,%hi(memset)
	addiu	$2,$2,%lo(memset)
	jalr	$2
	sw	$0,108($sp)
	sw	$0,112($sp)
	lw	$2,136($sp)
	sltu	$2,$2,64
	bne	$2,$0,$L587
	li	$2,-22			# 0xffffffffffffffea
	j	$L591
$L587:
	addiu	$2,$sp,44
	sw	$2,20($sp)
	lw	$2,132($sp)
	sw	$2,24($sp)
	lw	$2,136($sp)
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
	beq	$2,$0,$L589
	lw	$4,20($sp)
	lw	$5,24($sp)
	lw	$6,16($sp)
#APP
 # 1375 "/opt/tclinux_phoenix/modules/private/xpon/src/xpondrv.c" 1
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
$L589:
	lw	$2,16($sp)
	beq	$2,$0,$L590
	li	$2,-14			# 0xfffffffffffffff2
	j	$L591
$L590:
	addiu	$2,$sp,44
	addiu	$6,$sp,108
	addiu	$3,$sp,112
	move	$4,$2
	lui	$2,%hi($LC203)
	addiu	$5,$2,%lo($LC203)
	move	$7,$3
	lui	$2,%hi(sscanf)
	addiu	$2,$2,%lo(sscanf)
	jalr	$2
	lw	$3,108($sp)
	lui	$2,%hi(to1_timeout_cnt)
	sw	$3,%lo(to1_timeout_cnt)($2)
	lw	$3,112($sp)
	lui	$2,%hi(to1_rst_cnt)
	sw	$3,%lo(to1_rst_cnt)($2)
	lw	$2,136($sp)
$L591:
	lw	$31,124($sp)
	addiu	$sp,$sp,128
	j	$31
	.end	gpon_stage_change_chk_write_proc
	.size	gpon_stage_change_chk_write_proc, .-gpon_stage_change_chk_write_proc
	.rdata
	.align	2
$LC204:
	.ascii	"**************************************\012\000"
	.align	2
$LC205:
	.ascii	"**             Raw data             **\012\000"
	.align	2
$LC206:
	.ascii	" SCU_WAN_CONF_REG         (0xBFB00070): %#03x\012\000"
	.align	2
$LC207:
	.ascii	" G_ACTIVATION     (0xBFB640BC): %#03x\012\000"
	.align	2
$LC208:
	.ascii	" G_EQD            (0xBFB640A8): %#03x\012\000"
	.align	2
$LC209:
	.ascii	" G_RSP_TIME       (0xBFB640BC): %#03x\012\000"
	.align	2
$LC210:
	.ascii	" E_MPCP_STAT      (0xBFB66120): %#03x\012\000"
	.align	2
$LC211:
	.ascii	" E_MPCP_RGST_STAT (0xBFB66124): %#03x\012\000"
	.align	2
$LC212:
	.ascii	" E_SYNC_TIME      (0xBFB660D4): %#03x\012\000"
	.section	.text.xpon_dump_raw_data,"ax",@progbits
	.align	2
	.globl	xpon_dump_raw_data
	.set	nomips16
	.set	nomicromips
	.ent	xpon_dump_raw_data
	.type	xpon_dump_raw_data, @function
xpon_dump_raw_data:
	.frame	$sp,24,$31		# vars= 0, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-24
	sw	$31,20($sp)
	sw	$4,24($sp)
	sw	$5,28($sp)
	lw	$2,28($sp)
	lw	$2,0($2)
	move	$3,$2
	lw	$2,24($sp)
	addu	$2,$2,$3
	move	$4,$2
	lui	$2,%hi($LC136)
	addiu	$5,$2,%lo($LC136)
	lui	$2,%hi(sprintf)
	addiu	$2,$2,%lo(sprintf)
	jalr	$2
	nop

	move	$3,$2
	lw	$2,28($sp)
	lw	$2,0($2)
	addu	$3,$2,$3
	lw	$2,28($sp)
	sw	$3,0($2)
	lw	$2,28($sp)
	lw	$2,0($2)
	move	$3,$2
	lw	$2,24($sp)
	addu	$2,$2,$3
	move	$4,$2
	lui	$2,%hi($LC204)
	addiu	$5,$2,%lo($LC204)
	lui	$2,%hi(sprintf)
	addiu	$2,$2,%lo(sprintf)
	jalr	$2
	nop

	move	$3,$2
	lw	$2,28($sp)
	lw	$2,0($2)
	addu	$3,$2,$3
	lw	$2,28($sp)
	sw	$3,0($2)
	lw	$2,28($sp)
	lw	$2,0($2)
	move	$3,$2
	lw	$2,24($sp)
	addu	$2,$2,$3
	move	$4,$2
	lui	$2,%hi($LC205)
	addiu	$5,$2,%lo($LC205)
	lui	$2,%hi(sprintf)
	addiu	$2,$2,%lo(sprintf)
	jalr	$2
	nop

	move	$3,$2
	lw	$2,28($sp)
	lw	$2,0($2)
	addu	$3,$2,$3
	lw	$2,28($sp)
	sw	$3,0($2)
	lw	$2,28($sp)
	lw	$2,0($2)
	move	$3,$2
	lw	$2,24($sp)
	addu	$2,$2,$3
	move	$4,$2
	lui	$2,%hi($LC204)
	addiu	$5,$2,%lo($LC204)
	lui	$2,%hi(sprintf)
	addiu	$2,$2,%lo(sprintf)
	jalr	$2
	nop

	move	$3,$2
	lw	$2,28($sp)
	lw	$2,0($2)
	addu	$3,$2,$3
	lw	$2,28($sp)
	sw	$3,0($2)
	lw	$2,28($sp)
	lw	$2,0($2)
	move	$3,$2
	lw	$2,24($sp)
	addu	$2,$2,$3
	move	$4,$2
	lui	$2,%hi($LC136)
	addiu	$5,$2,%lo($LC136)
	lui	$2,%hi(sprintf)
	addiu	$2,$2,%lo(sprintf)
	jalr	$2
	nop

	move	$3,$2
	lw	$2,28($sp)
	lw	$2,0($2)
	addu	$3,$2,$3
	lw	$2,28($sp)
	sw	$3,0($2)
	lw	$2,28($sp)
	lw	$2,0($2)
	move	$3,$2
	lw	$2,24($sp)
	addu	$4,$2,$3
	lui	$2,%hi(xpon_regs_p)
	lw	$2,%lo(xpon_regs_p)($2)
	lw	$3,0($2)
	lui	$2,%hi($LC206)
	addiu	$5,$2,%lo($LC206)
	move	$6,$3
	lui	$2,%hi(sprintf)
	addiu	$2,$2,%lo(sprintf)
	jalr	$2
	nop

	move	$3,$2
	lw	$2,28($sp)
	lw	$2,0($2)
	addu	$3,$2,$3
	lw	$2,28($sp)
	sw	$3,0($2)
	lui	$2,%hi(pon_mode)
	lbu	$2,%lo(pon_mode)($2)
	andi	$2,$2,0x1
	beq	$2,$0,$L593
	nop

	lw	$2,28($sp)
	lw	$2,0($2)
	move	$3,$2
	lw	$2,24($sp)
	addu	$4,$2,$3
	lui	$2,%hi(xpon_regs_p)
	lw	$2,%lo(xpon_regs_p)($2)
	lw	$3,20($2)
	lui	$2,%hi($LC207)
	addiu	$5,$2,%lo($LC207)
	move	$6,$3
	lui	$2,%hi(sprintf)
	addiu	$2,$2,%lo(sprintf)
	jalr	$2
	nop

	move	$3,$2
	lw	$2,28($sp)
	lw	$2,0($2)
	addu	$3,$2,$3
	lw	$2,28($sp)
	sw	$3,0($2)
	lw	$2,28($sp)
	lw	$2,0($2)
	move	$3,$2
	lw	$2,24($sp)
	addu	$4,$2,$3
	lui	$2,%hi(xpon_regs_p)
	lw	$2,%lo(xpon_regs_p)($2)
	lw	$3,4($2)
	lui	$2,%hi($LC208)
	addiu	$5,$2,%lo($LC208)
	move	$6,$3
	lui	$2,%hi(sprintf)
	addiu	$2,$2,%lo(sprintf)
	jalr	$2
	nop

	move	$3,$2
	lw	$2,28($sp)
	lw	$2,0($2)
	addu	$3,$2,$3
	lw	$2,28($sp)
	sw	$3,0($2)
	lw	$2,28($sp)
	lw	$2,0($2)
	move	$3,$2
	lw	$2,24($sp)
	addu	$4,$2,$3
	lui	$2,%hi(xpon_regs_p)
	lw	$2,%lo(xpon_regs_p)($2)
	lw	$3,16($2)
	lui	$2,%hi($LC209)
	addiu	$5,$2,%lo($LC209)
	move	$6,$3
	lui	$2,%hi(sprintf)
	addiu	$2,$2,%lo(sprintf)
	jalr	$2
	nop

	move	$3,$2
	lw	$2,28($sp)
	lw	$2,0($2)
	addu	$3,$2,$3
	lw	$2,28($sp)
	sw	$3,0($2)
$L593:
	lui	$2,%hi(pon_mode)
	lbu	$2,%lo(pon_mode)($2)
	andi	$2,$2,0x2
	beq	$2,$0,$L594
	nop

	lw	$2,28($sp)
	lw	$2,0($2)
	move	$3,$2
	lw	$2,24($sp)
	addu	$4,$2,$3
	lui	$2,%hi(xpon_regs_p)
	lw	$2,%lo(xpon_regs_p)($2)
	lw	$3,8($2)
	lui	$2,%hi($LC210)
	addiu	$5,$2,%lo($LC210)
	move	$6,$3
	lui	$2,%hi(sprintf)
	addiu	$2,$2,%lo(sprintf)
	jalr	$2
	nop

	move	$3,$2
	lw	$2,28($sp)
	lw	$2,0($2)
	addu	$3,$2,$3
	lw	$2,28($sp)
	sw	$3,0($2)
	lw	$2,28($sp)
	lw	$2,0($2)
	move	$3,$2
	lw	$2,24($sp)
	addu	$4,$2,$3
	lui	$2,%hi(xpon_regs_p)
	lw	$2,%lo(xpon_regs_p)($2)
	lw	$3,24($2)
	lui	$2,%hi($LC211)
	addiu	$5,$2,%lo($LC211)
	move	$6,$3
	lui	$2,%hi(sprintf)
	addiu	$2,$2,%lo(sprintf)
	jalr	$2
	nop

	move	$3,$2
	lw	$2,28($sp)
	lw	$2,0($2)
	addu	$3,$2,$3
	lw	$2,28($sp)
	sw	$3,0($2)
	lw	$2,28($sp)
	lw	$2,0($2)
	move	$3,$2
	lw	$2,24($sp)
	addu	$4,$2,$3
	lui	$2,%hi(xpon_regs_p)
	lw	$2,%lo(xpon_regs_p)($2)
	lw	$3,12($2)
	lui	$2,%hi($LC212)
	addiu	$5,$2,%lo($LC212)
	move	$6,$3
	lui	$2,%hi(sprintf)
	addiu	$2,$2,%lo(sprintf)
	jalr	$2
	nop

	move	$3,$2
	lw	$2,28($sp)
	lw	$2,0($2)
	addu	$3,$2,$3
	lw	$2,28($sp)
	sw	$3,0($2)
$L594:
	nop
	lw	$31,20($sp)
	addiu	$sp,$sp,24
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	xpon_dump_raw_data
	.size	xpon_dump_raw_data, .-xpon_dump_raw_data
	.rdata
	.align	2
$LC213:
	.ascii	" MPCP Status: Registered\000"
	.align	2
$LC214:
	.ascii	" MPCP Status: Registering\000"
	.align	2
$LC215:
	.ascii	" MPCP Status: Unknown\000"
	.align	2
$LC216:
	.ascii	"\011Jiffies: %#lx(+%#lx +%#ld)\012\000"
	.align	2
$LC217:
	.ascii	" Discover Gate Count:\011 %#x(+%#x +%#d)\012\000"
	.align	2
$LC218:
	.ascii	" RX Register Count: \011 %#x(+%#d)\012\000"
	.align	2
$LC219:
	.ascii	" TX Register Count: \011 %#x(+%#d)\012\000"
	.align	2
$LC220:
	.ascii	" TX Register ACK Count:  %#x(+%#d)\012\000"
	.align	2
$LC221:
	.ascii	" Sync Time: %#x\012\000"
	.section	.text.get_epon_stat,"ax",@progbits
	.align	2
	.globl	get_epon_stat
	.set	nomips16
	.set	nomicromips
	.ent	get_epon_stat
	.type	get_epon_stat, @function
get_epon_stat:
	.frame	$sp,32,$31		# vars= 0, regs= 2/0, args= 24, gp= 0
	.mask	0x80010000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-32
	sw	$31,28($sp)
	sw	$16,24($sp)
	sw	$4,32($sp)
	sw	$5,36($sp)
	sw	$6,40($sp)
	lui	$2,%hi(jiffies)
	lw	$3,%lo(jiffies)($2)
	lw	$2,40($sp)
	sw	$3,16($2)
	lui	$2,%hi(xpon_regs_p)
	lw	$16,%lo(xpon_regs_p)($2)
	lui	$2,%hi(g_EPON_MAC_BASE)
	lw	$2,%lo(g_EPON_MAC_BASE)($2)
	addiu	$2,$2,24864
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	sw	$2,8($16)
	lui	$2,%hi(xpon_regs_p)
	lw	$2,%lo(xpon_regs_p)($2)
	lw	$2,8($2)
	andi	$3,$2,0xffff
	lw	$2,40($sp)
	sw	$3,8($2)
	lui	$2,%hi(xpon_regs_p)
	lw	$2,%lo(xpon_regs_p)($2)
	lw	$3,8($2)
	li	$2,16711680			# 0xff0000
	and	$2,$3,$2
	srl	$3,$2,16
	lw	$2,40($sp)
	sw	$3,0($2)
	lui	$2,%hi(xpon_regs_p)
	lw	$16,%lo(xpon_regs_p)($2)
	lui	$2,%hi(g_EPON_MAC_BASE)
	lw	$2,%lo(g_EPON_MAC_BASE)($2)
	addiu	$2,$2,24868
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	sw	$2,24($16)
	lui	$2,%hi(xpon_regs_p)
	lw	$2,%lo(xpon_regs_p)($2)
	lw	$2,24($2)
	andi	$2,$2,0xff00
	srl	$3,$2,8
	lw	$2,40($sp)
	sw	$3,4($2)
	lui	$2,%hi(xpon_regs_p)
	lw	$2,%lo(xpon_regs_p)($2)
	lw	$2,24($2)
	andi	$3,$2,0xff
	lw	$2,40($sp)
	sw	$3,12($2)
	lui	$2,%hi(xpon_regs_p)
	lw	$16,%lo(xpon_regs_p)($2)
	lui	$2,%hi(g_EPON_MAC_BASE)
	lw	$2,%lo(g_EPON_MAC_BASE)($2)
	addiu	$2,$2,24788
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	sw	$2,12($16)
	lui	$2,%hi(pon_mode)
	lbu	$2,%lo(pon_mode)($2)
	andi	$2,$2,0x4
	beq	$2,$0,$L597
	nop

	lui	$2,%hi(epon_proc_cnt)
	lw	$2,%lo(epon_proc_cnt)($2)
	beq	$2,$0,$L598
	nop

	lw	$2,40($sp)
	lw	$3,0($2)
	lui	$2,%hi(epon_stat_prev)
	lw	$2,%lo(epon_stat_prev)($2)
	lw	$2,0($2)
	bne	$3,$2,$L598
	nop

	lw	$2,40($sp)
	lw	$3,4($2)
	lui	$2,%hi(epon_stat_prev)
	lw	$2,%lo(epon_stat_prev)($2)
	lw	$2,4($2)
	bne	$3,$2,$L598
	nop

	lw	$2,40($sp)
	lw	$3,12($2)
	lui	$2,%hi(epon_stat_prev)
	lw	$2,%lo(epon_stat_prev)($2)
	lw	$2,12($2)
	bne	$3,$2,$L598
	nop

	lw	$2,36($sp)
	lw	$2,0($2)
	move	$3,$2
	lw	$2,32($sp)
	addu	$2,$2,$3
	move	$4,$2
	lui	$2,%hi($LC213)
	addiu	$5,$2,%lo($LC213)
	lui	$2,%hi(sprintf)
	addiu	$2,$2,%lo(sprintf)
	jalr	$2
	nop

	move	$3,$2
	lw	$2,36($sp)
	lw	$2,0($2)
	addu	$3,$2,$3
	lw	$2,36($sp)
	sw	$3,0($2)
	j	$L599
	nop

$L598:
	lui	$2,%hi(epon_proc_cnt)
	lw	$2,%lo(epon_proc_cnt)($2)
	beq	$2,$0,$L600
	nop

	lw	$2,36($sp)
	lw	$2,0($2)
	move	$3,$2
	lw	$2,32($sp)
	addu	$2,$2,$3
	move	$4,$2
	lui	$2,%hi($LC214)
	addiu	$5,$2,%lo($LC214)
	lui	$2,%hi(sprintf)
	addiu	$2,$2,%lo(sprintf)
	jalr	$2
	nop

	move	$3,$2
	lw	$2,36($sp)
	lw	$2,0($2)
	addu	$3,$2,$3
	lw	$2,36($sp)
	sw	$3,0($2)
	j	$L599
	nop

$L600:
	lw	$2,36($sp)
	lw	$2,0($2)
	move	$3,$2
	lw	$2,32($sp)
	addu	$2,$2,$3
	move	$4,$2
	lui	$2,%hi($LC215)
	addiu	$5,$2,%lo($LC215)
	lui	$2,%hi(sprintf)
	addiu	$2,$2,%lo(sprintf)
	jalr	$2
	nop

	move	$3,$2
	lw	$2,36($sp)
	lw	$2,0($2)
	addu	$3,$2,$3
	lw	$2,36($sp)
	sw	$3,0($2)
$L599:
	lw	$2,36($sp)
	lw	$2,0($2)
	move	$3,$2
	lw	$2,32($sp)
	addu	$5,$2,$3
	lw	$2,40($sp)
	lw	$6,16($2)
	lui	$2,%hi(epon_proc_cnt)
	lw	$2,%lo(epon_proc_cnt)($2)
	beq	$2,$0,$L601
	nop

	lw	$2,40($sp)
	lw	$3,16($2)
	lui	$2,%hi(epon_stat_prev)
	lw	$2,%lo(epon_stat_prev)($2)
	lw	$2,16($2)
	subu	$3,$3,$2
	j	$L602
	nop

$L601:
	lui	$2,%hi(epon_proc_cnt)
	lw	$3,%lo(epon_proc_cnt)($2)
$L602:
	lui	$2,%hi(epon_proc_cnt)
	lw	$2,%lo(epon_proc_cnt)($2)
	beq	$2,$0,$L603
	nop

	lw	$2,40($sp)
	lw	$4,16($2)
	lui	$2,%hi(epon_stat_prev)
	lw	$2,%lo(epon_stat_prev)($2)
	lw	$2,16($2)
	subu	$2,$4,$2
	j	$L604
	nop

$L603:
	lui	$2,%hi(epon_proc_cnt)
	lw	$2,%lo(epon_proc_cnt)($2)
$L604:
	sw	$2,16($sp)
	move	$4,$5
	lui	$2,%hi($LC216)
	addiu	$5,$2,%lo($LC216)
	move	$7,$3
	lui	$2,%hi(sprintf)
	addiu	$2,$2,%lo(sprintf)
	jalr	$2
	nop

	move	$3,$2
	lw	$2,36($sp)
	lw	$2,0($2)
	addu	$3,$2,$3
	lw	$2,36($sp)
	sw	$3,0($2)
	lw	$2,36($sp)
	lw	$2,0($2)
	move	$3,$2
	lw	$2,32($sp)
	addu	$5,$2,$3
	lw	$2,40($sp)
	lw	$6,8($2)
	lui	$2,%hi(epon_proc_cnt)
	lw	$2,%lo(epon_proc_cnt)($2)
	beq	$2,$0,$L605
	nop

	lw	$2,40($sp)
	lw	$3,8($2)
	lui	$2,%hi(epon_stat_prev)
	lw	$2,%lo(epon_stat_prev)($2)
	lw	$2,8($2)
	subu	$3,$3,$2
	j	$L606
	nop

$L605:
	lui	$2,%hi(epon_proc_cnt)
	lw	$3,%lo(epon_proc_cnt)($2)
$L606:
	lui	$2,%hi(epon_proc_cnt)
	lw	$2,%lo(epon_proc_cnt)($2)
	beq	$2,$0,$L607
	nop

	lw	$2,40($sp)
	lw	$4,8($2)
	lui	$2,%hi(epon_stat_prev)
	lw	$2,%lo(epon_stat_prev)($2)
	lw	$2,8($2)
	subu	$2,$4,$2
	j	$L608
	nop

$L607:
	lui	$2,%hi(epon_proc_cnt)
	lw	$2,%lo(epon_proc_cnt)($2)
$L608:
	sw	$2,16($sp)
	move	$4,$5
	lui	$2,%hi($LC217)
	addiu	$5,$2,%lo($LC217)
	move	$7,$3
	lui	$2,%hi(sprintf)
	addiu	$2,$2,%lo(sprintf)
	jalr	$2
	nop

	move	$3,$2
	lw	$2,36($sp)
	lw	$2,0($2)
	addu	$3,$2,$3
	lw	$2,36($sp)
	sw	$3,0($2)
	lw	$2,36($sp)
	lw	$2,0($2)
	move	$3,$2
	lw	$2,32($sp)
	addu	$4,$2,$3
	lw	$2,40($sp)
	lw	$6,0($2)
	lui	$2,%hi(epon_proc_cnt)
	lw	$2,%lo(epon_proc_cnt)($2)
	beq	$2,$0,$L609
	nop

	lw	$2,40($sp)
	lw	$3,0($2)
	lui	$2,%hi(epon_stat_prev)
	lw	$2,%lo(epon_stat_prev)($2)
	lw	$2,0($2)
	subu	$2,$3,$2
	j	$L610
	nop

$L609:
	lui	$2,%hi(epon_proc_cnt)
	lw	$2,%lo(epon_proc_cnt)($2)
$L610:
	lui	$3,%hi($LC218)
	addiu	$5,$3,%lo($LC218)
	move	$7,$2
	lui	$2,%hi(sprintf)
	addiu	$2,$2,%lo(sprintf)
	jalr	$2
	nop

	move	$3,$2
	lw	$2,36($sp)
	lw	$2,0($2)
	addu	$3,$2,$3
	lw	$2,36($sp)
	sw	$3,0($2)
	lw	$2,36($sp)
	lw	$2,0($2)
	move	$3,$2
	lw	$2,32($sp)
	addu	$4,$2,$3
	lw	$2,40($sp)
	lw	$6,4($2)
	lui	$2,%hi(epon_proc_cnt)
	lw	$2,%lo(epon_proc_cnt)($2)
	beq	$2,$0,$L611
	nop

	lw	$2,40($sp)
	lw	$3,4($2)
	lui	$2,%hi(epon_stat_prev)
	lw	$2,%lo(epon_stat_prev)($2)
	lw	$2,4($2)
	subu	$2,$3,$2
	j	$L612
	nop

$L611:
	lui	$2,%hi(epon_proc_cnt)
	lw	$2,%lo(epon_proc_cnt)($2)
$L612:
	lui	$3,%hi($LC219)
	addiu	$5,$3,%lo($LC219)
	move	$7,$2
	lui	$2,%hi(sprintf)
	addiu	$2,$2,%lo(sprintf)
	jalr	$2
	nop

	move	$3,$2
	lw	$2,36($sp)
	lw	$2,0($2)
	addu	$3,$2,$3
	lw	$2,36($sp)
	sw	$3,0($2)
	lw	$2,36($sp)
	lw	$2,0($2)
	move	$3,$2
	lw	$2,32($sp)
	addu	$4,$2,$3
	lw	$2,40($sp)
	lw	$6,12($2)
	lui	$2,%hi(epon_proc_cnt)
	lw	$2,%lo(epon_proc_cnt)($2)
	beq	$2,$0,$L613
	nop

	lw	$2,40($sp)
	lw	$3,12($2)
	lui	$2,%hi(epon_stat_prev)
	lw	$2,%lo(epon_stat_prev)($2)
	lw	$2,12($2)
	subu	$2,$3,$2
	j	$L614
	nop

$L613:
	lui	$2,%hi(epon_proc_cnt)
	lw	$2,%lo(epon_proc_cnt)($2)
$L614:
	lui	$3,%hi($LC220)
	addiu	$5,$3,%lo($LC220)
	move	$7,$2
	lui	$2,%hi(sprintf)
	addiu	$2,$2,%lo(sprintf)
	jalr	$2
	nop

	move	$3,$2
	lw	$2,36($sp)
	lw	$2,0($2)
	addu	$3,$2,$3
	lw	$2,36($sp)
	sw	$3,0($2)
	lw	$2,36($sp)
	lw	$2,0($2)
	move	$3,$2
	lw	$2,32($sp)
	addu	$4,$2,$3
	lui	$2,%hi(xpon_regs_p)
	lw	$2,%lo(xpon_regs_p)($2)
	lw	$2,12($2)
	andi	$3,$2,0xffff
	lui	$2,%hi($LC221)
	addiu	$5,$2,%lo($LC221)
	move	$6,$3
	lui	$2,%hi(sprintf)
	addiu	$2,$2,%lo(sprintf)
	jalr	$2
	nop

	move	$3,$2
	lw	$2,36($sp)
	lw	$2,0($2)
	addu	$3,$2,$3
	lw	$2,36($sp)
	sw	$3,0($2)
	lui	$2,%hi(epon_proc_cnt)
	lw	$2,%lo(epon_proc_cnt)($2)
	addiu	$3,$2,1
	lui	$2,%hi(epon_proc_cnt)
	sw	$3,%lo(epon_proc_cnt)($2)
$L597:
	lui	$2,%hi(epon_stat_prev)
	lw	$2,%lo(epon_stat_prev)($2)
	lw	$3,40($sp)
	lw	$3,16($3)
	sw	$3,16($2)
	lui	$2,%hi(epon_stat_prev)
	lw	$2,%lo(epon_stat_prev)($2)
	lw	$3,40($sp)
	lw	$3,8($3)
	sw	$3,8($2)
	lui	$2,%hi(epon_stat_prev)
	lw	$2,%lo(epon_stat_prev)($2)
	lw	$3,40($sp)
	lw	$3,0($3)
	sw	$3,0($2)
	lui	$2,%hi(epon_stat_prev)
	lw	$2,%lo(epon_stat_prev)($2)
	lw	$3,40($sp)
	lw	$3,4($3)
	sw	$3,4($2)
	lui	$2,%hi(epon_stat_prev)
	lw	$2,%lo(epon_stat_prev)($2)
	lw	$3,40($sp)
	lw	$3,12($3)
	sw	$3,12($2)
	nop
	lw	$31,28($sp)
	lw	$16,24($sp)
	addiu	$sp,$sp,32
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	get_epon_stat
	.size	get_epon_stat, .-get_epon_stat
	.globl	mpcpStateStr
	.data
	.align	2
	.type	mpcpStateStr, @object
	.size	mpcpStateStr, 220
mpcpStateStr:
	.ascii	"WAIT\000"
	.space	15
	.ascii	"REGISTERING\000"
	.space	8
	.ascii	"REGISTER_REQUEST\000"
	.space	3
	.ascii	"REGISTER_PENDING\000"
	.space	3
	.ascii	"RETRY\000"
	.space	14
	.ascii	"DENIED\000"
	.space	13
	.ascii	"REGISTER_ACK\000"
	.space	7
	.ascii	"NACK\000"
	.space	15
	.ascii	"REGISTERED\000"
	.space	9
	.ascii	"REMOTE_DEREGISTER\000"
	.space	2
	.ascii	"LOCAL_DEREGISTER\000"
	.space	3
	.rdata
	.align	2
$LC222:
	.ascii	"MPCP disc State Index %d exceed max Num\012\000"
	.align	2
$LC223:
	.ascii	" EPON LLID %d MPCP Disc State: %s \012\000"
	.align	2
$LC224:
	.ascii	"Silent\000"
	.align	2
$LC225:
	.ascii	"Normal\000"
	.align	2
$LC226:
	.ascii	" EPON LLID %d is in  %s State \012\000"
	.align	2
$LC227:
	.ascii	" EPON LLID %d Silent Time left %d s(Total %d s) \012\000"
	.section	.text.epon_dump_llid_info,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	epon_dump_llid_info
	.type	epon_dump_llid_info, @function
epon_dump_llid_info:
	.frame	$sp,80,$31		# vars= 48, regs= 1/0, args= 24, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	addiu	$sp,$sp,-80
	sw	$31,76($sp)
	sw	$4,80($sp)
	sw	$5,84($sp)
	sb	$0,24($sp)
	sw	$0,28($sp)
	sw	$0,48($sp)
	sw	$0,52($sp)
	sw	$0,56($sp)
	sw	$0,60($sp)
	sw	$0,64($sp)
	sb	$0,32($sp)
	sb	$0,24($sp)
	j	$L617
$L624:
	lui	$2,%hi(eponDrv)
	lw	$2,%lo(eponDrv)($2)
	lbu	$3,24($sp)
	li	$4,1			# 0x1
	sll	$3,$4,$3
	and	$2,$2,$3
	beq	$2,$0,$L618
	lbu	$2,24($sp)
	sll	$2,$2,3
	sll	$3,$2,3
	addu	$2,$2,$3
	addiu	$3,$2,16
	lui	$2,%hi(eponDrv)
	addiu	$2,$2,%lo(eponDrv)
	addu	$2,$3,$2
	addiu	$2,$2,4
	sw	$2,28($sp)
	lw	$2,28($sp)
	lbu	$2,20($2)
	sb	$2,32($sp)
	lbu	$2,32($sp)
	sltu	$2,$2,11
	bne	$2,$0,$L619
	lbu	$3,32($sp)
	lui	$2,%hi($LC222)
	addiu	$4,$2,%lo($LC222)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	j	$L616
$L619:
	lbu	$2,32($sp)
	sll	$2,$2,2
	sll	$3,$2,2
	addu	$2,$2,$3
	lui	$3,%hi(mpcpStateStr)
	addiu	$3,$3,%lo(mpcpStateStr)
	addu	$2,$2,$3
	addiu	$3,$sp,48
	sw	$3,36($sp)
	sw	$2,40($sp)
	lw	$2,36($sp)
	sw	$2,44($sp)
	lw	$3,36($sp)
	lw	$2,40($sp)
#APP
 # 27 "/opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/string.h" 1
	.set	noreorder
	.set	noat
1:	lbu	$1,($2)
	addiu	$2,1
	sb	$1,($3)
	bnez	$1,1b
	addiu	$3,1
	.set	at
	.set	reorder
 # 0 "" 2
#NO_APP
	sw	$3,36($sp)
	sw	$2,40($sp)
	lw	$2,84($sp)
	lw	$2,0($2)
	move	$3,$2
	lw	$2,80($sp)
	addu	$2,$2,$3
	lbu	$6,24($sp)
	addiu	$3,$sp,48
	move	$4,$2
	lui	$2,%hi($LC223)
	addiu	$5,$2,%lo($LC223)
	move	$7,$3
	lui	$2,%hi(sprintf)
	addiu	$2,$2,%lo(sprintf)
	jalr	$2
	move	$3,$2
	lw	$2,84($sp)
	lw	$2,0($2)
	addu	$3,$2,$3
	lw	$2,84($sp)
	sw	$3,0($2)
	lw	$2,84($sp)
	lw	$2,0($2)
	move	$3,$2
	lw	$2,80($sp)
	addu	$4,$2,$3
	lbu	$6,24($sp)
	lbu	$3,32($sp)
	li	$2,5			# 0x5
	bne	$3,$2,$L622
	lui	$2,%hi($LC224)
	addiu	$2,$2,%lo($LC224)
	j	$L623
$L622:
	lui	$2,%hi($LC225)
	addiu	$2,$2,%lo($LC225)
$L623:
	lui	$3,%hi($LC226)
	addiu	$5,$3,%lo($LC226)
	move	$7,$2
	lui	$2,%hi(sprintf)
	addiu	$2,$2,%lo(sprintf)
	jalr	$2
	move	$3,$2
	lw	$2,84($sp)
	lw	$2,0($2)
	addu	$3,$2,$3
	lw	$2,84($sp)
	sw	$3,0($2)
	lbu	$3,32($sp)
	li	$2,5			# 0x5
	bne	$3,$2,$L618
	lw	$2,84($sp)
	lw	$2,0($2)
	move	$3,$2
	lw	$2,80($sp)
	addu	$4,$2,$3
	lbu	$3,24($sp)
	lw	$2,28($sp)
	lhu	$2,68($2)
	move	$7,$2
	lui	$2,%hi(g_silence_time)
	lbu	$2,%lo(g_silence_time)($2)
	sw	$2,16($sp)
	lui	$2,%hi($LC227)
	addiu	$5,$2,%lo($LC227)
	move	$6,$3
	lui	$2,%hi(sprintf)
	addiu	$2,$2,%lo(sprintf)
	jalr	$2
	move	$3,$2
	lw	$2,84($sp)
	lw	$2,0($2)
	addu	$3,$2,$3
	lw	$2,84($sp)
	sw	$3,0($2)
$L618:
	lbu	$2,24($sp)
	addiu	$2,$2,1
	sb	$2,24($sp)
$L617:
	lbu	$2,24($sp)
	sltu	$2,$2,8
	bne	$2,$0,$L624
$L616:
	lw	$31,76($sp)
	addiu	$sp,$sp,80
	j	$31
	.end	epon_dump_llid_info
	.size	epon_dump_llid_info, .-epon_dump_llid_info
	.rdata
	.align	2
$LC228:
	.ascii	"Error: can't allocate buffer for epon_stat_prev.\012\000"
	.align	2
$LC229:
	.ascii	"Error: can't allocate buffer for epon_stat_cur.\012\000"
	.align	2
$LC230:
	.ascii	"Error: can't allocate buffer for xpon_regs_p.\012\000"
	.align	2
$LC231:
	.ascii	"%s\012\000"
	.align	2
$LC232:
	.ascii	"**************************************\000"
	.align	2
$LC233:
	.ascii	"**        XPON MAC Status           **\000"
	.align	2
$LC234:
	.ascii	" Mode: GPON\012\000"
	.align	2
$LC235:
	.ascii	" Mode: EPON\012\000"
	.align	2
$LC236:
	.ascii	" Mode: Error\012\000"
	.align	2
$LC237:
	.ascii	" Ploam state: O%d\012\000"
	.align	2
$LC238:
	.ascii	" Equalization Delay: %#x\012\000"
	.align	2
$LC239:
	.ascii	" Response Time: %#x\012\000"
	.section	.text.xpon_info_read_proc,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	xpon_info_read_proc
	.type	xpon_info_read_proc, @function
xpon_info_read_proc:
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
	sw	$7,68($sp)
	lui	$2,%hi(pon_mode)
	sb	$0,%lo(pon_mode)($2)
	lui	$2,%hi(epon_proc_cnt)
	lw	$2,%lo(epon_proc_cnt)($2)
	bne	$2,$0,$L626
	nop

	li	$2,20			# 0x14
	sw	$2,20($sp)
	li	$2,208			# 0xd0
	sw	$2,24($sp)
$L627 = .
	lw	$4,20($sp)
	lw	$5,24($sp)
	lui	$2,%hi(__kmalloc)
	addiu	$2,$2,%lo(__kmalloc)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(epon_stat_prev)
	sw	$3,%lo(epon_stat_prev)($2)
	lui	$2,%hi(epon_stat_prev)
	lw	$2,%lo(epon_stat_prev)($2)
	bne	$2,$0,$L626
	nop

	lui	$2,%hi($LC228)
	addiu	$4,$2,%lo($LC228)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	li	$2,-1			# 0xffffffffffffffff
	j	$L644
	nop

$L626:
	li	$2,20			# 0x14
	sw	$2,28($sp)
	li	$2,208			# 0xd0
	sw	$2,32($sp)
$L630 = .
	lw	$4,28($sp)
	lw	$5,32($sp)
	lui	$2,%hi(__kmalloc)
	addiu	$2,$2,%lo(__kmalloc)
	jalr	$2
	nop

	sw	$2,16($sp)
	lw	$2,16($sp)
	bne	$2,$0,$L632
	nop

	lui	$2,%hi($LC229)
	addiu	$4,$2,%lo($LC229)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	li	$2,-1			# 0xffffffffffffffff
	j	$L644
	nop

$L632:
	li	$2,28			# 0x1c
	sw	$2,36($sp)
	li	$2,208			# 0xd0
	sw	$2,40($sp)
$L633 = .
	lw	$4,36($sp)
	lw	$5,40($sp)
	lui	$2,%hi(__kmalloc)
	addiu	$2,$2,%lo(__kmalloc)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(xpon_regs_p)
	sw	$3,%lo(xpon_regs_p)($2)
	lui	$2,%hi(xpon_regs_p)
	lw	$2,%lo(xpon_regs_p)($2)
	bne	$2,$0,$L635
	nop

	lui	$2,%hi($LC230)
	addiu	$4,$2,%lo($LC230)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	li	$2,-1			# 0xffffffffffffffff
	j	$L644
	nop

$L635:
	lw	$4,56($sp)
	lui	$2,%hi($LC136)
	addiu	$5,$2,%lo($LC136)
	lui	$2,%hi(sprintf)
	addiu	$2,$2,%lo(sprintf)
	jalr	$2
	nop

	sw	$2,44($sp)
	lw	$2,44($sp)
	move	$3,$2
	lw	$2,56($sp)
	addu	$2,$2,$3
	move	$4,$2
	lui	$2,%hi($LC231)
	addiu	$5,$2,%lo($LC231)
	lui	$2,%hi($LC232)
	addiu	$6,$2,%lo($LC232)
	lui	$2,%hi(sprintf)
	addiu	$2,$2,%lo(sprintf)
	jalr	$2
	nop

	move	$3,$2
	lw	$2,44($sp)
	addu	$2,$3,$2
	sw	$2,44($sp)
	lw	$2,44($sp)
	move	$3,$2
	lw	$2,56($sp)
	addu	$2,$2,$3
	move	$4,$2
	lui	$2,%hi($LC231)
	addiu	$5,$2,%lo($LC231)
	lui	$2,%hi($LC233)
	addiu	$6,$2,%lo($LC233)
	lui	$2,%hi(sprintf)
	addiu	$2,$2,%lo(sprintf)
	jalr	$2
	nop

	move	$3,$2
	lw	$2,44($sp)
	addu	$2,$3,$2
	sw	$2,44($sp)
	lw	$2,44($sp)
	move	$3,$2
	lw	$2,56($sp)
	addu	$2,$2,$3
	move	$4,$2
	lui	$2,%hi($LC231)
	addiu	$5,$2,%lo($LC231)
	lui	$2,%hi($LC232)
	addiu	$6,$2,%lo($LC232)
	lui	$2,%hi(sprintf)
	addiu	$2,$2,%lo(sprintf)
	jalr	$2
	nop

	move	$3,$2
	lw	$2,44($sp)
	addu	$2,$3,$2
	sw	$2,44($sp)
	lw	$2,44($sp)
	move	$3,$2
	lw	$2,56($sp)
	addu	$2,$2,$3
	move	$4,$2
	lui	$2,%hi($LC136)
	addiu	$5,$2,%lo($LC136)
	lui	$2,%hi(sprintf)
	addiu	$2,$2,%lo(sprintf)
	jalr	$2
	nop

	move	$3,$2
	lw	$2,44($sp)
	addu	$2,$3,$2
	sw	$2,44($sp)
	lui	$2,%hi(xpon_regs_p)
	lw	$16,%lo(xpon_regs_p)($2)
	li	$2,-1078984704			# 0xffffffffbfb00000
	ori	$4,$2,0x70
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	andi	$2,$2,0x1
	sw	$2,0($16)
	lui	$2,%hi(xpon_regs_p)
	lw	$2,%lo(xpon_regs_p)($2)
	lw	$2,0($2)
	bne	$2,$0,$L636
	nop

	lui	$2,%hi(gpPhyData)
	lw	$2,%lo(gpPhyData)($2)
	lw	$3,60($2)
	li	$2,100663296			# 0x6000000
	and	$3,$3,$2
	li	$2,33554432			# 0x2000000
	bne	$3,$2,$L636
	nop

	lui	$2,%hi(pon_mode)
	lbu	$2,%lo(pon_mode)($2)
	ori	$2,$2,0x1
	andi	$3,$2,0x00ff
	lui	$2,%hi(pon_mode)
	sb	$3,%lo(pon_mode)($2)
	lw	$2,44($sp)
	move	$3,$2
	lw	$2,56($sp)
	addu	$2,$2,$3
	move	$4,$2
	lui	$2,%hi($LC234)
	addiu	$5,$2,%lo($LC234)
	lui	$2,%hi(sprintf)
	addiu	$2,$2,%lo(sprintf)
	jalr	$2
	nop

	move	$3,$2
	lw	$2,44($sp)
	addu	$2,$3,$2
	sw	$2,44($sp)
	j	$L637
	nop

$L636:
	lui	$2,%hi(xpon_regs_p)
	lw	$2,%lo(xpon_regs_p)($2)
	lw	$3,0($2)
	li	$2,1			# 0x1
	bne	$3,$2,$L638
	nop

	lui	$2,%hi(gpPhyData)
	lw	$2,%lo(gpPhyData)($2)
	lw	$3,60($2)
	li	$2,100663296			# 0x6000000
	and	$2,$3,$2
	bne	$2,$0,$L638
	nop

	lui	$2,%hi(pon_mode)
	lbu	$2,%lo(pon_mode)($2)
	ori	$2,$2,0x2
	andi	$3,$2,0x00ff
	lui	$2,%hi(pon_mode)
	sb	$3,%lo(pon_mode)($2)
	lw	$2,44($sp)
	move	$3,$2
	lw	$2,56($sp)
	addu	$2,$2,$3
	move	$4,$2
	lui	$2,%hi($LC235)
	addiu	$5,$2,%lo($LC235)
	lui	$2,%hi(sprintf)
	addiu	$2,$2,%lo(sprintf)
	jalr	$2
	nop

	move	$3,$2
	lw	$2,44($sp)
	addu	$2,$3,$2
	sw	$2,44($sp)
	j	$L637
	nop

$L638:
	lw	$2,44($sp)
	move	$3,$2
	lw	$2,56($sp)
	addu	$2,$2,$3
	move	$4,$2
	lui	$2,%hi($LC236)
	addiu	$5,$2,%lo($LC236)
	lui	$2,%hi(sprintf)
	addiu	$2,$2,%lo(sprintf)
	jalr	$2
	nop

	move	$3,$2
	lw	$2,44($sp)
	addu	$2,$3,$2
	sw	$2,44($sp)
$L637:
	lui	$2,%hi(pon_mode)
	lbu	$2,%lo(pon_mode)($2)
	andi	$2,$2,0x1
	beq	$2,$0,$L639
	nop

	lui	$2,%hi(xpon_regs_p)
	lw	$16,%lo(xpon_regs_p)($2)
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16572
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	sw	$2,20($16)
	lui	$2,%hi(xpon_regs_p)
	lw	$16,%lo(xpon_regs_p)($2)
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16552
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	sw	$2,4($16)
	lui	$2,%hi(xpon_regs_p)
	lw	$16,%lo(xpon_regs_p)($2)
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16556
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	sw	$2,16($16)
	lui	$2,%hi(pon_mode)
	lbu	$2,%lo(pon_mode)($2)
	andi	$2,$2,0x4
	beq	$2,$0,$L639
	nop

	lw	$2,44($sp)
	move	$3,$2
	lw	$2,56($sp)
	addu	$4,$2,$3
	lui	$2,%hi(xpon_regs_p)
	lw	$2,%lo(xpon_regs_p)($2)
	lw	$2,20($2)
	andi	$3,$2,0x7
	lui	$2,%hi($LC237)
	addiu	$5,$2,%lo($LC237)
	move	$6,$3
	lui	$2,%hi(sprintf)
	addiu	$2,$2,%lo(sprintf)
	jalr	$2
	nop

	move	$3,$2
	lw	$2,44($sp)
	addu	$2,$3,$2
	sw	$2,44($sp)
	lw	$2,44($sp)
	move	$3,$2
	lw	$2,56($sp)
	addu	$4,$2,$3
	lui	$2,%hi(xpon_regs_p)
	lw	$2,%lo(xpon_regs_p)($2)
	lw	$3,4($2)
	lui	$2,%hi($LC238)
	addiu	$5,$2,%lo($LC238)
	move	$6,$3
	lui	$2,%hi(sprintf)
	addiu	$2,$2,%lo(sprintf)
	jalr	$2
	nop

	move	$3,$2
	lw	$2,44($sp)
	addu	$2,$3,$2
	sw	$2,44($sp)
	lw	$2,44($sp)
	move	$3,$2
	lw	$2,56($sp)
	addu	$4,$2,$3
	lui	$2,%hi(xpon_regs_p)
	lw	$2,%lo(xpon_regs_p)($2)
	lw	$3,16($2)
	lui	$2,%hi($LC239)
	addiu	$5,$2,%lo($LC239)
	move	$6,$3
	lui	$2,%hi(sprintf)
	addiu	$2,$2,%lo(sprintf)
	jalr	$2
	nop

	move	$3,$2
	lw	$2,44($sp)
	addu	$2,$3,$2
	sw	$2,44($sp)
$L639:
	lui	$2,%hi(pon_mode)
	lbu	$2,%lo(pon_mode)($2)
	andi	$2,$2,0x2
	beq	$2,$0,$L640
	nop

	addiu	$2,$sp,44
	lw	$4,56($sp)
	move	$5,$2
	lw	$6,16($sp)
	lui	$2,%hi(get_epon_stat)
	addiu	$2,$2,%lo(get_epon_stat)
	jalr	$2
	nop

	addiu	$2,$sp,44
	lw	$4,56($sp)
	move	$5,$2
	lui	$2,%hi(epon_dump_llid_info)
	addiu	$2,$2,%lo(epon_dump_llid_info)
	jalr	$2
	nop

$L640:
	addiu	$2,$sp,44
	lw	$4,56($sp)
	move	$5,$2
	lui	$2,%hi(xpon_dump_raw_data)
	addiu	$2,$2,%lo(xpon_dump_raw_data)
	jalr	$2
	nop

	lw	$4,16($sp)
	lui	$2,%hi(kfree)
	addiu	$2,$2,%lo(kfree)
	jalr	$2
	nop

	lui	$2,%hi(xpon_regs_p)
	lw	$2,%lo(xpon_regs_p)($2)
	move	$4,$2
	lui	$2,%hi(kfree)
	addiu	$2,$2,%lo(kfree)
	jalr	$2
	nop

	lw	$3,44($sp)
	lw	$2,64($sp)
	subu	$2,$3,$2
	sw	$2,44($sp)
	lw	$2,64($sp)
	lw	$3,56($sp)
	addu	$3,$3,$2
	lw	$2,60($sp)
	sw	$3,0($2)
	lw	$3,44($sp)
	lw	$2,68($sp)
	slt	$2,$2,$3
	beq	$2,$0,$L641
	nop

	lw	$2,68($sp)
	sw	$2,44($sp)
	j	$L642
	nop

$L641:
	lw	$2,72($sp)
	li	$3,1			# 0x1
	sw	$3,0($2)
$L642:
	lw	$2,44($sp)
	bgez	$2,$L643
	nop

	sw	$0,44($sp)
$L643:
	lw	$2,44($sp)
$L644:
	lw	$31,52($sp)
	lw	$16,48($sp)
	addiu	$sp,$sp,56
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	xpon_info_read_proc
	.size	xpon_info_read_proc, .-xpon_info_read_proc
	.rdata
	.align	2
$LC240:
	.ascii	"%s:%d id:%d\012\000"
	.align	2
$LC241:
	.ascii	"%s:%d [%s] ignore phy los!\012\000"
	.align	2
$LC242:
	.ascii	"Illegal value of gpPhyData->working_mode: %d! %s:%d\012\000"
	.section	.text.xpon_phy_los_illegal_handler,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	xpon_phy_los_illegal_handler
	.type	xpon_phy_los_illegal_handler, @function
xpon_phy_los_illegal_handler:
	.frame	$sp,24,$31		# vars= 0, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-24
	sw	$31,20($sp)
	sw	$4,24($sp)
	sw	$5,28($sp)
	lui	$2,%hi(xpon_mac_print_open)
	lw	$2,%lo(xpon_mac_print_open)($2)
	beq	$2,$0,$L646
	nop

	lui	$2,%hi($LC240)
	addiu	$4,$2,%lo($LC240)
	lui	$2,%hi(__func__.34006)
	addiu	$5,$2,%lo(__func__.34006)
	li	$6,1582			# 0x62e
	lw	$7,28($sp)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L646:
	lui	$2,%hi(gpPhyData)
	lw	$2,%lo(gpPhyData)($2)
	lw	$3,60($2)
	li	$2,14680064			# 0xe00000
	and	$3,$3,$2
	li	$2,2097152			# 0x200000
	bne	$3,$2,$L647
	nop

	lui	$2,%hi(gpPhyData)
	lw	$2,%lo(gpPhyData)($2)
	lw	$3,60($2)
	li	$2,100663296			# 0x6000000
	and	$3,$3,$2
	li	$2,67108864			# 0x4000000
	bne	$3,$2,$L648
	nop

$L647:
	li	$2,1			# 0x1
	j	$L650
	nop

$L648:
	move	$2,$0
$L650:
	beq	$2,$0,$L651
	nop

	lui	$2,%hi(xpon_mac_print_open)
	lw	$2,%lo(xpon_mac_print_open)($2)
	beq	$2,$0,$L652
	nop

	lui	$2,%hi($LC241)
	addiu	$4,$2,%lo($LC241)
	lui	$2,%hi(__func__.34006)
	addiu	$5,$2,%lo(__func__.34006)
	li	$6,1585			# 0x631
	lui	$2,%hi(__func__.34006)
	addiu	$7,$2,%lo(__func__.34006)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	j	$L645
	nop

$L652:
	j	$L645
	nop

$L651:
	li	$4,2			# 0x2
	lui	$2,%hi(change_alarm_led_status)
	addiu	$2,$2,%lo(change_alarm_led_status)
	jalr	$2
	nop

	lui	$2,%hi(xpon_los_status)
	sw	$0,%lo(xpon_los_status)($2)
	lui	$2,%hi(gpPhyData)
	lw	$2,%lo(gpPhyData)($2)
	lw	$2,60($2)
	ext	$2,$2,25,2
	andi	$2,$2,0x00ff
	beq	$2,$0,$L655
	nop

	li	$3,1			# 0x1
	bne	$2,$3,$L657
	nop

	lui	$2,%hi(gpon_detect_los_lof)
	addiu	$2,$2,%lo(gpon_detect_los_lof)
	jalr	$2
	nop

	j	$L645
	nop

$L655:
	lui	$2,%hi(eponDetectPhyLosLof)
	addiu	$2,$2,%lo(eponDetectPhyLosLof)
	jalr	$2
	nop

	j	$L645
	nop

$L657:
	lui	$2,%hi(dump_stack)
	addiu	$2,$2,%lo(dump_stack)
	jalr	$2
	nop

	lui	$2,%hi(gpPhyData)
	lw	$2,%lo(gpPhyData)($2)
	lw	$2,60($2)
	ext	$2,$2,25,2
	andi	$2,$2,0x00ff
	move	$3,$2
	lui	$2,%hi($LC242)
	addiu	$4,$2,%lo($LC242)
	move	$5,$3
	lui	$2,%hi(__func__.34006)
	addiu	$6,$2,%lo(__func__.34006)
	li	$7,1605			# 0x645
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	nop
$L645:
	lw	$31,20($sp)
	addiu	$sp,$sp,24
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	xpon_phy_los_illegal_handler
	.size	xpon_phy_los_illegal_handler, .-xpon_phy_los_illegal_handler
	.section	.text.xpon_prepare_params,"ax",@progbits
	.align	2
	.globl	xpon_prepare_params
	.set	nomips16
	.set	nomicromips
	.ent	xpon_prepare_params
	.type	xpon_prepare_params, @function
xpon_prepare_params:
	.frame	$sp,16,$31		# vars= 16, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-16
	sw	$4,16($sp)
	sw	$5,20($sp)
	sw	$6,24($sp)
	sw	$0,0($sp)
	sw	$0,4($sp)
	sw	$0,8($sp)
	sw	$0,4($sp)
	j	$L659
	nop

$L668:
	lw	$2,8($sp)
	beq	$2,$0,$L661
	nop

	li	$3,1			# 0x1
	beq	$2,$3,$L662
	nop

	j	$L669
	nop

$L661:
	lw	$3,16($sp)
	lw	$2,4($sp)
	addu	$2,$3,$2
	lb	$3,0($2)
	li	$2,32			# 0x20
	beq	$3,$2,$L663
	nop

	lw	$3,16($sp)
	lw	$2,4($sp)
	addu	$2,$3,$2
	lb	$3,0($2)
	li	$2,10			# 0xa
	beq	$3,$2,$L663
	nop

	li	$2,1			# 0x1
	sw	$2,8($sp)
	lw	$2,0($sp)
	sll	$2,$2,2
	lw	$3,24($sp)
	addu	$2,$3,$2
	lw	$4,16($sp)
	lw	$3,4($sp)
	addu	$3,$4,$3
	sw	$3,0($2)
	j	$L664
	nop

$L663:
	j	$L664
	nop

$L662:
	lw	$3,16($sp)
	lw	$2,4($sp)
	addu	$2,$3,$2
	lb	$3,0($2)
	li	$2,32			# 0x20
	beq	$3,$2,$L665
	nop

	lw	$3,16($sp)
	lw	$2,4($sp)
	addu	$2,$3,$2
	lb	$3,0($2)
	li	$2,10			# 0xa
	bne	$3,$2,$L666
	nop

$L665:
	lw	$3,16($sp)
	lw	$2,4($sp)
	addu	$2,$3,$2
	sb	$0,0($2)
	sw	$0,8($sp)
	lw	$2,0($sp)
	addiu	$2,$2,1
	sw	$2,0($sp)
	lw	$2,0($sp)
	sltu	$2,$2,5
	bne	$2,$0,$L666
	nop

	li	$2,1			# 0x1
	j	$L667
	nop

$L666:
	j	$L664
	nop

$L669:
	move	$2,$0
	j	$L667
	nop

$L664:
	lw	$2,4($sp)
	addiu	$2,$2,1
	sw	$2,4($sp)
$L659:
	lw	$3,16($sp)
	lw	$2,4($sp)
	addu	$2,$3,$2
	lb	$2,0($2)
	bne	$2,$0,$L668
	nop

	lw	$2,20($sp)
	lw	$3,0($sp)
	sw	$3,0($2)
	li	$2,1			# 0x1
$L667:
	addiu	$sp,$sp,16
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	xpon_prepare_params
	.size	xpon_prepare_params, .-xpon_prepare_params
	.rdata
	.align	2
$LC243:
	.ascii	"%s:%d [%s] ignore phy ready!\012\000"
	.section	.text.xpon_phy_ready_handler,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	xpon_phy_ready_handler
	.type	xpon_phy_ready_handler, @function
xpon_phy_ready_handler:
	.frame	$sp,64,$31		# vars= 40, regs= 2/0, args= 16, gp= 0
	.mask	0x80010000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-64
	sw	$31,60($sp)
	sw	$16,56($sp)
	sw	$4,64($sp)
	sw	$0,16($sp)
	lui	$2,%hi(gpPhyData)
	lw	$2,%lo(gpPhyData)($2)
	lw	$3,60($2)
	li	$2,100663296			# 0x6000000
	and	$3,$3,$2
	li	$2,33554432			# 0x2000000
	bne	$3,$2,$L671
	nop

	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	addiu	$2,$2,312
	move	$4,$2
	lui	$2,%hi(gponDevSetSniffMode)
	addiu	$2,$2,%lo(gponDevSetSniffMode)
	jalr	$2
	nop

$L671:
	lui	$2,%hi(gpPhyData)
	lw	$2,%lo(gpPhyData)($2)
	lw	$3,60($2)
	li	$2,14680064			# 0xe00000
	and	$3,$3,$2
	li	$2,-2097152			# 0xffffffffffe00000
	addu	$2,$3,$2
	sltu	$2,$2,1
	andi	$2,$2,0x00ff
	beq	$2,$0,$L673
	nop

	lui	$2,%hi(xpon_mac_print_open)
	lw	$2,%lo(xpon_mac_print_open)($2)
	beq	$2,$0,$L674
	nop

	lui	$2,%hi($LC243)
	addiu	$4,$2,%lo($LC243)
	lui	$2,%hi(__func__.34036)
	addiu	$5,$2,%lo(__func__.34036)
	li	$6,1668			# 0x684
	lui	$2,%hi(__func__.34036)
	addiu	$7,$2,%lo(__func__.34036)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	j	$L670
	nop

$L674:
	j	$L670
	nop

$L673:
	lui	$2,%hi(xpon_los_status)
	li	$3,1			# 0x1
	sw	$3,%lo(xpon_los_status)($2)
$L686:
	lui	$2,%hi(gpPhyData)
	lw	$2,%lo(gpPhyData)($2)
	lw	$3,60($2)
	li	$2,100663296			# 0x6000000
	and	$3,$3,$2
	li	$2,67108864			# 0x4000000
	bne	$3,$2,$L676
	nop

	lui	$2,%hi(gpPhyData)
	lw	$3,%lo(gpPhyData)($2)
	lhu	$2,60($3)
	li	$4,2			# 0x2
	ins	$2,$4,11,2
	sh	$2,60($3)
	lui	$2,%hi(gpPhyData)
	lw	$16,%lo(gpPhyData)($2)
	li	$2,32794			# 0x801a
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
	bne	$3,$2,$L677
	nop

	lw	$2,32($sp)
	lw	$3,4($2)
	lui	$2,%hi($LC182)
	addiu	$4,$2,%lo($LC182)
	move	$5,$3
	lui	$2,%hi(panic)
	addiu	$2,$2,%lo(panic)
	jalr	$2
	nop

$L677:
	lw	$2,32($sp)
	lw	$3,8($2)
	li	$2,-1			# 0xffffffffffffffff
	bne	$3,$2,$L678
	nop

	lw	$2,32($sp)
	lw	$5,0($2)
	lw	$2,32($sp)
	lw	$3,4($2)
	lui	$2,%hi($LC183)
	addiu	$4,$2,%lo($LC183)
	move	$6,$3
	lui	$2,%hi(panic)
	addiu	$2,$2,%lo(panic)
	jalr	$2
	nop

$L678:
	lw	$2,32($sp)
	lw	$2,8($2)
	andi	$2,$2,0x00ff
	andi	$2,$2,0x3
	andi	$3,$2,0x00ff
	lhu	$2,60($16)
	ins	$2,$3,9,2
	sh	$2,60($16)
	j	$L682
	nop

$L676:
	lui	$2,%hi(gpPhyData)
	lw	$3,%lo(gpPhyData)($2)
	lhu	$2,60($3)
	li	$4,1			# 0x1
	ins	$2,$4,5,3
	sh	$2,60($3)
	lui	$2,%hi(gpPhyData)
	lw	$2,%lo(gpPhyData)($2)
	lw	$2,60($2)
	ext	$2,$2,25,2
	andi	$2,$2,0x00ff
	beq	$2,$0,$L684
	nop

	li	$3,1			# 0x1
	bne	$2,$3,$L687
	nop

	lui	$2,%hi(gpon_detect_phy_ready)
	addiu	$2,$2,%lo(gpon_detect_phy_ready)
	jalr	$2
	nop

	li	$2,1			# 0x1
	sw	$2,16($sp)
	j	$L682
	nop

$L684:
	lui	$2,%hi(enable_cpu_us_traffic)
	addiu	$2,$2,%lo(enable_cpu_us_traffic)
	jalr	$2
	nop

	lui	$2,%hi(eponDetectPhyReady)
	addiu	$2,$2,%lo(eponDetectPhyReady)
	jalr	$2
	nop

	li	$2,1			# 0x1
	sw	$2,16($sp)
	j	$L682
	nop

$L687:
	lui	$2,%hi(dump_stack)
	addiu	$2,$2,%lo(dump_stack)
	jalr	$2
	nop

	lui	$2,%hi(gpPhyData)
	lw	$2,%lo(gpPhyData)($2)
	lw	$2,60($2)
	ext	$2,$2,25,2
	andi	$2,$2,0x00ff
	move	$3,$2
	lui	$2,%hi($LC242)
	addiu	$4,$2,%lo($LC242)
	move	$5,$3
	lui	$2,%hi(__func__.34036)
	addiu	$6,$2,%lo(__func__.34036)
	li	$7,1699			# 0x6a3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	nop
$L682:
	lw	$2,16($sp)
	beq	$2,$0,$L686
	nop

$L670:
	lw	$31,60($sp)
	lw	$16,56($sp)
	addiu	$sp,$sp,64
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	xpon_phy_ready_handler
	.size	xpon_phy_ready_handler, .-xpon_phy_ready_handler
	.section	.text.xpon_phy_start_rouge_handle,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	xpon_phy_start_rouge_handle
	.type	xpon_phy_start_rouge_handle, @function
xpon_phy_start_rouge_handle:
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	li	$3,1			# 0x1
	sw	$3,-30416($2)
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	xpon_phy_start_rouge_handle
	.size	xpon_phy_start_rouge_handle, .-xpon_phy_start_rouge_handle
	.section	.text.xpon_phy_stop_rouge_handle,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	xpon_phy_stop_rouge_handle
	.type	xpon_phy_stop_rouge_handle, @function
xpon_phy_stop_rouge_handle:
	.frame	$sp,56,$31		# vars= 32, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-56
	sw	$31,52($sp)
	lui	$2,%hi(gpPhyData)
	lw	$2,%lo(gpPhyData)($2)
	lw	$3,60($2)
	li	$2,100663296			# 0x6000000
	and	$3,$3,$2
	li	$2,33554432			# 0x2000000
	bne	$3,$2,$L690
	nop

	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lw	$3,4($2)
	li	$2,7			# 0x7
	bne	$3,$2,$L690
	nop

	lui	$2,%hi(gpGponPriv)
	lw	$3,%lo(gpGponPriv)($2)
	lbu	$2,464($3)
	ins	$2,$0,5,1
	sb	$2,464($3)
	move	$4,$0
	lui	$2,%hi(gpon_disable_with_option)
	addiu	$2,$2,%lo(gpon_disable_with_option)
	jalr	$2
	nop

	li	$2,1			# 0x1
	sw	$2,28($sp)
	li	$2,12			# 0xc
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
	bne	$3,$2,$L692
	nop

	lw	$2,24($sp)
	lw	$3,4($2)
	lui	$2,%hi($LC182)
	addiu	$4,$2,%lo($LC182)
	move	$5,$3
	lui	$2,%hi(panic)
	addiu	$2,$2,%lo(panic)
	jalr	$2
	nop

$L692:
	lw	$2,24($sp)
	lw	$3,8($2)
	li	$2,-1			# 0xffffffffffffffff
	bne	$3,$2,$L693
	nop

	lw	$2,24($sp)
	lw	$5,0($2)
	lw	$2,24($sp)
	lw	$3,4($2)
	lui	$2,%hi($LC183)
	addiu	$4,$2,%lo($LC183)
	move	$6,$3
	lui	$2,%hi(panic)
	addiu	$2,$2,%lo(panic)
	jalr	$2
	nop

$L693:
	li	$4,1			# 0x1
	li	$5,9			# 0x9
	move	$6,$0
	lui	$2,%hi(xmcs_report_event)
	addiu	$2,$2,%lo(xmcs_report_event)
	jalr	$2
	nop

	move	$4,$0
	lui	$2,%hi(change_alarm_led_status)
	addiu	$2,$2,%lo(change_alarm_led_status)
	jalr	$2
	nop

$L690:
	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	sw	$0,-30416($2)
	lw	$31,52($sp)
	addiu	$sp,$sp,56
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	xpon_phy_stop_rouge_handle
	.size	xpon_phy_stop_rouge_handle, .-xpon_phy_stop_rouge_handle
	.section	.text.xpon_tx_power_handle,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	xpon_tx_power_handle
	.type	xpon_tx_power_handle, @function
xpon_tx_power_handle:
	.frame	$sp,8,$31		# vars= 8, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-8
	move	$2,$4
	sb	$2,8($sp)
	sb	$0,0($sp)
	addiu	$sp,$sp,8
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	xpon_tx_power_handle
	.size	xpon_tx_power_handle, .-xpon_tx_power_handle
	.rdata
	.align	2
$LC244:
	.ascii	"%s:%d PON WAN STOP! Ignore phy event:%d!\012\000"
	.align	2
$LC245:
	.ascii	"%s:%d Phy event:%d!\012\000"
	.align	2
$LC246:
	.ascii	"Event %d handler not implemented! %s:%d\012\000"
	.section	.text.xpon_phy_event_dispatch,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	xpon_phy_event_dispatch
	.type	xpon_phy_event_dispatch, @function
xpon_phy_event_dispatch:
	.frame	$sp,24,$31		# vars= 0, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-24
	sw	$31,20($sp)
	sw	$4,24($sp)
	lui	$2,%hi(gpPonSysData)
	lw	$2,%lo(gpPonSysData)($2)
	lw	$2,8($2)
	beq	$2,$0,$L699
	nop

	lui	$2,%hi(gpPhyData)
	lw	$2,%lo(gpPhyData)($2)
	lw	$3,60($2)
	li	$2,16777216			# 0x1000000
	and	$2,$3,$2
	beq	$2,$0,$L700
	nop

	lw	$2,24($sp)
	lw	$3,4($2)
	li	$2,260			# 0x104
	bne	$3,$2,$L700
	nop

	lui	$2,%hi(gpPhyData)
	lw	$3,%lo(gpPhyData)($2)
	lhu	$2,60($3)
	ins	$2,$0,8,1
	sh	$2,60($3)
	li	$4,1			# 0x1
	lui	$2,%hi(xmcs_set_connection_start)
	addiu	$2,$2,%lo(xmcs_set_connection_start)
	jalr	$2
	nop

	j	$L701
	nop

$L700:
	lui	$2,%hi(xpon_mac_print_open)
	lw	$2,%lo(xpon_mac_print_open)($2)
	beq	$2,$0,$L701
	nop

	lw	$2,24($sp)
	lw	$3,4($2)
	lui	$2,%hi($LC244)
	addiu	$4,$2,%lo($LC244)
	lui	$2,%hi(__func__.34057)
	addiu	$5,$2,%lo(__func__.34057)
	li	$6,1737			# 0x6c9
	move	$7,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L701:
	move	$2,$0
	j	$L702
	nop

$L699:
	lui	$2,%hi(xpon_mac_print_open)
	lw	$2,%lo(xpon_mac_print_open)($2)
	beq	$2,$0,$L703
	nop

	lw	$2,24($sp)
	lw	$3,4($2)
	lui	$2,%hi($LC245)
	addiu	$4,$2,%lo($LC245)
	lui	$2,%hi(__func__.34057)
	addiu	$5,$2,%lo(__func__.34057)
	li	$6,1741			# 0x6cd
	move	$7,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L703:
	lw	$2,24($sp)
	lw	$2,4($2)
	li	$3,6			# 0x6
	beq	$2,$3,$L705
	nop

	sltu	$3,$2,7
	beq	$3,$0,$L706
	nop

	li	$3,2			# 0x2
	beq	$2,$3,$L721
	nop

	sltu	$3,$2,3
	beq	$3,$0,$L708
	nop

	beq	$2,$0,$L705
	nop

	li	$3,1			# 0x1
	beq	$2,$3,$L722
	nop

	j	$L704
	nop

$L708:
	li	$3,4			# 0x4
	beq	$2,$3,$L723
	nop

	sltu	$2,$2,5
	beq	$2,$0,$L711
	nop

	j	$L720
	nop

$L706:
	li	$3,258			# 0x102
	beq	$2,$3,$L713
	nop

	sltu	$3,$2,259
	beq	$3,$0,$L714
	nop

	li	$3,8			# 0x8
	beq	$2,$3,$L705
	nop

	sltu	$3,$2,8
	bne	$3,$0,$L724
	nop

	li	$3,257			# 0x101
	beq	$2,$3,$L716
	nop

	j	$L704
	nop

$L714:
	li	$3,261			# 0x105
	beq	$2,$3,$L717
	nop

	li	$3,262			# 0x106
	beq	$2,$3,$L718
	nop

	li	$3,259			# 0x103
	beq	$2,$3,$L719
	nop

	j	$L704
	nop

$L705:
	lw	$2,24($sp)
	lw	$3,0($2)
	lw	$2,24($sp)
	lw	$2,4($2)
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(xpon_phy_los_illegal_handler)
	addiu	$2,$2,%lo(xpon_phy_los_illegal_handler)
	jalr	$2
	nop

	j	$L720
	nop

$L711:
	lw	$2,24($sp)
	lw	$2,0($2)
	move	$4,$2
	lui	$2,%hi(xpon_phy_ready_handler)
	addiu	$2,$2,%lo(xpon_phy_ready_handler)
	jalr	$2
	nop

	j	$L720
	nop

$L716:
	lui	$2,%hi(xpon_phy_start_rouge_handle)
	addiu	$2,$2,%lo(xpon_phy_start_rouge_handle)
	jalr	$2
	nop

	j	$L720
	nop

$L713:
	lui	$2,%hi(xpon_phy_stop_rouge_handle)
	addiu	$2,$2,%lo(xpon_phy_stop_rouge_handle)
	jalr	$2
	nop

	j	$L720
	nop

$L719:
	lui	$2,%hi(gpPhyData)
	lw	$3,%lo(gpPhyData)($2)
	lhu	$2,60($3)
	li	$4,1			# 0x1
	ins	$2,$4,8,1
	sh	$2,60($3)
	move	$4,$0
	lui	$2,%hi(xmcs_set_connection_start)
	addiu	$2,$2,%lo(xmcs_set_connection_start)
	jalr	$2
	nop

	j	$L720
	nop

$L717:
	move	$4,$0
	lui	$2,%hi(xpon_tx_power_handle)
	addiu	$2,$2,%lo(xpon_tx_power_handle)
	jalr	$2
	nop

	j	$L720
	nop

$L718:
	li	$4,1			# 0x1
	lui	$2,%hi(xpon_tx_power_handle)
	addiu	$2,$2,%lo(xpon_tx_power_handle)
	jalr	$2
	nop

	j	$L720
	nop

$L704:
	lw	$2,24($sp)
	lw	$3,4($2)
	lui	$2,%hi($LC246)
	addiu	$4,$2,%lo($LC246)
	move	$5,$3
	lui	$2,%hi(__func__.34057)
	addiu	$6,$2,%lo(__func__.34057)
	li	$7,1791			# 0x6ff
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	j	$L720
	nop

$L721:
	nop
	j	$L720
	nop

$L722:
	nop
	j	$L720
	nop

$L723:
	nop
	j	$L720
	nop

$L724:
	nop
$L720:
	move	$2,$0
$L702:
	lw	$31,20($sp)
	addiu	$sp,$sp,24
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	xpon_phy_event_dispatch
	.size	xpon_phy_event_dispatch, .-xpon_phy_event_dispatch
	.rdata
	.align	2
$LC247:
	.ascii	"XPON MAC Driver not ready to receive event from module w"
	.ascii	"ith id:%d,\000"
	.section	.text.xpon_mac_hook_dispatch,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	xpon_mac_hook_dispatch
	.type	xpon_mac_hook_dispatch, @function
xpon_mac_hook_dispatch:
	.frame	$sp,112,$31		# vars= 88, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-112
	sw	$31,108($sp)
	sw	$4,112($sp)
	lw	$2,112($sp)
	sw	$2,16($sp)
	lw	$2,16($sp)
	lw	$2,0($2)
	beq	$2,$0,$L727
	nop

	li	$3,1			# 0x1
	beq	$2,$3,$L728
	nop

	j	$L732
	nop

$L727:
	lw	$2,16($sp)
	lw	$2,4($2)
	move	$4,$2
	lui	$2,%hi(xpon_phy_event_dispatch)
	addiu	$2,$2,%lo(xpon_phy_event_dispatch)
	jalr	$2
	nop

	j	$L729
	nop

$L728:
	addiu	$2,$sp,80
	move	$4,$2
	move	$5,$0
	li	$6,21			# 0x15
	lui	$2,%hi(memset)
	addiu	$2,$2,%lo(memset)
	jalr	$2
	nop

	addiu	$3,$sp,20
	li	$2,1074003968			# 0x40040000
	ori	$4,$2,0xd910
	move	$5,$3
	lui	$2,%hi(gpon_cmd_proc)
	addiu	$2,$2,%lo(gpon_cmd_proc)
	jalr	$2
	nop

	addiu	$2,$sp,80
	addiu	$3,$2,8
	addiu	$2,$sp,20
	addiu	$2,$2,12
	move	$4,$3
	move	$5,$2
	li	$6,10			# 0xa
	lui	$2,%hi(memcpy)
	addiu	$2,$2,%lo(memcpy)
	jalr	$2
	nop

	lw	$2,16($sp)
	addiu	$2,$2,4
	addiu	$3,$sp,80
	move	$4,$3
	move	$5,$2
	li	$6,8			# 0x8
	lui	$2,%hi(memcpy)
	addiu	$2,$2,%lo(memcpy)
	jalr	$2
	nop

	addiu	$3,$sp,80
	li	$2,-2147221504			# 0xffffffff80040000
	ori	$4,$2,0xd901
	move	$5,$3
	lui	$2,%hi(gpon_cmd_proc)
	addiu	$2,$2,%lo(gpon_cmd_proc)
	jalr	$2
	nop

	j	$L729
	nop

$L732:
	lw	$2,16($sp)
	lw	$3,0($2)
	lui	$2,%hi($LC247)
	addiu	$4,$2,%lo($LC247)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	li	$2,-1			# 0xffffffffffffffff
	j	$L731
	nop

$L729:
	li	$2,2			# 0x2
$L731:
	lw	$31,108($sp)
	addiu	$sp,$sp,112
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	xpon_mac_hook_dispatch
	.size	xpon_mac_hook_dispatch, .-xpon_mac_hook_dispatch
	.rdata
	.align	2
$LC248:
	.ascii	"max_dscv_gate\000"
	.align	2
$LC249:
	.ascii	"\012create max_dscv_gate proc fail.\000"
	.section	.text.max_dscv_gate_proc_init,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	max_dscv_gate_proc_init
	.type	max_dscv_gate_proc_init, @function
max_dscv_gate_proc_init:
	.frame	$sp,32,$31		# vars= 8, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-32
	sw	$31,28($sp)
	lui	$2,%hi(xpon_proc_dir)
	lw	$2,%lo(xpon_proc_dir)($2)
	bne	$2,$0,$L734
	nop

	li	$2,-1			# 0xffffffffffffffff
	j	$L735
	nop

$L734:
	lui	$2,%hi(xpon_proc_dir)
	lw	$3,%lo(xpon_proc_dir)($2)
	lui	$2,%hi($LC248)
	addiu	$4,$2,%lo($LC248)
	move	$5,$0
	move	$6,$3
	lui	$2,%hi(create_proc_entry)
	addiu	$2,$2,%lo(create_proc_entry)
	jalr	$2
	nop

	sw	$2,16($sp)
	lw	$2,16($sp)
	bne	$2,$0,$L736
	nop

	lui	$2,%hi($LC249)
	addiu	$4,$2,%lo($LC249)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	li	$2,-1			# 0xffffffffffffffff
	j	$L735
	nop

$L736:
	lw	$2,16($sp)
	lui	$3,%hi(max_dscv_gate_read_proc)
	addiu	$3,$3,%lo(max_dscv_gate_read_proc)
	sw	$3,64($2)
	lw	$2,16($sp)
	lui	$3,%hi(max_dscv_gate_write_proc)
	addiu	$3,$3,%lo(max_dscv_gate_write_proc)
	sw	$3,68($2)
	move	$2,$0
$L735:
	lw	$31,28($sp)
	addiu	$sp,$sp,32
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	max_dscv_gate_proc_init
	.size	max_dscv_gate_proc_init, .-max_dscv_gate_proc_init
	.section	.text.xpon_rogue_mode_read_proc,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	xpon_rogue_mode_read_proc
	.type	xpon_rogue_mode_read_proc, @function
xpon_rogue_mode_read_proc:
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
	sw	$7,44($sp)
	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	lw	$3,-30416($2)
	lw	$4,32($sp)
	lui	$2,%hi($LC2)
	addiu	$5,$2,%lo($LC2)
	move	$6,$3
	lui	$2,%hi(sprintf)
	addiu	$2,$2,%lo(sprintf)
	jalr	$2
	nop

	sw	$2,16($sp)
	lw	$3,16($sp)
	lw	$2,40($sp)
	subu	$2,$3,$2
	sw	$2,16($sp)
	lw	$2,40($sp)
	lw	$3,32($sp)
	addu	$3,$3,$2
	lw	$2,36($sp)
	sw	$3,0($2)
	lw	$3,16($sp)
	lw	$2,44($sp)
	slt	$2,$2,$3
	beq	$2,$0,$L738
	nop

	lw	$2,44($sp)
	sw	$2,16($sp)
	j	$L739
	nop

$L738:
	lw	$2,48($sp)
	li	$3,1			# 0x1
	sw	$3,0($2)
$L739:
	lw	$2,16($sp)
	bgez	$2,$L740
	nop

	sw	$0,16($sp)
$L740:
	lw	$2,16($sp)
	lw	$31,28($sp)
	addiu	$sp,$sp,32
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	xpon_rogue_mode_read_proc
	.size	xpon_rogue_mode_read_proc, .-xpon_rogue_mode_read_proc
	.section	.text.xpon_fe_reset_happened_read_proc,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	xpon_fe_reset_happened_read_proc
	.type	xpon_fe_reset_happened_read_proc, @function
xpon_fe_reset_happened_read_proc:
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
	sw	$7,44($sp)
	sw	$0,16($sp)
	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	lw	$2,-30412($2)
	ext	$2,$2,31,1
	andi	$2,$2,0x00ff
	move	$3,$2
	lw	$4,32($sp)
	lui	$2,%hi($LC2)
	addiu	$5,$2,%lo($LC2)
	move	$6,$3
	lui	$2,%hi(sprintf)
	addiu	$2,$2,%lo(sprintf)
	jalr	$2
	nop

	sw	$2,16($sp)
	lw	$3,16($sp)
	lw	$2,40($sp)
	subu	$2,$3,$2
	sw	$2,16($sp)
	lw	$2,40($sp)
	lw	$3,32($sp)
	addu	$3,$3,$2
	lw	$2,36($sp)
	sw	$3,0($2)
	lw	$3,16($sp)
	lw	$2,44($sp)
	slt	$2,$2,$3
	beq	$2,$0,$L743
	nop

	lw	$2,44($sp)
	sw	$2,16($sp)
	j	$L744
	nop

$L743:
	lw	$2,48($sp)
	li	$3,1			# 0x1
	sw	$3,0($2)
$L744:
	lw	$2,16($sp)
	bgez	$2,$L745
	nop

	sw	$0,16($sp)
$L745:
	lw	$2,16($sp)
	lw	$31,28($sp)
	addiu	$sp,$sp,32
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	xpon_fe_reset_happened_read_proc
	.size	xpon_fe_reset_happened_read_proc, .-xpon_fe_reset_happened_read_proc
	.section	.text.xpon_fe_reset_happened_write_proc,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	xpon_fe_reset_happened_write_proc
	.type	xpon_fe_reset_happened_write_proc, @function
xpon_fe_reset_happened_write_proc:
	.frame	$sp,64,$31		# vars= 40, regs= 2/0, args= 16, gp= 0
	.mask	0x80010000,-4
	.fmask	0x00000000,0
	addiu	$sp,$sp,-64
	sw	$31,60($sp)
	sw	$16,56($sp)
	sw	$4,64($sp)
	sw	$5,68($sp)
	sw	$6,72($sp)
	sw	$7,76($sp)
	sw	$0,48($sp)
	sw	$0,52($sp)
	sw	$0,20($sp)
	lw	$2,72($sp)
	sltu	$2,$2,8
	bne	$2,$0,$L748
	li	$2,-22			# 0xffffffffffffffea
	j	$L752
$L748:
	addiu	$2,$sp,48
	sw	$2,24($sp)
	lw	$2,68($sp)
	sw	$2,28($sp)
	lw	$2,72($sp)
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
	beq	$2,$0,$L750
	lw	$4,24($sp)
	lw	$5,28($sp)
	lw	$6,16($sp)
#APP
 # 1911 "/opt/tclinux_phoenix/modules/private/xpon/src/xpondrv.c" 1
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
$L750:
	lw	$2,16($sp)
	beq	$2,$0,$L751
	li	$2,-14			# 0xfffffffffffffff2
	j	$L752
$L751:
	lw	$2,72($sp)
	addiu	$3,$sp,16
	addu	$2,$3,$2
	sb	$0,32($2)
	lui	$2,%hi(gpPonSysData)
	lw	$16,%lo(gpPonSysData)($2)
	addiu	$2,$sp,48
	move	$4,$2
	move	$5,$0
	li	$6,10			# 0xa
	lui	$2,%hi(simple_strtoul)
	addiu	$2,$2,%lo(simple_strtoul)
	jalr	$2
	andi	$2,$2,0x00ff
	andi	$2,$2,0x1
	andi	$4,$2,0x00ff
	li	$2,65536			# 0x10000
	addu	$3,$16,$2
	lbu	$2,-30412($3)
	ins	$2,$4,7,1
	sb	$2,-30412($3)
	lw	$2,72($sp)
$L752:
	lw	$31,60($sp)
	lw	$16,56($sp)
	addiu	$sp,$sp,64
	j	$31
	.end	xpon_fe_reset_happened_write_proc
	.size	xpon_fe_reset_happened_write_proc, .-xpon_fe_reset_happened_write_proc
	.rdata
	.align	2
$LC250:
	.ascii	"stage_chk_cnt\000"
	.align	2
$LC251:
	.ascii	"power_management\000"
	.align	2
$LC252:
	.ascii	"xpon\000"
	.align	2
$LC253:
	.ascii	"ponInfo\000"
	.align	2
$LC254:
	.ascii	"omci_oam_monitor\000"
	.align	2
$LC255:
	.ascii	"los_led\000"
	.align	2
$LC256:
	.ascii	"rogue_mode\000"
	.align	2
$LC257:
	.ascii	"fe_reset_happened\000"
	.section	.text.xpon_proc_init,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	xpon_proc_init
	.type	xpon_proc_init, @function
xpon_proc_init:
	.frame	$sp,32,$31		# vars= 8, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-32
	sw	$31,28($sp)
	sw	$0,16($sp)
	lui	$2,%hi(gpon_proc_dir)
	lw	$2,%lo(gpon_proc_dir)($2)
	bne	$2,$0,$L754
	nop

	lui	$2,%hi(xpon_proc_dir)
	lw	$2,%lo(xpon_proc_dir)($2)
	beq	$2,$0,$L755
	nop

$L754:
	move	$2,$0
	j	$L756
	nop

$L755:
	lui	$2,%hi($LC76)
	addiu	$4,$2,%lo($LC76)
	move	$5,$0
	lui	$2,%hi(proc_mkdir)
	addiu	$2,$2,%lo(proc_mkdir)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(gpon_proc_dir)
	sw	$3,%lo(gpon_proc_dir)($2)
	lui	$2,%hi(gpon_proc_dir)
	lw	$2,%lo(gpon_proc_dir)($2)
	bne	$2,$0,$L757
	nop

	li	$2,-1			# 0xffffffffffffffff
	j	$L756
	nop

$L757:
	lui	$2,%hi(gpon_proc_dir)
	lw	$3,%lo(gpon_proc_dir)($2)
	lui	$2,%hi($LC49)
	addiu	$4,$2,%lo($LC49)
	move	$5,$0
	move	$6,$3
	lui	$2,%hi(create_proc_entry)
	addiu	$2,$2,%lo(create_proc_entry)
	jalr	$2
	nop

	sw	$2,16($sp)
	lw	$2,16($sp)
	bne	$2,$0,$L758
	nop

	li	$2,-1			# 0xffffffffffffffff
	j	$L756
	nop

$L758:
	lw	$2,16($sp)
	lui	$3,%hi(gpon_debug_read_proc)
	addiu	$3,$3,%lo(gpon_debug_read_proc)
	sw	$3,64($2)
	lw	$2,16($sp)
	lui	$3,%hi(gpon_debug_write_proc)
	addiu	$3,$3,%lo(gpon_debug_write_proc)
	sw	$3,68($2)
	lui	$2,%hi(gpon_proc_dir)
	lw	$3,%lo(gpon_proc_dir)($2)
	lui	$2,%hi($LC250)
	addiu	$4,$2,%lo($LC250)
	move	$5,$0
	move	$6,$3
	lui	$2,%hi(create_proc_entry)
	addiu	$2,$2,%lo(create_proc_entry)
	jalr	$2
	nop

	sw	$2,16($sp)
	lw	$2,16($sp)
	bne	$2,$0,$L759
	nop

	li	$2,-1			# 0xffffffffffffffff
	j	$L756
	nop

$L759:
	lw	$2,16($sp)
	lui	$3,%hi(gpon_stage_change_chk_read_proc)
	addiu	$3,$3,%lo(gpon_stage_change_chk_read_proc)
	sw	$3,64($2)
	lw	$2,16($sp)
	lui	$3,%hi(gpon_stage_change_chk_write_proc)
	addiu	$3,$3,%lo(gpon_stage_change_chk_write_proc)
	sw	$3,68($2)
	lui	$2,%hi(gpon_proc_dir)
	lw	$3,%lo(gpon_proc_dir)($2)
	lui	$2,%hi($LC251)
	addiu	$4,$2,%lo($LC251)
	move	$5,$0
	move	$6,$3
	lui	$2,%hi(create_proc_entry)
	addiu	$2,$2,%lo(create_proc_entry)
	jalr	$2
	nop

	sw	$2,16($sp)
	lw	$2,16($sp)
	bne	$2,$0,$L760
	nop

	li	$2,-1			# 0xffffffffffffffff
	j	$L756
	nop

$L760:
	lw	$2,16($sp)
	lui	$3,%hi(gpon_power_management_read_proc)
	addiu	$3,$3,%lo(gpon_power_management_read_proc)
	sw	$3,64($2)
	lw	$2,16($sp)
	lui	$3,%hi(gpon_power_management_write_proc)
	addiu	$3,$3,%lo(gpon_power_management_write_proc)
	sw	$3,68($2)
	lui	$2,%hi($LC252)
	addiu	$4,$2,%lo($LC252)
	move	$5,$0
	lui	$2,%hi(proc_mkdir)
	addiu	$2,$2,%lo(proc_mkdir)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(xpon_proc_dir)
	sw	$3,%lo(xpon_proc_dir)($2)
	lui	$2,%hi(xpon_proc_dir)
	lw	$2,%lo(xpon_proc_dir)($2)
	bne	$2,$0,$L761
	nop

	li	$2,-1			# 0xffffffffffffffff
	j	$L756
	nop

$L761:
	lui	$2,%hi(xpon_proc_dir)
	lw	$3,%lo(xpon_proc_dir)($2)
	lui	$2,%hi($LC253)
	addiu	$4,$2,%lo($LC253)
	move	$5,$0
	move	$6,$3
	lui	$2,%hi(create_proc_entry)
	addiu	$2,$2,%lo(create_proc_entry)
	jalr	$2
	nop

	sw	$2,16($sp)
	lw	$2,16($sp)
	bne	$2,$0,$L762
	nop

	li	$2,-1			# 0xffffffffffffffff
	j	$L756
	nop

$L762:
	lw	$2,16($sp)
	lui	$3,%hi(xpon_info_read_proc)
	addiu	$3,$3,%lo(xpon_info_read_proc)
	sw	$3,64($2)
	lui	$2,%hi(xpon_proc_dir)
	lw	$3,%lo(xpon_proc_dir)($2)
	lui	$2,%hi($LC254)
	addiu	$4,$2,%lo($LC254)
	move	$5,$0
	move	$6,$3
	lui	$2,%hi(create_proc_entry)
	addiu	$2,$2,%lo(create_proc_entry)
	jalr	$2
	nop

	sw	$2,16($sp)
	lw	$2,16($sp)
	bne	$2,$0,$L763
	nop

	li	$2,-1			# 0xffffffffffffffff
	j	$L756
	nop

$L763:
	lw	$2,16($sp)
	lui	$3,%hi(xpon_omai_oam_monitor_read_proc)
	addiu	$3,$3,%lo(xpon_omai_oam_monitor_read_proc)
	sw	$3,64($2)
	lw	$2,16($sp)
	lui	$3,%hi(xpon_omci_oam_monitor_write_proc)
	addiu	$3,$3,%lo(xpon_omci_oam_monitor_write_proc)
	sw	$3,68($2)
	lui	$2,%hi(xpon_proc_dir)
	lw	$3,%lo(xpon_proc_dir)($2)
	lui	$2,%hi($LC255)
	addiu	$4,$2,%lo($LC255)
	move	$5,$0
	move	$6,$3
	lui	$2,%hi(create_proc_entry)
	addiu	$2,$2,%lo(create_proc_entry)
	jalr	$2
	nop

	sw	$2,16($sp)
	lw	$2,16($sp)
	bne	$2,$0,$L764
	nop

	li	$2,-1			# 0xffffffffffffffff
	j	$L756
	nop

$L764:
	lw	$2,16($sp)
	lui	$3,%hi(xpon_los_led_read_proc)
	addiu	$3,$3,%lo(xpon_los_led_read_proc)
	sw	$3,64($2)
	lw	$2,16($sp)
	lui	$3,%hi(xpon_los_led_write_proc)
	addiu	$3,$3,%lo(xpon_los_led_write_proc)
	sw	$3,68($2)
	lui	$2,%hi(xpon_proc_dir)
	lw	$3,%lo(xpon_proc_dir)($2)
	lui	$2,%hi($LC256)
	addiu	$4,$2,%lo($LC256)
	move	$5,$0
	move	$6,$3
	lui	$2,%hi(create_proc_entry)
	addiu	$2,$2,%lo(create_proc_entry)
	jalr	$2
	nop

	sw	$2,16($sp)
	lw	$2,16($sp)
	bne	$2,$0,$L765
	nop

	li	$2,-1			# 0xffffffffffffffff
	j	$L756
	nop

$L765:
	lw	$2,16($sp)
	lui	$3,%hi(xpon_rogue_mode_read_proc)
	addiu	$3,$3,%lo(xpon_rogue_mode_read_proc)
	sw	$3,64($2)
	lw	$2,16($sp)
	sw	$0,68($2)
	lui	$2,%hi(xpon_proc_dir)
	lw	$3,%lo(xpon_proc_dir)($2)
	lui	$2,%hi($LC257)
	addiu	$4,$2,%lo($LC257)
	move	$5,$0
	move	$6,$3
	lui	$2,%hi(create_proc_entry)
	addiu	$2,$2,%lo(create_proc_entry)
	jalr	$2
	nop

	sw	$2,16($sp)
	lw	$2,16($sp)
	bne	$2,$0,$L766
	nop

	li	$2,-1			# 0xffffffffffffffff
	j	$L756
	nop

$L766:
	lw	$2,16($sp)
	lui	$3,%hi(xpon_fe_reset_happened_read_proc)
	addiu	$3,$3,%lo(xpon_fe_reset_happened_read_proc)
	sw	$3,64($2)
	lw	$2,16($sp)
	lui	$3,%hi(xpon_fe_reset_happened_write_proc)
	addiu	$3,$3,%lo(xpon_fe_reset_happened_write_proc)
	sw	$3,68($2)
	lui	$2,%hi($LC25)
	addiu	$4,$2,%lo($LC25)
	move	$5,$0
	lui	$2,%hi(proc_mkdir)
	addiu	$2,$2,%lo(proc_mkdir)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(epon_proc_dir)
	sw	$3,%lo(epon_proc_dir)($2)
	lui	$2,%hi(epon_proc_dir)
	lw	$2,%lo(epon_proc_dir)($2)
	bne	$2,$0,$L767
	nop

	li	$2,-1			# 0xffffffffffffffff
	j	$L756
	nop

$L767:
	lui	$2,%hi(epon_proc_dir)
	lw	$3,%lo(epon_proc_dir)($2)
	lui	$2,%hi($LC248)
	addiu	$4,$2,%lo($LC248)
	move	$5,$0
	move	$6,$3
	lui	$2,%hi(create_proc_entry)
	addiu	$2,$2,%lo(create_proc_entry)
	jalr	$2
	nop

	sw	$2,16($sp)
	lw	$2,16($sp)
	bne	$2,$0,$L768
	nop

	li	$2,-1			# 0xffffffffffffffff
	j	$L756
	nop

$L768:
	lw	$2,16($sp)
	lui	$3,%hi(max_dscv_gate_read_proc)
	addiu	$3,$3,%lo(max_dscv_gate_read_proc)
	sw	$3,64($2)
	lw	$2,16($sp)
	lui	$3,%hi(max_dscv_gate_write_proc)
	addiu	$3,$3,%lo(max_dscv_gate_write_proc)
	sw	$3,68($2)
	move	$2,$0
$L756:
	lw	$31,28($sp)
	addiu	$sp,$sp,32
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	xpon_proc_init
	.size	xpon_proc_init, .-xpon_proc_init
	.globl	xpondrv_hook_dispatch_ops
	.rdata
	.align	2
$LC258:
	.ascii	"xpon_mac_hook_dispatch\000"
	.data
	.align	2
	.type	xpondrv_hook_dispatch_ops, @object
	.size	xpondrv_hook_dispatch_ops, 40
xpondrv_hook_dispatch_ops:
	.space	16
	.word	$LC258
	.word	1
	.word	xpon_mac_hook_dispatch
	.word	24
	.word	0
	.space	4
	.section	.text.xpondrv_cleanup,"ax",@progbits
	.align	2
	.globl	xpondrv_cleanup
	.set	nomips16
	.set	nomicromips
	.ent	xpondrv_cleanup
	.type	xpondrv_cleanup, @function
xpondrv_cleanup:
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
	beq	$2,$0,$L770
	nop

	lui	$2,%hi(gpPhyData)
	lw	$2,%lo(gpPhyData)($2)
	addiu	$2,$2,4
	move	$4,$2
	lui	$2,%hi(del_timer)
	addiu	$2,$2,%lo(del_timer)
	jalr	$2
	nop

	j	$L771
	nop

$L770:
	lui	$2,%hi(gpPhyData)
	lw	$2,%lo(gpPhyData)($2)
	addiu	$2,$2,4
	move	$4,$2
	lui	$2,%hi(del_timer_sync)
	addiu	$2,$2,%lo(del_timer_sync)
	jalr	$2
	nop

$L771:
	move	$2,$28
	lw	$2,20($2)
	move	$3,$2
	li	$2,134152192			# 0x7ff0000
	ori	$2,$2,0xff00
	and	$2,$3,$2
	beq	$2,$0,$L772
	nop

	lui	$2,%hi(gpPhyData)
	lw	$2,%lo(gpPhyData)($2)
	addiu	$2,$2,32
	move	$4,$2
	lui	$2,%hi(del_timer)
	addiu	$2,$2,%lo(del_timer)
	jalr	$2
	nop

	j	$L773
	nop

$L772:
	lui	$2,%hi(gpPhyData)
	lw	$2,%lo(gpPhyData)($2)
	addiu	$2,$2,32
	move	$4,$2
	lui	$2,%hi(del_timer_sync)
	addiu	$2,$2,%lo(del_timer_sync)
	jalr	$2
	nop

$L773:
	lui	$2,%hi(gpPonSysData)
	lw	$2,%lo(gpPonSysData)($2)
	li	$3,1			# 0x1
	sw	$3,8($2)
	move	$2,$28
	lw	$2,20($2)
	move	$3,$2
	li	$2,134152192			# 0x7ff0000
	ori	$2,$2,0xff00
	and	$2,$3,$2
	beq	$2,$0,$L774
	nop

	lui	$2,%hi(gpPhyData)
	lw	$2,%lo(gpPhyData)($2)
	addiu	$2,$2,64
	move	$4,$2
	lui	$2,%hi(del_timer)
	addiu	$2,$2,%lo(del_timer)
	jalr	$2
	nop

	j	$L775
	nop

$L774:
	lui	$2,%hi(gpPhyData)
	lw	$2,%lo(gpPhyData)($2)
	addiu	$2,$2,64
	move	$4,$2
	lui	$2,%hi(del_timer_sync)
	addiu	$2,$2,%lo(del_timer_sync)
	jalr	$2
	nop

$L775:
	lui	$2,%hi(xpon_daemon_quit)
	addiu	$2,$2,%lo(xpon_daemon_quit)
	jalr	$2
	nop

	lui	$2,%hi(gpon_deinit)
	addiu	$2,$2,%lo(gpon_deinit)
	jalr	$2
	nop

	lui	$2,%hi(eponExit)
	addiu	$2,$2,%lo(eponExit)
	jalr	$2
	nop

	lui	$2,%hi(pon_mci_destroy)
	addiu	$2,$2,%lo(pon_mci_destroy)
	jalr	$2
	nop

	lui	$2,%hi(pwan_destroy)
	addiu	$2,$2,%lo(pwan_destroy)
	jalr	$2
	nop

	li	$4,19			# 0x13
	move	$5,$0
	lui	$2,%hi(free_irq)
	addiu	$2,$2,%lo(free_irq)
	jalr	$2
	nop

	lui	$2,%hi(stop_omci_oam_monitor)
	addiu	$2,$2,%lo(stop_omci_oam_monitor)
	jalr	$2
	nop

	lui	$2,%hi(gpPonSysData)
	lw	$2,%lo(gpPonSysData)($2)
	beq	$2,$0,$L776
	nop

	lui	$2,%hi(gpPonSysData)
	lw	$2,%lo(gpPonSysData)($2)
	move	$4,$2
	lui	$2,%hi(kfree)
	addiu	$2,$2,%lo(kfree)
	jalr	$2
	nop

	lui	$2,%hi(gpPonSysData)
	sw	$0,%lo(gpPonSysData)($2)
$L776:
	lui	$2,%hi(gpon_proc_dir)
	lw	$2,%lo(gpon_proc_dir)($2)
	beq	$2,$0,$L777
	nop

	lui	$2,%hi(gpon_proc_dir)
	lw	$3,%lo(gpon_proc_dir)($2)
	lui	$2,%hi($LC251)
	addiu	$4,$2,%lo($LC251)
	move	$5,$3
	lui	$2,%hi(remove_proc_entry)
	addiu	$2,$2,%lo(remove_proc_entry)
	jalr	$2
	nop

	lui	$2,%hi(gpon_proc_dir)
	lw	$3,%lo(gpon_proc_dir)($2)
	lui	$2,%hi($LC49)
	addiu	$4,$2,%lo($LC49)
	move	$5,$3
	lui	$2,%hi(remove_proc_entry)
	addiu	$2,$2,%lo(remove_proc_entry)
	jalr	$2
	nop

	lui	$2,%hi(gpon_proc_dir)
	lw	$3,%lo(gpon_proc_dir)($2)
	lui	$2,%hi($LC250)
	addiu	$4,$2,%lo($LC250)
	move	$5,$3
	lui	$2,%hi(remove_proc_entry)
	addiu	$2,$2,%lo(remove_proc_entry)
	jalr	$2
	nop

$L777:
	lui	$2,%hi($LC76)
	addiu	$4,$2,%lo($LC76)
	move	$5,$0
	lui	$2,%hi(remove_proc_entry)
	addiu	$2,$2,%lo(remove_proc_entry)
	jalr	$2
	nop

	lui	$2,%hi(xpon_proc_dir)
	lw	$2,%lo(xpon_proc_dir)($2)
	beq	$2,$0,$L778
	nop

	lui	$2,%hi(xpon_proc_dir)
	lw	$3,%lo(xpon_proc_dir)($2)
	lui	$2,%hi($LC253)
	addiu	$4,$2,%lo($LC253)
	move	$5,$3
	lui	$2,%hi(remove_proc_entry)
	addiu	$2,$2,%lo(remove_proc_entry)
	jalr	$2
	nop

	lui	$2,%hi(xpon_proc_dir)
	lw	$3,%lo(xpon_proc_dir)($2)
	lui	$2,%hi($LC257)
	addiu	$4,$2,%lo($LC257)
	move	$5,$3
	lui	$2,%hi(remove_proc_entry)
	addiu	$2,$2,%lo(remove_proc_entry)
	jalr	$2
	nop

	lui	$2,%hi(xpon_proc_dir)
	lw	$3,%lo(xpon_proc_dir)($2)
	lui	$2,%hi($LC255)
	addiu	$4,$2,%lo($LC255)
	move	$5,$3
	lui	$2,%hi(remove_proc_entry)
	addiu	$2,$2,%lo(remove_proc_entry)
	jalr	$2
	nop

	lui	$2,%hi(xpon_proc_dir)
	lw	$3,%lo(xpon_proc_dir)($2)
	lui	$2,%hi($LC256)
	addiu	$4,$2,%lo($LC256)
	move	$5,$3
	lui	$2,%hi(remove_proc_entry)
	addiu	$2,$2,%lo(remove_proc_entry)
	jalr	$2
	nop

	lui	$2,%hi(xpon_proc_dir)
	lw	$3,%lo(xpon_proc_dir)($2)
	lui	$2,%hi($LC254)
	addiu	$4,$2,%lo($LC254)
	move	$5,$3
	lui	$2,%hi(remove_proc_entry)
	addiu	$2,$2,%lo(remove_proc_entry)
	jalr	$2
	nop

$L778:
	lui	$2,%hi($LC252)
	addiu	$4,$2,%lo($LC252)
	move	$5,$0
	lui	$2,%hi(remove_proc_entry)
	addiu	$2,$2,%lo(remove_proc_entry)
	jalr	$2
	nop

	lui	$2,%hi(epon_proc_dir)
	lw	$2,%lo(epon_proc_dir)($2)
	beq	$2,$0,$L779
	nop

	lui	$2,%hi(epon_proc_dir)
	lw	$3,%lo(epon_proc_dir)($2)
	lui	$2,%hi($LC248)
	addiu	$4,$2,%lo($LC248)
	move	$5,$3
	lui	$2,%hi(remove_proc_entry)
	addiu	$2,$2,%lo(remove_proc_entry)
	jalr	$2
	nop

$L779:
	lui	$2,%hi($LC25)
	addiu	$4,$2,%lo($LC25)
	move	$5,$0
	lui	$2,%hi(remove_proc_entry)
	addiu	$2,$2,%lo(remove_proc_entry)
	jalr	$2
	nop

	lui	$2,%hi(epon_stat_prev)
	lw	$2,%lo(epon_stat_prev)($2)
	beq	$2,$0,$L780
	nop

	lui	$2,%hi(epon_stat_prev)
	lw	$2,%lo(epon_stat_prev)($2)
	move	$4,$2
	lui	$2,%hi(kfree)
	addiu	$2,$2,%lo(kfree)
	jalr	$2
	nop

$L780:
	lui	$2,%hi(xpondrv_hook_dispatch_ops)
	addiu	$4,$2,%lo(xpondrv_hook_dispatch_ops)
	lui	$2,%hi(ecnt_unregister_hook)
	addiu	$2,$2,%lo(ecnt_unregister_hook)
	jalr	$2
	nop

	lw	$31,20($sp)
	addiu	$sp,$sp,24
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	xpondrv_cleanup
	.size	xpondrv_cleanup, .-xpondrv_cleanup
	.globl	cleanup_module
	cleanup_module = xpondrv_cleanup
	.section	.text.xpon_check_emergence_state,"ax",@progbits
	.align	2
	.globl	xpon_check_emergence_state
	.set	nomips16
	.set	nomicromips
	.ent	xpon_check_emergence_state
	.type	xpon_check_emergence_state, @function
xpon_check_emergence_state:
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	move	$2,$0
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	xpon_check_emergence_state
	.size	xpon_check_emergence_state, .-xpon_check_emergence_state
	.rdata
	.align	2
$LC259:
	.ascii	"xPON driver initialization\012\000"
	.align	2
$LC260:
	.ascii	"Alloc data struct memory failed\012\000"
	.align	2
$LC261:
	.ascii	"Alloc data struct memory successful, %d\012\000"
	.align	2
$LC262:
	.ascii	"PON WAN/OAM/OMCI interface driver initialization failed\012"
	.ascii	"\000"
	.align	2
$LC263:
	.ascii	"PON monitor and control interface driver initialization "
	.ascii	"failed\012\000"
	.align	2
$LC264:
	.ascii	"GPON initialization failed\012\000"
	.align	2
$LC265:
	.ascii	"QDMA buffer manager initialization failed\012\000"
	.align	2
$LC266:
	.ascii	"xpon dying gasp initialization failed\012\000"
	.align	2
$LC267:
	.ascii	"xpon vlan filter initialization failed\012\000"
	.align	2
$LC268:
	.ascii	"Register hook function failed! %s:%d\000"
	.section	.text.xpondrv_init,"ax",@progbits
	.align	2
	.globl	xpondrv_init
	.set	nomips16
	.set	nomicromips
	.ent	xpondrv_init
	.type	xpondrv_init, @function
xpondrv_init:
	.frame	$sp,32,$31		# vars= 8, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-32
	sw	$31,28($sp)
	lui	$2,%hi($LC259)
	addiu	$4,$2,%lo($LC259)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	li	$2,35344			# 0x8a10
	sw	$2,16($sp)
	li	$2,208			# 0xd0
	sw	$2,20($sp)
$L784 = .
	lw	$4,16($sp)
	lw	$5,20($sp)
	lui	$2,%hi(__kmalloc)
	addiu	$2,$2,%lo(__kmalloc)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(gpPonSysData)
	sw	$3,%lo(gpPonSysData)($2)
	lui	$2,%hi(gpPonSysData)
	lw	$2,%lo(gpPonSysData)($2)
	bne	$2,$0,$L786
	nop

	lui	$2,%hi($LC260)
	addiu	$4,$2,%lo($LC260)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	j	$L787
	nop

$L786:
	lui	$2,%hi(gpPonSysData)
	lw	$2,%lo(gpPonSysData)($2)
	move	$4,$2
	move	$5,$0
	li	$6,35344			# 0x8a10
	lui	$2,%hi(memset)
	addiu	$2,$2,%lo(memset)
	jalr	$2
	nop

	lui	$2,%hi($LC261)
	addiu	$4,$2,%lo($LC261)
	li	$5,35344			# 0x8a10
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi(xpondrv_init_global_data)
	addiu	$2,$2,%lo(xpondrv_init_global_data)
	jalr	$2
	nop

	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,35128			# 0x8938
	addu	$2,$3,$2
	move	$4,$2
	lui	$2,%hi(omci_oam_monitor_init)
	addiu	$2,$2,%lo(omci_oam_monitor_init)
	jalr	$2
	nop

	lui	$2,%hi(pwan_init)
	addiu	$2,$2,%lo(pwan_init)
	jalr	$2
	nop

	beq	$2,$0,$L788
	nop

	lui	$2,%hi($LC262)
	addiu	$4,$2,%lo($LC262)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	j	$L787
	nop

$L788:
	lui	$2,%hi(pon_mci_init)
	addiu	$2,$2,%lo(pon_mci_init)
	jalr	$2
	nop

	beq	$2,$0,$L789
	nop

	lui	$2,%hi($LC263)
	addiu	$4,$2,%lo($LC263)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	j	$L787
	nop

$L789:
	lui	$2,%hi(eponInit)
	addiu	$2,$2,%lo(eponInit)
	jalr	$2
	nop

	beq	$2,$0,$L790
	nop

	lui	$2,%hi($LC264)
	addiu	$4,$2,%lo($LC264)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	j	$L787
	nop

$L790:
	lui	$2,%hi(gpon_init)
	addiu	$2,$2,%lo(gpon_init)
	jalr	$2
	nop

	beq	$2,$0,$L791
	nop

	lui	$2,%hi($LC264)
	addiu	$4,$2,%lo($LC264)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	j	$L787
	nop

$L791:
	lui	$2,%hi(xpondrv_qdma_init)
	addiu	$2,$2,%lo(xpondrv_qdma_init)
	jalr	$2
	nop

	beq	$2,$0,$L792
	nop

	lui	$2,%hi($LC265)
	addiu	$4,$2,%lo($LC265)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	j	$L787
	nop

$L792:
	lui	$2,%hi(xpon_dying_gasp_init)
	addiu	$2,$2,%lo(xpon_dying_gasp_init)
	jalr	$2
	nop

	beq	$2,$0,$L793
	nop

	lui	$2,%hi($LC266)
	addiu	$4,$2,%lo($LC266)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	j	$L787
	nop

$L793:
	lui	$2,%hi(xpon_proc_init)
	addiu	$2,$2,%lo(xpon_proc_init)
	jalr	$2
	nop

	beq	$2,$0,$L794
	nop

	lui	$2,%hi($LC267)
	addiu	$4,$2,%lo($LC267)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	j	$L787
	nop

$L794:
	lui	$2,%hi(xpondrv_hook_dispatch_ops)
	addiu	$4,$2,%lo(xpondrv_hook_dispatch_ops)
	lui	$2,%hi(ecnt_register_hook)
	addiu	$2,$2,%lo(ecnt_register_hook)
	jalr	$2
	nop

	beq	$2,$0,$L795
	nop

	lui	$2,%hi($LC268)
	addiu	$4,$2,%lo($LC268)
	lui	$2,%hi(__func__.34142)
	addiu	$5,$2,%lo(__func__.34142)
	li	$6,2159			# 0x86f
	lui	$2,%hi(panic)
	addiu	$2,$2,%lo(panic)
	jalr	$2
	nop

$L795:
	li	$4,2			# 0x2
	lui	$2,%hi(change_alarm_led_status)
	addiu	$2,$2,%lo(change_alarm_led_status)
	jalr	$2
	nop

	lui	$2,%hi(gpPhyData)
	lw	$2,%lo(gpPhyData)($2)
	addiu	$4,$2,64
	lui	$2,%hi(gpPhyData)
	lw	$2,%lo(gpPhyData)($2)
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

	move	$2,$0
	j	$L796
	nop

$L787:
	lui	$2,%hi(xpondrv_cleanup)
	addiu	$2,$2,%lo(xpondrv_cleanup)
	jalr	$2
	nop

	li	$2,-14			# 0xfffffffffffffff2
$L796:
	lw	$31,28($sp)
	addiu	$sp,$sp,32
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	xpondrv_init
	.size	xpondrv_init, .-xpondrv_init
	.globl	init_module
	init_module = xpondrv_init
	.section	.text.xpon_stop_timer,"ax",@progbits
	.align	2
	.globl	xpon_stop_timer
	.set	nomips16
	.set	nomicromips
	.ent	xpon_stop_timer
	.type	xpon_stop_timer, @function
xpon_stop_timer:
	.frame	$sp,24,$31		# vars= 0, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-24
	sw	$31,20($sp)
	lui	$2,%hi(gpPhyData)
	lw	$2,%lo(gpPhyData)($2)
	lw	$3,60($2)
	li	$2,100663296			# 0x6000000
	and	$3,$3,$2
	li	$2,33554432			# 0x2000000
	bne	$3,$2,$L797
	nop

	lui	$2,%hi(gpon_stop_timer)
	addiu	$2,$2,%lo(gpon_stop_timer)
	jalr	$2
	nop

$L797:
	lw	$31,20($sp)
	addiu	$sp,$sp,24
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	xpon_stop_timer
	.size	xpon_stop_timer, .-xpon_stop_timer
	.section	.modinfo,"a",@progbits
	.align	2
	.type	__mod_license2225, @object
	.size	__mod_license2225, 12
__mod_license2225:
	.ascii	"license=GPL\000"
	.local	__key.33752
	.comm	__key.33752,0,4
	.rdata
	.align	2
	.type	__func__.33756, @object
	.size	__func__.33756, 25
__func__.33756:
	.ascii	"xpondrv_init_global_data\000"
	.align	2
	.type	__func__.33911, @object
	.size	__func__.33911, 14
__func__.33911:
	.ascii	"qdma_reg_init\000"
	.align	2
	.type	__func__.34006, @object
	.size	__func__.34006, 29
__func__.34006:
	.ascii	"xpon_phy_los_illegal_handler\000"
	.align	2
	.type	__func__.34036, @object
	.size	__func__.34036, 23
__func__.34036:
	.ascii	"xpon_phy_ready_handler\000"
	.align	2
	.type	__func__.34057, @object
	.size	__func__.34057, 24
__func__.34057:
	.ascii	"xpon_phy_event_dispatch\000"
	.align	2
	.type	__func__.34142, @object
	.size	__func__.34142, 13
__func__.34142:
	.ascii	"xpondrv_init\000"
	.ident	"GCC: (Buildroot 2015.08.1) 4.9.3"
