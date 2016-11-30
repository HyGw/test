	.file	1 "epon_main.c"
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
	.globl	g_EPON_MAC_BASE
	.section	.bss,"aw",@nobits
	.align	2
	.type	g_EPON_MAC_BASE, @object
	.size	g_EPON_MAC_BASE, 4
g_EPON_MAC_BASE:
	.space	4
	.globl	isSfu
	.data
	.type	isSfu, @object
	.size	isSfu, 1
isSfu:
	.byte	1
	.globl	epon_reg_lock
	.section	.bss
	.align	2
	.type	epon_reg_lock, @object
	.size	epon_reg_lock, 4
epon_reg_lock:
	.space	4
	.globl	eponReadyFlag
	.align	2
	.type	eponReadyFlag, @object
	.size	eponReadyFlag, 4
eponReadyFlag:
	.space	4
	.local	gEponTypeBTimer
	.comm	gEponTypeBTimer,28,4
	.globl	eponMacRestart_flag
	.align	2
	.type	eponMacRestart_flag, @object
	.size	eponMacRestart_flag, 4
eponMacRestart_flag:
	.space	4
	.globl	eponMacMpcpDscvGateTask
	.align	2
	.type	eponMacMpcpDscvGateTask, @object
	.size	eponMacMpcpDscvGateTask, 20
eponMacMpcpDscvGateTask:
	.space	20
	.globl	eponStartTask
	.align	2
	.type	eponStartTask, @object
	.size	eponStartTask, 20
eponStartTask:
	.space	20
	.rdata
	.align	2
$LC0:
	.ascii	"deregister\000"
	.align	2
$LC1:
	.ascii	"<deregister>\000"
	.align	2
$LC2:
	.ascii	"tmdrftflag\000"
	.align	2
$LC3:
	.ascii	"<tmdrftflag>\000"
	.align	2
$LC4:
	.ascii	"setprobe\000"
	.align	2
$LC5:
	.ascii	"<probe>\000"
	.align	2
$LC6:
	.ascii	"wanreset\000"
	.align	2
$LC7:
	.ascii	"<wanreset>\000"
	.align	2
$LC8:
	.ascii	"macreset\000"
	.align	2
$LC9:
	.ascii	"<macreset>\000"
	.align	2
$LC10:
	.ascii	"rgstack\000"
	.align	2
$LC11:
	.ascii	"<llid,ack>\000"
	.align	2
$LC12:
	.ascii	"ignoreIntMask\000"
	.align	2
$LC13:
	.ascii	"<mask(32bit)>\000"
	.align	2
$LC14:
	.ascii	"setDefLlidMask\000"
	.align	2
$LC15:
	.ascii	"<mask(16bit)>\000"
	.align	2
	.type	eponMpcpCmds, @object
	.size	eponMpcpCmds, 180
eponMpcpCmds:
	.word	$LC0
	.word	doEponDeregister
	.word	2
	.word	1
	.word	$LC1
	.word	$LC2
	.word	doEponSetTmdrftflag
	.word	2
	.word	1
	.word	$LC3
	.word	$LC4
	.word	doEponSetProbe
	.word	2
	.word	1
	.word	$LC5
	.word	$LC6
	.word	doEponResetWan
	.word	2
	.word	0
	.word	$LC7
	.word	$LC8
	.word	doEponResetMac
	.word	2
	.word	0
	.word	$LC9
	.word	$LC10
	.word	doEponSetRgstAckType
	.word	2
	.word	0
	.word	$LC11
	.word	$LC12
	.word	doEponSetIgnoreIntMask
	.word	2
	.word	0
	.word	$LC13
	.word	$LC14
	.word	doEponSetDefLlidMask
	.word	2
	.word	1
	.word	$LC15
	.word	0
	.word	0
	.word	16
	.word	0
	.word	0
	.align	2
$LC16:
	.ascii	"llidThrshldNum\000"
	.align	2
$LC17:
	.ascii	"<llidIndex(0~7)> <llidThrshldNum(1~3)>\000"
	.align	2
$LC18:
	.ascii	"llidQueThrshld\000"
	.align	2
$LC19:
	.ascii	"<llidIndex(0~7)> <thrshldIndex(0~2)> <q0 value> ... <q7 "
	.ascii	"value>\000"
	.align	2
	.type	eponSetCmds, @object
	.size	eponSetCmds, 60
eponSetCmds:
	.word	$LC16
	.word	doEponSetLlidThrshldNum
	.word	2
	.word	1
	.word	$LC17
	.word	$LC18
	.word	doEponSetLlidQueThrshld
	.word	2
	.word	1
	.word	$LC19
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.align	2
$LC20:
	.ascii	"regtest\000"
	.align	2
$LC21:
	.ascii	"<times>\000"
	.align	2
$LC22:
	.ascii	"regDefCheck\000"
	.align	2
$LC23:
	.ascii	"<regDefCheck>\000"
	.align	2
	.type	eponHwtestCmds, @object
	.size	eponHwtestCmds, 60
eponHwtestCmds:
	.word	$LC20
	.word	doEponRegtest
	.word	2
	.word	1
	.word	$LC21
	.word	$LC22
	.word	doEponRegDefCheck
	.word	2
	.word	0
	.word	$LC23
	.word	0
	.word	0
	.word	16
	.word	0
	.word	0
	.align	2
$LC24:
	.ascii	"debuglevel\000"
	.align	2
$LC25:
	.ascii	"<debuglevel>\000"
	.align	2
$LC26:
	.ascii	"dumpallreg\000"
	.align	2
$LC27:
	.ascii	"<dumpallreg>\000"
	.align	2
$LC28:
	.ascii	"mpcp\000"
	.align	2
$LC29:
	.ascii	"set\000"
	.align	2
$LC30:
	.ascii	"hwtest\000"
	.align	2
	.type	eponCmds, @object
	.size	eponCmds, 120
eponCmds:
	.word	$LC24
	.word	doEponDebuglevel
	.word	2
	.word	0
	.word	$LC25
	.word	$LC26
	.word	doEponDumpAllReg
	.word	2
	.word	0
	.word	$LC27
	.word	$LC28
	.word	doEponMpcp
	.word	18
	.word	0
	.word	0
	.word	$LC29
	.word	doEponSet
	.word	18
	.word	0
	.word	0
	.word	$LC30
	.word	doEponHwtest
	.word	18
	.word	0
	.word	0
	.word	0
	.word	0
	.word	16
	.word	0
	.word	0
	.globl	eponMacRegTable
	.section	.bss
	.align	2
	.type	eponMacRegTable, @object
	.size	eponMacRegTable, 4
eponMacRegTable:
	.space	4
	.globl	eponDebugLevel
	.data
	.align	2
	.type	eponDebugLevel, @object
	.size	eponDebugLevel, 4
eponDebugLevel:
	.word	1
	.globl	eponDrv
	.section	.bss
	.align	2
	.type	eponDrv, @object
	.size	eponDrv, 600
eponDrv:
	.space	600
	.section	.text.eponMacGetRegTblSize,"ax",@progbits
	.align	2
	.globl	eponMacGetRegTblSize
	.set	nomips16
	.set	nomicromips
	.ent	eponMacGetRegTblSize
	.type	eponMacGetRegTblSize, @function
eponMacGetRegTblSize:
	.frame	$sp,8,$31		# vars= 8, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-8
	sw	$0,0($sp)
	lui	$2,%hi(eponMacRegTable)
	lw	$2,%lo(eponMacRegTable)($2)
	bne	$2,$0,$L2
	nop

	move	$2,$0
	j	$L3
	nop

$L2:
	j	$L4
	nop

$L5:
	lw	$2,0($sp)
	addiu	$2,$2,1
	sw	$2,0($sp)
$L4:
	lui	$2,%hi(eponMacRegTable)
	lw	$3,%lo(eponMacRegTable)($2)
	lw	$2,0($sp)
	sll	$2,$2,2
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	lw	$2,0($2)
	bne	$2,$0,$L5
	nop

	lw	$2,0($sp)
$L3:
	addiu	$sp,$sp,8
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	eponMacGetRegTblSize
	.size	eponMacGetRegTblSize, .-eponMacGetRegTblSize
	.section	.text.doEpon,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	doEpon
	.type	doEpon, @function
doEpon:
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
	lui	$2,%hi(eponCmds)
	addiu	$4,$2,%lo(eponCmds)
	lw	$5,24($sp)
	lw	$6,28($sp)
	lw	$7,32($sp)
	lui	$2,%hi(subcmd)
	addiu	$2,$2,%lo(subcmd)
	jalr	$2
	nop

	lw	$31,20($sp)
	addiu	$sp,$sp,24
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	doEpon
	.size	doEpon, .-doEpon
	.section	.text.doEponMpcp,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	doEponMpcp
	.type	doEponMpcp, @function
doEponMpcp:
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
	lui	$2,%hi(eponMpcpCmds)
	addiu	$4,$2,%lo(eponMpcpCmds)
	lw	$5,24($sp)
	lw	$6,28($sp)
	lw	$7,32($sp)
	lui	$2,%hi(subcmd)
	addiu	$2,$2,%lo(subcmd)
	jalr	$2
	nop

	lw	$31,20($sp)
	addiu	$sp,$sp,24
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	doEponMpcp
	.size	doEponMpcp, .-doEponMpcp
	.section	.text.doEponSet,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	doEponSet
	.type	doEponSet, @function
doEponSet:
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
	lui	$2,%hi(eponSetCmds)
	addiu	$4,$2,%lo(eponSetCmds)
	lw	$5,24($sp)
	lw	$6,28($sp)
	lw	$7,32($sp)
	lui	$2,%hi(subcmd)
	addiu	$2,$2,%lo(subcmd)
	jalr	$2
	nop

	lw	$31,20($sp)
	addiu	$sp,$sp,24
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	doEponSet
	.size	doEponSet, .-doEponSet
	.rdata
	.align	2
$LC31:
	.ascii	"\015\012debugLevel = %u\012\000"
	.section	.text.doEponDebuglevel,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	doEponDebuglevel
	.type	doEponDebuglevel, @function
doEponDebuglevel:
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
	lw	$2,24($sp)
	slt	$2,$2,2
	beq	$2,$0,$L13
	nop

	lui	$2,%hi(eponDebugLevel)
	lw	$3,%lo(eponDebugLevel)($2)
	lui	$2,%hi($LC31)
	addiu	$4,$2,%lo($LC31)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	move	$2,$0
	j	$L14
	nop

$L13:
	lw	$2,28($sp)
	addiu	$2,$2,4
	lw	$2,0($2)
	move	$4,$2
	move	$5,$0
	li	$6,16			# 0x10
	lui	$2,%hi(simple_strtoul)
	addiu	$2,$2,%lo(simple_strtoul)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(eponDebugLevel)
	sw	$3,%lo(eponDebugLevel)($2)
	move	$2,$0
$L14:
	lw	$31,20($sp)
	addiu	$sp,$sp,24
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	doEponDebuglevel
	.size	doEponDebuglevel, .-doEponDebuglevel
	.section	.text.doEponDumpAllReg,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	doEponDumpAllReg
	.type	doEponDumpAllReg, @function
doEponDumpAllReg:
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
	lui	$2,%hi(eponMacDumpAllReg)
	addiu	$2,$2,%lo(eponMacDumpAllReg)
	jalr	$2
	nop

	move	$2,$0
	lw	$31,20($sp)
	addiu	$sp,$sp,24
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	doEponDumpAllReg
	.size	doEponDumpAllReg, .-doEponDumpAllReg
	.rdata
	.align	2
$LC32:
	.ascii	"\015\012deregister <LLID Index | 0-7>\012\000"
	.section	.text.doEponDeregister,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	doEponDeregister
	.type	doEponDeregister, @function
doEponDeregister:
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
	lw	$2,32($sp)
	slt	$2,$2,2
	beq	$2,$0,$L18
	nop

	lui	$2,%hi($LC32)
	addiu	$4,$2,%lo($LC32)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	move	$2,$0
	j	$L19
	nop

$L18:
	lw	$2,36($sp)
	addiu	$2,$2,4
	lw	$2,0($2)
	move	$4,$2
	move	$5,$0
	li	$6,16			# 0x10
	lui	$2,%hi(simple_strtoul)
	addiu	$2,$2,%lo(simple_strtoul)
	jalr	$2
	nop

	sw	$2,16($sp)
	lw	$2,16($sp)
	andi	$2,$2,0x00ff
	move	$4,$2
	lui	$2,%hi(eponMpcpLocalDergstr)
	addiu	$2,$2,%lo(eponMpcpLocalDergstr)
	jalr	$2
	nop

	move	$2,$0
$L19:
	lw	$31,28($sp)
	addiu	$sp,$sp,32
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	doEponDeregister
	.size	doEponDeregister, .-doEponDeregister
	.rdata
	.align	2
$LC33:
	.ascii	"\015\012tmdrftflag <0 | 1>\012\000"
	.section	.text.doEponSetTmdrftflag,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	doEponSetTmdrftflag
	.type	doEponSetTmdrftflag, @function
doEponSetTmdrftflag:
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
	lw	$2,24($sp)
	slt	$2,$2,2
	beq	$2,$0,$L21
	nop

	lui	$2,%hi($LC33)
	addiu	$4,$2,%lo($LC33)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	move	$2,$0
	j	$L22
	nop

$L21:
	lw	$2,28($sp)
	addiu	$2,$2,4
	lw	$2,0($2)
	move	$4,$2
	move	$5,$0
	li	$6,16			# 0x10
	lui	$2,%hi(simple_strtoul)
	addiu	$2,$2,%lo(simple_strtoul)
	jalr	$2
	nop

	move	$3,$2
	li	$2,1			# 0x1
	bne	$3,$2,$L23
	nop

	lui	$2,%hi(eponTimeDrftHandleFlag)
	li	$3,1			# 0x1
	sb	$3,%lo(eponTimeDrftHandleFlag)($2)
	j	$L24
	nop

$L23:
	lui	$2,%hi(eponTimeDrftHandleFlag)
	sb	$0,%lo(eponTimeDrftHandleFlag)($2)
$L24:
	move	$2,$0
$L22:
	lw	$31,20($sp)
	addiu	$sp,$sp,24
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	doEponSetTmdrftflag
	.size	doEponSetTmdrftflag, .-doEponSetTmdrftflag
	.globl	eponSetProbe
	.section	.bss
	.align	2
	.type	eponSetProbe, @object
	.size	eponSetProbe, 4
eponSetProbe:
	.space	4
	.globl	eponDefLlidMask
	.data
	.align	2
	.type	eponDefLlidMask, @object
	.size	eponDefLlidMask, 4
eponDefLlidMask:
	.word	1
	.globl	ignoreIntMask
	.align	2
	.type	ignoreIntMask, @object
	.size	ignoreIntMask, 4
ignoreIntMask:
	.word	-1
	.rdata
	.align	2
$LC34:
	.ascii	"\015\012 set epon mac probe after reset\012\000"
	.align	2
$LC35:
	.ascii	"\015\012 current defalt probe :0x%x\012\000"
	.align	2
$LC36:
	.ascii	"set probe bfb66100 :0x%x\000"
	.section	.text.doEponSetProbe,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	doEponSetProbe
	.type	doEponSetProbe, @function
doEponSetProbe:
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
	lw	$2,24($sp)
	slt	$2,$2,2
	beq	$2,$0,$L26
	nop

	lui	$2,%hi($LC34)
	addiu	$4,$2,%lo($LC34)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi(eponSetProbe)
	lw	$3,%lo(eponSetProbe)($2)
	lui	$2,%hi($LC35)
	addiu	$4,$2,%lo($LC35)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	move	$2,$0
	j	$L27
	nop

$L26:
	lw	$2,28($sp)
	addiu	$2,$2,4
	lw	$2,0($2)
	move	$4,$2
	move	$5,$0
	li	$6,16			# 0x10
	lui	$2,%hi(simple_strtoul)
	addiu	$2,$2,%lo(simple_strtoul)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(eponSetProbe)
	sw	$3,%lo(eponSetProbe)($2)
	lui	$2,%hi(eponSetProbe)
	lw	$3,%lo(eponSetProbe)($2)
	lui	$2,%hi($LC36)
	addiu	$4,$2,%lo($LC36)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	move	$2,$0
$L27:
	lw	$31,20($sp)
	addiu	$sp,$sp,24
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	doEponSetProbe
	.size	doEponSetProbe, .-doEponSetProbe
	.rdata
	.align	2
$LC37:
	.ascii	"\015\012 set default llid mask after reset\012\000"
	.align	2
$LC38:
	.ascii	"set default llid mask :0x%x\000"
	.section	.text.doEponSetDefLlidMask,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	doEponSetDefLlidMask
	.type	doEponSetDefLlidMask, @function
doEponSetDefLlidMask:
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
	lw	$2,24($sp)
	slt	$2,$2,2
	beq	$2,$0,$L29
	nop

	lui	$2,%hi($LC37)
	addiu	$4,$2,%lo($LC37)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi(eponDefLlidMask)
	lw	$3,%lo(eponDefLlidMask)($2)
	lui	$2,%hi($LC35)
	addiu	$4,$2,%lo($LC35)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	move	$2,$0
	j	$L30
	nop

$L29:
	lw	$2,28($sp)
	addiu	$2,$2,4
	lw	$2,0($2)
	move	$4,$2
	move	$5,$0
	li	$6,16			# 0x10
	lui	$2,%hi(simple_strtoul)
	addiu	$2,$2,%lo(simple_strtoul)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(eponDefLlidMask)
	sw	$3,%lo(eponDefLlidMask)($2)
	lui	$2,%hi(eponDefLlidMask)
	lw	$3,%lo(eponDefLlidMask)($2)
	lui	$2,%hi($LC38)
	addiu	$4,$2,%lo($LC38)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	move	$2,$0
$L30:
	lw	$31,20($sp)
	addiu	$sp,$sp,24
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	doEponSetDefLlidMask
	.size	doEponSetDefLlidMask, .-doEponSetDefLlidMask
	.rdata
	.align	2
$LC39:
	.ascii	"epon mac reset\000"
	.section	.text.doEponResetMac,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	doEponResetMac
	.type	doEponResetMac, @function
doEponResetMac:
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
	lui	$2,%hi(eponMacRestart)
	addiu	$2,$2,%lo(eponMacRestart)
	jalr	$2
	nop

	li	$4,4			# 0x4
	lui	$2,%hi($LC39)
	addiu	$5,$2,%lo($LC39)
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
	.end	doEponResetMac
	.size	doEponResetMac, .-doEponResetMac
	.rdata
	.align	2
$LC40:
	.ascii	"epon wan reset\000"
	.section	.text.doEponResetWan,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	doEponResetWan
	.type	doEponResetWan, @function
doEponResetWan:
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
	lui	$2,%hi(eponWanResetWithChannelRetire)
	addiu	$2,$2,%lo(eponWanResetWithChannelRetire)
	jalr	$2
	nop

	li	$4,4			# 0x4
	lui	$2,%hi($LC40)
	addiu	$5,$2,%lo($LC40)
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
	.end	doEponResetWan
	.size	doEponResetWan, .-doEponResetWan
	.rdata
	.align	2
$LC41:
	.ascii	"\015\012 rgstack <llid:0~7> <flag:0(nack)/1(ack)> \012\000"
	.align	2
$LC42:
	.ascii	" set status(ack/nack) of Register_ack. \012\000"
	.align	2
$LC43:
	.ascii	"\015\012 rgstack llid=%x,ack=%x\012\000"
	.section	.text.doEponSetRgstAckType,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	doEponSetRgstAckType
	.type	doEponSetRgstAckType, @function
doEponSetRgstAckType:
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
	sb	$0,17($sp)
	li	$2,1			# 0x1
	sb	$2,16($sp)
	lw	$2,32($sp)
	slt	$2,$2,3
	beq	$2,$0,$L36
	nop

	lui	$2,%hi($LC41)
	addiu	$4,$2,%lo($LC41)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi($LC42)
	addiu	$4,$2,%lo($LC42)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	move	$2,$0
	j	$L37
	nop

$L36:
	lw	$2,36($sp)
	addiu	$2,$2,4
	lw	$2,0($2)
	move	$4,$2
	move	$5,$0
	li	$6,16			# 0x10
	lui	$2,%hi(simple_strtoul)
	addiu	$2,$2,%lo(simple_strtoul)
	jalr	$2
	nop

	sb	$2,17($sp)
	lw	$2,36($sp)
	addiu	$2,$2,8
	lw	$2,0($2)
	move	$4,$2
	move	$5,$0
	li	$6,16			# 0x10
	lui	$2,%hi(simple_strtoul)
	addiu	$2,$2,%lo(simple_strtoul)
	jalr	$2
	nop

	sb	$2,16($sp)
	lbu	$2,16($sp)
	beq	$2,$0,$L38
	nop

	li	$2,1			# 0x1
	sb	$2,16($sp)
$L38:
	lbu	$3,17($sp)
	lbu	$2,16($sp)
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(eponMpcpSetDiscvRgstAck)
	addiu	$2,$2,%lo(eponMpcpSetDiscvRgstAck)
	jalr	$2
	nop

	lbu	$5,17($sp)
	lbu	$3,16($sp)
	lui	$2,%hi($LC43)
	addiu	$4,$2,%lo($LC43)
	move	$6,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	move	$2,$0
$L37:
	lw	$31,28($sp)
	addiu	$sp,$sp,32
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	doEponSetRgstAckType
	.size	doEponSetRgstAckType, .-doEponSetRgstAckType
	.rdata
	.align	2
$LC44:
	.ascii	"\015\012 ignoreIntMask <mask>,current mask %x \012\000"
	.align	2
$LC45:
	.ascii	"\015\012 ignoreIntMask =%x\012\000"
	.section	.text.doEponSetIgnoreIntMask,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	doEponSetIgnoreIntMask
	.type	doEponSetIgnoreIntMask, @function
doEponSetIgnoreIntMask:
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
	lw	$2,24($sp)
	slt	$2,$2,2
	beq	$2,$0,$L40
	nop

	lui	$2,%hi(ignoreIntMask)
	lw	$3,%lo(ignoreIntMask)($2)
	lui	$2,%hi($LC44)
	addiu	$4,$2,%lo($LC44)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	move	$2,$0
	j	$L41
	nop

$L40:
	lw	$2,28($sp)
	addiu	$2,$2,4
	lw	$2,0($2)
	move	$4,$2
	move	$5,$0
	li	$6,16			# 0x10
	lui	$2,%hi(simple_strtoul)
	addiu	$2,$2,%lo(simple_strtoul)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(ignoreIntMask)
	sw	$3,%lo(ignoreIntMask)($2)
	lui	$2,%hi(ignoreIntMask)
	lw	$3,%lo(ignoreIntMask)($2)
	lui	$2,%hi($LC45)
	addiu	$4,$2,%lo($LC45)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	move	$2,$0
$L41:
	lw	$31,20($sp)
	addiu	$sp,$sp,24
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	doEponSetIgnoreIntMask
	.size	doEponSetIgnoreIntMask, .-doEponSetIgnoreIntMask
	.rdata
	.align	2
$LC46:
	.ascii	"Usage: llidThrshldNum <llidIndex(0~7)> <llidThrshldNum(1"
	.ascii	"~3)>\012\000"
	.section	.text.doEponSetLlidThrshldNum,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	doEponSetLlidThrshldNum
	.type	doEponSetLlidThrshldNum, @function
doEponSetLlidThrshldNum:
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
	sb	$0,16($sp)
	sb	$0,17($sp)
	lw	$2,32($sp)
	slt	$2,$2,3
	beq	$2,$0,$L43
	nop

	lui	$2,%hi($LC46)
	addiu	$4,$2,%lo($LC46)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	li	$2,-1			# 0xffffffffffffffff
	j	$L44
	nop

$L43:
	lw	$2,36($sp)
	addiu	$2,$2,4
	lw	$2,0($2)
	move	$4,$2
	move	$5,$0
	li	$6,10			# 0xa
	lui	$2,%hi(simple_strtoul)
	addiu	$2,$2,%lo(simple_strtoul)
	jalr	$2
	nop

	sb	$2,16($sp)
	lw	$2,36($sp)
	addiu	$2,$2,8
	lw	$2,0($2)
	move	$4,$2
	move	$5,$0
	li	$6,10			# 0xa
	lui	$2,%hi(simple_strtoul)
	addiu	$2,$2,%lo(simple_strtoul)
	jalr	$2
	nop

	sb	$2,17($sp)
	lbu	$3,16($sp)
	lbu	$2,17($sp)
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(eponSetLlidThrshldNum)
	addiu	$2,$2,%lo(eponSetLlidThrshldNum)
	jalr	$2
	nop

	move	$2,$0
$L44:
	lw	$31,28($sp)
	addiu	$sp,$sp,32
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	doEponSetLlidThrshldNum
	.size	doEponSetLlidThrshldNum, .-doEponSetLlidThrshldNum
	.rdata
	.align	2
$LC47:
	.ascii	"Usage: llidQueThrshld <llidIndex(0~7)> <thrshldIndex(0~2"
	.ascii	")> <q0 value> ... <q7 value>\012\000"
	.align	2
$LC48:
	.ascii	"doEponSetLlidQueThrshld error\012\000"
	.section	.text.doEponSetLlidQueThrshld,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	doEponSetLlidQueThrshld
	.type	doEponSetLlidQueThrshld, @function
doEponSetLlidQueThrshld:
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
	sb	$0,16($sp)
	sb	$0,18($sp)
	sb	$0,19($sp)
	sw	$0,24($sp)
	sw	$0,28($sp)
	sw	$0,32($sp)
	sw	$0,36($sp)
	sb	$0,17($sp)
	sw	$0,20($sp)
	lw	$2,56($sp)
	slt	$2,$2,11
	beq	$2,$0,$L46
	nop

	lui	$2,%hi($LC47)
	addiu	$4,$2,%lo($LC47)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	li	$2,-1			# 0xffffffffffffffff
	j	$L53
	nop

$L46:
	lw	$2,60($sp)
	addiu	$2,$2,4
	lw	$2,0($2)
	move	$4,$2
	move	$5,$0
	li	$6,10			# 0xa
	lui	$2,%hi(simple_strtoul)
	addiu	$2,$2,%lo(simple_strtoul)
	jalr	$2
	nop

	sb	$2,18($sp)
	lw	$2,60($sp)
	addiu	$2,$2,8
	lw	$2,0($2)
	move	$4,$2
	move	$5,$0
	li	$6,10			# 0xa
	lui	$2,%hi(simple_strtoul)
	addiu	$2,$2,%lo(simple_strtoul)
	jalr	$2
	nop

	sb	$2,19($sp)
	sb	$0,17($sp)
	j	$L48
	nop

$L49:
	lbu	$16,17($sp)
	lbu	$2,17($sp)
	addiu	$2,$2,3
	sll	$2,$2,2
	lw	$3,60($sp)
	addu	$2,$3,$2
	lw	$2,0($2)
	move	$4,$2
	move	$5,$0
	li	$6,10			# 0xa
	lui	$2,%hi(simple_strtoul)
	addiu	$2,$2,%lo(simple_strtoul)
	jalr	$2
	nop

	andi	$3,$2,0xffff
	sll	$2,$16,1
	addiu	$4,$sp,16
	addu	$2,$4,$2
	sh	$3,8($2)
	lbu	$2,17($sp)
	addiu	$2,$2,1
	sb	$2,17($sp)
$L48:
	lbu	$2,17($sp)
	sltu	$2,$2,8
	bne	$2,$0,$L49
	nop

	sb	$0,16($sp)
	j	$L50
	nop

$L52:
	lbu	$2,16($sp)
	sll	$2,$2,1
	addiu	$3,$sp,16
	addu	$2,$3,$2
	lhu	$2,8($2)
	beq	$2,$0,$L51
	nop

	lbu	$2,18($sp)
	sb	$2,40($sp)
	lbu	$2,16($sp)
	sb	$2,41($sp)
	lbu	$2,19($sp)
	sb	$2,42($sp)
	lbu	$2,16($sp)
	sll	$2,$2,1
	addiu	$3,$sp,16
	addu	$2,$3,$2
	lhu	$2,8($2)
	sh	$2,44($sp)
	addiu	$2,$sp,40
	move	$4,$2
	lui	$2,%hi(epon_set_queue_threshold)
	addiu	$2,$2,%lo(epon_set_queue_threshold)
	jalr	$2
	nop

	sw	$2,20($sp)
	lw	$2,20($sp)
	bgez	$2,$L51
	nop

	lui	$2,%hi($LC48)
	addiu	$4,$2,%lo($LC48)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	li	$2,-1			# 0xffffffffffffffff
	j	$L53
	nop

$L51:
	lbu	$2,16($sp)
	addiu	$2,$2,1
	sb	$2,16($sp)
$L50:
	lbu	$2,16($sp)
	sltu	$2,$2,8
	bne	$2,$0,$L52
	nop

	move	$2,$0
$L53:
	lw	$31,52($sp)
	lw	$16,48($sp)
	addiu	$sp,$sp,56
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	doEponSetLlidQueThrshld
	.size	doEponSetLlidQueThrshld, .-doEponSetLlidQueThrshld
	.section	.text.eponTod1ppsIntHandler,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	eponTod1ppsIntHandler
	.type	eponTod1ppsIntHandler, @function
eponTod1ppsIntHandler:
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
	.end	eponTod1ppsIntHandler
	.size	eponTod1ppsIntHandler, .-eponTod1ppsIntHandler
	.section	.text.eponTodUpdtIntHandler,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	eponTodUpdtIntHandler
	.type	eponTodUpdtIntHandler, @function
eponTodUpdtIntHandler:
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
	.end	eponTodUpdtIntHandler
	.size	eponTodUpdtIntHandler, .-eponTodUpdtIntHandler
	.section	.text.eponPtpMsgTxIntHandler,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	eponPtpMsgTxIntHandler
	.type	eponPtpMsgTxIntHandler, @function
eponPtpMsgTxIntHandler:
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
	.end	eponPtpMsgTxIntHandler
	.size	eponPtpMsgTxIntHandler, .-eponPtpMsgTxIntHandler
	.globl	eponStaticRptEnable
	.section	.bss
	.type	eponStaticRptEnable, @object
	.size	eponStaticRptEnable, 1
eponStaticRptEnable:
	.space	1
	.rdata
	.align	2
$LC49:
	.ascii	"enter eponIsr\000"
	.align	2
$LC50:
	.ascii	"eponIsr interrupt status is 0\000"
	.align	2
$LC51:
	.ascii	"eponIsr interrupt status 0x%x --> 0x%x, after clear\000"
	.align	2
$LC52:
	.ascii	"EponIsr TIMEDRFT_INT isrValue=%X\000"
	.align	2
$LC53:
	.ascii	"EponIsr MPCP_TIMEOUT_INT\000"
	.align	2
$LC54:
	.ascii	"EponIsr REGISTER_REQ_DONE_INT\000"
	.align	2
$LC55:
	.ascii	"EponIsr REGISTER_ACK_DONE_INT\000"
	.align	2
$LC56:
	.ascii	"EponIsr LLID%d_RCV_RGST_INT\000"
	.align	2
$LC57:
	.ascii	"ERR: LLID%d_RCV_RGST_INT eponMpcpRgstIntHandler ret=%d\000"
	.align	2
$LC58:
	.ascii	"EponIsr Receive DiscvGate Message\000"
	.align	2
$LC59:
	.ascii	"EponIsr TOD_1PPS_INT\000"
	.align	2
$LC60:
	.ascii	"ERR: TOD_1PPS_INT eponTod1ppsIntHandler ret=%d\000"
	.align	2
$LC61:
	.ascii	"EponIsr TOD_UPDT_INT\000"
	.align	2
$LC62:
	.ascii	"ERR: TOD_UPDT_INT eponTodUpdtIntHandler ret=%d\000"
	.align	2
$LC63:
	.ascii	"EponIsr PTP_MSG_TX_INT\000"
	.align	2
$LC64:
	.ascii	"ERR: PTP_MSG_TX_INT eponPtpMsgTxIntHandler ret=%d\000"
	.align	2
$LC65:
	.ascii	"EponIsr GNT_BUF_OVRRUN_INT\000"
	.align	2
$LC66:
	.ascii	"ERR: GNT_BUF_OVRRUN_INT eponMpcpGntOvrRunHandler ret=%d\000"
	.align	2
$LC67:
	.ascii	"EponIsr RPT_OVERINTVL_INT\000"
	.align	2
$LC68:
	.ascii	"ERR: RPT_OVERINTVL_INT eponMpcpRptOvrIntvalIntHandler re"
	.ascii	"t=%d\000"
	.section	.text.eponIsr,"ax",@progbits
	.align	2
	.globl	eponIsr
	.set	nomips16
	.set	nomicromips
	.ent	eponIsr
	.type	eponIsr, @function
eponIsr:
	.frame	$sp,72,$31		# vars= 48, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	addiu	$sp,$sp,-72
	sw	$31,68($sp)
	sw	$0,24($sp)
	sw	$0,20($sp)
	li	$4,4			# 0x4
	lui	$2,%hi($LC49)
	addiu	$5,$2,%lo($LC49)
	lui	$2,%hi(dbgtoMem)
	addiu	$2,$2,%lo(dbgtoMem)
	jalr	$2
	lui	$2,%hi(g_EPON_MAC_BASE)
	lw	$2,%lo(g_EPON_MAC_BASE)($2)
	addiu	$2,$2,24580
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	sw	$2,16($sp)
	lui	$2,%hi(g_EPON_MAC_BASE)
	lw	$2,%lo(g_EPON_MAC_BASE)($2)
	addiu	$2,$2,24580
	li	$4,-1			# 0xffffffffffffffff
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	lui	$2,%hi(eponDrv)
	addiu	$2,$2,%lo(eponDrv)
	lbu	$2,4($2)
	beq	$2,$0,$L61
	lui	$2,%hi(eponDrv)
	addiu	$2,$2,%lo(eponDrv)
	lbu	$3,8($2)
	li	$2,1			# 0x1
	bne	$3,$2,$L61
	j	$L60
$L61:
	lw	$2,16($sp)
	bne	$2,$0,$L63
	li	$4,1			# 0x1
	lui	$2,%hi($LC50)
	addiu	$5,$2,%lo($LC50)
	lui	$2,%hi(dbgtoMem)
	addiu	$2,$2,%lo(dbgtoMem)
	jalr	$2
	j	$L60
$L63:
	lui	$2,%hi(g_EPON_MAC_BASE)
	lw	$2,%lo(g_EPON_MAC_BASE)($2)
	addiu	$2,$2,24580
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	sw	$2,28($sp)
	lw	$2,28($sp)
	beq	$2,$0,$L64
	li	$4,1			# 0x1
	lui	$2,%hi($LC51)
	addiu	$5,$2,%lo($LC51)
	lw	$6,16($sp)
	lw	$7,28($sp)
	lui	$2,%hi(dbgtoMem)
	addiu	$2,$2,%lo(dbgtoMem)
	jalr	$2
$L64:
	lui	$2,%hi(ignoreIntMask)
	lw	$2,%lo(ignoreIntMask)($2)
	lw	$3,16($sp)
	and	$2,$3,$2
	sw	$2,16($sp)
	lw	$2,16($sp)
	andi	$2,$2,0x2000
	beq	$2,$0,$L65
	li	$4,3			# 0x3
	lui	$2,%hi($LC52)
	addiu	$5,$2,%lo($LC52)
	lw	$6,16($sp)
	lui	$2,%hi(dbgtoMem)
	addiu	$2,$2,%lo(dbgtoMem)
	jalr	$2
	lw	$2,16($sp)
	ori	$2,$2,0x2000
	sw	$2,16($sp)
	move	$4,$0
	lui	$2,%hi(eponTimeDrftIntHandler)
	addiu	$2,$2,%lo(eponTimeDrftIntHandler)
	jalr	$2
$L65:
	lw	$2,16($sp)
	andi	$2,$2,0x4000
	beq	$2,$0,$L66
	li	$4,3			# 0x3
	lui	$2,%hi($LC53)
	addiu	$5,$2,%lo($LC53)
	lui	$2,%hi(dbgtoMem)
	addiu	$2,$2,%lo(dbgtoMem)
	jalr	$2
	lw	$2,16($sp)
	ori	$2,$2,0x4000
	sw	$2,16($sp)
	move	$4,$0
	lui	$2,%hi(eponMpcpTmOutIntHandler)
	addiu	$2,$2,%lo(eponMpcpTmOutIntHandler)
	jalr	$2
	j	$L67
$L66:
	lui	$2,%hi(mpcpTmOutCnt)
	sw	$0,%lo(mpcpTmOutCnt)($2)
$L67:
	lui	$2,%hi(gpEponPriv)
	lw	$2,%lo(gpEponPriv)($2)
	lbu	$2,40($2)
	beq	$2,$0,$L68
	lw	$3,16($sp)
	li	$2,16777216			# 0x1000000
	and	$2,$3,$2
	beq	$2,$0,$L69
	li	$4,3			# 0x3
	lui	$2,%hi($LC54)
	addiu	$5,$2,%lo($LC54)
	lui	$2,%hi(dbgtoMem)
	addiu	$2,$2,%lo(dbgtoMem)
	jalr	$2
	lui	$2,%hi(eponMpcpRgstReqIntHandler)
	addiu	$2,$2,%lo(eponMpcpRgstReqIntHandler)
	jalr	$2
$L69:
	lw	$3,16($sp)
	li	$2,33554432			# 0x2000000
	and	$2,$3,$2
	beq	$2,$0,$L68
	li	$4,3			# 0x3
	lui	$2,%hi($LC55)
	addiu	$5,$2,%lo($LC55)
	lui	$2,%hi(dbgtoMem)
	addiu	$2,$2,%lo(dbgtoMem)
	jalr	$2
	lui	$2,%hi(eponMpcpRgstAckIntHandler)
	addiu	$2,$2,%lo(eponMpcpRgstAckIntHandler)
	jalr	$2
