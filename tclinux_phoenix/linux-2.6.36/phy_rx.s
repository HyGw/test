	.file	1 "phy_rx.c"
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
	.ascii	"/opt/tclinux_phoenix/modules/private/xpon_phy/src/phy_rx"
	.ascii	".c\000"
	.align	2
$LC1:
	.ascii	"[%lu]0ms[%s:%d]\012  correct_bytes: %.8x\012\000"
	.align	2
$LC2:
	.ascii	"[%lu]0ms[%s:%d]\012  correct_codewords: %.8x\012\000"
	.align	2
$LC3:
	.ascii	"[%lu]0ms[%s:%d]\012  uncorrect_codewords: %.8x\012\000"
	.align	2
$LC4:
	.ascii	"[%lu]0ms[%s:%d]\012  total_rx_codewords: %.8x\012\000"
	.align	2
$LC5:
	.ascii	"[%lu]0ms[%s:%d]\012  fec_seconds: %.8x\012\000"
#NO_APP
	.section	.text.phy_rx_fec_counter,"ax",@progbits
	.align	2
	.globl	phy_rx_fec_counter
	.set	nomips16
	.set	nomicromips
	.ent	phy_rx_fec_counter
	.type	phy_rx_fec_counter, @function
phy_rx_fec_counter:
	.frame	$sp,72,$31		# vars= 40, regs= 2/0, args= 24, gp= 0
	.mask	0x80010000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-72
	sw	$31,68($sp)
	sw	$16,64($sp)
	sw	$4,72($sp)
	li	$4,1			# 0x1
	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$5,$2,0x234
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$2,$2,0x238
	sw	$2,24($sp)
	lw	$2,24($sp)
	lw	$2,0($2)
	sw	$2,28($sp)
	lw	$2,24($sp)
	lw	$2,0($2)
	sw	$2,28($sp)
	lw	$3,28($sp)
	lw	$2,72($sp)
	sw	$3,0($2)
	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$2,$2,0x23c
	sw	$2,32($sp)
	lw	$2,32($sp)
	lw	$2,0($2)
	sw	$2,36($sp)
	lw	$2,32($sp)
	lw	$2,0($2)
	sw	$2,36($sp)
	lw	$3,36($sp)
	lw	$2,72($sp)
	sw	$3,4($2)
	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$2,$2,0x240
	sw	$2,40($sp)
	lw	$2,40($sp)
	lw	$2,0($2)
	sw	$2,44($sp)
	lw	$2,40($sp)
	lw	$2,0($2)
	sw	$2,44($sp)
	lw	$3,44($sp)
	lw	$2,72($sp)
	sw	$3,8($2)
	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$2,$2,0x244
	sw	$2,48($sp)
	lw	$2,48($sp)
	lw	$2,0($2)
	sw	$2,52($sp)
	lw	$2,48($sp)
	lw	$2,0($2)
	sw	$2,52($sp)
	lw	$3,52($sp)
	lw	$2,72($sp)
	sw	$3,12($2)
	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$2,$2,0x248
	sw	$2,56($sp)
	lw	$2,56($sp)
	lw	$2,0($2)
	sw	$2,60($sp)
	lw	$2,56($sp)
	lw	$2,0($2)
	sw	$2,60($sp)
	lw	$3,60($sp)
	lw	$2,72($sp)
	sw	$3,16($2)
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,136($2)
	andi	$2,$2,0x10
	beq	$2,$0,$L7
	nop

	lui	$2,%hi(jiffies)
	lw	$16,%lo(jiffies)($2)
	lui	$2,%hi($LC0)
	addiu	$4,$2,%lo($LC0)
	li	$5,47			# 0x2f
	lui	$2,%hi(strrchr)
	addiu	$2,$2,%lo(strrchr)
	jalr	$2
	nop

	addiu	$3,$2,1
	lw	$2,72($sp)
	lw	$2,0($2)
	sw	$2,16($sp)
	lui	$2,%hi($LC1)
	addiu	$4,$2,%lo($LC1)
	move	$5,$16
	move	$6,$3
	li	$7,37			# 0x25
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L7:
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,136($2)
	andi	$2,$2,0x10
	beq	$2,$0,$L8
	nop

	lui	$2,%hi(jiffies)
	lw	$16,%lo(jiffies)($2)
	lui	$2,%hi($LC0)
	addiu	$4,$2,%lo($LC0)
	li	$5,47			# 0x2f
	lui	$2,%hi(strrchr)
	addiu	$2,$2,%lo(strrchr)
	jalr	$2
	nop

	addiu	$3,$2,1
	lw	$2,72($sp)
	lw	$2,4($2)
	sw	$2,16($sp)
	lui	$2,%hi($LC2)
	addiu	$4,$2,%lo($LC2)
	move	$5,$16
	move	$6,$3
	li	$7,38			# 0x26
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L8:
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,136($2)
	andi	$2,$2,0x10
	beq	$2,$0,$L9
	nop

	lui	$2,%hi(jiffies)
	lw	$16,%lo(jiffies)($2)
	lui	$2,%hi($LC0)
	addiu	$4,$2,%lo($LC0)
	li	$5,47			# 0x2f
	lui	$2,%hi(strrchr)
	addiu	$2,$2,%lo(strrchr)
	jalr	$2
	nop

	addiu	$3,$2,1
	lw	$2,72($sp)
	lw	$2,8($2)
	sw	$2,16($sp)
	lui	$2,%hi($LC3)
	addiu	$4,$2,%lo($LC3)
	move	$5,$16
	move	$6,$3
	li	$7,39			# 0x27
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L9:
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,136($2)
	andi	$2,$2,0x10
	beq	$2,$0,$L10
	nop

	lui	$2,%hi(jiffies)
	lw	$16,%lo(jiffies)($2)
	lui	$2,%hi($LC0)
	addiu	$4,$2,%lo($LC0)
	li	$5,47			# 0x2f
	lui	$2,%hi(strrchr)
	addiu	$2,$2,%lo(strrchr)
	jalr	$2
	nop

	addiu	$3,$2,1
	lw	$2,72($sp)
	lw	$2,12($2)
	sw	$2,16($sp)
	lui	$2,%hi($LC4)
	addiu	$4,$2,%lo($LC4)
	move	$5,$16
	move	$6,$3
	li	$7,40			# 0x28
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L10:
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,136($2)
	andi	$2,$2,0x10
	beq	$2,$0,$L1
	nop

	lui	$2,%hi(jiffies)
	lw	$16,%lo(jiffies)($2)
	lui	$2,%hi($LC0)
	addiu	$4,$2,%lo($LC0)
	li	$5,47			# 0x2f
	lui	$2,%hi(strrchr)
	addiu	$2,$2,%lo(strrchr)
	jalr	$2
	nop

	addiu	$3,$2,1
	lw	$2,72($sp)
	lw	$2,16($2)
	sw	$2,16($sp)
	lui	$2,%hi($LC5)
	addiu	$4,$2,%lo($LC5)
	move	$5,$16
	move	$6,$3
	li	$7,41			# 0x29
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L1:
	lw	$31,68($sp)
	lw	$16,64($sp)
	addiu	$sp,$sp,72
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	phy_rx_fec_counter
	.size	phy_rx_fec_counter, .-phy_rx_fec_counter
	.rdata
	.align	2
