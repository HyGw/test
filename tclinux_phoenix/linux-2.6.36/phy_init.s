	.file	1 "phy_init.c"
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
	.globl	phy_date_version
	.data
	.align	2
	.type	phy_date_version, @object
	.size	phy_date_version, 4
phy_date_version:
	.word	160329
	.globl	gpPhyPriv
	.section	.bss,"aw",@nobits
	.align	2
	.type	gpPhyPriv, @object
	.size	gpPhyPriv, 4
gpPhyPriv:
	.space	4
	.globl	i2c_access_queue
	.align	2
	.type	i2c_access_queue, @object
	.size	i2c_access_queue, 4
i2c_access_queue:
	.space	4
	.globl	los_status
	.align	2
	.type	los_status, @object
	.size	los_status, 4
los_status:
	.space	4
	.section	.text.gpio_tx_dis_reset,"ax",@progbits
	.align	2
	.globl	gpio_tx_dis_reset
	.set	nomips16
	.set	nomicromips
	.ent	gpio_tx_dis_reset
	.type	gpio_tx_dis_reset, @function
gpio_tx_dis_reset:
	.frame	$sp,24,$31		# vars= 0, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-24
	sw	$31,20($sp)
	move	$4,$0
	lui	$2,%hi(phy_tx_ctl)
	addiu	$2,$2,%lo(phy_tx_ctl)
	jalr	$2
	nop

	lui	$2,%hi(phy_tx_fault_reset)
	addiu	$2,$2,%lo(phy_tx_fault_reset)
	jalr	$2
	nop

	lw	$31,20($sp)
	addiu	$sp,$sp,24
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	gpio_tx_dis_reset
	.size	gpio_tx_dis_reset, .-gpio_tx_dis_reset
	.section	.text.gpio_BOSA_Tx_power_on,"ax",@progbits
	.align	2
	.globl	gpio_BOSA_Tx_power_on
	.set	nomips16
	.set	nomicromips
	.ent	gpio_BOSA_Tx_power_on
	.type	gpio_BOSA_Tx_power_on, @function
gpio_BOSA_Tx_power_on:
	.frame	$sp,24,$31		# vars= 0, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-24
	sw	$31,20($sp)
	li	$4,102			# 0x66
	lui	$2,%hi(ledTurnOff)
	addiu	$2,$2,%lo(ledTurnOff)
	jalr	$2
	nop

	lw	$31,20($sp)
	addiu	$sp,$sp,24
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	gpio_BOSA_Tx_power_on
	.size	gpio_BOSA_Tx_power_on, .-gpio_BOSA_Tx_power_on
	.section	.text.xPON_Phy_Reset,"ax",@progbits
	.align	2
	.globl	xPON_Phy_Reset
	.set	nomips16
	.set	nomicromips
	.ent	xPON_Phy_Reset
	.type	xPON_Phy_Reset, @function
xPON_Phy_Reset:
	.frame	$sp,40,$31		# vars= 16, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-40
	sw	$31,36($sp)
	sw	$0,16($sp)
	li	$2,-1078984704			# 0xffffffffbfb00000
	ori	$2,$2,0x830
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
	ori	$2,$2,0x1
	sw	$2,16($sp)
	lw	$4,16($sp)
	li	$2,-1078984704			# 0xffffffffbfb00000
	ori	$5,$2,0x830
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	li	$4,1			# 0x1
	lui	$2,%hi(__udelay)
	addiu	$2,$2,%lo(__udelay)
	jalr	$2
	nop

	lw	$3,16($sp)
	li	$2,-2			# 0xfffffffffffffffe
	and	$2,$3,$2
	sw	$2,16($sp)
	lw	$4,16($sp)
	li	$2,-1078984704			# 0xffffffffbfb00000
	ori	$5,$2,0x830
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
	.end	xPON_Phy_Reset
	.size	xPON_Phy_Reset, .-xPON_Phy_Reset
	.section	.text.phy_int_recover_expires,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	phy_int_recover_expires
	.type	phy_int_recover_expires, @function
phy_int_recover_expires:
	.frame	$sp,24,$31		# vars= 0, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-24
	sw	$31,20($sp)
	sw	$4,24($sp)
	li	$4,99			# 0x63
	lui	$2,%hi(phy_int_config)
	addiu	$2,$2,%lo(phy_int_config)
	jalr	$2
	nop

	lw	$31,20($sp)
	addiu	$sp,$sp,24
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	phy_int_recover_expires
	.size	phy_int_recover_expires, .-phy_int_recover_expires
	.rdata
	.align	2
$LC0:
	.ascii	"/opt/tclinux_phoenix/modules/private/xpon_phy/src/phy_in"
	.ascii	"it.c\000"
	.align	2
$LC1:
	.ascii	"[%lu]0ms[%s:%d]\012  mt7570_task_wait\012\000"
	.section	.text.LDDLA_task_wait,"ax",@progbits
	.align	2
	.globl	LDDLA_task_wait
	.set	nomips16
	.set	nomicromips
	.ent	LDDLA_task_wait
	.type	LDDLA_task_wait, @function
LDDLA_task_wait:
	.frame	$sp,24,$31		# vars= 0, regs= 2/0, args= 16, gp= 0
	.mask	0x80010000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-24
	sw	$31,20($sp)
	sw	$16,16($sp)
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,136($2)
	andi	$2,$2,0x8
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
	lui	$2,%hi($LC1)
	addiu	$4,$2,%lo($LC1)
	move	$5,$16
	move	$6,$3
	li	$7,176			# 0xb0
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L7:
	j	$L8
	nop

$L9:
	lui	$2,%hi(mt7570_internal_clock)
	addiu	$2,$2,%lo(mt7570_internal_clock)
	jalr	$2
	nop

	li	$4,1000			# 0x3e8
	lui	$2,%hi(msleep)
	addiu	$2,$2,%lo(msleep)
	jalr	$2
	nop

$L8:
	lui	$2,%hi(kthread_should_stop)
	addiu	$2,$2,%lo(kthread_should_stop)
	jalr	$2
	nop

	beq	$2,$0,$L9
	nop

	lw	$31,20($sp)
	lw	$16,16($sp)
	addiu	$sp,$sp,24
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	LDDLA_task_wait
	.size	LDDLA_task_wait, .-LDDLA_task_wait
	.rdata
	.align	2
$LC2:
	.ascii	"[%lu]0ms[%s:%d]\012  xPON PHY initialization done\012\000"
	.section	.text.phy_dev_init,"ax",@progbits
	.align	2
	.globl	phy_dev_init
	.set	nomips16
	.set	nomicromips
	.ent	phy_dev_init
	.type	phy_dev_init, @function
phy_dev_init:
	.frame	$sp,48,$31		# vars= 24, regs= 2/0, args= 16, gp= 0
	.mask	0x80010000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-48
	sw	$31,44($sp)
	sw	$16,40($sp)
	sw	$0,16($sp)
	sw	$0,20($sp)
	sw	$0,24($sp)
	li	$2,-1079902208			# 0xffffffffbfa20000
	ori	$4,$2,0x104
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	sw	$2,16($sp)
	lw	$2,16($sp)
	ori	$2,$2,0x8001
	sw	$2,16($sp)
	lw	$4,16($sp)
	li	$2,-1079902208			# 0xffffffffbfa20000
	ori	$5,$2,0x104
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$2,$2,0x108
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
	li	$2,-5			# 0xfffffffffffffffb
	and	$2,$3,$2
	sw	$2,16($sp)
	lw	$4,16($sp)
	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$5,$2,0x108
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	li	$2,-1078984704			# 0xffffffffbfb00000
	ori	$4,$2,0x92c
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	sw	$2,16($sp)
	lw	$3,16($sp)
	li	$2,-5			# 0xfffffffffffffffb
	and	$2,$3,$2
	sw	$2,16($sp)
	lw	$4,16($sp)
	li	$2,-1078984704			# 0xffffffffbfb00000
	ori	$5,$2,0x92c
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	li	$2,-1078984704			# 0xffffffffbfb00000
	ori	$4,$2,0x860
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	sw	$2,16($sp)
	lw	$3,16($sp)
	li	$2,-1025			# 0xfffffffffffffbff
	and	$2,$3,$2
	sw	$2,16($sp)
	lw	$4,16($sp)
	li	$2,-1078984704			# 0xffffffffbfb00000
	ori	$5,$2,0x860
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	li	$4,99			# 0x63
	lui	$2,%hi(phy_int_config)
	addiu	$2,$2,%lo(phy_int_config)
	jalr	$2
	nop

	li	$4,1			# 0x1
	li	$5,1			# 0x1
	li	$6,1			# 0x1
	lui	$2,%hi(phy_cnt_enable)
	addiu	$2,$2,%lo(phy_cnt_enable)
	jalr	$2
	nop

	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	li	$3,-1431633920			# 0xffffffffaaab0000
	ori	$3,$3,0x5983
	sw	$3,176($2)
	li	$2,-1431633920			# 0xffffffffaaab0000
	ori	$4,$2,0x5983
	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$5,$2,0x404
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	lui	$2,%hi(phy_reset_counter)
	addiu	$2,$2,%lo(phy_reset_counter)
	jalr	$2
	nop

	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,136($2)
	andi	$2,$2,0x8
	beq	$2,$0,$L12
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
	li	$7,295			# 0x127
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L12:
	move	$2,$0
	lw	$31,44($sp)
	lw	$16,40($sp)
	addiu	$sp,$sp,48
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	phy_dev_init
	.size	phy_dev_init, .-phy_dev_init
	.rdata
	.align	2
