	.file	1 "xpon_daemon.c"
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
	.section	.text.xpon_daemon_quit,"ax",@progbits
	.align	2
	.globl	xpon_daemon_quit
	.set	nomips16
	.set	nomicromips
	.ent	xpon_daemon_quit
	.type	xpon_daemon_quit, @function
xpon_daemon_quit:
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
	lw	$2,-30692($2)
	sw	$2,16($sp)
	lw	$2,16($sp)
	sltu	$2,$2,-4095
	xori	$2,$2,0x1
	andi	$2,$2,0x00ff
	bne	$2,$0,$L1
	nop

	li	$2,4			# 0x4
	sw	$2,20($sp)
	li	$2,1			# 0x1
	sw	$2,24($sp)
	addiu	$2,$sp,20
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

	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	lw	$2,-30692($2)
	move	$4,$2
	lui	$2,%hi(kthread_stop)
	addiu	$2,$2,%lo(kthread_stop)
	jalr	$2
	nop

$L1:
	lw	$31,36($sp)
	addiu	$sp,$sp,40
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	xpon_daemon_quit
	.size	xpon_daemon_quit, .-xpon_daemon_quit
	.rdata
	.align	2
$LC0:
	.ascii	"xpon daemon job queue full! Drop jobs:%d\012\000"
	.section	.text.xpon_daemon_job_enqueue,"ax",@progbits
	.align	2
	.globl	xpon_daemon_job_enqueue
	.set	nomips16
	.set	nomicromips
	.ent	xpon_daemon_job_enqueue
	.type	xpon_daemon_job_enqueue, @function
xpon_daemon_job_enqueue:
	.frame	$sp,64,$31		# vars= 40, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	addiu	$sp,$sp,-64
	sw	$31,60($sp)
	sw	$4,64($sp)
	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,34848			# 0x8820
	addu	$2,$3,$2
	sw	$2,16($sp)
	lw	$2,16($sp)
	addiu	$2,$2,256
	sw	$2,28($sp)
	lw	$2,28($sp)
	move	$4,$2
	lui	$2,%hi(_raw_spin_lock_irqsave)
	addiu	$2,$2,%lo(_raw_spin_lock_irqsave)
	jalr	$2
	sw	$2,20($sp)
	lw	$2,16($sp)
	lw	$2,260($2)
	sw	$2,24($sp)
	lw	$3,16($sp)
	lw	$2,24($sp)
	sll	$2,$2,4
	addu	$2,$3,$2
	lbu	$2,12($2)
	andi	$2,$2,0x00ff
	bne	$2,$0,$L6
	lw	$3,16($sp)
	lw	$2,24($sp)
	sll	$2,$2,4
	addu	$2,$3,$2
	lw	$3,64($sp)
	lw	$5,0($3)
	lw	$4,4($3)
	lw	$3,8($3)
	sw	$5,0($2)
	sw	$4,4($2)
	sw	$3,8($2)
#APP
 # 37 "/opt/tclinux_phoenix/modules/private/xpon/src/xpon_daemon.c" 1
	.set	push
	.set	noreorder
	.set	mips2
	sync
	.set	pop
 # 0 "" 2
#NO_APP
	lw	$3,16($sp)
	lw	$2,24($sp)
	sll	$2,$2,4
	addu	$2,$3,$2
	li	$3,1			# 0x1
	sb	$3,12($2)
#APP
 # 39 "/opt/tclinux_phoenix/modules/private/xpon/src/xpon_daemon.c" 1
	.set	push
	.set	noreorder
	.set	mips2
	sync
	.set	pop
 # 0 "" 2
#NO_APP
	lw	$2,16($sp)
	lw	$2,260($2)
	addiu	$2,$2,1
	andi	$3,$2,0xf
	lw	$2,16($sp)
	sw	$3,260($2)
	j	$L7
$L6:
	lui	$2,%hi(dump_stack)
	addiu	$2,$2,%lo(dump_stack)
	jalr	$2
	lw	$2,64($sp)
	lw	$3,0($2)
	lui	$2,%hi($LC0)
	addiu	$4,$2,%lo($LC0)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
$L7:
	lw	$2,16($sp)
	addiu	$2,$2,256
	sw	$2,32($sp)
	lw	$2,20($sp)
	sw	$2,36($sp)
	lw	$2,32($sp)
	move	$4,$2
	lw	$5,36($sp)
	lui	$2,%hi(_raw_spin_unlock_irqrestore)
	addiu	$2,$2,%lo(_raw_spin_unlock_irqrestore)
	jalr	$2
	lw	$31,60($sp)
	addiu	$sp,$sp,64
	j	$31
	.end	xpon_daemon_job_enqueue
	.size	xpon_daemon_job_enqueue, .-xpon_daemon_job_enqueue
	.section	.text.xpon_daemon_job_dequeue,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	xpon_daemon_job_dequeue
	.type	xpon_daemon_job_dequeue, @function
