	.file	1 "phy.c"
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
	.globl	isNeedResetTransciver
	.section	.bss,"aw",@nobits
	.align	2
	.type	isNeedResetTransciver, @object
	.size	isNeedResetTransciver, 4
isNeedResetTransciver:
	.space	4
	.globl	phy_version
	.data
	.align	2
	.type	phy_version, @object
	.size	phy_version, 4
phy_version:
	.word	103
	.rdata
	.align	2
$LC0:
	.ascii	"\012Error! phy_I2C_read return value = 0\012u1DevAddr:0x"
	.ascii	"%x, u4WordAddr:0x%x\012\000"
	.section	.text.phy_I2C_read,"ax",@progbits
	.align	2
	.globl	phy_I2C_read
	.set	nomips16
	.set	nomicromips
	.ent	phy_I2C_read
	.type	phy_I2C_read, @function
phy_I2C_read:
	.frame	$sp,56,$31		# vars= 16, regs= 1/0, args= 32, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-56
	sw	$31,52($sp)
	move	$8,$4
	move	$4,$6
	move	$3,$7
	lw	$2,80($sp)
	sb	$8,56($sp)
	sh	$5,60($sp)
	sb	$4,64($sp)
	sb	$3,68($sp)
	sh	$2,40($sp)
	lbu	$4,56($sp)
	lhu	$5,60($sp)
	lbu	$6,64($sp)
	lbu	$7,68($sp)
	lhu	$2,40($sp)
	lw	$3,72($sp)
	sw	$3,16($sp)
	lw	$3,76($sp)
	sw	$3,20($sp)
	sw	$2,24($sp)
	lui	$2,%hi(SIF_X_Read)
	addiu	$2,$2,%lo(SIF_X_Read)
	jalr	$2
	nop

	sw	$2,32($sp)
	lw	$2,32($sp)
	bne	$2,$0,$L2
	nop

	lbu	$3,64($sp)
	lui	$2,%hi($LC0)
	addiu	$4,$2,%lo($LC0)
	move	$5,$3
	lw	$6,72($sp)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L2:
	lw	$2,32($sp)
	andi	$2,$2,0xffff
	lw	$31,52($sp)
	addiu	$sp,$sp,56
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	phy_I2C_read
	.size	phy_I2C_read, .-phy_I2C_read
	.rdata
	.align	2
$LC1:
	.ascii	"\012Error! phy_I2C_write return value = 0\012u1DevAddr:0"
	.ascii	"x%x, u4WordAddr:0x%x, pu1Buf:0x%x\012\000"
	.section	.text.phy_I2C_write,"ax",@progbits
	.align	2
	.globl	phy_I2C_write
	.set	nomips16
	.set	nomicromips
	.ent	phy_I2C_write
	.type	phy_I2C_write, @function
phy_I2C_write:
	.frame	$sp,56,$31		# vars= 16, regs= 1/0, args= 32, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-56
	sw	$31,52($sp)
	move	$8,$4
	move	$4,$6
	move	$3,$7
	lw	$2,80($sp)
	sb	$8,56($sp)
	sh	$5,60($sp)
	sb	$4,64($sp)
	sb	$3,68($sp)
	sh	$2,40($sp)
	lbu	$4,56($sp)
	lhu	$5,60($sp)
	lbu	$6,64($sp)
	lbu	$7,68($sp)
	lhu	$2,40($sp)
	lw	$3,72($sp)
	sw	$3,16($sp)
	lw	$3,76($sp)
	sw	$3,20($sp)
	sw	$2,24($sp)
	lui	$2,%hi(SIF_X_Write)
	addiu	$2,$2,%lo(SIF_X_Write)
	jalr	$2
	nop

	sw	$2,32($sp)
	lw	$2,32($sp)
	bne	$2,$0,$L5
	nop

	lbu	$5,64($sp)
	lw	$2,76($sp)
	lw	$3,0($2)
	lui	$2,%hi($LC1)
	addiu	$4,$2,%lo($LC1)
	lw	$6,72($sp)
	move	$7,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L5:
	lw	$2,32($sp)
	andi	$2,$2,0xffff
	lw	$31,52($sp)
	addiu	$sp,$sp,56
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	phy_I2C_write
	.size	phy_I2C_write, .-phy_I2C_write
	.section	.text.phy_counter_clear,"ax",@progbits
	.align	2
	.globl	phy_counter_clear
	.set	nomips16
	.set	nomicromips
	.ent	phy_counter_clear
	.type	phy_counter_clear, @function
phy_counter_clear:
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
	lbu	$2,40($sp)
	andi	$2,$2,0x1
	beq	$2,$0,$L8
	nop

	li	$4,2			# 0x2
	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$5,$2,0x234
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

$L8:
	lbu	$2,40($sp)
	andi	$2,$2,0x2
	beq	$2,$0,$L9
	nop

	li	$4,8			# 0x8
	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$5,$2,0x234
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

$L9:
	lbu	$2,40($sp)
	andi	$2,$2,0x4
	beq	$2,$0,$L10
	nop

	li	$4,32			# 0x20
	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$5,$2,0x234
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

$L10:
	lbu	$2,40($sp)
	andi	$2,$2,0x8
	beq	$2,$0,$L11
	nop

	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$2,$2,0x424
	sw	$2,24($sp)
	lw	$2,24($sp)
	lw	$2,0($2)
	sw	$2,28($sp)
	lw	$2,24($sp)
	lw	$2,0($2)
	sw	$2,28($sp)
	lw	$2,28($sp)
	sw	$2,16($sp)
	lw	$2,16($sp)
	ori	$2,$2,0x4
	sw	$2,20($sp)
	lw	$4,20($sp)
	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$5,$2,0x424
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

$L11:
	lbu	$2,40($sp)
	andi	$2,$2,0x10
	beq	$2,$0,$L7
	nop

	li	$4,16			# 0x10
	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$5,$2,0x298
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

$L7:
	lw	$31,36($sp)
	addiu	$sp,$sp,40
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	phy_counter_clear
	.size	phy_counter_clear, .-phy_counter_clear
	.section	.text.phy_int_enable,"ax",@progbits
	.align	2
	.globl	phy_int_enable
	.set	nomips16
	.set	nomicromips
	.ent	phy_int_enable
	.type	phy_int_enable, @function
phy_int_enable:
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
	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$2,$2,0x5f0
	sw	$2,24($sp)
	lw	$2,24($sp)
	lw	$2,0($2)
	sw	$2,28($sp)
	lw	$2,24($sp)
	lw	$2,0($2)
	sw	$2,28($sp)
	lw	$2,28($sp)
	sw	$2,20($sp)
	lw	$3,20($sp)
	lw	$2,40($sp)
	or	$2,$3,$2
	andi	$2,$2,0xff
	sw	$2,16($sp)
	lw	$4,16($sp)
	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$5,$2,0x5f0
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	lw	$31,36($sp)
	addiu	$sp,$sp,40
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	phy_int_enable
	.size	phy_int_enable, .-phy_int_enable
	.section	.text.phy_int_disable,"ax",@progbits
	.align	2
	.globl	phy_int_disable
	.set	nomips16
	.set	nomicromips
	.ent	phy_int_disable
	.type	phy_int_disable, @function
phy_int_disable:
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
	lw	$2,40($sp)
	andi	$2,$2,0xff
	move	$4,$2
	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$5,$2,0x5f4
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$2,$2,0x5f0
	sw	$2,24($sp)
	lw	$2,24($sp)
	lw	$2,0($2)
	sw	$2,28($sp)
	lw	$2,24($sp)
	lw	$2,0($2)
	sw	$2,28($sp)
	lw	$2,28($sp)
	sw	$2,20($sp)
	lw	$2,40($sp)
	andi	$2,$2,0xff
	nor	$3,$0,$2
	lw	$2,20($sp)
	and	$2,$3,$2
	andi	$2,$2,0xff
	sw	$2,16($sp)
	lw	$4,16($sp)
	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$5,$2,0x5f0
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	lw	$31,36($sp)
	addiu	$sp,$sp,40
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	phy_int_disable
	.size	phy_int_disable, .-phy_int_disable
	.rdata
	.align	2
$LC2:
	.ascii	"/opt/tclinux_phoenix/modules/private/xpon_phy/src/phy.c\000"
	.align	2
$LC3:
	.ascii	"[%lu]0ms[%s:%d]\012  Phy_Int_Config read_data:0x%.8x\012"
	.ascii	"\000"
	.align	2
$LC4:
	.ascii	"[%lu]0ms[%s:%d]\012  Phy_Int_Config write_data:0x%.8x\012"
	.ascii	"\000"
	.section	.text.phy_int_config,"ax",@progbits
	.align	2
	.globl	phy_int_config
	.set	nomips16
	.set	nomicromips
	.ent	phy_int_config
	.type	phy_int_config, @function
phy_int_config:
	.frame	$sp,48,$31		# vars= 16, regs= 2/0, args= 24, gp= 0
	.mask	0x80010000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-48
	sw	$31,44($sp)
	sw	$16,40($sp)
	sw	$4,48($sp)
	sw	$0,24($sp)
	sw	$0,28($sp)
	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$2,$2,0x5f0
	sw	$2,32($sp)
	lw	$2,32($sp)
	lw	$2,0($2)
	sw	$2,36($sp)
	lw	$2,32($sp)
	lw	$2,0($2)
	sw	$2,36($sp)
	lw	$2,36($sp)
	sw	$2,28($sp)
	lw	$3,28($sp)
	li	$2,-256			# 0xffffffffffffff00
	and	$3,$3,$2
	lw	$2,48($sp)
	or	$2,$3,$2
	sw	$2,24($sp)
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,136($2)
	andi	$2,$2,0x10
	beq	$2,$0,$L20
	nop

	lui	$2,%hi(jiffies)
	lw	$16,%lo(jiffies)($2)
	lui	$2,%hi($LC2)
	addiu	$4,$2,%lo($LC2)
	li	$5,47			# 0x2f
	lui	$2,%hi(strrchr)
	addiu	$2,$2,%lo(strrchr)
	jalr	$2
	nop

	addiu	$3,$2,1
	lw	$2,28($sp)
	sw	$2,16($sp)
	lui	$2,%hi($LC3)
	addiu	$4,$2,%lo($LC3)
	move	$5,$16
	move	$6,$3
	li	$7,227			# 0xe3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L20:
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,136($2)
	andi	$2,$2,0x10
	beq	$2,$0,$L21
	nop

	lui	$2,%hi(jiffies)
	lw	$16,%lo(jiffies)($2)
	lui	$2,%hi($LC2)
	addiu	$4,$2,%lo($LC2)
	li	$5,47			# 0x2f
	lui	$2,%hi(strrchr)
	addiu	$2,$2,%lo(strrchr)
	jalr	$2
	nop

	addiu	$3,$2,1
	lw	$2,24($sp)
	sw	$2,16($sp)
	lui	$2,%hi($LC4)
	addiu	$4,$2,%lo($LC4)
	move	$5,$16
	move	$6,$3
	li	$7,228			# 0xe4
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L21:
	lw	$4,24($sp)
	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$5,$2,0x5f0
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	move	$2,$0
	lw	$31,44($sp)
	lw	$16,40($sp)
	addiu	$sp,$sp,48
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	phy_int_config
	.size	phy_int_config, .-phy_int_config
	.section	.text.phy_int_getting,"ax",@progbits
	.align	2
	.globl	phy_int_getting
	.set	nomips16
	.set	nomicromips
	.ent	phy_int_getting
	.type	phy_int_getting, @function
phy_int_getting:
	.frame	$sp,8,$31		# vars= 8, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-8
	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$2,$2,0x5f0
	sw	$2,0($sp)
	lw	$2,0($sp)
	lw	$2,0($2)
	sw	$2,4($sp)
	lw	$2,0($sp)
	lw	$2,0($2)
	sw	$2,4($sp)
	lw	$2,4($sp)
	addiu	$sp,$sp,8
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	phy_int_getting
	.size	phy_int_getting, .-phy_int_getting
	.section	.text.phy_gpon_mode,"ax",@progbits
	.align	2
	.globl	phy_gpon_mode
	.set	nomips16
	.set	nomicromips
	.ent	phy_gpon_mode
	.type	phy_gpon_mode, @function
phy_gpon_mode:
	.frame	$sp,16,$31		# vars= 16, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-16
	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$2,$2,0x124
	sw	$2,4($sp)
	lw	$2,4($sp)
	lw	$2,0($2)
	sw	$2,8($sp)
	lw	$2,4($sp)
	lw	$2,0($2)
	sw	$2,8($sp)
	lw	$2,8($sp)
	sw	$2,0($sp)
	lw	$2,0($sp)
	srl	$2,$2,31
	andi	$2,$2,0x00ff
	addiu	$sp,$sp,16
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	phy_gpon_mode
	.size	phy_gpon_mode, .-phy_gpon_mode
	.rdata
	.align	2
$LC5:
	.ascii	"can not recognize phy mode %d\012\000"
	.align	2
$LC6:
	.ascii	"can not recognize tx_enable %d\012\000"
	.section	.text.phy_mode_config,"ax",@progbits
	.align	2
	.globl	phy_mode_config
	.set	nomips16
	.set	nomicromips
	.ent	phy_mode_config
	.type	phy_mode_config, @function
phy_mode_config:
	.frame	$sp,88,$31		# vars= 64, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-88
	sw	$31,84($sp)
	sw	$4,88($sp)
	sw	$5,92($sp)
	sw	$0,16($sp)
	sw	$0,20($sp)
	lw	$3,88($sp)
	li	$2,1			# 0x1
	beq	$3,$2,$L30
	nop

	lw	$2,88($sp)
	beq	$2,$0,$L30
	nop

	lui	$2,%hi($LC5)
	addiu	$4,$2,%lo($LC5)
	lw	$5,88($sp)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	li	$2,1			# 0x1
	j	$L31
	nop

$L30:
	lw	$3,92($sp)
	li	$2,1			# 0x1
	beq	$3,$2,$L32
	nop

	lw	$2,92($sp)
	beq	$2,$0,$L32
	nop

	lui	$2,%hi($LC6)
	addiu	$4,$2,%lo($LC6)
	lw	$5,92($sp)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	li	$2,1			# 0x1
	j	$L31
	nop

$L32:
	lw	$3,88($sp)
	li	$2,1			# 0x1
	bne	$3,$2,$L33
	nop

	move	$4,$0
	lui	$2,%hi(phy_trans_power_switch)
	addiu	$2,$2,%lo(phy_trans_power_switch)
	jalr	$2
	nop

	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$2,$2,0x108
	sw	$2,24($sp)
	lw	$2,24($sp)
	lw	$2,0($2)
	sw	$2,28($sp)
	lw	$2,24($sp)
	lw	$2,0($2)
	sw	$2,28($sp)
	lw	$2,28($sp)
	sw	$2,20($sp)
	lw	$3,20($sp)
	li	$2,-33			# 0xffffffffffffffdf
	and	$2,$3,$2
	sw	$2,16($sp)
	lw	$4,16($sp)
	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$5,$2,0x108
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$2,$2,0x124
	sw	$2,32($sp)
	lw	$2,32($sp)
	lw	$2,0($2)
	sw	$2,36($sp)
	lw	$2,32($sp)
	lw	$2,0($2)
	sw	$2,36($sp)
	lw	$2,36($sp)
	sw	$2,20($sp)
	lw	$3,20($sp)
	li	$2,-2147483648			# 0xffffffff80000000
	or	$2,$3,$2
	sw	$2,16($sp)
	lw	$4,16($sp)
	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$5,$2,0x124
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$2,$2,0x108
	sw	$2,40($sp)
	lw	$2,40($sp)
	lw	$2,0($2)
	sw	$2,44($sp)
	lw	$2,40($sp)
	lw	$2,0($2)
	sw	$2,44($sp)
	lw	$2,44($sp)
	sw	$2,20($sp)
	lw	$3,20($sp)
	li	$2,-2013265920			# 0xffffffff88000000
	or	$2,$3,$2
	sw	$2,16($sp)
	lw	$4,16($sp)
	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$5,$2,0x108
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	li	$4,1000			# 0x3e8
	lui	$2,%hi(__udelay)
	addiu	$2,$2,%lo(__udelay)
	jalr	$2
	nop

	lw	$4,20($sp)
	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$5,$2,0x108
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	j	$L37
	nop

$L33:
	lw	$2,88($sp)
	bne	$2,$0,$L37
	nop

	move	$4,$0
	lui	$2,%hi(phy_trans_power_switch)
	addiu	$2,$2,%lo(phy_trans_power_switch)
	jalr	$2
	nop

	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$2,$2,0x108
	sw	$2,48($sp)
	lw	$2,48($sp)
	lw	$2,0($2)
	sw	$2,52($sp)
	lw	$2,48($sp)
	lw	$2,0($2)
	sw	$2,52($sp)
	lw	$2,52($sp)
	sw	$2,20($sp)
	lw	$3,20($sp)
	li	$2,-33			# 0xffffffffffffffdf
	and	$2,$3,$2
	sw	$2,16($sp)
	lw	$4,16($sp)
	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$5,$2,0x108
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$2,$2,0x124
	sw	$2,56($sp)
	lw	$2,56($sp)
	lw	$2,0($2)
	sw	$2,60($sp)
	lw	$2,56($sp)
	lw	$2,0($2)
	sw	$2,60($sp)
	lw	$2,60($sp)
	sw	$2,20($sp)
	lw	$2,20($sp)
	ext	$2,$2,0,31
	sw	$2,16($sp)
	lw	$4,16($sp)
	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$5,$2,0x124
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$2,$2,0x108
	sw	$2,64($sp)
	lw	$2,64($sp)
	lw	$2,0($2)
	sw	$2,68($sp)
	lw	$2,64($sp)
	lw	$2,0($2)
	sw	$2,68($sp)
	lw	$2,68($sp)
	sw	$2,20($sp)
	lw	$3,20($sp)
	li	$2,-2013265920			# 0xffffffff88000000
	or	$2,$3,$2
	sw	$2,16($sp)
	lw	$4,16($sp)
	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$5,$2,0x108
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	li	$4,1000			# 0x3e8
	lui	$2,%hi(__udelay)
	addiu	$2,$2,%lo(__udelay)
	jalr	$2
	nop

	lw	$4,20($sp)
	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$5,$2,0x108
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$2,$2,0x108
	sw	$2,72($sp)
	lw	$2,72($sp)
	lw	$2,0($2)
	sw	$2,76($sp)
	lw	$2,72($sp)
	lw	$2,0($2)
	sw	$2,76($sp)
	lw	$2,76($sp)
	sw	$2,20($sp)
	lw	$2,20($sp)
	ori	$2,$2,0x20
	sw	$2,16($sp)
	lw	$4,16($sp)
	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$5,$2,0x108
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

$L37:
	lw	$3,92($sp)
	li	$2,1			# 0x1
	bne	$3,$2,$L42
	nop

	li	$4,1			# 0x1
	lui	$2,%hi(phy_trans_power_switch)
	addiu	$2,$2,%lo(phy_trans_power_switch)
	jalr	$2
	nop

$L42:
	lui	$2,%hi(gpPhyPriv)
	lw	$3,%lo(gpPhyPriv)($2)
	lw	$2,88($sp)
	andi	$2,$2,0x00ff
	andi	$2,$2,0x3
	andi	$4,$2,0x00ff
	lbu	$2,132($3)
	ins	$2,$4,1,2
	sb	$2,132($3)
	move	$2,$0
$L31:
	lw	$31,84($sp)
	addiu	$sp,$sp,88
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	phy_mode_config
	.size	phy_mode_config, .-phy_mode_config
	.section	.text.phy_set_epon_ts_continue_mode,"ax",@progbits
	.align	2
	.globl	phy_set_epon_ts_continue_mode
	.set	nomips16
	.set	nomicromips
	.ent	phy_set_epon_ts_continue_mode
	.type	phy_set_epon_ts_continue_mode, @function
phy_set_epon_ts_continue_mode:
	.frame	$sp,48,$31		# vars= 24, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-48
	sw	$31,44($sp)
	sw	$4,48($sp)
	sw	$0,16($sp)
	lw	$3,48($sp)
	li	$2,1			# 0x1
	bne	$3,$2,$L44
	nop

	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$2,$2,0x124
	sw	$2,20($sp)
	lw	$2,20($sp)
	lw	$2,0($2)
	sw	$2,24($sp)
	lw	$2,20($sp)
	lw	$2,0($2)
	sw	$2,24($sp)
	lw	$2,24($sp)
	sw	$2,16($sp)
	lw	$3,16($sp)
	li	$2,67108864			# 0x4000000
	or	$2,$3,$2
	sw	$2,16($sp)
	lw	$4,16($sp)
	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$5,$2,0x124
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	j	$L46
	nop

$L44:
	lw	$2,48($sp)
	bne	$2,$0,$L46
	nop

	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$2,$2,0x124
	sw	$2,28($sp)
	lw	$2,28($sp)
	lw	$2,0($2)
	sw	$2,32($sp)
	lw	$2,28($sp)
	lw	$2,0($2)
	sw	$2,32($sp)
	lw	$2,32($sp)
	sw	$2,16($sp)
	lw	$3,16($sp)
	li	$2,-67174400			# 0xfffffffffbff0000
	ori	$2,$2,0xffff
	and	$2,$3,$2
	sw	$2,16($sp)
	lw	$4,16($sp)
	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$5,$2,0x124
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

$L46:
	move	$2,$0
	lw	$31,44($sp)
	addiu	$sp,$sp,48
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	phy_set_epon_ts_continue_mode
	.size	phy_set_epon_ts_continue_mode, .-phy_set_epon_ts_continue_mode
	.section	.text.phy_round_trip_delay_sof,"ax",@progbits
	.align	2
	.globl	phy_round_trip_delay_sof
	.set	nomips16
	.set	nomicromips
	.ent	phy_round_trip_delay_sof
	.type	phy_round_trip_delay_sof, @function
phy_round_trip_delay_sof:
	.frame	$sp,16,$31		# vars= 16, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-16
	sw	$0,0($sp)
	sh	$0,4($sp)
	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$2,$2,0x2d4
	sw	$2,8($sp)
	lw	$2,8($sp)
	lw	$2,0($2)
	sw	$2,12($sp)
	lw	$2,8($sp)
	lw	$2,0($2)
	sw	$2,12($sp)
	lw	$2,12($sp)
	sw	$2,0($sp)
	lw	$2,0($sp)
	sh	$2,4($sp)
	lhu	$2,4($sp)
	addiu	$sp,$sp,16
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	phy_round_trip_delay_sof
	.size	phy_round_trip_delay_sof, .-phy_round_trip_delay_sof
	.rdata
	.align	2
$LC7:
	.ascii	"[%lu]0ms[%s:%d]\012  eponMode_phy_reset 1:isPowerDisable"
	.ascii	" = %d,read_data=0x%.8x\012\000"
	.align	2
$LC8:
	.ascii	"[%lu]0ms[%s:%d]\012  eponMode_phy_reset:isPowerDisable ="
	.ascii	" %d\012\000"
	.section	.text.eponMode_phy_reset,"ax",@progbits
	.align	2
	.globl	eponMode_phy_reset
	.set	nomips16
	.set	nomicromips
	.ent	eponMode_phy_reset
	.type	eponMode_phy_reset, @function
eponMode_phy_reset:
	.frame	$sp,80,$31		# vars= 48, regs= 2/0, args= 24, gp= 0
	.mask	0x80010000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-80
	sw	$31,76($sp)
	sw	$16,72($sp)
	sw	$0,32($sp)
	sw	$0,36($sp)
	sw	$0,24($sp)
	sw	$0,28($sp)
	li	$2,-1078001664			# 0xffffffffbfbf0000
	ori	$4,$2,0x204
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	sw	$2,36($sp)
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,136($2)
	andi	$2,$2,0x10
	beq	$2,$0,$L53
	nop

	lui	$2,%hi(jiffies)
	lw	$16,%lo(jiffies)($2)
	lui	$2,%hi($LC2)
	addiu	$4,$2,%lo($LC2)
	li	$5,47			# 0x2f
	lui	$2,%hi(strrchr)
	addiu	$2,$2,%lo(strrchr)
	jalr	$2
	nop

	addiu	$3,$2,1
	lw	$2,24($sp)
	sw	$2,16($sp)
	lw	$2,36($sp)
	sw	$2,20($sp)
	lui	$2,%hi($LC7)
	addiu	$4,$2,%lo($LC7)
	move	$5,$16
	move	$6,$3
	li	$7,444			# 0x1bc
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L53:
	lui	$2,%hi(mt7570_select)
	lw	$3,%lo(mt7570_select)($2)
	li	$2,1			# 0x1
	bne	$3,$2,$L54
	nop

	li	$2,33554432			# 0x2000000
	sw	$2,28($sp)
	j	$L55
	nop

$L54:
	li	$2,33554432			# 0x2000000
	sw	$2,28($sp)
$L55:
	lw	$3,36($sp)
	lw	$2,28($sp)
	and	$2,$3,$2
	beq	$2,$0,$L56
	nop

	li	$2,1			# 0x1
	sw	$2,24($sp)
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,136($2)
	andi	$2,$2,0x10
	beq	$2,$0,$L56
	nop

	lui	$2,%hi(jiffies)
	lw	$16,%lo(jiffies)($2)
	lui	$2,%hi($LC2)
	addiu	$4,$2,%lo($LC2)
	li	$5,47			# 0x2f
	lui	$2,%hi(strrchr)
	addiu	$2,$2,%lo(strrchr)
	jalr	$2
	nop

	addiu	$3,$2,1
	lw	$2,24($sp)
	sw	$2,16($sp)
	lui	$2,%hi($LC8)
	addiu	$4,$2,%lo($LC8)
	move	$5,$16
	move	$6,$3
	li	$7,471			# 0x1d7
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L56:
	move	$4,$0
	lui	$2,%hi(phy_trans_power_switch)
	addiu	$2,$2,%lo(phy_trans_power_switch)
	jalr	$2
	nop

	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,136($2)
	andi	$2,$2,0x10
	beq	$2,$0,$L57
	nop

	lui	$2,%hi(jiffies)
	lw	$16,%lo(jiffies)($2)
	lui	$2,%hi($LC2)
	addiu	$4,$2,%lo($LC2)
	li	$5,47			# 0x2f
	lui	$2,%hi(strrchr)
	addiu	$2,$2,%lo(strrchr)
	jalr	$2
	nop

	addiu	$3,$2,1
	lw	$2,24($sp)
	sw	$2,16($sp)
	lw	$2,36($sp)
	sw	$2,20($sp)
	lui	$2,%hi($LC7)
	addiu	$4,$2,%lo($LC7)
	move	$5,$16
	move	$6,$3
	li	$7,477			# 0x1dd
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L57:
	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$2,$2,0x108
	sw	$2,40($sp)
	lw	$2,40($sp)
	lw	$2,0($2)
	sw	$2,44($sp)
	lw	$2,40($sp)
	lw	$2,0($2)
	sw	$2,44($sp)
	lw	$2,44($sp)
	sw	$2,36($sp)
	lw	$3,36($sp)
	li	$2,-33			# 0xffffffffffffffdf
	and	$2,$3,$2
	sw	$2,32($sp)
	lw	$4,32($sp)
	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$5,$2,0x108
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$2,$2,0x124
	sw	$2,48($sp)
	lw	$2,48($sp)
	lw	$2,0($2)
	sw	$2,52($sp)
	lw	$2,48($sp)
	lw	$2,0($2)
	sw	$2,52($sp)
	lw	$2,52($sp)
	sw	$2,36($sp)
	lw	$2,36($sp)
	ext	$2,$2,0,31
	sw	$2,32($sp)
	lw	$4,32($sp)
	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$5,$2,0x124
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$2,$2,0x108
	sw	$2,56($sp)
	lw	$2,56($sp)
	lw	$2,0($2)
	sw	$2,60($sp)
	lw	$2,56($sp)
	lw	$2,0($2)
	sw	$2,60($sp)
	lw	$2,60($sp)
	sw	$2,36($sp)
	lw	$3,36($sp)
	li	$2,-2013265920			# 0xffffffff88000000
	or	$2,$3,$2
	sw	$2,32($sp)
	lw	$4,32($sp)
	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$5,$2,0x108
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	li	$4,1000			# 0x3e8
	lui	$2,%hi(__udelay)
	addiu	$2,$2,%lo(__udelay)
	jalr	$2
	nop

	lw	$4,36($sp)
	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$5,$2,0x108
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$2,$2,0x108
	sw	$2,64($sp)
	lw	$2,64($sp)
	lw	$2,0($2)
	sw	$2,68($sp)
	lw	$2,64($sp)
	lw	$2,0($2)
	sw	$2,68($sp)
	lw	$2,68($sp)
	sw	$2,36($sp)
	lw	$2,36($sp)
	ori	$2,$2,0x20
	sw	$2,32($sp)
	lw	$4,32($sp)
	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$5,$2,0x108
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	lw	$2,24($sp)
	bne	$2,$0,$L62
	nop

	li	$4,1			# 0x1
	lui	$2,%hi(phy_trans_power_switch)
	addiu	$2,$2,%lo(phy_trans_power_switch)
	jalr	$2
	nop

	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,136($2)
	andi	$2,$2,0x10
	beq	$2,$0,$L64
	nop

	lui	$2,%hi(jiffies)
	lw	$16,%lo(jiffies)($2)
	lui	$2,%hi($LC2)
	addiu	$4,$2,%lo($LC2)
	li	$5,47			# 0x2f
	lui	$2,%hi(strrchr)
	addiu	$2,$2,%lo(strrchr)
	jalr	$2
	nop

	addiu	$3,$2,1
	lw	$2,24($sp)
	sw	$2,16($sp)
	lw	$2,36($sp)
	sw	$2,20($sp)
	lui	$2,%hi($LC7)
	addiu	$4,$2,%lo($LC7)
	move	$5,$16
	move	$6,$3
	li	$7,510			# 0x1fe
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	j	$L64
	nop

$L62:
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lw	$3,132($2)
	li	$2,-2147483648			# 0xffffffff80000000
	and	$2,$3,$2
	beq	$2,$0,$L64
	nop

	li	$4,1			# 0x1
	lui	$2,%hi(phy_trans_power_switch)
	addiu	$2,$2,%lo(phy_trans_power_switch)
	jalr	$2
	nop

	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,136($2)
	andi	$2,$2,0x10
	beq	$2,$0,$L64
	nop

	lui	$2,%hi(jiffies)
	lw	$16,%lo(jiffies)($2)
	lui	$2,%hi($LC2)
	addiu	$4,$2,%lo($LC2)
	li	$5,47			# 0x2f
	lui	$2,%hi(strrchr)
	addiu	$2,$2,%lo(strrchr)
	jalr	$2
	nop

	addiu	$3,$2,1
	lw	$2,24($sp)
	sw	$2,16($sp)
	lw	$2,36($sp)
	sw	$2,20($sp)
	lui	$2,%hi($LC7)
	addiu	$4,$2,%lo($LC7)
	move	$5,$16
	move	$6,$3
	li	$7,514			# 0x202
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L64:
	move	$2,$0
	lw	$31,76($sp)
	lw	$16,72($sp)
	addiu	$sp,$sp,80
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	eponMode_phy_reset
	.size	eponMode_phy_reset, .-eponMode_phy_reset
	.section	.text.phy_ready_status,"ax",@progbits
	.align	2
	.globl	phy_ready_status
	.set	nomips16
	.set	nomicromips
	.ent	phy_ready_status
	.type	phy_ready_status, @function
phy_ready_status:
	.frame	$sp,16,$31		# vars= 16, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-16
	sw	$0,0($sp)
	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$2,$2,0x130
	sw	$2,4($sp)
	lw	$2,4($sp)
	lw	$2,0($2)
	sw	$2,8($sp)
	lw	$2,4($sp)
	lw	$2,0($2)
	sw	$2,8($sp)
	lw	$2,8($sp)
	sw	$2,0($sp)
	lw	$2,0($sp)
	srl	$2,$2,18
	andi	$3,$2,0x7
	li	$2,6			# 0x6
	bne	$3,$2,$L68
	nop

	li	$2,1			# 0x1
	j	$L69
	nop

$L68:
	move	$2,$0
$L69:
	addiu	$sp,$sp,16
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	phy_ready_status
	.size	phy_ready_status, .-phy_ready_status
	.rdata
	.align	2
$LC9:
	.ascii	"[%lu]0ms[%s:%d]\012  Phy LOS. \012\000"
	.section	.text.phy_los_status,"ax",@progbits
	.align	2
	.globl	phy_los_status
	.set	nomips16
	.set	nomicromips
	.ent	phy_los_status
	.type	phy_los_status, @function
phy_los_status:
	.frame	$sp,40,$31		# vars= 16, regs= 2/0, args= 16, gp= 0
	.mask	0x80010000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-40
	sw	$31,36($sp)
	sw	$16,32($sp)
	sw	$0,16($sp)
	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$2,$2,0x5e0
	sw	$2,20($sp)
	lw	$2,20($sp)
	lw	$2,0($2)
	sw	$2,24($sp)
	lw	$2,20($sp)
	lw	$2,0($2)
	sw	$2,24($sp)
	lw	$2,24($sp)
	sw	$2,16($sp)
	lw	$2,16($sp)
	andi	$2,$2,0x1
	beq	$2,$0,$L72
	nop

	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,136($2)
	andi	$2,$2,0x10
	beq	$2,$0,$L73
	nop

	lui	$2,%hi(jiffies)
	lw	$16,%lo(jiffies)($2)
	lui	$2,%hi($LC2)
	addiu	$4,$2,%lo($LC2)
	li	$5,47			# 0x2f
	lui	$2,%hi(strrchr)
	addiu	$2,$2,%lo(strrchr)
	jalr	$2
	nop

	addiu	$3,$2,1
	lui	$2,%hi($LC9)
	addiu	$4,$2,%lo($LC9)
	move	$5,$16
	move	$6,$3
	li	$7,564			# 0x234
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L73:
	li	$2,1			# 0x1
	j	$L74
	nop

$L72:
	move	$2,$0
$L74:
	lw	$31,36($sp)
	lw	$16,32($sp)
	addiu	$sp,$sp,40
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	phy_los_status
	.size	phy_los_status, .-phy_los_status
	.rdata
	.align	2
$LC10:
	.ascii	"[%lu]0ms[%s:%d]\012  LOF Interrupt is not turned on. \012"
	.ascii	"\000"
	.align	2
$LC11:
	.ascii	"[%lu]0ms[%s:%d]\012  Phy LOF. \012\000"
	.align	2
$LC12:
	.ascii	"[%lu]0ms[%s:%d]\012  Phy not LOF. \012\000"
	.section	.text.phy_lof_status,"ax",@progbits
	.align	2
	.globl	phy_lof_status
	.set	nomips16
	.set	nomicromips
	.ent	phy_lof_status
	.type	phy_lof_status, @function
phy_lof_status:
	.frame	$sp,48,$31		# vars= 24, regs= 2/0, args= 16, gp= 0
	.mask	0x80010000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-48
	sw	$31,44($sp)
	sw	$16,40($sp)
	sw	$0,16($sp)
	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$2,$2,0x5f0
	sw	$2,20($sp)
	lw	$2,20($sp)
	lw	$2,0($2)
	sw	$2,24($sp)
	lw	$2,20($sp)
	lw	$2,0($2)
	sw	$2,24($sp)
	lw	$2,24($sp)
	sw	$2,16($sp)
	lw	$2,16($sp)
	andi	$2,$2,0x2
	bne	$2,$0,$L77
	nop

	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,136($2)
	andi	$2,$2,0x10
	beq	$2,$0,$L78
	nop

	lui	$2,%hi(jiffies)
	lw	$16,%lo(jiffies)($2)
	lui	$2,%hi($LC2)
	addiu	$4,$2,%lo($LC2)
	li	$5,47			# 0x2f
	lui	$2,%hi(strrchr)
	addiu	$2,$2,%lo(strrchr)
	jalr	$2
	nop

	addiu	$3,$2,1
	lui	$2,%hi($LC10)
	addiu	$4,$2,%lo($LC10)
	move	$5,$16
	move	$6,$3
	li	$7,594			# 0x252
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L78:
	move	$2,$0
	j	$L79
	nop

$L77:
	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$2,$2,0x5f8
	sw	$2,28($sp)
	lw	$2,28($sp)
	lw	$2,0($2)
	sw	$2,32($sp)
	lw	$2,28($sp)
	lw	$2,0($2)
	sw	$2,32($sp)
	lw	$2,32($sp)
	sw	$2,16($sp)
	lw	$2,16($sp)
	andi	$2,$2,0x2
	beq	$2,$0,$L81
	nop

	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,136($2)
	andi	$2,$2,0x10
	beq	$2,$0,$L82
	nop

	lui	$2,%hi(jiffies)
	lw	$16,%lo(jiffies)($2)
	lui	$2,%hi($LC2)
	addiu	$4,$2,%lo($LC2)
	li	$5,47			# 0x2f
	lui	$2,%hi(strrchr)
	addiu	$2,$2,%lo(strrchr)
	jalr	$2
	nop

	addiu	$3,$2,1
	lui	$2,%hi($LC11)
	addiu	$4,$2,%lo($LC11)
	move	$5,$16
	move	$6,$3
	li	$7,602			# 0x25a
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L82:
	li	$2,1			# 0x1
	j	$L79
	nop

$L81:
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,136($2)
	andi	$2,$2,0x10
	beq	$2,$0,$L83
	nop

	lui	$2,%hi(jiffies)
	lw	$16,%lo(jiffies)($2)
	lui	$2,%hi($LC2)
	addiu	$4,$2,%lo($LC2)
	li	$5,47			# 0x2f
	lui	$2,%hi(strrchr)
	addiu	$2,$2,%lo(strrchr)
	jalr	$2
	nop

	addiu	$3,$2,1
	lui	$2,%hi($LC12)
	addiu	$4,$2,%lo($LC12)
	move	$5,$16
	move	$6,$3
	li	$7,607			# 0x25f
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L83:
	move	$2,$0
$L79:
	lw	$31,44($sp)
	lw	$16,40($sp)
	addiu	$sp,$sp,48
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	phy_lof_status
	.size	phy_lof_status, .-phy_lof_status
	.section	.text.phy_reset_counter,"ax",@progbits
	.align	2
	.globl	phy_reset_counter
	.set	nomips16
	.set	nomicromips
	.ent	phy_reset_counter
	.type	phy_reset_counter, @function
