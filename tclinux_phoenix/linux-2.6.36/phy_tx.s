	.file	1 "phy_tx.c"
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
	.section	.text.phy_gpon_preamble,"ax",@progbits
	.align	2
	.globl	phy_gpon_preamble
	.set	nomips16
	.set	nomicromips
	.ent	phy_gpon_preamble
	.type	phy_gpon_preamble, @function
phy_gpon_preamble:
	.frame	$sp,40,$31		# vars= 16, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-40
	sw	$31,36($sp)
	sw	$4,40($sp)
	sw	$0,20($sp)
	sw	$0,16($sp)
	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$2,$2,0x400
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
	lw	$2,12($2)
	andi	$2,$2,0x1
	beq	$2,$0,$L3
	nop

	lw	$3,16($sp)
	li	$2,-256			# 0xffffffffffffff00
	and	$2,$3,$2
	lw	$3,40($sp)
	lbu	$3,0($3)
	or	$2,$2,$3
	sw	$2,16($sp)
$L3:
	lw	$2,40($sp)
	lw	$2,12($2)
	andi	$2,$2,0x2
	beq	$2,$0,$L4
	nop

	lw	$3,16($sp)
	li	$2,-65536			# 0xffffffffffff0000
	ori	$2,$2,0xff
	and	$2,$3,$2
	lw	$3,40($sp)
	lbu	$3,1($3)
	sll	$3,$3,8
	or	$2,$2,$3
	sw	$2,16($sp)
$L4:
	lw	$2,40($sp)
	lw	$2,12($2)
	andi	$2,$2,0x4
	beq	$2,$0,$L5
	nop

	lw	$3,16($sp)
	li	$2,-16777216			# 0xffffffffff000000
	ori	$2,$2,0xffff
	and	$2,$3,$2
	lw	$3,40($sp)
	lbu	$3,2($3)
	sll	$3,$3,16
	or	$2,$2,$3
	sw	$2,16($sp)
$L5:
	lw	$2,40($sp)
	lw	$2,12($2)
	andi	$2,$2,0x8
	beq	$2,$0,$L6
	nop

	lw	$2,16($sp)
	ext	$2,$2,0,24
	lw	$3,40($sp)
	lbu	$3,3($3)
	sll	$3,$3,24
	or	$2,$2,$3
	sw	$2,16($sp)
$L6:
	lw	$2,16($sp)
	sw	$2,20($sp)
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lw	$3,20($sp)
	sw	$3,180($2)
	lw	$4,20($sp)
	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$5,$2,0x400
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
	.end	phy_gpon_preamble
	.size	phy_gpon_preamble, .-phy_gpon_preamble
	.rdata
	.align	2
$LC0:
	.ascii	"/opt/tclinux_phoenix/modules/private/xpon_phy/src/phy_tx"
	.ascii	".c\000"
	.align	2
$LC1:
	.ascii	"[%lu]0ms[%s:%d]\012  TX FEC on. \012\000"
	.align	2
$LC2:
	.ascii	"[%lu]0ms[%s:%d]\012  TX FEC off. \012\000"
	.section	.text.phy_tx_fec_status,"ax",@progbits
	.align	2
	.globl	phy_tx_fec_status
	.set	nomips16
	.set	nomicromips
	.ent	phy_tx_fec_status
	.type	phy_tx_fec_status, @function
phy_tx_fec_status:
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
	ori	$2,$2,0x40c
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
	beq	$2,$0,$L10
	nop

	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,136($2)
	andi	$2,$2,0x10
	beq	$2,$0,$L11
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
	lui	$2,%hi($LC1)
	addiu	$4,$2,%lo($LC1)
	move	$5,$16
	move	$6,$3
	li	$7,93			# 0x5d
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L11:
	li	$2,1			# 0x1
	j	$L12
	nop

$L10:
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,136($2)
	andi	$2,$2,0x10
	beq	$2,$0,$L13
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
	lui	$2,%hi($LC2)
	addiu	$4,$2,%lo($LC2)
	move	$5,$16
	move	$6,$3
	li	$7,98			# 0x62
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L13:
	move	$2,$0
$L12:
	lw	$31,36($sp)
	lw	$16,32($sp)
	addiu	$sp,$sp,40
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	phy_tx_fec_status
	.size	phy_tx_fec_status, .-phy_tx_fec_status
	.rdata
	.align	2
$LC3:
	.ascii	"[%lu]0ms[%s:%d]\012  error input : delimiter pattern. \012"
	.ascii	"\000"
	.section	.text.phy_gpon_delimiter_guard,"ax",@progbits
	.align	2
	.globl	phy_gpon_delimiter_guard
	.set	nomips16
	.set	nomicromips
	.ent	phy_gpon_delimiter_guard
	.type	phy_gpon_delimiter_guard, @function
phy_gpon_delimiter_guard:
	.frame	$sp,32,$31		# vars= 8, regs= 2/0, args= 16, gp= 0
	.mask	0x80010000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-32
	sw	$31,28($sp)
	sw	$16,24($sp)
	sw	$4,32($sp)
	move	$2,$5
	sb	$2,36($sp)
	sw	$0,16($sp)
	lw	$3,32($sp)
	li	$2,-16777216			# 0xffffffffff000000
	and	$2,$3,$2
	beq	$2,$0,$L15
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
	lui	$2,%hi($LC3)
	addiu	$4,$2,%lo($LC3)
	move	$5,$16
	move	$6,$3
	li	$7,121			# 0x79
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L16:
	li	$2,1			# 0x1
	j	$L17
	nop

$L15:
	lbu	$2,36($sp)
	sll	$2,$2,24
	move	$3,$2
	lw	$2,32($sp)
	or	$2,$3,$2
	sw	$2,16($sp)
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lw	$3,16($sp)
	sw	$3,176($2)
	lw	$4,16($sp)
	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$5,$2,0x404
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	move	$2,$0
$L17:
	lw	$31,28($sp)
	lw	$16,24($sp)
	addiu	$sp,$sp,32
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	phy_gpon_delimiter_guard
	.size	phy_gpon_delimiter_guard, .-phy_gpon_delimiter_guard
	.rdata
	.align	2
$LC4:
	.ascii	"[%lu]0ms[%s:%d]\012  error input : extend_burst_mode. \012"
	.ascii	"\000"
	.align	2
$LC5:
	.ascii	"[%lu]0ms[%s:%d]\012  error input : oper_ranged_st. \012\000"
	.align	2
$LC6:
	.ascii	"[%lu]0ms[%s:%d]\012  error input : dis_scramble. \012\000"
	.section	.text.phy_gpon_extend_preamble,"ax",@progbits
	.align	2
	.globl	phy_gpon_extend_preamble
	.set	nomips16
	.set	nomicromips
	.ent	phy_gpon_extend_preamble
	.type	phy_gpon_extend_preamble, @function
phy_gpon_extend_preamble:
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
	ori	$2,$2,0x408
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
	lw	$2,12($2)
	andi	$2,$2,0x10
	beq	$2,$0,$L20
	nop

	lw	$3,16($sp)
	li	$2,-256			# 0xffffffffffffff00
	and	$2,$3,$2
	lw	$3,40($sp)
	lbu	$3,4($3)
	or	$2,$2,$3
	sw	$2,16($sp)
