	.file	1 "epon_mpcp.c"
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
	.globl	gDiscoveryGateMaxCnt
	.data
	.align	2
	.type	gDiscoveryGateMaxCnt, @object
	.size	gDiscoveryGateMaxCnt, 4
gDiscoveryGateMaxCnt:
	.word	100
	.globl	gDiscoveryGateCnt
	.section	.bss,"aw",@nobits
	.align	2
	.type	gDiscoveryGateCnt, @object
	.size	gDiscoveryGateCnt, 4
gDiscoveryGateCnt:
	.space	4
	.globl	gFeResetRetryCnt
	.align	2
	.type	gFeResetRetryCnt, @object
	.size	gFeResetRetryCnt, 4
gFeResetRetryCnt:
	.space	4
	.globl	gMpcpSynctime
	.data
	.align	1
	.type	gMpcpSynctime, @object
	.size	gMpcpSynctime, 2
gMpcpSynctime:
	.half	32
	.globl	mpcpDscvGateLock
	.section	.bss
	.align	2
	.type	mpcpDscvGateLock, @object
	.size	mpcpDscvGateLock, 4
mpcpDscvGateLock:
	.space	4
	.globl	mpcpDscvPendingLock
	.align	2
	.type	mpcpDscvPendingLock, @object
	.size	mpcpDscvPendingLock, 4
mpcpDscvPendingLock:
	.space	4
	.globl	isAuthFail
	.align	2
	.type	isAuthFail, @object
	.size	isAuthFail, 4
isAuthFail:
	.space	4
	.globl	isSlientBegin
	.align	2
	.type	isSlientBegin, @object
	.size	isSlientBegin, 4
isSlientBegin:
	.space	4
	.globl	g_silence_time
	.data
	.type	g_silence_time, @object
	.size	g_silence_time, 1
g_silence_time:
	.byte	60
	.rdata
	.align	2
$LC0:
	.ascii	"%d\012\000"
	.section	.text.max_dscv_gate_read_proc,"ax",@progbits
	.align	2
	.globl	max_dscv_gate_read_proc
	.set	nomips16
	.set	nomicromips
	.ent	max_dscv_gate_read_proc
	.type	max_dscv_gate_read_proc, @function
max_dscv_gate_read_proc:
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
	sw	$0,20($sp)
	lui	$2,%hi(gDiscoveryGateMaxCnt)
	lw	$3,%lo(gDiscoveryGateMaxCnt)($2)
	lw	$4,32($sp)
	lui	$2,%hi($LC0)
	addiu	$5,$2,%lo($LC0)
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
	beq	$2,$0,$L2
	nop

	lw	$2,44($sp)
	sw	$2,16($sp)
	j	$L3
	nop

$L2:
	lw	$2,48($sp)
	li	$3,1			# 0x1
	sw	$3,0($2)
$L3:
	lw	$2,16($sp)
	bgez	$2,$L4
	nop

	sw	$0,16($sp)
$L4:
	lw	$2,16($sp)
	lw	$31,28($sp)
	addiu	$sp,$sp,32
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	max_dscv_gate_read_proc
	.size	max_dscv_gate_read_proc, .-max_dscv_gate_read_proc
	.rdata
	.align	2
$LC1:
	.ascii	"%d\000"
	.section	.text.max_dscv_gate_write_proc,"ax",@progbits
	.align	2
	.globl	max_dscv_gate_write_proc
	.set	nomips16
	.set	nomicromips
	.ent	max_dscv_gate_write_proc
	.type	max_dscv_gate_write_proc, @function
max_dscv_gate_write_proc:
	.frame	$sp,64,$31		# vars= 40, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	addiu	$sp,$sp,-64
	sw	$31,60($sp)
	sw	$4,64($sp)
	sw	$5,68($sp)
	sw	$6,72($sp)
	sw	$7,76($sp)
	lw	$2,72($sp)
	sltu	$2,$2,8
	bne	$2,$0,$L7
	li	$2,-22			# 0xffffffffffffffea
	j	$L12
$L7:
	addiu	$2,$sp,44
	sw	$2,20($sp)
	lw	$2,68($sp)
	sw	$2,24($sp)
	lw	$2,72($sp)
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
	beq	$2,$0,$L9
	lw	$4,20($sp)
	lw	$5,24($sp)
	lw	$6,16($sp)
#APP
 # 158 "/opt/tclinux_phoenix/modules/private/xpon/src/epon/epon_mpcp.c" 1
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
$L9:
	lw	$2,16($sp)
	beq	$2,$0,$L10
	li	$2,-14			# 0xfffffffffffffff2
	j	$L12
$L10:
	lw	$2,72($sp)
	addiu	$3,$sp,16
	addu	$2,$3,$2
	sb	$0,28($2)
	addiu	$2,$sp,44
	addiu	$3,$sp,52
	move	$4,$2
	lui	$2,%hi($LC1)
	addiu	$5,$2,%lo($LC1)
	move	$6,$3
	lui	$2,%hi(sscanf)
	addiu	$2,$2,%lo(sscanf)
	jalr	$2
	lw	$2,52($sp)
	beq	$2,$0,$L11
	lw	$3,52($sp)
	lui	$2,%hi(gDiscoveryGateMaxCnt)
	sw	$3,%lo(gDiscoveryGateMaxCnt)($2)
$L11:
	lui	$2,%hi(gFeResetRetryCnt)
	sw	$0,%lo(gFeResetRetryCnt)($2)
	lui	$2,%hi(gDiscoveryGateCnt)
	sw	$0,%lo(gDiscoveryGateCnt)($2)
	lw	$2,72($sp)
$L12:
	lw	$31,60($sp)
	addiu	$sp,$sp,64
	j	$31
	.end	max_dscv_gate_write_proc
	.size	max_dscv_gate_write_proc, .-max_dscv_gate_write_proc
	.rdata
	.align	2
$LC2:
	.ascii	"eponMacCheckRegreqDoneBit done\000"
	.align	2
$LC3:
	.ascii	"ERR: eponMacCheckRegreqDoneBit not ready\000"
	.section	.text.eponMacCheckRegreqDoneBit,"ax",@progbits
	.align	2
	.globl	eponMacCheckRegreqDoneBit
	.set	nomips16
	.set	nomicromips
	.ent	eponMacCheckRegreqDoneBit
	.type	eponMacCheckRegreqDoneBit, @function
eponMacCheckRegreqDoneBit:
	.frame	$sp,32,$31		# vars= 8, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-32
	sw	$31,28($sp)
	lui	$2,%hi(g_EPON_MAC_BASE)
	lw	$2,%lo(g_EPON_MAC_BASE)($2)
	addiu	$2,$2,24616
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	sw	$2,16($sp)
	lw	$3,16($sp)
	li	$2,65536			# 0x10000
	and	$2,$3,$2
	beq	$2,$0,$L14
	nop

	li	$4,4			# 0x4
	lui	$2,%hi($LC2)
	addiu	$5,$2,%lo($LC2)
	lui	$2,%hi(dbgtoMem)
	addiu	$2,$2,%lo(dbgtoMem)
	jalr	$2
	nop

	move	$2,$0
	j	$L16
	nop

$L14:
	li	$4,1			# 0x1
	lui	$2,%hi($LC3)
	addiu	$5,$2,%lo($LC3)
	lui	$2,%hi(dbgtoMem)
	addiu	$2,$2,%lo(dbgtoMem)
	jalr	$2
	nop

	li	$2,-1			# 0xffffffffffffffff
$L16:
	lw	$31,28($sp)
	addiu	$sp,$sp,32
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	eponMacCheckRegreqDoneBit
	.size	eponMacCheckRegreqDoneBit, .-eponMacCheckRegreqDoneBit
	.section	.text.eponGetRandomRetryTimes,"ax",@progbits
	.align	2
	.globl	eponGetRandomRetryTimes
	.set	nomips16
	.set	nomicromips
	.ent	eponGetRandomRetryTimes
	.type	eponGetRandomRetryTimes, @function
eponGetRandomRetryTimes:
	.frame	$sp,32,$31		# vars= 8, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-32
	sw	$31,28($sp)
	sw	$0,16($sp)
	addiu	$2,$sp,16
	move	$4,$2
	li	$5,4			# 0x4
	lui	$2,%hi(get_random_bytes)
	addiu	$2,$2,%lo(get_random_bytes)
	jalr	$2
	nop

	lw	$2,16($sp)
	andi	$2,$2,0x00ff
	andi	$2,$2,0x7
	andi	$2,$2,0x00ff
	addiu	$2,$2,1
	andi	$2,$2,0x00ff
	lw	$31,28($sp)
	addiu	$sp,$sp,32
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	eponGetRandomRetryTimes
	.size	eponGetRandomRetryTimes, .-eponGetRandomRetryTimes
	.globl	mpcpTmOutCnt
	.section	.bss
	.align	2
	.type	mpcpTmOutCnt, @object
	.size	mpcpTmOutCnt, 4
mpcpTmOutCnt:
	.space	4
	.rdata
	.align	2
$LC4:
	.ascii	"enter eponMpcpDiscvGateIntHandler gMpcpSynctime =0x%x\000"
	.align	2
$LC5:
	.ascii	"enter %s:sync_time == 0, set __MPCPSyncTime = 0x%x\000"
	.align	2
$LC6:
	.ascii	"eponMpcpDiscvGateIntHandler llidIndex=%d state=%d\000"
	.align	2
$LC7:
	.ascii	"MPCP DiscState is REGISTER_REQUEST\000"
	.align	2
$LC8:
	.ascii	"eponMpcpDiscvGateIntHandler gDiscoveryGateCnt=%d,gFeRese"
	.ascii	"tRetryCnt=%d\000"
	.align	2
$LC9:
	.ascii	"Epon MPCP DiscvGate IntHandler done bit not ready\000"
	.align	2
$LC10:
	.ascii	"eponMpcpDiscvGateIntHandler READ_REG_WORD(e_llid_dscvry_"
	.ascii	"ctrl)\000"
	.align	2
$LC11:
	.ascii	"eponMpcpDiscvGateIntHandler enter while dscvCtrl.Raw=%08"
	.ascii	"X\000"
	.align	2
$LC12:
	.ascii	"EponIsr Receive DiscvGate Message\000"
	.align	2
$LC13:
	.ascii	"eponMpcpDiscvGateIntHandler done bit timeout\000"
	.align	2
$LC14:
	.ascii	"Epon Send MPCP REGISTER_REQUEST DiscvGate Finished\000"
	.section	.text.eponMpcpDiscvGateIntHandler,"ax",@progbits
	.align	2
	.globl	eponMpcpDiscvGateIntHandler
	.set	nomips16
	.set	nomicromips
	.ent	eponMpcpDiscvGateIntHandler
	.type	eponMpcpDiscvGateIntHandler, @function
eponMpcpDiscvGateIntHandler:
	.frame	$sp,360,$31		# vars= 336, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-360
	sw	$31,356($sp)
	sw	$4,360($sp)
	sb	$0,16($sp)
	lui	$2,%hi(eponDrv+20)
	addiu	$2,$2,%lo(eponDrv+20)
	sw	$2,20($sp)
	sw	$0,24($sp)
	sw	$0,28($sp)
	sw	$0,32($sp)
	lw	$2,20($sp)
	addiu	$2,$2,20
	sw	$2,36($sp)
	sw	$0,324($sp)
	lui	$2,%hi(gpPonSysData)
	lw	$2,%lo(gpPonSysData)($2)
	lw	$3,0($2)
	li	$2,2			# 0x2
	beq	$3,$2,$L20
	nop

	j	$L19
	nop

$L20:
	lui	$2,%hi(gMpcpSynctime)
	lhu	$2,%lo(gMpcpSynctime)($2)
	move	$3,$2
	li	$4,4			# 0x4
	lui	$2,%hi($LC4)
	addiu	$5,$2,%lo($LC4)
	move	$6,$3
	lui	$2,%hi(dbgtoMem)
	addiu	$2,$2,%lo(dbgtoMem)
	jalr	$2
	nop

	lui	$2,%hi(g_EPON_MAC_BASE)
	lw	$2,%lo(g_EPON_MAC_BASE)($2)
	addiu	$2,$2,24788
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	sw	$2,336($sp)
	lhu	$2,338($sp)
	move	$4,$2
	li	$2,-1078984704			# 0xffffffffbfb00000
	ori	$2,$2,0x64
	sw	$2,40($sp)
	lw	$2,40($sp)
	lw	$2,0($2)
	andi	$3,$2,0xf
	li	$2,3			# 0x3
	bne	$3,$2,$L23
	nop

	li	$2,-1078657024			# 0xffffffffbfb50000
	ori	$2,$2,0xfffc
	sw	$2,44($sp)
	lw	$2,44($sp)
	lw	$2,0($2)
	andi	$2,$2,0xf
	beq	$2,$0,$L23
	nop

	li	$2,-1078984704			# 0xffffffffbfb00000
	ori	$2,$2,0xf8
	sw	$2,48($sp)
	lw	$2,48($sp)
	lw	$2,0($2)
	andi	$3,$2,0x3
	li	$2,2			# 0x2
	bne	$3,$2,$L26
	nop

	li	$2,-1078984704			# 0xffffffffbfb00000
	ori	$2,$2,0x64
	sw	$2,52($sp)
	lw	$2,52($sp)
	lw	$3,0($2)
	li	$2,-65536			# 0xffffffffffff0000
	and	$3,$3,$2
	li	$2,327680			# 0x50000
	bne	$3,$2,$L26
	nop

	li	$2,-1078001664			# 0xffffffffbfbf0000
	ori	$2,$2,0x8214
	sw	$2,56($sp)
	lw	$2,56($sp)
	lw	$2,0($2)
	andi	$2,$2,0x8
	beq	$2,$0,$L29
	nop

	li	$2,-1078001664			# 0xffffffffbfbf0000
	ori	$2,$2,0x8214
	sw	$2,60($sp)
	lw	$2,60($sp)
	lw	$2,0($2)
	andi	$2,$2,0x20
	bne	$2,$0,$L31
	nop

	j	$L26
	nop

$L29:
	li	$2,-1078001664			# 0xffffffffbfbf0000
	ori	$2,$2,0x8214
	sw	$2,64($sp)
	lw	$2,64($sp)
	lw	$2,0($2)
	andi	$2,$2,0x2
	beq	$2,$0,$L26
	nop

$L31:
	li	$2,-1078001664			# 0xffffffffbfbf0000
	ori	$2,$2,0x8214
	sw	$2,68($sp)
	lw	$2,68($sp)
	lw	$2,0($2)
	andi	$2,$2,0x8
	beq	$2,$0,$L34
	nop

	li	$2,-1078001664			# 0xffffffffbfbf0000
	ori	$2,$2,0x8214
	sw	$2,72($sp)
	lw	$2,72($sp)
	lw	$2,0($2)
	andi	$2,$2,0x40
	beq	$2,$0,$L36
	nop

	j	$L26
	nop

$L34:
	li	$2,-1078001664			# 0xffffffffbfbf0000
	ori	$2,$2,0x8214
	sw	$2,76($sp)
	lw	$2,76($sp)
	lw	$2,0($2)
	andi	$2,$2,0x4
	beq	$2,$0,$L36
	nop

$L26:
	li	$2,-1078984704			# 0xffffffffbfb00000
	ori	$2,$2,0xf8
	sw	$2,80($sp)
	lw	$2,80($sp)
	lw	$2,0($2)
	andi	$3,$2,0x3
	li	$2,3			# 0x3
	bne	$3,$2,$L39
	nop

	li	$2,-1078984704			# 0xffffffffbfb00000
	ori	$2,$2,0x64
	sw	$2,84($sp)
	lw	$2,84($sp)
	lw	$3,0($2)
	li	$2,-65536			# 0xffffffffffff0000
	and	$3,$3,$2
	li	$2,327680			# 0x50000
	bne	$3,$2,$L39
	nop

	li	$2,-1078001664			# 0xffffffffbfbf0000
	ori	$2,$2,0x8214
	sw	$2,88($sp)
	lw	$2,88($sp)
	lw	$2,0($2)
	andi	$2,$2,0x8
	beq	$2,$0,$L42
	nop

	li	$2,-1078001664			# 0xffffffffbfbf0000
	ori	$2,$2,0x8214
	sw	$2,92($sp)
	lw	$2,92($sp)
	lw	$2,0($2)
	andi	$2,$2,0x20
	bne	$2,$0,$L36
	nop

	j	$L39
	nop

$L42:
	li	$2,-1078001664			# 0xffffffffbfbf0000
	ori	$2,$2,0x8214
	sw	$2,96($sp)
	lw	$2,96($sp)
	lw	$2,0($2)
	andi	$2,$2,0x2
	bne	$2,$0,$L36
	nop

$L39:
	li	$2,-1078984704			# 0xffffffffbfb00000
	ori	$2,$2,0xf8
	sw	$2,100($sp)
	lw	$2,100($sp)
	lw	$2,0($2)
	andi	$3,$2,0x3
	li	$2,2			# 0x2
	bne	$3,$2,$L46
	nop

	li	$2,-1078984704			# 0xffffffffbfb00000
	ori	$2,$2,0x64
	sw	$2,104($sp)
	lw	$2,104($sp)
	lw	$3,0($2)
	li	$2,-65536			# 0xffffffffffff0000
	and	$3,$3,$2
	li	$2,327680			# 0x50000
	bne	$3,$2,$L46
	nop

	li	$2,-1078001664			# 0xffffffffbfbf0000
	ori	$2,$2,0x8214
	sw	$2,108($sp)
	lw	$2,108($sp)
	lw	$2,0($2)
	andi	$2,$2,0x8
	beq	$2,$0,$L49
	nop

	li	$2,-1078001664			# 0xffffffffbfbf0000
	ori	$2,$2,0x8214
	sw	$2,112($sp)
	lw	$2,112($sp)
	lw	$2,0($2)
	andi	$2,$2,0x20
	beq	$2,$0,$L51
	nop

	j	$L46
	nop