phy_reset_counter:
	.frame	$sp,40,$31		# vars= 16, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-40
	sw	$31,36($sp)
	sw	$0,16($sp)
	sw	$0,20($sp)
	li	$4,2			# 0x2
	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$5,$2,0x234
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	li	$4,8			# 0x8
	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$5,$2,0x234
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	li	$4,32			# 0x20
	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$5,$2,0x234
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$2,$2,0x424
	sw	$2,24($sp)
	lw	$2,24($sp)
	lw	$2,0($2)
	sw	$2,28($sp)
	lw	$2,24($sp)
	lw	$2,0($2)
	sw	$2,28($sp)
	lw	$2,28($sp)
	sw	$2,16($sp)
	lw	$2,16($sp)
	ori	$2,$2,0x4
	sw	$2,20($sp)
	lw	$4,20($sp)
	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$5,$2,0x424
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	li	$4,16			# 0x10
	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$5,$2,0x298
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	move	$2,$0
	lw	$31,36($sp)
	addiu	$sp,$sp,40
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	phy_reset_counter
	.size	phy_reset_counter, .-phy_reset_counter
	.section	.text.phy_cnt_enable,"ax",@progbits
	.align	2
	.globl	phy_cnt_enable
	.set	nomips16
	.set	nomicromips
	.ent	phy_cnt_enable
	.type	phy_cnt_enable, @function
phy_cnt_enable:
	.frame	$sp,48,$31		# vars= 24, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-48
	sw	$31,44($sp)
	move	$3,$5
	move	$2,$6
	sb	$4,48($sp)
	sb	$3,52($sp)
	sb	$2,56($sp)
	sw	$0,24($sp)
	sw	$0,16($sp)
	sw	$0,20($sp)
	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$2,$2,0x230
	sw	$2,28($sp)
	lw	$2,28($sp)
	lw	$2,0($2)
	sw	$2,32($sp)
	lw	$2,28($sp)
	lw	$2,0($2)
	sw	$2,32($sp)
	lw	$2,32($sp)
	sw	$2,16($sp)
	lw	$3,16($sp)
	li	$2,-8			# 0xfffffffffffffff8
	and	$2,$3,$2
	sw	$2,16($sp)
	lbu	$3,48($sp)
	li	$2,1			# 0x1
	beq	$3,$2,$L89
	nop

	lbu	$2,48($sp)
	bne	$2,$0,$L90
	nop

$L89:
	lbu	$2,48($sp)
	lw	$3,16($sp)
	or	$2,$3,$2
	sw	$2,16($sp)
	lw	$2,20($sp)
	ori	$2,$2,0x1
	sw	$2,20($sp)
$L90:
	lbu	$3,52($sp)
	li	$2,1			# 0x1
	beq	$3,$2,$L91
	nop

	lbu	$2,52($sp)
	bne	$2,$0,$L92
	nop

$L91:
	lbu	$2,52($sp)
	sll	$2,$2,1
	move	$3,$2
	lw	$2,16($sp)
	or	$2,$2,$3
	sw	$2,16($sp)
	lw	$2,20($sp)
	ori	$2,$2,0x2
	sw	$2,20($sp)
$L92:
	lbu	$3,56($sp)
	li	$2,1			# 0x1
	beq	$3,$2,$L93
	nop

	lbu	$2,56($sp)
	bne	$2,$0,$L94
	nop

$L93:
	lbu	$2,56($sp)
	sll	$2,$2,2
	move	$3,$2
	lw	$2,16($sp)
	or	$2,$2,$3
	sw	$2,16($sp)
	lw	$2,20($sp)
	ori	$2,$2,0x4
	sw	$2,20($sp)
$L94:
	lw	$2,16($sp)
	sw	$2,24($sp)
	li	$4,1			# 0x1
	lui	$2,%hi(__udelay)
	addiu	$2,$2,%lo(__udelay)
	jalr	$2
	nop

	lw	$4,24($sp)
	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$5,$2,0x230
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	lw	$2,20($sp)
	andi	$2,$2,0x00ff
	move	$4,$2
	lui	$2,%hi(phy_counter_clear)
	addiu	$2,$2,%lo(phy_counter_clear)
	jalr	$2
	nop

	move	$2,$0
	lw	$31,44($sp)
	addiu	$sp,$sp,48
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	phy_cnt_enable
	.size	phy_cnt_enable, .-phy_cnt_enable
	.globl	phy_trans_spt_list
	.data
	.align	2
	.type	phy_trans_spt_list, @object
	.size	phy_trans_spt_list, 1476
phy_trans_spt_list:
	.ascii	"WTD\000"
	.space	13
	.ascii	"RTXM167-407-C10\000"
	.space	1
	.half	463
	.ascii	"WTD\000"
	.space	13
	.ascii	"RTXM169-504\000"
	.space	5
	.half	463
	.ascii	"WTD\000"
	.space	13
	.ascii	"RTXM169-514\000"
	.space	5
	.half	463
	.ascii	"SOURCEPHOTONICS\000"
	.space	1
	.ascii	"SFA3424THPCDFJ\000"
	.space	2
	.half	335
	.ascii	"SOURCEPHOTONICS\000"
	.space	1
	.ascii	"SFU34GBP2CDFA\000"
	.space	3
	.half	463
	.ascii	"Hisense\000"
	.space	9
	.ascii	"LTB34D2-SC+\000"
	.space	5
	.half	463
	.ascii	"Hisense\000"
	.space	9
	.ascii	"LTB34D3-SC+\000"
	.space	5
	.half	463
	.ascii	"Hisense\000"
	.space	9
	.ascii	"LTE3468L-BHG+\000"
	.space	3
	.half	399
	.ascii	"Hisense\000"
	.space	9
	.ascii	"LTB3468-BHG+\000"
	.space	4
	.half	335
	.ascii	"DARE TECH OPT\000"
	.space	3
	.ascii	"E31LA21B+\000"
	.space	7
	.half	463
	.ascii	"APAC Opto\000"
	.space	7
	.ascii	"LSF2C3STCN322\000"
	.space	3
	.half	335
	.ascii	"APAC Opto\000"
	.space	7
	.ascii	"LSF2C3MTCN3G4\000"
	.space	3
	.half	335
	.ascii	"MENTECHOPTO\000"
	.space	5
	.ascii	"NOG22-D6C-ND\000"
	.space	4
	.half	335
	.ascii	"DELTA\000"
	.space	11
	.ascii	"OPEP-33-A4G1RC\000"
	.space	2
	.half	463
	.ascii	"MENTECHOPTO\000"
	.space	5
	.ascii	"NOE22-LD6C-NT\000"
	.space	3
	.half	463
	.ascii	"SUPERXON LTD.\000"
	.space	3
	.ascii	"SOEB3466-FSGF\000"
	.space	3
	.half	463
	.ascii	"EOPTOLINK INC\000"
	.space	3
	.ascii	"EOLF-GUA-25-DZTE\000"
	.half	335
	.ascii	"MENTECHOPTO\000"
	.space	5
	.ascii	"NOG22-D6C-ND\000"
	.space	4
	.half	335
	.ascii	"SUPERXON LTD.\000"
	.space	3
	.ascii	"SOGP3412-FSGA\000"
	.space	3
	.half	335
	.ascii	"SUPERXON LTD.\000"
	.space	3
	.ascii	"SOEB3466-FUGF\000"
	.space	3
	.half	463
	.ascii	"WTD\000"
	.space	13
	.ascii	"RTXM167-407\000"
	.space	5
	.half	463
	.ascii	"XUGUANG LTD.\000"
	.space	4
	.ascii	"XTEC3411F-C\000"
	.space	5
	.half	463
	.ascii	"XUGUANG LTD.\000"
	.space	4
	.ascii	"XTGP3434FRS-A\000"
	.space	3
	.half	335
	.ascii	"T&W\000"
	.space	13
	.ascii	"TW2362G-CDEH\000"
	.space	4
	.half	271
	.ascii	"HUAWEI\000"
	.space	10
	.ascii	"HPSP2120\000"
	.space	8
	.half	463
	.ascii	"Hisense\000"
	.space	9
	.ascii	"LTE3468-BCG+\000"
	.space	4
	.half	271
	.ascii	"Hisense\000"
	.space	9
	.ascii	"LTE34D3-SH+\000"
	.space	5
	.half	463
	.ascii	"HONLUS\000"
	.space	10
	.ascii	"HOLSFP3420330C\000"
	.space	2
	.half	463
	.ascii	"HONLUS\000"
	.space	10
	.ascii	"HOLSP3412452DC0C\000"
	.half	463
	.ascii	"T&W\000"
	.space	13
	.ascii	"TW2362G-CDEH\000"
	.space	4
	.half	271
	.ascii	"EOPTOLINK INC\000"
	.space	3
	.ascii	"EOLF-GEUA-25-D1A\000"
	.half	463
	.ascii	"Q-STAR\000"
	.space	10
	.ascii	"BYFS-462A4-HB-12\000"
	.half	335
	.ascii	"Q-STAR\000"
	.space	10
	.ascii	"BYFS-44234-LB-12\000"
	.half	463
	.ascii	"MENTECHOPTO\000"
	.space	5
	.ascii	"NOG22-D6C-SD\000"
	.space	4
	.half	335
	.ascii	"MENTECHOPTO\000"
	.space	5
	.ascii	"NOE22-LD6C-SD\000"
	.space	3
	.half	463
	.ascii	"MENTECHOPTO\000"
	.space	5
	.ascii	"NOG22-LD6C-ST\000"
	.space	3
	.half	335
	.ascii	"MENTECHOPTO\000"
	.space	5
	.ascii	"NOE22-LD6C-ST\000"
	.space	3
	.half	463
	.ascii	"FIBERTOWER\000"
	.space	6
	.ascii	"FEU3411S-2DC-0C\000"
	.space	1
	.half	463
	.ascii	"FIBERTOWER\000"
	.space	6
	.ascii	"FGU3412S-2DC-1C\000"
	.space	1
	.half	335
	.ascii	"ZKTEL\000"
	.space	11
	.ascii	"ZP4342034-KCST\000"
	.space	2
	.half	335
	.ascii	"\000"
	.space	16
	.ascii	"\000"
	.space	16
	.half	0
	.section	.text.rtrim,"ax",@progbits
	.align	2
	.globl	rtrim
	.set	nomips16
	.set	nomicromips
	.ent	rtrim
	.type	rtrim, @function
rtrim:
	.frame	$sp,32,$31		# vars= 8, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-32
	sw	$31,28($sp)
	sw	$4,32($sp)
	sw	$0,16($sp)
	lw	$4,32($sp)
	lui	$2,%hi(strlen)
	addiu	$2,$2,%lo(strlen)
	jalr	$2
	nop

	sw	$2,20($sp)
	lw	$2,20($sp)
	addiu	$2,$2,-1
	sw	$2,16($sp)
	j	$L97
	nop

$L99:
	lw	$2,16($sp)
	lw	$3,32($sp)
	addu	$2,$3,$2
	sb	$0,0($2)
	lw	$2,16($sp)
	addiu	$2,$2,-1
	sw	$2,16($sp)
$L97:
	lw	$2,16($sp)
	bltz	$2,$L98
	nop

	lw	$2,16($sp)
	lw	$3,32($sp)
	addu	$2,$3,$2
	lb	$3,0($2)
	li	$2,32			# 0x20
	beq	$3,$2,$L99
	nop

$L98:
	lw	$2,32($sp)
	lw	$31,28($sp)
	addiu	$sp,$sp,32
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	rtrim
	.size	rtrim, .-rtrim
	.rdata
	.align	2
$LC13:
	.ascii	"vendor id : %s\012\000"
	.align	2
$LC14:
	.ascii	"vendor pn : %s\012\000"
	.align	2
$LC15:
	.ascii	"Match Vendor\012\000"
	.align	2
$LC16:
	.ascii	"\000"
	.section	.text.phy_trans_model_setting,"ax",@progbits
	.align	2
	.globl	phy_trans_model_setting
	.set	nomips16
	.set	nomicromips
	.ent	phy_trans_model_setting
	.type	phy_trans_model_setting, @function
phy_trans_model_setting:
	.frame	$sp,104,$31		# vars= 56, regs= 3/0, args= 32, gp= 0
	.mask	0x80030000,-4
	.fmask	0x00000000,0
	addiu	$sp,$sp,-104
	sw	$31,100($sp)
	sw	$17,96($sp)
	sw	$16,92($sp)
	sw	$0,36($sp)
	lui	$2,%hi(mt7570_select)
	lw	$3,%lo(mt7570_select)($2)
	li	$2,1			# 0x1
	bne	$3,$2,$L102
	lui	$2,%hi(mt7570_trans_model_setting)
	addiu	$2,$2,%lo(mt7570_trans_model_setting)
	jalr	$2
	j	$L101
$L102:
	sw	$0,32($sp)
	lui	$2,%hi(vendor_id.25118)
	addiu	$4,$2,%lo(vendor_id.25118)
	lui	$2,%hi(strlen)
	addiu	$2,$2,%lo(strlen)
	jalr	$2
	bne	$2,$0,$L104
	lui	$2,%hi(vendor_pn.25119)
	addiu	$4,$2,%lo(vendor_pn.25119)
	lui	$2,%hi(strlen)
	addiu	$2,$2,%lo(strlen)
	jalr	$2
	bne	$2,$0,$L104
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,116($2)
	move	$3,$2
	li	$2,20			# 0x14
	sw	$2,16($sp)
	lui	$2,%hi(vendor_id.25118)
	addiu	$2,$2,%lo(vendor_id.25118)
	sw	$2,20($sp)
	li	$2,16			# 0x10
	sw	$2,24($sp)
	move	$4,$0
	move	$5,$3
	li	$6,80			# 0x50
	li	$7,1			# 0x1
	lui	$2,%hi(phy_I2C_read)
	addiu	$2,$2,%lo(phy_I2C_read)
	jalr	$2
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,116($2)
	move	$3,$2
	li	$2,40			# 0x28
	sw	$2,16($sp)
	lui	$2,%hi(vendor_pn.25119)
	addiu	$2,$2,%lo(vendor_pn.25119)
	sw	$2,20($sp)
	li	$2,16			# 0x10
	sw	$2,24($sp)
	move	$4,$0
	move	$5,$3
	li	$6,80			# 0x50
	li	$7,1			# 0x1
	lui	$2,%hi(phy_I2C_read)
	addiu	$2,$2,%lo(phy_I2C_read)
	jalr	$2
	lui	$2,%hi(vendor_pn.25119)
	addiu	$2,$2,%lo(vendor_pn.25119)
	sb	$0,16($2)
	lui	$2,%hi(vendor_id.25118)
	addiu	$2,$2,%lo(vendor_id.25118)
	sb	$0,16($2)
	lui	$2,%hi(vendor_id.25118)
	addiu	$4,$2,%lo(vendor_id.25118)
	lui	$2,%hi(rtrim)
	addiu	$2,$2,%lo(rtrim)
	jalr	$2
	lui	$2,%hi(vendor_pn.25119)
	addiu	$4,$2,%lo(vendor_pn.25119)
	lui	$2,%hi(rtrim)
	addiu	$2,$2,%lo(rtrim)
	jalr	$2
$L104:
	lw	$2,36($sp)
	beq	$2,$0,$L105
	lui	$2,%hi($LC13)
	addiu	$4,$2,%lo($LC13)
	lui	$2,%hi(vendor_id.25118)
	addiu	$5,$2,%lo(vendor_id.25118)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	lui	$2,%hi($LC14)
	addiu	$4,$2,%lo($LC14)
	lui	$2,%hi(vendor_pn.25119)
	addiu	$5,$2,%lo(vendor_pn.25119)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
$L105:
	lui	$2,%hi(phy_trans_spt_list)
	addiu	$2,$2,%lo(phy_trans_spt_list)
	sw	$2,32($sp)
	j	$L106
$L116:
	lw	$16,32($sp)
	lw	$2,32($sp)
	move	$4,$2
	lui	$2,%hi(strlen)
	addiu	$2,$2,%lo(strlen)
	jalr	$2
	move	$17,$2
	lui	$2,%hi(vendor_id.25118)
	addiu	$4,$2,%lo(vendor_id.25118)
	lui	$2,%hi(strlen)
	addiu	$2,$2,%lo(strlen)
	jalr	$2
	sltu	$2,$2,$17
	beq	$2,$0,$L107
	lw	$2,32($sp)
	move	$4,$2
	lui	$2,%hi(strlen)
	addiu	$2,$2,%lo(strlen)
	jalr	$2
	move	$3,$2
	j	$L108
$L107:
	lui	$2,%hi(vendor_id.25118)
	addiu	$4,$2,%lo(vendor_id.25118)
	lui	$2,%hi(strlen)
	addiu	$2,$2,%lo(strlen)
	jalr	$2
	move	$3,$2
$L108:
	sw	$16,40($sp)
	lui	$2,%hi(vendor_id.25118)
	addiu	$2,$2,%lo(vendor_id.25118)
	sw	$2,44($sp)
	sw	$3,48($sp)
	lw	$4,40($sp)
	lw	$3,44($sp)
	lw	$2,48($sp)
#APP
 # 108 "/opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/string.h" 1
	.set	noreorder
	.set	noat
1:	lbu	$5,($4)
	beqz	$2,2f
	lbu	$1,($3)
	subu	$2,1
	bne	$1,$5,3f
	addiu	$4,1
	bnez	$5,1b
	addiu	$3,1
2:
	move	$5,$1
3:	subu	$5,$1
	.set	at
	.set	reorder
 # 0 "" 2
#NO_APP
	sw	$4,40($sp)
	sw	$3,44($sp)
	sw	$2,48($sp)
	sw	$5,52($sp)
	lw	$2,52($sp)
	bne	$2,$0,$L110
	lw	$2,32($sp)
	addiu	$16,$2,17
	lw	$2,32($sp)
	addiu	$2,$2,17
	move	$4,$2
	lui	$2,%hi(strlen)
	addiu	$2,$2,%lo(strlen)
	jalr	$2
	move	$17,$2
	lui	$2,%hi(vendor_pn.25119)
	addiu	$4,$2,%lo(vendor_pn.25119)
	lui	$2,%hi(strlen)
	addiu	$2,$2,%lo(strlen)
	jalr	$2
	sltu	$2,$2,$17
	beq	$2,$0,$L111
	lw	$2,32($sp)
	addiu	$2,$2,17
	move	$4,$2
	lui	$2,%hi(strlen)
	addiu	$2,$2,%lo(strlen)
	jalr	$2
	move	$3,$2
	j	$L112
$L111:
	lui	$2,%hi(vendor_pn.25119)
	addiu	$4,$2,%lo(vendor_pn.25119)
	lui	$2,%hi(strlen)
	addiu	$2,$2,%lo(strlen)
	jalr	$2
	move	$3,$2
$L112:
	sw	$16,56($sp)
	lui	$2,%hi(vendor_pn.25119)
	addiu	$2,$2,%lo(vendor_pn.25119)
	sw	$2,60($sp)
	sw	$3,64($sp)
	lw	$4,56($sp)
	lw	$3,60($sp)
	lw	$2,64($sp)
#APP
 # 108 "/opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/string.h" 1
	.set	noreorder
	.set	noat
1:	lbu	$5,($4)
	beqz	$2,2f
	lbu	$1,($3)
	subu	$2,1
	bne	$1,$5,3f
	addiu	$4,1
	bnez	$5,1b
	addiu	$3,1
2:
	move	$5,$1
3:	subu	$5,$1
	.set	at
	.set	reorder
 # 0 "" 2
#NO_APP
	sw	$4,56($sp)
	sw	$3,60($sp)
	sw	$2,64($sp)
	sw	$5,68($sp)
	lw	$2,68($sp)
	bne	$2,$0,$L110
	lw	$2,36($sp)
	beq	$2,$0,$L114
	lui	$2,%hi($LC15)
	addiu	$4,$2,%lo($LC15)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
$L114:
	lw	$2,32($sp)
	lhu	$2,34($2)
	move	$4,$2
	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$5,$2,0x138
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	j	$L101
$L110:
	lw	$2,32($sp)
	addiu	$2,$2,36
	sw	$2,32($sp)
$L106:
	lw	$2,32($sp)
	sw	$2,72($sp)
	lui	$2,%hi($LC16)
	addiu	$2,$2,%lo($LC16)
	sw	$2,76($sp)
	lw	$3,72($sp)
	lw	$2,76($sp)
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
	sw	$3,72($sp)
	sw	$2,76($sp)
	sw	$4,80($sp)
	lw	$2,80($sp)
	bne	$2,$0,$L116
$L101:
	lw	$31,100($sp)
	lw	$17,96($sp)
	lw	$16,92($sp)
	addiu	$sp,$sp,104
	j	$31
	.end	phy_trans_model_setting
	.size	phy_trans_model_setting, .-phy_trans_model_setting
	.section	.text.phy_trans_param_status_real,"ax",@progbits
	.align	2
	.globl	phy_trans_param_status_real
	.set	nomips16
	.set	nomicromips
	.ent	phy_trans_param_status_real
	.type	phy_trans_param_status_real, @function
phy_trans_param_status_real:
	.frame	$sp,40,$31		# vars= 0, regs= 1/0, args= 32, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-40
	sw	$31,36($sp)
	sw	$4,40($sp)
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,116($2)
	move	$3,$2
	li	$2,96			# 0x60
	sw	$2,16($sp)
	lw	$2,40($sp)
	sw	$2,20($sp)
	li	$2,10			# 0xa
	sw	$2,24($sp)
	move	$4,$0
	move	$5,$3
	li	$6,81			# 0x51
	li	$7,1			# 0x1
	lui	$2,%hi(phy_I2C_read)
	addiu	$2,$2,%lo(phy_I2C_read)
	jalr	$2
	nop

	lw	$31,36($sp)
	addiu	$sp,$sp,40
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	phy_trans_param_status_real
	.size	phy_trans_param_status_real, .-phy_trans_param_status_real
	.rdata
	.align	2
$LC17:
	.ascii	"[%lu]0ms[%s:%d]\012  rx power high alarm threshold: %.8x"
	.ascii	"\012\000"
	.align	2
$LC18:
	.ascii	"[%lu]0ms[%s:%d]\012  rx power low alarm threshold: %.8x\012"
	.ascii	"\000"
	.align	2
$LC19:
	.ascii	"[%lu]0ms[%s:%d]\012  tx power high alarm threshold: %.8x"
	.ascii	"\012\000"
	.section	.text.phy_trans_alarm_getting,"ax",@progbits
	.align	2
	.globl	phy_trans_alarm_getting
	.set	nomips16
	.set	nomicromips
	.ent	phy_trans_alarm_getting
	.type	phy_trans_alarm_getting, @function
phy_trans_alarm_getting:
	.frame	$sp,48,$31		# vars= 8, regs= 2/0, args= 32, gp= 0
	.mask	0x80010000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-48
	sw	$31,44($sp)
	sw	$16,40($sp)
	sw	$4,48($sp)
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,116($2)
	move	$3,$2
	li	$2,32			# 0x20
	sw	$2,16($sp)
	addiu	$2,$sp,32
	sw	$2,20($sp)
	li	$2,2			# 0x2
	sw	$2,24($sp)
	move	$4,$0
	move	$5,$3
	li	$6,81			# 0x51
	li	$7,1			# 0x1
	lui	$2,%hi(phy_I2C_read)
	addiu	$2,$2,%lo(phy_I2C_read)
	jalr	$2
	nop

	lbu	$2,33($sp)
	seh	$3,$2
	lbu	$2,32($sp)
	sll	$2,$2,8
	seh	$2,$2
	or	$2,$3,$2
	seh	$2,$2
	andi	$3,$2,0xffff
	lw	$2,48($sp)
	sh	$3,0($2)
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,116($2)
	move	$3,$2
	li	$2,34			# 0x22
	sw	$2,16($sp)
	addiu	$2,$sp,32
	sw	$2,20($sp)
	li	$2,2			# 0x2
	sw	$2,24($sp)
	move	$4,$0
	move	$5,$3
	li	$6,81			# 0x51
	li	$7,1			# 0x1
	lui	$2,%hi(phy_I2C_read)
	addiu	$2,$2,%lo(phy_I2C_read)
	jalr	$2
	nop

	lbu	$2,33($sp)
	seh	$3,$2
	lbu	$2,32($sp)
	sll	$2,$2,8
	seh	$2,$2
	or	$2,$3,$2
	seh	$2,$2
	andi	$3,$2,0xffff
	lw	$2,48($sp)
	sh	$3,2($2)
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,116($2)
	move	$3,$2
	li	$2,24			# 0x18
	sw	$2,16($sp)
	addiu	$2,$sp,32
	sw	$2,20($sp)
	li	$2,2			# 0x2
	sw	$2,24($sp)
	move	$4,$0
	move	$5,$3
	li	$6,81			# 0x51
	li	$7,1			# 0x1
	lui	$2,%hi(phy_I2C_read)
	addiu	$2,$2,%lo(phy_I2C_read)
	jalr	$2
	nop

	lbu	$2,33($sp)
	seh	$3,$2
	lbu	$2,32($sp)
	sll	$2,$2,8
	seh	$2,$2
	or	$2,$3,$2
	seh	$2,$2
	andi	$3,$2,0xffff
	lw	$2,48($sp)
	sh	$3,4($2)
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,116($2)
	move	$3,$2
	li	$2,26			# 0x1a
	sw	$2,16($sp)
	addiu	$2,$sp,32
	sw	$2,20($sp)
	li	$2,2			# 0x2
	sw	$2,24($sp)
	move	$4,$0
	move	$5,$3
	li	$6,81			# 0x51
	li	$7,1			# 0x1
	lui	$2,%hi(phy_I2C_read)
	addiu	$2,$2,%lo(phy_I2C_read)
	jalr	$2
	nop

	lbu	$2,33($sp)
	seh	$3,$2
	lbu	$2,32($sp)
	sll	$2,$2,8
	seh	$2,$2
	or	$2,$3,$2
	seh	$2,$2
	andi	$3,$2,0xffff
	lw	$2,48($sp)
	sh	$3,6($2)
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,136($2)
	andi	$2,$2,0x10
	beq	$2,$0,$L119
	nop

	lui	$2,%hi(jiffies)
	lw	$16,%lo(jiffies)($2)
	lui	$2,%hi($LC2)
	addiu	$4,$2,%lo($LC2)
	li	$5,47			# 0x2f
	lui	$2,%hi(strrchr)
	addiu	$2,$2,%lo(strrchr)
	jalr	$2
	nop

	addiu	$3,$2,1
	lw	$2,48($sp)
	lhu	$2,0($2)
	sw	$2,16($sp)
	lui	$2,%hi($LC17)
	addiu	$4,$2,%lo($LC17)
	move	$5,$16
	move	$6,$3
	li	$7,1016			# 0x3f8
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L119:
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,136($2)
	andi	$2,$2,0x10
	beq	$2,$0,$L120
	nop

	lui	$2,%hi(jiffies)
	lw	$16,%lo(jiffies)($2)
	lui	$2,%hi($LC2)
	addiu	$4,$2,%lo($LC2)
	li	$5,47			# 0x2f
	lui	$2,%hi(strrchr)
	addiu	$2,$2,%lo(strrchr)
	jalr	$2
	nop

	addiu	$3,$2,1
	lw	$2,48($sp)
	lhu	$2,2($2)
	sw	$2,16($sp)
	lui	$2,%hi($LC18)
	addiu	$4,$2,%lo($LC18)
	move	$5,$16
	move	$6,$3
	li	$7,1017			# 0x3f9
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L120:
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,136($2)
	andi	$2,$2,0x10
	beq	$2,$0,$L121
	nop

	lui	$2,%hi(jiffies)
	lw	$16,%lo(jiffies)($2)
	lui	$2,%hi($LC2)
	addiu	$4,$2,%lo($LC2)
	li	$5,47			# 0x2f
	lui	$2,%hi(strrchr)
	addiu	$2,$2,%lo(strrchr)
	jalr	$2
	nop

	addiu	$3,$2,1
	lw	$2,48($sp)
	lhu	$2,4($2)
	sw	$2,16($sp)
	lui	$2,%hi($LC19)
	addiu	$4,$2,%lo($LC19)
	move	$5,$16
	move	$6,$3
	li	$7,1018			# 0x3fa
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L121:
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,136($2)
	andi	$2,$2,0x10
	beq	$2,$0,$L118
	nop

	lui	$2,%hi(jiffies)
	lw	$16,%lo(jiffies)($2)
	lui	$2,%hi($LC2)
	addiu	$4,$2,%lo($LC2)
	li	$5,47			# 0x2f
	lui	$2,%hi(strrchr)
	addiu	$2,$2,%lo(strrchr)
	jalr	$2
	nop

	addiu	$3,$2,1
	lw	$2,48($sp)
	lhu	$2,6($2)
	sw	$2,16($sp)
	lui	$2,%hi($LC18)
	addiu	$4,$2,%lo($LC18)
	move	$5,$16
	move	$6,$3
	li	$7,1019			# 0x3fb
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L118:
	lw	$31,44($sp)
	lw	$16,40($sp)
	addiu	$sp,$sp,48
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	phy_trans_alarm_getting
	.size	phy_trans_alarm_getting, .-phy_trans_alarm_getting
	.rdata
	.align	2
$LC20:
	.ascii	"[%lu]0ms[%s:%d]\012  error input : delay_value. \012\000"
	.section	.text.phy_bit_delay,"ax",@progbits
	.align	2
	.globl	phy_bit_delay
	.set	nomips16
	.set	nomicromips
	.ent	phy_bit_delay
	.type	phy_bit_delay, @function
phy_bit_delay:
	.frame	$sp,40,$31		# vars= 16, regs= 2/0, args= 16, gp= 0
	.mask	0x80010000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-40
	sw	$31,36($sp)
	sw	$16,32($sp)
	move	$2,$4
	sb	$2,40($sp)
	sw	$0,16($sp)
	sw	$0,20($sp)
	lbu	$2,40($sp)
	andi	$2,$2,0xf8
	bne	$2,$0,$L124
	nop

	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$2,$2,0x110
	sw	$2,24($sp)
	lw	$2,24($sp)
	lw	$2,0($2)
	sw	$2,28($sp)
	lw	$2,24($sp)
	lw	$2,0($2)
	sw	$2,28($sp)
	lw	$2,28($sp)
	sw	$2,20($sp)
	lw	$3,20($sp)
	li	$2,-7929856			# 0xffffffffff870000
	ori	$2,$2,0xffff
	and	$2,$3,$2
	lbu	$3,40($sp)
	sll	$3,$3,19
	or	$3,$2,$3
	li	$2,8388608			# 0x800000
	or	$2,$3,$2
	sw	$2,16($sp)
	lw	$4,16($sp)
	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$5,$2,0x110
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	move	$2,$0
	j	$L126
	nop

$L124:
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,136($2)
	andi	$2,$2,0x10
	beq	$2,$0,$L127
	nop

	lui	$2,%hi(jiffies)
	lw	$16,%lo(jiffies)($2)
	lui	$2,%hi($LC2)
	addiu	$4,$2,%lo($LC2)
	li	$5,47			# 0x2f
	lui	$2,%hi(strrchr)
	addiu	$2,$2,%lo(strrchr)
	jalr	$2
	nop

	addiu	$3,$2,1
	lui	$2,%hi($LC20)
	addiu	$4,$2,%lo($LC20)
	move	$5,$16
	move	$6,$3
	li	$7,1074			# 0x432
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L127:
	li	$2,1			# 0x1
$L126:
	lw	$31,36($sp)
	lw	$16,32($sp)
	addiu	$sp,$sp,40
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	phy_bit_delay
	.size	phy_bit_delay, .-phy_bit_delay
	.rdata
	.align	2
$LC21:
	.ascii	"[%lu]0ms[%s:%d]\012  GPON calibration done. \012\000"
	.align	2
$LC22:
	.ascii	"[%lu]0ms[%s:%d]\012  VCO calibration output : tx[15:8]: "
	.ascii	"%.8x\012\000"
	.align	2
$LC23:
	.ascii	"[%lu]0ms[%s:%d]\012  VCO calibration output : rx[7:0]: %"
	.ascii	".8x\012\000"
	.align	2
$LC24:
	.ascii	"[%lu]0ms[%s:%d]\012  PI calibration output : %.8x\012\000"
	.align	2
$LC25:
	.ascii	"[%lu]0ms[%s:%d]\012  imp calibration output : tx[4:0]: %"
	.ascii	".8x\012\000"
	.align	2
$LC26:
	.ascii	"[%lu]0ms[%s:%d]\012  imp calibration output : rx[12:8]: "
	.ascii	"%.8x\012\000"
	.align	2
$LC27:
	.ascii	"[%lu]0ms[%s:%d]\012  SQTH calibration output[19:16]: %.8"
	.ascii	"x\012\000"
	.align	2
$LC28:
	.ascii	"[%lu]0ms[%s:%d]\012  TDC calibration output : gain[10:0]"
	.ascii	": %.8x\012\000"
	.align	2
$LC29:
	.ascii	"[%lu]0ms[%s:%d]\012  TDC calibration output : offset[21:"
	.ascii	"11]: %.8x\012\000"
	.align	2
$LC30:
	.ascii	"[%lu]0ms[%s:%d]\012  EPON calibration done. \012\000"
	.align	2
$LC31:
	.ascii	"[%lu]0ms[%s:%d]\012  snese amp calibration output : DCD["
	.ascii	"6:0]: %.8x\012\000"
	.align	2
$LC32:
	.ascii	"[%lu]0ms[%s:%d]\012  snese amp calibration output : DCE["
	.ascii	"14:8]: %.8x\012\000"
	.align	2
$LC33:
	.ascii	"[%lu]0ms[%s:%d]\012  snese amp calibration output : DCEY"
	.ascii	"E[22:16]: %.8x\012\000"
	.section	.text.phy_calibration_test,"ax",@progbits
	.align	2
	.globl	phy_calibration_test
	.set	nomips16
	.set	nomicromips
	.ent	phy_calibration_test
	.type	phy_calibration_test, @function
phy_calibration_test:
	.frame	$sp,144,$31		# vars= 112, regs= 2/0, args= 24, gp= 0
	.mask	0x80010000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-144
	sw	$31,140($sp)
	sw	$16,136($sp)
	sw	$0,24($sp)
	sw	$0,28($sp)
	sw	$0,32($sp)
	j	$L129
	nop

