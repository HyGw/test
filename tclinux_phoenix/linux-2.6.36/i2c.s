	.file	1 "i2c.c"
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
	.local	i2cMasterBaseAddr
	.comm	i2cMasterBaseAddr,4,4
	.section	.text.ioWritePhyReg8,"ax",@progbits
	.align	2
	.globl	ioWritePhyReg8
	.set	nomips16
	.set	nomicromips
	.ent	ioWritePhyReg8
	.type	ioWritePhyReg8, @function
ioWritePhyReg8:
	.frame	$sp,32,$31		# vars= 8, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-32
	sw	$31,28($sp)
	move	$3,$4
	move	$2,$5
	sh	$3,32($sp)
	sb	$2,36($sp)
	lhu	$2,32($sp)
	srl	$2,$2,8
	andi	$2,$2,0xffff
	andi	$2,$2,0x00ff
	sb	$2,16($sp)
	lhu	$2,32($sp)
	andi	$2,$2,0x00ff
	sb	$2,17($sp)
	lbu	$2,36($sp)
	sb	$2,18($sp)
	li	$4,96			# 0x60
	li	$5,3			# 0x3
	addiu	$2,$sp,16
	move	$6,$2
	lui	$2,%hi(i2c_write_data)
	addiu	$2,$2,%lo(i2c_write_data)
	jalr	$2
	nop

	lw	$31,28($sp)
	addiu	$sp,$sp,32
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	ioWritePhyReg8
	.size	ioWritePhyReg8, .-ioWritePhyReg8
	.section	.text.ioReadPhyReg32,"ax",@progbits
	.align	2
	.globl	ioReadPhyReg32
	.set	nomips16
	.set	nomicromips
	.ent	ioReadPhyReg32
	.type	ioReadPhyReg32, @function
ioReadPhyReg32:
	.frame	$sp,40,$31		# vars= 16, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-40
	sw	$31,36($sp)
	move	$2,$4
	sh	$2,40($sp)
	sw	$0,16($sp)
	lhu	$2,40($sp)
	srl	$2,$2,8
	andi	$2,$2,0xffff
	andi	$2,$2,0x00ff
	sb	$2,28($sp)
	lhu	$2,40($sp)
	andi	$2,$2,0x00ff
	sb	$2,29($sp)
	addiu	$2,$sp,28
	li	$4,96			# 0x60
	li	$5,2			# 0x2
	li	$6,4			# 0x4
	move	$7,$2
	lui	$2,%hi(i2c_write_read_data)
	addiu	$2,$2,%lo(i2c_write_read_data)
	jalr	$2
	nop

	sw	$2,24($sp)
	lw	$2,24($sp)
	bne	$2,$0,$L4
	nop

	sw	$0,20($sp)
	j	$L5
	nop

$L6:
	lw	$2,20($sp)
	addiu	$3,$sp,16
	addu	$2,$3,$2
	lbu	$2,12($2)
	move	$3,$2
	lw	$2,20($sp)
	sll	$2,$2,3
	sll	$2,$3,$2
	move	$3,$2
	lw	$2,16($sp)
	or	$2,$2,$3
	sw	$2,16($sp)
	lw	$2,20($sp)
	addiu	$2,$2,1
	sw	$2,20($sp)
$L5:
	lw	$2,20($sp)
	slt	$2,$2,4
	bne	$2,$0,$L6
	nop

$L4:
	lw	$2,16($sp)
	lw	$31,36($sp)
	addiu	$sp,$sp,40
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	ioReadPhyReg32
	.size	ioReadPhyReg32, .-ioReadPhyReg32
	.section	.text.ioWritePhyReg32,"ax",@progbits
	.align	2
	.globl	ioWritePhyReg32
	.set	nomips16
	.set	nomicromips
	.ent	ioWritePhyReg32
	.type	ioWritePhyReg32, @function
