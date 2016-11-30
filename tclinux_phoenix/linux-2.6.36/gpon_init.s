	.file	1 "gpon_init.c"
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
	.globl	g_gpon_mac_reg_BASE
	.section	.bss,"aw",@nobits
	.align	2
	.type	g_gpon_mac_reg_BASE, @object
	.size	g_gpon_mac_reg_BASE, 4
g_gpon_mac_reg_BASE:
	.space	4
	.rdata
	.align	2
$LC0:
	.byte	71
	.byte	67
	.byte	48
	.byte	48
	.byte	48
	.byte	54
	.byte	57
	.byte	70
	.byte	54
	.byte	0
	.align	2
$LC1:
	.byte	77
	.byte	84
	.byte	75
	.byte	71
	.byte	0
	.byte	0
	.byte	0
	.byte	1
	.section	.text.gpon_init_private_data,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	gpon_init_private_data
	.type	gpon_init_private_data, @function
gpon_init_private_data:
	.frame	$sp,64,$31		# vars= 40, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-64
	sw	$31,60($sp)
	sw	$4,64($sp)
	sw	$0,16($sp)
	lui	$2,%hi($LC0)
	addiu	$3,$2,%lo($LC0)
	lwl	$4,%lo($LC0)($2)
	lwr	$4,3($3)
	addiu	$3,$2,%lo($LC0)
	lwl	$5,4($3)
	move	$6,$5
	lwr	$6,7($3)
	move	$3,$6
	sw	$4,32($sp)
	sw	$3,36($sp)
	addiu	$2,$2,%lo($LC0)
	lbu	$3,8($2)
	sb	$3,40($sp)
	lbu	$2,9($2)
	sb	$2,41($sp)
	lui	$2,%hi($LC1)
	addiu	$3,$2,%lo($LC1)
	lwl	$4,%lo($LC1)($2)
	move	$5,$4
	lwr	$5,3($3)
	move	$3,$5
	addiu	$2,$2,%lo($LC1)
	lwl	$4,4($2)
	move	$6,$4
	lwr	$6,7($2)
	move	$2,$6
	sw	$3,44($sp)
	sw	$2,48($sp)
	lw	$2,64($sp)
	sw	$0,0($2)
	lw	$2,64($sp)
	addiu	$2,$2,205
	move	$4,$2
	move	$5,$0
	li	$6,8			# 0x8
	lui	$2,%hi(memset)
	addiu	$2,$2,%lo(memset)
	jalr	$2
	nop

	lw	$2,64($sp)
	addiu	$2,$2,216
	move	$4,$2
	move	$5,$0
	li	$6,10			# 0xa
	lui	$2,%hi(memset)
	addiu	$2,$2,%lo(memset)
	jalr	$2
	nop

	lw	$3,64($sp)
	lbu	$2,464($3)
	ins	$2,$0,6,1
	sb	$2,464($3)
	lw	$3,64($sp)
	lbu	$2,464($3)
	ins	$2,$0,5,1
	sb	$2,464($3)
	lw	$2,64($sp)
	li	$3,-1			# 0xffffffffffffffff
	sb	$3,204($2)
	lw	$2,64($sp)
	sb	$0,226($2)
	lw	$2,64($sp)
	li	$3,-1			# 0xffffffffffffffff
	sh	$3,252($2)
	lw	$2,64($sp)
	sb	$0,254($2)
	lw	$2,64($sp)
	li	$3,10000			# 0x2710
	sw	$3,256($2)
	lw	$2,64($sp)
	li	$3,100			# 0x64
	sw	$3,260($2)
	lw	$2,64($sp)
	sw	$0,264($2)
	lw	$2,64($sp)
	sw	$0,268($2)
	lw	$2,64($sp)
	sw	$0,272($2)
	lw	$2,64($sp)
	sb	$0,276($2)
	lw	$2,64($sp)
	sb	$0,277($2)
	lw	$2,64($sp)
	sb	$0,278($2)
	lw	$2,64($sp)
	sb	$0,279($2)
	lw	$2,64($sp)
	sb	$0,280($2)
	lw	$2,64($sp)
	sw	$0,284($2)
	lw	$2,64($sp)
	sw	$0,288($2)
	lw	$2,64($sp)
	sw	$0,300($2)
	lw	$3,64($sp)
	lhu	$2,448($3)
	ins	$2,$0,15,1
	sh	$2,448($3)
	lw	$3,64($sp)
	lhu	$2,448($3)
	ins	$2,$0,14,1
	sh	$2,448($3)
	lw	$3,64($sp)
	lhu	$2,448($3)
	ins	$2,$0,11,1
	sh	$2,448($3)
	lw	$3,64($sp)
	lhu	$2,448($3)
	ins	$2,$0,10,1
	sh	$2,448($3)
	lw	$3,64($sp)
	lhu	$2,448($3)
	ins	$2,$0,9,1
	sh	$2,448($3)
	lw	$2,64($sp)
	li	$3,28			# 0x1c
	sb	$3,294($2)
	lw	$2,64($sp)
	li	$3,1419			# 0x58b
	sh	$3,292($2)
	lw	$2,64($sp)
	sb	$0,295($2)
	lw	$3,64($sp)
	lhu	$2,448($3)
	li	$4,1			# 0x1
	ins	$2,$4,8,1
	sh	$2,448($3)
	lw	$3,64($sp)
	lhu	$2,448($3)
	ins	$2,$0,7,1
	sh	$2,448($3)
	lw	$2,64($sp)
	li	$3,-1			# 0xffffffffffffffff
	sw	$3,308($2)
	lw	$2,64($sp)
	sw	$0,340($2)
	lw	$2,64($sp)
	li	$3,32769			# 0x8001
	sw	$3,312($2)
	lw	$2,64($sp)
	li	$3,1			# 0x1
	sw	$3,336($2)
	lw	$2,64($sp)
	li	$3,-1			# 0xffffffffffffffff
	sw	$3,304($2)
	lw	$2,64($sp)
	addiu	$2,$2,460
	sw	$2,20($sp)
	lw	$2,64($sp)
	sw	$0,460($2)
	lw	$2,64($sp)
	li	$3,160			# 0xa0
	sh	$3,296($2)
	lw	$2,64($sp)
	addiu	$2,$2,456
	sw	$2,24($sp)
	lw	$2,64($sp)
	sw	$0,456($2)
	lw	$2,64($sp)
	addiu	$2,$2,452
	sw	$2,28($sp)
	lw	$2,64($sp)
	sw	$0,452($2)
	lw	$2,64($sp)
	addiu	$2,$2,468
	move	$4,$2
	lui	$2,%hi(__key.38640)
	addiu	$5,$2,%lo(__key.38640)
	lui	$2,%hi(__init_waitqueue_head)
	addiu	$2,$2,%lo(__init_waitqueue_head)
	jalr	$2
	nop

	lw	$3,64($sp)
	lbu	$2,464($3)
	ins	$2,$0,7,1
	sb	$2,464($3)
	lw	$2,64($sp)
	addiu	$3,$2,216
	addiu	$2,$sp,32
	move	$4,$3
	move	$5,$2
	li	$6,10			# 0xa
	lui	$2,%hi(memcpy)
	addiu	$2,$2,%lo(memcpy)
	jalr	$2
	nop

	lw	$2,64($sp)
	addiu	$3,$2,205
	addiu	$2,$sp,44
	move	$4,$3
	move	$5,$2
	li	$6,8			# 0x8
	lui	$2,%hi(memcpy)
	addiu	$2,$2,%lo(memcpy)
	jalr	$2
	nop

	lw	$2,64($sp)
	lui	$3,%hi(gponRecovery)
	addiu	$3,$3,%lo(gponRecovery)
	sw	$3,480($2)
	lui	$2,%hi(gpon_recovery_init)
	addiu	$2,$2,%lo(gpon_recovery_init)
	jalr	$2
	nop

	lw	$2,16($sp)
	lw	$31,60($sp)
	addiu	$sp,$sp,64
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	gpon_init_private_data
	.size	gpon_init_private_data, .-gpon_init_private_data
	.section	.text.register_gpon_isr,"ax",@progbits
	.align	2
	.globl	register_gpon_isr
	.set	nomips16
	.set	nomicromips
	.ent	register_gpon_isr
	.type	register_gpon_isr, @function
