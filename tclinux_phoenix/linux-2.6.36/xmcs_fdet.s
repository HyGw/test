	.file	1 "xmcs_fdet.c"
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
	.section	.text.xmcs_event_check_valid,"ax",@progbits
	.align	2
	.globl	xmcs_event_check_valid
	.set	nomips16
	.set	nomicromips
	.ent	xmcs_event_check_valid
	.type	xmcs_event_check_valid, @function
xmcs_event_check_valid:
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	sw	$4,0($sp)
	sw	$5,4($sp)
	sw	$6,8($sp)
	li	$2,1			# 0x1
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	xmcs_event_check_valid
	.size	xmcs_event_check_valid, .-xmcs_event_check_valid
	.rdata
	.align	2
$LC0:
	.ascii	"[%lu0ms]xmcs_report_event begin ,type %d id %d value %u\012"
	.ascii	"\000"
	.align	2
$LC1:
	.ascii	"[%lu0ms]WARNNING: xmcs Event exceed 16,type %d id %d val"
	.ascii	"ue %u\012\000"
	.align	2
$LC2:
	.ascii	"ERROR: xmcs Event exceed max num,type %d id %d value %u\012"
	.ascii	"\000"
	.section	.text.xmcs_report_event,"ax",@progbits
	.align	2
	.globl	xmcs_report_event
	.set	nomips16
	.set	nomicromips
	.ent	xmcs_report_event
	.type	xmcs_report_event, @function
xmcs_report_event:
	.frame	$sp,72,$31		# vars= 40, regs= 1/0, args= 24, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-72
	sw	$31,68($sp)
	sw	$4,72($sp)
	sw	$5,76($sp)
	sw	$6,80($sp)
	li	$2,-1078984704			# 0xffffffffbfb00000
	ori	$4,$2,0x70
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	andi	$2,$2,0x3
	sw	$2,24($sp)
	lui	$2,%hi(gpPonSysData)
	lw	$2,%lo(gpPonSysData)($2)
	lw	$3,8($2)
	li	$2,1			# 0x1
	bne	$3,$2,$L4
	nop

	j	$L3
	nop

$L4:
	lw	$4,72($sp)
	lw	$5,76($sp)
	lw	$6,80($sp)
	lui	$2,%hi(xmcs_event_check_valid)
	addiu	$2,$2,%lo(xmcs_event_check_valid)
	jalr	$2
	nop

	bne	$2,$0,$L6
	nop

	j	$L3
	nop

$L6:
	lw	$2,24($sp)
	bne	$2,$0,$L7
	nop

	lw	$3,72($sp)
	li	$2,2			# 0x2
	bne	$3,$2,$L7
	nop

	j	$L3
	nop

$L7:
	lw	$3,24($sp)
	li	$2,1			# 0x1
	bne	$3,$2,$L8
	nop

	lw	$3,72($sp)
	li	$2,1			# 0x1
	bne	$3,$2,$L8
	nop

	j	$L3
	nop

