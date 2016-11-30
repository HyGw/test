	.file	1 "xmcs_mci.c"
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
	.section	.text.pon_mci_ioctl,"ax",@progbits
	.align	2
	.globl	pon_mci_ioctl
	.set	nomips16
	.set	nomicromips
	.ent	pon_mci_ioctl
	.type	pon_mci_ioctl, @function
pon_mci_ioctl:
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
	li	$2,-22			# 0xffffffffffffffea
	sw	$2,16($sp)
	j	$L2
	nop

$L5:
	li	$4,100			# 0x64
	lui	$2,%hi(msleep)
	addiu	$2,$2,%lo(msleep)
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
	beq	$2,$0,$L3
	nop

	li	$2,1			# 0x1
	j	$L4
	nop

$L3:
	move	$2,$0
$L4:
	li	$3,1			# 0x1
	beq	$2,$3,$L5
	nop

	lw	$2,36($sp)
	srl	$2,$2,8
	andi	$3,$2,0xff
	li	$2,217			# 0xd9
	bne	$3,$2,$L6
	nop

	lw	$4,36($sp)
	lw	$5,40($sp)
	lui	$2,%hi(gpon_cmd_proc)
	addiu	$2,$2,%lo(gpon_cmd_proc)
	jalr	$2
	nop

	sw	$2,16($sp)
	j	$L7
	nop

$L6:
	lw	$2,36($sp)
	srl	$2,$2,8
	andi	$3,$2,0xff
	li	$2,216			# 0xd8
	bne	$3,$2,$L8
	nop

	lw	$4,36($sp)
	lw	$5,40($sp)
	lui	$2,%hi(epon_cmd_proc)
	addiu	$2,$2,%lo(epon_cmd_proc)
	jalr	$2
	nop

	sw	$2,16($sp)
	j	$L7
	nop

$L8:
	lw	$2,36($sp)
	srl	$2,$2,8
	andi	$3,$2,0xff
	li	$2,218			# 0xda
	bne	$3,$2,$L9
	nop

	lw	$4,36($sp)
	lw	$5,40($sp)
	lui	$2,%hi(if_cmd_proc)
	addiu	$2,$2,%lo(if_cmd_proc)
	jalr	$2
	nop

	sw	$2,16($sp)
	j	$L7
	nop

$L9:
	lw	$2,36($sp)
	srl	$2,$2,8
	andi	$3,$2,0xff
	li	$2,219			# 0xdb
	bne	$3,$2,$L10
	nop

	lw	$4,36($sp)
	lw	$5,40($sp)
	lui	$2,%hi(fdet_cmd_proc)
	addiu	$2,$2,%lo(fdet_cmd_proc)
	jalr	$2
	nop

	sw	$2,16($sp)
	j	$L7
	nop

$L10:
	lw	$2,36($sp)
	srl	$2,$2,8
	andi	$3,$2,0xff
	li	$2,215			# 0xd7
	bne	$3,$2,$L7
	nop

	lw	$4,36($sp)
	lw	$5,40($sp)
	lui	$2,%hi(phy_cmd_proc)
	addiu	$2,$2,%lo(phy_cmd_proc)
	jalr	$2
	nop

	sw	$2,16($sp)
$L7:
	lw	$2,16($sp)
	lw	$31,28($sp)
	addiu	$sp,$sp,32
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	pon_mci_ioctl
	.size	pon_mci_ioctl, .-pon_mci_ioctl
	.globl	xmci_fops
	.data
	.align	2
	.type	xmci_fops, @object
	.size	xmci_fops, 100
xmci_fops:
	.word	__this_module
	.space	28
	.word	pon_mci_ioctl
	.space	64
	.rdata
	.align	2
$LC0:
	.ascii	"[%lu0ms]PON MCI destroy successful\012\000"
	.section	.text.pon_mci_destroy,"ax",@progbits
	.align	2
	.globl	pon_mci_destroy
	.set	nomips16
	.set	nomicromips
	.ent	pon_mci_destroy
	.type	pon_mci_destroy, @function
pon_mci_destroy:
	.frame	$sp,32,$31		# vars= 8, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-32
	sw	$31,28($sp)
	lui	$2,%hi(gpMcsPriv)
	lw	$2,%lo(gpMcsPriv)($2)
	lw	$2,0($2)
	beq	$2,$0,$L13
	nop

	lui	$2,%hi(gpMcsPriv)
	lw	$2,%lo(gpMcsPriv)($2)
	lw	$2,0($2)
	move	$4,$2
	lui	$2,%hi(cdev_del)
	addiu	$2,$2,%lo(cdev_del)
	jalr	$2
	nop

	lui	$2,%hi(gpMcsPriv)
	lw	$2,%lo(gpMcsPriv)($2)
	sw	$0,0($2)
$L13:
	li	$2,199229440			# 0xbe00000
	sw	$2,16($sp)
	lw	$4,16($sp)
	li	$5,1			# 0x1
	lui	$2,%hi(unregister_chrdev_region)
	addiu	$2,$2,%lo(unregister_chrdev_region)
	jalr	$2
	nop

	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	lhu	$2,-30816($2)
	andi	$2,$2,0x40
	beq	$2,$0,$L12
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

$L12:
	lw	$31,28($sp)
	addiu	$sp,$sp,32
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	pon_mci_destroy
	.size	pon_mci_destroy, .-pon_mci_destroy
	.rdata
	.align	2
$LC1:
	.ascii	"pon\000"
	.align	2
$LC2:
	.ascii	"[%lu0ms]Get the PON MCI device number (register_chrdev_r"
	.ascii	"egion()) failed\012\000"
	.align	2
$LC3:
	.ascii	"[%lu0ms]Alloc character device (cdev_alloc()) failed\012"
	.ascii	"\000"
	.align	2