xpon_daemon_job_dequeue:
	.frame	$sp,8,$31		# vars= 8, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	addiu	$sp,$sp,-8
	sw	$4,8($sp)
	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,34848			# 0x8820
	addu	$2,$3,$2
	sw	$2,0($sp)
	lw	$2,0($sp)
	lw	$2,264($2)
	sw	$2,4($sp)
	lw	$3,0($sp)
	lw	$2,4($sp)
	sll	$2,$2,4
	addu	$2,$3,$2
	lbu	$2,12($2)
	andi	$2,$2,0x00ff
	bne	$2,$0,$L9
	move	$2,$0
	j	$L10
$L9:
	lw	$2,8($sp)
	lw	$4,0($sp)
	lw	$3,4($sp)
	sll	$3,$3,4
	addu	$3,$4,$3
	lw	$5,0($3)
	lw	$4,4($3)
	lw	$3,8($3)
	sw	$5,0($2)
	sw	$4,4($2)
	sw	$3,8($2)
#APP
 # 64 "/opt/tclinux_phoenix/modules/private/xpon/src/xpon_daemon.c" 1
	.set	push
	.set	noreorder
	.set	mips2
	sync
	.set	pop
 # 0 "" 2
#NO_APP
	lw	$3,0($sp)
	lw	$2,4($sp)
	sll	$2,$2,4
	addu	$2,$3,$2
	sb	$0,12($2)
	lw	$2,0($sp)
	lw	$2,264($2)
	addiu	$2,$2,1
	andi	$3,$2,0xf
	lw	$2,0($sp)
	sw	$3,264($2)
	li	$2,1			# 0x1
$L10:
	addiu	$sp,$sp,8
	j	$31
	.end	xpon_daemon_job_dequeue
	.size	xpon_daemon_job_dequeue, .-xpon_daemon_job_dequeue
	.rdata
	.align	2
$LC1:
	.ascii	"[%lu0ms]%s\012\000"
	.align	2
$LC2:
	.ascii	"ECNT_HOOK_ERROR occur with cmd_id:0x%x\012\000"
	.align	2
$LC3:
	.ascii	"No such API with type:%d, cmd_id:0x%x\012\000"
	.section	.text.handle_xpon_daemon_job_get_phy_params,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	handle_xpon_daemon_job_get_phy_params
	.type	handle_xpon_daemon_job_get_phy_params, @function
handle_xpon_daemon_job_get_phy_params:
	.frame	$sp,96,$31		# vars= 72, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	addiu	$sp,$sp,-96
	sw	$31,92($sp)
	sw	$0,56($sp)
	sw	$0,60($sp)
	sw	$0,64($sp)
	sw	$0,68($sp)
	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	lhu	$2,-30816($2)
	andi	$2,$2,0x40
	beq	$2,$0,$L12
	lui	$2,%hi(jiffies)
	lw	$3,%lo(jiffies)($2)
	lui	$2,%hi($LC1)
	addiu	$4,$2,%lo($LC1)
	move	$5,$3
	lui	$2,%hi(__func__.33582)
	addiu	$6,$2,%lo(__func__.33582)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
$L12:
	addiu	$2,$sp,56
	sw	$2,16($sp)
	li	$2,32796			# 0x801c
	sw	$2,20($sp)
	lw	$2,16($sp)
	sw	$2,24($sp)
	sw	$0,72($sp)
	sw	$0,76($sp)
	sw	$0,80($sp)
	sw	$0,84($sp)
	sw	$0,72($sp)
	lw	$2,20($sp)
	sw	$2,76($sp)
	lw	$2,24($sp)
	sw	$2,84($sp)
	addiu	$2,$sp,72
	sw	$2,28($sp)
	li	$4,25			# 0x19
	move	$5,$0
	lw	$6,28($sp)
	lui	$2,%hi(__ECNT_HOOK)
	addiu	$2,$2,%lo(__ECNT_HOOK)
	jalr	$2
	move	$3,$2
	li	$2,-1			# 0xffffffffffffffff
	bne	$3,$2,$L13
	lw	$2,28($sp)
	lw	$3,4($2)
	lui	$2,%hi($LC2)
	addiu	$4,$2,%lo($LC2)
	move	$5,$3
	lui	$2,%hi(panic)
	addiu	$2,$2,%lo(panic)
	jalr	$2
$L13:
	lw	$2,28($sp)
	lw	$3,8($2)
	li	$2,-1			# 0xffffffffffffffff
	bne	$3,$2,$L14
	lw	$2,28($sp)
	lw	$5,0($2)
	lw	$2,28($sp)
	lw	$3,4($2)
	lui	$2,%hi($LC3)
	addiu	$4,$2,%lo($LC3)
	move	$6,$3
	lui	$2,%hi(panic)
	addiu	$2,$2,%lo(panic)
	jalr	$2