$L8:
	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	lhu	$2,-30816($2)
	andi	$2,$2,0x4
	beq	$2,$0,$L9
	nop

	lui	$2,%hi(jiffies)
	lw	$3,%lo(jiffies)($2)
	lw	$2,80($sp)
	sw	$2,16($sp)
	lui	$2,%hi($LC0)
	addiu	$4,$2,%lo($LC0)
	move	$5,$3
	lw	$6,72($sp)
	lw	$7,76($sp)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L9:
	lui	$2,%hi(gpMcsPriv)
	lw	$2,%lo(gpMcsPriv)($2)
	addiu	$2,$2,408
	sw	$2,36($sp)
	lw	$2,36($sp)
	move	$4,$2
	lui	$2,%hi(_raw_spin_lock_irqsave)
	addiu	$2,$2,%lo(_raw_spin_lock_irqsave)
	jalr	$2
	nop

	sw	$2,28($sp)
	lui	$2,%hi(gpMcsPriv)
	lw	$2,%lo(gpMcsPriv)($2)
	lw	$2,400($2)
	sw	$2,32($sp)
	lw	$2,32($sp)
	sltu	$2,$2,32
	beq	$2,$0,$L11
	nop

	lui	$2,%hi(gpMcsPriv)
	lw	$3,%lo(gpMcsPriv)($2)
	lw	$2,32($sp)
	sll	$2,$2,2
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	lw	$3,72($sp)
	sw	$3,16($2)
	lui	$2,%hi(gpMcsPriv)
	lw	$3,%lo(gpMcsPriv)($2)
	lw	$2,32($sp)
	sll	$2,$2,2
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	lw	$3,76($sp)
	sw	$3,20($2)
	lui	$2,%hi(gpMcsPriv)
	lw	$3,%lo(gpMcsPriv)($2)
	lw	$2,32($sp)
	sll	$2,$2,2
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	lw	$3,80($sp)
	sw	$3,24($2)
	lui	$2,%hi(gpMcsPriv)
	lw	$2,%lo(gpMcsPriv)($2)
	lw	$3,400($2)
	addiu	$3,$3,1
	sw	$3,400($2)
	lw	$2,32($sp)
	sltu	$2,$2,16
	bne	$2,$0,$L13
	nop

	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	lhu	$2,-30816($2)
	andi	$2,$2,0x400
	beq	$2,$0,$L13
	nop

	lui	$2,%hi(jiffies)
	lw	$3,%lo(jiffies)($2)
	lw	$2,80($sp)
	sw	$2,16($sp)
	lui	$2,%hi($LC1)
	addiu	$4,$2,%lo($LC1)
	move	$5,$3
	lw	$6,72($sp)
	lw	$7,76($sp)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	j	$L13
	nop

$L11:
	lui	$2,%hi($LC2)
	addiu	$4,$2,%lo($LC2)
	lw	$5,72($sp)
	lw	$6,76($sp)
	lw	$7,80($sp)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L13:
	lui	$2,%hi(gpMcsPriv)
	lw	$2,%lo(gpMcsPriv)($2)
	addiu	$2,$2,408
	sw	$2,40($sp)
	lw	$2,28($sp)
	sw	$2,44($sp)
	lw	$2,40($sp)
	move	$4,$2
	lw	$5,44($sp)
	lui	$2,%hi(_raw_spin_unlock_irqrestore)
	addiu	$2,$2,%lo(_raw_spin_unlock_irqrestore)
	jalr	$2
	nop

	lui	$2,%hi(gpMcsPriv)
	lw	$2,%lo(gpMcsPriv)($2)
	addiu	$2,$2,4
	move	$4,$2
	li	$5,1			# 0x1
	li	$6,1			# 0x1
	move	$7,$0
	lui	$2,%hi(__wake_up)
	addiu	$2,$2,%lo(__wake_up)
	jalr	$2
	nop

$L3:
	lw	$31,68($sp)
	addiu	$sp,$sp,72
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	xmcs_report_event
	.size	xmcs_report_event, .-xmcs_report_event
	.section	.text.xmcs_waiting_event,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	xmcs_waiting_event
	.type	xmcs_waiting_event, @function
xmcs_waiting_event:
	.frame	$sp,528,$31		# vars= 504, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	addiu	$sp,$sp,-528
	sw	$31,524($sp)
	sw	$4,528($sp)
	sw	$0,16($sp)
	lui	$2,%hi(gpMcsPriv)
	lw	$2,%lo(gpMcsPriv)($2)
	lw	$2,400($2)
	bne	$2,$0,$L15
	sw	$0,488($sp)
	sw	$0,492($sp)
	sw	$0,496($sp)
	sw	$0,500($sp)
	sw	$0,504($sp)
	move	$2,$28
	lw	$2,0($2)
	sw	$2,492($sp)
	lui	$2,%hi(autoremove_wake_function)
	addiu	$2,$2,%lo(autoremove_wake_function)
	sw	$2,496($sp)
	addiu	$2,$sp,488
	addiu	$2,$2,12
	sw	$2,500($sp)
	addiu	$2,$sp,488
	addiu	$2,$2,12
	sw	$2,504($sp)
