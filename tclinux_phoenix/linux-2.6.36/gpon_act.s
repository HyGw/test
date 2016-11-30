	.file	1 "gpon_act.c"
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
	.globl	taskLedConfig
	.section	.bss,"aw",@nobits
	.align	2
	.type	taskLedConfig, @object
	.size	taskLedConfig, 20
taskLedConfig:
	.space	20
	.globl	gponRecvPloamMessageTask
	.align	2
	.type	gponRecvPloamMessageTask, @object
	.size	gponRecvPloamMessageTask, 20
gponRecvPloamMessageTask:
	.space	20
	.globl	to1_timeout_cnt
	.data
	.align	2
	.type	to1_timeout_cnt, @object
	.size	to1_timeout_cnt, 4
to1_timeout_cnt:
	.word	20
	.globl	to1_rst_cnt
	.section	.bss
	.align	2
	.type	to1_rst_cnt, @object
	.size	to1_rst_cnt, 4
to1_rst_cnt:
	.space	4
	.rdata
	.align	2
$LC0:
	.ascii	"[%lu0ms]TO1 timer timeout. Current State:O%d, Next State"
	.ascii	":O2\012\000"
	.align	2
$LC1:
	.ascii	"[%lu0ms]%s TO1_TIMEOUT_MAX_CNT:%d trigger mac & phy rese"
	.ascii	"t.\015\012\000"
	.section	.text.gpon_act_to1_timer_expires,"ax",@progbits
	.align	2
	.globl	gpon_act_to1_timer_expires
	.set	nomips16
	.set	nomicromips
	.ent	gpon_act_to1_timer_expires
	.type	gpon_act_to1_timer_expires, @function
gpon_act_to1_timer_expires:
	.frame	$sp,56,$31		# vars= 32, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	addiu	$sp,$sp,-56
	sw	$31,52($sp)
	sw	$4,56($sp)
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lw	$3,4($2)
	li	$2,3			# 0x3
	beq	$3,$2,$L2
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lw	$3,4($2)
	li	$2,4			# 0x4
	bne	$3,$2,$L1
$L2:
	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	lhu	$2,-30816($2)
	andi	$2,$2,0x9
	beq	$2,$0,$L4
	lui	$2,%hi(jiffies)
	lw	$3,%lo(jiffies)($2)
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	lw	$2,16572($2)
	ext	$2,$2,0,3
	andi	$2,$2,0x00ff
	move	$6,$2
	lui	$2,%hi($LC0)
	addiu	$4,$2,%lo($LC0)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
$L4:
	li	$2,1			# 0x1
	sw	$2,16($sp)
	lui	$2,%hi(to1_timeout_cnt)
	addiu	$2,$2,%lo(to1_timeout_cnt)
	sw	$2,20($sp)
#APP
 # 192 "/opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/atomic.h" 1
			

 # 0 "" 2
#NO_APP
	lw	$5,20($sp)
	lw	$2,16($sp)
	lw	$3,20($sp)
#APP
 # 211 "/opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/atomic.h" 1
		.set	mips3					
1:	ll	$6, 0($5)		# atomic_sub_return	
	subu	$4, $6, $2				
	sc	$4, 0($5)					
	beqz	$4, 2f					
	subu	$4, $6, $2				
	.subsection 2					
2:	b	1b					
	.previous					
	.set	mips0					

 # 0 "" 2
#NO_APP
	move	$3,$6
	sw	$4,24($sp)
	sw	$3,28($sp)
#APP
 # 235 "/opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/atomic.h" 1
			

 # 0 "" 2
#NO_APP
	lw	$2,24($sp)
	bne	$2,$0,$L6
	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	lhu	$2,-30816($2)
	andi	$2,$2,0x9
	beq	$2,$0,$L7
	lui	$2,%hi(jiffies)
	lw	$3,%lo(jiffies)($2)
	lui	$2,%hi($LC1)
	addiu	$4,$2,%lo($LC1)
	move	$5,$3
	lui	$2,%hi(__func__.30953)
	addiu	$6,$2,%lo(__func__.30953)
	li	$7,20			# 0x14
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
$L7:
	li	$4,1			# 0x1
	lui	$2,%hi(xmcs_set_connection_start)
	addiu	$2,$2,%lo(xmcs_set_connection_start)
	jalr	$2
	lui	$2,%hi(to1_timeout_cnt)
	li	$3,20			# 0x14
	sw	$3,%lo(to1_timeout_cnt)($2)
	li	$2,1			# 0x1
	sw	$2,32($sp)
	lui	$2,%hi(to1_rst_cnt)
	addiu	$2,$2,%lo(to1_rst_cnt)
	sw	$2,36($sp)
	lw	$4,36($sp)
	lw	$3,32($sp)
	lw	$2,36($sp)
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
	sw	$2,40($sp)