ioWritePhyReg32:
	.frame	$sp,40,$31		# vars= 16, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-40
	sw	$31,36($sp)
	move	$2,$4
	sw	$5,44($sp)
	sh	$2,40($sp)
	lhu	$2,40($sp)
	srl	$2,$2,8
	andi	$2,$2,0xffff
	andi	$2,$2,0x00ff
	sb	$2,20($sp)
	lhu	$2,40($sp)
	andi	$2,$2,0x00ff
	sb	$2,21($sp)
	sw	$0,16($sp)
	j	$L9
	nop

$L10:
	lw	$2,16($sp)
	addiu	$2,$2,2
	lw	$3,16($sp)
	sll	$3,$3,3
	lw	$4,44($sp)
	srl	$3,$4,$3
	andi	$3,$3,0x00ff
	addiu	$4,$sp,16
	addu	$2,$4,$2
	sb	$3,4($2)
	lw	$2,16($sp)
	addiu	$2,$2,1
	sw	$2,16($sp)
$L9:
	lw	$2,16($sp)
	slt	$2,$2,4
	bne	$2,$0,$L10
	nop

	addiu	$2,$sp,20
	li	$4,96			# 0x60
	li	$5,6			# 0x6
	move	$6,$2
	lui	$2,%hi(i2c_write_data)
	addiu	$2,$2,%lo(i2c_write_data)
	jalr	$2
	nop

	lw	$31,36($sp)
	addiu	$sp,$sp,40
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	ioWritePhyReg32
	.size	ioWritePhyReg32, .-ioWritePhyReg32
	.rdata
	.align	2
$LC0:
	.ascii	"CSR: 0004 Slave Address Register: \011\011%.4x\012\000"
	.align	2
$LC1:
	.ascii	"CSR: 0010 Control Register: \011\011\011%.4x\012\000"
	.align	2
$LC2:
	.ascii	"CSR: 0014 Transfer Length Register: \011%.4x\012\000"
	.align	2
$LC3:
	.ascii	"CSR: 0018 Translation Length Register: \011%.4x\012\000"
	.align	2
$LC4:
	.ascii	"CSR: 001C Delay Register: \011\011\011\011%.4x\012\000"
	.align	2
$LC5:
	.ascii	"CSR: 0020 Timeing Control Register: \011%.4x\012\000"
	.align	2
$LC6:
	.ascii	"CSR: 0030 FIFO Status Register: \011\011%.4x\012\000"
	.align	2
$LC7:
	.ascii	"CSR: 0040 IO Config Register:\011\011\011%.4x\012\000"
	.align	2
$LC8:
	.ascii	"CSR: 0048 High Speed Mode Register:\011\011%.4x\012\000"
	.align	2
$LC9:
	.ascii	"CSR: 0064 Debug Status Register:\011\011%.4x\012\000"
	.align	2
$LC10:
	.ascii	"CSR: 0064 Debug Control Register:\011\011%.4x\012\000"
	.section	.text.__print_csr,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	__print_csr
	.type	__print_csr, @function