$L24:
	lui	$2,%hi(gpMcsPriv)
	lw	$2,%lo(gpMcsPriv)($2)
	addiu	$3,$2,4
	addiu	$2,$sp,488
	move	$4,$3
	move	$5,$2
	li	$6,1			# 0x1
	lui	$2,%hi(prepare_to_wait)
	addiu	$2,$2,%lo(prepare_to_wait)
	jalr	$2
	lui	$2,%hi(gpMcsPriv)
	lw	$2,%lo(gpMcsPriv)($2)
	lw	$2,400($2)
	beq	$2,$0,$L16
	j	$L30
$L16:
	move	$2,$28
	lw	$2,0($2)
	sw	$2,52($sp)
	lw	$2,52($sp)
	sw	$2,56($sp)
	li	$2,1			# 0x1
	sw	$2,60($sp)
	lw	$2,56($sp)
	lw	$2,4($2)
	sw	$2,64($sp)
	lw	$2,60($sp)
	sw	$2,68($sp)
	lw	$2,64($sp)
	addiu	$2,$2,8
	lw	$3,68($sp)
	sw	$3,72($sp)
	sw	$2,76($sp)
	lw	$2,72($sp)
	bgez	$2,$L18
	addiu	$2,$2,31
$L18:
	sra	$2,$2,5
	sll	$2,$2,2
	lw	$3,76($sp)
	addu	$2,$3,$2
	lw	$3,0($2)
	lw	$2,72($sp)
	andi	$2,$2,0x1f
	srl	$2,$3,$2
	andi	$2,$2,0x1
	sltu	$2,$0,$2
	andi	$2,$2,0x00ff
	bne	$2,$0,$L23
	lui	$2,%hi(schedule)
	addiu	$2,$2,%lo(schedule)
	jalr	$2
	j	$L24
$L23:
	li	$2,-512			# 0xfffffffffffffe00
	sw	$2,16($sp)
$L30:
	.set	noreorder
	nop
	.set	reorder
	lui	$2,%hi(gpMcsPriv)
	lw	$2,%lo(gpMcsPriv)($2)
	addiu	$3,$2,4
	addiu	$2,$sp,488
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(finish_wait)
	addiu	$2,$2,%lo(finish_wait)
	jalr	$2
$L15:
	lui	$2,%hi(gpMcsPriv)
	lw	$2,%lo(gpMcsPriv)($2)
	addiu	$2,$2,408
	sw	$2,80($sp)
	lw	$2,80($sp)
	move	$4,$2
	lui	$2,%hi(_raw_spin_lock_irqsave)
	addiu	$2,$2,%lo(_raw_spin_lock_irqsave)
	jalr	$2
	sw	$2,24($sp)
	lui	$2,%hi(gpMcsPriv)
	lw	$2,%lo(gpMcsPriv)($2)
	addiu	$2,$2,16
	addiu	$3,$sp,92
	move	$4,$3
	move	$5,$2
	li	$6,388			# 0x184
	lui	$2,%hi(memcpy)
	addiu	$2,$2,%lo(memcpy)
	jalr	$2
	lui	$2,%hi(gpMcsPriv)
	lw	$2,%lo(gpMcsPriv)($2)
	addiu	$2,$2,16
	move	$4,$2
	move	$5,$0
	li	$6,388			# 0x184
	lui	$2,%hi(memset)
	addiu	$2,$2,%lo(memset)
	jalr	$2
	lui	$2,%hi(gpMcsPriv)
	lw	$2,%lo(gpMcsPriv)($2)
	addiu	$2,$2,408
	sw	$2,84($sp)
	lw	$2,24($sp)
	sw	$2,88($sp)
	lw	$2,84($sp)
	move	$4,$2
	lw	$5,88($sp)
	lui	$2,%hi(_raw_spin_unlock_irqrestore)
	addiu	$2,$2,%lo(_raw_spin_unlock_irqrestore)
	jalr	$2
	lw	$2,528($sp)
	sw	$2,28($sp)
	addiu	$2,$sp,92
	sw	$2,32($sp)
	li	$2,388			# 0x184
	sw	$2,20($sp)
	lw	$2,28($sp)
	sw	$2,36($sp)
	lw	$2,20($sp)
	sw	$2,40($sp)
	move	$2,$28
	lw	$2,24($2)
	sw	$2,44($sp)
	lw	$3,36($sp)
	lw	$2,40($sp)
	addu	$3,$3,$2
	lw	$2,36($sp)
	or	$3,$3,$2
	lw	$2,40($sp)
	or	$3,$3,$2
	lw	$2,44($sp)
	and	$2,$3,$2
	sw	$2,48($sp)
	lw	$2,48($sp)
	sltu	$2,$2,1
	andi	$2,$2,0x00ff
	sltu	$2,$0,$2
	andi	$2,$2,0x00ff
	beq	$2,$0,$L26
	lw	$4,28($sp)
	lw	$5,32($sp)
	lw	$6,20($sp)