$L163:
	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$2,$2,0x124
	sw	$2,36($sp)
	lw	$2,36($sp)
	lw	$2,0($2)
	sw	$2,40($sp)
	lw	$2,36($sp)
	lw	$2,0($2)
	sw	$2,40($sp)
	lw	$2,40($sp)
	sw	$2,28($sp)
	lw	$3,28($sp)
	li	$2,-2147483648			# 0xffffffff80000000
	or	$2,$3,$2
	sw	$2,32($sp)
	lw	$4,32($sp)
	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$5,$2,0x124
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$2,$2,0x108
	sw	$2,44($sp)
	lw	$2,44($sp)
	lw	$2,0($2)
	sw	$2,48($sp)
	lw	$2,44($sp)
	lw	$2,0($2)
	sw	$2,48($sp)
	lw	$2,48($sp)
	sw	$2,28($sp)
	lw	$3,28($sp)
	li	$2,-2013265920			# 0xffffffff88000000
	or	$2,$3,$2
	sw	$2,32($sp)
	lw	$4,32($sp)
	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$5,$2,0x108
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	lw	$4,28($sp)
	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$5,$2,0x108
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,136($2)
	andi	$2,$2,0x1
	beq	$2,$0,$L132
	nop

	lui	$2,%hi(jiffies)
	lw	$16,%lo(jiffies)($2)
	lui	$2,%hi($LC2)
	addiu	$4,$2,%lo($LC2)
	li	$5,47			# 0x2f
	lui	$2,%hi(strrchr)
	addiu	$2,$2,%lo(strrchr)
	jalr	$2
	nop

	addiu	$3,$2,1
	lui	$2,%hi($LC21)
	addiu	$4,$2,%lo($LC21)
	move	$5,$16
	move	$6,$3
	li	$7,1104			# 0x450
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L132:
	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$2,$2,0x144
	sw	$2,52($sp)
	lw	$2,52($sp)
	lw	$2,0($2)
	sw	$2,56($sp)
	lw	$2,52($sp)
	lw	$2,0($2)
	sw	$2,56($sp)
	lw	$2,56($sp)
	sw	$2,28($sp)
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,136($2)
	andi	$2,$2,0x1
	beq	$2,$0,$L134
	nop

	lui	$2,%hi(jiffies)
	lw	$16,%lo(jiffies)($2)
	lui	$2,%hi($LC2)
	addiu	$4,$2,%lo($LC2)
	li	$5,47			# 0x2f
	lui	$2,%hi(strrchr)
	addiu	$2,$2,%lo(strrchr)
	jalr	$2
	nop

	addiu	$3,$2,1
	lw	$2,28($sp)
	andi	$2,$2,0xff00
	srl	$2,$2,8
	sw	$2,16($sp)
	lui	$2,%hi($LC22)
	addiu	$4,$2,%lo($LC22)
	move	$5,$16
	move	$6,$3
	li	$7,1106			# 0x452
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L134:
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,136($2)
	andi	$2,$2,0x1
	beq	$2,$0,$L135
	nop

	lui	$2,%hi(jiffies)
	lw	$16,%lo(jiffies)($2)
	lui	$2,%hi($LC2)
	addiu	$4,$2,%lo($LC2)
	li	$5,47			# 0x2f
	lui	$2,%hi(strrchr)
	addiu	$2,$2,%lo(strrchr)
	jalr	$2
	nop

	addiu	$3,$2,1
	lw	$2,28($sp)
	andi	$2,$2,0xff
	sw	$2,16($sp)
	lui	$2,%hi($LC23)
	addiu	$4,$2,%lo($LC23)
	move	$5,$16
	move	$6,$3
	li	$7,1107			# 0x453
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L135:
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,136($2)
	andi	$2,$2,0x1
	beq	$2,$0,$L136
	nop

	lui	$2,%hi(jiffies)
	lw	$16,%lo(jiffies)($2)
	lui	$2,%hi($LC2)
	addiu	$4,$2,%lo($LC2)
	li	$5,47			# 0x2f
	lui	$2,%hi(strrchr)
	addiu	$2,$2,%lo(strrchr)
	jalr	$2
	nop

	addiu	$6,$2,1
	lw	$3,28($sp)
	li	$2,16711680			# 0xff0000
	and	$2,$3,$2
	srl	$2,$2,16
	sw	$2,16($sp)
	lui	$2,%hi($LC24)
	addiu	$4,$2,%lo($LC24)
	move	$5,$16
	li	$7,1108			# 0x454
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L136:
	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$2,$2,0x140
	sw	$2,60($sp)
	lw	$2,60($sp)
	lw	$2,0($2)
	sw	$2,64($sp)
	lw	$2,60($sp)
	lw	$2,0($2)
	sw	$2,64($sp)
	lw	$2,64($sp)
	sw	$2,28($sp)
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,136($2)
	andi	$2,$2,0x1
	beq	$2,$0,$L138
	nop

	lui	$2,%hi(jiffies)
	lw	$16,%lo(jiffies)($2)
	lui	$2,%hi($LC2)
	addiu	$4,$2,%lo($LC2)
	li	$5,47			# 0x2f
	lui	$2,%hi(strrchr)
	addiu	$2,$2,%lo(strrchr)
	jalr	$2
	nop

	addiu	$3,$2,1
	lw	$2,28($sp)
	andi	$2,$2,0x1f
	sw	$2,16($sp)
	lui	$2,%hi($LC25)
	addiu	$4,$2,%lo($LC25)
	move	$5,$16
	move	$6,$3
	li	$7,1110			# 0x456
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L138:
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,136($2)
	andi	$2,$2,0x1
	beq	$2,$0,$L139
	nop

	lui	$2,%hi(jiffies)
	lw	$16,%lo(jiffies)($2)
	lui	$2,%hi($LC2)
	addiu	$4,$2,%lo($LC2)
	li	$5,47			# 0x2f
	lui	$2,%hi(strrchr)
	addiu	$2,$2,%lo(strrchr)
	jalr	$2
	nop

	addiu	$3,$2,1
	lw	$2,28($sp)
	andi	$2,$2,0x1f00
	srl	$2,$2,8
	sw	$2,16($sp)
	lui	$2,%hi($LC26)
	addiu	$4,$2,%lo($LC26)
	move	$5,$16
	move	$6,$3
	li	$7,1111			# 0x457
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L139:
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,136($2)
	andi	$2,$2,0x1
	beq	$2,$0,$L140
	nop

	lui	$2,%hi(jiffies)
	lw	$16,%lo(jiffies)($2)
	lui	$2,%hi($LC2)
	addiu	$4,$2,%lo($LC2)
	li	$5,47			# 0x2f
	lui	$2,%hi(strrchr)
	addiu	$2,$2,%lo(strrchr)
	jalr	$2
	nop

	addiu	$6,$2,1
	lw	$3,28($sp)
	li	$2,983040			# 0xf0000
	and	$2,$3,$2
	srl	$2,$2,16
	sw	$2,16($sp)
	lui	$2,%hi($LC27)
	addiu	$4,$2,%lo($LC27)
	move	$5,$16
	li	$7,1112			# 0x458
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L140:
	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$2,$2,0x12c
	sw	$2,68($sp)
	lw	$2,68($sp)
	lw	$2,0($2)
	sw	$2,72($sp)
	lw	$2,68($sp)
	lw	$2,0($2)
	sw	$2,72($sp)
	lw	$2,72($sp)
	sw	$2,28($sp)
	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$2,$2,0x1f4
	sw	$2,76($sp)
	lw	$2,76($sp)
	lw	$2,0($2)
	sw	$2,80($sp)
	lw	$2,76($sp)
	lw	$2,0($2)
	sw	$2,80($sp)
	lw	$2,80($sp)
	sw	$2,28($sp)
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,136($2)
	andi	$2,$2,0x1
	beq	$2,$0,$L143
	nop

	lui	$2,%hi(jiffies)
	lw	$16,%lo(jiffies)($2)
	lui	$2,%hi($LC2)
	addiu	$4,$2,%lo($LC2)
	li	$5,47			# 0x2f
	lui	$2,%hi(strrchr)
	addiu	$2,$2,%lo(strrchr)
	jalr	$2
	nop

	addiu	$3,$2,1
	lw	$2,28($sp)
	andi	$2,$2,0x7ff
	sw	$2,16($sp)
	lui	$2,%hi($LC28)
	addiu	$4,$2,%lo($LC28)
	move	$5,$16
	move	$6,$3
	li	$7,1118			# 0x45e
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L143:
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,136($2)
	andi	$2,$2,0x1
	beq	$2,$0,$L144
	nop

	lui	$2,%hi(jiffies)
	lw	$16,%lo(jiffies)($2)
	lui	$2,%hi($LC2)
	addiu	$4,$2,%lo($LC2)
	li	$5,47			# 0x2f
	lui	$2,%hi(strrchr)
	addiu	$2,$2,%lo(strrchr)
	jalr	$2
	nop

	addiu	$6,$2,1
	lw	$3,28($sp)
	li	$2,4128768			# 0x3f0000
	ori	$2,$2,0xf800
	and	$2,$3,$2
	srl	$2,$2,11
	sw	$2,16($sp)
	lui	$2,%hi($LC29)
	addiu	$4,$2,%lo($LC29)
	move	$5,$16
	li	$7,1119			# 0x45f
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L144:
	li	$4,1			# 0x1
	lui	$2,%hi(msleep)
	addiu	$2,$2,%lo(msleep)
	jalr	$2
	nop

	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$2,$2,0x124
	sw	$2,84($sp)
	lw	$2,84($sp)
	lw	$2,0($2)
	sw	$2,88($sp)
	lw	$2,84($sp)
	lw	$2,0($2)
	sw	$2,88($sp)
	lw	$2,88($sp)
	sw	$2,28($sp)
	lw	$2,28($sp)
	ext	$2,$2,0,31
	sw	$2,32($sp)
	lw	$4,32($sp)
	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$5,$2,0x124
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$2,$2,0x108
	sw	$2,92($sp)
	lw	$2,92($sp)
	lw	$2,0($2)
	sw	$2,96($sp)
	lw	$2,92($sp)
	lw	$2,0($2)
	sw	$2,96($sp)
	lw	$2,96($sp)
	sw	$2,28($sp)
	lw	$3,28($sp)
	li	$2,-2013265920			# 0xffffffff88000000
	or	$2,$3,$2
	sw	$2,32($sp)
	lw	$4,32($sp)
	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$5,$2,0x108
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	lw	$4,28($sp)
	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$5,$2,0x108
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,136($2)
	andi	$2,$2,0x1
	beq	$2,$0,$L147
	nop

	lui	$2,%hi(jiffies)
	lw	$16,%lo(jiffies)($2)
	lui	$2,%hi($LC2)
	addiu	$4,$2,%lo($LC2)
	li	$5,47			# 0x2f
	lui	$2,%hi(strrchr)
	addiu	$2,$2,%lo(strrchr)
	jalr	$2
	nop

	addiu	$3,$2,1
	lui	$2,%hi($LC30)
	addiu	$4,$2,%lo($LC30)
	move	$5,$16
	move	$6,$3
	li	$7,1128			# 0x468
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L147:
	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$2,$2,0x144
	sw	$2,100($sp)
	lw	$2,100($sp)
	lw	$2,0($2)
	sw	$2,104($sp)
	lw	$2,100($sp)
	lw	$2,0($2)
	sw	$2,104($sp)
	lw	$2,104($sp)
	sw	$2,28($sp)
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,136($2)
	andi	$2,$2,0x1
	beq	$2,$0,$L149
	nop

	lui	$2,%hi(jiffies)
	lw	$16,%lo(jiffies)($2)
	lui	$2,%hi($LC2)
	addiu	$4,$2,%lo($LC2)
	li	$5,47			# 0x2f
	lui	$2,%hi(strrchr)
	addiu	$2,$2,%lo(strrchr)
	jalr	$2
	nop

	addiu	$3,$2,1
	lw	$2,28($sp)
	andi	$2,$2,0xff00
	srl	$2,$2,8
	sw	$2,16($sp)
	lui	$2,%hi($LC22)
	addiu	$4,$2,%lo($LC22)
	move	$5,$16
	move	$6,$3
	li	$7,1130			# 0x46a
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L149:
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,136($2)
	andi	$2,$2,0x1
	beq	$2,$0,$L150
	nop

	lui	$2,%hi(jiffies)
	lw	$16,%lo(jiffies)($2)
	lui	$2,%hi($LC2)
	addiu	$4,$2,%lo($LC2)
	li	$5,47			# 0x2f
	lui	$2,%hi(strrchr)
	addiu	$2,$2,%lo(strrchr)
	jalr	$2
	nop

	addiu	$3,$2,1
	lw	$2,28($sp)
	andi	$2,$2,0xff
	sw	$2,16($sp)
	lui	$2,%hi($LC23)
	addiu	$4,$2,%lo($LC23)
	move	$5,$16
	move	$6,$3
	li	$7,1131			# 0x46b
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L150:
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,136($2)
	andi	$2,$2,0x1
	beq	$2,$0,$L151
	nop

	lui	$2,%hi(jiffies)
	lw	$16,%lo(jiffies)($2)
	lui	$2,%hi($LC2)
	addiu	$4,$2,%lo($LC2)
	li	$5,47			# 0x2f
	lui	$2,%hi(strrchr)
	addiu	$2,$2,%lo(strrchr)
	jalr	$2
	nop

	addiu	$6,$2,1
	lw	$3,28($sp)
	li	$2,16711680			# 0xff0000
	and	$2,$3,$2
	srl	$2,$2,16
	sw	$2,16($sp)
	lui	$2,%hi($LC24)
	addiu	$4,$2,%lo($LC24)
	move	$5,$16
	li	$7,1132			# 0x46c
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L151:
	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$2,$2,0x140
	sw	$2,108($sp)
	lw	$2,108($sp)
	lw	$2,0($2)
	sw	$2,112($sp)
	lw	$2,108($sp)
	lw	$2,0($2)
	sw	$2,112($sp)
	lw	$2,112($sp)
	sw	$2,28($sp)
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,136($2)
	andi	$2,$2,0x1
	beq	$2,$0,$L153
	nop

	lui	$2,%hi(jiffies)
	lw	$16,%lo(jiffies)($2)
	lui	$2,%hi($LC2)
	addiu	$4,$2,%lo($LC2)
	li	$5,47			# 0x2f
	lui	$2,%hi(strrchr)
	addiu	$2,$2,%lo(strrchr)
	jalr	$2
	nop

	addiu	$3,$2,1
	lw	$2,28($sp)
	andi	$2,$2,0x1f
	sw	$2,16($sp)
	lui	$2,%hi($LC25)
	addiu	$4,$2,%lo($LC25)
	move	$5,$16
	move	$6,$3
	li	$7,1134			# 0x46e
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L153:
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,136($2)
	andi	$2,$2,0x1
	beq	$2,$0,$L154
	nop

	lui	$2,%hi(jiffies)
	lw	$16,%lo(jiffies)($2)
	lui	$2,%hi($LC2)
	addiu	$4,$2,%lo($LC2)
	li	$5,47			# 0x2f
	lui	$2,%hi(strrchr)
	addiu	$2,$2,%lo(strrchr)
	jalr	$2
	nop

	addiu	$3,$2,1
	lw	$2,28($sp)
	andi	$2,$2,0x1f00
	srl	$2,$2,8
	sw	$2,16($sp)
	lui	$2,%hi($LC26)
	addiu	$4,$2,%lo($LC26)
	move	$5,$16
	move	$6,$3
	li	$7,1135			# 0x46f
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L154:
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,136($2)
	andi	$2,$2,0x1
	beq	$2,$0,$L155
	nop

	lui	$2,%hi(jiffies)
	lw	$16,%lo(jiffies)($2)
	lui	$2,%hi($LC2)
	addiu	$4,$2,%lo($LC2)
	li	$5,47			# 0x2f
	lui	$2,%hi(strrchr)
	addiu	$2,$2,%lo(strrchr)
	jalr	$2
	nop

	addiu	$6,$2,1
	lw	$3,28($sp)
	li	$2,983040			# 0xf0000
	and	$2,$3,$2
	srl	$2,$2,16
	sw	$2,16($sp)
	lui	$2,%hi($LC27)
	addiu	$4,$2,%lo($LC27)
	move	$5,$16
	li	$7,1136			# 0x470
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L155:
	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$2,$2,0x12c
	sw	$2,116($sp)
	lw	$2,116($sp)
	lw	$2,0($2)
	sw	$2,120($sp)
	lw	$2,116($sp)
	lw	$2,0($2)
	sw	$2,120($sp)
	lw	$2,120($sp)
	sw	$2,28($sp)
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,136($2)
	andi	$2,$2,0x1
	beq	$2,$0,$L157
	nop

	lui	$2,%hi(jiffies)
	lw	$16,%lo(jiffies)($2)
	lui	$2,%hi($LC2)
	addiu	$4,$2,%lo($LC2)
	li	$5,47			# 0x2f
	lui	$2,%hi(strrchr)
	addiu	$2,$2,%lo(strrchr)
	jalr	$2
	nop

	addiu	$3,$2,1
	lw	$2,28($sp)
	andi	$2,$2,0x7f
	sw	$2,16($sp)
	lui	$2,%hi($LC31)
	addiu	$4,$2,%lo($LC31)
	move	$5,$16
	move	$6,$3
	li	$7,1139			# 0x473
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L157:
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,136($2)
	andi	$2,$2,0x1
	beq	$2,$0,$L158
	nop

	lui	$2,%hi(jiffies)
	lw	$16,%lo(jiffies)($2)
	lui	$2,%hi($LC2)
	addiu	$4,$2,%lo($LC2)
	li	$5,47			# 0x2f
	lui	$2,%hi(strrchr)
	addiu	$2,$2,%lo(strrchr)
	jalr	$2
	nop

	addiu	$3,$2,1
	lw	$2,28($sp)
	andi	$2,$2,0x7f00
	srl	$2,$2,8
	sw	$2,16($sp)
	lui	$2,%hi($LC32)
	addiu	$4,$2,%lo($LC32)
	move	$5,$16
	move	$6,$3
	li	$7,1140			# 0x474
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L158:
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,136($2)
	andi	$2,$2,0x1
	beq	$2,$0,$L159
	nop

	lui	$2,%hi(jiffies)
	lw	$16,%lo(jiffies)($2)
	lui	$2,%hi($LC2)
	addiu	$4,$2,%lo($LC2)
	li	$5,47			# 0x2f
	lui	$2,%hi(strrchr)
	addiu	$2,$2,%lo(strrchr)
	jalr	$2
	nop

	addiu	$6,$2,1
	lw	$3,28($sp)
	li	$2,8323072			# 0x7f0000
	and	$2,$3,$2
	srl	$2,$2,16
	sw	$2,16($sp)
	lui	$2,%hi($LC33)
	addiu	$4,$2,%lo($LC33)
	move	$5,$16
	li	$7,1141			# 0x475
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L159:
	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$2,$2,0x1f4
	sw	$2,124($sp)
	lw	$2,124($sp)
	lw	$2,0($2)
	sw	$2,128($sp)
	lw	$2,124($sp)
	lw	$2,0($2)
	sw	$2,128($sp)
	lw	$2,128($sp)
	sw	$2,28($sp)
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,136($2)
	andi	$2,$2,0x1
	beq	$2,$0,$L161
	nop

	lui	$2,%hi(jiffies)
	lw	$16,%lo(jiffies)($2)
	lui	$2,%hi($LC2)
	addiu	$4,$2,%lo($LC2)
	li	$5,47			# 0x2f
	lui	$2,%hi(strrchr)
	addiu	$2,$2,%lo(strrchr)
	jalr	$2
	nop

	addiu	$3,$2,1
	lw	$2,28($sp)
	andi	$2,$2,0x7ff
	sw	$2,16($sp)
	lui	$2,%hi($LC28)
	addiu	$4,$2,%lo($LC28)
	move	$5,$16
	move	$6,$3
	li	$7,1144			# 0x478
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L161:
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,136($2)
	andi	$2,$2,0x1
	beq	$2,$0,$L162
	nop

	lui	$2,%hi(jiffies)
	lw	$16,%lo(jiffies)($2)
	lui	$2,%hi($LC2)
	addiu	$4,$2,%lo($LC2)
	li	$5,47			# 0x2f
	lui	$2,%hi(strrchr)
	addiu	$2,$2,%lo(strrchr)
	jalr	$2
	nop

	addiu	$6,$2,1
	lw	$3,28($sp)
	li	$2,4128768			# 0x3f0000
	ori	$2,$2,0xf800
	and	$2,$3,$2
	srl	$2,$2,11
	sw	$2,16($sp)
	lui	$2,%hi($LC29)
	addiu	$4,$2,%lo($LC29)
	move	$5,$16
	li	$7,1145			# 0x479
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L162:
	li	$4,1			# 0x1
	lui	$2,%hi(msleep)
	addiu	$2,$2,%lo(msleep)
	jalr	$2
	nop

	lw	$2,24($sp)
	addiu	$2,$2,1
	sw	$2,24($sp)
$L129:
	lw	$2,24($sp)
	slt	$2,$2,100
	bne	$2,$0,$L163
	nop

	lw	$31,140($sp)
	lw	$16,136($sp)
	addiu	$sp,$sp,144
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	phy_calibration_test
	.size	phy_calibration_test, .-phy_calibration_test
	.rdata
	.align	2
$LC34:
	.ascii	"test 1 = %d \012\000"
	.align	2
$LC35:
	.ascii	"test 2 = %d \012\000"
	.align	2
$LC36:
	.ascii	"test 3 = %d \012\000"
	.align	2
$LC37:
	.ascii	"test 4 = %d \012\000"
	.align	2
$LC38:
	.ascii	"bank 1 pass \012\000"
	.align	2
$LC39:
	.ascii	"bank 2 pass \012\000"
	.align	2
$LC40:
	.ascii	"bank 3 pass \012\000"
	.align	2
$LC41:
	.ascii	"bank 4 pass \012\000"
	.align	2
$LC42:
	.ascii	"bank 5 pass \012\000"
	.section	.text.phy_pbus_stress_test,"ax",@progbits
	.align	2
	.globl	phy_pbus_stress_test
	.set	nomips16
	.set	nomicromips
	.ent	phy_pbus_stress_test
	.type	phy_pbus_stress_test, @function
phy_pbus_stress_test:
	.frame	$sp,184,$31		# vars= 160, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-184
	sw	$31,180($sp)
	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$2,$2,0x164
	sw	$2,36($sp)
	lw	$2,36($sp)
	lw	$2,0($2)
	sw	$2,40($sp)
	lw	$2,36($sp)
	lw	$2,0($2)
	sw	$2,40($sp)
	lw	$2,40($sp)
	sw	$2,112($sp)
	lw	$2,112($sp)
	sw	$2,92($sp)
	lw	$2,92($sp)
	sw	$2,132($sp)
	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$2,$2,0x200
	sw	$2,44($sp)
	lw	$2,44($sp)
	lw	$2,0($2)
	sw	$2,48($sp)
	lw	$2,44($sp)
	lw	$2,0($2)
	sw	$2,48($sp)
	lw	$2,48($sp)
	sw	$2,116($sp)
	lw	$2,116($sp)
	sw	$2,96($sp)
	lw	$2,96($sp)
	sw	$2,136($sp)
	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$2,$2,0x3a0
	sw	$2,52($sp)
	lw	$2,52($sp)
	lw	$2,0($2)
	sw	$2,56($sp)
	lw	$2,52($sp)
	lw	$2,0($2)
	sw	$2,56($sp)
	lw	$2,56($sp)
	sw	$2,120($sp)
	lw	$2,120($sp)
	sw	$2,100($sp)
	lw	$2,100($sp)
	sw	$2,140($sp)
	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$2,$2,0x400
	sw	$2,60($sp)
	lw	$2,60($sp)
	lw	$2,0($2)
	sw	$2,64($sp)
	lw	$2,60($sp)
	lw	$2,0($2)
	sw	$2,64($sp)
	lw	$2,64($sp)
	sw	$2,124($sp)
	lw	$2,124($sp)
	sw	$2,104($sp)
	lw	$2,104($sp)
	sw	$2,144($sp)
	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$2,$2,0x534
	sw	$2,68($sp)
	lw	$2,68($sp)
	lw	$2,0($2)
	sw	$2,72($sp)
	lw	$2,68($sp)
	lw	$2,0($2)
	sw	$2,72($sp)
	lw	$2,72($sp)
	sw	$2,128($sp)
	lw	$2,128($sp)
	sw	$2,108($sp)
	lw	$2,108($sp)
	sw	$2,148($sp)
	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$2,$2,0x164
	sw	$2,152($sp)
	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$2,$2,0x200
	sw	$2,156($sp)
	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$2,$2,0x3a0
	sw	$2,160($sp)
	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$2,$2,0x400
	sw	$2,164($sp)
	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$2,$2,0x534
	sw	$2,168($sp)
	sw	$0,16($sp)
	sw	$0,20($sp)
	sw	$0,24($sp)
	sw	$0,28($sp)
	sw	$0,32($sp)
	j	$L170
	nop

$L179:
	lui	$2,%hi(rand)
	addiu	$2,$2,%lo(rand)
	jalr	$2
	nop

	move	$3,$2
	li	$2,-2147483648			# 0xffffffff80000000
	ori	$2,$2,0x1
	and	$2,$3,$2
	bgez	$2,$L171
	nop

	addiu	$2,$2,-1
	li	$3,-2			# 0xfffffffffffffffe
	or	$2,$2,$3
	addiu	$2,$2,1
$L171:
	sw	$2,20($sp)
	lui	$2,%hi($LC34)
	addiu	$4,$2,%lo($LC34)
	lw	$5,20($sp)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	li	$4,10			# 0xa
	lui	$2,%hi(msleep)
	addiu	$2,$2,%lo(msleep)
	jalr	$2
	nop

	lui	$2,%hi(rand)
	addiu	$2,$2,%lo(rand)
	jalr	$2
	nop

	move	$3,$2
	li	$2,-2147483648			# 0xffffffff80000000
	ori	$2,$2,0x1
	and	$2,$3,$2
	bgez	$2,$L172
	nop

	addiu	$2,$2,-1
	li	$3,-2			# 0xfffffffffffffffe
	or	$2,$2,$3
	addiu	$2,$2,1
$L172:
	sw	$2,24($sp)
	lui	$2,%hi($LC35)
	addiu	$4,$2,%lo($LC35)
	lw	$5,24($sp)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	li	$4,10			# 0xa
	lui	$2,%hi(msleep)
	addiu	$2,$2,%lo(msleep)
	jalr	$2
	nop

	lui	$2,%hi(rand)
	addiu	$2,$2,%lo(rand)
	jalr	$2
	nop

	move	$3,$2
	li	$2,1717960704			# 0x66660000
	ori	$2,$2,0x6667
	mult	$3,$2
	mfhi	$2
	sra	$4,$2,1
	sra	$2,$3,31
	subu	$4,$4,$2
	move	$2,$4
	sll	$2,$2,2
	addu	$2,$2,$4
	subu	$2,$3,$2
	sw	$2,28($sp)
	lui	$2,%hi($LC36)
	addiu	$4,$2,%lo($LC36)
	lw	$5,28($sp)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	li	$4,10			# 0xa
	lui	$2,%hi(msleep)
	addiu	$2,$2,%lo(msleep)
	jalr	$2
	nop

	lui	$2,%hi(rand)
	addiu	$2,$2,%lo(rand)
	jalr	$2
	nop

	move	$3,$2
	li	$2,1717960704			# 0x66660000
	ori	$2,$2,0x6667
	mult	$3,$2
	mfhi	$2
	sra	$4,$2,1
	sra	$2,$3,31
	subu	$4,$4,$2
	move	$2,$4
	sll	$2,$2,2
	addu	$2,$2,$4
	subu	$2,$3,$2
	sw	$2,32($sp)
	lui	$2,%hi($LC37)
	addiu	$4,$2,%lo($LC37)
	lw	$5,32($sp)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lw	$2,20($sp)
	bne	$2,$0,$L173
	nop

	lw	$2,28($sp)
	sll	$2,$2,2
	addiu	$3,$sp,16
	addu	$2,$3,$2
	lw	$2,136($2)
	sw	$2,76($sp)
	lw	$2,76($sp)
	lw	$2,0($2)
	sw	$2,80($sp)
	lw	$2,76($sp)
	lw	$2,0($2)
	sw	$2,80($sp)
	lw	$3,80($sp)
	lw	$2,28($sp)
	sll	$2,$2,2
	addiu	$4,$sp,16
	addu	$2,$4,$2
	sw	$3,96($2)
	j	$L175
	nop

$L173:
	lw	$2,28($sp)
	sll	$2,$2,2
	addiu	$3,$sp,16
	addu	$2,$3,$2
	lw	$4,76($2)
	lw	$2,28($sp)
	sll	$2,$2,2
	addiu	$3,$sp,16
	addu	$2,$3,$2
	lw	$2,136($2)
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

$L175:
	lw	$2,24($sp)
	bne	$2,$0,$L176
	nop

	lw	$2,32($sp)
	sll	$2,$2,2
	addiu	$3,$sp,16
	addu	$2,$3,$2
	lw	$2,136($2)
	sw	$2,84($sp)
	lw	$2,84($sp)
	lw	$2,0($2)
	sw	$2,88($sp)
	lw	$2,84($sp)
	lw	$2,0($2)
	sw	$2,88($sp)
	lw	$3,88($sp)
	lw	$2,32($sp)
	sll	$2,$2,2
	addiu	$4,$sp,16
	addu	$2,$4,$2
	sw	$3,96($2)
	j	$L178
	nop

$L176:
	lw	$2,32($sp)
	sll	$2,$2,2
	addiu	$3,$sp,16
	addu	$2,$3,$2
	lw	$4,76($2)
	lw	$2,32($sp)
	sll	$2,$2,2
	addiu	$3,$sp,16
	addu	$2,$3,$2
	lw	$2,136($2)
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

$L178:
	lw	$2,16($sp)
	addiu	$2,$2,1
	sw	$2,16($sp)
$L170:
	lw	$3,16($sp)
	li	$2,65536			# 0x10000
	ori	$2,$2,0x86a0
	slt	$2,$3,$2
	bne	$2,$0,$L179
	nop

	lw	$3,132($sp)
	lw	$2,112($sp)
	bne	$3,$2,$L180
	nop

	lw	$3,132($sp)
	lw	$2,92($sp)
	bne	$3,$2,$L180
	nop

	lui	$2,%hi($LC38)
	addiu	$4,$2,%lo($LC38)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L180:
	lw	$3,136($sp)
	lw	$2,116($sp)
	bne	$3,$2,$L181
	nop

	lw	$3,136($sp)
	lw	$2,96($sp)
	bne	$3,$2,$L181
	nop

	lui	$2,%hi($LC39)
	addiu	$4,$2,%lo($LC39)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L181:
	lw	$3,140($sp)
	lw	$2,120($sp)
	bne	$3,$2,$L182
	nop

	lw	$3,140($sp)
	lw	$2,100($sp)
	bne	$3,$2,$L182
	nop

	lui	$2,%hi($LC40)
	addiu	$4,$2,%lo($LC40)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L182:
	lw	$3,144($sp)
	lw	$2,124($sp)
	bne	$3,$2,$L183
	nop

	lw	$3,144($sp)
	lw	$2,104($sp)
	bne	$3,$2,$L183
	nop

	lui	$2,%hi($LC41)
	addiu	$4,$2,%lo($LC41)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L183:
	lw	$3,148($sp)
	lw	$2,128($sp)
	bne	$3,$2,$L164
	nop

	lw	$3,148($sp)
	lw	$2,108($sp)
	bne	$3,$2,$L164
	nop

	lui	$2,%hi($LC42)
	addiu	$4,$2,%lo($LC42)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L164:
	lw	$31,180($sp)
	addiu	$sp,$sp,184
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	phy_pbus_stress_test
	.size	phy_pbus_stress_test, .-phy_pbus_stress_test
	.rdata
	.align	2
$LC43:
	.ascii	"[%lu]0ms[%s:%d]\012  Phy_Fw_Ready function success. \012"
	.ascii	"\000"
	.section	.text.phy_fw_ready,"ax",@progbits
	.align	2
	.globl	phy_fw_ready
	.set	nomips16
	.set	nomicromips
	.ent	phy_fw_ready
	.type	phy_fw_ready, @function
phy_fw_ready:
	.frame	$sp,40,$31		# vars= 16, regs= 2/0, args= 16, gp= 0
	.mask	0x80010000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-40
	sw	$31,36($sp)
	sw	$16,32($sp)
	move	$2,$4
	sb	$2,40($sp)
	sw	$0,20($sp)
	sw	$0,16($sp)
	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$2,$2,0x104
	sw	$2,24($sp)
	lw	$2,24($sp)
	lw	$2,0($2)
	sw	$2,28($sp)
	lw	$2,24($sp)
	lw	$2,0($2)
	sw	$2,28($sp)
	lw	$2,28($sp)
	sw	$2,20($sp)
	lbu	$3,40($sp)
	li	$2,1			# 0x1
	bne	$3,$2,$L187
	nop

	lw	$2,20($sp)
	ori	$2,$2,0x1
	sw	$2,16($sp)
	j	$L188
	nop

$L187:
	lbu	$2,40($sp)
	bne	$2,$0,$L189
	nop

	lw	$3,20($sp)
	li	$2,-2			# 0xfffffffffffffffe
	and	$2,$3,$2
	sw	$2,16($sp)
	j	$L188
	nop

$L189:
	li	$2,1			# 0x1
	j	$L190
	nop

$L188:
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,136($2)
	andi	$2,$2,0x10
	beq	$2,$0,$L191
	nop

	lui	$2,%hi(jiffies)
	lw	$16,%lo(jiffies)($2)
	lui	$2,%hi($LC2)
	addiu	$4,$2,%lo($LC2)
	li	$5,47			# 0x2f
	lui	$2,%hi(strrchr)
	addiu	$2,$2,%lo(strrchr)
	jalr	$2
	nop

	addiu	$3,$2,1
	lui	$2,%hi($LC43)
	addiu	$4,$2,%lo($LC43)
	move	$5,$16
	move	$6,$3
	li	$7,1251			# 0x4e3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L191:
	lw	$4,16($sp)
	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$5,$2,0x104
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	move	$2,$0
$L190:
	lw	$31,36($sp)
	lw	$16,32($sp)
	addiu	$sp,$sp,40
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	phy_fw_ready
	.size	phy_fw_ready, .-phy_fw_ready
	.rdata
	.align	2
$LC44:
	.ascii	"[%lu]0ms[%s:%d]\012  error input : freq_sele. \012\000"
	.align	2
$LC45:
	.ascii	"[%lu]0ms[%s:%d]\012  Phy Freq Meter enable success. \012"
	.ascii	"\000"
	.align	2
$LC46:
	.ascii	"[%lu]0ms[%s:%d]\012  freq meter result : 0x%.8x\012\000"
	.align	2
$LC47:
	.ascii	"[%lu]0ms[%s:%d]\012  Phy Freq Meter disable success. \012"
	.ascii	"\000"
	.section	.text.phy_freq_meter,"ax",@progbits
	.align	2
	.globl	phy_freq_meter
	.set	nomips16
	.set	nomicromips
	.ent	phy_freq_meter
	.type	phy_freq_meter, @function
phy_freq_meter:
	.frame	$sp,88,$31		# vars= 56, regs= 2/0, args= 24, gp= 0
	.mask	0x80010000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-88
	sw	$31,84($sp)
	sw	$16,80($sp)
	move	$3,$4
	move	$2,$5
	sb	$3,88($sp)
	sb	$2,92($sp)
	sw	$0,24($sp)
	sw	$0,28($sp)
	lbu	$3,88($sp)
	li	$2,1			# 0x1
	bne	$3,$2,$L193
	nop

	lbu	$2,92($sp)
	andi	$2,$2,0xf0
	beq	$2,$0,$L194
	nop

	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,136($2)
	andi	$2,$2,0x1
	beq	$2,$0,$L195
	nop

	lui	$2,%hi(jiffies)
	lw	$16,%lo(jiffies)($2)
	lui	$2,%hi($LC2)
	addiu	$4,$2,%lo($LC2)
	li	$5,47			# 0x2f
	lui	$2,%hi(strrchr)
	addiu	$2,$2,%lo(strrchr)
	jalr	$2
	nop

	addiu	$3,$2,1
	lui	$2,%hi($LC44)
	addiu	$4,$2,%lo($LC44)
	move	$5,$16
	move	$6,$3
	li	$7,1279			# 0x4ff
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L195:
	li	$2,1			# 0x1
	j	$L196
	nop

$L194:
	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$2,$2,0x108
	sw	$2,32($sp)
	lw	$2,32($sp)
	lw	$2,0($2)
	sw	$2,36($sp)
	lw	$2,32($sp)
	lw	$2,0($2)
	sw	$2,36($sp)
	lw	$2,36($sp)
	sw	$2,24($sp)
	lw	$3,24($sp)
	li	$2,-16842752			# 0xfffffffffeff0000
	ori	$2,$2,0xffff
	and	$2,$3,$2
	sw	$2,28($sp)
	lw	$4,28($sp)
	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$5,$2,0x108
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$2,$2,0x128
	sw	$2,40($sp)
	lw	$2,40($sp)
	lw	$2,0($2)
	sw	$2,44($sp)
	lw	$2,40($sp)
	lw	$2,0($2)
	sw	$2,44($sp)
	lw	$2,44($sp)
	sw	$2,24($sp)
	lw	$3,24($sp)
	li	$2,-251723776			# 0xfffffffff0ff0000
	ori	$2,$2,0xffff
	and	$2,$3,$2
	sw	$2,24($sp)
	lbu	$2,92($sp)
	sll	$2,$2,24
	move	$3,$2
	lw	$2,24($sp)
	or	$2,$3,$2
	sw	$2,28($sp)
	lw	$4,28($sp)
	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$5,$2,0x128
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	lw	$3,24($sp)
	li	$2,65536			# 0x10000
	or	$2,$3,$2
	sw	$2,28($sp)
	lw	$4,28($sp)
	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$5,$2,0x128
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

$L200:
	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$2,$2,0x14c
	sw	$2,48($sp)
	lw	$2,48($sp)
	lw	$2,0($2)
	sw	$2,52($sp)
	lw	$2,48($sp)
	lw	$2,0($2)
	sw	$2,52($sp)
	lw	$2,52($sp)
	sw	$2,24($sp)
	lw	$3,24($sp)
	li	$2,65536			# 0x10000
	and	$2,$3,$2
	bne	$2,$0,$L200
	nop

	lw	$3,24($sp)
	li	$2,65536			# 0x10000
	and	$2,$3,$2
	beq	$2,$0,$L201
	nop

	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,136($2)
	andi	$2,$2,0x10
	beq	$2,$0,$L202
	nop

	lui	$2,%hi(jiffies)
	lw	$16,%lo(jiffies)($2)
	lui	$2,%hi($LC2)
	addiu	$4,$2,%lo($LC2)
	li	$5,47			# 0x2f
	lui	$2,%hi(strrchr)
	addiu	$2,$2,%lo(strrchr)
	jalr	$2
	nop

	addiu	$3,$2,1
	lui	$2,%hi($LC45)
	addiu	$4,$2,%lo($LC45)
	move	$5,$16
	move	$6,$3
	li	$7,1297			# 0x511
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L202:
	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$2,$2,0x128
	sw	$2,56($sp)
	lw	$2,56($sp)
	lw	$2,0($2)
	sw	$2,60($sp)
	lw	$2,56($sp)
	lw	$2,0($2)
	sw	$2,60($sp)
	lw	$2,60($sp)
	sw	$2,24($sp)
	lw	$2,24($sp)
	andi	$2,$2,0x7fff
	sw	$2,24($sp)
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,136($2)
	andi	$2,$2,0x10
	beq	$2,$0,$L204
	nop

	lui	$2,%hi(jiffies)
	lw	$16,%lo(jiffies)($2)
	lui	$2,%hi($LC2)
	addiu	$4,$2,%lo($LC2)
	li	$5,47			# 0x2f
	lui	$2,%hi(strrchr)
	addiu	$2,$2,%lo(strrchr)
	jalr	$2
	nop

	addiu	$3,$2,1
	lw	$2,24($sp)
	sw	$2,16($sp)
	lui	$2,%hi($LC46)
	addiu	$4,$2,%lo($LC46)
	move	$5,$16
	move	$6,$3
	li	$7,1300			# 0x514
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L204:
	move	$2,$0
	j	$L196
	nop

$L201:
	li	$2,1			# 0x1
	j	$L196
	nop

$L193:
	lbu	$2,88($sp)
	bne	$2,$0,$L205
	nop

	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$2,$2,0x108
	sw	$2,64($sp)
	lw	$2,64($sp)
	lw	$2,0($2)
	sw	$2,68($sp)
	lw	$2,64($sp)
	lw	$2,0($2)
	sw	$2,68($sp)
	lw	$2,68($sp)
	sw	$2,24($sp)
	lw	$3,24($sp)
	li	$2,16777216			# 0x1000000
	or	$2,$3,$2
	sw	$2,28($sp)
	lw	$4,28($sp)
	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$5,$2,0x108
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$2,$2,0x128
	sw	$2,72($sp)
	lw	$2,72($sp)
	lw	$2,0($2)
	sw	$2,76($sp)
	lw	$2,72($sp)
	lw	$2,0($2)
	sw	$2,76($sp)
	lw	$2,76($sp)
	sw	$2,24($sp)
	lw	$3,24($sp)
	li	$2,-131072			# 0xfffffffffffe0000
	ori	$2,$2,0xffff
	and	$2,$3,$2
	sw	$2,28($sp)
	lw	$4,28($sp)
	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$5,$2,0x128
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,136($2)
	andi	$2,$2,0x10
	beq	$2,$0,$L208
	nop

	lui	$2,%hi(jiffies)
	lw	$16,%lo(jiffies)($2)
	lui	$2,%hi($LC2)
	addiu	$4,$2,%lo($LC2)
	li	$5,47			# 0x2f
	lui	$2,%hi(strrchr)
	addiu	$2,$2,%lo(strrchr)
	jalr	$2
	nop

	addiu	$3,$2,1
	lui	$2,%hi($LC47)
	addiu	$4,$2,%lo($LC47)
	move	$5,$16
	move	$6,$3
	li	$7,1319			# 0x527
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L208:
	move	$2,$0
	j	$L196
	nop

$L205:
	li	$2,1			# 0x1
$L196:
	lw	$31,84($sp)
	lw	$16,80($sp)
	addiu	$sp,$sp,88
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	phy_freq_meter
	.size	phy_freq_meter, .-phy_freq_meter
	.rdata
	.align	2
$LC48:
	.ascii	"GPON Mode \012\000"
	.align	2
$LC49:
	.ascii	"EPON Mode \012\000"
	.align	2
$LC50:
	.ascii	"FW Ready \012\000"
	.align	2
$LC51:
	.ascii	"FW Not Ready \012\000"
	.align	2
$LC52:
	.ascii	"PHY Ready \012\000"
	.align	2
$LC53:
	.ascii	"PHY Not Ready \012\000"
	.align	2
$LC54:
	.ascii	"RX Sync OK \012\000"
	.align	2
$LC55:
	.ascii	"RX Not Sync\012\000"
	.align	2
$LC56:
	.ascii	"PON PHY driver version is %d.%d.%d\012\000"
	.align	2
$LC57:
	.ascii	"LOS status = %d\012\000"
	.align	2
$LC58:
	.ascii	"MT7570 Exist\012\000"
	.align	2
$LC59:
	.ascii	"Internal DDMI Enabled\012\000"
	.align	2
$LC60:
	.ascii	"Internal DDMI Disabled\012\000"
	.align	2
$LC61:
	.ascii	"P0 under open loop mode \012\000"
	.align	2
$LC62:
	.ascii	"P0 under close loop mode \012\000"
	.align	2
$LC63:
	.ascii	"P1 under open loop mode \012\000"
	.align	2