$L20:
	lw	$2,40($sp)
	lw	$2,12($2)
	andi	$2,$2,0x20
	beq	$2,$0,$L21
	nop

	lw	$3,16($sp)
	li	$2,-65536			# 0xffffffffffff0000
	ori	$2,$2,0xff
	and	$2,$3,$2
	lw	$3,40($sp)
	lbu	$3,5($3)
	sll	$3,$3,8
	or	$2,$2,$3
	sw	$2,16($sp)
$L21:
	lw	$2,40($sp)
	lw	$2,12($2)
	andi	$2,$2,0x40
	beq	$2,$0,$L22
	nop

	lw	$2,40($sp)
	lbu	$3,6($2)
	li	$2,1			# 0x1
	beq	$3,$2,$L23
	nop

	lw	$2,40($sp)
	lbu	$2,6($2)
	beq	$2,$0,$L23
	nop

	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,136($2)
	andi	$2,$2,0x1
	beq	$2,$0,$L24
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
	lui	$2,%hi($LC4)
	addiu	$4,$2,%lo($LC4)
	move	$5,$16
	move	$6,$3
	li	$7,159			# 0x9f
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L24:
	li	$2,1			# 0x1
	j	$L25
	nop

$L23:
	lw	$3,16($sp)
	li	$2,-131072			# 0xfffffffffffe0000
	ori	$2,$2,0xffff
	and	$2,$3,$2
	lw	$3,40($sp)
	lbu	$3,6($3)
	sll	$3,$3,16
	or	$2,$2,$3
	sw	$2,16($sp)
$L22:
	lw	$2,40($sp)
	lw	$2,12($2)
	andi	$2,$2,0x80
	beq	$2,$0,$L26
	nop

	lw	$2,40($sp)
	lbu	$2,7($2)
	andi	$2,$2,0xfc
	beq	$2,$0,$L27
	nop

	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,136($2)
	andi	$2,$2,0x1
	beq	$2,$0,$L28
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
	lui	$2,%hi($LC5)
	addiu	$4,$2,%lo($LC5)
	move	$5,$16
	move	$6,$3
	li	$7,168			# 0xa8
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L28:
	li	$2,1			# 0x1
	j	$L25
	nop

$L27:
	lw	$3,16($sp)
	li	$2,-458752			# 0xfffffffffff90000
	ori	$2,$2,0xffff
	and	$2,$3,$2
	lw	$3,40($sp)
	lbu	$3,7($3)
	sll	$3,$3,17
	or	$2,$2,$3
	sw	$2,16($sp)
$L26:
	lw	$2,40($sp)
	lw	$2,12($2)
	andi	$2,$2,0x100
	beq	$2,$0,$L29
	nop

	lw	$2,40($sp)
	lbu	$2,9($2)
	andi	$2,$2,0xfe
	beq	$2,$0,$L30
	nop

	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,136($2)
	andi	$2,$2,0x1
	beq	$2,$0,$L31
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
	lui	$2,%hi($LC5)
	addiu	$4,$2,%lo($LC5)
	move	$5,$16
	move	$6,$3
	li	$7,178			# 0xb2
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L31:
	li	$2,1			# 0x1
	j	$L25
	nop

$L30:
	lw	$3,16($sp)
	li	$2,-589824			# 0xfffffffffff70000
	ori	$2,$2,0xffff
	and	$2,$3,$2
	lw	$3,40($sp)
	lbu	$3,9($3)
	sll	$3,$3,19
	or	$2,$2,$3
	sw	$2,16($sp)
$L29:
	lw	$2,40($sp)
	lw	$2,12($2)
	andi	$2,$2,0x200
	beq	$2,$0,$L32
	nop

	lw	$2,40($sp)
	lbu	$3,8($2)
	li	$2,1			# 0x1
	beq	$3,$2,$L33
	nop

	lw	$2,40($sp)
	lbu	$2,8($2)
	beq	$2,$0,$L33
	nop

	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,136($2)
	andi	$2,$2,0x1
	beq	$2,$0,$L34
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
	lui	$2,%hi($LC6)
	addiu	$4,$2,%lo($LC6)
	move	$5,$16
	move	$6,$3
	li	$7,188			# 0xbc
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L34:
	li	$2,1			# 0x1
	j	$L25
	nop

$L33:
	lw	$3,16($sp)
	li	$2,-16842752			# 0xfffffffffeff0000
	ori	$2,$2,0xffff
	and	$2,$3,$2
	lw	$3,40($sp)
	lbu	$3,8($3)
	sll	$3,$3,24
	or	$2,$2,$3
	sw	$2,16($sp)
$L32:
	lw	$2,16($sp)
	sw	$2,20($sp)
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lw	$3,20($sp)
	sw	$3,184($2)
	lw	$4,20($sp)
	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$5,$2,0x408
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	move	$2,$0
$L25:
	lw	$31,36($sp)
	lw	$16,32($sp)
	addiu	$sp,$sp,40
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	phy_gpon_extend_preamble
	.size	phy_gpon_extend_preamble, .-phy_gpon_extend_preamble
	.rdata
	.align	2
$LC7:
	.ascii	"[%lu]0ms[%s:%d]\012  error input : rogue mode. \012\000"
	.align	2
$LC8:
	.ascii	"[%lu]0ms[%s:%d]\012  PON mode error. \012\000"
	.align	2
$LC9:
	.ascii	"ECNT_HOOK_ERROR occur with event id:%d. %s:%d\012\000"
	.align	2
$LC10:
	.ascii	"[%lu]0ms[%s:%d]\012  Phy_Rogue_PRBS mode\012\000"
	.align	2
$LC11:
	.ascii	"[%lu]0ms[%s:%d]\012  Phy_NO_Rogue_PRBS mode\012\000"
	.section	.text.phy_rogue_prbs_config,"ax",@progbits
	.align	2
	.globl	phy_rogue_prbs_config
	.set	nomips16
	.set	nomicromips
	.ent	phy_rogue_prbs_config
	.type	phy_rogue_prbs_config, @function
phy_rogue_prbs_config:
	.frame	$sp,144,$31		# vars= 120, regs= 2/0, args= 16, gp= 0
	.mask	0x80010000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-144
	sw	$31,140($sp)
	sw	$16,136($sp)
	move	$2,$4
	sb	$2,144($sp)
	sw	$0,16($sp)
	sw	$0,20($sp)
	lbu	$3,144($sp)
	li	$2,1			# 0x1
	beq	$3,$2,$L36
	nop

	lbu	$2,144($sp)
	beq	$2,$0,$L36
	nop

	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,136($2)
	andi	$2,$2,0x1
	beq	$2,$0,$L37
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
	lui	$2,%hi($LC7)
	addiu	$4,$2,%lo($LC7)
	move	$5,$16
	move	$6,$3
	li	$7,227			# 0xe3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L37:
	li	$2,1			# 0x1
	j	$L38
	nop

$L36:
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lw	$3,132($2)
	li	$2,100663296			# 0x6000000
	and	$3,$3,$2
	li	$2,33554432			# 0x2000000
	beq	$3,$2,$L39
	nop

	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lw	$3,132($2)
	li	$2,100663296			# 0x6000000
	and	$2,$3,$2
	beq	$2,$0,$L39
	nop

	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,136($2)
	andi	$2,$2,0x1
	beq	$2,$0,$L40
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
	lui	$2,%hi($LC8)
	addiu	$4,$2,%lo($LC8)
	move	$5,$16
	move	$6,$3
	li	$7,234			# 0xea
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L40:
	li	$2,1			# 0x1
	j	$L38
	nop