$LC3:
	.ascii	"[%lu]0ms[%s:%d]\012  phy_los_handler doing phy_trans_los"
	.ascii	" \012\000"
	.section	.text.phy_los_handler,"ax",@progbits
	.align	2
	.globl	phy_los_handler
	.set	nomips16
	.set	nomicromips
	.ent	phy_los_handler
	.type	phy_los_handler, @function
phy_los_handler:
	.frame	$sp,40,$31		# vars= 16, regs= 2/0, args= 16, gp= 0
	.mask	0x80010000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-40
	sw	$31,36($sp)
	sw	$16,32($sp)
	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$2,$2,0x5f8
	sw	$2,20($sp)
	lw	$2,20($sp)
	lw	$2,0($2)
	sw	$2,24($sp)
	lw	$2,20($sp)
	lw	$2,0($2)
	sw	$2,24($sp)
	lw	$2,24($sp)
	sw	$2,16($sp)
	lw	$4,16($sp)
	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$5,$2,0x5f4
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	lui	$2,%hi(mt7570_select)
	lw	$3,%lo(mt7570_select)($2)
	li	$2,1			# 0x1
	bne	$3,$2,$L16
	nop

	lui	$2,%hi(TEC_switch)
	sw	$0,%lo(TEC_switch)($2)
	lui	$2,%hi(BiasTracking_switch)
	sw	$0,%lo(BiasTracking_switch)($2)
$L16:
	move	$4,$0
	lui	$2,%hi(phy_bit_delay)
	addiu	$2,$2,%lo(phy_bit_delay)
	jalr	$2
	nop

	lui	$2,%hi(phy_reset_counter)
	addiu	$2,$2,%lo(phy_reset_counter)
	jalr	$2
	nop

	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,136($2)
	andi	$2,$2,0x2
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
	lui	$2,%hi($LC3)
	addiu	$4,$2,%lo($LC3)
	move	$5,$16
	move	$6,$3
	li	$7,338			# 0x152
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L14:
	lw	$31,36($sp)
	lw	$16,32($sp)
	addiu	$sp,$sp,40
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	phy_los_handler
	.size	phy_los_handler, .-phy_los_handler
	.section	.text.phy_ready_handler,"ax",@progbits
	.align	2
	.globl	phy_ready_handler
	.set	nomips16
	.set	nomicromips
	.ent	phy_ready_handler
	.type	phy_ready_handler, @function
phy_ready_handler:
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	lui	$2,%hi(mt7570_select)
	lw	$3,%lo(mt7570_select)($2)
	li	$2,1			# 0x1
	bne	$3,$2,$L18
	nop

	lui	$2,%hi(TEC_switch)
	li	$3,1			# 0x1
	sw	$3,%lo(TEC_switch)($2)
	lui	$2,%hi(BiasTracking_switch)
	li	$3,1			# 0x1
	sw	$3,%lo(BiasTracking_switch)($2)
$L18:
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	phy_ready_handler
	.size	phy_ready_handler, .-phy_ready_handler
	.rdata
	.align	2
$LC4:
	.ascii	"[%lu]0ms[%s:%d]\012  PHY IRQ: TRSNS_LOS(%d)|PHY_ILLG(%d)"
	.ascii	" interrupt.\012\000"
	.align	2
$LC5:
	.ascii	"[%lu]0ms[%s:%d]\012  PHY IRQ: transceiver LOF interrupt."
	.ascii	"\012\000"
	.align	2
$LC6:
	.ascii	"[%lu]0ms[%s:%d]\012  turn off power.\012\000"
	.align	2
$LC7:
	.ascii	"[%lu]0ms[%s:%d]\012  PHY IRQ: transceiver TF interrupt.\012"
	.ascii	"\000"
	.align	2
$LC8:
	.ascii	"[%lu]0ms[%s:%d]\012  PHY IRQ: transceiver interrupt.\012"
	.ascii	"\000"
	.align	2
$LC9:
	.ascii	"[%lu]0ms[%s:%d]\012  PHY IRQ: transceiver TX_SD fail int"
	.ascii	"errupt.\012\000"
	.align	2
$LC10:
	.ascii	"[%lu]0ms[%s:%d]\012  PHY IRQ: phy ready interrupt.\012\000"
	.align	2
$LC11:
	.ascii	"[%lu]0ms[%s:%d]\012  PHY IRQ: FALSE phy ready interrupt."
	.ascii	"\012\000"
	.align	2
$LC12:
	.ascii	"[%lu]0ms[%s:%d]\012  PHY IRQ: I2C master interrupt.\012\000"
	.align	2
$LC13:
	.ascii	"[%lu]0ms[%s:%d]\012  PHY detect interrupt error.\012\000"
	.section	.text.phy_isr,"ax",@progbits
	.align	2
	.globl	phy_isr
	.set	nomips16
	.set	nomicromips
	.ent	phy_isr
	.type	phy_isr, @function
phy_isr:
	.frame	$sp,200,$31		# vars= 168, regs= 2/0, args= 24, gp= 0
	.mask	0x80010000,-4
	.fmask	0x00000000,0
	addiu	$sp,$sp,-200
	sw	$31,196($sp)
	sw	$16,192($sp)
	addiu	$2,$sp,104
	sw	$2,24($sp)
	sw	$0,108($sp)
	sw	$0,112($sp)
	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$2,$2,0x5f8
	sw	$2,32($sp)
	lw	$2,32($sp)
	lw	$2,0($2)
	sw	$2,36($sp)
	lw	$2,32($sp)
	lw	$2,0($2)
	sw	$2,36($sp)
	lw	$2,36($sp)
	sw	$2,28($sp)
	lw	$4,28($sp)
	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$5,$2,0x5f4
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	addiu	$2,$2,168
	sw	$2,40($sp)
	lw	$2,40($sp)
	move	$4,$2
	lui	$2,%hi(_raw_spin_lock)
	addiu	$2,$2,%lo(_raw_spin_lock)
	jalr	$2
	lw	$2,28($sp)
	andi	$2,$2,0x1
	bne	$2,$0,$L22
	lw	$2,28($sp)
	andi	$2,$2,0x40
	beq	$2,$0,$L23
$L22:
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,136($2)
	andi	$2,$2,0x2
	beq	$2,$0,$L24
	lui	$2,%hi(jiffies)
	lw	$16,%lo(jiffies)($2)
	lui	$2,%hi($LC0)
	addiu	$4,$2,%lo($LC0)
	li	$5,47			# 0x2f
	lui	$2,%hi(strrchr)
	addiu	$2,$2,%lo(strrchr)
	jalr	$2
	addiu	$3,$2,1
	lw	$2,28($sp)
	andi	$2,$2,0x1
	sltu	$2,$0,$2
	andi	$2,$2,0x00ff
	move	$4,$2
	lw	$2,28($sp)
	andi	$2,$2,0x40
	sltu	$2,$0,$2
	andi	$2,$2,0x00ff
	sw	$4,16($sp)
	sw	$2,20($sp)
	lui	$2,%hi($LC4)
	addiu	$4,$2,%lo($LC4)
	move	$5,$16
	move	$6,$3
	li	$7,395			# 0x18b
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
$L24:
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	sw	$0,172($2)
	lw	$2,28($sp)
	andi	$2,$2,0x1
	beq	$2,$0,$L25
	lw	$2,28($sp)
	andi	$2,$2,0x40
	beq	$2,$0,$L25
	li	$2,8			# 0x8
	sw	$2,44($sp)
	sw	$0,180($sp)
	sw	$0,184($sp)
	lw	$2,44($sp)
	sw	$2,184($sp)
	addiu	$2,$sp,180
	move	$4,$2
	lui	$2,%hi(phy_event_handler)
	addiu	$2,$2,%lo(phy_event_handler)
	jalr	$2
	j	$L23
$L25:
	lw	$2,28($sp)
	andi	$2,$2,0x1
	beq	$2,$0,$L26
	sw	$0,48($sp)
	sw	$0,172($sp)
	sw	$0,176($sp)
	lw	$2,48($sp)
	sw	$2,176($sp)
	addiu	$2,$sp,172
	move	$4,$2
	lui	$2,%hi(phy_event_handler)
	addiu	$2,$2,%lo(phy_event_handler)
	jalr	$2
	j	$L23
$L26:
	lw	$2,28($sp)
	andi	$2,$2,0x40
	beq	$2,$0,$L23
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lw	$3,132($2)
	li	$2,100663296			# 0x6000000
	and	$3,$3,$2
	li	$2,33554432			# 0x2000000
	bne	$3,$2,$L27
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lw	$3,132($2)
	li	$2,16777216			# 0x1000000
	and	$2,$3,$2
	bne	$2,$0,$L23
$L27:
	li	$2,6			# 0x6
	sw	$2,52($sp)
	sw	$0,164($sp)
	sw	$0,168($sp)
	lw	$2,52($sp)
	sw	$2,168($sp)
	addiu	$2,$sp,164
	move	$4,$2
	lui	$2,%hi(phy_event_handler)
	addiu	$2,$2,%lo(phy_event_handler)
	jalr	$2
$L23:
	lw	$2,28($sp)
	andi	$2,$2,0x2
	beq	$2,$0,$L28
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,136($2)
	andi	$2,$2,0x2
	beq	$2,$0,$L29
	lui	$2,%hi(jiffies)
	lw	$16,%lo(jiffies)($2)
	lui	$2,%hi($LC0)
	addiu	$4,$2,%lo($LC0)
	li	$5,47			# 0x2f
	lui	$2,%hi(strrchr)
	addiu	$2,$2,%lo(strrchr)
	jalr	$2
	addiu	$3,$2,1
	lui	$2,%hi($LC5)
	addiu	$4,$2,%lo($LC5)
	move	$5,$16
	move	$6,$3
	li	$7,409			# 0x199
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
$L29:
	li	$2,1			# 0x1
	sw	$2,56($sp)
	sw	$0,156($sp)
	sw	$0,160($sp)
	lw	$2,56($sp)
	sw	$2,160($sp)
	addiu	$2,$sp,156
	move	$4,$2
	lui	$2,%hi(phy_event_handler)
	addiu	$2,$2,%lo(phy_event_handler)
	jalr	$2