$LC6:
	.ascii	"[%lu]0ms[%s:%d]\012  Phy BIP Counter : %.8x\012\000"
	.section	.text.phy_bip_counter,"ax",@progbits
	.align	2
	.globl	phy_bip_counter
	.set	nomips16
	.set	nomicromips
	.ent	phy_bip_counter
	.type	phy_bip_counter, @function
phy_bip_counter:
	.frame	$sp,48,$31		# vars= 16, regs= 2/0, args= 24, gp= 0
	.mask	0x80010000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-48
	sw	$31,44($sp)
	sw	$16,40($sp)
	sw	$0,24($sp)
	li	$4,4			# 0x4
	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$5,$2,0x234
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$2,$2,0x24c
	sw	$2,28($sp)
	lw	$2,28($sp)
	lw	$2,0($2)
	sw	$2,32($sp)
	lw	$2,28($sp)
	lw	$2,0($2)
	sw	$2,32($sp)
	lw	$2,32($sp)
	sw	$2,24($sp)
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,136($2)
	andi	$2,$2,0x10
	beq	$2,$0,$L14
	nop

	lui	$2,%hi(jiffies)
	lw	$16,%lo(jiffies)($2)
	lui	$2,%hi($LC0)
	addiu	$4,$2,%lo($LC0)
	li	$5,47			# 0x2f
	lui	$2,%hi(strrchr)
	addiu	$2,$2,%lo(strrchr)
	jalr	$2
	nop

	addiu	$3,$2,1
	lw	$2,24($sp)
	sw	$2,16($sp)
	lui	$2,%hi($LC6)
	addiu	$4,$2,%lo($LC6)
	move	$5,$16
	move	$6,$3
	li	$7,62			# 0x3e
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L14:
	li	$4,8			# 0x8
	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$5,$2,0x234
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	lw	$2,24($sp)
	lw	$31,44($sp)
	lw	$16,40($sp)
	addiu	$sp,$sp,48
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	phy_bip_counter
	.size	phy_bip_counter, .-phy_bip_counter
	.rdata
	.align	2
$LC7:
	.ascii	"[%lu]0ms[%s:%d]\012  frame_count_high: %.8x\012\000"
	.align	2
$LC8:
	.ascii	"[%lu]0ms[%s:%d]\012  frame_count_low: %.8x\012\000"
	.align	2
$LC9:
	.ascii	"[%lu]0ms[%s:%d]\012  lof_counter: %.8x\012\000"
	.section	.text.phy_rx_frame_counter,"ax",@progbits
	.align	2
	.globl	phy_rx_frame_counter
	.set	nomips16
	.set	nomicromips
	.ent	phy_rx_frame_counter
	.type	phy_rx_frame_counter, @function
phy_rx_frame_counter:
	.frame	$sp,56,$31		# vars= 24, regs= 2/0, args= 24, gp= 0
	.mask	0x80010000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-56
	sw	$31,52($sp)
	sw	$16,48($sp)
	sw	$4,56($sp)
	li	$4,16			# 0x10
	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$5,$2,0x234
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$2,$2,0x254
	sw	$2,24($sp)
	lw	$2,24($sp)
	lw	$2,0($2)
	sw	$2,28($sp)
	lw	$2,24($sp)
	lw	$2,0($2)
	sw	$2,28($sp)
	lw	$3,28($sp)
	lw	$2,56($sp)
	sw	$3,4($2)
	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$2,$2,0x250
	sw	$2,32($sp)
	lw	$2,32($sp)
	lw	$2,0($2)
	sw	$2,36($sp)
	lw	$2,32($sp)
	lw	$2,0($2)
	sw	$2,36($sp)
	lw	$3,36($sp)
	lw	$2,56($sp)
	sw	$3,0($2)
	lui	$2,%hi(phy_gpon_mode)
	addiu	$2,$2,%lo(phy_gpon_mode)
	jalr	$2
	nop

	beq	$2,$0,$L19
	nop

	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$2,$2,0x258
	sw	$2,40($sp)
	lw	$2,40($sp)
	lw	$2,0($2)
	sw	$2,44($sp)
	lw	$2,40($sp)
	lw	$2,0($2)
	sw	$2,44($sp)
	lw	$3,44($sp)
	lw	$2,56($sp)
	sw	$3,8($2)