$L68:
	sw	$0,20($sp)
	j	$L70
$L72:
	li	$3,2			# 0x2
	lw	$2,20($sp)
	sll	$2,$3,$2
	move	$3,$2
	lw	$2,16($sp)
	and	$2,$3,$2
	beq	$2,$0,$L71
	li	$4,3			# 0x3
	lui	$2,%hi($LC56)
	addiu	$5,$2,%lo($LC56)
	lw	$6,20($sp)
	lui	$2,%hi(dbgtoMem)
	addiu	$2,$2,%lo(dbgtoMem)
	jalr	$2
	lw	$2,20($sp)
	andi	$2,$2,0x00ff
	move	$4,$2
	lui	$2,%hi(eponMpcpRgstIntHandler)
	addiu	$2,$2,%lo(eponMpcpRgstIntHandler)
	jalr	$2
	sw	$2,24($sp)
	lw	$2,24($sp)
	bgez	$2,$L71
	li	$4,1			# 0x1
	lui	$2,%hi($LC57)
	addiu	$5,$2,%lo($LC57)
	lw	$6,20($sp)
	lw	$7,24($sp)
	lui	$2,%hi(dbgtoMem)
	addiu	$2,$2,%lo(dbgtoMem)
	jalr	$2
$L71:
	lw	$2,20($sp)
	addiu	$2,$2,1
	sw	$2,20($sp)
$L70:
	lw	$2,20($sp)
	slt	$2,$2,8
	bne	$2,$0,$L72
	lw	$2,16($sp)
	andi	$2,$2,0x1
	beq	$2,$0,$L73
	li	$4,4			# 0x4
	lui	$2,%hi($LC58)
	addiu	$5,$2,%lo($LC58)
	lui	$2,%hi(dbgtoMem)
	addiu	$2,$2,%lo(dbgtoMem)
	jalr	$2
	lw	$2,16($sp)
	ori	$2,$2,0x1
	sw	$2,16($sp)
	lui	$2,%hi(eponMacMpcpDscvGateTask)
	addiu	$2,$2,%lo(eponMacMpcpDscvGateTask)
	sw	$2,32($sp)
	lw	$2,32($sp)
	addiu	$2,$2,4
	sw	$0,36($sp)
	sw	$2,40($sp)
	lw	$2,36($sp)
	andi	$2,$2,0xffff
	andi	$2,$2,0x1f
	sh	$2,44($sp)
#APP
 # 261 "/opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/bitops.h" 1
			

 # 0 "" 2
#NO_APP
	lw	$2,36($sp)
	srl	$2,$2,5
	sll	$2,$2,2
	lw	$3,40($sp)
	addu	$2,$3,$2
	sw	$2,48($sp)
	lhu	$2,44($sp)
	li	$3,1			# 0x1
	sll	$2,$3,$2
	lw	$5,48($sp)
	lw	$3,48($sp)
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
	sw	$4,52($sp)
	sw	$3,56($sp)
#APP
 # 312 "/opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/bitops.h" 1
			

 # 0 "" 2
#NO_APP
	lw	$2,56($sp)
	sltu	$2,$0,$2
	andi	$2,$2,0x00ff
	bne	$2,$0,$L73
	lw	$4,32($sp)
	lui	$2,%hi(__tasklet_schedule)
	addiu	$2,$2,%lo(__tasklet_schedule)
	jalr	$2
$L73:
	lw	$2,16($sp)
	andi	$2,$2,0x1000
	beq	$2,$0,$L75
	li	$4,3			# 0x3
	lui	$2,%hi($LC59)
	addiu	$5,$2,%lo($LC59)
	lui	$2,%hi(dbgtoMem)
	addiu	$2,$2,%lo(dbgtoMem)
	jalr	$2
	lw	$2,16($sp)
	ori	$2,$2,0x1000
	sw	$2,16($sp)
	lui	$2,%hi(eponTod1ppsIntHandler)
	addiu	$2,$2,%lo(eponTod1ppsIntHandler)
	jalr	$2
	sw	$2,24($sp)
	lw	$2,24($sp)
	bgez	$2,$L75
	li	$4,1			# 0x1
	lui	$2,%hi($LC60)
	addiu	$5,$2,%lo($LC60)
	lw	$6,24($sp)
	lui	$2,%hi(dbgtoMem)
	addiu	$2,$2,%lo(dbgtoMem)
	jalr	$2
$L75:
	lw	$2,16($sp)
	andi	$2,$2,0x800
	beq	$2,$0,$L76
	li	$4,3			# 0x3
	lui	$2,%hi($LC61)
	addiu	$5,$2,%lo($LC61)
	lui	$2,%hi(dbgtoMem)
	addiu	$2,$2,%lo(dbgtoMem)
	jalr	$2
	lw	$2,16($sp)
	ori	$2,$2,0x800
	sw	$2,16($sp)
	lui	$2,%hi(eponTodUpdtIntHandler)
	addiu	$2,$2,%lo(eponTodUpdtIntHandler)
	jalr	$2
	sw	$2,24($sp)
	lw	$2,24($sp)
	bgez	$2,$L76
	li	$4,1			# 0x1
	lui	$2,%hi($LC62)
	addiu	$5,$2,%lo($LC62)
	lw	$6,24($sp)
	lui	$2,%hi(dbgtoMem)
	addiu	$2,$2,%lo(dbgtoMem)
	jalr	$2
$L76:
	lw	$2,16($sp)
	andi	$2,$2,0x400
	beq	$2,$0,$L77
	li	$4,3			# 0x3
	lui	$2,%hi($LC63)
	addiu	$5,$2,%lo($LC63)
	lui	$2,%hi(dbgtoMem)
	addiu	$2,$2,%lo(dbgtoMem)
	jalr	$2
	lw	$2,16($sp)
	ori	$2,$2,0x400
	sw	$2,16($sp)
	lui	$2,%hi(eponPtpMsgTxIntHandler)
	addiu	$2,$2,%lo(eponPtpMsgTxIntHandler)
	jalr	$2
	sw	$2,24($sp)
	lw	$2,24($sp)
	bgez	$2,$L77
	li	$4,1			# 0x1
	lui	$2,%hi($LC64)
	addiu	$5,$2,%lo($LC64)
	lw	$6,24($sp)
	lui	$2,%hi(dbgtoMem)
	addiu	$2,$2,%lo(dbgtoMem)
	jalr	$2
$L77:
	lw	$2,16($sp)
	andi	$2,$2,0x200
	beq	$2,$0,$L78
	li	$4,3			# 0x3
	lui	$2,%hi($LC65)
	addiu	$5,$2,%lo($LC65)
	lui	$2,%hi(dbgtoMem)
	addiu	$2,$2,%lo(dbgtoMem)
	jalr	$2
	lw	$2,16($sp)
	ori	$2,$2,0x200
	sw	$2,16($sp)
	lui	$2,%hi(eponMpcpGntOvrRunIntHandler)
	addiu	$2,$2,%lo(eponMpcpGntOvrRunIntHandler)
	jalr	$2
	sw	$2,24($sp)
	lw	$2,24($sp)
	bgez	$2,$L78
	li	$4,1			# 0x1
	lui	$2,%hi($LC66)
	addiu	$5,$2,%lo($LC66)
	lw	$6,24($sp)
	lui	$2,%hi(dbgtoMem)
	addiu	$2,$2,%lo(dbgtoMem)
	jalr	$2
$L78:
	lw	$2,16($sp)
	andi	$2,$2,0x8000
	beq	$2,$0,$L79
	li	$4,3			# 0x3
	lui	$2,%hi($LC67)
	addiu	$5,$2,%lo($LC67)
	lui	$2,%hi(dbgtoMem)
	addiu	$2,$2,%lo(dbgtoMem)
	jalr	$2
	lw	$2,16($sp)
	ori	$2,$2,0x8000
	sw	$2,16($sp)
	lui	$2,%hi(eponMpcpRptOvrIntvalIntHandler)
	addiu	$2,$2,%lo(eponMpcpRptOvrIntvalIntHandler)
	jalr	$2
	sw	$2,24($sp)
	lw	$2,24($sp)
	bgez	$2,$L79
	li	$4,1			# 0x1
	lui	$2,%hi($LC68)
	addiu	$5,$2,%lo($LC68)
	lw	$6,24($sp)
	lui	$2,%hi(dbgtoMem)
	addiu	$2,$2,%lo(dbgtoMem)
	jalr	$2
$L79:
	.set	noreorder
	nop
	.set	reorder
$L60:
	lw	$31,68($sp)
	addiu	$sp,$sp,72
	j	$31
	.end	eponIsr
	.size	eponIsr, .-eponIsr
	.globl	dbgPrintLock
	.section	.bss
	.align	2
	.type	dbgPrintLock, @object
	.size	dbgPrintLock, 4
dbgPrintLock:
	.space	4
	.globl	dbgPrintFlags
	.align	2
	.type	dbgPrintFlags, @object
	.size	dbgPrintFlags, 4
dbgPrintFlags:
	.space	4
	.globl	screenprint
	.align	3
	.type	screenprint, @object
	.size	screenprint, 136
screenprint:
	.space	136
	.globl	print_mem
	.align	3
	.type	print_mem, @object
	.size	print_mem, 34816
print_mem:
	.space	34816
	.globl	test
	.align	2
	.type	test, @object
	.size	test, 128
test:
	.space	128
	.local	pWrite
	.comm	pWrite,4,4
	.local	pRead
	.comm	pRead,4,4
	.local	temp
	.comm	temp,128,4
	.globl	dbgPrint
	.align	2
	.type	dbgPrint, @object
	.size	dbgPrint, 28
dbgPrint:
	.space	28
	.section	.text.dbgPrintQuit,"ax",@progbits
	.align	2
	.globl	dbgPrintQuit
	.set	nomips16
	.set	nomicromips
	.ent	dbgPrintQuit
	.type	dbgPrintQuit, @function
dbgPrintQuit:
	.frame	$sp,24,$31		# vars= 0, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-24
	sw	$31,20($sp)
	lui	$2,%hi(dbgPrint)
	addiu	$4,$2,%lo(dbgPrint)
	lui	$2,%hi(del_timer)
	addiu	$2,$2,%lo(del_timer)
	jalr	$2
	nop

	lw	$31,20($sp)
	addiu	$sp,$sp,24
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	dbgPrintQuit
	.size	dbgPrintQuit, .-dbgPrintQuit
	.section	.text.dbgPrintInit,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	dbgPrintInit
	.type	dbgPrintInit, @function
dbgPrintInit:
	.frame	$sp,24,$31		# vars= 0, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-24
	sw	$31,20($sp)
	lui	$2,%hi(dbgPrint)
	addiu	$4,$2,%lo(dbgPrint)
	move	$5,$0
	move	$6,$0
	lui	$2,%hi(init_timer_key)
	addiu	$2,$2,%lo(init_timer_key)
	jalr	$2
	nop

	lui	$2,%hi(dbgPrint)
	addiu	$2,$2,%lo(dbgPrint)
	lui	$3,%hi(dbgPrintTimer)
	addiu	$3,$3,%lo(dbgPrintTimer)
	sw	$3,16($2)
	lui	$2,%hi(jiffies)
	lw	$2,%lo(jiffies)($2)
	addiu	$3,$2,50
	lui	$2,%hi(dbgPrint)
	addiu	$2,$2,%lo(dbgPrint)
	sw	$3,8($2)
	lui	$2,%hi(dbgPrint)
	addiu	$4,$2,%lo(dbgPrint)
	lui	$2,%hi(add_timer)
	addiu	$2,$2,%lo(add_timer)
	jalr	$2
	nop

	lui	$2,%hi(pWrite)
	lui	$3,%hi(print_mem)
	addiu	$3,$3,%lo(print_mem)
	sw	$3,%lo(pWrite)($2)
	lui	$2,%hi(pRead)
	lui	$3,%hi(print_mem)
	addiu	$3,$3,%lo(print_mem)
	sw	$3,%lo(pRead)($2)
	lw	$31,20($sp)
	addiu	$sp,$sp,24
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	dbgPrintInit
	.size	dbgPrintInit, .-dbgPrintInit
	.rdata
	.align	2
$LC69:
	.ascii	"Memory out.\012\000"
	.section	.text.stringWrite,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	stringWrite
	.type	stringWrite, @function
stringWrite:
	.frame	$sp,48,$31		# vars= 24, regs= 2/0, args= 16, gp= 0
	.mask	0x80010000,-4
	.fmask	0x00000000,0
	addiu	$sp,$sp,-48
	sw	$31,44($sp)
	sw	$16,40($sp)
	lui	$2,%hi(pWrite)
	lw	$3,%lo(pWrite)($2)
	lui	$2,%hi(print_mem+34816)
	addiu	$2,$2,%lo(print_mem+34816)
	bne	$3,$2,$L83
	lui	$2,%hi(pWrite)
	lui	$3,%hi(print_mem)
	addiu	$3,$3,%lo(print_mem)
	sw	$3,%lo(pWrite)($2)
$L83:
	lui	$2,%hi(pWrite)
	lw	$2,%lo(pWrite)($2)
	sw	$2,16($sp)
	lui	$2,%hi(test)
	addiu	$2,$2,%lo(test)
	sw	$2,20($sp)
	lw	$3,16($sp)
	lw	$2,20($sp)
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
	sw	$3,16($sp)
	sw	$2,20($sp)
	sw	$4,24($sp)
	lw	$2,24($sp)
	beq	$2,$0,$L85
	lui	$2,%hi($LC69)
	addiu	$4,$2,%lo($LC69)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	j	$L82
$L85:
	lui	$2,%hi(pWrite)
	lw	$16,%lo(pWrite)($2)
	addiu	$2,$sp,32
	move	$4,$2
	lui	$2,%hi(ktime_get)
	addiu	$2,$2,%lo(ktime_get)
	jalr	$2
	lw	$3,36($sp)
	lw	$2,32($sp)
	sw	$3,4($16)
	sw	$2,0($16)
	lui	$2,%hi(pWrite)
	lw	$2,%lo(pWrite)($2)
	addiu	$2,$2,8
	move	$4,$2
	lui	$2,%hi(temp)
	addiu	$5,$2,%lo(temp)
	li	$6,128			# 0x80
	lui	$2,%hi(memcpy)
	addiu	$2,$2,%lo(memcpy)
	jalr	$2
	lui	$2,%hi(pWrite)
	lw	$2,%lo(pWrite)($2)
	addiu	$3,$2,136
	lui	$2,%hi(pWrite)
	sw	$3,%lo(pWrite)($2)
$L82:
	lw	$31,44($sp)
	lw	$16,40($sp)
	addiu	$sp,$sp,48
	j	$31
	.end	stringWrite
	.size	stringWrite, .-stringWrite
	.rdata
	.align	2
$LC70:
	.ascii	"[%03dmin : %03ds : %03dms: %03dus : %03dns] \000"
	.align	2
$LC71:
	.ascii	"%s\012\000"
	.section	.text.stringRead,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	stringRead
	.type	stringRead, @function
stringRead:
	.frame	$sp,72,$31		# vars= 40, regs= 1/0, args= 24, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	addiu	$sp,$sp,-72
	sw	$31,68($sp)
	sw	$0,24($sp)
	j	$L88
$L93:
	lui	$2,%hi(pRead)
	lw	$3,%lo(pRead)($2)
	lui	$2,%hi(print_mem+34816)
	addiu	$2,$2,%lo(print_mem+34816)
	bne	$3,$2,$L89
	lui	$2,%hi(pRead)
	lui	$3,%hi(print_mem)
	addiu	$3,$3,%lo(print_mem)
	sw	$3,%lo(pRead)($2)
$L89:
	lui	$2,%hi(pRead)
	lw	$2,%lo(pRead)($2)
	addiu	$2,$2,8
	sw	$2,28($sp)
	lui	$2,%hi(test)
	addiu	$2,$2,%lo(test)
	sw	$2,32($sp)
	lw	$3,28($sp)
	lw	$2,32($sp)
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
	sw	$3,28($sp)
	sw	$2,32($sp)
	sw	$4,36($sp)
	lw	$2,36($sp)
	bne	$2,$0,$L91
	j	$L87
$L91:
	lui	$2,%hi(pRead)
	lw	$2,%lo(pRead)($2)
	sw	$2,40($sp)
	lw	$2,40($sp)
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
	sw	$3,44($sp)
	lw	$2,40($sp)
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
	sw	$3,48($sp)
	lw	$2,40($sp)
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
	sw	$3,52($sp)
	lw	$2,40($sp)
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
	sw	$3,56($sp)
	lw	$2,40($sp)
	lw	$2,0($2)
	li	$3,-2004353024			# 0xffffffff88880000
	ori	$3,$3,0x8889
	mult	$2,$3
	mfhi	$3
	addu	$3,$3,$2
	sra	$3,$3,5
	sra	$2,$2,31
	subu	$2,$3,$2
	sw	$2,60($sp)
	lw	$2,48($sp)
	sw	$2,16($sp)
	lw	$2,44($sp)
	sw	$2,20($sp)
	lui	$2,%hi($LC70)
	addiu	$4,$2,%lo($LC70)
	lw	$5,60($sp)
	lw	$6,56($sp)
	lw	$7,52($sp)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	lui	$2,%hi(pRead)
	lw	$2,%lo(pRead)($2)
	addiu	$3,$2,8
	lui	$2,%hi($LC71)
	addiu	$4,$2,%lo($LC71)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
#APP
 # 1138 "/opt/tclinux_phoenix/modules/private/xpon/src/epon/epon_main.c" 1
	.set	push
	.set	noreorder
	.set	mips2
	sync
	.set	pop
 # 0 "" 2
#NO_APP
	lui	$2,%hi(pRead)
	lw	$2,%lo(pRead)($2)
	move	$4,$2
	move	$5,$0
	li	$6,136			# 0x88
	lui	$2,%hi(memset)
	addiu	$2,$2,%lo(memset)
	jalr	$2
	lui	$2,%hi(pRead)
	lw	$2,%lo(pRead)($2)
	addiu	$3,$2,136
	lui	$2,%hi(pRead)
	sw	$3,%lo(pRead)($2)
	lw	$2,24($sp)
	addiu	$2,$2,1
	sw	$2,24($sp)
$L88:
	lw	$2,24($sp)
	slt	$2,$2,60
	bne	$2,$0,$L93
$L87:
	lw	$31,68($sp)
	addiu	$sp,$sp,72
	j	$31
	.end	stringRead
	.size	stringRead, .-stringRead
	.section	.text.dbgPrintTimer,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	dbgPrintTimer
	.type	dbgPrintTimer, @function
dbgPrintTimer:
	.frame	$sp,24,$31		# vars= 0, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-24
	sw	$31,20($sp)
	lui	$2,%hi(stringRead)
	addiu	$2,$2,%lo(stringRead)
	jalr	$2
	nop

	lui	$2,%hi(jiffies)
	lw	$2,%lo(jiffies)($2)
	addiu	$3,$2,50
	lui	$2,%hi(dbgPrint)
	addiu	$4,$2,%lo(dbgPrint)
	move	$5,$3
	lui	$2,%hi(mod_timer)
	addiu	$2,$2,%lo(mod_timer)
	jalr	$2
	nop

	lw	$31,20($sp)
	addiu	$sp,$sp,24
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	dbgPrintTimer
	.size	dbgPrintTimer, .-dbgPrintTimer
	.section	.text.dbgtoMem,"ax",@progbits
	.align	2
	.globl	dbgtoMem
	.set	nomips16
	.set	nomicromips
	.ent	dbgtoMem
	.type	dbgtoMem, @function
dbgtoMem:
	.frame	$sp,56,$31		# vars= 32, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-56
	sw	$31,52($sp)
	sw	$4,56($sp)
	sw	$6,64($sp)
	sw	$7,68($sp)
	sw	$5,60($sp)
	lui	$2,%hi(eponDebugLevel)
	lw	$3,%lo(eponDebugLevel)($2)
	lw	$2,56($sp)
	sltu	$2,$3,$2
	bne	$2,$0,$L95
	nop

	lui	$2,%hi(dbgPrintLock)
	addiu	$2,$2,%lo(dbgPrintLock)
	sw	$2,16($sp)
	lw	$2,16($sp)
	move	$4,$2
	lui	$2,%hi(_raw_spin_lock_irqsave)
	addiu	$2,$2,%lo(_raw_spin_lock_irqsave)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(dbgPrintFlags)
	sw	$3,%lo(dbgPrintFlags)($2)
	lui	$2,%hi(temp)
	addiu	$4,$2,%lo(temp)
	move	$5,$0
	li	$6,128			# 0x80
	lui	$2,%hi(memset)
	addiu	$2,$2,%lo(memset)
	jalr	$2
	nop

	addiu	$2,$sp,64
	sw	$2,28($sp)
	lw	$3,28($sp)
	lui	$2,%hi(temp)
	addiu	$4,$2,%lo(temp)
	li	$5,128			# 0x80
	lw	$6,60($sp)
	move	$7,$3
	lui	$2,%hi(vsnprintf)
	addiu	$2,$2,%lo(vsnprintf)
	jalr	$2
	nop

	lui	$2,%hi(stringWrite)
	addiu	$2,$2,%lo(stringWrite)
	jalr	$2
	nop

	lui	$2,%hi(dbgPrintFlags)
	lw	$2,%lo(dbgPrintFlags)($2)
	lui	$3,%hi(dbgPrintLock)
	addiu	$3,$3,%lo(dbgPrintLock)
	sw	$3,20($sp)
	sw	$2,24($sp)
	lw	$2,20($sp)
	move	$4,$2
	lw	$5,24($sp)
	lui	$2,%hi(_raw_spin_unlock_irqrestore)
	addiu	$2,$2,%lo(_raw_spin_unlock_irqrestore)
	jalr	$2
	nop

$L95:
	lw	$31,52($sp)
	addiu	$sp,$sp,56
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	dbgtoMem
	.size	dbgtoMem, .-dbgtoMem
	.section	__ksymtab_strings,"a",@progbits
	.type	__kstrtab_dbgtoMem, @object
	.size	__kstrtab_dbgtoMem, 9
__kstrtab_dbgtoMem:
	.ascii	"dbgtoMem\000"
	.section	__ksymtab,"a",@progbits
	.align	2
	.type	__ksymtab_dbgtoMem, @object
	.size	__ksymtab_dbgtoMem, 8
__ksymtab_dbgtoMem:
	.word	dbgtoMem
	.word	__kstrtab_dbgtoMem
	.rdata
	.align	2
$LC72:
	.ascii	"ERR: eponIrqInit request_irq err=%d\000"
	.section	.text.eponIrqRegister,"ax",@progbits
	.align	2
	.globl	eponIrqRegister
	.set	nomips16
	.set	nomicromips
	.ent	eponIrqRegister
	.type	eponIrqRegister, @function
eponIrqRegister:
	.frame	$sp,72,$31		# vars= 48, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-72
	sw	$31,68($sp)
	li	$2,16			# 0x10
	sw	$2,20($sp)
	li	$2,4			# 0x4
	sw	$2,24($sp)
	lui	$2,%hi(eponIsr)
	addiu	$2,$2,%lo(eponIsr)
	sw	$2,28($sp)
	sw	$0,32($sp)
	li	$2,5			# 0x5
	sw	$2,36($sp)
	lw	$2,24($sp)
	sw	$2,44($sp)
	lw	$2,28($sp)
	sw	$2,48($sp)
	addiu	$2,$sp,36
	lw	$4,20($sp)
	move	$5,$0
	move	$6,$2
	lui	$2,%hi(__ECNT_HOOK)
	addiu	$2,$2,%lo(__ECNT_HOOK)
	jalr	$2
	nop

	sw	$2,32($sp)
	lw	$3,32($sp)
	li	$2,-1			# 0xffffffffffffffff
	beq	$3,$2,$L100
	nop

	lw	$2,40($sp)
	j	$L102
	nop

$L100:
	li	$2,-1			# 0xffffffffffffffff
$L102:
	sw	$2,16($sp)
	lw	$2,16($sp)
	beq	$2,$0,$L103
	nop

	li	$4,1			# 0x1
	lui	$2,%hi($LC72)
	addiu	$5,$2,%lo($LC72)
	lw	$6,16($sp)
	lui	$2,%hi(dbgtoMem)
	addiu	$2,$2,%lo(dbgtoMem)
	jalr	$2
	nop

	lw	$2,16($sp)
	j	$L104
	nop

$L103:
	move	$2,$0
$L104:
	lw	$31,68($sp)
	addiu	$sp,$sp,72
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	eponIrqRegister
	.size	eponIrqRegister, .-eponIrqRegister
	.rdata
	.align	2
$LC73:
	.ascii	"eponIrqDinit QDMA_CALLBACK_EPON_MAC_HANDLER\000"
	.section	.text.eponIrqUnregister,"ax",@progbits
	.align	2
	.globl	eponIrqUnregister
	.set	nomips16
	.set	nomicromips
	.ent	eponIrqUnregister
	.type	eponIrqUnregister, @function
eponIrqUnregister:
	.frame	$sp,72,$31		# vars= 48, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-72
	sw	$31,68($sp)
	li	$2,16			# 0x10
	sw	$2,16($sp)
	li	$2,4			# 0x4
	sw	$2,20($sp)
	sw	$0,24($sp)
	li	$2,6			# 0x6
	sw	$2,28($sp)
	lw	$2,20($sp)
	sw	$2,36($sp)
	addiu	$2,$sp,28
	lw	$4,16($sp)
	move	$5,$0
	move	$6,$2
	lui	$2,%hi(__ECNT_HOOK)
	addiu	$2,$2,%lo(__ECNT_HOOK)
	jalr	$2
	nop

	sw	$2,24($sp)
	li	$4,4			# 0x4
	lui	$2,%hi($LC73)
	addiu	$5,$2,%lo($LC73)
	lui	$2,%hi(dbgtoMem)
	addiu	$2,$2,%lo(dbgtoMem)
	jalr	$2
	nop

	lw	$31,68($sp)
	addiu	$sp,$sp,72
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	eponIrqUnregister
	.size	eponIrqUnregister, .-eponIrqUnregister
	.rdata
	.align	2
$LC74:
	.ascii	"<6>The MAC address in flash is null!\012\000"
	.section	.text.getOnuMacAddr,"ax",@progbits
	.align	2
	.globl	getOnuMacAddr
	.set	nomips16
	.set	nomicromips
	.ent	getOnuMacAddr
	.type	getOnuMacAddr, @function
getOnuMacAddr:
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
	j	$L110
	nop

$L115:
	li	$2,-1079967744			# 0xffffffffbfa10000
	ori	$2,$2,0x114
	lw	$2,0($2)
	andi	$2,$2,0x2
	beq	$2,$0,$L111
	nop

	lui	$2,%hi(ranand_read_byte)
	lw	$2,%lo(ranand_read_byte)($2)
	beq	$2,$0,$L112
	nop

	lui	$2,%hi(ranand_read_byte)
	lw	$2,%lo(ranand_read_byte)($2)
	lui	$3,%hi(flash_base)
	lw	$4,%lo(flash_base)($3)
	lw	$3,16($sp)
	addu	$4,$4,$3
	li	$3,65352			# 0xff48
	addu	$3,$4,$3
	move	$17,$3
	move	$16,$0
	move	$5,$17
	move	$4,$16
	jalr	$2
	nop

	seb	$2,$2
	j	$L114
	nop

$L112:
	li	$2,-1			# 0xffffffffffffffff
	j	$L114
	nop

$L111:
	lui	$2,%hi(flash_base)
	lw	$3,%lo(flash_base)($2)
	lw	$2,16($sp)
	addu	$3,$3,$2
	li	$2,65352			# 0xff48
	addu	$2,$3,$2
	move	$4,$2
	lui	$2,%hi(ReadSPIByte)
	addiu	$2,$2,%lo(ReadSPIByte)
	jalr	$2
	nop

	seb	$2,$2
$L114:
	lw	$3,16($sp)
	addiu	$4,$sp,16
	addu	$3,$4,$3
	sb	$2,4($3)
	lw	$2,16($sp)
	addiu	$2,$2,1
	sw	$2,16($sp)
$L110:
	lw	$2,16($sp)
	slt	$2,$2,6
	bne	$2,$0,$L115
	nop

	lb	$2,20($sp)
	bne	$2,$0,$L116
	nop

	lb	$2,21($sp)
	bne	$2,$0,$L116
	nop

	lb	$2,22($sp)
	bne	$2,$0,$L116
	nop

	lb	$2,23($sp)
	bne	$2,$0,$L116
	nop

	lb	$2,24($sp)
	bne	$2,$0,$L116
	nop

	lb	$2,25($sp)
	bne	$2,$0,$L116
	nop

	lui	$2,%hi($LC74)
	addiu	$4,$2,%lo($LC74)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	j	$L117
	nop

$L116:
	addiu	$2,$sp,20
	lw	$4,48($sp)
	move	$5,$2
	li	$6,6			# 0x6
	lui	$2,%hi(memcpy)
	addiu	$2,$2,%lo(memcpy)
	jalr	$2
	nop

$L117:
	move	$2,$0
	lw	$31,44($sp)
	lw	$17,40($sp)
	lw	$16,36($sp)
	addiu	$sp,$sp,48
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	getOnuMacAddr
	.size	getOnuMacAddr, .-getOnuMacAddr
	.rdata
	.align	2
$LC75:
	.ascii	"ERR: eponMacSetMacAddr timers == 100\000"
	.section	.text.eponMacCheckMacCfg,"ax",@progbits
	.align	2
	.globl	eponMacCheckMacCfg
	.set	nomips16
	.set	nomicromips
	.ent	eponMacCheckMacCfg
	.type	eponMacCheckMacCfg, @function
eponMacCheckMacCfg:
	.frame	$sp,32,$31		# vars= 8, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-32
	sw	$31,28($sp)
	sw	$0,16($sp)
	j	$L120
	nop

$L123:
	lui	$2,%hi(g_EPON_MAC_BASE)
	lw	$2,%lo(g_EPON_MAC_BASE)($2)
	addiu	$2,$2,24656
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	sw	$2,20($sp)
	lw	$3,20($sp)
	li	$2,65536			# 0x10000
	and	$2,$3,$2
	bne	$2,$0,$L121
	nop

	j	$L122
	nop

$L121:
	lw	$2,16($sp)
	addiu	$2,$2,1
	sw	$2,16($sp)
$L120:
	lw	$2,16($sp)
	sltu	$2,$2,100
	bne	$2,$0,$L123
	nop

$L122:
	lw	$3,16($sp)
	li	$2,100			# 0x64
	bne	$3,$2,$L124
	nop

	li	$4,1			# 0x1
	lui	$2,%hi($LC75)
	addiu	$5,$2,%lo($LC75)
	lui	$2,%hi(dbgtoMem)
	addiu	$2,$2,%lo(dbgtoMem)
	jalr	$2
	nop

	li	$2,-1			# 0xffffffffffffffff
	j	$L126
	nop

$L124:
	move	$2,$0
$L126:
	lw	$31,28($sp)
	addiu	$sp,$sp,32
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	eponMacCheckMacCfg
	.size	eponMacCheckMacCfg, .-eponMacCheckMacCfg
	.rdata
	.align	2
$LC76:
	.ascii	"enter eponMacSetMacAddr\000"
	.align	2
$LC77:
	.ascii	"ERR: eponMacSetMacAddr llidEntry_p == NULL\000"
	.align	2
$LC78:
	.ascii	"ERR: eponMacSetMacAddr set 1 timers == 100\000"
	.align	2
$LC79:
	.ascii	"ERR: eponMacSetMacAddr set 2 timers == 100\000"
	.section	.text.eponMacSetMacAddr,"ax",@progbits
	.align	2
	.globl	eponMacSetMacAddr
	.set	nomips16
	.set	nomicromips
	.ent	eponMacSetMacAddr
	.type	eponMacSetMacAddr, @function
eponMacSetMacAddr:
	.frame	$sp,32,$31		# vars= 8, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-32
	sw	$31,28($sp)
	sw	$4,32($sp)
	li	$4,4			# 0x4
	lui	$2,%hi($LC76)
	addiu	$5,$2,%lo($LC76)
	lui	$2,%hi(dbgtoMem)
	addiu	$2,$2,%lo(dbgtoMem)
	jalr	$2
	nop

	lw	$2,32($sp)
	bne	$2,$0,$L128
	nop

	li	$4,1			# 0x1
	lui	$2,%hi($LC77)
	addiu	$5,$2,%lo($LC77)
	lui	$2,%hi(dbgtoMem)
	addiu	$2,$2,%lo(dbgtoMem)
	jalr	$2
	nop

	li	$2,-1			# 0xffffffffffffffff
	j	$L133
	nop

$L128:
	lui	$2,%hi(eponMacCheckMacCfg)
	addiu	$2,$2,%lo(eponMacCheckMacCfg)
	jalr	$2
	nop

	bgez	$2,$L130
	nop

	li	$4,1			# 0x1
	lui	$2,%hi($LC75)
	addiu	$5,$2,%lo($LC75)
	lui	$2,%hi(dbgtoMem)
	addiu	$2,$2,%lo(dbgtoMem)
	jalr	$2
	nop

	li	$2,-1			# 0xffffffffffffffff
	j	$L133
	nop

$L130:
	sw	$0,16($sp)
	lw	$2,32($sp)
	addiu	$2,$2,6
	move	$4,$2
	lui	$2,%hi(get32)
	addiu	$2,$2,%lo(get32)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(g_EPON_MAC_BASE)
	lw	$2,%lo(g_EPON_MAC_BASE)($2)
	addiu	$2,$2,24660
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	lw	$2,16($sp)
	ins	$2,$0,0,1
	sw	$2,16($sp)
	lw	$2,32($sp)
	lbu	$2,0($2)
	andi	$2,$2,0x7
	andi	$3,$2,0x00ff
	lw	$2,16($sp)
	ins	$2,$3,1,3
	sw	$2,16($sp)
	lw	$2,16($sp)
	li	$3,1			# 0x1
	ins	$2,$3,31,1
	sw	$2,16($sp)
	lw	$3,16($sp)
	lui	$2,%hi(g_EPON_MAC_BASE)
	lw	$2,%lo(g_EPON_MAC_BASE)($2)
	addiu	$2,$2,24656
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	lui	$2,%hi(eponMacCheckMacCfg)
	addiu	$2,$2,%lo(eponMacCheckMacCfg)
	jalr	$2
	nop

	bgez	$2,$L131
	nop

	li	$4,1			# 0x1
	lui	$2,%hi($LC78)
	addiu	$5,$2,%lo($LC78)
	lui	$2,%hi(dbgtoMem)
	addiu	$2,$2,%lo(dbgtoMem)
	jalr	$2
	nop

	li	$2,-1			# 0xffffffffffffffff
	j	$L133
	nop

$L131:
	sw	$0,16($sp)
	lw	$2,32($sp)
	addiu	$2,$2,4
	move	$4,$2
	lui	$2,%hi(get16)
	addiu	$2,$2,%lo(get16)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(g_EPON_MAC_BASE)
	lw	$2,%lo(g_EPON_MAC_BASE)($2)
	addiu	$2,$2,24660
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	lw	$2,16($sp)
	li	$3,1			# 0x1
	ins	$2,$3,0,1
	sw	$2,16($sp)
	lw	$2,32($sp)
	lbu	$2,0($2)
	andi	$2,$2,0x7
	andi	$3,$2,0x00ff
	lw	$2,16($sp)
	ins	$2,$3,1,3
	sw	$2,16($sp)
	lw	$2,16($sp)
	li	$3,1			# 0x1
	ins	$2,$3,31,1
	sw	$2,16($sp)
	lw	$3,16($sp)
	lui	$2,%hi(g_EPON_MAC_BASE)
	lw	$2,%lo(g_EPON_MAC_BASE)($2)
	addiu	$2,$2,24656
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	lui	$2,%hi(eponMacCheckMacCfg)
	addiu	$2,$2,%lo(eponMacCheckMacCfg)
	jalr	$2
	nop

	bgez	$2,$L132
	nop

	li	$4,1			# 0x1
	lui	$2,%hi($LC79)
	addiu	$5,$2,%lo($LC79)
	lui	$2,%hi(dbgtoMem)
	addiu	$2,$2,%lo(dbgtoMem)
	jalr	$2
	nop

	li	$2,-1			# 0xffffffffffffffff
	j	$L133
	nop

$L132:
	move	$2,$0
$L133:
	lw	$31,28($sp)
	addiu	$sp,$sp,32
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	eponMacSetMacAddr
	.size	eponMacSetMacAddr, .-eponMacSetMacAddr
	.rdata
	.align	2
$LC80:
	.ascii	"enter eponMacGetMacAddr\000"
	.align	2
$LC81:
	.ascii	"ERR: eponMacGetMacAddr timers == 100\000"
	.align	2
$LC82:
	.ascii	"eponMacSetMacAddr set 1 macLow == %8X\000"
	.align	2
$LC83:
	.ascii	"eponMacSetMacAddr set 2 macHigh == %8X\000"
	.section	.text.eponMacGetMacAddr,"ax",@progbits
	.align	2
	.globl	eponMacGetMacAddr
	.set	nomips16
	.set	nomicromips
	.ent	eponMacGetMacAddr
	.type	eponMacGetMacAddr, @function