$L6:
	li	$4,2			# 0x2
	lui	$2,%hi(gpon_act_change_gpon_state)
	addiu	$2,$2,%lo(gpon_act_change_gpon_state)
	jalr	$2
$L1:
	lw	$31,52($sp)
	addiu	$sp,$sp,56
	j	$31
	.end	gpon_act_to1_timer_expires
	.size	gpon_act_to1_timer_expires, .-gpon_act_to1_timer_expires
	.rdata
	.align	2
$LC2:
	.ascii	"[%lu0ms]TO2 timer timeout. Current State:O%d, Next State"
	.ascii	":O1\012\000"
	.section	.text.gpon_act_to2_timer_expires,"ax",@progbits
	.align	2
	.globl	gpon_act_to2_timer_expires
	.set	nomips16
	.set	nomicromips
	.ent	gpon_act_to2_timer_expires
	.type	gpon_act_to2_timer_expires, @function
gpon_act_to2_timer_expires:
	.frame	$sp,24,$31		# vars= 0, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-24
	sw	$31,20($sp)
	sw	$4,24($sp)
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lw	$3,4($2)
	li	$2,6			# 0x6
	beq	$3,$2,$L9
	nop

	j	$L8
	nop

$L9:
	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	lhu	$2,-30816($2)
	andi	$2,$2,0x9
	beq	$2,$0,$L11
	nop

	lui	$2,%hi(jiffies)
	lw	$3,%lo(jiffies)($2)
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	lw	$2,16572($2)
	ext	$2,$2,0,3
	andi	$2,$2,0x00ff
	move	$6,$2
	lui	$2,%hi($LC2)
	addiu	$4,$2,%lo($LC2)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L11:
	li	$4,1			# 0x1
	lui	$2,%hi(xpon_set_qdma_qos)
	addiu	$2,$2,%lo(xpon_set_qdma_qos)
	jalr	$2
	nop

	lui	$2,%hi(gpPonSysData)
	lw	$2,%lo(gpPonSysData)($2)
	sw	$0,0($2)
	lui	$2,%hi(gpPonSysData)
	lw	$2,%lo(gpPonSysData)($2)
	li	$3,1			# 0x1
	sw	$3,4($2)
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lw	$3,448($2)
	li	$2,33554432			# 0x2000000
	and	$2,$3,$2
	sltu	$2,$2,1
	andi	$2,$2,0x00ff
	beq	$2,$0,$L12
	nop

	lui	$2,%hi(is_hwnat_dont_clean)
	sw	$0,%lo(is_hwnat_dont_clean)($2)
$L12:
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

	lui	$2,%hi(stop_omci_oam_monitor)
	addiu	$2,$2,%lo(stop_omci_oam_monitor)
	jalr	$2
	nop

	move	$4,$0
	lui	$2,%hi(gpon_disable_with_option)
	addiu	$2,$2,%lo(gpon_disable_with_option)
	jalr	$2
	nop

$L8:
	lw	$31,20($sp)
	addiu	$sp,$sp,24
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	gpon_act_to2_timer_expires
	.size	gpon_act_to2_timer_expires, .-gpon_act_to2_timer_expires
	.rdata
	.align	2
$LC3:
	.ascii	"[%lu0ms]PLOAM: State non-change: original = O%d  new = O"
	.ascii	"%d\012\000"
	.align	2
$LC4:
	.ascii	"ECNT_HOOK_ERROR occur with cmd_id:0x%x\012\000"
	.align	2
$LC5:
	.ascii	"No such API with type:%d, cmd_id:0x%x\012\000"
	.align	2
$LC6:
	.ascii	"[%lu0ms]PLOAM: State change O%d --->  O%d\012\000"
	.align	2
$LC7:
	.ascii	"[%lu0ms]Start the T1 timer. Current State:O%d\012\000"
	.align	2
$LC8:
	.ascii	"[%lu0ms]Start the T2 timer. Current State:O%d\012\000"
	.align	2
$LC9:
	.ascii	"[%lu0ms]Change the current state: O%d. Last State:O%d\012"
	.ascii	"\000"
	.section	.text.gpon_act_change_gpon_state,"ax",@progbits
	.align	2
	.globl	gpon_act_change_gpon_state
	.set	nomips16
	.set	nomicromips
	.ent	gpon_act_change_gpon_state
	.type	gpon_act_change_gpon_state, @function