$L19:
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,136($2)
	andi	$2,$2,0x10
	beq	$2,$0,$L21
	nop

	lui	$2,%hi(jiffies)
	lw	$16,%lo(jiffies)($2)
	lui	$2,%hi($LC0)
	addiu	$4,$2,%lo($LC0)
	li	$5,47			# 0x2f
	lui	$2,%hi(strrchr)
	addiu	$2,$2,%lo(strrchr)
	jalr	$2
	nop

	addiu	$3,$2,1
	lw	$2,56($sp)
	lw	$2,4($2)
	sw	$2,16($sp)
	lui	$2,%hi($LC7)
	addiu	$4,$2,%lo($LC7)
	move	$5,$16
	move	$6,$3
	li	$7,91			# 0x5b
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L21:
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,136($2)
	andi	$2,$2,0x10
	beq	$2,$0,$L22
	nop

	lui	$2,%hi(jiffies)
	lw	$16,%lo(jiffies)($2)
	lui	$2,%hi($LC0)
	addiu	$4,$2,%lo($LC0)
	li	$5,47			# 0x2f
	lui	$2,%hi(strrchr)
	addiu	$2,$2,%lo(strrchr)
	jalr	$2
	nop

	addiu	$3,$2,1
	lw	$2,56($sp)
	lw	$2,0($2)
	sw	$2,16($sp)
	lui	$2,%hi($LC8)
	addiu	$4,$2,%lo($LC8)
	move	$5,$16
	move	$6,$3
	li	$7,92			# 0x5c
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L22:
	lui	$2,%hi(phy_gpon_mode)
	addiu	$2,$2,%lo(phy_gpon_mode)
	jalr	$2
	nop

	beq	$2,$0,$L16
	nop

	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,136($2)
	andi	$2,$2,0x10
	beq	$2,$0,$L16
	nop

	lui	$2,%hi(jiffies)
	lw	$16,%lo(jiffies)($2)
	lui	$2,%hi($LC0)
	addiu	$4,$2,%lo($LC0)
	li	$5,47			# 0x2f
	lui	$2,%hi(strrchr)
	addiu	$2,$2,%lo(strrchr)
	jalr	$2
	nop

	addiu	$3,$2,1
	lw	$2,56($sp)
	lw	$2,8($2)
	sw	$2,16($sp)
	lui	$2,%hi($LC9)
	addiu	$4,$2,%lo($LC9)
	move	$5,$16
	move	$6,$3
	li	$7,94			# 0x5e
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L16:
	lw	$31,52($sp)
	lw	$16,48($sp)
	addiu	$sp,$sp,56
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	phy_rx_frame_counter
	.size	phy_rx_frame_counter, .-phy_rx_frame_counter
	.rdata
	.align	2
$LC10:
	.ascii	"[%lu]0ms[%s:%d]\012  Rx FEC Enable\012\000"
	.align	2
$LC11:
	.ascii	"[%lu]0ms[%s:%d]\012  Rx FEC Disable\012\000"
	.section	.text.phy_rx_fec_setting,"ax",@progbits
	.align	2
	.globl	phy_rx_fec_setting
	.set	nomips16
	.set	nomicromips
	.ent	phy_rx_fec_setting
	.type	phy_rx_fec_setting, @function
phy_rx_fec_setting:
	.frame	$sp,24,$31		# vars= 0, regs= 2/0, args= 16, gp= 0
	.mask	0x80010000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-24
	sw	$31,20($sp)
	sw	$16,16($sp)
	move	$2,$4
	sb	$2,24($sp)
	lbu	$3,24($sp)
	li	$2,1			# 0x1
	bne	$3,$2,$L25
	nop

	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,136($2)
	andi	$2,$2,0x10
	beq	$2,$0,$L26
	nop

	lui	$2,%hi(jiffies)
	lw	$16,%lo(jiffies)($2)
	lui	$2,%hi($LC0)
	addiu	$4,$2,%lo($LC0)
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
	li	$7,113			# 0x71
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L26:
	li	$4,1			# 0x1
	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$5,$2,0x268
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	move	$2,$0
	j	$L27
	nop

$L25:
	lbu	$2,24($sp)
	bne	$2,$0,$L28
	nop

	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,136($2)
	andi	$2,$2,0x10
	beq	$2,$0,$L29
	nop

	lui	$2,%hi(jiffies)
	lw	$16,%lo(jiffies)($2)
	lui	$2,%hi($LC0)
	addiu	$4,$2,%lo($LC0)
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
	li	$7,119			# 0x77
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L29:
	move	$4,$0
	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$5,$2,0x268
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	move	$2,$0
	j	$L27
	nop

$L28:
	li	$2,1			# 0x1
$L27:
	lw	$31,20($sp)
	lw	$16,16($sp)
	addiu	$sp,$sp,24
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	phy_rx_fec_setting
	.size	phy_rx_fec_setting, .-phy_rx_fec_setting
	.section	.text.phy_rx_fec_getting,"ax",@progbits
	.align	2
	.globl	phy_rx_fec_getting
	.set	nomips16
	.set	nomicromips
	.ent	phy_rx_fec_getting
	.type	phy_rx_fec_getting, @function