eponMacGetMacAddr:
	.frame	$sp,40,$31		# vars= 16, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-40
	sw	$31,36($sp)
	sw	$4,40($sp)
	sw	$0,16($sp)
	sw	$0,20($sp)
	li	$4,4			# 0x4
	lui	$2,%hi($LC80)
	addiu	$5,$2,%lo($LC80)
	lui	$2,%hi(dbgtoMem)
	addiu	$2,$2,%lo(dbgtoMem)
	jalr	$2
	nop

	lw	$2,40($sp)
	bne	$2,$0,$L135
	nop

	li	$2,-1			# 0xffffffffffffffff
	j	$L140
	nop

$L135:
	lui	$2,%hi(eponMacCheckMacCfg)
	addiu	$2,$2,%lo(eponMacCheckMacCfg)
	jalr	$2
	nop

	bgez	$2,$L137
	nop

	li	$4,1			# 0x1
	lui	$2,%hi($LC81)
	addiu	$5,$2,%lo($LC81)
	lui	$2,%hi(dbgtoMem)
	addiu	$2,$2,%lo(dbgtoMem)
	jalr	$2
	nop

	li	$2,-1			# 0xffffffffffffffff
	j	$L140
	nop

$L137:
	sw	$0,24($sp)
	lw	$2,24($sp)
	ins	$2,$0,0,1
	sw	$2,24($sp)
	lw	$2,40($sp)
	lbu	$2,0($2)
	andi	$2,$2,0x7
	andi	$3,$2,0x00ff
	lw	$2,24($sp)
	ins	$2,$3,1,3
	sw	$2,24($sp)
	lw	$2,24($sp)
	ins	$2,$0,31,1
	sw	$2,24($sp)
	lw	$3,24($sp)
	lui	$2,%hi(g_EPON_MAC_BASE)
	lw	$2,%lo(g_EPON_MAC_BASE)($2)
	addiu	$2,$2,24656
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	lui	$2,%hi(eponMacCheckMacCfg)
	addiu	$2,$2,%lo(eponMacCheckMacCfg)
	jalr	$2
	nop

	bgez	$2,$L138
	nop

	li	$4,1			# 0x1
	lui	$2,%hi($LC78)
	addiu	$5,$2,%lo($LC78)
	lui	$2,%hi(dbgtoMem)
	addiu	$2,$2,%lo(dbgtoMem)
	jalr	$2
	nop

	li	$2,-1			# 0xffffffffffffffff
	j	$L140
	nop

$L138:
	lui	$2,%hi(g_EPON_MAC_BASE)
	lw	$2,%lo(g_EPON_MAC_BASE)($2)
	addiu	$2,$2,24660
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	sw	$2,16($sp)
	li	$4,4			# 0x4
	lui	$2,%hi($LC82)
	addiu	$5,$2,%lo($LC82)
	lw	$6,16($sp)
	lui	$2,%hi(dbgtoMem)
	addiu	$2,$2,%lo(dbgtoMem)
	jalr	$2
	nop

	sw	$0,24($sp)
	lw	$2,24($sp)
	li	$3,1			# 0x1
	ins	$2,$3,0,1
	sw	$2,24($sp)
	lw	$2,40($sp)
	lbu	$2,0($2)
	andi	$2,$2,0x7
	andi	$3,$2,0x00ff
	lw	$2,24($sp)
	ins	$2,$3,1,3
	sw	$2,24($sp)
	lw	$2,24($sp)
	ins	$2,$0,31,1
	sw	$2,24($sp)
	lw	$3,24($sp)
	lui	$2,%hi(g_EPON_MAC_BASE)
	lw	$2,%lo(g_EPON_MAC_BASE)($2)
	addiu	$2,$2,24656
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	lui	$2,%hi(eponMacCheckMacCfg)
	addiu	$2,$2,%lo(eponMacCheckMacCfg)
	jalr	$2
	nop

	bgez	$2,$L139
	nop

	li	$4,1			# 0x1
	lui	$2,%hi($LC79)
	addiu	$5,$2,%lo($LC79)
	lui	$2,%hi(dbgtoMem)
	addiu	$2,$2,%lo(dbgtoMem)
	jalr	$2
	nop

	li	$2,-1			# 0xffffffffffffffff
	j	$L140
	nop

$L139:
	lui	$2,%hi(g_EPON_MAC_BASE)
	lw	$2,%lo(g_EPON_MAC_BASE)($2)
	addiu	$2,$2,24660
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	sw	$2,20($sp)
	li	$4,4			# 0x4
	lui	$2,%hi($LC83)
	addiu	$5,$2,%lo($LC83)
	lw	$6,20($sp)
	lui	$2,%hi(dbgtoMem)
	addiu	$2,$2,%lo(dbgtoMem)
	jalr	$2
	nop

	move	$2,$0
$L140:
	lw	$31,36($sp)
	addiu	$sp,$sp,40
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	eponMacGetMacAddr
	.size	eponMacGetMacAddr, .-eponMacGetMacAddr
	.rdata
	.align	2
$LC84:
	.ascii	"enter eponLlidEnable\000"
	.align	2
$LC85:
	.ascii	"ERR: eponMacSetMacAddr fail llidIndex=%d\000"
	.section	.text.eponLlidEnable,"ax",@progbits
	.align	2
	.globl	eponLlidEnable
	.set	nomips16
	.set	nomicromips
	.ent	eponLlidEnable
	.type	eponLlidEnable, @function
eponLlidEnable:
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
	sw	$0,20($sp)
	sw	$0,24($sp)
	sh	$0,28($sp)
	li	$4,4			# 0x4
	lui	$2,%hi($LC84)
	addiu	$5,$2,%lo($LC84)
	lui	$2,%hi(dbgtoMem)
	addiu	$2,$2,%lo(dbgtoMem)
	jalr	$2
	nop

	addiu	$2,$sp,24
	move	$4,$2
	lui	$2,%hi(eponOnuMacAddr)
	addiu	$5,$2,%lo(eponOnuMacAddr)
	li	$6,6			# 0x6
	lui	$2,%hi(memcpy)
	addiu	$2,$2,%lo(memcpy)
	jalr	$2
	nop

	addiu	$2,$sp,24
	addiu	$2,$2,2
	move	$4,$2
	lui	$2,%hi(get32)
	addiu	$2,$2,%lo(get32)
	jalr	$2
	nop

	sw	$2,20($sp)
	lbu	$3,40($sp)
	lw	$2,20($sp)
	addu	$2,$3,$2
	sw	$2,20($sp)
	addiu	$2,$sp,24
	addiu	$3,$2,3
	addiu	$2,$sp,20
	addiu	$2,$2,1
	move	$4,$3
	move	$5,$2
	li	$6,3			# 0x3
	lui	$2,%hi(memcpy)
	addiu	$2,$2,%lo(memcpy)
	jalr	$2
	nop

	lw	$2,16($sp)
	addiu	$3,$2,4
	addiu	$2,$sp,24
	move	$4,$3
	move	$5,$2
	li	$6,6			# 0x6
	lui	$2,%hi(memcpy)
	addiu	$2,$2,%lo(memcpy)
	jalr	$2
	nop

	sw	$0,20($sp)
	lw	$4,16($sp)
	lui	$2,%hi(eponMacSetMacAddr)
	addiu	$2,$2,%lo(eponMacSetMacAddr)
	jalr	$2
	nop

	bgez	$2,$L142
	nop

	lbu	$3,40($sp)
	li	$4,1			# 0x1
	lui	$2,%hi($LC85)
	addiu	$5,$2,%lo($LC85)
	move	$6,$3
	lui	$2,%hi(dbgtoMem)
	addiu	$2,$2,%lo(dbgtoMem)
	jalr	$2
	nop

	li	$2,-1			# 0xffffffffffffffff
	j	$L145
	nop

$L142:
	lui	$2,%hi(eponDebugLevel)
	lw	$2,%lo(eponDebugLevel)($2)
	sltu	$2,$2,2
	bne	$2,$0,$L144
	nop

	lw	$4,16($sp)
	lui	$2,%hi(eponMacGetMacAddr)
	addiu	$2,$2,%lo(eponMacGetMacAddr)
	jalr	$2
	nop

$L144:
	lw	$2,16($sp)
	sb	$0,20($2)
	lw	$2,16($sp)
	li	$3,1			# 0x1
	sw	$3,24($2)
	lw	$2,16($sp)
	sw	$0,56($2)
	lbu	$2,40($sp)
	move	$4,$2
	lui	$2,%hi(eponMpcpDscvFsmWaitHandler)
	addiu	$2,$2,%lo(eponMpcpDscvFsmWaitHandler)
	jalr	$2
	nop

	lw	$2,16($sp)
	li	$3,1			# 0x1
	sb	$3,21($2)
	lw	$2,16($sp)
	li	$3,1			# 0x1
	sb	$3,20($2)
	lw	$2,16($sp)
	li	$3,1			# 0x1
	sb	$3,1($2)
	lui	$2,%hi(g_EPON_MAC_BASE)
	lw	$2,%lo(g_EPON_MAC_BASE)($2)
	addiu	$2,$2,24584
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	sw	$2,20($sp)
	lbu	$2,40($sp)
	addiu	$2,$2,1
	li	$3,1			# 0x1
	sll	$2,$3,$2
	move	$3,$2
	lw	$2,20($sp)
	or	$2,$3,$2
	sw	$2,20($sp)
	lw	$3,20($sp)
	lui	$2,%hi(g_EPON_MAC_BASE)
	lw	$2,%lo(g_EPON_MAC_BASE)($2)
	addiu	$2,$2,24584
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	move	$2,$0
$L145:
	lw	$31,36($sp)
	addiu	$sp,$sp,40
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	eponLlidEnable
	.size	eponLlidEnable, .-eponLlidEnable
	.rdata
	.align	2
$LC86:
	.ascii	"enter eponLlidDisable\000"
	.section	.text.eponLlidDisable,"ax",@progbits
	.align	2
	.globl	eponLlidDisable
	.set	nomips16
	.set	nomicromips
	.ent	eponLlidDisable
	.type	eponLlidDisable, @function
eponLlidDisable:
	.frame	$sp,176,$31		# vars= 152, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-176
	sw	$31,172($sp)
	move	$2,$4
	sb	$2,176($sp)
	lbu	$2,176($sp)
	sll	$2,$2,3
	sll	$3,$2,3
	addu	$2,$2,$3
	addiu	$3,$2,16
	lui	$2,%hi(eponDrv)
	addiu	$2,$2,%lo(eponDrv)
	addu	$2,$3,$2
	addiu	$2,$2,4
	sw	$2,16($sp)
	sw	$0,20($sp)
	li	$4,4			# 0x4
	lui	$2,%hi($LC86)
	addiu	$5,$2,%lo($LC86)
	lui	$2,%hi(dbgtoMem)
	addiu	$2,$2,%lo(dbgtoMem)
	jalr	$2
	nop

	lw	$2,16($sp)
	sb	$0,1($2)
	lui	$2,%hi(g_EPON_MAC_BASE)
	lw	$2,%lo(g_EPON_MAC_BASE)($2)
	addiu	$2,$2,24584
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	sw	$2,20($sp)
	lbu	$2,176($sp)
	addiu	$2,$2,1
	li	$3,1			# 0x1
	sll	$2,$3,$2
	nor	$2,$0,$2
	move	$3,$2
	lw	$2,20($sp)
	and	$2,$2,$3
	sw	$2,20($sp)
	lui	$2,%hi(g_EPON_MAC_BASE)
	lw	$2,%lo(g_EPON_MAC_BASE)($2)
	addiu	$2,$2,24584
	lw	$4,20($sp)
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	lui	$2,%hi(g_EPON_MAC_BASE)
	lw	$2,%lo(g_EPON_MAC_BASE)($2)
	addiu	$3,$2,24620
	lbu	$2,176($sp)
	sll	$2,$2,2
	addu	$2,$3,$2
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	sw	$2,36($sp)
	lw	$2,36($sp)
	ins	$2,$0,30,2
	sw	$2,36($sp)
	lw	$4,36($sp)
	lui	$2,%hi(g_EPON_MAC_BASE)
	lw	$2,%lo(g_EPON_MAC_BASE)($2)
	addiu	$3,$2,24620
	lbu	$2,176($sp)
	sll	$2,$2,2
	addu	$2,$3,$2
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	lbu	$2,176($sp)
	li	$3,1			# 0x1
	sw	$3,24($sp)
	sb	$2,28($sp)
	sw	$0,32($sp)
	li	$2,47			# 0x2f
	sw	$2,40($sp)
	lw	$2,24($sp)
	sw	$2,44($sp)
	lbu	$2,28($sp)
	sw	$2,68($sp)
	addiu	$2,$sp,40
	li	$4,18			# 0x12
	move	$5,$0
	move	$6,$2
	lui	$2,%hi(__ECNT_HOOK)
	addiu	$2,$2,%lo(__ECNT_HOOK)
	jalr	$2
	nop

	sw	$2,32($sp)
	lbu	$2,176($sp)
	move	$4,$2
	lui	$2,%hi(xmcs_remove_llid)
	addiu	$2,$2,%lo(xmcs_remove_llid)
	jalr	$2
	nop

	move	$2,$0
	lw	$31,172($sp)
	addiu	$sp,$sp,176
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	eponLlidDisable
	.size	eponLlidDisable, .-eponLlidDisable
	.rdata
	.align	2
$LC87:
	.ascii	"eponInitAllLlid llid %d , Timeout %d, State %d\000"
	.section	.text.eponInitAllLlid,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	eponInitAllLlid
	.type	eponInitAllLlid, @function
eponInitAllLlid:
	.frame	$sp,64,$31		# vars= 32, regs= 1/0, args= 24, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-64
	sw	$31,60($sp)
	sb	$0,24($sp)
	sw	$0,28($sp)
	sw	$0,32($sp)
	sw	$0,36($sp)
	sw	$0,40($sp)
	lw	$2,28($sp)
	ori	$2,$2,0xfa01
	sw	$2,28($sp)
	lui	$2,%hi(gpEponPriv)
	lw	$2,%lo(gpEponPriv)($2)
	lbu	$2,40($2)
	beq	$2,$0,$L152
	nop

	lw	$3,28($sp)
	li	$2,50331648			# 0x3000000
	or	$2,$3,$2
	sw	$2,28($sp)
$L152:
	lui	$2,%hi(g_EPON_MAC_BASE)
	lw	$2,%lo(g_EPON_MAC_BASE)($2)
	addiu	$2,$2,24584
	lw	$4,28($sp)
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	lui	$2,%hi(eponDrv)
	li	$3,1			# 0x1
	sw	$3,%lo(eponDrv)($2)
	lui	$2,%hi(eponDefLlidMask)
	lw	$3,%lo(eponDefLlidMask)($2)
	lui	$2,%hi(eponDrv)
	sw	$3,%lo(eponDrv)($2)
	j	$L153
	nop

$L159:
	lbu	$2,24($sp)
	sll	$2,$2,3
	sll	$3,$2,3
	addu	$2,$2,$3
	addiu	$3,$2,16
	lui	$2,%hi(eponDrv)
	addiu	$2,$2,%lo(eponDrv)
	addu	$2,$3,$2
	addiu	$2,$2,4
	sw	$2,32($sp)
	lw	$2,32($sp)
	lhu	$2,68($2)
	sw	$2,36($sp)
	lw	$2,32($sp)
	lbu	$2,20($2)
	sw	$2,40($sp)
	lbu	$3,24($sp)
	lw	$2,40($sp)
	sw	$2,16($sp)
	li	$4,4			# 0x4
	lui	$2,%hi($LC87)
	addiu	$5,$2,%lo($LC87)
	move	$6,$3
	lw	$7,36($sp)
	lui	$2,%hi(dbgtoMem)
	addiu	$2,$2,%lo(dbgtoMem)
	jalr	$2
	nop

	lw	$4,32($sp)
	move	$5,$0
	li	$6,72			# 0x48
	lui	$2,%hi(memset)
	addiu	$2,$2,%lo(memset)
	jalr	$2
	nop

	lw	$2,32($sp)
	lbu	$3,24($sp)
	sb	$3,0($2)
	lui	$2,%hi(mpcpDscvPendingLock)
	addiu	$2,$2,%lo(mpcpDscvPendingLock)
	sw	$2,44($sp)
	lui	$2,%hi(mpcpDscvPendingLock)
	sw	$0,%lo(mpcpDscvPendingLock)($2)
	lui	$2,%hi(mpcpDscvGateLock)
	addiu	$2,$2,%lo(mpcpDscvGateLock)
	sw	$2,48($sp)
	lui	$2,%hi(mpcpDscvGateLock)
	sw	$0,%lo(mpcpDscvGateLock)($2)
	lui	$2,%hi(eponDrv)
	lw	$2,%lo(eponDrv)($2)
	lbu	$3,24($sp)
	li	$4,1			# 0x1
	sll	$3,$4,$3
	and	$2,$2,$3
	beq	$2,$0,$L156
	nop

	lbu	$2,24($sp)
	move	$4,$2
	lui	$2,%hi(eponLlidEnable)
	addiu	$2,$2,%lo(eponLlidEnable)
	jalr	$2
	nop

$L156:
	lw	$3,40($sp)
	li	$2,5			# 0x5
	bne	$3,$2,$L157
	nop

	lw	$2,36($sp)
	blez	$2,$L158
	nop

	lui	$2,%hi(g_silence_time)
	lbu	$2,%lo(g_silence_time)($2)
	move	$3,$2
	lw	$2,36($sp)
	slt	$2,$3,$2
	bne	$2,$0,$L158
	nop

	lw	$2,36($sp)
	andi	$3,$2,0xffff
	lw	$2,32($sp)
	sh	$3,68($2)
$L158:
	lw	$2,40($sp)
	andi	$3,$2,0x00ff
	lw	$2,32($sp)
	sb	$3,20($2)
$L157:
	lbu	$2,24($sp)
	addiu	$2,$2,1
	sb	$2,24($sp)
$L153:
	lbu	$2,24($sp)
	sltu	$2,$2,8
	bne	$2,$0,$L159
	nop

	move	$2,$0
	lw	$31,60($sp)
	addiu	$sp,$sp,64
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	eponInitAllLlid
	.size	eponInitAllLlid, .-eponInitAllLlid
	.section	.text.epon_set_qdma_qos,"ax",@progbits
	.align	2
	.globl	epon_set_qdma_qos
	.set	nomips16
	.set	nomicromips
	.ent	epon_set_qdma_qos
	.type	epon_set_qdma_qos, @function
epon_set_qdma_qos:
	.frame	$sp,248,$31		# vars= 224, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-248
	sw	$31,244($sp)
	addiu	$2,$sp,64
	move	$4,$2
	move	$5,$0
	li	$6,8			# 0x8
	lui	$2,%hi(memset)
	addiu	$2,$2,%lo(memset)
	jalr	$2
	nop

	li	$2,1000			# 0x3e8
	sh	$2,64($sp)
	sw	$0,68($sp)
	li	$2,16			# 0x10
	sw	$2,16($sp)
	addiu	$2,$sp,64
	sw	$2,20($sp)
	sw	$0,24($sp)
	li	$2,33			# 0x21
	sw	$2,180($sp)
	lw	$2,20($sp)
	sw	$2,188($sp)
	addiu	$2,$sp,180
	lw	$4,16($sp)
	move	$5,$0
	move	$6,$2
	lui	$2,%hi(__ECNT_HOOK)
	addiu	$2,$2,%lo(__ECNT_HOOK)
	jalr	$2
	nop

	sw	$2,24($sp)
	addiu	$2,$sp,84
	move	$4,$2
	move	$5,$0
	li	$6,12			# 0xc
	lui	$2,%hi(memset)
	addiu	$2,$2,%lo(memset)
	jalr	$2
	nop

	li	$2,1			# 0x1
	sw	$2,84($sp)
	li	$2,3			# 0x3
	sw	$2,88($sp)
	li	$2,255			# 0xff
	sw	$2,92($sp)
	li	$2,16			# 0x10
	sw	$2,28($sp)
	addiu	$2,$sp,84
	sw	$2,32($sp)
	sw	$0,36($sp)
	li	$2,57			# 0x39
	sw	$2,152($sp)
	lw	$2,32($sp)
	sw	$2,160($sp)
	addiu	$2,$sp,152
	lw	$4,28($sp)
	move	$5,$0
	move	$6,$2
	lui	$2,%hi(__ECNT_HOOK)
	addiu	$2,$2,%lo(__ECNT_HOOK)
	jalr	$2
	nop

	sw	$2,36($sp)
	addiu	$2,$sp,72
	move	$4,$2
	move	$5,$0
	li	$6,12			# 0xc
	lui	$2,%hi(memset)
	addiu	$2,$2,%lo(memset)
	jalr	$2
	nop

	li	$2,16			# 0x10
	sw	$2,40($sp)
	addiu	$2,$sp,72
	sw	$2,44($sp)
	sw	$0,48($sp)
	li	$2,60			# 0x3c
	sw	$2,124($sp)
	lw	$2,44($sp)
	sw	$2,132($sp)
	addiu	$2,$sp,124
	lw	$4,40($sp)
	move	$5,$0
	move	$6,$2
	lui	$2,%hi(__ECNT_HOOK)
	addiu	$2,$2,%lo(__ECNT_HOOK)
	jalr	$2
	nop

	sw	$2,48($sp)
	li	$2,768			# 0x300
	sh	$2,74($sp)
	li	$2,48			# 0x30
	sh	$2,78($sp)
	li	$2,48			# 0x30
	sh	$2,82($sp)
	li	$2,16			# 0x10
	sw	$2,52($sp)
	addiu	$2,$sp,72
	sw	$2,56($sp)
	sw	$0,60($sp)
	li	$2,59			# 0x3b
	sw	$2,96($sp)
	lw	$2,56($sp)
	sw	$2,104($sp)
	addiu	$2,$sp,96
	lw	$4,52($sp)
	move	$5,$0
	move	$6,$2
	lui	$2,%hi(__ECNT_HOOK)
	addiu	$2,$2,%lo(__ECNT_HOOK)
	jalr	$2
	nop

	sw	$2,60($sp)
	move	$2,$0
	lw	$31,244($sp)
	addiu	$sp,$sp,248
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	epon_set_qdma_qos
	.size	epon_set_qdma_qos, .-epon_set_qdma_qos
	.section	.text.eponTxOamFavorModeCtl,"ax",@progbits
	.align	2
	.globl	eponTxOamFavorModeCtl
	.set	nomips16
	.set	nomicromips
	.ent	eponTxOamFavorModeCtl
	.type	eponTxOamFavorModeCtl, @function
eponTxOamFavorModeCtl:
	.frame	$sp,288,$31		# vars= 264, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-288
	sw	$31,284($sp)
	move	$2,$4
	sb	$2,288($sp)
	sw	$0,16($sp)
	li	$2,-1078984704			# 0xffffffffbfb00000
	ori	$2,$2,0x64
	lw	$3,0($2)
	li	$2,-65536			# 0xffffffffffff0000
	and	$3,$3,$2
	li	$2,524288			# 0x80000
	beq	$3,$2,$L176
	nop

	move	$2,$0
	j	$L192
	nop

$L176:
	addiu	$2,$sp,60
	move	$4,$2
	move	$5,$0
	li	$6,8			# 0x8
	lui	$2,%hi(memset)
	addiu	$2,$2,%lo(memset)
	jalr	$2
	nop

	lbu	$3,288($sp)
	li	$2,1			# 0x1
	bne	$3,$2,$L178
	nop

	sw	$0,60($sp)
	li	$2,7			# 0x7
	sw	$2,64($sp)
	li	$2,16			# 0x10
	sw	$2,20($sp)
	addiu	$2,$sp,60
	sw	$2,24($sp)
	sw	$0,28($sp)
	li	$2,73			# 0x49
	sw	$2,96($sp)
	lw	$2,24($sp)
	sw	$2,104($sp)
	addiu	$2,$sp,96
	lw	$4,20($sp)
	move	$5,$0
	move	$6,$2
	lui	$2,%hi(__ECNT_HOOK)
	addiu	$2,$2,%lo(__ECNT_HOOK)
	jalr	$2
	nop

	sw	$2,28($sp)
	li	$2,1			# 0x1
	sw	$2,32($sp)
	sw	$0,36($sp)
	li	$2,67			# 0x43
	sw	$2,124($sp)
	lw	$2,32($sp)
	sw	$2,176($sp)
	addiu	$2,$sp,124
	li	$4,18			# 0x12
	move	$5,$0
	move	$6,$2
	lui	$2,%hi(__ECNT_HOOK)
	addiu	$2,$2,%lo(__ECNT_HOOK)
	jalr	$2
	nop

	sw	$2,36($sp)
	lui	$2,%hi(g_EPON_MAC_BASE)
	lw	$2,%lo(g_EPON_MAC_BASE)($2)
	addiu	$2,$2,24576
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	sw	$2,16($sp)
	lw	$3,16($sp)
	li	$2,33554432			# 0x2000000
	or	$2,$3,$2
	sw	$2,16($sp)
	lui	$2,%hi(g_EPON_MAC_BASE)
	lw	$2,%lo(g_EPON_MAC_BASE)($2)
	addiu	$2,$2,24576
	lw	$4,16($sp)
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	j	$L185
	nop

$L178:
	li	$2,1			# 0x1
	sw	$2,60($sp)
	li	$2,7			# 0x7
	sw	$2,64($sp)
	li	$2,16			# 0x10
	sw	$2,40($sp)
	addiu	$2,$sp,60
	sw	$2,44($sp)
	sw	$0,48($sp)
	li	$2,73			# 0x49
	sw	$2,68($sp)
	lw	$2,44($sp)
	sw	$2,76($sp)
	addiu	$2,$sp,68
	lw	$4,40($sp)
	move	$5,$0
	move	$6,$2
	lui	$2,%hi(__ECNT_HOOK)
	addiu	$2,$2,%lo(__ECNT_HOOK)
	jalr	$2
	nop

	sw	$2,48($sp)
	sw	$0,52($sp)
	sw	$0,56($sp)
	li	$2,67			# 0x43
	sw	$2,124($sp)
	lw	$2,52($sp)
	sw	$2,176($sp)
	addiu	$2,$sp,124
	li	$4,18			# 0x12
	move	$5,$0
	move	$6,$2
	lui	$2,%hi(__ECNT_HOOK)
	addiu	$2,$2,%lo(__ECNT_HOOK)
	jalr	$2
	nop

	sw	$2,56($sp)
	lui	$2,%hi(g_EPON_MAC_BASE)
	lw	$2,%lo(g_EPON_MAC_BASE)($2)
	addiu	$2,$2,24576
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	sw	$2,16($sp)
	lw	$3,16($sp)
	li	$2,-33619968			# 0xfffffffffdff0000
	ori	$2,$2,0xffff
	and	$2,$3,$2
	sw	$2,16($sp)
	lui	$2,%hi(g_EPON_MAC_BASE)
	lw	$2,%lo(g_EPON_MAC_BASE)($2)
	addiu	$2,$2,24576
	lw	$4,16($sp)
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

$L185:
	move	$2,$0
$L192:
	lw	$31,284($sp)
	addiu	$sp,$sp,288
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	eponTxOamFavorModeCtl
	.size	eponTxOamFavorModeCtl, .-eponTxOamFavorModeCtl
	.section	.text.eponInitParam,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	eponInitParam
	.type	eponInitParam, @function
eponInitParam:
	.frame	$sp,24,$31		# vars= 0, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-24
	sw	$31,20($sp)
	lui	$2,%hi(eponDrv)
	addiu	$2,$2,%lo(eponDrv)
	sb	$0,4($2)
	lui	$2,%hi(eponDrv)
	addiu	$2,$2,%lo(eponDrv)
	sh	$0,6($2)
	lui	$2,%hi(eponDrv)
	addiu	$2,$2,%lo(eponDrv)
	sw	$0,12($2)
	lui	$2,%hi(eponDrv)
	addiu	$2,$2,%lo(eponDrv)
	sw	$0,16($2)
	lui	$2,%hi(eponDrv)
	li	$3,1			# 0x1
	sw	$3,%lo(eponDrv)($2)
	lui	$2,%hi(gpWanPriv)
	lw	$2,%lo(gpWanPriv)($2)
	li	$3,1			# 0x1
	sw	$3,20($2)
	lui	$2,%hi(xpon_reset_qdma_tx_buf)
	addiu	$2,$2,%lo(xpon_reset_qdma_tx_buf)
	jalr	$2
	nop

	lui	$2,%hi(epon_set_qdma_qos)
	addiu	$2,$2,%lo(epon_set_qdma_qos)
	jalr	$2
	nop

	move	$2,$0
	lw	$31,20($sp)
	addiu	$sp,$sp,24
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	eponInitParam
	.size	eponInitParam, .-eponInitParam
	.section	.text.eponHwDygaspCtrl,"ax",@progbits
	.align	2
	.globl	eponHwDygaspCtrl
	.set	nomips16
	.set	nomicromips
	.ent	eponHwDygaspCtrl
	.type	eponHwDygaspCtrl, @function
eponHwDygaspCtrl:
	.frame	$sp,32,$31		# vars= 8, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-32
	sw	$31,28($sp)
	move	$2,$4
	sb	$2,32($sp)
	lui	$2,%hi(g_EPON_MAC_BASE)
	lw	$2,%lo(g_EPON_MAC_BASE)($2)
	addiu	$2,$2,24592
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	sw	$2,16($sp)
	lbu	$3,32($sp)
	li	$2,1			# 0x1
	bne	$3,$2,$L196
	nop

	li	$2,-2147483648			# 0xffffffff80000000
	ori	$2,$2,0x102
	sw	$2,16($sp)
	lui	$2,%hi(g_EPON_MAC_BASE)
	lw	$2,%lo(g_EPON_MAC_BASE)($2)
	addiu	$2,$2,24592
	lw	$4,16($sp)
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	j	$L197
	nop

$L196:
	lbu	$2,32($sp)
	bne	$2,$0,$L197
	nop

	sw	$0,16($sp)
	lui	$2,%hi(g_EPON_MAC_BASE)
	lw	$2,%lo(g_EPON_MAC_BASE)($2)
	addiu	$2,$2,24592
	lw	$4,16($sp)
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

$L197:
	move	$2,$0
	lw	$31,28($sp)
	addiu	$sp,$sp,32
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	eponHwDygaspCtrl
	.size	eponHwDygaspCtrl, .-eponHwDygaspCtrl
	.section	.text.eponDevGetDyingGaspMode,"ax",@progbits
	.align	2
	.globl	eponDevGetDyingGaspMode
	.set	nomips16
	.set	nomicromips
	.ent	eponDevGetDyingGaspMode
	.type	eponDevGetDyingGaspMode, @function
eponDevGetDyingGaspMode:
	.frame	$sp,32,$31		# vars= 8, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-32
	sw	$31,28($sp)
	sw	$4,32($sp)
	lui	$2,%hi(g_EPON_MAC_BASE)
	lw	$2,%lo(g_EPON_MAC_BASE)($2)
	addiu	$2,$2,24592
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	sw	$2,16($sp)
	lw	$2,16($sp)
	ext	$2,$2,31,1
	andi	$2,$2,0x00ff
	move	$3,$2
	lw	$2,32($sp)
	sb	$3,0($2)
	move	$2,$0
	lw	$31,28($sp)
	addiu	$sp,$sp,32
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	eponDevGetDyingGaspMode
	.size	eponDevGetDyingGaspMode, .-eponDevGetDyingGaspMode
	.section	.text.eponDevSetDyingGaspNum,"ax",@progbits
	.align	2
	.globl	eponDevSetDyingGaspNum
	.set	nomips16
	.set	nomicromips
	.ent	eponDevSetDyingGaspNum
	.type	eponDevSetDyingGaspNum, @function
eponDevSetDyingGaspNum:
	.frame	$sp,32,$31		# vars= 8, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-32
	sw	$31,28($sp)
	sw	$4,32($sp)
	lui	$2,%hi(g_EPON_MAC_BASE)
	lw	$2,%lo(g_EPON_MAC_BASE)($2)
	addiu	$2,$2,24592
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	sw	$2,16($sp)
	lw	$2,32($sp)
	andi	$2,$2,0x00ff
	andi	$2,$2,0x3
	andi	$3,$2,0x00ff
	lw	$2,16($sp)
	ins	$2,$3,8,2
	sw	$2,16($sp)
	lw	$3,16($sp)
	lui	$2,%hi(g_EPON_MAC_BASE)
	lw	$2,%lo(g_EPON_MAC_BASE)($2)
	addiu	$2,$2,24592
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
	.end	eponDevSetDyingGaspNum
	.size	eponDevSetDyingGaspNum, .-eponDevSetDyingGaspNum
	.section	.text.eponDevGetDyingGaspNum,"ax",@progbits
	.align	2
	.globl	eponDevGetDyingGaspNum
	.set	nomips16
	.set	nomicromips
	.ent	eponDevGetDyingGaspNum
	.type	eponDevGetDyingGaspNum, @function
eponDevGetDyingGaspNum:
	.frame	$sp,32,$31		# vars= 8, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-32
	sw	$31,28($sp)
	sw	$4,32($sp)
	lui	$2,%hi(g_EPON_MAC_BASE)
	lw	$2,%lo(g_EPON_MAC_BASE)($2)
	addiu	$2,$2,24592
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	sw	$2,16($sp)
	lw	$2,16($sp)
	ext	$2,$2,8,2
	andi	$2,$2,0x00ff
	move	$3,$2
	lw	$2,32($sp)
	sw	$3,0($2)
	move	$2,$0
	lw	$31,28($sp)
	addiu	$sp,$sp,32
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	eponDevGetDyingGaspNum
	.size	eponDevGetDyingGaspNum, .-eponDevGetDyingGaspNum
	.section	.text.eponSnifferModeCtrl,"ax",@progbits
	.align	2
	.globl	eponSnifferModeCtrl
	.set	nomips16
	.set	nomicromips
	.ent	eponSnifferModeCtrl
	.type	eponSnifferModeCtrl, @function
eponSnifferModeCtrl:
	.frame	$sp,40,$31		# vars= 16, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-40
	sw	$31,36($sp)
	move	$2,$4
	sb	$2,40($sp)
	sw	$0,16($sp)
	sw	$0,20($sp)
	lui	$2,%hi(gpEponPriv)
	lw	$2,%lo(gpEponPriv)($2)
	lbu	$2,38($2)
	sb	$2,24($sp)
	li	$2,-1078984704			# 0xffffffffbfb00000
	ori	$2,$2,0x64
	lw	$3,0($2)
	li	$2,-65536			# 0xffffffffffff0000
	and	$3,$3,$2
	li	$2,524288			# 0x80000
	beq	$3,$2,$L206
	nop

	j	$L205
	nop

$L206:
	lui	$2,%hi(g_EPON_MAC_BASE)
	lw	$2,%lo(g_EPON_MAC_BASE)($2)
	addiu	$2,$2,24576
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	sw	$2,16($sp)
	lbu	$2,40($sp)
	beq	$2,$0,$L208
	nop

	lw	$3,16($sp)
	li	$2,71303168			# 0x4400000
	ori	$2,$2,0x400
	or	$2,$3,$2
	sw	$2,16($sp)
	lbu	$3,40($sp)
	li	$2,2			# 0x2
	bne	$3,$2,$L209
	nop

	lw	$3,16($sp)
	li	$2,262144			# 0x40000
	or	$2,$3,$2
	sw	$2,16($sp)
$L209:
	lui	$2,%hi(g_EPON_MAC_BASE)
	lw	$2,%lo(g_EPON_MAC_BASE)($2)
	addiu	$2,$2,24576
	lw	$4,16($sp)
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	lui	$2,%hi(g_EPON_MAC_BASE)
	lw	$2,%lo(g_EPON_MAC_BASE)($2)
	addiu	$2,$2,25048
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	sw	$2,20($sp)
	lw	$3,20($sp)
	li	$2,-4194304			# 0xffffffffffc00000
	ori	$2,$2,0xffff
	and	$2,$3,$2
	sw	$2,20($sp)
	lbu	$2,24($sp)
	andi	$2,$2,0x3f
	sll	$2,$2,16
	move	$3,$2
	lw	$2,20($sp)
	or	$2,$2,$3
	sw	$2,20($sp)
	lui	$2,%hi(g_EPON_MAC_BASE)
	lw	$2,%lo(g_EPON_MAC_BASE)($2)
	addiu	$2,$2,25048
	lw	$4,20($sp)
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	j	$L205
	nop

$L208:
	lw	$3,16($sp)
	li	$2,-71630848			# 0xfffffffffbbb0000
	ori	$2,$2,0xfbff
	and	$2,$3,$2
	sw	$2,16($sp)
	lui	$2,%hi(g_EPON_MAC_BASE)
	lw	$2,%lo(g_EPON_MAC_BASE)($2)
	addiu	$2,$2,24576
	lw	$4,16($sp)
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	lui	$2,%hi(g_EPON_MAC_BASE)
	lw	$2,%lo(g_EPON_MAC_BASE)($2)
	addiu	$2,$2,25048
	move	$4,$0
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

$L205:
	lw	$31,36($sp)
	addiu	$sp,$sp,40
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	eponSnifferModeCtrl
	.size	eponSnifferModeCtrl, .-eponSnifferModeCtrl
	.section	.text.eponSetSnifferModeConfig,"ax",@progbits
	.align	2
	.globl	eponSetSnifferModeConfig
	.set	nomips16
	.set	nomicromips
	.ent	eponSetSnifferModeConfig
	.type	eponSetSnifferModeConfig, @function
eponSetSnifferModeConfig:
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
	sh	$2,28($sp)
	li	$2,-1078984704			# 0xffffffffbfb00000
	ori	$2,$2,0x64
	lw	$3,0($2)
	li	$2,-65536			# 0xffffffffffff0000
	and	$3,$3,$2
	li	$2,524288			# 0x80000
	beq	$3,$2,$L211
	nop

	move	$2,$0
	j	$L212
	nop