$LC64:
	.ascii	"P1 under close loop mode \012\000"
	.align	2
$LC65:
	.ascii	"Vendor ID : %s\012\000"
	.align	2
$LC66:
	.ascii	"Vendor PN : %s\012\000"
	.section	.text.pon_phy_status,"ax",@progbits
	.align	2
	.globl	pon_phy_status
	.set	nomips16
	.set	nomicromips
	.ent	pon_phy_status
	.type	pon_phy_status, @function
pon_phy_status:
	.frame	$sp,120,$31		# vars= 80, regs= 1/0, args= 32, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-120
	sw	$31,116($sp)
	sw	$0,32($sp)
	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$2,$2,0x124
	sw	$2,36($sp)
	lw	$2,36($sp)
	lw	$2,0($2)
	sw	$2,40($sp)
	lw	$2,36($sp)
	lw	$2,0($2)
	sw	$2,40($sp)
	lw	$2,40($sp)
	sw	$2,32($sp)
	lw	$2,32($sp)
	srl	$3,$2,31
	li	$2,1			# 0x1
	bne	$3,$2,$L211
	nop

	lui	$2,%hi($LC48)
	addiu	$4,$2,%lo($LC48)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	j	$L212
	nop

$L211:
	lui	$2,%hi($LC49)
	addiu	$4,$2,%lo($LC49)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L212:
	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$2,$2,0x104
	sw	$2,44($sp)
	lw	$2,44($sp)
	lw	$2,0($2)
	sw	$2,48($sp)
	lw	$2,44($sp)
	lw	$2,0($2)
	sw	$2,48($sp)
	lw	$2,48($sp)
	sw	$2,32($sp)
	lw	$2,32($sp)
	andi	$2,$2,0x1
	beq	$2,$0,$L214
	nop

	lui	$2,%hi($LC50)
	addiu	$4,$2,%lo($LC50)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	j	$L215
	nop

$L214:
	lui	$2,%hi($LC51)
	addiu	$4,$2,%lo($LC51)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L215:
	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$2,$2,0x130
	sw	$2,52($sp)
	lw	$2,52($sp)
	lw	$2,0($2)
	sw	$2,56($sp)
	lw	$2,52($sp)
	lw	$2,0($2)
	sw	$2,56($sp)
	lw	$2,56($sp)
	sw	$2,32($sp)
	lw	$2,32($sp)
	srl	$2,$2,18
	andi	$3,$2,0x7
	li	$2,6			# 0x6
	bne	$3,$2,$L217
	nop

	lui	$2,%hi($LC52)
	addiu	$4,$2,%lo($LC52)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	j	$L218
	nop

$L217:
	lui	$2,%hi($LC53)
	addiu	$4,$2,%lo($LC53)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L218:
	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$2,$2,0x21c
	sw	$2,60($sp)
	lw	$2,60($sp)
	lw	$2,0($2)
	sw	$2,64($sp)
	lw	$2,60($sp)
	lw	$2,0($2)
	sw	$2,64($sp)
	lw	$2,64($sp)
	sw	$2,32($sp)
	lw	$2,32($sp)
	andi	$3,$2,0xf
	li	$2,10			# 0xa
	bne	$3,$2,$L220
	nop

	lui	$2,%hi($LC54)
	addiu	$4,$2,%lo($LC54)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	j	$L221
	nop

$L220:
	lui	$2,%hi($LC55)
	addiu	$4,$2,%lo($LC55)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L221:
	lui	$2,%hi(phy_date_version)
	lw	$5,%lo(phy_date_version)($2)
	lui	$2,%hi(phy_version)
	lw	$6,%lo(phy_version)($2)
	lui	$2,%hi(mt7570_version)
	lw	$3,%lo(mt7570_version)($2)
	lui	$2,%hi($LC56)
	addiu	$4,$2,%lo($LC56)
	move	$7,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi(phy_los_status)
	addiu	$2,$2,%lo(phy_los_status)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi($LC57)
	addiu	$4,$2,%lo($LC57)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi(mt7570_select)
	lw	$3,%lo(mt7570_select)($2)
	li	$2,1			# 0x1
	bne	$3,$2,$L222
	nop

	lui	$2,%hi($LC58)
	addiu	$4,$2,%lo($LC58)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi(internal_DDMI)
	lw	$3,%lo(internal_DDMI)($2)
	li	$2,1			# 0x1
	bne	$3,$2,$L223
	nop

	lui	$2,%hi($LC59)
	addiu	$4,$2,%lo($LC59)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	j	$L224
	nop

$L223:
	lui	$2,%hi($LC60)
	addiu	$4,$2,%lo($LC60)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L224:
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,116($2)
	move	$3,$2
	li	$2,316			# 0x13c
	sw	$2,16($sp)
	addiu	$2,$sp,68
	sw	$2,20($sp)
	li	$2,4			# 0x4
	sw	$2,24($sp)
	move	$4,$0
	move	$5,$3
	li	$6,112			# 0x70
	li	$7,2			# 0x2
	lui	$2,%hi(phy_I2C_read)
	addiu	$2,$2,%lo(phy_I2C_read)
	jalr	$2
	nop

	lbu	$2,68($sp)
	andi	$2,$2,0x2
	sra	$2,$2,1
	sw	$2,32($sp)
	lw	$3,32($sp)
	li	$2,1			# 0x1
	bne	$3,$2,$L225
	nop

	lui	$2,%hi($LC61)
	addiu	$4,$2,%lo($LC61)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	j	$L226
	nop

$L225:
	lui	$2,%hi($LC62)
	addiu	$4,$2,%lo($LC62)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L226:
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,116($2)
	move	$3,$2
	li	$2,332			# 0x14c
	sw	$2,16($sp)
	addiu	$2,$sp,68
	sw	$2,20($sp)
	li	$2,4			# 0x4
	sw	$2,24($sp)
	move	$4,$0
	move	$5,$3
	li	$6,112			# 0x70
	li	$7,2			# 0x2
	lui	$2,%hi(phy_I2C_read)
	addiu	$2,$2,%lo(phy_I2C_read)
	jalr	$2
	nop

	lbu	$2,68($sp)
	andi	$2,$2,0x2
	sra	$2,$2,1
	sw	$2,32($sp)
	lw	$3,32($sp)
	li	$2,1			# 0x1
	bne	$3,$2,$L227
	nop

	lui	$2,%hi($LC63)
	addiu	$4,$2,%lo($LC63)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	j	$L209
	nop

$L227:
	lui	$2,%hi($LC64)
	addiu	$4,$2,%lo($LC64)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	j	$L209
	nop

$L222:
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,116($2)
	move	$3,$2
	li	$2,20			# 0x14
	sw	$2,16($sp)
	addiu	$2,$sp,72
	sw	$2,20($sp)
	li	$2,16			# 0x10
	sw	$2,24($sp)
	move	$4,$0
	move	$5,$3
	li	$6,80			# 0x50
	li	$7,1			# 0x1
	lui	$2,%hi(phy_I2C_read)
	addiu	$2,$2,%lo(phy_I2C_read)
	jalr	$2
	nop

	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,116($2)
	move	$3,$2
	li	$2,40			# 0x28
	sw	$2,16($sp)
	addiu	$2,$sp,92
	sw	$2,20($sp)
	li	$2,16			# 0x10
	sw	$2,24($sp)
	move	$4,$0
	move	$5,$3
	li	$6,80			# 0x50
	li	$7,1			# 0x1
	lui	$2,%hi(phy_I2C_read)
	addiu	$2,$2,%lo(phy_I2C_read)
	jalr	$2
	nop

	sb	$0,108($sp)
	sb	$0,88($sp)
	addiu	$3,$sp,72
	lui	$2,%hi($LC65)
	addiu	$4,$2,%lo($LC65)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	addiu	$3,$sp,92
	lui	$2,%hi($LC66)
	addiu	$4,$2,%lo($LC66)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L209:
	lw	$31,116($sp)
	addiu	$sp,$sp,120
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	pon_phy_status
	.size	pon_phy_status, .-pon_phy_status
	.rdata
	.align	2
$LC67:
	.ascii	"0x%x = 0x%x \012\000"
	.section	.text.pon_register,"ax",@progbits
	.align	2
	.globl	pon_register
	.set	nomips16
	.set	nomicromips
	.ent	pon_register
	.type	pon_register, @function
pon_register:
	.frame	$sp,40,$31		# vars= 16, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-40
	sw	$31,36($sp)
	sw	$4,40($sp)
	sw	$0,16($sp)
	lw	$2,40($sp)
	sw	$2,20($sp)
	lw	$2,20($sp)
	lw	$2,0($2)
	sw	$2,24($sp)
	lw	$2,20($sp)
	lw	$2,0($2)
	sw	$2,24($sp)
	lw	$2,24($sp)
	sw	$2,16($sp)
	lui	$2,%hi($LC67)
	addiu	$4,$2,%lo($LC67)
	lw	$5,40($sp)
	lw	$6,16($sp)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lw	$31,36($sp)
	addiu	$sp,$sp,40
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	pon_register
	.size	pon_register, .-pon_register
	.rdata
	.align	2
$LC68:
	.ascii	"[%lu]0ms[%s:%d]\012  eye register 0x11C read_data :0x%.8"
	.ascii	"x\012\000"
	.align	2
$LC69:
	.ascii	"[%lu]0ms[%s:%d]\012  eye register 0x11C write_data :0x%."
	.ascii	"8x\012\000"
	.align	2
$LC70:
	.ascii	"[%lu]0ms[%s:%d]\012  eye register 0x110 read_data :0x%.8"
	.ascii	"x\012\000"
	.align	2
$LC71:
	.ascii	"[%lu]0ms[%s:%d]\012  eye register 0x110 write_data :0x%."
	.ascii	"8x\012\000"
	.align	2
$LC72:
	.ascii	"[%lu]0ms[%s:%d]\012  eye register 0x12C read_data :0x%.8"
	.ascii	"x\012\000"
	.align	2
$LC73:
	.ascii	"[%lu]0ms[%s:%d]\012  eye register 0x12C write_data :0x%."
	.ascii	"8x\012\000"
	.align	2
$LC74:
	.ascii	"[%lu]0ms[%s:%d]\012  eye register 0x118 read_data :0x%.8"
	.ascii	"x\012\000"
	.align	2
$LC75:
	.ascii	"[%lu]0ms[%s:%d]\012  eye register 0x118 write_data :0x%."
	.ascii	"8x\012\000"
	.align	2
$LC76:
	.ascii	"%d %d %d \012\000"
	.section	.text.phy_eye_monitor,"ax",@progbits
	.align	2
	.globl	phy_eye_monitor
	.set	nomips16
	.set	nomicromips
	.ent	phy_eye_monitor
	.type	phy_eye_monitor, @function
phy_eye_monitor:
	.frame	$sp,152,$31		# vars= 120, regs= 2/0, args= 24, gp= 0
	.mask	0x80010000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-152
	sw	$31,148($sp)
	sw	$16,144($sp)
	move	$3,$4
	move	$2,$5
	sw	$6,160($sp)
	sh	$3,152($sp)
	sb	$2,156($sp)
	sw	$0,24($sp)
	sw	$0,48($sp)
	sw	$0,36($sp)
	lui	$2,%hi(pon_phy_deinit)
	addiu	$2,$2,%lo(pon_phy_deinit)
	jalr	$2
	nop

	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$2,$2,0x11c
	sw	$2,52($sp)
	lw	$2,52($sp)
	lw	$2,0($2)
	sw	$2,56($sp)
	lw	$2,52($sp)
	lw	$2,0($2)
	sw	$2,56($sp)
	lw	$2,56($sp)
	sw	$2,48($sp)
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,136($2)
	andi	$2,$2,0x10
	beq	$2,$0,$L234
	nop

	lui	$2,%hi(jiffies)
	lw	$16,%lo(jiffies)($2)
	lui	$2,%hi($LC2)
	addiu	$4,$2,%lo($LC2)
	li	$5,47			# 0x2f
	lui	$2,%hi(strrchr)
	addiu	$2,$2,%lo(strrchr)
	jalr	$2
	nop

	addiu	$3,$2,1
	lw	$2,48($sp)
	sw	$2,16($sp)
	lui	$2,%hi($LC68)
	addiu	$4,$2,%lo($LC68)
	move	$5,$16
	move	$6,$3
	li	$7,1459			# 0x5b3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L234:
	lw	$3,48($sp)
	li	$2,-65536			# 0xffffffffffff0000
	and	$3,$3,$2
	lhu	$2,152($sp)
	or	$2,$3,$2
	sw	$2,48($sp)
	lw	$3,48($sp)
	li	$2,134217728			# 0x8000000
	or	$2,$3,$2
	sw	$2,48($sp)
	lw	$2,48($sp)
	sw	$2,24($sp)
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,136($2)
	andi	$2,$2,0x10
	beq	$2,$0,$L235
	nop

	lui	$2,%hi(jiffies)
	lw	$16,%lo(jiffies)($2)
	lui	$2,%hi($LC2)
	addiu	$4,$2,%lo($LC2)
	li	$5,47			# 0x2f
	lui	$2,%hi(strrchr)
	addiu	$2,$2,%lo(strrchr)
	jalr	$2
	nop

	addiu	$3,$2,1
	lw	$2,24($sp)
	sw	$2,16($sp)
	lui	$2,%hi($LC69)
	addiu	$4,$2,%lo($LC69)
	move	$5,$16
	move	$6,$3
	li	$7,1463			# 0x5b7
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L235:
	lw	$4,24($sp)
	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$5,$2,0x11c
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$2,$2,0x110
	sw	$2,60($sp)
	lw	$2,60($sp)
	lw	$2,0($2)
	sw	$2,64($sp)
	lw	$2,60($sp)
	lw	$2,0($2)
	sw	$2,64($sp)
	lw	$2,64($sp)
	sw	$2,48($sp)
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,136($2)
	andi	$2,$2,0x10
	beq	$2,$0,$L237
	nop

	lui	$2,%hi(jiffies)
	lw	$16,%lo(jiffies)($2)
	lui	$2,%hi($LC2)
	addiu	$4,$2,%lo($LC2)
	li	$5,47			# 0x2f
	lui	$2,%hi(strrchr)
	addiu	$2,$2,%lo(strrchr)
	jalr	$2
	nop

	addiu	$3,$2,1
	lw	$2,48($sp)
	sw	$2,16($sp)
	lui	$2,%hi($LC70)
	addiu	$4,$2,%lo($LC70)
	move	$5,$16
	move	$6,$3
	li	$7,1466			# 0x5ba
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L237:
	lw	$2,48($sp)
	ori	$2,$2,0x800
	sw	$2,24($sp)
	li	$4,1			# 0x1
	lui	$2,%hi(msleep)
	addiu	$2,$2,%lo(msleep)
	jalr	$2
	nop

	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,136($2)
	andi	$2,$2,0x10
	beq	$2,$0,$L238
	nop

	lui	$2,%hi(jiffies)
	lw	$16,%lo(jiffies)($2)
	lui	$2,%hi($LC2)
	addiu	$4,$2,%lo($LC2)
	li	$5,47			# 0x2f
	lui	$2,%hi(strrchr)
	addiu	$2,$2,%lo(strrchr)
	jalr	$2
	nop

	addiu	$3,$2,1
	lw	$2,24($sp)
	sw	$2,16($sp)
	lui	$2,%hi($LC71)
	addiu	$4,$2,%lo($LC71)
	move	$5,$16
	move	$6,$3
	li	$7,1469			# 0x5bd
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L238:
	lw	$4,24($sp)
	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$5,$2,0x110
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	li	$2,64			# 0x40
	sw	$2,28($sp)
	j	$L239
	nop

$L244:
	lw	$2,28($sp)
	sw	$2,36($sp)
	lw	$2,28($sp)
	bgez	$2,$L240
	nop

	lw	$2,36($sp)
	addiu	$2,$2,128
	sw	$2,36($sp)
$L240:
	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$2,$2,0x12c
	sw	$2,68($sp)
	lw	$2,68($sp)
	lw	$2,0($2)
	sw	$2,72($sp)
	lw	$2,68($sp)
	lw	$2,0($2)
	sw	$2,72($sp)
	lw	$2,72($sp)
	sw	$2,48($sp)
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,136($2)
	andi	$2,$2,0x10
	beq	$2,$0,$L242
	nop

	lui	$2,%hi(jiffies)
	lw	$16,%lo(jiffies)($2)
	lui	$2,%hi($LC2)
	addiu	$4,$2,%lo($LC2)
	li	$5,47			# 0x2f
	lui	$2,%hi(strrchr)
	addiu	$2,$2,%lo(strrchr)
	jalr	$2
	nop

	addiu	$3,$2,1
	lw	$2,48($sp)
	sw	$2,16($sp)
	lui	$2,%hi($LC72)
	addiu	$4,$2,%lo($LC72)
	move	$5,$16
	move	$6,$3
	li	$7,1478			# 0x5c6
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L242:
	lw	$3,48($sp)
	li	$2,-2130771968			# 0xffffffff80ff0000
	ori	$2,$2,0xffff
	and	$2,$3,$2
	lw	$3,36($sp)
	addiu	$3,$3,64
	andi	$3,$3,0x7f
	sll	$3,$3,24
	or	$2,$2,$3
	sw	$2,24($sp)
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,136($2)
	andi	$2,$2,0x10
	beq	$2,$0,$L243
	nop

	lui	$2,%hi(jiffies)
	lw	$16,%lo(jiffies)($2)
	lui	$2,%hi($LC2)
	addiu	$4,$2,%lo($LC2)
	li	$5,47			# 0x2f
	lui	$2,%hi(strrchr)
	addiu	$2,$2,%lo(strrchr)
	jalr	$2
	nop

	addiu	$3,$2,1
	lw	$2,24($sp)
	sw	$2,16($sp)
	lui	$2,%hi($LC73)
	addiu	$4,$2,%lo($LC73)
	move	$5,$16
	move	$6,$3
	li	$7,1480			# 0x5c8
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L243:
	lw	$4,24($sp)
	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$5,$2,0x12c
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	lbu	$2,156($sp)
	lw	$3,28($sp)
	subu	$2,$3,$2
	sw	$2,28($sp)
$L239:
	lw	$2,160($sp)
	subu	$3,$0,$2
	lw	$2,28($sp)
	slt	$2,$3,$2
	bne	$2,$0,$L244
	nop

	sw	$0,40($sp)
	sw	$0,44($sp)
	lw	$2,160($sp)
	subu	$2,$0,$2
	sw	$2,28($sp)
	j	$L245
	nop

$L267:
	lw	$2,28($sp)
	sw	$2,36($sp)
	lw	$2,28($sp)
	bgez	$2,$L246
	nop

	lw	$2,36($sp)
	addiu	$2,$2,128
	sw	$2,36($sp)
$L246:
	lw	$2,36($sp)
	andi	$2,$2,0x1
	bne	$2,$0,$L247
	nop

	sw	$0,40($sp)
	lbu	$2,156($sp)
	sw	$2,44($sp)
	j	$L248
	nop

$L247:
	li	$2,127			# 0x7f
	sw	$2,40($sp)
	lbu	$2,156($sp)
	subu	$2,$0,$2
	sw	$2,44($sp)
$L248:
	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$2,$2,0x12c
	sw	$2,76($sp)
	lw	$2,76($sp)
	lw	$2,0($2)
	sw	$2,80($sp)
	lw	$2,76($sp)
	lw	$2,0($2)
	sw	$2,80($sp)
	lw	$2,80($sp)
	sw	$2,48($sp)
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,136($2)
	andi	$2,$2,0x10
	beq	$2,$0,$L250
	nop

	lui	$2,%hi(jiffies)
	lw	$16,%lo(jiffies)($2)
	lui	$2,%hi($LC2)
	addiu	$4,$2,%lo($LC2)
	li	$5,47			# 0x2f
	lui	$2,%hi(strrchr)
	addiu	$2,$2,%lo(strrchr)
	jalr	$2
	nop

	addiu	$3,$2,1
	lw	$2,48($sp)
	sw	$2,16($sp)
	lui	$2,%hi($LC72)
	addiu	$4,$2,%lo($LC72)
	move	$5,$16
	move	$6,$3
	li	$7,1502			# 0x5de
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L250:
	lw	$3,48($sp)
	li	$2,-2130771968			# 0xffffffff80ff0000
	ori	$2,$2,0xffff
	and	$2,$3,$2
	lw	$3,36($sp)
	addiu	$3,$3,64
	andi	$3,$3,0x7f
	sll	$3,$3,24
	or	$2,$2,$3
	sw	$2,24($sp)
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,136($2)
	andi	$2,$2,0x10
	beq	$2,$0,$L251
	nop

	lui	$2,%hi(jiffies)
	lw	$16,%lo(jiffies)($2)
	lui	$2,%hi($LC2)
	addiu	$4,$2,%lo($LC2)
	li	$5,47			# 0x2f
	lui	$2,%hi(strrchr)
	addiu	$2,$2,%lo(strrchr)
	jalr	$2
	nop

	addiu	$3,$2,1
	lw	$2,24($sp)
	sw	$2,16($sp)
	lui	$2,%hi($LC73)
	addiu	$4,$2,%lo($LC73)
	move	$5,$16
	move	$6,$3
	li	$7,1504			# 0x5e0
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L251:
	lw	$4,24($sp)
	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$5,$2,0x12c
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	lw	$2,40($sp)
	sw	$2,32($sp)
	j	$L252
	nop

$L266:
	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$2,$2,0x118
	sw	$2,84($sp)
	lw	$2,84($sp)
	lw	$2,0($2)
	sw	$2,88($sp)
	lw	$2,84($sp)
	lw	$2,0($2)
	sw	$2,88($sp)
	lw	$2,88($sp)
	sw	$2,48($sp)
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,136($2)
	andi	$2,$2,0x10
	beq	$2,$0,$L254
	nop

	lui	$2,%hi(jiffies)
	lw	$16,%lo(jiffies)($2)
	lui	$2,%hi($LC2)
	addiu	$4,$2,%lo($LC2)
	li	$5,47			# 0x2f
	lui	$2,%hi(strrchr)
	addiu	$2,$2,%lo(strrchr)
	jalr	$2
	nop

	addiu	$3,$2,1
	lw	$2,48($sp)
	sw	$2,16($sp)
	lui	$2,%hi($LC74)
	addiu	$4,$2,%lo($LC74)
	move	$5,$16
	move	$6,$3
	li	$7,1509			# 0x5e5
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L254:
	lw	$3,48($sp)
	li	$2,-128			# 0xffffffffffffff80
	and	$3,$3,$2
	lw	$2,32($sp)
	addiu	$2,$2,64
	andi	$2,$2,0x7f
	or	$2,$3,$2
	sw	$2,24($sp)
	lui	$2,%hi(schedule)
	addiu	$2,$2,%lo(schedule)
	jalr	$2
	nop

	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,136($2)
	andi	$2,$2,0x10
	beq	$2,$0,$L255
	nop

	lui	$2,%hi(jiffies)
	lw	$16,%lo(jiffies)($2)
	lui	$2,%hi($LC2)
	addiu	$4,$2,%lo($LC2)
	li	$5,47			# 0x2f
	lui	$2,%hi(strrchr)
	addiu	$2,$2,%lo(strrchr)
	jalr	$2
	nop

	addiu	$3,$2,1
	lw	$2,24($sp)
	sw	$2,16($sp)
	lui	$2,%hi($LC75)
	addiu	$4,$2,%lo($LC75)
	move	$5,$16
	move	$6,$3
	li	$7,1512			# 0x5e8
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L255:
	lw	$4,24($sp)
	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$5,$2,0x118
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$2,$2,0x110
	sw	$2,92($sp)
	lw	$2,92($sp)
	lw	$2,0($2)
	sw	$2,96($sp)
	lw	$2,92($sp)
	lw	$2,0($2)
	sw	$2,96($sp)
	lw	$2,96($sp)
	sw	$2,48($sp)
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,136($2)
	andi	$2,$2,0x10
	beq	$2,$0,$L257
	nop

	lui	$2,%hi(jiffies)
	lw	$16,%lo(jiffies)($2)
	lui	$2,%hi($LC2)
	addiu	$4,$2,%lo($LC2)
	li	$5,47			# 0x2f
	lui	$2,%hi(strrchr)
	addiu	$2,$2,%lo(strrchr)
	jalr	$2
	nop

	addiu	$3,$2,1
	lw	$2,48($sp)
	sw	$2,16($sp)
	lui	$2,%hi($LC70)
	addiu	$4,$2,%lo($LC70)
	move	$5,$16
	move	$6,$3
	li	$7,1515			# 0x5eb
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L257:
	lw	$2,48($sp)
	ori	$2,$2,0x1000
	sw	$2,24($sp)
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,136($2)
	andi	$2,$2,0x10
	beq	$2,$0,$L258
	nop

	lui	$2,%hi(jiffies)
	lw	$16,%lo(jiffies)($2)
	lui	$2,%hi($LC2)
	addiu	$4,$2,%lo($LC2)
	li	$5,47			# 0x2f
	lui	$2,%hi(strrchr)
	addiu	$2,$2,%lo(strrchr)
	jalr	$2
	nop

	addiu	$3,$2,1
	lw	$2,24($sp)
	sw	$2,16($sp)
	lui	$2,%hi($LC71)
	addiu	$4,$2,%lo($LC71)
	move	$5,$16
	move	$6,$3
	li	$7,1518			# 0x5ee
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L258:
	lw	$4,24($sp)
	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$5,$2,0x110
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

$L260:
	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$2,$2,0x13c
	sw	$2,100($sp)
	lw	$2,100($sp)
	lw	$2,0($2)
	sw	$2,104($sp)
	lw	$2,100($sp)
	lw	$2,0($2)
	sw	$2,104($sp)
	lw	$2,104($sp)
	sw	$2,48($sp)
	lw	$2,48($sp)
	andi	$2,$2,0x400
	beq	$2,$0,$L260
	nop

	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$2,$2,0x148
	sw	$2,108($sp)
	lw	$2,108($sp)
	lw	$2,0($2)
	sw	$2,112($sp)
	lw	$2,108($sp)
	lw	$2,0($2)
	sw	$2,112($sp)
	lw	$2,112($sp)
	sw	$2,48($sp)
	lw	$2,48($sp)
	ext	$2,$2,0,20
	sw	$2,48($sp)
	lui	$2,%hi($LC76)
	addiu	$4,$2,%lo($LC76)
	lw	$5,36($sp)
	lw	$6,32($sp)
	lw	$7,48($sp)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$2,$2,0x110
	sw	$2,116($sp)
	lw	$2,116($sp)
	lw	$2,0($2)
	sw	$2,120($sp)
	lw	$2,116($sp)
	lw	$2,0($2)
	sw	$2,120($sp)
	lw	$2,120($sp)
	sw	$2,48($sp)
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,136($2)
	andi	$2,$2,0x10
	beq	$2,$0,$L263
	nop

	lui	$2,%hi(jiffies)
	lw	$16,%lo(jiffies)($2)
	lui	$2,%hi($LC2)
	addiu	$4,$2,%lo($LC2)
	li	$5,47			# 0x2f
	lui	$2,%hi(strrchr)
	addiu	$2,$2,%lo(strrchr)
	jalr	$2
	nop

	addiu	$3,$2,1
	lw	$2,48($sp)
	sw	$2,16($sp)
	lui	$2,%hi($LC70)
	addiu	$4,$2,%lo($LC70)
	move	$5,$16
	move	$6,$3
	li	$7,1528			# 0x5f8
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L263:
	lw	$3,48($sp)
	li	$2,-4097			# 0xffffffffffffefff
	and	$2,$3,$2
	sw	$2,24($sp)
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,136($2)
	andi	$2,$2,0x10
	beq	$2,$0,$L264
	nop

	lui	$2,%hi(jiffies)
	lw	$16,%lo(jiffies)($2)
	lui	$2,%hi($LC2)
	addiu	$4,$2,%lo($LC2)
	li	$5,47			# 0x2f
	lui	$2,%hi(strrchr)
	addiu	$2,$2,%lo(strrchr)
	jalr	$2
	nop

	addiu	$3,$2,1
	lw	$2,24($sp)
	sw	$2,16($sp)
	lui	$2,%hi($LC71)
	addiu	$4,$2,%lo($LC71)
	move	$5,$16
	move	$6,$3
	li	$7,1531			# 0x5fb
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L264:
	lw	$4,24($sp)
	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$5,$2,0x110
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	lw	$3,32($sp)
	lw	$2,44($sp)
	addu	$2,$3,$2
	sw	$2,32($sp)
$L252:
	lw	$2,32($sp)
	slt	$2,$2,128
	beq	$2,$0,$L265
	nop

	lw	$2,32($sp)
	bgez	$2,$L266
	nop

$L265:
	lbu	$2,156($sp)
	lw	$3,28($sp)
	addu	$2,$3,$2
	sw	$2,28($sp)
$L245:
	li	$3,128			# 0x80
	lw	$2,160($sp)
	subu	$3,$3,$2
	lw	$2,28($sp)
	slt	$2,$2,$3
	bne	$2,$0,$L267
	nop

	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$2,$2,0x110
	sw	$2,124($sp)
	lw	$2,124($sp)
	lw	$2,0($2)
	sw	$2,128($sp)
	lw	$2,124($sp)
	lw	$2,0($2)
	sw	$2,128($sp)
	lw	$2,128($sp)
	sw	$2,48($sp)
	lw	$3,48($sp)
	li	$2,-2049			# 0xfffffffffffff7ff
	and	$2,$3,$2
	sw	$2,24($sp)
	li	$3,127			# 0x7f
	lw	$2,160($sp)
	subu	$2,$3,$2
	sw	$2,28($sp)
	j	$L269
	nop

$L273:
	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$2,$2,0x12c
	sw	$2,132($sp)
	lw	$2,132($sp)
	lw	$2,0($2)
	sw	$2,136($sp)
	lw	$2,132($sp)
	lw	$2,0($2)
	sw	$2,136($sp)
	lw	$2,136($sp)
	sw	$2,48($sp)
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,136($2)
	andi	$2,$2,0x10
	beq	$2,$0,$L271
	nop

	lui	$2,%hi(jiffies)
	lw	$16,%lo(jiffies)($2)
	lui	$2,%hi($LC2)
	addiu	$4,$2,%lo($LC2)
	li	$5,47			# 0x2f
	lui	$2,%hi(strrchr)
	addiu	$2,$2,%lo(strrchr)
	jalr	$2
	nop

	addiu	$3,$2,1
	lw	$2,48($sp)
	sw	$2,16($sp)
	lui	$2,%hi($LC72)
	addiu	$4,$2,%lo($LC72)
	move	$5,$16
	move	$6,$3
	li	$7,1541			# 0x605
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L271:
	lw	$3,48($sp)
	li	$2,-2130771968			# 0xffffffff80ff0000
	ori	$2,$2,0xffff
	and	$2,$3,$2
	lw	$3,28($sp)
	addiu	$3,$3,64
	andi	$3,$3,0x7f
	sll	$3,$3,24
	or	$2,$2,$3
	sw	$2,24($sp)
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,136($2)
	andi	$2,$2,0x10
	beq	$2,$0,$L272
	nop

	lui	$2,%hi(jiffies)
	lw	$16,%lo(jiffies)($2)
	lui	$2,%hi($LC2)
	addiu	$4,$2,%lo($LC2)
	li	$5,47			# 0x2f
	lui	$2,%hi(strrchr)
	addiu	$2,$2,%lo(strrchr)
	jalr	$2
	nop

	addiu	$3,$2,1
	lw	$2,24($sp)
	sw	$2,16($sp)
	lui	$2,%hi($LC73)
	addiu	$4,$2,%lo($LC73)
	move	$5,$16
	move	$6,$3
	li	$7,1543			# 0x607
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L272:
	lw	$4,24($sp)
	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$5,$2,0x12c
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	lbu	$2,156($sp)
	lw	$3,28($sp)
	subu	$2,$3,$2
	sw	$2,28($sp)
$L269:
	lw	$2,28($sp)
	slt	$2,$2,65
	beq	$2,$0,$L273
	nop

	lw	$4,24($sp)
	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$5,$2,0x110
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	lw	$31,148($sp)
	lw	$16,144($sp)
	addiu	$sp,$sp,152
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	phy_eye_monitor
	.size	phy_eye_monitor, .-phy_eye_monitor
	.rdata
	.align	2
$LC77:
	.ascii	"%d\012\000"
	.section	.text.phy_rogue_read_proc,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	phy_rogue_read_proc
	.type	phy_rogue_read_proc, @function
phy_rogue_read_proc:
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
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lw	$2,132($2)
	ext	$2,$2,24,1
	andi	$2,$2,0x00ff
	move	$3,$2
	lw	$4,32($sp)
	lui	$2,%hi($LC77)
	addiu	$5,$2,%lo($LC77)
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
	beq	$2,$0,$L275
	nop

	lw	$2,44($sp)
	sw	$2,16($sp)
	j	$L276
	nop

$L275:
	lw	$2,48($sp)
	li	$3,1			# 0x1
	sw	$3,0($2)
$L276:
	lw	$2,16($sp)
	bgez	$2,$L277
	nop

	sw	$0,16($sp)
$L277:
	lw	$2,16($sp)
	lw	$31,28($sp)
	addiu	$sp,$sp,32
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	phy_rogue_read_proc
	.size	phy_rogue_read_proc, .-phy_rogue_read_proc
	.globl	gphy_test_rx_ber
	.section	.bss
	.align	2
	.type	gphy_test_rx_ber, @object
	.size	gphy_test_rx_ber, 4
gphy_test_rx_ber:
	.space	4
	.rdata
	.align	2
$LC78:
	.ascii	"\012\000"
	.align	2
$LC79:
	.ascii	"%s\012\000"
	.align	2
$LC80:
	.ascii	"**************************************\000"
	.align	2
$LC81:
	.ascii	"**       XPON  PHY Status           **\000"
	.align	2
$LC82:
	.ascii	" PHY Status: plug   (PHY_RD: %#x <b[20:18]>   RX_SYNC: %"
	.ascii	"#x <b[3:0]>)\012\000"
	.align	2
$LC83:
	.ascii	" PHY Status: unplug (PHY_RD: %#x <b[20:18]>   RX_SYNC: %"
	.ascii	"#x <b[3:0]>)\012\000"
	.align	2
$LC84:
	.ascii	"**************************************\012\000"
	.align	2
$LC85:
	.ascii	"**             Raw data             **\012\000"
	.align	2
$LC86:
	.ascii	" PHYSTA1          (0xBFAF0130): %#03x\012\000"
	.align	2
$LC87:
	.ascii	" XPON_STA         (0xBFAF05E0): %#03x\012\000"
	.align	2
$LC88:
	.ascii	" PHYRX_STATUS     (0xBFAF021C): %#03x\012\000"
	.section	.text.phy_info_read_proc,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	phy_info_read_proc
	.type	phy_info_read_proc, @function
phy_info_read_proc:
	.frame	$sp,64,$31		# vars= 40, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-64
	sw	$31,60($sp)
	sw	$4,64($sp)
	sw	$5,68($sp)
	sw	$6,72($sp)
	sw	$7,76($sp)
	sw	$0,16($sp)
	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$2,$2,0x130
	sw	$2,32($sp)
	lw	$2,32($sp)
	lw	$2,0($2)
	sw	$2,36($sp)
	lw	$2,32($sp)
	lw	$2,0($2)
	sw	$2,36($sp)
	lw	$2,36($sp)
	sw	$2,20($sp)
	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$2,$2,0x5e0
	sw	$2,40($sp)
	lw	$2,40($sp)
	lw	$2,0($2)
	sw	$2,44($sp)
	lw	$2,40($sp)
	lw	$2,0($2)
	sw	$2,44($sp)
	lw	$2,44($sp)
	sw	$2,24($sp)
	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$2,$2,0x21c
	sw	$2,48($sp)
	lw	$2,48($sp)
	lw	$2,0($2)
	sw	$2,52($sp)
	lw	$2,48($sp)
	lw	$2,0($2)
	sw	$2,52($sp)
	lw	$2,52($sp)
	sw	$2,28($sp)
	lw	$4,64($sp)
	lui	$2,%hi($LC78)
	addiu	$5,$2,%lo($LC78)
	lui	$2,%hi(sprintf)
	addiu	$2,$2,%lo(sprintf)
	jalr	$2
	nop

	sw	$2,16($sp)
	lw	$2,16($sp)
	lw	$3,64($sp)
	addu	$2,$3,$2
	move	$4,$2
	lui	$2,%hi($LC79)
	addiu	$5,$2,%lo($LC79)
	lui	$2,%hi($LC80)
	addiu	$6,$2,%lo($LC80)
	lui	$2,%hi(sprintf)
	addiu	$2,$2,%lo(sprintf)
	jalr	$2
	nop

	move	$3,$2
	lw	$2,16($sp)
	addu	$2,$2,$3
	sw	$2,16($sp)
	lw	$2,16($sp)
	lw	$3,64($sp)
	addu	$2,$3,$2
	move	$4,$2
	lui	$2,%hi($LC79)
	addiu	$5,$2,%lo($LC79)
	lui	$2,%hi($LC81)
	addiu	$6,$2,%lo($LC81)
	lui	$2,%hi(sprintf)
	addiu	$2,$2,%lo(sprintf)
	jalr	$2
	nop

	move	$3,$2
	lw	$2,16($sp)
	addu	$2,$2,$3
	sw	$2,16($sp)
	lw	$2,16($sp)
	lw	$3,64($sp)
	addu	$2,$3,$2
	move	$4,$2
	lui	$2,%hi($LC79)
	addiu	$5,$2,%lo($LC79)
	lui	$2,%hi($LC80)
	addiu	$6,$2,%lo($LC80)
	lui	$2,%hi(sprintf)
	addiu	$2,$2,%lo(sprintf)
	jalr	$2
	nop

	move	$3,$2
	lw	$2,16($sp)
	addu	$2,$2,$3
	sw	$2,16($sp)
	lw	$2,16($sp)
	lw	$3,64($sp)
	addu	$2,$3,$2
	move	$4,$2
	lui	$2,%hi($LC78)
	addiu	$5,$2,%lo($LC78)
	lui	$2,%hi(sprintf)
	addiu	$2,$2,%lo(sprintf)
	jalr	$2
	nop

	move	$3,$2
	lw	$2,16($sp)
	addu	$2,$2,$3
	sw	$2,16($sp)
	lw	$2,24($sp)
	bne	$2,$0,$L283
	nop

	lw	$2,16($sp)
	lw	$3,64($sp)
	addu	$4,$3,$2
	lw	$3,20($sp)
	li	$2,1572864			# 0x180000
	and	$2,$3,$2
	srl	$2,$2,18
	andi	$6,$2,0x7
	lw	$2,28($sp)
	andi	$3,$2,0xf
	lui	$2,%hi($LC82)
	addiu	$5,$2,%lo($LC82)
	move	$7,$3
	lui	$2,%hi(sprintf)
	addiu	$2,$2,%lo(sprintf)
	jalr	$2
	nop

	move	$3,$2
	lw	$2,16($sp)
	addu	$2,$2,$3
	sw	$2,16($sp)
	j	$L284
	nop