$L28:
	lw	$2,28($sp)
	andi	$2,$2,0x4
	beq	$2,$0,$L30
	lw	$4,24($sp)
	lui	$2,%hi(phy_trans_tx_getting)
	addiu	$2,$2,%lo(phy_trans_tx_getting)
	jalr	$2
	lw	$2,24($sp)
	lbu	$2,2($2)
	bne	$2,$0,$L31
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,136($2)
	andi	$2,$2,0x2
	beq	$2,$0,$L32
	lui	$2,%hi(jiffies)
	lw	$16,%lo(jiffies)($2)
	lui	$2,%hi($LC0)
	addiu	$4,$2,%lo($LC0)
	li	$5,47			# 0x2f
	lui	$2,%hi(strrchr)
	addiu	$2,$2,%lo(strrchr)
	jalr	$2
	addiu	$3,$2,1
	lui	$2,%hi($LC6)
	addiu	$4,$2,%lo($LC6)
	move	$5,$16
	move	$6,$3
	li	$7,416			# 0x1a0
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
$L32:
	li	$2,2			# 0x2
	sw	$2,60($sp)
	sw	$0,148($sp)
	sw	$0,152($sp)
	lw	$2,60($sp)
	sw	$2,152($sp)
	addiu	$2,$sp,148
	move	$4,$2
	lui	$2,%hi(phy_event_handler)
	addiu	$2,$2,%lo(phy_event_handler)
	jalr	$2
$L31:
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,136($2)
	andi	$2,$2,0x2
	beq	$2,$0,$L30
	lui	$2,%hi(jiffies)
	lw	$16,%lo(jiffies)($2)
	lui	$2,%hi($LC0)
	addiu	$4,$2,%lo($LC0)
	li	$5,47			# 0x2f
	lui	$2,%hi(strrchr)
	addiu	$2,$2,%lo(strrchr)
	jalr	$2
	addiu	$3,$2,1
	lui	$2,%hi($LC7)
	addiu	$4,$2,%lo($LC7)
	move	$5,$16
	move	$6,$3
	li	$7,419			# 0x1a3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
$L30:
	lw	$2,28($sp)
	andi	$2,$2,0x8
	beq	$2,$0,$L33
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,136($2)
	andi	$2,$2,0x2
	beq	$2,$0,$L34
	lui	$2,%hi(jiffies)
	lw	$16,%lo(jiffies)($2)
	lui	$2,%hi($LC0)
	addiu	$4,$2,%lo($LC0)
	li	$5,47			# 0x2f
	lui	$2,%hi(strrchr)
	addiu	$2,$2,%lo(strrchr)
	jalr	$2
	addiu	$3,$2,1
	lui	$2,%hi($LC8)
	addiu	$4,$2,%lo($LC8)
	move	$5,$16
	move	$6,$3
	li	$7,423			# 0x1a7
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
$L34:
	li	$2,3			# 0x3
	sw	$2,64($sp)
	sw	$0,140($sp)
	sw	$0,144($sp)
	lw	$2,64($sp)
	sw	$2,144($sp)
	addiu	$2,$sp,140
	move	$4,$2
	lui	$2,%hi(phy_event_handler)
	addiu	$2,$2,%lo(phy_event_handler)
	jalr	$2
$L33:
	lw	$2,28($sp)
	andi	$2,$2,0x10
	beq	$2,$0,$L35
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,136($2)
	andi	$2,$2,0x2
	beq	$2,$0,$L36
	lui	$2,%hi(jiffies)
	lw	$16,%lo(jiffies)($2)
	lui	$2,%hi($LC0)
	addiu	$4,$2,%lo($LC0)
	li	$5,47			# 0x2f
	lui	$2,%hi(strrchr)
	addiu	$2,$2,%lo(strrchr)
	jalr	$2
	addiu	$3,$2,1
	lui	$2,%hi($LC9)
	addiu	$4,$2,%lo($LC9)
	move	$5,$16
	move	$6,$3
	li	$7,428			# 0x1ac
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
$L36:
	li	$2,4			# 0x4
	sw	$2,68($sp)
	sw	$0,132($sp)
	sw	$0,136($sp)
	lw	$2,68($sp)
	sw	$2,136($sp)
	addiu	$2,$sp,132
	move	$4,$2
	lui	$2,%hi(phy_event_handler)
	addiu	$2,$2,%lo(phy_event_handler)
	jalr	$2
$L35:
	lw	$2,28($sp)
	andi	$2,$2,0x20
	beq	$2,$0,$L37
	lw	$2,28($sp)
	sw	$2,72($sp)
	lw	$2,72($sp)
	andi	$2,$2,0x1
	beq	$2,$0,$L38
	lw	$2,72($sp)
	andi	$2,$2,0x40
	bne	$2,$0,$L39
$L38:
	li	$2,1			# 0x1
	j	$L41
$L39:
	move	$2,$0
$L41:
	beq	$2,$0,$L42
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	li	$3,1			# 0x1
	sw	$3,172($2)
	li	$2,5			# 0x5
	sw	$2,76($sp)
	sw	$0,124($sp)
	sw	$0,128($sp)
	lw	$2,76($sp)
	sw	$2,128($sp)
	addiu	$2,$sp,124
	move	$4,$2
	lui	$2,%hi(phy_event_handler)
	addiu	$2,$2,%lo(phy_event_handler)
	jalr	$2
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,136($2)
	andi	$2,$2,0x2
	beq	$2,$0,$L37
	lui	$2,%hi(jiffies)
	lw	$16,%lo(jiffies)($2)
	lui	$2,%hi($LC0)
	addiu	$4,$2,%lo($LC0)
	li	$5,47			# 0x2f
	lui	$2,%hi(strrchr)
	addiu	$2,$2,%lo(strrchr)
	jalr	$2
	addiu	$3,$2,1
	lui	$2,%hi($LC10)
	addiu	$4,$2,%lo($LC10)
	move	$5,$16
	move	$6,$3
	li	$7,439			# 0x1b7
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	j	$L37
$L42:
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,136($2)
	andi	$2,$2,0x2
	beq	$2,$0,$L37
	lui	$2,%hi(jiffies)
	lw	$16,%lo(jiffies)($2)
	lui	$2,%hi($LC0)
	addiu	$4,$2,%lo($LC0)
	li	$5,47			# 0x2f
	lui	$2,%hi(strrchr)
	addiu	$2,$2,%lo(strrchr)
	jalr	$2
	addiu	$3,$2,1
	lui	$2,%hi($LC11)
	addiu	$4,$2,%lo($LC11)
	move	$5,$16
	move	$6,$3
	li	$7,441			# 0x1b9
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
$L37:
	lw	$2,28($sp)
	andi	$2,$2,0x80
	beq	$2,$0,$L44
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,136($2)
	andi	$2,$2,0x2
	beq	$2,$0,$L45
	lui	$2,%hi(jiffies)
	lw	$16,%lo(jiffies)($2)
	lui	$2,%hi($LC0)
	addiu	$4,$2,%lo($LC0)
	li	$5,47			# 0x2f
	lui	$2,%hi(strrchr)
	addiu	$2,$2,%lo(strrchr)
	jalr	$2
	addiu	$3,$2,1
	lui	$2,%hi($LC12)
	addiu	$4,$2,%lo($LC12)
	move	$5,$16
	move	$6,$3
	li	$7,446			# 0x1be
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
$L45:
	li	$2,7			# 0x7
	sw	$2,80($sp)
	sw	$0,116($sp)
	sw	$0,120($sp)
	lw	$2,80($sp)
	sw	$2,120($sp)
	addiu	$2,$sp,116
	move	$4,$2
	lui	$2,%hi(phy_event_handler)
	addiu	$2,$2,%lo(phy_event_handler)
	jalr	$2
$L44:
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,126($2)
	bne	$2,$0,$L46
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lui	$3,%hi(jiffies)
	lw	$3,%lo(jiffies)($3)
	sw	$3,128($2)
$L46:
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$3,126($2)
	addiu	$3,$3,1
	andi	$3,$3,0xffff
	sh	$3,126($2)
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$3,126($2)
	li	$2,10			# 0xa
	bne	$3,$2,$L47
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	sh	$0,126($2)
	lui	$2,%hi(jiffies)
	lw	$3,%lo(jiffies)($2)
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lw	$2,128($2)
	subu	$2,$3,$2
	sltu	$2,$2,50
	beq	$2,$0,$L47
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,136($2)
	andi	$2,$2,0x1
	beq	$2,$0,$L47
	lui	$2,%hi(jiffies)
	lw	$16,%lo(jiffies)($2)
	lui	$2,%hi($LC0)
	addiu	$4,$2,%lo($LC0)
	li	$5,47			# 0x2f
	lui	$2,%hi(strrchr)
	addiu	$2,$2,%lo(strrchr)
	jalr	$2
	addiu	$3,$2,1
	lui	$2,%hi($LC13)
	addiu	$4,$2,%lo($LC13)
	move	$5,$16
	move	$6,$3
	li	$7,465			# 0x1d1
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
$L47:
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	addiu	$2,$2,168
	sw	$2,84($sp)
	lw	$2,84($sp)
	sw	$2,88($sp)
	lw	$2,88($sp)
	sw	$2,92($sp)
	lw	$2,92($sp)
	sw	$2,96($sp)
	lw	$2,96($sp)
	lhu	$2,2($2)
	addiu	$2,$2,1
	sw	$2,100($sp)