$LC4:
	.ascii	"[%lu0ms]PON MCI driver register failed\012\000"
	.align	2
$LC5:
	.ascii	"[%lu0ms]PON MCI initialization successful\012\000"
	.section	.text.pon_mci_init,"ax",@progbits
	.align	2
	.globl	pon_mci_init
	.set	nomips16
	.set	nomicromips
	.ent	pon_mci_init
	.type	pon_mci_init, @function
pon_mci_init:
	.frame	$sp,40,$31		# vars= 16, regs= 2/0, args= 16, gp= 0
	.mask	0x80010000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-40
	sw	$31,36($sp)
	sw	$16,32($sp)
	sw	$0,16($sp)
	li	$2,199229440			# 0xbe00000
	sw	$2,20($sp)
	lw	$4,20($sp)
	li	$5,1			# 0x1
	lui	$2,%hi($LC1)
	addiu	$6,$2,%lo($LC1)
	lui	$2,%hi(register_chrdev_region)
	addiu	$2,$2,%lo(register_chrdev_region)
	jalr	$2
	nop

	sw	$2,16($sp)
	lw	$2,16($sp)
	bgez	$2,$L16
	nop

	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	lhu	$2,-30816($2)
	andi	$2,$2,0x1
	beq	$2,$0,$L17
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

$L17:
	lw	$2,16($sp)
	j	$L18
	nop

$L16:
	lui	$2,%hi(gpMcsPriv)
	lw	$16,%lo(gpMcsPriv)($2)
	lui	$2,%hi(cdev_alloc)
	addiu	$2,$2,%lo(cdev_alloc)
	jalr	$2
	nop

	sw	$2,0($16)
	lui	$2,%hi(gpMcsPriv)
	lw	$2,%lo(gpMcsPriv)($2)
	lw	$2,0($2)
	bne	$2,$0,$L19
	nop

	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	lhu	$2,-30816($2)
	andi	$2,$2,0x1
	beq	$2,$0,$L20
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

$L20:
	li	$2,-12			# 0xfffffffffffffff4
	sw	$2,16($sp)
	j	$L21
	nop

$L19:
	lui	$2,%hi(gpMcsPriv)
	lw	$2,%lo(gpMcsPriv)($2)
	lw	$2,0($2)
	move	$4,$2
	lui	$2,%hi(xmci_fops)
	addiu	$5,$2,%lo(xmci_fops)
	lui	$2,%hi(cdev_init)
	addiu	$2,$2,%lo(cdev_init)
	jalr	$2
	nop

	lui	$2,%hi(gpMcsPriv)
	lw	$2,%lo(gpMcsPriv)($2)
	lw	$2,0($2)
	lui	$3,%hi(__this_module)
	addiu	$3,$3,%lo(__this_module)
	sw	$3,36($2)
	lui	$2,%hi(gpMcsPriv)
	lw	$2,%lo(gpMcsPriv)($2)
	lw	$2,0($2)
	lui	$3,%hi(xmci_fops)
	addiu	$3,$3,%lo(xmci_fops)
	sw	$3,40($2)
	lui	$2,%hi(gpMcsPriv)
	lw	$2,%lo(gpMcsPriv)($2)
	lw	$2,0($2)
	move	$4,$2
	lw	$5,20($sp)
	li	$6,1			# 0x1
	lui	$2,%hi(cdev_add)
	addiu	$2,$2,%lo(cdev_add)
	jalr	$2
	nop

	sw	$2,16($sp)
	lw	$2,16($sp)
	bgez	$2,$L22
	nop

	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	lhu	$2,-30816($2)
	andi	$2,$2,0x1
	beq	$2,$0,$L23
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

	j	$L21
	nop

$L23:
	j	$L21
	nop

$L22:
	lui	$2,%hi(gpMcsPriv)
	lw	$2,%lo(gpMcsPriv)($2)
	addiu	$2,$2,4
	move	$4,$2
	lui	$2,%hi(__key.30829)
	addiu	$5,$2,%lo(__key.30829)
	lui	$2,%hi(__init_waitqueue_head)
	addiu	$2,$2,%lo(__init_waitqueue_head)
	jalr	$2
	nop

	lui	$2,%hi(gpMcsPriv)
	lw	$2,%lo(gpMcsPriv)($2)
	addiu	$2,$2,16
	move	$4,$2
	move	$5,$0
	li	$6,388			# 0x184
	lui	$2,%hi(memset)
	addiu	$2,$2,%lo(memset)
	jalr	$2
	nop

	lui	$2,%hi(gpMcsPriv)
	lw	$3,%lo(gpMcsPriv)($2)
	lw	$2,404($3)
	ins	$2,$0,31,1
	sw	$2,404($3)
	lui	$2,%hi(gpMcsPriv)
	lw	$2,%lo(gpMcsPriv)($2)
	addiu	$2,$2,408
	sw	$2,24($sp)
	lui	$2,%hi(gpMcsPriv)
	lw	$2,%lo(gpMcsPriv)($2)
	sw	$0,408($2)
	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	lhu	$2,-30816($2)
	andi	$2,$2,0x40
	beq	$2,$0,$L25
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

$L25:
	move	$2,$0
	j	$L18
	nop

$L21:
	lui	$2,%hi(pon_mci_destroy)
	addiu	$2,$2,%lo(pon_mci_destroy)
	jalr	$2
	nop

	lw	$2,16($sp)
$L18:
	lw	$31,36($sp)
	lw	$16,32($sp)
	addiu	$sp,$sp,40
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	pon_mci_init
	.size	pon_mci_init, .-pon_mci_init
	.local	__key.30829
	.comm	__key.30829,0,4
	.ident	"GCC: (Buildroot 2015.08.1) 4.9.3"