$L283:
	lw	$2,16($sp)
	lw	$3,64($sp)
	addu	$4,$3,$2
	lw	$3,20($sp)
	li	$2,1572864			# 0x180000
	and	$2,$3,$2
	srl	$2,$2,18
	andi	$6,$2,0x7
	lw	$2,28($sp)
	andi	$3,$2,0xf
	lui	$2,%hi($LC83)
	addiu	$5,$2,%lo($LC83)
	move	$7,$3
	lui	$2,%hi(sprintf)
	addiu	$2,$2,%lo(sprintf)
	jalr	$2
	nop

	move	$3,$2
	lw	$2,16($sp)
	addu	$2,$2,$3
	sw	$2,16($sp)
$L284:
	lw	$2,16($sp)
	lw	$3,64($sp)
	addu	$2,$3,$2
	move	$4,$2
	lui	$2,%hi($LC84)
	addiu	$5,$2,%lo($LC84)
	lui	$2,%hi(sprintf)
	addiu	$2,$2,%lo(sprintf)
	jalr	$2
	nop

	move	$3,$2
	lw	$2,16($sp)
	addu	$2,$2,$3
	sw	$2,16($sp)
	lw	$2,16($sp)
	lw	$3,64($sp)
	addu	$2,$3,$2
	move	$4,$2
	lui	$2,%hi($LC85)
	addiu	$5,$2,%lo($LC85)
	lui	$2,%hi(sprintf)
	addiu	$2,$2,%lo(sprintf)
	jalr	$2
	nop

	move	$3,$2
	lw	$2,16($sp)
	addu	$2,$2,$3
	sw	$2,16($sp)
	lw	$2,16($sp)
	lw	$3,64($sp)
	addu	$2,$3,$2
	move	$4,$2
	lui	$2,%hi($LC84)
	addiu	$5,$2,%lo($LC84)
	lui	$2,%hi(sprintf)
	addiu	$2,$2,%lo(sprintf)
	jalr	$2
	nop

	move	$3,$2
	lw	$2,16($sp)
	addu	$2,$2,$3
	sw	$2,16($sp)
	lw	$2,16($sp)
	lw	$3,64($sp)
	addu	$2,$3,$2
	move	$4,$2
	lui	$2,%hi($LC86)
	addiu	$5,$2,%lo($LC86)
	lw	$6,20($sp)
	lui	$2,%hi(sprintf)
	addiu	$2,$2,%lo(sprintf)
	jalr	$2
	nop

	move	$3,$2
	lw	$2,16($sp)
	addu	$2,$2,$3
	sw	$2,16($sp)
	lw	$2,16($sp)
	lw	$3,64($sp)
	addu	$2,$3,$2
	move	$4,$2
	lui	$2,%hi($LC87)
	addiu	$5,$2,%lo($LC87)
	lw	$6,24($sp)
	lui	$2,%hi(sprintf)
	addiu	$2,$2,%lo(sprintf)
	jalr	$2
	nop

	move	$3,$2
	lw	$2,16($sp)
	addu	$2,$2,$3
	sw	$2,16($sp)
	lw	$2,16($sp)
	lw	$3,64($sp)
	addu	$2,$3,$2
	move	$4,$2
	lui	$2,%hi($LC88)
	addiu	$5,$2,%lo($LC88)
	lw	$6,28($sp)
	lui	$2,%hi(sprintf)
	addiu	$2,$2,%lo(sprintf)
	jalr	$2
	nop

	move	$3,$2
	lw	$2,16($sp)
	addu	$2,$2,$3
	sw	$2,16($sp)
	lw	$3,16($sp)
	lw	$2,72($sp)
	subu	$2,$3,$2
	sw	$2,16($sp)
	lw	$2,72($sp)
	lw	$3,64($sp)
	addu	$3,$3,$2
	lw	$2,68($sp)
	sw	$3,0($2)
	lw	$3,16($sp)
	lw	$2,76($sp)
	slt	$2,$2,$3
	beq	$2,$0,$L285
	nop

	lw	$2,76($sp)
	sw	$2,16($sp)
	j	$L286
	nop

$L285:
	lw	$2,80($sp)
	li	$3,1			# 0x1
	sw	$3,0($2)
$L286:
	lw	$2,16($sp)
	bgez	$2,$L287
	nop

	sw	$0,16($sp)
$L287:
	lw	$2,16($sp)
	lw	$31,60($sp)
	addiu	$sp,$sp,64
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	phy_info_read_proc
	.size	phy_info_read_proc, .-phy_info_read_proc
	.rdata
	.align	2
$LC89:
	.ascii	"Temperature code: %x\012\000"
	.align	2
$LC90:
	.ascii	"VCC code: %x\012\000"
	.align	2
$LC91:
	.ascii	"Bias Current: %x\012\000"
	.align	2
$LC92:
	.ascii	"Modulation Current: %x\012\000"
	.align	2
$LC93:
	.ascii	"RSSI Current: %x\012\000"
	.align	2
$LC94:
	.ascii	"MPD Current: %x\012\000"
	.align	2
$LC95:
	.ascii	"phy_i2c_div_clock: 0x%x\012\000"
	.section	.text.phy_read_proc,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	phy_read_proc
	.type	phy_read_proc, @function
phy_read_proc:
	.frame	$sp,88,$31		# vars= 64, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-88
	sw	$31,84($sp)
	sw	$4,88($sp)
	sw	$5,92($sp)
	sw	$6,96($sp)
	sw	$7,100($sp)
	sw	$0,16($sp)
	sw	$0,64($sp)
	sw	$0,68($sp)
	sh	$0,72($sp)
	sw	$0,24($sp)
	sw	$0,28($sp)
	sw	$0,32($sp)
	sw	$0,36($sp)
	sw	$0,40($sp)
	sw	$0,44($sp)
	lui	$2,%hi(mt7570_ADC_temperature_get)
	addiu	$2,$2,%lo(mt7570_ADC_temperature_get)
	jalr	$2
	nop

	sw	$2,24($sp)
	lui	$2,%hi(mt7570_ADC_voltage_get)
	addiu	$2,$2,%lo(mt7570_ADC_voltage_get)
	jalr	$2
	nop

	sw	$2,28($sp)
	li	$4,3			# 0x3
	lui	$2,%hi(mt7570_information_output)
	addiu	$2,$2,%lo(mt7570_information_output)
	jalr	$2
	nop

	sw	$2,32($sp)
	li	$4,4			# 0x4
	lui	$2,%hi(mt7570_information_output)
	addiu	$2,$2,%lo(mt7570_information_output)
	jalr	$2
	nop

	sw	$2,36($sp)
	lui	$2,%hi(mt7570_RSSI_current)
	addiu	$2,$2,%lo(mt7570_RSSI_current)
	jalr	$2
	nop

	sw	$2,40($sp)
	lui	$2,%hi(mt7570_MPD_current)
	addiu	$2,$2,%lo(mt7570_MPD_current)
	jalr	$2
	nop

	sw	$2,44($sp)
	sw	$0,48($sp)
	sw	$0,20($sp)
	lw	$2,16($sp)
	lw	$3,88($sp)
	addu	$2,$3,$2
	move	$4,$2
	lui	$2,%hi($LC89)
	addiu	$5,$2,%lo($LC89)
	lw	$6,24($sp)
	lui	$2,%hi(sprintf)
	addiu	$2,$2,%lo(sprintf)
	jalr	$2
	nop

	move	$3,$2
	lw	$2,16($sp)
	addu	$2,$2,$3
	sw	$2,16($sp)
	lw	$2,16($sp)
	lw	$3,88($sp)
	addu	$2,$3,$2
	move	$4,$2
	lui	$2,%hi($LC90)
	addiu	$5,$2,%lo($LC90)
	lw	$6,28($sp)
	lui	$2,%hi(sprintf)
	addiu	$2,$2,%lo(sprintf)
	jalr	$2
	nop

	move	$3,$2
	lw	$2,16($sp)
	addu	$2,$2,$3
	sw	$2,16($sp)
	lw	$2,16($sp)
	lw	$3,88($sp)
	addu	$2,$3,$2
	move	$4,$2
	lui	$2,%hi($LC91)
	addiu	$5,$2,%lo($LC91)
	lw	$6,32($sp)
	lui	$2,%hi(sprintf)
	addiu	$2,$2,%lo(sprintf)
	jalr	$2
	nop

	move	$3,$2
	lw	$2,16($sp)
	addu	$2,$2,$3
	sw	$2,16($sp)
	lw	$2,16($sp)
	lw	$3,88($sp)
	addu	$2,$3,$2
	move	$4,$2
	lui	$2,%hi($LC92)
	addiu	$5,$2,%lo($LC92)
	lw	$6,36($sp)
	lui	$2,%hi(sprintf)
	addiu	$2,$2,%lo(sprintf)
	jalr	$2
	nop

	move	$3,$2
	lw	$2,16($sp)
	addu	$2,$2,$3
	sw	$2,16($sp)
	lw	$2,16($sp)
	lw	$3,88($sp)
	addu	$2,$3,$2
	move	$4,$2
	lui	$2,%hi($LC93)
	addiu	$5,$2,%lo($LC93)
	lw	$6,40($sp)
	lui	$2,%hi(sprintf)
	addiu	$2,$2,%lo(sprintf)
	jalr	$2
	nop

	move	$3,$2
	lw	$2,16($sp)
	addu	$2,$2,$3
	sw	$2,16($sp)
	lw	$2,16($sp)
	lw	$3,88($sp)
	addu	$2,$3,$2
	move	$4,$2
	lui	$2,%hi($LC94)
	addiu	$5,$2,%lo($LC94)
	lw	$6,44($sp)
	lui	$2,%hi(sprintf)
	addiu	$2,$2,%lo(sprintf)
	jalr	$2
	nop

	move	$3,$2
	lw	$2,16($sp)
	addu	$2,$2,$3
	sw	$2,16($sp)
	lw	$2,16($sp)
	lw	$3,88($sp)
	addu	$3,$3,$2
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,116($2)
	move	$6,$2
	move	$4,$3
	lui	$2,%hi($LC95)
	addiu	$5,$2,%lo($LC95)
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
	sw	$2,48($sp)
	lw	$3,48($sp)
	lw	$2,96($sp)
	slt	$2,$3,$2
	beq	$2,$0,$L290
	nop

	sw	$0,16($sp)
	lw	$2,48($sp)
	sw	$2,20($sp)
$L290:
	lw	$3,96($sp)
	lw	$2,100($sp)
	addu	$3,$3,$2
	lw	$2,48($sp)
	slt	$2,$3,$2
	beq	$2,$0,$L291
	nop

	j	$L292
	nop

$L291:
	lw	$2,104($sp)
	li	$3,1			# 0x1
	sw	$3,0($2)
$L292:
	lw	$3,96($sp)
	lw	$2,20($sp)
	subu	$2,$3,$2
	move	$3,$2
	lw	$2,88($sp)
	addu	$3,$2,$3
	lw	$2,92($sp)
	sw	$3,0($2)
	lw	$3,20($sp)
	lw	$2,96($sp)
	subu	$2,$3,$2
	lw	$3,16($sp)
	addu	$2,$3,$2
	sw	$2,16($sp)
	lw	$2,16($sp)
	bgez	$2,$L293
	nop

	sw	$0,16($sp)
$L293:
	lw	$3,16($sp)
	lw	$2,100($sp)
	slt	$2,$2,$3
	beq	$2,$0,$L294
	nop

	lw	$2,100($sp)
	sw	$2,16($sp)
$L294:
	lw	$2,16($sp)
	lw	$31,84($sp)
	addiu	$sp,$sp,88
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	phy_read_proc
	.size	phy_read_proc, .-phy_read_proc
	.globl	xpon_phy_print_open
	.section	.bss
	.align	2
	.type	xpon_phy_print_open, @object
	.size	xpon_phy_print_open, 4
xpon_phy_print_open:
	.space	4
	.rdata
	.align	2
$LC96:
	.ascii	"%s %x %x %x\000"
	.align	2
$LC97:
	.ascii	"%s %s %x\000"
	.align	2
$LC98:
	.ascii	"Phy_Int_Config\000"
	.align	2
$LC99:
	.ascii	"show\000"
	.align	2
$LC100:
	.ascii	"xpon_print\000"
	.align	2
$LC101:
	.ascii	"Phy_Mode_Config\000"
	.align	2
$LC102:
	.ascii	"isPhyReady\000"
	.align	2
$LC103:
	.ascii	"isPhyLOS\000"
	.align	2
$LC104:
	.ascii	"isPhyLOF\000"
	.align	2
$LC105:
	.ascii	"Phy_Reset_Counter\000"
	.align	2
$LC106:
	.ascii	"Phy_ErrCnt_Enable\000"
	.align	2
$LC107:
	.ascii	"Phy_Bit_Delay\000"
	.align	2
$LC108:
	.ascii	"Phy_TX_FEC_Status\000"
	.align	2
$LC109:
	.ascii	"Phy_Gpon_Delimiter_Guard\000"
	.align	2
$LC110:
	.ascii	"Phy_Tx_Burst_Config\000"
	.align	2
$LC111:
	.ascii	"Phy_Rogue_PRBS\000"
	.align	2
$LC112:
	.ascii	"Phy_Tx_Amp_Setting\000"
	.align	2
$LC113:
	.ascii	"Phy_Trans_Tx_Setting\000"
	.align	2
$LC114:
	.ascii	"Phy_Bip_Counter\000"
	.align	2
$LC115:
	.ascii	"Phy_RX_FEC_Setting\000"
	.align	2
$LC116:
	.ascii	"Phy_RX_FEC_Status\000"
	.align	2
$LC117:
	.ascii	"Phy_Trans_RX_Setting\000"
	.align	2
$LC118:
	.ascii	"Phy_Gpon_Preamble\000"
	.align	2
$LC119:
	.ascii	"Phy_Gpon_Extend_Preamble\000"
	.align	2
$LC120:
	.ascii	"Phy_Fec_Counter\000"
	.align	2
$LC121:
	.ascii	"Phy_Frame_Counter\000"
	.align	2
$LC122:
	.ascii	"Phy_Trans_Alarm\000"
	.align	2
$LC123:
	.ascii	"Phy_Tx_Frame_Counter\000"
	.align	2
$LC124:
	.ascii	"just test \012\000"
	.align	2
$LC125:
	.ascii	"Phy_Tx_Burst_Counter\000"
	.align	2
$LC126:
	.ascii	"Phy_Epon_Frame_Err_Cnt\000"
	.align	2
$LC127:
	.ascii	"Phy_Fw_Ready\000"
	.align	2
$LC128:
	.ascii	"Phy_Freq_Meter\000"
	.align	2
$LC129:
	.ascii	"Phy_Byte_Delay_Manual\000"
	.align	2
$LC130:
	.ascii	"Phy_Tx_Fec_Manual\000"
	.align	2
$LC131:
	.ascii	"eye_monitor\000"
	.align	2
$LC132:
	.ascii	"calibration\000"
	.align	2
$LC133:
	.ascii	"PBUS_test\000"
	.align	2
$LC134:
	.ascii	"power_on\000"
	.align	2
$LC135:
	.ascii	"ECNT_HOOK_ERROR occur with event id:%d. %s:%d\012\000"
	.align	2
$LC136:
	.ascii	"Phy_Tx_Test_Pattern\000"
	.align	2
$LC137:
	.ascii	"Phy_Rx_Sens_Test\000"
	.align	2
$LC138:
	.ascii	"7570_voltage\000"
	.align	2
$LC139:
	.ascii	"7570_current\000"
	.align	2
$LC140:
	.ascii	"7570_Vtemp_ADC\000"
	.align	2
$LC141:
	.ascii	"EnvTemp_cal\000"
	.align	2
$LC142:
	.ascii	"set_flash_register_Tx_data\000"
	.align	2
$LC143:
	.ascii	"LUT\000"
	.align	2
$LC144:
	.ascii	"7570_temperature\000"
	.align	2
$LC145:
	.ascii	"set_flash_register\000"
	.align	2
$LC146:
	.ascii	"save_flash_matrix\000"
	.align	2
$LC147:
	.ascii	"get_flash_matrix\000"
	.align	2
$LC148:
	.ascii	"get_flash_register\000"
	.align	2
$LC149:
	.ascii	"set_flash_register_default\000"
	.align	2
$LC150:
	.ascii	"mt7570_information_output\000"
	.align	2
$LC151:
	.ascii	"mt7570_register_dump\000"
	.align	2
$LC152:
	.ascii	"mt7570_MPD_current\000"
	.align	2
$LC153:
	.ascii	"mt7570_RSSI_current\000"
	.align	2
$LC154:
	.ascii	"dark_current\000"
	.align	2
$LC155:
	.ascii	"flash_dump\000"
	.align	2
$LC156:
	.ascii	"DOL_flash_dump\000"
	.align	2
$LC157:
	.ascii	"GPON_Tx_calibration\000"
	.align	2
$LC158:
	.ascii	"EPON_Tx_calibration\000"
	.align	2
$LC159:
	.ascii	"restart_ERC\000"
	.align	2
$LC160:
	.ascii	"LOS_calibration\000"
	.align	2
$LC161:
	.ascii	"set_flash_register_BiasModCurrent\000"
	.align	2
$LC162:
	.ascii	"set_flash_register_P0P1\000"
	.align	2
$LC163:
	.ascii	"set_flash_register_APD\000"
	.align	2
$LC164:
	.ascii	"set_flash_register_LOS\000"
	.align	2
$LC165:
	.ascii	"set_flash_register_DDMI_TxPower\000"
	.align	2
$LC166:
	.ascii	"set_flash_register_DDMI_RxPower\000"
	.align	2
$LC167:
	.ascii	"DDMI_check_8472\000"
	.align	2
$LC168:
	.ascii	"set_flash_register_TIAGAIN\000"
	.align	2
$LC169:
	.ascii	"temperature_calibration\000"
	.align	2
$LC170:
	.ascii	"GPON_BER\000"
	.align	2
$LC171:
	.ascii	"EPON_BER\000"
	.align	2
$LC172:
	.ascii	"show_BoB_information\000"
	.align	2
$LC173:
	.ascii	"GPON_pattern\000"
	.align	2
$LC174:
	.ascii	"EPON_pattern\000"
	.align	2
$LC175:
	.ascii	"dis_pattern\000"
	.align	2
$LC176:
	.ascii	"pon_phy_status\000"
	.align	2
$LC177:
	.ascii	"open_loop\000"
	.align	2
$LC178:
	.ascii	"closed_loop\000"
	.align	2
$LC179:
	.ascii	"sg_closed_loop\000"
	.align	2
$LC180:
	.ascii	"pon_register\000"
	.align	2
$LC181:
	.ascii	"Ibias_temperature_set\000"
	.align	2
$LC182:
	.ascii	"restart_ERC_P0\000"
	.align	2
$LC183:
	.ascii	"ADC_calibration\000"
	.align	2
$LC184:
	.ascii	"dis_ddmi\000"
	.align	2
$LC185:
	.ascii	"DDMI disabled\012\000"
	.align	2
$LC186:
	.ascii	"rebuild_LUT\000"
	.align	2
$LC187:
	.ascii	"msg\000"
	.align	2
$LC188:
	.ascii	"act\000"
	.align	2
$LC189:
	.ascii	"int\000"
	.align	2
$LC190:
	.ascii	"trace\000"
	.align	2
$LC191:
	.ascii	"debug\000"
	.align	2
$LC192:
	.ascii	"err\000"
	.align	2
$LC193:
	.ascii	"PHY Debug Level: %x\012\000"
	.align	2
$LC194:
	.ascii	"phy_i2c_div_clock\000"
	.align	2
$LC195:
	.ascii	"Usage\000"
	.align	2
$LC196:
	.ascii	"IO_GTransREG (device_addr) (sub_addr) \012\000"
	.align	2
$LC197:
	.ascii	"Phy_Int_Config (interrupt) \012\000"
	.align	2
$LC198:
	.ascii	"Phy_Mode_Config (XPON_mode) \012\000"
	.align	2
$LC199:
	.ascii	"isPhyReady \012\000"
	.align	2
$LC200:
	.ascii	"isPhyLOS \012\000"
	.align	2
$LC201:
	.ascii	"isPhyLOF \012\000"
	.align	2
$LC202:
	.ascii	"Phy_Reset_Counter \012\000"
	.align	2
$LC203:
	.ascii	"Phy_ErrCnt_Enable (enable) (enable) (enable)\012\000"
	.align	2
$LC204:
	.ascii	"Phy_Bit_Delay (delay_value) \012\000"
	.align	2
$LC205:
	.ascii	"Phy_TX_FEC_Status \012\000"
	.align	2
$LC206:
	.ascii	"Phy_Gpon_Delimiter_Guard (delimiter_pat) (guard_time_pat"
	.ascii	") \012\000"
	.align	2
$LC207:
	.ascii	"Phy_Tx_Burst_Config (burst_mode) \012\000"
	.align	2
$LC208:
	.ascii	"Phy_Rogue_PRBS (rogue_mode) \012\000"
	.align	2
$LC209:
	.ascii	"Phy_Tx_Amp_Setting (amp_level) \012\000"
	.align	2
$LC210:
	.ascii	"Phy_Trans_Tx_Setting (enable) (enable) (enable)\012\000"
	.align	2
$LC211:
	.ascii	"Phy_Bip_Counter \012\000"
	.align	2
$LC212:
	.ascii	"Phy_RX_FEC_Setting (fec_control) \012\000"
	.align	2
$LC213:
	.ascii	"Phy_RX_FEC_Status \012\000"
	.align	2
$LC214:
	.ascii	"Phy_Trans_RX_Setting (enable) \012\000"
	.align	2
$LC215:
	.ascii	"Trans_Param_Status \012\000"
	.align	2
$LC216:
	.ascii	"Phy_Gpon_Preamble (maks) \012\000"
	.align	2
$LC217:
	.ascii	"Phy_Gpon_Extend_Preamble (mask)\012\000"
	.align	2
$LC218:
	.ascii	"Phy_Fec_Counter \012\000"
	.align	2
$LC219:
	.ascii	"Phy_Frame_Counter \012\000"
	.align	2
$LC220:
	.ascii	"Phy_Optical_LOS_Time \012\000"
	.align	2
$LC221:
	.ascii	"Phy_Trans_Alarm \012\000"
	.align	2
$LC222:
	.ascii	"Phy_Mode_Select \012\000"
	.align	2
$LC223:
	.ascii	"Phy_Init_Polling \012\000"
	.align	2
$LC224:
	.ascii	"Phy_Tx_Test_Pattern (pattern) \012\000"
	.align	2
$LC225:
	.ascii	"msg (act|int|trace|debug|err) (enable) \012\000"
	.align	2
$LC226:
	.ascii	"Phy_Tx_Frame_Counter \012\000"
	.align	2
$LC227:
	.ascii	"Phy_Tx_Burst_Counter \012\000"
	.align	2
$LC228:
	.ascii	"Phy_Epon_Frame_Err_Cnt \012\000"
	.align	2
$LC229:
	.ascii	"Phy_Fw_Ready (fw_rdy_en) \012\000"
	.align	2
$LC230:
	.ascii	"Phy_Freq_Meter (freq_en) (freq_sele) \012\000"
	.align	2
$LC231:
	.ascii	"Phy_Byte_Delay_Manual (byte_dly_en) (byte_dly_value) \012"
	.ascii	"\000"
	.align	2
$LC232:
	.ascii	"Phy_Tx_Fec_Manual (tx_fec_en) \012\000"
	.align	2
$LC233:
	.ascii	"eye_monitor \012\000"
	.align	2
$LC234:
	.ascii	"calibration \012\000"
	.align	2
$LC235:
	.ascii	"PBUS_test \012\000"
	.align	2
$LC236:
	.ascii	"phy_i2c_div_clock \012\000"
	.section	.text.phy_write_proc,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	phy_write_proc
	.type	phy_write_proc, @function
phy_write_proc:
	.frame	$sp,1544,$31		# vars= 1512, regs= 1/0, args= 24, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	addiu	$sp,$sp,-1544
	sw	$31,1540($sp)
	sw	$4,1544($sp)
	sw	$5,1548($sp)
	sw	$6,1552($sp)
	sw	$7,1556($sp)
	sw	$0,1412($sp)
	sw	$0,28($sp)
	addiu	$2,$sp,1216
	move	$4,$2
	move	$5,$0
	li	$6,64			# 0x40
	lui	$2,%hi(memset)
	addiu	$2,$2,%lo(memset)
	jalr	$2
	addiu	$2,$sp,1280
	move	$4,$2
	move	$5,$0
	li	$6,64			# 0x40
	lui	$2,%hi(memset)
	addiu	$2,$2,%lo(memset)
	jalr	$2
	addiu	$2,$sp,1344
	move	$4,$2
	move	$5,$0
	li	$6,64			# 0x40
	lui	$2,%hi(memset)
	addiu	$2,$2,%lo(memset)
	jalr	$2
	lw	$2,1552($sp)
	sltu	$2,$2,64
	bne	$2,$0,$L297
	li	$2,-22			# 0xffffffffffffffea
	j	$L503
$L297:
	addiu	$2,$sp,1216
	sw	$2,32($sp)
	lw	$2,1548($sp)
	sw	$2,36($sp)
	lw	$2,1552($sp)
	sw	$2,24($sp)
	lw	$2,36($sp)
	sw	$2,40($sp)
	lw	$2,24($sp)
	sw	$2,44($sp)
	move	$2,$28
	lw	$2,24($2)
	sw	$2,48($sp)
	lw	$3,40($sp)
	lw	$2,44($sp)
	addu	$3,$3,$2
	lw	$2,40($sp)
	or	$3,$3,$2
	lw	$2,44($sp)
	or	$3,$3,$2
	lw	$2,48($sp)
	and	$2,$3,$2
	sw	$2,52($sp)
	lw	$2,52($sp)
	sltu	$2,$2,1
	andi	$2,$2,0x00ff
	sltu	$2,$0,$2
	andi	$2,$2,0x00ff
	beq	$2,$0,$L299
	lw	$4,32($sp)
	lw	$5,36($sp)
	lw	$6,24($sp)
#APP
 # 1703 "/opt/tclinux_phoenix/modules/private/xpon_phy/src/phy.c" 1
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
	sw	$2,24($sp)
$L299:
	lw	$2,24($sp)
	beq	$2,$0,$L300
	li	$2,-14			# 0xfffffffffffffff2
	j	$L503
$L300:
	addiu	$4,$sp,1216
	addiu	$6,$sp,1280
	addiu	$3,$sp,1408
	addiu	$2,$sp,1412
	sw	$2,16($sp)
	addiu	$2,$sp,1416
	sw	$2,20($sp)
	lui	$2,%hi($LC96)
	addiu	$5,$2,%lo($LC96)
	move	$7,$3
	lui	$2,%hi(sscanf)
	addiu	$2,$2,%lo(sscanf)
	jalr	$2
	addiu	$4,$sp,1216
	addiu	$6,$sp,1280
	addiu	$3,$sp,1344
	addiu	$2,$sp,1420
	sw	$2,16($sp)
	lui	$2,%hi($LC97)
	addiu	$5,$2,%lo($LC97)
	move	$7,$3
	lui	$2,%hi(sscanf)
	addiu	$2,$2,%lo(sscanf)
	jalr	$2
	addiu	$2,$sp,1280
	sw	$2,80($sp)
	lui	$2,%hi($LC98)
	addiu	$2,$2,%lo($LC98)
	sw	$2,84($sp)
	lw	$3,80($sp)
	lw	$2,84($sp)
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
	sw	$3,80($sp)
	sw	$2,84($sp)
	sw	$4,88($sp)
	lw	$2,88($sp)
	bne	$2,$0,$L302
	lw	$2,1408($sp)
	move	$4,$2
	lui	$2,%hi(phy_int_config)
	addiu	$2,$2,%lo(phy_int_config)
	jalr	$2
	j	$L303
$L302:
	addiu	$2,$sp,1280
	sw	$2,92($sp)
	lui	$2,%hi($LC99)
	addiu	$2,$2,%lo($LC99)
	sw	$2,96($sp)
	lw	$3,92($sp)
	lw	$2,96($sp)
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
	sw	$3,92($sp)
	sw	$2,96($sp)
	sw	$4,100($sp)
	lw	$2,100($sp)
	bne	$2,$0,$L305
	addiu	$2,$sp,1344
	sw	$2,104($sp)
	lui	$2,%hi($LC100)
	addiu	$2,$2,%lo($LC100)
	sw	$2,108($sp)
	lw	$3,104($sp)
	lw	$2,108($sp)
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
	sw	$3,104($sp)
	sw	$2,108($sp)
	sw	$4,112($sp)
	lw	$2,112($sp)
	bne	$2,$0,$L303
	lw	$2,1420($sp)
	move	$3,$2
	lui	$2,%hi(xpon_phy_print_open)
	sw	$3,%lo(xpon_phy_print_open)($2)
	j	$L303
$L305:
	addiu	$2,$sp,1280
	sw	$2,116($sp)
	lui	$2,%hi($LC101)
	addiu	$2,$2,%lo($LC101)
	sw	$2,120($sp)
	lw	$3,116($sp)
	lw	$2,120($sp)
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
	sw	$3,116($sp)
	sw	$2,120($sp)
	sw	$4,124($sp)
	lw	$2,124($sp)
	bne	$2,$0,$L309
	lw	$2,1408($sp)
	move	$4,$2
	li	$5,1			# 0x1
	lui	$2,%hi(phy_mode_config)
	addiu	$2,$2,%lo(phy_mode_config)
	jalr	$2
	j	$L303
$L309:
	addiu	$2,$sp,1280
	sw	$2,128($sp)
	lui	$2,%hi($LC102)
	addiu	$2,$2,%lo($LC102)
	sw	$2,132($sp)
	lw	$3,128($sp)
	lw	$2,132($sp)
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
	sw	$3,128($sp)
	sw	$2,132($sp)
	sw	$4,136($sp)
	lw	$2,136($sp)
	bne	$2,$0,$L311
	lui	$2,%hi(phy_ready_status)
	addiu	$2,$2,%lo(phy_ready_status)
	jalr	$2
	j	$L303
$L311:
	addiu	$2,$sp,1280
	sw	$2,140($sp)
	lui	$2,%hi($LC103)
	addiu	$2,$2,%lo($LC103)
	sw	$2,144($sp)
	lw	$3,140($sp)
	lw	$2,144($sp)
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
	sw	$3,140($sp)
	sw	$2,144($sp)
	sw	$4,148($sp)
	lw	$2,148($sp)
	bne	$2,$0,$L313
	lui	$2,%hi(phy_los_status)
	addiu	$2,$2,%lo(phy_los_status)
	jalr	$2
	j	$L303
$L313:
	addiu	$2,$sp,1280
	sw	$2,152($sp)
	lui	$2,%hi($LC104)
	addiu	$2,$2,%lo($LC104)
	sw	$2,156($sp)
	lw	$3,152($sp)
	lw	$2,156($sp)
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
	sw	$3,152($sp)
	sw	$2,156($sp)
	sw	$4,160($sp)
	lw	$2,160($sp)
	bne	$2,$0,$L315
	lui	$2,%hi(phy_lof_status)
	addiu	$2,$2,%lo(phy_lof_status)
	jalr	$2
	j	$L303
$L315:
	addiu	$2,$sp,1280
	sw	$2,164($sp)
	lui	$2,%hi($LC105)
	addiu	$2,$2,%lo($LC105)
	sw	$2,168($sp)
	lw	$3,164($sp)
	lw	$2,168($sp)
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
	sw	$3,164($sp)
	sw	$2,168($sp)
	sw	$4,172($sp)
	lw	$2,172($sp)
	bne	$2,$0,$L317
	lui	$2,%hi(phy_reset_counter)
	addiu	$2,$2,%lo(phy_reset_counter)
	jalr	$2
	j	$L303
$L317:
	addiu	$2,$sp,1280
	sw	$2,176($sp)
	lui	$2,%hi($LC106)
	addiu	$2,$2,%lo($LC106)
	sw	$2,180($sp)
	lw	$3,176($sp)
	lw	$2,180($sp)
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
	sw	$3,176($sp)
	sw	$2,180($sp)
	sw	$4,184($sp)
	lw	$2,184($sp)
	bne	$2,$0,$L319
	lw	$2,1408($sp)
	andi	$2,$2,0x00ff
	move	$4,$2
	lw	$2,1412($sp)
	andi	$2,$2,0x00ff
	move	$3,$2
	lw	$2,1416($sp)
	andi	$2,$2,0x00ff
	move	$5,$3
	move	$6,$2
	lui	$2,%hi(phy_cnt_enable)
	addiu	$2,$2,%lo(phy_cnt_enable)
	jalr	$2
	j	$L303
$L319:
	addiu	$2,$sp,1280
	sw	$2,188($sp)
	lui	$2,%hi($LC107)
	addiu	$2,$2,%lo($LC107)
	sw	$2,192($sp)
	lw	$3,188($sp)
	lw	$2,192($sp)
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
	sw	$3,188($sp)
	sw	$2,192($sp)
	sw	$4,196($sp)
	lw	$2,196($sp)
	bne	$2,$0,$L321
	lw	$2,1408($sp)
	andi	$2,$2,0x00ff
	move	$4,$2
	lui	$2,%hi(phy_bit_delay)
	addiu	$2,$2,%lo(phy_bit_delay)
	jalr	$2
	j	$L303
$L321:
	addiu	$2,$sp,1280
	sw	$2,200($sp)
	lui	$2,%hi($LC108)
	addiu	$2,$2,%lo($LC108)
	sw	$2,204($sp)
	lw	$3,200($sp)
	lw	$2,204($sp)
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
	sw	$3,200($sp)
	sw	$2,204($sp)
	sw	$4,208($sp)
	lw	$2,208($sp)
	bne	$2,$0,$L323
	lui	$2,%hi(phy_tx_fec_status)
	addiu	$2,$2,%lo(phy_tx_fec_status)
	jalr	$2
	j	$L303
$L323:
	addiu	$2,$sp,1280
	sw	$2,212($sp)
	lui	$2,%hi($LC109)
	addiu	$2,$2,%lo($LC109)
	sw	$2,216($sp)
	lw	$3,212($sp)
	lw	$2,216($sp)
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
	sw	$3,212($sp)
	sw	$2,216($sp)
	sw	$4,220($sp)
	lw	$2,220($sp)
	bne	$2,$0,$L325
	lw	$2,1408($sp)
	lw	$3,1412($sp)
	andi	$3,$3,0x00ff
	move	$4,$2
	move	$5,$3
	lui	$2,%hi(phy_gpon_delimiter_guard)
	addiu	$2,$2,%lo(phy_gpon_delimiter_guard)
	jalr	$2
	j	$L303
$L325:
	addiu	$2,$sp,1280
	sw	$2,224($sp)
	lui	$2,%hi($LC110)
	addiu	$2,$2,%lo($LC110)
	sw	$2,228($sp)
	lw	$3,224($sp)
	lw	$2,228($sp)
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
	sw	$3,224($sp)
	sw	$2,228($sp)
	sw	$4,232($sp)
	lw	$2,232($sp)
	bne	$2,$0,$L327
	lw	$2,1408($sp)
	andi	$2,$2,0x00ff
	move	$4,$2
	lui	$2,%hi(phy_tx_burst_config)
	addiu	$2,$2,%lo(phy_tx_burst_config)
	jalr	$2
	j	$L303
$L327:
	addiu	$2,$sp,1280
	sw	$2,236($sp)
	lui	$2,%hi($LC111)
	addiu	$2,$2,%lo($LC111)
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
	bne	$2,$0,$L329
	lw	$2,1408($sp)
	andi	$2,$2,0x00ff
	move	$4,$2
	lui	$2,%hi(phy_rogue_prbs_config)
	addiu	$2,$2,%lo(phy_rogue_prbs_config)
	jalr	$2
	j	$L303
$L329:
	addiu	$2,$sp,1280
	sw	$2,248($sp)
	lui	$2,%hi($LC112)
	addiu	$2,$2,%lo($LC112)
	sw	$2,252($sp)
	lw	$3,248($sp)
	lw	$2,252($sp)
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
	sw	$3,248($sp)
	sw	$2,252($sp)
	sw	$4,256($sp)
	lw	$2,256($sp)
	bne	$2,$0,$L331
	lw	$2,1408($sp)
	andi	$2,$2,0x00ff
	move	$4,$2
	lui	$2,%hi(phy_tx_amp_setting)
	addiu	$2,$2,%lo(phy_tx_amp_setting)
	jalr	$2
	j	$L303
$L331:
	addiu	$2,$sp,1280
	sw	$2,260($sp)
	lui	$2,%hi($LC113)
	addiu	$2,$2,%lo($LC113)
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
	bne	$2,$0,$L333
	addiu	$2,$sp,1464
	sw	$2,56($sp)
	lw	$2,1408($sp)
	andi	$3,$2,0x00ff
	lw	$2,56($sp)
	sb	$3,0($2)
	lw	$2,1412($sp)
	andi	$3,$2,0x00ff
	lw	$2,56($sp)
	sb	$3,1($2)
	lw	$2,1416($sp)
	andi	$3,$2,0x00ff
	lw	$2,56($sp)
	sb	$3,2($2)
	lw	$4,56($sp)
	lui	$2,%hi(phy_trans_tx_setting)
	addiu	$2,$2,%lo(phy_trans_tx_setting)
	jalr	$2
	j	$L303
$L333:
	addiu	$2,$sp,1280
	sw	$2,272($sp)
	lui	$2,%hi($LC114)
	addiu	$2,$2,%lo($LC114)
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
	bne	$2,$0,$L335
	lui	$2,%hi(phy_bip_counter)
	addiu	$2,$2,%lo(phy_bip_counter)
	jalr	$2
	j	$L303
$L335:
	addiu	$2,$sp,1280
	sw	$2,284($sp)
	lui	$2,%hi($LC115)
	addiu	$2,$2,%lo($LC115)
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
	bne	$2,$0,$L337
	lw	$2,1408($sp)
	andi	$2,$2,0x00ff
	move	$4,$2
	lui	$2,%hi(phy_rx_fec_setting)
	addiu	$2,$2,%lo(phy_rx_fec_setting)
	jalr	$2
	j	$L303
$L337:
	addiu	$2,$sp,1280
	sw	$2,296($sp)
	lui	$2,%hi($LC116)
	addiu	$2,$2,%lo($LC116)
	sw	$2,300($sp)
	lw	$3,296($sp)
	lw	$2,300($sp)
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
	sw	$3,296($sp)
	sw	$2,300($sp)
	sw	$4,304($sp)
	lw	$2,304($sp)
	bne	$2,$0,$L339
	lui	$2,%hi(phy_rx_fec_status)
	addiu	$2,$2,%lo(phy_rx_fec_status)
	jalr	$2
	j	$L303