__print_csr:
	.frame	$sp,32,$31		# vars= 8, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-32
	sw	$31,28($sp)
	lui	$2,%hi(i2cMasterBaseAddr)
	lw	$2,%lo(i2cMasterBaseAddr)($2)
	sw	$2,16($sp)
	lw	$2,16($sp)
	addiu	$2,$2,4
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi($LC0)
	addiu	$4,$2,%lo($LC0)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lw	$2,16($sp)
	addiu	$2,$2,16
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi($LC1)
	addiu	$4,$2,%lo($LC1)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lw	$2,16($sp)
	addiu	$2,$2,20
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi($LC2)
	addiu	$4,$2,%lo($LC2)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lw	$2,16($sp)
	addiu	$2,$2,24
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi($LC3)
	addiu	$4,$2,%lo($LC3)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lw	$2,16($sp)
	addiu	$2,$2,28
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi($LC4)
	addiu	$4,$2,%lo($LC4)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lw	$2,16($sp)
	addiu	$2,$2,32
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi($LC5)
	addiu	$4,$2,%lo($LC5)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lw	$2,16($sp)
	addiu	$2,$2,48
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi($LC6)
	addiu	$4,$2,%lo($LC6)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lw	$2,16($sp)
	addiu	$2,$2,64
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi($LC7)
	addiu	$4,$2,%lo($LC7)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lw	$2,16($sp)
	addiu	$2,$2,72
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi($LC8)
	addiu	$4,$2,%lo($LC8)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lw	$2,16($sp)
	addiu	$2,$2,100
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi($LC9)
	addiu	$4,$2,%lo($LC9)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lw	$2,16($sp)
	addiu	$2,$2,104
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi($LC10)
	addiu	$4,$2,%lo($LC10)
	move	$5,$3
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
	.end	__print_csr
	.size	__print_csr, .-__print_csr
	.section	.text.i2c_write_read_data,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	i2c_write_read_data
	.type	i2c_write_read_data, @function
i2c_write_read_data:
	.frame	$sp,56,$31		# vars= 32, regs= 2/0, args= 16, gp= 0
	.mask	0x80010000,-4
	.fmask	0x00000000,0
	addiu	$sp,$sp,-56
	sw	$31,52($sp)
	sw	$16,48($sp)
	move	$2,$4
	sw	$5,60($sp)
	sw	$6,64($sp)
	sw	$7,68($sp)
	sb	$2,56($sp)
	lui	$2,%hi(i2cMasterBaseAddr)
	lw	$2,%lo(i2cMasterBaseAddr)($2)
	sw	$2,28($sp)
	sw	$0,16($sp)
	li	$2,65535			# 0xffff
	sw	$2,24($sp)
	lbu	$2,56($sp)
	sll	$2,$2,1
	sb	$2,56($sp)
	lbu	$3,56($sp)
	lw	$2,28($sp)
	addiu	$2,$2,4
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	lw	$2,28($sp)
	addiu	$2,$2,20
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	move	$3,$2
	li	$2,-7937			# 0xffffffffffffe0ff
	and	$2,$3,$2
	sw	$2,32($sp)
	lw	$2,64($sp)
	sll	$2,$2,8
	andi	$2,$2,0x1f00
	lw	$3,32($sp)
	or	$2,$3,$2
	sw	$2,32($sp)
	lw	$2,28($sp)
	addiu	$2,$2,20
	lw	$4,32($sp)
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	lw	$2,28($sp)
	addiu	$2,$2,20
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	move	$3,$2
	li	$2,-256			# 0xffffffffffffff00
	and	$2,$3,$2
	sw	$2,36($sp)
	lw	$2,60($sp)
	andi	$2,$2,0xff
	lw	$3,36($sp)
	or	$2,$3,$2
	sw	$2,36($sp)
	lw	$2,28($sp)
	addiu	$2,$2,20
	lw	$4,36($sp)
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	lw	$2,28($sp)
	addiu	$2,$2,24
	li	$4,2			# 0x2
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	lw	$2,28($sp)
	addiu	$2,$2,56
	li	$4,1			# 0x1
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	lw	$2,28($sp)
	addiu	$2,$2,16
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	move	$3,$2
	li	$2,-127			# 0xffffffffffffff81
	and	$2,$3,$2
	sw	$2,40($sp)
	lw	$2,40($sp)
	ori	$2,$2,0x3a
	sw	$2,40($sp)
	lw	$2,28($sp)
	addiu	$2,$2,16
	lw	$4,40($sp)
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	sw	$0,20($sp)
	j	$L15
$L16:
	lw	$2,20($sp)
	lw	$3,68($sp)
	addu	$2,$3,$2
	lbu	$2,0($2)
	move	$3,$2
	lw	$2,28($sp)
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	lw	$2,20($sp)
	addiu	$2,$2,1
	sw	$2,20($sp)