register_gpon_isr:
	.frame	$sp,72,$31		# vars= 48, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-72
	sw	$31,68($sp)
	li	$2,16			# 0x10
	sw	$2,16($sp)
	li	$2,3			# 0x3
	sw	$2,20($sp)
	lui	$2,%hi(gpon_isr)
	addiu	$2,$2,%lo(gpon_isr)
	sw	$2,24($sp)
	sw	$0,28($sp)
	li	$2,5			# 0x5
	sw	$2,32($sp)
	lw	$2,20($sp)
	sw	$2,40($sp)
	lw	$2,24($sp)
	sw	$2,44($sp)
	addiu	$2,$sp,32
	lw	$4,16($sp)
	move	$5,$0
	move	$6,$2
	lui	$2,%hi(__ECNT_HOOK)
	addiu	$2,$2,%lo(__ECNT_HOOK)
	jalr	$2
	nop

	sw	$2,28($sp)
	lw	$3,28($sp)
	li	$2,-1			# 0xffffffffffffffff
	beq	$3,$2,$L7
	nop

	lw	$2,36($sp)
	j	$L10
	nop

$L7:
	li	$2,-1			# 0xffffffffffffffff
$L10:
	lw	$31,68($sp)
	addiu	$sp,$sp,72
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	register_gpon_isr
	.size	register_gpon_isr, .-register_gpon_isr
	.section	.text.unregister_gpon_isr,"ax",@progbits
	.align	2
	.globl	unregister_gpon_isr
	.set	nomips16
	.set	nomicromips
	.ent	unregister_gpon_isr
	.type	unregister_gpon_isr, @function