$L339:
	addiu	$2,$sp,1280
	sw	$2,308($sp)
	lui	$2,%hi($LC117)
	addiu	$2,$2,%lo($LC117)
	sw	$2,312($sp)
	lw	$3,308($sp)
	lw	$2,312($sp)
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
	sw	$3,308($sp)
	sw	$2,312($sp)
	sw	$4,316($sp)
	lw	$2,316($sp)
	bne	$2,$0,$L341
	lw	$2,1408($sp)
	andi	$2,$2,0x00ff
	move	$4,$2
	lui	$2,%hi(phy_trans_rx_setting)
	addiu	$2,$2,%lo(phy_trans_rx_setting)
	jalr	$2
	j	$L303
$L341:
	addiu	$2,$sp,1280
	sw	$2,320($sp)
	lui	$2,%hi($LC118)
	addiu	$2,$2,%lo($LC118)
	sw	$2,324($sp)
	lw	$3,320($sp)
	lw	$2,324($sp)
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
	sw	$3,320($sp)
	sw	$2,324($sp)
	sw	$4,328($sp)
	lw	$2,328($sp)
	bne	$2,$0,$L343
	addiu	$2,$sp,1468
	sw	$2,60($sp)
	lw	$3,1408($sp)
	lw	$2,60($sp)
	sw	$3,12($2)
	lw	$2,60($sp)
	li	$3,16			# 0x10
	sb	$3,0($2)
	lw	$2,60($sp)
	li	$3,17			# 0x11
	sb	$3,1($2)
	lw	$2,60($sp)
	li	$3,18			# 0x12
	sb	$3,2($2)
	lw	$2,60($sp)
	li	$3,19			# 0x13
	sb	$3,3($2)
	lw	$4,60($sp)
	lui	$2,%hi(phy_gpon_preamble)
	addiu	$2,$2,%lo(phy_gpon_preamble)
	jalr	$2
	j	$L303
$L343:
	addiu	$2,$sp,1280
	sw	$2,332($sp)
	lui	$2,%hi($LC119)
	addiu	$2,$2,%lo($LC119)
	sw	$2,336($sp)
	lw	$3,332($sp)
	lw	$2,336($sp)
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
	sw	$3,332($sp)
	sw	$2,336($sp)
	sw	$4,340($sp)
	lw	$2,340($sp)
	bne	$2,$0,$L345
	addiu	$2,$sp,1484
	sw	$2,64($sp)
	lw	$3,1408($sp)
	lw	$2,64($sp)
	sw	$3,12($2)
	lw	$2,64($sp)
	li	$3,20			# 0x14
	sb	$3,4($2)
	lw	$2,64($sp)
	li	$3,21			# 0x15
	sb	$3,5($2)
	lw	$2,64($sp)
	li	$3,1			# 0x1
	sb	$3,6($2)
	lw	$2,64($sp)
	li	$3,1			# 0x1
	sb	$3,7($2)
	lw	$2,64($sp)
	li	$3,1			# 0x1
	sb	$3,8($2)
	lw	$4,64($sp)
	lui	$2,%hi(phy_gpon_extend_preamble)
	addiu	$2,$2,%lo(phy_gpon_extend_preamble)
	jalr	$2
	j	$L303
$L345:
	addiu	$2,$sp,1280
	sw	$2,344($sp)
	lui	$2,%hi($LC120)
	addiu	$2,$2,%lo($LC120)
	sw	$2,348($sp)
	lw	$3,344($sp)
	lw	$2,348($sp)
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
	sw	$3,344($sp)
	sw	$2,348($sp)
	sw	$4,352($sp)
	lw	$2,352($sp)
	bne	$2,$0,$L347
	addiu	$2,$sp,1500
	sw	$2,68($sp)
	lw	$4,68($sp)
	lui	$2,%hi(phy_rx_fec_counter)
	addiu	$2,$2,%lo(phy_rx_fec_counter)
	jalr	$2
	j	$L303
$L347:
	addiu	$2,$sp,1280
	sw	$2,356($sp)
	lui	$2,%hi($LC121)
	addiu	$2,$2,%lo($LC121)
	sw	$2,360($sp)
	lw	$3,356($sp)
	lw	$2,360($sp)
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
	sw	$3,356($sp)
	sw	$2,360($sp)
	sw	$4,364($sp)
	lw	$2,364($sp)
	bne	$2,$0,$L349
	addiu	$2,$sp,1520
	sw	$2,72($sp)
	lw	$4,72($sp)
	lui	$2,%hi(phy_rx_frame_counter)
	addiu	$2,$2,%lo(phy_rx_frame_counter)
	jalr	$2
	j	$L303
$L349:
	addiu	$2,$sp,1280
	sw	$2,368($sp)
	lui	$2,%hi($LC122)
	addiu	$2,$2,%lo($LC122)
	sw	$2,372($sp)
	lw	$3,368($sp)
	lw	$2,372($sp)
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
	sw	$3,368($sp)
	sw	$2,372($sp)
	sw	$4,376($sp)
	lw	$2,376($sp)
	bne	$2,$0,$L351
	lui	$2,%hi(phy_rx_power_alarm)
	addiu	$2,$2,%lo(phy_rx_power_alarm)
	jalr	$2
	lui	$2,%hi(phy_tx_alarm)
	addiu	$2,$2,%lo(phy_tx_alarm)
	jalr	$2
	j	$L303
$L351:
	addiu	$2,$sp,1280
	sw	$2,380($sp)
	lui	$2,%hi($LC123)
	addiu	$2,$2,%lo($LC123)
	sw	$2,384($sp)
	lw	$3,380($sp)
	lw	$2,384($sp)
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
	sw	$3,380($sp)
	sw	$2,384($sp)
	sw	$4,388($sp)
	lw	$2,388($sp)
	bne	$2,$0,$L353
	lui	$2,%hi(phy_tx_frame_counter)
	addiu	$2,$2,%lo(phy_tx_frame_counter)
	jalr	$2
	lui	$2,%hi($LC124)
	addiu	$4,$2,%lo($LC124)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	j	$L303
$L353:
	addiu	$2,$sp,1280
	sw	$2,392($sp)
	lui	$2,%hi($LC125)
	addiu	$2,$2,%lo($LC125)
	sw	$2,396($sp)
	lw	$3,392($sp)
	lw	$2,396($sp)
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
	sw	$3,392($sp)
	sw	$2,396($sp)
	sw	$4,400($sp)
	lw	$2,400($sp)
	bne	$2,$0,$L355
	lui	$2,%hi(phy_tx_burst_counter)
	addiu	$2,$2,%lo(phy_tx_burst_counter)
	jalr	$2
	lui	$2,%hi($LC124)
	addiu	$4,$2,%lo($LC124)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	j	$L303
$L355:
	addiu	$2,$sp,1280
	sw	$2,404($sp)
	lui	$2,%hi($LC126)
	addiu	$2,$2,%lo($LC126)
	sw	$2,408($sp)
	lw	$3,404($sp)
	lw	$2,408($sp)
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
	sw	$3,404($sp)
	sw	$2,408($sp)
	sw	$4,412($sp)
	lw	$2,412($sp)
	bne	$2,$0,$L357
	lui	$2,%hi(phy_epon_frame_err_cnt)
	addiu	$2,$2,%lo(phy_epon_frame_err_cnt)
	jalr	$2
	lui	$2,%hi($LC124)
	addiu	$4,$2,%lo($LC124)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	j	$L303
$L357:
	addiu	$2,$sp,1280
	sw	$2,416($sp)
	lui	$2,%hi($LC127)
	addiu	$2,$2,%lo($LC127)
	sw	$2,420($sp)
	lw	$3,416($sp)
	lw	$2,420($sp)
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
	sw	$3,416($sp)
	sw	$2,420($sp)
	sw	$4,424($sp)
	lw	$2,424($sp)
	bne	$2,$0,$L359
	lw	$2,1408($sp)
	andi	$2,$2,0x00ff
	move	$4,$2
	lui	$2,%hi(phy_fw_ready)
	addiu	$2,$2,%lo(phy_fw_ready)
	jalr	$2
	lui	$2,%hi($LC124)
	addiu	$4,$2,%lo($LC124)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	j	$L303
$L359:
	addiu	$2,$sp,1280
	sw	$2,428($sp)
	lui	$2,%hi($LC128)
	addiu	$2,$2,%lo($LC128)
	sw	$2,432($sp)
	lw	$3,428($sp)
	lw	$2,432($sp)
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
	sw	$3,428($sp)
	sw	$2,432($sp)
	sw	$4,436($sp)
	lw	$2,436($sp)
	bne	$2,$0,$L361
	lw	$2,1408($sp)
	andi	$2,$2,0x00ff
	move	$3,$2
	lw	$2,1412($sp)
	andi	$2,$2,0x00ff
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(phy_freq_meter)
	addiu	$2,$2,%lo(phy_freq_meter)
	jalr	$2
	j	$L303
$L361:
	addiu	$2,$sp,1280
	sw	$2,440($sp)
	lui	$2,%hi($LC129)
	addiu	$2,$2,%lo($LC129)
	sw	$2,444($sp)
	lw	$3,440($sp)
	lw	$2,444($sp)
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
	sw	$3,440($sp)
	sw	$2,444($sp)
	sw	$4,448($sp)
	lw	$2,448($sp)
	bne	$2,$0,$L363
	lw	$2,1408($sp)
	andi	$2,$2,0x00ff
	move	$3,$2
	lw	$2,1412($sp)
	andi	$2,$2,0x00ff
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(phy_byte_delay_manual)
	addiu	$2,$2,%lo(phy_byte_delay_manual)
	jalr	$2
	j	$L303
$L363:
	addiu	$2,$sp,1280
	sw	$2,452($sp)
	lui	$2,%hi($LC130)
	addiu	$2,$2,%lo($LC130)
	sw	$2,456($sp)
	lw	$3,452($sp)
	lw	$2,456($sp)
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
	sw	$3,452($sp)
	sw	$2,456($sp)
	sw	$4,460($sp)
	lw	$2,460($sp)
	bne	$2,$0,$L365
	lw	$2,1408($sp)
	andi	$2,$2,0x00ff
	move	$4,$2
	lui	$2,%hi(phy_tx_fec_manual)
	addiu	$2,$2,%lo(phy_tx_fec_manual)
	jalr	$2
	j	$L303
$L365:
	addiu	$2,$sp,1280
	sw	$2,464($sp)
	lui	$2,%hi($LC131)
	addiu	$2,$2,%lo($LC131)
	sw	$2,468($sp)
	lw	$3,464($sp)
	lw	$2,468($sp)
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
	sw	$3,464($sp)
	sw	$2,468($sp)
	sw	$4,472($sp)
	lw	$2,472($sp)
	bne	$2,$0,$L367
	lw	$2,1408($sp)
	andi	$2,$2,0xffff
	move	$4,$2
	lw	$2,1412($sp)
	andi	$2,$2,0x00ff
	move	$3,$2
	lw	$2,1416($sp)
	move	$5,$3
	move	$6,$2
	lui	$2,%hi(phy_eye_monitor)
	addiu	$2,$2,%lo(phy_eye_monitor)
	jalr	$2
	j	$L303
$L367:
	addiu	$2,$sp,1280
	sw	$2,476($sp)
	lui	$2,%hi($LC132)
	addiu	$2,$2,%lo($LC132)
	sw	$2,480($sp)
	lw	$3,476($sp)
	lw	$2,480($sp)
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
	sw	$3,476($sp)
	sw	$2,480($sp)
	sw	$4,484($sp)
	lw	$2,484($sp)
	bne	$2,$0,$L369
	lui	$2,%hi(phy_calibration_test)
	addiu	$2,$2,%lo(phy_calibration_test)
	jalr	$2
	j	$L303
$L369:
	addiu	$2,$sp,1280
	sw	$2,488($sp)
	lui	$2,%hi($LC133)
	addiu	$2,$2,%lo($LC133)
	sw	$2,492($sp)
	lw	$3,488($sp)
	lw	$2,492($sp)
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
	sw	$3,488($sp)
	sw	$2,492($sp)
	sw	$4,496($sp)
	lw	$2,496($sp)
	bne	$2,$0,$L371
	lui	$2,%hi(phy_pbus_stress_test)
	addiu	$2,$2,%lo(phy_pbus_stress_test)
	jalr	$2
	j	$L303
$L371:
	addiu	$2,$sp,1280
	sw	$2,500($sp)
	lui	$2,%hi($LC134)
	addiu	$2,$2,%lo($LC134)
	sw	$2,504($sp)
	lw	$3,500($sp)
	lw	$2,504($sp)
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
	sw	$3,500($sp)
	sw	$2,504($sp)
	sw	$4,508($sp)
	lw	$2,508($sp)
	bne	$2,$0,$L373
	lw	$2,1408($sp)
	bne	$2,$0,$L374
	li	$2,262			# 0x106
	sw	$2,512($sp)
	sw	$0,1444($sp)
	sw	$0,1448($sp)
	sw	$0,1452($sp)
	sw	$0,1456($sp)
	sw	$0,1460($sp)
	lw	$2,512($sp)
	sw	$2,1448($sp)
	sw	$0,1452($sp)
	addiu	$2,$sp,1444
	sw	$2,1456($sp)
	addiu	$2,$sp,1452
	li	$4,24			# 0x18
	move	$5,$0
	move	$6,$2
	lui	$2,%hi(__ECNT_HOOK)
	addiu	$2,$2,%lo(__ECNT_HOOK)
	jalr	$2
	move	$3,$2
	li	$2,-1			# 0xffffffffffffffff
	bne	$3,$2,$L376
	lui	$2,%hi($LC135)
	addiu	$4,$2,%lo($LC135)
	lw	$5,512($sp)
	lui	$2,%hi(__func__.24637)
	addiu	$6,$2,%lo(__func__.24637)
	li	$7,107			# 0x6b
	lui	$2,%hi(panic)
	addiu	$2,$2,%lo(panic)
	jalr	$2
$L374:
	lw	$3,1408($sp)
	li	$2,1			# 0x1
	bne	$3,$2,$L376
	li	$2,261			# 0x105
	sw	$2,516($sp)
	sw	$0,1424($sp)
	sw	$0,1428($sp)
	sw	$0,1432($sp)
	sw	$0,1436($sp)
	sw	$0,1440($sp)
	lw	$2,516($sp)
	sw	$2,1428($sp)
	sw	$0,1432($sp)
	addiu	$2,$sp,1424
	sw	$2,1436($sp)
	addiu	$2,$sp,1432
	li	$4,24			# 0x18
	move	$5,$0
	move	$6,$2
	lui	$2,%hi(__ECNT_HOOK)
	addiu	$2,$2,%lo(__ECNT_HOOK)
	jalr	$2
	move	$3,$2
	li	$2,-1			# 0xffffffffffffffff
	bne	$3,$2,$L376
	lui	$2,%hi($LC135)
	addiu	$4,$2,%lo($LC135)
	lw	$5,516($sp)
	lui	$2,%hi(__func__.24637)
	addiu	$6,$2,%lo(__func__.24637)
	li	$7,107			# 0x6b
	lui	$2,%hi(panic)
	addiu	$2,$2,%lo(panic)
	jalr	$2
$L376:
	lw	$2,1408($sp)
	andi	$2,$2,0x00ff
	move	$4,$2
	lui	$2,%hi(phy_tx_power_config)
	addiu	$2,$2,%lo(phy_tx_power_config)
	jalr	$2
	lw	$2,1408($sp)
	andi	$2,$2,0x00ff
	move	$4,$2
	lui	$2,%hi(phy_trans_power_switch)
	addiu	$2,$2,%lo(phy_trans_power_switch)
	jalr	$2
	j	$L303
$L373:
	addiu	$2,$sp,1280
	sw	$2,520($sp)
	lui	$2,%hi($LC136)
	addiu	$2,$2,%lo($LC136)
	sw	$2,524($sp)
	lw	$3,520($sp)
	lw	$2,524($sp)
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
	sw	$3,520($sp)
	sw	$2,524($sp)
	sw	$4,528($sp)
	lw	$2,528($sp)
	bne	$2,$0,$L379
	lw	$2,1408($sp)
	move	$4,$2
	lui	$2,%hi(phy_tx_test_pattern)
	addiu	$2,$2,%lo(phy_tx_test_pattern)
	jalr	$2
	j	$L303
$L379:
	addiu	$2,$sp,1280
	sw	$2,532($sp)
	lui	$2,%hi($LC137)
	addiu	$2,$2,%lo($LC137)
	sw	$2,536($sp)
	lw	$3,532($sp)
	lw	$2,536($sp)
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
	sw	$3,532($sp)
	sw	$2,536($sp)
	sw	$4,540($sp)
	lw	$2,540($sp)
	bne	$2,$0,$L381
	lui	$2,%hi(phy_rx_sensitivity_test)
	addiu	$2,$2,%lo(phy_rx_sensitivity_test)
	jalr	$2
	j	$L303
$L381:
	addiu	$2,$sp,1280
	sw	$2,544($sp)
	lui	$2,%hi($LC138)
	addiu	$2,$2,%lo($LC138)
	sw	$2,548($sp)
	lw	$3,544($sp)
	lw	$2,548($sp)
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
	sw	$3,544($sp)
	sw	$2,548($sp)
	sw	$4,552($sp)
	lw	$2,552($sp)
	bne	$2,$0,$L383
	lui	$2,%hi(mt7570_supply_voltage_get_8472)
	addiu	$2,$2,%lo(mt7570_supply_voltage_get_8472)
	jalr	$2
	j	$L303
$L383:
	addiu	$2,$sp,1280
	sw	$2,556($sp)
	lui	$2,%hi($LC139)
	addiu	$2,$2,%lo($LC139)
	sw	$2,560($sp)
	lw	$3,556($sp)
	lw	$2,560($sp)
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
	sw	$3,556($sp)
	sw	$2,560($sp)
	sw	$4,564($sp)
	lw	$2,564($sp)
	bne	$2,$0,$L385
	lui	$2,%hi(mt7570_bias_current_get_8472)
	addiu	$2,$2,%lo(mt7570_bias_current_get_8472)
	jalr	$2
	j	$L303
$L385:
	addiu	$2,$sp,1280
	sw	$2,568($sp)
	lui	$2,%hi($LC140)
	addiu	$2,$2,%lo($LC140)
	sw	$2,572($sp)
	lw	$3,568($sp)
	lw	$2,572($sp)
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
	sw	$3,568($sp)
	sw	$2,572($sp)
	sw	$4,576($sp)
	lw	$2,576($sp)
	bne	$2,$0,$L387
	lui	$2,%hi(mt7570_Vtemp_ADC_get)
	addiu	$2,$2,%lo(mt7570_Vtemp_ADC_get)
	jalr	$2
	j	$L303
$L387:
	addiu	$2,$sp,1280
	sw	$2,580($sp)
	lui	$2,%hi($LC141)
	addiu	$2,$2,%lo($LC141)
	sw	$2,584($sp)
	lw	$3,580($sp)
	lw	$2,584($sp)
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
	sw	$3,580($sp)
	sw	$2,584($sp)
	sw	$4,588($sp)
	lw	$2,588($sp)
	bne	$2,$0,$L389
	lw	$2,1408($sp)
	move	$4,$2
	lui	$2,%hi(set_flash_register_EnvTemp)
	addiu	$2,$2,%lo(set_flash_register_EnvTemp)
	jalr	$2
	j	$L303
$L389:
	addiu	$2,$sp,1280
	sw	$2,592($sp)
	lui	$2,%hi($LC142)
	addiu	$2,$2,%lo($LC142)
	sw	$2,596($sp)
	lw	$3,592($sp)
	lw	$2,596($sp)
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
	sw	$3,592($sp)
	sw	$2,596($sp)
	sw	$4,600($sp)
	lw	$2,600($sp)
	bne	$2,$0,$L391
	lui	$2,%hi(set_flash_register_Tx_data)
	addiu	$2,$2,%lo(set_flash_register_Tx_data)
	jalr	$2
	j	$L303
$L391:
	addiu	$2,$sp,1280
	sw	$2,604($sp)
	lui	$2,%hi($LC143)
	addiu	$2,$2,%lo($LC143)
	sw	$2,608($sp)
	lw	$3,604($sp)
	lw	$2,608($sp)
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
	sw	$3,604($sp)
	sw	$2,608($sp)
	sw	$4,612($sp)
	lw	$2,612($sp)
	bne	$2,$0,$L393
	lui	$2,%hi(mt7570_show_LUT)
	addiu	$2,$2,%lo(mt7570_show_LUT)
	jalr	$2
	j	$L303
$L393:
	addiu	$2,$sp,1280
	sw	$2,616($sp)
	lui	$2,%hi($LC144)
	addiu	$2,$2,%lo($LC144)
	sw	$2,620($sp)
	lw	$3,616($sp)
	lw	$2,620($sp)
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
	sw	$3,616($sp)
	sw	$2,620($sp)
	sw	$4,624($sp)
	lw	$2,624($sp)
	bne	$2,$0,$L395
	lui	$2,%hi(mt7570_temperature_get_8472)
	addiu	$2,$2,%lo(mt7570_temperature_get_8472)
	jalr	$2
	j	$L303
$L395:
	addiu	$2,$sp,1280
	sw	$2,628($sp)
	lui	$2,%hi($LC145)
	addiu	$2,$2,%lo($LC145)
	sw	$2,632($sp)
	lw	$3,628($sp)
	lw	$2,632($sp)
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
	sw	$3,628($sp)
	sw	$2,632($sp)
	sw	$4,636($sp)
	lw	$2,636($sp)
	bne	$2,$0,$L397
	lw	$3,1408($sp)
	lw	$2,1412($sp)
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(set_flash_register)
	addiu	$2,$2,%lo(set_flash_register)
	jalr	$2
	j	$L303
$L397:
	addiu	$2,$sp,1280
	sw	$2,640($sp)
	lui	$2,%hi($LC146)
	addiu	$2,$2,%lo($LC146)
	sw	$2,644($sp)
	lw	$3,640($sp)
	lw	$2,644($sp)
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
	sw	$3,640($sp)
	sw	$2,644($sp)
	sw	$4,648($sp)
	lw	$2,648($sp)
	bne	$2,$0,$L399
	lui	$2,%hi(save_flash_matrix)
	addiu	$2,$2,%lo(save_flash_matrix)
	jalr	$2
	j	$L303
$L399:
	addiu	$2,$sp,1280
	sw	$2,652($sp)
	lui	$2,%hi($LC147)
	addiu	$2,$2,%lo($LC147)
	sw	$2,656($sp)
	lw	$3,652($sp)
	lw	$2,656($sp)
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
	sw	$3,652($sp)
	sw	$2,656($sp)
	sw	$4,660($sp)
	lw	$2,660($sp)
	bne	$2,$0,$L401
	lui	$2,%hi(get_flash_matrix)
	addiu	$2,$2,%lo(get_flash_matrix)
	jalr	$2
	j	$L303
$L401:
	addiu	$2,$sp,1280
	sw	$2,664($sp)
	lui	$2,%hi($LC148)
	addiu	$2,$2,%lo($LC148)
	sw	$2,668($sp)
	lw	$3,664($sp)
	lw	$2,668($sp)
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
	sw	$3,664($sp)
	sw	$2,668($sp)
	sw	$4,672($sp)
	lw	$2,672($sp)
	bne	$2,$0,$L403
	lw	$2,1408($sp)
	move	$4,$2
	lui	$2,%hi(get_flash_register)
	addiu	$2,$2,%lo(get_flash_register)
	jalr	$2
	j	$L303
$L403:
	addiu	$2,$sp,1280
	sw	$2,676($sp)
	lui	$2,%hi($LC149)
	addiu	$2,$2,%lo($LC149)
	sw	$2,680($sp)
	lw	$3,676($sp)
	lw	$2,680($sp)
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
	sw	$3,676($sp)
	sw	$2,680($sp)
	sw	$4,684($sp)
	lw	$2,684($sp)
	bne	$2,$0,$L405
	lui	$2,%hi(set_flash_register_default)
	addiu	$2,$2,%lo(set_flash_register_default)
	jalr	$2
	j	$L303
$L405:
	addiu	$2,$sp,1280
	sw	$2,688($sp)
	lui	$2,%hi($LC150)
	addiu	$2,$2,%lo($LC150)
	sw	$2,692($sp)
	lw	$3,688($sp)
	lw	$2,692($sp)
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
	sw	$3,688($sp)
	sw	$2,692($sp)
	sw	$4,696($sp)
	lw	$2,696($sp)
	bne	$2,$0,$L407
	lw	$2,1408($sp)
	move	$4,$2
	lui	$2,%hi(mt7570_information_output)
	addiu	$2,$2,%lo(mt7570_information_output)
	jalr	$2
	j	$L303
$L407:
	addiu	$2,$sp,1280
	sw	$2,700($sp)
	lui	$2,%hi($LC151)
	addiu	$2,$2,%lo($LC151)
	sw	$2,704($sp)
	lw	$3,700($sp)
	lw	$2,704($sp)
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
	sw	$3,700($sp)
	sw	$2,704($sp)
	sw	$4,708($sp)
	lw	$2,708($sp)
	bne	$2,$0,$L409
	lw	$2,1408($sp)
	move	$4,$2
	lui	$2,%hi(mt7570_register_dump)
	addiu	$2,$2,%lo(mt7570_register_dump)
	jalr	$2
	j	$L303
$L409:
	addiu	$2,$sp,1280
	sw	$2,712($sp)
	lui	$2,%hi($LC152)
	addiu	$2,$2,%lo($LC152)
	sw	$2,716($sp)
	lw	$3,712($sp)
	lw	$2,716($sp)
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
	sw	$3,712($sp)
	sw	$2,716($sp)
	sw	$4,720($sp)
	lw	$2,720($sp)
	bne	$2,$0,$L411
	lui	$2,%hi(mt7570_MPD_current)
	addiu	$2,$2,%lo(mt7570_MPD_current)
	jalr	$2
	j	$L303
$L411:
	addiu	$2,$sp,1280
	sw	$2,724($sp)
	lui	$2,%hi($LC153)
	addiu	$2,$2,%lo($LC153)
	sw	$2,728($sp)
	lw	$3,724($sp)
	lw	$2,728($sp)
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
	sw	$3,724($sp)
	sw	$2,728($sp)
	sw	$4,732($sp)
	lw	$2,732($sp)
	bne	$2,$0,$L413
	lui	$2,%hi(mt7570_RSSI_current)
	addiu	$2,$2,%lo(mt7570_RSSI_current)
	jalr	$2
	j	$L303
$L413:
	addiu	$2,$sp,1280
	sw	$2,736($sp)
	lui	$2,%hi($LC154)
	addiu	$2,$2,%lo($LC154)
	sw	$2,740($sp)
	lw	$3,736($sp)
	lw	$2,740($sp)
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
	sw	$3,736($sp)
	sw	$2,740($sp)
	sw	$4,744($sp)
	lw	$2,744($sp)
	bne	$2,$0,$L415
	lui	$2,%hi(mt7570_dark_current)
	addiu	$2,$2,%lo(mt7570_dark_current)
	jalr	$2
	j	$L303
$L415:
	addiu	$2,$sp,1280
	sw	$2,748($sp)
	lui	$2,%hi($LC155)
	addiu	$2,$2,%lo($LC155)
	sw	$2,752($sp)
	lw	$3,748($sp)
	lw	$2,752($sp)
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
	sw	$3,748($sp)
	sw	$2,752($sp)
	sw	$4,756($sp)
	lw	$2,756($sp)
	bne	$2,$0,$L417
	lui	$2,%hi(flash_dump)
	addiu	$2,$2,%lo(flash_dump)
	jalr	$2
	j	$L303
$L417:
	addiu	$2,$sp,1280
	sw	$2,760($sp)
	lui	$2,%hi($LC156)
	addiu	$2,$2,%lo($LC156)
	sw	$2,764($sp)
	lw	$3,760($sp)
	lw	$2,764($sp)
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
	sw	$3,760($sp)
	sw	$2,764($sp)
	sw	$4,768($sp)
	lw	$2,768($sp)
	bne	$2,$0,$L419
	lui	$2,%hi(DOL_flash_dump)
	addiu	$2,$2,%lo(DOL_flash_dump)
	jalr	$2
	j	$L303
$L419:
	addiu	$2,$sp,1280
	sw	$2,772($sp)
	lui	$2,%hi($LC157)
	addiu	$2,$2,%lo($LC157)
	sw	$2,776($sp)
	lw	$3,772($sp)
	lw	$2,776($sp)
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
	sw	$3,772($sp)
	sw	$2,776($sp)
	sw	$4,780($sp)
	lw	$2,780($sp)
	bne	$2,$0,$L421
	lui	$2,%hi(GPON_Tx_calibration)
	addiu	$2,$2,%lo(GPON_Tx_calibration)
	jalr	$2
	j	$L303
$L421:
	addiu	$2,$sp,1280
	sw	$2,784($sp)
	lui	$2,%hi($LC158)
	addiu	$2,$2,%lo($LC158)
	sw	$2,788($sp)
	lw	$3,784($sp)
	lw	$2,788($sp)
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
	sw	$3,784($sp)
	sw	$2,788($sp)
	sw	$4,792($sp)
	lw	$2,792($sp)
	bne	$2,$0,$L423
	lui	$2,%hi(EPON_Tx_calibration)
	addiu	$2,$2,%lo(EPON_Tx_calibration)
	jalr	$2
	j	$L303
$L423:
	addiu	$2,$sp,1280
	sw	$2,796($sp)
	lui	$2,%hi($LC159)
	addiu	$2,$2,%lo($LC159)
	sw	$2,800($sp)
	lw	$3,796($sp)
	lw	$2,800($sp)
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
	sw	$3,796($sp)
	sw	$2,800($sp)
	sw	$4,804($sp)
	lw	$2,804($sp)
	bne	$2,$0,$L425
	lui	$2,%hi(mt7570_restart_ERC)
	addiu	$2,$2,%lo(mt7570_restart_ERC)
	jalr	$2
	j	$L303
$L425:
	addiu	$2,$sp,1280
	sw	$2,808($sp)
	lui	$2,%hi($LC160)
	addiu	$2,$2,%lo($LC160)
	sw	$2,812($sp)
	lw	$3,808($sp)
	lw	$2,812($sp)
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
	sw	$3,808($sp)
	sw	$2,812($sp)
	sw	$4,816($sp)
	lw	$2,816($sp)
	bne	$2,$0,$L427
	lw	$3,1408($sp)
	lw	$2,1412($sp)
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(LOS_calibration)
	addiu	$2,$2,%lo(LOS_calibration)
	jalr	$2
	j	$L303
$L427:
	addiu	$2,$sp,1280
	sw	$2,820($sp)
	lui	$2,%hi($LC161)
	addiu	$2,$2,%lo($LC161)
	sw	$2,824($sp)
	lw	$3,820($sp)
	lw	$2,824($sp)
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
	sw	$3,820($sp)
	sw	$2,824($sp)
	sw	$4,828($sp)
	lw	$2,828($sp)
	bne	$2,$0,$L429
	lui	$2,%hi(set_flash_register_BiasModCurrent)
	addiu	$2,$2,%lo(set_flash_register_BiasModCurrent)
	jalr	$2
	j	$L303
$L429:
	addiu	$2,$sp,1280
	sw	$2,832($sp)
	lui	$2,%hi($LC162)
	addiu	$2,$2,%lo($LC162)
	sw	$2,836($sp)
	lw	$3,832($sp)
	lw	$2,836($sp)
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
	sw	$3,832($sp)
	sw	$2,836($sp)
	sw	$4,840($sp)
	lw	$2,840($sp)
	bne	$2,$0,$L431
	lui	$2,%hi(set_flash_register_P0P1)
	addiu	$2,$2,%lo(set_flash_register_P0P1)
	jalr	$2
	j	$L303
$L431:
	addiu	$2,$sp,1280
	sw	$2,844($sp)
	lui	$2,%hi($LC163)
	addiu	$2,$2,%lo($LC163)
	sw	$2,848($sp)
	lw	$3,844($sp)
	lw	$2,848($sp)
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
	sw	$3,844($sp)
	sw	$2,848($sp)
	sw	$4,852($sp)
	lw	$2,852($sp)
	bne	$2,$0,$L433
	lw	$4,1408($sp)
	lw	$3,1412($sp)
	lw	$2,1416($sp)
	move	$5,$3
	move	$6,$2
	lui	$2,%hi(set_flash_register_APD)
	addiu	$2,$2,%lo(set_flash_register_APD)
	jalr	$2
	j	$L303
$L433:
	addiu	$2,$sp,1280
	sw	$2,856($sp)
	lui	$2,%hi($LC164)
	addiu	$2,$2,%lo($LC164)
	sw	$2,860($sp)
	lw	$3,856($sp)
	lw	$2,860($sp)
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
	sw	$3,856($sp)
	sw	$2,860($sp)
	sw	$4,864($sp)
	lw	$2,864($sp)
	bne	$2,$0,$L435
	lui	$2,%hi(set_flash_register_LOS)
	addiu	$2,$2,%lo(set_flash_register_LOS)
	jalr	$2
	j	$L303
$L435:
	addiu	$2,$sp,1280
	sw	$2,868($sp)
	lui	$2,%hi($LC165)
	addiu	$2,$2,%lo($LC165)
	sw	$2,872($sp)
	lw	$3,868($sp)
	lw	$2,872($sp)
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
	sw	$3,868($sp)
	sw	$2,872($sp)
	sw	$4,876($sp)
	lw	$2,876($sp)
	bne	$2,$0,$L437
	lw	$3,1408($sp)
	lw	$2,1412($sp)
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(set_flash_register_DDMI_TxPower)
	addiu	$2,$2,%lo(set_flash_register_DDMI_TxPower)
	jalr	$2
	j	$L303
$L437:
	addiu	$2,$sp,1280
	sw	$2,880($sp)
	lui	$2,%hi($LC166)
	addiu	$2,$2,%lo($LC166)
	sw	$2,884($sp)
	lw	$3,880($sp)
	lw	$2,884($sp)
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
	sw	$3,880($sp)
	sw	$2,884($sp)
	sw	$4,888($sp)
	lw	$2,888($sp)
	bne	$2,$0,$L439
	lw	$3,1408($sp)
	lw	$2,1412($sp)
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(set_flash_register_DDMI_RxPower)
	addiu	$2,$2,%lo(set_flash_register_DDMI_RxPower)
	jalr	$2
	j	$L303
$L439:
	addiu	$2,$sp,1280
	sw	$2,892($sp)
	lui	$2,%hi($LC167)
	addiu	$2,$2,%lo($LC167)
	sw	$2,896($sp)
	lw	$3,892($sp)
	lw	$2,896($sp)
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
	sw	$3,892($sp)
	sw	$2,896($sp)
	sw	$4,900($sp)
	lw	$2,900($sp)
	bne	$2,$0,$L441
	lui	$2,%hi(DDMI_check_8472)
	addiu	$2,$2,%lo(DDMI_check_8472)
	jalr	$2
	j	$L303
$L441:
	addiu	$2,$sp,1280
	sw	$2,904($sp)
	lui	$2,%hi($LC168)
	addiu	$2,$2,%lo($LC168)
	sw	$2,908($sp)
	lw	$3,904($sp)
	lw	$2,908($sp)
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
	sw	$3,904($sp)
	sw	$2,908($sp)
	sw	$4,912($sp)
	lw	$2,912($sp)
	bne	$2,$0,$L443
	lui	$2,%hi(set_flash_register_TIAGAIN)
	addiu	$2,$2,%lo(set_flash_register_TIAGAIN)
	jalr	$2
	j	$L303
$L443:
	addiu	$2,$sp,1280
	sw	$2,916($sp)
	lui	$2,%hi($LC169)
	addiu	$2,$2,%lo($LC169)
	sw	$2,920($sp)
	lw	$3,916($sp)
	lw	$2,920($sp)
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
	sw	$3,916($sp)
	sw	$2,920($sp)
	sw	$4,924($sp)
	lw	$2,924($sp)
	bne	$2,$0,$L445
	lw	$3,1408($sp)
	lw	$2,1412($sp)
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(set_flash_register_temperature_offset)
	addiu	$2,$2,%lo(set_flash_register_temperature_offset)
	jalr	$2
	j	$L303
$L445:
	addiu	$2,$sp,1280
	sw	$2,928($sp)
	lui	$2,%hi($LC170)
	addiu	$2,$2,%lo($LC170)
	sw	$2,932($sp)
	lw	$3,928($sp)
	lw	$2,932($sp)
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
	sw	$3,928($sp)
	sw	$2,932($sp)
	sw	$4,936($sp)
	lw	$2,936($sp)
	bne	$2,$0,$L447
	lw	$2,1408($sp)
	move	$4,$2
	lui	$2,%hi(GPON_BER)
	addiu	$2,$2,%lo(GPON_BER)
	jalr	$2
	j	$L303
$L447:
	addiu	$2,$sp,1280
	sw	$2,940($sp)
	lui	$2,%hi($LC171)
	addiu	$2,$2,%lo($LC171)
	sw	$2,944($sp)
	lw	$3,940($sp)
	lw	$2,944($sp)
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
	sw	$3,940($sp)
	sw	$2,944($sp)
	sw	$4,948($sp)
	lw	$2,948($sp)
	bne	$2,$0,$L449
	lw	$2,1408($sp)
	move	$4,$2
	lui	$2,%hi(EPON_BER)
	addiu	$2,$2,%lo(EPON_BER)
	jalr	$2
	j	$L303
$L449:
	addiu	$2,$sp,1280
	sw	$2,952($sp)
	lui	$2,%hi($LC172)
	addiu	$2,$2,%lo($LC172)
	sw	$2,956($sp)
	lw	$3,952($sp)
	lw	$2,956($sp)
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
	sw	$3,952($sp)
	sw	$2,956($sp)
	sw	$4,960($sp)
	lw	$2,960($sp)
	bne	$2,$0,$L451
	lui	$2,%hi(show_BoB_information)
	addiu	$2,$2,%lo(show_BoB_information)
	jalr	$2
	j	$L303
$L451:
	addiu	$2,$sp,1280
	sw	$2,964($sp)
	lui	$2,%hi($LC173)
	addiu	$2,$2,%lo($LC173)
	sw	$2,968($sp)
	lw	$3,964($sp)
	lw	$2,968($sp)
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
	sw	$3,964($sp)
	sw	$2,968($sp)
	sw	$4,972($sp)
	lw	$2,972($sp)
	bne	$2,$0,$L453
	li	$4,1			# 0x1
	lui	$2,%hi(mt7570_calibration_pattern)
	addiu	$2,$2,%lo(mt7570_calibration_pattern)
	jalr	$2
	j	$L303