$L15:
	lw	$3,20($sp)
	lw	$2,60($sp)
	sltu	$2,$3,$2
	bne	$2,$0,$L16
#APP
 # 116 "/opt/tclinux_phoenix/modules/private/xpon_phy/src/i2c.c" 1
	.set	push
	.set	noreorder
	.set	mips2
	sync
	.set	pop
 # 0 "" 2
#NO_APP
	lw	$2,28($sp)
	addiu	$2,$2,36
	li	$4,1			# 0x1
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
$L22:
	lw	$2,28($sp)
	addiu	$2,$2,12
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	sh	$2,44($sp)
	lhu	$2,44($sp)
	andi	$2,$2,0x1
	beq	$2,$0,$L17
	j	$L18
$L17:
	lhu	$2,44($sp)
	andi	$2,$2,0x4
	beq	$2,$0,$L19
	li	$2,-14			# 0xfffffffffffffff2
	sw	$2,16($sp)
	j	$L18
$L19:
	lhu	$2,44($sp)
	andi	$2,$2,0x2
	beq	$2,$0,$L20
	li	$2,-14			# 0xfffffffffffffff2
	sw	$2,16($sp)
	j	$L18
$L20:
	lw	$2,24($sp)
	bne	$2,$0,$L21
	li	$2,-14			# 0xfffffffffffffff2
	sw	$2,16($sp)
	j	$L18
$L21:
	lw	$2,24($sp)
	addiu	$2,$2,-1
	sw	$2,24($sp)
	j	$L22
$L18:
	lw	$2,28($sp)
	addiu	$2,$2,12
	li	$4,7			# 0x7
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	lw	$2,16($sp)
	bne	$2,$0,$L23
	sw	$0,20($sp)
	j	$L24
$L25:
	lw	$2,20($sp)
	lw	$3,68($sp)
	addu	$16,$3,$2
	lw	$2,28($sp)
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	andi	$2,$2,0x00ff
	sb	$2,0($16)
	lw	$2,20($sp)
	addiu	$2,$2,1
	sw	$2,20($sp)
$L24:
	lw	$2,20($sp)
	slt	$2,$2,4
	bne	$2,$0,$L25
$L23:
	lw	$2,16($sp)
	lw	$31,52($sp)
	lw	$16,48($sp)
	addiu	$sp,$sp,56
	j	$31
	.end	i2c_write_read_data
	.size	i2c_write_read_data, .-i2c_write_read_data
	.section	.text.i2c_write_data,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	i2c_write_data
	.type	i2c_write_data, @function
i2c_write_data:
	.frame	$sp,56,$31		# vars= 32, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	addiu	$sp,$sp,-56
	sw	$31,52($sp)
	move	$2,$4
	sw	$5,60($sp)
	sw	$6,64($sp)
	sb	$2,56($sp)
	lui	$2,%hi(i2cMasterBaseAddr)
	lw	$2,%lo(i2cMasterBaseAddr)($2)
	sw	$2,28($sp)
	li	$2,65535			# 0xffff
	sw	$2,24($sp)
	lbu	$2,56($sp)
	sll	$2,$2,1
	sb	$2,56($sp)
	lbu	$3,56($sp)
	lw	$2,28($sp)
	addiu	$2,$2,4
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	lw	$2,28($sp)
	addiu	$2,$2,20
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	move	$3,$2
	li	$2,-7937			# 0xffffffffffffe0ff
	and	$2,$3,$2
	sw	$2,32($sp)
	lw	$2,32($sp)
	ori	$2,$2,0x100
	sw	$2,32($sp)
	lw	$2,28($sp)
	addiu	$2,$2,20
	lw	$4,32($sp)
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	lw	$2,28($sp)
	addiu	$2,$2,20
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	move	$3,$2
	li	$2,-256			# 0xffffffffffffff00
	and	$2,$3,$2
	sw	$2,36($sp)
	lw	$2,60($sp)
	andi	$2,$2,0xff
	lw	$3,36($sp)
	or	$2,$3,$2
	sw	$2,36($sp)
	lw	$2,28($sp)
	addiu	$2,$2,20
	lw	$4,36($sp)
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	lw	$2,28($sp)
	addiu	$2,$2,24
	li	$4,1			# 0x1
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	lw	$2,28($sp)
	addiu	$2,$2,56
	li	$4,1			# 0x1
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	lw	$2,28($sp)
	addiu	$2,$2,16
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	move	$3,$2
	li	$2,-127			# 0xffffffffffffff81
	and	$2,$3,$2
	sw	$2,40($sp)
	lw	$2,40($sp)
	ori	$2,$2,0x28
	sw	$2,40($sp)
	lw	$2,28($sp)
	addiu	$2,$2,16
	lw	$4,40($sp)
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	sw	$0,20($sp)
	j	$L28