#APP
 # 139 "/opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/spinlock.h" 1
	.set	push
	.set	noreorder
	.set	mips2
	sync
	.set	pop
 # 0 "" 2
#NO_APP
	lw	$2,100($sp)
	andi	$3,$2,0xffff
	lw	$2,96($sp)
	sh	$3,2($2)
#APP
 # 141 "/opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/spinlock.h" 1
	.set	push
	.set	noreorder
	.set	mips2
	sync
	.set	pop
 # 0 "" 2
#NO_APP
	lw	$31,196($sp)
	lw	$16,192($sp)
	addiu	$sp,$sp,200
	j	$31
	.end	phy_isr
	.size	phy_isr, .-phy_isr
	.section	.text.phy_register_cb_handler,"ax",@progbits
	.align	2
	.globl	phy_register_cb_handler
	.set	nomips16
	.set	nomicromips
	.ent	phy_register_cb_handler
	.type	phy_register_cb_handler, @function
phy_register_cb_handler:
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	sw	$4,0($sp)
	sw	$5,4($sp)
	lw	$2,4($sp)
	bne	$2,$0,$L49
	nop

	li	$2,-1			# 0xffffffffffffffff
	j	$L50
	nop

$L49:
	li	$2,-1			# 0xffffffffffffffff
$L50:
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	phy_register_cb_handler
	.size	phy_register_cb_handler, .-phy_register_cb_handler
	.section	.text.phy_unregister_cb_handler,"ax",@progbits
	.align	2
	.globl	phy_unregister_cb_handler
	.set	nomips16
	.set	nomicromips
	.ent	phy_unregister_cb_handler
	.type	phy_unregister_cb_handler, @function
phy_unregister_cb_handler:
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	sw	$4,0($sp)
	li	$2,-14			# 0xfffffffffffffff2
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	phy_unregister_cb_handler
	.size	phy_unregister_cb_handler, .-phy_unregister_cb_handler
	.rdata
	.align	2
$LC14:
	.ascii	"[%lu]0ms[%s:%d]\012  Register GPON ISR handler function "
	.ascii	"failed.\012\000"
	.section	.text.xpon_phy_start,"ax",@progbits
	.align	2
	.globl	xpon_phy_start
	.set	nomips16
	.set	nomicromips
	.ent	xpon_phy_start
	.type	xpon_phy_start, @function
xpon_phy_start:
	.frame	$sp,72,$31		# vars= 48, regs= 2/0, args= 16, gp= 0
	.mask	0x80010000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-72
	sw	$31,68($sp)
	sw	$16,64($sp)
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	li	$3,1			# 0x1
	sb	$3,138($2)
	li	$4,255			# 0xff
	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$5,$2,0x5f4
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	li	$2,16			# 0x10
	sw	$2,20($sp)
	li	$2,5			# 0x5
	sw	$2,24($sp)
	lui	$2,%hi(phy_isr)
	addiu	$2,$2,%lo(phy_isr)
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
	beq	$3,$2,$L54
	nop

	lw	$2,40($sp)
	j	$L56
	nop

$L54:
	li	$2,-1			# 0xffffffffffffffff
$L56:
	sw	$2,16($sp)
	lw	$2,16($sp)
	beq	$2,$0,$L57
	nop

	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,136($2)
	andi	$2,$2,0x1
	beq	$2,$0,$L58
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
	li	$7,534			# 0x216
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L58:
	lw	$2,16($sp)
	j	$L59
	nop

$L57:
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

	move	$2,$0
$L59:
	lw	$31,68($sp)
	lw	$16,64($sp)
	addiu	$sp,$sp,72
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	xpon_phy_start
	.size	xpon_phy_start, .-xpon_phy_start
	.section	.text.xpon_phy_stop,"ax",@progbits
	.align	2
	.globl	xpon_phy_stop
	.set	nomips16
	.set	nomicromips
	.ent	xpon_phy_stop
	.type	xpon_phy_stop, @function
xpon_phy_stop:
	.frame	$sp,72,$31		# vars= 48, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-72
	sw	$31,68($sp)
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	sb	$0,138($2)
	li	$2,16			# 0x10
	sw	$2,16($sp)
	li	$2,5			# 0x5
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
	move	$4,$0
	lui	$2,%hi(phy_fw_ready)
	addiu	$2,$2,%lo(phy_fw_ready)
	jalr	$2
	nop

	move	$2,$28
	lw	$2,20($2)
	move	$3,$2
	li	$2,134152192			# 0x7ff0000
	ori	$2,$2,0xff00
	and	$2,$3,$2
	beq	$2,$0,$L64
	nop

	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	addiu	$2,$2,140
	move	$4,$2
	lui	$2,%hi(del_timer)
	addiu	$2,$2,%lo(del_timer)
	jalr	$2
	nop

	j	$L65
	nop

$L64:
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	addiu	$2,$2,140
	move	$4,$2
	lui	$2,%hi(del_timer_sync)
	addiu	$2,$2,%lo(del_timer_sync)
	jalr	$2
	nop

$L65:
	move	$2,$0
	lw	$31,68($sp)
	addiu	$sp,$sp,72
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	xpon_phy_stop
	.size	xpon_phy_stop, .-xpon_phy_stop
	.globl	gphy_rx_test_timer
	.section	.bss
	.align	2
	.type	gphy_rx_test_timer, @object
	.size	gphy_rx_test_timer, 28
gphy_rx_test_timer:
	.space	28
	.rdata
	.align	2
$LC15:
	.ascii	"Rx_Sensitivity_Test PRBS error counter: 0x%x\012\000"
	.globl	__floatunsidf
	.globl	__divdf3
	.globl	__muldf3
	.globl	__fixunsdfsi
	.align	2
$LC18:
	.ascii	"Rx_Sensitivity_Test BER: %u\012\000"
	.section	.text.phy_rx_sens_timer,"ax",@progbits
	.align	2
	.globl	phy_rx_sens_timer
	.set	nomips16
	.set	nomicromips
	.ent	phy_rx_sens_timer
	.type	phy_rx_sens_timer, @function
phy_rx_sens_timer:
	.frame	$sp,48,$31		# vars= 24, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-48
	sw	$31,44($sp)
	sw	$4,48($sp)
	sw	$0,16($sp)
	sw	$0,28($sp)
	sw	$0,24($sp)
	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$2,$2,0x2c0
	sw	$2,32($sp)
	lw	$2,32($sp)
	lw	$2,0($2)
	sw	$2,36($sp)
	lw	$2,32($sp)
	lw	$2,0($2)
	sw	$2,36($sp)
	lw	$2,36($sp)
	sw	$2,16($sp)
	move	$4,$0
	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$5,$2,0x4a0
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	move	$4,$0
	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$5,$2,0x4a4
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	lui	$2,%hi($LC15)
	addiu	$4,$2,%lo($LC15)
	lw	$5,16($sp)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi(__floatunsidf)
	addiu	$2,$2,%lo(__floatunsidf)
	lw	$4,16($sp)
	jalr	$2
	nop

	move	$5,$3
	move	$4,$2
	lui	$3,%hi($LC16)
	lui	$2,%hi(__divdf3)
	addiu	$2,$2,%lo(__divdf3)
	lw	$7,%lo($LC16+4)($3)
	lw	$6,%lo($LC16)($3)
	jalr	$2
	nop

	sw	$3,28($sp)
	sw	$2,24($sp)
	lui	$3,%hi($LC17)
	lui	$2,%hi(__muldf3)
	addiu	$2,$2,%lo(__muldf3)
	lw	$5,28($sp)
	lw	$4,24($sp)
	lw	$7,%lo($LC17+4)($3)
	lw	$6,%lo($LC17)($3)
	jalr	$2
	nop

	move	$5,$3
	move	$4,$2
	lui	$2,%hi(__fixunsdfsi)
	addiu	$2,$2,%lo(__fixunsdfsi)
	jalr	$2
	nop

	sw	$2,16($sp)
	lui	$2,%hi(gphy_test_rx_ber)
	lw	$3,16($sp)
	sw	$3,%lo(gphy_test_rx_ber)($2)
	lui	$2,%hi($LC18)
	addiu	$4,$2,%lo($LC18)
	lw	$5,16($sp)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lw	$31,44($sp)
	addiu	$sp,$sp,48
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	phy_rx_sens_timer
	.size	phy_rx_sens_timer, .-phy_rx_sens_timer
	.rdata
	.align	2
$LC19:
	.ascii	"Rx_Sensitivity_Test start: 0x%x\012\000"
	.section	.text.phy_rx_sensitivity_test,"ax",@progbits
	.align	2
	.globl	phy_rx_sensitivity_test
	.set	nomips16
	.set	nomicromips
	.ent	phy_rx_sensitivity_test
	.type	phy_rx_sensitivity_test, @function