gpon_act_change_gpon_state:
	.frame	$sp,272,$31		# vars= 248, regs= 2/0, args= 16, gp= 0
	.mask	0x80010000,-4
	.fmask	0x00000000,0
	addiu	$sp,$sp,-272
	sw	$31,268($sp)
	sw	$16,264($sp)
	sw	$4,272($sp)
	sw	$0,16($sp)
	sb	$0,20($sp)
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	addiu	$2,$2,452
	sw	$2,28($sp)
	lw	$2,28($sp)
	move	$4,$2
	lui	$2,%hi(_raw_spin_lock_irqsave)
	addiu	$2,$2,%lo(_raw_spin_lock_irqsave)
	jalr	$2
	sw	$2,16($sp)
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lw	$3,4($2)
	lw	$2,272($sp)
	bne	$3,$2,$L16
	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	lhu	$2,-30816($2)
	andi	$2,$2,0x10
	beq	$2,$0,$L17
	lui	$2,%hi(jiffies)
	lw	$5,%lo(jiffies)($2)
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lw	$3,4($2)
	lui	$2,%hi($LC3)
	addiu	$4,$2,%lo($LC3)
	move	$6,$3
	lw	$7,272($sp)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
$L17:
	li	$2,1			# 0x1
	sb	$2,20($sp)
$L16:
	lw	$3,272($sp)
	li	$2,5			# 0x5
	bne	$3,$2,$L18
	li	$2,128			# 0x80
	sw	$2,156($sp)
	li	$2,3			# 0x3
	sb	$2,151($sp)
	li	$2,18			# 0x12
	sw	$2,32($sp)
	addiu	$2,$sp,144
	sw	$2,36($sp)
	sw	$0,240($sp)
	sw	$0,244($sp)
	sw	$0,248($sp)
	sw	$0,252($sp)
	li	$2,1			# 0x1
	sw	$2,240($sp)
	lw	$2,32($sp)
	sw	$2,244($sp)
	lw	$2,36($sp)
	sw	$2,252($sp)
	addiu	$2,$sp,240
	sw	$2,40($sp)
	li	$4,25			# 0x19
	move	$5,$0
	lw	$6,40($sp)
	lui	$2,%hi(__ECNT_HOOK)
	addiu	$2,$2,%lo(__ECNT_HOOK)
	jalr	$2
	move	$3,$2
	li	$2,-1			# 0xffffffffffffffff
	bne	$3,$2,$L19
	lw	$2,40($sp)
	lw	$3,4($2)
	lui	$2,%hi($LC4)
	addiu	$4,$2,%lo($LC4)
	move	$5,$3
	lui	$2,%hi(panic)
	addiu	$2,$2,%lo(panic)
	jalr	$2
$L19:
	lw	$2,40($sp)
	lw	$3,8($2)
	li	$2,-1			# 0xffffffffffffffff
	bne	$3,$2,$L20
	lw	$2,40($sp)
	lw	$5,0($2)
	lw	$2,40($sp)
	lw	$3,4($2)
	lui	$2,%hi($LC5)
	addiu	$4,$2,%lo($LC5)
	move	$6,$3
	lui	$2,%hi(panic)
	addiu	$2,$2,%lo(panic)
	jalr	$2
$L20:
	li	$4,1			# 0x1
	lui	$2,%hi(gponDevSetFilterExtBurstLengthPLOAM)
	addiu	$2,$2,%lo(gponDevSetFilterExtBurstLengthPLOAM)
	jalr	$2
	li	$4,1			# 0x1
	lui	$2,%hi(gponDevSetFilterUpstreamOverheadPLOAM)
	addiu	$2,$2,%lo(gponDevSetFilterUpstreamOverheadPLOAM)
	jalr	$2
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lw	$3,308($2)
	li	$2,-1			# 0xffffffffffffffff
	bne	$3,$2,$L25
	lui	$2,%hi(gpGponPriv)
	lw	$16,%lo(gpGponPriv)($2)
	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$2,$2,0x2d4
	sw	$2,44($sp)
	li	$2,65535			# 0xffff
	sw	$2,48($sp)
	sw	$0,52($sp)
	lw	$2,44($sp)
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	move	$3,$2
	lw	$2,52($sp)
	srl	$3,$3,$2
	lw	$2,48($sp)
	and	$2,$3,$2
	sw	$2,308($16)
	j	$L25