phy_rx_fec_getting:
	.frame	$sp,8,$31		# vars= 8, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-8
	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$2,$2,0x268
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
	.end	phy_rx_fec_getting
	.size	phy_rx_fec_getting, .-phy_rx_fec_getting
	.rdata
	.align	2
$LC12:
	.ascii	"[%lu]0ms[%s:%d]\012  RX FEC on.\012\000"
	.align	2
$LC13:
	.ascii	"[%lu]0ms[%s:%d]\012  RX FEC off.\012\000"
	.section	.text.phy_rx_fec_status,"ax",@progbits
	.align	2
	.globl	phy_rx_fec_status
	.set	nomips16
	.set	nomicromips
	.ent	phy_rx_fec_status
	.type	phy_rx_fec_status, @function
phy_rx_fec_status:
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
	ori	$2,$2,0x21c
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
	andi	$2,$2,0x8000
	beq	$2,$0,$L35
	nop

	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,136($2)
	andi	$2,$2,0x10
	beq	$2,$0,$L36
	nop

	lui	$2,%hi(jiffies)
	lw	$16,%lo(jiffies)($2)
	lui	$2,%hi($LC0)
	addiu	$4,$2,%lo($LC0)
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
	li	$7,164			# 0xa4
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L36:
	li	$2,1			# 0x1
	j	$L37
	nop

$L35:
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,136($2)
	andi	$2,$2,0x10
	beq	$2,$0,$L38
	nop

	lui	$2,%hi(jiffies)
	lw	$16,%lo(jiffies)($2)
	lui	$2,%hi($LC0)
	addiu	$4,$2,%lo($LC0)
	li	$5,47			# 0x2f
	lui	$2,%hi(strrchr)
	addiu	$2,$2,%lo(strrchr)
	jalr	$2
	nop

	addiu	$3,$2,1
	lui	$2,%hi($LC13)
	addiu	$4,$2,%lo($LC13)
	move	$5,$16
	move	$6,$3
	li	$7,169			# 0xa9
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L38:
	move	$2,$0
$L37:
	lw	$31,36($sp)
	lw	$16,32($sp)
	addiu	$sp,$sp,40
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	phy_rx_fec_status
	.size	phy_rx_fec_status, .-phy_rx_fec_status
	.rdata
	.align	2
$LC14:
	.ascii	"[%lu]0ms[%s:%d]\012  error input : rx_sd_inv.\012\000"
	.align	2
$LC15:
	.ascii	"[%lu]0ms[%s:%d]\012  rx_sd_inv success \012\000"
	.section	.text.phy_trans_rx_setting,"ax",@progbits
	.align	2
	.globl	phy_trans_rx_setting
	.set	nomips16
	.set	nomicromips
	.ent	phy_trans_rx_setting
	.type	phy_trans_rx_setting, @function
phy_trans_rx_setting:
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
	ori	$2,$2,0x138
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
	bne	$3,$2,$L41
	nop

	lw	$2,20($sp)
	ori	$2,$2,0x40
	sw	$2,16($sp)
	j	$L42
	nop

$L41:
	lbu	$2,40($sp)
	bne	$2,$0,$L43
	nop

	lw	$3,20($sp)
	li	$2,-65			# 0xffffffffffffffbf
	and	$2,$3,$2
	sw	$2,16($sp)
	j	$L42
	nop

$L43:
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,136($2)
	andi	$2,$2,0x10
	beq	$2,$0,$L44
	nop

	lui	$2,%hi(jiffies)
	lw	$16,%lo(jiffies)($2)
	lui	$2,%hi($LC0)
	addiu	$4,$2,%lo($LC0)
	li	$5,47			# 0x2f
	lui	$2,%hi(strrchr)
	addiu	$2,$2,%lo(strrchr)
	jalr	$2
	nop

	addiu	$3,$2,1
	lui	$2,%hi($LC14)
	addiu	$4,$2,%lo($LC14)
	move	$5,$16
	move	$6,$3
	li	$7,197			# 0xc5
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L44:
	li	$2,1			# 0x1
	j	$L45
	nop

$L42:
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,136($2)
	andi	$2,$2,0x10
	beq	$2,$0,$L46
	nop

	lui	$2,%hi(jiffies)
	lw	$16,%lo(jiffies)($2)
	lui	$2,%hi($LC0)
	addiu	$4,$2,%lo($LC0)
	li	$5,47			# 0x2f
	lui	$2,%hi(strrchr)
	addiu	$2,$2,%lo(strrchr)
	jalr	$2
	nop

	addiu	$3,$2,1
	lui	$2,%hi($LC15)
	addiu	$4,$2,%lo($LC15)
	move	$5,$16
	move	$6,$3
	li	$7,200			# 0xc8
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L46:
	lw	$4,16($sp)
	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$5,$2,0x138
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	move	$2,$0
$L45:
	lw	$31,36($sp)
	lw	$16,32($sp)
	addiu	$sp,$sp,40
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	phy_trans_rx_setting
	.size	phy_trans_rx_setting, .-phy_trans_rx_setting
	.section	.text.phy_trans_rx_getting,"ax",@progbits
	.align	2
	.globl	phy_trans_rx_getting
	.set	nomips16
	.set	nomicromips
	.ent	phy_trans_rx_getting
	.type	phy_trans_rx_getting, @function
phy_trans_rx_getting:
	.frame	$sp,16,$31		# vars= 16, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-16
	sw	$0,0($sp)
	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$2,$2,0x138
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
	andi	$2,$2,0x40
	bne	$2,$0,$L49
	nop

	move	$2,$0
	j	$L50
	nop