unregister_gpon_isr:
	.frame	$sp,64,$31		# vars= 40, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-64
	sw	$31,60($sp)
	li	$2,16			# 0x10
	sw	$2,16($sp)
	li	$2,3			# 0x3
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
	lw	$3,24($sp)
	li	$2,-1			# 0xffffffffffffffff
	beq	$3,$2,$L12
	nop

	lw	$2,32($sp)
	j	$L15
	nop

$L12:
	li	$2,-1			# 0xffffffffffffffff
$L15:
	lw	$31,60($sp)
	addiu	$sp,$sp,64
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	unregister_gpon_isr
	.size	unregister_gpon_isr, .-unregister_gpon_isr
	.rdata
	.align	2
$LC2:
	.ascii	"[%lu0ms]ioremap the GPON base address failed.\012\000"
	.align	2
$LC3:
	.ascii	"[%lu0ms]GPON private data initialization failed.\012\000"
	.align	2
$LC4:
	.ascii	"[%lu0ms]GPON PLOAM initialization failed.\012\000"
	.align	2
$LC5:
	.ascii	"[%lu0ms]GPON ONU activation initialization failed.\012\000"
	.align	2
$LC6:
	.ascii	"[%lu0ms]GPON device initialization failed.\012\000"
	.align	2
$LC7:
	.ascii	"[%lu0ms]GPON power management initialization failed.\012"
	.ascii	"\000"
	.align	2
$LC8:
	.ascii	"[%lu0ms]GPON initialization done\012\000"
	.section	.text.gpon_init,"ax",@progbits
	.align	2
	.globl	gpon_init
	.set	nomips16
	.set	nomicromips
	.ent	gpon_init
	.type	gpon_init, @function
gpon_init:
	.frame	$sp,96,$31		# vars= 72, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-96
	sw	$31,92($sp)
	sw	$0,16($sp)
	li	$2,532021248			# 0x1fb60000
	sw	$2,20($sp)
	li	$2,17356			# 0x43cc
	sw	$2,24($sp)
	li	$2,512			# 0x200
	sw	$2,28($sp)
	lw	$2,20($sp)
	sw	$2,32($sp)
	lw	$2,24($sp)
	sw	$2,36($sp)
	lw	$2,28($sp)
	sw	$2,40($sp)
	move	$2,$0
	sw	$2,44($sp)
	lw	$2,44($sp)
	beq	$2,$0,$L18
	nop

	lw	$2,44($sp)
	j	$L19
	nop

$L18:
	lw	$4,20($sp)
	lw	$5,24($sp)
	lw	$6,28($sp)
	lui	$2,%hi(__ioremap)
	addiu	$2,$2,%lo(__ioremap)
	jalr	$2
	nop

$L19:
	lui	$3,%hi(g_gpon_mac_reg_BASE)
	sw	$2,%lo(g_gpon_mac_reg_BASE)($3)
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	bne	$2,$0,$L20
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
	lui	$2,%hi($LC2)
	addiu	$4,$2,%lo($LC2)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L21:
	li	$2,-12			# 0xfffffffffffffff4
	j	$L22
	nop

$L20:
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	move	$4,$2
	lui	$2,%hi(gpon_init_private_data)
	addiu	$2,$2,%lo(gpon_init_private_data)
	jalr	$2
	nop

	sw	$2,16($sp)
	lw	$2,16($sp)
	beq	$2,$0,$L23
	nop

	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	lhu	$2,-30816($2)
	andi	$2,$2,0x1
	beq	$2,$0,$L24
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