$L453:
	addiu	$2,$sp,1280
	sw	$2,976($sp)
	lui	$2,%hi($LC174)
	addiu	$2,$2,%lo($LC174)
	sw	$2,980($sp)
	lw	$3,976($sp)
	lw	$2,980($sp)
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
	sw	$3,976($sp)
	sw	$2,980($sp)
	sw	$4,984($sp)
	lw	$2,984($sp)
	bne	$2,$0,$L455
	move	$4,$0
	lui	$2,%hi(mt7570_calibration_pattern)
	addiu	$2,$2,%lo(mt7570_calibration_pattern)
	jalr	$2
	j	$L303
$L455:
	addiu	$2,$sp,1280
	sw	$2,988($sp)
	lui	$2,%hi($LC175)
	addiu	$2,$2,%lo($LC175)
	sw	$2,992($sp)
	lw	$3,988($sp)
	lw	$2,992($sp)
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
	sw	$3,988($sp)
	sw	$2,992($sp)
	sw	$4,996($sp)
	lw	$2,996($sp)
	bne	$2,$0,$L457
	lui	$2,%hi(mt7570_disable_pattern)
	addiu	$2,$2,%lo(mt7570_disable_pattern)
	jalr	$2
	j	$L303
$L457:
	addiu	$2,$sp,1280
	sw	$2,1000($sp)
	lui	$2,%hi($LC176)
	addiu	$2,$2,%lo($LC176)
	sw	$2,1004($sp)
	lw	$3,1000($sp)
	lw	$2,1004($sp)
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
	sw	$3,1000($sp)
	sw	$2,1004($sp)
	sw	$4,1008($sp)
	lw	$2,1008($sp)
	bne	$2,$0,$L459
	lui	$2,%hi(pon_phy_status)
	addiu	$2,$2,%lo(pon_phy_status)
	jalr	$2
	j	$L303
$L459:
	addiu	$2,$sp,1280
	sw	$2,1012($sp)
	lui	$2,%hi($LC177)
	addiu	$2,$2,%lo($LC177)
	sw	$2,1016($sp)
	lw	$3,1012($sp)
	lw	$2,1016($sp)
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
	sw	$3,1012($sp)
	sw	$2,1016($sp)
	sw	$4,1020($sp)
	lw	$2,1020($sp)
	bne	$2,$0,$L461
	lui	$2,%hi(mt7570_OpenLoopMode)
	addiu	$2,$2,%lo(mt7570_OpenLoopMode)
	jalr	$2
	j	$L303
$L461:
	addiu	$2,$sp,1280
	sw	$2,1024($sp)
	lui	$2,%hi($LC178)
	addiu	$2,$2,%lo($LC178)
	sw	$2,1028($sp)
	lw	$3,1024($sp)
	lw	$2,1028($sp)
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
	sw	$3,1024($sp)
	sw	$2,1028($sp)
	sw	$4,1032($sp)
	lw	$2,1032($sp)
	bne	$2,$0,$L463
	lui	$2,%hi(mt7570_DualClosedLoopMode)
	addiu	$2,$2,%lo(mt7570_DualClosedLoopMode)
	jalr	$2
	j	$L303
$L463:
	addiu	$2,$sp,1280
	sw	$2,1036($sp)
	lui	$2,%hi($LC179)
	addiu	$2,$2,%lo($LC179)
	sw	$2,1040($sp)
	lw	$3,1036($sp)
	lw	$2,1040($sp)
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
	sw	$3,1036($sp)
	sw	$2,1040($sp)
	sw	$4,1044($sp)
	lw	$2,1044($sp)
	bne	$2,$0,$L465
	lui	$2,%hi(mt7570_SingleClosedLoopMode)
	addiu	$2,$2,%lo(mt7570_SingleClosedLoopMode)
	jalr	$2
	j	$L303
$L465:
	addiu	$2,$sp,1280
	sw	$2,1048($sp)
	lui	$2,%hi($LC180)
	addiu	$2,$2,%lo($LC180)
	sw	$2,1052($sp)
	lw	$3,1048($sp)
	lw	$2,1052($sp)
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
	sw	$3,1048($sp)
	sw	$2,1052($sp)
	sw	$4,1056($sp)
	lw	$2,1056($sp)
	bne	$2,$0,$L467
	lw	$2,1408($sp)
	move	$4,$2
	lui	$2,%hi(pon_register)
	addiu	$2,$2,%lo(pon_register)
	jalr	$2
	j	$L303
$L467:
	addiu	$2,$sp,1280
	sw	$2,1060($sp)
	lui	$2,%hi($LC181)
	addiu	$2,$2,%lo($LC181)
	sw	$2,1064($sp)
	lw	$3,1060($sp)
	lw	$2,1064($sp)
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
	sw	$3,1060($sp)
	sw	$2,1064($sp)
	sw	$4,1068($sp)
	lw	$2,1068($sp)
	bne	$2,$0,$L469
	lui	$2,%hi(Ibias_temperature_set)
	addiu	$2,$2,%lo(Ibias_temperature_set)
	jalr	$2
	j	$L303
$L469:
	addiu	$2,$sp,1280
	sw	$2,1072($sp)
	lui	$2,%hi($LC182)
	addiu	$2,$2,%lo($LC182)
	sw	$2,1076($sp)
	lw	$3,1072($sp)
	lw	$2,1076($sp)
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
	sw	$3,1072($sp)
	sw	$2,1076($sp)
	sw	$4,1080($sp)
	lw	$2,1080($sp)
	bne	$2,$0,$L471
	lui	$2,%hi(mt7570_restart_ERC_P0)
	addiu	$2,$2,%lo(mt7570_restart_ERC_P0)
	jalr	$2
	j	$L303
$L471:
	addiu	$2,$sp,1280
	sw	$2,1084($sp)
	lui	$2,%hi($LC183)
	addiu	$2,$2,%lo($LC183)
	sw	$2,1088($sp)
	lw	$3,1084($sp)
	lw	$2,1088($sp)
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
	sw	$3,1084($sp)
	sw	$2,1088($sp)
	sw	$4,1092($sp)
	lw	$2,1092($sp)
	bne	$2,$0,$L473
	lui	$2,%hi(mt7570_ADC_calibration)
	addiu	$2,$2,%lo(mt7570_ADC_calibration)
	jalr	$2
	j	$L303
$L473:
	addiu	$2,$sp,1280
	sw	$2,1096($sp)
	lui	$2,%hi($LC184)
	addiu	$2,$2,%lo($LC184)
	sw	$2,1100($sp)
	lw	$3,1096($sp)
	lw	$2,1100($sp)
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
	sw	$3,1096($sp)
	sw	$2,1100($sp)
	sw	$4,1104($sp)
	lw	$2,1104($sp)
	bne	$2,$0,$L475
	lui	$2,%hi(internal_DDMI)
	sw	$0,%lo(internal_DDMI)($2)
	lui	$2,%hi($LC185)
	addiu	$4,$2,%lo($LC185)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	j	$L303
$L475:
	addiu	$2,$sp,1280
	sw	$2,1108($sp)
	lui	$2,%hi($LC186)
	addiu	$2,$2,%lo($LC186)
	sw	$2,1112($sp)
	lw	$3,1108($sp)
	lw	$2,1112($sp)
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
	sw	$3,1108($sp)
	sw	$2,1112($sp)
	sw	$4,1116($sp)
	lw	$2,1116($sp)
	bne	$2,$0,$L477
	lui	$2,%hi(mt7570_LUT_recover)
	addiu	$2,$2,%lo(mt7570_LUT_recover)
	jalr	$2
	j	$L303
$L477:
	addiu	$2,$sp,1280
	sw	$2,1120($sp)
	lui	$2,%hi($LC187)
	addiu	$2,$2,%lo($LC187)
	sw	$2,1124($sp)
	lw	$3,1120($sp)
	lw	$2,1124($sp)
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
	sw	$3,1120($sp)
	sw	$2,1124($sp)
	sw	$4,1128($sp)
	lw	$2,1128($sp)
	bne	$2,$0,$L479
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,136($2)
	sw	$2,76($sp)
	addiu	$2,$sp,1344
	sw	$2,1132($sp)
	lui	$2,%hi($LC188)
	addiu	$2,$2,%lo($LC188)
	sw	$2,1136($sp)
	lw	$3,1132($sp)
	lw	$2,1136($sp)
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
	sw	$3,1132($sp)
	sw	$2,1136($sp)
	sw	$4,1140($sp)
	lw	$2,1140($sp)
	bne	$2,$0,$L481
	lui	$2,%hi(gpPhyPriv)
	lw	$3,%lo(gpPhyPriv)($2)
	lw	$2,1420($sp)
	beq	$2,$0,$L482
	lw	$2,76($sp)
	seh	$2,$2
	ori	$2,$2,0x4
	seh	$2,$2
	andi	$2,$2,0xffff
	j	$L483
$L482:
	lw	$2,76($sp)
	andi	$4,$2,0xffff
	li	$2,-5			# 0xfffffffffffffffb
	and	$2,$4,$2
	andi	$2,$2,0xffff
$L483:
	sh	$2,136($3)
	j	$L484
$L481:
	addiu	$2,$sp,1344
	sw	$2,1144($sp)
	lui	$2,%hi($LC189)
	addiu	$2,$2,%lo($LC189)
	sw	$2,1148($sp)
	lw	$3,1144($sp)
	lw	$2,1148($sp)
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
	sw	$3,1144($sp)
	sw	$2,1148($sp)
	sw	$4,1152($sp)
	lw	$2,1152($sp)
	bne	$2,$0,$L486
	lui	$2,%hi(gpPhyPriv)
	lw	$3,%lo(gpPhyPriv)($2)
	lw	$2,1420($sp)
	beq	$2,$0,$L487
	lw	$2,76($sp)
	seh	$2,$2
	ori	$2,$2,0x2
	seh	$2,$2
	andi	$2,$2,0xffff
	j	$L488
$L487:
	lw	$2,76($sp)
	andi	$4,$2,0xffff
	li	$2,-3			# 0xfffffffffffffffd
	and	$2,$4,$2
	andi	$2,$2,0xffff
$L488:
	sh	$2,136($3)
	j	$L484
$L486:
	addiu	$2,$sp,1344
	sw	$2,1156($sp)
	lui	$2,%hi($LC190)
	addiu	$2,$2,%lo($LC190)
	sw	$2,1160($sp)
	lw	$3,1156($sp)
	lw	$2,1160($sp)
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
	sw	$3,1156($sp)
	sw	$2,1160($sp)
	sw	$4,1164($sp)
	lw	$2,1164($sp)
	bne	$2,$0,$L490
	lui	$2,%hi(gpPhyPriv)
	lw	$3,%lo(gpPhyPriv)($2)
	lw	$2,1420($sp)
	beq	$2,$0,$L491
	lw	$2,76($sp)
	seh	$2,$2
	ori	$2,$2,0x8
	seh	$2,$2
	andi	$2,$2,0xffff
	j	$L492
$L491:
	lw	$2,76($sp)
	andi	$4,$2,0xffff
	li	$2,-9			# 0xfffffffffffffff7
	and	$2,$4,$2
	andi	$2,$2,0xffff
$L492:
	sh	$2,136($3)
	j	$L484
$L490:
	addiu	$2,$sp,1344
	sw	$2,1168($sp)
	lui	$2,%hi($LC191)
	addiu	$2,$2,%lo($LC191)
	sw	$2,1172($sp)
	lw	$3,1168($sp)
	lw	$2,1172($sp)
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
	sw	$3,1168($sp)
	sw	$2,1172($sp)
	sw	$4,1176($sp)
	lw	$2,1176($sp)
	bne	$2,$0,$L494
	lui	$2,%hi(gpPhyPriv)
	lw	$3,%lo(gpPhyPriv)($2)
	lw	$2,1420($sp)
	beq	$2,$0,$L495
	lw	$2,76($sp)
	seh	$2,$2
	ori	$2,$2,0x10
	seh	$2,$2
	andi	$2,$2,0xffff
	j	$L496
$L495:
	lw	$2,76($sp)
	andi	$4,$2,0xffff
	li	$2,-17			# 0xffffffffffffffef
	and	$2,$4,$2
	andi	$2,$2,0xffff
$L496:
	sh	$2,136($3)
	j	$L484
$L494:
	addiu	$2,$sp,1344
	sw	$2,1180($sp)
	lui	$2,%hi($LC192)
	addiu	$2,$2,%lo($LC192)
	sw	$2,1184($sp)
	lw	$3,1180($sp)
	lw	$2,1184($sp)
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
	sw	$3,1180($sp)
	sw	$2,1184($sp)
	sw	$4,1188($sp)
	lw	$2,1188($sp)
	bne	$2,$0,$L484
	lui	$2,%hi(gpPhyPriv)
	lw	$3,%lo(gpPhyPriv)($2)
	lw	$2,1420($sp)
	beq	$2,$0,$L498
	lw	$2,76($sp)
	seh	$2,$2
	ori	$2,$2,0x1
	seh	$2,$2
	andi	$2,$2,0xffff
	j	$L499
$L498:
	lw	$2,76($sp)
	andi	$4,$2,0xffff
	li	$2,-2			# 0xfffffffffffffffe
	and	$2,$4,$2
	andi	$2,$2,0xffff
$L499:
	sh	$2,136($3)
$L484:
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,136($2)
	move	$3,$2
	lui	$2,%hi($LC193)
	addiu	$4,$2,%lo($LC193)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	j	$L303
$L479:
	addiu	$2,$sp,1280
	sw	$2,1192($sp)
	lui	$2,%hi($LC194)
	addiu	$2,$2,%lo($LC194)
	sw	$2,1196($sp)
	lw	$3,1192($sp)
	lw	$2,1196($sp)
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
	sw	$3,1192($sp)
	sw	$2,1196($sp)
	sw	$4,1200($sp)
	lw	$2,1200($sp)
	bne	$2,$0,$L501
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lw	$3,1408($sp)
	andi	$3,$3,0xffff
	sh	$3,116($2)
	j	$L303
$L501:
	addiu	$2,$sp,1280
	sw	$2,1204($sp)
	lui	$2,%hi($LC195)
	addiu	$2,$2,%lo($LC195)
	sw	$2,1208($sp)
	lw	$3,1204($sp)
	lw	$2,1208($sp)
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
	sw	$3,1204($sp)
	sw	$2,1208($sp)
	sw	$4,1212($sp)
	lw	$2,1212($sp)
	bne	$2,$0,$L303
	lui	$2,%hi($LC196)
	addiu	$4,$2,%lo($LC196)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	lui	$2,%hi($LC197)
	addiu	$4,$2,%lo($LC197)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	lui	$2,%hi($LC198)
	addiu	$4,$2,%lo($LC198)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	lui	$2,%hi($LC199)
	addiu	$4,$2,%lo($LC199)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	lui	$2,%hi($LC200)
	addiu	$4,$2,%lo($LC200)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	lui	$2,%hi($LC201)
	addiu	$4,$2,%lo($LC201)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	lui	$2,%hi($LC202)
	addiu	$4,$2,%lo($LC202)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	lui	$2,%hi($LC203)
	addiu	$4,$2,%lo($LC203)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	lui	$2,%hi($LC204)
	addiu	$4,$2,%lo($LC204)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	lui	$2,%hi($LC205)
	addiu	$4,$2,%lo($LC205)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	lui	$2,%hi($LC206)
	addiu	$4,$2,%lo($LC206)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	lui	$2,%hi($LC207)
	addiu	$4,$2,%lo($LC207)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	lui	$2,%hi($LC208)
	addiu	$4,$2,%lo($LC208)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	lui	$2,%hi($LC209)
	addiu	$4,$2,%lo($LC209)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	lui	$2,%hi($LC210)
	addiu	$4,$2,%lo($LC210)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	lui	$2,%hi($LC211)
	addiu	$4,$2,%lo($LC211)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	lui	$2,%hi($LC212)
	addiu	$4,$2,%lo($LC212)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	lui	$2,%hi($LC213)
	addiu	$4,$2,%lo($LC213)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	lui	$2,%hi($LC214)
	addiu	$4,$2,%lo($LC214)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	lui	$2,%hi($LC215)
	addiu	$4,$2,%lo($LC215)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	lui	$2,%hi($LC216)
	addiu	$4,$2,%lo($LC216)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	lui	$2,%hi($LC217)
	addiu	$4,$2,%lo($LC217)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	lui	$2,%hi($LC218)
	addiu	$4,$2,%lo($LC218)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	lui	$2,%hi($LC219)
	addiu	$4,$2,%lo($LC219)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	lui	$2,%hi($LC220)
	addiu	$4,$2,%lo($LC220)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	lui	$2,%hi($LC221)
	addiu	$4,$2,%lo($LC221)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	lui	$2,%hi($LC222)
	addiu	$4,$2,%lo($LC222)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	lui	$2,%hi($LC223)
	addiu	$4,$2,%lo($LC223)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	lui	$2,%hi($LC224)
	addiu	$4,$2,%lo($LC224)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	lui	$2,%hi($LC225)
	addiu	$4,$2,%lo($LC225)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	lui	$2,%hi($LC226)
	addiu	$4,$2,%lo($LC226)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	lui	$2,%hi($LC227)
	addiu	$4,$2,%lo($LC227)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	lui	$2,%hi($LC228)
	addiu	$4,$2,%lo($LC228)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	lui	$2,%hi($LC229)
	addiu	$4,$2,%lo($LC229)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	lui	$2,%hi($LC230)
	addiu	$4,$2,%lo($LC230)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	lui	$2,%hi($LC231)
	addiu	$4,$2,%lo($LC231)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	lui	$2,%hi($LC232)
	addiu	$4,$2,%lo($LC232)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	lui	$2,%hi($LC233)
	addiu	$4,$2,%lo($LC233)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	lui	$2,%hi($LC234)
	addiu	$4,$2,%lo($LC234)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	lui	$2,%hi($LC235)
	addiu	$4,$2,%lo($LC235)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	lui	$2,%hi($LC236)
	addiu	$4,$2,%lo($LC236)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
$L303:
	lw	$2,1552($sp)
$L503:
	lw	$31,1540($sp)
	addiu	$sp,$sp,1544
	j	$31
	.end	phy_write_proc
	.size	phy_write_proc, .-phy_write_proc
	.rdata
	.align	2
$LC237:
	.ascii	"%x %x %x %x %x %x\012\000"
	.section	.text.phy_proc_ddmi_read_proc,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	phy_proc_ddmi_read_proc
	.type	phy_proc_ddmi_read_proc, @function
phy_proc_ddmi_read_proc:
	.frame	$sp,88,$31		# vars= 48, regs= 1/0, args= 32, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-88
	sw	$31,84($sp)
	sw	$4,88($sp)
	sw	$5,92($sp)
	sw	$6,96($sp)
	sw	$7,100($sp)
	sw	$0,32($sp)
	sw	$0,68($sp)
	sw	$0,72($sp)
	sh	$0,76($sp)
	sw	$0,40($sp)
	sw	$0,44($sp)
	sw	$0,48($sp)
	sw	$0,52($sp)
	sw	$0,56($sp)
	sw	$0,60($sp)
	lui	$2,%hi(mt7570_ADC_temperature_get)
	addiu	$2,$2,%lo(mt7570_ADC_temperature_get)
	jalr	$2
	nop

	sw	$2,40($sp)
	lui	$2,%hi(mt7570_ADC_voltage_get)
	addiu	$2,$2,%lo(mt7570_ADC_voltage_get)
	jalr	$2
	nop

	sw	$2,44($sp)
	li	$4,3			# 0x3
	lui	$2,%hi(mt7570_information_output)
	addiu	$2,$2,%lo(mt7570_information_output)
	jalr	$2
	nop

	sw	$2,48($sp)
	li	$4,4			# 0x4
	lui	$2,%hi(mt7570_information_output)
	addiu	$2,$2,%lo(mt7570_information_output)
	jalr	$2
	nop

	sw	$2,52($sp)
	lui	$2,%hi(mt7570_RSSI_current)
	addiu	$2,$2,%lo(mt7570_RSSI_current)
	jalr	$2
	nop

	sw	$2,56($sp)
	lui	$2,%hi(mt7570_MPD_current)
	addiu	$2,$2,%lo(mt7570_MPD_current)
	jalr	$2
	nop

	sw	$2,60($sp)
	sw	$0,64($sp)
	sw	$0,36($sp)
	lw	$2,32($sp)
	lw	$3,88($sp)
	addu	$3,$3,$2
	lw	$2,48($sp)
	sw	$2,16($sp)
	lw	$2,52($sp)
	sw	$2,20($sp)
	lw	$2,56($sp)
	sw	$2,24($sp)
	lw	$2,60($sp)
	sw	$2,28($sp)
	move	$4,$3
	lui	$2,%hi($LC237)
	addiu	$5,$2,%lo($LC237)
	lw	$6,40($sp)
	lw	$7,44($sp)
	lui	$2,%hi(sprintf)
	addiu	$2,$2,%lo(sprintf)
	jalr	$2
	nop

	move	$3,$2
	lw	$2,32($sp)
	addu	$2,$2,$3
	sw	$2,32($sp)
	lw	$3,36($sp)
	lw	$2,32($sp)
	addu	$2,$3,$2
	sw	$2,64($sp)
	lw	$3,64($sp)
	lw	$2,96($sp)
	slt	$2,$3,$2
	beq	$2,$0,$L505
	nop

	sw	$0,32($sp)
	lw	$2,64($sp)
	sw	$2,36($sp)
$L505:
	lw	$3,96($sp)
	lw	$2,100($sp)
	addu	$3,$3,$2
	lw	$2,64($sp)
	slt	$2,$3,$2
	beq	$2,$0,$L506
	nop

	j	$L507
	nop

$L506:
	lw	$2,104($sp)
	li	$3,1			# 0x1
	sw	$3,0($2)
$L507:
	lw	$3,96($sp)
	lw	$2,36($sp)
	subu	$2,$3,$2
	move	$3,$2
	lw	$2,88($sp)
	addu	$3,$2,$3
	lw	$2,92($sp)
	sw	$3,0($2)
	lw	$3,36($sp)
	lw	$2,96($sp)
	subu	$2,$3,$2
	lw	$3,32($sp)
	addu	$2,$3,$2
	sw	$2,32($sp)
	lw	$2,32($sp)
	bgez	$2,$L508
	nop

	sw	$0,32($sp)
$L508:
	lw	$3,32($sp)
	lw	$2,100($sp)
	slt	$2,$2,$3
	beq	$2,$0,$L509
	nop

	lw	$2,100($sp)
	sw	$2,32($sp)
$L509:
	lw	$2,32($sp)
	lw	$31,84($sp)
	addiu	$sp,$sp,88
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	phy_proc_ddmi_read_proc
	.size	phy_proc_ddmi_read_proc, .-phy_proc_ddmi_read_proc
	.rdata
	.align	2
$LC238:
	.ascii	"%x\012\000"
	.section	.text.phy_proc_temperature_read_proc,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	phy_proc_temperature_read_proc
	.type	phy_proc_temperature_read_proc, @function
phy_proc_temperature_read_proc:
	.frame	$sp,56,$31		# vars= 32, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-56
	sw	$31,52($sp)
	sw	$4,56($sp)
	sw	$5,60($sp)
	sw	$6,64($sp)
	sw	$7,68($sp)
	sw	$0,16($sp)
	sw	$0,32($sp)
	sw	$0,36($sp)
	sh	$0,40($sp)
	sw	$0,24($sp)
	lui	$2,%hi(mt7570_ADC_temperature_get)
	addiu	$2,$2,%lo(mt7570_ADC_temperature_get)
	jalr	$2
	nop

	sw	$2,24($sp)
	sw	$0,28($sp)
	sw	$0,20($sp)
	lw	$2,16($sp)
	lw	$3,56($sp)
	addu	$2,$3,$2
	move	$4,$2
	lui	$2,%hi($LC238)
	addiu	$5,$2,%lo($LC238)
	lw	$6,24($sp)
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
	sw	$2,28($sp)
	lw	$3,28($sp)
	lw	$2,64($sp)
	slt	$2,$3,$2
	beq	$2,$0,$L512
	nop

	sw	$0,16($sp)
	lw	$2,28($sp)
	sw	$2,20($sp)
$L512:
	lw	$3,64($sp)
	lw	$2,68($sp)
	addu	$3,$3,$2
	lw	$2,28($sp)
	slt	$2,$3,$2
	beq	$2,$0,$L513
	nop

	j	$L514
	nop

$L513:
	lw	$2,72($sp)
	li	$3,1			# 0x1
	sw	$3,0($2)
$L514:
	lw	$3,64($sp)
	lw	$2,20($sp)
	subu	$2,$3,$2
	move	$3,$2
	lw	$2,56($sp)
	addu	$3,$2,$3
	lw	$2,60($sp)
	sw	$3,0($2)
	lw	$3,20($sp)
	lw	$2,64($sp)
	subu	$2,$3,$2
	lw	$3,16($sp)
	addu	$2,$3,$2
	sw	$2,16($sp)
	lw	$2,16($sp)
	bgez	$2,$L515
	nop

	sw	$0,16($sp)
$L515:
	lw	$3,16($sp)
	lw	$2,68($sp)
	slt	$2,$2,$3
	beq	$2,$0,$L516
	nop

	lw	$2,68($sp)
	sw	$2,16($sp)
$L516:
	lw	$2,16($sp)
	lw	$31,52($sp)
	addiu	$sp,$sp,56
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	phy_proc_temperature_read_proc
	.size	phy_proc_temperature_read_proc, .-phy_proc_temperature_read_proc
	.section	.text.phy_proc_vcc_read_proc,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	phy_proc_vcc_read_proc
	.type	phy_proc_vcc_read_proc, @function
phy_proc_vcc_read_proc:
	.frame	$sp,56,$31		# vars= 32, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-56
	sw	$31,52($sp)
	sw	$4,56($sp)
	sw	$5,60($sp)
	sw	$6,64($sp)
	sw	$7,68($sp)
	sw	$0,16($sp)
	sw	$0,32($sp)
	sw	$0,36($sp)
	sh	$0,40($sp)
	sw	$0,24($sp)
	lui	$2,%hi(mt7570_ADC_voltage_get)
	addiu	$2,$2,%lo(mt7570_ADC_voltage_get)
	jalr	$2
	nop

	sw	$2,24($sp)
	sw	$0,28($sp)
	sw	$0,20($sp)
	lw	$2,16($sp)
	lw	$3,56($sp)
	addu	$2,$3,$2
	move	$4,$2
	lui	$2,%hi($LC238)
	addiu	$5,$2,%lo($LC238)
	lw	$6,24($sp)
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
	sw	$2,28($sp)
	lw	$3,28($sp)
	lw	$2,64($sp)
	slt	$2,$3,$2
	beq	$2,$0,$L519
	nop

	sw	$0,16($sp)
	lw	$2,28($sp)
	sw	$2,20($sp)
$L519:
	lw	$3,64($sp)
	lw	$2,68($sp)
	addu	$3,$3,$2
	lw	$2,28($sp)
	slt	$2,$3,$2
	beq	$2,$0,$L520
	nop

	j	$L521
	nop

$L520:
	lw	$2,72($sp)
	li	$3,1			# 0x1
	sw	$3,0($2)
$L521:
	lw	$3,64($sp)
	lw	$2,20($sp)
	subu	$2,$3,$2
	move	$3,$2
	lw	$2,56($sp)
	addu	$3,$2,$3
	lw	$2,60($sp)
	sw	$3,0($2)
	lw	$3,20($sp)
	lw	$2,64($sp)
	subu	$2,$3,$2
	lw	$3,16($sp)
	addu	$2,$3,$2
	sw	$2,16($sp)
	lw	$2,16($sp)
	bgez	$2,$L522
	nop

	sw	$0,16($sp)
$L522:
	lw	$3,16($sp)
	lw	$2,68($sp)
	slt	$2,$2,$3
	beq	$2,$0,$L523
	nop

	lw	$2,68($sp)
	sw	$2,16($sp)
$L523:
	lw	$2,16($sp)
	lw	$31,52($sp)
	addiu	$sp,$sp,56
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	phy_proc_vcc_read_proc
	.size	phy_proc_vcc_read_proc, .-phy_proc_vcc_read_proc
	.section	.text.phy_proc_bias_read_proc,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	phy_proc_bias_read_proc
	.type	phy_proc_bias_read_proc, @function
phy_proc_bias_read_proc:
	.frame	$sp,56,$31		# vars= 32, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-56
	sw	$31,52($sp)
	sw	$4,56($sp)
	sw	$5,60($sp)
	sw	$6,64($sp)
	sw	$7,68($sp)
	sw	$0,16($sp)
	sw	$0,32($sp)
	sw	$0,36($sp)
	sh	$0,40($sp)
	sw	$0,24($sp)
	li	$4,3			# 0x3
	lui	$2,%hi(mt7570_information_output)
	addiu	$2,$2,%lo(mt7570_information_output)
	jalr	$2
	nop

	sw	$2,24($sp)
	sw	$0,28($sp)
	sw	$0,20($sp)
	lw	$2,16($sp)
	lw	$3,56($sp)
	addu	$2,$3,$2
	move	$4,$2
	lui	$2,%hi($LC238)
	addiu	$5,$2,%lo($LC238)
	lw	$6,24($sp)
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
	sw	$2,28($sp)
	lw	$3,28($sp)
	lw	$2,64($sp)
	slt	$2,$3,$2
	beq	$2,$0,$L526
	nop

	sw	$0,16($sp)
	lw	$2,28($sp)
	sw	$2,20($sp)
$L526:
	lw	$3,64($sp)
	lw	$2,68($sp)
	addu	$3,$3,$2
	lw	$2,28($sp)
	slt	$2,$3,$2
	beq	$2,$0,$L527
	nop

	j	$L528
	nop

$L527:
	lw	$2,72($sp)
	li	$3,1			# 0x1
	sw	$3,0($2)
$L528:
	lw	$3,64($sp)
	lw	$2,20($sp)
	subu	$2,$3,$2
	move	$3,$2
	lw	$2,56($sp)
	addu	$3,$2,$3
	lw	$2,60($sp)
	sw	$3,0($2)
	lw	$3,20($sp)
	lw	$2,64($sp)
	subu	$2,$3,$2
	lw	$3,16($sp)
	addu	$2,$3,$2
	sw	$2,16($sp)
	lw	$2,16($sp)
	bgez	$2,$L529
	nop

	sw	$0,16($sp)
$L529:
	lw	$3,16($sp)
	lw	$2,68($sp)
	slt	$2,$2,$3
	beq	$2,$0,$L530
	nop

	lw	$2,68($sp)
	sw	$2,16($sp)
$L530:
	lw	$2,16($sp)
	lw	$31,52($sp)
	addiu	$sp,$sp,56
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	phy_proc_bias_read_proc
	.size	phy_proc_bias_read_proc, .-phy_proc_bias_read_proc
	.section	.text.phy_proc_modulation_read_proc,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	phy_proc_modulation_read_proc
	.type	phy_proc_modulation_read_proc, @function
phy_proc_modulation_read_proc:
	.frame	$sp,56,$31		# vars= 32, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-56
	sw	$31,52($sp)
	sw	$4,56($sp)
	sw	$5,60($sp)
	sw	$6,64($sp)
	sw	$7,68($sp)
	sw	$0,16($sp)
	sw	$0,32($sp)
	sw	$0,36($sp)
	sh	$0,40($sp)
	sw	$0,24($sp)
	li	$4,4			# 0x4
	lui	$2,%hi(mt7570_information_output)
	addiu	$2,$2,%lo(mt7570_information_output)
	jalr	$2
	nop

	sw	$2,24($sp)
	sw	$0,28($sp)
	sw	$0,20($sp)
	lw	$2,16($sp)
	lw	$3,56($sp)
	addu	$2,$3,$2
	move	$4,$2
	lui	$2,%hi($LC238)
	addiu	$5,$2,%lo($LC238)
	lw	$6,24($sp)
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
	sw	$2,28($sp)
	lw	$3,28($sp)
	lw	$2,64($sp)
	slt	$2,$3,$2
	beq	$2,$0,$L533
	nop

	sw	$0,16($sp)
	lw	$2,28($sp)
	sw	$2,20($sp)
$L533:
	lw	$3,64($sp)
	lw	$2,68($sp)
	addu	$3,$3,$2
	lw	$2,28($sp)
	slt	$2,$3,$2
	beq	$2,$0,$L534
	nop

	j	$L535
	nop

$L534:
	lw	$2,72($sp)
	li	$3,1			# 0x1
	sw	$3,0($2)
$L535:
	lw	$3,64($sp)
	lw	$2,20($sp)
	subu	$2,$3,$2
	move	$3,$2
	lw	$2,56($sp)
	addu	$3,$2,$3
	lw	$2,60($sp)
	sw	$3,0($2)
	lw	$3,20($sp)
	lw	$2,64($sp)
	subu	$2,$3,$2
	lw	$3,16($sp)
	addu	$2,$3,$2
	sw	$2,16($sp)
	lw	$2,16($sp)
	bgez	$2,$L536
	nop

	sw	$0,16($sp)
$L536:
	lw	$3,16($sp)
	lw	$2,68($sp)
	slt	$2,$2,$3
	beq	$2,$0,$L537
	nop

	lw	$2,68($sp)
	sw	$2,16($sp)
$L537:
	lw	$2,16($sp)
	lw	$31,52($sp)
	addiu	$sp,$sp,56
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	phy_proc_modulation_read_proc
	.size	phy_proc_modulation_read_proc, .-phy_proc_modulation_read_proc
	.section	.text.phy_proc_rssi_read_proc,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	phy_proc_rssi_read_proc
	.type	phy_proc_rssi_read_proc, @function
phy_proc_rssi_read_proc:
	.frame	$sp,56,$31		# vars= 32, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-56
	sw	$31,52($sp)
	sw	$4,56($sp)
	sw	$5,60($sp)
	sw	$6,64($sp)
	sw	$7,68($sp)
	sw	$0,16($sp)
	sw	$0,32($sp)
	sw	$0,36($sp)
	sh	$0,40($sp)
	sw	$0,24($sp)
	lui	$2,%hi(mt7570_RSSI_current)
	addiu	$2,$2,%lo(mt7570_RSSI_current)
	jalr	$2
	nop

	sw	$2,24($sp)
	sw	$0,28($sp)
	sw	$0,20($sp)
	lw	$2,16($sp)
	lw	$3,56($sp)
	addu	$2,$3,$2
	move	$4,$2
	lui	$2,%hi($LC238)
	addiu	$5,$2,%lo($LC238)
	lw	$6,24($sp)
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
	sw	$2,28($sp)
	lw	$3,28($sp)
	lw	$2,64($sp)
	slt	$2,$3,$2
	beq	$2,$0,$L540
	nop

	sw	$0,16($sp)
	lw	$2,28($sp)
	sw	$2,20($sp)
$L540:
	lw	$3,64($sp)
	lw	$2,68($sp)
	addu	$3,$3,$2
	lw	$2,28($sp)
	slt	$2,$3,$2
	beq	$2,$0,$L541
	nop

	j	$L542
	nop

$L541:
	lw	$2,72($sp)
	li	$3,1			# 0x1
	sw	$3,0($2)
$L542:
	lw	$3,64($sp)
	lw	$2,20($sp)
	subu	$2,$3,$2
	move	$3,$2
	lw	$2,56($sp)
	addu	$3,$2,$3
	lw	$2,60($sp)
	sw	$3,0($2)
	lw	$3,20($sp)
	lw	$2,64($sp)
	subu	$2,$3,$2
	lw	$3,16($sp)
	addu	$2,$3,$2
	sw	$2,16($sp)
	lw	$2,16($sp)
	bgez	$2,$L543
	nop

	sw	$0,16($sp)
$L543:
	lw	$3,16($sp)
	lw	$2,68($sp)
	slt	$2,$2,$3
	beq	$2,$0,$L544
	nop

	lw	$2,68($sp)
	sw	$2,16($sp)
$L544:
	lw	$2,16($sp)
	lw	$31,52($sp)
	addiu	$sp,$sp,56
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	phy_proc_rssi_read_proc
	.size	phy_proc_rssi_read_proc, .-phy_proc_rssi_read_proc
	.section	.text.phy_proc_mpd_read_proc,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	phy_proc_mpd_read_proc
	.type	phy_proc_mpd_read_proc, @function
phy_proc_mpd_read_proc:
	.frame	$sp,56,$31		# vars= 32, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-56
	sw	$31,52($sp)
	sw	$4,56($sp)
	sw	$5,60($sp)
	sw	$6,64($sp)
	sw	$7,68($sp)
	sw	$0,16($sp)
	sw	$0,32($sp)
	sw	$0,36($sp)
	sh	$0,40($sp)
	sw	$0,24($sp)
	lui	$2,%hi(mt7570_MPD_current)
	addiu	$2,$2,%lo(mt7570_MPD_current)
	jalr	$2
	nop

	sw	$2,24($sp)
	sw	$0,28($sp)
	sw	$0,20($sp)
	lw	$2,16($sp)
	lw	$3,56($sp)
	addu	$2,$3,$2
	move	$4,$2
	lui	$2,%hi($LC238)
	addiu	$5,$2,%lo($LC238)
	lw	$6,24($sp)
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
	sw	$2,28($sp)
	lw	$3,28($sp)
	lw	$2,64($sp)
	slt	$2,$3,$2
	beq	$2,$0,$L547
	nop

	sw	$0,16($sp)
	lw	$2,28($sp)
	sw	$2,20($sp)
$L547:
	lw	$3,64($sp)
	lw	$2,68($sp)
	addu	$3,$3,$2
	lw	$2,28($sp)
	slt	$2,$3,$2
	beq	$2,$0,$L548
	nop

	j	$L549
	nop

$L548:
	lw	$2,72($sp)
	li	$3,1			# 0x1
	sw	$3,0($2)
$L549:
	lw	$3,64($sp)
	lw	$2,20($sp)
	subu	$2,$3,$2
	move	$3,$2
	lw	$2,56($sp)
	addu	$3,$2,$3
	lw	$2,60($sp)
	sw	$3,0($2)
	lw	$3,20($sp)
	lw	$2,64($sp)
	subu	$2,$3,$2
	lw	$3,16($sp)
	addu	$2,$3,$2
	sw	$2,16($sp)
	lw	$2,16($sp)
	bgez	$2,$L550
	nop

	sw	$0,16($sp)
$L550:
	lw	$3,16($sp)
	lw	$2,68($sp)
	slt	$2,$2,$3
	beq	$2,$0,$L551
	nop

	lw	$2,68($sp)
	sw	$2,16($sp)
$L551:
	lw	$2,16($sp)
	lw	$31,52($sp)
	addiu	$sp,$sp,56
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	phy_proc_mpd_read_proc
	.size	phy_proc_mpd_read_proc, .-phy_proc_mpd_read_proc
	.rdata
	.align	2
$LC239:
	.ascii	"%x\000"
	.section	.text.phy_proc_apd_control_write_proc,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	phy_proc_apd_control_write_proc
	.type	phy_proc_apd_control_write_proc, @function