$L211:
	lui	$2,%hi(gpEponPriv)
	lw	$2,%lo(gpEponPriv)($2)
	lbu	$3,24($sp)
	sb	$3,37($2)
	lui	$2,%hi(gpEponPriv)
	lw	$2,%lo(gpEponPriv)($2)
	lhu	$3,28($sp)
	andi	$3,$3,0x00ff
	sb	$3,38($2)
	lbu	$2,24($sp)
	move	$4,$2
	lui	$2,%hi(eponSnifferModeCtrl)
	addiu	$2,$2,%lo(eponSnifferModeCtrl)
	jalr	$2
	nop

	move	$2,$0
$L212:
	lw	$31,20($sp)
	addiu	$sp,$sp,24
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	eponSetSnifferModeConfig
	.size	eponSetSnifferModeConfig, .-eponSetSnifferModeConfig
	.rdata
	.align	2
$LC88:
	.ascii	"e_int_status=%x,e_llid0_dscvry_sts=%x\000"
	.align	2
$LC89:
	.ascii	"Enable epon static report\012\000"
	.align	2
$LC90:
	.ascii	"Raw = %x\012\000"
	.section	.text.eponMacSwReset,"ax",@progbits
	.align	2
	.globl	eponMacSwReset
	.set	nomips16
	.set	nomicromips
	.ent	eponMacSwReset
	.type	eponMacSwReset, @function
eponMacSwReset:
	.frame	$sp,264,$31		# vars= 240, regs= 2/0, args= 16, gp= 0
	.mask	0x80010000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-264
	sw	$31,260($sp)
	sw	$16,256($sp)
	sw	$0,252($sp)
	li	$2,-1078984704			# 0xffffffffbfb00000
	ori	$4,$2,0x834
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	sw	$2,16($sp)
	lw	$3,16($sp)
	li	$2,-2147483648			# 0xffffffff80000000
	or	$2,$3,$2
	sw	$2,16($sp)
	lw	$4,16($sp)
	li	$2,-1078984704			# 0xffffffffbfb00000
	ori	$5,$2,0x834
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	j	$L214
	nop

$L215:
	lw	$2,252($sp)
	addiu	$2,$2,1
	sw	$2,252($sp)
$L214:
	lw	$3,252($sp)
	li	$2,786432			# 0xc0000
	ori	$2,$2,0x3500
	sltu	$2,$3,$2
	bne	$2,$0,$L215
	nop

	lw	$2,16($sp)
	ext	$2,$2,0,31
	sw	$2,16($sp)
	lw	$4,16($sp)
	li	$2,-1078984704			# 0xffffffffbfb00000
	ori	$5,$2,0x834
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	sw	$0,252($sp)
	lui	$2,%hi(g_EPON_MAC_BASE)
	lw	$2,%lo(g_EPON_MAC_BASE)($2)
	addiu	$2,$2,24576
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	sw	$2,20($sp)
	lw	$2,20($sp)
	ori	$2,$2,0x10
	sw	$2,20($sp)
	lui	$2,%hi(g_EPON_MAC_BASE)
	lw	$2,%lo(g_EPON_MAC_BASE)($2)
	addiu	$2,$2,24576
	lw	$4,20($sp)
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	j	$L216
	nop

$L217:
	lw	$2,252($sp)
	addiu	$2,$2,1
	sw	$2,252($sp)
$L216:
	lw	$3,252($sp)
	li	$2,786432			# 0xc0000
	ori	$2,$2,0x3500
	sltu	$2,$3,$2
	bne	$2,$0,$L217
	nop

	lw	$3,20($sp)
	li	$2,-17			# 0xffffffffffffffef
	and	$2,$3,$2
	sw	$2,20($sp)
	lui	$2,%hi(g_EPON_MAC_BASE)
	lw	$2,%lo(g_EPON_MAC_BASE)($2)
	addiu	$2,$2,24576
	lw	$4,20($sp)
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	sw	$0,252($sp)
	j	$L218
	nop

$L219:
	lw	$2,252($sp)
	addiu	$2,$2,1
	sw	$2,252($sp)
$L218:
	lw	$3,252($sp)
	li	$2,786432			# 0xc0000
	ori	$2,$2,0x3500
	sltu	$2,$3,$2
	bne	$2,$0,$L219
	nop

	lui	$2,%hi(eponSetProbe)
	lw	$2,%lo(eponSetProbe)($2)
	beq	$2,$0,$L220
	nop

	lui	$2,%hi(eponSetProbe)
	lw	$3,%lo(eponSetProbe)($2)
	lui	$2,%hi(g_EPON_MAC_BASE)
	lw	$2,%lo(g_EPON_MAC_BASE)($2)
	addiu	$2,$2,24832
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

$L220:
	lui	$2,%hi(g_EPON_MAC_BASE)
	lw	$2,%lo(g_EPON_MAC_BASE)($2)
	addiu	$2,$2,24580
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	move	$16,$2
	lui	$2,%hi(g_EPON_MAC_BASE)
	lw	$2,%lo(g_EPON_MAC_BASE)($2)
	addiu	$2,$2,24620
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	move	$3,$2
	li	$4,4			# 0x4
	lui	$2,%hi($LC88)
	addiu	$5,$2,%lo($LC88)
	move	$6,$16
	move	$7,$3
	lui	$2,%hi(dbgtoMem)
	addiu	$2,$2,%lo(dbgtoMem)
	jalr	$2
	nop

	lw	$2,20($sp)
	ori	$2,$2,0x2
	sw	$2,20($sp)
	lui	$2,%hi(g_EPON_MAC_BASE)
	lw	$2,%lo(g_EPON_MAC_BASE)($2)
	addiu	$2,$2,24576
	lw	$4,20($sp)
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	lui	$2,%hi(g_EPON_MAC_BASE)
	lw	$2,%lo(g_EPON_MAC_BASE)($2)
	addiu	$2,$2,24800
	li	$4,8			# 0x8
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	lui	$2,%hi(g_EPON_MAC_BASE)
	lw	$2,%lo(g_EPON_MAC_BASE)($2)
	addiu	$3,$2,24820
	li	$2,5177344			# 0x4f0000
	ori	$4,$2,0xfff1
	move	$5,$3
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	lui	$2,%hi(g_EPON_MAC_BASE)
	lw	$2,%lo(g_EPON_MAC_BASE)($2)
	addiu	$2,$2,24824
	li	$4,6			# 0x6
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	lui	$2,%hi(g_EPON_MAC_BASE)
	lw	$2,%lo(g_EPON_MAC_BASE)($2)
	addiu	$3,$2,24784
	li	$2,539230208			# 0x20240000
	ori	$4,$2,0x3e8
	move	$5,$3
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	lui	$2,%hi(g_EPON_MAC_BASE)
	lw	$2,%lo(g_EPON_MAC_BASE)($2)
	addiu	$2,$2,24576
	sw	$2,24($sp)
	li	$2,1			# 0x1
	sw	$2,28($sp)
	li	$2,6			# 0x6
	sw	$2,32($sp)
	li	$2,1			# 0x1
	sw	$2,36($sp)
	lw	$2,24($sp)
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	sw	$2,40($sp)
	lw	$2,32($sp)
	lw	$3,28($sp)
	sll	$2,$3,$2
	nor	$3,$0,$2
	lw	$2,40($sp)
	and	$3,$3,$2
	lw	$4,28($sp)
	lw	$2,36($sp)
	and	$4,$4,$2
	lw	$2,32($sp)
	sll	$2,$4,$2
	or	$2,$3,$2
	sw	$2,40($sp)
	lw	$2,24($sp)
	lw	$4,40($sp)
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	lui	$2,%hi(g_EPON_MAC_BASE)
	lw	$2,%lo(g_EPON_MAC_BASE)($2)
	addiu	$2,$2,24576
	sw	$2,44($sp)
	li	$2,1			# 0x1
	sw	$2,48($sp)
	li	$2,1			# 0x1
	sw	$2,52($sp)
	li	$2,1			# 0x1
	sw	$2,56($sp)
	lw	$2,44($sp)
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	sw	$2,60($sp)
	lw	$2,52($sp)
	lw	$3,48($sp)
	sll	$2,$3,$2
	nor	$3,$0,$2
	lw	$2,60($sp)
	and	$3,$3,$2
	lw	$4,48($sp)
	lw	$2,56($sp)
	and	$4,$4,$2
	lw	$2,52($sp)
	sll	$2,$4,$2
	or	$2,$3,$2
	sw	$2,60($sp)
	lw	$2,44($sp)
	lw	$4,60($sp)
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	lui	$2,%hi(eponStaticRptEnable)
	lbu	$3,%lo(eponStaticRptEnable)($2)
	li	$2,1			# 0x1
	bne	$3,$2,$L221
	nop

	lui	$2,%hi($LC89)
	addiu	$4,$2,%lo($LC89)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi(g_EPON_MAC_BASE)
	lw	$2,%lo(g_EPON_MAC_BASE)($2)
	addiu	$2,$2,24676
	sw	$2,64($sp)
	li	$2,1			# 0x1
	sw	$2,68($sp)
	li	$2,8			# 0x8
	sw	$2,72($sp)
	li	$2,1			# 0x1
	sw	$2,76($sp)
	lw	$2,64($sp)
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	sw	$2,80($sp)
	lw	$2,72($sp)
	lw	$3,68($sp)
	sll	$2,$3,$2
	nor	$3,$0,$2
	lw	$2,80($sp)
	and	$3,$3,$2
	lw	$4,68($sp)
	lw	$2,76($sp)
	and	$4,$4,$2
	lw	$2,72($sp)
	sll	$2,$4,$2
	or	$2,$3,$2
	sw	$2,80($sp)
	lw	$2,64($sp)
	lw	$4,80($sp)
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	lui	$2,%hi(g_EPON_MAC_BASE)
	lw	$2,%lo(g_EPON_MAC_BASE)($2)
	addiu	$2,$2,24676
	sw	$2,84($sp)
	li	$2,1			# 0x1
	sw	$2,88($sp)
	li	$2,8			# 0x8
	sw	$2,92($sp)
	lw	$2,84($sp)
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	lui	$2,%hi(g_EPON_MAC_BASE)
	lw	$2,%lo(g_EPON_MAC_BASE)($2)
	addiu	$2,$2,24676
	sw	$2,96($sp)
	li	$2,1			# 0x1
	sw	$2,100($sp)
	li	$2,8			# 0x8
	sw	$2,104($sp)
	lw	$2,96($sp)
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	lui	$2,%hi(g_EPON_MAC_BASE)
	lw	$2,%lo(g_EPON_MAC_BASE)($2)
	addiu	$2,$2,24676
	sw	$2,108($sp)
	li	$2,1			# 0x1
	sw	$2,112($sp)
	li	$2,8			# 0x8
	sw	$2,116($sp)
	lw	$2,108($sp)
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	lui	$2,%hi(g_EPON_MAC_BASE)
	lw	$2,%lo(g_EPON_MAC_BASE)($2)
	addiu	$2,$2,24676
	sw	$2,120($sp)
	li	$2,1			# 0x1
	sw	$2,124($sp)
	li	$2,8			# 0x8
	sw	$2,128($sp)
	lw	$2,120($sp)
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	lui	$2,%hi(g_EPON_MAC_BASE)
	lw	$2,%lo(g_EPON_MAC_BASE)($2)
	addiu	$2,$2,24676
	sw	$2,132($sp)
	li	$2,1			# 0x1
	sw	$2,136($sp)
	li	$2,8			# 0x8
	sw	$2,140($sp)
	sw	$0,144($sp)
	lw	$2,132($sp)
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	sw	$2,148($sp)
	lw	$2,140($sp)
	lw	$3,136($sp)
	sll	$2,$3,$2
	nor	$3,$0,$2
	lw	$2,148($sp)
	and	$3,$3,$2
	lw	$4,136($sp)
	lw	$2,144($sp)
	and	$4,$4,$2
	lw	$2,140($sp)
	sll	$2,$4,$2
	or	$2,$3,$2
	sw	$2,148($sp)
	lw	$2,132($sp)
	lw	$4,148($sp)
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	lui	$2,%hi(g_EPON_MAC_BASE)
	lw	$2,%lo(g_EPON_MAC_BASE)($2)
	addiu	$2,$2,24676
	sw	$2,152($sp)
	li	$2,63			# 0x3f
	sw	$2,156($sp)
	sw	$0,160($sp)
	li	$2,1			# 0x1
	sw	$2,164($sp)
	lw	$2,152($sp)
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	sw	$2,168($sp)
	lw	$2,160($sp)
	lw	$3,156($sp)
	sll	$2,$3,$2
	nor	$3,$0,$2
	lw	$2,168($sp)
	and	$3,$3,$2
	lw	$4,156($sp)
	lw	$2,164($sp)
	and	$4,$4,$2
	lw	$2,160($sp)
	sll	$2,$4,$2
	or	$2,$3,$2
	sw	$2,168($sp)
	lw	$2,152($sp)
	lw	$4,168($sp)
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	lui	$2,%hi(g_EPON_MAC_BASE)
	lw	$2,%lo(g_EPON_MAC_BASE)($2)
	addiu	$2,$2,24672
	sw	$2,172($sp)
	li	$2,-1			# 0xffffffffffffffff
	sw	$2,176($sp)
	sw	$0,180($sp)
	li	$2,16842752			# 0x1010000
	ori	$2,$2,0xa000
	sw	$2,184($sp)
	lw	$2,172($sp)
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	sw	$2,188($sp)
	lw	$2,180($sp)
	lw	$3,176($sp)
	sll	$2,$3,$2
	nor	$3,$0,$2
	lw	$2,188($sp)
	and	$3,$3,$2
	lw	$4,176($sp)
	lw	$2,184($sp)
	and	$4,$4,$2
	lw	$2,180($sp)
	sll	$2,$4,$2
	or	$2,$3,$2
	sw	$2,188($sp)
	lw	$2,172($sp)
	lw	$4,188($sp)
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	lui	$2,%hi(g_EPON_MAC_BASE)
	lw	$2,%lo(g_EPON_MAC_BASE)($2)
	addiu	$2,$2,24672
	sw	$2,192($sp)
	li	$2,-1			# 0xffffffffffffffff
	sw	$2,196($sp)
	sw	$0,200($sp)
	sw	$0,204($sp)
	lw	$2,192($sp)
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	sw	$2,208($sp)
	lw	$2,200($sp)
	lw	$3,196($sp)
	sll	$2,$3,$2
	nor	$3,$0,$2
	lw	$2,208($sp)
	and	$3,$3,$2
	lw	$4,196($sp)
	lw	$2,204($sp)
	and	$4,$4,$2
	lw	$2,200($sp)
	sll	$2,$4,$2
	or	$2,$3,$2
	sw	$2,208($sp)
	lw	$2,192($sp)
	lw	$4,208($sp)
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	lui	$2,%hi(g_EPON_MAC_BASE)
	lw	$2,%lo(g_EPON_MAC_BASE)($2)
	addiu	$2,$2,24576
	sw	$2,212($sp)
	li	$2,1			# 0x1
	sw	$2,216($sp)
	li	$2,5			# 0x5
	sw	$2,220($sp)
	li	$2,1			# 0x1
	sw	$2,224($sp)
	lw	$2,212($sp)
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	sw	$2,228($sp)
	lw	$2,220($sp)
	lw	$3,216($sp)
	sll	$2,$3,$2
	nor	$3,$0,$2
	lw	$2,228($sp)
	and	$3,$3,$2
	lw	$4,216($sp)
	lw	$2,224($sp)
	and	$4,$4,$2
	lw	$2,220($sp)
	sll	$2,$4,$2
	or	$2,$3,$2
	sw	$2,228($sp)
	lw	$2,212($sp)
	lw	$4,228($sp)
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	lui	$2,%hi(g_EPON_MAC_BASE)
	lw	$2,%lo(g_EPON_MAC_BASE)($2)
	addiu	$2,$2,24792
	sw	$2,232($sp)
	li	$2,63			# 0x3f
	sw	$2,236($sp)
	sw	$0,240($sp)
	li	$2,8			# 0x8
	sw	$2,244($sp)
	lw	$2,232($sp)
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	sw	$2,248($sp)
	lw	$2,240($sp)
	lw	$3,236($sp)
	sll	$2,$3,$2
	nor	$3,$0,$2
	lw	$2,248($sp)
	and	$3,$3,$2
	lw	$4,236($sp)
	lw	$2,244($sp)
	and	$4,$4,$2
	lw	$2,240($sp)
	sll	$2,$4,$2
	or	$2,$3,$2
	sw	$2,248($sp)
	lw	$2,232($sp)
	lw	$4,248($sp)
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

$L221:
	lui	$2,%hi(g_EPON_MAC_BASE)
	lw	$2,%lo(g_EPON_MAC_BASE)($2)
	addiu	$2,$2,24576
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	sw	$2,20($sp)
	lw	$3,20($sp)
	li	$2,8388608			# 0x800000
	or	$2,$3,$2
	sw	$2,20($sp)
	lui	$2,%hi(g_EPON_MAC_BASE)
	lw	$2,%lo(g_EPON_MAC_BASE)($2)
	addiu	$2,$2,24576
	lw	$4,20($sp)
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	lui	$2,%hi($LC90)
	addiu	$4,$2,%lo($LC90)
	lw	$5,20($sp)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
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
	move	$2,$0
	lw	$31,260($sp)
	lw	$16,256($sp)
	addiu	$sp,$sp,264
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	eponMacSwReset
	.size	eponMacSwReset, .-eponMacSwReset
	.rdata
	.align	2
$LC91:
	.ascii	"enter eponMacStop\000"
	.section	.text.eponMacStop,"ax",@progbits
	.align	2
	.globl	eponMacStop
	.set	nomips16
	.set	nomicromips
	.ent	eponMacStop
	.type	eponMacStop, @function
eponMacStop:
	.frame	$sp,32,$31		# vars= 8, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-32
	sw	$31,28($sp)
	sb	$0,16($sp)
	li	$4,4			# 0x4
	lui	$2,%hi($LC91)
	addiu	$5,$2,%lo($LC91)
	lui	$2,%hi(dbgtoMem)
	addiu	$2,$2,%lo(dbgtoMem)
	jalr	$2
	nop

	lui	$2,%hi(eponMacSwReset)
	addiu	$2,$2,%lo(eponMacSwReset)
	jalr	$2
	nop

	sb	$0,16($sp)
	j	$L228
	nop

$L229:
	lbu	$2,16($sp)
	move	$4,$2
	lui	$2,%hi(xmcs_remove_llid)
	addiu	$2,$2,%lo(xmcs_remove_llid)
	jalr	$2
	nop

	lbu	$2,16($sp)
	addiu	$2,$2,1
	sb	$2,16($sp)
$L228:
	lbu	$2,16($sp)
	sltu	$2,$2,8
	bne	$2,$0,$L229
	nop

	move	$2,$0
	lw	$31,28($sp)
	addiu	$sp,$sp,32
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	eponMacStop
	.size	eponMacStop, .-eponMacStop
	.section	.text.checkEponMacRestart,"ax",@progbits
	.align	2
	.globl	checkEponMacRestart
	.set	nomips16
	.set	nomicromips
	.ent	checkEponMacRestart
	.type	checkEponMacRestart, @function
checkEponMacRestart:
	.frame	$sp,32,$31		# vars= 8, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-32
	sw	$31,28($sp)
	sw	$0,16($sp)
	j	$L232
	nop

$L236:
	lui	$2,%hi(eponMacRestart_flag)
	addiu	$2,$2,%lo(eponMacRestart_flag)
	lw	$2,0($2)
	beq	$2,$0,$L233
	nop

	li	$4,1000			# 0x3e8
	lui	$2,%hi(__udelay)
	addiu	$2,$2,%lo(__udelay)
	jalr	$2
	nop

	j	$L237
	nop

$L233:
	j	$L231
	nop

$L237:
	lw	$2,16($sp)
	addiu	$2,$2,1
	sw	$2,16($sp)
$L232:
	lw	$2,16($sp)
	slt	$2,$2,1001
	bne	$2,$0,$L236
	nop

$L231:
	lw	$31,28($sp)
	addiu	$sp,$sp,32
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	checkEponMacRestart
	.size	checkEponMacRestart, .-checkEponMacRestart
	.rdata
	.align	2
$LC92:
	.ascii	"enter eponMacRestart\000"
	.section	.text.eponMacRestart,"ax",@progbits
	.align	2
	.globl	eponMacRestart
	.set	nomips16
	.set	nomicromips
	.ent	eponMacRestart
	.type	eponMacRestart, @function
eponMacRestart:
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
	lui	$2,%hi($LC92)
	addiu	$5,$2,%lo($LC92)
	lui	$2,%hi(dbgtoMem)
	addiu	$2,$2,%lo(dbgtoMem)
	jalr	$2
	nop

	lui	$2,%hi(eponMacRestart_flag)
	li	$3,1			# 0x1
	sw	$3,%lo(eponMacRestart_flag)($2)
	lui	$2,%hi(eponMacSwReset)
	addiu	$2,$2,%lo(eponMacSwReset)
	jalr	$2
	nop

	sb	$0,16($sp)
	j	$L239
	nop

$L240:
	lbu	$2,16($sp)
	move	$4,$2
	lui	$2,%hi(xmcs_remove_llid)
	addiu	$2,$2,%lo(xmcs_remove_llid)
	jalr	$2
	nop

	lbu	$2,16($sp)
	addiu	$2,$2,1
	sb	$2,16($sp)
$L239:
	lbu	$2,16($sp)
	sltu	$2,$2,8
	bne	$2,$0,$L240
	nop

	lui	$2,%hi(eponInitAllLlid)
	addiu	$2,$2,%lo(eponInitAllLlid)
	jalr	$2
	nop

	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	lw	$3,-30828($2)
	li	$2,1073741824			# 0x40000000
	and	$2,$3,$2
	beq	$2,$0,$L241
	nop

	li	$4,1			# 0x1
	lui	$2,%hi(eponHwDygaspCtrl)
	addiu	$2,$2,%lo(eponHwDygaspCtrl)
	jalr	$2
	nop

	j	$L242
	nop

$L241:
	move	$4,$0
	lui	$2,%hi(eponHwDygaspCtrl)
	addiu	$2,$2,%lo(eponHwDygaspCtrl)
	jalr	$2
	nop

$L242:
	lui	$2,%hi(gpEponPriv)
	lw	$2,%lo(gpEponPriv)($2)
	lbu	$2,36($2)
	beq	$2,$0,$L243
	nop

	li	$4,1			# 0x1
	lui	$2,%hi(eponTxOamFavorModeCtl)
	addiu	$2,$2,%lo(eponTxOamFavorModeCtl)
	jalr	$2
	nop

	j	$L244
	nop

$L243:
	move	$4,$0
	lui	$2,%hi(eponTxOamFavorModeCtl)
	addiu	$2,$2,%lo(eponTxOamFavorModeCtl)
	jalr	$2
	nop

$L244:
	lui	$2,%hi(gpEponPriv)
	lw	$2,%lo(gpEponPriv)($2)
	lbu	$2,37($2)
	sb	$2,17($sp)
	lbu	$2,17($sp)
	move	$4,$2
	lui	$2,%hi(eponSnifferModeCtrl)
	addiu	$2,$2,%lo(eponSnifferModeCtrl)
	jalr	$2
	nop

	lui	$2,%hi(eponMacRestart_flag)
	sw	$0,%lo(eponMacRestart_flag)($2)
	move	$2,$0
	lw	$31,28($sp)
	addiu	$sp,$sp,32
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	eponMacRestart
	.size	eponMacRestart, .-eponMacRestart
	.section	.text.eponMacTxRxEnable,"ax",@progbits
	.align	2
	.globl	eponMacTxRxEnable
	.set	nomips16
	.set	nomicromips
	.ent	eponMacTxRxEnable
	.type	eponMacTxRxEnable, @function
eponMacTxRxEnable:
	.frame	$sp,32,$31		# vars= 8, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-32
	sw	$31,28($sp)
	lui	$2,%hi(g_EPON_MAC_BASE)
	lw	$2,%lo(g_EPON_MAC_BASE)($2)
	addiu	$2,$2,24576
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	sw	$2,16($sp)
	lw	$3,16($sp)
	li	$2,-769			# 0xfffffffffffffcff
	and	$2,$3,$2
	sw	$2,16($sp)
	lui	$2,%hi(g_EPON_MAC_BASE)
	lw	$2,%lo(g_EPON_MAC_BASE)($2)
	addiu	$2,$2,24576
	lw	$4,16($sp)
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
	.end	eponMacTxRxEnable
	.size	eponMacTxRxEnable, .-eponMacTxRxEnable
	.section	.text.eponMacTxRxDisable,"ax",@progbits
	.align	2
	.globl	eponMacTxRxDisable
	.set	nomips16
	.set	nomicromips
	.ent	eponMacTxRxDisable
	.type	eponMacTxRxDisable, @function
eponMacTxRxDisable:
	.frame	$sp,32,$31		# vars= 8, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-32
	sw	$31,28($sp)
	lui	$2,%hi(g_EPON_MAC_BASE)
	lw	$2,%lo(g_EPON_MAC_BASE)($2)
	addiu	$2,$2,24576
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	sw	$2,16($sp)
	lw	$2,16($sp)
	ori	$2,$2,0x300
	sw	$2,16($sp)
	lui	$2,%hi(g_EPON_MAC_BASE)
	lw	$2,%lo(g_EPON_MAC_BASE)($2)
	addiu	$2,$2,24576
	lw	$4,16($sp)
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
	.end	eponMacTxRxDisable
	.size	eponMacTxRxDisable, .-eponMacTxRxDisable
	.section	.text.eponWanStopWithChannelRetire,"ax",@progbits
	.align	2
	.globl	eponWanStopWithChannelRetire
	.set	nomips16
	.set	nomicromips
	.ent	eponWanStopWithChannelRetire
	.type	eponWanStopWithChannelRetire, @function
eponWanStopWithChannelRetire:
	.frame	$sp,280,$31		# vars= 256, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-280
	sw	$31,276($sp)
	sb	$0,17($sp)
	sb	$0,16($sp)
	lui	$2,%hi(epon_reg_lock)
	addiu	$2,$2,%lo(epon_reg_lock)
	sw	$2,24($sp)
	lw	$2,24($sp)
	move	$4,$2
	lui	$2,%hi(_raw_spin_lock_irqsave)
	addiu	$2,$2,%lo(_raw_spin_lock_irqsave)
	jalr	$2
	nop

	sw	$2,20($sp)
	lui	$2,%hi(eponMacTxRxDisable)
	addiu	$2,$2,%lo(eponMacTxRxDisable)
	jalr	$2
	nop

	li	$4,1000			# 0x3e8
	lui	$2,%hi(__udelay)
	addiu	$2,$2,%lo(__udelay)
	jalr	$2
	nop

	lui	$2,%hi(disable_cpu_us_traffic)
	addiu	$2,$2,%lo(disable_cpu_us_traffic)
	jalr	$2
	nop

	sb	$0,16($sp)
	j	$L252
	nop

$L256:
	lbu	$2,16($sp)
	li	$3,1			# 0x1
	sw	$3,28($sp)
	sw	$0,32($sp)
	sb	$2,36($sp)
	sw	$0,40($sp)
	sw	$0,44($sp)
	li	$2,1			# 0x1
	sw	$2,120($sp)
	lw	$2,28($sp)
	sw	$2,124($sp)
	lw	$2,32($sp)
	sw	$2,140($sp)
	lbu	$2,36($sp)
	sw	$2,148($sp)
	lw	$2,40($sp)
	sw	$2,172($sp)
	addiu	$2,$sp,120
	li	$4,18			# 0x12
	move	$5,$0
	move	$6,$2
	lui	$2,%hi(__ECNT_HOOK)
	addiu	$2,$2,%lo(__ECNT_HOOK)
	jalr	$2
	nop

	sw	$2,44($sp)
	lbu	$2,16($sp)
	addiu	$2,$2,1
	sb	$2,16($sp)
$L252:
	lbu	$2,16($sp)
	sltu	$2,$2,32
	bne	$2,$0,$L256
	nop

	sb	$0,16($sp)
	j	$L257
	nop

$L261:
	lbu	$2,16($sp)
	li	$3,1			# 0x1
	sw	$3,48($sp)
	li	$3,1			# 0x1
	sw	$3,52($sp)
	sb	$2,56($sp)
	sw	$0,60($sp)
	sw	$0,64($sp)
	li	$2,1			# 0x1
	sw	$2,120($sp)
	lw	$2,48($sp)
	sw	$2,124($sp)
	lw	$2,52($sp)
	sw	$2,140($sp)
	lbu	$2,56($sp)
	sw	$2,148($sp)
	lw	$2,60($sp)
	sw	$2,172($sp)
	addiu	$2,$sp,120
	li	$4,18			# 0x12
	move	$5,$0
	move	$6,$2
	lui	$2,%hi(__ECNT_HOOK)
	addiu	$2,$2,%lo(__ECNT_HOOK)
	jalr	$2
	nop

	sw	$2,64($sp)
	lbu	$2,16($sp)
	addiu	$2,$2,1
	sb	$2,16($sp)
$L257:
	lbu	$2,16($sp)
	sltu	$2,$2,16
	bne	$2,$0,$L261
	nop

	sb	$0,16($sp)
	j	$L262
	nop

$L266:
	lbu	$2,16($sp)
	li	$3,1			# 0x1
	sw	$3,68($sp)
	sb	$2,72($sp)
	sw	$0,76($sp)
	sw	$0,80($sp)
	li	$2,3			# 0x3
	sw	$2,120($sp)
	lw	$2,68($sp)
	sw	$2,128($sp)
	lbu	$2,72($sp)
	sw	$2,148($sp)
	lw	$2,76($sp)
	sw	$2,172($sp)
	addiu	$2,$sp,120
	li	$4,18			# 0x12
	move	$5,$0
	move	$6,$2
	lui	$2,%hi(__ECNT_HOOK)
	addiu	$2,$2,%lo(__ECNT_HOOK)
	jalr	$2
	nop

	sw	$2,80($sp)
	lbu	$2,16($sp)
	addiu	$2,$2,1
	sb	$2,16($sp)
$L262:
	lbu	$2,16($sp)
	sltu	$2,$2,8
	bne	$2,$0,$L266
	nop

	li	$4,1000			# 0x3e8
	lui	$2,%hi(__udelay)
	addiu	$2,$2,%lo(__udelay)
	jalr	$2
	nop

	li	$2,1			# 0x1
	sw	$2,84($sp)
	li	$2,8			# 0x8
	sb	$2,88($sp)
	sw	$0,92($sp)
	li	$2,46			# 0x2e
	sw	$2,120($sp)
	lw	$2,84($sp)
	sw	$2,124($sp)
	lbu	$2,88($sp)
	sw	$2,148($sp)
	addiu	$2,$sp,120
	li	$4,18			# 0x12
	move	$5,$0
	move	$6,$2
	lui	$2,%hi(__ECNT_HOOK)
	addiu	$2,$2,%lo(__ECNT_HOOK)
	jalr	$2
	nop

	sw	$2,92($sp)
	lui	$2,%hi(eponMacStop)
	addiu	$2,$2,%lo(eponMacStop)
	jalr	$2
	nop

	lui	$2,%hi(eponMacTxRxDisable)
	addiu	$2,$2,%lo(eponMacTxRxDisable)
	jalr	$2
	nop

	lui	$2,%hi(epon_reg_lock)
	addiu	$2,$2,%lo(epon_reg_lock)
	sw	$2,96($sp)
	lw	$2,20($sp)
	sw	$2,100($sp)
	lw	$2,96($sp)
	move	$4,$2
	lw	$5,100($sp)
	lui	$2,%hi(_raw_spin_unlock_irqrestore)
	addiu	$2,$2,%lo(_raw_spin_unlock_irqrestore)
	jalr	$2
	nop

	move	$2,$0
	lw	$31,276($sp)
	addiu	$sp,$sp,280
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	eponWanStopWithChannelRetire
	.size	eponWanStopWithChannelRetire, .-eponWanStopWithChannelRetire
	.globl	g_EponLaserTime
	.section	.bss
	.align	2
	.type	g_EponLaserTime, @object
	.size	g_EponLaserTime, 4
g_EponLaserTime:
	.space	4
	.section	.text.eponSetlaserTime,"ax",@progbits
	.align	2
	.globl	eponSetlaserTime
	.set	nomips16
	.set	nomicromips
	.ent	eponSetlaserTime
	.type	eponSetlaserTime, @function
eponSetlaserTime:
	.frame	$sp,32,$31		# vars= 8, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-32
	sw	$31,28($sp)
	sw	$0,16($sp)
	sw	$0,20($sp)
	lui	$2,%hi(g_EPON_MAC_BASE)
	lw	$2,%lo(g_EPON_MAC_BASE)($2)
	addiu	$2,$2,24796
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	sw	$2,16($sp)
	lw	$2,16($sp)
	andi	$3,$2,0xff
	lui	$2,%hi(g_EponLaserTime)
	lw	$2,%lo(g_EponLaserTime)($2)
	subu	$2,$3,$2
	sw	$2,20($sp)
	lw	$3,16($sp)
	li	$2,-65536			# 0xffffffffffff0000
	ori	$2,$2,0xff
	and	$2,$3,$2
	sw	$2,16($sp)
	lw	$2,20($sp)
	sll	$2,$2,8
	lw	$3,16($sp)
	or	$2,$3,$2
	sw	$2,16($sp)
	lui	$2,%hi(g_EPON_MAC_BASE)
	lw	$2,%lo(g_EPON_MAC_BASE)($2)
	addiu	$2,$2,24796
	lw	$4,16($sp)
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
	.end	eponSetlaserTime
	.size	eponSetlaserTime, .-eponSetlaserTime
	.section	.text.eponWanResetWithChannelRetire,"ax",@progbits
	.align	2
	.globl	eponWanResetWithChannelRetire
	.set	nomips16
	.set	nomicromips
	.ent	eponWanResetWithChannelRetire
	.type	eponWanResetWithChannelRetire, @function
eponWanResetWithChannelRetire:
	.frame	$sp,400,$31		# vars= 376, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-400
	sw	$31,396($sp)
	sb	$0,17($sp)
	sw	$0,20($sp)
	sw	$0,188($sp)
	sb	$0,16($sp)
	lui	$2,%hi(epon_reg_lock)
	addiu	$2,$2,%lo(epon_reg_lock)
	sw	$2,32($sp)
	lw	$2,32($sp)
	move	$4,$2
	lui	$2,%hi(_raw_spin_lock_irqsave)
	addiu	$2,$2,%lo(_raw_spin_lock_irqsave)
	jalr	$2
	nop

	sw	$2,24($sp)
	lui	$2,%hi(eponMacTxRxDisable)
	addiu	$2,$2,%lo(eponMacTxRxDisable)
	jalr	$2
	nop

	sw	$0,188($sp)
	j	$L277
	nop

$L278:
	lw	$2,188($sp)
	addiu	$2,$2,1
	sw	$2,188($sp)
$L277:
	lw	$3,188($sp)
	li	$2,786432			# 0xc0000
	ori	$2,$2,0x3500
	sltu	$2,$3,$2
	bne	$2,$0,$L278
	nop

	sw	$0,188($sp)
	lui	$2,%hi(g_EPON_MAC_BASE)
	lw	$2,%lo(g_EPON_MAC_BASE)($2)
	addiu	$2,$2,24576
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	sw	$2,20($sp)
	lw	$2,20($sp)
	ori	$2,$2,0x10
	sw	$2,20($sp)
	lui	$2,%hi(g_EPON_MAC_BASE)
	lw	$2,%lo(g_EPON_MAC_BASE)($2)
	addiu	$2,$2,24576
	lw	$4,20($sp)
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	j	$L279
	nop

$L280:
	lw	$2,188($sp)
	addiu	$2,$2,1
	sw	$2,188($sp)
$L279:
	lw	$3,188($sp)
	li	$2,786432			# 0xc0000
	ori	$2,$2,0x3500
	sltu	$2,$3,$2
	bne	$2,$0,$L280
	nop

	lw	$3,20($sp)
	li	$2,-17			# 0xffffffffffffffef
	and	$2,$3,$2
	sw	$2,20($sp)
	lui	$2,%hi(g_EPON_MAC_BASE)
	lw	$2,%lo(g_EPON_MAC_BASE)($2)
	addiu	$2,$2,24576
	lw	$4,20($sp)
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	sw	$0,188($sp)
	j	$L281
	nop

$L282:
	lw	$2,188($sp)
	addiu	$2,$2,1
	sw	$2,188($sp)
$L281:
	lw	$3,188($sp)
	li	$2,786432			# 0xc0000
	ori	$2,$2,0x3500
	sltu	$2,$3,$2
	bne	$2,$0,$L282
	nop

	li	$2,-1078984704			# 0xffffffffbfb00000
	ori	$4,$2,0x834
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	sw	$2,28($sp)
	lw	$3,28($sp)
	li	$2,-2147483648			# 0xffffffff80000000
	or	$2,$3,$2
	sw	$2,28($sp)
	lw	$4,28($sp)
	li	$2,-1078984704			# 0xffffffffbfb00000
	ori	$5,$2,0x834
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	j	$L283
	nop

$L284:
	lw	$2,188($sp)
	addiu	$2,$2,1
	sw	$2,188($sp)
$L283:
	lw	$3,188($sp)
	li	$2,786432			# 0xc0000
	ori	$2,$2,0x3500
	sltu	$2,$3,$2
	bne	$2,$0,$L284
	nop

	lw	$2,28($sp)
	ext	$2,$2,0,31
	sw	$2,28($sp)
	lw	$4,28($sp)
	li	$2,-1078984704			# 0xffffffffbfb00000
	ori	$5,$2,0x834
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	sb	$0,16($sp)
	j	$L285
	nop