$L14:
	lui	$2,%hi(gpPhyData)
	lw	$2,%lo(gpPhyData)($2)
	addiu	$2,$2,92
	sw	$2,32($sp)
	lw	$2,32($sp)
	move	$4,$2
	lui	$2,%hi(_raw_spin_lock)
	addiu	$2,$2,%lo(_raw_spin_lock)
	jalr	$2
	lui	$2,%hi(gpPhyData)
	lw	$2,%lo(gpPhyData)($2)
	lw	$4,56($sp)
	lw	$3,60($sp)
	sw	$4,96($2)
	sw	$3,100($2)
	lhu	$3,64($sp)
	sh	$3,104($2)
	lui	$2,%hi(gpPhyData)
	lw	$2,%lo(gpPhyData)($2)
	addiu	$2,$2,92
	sw	$2,36($sp)
	lw	$2,36($sp)
	sw	$2,40($sp)
	lw	$2,40($sp)
	sw	$2,44($sp)
	lw	$2,44($sp)
	sw	$2,48($sp)
	lw	$2,48($sp)
	lhu	$2,2($2)
	addiu	$2,$2,1
	sw	$2,52($sp)
#APP
 # 139 "/opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/spinlock.h" 1
	.set	push
	.set	noreorder
	.set	mips2
	sync
	.set	pop
 # 0 "" 2
#NO_APP
	lw	$2,52($sp)
	andi	$3,$2,0xffff
	lw	$2,48($sp)
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
	lw	$2,68($sp)
	beq	$2,$0,$L17
	lw	$2,68($sp)
	move	$4,$2
	lui	$2,%hi(xmcs_detect_phy_trans_alarm)
	addiu	$2,$2,%lo(xmcs_detect_phy_trans_alarm)
	jalr	$2
$L17:
	lui	$2,%hi(gpPhyData)
	lw	$2,%lo(gpPhyData)($2)
	sb	$0,62($2)
	lw	$31,92($sp)
	addiu	$sp,$sp,96
	j	$31
	.end	handle_xpon_daemon_job_get_phy_params
	.size	handle_xpon_daemon_job_get_phy_params, .-handle_xpon_daemon_job_get_phy_params
	.section	.text.fe_gdm2_channel_retire,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	fe_gdm2_channel_retire
	.type	fe_gdm2_channel_retire, @function
fe_gdm2_channel_retire:
	.frame	$sp,168,$31		# vars= 144, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-168
	sw	$31,164($sp)
	li	$2,1			# 0x1
	sw	$2,16($sp)
	li	$2,32			# 0x20
	sb	$2,20($sp)
	sw	$0,24($sp)
	li	$2,46			# 0x2e
	sw	$2,28($sp)
	lw	$2,16($sp)
	sw	$2,32($sp)
	lbu	$2,20($sp)
	sw	$2,56($sp)
	addiu	$2,$sp,28
	li	$4,18			# 0x12
	move	$5,$0
	move	$6,$2
	lui	$2,%hi(__ECNT_HOOK)
	addiu	$2,$2,%lo(__ECNT_HOOK)
	jalr	$2
	nop

	sw	$2,24($sp)
	lw	$31,164($sp)
	addiu	$sp,$sp,168
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	fe_gdm2_channel_retire
	.size	fe_gdm2_channel_retire, .-fe_gdm2_channel_retire
	.rdata
	.align	2
$LC4:
	.ascii	"%s:%d #######Doing GPON MAC and XPON PHY Reset!#######\012"
	.ascii	"\000"
	.align	2
$LC5:
	.ascii	"%s:phy device initialization failed.\012\000"
	.section	.text.handle_xpon_daemon_job_gpon_dev_reset,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	handle_xpon_daemon_job_gpon_dev_reset
	.type	handle_xpon_daemon_job_gpon_dev_reset, @function
handle_xpon_daemon_job_gpon_dev_reset:
	.frame	$sp,248,$31		# vars= 224, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-248
	sw	$31,244($sp)
	li	$2,1			# 0x1
	sw	$2,16($sp)
	lui	$2,%hi(xpon_mac_print_open)
	lw	$2,%lo(xpon_mac_print_open)($2)
	beq	$2,$0,$L23
	nop

	lui	$2,%hi($LC4)
	addiu	$4,$2,%lo($LC4)
	lui	$2,%hi(__func__.33591)
	addiu	$5,$2,%lo(__func__.33591)
	li	$6,154			# 0x9a
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L23:
	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$3,$3,$2
	lbu	$2,-30412($3)
	li	$4,1			# 0x1
	ins	$2,$4,6,1
	sb	$2,-30412($3)
	li	$2,2			# 0x2
	sw	$2,24($sp)
	lw	$2,24($sp)
	sw	$2,28($sp)
	sw	$0,32($sp)
	sw	$0,212($sp)
	sw	$0,216($sp)
	sw	$0,220($sp)
	sw	$0,224($sp)
	li	$2,1			# 0x1
	sw	$2,212($sp)
	lw	$2,28($sp)
	sw	$2,216($sp)
	lw	$2,32($sp)
	sw	$2,224($sp)
	addiu	$2,$sp,212
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
	bne	$3,$2,$L24
	nop

	lw	$2,36($sp)
	lw	$3,4($2)
	lui	$2,%hi($LC2)
	addiu	$4,$2,%lo($LC2)
	move	$5,$3
	lui	$2,%hi(panic)
	addiu	$2,$2,%lo(panic)
	jalr	$2
	nop