$L39:
	lbu	$3,144($sp)
	li	$2,1			# 0x1
	bne	$3,$2,$L41
	nop

	li	$2,257			# 0x101
	sw	$2,24($sp)
	sw	$0,116($sp)
	sw	$0,120($sp)
	sw	$0,124($sp)
	sw	$0,128($sp)
	sw	$0,132($sp)
	lw	$2,24($sp)
	sw	$2,120($sp)
	sw	$0,124($sp)
	addiu	$2,$sp,116
	sw	$2,128($sp)
	addiu	$2,$sp,124
	li	$4,24			# 0x18
	move	$5,$0
	move	$6,$2
	lui	$2,%hi(__ECNT_HOOK)
	addiu	$2,$2,%lo(__ECNT_HOOK)
	jalr	$2
	nop

	move	$3,$2
	li	$2,-1			# 0xffffffffffffffff
	bne	$3,$2,$L42
	nop

	lui	$2,%hi($LC9)
	addiu	$4,$2,%lo($LC9)
	lw	$5,24($sp)
	lui	$2,%hi(__func__.24679)
	addiu	$6,$2,%lo(__func__.24679)
	li	$7,107			# 0x6b
	lui	$2,%hi(panic)
	addiu	$2,$2,%lo(panic)
	jalr	$2
	nop

$L42:
	lui	$2,%hi(gpPhyPriv)
	lw	$3,%lo(gpPhyPriv)($2)
	lbu	$2,132($3)
	li	$4,1			# 0x1
	ins	$2,$4,0,1
	sb	$2,132($3)
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lw	$3,132($2)
	li	$2,100663296			# 0x6000000
	and	$3,$3,$2
	li	$2,33554432			# 0x2000000
	bne	$3,$2,$L43
	nop

	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$2,$2,0x1fc
	sw	$2,28($sp)
	lw	$2,28($sp)
	lw	$2,0($2)
	sw	$2,32($sp)
	lw	$2,28($sp)
	lw	$2,0($2)
	sw	$2,32($sp)
	lw	$2,32($sp)
	sw	$2,20($sp)
	lw	$3,20($sp)
	li	$2,268435456			# 0x10000000
	or	$2,$3,$2
	sw	$2,16($sp)
	lw	$4,16($sp)
	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$5,$2,0x1fc
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$2,$2,0x4a0
	sw	$2,36($sp)
	lw	$2,36($sp)
	lw	$2,0($2)
	sw	$2,40($sp)
	lw	$2,36($sp)
	lw	$2,0($2)
	sw	$2,40($sp)
	lw	$2,40($sp)
	sw	$2,20($sp)
	lw	$2,20($sp)
	ori	$2,$2,0x5
	sw	$2,16($sp)
	lw	$4,16($sp)
	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$5,$2,0x4a0
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$2,$2,0x4a4
	sw	$2,44($sp)
	lw	$2,44($sp)
	lw	$2,0($2)
	sw	$2,48($sp)
	lw	$2,44($sp)
	lw	$2,0($2)
	sw	$2,48($sp)
	lw	$2,48($sp)
	sw	$2,20($sp)
	lw	$2,20($sp)
	ori	$2,$2,0x1
	sw	$2,16($sp)
	lw	$4,16($sp)
	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$5,$2,0x4a4
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	j	$L47
	nop

$L43:
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lw	$3,132($2)
	li	$2,100663296			# 0x6000000
	and	$2,$3,$2
	bne	$2,$0,$L47
	nop

	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$2,$2,0x108
	sw	$2,52($sp)
	lw	$2,52($sp)
	lw	$2,0($2)
	sw	$2,56($sp)
	lw	$2,52($sp)
	lw	$2,0($2)
	sw	$2,56($sp)
	lw	$2,56($sp)
	sw	$2,20($sp)
	lw	$2,20($sp)
	ori	$2,$2,0x80
	sw	$2,16($sp)
	lw	$4,16($sp)
	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$5,$2,0x108
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

$L47:
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,136($2)
	andi	$2,$2,0x10
	beq	$2,$0,$L50
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
	li	$7,275			# 0x113
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	j	$L50
	nop

$L41:
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lw	$3,132($2)
	li	$2,100663296			# 0x6000000
	and	$3,$3,$2
	li	$2,33554432			# 0x2000000
	bne	$3,$2,$L51
	nop

	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$2,$2,0x1fc
	sw	$2,60($sp)
	lw	$2,60($sp)
	lw	$2,0($2)
	sw	$2,64($sp)
	lw	$2,60($sp)
	lw	$2,0($2)
	sw	$2,64($sp)
	lw	$2,64($sp)
	sw	$2,20($sp)
	lw	$3,20($sp)
	li	$2,-268500992			# 0xffffffffefff0000
	ori	$2,$2,0xffff
	and	$2,$3,$2
	sw	$2,16($sp)
	lw	$4,16($sp)
	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$5,$2,0x1fc
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$2,$2,0x4a0
	sw	$2,68($sp)
	lw	$2,68($sp)
	lw	$2,0($2)
	sw	$2,72($sp)
	lw	$2,68($sp)
	lw	$2,0($2)
	sw	$2,72($sp)
	lw	$2,72($sp)
	sw	$2,20($sp)
	lw	$3,20($sp)
	li	$2,-6			# 0xfffffffffffffffa
	and	$2,$3,$2
	sw	$2,16($sp)
	lw	$4,16($sp)
	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$5,$2,0x4a0
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$2,$2,0x4a4
	sw	$2,76($sp)
	lw	$2,76($sp)
	lw	$2,0($2)
	sw	$2,80($sp)
	lw	$2,76($sp)
	lw	$2,0($2)
	sw	$2,80($sp)
	lw	$2,80($sp)
	sw	$2,20($sp)
	lw	$3,20($sp)
	li	$2,-2			# 0xfffffffffffffffe
	and	$2,$3,$2
	sw	$2,16($sp)
	lw	$4,16($sp)
	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$5,$2,0x4a4
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	li	$4,64			# 0x40
	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$5,$2,0x5f4
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	j	$L55
	nop

$L51:
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lw	$3,132($2)
	li	$2,100663296			# 0x6000000
	and	$2,$3,$2
	bne	$2,$0,$L55
	nop

	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$2,$2,0x108
	sw	$2,84($sp)
	lw	$2,84($sp)
	lw	$2,0($2)
	sw	$2,88($sp)
	lw	$2,84($sp)
	lw	$2,0($2)
	sw	$2,88($sp)
	lw	$2,88($sp)
	sw	$2,20($sp)
	lw	$3,20($sp)
	li	$2,-129			# 0xffffffffffffff7f
	and	$2,$3,$2
	sw	$2,16($sp)
	lw	$4,16($sp)
	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$5,$2,0x108
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