$L289:
	lbu	$2,16($sp)
	li	$3,1			# 0x1
	sw	$3,36($sp)
	sw	$0,40($sp)
	sb	$2,44($sp)
	sw	$0,48($sp)
	sw	$0,52($sp)
	li	$2,1			# 0x1
	sw	$2,208($sp)
	lw	$2,36($sp)
	sw	$2,212($sp)
	lw	$2,40($sp)
	sw	$2,228($sp)
	lbu	$2,44($sp)
	sw	$2,236($sp)
	lw	$2,48($sp)
	sw	$2,260($sp)
	addiu	$2,$sp,208
	li	$4,18			# 0x12
	move	$5,$0
	move	$6,$2
	lui	$2,%hi(__ECNT_HOOK)
	addiu	$2,$2,%lo(__ECNT_HOOK)
	jalr	$2
	nop

	sw	$2,52($sp)
	lbu	$2,16($sp)
	addiu	$2,$2,1
	sb	$2,16($sp)
$L285:
	lbu	$2,16($sp)
	sltu	$2,$2,32
	bne	$2,$0,$L289
	nop

	sb	$0,16($sp)
	j	$L290
	nop

$L294:
	lbu	$2,16($sp)
	li	$3,1			# 0x1
	sw	$3,56($sp)
	li	$3,1			# 0x1
	sw	$3,60($sp)
	sb	$2,64($sp)
	sw	$0,68($sp)
	sw	$0,72($sp)
	li	$2,1			# 0x1
	sw	$2,208($sp)
	lw	$2,56($sp)
	sw	$2,212($sp)
	lw	$2,60($sp)
	sw	$2,228($sp)
	lbu	$2,64($sp)
	sw	$2,236($sp)
	lw	$2,68($sp)
	sw	$2,260($sp)
	addiu	$2,$sp,208
	li	$4,18			# 0x12
	move	$5,$0
	move	$6,$2
	lui	$2,%hi(__ECNT_HOOK)
	addiu	$2,$2,%lo(__ECNT_HOOK)
	jalr	$2
	nop

	sw	$2,72($sp)
	lbu	$2,16($sp)
	addiu	$2,$2,1
	sb	$2,16($sp)
$L290:
	lbu	$2,16($sp)
	sltu	$2,$2,16
	bne	$2,$0,$L294
	nop

	sb	$0,16($sp)
	j	$L295
	nop

$L299:
	lbu	$2,16($sp)
	li	$3,1			# 0x1
	sw	$3,76($sp)
	sb	$2,80($sp)
	sw	$0,84($sp)
	sw	$0,88($sp)
	li	$2,3			# 0x3
	sw	$2,208($sp)
	lw	$2,76($sp)
	sw	$2,216($sp)
	lbu	$2,80($sp)
	sw	$2,236($sp)
	lw	$2,84($sp)
	sw	$2,260($sp)
	addiu	$2,$sp,208
	li	$4,18			# 0x12
	move	$5,$0
	move	$6,$2
	lui	$2,%hi(__ECNT_HOOK)
	addiu	$2,$2,%lo(__ECNT_HOOK)
	jalr	$2
	nop

	sw	$2,88($sp)
	lbu	$2,16($sp)
	addiu	$2,$2,1
	sb	$2,16($sp)
$L295:
	lbu	$2,16($sp)
	sltu	$2,$2,8
	bne	$2,$0,$L299
	nop

	li	$4,1000			# 0x3e8
	lui	$2,%hi(__udelay)
	addiu	$2,$2,%lo(__udelay)
	jalr	$2
	nop

	li	$2,1			# 0x1
	sw	$2,92($sp)
	li	$2,8			# 0x8
	sb	$2,96($sp)
	sw	$0,100($sp)
	li	$2,46			# 0x2e
	sw	$2,208($sp)
	lw	$2,92($sp)
	sw	$2,212($sp)
	lbu	$2,96($sp)
	sw	$2,236($sp)
	addiu	$2,$sp,208
	li	$4,18			# 0x12
	move	$5,$0
	move	$6,$2
	lui	$2,%hi(__ECNT_HOOK)
	addiu	$2,$2,%lo(__ECNT_HOOK)
	jalr	$2
	nop

	sw	$2,100($sp)
	lui	$2,%hi(eponMacRestart)
	addiu	$2,$2,%lo(eponMacRestart)
	jalr	$2
	nop

	lui	$2,%hi(eponMacTxRxDisable)
	addiu	$2,$2,%lo(eponMacTxRxDisable)
	jalr	$2
	nop

	sb	$0,16($sp)
	j	$L303
	nop

$L313:
	lbu	$2,16($sp)
	li	$3,1			# 0x1
	sw	$3,104($sp)
	sw	$0,108($sp)
	sb	$2,112($sp)
	li	$2,1			# 0x1
	sw	$2,116($sp)
	sw	$0,120($sp)
	li	$2,1			# 0x1
	sw	$2,208($sp)
	lw	$2,104($sp)
	sw	$2,212($sp)
	lw	$2,108($sp)
	sw	$2,228($sp)
	lbu	$2,112($sp)
	sw	$2,236($sp)
	lw	$2,116($sp)
	sw	$2,260($sp)
	addiu	$2,$sp,208
	li	$4,18			# 0x12
	move	$5,$0
	move	$6,$2
	lui	$2,%hi(__ECNT_HOOK)
	addiu	$2,$2,%lo(__ECNT_HOOK)
	jalr	$2
	nop

	sw	$2,120($sp)
	lbu	$2,16($sp)
	li	$3,1			# 0x1
	sw	$3,124($sp)
	li	$3,1			# 0x1
	sw	$3,128($sp)
	sb	$2,132($sp)
	li	$2,1			# 0x1
	sw	$2,136($sp)
	sw	$0,140($sp)
	li	$2,1			# 0x1
	sw	$2,208($sp)
	lw	$2,124($sp)
	sw	$2,212($sp)
	lw	$2,128($sp)
	sw	$2,228($sp)
	lbu	$2,132($sp)
	sw	$2,236($sp)
	lw	$2,136($sp)
	sw	$2,260($sp)
	addiu	$2,$sp,208
	li	$4,18			# 0x12
	move	$5,$0
	move	$6,$2
	lui	$2,%hi(__ECNT_HOOK)
	addiu	$2,$2,%lo(__ECNT_HOOK)
	jalr	$2
	nop

	sw	$2,140($sp)
	lbu	$2,16($sp)
	li	$3,1			# 0x1
	sw	$3,144($sp)
	sb	$2,148($sp)
	li	$2,1			# 0x1
	sw	$2,152($sp)
	sw	$0,156($sp)
	li	$2,3			# 0x3
	sw	$2,208($sp)
	lw	$2,144($sp)
	sw	$2,216($sp)
	lbu	$2,148($sp)
	sw	$2,236($sp)
	lw	$2,152($sp)
	sw	$2,260($sp)
	addiu	$2,$sp,208
	li	$4,18			# 0x12
	move	$5,$0
	move	$6,$2
	lui	$2,%hi(__ECNT_HOOK)
	addiu	$2,$2,%lo(__ECNT_HOOK)
	jalr	$2
	nop

	sw	$2,156($sp)
	lbu	$2,16($sp)
	addiu	$2,$2,1
	sb	$2,16($sp)
$L303:
	lbu	$2,16($sp)
	sltu	$2,$2,8
	bne	$2,$0,$L313
	nop

	lui	$2,%hi(gpEponPriv)
	lw	$2,%lo(gpEponPriv)($2)
	lbu	$2,36($2)
	beq	$2,$0,$L314
	nop

	li	$2,16			# 0x10
	sb	$2,16($sp)
	j	$L315
	nop

$L319:
	lbu	$2,16($sp)
	li	$3,1			# 0x1
	sw	$3,160($sp)
	sw	$0,164($sp)
	sb	$2,168($sp)
	li	$2,1			# 0x1
	sw	$2,172($sp)
	sw	$0,176($sp)
	li	$2,1			# 0x1
	sw	$2,208($sp)
	lw	$2,160($sp)
	sw	$2,212($sp)
	lw	$2,164($sp)
	sw	$2,228($sp)
	lbu	$2,168($sp)
	sw	$2,236($sp)
	lw	$2,172($sp)
	sw	$2,260($sp)
	addiu	$2,$sp,208
	li	$4,18			# 0x12
	move	$5,$0
	move	$6,$2
	lui	$2,%hi(__ECNT_HOOK)
	addiu	$2,$2,%lo(__ECNT_HOOK)
	jalr	$2
	nop

	sw	$2,176($sp)
	lbu	$2,16($sp)
	addiu	$2,$2,1
	sb	$2,16($sp)
$L315:
	lbu	$2,16($sp)
	sltu	$2,$2,24
	bne	$2,$0,$L319
	nop

$L314:
	lui	$2,%hi(eponMacTxRxEnable)
	addiu	$2,$2,%lo(eponMacTxRxEnable)
	jalr	$2
	nop

	lui	$2,%hi(eponSetlaserTime)
	addiu	$2,$2,%lo(eponSetlaserTime)
	jalr	$2
	nop

	lui	$2,%hi(epon_reg_lock)
	addiu	$2,$2,%lo(epon_reg_lock)
	sw	$2,180($sp)
	lw	$2,24($sp)
	sw	$2,184($sp)
	lw	$2,180($sp)
	move	$4,$2
	lw	$5,184($sp)
	lui	$2,%hi(_raw_spin_unlock_irqrestore)
	addiu	$2,$2,%lo(_raw_spin_unlock_irqrestore)
	jalr	$2
	nop

	move	$2,$0
	lw	$31,396($sp)
	addiu	$sp,$sp,400
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	eponWanResetWithChannelRetire
	.size	eponWanResetWithChannelRetire, .-eponWanResetWithChannelRetire
	.section	.text.eponPhyInit,"ax",@progbits
	.align	2
	.globl	eponPhyInit
	.set	nomips16
	.set	nomicromips
	.ent	eponPhyInit
	.type	eponPhyInit, @function
eponPhyInit:
	.frame	$sp,24,$31		# vars= 0, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-24
	sw	$31,20($sp)
	li	$4,2130706432			# 0x7f000000
	li	$5,292			# 0x124
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	li	$2,-847183872			# 0xffffffffcd810000
	ori	$4,$2,0x110
	li	$5,264			# 0x108
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	li	$2,1166082048			# 0x45810000
	ori	$4,$2,0x110
	li	$5,264			# 0x108
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	li	$2,524288			# 0x80000
	ori	$4,$2,0x6100
	li	$5,464			# 0x1d0
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	li	$2,1166082048			# 0x45810000
	ori	$4,$2,0xe130
	li	$5,264			# 0x108
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	li	$2,1073741824			# 0x40000000
	ori	$4,$2,0xc8
	li	$5,496			# 0x1f0
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	li	$4,19			# 0x13
	li	$5,468			# 0x1d4
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	li	$4,69			# 0x45
	li	$5,516			# 0x204
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	lw	$31,20($sp)
	addiu	$sp,$sp,24
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	eponPhyInit
	.size	eponPhyInit, .-eponPhyInit
	.globl	eponOnuMacAddr
	.section	.bss
	.align	2
	.type	eponOnuMacAddr, @object
	.size	eponOnuMacAddr, 6
eponOnuMacAddr:
	.space	6
	.local	rgstIntvalTimer
	.comm	rgstIntvalTimer,28,4
	.rdata
	.align	2
$LC93:
	.ascii	"GDMA2_RX_OVDROPCNT:   newDropCounter = 0x%08lx\012\000"
	.align	2
$LC94:
	.ascii	"GDMA2_RX_OVDROPCNT:   oldDropCounter = 0x%08lx\012\000"
	.align	2
$LC95:
	.ascii	"enter rgstIntvalTimerHandler phy ready is %d\000"
	.align	2
$LC96:
	.ascii	"rgstIntvalTimerHandler start epon: llidIndex=%d state=%d"
	.ascii	" rgstAgainTimeout = %d\000"
	.align	2
$LC97:
	.ascii	"Onu is in silent time out is %d rest, delete 1\000"
	.align	2
$LC98:
	.ascii	"ECNT_HOOK_ERROR occur with cmd_id:0x%x\012\000"
	.align	2
$LC99:
	.ascii	"No such API with type:%d, cmd_id:0x%x\012\000"
	.align	2
$LC100:
	.ascii	"Epon phy up and silent time is 0, begin to response MPCP"
	.ascii	"\000"
	.section	.text.rgstIntvalTimerHandler,"ax",@progbits
	.align	2
	.globl	rgstIntvalTimerHandler
	.set	nomips16
	.set	nomicromips
	.ent	rgstIntvalTimerHandler
	.type	rgstIntvalTimerHandler, @function
rgstIntvalTimerHandler:
	.frame	$sp,216,$31		# vars= 184, regs= 1/0, args= 24, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-216
	sw	$31,212($sp)
	sw	$4,216($sp)
	sb	$0,24($sp)
	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	lw	$3,-30412($2)
	li	$2,1073741824			# 0x40000000
	and	$2,$3,$2
	beq	$2,$0,$L328
	nop

	j	$L329
	nop

$L328:
	sw	$0,28($sp)
	lui	$2,%hi(eponDrv+20)
	addiu	$2,$2,%lo(eponDrv+20)
	sw	$2,32($sp)
	lui	$2,%hi(timer_count.42748)
	lbu	$3,%lo(timer_count.42748)($2)
	lui	$2,%hi(rxDropCheckTimer)
	lbu	$2,%lo(rxDropCheckTimer)($2)
	bne	$3,$2,$L330
	nop

	li	$2,1			# 0x1
	sw	$2,36($sp)
	lui	$2,%hi(newDropCounter.42750)
	addiu	$2,$2,%lo(newDropCounter.42750)
	sw	$2,40($sp)
	sw	$0,44($sp)
	li	$2,20			# 0x14
	sw	$2,80($sp)
	lw	$2,36($sp)
	sw	$2,84($sp)
	addiu	$2,$sp,80
	li	$4,18			# 0x12
	move	$5,$0
	move	$6,$2
	lui	$2,%hi(__ECNT_HOOK)
	addiu	$2,$2,%lo(__ECNT_HOOK)
	jalr	$2
	nop

	sw	$2,44($sp)
	lw	$3,120($sp)
	lw	$2,40($sp)
	sw	$3,0($2)
	lui	$2,%hi(newDropCounter.42750)
	lw	$3,%lo(newDropCounter.42750)($2)
	lui	$2,%hi(oldDropCounter.42749)
	lw	$2,%lo(oldDropCounter.42749)($2)
	subu	$3,$3,$2
	lui	$2,%hi(dropThred)
	lw	$2,%lo(dropThred)($2)
	sltu	$2,$2,$3
	beq	$2,$0,$L334
	nop

	lui	$2,%hi(newDropCounter.42750)
	lw	$3,%lo(newDropCounter.42750)($2)
	lui	$2,%hi($LC93)
	addiu	$4,$2,%lo($LC93)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi(oldDropCounter.42749)
	lw	$3,%lo(oldDropCounter.42749)($2)
	lui	$2,%hi($LC94)
	addiu	$4,$2,%lo($LC94)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi(noRxDscpInt)
	li	$3,1			# 0x1
	sb	$3,%lo(noRxDscpInt)($2)
	j	$L335
	nop

$L334:
	lui	$2,%hi(noRxDscpInt)
	sb	$0,%lo(noRxDscpInt)($2)
$L335:
	lui	$2,%hi(newDropCounter.42750)
	lw	$3,%lo(newDropCounter.42750)($2)
	lui	$2,%hi(oldDropCounter.42749)
	sw	$3,%lo(oldDropCounter.42749)($2)
	lui	$2,%hi(timer_count.42748)
	sb	$0,%lo(timer_count.42748)($2)
	j	$L336
	nop

$L330:
	lui	$2,%hi(timer_count.42748)
	lbu	$2,%lo(timer_count.42748)($2)
	addiu	$2,$2,1
	andi	$3,$2,0x00ff
	lui	$2,%hi(timer_count.42748)
	sb	$3,%lo(timer_count.42748)($2)
$L336:
	lui	$2,%hi(eponReadyFlag)
	lw	$3,%lo(eponReadyFlag)($2)
	li	$4,4			# 0x4
	lui	$2,%hi($LC95)
	addiu	$5,$2,%lo($LC95)
	move	$6,$3
	lui	$2,%hi(dbgtoMem)
	addiu	$2,$2,%lo(dbgtoMem)
	jalr	$2
	nop

	sb	$0,24($sp)
	j	$L337
	nop

$L345:
	lbu	$3,24($sp)
	lw	$2,32($sp)
	lbu	$2,20($2)
	move	$7,$2
	lw	$2,32($sp)
	lhu	$2,68($2)
	sw	$2,16($sp)
	li	$4,4			# 0x4
	lui	$2,%hi($LC96)
	addiu	$5,$2,%lo($LC96)
	move	$6,$3
	lui	$2,%hi(dbgtoMem)
	addiu	$2,$2,%lo(dbgtoMem)
	jalr	$2
	nop

	lw	$2,32($sp)
	lhu	$2,68($2)
	beq	$2,$0,$L338
	nop

	lw	$2,32($sp)
	lhu	$2,68($2)
	move	$3,$2
	li	$4,4			# 0x4
	lui	$2,%hi($LC97)
	addiu	$5,$2,%lo($LC97)
	move	$6,$3
	lui	$2,%hi(dbgtoMem)
	addiu	$2,$2,%lo(dbgtoMem)
	jalr	$2
	nop

	lw	$2,32($sp)
	lhu	$2,68($2)
	addiu	$2,$2,-1
	andi	$3,$2,0xffff
	lw	$2,32($sp)
	sh	$3,68($2)
	li	$2,1			# 0x1
	sw	$2,28($sp)
$L338:
	lw	$2,28($sp)
	bne	$2,$0,$L339
	nop

	li	$2,32793			# 0x8019
	sw	$2,48($sp)
	lw	$2,48($sp)
	sw	$2,52($sp)
	sw	$0,56($sp)
	sw	$0,64($sp)
	sw	$0,68($sp)
	sw	$0,72($sp)
	sw	$0,76($sp)
	sw	$0,64($sp)
	lw	$2,52($sp)
	sw	$2,68($sp)
	lw	$2,56($sp)
	sw	$2,76($sp)
	addiu	$2,$sp,64
	sw	$2,60($sp)
	li	$4,25			# 0x19
	move	$5,$0
	lw	$6,60($sp)
	lui	$2,%hi(__ECNT_HOOK)
	addiu	$2,$2,%lo(__ECNT_HOOK)
	jalr	$2
	nop

	move	$3,$2
	li	$2,-1			# 0xffffffffffffffff
	bne	$3,$2,$L340
	nop

	lw	$2,60($sp)
	lw	$3,4($2)
	lui	$2,%hi($LC98)
	addiu	$4,$2,%lo($LC98)
	move	$5,$3
	lui	$2,%hi(panic)
	addiu	$2,$2,%lo(panic)
	jalr	$2
	nop

$L340:
	lw	$2,60($sp)
	lw	$3,8($2)
	li	$2,-1			# 0xffffffffffffffff
	bne	$3,$2,$L341
	nop

	lw	$2,60($sp)
	lw	$5,0($2)
	lw	$2,60($sp)
	lw	$3,4($2)
	lui	$2,%hi($LC99)
	addiu	$4,$2,%lo($LC99)
	move	$6,$3
	lui	$2,%hi(panic)
	addiu	$2,$2,%lo(panic)
	jalr	$2
	nop

$L341:
	lw	$2,60($sp)
	lw	$3,8($2)
	li	$2,1			# 0x1
	bne	$3,$2,$L339
	nop

	lw	$2,32($sp)
	lbu	$3,20($2)
	li	$2,5			# 0x5
	bne	$3,$2,$L339
	nop

	lui	$2,%hi(eponPhyReadyRealStart)
	addiu	$2,$2,%lo(eponPhyReadyRealStart)
	jalr	$2
	nop

	lw	$2,32($sp)
	li	$3,1			# 0x1
	sb	$3,20($2)
	li	$4,2			# 0x2
	lui	$2,%hi($LC100)
	addiu	$5,$2,%lo($LC100)
	lui	$2,%hi(dbgtoMem)
	addiu	$2,$2,%lo(dbgtoMem)
	jalr	$2
	nop

$L339:
	lbu	$2,24($sp)
	addiu	$2,$2,1
	sb	$2,24($sp)
	lw	$2,32($sp)
	addiu	$2,$2,72
	sw	$2,32($sp)
$L337:
	lbu	$2,24($sp)
	sltu	$2,$2,8
	bne	$2,$0,$L345
	nop

$L329:
	lui	$2,%hi(jiffies)
	lw	$2,%lo(jiffies)($2)
	addiu	$3,$2,100
	lui	$2,%hi(rgstIntvalTimer)
	addiu	$4,$2,%lo(rgstIntvalTimer)
	move	$5,$3
	lui	$2,%hi(mod_timer)
	addiu	$2,$2,%lo(mod_timer)
	jalr	$2
	nop

	lw	$31,212($sp)
	addiu	$sp,$sp,216
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	rgstIntvalTimerHandler
	.size	rgstIntvalTimerHandler, .-rgstIntvalTimerHandler
	.section	.text.eponEnable,"ax",@progbits
	.align	2
	.globl	eponEnable
	.set	nomips16
	.set	nomicromips
	.ent	eponEnable
	.type	eponEnable, @function
eponEnable:
	.frame	$sp,24,$31		# vars= 0, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-24
	sw	$31,20($sp)
	lui	$2,%hi(eponWanResetWithChannelRetire)
	addiu	$2,$2,%lo(eponWanResetWithChannelRetire)
	jalr	$2
	nop

	lui	$2,%hi(eponIrqRegister)
	addiu	$2,$2,%lo(eponIrqRegister)
	jalr	$2
	nop

	lw	$31,20($sp)
	addiu	$sp,$sp,24
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	eponEnable
	.size	eponEnable, .-eponEnable
	.section	.text.eponDisable,"ax",@progbits
	.align	2
	.globl	eponDisable
	.set	nomips16
	.set	nomicromips
	.ent	eponDisable
	.type	eponDisable, @function
eponDisable:
	.frame	$sp,24,$31		# vars= 0, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-24
	sw	$31,20($sp)
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
	lui	$2,%hi(eponIrqUnregister)
	addiu	$2,$2,%lo(eponIrqUnregister)
	jalr	$2
	nop

	lui	$2,%hi(eponWanStopWithChannelRetire)
	addiu	$2,$2,%lo(eponWanStopWithChannelRetire)
	jalr	$2
	nop

	lw	$31,20($sp)
	addiu	$sp,$sp,24
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	eponDisable
	.size	eponDisable, .-eponDisable
	.section	.text.eponFirmwareSetMode,"ax",@progbits
	.align	2
	.globl	eponFirmwareSetMode
	.set	nomips16
	.set	nomicromips
	.ent	eponFirmwareSetMode
	.type	eponFirmwareSetMode, @function
eponFirmwareSetMode:
	.frame	$sp,168,$31		# vars= 168, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-168
	sw	$4,168($sp)
	li	$2,-1078984704			# 0xffffffffbfb00000
	ori	$2,$2,0x64
	sw	$2,8($sp)
	lw	$2,8($sp)
	lw	$2,0($2)
	andi	$3,$2,0xf
	li	$2,3			# 0x3
	bne	$3,$2,$L350
	nop

	li	$2,-1078657024			# 0xffffffffbfb50000
	ori	$2,$2,0xfffc
	sw	$2,12($sp)
	lw	$2,12($sp)
	lw	$2,0($2)
	andi	$2,$2,0xf
	beq	$2,$0,$L350
	nop

	li	$2,-1078984704			# 0xffffffffbfb00000
	ori	$2,$2,0xf8
	sw	$2,16($sp)
	lw	$2,16($sp)
	lw	$2,0($2)
	andi	$3,$2,0x3
	li	$2,2			# 0x2
	bne	$3,$2,$L353
	nop

	li	$2,-1078984704			# 0xffffffffbfb00000
	ori	$2,$2,0x64
	sw	$2,20($sp)
	lw	$2,20($sp)
	lw	$3,0($2)
	li	$2,-65536			# 0xffffffffffff0000
	and	$3,$3,$2
	li	$2,327680			# 0x50000
	bne	$3,$2,$L353
	nop

	li	$2,-1078001664			# 0xffffffffbfbf0000
	ori	$2,$2,0x8214
	sw	$2,24($sp)
	lw	$2,24($sp)
	lw	$2,0($2)
	andi	$2,$2,0x8
	beq	$2,$0,$L356
	nop

	li	$2,-1078001664			# 0xffffffffbfbf0000
	ori	$2,$2,0x8214
	sw	$2,28($sp)
	lw	$2,28($sp)
	lw	$2,0($2)
	andi	$2,$2,0x20
	bne	$2,$0,$L358
	nop

	j	$L353
	nop

$L356:
	li	$2,-1078001664			# 0xffffffffbfbf0000
	ori	$2,$2,0x8214
	sw	$2,32($sp)
	lw	$2,32($sp)
	lw	$2,0($2)
	andi	$2,$2,0x2
	beq	$2,$0,$L353
	nop

$L358:
	li	$2,-1078001664			# 0xffffffffbfbf0000
	ori	$2,$2,0x8214
	sw	$2,36($sp)
	lw	$2,36($sp)
	lw	$2,0($2)
	andi	$2,$2,0x8
	beq	$2,$0,$L361
	nop

	li	$2,-1078001664			# 0xffffffffbfbf0000
	ori	$2,$2,0x8214
	sw	$2,40($sp)
	lw	$2,40($sp)
	lw	$2,0($2)
	andi	$2,$2,0x40
	beq	$2,$0,$L363
	nop

	j	$L353
	nop

$L361:
	li	$2,-1078001664			# 0xffffffffbfbf0000
	ori	$2,$2,0x8214
	sw	$2,44($sp)
	lw	$2,44($sp)
	lw	$2,0($2)
	andi	$2,$2,0x4
	beq	$2,$0,$L363
	nop

$L353:
	li	$2,-1078984704			# 0xffffffffbfb00000
	ori	$2,$2,0xf8
	sw	$2,48($sp)
	lw	$2,48($sp)
	lw	$2,0($2)
	andi	$3,$2,0x3
	li	$2,3			# 0x3
	bne	$3,$2,$L366
	nop

	li	$2,-1078984704			# 0xffffffffbfb00000
	ori	$2,$2,0x64
	sw	$2,52($sp)
	lw	$2,52($sp)
	lw	$3,0($2)
	li	$2,-65536			# 0xffffffffffff0000
	and	$3,$3,$2
	li	$2,327680			# 0x50000
	bne	$3,$2,$L366
	nop

	li	$2,-1078001664			# 0xffffffffbfbf0000
	ori	$2,$2,0x8214
	sw	$2,56($sp)
	lw	$2,56($sp)
	lw	$2,0($2)
	andi	$2,$2,0x8
	beq	$2,$0,$L369
	nop

	li	$2,-1078001664			# 0xffffffffbfbf0000
	ori	$2,$2,0x8214
	sw	$2,60($sp)
	lw	$2,60($sp)
	lw	$2,0($2)
	andi	$2,$2,0x20
	bne	$2,$0,$L363
	nop

	j	$L366
	nop

$L369:
	li	$2,-1078001664			# 0xffffffffbfbf0000
	ori	$2,$2,0x8214
	sw	$2,64($sp)
	lw	$2,64($sp)
	lw	$2,0($2)
	andi	$2,$2,0x2
	bne	$2,$0,$L363
	nop

$L366:
	li	$2,-1078984704			# 0xffffffffbfb00000
	ori	$2,$2,0xf8
	sw	$2,68($sp)
	lw	$2,68($sp)
	lw	$2,0($2)
	andi	$3,$2,0x3
	li	$2,2			# 0x2
	bne	$3,$2,$L373
	nop

	li	$2,-1078984704			# 0xffffffffbfb00000
	ori	$2,$2,0x64
	sw	$2,72($sp)
	lw	$2,72($sp)
	lw	$3,0($2)
	li	$2,-65536			# 0xffffffffffff0000
	and	$3,$3,$2
	li	$2,327680			# 0x50000
	bne	$3,$2,$L373
	nop

	li	$2,-1078001664			# 0xffffffffbfbf0000
	ori	$2,$2,0x8214
	sw	$2,76($sp)
	lw	$2,76($sp)
	lw	$2,0($2)
	andi	$2,$2,0x8
	beq	$2,$0,$L376
	nop

	li	$2,-1078001664			# 0xffffffffbfbf0000
	ori	$2,$2,0x8214
	sw	$2,80($sp)
	lw	$2,80($sp)
	lw	$2,0($2)
	andi	$2,$2,0x20
	beq	$2,$0,$L378
	nop

	j	$L373
	nop

$L376:
	li	$2,-1078001664			# 0xffffffffbfbf0000
	ori	$2,$2,0x8214
	sw	$2,84($sp)
	lw	$2,84($sp)
	lw	$2,0($2)
	andi	$2,$2,0x2
	bne	$2,$0,$L373
	nop

$L378:
	li	$2,-1078001664			# 0xffffffffbfbf0000
	ori	$2,$2,0x8214
	sw	$2,88($sp)
	lw	$2,88($sp)
	lw	$2,0($2)
	andi	$2,$2,0x8
	beq	$2,$0,$L381
	nop

	li	$2,-1078001664			# 0xffffffffbfbf0000
	ori	$2,$2,0x8214
	sw	$2,92($sp)
	lw	$2,92($sp)
	lw	$2,0($2)
	andi	$2,$2,0x40
	beq	$2,$0,$L363
	nop

	j	$L373
	nop

$L381:
	li	$2,-1078001664			# 0xffffffffbfbf0000
	ori	$2,$2,0x8214
	sw	$2,96($sp)
	lw	$2,96($sp)
	lw	$2,0($2)
	andi	$2,$2,0x4
	beq	$2,$0,$L363
	nop

$L373:
	li	$2,-1078984704			# 0xffffffffbfb00000
	ori	$2,$2,0xf8
	sw	$2,100($sp)
	lw	$2,100($sp)
	lw	$2,0($2)
	andi	$3,$2,0x3
	li	$2,3			# 0x3
	bne	$3,$2,$L385
	nop

	li	$2,-1078984704			# 0xffffffffbfb00000
	ori	$2,$2,0x64
	sw	$2,104($sp)
	lw	$2,104($sp)
	lw	$3,0($2)
	li	$2,-65536			# 0xffffffffffff0000
	and	$3,$3,$2
	li	$2,327680			# 0x50000
	bne	$3,$2,$L385
	nop

	li	$2,-1078001664			# 0xffffffffbfbf0000
	ori	$2,$2,0x8214
	sw	$2,108($sp)
	lw	$2,108($sp)
	lw	$2,0($2)
	andi	$2,$2,0x8
	beq	$2,$0,$L388
	nop

	li	$2,-1078001664			# 0xffffffffbfbf0000
	ori	$2,$2,0x8214
	sw	$2,112($sp)
	lw	$2,112($sp)
	lw	$2,0($2)
	andi	$2,$2,0x20
	beq	$2,$0,$L363
	nop

	j	$L385
	nop

$L388:
	li	$2,-1078001664			# 0xffffffffbfbf0000
	ori	$2,$2,0x8214
	sw	$2,116($sp)
	lw	$2,116($sp)
	lw	$2,0($2)
	andi	$2,$2,0x2
	beq	$2,$0,$L363
	nop

$L385:
	li	$2,-1078984704			# 0xffffffffbfb00000
	ori	$2,$2,0xf8
	sw	$2,120($sp)
	lw	$2,120($sp)
	lw	$2,0($2)
	andi	$3,$2,0x3
	li	$2,2			# 0x2
	bne	$3,$2,$L350
	nop

	li	$2,-1078984704			# 0xffffffffbfb00000
	ori	$2,$2,0x64
	sw	$2,124($sp)
	lw	$2,124($sp)
	lw	$3,0($2)
	li	$2,-65536			# 0xffffffffffff0000
	and	$3,$3,$2
	li	$2,327680			# 0x50000
	bne	$3,$2,$L350
	nop

	li	$2,-1078001664			# 0xffffffffbfbf0000
	ori	$2,$2,0x8214
	sw	$2,128($sp)
	lw	$2,128($sp)
	lw	$2,0($2)
	andi	$2,$2,0x8
	beq	$2,$0,$L394
	nop

	li	$2,-1078001664			# 0xffffffffbfbf0000
	ori	$2,$2,0x8214
	sw	$2,132($sp)
	lw	$2,132($sp)
	lw	$2,0($2)
	andi	$2,$2,0x40
	bne	$2,$0,$L363
	nop

	j	$L348
	nop

$L394:
	li	$2,-1078001664			# 0xffffffffbfbf0000
	ori	$2,$2,0x8214
	sw	$2,136($sp)
	lw	$2,136($sp)
	lw	$2,0($2)
	andi	$2,$2,0x4
	beq	$2,$0,$L350
	nop

$L363:
	lw	$2,168($sp)
	bne	$2,$0,$L397
	nop

	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$2,$2,0x124
	sw	$2,140($sp)
	lw	$2,140($sp)
	lw	$2,0($2)
	sw	$2,0($sp)
	lw	$3,0($sp)
	li	$2,-67174400			# 0xfffffffffbff0000
	ori	$2,$2,0xffff
	and	$2,$3,$2
	li	$3,-1079050240			# 0xffffffffbfaf0000
	ori	$3,$3,0x124
	sw	$3,144($sp)
	sw	$2,148($sp)
	lw	$2,144($sp)
	lw	$3,148($sp)
	sw	$3,0($2)
	j	$L350
	nop

$L397:
	lw	$3,168($sp)
	li	$2,1			# 0x1
	bne	$3,$2,$L350
	nop

	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$2,$2,0x124
	sw	$2,152($sp)
	lw	$2,152($sp)
	lw	$2,0($2)
	sw	$2,4($sp)
	lw	$3,4($sp)
	li	$2,67108864			# 0x4000000
	or	$2,$3,$2
	li	$3,-1079050240			# 0xffffffffbfaf0000
	ori	$3,$3,0x124
	sw	$3,156($sp)
	sw	$2,160($sp)
	lw	$2,156($sp)
	lw	$3,160($sp)
	sw	$3,0($2)
$L350:
	nop
$L348:
	addiu	$sp,$sp,168
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	eponFirmwareSetMode
	.size	eponFirmwareSetMode, .-eponFirmwareSetMode
	.rdata
	.align	2
$LC101:
	.ascii	"eponStart\000"
	.section	.text.eponStart,"ax",@progbits
	.align	2
	.globl	eponStart
	.set	nomips16
	.set	nomicromips
	.ent	eponStart
	.type	eponStart, @function
eponStart:
	.frame	$sp,32,$31		# vars= 8, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-32
	sw	$31,28($sp)
	sw	$4,32($sp)
	li	$4,4			# 0x4
	lui	$2,%hi($LC101)
	addiu	$5,$2,%lo($LC101)
	lui	$2,%hi(dbgtoMem)
	addiu	$2,$2,%lo(dbgtoMem)
	jalr	$2
	nop

	li	$2,-1078984704			# 0xffffffffbfb00000
	ori	$2,$2,0x70
	sw	$2,16($sp)
	li	$2,1			# 0x1
	sw	$2,20($sp)
	lw	$2,16($sp)
	lw	$3,20($sp)
	sw	$3,0($2)
	move	$4,$0
	lui	$2,%hi(eponFirmwareSetMode)
	addiu	$2,$2,%lo(eponFirmwareSetMode)
	jalr	$2
	nop

	lui	$2,%hi(eponInitParam)
	addiu	$2,$2,%lo(eponInitParam)
	jalr	$2
	nop

	lui	$2,%hi(eponInitAllLlid)
	addiu	$2,$2,%lo(eponInitAllLlid)
	jalr	$2
	nop

	lui	$2,%hi(eponEnable)
	addiu	$2,$2,%lo(eponEnable)
	jalr	$2
	nop

	nop
	lw	$31,28($sp)
	addiu	$sp,$sp,32
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	eponStart
	.size	eponStart, .-eponStart
	.rdata
	.align	2
$LC102:
	.ascii	"ERR: eponSetLlidQueThrod error\000"
	.section	.text.eponCleanAllLlidQueThrod,"ax",@progbits
	.align	2
	.globl	eponCleanAllLlidQueThrod
	.set	nomips16
	.set	nomicromips
	.ent	eponCleanAllLlidQueThrod
	.type	eponCleanAllLlidQueThrod, @function
eponCleanAllLlidQueThrod:
	.frame	$sp,40,$31		# vars= 16, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-40
	sw	$31,36($sp)
	sb	$0,16($sp)
	sb	$0,17($sp)
	sb	$0,18($sp)
	sw	$0,20($sp)
	sb	$0,17($sp)
	j	$L404
	nop

$L411:
	sb	$0,18($sp)
	j	$L405
	nop

$L410:
	sb	$0,16($sp)
	j	$L406
	nop

$L409:
	lbu	$2,17($sp)
	sb	$2,24($sp)
	lbu	$2,16($sp)
	sb	$2,25($sp)
	lbu	$2,18($sp)
	sb	$2,26($sp)
	sh	$0,28($sp)
	addiu	$2,$sp,24
	move	$4,$2
	lui	$2,%hi(epon_set_queue_threshold)
	addiu	$2,$2,%lo(epon_set_queue_threshold)
	jalr	$2
	nop

	sw	$2,20($sp)
	lw	$2,20($sp)
	bgez	$2,$L407
	nop

	li	$4,1			# 0x1
	lui	$2,%hi($LC102)
	addiu	$5,$2,%lo($LC102)
	lui	$2,%hi(dbgtoMem)
	addiu	$2,$2,%lo(dbgtoMem)
	jalr	$2
	nop

	li	$2,-1			# 0xffffffffffffffff
	j	$L412
	nop