$L24:
	lw	$2,36($sp)
	lw	$3,8($2)
	li	$2,-1			# 0xffffffffffffffff
	bne	$3,$2,$L25
	nop

	lw	$2,36($sp)
	lw	$5,0($2)
	lw	$2,36($sp)
	lw	$3,4($2)
	lui	$2,%hi($LC3)
	addiu	$4,$2,%lo($LC3)
	move	$6,$3
	lui	$2,%hi(panic)
	addiu	$2,$2,%lo(panic)
	jalr	$2
	nop

$L25:
	lui	$2,%hi(xpon_stop_timer)
	addiu	$2,$2,%lo(xpon_stop_timer)
	jalr	$2
	nop

	li	$4,1			# 0x1
	lui	$2,%hi(gponDevMbiStop)
	addiu	$2,$2,%lo(gponDevMbiStop)
	jalr	$2
	nop

	lui	$2,%hi(disable_cpu_us_traffic)
	addiu	$2,$2,%lo(disable_cpu_us_traffic)
	jalr	$2
	nop

	lui	$2,%hi(fe_gdm2_channel_retire)
	addiu	$2,$2,%lo(fe_gdm2_channel_retire)
	jalr	$2
	nop

	lw	$2,16($sp)
	beq	$2,$0,$L29
	nop

	lui	$2,%hi(gpon_dev_reset_GPON_MAC)
	addiu	$2,$2,%lo(gpon_dev_reset_GPON_MAC)
	jalr	$2
	nop

	j	$L30
	nop

$L29:
	lui	$2,%hi(gpon_dev_reset_GPON_MAC)
	addiu	$2,$2,%lo(gpon_dev_reset_GPON_MAC)
	jalr	$2
	nop

	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	addiu	$2,$2,456
	sw	$2,40($sp)
	lw	$2,40($sp)
	move	$4,$2
	lui	$2,%hi(_raw_spin_lock_irqsave)
	addiu	$2,$2,%lo(_raw_spin_lock_irqsave)
	jalr	$2
	nop

	sw	$2,20($sp)
	li	$2,-1078984704			# 0xffffffffbfb00000
	ori	$2,$2,0x380
	sw	$2,44($sp)
	li	$2,33554432			# 0x2000000
	sw	$2,48($sp)
	lw	$2,44($sp)
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	sw	$2,52($sp)
	lw	$2,48($sp)
	nor	$2,$0,$2
	lw	$3,52($sp)
	and	$2,$3,$2
	sw	$2,52($sp)
	lw	$2,44($sp)
	lw	$4,52($sp)
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
	ori	$2,$2,0x380
	sw	$2,56($sp)
	li	$2,33554432			# 0x2000000
	sw	$2,60($sp)
	lw	$2,56($sp)
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	sw	$2,64($sp)
	lw	$3,64($sp)
	lw	$2,60($sp)
	or	$2,$3,$2
	sw	$2,64($sp)
	lw	$2,56($sp)
	lw	$4,64($sp)
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	addiu	$2,$2,456
	sw	$2,68($sp)
	lw	$2,20($sp)
	sw	$2,72($sp)
	lw	$2,68($sp)
	move	$4,$2
	lw	$5,72($sp)
	lui	$2,%hi(_raw_spin_unlock_irqrestore)
	addiu	$2,$2,%lo(_raw_spin_unlock_irqrestore)
	jalr	$2
	nop

	li	$2,4			# 0x4
	sw	$2,76($sp)
	lw	$2,76($sp)
	sw	$2,80($sp)
	sw	$0,84($sp)
	sw	$0,188($sp)
	sw	$0,192($sp)
	sw	$0,196($sp)
	sw	$0,200($sp)
	li	$2,1			# 0x1
	sw	$2,188($sp)
	lw	$2,80($sp)
	sw	$2,192($sp)
	lw	$2,84($sp)
	sw	$2,200($sp)
	addiu	$2,$sp,188
	sw	$2,88($sp)
	li	$4,25			# 0x19
	move	$5,$0
	lw	$6,88($sp)
	lui	$2,%hi(__ECNT_HOOK)
	addiu	$2,$2,%lo(__ECNT_HOOK)
	jalr	$2
	nop

	move	$3,$2
	li	$2,-1			# 0xffffffffffffffff
	bne	$3,$2,$L32
	nop

	lw	$2,88($sp)
	lw	$3,4($2)
	lui	$2,%hi($LC2)
	addiu	$4,$2,%lo($LC2)
	move	$5,$3
	lui	$2,%hi(panic)
	addiu	$2,$2,%lo(panic)
	jalr	$2
	nop