$L49:
	li	$2,1			# 0x1
$L50:
	addiu	$sp,$sp,16
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	phy_trans_rx_getting
	.size	phy_trans_rx_getting, .-phy_trans_rx_getting
	.rdata
	.align	2
$LC16:
	.ascii	"[%lu]0ms[%s:%d]\012  unknown BER \012\000"
	.globl	__udivdi3
	.align	2
$LC17:
	.ascii	"[%lu]0ms[%s:%d]\012  ber_counter = %d \012\000"
	.align	2
$LC18:
	.ascii	"[%lu]0ms[%s:%d]\012  BER_order = %d \012\000"
	.section	.text.phy_ber_alarm,"ax",@progbits
	.align	2
	.globl	phy_ber_alarm
	.set	nomips16
	.set	nomicromips
	.ent	phy_ber_alarm
	.type	phy_ber_alarm, @function
phy_ber_alarm:
	.frame	$sp,96,$31		# vars= 48, regs= 5/0, args= 24, gp= 0
	.mask	0x800f0000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-96
	sw	$31,92($sp)
	sw	$19,88($sp)
	sw	$18,84($sp)
	sw	$17,80($sp)
	sw	$16,76($sp)
	move	$3,$0
	move	$2,$0
	sw	$3,44($sp)
	sw	$2,40($sp)
	move	$3,$0
	move	$2,$0
	sw	$3,28($sp)
	sw	$2,24($sp)
	sw	$0,32($sp)
	move	$3,$0
	move	$2,$0
	sw	$3,52($sp)
	sw	$2,48($sp)
	addiu	$2,$sp,60
	sw	$2,56($sp)
	lui	$2,%hi(ber_counter.24683)
	lw	$3,%lo(ber_counter.24683)($2)
	li	$2,1759182848			# 0x68db0000
	ori	$2,$2,0x8bad
	mult	$3,$2
	mfhi	$2
	sra	$4,$2,12
	sra	$2,$3,31
	subu	$2,$4,$2
	li	$4,10000			# 0x2710
	mul	$2,$2,$4
	subu	$2,$3,$2
	bne	$2,$0,$L52
	nop

	li	$4,6			# 0x6
	lui	$2,%hi(phy_counter_clear)
	addiu	$2,$2,%lo(phy_counter_clear)
	jalr	$2
	nop

	lui	$2,%hi(ber_counter.24683)
	sw	$0,%lo(ber_counter.24683)($2)
$L52:
	lui	$2,%hi(ber_counter.24683)
	lw	$2,%lo(ber_counter.24683)($2)
	addiu	$3,$2,1
	lui	$2,%hi(ber_counter.24683)
	sw	$3,%lo(ber_counter.24683)($2)
	lw	$4,56($sp)
	lui	$2,%hi(phy_rx_frame_counter)
	addiu	$2,$2,%lo(phy_rx_frame_counter)
	jalr	$2
	nop

	lw	$2,56($sp)
	lw	$2,0($2)
	sw	$2,44($sp)
	sw	$0,40($sp)
	lui	$2,%hi(phy_bip_counter)
	addiu	$2,$2,%lo(phy_bip_counter)
	jalr	$2
	nop

	sw	$2,28($sp)
	sra	$2,$2,31
	sw	$2,24($sp)
	lw	$2,24($sp)
	bne	$2,$0,$L53
	nop

	lw	$2,24($sp)
	bne	$2,$0,$L66
	nop

	lw	$2,28($sp)
	sltu	$2,$2,100
	beq	$2,$0,$L53
	nop

$L66:
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,136($2)
	andi	$2,$2,0x10
	beq	$2,$0,$L55
	nop

	lui	$2,%hi(jiffies)
	lw	$16,%lo(jiffies)($2)
	lui	$2,%hi($LC0)
	addiu	$4,$2,%lo($LC0)
	li	$5,47			# 0x2f
	lui	$2,%hi(strrchr)
	addiu	$2,$2,%lo(strrchr)
	jalr	$2
	nop

	addiu	$3,$2,1
	lui	$2,%hi($LC16)
	addiu	$4,$2,%lo($LC16)
	move	$5,$16
	move	$6,$3
	li	$7,259			# 0x103
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L55:
	move	$2,$0
	j	$L65
	nop

$L53:
	li	$2,1			# 0x1
	sw	$2,32($sp)
	j	$L57
	nop

$L62:
	lw	$3,28($sp)
	lw	$2,24($sp)
	srl	$4,$3,31
	sll	$18,$2,1
	or	$18,$4,$18
	sll	$19,$3,1
	move	$3,$19
	move	$2,$18
	srl	$4,$3,30
	sll	$16,$2,2
	or	$16,$4,$16
	sll	$17,$3,2
	addu	$5,$3,$17
	sltu	$6,$5,$3
	addu	$4,$2,$16
	addu	$2,$6,$4
	move	$4,$2
	sw	$5,28($sp)
	sw	$4,24($sp)
	lw	$3,40($sp)
	li	$2,262144			# 0x40000
	ori	$2,$2,0xbf00
	mul	$3,$3,$2
	lw	$2,44($sp)
	move	$4,$0
	mul	$2,$2,$4
	addu	$4,$3,$2
	lw	$3,44($sp)
	li	$2,262144			# 0x40000
	ori	$2,$2,0xbf00
	multu	$3,$2
	mflo	$3
	mfhi	$2
	addu	$4,$4,$2
	move	$2,$4
	lui	$4,%hi(__udivdi3)
	addiu	$8,$4,%lo(__udivdi3)
	lw	$5,28($sp)
	lw	$4,24($sp)
	move	$7,$3
	move	$6,$2
	jalr	$8
	nop

	sw	$3,52($sp)
	sw	$2,48($sp)
	lw	$2,48($sp)
	bne	$2,$0,$L58
	nop

	lw	$2,48($sp)
	bne	$2,$0,$L67
	nop

	lw	$2,52($sp)
	sltu	$2,$2,10
	beq	$2,$0,$L58
	nop