$L407:
	lbu	$2,16($sp)
	addiu	$2,$2,1
	sb	$2,16($sp)
$L406:
	lbu	$2,16($sp)
	sltu	$2,$2,8
	bne	$2,$0,$L409
	nop

	lbu	$2,18($sp)
	addiu	$2,$2,1
	sb	$2,18($sp)
$L405:
	lbu	$2,18($sp)
	sltu	$2,$2,3
	bne	$2,$0,$L410
	nop

	lbu	$2,17($sp)
	addiu	$2,$2,1
	sb	$2,17($sp)
$L404:
	lbu	$2,17($sp)
	sltu	$2,$2,8
	bne	$2,$0,$L411
	nop

	move	$2,$0
$L412:
	lw	$31,36($sp)
	addiu	$sp,$sp,40
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	eponCleanAllLlidQueThrod
	.size	eponCleanAllLlidQueThrod, .-eponCleanAllLlidQueThrod
	.rdata
	.align	2
$LC103:
	.ascii	"eponDumpAllLlidQueThrod enter\012\000"
	.align	2
$LC104:
	.ascii	"\012 ch%d,set%d : \000"
	.align	2
$LC105:
	.ascii	"ERR: eponGetLlidQueThrod error\000"
	.align	2
$LC106:
	.ascii	"%x \000"
	.align	2
$LC107:
	.ascii	"\012\000"
	.section	.text.eponDumpAllLlidQueThrod,"ax",@progbits
	.align	2
	.globl	eponDumpAllLlidQueThrod
	.set	nomips16
	.set	nomicromips
	.ent	eponDumpAllLlidQueThrod
	.type	eponDumpAllLlidQueThrod, @function
eponDumpAllLlidQueThrod:
	.frame	$sp,40,$31		# vars= 16, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-40
	sw	$31,36($sp)
	sb	$0,16($sp)
	sw	$0,20($sp)
	sb	$0,17($sp)
	sb	$0,18($sp)
	lui	$2,%hi($LC103)
	addiu	$4,$2,%lo($LC103)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	sb	$0,17($sp)
	j	$L414
	nop

$L421:
	sb	$0,18($sp)
	j	$L415
	nop

$L420:
	lbu	$5,17($sp)
	lbu	$3,18($sp)
	lui	$2,%hi($LC104)
	addiu	$4,$2,%lo($LC104)
	move	$6,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	sb	$0,16($sp)
	j	$L416
	nop

$L419:
	addiu	$2,$sp,24
	move	$4,$2
	move	$5,$0
	li	$6,6			# 0x6
	lui	$2,%hi(memset)
	addiu	$2,$2,%lo(memset)
	jalr	$2
	nop

	lbu	$2,17($sp)
	sb	$2,24($sp)
	lbu	$2,16($sp)
	sb	$2,25($sp)
	lbu	$2,18($sp)
	sb	$2,26($sp)
	addiu	$2,$sp,24
	move	$4,$2
	lui	$2,%hi(epon_get_queue_threshold)
	addiu	$2,$2,%lo(epon_get_queue_threshold)
	jalr	$2
	nop

	sw	$2,20($sp)
	lw	$2,20($sp)
	bgez	$2,$L417
	nop

	li	$4,1			# 0x1
	lui	$2,%hi($LC105)
	addiu	$5,$2,%lo($LC105)
	lui	$2,%hi(dbgtoMem)
	addiu	$2,$2,%lo(dbgtoMem)
	jalr	$2
	nop

	li	$2,-1			# 0xffffffffffffffff
	j	$L422
	nop

$L417:
	lhu	$2,28($sp)
	move	$3,$2
	lui	$2,%hi($LC106)
	addiu	$4,$2,%lo($LC106)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lbu	$2,16($sp)
	addiu	$2,$2,1
	sb	$2,16($sp)
$L416:
	lbu	$2,16($sp)
	sltu	$2,$2,8
	bne	$2,$0,$L419
	nop

	lbu	$2,18($sp)
	addiu	$2,$2,1
	sb	$2,18($sp)
$L415:
	lbu	$2,18($sp)
	sltu	$2,$2,3
	bne	$2,$0,$L420
	nop

	lui	$2,%hi($LC107)
	addiu	$4,$2,%lo($LC107)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lbu	$2,17($sp)
	addiu	$2,$2,1
	sb	$2,17($sp)
$L414:
	lbu	$2,17($sp)
	sltu	$2,$2,8
	bne	$2,$0,$L421
	nop

	move	$2,$0
$L422:
	lw	$31,36($sp)
	addiu	$sp,$sp,40
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	eponDumpAllLlidQueThrod
	.size	eponDumpAllLlidQueThrod, .-eponDumpAllLlidQueThrod
	.rdata
	.align	2
$LC108:
	.ascii	"Timeout for set EPON Threshold configuration.\012\000"
	.section	.text.epon_set_queue_threshold,"ax",@progbits
	.align	2
	.globl	epon_set_queue_threshold
	.set	nomips16
	.set	nomicromips
	.ent	epon_set_queue_threshold
	.type	epon_set_queue_threshold, @function
epon_set_queue_threshold:
	.frame	$sp,32,$31		# vars= 8, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-32
	sw	$31,28($sp)
	sw	$4,32($sp)
	sw	$0,20($sp)
	li	$2,3			# 0x3
	sw	$2,16($sp)
	lw	$2,32($sp)
	lbu	$2,0($2)
	sltu	$2,$2,8
	beq	$2,$0,$L424
	nop

	lw	$2,32($sp)
	lbu	$2,1($2)
	sltu	$2,$2,8
	beq	$2,$0,$L424
	nop

	lw	$2,32($sp)
	lbu	$2,2($2)
	sltu	$2,$2,3
	bne	$2,$0,$L425
	nop

$L424:
	li	$2,-1			# 0xffffffffffffffff
	j	$L426
	nop

$L425:
	lw	$2,32($sp)
	lhu	$2,4($2)
	sll	$3,$2,8
	li	$2,16711680			# 0xff0000
	ori	$2,$2,0xff00
	and	$3,$3,$2
	li	$2,-2147483648			# 0xffffffff80000000
	or	$3,$3,$2
	lw	$2,32($sp)
	lbu	$2,2($2)
	sll	$2,$2,6
	andi	$2,$2,0xff
	or	$3,$3,$2
	lw	$2,32($sp)
	lbu	$2,0($2)
	sll	$2,$2,3
	andi	$2,$2,0x38
	or	$3,$3,$2
	lw	$2,32($sp)
	lbu	$2,1($2)
	andi	$2,$2,0x7
	or	$2,$3,$2
	sw	$2,20($sp)
	lui	$2,%hi(g_EPON_MAC_BASE)
	lw	$2,%lo(g_EPON_MAC_BASE)($2)
	addiu	$2,$2,24684
	lw	$4,20($sp)
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	j	$L427
	nop

$L429:
	lui	$2,%hi(g_EPON_MAC_BASE)
	lw	$2,%lo(g_EPON_MAC_BASE)($2)
	addiu	$2,$2,24684
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	sw	$2,20($sp)
	lw	$3,20($sp)
	li	$2,1073741824			# 0x40000000
	and	$2,$3,$2
	beq	$2,$0,$L428
	nop

	move	$2,$0
	j	$L426
	nop

$L428:
	li	$4,1000			# 0x3e8
	lui	$2,%hi(__udelay)
	addiu	$2,$2,%lo(__udelay)
	jalr	$2
	nop

$L427:
	lw	$2,16($sp)
	addiu	$3,$2,-1
	sw	$3,16($sp)
	bne	$2,$0,$L429
	nop

	lui	$2,%hi($LC108)
	addiu	$4,$2,%lo($LC108)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	li	$2,-1			# 0xffffffffffffffff
$L426:
	lw	$31,28($sp)
	addiu	$sp,$sp,32
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	epon_set_queue_threshold
	.size	epon_set_queue_threshold, .-epon_set_queue_threshold
	.rdata
	.align	2
$LC109:
	.ascii	"Timeout for get EPON Threshold configuration.\012\000"
	.section	.text.epon_get_queue_threshold,"ax",@progbits
	.align	2
	.globl	epon_get_queue_threshold
	.set	nomips16
	.set	nomicromips
	.ent	epon_get_queue_threshold
	.type	epon_get_queue_threshold, @function
epon_get_queue_threshold:
	.frame	$sp,40,$31		# vars= 16, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-40
	sw	$31,36($sp)
	sw	$4,40($sp)
	sw	$0,20($sp)
	li	$2,3			# 0x3
	sw	$2,16($sp)
	lw	$2,40($sp)
	lbu	$2,0($2)
	sltu	$2,$2,8
	beq	$2,$0,$L431
	nop

	lw	$2,40($sp)
	lbu	$2,1($2)
	sltu	$2,$2,8
	beq	$2,$0,$L431
	nop

	lw	$2,40($sp)
	lbu	$2,2($2)
	sltu	$2,$2,3
	bne	$2,$0,$L432
	nop

$L431:
	li	$2,-1			# 0xffffffffffffffff
	j	$L433
	nop

$L432:
	lw	$2,40($sp)
	lbu	$2,2($2)
	sll	$2,$2,6
	andi	$3,$2,0xff
	lw	$2,40($sp)
	lbu	$2,0($2)
	sll	$2,$2,3
	andi	$2,$2,0x38
	or	$3,$3,$2
	lw	$2,40($sp)
	lbu	$2,1($2)
	andi	$2,$2,0x7
	or	$2,$3,$2
	sw	$2,20($sp)
	lui	$2,%hi(g_EPON_MAC_BASE)
	lw	$2,%lo(g_EPON_MAC_BASE)($2)
	addiu	$2,$2,24684
	lw	$4,20($sp)
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	j	$L434
	nop

$L437:
	lui	$2,%hi(g_EPON_MAC_BASE)
	lw	$2,%lo(g_EPON_MAC_BASE)($2)
	addiu	$2,$2,24684
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	sw	$2,20($sp)
	lw	$3,20($sp)
	li	$2,1073741824			# 0x40000000
	and	$2,$3,$2
	beq	$2,$0,$L435
	nop

	lw	$3,20($sp)
	li	$2,16711680			# 0xff0000
	ori	$2,$2,0xff00
	and	$2,$3,$2
	srl	$2,$2,8
	sw	$2,24($sp)
	lw	$2,24($sp)
	bltz	$2,$L436
	nop

	lw	$2,24($sp)
	andi	$3,$2,0xffff
	lw	$2,40($sp)
	sh	$3,4($2)
	move	$2,$0
	j	$L433
	nop

$L436:
	lw	$2,24($sp)
	j	$L433
	nop

$L435:
	li	$4,1000			# 0x3e8
	lui	$2,%hi(__udelay)
	addiu	$2,$2,%lo(__udelay)
	jalr	$2
	nop

$L434:
	lw	$2,16($sp)
	addiu	$3,$2,-1
	sw	$3,16($sp)
	bne	$2,$0,$L437
	nop

	lui	$2,%hi($LC109)
	addiu	$4,$2,%lo($LC109)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	li	$2,-1			# 0xffffffffffffffff
$L433:
	lw	$31,36($sp)
	addiu	$sp,$sp,40
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	epon_get_queue_threshold
	.size	epon_get_queue_threshold, .-epon_get_queue_threshold
	.rdata
	.align	2
$LC110:
	.ascii	"eponStop\000"
	.section	.text.eponStop,"ax",@progbits
	.align	2
	.globl	eponStop
	.set	nomips16
	.set	nomicromips
	.ent	eponStop
	.type	eponStop, @function
eponStop:
	.frame	$sp,56,$31		# vars= 32, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-56
	sw	$31,52($sp)
	li	$4,4			# 0x4
	lui	$2,%hi($LC110)
	addiu	$5,$2,%lo($LC110)
	lui	$2,%hi(dbgtoMem)
	addiu	$2,$2,%lo(dbgtoMem)
	jalr	$2
	nop

	lui	$2,%hi(eponCleanAllLlidQueThrod)
	addiu	$2,$2,%lo(eponCleanAllLlidQueThrod)
	jalr	$2
	nop

	lui	$2,%hi(eponDisable)
	addiu	$2,$2,%lo(eponDisable)
	jalr	$2
	nop

	li	$2,31			# 0x1f
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
	bne	$3,$2,$L439
	nop

	lw	$2,24($sp)
	lw	$3,4($2)
	lui	$2,%hi($LC98)
	addiu	$4,$2,%lo($LC98)
	move	$5,$3
	lui	$2,%hi(panic)
	addiu	$2,$2,%lo(panic)
	jalr	$2
	nop

$L439:
	lw	$2,24($sp)
	lw	$3,8($2)
	li	$2,-1			# 0xffffffffffffffff
	bne	$3,$2,$L440
	nop

	lw	$2,24($sp)
	lw	$5,0($2)
	lw	$2,24($sp)
	lw	$3,4($2)
	lui	$2,%hi($LC99)
	addiu	$4,$2,%lo($LC99)
	move	$6,$3
	lui	$2,%hi(panic)
	addiu	$2,$2,%lo(panic)
	jalr	$2
	nop

$L440:
	nop
	lw	$31,52($sp)
	addiu	$sp,$sp,56
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	eponStop
	.size	eponStop, .-eponStop
	.globl	eponMacFops
	.data
	.align	2
	.type	eponMacFops, @object
	.size	eponMacFops, 100
eponMacFops:
	.word	__this_module
	.space	4
	.word	0
	.word	0
	.space	16
	.word	eponMacIoctl
	.space	8
	.word	eponMacOpen
	.space	4
	.word	0
	.space	44
	.rdata
	.align	2
$LC111:
	.ascii	"eponMacOpen entered\000"
	.section	.text.eponMacOpen,"ax",@progbits
	.align	2
	.globl	eponMacOpen
	.set	nomips16
	.set	nomicromips
	.ent	eponMacOpen
	.type	eponMacOpen, @function
eponMacOpen:
	.frame	$sp,24,$31		# vars= 0, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-24
	sw	$31,20($sp)
	sw	$4,24($sp)
	sw	$5,28($sp)
	li	$4,4			# 0x4
	lui	$2,%hi($LC111)
	addiu	$5,$2,%lo($LC111)
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
	.end	eponMacOpen
	.size	eponMacOpen, .-eponMacOpen
	.rdata
	.align	2
$LC112:
	.ascii	"enter eponCmdInit\000"
	.align	2
$LC113:
	.ascii	"epon\000"
	.align	2
$LC114:
	.ascii	"ERR: eponCmdInit ret=%d \012\000"
	.section	.text.eponCmdInit,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	eponCmdInit
	.type	eponCmdInit, @function
eponCmdInit:
	.frame	$sp,48,$31		# vars= 24, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-48
	sw	$31,44($sp)
	li	$2,-1			# 0xffffffffffffffff
	sw	$2,16($sp)
	addiu	$2,$sp,20
	move	$4,$2
	move	$5,$0
	li	$6,20			# 0x14
	lui	$2,%hi(memset)
	addiu	$2,$2,%lo(memset)
	jalr	$2
	nop

	li	$4,4			# 0x4
	lui	$2,%hi($LC112)
	addiu	$5,$2,%lo($LC112)
	lui	$2,%hi(dbgtoMem)
	addiu	$2,$2,%lo(dbgtoMem)
	jalr	$2
	nop

	lui	$2,%hi($LC113)
	addiu	$2,$2,%lo($LC113)
	sw	$2,20($sp)
	lui	$2,%hi(doEpon)
	addiu	$2,$2,%lo(doEpon)
	sw	$2,24($sp)
	li	$2,18			# 0x12
	sw	$2,28($sp)
	sw	$0,32($sp)
	sw	$0,36($sp)
	addiu	$2,$sp,20
	move	$4,$2
	lui	$2,%hi(cmd_register)
	addiu	$2,$2,%lo(cmd_register)
	jalr	$2
	nop

	sw	$2,16($sp)
	lw	$2,16($sp)
	bgez	$2,$L447
	nop

	li	$4,1			# 0x1
	lui	$2,%hi($LC114)
	addiu	$5,$2,%lo($LC114)
	lw	$6,16($sp)
	lui	$2,%hi(dbgtoMem)
	addiu	$2,$2,%lo(dbgtoMem)
	jalr	$2
	nop

$L447:
	lw	$2,16($sp)
	lw	$31,44($sp)
	addiu	$sp,$sp,48
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	eponCmdInit
	.size	eponCmdInit, .-eponCmdInit
	.section	.text.eponCmdExit,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	eponCmdExit
	.type	eponCmdExit, @function
eponCmdExit:
	.frame	$sp,24,$31		# vars= 0, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-24
	sw	$31,20($sp)
	lui	$2,%hi($LC113)
	addiu	$4,$2,%lo($LC113)
	lui	$2,%hi(cmd_unregister)
	addiu	$2,$2,%lo(cmd_unregister)
	jalr	$2
	nop

	lw	$31,20($sp)
	addiu	$sp,$sp,24
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	eponCmdExit
	.size	eponCmdExit, .-eponCmdExit
	.rdata
	.align	2
$LC115:
	.ascii	"ignore epon phy los\000"
	.align	2
$LC116:
	.ascii	"EponIsr Epon Phy Los\000"
	.section	.text.eponDetectPhyLosHandler,"ax",@progbits
	.align	2
	.globl	eponDetectPhyLosHandler
	.set	nomips16
	.set	nomicromips
	.ent	eponDetectPhyLosHandler
	.type	eponDetectPhyLosHandler, @function
eponDetectPhyLosHandler:
	.frame	$sp,24,$31		# vars= 0, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-24
	sw	$31,20($sp)
	lui	$2,%hi(gpPhyData)
	lw	$3,%lo(gpPhyData)($2)
	lhu	$2,60($3)
	ins	$2,$0,5,3
	sh	$2,60($3)
	li	$4,2			# 0x2
	move	$5,$0
	move	$6,$0
	lui	$2,%hi(xmcs_report_event)
	addiu	$2,$2,%lo(xmcs_report_event)
	jalr	$2
	nop

	lui	$2,%hi(ignoreIntMask)
	lw	$2,%lo(ignoreIntMask)($2)
	andi	$2,$2,0x8000
	bne	$2,$0,$L451
	nop

	li	$4,4			# 0x4
	lui	$2,%hi($LC115)
	addiu	$5,$2,%lo($LC115)
	lui	$2,%hi(dbgtoMem)
	addiu	$2,$2,%lo(dbgtoMem)
	jalr	$2
	nop

	j	$L450
	nop

$L451:
	lui	$2,%hi(eponReadyFlag)
	sw	$0,%lo(eponReadyFlag)($2)
	li	$4,3			# 0x3
	lui	$2,%hi($LC116)
	addiu	$5,$2,%lo($LC116)
	lui	$2,%hi(dbgtoMem)
	addiu	$2,$2,%lo(dbgtoMem)
	jalr	$2
	nop

	lui	$2,%hi(gpPonSysData)
	lw	$2,%lo(gpPonSysData)($2)
	sw	$0,0($2)
	lui	$2,%hi(eponStop)
	addiu	$2,$2,%lo(eponStop)
	jalr	$2
	nop

	lui	$2,%hi(gpWanPriv)
	lw	$2,%lo(gpWanPriv)($2)
	li	$3,1			# 0x1
	sw	$3,20($2)
	lui	$2,%hi(xpon_reset_qdma_tx_buf)
	addiu	$2,$2,%lo(xpon_reset_qdma_tx_buf)
	jalr	$2
	nop

	lui	$2,%hi(gpPonSysData)
	lw	$2,%lo(gpPonSysData)($2)
	li	$3,2			# 0x2
	sw	$3,4($2)
	move	$4,$0
	lui	$2,%hi(eponFirmwareSetMode)
	addiu	$2,$2,%lo(eponFirmwareSetMode)
	jalr	$2
	nop

	lui	$2,%hi(stop_omci_oam_monitor)
	addiu	$2,$2,%lo(stop_omci_oam_monitor)
	jalr	$2
	nop

	lui	$2,%hi(schedule_fe_reset)
	addiu	$2,$2,%lo(schedule_fe_reset)
	jalr	$2
	nop

$L450:
	lw	$31,20($sp)
	addiu	$sp,$sp,24
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	eponDetectPhyLosHandler
	.size	eponDetectPhyLosHandler, .-eponDetectPhyLosHandler
	.section	.text.epon_los_timer_func,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	epon_los_timer_func
	.type	epon_los_timer_func, @function
epon_los_timer_func:
	.frame	$sp,56,$31		# vars= 32, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-56
	sw	$31,52($sp)
	lui	$2,%hi(eponDrv)
	addiu	$2,$2,%lo(eponDrv)
	sb	$0,8($2)
	lui	$2,%hi(eponDrv)
	addiu	$2,$2,%lo(eponDrv)
	lbu	$2,4($2)
	beq	$2,$0,$L454
	nop

	li	$2,32793			# 0x8019
	sw	$2,16($sp)
	lw	$2,16($sp)
	sw	$2,20($sp)
	sw	$0,24($sp)
	sw	$0,32($sp)
	sw	$0,36($sp)
	sw	$0,40($sp)
	sw	$0,44($sp)
	sw	$0,32($sp)
	lw	$2,20($sp)
	sw	$2,36($sp)
	lw	$2,24($sp)
	sw	$2,44($sp)
	addiu	$2,$sp,32
	sw	$2,28($sp)
	li	$4,25			# 0x19
	move	$5,$0
	lw	$6,28($sp)
	lui	$2,%hi(__ECNT_HOOK)
	addiu	$2,$2,%lo(__ECNT_HOOK)
	jalr	$2
	nop

	move	$3,$2
	li	$2,-1			# 0xffffffffffffffff
	bne	$3,$2,$L455
	nop

	lw	$2,28($sp)
	lw	$3,4($2)
	lui	$2,%hi($LC98)
	addiu	$4,$2,%lo($LC98)
	move	$5,$3
	lui	$2,%hi(panic)
	addiu	$2,$2,%lo(panic)
	jalr	$2
	nop

$L455:
	lw	$2,28($sp)
	lw	$3,8($2)
	li	$2,-1			# 0xffffffffffffffff
	bne	$3,$2,$L456
	nop

	lw	$2,28($sp)
	lw	$5,0($2)
	lw	$2,28($sp)
	lw	$3,4($2)
	lui	$2,%hi($LC99)
	addiu	$4,$2,%lo($LC99)
	move	$6,$3
	lui	$2,%hi(panic)
	addiu	$2,$2,%lo(panic)
	jalr	$2
	nop

$L456:
	lw	$2,28($sp)
	lw	$3,8($2)
	li	$2,1			# 0x1
	bne	$3,$2,$L454
	nop

	j	$L453
	nop

$L454:
	lui	$2,%hi(eponDetectPhyLosHandler)
	addiu	$2,$2,%lo(eponDetectPhyLosHandler)
	jalr	$2
	nop

$L453:
	lw	$31,52($sp)
	addiu	$sp,$sp,56
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	epon_los_timer_func
	.size	epon_los_timer_func, .-epon_los_timer_func
	.rdata
	.align	2
$LC117:
	.ascii	"EponIsr Epon typeB Phy Los\000"
	.section	.text.epon_typeB_phy_los,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	epon_typeB_phy_los
	.type	epon_typeB_phy_los, @function
epon_typeB_phy_los:
	.frame	$sp,40,$31		# vars= 16, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-40
	sw	$31,36($sp)
	lui	$2,%hi(eponDrv+20)
	addiu	$2,$2,%lo(eponDrv+20)
	sw	$2,20($sp)
	lw	$2,20($sp)
	addiu	$2,$2,20
	sw	$2,24($sp)
	sw	$0,16($sp)
	lui	$2,%hi(eponDrv)
	addiu	$2,$2,%lo(eponDrv)
	lbu	$2,4($2)
	beq	$2,$0,$L462
	nop

	lw	$2,24($sp)
	lbu	$3,0($2)
	li	$2,8			# 0x8
	bne	$3,$2,$L462
	nop

	li	$2,1			# 0x1
	sw	$2,16($sp)
	lui	$2,%hi(gpPhyData)
	lw	$3,%lo(gpPhyData)($2)
	lhu	$2,60($3)
	ins	$2,$0,5,3
	sh	$2,60($3)
	li	$4,3			# 0x3
	lui	$2,%hi($LC117)
	addiu	$5,$2,%lo($LC117)
	lui	$2,%hi(dbgtoMem)
	addiu	$2,$2,%lo(dbgtoMem)
	jalr	$2
	nop

	lui	$2,%hi(eponDrv)
	addiu	$2,$2,%lo(eponDrv)
	lhu	$2,6($2)
	move	$3,$2
	lui	$2,%hi(gEponTypeBTimer)
	addiu	$2,$2,%lo(gEponTypeBTimer)
	sw	$3,20($2)
	move	$2,$28
	lw	$2,20($2)
	move	$3,$2
	li	$2,134152192			# 0x7ff0000
	ori	$2,$2,0xff00
	and	$2,$3,$2
	beq	$2,$0,$L463
	nop

	lui	$2,%hi(gEponTypeBTimer)
	addiu	$4,$2,%lo(gEponTypeBTimer)
	lui	$2,%hi(del_timer)
	addiu	$2,$2,%lo(del_timer)
	jalr	$2
	nop

	j	$L464
	nop

$L463:
	lui	$2,%hi(gEponTypeBTimer)
	addiu	$4,$2,%lo(gEponTypeBTimer)
	lui	$2,%hi(del_timer_sync)
	addiu	$2,$2,%lo(del_timer_sync)
	jalr	$2
	nop

$L464:
	lui	$2,%hi(gEponTypeBTimer)
	addiu	$2,$2,%lo(gEponTypeBTimer)
	lw	$2,20($2)
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
	addu	$3,$3,$2
	lui	$2,%hi(gEponTypeBTimer)
	addiu	$4,$2,%lo(gEponTypeBTimer)
	move	$5,$3
	lui	$2,%hi(mod_timer)
	addiu	$2,$2,%lo(mod_timer)
	jalr	$2
	nop

	lui	$2,%hi(eponDrv)
	addiu	$2,$2,%lo(eponDrv)
	li	$3,1			# 0x1
	sb	$3,8($2)
$L462:
	lw	$2,16($sp)
	lw	$31,36($sp)
	addiu	$sp,$sp,40
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	epon_typeB_phy_los
	.size	epon_typeB_phy_los, .-epon_typeB_phy_los
	.section	.text.eponDetectPhyLosLof,"ax",@progbits
	.align	2
	.globl	eponDetectPhyLosLof
	.set	nomips16
	.set	nomicromips
	.ent	eponDetectPhyLosLof
	.type	eponDetectPhyLosLof, @function
eponDetectPhyLosLof:
	.frame	$sp,24,$31		# vars= 0, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-24
	sw	$31,20($sp)
	lui	$2,%hi(epon_typeB_phy_los)
	addiu	$2,$2,%lo(epon_typeB_phy_los)
	jalr	$2
	nop

	bne	$2,$0,$L466
	nop

	lui	$2,%hi(eponDetectPhyLosHandler)
	addiu	$2,$2,%lo(eponDetectPhyLosHandler)
	jalr	$2
	nop

$L466:
	lw	$31,20($sp)
	addiu	$sp,$sp,24
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	eponDetectPhyLosLof
	.size	eponDetectPhyLosLof, .-eponDetectPhyLosLof
	.rdata
	.align	2
$LC118:
	.ascii	"ignore epon phy ready\000"
	.align	2
$LC119:
	.ascii	"EponIsr Epon Phy Ready\000"
	.section	.text.eponPhyReadyRealStart,"ax",@progbits
	.align	2
	.globl	eponPhyReadyRealStart
	.set	nomips16
	.set	nomicromips
	.ent	eponPhyReadyRealStart
	.type	eponPhyReadyRealStart, @function
eponPhyReadyRealStart:
	.frame	$sp,56,$31		# vars= 32, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	addiu	$sp,$sp,-56
	sw	$31,52($sp)
	lui	$2,%hi(ignoreIntMask)
	lw	$2,%lo(ignoreIntMask)($2)
	andi	$2,$2,0x8000
	bne	$2,$0,$L469
	li	$4,4			# 0x4
	lui	$2,%hi($LC118)
	addiu	$5,$2,%lo($LC118)
	lui	$2,%hi(dbgtoMem)
	addiu	$2,$2,%lo(dbgtoMem)
	jalr	$2
	j	$L468
$L469:
	li	$4,3			# 0x3
	lui	$2,%hi($LC119)
	addiu	$5,$2,%lo($LC119)
	lui	$2,%hi(dbgtoMem)
	addiu	$2,$2,%lo(dbgtoMem)
	jalr	$2
	lui	$2,%hi(gpPonSysData)
	lw	$2,%lo(gpPonSysData)($2)
	lw	$3,4($2)
	li	$2,1			# 0x1
	bne	$3,$2,$L471
	li	$4,1			# 0x1
	li	$5,5			# 0x5
	move	$6,$0
	lui	$2,%hi(xmcs_report_event)
	addiu	$2,$2,%lo(xmcs_report_event)
	jalr	$2
$L471:
	li	$4,2			# 0x2
	li	$5,1			# 0x1
	move	$6,$0
	lui	$2,%hi(xmcs_report_event)
	addiu	$2,$2,%lo(xmcs_report_event)
	jalr	$2
	lui	$2,%hi(gpPonSysData)
	lw	$2,%lo(gpPonSysData)($2)
	lw	$2,8($2)
	bne	$2,$0,$L468
	lui	$2,%hi(gpPonSysData)
	lw	$2,%lo(gpPonSysData)($2)
	li	$3,2			# 0x2
	sw	$3,0($2)
	lui	$2,%hi(eponStartTask)
	addiu	$2,$2,%lo(eponStartTask)
	sw	$2,16($sp)
	lw	$2,16($sp)
	addiu	$2,$2,4
	sw	$0,20($sp)
	sw	$2,24($sp)
	lw	$2,20($sp)
	andi	$2,$2,0xffff
	andi	$2,$2,0x1f
	sh	$2,28($sp)
#APP
 # 261 "/opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/bitops.h" 1
			

 # 0 "" 2
#NO_APP
	lw	$2,20($sp)
	srl	$2,$2,5
	sll	$2,$2,2
	lw	$3,24($sp)
	addu	$2,$3,$2
	sw	$2,32($sp)
	lhu	$2,28($sp)
	li	$3,1			# 0x1
	sll	$2,$3,$2
	lw	$5,32($sp)
	lw	$3,32($sp)
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
	sw	$4,36($sp)
	sw	$3,40($sp)
#APP
 # 312 "/opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/bitops.h" 1
			

 # 0 "" 2
#NO_APP
	lw	$2,40($sp)
	sltu	$2,$0,$2
	andi	$2,$2,0x00ff
	bne	$2,$0,$L473
	lw	$4,16($sp)
	lui	$2,%hi(__tasklet_schedule)
	addiu	$2,$2,%lo(__tasklet_schedule)
	jalr	$2
$L473:
	lui	$2,%hi(gpWanPriv)
	lw	$3,%lo(gpWanPriv)($2)
	li	$2,65536			# 0x10000
	addu	$3,$3,$2
	lbu	$2,-31786($3)
	ins	$2,$0,5,1
	sb	$2,-31786($3)
	lui	$2,%hi(gpWanPriv)
	lw	$3,%lo(gpWanPriv)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	lw	$2,-31788($2)
	ext	$2,$2,14,1
	andi	$2,$2,0x00ff
	move	$4,$2
	lui	$2,%hi(xpon_set_qdma_qos)
	addiu	$2,$2,%lo(xpon_set_qdma_qos)
	jalr	$2
$L468:
	lw	$31,52($sp)
	addiu	$sp,$sp,56
	j	$31
	.end	eponPhyReadyRealStart
	.size	eponPhyReadyRealStart, .-eponPhyReadyRealStart
	.section	.text.eponDetectPhyReady,"ax",@progbits
	.align	2
	.globl	eponDetectPhyReady
	.set	nomips16
	.set	nomicromips
	.ent	eponDetectPhyReady
	.type	eponDetectPhyReady, @function
eponDetectPhyReady:
	.frame	$sp,56,$31		# vars= 32, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	addiu	$sp,$sp,-56
	sw	$31,52($sp)
	lui	$2,%hi(eponReadyFlag)
	li	$3,1			# 0x1
	sw	$3,%lo(eponReadyFlag)($2)
	lui	$2,%hi(ignoreIntMask)
	lw	$2,%lo(ignoreIntMask)($2)
	andi	$2,$2,0x8000
	bne	$2,$0,$L475
	li	$4,4			# 0x4
	lui	$2,%hi($LC118)
	addiu	$5,$2,%lo($LC118)
	lui	$2,%hi(dbgtoMem)
	addiu	$2,$2,%lo(dbgtoMem)
	jalr	$2
	j	$L474
$L475:
	move	$4,$0
	lui	$2,%hi(change_alarm_led_status)
	addiu	$2,$2,%lo(change_alarm_led_status)
	jalr	$2
	li	$4,3			# 0x3
	lui	$2,%hi($LC119)
	addiu	$5,$2,%lo($LC119)
	lui	$2,%hi(dbgtoMem)
	addiu	$2,$2,%lo(dbgtoMem)
	jalr	$2
	lui	$2,%hi(eponDrv)
	addiu	$2,$2,%lo(eponDrv)
	lbu	$2,8($2)
	bne	$2,$0,$L477
	move	$4,$0
	lui	$2,%hi(eponFirmwareSetMode)
	addiu	$2,$2,%lo(eponFirmwareSetMode)
	jalr	$2
$L477:
	lui	$2,%hi(gpPonSysData)
	lw	$2,%lo(gpPonSysData)($2)
	lw	$3,4($2)
	li	$2,1			# 0x1
	bne	$3,$2,$L478
	li	$4,1			# 0x1
	li	$5,5			# 0x5
	move	$6,$0
	lui	$2,%hi(xmcs_report_event)
	addiu	$2,$2,%lo(xmcs_report_event)
	jalr	$2
$L478:
	li	$4,2			# 0x2
	li	$5,1			# 0x1
	move	$6,$0
	lui	$2,%hi(xmcs_report_event)
	addiu	$2,$2,%lo(xmcs_report_event)
	jalr	$2
	lui	$2,%hi(gpPonSysData)
	lw	$2,%lo(gpPonSysData)($2)
	lw	$2,8($2)
	bne	$2,$0,$L474
	lui	$2,%hi(gpPonSysData)
	lw	$2,%lo(gpPonSysData)($2)
	li	$3,2			# 0x2
	sw	$3,0($2)
	lui	$2,%hi(eponDrv)
	addiu	$2,$2,%lo(eponDrv)
	lbu	$2,8($2)
	bne	$2,$0,$L474
	lui	$2,%hi(eponStartTask)
	addiu	$2,$2,%lo(eponStartTask)
	sw	$2,16($sp)
	lw	$2,16($sp)
	addiu	$2,$2,4
	sw	$0,20($sp)
	sw	$2,24($sp)
	lw	$2,20($sp)
	andi	$2,$2,0xffff
	andi	$2,$2,0x1f
	sh	$2,28($sp)
#APP
 # 261 "/opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/bitops.h" 1
			

 # 0 "" 2
#NO_APP
	lw	$2,20($sp)
	srl	$2,$2,5
	sll	$2,$2,2
	lw	$3,24($sp)
	addu	$2,$3,$2
	sw	$2,32($sp)
	lhu	$2,28($sp)
	li	$3,1			# 0x1
	sll	$2,$3,$2
	lw	$5,32($sp)
	lw	$3,32($sp)
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
	sw	$4,36($sp)
	sw	$3,40($sp)
#APP
 # 312 "/opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/bitops.h" 1
			

 # 0 "" 2
#NO_APP
	lw	$2,40($sp)
	sltu	$2,$0,$2
	andi	$2,$2,0x00ff
	bne	$2,$0,$L480
	lw	$4,16($sp)
	lui	$2,%hi(__tasklet_schedule)
	addiu	$2,$2,%lo(__tasklet_schedule)
	jalr	$2
$L480:
	lui	$2,%hi(gpWanPriv)
	lw	$3,%lo(gpWanPriv)($2)
	li	$2,65536			# 0x10000
	addu	$3,$3,$2
	lbu	$2,-31786($3)
	ins	$2,$0,5,1
	sb	$2,-31786($3)
	lui	$2,%hi(gpWanPriv)
	lw	$3,%lo(gpWanPriv)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	lw	$2,-31788($2)
	ext	$2,$2,14,1
	andi	$2,$2,0x00ff
	move	$4,$2
	lui	$2,%hi(xpon_set_qdma_qos)
	addiu	$2,$2,%lo(xpon_set_qdma_qos)
	jalr	$2
$L474:
	lw	$31,52($sp)
	addiu	$sp,$sp,56
	j	$31
	.end	eponDetectPhyReady
	.size	eponDetectPhyReady, .-eponDetectPhyReady
	.section	.text.epon_init_private_data,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	epon_init_private_data
	.type	epon_init_private_data, @function
epon_init_private_data:
	.frame	$sp,8,$31		# vars= 8, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-8
	sw	$4,8($sp)
	sw	$0,0($sp)
	lw	$2,8($sp)
	sb	$0,40($2)
	lw	$2,8($sp)
	sb	$0,37($2)
	lw	$2,8($sp)
	sb	$0,38($2)
	lw	$2,8($sp)
	sb	$0,36($2)
	li	$2,-1078984704			# 0xffffffffbfb00000
	ori	$2,$2,0x64
	lw	$3,0($2)
	li	$2,-65536			# 0xffffffffffff0000
	and	$3,$3,$2
	li	$2,524288			# 0x80000
	bne	$3,$2,$L482
	nop

	lw	$2,8($sp)
	li	$3,1			# 0x1
	sb	$3,40($2)