$L29:
	lw	$2,20($sp)
	lw	$3,64($sp)
	addu	$2,$3,$2
	lbu	$2,0($2)
	move	$3,$2
	lw	$2,28($sp)
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	lw	$2,20($sp)
	addiu	$2,$2,1
	sw	$2,20($sp)
$L28:
	lw	$3,20($sp)
	lw	$2,60($sp)
	sltu	$2,$3,$2
	bne	$2,$0,$L29
#APP
 # 173 "/opt/tclinux_phoenix/modules/private/xpon_phy/src/i2c.c" 1
	.set	push
	.set	noreorder
	.set	mips2
	sync
	.set	pop
 # 0 "" 2
#NO_APP
	lw	$2,28($sp)
	addiu	$2,$2,36
	li	$4,1			# 0x1
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
$L35:
	lw	$2,28($sp)
	addiu	$2,$2,12
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	sh	$2,44($sp)
	lhu	$2,44($sp)
	andi	$2,$2,0x1
	beq	$2,$0,$L30
	j	$L31
$L30:
	lhu	$2,44($sp)
	andi	$2,$2,0x4
	beq	$2,$0,$L32
	li	$2,-14			# 0xfffffffffffffff2
	sw	$2,16($sp)
	j	$L31
$L32:
	lhu	$2,44($sp)
	andi	$2,$2,0x2
	beq	$2,$0,$L33
	li	$2,-14			# 0xfffffffffffffff2
	sw	$2,16($sp)
	j	$L31
$L33:
	lw	$2,24($sp)
	bne	$2,$0,$L34
	li	$2,-14			# 0xfffffffffffffff2
	sw	$2,16($sp)
	j	$L31
$L34:
	lw	$2,24($sp)
	addiu	$2,$2,-1
	sw	$2,24($sp)
	j	$L35
$L31:
	lw	$2,28($sp)
	addiu	$2,$2,12
	li	$4,7			# 0x7
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	lw	$2,16($sp)
	lw	$31,52($sp)
	addiu	$sp,$sp,56
	j	$31
	.end	i2c_write_data
	.size	i2c_write_data, .-i2c_write_data
	.rdata
	.align	2
$LC11:
	.ascii	"%s %x %x\000"
	.align	2
$LC12:
	.ascii	"read\000"
	.align	2
$LC13:
	.ascii	"Read REG:%.4x: %.8x\012\000"
	.align	2
$LC14:
	.ascii	"write\000"
	.align	2
$LC15:
	.ascii	"show\000"
	.section	.text.i2c_write_proc,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	i2c_write_proc
	.type	i2c_write_proc, @function
i2c_write_proc:
	.frame	$sp,200,$31		# vars= 168, regs= 2/0, args= 24, gp= 0
	.mask	0x80010000,-4
	.fmask	0x00000000,0
	addiu	$sp,$sp,-200
	sw	$31,196($sp)
	sw	$16,192($sp)
	sw	$4,200($sp)
	sw	$5,204($sp)
	sw	$6,208($sp)
	sw	$7,212($sp)
	lw	$2,208($sp)
	sltu	$2,$2,64
	bne	$2,$0,$L38
	li	$2,-22			# 0xffffffffffffffea
	j	$L48