#APP
 # 98 "/opt/tclinux_phoenix/modules/private/xpon/src/xmcs/xmcs_fdet.c" 1
	.set	noat
	la	$1, __copy_user
	jalr	$1
	.set	at
	
 # 0 "" 2
#NO_APP
	move	$2,$6
	sw	$2,20($sp)
$L26:
	lw	$2,20($sp)
	beq	$2,$0,$L27
	li	$2,-14			# 0xfffffffffffffff2
	j	$L29
$L27:
#APP
 # 101 "/opt/tclinux_phoenix/modules/private/xpon/src/xmcs/xmcs_fdet.c" 1
	.set	push
	.set	noreorder
	.set	mips2
	sync
	.set	pop
 # 0 "" 2
#NO_APP
	move	$2,$0
$L29:
	lw	$31,524($sp)
	addiu	$sp,$sp,528
	j	$31
	.end	xmcs_waiting_event
	.size	xmcs_waiting_event, .-xmcs_waiting_event
	.rdata
	.align	2
$LC3:
	.ascii	"driver layer event_ctrl_flag: %x\012\000"
	.section	.text.xmcs_set_event_ctrl,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	xmcs_set_event_ctrl
	.type	xmcs_set_event_ctrl, @function
xmcs_set_event_ctrl:
	.frame	$sp,88,$31		# vars= 64, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	addiu	$sp,$sp,-88
	sw	$31,84($sp)
	sw	$4,88($sp)
	sh	$0,20($sp)
	addiu	$2,$sp,60
	sw	$2,24($sp)
	lw	$2,88($sp)
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
	beq	$2,$0,$L32
	lw	$4,24($sp)
	lw	$5,28($sp)
	lw	$6,16($sp)
#APP
 # 117 "/opt/tclinux_phoenix/modules/private/xpon/src/xmcs/xmcs_fdet.c" 1
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
$L32:
	lw	$2,16($sp)
	beq	$2,$0,$L33
	li	$2,-14			# 0xfffffffffffffff2
	j	$L36
$L33:
	lui	$2,%hi(gpMcsPriv)
	lw	$2,%lo(gpMcsPriv)($2)
	addiu	$2,$2,408
	sw	$2,48($sp)
	lw	$2,48($sp)
	move	$4,$2
	lui	$2,%hi(_raw_spin_lock_irqsave)
	addiu	$2,$2,%lo(_raw_spin_lock_irqsave)
	jalr	$2
	sh	$2,20($sp)
	lui	$2,%hi(gpMcsPriv)
	lw	$2,%lo(gpMcsPriv)($2)
	addiu	$3,$2,404
	addiu	$2,$sp,60
	move	$4,$3
	move	$5,$2
	li	$6,4			# 0x4
	lui	$2,%hi(memcpy)
	addiu	$2,$2,%lo(memcpy)
	jalr	$2
	lui	$2,%hi(gpMcsPriv)
	lw	$2,%lo(gpMcsPriv)($2)
	addiu	$3,$2,408
	lhu	$2,20($sp)
	sw	$3,52($sp)
	sw	$2,56($sp)
	lw	$2,52($sp)
	move	$4,$2
	lw	$5,56($sp)
	lui	$2,%hi(_raw_spin_unlock_irqrestore)
	addiu	$2,$2,%lo(_raw_spin_unlock_irqrestore)
	jalr	$2
	lui	$2,%hi($LC3)
	addiu	$4,$2,%lo($LC3)
	lw	$5,60($sp)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	move	$2,$0