phy_rx_sensitivity_test:
	.frame	$sp,64,$31		# vars= 40, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-64
	sw	$31,60($sp)
	sw	$0,16($sp)
	lui	$2,%hi(gphy_test_rx_ber)
	sw	$0,%lo(gphy_test_rx_ber)($2)
	lui	$2,%hi(jiffies)
	lw	$2,%lo(jiffies)($2)
	addiu	$3,$2,1000
	lui	$2,%hi(gphy_rx_test_timer)
	addiu	$2,$2,%lo(gphy_rx_test_timer)
	sw	$3,8($2)
	move	$4,$0
	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$5,$2,0x4a0
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	move	$4,$0
	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$5,$2,0x4a4
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	li	$4,-14680064			# 0xffffffffff200000
	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$5,$2,0x124
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	li	$2,8454144			# 0x810000
	ori	$4,$2,0xfd53
	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$5,$2,0x2a0
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	li	$2,16711680			# 0xff0000
	ori	$4,$2,0xffff
	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$5,$2,0x2a4
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	li	$4,5			# 0x5
	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$5,$2,0x4a0
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	lui	$2,%hi(gphy_rx_test_timer)
	addiu	$4,$2,%lo(gphy_rx_test_timer)
	lui	$2,%hi(add_timer)
	addiu	$2,$2,%lo(add_timer)
	jalr	$2
	nop

	li	$4,1			# 0x1
	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$5,$2,0x4a4
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$2,$2,0x2c4
	sw	$2,20($sp)
	lw	$2,20($sp)
	lw	$2,0($2)
	sw	$2,24($sp)
	lw	$2,20($sp)
	lw	$2,0($2)
	sw	$2,24($sp)
	lw	$2,24($sp)
	sw	$2,16($sp)
	lui	$2,%hi($LC19)
	addiu	$4,$2,%lo($LC19)
	lw	$5,16($sp)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lw	$31,60($sp)
	addiu	$sp,$sp,64
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	phy_rx_sensitivity_test
	.size	phy_rx_sensitivity_test, .-phy_rx_sensitivity_test
	.rdata
	.align	2
$LC20:
	.ascii	"unknown command id!\012\000"
	.section	.text.pon_phy_get_api_dispatch,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	pon_phy_get_api_dispatch
	.type	pon_phy_get_api_dispatch, @function
pon_phy_get_api_dispatch:
	.frame	$sp,32,$31		# vars= 0, regs= 3/0, args= 16, gp= 0
	.mask	0x80030000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-32
	sw	$31,28($sp)
	sw	$17,24($sp)
	sw	$16,20($sp)
	sw	$4,32($sp)
	lw	$2,32($sp)
	sw	$0,8($2)
	lw	$2,32($sp)
	lw	$3,4($2)
	li	$2,-65536			# 0xffffffffffff0000
	ori	$2,$2,0x7fff
	addu	$2,$3,$2
	sltu	$3,$2,30
	beq	$3,$0,$L72
	nop

	sll	$3,$2,2
	lui	$2,%hi($L74)
	addiu	$2,$2,%lo($L74)
	addu	$2,$3,$2
	lw	$2,0($2)
	j	$2
	nop

	.rdata
	.align	2
	.align	2
$L74:
	.word	$L73
	.word	$L75
	.word	$L72
	.word	$L72
	.word	$L72
	.word	$L72
	.word	$L72
	.word	$L76
	.word	$L77
	.word	$L78
	.word	$L72
	.word	$L72
	.word	$L79
	.word	$L72
	.word	$L72
	.word	$L72
	.word	$L72
	.word	$L80
	.word	$L81
	.word	$L72
	.word	$L72
	.word	$L82
	.word	$L83
	.word	$L84
	.word	$L85
	.word	$L86
	.word	$L87
	.word	$L88
	.word	$L89
	.word	$L90
	.section	.text.pon_phy_get_api_dispatch
$L73:
	lui	$2,%hi(phy_los_status)
	addiu	$2,$2,%lo(phy_los_status)
	jalr	$2
	nop

	move	$3,$2
	lw	$2,32($sp)
	sw	$3,8($2)
	j	$L71
	nop

$L75:
	lui	$2,%hi(phy_ready_status)
	addiu	$2,$2,%lo(phy_ready_status)
	jalr	$2
	nop

	move	$3,$2
	lw	$2,32($sp)
	sw	$3,8($2)
	j	$L71
	nop

$L85:
	lui	$2,%hi(phy_ready_status)
	addiu	$2,$2,%lo(phy_ready_status)
	jalr	$2
	nop

	move	$3,$2
	li	$2,1			# 0x1
	bne	$3,$2,$L92
	nop

	lui	$2,%hi(phy_los_status)
	addiu	$2,$2,%lo(phy_los_status)
	jalr	$2
	nop

	bne	$2,$0,$L92
	nop

	li	$2,1			# 0x1
	j	$L94
	nop

$L92:
	move	$2,$0
$L94:
	lw	$3,32($sp)
	sw	$2,8($3)
	j	$L71
	nop

$L86:
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lw	$2,132($2)
	ext	$2,$2,25,2
	andi	$2,$2,0x00ff
	move	$3,$2
	lw	$2,32($sp)
	sw	$3,8($2)
	j	$L71
	nop

$L87:
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lw	$2,132($2)
	ext	$2,$2,30,1
	andi	$2,$2,0x00ff
	move	$3,$2
	lw	$2,32($sp)
	sw	$3,8($2)
	j	$L71
	nop

$L88:
	lui	$2,%hi(mt7570_select)
	lw	$3,%lo(mt7570_select)($2)
	li	$2,1			# 0x1
	bne	$3,$2,$L97
	nop

	lw	$2,32($sp)
	lw	$2,12($2)
	move	$4,$2
	lui	$2,%hi(mt7570_param_status_real)
	addiu	$2,$2,%lo(mt7570_param_status_real)
	jalr	$2
	nop

	lw	$2,32($sp)
	lw	$2,12($2)
	lui	$3,%hi(mt7570_alarm)
	lw	$3,%lo(mt7570_alarm)($3)
	sw	$3,12($2)
	j	$L71
	nop

$L97:
	lw	$2,32($sp)
	lw	$2,12($2)
	move	$4,$2
	lui	$2,%hi(phy_trans_param_status_real)
	addiu	$2,$2,%lo(phy_trans_param_status_real)
	jalr	$2
	nop

	lw	$2,32($sp)
	lw	$16,12($2)
	lui	$2,%hi(phy_tx_alarm)
	addiu	$2,$2,%lo(phy_tx_alarm)
	jalr	$2
	nop

	move	$17,$2
	lui	$2,%hi(phy_rx_power_alarm)
	addiu	$2,$2,%lo(phy_rx_power_alarm)
	jalr	$2
	nop

	or	$2,$17,$2
	sw	$2,12($16)
	j	$L71
	nop

$L89:
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lw	$2,132($2)
	ext	$2,$2,31,1
	andi	$2,$2,0x00ff
	move	$3,$2
	lw	$2,32($sp)
	sw	$3,8($2)
	j	$L71
	nop

$L80:
	lui	$2,%hi(phy_bip_counter)
	addiu	$2,$2,%lo(phy_bip_counter)
	jalr	$2
	nop

	move	$3,$2
	lw	$2,32($sp)
	sw	$3,8($2)
	j	$L71
	nop

$L79:
	lw	$2,32($sp)
	lw	$2,12($2)
	move	$4,$2
	lui	$2,%hi(phy_rx_fec_counter)
	addiu	$2,$2,%lo(phy_rx_fec_counter)
	jalr	$2
	nop

	j	$L71
	nop

$L81:
	lw	$2,32($sp)
	lw	$2,12($2)
	move	$4,$2
	lui	$2,%hi(phy_rx_frame_counter)
	addiu	$2,$2,%lo(phy_rx_frame_counter)
	jalr	$2
	nop

	j	$L71
	nop

$L83:
	lui	$2,%hi(phy_rx_fec_getting)
	addiu	$2,$2,%lo(phy_rx_fec_getting)
	jalr	$2
	nop

	move	$3,$2
	lw	$2,32($sp)
	sw	$3,8($2)
	j	$L71
	nop

$L82:
	lui	$2,%hi(phy_rx_fec_status)
	addiu	$2,$2,%lo(phy_rx_fec_status)
	jalr	$2
	nop

	move	$3,$2
	lw	$2,32($sp)
	sw	$3,8($2)
	j	$L71
	nop

$L76:
	lui	$2,%hi(phy_tx_fec_status)
	addiu	$2,$2,%lo(phy_tx_fec_status)
	jalr	$2
	nop

	move	$3,$2
	lw	$2,32($sp)
	sw	$3,8($2)
	j	$L71
	nop

$L77:
	lui	$2,%hi(phy_tx_burst_getting)
	addiu	$2,$2,%lo(phy_tx_burst_getting)
	jalr	$2
	nop

	move	$3,$2
	lw	$2,32($sp)
	sw	$3,8($2)
	j	$L71
	nop

$L78:
	lw	$2,32($sp)
	lw	$2,12($2)
	move	$4,$2
	lui	$2,%hi(phy_trans_tx_getting)
	addiu	$2,$2,%lo(phy_trans_tx_getting)
	jalr	$2
	nop

	j	$L71
	nop

$L84:
	lui	$2,%hi(phy_trans_rx_getting)
	addiu	$2,$2,%lo(phy_trans_rx_getting)
	jalr	$2
	nop

	move	$3,$2
	lw	$2,32($sp)
	sw	$3,8($2)
	j	$L71
	nop

$L90:
	lui	$2,%hi(phy_round_trip_delay_sof)
	addiu	$2,$2,%lo(phy_round_trip_delay_sof)
	jalr	$2
	nop

	move	$3,$2
	lw	$2,32($sp)
	sw	$3,8($2)
	j	$L71
	nop

$L72:
	lui	$2,%hi(dump_stack)
	addiu	$2,$2,%lo(dump_stack)
	jalr	$2
	nop

	lui	$2,%hi($LC20)
	addiu	$4,$2,%lo($LC20)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lw	$2,32($sp)
	li	$3,-1			# 0xffffffffffffffff
	sw	$3,8($2)
	nop
$L71:
	lw	$31,28($sp)
	lw	$17,24($sp)
	lw	$16,20($sp)
	addiu	$sp,$sp,32
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	pon_phy_get_api_dispatch
	.size	pon_phy_get_api_dispatch, .-pon_phy_get_api_dispatch
	.section	.text.pon_phy_set_api_dispatch,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	pon_phy_set_api_dispatch
	.type	pon_phy_set_api_dispatch, @function