$L32:
	lw	$2,88($sp)
	lw	$3,8($2)
	li	$2,-1			# 0xffffffffffffffff
	bne	$3,$2,$L33
	nop

	lw	$2,88($sp)
	lw	$5,0($2)
	lw	$2,88($sp)
	lw	$3,4($2)
	lui	$2,%hi($LC3)
	addiu	$4,$2,%lo($LC3)
	move	$6,$3
	lui	$2,%hi(panic)
	addiu	$2,$2,%lo(panic)
	jalr	$2
	nop

$L33:
	lw	$2,88($sp)
	lw	$2,8($2)
	beq	$2,$0,$L37
	nop

	lui	$2,%hi(dump_stack)
	addiu	$2,$2,%lo(dump_stack)
	jalr	$2
	nop

	lui	$2,%hi($LC5)
	addiu	$4,$2,%lo($LC5)
	lui	$2,%hi(__func__.33591)
	addiu	$5,$2,%lo(__func__.33591)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L37:
	li	$2,8			# 0x8
	sw	$2,92($sp)
	lw	$2,92($sp)
	sw	$2,96($sp)
	sw	$0,100($sp)
	sw	$0,172($sp)
	sw	$0,176($sp)
	sw	$0,180($sp)
	sw	$0,184($sp)
	li	$2,1			# 0x1
	sw	$2,172($sp)
	lw	$2,96($sp)
	sw	$2,176($sp)
	lw	$2,100($sp)
	sw	$2,184($sp)
	addiu	$2,$sp,172
	sw	$2,104($sp)
	li	$4,25			# 0x19
	move	$5,$0
	lw	$6,104($sp)
	lui	$2,%hi(__ECNT_HOOK)
	addiu	$2,$2,%lo(__ECNT_HOOK)
	jalr	$2
	nop

	move	$3,$2
	li	$2,-1			# 0xffffffffffffffff
	bne	$3,$2,$L38
	nop

	lw	$2,104($sp)
	lw	$3,4($2)
	lui	$2,%hi($LC2)
	addiu	$4,$2,%lo($LC2)
	move	$5,$3
	lui	$2,%hi(panic)
	addiu	$2,$2,%lo(panic)
	jalr	$2
	nop

$L38:
	lw	$2,104($sp)
	lw	$3,8($2)
	li	$2,-1			# 0xffffffffffffffff
	bne	$3,$2,$L39
	nop

	lw	$2,104($sp)
	lw	$5,0($2)
	lw	$2,104($sp)
	lw	$3,4($2)
	lui	$2,%hi($LC3)
	addiu	$4,$2,%lo($LC3)
	move	$6,$3
	lui	$2,%hi(panic)
	addiu	$2,$2,%lo(panic)
	jalr	$2
	nop

$L39:
	li	$4,1			# 0x1
	lui	$2,%hi(XPON_PHY_SET_MODE)
	addiu	$2,$2,%lo(XPON_PHY_SET_MODE)
	jalr	$2
	nop

$L30:
	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$3,$3,$2
	lbu	$2,-30412($3)
	ins	$2,$0,6,1
	sb	$2,-30412($3)
	lui	$2,%hi(gpPhyData)
	lw	$2,%lo(gpPhyData)($2)
	lw	$3,60($2)
	li	$2,14680064			# 0xe00000
	and	$3,$3,$2
	li	$2,2097152			# 0x200000
	bne	$3,$2,$L43
	nop

	li	$4,2			# 0x2
	lui	$2,%hi(gpon_act_change_gpon_state)
	addiu	$2,$2,%lo(gpon_act_change_gpon_state)
	jalr	$2
	nop

	lui	$2,%hi(gpon_enable)
	addiu	$2,$2,%lo(gpon_enable)
	jalr	$2
	nop

	j	$L44
	nop

$L43:
	li	$4,1			# 0x1
	lui	$2,%hi(gpon_act_change_gpon_state)
	addiu	$2,$2,%lo(gpon_act_change_gpon_state)
	jalr	$2
	nop