$L49:
	li	$2,-1078001664			# 0xffffffffbfbf0000
	ori	$2,$2,0x8214
	sw	$2,116($sp)
	lw	$2,116($sp)
	lw	$2,0($2)
	andi	$2,$2,0x2
	bne	$2,$0,$L46
	nop

$L51:
	li	$2,-1078001664			# 0xffffffffbfbf0000
	ori	$2,$2,0x8214
	sw	$2,120($sp)
	lw	$2,120($sp)
	lw	$2,0($2)
	andi	$2,$2,0x8
	beq	$2,$0,$L54
	nop

	li	$2,-1078001664			# 0xffffffffbfbf0000
	ori	$2,$2,0x8214
	sw	$2,124($sp)
	lw	$2,124($sp)
	lw	$2,0($2)
	andi	$2,$2,0x40
	beq	$2,$0,$L36
	nop

	j	$L46
	nop

$L54:
	li	$2,-1078001664			# 0xffffffffbfbf0000
	ori	$2,$2,0x8214
	sw	$2,128($sp)
	lw	$2,128($sp)
	lw	$2,0($2)
	andi	$2,$2,0x4
	beq	$2,$0,$L36
	nop

$L46:
	li	$2,-1078984704			# 0xffffffffbfb00000
	ori	$2,$2,0xf8
	sw	$2,132($sp)
	lw	$2,132($sp)
	lw	$2,0($2)
	andi	$3,$2,0x3
	li	$2,3			# 0x3
	bne	$3,$2,$L58
	nop

	li	$2,-1078984704			# 0xffffffffbfb00000
	ori	$2,$2,0x64
	sw	$2,136($sp)
	lw	$2,136($sp)
	lw	$3,0($2)
	li	$2,-65536			# 0xffffffffffff0000
	and	$3,$3,$2
	li	$2,327680			# 0x50000
	bne	$3,$2,$L58
	nop

	li	$2,-1078001664			# 0xffffffffbfbf0000
	ori	$2,$2,0x8214
	sw	$2,140($sp)
	lw	$2,140($sp)
	lw	$2,0($2)
	andi	$2,$2,0x8
	beq	$2,$0,$L61
	nop

	li	$2,-1078001664			# 0xffffffffbfbf0000
	ori	$2,$2,0x8214
	sw	$2,144($sp)
	lw	$2,144($sp)
	lw	$2,0($2)
	andi	$2,$2,0x20
	beq	$2,$0,$L36
	nop

	j	$L58
	nop

$L61:
	li	$2,-1078001664			# 0xffffffffbfbf0000
	ori	$2,$2,0x8214
	sw	$2,148($sp)
	lw	$2,148($sp)
	lw	$2,0($2)
	andi	$2,$2,0x2
	beq	$2,$0,$L36
	nop

$L58:
	li	$2,-1078984704			# 0xffffffffbfb00000
	ori	$2,$2,0xf8
	sw	$2,152($sp)
	lw	$2,152($sp)
	lw	$2,0($2)
	andi	$3,$2,0x3
	li	$2,2			# 0x2
	bne	$3,$2,$L23
	nop

	li	$2,-1078984704			# 0xffffffffbfb00000
	ori	$2,$2,0x64
	sw	$2,156($sp)
	lw	$2,156($sp)
	lw	$3,0($2)
	li	$2,-65536			# 0xffffffffffff0000
	and	$3,$3,$2
	li	$2,327680			# 0x50000
	bne	$3,$2,$L23
	nop

	li	$2,-1078001664			# 0xffffffffbfbf0000
	ori	$2,$2,0x8214
	sw	$2,160($sp)
	lw	$2,160($sp)
	lw	$2,0($2)
	andi	$2,$2,0x8
	beq	$2,$0,$L67
	nop

	li	$2,-1078001664			# 0xffffffffbfbf0000
	ori	$2,$2,0x8214
	sw	$2,164($sp)
	lw	$2,164($sp)
	lw	$2,0($2)
	andi	$2,$2,0x40
	bne	$2,$0,$L36
	nop

	j	$L23
	nop

$L67:
	li	$2,-1078001664			# 0xffffffffbfbf0000
	ori	$2,$2,0x8214
	sw	$2,168($sp)
	lw	$2,168($sp)
	lw	$2,0($2)
	andi	$2,$2,0x4
	beq	$2,$0,$L23
	nop

$L36:
	li	$2,64			# 0x40
	j	$L70
	nop

$L23:
	li	$2,95			# 0x5f
$L70:
	slt	$2,$2,$4
	beq	$2,$0,$L71
	nop

	li	$2,-1078984704			# 0xffffffffbfb00000
	ori	$2,$2,0x64
	sw	$2,172($sp)
	lw	$2,172($sp)
	lw	$2,0($2)
	andi	$3,$2,0xf
	li	$2,3			# 0x3
	bne	$3,$2,$L73
	nop

	li	$2,-1078657024			# 0xffffffffbfb50000
	ori	$2,$2,0xfffc
	sw	$2,176($sp)
	lw	$2,176($sp)
	lw	$2,0($2)
	andi	$2,$2,0xf
	beq	$2,$0,$L73
	nop

	li	$2,-1078984704			# 0xffffffffbfb00000
	ori	$2,$2,0xf8
	sw	$2,180($sp)
	lw	$2,180($sp)
	lw	$2,0($2)
	andi	$3,$2,0x3
	li	$2,2			# 0x2
	bne	$3,$2,$L76
	nop

	li	$2,-1078984704			# 0xffffffffbfb00000
	ori	$2,$2,0x64
	sw	$2,184($sp)
	lw	$2,184($sp)
	lw	$3,0($2)
	li	$2,-65536			# 0xffffffffffff0000
	and	$3,$3,$2
	li	$2,327680			# 0x50000
	bne	$3,$2,$L76
	nop

	li	$2,-1078001664			# 0xffffffffbfbf0000
	ori	$2,$2,0x8214
	sw	$2,188($sp)
	lw	$2,188($sp)
	lw	$2,0($2)
	andi	$2,$2,0x8
	beq	$2,$0,$L79
	nop

	li	$2,-1078001664			# 0xffffffffbfbf0000
	ori	$2,$2,0x8214
	sw	$2,192($sp)
	lw	$2,192($sp)
	lw	$2,0($2)
	andi	$2,$2,0x20
	bne	$2,$0,$L81
	nop

	j	$L76
	nop

$L79:
	li	$2,-1078001664			# 0xffffffffbfbf0000
	ori	$2,$2,0x8214
	sw	$2,196($sp)
	lw	$2,196($sp)
	lw	$2,0($2)
	andi	$2,$2,0x2
	beq	$2,$0,$L76
	nop

$L81:
	li	$2,-1078001664			# 0xffffffffbfbf0000
	ori	$2,$2,0x8214
	sw	$2,200($sp)
	lw	$2,200($sp)
	lw	$2,0($2)
	andi	$2,$2,0x8
	beq	$2,$0,$L84
	nop

	li	$2,-1078001664			# 0xffffffffbfbf0000
	ori	$2,$2,0x8214
	sw	$2,204($sp)
	lw	$2,204($sp)
	lw	$2,0($2)
	andi	$2,$2,0x40
	beq	$2,$0,$L86
	nop

	j	$L76
	nop

$L84:
	li	$2,-1078001664			# 0xffffffffbfbf0000
	ori	$2,$2,0x8214
	sw	$2,208($sp)
	lw	$2,208($sp)
	lw	$2,0($2)
	andi	$2,$2,0x4
	beq	$2,$0,$L86
	nop

$L76:
	li	$2,-1078984704			# 0xffffffffbfb00000
	ori	$2,$2,0xf8
	sw	$2,212($sp)
	lw	$2,212($sp)
	lw	$2,0($2)
	andi	$3,$2,0x3
	li	$2,3			# 0x3
	bne	$3,$2,$L89
	nop

	li	$2,-1078984704			# 0xffffffffbfb00000
	ori	$2,$2,0x64
	sw	$2,216($sp)
	lw	$2,216($sp)
	lw	$3,0($2)
	li	$2,-65536			# 0xffffffffffff0000
	and	$3,$3,$2
	li	$2,327680			# 0x50000
	bne	$3,$2,$L89
	nop

	li	$2,-1078001664			# 0xffffffffbfbf0000
	ori	$2,$2,0x8214
	sw	$2,220($sp)
	lw	$2,220($sp)
	lw	$2,0($2)
	andi	$2,$2,0x8
	beq	$2,$0,$L92
	nop

	li	$2,-1078001664			# 0xffffffffbfbf0000
	ori	$2,$2,0x8214
	sw	$2,224($sp)
	lw	$2,224($sp)
	lw	$2,0($2)
	andi	$2,$2,0x20
	bne	$2,$0,$L86
	nop

	j	$L89
	nop

$L92:
	li	$2,-1078001664			# 0xffffffffbfbf0000
	ori	$2,$2,0x8214
	sw	$2,228($sp)
	lw	$2,228($sp)
	lw	$2,0($2)
	andi	$2,$2,0x2
	bne	$2,$0,$L86
	nop

$L89:
	li	$2,-1078984704			# 0xffffffffbfb00000
	ori	$2,$2,0xf8
	sw	$2,232($sp)
	lw	$2,232($sp)
	lw	$2,0($2)
	andi	$3,$2,0x3
	li	$2,2			# 0x2
	bne	$3,$2,$L96
	nop

	li	$2,-1078984704			# 0xffffffffbfb00000
	ori	$2,$2,0x64
	sw	$2,236($sp)
	lw	$2,236($sp)
	lw	$3,0($2)
	li	$2,-65536			# 0xffffffffffff0000
	and	$3,$3,$2
	li	$2,327680			# 0x50000
	bne	$3,$2,$L96
	nop

	li	$2,-1078001664			# 0xffffffffbfbf0000
	ori	$2,$2,0x8214
	sw	$2,240($sp)
	lw	$2,240($sp)
	lw	$2,0($2)
	andi	$2,$2,0x8
	beq	$2,$0,$L99
	nop

	li	$2,-1078001664			# 0xffffffffbfbf0000
	ori	$2,$2,0x8214
	sw	$2,244($sp)
	lw	$2,244($sp)
	lw	$2,0($2)
	andi	$2,$2,0x20
	beq	$2,$0,$L101
	nop

	j	$L96
	nop

$L99:
	li	$2,-1078001664			# 0xffffffffbfbf0000
	ori	$2,$2,0x8214
	sw	$2,248($sp)
	lw	$2,248($sp)
	lw	$2,0($2)
	andi	$2,$2,0x2
	bne	$2,$0,$L96
	nop

$L101:
	li	$2,-1078001664			# 0xffffffffbfbf0000
	ori	$2,$2,0x8214
	sw	$2,252($sp)
	lw	$2,252($sp)
	lw	$2,0($2)
	andi	$2,$2,0x8
	beq	$2,$0,$L104
	nop

	li	$2,-1078001664			# 0xffffffffbfbf0000
	ori	$2,$2,0x8214
	sw	$2,256($sp)
	lw	$2,256($sp)
	lw	$2,0($2)
	andi	$2,$2,0x40
	beq	$2,$0,$L86
	nop

	j	$L96
	nop

$L104:
	li	$2,-1078001664			# 0xffffffffbfbf0000
	ori	$2,$2,0x8214
	sw	$2,260($sp)
	lw	$2,260($sp)
	lw	$2,0($2)
	andi	$2,$2,0x4
	beq	$2,$0,$L86
	nop

$L96:
	li	$2,-1078984704			# 0xffffffffbfb00000
	ori	$2,$2,0xf8
	sw	$2,264($sp)
	lw	$2,264($sp)
	lw	$2,0($2)
	andi	$3,$2,0x3
	li	$2,3			# 0x3
	bne	$3,$2,$L108
	nop

	li	$2,-1078984704			# 0xffffffffbfb00000
	ori	$2,$2,0x64
	sw	$2,268($sp)
	lw	$2,268($sp)
	lw	$3,0($2)
	li	$2,-65536			# 0xffffffffffff0000
	and	$3,$3,$2
	li	$2,327680			# 0x50000
	bne	$3,$2,$L108
	nop

	li	$2,-1078001664			# 0xffffffffbfbf0000
	ori	$2,$2,0x8214
	sw	$2,272($sp)
	lw	$2,272($sp)
	lw	$2,0($2)
	andi	$2,$2,0x8
	beq	$2,$0,$L111
	nop

	li	$2,-1078001664			# 0xffffffffbfbf0000
	ori	$2,$2,0x8214
	sw	$2,276($sp)
	lw	$2,276($sp)
	lw	$2,0($2)
	andi	$2,$2,0x20
	beq	$2,$0,$L86
	nop

	j	$L108
	nop

$L111:
	li	$2,-1078001664			# 0xffffffffbfbf0000
	ori	$2,$2,0x8214
	sw	$2,280($sp)
	lw	$2,280($sp)
	lw	$2,0($2)
	andi	$2,$2,0x2
	beq	$2,$0,$L86
	nop

$L108:
	li	$2,-1078984704			# 0xffffffffbfb00000
	ori	$2,$2,0xf8
	sw	$2,284($sp)
	lw	$2,284($sp)
	lw	$2,0($2)
	andi	$3,$2,0x3
	li	$2,2			# 0x2
	bne	$3,$2,$L73
	nop

	li	$2,-1078984704			# 0xffffffffbfb00000
	ori	$2,$2,0x64
	sw	$2,288($sp)
	lw	$2,288($sp)
	lw	$3,0($2)
	li	$2,-65536			# 0xffffffffffff0000
	and	$3,$3,$2
	li	$2,327680			# 0x50000
	bne	$3,$2,$L73
	nop

	li	$2,-1078001664			# 0xffffffffbfbf0000
	ori	$2,$2,0x8214
	sw	$2,292($sp)
	lw	$2,292($sp)
	lw	$2,0($2)
	andi	$2,$2,0x8
	beq	$2,$0,$L117
	nop

	li	$2,-1078001664			# 0xffffffffbfbf0000
	ori	$2,$2,0x8214
	sw	$2,296($sp)
	lw	$2,296($sp)
	lw	$2,0($2)
	andi	$2,$2,0x40
	bne	$2,$0,$L86
	nop

	j	$L73
	nop

$L117:
	li	$2,-1078001664			# 0xffffffffbfbf0000
	ori	$2,$2,0x8214
	sw	$2,300($sp)
	lw	$2,300($sp)
	lw	$2,0($2)
	andi	$2,$2,0x4
	beq	$2,$0,$L73
	nop

$L86:
	li	$2,64			# 0x40
	j	$L120
	nop

$L73:
	li	$2,95			# 0x5f
$L120:
	lui	$3,%hi(g_EPON_MAC_BASE)
	lw	$3,%lo(g_EPON_MAC_BASE)($3)
	addiu	$3,$3,24788
	move	$4,$2
	move	$5,$3
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	lui	$2,%hi(g_EPON_MAC_BASE)
	lw	$2,%lo(g_EPON_MAC_BASE)($2)
	addiu	$3,$2,24820
	li	$2,9306112			# 0x8e0000
	ori	$4,$2,0xfff1
	move	$5,$3
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	j	$L121
	nop

$L71:
	lhu	$2,338($sp)
	bne	$2,$0,$L122
	nop

	lui	$2,%hi(gMpcpSynctime)
	lhu	$2,%lo(gMpcpSynctime)($2)
	move	$3,$2
	lui	$2,%hi(g_EPON_MAC_BASE)
	lw	$2,%lo(g_EPON_MAC_BASE)($2)
	addiu	$2,$2,24788
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	lui	$2,%hi(g_EPON_MAC_BASE)
	lw	$2,%lo(g_EPON_MAC_BASE)($2)
	addiu	$2,$2,24788
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	sw	$2,336($sp)
	lui	$2,%hi(gMpcpSynctime)
	lhu	$2,%lo(gMpcpSynctime)($2)
	sll	$3,$2,16
	li	$2,3080192			# 0x2f0000
	ori	$2,$2,0xfff1
	addu	$2,$3,$2
	move	$3,$2
	lui	$2,%hi(g_EPON_MAC_BASE)
	lw	$2,%lo(g_EPON_MAC_BASE)($2)
	addiu	$2,$2,24820
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	lui	$2,%hi(gMpcpSynctime)
	lhu	$2,%lo(gMpcpSynctime)($2)
	move	$3,$2
	li	$4,4			# 0x4
	lui	$2,%hi($LC5)
	addiu	$5,$2,%lo($LC5)
	lui	$2,%hi(__func__.41647)
	addiu	$6,$2,%lo(__func__.41647)
	move	$7,$3
	lui	$2,%hi(dbgtoMem)
	addiu	$2,$2,%lo(dbgtoMem)
	jalr	$2
	nop

	j	$L121
	nop

$L122:
	lhu	$2,338($sp)
	sll	$3,$2,16
	li	$2,3080192			# 0x2f0000
	ori	$2,$2,0xfff1
	addu	$2,$3,$2
	move	$3,$2
	lui	$2,%hi(g_EPON_MAC_BASE)
	lw	$2,%lo(g_EPON_MAC_BASE)($2)
	addiu	$2,$2,24820
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	lhu	$3,338($sp)
	lui	$2,%hi(gMpcpSynctime)
	lhu	$2,%lo(gMpcpSynctime)($2)
	beq	$3,$2,$L121
	nop

	lhu	$3,338($sp)
	lui	$2,%hi(gMpcpSynctime)
	sh	$3,%lo(gMpcpSynctime)($2)
$L121:
	sb	$0,16($sp)
	j	$L123
	nop

$L143:
	lbu	$3,16($sp)
	lw	$2,20($sp)
	lbu	$2,20($2)
	move	$7,$2
	li	$4,4			# 0x4
	lui	$2,%hi($LC6)
	addiu	$5,$2,%lo($LC6)
	move	$6,$3
	lui	$2,%hi(dbgtoMem)
	addiu	$2,$2,%lo(dbgtoMem)
	jalr	$2
	nop

	lw	$2,20($sp)
	lbu	$2,1($2)
	bne	$2,$0,$L124
	nop

	j	$L125
	nop