$L482:
	lw	$2,0($sp)
	addiu	$sp,$sp,8
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	epon_init_private_data
	.size	epon_init_private_data, .-epon_init_private_data
	.rdata
	.align	2
$LC120:
	.ascii	"eponInit\000"
	.align	2
$LC121:
	.ascii	"ERR: ioremap the EPON base address failed:addr=0x%x,size"
	.ascii	"=0x%x\000"
	.align	2
$LC122:
	.ascii	"EPON private data initialization failed\000"
	.align	2
$LC123:
	.ascii	"epon_mac\000"
	.section	.text.eponInit,"ax",@progbits
	.align	2
	.globl	eponInit
	.set	nomips16
	.set	nomicromips
	.ent	eponInit
	.type	eponInit, @function
eponInit:
	.frame	$sp,88,$31		# vars= 56, regs= 1/0, args= 24, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-88
	sw	$31,84($sp)
	li	$2,-1			# 0xffffffffffffffff
	sw	$2,24($sp)
	li	$4,4			# 0x4
	lui	$2,%hi($LC120)
	addiu	$5,$2,%lo($LC120)
	lui	$2,%hi(dbgtoMem)
	addiu	$2,$2,%lo(dbgtoMem)
	jalr	$2
	nop

	lui	$2,%hi(epon_reg_lock)
	addiu	$2,$2,%lo(epon_reg_lock)
	sw	$2,28($sp)
	lui	$2,%hi(epon_reg_lock)
	sw	$0,%lo(epon_reg_lock)($2)
	li	$2,532021248			# 0x1fb60000
	sw	$2,32($sp)
	li	$2,61188			# 0xef04
	sw	$2,36($sp)
	li	$2,512			# 0x200
	sw	$2,40($sp)
	lw	$2,32($sp)
	sw	$2,44($sp)
	lw	$2,36($sp)
	sw	$2,48($sp)
	lw	$2,40($sp)
	sw	$2,52($sp)
	move	$2,$0
	sw	$2,56($sp)
	lw	$2,56($sp)
	beq	$2,$0,$L487
	nop

	lw	$2,56($sp)
	j	$L488
	nop

$L487:
	lw	$4,32($sp)
	lw	$5,36($sp)
	lw	$6,40($sp)
	lui	$2,%hi(__ioremap)
	addiu	$2,$2,%lo(__ioremap)
	jalr	$2
	nop

$L488:
	lui	$3,%hi(g_EPON_MAC_BASE)
	sw	$2,%lo(g_EPON_MAC_BASE)($3)
	lui	$2,%hi(g_EPON_MAC_BASE)
	lw	$2,%lo(g_EPON_MAC_BASE)($2)
	bne	$2,$0,$L489
	nop

	li	$4,1			# 0x1
	lui	$2,%hi($LC121)
	addiu	$5,$2,%lo($LC121)
	li	$6,532021248			# 0x1fb60000
	li	$7,61188			# 0xef04
	lui	$2,%hi(dbgtoMem)
	addiu	$2,$2,%lo(dbgtoMem)
	jalr	$2
	nop

	li	$2,-12			# 0xfffffffffffffff4
	j	$L496
	nop

$L489:
	lui	$2,%hi(gpEponPriv)
	lw	$2,%lo(gpEponPriv)($2)
	move	$4,$2
	lui	$2,%hi(epon_init_private_data)
	addiu	$2,$2,%lo(epon_init_private_data)
	jalr	$2
	nop

	sw	$2,24($sp)
	lw	$2,24($sp)
	bgez	$2,$L491
	nop

	li	$4,1			# 0x1
	lui	$2,%hi($LC122)
	addiu	$5,$2,%lo($LC122)
	lui	$2,%hi(dbgtoMem)
	addiu	$2,$2,%lo(dbgtoMem)
	jalr	$2
	nop

	lw	$2,24($sp)
	j	$L496
	nop

$L491:
	lui	$2,%hi(eponMacTableInit)
	addiu	$2,$2,%lo(eponMacTableInit)
	jalr	$2
	nop

	lui	$2,%hi(eponOnuMacAddr)
	addiu	$4,$2,%lo(eponOnuMacAddr)
	lui	$2,%hi(getOnuMacAddr)
	addiu	$2,$2,%lo(getOnuMacAddr)
	jalr	$2
	nop

	lui	$2,%hi(eponCmdInit)
	addiu	$2,$2,%lo(eponCmdInit)
	jalr	$2
	nop

	lui	$2,%hi(eponDrv)
	addiu	$4,$2,%lo(eponDrv)
	move	$5,$0
	li	$6,600			# 0x258
	lui	$2,%hi(memset)
	addiu	$2,$2,%lo(memset)
	jalr	$2
	nop

	move	$4,$0
	lui	$2,%hi(eponFirmwareSetMode)
	addiu	$2,$2,%lo(eponFirmwareSetMode)
	jalr	$2
	nop

	li	$2,221			# 0xdd
	sw	$2,60($sp)
	lui	$2,%hi($LC123)
	addiu	$2,$2,%lo($LC123)
	sw	$2,64($sp)
	lui	$2,%hi(eponMacFops)
	addiu	$2,$2,%lo(eponMacFops)
	sw	$2,68($sp)
	lw	$2,68($sp)
	sw	$2,16($sp)
	lw	$4,60($sp)
	move	$5,$0
	li	$6,256			# 0x100
	lw	$7,64($sp)
	lui	$2,%hi(__register_chrdev)
	addiu	$2,$2,%lo(__register_chrdev)
	jalr	$2
	nop

	sw	$2,24($sp)
	lw	$2,24($sp)
	bgez	$2,$L493
	nop

	lw	$2,24($sp)
	j	$L496
	nop

$L493:
	lui	$2,%hi(eponMacMpcpDscvGateTask)
	addiu	$4,$2,%lo(eponMacMpcpDscvGateTask)
	lui	$2,%hi(eponMpcpDiscvGateIntHandler)
	addiu	$5,$2,%lo(eponMpcpDiscvGateIntHandler)
	move	$6,$0
	lui	$2,%hi(tasklet_init)
	addiu	$2,$2,%lo(tasklet_init)
	jalr	$2
	nop

	lui	$2,%hi(eponStartTask)
	addiu	$4,$2,%lo(eponStartTask)
	lui	$2,%hi(eponStart)
	addiu	$5,$2,%lo(eponStart)
	move	$6,$0
	lui	$2,%hi(tasklet_init)
	addiu	$2,$2,%lo(tasklet_init)
	jalr	$2
	nop

	lui	$2,%hi(rgstIntvalTimer)
	addiu	$4,$2,%lo(rgstIntvalTimer)
	li	$5,1			# 0x1
	lui	$2,%hi(rgstIntvalTimerHandler)
	addiu	$6,$2,%lo(rgstIntvalTimerHandler)
	move	$7,$0
	lui	$2,%hi(eponMacAddTimer)
	addiu	$2,$2,%lo(eponMacAddTimer)
	jalr	$2
	nop

	addiu	$2,$sp,72
	move	$4,$2
	lui	$2,%hi(xmcs_get_onu_type)
	addiu	$2,$2,%lo(xmcs_get_onu_type)
	jalr	$2
	nop

	lw	$3,72($sp)
	li	$2,2			# 0x2
	beq	$3,$2,$L494
	nop

	lui	$2,%hi(isSfu)
	li	$3,1			# 0x1
	sb	$3,%lo(isSfu)($2)
	j	$L495
	nop

$L494:
	lui	$2,%hi(isSfu)
	sb	$0,%lo(isSfu)($2)
$L495:
	lui	$2,%hi(gEponTypeBTimer)
	addiu	$4,$2,%lo(gEponTypeBTimer)
	move	$5,$0
	move	$6,$0
	lui	$2,%hi(init_timer_key)
	addiu	$2,$2,%lo(init_timer_key)
	jalr	$2
	nop

	lui	$2,%hi(gEponTypeBTimer)
	addiu	$2,$2,%lo(gEponTypeBTimer)
	li	$3,1000			# 0x3e8
	sw	$3,20($2)
	lui	$2,%hi(gEponTypeBTimer)
	addiu	$2,$2,%lo(gEponTypeBTimer)
	lui	$3,%hi(epon_los_timer_func)
	addiu	$3,$3,%lo(epon_los_timer_func)
	sw	$3,16($2)
	lui	$2,%hi(dbgPrintInit)
	addiu	$2,$2,%lo(dbgPrintInit)
	jalr	$2
	nop

	lw	$2,24($sp)
$L496:
	lw	$31,84($sp)
	addiu	$sp,$sp,88
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	eponInit
	.size	eponInit, .-eponInit
	.rdata
	.align	2
$LC124:
	.ascii	"eponExit\000"
	.section	.text.eponExit,"ax",@progbits
	.align	2
	.globl	eponExit
	.set	nomips16
	.set	nomicromips
	.ent	eponExit
	.type	eponExit, @function
eponExit:
	.frame	$sp,40,$31		# vars= 16, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-40
	sw	$31,36($sp)
	li	$4,4			# 0x4
	lui	$2,%hi($LC124)
	addiu	$5,$2,%lo($LC124)
	lui	$2,%hi(dbgtoMem)
	addiu	$2,$2,%lo(dbgtoMem)
	jalr	$2
	nop

	lui	$2,%hi(eponCmdExit)
	addiu	$2,$2,%lo(eponCmdExit)
	jalr	$2
	nop

	li	$2,221			# 0xdd
	sw	$2,16($sp)
	lui	$2,%hi($LC123)
	addiu	$2,$2,%lo($LC123)
	sw	$2,20($sp)
	lw	$4,16($sp)
	move	$5,$0
	li	$6,256			# 0x100
	lw	$7,20($sp)
	lui	$2,%hi(__unregister_chrdev)
	addiu	$2,$2,%lo(__unregister_chrdev)
	jalr	$2
	nop

	lui	$2,%hi(eponMacMpcpDscvGateTask)
	addiu	$4,$2,%lo(eponMacMpcpDscvGateTask)
	lui	$2,%hi(tasklet_kill)
	addiu	$2,$2,%lo(tasklet_kill)
	jalr	$2
	nop

	lui	$2,%hi(eponStartTask)
	addiu	$4,$2,%lo(eponStartTask)
	lui	$2,%hi(tasklet_kill)
	addiu	$2,$2,%lo(tasklet_kill)
	jalr	$2
	nop

	lui	$2,%hi(rgstIntvalTimer)
	addiu	$4,$2,%lo(rgstIntvalTimer)
	lui	$2,%hi(eponMacDelTimer)
	addiu	$2,$2,%lo(eponMacDelTimer)
	jalr	$2
	nop

	lui	$2,%hi(dbgPrintQuit)
	addiu	$2,$2,%lo(dbgPrintQuit)
	jalr	$2
	nop

	lui	$2,%hi(g_EPON_MAC_BASE)
	lw	$2,%lo(g_EPON_MAC_BASE)($2)
	beq	$2,$0,$L498
	nop

	lui	$2,%hi(g_EPON_MAC_BASE)
	lw	$2,%lo(g_EPON_MAC_BASE)($2)
	sw	$2,24($sp)
	lw	$2,24($sp)
	sw	$2,28($sp)
	move	$2,$0
	bne	$2,$0,$L500
	nop

	lw	$4,24($sp)
	lui	$2,%hi(__iounmap)
	addiu	$2,$2,%lo(__iounmap)
	jalr	$2
	nop

$L500:
	lui	$2,%hi(g_EPON_MAC_BASE)
	sw	$0,%lo(g_EPON_MAC_BASE)($2)
$L498:
	lui	$2,%hi(eponMacTableExit)
	addiu	$2,$2,%lo(eponMacTableExit)
	jalr	$2
	nop

	move	$2,$28
	lw	$2,20($2)
	move	$3,$2
	li	$2,134152192			# 0x7ff0000
	ori	$2,$2,0xff00
	and	$2,$3,$2
	beq	$2,$0,$L501
	nop

	lui	$2,%hi(gEponTypeBTimer)
	addiu	$4,$2,%lo(gEponTypeBTimer)
	lui	$2,%hi(del_timer)
	addiu	$2,$2,%lo(del_timer)
	jalr	$2
	nop

	j	$L502
	nop

$L501:
	lui	$2,%hi(gEponTypeBTimer)
	addiu	$4,$2,%lo(gEponTypeBTimer)
	lui	$2,%hi(del_timer_sync)
	addiu	$2,$2,%lo(del_timer_sync)
	jalr	$2
	nop

$L502:
	nop
	lw	$31,36($sp)
	addiu	$sp,$sp,40
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	eponExit
	.size	eponExit, .-eponExit
	.section	.text.eponMacTableInit,"ax",@progbits
	.align	2
	.globl	eponMacTableInit
	.set	nomips16
	.set	nomicromips
	.ent	eponMacTableInit
	.type	eponMacTableInit, @function
eponMacTableInit:
	.frame	$sp,40,$31		# vars= 16, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-40
	sw	$31,36($sp)
	sw	$0,16($sp)
	li	$2,840			# 0x348
	sw	$2,20($sp)
	li	$2,208			# 0xd0
	sw	$2,24($sp)
