	.file	1 "xpon_led.c"
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
	.ascii	"[%lu0ms]turn on LED success. \012\000"
	.align	2
$LC1:
	.ascii	"[%lu0ms]turn off LED success. \012\000"
	.align	2
$LC2:
	.ascii	"[%lu0ms]LED flicker success. \012\000"
	.align	2
$LC3:
	.ascii	"[%lu0ms]LOS LED fail. \012\000"
#NO_APP
	.section	.text.change_alarm_led_status,"ax",@progbits
	.align	2
	.globl	change_alarm_led_status
	.set	nomips16
	.set	nomicromips
	.ent	change_alarm_led_status
	.type	change_alarm_led_status, @function
change_alarm_led_status:
	.frame	$sp,24,$31		# vars= 0, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-24
	sw	$31,20($sp)
	sw	$4,24($sp)
	lw	$2,24($sp)
	li	$3,1			# 0x1
	beq	$2,$3,$L3
	nop

	sltu	$3,$2,1
	bne	$3,$0,$L4
	nop

	li	$3,2			# 0x2
	beq	$2,$3,$L5
	nop

	j	$L11
	nop

$L3:
	li	$4,67			# 0x43
	lui	$2,%hi(ledTurnOff)
	addiu	$2,$2,%lo(ledTurnOff)
	jalr	$2
	nop

	li	$4,66			# 0x42
	lui	$2,%hi(ledTurnOn)
	addiu	$2,$2,%lo(ledTurnOn)
	jalr	$2
	nop

	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	lhu	$2,-30816($2)
	andi	$2,$2,0x100
	beq	$2,$0,$L6
	nop

	lui	$2,%hi(jiffies)
	lw	$3,%lo(jiffies)($2)
	lui	$2,%hi($LC0)
	addiu	$4,$2,%lo($LC0)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	j	$L1
	nop

$L6:
	j	$L1
	nop

$L4:
	li	$4,66			# 0x42
	lui	$2,%hi(ledTurnOff)
	addiu	$2,$2,%lo(ledTurnOff)
	jalr	$2
	nop

	li	$4,67			# 0x43
	lui	$2,%hi(ledTurnOff)
	addiu	$2,$2,%lo(ledTurnOff)
	jalr	$2
	nop

	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	lhu	$2,-30816($2)
	andi	$2,$2,0x100
	beq	$2,$0,$L8
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

	j	$L1
	nop

$L8:
	j	$L1
	nop

$L5:
	li	$4,66			# 0x42
	lui	$2,%hi(ledTurnOff)
	addiu	$2,$2,%lo(ledTurnOff)
	jalr	$2
	nop

	li	$4,67			# 0x43
	lui	$2,%hi(ledTurnOn)
	addiu	$2,$2,%lo(ledTurnOn)
	jalr	$2
	nop

	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	lhu	$2,-30816($2)
	andi	$2,$2,0x100
	beq	$2,$0,$L9
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

	j	$L1
	nop

$L9:
	j	$L1
	nop

$L11:
	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	lhu	$2,-30816($2)
	andi	$2,$2,0x1
	beq	$2,$0,$L10
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

$L10:
	nop
$L1:
	lw	$31,20($sp)
	addiu	$sp,$sp,24
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	change_alarm_led_status
	.size	change_alarm_led_status, .-change_alarm_led_status
	.rdata
	.align	2
$LC4:
	.ascii	"%d\012\000"
	.section	.text.xpon_los_led_read_proc,"ax",@progbits
	.align	2
	.globl	xpon_los_led_read_proc
	.set	nomips16
	.set	nomicromips
	.ent	xpon_los_led_read_proc
	.type	xpon_los_led_read_proc, @function
xpon_los_led_read_proc:
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
	lui	$2,%hi(xpon_los_status)
	lw	$3,%lo(xpon_los_status)($2)
	lw	$4,32($sp)
	lui	$2,%hi($LC4)
	addiu	$5,$2,%lo($LC4)
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
	beq	$2,$0,$L13
	nop

	lw	$2,44($sp)
	sw	$2,16($sp)
	j	$L14
	nop

$L13:
	lw	$2,48($sp)
	li	$3,1			# 0x1
	sw	$3,0($2)
$L14:
	lw	$2,16($sp)
	bgez	$2,$L15
	nop

	sw	$0,16($sp)
$L15:
	lw	$2,16($sp)
	lw	$31,28($sp)
	addiu	$sp,$sp,32
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	xpon_los_led_read_proc
	.size	xpon_los_led_read_proc, .-xpon_los_led_read_proc
	.section	.text.xpon_los_led_write_proc,"ax",@progbits
	.align	2
	.globl	xpon_los_led_write_proc
	.set	nomips16
	.set	nomicromips
	.ent	xpon_los_led_write_proc
	.type	xpon_los_led_write_proc, @function
xpon_los_led_write_proc:
	.frame	$sp,64,$31		# vars= 40, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	addiu	$sp,$sp,-64
	sw	$31,60($sp)
	sw	$4,64($sp)
	sw	$5,68($sp)
	sw	$6,72($sp)
	sw	$7,76($sp)
	sw	$0,48($sp)
	sw	$0,52($sp)
	sw	$0,20($sp)
	lw	$2,72($sp)
	sltu	$2,$2,8
	bne	$2,$0,$L18
	li	$2,-22			# 0xffffffffffffffea
	j	$L22
$L18:
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
	beq	$2,$0,$L20
	lw	$4,24($sp)
	lw	$5,28($sp)
	lw	$6,16($sp)
#APP
 # 202 "/opt/tclinux_phoenix/modules/private/xpon/src/xpon_led.c" 1
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
$L20:
	lw	$2,16($sp)
	beq	$2,$0,$L21
	li	$2,-14			# 0xfffffffffffffff2
	j	$L22
$L21:
	lw	$2,72($sp)
	addiu	$3,$sp,16
	addu	$2,$3,$2
	sb	$0,32($2)
	addiu	$2,$sp,48
	move	$4,$2
	move	$5,$0
	li	$6,10			# 0xa
	lui	$2,%hi(simple_strtoul)
	addiu	$2,$2,%lo(simple_strtoul)
	jalr	$2
	sw	$2,20($sp)
	lw	$4,20($sp)
	lui	$2,%hi(change_alarm_led_status)
	addiu	$2,$2,%lo(change_alarm_led_status)
	jalr	$2
	lw	$2,72($sp)
$L22:
	lw	$31,60($sp)
	addiu	$sp,$sp,64
	j	$31
	.end	xpon_los_led_write_proc
	.size	xpon_los_led_write_proc, .-xpon_los_led_write_proc
	.ident	"GCC: (Buildroot 2015.08.1) 4.9.3"