$L124:
	lw	$2,20($sp)
	lbu	$3,20($2)
	li	$2,1			# 0x1
	beq	$3,$2,$L126
	nop

	lw	$2,20($sp)
	lbu	$3,20($2)
	li	$2,2			# 0x2
	bne	$3,$2,$L125
	nop

$L126:
	lw	$2,20($sp)
	lbu	$3,20($2)
	li	$2,2			# 0x2
	bne	$3,$2,$L127
	nop

	li	$4,2			# 0x2
	lui	$2,%hi($LC7)
	addiu	$5,$2,%lo($LC7)
	lui	$2,%hi(dbgtoMem)
	addiu	$2,$2,%lo(dbgtoMem)
	jalr	$2
	nop

$L127:
	lw	$2,20($sp)
	lbu	$2,22($2)
	beq	$2,$0,$L128
	nop

	lw	$2,20($sp)
	lbu	$2,22($2)
	addiu	$2,$2,-1
	andi	$3,$2,0x00ff
	lw	$2,20($sp)
	sb	$3,22($2)
	j	$L19
	nop

$L128:
	lui	$2,%hi(gDiscoveryGateCnt)
	lw	$2,%lo(gDiscoveryGateCnt)($2)
	addiu	$3,$2,1
	lui	$2,%hi(gDiscoveryGateCnt)
	sw	$3,%lo(gDiscoveryGateCnt)($2)
	lui	$2,%hi(gDiscoveryGateCnt)
	lw	$6,%lo(gDiscoveryGateCnt)($2)
	lui	$2,%hi(gFeResetRetryCnt)
	lw	$3,%lo(gFeResetRetryCnt)($2)
	li	$4,4			# 0x4
	lui	$2,%hi($LC8)
	addiu	$5,$2,%lo($LC8)
	move	$7,$3
	lui	$2,%hi(dbgtoMem)
	addiu	$2,$2,%lo(dbgtoMem)
	jalr	$2
	nop

	lui	$2,%hi(gDiscoveryGateCnt)
	lw	$3,%lo(gDiscoveryGateCnt)($2)
	lui	$2,%hi(gDiscoveryGateMaxCnt)
	lw	$2,%lo(gDiscoveryGateMaxCnt)($2)
	slt	$2,$3,$2
	bne	$2,$0,$L129
	nop

	lui	$2,%hi(gFeResetRetryCnt)
	lw	$2,%lo(gFeResetRetryCnt)($2)
	slt	$2,$2,3
	beq	$2,$0,$L130
	nop

	lui	$2,%hi(gFeResetRetryCnt)
	lw	$2,%lo(gFeResetRetryCnt)($2)
	addiu	$3,$2,1
	lui	$2,%hi(gFeResetRetryCnt)
	sw	$3,%lo(gFeResetRetryCnt)($2)
$L130:
	lui	$2,%hi(gDiscoveryGateCnt)
	sw	$0,%lo(gDiscoveryGateCnt)($2)
$L129:
	lui	$2,%hi(eponMacCheckRegreqDoneBit)
	addiu	$2,$2,%lo(eponMacCheckRegreqDoneBit)
	jalr	$2
	nop

	bgez	$2,$L131
	nop

	li	$4,4			# 0x4
	lui	$2,%hi($LC9)
	addiu	$5,$2,%lo($LC9)
	lui	$2,%hi(dbgtoMem)
	addiu	$2,$2,%lo(dbgtoMem)
	jalr	$2
	nop

	lui	$2,%hi(eponDebugLevel)
	lw	$2,%lo(eponDebugLevel)($2)
	sltu	$2,$2,4
	bne	$2,$0,$L132
	nop

	lui	$2,%hi(eponMacDumpAllReg)
	addiu	$2,$2,%lo(eponMacDumpAllReg)
	jalr	$2
	nop

	j	$L19
	nop

$L132:
	j	$L19
	nop

$L131:
	lui	$2,%hi(eponGetRandomRetryTimes)
	addiu	$2,$2,%lo(eponGetRandomRetryTimes)
	jalr	$2
	nop

	andi	$3,$2,0x00ff
	lw	$2,20($sp)
	sb	$3,22($2)
	lw	$2,20($sp)
	lbu	$3,20($2)
	li	$2,1			# 0x1
	beq	$3,$2,$L133
	nop

	lw	$2,20($sp)
	lbu	$3,20($2)
	li	$2,2			# 0x2
	bne	$3,$2,$L134
	nop

$L133:
	lui	$2,%hi(mpcpDscvGateLock)
	addiu	$2,$2,%lo(mpcpDscvGateLock)
	sw	$2,304($sp)
	lw	$2,304($sp)
	move	$4,$2
	lui	$2,%hi(_raw_spin_lock_irqsave)
	addiu	$2,$2,%lo(_raw_spin_lock_irqsave)
	jalr	$2
	nop

	sw	$2,32($sp)
	sw	$0,24($sp)
	lui	$2,%hi(g_EPON_MAC_BASE)
	lw	$2,%lo(g_EPON_MAC_BASE)($2)
	addiu	$3,$2,24620
	lbu	$2,16($sp)
	sll	$2,$2,2
	addu	$2,$3,$2
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	sw	$2,28($sp)
	lw	$2,28($sp)
	ext	$2,$2,0,30
	sw	$2,28($sp)
	lui	$2,%hi(g_EPON_MAC_BASE)
	lw	$2,%lo(g_EPON_MAC_BASE)($2)
	addiu	$3,$2,24620
	lbu	$2,16($sp)
	sll	$2,$2,2
	addu	$2,$3,$2
	lw	$4,28($sp)
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	j	$L136
	nop

$L137:
	lw	$2,24($sp)
	addiu	$2,$2,1
	sw	$2,24($sp)
$L136:
	lw	$2,24($sp)
	slt	$2,$2,30
	bne	$2,$0,$L137
	nop

	lui	$2,%hi(g_EPON_MAC_BASE)
	lw	$2,%lo(g_EPON_MAC_BASE)($2)
	addiu	$3,$2,24620
	lbu	$2,16($sp)
	sll	$2,$2,2
	addu	$2,$3,$2
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	sw	$2,28($sp)
	lw	$2,28($sp)
	ext	$2,$2,0,30
	sw	$2,28($sp)
	lw	$3,28($sp)
	li	$2,2147418112			# 0x7fff0000
	ori	$2,$2,0xffff
	or	$2,$3,$2
	sw	$2,28($sp)
	lui	$2,%hi(g_EPON_MAC_BASE)
	lw	$2,%lo(g_EPON_MAC_BASE)($2)
	addiu	$3,$2,24620
	lbu	$2,16($sp)
	sll	$2,$2,2
	addu	$2,$3,$2
	lw	$4,28($sp)
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	move	$4,$0
	lui	$2,%hi(eponFirmwareSetMode)
	addiu	$2,$2,%lo(eponFirmwareSetMode)
	jalr	$2
	nop

	sw	$0,316($sp)
	li	$4,4			# 0x4
	lui	$2,%hi($LC10)
	addiu	$5,$2,%lo($LC10)
	lui	$2,%hi(dbgtoMem)
	addiu	$2,$2,%lo(dbgtoMem)
	jalr	$2
	nop

	lbu	$2,16($sp)
	andi	$2,$2,0x7
	andi	$3,$2,0x00ff
	lw	$2,316($sp)
	ins	$2,$3,0,3
	sw	$2,316($sp)
	lw	$2,316($sp)
	ins	$2,$0,8,1
	sw	$2,316($sp)
	lw	$2,316($sp)
	li	$3,1			# 0x1
	ins	$2,$3,30,2
	sw	$2,316($sp)
	lw	$2,316($sp)
	li	$3,1			# 0x1
	ins	$2,$3,16,1
	sw	$2,316($sp)
	lw	$3,316($sp)
	lui	$2,%hi(g_EPON_MAC_BASE)
	lw	$2,%lo(g_EPON_MAC_BASE)($2)
	addiu	$2,$2,24616
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	lw	$3,316($sp)
	li	$4,4			# 0x4
	lui	$2,%hi($LC11)
	addiu	$5,$2,%lo($LC11)
	move	$6,$3
	lui	$2,%hi(dbgtoMem)
	addiu	$2,$2,%lo(dbgtoMem)
	jalr	$2
	nop

	li	$4,2			# 0x2
	lui	$2,%hi($LC12)
	addiu	$5,$2,%lo($LC12)
	lui	$2,%hi(dbgtoMem)
	addiu	$2,$2,%lo(dbgtoMem)
	jalr	$2
	nop

	lui	$2,%hi(gpEponPriv)
	lw	$2,%lo(gpEponPriv)($2)
	lbu	$2,40($2)
	bne	$2,$0,$L138
	nop

	sw	$0,24($sp)
$L142:
	lui	$2,%hi(g_EPON_MAC_BASE)
	lw	$2,%lo(g_EPON_MAC_BASE)($2)
	addiu	$2,$2,24616
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	sw	$2,316($sp)
	lw	$3,316($sp)
	li	$2,65536			# 0x10000
	and	$2,$3,$2
	beq	$2,$0,$L139
	nop

	j	$L140
	nop

$L139:
	lw	$3,24($sp)
	li	$2,4980736			# 0x4c0000
	ori	$2,$2,0x4b40
	slt	$2,$3,$2
	bne	$2,$0,$L141
	nop

	li	$4,4			# 0x4
	lui	$2,%hi($LC13)
	addiu	$5,$2,%lo($LC13)
	lui	$2,%hi(dbgtoMem)
	addiu	$2,$2,%lo(dbgtoMem)
	jalr	$2
	nop

	j	$L140
	nop

$L141:
	lw	$2,24($sp)
	addiu	$2,$2,1
	sw	$2,24($sp)
	j	$L142
	nop

$L140:
	li	$4,1			# 0x1
	lui	$2,%hi(eponFirmwareSetMode)
	addiu	$2,$2,%lo(eponFirmwareSetMode)
	jalr	$2
	nop

	li	$4,2			# 0x2
	lui	$2,%hi($LC14)
	addiu	$5,$2,%lo($LC14)
	lui	$2,%hi(dbgtoMem)
	addiu	$2,$2,%lo(dbgtoMem)
	jalr	$2
	nop

	lw	$2,20($sp)
	li	$3,2			# 0x2
	sb	$3,20($2)
$L138:
	lui	$2,%hi(mpcpDscvGateLock)
	addiu	$2,$2,%lo(mpcpDscvGateLock)
	sw	$2,308($sp)
	lw	$2,32($sp)
	sw	$2,312($sp)
	lw	$2,308($sp)
	move	$4,$2
	lw	$5,312($sp)
	lui	$2,%hi(_raw_spin_unlock_irqrestore)
	addiu	$2,$2,%lo(_raw_spin_unlock_irqrestore)
	jalr	$2
	nop

$L134:
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
	j	$L19
	nop

$L125:
	lbu	$2,16($sp)
	addiu	$2,$2,1
	sb	$2,16($sp)
	lw	$2,20($sp)
	addiu	$2,$2,72
	sw	$2,20($sp)
$L123:
	lbu	$2,16($sp)
	sltu	$2,$2,8
	bne	$2,$0,$L143
	nop

	nop
$L19:
	lw	$31,356($sp)
	addiu	$sp,$sp,360
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	eponMpcpDiscvGateIntHandler
	.size	eponMpcpDiscvGateIntHandler, .-eponMpcpDiscvGateIntHandler
	.section	.text.eponFirmwareModeSelect,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	eponFirmwareModeSelect
	.type	eponFirmwareModeSelect, @function
eponFirmwareModeSelect:
	.frame	$sp,224,$31		# vars= 200, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-224
	sw	$31,220($sp)
	li	$2,-1078984704			# 0xffffffffbfb00000
	ori	$2,$2,0x64
	sw	$2,32($sp)
	lw	$2,32($sp)
	lw	$2,0($2)
	andi	$3,$2,0xf
	li	$2,3			# 0x3
	bne	$3,$2,$L147
	nop

	li	$2,-1078657024			# 0xffffffffbfb50000
	ori	$2,$2,0xfffc
	sw	$2,36($sp)
	lw	$2,36($sp)
	lw	$2,0($2)
	andi	$2,$2,0xf
	beq	$2,$0,$L147
	nop

	li	$2,-1078984704			# 0xffffffffbfb00000
	ori	$2,$2,0xf8
	sw	$2,40($sp)
	lw	$2,40($sp)
	lw	$2,0($2)
	andi	$3,$2,0x3
	li	$2,2			# 0x2
	bne	$3,$2,$L150
	nop

	li	$2,-1078984704			# 0xffffffffbfb00000
	ori	$2,$2,0x64
	sw	$2,44($sp)
	lw	$2,44($sp)
	lw	$3,0($2)
	li	$2,-65536			# 0xffffffffffff0000
	and	$3,$3,$2
	li	$2,327680			# 0x50000
	bne	$3,$2,$L150
	nop

	li	$2,-1078001664			# 0xffffffffbfbf0000
	ori	$2,$2,0x8214
	sw	$2,48($sp)
	lw	$2,48($sp)
	lw	$2,0($2)
	andi	$2,$2,0x8
	beq	$2,$0,$L153
	nop

	li	$2,-1078001664			# 0xffffffffbfbf0000
	ori	$2,$2,0x8214
	sw	$2,52($sp)
	lw	$2,52($sp)
	lw	$2,0($2)
	andi	$2,$2,0x20
	bne	$2,$0,$L155
	nop

	j	$L150
	nop

$L153:
	li	$2,-1078001664			# 0xffffffffbfbf0000
	ori	$2,$2,0x8214
	sw	$2,56($sp)
	lw	$2,56($sp)
	lw	$2,0($2)
	andi	$2,$2,0x2
	beq	$2,$0,$L150
	nop

$L155:
	li	$2,-1078001664			# 0xffffffffbfbf0000
	ori	$2,$2,0x8214
	sw	$2,60($sp)
	lw	$2,60($sp)
	lw	$2,0($2)
	andi	$2,$2,0x8
	beq	$2,$0,$L158
	nop

	li	$2,-1078001664			# 0xffffffffbfbf0000
	ori	$2,$2,0x8214
	sw	$2,64($sp)
	lw	$2,64($sp)
	lw	$2,0($2)
	andi	$2,$2,0x40
	beq	$2,$0,$L160
	nop

	j	$L150
	nop

$L158:
	li	$2,-1078001664			# 0xffffffffbfbf0000
	ori	$2,$2,0x8214
	sw	$2,68($sp)
	lw	$2,68($sp)
	lw	$2,0($2)
	andi	$2,$2,0x4
	beq	$2,$0,$L160
	nop

$L150:
	li	$2,-1078984704			# 0xffffffffbfb00000
	ori	$2,$2,0xf8
	sw	$2,72($sp)
	lw	$2,72($sp)
	lw	$2,0($2)
	andi	$3,$2,0x3
	li	$2,3			# 0x3
	bne	$3,$2,$L163
	nop

	li	$2,-1078984704			# 0xffffffffbfb00000
	ori	$2,$2,0x64
	sw	$2,76($sp)
	lw	$2,76($sp)
	lw	$3,0($2)
	li	$2,-65536			# 0xffffffffffff0000
	and	$3,$3,$2
	li	$2,327680			# 0x50000
	bne	$3,$2,$L163
	nop

	li	$2,-1078001664			# 0xffffffffbfbf0000
	ori	$2,$2,0x8214
	sw	$2,80($sp)
	lw	$2,80($sp)
	lw	$2,0($2)
	andi	$2,$2,0x8
	beq	$2,$0,$L166
	nop

	li	$2,-1078001664			# 0xffffffffbfbf0000
	ori	$2,$2,0x8214
	sw	$2,84($sp)
	lw	$2,84($sp)
	lw	$2,0($2)
	andi	$2,$2,0x20
	bne	$2,$0,$L160
	nop

	j	$L163
	nop

$L166:
	li	$2,-1078001664			# 0xffffffffbfbf0000
	ori	$2,$2,0x8214
	sw	$2,88($sp)
	lw	$2,88($sp)
	lw	$2,0($2)
	andi	$2,$2,0x2
	bne	$2,$0,$L160
	nop

$L163:
	li	$2,-1078984704			# 0xffffffffbfb00000
	ori	$2,$2,0xf8
	sw	$2,92($sp)
	lw	$2,92($sp)
	lw	$2,0($2)
	andi	$3,$2,0x3
	li	$2,2			# 0x2
	bne	$3,$2,$L170
	nop

	li	$2,-1078984704			# 0xffffffffbfb00000
	ori	$2,$2,0x64
	sw	$2,96($sp)
	lw	$2,96($sp)
	lw	$3,0($2)
	li	$2,-65536			# 0xffffffffffff0000
	and	$3,$3,$2
	li	$2,327680			# 0x50000
	bne	$3,$2,$L170
	nop

	li	$2,-1078001664			# 0xffffffffbfbf0000
	ori	$2,$2,0x8214
	sw	$2,100($sp)
	lw	$2,100($sp)
	lw	$2,0($2)
	andi	$2,$2,0x8
	beq	$2,$0,$L173
	nop

	li	$2,-1078001664			# 0xffffffffbfbf0000
	ori	$2,$2,0x8214
	sw	$2,104($sp)
	lw	$2,104($sp)
	lw	$2,0($2)
	andi	$2,$2,0x20
	beq	$2,$0,$L175
	nop

	j	$L170
	nop

$L173:
	li	$2,-1078001664			# 0xffffffffbfbf0000
	ori	$2,$2,0x8214
	sw	$2,108($sp)
	lw	$2,108($sp)
	lw	$2,0($2)
	andi	$2,$2,0x2
	bne	$2,$0,$L170
	nop

$L175:
	li	$2,-1078001664			# 0xffffffffbfbf0000
	ori	$2,$2,0x8214
	sw	$2,112($sp)
	lw	$2,112($sp)
	lw	$2,0($2)
	andi	$2,$2,0x8
	beq	$2,$0,$L178
	nop

	li	$2,-1078001664			# 0xffffffffbfbf0000
	ori	$2,$2,0x8214
	sw	$2,116($sp)
	lw	$2,116($sp)
	lw	$2,0($2)
	andi	$2,$2,0x40
	beq	$2,$0,$L160
	nop

	j	$L170
	nop