$L18:
	lw	$3,272($sp)
	li	$2,4			# 0x4
	beq	$3,$2,$L26
	lw	$3,272($sp)
	li	$2,3			# 0x3
	bne	$3,$2,$L27
$L26:
	li	$2,128			# 0x80
	sw	$2,156($sp)
	li	$2,2			# 0x2
	sb	$2,151($sp)
	li	$2,18			# 0x12
	sw	$2,56($sp)
	addiu	$2,$sp,144
	sw	$2,60($sp)
	sw	$0,224($sp)
	sw	$0,228($sp)
	sw	$0,232($sp)
	sw	$0,236($sp)
	li	$2,1			# 0x1
	sw	$2,224($sp)
	lw	$2,56($sp)
	sw	$2,228($sp)
	lw	$2,60($sp)
	sw	$2,236($sp)
	addiu	$2,$sp,224
	sw	$2,64($sp)
	li	$4,25			# 0x19
	move	$5,$0
	lw	$6,64($sp)
	lui	$2,%hi(__ECNT_HOOK)
	addiu	$2,$2,%lo(__ECNT_HOOK)
	jalr	$2
	move	$3,$2
	li	$2,-1			# 0xffffffffffffffff
	bne	$3,$2,$L28
	lw	$2,64($sp)
	lw	$3,4($2)
	lui	$2,%hi($LC4)
	addiu	$4,$2,%lo($LC4)
	move	$5,$3
	lui	$2,%hi(panic)
	addiu	$2,$2,%lo(panic)
	jalr	$2
$L28:
	lw	$2,64($sp)
	lw	$3,8($2)
	li	$2,-1			# 0xffffffffffffffff
	bne	$3,$2,$L56
	lw	$2,64($sp)
	lw	$5,0($2)
	lw	$2,64($sp)
	lw	$3,4($2)
	lui	$2,%hi($LC5)
	addiu	$4,$2,%lo($LC5)
	move	$6,$3
	lui	$2,%hi(panic)
	addiu	$2,$2,%lo(panic)
	jalr	$2
$L27:
	lw	$3,272($sp)
	li	$2,2			# 0x2
	bne	$3,$2,$L25
	li	$2,240			# 0xf0
	sw	$2,156($sp)
	sb	$0,148($sp)
	sb	$0,149($sp)
	sb	$0,150($sp)
	sb	$0,151($sp)
	li	$2,18			# 0x12
	sw	$2,68($sp)
	addiu	$2,$sp,144
	sw	$2,72($sp)
	sw	$0,208($sp)
	sw	$0,212($sp)
	sw	$0,216($sp)
	sw	$0,220($sp)
	li	$2,1			# 0x1
	sw	$2,208($sp)
	lw	$2,68($sp)
	sw	$2,212($sp)
	lw	$2,72($sp)
	sw	$2,220($sp)
	addiu	$2,$sp,208
	sw	$2,76($sp)
	li	$4,25			# 0x19
	move	$5,$0
	lw	$6,76($sp)
	lui	$2,%hi(__ECNT_HOOK)
	addiu	$2,$2,%lo(__ECNT_HOOK)
	jalr	$2
	move	$3,$2
	li	$2,-1			# 0xffffffffffffffff
	bne	$3,$2,$L32
	lw	$2,76($sp)
	lw	$3,4($2)
	lui	$2,%hi($LC4)
	addiu	$4,$2,%lo($LC4)
	move	$5,$3
	lui	$2,%hi(panic)
	addiu	$2,$2,%lo(panic)
	jalr	$2
$L32:
	lw	$2,76($sp)
	lw	$3,8($2)
	li	$2,-1			# 0xffffffffffffffff
	bne	$3,$2,$L33
	lw	$2,76($sp)
	lw	$5,0($2)
	lw	$2,76($sp)
	lw	$3,4($2)
	lui	$2,%hi($LC5)
	addiu	$4,$2,%lo($LC5)
	move	$6,$3
	lui	$2,%hi(panic)
	addiu	$2,$2,%lo(panic)
	jalr	$2