$L67:
	lw	$2,48($sp)
	lw	$3,52($sp)
	or	$2,$2,$3
	beq	$2,$0,$L58
	nop

	j	$L61
	nop

$L58:
	lw	$2,32($sp)
	addiu	$2,$2,1
	sw	$2,32($sp)
$L57:
	lw	$2,32($sp)
	slt	$2,$2,11
	bne	$2,$0,$L62
	nop

$L61:
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,136($2)
	andi	$2,$2,0x10
	beq	$2,$0,$L63
	nop

	lui	$2,%hi(jiffies)
	lw	$16,%lo(jiffies)($2)
	lui	$2,%hi($LC0)
	addiu	$4,$2,%lo($LC0)
	li	$5,47			# 0x2f
	lui	$2,%hi(strrchr)
	addiu	$2,$2,%lo(strrchr)
	jalr	$2
	nop

	addiu	$3,$2,1
	lui	$2,%hi(ber_counter.24683)
	lw	$2,%lo(ber_counter.24683)($2)
	sw	$2,16($sp)
	lui	$2,%hi($LC17)
	addiu	$4,$2,%lo($LC17)
	move	$5,$16
	move	$6,$3
	li	$7,274			# 0x112
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L63:
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,136($2)
	andi	$2,$2,0x10
	beq	$2,$0,$L64
	nop

	lui	$2,%hi(jiffies)
	lw	$16,%lo(jiffies)($2)
	lui	$2,%hi($LC0)
	addiu	$4,$2,%lo($LC0)
	li	$5,47			# 0x2f
	lui	$2,%hi(strrchr)
	addiu	$2,$2,%lo(strrchr)
	jalr	$2
	nop

	addiu	$3,$2,1
	lw	$2,32($sp)
	sw	$2,16($sp)
	lui	$2,%hi($LC18)
	addiu	$4,$2,%lo($LC18)
	move	$5,$16
	move	$6,$3
	li	$7,275			# 0x113
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L64:
	lw	$2,32($sp)
$L65:
	lw	$31,92($sp)
	lw	$19,88($sp)
	lw	$18,84($sp)
	lw	$17,80($sp)
	lw	$16,76($sp)
	addiu	$sp,$sp,96
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	phy_ber_alarm
	.size	phy_ber_alarm, .-phy_ber_alarm
	.rdata
	.align	2
$LC19:
	.ascii	"[%lu]0ms[%s:%d]\012  error input : gpon_m1_lmt. \012\000"
	.align	2
$LC20:
	.ascii	"[%lu]0ms[%s:%d]\012  error input : gpon_m2_lmt. \012\000"
	.align	2
$LC21:
	.ascii	"[%lu]0ms[%s:%d]\012  error input : presync_mismatch_lmt."
	.ascii	" \012\000"
	.align	2
$LC22:
	.ascii	"[%lu]0ms[%s:%d]\012  error input : insync_mismatch_lmt. "
	.ascii	"\012\000"
	.align	2
$LC23:
	.ascii	"[%lu]0ms[%s:%d]\012  error input : gpon_gsync_prot. \012"
	.ascii	"\000"
	.section	.text.phy_gpon_psync_setting,"ax",@progbits
	.align	2
	.globl	phy_gpon_psync_setting
	.set	nomips16
	.set	nomicromips
	.ent	phy_gpon_psync_setting
	.type	phy_gpon_psync_setting, @function
phy_gpon_psync_setting:
	.frame	$sp,40,$31		# vars= 16, regs= 2/0, args= 16, gp= 0
	.mask	0x80010000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-40
	sw	$31,36($sp)
	sw	$16,32($sp)
	sw	$4,40($sp)
	sw	$0,16($sp)
	sw	$0,20($sp)
	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$2,$2,0x20c
	sw	$2,24($sp)
	lw	$2,24($sp)
	lw	$2,0($2)
	sw	$2,28($sp)
	lw	$2,24($sp)
	lw	$2,0($2)
	sw	$2,28($sp)
	lw	$2,28($sp)
	sw	$2,16($sp)
	lw	$2,40($sp)
	lw	$2,8($2)
	andi	$2,$2,0x1
	beq	$2,$0,$L70
	nop

	lw	$2,40($sp)
	lbu	$2,0($2)
	andi	$2,$2,0xf8
	beq	$2,$0,$L71
	nop

	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,136($2)
	andi	$2,$2,0x1
	beq	$2,$0,$L72
	nop

	lui	$2,%hi(jiffies)
	lw	$16,%lo(jiffies)($2)
	lui	$2,%hi($LC0)
	addiu	$4,$2,%lo($LC0)
	li	$5,47			# 0x2f
	lui	$2,%hi(strrchr)
	addiu	$2,$2,%lo(strrchr)
	jalr	$2
	nop

	addiu	$3,$2,1
	lui	$2,%hi($LC19)
	addiu	$4,$2,%lo($LC19)
	move	$5,$16
	move	$6,$3
	li	$7,302			# 0x12e
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L72:
	li	$2,1			# 0x1
	j	$L73
	nop