$L44:
	li	$2,1			# 0x1
	sw	$2,108($sp)
	lw	$2,108($sp)
	sw	$2,112($sp)
	sw	$0,116($sp)
	sw	$0,156($sp)
	sw	$0,160($sp)
	sw	$0,164($sp)
	sw	$0,168($sp)
	li	$2,1			# 0x1
	sw	$2,156($sp)
	lw	$2,112($sp)
	sw	$2,160($sp)
	lw	$2,116($sp)
	sw	$2,168($sp)
	addiu	$2,$sp,156
	sw	$2,120($sp)
	li	$4,25			# 0x19
	move	$5,$0
	lw	$6,120($sp)
	lui	$2,%hi(__ECNT_HOOK)
	addiu	$2,$2,%lo(__ECNT_HOOK)
	jalr	$2
	nop

	move	$3,$2
	li	$2,-1			# 0xffffffffffffffff
	bne	$3,$2,$L45
	nop

	lw	$2,120($sp)
	lw	$3,4($2)
	lui	$2,%hi($LC2)
	addiu	$4,$2,%lo($LC2)
	move	$5,$3
	lui	$2,%hi(panic)
	addiu	$2,$2,%lo(panic)
	jalr	$2
	nop

$L45:
	lw	$2,120($sp)
	lw	$3,8($2)
	li	$2,-1			# 0xffffffffffffffff
	bne	$3,$2,$L46
	nop

	lw	$2,120($sp)
	lw	$5,0($2)
	lw	$2,120($sp)
	lw	$3,4($2)
	lui	$2,%hi($LC3)
	addiu	$4,$2,%lo($LC3)
	move	$6,$3
	lui	$2,%hi(panic)
	addiu	$2,$2,%lo(panic)
	jalr	$2
	nop

$L46:
	li	$2,1			# 0x1
	sw	$2,136($sp)
	li	$2,5			# 0x5
	sw	$2,124($sp)
	addiu	$2,$sp,136
	sw	$2,128($sp)
	sw	$0,140($sp)
	sw	$0,144($sp)
	sw	$0,148($sp)
	sw	$0,152($sp)
	li	$2,1			# 0x1
	sw	$2,140($sp)
	lw	$2,124($sp)
	sw	$2,144($sp)
	lw	$2,128($sp)
	sw	$2,152($sp)
	addiu	$2,$sp,140
	sw	$2,132($sp)
	li	$4,25			# 0x19
	move	$5,$0
	lw	$6,132($sp)
	lui	$2,%hi(__ECNT_HOOK)
	addiu	$2,$2,%lo(__ECNT_HOOK)
	jalr	$2
	nop

	move	$3,$2
	li	$2,-1			# 0xffffffffffffffff
	bne	$3,$2,$L50
	nop

	lw	$2,132($sp)
	lw	$3,4($2)
	lui	$2,%hi($LC2)
	addiu	$4,$2,%lo($LC2)
	move	$5,$3
	lui	$2,%hi(panic)
	addiu	$2,$2,%lo(panic)
	jalr	$2
	nop

$L50:
	lw	$2,132($sp)
	lw	$3,8($2)
	li	$2,-1			# 0xffffffffffffffff
	bne	$3,$2,$L51
	nop

	lw	$2,132($sp)
	lw	$5,0($2)
	lw	$2,132($sp)
	lw	$3,4($2)
	lui	$2,%hi($LC3)
	addiu	$4,$2,%lo($LC3)
	move	$6,$3
	lui	$2,%hi(panic)
	addiu	$2,$2,%lo(panic)
	jalr	$2
	nop

$L51:
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

	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	addiu	$2,$2,468
	move	$4,$2
	li	$5,1			# 0x1
	li	$6,1			# 0x1
	move	$7,$0
	lui	$2,%hi(__wake_up)
	addiu	$2,$2,%lo(__wake_up)
	jalr	$2
	nop

	lw	$31,244($sp)
	addiu	$sp,$sp,248
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	handle_xpon_daemon_job_gpon_dev_reset
	.size	handle_xpon_daemon_job_gpon_dev_reset, .-handle_xpon_daemon_job_gpon_dev_reset
	.rdata
	.align	2
$LC6:
	.ascii	"[%s]job id not recognized: %d\012\000"
	.section	.text.xpon_daemon_job_dispatch,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	xpon_daemon_job_dispatch
	.type	xpon_daemon_job_dispatch, @function
xpon_daemon_job_dispatch:
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
	lw	$2,0($2)
	li	$3,1			# 0x1
	beq	$2,$3,$L57
	nop

	sltu	$3,$2,1
	bne	$3,$0,$L58
	nop

	li	$3,2			# 0x2
	beq	$2,$3,$L64
	nop

	li	$3,4			# 0x4
	beq	$2,$3,$L60
	nop

	j	$L63
	nop

$L58:
	lui	$2,%hi(handle_xpon_daemon_job_get_phy_params)
	addiu	$2,$2,%lo(handle_xpon_daemon_job_get_phy_params)
	jalr	$2
	nop

	j	$L61
	nop

$L57:
	lui	$2,%hi(handle_xpon_daemon_job_gpon_dev_reset)
	addiu	$2,$2,%lo(handle_xpon_daemon_job_gpon_dev_reset)
	jalr	$2
	nop

	j	$L61
	nop