$L55:
	li	$2,258			# 0x102
	sw	$2,92($sp)
	sw	$0,96($sp)
	sw	$0,100($sp)
	sw	$0,104($sp)
	sw	$0,108($sp)
	sw	$0,112($sp)
	lw	$2,92($sp)
	sw	$2,100($sp)
	sw	$0,104($sp)
	addiu	$2,$sp,96
	sw	$2,108($sp)
	addiu	$2,$sp,104
	li	$4,24			# 0x18
	move	$5,$0
	move	$6,$2
	lui	$2,%hi(__ECNT_HOOK)
	addiu	$2,$2,%lo(__ECNT_HOOK)
	jalr	$2
	nop

	move	$3,$2
	li	$2,-1			# 0xffffffffffffffff
	bne	$3,$2,$L57
	nop

	lui	$2,%hi($LC9)
	addiu	$4,$2,%lo($LC9)
	lw	$5,92($sp)
	lui	$2,%hi(__func__.24679)
	addiu	$6,$2,%lo(__func__.24679)
	li	$7,107			# 0x6b
	lui	$2,%hi(panic)
	addiu	$2,$2,%lo(panic)
	jalr	$2
	nop

$L57:
	lui	$2,%hi(gpPhyPriv)
	lw	$3,%lo(gpPhyPriv)($2)
	lbu	$2,132($3)
	ins	$2,$0,0,1
	sb	$2,132($3)
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,136($2)
	andi	$2,$2,0x10
	beq	$2,$0,$L50
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
	li	$7,319			# 0x13f
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L50:
	move	$2,$0
$L38:
	lw	$31,140($sp)
	lw	$16,136($sp)
	addiu	$sp,$sp,144
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	phy_rogue_prbs_config
	.size	phy_rogue_prbs_config, .-phy_rogue_prbs_config
	.rdata
	.align	2
$LC12:
	.ascii	"[%lu]0ms[%s:%d]\012  error input : burst_mode. \012\000"
	.align	2
$LC13:
	.ascii	"[%lu]0ms[%s:%d]\012  Phy_Tx_Burst_Config read_data:0x%.8"
	.ascii	"x\012\000"
	.align	2
$LC14:
	.ascii	"[%lu]0ms[%s:%d]\012  Phy_Tx_Burst_Config write_data:0x%."
	.ascii	"8x\012\000"
	.section	.text.phy_tx_burst_config,"ax",@progbits
	.align	2
	.globl	phy_tx_burst_config
	.set	nomips16
	.set	nomicromips
	.ent	phy_tx_burst_config
	.type	phy_tx_burst_config, @function
phy_tx_burst_config:
	.frame	$sp,56,$31		# vars= 24, regs= 2/0, args= 24, gp= 0
	.mask	0x80010000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-56
	sw	$31,52($sp)
	sw	$16,48($sp)
	move	$2,$4
	sb	$2,56($sp)
	lbu	$2,56($sp)
	beq	$2,$0,$L59
	nop

	lbu	$3,56($sp)
	li	$2,1			# 0x1
	beq	$3,$2,$L59
	nop

	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,136($2)
	andi	$2,$2,0x1
	beq	$2,$0,$L60
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
	li	$7,339			# 0x153
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L60:
	li	$2,1			# 0x1
	j	$L61
	nop

$L59:
	sw	$0,24($sp)
	sw	$0,28($sp)
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
	sw	$2,28($sp)
	lw	$3,28($sp)
	li	$2,-161			# 0xffffffffffffff5f
	and	$3,$3,$2
	lbu	$2,56($sp)
	sll	$2,$2,5
	sll	$4,$2,2
	addu	$2,$2,$4
	or	$2,$3,$2
	sw	$2,24($sp)
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
	lw	$2,28($sp)
	sw	$2,16($sp)
	lui	$2,%hi($LC13)
	addiu	$4,$2,%lo($LC13)
	move	$5,$16
	move	$6,$3
	li	$7,348			# 0x15c
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
	lw	$2,24($sp)
	sw	$2,16($sp)
	lui	$2,%hi($LC14)
	addiu	$4,$2,%lo($LC14)
	move	$5,$16
	move	$6,$3
	li	$7,349			# 0x15d
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L64:
	lw	$4,24($sp)
	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$5,$2,0x108
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	lui	$2,%hi(phy_los_status)
	addiu	$2,$2,%lo(phy_los_status)
	jalr	$2
	nop

	move	$3,$2
	li	$2,1			# 0x1
	bne	$3,$2,$L65
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
	sw	$2,28($sp)
	lw	$3,28($sp)
	li	$2,-2013265920			# 0xffffffff88000000
	or	$2,$3,$2
	sw	$2,24($sp)
	lw	$4,24($sp)
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

	lw	$4,28($sp)
	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$5,$2,0x108
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

$L65:
	lui	$2,%hi(gpPhyPriv)
	lw	$3,%lo(gpPhyPriv)($2)
	lbu	$2,56($sp)
	xori	$2,$2,0x1
	sltu	$2,$2,1
	andi	$4,$2,0x00ff
	lbu	$2,132($3)
	ins	$2,$4,6,1
	sb	$2,132($3)
	lbu	$2,56($sp)
	bne	$2,$0,$L67
	nop

	li	$4,1			# 0x1
	lui	$2,%hi(phy_tx_power_config)
	addiu	$2,$2,%lo(phy_tx_power_config)
	jalr	$2
	nop

	li	$4,1			# 0x1
	lui	$2,%hi(phy_trans_power_switch)
	addiu	$2,$2,%lo(phy_trans_power_switch)
	jalr	$2
	nop

$L67:
	move	$2,$0
$L61:
	lw	$31,52($sp)
	lw	$16,48($sp)
	addiu	$sp,$sp,56
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	phy_tx_burst_config
	.size	phy_tx_burst_config, .-phy_tx_burst_config
	.section	.text.phy_tx_burst_getting,"ax",@progbits
	.align	2
	.globl	phy_tx_burst_getting
	.set	nomips16
	.set	nomicromips
	.ent	phy_tx_burst_getting
	.type	phy_tx_burst_getting, @function
phy_tx_burst_getting:
	.frame	$sp,16,$31		# vars= 16, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-16
	sw	$0,0($sp)
	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$2,$2,0x108
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
	andi	$2,$2,0xa0
	bne	$2,$0,$L70
	nop

	move	$2,$0
	j	$L71
	nop

$L70:
	li	$2,1			# 0x1
$L71:
	addiu	$sp,$sp,16
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	phy_tx_burst_getting
	.size	phy_tx_burst_getting, .-phy_tx_burst_getting
	.section	.text.phy_gpon_tx_power,"ax",@progbits
	.align	2
	.globl	phy_gpon_tx_power
	.set	nomips16
	.set	nomicromips
	.ent	phy_gpon_tx_power
	.type	phy_gpon_tx_power, @function
phy_gpon_tx_power:
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	move	$2,$4
	sh	$2,0($sp)
	move	$2,$0
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	phy_gpon_tx_power
	.size	phy_gpon_tx_power, .-phy_gpon_tx_power
	.rdata
	.align	2
$LC15:
	.ascii	"[%lu]0ms[%s:%d]\012  error input : amp_level. \012\000"
	.section	.text.phy_tx_amp_setting,"ax",@progbits
	.align	2
	.globl	phy_tx_amp_setting
	.set	nomips16
	.set	nomicromips
	.ent	phy_tx_amp_setting
	.type	phy_tx_amp_setting, @function