$L71:
	lw	$3,16($sp)
	li	$2,-8			# 0xfffffffffffffff8
	and	$2,$3,$2
	lw	$3,40($sp)
	lbu	$3,0($3)
	or	$2,$2,$3
	sw	$2,16($sp)
$L70:
	lw	$2,40($sp)
	lw	$2,8($2)
	andi	$2,$2,0x2
	beq	$2,$0,$L74
	nop

	lw	$2,40($sp)
	lbu	$2,1($2)
	andi	$2,$2,0xf8
	beq	$2,$0,$L75
	nop

	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,136($2)
	andi	$2,$2,0x1
	beq	$2,$0,$L76
	nop

	lui	$2,%hi(jiffies)
	lw	$16,%lo(jiffies)($2)
	lui	$2,%hi($LC0)
	addiu	$4,$2,%lo($LC0)
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
	li	$7,311			# 0x137
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L76:
	li	$2,1			# 0x1
	j	$L73
	nop

$L75:
	lw	$3,16($sp)
	li	$2,-57			# 0xffffffffffffffc7
	and	$2,$3,$2
	lw	$3,40($sp)
	lbu	$3,1($3)
	sll	$3,$3,3
	or	$2,$2,$3
	sw	$2,16($sp)
$L74:
	lw	$2,40($sp)
	lw	$2,8($2)
	andi	$2,$2,0x4
	beq	$2,$0,$L77
	nop

	lw	$2,40($sp)
	lbu	$2,2($2)
	andi	$2,$2,0xf8
	beq	$2,$0,$L78
	nop

	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,136($2)
	andi	$2,$2,0x1
	beq	$2,$0,$L79
	nop

	lui	$2,%hi(jiffies)
	lw	$16,%lo(jiffies)($2)
	lui	$2,%hi($LC0)
	addiu	$4,$2,%lo($LC0)
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
	li	$7,320			# 0x140
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L79:
	li	$2,1			# 0x1
	j	$L73
	nop

$L78:
	lw	$3,16($sp)
	li	$2,-1793			# 0xfffffffffffff8ff
	and	$2,$3,$2
	lw	$3,40($sp)
	lbu	$3,2($3)
	sll	$3,$3,8
	or	$2,$2,$3
	sw	$2,16($sp)
$L77:
	lw	$2,40($sp)
	lw	$2,8($2)
	andi	$2,$2,0x8
	beq	$2,$0,$L80
	nop

	lw	$2,40($sp)
	lbu	$2,3($2)
	andi	$2,$2,0xf8
	beq	$2,$0,$L81
	nop

	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,136($2)
	andi	$2,$2,0x1
	beq	$2,$0,$L82
	nop

	lui	$2,%hi(jiffies)
	lw	$16,%lo(jiffies)($2)
	lui	$2,%hi($LC0)
	addiu	$4,$2,%lo($LC0)
	li	$5,47			# 0x2f
	lui	$2,%hi(strrchr)
	addiu	$2,$2,%lo(strrchr)
	jalr	$2
	nop

	addiu	$3,$2,1
	lui	$2,%hi($LC22)
	addiu	$4,$2,%lo($LC22)
	move	$5,$16
	move	$6,$3
	li	$7,329			# 0x149
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L82:
	li	$2,1			# 0x1
	j	$L73
	nop

$L81:
	lw	$3,16($sp)
	li	$2,-14337			# 0xffffffffffffc7ff
	and	$2,$3,$2
	lw	$3,40($sp)
	lbu	$3,3($3)
	sll	$3,$3,11
	or	$2,$2,$3
	sw	$2,16($sp)
$L80:
	lw	$2,40($sp)
	lw	$2,8($2)
	andi	$2,$2,0x10
	beq	$2,$0,$L83
	nop

	lw	$2,40($sp)
	lbu	$2,4($2)
	andi	$2,$2,0xfe
	beq	$2,$0,$L84
	nop

	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,136($2)
	andi	$2,$2,0x1
	beq	$2,$0,$L85
	nop

	lui	$2,%hi(jiffies)
	lw	$16,%lo(jiffies)($2)
	lui	$2,%hi($LC0)
	addiu	$4,$2,%lo($LC0)
	li	$5,47			# 0x2f
	lui	$2,%hi(strrchr)
	addiu	$2,$2,%lo(strrchr)
	jalr	$2
	nop

	addiu	$3,$2,1
	lui	$2,%hi($LC23)
	addiu	$4,$2,%lo($LC23)
	move	$5,$16
	move	$6,$3
	li	$7,338			# 0x152
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L85:
	li	$2,1			# 0x1
	j	$L73
	nop

$L84:
	lw	$2,40($sp)
	lbu	$2,4($2)
	sll	$2,$2,14
	move	$3,$2
	lw	$2,16($sp)
	and	$3,$3,$2
	li	$2,-16385			# 0xffffffffffffbfff
	and	$2,$3,$2
	sw	$2,16($sp)
$L83:
	lw	$2,16($sp)
	sw	$2,20($sp)
	lw	$4,20($sp)
	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$5,$2,0x20c
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	move	$2,$0
$L73:
	lw	$31,36($sp)
	lw	$16,32($sp)
	addiu	$sp,$sp,40
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	phy_gpon_psync_setting
	.size	phy_gpon_psync_setting, .-phy_gpon_psync_setting
	.rdata
	.align	2
$LC24:
	.ascii	"[%lu]0ms[%s:%d]\012  Phy Epon Frame Error Counter : %.8x"
	.ascii	"\012\000"
	.section	.text.phy_epon_frame_err_cnt,"ax",@progbits
	.align	2
	.globl	phy_epon_frame_err_cnt
	.set	nomips16
	.set	nomicromips
	.ent	phy_epon_frame_err_cnt
	.type	phy_epon_frame_err_cnt, @function