$L505 = .
	lw	$4,20($sp)
	lw	$5,24($sp)
	lui	$2,%hi(__kmalloc)
	addiu	$2,$2,%lo(__kmalloc)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(eponMacRegTable)
	sw	$3,%lo(eponMacRegTable)($2)
	lui	$2,%hi(eponMacRegTable)
	lw	$3,%lo(eponMacRegTable)($2)
	lw	$2,16($sp)
	sll	$2,$2,2
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	lui	$3,%hi(g_EPON_MAC_BASE)
	lw	$3,%lo(g_EPON_MAC_BASE)($3)
	addiu	$3,$3,24576
	sw	$3,0($2)
	lui	$2,%hi(eponMacRegTable)
	lw	$3,%lo(eponMacRegTable)($2)
	lw	$2,16($sp)
	sll	$2,$2,2
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	li	$3,8388608			# 0x800000
	ori	$3,$3,0x42
	sw	$3,4($2)
	lui	$2,%hi(eponMacRegTable)
	lw	$3,%lo(eponMacRegTable)($2)
	lw	$2,16($sp)
	sll	$2,$2,2
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	li	$3,4128768			# 0x3f0000
	ori	$3,$3,0x7
	sw	$3,8($2)
	lw	$2,16($sp)
	addiu	$2,$2,1
	sw	$2,16($sp)
	lui	$2,%hi(eponMacRegTable)
	lw	$3,%lo(eponMacRegTable)($2)
	lw	$2,16($sp)
	sll	$2,$2,2
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	lui	$3,%hi(g_EPON_MAC_BASE)
	lw	$3,%lo(g_EPON_MAC_BASE)($3)
	addiu	$3,$3,24580
	sw	$3,0($2)
	lui	$2,%hi(eponMacRegTable)
	lw	$3,%lo(eponMacRegTable)($2)
	lw	$2,16($sp)
	sll	$2,$2,2
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	sw	$0,4($2)
	lui	$2,%hi(eponMacRegTable)
	lw	$3,%lo(eponMacRegTable)($2)
	lw	$2,16($sp)
	sll	$2,$2,2
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	sw	$0,8($2)
	lw	$2,16($sp)
	addiu	$2,$2,1
	sw	$2,16($sp)
	lui	$2,%hi(eponMacRegTable)
	lw	$3,%lo(eponMacRegTable)($2)
	lw	$2,16($sp)
	sll	$2,$2,2
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	lui	$3,%hi(g_EPON_MAC_BASE)
	lw	$3,%lo(g_EPON_MAC_BASE)($3)
	addiu	$3,$3,24584
	sw	$3,0($2)
	lui	$2,%hi(eponMacRegTable)
	lw	$3,%lo(eponMacRegTable)($2)
	lw	$2,16($sp)
	sll	$2,$2,2
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	sw	$0,4($2)
	lui	$2,%hi(eponMacRegTable)
	lw	$3,%lo(eponMacRegTable)($2)
	lw	$2,16($sp)
	sll	$2,$2,2
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	sw	$0,8($2)
	lw	$2,16($sp)
	addiu	$2,$2,1
	sw	$2,16($sp)
	lui	$2,%hi(eponMacRegTable)
	lw	$3,%lo(eponMacRegTable)($2)
	lw	$2,16($sp)
	sll	$2,$2,2
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	lui	$3,%hi(g_EPON_MAC_BASE)
	lw	$3,%lo(g_EPON_MAC_BASE)($3)
	addiu	$3,$3,24588
	sw	$3,0($2)
	lui	$2,%hi(eponMacRegTable)
	lw	$3,%lo(eponMacRegTable)($2)
	lw	$2,16($sp)
	sll	$2,$2,2
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	sw	$0,4($2)
	lui	$2,%hi(eponMacRegTable)
	lw	$3,%lo(eponMacRegTable)($2)
	lw	$2,16($sp)
	sll	$2,$2,2
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	sw	$0,8($2)
	lw	$2,16($sp)
	addiu	$2,$2,1
	sw	$2,16($sp)
	lui	$2,%hi(eponMacRegTable)
	lw	$3,%lo(eponMacRegTable)($2)
	lw	$2,16($sp)
	sll	$2,$2,2
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	lui	$3,%hi(g_EPON_MAC_BASE)
	lw	$3,%lo(g_EPON_MAC_BASE)($3)
	addiu	$3,$3,24592
	sw	$3,0($2)
	lui	$2,%hi(eponMacRegTable)
	lw	$3,%lo(eponMacRegTable)($2)
	lw	$2,16($sp)
	sll	$2,$2,2
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	li	$3,256			# 0x100
	sw	$3,4($2)
	lui	$2,%hi(eponMacRegTable)
	lw	$3,%lo(eponMacRegTable)($2)
	lw	$2,16($sp)
	sll	$2,$2,2
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	li	$3,-2147483648			# 0xffffffff80000000
	ori	$3,$3,0x37f
	sw	$3,8($2)
	lw	$2,16($sp)
	addiu	$2,$2,1
	sw	$2,16($sp)
	lui	$2,%hi(eponMacRegTable)
	lw	$3,%lo(eponMacRegTable)($2)
	lw	$2,16($sp)
	sll	$2,$2,2
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	lui	$3,%hi(g_EPON_MAC_BASE)
	lw	$3,%lo(g_EPON_MAC_BASE)($3)
	addiu	$3,$3,24596
	sw	$3,0($2)
	lui	$2,%hi(eponMacRegTable)
	lw	$3,%lo(eponMacRegTable)($2)
	lw	$2,16($sp)
	sll	$2,$2,2
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	li	$3,64			# 0x40
	sw	$3,4($2)
	lui	$2,%hi(eponMacRegTable)
	lw	$3,%lo(eponMacRegTable)($2)
	lw	$2,16($sp)
	sll	$2,$2,2
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	li	$3,127			# 0x7f
	sw	$3,8($2)
	lw	$2,16($sp)
	addiu	$2,$2,1
	sw	$2,16($sp)
	lui	$2,%hi(eponMacRegTable)
	lw	$3,%lo(eponMacRegTable)($2)
	lw	$2,16($sp)
	sll	$2,$2,2
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	lui	$3,%hi(g_EPON_MAC_BASE)
	lw	$3,%lo(g_EPON_MAC_BASE)($3)
	addiu	$3,$3,24608
	sw	$3,0($2)
	lui	$2,%hi(eponMacRegTable)
	lw	$3,%lo(eponMacRegTable)($2)
	lw	$2,16($sp)
	sll	$2,$2,2
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	sw	$0,4($2)
	lui	$2,%hi(eponMacRegTable)
	lw	$3,%lo(eponMacRegTable)($2)
	lw	$2,16($sp)
	sll	$2,$2,2
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	li	$3,252641280			# 0xf0f0000
	ori	$3,$3,0xf0f
	sw	$3,8($2)
	lw	$2,16($sp)
	addiu	$2,$2,1
	sw	$2,16($sp)
	lui	$2,%hi(eponMacRegTable)
	lw	$3,%lo(eponMacRegTable)($2)
	lw	$2,16($sp)
	sll	$2,$2,2
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	lui	$3,%hi(g_EPON_MAC_BASE)
	lw	$3,%lo(g_EPON_MAC_BASE)($3)
	addiu	$3,$3,24612
	sw	$3,0($2)
	lui	$2,%hi(eponMacRegTable)
	lw	$3,%lo(eponMacRegTable)($2)
	lw	$2,16($sp)
	sll	$2,$2,2
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	sw	$0,4($2)
	lui	$2,%hi(eponMacRegTable)
	lw	$3,%lo(eponMacRegTable)($2)
	lw	$2,16($sp)
	sll	$2,$2,2
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	li	$3,252641280			# 0xf0f0000
	ori	$3,$3,0xf0f
	sw	$3,8($2)
	lw	$2,16($sp)
	addiu	$2,$2,1
	sw	$2,16($sp)
	lui	$2,%hi(eponMacRegTable)
	lw	$3,%lo(eponMacRegTable)($2)
	lw	$2,16($sp)
	sll	$2,$2,2
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	lui	$3,%hi(g_EPON_MAC_BASE)
	lw	$3,%lo(g_EPON_MAC_BASE)($3)
	addiu	$3,$3,24616
	sw	$3,0($2)
	lui	$2,%hi(eponMacRegTable)
	lw	$3,%lo(eponMacRegTable)($2)
	lw	$2,16($sp)
	sll	$2,$2,2
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	li	$3,65536			# 0x10000
	sw	$3,4($2)
	lui	$2,%hi(eponMacRegTable)
	lw	$3,%lo(eponMacRegTable)($2)
	lw	$2,16($sp)
	sll	$2,$2,2
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	li	$3,-2147483648			# 0xffffffff80000000
	ori	$3,$3,0x1107
	sw	$3,8($2)
	lw	$2,16($sp)
	addiu	$2,$2,1
	sw	$2,16($sp)
	lui	$2,%hi(eponMacRegTable)
	lw	$3,%lo(eponMacRegTable)($2)
	lw	$2,16($sp)
	sll	$2,$2,2
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	lui	$3,%hi(g_EPON_MAC_BASE)
	lw	$3,%lo(g_EPON_MAC_BASE)($3)
	addiu	$3,$3,24620
	sw	$3,0($2)
	lui	$2,%hi(eponMacRegTable)
	lw	$3,%lo(eponMacRegTable)($2)
	lw	$2,16($sp)
	sll	$2,$2,2
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	sw	$0,4($2)
	lui	$2,%hi(eponMacRegTable)
	lw	$3,%lo(eponMacRegTable)($2)
	lw	$2,16($sp)
	sll	$2,$2,2
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	li	$3,-1073741824			# 0xffffffffc0000000
	sw	$3,8($2)
	lw	$2,16($sp)
	addiu	$2,$2,1
	sw	$2,16($sp)
	lui	$2,%hi(eponMacRegTable)
	lw	$3,%lo(eponMacRegTable)($2)
	lw	$2,16($sp)
	sll	$2,$2,2
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	lui	$3,%hi(g_EPON_MAC_BASE)
	lw	$3,%lo(g_EPON_MAC_BASE)($3)
	addiu	$3,$3,24624
	sw	$3,0($2)
	lui	$2,%hi(eponMacRegTable)
	lw	$3,%lo(eponMacRegTable)($2)
	lw	$2,16($sp)
	sll	$2,$2,2
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	sw	$0,4($2)
	lui	$2,%hi(eponMacRegTable)
	lw	$3,%lo(eponMacRegTable)($2)
	lw	$2,16($sp)
	sll	$2,$2,2
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	li	$3,-1073741824			# 0xffffffffc0000000
	sw	$3,8($2)
	lw	$2,16($sp)
	addiu	$2,$2,1
	sw	$2,16($sp)
	lui	$2,%hi(eponMacRegTable)
	lw	$3,%lo(eponMacRegTable)($2)
	lw	$2,16($sp)
	sll	$2,$2,2
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	lui	$3,%hi(g_EPON_MAC_BASE)
	lw	$3,%lo(g_EPON_MAC_BASE)($3)
	addiu	$3,$3,24628
	sw	$3,0($2)
	lui	$2,%hi(eponMacRegTable)
	lw	$3,%lo(eponMacRegTable)($2)
	lw	$2,16($sp)
	sll	$2,$2,2
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	sw	$0,4($2)
	lui	$2,%hi(eponMacRegTable)
	lw	$3,%lo(eponMacRegTable)($2)
	lw	$2,16($sp)
	sll	$2,$2,2
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	li	$3,-1073741824			# 0xffffffffc0000000
	sw	$3,8($2)
	lw	$2,16($sp)
	addiu	$2,$2,1
	sw	$2,16($sp)
	lui	$2,%hi(eponMacRegTable)
	lw	$3,%lo(eponMacRegTable)($2)
	lw	$2,16($sp)
	sll	$2,$2,2
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	lui	$3,%hi(g_EPON_MAC_BASE)
	lw	$3,%lo(g_EPON_MAC_BASE)($3)
	addiu	$3,$3,24632
	sw	$3,0($2)
	lui	$2,%hi(eponMacRegTable)
	lw	$3,%lo(eponMacRegTable)($2)
	lw	$2,16($sp)
	sll	$2,$2,2
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	sw	$0,4($2)
	lui	$2,%hi(eponMacRegTable)
	lw	$3,%lo(eponMacRegTable)($2)
	lw	$2,16($sp)
	sll	$2,$2,2
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	li	$3,-1073741824			# 0xffffffffc0000000
	sw	$3,8($2)
	lw	$2,16($sp)
	addiu	$2,$2,1
	sw	$2,16($sp)
	lui	$2,%hi(eponMacRegTable)
	lw	$3,%lo(eponMacRegTable)($2)
	lw	$2,16($sp)
	sll	$2,$2,2
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	lui	$3,%hi(g_EPON_MAC_BASE)
	lw	$3,%lo(g_EPON_MAC_BASE)($3)
	addiu	$3,$3,24636
	sw	$3,0($2)
	lui	$2,%hi(eponMacRegTable)
	lw	$3,%lo(eponMacRegTable)($2)
	lw	$2,16($sp)
	sll	$2,$2,2
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	sw	$0,4($2)
	lui	$2,%hi(eponMacRegTable)
	lw	$3,%lo(eponMacRegTable)($2)
	lw	$2,16($sp)
	sll	$2,$2,2
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	li	$3,-1073741824			# 0xffffffffc0000000
	sw	$3,8($2)
	lw	$2,16($sp)
	addiu	$2,$2,1
	sw	$2,16($sp)
	lui	$2,%hi(eponMacRegTable)
	lw	$3,%lo(eponMacRegTable)($2)
	lw	$2,16($sp)
	sll	$2,$2,2
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	lui	$3,%hi(g_EPON_MAC_BASE)
	lw	$3,%lo(g_EPON_MAC_BASE)($3)
	addiu	$3,$3,24640
	sw	$3,0($2)
	lui	$2,%hi(eponMacRegTable)
	lw	$3,%lo(eponMacRegTable)($2)
	lw	$2,16($sp)
	sll	$2,$2,2
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	sw	$0,4($2)
	lui	$2,%hi(eponMacRegTable)
	lw	$3,%lo(eponMacRegTable)($2)
	lw	$2,16($sp)
	sll	$2,$2,2
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	li	$3,-1073741824			# 0xffffffffc0000000
	sw	$3,8($2)
	lw	$2,16($sp)
	addiu	$2,$2,1
	sw	$2,16($sp)
	lui	$2,%hi(eponMacRegTable)
	lw	$3,%lo(eponMacRegTable)($2)
	lw	$2,16($sp)
	sll	$2,$2,2
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	lui	$3,%hi(g_EPON_MAC_BASE)
	lw	$3,%lo(g_EPON_MAC_BASE)($3)
	addiu	$3,$3,24644
	sw	$3,0($2)
	lui	$2,%hi(eponMacRegTable)
	lw	$3,%lo(eponMacRegTable)($2)
	lw	$2,16($sp)
	sll	$2,$2,2
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	sw	$0,4($2)
	lui	$2,%hi(eponMacRegTable)
	lw	$3,%lo(eponMacRegTable)($2)
	lw	$2,16($sp)
	sll	$2,$2,2
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	li	$3,-1073741824			# 0xffffffffc0000000
	sw	$3,8($2)
	lw	$2,16($sp)
	addiu	$2,$2,1
	sw	$2,16($sp)
	lui	$2,%hi(eponMacRegTable)
	lw	$3,%lo(eponMacRegTable)($2)
	lw	$2,16($sp)
	sll	$2,$2,2
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	lui	$3,%hi(g_EPON_MAC_BASE)
	lw	$3,%lo(g_EPON_MAC_BASE)($3)
	addiu	$3,$3,24648
	sw	$3,0($2)
	lui	$2,%hi(eponMacRegTable)
	lw	$3,%lo(eponMacRegTable)($2)
	lw	$2,16($sp)
	sll	$2,$2,2
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	sw	$0,4($2)
	lui	$2,%hi(eponMacRegTable)
	lw	$3,%lo(eponMacRegTable)($2)
	lw	$2,16($sp)
	sll	$2,$2,2
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	li	$3,-1073741824			# 0xffffffffc0000000
	sw	$3,8($2)
	lw	$2,16($sp)
	addiu	$2,$2,1
	sw	$2,16($sp)
	lui	$2,%hi(eponMacRegTable)
	lw	$3,%lo(eponMacRegTable)($2)
	lw	$2,16($sp)
	sll	$2,$2,2
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	lui	$3,%hi(g_EPON_MAC_BASE)
	lw	$3,%lo(g_EPON_MAC_BASE)($3)
	addiu	$3,$3,24656
	sw	$3,0($2)
	lui	$2,%hi(eponMacRegTable)
	lw	$3,%lo(eponMacRegTable)($2)
	lw	$2,16($sp)
	sll	$2,$2,2
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	sw	$0,4($2)
	lui	$2,%hi(eponMacRegTable)
	lw	$3,%lo(eponMacRegTable)($2)
	lw	$2,16($sp)
	sll	$2,$2,2
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	li	$3,-2147483648			# 0xffffffff80000000
	ori	$3,$3,0xf
	sw	$3,8($2)
	lw	$2,16($sp)
	addiu	$2,$2,1
	sw	$2,16($sp)
	lui	$2,%hi(eponMacRegTable)
	lw	$3,%lo(eponMacRegTable)($2)
	lw	$2,16($sp)
	sll	$2,$2,2
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	lui	$3,%hi(g_EPON_MAC_BASE)
	lw	$3,%lo(g_EPON_MAC_BASE)($3)
	addiu	$3,$3,24660
	sw	$3,0($2)
	lui	$2,%hi(eponMacRegTable)
	lw	$3,%lo(eponMacRegTable)($2)
	lw	$2,16($sp)
	sll	$2,$2,2
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	sw	$0,4($2)
	lui	$2,%hi(eponMacRegTable)
	lw	$3,%lo(eponMacRegTable)($2)
	lw	$2,16($sp)
	sll	$2,$2,2
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	sw	$0,8($2)
	lw	$2,16($sp)
	addiu	$2,$2,1
	sw	$2,16($sp)
	lui	$2,%hi(eponMacRegTable)
	lw	$3,%lo(eponMacRegTable)($2)
	lw	$2,16($sp)
	sll	$2,$2,2
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	lui	$3,%hi(g_EPON_MAC_BASE)
	lw	$3,%lo(g_EPON_MAC_BASE)($3)
	addiu	$3,$3,24664
	sw	$3,0($2)
	lui	$2,%hi(eponMacRegTable)
	lw	$3,%lo(eponMacRegTable)($2)
	lw	$2,16($sp)
	sll	$2,$2,2
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	sw	$0,4($2)
	lui	$2,%hi(eponMacRegTable)
	lw	$3,%lo(eponMacRegTable)($2)
	lw	$2,16($sp)
	sll	$2,$2,2
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	li	$3,-2147483648			# 0xffffffff80000000
	ori	$3,$3,0x3f
	sw	$3,8($2)
	lw	$2,16($sp)
	addiu	$2,$2,1
	sw	$2,16($sp)
	lui	$2,%hi(eponMacRegTable)
	lw	$3,%lo(eponMacRegTable)($2)
	lw	$2,16($sp)
	sll	$2,$2,2
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	lui	$3,%hi(g_EPON_MAC_BASE)
	lw	$3,%lo(g_EPON_MAC_BASE)($3)
	addiu	$3,$3,24668
	sw	$3,0($2)
	lui	$2,%hi(eponMacRegTable)
	lw	$3,%lo(eponMacRegTable)($2)
	lw	$2,16($sp)
	sll	$2,$2,2
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	sw	$0,4($2)
	lui	$2,%hi(eponMacRegTable)
	lw	$3,%lo(eponMacRegTable)($2)
	lw	$2,16($sp)
	sll	$2,$2,2
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	sw	$0,8($2)
	lw	$2,16($sp)
	addiu	$2,$2,1
	sw	$2,16($sp)
	lui	$2,%hi(eponMacRegTable)
	lw	$3,%lo(eponMacRegTable)($2)
	lw	$2,16($sp)
	sll	$2,$2,2
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	lui	$3,%hi(g_EPON_MAC_BASE)
	lw	$3,%lo(g_EPON_MAC_BASE)($3)
	addiu	$3,$3,24672
	sw	$3,0($2)
	lui	$2,%hi(eponMacRegTable)
	lw	$3,%lo(eponMacRegTable)($2)
	lw	$2,16($sp)
	sll	$2,$2,2
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	sw	$0,4($2)
	lui	$2,%hi(eponMacRegTable)
	lw	$3,%lo(eponMacRegTable)($2)
	lw	$2,16($sp)
	sll	$2,$2,2
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	li	$3,-1			# 0xffffffffffffffff
	sw	$3,8($2)
	lw	$2,16($sp)
	addiu	$2,$2,1
	sw	$2,16($sp)
	lui	$2,%hi(eponMacRegTable)
	lw	$3,%lo(eponMacRegTable)($2)
	lw	$2,16($sp)
	sll	$2,$2,2
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	lui	$3,%hi(g_EPON_MAC_BASE)
	lw	$3,%lo(g_EPON_MAC_BASE)($3)
	addiu	$3,$3,24676
	sw	$3,0($2)
	lui	$2,%hi(eponMacRegTable)
	lw	$3,%lo(eponMacRegTable)($2)
	lw	$2,16($sp)
	sll	$2,$2,2
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	sw	$0,4($2)
	lui	$2,%hi(eponMacRegTable)
	lw	$3,%lo(eponMacRegTable)($2)
	lw	$2,16($sp)
	sll	$2,$2,2
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	li	$3,319			# 0x13f
	sw	$3,8($2)
	lw	$2,16($sp)
	addiu	$2,$2,1
	sw	$2,16($sp)
	lui	$2,%hi(eponMacRegTable)
	lw	$3,%lo(eponMacRegTable)($2)
	lw	$2,16($sp)
	sll	$2,$2,2
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	lui	$3,%hi(g_EPON_MAC_BASE)
	lw	$3,%lo(g_EPON_MAC_BASE)($3)
	addiu	$3,$3,24680
	sw	$3,0($2)
	lui	$2,%hi(eponMacRegTable)
	lw	$3,%lo(eponMacRegTable)($2)
	lw	$2,16($sp)
	sll	$2,$2,2
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	li	$3,655360			# 0xa0000
	sw	$3,4($2)
	lui	$2,%hi(eponMacRegTable)
	lw	$3,%lo(eponMacRegTable)($2)
	lw	$2,16($sp)
	sll	$2,$2,2
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	sw	$0,8($2)
	lw	$2,16($sp)
	addiu	$2,$2,1
	sw	$2,16($sp)
	lui	$2,%hi(eponMacRegTable)
	lw	$3,%lo(eponMacRegTable)($2)
	lw	$2,16($sp)
	sll	$2,$2,2
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	lui	$3,%hi(g_EPON_MAC_BASE)
	lw	$3,%lo(g_EPON_MAC_BASE)($3)
	addiu	$3,$3,24684
	sw	$3,0($2)
	lui	$2,%hi(eponMacRegTable)
	lw	$3,%lo(eponMacRegTable)($2)
	lw	$2,16($sp)
	sll	$2,$2,2
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	sw	$0,4($2)
	lui	$2,%hi(eponMacRegTable)
	lw	$3,%lo(eponMacRegTable)($2)
	lw	$2,16($sp)
	sll	$2,$2,2
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	sw	$0,8($2)
	lw	$2,16($sp)
	addiu	$2,$2,1
	sw	$2,16($sp)
	lui	$2,%hi(eponMacRegTable)
	lw	$3,%lo(eponMacRegTable)($2)
	lw	$2,16($sp)
	sll	$2,$2,2
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	lui	$3,%hi(g_EPON_MAC_BASE)
	lw	$3,%lo(g_EPON_MAC_BASE)($3)
	addiu	$3,$3,24704
	sw	$3,0($2)
	lui	$2,%hi(eponMacRegTable)
	lw	$3,%lo(eponMacRegTable)($2)
	lw	$2,16($sp)
	sll	$2,$2,2
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	sw	$0,4($2)
	lui	$2,%hi(eponMacRegTable)
	lw	$3,%lo(eponMacRegTable)($2)
	lw	$2,16($sp)
	sll	$2,$2,2
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	sw	$0,8($2)
	lw	$2,16($sp)
	addiu	$2,$2,1
	sw	$2,16($sp)
	lui	$2,%hi(eponMacRegTable)
	lw	$3,%lo(eponMacRegTable)($2)
	lw	$2,16($sp)
	sll	$2,$2,2
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	lui	$3,%hi(g_EPON_MAC_BASE)
	lw	$3,%lo(g_EPON_MAC_BASE)($3)
	addiu	$3,$3,24752
	sw	$3,0($2)
	lui	$2,%hi(eponMacRegTable)
	lw	$3,%lo(eponMacRegTable)($2)
	lw	$2,16($sp)
	sll	$2,$2,2
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	sw	$0,4($2)
	lui	$2,%hi(eponMacRegTable)
	lw	$3,%lo(eponMacRegTable)($2)
	lw	$2,16($sp)
	sll	$2,$2,2
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	sw	$0,8($2)
	lw	$2,16($sp)
	addiu	$2,$2,1
	sw	$2,16($sp)
	lui	$2,%hi(eponMacRegTable)
	lw	$3,%lo(eponMacRegTable)($2)
	lw	$2,16($sp)
	sll	$2,$2,2
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	lui	$3,%hi(g_EPON_MAC_BASE)
	lw	$3,%lo(g_EPON_MAC_BASE)($3)
	addiu	$3,$3,24756
	sw	$3,0($2)
	lui	$2,%hi(eponMacRegTable)
	lw	$3,%lo(eponMacRegTable)($2)
	lw	$2,16($sp)
	sll	$2,$2,2
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	sw	$0,4($2)
	lui	$2,%hi(eponMacRegTable)
	lw	$3,%lo(eponMacRegTable)($2)
	lw	$2,16($sp)
	sll	$2,$2,2
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	sw	$0,8($2)
	lw	$2,16($sp)
	addiu	$2,$2,1
	sw	$2,16($sp)
	lui	$2,%hi(eponMacRegTable)
	lw	$3,%lo(eponMacRegTable)($2)
	lw	$2,16($sp)
	sll	$2,$2,2
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	lui	$3,%hi(g_EPON_MAC_BASE)
	lw	$3,%lo(g_EPON_MAC_BASE)($3)
	addiu	$3,$3,24760
	sw	$3,0($2)
	lui	$2,%hi(eponMacRegTable)
	lw	$3,%lo(eponMacRegTable)($2)
	lw	$2,16($sp)
	sll	$2,$2,2
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	sw	$0,4($2)
	lui	$2,%hi(eponMacRegTable)
	lw	$3,%lo(eponMacRegTable)($2)
	lw	$2,16($sp)
	sll	$2,$2,2
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	sw	$0,8($2)
	lw	$2,16($sp)
	addiu	$2,$2,1
	sw	$2,16($sp)
	lui	$2,%hi(eponMacRegTable)
	lw	$3,%lo(eponMacRegTable)($2)
	lw	$2,16($sp)
	sll	$2,$2,2
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	lui	$3,%hi(g_EPON_MAC_BASE)
	lw	$3,%lo(g_EPON_MAC_BASE)($3)
	addiu	$3,$3,24764
	sw	$3,0($2)
	lui	$2,%hi(eponMacRegTable)
	lw	$3,%lo(eponMacRegTable)($2)
	lw	$2,16($sp)
	sll	$2,$2,2
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	sw	$0,4($2)
	lui	$2,%hi(eponMacRegTable)
	lw	$3,%lo(eponMacRegTable)($2)
	lw	$2,16($sp)
	sll	$2,$2,2
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	sw	$0,8($2)
	lw	$2,16($sp)
	addiu	$2,$2,1
	sw	$2,16($sp)
	lui	$2,%hi(eponMacRegTable)
	lw	$3,%lo(eponMacRegTable)($2)
	lw	$2,16($sp)
	sll	$2,$2,2
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	lui	$3,%hi(g_EPON_MAC_BASE)
	lw	$3,%lo(g_EPON_MAC_BASE)($3)
	addiu	$3,$3,24768
	sw	$3,0($2)
	lui	$2,%hi(eponMacRegTable)
	lw	$3,%lo(eponMacRegTable)($2)
	lw	$2,16($sp)
	sll	$2,$2,2
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	sw	$0,4($2)
	lui	$2,%hi(eponMacRegTable)
	lw	$3,%lo(eponMacRegTable)($2)
	lw	$2,16($sp)
	sll	$2,$2,2
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	sw	$0,8($2)
	lw	$2,16($sp)
	addiu	$2,$2,1
	sw	$2,16($sp)
	lui	$2,%hi(eponMacRegTable)
	lw	$3,%lo(eponMacRegTable)($2)
	lw	$2,16($sp)
	sll	$2,$2,2
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	lui	$3,%hi(g_EPON_MAC_BASE)
	lw	$3,%lo(g_EPON_MAC_BASE)($3)
	addiu	$3,$3,24772
	sw	$3,0($2)
	lui	$2,%hi(eponMacRegTable)
	lw	$3,%lo(eponMacRegTable)($2)
	lw	$2,16($sp)
	sll	$2,$2,2
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	sw	$0,4($2)
	lui	$2,%hi(eponMacRegTable)
	lw	$3,%lo(eponMacRegTable)($2)
	lw	$2,16($sp)
	sll	$2,$2,2
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	sw	$0,8($2)
	lw	$2,16($sp)
	addiu	$2,$2,1
	sw	$2,16($sp)
	lui	$2,%hi(eponMacRegTable)
	lw	$3,%lo(eponMacRegTable)($2)
	lw	$2,16($sp)
	sll	$2,$2,2
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	lui	$3,%hi(g_EPON_MAC_BASE)
	lw	$3,%lo(g_EPON_MAC_BASE)($3)
	addiu	$3,$3,24784
	sw	$3,0($2)
	lui	$2,%hi(eponMacRegTable)
	lw	$3,%lo(eponMacRegTable)($2)
	lw	$2,16($sp)
	sll	$2,$2,2
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	li	$3,606732288			# 0x242a0000
	ori	$3,$3,0x3e8
	sw	$3,4($2)
	lui	$2,%hi(eponMacRegTable)
	lw	$3,%lo(eponMacRegTable)($2)
	lw	$2,16($sp)
	sll	$2,$2,2
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	li	$3,-65536			# 0xffffffffffff0000
	ori	$3,$3,0xfff
	sw	$3,8($2)
	lw	$2,16($sp)
	addiu	$2,$2,1
	sw	$2,16($sp)
	lui	$2,%hi(eponMacRegTable)
	lw	$3,%lo(eponMacRegTable)($2)
	lw	$2,16($sp)
	sll	$2,$2,2
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	lui	$3,%hi(g_EPON_MAC_BASE)
	lw	$3,%lo(g_EPON_MAC_BASE)($3)
	addiu	$3,$3,24788
	sw	$3,0($2)
	lui	$2,%hi(eponMacRegTable)
	lw	$3,%lo(eponMacRegTable)($2)
	lw	$2,16($sp)
	sll	$2,$2,2
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	sw	$0,4($2)
	lui	$2,%hi(eponMacRegTable)
	lw	$3,%lo(eponMacRegTable)($2)
	lw	$2,16($sp)
	sll	$2,$2,2
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	li	$3,65535			# 0xffff
	sw	$3,8($2)
	lw	$2,16($sp)
	addiu	$2,$2,1
	sw	$2,16($sp)
	lui	$2,%hi(eponMacRegTable)
	lw	$3,%lo(eponMacRegTable)($2)
	lw	$2,16($sp)
	sll	$2,$2,2
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	lui	$3,%hi(g_EPON_MAC_BASE)
	lw	$3,%lo(g_EPON_MAC_BASE)($3)
	addiu	$3,$3,24792
	sw	$3,0($2)
	lui	$2,%hi(eponMacRegTable)
	lw	$3,%lo(eponMacRegTable)($2)
	lw	$2,16($sp)
	sll	$2,$2,2
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	li	$3,638713856			# 0x26120000
	ori	$3,$3,0x40c
	sw	$3,4($2)
	lui	$2,%hi(eponMacRegTable)
	lw	$3,%lo(eponMacRegTable)($2)
	lw	$2,16($sp)
	sll	$2,$2,2
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	li	$3,-193			# 0xffffffffffffff3f
	sw	$3,8($2)
	lw	$2,16($sp)
	addiu	$2,$2,1
	sw	$2,16($sp)
	lui	$2,%hi(eponMacRegTable)
	lw	$3,%lo(eponMacRegTable)($2)
	lw	$2,16($sp)
	sll	$2,$2,2
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	lui	$3,%hi(g_EPON_MAC_BASE)
	lw	$3,%lo(g_EPON_MAC_BASE)($3)
	addiu	$3,$3,24796
	sw	$3,0($2)
	lui	$2,%hi(eponMacRegTable)
	lw	$3,%lo(eponMacRegTable)($2)
	lw	$2,16($sp)
	sll	$2,$2,2
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	li	$3,8224			# 0x2020
	sw	$3,4($2)
	lui	$2,%hi(eponMacRegTable)
	lw	$3,%lo(eponMacRegTable)($2)
	lw	$2,16($sp)
	sll	$2,$2,2
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	li	$3,65535			# 0xffff
	sw	$3,8($2)
	lw	$2,16($sp)
	addiu	$2,$2,1
	sw	$2,16($sp)
	lui	$2,%hi(eponMacRegTable)
	lw	$3,%lo(eponMacRegTable)($2)
	lw	$2,16($sp)
	sll	$2,$2,2
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	lui	$3,%hi(g_EPON_MAC_BASE)
	lw	$3,%lo(g_EPON_MAC_BASE)($3)
	addiu	$3,$3,24800
	sw	$3,0($2)
	lui	$2,%hi(eponMacRegTable)
	lw	$3,%lo(eponMacRegTable)($2)
	lw	$2,16($sp)
	sll	$2,$2,2
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	li	$3,8			# 0x8
	sw	$3,4($2)
	lui	$2,%hi(eponMacRegTable)
	lw	$3,%lo(eponMacRegTable)($2)
	lw	$2,16($sp)
	sll	$2,$2,2
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	li	$3,255			# 0xff
	sw	$3,8($2)
	lw	$2,16($sp)
	addiu	$2,$2,1
	sw	$2,16($sp)
	lui	$2,%hi(eponMacRegTable)
	lw	$3,%lo(eponMacRegTable)($2)
	lw	$2,16($sp)
	sll	$2,$2,2
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	lui	$3,%hi(g_EPON_MAC_BASE)
	lw	$3,%lo(g_EPON_MAC_BASE)($3)
	addiu	$3,$3,24804
	sw	$3,0($2)
	lui	$2,%hi(eponMacRegTable)
	lw	$3,%lo(eponMacRegTable)($2)
	lw	$2,16($sp)
	sll	$2,$2,2
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	li	$3,62455808			# 0x3b90000
	ori	$3,$3,0xaca0
	sw	$3,4($2)
	lui	$2,%hi(eponMacRegTable)
	lw	$3,%lo(eponMacRegTable)($2)
	lw	$2,16($sp)
	sll	$2,$2,2
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	li	$3,-1			# 0xffffffffffffffff
	sw	$3,8($2)
	lw	$2,16($sp)
	addiu	$2,$2,1
	sw	$2,16($sp)
	lui	$2,%hi(eponMacRegTable)
	lw	$3,%lo(eponMacRegTable)($2)
	lw	$2,16($sp)
	sll	$2,$2,2
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	lui	$3,%hi(g_EPON_MAC_BASE)
	lw	$3,%lo(g_EPON_MAC_BASE)($3)
	addiu	$3,$3,24808
	sw	$3,0($2)
	lui	$2,%hi(eponMacRegTable)
	lw	$3,%lo(eponMacRegTable)($2)
	lw	$2,16($sp)
	sll	$2,$2,2
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	li	$3,3080192			# 0x2f0000
	ori	$3,$3,0xaf08
	sw	$3,4($2)
	lui	$2,%hi(eponMacRegTable)
	lw	$3,%lo(eponMacRegTable)($2)
	lw	$2,16($sp)
	sll	$2,$2,2
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	li	$3,16711680			# 0xff0000
	ori	$3,$3,0xffff
	sw	$3,8($2)
	lw	$2,16($sp)
	addiu	$2,$2,1
	sw	$2,16($sp)
	lui	$2,%hi(eponMacRegTable)
	lw	$3,%lo(eponMacRegTable)($2)
	lw	$2,16($sp)
	sll	$2,$2,2
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	lui	$3,%hi(g_EPON_MAC_BASE)
	lw	$3,%lo(g_EPON_MAC_BASE)($3)
	addiu	$3,$3,24812
	sw	$3,0($2)
	lui	$2,%hi(eponMacRegTable)
	lw	$3,%lo(eponMacRegTable)($2)
	lw	$2,16($sp)
	sll	$2,$2,2
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	li	$3,62455808			# 0x3b90000
	ori	$3,$3,0xaca0
	sw	$3,4($2)
	lui	$2,%hi(eponMacRegTable)
	lw	$3,%lo(eponMacRegTable)($2)
	lw	$2,16($sp)
	sll	$2,$2,2
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	li	$3,-1			# 0xffffffffffffffff
	sw	$3,8($2)
	lw	$2,16($sp)
	addiu	$2,$2,1
	sw	$2,16($sp)
	lui	$2,%hi(eponMacRegTable)
	lw	$3,%lo(eponMacRegTable)($2)
	lw	$2,16($sp)
	sll	$2,$2,2
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	lui	$3,%hi(g_EPON_MAC_BASE)
	lw	$3,%lo(g_EPON_MAC_BASE)($3)
	addiu	$3,$3,24816
	sw	$3,0($2)
	lui	$2,%hi(eponMacRegTable)
	lw	$3,%lo(eponMacRegTable)($2)
	lw	$2,16($sp)
	sll	$2,$2,2
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	li	$3,1024			# 0x400
	sw	$3,4($2)
	lui	$2,%hi(eponMacRegTable)
	lw	$3,%lo(eponMacRegTable)($2)
	lw	$2,16($sp)
	sll	$2,$2,2
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	li	$3,65535			# 0xffff
	sw	$3,8($2)
	lw	$2,16($sp)
	addiu	$2,$2,1
	sw	$2,16($sp)
	lui	$2,%hi(eponMacRegTable)
	lw	$3,%lo(eponMacRegTable)($2)
	lw	$2,16($sp)
	sll	$2,$2,2
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	lui	$3,%hi(g_EPON_MAC_BASE)
	lw	$3,%lo(g_EPON_MAC_BASE)($3)
	addiu	$3,$3,24820
	sw	$3,0($2)
	lui	$2,%hi(eponMacRegTable)
	lw	$3,%lo(eponMacRegTable)($2)
	lw	$2,16($sp)
	sll	$2,$2,2
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	li	$3,5177344			# 0x4f0000
	ori	$3,$3,0xfff1
	sw	$3,4($2)
	lui	$2,%hi(eponMacRegTable)
	lw	$3,%lo(eponMacRegTable)($2)
	lw	$2,16($sp)
	sll	$2,$2,2
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	li	$3,-1			# 0xffffffffffffffff
	sw	$3,8($2)
	lw	$2,16($sp)
	addiu	$2,$2,1
	sw	$2,16($sp)
	lui	$2,%hi(eponMacRegTable)
	lw	$3,%lo(eponMacRegTable)($2)
	lw	$2,16($sp)
	sll	$2,$2,2
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	lui	$3,%hi(g_EPON_MAC_BASE)
	lw	$3,%lo(g_EPON_MAC_BASE)($3)
	addiu	$3,$3,24824
	sw	$3,0($2)
	lui	$2,%hi(eponMacRegTable)
	lw	$3,%lo(eponMacRegTable)($2)
	lw	$2,16($sp)
	sll	$2,$2,2
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	li	$3,6			# 0x6
	sw	$3,4($2)
	lui	$2,%hi(eponMacRegTable)
	lw	$3,%lo(eponMacRegTable)($2)
	lw	$2,16($sp)
	sll	$2,$2,2
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	li	$3,-1			# 0xffffffffffffffff
	sw	$3,8($2)
	lw	$2,16($sp)
	addiu	$2,$2,1
	sw	$2,16($sp)
	lui	$2,%hi(eponMacRegTable)
	lw	$3,%lo(eponMacRegTable)($2)
	lw	$2,16($sp)
	sll	$2,$2,2
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	lui	$3,%hi(g_EPON_MAC_BASE)
	lw	$3,%lo(g_EPON_MAC_BASE)($3)
	addiu	$3,$3,24832
	sw	$3,0($2)
	lui	$2,%hi(eponMacRegTable)
	lw	$3,%lo(eponMacRegTable)($2)
	lw	$2,16($sp)
	sll	$2,$2,2
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	sw	$0,4($2)
	lui	$2,%hi(eponMacRegTable)
	lw	$3,%lo(eponMacRegTable)($2)
	lw	$2,16($sp)
	sll	$2,$2,2
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	li	$3,31			# 0x1f
	sw	$3,8($2)
	lw	$2,16($sp)
	addiu	$2,$2,1
	sw	$2,16($sp)
	lui	$2,%hi(eponMacRegTable)
	lw	$3,%lo(eponMacRegTable)($2)
	lw	$2,16($sp)
	sll	$2,$2,2
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	lui	$3,%hi(g_EPON_MAC_BASE)
	lw	$3,%lo(g_EPON_MAC_BASE)($3)
	addiu	$3,$3,24836
	sw	$3,0($2)
	lui	$2,%hi(eponMacRegTable)
	lw	$3,%lo(eponMacRegTable)($2)
	lw	$2,16($sp)
	sll	$2,$2,2
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	sw	$0,4($2)
	lui	$2,%hi(eponMacRegTable)
	lw	$3,%lo(eponMacRegTable)($2)
	lw	$2,16($sp)
	sll	$2,$2,2
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	sw	$0,8($2)
	lw	$2,16($sp)
	addiu	$2,$2,1
	sw	$2,16($sp)
	lui	$2,%hi(eponMacRegTable)
	lw	$3,%lo(eponMacRegTable)($2)
	lw	$2,16($sp)
	sll	$2,$2,2
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	lui	$3,%hi(g_EPON_MAC_BASE)
	lw	$3,%lo(g_EPON_MAC_BASE)($3)
	addiu	$3,$3,24840
	sw	$3,0($2)
	lui	$2,%hi(eponMacRegTable)
	lw	$3,%lo(eponMacRegTable)($2)
	lw	$2,16($sp)
	sll	$2,$2,2
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	sw	$0,4($2)
	lui	$2,%hi(eponMacRegTable)
	lw	$3,%lo(eponMacRegTable)($2)
	lw	$2,16($sp)
	sll	$2,$2,2
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	sw	$0,8($2)
	lw	$2,16($sp)
	addiu	$2,$2,1
	sw	$2,16($sp)
	lui	$2,%hi(eponMacRegTable)
	lw	$3,%lo(eponMacRegTable)($2)
	lw	$2,16($sp)
	sll	$2,$2,2
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	lui	$3,%hi(g_EPON_MAC_BASE)
	lw	$3,%lo(g_EPON_MAC_BASE)($3)
	addiu	$3,$3,24844
	sw	$3,0($2)
	lui	$2,%hi(eponMacRegTable)
	lw	$3,%lo(eponMacRegTable)($2)
	lw	$2,16($sp)
	sll	$2,$2,2
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	sw	$0,4($2)
	lui	$2,%hi(eponMacRegTable)
	lw	$3,%lo(eponMacRegTable)($2)
	lw	$2,16($sp)
	sll	$2,$2,2
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	sw	$0,8($2)
	lw	$2,16($sp)
	addiu	$2,$2,1
	sw	$2,16($sp)
	lui	$2,%hi(eponMacRegTable)
	lw	$3,%lo(eponMacRegTable)($2)
	lw	$2,16($sp)
	sll	$2,$2,2
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	lui	$3,%hi(g_EPON_MAC_BASE)
	lw	$3,%lo(g_EPON_MAC_BASE)($3)
	addiu	$3,$3,24848
	sw	$3,0($2)
	lui	$2,%hi(eponMacRegTable)
	lw	$3,%lo(eponMacRegTable)($2)
	lw	$2,16($sp)
	sll	$2,$2,2
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	sw	$0,4($2)
	lui	$2,%hi(eponMacRegTable)
	lw	$3,%lo(eponMacRegTable)($2)
	lw	$2,16($sp)
	sll	$2,$2,2
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	sw	$0,8($2)
	lw	$2,16($sp)
	addiu	$2,$2,1
	sw	$2,16($sp)
	lui	$2,%hi(eponMacRegTable)
	lw	$3,%lo(eponMacRegTable)($2)
	lw	$2,16($sp)
	sll	$2,$2,2
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	lui	$3,%hi(g_EPON_MAC_BASE)
	lw	$3,%lo(g_EPON_MAC_BASE)($3)
	addiu	$3,$3,24852
	sw	$3,0($2)
	lui	$2,%hi(eponMacRegTable)
	lw	$3,%lo(eponMacRegTable)($2)
	lw	$2,16($sp)
	sll	$2,$2,2
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	sw	$0,4($2)
	lui	$2,%hi(eponMacRegTable)
	lw	$3,%lo(eponMacRegTable)($2)
	lw	$2,16($sp)
	sll	$2,$2,2
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	sw	$0,8($2)
	lw	$2,16($sp)
	addiu	$2,$2,1
	sw	$2,16($sp)
	lui	$2,%hi(eponMacRegTable)
	lw	$3,%lo(eponMacRegTable)($2)
	lw	$2,16($sp)
	sll	$2,$2,2
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	lui	$3,%hi(g_EPON_MAC_BASE)
	lw	$3,%lo(g_EPON_MAC_BASE)($3)
	addiu	$3,$3,24856
	sw	$3,0($2)
	lui	$2,%hi(eponMacRegTable)
	lw	$3,%lo(eponMacRegTable)($2)
	lw	$2,16($sp)
	sll	$2,$2,2
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	sw	$0,4($2)
	lui	$2,%hi(eponMacRegTable)
	lw	$3,%lo(eponMacRegTable)($2)
	lw	$2,16($sp)
	sll	$2,$2,2
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	sw	$0,8($2)
	lw	$2,16($sp)
	addiu	$2,$2,1
	sw	$2,16($sp)
	lui	$2,%hi(eponMacRegTable)
	lw	$3,%lo(eponMacRegTable)($2)
	lw	$2,16($sp)
	sll	$2,$2,2
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	lui	$3,%hi(g_EPON_MAC_BASE)
	lw	$3,%lo(g_EPON_MAC_BASE)($3)
	addiu	$3,$3,24860
	sw	$3,0($2)
	lui	$2,%hi(eponMacRegTable)
	lw	$3,%lo(eponMacRegTable)($2)
	lw	$2,16($sp)
	sll	$2,$2,2
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	sw	$0,4($2)
	lui	$2,%hi(eponMacRegTable)
	lw	$3,%lo(eponMacRegTable)($2)
	lw	$2,16($sp)
	sll	$2,$2,2
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	sw	$0,8($2)
	lw	$2,16($sp)
	addiu	$2,$2,1
	sw	$2,16($sp)
	lui	$2,%hi(eponMacRegTable)
	lw	$3,%lo(eponMacRegTable)($2)
	lw	$2,16($sp)
	sll	$2,$2,2
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	lui	$3,%hi(g_EPON_MAC_BASE)
	lw	$3,%lo(g_EPON_MAC_BASE)($3)
	addiu	$3,$3,24864
	sw	$3,0($2)
	lui	$2,%hi(eponMacRegTable)
	lw	$3,%lo(eponMacRegTable)($2)
	lw	$2,16($sp)
	sll	$2,$2,2
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	sw	$0,4($2)
	lui	$2,%hi(eponMacRegTable)
	lw	$3,%lo(eponMacRegTable)($2)
	lw	$2,16($sp)
	sll	$2,$2,2
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	sw	$0,8($2)
	lw	$2,16($sp)
	addiu	$2,$2,1
	sw	$2,16($sp)
	lui	$2,%hi(eponMacRegTable)
	lw	$3,%lo(eponMacRegTable)($2)
	lw	$2,16($sp)
	sll	$2,$2,2
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	lui	$3,%hi(g_EPON_MAC_BASE)
	lw	$3,%lo(g_EPON_MAC_BASE)($3)
	addiu	$3,$3,24868
	sw	$3,0($2)
	lui	$2,%hi(eponMacRegTable)
	lw	$3,%lo(eponMacRegTable)($2)
	lw	$2,16($sp)
	sll	$2,$2,2
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	sw	$0,4($2)
	lui	$2,%hi(eponMacRegTable)
	lw	$3,%lo(eponMacRegTable)($2)
	lw	$2,16($sp)
	sll	$2,$2,2
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	sw	$0,8($2)
	lw	$2,16($sp)
	addiu	$2,$2,1
	sw	$2,16($sp)
	lui	$2,%hi(eponMacRegTable)
	lw	$3,%lo(eponMacRegTable)($2)
	lw	$2,16($sp)
	sll	$2,$2,2
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	lui	$3,%hi(g_EPON_MAC_BASE)
	lw	$3,%lo(g_EPON_MAC_BASE)($3)
	addiu	$3,$3,24872
	sw	$3,0($2)
	lui	$2,%hi(eponMacRegTable)
	lw	$3,%lo(eponMacRegTable)($2)
	lw	$2,16($sp)
	sll	$2,$2,2
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	sw	$0,4($2)
	lui	$2,%hi(eponMacRegTable)
	lw	$3,%lo(eponMacRegTable)($2)
	lw	$2,16($sp)
	sll	$2,$2,2
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	sw	$0,8($2)
	lw	$2,16($sp)
	addiu	$2,$2,1
	sw	$2,16($sp)
	lui	$2,%hi(eponMacRegTable)
	lw	$3,%lo(eponMacRegTable)($2)
	lw	$2,16($sp)
	sll	$2,$2,2
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	lui	$3,%hi(g_EPON_MAC_BASE)
	lw	$3,%lo(g_EPON_MAC_BASE)($3)
	addiu	$3,$3,24876
	sw	$3,0($2)
	lui	$2,%hi(eponMacRegTable)
	lw	$3,%lo(eponMacRegTable)($2)
	lw	$2,16($sp)
	sll	$2,$2,2
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	li	$3,65535			# 0xffff
	sw	$3,4($2)
	lui	$2,%hi(eponMacRegTable)
	lw	$3,%lo(eponMacRegTable)($2)
	lw	$2,16($sp)
	sll	$2,$2,2
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	sw	$0,8($2)
	lw	$2,16($sp)
	addiu	$2,$2,1
	sw	$2,16($sp)
	lui	$2,%hi(eponMacRegTable)
	lw	$3,%lo(eponMacRegTable)($2)
	lw	$2,16($sp)
	sll	$2,$2,2
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	lui	$3,%hi(g_EPON_MAC_BASE)
	lw	$3,%lo(g_EPON_MAC_BASE)($3)
	addiu	$3,$3,24880
	sw	$3,0($2)
	lui	$2,%hi(eponMacRegTable)
	lw	$3,%lo(eponMacRegTable)($2)
	lw	$2,16($sp)
	sll	$2,$2,2
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	sw	$0,4($2)
	lui	$2,%hi(eponMacRegTable)
	lw	$3,%lo(eponMacRegTable)($2)
	lw	$2,16($sp)
	sll	$2,$2,2
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	sw	$0,8($2)
	lw	$2,16($sp)
	addiu	$2,$2,1
	sw	$2,16($sp)
	lui	$2,%hi(eponMacRegTable)
	lw	$3,%lo(eponMacRegTable)($2)
	lw	$2,16($sp)
	sll	$2,$2,2
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	lui	$3,%hi(g_EPON_MAC_BASE)
	lw	$3,%lo(g_EPON_MAC_BASE)($3)
	addiu	$3,$3,24884
	sw	$3,0($2)
	lui	$2,%hi(eponMacRegTable)
	lw	$3,%lo(eponMacRegTable)($2)
	lw	$2,16($sp)
	sll	$2,$2,2
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	sw	$0,4($2)
	lui	$2,%hi(eponMacRegTable)
	lw	$3,%lo(eponMacRegTable)($2)
	lw	$2,16($sp)
	sll	$2,$2,2
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	sw	$0,8($2)
	lw	$2,16($sp)
	addiu	$2,$2,1
	sw	$2,16($sp)
	lui	$2,%hi(eponMacRegTable)
	lw	$3,%lo(eponMacRegTable)($2)
	lw	$2,16($sp)
	sll	$2,$2,2
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	lui	$3,%hi(g_EPON_MAC_BASE)
	lw	$3,%lo(g_EPON_MAC_BASE)($3)
	addiu	$3,$3,24928
	sw	$3,0($2)
	lui	$2,%hi(eponMacRegTable)
	lw	$3,%lo(eponMacRegTable)($2)
	lw	$2,16($sp)
	sll	$2,$2,2
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	sw	$0,4($2)
	lui	$2,%hi(eponMacRegTable)
	lw	$3,%lo(eponMacRegTable)($2)
	lw	$2,16($sp)
	sll	$2,$2,2
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	sw	$0,8($2)
	lw	$2,16($sp)
	addiu	$2,$2,1
	sw	$2,16($sp)
	lui	$2,%hi(eponMacRegTable)
	lw	$3,%lo(eponMacRegTable)($2)
	lw	$2,16($sp)
	sll	$2,$2,2
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	lui	$3,%hi(g_EPON_MAC_BASE)
	lw	$3,%lo(g_EPON_MAC_BASE)($3)
	addiu	$3,$3,24932
	sw	$3,0($2)
	lui	$2,%hi(eponMacRegTable)
	lw	$3,%lo(eponMacRegTable)($2)
	lw	$2,16($sp)
	sll	$2,$2,2
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	li	$3,2			# 0x2
	sw	$3,4($2)
	lui	$2,%hi(eponMacRegTable)
	lw	$3,%lo(eponMacRegTable)($2)
	lw	$2,16($sp)
	sll	$2,$2,2
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	sw	$0,8($2)
	lw	$2,16($sp)
	addiu	$2,$2,1
	sw	$2,16($sp)
	lui	$2,%hi(eponMacRegTable)
	lw	$3,%lo(eponMacRegTable)($2)
	lw	$2,16($sp)
	sll	$2,$2,2
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	sw	$0,0($2)
	lui	$2,%hi(eponMacRegTable)
	lw	$3,%lo(eponMacRegTable)($2)
	lw	$2,16($sp)
	sll	$2,$2,2
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	sw	$0,8($2)
	lw	$31,36($sp)
	addiu	$sp,$sp,40
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	eponMacTableInit
	.size	eponMacTableInit, .-eponMacTableInit
	.section	.text.eponMacTableExit,"ax",@progbits
	.align	2
	.globl	eponMacTableExit
	.set	nomips16
	.set	nomicromips
	.ent	eponMacTableExit
	.type	eponMacTableExit, @function
eponMacTableExit:
	.frame	$sp,24,$31		# vars= 0, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-24
	sw	$31,20($sp)
	lui	$2,%hi(eponMacRegTable)
	lw	$2,%lo(eponMacRegTable)($2)
	move	$4,$2
	lui	$2,%hi(kfree)
	addiu	$2,$2,%lo(kfree)
	jalr	$2
	nop

	lui	$2,%hi(eponMacRegTable)
	sw	$0,%lo(eponMacRegTable)($2)
	lw	$31,20($sp)
	addiu	$sp,$sp,24
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	eponMacTableExit
	.size	eponMacTableExit, .-eponMacTableExit
	.rdata
	.align	2
$LC125:
	.ascii	"ERR: dump epon mac register fail\000"
	.align	2
$LC126:
	.ascii	"Dump EPON MAC REG\000"
	.align	2
$LC127:
	.ascii	"\015\012 %X : %X\000"
	.section	.text.eponMacDumpAllReg,"ax",@progbits
	.align	2
	.globl	eponMacDumpAllReg
	.set	nomips16
	.set	nomicromips
	.ent	eponMacDumpAllReg
	.type	eponMacDumpAllReg, @function
eponMacDumpAllReg:
	.frame	$sp,32,$31		# vars= 8, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-32
	sw	$31,28($sp)
	sw	$0,16($sp)
	sw	$0,20($sp)
	lui	$2,%hi(eponMacRegTable)
	lw	$2,%lo(eponMacRegTable)($2)
	bne	$2,$0,$L509
	nop

	li	$4,1			# 0x1
	lui	$2,%hi($LC125)
	addiu	$5,$2,%lo($LC125)
	lui	$2,%hi(dbgtoMem)
	addiu	$2,$2,%lo(dbgtoMem)
	jalr	$2
	nop

	li	$2,-1			# 0xffffffffffffffff
	j	$L510
	nop

$L509:
	li	$4,4			# 0x4
	lui	$2,%hi($LC126)
	addiu	$5,$2,%lo($LC126)
	lui	$2,%hi(dbgtoMem)
	addiu	$2,$2,%lo(dbgtoMem)
	jalr	$2
	nop

	sw	$0,16($sp)
	j	$L511
	nop

$L512:
	lui	$2,%hi(eponMacRegTable)
	lw	$3,%lo(eponMacRegTable)($2)
	lw	$2,16($sp)
	sll	$2,$2,2
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	lw	$2,0($2)
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	sw	$2,20($sp)
	lui	$2,%hi(eponMacRegTable)
	lw	$3,%lo(eponMacRegTable)($2)
	lw	$2,16($sp)
	sll	$2,$2,2
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	lw	$3,0($2)
	lui	$2,%hi($LC127)
	addiu	$4,$2,%lo($LC127)
	move	$5,$3
	lw	$6,20($sp)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lw	$2,16($sp)
	addiu	$2,$2,1
	sw	$2,16($sp)
$L511:
	lui	$2,%hi(eponMacRegTable)
	lw	$3,%lo(eponMacRegTable)($2)
	lw	$2,16($sp)
	sll	$2,$2,2
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	lw	$2,0($2)
	bne	$2,$0,$L512
	nop

	move	$2,$0
$L510:
	lw	$31,28($sp)
	addiu	$sp,$sp,32
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	eponMacDumpAllReg
	.size	eponMacDumpAllReg, .-eponMacDumpAllReg
	.section	.text.doEponHwtest,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	doEponHwtest
	.type	doEponHwtest, @function
doEponHwtest:
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
	lui	$2,%hi(eponHwtestCmds)
	addiu	$4,$2,%lo(eponHwtestCmds)
	lw	$5,24($sp)
	lw	$6,28($sp)
	lw	$7,32($sp)
	lui	$2,%hi(subcmd)
	addiu	$2,$2,%lo(subcmd)
	jalr	$2
	nop

	lw	$31,20($sp)
	addiu	$sp,$sp,24
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	doEponHwtest
	.size	doEponHwtest, .-doEponHwtest
	.rdata
	.align	2
$LC128:
	.ascii	"\015\012regtest <times>\000"
	.section	.text.doEponRegtest,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	doEponRegtest
	.type	doEponRegtest, @function
doEponRegtest:
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
	sw	$0,16($sp)
	lw	$2,32($sp)
	slt	$2,$2,2
	beq	$2,$0,$L516
	nop

	lui	$2,%hi($LC128)
	addiu	$4,$2,%lo($LC128)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	move	$2,$0
	j	$L517
	nop

$L516:
	lw	$2,36($sp)
	addiu	$2,$2,4
	lw	$2,0($2)
	move	$4,$2
	move	$5,$0
	li	$6,16			# 0x10
	lui	$2,%hi(simple_strtoul)
	addiu	$2,$2,%lo(simple_strtoul)
	jalr	$2
	nop

	sw	$2,16($sp)
	lw	$4,16($sp)
	lui	$2,%hi(eponMacRegTest)
	addiu	$2,$2,%lo(eponMacRegTest)
	jalr	$2
	nop

	move	$2,$0
$L517:
	lw	$31,28($sp)
	addiu	$sp,$sp,32
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	doEponRegtest
	.size	doEponRegtest, .-doEponRegtest
	.rdata
	.align	2
$LC129:
	.ascii	"EPON Register default value verifycation is failure!!\015"
	.ascii	"\012\000"
	.align	2
$LC130:
	.ascii	"EPON Register default value verifycation is ok!!\015\012"
	.ascii	"\000"
	.section	.text.doEponRegDefCheck,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	doEponRegDefCheck
	.type	doEponRegDefCheck, @function
doEponRegDefCheck:
	.frame	$sp,24,$31		# vars= 0, regs= 2/0, args= 16, gp= 0
	.mask	0x80010000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-24
	sw	$31,20($sp)
	sw	$16,16($sp)
	sw	$4,24($sp)
	sw	$5,28($sp)
	sw	$6,32($sp)
	lui	$2,%hi(eponMacSwReset)
	addiu	$2,$2,%lo(eponMacSwReset)
	jalr	$2
	nop

	lui	$2,%hi(eponMacRegTable)
	lw	$16,%lo(eponMacRegTable)($2)
	lui	$2,%hi(eponMacGetRegTblSize)
	addiu	$2,$2,%lo(eponMacGetRegTblSize)
	jalr	$2
	nop

	move	$4,$16
	move	$5,$2
	lui	$2,%hi(eponRegDefCheck)
	addiu	$2,$2,%lo(eponRegDefCheck)
	jalr	$2
	nop

	move	$3,$2
	li	$2,-1			# 0xffffffffffffffff
	bne	$3,$2,$L519
	nop

	lui	$2,%hi($LC129)
	addiu	$4,$2,%lo($LC129)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	j	$L520
	nop

$L519:
	lui	$2,%hi($LC130)
	addiu	$4,$2,%lo($LC130)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L520:
	move	$2,$0
	lw	$31,20($sp)
	lw	$16,16($sp)
	addiu	$sp,$sp,24
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	doEponRegDefCheck
	.size	doEponRegDefCheck, .-doEponRegDefCheck
	.local	timer_count.42748
	.comm	timer_count.42748,1,1
	.local	newDropCounter.42750
	.comm	newDropCounter.42750,4,4
	.local	oldDropCounter.42749
	.comm	oldDropCounter.42749,4,4
	.ident	"GCC: (Buildroot 2015.08.1) 4.9.3"