phy_tx_amp_setting:
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
	lbu	$2,40($sp)
	andi	$2,$2,0xf0
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
	lui	$2,%hi($LC15)
	addiu	$4,$2,%lo($LC15)
	move	$5,$16
	move	$6,$3
	li	$7,414			# 0x19e
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L76:
	li	$2,1			# 0x1
	j	$L77
	nop

$L75:
	sw	$0,16($sp)
	sw	$0,20($sp)
	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$2,$2,0x1a0
	sw	$2,24($sp)
	lw	$2,24($sp)
	lw	$2,0($2)
	sw	$2,28($sp)
	lw	$2,24($sp)
	lw	$2,0($2)
	sw	$2,28($sp)
	lw	$2,28($sp)
	sw	$2,16($sp)
	lw	$3,16($sp)
	li	$2,-1048576			# 0xfffffffffff00000
	ori	$2,$2,0xffff
	and	$2,$3,$2
	lbu	$3,40($sp)
	sll	$3,$3,16
	or	$2,$2,$3
	sw	$2,20($sp)
	lw	$4,20($sp)
	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$5,$2,0x1a0
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	move	$2,$0
$L77:
	lw	$31,36($sp)
	lw	$16,32($sp)
	addiu	$sp,$sp,40
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	phy_tx_amp_setting
	.size	phy_tx_amp_setting, .-phy_tx_amp_setting
	.rdata
	.align	2
$LC16:
	.ascii	"[%lu]0ms[%s:%d]\012  error input : tx_sd_inv.\012\000"
	.align	2
$LC17:
	.ascii	"[%lu]0ms[%s:%d]\012  error input : burst_en_inv.\012\000"
	.align	2
$LC18:
	.ascii	"[%lu]0ms[%s:%d]\012  error input : tx_fault_inv.\012\000"
	.section	.text.phy_trans_tx_setting,"ax",@progbits
	.align	2
	.globl	phy_trans_tx_setting
	.set	nomips16
	.set	nomicromips
	.ent	phy_trans_tx_setting
	.type	phy_trans_tx_setting, @function
phy_trans_tx_setting:
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
	ori	$2,$2,0x138
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
	lbu	$3,0($2)
	li	$2,1			# 0x1
	beq	$3,$2,$L81
	nop

	lw	$2,40($sp)
	lbu	$2,0($2)
	bne	$2,$0,$L82
	nop

$L81:
	lw	$3,16($sp)
	li	$2,-17			# 0xffffffffffffffef
	and	$2,$3,$2
	lw	$3,40($sp)
	lbu	$3,0($3)
	sll	$3,$3,4
	or	$2,$2,$3
	sw	$2,16($sp)
	j	$L83
	nop

$L82:
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,136($2)
	andi	$2,$2,0x1
	beq	$2,$0,$L83
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
	li	$7,446			# 0x1be
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L83:
	lw	$2,40($sp)
	lbu	$3,1($2)
	li	$2,1			# 0x1
	beq	$3,$2,$L84
	nop

	lw	$2,40($sp)
	lbu	$2,1($2)
	bne	$2,$0,$L85
	nop

$L84:
	lw	$3,16($sp)
	li	$2,-129			# 0xffffffffffffff7f
	and	$2,$3,$2
	lw	$3,40($sp)
	lbu	$3,1($3)
	sll	$3,$3,7
	or	$2,$2,$3
	sw	$2,16($sp)
	j	$L86
	nop

$L85:
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,136($2)
	andi	$2,$2,0x1
	beq	$2,$0,$L86
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
	lui	$2,%hi($LC17)
	addiu	$4,$2,%lo($LC17)
	move	$5,$16
	move	$6,$3
	li	$7,451			# 0x1c3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L86:
	lw	$2,40($sp)
	lbu	$3,2($2)
	li	$2,1			# 0x1
	beq	$3,$2,$L87
	nop

	lw	$2,40($sp)
	lbu	$2,2($2)
	bne	$2,$0,$L88
	nop

$L87:
	lw	$3,16($sp)
	li	$2,-33			# 0xffffffffffffffdf
	and	$2,$3,$2
	lw	$3,40($sp)
	lbu	$3,2($3)
	sll	$3,$3,5
	or	$2,$2,$3
	sw	$2,16($sp)
	j	$L89
	nop

$L88:
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,136($2)
	andi	$2,$2,0x1
	beq	$2,$0,$L89
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
	lui	$2,%hi($LC18)
	addiu	$4,$2,%lo($LC18)
	move	$5,$16
	move	$6,$3
	li	$7,456			# 0x1c8
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L89:
	lw	$2,16($sp)
	sw	$2,20($sp)
	lw	$4,20($sp)
	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$5,$2,0x138
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	move	$2,$0
	lw	$31,36($sp)
	lw	$16,32($sp)
	addiu	$sp,$sp,40
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	phy_trans_tx_setting
	.size	phy_trans_tx_setting, .-phy_trans_tx_setting
	.section	.text.phy_trans_tx_getting,"ax",@progbits
	.align	2
	.globl	phy_trans_tx_getting
	.set	nomips16
	.set	nomicromips
	.ent	phy_trans_tx_getting
	.type	phy_trans_tx_getting, @function
phy_trans_tx_getting:
	.frame	$sp,16,$31		# vars= 16, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-16
	sw	$4,16($sp)
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
	andi	$2,$2,0x10
	beq	$2,$0,$L93
	nop

	lw	$2,16($sp)
	li	$3,1			# 0x1
	sb	$3,0($2)
	j	$L94
	nop

$L93:
	lw	$2,16($sp)
	sb	$0,0($2)
$L94:
	lw	$2,0($sp)
	andi	$2,$2,0x80
	beq	$2,$0,$L95
	nop

	lw	$2,16($sp)
	li	$3,1			# 0x1
	sb	$3,1($2)
	j	$L96
	nop

$L95:
	lw	$2,16($sp)
	sb	$0,1($2)
$L96:
	lw	$2,0($sp)
	andi	$2,$2,0x20
	beq	$2,$0,$L97
	nop

	lw	$2,16($sp)
	li	$3,1			# 0x1
	sb	$3,2($2)
	j	$L91
	nop

$L97:
	lw	$2,16($sp)
	sb	$0,2($2)
$L91:
	addiu	$sp,$sp,16
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	phy_trans_tx_getting
	.size	phy_trans_tx_getting, .-phy_trans_tx_getting
	.rdata
	.align	2
$LC19:
	.ascii	"[%lu]0ms[%s:%d]\012  Tx power is below low alarm. \012\000"
	.align	2
$LC20:
	.ascii	"[%lu]0ms[%s:%d]\012  Tx power exceeds high alarm. \012\000"
	.align	2
$LC21:
	.ascii	"[%lu]0ms[%s:%d]\012  Bias Current is below low alarm. \012"
	.ascii	"\000"
	.align	2
$LC22:
	.ascii	"[%lu]0ms[%s:%d]\012  Bias Current exceeds high alarm. \012"
	.ascii	"\000"
	.section	.text.phy_tx_alarm,"ax",@progbits
	.align	2
	.globl	phy_tx_alarm
	.set	nomips16
	.set	nomicromips
	.ent	phy_tx_alarm
	.type	phy_tx_alarm, @function