$L33:
	sw	$0,188($sp)
	li	$2,10			# 0xa
	sw	$2,80($sp)
	addiu	$2,$sp,188
	sw	$2,84($sp)
	sw	$0,192($sp)
	sw	$0,196($sp)
	sw	$0,200($sp)
	sw	$0,204($sp)
	li	$2,1			# 0x1
	sw	$2,192($sp)
	lw	$2,80($sp)
	sw	$2,196($sp)
	lw	$2,84($sp)
	sw	$2,204($sp)
	addiu	$2,$sp,192
	sw	$2,88($sp)
	li	$4,25			# 0x19
	move	$5,$0
	lw	$6,88($sp)
	lui	$2,%hi(__ECNT_HOOK)
	addiu	$2,$2,%lo(__ECNT_HOOK)
	jalr	$2
	move	$3,$2
	li	$2,-1			# 0xffffffffffffffff
	bne	$3,$2,$L36
	lw	$2,88($sp)
	lw	$3,4($2)
	lui	$2,%hi($LC4)
	addiu	$4,$2,%lo($LC4)
	move	$5,$3
	lui	$2,%hi(panic)
	addiu	$2,$2,%lo(panic)
	jalr	$2
$L36:
	lw	$2,88($sp)
	lw	$3,8($2)
	li	$2,-1			# 0xffffffffffffffff
	bne	$3,$2,$L37
	lw	$2,88($sp)
	lw	$5,0($2)
	lw	$2,88($sp)
	lw	$3,4($2)
	lui	$2,%hi($LC5)
	addiu	$4,$2,%lo($LC5)
	move	$6,$3
	lui	$2,%hi(panic)
	addiu	$2,$2,%lo(panic)
	jalr	$2
$L37:
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lhu	$2,292($2)
	move	$4,$2
	lui	$2,%hi(gponDevSetResponseTime)
	addiu	$2,$2,%lo(gponDevSetResponseTime)
	jalr	$2
	j	$L25
$L56:
	.set	noreorder
	nop
	.set	reorder
$L25:
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16572
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	sw	$2,140($sp)
	lw	$2,140($sp)
	ext	$2,$2,0,3
	andi	$2,$2,0x00ff
	sw	$2,24($sp)
	lw	$2,272($sp)
	andi	$2,$2,0x00ff
	andi	$2,$2,0x7
	andi	$3,$2,0x00ff
	lw	$2,140($sp)
	ins	$2,$3,0,3
	sw	$2,140($sp)
	lw	$3,140($sp)
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16572
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	lhu	$2,-30816($2)
	andi	$2,$2,0x10
	beq	$2,$0,$L41
	lui	$2,%hi(jiffies)
	lw	$5,%lo(jiffies)($2)
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lw	$3,4($2)
	lui	$2,%hi($LC6)
	addiu	$4,$2,%lo($LC6)
	move	$6,$3
	lw	$7,272($sp)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
$L41:
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lw	$3,272($sp)
	sw	$3,4($2)
	lw	$3,272($sp)
	li	$2,5			# 0x5
	bne	$3,$2,$L42
	lui	$2,%hi(to1_timeout_cnt)
	li	$3,20			# 0x14
	sw	$3,%lo(to1_timeout_cnt)($2)
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	sw	$0,0($2)
	li	$2,2			# 0x2
	sw	$2,168($sp)
	li	$2,9			# 0x9
	sw	$2,92($sp)
	addiu	$2,$sp,168
	sw	$2,96($sp)
	sw	$0,172($sp)
	sw	$0,176($sp)
	sw	$0,180($sp)
	sw	$0,184($sp)
	li	$2,1			# 0x1
	sw	$2,172($sp)
	lw	$2,92($sp)
	sw	$2,176($sp)
	lw	$2,96($sp)
	sw	$2,184($sp)
	addiu	$2,$sp,172
	sw	$2,100($sp)
	li	$4,25			# 0x19
	move	$5,$0
	lw	$6,100($sp)
	lui	$2,%hi(__ECNT_HOOK)
	addiu	$2,$2,%lo(__ECNT_HOOK)
	jalr	$2
	move	$3,$2
	li	$2,-1			# 0xffffffffffffffff
	bne	$3,$2,$L43
	lw	$2,100($sp)
	lw	$3,4($2)
	lui	$2,%hi($LC4)
	addiu	$4,$2,%lo($LC4)
	move	$5,$3
	lui	$2,%hi(panic)
	addiu	$2,$2,%lo(panic)
	jalr	$2
$L43:
	lw	$2,100($sp)
	lw	$3,8($2)
	li	$2,-1			# 0xffffffffffffffff
	bne	$3,$2,$L47
	lw	$2,100($sp)
	lw	$5,0($2)
	lw	$2,100($sp)
	lw	$3,4($2)
	lui	$2,%hi($LC5)
	addiu	$4,$2,%lo($LC5)
	move	$6,$3
	lui	$2,%hi(panic)
	addiu	$2,$2,%lo(panic)
	jalr	$2