$L178:
	li	$2,-1078001664			# 0xffffffffbfbf0000
	ori	$2,$2,0x8214
	sw	$2,120($sp)
	lw	$2,120($sp)
	lw	$2,0($2)
	andi	$2,$2,0x4
	beq	$2,$0,$L160
	nop

$L170:
	li	$2,-1078984704			# 0xffffffffbfb00000
	ori	$2,$2,0xf8
	sw	$2,124($sp)
	lw	$2,124($sp)
	lw	$2,0($2)
	andi	$3,$2,0x3
	li	$2,3			# 0x3
	bne	$3,$2,$L182
	nop

	li	$2,-1078984704			# 0xffffffffbfb00000
	ori	$2,$2,0x64
	sw	$2,128($sp)
	lw	$2,128($sp)
	lw	$3,0($2)
	li	$2,-65536			# 0xffffffffffff0000
	and	$3,$3,$2
	li	$2,327680			# 0x50000
	bne	$3,$2,$L182
	nop

	li	$2,-1078001664			# 0xffffffffbfbf0000
	ori	$2,$2,0x8214
	sw	$2,132($sp)
	lw	$2,132($sp)
	lw	$2,0($2)
	andi	$2,$2,0x8
	beq	$2,$0,$L185
	nop

	li	$2,-1078001664			# 0xffffffffbfbf0000
	ori	$2,$2,0x8214
	sw	$2,136($sp)
	lw	$2,136($sp)
	lw	$2,0($2)
	andi	$2,$2,0x20
	beq	$2,$0,$L160
	nop

	j	$L182
	nop

$L185:
	li	$2,-1078001664			# 0xffffffffbfbf0000
	ori	$2,$2,0x8214
	sw	$2,140($sp)
	lw	$2,140($sp)
	lw	$2,0($2)
	andi	$2,$2,0x2
	beq	$2,$0,$L160
	nop

$L182:
	li	$2,-1078984704			# 0xffffffffbfb00000
	ori	$2,$2,0xf8
	sw	$2,144($sp)
	lw	$2,144($sp)
	lw	$2,0($2)
	andi	$3,$2,0x3
	li	$2,2			# 0x2
	bne	$3,$2,$L147
	nop

	li	$2,-1078984704			# 0xffffffffbfb00000
	ori	$2,$2,0x64
	sw	$2,148($sp)
	lw	$2,148($sp)
	lw	$3,0($2)
	li	$2,-65536			# 0xffffffffffff0000
	and	$3,$3,$2
	li	$2,327680			# 0x50000
	bne	$3,$2,$L147
	nop

	li	$2,-1078001664			# 0xffffffffbfbf0000
	ori	$2,$2,0x8214
	sw	$2,152($sp)
	lw	$2,152($sp)
	lw	$2,0($2)
	andi	$2,$2,0x8
	beq	$2,$0,$L191
	nop

	li	$2,-1078001664			# 0xffffffffbfbf0000
	ori	$2,$2,0x8214
	sw	$2,156($sp)
	lw	$2,156($sp)
	lw	$2,0($2)
	andi	$2,$2,0x40
	bne	$2,$0,$L160
	nop

	j	$L147
	nop

$L191:
	li	$2,-1078001664			# 0xffffffffbfbf0000
	ori	$2,$2,0x8214
	sw	$2,160($sp)
	lw	$2,160($sp)
	lw	$2,0($2)
	andi	$2,$2,0x4
	beq	$2,$0,$L147
	nop

$L160:
	lui	$2,%hi(g_EPON_MAC_BASE)
	lw	$2,%lo(g_EPON_MAC_BASE)($2)
	addiu	$2,$2,24788
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	sw	$2,212($sp)
	lhu	$2,214($sp)
	sltu	$2,$2,33
	beq	$2,$0,$L194
	nop

	lui	$2,%hi(g_EPON_MAC_BASE)
	lw	$2,%lo(g_EPON_MAC_BASE)($2)
	addiu	$2,$2,24804
	sw	$2,164($sp)
	lw	$2,164($sp)
	lw	$2,0($2)
	sw	$2,16($sp)
	lui	$2,%hi(g_EPON_MAC_BASE)
	lw	$2,%lo(g_EPON_MAC_BASE)($2)
	addiu	$2,$2,24804
	move	$3,$2
	lw	$2,16($sp)
	ext	$2,$2,0,31
	sw	$3,168($sp)
	sw	$2,172($sp)
	lw	$2,168($sp)
	lw	$3,172($sp)
	sw	$3,0($2)
	lui	$2,%hi(g_EPON_MAC_BASE)
	lw	$2,%lo(g_EPON_MAC_BASE)($2)
	addiu	$2,$2,24804
	sw	$2,176($sp)
	lw	$2,176($sp)
	lw	$2,0($2)
	sw	$2,20($sp)
	lui	$2,%hi(g_EPON_MAC_BASE)
	lw	$2,%lo(g_EPON_MAC_BASE)($2)
	addiu	$2,$2,24804
	move	$4,$2
	lw	$3,20($sp)
	li	$2,-1073807360			# 0xffffffffbfff0000
	ori	$2,$2,0xffff
	and	$2,$3,$2
	sw	$4,180($sp)
	sw	$2,184($sp)
	lw	$2,180($sp)
	lw	$3,184($sp)
	sw	$3,0($2)
	j	$L147
	nop

$L194:
	lui	$2,%hi(g_EPON_MAC_BASE)
	lw	$2,%lo(g_EPON_MAC_BASE)($2)
	addiu	$2,$2,24804
	sw	$2,188($sp)
	lw	$2,188($sp)
	lw	$2,0($2)
	sw	$2,24($sp)
	lui	$2,%hi(g_EPON_MAC_BASE)
	lw	$2,%lo(g_EPON_MAC_BASE)($2)
	addiu	$2,$2,24804
	move	$3,$2
	lw	$2,24($sp)
	ext	$2,$2,0,31
	sw	$3,192($sp)
	sw	$2,196($sp)
	lw	$2,192($sp)
	lw	$3,196($sp)
	sw	$3,0($2)
	lui	$2,%hi(g_EPON_MAC_BASE)
	lw	$2,%lo(g_EPON_MAC_BASE)($2)
	addiu	$2,$2,24804
	sw	$2,200($sp)
	lw	$2,200($sp)
	lw	$2,0($2)
	sw	$2,28($sp)
	lui	$2,%hi(g_EPON_MAC_BASE)
	lw	$2,%lo(g_EPON_MAC_BASE)($2)
	addiu	$2,$2,24804
	move	$4,$2
	lw	$3,28($sp)
	li	$2,1073741824			# 0x40000000
	or	$2,$3,$2
	sw	$4,204($sp)
	sw	$2,208($sp)
	lw	$2,204($sp)
	lw	$3,208($sp)
	sw	$3,0($2)
$L147:
	li	$4,1			# 0x1
	lui	$2,%hi(eponFirmwareSetMode)
	addiu	$2,$2,%lo(eponFirmwareSetMode)
	jalr	$2
	nop

	lw	$31,220($sp)
	addiu	$sp,$sp,224
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	eponFirmwareModeSelect
	.size	eponFirmwareModeSelect, .-eponFirmwareModeSelect
	.section	.text.eponClearRetryCounter,"ax",@progbits
	.align	2
	.globl	eponClearRetryCounter
	.set	nomips16
	.set	nomicromips
	.ent	eponClearRetryCounter
	.type	eponClearRetryCounter, @function
eponClearRetryCounter:
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	lui	$2,%hi(gDiscoveryGateCnt)
	sw	$0,%lo(gDiscoveryGateCnt)($2)
	lui	$2,%hi(gFeResetRetryCnt)
	sw	$0,%lo(gFeResetRetryCnt)($2)
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	eponClearRetryCounter
	.size	eponClearRetryCounter, .-eponClearRetryCounter
	.rdata
	.align	2
$LC15:
	.ascii	"Epon Send MPCP REGISTER_REQUEST DiscvGate Finished, llid"
	.ascii	"Index = %d\000"
	.section	.text.eponMpcpRgstReqIntHandler,"ax",@progbits
	.align	2
	.globl	eponMpcpRgstReqIntHandler
	.set	nomips16
	.set	nomicromips
	.ent	eponMpcpRgstReqIntHandler
	.type	eponMpcpRgstReqIntHandler, @function
eponMpcpRgstReqIntHandler:
	.frame	$sp,40,$31		# vars= 16, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-40
	sw	$31,36($sp)
	sb	$0,16($sp)
	sw	$0,20($sp)
	lui	$2,%hi(g_EPON_MAC_BASE)
	lw	$2,%lo(g_EPON_MAC_BASE)($2)
	addiu	$2,$2,24616
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	sw	$2,24($sp)
	lw	$2,24($sp)
	ext	$2,$2,0,3
	andi	$2,$2,0x00ff
	sb	$2,16($sp)
	lbu	$2,16($sp)
	sll	$2,$2,3
	sll	$3,$2,3
	addu	$2,$2,$3
	addiu	$3,$2,16
	lui	$2,%hi(eponDrv)
	addiu	$2,$2,%lo(eponDrv)
	addu	$2,$3,$2
	addiu	$2,$2,4
	sw	$2,20($sp)
	lw	$2,20($sp)
	li	$3,2			# 0x2
	sb	$3,20($2)
	lbu	$3,16($sp)
	li	$4,2			# 0x2
	lui	$2,%hi($LC15)
	addiu	$5,$2,%lo($LC15)
	move	$6,$3
	lui	$2,%hi(dbgtoMem)
	addiu	$2,$2,%lo(dbgtoMem)
	jalr	$2
	nop

	move	$2,$0
	lw	$31,36($sp)
	addiu	$sp,$sp,40
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	eponMpcpRgstReqIntHandler
	.size	eponMpcpRgstReqIntHandler, .-eponMpcpRgstReqIntHandler
	.rdata
	.align	2
$LC16:
	.ascii	"Epon Send MPCP Ack, Mpcp Handshake Done,llidIndex = %d\000"
	.section	.text.eponMpcpRgstAckIntHandler,"ax",@progbits
	.align	2
	.globl	eponMpcpRgstAckIntHandler
	.set	nomips16
	.set	nomicromips
	.ent	eponMpcpRgstAckIntHandler
	.type	eponMpcpRgstAckIntHandler, @function
eponMpcpRgstAckIntHandler:
	.frame	$sp,48,$31		# vars= 24, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-48
	sw	$31,44($sp)
	sb	$0,16($sp)
	sw	$0,20($sp)
	sw	$0,24($sp)
	lui	$2,%hi(g_EPON_MAC_BASE)
	lw	$2,%lo(g_EPON_MAC_BASE)($2)
	addiu	$2,$2,24616
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	sw	$2,28($sp)
	lw	$2,28($sp)
	ext	$2,$2,0,3
	andi	$2,$2,0x00ff
	sb	$2,16($sp)
	lbu	$2,16($sp)
	sll	$2,$2,3
	sll	$3,$2,3
	addu	$2,$2,$3
	addiu	$3,$2,16
	lui	$2,%hi(eponDrv)
	addiu	$2,$2,%lo(eponDrv)
	addu	$2,$3,$2
	addiu	$2,$2,4
	sw	$2,20($sp)
	lw	$2,20($sp)
	addiu	$2,$2,20
	sw	$2,24($sp)
	sw	$0,32($sp)
	lui	$2,%hi(g_EPON_MAC_BASE)
	lw	$2,%lo(g_EPON_MAC_BASE)($2)
	addiu	$3,$2,24620
	lbu	$2,16($sp)
	sll	$2,$2,2
	addu	$2,$3,$2
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	sw	$2,32($sp)
	lw	$2,32($sp)
	li	$3,2			# 0x2
	ins	$2,$3,30,2
	sw	$2,32($sp)
	lw	$4,32($sp)
	lui	$2,%hi(g_EPON_MAC_BASE)
	lw	$2,%lo(g_EPON_MAC_BASE)($2)
	addiu	$3,$2,24620
	lbu	$2,16($sp)
	sll	$2,$2,2
	addu	$2,$3,$2
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	lw	$2,24($sp)
	li	$3,8			# 0x8
	sb	$3,0($2)
	lui	$2,%hi(mpcpTmOutCnt)
	sw	$0,%lo(mpcpTmOutCnt)($2)
	lui	$2,%hi(eponClearRetryCounter)
	addiu	$2,$2,%lo(eponClearRetryCounter)
	jalr	$2
	nop

	lbu	$2,16($sp)
	sb	$2,36($sp)
	lw	$2,20($sp)
	lhu	$2,2($2)
	sh	$2,38($sp)
	lbu	$2,16($sp)
	li	$4,2			# 0x2
	li	$5,2			# 0x2
	move	$6,$2
	lui	$2,%hi(xmcs_report_event)
	addiu	$2,$2,%lo(xmcs_report_event)
	jalr	$2
	nop

	lbu	$3,16($sp)
	li	$4,2			# 0x2
	lui	$2,%hi($LC16)
	addiu	$5,$2,%lo($LC16)
	move	$6,$3
	lui	$2,%hi(dbgtoMem)
	addiu	$2,$2,%lo(dbgtoMem)
	jalr	$2
	nop

	addiu	$2,$sp,36
	move	$4,$2
	lui	$2,%hi(xmcs_create_llid)
	addiu	$2,$2,%lo(xmcs_create_llid)
	jalr	$2
	nop

	lui	$2,%hi(eponFirmwareModeSelect)
	addiu	$2,$2,%lo(eponFirmwareModeSelect)
	jalr	$2
	nop

	lui	$2,%hi(start_omci_oam_monitor)
	addiu	$2,$2,%lo(start_omci_oam_monitor)
	jalr	$2
	nop

	move	$2,$0
	lw	$31,44($sp)
	addiu	$sp,$sp,48
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	eponMpcpRgstAckIntHandler
	.size	eponMpcpRgstAckIntHandler, .-eponMpcpRgstAckIntHandler
	.rdata
	.align	2
$LC17:
	.ascii	"ERR: eponMpcpDscvFsmPendingHandler done bit not ready\000"
	.align	2
$LC18:
	.ascii	"eponMpcpDscvFsmPendingHandler done bit OK!! RgstrAck=%d\000"
	.align	2
$LC19:
	.ascii	"eponMpcpDscvFsmPendingHandler mpcp_cmd_done, cnt=%d\000"
	.align	2
$LC20:
	.ascii	"Epon Send MPCP Ack, Mpcp Handshake Done\000"
	.align	2
$LC21:
	.ascii	"eponMpcpDscvFsmPendingHandler111  mpcp_cmd_done cnt=%d\000"
	.align	2
$LC22:
	.ascii	"Epon Send MPCP Nack, MPCP Retry\000"
	.section	.text.eponMpcpDscvFsmPendingHandler,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	eponMpcpDscvFsmPendingHandler
	.type	eponMpcpDscvFsmPendingHandler, @function
eponMpcpDscvFsmPendingHandler:
	.frame	$sp,80,$31		# vars= 56, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-80
	sw	$31,76($sp)
	move	$2,$4
	sb	$2,80($sp)
	lbu	$2,80($sp)
	sll	$2,$2,3
	sll	$3,$2,3
	addu	$2,$2,$3
	addiu	$3,$2,16
	lui	$2,%hi(eponDrv)
	addiu	$2,$2,%lo(eponDrv)
	addu	$2,$3,$2
	addiu	$2,$2,4
	sw	$2,20($sp)
	lw	$2,20($sp)
	addiu	$2,$2,20
	sw	$2,24($sp)
	sw	$0,16($sp)
	lui	$2,%hi(eponMacCheckRegreqDoneBit)
	addiu	$2,$2,%lo(eponMacCheckRegreqDoneBit)
	jalr	$2
	nop

	bgez	$2,$L205
	nop

	li	$4,1			# 0x1
	lui	$2,%hi($LC17)
	addiu	$5,$2,%lo($LC17)
	lui	$2,%hi(dbgtoMem)
	addiu	$2,$2,%lo(dbgtoMem)
	jalr	$2
	nop

	li	$2,-1			# 0xffffffffffffffff
	j	$L217
	nop

$L205:
	lw	$2,24($sp)
	lbu	$2,1($2)
	move	$3,$2
	li	$4,4			# 0x4
	lui	$2,%hi($LC18)
	addiu	$5,$2,%lo($LC18)
	move	$6,$3
	lui	$2,%hi(dbgtoMem)
	addiu	$2,$2,%lo(dbgtoMem)
	jalr	$2
	nop

	lui	$2,%hi(mpcpDscvPendingLock)
	addiu	$2,$2,%lo(mpcpDscvPendingLock)
	sw	$2,32($sp)
	lw	$2,32($sp)
	move	$4,$2
	lui	$2,%hi(_raw_spin_lock_irqsave)
	addiu	$2,$2,%lo(_raw_spin_lock_irqsave)
	jalr	$2
	nop

	sw	$2,28($sp)
	sw	$0,48($sp)
	lw	$2,24($sp)
	lbu	$3,1($2)
	li	$2,1			# 0x1
	bne	$3,$2,$L208
	nop

	lw	$2,24($sp)
	li	$3,6			# 0x6
	sb	$3,0($2)
	lw	$2,24($sp)
	li	$3,1			# 0x1
	sw	$3,36($2)
	sw	$0,44($sp)
	lw	$2,44($sp)
	li	$3,3			# 0x3
	ins	$2,$3,30,2
	sw	$2,44($sp)
	lw	$2,44($sp)
	li	$3,1			# 0x1
	ins	$2,$3,12,1
	sw	$2,44($sp)
	lbu	$2,80($sp)
	andi	$2,$2,0x7
	andi	$3,$2,0x00ff
	lw	$2,44($sp)
	ins	$2,$3,0,3
	sw	$2,44($sp)
	lw	$2,44($sp)
	li	$3,1			# 0x1
	ins	$2,$3,16,1
	sw	$2,44($sp)
	lw	$3,44($sp)
	lui	$2,%hi(g_EPON_MAC_BASE)
	lw	$2,%lo(g_EPON_MAC_BASE)($2)
	addiu	$2,$2,24616
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	lui	$2,%hi(gpEponPriv)
	lw	$2,%lo(gpEponPriv)($2)
	lbu	$2,40($2)
	bne	$2,$0,$L208
	nop

	sw	$0,16($sp)