$L38:
	addiu	$2,$sp,88
	sw	$2,28($sp)
	lw	$2,204($sp)
	sw	$2,32($sp)
	lw	$2,208($sp)
	sw	$2,24($sp)
	lw	$2,32($sp)
	sw	$2,36($sp)
	lw	$2,24($sp)
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
	beq	$2,$0,$L40
	lw	$4,28($sp)
	lw	$5,32($sp)
	lw	$6,24($sp)
#APP
 # 208 "/opt/tclinux_phoenix/modules/private/xpon_phy/src/i2c.c" 1
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
$L40:
	lw	$2,24($sp)
	beq	$2,$0,$L41
	li	$2,-14			# 0xfffffffffffffff2
	j	$L48
$L41:
	addiu	$4,$sp,88
	addiu	$6,$sp,152
	addiu	$3,$sp,184
	addiu	$2,$sp,188
	sw	$2,16($sp)
	lui	$2,%hi($LC11)
	addiu	$5,$2,%lo($LC11)
	move	$7,$3
	lui	$2,%hi(sscanf)
	addiu	$2,$2,%lo(sscanf)
	jalr	$2
	addiu	$2,$sp,152
	sw	$2,52($sp)
	lui	$2,%hi($LC12)
	addiu	$2,$2,%lo($LC12)
	sw	$2,56($sp)
	lw	$3,52($sp)
	lw	$2,56($sp)
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
	sw	$3,52($sp)
	sw	$2,56($sp)
	sw	$4,60($sp)
	lw	$2,60($sp)
	bne	$2,$0,$L43
	lw	$16,184($sp)
	lw	$2,184($sp)
	andi	$2,$2,0xffff
	move	$4,$2
	lui	$2,%hi(ioReadPhyReg32)
	addiu	$2,$2,%lo(ioReadPhyReg32)
	jalr	$2
	move	$3,$2
	lui	$2,%hi($LC13)
	addiu	$4,$2,%lo($LC13)
	move	$5,$16
	move	$6,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	j	$L44
$L43:
	addiu	$2,$sp,152
	sw	$2,64($sp)
	lui	$2,%hi($LC14)
	addiu	$2,$2,%lo($LC14)
	sw	$2,68($sp)
	lw	$3,64($sp)
	lw	$2,68($sp)
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
	sw	$3,64($sp)
	sw	$2,68($sp)
	sw	$4,72($sp)
	lw	$2,72($sp)
	bne	$2,$0,$L46
	lw	$2,184($sp)
	andi	$2,$2,0xffff
	move	$3,$2
	lw	$2,188($sp)
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(ioWritePhyReg32)
	addiu	$2,$2,%lo(ioWritePhyReg32)
	jalr	$2
	j	$L44
$L46:
	addiu	$2,$sp,152
	sw	$2,76($sp)
	lui	$2,%hi($LC15)
	addiu	$2,$2,%lo($LC15)
	sw	$2,80($sp)
	lw	$3,76($sp)
	lw	$2,80($sp)
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
	sw	$3,76($sp)
	sw	$2,80($sp)
	sw	$4,84($sp)
	lw	$2,84($sp)
	bne	$2,$0,$L44
	lui	$2,%hi(__print_csr)
	addiu	$2,$2,%lo(__print_csr)
	jalr	$2
$L44:
	lw	$2,208($sp)
$L48:
	lw	$31,196($sp)
	lw	$16,192($sp)
	addiu	$sp,$sp,200
	j	$31
	.end	i2c_write_proc
	.size	i2c_write_proc, .-i2c_write_proc
	.rdata
	.align	2
$LC16:
	.ascii	"ioremap the I2C base address failed.\012\000"
	.align	2