$L36:
	lw	$31,84($sp)
	addiu	$sp,$sp,88
	j	$31
	.end	xmcs_set_event_ctrl
	.size	xmcs_set_event_ctrl, .-xmcs_set_event_ctrl
	.section	.text.xmcs_get_event_ctrl,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	xmcs_get_event_ctrl
	.type	xmcs_get_event_ctrl, @function
xmcs_get_event_ctrl:
	.frame	$sp,88,$31		# vars= 64, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	addiu	$sp,$sp,-88
	sw	$31,84($sp)
	sw	$4,88($sp)
	sh	$0,20($sp)
	lui	$2,%hi(gpMcsPriv)
	lw	$2,%lo(gpMcsPriv)($2)
	addiu	$2,$2,408
	sw	$2,48($sp)
	lw	$2,48($sp)
	move	$4,$2
	lui	$2,%hi(_raw_spin_lock_irqsave)
	addiu	$2,$2,%lo(_raw_spin_lock_irqsave)
	jalr	$2
	sh	$2,20($sp)
	lui	$2,%hi(gpMcsPriv)
	lw	$2,%lo(gpMcsPriv)($2)
	addiu	$2,$2,404
	addiu	$3,$sp,60
	move	$4,$3
	move	$5,$2
	li	$6,4			# 0x4
	lui	$2,%hi(memcpy)
	addiu	$2,$2,%lo(memcpy)
	jalr	$2
	lui	$2,%hi(gpMcsPriv)
	lw	$2,%lo(gpMcsPriv)($2)
	addiu	$3,$2,408
	lhu	$2,20($sp)
	sw	$3,52($sp)
	sw	$2,56($sp)
	lw	$2,52($sp)
	move	$4,$2
	lw	$5,56($sp)
	lui	$2,%hi(_raw_spin_unlock_irqrestore)
	addiu	$2,$2,%lo(_raw_spin_unlock_irqrestore)
	jalr	$2
	lw	$2,88($sp)
	sw	$2,24($sp)
	addiu	$2,$sp,60
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
	beq	$2,$0,$L39
	lw	$4,24($sp)
	lw	$5,28($sp)
	lw	$6,16($sp)
#APP
 # 144 "/opt/tclinux_phoenix/modules/private/xpon/src/xmcs/xmcs_fdet.c" 1
	.set	noat
	la	$1, __copy_user
	jalr	$1
	.set	at
	
 # 0 "" 2
#NO_APP
	move	$2,$6
	sw	$2,16($sp)
$L39:
	lw	$2,16($sp)
	beq	$2,$0,$L40
	li	$2,-14			# 0xfffffffffffffff2
	j	$L42
$L40:
	move	$2,$0
$L42:
	lw	$31,84($sp)
	addiu	$sp,$sp,88
	j	$31
	.end	xmcs_get_event_ctrl
	.size	xmcs_get_event_ctrl, .-xmcs_get_event_ctrl
	.rdata
	.align	2
$LC4:
	.ascii	"\015\012[%s][%d](%d)\015\012\000"
	.align	2
$LC5:
	.ascii	"[%lu0ms]No such I/O command, cmd: %x\012\000"
	.section	.text.fdet_cmd_proc,"ax",@progbits
	.align	2
	.globl	fdet_cmd_proc
	.set	nomips16
	.set	nomicromips
	.ent	fdet_cmd_proc
	.type	fdet_cmd_proc, @function