$L24:
	lw	$2,16($sp)
	j	$L22
	nop

$L23:
	lui	$2,%hi(ploam_init)
	addiu	$2,$2,%lo(ploam_init)
	jalr	$2
	nop

	sw	$2,16($sp)
	lw	$2,16($sp)
	beq	$2,$0,$L25
	nop

	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	lhu	$2,-30816($2)
	andi	$2,$2,0x1
	beq	$2,$0,$L26
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

$L26:
	lw	$2,16($sp)
	j	$L22
	nop

$L25:
	lui	$2,%hi(gpon_act_init)
	addiu	$2,$2,%lo(gpon_act_init)
	jalr	$2
	nop

	sw	$2,16($sp)
	lw	$2,16($sp)
	beq	$2,$0,$L27
	nop

	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	lhu	$2,-30816($2)
	andi	$2,$2,0x1
	beq	$2,$0,$L28
	nop

	lui	$2,%hi(jiffies)
	lw	$3,%lo(jiffies)($2)
	lui	$2,%hi($LC5)
	addiu	$4,$2,%lo($LC5)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L28:
	lw	$2,16($sp)
	j	$L22
	nop

$L27:
	lui	$2,%hi(gpon_dev_init)
	addiu	$2,$2,%lo(gpon_dev_init)
	jalr	$2
	nop

	sw	$2,16($sp)
	lw	$2,16($sp)
	beq	$2,$0,$L29
	nop

	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	lhu	$2,-30816($2)
	andi	$2,$2,0x1
	beq	$2,$0,$L30
	nop

	lui	$2,%hi(jiffies)
	lw	$3,%lo(jiffies)($2)
	lui	$2,%hi($LC6)
	addiu	$4,$2,%lo($LC6)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L30:
	lw	$2,16($sp)
	j	$L22
	nop

$L29:
	li	$4,1			# 0x1
	lui	$2,%hi(gponDevMbiStop)
	addiu	$2,$2,%lo(gponDevMbiStop)
	jalr	$2
	nop

	lui	$2,%hi(gpon_power_management_init)
	addiu	$2,$2,%lo(gpon_power_management_init)
	jalr	$2
	nop

	sw	$2,16($sp)
	lw	$2,16($sp)
	beq	$2,$0,$L31
	nop

	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	lhu	$2,-30816($2)
	andi	$2,$2,0x1
	beq	$2,$0,$L32
	nop

	lui	$2,%hi(jiffies)
	lw	$3,%lo(jiffies)($2)
	lui	$2,%hi($LC7)
	addiu	$4,$2,%lo($LC7)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L32:
	lw	$2,16($sp)
	j	$L22
	nop

$L31:
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lbu	$2,294($2)
	move	$4,$2
	lui	$2,%hi(gponDevSetInternalDelayFineTune)
	addiu	$2,$2,%lo(gponDevSetInternalDelayFineTune)
	jalr	$2
	nop

	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	addiu	$2,$2,312
	move	$4,$2
	lui	$2,%hi(gponDevSetSniffMode)
	addiu	$2,$2,%lo(gponDevSetSniffMode)
	jalr	$2
	nop

	move	$4,$0
	lui	$2,%hi(gponDevSetDbaBackdoor)
	addiu	$2,$2,%lo(gponDevSetDbaBackdoor)
	jalr	$2
	nop

	move	$4,$0
	lui	$2,%hi(gponDevSetDbaSlightModify)
	addiu	$2,$2,%lo(gponDevSetDbaSlightModify)
	jalr	$2
	nop

	li	$2,-1079902208			# 0xffffffffbfa20000
	ori	$2,$2,0x104
	sw	$2,48($sp)
	li	$2,4			# 0x4
	sw	$2,52($sp)
	lw	$2,48($sp)
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	sw	$2,56($sp)
	lw	$3,56($sp)
	lw	$2,52($sp)
	or	$2,$3,$2
	sw	$2,56($sp)
	lw	$2,48($sp)
	lw	$4,56($sp)
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	li	$2,-1079902208			# 0xffffffffbfa20000
	ori	$2,$2,0xe4
	sw	$2,60($sp)
	li	$2,256			# 0x100
	sw	$2,64($sp)
	lw	$2,60($sp)
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	sw	$2,68($sp)
	lw	$3,68($sp)
	lw	$2,64($sp)
	or	$2,$3,$2
	sw	$2,68($sp)
	lw	$2,60($sp)
	lw	$4,68($sp)
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	li	$2,-1079902208			# 0xffffffffbfa20000
	ori	$2,$2,0xec
	sw	$2,72($sp)
	li	$2,2			# 0x2
	sw	$2,76($sp)
	lw	$2,72($sp)
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	sw	$2,80($sp)
	lw	$3,80($sp)
	lw	$2,76($sp)
	or	$2,$3,$2
	sw	$2,80($sp)
	lw	$2,72($sp)
	lw	$4,80($sp)
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	lhu	$2,-30816($2)
	andi	$2,$2,0x40
	beq	$2,$0,$L33
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