$L42:
	lw	$3,272($sp)
	li	$2,5			# 0x5
	beq	$3,$2,$L47
	lui	$2,%hi(gpWanPriv)
	lw	$3,%lo(gpWanPriv)($2)
	li	$2,65536			# 0x10000
	addu	$3,$3,$2
	lbu	$2,-31786($3)
	li	$4,1			# 0x1
	ins	$2,$4,5,1
	sb	$2,-31786($3)
	li	$4,1			# 0x1
	lui	$2,%hi(xpon_set_qdma_qos)
	addiu	$2,$2,%lo(xpon_set_qdma_qos)
	jalr	$2
$L47:
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lw	$3,4($2)
	li	$2,3			# 0x3
	beq	$3,$2,$L48
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lw	$3,4($2)
	li	$2,4			# 0x4
	bne	$3,$2,$L49
$L48:
	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	lhu	$2,-30816($2)
	andi	$2,$2,0x8
	beq	$2,$0,$L50
	lui	$2,%hi(jiffies)
	lw	$3,%lo(jiffies)($2)
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	lw	$2,16572($2)
	ext	$2,$2,0,3
	andi	$2,$2,0x00ff
	move	$6,$2
	lui	$2,%hi($LC7)
	addiu	$4,$2,%lo($LC7)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
$L50:
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	addiu	$4,$2,8
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lw	$2,28($2)
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
	j	$L51
$L49:
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lw	$3,4($2)
	li	$2,6			# 0x6
	bne	$3,$2,$L51
	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	lhu	$2,-30816($2)
	andi	$2,$2,0x8
	beq	$2,$0,$L52
	lui	$2,%hi(jiffies)
	lw	$3,%lo(jiffies)($2)
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	lw	$2,16572($2)
	ext	$2,$2,0,3
	andi	$2,$2,0x00ff
	move	$6,$2
	lui	$2,%hi($LC8)
	addiu	$4,$2,%lo($LC8)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
$L52:
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	addiu	$4,$2,36
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lw	$2,56($2)
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
$L51:
	lui	$2,%hi(taskLedConfig)
	addiu	$2,$2,%lo(taskLedConfig)
	sw	$2,104($sp)
	lw	$2,104($sp)
	addiu	$2,$2,4
	sw	$0,108($sp)
	sw	$2,112($sp)
	lw	$2,108($sp)
	andi	$2,$2,0xffff
	andi	$2,$2,0x1f
	sh	$2,116($sp)
#APP
 # 261 "/opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/bitops.h" 1
			

 # 0 "" 2
#NO_APP
	lw	$2,108($sp)
	srl	$2,$2,5
	sll	$2,$2,2
	lw	$3,112($sp)
	addu	$2,$3,$2
	sw	$2,120($sp)
	lhu	$2,116($sp)
	li	$3,1			# 0x1
	sll	$2,$3,$2
	lw	$5,120($sp)
	lw	$3,120($sp)
#APP
 # 282 "/opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/bitops.h" 1
		.set	push					
	.set	noreorder				
	.set	mips3					
1:	ll	$4, 0($5)		# test_and_set_bit	
	or	$6, $4, $2				
	sc	$6, 0($5)					
	beqz	$6, 2f					
	 and	$6, $4, $2				
	.subsection 2					
2:	b	1b					
	 nop						
	.previous					
	.set	pop					

 # 0 "" 2
#NO_APP
	move	$3,$6
	sw	$4,124($sp)
	sw	$3,128($sp)
#APP
 # 312 "/opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/bitops.h" 1
			

 # 0 "" 2
#NO_APP
	lw	$2,128($sp)
	sltu	$2,$0,$2
	andi	$2,$2,0x00ff
	bne	$2,$0,$L54
	lw	$4,104($sp)
	lui	$2,%hi(__tasklet_schedule)
	addiu	$2,$2,%lo(__tasklet_schedule)
	jalr	$2
$L54:
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	addiu	$2,$2,452
	sw	$2,132($sp)
	lw	$2,16($sp)
	sw	$2,136($sp)
	lw	$2,132($sp)
	move	$4,$2
	lw	$5,136($sp)
	lui	$2,%hi(_raw_spin_unlock_irqrestore)
	addiu	$2,$2,%lo(_raw_spin_unlock_irqrestore)
	jalr	$2
	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	lhu	$2,-30816($2)
	andi	$2,$2,0x8
	beq	$2,$0,$L14
	lui	$2,%hi(jiffies)
	lw	$3,%lo(jiffies)($2)
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	lw	$2,16572($2)
	ext	$2,$2,0,3
	andi	$2,$2,0x00ff
	move	$6,$2
	lui	$2,%hi($LC9)
	addiu	$4,$2,%lo($LC9)
	move	$5,$3
	lw	$7,24($sp)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