pon_phy_set_api_dispatch:
	.frame	$sp,24,$31		# vars= 0, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-24
	sw	$31,20($sp)
	sw	$4,24($sp)
	lw	$2,24($sp)
	sw	$0,8($2)
	lw	$2,24($sp)
	lw	$2,4($2)
	sltu	$3,$2,26
	beq	$3,$0,$L100
	nop

	sll	$3,$2,2
	lui	$2,%hi($L102)
	addiu	$2,$2,%lo($L102)
	addu	$2,$3,$2
	lw	$2,0($2)
	j	$2
	nop

	.rdata
	.align	2
	.align	2
$L102:
	.word	$L100
	.word	$L101
	.word	$L103
	.word	$L100
	.word	$L104
	.word	$L105
	.word	$L106
	.word	$L107
	.word	$L108
	.word	$L109
	.word	$L110
	.word	$L111
	.word	$L112
	.word	$L113
	.word	$L114
	.word	$L100
	.word	$L115
	.word	$L116
	.word	$L117
	.word	$L118
	.word	$L119
	.word	$L120
	.word	$L121
	.word	$L122
	.word	$L100
	.word	$L123
	.section	.text.pon_phy_set_api_dispatch
$L107:
	lw	$2,24($sp)
	lw	$2,12($2)
	lw	$3,0($2)
	lw	$2,24($sp)
	lw	$2,12($2)
	lw	$2,4($2)
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(phy_mode_config)
	addiu	$2,$2,%lo(phy_mode_config)
	jalr	$2
	nop

	move	$3,$2
	lw	$2,24($sp)
	sw	$3,8($2)
	j	$L99
	nop

$L121:
	lui	$2,%hi(phy_reset_counter)
	addiu	$2,$2,%lo(phy_reset_counter)
	jalr	$2
	nop

	move	$3,$2
	lw	$2,24($sp)
	sw	$3,8($2)
	j	$L99
	nop

$L101:
	lui	$2,%hi(xpon_phy_start)
	addiu	$2,$2,%lo(xpon_phy_start)
	jalr	$2
	nop

	j	$L99
	nop

$L103:
	lui	$2,%hi(xpon_phy_stop)
	addiu	$2,$2,%lo(xpon_phy_stop)
	jalr	$2
	nop

	j	$L99
	nop

$L105:
	lw	$2,24($sp)
	lw	$2,12($2)
	lw	$2,0($2)
	andi	$2,$2,0x00ff
	move	$4,$2
	lui	$2,%hi(phy_fw_ready)
	addiu	$2,$2,%lo(phy_fw_ready)
	jalr	$2
	nop

	move	$3,$2
	lw	$2,24($sp)
	sw	$3,8($2)
	j	$L99
	nop

$L109:
	lw	$2,24($sp)
	lw	$2,12($2)
	lw	$2,0($2)
	andi	$2,$2,0x00ff
	move	$4,$2
	lui	$2,%hi(phy_counter_clear)
	addiu	$2,$2,%lo(phy_counter_clear)
	jalr	$2
	nop

	j	$L99
	nop

$L106:
	lui	$2,%hi(eponMode_phy_reset)
	addiu	$2,$2,%lo(eponMode_phy_reset)
	jalr	$2
	nop

	move	$3,$2
	lw	$2,24($sp)
	sw	$3,8($2)
	j	$L99
	nop

$L110:
	lw	$2,24($sp)
	lw	$2,12($2)
	lw	$2,0($2)
	andi	$2,$2,0x00ff
	move	$4,$2
	lui	$2,%hi(phy_bit_delay)
	addiu	$2,$2,%lo(phy_bit_delay)
	jalr	$2
	nop

	move	$3,$2
	lw	$2,24($sp)
	sw	$3,8($2)
	j	$L99
	nop

$L111:
	lw	$2,24($sp)
	lw	$2,12($2)
	lw	$2,0($2)
	andi	$2,$2,0x00ff
	move	$4,$2
	lui	$2,%hi(phy_tx_power_config)
	addiu	$2,$2,%lo(phy_tx_power_config)
	jalr	$2
	nop

	j	$L99
	nop

$L118:
	lw	$2,24($sp)
	lw	$2,12($2)
	lw	$2,0($2)
	andi	$2,$2,0x00ff
	move	$4,$2
	lui	$2,%hi(phy_rogue_prbs_config)
	addiu	$2,$2,%lo(phy_rogue_prbs_config)
	jalr	$2
	nop

	move	$3,$2
	lw	$2,24($sp)
	sw	$3,8($2)
	j	$L99
	nop

$L104:
	lui	$2,%hi(phy_dev_init)
	addiu	$2,$2,%lo(phy_dev_init)
	jalr	$2
	nop

	move	$3,$2
	lw	$2,24($sp)
	sw	$3,8($2)
	j	$L99
	nop

$L112:
	lw	$2,24($sp)
	lw	$2,12($2)
	lw	$2,0($2)
	andi	$2,$2,0x00ff
	move	$4,$2
	lui	$2,%hi(phy_trans_power_switch)
	addiu	$2,$2,%lo(phy_trans_power_switch)
	jalr	$2
	nop

	move	$3,$2
	lw	$2,24($sp)
	sw	$3,8($2)
	j	$L99
	nop

$L123:
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lw	$2,180($2)
	move	$4,$2
	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$5,$2,0x400
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lw	$2,176($2)
	move	$4,$2
	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$5,$2,0x404
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lw	$2,184($2)
	move	$4,$2
	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$5,$2,0x408
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	j	$L99
	nop

$L108:
	lui	$2,%hi(phy_trans_model_setting)
	addiu	$2,$2,%lo(phy_trans_model_setting)
	jalr	$2
	nop

	j	$L99
	nop

$L119:
	lw	$2,24($sp)
	lw	$2,12($2)
	lw	$2,0($2)
	andi	$2,$2,0x00ff
	move	$4,$2
	lui	$2,%hi(phy_rx_fec_setting)
	addiu	$2,$2,%lo(phy_rx_fec_setting)
	jalr	$2
	nop

	move	$3,$2
	lw	$2,24($sp)
	sw	$3,8($2)
	j	$L99
	nop

$L113:
	lw	$2,24($sp)
	lw	$2,12($2)
	lw	$2,0($2)
	andi	$2,$2,0x00ff
	move	$4,$2
	lui	$2,%hi(phy_tx_burst_config)
	addiu	$2,$2,%lo(phy_tx_burst_config)
	jalr	$2
	nop

	move	$3,$2
	lw	$2,24($sp)
	sw	$3,8($2)
	j	$L99
	nop

$L114:
	lw	$2,24($sp)
	lw	$2,12($2)
	lw	$3,0($2)
	lw	$2,24($sp)
	lw	$2,12($2)
	lbu	$2,4($2)
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(phy_gpon_delimiter_guard)
	addiu	$2,$2,%lo(phy_gpon_delimiter_guard)
	jalr	$2
	nop

	move	$3,$2
	lw	$2,24($sp)
	sw	$3,8($2)
	j	$L99
	nop

$L115:
	lw	$2,24($sp)
	lw	$2,12($2)
	move	$4,$2
	lui	$2,%hi(phy_trans_tx_setting)
	addiu	$2,$2,%lo(phy_trans_tx_setting)
	jalr	$2
	nop

	move	$3,$2
	lw	$2,24($sp)
	sw	$3,8($2)
	j	$L99
	nop

$L120:
	lw	$2,24($sp)
	lw	$2,12($2)
	lw	$2,0($2)
	andi	$2,$2,0x00ff
	move	$4,$2
	lui	$2,%hi(phy_trans_rx_setting)
	addiu	$2,$2,%lo(phy_trans_rx_setting)
	jalr	$2
	nop

	move	$3,$2
	lw	$2,24($sp)
	sw	$3,8($2)
	j	$L99
	nop

$L116:
	lw	$2,24($sp)
	lw	$2,12($2)
	move	$4,$2
	lui	$2,%hi(phy_gpon_preamble)
	addiu	$2,$2,%lo(phy_gpon_preamble)
	jalr	$2
	nop

	move	$3,$2
	lw	$2,24($sp)
	sw	$3,8($2)
	j	$L99
	nop

$L117:
	lw	$2,24($sp)
	lw	$2,12($2)
	move	$4,$2
	lui	$2,%hi(phy_gpon_extend_preamble)
	addiu	$2,$2,%lo(phy_gpon_extend_preamble)
	jalr	$2
	nop

	move	$3,$2
	lw	$2,24($sp)
	sw	$3,8($2)
	j	$L99
	nop

$L122:
	lw	$2,24($sp)
	lw	$2,12($2)
	lw	$2,0($2)
	move	$4,$2
	lui	$2,%hi(phy_set_epon_ts_continue_mode)
	addiu	$2,$2,%lo(phy_set_epon_ts_continue_mode)
	jalr	$2
	nop

	move	$3,$2
	lw	$2,24($sp)
	sw	$3,8($2)
	j	$L99
	nop

$L100:
	lui	$2,%hi($LC20)
	addiu	$4,$2,%lo($LC20)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lw	$2,24($sp)
	li	$3,-1			# 0xffffffffffffffff
	sw	$3,8($2)
	nop
$L99:
	lw	$31,20($sp)
	addiu	$sp,$sp,24
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	pon_phy_set_api_dispatch
	.size	pon_phy_set_api_dispatch, .-pon_phy_set_api_dispatch
	.rdata
	.align	2