phy_proc_apd_control_write_proc:
	.frame	$sp,128,$31		# vars= 104, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	addiu	$sp,$sp,-128
	sw	$31,124($sp)
	sw	$4,128($sp)
	sw	$5,132($sp)
	sw	$6,136($sp)
	sw	$7,140($sp)
	lw	$2,136($sp)
	sltu	$2,$2,64
	bne	$2,$0,$L554
	li	$2,-22			# 0xffffffffffffffea
	j	$L558
$L554:
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
	beq	$2,$0,$L556
	lw	$4,20($sp)
	lw	$5,24($sp)
	lw	$6,16($sp)
#APP
 # 2402 "/opt/tclinux_phoenix/modules/private/xpon_phy/src/phy.c" 1
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
$L556:
	lw	$2,16($sp)
	beq	$2,$0,$L557
	li	$2,-14			# 0xfffffffffffffff2
	j	$L558
$L557:
	addiu	$2,$sp,44
	addiu	$3,$sp,112
	move	$4,$2
	lui	$2,%hi($LC239)
	addiu	$5,$2,%lo($LC239)
	move	$6,$3
	lui	$2,%hi(sscanf)
	addiu	$2,$2,%lo(sscanf)
	jalr	$2
	lw	$2,112($sp)
	andi	$2,$2,0x00ff
	sb	$2,108($sp)
	addiu	$2,$sp,108
	move	$4,$2
	lui	$2,%hi(mt7570_APD_I2C_write)
	addiu	$2,$2,%lo(mt7570_APD_I2C_write)
	jalr	$2
	lw	$2,136($sp)
$L558:
	lw	$31,124($sp)
	addiu	$sp,$sp,128
	j	$31
	.end	phy_proc_apd_control_write_proc
	.size	phy_proc_apd_control_write_proc, .-phy_proc_apd_control_write_proc
	.local	phy_proc_dir
	.comm	phy_proc_dir,4,4
	.local	phy_proc
	.comm	phy_proc,4,4
	.local	phy_rogue
	.comm	phy_rogue,4,4
	.local	phy_info
	.comm	phy_info,4,4
	.local	phy_proc_ddmi
	.comm	phy_proc_ddmi,4,4
	.local	phy_proc_temperature
	.comm	phy_proc_temperature,4,4
	.local	phy_proc_vcc
	.comm	phy_proc_vcc,4,4
	.local	phy_proc_bias
	.comm	phy_proc_bias,4,4
	.local	phy_proc_modulation
	.comm	phy_proc_modulation,4,4
	.local	phy_proc_rssi
	.comm	phy_proc_rssi,4,4
	.local	phy_proc_mpd
	.comm	phy_proc_mpd,4,4
	.local	phy_proc_apd_control
	.comm	phy_proc_apd_control,4,4
	.rdata
	.align	2
$LC240:
	.ascii	"pon_phy\000"
	.align	2
$LC241:
	.ascii	"info\000"
	.align	2
$LC242:
	.ascii	"ddmi\000"
	.align	2
$LC243:
	.ascii	"Temperature\000"
	.align	2
$LC244:
	.ascii	"VCC\000"
	.align	2
$LC245:
	.ascii	"BiasCurrent\000"
	.align	2
$LC246:
	.ascii	"ModulationCurrent\000"
	.align	2
$LC247:
	.ascii	"RSSICurrent\000"
	.align	2
$LC248:
	.ascii	"MPDCurrent\000"
	.align	2
$LC249:
	.ascii	"APDVolControl\000"
	.align	2
$LC250:
	.ascii	"rogue\000"
	.section	.text.phy_debug_init,"ax",@progbits
	.align	2
	.globl	phy_debug_init
	.set	nomips16
	.set	nomicromips
	.ent	phy_debug_init
	.type	phy_debug_init, @function
phy_debug_init:
	.frame	$sp,24,$31		# vars= 0, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-24
	sw	$31,20($sp)
	lui	$2,%hi($LC240)
	addiu	$4,$2,%lo($LC240)
	move	$5,$0
	lui	$2,%hi(proc_mkdir)
	addiu	$2,$2,%lo(proc_mkdir)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(phy_proc_dir)
	sw	$3,%lo(phy_proc_dir)($2)
	lui	$2,%hi(phy_proc_dir)
	lw	$2,%lo(phy_proc_dir)($2)
	beq	$2,$0,$L560
	nop

	lui	$2,%hi(phy_proc_dir)
	lw	$3,%lo(phy_proc_dir)($2)
	lui	$2,%hi($LC191)
	addiu	$4,$2,%lo($LC191)
	move	$5,$0
	move	$6,$3
	lui	$2,%hi(create_proc_entry)
	addiu	$2,$2,%lo(create_proc_entry)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(phy_proc)
	sw	$3,%lo(phy_proc)($2)
	lui	$2,%hi(phy_proc)
	lw	$2,%lo(phy_proc)($2)
	beq	$2,$0,$L561
	nop

	lui	$2,%hi(phy_proc)
	lw	$2,%lo(phy_proc)($2)
	lui	$3,%hi(phy_write_proc)
	addiu	$3,$3,%lo(phy_write_proc)
	sw	$3,68($2)
	lui	$2,%hi(phy_proc)
	lw	$2,%lo(phy_proc)($2)
	lui	$3,%hi(phy_read_proc)
	addiu	$3,$3,%lo(phy_read_proc)
	sw	$3,64($2)
$L561:
	lui	$2,%hi(phy_proc_dir)
	lw	$3,%lo(phy_proc_dir)($2)
	lui	$2,%hi($LC241)
	addiu	$4,$2,%lo($LC241)
	move	$5,$0
	move	$6,$3
	lui	$2,%hi(create_proc_entry)
	addiu	$2,$2,%lo(create_proc_entry)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(phy_info)
	sw	$3,%lo(phy_info)($2)
	lui	$2,%hi(phy_info)
	lw	$2,%lo(phy_info)($2)
	beq	$2,$0,$L562
	nop

	lui	$2,%hi(phy_info)
	lw	$2,%lo(phy_info)($2)
	sw	$0,68($2)
	lui	$2,%hi(phy_info)
	lw	$2,%lo(phy_info)($2)
	lui	$3,%hi(phy_info_read_proc)
	addiu	$3,$3,%lo(phy_info_read_proc)
	sw	$3,64($2)
$L562:
	lui	$2,%hi(phy_proc_dir)
	lw	$3,%lo(phy_proc_dir)($2)
	lui	$2,%hi($LC242)
	addiu	$4,$2,%lo($LC242)
	move	$5,$0
	move	$6,$3
	lui	$2,%hi(create_proc_entry)
	addiu	$2,$2,%lo(create_proc_entry)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(phy_proc_ddmi)
	sw	$3,%lo(phy_proc_ddmi)($2)
	lui	$2,%hi(phy_proc_ddmi)
	lw	$2,%lo(phy_proc_ddmi)($2)
	beq	$2,$0,$L563
	nop

	lui	$2,%hi(phy_proc_ddmi)
	lw	$2,%lo(phy_proc_ddmi)($2)
	lui	$3,%hi(phy_proc_ddmi_read_proc)
	addiu	$3,$3,%lo(phy_proc_ddmi_read_proc)
	sw	$3,64($2)
$L563:
	lui	$2,%hi(phy_proc_dir)
	lw	$3,%lo(phy_proc_dir)($2)
	lui	$2,%hi($LC243)
	addiu	$4,$2,%lo($LC243)
	move	$5,$0
	move	$6,$3
	lui	$2,%hi(create_proc_entry)
	addiu	$2,$2,%lo(create_proc_entry)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(phy_proc_temperature)
	sw	$3,%lo(phy_proc_temperature)($2)
	lui	$2,%hi(phy_proc_temperature)
	lw	$2,%lo(phy_proc_temperature)($2)
	beq	$2,$0,$L564
	nop

	lui	$2,%hi(phy_proc_temperature)
	lw	$2,%lo(phy_proc_temperature)($2)
	lui	$3,%hi(phy_proc_temperature_read_proc)
	addiu	$3,$3,%lo(phy_proc_temperature_read_proc)
	sw	$3,64($2)
$L564:
	lui	$2,%hi(phy_proc_dir)
	lw	$3,%lo(phy_proc_dir)($2)
	lui	$2,%hi($LC244)
	addiu	$4,$2,%lo($LC244)
	move	$5,$0
	move	$6,$3
	lui	$2,%hi(create_proc_entry)
	addiu	$2,$2,%lo(create_proc_entry)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(phy_proc_vcc)
	sw	$3,%lo(phy_proc_vcc)($2)
	lui	$2,%hi(phy_proc_vcc)
	lw	$2,%lo(phy_proc_vcc)($2)
	beq	$2,$0,$L565
	nop

	lui	$2,%hi(phy_proc_vcc)
	lw	$2,%lo(phy_proc_vcc)($2)
	lui	$3,%hi(phy_proc_vcc_read_proc)
	addiu	$3,$3,%lo(phy_proc_vcc_read_proc)
	sw	$3,64($2)
$L565:
	lui	$2,%hi(phy_proc_dir)
	lw	$3,%lo(phy_proc_dir)($2)
	lui	$2,%hi($LC245)
	addiu	$4,$2,%lo($LC245)
	move	$5,$0
	move	$6,$3
	lui	$2,%hi(create_proc_entry)
	addiu	$2,$2,%lo(create_proc_entry)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(phy_proc_bias)
	sw	$3,%lo(phy_proc_bias)($2)
	lui	$2,%hi(phy_proc_bias)
	lw	$2,%lo(phy_proc_bias)($2)
	beq	$2,$0,$L566
	nop

	lui	$2,%hi(phy_proc_bias)
	lw	$2,%lo(phy_proc_bias)($2)
	lui	$3,%hi(phy_proc_bias_read_proc)
	addiu	$3,$3,%lo(phy_proc_bias_read_proc)
	sw	$3,64($2)
$L566:
	lui	$2,%hi(phy_proc_dir)
	lw	$3,%lo(phy_proc_dir)($2)
	lui	$2,%hi($LC246)
	addiu	$4,$2,%lo($LC246)
	move	$5,$0
	move	$6,$3
	lui	$2,%hi(create_proc_entry)
	addiu	$2,$2,%lo(create_proc_entry)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(phy_proc_modulation)
	sw	$3,%lo(phy_proc_modulation)($2)
	lui	$2,%hi(phy_proc_modulation)
	lw	$2,%lo(phy_proc_modulation)($2)
	beq	$2,$0,$L567
	nop

	lui	$2,%hi(phy_proc_modulation)
	lw	$2,%lo(phy_proc_modulation)($2)
	lui	$3,%hi(phy_proc_modulation_read_proc)
	addiu	$3,$3,%lo(phy_proc_modulation_read_proc)
	sw	$3,64($2)
$L567:
	lui	$2,%hi(phy_proc_dir)
	lw	$3,%lo(phy_proc_dir)($2)
	lui	$2,%hi($LC247)
	addiu	$4,$2,%lo($LC247)
	move	$5,$0
	move	$6,$3
	lui	$2,%hi(create_proc_entry)
	addiu	$2,$2,%lo(create_proc_entry)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(phy_proc_rssi)
	sw	$3,%lo(phy_proc_rssi)($2)
	lui	$2,%hi(phy_proc_rssi)
	lw	$2,%lo(phy_proc_rssi)($2)
	beq	$2,$0,$L568
	nop

	lui	$2,%hi(phy_proc_rssi)
	lw	$2,%lo(phy_proc_rssi)($2)
	lui	$3,%hi(phy_proc_rssi_read_proc)
	addiu	$3,$3,%lo(phy_proc_rssi_read_proc)
	sw	$3,64($2)
$L568:
	lui	$2,%hi(phy_proc_dir)
	lw	$3,%lo(phy_proc_dir)($2)
	lui	$2,%hi($LC248)
	addiu	$4,$2,%lo($LC248)
	move	$5,$0
	move	$6,$3
	lui	$2,%hi(create_proc_entry)
	addiu	$2,$2,%lo(create_proc_entry)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(phy_proc_mpd)
	sw	$3,%lo(phy_proc_mpd)($2)
	lui	$2,%hi(phy_proc_mpd)
	lw	$2,%lo(phy_proc_mpd)($2)
	beq	$2,$0,$L569
	nop

	lui	$2,%hi(phy_proc_mpd)
	lw	$2,%lo(phy_proc_mpd)($2)
	lui	$3,%hi(phy_proc_mpd_read_proc)
	addiu	$3,$3,%lo(phy_proc_mpd_read_proc)
	sw	$3,64($2)
$L569:
	lui	$2,%hi(phy_proc_dir)
	lw	$3,%lo(phy_proc_dir)($2)
	lui	$2,%hi($LC249)
	addiu	$4,$2,%lo($LC249)
	move	$5,$0
	move	$6,$3
	lui	$2,%hi(create_proc_entry)
	addiu	$2,$2,%lo(create_proc_entry)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(phy_proc_apd_control)
	sw	$3,%lo(phy_proc_apd_control)($2)
	lui	$2,%hi(phy_proc_apd_control)
	lw	$2,%lo(phy_proc_apd_control)($2)
	beq	$2,$0,$L570
	nop

	lui	$2,%hi(phy_proc_apd_control)
	lw	$2,%lo(phy_proc_apd_control)($2)
	lui	$3,%hi(phy_proc_apd_control_write_proc)
	addiu	$3,$3,%lo(phy_proc_apd_control_write_proc)
	sw	$3,68($2)
$L570:
	lui	$2,%hi(phy_proc_dir)
	lw	$3,%lo(phy_proc_dir)($2)
	lui	$2,%hi($LC250)
	addiu	$4,$2,%lo($LC250)
	move	$5,$0
	move	$6,$3
	lui	$2,%hi(create_proc_entry)
	addiu	$2,$2,%lo(create_proc_entry)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(phy_rogue)
	sw	$3,%lo(phy_rogue)($2)
	lui	$2,%hi(phy_rogue)
	lw	$2,%lo(phy_rogue)($2)
	beq	$2,$0,$L560
	nop

	lui	$2,%hi(phy_rogue)
	lw	$2,%lo(phy_rogue)($2)
	lui	$3,%hi(phy_rogue_read_proc)
	addiu	$3,$3,%lo(phy_rogue_read_proc)
	sw	$3,64($2)
$L560:
	move	$2,$0
	lw	$31,20($sp)
	addiu	$sp,$sp,24
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	phy_debug_init
	.size	phy_debug_init, .-phy_debug_init
	.section	.text.phy_power,"ax",@progbits
	.align	2
	.globl	phy_power
	.set	nomips16
	.set	nomicromips
	.ent	phy_power
	.type	phy_power, @function
phy_power:
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	move	$2,$4
	sb	$2,0($sp)
	move	$2,$0
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	phy_power
	.size	phy_power, .-phy_power
	.section	.text.phy_debug_deinit,"ax",@progbits
	.align	2
	.globl	phy_debug_deinit
	.set	nomips16
	.set	nomicromips
	.ent	phy_debug_deinit
	.type	phy_debug_deinit, @function
phy_debug_deinit:
	.frame	$sp,24,$31		# vars= 0, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-24
	sw	$31,20($sp)
	lui	$2,%hi(phy_rogue)
	lw	$2,%lo(phy_rogue)($2)
	beq	$2,$0,$L575
	nop

	lui	$2,%hi(phy_proc_dir)
	lw	$3,%lo(phy_proc_dir)($2)
	lui	$2,%hi($LC250)
	addiu	$4,$2,%lo($LC250)
	move	$5,$3
	lui	$2,%hi(remove_proc_entry)
	addiu	$2,$2,%lo(remove_proc_entry)
	jalr	$2
	nop

$L575:
	lui	$2,%hi(phy_proc)
	lw	$2,%lo(phy_proc)($2)
	beq	$2,$0,$L576
	nop

	lui	$2,%hi(phy_proc_dir)
	lw	$3,%lo(phy_proc_dir)($2)
	lui	$2,%hi($LC191)
	addiu	$4,$2,%lo($LC191)
	move	$5,$3
	lui	$2,%hi(remove_proc_entry)
	addiu	$2,$2,%lo(remove_proc_entry)
	jalr	$2
	nop

$L576:
	lui	$2,%hi(phy_info)
	lw	$2,%lo(phy_info)($2)
	beq	$2,$0,$L577
	nop

	lui	$2,%hi(phy_proc_dir)
	lw	$3,%lo(phy_proc_dir)($2)
	lui	$2,%hi($LC241)
	addiu	$4,$2,%lo($LC241)
	move	$5,$3
	lui	$2,%hi(remove_proc_entry)
	addiu	$2,$2,%lo(remove_proc_entry)
	jalr	$2
	nop

$L577:
	lui	$2,%hi(phy_proc_ddmi)
	lw	$2,%lo(phy_proc_ddmi)($2)
	beq	$2,$0,$L578
	nop

	lui	$2,%hi(phy_proc_dir)
	lw	$3,%lo(phy_proc_dir)($2)
	lui	$2,%hi($LC242)
	addiu	$4,$2,%lo($LC242)
	move	$5,$3
	lui	$2,%hi(remove_proc_entry)
	addiu	$2,$2,%lo(remove_proc_entry)
	jalr	$2
	nop

$L578:
	lui	$2,%hi(phy_proc_temperature)
	lw	$2,%lo(phy_proc_temperature)($2)
	beq	$2,$0,$L579
	nop

	lui	$2,%hi(phy_proc_dir)
	lw	$3,%lo(phy_proc_dir)($2)
	lui	$2,%hi($LC243)
	addiu	$4,$2,%lo($LC243)
	move	$5,$3
	lui	$2,%hi(remove_proc_entry)
	addiu	$2,$2,%lo(remove_proc_entry)
	jalr	$2
	nop

$L579:
	lui	$2,%hi(phy_proc_vcc)
	lw	$2,%lo(phy_proc_vcc)($2)
	beq	$2,$0,$L580
	nop

	lui	$2,%hi(phy_proc_dir)
	lw	$3,%lo(phy_proc_dir)($2)
	lui	$2,%hi($LC244)
	addiu	$4,$2,%lo($LC244)
	move	$5,$3
	lui	$2,%hi(remove_proc_entry)
	addiu	$2,$2,%lo(remove_proc_entry)
	jalr	$2
	nop

$L580:
	lui	$2,%hi(phy_proc_bias)
	lw	$2,%lo(phy_proc_bias)($2)
	beq	$2,$0,$L581
	nop

	lui	$2,%hi(phy_proc_dir)
	lw	$3,%lo(phy_proc_dir)($2)
	lui	$2,%hi($LC245)
	addiu	$4,$2,%lo($LC245)
	move	$5,$3
	lui	$2,%hi(remove_proc_entry)
	addiu	$2,$2,%lo(remove_proc_entry)
	jalr	$2
	nop

$L581:
	lui	$2,%hi(phy_proc_modulation)
	lw	$2,%lo(phy_proc_modulation)($2)
	beq	$2,$0,$L582
	nop

	lui	$2,%hi(phy_proc_dir)
	lw	$3,%lo(phy_proc_dir)($2)
	lui	$2,%hi($LC246)
	addiu	$4,$2,%lo($LC246)
	move	$5,$3
	lui	$2,%hi(remove_proc_entry)
	addiu	$2,$2,%lo(remove_proc_entry)
	jalr	$2
	nop

$L582:
	lui	$2,%hi(phy_proc_rssi)
	lw	$2,%lo(phy_proc_rssi)($2)
	beq	$2,$0,$L583
	nop

	lui	$2,%hi(phy_proc_dir)
	lw	$3,%lo(phy_proc_dir)($2)
	lui	$2,%hi($LC247)
	addiu	$4,$2,%lo($LC247)
	move	$5,$3
	lui	$2,%hi(remove_proc_entry)
	addiu	$2,$2,%lo(remove_proc_entry)
	jalr	$2
	nop

$L583:
	lui	$2,%hi(phy_proc_mpd)
	lw	$2,%lo(phy_proc_mpd)($2)
	beq	$2,$0,$L584
	nop

	lui	$2,%hi(phy_proc_dir)
	lw	$3,%lo(phy_proc_dir)($2)
	lui	$2,%hi($LC248)
	addiu	$4,$2,%lo($LC248)
	move	$5,$3
	lui	$2,%hi(remove_proc_entry)
	addiu	$2,$2,%lo(remove_proc_entry)
	jalr	$2
	nop

$L584:
	lui	$2,%hi(phy_proc_apd_control)
	lw	$2,%lo(phy_proc_apd_control)($2)
	beq	$2,$0,$L585
	nop

	lui	$2,%hi(phy_proc_dir)
	lw	$3,%lo(phy_proc_dir)($2)
	lui	$2,%hi($LC249)
	addiu	$4,$2,%lo($LC249)
	move	$5,$3
	lui	$2,%hi(remove_proc_entry)
	addiu	$2,$2,%lo(remove_proc_entry)
	jalr	$2
	nop

$L585:
	lui	$2,%hi($LC240)
	addiu	$4,$2,%lo($LC240)
	move	$5,$0
	lui	$2,%hi(remove_proc_entry)
	addiu	$2,$2,%lo(remove_proc_entry)
	jalr	$2
	nop

	move	$2,$0
	lw	$31,20($sp)
	addiu	$sp,$sp,24
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	phy_debug_deinit
	.size	phy_debug_deinit, .-phy_debug_deinit
	.rdata
	.align	2
$LC251:
	.ascii	"[%lu]0ms[%s:%d]\012  PON PHY set txPowerEnFlag to %d.\012"
	.ascii	"\000"
	.section	.text.phy_tx_power_config,"ax",@progbits
	.align	2
	.globl	phy_tx_power_config
	.set	nomips16
	.set	nomicromips
	.ent	phy_tx_power_config
	.type	phy_tx_power_config, @function
phy_tx_power_config:
	.frame	$sp,32,$31		# vars= 0, regs= 2/0, args= 24, gp= 0
	.mask	0x80010000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-32
	sw	$31,28($sp)
	sw	$16,24($sp)
	move	$2,$4
	sb	$2,32($sp)
	lui	$2,%hi(gpPhyPriv)
	lw	$3,%lo(gpPhyPriv)($2)
	lbu	$2,32($sp)
	andi	$2,$2,0x1
	andi	$4,$2,0x00ff
	lbu	$2,132($3)
	ins	$2,$4,7,1
	sb	$2,132($3)
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,136($2)
	andi	$2,$2,0x10
	beq	$2,$0,$L587
	nop

	lui	$2,%hi(jiffies)
	lw	$16,%lo(jiffies)($2)
	lui	$2,%hi($LC2)
	addiu	$4,$2,%lo($LC2)
	li	$5,47			# 0x2f
	lui	$2,%hi(strrchr)
	addiu	$2,$2,%lo(strrchr)
	jalr	$2
	nop

	addiu	$3,$2,1
	lbu	$2,32($sp)
	sw	$2,16($sp)
	lui	$2,%hi($LC251)
	addiu	$4,$2,%lo($LC251)
	move	$5,$16
	move	$6,$3
	li	$7,2557			# 0x9fd
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L587:
	lw	$31,28($sp)
	lw	$16,24($sp)
	addiu	$sp,$sp,32
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	phy_tx_power_config
	.size	phy_tx_power_config, .-phy_tx_power_config
	.section	.text.los_status_read_proc,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	los_status_read_proc
	.type	los_status_read_proc, @function
los_status_read_proc:
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
	sw	$0,20($sp)
	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$2,$2,0x5e0
	sw	$2,24($sp)
	lw	$2,24($sp)
	lw	$2,0($2)
	sw	$2,28($sp)
	lw	$2,24($sp)
	lw	$2,0($2)
	sw	$2,28($sp)
	lw	$2,28($sp)
	sw	$2,20($sp)
	lw	$2,20($sp)
	andi	$2,$2,0x1
	beq	$2,$0,$L591
	nop

	lui	$2,%hi(los_status)
	sw	$0,%lo(los_status)($2)
	j	$L592
	nop

$L591:
	lui	$2,%hi(los_status)
	li	$3,1			# 0x1
	sw	$3,%lo(los_status)($2)
$L592:
	lui	$2,%hi(los_status)
	lw	$3,%lo(los_status)($2)
	lw	$4,40($sp)
	lui	$2,%hi($LC77)
	addiu	$5,$2,%lo($LC77)
	move	$6,$3
	lui	$2,%hi(sprintf)
	addiu	$2,$2,%lo(sprintf)
	jalr	$2
	nop

	sw	$2,16($sp)
	lw	$3,16($sp)
	lw	$2,48($sp)
	subu	$2,$3,$2
	sw	$2,16($sp)
	lw	$2,48($sp)
	lw	$3,40($sp)
	addu	$3,$3,$2
	lw	$2,44($sp)
	sw	$3,0($2)
	lw	$3,16($sp)
	lw	$2,52($sp)
	slt	$2,$2,$3
	beq	$2,$0,$L593
	nop

	lw	$2,52($sp)
	sw	$2,16($sp)
	j	$L594
	nop

$L593:
	lw	$2,56($sp)
	li	$3,1			# 0x1
	sw	$3,0($2)
$L594:
	lw	$2,16($sp)
	bgez	$2,$L595
	nop

	sw	$0,16($sp)
$L595:
	lw	$2,16($sp)
	lw	$31,36($sp)
	addiu	$sp,$sp,40
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	los_status_read_proc
	.size	los_status_read_proc, .-los_status_read_proc
	.section	.text.los_status_write_proc,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	los_status_write_proc
	.type	los_status_write_proc, @function
los_status_write_proc:
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	sw	$4,0($sp)
	sw	$5,4($sp)
	sw	$6,8($sp)
	sw	$7,12($sp)
	move	$2,$0
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	los_status_write_proc
	.size	los_status_write_proc, .-los_status_write_proc
	.rdata
	.align	2
$LC252:
	.ascii	"tc3162/los_status\000"
	.align	2
$LC253:
	.ascii	"\012create los_proc proc fail.\000"
	.section	.text.los_status_init,"ax",@progbits
	.align	2
	.globl	los_status_init
	.set	nomips16
	.set	nomicromips
	.ent	los_status_init
	.type	los_status_init, @function
los_status_init:
	.frame	$sp,32,$31		# vars= 8, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-32
	sw	$31,28($sp)
	sw	$0,16($sp)
	lui	$2,%hi($LC252)
	addiu	$4,$2,%lo($LC252)
	move	$5,$0
	move	$6,$0
	lui	$2,%hi(create_proc_entry)
	addiu	$2,$2,%lo(create_proc_entry)
	jalr	$2
	nop

	sw	$2,16($sp)
	lw	$2,16($sp)
	bne	$2,$0,$L600
	nop

	lui	$2,%hi($LC253)
	addiu	$4,$2,%lo($LC253)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	li	$2,-1			# 0xffffffffffffffff
	j	$L601
	nop

$L600:
	lw	$2,16($sp)
	lui	$3,%hi(los_status_read_proc)
	addiu	$3,$3,%lo(los_status_read_proc)
	sw	$3,64($2)
	lw	$2,16($sp)
	lui	$3,%hi(los_status_write_proc)
	addiu	$3,$3,%lo(los_status_write_proc)
	sw	$3,68($2)
	move	$2,$0
$L601:
	lw	$31,28($sp)
	addiu	$sp,$sp,32
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	los_status_init
	.size	los_status_init, .-los_status_init
	.section	.text.phy_tx_fault_reset,"ax",@progbits
	.align	2
	.globl	phy_tx_fault_reset
	.set	nomips16
	.set	nomicromips
	.ent	phy_tx_fault_reset
	.type	phy_tx_fault_reset, @function
phy_tx_fault_reset:
	.frame	$sp,24,$31		# vars= 0, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-24
	sw	$31,20($sp)
	lui	$2,%hi(mt7570_select)
	lw	$3,%lo(mt7570_select)($2)
	li	$2,1			# 0x1
	bne	$3,$2,$L602
	nop

	lui	$2,%hi(mt7570_safe_circuit_reset)
	addiu	$2,$2,%lo(mt7570_safe_circuit_reset)
	jalr	$2
	nop

$L602:
	lw	$31,20($sp)
	addiu	$sp,$sp,24
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	phy_tx_fault_reset
	.size	phy_tx_fault_reset, .-phy_tx_fault_reset
	.rdata
	.align	2
$LC254:
	.ascii	"[%lu0ms]%s:%d los detected by polling\012\000"
	.align	2
$LC255:
	.ascii	"[%lu0ms]%s:%d [%s] event los polled\012\000"
	.align	2
$LC256:
	.ascii	"[%lu0ms]%s:%d ready detected by polling\012\000"
	.align	2
$LC257:
	.ascii	"[%lu0ms]%s:%d [%s] event ready polled\012\000"
	.section	.text.phy_event_poll,"ax",@progbits
	.align	2
	.globl	phy_event_poll
	.set	nomips16
	.set	nomicromips
	.ent	phy_event_poll
	.type	phy_event_poll, @function
phy_event_poll:
	.frame	$sp,72,$31		# vars= 40, regs= 1/0, args= 24, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-72
	sw	$31,68($sp)
	sw	$4,72($sp)
	li	$2,1			# 0x1
	sw	$2,40($sp)
	sw	$0,44($sp)
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lbu	$2,138($2)
	beq	$2,$0,$L604
	nop

	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	addiu	$2,$2,168
	sw	$2,28($sp)
	lw	$2,28($sp)
	move	$4,$2
	lui	$2,%hi(_raw_spin_lock_irqsave)
	addiu	$2,$2,%lo(_raw_spin_lock_irqsave)
	jalr	$2
	nop

	sw	$2,24($sp)
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lw	$3,172($2)
	li	$2,1			# 0x1
	bne	$3,$2,$L608
	nop

	lui	$2,%hi(phy_ready_status)
	addiu	$2,$2,%lo(phy_ready_status)
	jalr	$2
	nop

	move	$3,$2
	li	$2,1			# 0x1
	bne	$3,$2,$L609
	nop

	lui	$2,%hi(phy_los_status)
	addiu	$2,$2,%lo(phy_los_status)
	jalr	$2
	nop

	bne	$2,$0,$L609
	nop

	li	$2,1			# 0x1
	j	$L611
	nop

$L609:
	move	$2,$0
$L611:
	bne	$2,$0,$L615
	nop

	lui	$2,%hi(xpon_phy_print_open)
	lw	$2,%lo(xpon_phy_print_open)($2)
	beq	$2,$0,$L613
	nop

	lui	$2,%hi(jiffies)
	lw	$3,%lo(jiffies)($2)
	lui	$2,%hi($LC254)
	addiu	$4,$2,%lo($LC254)
	move	$5,$3
	lui	$2,%hi(__func__.25485)
	addiu	$6,$2,%lo(__func__.25485)
	li	$7,2668			# 0xa6c
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L613:
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	sw	$0,172($2)
	lui	$2,%hi(xpon_phy_print_open)
	lw	$2,%lo(xpon_phy_print_open)($2)
	beq	$2,$0,$L614
	nop

	lui	$2,%hi(jiffies)
	lw	$3,%lo(jiffies)($2)
	lui	$2,%hi(__func__.25485)
	addiu	$2,$2,%lo(__func__.25485)
	sw	$2,16($sp)
	lui	$2,%hi($LC255)
	addiu	$4,$2,%lo($LC255)
	move	$5,$3
	lui	$2,%hi(__func__.25485)
	addiu	$6,$2,%lo(__func__.25485)
	li	$7,2670			# 0xa6e
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L614:
	sw	$0,44($sp)
	addiu	$2,$sp,40
	move	$4,$2
	lui	$2,%hi(phy_event_handler)
	addiu	$2,$2,%lo(phy_event_handler)
	jalr	$2
	nop

	j	$L615
	nop

$L608:
	lui	$2,%hi(phy_ready_status)
	addiu	$2,$2,%lo(phy_ready_status)
	jalr	$2
	nop

	move	$3,$2
	li	$2,1			# 0x1
	bne	$3,$2,$L616
	nop

	lui	$2,%hi(phy_los_status)
	addiu	$2,$2,%lo(phy_los_status)
	jalr	$2
	nop

	bne	$2,$0,$L616
	nop

	li	$2,1			# 0x1
	j	$L618
	nop

$L616:
	move	$2,$0
$L618:
	beq	$2,$0,$L615
	nop

	lui	$2,%hi(xpon_phy_print_open)
	lw	$2,%lo(xpon_phy_print_open)($2)
	beq	$2,$0,$L619
	nop

	lui	$2,%hi(jiffies)
	lw	$3,%lo(jiffies)($2)
	lui	$2,%hi($LC256)
	addiu	$4,$2,%lo($LC256)
	move	$5,$3
	lui	$2,%hi(__func__.25485)
	addiu	$6,$2,%lo(__func__.25485)
	li	$7,2679			# 0xa77
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L619:
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	li	$3,1			# 0x1
	sw	$3,172($2)
	lui	$2,%hi(xpon_phy_print_open)
	lw	$2,%lo(xpon_phy_print_open)($2)
	beq	$2,$0,$L620
	nop

	lui	$2,%hi(jiffies)
	lw	$3,%lo(jiffies)($2)
	lui	$2,%hi(__func__.25485)
	addiu	$2,$2,%lo(__func__.25485)
	sw	$2,16($sp)
	lui	$2,%hi($LC257)
	addiu	$4,$2,%lo($LC257)
	move	$5,$3
	lui	$2,%hi(__func__.25485)
	addiu	$6,$2,%lo(__func__.25485)
	li	$7,2681			# 0xa79
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L620:
	li	$2,5			# 0x5
	sw	$2,44($sp)
	addiu	$2,$sp,40
	move	$4,$2
	lui	$2,%hi(phy_event_handler)
	addiu	$2,$2,%lo(phy_event_handler)
	jalr	$2
	nop

$L615:
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	addiu	$2,$2,168
	sw	$2,32($sp)
	lw	$2,24($sp)
	sw	$2,36($sp)
	lw	$2,32($sp)
	move	$4,$2
	lw	$5,36($sp)
	lui	$2,%hi(_raw_spin_unlock_irqrestore)
	addiu	$2,$2,%lo(_raw_spin_unlock_irqrestore)
	jalr	$2
	nop

	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	addiu	$4,$2,140
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lw	$2,160($2)
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

$L604:
	lw	$31,68($sp)
	addiu	$sp,$sp,72
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	phy_event_poll
	.size	phy_event_poll, .-phy_event_poll
	.rdata
	.align	2
$LC258:
	.ascii	"[%lu0ms]%s:%d [%s] event:%d, src:%d\012\000"
	.align	2
$LC259:
	.ascii	"[%s] event %d still left for ISR to handle\012\000"
	.section	.text.phy_event_handler,"ax",@progbits
	.align	2
	.globl	phy_event_handler
	.set	nomips16
	.set	nomicromips
	.ent	phy_event_handler
	.type	phy_event_handler, @function
phy_event_handler:
	.frame	$sp,56,$31		# vars= 16, regs= 1/0, args= 32, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-56
	sw	$31,52($sp)
	sw	$4,56($sp)
	lui	$2,%hi(xpon_phy_print_open)
	lw	$2,%lo(xpon_phy_print_open)($2)
	beq	$2,$0,$L622
	nop

	lui	$2,%hi(jiffies)
	lw	$5,%lo(jiffies)($2)
	lw	$2,56($sp)
	lw	$3,4($2)
	lw	$2,56($sp)
	lw	$2,0($2)
	lui	$4,%hi(__func__.25489)
	addiu	$4,$4,%lo(__func__.25489)
	sw	$4,16($sp)
	sw	$3,20($sp)
	sw	$2,24($sp)
	lui	$2,%hi($LC258)
	addiu	$4,$2,%lo($LC258)
	lui	$2,%hi(__func__.25489)
	addiu	$6,$2,%lo(__func__.25489)
	li	$7,2694			# 0xa86
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L622:
	lw	$2,56($sp)
	lw	$2,4($2)
	sltu	$3,$2,9
	beq	$3,$0,$L623
	nop

	sll	$3,$2,2
	lui	$2,%hi($L625)
	addiu	$2,$2,%lo($L625)
	addu	$2,$3,$2
	lw	$2,0($2)
	j	$2
	nop

	.rdata
	.align	2
	.align	2
$L625:
	.word	$L624
	.word	$L634
	.word	$L634
	.word	$L634
	.word	$L634
	.word	$L630
	.word	$L624
	.word	$L634
	.word	$L624
	.section	.text.phy_event_handler
$L624:
	lui	$2,%hi(phy_los_handler)
	addiu	$2,$2,%lo(phy_los_handler)
	jalr	$2
	nop

	j	$L632
	nop

$L630:
	lui	$2,%hi(phy_ready_handler)
	addiu	$2,$2,%lo(phy_ready_handler)
	jalr	$2
	nop

	j	$L632
	nop

$L623:
	lw	$2,56($sp)
	lw	$3,4($2)
	lui	$2,%hi($LC259)
	addiu	$4,$2,%lo($LC259)
	lui	$2,%hi(__func__.25489)
	addiu	$5,$2,%lo(__func__.25489)
	move	$6,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	j	$L632
	nop

$L634:
	nop
$L632:
	lw	$2,56($sp)
	sw	$2,32($sp)
	sw	$0,36($sp)
	sw	$0,40($sp)
	sw	$0,44($sp)
	sw	$0,36($sp)
	lw	$2,32($sp)
	sw	$2,40($sp)
	addiu	$2,$sp,36
	li	$4,24			# 0x18
	move	$5,$0
	move	$6,$2
	lui	$2,%hi(__ECNT_HOOK)
	addiu	$2,$2,%lo(__ECNT_HOOK)
	jalr	$2
	nop

	move	$3,$2
	li	$2,-1			# 0xffffffffffffffff
	bne	$3,$2,$L621
	nop

	lw	$2,32($sp)
	lw	$3,4($2)
	lui	$2,%hi($LC135)
	addiu	$4,$2,%lo($LC135)
	move	$5,$3
	lui	$2,%hi(__func__.24631)
	addiu	$6,$2,%lo(__func__.24631)
	li	$7,93			# 0x5d
	lui	$2,%hi(panic)
	addiu	$2,$2,%lo(panic)
	jalr	$2
	nop

$L621:
	lw	$31,52($sp)
	addiu	$sp,$sp,56
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	phy_event_handler
	.size	phy_event_handler, .-phy_event_handler
	.local	vendor_id.25118
	.comm	vendor_id.25118,17,4
	.local	vendor_pn.25119
	.comm	vendor_pn.25119,17,4
	.rdata
	.align	2
	.type	__func__.24637, @object
	.size	__func__.24637, 20
__func__.24637:
	.ascii	"REPORT_EVENT_TO_MAC\000"
	.align	2
	.type	__func__.25485, @object
	.size	__func__.25485, 15
__func__.25485:
	.ascii	"phy_event_poll\000"
	.align	2
	.type	__func__.25489, @object
	.size	__func__.25489, 18
__func__.25489:
	.ascii	"phy_event_handler\000"
	.align	2
	.type	__func__.24631, @object
	.size	__func__.24631, 23
__func__.24631:
	.ascii	"XPON_MAC_EVENT_HANDLER\000"
	.ident	"GCC: (Buildroot 2015.08.1) 4.9.3"