$L14:
	lw	$31,268($sp)
	lw	$16,264($sp)
	addiu	$sp,$sp,272
	j	$31
	.end	gpon_act_change_gpon_state
	.size	gpon_act_change_gpon_state, .-gpon_act_change_gpon_state
	.section	.text.gpon_act_get_gpon_state,"ax",@progbits
	.align	2
	.globl	gpon_act_get_gpon_state
	.set	nomips16
	.set	nomicromips
	.ent	gpon_act_get_gpon_state
	.type	gpon_act_get_gpon_state, @function
gpon_act_get_gpon_state:
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lw	$2,4($2)
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	gpon_act_get_gpon_state
	.size	gpon_act_get_gpon_state, .-gpon_act_get_gpon_state
	.section	.text.gpon_act_led_config,"ax",@progbits
	.align	2
	.globl	gpon_act_led_config
	.set	nomips16
	.set	nomicromips
	.ent	gpon_act_led_config
	.type	gpon_act_led_config, @function
gpon_act_led_config:
	.frame	$sp,32,$31		# vars= 8, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-32
	sw	$31,28($sp)
	li	$2,-1078984704			# 0xffffffffbfb00000
	ori	$2,$2,0x64
	lw	$3,0($2)
	li	$2,-65536			# 0xffffffffffff0000
	and	$3,$3,$2
	li	$2,524288			# 0x80000
	bne	$3,$2,$L60
	nop

	li	$2,-1078984704			# 0xffffffffbfb00000
	ori	$2,$2,0x9c
	lw	$2,0($2)
	andi	$2,$2,0x1
	sltu	$2,$2,1
	andi	$2,$2,0x00ff
	j	$L61
	nop

$L60:
	li	$2,-1078984704			# 0xffffffffbfb00000
	ori	$2,$2,0x64
	lw	$3,0($2)
	li	$2,-65536			# 0xffffffffffff0000
	and	$3,$3,$2
	li	$2,458752			# 0x70000
	beq	$3,$2,$L62
	nop

	li	$2,-1078984704			# 0xffffffffbfb00000
	ori	$2,$2,0x64
	lw	$3,0($2)
	li	$2,-65536			# 0xffffffffffff0000
	and	$3,$3,$2
	li	$2,524288			# 0x80000
	bne	$3,$2,$L63
	nop

$L62:
	li	$2,-1078984704			# 0xffffffffbfb00000
	ori	$2,$2,0x8c
	sw	$2,16($sp)
	lw	$2,16($sp)
	lw	$3,0($2)
	li	$2,536870912			# 0x20000000
	and	$2,$3,$2
	sltu	$2,$2,1
	andi	$2,$2,0x00ff
	j	$L61
	nop

$L63:
	li	$2,-1078984704			# 0xffffffffbfb00000
	ori	$2,$2,0x8c
	sw	$2,20($sp)
	lw	$2,20($sp)
	lw	$2,0($2)
	srl	$2,$2,31
	andi	$2,$2,0x00ff
$L61:
	beq	$2,$0,$L67
	nop

	j	$L59
	nop

$L67:
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lw	$3,4($2)
	li	$2,1			# 0x1
	beq	$3,$2,$L69
	nop

	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lw	$3,4($2)
	li	$2,6			# 0x6
	beq	$3,$2,$L69
	nop

	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lw	$3,4($2)
	li	$2,7			# 0x7
	bne	$3,$2,$L70
	nop

$L69:
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lw	$3,4($2)
	li	$2,7			# 0x7
	bne	$3,$2,$L71
	nop

	li	$4,1			# 0x1
	lui	$2,%hi(change_alarm_led_status)
	addiu	$2,$2,%lo(change_alarm_led_status)
	jalr	$2
	nop

$L71:
	li	$4,64			# 0x40
	lui	$2,%hi(ledTurnOff)
	addiu	$2,$2,%lo(ledTurnOff)
	jalr	$2
	nop

	li	$4,65			# 0x41
	lui	$2,%hi(ledTurnOff)
	addiu	$2,$2,%lo(ledTurnOff)
	jalr	$2
	nop

	lui	$2,%hi(led_xpon_status)
	sw	$0,%lo(led_xpon_status)($2)
	j	$L59
	nop