$L212:
	lw	$2,16($sp)
	addiu	$2,$2,1
	sw	$2,16($sp)
	lui	$2,%hi(g_EPON_MAC_BASE)
	lw	$2,%lo(g_EPON_MAC_BASE)($2)
	addiu	$2,$2,24616
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	sw	$2,44($sp)
	lw	$3,44($sp)
	li	$2,65536			# 0x10000
	and	$2,$3,$2
	beq	$2,$0,$L209
	nop

	li	$4,4			# 0x4
	lui	$2,%hi($LC19)
	addiu	$5,$2,%lo($LC19)
	lw	$6,16($sp)
	lui	$2,%hi(dbgtoMem)
	addiu	$2,$2,%lo(dbgtoMem)
	jalr	$2
	nop

	j	$L210
	nop

$L209:
	lw	$3,16($sp)
	li	$2,983040			# 0xf0000
	ori	$2,$2,0x4241
	sltu	$2,$3,$2
	bne	$2,$0,$L211
	nop

	j	$L210
	nop

$L211:
	j	$L212
	nop

$L210:
	lui	$2,%hi(g_EPON_MAC_BASE)
	lw	$2,%lo(g_EPON_MAC_BASE)($2)
	addiu	$3,$2,24620
	lbu	$2,80($sp)
	sll	$2,$2,2
	addu	$2,$3,$2
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	sw	$2,48($sp)
	lw	$2,48($sp)
	li	$3,2			# 0x2
	ins	$2,$3,30,2
	sw	$2,48($sp)
	lw	$4,48($sp)
	lui	$2,%hi(g_EPON_MAC_BASE)
	lw	$2,%lo(g_EPON_MAC_BASE)($2)
	addiu	$3,$2,24620
	lbu	$2,80($sp)
	sll	$2,$2,2
	addu	$2,$3,$2
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	lw	$2,24($sp)
	li	$3,8			# 0x8
	sb	$3,0($2)
	lui	$2,%hi(mpcpTmOutCnt)
	sw	$0,%lo(mpcpTmOutCnt)($2)
	lui	$2,%hi(eponClearRetryCounter)
	addiu	$2,$2,%lo(eponClearRetryCounter)
	jalr	$2
	nop

	lbu	$2,80($sp)
	sb	$2,52($sp)
	lw	$2,20($sp)
	lhu	$2,2($2)
	sh	$2,54($sp)
	lbu	$2,80($sp)
	li	$4,2			# 0x2
	li	$5,2			# 0x2
	move	$6,$2
	lui	$2,%hi(xmcs_report_event)
	addiu	$2,$2,%lo(xmcs_report_event)
	jalr	$2
	nop

	li	$4,2			# 0x2
	lui	$2,%hi($LC20)
	addiu	$5,$2,%lo($LC20)
	lui	$2,%hi(dbgtoMem)
	addiu	$2,$2,%lo(dbgtoMem)
	jalr	$2
	nop

	addiu	$2,$sp,52
	move	$4,$2
	lui	$2,%hi(xmcs_create_llid)
	addiu	$2,$2,%lo(xmcs_create_llid)
	jalr	$2
	nop

	lui	$2,%hi(eponFirmwareModeSelect)
	addiu	$2,$2,%lo(eponFirmwareModeSelect)
	jalr	$2
	nop

	lui	$2,%hi(start_omci_oam_monitor)
	addiu	$2,$2,%lo(start_omci_oam_monitor)
	jalr	$2
	nop

$L208:
	lw	$2,24($sp)
	lbu	$2,1($2)
	bne	$2,$0,$L213
	nop

	lw	$2,24($sp)
	li	$3,7			# 0x7
	sb	$3,0($2)
	sw	$0,44($sp)
	lw	$2,44($sp)
	li	$3,3			# 0x3
	ins	$2,$3,30,2
	sw	$2,44($sp)
	lw	$2,44($sp)
	ins	$2,$0,12,1
	sw	$2,44($sp)
	lbu	$2,80($sp)
	andi	$2,$2,0x7
	andi	$3,$2,0x00ff
	lw	$2,44($sp)
	ins	$2,$3,0,3
	sw	$2,44($sp)
	lw	$2,44($sp)
	li	$3,1			# 0x1
	ins	$2,$3,16,1
	sw	$2,44($sp)
	lw	$3,44($sp)
	lui	$2,%hi(g_EPON_MAC_BASE)
	lw	$2,%lo(g_EPON_MAC_BASE)($2)
	addiu	$2,$2,24616
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	sw	$0,16($sp)
$L216:
	lw	$2,16($sp)
	addiu	$2,$2,1
	sw	$2,16($sp)
	lui	$2,%hi(g_EPON_MAC_BASE)
	lw	$2,%lo(g_EPON_MAC_BASE)($2)
	addiu	$2,$2,24616
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	sw	$2,44($sp)
	lw	$3,44($sp)
	li	$2,65536			# 0x10000
	and	$2,$3,$2
	beq	$2,$0,$L214
	nop

	li	$4,4			# 0x4
	lui	$2,%hi($LC21)
	addiu	$5,$2,%lo($LC21)
	lw	$6,16($sp)
	lui	$2,%hi(dbgtoMem)
	addiu	$2,$2,%lo(dbgtoMem)
	jalr	$2
	nop

	nop
	move	$4,$0
	lui	$2,%hi(eponFirmwareSetMode)
	addiu	$2,$2,%lo(eponFirmwareSetMode)
	jalr	$2
	nop

	lw	$2,24($sp)
	sb	$0,0($2)
	lbu	$2,80($sp)
	move	$4,$2
	lui	$2,%hi(eponMpcpDscvFsmWaitHandler)
	addiu	$2,$2,%lo(eponMpcpDscvFsmWaitHandler)
	jalr	$2
	nop

	li	$4,2			# 0x2
	lui	$2,%hi($LC22)
	addiu	$5,$2,%lo($LC22)
	lui	$2,%hi(dbgtoMem)
	addiu	$2,$2,%lo(dbgtoMem)
	jalr	$2
	nop

	j	$L213
	nop

$L214:
	j	$L216
	nop

$L213:
	lui	$2,%hi(mpcpDscvPendingLock)
	addiu	$2,$2,%lo(mpcpDscvPendingLock)
	sw	$2,36($sp)
	lw	$2,28($sp)
	sw	$2,40($sp)
	lw	$2,36($sp)
	move	$4,$2
	lw	$5,40($sp)
	lui	$2,%hi(_raw_spin_unlock_irqrestore)
	addiu	$2,$2,%lo(_raw_spin_unlock_irqrestore)
	jalr	$2
	nop

	move	$2,$0
$L217:
	lw	$31,76($sp)
	addiu	$sp,$sp,80
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	eponMpcpDscvFsmPendingHandler
	.size	eponMpcpDscvFsmPendingHandler, .-eponMpcpDscvFsmPendingHandler
	.rdata
	.align	2
$LC23:
	.ascii	"enter eponMpcpRgstIntHandler llidIndex=%d, mpcpDiscState"
	.ascii	"=%d\000"
	.align	2
$LC24:
	.ascii	"enter eponMpcpRgstIntHandler enableFlag== DISABLE\000"
	.align	2
$LC25:
	.ascii	"eponMpcpRgstIntHandler lidEntry_p->eponMpcp.eponDiscFsm."
	.ascii	"begin == FALSE\000"
	.align	2
$LC26:
	.ascii	"eponMpcpRgstIntHandler(%d)  llidDscvSts=%x\000"
	.align	2
$LC27:
	.ascii	"Epon Receive MPCP Register, Now State is REGISTER_REQUES"
	.ascii	"T, llidDscvSts=%x\000"
	.align	2
$LC28:
	.ascii	"eponMpcpRgstIntHandler LLID=%d llidEntry_p->llid =%d\000"
	.align	2
$LC29:
	.ascii	"ERR: eponMpcpRgstIntHandler  llidDscvSts.Bits.llidValid "
	.ascii	"error\000"
	.align	2
$LC30:
	.ascii	"enter eponMpcpRgstIntHandler:gFeResetRetryCnt = 0\000"
	.align	2
$LC31:
	.ascii	"eponMpcpRgstIntHandler LLID=%d flag_rx=nack\000"
	.align	2
$LC32:
	.ascii	"Epon received MPCP Nack change to silent, time %d\000"
	.align	2
$LC33:
	.ascii	"eponMpcpRgstIntHandler  mpcpDiscState==MPCP_STATE_REGIST"
	.ascii	"ERED\000"
	.align	2
$LC34:
	.ascii	"Epon Receive MPCP Reregister Msg LLID %d \000"
	.align	2
$LC35:
	.ascii	"Receive REREGISTER Message , Report DEREGISTER Event\000"
	.align	2
$LC36:
	.ascii	"Epon Receive MPCP DEREGISTER Message LLID %d \000"
	.align	2
$LC37:
	.ascii	"AuthFail DiscState Change to DENIED, Report EPON_DENIED "
	.ascii	"Event \000"
	.align	2
$LC38:
	.ascii	"change to silent, time %d\000"
	.align	2
$LC39:
	.ascii	"DiscState Change to DEREGISTER , Report DEREGISTER Event"
	.ascii	"\000"
	.section	.text.eponMpcpRgstIntHandler,"ax",@progbits
	.align	2
	.globl	eponMpcpRgstIntHandler
	.set	nomips16
	.set	nomicromips
	.ent	eponMpcpRgstIntHandler
	.type	eponMpcpRgstIntHandler, @function
eponMpcpRgstIntHandler:
	.frame	$sp,312,$31		# vars= 288, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-312
	sw	$31,308($sp)
	move	$2,$4
	sb	$2,312($sp)
	lbu	$2,312($sp)
	sll	$2,$2,3
	sll	$3,$2,3
	addu	$2,$2,$3
	addiu	$3,$2,16
	lui	$2,%hi(eponDrv)
	addiu	$2,$2,%lo(eponDrv)
	addu	$2,$3,$2
	addiu	$2,$2,4
	sw	$2,16($sp)
	lw	$2,16($sp)
	addiu	$2,$2,20
	sw	$2,20($sp)
	sw	$0,292($sp)
	lbu	$3,312($sp)
	lw	$2,20($sp)
	lbu	$2,0($2)
	move	$7,$2
	li	$4,4			# 0x4
	lui	$2,%hi($LC23)
	addiu	$5,$2,%lo($LC23)
	move	$6,$3
	lui	$2,%hi(dbgtoMem)
	addiu	$2,$2,%lo(dbgtoMem)
	jalr	$2
	nop

	lw	$2,16($sp)
	lbu	$2,1($2)
	bne	$2,$0,$L219
	nop

	li	$4,4			# 0x4
	lui	$2,%hi($LC24)
	addiu	$5,$2,%lo($LC24)
	lui	$2,%hi(dbgtoMem)
	addiu	$2,$2,%lo(dbgtoMem)
	jalr	$2
	nop

	move	$2,$0
	j	$L335
	nop

$L219:
	lw	$2,20($sp)
	lw	$2,4($2)
	bne	$2,$0,$L221
	nop

	li	$4,4			# 0x4
	lui	$2,%hi($LC25)
	addiu	$5,$2,%lo($LC25)
	lui	$2,%hi(dbgtoMem)
	addiu	$2,$2,%lo(dbgtoMem)
	jalr	$2
	nop

	move	$2,$0
	j	$L335
	nop

$L221:
	lw	$2,16($sp)
	sb	$0,22($2)
	lui	$2,%hi(g_EPON_MAC_BASE)
	lw	$2,%lo(g_EPON_MAC_BASE)($2)
	addiu	$3,$2,24620
	lbu	$2,312($sp)
	sll	$2,$2,2
	addu	$2,$3,$2
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	sw	$2,288($sp)
	lw	$3,288($sp)
	li	$2,50331648			# 0x3000000
	and	$3,$3,$2
	li	$2,33554432			# 0x2000000
	beq	$3,$2,$L222
	nop

	lbu	$6,312($sp)
	lw	$3,288($sp)
	li	$4,4			# 0x4
	lui	$2,%hi($LC26)
	addiu	$5,$2,%lo($LC26)
	move	$7,$3
	lui	$2,%hi(dbgtoMem)
	addiu	$2,$2,%lo(dbgtoMem)
	jalr	$2
	nop

$L222:
	lw	$2,20($sp)
	lbu	$3,0($2)
	li	$2,2			# 0x2
	bne	$3,$2,$L223
	nop

	lw	$3,288($sp)
	li	$4,2			# 0x2
	lui	$2,%hi($LC27)
	addiu	$5,$2,%lo($LC27)
	move	$6,$3
	lui	$2,%hi(dbgtoMem)
	addiu	$2,$2,%lo(dbgtoMem)
	jalr	$2
	nop

	lui	$2,%hi(g_EPON_MAC_BASE)
	lw	$2,%lo(g_EPON_MAC_BASE)($2)
	addiu	$2,$2,24788
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	sw	$2,300($sp)
	lhu	$2,302($sp)
	move	$4,$2
	li	$2,-1078984704			# 0xffffffffbfb00000
	ori	$2,$2,0x64
	sw	$2,24($sp)
	lw	$2,24($sp)
	lw	$2,0($2)
	andi	$3,$2,0xf
	li	$2,3			# 0x3
	bne	$3,$2,$L225
	nop

	li	$2,-1078657024			# 0xffffffffbfb50000
	ori	$2,$2,0xfffc
	sw	$2,28($sp)
	lw	$2,28($sp)
	lw	$2,0($2)
	andi	$2,$2,0xf
	beq	$2,$0,$L225
	nop

	li	$2,-1078984704			# 0xffffffffbfb00000
	ori	$2,$2,0xf8
	sw	$2,32($sp)
	lw	$2,32($sp)
	lw	$2,0($2)
	andi	$3,$2,0x3
	li	$2,2			# 0x2
	bne	$3,$2,$L228
	nop

	li	$2,-1078984704			# 0xffffffffbfb00000
	ori	$2,$2,0x64
	sw	$2,36($sp)
	lw	$2,36($sp)
	lw	$3,0($2)
	li	$2,-65536			# 0xffffffffffff0000
	and	$3,$3,$2
	li	$2,327680			# 0x50000
	bne	$3,$2,$L228
	nop

	li	$2,-1078001664			# 0xffffffffbfbf0000
	ori	$2,$2,0x8214
	sw	$2,40($sp)
	lw	$2,40($sp)
	lw	$2,0($2)
	andi	$2,$2,0x8
	beq	$2,$0,$L231
	nop

	li	$2,-1078001664			# 0xffffffffbfbf0000
	ori	$2,$2,0x8214
	sw	$2,44($sp)
	lw	$2,44($sp)
	lw	$2,0($2)
	andi	$2,$2,0x20
	bne	$2,$0,$L233
	nop

	j	$L228
	nop

$L231:
	li	$2,-1078001664			# 0xffffffffbfbf0000
	ori	$2,$2,0x8214
	sw	$2,48($sp)
	lw	$2,48($sp)
	lw	$2,0($2)
	andi	$2,$2,0x2
	beq	$2,$0,$L228
	nop

$L233:
	li	$2,-1078001664			# 0xffffffffbfbf0000
	ori	$2,$2,0x8214
	sw	$2,52($sp)
	lw	$2,52($sp)
	lw	$2,0($2)
	andi	$2,$2,0x8
	beq	$2,$0,$L236
	nop

	li	$2,-1078001664			# 0xffffffffbfbf0000
	ori	$2,$2,0x8214
	sw	$2,56($sp)
	lw	$2,56($sp)
	lw	$2,0($2)
	andi	$2,$2,0x40
	beq	$2,$0,$L238
	nop

	j	$L228
	nop

$L236:
	li	$2,-1078001664			# 0xffffffffbfbf0000
	ori	$2,$2,0x8214
	sw	$2,60($sp)
	lw	$2,60($sp)
	lw	$2,0($2)
	andi	$2,$2,0x4
	beq	$2,$0,$L238
	nop

$L228:
	li	$2,-1078984704			# 0xffffffffbfb00000
	ori	$2,$2,0xf8
	sw	$2,64($sp)
	lw	$2,64($sp)
	lw	$2,0($2)
	andi	$3,$2,0x3
	li	$2,3			# 0x3
	bne	$3,$2,$L241
	nop

	li	$2,-1078984704			# 0xffffffffbfb00000
	ori	$2,$2,0x64
	sw	$2,68($sp)
	lw	$2,68($sp)
	lw	$3,0($2)
	li	$2,-65536			# 0xffffffffffff0000
	and	$3,$3,$2
	li	$2,327680			# 0x50000
	bne	$3,$2,$L241
	nop

	li	$2,-1078001664			# 0xffffffffbfbf0000
	ori	$2,$2,0x8214
	sw	$2,72($sp)
	lw	$2,72($sp)
	lw	$2,0($2)
	andi	$2,$2,0x8
	beq	$2,$0,$L244
	nop

	li	$2,-1078001664			# 0xffffffffbfbf0000
	ori	$2,$2,0x8214
	sw	$2,76($sp)
	lw	$2,76($sp)
	lw	$2,0($2)
	andi	$2,$2,0x20
	bne	$2,$0,$L238
	nop

	j	$L241
	nop

$L244:
	li	$2,-1078001664			# 0xffffffffbfbf0000
	ori	$2,$2,0x8214
	sw	$2,80($sp)
	lw	$2,80($sp)
	lw	$2,0($2)
	andi	$2,$2,0x2
	bne	$2,$0,$L238
	nop