$LC17:
	.ascii	"i2c\000"
	.section	.text.i2c_init,"ax",@progbits
	.align	2
	.globl	i2c_init
	.set	nomips16
	.set	nomicromips
	.ent	i2c_init
	.type	i2c_init, @function
i2c_init:
	.frame	$sp,64,$31		# vars= 40, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-64
	sw	$31,60($sp)
	sw	$0,16($sp)
	li	$2,532611072			# 0x1fbf0000
	ori	$2,$2,0x300
	sw	$2,24($sp)
	li	$2,255			# 0xff
	sw	$2,28($sp)
	li	$2,512			# 0x200
	sw	$2,32($sp)
	lw	$2,24($sp)
	sw	$2,36($sp)
	lw	$2,28($sp)
	sw	$2,40($sp)
	lw	$2,32($sp)
	sw	$2,44($sp)
	move	$2,$0
	sw	$2,48($sp)
	lw	$2,48($sp)
	beq	$2,$0,$L51
	nop

	lw	$2,48($sp)
	j	$L52
	nop

$L51:
	lw	$4,24($sp)
	lw	$5,28($sp)
	lw	$6,32($sp)
	lui	$2,%hi(__ioremap)
	addiu	$2,$2,%lo(__ioremap)
	jalr	$2
	nop

$L52:
	move	$3,$2
	lui	$2,%hi(i2cMasterBaseAddr)
	sw	$3,%lo(i2cMasterBaseAddr)($2)
	lui	$2,%hi(i2cMasterBaseAddr)
	lw	$2,%lo(i2cMasterBaseAddr)($2)
	bne	$2,$0,$L53
	nop

	lui	$2,%hi($LC16)
	addiu	$4,$2,%lo($LC16)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	li	$2,-14			# 0xfffffffffffffff2
	j	$L54
	nop

$L53:
	lui	$2,%hi($LC17)
	addiu	$4,$2,%lo($LC17)
	move	$5,$0
	move	$6,$0
	lui	$2,%hi(create_proc_entry)
	addiu	$2,$2,%lo(create_proc_entry)
	jalr	$2
	nop

	sw	$2,20($sp)
	lw	$2,20($sp)
	beq	$2,$0,$L55
	nop

	lw	$2,20($sp)
	lui	$3,%hi(i2c_write_proc)
	addiu	$3,$3,%lo(i2c_write_proc)
	sw	$3,68($2)
$L55:
	lw	$2,16($sp)
$L54:
	lw	$31,60($sp)
	addiu	$sp,$sp,64
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	i2c_init
	.size	i2c_init, .-i2c_init
	.section	.text.i2c_exit,"ax",@progbits
	.align	2
	.globl	i2c_exit
	.set	nomips16
	.set	nomicromips
	.ent	i2c_exit
	.type	i2c_exit, @function
i2c_exit:
	.frame	$sp,32,$31		# vars= 8, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-32
	sw	$31,28($sp)
	lui	$2,%hi($LC17)
	addiu	$4,$2,%lo($LC17)
	move	$5,$0
	lui	$2,%hi(remove_proc_entry)
	addiu	$2,$2,%lo(remove_proc_entry)
	jalr	$2
	nop

	lui	$2,%hi(i2cMasterBaseAddr)
	lw	$2,%lo(i2cMasterBaseAddr)($2)
	sw	$2,16($sp)
	lw	$2,16($sp)
	sw	$2,20($sp)
	move	$2,$0
	bne	$2,$0,$L56
	nop

	lw	$4,16($sp)
	lui	$2,%hi(__iounmap)
	addiu	$2,$2,%lo(__iounmap)
	jalr	$2
	nop

$L56:
	lw	$31,28($sp)
	addiu	$sp,$sp,32
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	i2c_exit
	.size	i2c_exit, .-i2c_exit
	.ident	"GCC: (Buildroot 2015.08.1) 4.9.3"