$L60:
	li	$2,1			# 0x1
	sw	$2,16($sp)
	j	$L61
	nop

$L63:
	lui	$2,%hi(dump_stack)
	addiu	$2,$2,%lo(dump_stack)
	jalr	$2
	nop

	lw	$2,32($sp)
	lw	$3,0($2)
	lui	$2,%hi($LC6)
	addiu	$4,$2,%lo($LC6)
	lui	$2,%hi(__func__.33605)
	addiu	$5,$2,%lo(__func__.33605)
	move	$6,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	li	$2,2			# 0x2
	sw	$2,16($sp)
	j	$L61
	nop

$L64:
	nop
$L61:
	lw	$2,16($sp)
	lw	$31,28($sp)
	addiu	$sp,$sp,32
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	xpon_daemon_job_dispatch
	.size	xpon_daemon_job_dispatch, .-xpon_daemon_job_dispatch
	.section	.text.xpon_daemon,"ax",@progbits
	.align	2
	.globl	xpon_daemon
	.set	nomips16
	.set	nomicromips
	.ent	xpon_daemon
	.type	xpon_daemon, @function
xpon_daemon:
	.frame	$sp,296,$31		# vars= 272, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-296
	sw	$31,292($sp)
	sw	$4,296($sp)
	addiu	$3,$sp,64
	li	$2,192			# 0xc0
	move	$4,$3
	move	$5,$0
	move	$6,$2
	lui	$2,%hi(memset)
	addiu	$2,$2,%lo(memset)
	jalr	$2
	nop

	sw	$0,16($sp)
	sw	$0,20($sp)
	j	$L66
	nop

$L90:
	sw	$0,24($sp)
	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,34848			# 0x8820
	addu	$2,$3,$2
	sw	$2,28($sp)
	lw	$2,28($sp)
	lw	$2,264($2)
	lw	$3,28($sp)
	sll	$2,$2,4
	addu	$2,$3,$2
	lbu	$2,12($2)
	andi	$2,$2,0x00ff
	xori	$2,$2,0x1
	sltu	$2,$2,1
	andi	$2,$2,0x00ff
	bne	$2,$0,$L68
	nop

	sw	$0,268($sp)
	sw	$0,272($sp)
	sw	$0,276($sp)
	sw	$0,280($sp)
	sw	$0,284($sp)
	move	$2,$28
	lw	$2,0($2)
	sw	$2,272($sp)
	lui	$2,%hi(autoremove_wake_function)
	addiu	$2,$2,%lo(autoremove_wake_function)
	sw	$2,276($sp)
	addiu	$2,$sp,268
	addiu	$2,$2,12
	sw	$2,280($sp)
	addiu	$2,$sp,268
	addiu	$2,$2,12
	sw	$2,284($sp)
$L78:
	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,34832			# 0x8810
	addu	$3,$3,$2
	addiu	$2,$sp,268
	move	$4,$3
	move	$5,$2
	li	$6,1			# 0x1
	lui	$2,%hi(prepare_to_wait)
	addiu	$2,$2,%lo(prepare_to_wait)
	jalr	$2
	nop

	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,34848			# 0x8820
	addu	$2,$3,$2
	sw	$2,32($sp)
	lw	$2,32($sp)
	lw	$2,264($2)
	lw	$3,32($sp)
	sll	$2,$2,4
	addu	$2,$3,$2
	lbu	$2,12($2)
	andi	$2,$2,0x00ff
	xori	$2,$2,0x1
	sltu	$2,$2,1
	andi	$2,$2,0x00ff
	beq	$2,$0,$L70
	nop

	j	$L92
	nop

$L70:
	move	$2,$28
	lw	$2,0($2)
	sw	$2,36($sp)
	lw	$2,36($sp)
	sw	$2,40($sp)
	li	$2,1			# 0x1
	sw	$2,44($sp)
	lw	$2,40($sp)
	lw	$2,4($2)
	sw	$2,48($sp)
	lw	$2,44($sp)
	sw	$2,52($sp)
	lw	$2,48($sp)
	addiu	$2,$2,8
	lw	$3,52($sp)
	sw	$3,56($sp)
	sw	$2,60($sp)
	lw	$2,56($sp)
	bgez	$2,$L72
	nop

	addiu	$2,$2,31
$L72:
	sra	$2,$2,5
	sll	$2,$2,2
	lw	$3,60($sp)
	addu	$2,$3,$2
	lw	$3,0($2)
	lw	$2,56($sp)
	andi	$2,$2,0x1f
	srl	$2,$3,$2
	andi	$2,$2,0x1
	sltu	$2,$0,$2
	andi	$2,$2,0x00ff
	bne	$2,$0,$L77
	nop

	lui	$2,%hi(schedule)
	addiu	$2,$2,%lo(schedule)
	jalr	$2
	nop

	j	$L78
	nop