$L241:
	li	$2,-1078984704			# 0xffffffffbfb00000
	ori	$2,$2,0xf8
	sw	$2,84($sp)
	lw	$2,84($sp)
	lw	$2,0($2)
	andi	$3,$2,0x3
	li	$2,2			# 0x2
	bne	$3,$2,$L248
	nop

	li	$2,-1078984704			# 0xffffffffbfb00000
	ori	$2,$2,0x64
	sw	$2,88($sp)
	lw	$2,88($sp)
	lw	$3,0($2)
	li	$2,-65536			# 0xffffffffffff0000
	and	$3,$3,$2
	li	$2,327680			# 0x50000
	bne	$3,$2,$L248
	nop

	li	$2,-1078001664			# 0xffffffffbfbf0000
	ori	$2,$2,0x8214
	sw	$2,92($sp)
	lw	$2,92($sp)
	lw	$2,0($2)
	andi	$2,$2,0x8
	beq	$2,$0,$L251
	nop

	li	$2,-1078001664			# 0xffffffffbfbf0000
	ori	$2,$2,0x8214
	sw	$2,96($sp)
	lw	$2,96($sp)
	lw	$2,0($2)
	andi	$2,$2,0x20
	beq	$2,$0,$L253
	nop

	j	$L248
	nop

$L251:
	li	$2,-1078001664			# 0xffffffffbfbf0000
	ori	$2,$2,0x8214
	sw	$2,100($sp)
	lw	$2,100($sp)
	lw	$2,0($2)
	andi	$2,$2,0x2
	bne	$2,$0,$L248
	nop

$L253:
	li	$2,-1078001664			# 0xffffffffbfbf0000
	ori	$2,$2,0x8214
	sw	$2,104($sp)
	lw	$2,104($sp)
	lw	$2,0($2)
	andi	$2,$2,0x8
	beq	$2,$0,$L256
	nop

	li	$2,-1078001664			# 0xffffffffbfbf0000
	ori	$2,$2,0x8214
	sw	$2,108($sp)
	lw	$2,108($sp)
	lw	$2,0($2)
	andi	$2,$2,0x40
	beq	$2,$0,$L238
	nop

	j	$L248
	nop

$L256:
	li	$2,-1078001664			# 0xffffffffbfbf0000
	ori	$2,$2,0x8214
	sw	$2,112($sp)
	lw	$2,112($sp)
	lw	$2,0($2)
	andi	$2,$2,0x4
	beq	$2,$0,$L238
	nop

$L248:
	li	$2,-1078984704			# 0xffffffffbfb00000
	ori	$2,$2,0xf8
	sw	$2,116($sp)
	lw	$2,116($sp)
	lw	$2,0($2)
	andi	$3,$2,0x3
	li	$2,3			# 0x3
	bne	$3,$2,$L260
	nop

	li	$2,-1078984704			# 0xffffffffbfb00000
	ori	$2,$2,0x64
	sw	$2,120($sp)
	lw	$2,120($sp)
	lw	$3,0($2)
	li	$2,-65536			# 0xffffffffffff0000
	and	$3,$3,$2
	li	$2,327680			# 0x50000
	bne	$3,$2,$L260
	nop

	li	$2,-1078001664			# 0xffffffffbfbf0000
	ori	$2,$2,0x8214
	sw	$2,124($sp)
	lw	$2,124($sp)
	lw	$2,0($2)
	andi	$2,$2,0x8
	beq	$2,$0,$L263
	nop

	li	$2,-1078001664			# 0xffffffffbfbf0000
	ori	$2,$2,0x8214
	sw	$2,128($sp)
	lw	$2,128($sp)
	lw	$2,0($2)
	andi	$2,$2,0x20
	beq	$2,$0,$L238
	nop

	j	$L260
	nop

$L263:
	li	$2,-1078001664			# 0xffffffffbfbf0000
	ori	$2,$2,0x8214
	sw	$2,132($sp)
	lw	$2,132($sp)
	lw	$2,0($2)
	andi	$2,$2,0x2
	beq	$2,$0,$L238
	nop

$L260:
	li	$2,-1078984704			# 0xffffffffbfb00000
	ori	$2,$2,0xf8
	sw	$2,136($sp)
	lw	$2,136($sp)
	lw	$2,0($2)
	andi	$3,$2,0x3
	li	$2,2			# 0x2
	bne	$3,$2,$L225
	nop

	li	$2,-1078984704			# 0xffffffffbfb00000
	ori	$2,$2,0x64
	sw	$2,140($sp)
	lw	$2,140($sp)
	lw	$3,0($2)
	li	$2,-65536			# 0xffffffffffff0000
	and	$3,$3,$2
	li	$2,327680			# 0x50000
	bne	$3,$2,$L225
	nop

	li	$2,-1078001664			# 0xffffffffbfbf0000
	ori	$2,$2,0x8214
	sw	$2,144($sp)
	lw	$2,144($sp)
	lw	$2,0($2)
	andi	$2,$2,0x8
	beq	$2,$0,$L269
	nop

	li	$2,-1078001664			# 0xffffffffbfbf0000
	ori	$2,$2,0x8214
	sw	$2,148($sp)
	lw	$2,148($sp)
	lw	$2,0($2)
	andi	$2,$2,0x40
	bne	$2,$0,$L238
	nop

	j	$L225
	nop

$L269:
	li	$2,-1078001664			# 0xffffffffbfbf0000
	ori	$2,$2,0x8214
	sw	$2,152($sp)
	lw	$2,152($sp)
	lw	$2,0($2)
	andi	$2,$2,0x4
	beq	$2,$0,$L225
	nop

$L238:
	li	$2,64			# 0x40
	j	$L272
	nop

$L225:
	li	$2,95			# 0x5f
$L272:
	slt	$2,$2,$4
	beq	$2,$0,$L273
	nop

	li	$2,-1078984704			# 0xffffffffbfb00000
	ori	$2,$2,0x64
	sw	$2,156($sp)
	lw	$2,156($sp)
	lw	$2,0($2)
	andi	$3,$2,0xf
	li	$2,3			# 0x3
	bne	$3,$2,$L275
	nop

	li	$2,-1078657024			# 0xffffffffbfb50000
	ori	$2,$2,0xfffc
	sw	$2,160($sp)
	lw	$2,160($sp)
	lw	$2,0($2)
	andi	$2,$2,0xf
	beq	$2,$0,$L275
	nop

	li	$2,-1078984704			# 0xffffffffbfb00000
	ori	$2,$2,0xf8
	sw	$2,164($sp)
	lw	$2,164($sp)
	lw	$2,0($2)
	andi	$3,$2,0x3
	li	$2,2			# 0x2
	bne	$3,$2,$L278
	nop

	li	$2,-1078984704			# 0xffffffffbfb00000
	ori	$2,$2,0x64
	sw	$2,168($sp)
	lw	$2,168($sp)
	lw	$3,0($2)
	li	$2,-65536			# 0xffffffffffff0000
	and	$3,$3,$2
	li	$2,327680			# 0x50000
	bne	$3,$2,$L278
	nop

	li	$2,-1078001664			# 0xffffffffbfbf0000
	ori	$2,$2,0x8214
	sw	$2,172($sp)
	lw	$2,172($sp)
	lw	$2,0($2)
	andi	$2,$2,0x8
	beq	$2,$0,$L281
	nop

	li	$2,-1078001664			# 0xffffffffbfbf0000
	ori	$2,$2,0x8214
	sw	$2,176($sp)
	lw	$2,176($sp)
	lw	$2,0($2)
	andi	$2,$2,0x20
	bne	$2,$0,$L283
	nop

	j	$L278
	nop

$L281:
	li	$2,-1078001664			# 0xffffffffbfbf0000
	ori	$2,$2,0x8214
	sw	$2,180($sp)
	lw	$2,180($sp)
	lw	$2,0($2)
	andi	$2,$2,0x2
	beq	$2,$0,$L278
	nop

$L283:
	li	$2,-1078001664			# 0xffffffffbfbf0000
	ori	$2,$2,0x8214
	sw	$2,184($sp)
	lw	$2,184($sp)
	lw	$2,0($2)
	andi	$2,$2,0x8
	beq	$2,$0,$L286
	nop

	li	$2,-1078001664			# 0xffffffffbfbf0000
	ori	$2,$2,0x8214
	sw	$2,188($sp)
	lw	$2,188($sp)
	lw	$2,0($2)
	andi	$2,$2,0x40
	beq	$2,$0,$L288
	nop

	j	$L278
	nop

$L286:
	li	$2,-1078001664			# 0xffffffffbfbf0000
	ori	$2,$2,0x8214
	sw	$2,192($sp)
	lw	$2,192($sp)
	lw	$2,0($2)
	andi	$2,$2,0x4
	beq	$2,$0,$L288
	nop

$L278:
	li	$2,-1078984704			# 0xffffffffbfb00000
	ori	$2,$2,0xf8
	sw	$2,196($sp)
	lw	$2,196($sp)
	lw	$2,0($2)
	andi	$3,$2,0x3
	li	$2,3			# 0x3
	bne	$3,$2,$L291
	nop

	li	$2,-1078984704			# 0xffffffffbfb00000
	ori	$2,$2,0x64
	sw	$2,200($sp)
	lw	$2,200($sp)
	lw	$3,0($2)
	li	$2,-65536			# 0xffffffffffff0000
	and	$3,$3,$2
	li	$2,327680			# 0x50000
	bne	$3,$2,$L291
	nop

	li	$2,-1078001664			# 0xffffffffbfbf0000
	ori	$2,$2,0x8214
	sw	$2,204($sp)
	lw	$2,204($sp)
	lw	$2,0($2)
	andi	$2,$2,0x8
	beq	$2,$0,$L294
	nop

	li	$2,-1078001664			# 0xffffffffbfbf0000
	ori	$2,$2,0x8214
	sw	$2,208($sp)
	lw	$2,208($sp)
	lw	$2,0($2)
	andi	$2,$2,0x20
	bne	$2,$0,$L288
	nop

	j	$L291
	nop

$L294:
	li	$2,-1078001664			# 0xffffffffbfbf0000
	ori	$2,$2,0x8214
	sw	$2,212($sp)
	lw	$2,212($sp)
	lw	$2,0($2)
	andi	$2,$2,0x2
	bne	$2,$0,$L288
	nop

$L291:
	li	$2,-1078984704			# 0xffffffffbfb00000
	ori	$2,$2,0xf8
	sw	$2,216($sp)
	lw	$2,216($sp)
	lw	$2,0($2)
	andi	$3,$2,0x3
	li	$2,2			# 0x2
	bne	$3,$2,$L298
	nop

	li	$2,-1078984704			# 0xffffffffbfb00000
	ori	$2,$2,0x64
	sw	$2,220($sp)
	lw	$2,220($sp)
	lw	$3,0($2)
	li	$2,-65536			# 0xffffffffffff0000
	and	$3,$3,$2
	li	$2,327680			# 0x50000
	bne	$3,$2,$L298
	nop

	li	$2,-1078001664			# 0xffffffffbfbf0000
	ori	$2,$2,0x8214
	sw	$2,224($sp)
	lw	$2,224($sp)
	lw	$2,0($2)
	andi	$2,$2,0x8
	beq	$2,$0,$L301
	nop

	li	$2,-1078001664			# 0xffffffffbfbf0000
	ori	$2,$2,0x8214
	sw	$2,228($sp)
	lw	$2,228($sp)
	lw	$2,0($2)
	andi	$2,$2,0x20
	beq	$2,$0,$L303
	nop

	j	$L298
	nop

$L301:
	li	$2,-1078001664			# 0xffffffffbfbf0000
	ori	$2,$2,0x8214
	sw	$2,232($sp)
	lw	$2,232($sp)
	lw	$2,0($2)
	andi	$2,$2,0x2
	bne	$2,$0,$L298
	nop

$L303:
	li	$2,-1078001664			# 0xffffffffbfbf0000
	ori	$2,$2,0x8214
	sw	$2,236($sp)
	lw	$2,236($sp)
	lw	$2,0($2)
	andi	$2,$2,0x8
	beq	$2,$0,$L306
	nop

	li	$2,-1078001664			# 0xffffffffbfbf0000
	ori	$2,$2,0x8214
	sw	$2,240($sp)
	lw	$2,240($sp)
	lw	$2,0($2)
	andi	$2,$2,0x40
	beq	$2,$0,$L288
	nop

	j	$L298
	nop

$L306:
	li	$2,-1078001664			# 0xffffffffbfbf0000
	ori	$2,$2,0x8214
	sw	$2,244($sp)
	lw	$2,244($sp)
	lw	$2,0($2)
	andi	$2,$2,0x4
	beq	$2,$0,$L288
	nop

$L298:
	li	$2,-1078984704			# 0xffffffffbfb00000
	ori	$2,$2,0xf8
	sw	$2,248($sp)
	lw	$2,248($sp)
	lw	$2,0($2)
	andi	$3,$2,0x3
	li	$2,3			# 0x3
	bne	$3,$2,$L310
	nop

	li	$2,-1078984704			# 0xffffffffbfb00000
	ori	$2,$2,0x64
	sw	$2,252($sp)
	lw	$2,252($sp)
	lw	$3,0($2)
	li	$2,-65536			# 0xffffffffffff0000
	and	$3,$3,$2
	li	$2,327680			# 0x50000
	bne	$3,$2,$L310
	nop

	li	$2,-1078001664			# 0xffffffffbfbf0000
	ori	$2,$2,0x8214
	sw	$2,256($sp)
	lw	$2,256($sp)
	lw	$2,0($2)
	andi	$2,$2,0x8
	beq	$2,$0,$L313
	nop

	li	$2,-1078001664			# 0xffffffffbfbf0000
	ori	$2,$2,0x8214
	sw	$2,260($sp)
	lw	$2,260($sp)
	lw	$2,0($2)
	andi	$2,$2,0x20
	beq	$2,$0,$L288
	nop

	j	$L310
	nop

$L313:
	li	$2,-1078001664			# 0xffffffffbfbf0000
	ori	$2,$2,0x8214
	sw	$2,264($sp)
	lw	$2,264($sp)
	lw	$2,0($2)
	andi	$2,$2,0x2
	beq	$2,$0,$L288
	nop

$L310:
	li	$2,-1078984704			# 0xffffffffbfb00000
	ori	$2,$2,0xf8
	sw	$2,268($sp)
	lw	$2,268($sp)
	lw	$2,0($2)
	andi	$3,$2,0x3
	li	$2,2			# 0x2
	bne	$3,$2,$L275
	nop

	li	$2,-1078984704			# 0xffffffffbfb00000
	ori	$2,$2,0x64
	sw	$2,272($sp)
	lw	$2,272($sp)
	lw	$3,0($2)
	li	$2,-65536			# 0xffffffffffff0000
	and	$3,$3,$2
	li	$2,327680			# 0x50000
	bne	$3,$2,$L275
	nop

	li	$2,-1078001664			# 0xffffffffbfbf0000
	ori	$2,$2,0x8214
	sw	$2,276($sp)
	lw	$2,276($sp)
	lw	$2,0($2)
	andi	$2,$2,0x8
	beq	$2,$0,$L319
	nop

	li	$2,-1078001664			# 0xffffffffbfbf0000
	ori	$2,$2,0x8214
	sw	$2,280($sp)
	lw	$2,280($sp)
	lw	$2,0($2)
	andi	$2,$2,0x40
	bne	$2,$0,$L288
	nop

	j	$L275
	nop

$L319:
	li	$2,-1078001664			# 0xffffffffbfbf0000
	ori	$2,$2,0x8214
	sw	$2,284($sp)
	lw	$2,284($sp)
	lw	$2,0($2)
	andi	$2,$2,0x4
	beq	$2,$0,$L275
	nop

$L288:
	li	$2,64			# 0x40
	j	$L322
	nop

$L275:
	li	$2,95			# 0x5f
$L322:
	lui	$3,%hi(g_EPON_MAC_BASE)
	lw	$3,%lo(g_EPON_MAC_BASE)($3)
	addiu	$3,$3,24788
	move	$4,$2
	move	$5,$3
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	lui	$2,%hi(g_EPON_MAC_BASE)
	lw	$2,%lo(g_EPON_MAC_BASE)($2)
	addiu	$3,$2,24820
	li	$2,9306112			# 0x8e0000
	ori	$4,$2,0xfff1
	move	$5,$3
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	j	$L323
	nop

$L273:
	lhu	$2,302($sp)
	bne	$2,$0,$L324
	nop

	lui	$2,%hi(gMpcpSynctime)
	lhu	$2,%lo(gMpcpSynctime)($2)
	move	$3,$2
	lui	$2,%hi(g_EPON_MAC_BASE)
	lw	$2,%lo(g_EPON_MAC_BASE)($2)
	addiu	$2,$2,24788
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	lui	$2,%hi(g_EPON_MAC_BASE)
	lw	$2,%lo(g_EPON_MAC_BASE)($2)
	addiu	$2,$2,24788
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	sw	$2,300($sp)
	lui	$2,%hi(gMpcpSynctime)
	lhu	$2,%lo(gMpcpSynctime)($2)
	sll	$3,$2,16
	li	$2,3080192			# 0x2f0000
	ori	$2,$2,0xfff1
	addu	$2,$3,$2
	move	$3,$2
	lui	$2,%hi(g_EPON_MAC_BASE)
	lw	$2,%lo(g_EPON_MAC_BASE)($2)
	addiu	$2,$2,24820
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	lui	$2,%hi(gMpcpSynctime)
	lhu	$2,%lo(gMpcpSynctime)($2)
	move	$3,$2
	li	$4,4			# 0x4
	lui	$2,%hi($LC5)
	addiu	$5,$2,%lo($LC5)
	lui	$2,%hi(__func__.41718)
	addiu	$6,$2,%lo(__func__.41718)
	move	$7,$3
	lui	$2,%hi(dbgtoMem)
	addiu	$2,$2,%lo(dbgtoMem)
	jalr	$2
	nop

	j	$L323
	nop