phy_epon_frame_err_cnt:
	.frame	$sp,48,$31		# vars= 16, regs= 2/0, args= 24, gp= 0
	.mask	0x80010000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-48
	sw	$31,44($sp)
	sw	$16,40($sp)
	li	$4,32			# 0x20
	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$5,$2,0x298
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,136($2)
	andi	$2,$2,0x10
	beq	$2,$0,$L87
	nop

	lui	$2,%hi(jiffies)
	lw	$16,%lo(jiffies)($2)
	lui	$2,%hi($LC0)
	addiu	$4,$2,%lo($LC0)
	li	$5,47			# 0x2f
	lui	$2,%hi(strrchr)
	addiu	$2,$2,%lo(strrchr)
	jalr	$2
	nop

	addiu	$3,$2,1
	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$2,$2,0x228
	sw	$2,24($sp)
	lw	$2,24($sp)
	lw	$2,0($2)
	sw	$2,28($sp)
	lw	$2,24($sp)
	lw	$2,0($2)
	sw	$2,28($sp)
	lw	$2,28($sp)
	sw	$2,16($sp)
	lui	$2,%hi($LC24)
	addiu	$4,$2,%lo($LC24)
	move	$5,$16
	move	$6,$3
	li	$7,369			# 0x171
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L87:
	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$2,$2,0x228
	sw	$2,32($sp)
	lw	$2,32($sp)
	lw	$2,0($2)
	sw	$2,36($sp)
	lw	$2,32($sp)
	lw	$2,0($2)
	sw	$2,36($sp)
	lw	$2,36($sp)
	lw	$31,44($sp)
	lw	$16,40($sp)
	addiu	$sp,$sp,48
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	phy_epon_frame_err_cnt
	.size	phy_epon_frame_err_cnt, .-phy_epon_frame_err_cnt
	.rdata
	.align	2
$LC25:
	.ascii	"[%lu]0ms[%s:%d]\012  Rx power is below low alarm. \012\000"
	.align	2
$LC26:
	.ascii	"[%lu]0ms[%s:%d]\012  Rx power exceeds high alarm. \012\000"
	.section	.text.phy_rx_power_alarm,"ax",@progbits
	.align	2
	.globl	phy_rx_power_alarm
	.set	nomips16
	.set	nomicromips
	.ent	phy_rx_power_alarm
	.type	phy_rx_power_alarm, @function
phy_rx_power_alarm:
	.frame	$sp,56,$31		# vars= 16, regs= 2/0, args= 32, gp= 0
	.mask	0x80010000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-56
	sw	$31,52($sp)
	sw	$16,48($sp)
	sw	$0,36($sp)
	sw	$0,32($sp)
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,116($2)
	move	$3,$2
	li	$2,113			# 0x71
	sw	$2,16($sp)
	addiu	$2,$sp,40
	sw	$2,20($sp)
	li	$2,1			# 0x1
	sw	$2,24($sp)
	move	$4,$0
	move	$5,$3
	li	$6,81			# 0x51
	li	$7,1			# 0x1
	lui	$2,%hi(phy_I2C_read)
	addiu	$2,$2,%lo(phy_I2C_read)
	jalr	$2
	nop

	lbu	$2,40($sp)
	sw	$2,36($sp)
	lw	$2,36($sp)
	andi	$2,$2,0x40
	beq	$2,$0,$L92
	nop

	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,136($2)
	andi	$2,$2,0x10
	beq	$2,$0,$L93
	nop

	lui	$2,%hi(jiffies)
	lw	$16,%lo(jiffies)($2)
	lui	$2,%hi($LC0)
	addiu	$4,$2,%lo($LC0)
	li	$5,47			# 0x2f
	lui	$2,%hi(strrchr)
	addiu	$2,$2,%lo(strrchr)
	jalr	$2
	nop

	addiu	$3,$2,1
	lui	$2,%hi($LC25)
	addiu	$4,$2,%lo($LC25)
	move	$5,$16
	move	$6,$3
	li	$7,409			# 0x199
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L93:
	lw	$2,32($sp)
	ori	$2,$2,0x20
	sw	$2,32($sp)
$L92:
	lw	$2,36($sp)
	andi	$2,$2,0x80
	beq	$2,$0,$L94
	nop

	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,136($2)
	andi	$2,$2,0x10
	beq	$2,$0,$L95
	nop

	lui	$2,%hi(jiffies)
	lw	$16,%lo(jiffies)($2)
	lui	$2,%hi($LC0)
	addiu	$4,$2,%lo($LC0)
	li	$5,47			# 0x2f
	lui	$2,%hi(strrchr)
	addiu	$2,$2,%lo(strrchr)
	jalr	$2
	nop

	addiu	$3,$2,1
	lui	$2,%hi($LC26)
	addiu	$4,$2,%lo($LC26)
	move	$5,$16
	move	$6,$3
	li	$7,415			# 0x19f
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L95:
	lw	$2,32($sp)
	ori	$2,$2,0x10
	sw	$2,32($sp)
$L94:
	lw	$2,32($sp)
	lw	$31,52($sp)
	lw	$16,48($sp)
	addiu	$sp,$sp,56
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	phy_rx_power_alarm
	.size	phy_rx_power_alarm, .-phy_rx_power_alarm
	.local	ber_counter.24683
	.comm	ber_counter.24683,4,4
	.ident	"GCC: (Buildroot 2015.08.1) 4.9.3"