$L33:
	lw	$2,16($sp)
$L22:
	lw	$31,92($sp)
	addiu	$sp,$sp,96
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	gpon_init
	.size	gpon_init, .-gpon_init
	.section	.text.gpon_deinit,"ax",@progbits
	.align	2
	.globl	gpon_deinit
	.set	nomips16
	.set	nomicromips
	.ent	gpon_deinit
	.type	gpon_deinit, @function
gpon_deinit:
	.frame	$sp,32,$31		# vars= 8, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-32
	sw	$31,28($sp)
	lui	$2,%hi(xpon_power_management_deinit)
	addiu	$2,$2,%lo(xpon_power_management_deinit)
	jalr	$2
	nop

	move	$2,$28
	lw	$2,20($2)
	move	$3,$2
	li	$2,134152192			# 0x7ff0000
	ori	$2,$2,0xff00
	and	$2,$3,$2
	beq	$2,$0,$L35
	nop

	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	addiu	$2,$2,8
	move	$4,$2
	lui	$2,%hi(del_timer)
	addiu	$2,$2,%lo(del_timer)
	jalr	$2
	nop

	j	$L36
	nop

$L35:
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	addiu	$2,$2,8
	move	$4,$2
	lui	$2,%hi(del_timer_sync)
	addiu	$2,$2,%lo(del_timer_sync)
	jalr	$2
	nop

$L36:
	move	$2,$28
	lw	$2,20($2)
	move	$3,$2
	li	$2,134152192			# 0x7ff0000
	ori	$2,$2,0xff00
	and	$2,$3,$2
	beq	$2,$0,$L37
	nop

	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	addiu	$2,$2,36
	move	$4,$2
	lui	$2,%hi(del_timer)
	addiu	$2,$2,%lo(del_timer)
	jalr	$2
	nop

	j	$L38
	nop

$L37:
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	addiu	$2,$2,36
	move	$4,$2
	lui	$2,%hi(del_timer_sync)
	addiu	$2,$2,%lo(del_timer_sync)
	jalr	$2
	nop

$L38:
	move	$2,$28
	lw	$2,20($2)
	move	$3,$2
	li	$2,134152192			# 0x7ff0000
	ori	$2,$2,0xff00
	and	$2,$3,$2
	beq	$2,$0,$L39
	nop

	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	addiu	$2,$2,64
	move	$4,$2
	lui	$2,%hi(del_timer)
	addiu	$2,$2,%lo(del_timer)
	jalr	$2
	nop

	j	$L40
	nop

$L39:
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	addiu	$2,$2,64
	move	$4,$2
	lui	$2,%hi(del_timer_sync)
	addiu	$2,$2,%lo(del_timer_sync)
	jalr	$2
	nop

$L40:
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	beq	$2,$0,$L41
	nop

	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	sw	$2,16($sp)
	lw	$2,16($sp)
	sw	$2,20($sp)
	move	$2,$0
	bne	$2,$0,$L43
	nop

	lw	$4,16($sp)
	lui	$2,%hi(__iounmap)
	addiu	$2,$2,%lo(__iounmap)
	jalr	$2
	nop

$L43:
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	sw	$0,%lo(g_gpon_mac_reg_BASE)($2)
$L41:
	lui	$2,%hi(gpon_act_deinit)
	addiu	$2,$2,%lo(gpon_act_deinit)
	jalr	$2
	nop

	lw	$31,28($sp)
	addiu	$sp,$sp,32
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	gpon_deinit
	.size	gpon_deinit, .-gpon_deinit
	.local	__key.38640
	.comm	__key.38640,0,4
	.ident	"GCC: (Buildroot 2015.08.1) 4.9.3"