$L324:
	lhu	$2,302($sp)
	sll	$3,$2,16
	li	$2,3080192			# 0x2f0000
	ori	$2,$2,0xfff1
	addu	$2,$3,$2
	move	$3,$2
	lui	$2,%hi(g_EPON_MAC_BASE)
	lw	$2,%lo(g_EPON_MAC_BASE)($2)
	addiu	$2,$2,24820
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	lhu	$3,302($sp)
	lui	$2,%hi(gMpcpSynctime)
	lhu	$2,%lo(gMpcpSynctime)($2)
	beq	$3,$2,$L323
	nop

	lhu	$3,302($sp)
	lui	$2,%hi(gMpcpSynctime)
	sh	$3,%lo(gMpcpSynctime)($2)
$L323:
	lw	$3,288($sp)
	li	$2,50331648			# 0x3000000
	and	$3,$3,$2
	li	$2,33554432			# 0x2000000
	bne	$3,$2,$L325
	nop

	lw	$3,288($sp)
	li	$2,65536			# 0x10000
	and	$2,$3,$2
	beq	$2,$0,$L326
	nop

	lhu	$3,290($sp)
	lw	$2,16($sp)
	sh	$3,2($2)
	lbu	$3,312($sp)
	lw	$2,16($sp)
	lhu	$2,2($2)
	move	$7,$2
	li	$4,4			# 0x4
	lui	$2,%hi($LC28)
	addiu	$5,$2,%lo($LC28)
	move	$6,$3
	lui	$2,%hi(dbgtoMem)
	addiu	$2,$2,%lo(dbgtoMem)
	jalr	$2
	nop

	lw	$2,20($sp)
	li	$3,3			# 0x3
	sb	$3,0($2)
	lbu	$2,312($sp)
	move	$4,$2
	lui	$2,%hi(eponMpcpDscvFsmPendingHandler)
	addiu	$2,$2,%lo(eponMpcpDscvFsmPendingHandler)
	jalr	$2
	nop

	lui	$2,%hi(gFeResetRetryCnt)
	sw	$0,%lo(gFeResetRetryCnt)($2)
	lui	$2,%hi(gDiscoveryGateCnt)
	sw	$0,%lo(gDiscoveryGateCnt)($2)
	li	$4,4			# 0x4
	lui	$2,%hi($LC30)
	addiu	$5,$2,%lo($LC30)
	lui	$2,%hi(dbgtoMem)
	addiu	$2,$2,%lo(dbgtoMem)
	jalr	$2
	nop

	j	$L325
	nop

$L326:
	li	$4,1			# 0x1
	lui	$2,%hi($LC29)
	addiu	$5,$2,%lo($LC29)
	lui	$2,%hi(dbgtoMem)
	addiu	$2,$2,%lo(dbgtoMem)
	jalr	$2
	nop

	li	$2,-1			# 0xffffffffffffffff
	j	$L335
	nop

$L325:
	lw	$3,288($sp)
	li	$2,50331648			# 0x3000000
	and	$3,$3,$2
	li	$2,50331648			# 0x3000000
	bne	$3,$2,$L331
	nop

	lbu	$3,312($sp)
	li	$4,2			# 0x2
	lui	$2,%hi($LC31)
	addiu	$5,$2,%lo($LC31)
	move	$6,$3
	lui	$2,%hi(dbgtoMem)
	addiu	$2,$2,%lo(dbgtoMem)
	jalr	$2
	nop

	lw	$2,20($sp)
	li	$3,5			# 0x5
	sb	$3,0($2)
	lbu	$2,312($sp)
	li	$4,2			# 0x2
	li	$5,4			# 0x4
	move	$6,$2
	lui	$2,%hi(xmcs_report_event)
	addiu	$2,$2,%lo(xmcs_report_event)
	jalr	$2
	nop

	lui	$2,%hi(g_silence_time)
	lbu	$2,%lo(g_silence_time)($2)
	bne	$2,$0,$L329
	nop

	lw	$2,20($sp)
	sb	$0,0($2)
	j	$L330
	nop

$L329:
	lui	$2,%hi(g_silence_time)
	lbu	$2,%lo(g_silence_time)($2)
	andi	$3,$2,0xffff
	lw	$2,16($sp)
	sh	$3,68($2)
	lui	$2,%hi(g_silence_time)
	lbu	$2,%lo(g_silence_time)($2)
	move	$3,$2
	li	$4,2			# 0x2
	lui	$2,%hi($LC32)
	addiu	$5,$2,%lo($LC32)
	move	$6,$3
	lui	$2,%hi(dbgtoMem)
	addiu	$2,$2,%lo(dbgtoMem)
	jalr	$2
	nop

$L330:
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
	lbu	$2,312($sp)
	move	$4,$2
	lui	$2,%hi(eponMpcpDscvFsmWaitHandler)
	addiu	$2,$2,%lo(eponMpcpDscvFsmWaitHandler)
	jalr	$2
	nop

	j	$L331
	nop

$L223:
	lw	$2,20($sp)
	lbu	$3,0($2)
	li	$2,8			# 0x8
	bne	$3,$2,$L331
	nop

	li	$4,4			# 0x4
	lui	$2,%hi($LC33)
	addiu	$5,$2,%lo($LC33)
	lui	$2,%hi(dbgtoMem)
	addiu	$2,$2,%lo(dbgtoMem)
	jalr	$2
	nop

	lw	$3,288($sp)
	li	$2,50331648			# 0x3000000
	and	$2,$3,$2
	bne	$2,$0,$L332
	nop

	lbu	$3,312($sp)
	li	$4,2			# 0x2
	lui	$2,%hi($LC34)
	addiu	$5,$2,%lo($LC34)
	move	$6,$3
	lui	$2,%hi(dbgtoMem)
	addiu	$2,$2,%lo(dbgtoMem)
	jalr	$2
	nop

	lui	$2,%hi(g_EPON_MAC_BASE)
	lw	$2,%lo(g_EPON_MAC_BASE)($2)
	addiu	$3,$2,24620
	lbu	$2,312($sp)
	sll	$2,$2,2
	addu	$2,$3,$2
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	sw	$2,292($sp)
	lw	$2,292($sp)
	li	$3,1			# 0x1
	ins	$2,$3,30,2
	sw	$2,292($sp)
	lw	$4,292($sp)
	lui	$2,%hi(g_EPON_MAC_BASE)
	lw	$2,%lo(g_EPON_MAC_BASE)($2)
	addiu	$3,$2,24620
	lbu	$2,312($sp)
	sll	$2,$2,2
	addu	$2,$3,$2
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	lw	$2,20($sp)
	li	$3,3			# 0x3
	sb	$3,0($2)
	li	$4,2			# 0x2
	lui	$2,%hi($LC35)
	addiu	$5,$2,%lo($LC35)
	lui	$2,%hi(dbgtoMem)
	addiu	$2,$2,%lo(dbgtoMem)
	jalr	$2
	nop

	lbu	$2,312($sp)
	move	$4,$2
	lui	$2,%hi(eponMpcpDscvFsmPendingHandler)
	addiu	$2,$2,%lo(eponMpcpDscvFsmPendingHandler)
	jalr	$2
	nop

$L332:
	lw	$3,288($sp)
	li	$2,50331648			# 0x3000000
	and	$3,$3,$2
	li	$2,16777216			# 0x1000000
	bne	$3,$2,$L331
	nop

	lbu	$3,312($sp)
	li	$4,2			# 0x2
	lui	$2,%hi($LC36)
	addiu	$5,$2,%lo($LC36)
	move	$6,$3
	lui	$2,%hi(dbgtoMem)
	addiu	$2,$2,%lo(dbgtoMem)
	jalr	$2
	nop

	lui	$2,%hi(isAuthFail)
	lw	$2,%lo(isAuthFail)($2)
	beq	$2,$0,$L333
	nop

	lw	$2,20($sp)
	li	$3,5			# 0x5
	sb	$3,0($2)
	lbu	$2,312($sp)
	li	$4,2			# 0x2
	li	$5,4			# 0x4
	move	$6,$2
	lui	$2,%hi(xmcs_report_event)
	addiu	$2,$2,%lo(xmcs_report_event)
	jalr	$2
	nop

	li	$4,2			# 0x2
	lui	$2,%hi($LC37)
	addiu	$5,$2,%lo($LC37)
	lui	$2,%hi(dbgtoMem)
	addiu	$2,$2,%lo(dbgtoMem)
	jalr	$2
	nop

	lui	$2,%hi(g_silence_time)
	lbu	$2,%lo(g_silence_time)($2)
	move	$3,$2
	li	$4,2			# 0x2
	lui	$2,%hi($LC38)
	addiu	$5,$2,%lo($LC38)
	move	$6,$3
	lui	$2,%hi(dbgtoMem)
	addiu	$2,$2,%lo(dbgtoMem)
	jalr	$2
	nop

	lui	$2,%hi(g_silence_time)
	lbu	$2,%lo(g_silence_time)($2)
	andi	$3,$2,0xffff
	lw	$2,16($sp)
	sh	$3,68($2)
	j	$L334
	nop

$L333:
	lw	$2,20($sp)
	li	$3,9			# 0x9
	sb	$3,0($2)
	lbu	$2,312($sp)
	li	$4,2			# 0x2
	li	$5,3			# 0x3
	move	$6,$2
	lui	$2,%hi(xmcs_report_event)
	addiu	$2,$2,%lo(xmcs_report_event)
	jalr	$2
	nop

	li	$4,2			# 0x2
	lui	$2,%hi($LC39)
	addiu	$5,$2,%lo($LC39)
	lui	$2,%hi(dbgtoMem)
	addiu	$2,$2,%lo(dbgtoMem)
	jalr	$2
	nop

	lbu	$2,312($sp)
	move	$4,$2
	lui	$2,%hi(xmcs_remove_llid)
	addiu	$2,$2,%lo(xmcs_remove_llid)
	jalr	$2
	nop

	lw	$2,20($sp)
	sb	$0,0($2)
$L334:
	lbu	$2,312($sp)
	move	$4,$2
	lui	$2,%hi(eponMpcpDscvFsmWaitHandler)
	addiu	$2,$2,%lo(eponMpcpDscvFsmWaitHandler)
	jalr	$2
	nop

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
	move	$4,$0
	lui	$2,%hi(eponFirmwareSetMode)
	addiu	$2,$2,%lo(eponFirmwareSetMode)
	jalr	$2
	nop

$L331:
	move	$2,$0
$L335:
	lw	$31,308($sp)
	addiu	$sp,$sp,312
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	eponMpcpRgstIntHandler
	.size	eponMpcpRgstIntHandler, .-eponMpcpRgstIntHandler
	.rdata
	.align	2
$LC40:
	.ascii	"ERR: eponMpcpLocalDergstr mpcpDiscFsm->begin == FALSE\000"
	.align	2
$LC41:
	.ascii	"eponMpcpLocalDergstr done bit not ready\000"
	.align	2
$LC42:
	.ascii	"Receive Local Deregister Message , Report DEREGISTER Eve"
	.ascii	"nt\000"
	.align	2
$LC43:
	.ascii	"eponMpcpLocalDergstr send dereigster dscvCtrl.Raw=0x%x\000"
	.align	2
$LC44:
	.ascii	"REPORT REGISTER EVENT\000"
	.section	.text.eponMpcpLocalDergstr,"ax",@progbits
	.align	2
	.globl	eponMpcpLocalDergstr
	.set	nomips16
	.set	nomicromips
	.ent	eponMpcpLocalDergstr
	.type	eponMpcpLocalDergstr, @function
eponMpcpLocalDergstr:
	.frame	$sp,40,$31		# vars= 16, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-40
	sw	$31,36($sp)
	move	$2,$4
	sb	$2,40($sp)
	lbu	$2,40($sp)
	sll	$2,$2,3
	sll	$3,$2,3
	addu	$2,$2,$3
	addiu	$3,$2,16
	lui	$2,%hi(eponDrv)
	addiu	$2,$2,%lo(eponDrv)
	addu	$2,$3,$2
	addiu	$2,$2,4
	sw	$2,16($sp)
	lw	$2,16($sp)
	addiu	$2,$2,20
	sw	$2,20($sp)
	sw	$0,28($sp)
	lw	$2,20($sp)
	lw	$2,4($2)
	bne	$2,$0,$L337
	nop

	li	$4,1			# 0x1
	lui	$2,%hi($LC40)
	addiu	$5,$2,%lo($LC40)
	lui	$2,%hi(dbgtoMem)
	addiu	$2,$2,%lo(dbgtoMem)
	jalr	$2
	nop

	li	$2,-1			# 0xffffffffffffffff
	j	$L345
	nop

$L337:
	lw	$2,20($sp)
	lbu	$3,0($2)
	li	$2,8			# 0x8
	bne	$3,$2,$L339
	nop

	lui	$2,%hi(eponMacCheckRegreqDoneBit)
	addiu	$2,$2,%lo(eponMacCheckRegreqDoneBit)
	jalr	$2
	nop

	bgez	$2,$L340
	nop

	li	$4,4			# 0x4
	lui	$2,%hi($LC41)
	addiu	$5,$2,%lo($LC41)
	lui	$2,%hi(dbgtoMem)
	addiu	$2,$2,%lo(dbgtoMem)
	jalr	$2
	nop

	li	$2,-1			# 0xffffffffffffffff
	j	$L345
	nop

$L340:
	lw	$2,20($sp)
	li	$3,10			# 0xa
	sb	$3,0($2)
	li	$4,4			# 0x4
	lui	$2,%hi($LC42)
	addiu	$5,$2,%lo($LC42)
	lui	$2,%hi(dbgtoMem)
	addiu	$2,$2,%lo(dbgtoMem)
	jalr	$2
	nop

	move	$4,$0
	lui	$2,%hi(eponFirmwareSetMode)
	addiu	$2,$2,%lo(eponFirmwareSetMode)
	jalr	$2
	nop

	sw	$0,24($sp)
	lw	$2,24($sp)
	li	$3,2			# 0x2
	ins	$2,$3,30,2
	sw	$2,24($sp)
	lw	$2,24($sp)
	li	$3,1			# 0x1
	ins	$2,$3,8,1
	sw	$2,24($sp)
	lbu	$2,40($sp)
	andi	$2,$2,0x7
	andi	$3,$2,0x00ff
	lw	$2,24($sp)
	ins	$2,$3,0,3
	sw	$2,24($sp)
	lw	$2,24($sp)
	li	$3,1			# 0x1
	ins	$2,$3,16,1
	sw	$2,24($sp)
	lw	$3,24($sp)
	lui	$2,%hi(g_EPON_MAC_BASE)
	lw	$2,%lo(g_EPON_MAC_BASE)($2)
	addiu	$2,$2,24616
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	lw	$3,24($sp)
	li	$4,4			# 0x4
	lui	$2,%hi($LC43)
	addiu	$5,$2,%lo($LC43)
	move	$6,$3
	lui	$2,%hi(dbgtoMem)
	addiu	$2,$2,%lo(dbgtoMem)
	jalr	$2
	nop

$L343:
	lui	$2,%hi(g_EPON_MAC_BASE)
	lw	$2,%lo(g_EPON_MAC_BASE)($2)
	addiu	$2,$2,24616
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	sw	$2,24($sp)
	lw	$3,24($sp)
	li	$2,65536			# 0x10000
	and	$2,$3,$2
	beq	$2,$0,$L341
	nop

	nop
	lw	$2,20($sp)
	li	$3,8			# 0x8
	sb	$3,0($2)
	lui	$2,%hi(eponClearRetryCounter)
	addiu	$2,$2,%lo(eponClearRetryCounter)
	jalr	$2
	nop

	li	$4,4			# 0x4
	lui	$2,%hi($LC44)
	addiu	$5,$2,%lo($LC44)
	lui	$2,%hi(dbgtoMem)
	addiu	$2,$2,%lo(dbgtoMem)
	jalr	$2
	nop

	li	$4,1			# 0x1
	lui	$2,%hi(eponFirmwareSetMode)
	addiu	$2,$2,%lo(eponFirmwareSetMode)
	jalr	$2
	nop

	j	$L339
	nop

$L341:
	j	$L343
	nop

$L339:
	lw	$2,20($sp)
	lbu	$3,0($2)
	li	$2,2			# 0x2
	bne	$3,$2,$L344
	nop

	lw	$2,20($sp)
	sb	$0,0($2)
	lbu	$2,40($sp)
	move	$4,$2
	lui	$2,%hi(eponMpcpDscvFsmWaitHandler)
	addiu	$2,$2,%lo(eponMpcpDscvFsmWaitHandler)
	jalr	$2
	nop

$L344:
	move	$2,$0
$L345:
	lw	$31,36($sp)
	addiu	$sp,$sp,40
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	eponMpcpLocalDergstr
	.size	eponMpcpLocalDergstr, .-eponMpcpLocalDergstr
	.rdata
	.align	2
$LC45:
	.ascii	"ERR: eponMpcpSetDiscvRgstAck param error\000"
	.section	.text.eponMpcpSetDiscvRgstAck,"ax",@progbits
	.align	2
	.globl	eponMpcpSetDiscvRgstAck
	.set	nomips16
	.set	nomicromips
	.ent	eponMpcpSetDiscvRgstAck
	.type	eponMpcpSetDiscvRgstAck, @function
eponMpcpSetDiscvRgstAck:
	.frame	$sp,24,$31		# vars= 0, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-24
	sw	$31,20($sp)
	move	$3,$4
	move	$2,$5
	sb	$3,24($sp)
	sb	$2,28($sp)
	lbu	$2,24($sp)
	sltu	$2,$2,9
	beq	$2,$0,$L347
	nop

	lbu	$2,28($sp)
	sltu	$2,$2,2
	bne	$2,$0,$L348
	nop

$L347:
	li	$4,1			# 0x1
	lui	$2,%hi($LC45)
	addiu	$5,$2,%lo($LC45)
	lui	$2,%hi(dbgtoMem)
	addiu	$2,$2,%lo(dbgtoMem)
	jalr	$2
	nop

	li	$2,-1			# 0xffffffffffffffff
	j	$L349
	nop