$L70:
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lw	$3,4($2)
	li	$2,5			# 0x5
	bne	$3,$2,$L72
	nop

	li	$4,65			# 0x41
	lui	$2,%hi(ledTurnOff)
	addiu	$2,$2,%lo(ledTurnOff)
	jalr	$2
	nop

	li	$4,64			# 0x40
	lui	$2,%hi(ledTurnOn)
	addiu	$2,$2,%lo(ledTurnOn)
	jalr	$2
	nop

	move	$4,$0
	lui	$2,%hi(change_alarm_led_status)
	addiu	$2,$2,%lo(change_alarm_led_status)
	jalr	$2
	nop

	lui	$2,%hi(led_xpon_status)
	li	$3,2			# 0x2
	sw	$3,%lo(led_xpon_status)($2)
	j	$L59
	nop

$L72:
	li	$4,64			# 0x40
	lui	$2,%hi(ledTurnOff)
	addiu	$2,$2,%lo(ledTurnOff)
	jalr	$2
	nop

	li	$4,65			# 0x41
	lui	$2,%hi(ledTurnOn)
	addiu	$2,$2,%lo(ledTurnOn)
	jalr	$2
	nop

	lui	$2,%hi(led_xpon_status)
	li	$3,1			# 0x1
	sw	$3,%lo(led_xpon_status)($2)
$L59:
	lw	$31,28($sp)
	addiu	$sp,$sp,32
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	gpon_act_led_config
	.size	gpon_act_led_config, .-gpon_act_led_config
	.section	.text.gpon_act_init,"ax",@progbits
	.align	2
	.globl	gpon_act_init
	.set	nomips16
	.set	nomicromips
	.ent	gpon_act_init
	.type	gpon_act_init, @function
gpon_act_init:
	.frame	$sp,24,$31		# vars= 0, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-24
	sw	$31,20($sp)
	lui	$2,%hi(taskLedConfig)
	addiu	$4,$2,%lo(taskLedConfig)
	lui	$2,%hi(gpon_act_led_config)
	addiu	$5,$2,%lo(gpon_act_led_config)
	move	$6,$0
	lui	$2,%hi(tasklet_init)
	addiu	$2,$2,%lo(tasklet_init)
	jalr	$2
	nop

	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	li	$3,1			# 0x1
	sw	$3,4($2)
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	addiu	$2,$2,8
	move	$4,$2
	move	$5,$0
	move	$6,$0
	lui	$2,%hi(init_timer_key)
	addiu	$2,$2,%lo(init_timer_key)
	jalr	$2
	nop

	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lui	$3,%hi(gpGponPriv)
	lw	$3,%lo(gpGponPriv)($3)
	lw	$3,256($3)
	sw	$3,28($2)
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lui	$3,%hi(gpon_act_to1_timer_expires)
	addiu	$3,$3,%lo(gpon_act_to1_timer_expires)
	sw	$3,24($2)
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	addiu	$2,$2,36
	move	$4,$2
	move	$5,$0
	move	$6,$0
	lui	$2,%hi(init_timer_key)
	addiu	$2,$2,%lo(init_timer_key)
	jalr	$2
	nop

	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lui	$3,%hi(gpGponPriv)
	lw	$3,%lo(gpGponPriv)($3)
	lw	$3,260($3)
	sw	$3,56($2)
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lui	$3,%hi(gpon_act_to2_timer_expires)
	addiu	$3,$3,%lo(gpon_act_to2_timer_expires)
	sw	$3,52($2)
	move	$2,$0
	lw	$31,20($sp)
	addiu	$sp,$sp,24
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	gpon_act_init
	.size	gpon_act_init, .-gpon_act_init
	.section	.text.gpon_act_deinit,"ax",@progbits
	.align	2
	.globl	gpon_act_deinit
	.set	nomips16
	.set	nomicromips
	.ent	gpon_act_deinit
	.type	gpon_act_deinit, @function
gpon_act_deinit:
	.frame	$sp,24,$31		# vars= 0, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-24
	sw	$31,20($sp)
	lui	$2,%hi(taskLedConfig)
	addiu	$4,$2,%lo(taskLedConfig)
	lui	$2,%hi(tasklet_kill)
	addiu	$2,$2,%lo(tasklet_kill)
	jalr	$2
	nop

	move	$2,$0
	lw	$31,20($sp)
	addiu	$sp,$sp,24
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	gpon_act_deinit
	.size	gpon_act_deinit, .-gpon_act_deinit
	.rdata
	.align	2
	.type	__func__.30953, @object
	.size	__func__.30953, 27
__func__.30953:
	.ascii	"gpon_act_to1_timer_expires\000"
	.ident	"GCC: (Buildroot 2015.08.1) 4.9.3"