$LC21:
	.ascii	"unknown api_data->api_type: %d\012\000"
	.section	.text.pon_phy_api_dispatch,"ax",@progbits
	.align	2
	.globl	pon_phy_api_dispatch
	.set	nomips16
	.set	nomicromips
	.ent	pon_phy_api_dispatch
	.type	pon_phy_api_dispatch, @function
pon_phy_api_dispatch:
	.frame	$sp,32,$31		# vars= 8, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-32
	sw	$31,28($sp)
	sw	$4,32($sp)
	lw	$2,32($sp)
	sw	$2,16($sp)
	lw	$2,16($sp)
	lw	$2,0($2)
	beq	$2,$0,$L127
	nop

	li	$3,1			# 0x1
	beq	$2,$3,$L128
	nop

	j	$L131
	nop

$L127:
	lw	$4,16($sp)
	lui	$2,%hi(pon_phy_get_api_dispatch)
	addiu	$2,$2,%lo(pon_phy_get_api_dispatch)
	jalr	$2
	nop

	j	$L129
	nop

$L128:
	lw	$4,16($sp)
	lui	$2,%hi(pon_phy_set_api_dispatch)
	addiu	$2,$2,%lo(pon_phy_set_api_dispatch)
	jalr	$2
	nop

	j	$L129
	nop

$L131:
	lui	$2,%hi(dump_stack)
	addiu	$2,$2,%lo(dump_stack)
	jalr	$2
	nop

	lw	$2,16($sp)
	lw	$3,0($2)
	lui	$2,%hi($LC21)
	addiu	$4,$2,%lo($LC21)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lw	$2,16($sp)
	li	$3,-1			# 0xffffffffffffffff
	sw	$3,8($2)
	nop
$L129:
	li	$2,2			# 0x2
	lw	$31,28($sp)
	addiu	$sp,$sp,32
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	pon_phy_api_dispatch
	.size	pon_phy_api_dispatch, .-pon_phy_api_dispatch
	.globl	phy_api_dispatch_hook_ops
	.rdata
	.align	2
$LC22:
	.ascii	"pon_phy_api_dispatch\000"
	.data
	.align	2
	.type	phy_api_dispatch_hook_ops, @object
	.size	phy_api_dispatch_hook_ops, 40
phy_api_dispatch_hook_ops:
	.space	16
	.word	$LC22
	.word	1
	.word	pon_phy_api_dispatch
	.word	25
	.word	0
	.word	1
	.rdata
	.align	2
$LC23:
	.ascii	"Alloc data struct memory failed\012\000"
	.align	2
$LC24:
	.ascii	"[%lu]0ms[%s:%d]\012  phy debug initialization failed.\012"
	.ascii	"\000"
	.align	2
$LC25:
	.ascii	"[%lu]0ms[%s:%d]\012  phy los status initialization faile"
	.ascii	"d.\012\000"
	.align	2
$LC26:
	.ascii	"[%lu]0ms[%s:%d]\012  phy device initialization failed.\012"
	.ascii	"\000"
	.align	2
$LC27:
	.ascii	"EN7570 found!\012\000"
	.align	2
$LC28:
	.ascii	"[%lu]0ms[%s:%d]\012  7570 initialization failed.\012\000"
	.align	2
$LC29:
	.ascii	"mt7570_task_wait\000"
	.align	2
$LC30:
	.ascii	"EN7570 not found!\012\000"
	.align	2
$LC31:
	.ascii	"PON PHY driver version is %d.%d.%d\012\000"
	.align	2
$LC32:
	.ascii	"Register hook function failed! %s:%d\000"
	.section	.text.pon_phy_init,"ax",@progbits
	.align	2
	.globl	pon_phy_init
	.set	nomips16
	.set	nomicromips
	.ent	pon_phy_init
	.type	pon_phy_init, @function
pon_phy_init:
	.frame	$sp,64,$31		# vars= 40, regs= 2/0, args= 16, gp= 0
	.mask	0x80010000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-64
	sw	$31,60($sp)
	sw	$16,56($sp)
	sw	$0,16($sp)
	sw	$0,20($sp)
	sw	$0,24($sp)
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	bne	$2,$0,$L133
	nop

	li	$2,220			# 0xdc
	sw	$2,32($sp)
	li	$2,208			# 0xd0
	sw	$2,36($sp)
$L134 = .
	lw	$4,32($sp)
	lw	$5,36($sp)
	lui	$2,%hi(__kmalloc)
	addiu	$2,$2,%lo(__kmalloc)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(gpPhyPriv)
	sw	$3,%lo(gpPhyPriv)($2)
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	bne	$2,$0,$L133
	nop

	lui	$2,%hi($LC23)
	addiu	$4,$2,%lo($LC23)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lw	$2,16($sp)
	j	$L151
	nop

$L133:
	lui	$2,%hi(xPON_Phy_Reset)
	addiu	$2,$2,%lo(xPON_Phy_Reset)
	jalr	$2
	nop

	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	li	$3,96			# 0x60
	sh	$3,116($2)
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	sb	$0,138($2)
	lui	$2,%hi(gpPhyPriv)
	lw	$3,%lo(gpPhyPriv)($2)
	lbu	$2,132($3)
	ins	$2,$0,0,1
	sb	$2,132($3)
	lui	$2,%hi(gpPhyPriv)
	lw	$3,%lo(gpPhyPriv)($2)
	lbu	$2,132($3)
	li	$4,2			# 0x2
	ins	$2,$4,1,2
	sb	$2,132($3)
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	addiu	$2,$2,168
	sw	$2,40($sp)
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	sw	$0,168($2)
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	li	$3,2			# 0x2
	sw	$3,172($2)
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	addiu	$2,$2,140
	move	$4,$2
	move	$5,$0
	move	$6,$0
	lui	$2,%hi(init_timer_key)
	addiu	$2,$2,%lo(init_timer_key)
	jalr	$2
	nop

	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	li	$3,1000			# 0x3e8
	sw	$3,160($2)
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lui	$3,%hi(phy_event_poll)
	addiu	$3,$3,%lo(phy_event_poll)
	sw	$3,156($2)
	lui	$2,%hi(phy_debug_init)
	addiu	$2,$2,%lo(phy_debug_init)
	jalr	$2
	nop

	sw	$2,16($sp)
	lw	$2,16($sp)
	beq	$2,$0,$L138
	nop

	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,136($2)
	andi	$2,$2,0x1
	beq	$2,$0,$L139
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
	lui	$2,%hi($LC24)
	addiu	$4,$2,%lo($LC24)
	move	$5,$16
	move	$6,$3
	li	$7,910			# 0x38e
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L139:
	lw	$2,16($sp)
	j	$L151
	nop

$L138:
	lui	$2,%hi(los_status_init)
	addiu	$2,$2,%lo(los_status_init)
	jalr	$2
	nop

	sw	$2,16($sp)
	lw	$2,16($sp)
	beq	$2,$0,$L140
	nop

	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,136($2)
	andi	$2,$2,0x1
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
	lui	$2,%hi($LC25)
	addiu	$4,$2,%lo($LC25)
	move	$5,$16
	move	$6,$3
	li	$7,916			# 0x394
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L141:
	lw	$2,16($sp)
	j	$L151
	nop

$L140:
	lui	$2,%hi(phy_dev_init)
	addiu	$2,$2,%lo(phy_dev_init)
	jalr	$2
	nop

	sw	$2,16($sp)
	lw	$2,16($sp)
	beq	$2,$0,$L142
	nop

	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,136($2)
	andi	$2,$2,0x1
	beq	$2,$0,$L143
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
	li	$7,922			# 0x39a
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L143:
	lw	$2,16($sp)
	j	$L151
	nop

$L142:
	lui	$2,%hi(mt7570_EN7570_detection)
	addiu	$2,$2,%lo(mt7570_EN7570_detection)
	jalr	$2
	nop

	move	$3,$2
	li	$2,1			# 0x1
	bne	$3,$2,$L144
	nop

	lui	$2,%hi($LC27)
	addiu	$4,$2,%lo($LC27)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi(mt7570_init)
	addiu	$2,$2,%lo(mt7570_init)
	jalr	$2
	nop

	sw	$2,16($sp)
	lw	$2,16($sp)
	beq	$2,$0,$L145
	nop

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
	lui	$2,%hi($LC28)
	addiu	$4,$2,%lo($LC28)
	move	$5,$16
	move	$6,$3
	li	$7,931			# 0x3a3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L146:
	lw	$2,16($sp)
	j	$L151
	nop

$L145:
	lui	$2,%hi(gpio_tx_dis_reset)
	addiu	$2,$2,%lo(gpio_tx_dis_reset)
	jalr	$2
	nop

	lui	$2,%hi(gpio_BOSA_Tx_power_on)
	addiu	$2,$2,%lo(gpio_BOSA_Tx_power_on)
	jalr	$2
	nop

	lui	$2,%hi(gpPhyPriv)
	lw	$16,%lo(gpPhyPriv)($2)
	lui	$2,%hi(LDDLA_task_wait)
	addiu	$4,$2,%lo(LDDLA_task_wait)
	move	$5,$0
	lui	$2,%hi($LC29)
	addiu	$6,$2,%lo($LC29)
	lui	$2,%hi(kthread_create)
	addiu	$2,$2,%lo(kthread_create)
	jalr	$2
	nop

	sw	$2,28($sp)
	lw	$2,28($sp)
	sw	$2,44($sp)
	lw	$2,44($sp)
	sltu	$2,$2,-4095
	xori	$2,$2,0x1
	andi	$2,$2,0x00ff
	bne	$2,$0,$L148
	nop

	lw	$4,28($sp)
	lui	$2,%hi(wake_up_process)
	addiu	$2,$2,%lo(wake_up_process)
	jalr	$2
	nop

$L148:
	lw	$2,28($sp)
	sw	$2,112($16)
	j	$L149
	nop