phy_tx_alarm:
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
	li	$2,112			# 0x70
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
	andi	$2,$2,0x1
	beq	$2,$0,$L100
	nop

	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,136($2)
	andi	$2,$2,0x10
	beq	$2,$0,$L101
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
	li	$7,523			# 0x20b
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L101:
	lw	$2,32($sp)
	ori	$2,$2,0x2
	sw	$2,32($sp)
$L100:
	lw	$2,36($sp)
	andi	$2,$2,0x2
	beq	$2,$0,$L102
	nop

	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,136($2)
	andi	$2,$2,0x10
	beq	$2,$0,$L103
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
	li	$7,529			# 0x211
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L103:
	lw	$2,32($sp)
	ori	$2,$2,0x1
	sw	$2,32($sp)
$L102:
	lw	$2,36($sp)
	andi	$2,$2,0x4
	beq	$2,$0,$L104
	nop

	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,136($2)
	andi	$2,$2,0x10
	beq	$2,$0,$L105
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
	li	$7,535			# 0x217
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L105:
	lw	$2,32($sp)
	ori	$2,$2,0x8
	sw	$2,32($sp)
$L104:
	lw	$2,36($sp)
	andi	$2,$2,0x8
	beq	$2,$0,$L106
	nop

	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,136($2)
	andi	$2,$2,0x10
	beq	$2,$0,$L107
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
	li	$7,541			# 0x21d
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L107:
	lw	$2,32($sp)
	ori	$2,$2,0x4
	sw	$2,32($sp)
$L106:
	lw	$2,32($sp)
	lw	$31,52($sp)
	lw	$16,48($sp)
	addiu	$sp,$sp,56
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	phy_tx_alarm
	.size	phy_tx_alarm, .-phy_tx_alarm
	.rdata
	.align	2
$LC23:
	.ascii	"[%lu]0ms[%s:%d]\012  Phy Tx Frame Counter : %.8x\012\000"
	.section	.text.phy_tx_frame_counter,"ax",@progbits
	.align	2
	.globl	phy_tx_frame_counter
	.set	nomips16
	.set	nomicromips
	.ent	phy_tx_frame_counter
	.type	phy_tx_frame_counter, @function
phy_tx_frame_counter:
	.frame	$sp,56,$31		# vars= 24, regs= 2/0, args= 24, gp= 0
	.mask	0x80010000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-56
	sw	$31,52($sp)
	sw	$16,48($sp)
	sw	$0,24($sp)
	sw	$0,28($sp)
	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$2,$2,0x424
	sw	$2,32($sp)
	lw	$2,32($sp)
	lw	$2,0($2)
	sw	$2,36($sp)
	lw	$2,32($sp)
	lw	$2,0($2)
	sw	$2,36($sp)
	lw	$2,36($sp)
	sw	$2,24($sp)
	lw	$2,24($sp)
	ori	$2,$2,0x8
	sw	$2,28($sp)
	lw	$4,28($sp)
	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$5,$2,0x424
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$2,$2,0x434
	sw	$2,40($sp)
	lw	$2,40($sp)
	lw	$2,0($2)
	sw	$2,44($sp)
	lw	$2,40($sp)
	lw	$2,0($2)
	sw	$2,44($sp)
	lw	$2,44($sp)
	sw	$2,24($sp)
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,136($2)
	andi	$2,$2,0x10
	beq	$2,$0,$L112
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
	lui	$2,%hi($LC23)
	addiu	$4,$2,%lo($LC23)
	move	$5,$16
	move	$6,$3
	li	$7,568			# 0x238
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L112:
	lw	$2,24($sp)
	lw	$31,52($sp)
	lw	$16,48($sp)
	addiu	$sp,$sp,56
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	phy_tx_frame_counter
	.size	phy_tx_frame_counter, .-phy_tx_frame_counter
	.rdata
	.align	2
$LC24:
	.ascii	"[%lu]0ms[%s:%d]\012  Phy Tx Burst Counter : %.8x\012\000"
	.section	.text.phy_tx_burst_counter,"ax",@progbits
	.align	2
	.globl	phy_tx_burst_counter
	.set	nomips16
	.set	nomicromips
	.ent	phy_tx_burst_counter
	.type	phy_tx_burst_counter, @function
phy_tx_burst_counter:
	.frame	$sp,56,$31		# vars= 24, regs= 2/0, args= 24, gp= 0
	.mask	0x80010000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-56
	sw	$31,52($sp)
	sw	$16,48($sp)
	sw	$0,24($sp)
	sw	$0,28($sp)
	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$2,$2,0x424
	sw	$2,32($sp)
	lw	$2,32($sp)
	lw	$2,0($2)
	sw	$2,36($sp)
	lw	$2,32($sp)
	lw	$2,0($2)
	sw	$2,36($sp)
	lw	$2,36($sp)
	sw	$2,24($sp)
	lw	$2,24($sp)
	ori	$2,$2,0x8
	sw	$2,28($sp)
	lw	$4,28($sp)
	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$5,$2,0x424
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$2,$2,0x438
	sw	$2,40($sp)
	lw	$2,40($sp)
	lw	$2,0($2)
	sw	$2,44($sp)
	lw	$2,40($sp)
	lw	$2,0($2)
	sw	$2,44($sp)
	lw	$2,44($sp)
	sw	$2,24($sp)
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,136($2)
	andi	$2,$2,0x10
	beq	$2,$0,$L117
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
	lui	$2,%hi($LC24)
	addiu	$4,$2,%lo($LC24)
	move	$5,$16
	move	$6,$3
	li	$7,594			# 0x252
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L117:
	lw	$31,52($sp)
	lw	$16,48($sp)
	addiu	$sp,$sp,56
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	phy_tx_burst_counter
	.size	phy_tx_burst_counter, .-phy_tx_burst_counter
	.section	.text.phy_tx_test_pattern,"ax",@progbits
	.align	2
	.globl	phy_tx_test_pattern
	.set	nomips16
	.set	nomicromips
	.ent	phy_tx_test_pattern
	.type	phy_tx_test_pattern, @function
phy_tx_test_pattern:
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
	sltu	$2,$2,13
	beq	$2,$0,$L119
	nop

	lw	$2,40($sp)
	sll	$3,$2,2
	lui	$2,%hi($L121)
	addiu	$2,$2,%lo($L121)
	addu	$2,$3,$2
	lw	$2,0($2)
	j	$2
	nop

	.rdata
	.align	2
	.align	2
$L121:
	.word	$L120
	.word	$L122
	.word	$L122
	.word	$L122
	.word	$L122
	.word	$L123
	.word	$L123
	.word	$L123
	.word	$L119
	.word	$L122
	.word	$L122
	.word	$L124
	.word	$L125
	.section	.text.phy_tx_test_pattern
$L120:
	move	$4,$0
	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$5,$2,0x4a4
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	lw	$4,40($sp)
	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$5,$2,0x4a0
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	j	$L118
	nop

$L122:
	move	$4,$0
	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$5,$2,0x4a4
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	lw	$4,40($sp)
	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$5,$2,0x4a0
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	j	$L118
	nop

$L123:
	lw	$4,40($sp)
	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$5,$2,0x4a0
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	li	$4,1			# 0x1
	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$5,$2,0x4a4
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	j	$L118
	nop

$L124:
	li	$4,2			# 0x2
	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$5,$2,0x520
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	li	$4,1			# 0x1
	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$5,$2,0x510
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	j	$L118
	nop