$L348:
	lbu	$2,24($sp)
	lui	$3,%hi(eponDrv)
	sll	$2,$2,3
	sll	$4,$2,3
	addu	$2,$2,$4
	addiu	$3,$3,%lo(eponDrv)
	addu	$2,$2,$3
	lbu	$3,28($sp)
	sb	$3,41($2)
	move	$2,$0
$L349:
	lw	$31,20($sp)
	addiu	$sp,$sp,24
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	eponMpcpSetDiscvRgstAck
	.size	eponMpcpSetDiscvRgstAck, .-eponMpcpSetDiscvRgstAck
	.globl	eponTimeDrftHandleFlag
	.data
	.type	eponTimeDrftHandleFlag, @object
	.size	eponTimeDrftHandleFlag, 1
eponTimeDrftHandleFlag:
	.byte	1
	.rdata
	.align	2
$LC46:
	.ascii	"eponTimeDrftIntHandler 255\000"
	.align	2
$LC47:
	.ascii	"eponTimeDrftIntHandler 8~16 Raw=%d\000"
	.align	2
$LC48:
	.ascii	"eponTimeDrftIntHandler error Raw=%d\000"
	.section	.text.eponTimeDrftIntHandler,"ax",@progbits
	.align	2
	.globl	eponTimeDrftIntHandler
	.set	nomips16
	.set	nomicromips
	.ent	eponTimeDrftIntHandler
	.type	eponTimeDrftIntHandler, @function
eponTimeDrftIntHandler:
	.frame	$sp,32,$31		# vars= 8, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-32
	sw	$31,28($sp)
	sw	$4,32($sp)
	sw	$0,16($sp)
	lui	$2,%hi(eponTimeDrftHandleFlag)
	lbu	$3,%lo(eponTimeDrftHandleFlag)($2)
	li	$2,1			# 0x1
	bne	$3,$2,$L351
	nop

	lui	$2,%hi(g_EPON_MAC_BASE)
	lw	$2,%lo(g_EPON_MAC_BASE)($2)
	addiu	$2,$2,24884
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	sw	$2,16($sp)
	lw	$2,16($sp)
	andi	$2,$2,0xff
	sw	$2,16($sp)
	lw	$3,16($sp)
	li	$2,255			# 0xff
	bne	$3,$2,$L352
	nop

	lui	$2,%hi(eponDebugLevel)
	lw	$2,%lo(eponDebugLevel)($2)
	sltu	$2,$2,4
	bne	$2,$0,$L353
	nop

	lui	$2,%hi(eponMacDumpAllReg)
	addiu	$2,$2,%lo(eponMacDumpAllReg)
	jalr	$2
	nop

$L353:
	li	$4,4			# 0x4
	lui	$2,%hi($LC46)
	addiu	$5,$2,%lo($LC46)
	lui	$2,%hi(dbgtoMem)
	addiu	$2,$2,%lo(dbgtoMem)
	jalr	$2
	nop

	lui	$2,%hi(eponDrv)
	addiu	$2,$2,%lo(eponDrv)
	lw	$2,12($2)
	addiu	$3,$2,1
	lui	$2,%hi(eponDrv)
	addiu	$2,$2,%lo(eponDrv)
	sw	$3,12($2)
	j	$L351
	nop

$L352:
	lw	$2,16($sp)
	sltu	$2,$2,17
	beq	$2,$0,$L354
	nop

	li	$4,4			# 0x4
	lui	$2,%hi($LC47)
	addiu	$5,$2,%lo($LC47)
	lw	$6,16($sp)
	lui	$2,%hi(dbgtoMem)
	addiu	$2,$2,%lo(dbgtoMem)
	jalr	$2
	nop

	lui	$2,%hi(eponDebugLevel)
	lw	$2,%lo(eponDebugLevel)($2)
	sltu	$2,$2,4
	bne	$2,$0,$L355
	nop

	lui	$2,%hi(eponMacDumpAllReg)
	addiu	$2,$2,%lo(eponMacDumpAllReg)
	jalr	$2
	nop

$L355:
	lui	$2,%hi(eponDrv)
	addiu	$2,$2,%lo(eponDrv)
	lw	$2,16($2)
	addiu	$3,$2,1
	lui	$2,%hi(eponDrv)
	addiu	$2,$2,%lo(eponDrv)
	sw	$3,16($2)
	j	$L351
	nop

$L354:
	li	$4,4			# 0x4
	lui	$2,%hi($LC48)
	addiu	$5,$2,%lo($LC48)
	lw	$6,16($sp)
	lui	$2,%hi(dbgtoMem)
	addiu	$2,$2,%lo(dbgtoMem)
	jalr	$2
	nop

	lui	$2,%hi(eponDebugLevel)
	lw	$2,%lo(eponDebugLevel)($2)
	sltu	$2,$2,4
	bne	$2,$0,$L356
	nop

	lui	$2,%hi(eponMacDumpAllReg)
	addiu	$2,$2,%lo(eponMacDumpAllReg)
	jalr	$2
	nop

$L356:
	lui	$2,%hi(eponWanResetWithChannelRetire)
	addiu	$2,$2,%lo(eponWanResetWithChannelRetire)
	jalr	$2
	nop

$L351:
	lui	$2,%hi(g_EPON_MAC_BASE)
	lw	$2,%lo(g_EPON_MAC_BASE)($2)
	addiu	$2,$2,24884
	move	$4,$0
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	lw	$31,28($sp)
	addiu	$sp,$sp,32
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	eponTimeDrftIntHandler
	.size	eponTimeDrftIntHandler, .-eponTimeDrftIntHandler
	.rdata
	.align	2
$LC49:
	.ascii	"eponMpcpTmOutIntHandler llidIndex=%d\000"
	.section	.text.eponMpcpTmOutIntHandler,"ax",@progbits
	.align	2
	.globl	eponMpcpTmOutIntHandler
	.set	nomips16
	.set	nomicromips
	.ent	eponMpcpTmOutIntHandler
	.type	eponMpcpTmOutIntHandler, @function
eponMpcpTmOutIntHandler:
	.frame	$sp,56,$31		# vars= 32, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	addiu	$sp,$sp,-56
	sw	$31,52($sp)
	sw	$4,56($sp)
	sb	$0,16($sp)
	sb	$0,17($sp)
	lui	$2,%hi(g_EPON_MAC_BASE)
	lw	$2,%lo(g_EPON_MAC_BASE)($2)
	addiu	$2,$2,24588
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	sw	$2,40($sp)
	lbu	$2,42($sp)
	sb	$2,17($sp)
	sb	$0,16($sp)
	j	$L358
$L361:
	lbu	$3,17($sp)
	lbu	$2,16($sp)
	sra	$2,$3,$2
	andi	$2,$2,0x1
	beq	$2,$0,$L359
	lbu	$3,16($sp)
	li	$4,4			# 0x4
	lui	$2,%hi($LC49)
	addiu	$5,$2,%lo($LC49)
	move	$6,$3
	lui	$2,%hi(dbgtoMem)
	addiu	$2,$2,%lo(dbgtoMem)
	jalr	$2
	lbu	$2,16($sp)
	sll	$2,$2,3
	sll	$3,$2,3
	addu	$2,$2,$3
	addiu	$3,$2,16
	lui	$2,%hi(eponDrv)
	addiu	$2,$2,%lo(eponDrv)
	addu	$2,$3,$2
	addiu	$2,$2,4
	sw	$2,20($sp)
	lw	$2,20($sp)
	lbu	$2,1($2)
	bne	$2,$0,$L360
	j	$L359
$L360:
	lw	$2,20($sp)
	addiu	$2,$2,20
	sw	$2,24($sp)
	lw	$2,24($sp)
	lw	$3,4($2)
	li	$2,1			# 0x1
	bne	$3,$2,$L359
	lw	$2,24($sp)
	sb	$0,0($2)
	li	$2,1			# 0x1
	sw	$2,28($sp)
	lui	$2,%hi(mpcpTmOutCnt)
	addiu	$2,$2,%lo(mpcpTmOutCnt)
	sw	$2,32($sp)
	lw	$4,32($sp)
	lw	$3,28($sp)
	lw	$2,32($sp)
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
	sw	$2,36($sp)
$L359:
	lbu	$2,16($sp)
	addiu	$2,$2,1
	sb	$2,16($sp)
$L358:
	lbu	$2,16($sp)
	sltu	$2,$2,8
	bne	$2,$0,$L361
	lw	$2,40($sp)
	andi	$3,$2,0xff00
	lui	$2,%hi(g_EPON_MAC_BASE)
	lw	$2,%lo(g_EPON_MAC_BASE)($2)
	addiu	$2,$2,24588
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	lui	$2,%hi(mpcpTmOutCnt)
	addiu	$2,$2,%lo(mpcpTmOutCnt)
	lw	$2,0($2)
	slt	$2,$2,11
	beq	$2,$0,$L362
	lui	$2,%hi(eponWanResetWithChannelRetire)
	addiu	$2,$2,%lo(eponWanResetWithChannelRetire)
	jalr	$2
	j	$L357
$L362:
	lui	$2,%hi(mpcpTmOutCnt)
	addiu	$2,$2,%lo(mpcpTmOutCnt)
	lw	$2,0($2)
	andi	$2,$2,0x1
	bne	$2,$0,$L357
	lui	$2,%hi(eponWanResetWithChannelRetire)
	addiu	$2,$2,%lo(eponWanResetWithChannelRetire)
	jalr	$2
$L357:
	lw	$31,52($sp)
	addiu	$sp,$sp,56
	j	$31
	.end	eponMpcpTmOutIntHandler
	.size	eponMpcpTmOutIntHandler, .-eponMpcpTmOutIntHandler
	.rdata
	.align	2
$LC50:
	.ascii	"eponMpcpDscvFsmWaitHandler llidIndex=%d\000"
	.section	.text.eponMpcpDscvFsmWaitHandler,"ax",@progbits
	.align	2
	.globl	eponMpcpDscvFsmWaitHandler
	.set	nomips16
	.set	nomicromips
	.ent	eponMpcpDscvFsmWaitHandler
	.type	eponMpcpDscvFsmWaitHandler, @function
eponMpcpDscvFsmWaitHandler:
	.frame	$sp,40,$31		# vars= 16, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-40
	sw	$31,36($sp)
	move	$2,$4
	sb	$2,40($sp)
	lbu	$2,40($sp)
	sll	$2,$2,3
	sll	$3,$2,3
	addu	$2,$2,$3
	addiu	$3,$2,16
	lui	$2,%hi(eponDrv)
	addiu	$2,$2,%lo(eponDrv)
	addu	$2,$3,$2
	addiu	$2,$2,4
	sw	$2,20($sp)
	lw	$2,20($sp)
	addiu	$2,$2,20
	sw	$2,24($sp)
	sw	$0,28($sp)
	sw	$0,16($sp)
	lbu	$3,40($sp)
	li	$4,4			# 0x4
	lui	$2,%hi($LC50)
	addiu	$5,$2,%lo($LC50)
	move	$6,$3
	lui	$2,%hi(dbgtoMem)
	addiu	$2,$2,%lo(dbgtoMem)
	jalr	$2
	nop

	lw	$2,24($sp)
	lw	$3,4($2)
	li	$2,1			# 0x1
	bne	$3,$2,$L365
	nop

	lw	$2,24($sp)
	sw	$0,36($2)
	lw	$2,24($sp)
	lbu	$3,0($2)
	li	$2,5			# 0x5
	beq	$3,$2,$L366
	nop

	lw	$2,24($sp)
	li	$3,1			# 0x1
	sb	$3,0($2)
$L366:
	sw	$0,16($sp)
	j	$L367
	nop

$L368:
	lui	$2,%hi(g_EPON_MAC_BASE)
	lw	$2,%lo(g_EPON_MAC_BASE)($2)
	addiu	$3,$2,24620
	lbu	$2,40($sp)
	sll	$2,$2,2
	addu	$2,$3,$2
	lw	$2,0($2)
	sw	$2,28($sp)
	lw	$2,28($sp)
	ext	$2,$2,0,30
	sw	$2,28($sp)
	lui	$2,%hi(g_EPON_MAC_BASE)
	lw	$2,%lo(g_EPON_MAC_BASE)($2)
	addiu	$3,$2,24620
	lbu	$2,40($sp)
	sll	$2,$2,2
	addu	$2,$3,$2
	lw	$3,28($sp)
	sw	$3,0($2)
	lw	$2,16($sp)
	addiu	$2,$2,1
	sw	$2,16($sp)
$L367:
	lw	$2,16($sp)
	sltu	$2,$2,10
	bne	$2,$0,$L368
	nop

	lui	$2,%hi(g_EPON_MAC_BASE)
	lw	$2,%lo(g_EPON_MAC_BASE)($2)
	addiu	$3,$2,24620
	lbu	$2,40($sp)
	sll	$2,$2,2
	addu	$2,$3,$2
	lw	$2,0($2)
	sw	$2,28($sp)
	lw	$2,28($sp)
	ext	$2,$2,0,30
	sw	$2,28($sp)
	lw	$3,28($sp)
	li	$2,2147418112			# 0x7fff0000
	ori	$2,$2,0xffff
	or	$2,$3,$2
	sw	$2,28($sp)
	lui	$2,%hi(g_EPON_MAC_BASE)
	lw	$2,%lo(g_EPON_MAC_BASE)($2)
	addiu	$3,$2,24620
	lbu	$2,40($sp)
	sll	$2,$2,2
	addu	$2,$3,$2
	lw	$3,28($sp)
	sw	$3,0($2)
$L365:
	lui	$2,%hi(stop_omci_oam_monitor_not_online)
	addiu	$2,$2,%lo(stop_omci_oam_monitor_not_online)
	jalr	$2
	nop

	move	$2,$0
	lw	$31,36($sp)
	addiu	$sp,$sp,40
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	eponMpcpDscvFsmWaitHandler
	.size	eponMpcpDscvFsmWaitHandler, .-eponMpcpDscvFsmWaitHandler
	.rdata
	.align	2
$LC51:
	.ascii	"eponMpcpGntOvrRunIntHandler\000"
	.section	.text.eponMpcpGntOvrRunIntHandler,"ax",@progbits
	.align	2
	.globl	eponMpcpGntOvrRunIntHandler
	.set	nomips16
	.set	nomicromips
	.ent	eponMpcpGntOvrRunIntHandler
	.type	eponMpcpGntOvrRunIntHandler, @function
eponMpcpGntOvrRunIntHandler:
	.frame	$sp,24,$31		# vars= 0, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-24
	sw	$31,20($sp)
	li	$4,4			# 0x4
	lui	$2,%hi($LC51)
	addiu	$5,$2,%lo($LC51)
	lui	$2,%hi(dbgtoMem)
	addiu	$2,$2,%lo(dbgtoMem)
	jalr	$2
	nop

	move	$2,$0
	lw	$31,20($sp)
	addiu	$sp,$sp,24
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	eponMpcpGntOvrRunIntHandler
	.size	eponMpcpGntOvrRunIntHandler, .-eponMpcpGntOvrRunIntHandler
	.rdata
	.align	2
$LC52:
	.ascii	"eponMpcpRptOvrIntvalIntHandler\000"
	.align	2
$LC53:
	.ascii	"eponMpcpRptOvrIntvalIntHandler llidIndex=%d\000"
	.section	.text.eponMpcpRptOvrIntvalIntHandler,"ax",@progbits
	.align	2
	.globl	eponMpcpRptOvrIntvalIntHandler
	.set	nomips16
	.set	nomicromips
	.ent	eponMpcpRptOvrIntvalIntHandler
	.type	eponMpcpRptOvrIntvalIntHandler, @function
eponMpcpRptOvrIntvalIntHandler:
	.frame	$sp,32,$31		# vars= 8, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-32
	sw	$31,28($sp)
	sb	$0,16($sp)
	sb	$0,17($sp)
	li	$4,4			# 0x4
	lui	$2,%hi($LC52)
	addiu	$5,$2,%lo($LC52)
	lui	$2,%hi(dbgtoMem)
	addiu	$2,$2,%lo(dbgtoMem)
	jalr	$2
	nop

	lui	$2,%hi(g_EPON_MAC_BASE)
	lw	$2,%lo(g_EPON_MAC_BASE)($2)
	addiu	$2,$2,24588
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	sw	$2,20($sp)
	lbu	$2,23($sp)
	sb	$2,17($sp)
	sb	$0,16($sp)
	j	$L373
	nop

$L375:
	lbu	$3,17($sp)
	lbu	$2,16($sp)
	sra	$2,$3,$2
	andi	$2,$2,0x1
	beq	$2,$0,$L374
	nop

	lbu	$3,16($sp)
	li	$4,4			# 0x4
	lui	$2,%hi($LC53)
	addiu	$5,$2,%lo($LC53)
	move	$6,$3
	lui	$2,%hi(dbgtoMem)
	addiu	$2,$2,%lo(dbgtoMem)
	jalr	$2
	nop

$L374:
	lbu	$2,16($sp)
	addiu	$2,$2,1
	sb	$2,16($sp)
$L373:
	lbu	$2,16($sp)
	sltu	$2,$2,8
	bne	$2,$0,$L375
	nop

	lw	$2,20($sp)
	andi	$3,$2,0xff
	lui	$2,%hi(g_EPON_MAC_BASE)
	lw	$2,%lo(g_EPON_MAC_BASE)($2)
	addiu	$2,$2,24588
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	move	$2,$0
	lw	$31,28($sp)
	addiu	$sp,$sp,32
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	eponMpcpRptOvrIntvalIntHandler
	.size	eponMpcpRptOvrIntvalIntHandler, .-eponMpcpRptOvrIntvalIntHandler
	.rdata
	.align	2
	.type	__func__.41647, @object
	.size	__func__.41647, 28
__func__.41647:
	.ascii	"eponMpcpDiscvGateIntHandler\000"
	.align	2
	.type	__func__.41718, @object
	.size	__func__.41718, 23
__func__.41718:
	.ascii	"eponMpcpRgstIntHandler\000"
	.ident	"GCC: (Buildroot 2015.08.1) 4.9.3"