$L144:
	lui	$2,%hi($LC30)
	addiu	$4,$2,%lo($LC30)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L149:
	lui	$2,%hi(phy_trans_model_setting)
	addiu	$2,$2,%lo(phy_trans_model_setting)
	jalr	$2
	nop

	lui	$2,%hi(phy_date_version)
	lw	$5,%lo(phy_date_version)($2)
	lui	$2,%hi(phy_version)
	lw	$6,%lo(phy_version)($2)
	lui	$2,%hi(mt7570_version)
	lw	$3,%lo(mt7570_version)($2)
	lui	$2,%hi($LC31)
	addiu	$4,$2,%lo($LC31)
	move	$7,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi(gphy_rx_test_timer)
	addiu	$4,$2,%lo(gphy_rx_test_timer)
	move	$5,$0
	move	$6,$0
	lui	$2,%hi(init_timer_key)
	addiu	$2,$2,%lo(init_timer_key)
	jalr	$2
	nop

	lui	$2,%hi(gphy_rx_test_timer)
	addiu	$2,$2,%lo(gphy_rx_test_timer)
	li	$3,10000			# 0x2710
	sw	$3,20($2)
	lui	$2,%hi(gphy_rx_test_timer)
	addiu	$2,$2,%lo(gphy_rx_test_timer)
	lui	$3,%hi(phy_rx_sens_timer)
	addiu	$3,$3,%lo(phy_rx_sens_timer)
	sw	$3,16($2)
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	addiu	$2,$2,28
	move	$4,$2
	move	$5,$0
	move	$6,$0
	lui	$2,%hi(init_timer_key)
	addiu	$2,$2,%lo(init_timer_key)
	jalr	$2
	nop

	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	li	$3,50000			# 0xc350
	sw	$3,48($2)
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lui	$3,%hi(phy_int_recover_expires)
	addiu	$3,$3,%lo(phy_int_recover_expires)
	sw	$3,44($2)
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	addiu	$2,$2,124
	move	$4,$2
	move	$5,$0
	li	$6,12			# 0xc
	lui	$2,%hi(memset)
	addiu	$2,$2,%lo(memset)
	jalr	$2
	nop

	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	li	$3,-56			# 0xffffffffffffffc8
	sb	$3,124($2)
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	li	$3,30			# 0x1e
	sb	$3,125($2)
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	sh	$0,126($2)
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	sw	$0,128($2)
	lui	$2,%hi(gpPhyPriv)
	lw	$3,%lo(gpPhyPriv)($2)
	lbu	$2,132($3)
	li	$4,1			# 0x1
	ins	$2,$4,7,1
	sb	$2,132($3)
	lui	$2,%hi(gpPhyPriv)
	lw	$3,%lo(gpPhyPriv)($2)
	lbu	$2,132($3)
	ins	$2,$0,6,1
	sb	$2,132($3)
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	li	$3,1			# 0x1
	sh	$3,136($2)
	lui	$2,%hi(phy_api_dispatch_hook_ops)
	addiu	$4,$2,%lo(phy_api_dispatch_hook_ops)
	lui	$2,%hi(ecnt_register_hook)
	addiu	$2,$2,%lo(ecnt_register_hook)
	jalr	$2
	nop

	beq	$2,$0,$L150
	nop

	lui	$2,%hi($LC32)
	addiu	$4,$2,%lo($LC32)
	lui	$2,%hi(__func__.27178)
	addiu	$5,$2,%lo(__func__.27178)
	li	$6,980			# 0x3d4
	lui	$2,%hi(panic)
	addiu	$2,$2,%lo(panic)
	jalr	$2
	nop

$L150:
	lw	$2,16($sp)
$L151:
	lw	$31,60($sp)
	lw	$16,56($sp)
	addiu	$sp,$sp,64
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	pon_phy_init
	.size	pon_phy_init, .-pon_phy_init
	.globl	init_module
	init_module = pon_phy_init
	.section	.text.pon_phy_deinit,"ax",@progbits
	.align	2
	.globl	pon_phy_deinit
	.set	nomips16
	.set	nomicromips
	.ent	pon_phy_deinit
	.type	pon_phy_deinit, @function
pon_phy_deinit:
	.frame	$sp,80,$31		# vars= 56, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-80
	sw	$31,76($sp)
	move	$2,$28
	lw	$2,20($2)
	move	$3,$2
	li	$2,134152192			# 0x7ff0000
	ori	$2,$2,0xff00
	and	$2,$3,$2
	beq	$2,$0,$L153
	nop

	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	addiu	$2,$2,28
	move	$4,$2
	lui	$2,%hi(del_timer)
	addiu	$2,$2,%lo(del_timer)
	jalr	$2
	nop

	j	$L154
	nop

$L153:
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	addiu	$2,$2,28
	move	$4,$2
	lui	$2,%hi(del_timer_sync)
	addiu	$2,$2,%lo(del_timer_sync)
	jalr	$2
	nop

$L154:
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lw	$2,112($2)
	sw	$2,16($sp)
	lw	$2,16($sp)
	sltu	$2,$2,-4095
	xori	$2,$2,0x1
	andi	$2,$2,0x00ff
	bne	$2,$0,$L156
	nop

	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lw	$2,112($2)
	move	$4,$2
	lui	$2,%hi(kthread_stop)
	addiu	$2,$2,%lo(kthread_stop)
	jalr	$2
	nop

$L156:
	li	$2,16			# 0x10
	sw	$2,20($sp)
	li	$2,5			# 0x5
	sw	$2,24($sp)
	sw	$0,28($sp)
	li	$2,6			# 0x6
	sw	$2,32($sp)
	lw	$2,24($sp)
	sw	$2,40($sp)
	addiu	$2,$sp,32
	lw	$4,20($sp)
	move	$5,$0
	move	$6,$2
	lui	$2,%hi(__ECNT_HOOK)
	addiu	$2,$2,%lo(__ECNT_HOOK)
	jalr	$2
	nop

	sw	$2,28($sp)
	move	$4,$0
	lui	$2,%hi(phy_trans_power_switch)
	addiu	$2,$2,%lo(phy_trans_power_switch)
	jalr	$2
	nop

	lui	$2,%hi(phy_debug_deinit)
	addiu	$2,$2,%lo(phy_debug_deinit)
	jalr	$2
	nop

	lui	$2,%hi(mt7570_select)
	lw	$3,%lo(mt7570_select)($2)
	li	$2,1			# 0x1
	bne	$3,$2,$L160
	nop

	lui	$2,%hi(i2c_access_queue)
	lw	$2,%lo(i2c_access_queue)($2)
	move	$4,$2
	lui	$2,%hi(flush_workqueue)
	addiu	$2,$2,%lo(flush_workqueue)
	jalr	$2
	nop

	lui	$2,%hi(i2c_access_queue)
	lw	$2,%lo(i2c_access_queue)($2)
	move	$4,$2
	lui	$2,%hi(destroy_workqueue)
	addiu	$2,$2,%lo(destroy_workqueue)
	jalr	$2
	nop

$L160:
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	beq	$2,$0,$L161
	nop

	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	move	$4,$2
	lui	$2,%hi(kfree)
	addiu	$2,$2,%lo(kfree)
	jalr	$2
	nop

	lui	$2,%hi(gpPhyPriv)
	sw	$0,%lo(gpPhyPriv)($2)
$L161:
	lui	$2,%hi(phy_api_dispatch_hook_ops)
	addiu	$4,$2,%lo(phy_api_dispatch_hook_ops)
	lui	$2,%hi(ecnt_unregister_hook)
	addiu	$2,$2,%lo(ecnt_unregister_hook)
	jalr	$2
	nop

	lw	$31,76($sp)
	addiu	$sp,$sp,80
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	pon_phy_deinit
	.size	pon_phy_deinit, .-pon_phy_deinit
	.globl	cleanup_module
	cleanup_module = pon_phy_deinit
	.section	.modinfo,"a",@progbits
	.align	2
	.type	__mod_license1073, @object
	.size	__mod_license1073, 12
__mod_license1073:
	.ascii	"license=GPL\000"
	.section	__ksymtab_strings,"a",@progbits
	.type	__kstrtab_phy_register_cb_handler, @object
	.size	__kstrtab_phy_register_cb_handler, 24
__kstrtab_phy_register_cb_handler:
	.ascii	"phy_register_cb_handler\000"
	.section	__ksymtab,"a",@progbits
	.align	2
	.type	__ksymtab_phy_register_cb_handler, @object
	.size	__ksymtab_phy_register_cb_handler, 8
__ksymtab_phy_register_cb_handler:
	.word	phy_register_cb_handler
	.word	__kstrtab_phy_register_cb_handler
	.section	__ksymtab_strings
	.type	__kstrtab_phy_unregister_cb_handler, @object
	.size	__kstrtab_phy_unregister_cb_handler, 26
__kstrtab_phy_unregister_cb_handler:
	.ascii	"phy_unregister_cb_handler\000"
	.section	__ksymtab
	.align	2
	.type	__ksymtab_phy_unregister_cb_handler, @object
	.size	__ksymtab_phy_unregister_cb_handler, 8
__ksymtab_phy_unregister_cb_handler:
	.word	phy_unregister_cb_handler
	.word	__kstrtab_phy_unregister_cb_handler
	.rdata
	.align	2
	.type	__func__.27178, @object
	.size	__func__.27178, 13
__func__.27178:
	.ascii	"pon_phy_init\000"
	.align	3
$LC16:
	.word	1108927001
	.word	1697371128
	.align	3
$LC17:
	.word	1114446484
	.word	-1577058304
	.ident	"GCC: (Buildroot 2015.08.1) 4.9.3"