$L125:
	move	$4,$0
	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$5,$2,0x4a4
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	move	$4,$0
	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$5,$2,0x4a0
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	move	$4,$0
	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$5,$2,0x520
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	move	$4,$0
	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$5,$2,0x510
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$2,$2,0x42c
	sw	$2,24($sp)
	lw	$2,24($sp)
	lw	$2,0($2)
	sw	$2,28($sp)
	lw	$2,24($sp)
	lw	$2,0($2)
	sw	$2,28($sp)
	lw	$3,28($sp)
	li	$2,-3			# 0xfffffffffffffffd
	and	$2,$3,$2
	sw	$2,16($sp)
	lw	$2,16($sp)
	ori	$2,$2,0x2
	sw	$2,20($sp)
	lw	$4,20($sp)
	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$5,$2,0x42c
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	lw	$4,16($sp)
	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$5,$2,0x42c
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

$L119:
	move	$4,$0
	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$5,$2,0x4a4
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	move	$4,$0
	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$5,$2,0x4a0
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	nop
$L118:
	lw	$31,36($sp)
	addiu	$sp,$sp,40
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	phy_tx_test_pattern
	.size	phy_tx_test_pattern, .-phy_tx_test_pattern
	.rdata
	.align	2
$LC25:
	.ascii	"[%lu]0ms[%s:%d]\012  error input : byte dly. \012\000"
	.align	2
$LC26:
	.ascii	"[%lu]0ms[%s:%d]\012  Byte delay manual enable setting su"
	.ascii	"ccess. \012\000"
	.align	2
$LC27:
	.ascii	"[%lu]0ms[%s:%d]\012  Byte delay manual disable setting s"
	.ascii	"uccess. \012\000"
	.section	.text.phy_byte_delay_manual,"ax",@progbits
	.align	2
	.globl	phy_byte_delay_manual
	.set	nomips16
	.set	nomicromips
	.ent	phy_byte_delay_manual
	.type	phy_byte_delay_manual, @function
phy_byte_delay_manual:
	.frame	$sp,48,$31		# vars= 24, regs= 2/0, args= 16, gp= 0
	.mask	0x80010000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-48
	sw	$31,44($sp)
	sw	$16,40($sp)
	move	$3,$4
	move	$2,$5
	sb	$3,48($sp)
	sb	$2,52($sp)
	sw	$0,16($sp)
	sw	$0,20($sp)
	lbu	$3,48($sp)
	li	$2,1			# 0x1
	bne	$3,$2,$L129
	nop

	lbu	$2,52($sp)
	andi	$2,$2,0xfc
	beq	$2,$0,$L130
	nop

	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,136($2)
	andi	$2,$2,0x1
	beq	$2,$0,$L131
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
	li	$7,709			# 0x2c5
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L131:
	li	$2,1			# 0x1
	j	$L132
	nop

$L130:
	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$2,$2,0x430
	sw	$2,24($sp)
	lw	$2,24($sp)
	lw	$2,0($2)
	sw	$2,28($sp)
	lw	$2,24($sp)
	lw	$2,0($2)
	sw	$2,28($sp)
	lw	$2,28($sp)
	sw	$2,16($sp)
	lw	$3,16($sp)
	li	$2,-113			# 0xffffffffffffff8f
	and	$2,$3,$2
	lbu	$3,52($sp)
	sll	$3,$3,5
	or	$2,$2,$3
	ori	$2,$2,0x10
	sw	$2,20($sp)
	lw	$4,20($sp)
	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$5,$2,0x430
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,136($2)
	andi	$2,$2,0x10
	beq	$2,$0,$L134
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
	li	$7,715			# 0x2cb
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L134:
	move	$2,$0
	j	$L132
	nop

$L129:
	lbu	$2,48($sp)
	bne	$2,$0,$L135
	nop

	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$2,$2,0x430
	sw	$2,32($sp)
	lw	$2,32($sp)
	lw	$2,0($2)
	sw	$2,36($sp)
	lw	$2,32($sp)
	lw	$2,0($2)
	sw	$2,36($sp)
	lw	$2,36($sp)
	sw	$2,16($sp)
	lw	$3,16($sp)
	li	$2,-113			# 0xffffffffffffff8f
	and	$2,$3,$2
	sw	$2,20($sp)
	lw	$4,20($sp)
	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$5,$2,0x430
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,136($2)
	andi	$2,$2,0x10
	beq	$2,$0,$L137
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
	lui	$2,%hi($LC27)
	addiu	$4,$2,%lo($LC27)
	move	$5,$16
	move	$6,$3
	li	$7,723			# 0x2d3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L137:
	move	$2,$0
	j	$L132
	nop

$L135:
	li	$2,1			# 0x1
$L132:
	lw	$31,44($sp)
	lw	$16,40($sp)
	addiu	$sp,$sp,48
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	phy_byte_delay_manual
	.size	phy_byte_delay_manual, .-phy_byte_delay_manual
	.rdata
	.align	2
$LC28:
	.ascii	"[%lu]0ms[%s:%d]\012  Tx FEC manual enable setting succes"
	.ascii	"s. \012\000"
	.align	2
$LC29:
	.ascii	"[%lu]0ms[%s:%d]\012  Tx FEC manual disable setting succe"
	.ascii	"ss. \012\000"
	.section	.text.phy_tx_fec_manual,"ax",@progbits
	.align	2
	.globl	phy_tx_fec_manual
	.set	nomips16
	.set	nomicromips
	.ent	phy_tx_fec_manual
	.type	phy_tx_fec_manual, @function
phy_tx_fec_manual:
	.frame	$sp,48,$31		# vars= 24, regs= 2/0, args= 16, gp= 0
	.mask	0x80010000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-48
	sw	$31,44($sp)
	sw	$16,40($sp)
	move	$2,$4
	sb	$2,48($sp)
	sw	$0,16($sp)
	sw	$0,20($sp)
	lbu	$3,48($sp)
	li	$2,1			# 0x1
	bne	$3,$2,$L139
	nop

	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$2,$2,0x430
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
	ori	$2,$2,0xc
	sw	$2,20($sp)
	lw	$4,20($sp)
	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$5,$2,0x430
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,136($2)
	andi	$2,$2,0x10
	beq	$2,$0,$L141
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
	lui	$2,%hi($LC28)
	addiu	$4,$2,%lo($LC28)
	move	$5,$16
	move	$6,$3
	li	$7,749			# 0x2ed
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L141:
	move	$2,$0
	j	$L142
	nop

$L139:
	lbu	$2,48($sp)
	bne	$2,$0,$L143
	nop

	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$2,$2,0x430
	sw	$2,32($sp)
	lw	$2,32($sp)
	lw	$2,0($2)
	sw	$2,36($sp)
	lw	$2,32($sp)
	lw	$2,0($2)
	sw	$2,36($sp)
	lw	$2,36($sp)
	sw	$2,16($sp)
	lw	$3,16($sp)
	li	$2,-13			# 0xfffffffffffffff3
	and	$2,$3,$2
	sw	$2,20($sp)
	lw	$4,20($sp)
	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$5,$2,0x430
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,136($2)
	andi	$2,$2,0x10
	beq	$2,$0,$L145
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
	lui	$2,%hi($LC29)
	addiu	$4,$2,%lo($LC29)
	move	$5,$16
	move	$6,$3
	li	$7,757			# 0x2f5
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L145:
	move	$2,$0
	j	$L142
	nop