fdet_cmd_proc:
	.frame	$sp,32,$31		# vars= 8, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-32
	sw	$31,28($sp)
	sw	$4,32($sp)
	sw	$5,36($sp)
	sw	$0,20($sp)
	lw	$2,32($sp)
	li	$3,1074003968			# 0x40040000
	ori	$3,$3,0xdb02
	beq	$2,$3,$L45
	nop

	li	$3,1074003968			# 0x40040000
	ori	$3,$3,0xdb03
	sltu	$3,$2,$3
	beq	$3,$0,$L46
	nop

	li	$3,1074003968			# 0x40040000
	ori	$3,$3,0xdb01
	beq	$2,$3,$L47
	nop

	j	$L44
	nop

$L46:
	li	$3,1074003968			# 0x40040000
	ori	$3,$3,0xdb03
	beq	$2,$3,$L48
	nop

	li	$3,-2147221504			# 0xffffffff80040000
	ori	$3,$3,0xdb03
	beq	$2,$3,$L49
	nop

	j	$L44
	nop

$L47:
	lw	$2,36($sp)
	move	$4,$2
	lui	$2,%hi(xmcs_waiting_event)
	addiu	$2,$2,%lo(xmcs_waiting_event)
	jalr	$2
	nop

	sw	$2,16($sp)
	j	$L50
	nop

$L45:
	lui	$2,%hi(__this_module)
	addiu	$4,$2,%lo(__this_module)
	lui	$2,%hi(module_refcount)
	addiu	$2,$2,%lo(module_refcount)
	jalr	$2
	nop

	sw	$2,20($sp)
	lui	$2,%hi($LC4)
	addiu	$4,$2,%lo($LC4)
	lui	$2,%hi(__func__.32889)
	addiu	$5,$2,%lo(__func__.32889)
	li	$6,164			# 0xa4
	lw	$7,20($sp)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lw	$2,20($sp)
	addiu	$2,$2,-2
	sw	$2,20($sp)
	j	$L51
	nop

$L52:
	lui	$2,%hi(__this_module)
	addiu	$4,$2,%lo(__this_module)
	lui	$2,%hi(module_put)
	addiu	$2,$2,%lo(module_put)
	jalr	$2
	nop

	lw	$2,20($sp)
	addiu	$2,$2,-1
	sw	$2,20($sp)
$L51:
	lw	$2,20($sp)
	bgtz	$2,$L52
	nop

	j	$L50
	nop

$L49:
	lw	$2,36($sp)
	move	$4,$2
	lui	$2,%hi(xmcs_set_event_ctrl)
	addiu	$2,$2,%lo(xmcs_set_event_ctrl)
	jalr	$2
	nop

	sw	$2,16($sp)
	j	$L50
	nop

$L48:
	lw	$2,36($sp)
	move	$4,$2
	lui	$2,%hi(xmcs_get_event_ctrl)
	addiu	$2,$2,%lo(xmcs_get_event_ctrl)
	jalr	$2
	nop

	sw	$2,16($sp)
	j	$L50
	nop

$L44:
	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	lhu	$2,-30816($2)
	andi	$2,$2,0x1
	beq	$2,$0,$L53
	nop

	lui	$2,%hi(jiffies)
	lw	$3,%lo(jiffies)($2)
	lui	$2,%hi($LC5)
	addiu	$4,$2,%lo($LC5)
	move	$5,$3
	lw	$6,32($sp)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	j	$L55
	nop

$L53:
$L55:
	nop
$L50:
	lw	$2,16($sp)
	lw	$31,28($sp)
	addiu	$sp,$sp,32
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	fdet_cmd_proc
	.size	fdet_cmd_proc, .-fdet_cmd_proc
	.rdata
	.align	2
	.type	__func__.32889, @object
	.size	__func__.32889, 14
__func__.32889:
	.ascii	"fdet_cmd_proc\000"
	.ident	"GCC: (Buildroot 2015.08.1) 4.9.3"