$L77:
	li	$2,-512			# 0xfffffffffffffe00
	sw	$2,24($sp)
$L92:
	nop
	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,34832			# 0x8810
	addu	$3,$3,$2
	addiu	$2,$sp,268
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(finish_wait)
	addiu	$2,$2,%lo(finish_wait)
	jalr	$2
	nop

$L68:
	sw	$0,20($sp)
	j	$L79
	nop

$L85:
	sw	$0,16($sp)
	j	$L80
	nop

$L83:
	lw	$3,260($sp)
	lw	$2,16($sp)
	sll	$2,$2,2
	sll	$4,$2,2
	subu	$2,$4,$2
	addiu	$4,$sp,16
	addu	$2,$4,$2
	lw	$2,52($2)
	sltu	$2,$2,$3
	beq	$2,$0,$L81
	nop

	j	$L82
	nop

$L81:
	lw	$2,16($sp)
	addiu	$2,$2,1
	sw	$2,16($sp)
$L80:
	lw	$3,16($sp)
	lw	$2,20($sp)
	sltu	$2,$3,$2
	bne	$2,$0,$L83
	nop

$L82:
	lw	$2,16($sp)
	addiu	$2,$2,1
	sll	$2,$2,2
	sll	$3,$2,2
	subu	$2,$3,$2
	addiu	$3,$sp,64
	addu	$4,$3,$2
	lw	$2,16($sp)
	sll	$2,$2,2
	sll	$3,$2,2
	subu	$2,$3,$2
	addiu	$3,$sp,64
	addu	$5,$3,$2
	lw	$3,20($sp)
	lw	$2,16($sp)
	subu	$2,$3,$2
	sll	$2,$2,2
	sll	$3,$2,2
	subu	$2,$3,$2
	move	$6,$2
	lui	$2,%hi(memmove)
	addiu	$2,$2,%lo(memmove)
	jalr	$2
	nop

	lw	$2,16($sp)
	sll	$2,$2,2
	sll	$3,$2,2
	subu	$2,$3,$2
	addiu	$3,$sp,16
	addu	$2,$3,$2
	lw	$5,256($sp)
	lw	$4,260($sp)
	lw	$3,264($sp)
	sw	$5,48($2)
	sw	$4,52($2)
	sw	$3,56($2)
	lw	$2,20($sp)
	addiu	$2,$2,1
	sw	$2,20($sp)
$L79:
	lw	$2,20($sp)
	sltu	$2,$2,16
	beq	$2,$0,$L84
	nop

	addiu	$2,$sp,256
	move	$4,$2
	lui	$2,%hi(xpon_daemon_job_dequeue)
	addiu	$2,$2,%lo(xpon_daemon_job_dequeue)
	jalr	$2
	nop

	move	$3,$2
	li	$2,1			# 0x1
	beq	$3,$2,$L85
	nop

$L84:
	sw	$0,16($sp)
	j	$L86
	nop

$L89:
	addiu	$4,$sp,64
	lw	$2,16($sp)
	move	$3,$2
	sll	$2,$3,2
	move	$3,$2
	sll	$2,$3,2
	subu	$2,$2,$3
	addu	$2,$4,$2
	move	$4,$2
	lui	$2,%hi(xpon_daemon_job_dispatch)
	addiu	$2,$2,%lo(xpon_daemon_job_dispatch)
	jalr	$2
	nop

	beq	$2,$0,$L87
	nop

	move	$2,$0
	j	$L91
	nop

$L87:
	lw	$2,16($sp)
	addiu	$2,$2,1
	sw	$2,16($sp)
$L86:
	lw	$3,16($sp)
	lw	$2,20($sp)
	sltu	$2,$3,$2
	bne	$2,$0,$L89
	nop

$L66:
	lui	$2,%hi(kthread_should_stop)
	addiu	$2,$2,%lo(kthread_should_stop)
	jalr	$2
	nop

	beq	$2,$0,$L90
	nop

	move	$2,$0
$L91:
	lw	$31,292($sp)
	addiu	$sp,$sp,296
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	xpon_daemon
	.size	xpon_daemon, .-xpon_daemon
	.rdata
	.align	2
	.type	__func__.33582, @object
	.size	__func__.33582, 38
__func__.33582:
	.ascii	"handle_xpon_daemon_job_get_phy_params\000"
	.align	2
	.type	__func__.33591, @object
	.size	__func__.33591, 38
__func__.33591:
	.ascii	"handle_xpon_daemon_job_gpon_dev_reset\000"
	.align	2
	.type	__func__.33605, @object
	.size	__func__.33605, 25
__func__.33605:
	.ascii	"xpon_daemon_job_dispatch\000"
	.ident	"GCC: (Buildroot 2015.08.1) 4.9.3"