$L143:
	li	$2,1			# 0x1
$L142:
	lw	$31,44($sp)
	lw	$16,40($sp)
	addiu	$sp,$sp,48
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	phy_tx_fec_manual
	.size	phy_tx_fec_manual, .-phy_tx_fec_manual
	.rdata
	.align	2
$LC30:
	.ascii	"[%lu]0ms[%s:%d]\012  PHY Tx ctl type error. \012\000"
	.section	.text.phy_tx_ctl,"ax",@progbits
	.align	2
	.globl	phy_tx_ctl
	.set	nomips16
	.set	nomicromips
	.ent	phy_tx_ctl
	.type	phy_tx_ctl, @function
phy_tx_ctl:
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
	bne	$3,$2,$L147
	nop

	li	$4,42			# 0x2a
	lui	$2,%hi(ledTurnOff)
	addiu	$2,$2,%lo(ledTurnOff)
	jalr	$2
	nop

	j	$L146
	nop

$L147:
	lbu	$2,24($sp)
	bne	$2,$0,$L149
	nop

	li	$4,42			# 0x2a
	lui	$2,%hi(ledTurnOn)
	addiu	$2,$2,%lo(ledTurnOn)
	jalr	$2
	nop

	j	$L146
	nop

$L149:
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,136($2)
	andi	$2,$2,0x1
	beq	$2,$0,$L146
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
	lui	$2,%hi($LC30)
	addiu	$4,$2,%lo($LC30)
	move	$5,$16
	move	$6,$3
	li	$7,771			# 0x303
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L146:
	lw	$31,20($sp)
	lw	$16,16($sp)
	addiu	$sp,$sp,24
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	phy_tx_ctl
	.size	phy_tx_ctl, .-phy_tx_ctl
	.section	.text.mt7570_safe_circuit_reset_for_work_queue,"ax",@progbits
	.align	2
	.globl	mt7570_safe_circuit_reset_for_work_queue
	.set	nomips16
	.set	nomicromips
	.ent	mt7570_safe_circuit_reset_for_work_queue
	.type	mt7570_safe_circuit_reset_for_work_queue, @function
mt7570_safe_circuit_reset_for_work_queue:
	.frame	$sp,24,$31		# vars= 0, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-24
	sw	$31,20($sp)
	sw	$4,24($sp)
	lui	$2,%hi(mt7570_safe_circuit_reset)
	addiu	$2,$2,%lo(mt7570_safe_circuit_reset)
	jalr	$2
	nop

	lw	$31,20($sp)
	addiu	$sp,$sp,24
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	mt7570_safe_circuit_reset_for_work_queue
	.size	mt7570_safe_circuit_reset_for_work_queue, .-mt7570_safe_circuit_reset_for_work_queue
	.globl	work_mt7570_safe_circuit_reset
	.data
	.align	2
	.type	work_mt7570_safe_circuit_reset, @object
	.size	work_mt7570_safe_circuit_reset, 16
work_mt7570_safe_circuit_reset:
	.word	1280
	.word	work_mt7570_safe_circuit_reset+4
	.word	work_mt7570_safe_circuit_reset+4
	.word	mt7570_safe_circuit_reset_for_work_queue
	.rdata
	.align	2
$LC31:
	.ascii	"[%lu]0ms[%s:%d]\012  turn off transceiver power success!"
	.ascii	" \012\000"
	.align	2
$LC32:
	.ascii	"[%lu]0ms[%s:%d]\012  turn on transceiver power success! "
	.ascii	"\012\000"
	.align	2
$LC33:
	.ascii	"[%lu]0ms[%s:%d]\012  setting transceiver power fail. \012"
	.ascii	"\000"
	.section	.text.phy_trans_power_switch,"ax",@progbits
	.align	2
	.globl	phy_trans_power_switch
	.set	nomips16
	.set	nomicromips
	.ent	phy_trans_power_switch
	.type	phy_trans_power_switch, @function
phy_trans_power_switch:
	.frame	$sp,32,$31		# vars= 8, regs= 2/0, args= 16, gp= 0
	.mask	0x80010000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-32
	sw	$31,28($sp)
	sw	$16,24($sp)
	move	$2,$4
	sb	$2,32($sp)
	sw	$0,16($sp)
	sw	$0,20($sp)
	lbu	$2,32($sp)
	bne	$2,$0,$L152
	nop

	move	$4,$0
	lui	$2,%hi(phy_tx_ctl)
	addiu	$2,$2,%lo(phy_tx_ctl)
	jalr	$2
	nop

	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,136($2)
	andi	$2,$2,0x10
	beq	$2,$0,$L153
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
	lui	$2,%hi($LC31)
	addiu	$4,$2,%lo($LC31)
	move	$5,$16
	move	$6,$3
	li	$7,809			# 0x329
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L153:
	move	$2,$0
	j	$L154
	nop

$L152:
	lbu	$3,32($sp)
	li	$2,1			# 0x1
	bne	$3,$2,$L155
	nop

	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lw	$3,132($2)
	li	$2,-2147483648			# 0xffffffff80000000
	and	$2,$3,$2
	beq	$2,$0,$L156
	nop

	li	$4,1			# 0x1
	lui	$2,%hi(phy_tx_ctl)
	addiu	$2,$2,%lo(phy_tx_ctl)
	jalr	$2
	nop

	lui	$2,%hi(mt7570_select)
	lw	$3,%lo(mt7570_select)($2)
	li	$2,1			# 0x1
	bne	$3,$2,$L157
	nop

	lui	$2,%hi(i2c_access_queue)
	lw	$2,%lo(i2c_access_queue)($2)
	beq	$2,$0,$L157
	nop

	lui	$2,%hi(i2c_access_queue)
	lw	$2,%lo(i2c_access_queue)($2)
	move	$4,$2
	lui	$2,%hi(work_mt7570_safe_circuit_reset)
	addiu	$5,$2,%lo(work_mt7570_safe_circuit_reset)
	lui	$2,%hi(queue_work)
	addiu	$2,$2,%lo(queue_work)
	jalr	$2
	nop

$L157:
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,136($2)
	andi	$2,$2,0x10
	beq	$2,$0,$L156
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
	lui	$2,%hi($LC32)
	addiu	$4,$2,%lo($LC32)
	move	$5,$16
	move	$6,$3
	li	$7,821			# 0x335
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L156:
	move	$2,$0
	j	$L154
	nop

$L155:
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,136($2)
	andi	$2,$2,0x10
	beq	$2,$0,$L158
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
	lui	$2,%hi($LC33)
	addiu	$4,$2,%lo($LC33)
	move	$5,$16
	move	$6,$3
	li	$7,827			# 0x33b
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L158:
	li	$2,1			# 0x1
$L154:
	lw	$31,28($sp)
	lw	$16,24($sp)
	addiu	$sp,$sp,32
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	phy_trans_power_switch
	.size	phy_trans_power_switch, .-phy_trans_power_switch
	.rdata
	.align	2
	.type	__func__.24679, @object
	.size	__func__.24679, 20
__func__.24679:
	.ascii	"REPORT_EVENT_TO_MAC\000"
	.ident	"GCC: (Buildroot 2015.08.1) 4.9.3"
