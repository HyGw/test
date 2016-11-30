	.file	1 "qdma_bmgr.c"
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
	.globl	qdma_operation
	.data
	.align	2
	.type	qdma_operation, @object
	.size	qdma_operation, 396
qdma_operation:
	.word	qdma_init
	.word	qdma_deinit
	.word	qdma_tx_dma_mode
	.word	qdma_rx_dma_mode
	.word	qdma_loopback_mode
	.word	qdma_register_callback_function
	.word	qdma_unregister_callback_function
	.word	qdma_enable_rxpkt_int
	.word	qdma_disable_rxpkt_int
	.word	qdma_receive_packets
	.word	qdma_transmit_packet
	.word	qdma_set_tx_qos_weight
	.word	qdma_get_tx_qos_weight
	.word	qdma_set_tx_qos
	.word	qdma_get_tx_qos
	.word	qdma_set_mac_limit_threshold
	.word	qdma_get_mac_limit_threshold
	.word	qdma_set_txbuf_threshold
	.word	qdma_get_txbuf_threshold
	.word	qdma_set_prefetch_mode
	.word	qdma_set_pktsize_overhead_en
	.word	qdma_get_pktsize_overhead_en
	.word	qdma_set_pktsize_overhead_value
	.word	qdma_get_pktsize_overhead_value
	.word	qdma_set_lmgr_low_threshold
	.word	qdma_get_lmgr_low_threshold
	.word	qdma_get_lmgr_status
	.word	qdma_set_dbg_level
	.word	qdma_dump_dma_busy
	.word	qdma_dump_reg_polling
	.word	qdma_set_force_receive_rx_ring1
	.word	qdma_set_tx_drop_en
	.word	qdma_get_tx_drop_en
	.word	qdma_set_tx_ratemeter
	.word	qdma_get_tx_ratemeter
	.word	qdma_enable_tx_ratelimit
	.word	qdma_set_tx_ratelimit_cfg
	.word	qdma_get_tx_ratelimit_cfg
	.word	qdma_set_tx_ratelimit
	.word	qdma_get_tx_ratelimit
	.word	qdma_set_tx_dba_report
	.word	qdma_get_tx_dba_report
	.word	qdma_set_rx_protect_en
	.word	qdma_get_rx_protect_en
	.word	qdma_set_rx_low_threshold
	.word	qdma_get_rx_low_threshold
	.word	qdma_set_rx_ratelimit_en
	.word	qdma_set_rx_ratelimit_pkt_mode
	.word	qdma_get_rx_ratelimit_cfg
	.word	qdma_set_rx_ratelimit
	.word	qdma_get_rx_ratelimit
	.word	qdma_set_txq_dei_drop_mode
	.word	qdma_get_txq_dei_drop_mode
	.word	qdma_set_txq_cngst_mode
	.word	qdma_get_txq_cngst_mode
	.word	qdma_set_txq_dei_threshold_scale
	.word	qdma_get_txq_dei_threshold_scale
	.word	qdma_set_txq_cngst_auto_config
	.word	qdma_get_txq_cngst_auto_config
	.word	qdma_set_txq_cngst_dynamic_threshold
	.word	qdma_get_txq_cngst_dynamic_threshold
	.word	qdma_set_txq_cngst_total_threshold
	.word	qdma_get_txq_cngst_total_threshold
	.word	qdma_set_txq_cngst_channel_threshold
	.word	qdma_get_txq_cngst_channel_threshold
	.word	qdma_set_txq_cngst_queue_threshold
	.word	qdma_get_txq_cngst_queue_threshold
	.word	qdma_set_txq_peekrate_params
	.word	qdma_get_txq_peekrate_params
	.word	qdma_set_txq_cngst_static_queue_normal_threshold
	.word	qdma_set_txq_cngst_static_queue_dei_threshold
	.word	qdma_get_txq_cngst_dynamic_info
	.word	qdma_get_txq_cngst_static_info
	.word	qdma_set_txq_cngst_queue_nonblocking
	.word	qdma_get_txq_cngst_queue_nonblocking
	.word	qdma_set_txq_cngst_channel_nonblocking
	.word	qdma_get_txq_cngst_channel_nonblocking
	.word	qdma_set_virtual_channel_mode
	.word	qdma_get_virtual_channel_mode
	.word	qdma_set_virtual_channel_qos
	.word	qdma_get_virtual_channel_qos
	.word	qdma_clear_and_set_dbg_cntr_channel_group
	.word	qdma_clear_and_set_dbg_cntr_queue_group
	.word	qdma_clear_dbg_cntr_value_all
	.word	qdma_dump_dbg_cntr_value
	.word	qdma_dump_tx_qos
	.word	qdma_dump_virtual_channel_qos
	.word	qdma_dump_tx_ratelimit
	.word	qdma_dump_rx_ratelimit
	.word	qdma_dump_tx_dba_report
	.word	qdma_dump_txq_cngst
	.word	qdma_clear_cpu_counters
	.word	qdma_dump_cpu_counters
	.word	qdma_dump_register_value
	.word	qdma_dump_descriptor_info
	.word	qdma_dump_irq_info
	.word	qdma_dump_hwfwd_info
	.word	qdma_dump_info_all
	.word	qdma_read_vip_info
	.globl	gpQdmaPriv
	.section	.bss,"aw",@nobits
	.align	2
	.type	gpQdmaPriv, @object
	.size	gpQdmaPriv, 4
gpQdmaPriv:
	.space	4
	.globl	storm_ctrl_shrehold_lan
	.data
	.align	2
	.type	storm_ctrl_shrehold_lan, @object
	.size	storm_ctrl_shrehold_lan, 4
storm_ctrl_shrehold_lan:
	.word	120
	.globl	hwfwd_payload_max_size
	.section	.bss
	.align	2
	.type	hwfwd_payload_max_size, @object
	.size	hwfwd_payload_max_size, 4
hwfwd_payload_max_size:
	.space	4
	.section	__ksymtab_strings,"a",@progbits
	.type	__kstrtab_storm_ctrl_shrehold_lan, @object
	.size	__kstrtab_storm_ctrl_shrehold_lan, 24
__kstrtab_storm_ctrl_shrehold_lan:
	.ascii	"storm_ctrl_shrehold_lan\000"
	.section	__ksymtab,"a",@progbits
	.align	2
	.type	__ksymtab_storm_ctrl_shrehold_lan, @object
	.size	__ksymtab_storm_ctrl_shrehold_lan, 8
__ksymtab_storm_ctrl_shrehold_lan:
	.word	storm_ctrl_shrehold_lan
	.word	__kstrtab_storm_ctrl_shrehold_lan
	.globl	qdma_lan_fwd_timer
	.data
	.align	2
	.type	qdma_lan_fwd_timer, @object
	.size	qdma_lan_fwd_timer, 4
qdma_lan_fwd_timer:
	.word	100
	.section	__ksymtab_strings
	.type	__kstrtab_qdma_lan_fwd_timer, @object
	.size	__kstrtab_qdma_lan_fwd_timer, 19
__kstrtab_qdma_lan_fwd_timer:
	.ascii	"qdma_lan_fwd_timer\000"
	.section	__ksymtab
	.align	2
	.type	__ksymtab_qdma_lan_fwd_timer, @object
	.size	__ksymtab_qdma_lan_fwd_timer, 8
__ksymtab_qdma_lan_fwd_timer:
	.word	qdma_lan_fwd_timer
	.word	__kstrtab_qdma_lan_fwd_timer
	.globl	physical_size
	.section	.bss
	.align	2
	.type	physical_size, @object
	.size	physical_size, 4
physical_size:
	.space	4
	.globl	macType
	.align	2
	.type	macType, @object
	.size	macType, 4
macType:
	.space	4
	.globl	gQueueMask
	.align	2
	.type	gQueueMask, @object
	.size	gQueueMask, 4
gQueueMask:
	.space	4
	.globl	isNeedHWQoS
	.data
	.align	2
	.type	isNeedHWQoS, @object
	.size	isNeedHWQoS, 4
isNeedHWQoS:
	.word	1
	.globl	qos_wrr_info
	.section	.bss
	.align	2
	.type	qos_wrr_info, @object
	.size	qos_wrr_info, 20
qos_wrr_info:
	.space	20
	.globl	max_prio
	.data
	.align	2
	.type	max_prio, @object
	.size	max_prio, 4
max_prio:
	.word	3
	.globl	qos_wrr_user
	.section	.bss
	.type	qos_wrr_user, @object
	.size	qos_wrr_user, 1
qos_wrr_user:
	.space	1
	.section	.text.qdma_kfree_skb,"ax",@progbits
	.align	2
	.globl	qdma_kfree_skb
	.set	nomips16
	.set	nomicromips
	.ent	qdma_kfree_skb
	.type	qdma_kfree_skb, @function
qdma_kfree_skb:
	.frame	$sp,64,$31		# vars= 40, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	addiu	$sp,$sp,-64
	sw	$31,60($sp)
	sw	$4,64($sp)
	sw	$5,68($sp)
	sw	$0,20($sp)
	sw	$0,24($sp)
	sw	$0,16($sp)
	lw	$2,68($sp)
	ext	$2,$2,3,8
	andi	$2,$2,0x00ff
	sw	$2,20($sp)
	lw	$2,68($sp)
	ext	$2,$2,0,3
	andi	$2,$2,0x00ff
	sw	$2,24($sp)
	lw	$2,20($sp)
	sll	$3,$2,2
	lui	$2,%hi(chnlLimit)
	addiu	$2,$2,%lo(chnlLimit)
	addu	$2,$3,$2
	li	$3,1			# 0x1
	sw	$3,28($sp)
	sw	$2,32($sp)
	lw	$4,32($sp)
	lw	$3,28($sp)
	lw	$2,32($sp)
#APP
 # 112 "/opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/atomic.h" 1
		.set	mips3					
1:	ll	$5, 0($4)		# atomic_sub		
	subu	$5, $3					
	sc	$5, 0($4)					
	beqz	$5, 2f					
	.subsection 2					
2:	b	1b					
	.previous					
	.set	mips0					

 # 0 "" 2
#NO_APP
	move	$2,$5
	sw	$2,36($sp)
	lw	$2,20($sp)
	sll	$3,$2,3
	lw	$2,24($sp)
	addu	$2,$3,$2
	sll	$3,$2,2
	lui	$2,%hi(queueLimit)
	addiu	$2,$2,%lo(queueLimit)
	addu	$2,$3,$2
	li	$3,1			# 0x1
	sw	$3,40($sp)
	sw	$2,44($sp)
	lw	$4,44($sp)
	lw	$3,40($sp)
	lw	$2,44($sp)
#APP
 # 112 "/opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/atomic.h" 1
		.set	mips3					
1:	ll	$5, 0($4)		# atomic_sub		
	subu	$5, $3					
	sc	$5, 0($4)					
	beqz	$5, 2f					
	.subsection 2					
2:	b	1b					
	.previous					
	.set	mips0					

 # 0 "" 2
#NO_APP
	move	$2,$5
	sw	$2,48($sp)
	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lhu	$3,188($2)
	addiu	$3,$3,1
	andi	$3,$3,0xffff
	sh	$3,188($2)
	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lw	$2,156($2)
	beq	$2,$0,$L2
	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lw	$2,156($2)
	lw	$4,64($sp)
	lw	$5,68($sp)
	jalr	$2
	beq	$2,$0,$L4
	li	$2,-1			# 0xffffffffffffffff
	sw	$2,16($sp)
	j	$L4
$L2:
	lw	$4,64($sp)
	lui	$2,%hi(dev_kfree_skb_any)
	addiu	$2,$2,%lo(dev_kfree_skb_any)
	jalr	$2
$L4:
	lw	$2,16($sp)
	lw	$31,60($sp)
	addiu	$sp,$sp,64
	j	$31
	.end	qdma_kfree_skb
	.size	qdma_kfree_skb, .-qdma_kfree_skb
	.rdata
	.align	2
$LC0:
	.ascii	"Fault: Rate Meter unit is %d, which should between 2 and"
	.ascii	" %d\012\000"
	.section	.text.qdmaRateMeterValueCalCulate,"ax",@progbits
	.align	2
	.globl	qdmaRateMeterValueCalCulate
	.set	nomips16
	.set	nomicromips
	.ent	qdmaRateMeterValueCalCulate
	.type	qdmaRateMeterValueCalCulate, @function
qdmaRateMeterValueCalCulate:
	.frame	$sp,48,$31		# vars= 24, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-48
	sw	$31,44($sp)
	sw	$4,48($sp)
	sw	$5,52($sp)
	sw	$6,56($sp)
	li	$2,2147418112			# 0x7fff0000
	ori	$2,$2,0xffff
	sw	$2,16($sp)
	sw	$0,20($sp)
	sw	$0,24($sp)
	sw	$0,28($sp)
	lw	$2,48($sp)
	slt	$2,$2,2
	bne	$2,$0,$L7
	nop

	lw	$2,48($sp)
	slt	$2,$2,65
	bne	$2,$0,$L8
	nop

$L7:
	lui	$2,%hi($LC0)
	addiu	$4,$2,%lo($LC0)
	lw	$5,48($sp)
	li	$6,64			# 0x40
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	li	$2,-1			# 0xffffffffffffffff
	j	$L9
	nop

$L8:
	sw	$0,20($sp)
	j	$L10
	nop

$L14:
	li	$2,65536			# 0x10000
	ori	$3,$2,0xf400
	lw	$2,20($sp)
	sll	$3,$3,$2
	lw	$2,48($sp)
	div	$0,$3,$2
	mfhi	$3
	mflo	$2
	sw	$2,24($sp)
	lw	$2,24($sp)
	blez	$2,$L11
	nop

	lw	$3,24($sp)
	li	$2,65536			# 0x10000
	slt	$2,$3,$2
	beq	$2,$0,$L11
	nop

	li	$2,65536			# 0x10000
	ori	$3,$2,0xf400
	lw	$2,20($sp)
	sll	$3,$3,$2
	lw	$2,24($sp)
	div	$0,$3,$2
	mfhi	$3
	mflo	$2
	sw	$2,28($sp)
	lw	$2,28($sp)
	sw	$2,32($sp)
	lw	$2,32($sp)
	bgez	$2,$L12
	nop

	subu	$2,$0,$2
$L12:
	move	$3,$2
	lw	$2,16($sp)
	sw	$2,36($sp)
	lw	$2,36($sp)
	bgez	$2,$L13
	nop

	subu	$2,$0,$2
$L13:
	slt	$2,$3,$2
	beq	$2,$0,$L11
	nop

	lw	$2,28($sp)
	sw	$2,16($sp)
	lw	$2,20($sp)
	andi	$3,$2,0xffff
	lw	$2,52($sp)
	sh	$3,0($2)
	lw	$2,24($sp)
	andi	$3,$2,0xffff
	lw	$2,56($sp)
	sh	$3,0($2)
	lw	$2,16($sp)
	bne	$2,$0,$L11
	nop

	li	$2,1			# 0x1
	j	$L9
	nop

$L11:
	lw	$2,20($sp)
	addiu	$2,$2,1
	sw	$2,20($sp)
$L10:
	lw	$2,20($sp)
	slt	$2,$2,4
	bne	$2,$0,$L14
	nop

	move	$2,$0
$L9:
	lw	$31,44($sp)
	addiu	$sp,$sp,48
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	qdmaRateMeterValueCalCulate
	.size	qdmaRateMeterValueCalCulate, .-qdmaRateMeterValueCalCulate
	.section	.text.qdma_set_tx_delay,"ax",@progbits
	.align	2
	.globl	qdma_set_tx_delay
	.set	nomips16
	.set	nomicromips
	.ent	qdma_set_tx_delay
	.type	qdma_set_tx_delay, @function
qdma_set_tx_delay:
	.frame	$sp,40,$31		# vars= 40, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-40
	move	$3,$4
	move	$2,$5
	sb	$3,40($sp)
	sh	$2,44($sp)
	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lw	$2,0($2)
	sw	$2,0($sp)
	lbu	$2,40($sp)
	beq	$2,$0,$L16
	nop

	lbu	$2,40($sp)
	andi	$3,$2,0xffff
	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lhu	$2,30($2)
	sltu	$2,$2,$3
	beq	$2,$0,$L17
	nop

$L16:
	li	$2,-22			# 0xffffffffffffffea
	j	$L18
	nop

$L17:
	lw	$2,0($sp)
	addiu	$2,$2,100
	sw	$2,12($sp)
	lw	$2,12($sp)
	lw	$2,0($2)
	sw	$2,4($sp)
	lw	$2,0($sp)
	addiu	$3,$2,100
	lw	$4,4($sp)
	li	$2,-268435456			# 0xfffffffff0000000
	ori	$2,$2,0xffff
	and	$4,$4,$2
	lbu	$2,40($sp)
	sll	$2,$2,16
	move	$5,$2
	li	$2,268369920			# 0xfff0000
	and	$2,$5,$2
	or	$2,$4,$2
	sw	$3,16($sp)
	sw	$2,20($sp)
	lw	$2,16($sp)
	lw	$3,20($sp)
	sw	$3,0($2)
	lw	$2,0($sp)
	addiu	$2,$2,112
	sw	$2,24($sp)
	lw	$2,24($sp)
	lw	$2,0($2)
	sw	$2,8($sp)
	lw	$2,0($sp)
	addiu	$3,$2,112
	lw	$4,8($sp)
	li	$2,-65536			# 0xffffffffffff0000
	and	$4,$4,$2
	lhu	$2,44($sp)
	or	$2,$4,$2
	sw	$3,28($sp)
	sw	$2,32($sp)
	lw	$2,28($sp)
	lw	$3,32($sp)
	sw	$3,0($2)
	move	$2,$0
$L18:
	addiu	$sp,$sp,40
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	qdma_set_tx_delay
	.size	qdma_set_tx_delay, .-qdma_set_tx_delay
	.section	.text.qdma_set_rx_delay,"ax",@progbits
	.align	2
	.globl	qdma_set_rx_delay
	.set	nomips16
	.set	nomicromips
	.ent	qdma_set_rx_delay
	.type	qdma_set_rx_delay, @function
qdma_set_rx_delay:
	.frame	$sp,40,$31		# vars= 40, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-40
	move	$3,$4
	move	$2,$5
	sw	$6,48($sp)
	sb	$3,40($sp)
	sb	$2,44($sp)
	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lw	$2,0($2)
	sw	$2,0($sp)
	lbu	$2,40($sp)
	beq	$2,$0,$L22
	nop

	lw	$2,0($sp)
	addiu	$2,$2,92
	sw	$2,12($sp)
	lw	$2,12($sp)
	lw	$2,0($2)
	sw	$2,4($sp)
	lw	$2,0($sp)
	addiu	$3,$2,92
	lw	$2,48($sp)
	sll	$2,$2,4
	li	$4,65535			# 0xffff
	sll	$2,$4,$2
	nor	$2,$0,$2
	move	$4,$2
	lw	$2,4($sp)
	and	$2,$4,$2
	lbu	$4,40($sp)
	sll	$4,$4,8
	ori	$5,$4,0x8000
	lbu	$4,44($sp)
	or	$5,$5,$4
	lw	$4,48($sp)
	sll	$4,$4,4
	sll	$5,$5,$4
	lw	$4,48($sp)
	sll	$4,$4,4
	li	$6,65535			# 0xffff
	sll	$4,$6,$4
	and	$4,$5,$4
	or	$2,$2,$4
	sw	$3,16($sp)
	sw	$2,20($sp)
	lw	$2,16($sp)
	lw	$3,20($sp)
	sw	$3,0($2)
	j	$L24
	nop

$L22:
	lw	$2,0($sp)
	addiu	$2,$2,92
	sw	$2,24($sp)
	lw	$2,24($sp)
	lw	$2,0($2)
	sw	$2,8($sp)
	lw	$2,0($sp)
	addiu	$3,$2,92
	lw	$2,48($sp)
	sll	$2,$2,4
	li	$4,65535			# 0xffff
	sll	$2,$4,$2
	nor	$2,$0,$2
	move	$4,$2
	lw	$2,8($sp)
	and	$2,$4,$2
	sw	$3,28($sp)
	sw	$2,32($sp)
	lw	$2,28($sp)
	lw	$3,32($sp)
	sw	$3,0($2)
$L24:
	move	$2,$0
	addiu	$sp,$sp,40
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	qdma_set_rx_delay
	.size	qdma_set_rx_delay, .-qdma_set_rx_delay
	.section	.text.qdma_bm_tx_polling_mode,"ax",@progbits
	.align	2
	.globl	qdma_bm_tx_polling_mode
	.set	nomips16
	.set	nomicromips
	.ent	qdma_bm_tx_polling_mode
	.type	qdma_bm_tx_polling_mode, @function
qdma_bm_tx_polling_mode:
	.frame	$sp,32,$31		# vars= 8, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-32
	sw	$31,28($sp)
	sw	$4,32($sp)
	move	$2,$5
	sb	$2,36($sp)
	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lw	$2,0($2)
	sw	$2,16($sp)
	lw	$3,32($sp)
	li	$2,1			# 0x1
	bne	$3,$2,$L28
	nop

	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lbu	$3,36($sp)
	sb	$3,141($2)
	lw	$4,16($sp)
	li	$5,529			# 0x211
	lui	$2,%hi(qdmaDisableInt)
	addiu	$2,$2,%lo(qdmaDisableInt)
	jalr	$2
	nop

	lui	$2,%hi(gpQdmaPriv)
	lw	$3,%lo(gpQdmaPriv)($2)
	lbu	$2,140($3)
	li	$4,1			# 0x1
	ins	$2,$4,7,1
	sb	$2,140($3)
	j	$L29
	nop

$L28:
	lui	$2,%hi(gpQdmaPriv)
	lw	$3,%lo(gpQdmaPriv)($2)
	lbu	$2,140($3)
	ins	$2,$0,7,1
	sb	$2,140($3)
	lw	$4,16($sp)
	li	$5,529			# 0x211
	lui	$2,%hi(qdmaEnableInt)
	addiu	$2,$2,%lo(qdmaEnableInt)
	jalr	$2
	nop

$L29:
	move	$2,$0
	lw	$31,28($sp)
	addiu	$sp,$sp,32
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	qdma_bm_tx_polling_mode
	.size	qdma_bm_tx_polling_mode, .-qdma_bm_tx_polling_mode
	.section	.text.qdma_receive_packet_mode,"ax",@progbits
	.align	2
	.globl	qdma_receive_packet_mode
	.set	nomips16
	.set	nomicromips
	.ent	qdma_receive_packet_mode
	.type	qdma_receive_packet_mode, @function
qdma_receive_packet_mode:
	.frame	$sp,32,$31		# vars= 8, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-32
	sw	$31,28($sp)
	sw	$4,32($sp)
	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lw	$2,0($2)
	sw	$2,16($sp)
	lw	$2,32($sp)
	bne	$2,$0,$L32
	nop

	lw	$4,16($sp)
	li	$5,170			# 0xaa
	lui	$2,%hi(qdmaDisableInt)
	addiu	$2,$2,%lo(qdmaDisableInt)
	jalr	$2
	nop

	lui	$2,%hi(gpQdmaPriv)
	lw	$3,%lo(gpQdmaPriv)($2)
	lbu	$2,140($3)
	li	$4,1			# 0x1
	ins	$2,$4,6,1
	sb	$2,140($3)
	lui	$2,%hi(gpQdmaPriv)
	lw	$3,%lo(gpQdmaPriv)($2)
	lbu	$2,140($3)
	ins	$2,$0,5,1
	sb	$2,140($3)
	j	$L33
	nop

$L32:
	lui	$2,%hi(gpQdmaPriv)
	lw	$3,%lo(gpQdmaPriv)($2)
	lbu	$2,140($3)
	ins	$2,$0,6,1
	sb	$2,140($3)
	lw	$3,32($sp)
	li	$2,2			# 0x2
	bne	$3,$2,$L34
	nop

	lui	$2,%hi(gpQdmaPriv)
	lw	$3,%lo(gpQdmaPriv)($2)
	lbu	$2,140($3)
	li	$4,1			# 0x1
	ins	$2,$4,5,1
	sb	$2,140($3)
	j	$L35
	nop

$L34:
	lui	$2,%hi(gpQdmaPriv)
	lw	$3,%lo(gpQdmaPriv)($2)
	lbu	$2,140($3)
	ins	$2,$0,5,1
	sb	$2,140($3)
$L35:
	lw	$4,16($sp)
	li	$5,170			# 0xaa
	lui	$2,%hi(qdmaEnableInt)
	addiu	$2,$2,%lo(qdmaEnableInt)
	jalr	$2
	nop

$L33:
	move	$2,$0
	lw	$31,28($sp)
	addiu	$sp,$sp,32
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	qdma_receive_packet_mode
	.size	qdma_receive_packet_mode, .-qdma_receive_packet_mode
	.section	.text.qdma_recycle_packet_mode,"ax",@progbits
	.align	2
	.globl	qdma_recycle_packet_mode
	.set	nomips16
	.set	nomicromips
	.ent	qdma_recycle_packet_mode
	.type	qdma_recycle_packet_mode, @function
qdma_recycle_packet_mode:
	.frame	$sp,32,$31		# vars= 8, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-32
	sw	$31,28($sp)
	sw	$4,32($sp)
	move	$2,$5
	sb	$2,36($sp)
	sw	$0,16($sp)
	lw	$2,32($sp)
	bne	$2,$0,$L38
	nop

	lbu	$2,36($sp)
	beq	$2,$0,$L39
	nop

	lbu	$2,36($sp)
	li	$4,1			# 0x1
	move	$5,$2
	lui	$2,%hi(qdma_bm_tx_polling_mode)
	addiu	$2,$2,%lo(qdma_bm_tx_polling_mode)
	jalr	$2
	nop

	sw	$2,16($sp)
	j	$L41
	nop

$L39:
	li	$4,1			# 0x1
	li	$5,32			# 0x20
	lui	$2,%hi(qdma_bm_tx_polling_mode)
	addiu	$2,$2,%lo(qdma_bm_tx_polling_mode)
	jalr	$2
	nop

	sw	$2,16($sp)
	j	$L41
	nop

$L38:
	move	$4,$0
	move	$5,$0
	lui	$2,%hi(qdma_bm_tx_polling_mode)
	addiu	$2,$2,%lo(qdma_bm_tx_polling_mode)
	jalr	$2
	nop

	sw	$2,16($sp)
$L41:
	lw	$2,16($sp)
	lw	$31,28($sp)
	addiu	$sp,$sp,32
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	qdma_recycle_packet_mode
	.size	qdma_recycle_packet_mode, .-qdma_recycle_packet_mode
	.section	.imem_text,"ax",@progbits
	.align	2
	.globl	allocate_skb_buffer
	.set	nomips16
	.set	nomicromips
	.ent	allocate_skb_buffer
	.type	allocate_skb_buffer, @function
allocate_skb_buffer:
	.frame	$sp,64,$31		# vars= 40, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-64
	sw	$31,60($sp)
	sw	$0,16($sp)
	sw	$0,20($sp)
	lui	$2,%hi(skbmgr_alloc_skb2k)
	addiu	$2,$2,%lo(skbmgr_alloc_skb2k)
	jalr	$2
	nop

	sw	$2,24($sp)
	lw	$2,24($sp)
	sltu	$2,$0,$2
	andi	$2,$2,0x00ff
	beq	$2,$0,$L44
	nop

	lw	$2,24($sp)
	sw	$2,28($sp)
	li	$2,32			# 0x20
	sw	$2,32($sp)
	lw	$2,28($sp)
	lw	$3,256($2)
	lw	$2,32($sp)
	addu	$3,$3,$2
	lw	$2,28($sp)
	sw	$3,256($2)
	lw	$2,28($sp)
	lw	$3,244($2)
	lw	$2,32($sp)
	addu	$3,$3,$2
	lw	$2,28($sp)
	sw	$3,244($2)
$L44:
	lw	$2,24($sp)
	sw	$2,20($sp)
	lw	$2,20($sp)
	bne	$2,$0,$L46
	nop

	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lw	$3,176($2)
	addiu	$3,$3,1
	sw	$3,176($2)
	j	$L47
	nop

$L46:
	lw	$2,20($sp)
	lw	$2,244($2)
	andi	$2,$2,0x7
	sw	$2,16($sp)
	lw	$2,16($sp)
	beq	$2,$0,$L48
	nop

	li	$3,8			# 0x8
	lw	$2,16($sp)
	subu	$2,$3,$2
	lw	$3,20($sp)
	sw	$3,36($sp)
	sw	$2,40($sp)
	lw	$2,36($sp)
	lw	$3,256($2)
	lw	$2,40($sp)
	addu	$3,$3,$2
	lw	$2,36($sp)
	sw	$3,256($2)
	lw	$2,36($sp)
	lw	$3,244($2)
	lw	$2,40($sp)
	addu	$3,$3,$2
	lw	$2,36($sp)
	sw	$3,244($2)
$L48:
	lw	$2,20($sp)
	sw	$2,44($sp)
	li	$2,2			# 0x2
	sw	$2,48($sp)
	lw	$2,44($sp)
	lw	$3,256($2)
	lw	$2,48($sp)
	addu	$3,$3,$2
	lw	$2,44($sp)
	sw	$3,256($2)
	lw	$2,44($sp)
	lw	$3,244($2)
	lw	$2,48($sp)
	addu	$3,$3,$2
	lw	$2,44($sp)
	sw	$3,244($2)
$L47:
	lw	$2,20($sp)
	lw	$31,60($sp)
	addiu	$sp,$sp,64
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	allocate_skb_buffer
	.size	allocate_skb_buffer, .-allocate_skb_buffer
	.section	.text.qdma_prepare_rx_buffer,"ax",@progbits
	.align	2
	.globl	qdma_prepare_rx_buffer
	.set	nomips16
	.set	nomicromips
	.ent	qdma_prepare_rx_buffer
	.type	qdma_prepare_rx_buffer, @function
qdma_prepare_rx_buffer:
	.frame	$sp,32,$31		# vars= 8, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-32
	sw	$31,28($sp)
	sw	$4,32($sp)
	sw	$0,16($sp)
	sw	$0,20($sp)
	lui	$2,%hi(allocate_skb_buffer)
	addiu	$2,$2,%lo(allocate_skb_buffer)
	jalr	$2
	nop

	sw	$2,20($sp)
	lw	$2,20($sp)
	bne	$2,$0,$L51
	nop

	j	$L52
	nop

$L51:
	lw	$4,20($sp)
	lw	$5,32($sp)
	lui	$2,%hi(qdma_bm_hook_receive_buffer)
	addiu	$2,$2,%lo(qdma_bm_hook_receive_buffer)
	jalr	$2
	nop

	sw	$2,16($sp)
	lw	$2,16($sp)
	beq	$2,$0,$L53
	nop

	li	$2,-19			# 0xffffffffffffffed
	sw	$2,16($sp)
	j	$L52
	nop

$L53:
	lw	$2,16($sp)
	j	$L54
	nop

$L52:
	lw	$2,20($sp)
	beq	$2,$0,$L55
	nop

	lw	$4,20($sp)
	lui	$2,%hi(dev_kfree_skb_any)
	addiu	$2,$2,%lo(dev_kfree_skb_any)
	jalr	$2
	nop

$L55:
	lw	$2,16($sp)
$L54:
	lw	$31,28($sp)
	addiu	$sp,$sp,32
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	qdma_prepare_rx_buffer
	.size	qdma_prepare_rx_buffer, .-qdma_prepare_rx_buffer
	.section	.imem_text
	.align	2
	.globl	qdma_bm_receive_packets
	.set	nomips16
	.set	nomicromips
	.ent	qdma_bm_receive_packets
	.type	qdma_bm_receive_packets, @function
qdma_bm_receive_packets:
	.frame	$sp,96,$31		# vars= 72, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-96
	sw	$31,92($sp)
	sw	$4,96($sp)
	sw	$5,100($sp)
	sw	$0,28($sp)
	sw	$0,32($sp)
	lw	$2,96($sp)
	sw	$2,16($sp)
	sw	$0,20($sp)
	sw	$0,36($sp)
	sw	$0,24($sp)
	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lw	$2,0($2)
	sw	$2,40($sp)
	sw	$0,44($sp)
$L78:
	lui	$2,%hi(gpQdmaPriv)
	lw	$3,%lo(gpQdmaPriv)($2)
	lw	$2,100($sp)
	addiu	$2,$2,24
	sll	$2,$2,2
	addu	$2,$3,$2
	lw	$2,0($2)
	beq	$2,$0,$L57
	nop

	lui	$2,%hi(gpQdmaPriv)
	lw	$3,%lo(gpQdmaPriv)($2)
	lw	$2,100($sp)
	addiu	$2,$2,24
	sll	$2,$2,2
	addu	$2,$3,$2
	lw	$3,0($2)
	lui	$2,%hi(gpQdmaPriv)
	lw	$4,%lo(gpQdmaPriv)($2)
	lw	$2,100($sp)
	addiu	$2,$2,26
	sll	$2,$2,2
	addu	$2,$4,$2
	lw	$2,0($2)
	beq	$3,$2,$L57
	nop

	lui	$2,%hi(gpQdmaPriv)
	lw	$3,%lo(gpQdmaPriv)($2)
	lw	$2,100($sp)
	addiu	$2,$2,24
	sll	$2,$2,2
	addu	$2,$3,$2
	lw	$2,0($2)
	lw	$2,0($2)
	lw	$3,4($2)
	li	$2,-2147483648			# 0xffffffff80000000
	and	$2,$3,$2
	bne	$2,$0,$L58
	nop

$L57:
	lw	$2,20($sp)
	j	$L79
	nop

$L58:
	lui	$2,%hi(gpQdmaPriv)
	lw	$3,%lo(gpQdmaPriv)($2)
	lw	$2,100($sp)
	addiu	$2,$2,24
	sll	$2,$2,2
	addu	$2,$3,$2
	lw	$2,0($2)
	lw	$2,0($2)
	addiu	$3,$sp,52
	move	$4,$3
	move	$5,$2
	li	$6,32			# 0x20
	lui	$2,%hi(memcpy)
	addiu	$2,$2,%lo(memcpy)
	jalr	$2
	nop

	lui	$2,%hi(gpQdmaPriv)
	lw	$3,%lo(gpQdmaPriv)($2)
	lw	$2,100($sp)
	addiu	$2,$2,24
	sll	$2,$2,2
	addu	$2,$3,$2
	lw	$2,0($2)
	sw	$2,28($sp)
	lui	$2,%hi(gpQdmaPriv)
	lw	$4,%lo(gpQdmaPriv)($2)
	lui	$2,%hi(gpQdmaPriv)
	lw	$3,%lo(gpQdmaPriv)($2)
	lw	$2,100($sp)
	addiu	$2,$2,24
	sll	$2,$2,2
	addu	$2,$3,$2
	lw	$2,0($2)
	lw	$3,12($2)
	lw	$2,100($sp)
	addiu	$2,$2,24
	sll	$2,$2,2
	addu	$2,$4,$2
	sw	$3,0($2)
	lw	$2,20($sp)
	addiu	$2,$2,1
	sw	$2,20($sp)
	lw	$2,60($sp)
	beq	$2,$0,$L60
	nop

	lhu	$2,58($sp)
	bne	$2,$0,$L61
	nop

$L60:
	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lw	$3,100($sp)
	addiu	$3,$3,112
	sll	$3,$3,1
	addu	$3,$2,$3
	lhu	$3,4($3)
	addiu	$3,$3,1
	andi	$3,$3,0xffff
	lw	$4,100($sp)
	addiu	$4,$4,112
	sll	$4,$4,1
	addu	$2,$2,$4
	sh	$3,4($2)
	j	$L62
	nop

$L61:
	lw	$3,56($sp)
	li	$2,1073741824			# 0x40000000
	and	$2,$3,$2
	beq	$2,$0,$L63
	nop

	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lw	$3,100($sp)
	addiu	$3,$3,112
	sll	$3,$3,1
	addu	$3,$2,$3
	lhu	$3,4($3)
	addiu	$3,$3,1
	andi	$3,$3,0xffff
	lw	$4,100($sp)
	addiu	$4,$4,112
	sll	$4,$4,1
	addu	$2,$2,$4
	sh	$3,4($2)
	lw	$2,28($sp)
	lw	$2,8($2)
	sw	$2,24($sp)
	j	$L64
	nop

$L63:
	lw	$2,60($sp)
	lhu	$3,58($sp)
	move	$4,$0
	move	$5,$2
	move	$6,$3
	li	$7,2			# 0x2
	lui	$2,%hi(dma_unmap_single)
	addiu	$2,$2,%lo(dma_unmap_single)
	jalr	$2
	nop

	lui	$2,%hi(allocate_skb_buffer)
	addiu	$2,$2,%lo(allocate_skb_buffer)
	jalr	$2
	nop

	sw	$2,24($sp)
	lw	$2,24($sp)
	bne	$2,$0,$L65
	nop

	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lw	$3,100($sp)
	addiu	$3,$3,120
	sll	$3,$3,1
	addu	$3,$2,$3
	lhu	$3,8($3)
	addiu	$3,$3,1
	andi	$3,$3,0xffff
	lw	$4,100($sp)
	addiu	$4,$4,120
	sll	$4,$4,1
	addu	$2,$2,$4
	sh	$3,8($2)
	lw	$2,28($sp)
	lw	$2,8($2)
	sw	$2,24($sp)
	j	$L64
	nop

$L65:
	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lw	$2,148($2)
	sw	$2,44($sp)
	lw	$2,44($sp)
	beq	$2,$0,$L66
	nop

	lw	$2,28($sp)
	lw	$3,8($2)
	lhu	$2,58($sp)
	move	$7,$2
	addiu	$2,$sp,52
	addiu	$4,$2,16
	lw	$2,44($sp)
	li	$5,16			# 0x10
	move	$6,$3
	jalr	$2
	nop

	sw	$2,36($sp)
	lw	$2,36($sp)
	bgez	$2,$L67
	nop

	lw	$3,36($sp)
	li	$2,-30			# 0xffffffffffffffe2
	bne	$3,$2,$L68
	nop

	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lw	$3,100($sp)
	addiu	$3,$3,136
	sll	$3,$3,1
	addu	$3,$2,$3
	lhu	$3,4($3)
	addiu	$3,$3,1
	andi	$3,$3,0xffff
	lw	$4,100($sp)
	addiu	$4,$4,136
	sll	$4,$4,1
	addu	$2,$2,$4
	sh	$3,4($2)
	j	$L69
	nop

$L68:
	lw	$3,36($sp)
	li	$2,-46			# 0xffffffffffffffd2
	bne	$3,$2,$L70
	nop

	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lw	$3,100($sp)
	addiu	$3,$3,140
	sll	$3,$3,1
	addu	$3,$2,$3
	lhu	$3,4($3)
	addiu	$3,$3,1
	andi	$3,$3,0xffff
	lw	$4,100($sp)
	addiu	$4,$4,140
	sll	$4,$4,1
	addu	$2,$2,$4
	sh	$3,4($2)
	j	$L69
	nop

$L70:
	lw	$3,36($sp)
	li	$2,-62			# 0xffffffffffffffc2
	bne	$3,$2,$L69
	nop

	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lw	$3,100($sp)
	addiu	$3,$3,136
	sll	$3,$3,1
	addu	$3,$2,$3
	lhu	$3,8($3)
	addiu	$3,$3,1
	andi	$3,$3,0xffff
	lw	$4,100($sp)
	addiu	$4,$4,136
	sll	$4,$4,1
	addu	$2,$2,$4
	sh	$3,8($2)
$L69:
	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lw	$3,100($sp)
	addiu	$3,$3,112
	sll	$3,$3,1
	addu	$3,$2,$3
	lhu	$3,8($3)
	addiu	$3,$3,1
	andi	$3,$3,0xffff
	lw	$4,100($sp)
	addiu	$4,$4,112
	sll	$4,$4,1
	addu	$2,$2,$4
	sh	$3,8($2)
	j	$L64
	nop

$L67:
	lw	$3,36($sp)
	li	$2,16			# 0x10
	bne	$3,$2,$L72
	nop

	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lw	$3,100($sp)
	addiu	$3,$3,136
	sll	$3,$3,1
	addu	$3,$2,$3
	lhu	$3,4($3)
	addiu	$3,$3,1
	andi	$3,$3,0xffff
	lw	$4,100($sp)
	addiu	$4,$4,136
	sll	$4,$4,1
	addu	$2,$2,$4
	sh	$3,4($2)
	j	$L73
	nop

$L72:
	lw	$3,36($sp)
	li	$2,32			# 0x20
	bne	$3,$2,$L74
	nop

	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lw	$3,100($sp)
	addiu	$3,$3,140
	sll	$3,$3,1
	addu	$3,$2,$3
	lhu	$3,4($3)
	addiu	$3,$3,1
	andi	$3,$3,0xffff
	lw	$4,100($sp)
	addiu	$4,$4,140
	sll	$4,$4,1
	addu	$2,$2,$4
	sh	$3,4($2)
	j	$L73
	nop

$L74:
	lw	$3,36($sp)
	li	$2,48			# 0x30
	bne	$3,$2,$L73
	nop

	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lw	$3,100($sp)
	addiu	$3,$3,136
	sll	$3,$3,1
	addu	$3,$2,$3
	lhu	$3,8($3)
	addiu	$3,$3,1
	andi	$3,$3,0xffff
	lw	$4,100($sp)
	addiu	$4,$4,136
	sll	$4,$4,1
	addu	$2,$2,$4
	sh	$3,8($2)
$L73:
	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lw	$3,100($sp)
	addiu	$3,$3,52
	sll	$3,$3,2
	addu	$3,$2,$3
	lw	$3,4($3)
	addiu	$3,$3,1
	lw	$4,100($sp)
	addiu	$4,$4,52
	sll	$4,$4,2
	addu	$2,$2,$4
	sw	$3,4($2)
	j	$L64
	nop

$L66:
	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lw	$3,100($sp)
	addiu	$3,$3,116
	sll	$3,$3,1
	addu	$3,$2,$3
	lhu	$3,8($3)
	addiu	$3,$3,1
	andi	$3,$3,0xffff
	lw	$4,100($sp)
	addiu	$4,$4,116
	sll	$4,$4,1
	addu	$2,$2,$4
	sh	$3,8($2)
	lw	$2,28($sp)
	lw	$2,8($2)
	move	$4,$2
	lui	$2,%hi(dev_kfree_skb_any)
	addiu	$2,$2,%lo(dev_kfree_skb_any)
	jalr	$2
	nop

$L64:
	lw	$2,100($sp)
	sw	$2,48($sp)
	lui	$2,%hi(gpQdmaPriv)
	lw	$3,%lo(gpQdmaPriv)($2)
	lw	$2,48($sp)
	addiu	$2,$2,26
	sll	$2,$2,2
	addu	$2,$3,$2
	lw	$2,0($2)
	lw	$3,12($2)
	lui	$2,%hi(gpQdmaPriv)
	lw	$4,%lo(gpQdmaPriv)($2)
	lw	$2,48($sp)
	addiu	$2,$2,24
	sll	$2,$2,2
	addu	$2,$4,$2
	lw	$2,0($2)
	xor	$2,$3,$2
	sltu	$2,$0,$2
	andi	$2,$2,0x00ff
	beq	$2,$0,$L76
	nop

	lw	$4,24($sp)
	lw	$5,100($sp)
	lui	$2,%hi(qdma_bm_hook_receive_buffer)
	addiu	$2,$2,%lo(qdma_bm_hook_receive_buffer)
	jalr	$2
	nop

	j	$L77
	nop

$L76:
	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lw	$3,100($sp)
	addiu	$3,$3,120
	sll	$3,$3,1
	addu	$3,$2,$3
	lhu	$3,4($3)
	addiu	$3,$3,1
	andi	$3,$3,0xffff
	lw	$4,100($sp)
	addiu	$4,$4,120
	sll	$4,$4,1
	addu	$2,$2,$4
	sh	$3,4($2)
	lw	$4,24($sp)
	lui	$2,%hi(dev_kfree_skb_any)
	addiu	$2,$2,%lo(dev_kfree_skb_any)
	jalr	$2
	nop

$L77:
	lw	$2,96($sp)
	beq	$2,$0,$L78
	nop

	lw	$2,16($sp)
	addiu	$2,$2,-1
	sw	$2,16($sp)
	lw	$2,16($sp)
	bne	$2,$0,$L78
	nop

$L62:
	lw	$2,20($sp)
$L79:
	lw	$31,92($sp)
	addiu	$sp,$sp,96
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	qdma_bm_receive_packets
	.size	qdma_bm_receive_packets, .-qdma_bm_receive_packets
	.align	2
	.globl	qdma_bm_hook_receive_buffer
	.set	nomips16
	.set	nomicromips
	.ent	qdma_bm_hook_receive_buffer
	.type	qdma_bm_hook_receive_buffer, @function
qdma_bm_hook_receive_buffer:
	.frame	$sp,104,$31		# vars= 80, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	addiu	$sp,$sp,-104
	sw	$31,100($sp)
	sw	$4,104($sp)
	sw	$5,108($sp)
	sw	$0,20($sp)
	sw	$0,24($sp)
	sw	$0,28($sp)
	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lw	$2,0($2)
	sw	$2,32($sp)
	sw	$0,36($sp)
	sw	$0,16($sp)
	sw	$0,40($sp)
	lw	$2,104($sp)
	bne	$2,$0,$L81
	li	$2,-14			# 0xfffffffffffffff2
	j	$L82
$L81:
	lw	$2,108($sp)
	sw	$2,48($sp)
	sw	$0,52($sp)
	sw	$0,56($sp)
	lui	$2,%hi(gpQdmaPriv)
	lw	$3,%lo(gpQdmaPriv)($2)
	lw	$2,48($sp)
	addiu	$2,$2,24
	sll	$2,$2,2
	addu	$2,$3,$2
	lw	$2,0($2)
	beq	$2,$0,$L83
	lui	$2,%hi(gpQdmaPriv)
	lw	$3,%lo(gpQdmaPriv)($2)
	lw	$2,48($sp)
	addiu	$2,$2,26
	sll	$2,$2,2
	addu	$2,$3,$2
	lw	$2,0($2)
	bne	$2,$0,$L84
	lui	$2,%hi(gpQdmaPriv)
	lw	$3,%lo(gpQdmaPriv)($2)
	lw	$2,48($sp)
	addiu	$2,$2,24
	sll	$2,$2,2
	addu	$2,$3,$2
	lw	$2,0($2)
	sw	$2,52($sp)
	lui	$2,%hi(gpQdmaPriv)
	lw	$3,%lo(gpQdmaPriv)($2)
	lw	$2,48($sp)
	addiu	$2,$2,26
	sll	$2,$2,2
	addu	$2,$3,$2
	lw	$3,52($sp)
	sw	$3,0($2)
	j	$L83
$L84:
	lui	$2,%hi(gpQdmaPriv)
	lw	$3,%lo(gpQdmaPriv)($2)
	lw	$2,48($sp)
	addiu	$2,$2,26
	sll	$2,$2,2
	addu	$2,$3,$2
	lw	$2,0($2)
	lw	$3,12($2)
	lui	$2,%hi(gpQdmaPriv)
	lw	$4,%lo(gpQdmaPriv)($2)
	lw	$2,48($sp)
	addiu	$2,$2,24
	sll	$2,$2,2
	addu	$2,$4,$2
	lw	$2,0($2)
	beq	$3,$2,$L83
	lui	$2,%hi(gpQdmaPriv)
	lw	$3,%lo(gpQdmaPriv)($2)
	lw	$2,48($sp)
	addiu	$2,$2,26
	sll	$2,$2,2
	addu	$2,$3,$2
	lw	$2,0($2)
	lw	$2,12($2)
	sw	$2,52($sp)
	lui	$2,%hi(gpQdmaPriv)
	lw	$3,%lo(gpQdmaPriv)($2)
	lw	$2,48($sp)
	addiu	$2,$2,26
	sll	$2,$2,2
	addu	$2,$3,$2
	lw	$3,52($sp)
	sw	$3,0($2)
$L83:
	lw	$2,52($sp)
	sw	$2,20($sp)
	lw	$2,20($sp)
	bne	$2,$0,$L86
	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lw	$3,108($sp)
	addiu	$3,$3,120
	sll	$3,$3,1
	addu	$3,$2,$3
	lhu	$3,4($3)
	addiu	$3,$3,1
	andi	$3,$3,0xffff
	lw	$4,108($sp)
	addiu	$4,$4,120
	sll	$4,$4,1
	addu	$2,$2,$4
	sh	$3,4($2)
	li	$2,-12			# 0xfffffffffffffff4
	j	$L82
$L86:
	lw	$2,104($sp)
	lw	$2,244($2)
	andi	$2,$2,0x3
	bne	$2,$0,$L87
	lw	$2,104($sp)
	sw	$2,60($sp)
	li	$2,2			# 0x2
	sw	$2,64($sp)
	lw	$2,60($sp)
	lw	$3,256($2)
	lw	$2,64($sp)
	addu	$3,$3,$2
	lw	$2,60($sp)
	sw	$3,256($2)
	lw	$2,60($sp)
	lw	$3,244($2)
	lw	$2,64($sp)
	addu	$3,$3,$2
	lw	$2,60($sp)
	sw	$3,244($2)
$L87:
	lw	$2,104($sp)
	lw	$2,244($2)
	addiu	$2,$2,-2
	move	$5,$2
	lw	$2,104($sp)
	sw	$2,68($sp)
	lw	$2,68($sp)
	sw	$2,72($sp)
	lw	$2,72($sp)
	lw	$2,84($2)
	bne	$2,$0,$L89
	lw	$2,68($sp)
	lw	$2,248($2)
	move	$3,$2
	lw	$2,68($sp)
	lw	$2,244($2)
	subu	$2,$3,$2
	j	$L91
$L89:
	move	$2,$0
$L91:
	move	$4,$0
	move	$6,$2
	li	$7,2			# 0x2
	lui	$2,%hi(dma_map_single)
	addiu	$2,$2,%lo(dma_map_single)
	jalr	$2
	sw	$2,28($sp)
	lui	$2,%hi(gpQdmaPriv)
	lw	$3,%lo(gpQdmaPriv)($2)
	lw	$2,108($sp)
	addiu	$2,$2,28
	sll	$2,$2,2
	addu	$2,$3,$2
	lw	$2,0($2)
	lw	$2,0($2)
	move	$3,$2
	li	$2,-1610678272			# 0xffffffff9fff0000
	ori	$2,$2,0xffff
	and	$2,$3,$2
	sw	$2,24($sp)
	sw	$0,16($sp)
	j	$L92
$L93:
	lw	$3,24($sp)
	lw	$2,16($sp)
	addiu	$2,$2,4
	sll	$2,$2,2
	addu	$2,$3,$2
	sw	$0,0($2)
	lw	$2,16($sp)
	addiu	$2,$2,1
	sw	$2,16($sp)
$L92:
	lw	$2,16($sp)
	slt	$2,$2,4
	bne	$2,$0,$L93
	lw	$2,24($sp)
	lw	$3,28($sp)
	sw	$3,8($2)
	lw	$2,20($sp)
	lw	$2,4($2)
	andi	$2,$2,0xffff
	andi	$2,$2,0xfff
	andi	$4,$2,0xffff
	lw	$3,24($sp)
	lw	$2,12($3)
	ins	$2,$4,0,12
	sw	$2,12($3)
	lw	$2,104($sp)
	sw	$2,76($sp)
	lw	$2,76($sp)
	sw	$2,80($sp)
	lw	$2,80($sp)
	lw	$2,84($2)
	bne	$2,$0,$L95
	lw	$2,76($sp)
	lw	$2,248($2)
	move	$3,$2
	lw	$2,76($sp)
	lw	$2,244($2)
	subu	$2,$3,$2
	j	$L97
$L95:
	move	$2,$0
$L97:
	andi	$3,$2,0xffff
	lw	$2,24($sp)
	sh	$3,6($2)
	lw	$3,24($sp)
	lw	$2,4($3)
	ins	$2,$0,31,1
	sw	$2,4($3)
	lw	$2,24($sp)
#APP
 # 793 "/opt/tclinux_phoenix/modules/private/qdma/EN7512/qdma_bmgr.c" 1
		.set	push			
	.set	noreorder		
	.set	mips3			
1:	cache	21, ($2)		
2:	.set	pop			
	.section __ex_table,"a"	
	.word 1b, 2b		
	.previous
 # 0 "" 2
#NO_APP
	lui	$2,%hi(gpQdmaPriv)
	lw	$3,%lo(gpQdmaPriv)($2)
	lw	$2,108($sp)
	addiu	$2,$2,28
	sll	$2,$2,2
	addu	$2,$3,$2
	lw	$2,0($2)
	lw	$3,104($sp)
	sw	$3,8($2)
	lui	$2,%hi(gpQdmaPriv)
	lw	$3,%lo(gpQdmaPriv)($2)
	lw	$2,108($sp)
	addiu	$2,$2,28
	sll	$2,$2,2
	addu	$2,$3,$2
	lw	$3,20($sp)
	sw	$3,0($2)
#APP
 # 797 "/opt/tclinux_phoenix/modules/private/qdma/EN7512/qdma_bmgr.c" 1
	.set	push
	.set	noreorder
	.set	mips2
	sync
	.set	pop
 # 0 "" 2
#NO_APP
	lw	$2,108($sp)
	sll	$2,$2,8
	move	$3,$2
	lw	$2,32($sp)
	addu	$2,$3,$2
	addiu	$2,$2,24
	sw	$2,84($sp)
	lw	$2,84($sp)
	lw	$2,0($2)
	sw	$2,44($sp)
	lw	$2,108($sp)
	sll	$2,$2,8
	move	$3,$2
	lw	$2,32($sp)
	addu	$2,$3,$2
	addiu	$3,$2,24
	lw	$4,44($sp)
	li	$2,-4096			# 0xfffffffffffff000
	and	$4,$4,$2
	lw	$2,20($sp)
	lw	$2,4($2)
	andi	$2,$2,0xfff
	or	$2,$4,$2
	sw	$3,88($sp)
	sw	$2,92($sp)
	lw	$2,88($sp)
	lw	$3,92($sp)
	sw	$3,0($2)
	lw	$2,40($sp)
$L82:
	lw	$31,100($sp)
	addiu	$sp,$sp,104
	j	$31
	.end	qdma_bm_hook_receive_buffer
	.size	qdma_bm_hook_receive_buffer, .-qdma_bm_hook_receive_buffer
	.section	.text.qdma_bm_recycle_receive_buffer,"ax",@progbits
	.align	2
	.globl	qdma_bm_recycle_receive_buffer
	.set	nomips16
	.set	nomicromips
	.ent	qdma_bm_recycle_receive_buffer
	.type	qdma_bm_recycle_receive_buffer, @function
qdma_bm_recycle_receive_buffer:
	.frame	$sp,80,$31		# vars= 56, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-80
	sw	$31,76($sp)
	sw	$4,80($sp)
	sw	$0,16($sp)
	sw	$0,20($sp)
	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lw	$2,0($2)
	sw	$2,24($sp)
	sw	$0,28($sp)
	lui	$2,%hi(gpQdmaPriv)
	lw	$3,%lo(gpQdmaPriv)($2)
	lw	$2,80($sp)
	addiu	$2,$2,24
	sll	$2,$2,2
	addu	$2,$3,$2
	lw	$2,0($2)
	beq	$2,$0,$L100
	nop

	lui	$2,%hi(gpQdmaPriv)
	lw	$3,%lo(gpQdmaPriv)($2)
	lw	$2,80($sp)
	addiu	$2,$2,26
	sll	$2,$2,2
	addu	$2,$3,$2
	lw	$2,0($2)
	bne	$2,$0,$L101
	nop

$L100:
	li	$2,-14			# 0xfffffffffffffff2
	j	$L102
	nop

$L101:
	j	$L103
	nop

$L110:
	lui	$2,%hi(gpQdmaPriv)
	lw	$3,%lo(gpQdmaPriv)($2)
	lw	$2,80($sp)
	addiu	$2,$2,24
	sll	$2,$2,2
	addu	$2,$3,$2
	lw	$2,0($2)
	sw	$2,20($sp)
	lui	$2,%hi(gpQdmaPriv)
	lw	$3,%lo(gpQdmaPriv)($2)
	lw	$2,80($sp)
	addiu	$2,$2,24
	sll	$2,$2,2
	addu	$2,$3,$2
	lw	$2,0($2)
	lw	$2,0($2)
	sw	$2,16($sp)
	lw	$2,20($sp)
	lw	$2,8($2)
	beq	$2,$0,$L104
	nop

	lw	$2,16($sp)
	lw	$2,8($2)
	beq	$2,$0,$L104
	nop

	lw	$2,16($sp)
	lw	$3,8($2)
	lw	$2,20($sp)
	lw	$2,8($2)
	sw	$2,40($sp)
	lw	$2,40($sp)
	sw	$2,44($sp)
	lw	$2,44($sp)
	lw	$2,84($2)
	bne	$2,$0,$L106
	nop

	lw	$2,40($sp)
	lw	$2,248($2)
	move	$4,$2
	lw	$2,40($sp)
	lw	$2,244($2)
	subu	$2,$4,$2
	j	$L108
	nop

$L106:
	move	$2,$0
$L108:
	move	$4,$0
	move	$5,$3
	move	$6,$2
	li	$7,2			# 0x2
	lui	$2,%hi(dma_unmap_single)
	addiu	$2,$2,%lo(dma_unmap_single)
	jalr	$2
	nop

	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lw	$2,148($2)
	beq	$2,$0,$L109
	nop

	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lw	$2,148($2)
	lw	$3,16($sp)
	addiu	$4,$3,16
	lw	$3,20($sp)
	lw	$6,8($3)
	lw	$3,16($sp)
	lhu	$3,6($3)
	li	$5,16			# 0x10
	move	$7,$3
	jalr	$2
	nop

	j	$L104
	nop

$L109:
	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lw	$3,80($sp)
	addiu	$3,$3,116
	sll	$3,$3,1
	addu	$3,$2,$3
	lhu	$3,8($3)
	addiu	$3,$3,1
	andi	$3,$3,0xffff
	lw	$4,80($sp)
	addiu	$4,$4,116
	sll	$4,$4,1
	addu	$2,$2,$4
	sh	$3,8($2)
	lw	$2,20($sp)
	lw	$2,8($2)
	move	$4,$2
	lui	$2,%hi(dev_kfree_skb_any)
	addiu	$2,$2,%lo(dev_kfree_skb_any)
	jalr	$2
	nop

$L104:
	lui	$2,%hi(gpQdmaPriv)
	lw	$4,%lo(gpQdmaPriv)($2)
	lw	$2,20($sp)
	lw	$3,12($2)
	lw	$2,80($sp)
	addiu	$2,$2,24
	sll	$2,$2,2
	addu	$2,$4,$2
	sw	$3,0($2)
$L103:
	lui	$2,%hi(gpQdmaPriv)
	lw	$3,%lo(gpQdmaPriv)($2)
	lw	$2,80($sp)
	addiu	$2,$2,24
	sll	$2,$2,2
	addu	$2,$3,$2
	lw	$3,0($2)
	lui	$2,%hi(gpQdmaPriv)
	lw	$4,%lo(gpQdmaPriv)($2)
	lw	$2,80($sp)
	addiu	$2,$2,26
	sll	$2,$2,2
	addu	$2,$4,$2
	lw	$2,0($2)
	bne	$3,$2,$L110
	nop

	lui	$2,%hi(gpQdmaPriv)
	lw	$3,%lo(gpQdmaPriv)($2)
	lw	$2,80($sp)
	addiu	$2,$2,24
	sll	$2,$2,2
	addu	$2,$3,$2
	lw	$2,0($2)
	sw	$2,20($sp)
	lui	$2,%hi(gpQdmaPriv)
	lw	$3,%lo(gpQdmaPriv)($2)
	lw	$2,80($sp)
	addiu	$2,$2,28
	sll	$2,$2,2
	addu	$2,$3,$2
	lw	$3,20($sp)
	sw	$3,0($2)
	lw	$2,80($sp)
	sll	$2,$2,8
	move	$3,$2
	lw	$2,24($sp)
	addu	$2,$3,$2
	addiu	$2,$2,24
	sw	$2,48($sp)
	lw	$2,48($sp)
	lw	$2,0($2)
	sw	$2,32($sp)
	lw	$2,80($sp)
	sll	$2,$2,8
	move	$3,$2
	lw	$2,24($sp)
	addu	$2,$3,$2
	addiu	$3,$2,24
	lw	$4,32($sp)
	li	$2,-4096			# 0xfffffffffffff000
	and	$4,$4,$2
	lw	$2,20($sp)
	lw	$2,4($2)
	andi	$2,$2,0xfff
	or	$2,$4,$2
	sw	$3,52($sp)
	sw	$2,56($sp)
	lw	$2,52($sp)
	lw	$3,56($sp)
	sw	$3,0($2)
	lw	$2,80($sp)
	sll	$2,$2,8
	move	$3,$2
	lw	$2,24($sp)
	addu	$2,$3,$2
	addiu	$2,$2,28
	sw	$2,60($sp)
	lw	$2,60($sp)
	lw	$2,0($2)
	sw	$2,36($sp)
	lw	$2,80($sp)
	sll	$2,$2,8
	move	$3,$2
	lw	$2,24($sp)
	addu	$2,$3,$2
	addiu	$3,$2,28
	lw	$4,36($sp)
	li	$2,-4096			# 0xfffffffffffff000
	and	$4,$4,$2
	lw	$2,20($sp)
	lw	$2,4($2)
	andi	$2,$2,0xfff
	or	$2,$4,$2
	sw	$3,64($sp)
	sw	$2,68($sp)
	lw	$2,64($sp)
	lw	$3,68($sp)
	sw	$3,0($2)
	move	$2,$0
$L102:
	lw	$31,76($sp)
	addiu	$sp,$sp,80
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	qdma_bm_recycle_receive_buffer
	.size	qdma_bm_recycle_receive_buffer, .-qdma_bm_recycle_receive_buffer
	.section	.imem_text
	.align	2
	.globl	qdma_bm_transmit_done
	.set	nomips16
	.set	nomicromips
	.ent	qdma_bm_transmit_done
	.type	qdma_bm_transmit_done, @function
qdma_bm_transmit_done:
	.frame	$sp,256,$31		# vars= 232, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	addiu	$sp,$sp,-256
	sw	$31,252($sp)
	sw	$4,256($sp)
	sw	$0,40($sp)
	sw	$0,44($sp)
	sw	$0,16($sp)
	sw	$0,20($sp)
	sw	$0,48($sp)
	sw	$0,52($sp)
	sw	$0,24($sp)
	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lw	$2,0($2)
	sw	$2,56($sp)
	sw	$0,60($sp)
	sw	$0,64($sp)
	sw	$0,28($sp)
	li	$2,5			# 0x5
	sw	$2,32($sp)
	sw	$0,68($sp)
	sw	$0,72($sp)
	sw	$0,36($sp)
	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lw	$3,140($2)
	li	$2,-2147483648			# 0xffffffff80000000
	and	$2,$3,$2
	beq	$2,$0,$L114
	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	addiu	$2,$2,16
	sw	$2,84($sp)
	lw	$2,84($sp)
	move	$4,$2
	lui	$2,%hi(_raw_spin_lock_irqsave)
	addiu	$2,$2,%lo(_raw_spin_lock_irqsave)
	jalr	$2
	sw	$2,36($sp)
$L114:
	lw	$2,56($sp)
	addiu	$2,$2,108
	sw	$2,88($sp)
	lw	$2,88($sp)
	lw	$2,0($2)
	sw	$2,68($sp)
	lw	$2,68($sp)
	andi	$2,$2,0xfff
	sw	$2,64($sp)
	lw	$3,68($sp)
	li	$2,268369920			# 0xfff0000
	and	$2,$3,$2
	srl	$2,$2,16
	sw	$2,60($sp)
	lw	$2,60($sp)
	bne	$2,$0,$L117
	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lhu	$3,182($2)
	addiu	$3,$3,1
	andi	$3,$3,0xffff
	sh	$3,182($2)
	j	$L118
$L117:
	lw	$2,256($sp)
	beq	$2,$0,$L119
	lw	$3,256($sp)
	lw	$2,60($sp)
	sltu	$2,$3,$2
	beq	$2,$0,$L119
	lw	$2,256($sp)
	j	$L120
$L119:
	lw	$2,60($sp)
$L120:
	sw	$2,60($sp)
	sw	$0,16($sp)
	j	$L121
$L142:
	lw	$3,16($sp)
	lw	$2,64($sp)
	addu	$2,$3,$2
	andi	$2,$2,0xff
	sll	$3,$2,2
	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lw	$2,56($2)
	addu	$2,$3,$2
	sw	$2,72($sp)
	li	$2,5			# 0x5
	sw	$2,32($sp)
	j	$L122
$L126:
	lw	$2,72($sp)
	lw	$2,0($2)
	sw	$2,28($sp)
	lw	$3,28($sp)
	li	$2,-1			# 0xffffffffffffffff
	bne	$3,$2,$L123
	lw	$2,32($sp)
	bne	$2,$0,$L122
	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lhu	$3,172($2)
	addiu	$3,$3,1
	andi	$3,$3,0xffff
	sh	$3,172($2)
	li	$2,-61			# 0xffffffffffffffc3
	sw	$2,24($sp)
	j	$L124
$L123:
	lw	$2,72($sp)
	li	$3,-1			# 0xffffffffffffffff
	sw	$3,0($2)
	j	$L125
$L122:
	lw	$2,32($sp)
	addiu	$3,$2,-1
	sw	$3,32($sp)
	bne	$2,$0,$L126
$L125:
	lw	$2,28($sp)
	andi	$2,$2,0xfff
	sw	$2,48($sp)
	lw	$3,28($sp)
	li	$2,65536			# 0x10000
	and	$2,$3,$2
	srl	$2,$2,16
	sw	$2,52($sp)
	lw	$2,48($sp)
	bltz	$2,$L127
	lui	$2,%hi(gpQdmaPriv)
	lw	$3,%lo(gpQdmaPriv)($2)
	lw	$2,52($sp)
	addiu	$2,$2,8
	sll	$2,$2,1
	addu	$2,$3,$2
	lhu	$2,4($2)
	move	$3,$2
	lw	$2,48($sp)
	slt	$2,$2,$3
	bne	$2,$0,$L128
$L127:
	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lhu	$3,174($2)
	addiu	$3,$3,1
	andi	$3,$3,0xffff
	sh	$3,174($2)
	li	$2,-14			# 0xfffffffffffffff2
	sw	$2,24($sp)
	j	$L129
$L128:
	lw	$2,48($sp)
	sll	$3,$2,4
	lui	$2,%hi(gpQdmaPriv)
	lw	$4,%lo(gpQdmaPriv)($2)
	lw	$2,52($sp)
	addiu	$2,$2,10
	sll	$2,$2,2
	addu	$2,$4,$2
	lw	$2,0($2)
	addu	$2,$3,$2
	sw	$2,44($sp)
	lw	$2,44($sp)
	lw	$3,4($2)
	lw	$2,48($sp)
	bne	$3,$2,$L130
	lw	$2,44($sp)
	lw	$2,12($2)
	beq	$2,$0,$L131
$L130:
	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lw	$3,52($sp)
	addiu	$3,$3,108
	sll	$3,$3,1
	addu	$3,$2,$3
	lhu	$3,4($3)
	addiu	$3,$3,1
	andi	$3,$3,0xffff
	lw	$4,52($sp)
	addiu	$4,$4,108
	sll	$4,$4,1
	addu	$2,$2,$4
	sh	$3,4($2)
	li	$2,-14			# 0xfffffffffffffff2
	sw	$2,24($sp)
	j	$L129
$L131:
	lw	$2,44($sp)
	lw	$2,0($2)
	sw	$2,40($sp)
	lw	$2,40($sp)
	lw	$3,4($2)
	li	$2,-2147483648			# 0xffffffff80000000
	and	$2,$3,$2
	bne	$2,$0,$L132
	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lw	$3,52($sp)
	addiu	$3,$3,108
	sll	$3,$3,1
	addu	$3,$2,$3
	lhu	$3,8($3)
	addiu	$3,$3,1
	andi	$3,$3,0xffff
	lw	$4,52($sp)
	addiu	$4,$4,108
	sll	$4,$4,1
	addu	$2,$2,$4
	sh	$3,8($2)
	li	$2,-14			# 0xfffffffffffffff2
	sw	$2,24($sp)
	j	$L129
$L132:
	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lw	$3,52($sp)
	addiu	$3,$3,50
	sll	$3,$3,2
	addu	$3,$2,$3
	lw	$3,4($3)
	addiu	$3,$3,1
	lw	$4,52($sp)
	addiu	$4,$4,50
	sll	$4,$4,2
	addu	$2,$2,$4
	sw	$3,4($2)
	lw	$2,44($sp)
	lw	$3,8($2)
	lw	$2,40($sp)
	lw	$2,16($2)
	sw	$3,92($sp)
	sw	$2,96($sp)
	sw	$0,100($sp)
	sw	$0,104($sp)
	sw	$0,108($sp)
	lw	$2,96($sp)
	ext	$2,$2,3,8
	andi	$2,$2,0x00ff
	sw	$2,100($sp)
	lw	$2,96($sp)
	ext	$2,$2,0,3
	andi	$2,$2,0x00ff
	sw	$2,104($sp)
	lw	$2,100($sp)
	sll	$3,$2,2
	lui	$2,%hi(chnlLimit)
	addiu	$2,$2,%lo(chnlLimit)
	addu	$2,$3,$2
	li	$3,1			# 0x1
	sw	$3,112($sp)
	sw	$2,116($sp)
	lw	$4,116($sp)
	lw	$3,112($sp)
	lw	$2,116($sp)
#APP
 # 112 "/opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/atomic.h" 1
		.set	mips3					
1:	ll	$5, 0($4)		# atomic_sub		
	subu	$5, $3					
	sc	$5, 0($4)					
	beqz	$5, 2f					
	.subsection 2					
2:	b	1b					
	.previous					
	.set	mips0					

 # 0 "" 2
#NO_APP
	move	$2,$5
	sw	$2,120($sp)
	lw	$2,100($sp)
	sll	$3,$2,3
	lw	$2,104($sp)
	addu	$2,$3,$2
	sll	$3,$2,2
	lui	$2,%hi(queueLimit)
	addiu	$2,$2,%lo(queueLimit)
	addu	$2,$3,$2
	li	$3,1			# 0x1
	sw	$3,124($sp)
	sw	$2,128($sp)
	lw	$4,128($sp)
	lw	$3,124($sp)
	lw	$2,128($sp)
#APP
 # 112 "/opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/atomic.h" 1
		.set	mips3					
1:	ll	$5, 0($4)		# atomic_sub		
	subu	$5, $3					
	sc	$5, 0($4)					
	beqz	$5, 2f					
	.subsection 2					
2:	b	1b					
	.previous					
	.set	mips0					

 # 0 "" 2
#NO_APP
	move	$2,$5
	sw	$2,132($sp)
	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lhu	$3,188($2)
	addiu	$3,$3,1
	andi	$3,$3,0xffff
	sh	$3,188($2)
	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lw	$2,156($2)
	beq	$2,$0,$L133
	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lw	$2,156($2)
	lw	$4,92($sp)
	lw	$5,96($sp)
	jalr	$2
	beq	$2,$0,$L149
	li	$2,-1			# 0xffffffffffffffff
	sw	$2,108($sp)
	j	$L135
$L133:
	lw	$4,92($sp)
	lui	$2,%hi(dev_kfree_skb_any)
	addiu	$2,$2,%lo(dev_kfree_skb_any)
	jalr	$2
	j	$L135
$L149:
	.set	noreorder
	nop
	.set	reorder
$L135:
	lw	$2,108($sp)
	beq	$2,$0,$L137
	addiu	$2,$sp,200
	move	$4,$2
	lui	$2,%hi(qdma_dump_irq_info)
	addiu	$2,$2,%lo(qdma_dump_irq_info)
	jalr	$2
	li	$2,-14			# 0xfffffffffffffff2
	sw	$2,24($sp)
	j	$L124
$L137:
	lw	$2,40($sp)
	sw	$0,16($2)
	lw	$2,40($sp)
	sw	$0,20($2)
	lui	$2,%hi(gpQdmaPriv)
	lw	$3,%lo(gpQdmaPriv)($2)
	lw	$2,52($sp)
	sll	$2,$2,2
	addu	$2,$3,$2
	addiu	$2,$2,4
	sw	$2,136($sp)
	lw	$2,136($sp)
	move	$4,$2
	lui	$2,%hi(_raw_spin_lock)
	addiu	$2,$2,%lo(_raw_spin_lock)
	jalr	$2
	lw	$2,44($sp)
	sw	$2,140($sp)
	lw	$2,52($sp)
	sw	$2,144($sp)
	lw	$2,140($sp)
	lw	$2,12($2)
	bne	$2,$0,$L139
	lw	$2,140($sp)
	sw	$0,8($2)
	lui	$2,%hi(gpQdmaPriv)
	lw	$3,%lo(gpQdmaPriv)($2)
	lw	$2,144($sp)
	addiu	$2,$2,18
	sll	$2,$2,2
	addu	$2,$3,$2
	lw	$2,0($2)
	bne	$2,$0,$L140
	lui	$2,%hi(gpQdmaPriv)
	lw	$3,%lo(gpQdmaPriv)($2)
	lw	$2,144($sp)
	addiu	$2,$2,18
	sll	$2,$2,2
	addu	$2,$3,$2
	lw	$3,140($sp)
	sw	$3,0($2)
	lui	$2,%hi(gpQdmaPriv)
	lw	$3,%lo(gpQdmaPriv)($2)
	lw	$2,144($sp)
	addiu	$2,$2,20
	sll	$2,$2,2
	addu	$2,$3,$2
	lw	$3,140($sp)
	sw	$3,0($2)
	j	$L139
$L140:
	lui	$2,%hi(gpQdmaPriv)
	lw	$3,%lo(gpQdmaPriv)($2)
	lw	$2,144($sp)
	addiu	$2,$2,20
	sll	$2,$2,2
	addu	$2,$3,$2
	lw	$2,0($2)
	lw	$3,140($sp)
	sw	$3,12($2)
	lui	$2,%hi(gpQdmaPriv)
	lw	$4,%lo(gpQdmaPriv)($2)
	lui	$2,%hi(gpQdmaPriv)
	lw	$3,%lo(gpQdmaPriv)($2)
	lw	$2,144($sp)
	addiu	$2,$2,20
	sll	$2,$2,2
	addu	$2,$3,$2
	lw	$2,0($2)
	lw	$3,12($2)
	lw	$2,144($sp)
	addiu	$2,$2,20
	sll	$2,$2,2
	addu	$2,$4,$2
	sw	$3,0($2)
$L139:
	lui	$2,%hi(gpQdmaPriv)
	lw	$3,%lo(gpQdmaPriv)($2)
	lw	$2,52($sp)
	sll	$2,$2,2
	addu	$2,$3,$2
	addiu	$2,$2,4
	sw	$2,148($sp)
	lw	$2,148($sp)
	sw	$2,152($sp)
	lw	$2,152($sp)
	sw	$2,156($sp)
	lw	$2,156($sp)
	sw	$2,160($sp)
	lw	$2,160($sp)
	lhu	$2,2($2)
	addiu	$2,$2,1
	sw	$2,164($sp)
#APP
 # 139 "/opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/spinlock.h" 1
	.set	push
	.set	noreorder
	.set	mips2
	sync
	.set	pop
 # 0 "" 2
#NO_APP
	lw	$2,164($sp)
	andi	$3,$2,0xffff
	lw	$2,160($sp)
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
$L129:
	lw	$2,16($sp)
	addiu	$2,$2,1
	sw	$2,16($sp)
$L121:
	lw	$3,16($sp)
	lw	$2,60($sp)
	sltu	$2,$3,$2
	bne	$2,$0,$L142
$L124:
	lw	$2,16($sp)
	beq	$2,$0,$L118
	sw	$0,20($sp)
	j	$L143
$L145:
	lw	$2,56($sp)
	addiu	$2,$2,104
	sw	$2,168($sp)
	lw	$2,168($sp)
	lw	$2,0($2)
	sw	$2,76($sp)
	lw	$2,56($sp)
	addiu	$3,$2,104
	lw	$4,76($sp)
	li	$2,-256			# 0xffffffffffffff00
	and	$2,$4,$2
	ori	$2,$2,0x80
	sw	$3,172($sp)
	sw	$2,176($sp)
	lw	$2,172($sp)
	lw	$3,176($sp)
	sw	$3,0($2)
	lw	$2,20($sp)
	addiu	$2,$2,1
	sw	$2,20($sp)
$L143:
	lw	$2,16($sp)
	sra	$3,$2,7
	lw	$2,20($sp)
	slt	$2,$2,$3
	bne	$2,$0,$L145
	lw	$2,56($sp)
	addiu	$2,$2,104
	sw	$2,180($sp)
	lw	$2,180($sp)
	lw	$2,0($2)
	sw	$2,80($sp)
	lw	$2,56($sp)
	addiu	$3,$2,104
	lw	$4,80($sp)
	li	$2,-256			# 0xffffffffffffff00
	and	$4,$4,$2
	lw	$2,16($sp)
	andi	$2,$2,0x7f
	or	$2,$4,$2
	sw	$3,184($sp)
	sw	$2,188($sp)
	lw	$2,184($sp)
	lw	$3,188($sp)
	sw	$3,0($2)
$L118:
	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lw	$3,140($2)
	li	$2,-2147483648			# 0xffffffff80000000
	and	$2,$3,$2
	beq	$2,$0,$L147
	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	addiu	$2,$2,16
	sw	$2,192($sp)
	lw	$2,36($sp)
	sw	$2,196($sp)
	lw	$2,192($sp)
	move	$4,$2
	lw	$5,196($sp)
	lui	$2,%hi(_raw_spin_unlock_irqrestore)
	addiu	$2,$2,%lo(_raw_spin_unlock_irqrestore)
	jalr	$2
$L147:
	lw	$2,24($sp)
	lw	$31,252($sp)
	addiu	$sp,$sp,256
	j	$31
	.end	qdma_bm_transmit_done
	.size	qdma_bm_transmit_done, .-qdma_bm_transmit_done
	.align	2
	.globl	qdma_bm_transmit_packet
	.set	nomips16
	.set	nomicromips
	.ent	qdma_bm_transmit_packet
	.type	qdma_bm_transmit_packet, @function
qdma_bm_transmit_packet:
	.frame	$sp,160,$31		# vars= 136, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	addiu	$sp,$sp,-160
	sw	$31,156($sp)
	sw	$4,160($sp)
	sw	$5,164($sp)
	sw	$6,168($sp)
	sw	$7,172($sp)
	sw	$0,16($sp)
	sw	$0,20($sp)
	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lw	$2,0($2)
	sw	$2,24($sp)
	addiu	$2,$sp,116
	sw	$2,28($sp)
	sw	$0,32($sp)
	sw	$0,36($sp)
	sw	$0,40($sp)
	sw	$0,44($sp)
	lw	$2,28($sp)
	addiu	$3,$sp,168
	sw	$3,0($2)
	lw	$2,28($sp)
	addiu	$3,$sp,172
	sw	$3,4($2)
	lw	$2,160($sp)
	sltu	$2,$2,1
	andi	$2,$2,0x00ff
	bne	$2,$0,$L151
	lw	$2,160($sp)
	lw	$2,80($2)
	sltu	$2,$2,1
	andi	$2,$2,0x00ff
	beq	$2,$0,$L152
$L151:
	li	$2,1			# 0x1
	j	$L153
$L152:
	move	$2,$0
$L153:
	bne	$2,$0,$L154
	lw	$2,160($sp)
	lw	$2,80($2)
	sltu	$2,$2,2049
	xori	$2,$2,0x1
	andi	$2,$2,0x00ff
	beq	$2,$0,$L155
$L154:
	li	$2,-14			# 0xfffffffffffffff2
	j	$L164
$L155:
	lui	$2,%hi(gpQdmaPriv)
	lw	$3,%lo(gpQdmaPriv)($2)
	lw	$2,164($sp)
	sll	$2,$2,2
	addu	$2,$3,$2
	addiu	$2,$2,4
	sw	$2,52($sp)
	lw	$2,52($sp)
	move	$4,$2
	lui	$2,%hi(_raw_spin_lock_irqsave)
	addiu	$2,$2,%lo(_raw_spin_lock_irqsave)
	jalr	$2
	sw	$2,40($sp)
	lw	$2,164($sp)
	sw	$2,56($sp)
	sw	$0,60($sp)
	lui	$2,%hi(gpQdmaPriv)
	lw	$3,%lo(gpQdmaPriv)($2)
	lw	$2,56($sp)
	addiu	$2,$2,18
	sll	$2,$2,2
	addu	$2,$3,$2
	lw	$2,0($2)
	sw	$2,60($sp)
	lui	$2,%hi(gpQdmaPriv)
	lw	$3,%lo(gpQdmaPriv)($2)
	lw	$2,56($sp)
	addiu	$2,$2,18
	sll	$2,$2,2
	addu	$2,$3,$2
	lw	$3,0($2)
	lui	$2,%hi(gpQdmaPriv)
	lw	$4,%lo(gpQdmaPriv)($2)
	lw	$2,56($sp)
	addiu	$2,$2,20
	sll	$2,$2,2
	addu	$2,$4,$2
	lw	$2,0($2)
	bne	$3,$2,$L158
	lui	$2,%hi(gpQdmaPriv)
	lw	$3,%lo(gpQdmaPriv)($2)
	lw	$2,56($sp)
	addiu	$2,$2,18
	sll	$2,$2,2
	addu	$2,$3,$2
	sw	$0,0($2)
	lui	$2,%hi(gpQdmaPriv)
	lw	$3,%lo(gpQdmaPriv)($2)
	lw	$2,56($sp)
	addiu	$2,$2,20
	sll	$2,$2,2
	addu	$2,$3,$2
	sw	$0,0($2)
	j	$L159
$L158:
	lui	$2,%hi(gpQdmaPriv)
	lw	$4,%lo(gpQdmaPriv)($2)
	lui	$2,%hi(gpQdmaPriv)
	lw	$3,%lo(gpQdmaPriv)($2)
	lw	$2,56($sp)
	addiu	$2,$2,18
	sll	$2,$2,2
	addu	$2,$3,$2
	lw	$2,0($2)
	lw	$3,12($2)
	lw	$2,56($sp)
	addiu	$2,$2,18
	sll	$2,$2,2
	addu	$2,$4,$2
	sw	$3,0($2)
$L159:
	lw	$2,60($sp)
	beq	$2,$0,$L160
	lw	$2,60($sp)
	sw	$0,12($2)
$L160:
	lw	$2,60($sp)
	sw	$2,16($sp)
	lw	$2,16($sp)
	sltu	$2,$2,1
	andi	$2,$2,0x00ff
	beq	$2,$0,$L162
	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lw	$3,164($sp)
	addiu	$3,$3,116
	sll	$3,$3,1
	addu	$3,$2,$3
	lhu	$3,4($3)
	addiu	$3,$3,1
	andi	$3,$3,0xffff
	lw	$4,164($sp)
	addiu	$4,$4,116
	sll	$4,$4,1
	addu	$2,$2,$4
	sh	$3,4($2)
	lui	$2,%hi(gpQdmaPriv)
	lw	$3,%lo(gpQdmaPriv)($2)
	lw	$2,164($sp)
	sll	$2,$2,2
	addu	$2,$3,$2
	addiu	$2,$2,4
	sw	$2,64($sp)
	lw	$2,40($sp)
	sw	$2,68($sp)
	lw	$2,64($sp)
	move	$4,$2
	lw	$5,68($sp)
	lui	$2,%hi(_raw_spin_unlock_irqrestore)
	addiu	$2,$2,%lo(_raw_spin_unlock_irqrestore)
	jalr	$2
	li	$2,-63			# 0xffffffffffffffc1
	j	$L164
$L162:
	lw	$2,28($sp)
	lw	$2,0($2)
	lw	$2,0($2)
	ext	$2,$2,3,8
	andi	$2,$2,0x00ff
	sw	$2,32($sp)
	lw	$2,28($sp)
	lw	$2,0($2)
	lw	$2,0($2)
	ext	$2,$2,0,3
	andi	$2,$2,0x00ff
	sw	$2,36($sp)
	lw	$2,32($sp)
	sll	$3,$2,2
	lui	$2,%hi(chnlLimit)
	addiu	$2,$2,%lo(chnlLimit)
	addu	$2,$3,$2
	li	$3,1			# 0x1
	sw	$3,72($sp)
	sw	$2,76($sp)
	lw	$4,76($sp)
	lw	$3,72($sp)
	lw	$2,76($sp)
#APP
 # 67 "/opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/atomic.h" 1
		.set	mips3					
1:	ll	$5, 0($4)		# atomic_add		
	addu	$5, $3					
	sc	$5, 0($4)					
	beqz	$5, 2f					
	.subsection 2					
2:	b	1b					
	.previous					
	.set	mips0					

 # 0 "" 2
#NO_APP
	move	$2,$5
	sw	$2,80($sp)
	lw	$2,32($sp)
	sll	$3,$2,3
	lw	$2,36($sp)
	addu	$2,$3,$2
	sll	$3,$2,2
	lui	$2,%hi(queueLimit)
	addiu	$2,$2,%lo(queueLimit)
	addu	$2,$3,$2
	li	$3,1			# 0x1
	sw	$3,84($sp)
	sw	$2,88($sp)
	lw	$4,88($sp)
	lw	$3,84($sp)
	lw	$2,88($sp)
#APP
 # 67 "/opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/atomic.h" 1
		.set	mips3					
1:	ll	$5, 0($4)		# atomic_add		
	addu	$5, $3					
	sc	$5, 0($4)					
	beqz	$5, 2f					
	.subsection 2					
2:	b	1b					
	.previous					
	.set	mips0					

 # 0 "" 2
#NO_APP
	move	$2,$5
	sw	$2,92($sp)
	lui	$2,%hi(gpQdmaPriv)
	lw	$3,%lo(gpQdmaPriv)($2)
	lw	$2,164($sp)
	addiu	$2,$2,22
	sll	$2,$2,2
	addu	$2,$3,$2
	lw	$2,0($2)
	lw	$2,0($2)
	move	$3,$2
	li	$2,-1610678272			# 0xffffffff9fff0000
	ori	$2,$2,0xffff
	and	$2,$3,$2
	sw	$2,20($sp)
	lw	$3,168($sp)
	lw	$2,20($sp)
	sw	$3,16($2)
	lw	$3,172($sp)
	lw	$2,20($sp)
	sw	$3,20($2)
	lw	$2,16($sp)
	lw	$2,4($2)
	andi	$2,$2,0xffff
	andi	$2,$2,0xfff
	andi	$4,$2,0xffff
	lw	$3,20($sp)
	lw	$2,12($3)
	ins	$2,$4,0,12
	sw	$2,12($3)
	lw	$2,160($sp)
	lw	$3,256($2)
	lw	$2,160($sp)
	lw	$2,80($2)
	move	$4,$0
	move	$5,$3
	move	$6,$2
	li	$7,1			# 0x1
	lui	$2,%hi(dma_map_single)
	addiu	$2,$2,%lo(dma_map_single)
	jalr	$2
	move	$3,$2
	lw	$2,20($sp)
	sw	$3,8($2)
	lw	$2,160($sp)
	lw	$2,80($2)
	andi	$3,$2,0xffff
	lw	$2,20($sp)
	sh	$3,6($2)
	lw	$3,20($sp)
	lw	$2,4($3)
	ins	$2,$0,31,1
	sw	$2,4($3)
	lw	$2,20($sp)
#APP
 # 1064 "/opt/tclinux_phoenix/modules/private/qdma/EN7512/qdma_bmgr.c" 1
		.set	push			
	.set	noreorder		
	.set	mips3			
1:	cache	21, ($2)		
2:	.set	pop			
	.section __ex_table,"a"	
	.word 1b, 2b		
	.previous
 # 0 "" 2
#NO_APP
	lui	$2,%hi(gpQdmaPriv)
	lw	$3,%lo(gpQdmaPriv)($2)
	lw	$2,164($sp)
	addiu	$2,$2,22
	sll	$2,$2,2
	addu	$2,$3,$2
	lw	$2,0($2)
	lw	$3,160($sp)
	sw	$3,8($2)
	lui	$2,%hi(gpQdmaPriv)
	lw	$3,%lo(gpQdmaPriv)($2)
	lw	$2,164($sp)
	addiu	$2,$2,22
	sll	$2,$2,2
	addu	$2,$3,$2
	lw	$3,16($sp)
	sw	$3,0($2)
#APP
 # 1068 "/opt/tclinux_phoenix/modules/private/qdma/EN7512/qdma_bmgr.c" 1
	.set	push
	.set	noreorder
	.set	mips2
	sync
	.set	pop
 # 0 "" 2
#NO_APP
	lw	$2,164($sp)
	sll	$2,$2,8
	move	$3,$2
	lw	$2,24($sp)
	addu	$2,$3,$2
	addiu	$2,$2,16
	sw	$2,96($sp)
	lw	$2,96($sp)
	lw	$2,0($2)
	sw	$2,48($sp)
	lw	$2,164($sp)
	sll	$2,$2,8
	move	$3,$2
	lw	$2,24($sp)
	addu	$2,$3,$2
	addiu	$3,$2,16
	lw	$4,48($sp)
	li	$2,-4096			# 0xfffffffffffff000
	and	$4,$4,$2
	lw	$2,16($sp)
	lw	$2,4($2)
	andi	$2,$2,0xfff
	or	$2,$4,$2
	sw	$3,100($sp)
	sw	$2,104($sp)
	lw	$2,100($sp)
	lw	$3,104($sp)
	sw	$3,0($2)
	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lw	$3,164($sp)
	addiu	$3,$3,48
	sll	$3,$3,2
	addu	$3,$2,$3
	lw	$3,4($3)
	addiu	$3,$3,1
	lw	$4,164($sp)
	addiu	$4,$4,48
	sll	$4,$4,2
	addu	$2,$2,$4
	sw	$3,4($2)
	lui	$2,%hi(gpQdmaPriv)
	lw	$3,%lo(gpQdmaPriv)($2)
	lw	$2,164($sp)
	sll	$2,$2,2
	addu	$2,$3,$2
	addiu	$2,$2,4
	sw	$2,108($sp)
	lw	$2,40($sp)
	sw	$2,112($sp)
	lw	$2,108($sp)
	move	$4,$2
	lw	$5,112($sp)
	lui	$2,%hi(_raw_spin_unlock_irqrestore)
	addiu	$2,$2,%lo(_raw_spin_unlock_irqrestore)
	jalr	$2
	lw	$2,44($sp)
$L164:
	lw	$31,156($sp)
	addiu	$sp,$sp,160
	j	$31
	.end	qdma_bm_transmit_packet
	.size	qdma_bm_transmit_packet, .-qdma_bm_transmit_packet
	.section	.text.qdma_bm_recycle_transmit_buffer,"ax",@progbits
	.align	2
	.globl	qdma_bm_recycle_transmit_buffer
	.set	nomips16
	.set	nomicromips
	.ent	qdma_bm_recycle_transmit_buffer
	.type	qdma_bm_recycle_transmit_buffer, @function
qdma_bm_recycle_transmit_buffer:
	.frame	$sp,160,$31		# vars= 136, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	addiu	$sp,$sp,-160
	sw	$31,156($sp)
	sw	$4,160($sp)
	sw	$0,20($sp)
	sw	$0,24($sp)
	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lw	$2,0($2)
	sw	$2,28($sp)
	sw	$0,32($sp)
	sw	$0,36($sp)
	sw	$0,16($sp)
	lui	$2,%hi(gpQdmaPriv)
	lw	$3,%lo(gpQdmaPriv)($2)
	lw	$2,160($sp)
	sll	$2,$2,2
	addu	$2,$3,$2
	addiu	$2,$2,4
	sw	$2,48($sp)
	lw	$2,48($sp)
	move	$4,$2
	lui	$2,%hi(_raw_spin_lock_irqsave)
	addiu	$2,$2,%lo(_raw_spin_lock_irqsave)
	jalr	$2
	sw	$2,36($sp)
	sw	$0,16($sp)
	j	$L167
$L178:
	lw	$2,16($sp)
	sll	$3,$2,4
	lui	$2,%hi(gpQdmaPriv)
	lw	$4,%lo(gpQdmaPriv)($2)
	lw	$2,160($sp)
	addiu	$2,$2,10
	sll	$2,$2,2
	addu	$2,$4,$2
	lw	$2,0($2)
	addu	$2,$3,$2
	sw	$2,20($sp)
	lw	$2,20($sp)
	lw	$2,12($2)
	bne	$2,$0,$L168
	lui	$2,%hi(gpQdmaPriv)
	lw	$3,%lo(gpQdmaPriv)($2)
	lw	$2,160($sp)
	addiu	$2,$2,20
	sll	$2,$2,2
	addu	$2,$3,$2
	lw	$3,0($2)
	lw	$2,20($sp)
	beq	$3,$2,$L168
	lui	$2,%hi(gpQdmaPriv)
	lw	$3,%lo(gpQdmaPriv)($2)
	lw	$2,160($sp)
	addiu	$2,$2,22
	sll	$2,$2,2
	addu	$2,$3,$2
	lw	$3,0($2)
	lw	$2,20($sp)
	beq	$3,$2,$L168
	lw	$2,20($sp)
	lw	$2,0($2)
	sw	$2,24($sp)
	lw	$2,20($sp)
	lw	$2,8($2)
	beq	$2,$0,$L169
	lw	$2,24($sp)
	lw	$2,8($2)
	beq	$2,$0,$L169
	lw	$2,24($sp)
	lw	$3,8($2)
	lw	$2,24($sp)
	lhu	$2,6($2)
	move	$4,$0
	move	$5,$3
	move	$6,$2
	li	$7,1			# 0x1
	lui	$2,%hi(dma_unmap_single)
	addiu	$2,$2,%lo(dma_unmap_single)
	jalr	$2
	lw	$2,20($sp)
	lw	$3,8($2)
	lw	$2,24($sp)
	lw	$2,16($2)
	sw	$3,52($sp)
	sw	$2,56($sp)
	sw	$0,60($sp)
	sw	$0,64($sp)
	sw	$0,68($sp)
	lw	$2,56($sp)
	ext	$2,$2,3,8
	andi	$2,$2,0x00ff
	sw	$2,60($sp)
	lw	$2,56($sp)
	ext	$2,$2,0,3
	andi	$2,$2,0x00ff
	sw	$2,64($sp)
	lw	$2,60($sp)
	sll	$3,$2,2
	lui	$2,%hi(chnlLimit)
	addiu	$2,$2,%lo(chnlLimit)
	addu	$2,$3,$2
	li	$3,1			# 0x1
	sw	$3,72($sp)
	sw	$2,76($sp)
	lw	$4,76($sp)
	lw	$3,72($sp)
	lw	$2,76($sp)
#APP
 # 112 "/opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/atomic.h" 1
		.set	mips3					
1:	ll	$5, 0($4)		# atomic_sub		
	subu	$5, $3					
	sc	$5, 0($4)					
	beqz	$5, 2f					
	.subsection 2					
2:	b	1b					
	.previous					
	.set	mips0					

 # 0 "" 2
#NO_APP
	move	$2,$5
	sw	$2,80($sp)
	lw	$2,60($sp)
	sll	$3,$2,3
	lw	$2,64($sp)
	addu	$2,$3,$2
	sll	$3,$2,2
	lui	$2,%hi(queueLimit)
	addiu	$2,$2,%lo(queueLimit)
	addu	$2,$3,$2
	li	$3,1			# 0x1
	sw	$3,84($sp)
	sw	$2,88($sp)
	lw	$4,88($sp)
	lw	$3,84($sp)
	lw	$2,88($sp)
#APP
 # 112 "/opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/atomic.h" 1
		.set	mips3					
1:	ll	$5, 0($4)		# atomic_sub		
	subu	$5, $3					
	sc	$5, 0($4)					
	beqz	$5, 2f					
	.subsection 2					
2:	b	1b					
	.previous					
	.set	mips0					

 # 0 "" 2
#NO_APP
	move	$2,$5
	sw	$2,92($sp)
	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lhu	$3,188($2)
	addiu	$3,$3,1
	andi	$3,$3,0xffff
	sh	$3,188($2)
	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lw	$2,156($2)
	beq	$2,$0,$L170
	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lw	$2,156($2)
	lw	$4,52($sp)
	lw	$5,56($sp)
	jalr	$2
	beq	$2,$0,$L182
	li	$2,-1			# 0xffffffffffffffff
	sw	$2,68($sp)
	j	$L172
$L170:
	lw	$4,52($sp)
	lui	$2,%hi(dev_kfree_skb_any)
	addiu	$2,$2,%lo(dev_kfree_skb_any)
	jalr	$2
	j	$L172
$L182:
	.set	noreorder
	nop
	.set	reorder
$L172:
	lw	$2,24($sp)
	addiu	$2,$2,16
	sw	$2,32($sp)
	lw	$2,32($sp)
	beq	$2,$0,$L169
	lw	$2,24($sp)
	addiu	$2,$2,16
	move	$4,$2
	move	$5,$0
	li	$6,8			# 0x8
	lui	$2,%hi(memset)
	addiu	$2,$2,%lo(memset)
	jalr	$2
$L169:
	lw	$2,20($sp)
	sw	$2,96($sp)
	lw	$2,160($sp)
	sw	$2,100($sp)
	lw	$2,96($sp)
	lw	$2,12($2)
	bne	$2,$0,$L168
	lw	$2,96($sp)
	sw	$0,8($2)
	lui	$2,%hi(gpQdmaPriv)
	lw	$3,%lo(gpQdmaPriv)($2)
	lw	$2,100($sp)
	addiu	$2,$2,18
	sll	$2,$2,2
	addu	$2,$3,$2
	lw	$2,0($2)
	bne	$2,$0,$L176
	lui	$2,%hi(gpQdmaPriv)
	lw	$3,%lo(gpQdmaPriv)($2)
	lw	$2,100($sp)
	addiu	$2,$2,18
	sll	$2,$2,2
	addu	$2,$3,$2
	lw	$3,96($sp)
	sw	$3,0($2)
	lui	$2,%hi(gpQdmaPriv)
	lw	$3,%lo(gpQdmaPriv)($2)
	lw	$2,100($sp)
	addiu	$2,$2,20
	sll	$2,$2,2
	addu	$2,$3,$2
	lw	$3,96($sp)
	sw	$3,0($2)
	j	$L168
$L176:
	lui	$2,%hi(gpQdmaPriv)
	lw	$3,%lo(gpQdmaPriv)($2)
	lw	$2,100($sp)
	addiu	$2,$2,20
	sll	$2,$2,2
	addu	$2,$3,$2
	lw	$2,0($2)
	lw	$3,96($sp)
	sw	$3,12($2)
	lui	$2,%hi(gpQdmaPriv)
	lw	$4,%lo(gpQdmaPriv)($2)
	lui	$2,%hi(gpQdmaPriv)
	lw	$3,%lo(gpQdmaPriv)($2)
	lw	$2,100($sp)
	addiu	$2,$2,20
	sll	$2,$2,2
	addu	$2,$3,$2
	lw	$2,0($2)
	lw	$3,12($2)
	lw	$2,100($sp)
	addiu	$2,$2,20
	sll	$2,$2,2
	addu	$2,$4,$2
	sw	$3,0($2)
$L168:
	lw	$2,16($sp)
	addiu	$2,$2,1
	sw	$2,16($sp)
$L167:
	lui	$2,%hi(gpQdmaPriv)
	lw	$3,%lo(gpQdmaPriv)($2)
	lw	$2,160($sp)
	addiu	$2,$2,8
	sll	$2,$2,1
	addu	$2,$3,$2
	lhu	$2,4($2)
	move	$3,$2
	lw	$2,16($sp)
	slt	$2,$2,$3
	bne	$2,$0,$L178
	lui	$2,%hi(gpQdmaPriv)
	lw	$3,%lo(gpQdmaPriv)($2)
	lw	$2,160($sp)
	sll	$2,$2,2
	addu	$2,$3,$2
	addiu	$2,$2,4
	sw	$2,104($sp)
	lw	$2,36($sp)
	sw	$2,108($sp)
	lw	$2,104($sp)
	move	$4,$2
	lw	$5,108($sp)
	lui	$2,%hi(_raw_spin_unlock_irqrestore)
	addiu	$2,$2,%lo(_raw_spin_unlock_irqrestore)
	jalr	$2
	lui	$2,%hi(gpQdmaPriv)
	lw	$3,%lo(gpQdmaPriv)($2)
	lw	$2,160($sp)
	addiu	$2,$2,22
	sll	$2,$2,2
	addu	$2,$3,$2
	lw	$2,0($2)
	sw	$2,20($sp)
	lw	$2,160($sp)
	sll	$2,$2,8
	move	$3,$2
	lw	$2,28($sp)
	addu	$2,$3,$2
	addiu	$2,$2,16
	sw	$2,112($sp)
	lw	$2,112($sp)
	lw	$2,0($2)
	sw	$2,40($sp)
	lw	$2,160($sp)
	sll	$2,$2,8
	move	$3,$2
	lw	$2,28($sp)
	addu	$2,$3,$2
	addiu	$3,$2,16
	lw	$4,40($sp)
	li	$2,-4096			# 0xfffffffffffff000
	and	$4,$4,$2
	lw	$2,20($sp)
	lw	$2,4($2)
	andi	$2,$2,0xfff
	or	$2,$4,$2
	sw	$3,116($sp)
	sw	$2,120($sp)
	lw	$2,116($sp)
	lw	$3,120($sp)
	sw	$3,0($2)
	lw	$2,160($sp)
	sll	$2,$2,8
	move	$3,$2
	lw	$2,28($sp)
	addu	$2,$3,$2
	addiu	$2,$2,20
	sw	$2,124($sp)
	lw	$2,124($sp)
	lw	$2,0($2)
	sw	$2,44($sp)
	lw	$2,160($sp)
	sll	$2,$2,8
	move	$3,$2
	lw	$2,28($sp)
	addu	$2,$3,$2
	addiu	$3,$2,20
	lw	$4,44($sp)
	li	$2,-4096			# 0xfffffffffffff000
	and	$4,$4,$2
	lw	$2,20($sp)
	lw	$2,4($2)
	andi	$2,$2,0xfff
	or	$2,$4,$2
	sw	$3,128($sp)
	sw	$2,132($sp)
	lw	$2,128($sp)
	lw	$3,132($sp)
	sw	$3,0($2)
	move	$2,$0
	lw	$31,156($sp)
	addiu	$sp,$sp,160
	j	$31
	.end	qdma_bm_recycle_transmit_buffer
	.size	qdma_bm_recycle_transmit_buffer, .-qdma_bm_recycle_transmit_buffer
	.section	.text.qdma_bm_counters_read_proc,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	qdma_bm_counters_read_proc
	.type	qdma_bm_counters_read_proc, @function
qdma_bm_counters_read_proc:
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
	sw	$7,36($sp)
	move	$4,$0
	lui	$2,%hi(qdma_dump_cpu_counters)
	addiu	$2,$2,%lo(qdma_dump_cpu_counters)
	jalr	$2
	nop

	move	$2,$0
	lw	$31,20($sp)
	addiu	$sp,$sp,24
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	qdma_bm_counters_read_proc
	.size	qdma_bm_counters_read_proc, .-qdma_bm_counters_read_proc
	.rdata
	.align	2
$LC1:
	.ascii	"%d\000"
	.section	.text.qdma_bm_counters_write_proc,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	qdma_bm_counters_write_proc
	.type	qdma_bm_counters_write_proc, @function
qdma_bm_counters_write_proc:
	.frame	$sp,120,$31		# vars= 96, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	addiu	$sp,$sp,-120
	sw	$31,116($sp)
	sw	$4,120($sp)
	sw	$5,124($sp)
	sw	$6,128($sp)
	sw	$7,132($sp)
	sw	$0,108($sp)
	lw	$2,128($sp)
	sltu	$2,$2,64
	bne	$2,$0,$L186
	li	$2,-22			# 0xffffffffffffffea
	j	$L191
$L186:
	addiu	$2,$sp,44
	sw	$2,20($sp)
	lw	$2,124($sp)
	sw	$2,24($sp)
	lw	$2,128($sp)
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
	beq	$2,$0,$L188
	lw	$4,20($sp)
	lw	$5,24($sp)
	lw	$6,16($sp)
#APP
 # 1347 "/opt/tclinux_phoenix/modules/private/qdma/EN7512/qdma_bmgr.c" 1
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
$L188:
	lw	$2,16($sp)
	beq	$2,$0,$L189
	li	$2,-14			# 0xfffffffffffffff2
	j	$L191
$L189:
	addiu	$2,$sp,44
	addiu	$3,$sp,108
	move	$4,$2
	lui	$2,%hi($LC1)
	addiu	$5,$2,%lo($LC1)
	move	$6,$3
	lui	$2,%hi(sscanf)
	addiu	$2,$2,%lo(sscanf)
	jalr	$2
	lw	$3,108($sp)
	li	$2,1			# 0x1
	bne	$3,$2,$L190
	move	$4,$0
	lui	$2,%hi(qdma_clear_cpu_counters)
	addiu	$2,$2,%lo(qdma_clear_cpu_counters)
	jalr	$2
$L190:
	lw	$2,128($sp)
$L191:
	lw	$31,116($sp)
	addiu	$sp,$sp,120
	j	$31
	.end	qdma_bm_counters_write_proc
	.size	qdma_bm_counters_write_proc, .-qdma_bm_counters_write_proc
	.section	.text.qdma_bm_debug_read_proc,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	qdma_bm_debug_read_proc
	.type	qdma_bm_debug_read_proc, @function
qdma_bm_debug_read_proc:
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
	.end	qdma_bm_debug_read_proc
	.size	qdma_bm_debug_read_proc, .-qdma_bm_debug_read_proc
	.rdata
	.align	2
$LC2:
	.ascii	"%s %s %d %x\000"
	.align	2
$LC3:
	.ascii	"level\000"
	.align	2
$LC4:
	.ascii	"dump\000"
	.align	2
$LC5:
	.ascii	"csr\000"
	.align	2
$LC6:
	.ascii	"dscp\000"
	.align	2
$LC7:
	.ascii	"irq\000"
	.align	2
$LC8:
	.ascii	"hwfwd\000"
	.align	2
$LC9:
	.ascii	"dbgcntr\000"
	.align	2
$LC10:
	.ascii	"clear\000"
	.align	2
$LC11:
	.ascii	"queue\000"
	.align	2
$LC12:
	.ascii	"channel_cpu\000"
	.align	2
$LC13:
	.ascii	"channel_fwd1\000"
	.align	2
$LC14:
	.ascii	"channel_fwd2\000"
	.align	2
$LC15:
	.ascii	"test\000"
	.align	2
$LC16:
	.ascii	"wrr\000"
	.align	2
$LC17:
	.ascii	"Channel:%d, Type:%d, Q0:%d, Q1:%d, Q2:%d, Q3:%d, Q4:%d, "
	.ascii	"Q5:%d, Q6:%d, Q7:%d\012\000"
	.align	2
$LC18:
	.ascii	"dma_busy\000"
	.align	2
$LC19:
	.ascii	"reg_polling\000"
	.align	2
$LC20:
	.ascii	"forceRX1\000"
	.section	.text.qdma_bm_debug_write_proc,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	qdma_bm_debug_write_proc
	.type	qdma_bm_debug_write_proc, @function
qdma_bm_debug_write_proc:
	.frame	$sp,496,$31		# vars= 440, regs= 1/0, args= 48, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	addiu	$sp,$sp,-496
	sw	$31,492($sp)
	sw	$4,496($sp)
	sw	$5,500($sp)
	sw	$6,504($sp)
	sw	$7,508($sp)
	sw	$0,60($sp)
	sw	$0,432($sp)
	sw	$0,436($sp)
	lw	$2,504($sp)
	sltu	$2,$2,64
	bne	$2,$0,$L195
	li	$2,-22			# 0xffffffffffffffea
	j	$L240
$L195:
	addiu	$2,$sp,304
	sw	$2,64($sp)
	lw	$2,500($sp)
	sw	$2,68($sp)
	lw	$2,504($sp)
	sw	$2,48($sp)
	lw	$2,68($sp)
	sw	$2,72($sp)
	lw	$2,48($sp)
	sw	$2,76($sp)
	move	$2,$28
	lw	$2,24($2)
	sw	$2,80($sp)
	lw	$3,72($sp)
	lw	$2,76($sp)
	addu	$3,$3,$2
	lw	$2,72($sp)
	or	$3,$3,$2
	lw	$2,76($sp)
	or	$3,$3,$2
	lw	$2,80($sp)
	and	$2,$3,$2
	sw	$2,84($sp)
	lw	$2,84($sp)
	sltu	$2,$2,1
	andi	$2,$2,0x00ff
	sltu	$2,$0,$2
	andi	$2,$2,0x00ff
	beq	$2,$0,$L197
	lw	$4,64($sp)
	lw	$5,68($sp)
	lw	$6,48($sp)
#APP
 # 1377 "/opt/tclinux_phoenix/modules/private/qdma/EN7512/qdma_bmgr.c" 1
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
	sw	$2,48($sp)
$L197:
	lw	$2,48($sp)
	beq	$2,$0,$L198
	li	$2,-14			# 0xfffffffffffffff2
	j	$L240
$L198:
	addiu	$4,$sp,304
	addiu	$6,$sp,368
	addiu	$3,$sp,400
	addiu	$2,$sp,432
	sw	$2,16($sp)
	addiu	$2,$sp,436
	sw	$2,20($sp)
	lui	$2,%hi($LC2)
	addiu	$5,$2,%lo($LC2)
	move	$7,$3
	lui	$2,%hi(sscanf)
	addiu	$2,$2,%lo(sscanf)
	jalr	$2
	addiu	$2,$sp,368
	sw	$2,88($sp)
	lui	$2,%hi($LC3)
	addiu	$2,$2,%lo($LC3)
	sw	$2,92($sp)
	lw	$3,88($sp)
	lw	$2,92($sp)
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
	sw	$3,88($sp)
	sw	$2,92($sp)
	sw	$4,96($sp)
	lw	$2,96($sp)
	beq	$2,$0,$L200
	addiu	$2,$sp,368
	sw	$2,100($sp)
	lui	$2,%hi($LC4)
	addiu	$2,$2,%lo($LC4)
	sw	$2,104($sp)
	lw	$3,100($sp)
	lw	$2,104($sp)
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
	sw	$3,100($sp)
	sw	$2,104($sp)
	sw	$4,108($sp)
	lw	$2,108($sp)
	bne	$2,$0,$L202
	addiu	$2,$sp,400
	sw	$2,112($sp)
	lui	$2,%hi($LC5)
	addiu	$2,$2,%lo($LC5)
	sw	$2,116($sp)
	lw	$3,112($sp)
	lw	$2,116($sp)
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
	sw	$3,112($sp)
	sw	$2,116($sp)
	sw	$4,120($sp)
	lw	$2,120($sp)
	bne	$2,$0,$L204
	move	$4,$0
	lui	$2,%hi(qdma_dump_register_value)
	addiu	$2,$2,%lo(qdma_dump_register_value)
	jalr	$2
	j	$L200
$L204:
	addiu	$2,$sp,400
	sw	$2,124($sp)
	lui	$2,%hi($LC6)
	addiu	$2,$2,%lo($LC6)
	sw	$2,128($sp)
	lw	$3,124($sp)
	lw	$2,128($sp)
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
	sw	$3,124($sp)
	sw	$2,128($sp)
	sw	$4,132($sp)
	lw	$2,132($sp)
	bne	$2,$0,$L207
	move	$4,$0
	lui	$2,%hi(qdma_dump_descriptor_info)
	addiu	$2,$2,%lo(qdma_dump_descriptor_info)
	jalr	$2
	j	$L200
$L207:
	addiu	$2,$sp,400
	sw	$2,136($sp)
	lui	$2,%hi($LC7)
	addiu	$2,$2,%lo($LC7)
	sw	$2,140($sp)
	lw	$3,136($sp)
	lw	$2,140($sp)
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
	sw	$3,136($sp)
	sw	$2,140($sp)
	sw	$4,144($sp)
	lw	$2,144($sp)
	bne	$2,$0,$L209
	move	$4,$0
	lui	$2,%hi(qdma_dump_irq_info)
	addiu	$2,$2,%lo(qdma_dump_irq_info)
	jalr	$2
	j	$L200
$L209:
	addiu	$2,$sp,400
	sw	$2,148($sp)
	lui	$2,%hi($LC8)
	addiu	$2,$2,%lo($LC8)
	sw	$2,152($sp)
	lw	$3,148($sp)
	lw	$2,152($sp)
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
	sw	$3,148($sp)
	sw	$2,152($sp)
	sw	$4,156($sp)
	lw	$2,156($sp)
	bne	$2,$0,$L200
	move	$4,$0
	lui	$2,%hi(qdma_dump_hwfwd_info)
	addiu	$2,$2,%lo(qdma_dump_hwfwd_info)
	jalr	$2
	j	$L200
$L202:
	addiu	$2,$sp,368
	sw	$2,160($sp)
	lui	$2,%hi($LC9)
	addiu	$2,$2,%lo($LC9)
	sw	$2,164($sp)
	lw	$3,160($sp)
	lw	$2,164($sp)
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
	sw	$3,160($sp)
	sw	$2,164($sp)
	sw	$4,168($sp)
	lw	$2,168($sp)
	bne	$2,$0,$L212
	addiu	$2,$sp,400
	sw	$2,172($sp)
	lui	$2,%hi($LC4)
	addiu	$2,$2,%lo($LC4)
	sw	$2,176($sp)
	lw	$3,172($sp)
	lw	$2,176($sp)
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
	sw	$3,172($sp)
	sw	$2,176($sp)
	sw	$4,180($sp)
	lw	$2,180($sp)
	bne	$2,$0,$L214
	move	$4,$0
	lui	$2,%hi(qdma_dump_dbg_cntr_value)
	addiu	$2,$2,%lo(qdma_dump_dbg_cntr_value)
	jalr	$2
	j	$L200
$L214:
	addiu	$2,$sp,400
	sw	$2,184($sp)
	lui	$2,%hi($LC10)
	addiu	$2,$2,%lo($LC10)
	sw	$2,188($sp)
	lw	$3,184($sp)
	lw	$2,188($sp)
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
	sw	$3,184($sp)
	sw	$2,188($sp)
	sw	$4,192($sp)
	lw	$2,192($sp)
	bne	$2,$0,$L217
	move	$4,$0
	lui	$2,%hi(qdma_clear_dbg_cntr_value_all)
	addiu	$2,$2,%lo(qdma_clear_dbg_cntr_value_all)
	jalr	$2
	j	$L200
$L217:
	addiu	$2,$sp,400
	sw	$2,196($sp)
	lui	$2,%hi($LC11)
	addiu	$2,$2,%lo($LC11)
	sw	$2,200($sp)
	lw	$3,196($sp)
	lw	$2,200($sp)
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
	sw	$3,196($sp)
	sw	$2,200($sp)
	sw	$4,204($sp)
	lw	$2,204($sp)
	bne	$2,$0,$L219
	move	$4,$0
	lui	$2,%hi(qdma_clear_and_set_dbg_cntr_queue_group)
	addiu	$2,$2,%lo(qdma_clear_and_set_dbg_cntr_queue_group)
	jalr	$2
	j	$L200
$L219:
	addiu	$2,$sp,400
	sw	$2,208($sp)
	lui	$2,%hi($LC12)
	addiu	$2,$2,%lo($LC12)
	sw	$2,212($sp)
	lw	$3,208($sp)
	lw	$2,212($sp)
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
	sw	$3,208($sp)
	sw	$2,212($sp)
	sw	$4,216($sp)
	lw	$2,216($sp)
	bne	$2,$0,$L221
	sw	$0,448($sp)
	addiu	$2,$sp,440
	move	$4,$2
	lui	$2,%hi(qdma_clear_and_set_dbg_cntr_channel_group)
	addiu	$2,$2,%lo(qdma_clear_and_set_dbg_cntr_channel_group)
	jalr	$2
	j	$L200
$L221:
	addiu	$2,$sp,400
	sw	$2,220($sp)
	lui	$2,%hi($LC13)
	addiu	$2,$2,%lo($LC13)
	sw	$2,224($sp)
	lw	$3,220($sp)
	lw	$2,224($sp)
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
	sw	$3,220($sp)
	sw	$2,224($sp)
	sw	$4,228($sp)
	lw	$2,228($sp)
	bne	$2,$0,$L223
	li	$2,1			# 0x1
	sw	$2,448($sp)
	addiu	$2,$sp,440
	move	$4,$2
	lui	$2,%hi(qdma_clear_and_set_dbg_cntr_channel_group)
	addiu	$2,$2,%lo(qdma_clear_and_set_dbg_cntr_channel_group)
	jalr	$2
	j	$L200
$L223:
	addiu	$2,$sp,400
	sw	$2,232($sp)
	lui	$2,%hi($LC14)
	addiu	$2,$2,%lo($LC14)
	sw	$2,236($sp)
	lw	$3,232($sp)
	lw	$2,236($sp)
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
	sw	$3,232($sp)
	sw	$2,236($sp)
	sw	$4,240($sp)
	lw	$2,240($sp)
	bne	$2,$0,$L200
	li	$2,2			# 0x2
	sw	$2,448($sp)
	addiu	$2,$sp,440
	move	$4,$2
	lui	$2,%hi(qdma_clear_and_set_dbg_cntr_channel_group)
	addiu	$2,$2,%lo(qdma_clear_and_set_dbg_cntr_channel_group)
	jalr	$2
	j	$L200
$L212:
	addiu	$2,$sp,368
	sw	$2,244($sp)
	lui	$2,%hi($LC15)
	addiu	$2,$2,%lo($LC15)
	sw	$2,248($sp)
	lw	$3,244($sp)
	lw	$2,248($sp)
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
	sw	$3,244($sp)
	sw	$2,248($sp)
	sw	$4,252($sp)
	lw	$2,252($sp)
	bne	$2,$0,$L200
	addiu	$2,$sp,400
	sw	$2,256($sp)
	lui	$2,%hi($LC16)
	addiu	$2,$2,%lo($LC16)
	sw	$2,260($sp)
	lw	$3,256($sp)
	lw	$2,260($sp)
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
	sw	$3,256($sp)
	sw	$2,260($sp)
	sw	$4,264($sp)
	lw	$2,264($sp)
	bne	$2,$0,$L227
	sw	$0,52($sp)
	j	$L228
$L232:
	lw	$2,52($sp)
	andi	$2,$2,0x00ff
	sb	$2,468($sp)
	lw	$3,52($sp)
	li	$2,-2147483648			# 0xffffffff80000000
	ori	$2,$2,0x7
	and	$2,$3,$2
	bgez	$2,$L229
	addiu	$2,$2,-1
	li	$3,-8			# 0xfffffffffffffff8
	or	$2,$2,$3
	addiu	$2,$2,1
$L229:
	sw	$2,472($sp)
	sw	$0,56($sp)
	j	$L230
$L231:
	lw	$2,52($sp)
	andi	$2,$2,0x00ff
	sll	$2,$2,1
	sll	$3,$2,2
	addu	$2,$2,$3
	andi	$3,$2,0x00ff
	lw	$2,56($sp)
	andi	$2,$2,0x00ff
	addu	$2,$3,$2
	andi	$3,$2,0x00ff
	lw	$2,56($sp)
	addiu	$4,$sp,48
	addu	$2,$4,$2
	sb	$3,428($2)
	lw	$2,56($sp)
	addiu	$2,$2,1
	sw	$2,56($sp)
$L230:
	lw	$2,56($sp)
	slt	$2,$2,8
	bne	$2,$0,$L231
	addiu	$2,$sp,468
	sw	$2,456($sp)
	addiu	$2,$sp,440
	move	$4,$2
	lui	$2,%hi(qdma_set_tx_qos)
	addiu	$2,$2,%lo(qdma_set_tx_qos)
	jalr	$2
	lw	$2,52($sp)
	addiu	$2,$2,1
	sw	$2,52($sp)
$L228:
	lw	$2,52($sp)
	slt	$2,$2,8
	bne	$2,$0,$L232
	sw	$0,52($sp)
	j	$L233
$L234:
	addiu	$2,$sp,468
	move	$4,$2
	move	$5,$0
	li	$6,16			# 0x10
	lui	$2,%hi(memset)
	addiu	$2,$2,%lo(memset)
	jalr	$2
	lw	$2,52($sp)
	andi	$2,$2,0x00ff
	sb	$2,468($sp)
	addiu	$2,$sp,468
	sw	$2,456($sp)
	addiu	$2,$sp,440
	move	$4,$2
	lui	$2,%hi(qdma_get_tx_qos)
	addiu	$2,$2,%lo(qdma_get_tx_qos)
	jalr	$2
	lw	$3,472($sp)
	lbu	$2,476($sp)
	move	$10,$2
	lbu	$2,477($sp)
	move	$9,$2
	lbu	$2,478($sp)
	move	$8,$2
	lbu	$2,479($sp)
	move	$7,$2
	lbu	$2,480($sp)
	move	$6,$2
	lbu	$2,481($sp)
	move	$5,$2
	lbu	$2,482($sp)
	move	$4,$2
	lbu	$2,483($sp)
	sw	$9,16($sp)
	sw	$8,20($sp)
	sw	$7,24($sp)
	sw	$6,28($sp)
	sw	$5,32($sp)
	sw	$4,36($sp)
	sw	$2,40($sp)
	lui	$2,%hi($LC17)
	addiu	$4,$2,%lo($LC17)
	lw	$5,52($sp)
	move	$6,$3
	move	$7,$10
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	lw	$2,52($sp)
	addiu	$2,$2,1
	sw	$2,52($sp)
$L233:
	lw	$2,52($sp)
	slt	$2,$2,8
	bne	$2,$0,$L234
	j	$L200
$L227:
	addiu	$2,$sp,400
	sw	$2,268($sp)
	lui	$2,%hi($LC18)
	addiu	$2,$2,%lo($LC18)
	sw	$2,272($sp)
	lw	$3,268($sp)
	lw	$2,272($sp)
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
	sw	$3,268($sp)
	sw	$2,272($sp)
	sw	$4,276($sp)
	lw	$2,276($sp)
	bne	$2,$0,$L236
	lw	$2,432($sp)
	sw	$2,452($sp)
	addiu	$2,$sp,440
	move	$4,$2
	lui	$2,%hi(qdma_dump_dma_busy)
	addiu	$2,$2,%lo(qdma_dump_dma_busy)
	jalr	$2
	j	$L200
$L236:
	addiu	$2,$sp,400
	sw	$2,280($sp)
	lui	$2,%hi($LC19)
	addiu	$2,$2,%lo($LC19)
	sw	$2,284($sp)
	lw	$3,280($sp)
	lw	$2,284($sp)
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
	sw	$3,280($sp)
	sw	$2,284($sp)
	sw	$4,288($sp)
	lw	$2,288($sp)
	bne	$2,$0,$L238
	lw	$2,436($sp)
	sw	$2,456($sp)
	lw	$2,432($sp)
	sw	$2,460($sp)
	addiu	$2,$sp,440
	move	$4,$2
	lui	$2,%hi(qdma_dump_reg_polling)
	addiu	$2,$2,%lo(qdma_dump_reg_polling)
	jalr	$2
	j	$L200
$L238:
	addiu	$2,$sp,400
	sw	$2,292($sp)
	lui	$2,%hi($LC20)
	addiu	$2,$2,%lo($LC20)
	sw	$2,296($sp)
	lw	$3,292($sp)
	lw	$2,296($sp)
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
	sw	$3,292($sp)
	sw	$2,296($sp)
	sw	$4,300($sp)
	lw	$2,300($sp)
	bne	$2,$0,$L200
	lw	$2,432($sp)
	sw	$2,464($sp)
	addiu	$2,$sp,440
	move	$4,$2
	lui	$2,%hi(qdma_set_force_receive_rx_ring1)
	addiu	$2,$2,%lo(qdma_set_force_receive_rx_ring1)
	jalr	$2
$L200:
	lw	$2,504($sp)
$L240:
	lw	$31,492($sp)
	addiu	$sp,$sp,496
	j	$31
	.end	qdma_bm_debug_write_proc
	.size	qdma_bm_debug_write_proc, .-qdma_bm_debug_write_proc
	.rdata
	.align	2
$LC21:
	.ascii	"echo ratelimit [tx/rx] r > /proc/%s\012\000"
	.align	2
$LC22:
	.ascii	"qdma_lan/ratelimit\000"
	.align	2
$LC23:
	.ascii	"echo ratelimit [tx/rx] w [Enable/Disable] [unit:(1~%dkbp"
	.ascii	"s)] > /proc/%s\012\000"
	.align	2
$LC24:
	.ascii	"echo ratelimit [tx/rx] w [chanIdx/ringIdx] [pir:(kbps)] "
	.ascii	"[dropEn:Enable/Disable]> /proc/%s\012\000"
	.align	2
$LC25:
	.ascii	"echo ratemeter tx r > /proc/%s\012\000"
	.align	2
$LC26:
	.ascii	"echo ratemeter tx w [Enable/Disable] [unit:(2~%dkbps)] >"
	.ascii	" /proc/%s\012\000"
	.section	.text.qdma_bm_ratelimit_read_proc,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	qdma_bm_ratelimit_read_proc
	.type	qdma_bm_ratelimit_read_proc, @function
qdma_bm_ratelimit_read_proc:
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
	sw	$0,16($sp)
	sw	$0,24($sp)
	sw	$0,20($sp)
	lw	$2,16($sp)
	lw	$3,40($sp)
	addu	$2,$3,$2
	move	$4,$2
	lui	$2,%hi($LC21)
	addiu	$5,$2,%lo($LC21)
	lui	$2,%hi($LC22)
	addiu	$6,$2,%lo($LC22)
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
	sw	$2,24($sp)
	lw	$3,24($sp)
	lw	$2,48($sp)
	slt	$2,$3,$2
	beq	$2,$0,$L242
	nop

	sw	$0,16($sp)
	lw	$2,24($sp)
	sw	$2,20($sp)
$L242:
	lw	$3,48($sp)
	lw	$2,52($sp)
	addu	$3,$3,$2
	lw	$2,24($sp)
	slt	$2,$3,$2
	beq	$2,$0,$L243
	nop

	j	$L244
	nop

$L243:
	lw	$2,16($sp)
	lw	$3,40($sp)
	addu	$2,$3,$2
	move	$4,$2
	lui	$2,%hi($LC23)
	addiu	$5,$2,%lo($LC23)
	li	$6,64			# 0x40
	lui	$2,%hi($LC22)
	addiu	$7,$2,%lo($LC22)
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
	sw	$2,24($sp)
	lw	$3,24($sp)
	lw	$2,48($sp)
	slt	$2,$3,$2
	beq	$2,$0,$L245
	nop

	sw	$0,16($sp)
	lw	$2,24($sp)
	sw	$2,20($sp)
$L245:
	lw	$3,48($sp)
	lw	$2,52($sp)
	addu	$3,$3,$2
	lw	$2,24($sp)
	slt	$2,$3,$2
	beq	$2,$0,$L246
	nop

	j	$L244
	nop

$L246:
	lw	$2,16($sp)
	lw	$3,40($sp)
	addu	$2,$3,$2
	move	$4,$2
	lui	$2,%hi($LC24)
	addiu	$5,$2,%lo($LC24)
	lui	$2,%hi($LC22)
	addiu	$6,$2,%lo($LC22)
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
	sw	$2,24($sp)
	lw	$3,24($sp)
	lw	$2,48($sp)
	slt	$2,$3,$2
	beq	$2,$0,$L247
	nop

	sw	$0,16($sp)
	lw	$2,24($sp)
	sw	$2,20($sp)
$L247:
	lw	$3,48($sp)
	lw	$2,52($sp)
	addu	$3,$3,$2
	lw	$2,24($sp)
	slt	$2,$3,$2
	beq	$2,$0,$L248
	nop

	j	$L244
	nop

$L248:
	lw	$2,16($sp)
	lw	$3,40($sp)
	addu	$2,$3,$2
	move	$4,$2
	lui	$2,%hi($LC25)
	addiu	$5,$2,%lo($LC25)
	lui	$2,%hi($LC22)
	addiu	$6,$2,%lo($LC22)
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
	sw	$2,24($sp)
	lw	$3,24($sp)
	lw	$2,48($sp)
	slt	$2,$3,$2
	beq	$2,$0,$L249
	nop

	sw	$0,16($sp)
	lw	$2,24($sp)
	sw	$2,20($sp)
$L249:
	lw	$3,48($sp)
	lw	$2,52($sp)
	addu	$3,$3,$2
	lw	$2,24($sp)
	slt	$2,$3,$2
	beq	$2,$0,$L250
	nop

	j	$L244
	nop

$L250:
	lw	$2,16($sp)
	lw	$3,40($sp)
	addu	$2,$3,$2
	move	$4,$2
	lui	$2,%hi($LC26)
	addiu	$5,$2,%lo($LC26)
	li	$6,64			# 0x40
	lui	$2,%hi($LC22)
	addiu	$7,$2,%lo($LC22)
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
	sw	$2,24($sp)
	lw	$3,24($sp)
	lw	$2,48($sp)
	slt	$2,$3,$2
	beq	$2,$0,$L251
	nop

	sw	$0,16($sp)
	lw	$2,24($sp)
	sw	$2,20($sp)
$L251:
	lw	$3,48($sp)
	lw	$2,52($sp)
	addu	$3,$3,$2
	lw	$2,24($sp)
	slt	$2,$3,$2
	beq	$2,$0,$L252
	nop

	j	$L244
	nop

$L252:
	lw	$2,56($sp)
	li	$3,1			# 0x1
	sw	$3,0($2)
$L244:
	lw	$3,48($sp)
	lw	$2,20($sp)
	subu	$2,$3,$2
	move	$3,$2
	lw	$2,40($sp)
	addu	$3,$2,$3
	lw	$2,44($sp)
	sw	$3,0($2)
	lw	$3,20($sp)
	lw	$2,48($sp)
	subu	$2,$3,$2
	lw	$3,16($sp)
	addu	$2,$3,$2
	sw	$2,16($sp)
	lw	$2,16($sp)
	bgez	$2,$L253
	nop

	sw	$0,16($sp)
$L253:
	lw	$3,16($sp)
	lw	$2,52($sp)
	slt	$2,$2,$3
	beq	$2,$0,$L254
	nop

	lw	$2,52($sp)
	sw	$2,16($sp)
$L254:
	lw	$2,16($sp)
	lw	$31,36($sp)
	addiu	$sp,$sp,40
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	qdma_bm_ratelimit_read_proc
	.size	qdma_bm_ratelimit_read_proc, .-qdma_bm_ratelimit_read_proc
	.rdata
	.align	2
$LC27:
	.ascii	"%s %s %s %s %d %s\000"
	.align	2
$LC28:
	.ascii	"Enable\000"
	.align	2
$LC29:
	.ascii	"Disable\000"
	.align	2
$LC30:
	.ascii	"tx\000"
	.align	2
$LC31:
	.ascii	"rx\000"
	.align	2
$LC32:
	.ascii	"Fault: index is %d, which should between 0 and %d\012\000"
	.align	2
$LC33:
	.ascii	"w\000"
	.align	2
$LC34:
	.ascii	"ratelimit\000"
	.align	2
$LC35:
	.ascii	"Fault: Rate Limit unit is %dkbps, which should between 1"
	.ascii	" and %d\012\000"
	.align	2
$LC36:
	.ascii	"ratemeter\000"
	.align	2
$LC37:
	.ascii	"Fault: Rate Meter unit is %dkpbs, which should between 2"
	.ascii	" and %d\012\000"
	.align	2
$LC38:
	.ascii	"r\000"
	.align	2
$LC39:
	.ascii	"txRateLimitEn:Enable, txBucketScale:%d, unit:%dkbps\012\000"
	.align	2
$LC40:
	.ascii	"Channel_%d:  pir:%dkbps, pbs:%dByte\012\000"
	.align	2
$LC41:
	.ascii	"Error: ratelimitValue is %dkbps, which shold less than 4"
	.ascii	"gbps!\000"
	.align	2
$LC42:
	.ascii	"channel:%d, pirParamValue:%ukbps\012\000"
	.align	2
$LC43:
	.ascii	"rxDropEn:Enable, rxRateLimitEn:%s, rxBucketScale:%d, rxR"
	.ascii	"ateLimitUnit:%dkbps\012\000"
	.align	2
$LC44:
	.ascii	"RX%d:  pir:%dkbps, pbs:%dByte\012\000"
	.align	2
$LC45:
	.ascii	"ringIdx:%d, pirParamValue:%ukbps\012\000"
	.align	2
$LC46:
	.ascii	"Fault: ratelimit only effect on tx/rx Direction\012\000"
	.align	2
$LC47:
	.ascii	"txRateMeterEn:Enable, txRateMeterDivisor:%d, txRateMeter"
	.ascii	"TimeSlice:%dus\012\000"
	.align	2
$LC48:
	.ascii	"Channel_%d: rateMeter:%dkbps\012\000"
	.align	2
$LC49:
	.ascii	"Fault: ratemeter only effect on tx Direction\012\000"
	.section	.text.qdma_bm_ratelimit_write_proc,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	qdma_bm_ratelimit_write_proc
	.type	qdma_bm_ratelimit_write_proc, @function
qdma_bm_ratelimit_write_proc:
	.frame	$sp,760,$31		# vars= 720, regs= 1/0, args= 32, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	addiu	$sp,$sp,-760
	sw	$31,756($sp)
	sw	$4,760($sp)
	sw	$5,764($sp)
	sw	$6,768($sp)
	sw	$7,772($sp)
	sw	$0,32($sp)
	sw	$0,644($sp)
	sw	$0,36($sp)
	sw	$0,48($sp)
	sw	$0,40($sp)
	sw	$0,52($sp)
	lw	$2,768($sp)
	sltu	$2,$2,128
	bne	$2,$0,$L257
	li	$2,-22			# 0xffffffffffffffea
	j	$L327
$L257:
	addiu	$2,$sp,356
	sw	$2,56($sp)
	lw	$2,764($sp)
	sw	$2,60($sp)
	lw	$2,768($sp)
	sw	$2,44($sp)
	lw	$2,60($sp)
	sw	$2,64($sp)
	lw	$2,44($sp)
	sw	$2,68($sp)
	move	$2,$28
	lw	$2,24($2)
	sw	$2,72($sp)
	lw	$3,64($sp)
	lw	$2,68($sp)
	addu	$3,$3,$2
	lw	$2,64($sp)
	or	$3,$3,$2
	lw	$2,68($sp)
	or	$3,$3,$2
	lw	$2,72($sp)
	and	$2,$3,$2
	sw	$2,76($sp)
	lw	$2,76($sp)
	sltu	$2,$2,1
	andi	$2,$2,0x00ff
	sltu	$2,$0,$2
	andi	$2,$2,0x00ff
	beq	$2,$0,$L259
	lw	$4,56($sp)
	lw	$5,60($sp)
	lw	$6,44($sp)
#APP
 # 1507 "/opt/tclinux_phoenix/modules/private/qdma/EN7512/qdma_bmgr.c" 1
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
	sw	$2,44($sp)
$L259:
	lw	$2,44($sp)
	beq	$2,$0,$L260
	li	$2,-14			# 0xfffffffffffffff2
	j	$L327
$L260:
	addiu	$4,$sp,356
	addiu	$6,$sp,484
	addiu	$3,$sp,516
	addiu	$2,$sp,548
	sw	$2,16($sp)
	addiu	$2,$sp,580
	sw	$2,20($sp)
	addiu	$2,$sp,644
	sw	$2,24($sp)
	addiu	$2,$sp,612
	sw	$2,28($sp)
	lui	$2,%hi($LC27)
	addiu	$5,$2,%lo($LC27)
	move	$7,$3
	lui	$2,%hi(sscanf)
	addiu	$2,$2,%lo(sscanf)
	jalr	$2
	addiu	$2,$sp,580
	sw	$2,80($sp)
	lui	$2,%hi($LC28)
	addiu	$2,$2,%lo($LC28)
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
	bne	$2,$0,$L262
	li	$2,1			# 0x1
	sw	$2,40($sp)
	j	$L263
$L262:
	addiu	$2,$sp,580
	sw	$2,92($sp)
	lui	$2,%hi($LC29)
	addiu	$2,$2,%lo($LC29)
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
	bne	$2,$0,$L265
	sw	$0,40($sp)
	j	$L263
$L265:
	addiu	$2,$sp,580
	move	$4,$2
	move	$5,$0
	li	$6,10			# 0xa
	lui	$2,%hi(simple_strtoul)
	addiu	$2,$2,%lo(simple_strtoul)
	jalr	$2
	sw	$2,32($sp)
	lw	$2,32($sp)
	bltz	$2,$L266
	lw	$2,32($sp)
	slt	$2,$2,9
	bne	$2,$0,$L267
	addiu	$2,$sp,516
	sw	$2,104($sp)
	lui	$2,%hi($LC30)
	addiu	$2,$2,%lo($LC30)
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
	beq	$2,$0,$L266
$L267:
	lw	$2,32($sp)
	slt	$2,$2,3
	bne	$2,$0,$L263
	addiu	$2,$sp,516
	sw	$2,116($sp)
	lui	$2,%hi($LC31)
	addiu	$2,$2,%lo($LC31)
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
	bne	$2,$0,$L263
$L266:
	addiu	$2,$sp,516
	sw	$2,128($sp)
	lui	$2,%hi($LC30)
	addiu	$2,$2,%lo($LC30)
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
	bne	$2,$0,$L271
	li	$2,8			# 0x8
	j	$L272
$L271:
	li	$2,2			# 0x2
$L272:
	lui	$3,%hi($LC32)
	addiu	$4,$3,%lo($LC32)
	lw	$5,32($sp)
	move	$6,$2
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	li	$2,-14			# 0xfffffffffffffff2
	j	$L327
$L263:
	addiu	$2,$sp,612
	sw	$2,140($sp)
	lui	$2,%hi($LC28)
	addiu	$2,$2,%lo($LC28)
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
	bne	$2,$0,$L274
	li	$2,1			# 0x1
	sw	$2,52($sp)
	j	$L275
$L274:
	sw	$0,52($sp)
$L275:
	addiu	$2,$sp,548
	sw	$2,152($sp)
	lui	$2,%hi($LC33)
	addiu	$2,$2,%lo($LC33)
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
	bne	$2,$0,$L277
	addiu	$2,$sp,484
	sw	$2,164($sp)
	lui	$2,%hi($LC34)
	addiu	$2,$2,%lo($LC34)
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
	bne	$2,$0,$L277
	addiu	$2,$sp,580
	sw	$2,176($sp)
	lui	$2,%hi($LC28)
	addiu	$2,$2,%lo($LC28)
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
	beq	$2,$0,$L280
	addiu	$2,$sp,580
	sw	$2,188($sp)
	lui	$2,%hi($LC29)
	addiu	$2,$2,%lo($LC29)
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
	bne	$2,$0,$L277
$L280:
	lw	$2,644($sp)
	sw	$2,36($sp)
	lw	$2,36($sp)
	blez	$2,$L282
	lw	$2,36($sp)
	slt	$2,$2,65
	bne	$2,$0,$L277
$L282:
	lui	$2,%hi($LC35)
	addiu	$4,$2,%lo($LC35)
	lw	$5,36($sp)
	li	$6,64			# 0x40
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	li	$2,-14			# 0xfffffffffffffff2
	j	$L327
$L277:
	addiu	$2,$sp,548
	sw	$2,200($sp)
	lui	$2,%hi($LC33)
	addiu	$2,$2,%lo($LC33)
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
	bne	$2,$0,$L284
	addiu	$2,$sp,484
	sw	$2,212($sp)
	lui	$2,%hi($LC36)
	addiu	$2,$2,%lo($LC36)
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
	bne	$2,$0,$L284
	lw	$2,644($sp)
	sw	$2,36($sp)
	lw	$2,36($sp)
	slt	$2,$2,2
	bne	$2,$0,$L286
	lw	$2,36($sp)
	slt	$2,$2,65
	bne	$2,$0,$L284
$L286:
	lui	$2,%hi($LC37)
	addiu	$4,$2,%lo($LC37)
	lw	$5,36($sp)
	li	$6,64			# 0x40
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	li	$2,-14			# 0xfffffffffffffff2
	j	$L327
$L284:
	addiu	$2,$sp,484
	sw	$2,224($sp)
	lui	$2,%hi($LC34)
	addiu	$2,$2,%lo($LC34)
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
	bne	$2,$0,$L288
	addiu	$2,$sp,516
	sw	$2,236($sp)
	lui	$2,%hi($LC30)
	addiu	$2,$2,%lo($LC30)
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
	bne	$2,$0,$L290
	addiu	$2,$sp,548
	sw	$2,248($sp)
	lui	$2,%hi($LC38)
	addiu	$2,$2,%lo($LC38)
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
	bne	$2,$0,$L292
	addiu	$2,$sp,648
	sw	$2,728($sp)
	addiu	$2,$sp,720
	move	$4,$2
	lui	$2,%hi(qdma_get_tx_ratelimit_cfg)
	addiu	$2,$2,%lo(qdma_get_tx_ratelimit_cfg)
	jalr	$2
	lw	$2,652($sp)
	move	$3,$2
	li	$2,1			# 0x1
	sll	$3,$2,$3
	lhu	$2,648($sp)
	move	$6,$2
	lui	$2,%hi($LC39)
	addiu	$4,$2,%lo($LC39)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	lhu	$2,648($sp)
	beq	$2,$0,$L316
	sw	$0,32($sp)
	j	$L294
$L297:
	lw	$2,32($sp)
	andi	$2,$2,0x00ff
	sb	$2,680($sp)
	addiu	$2,$sp,680
	sw	$2,728($sp)
	addiu	$2,$sp,720
	move	$4,$2
	lui	$2,%hi(qdma_get_tx_ratelimit)
	addiu	$2,$2,%lo(qdma_get_tx_ratelimit)
	jalr	$2
	lw	$2,684($sp)
	bne	$2,$0,$L295
	lw	$2,688($sp)
	beq	$2,$0,$L296
$L295:
	lw	$6,684($sp)
	lw	$2,688($sp)
	lw	$3,652($sp)
	sll	$3,$2,$3
	lui	$2,%hi($LC40)
	addiu	$4,$2,%lo($LC40)
	lw	$5,32($sp)
	move	$7,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
$L296:
	lw	$2,32($sp)
	addiu	$2,$2,1
	sw	$2,32($sp)
$L294:
	lw	$2,32($sp)
	slt	$2,$2,8
	bne	$2,$0,$L297
	j	$L316
$L292:
	addiu	$2,$sp,548
	sw	$2,260($sp)
	lui	$2,%hi($LC33)
	addiu	$2,$2,%lo($LC33)
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
	bne	$2,$0,$L316
	lw	$2,36($sp)
	beq	$2,$0,$L299
	li	$2,1			# 0x1
	sw	$2,652($sp)
	lw	$2,36($sp)
	andi	$2,$2,0xffff
	sh	$2,648($sp)
	addiu	$2,$sp,648
	sw	$2,728($sp)
	j	$L316
$L299:
	lw	$2,32($sp)
	andi	$2,$2,0x00ff
	sb	$2,672($sp)
	lw	$2,644($sp)
	sw	$2,676($sp)
	lw	$3,676($sp)
	li	$2,4194304			# 0x400000
	ori	$2,$2,0x1
	sltu	$2,$3,$2
	bne	$2,$0,$L300
	lw	$3,644($sp)
	lui	$2,%hi($LC41)
	addiu	$4,$2,%lo($LC41)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	li	$2,-14			# 0xfffffffffffffff2
	j	$L327
$L300:
	lw	$3,676($sp)
	lui	$2,%hi($LC42)
	addiu	$4,$2,%lo($LC42)
	lw	$5,32($sp)
	move	$6,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	addiu	$2,$sp,672
	sw	$2,728($sp)
	addiu	$2,$sp,720
	move	$4,$2
	lui	$2,%hi(qdma_set_tx_ratelimit)
	addiu	$2,$2,%lo(qdma_set_tx_ratelimit)
	jalr	$2
	j	$L316
$L290:
	addiu	$2,$sp,516
	sw	$2,272($sp)
	lui	$2,%hi($LC31)
	addiu	$2,$2,%lo($LC31)
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
	bne	$2,$0,$L303
	addiu	$2,$sp,548
	sw	$2,284($sp)
	lui	$2,%hi($LC38)
	addiu	$2,$2,%lo($LC38)
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
	bne	$2,$0,$L305
	addiu	$2,$sp,656
	sw	$2,728($sp)
	addiu	$2,$sp,720
	move	$4,$2
	lui	$2,%hi(qdma_get_rx_ratelimit_cfg)
	addiu	$2,$2,%lo(qdma_get_rx_ratelimit_cfg)
	jalr	$2
	lbu	$2,656($sp)
	beq	$2,$0,$L306
	lui	$2,%hi($LC28)
	addiu	$2,$2,%lo($LC28)
	j	$L307
$L306:
	lui	$2,%hi($LC29)
	addiu	$2,$2,%lo($LC29)
$L307:
	lw	$3,668($sp)
	move	$4,$3
	li	$3,1			# 0x1
	sll	$6,$3,$4
	lhu	$3,664($sp)
	move	$7,$3
	lui	$3,%hi($LC43)
	addiu	$4,$3,%lo($LC43)
	move	$5,$2
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	lhu	$2,664($sp)
	beq	$2,$0,$L316
	sw	$0,32($sp)
	j	$L309
$L312:
	lw	$2,32($sp)
	andi	$2,$2,0x00ff
	sb	$2,700($sp)
	addiu	$2,$sp,700
	sw	$2,728($sp)
	addiu	$2,$sp,720
	move	$4,$2
	lui	$2,%hi(qdma_get_rx_ratelimit)
	addiu	$2,$2,%lo(qdma_get_rx_ratelimit)
	jalr	$2
	lw	$2,704($sp)
	bne	$2,$0,$L310
	lw	$2,708($sp)
	beq	$2,$0,$L311
$L310:
	lw	$6,704($sp)
	lw	$2,708($sp)
	lw	$3,668($sp)
	sll	$3,$2,$3
	lui	$2,%hi($LC44)
	addiu	$4,$2,%lo($LC44)
	lw	$5,32($sp)
	move	$7,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
$L311:
	lw	$2,32($sp)
	addiu	$2,$2,1
	sw	$2,32($sp)
$L309:
	lw	$2,32($sp)
	slt	$2,$2,2
	bne	$2,$0,$L312
	j	$L316
$L305:
	addiu	$2,$sp,548
	sw	$2,296($sp)
	lui	$2,%hi($LC33)
	addiu	$2,$2,%lo($LC33)
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
	bne	$2,$0,$L316
	lw	$2,36($sp)
	beq	$2,$0,$L314
	lw	$2,40($sp)
	andi	$2,$2,0x00ff
	sb	$2,656($sp)
	li	$2,1			# 0x1
	sw	$2,668($sp)
	lw	$2,36($sp)
	andi	$2,$2,0xffff
	sh	$2,664($sp)
	lw	$2,40($sp)
	sw	$2,728($sp)
	addiu	$2,$sp,720
	move	$4,$2
	lui	$2,%hi(qdma_set_rx_ratelimit_en)
	addiu	$2,$2,%lo(qdma_set_rx_ratelimit_en)
	jalr	$2
	sw	$0,728($sp)
	addiu	$2,$sp,720
	move	$4,$2
	lui	$2,%hi(qdma_set_rx_ratelimit_pkt_mode)
	addiu	$2,$2,%lo(qdma_set_rx_ratelimit_pkt_mode)
	jalr	$2
	j	$L316
$L314:
	lw	$2,32($sp)
	andi	$2,$2,0x00ff
	sb	$2,692($sp)
	lw	$2,644($sp)
	sw	$2,696($sp)
	lw	$3,696($sp)
	li	$2,4194304			# 0x400000
	ori	$2,$2,0x1
	sltu	$2,$3,$2
	bne	$2,$0,$L315
	lw	$3,644($sp)
	lui	$2,%hi($LC41)
	addiu	$4,$2,%lo($LC41)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	li	$2,-14			# 0xfffffffffffffff2
	j	$L327
$L315:
	lw	$3,696($sp)
	lui	$2,%hi($LC45)
	addiu	$4,$2,%lo($LC45)
	lw	$5,32($sp)
	move	$6,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	addiu	$2,$sp,692
	sw	$2,728($sp)
	addiu	$2,$sp,720
	move	$4,$2
	lui	$2,%hi(qdma_set_rx_ratelimit)
	addiu	$2,$2,%lo(qdma_set_rx_ratelimit)
	jalr	$2
	j	$L316
$L303:
	lui	$2,%hi($LC46)
	addiu	$4,$2,%lo($LC46)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	j	$L316
$L288:
	addiu	$2,$sp,484
	sw	$2,308($sp)
	lui	$2,%hi($LC36)
	addiu	$2,$2,%lo($LC36)
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
	bne	$2,$0,$L316
	addiu	$2,$sp,516
	sw	$2,320($sp)
	lui	$2,%hi($LC30)
	addiu	$2,$2,%lo($LC30)
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
	bne	$2,$0,$L319
	addiu	$2,$sp,548
	sw	$2,332($sp)
	lui	$2,%hi($LC38)
	addiu	$2,$2,%lo($LC38)
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
	bne	$2,$0,$L321
	addiu	$2,$sp,712
	sw	$2,728($sp)
	addiu	$2,$sp,720
	move	$4,$2
	lui	$2,%hi(qdma_get_tx_ratemeter)
	addiu	$2,$2,%lo(qdma_get_tx_ratemeter)
	jalr	$2
	lw	$2,716($sp)
	move	$3,$2
	li	$2,1			# 0x1
	sll	$3,$2,$3
	lhu	$2,712($sp)
	move	$6,$2
	lui	$2,%hi($LC47)
	addiu	$4,$2,%lo($LC47)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	sw	$0,32($sp)
	j	$L322
$L324:
	lw	$2,32($sp)
	sw	$2,728($sp)
	addiu	$2,$sp,720
	move	$4,$2
	lui	$2,%hi(qdma_get_tx_channel_datarate)
	addiu	$2,$2,%lo(qdma_get_tx_channel_datarate)
	jalr	$2
	sw	$2,48($sp)
	lw	$3,48($sp)
	li	$2,-1			# 0xffffffffffffffff
	beq	$3,$2,$L323
	lw	$2,48($sp)
	li	$3,274857984			# 0x10620000
	ori	$3,$3,0x4dd3
	mult	$2,$3
	mfhi	$3
	sra	$3,$3,6
	sra	$2,$2,31
	subu	$3,$3,$2
	lui	$2,%hi($LC48)
	addiu	$4,$2,%lo($LC48)
	lw	$5,32($sp)
	move	$6,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
$L323:
	lw	$2,32($sp)
	addiu	$2,$2,1
	sw	$2,32($sp)
$L322:
	lw	$2,32($sp)
	slt	$2,$2,8
	bne	$2,$0,$L324
	j	$L316
$L321:
	addiu	$2,$sp,548
	sw	$2,344($sp)
	lui	$2,%hi($LC33)
	addiu	$2,$2,%lo($LC33)
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
	bne	$2,$0,$L316
	addiu	$2,$sp,712
	addiu	$3,$2,4
	addiu	$2,$sp,712
	lw	$4,36($sp)
	move	$5,$3
	move	$6,$2
	lui	$2,%hi(qdmaRateMeterValueCalCulate)
	addiu	$2,$2,%lo(qdmaRateMeterValueCalCulate)
	jalr	$2
	move	$3,$2
	li	$2,-1			# 0xffffffffffffffff
	beq	$3,$2,$L316
	addiu	$2,$sp,712
	sw	$2,728($sp)
	addiu	$2,$sp,720
	move	$4,$2
	lui	$2,%hi(qdma_set_tx_ratemeter)
	addiu	$2,$2,%lo(qdma_set_tx_ratemeter)
	jalr	$2
	j	$L316
$L319:
	lui	$2,%hi($LC49)
	addiu	$4,$2,%lo($LC49)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
$L316:
	lw	$2,768($sp)
$L327:
	lw	$31,756($sp)
	addiu	$sp,$sp,760
	j	$31
	.end	qdma_bm_ratelimit_write_proc
	.size	qdma_bm_ratelimit_write_proc, .-qdma_bm_ratelimit_write_proc
	.rdata
	.align	2
$LC50:
	.ascii	"echo [chnlId:0~31] sp > /proc/%s\012\000"
	.align	2
$LC51:
	.ascii	"qdma_lan/qos\000"
	.align	2
$LC52:
	.ascii	"echo [chnlId:0~31] wrr w0 w1 w2 w3 w4 w5 w6 w7 > /proc/%"
	.ascii	"s\012\000"
	.align	2
$LC53:
	.ascii	"echo [chnlId:0~31] wrr2 w0 w1 > /proc/%s\012\000"
	.align	2
$LC54:
	.ascii	"echo [chnlId:0~31] wrr3 w0 w1 w2 > /proc/%s\012\000"
	.align	2
$LC55:
	.ascii	"echo [chnlId:0~31] wrr4 w0 w1 w2 w3 > /proc/%s\012\000"
	.align	2
$LC56:
	.ascii	"echo [chnlId:0~31] wrr5 w0 w1 w2 w3 w4 > /proc/%s\012\000"
	.align	2
$LC57:
	.ascii	"echo [chnlId:0~31] wrr6 w0 w1 w2 w3 w4 w5 > /proc/%s\012"
	.ascii	"\000"
	.align	2
$LC58:
	.ascii	"echo [chnlId:0~31] wrr7 w0 w1 w2 w3 w4 w5 w6 > /proc/%s\012"
	.ascii	"\000"
	.section	.text.qdma_bm_qos_read_proc,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	qdma_bm_qos_read_proc
	.type	qdma_bm_qos_read_proc, @function
qdma_bm_qos_read_proc:
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
	sw	$0,16($sp)
	sw	$0,24($sp)
	sw	$0,20($sp)
	lw	$2,16($sp)
	lw	$3,40($sp)
	addu	$2,$3,$2
	move	$4,$2
	lui	$2,%hi($LC50)
	addiu	$5,$2,%lo($LC50)
	lui	$2,%hi($LC51)
	addiu	$6,$2,%lo($LC51)
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
	sw	$2,24($sp)
	lw	$3,24($sp)
	lw	$2,48($sp)
	slt	$2,$3,$2
	beq	$2,$0,$L329
	nop

	sw	$0,16($sp)
	lw	$2,24($sp)
	sw	$2,20($sp)
$L329:
	lw	$3,48($sp)
	lw	$2,52($sp)
	addu	$3,$3,$2
	lw	$2,24($sp)
	slt	$2,$3,$2
	beq	$2,$0,$L330
	nop

	j	$L331
	nop

$L330:
	lw	$2,16($sp)
	lw	$3,40($sp)
	addu	$2,$3,$2
	move	$4,$2
	lui	$2,%hi($LC52)
	addiu	$5,$2,%lo($LC52)
	lui	$2,%hi($LC51)
	addiu	$6,$2,%lo($LC51)
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
	sw	$2,24($sp)
	lw	$3,24($sp)
	lw	$2,48($sp)
	slt	$2,$3,$2
	beq	$2,$0,$L332
	nop

	sw	$0,16($sp)
	lw	$2,24($sp)
	sw	$2,20($sp)
$L332:
	lw	$3,48($sp)
	lw	$2,52($sp)
	addu	$3,$3,$2
	lw	$2,24($sp)
	slt	$2,$3,$2
	beq	$2,$0,$L333
	nop

	j	$L331
	nop

$L333:
	lw	$2,16($sp)
	lw	$3,40($sp)
	addu	$2,$3,$2
	move	$4,$2
	lui	$2,%hi($LC53)
	addiu	$5,$2,%lo($LC53)
	lui	$2,%hi($LC51)
	addiu	$6,$2,%lo($LC51)
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
	sw	$2,24($sp)
	lw	$3,24($sp)
	lw	$2,48($sp)
	slt	$2,$3,$2
	beq	$2,$0,$L334
	nop

	sw	$0,16($sp)
	lw	$2,24($sp)
	sw	$2,20($sp)
$L334:
	lw	$3,48($sp)
	lw	$2,52($sp)
	addu	$3,$3,$2
	lw	$2,24($sp)
	slt	$2,$3,$2
	beq	$2,$0,$L335
	nop

	j	$L331
	nop

$L335:
	lw	$2,16($sp)
	lw	$3,40($sp)
	addu	$2,$3,$2
	move	$4,$2
	lui	$2,%hi($LC54)
	addiu	$5,$2,%lo($LC54)
	lui	$2,%hi($LC51)
	addiu	$6,$2,%lo($LC51)
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
	sw	$2,24($sp)
	lw	$3,24($sp)
	lw	$2,48($sp)
	slt	$2,$3,$2
	beq	$2,$0,$L336
	nop

	sw	$0,16($sp)
	lw	$2,24($sp)
	sw	$2,20($sp)
$L336:
	lw	$3,48($sp)
	lw	$2,52($sp)
	addu	$3,$3,$2
	lw	$2,24($sp)
	slt	$2,$3,$2
	beq	$2,$0,$L337
	nop

	j	$L331
	nop

$L337:
	lw	$2,16($sp)
	lw	$3,40($sp)
	addu	$2,$3,$2
	move	$4,$2
	lui	$2,%hi($LC55)
	addiu	$5,$2,%lo($LC55)
	lui	$2,%hi($LC51)
	addiu	$6,$2,%lo($LC51)
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
	sw	$2,24($sp)
	lw	$3,24($sp)
	lw	$2,48($sp)
	slt	$2,$3,$2
	beq	$2,$0,$L338
	nop

	sw	$0,16($sp)
	lw	$2,24($sp)
	sw	$2,20($sp)
$L338:
	lw	$3,48($sp)
	lw	$2,52($sp)
	addu	$3,$3,$2
	lw	$2,24($sp)
	slt	$2,$3,$2
	beq	$2,$0,$L339
	nop

	j	$L331
	nop

$L339:
	lw	$2,16($sp)
	lw	$3,40($sp)
	addu	$2,$3,$2
	move	$4,$2
	lui	$2,%hi($LC56)
	addiu	$5,$2,%lo($LC56)
	lui	$2,%hi($LC51)
	addiu	$6,$2,%lo($LC51)
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
	sw	$2,24($sp)
	lw	$3,24($sp)
	lw	$2,48($sp)
	slt	$2,$3,$2
	beq	$2,$0,$L340
	nop

	sw	$0,16($sp)
	lw	$2,24($sp)
	sw	$2,20($sp)
$L340:
	lw	$3,48($sp)
	lw	$2,52($sp)
	addu	$3,$3,$2
	lw	$2,24($sp)
	slt	$2,$3,$2
	beq	$2,$0,$L341
	nop

	j	$L331
	nop

$L341:
	lw	$2,16($sp)
	lw	$3,40($sp)
	addu	$2,$3,$2
	move	$4,$2
	lui	$2,%hi($LC57)
	addiu	$5,$2,%lo($LC57)
	lui	$2,%hi($LC51)
	addiu	$6,$2,%lo($LC51)
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
	sw	$2,24($sp)
	lw	$3,24($sp)
	lw	$2,48($sp)
	slt	$2,$3,$2
	beq	$2,$0,$L342
	nop

	sw	$0,16($sp)
	lw	$2,24($sp)
	sw	$2,20($sp)
$L342:
	lw	$3,48($sp)
	lw	$2,52($sp)
	addu	$3,$3,$2
	lw	$2,24($sp)
	slt	$2,$3,$2
	beq	$2,$0,$L343
	nop

	j	$L331
	nop

$L343:
	lw	$2,16($sp)
	lw	$3,40($sp)
	addu	$2,$3,$2
	move	$4,$2
	lui	$2,%hi($LC58)
	addiu	$5,$2,%lo($LC58)
	lui	$2,%hi($LC51)
	addiu	$6,$2,%lo($LC51)
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
	sw	$2,24($sp)
	lw	$3,24($sp)
	lw	$2,48($sp)
	slt	$2,$3,$2
	beq	$2,$0,$L344
	nop

	sw	$0,16($sp)
	lw	$2,24($sp)
	sw	$2,20($sp)
$L344:
	lw	$3,48($sp)
	lw	$2,52($sp)
	addu	$3,$3,$2
	lw	$2,24($sp)
	slt	$2,$3,$2
	beq	$2,$0,$L345
	nop

	j	$L331
	nop

$L345:
	lw	$2,56($sp)
	li	$3,1			# 0x1
	sw	$3,0($2)
$L331:
	lw	$3,48($sp)
	lw	$2,20($sp)
	subu	$2,$3,$2
	move	$3,$2
	lw	$2,40($sp)
	addu	$3,$2,$3
	lw	$2,44($sp)
	sw	$3,0($2)
	lw	$3,20($sp)
	lw	$2,48($sp)
	subu	$2,$3,$2
	lw	$3,16($sp)
	addu	$2,$3,$2
	sw	$2,16($sp)
	lw	$2,16($sp)
	bgez	$2,$L346
	nop

	sw	$0,16($sp)
$L346:
	lw	$3,16($sp)
	lw	$2,52($sp)
	slt	$2,$2,$3
	beq	$2,$0,$L347
	nop

	lw	$2,52($sp)
	sw	$2,16($sp)
$L347:
	lw	$2,16($sp)
	lw	$31,36($sp)
	addiu	$sp,$sp,40
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	qdma_bm_qos_read_proc
	.size	qdma_bm_qos_read_proc, .-qdma_bm_qos_read_proc
	.rdata
	.align	2
$LC59:
	.ascii	"%d %s %d %d %d %d %d %d %d %d\000"
	.align	2
$LC60:
	.ascii	"sp\000"
	.align	2
$LC61:
	.ascii	"wrr2\000"
	.align	2
$LC62:
	.ascii	"wrr3\000"
	.align	2
$LC63:
	.ascii	"wrr4\000"
	.align	2
$LC64:
	.ascii	"wrr5\000"
	.align	2
$LC65:
	.ascii	"wrr6\000"
	.align	2
$LC66:
	.ascii	"wrr7\000"
	.section	.text.qdma_bm_qos_write_proc,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	qdma_bm_qos_write_proc
	.type	qdma_bm_qos_write_proc, @function
qdma_bm_qos_write_proc:
	.frame	$sp,408,$31		# vars= 352, regs= 1/0, args= 48, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	addiu	$sp,$sp,-408
	sw	$31,404($sp)
	sw	$4,408($sp)
	sw	$5,412($sp)
	sw	$6,416($sp)
	sw	$7,420($sp)
	sw	$0,48($sp)
	sw	$0,320($sp)
	lw	$2,416($sp)
	sltu	$2,$2,128
	bne	$2,$0,$L350
	li	$2,-22			# 0xffffffffffffffea
	j	$L372
$L350:
	addiu	$2,$sp,176
	sw	$2,56($sp)
	lw	$2,412($sp)
	sw	$2,60($sp)
	lw	$2,416($sp)
	sw	$2,52($sp)
	lw	$2,60($sp)
	sw	$2,64($sp)
	lw	$2,52($sp)
	sw	$2,68($sp)
	move	$2,$28
	lw	$2,24($2)
	sw	$2,72($sp)
	lw	$3,64($sp)
	lw	$2,68($sp)
	addu	$3,$3,$2
	lw	$2,64($sp)
	or	$3,$3,$2
	lw	$2,68($sp)
	or	$3,$3,$2
	lw	$2,72($sp)
	and	$2,$3,$2
	sw	$2,76($sp)
	lw	$2,76($sp)
	sltu	$2,$2,1
	andi	$2,$2,0x00ff
	sltu	$2,$0,$2
	andi	$2,$2,0x00ff
	beq	$2,$0,$L352
	lw	$4,56($sp)
	lw	$5,60($sp)
	lw	$6,52($sp)
#APP
 # 1760 "/opt/tclinux_phoenix/modules/private/qdma/EN7512/qdma_bmgr.c" 1
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
	sw	$2,52($sp)
$L352:
	lw	$2,52($sp)
	beq	$2,$0,$L353
	li	$2,-14			# 0xfffffffffffffff2
	j	$L372
$L353:
	addiu	$4,$sp,176
	addiu	$6,$sp,320
	addiu	$3,$sp,304
	addiu	$2,$sp,324
	sw	$2,16($sp)
	addiu	$2,$sp,324
	addiu	$2,$2,4
	sw	$2,20($sp)
	addiu	$2,$sp,324
	addiu	$2,$2,8
	sw	$2,24($sp)
	addiu	$2,$sp,324
	addiu	$2,$2,12
	sw	$2,28($sp)
	addiu	$2,$sp,324
	addiu	$2,$2,16
	sw	$2,32($sp)
	addiu	$2,$sp,324
	addiu	$2,$2,20
	sw	$2,36($sp)
	addiu	$2,$sp,324
	addiu	$2,$2,24
	sw	$2,40($sp)
	addiu	$2,$sp,324
	addiu	$2,$2,28
	sw	$2,44($sp)
	lui	$2,%hi($LC59)
	addiu	$5,$2,%lo($LC59)
	move	$7,$3
	lui	$2,%hi(sscanf)
	addiu	$2,$2,%lo(sscanf)
	jalr	$2
	lw	$2,320($sp)
	andi	$2,$2,0x00ff
	sb	$2,356($sp)
	sw	$0,48($sp)
	j	$L354
$L355:
	lw	$2,48($sp)
	sll	$2,$2,2
	addiu	$3,$sp,48
	addu	$2,$3,$2
	lw	$2,276($2)
	andi	$3,$2,0x00ff
	lw	$2,48($sp)
	addiu	$4,$sp,48
	addu	$2,$4,$2
	sb	$3,316($2)
	lw	$2,48($sp)
	addiu	$2,$2,1
	sw	$2,48($sp)
$L354:
	lw	$2,48($sp)
	slt	$2,$2,8
	bne	$2,$0,$L355
	addiu	$2,$sp,304
	sw	$2,80($sp)
	lui	$2,%hi($LC60)
	addiu	$2,$2,%lo($LC60)
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
	bne	$2,$0,$L357
	li	$2,1			# 0x1
	sw	$2,360($sp)
	j	$L358
$L357:
	addiu	$2,$sp,304
	sw	$2,92($sp)
	lui	$2,%hi($LC16)
	addiu	$2,$2,%lo($LC16)
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
	bne	$2,$0,$L360
	sw	$0,360($sp)
	j	$L358
$L360:
	addiu	$2,$sp,304
	sw	$2,104($sp)
	lui	$2,%hi($LC61)
	addiu	$2,$2,%lo($LC61)
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
	bne	$2,$0,$L362
	li	$2,7			# 0x7
	sw	$2,360($sp)
	j	$L358
$L362:
	addiu	$2,$sp,304
	sw	$2,116($sp)
	lui	$2,%hi($LC62)
	addiu	$2,$2,%lo($LC62)
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
	bne	$2,$0,$L364
	li	$2,6			# 0x6
	sw	$2,360($sp)
	j	$L358
$L364:
	addiu	$2,$sp,304
	sw	$2,128($sp)
	lui	$2,%hi($LC63)
	addiu	$2,$2,%lo($LC63)
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
	bne	$2,$0,$L366
	li	$2,5			# 0x5
	sw	$2,360($sp)
	j	$L358
$L366:
	addiu	$2,$sp,304
	sw	$2,140($sp)
	lui	$2,%hi($LC64)
	addiu	$2,$2,%lo($LC64)
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
	bne	$2,$0,$L368
	li	$2,4			# 0x4
	sw	$2,360($sp)
	j	$L358
$L368:
	addiu	$2,$sp,304
	sw	$2,152($sp)
	lui	$2,%hi($LC65)
	addiu	$2,$2,%lo($LC65)
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
	bne	$2,$0,$L370
	li	$2,3			# 0x3
	sw	$2,360($sp)
	j	$L358
$L370:
	addiu	$2,$sp,304
	sw	$2,164($sp)
	lui	$2,%hi($LC66)
	addiu	$2,$2,%lo($LC66)
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
	bne	$2,$0,$L358
	li	$2,2			# 0x2
	sw	$2,360($sp)
$L358:
	addiu	$2,$sp,356
	sw	$2,388($sp)
	addiu	$2,$sp,372
	move	$4,$2
	lui	$2,%hi(qdma_set_tx_qos)
	addiu	$2,$2,%lo(qdma_set_tx_qos)
	jalr	$2
	lw	$2,416($sp)
$L372:
	lw	$31,404($sp)
	addiu	$sp,$sp,408
	j	$31
	.end	qdma_bm_qos_write_proc
	.size	qdma_bm_qos_write_proc, .-qdma_bm_qos_write_proc
	.section	.text.qdma_bm_task,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	qdma_bm_task
	.type	qdma_bm_task, @function
qdma_bm_task:
	.frame	$sp,136,$31		# vars= 112, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	addiu	$sp,$sp,-136
	sw	$31,132($sp)
	sw	$4,136($sp)
	li	$2,128			# 0x80
	sw	$2,20($sp)
	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lw	$2,0($2)
	sw	$2,24($sp)
	sw	$0,28($sp)
	sw	$0,16($sp)
	sw	$0,16($sp)
	j	$L374
$L375:
	lui	$2,%hi(gpQdmaPriv)
	lw	$3,%lo(gpQdmaPriv)($2)
	lw	$2,16($sp)
	addiu	$2,$2,136
	sll	$2,$2,1
	addu	$2,$3,$2
	sh	$0,4($2)
	lui	$2,%hi(gpQdmaPriv)
	lw	$3,%lo(gpQdmaPriv)($2)
	lw	$2,16($sp)
	addiu	$2,$2,140
	sll	$2,$2,1
	addu	$2,$3,$2
	sh	$0,4($2)
	lui	$2,%hi(gpQdmaPriv)
	lw	$3,%lo(gpQdmaPriv)($2)
	lw	$2,16($sp)
	addiu	$2,$2,136
	sll	$2,$2,1
	addu	$2,$3,$2
	sh	$0,8($2)
	lw	$2,16($sp)
	addiu	$2,$2,1
	sw	$2,16($sp)
$L374:
	lw	$2,16($sp)
	slt	$2,$2,2
	bne	$2,$0,$L375
	lw	$2,20($sp)
	sw	$2,104($sp)
	addiu	$2,$sp,96
	move	$4,$2
	lui	$2,%hi(qdma_receive_packets)
	addiu	$2,$2,%lo(qdma_receive_packets)
	jalr	$2
	move	$3,$2
	lw	$2,20($sp)
	sltu	$2,$3,$2
	beq	$2,$0,$L376
	lw	$4,24($sp)
	li	$5,170			# 0xaa
	lui	$2,%hi(qdmaEnableInt)
	addiu	$2,$2,%lo(qdmaEnableInt)
	jalr	$2
	j	$L373
$L376:
	sw	$0,16($sp)
	j	$L378
$L385:
	lui	$2,%hi(gpQdmaPriv)
	lw	$3,%lo(gpQdmaPriv)($2)
	lw	$2,16($sp)
	addiu	$2,$2,136
	sll	$2,$2,1
	addu	$2,$3,$2
	lhu	$2,4($2)
	move	$3,$2
	lui	$2,%hi(storm_ctrl_shrehold_lan)
	lw	$2,%lo(storm_ctrl_shrehold_lan)($2)
	slt	$2,$2,$3
	beq	$2,$0,$L379
	li	$2,-1078657024			# 0xffffffffbfb50000
	ori	$2,$2,0x500
	sw	$2,32($sp)
	lw	$2,32($sp)
	lw	$2,0($2)
	sw	$2,28($sp)
	lw	$3,28($sp)
	li	$2,-3841			# 0xfffffffffffff0ff
	and	$2,$3,$2
	sw	$2,28($sp)
	lw	$2,28($sp)
	ori	$2,$2,0x700
	sw	$2,28($sp)
	li	$2,-1078657024			# 0xffffffffbfb50000
	ori	$2,$2,0x500
	sw	$2,36($sp)
	lw	$2,28($sp)
	sw	$2,40($sp)
	lw	$2,36($sp)
	lw	$3,40($sp)
	sw	$3,0($2)
	j	$L381
$L379:
	lui	$2,%hi(gpQdmaPriv)
	lw	$3,%lo(gpQdmaPriv)($2)
	lw	$2,16($sp)
	addiu	$2,$2,140
	sll	$2,$2,1
	addu	$2,$3,$2
	lhu	$2,4($2)
	move	$3,$2
	lui	$2,%hi(storm_ctrl_shrehold_lan)
	lw	$2,%lo(storm_ctrl_shrehold_lan)($2)
	slt	$2,$2,$3
	beq	$2,$0,$L382
	li	$2,-1078657024			# 0xffffffffbfb50000
	ori	$2,$2,0x500
	sw	$2,44($sp)
	lw	$2,44($sp)
	lw	$2,0($2)
	sw	$2,28($sp)
	lw	$3,28($sp)
	li	$2,-241			# 0xffffffffffffff0f
	and	$2,$3,$2
	sw	$2,28($sp)
	lw	$2,28($sp)
	ori	$2,$2,0x70
	sw	$2,28($sp)
	li	$2,-1078657024			# 0xffffffffbfb50000
	ori	$2,$2,0x500
	sw	$2,48($sp)
	lw	$2,28($sp)
	sw	$2,52($sp)
	lw	$2,48($sp)
	lw	$3,52($sp)
	sw	$3,0($2)
	j	$L381
$L382:
	lui	$2,%hi(gpQdmaPriv)
	lw	$3,%lo(gpQdmaPriv)($2)
	lw	$2,16($sp)
	addiu	$2,$2,136
	sll	$2,$2,1
	addu	$2,$3,$2
	lhu	$2,8($2)
	move	$3,$2
	lui	$2,%hi(storm_ctrl_shrehold_lan)
	lw	$2,%lo(storm_ctrl_shrehold_lan)($2)
	slt	$2,$2,$3
	beq	$2,$0,$L381
	li	$2,-1078657024			# 0xffffffffbfb50000
	ori	$2,$2,0x500
	sw	$2,56($sp)
	lw	$2,56($sp)
	lw	$2,0($2)
	sw	$2,28($sp)
	lw	$3,28($sp)
	li	$2,-16			# 0xfffffffffffffff0
	and	$2,$3,$2
	sw	$2,28($sp)
	lw	$2,28($sp)
	ori	$2,$2,0x7
	sw	$2,28($sp)
	li	$2,-1078657024			# 0xffffffffbfb50000
	ori	$2,$2,0x500
	sw	$2,60($sp)
	lw	$2,28($sp)
	sw	$2,64($sp)
	lw	$2,60($sp)
	lw	$3,64($sp)
	sw	$3,0($2)
$L381:
	lw	$2,16($sp)
	addiu	$2,$2,1
	sw	$2,16($sp)
$L378:
	lw	$2,16($sp)
	slt	$2,$2,2
	bne	$2,$0,$L385
	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	addiu	$2,$2,304
	move	$4,$2
	lui	$2,%hi(del_timer_sync)
	addiu	$2,$2,%lo(del_timer_sync)
	jalr	$2
	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lui	$3,%hi(qdma_lan_fwd_timer)
	lw	$3,%lo(qdma_lan_fwd_timer)($3)
	sw	$3,324($2)
	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	addiu	$4,$2,304
	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lw	$2,324($2)
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
	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	addiu	$2,$2,120
	sw	$2,68($sp)
	lw	$2,68($sp)
	addiu	$2,$2,4
	sw	$0,72($sp)
	sw	$2,76($sp)
	lw	$2,72($sp)
	andi	$2,$2,0xffff
	andi	$2,$2,0x1f
	sh	$2,80($sp)
#APP
 # 261 "/opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/bitops.h" 1
			

 # 0 "" 2
#NO_APP
	lw	$2,72($sp)
	srl	$2,$2,5
	sll	$2,$2,2
	lw	$3,76($sp)
	addu	$2,$3,$2
	sw	$2,84($sp)
	lhu	$2,80($sp)
	li	$3,1			# 0x1
	sll	$2,$3,$2
	lw	$5,84($sp)
	lw	$3,84($sp)
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
	sw	$4,88($sp)
	sw	$3,92($sp)
#APP
 # 312 "/opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/bitops.h" 1
			

 # 0 "" 2
#NO_APP
	lw	$2,92($sp)
	sltu	$2,$0,$2
	andi	$2,$2,0x00ff
	bne	$2,$0,$L373
	lw	$4,68($sp)
	lui	$2,%hi(__tasklet_schedule)
	addiu	$2,$2,%lo(__tasklet_schedule)
	jalr	$2
$L373:
	lw	$31,132($sp)
	addiu	$sp,$sp,136
	j	$31
	.end	qdma_bm_task
	.size	qdma_bm_task, .-qdma_bm_task
	.section	.imem_text
	.align	2
	.globl	ecnt_qdma_lan_hook
	.set	nomips16
	.set	nomicromips
	.ent	ecnt_qdma_lan_hook
	.type	ecnt_qdma_lan_hook, @function
ecnt_qdma_lan_hook:
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
	sltu	$2,$2,99
	bne	$2,$0,$L388
	nop

	li	$2,-1			# 0xffffffffffffffff
	j	$L389
	nop

$L388:
	lw	$2,16($sp)
	lw	$3,0($2)
	lui	$2,%hi(qdma_operation)
	sll	$3,$3,2
	addiu	$2,$2,%lo(qdma_operation)
	addu	$2,$3,$2
	lw	$2,0($2)
	lw	$4,16($sp)
	jalr	$2
	nop

	move	$3,$2
	lw	$2,16($sp)
	sw	$3,4($2)
	li	$2,1			# 0x1
$L389:
	lw	$31,28($sp)
	addiu	$sp,$sp,32
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	ecnt_qdma_lan_hook
	.size	ecnt_qdma_lan_hook, .-ecnt_qdma_lan_hook
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	qdma_isr
	.type	qdma_isr, @function
qdma_isr:
	.frame	$sp,184,$31		# vars= 160, regs= 2/0, args= 16, gp= 0
	.mask	0x80010000,-4
	.fmask	0x00000000,0
	addiu	$sp,$sp,-184
	sw	$31,180($sp)
	sw	$16,176($sp)
	sw	$4,184($sp)
	sw	$5,188($sp)
	sw	$0,16($sp)
	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lw	$2,0($2)
	sw	$2,20($sp)
	lw	$2,20($sp)
	addiu	$2,$2,80
	sw	$2,24($sp)
	lw	$2,24($sp)
	lw	$16,0($2)
	lw	$4,20($sp)
	lui	$2,%hi(qdmaGetIntMask)
	addiu	$2,$2,%lo(qdmaGetIntMask)
	jalr	$2
	and	$2,$16,$2
	sw	$2,16($sp)
	lw	$2,16($sp)
	andi	$2,$2,0x33
	beq	$2,$0,$L392
	lw	$2,16($sp)
	andi	$2,$2,0x22
	beq	$2,$0,$L393
	lw	$2,20($sp)
	addiu	$2,$2,80
	sw	$2,28($sp)
	li	$2,34			# 0x22
	sw	$2,32($sp)
	lw	$2,28($sp)
	lw	$3,32($sp)
	sw	$3,0($2)
	lw	$2,16($sp)
	andi	$2,$2,0x800
	beq	$2,$0,$L394
	lw	$2,20($sp)
	addiu	$2,$2,80
	sw	$2,36($sp)
	li	$2,2048			# 0x800
	sw	$2,40($sp)
	lw	$2,36($sp)
	lw	$3,40($sp)
	sw	$3,0($2)
$L394:
	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lw	$3,140($2)
	li	$2,536870912			# 0x20000000
	and	$2,$3,$2
	beq	$2,$0,$L395
	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lw	$2,152($2)
	beq	$2,$0,$L397
	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lw	$2,152($2)
	move	$4,$0
	jalr	$2
	j	$L397
$L395:
	lw	$4,20($sp)
	li	$5,170			# 0xaa
	lui	$2,%hi(qdmaDisableInt)
	addiu	$2,$2,%lo(qdmaDisableInt)
	jalr	$2
	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	addiu	$2,$2,120
	sw	$2,44($sp)
	lw	$2,44($sp)
	addiu	$2,$2,4
	sw	$0,48($sp)
	sw	$2,52($sp)
	lw	$2,48($sp)
	andi	$2,$2,0xffff
	andi	$2,$2,0x1f
	sh	$2,56($sp)
#APP
 # 261 "/opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/bitops.h" 1
			

 # 0 "" 2
#NO_APP
	lw	$2,48($sp)
	srl	$2,$2,5
	sll	$2,$2,2
	lw	$3,52($sp)
	addu	$2,$3,$2
	sw	$2,60($sp)
	lhu	$2,56($sp)
	li	$3,1			# 0x1
	sll	$2,$3,$2
	lw	$5,60($sp)
	lw	$3,60($sp)
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
	sw	$4,64($sp)
	sw	$3,68($sp)
#APP
 # 312 "/opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/bitops.h" 1
			

 # 0 "" 2
#NO_APP
	lw	$2,68($sp)
	sltu	$2,$0,$2
	andi	$2,$2,0x00ff
	bne	$2,$0,$L397
	lw	$4,44($sp)
	lui	$2,%hi(__tasklet_schedule)
	addiu	$2,$2,%lo(__tasklet_schedule)
	jalr	$2
$L397:
	lw	$2,16($sp)
	andi	$2,$2,0x2
	beq	$2,$0,$L399
	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lw	$3,288($2)
	addiu	$3,$3,1
	sw	$3,288($2)
$L399:
	lw	$2,16($sp)
	andi	$2,$2,0x20
	beq	$2,$0,$L393
	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lw	$3,292($2)
	addiu	$3,$3,1
	sw	$3,292($2)
$L393:
	lw	$2,16($sp)
	andi	$2,$2,0x1
	beq	$2,$0,$L392
	lw	$2,20($sp)
	addiu	$2,$2,80
	sw	$2,72($sp)
	li	$2,1			# 0x1
	sw	$2,76($sp)
	lw	$2,72($sp)
	lw	$3,76($sp)
	sw	$3,0($2)
	move	$4,$0
	lui	$2,%hi(qdma_bm_transmit_done)
	addiu	$2,$2,%lo(qdma_bm_transmit_done)
	jalr	$2
	lui	$2,%hi(sw_PKTQOS_CLEAR_STOP)
	lw	$2,%lo(sw_PKTQOS_CLEAR_STOP)($2)
	beq	$2,$0,$L400
	lui	$2,%hi(sw_PKTQOS_CLEAR_STOP)
	lw	$2,%lo(sw_PKTQOS_CLEAR_STOP)($2)
	jalr	$2
$L400:
	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lw	$3,296($2)
	addiu	$3,$3,1
	sw	$3,296($2)
$L392:
	lw	$3,16($sp)
	li	$2,16973824			# 0x1030000
	and	$2,$3,$2
	beq	$2,$0,$L401
	lw	$3,16($sp)
	li	$2,16777216			# 0x1000000
	and	$2,$3,$2
	beq	$2,$0,$L402
	lw	$2,20($sp)
	addiu	$2,$2,80
	sw	$2,80($sp)
	li	$2,16777216			# 0x1000000
	sw	$2,84($sp)
	lw	$2,80($sp)
	lw	$3,84($sp)
	sw	$3,0($2)
	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lw	$2,168($2)
	beq	$2,$0,$L402
	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lw	$2,168($2)
	jalr	$2
$L402:
	lw	$3,16($sp)
	li	$2,131072			# 0x20000
	and	$2,$3,$2
	beq	$2,$0,$L403
	lw	$2,20($sp)
	addiu	$2,$2,80
	sw	$2,88($sp)
	li	$2,131072			# 0x20000
	sw	$2,92($sp)
	lw	$2,88($sp)
	lw	$3,92($sp)
	sw	$3,0($2)
	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lw	$2,164($2)
	beq	$2,$0,$L403
	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lw	$2,164($2)
	jalr	$2
$L403:
	lw	$3,16($sp)
	li	$2,65536			# 0x10000
	and	$2,$3,$2
	beq	$2,$0,$L401
	lw	$2,20($sp)
	addiu	$2,$2,80
	sw	$2,96($sp)
	li	$2,65536			# 0x10000
	sw	$2,100($sp)
	lw	$2,96($sp)
	lw	$3,100($sp)
	sw	$3,0($2)
	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lw	$2,160($2)
	beq	$2,$0,$L401
	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lw	$2,160($2)
	jalr	$2
$L401:
	lw	$2,16($sp)
	andi	$2,$2,0xf7cc
	beq	$2,$0,$L404
	lw	$2,16($sp)
	andi	$2,$2,0x200
	beq	$2,$0,$L405
	lw	$2,20($sp)
	addiu	$2,$2,80
	sw	$2,104($sp)
	li	$2,512			# 0x200
	sw	$2,108($sp)
	lw	$2,104($sp)
	lw	$3,108($sp)
	sw	$3,0($2)
	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lhu	$3,180($2)
	addiu	$3,$3,1
	andi	$3,$3,0xffff
	sh	$3,180($2)
$L405:
	lw	$2,16($sp)
	andi	$2,$2,0x2000
	beq	$2,$0,$L406
	lw	$2,20($sp)
	addiu	$2,$2,80
	sw	$2,112($sp)
	li	$2,8192			# 0x2000
	sw	$2,116($sp)
	lw	$2,112($sp)
	lw	$3,116($sp)
	sw	$3,0($2)
	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lhu	$3,252($2)
	addiu	$3,$3,1
	andi	$3,$3,0xffff
	sh	$3,252($2)
$L406:
	lw	$2,16($sp)
	andi	$2,$2,0x8000
	beq	$2,$0,$L407
	lw	$2,20($sp)
	addiu	$2,$2,80
	sw	$2,120($sp)
	li	$2,32768			# 0x8000
	sw	$2,124($sp)
	lw	$2,120($sp)
	lw	$3,124($sp)
	sw	$3,0($2)
	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lhu	$3,254($2)
	addiu	$3,$3,1
	andi	$3,$3,0xffff
	sh	$3,254($2)
$L407:
	lw	$2,16($sp)
	andi	$2,$2,0x1000
	beq	$2,$0,$L408
	lw	$2,20($sp)
	addiu	$2,$2,80
	sw	$2,128($sp)
	li	$2,4096			# 0x1000
	sw	$2,132($sp)
	lw	$2,128($sp)
	lw	$3,132($sp)
	sw	$3,0($2)
	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lhu	$3,256($2)
	addiu	$3,$3,1
	andi	$3,$3,0xffff
	sh	$3,256($2)
$L408:
	lw	$2,16($sp)
	andi	$2,$2,0x4000
	beq	$2,$0,$L409
	lw	$2,20($sp)
	addiu	$2,$2,80
	sw	$2,136($sp)
	li	$2,16384			# 0x4000
	sw	$2,140($sp)
	lw	$2,136($sp)
	lw	$3,140($sp)
	sw	$3,0($2)
	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lhu	$3,258($2)
	addiu	$3,$3,1
	andi	$3,$3,0xffff
	sh	$3,258($2)
$L409:
	lw	$2,16($sp)
	andi	$2,$2,0x100
	beq	$2,$0,$L410
	lw	$2,20($sp)
	addiu	$2,$2,80
	sw	$2,144($sp)
	li	$2,256			# 0x100
	sw	$2,148($sp)
	lw	$2,144($sp)
	lw	$3,148($sp)
	sw	$3,0($2)
	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lw	$2,152($2)
	beq	$2,$0,$L411
	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lw	$2,152($2)
	li	$4,2			# 0x2
	jalr	$2
$L411:
	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lhu	$3,184($2)
	addiu	$3,$3,1
	andi	$3,$3,0xffff
	sh	$3,184($2)
$L410:
	lw	$2,16($sp)
	andi	$2,$2,0x400
	beq	$2,$0,$L412
	lw	$2,20($sp)
	addiu	$2,$2,80
	sw	$2,152($sp)
	li	$2,1024			# 0x400
	sw	$2,156($sp)
	lw	$2,152($sp)
	lw	$3,156($sp)
	sw	$3,0($2)
	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lw	$2,152($2)
	beq	$2,$0,$L413
	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lw	$2,152($2)
	li	$4,2			# 0x2
	jalr	$2
$L413:
	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lhu	$3,186($2)
	addiu	$3,$3,1
	andi	$3,$3,0xffff
	sh	$3,186($2)
$L412:
	lw	$2,16($sp)
	andi	$2,$2,0x8
	bne	$2,$0,$L414
	lw	$2,16($sp)
	andi	$2,$2,0x80
	beq	$2,$0,$L415
$L414:
	lw	$2,20($sp)
	addiu	$2,$2,80
	sw	$2,160($sp)
	li	$2,136			# 0x88
	sw	$2,164($sp)
	lw	$2,160($sp)
	lw	$3,164($sp)
	sw	$3,0($2)
	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lw	$3,96($2)
	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lw	$2,104($2)
	beq	$3,$2,$L416
	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lw	$3,100($2)
	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lw	$2,108($2)
	bne	$3,$2,$L417
$L416:
	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lw	$2,152($2)
	beq	$2,$0,$L417
	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lw	$2,152($2)
	li	$4,1			# 0x1
	jalr	$2
$L417:
	lw	$2,16($sp)
	andi	$2,$2,0x8
	beq	$2,$0,$L418
	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lhu	$3,268($2)
	addiu	$3,$3,1
	andi	$3,$3,0xffff
	sh	$3,268($2)
$L418:
	lw	$2,16($sp)
	andi	$2,$2,0x80
	beq	$2,$0,$L415
	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lhu	$3,270($2)
	addiu	$3,$3,1
	andi	$3,$3,0xffff
	sh	$3,270($2)
$L415:
	lw	$2,16($sp)
	andi	$2,$2,0x4
	bne	$2,$0,$L419
	lw	$2,16($sp)
	andi	$2,$2,0x40
	beq	$2,$0,$L404
$L419:
	lw	$2,20($sp)
	addiu	$2,$2,80
	sw	$2,168($sp)
	li	$2,68			# 0x44
	sw	$2,172($sp)
	lw	$2,168($sp)
	lw	$3,172($sp)
	sw	$3,0($2)
	lw	$2,16($sp)
	andi	$2,$2,0x4
	beq	$2,$0,$L420
	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lhu	$3,272($2)
	addiu	$3,$3,1
	andi	$3,$3,0xffff
	sh	$3,272($2)
$L420:
	lw	$2,16($sp)
	andi	$2,$2,0x40
	beq	$2,$0,$L404
	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lhu	$3,274($2)
	addiu	$3,$3,1
	andi	$3,$3,0xffff
	sh	$3,274($2)
$L404:
	li	$2,1			# 0x1
	lw	$31,180($sp)
	lw	$16,176($sp)
	addiu	$sp,$sp,184
	j	$31
	.end	qdma_isr
	.size	qdma_isr, .-qdma_isr
	.globl	ecnt_driver_qdma_op
	.rdata
	.align	2
$LC67:
	.ascii	"driver_qdma_lan_hook\000"
	.data
	.align	2
	.type	ecnt_driver_qdma_op, @object
	.size	ecnt_driver_qdma_op, 40
ecnt_driver_qdma_op:
	.space	16
	.word	$LC67
	.word	1
	.word	ecnt_qdma_lan_hook
	.word	17
	.word	0
	.word	1
	.rdata
	.align	2
$LC68:
	.ascii	"%s: physical_size is 0x%08x\012\000"
	.align	2
$LC69:
	.ascii	"qdma_lan\000"
	.section	.text.qdma_bm_dscp_init,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	qdma_bm_dscp_init
	.type	qdma_bm_dscp_init, @function
qdma_bm_dscp_init:
	.frame	$sp,568,$31		# vars= 544, regs= 2/0, args= 16, gp= 0
	.mask	0x80010000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-568
	sw	$31,564($sp)
	sw	$16,560($sp)
	sw	$4,568($sp)
	sw	$5,572($sp)
	sw	$6,576($sp)
	sw	$7,580($sp)
	sw	$0,32($sp)
	sw	$0,524($sp)
	sw	$0,528($sp)
	sw	$0,532($sp)
	sw	$0,536($sp)
	sw	$0,36($sp)
	sw	$0,40($sp)
	sw	$0,44($sp)
	sw	$0,16($sp)
	sw	$0,48($sp)
	sw	$0,20($sp)
	sw	$0,52($sp)
	sw	$0,56($sp)
	sw	$0,60($sp)
	sw	$0,64($sp)
	sw	$0,24($sp)
	sw	$0,28($sp)
	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lw	$2,0($2)
	sw	$2,48($sp)
	lhu	$2,570($sp)
	sw	$2,544($sp)
	lhu	$2,568($sp)
	sw	$2,540($sp)
	lhu	$2,574($sp)
	sw	$2,552($sp)
	lhu	$2,572($sp)
	sw	$2,548($sp)
	lhu	$2,576($sp)
	sw	$2,52($sp)
	lhu	$2,578($sp)
	sw	$2,56($sp)
	lhu	$2,580($sp)
	sw	$2,60($sp)
	lw	$2,544($sp)
	sltu	$2,$2,4097
	beq	$2,$0,$L423
	nop

	lw	$2,540($sp)
	sltu	$2,$2,4097
	beq	$2,$0,$L423
	nop

	lw	$2,552($sp)
	sltu	$2,$2,4097
	beq	$2,$0,$L423
	nop

	lw	$2,548($sp)
	sltu	$2,$2,4097
	beq	$2,$0,$L423
	nop

	lw	$2,52($sp)
	sltu	$2,$2,4097
	beq	$2,$0,$L423
	nop

	lw	$2,56($sp)
	sltu	$2,$2,4096
	beq	$2,$0,$L423
	nop

	lw	$2,60($sp)
	sltu	$2,$2,2049
	beq	$2,$0,$L423
	nop

	lw	$2,60($sp)
	sltu	$2,$2,48
	beq	$2,$0,$L424
	nop

$L423:
	li	$2,-14			# 0xfffffffffffffff2
	j	$L500
	nop

$L424:
	lw	$2,60($sp)
	andi	$2,$2,0x3
	beq	$2,$0,$L426
	nop

	li	$2,-14			# 0xfffffffffffffff2
	j	$L500
	nop

$L426:
	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	addiu	$2,$2,20
	move	$4,$2
	addiu	$5,$sp,568
	li	$6,14			# 0xe
	lui	$2,%hi(memcpy)
	addiu	$2,$2,%lo(memcpy)
	jalr	$2
	nop

	lw	$3,544($sp)
	lw	$2,540($sp)
	addu	$3,$3,$2
	lw	$2,552($sp)
	addu	$3,$3,$2
	lw	$2,548($sp)
	addu	$2,$3,$2
	sll	$3,$2,5
	addiu	$2,$sp,524
	move	$4,$0
	move	$5,$3
	move	$6,$2
	li	$7,208			# 0xd0
	lui	$2,%hi(dma_alloc_coherent)
	addiu	$2,$2,%lo(dma_alloc_coherent)
	jalr	$2
	nop

	sw	$2,36($sp)
	lw	$2,36($sp)
	bne	$2,$0,$L427
	nop

	li	$2,-12			# 0xfffffffffffffff4
	j	$L500
	nop

$L427:
	lw	$2,48($sp)
	addiu	$3,$2,264
	lw	$2,524($sp)
	sw	$3,144($sp)
	sw	$2,148($sp)
	lw	$2,144($sp)
	lw	$3,148($sp)
	sw	$3,0($2)
	lw	$2,48($sp)
	addiu	$3,$2,8
	lw	$2,544($sp)
	sll	$4,$2,5
	lw	$2,524($sp)
	addu	$2,$4,$2
	sw	$3,152($sp)
	sw	$2,156($sp)
	lw	$2,152($sp)
	lw	$3,156($sp)
	sw	$3,0($2)
	lw	$2,48($sp)
	addiu	$3,$2,268
	lw	$4,544($sp)
	lw	$2,540($sp)
	addu	$2,$4,$2
	sll	$4,$2,5
	lw	$2,524($sp)
	addu	$2,$4,$2
	sw	$3,160($sp)
	sw	$2,164($sp)
	lw	$2,160($sp)
	lw	$3,164($sp)
	sw	$3,0($2)
	lw	$2,48($sp)
	addiu	$3,$2,12
	lw	$4,544($sp)
	lw	$2,540($sp)
	addu	$4,$4,$2
	lw	$2,552($sp)
	addu	$2,$4,$2
	sll	$4,$2,5
	lw	$2,524($sp)
	addu	$2,$4,$2
	sw	$3,168($sp)
	sw	$2,172($sp)
	lw	$2,168($sp)
	lw	$3,172($sp)
	sw	$3,0($2)
	lw	$2,48($sp)
	addiu	$2,$2,256
	sw	$2,176($sp)
	lw	$2,176($sp)
	lw	$2,0($2)
	sw	$2,68($sp)
	lw	$2,48($sp)
	addiu	$3,$2,256
	lw	$4,68($sp)
	li	$2,-268435456			# 0xfffffffff0000000
	ori	$2,$2,0xffff
	and	$4,$4,$2
	lw	$2,552($sp)
	sll	$5,$2,16
	li	$2,268369920			# 0xfff0000
	and	$2,$5,$2
	or	$2,$4,$2
	sw	$3,180($sp)
	sw	$2,184($sp)
	lw	$2,180($sp)
	lw	$3,184($sp)
	sw	$3,0($2)
	lw	$2,48($sp)
	addiu	$2,$2,256
	sw	$2,188($sp)
	lw	$2,188($sp)
	lw	$2,0($2)
	sw	$2,72($sp)
	lw	$2,48($sp)
	addiu	$3,$2,256
	lw	$4,72($sp)
	li	$2,-4096			# 0xfffffffffffff000
	and	$4,$4,$2
	lw	$2,548($sp)
	andi	$2,$2,0xfff
	or	$2,$4,$2
	sw	$3,192($sp)
	sw	$2,196($sp)
	lw	$2,192($sp)
	lw	$3,196($sp)
	sw	$3,0($2)
	lw	$2,48($sp)
	addiu	$2,$2,260
	sw	$2,200($sp)
	lw	$2,200($sp)
	lw	$2,0($2)
	sw	$2,76($sp)
	lw	$2,48($sp)
	addiu	$3,$2,260
	lw	$4,76($sp)
	li	$2,-268435456			# 0xfffffffff0000000
	ori	$2,$2,0xffff
	and	$4,$4,$2
	li	$2,2097152			# 0x200000
	or	$2,$4,$2
	sw	$3,204($sp)
	sw	$2,208($sp)
	lw	$2,204($sp)
	lw	$3,208($sp)
	sw	$3,0($2)
	lw	$2,48($sp)
	addiu	$2,$2,48
	sw	$2,212($sp)
	lw	$2,212($sp)
	lw	$2,0($2)
	sw	$2,80($sp)
	lw	$2,48($sp)
	addiu	$3,$2,48
	lw	$4,80($sp)
	li	$2,-16777216			# 0xffffffffff000000
	ori	$2,$2,0xffff
	and	$4,$4,$2
	li	$2,1310720			# 0x140000
	or	$2,$4,$2
	sw	$3,216($sp)
	sw	$2,220($sp)
	lw	$2,216($sp)
	lw	$3,220($sp)
	sw	$3,0($2)
	lw	$2,48($sp)
	addiu	$2,$2,260
	sw	$2,224($sp)
	lw	$2,224($sp)
	lw	$2,0($2)
	sw	$2,84($sp)
	lw	$2,48($sp)
	addiu	$3,$2,260
	lw	$4,84($sp)
	li	$2,-4096			# 0xfffffffffffff000
	and	$4,$4,$2
	lw	$2,548($sp)
	srl	$2,$2,2
	andi	$2,$2,0xfff
	or	$2,$4,$2
	sw	$3,228($sp)
	sw	$2,232($sp)
	lw	$2,228($sp)
	lw	$3,232($sp)
	sw	$3,0($2)
	lw	$2,56($sp)
	beq	$2,$0,$L433
	nop

	lui	$2,%hi(gpQdmaPriv)
	lw	$16,%lo(gpQdmaPriv)($2)
	lw	$2,56($sp)
	sll	$3,$2,2
	addiu	$2,$sp,528
	move	$4,$0
	move	$5,$3
	move	$6,$2
	li	$7,208			# 0xd0
	lui	$2,%hi(dma_alloc_coherent)
	addiu	$2,$2,%lo(dma_alloc_coherent)
	jalr	$2
	nop

	sw	$2,56($16)
	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lw	$2,56($2)
	bne	$2,$0,$L434
	nop

	li	$2,-12			# 0xfffffffffffffff4
	j	$L500
	nop

$L434:
	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lw	$2,56($2)
	move	$3,$2
	lw	$2,56($sp)
	sll	$2,$2,2
	move	$4,$3
	li	$5,-1			# 0xffffffffffffffff
	move	$6,$2
	lui	$2,%hi(memset)
	addiu	$2,$2,%lo(memset)
	jalr	$2
	nop

	lw	$2,48($sp)
	addiu	$3,$2,96
	lw	$2,528($sp)
	sw	$3,236($sp)
	sw	$2,240($sp)
	lw	$2,236($sp)
	lw	$3,240($sp)
	sw	$3,0($2)
	lw	$2,48($sp)
	addiu	$2,$2,100
	sw	$2,244($sp)
	lw	$2,244($sp)
	lw	$2,0($2)
	sw	$2,88($sp)
	lw	$2,48($sp)
	addiu	$3,$2,100
	lw	$4,88($sp)
	li	$2,-4096			# 0xfffffffffffff000
	and	$4,$4,$2
	lw	$2,56($sp)
	andi	$2,$2,0xfff
	or	$2,$4,$2
	sw	$3,248($sp)
	sw	$2,252($sp)
	lw	$2,248($sp)
	lw	$3,252($sp)
	sw	$3,0($2)
	lw	$2,48($sp)
	addiu	$2,$2,100
	sw	$2,256($sp)
	lw	$2,256($sp)
	lw	$2,0($2)
	sw	$2,92($sp)
	lw	$2,48($sp)
	addiu	$3,$2,100
	lw	$4,92($sp)
	li	$2,-268435456			# 0xfffffffff0000000
	ori	$2,$2,0xffff
	and	$4,$4,$2
	li	$2,1048576			# 0x100000
	or	$2,$4,$2
	sw	$3,260($sp)
	sw	$2,264($sp)
	lw	$2,260($sp)
	lw	$3,264($sp)
	sw	$3,0($2)
$L433:
	lui	$2,%hi(gpQdmaPriv)
	lw	$16,%lo(gpQdmaPriv)($2)
	lw	$3,544($sp)
	lw	$2,540($sp)
	addu	$3,$3,$2
	lw	$2,552($sp)
	addu	$3,$3,$2
	lw	$2,548($sp)
	addu	$2,$3,$2
	sll	$2,$2,4
	sw	$2,268($sp)
	li	$2,208			# 0xd0
	sw	$2,272($sp)
	lw	$2,272($sp)
	ori	$2,$2,0x8000
	lw	$3,268($sp)
	sw	$3,276($sp)
	sw	$2,280($sp)
$L437 = .
	lw	$4,276($sp)
	lw	$5,280($sp)
	lui	$2,%hi(__kmalloc)
	addiu	$2,$2,%lo(__kmalloc)
	jalr	$2
	nop

	sw	$2,36($16)
	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lw	$2,36($2)
	bne	$2,$0,$L440
	nop

	li	$2,-12			# 0xfffffffffffffff4
	j	$L500
	nop

$L440:
	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lui	$3,%hi(gpQdmaPriv)
	lw	$3,%lo(gpQdmaPriv)($3)
	lw	$3,36($3)
	sw	$3,44($2)
	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lui	$3,%hi(gpQdmaPriv)
	lw	$3,%lo(gpQdmaPriv)($3)
	lw	$4,36($3)
	lw	$3,544($sp)
	sll	$3,$3,4
	addu	$3,$4,$3
	sw	$3,40($2)
	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lui	$3,%hi(gpQdmaPriv)
	lw	$3,%lo(gpQdmaPriv)($3)
	lw	$4,36($3)
	lw	$5,544($sp)
	lw	$3,540($sp)
	addu	$3,$5,$3
	sll	$3,$3,4
	addu	$3,$4,$3
	sw	$3,52($2)
	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lui	$3,%hi(gpQdmaPriv)
	lw	$3,%lo(gpQdmaPriv)($3)
	lw	$4,36($3)
	lw	$5,544($sp)
	lw	$3,540($sp)
	addu	$5,$5,$3
	lw	$3,552($sp)
	addu	$3,$5,$3
	sll	$3,$3,4
	addu	$3,$4,$3
	sw	$3,48($2)
	sw	$0,16($sp)
	j	$L441
	nop

$L457:
	lw	$2,16($sp)
	sll	$3,$2,4
	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lw	$2,36($2)
	addu	$2,$3,$2
	sw	$2,32($sp)
	lw	$2,16($sp)
	sll	$3,$2,5
	lw	$2,36($sp)
	addu	$2,$3,$2
	move	$3,$2
	lw	$2,32($sp)
	sw	$3,0($2)
	lw	$3,544($sp)
	lw	$2,16($sp)
	sltu	$2,$2,$3
	beq	$2,$0,$L442
	nop

	lw	$2,32($sp)
	lw	$3,16($sp)
	sw	$3,4($2)
	lw	$2,32($sp)
	sw	$0,12($2)
	lw	$2,32($sp)
	lw	$3,0($2)
	lw	$2,4($3)
	li	$4,1			# 0x1
	ins	$2,$4,31,1
	sw	$2,4($3)
	lw	$2,32($sp)
	sw	$2,284($sp)
	li	$2,1			# 0x1
	sw	$2,288($sp)
	lw	$2,284($sp)
	lw	$2,12($2)
	bne	$2,$0,$L447
	nop

	lw	$2,284($sp)
	sw	$0,8($2)
	lui	$2,%hi(gpQdmaPriv)
	lw	$3,%lo(gpQdmaPriv)($2)
	lw	$2,288($sp)
	addiu	$2,$2,18
	sll	$2,$2,2
	addu	$2,$3,$2
	lw	$2,0($2)
	bne	$2,$0,$L445
	nop

	lui	$2,%hi(gpQdmaPriv)
	lw	$3,%lo(gpQdmaPriv)($2)
	lw	$2,288($sp)
	addiu	$2,$2,18
	sll	$2,$2,2
	addu	$2,$3,$2
	lw	$3,284($sp)
	sw	$3,0($2)
	lui	$2,%hi(gpQdmaPriv)
	lw	$3,%lo(gpQdmaPriv)($2)
	lw	$2,288($sp)
	addiu	$2,$2,20
	sll	$2,$2,2
	addu	$2,$3,$2
	lw	$3,284($sp)
	sw	$3,0($2)
	j	$L447
	nop

$L445:
	lui	$2,%hi(gpQdmaPriv)
	lw	$3,%lo(gpQdmaPriv)($2)
	lw	$2,288($sp)
	addiu	$2,$2,20
	sll	$2,$2,2
	addu	$2,$3,$2
	lw	$2,0($2)
	lw	$3,284($sp)
	sw	$3,12($2)
	lui	$2,%hi(gpQdmaPriv)
	lw	$4,%lo(gpQdmaPriv)($2)
	lui	$2,%hi(gpQdmaPriv)
	lw	$3,%lo(gpQdmaPriv)($2)
	lw	$2,288($sp)
	addiu	$2,$2,20
	sll	$2,$2,2
	addu	$2,$3,$2
	lw	$2,0($2)
	lw	$3,12($2)
	lw	$2,288($sp)
	addiu	$2,$2,20
	sll	$2,$2,2
	addu	$2,$4,$2
	sw	$3,0($2)
	j	$L447
	nop

$L442:
	lw	$3,544($sp)
	lw	$2,540($sp)
	addu	$3,$3,$2
	lw	$2,16($sp)
	sltu	$2,$2,$3
	beq	$2,$0,$L448
	nop

	lw	$2,544($sp)
	lw	$3,16($sp)
	subu	$3,$3,$2
	lw	$2,32($sp)
	sw	$3,4($2)
	lw	$2,32($sp)
	sw	$0,12($2)
	lw	$2,32($sp)
	lw	$3,0($2)
	lw	$2,4($3)
	li	$4,1			# 0x1
	ins	$2,$4,31,1
	sw	$2,4($3)
	lw	$2,32($sp)
	sw	$2,292($sp)
	sw	$0,296($sp)
	lw	$2,292($sp)
	lw	$2,12($2)
	bne	$2,$0,$L447
	nop

	lw	$2,292($sp)
	sw	$0,8($2)
	lui	$2,%hi(gpQdmaPriv)
	lw	$3,%lo(gpQdmaPriv)($2)
	lw	$2,296($sp)
	addiu	$2,$2,18
	sll	$2,$2,2
	addu	$2,$3,$2
	lw	$2,0($2)
	bne	$2,$0,$L451
	nop

	lui	$2,%hi(gpQdmaPriv)
	lw	$3,%lo(gpQdmaPriv)($2)
	lw	$2,296($sp)
	addiu	$2,$2,18
	sll	$2,$2,2
	addu	$2,$3,$2
	lw	$3,292($sp)
	sw	$3,0($2)
	lui	$2,%hi(gpQdmaPriv)
	lw	$3,%lo(gpQdmaPriv)($2)
	lw	$2,296($sp)
	addiu	$2,$2,20
	sll	$2,$2,2
	addu	$2,$3,$2
	lw	$3,292($sp)
	sw	$3,0($2)
	j	$L447
	nop

$L451:
	lui	$2,%hi(gpQdmaPriv)
	lw	$3,%lo(gpQdmaPriv)($2)
	lw	$2,296($sp)
	addiu	$2,$2,20
	sll	$2,$2,2
	addu	$2,$3,$2
	lw	$2,0($2)
	lw	$3,292($sp)
	sw	$3,12($2)
	lui	$2,%hi(gpQdmaPriv)
	lw	$4,%lo(gpQdmaPriv)($2)
	lui	$2,%hi(gpQdmaPriv)
	lw	$3,%lo(gpQdmaPriv)($2)
	lw	$2,296($sp)
	addiu	$2,$2,20
	sll	$2,$2,2
	addu	$2,$3,$2
	lw	$2,0($2)
	lw	$3,12($2)
	lw	$2,296($sp)
	addiu	$2,$2,20
	sll	$2,$2,2
	addu	$2,$4,$2
	sw	$3,0($2)
	j	$L447
	nop

$L448:
	lw	$3,544($sp)
	lw	$2,540($sp)
	addu	$3,$3,$2
	lw	$2,552($sp)
	addu	$3,$3,$2
	lw	$2,16($sp)
	sltu	$2,$2,$3
	beq	$2,$0,$L453
	nop

	lw	$2,544($sp)
	lw	$3,16($sp)
	subu	$3,$3,$2
	lw	$2,540($sp)
	subu	$3,$3,$2
	lw	$2,32($sp)
	sw	$3,4($2)
	lw	$2,32($sp)
	sw	$2,300($sp)
	li	$2,1			# 0x1
	sw	$2,304($sp)
	lui	$2,%hi(gpQdmaPriv)
	lw	$3,%lo(gpQdmaPriv)($2)
	lw	$2,304($sp)
	addiu	$2,$2,24
	sll	$2,$2,2
	addu	$2,$3,$2
	lw	$2,0($2)
	bne	$2,$0,$L454
	nop

	lui	$2,%hi(gpQdmaPriv)
	lw	$3,%lo(gpQdmaPriv)($2)
	lw	$2,304($sp)
	addiu	$2,$2,24
	sll	$2,$2,2
	addu	$2,$3,$2
	lw	$3,300($sp)
	sw	$3,0($2)
	lui	$2,%hi(gpQdmaPriv)
	lw	$3,%lo(gpQdmaPriv)($2)
	lw	$2,304($sp)
	addiu	$2,$2,24
	sll	$2,$2,2
	addu	$2,$3,$2
	lw	$3,0($2)
	lw	$2,300($sp)
	sw	$3,12($2)
	j	$L447
	nop

$L454:
	lui	$2,%hi(gpQdmaPriv)
	lw	$3,%lo(gpQdmaPriv)($2)
	lw	$2,304($sp)
	addiu	$2,$2,24
	sll	$2,$2,2
	addu	$2,$3,$2
	lw	$2,0($2)
	lw	$3,12($2)
	lw	$2,300($sp)
	sw	$3,12($2)
	lui	$2,%hi(gpQdmaPriv)
	lw	$3,%lo(gpQdmaPriv)($2)
	lw	$2,304($sp)
	addiu	$2,$2,24
	sll	$2,$2,2
	addu	$2,$3,$2
	lw	$2,0($2)
	lw	$3,300($sp)
	sw	$3,12($2)
	lui	$2,%hi(gpQdmaPriv)
	lw	$3,%lo(gpQdmaPriv)($2)
	lw	$2,304($sp)
	addiu	$2,$2,24
	sll	$2,$2,2
	addu	$2,$3,$2
	lw	$3,300($sp)
	sw	$3,0($2)
	j	$L447
	nop

$L453:
	lw	$2,544($sp)
	lw	$3,16($sp)
	subu	$3,$3,$2
	lw	$2,540($sp)
	subu	$3,$3,$2
	lw	$2,552($sp)
	subu	$3,$3,$2
	lw	$2,32($sp)
	sw	$3,4($2)
	lw	$2,32($sp)
	sw	$2,308($sp)
	sw	$0,312($sp)
	lui	$2,%hi(gpQdmaPriv)
	lw	$3,%lo(gpQdmaPriv)($2)
	lw	$2,312($sp)
	addiu	$2,$2,24
	sll	$2,$2,2
	addu	$2,$3,$2
	lw	$2,0($2)
	bne	$2,$0,$L456
	nop

	lui	$2,%hi(gpQdmaPriv)
	lw	$3,%lo(gpQdmaPriv)($2)
	lw	$2,312($sp)
	addiu	$2,$2,24
	sll	$2,$2,2
	addu	$2,$3,$2
	lw	$3,308($sp)
	sw	$3,0($2)
	lui	$2,%hi(gpQdmaPriv)
	lw	$3,%lo(gpQdmaPriv)($2)
	lw	$2,312($sp)
	addiu	$2,$2,24
	sll	$2,$2,2
	addu	$2,$3,$2
	lw	$3,0($2)
	lw	$2,308($sp)
	sw	$3,12($2)
	j	$L447
	nop

$L456:
	lui	$2,%hi(gpQdmaPriv)
	lw	$3,%lo(gpQdmaPriv)($2)
	lw	$2,312($sp)
	addiu	$2,$2,24
	sll	$2,$2,2
	addu	$2,$3,$2
	lw	$2,0($2)
	lw	$3,12($2)
	lw	$2,308($sp)
	sw	$3,12($2)
	lui	$2,%hi(gpQdmaPriv)
	lw	$3,%lo(gpQdmaPriv)($2)
	lw	$2,312($sp)
	addiu	$2,$2,24
	sll	$2,$2,2
	addu	$2,$3,$2
	lw	$2,0($2)
	lw	$3,308($sp)
	sw	$3,12($2)
	lui	$2,%hi(gpQdmaPriv)
	lw	$3,%lo(gpQdmaPriv)($2)
	lw	$2,312($sp)
	addiu	$2,$2,24
	sll	$2,$2,2
	addu	$2,$3,$2
	lw	$3,308($sp)
	sw	$3,0($2)
$L447:
	lw	$2,16($sp)
	addiu	$2,$2,1
	sw	$2,16($sp)
$L441:
	lw	$3,544($sp)
	lw	$2,540($sp)
	addu	$3,$3,$2
	lw	$2,552($sp)
	addu	$3,$3,$2
	lw	$2,548($sp)
	addu	$3,$3,$2
	lw	$2,16($sp)
	sltu	$2,$2,$3
	bne	$2,$0,$L457
	nop

	li	$2,1			# 0x1
	sw	$2,316($sp)
	sw	$0,320($sp)
	lui	$2,%hi(gpQdmaPriv)
	lw	$3,%lo(gpQdmaPriv)($2)
	lw	$2,316($sp)
	addiu	$2,$2,18
	sll	$2,$2,2
	addu	$2,$3,$2
	lw	$2,0($2)
	sw	$2,320($sp)
	lui	$2,%hi(gpQdmaPriv)
	lw	$3,%lo(gpQdmaPriv)($2)
	lw	$2,316($sp)
	addiu	$2,$2,18
	sll	$2,$2,2
	addu	$2,$3,$2
	lw	$3,0($2)
	lui	$2,%hi(gpQdmaPriv)
	lw	$4,%lo(gpQdmaPriv)($2)
	lw	$2,316($sp)
	addiu	$2,$2,20
	sll	$2,$2,2
	addu	$2,$4,$2
	lw	$2,0($2)
	bne	$3,$2,$L458
	nop

	lui	$2,%hi(gpQdmaPriv)
	lw	$3,%lo(gpQdmaPriv)($2)
	lw	$2,316($sp)
	addiu	$2,$2,18
	sll	$2,$2,2
	addu	$2,$3,$2
	sw	$0,0($2)
	lui	$2,%hi(gpQdmaPriv)
	lw	$3,%lo(gpQdmaPriv)($2)
	lw	$2,316($sp)
	addiu	$2,$2,20
	sll	$2,$2,2
	addu	$2,$3,$2
	sw	$0,0($2)
	j	$L459
	nop

$L458:
	lui	$2,%hi(gpQdmaPriv)
	lw	$4,%lo(gpQdmaPriv)($2)
	lui	$2,%hi(gpQdmaPriv)
	lw	$3,%lo(gpQdmaPriv)($2)
	lw	$2,316($sp)
	addiu	$2,$2,18
	sll	$2,$2,2
	addu	$2,$3,$2
	lw	$2,0($2)
	lw	$3,12($2)
	lw	$2,316($sp)
	addiu	$2,$2,18
	sll	$2,$2,2
	addu	$2,$4,$2
	sw	$3,0($2)
$L459:
	lw	$2,320($sp)
	beq	$2,$0,$L460
	nop

	lw	$2,320($sp)
	sw	$0,12($2)
$L460:
	lw	$2,320($sp)
	sw	$2,32($sp)
	lw	$2,32($sp)
	bne	$2,$0,$L462
	nop

	li	$2,-63			# 0xffffffffffffffc1
	j	$L500
	nop

$L462:
	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lw	$3,32($sp)
	sw	$3,92($2)
	lw	$2,48($sp)
	addiu	$2,$2,272
	sw	$2,324($sp)
	lw	$2,324($sp)
	lw	$2,0($2)
	sw	$2,96($sp)
	lw	$2,48($sp)
	addiu	$3,$2,272
	lw	$4,96($sp)
	li	$2,-4096			# 0xfffffffffffff000
	and	$4,$4,$2
	lw	$2,32($sp)
	lw	$2,4($2)
	andi	$2,$2,0xfff
	or	$2,$4,$2
	sw	$3,328($sp)
	sw	$2,332($sp)
	lw	$2,328($sp)
	lw	$3,332($sp)
	sw	$3,0($2)
	lw	$2,48($sp)
	addiu	$2,$2,276
	sw	$2,336($sp)
	lw	$2,336($sp)
	lw	$2,0($2)
	sw	$2,100($sp)
	lw	$2,48($sp)
	addiu	$3,$2,276
	lw	$4,100($sp)
	li	$2,-4096			# 0xfffffffffffff000
	and	$4,$4,$2
	lw	$2,32($sp)
	lw	$2,4($2)
	andi	$2,$2,0xfff
	or	$2,$4,$2
	sw	$3,340($sp)
	sw	$2,344($sp)
	lw	$2,340($sp)
	lw	$3,344($sp)
	sw	$3,0($2)
	li	$2,1			# 0x1
	sw	$2,348($sp)
	sw	$0,352($sp)
	sw	$0,356($sp)
	lui	$2,%hi(gpQdmaPriv)
	lw	$3,%lo(gpQdmaPriv)($2)
	lw	$2,348($sp)
	addiu	$2,$2,24
	sll	$2,$2,2
	addu	$2,$3,$2
	lw	$2,0($2)
	beq	$2,$0,$L465
	nop

	lui	$2,%hi(gpQdmaPriv)
	lw	$3,%lo(gpQdmaPriv)($2)
	lw	$2,348($sp)
	addiu	$2,$2,26
	sll	$2,$2,2
	addu	$2,$3,$2
	lw	$2,0($2)
	bne	$2,$0,$L466
	nop

	lui	$2,%hi(gpQdmaPriv)
	lw	$3,%lo(gpQdmaPriv)($2)
	lw	$2,348($sp)
	addiu	$2,$2,24
	sll	$2,$2,2
	addu	$2,$3,$2
	lw	$2,0($2)
	sw	$2,352($sp)
	lui	$2,%hi(gpQdmaPriv)
	lw	$3,%lo(gpQdmaPriv)($2)
	lw	$2,348($sp)
	addiu	$2,$2,26
	sll	$2,$2,2
	addu	$2,$3,$2
	lw	$3,352($sp)
	sw	$3,0($2)
	j	$L465
	nop

$L466:
	lui	$2,%hi(gpQdmaPriv)
	lw	$3,%lo(gpQdmaPriv)($2)
	lw	$2,348($sp)
	addiu	$2,$2,26
	sll	$2,$2,2
	addu	$2,$3,$2
	lw	$2,0($2)
	lw	$3,12($2)
	lui	$2,%hi(gpQdmaPriv)
	lw	$4,%lo(gpQdmaPriv)($2)
	lw	$2,348($sp)
	addiu	$2,$2,24
	sll	$2,$2,2
	addu	$2,$4,$2
	lw	$2,0($2)
	beq	$3,$2,$L465
	nop

	lui	$2,%hi(gpQdmaPriv)
	lw	$3,%lo(gpQdmaPriv)($2)
	lw	$2,348($sp)
	addiu	$2,$2,26
	sll	$2,$2,2
	addu	$2,$3,$2
	lw	$2,0($2)
	lw	$2,12($2)
	sw	$2,352($sp)
	lui	$2,%hi(gpQdmaPriv)
	lw	$3,%lo(gpQdmaPriv)($2)
	lw	$2,348($sp)
	addiu	$2,$2,26
	sll	$2,$2,2
	addu	$2,$3,$2
	lw	$3,352($sp)
	sw	$3,0($2)
$L465:
	lw	$2,352($sp)
	sw	$2,32($sp)
	lw	$2,32($sp)
	bne	$2,$0,$L468
	nop

	li	$2,-63			# 0xffffffffffffffc1
	j	$L500
	nop

$L468:
	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lw	$3,32($sp)
	sw	$3,116($2)
	lw	$2,48($sp)
	addiu	$2,$2,280
	sw	$2,360($sp)
	lw	$2,360($sp)
	lw	$2,0($2)
	sw	$2,104($sp)
	lw	$2,48($sp)
	addiu	$3,$2,280
	lw	$4,104($sp)
	li	$2,-4096			# 0xfffffffffffff000
	and	$4,$4,$2
	lw	$2,32($sp)
	lw	$2,4($2)
	andi	$2,$2,0xfff
	or	$2,$4,$2
	sw	$3,364($sp)
	sw	$2,368($sp)
	lw	$2,364($sp)
	lw	$3,368($sp)
	sw	$3,0($2)
	lw	$2,48($sp)
	addiu	$2,$2,284
	sw	$2,372($sp)
	lw	$2,372($sp)
	lw	$2,0($2)
	sw	$2,108($sp)
	lw	$2,48($sp)
	addiu	$3,$2,284
	lw	$4,108($sp)
	li	$2,-4096			# 0xfffffffffffff000
	and	$4,$4,$2
	lw	$2,32($sp)
	lw	$2,4($2)
	andi	$2,$2,0xfff
	or	$2,$4,$2
	sw	$3,376($sp)
	sw	$2,380($sp)
	lw	$2,376($sp)
	lw	$3,380($sp)
	sw	$3,0($2)
	sw	$0,384($sp)
	sw	$0,388($sp)
	lui	$2,%hi(gpQdmaPriv)
	lw	$3,%lo(gpQdmaPriv)($2)
	lw	$2,384($sp)
	addiu	$2,$2,18
	sll	$2,$2,2
	addu	$2,$3,$2
	lw	$2,0($2)
	sw	$2,388($sp)
	lui	$2,%hi(gpQdmaPriv)
	lw	$3,%lo(gpQdmaPriv)($2)
	lw	$2,384($sp)
	addiu	$2,$2,18
	sll	$2,$2,2
	addu	$2,$3,$2
	lw	$3,0($2)
	lui	$2,%hi(gpQdmaPriv)
	lw	$4,%lo(gpQdmaPriv)($2)
	lw	$2,384($sp)
	addiu	$2,$2,20
	sll	$2,$2,2
	addu	$2,$4,$2
	lw	$2,0($2)
	bne	$3,$2,$L471
	nop

	lui	$2,%hi(gpQdmaPriv)
	lw	$3,%lo(gpQdmaPriv)($2)
	lw	$2,384($sp)
	addiu	$2,$2,18
	sll	$2,$2,2
	addu	$2,$3,$2
	sw	$0,0($2)
	lui	$2,%hi(gpQdmaPriv)
	lw	$3,%lo(gpQdmaPriv)($2)
	lw	$2,384($sp)
	addiu	$2,$2,20
	sll	$2,$2,2
	addu	$2,$3,$2
	sw	$0,0($2)
	j	$L472
	nop

$L471:
	lui	$2,%hi(gpQdmaPriv)
	lw	$4,%lo(gpQdmaPriv)($2)
	lui	$2,%hi(gpQdmaPriv)
	lw	$3,%lo(gpQdmaPriv)($2)
	lw	$2,384($sp)
	addiu	$2,$2,18
	sll	$2,$2,2
	addu	$2,$3,$2
	lw	$2,0($2)
	lw	$3,12($2)
	lw	$2,384($sp)
	addiu	$2,$2,18
	sll	$2,$2,2
	addu	$2,$4,$2
	sw	$3,0($2)
$L472:
	lw	$2,388($sp)
	beq	$2,$0,$L473
	nop

	lw	$2,388($sp)
	sw	$0,12($2)
$L473:
	lw	$2,388($sp)
	sw	$2,32($sp)
	lw	$2,32($sp)
	bne	$2,$0,$L475
	nop

	li	$2,-63			# 0xffffffffffffffc1
	j	$L500
	nop

$L475:
	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lw	$3,32($sp)
	sw	$3,88($2)
	lw	$2,48($sp)
	addiu	$2,$2,16
	sw	$2,392($sp)
	lw	$2,392($sp)
	lw	$2,0($2)
	sw	$2,112($sp)
	lw	$2,48($sp)
	addiu	$3,$2,16
	lw	$4,112($sp)
	li	$2,-4096			# 0xfffffffffffff000
	and	$4,$4,$2
	lw	$2,32($sp)
	lw	$2,4($2)
	andi	$2,$2,0xfff
	or	$2,$4,$2
	sw	$3,396($sp)
	sw	$2,400($sp)
	lw	$2,396($sp)
	lw	$3,400($sp)
	sw	$3,0($2)
	lw	$2,48($sp)
	addiu	$2,$2,20
	sw	$2,404($sp)
	lw	$2,404($sp)
	lw	$2,0($2)
	sw	$2,116($sp)
	lw	$2,48($sp)
	addiu	$3,$2,20
	lw	$4,116($sp)
	li	$2,-4096			# 0xfffffffffffff000
	and	$4,$4,$2
	lw	$2,32($sp)
	lw	$2,4($2)
	andi	$2,$2,0xfff
	or	$2,$4,$2
	sw	$3,408($sp)
	sw	$2,412($sp)
	lw	$2,408($sp)
	lw	$3,412($sp)
	sw	$3,0($2)
	sw	$0,416($sp)
	sw	$0,420($sp)
	sw	$0,424($sp)
	lui	$2,%hi(gpQdmaPriv)
	lw	$3,%lo(gpQdmaPriv)($2)
	lw	$2,416($sp)
	addiu	$2,$2,24
	sll	$2,$2,2
	addu	$2,$3,$2
	lw	$2,0($2)
	beq	$2,$0,$L478
	nop

	lui	$2,%hi(gpQdmaPriv)
	lw	$3,%lo(gpQdmaPriv)($2)
	lw	$2,416($sp)
	addiu	$2,$2,26
	sll	$2,$2,2
	addu	$2,$3,$2
	lw	$2,0($2)
	bne	$2,$0,$L479
	nop

	lui	$2,%hi(gpQdmaPriv)
	lw	$3,%lo(gpQdmaPriv)($2)
	lw	$2,416($sp)
	addiu	$2,$2,24
	sll	$2,$2,2
	addu	$2,$3,$2
	lw	$2,0($2)
	sw	$2,420($sp)
	lui	$2,%hi(gpQdmaPriv)
	lw	$3,%lo(gpQdmaPriv)($2)
	lw	$2,416($sp)
	addiu	$2,$2,26
	sll	$2,$2,2
	addu	$2,$3,$2
	lw	$3,420($sp)
	sw	$3,0($2)
	j	$L478
	nop

$L479:
	lui	$2,%hi(gpQdmaPriv)
	lw	$3,%lo(gpQdmaPriv)($2)
	lw	$2,416($sp)
	addiu	$2,$2,26
	sll	$2,$2,2
	addu	$2,$3,$2
	lw	$2,0($2)
	lw	$3,12($2)
	lui	$2,%hi(gpQdmaPriv)
	lw	$4,%lo(gpQdmaPriv)($2)
	lw	$2,416($sp)
	addiu	$2,$2,24
	sll	$2,$2,2
	addu	$2,$4,$2
	lw	$2,0($2)
	beq	$3,$2,$L478
	nop

	lui	$2,%hi(gpQdmaPriv)
	lw	$3,%lo(gpQdmaPriv)($2)
	lw	$2,416($sp)
	addiu	$2,$2,26
	sll	$2,$2,2
	addu	$2,$3,$2
	lw	$2,0($2)
	lw	$2,12($2)
	sw	$2,420($sp)
	lui	$2,%hi(gpQdmaPriv)
	lw	$3,%lo(gpQdmaPriv)($2)
	lw	$2,416($sp)
	addiu	$2,$2,26
	sll	$2,$2,2
	addu	$2,$3,$2
	lw	$3,420($sp)
	sw	$3,0($2)
$L478:
	lw	$2,420($sp)
	sw	$2,32($sp)
	lw	$2,32($sp)
	bne	$2,$0,$L481
	nop

	li	$2,-63			# 0xffffffffffffffc1
	j	$L500
	nop

$L481:
	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lw	$3,32($sp)
	sw	$3,112($2)
	lw	$2,48($sp)
	addiu	$2,$2,24
	sw	$2,428($sp)
	lw	$2,428($sp)
	lw	$2,0($2)
	sw	$2,120($sp)
	lw	$2,48($sp)
	addiu	$3,$2,24
	lw	$4,120($sp)
	li	$2,-4096			# 0xfffffffffffff000
	and	$4,$4,$2
	lw	$2,32($sp)
	lw	$2,4($2)
	andi	$2,$2,0xfff
	or	$2,$4,$2
	sw	$3,432($sp)
	sw	$2,436($sp)
	lw	$2,432($sp)
	lw	$3,436($sp)
	sw	$3,0($2)
	lw	$2,48($sp)
	addiu	$2,$2,28
	sw	$2,440($sp)
	lw	$2,440($sp)
	lw	$2,0($2)
	sw	$2,124($sp)
	lw	$2,48($sp)
	addiu	$3,$2,28
	lw	$4,124($sp)
	li	$2,-4096			# 0xfffffffffffff000
	and	$4,$4,$2
	lw	$2,32($sp)
	lw	$2,4($2)
	andi	$2,$2,0xfff
	or	$2,$4,$2
	sw	$3,444($sp)
	sw	$2,448($sp)
	lw	$2,444($sp)
	lw	$3,448($sp)
	sw	$3,0($2)
	sw	$0,20($sp)
	j	$L484
	nop

$L488:
	lw	$4,20($sp)
	lui	$2,%hi(qdma_prepare_rx_buffer)
	addiu	$2,$2,%lo(qdma_prepare_rx_buffer)
	jalr	$2
	nop

	beq	$2,$0,$L485
	nop

	j	$L486
	nop

$L485:
	lw	$2,20($sp)
	sw	$2,452($sp)
	lui	$2,%hi(gpQdmaPriv)
	lw	$3,%lo(gpQdmaPriv)($2)
	lw	$2,452($sp)
	addiu	$2,$2,26
	sll	$2,$2,2
	addu	$2,$3,$2
	lw	$2,0($2)
	lw	$3,12($2)
	lui	$2,%hi(gpQdmaPriv)
	lw	$4,%lo(gpQdmaPriv)($2)
	lw	$2,452($sp)
	addiu	$2,$2,24
	sll	$2,$2,2
	addu	$2,$4,$2
	lw	$2,0($2)
	xor	$2,$3,$2
	sltu	$2,$0,$2
	andi	$2,$2,0x00ff
	bne	$2,$0,$L488
	nop

$L486:
	lw	$2,20($sp)
	addiu	$2,$2,1
	sw	$2,20($sp)
$L484:
	lw	$2,20($sp)
	slt	$2,$2,2
	bne	$2,$0,$L488
	nop

	lw	$2,52($sp)
	beq	$2,$0,$L489
	nop

	lw	$2,52($sp)
	sll	$2,$2,4
	sw	$2,40($sp)
	lw	$3,60($sp)
	lw	$2,52($sp)
	mul	$2,$3,$2
	sw	$2,44($sp)
	lui	$2,%hi(physical_size)
	lw	$3,44($sp)
	sw	$3,%lo(physical_size)($2)
	lui	$2,%hi(physical_size)
	lw	$3,%lo(physical_size)($2)
	lui	$2,%hi($LC68)
	addiu	$4,$2,%lo($LC68)
	lui	$2,%hi($LC69)
	addiu	$5,$2,%lo($LC69)
	move	$6,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lw	$3,60($sp)
	sw	$3,68($2)
	lui	$2,%hi(gpQdmaPriv)
	lw	$16,%lo(gpQdmaPriv)($2)
	addiu	$2,$sp,532
	move	$4,$0
	lw	$5,40($sp)
	move	$6,$2
	li	$7,208			# 0xd0
	lui	$2,%hi(dma_alloc_coherent)
	addiu	$2,$2,%lo(dma_alloc_coherent)
	jalr	$2
	nop

	sw	$2,60($16)
	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lw	$2,60($2)
	bne	$2,$0,$L490
	nop

	li	$2,-12			# 0xfffffffffffffff4
	j	$L500
	nop

$L490:
	lui	$2,%hi(gpQdmaPriv)
	lw	$16,%lo(gpQdmaPriv)($2)
	addiu	$2,$sp,536
	move	$4,$0
	lw	$5,44($sp)
	move	$6,$2
	li	$7,208			# 0xd0
	lui	$2,%hi(dma_alloc_coherent)
	addiu	$2,$2,%lo(dma_alloc_coherent)
	jalr	$2
	nop

	sw	$2,64($16)
	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lw	$2,64($2)
	bne	$2,$0,$L491
	nop

	li	$2,-12			# 0xfffffffffffffff4
	j	$L500
	nop

$L491:
	lw	$2,48($sp)
	addiu	$3,$2,32
	lw	$2,532($sp)
	sw	$3,456($sp)
	sw	$2,460($sp)
	lw	$2,456($sp)
	lw	$3,460($sp)
	sw	$3,0($2)
	lw	$2,48($sp)
	addiu	$3,$2,36
	lw	$2,536($sp)
	sw	$3,464($sp)
	sw	$2,468($sp)
	lw	$2,464($sp)
	lw	$3,468($sp)
	sw	$3,0($2)
	lw	$2,48($sp)
	addiu	$2,$2,48
	sw	$2,472($sp)
	lw	$2,472($sp)
	lw	$2,0($2)
	sw	$2,128($sp)
	lw	$2,48($sp)
	addiu	$3,$2,48
	lw	$4,128($sp)
	li	$2,-8192			# 0xffffffffffffe000
	and	$4,$4,$2
	lw	$2,52($sp)
	andi	$2,$2,0x1fff
	or	$2,$4,$2
	sw	$3,476($sp)
	sw	$2,480($sp)
	lw	$2,476($sp)
	lw	$3,480($sp)
	sw	$3,0($2)
	lw	$2,48($sp)
	addiu	$2,$2,40
	sw	$2,484($sp)
	lw	$2,484($sp)
	lw	$2,0($2)
	sw	$2,132($sp)
	lw	$2,48($sp)
	addiu	$3,$2,40
	lw	$4,132($sp)
	li	$2,-805371904			# 0xffffffffcfff0000
	ori	$2,$2,0xffff
	and	$2,$4,$2
	sw	$3,488($sp)
	sw	$2,492($sp)
	lw	$2,488($sp)
	lw	$3,492($sp)
	sw	$3,0($2)
	lw	$2,48($sp)
	addiu	$2,$2,40
	sw	$2,496($sp)
	lw	$2,496($sp)
	lw	$2,0($2)
	sw	$2,136($sp)
	lw	$2,48($sp)
	addiu	$3,$2,40
	lw	$4,136($sp)
	li	$2,-8192			# 0xffffffffffffe000
	and	$2,$4,$2
	ori	$2,$2,0x80
	sw	$3,500($sp)
	sw	$2,504($sp)
	lw	$2,500($sp)
	lw	$3,504($sp)
	sw	$3,0($2)
	lw	$2,48($sp)
	addiu	$2,$2,48
	sw	$2,508($sp)
	lw	$2,508($sp)
	lw	$2,0($2)
	sw	$2,140($sp)
	lw	$2,48($sp)
	addiu	$3,$2,48
	lw	$4,140($sp)
	li	$2,-2147483648			# 0xffffffff80000000
	or	$2,$4,$2
	sw	$3,512($sp)
	sw	$2,516($sp)
	lw	$2,512($sp)
	lw	$3,516($sp)
	sw	$3,0($2)
	sw	$0,24($sp)
	li	$2,25			# 0x19
	sw	$2,28($sp)
	j	$L496
	nop

$L499:
	li	$4,1000			# 0x3e8
	lui	$2,%hi(__udelay)
	addiu	$2,$2,%lo(__udelay)
	jalr	$2
	nop

	lw	$2,48($sp)
	addiu	$2,$2,48
	sw	$2,520($sp)
	lw	$2,520($sp)
	lw	$2,0($2)
	bltz	$2,$L496
	nop

	li	$2,1			# 0x1
	sw	$2,24($sp)
	j	$L498
	nop

$L496:
	lw	$2,28($sp)
	addiu	$3,$2,-1
	sw	$3,28($sp)
	bgtz	$2,$L499
	nop

$L498:
	lw	$2,24($sp)
	bne	$2,$0,$L489
	nop

	li	$2,-1			# 0xffffffffffffffff
	j	$L500
	nop

$L489:
	move	$2,$0
$L500:
	lw	$31,564($sp)
	lw	$16,560($sp)
	addiu	$sp,$sp,568
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	qdma_bm_dscp_init
	.size	qdma_bm_dscp_init, .-qdma_bm_dscp_init
	.section	.text.qdma_bm_dscp_deinit,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	qdma_bm_dscp_deinit
	.type	qdma_bm_dscp_deinit, @function
qdma_bm_dscp_deinit:
	.frame	$sp,112,$31		# vars= 88, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-112
	sw	$31,108($sp)
	sw	$0,24($sp)
	sw	$0,28($sp)
	sw	$0,32($sp)
	sw	$0,16($sp)
	sw	$0,20($sp)
	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lw	$2,0($2)
	sw	$2,32($sp)
	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lhu	$2,20($2)
	move	$3,$2
	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lhu	$2,22($2)
	addu	$2,$3,$2
	lui	$3,%hi(gpQdmaPriv)
	lw	$3,%lo(gpQdmaPriv)($3)
	lhu	$3,24($3)
	addu	$2,$2,$3
	lui	$3,%hi(gpQdmaPriv)
	lw	$3,%lo(gpQdmaPriv)($3)
	lhu	$3,26($3)
	addu	$2,$2,$3
	sw	$2,28($sp)
	lw	$2,32($sp)
	addiu	$2,$2,4
	sw	$2,52($sp)
	lw	$2,52($sp)
	lw	$2,0($2)
	sw	$2,36($sp)
	lw	$2,32($sp)
	addiu	$3,$2,4
	lw	$4,36($sp)
	li	$2,-2			# 0xfffffffffffffffe
	and	$2,$4,$2
	sw	$3,56($sp)
	sw	$2,60($sp)
	lw	$2,56($sp)
	lw	$3,60($sp)
	sw	$3,0($2)
	lw	$2,32($sp)
	addiu	$2,$2,4
	sw	$2,64($sp)
	lw	$2,64($sp)
	lw	$2,0($2)
	sw	$2,40($sp)
	lw	$2,32($sp)
	addiu	$3,$2,4
	lw	$4,40($sp)
	li	$2,-5			# 0xfffffffffffffffb
	and	$2,$4,$2
	sw	$3,68($sp)
	sw	$2,72($sp)
	lw	$2,68($sp)
	lw	$3,72($sp)
	sw	$3,0($2)
	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lhu	$2,30($2)
	move	$4,$2
	lui	$2,%hi(qdma_bm_transmit_done)
	addiu	$2,$2,%lo(qdma_bm_transmit_done)
	jalr	$2
	nop

	li	$4,1			# 0x1
	move	$5,$0
	lui	$2,%hi(qdma_recycle_packet_mode)
	addiu	$2,$2,%lo(qdma_recycle_packet_mode)
	jalr	$2
	nop

	sw	$0,20($sp)
	j	$L504
	nop

$L505:
	lw	$4,20($sp)
	lui	$2,%hi(qdma_bm_recycle_receive_buffer)
	addiu	$2,$2,%lo(qdma_bm_recycle_receive_buffer)
	jalr	$2
	nop

	lw	$2,20($sp)
	addiu	$2,$2,1
	sw	$2,20($sp)
$L504:
	lw	$2,20($sp)
	slt	$2,$2,2
	bne	$2,$0,$L505
	nop

	sw	$0,20($sp)
	j	$L506
	nop

$L507:
	lw	$4,20($sp)
	lui	$2,%hi(qdma_bm_recycle_transmit_buffer)
	addiu	$2,$2,%lo(qdma_bm_recycle_transmit_buffer)
	jalr	$2
	nop

	lw	$2,20($sp)
	addiu	$2,$2,1
	sw	$2,20($sp)
$L506:
	lw	$2,20($sp)
	slt	$2,$2,2
	bne	$2,$0,$L507
	nop

	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lw	$2,36($2)
	beq	$2,$0,$L508
	nop

	sw	$0,16($sp)
	j	$L509
	nop

$L511:
	lw	$2,16($sp)
	sll	$3,$2,4
	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lw	$2,36($2)
	addu	$2,$3,$2
	sw	$2,24($sp)
	lw	$2,24($sp)
	lw	$2,8($2)
	beq	$2,$0,$L510
	nop

	lw	$2,24($sp)
	lw	$2,0($2)
	lw	$2,8($2)
	beq	$2,$0,$L510
	nop

	lw	$2,24($sp)
	lw	$2,0($2)
	lw	$3,8($2)
	lw	$2,24($sp)
	lw	$2,0($2)
	lhu	$2,6($2)
	move	$4,$0
	move	$5,$3
	move	$6,$2
	move	$7,$0
	lui	$2,%hi(dma_unmap_single)
	addiu	$2,$2,%lo(dma_unmap_single)
	jalr	$2
	nop

$L510:
	lw	$2,16($sp)
	addiu	$2,$2,1
	sw	$2,16($sp)
$L509:
	lw	$3,16($sp)
	lw	$2,28($sp)
	sltu	$2,$3,$2
	bne	$2,$0,$L511
	nop

	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lw	$2,36($2)
	sw	$2,24($sp)
	lw	$2,24($sp)
	lw	$2,0($2)
	beq	$2,$0,$L512
	nop

	lw	$2,28($sp)
	sll	$5,$2,5
	lw	$2,24($sp)
	lw	$3,0($2)
	lw	$2,32($sp)
	addiu	$2,$2,264
	sw	$2,76($sp)
	lw	$2,76($sp)
	lw	$2,0($2)
	move	$4,$0
	move	$6,$3
	move	$7,$2
	lui	$2,%hi(dma_free_coherent)
	addiu	$2,$2,%lo(dma_free_coherent)
	jalr	$2
	nop

	lw	$2,24($sp)
	sw	$0,0($2)
$L512:
	lw	$4,24($sp)
	lui	$2,%hi(kfree)
	addiu	$2,$2,%lo(kfree)
	jalr	$2
	nop

	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	sw	$0,36($2)
$L508:
	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lw	$2,56($2)
	beq	$2,$0,$L514
	nop

	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lhu	$2,30($2)
	sll	$2,$2,2
	move	$5,$2
	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lw	$2,56($2)
	move	$3,$2
	lw	$2,32($sp)
	addiu	$2,$2,96
	sw	$2,80($sp)
	lw	$2,80($sp)
	lw	$2,0($2)
	move	$4,$0
	move	$6,$3
	move	$7,$2
	lui	$2,%hi(dma_free_coherent)
	addiu	$2,$2,%lo(dma_free_coherent)
	jalr	$2
	nop

	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	sw	$0,56($2)
$L514:
	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lw	$2,60($2)
	beq	$2,$0,$L516
	nop

	lw	$2,32($sp)
	addiu	$2,$2,48
	sw	$2,84($sp)
	lw	$2,84($sp)
	lw	$2,0($2)
	andi	$2,$2,0x1fff
	sll	$2,$2,4
	sw	$2,44($sp)
	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lw	$2,60($2)
	move	$3,$2
	lw	$2,32($sp)
	addiu	$2,$2,32
	sw	$2,88($sp)
	lw	$2,88($sp)
	lw	$2,0($2)
	move	$4,$0
	lw	$5,44($sp)
	move	$6,$3
	move	$7,$2
	lui	$2,%hi(dma_free_coherent)
	addiu	$2,$2,%lo(dma_free_coherent)
	jalr	$2
	nop

	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	sw	$0,60($2)
$L516:
	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lw	$2,64($2)
	beq	$2,$0,$L519
	nop

	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lw	$3,68($2)
	lw	$2,32($sp)
	addiu	$2,$2,48
	sw	$2,92($sp)
	lw	$2,92($sp)
	lw	$2,0($2)
	andi	$2,$2,0x1fff
	mul	$2,$3,$2
	sw	$2,48($sp)
	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lw	$2,64($2)
	move	$3,$2
	lw	$2,32($sp)
	addiu	$2,$2,36
	sw	$2,96($sp)
	lw	$2,96($sp)
	lw	$2,0($2)
	move	$4,$0
	lw	$5,48($sp)
	move	$6,$3
	move	$7,$2
	lui	$2,%hi(dma_free_coherent)
	addiu	$2,$2,%lo(dma_free_coherent)
	jalr	$2
	nop

	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	sw	$0,64($2)
$L519:
	move	$2,$0
	lw	$31,108($sp)
	addiu	$sp,$sp,112
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	qdma_bm_dscp_deinit
	.size	qdma_bm_dscp_deinit, .-qdma_bm_dscp_deinit
	.rdata
	.align	2
$LC70:
	.ascii	"qdma_lan/counters\000"
	.align	2
$LC71:
	.ascii	"qdma_lan/debug\000"
	.section	.exit.text,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	qdma_module_cleanup
	.type	qdma_module_cleanup, @function
qdma_module_cleanup:
	.frame	$sp,32,$31		# vars= 8, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-32
	sw	$31,28($sp)
	lui	$2,%hi(qdma_dvt_deinit)
	addiu	$2,$2,%lo(qdma_dvt_deinit)
	jalr	$2
	nop

	lui	$2,%hi($LC70)
	addiu	$4,$2,%lo($LC70)
	move	$5,$0
	lui	$2,%hi(remove_proc_entry)
	addiu	$2,$2,%lo(remove_proc_entry)
	jalr	$2
	nop

	lui	$2,%hi($LC71)
	addiu	$4,$2,%lo($LC71)
	move	$5,$0
	lui	$2,%hi(remove_proc_entry)
	addiu	$2,$2,%lo(remove_proc_entry)
	jalr	$2
	nop

	lui	$2,%hi($LC22)
	addiu	$4,$2,%lo($LC22)
	move	$5,$0
	lui	$2,%hi(remove_proc_entry)
	addiu	$2,$2,%lo(remove_proc_entry)
	jalr	$2
	nop

	lui	$2,%hi($LC51)
	addiu	$4,$2,%lo($LC51)
	move	$5,$0
	lui	$2,%hi(remove_proc_entry)
	addiu	$2,$2,%lo(remove_proc_entry)
	jalr	$2
	nop

	lui	$2,%hi($LC69)
	addiu	$4,$2,%lo($LC69)
	move	$5,$0
	lui	$2,%hi(remove_proc_entry)
	addiu	$2,$2,%lo(remove_proc_entry)
	jalr	$2
	nop

	lui	$2,%hi(ecnt_driver_qdma_op)
	addiu	$4,$2,%lo(ecnt_driver_qdma_op)
	lui	$2,%hi(ecnt_unregister_hook)
	addiu	$2,$2,%lo(ecnt_unregister_hook)
	jalr	$2
	nop

	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	addiu	$2,$2,304
	move	$4,$2
	lui	$2,%hi(del_timer_sync)
	addiu	$2,$2,%lo(del_timer_sync)
	jalr	$2
	nop

	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lw	$3,140($2)
	li	$2,268435456			# 0x10000000
	and	$2,$3,$2
	beq	$2,$0,$L524
	nop

	li	$4,22			# 0x16
	move	$5,$0
	lui	$2,%hi(free_irq)
	addiu	$2,$2,%lo(free_irq)
	jalr	$2
	nop

$L524:
	lui	$2,%hi(qdma_dev_destroy)
	addiu	$2,$2,%lo(qdma_dev_destroy)
	jalr	$2
	nop

	lui	$2,%hi(qdma_bm_dscp_deinit)
	addiu	$2,$2,%lo(qdma_bm_dscp_deinit)
	jalr	$2
	nop

	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lw	$2,0($2)
	beq	$2,$0,$L525
	nop

	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lw	$2,0($2)
	sw	$2,16($sp)
	lw	$2,16($sp)
	sw	$2,20($sp)
	move	$2,$0
	bne	$2,$0,$L527
	nop

	lw	$4,16($sp)
	lui	$2,%hi(__iounmap)
	addiu	$2,$2,%lo(__iounmap)
	jalr	$2
	nop

$L527:
	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	sw	$0,0($2)
$L525:
	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	move	$4,$2
	lui	$2,%hi(kfree)
	addiu	$2,$2,%lo(kfree)
	jalr	$2
	nop

	lui	$2,%hi(gpQdmaPriv)
	sw	$0,%lo(gpQdmaPriv)($2)
	nop
	lw	$31,28($sp)
	addiu	$sp,$sp,32
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	qdma_module_cleanup
	.size	qdma_module_cleanup, .-qdma_module_cleanup
	.globl	cleanup_module
	cleanup_module = qdma_module_cleanup
	.section	.text.qdma_fwd_cfg_timer_expires,"ax",@progbits
	.align	2
	.globl	qdma_fwd_cfg_timer_expires
	.set	nomips16
	.set	nomicromips
	.ent	qdma_fwd_cfg_timer_expires
	.type	qdma_fwd_cfg_timer_expires, @function
qdma_fwd_cfg_timer_expires:
	.frame	$sp,304,$31		# vars= 280, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-304
	sw	$31,300($sp)
	sw	$4,304($sp)
	sw	$0,16($sp)
	sw	$0,20($sp)
	li	$2,-1078657024			# 0xffffffffbfb50000
	ori	$2,$2,0xe00
	sw	$2,24($sp)
	lw	$2,24($sp)
	lw	$2,0($2)
	sw	$2,16($sp)
	lw	$2,16($sp)
	andi	$2,$2,0x1
	beq	$2,$0,$L531
	nop

	lw	$2,20($sp)
	sw	$2,28($sp)
	sw	$0,32($sp)
	li	$2,4			# 0x4
	sw	$2,36($sp)
	sw	$0,40($sp)
	li	$2,10			# 0xa
	sw	$2,124($sp)
	lw	$2,28($sp)
	sw	$2,128($sp)
	lw	$2,32($sp)
	sw	$2,176($sp)
	lw	$2,36($sp)
	sw	$2,180($sp)
	addiu	$2,$sp,124
	li	$4,18			# 0x12
	move	$5,$0
	move	$6,$2
	lui	$2,%hi(__ECNT_HOOK)
	addiu	$2,$2,%lo(__ECNT_HOOK)
	jalr	$2
	nop

	sw	$2,40($sp)
	lw	$2,20($sp)
	sw	$2,44($sp)
	li	$2,1			# 0x1
	sw	$2,48($sp)
	li	$2,4			# 0x4
	sw	$2,52($sp)
	sw	$0,56($sp)
	li	$2,10			# 0xa
	sw	$2,124($sp)
	lw	$2,44($sp)
	sw	$2,128($sp)
	lw	$2,48($sp)
	sw	$2,176($sp)
	lw	$2,52($sp)
	sw	$2,180($sp)
	addiu	$2,$sp,124
	li	$4,18			# 0x12
	move	$5,$0
	move	$6,$2
	lui	$2,%hi(__ECNT_HOOK)
	addiu	$2,$2,%lo(__ECNT_HOOK)
	jalr	$2
	nop

	sw	$2,56($sp)
	lw	$2,20($sp)
	sw	$2,60($sp)
	li	$2,2			# 0x2
	sw	$2,64($sp)
	li	$2,4			# 0x4
	sw	$2,68($sp)
	sw	$0,72($sp)
	li	$2,10			# 0xa
	sw	$2,124($sp)
	lw	$2,60($sp)
	sw	$2,128($sp)
	lw	$2,64($sp)
	sw	$2,176($sp)
	lw	$2,68($sp)
	sw	$2,180($sp)
	addiu	$2,$sp,124
	li	$4,18			# 0x12
	move	$5,$0
	move	$6,$2
	lui	$2,%hi(__ECNT_HOOK)
	addiu	$2,$2,%lo(__ECNT_HOOK)
	jalr	$2
	nop

	sw	$2,72($sp)
	j	$L529
	nop

$L531:
	lw	$2,20($sp)
	sw	$2,76($sp)
	sw	$0,80($sp)
	li	$2,5			# 0x5
	sw	$2,84($sp)
	sw	$0,88($sp)
	li	$2,10			# 0xa
	sw	$2,124($sp)
	lw	$2,76($sp)
	sw	$2,128($sp)
	lw	$2,80($sp)
	sw	$2,176($sp)
	lw	$2,84($sp)
	sw	$2,180($sp)
	addiu	$2,$sp,124
	li	$4,18			# 0x12
	move	$5,$0
	move	$6,$2
	lui	$2,%hi(__ECNT_HOOK)
	addiu	$2,$2,%lo(__ECNT_HOOK)
	jalr	$2
	nop

	sw	$2,88($sp)
	lw	$2,20($sp)
	sw	$2,92($sp)
	li	$2,1			# 0x1
	sw	$2,96($sp)
	li	$2,5			# 0x5
	sw	$2,100($sp)
	sw	$0,104($sp)
	li	$2,10			# 0xa
	sw	$2,124($sp)
	lw	$2,92($sp)
	sw	$2,128($sp)
	lw	$2,96($sp)
	sw	$2,176($sp)
	lw	$2,100($sp)
	sw	$2,180($sp)
	addiu	$2,$sp,124
	li	$4,18			# 0x12
	move	$5,$0
	move	$6,$2
	lui	$2,%hi(__ECNT_HOOK)
	addiu	$2,$2,%lo(__ECNT_HOOK)
	jalr	$2
	nop

	sw	$2,104($sp)
	lw	$2,20($sp)
	sw	$2,108($sp)
	li	$2,2			# 0x2
	sw	$2,112($sp)
	li	$2,5			# 0x5
	sw	$2,116($sp)
	sw	$0,120($sp)
	li	$2,10			# 0xa
	sw	$2,124($sp)
	lw	$2,108($sp)
	sw	$2,128($sp)
	lw	$2,112($sp)
	sw	$2,176($sp)
	lw	$2,116($sp)
	sw	$2,180($sp)
	addiu	$2,$sp,124
	li	$4,18			# 0x12
	move	$5,$0
	move	$6,$2
	lui	$2,%hi(__ECNT_HOOK)
	addiu	$2,$2,%lo(__ECNT_HOOK)
	jalr	$2
	nop

	sw	$2,120($sp)
$L529:
	lw	$31,300($sp)
	addiu	$sp,$sp,304
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	qdma_fwd_cfg_timer_expires
	.size	qdma_fwd_cfg_timer_expires, .-qdma_fwd_cfg_timer_expires
	.rdata
	.align	2
$LC72:
	.ascii	"macType = %u\012\000"
	.align	2
$LC73:
	.ascii	"mac type is MAC_TYPE_ATM\012\000"
	.align	2
$LC74:
	.ascii	"mac type is MAC_TYPE_PTM\012\000"
	.section	.init.text,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	qdma_module_init
	.type	qdma_module_init, @function
qdma_module_init:
	.frame	$sp,192,$31		# vars= 160, regs= 2/0, args= 24, gp= 0
	.mask	0x80010000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-192
	sw	$31,188($sp)
	sw	$16,184($sp)
	sw	$0,24($sp)
	sw	$0,28($sp)
	sw	$0,32($sp)
	sw	$0,36($sp)
	lui	$2,%hi(macType)
	lw	$3,%lo(macType)($2)
	lui	$2,%hi($LC72)
	addiu	$4,$2,%lo($LC72)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi(macType)
	lw	$3,%lo(macType)($2)
	li	$2,3			# 0x3
	bne	$3,$2,$L552
	nop

	lui	$2,%hi($LC73)
	addiu	$4,$2,%lo($LC73)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	li	$2,-1078984704			# 0xffffffffbfb00000
	ori	$2,$2,0x70
	sw	$2,40($sp)
	lw	$2,40($sp)
	lw	$2,0($2)
	sw	$2,36($sp)
	lw	$3,36($sp)
	li	$2,-8			# 0xfffffffffffffff8
	and	$2,$3,$2
	sw	$2,36($sp)
	lw	$2,36($sp)
	ori	$2,$2,0x3
	sw	$2,36($sp)
	li	$2,-1078984704			# 0xffffffffbfb00000
	ori	$2,$2,0x70
	sw	$2,44($sp)
	lw	$2,36($sp)
	sw	$2,48($sp)
	lw	$2,44($sp)
	lw	$3,48($sp)
	sw	$3,0($2)
	j	$L554
	nop

$L552:
	lui	$2,%hi(macType)
	lw	$3,%lo(macType)($2)
	li	$2,2			# 0x2
	bne	$3,$2,$L554
	nop

	lui	$2,%hi($LC74)
	addiu	$4,$2,%lo($LC74)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	li	$2,-1078984704			# 0xffffffffbfb00000
	ori	$2,$2,0x70
	sw	$2,52($sp)
	lw	$2,52($sp)
	lw	$2,0($2)
	sw	$2,36($sp)
	lw	$3,36($sp)
	li	$2,-8			# 0xfffffffffffffff8
	and	$2,$3,$2
	sw	$2,36($sp)
	lw	$2,36($sp)
	ori	$2,$2,0x2
	sw	$2,36($sp)
	li	$2,-1078984704			# 0xffffffffbfb00000
	ori	$2,$2,0x70
	sw	$2,56($sp)
	lw	$2,36($sp)
	sw	$2,60($sp)
	lw	$2,56($sp)
	lw	$3,60($sp)
	sw	$3,0($2)
$L554:
	li	$2,340			# 0x154
	sw	$2,64($sp)
	li	$2,208			# 0xd0
	sw	$2,68($sp)
	lw	$2,68($sp)
	ori	$2,$2,0x8000
	lw	$3,64($sp)
	sw	$3,72($sp)
	sw	$2,76($sp)
$L556 = .
	lw	$4,72($sp)
	lw	$5,76($sp)
	lui	$2,%hi(__kmalloc)
	addiu	$2,$2,%lo(__kmalloc)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(gpQdmaPriv)
	sw	$3,%lo(gpQdmaPriv)($2)
	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	bne	$2,$0,$L559
	nop

	li	$2,-12			# 0xfffffffffffffff4
	j	$L580
	nop

$L559:
	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	li	$3,16			# 0x10
	sb	$3,141($2)
	sw	$0,24($sp)
	j	$L561
	nop

$L563:
	lui	$2,%hi(gpQdmaPriv)
	lw	$3,%lo(gpQdmaPriv)($2)
	lw	$2,24($sp)
	sll	$2,$2,2
	addu	$2,$3,$2
	addiu	$2,$2,4
	sw	$2,80($sp)
	lui	$2,%hi(gpQdmaPriv)
	lw	$3,%lo(gpQdmaPriv)($2)
	lw	$2,24($sp)
	sll	$2,$2,2
	addu	$2,$3,$2
	sw	$0,4($2)
	lw	$2,24($sp)
	addiu	$2,$2,1
	sw	$2,24($sp)
$L561:
	lw	$2,24($sp)
	slt	$2,$2,2
	bne	$2,$0,$L563
	nop

	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	addiu	$2,$2,12
	sw	$2,84($sp)
	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	sw	$0,12($2)
	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	addiu	$2,$2,16
	sw	$2,88($sp)
	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	sw	$0,16($2)
	lui	$2,%hi(gpQdmaPriv)
	lw	$16,%lo(gpQdmaPriv)($2)
	li	$2,531955712			# 0x1fb50000
	ori	$2,$2,0x4000
	sw	$2,92($sp)
	li	$2,1024			# 0x400
	sw	$2,96($sp)
	li	$2,512			# 0x200
	sw	$2,100($sp)
	lw	$2,92($sp)
	sw	$2,104($sp)
	lw	$2,96($sp)
	sw	$2,108($sp)
	lw	$2,100($sp)
	sw	$2,112($sp)
	move	$2,$0
	sw	$2,116($sp)
	lw	$2,116($sp)
	beq	$2,$0,$L567
	nop

	lw	$2,116($sp)
	j	$L568
	nop

$L567:
	lw	$4,92($sp)
	lw	$5,96($sp)
	lw	$6,100($sp)
	lui	$2,%hi(__ioremap)
	addiu	$2,$2,%lo(__ioremap)
	jalr	$2
	nop

$L568:
	sw	$2,0($16)
	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lw	$2,0($2)
	bne	$2,$0,$L569
	nop

	li	$2,-14			# 0xfffffffffffffff2
	j	$L580
	nop

$L569:
	lui	$2,%hi(hwfwd_payload_max_size)
	li	$3,2048			# 0x800
	sw	$3,%lo(hwfwd_payload_max_size)($2)
	li	$2,128			# 0x80
	sh	$2,142($sp)
	li	$2,128			# 0x80
	sh	$2,140($sp)
	li	$2,512			# 0x200
	sh	$2,146($sp)
	li	$2,128			# 0x80
	sh	$2,144($sp)
	li	$2,1024			# 0x400
	sh	$2,148($sp)
	li	$2,256			# 0x100
	sh	$2,150($sp)
	lui	$2,%hi(hwfwd_payload_max_size)
	lw	$2,%lo(hwfwd_payload_max_size)($2)
	andi	$2,$2,0xffff
	sh	$2,152($sp)
	lw	$4,140($sp)
	lw	$5,144($sp)
	lw	$6,148($sp)
	lw	$7,152($sp)
	lui	$2,%hi(qdma_bm_dscp_init)
	addiu	$2,$2,%lo(qdma_bm_dscp_init)
	jalr	$2
	nop

	sw	$2,28($sp)
	lw	$2,28($sp)
	beq	$2,$0,$L570
	nop

	lw	$2,28($sp)
	j	$L580
	nop

$L570:
	lui	$2,%hi(qdma_dev_init)
	addiu	$2,$2,%lo(qdma_dev_init)
	jalr	$2
	nop

	sw	$2,28($sp)
	lw	$2,28($sp)
	beq	$2,$0,$L571
	nop

	lw	$2,28($sp)
	j	$L580
	nop

$L571:
	li	$2,22			# 0x16
	sw	$2,120($sp)
	lui	$2,%hi(qdma_isr)
	addiu	$2,$2,%lo(qdma_isr)
	sw	$2,124($sp)
	sw	$0,128($sp)
	lui	$2,%hi($LC69)
	addiu	$2,$2,%lo($LC69)
	sw	$2,132($sp)
	sw	$0,136($sp)
	lw	$2,132($sp)
	sw	$2,16($sp)
	lw	$2,136($sp)
	sw	$2,20($sp)
	lw	$4,120($sp)
	lw	$5,124($sp)
	move	$6,$0
	lw	$7,128($sp)
	lui	$2,%hi(request_threaded_irq)
	addiu	$2,$2,%lo(request_threaded_irq)
	jalr	$2
	nop

	beq	$2,$0,$L573
	nop

	li	$2,-19			# 0xffffffffffffffed
	j	$L580
	nop

$L573:
	lui	$2,%hi(gpQdmaPriv)
	lw	$3,%lo(gpQdmaPriv)($2)
	lbu	$2,140($3)
	li	$4,1			# 0x1
	ins	$2,$4,4,1
	sb	$2,140($3)
	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	addiu	$2,$2,304
	move	$4,$2
	move	$5,$0
	move	$6,$0
	lui	$2,%hi(init_timer_key)
	addiu	$2,$2,%lo(init_timer_key)
	jalr	$2
	nop

	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lui	$3,%hi(qdma_lan_fwd_timer)
	lw	$3,%lo(qdma_lan_fwd_timer)($3)
	sw	$3,324($2)
	lui	$2,%hi(gpQdmaPriv)
	lw	$2,%lo(gpQdmaPriv)($2)
	lui	$3,%hi(qdma_fwd_cfg_timer_expires)
	addiu	$3,$3,%lo(qdma_fwd_cfg_timer_expires)
	sw	$3,320($2)
	li	$4,2			# 0x2
	lui	$2,%hi(qdma_receive_packet_mode)
	addiu	$2,$2,%lo(qdma_receive_packet_mode)
	jalr	$2
	nop

	li	$4,1			# 0x1
	li	$5,16			# 0x10
	lui	$2,%hi(qdma_recycle_packet_mode)
	addiu	$2,$2,%lo(qdma_recycle_packet_mode)
	jalr	$2
	nop

	li	$2,1			# 0x1
	sw	$2,164($sp)
	addiu	$2,$sp,156
	move	$4,$2
	lui	$2,%hi(qdma_set_rx_protect_en)
	addiu	$2,$2,%lo(qdma_set_rx_protect_en)
	jalr	$2
	nop

	li	$2,1			# 0x1
	sw	$2,164($sp)
	addiu	$2,$sp,156
	move	$4,$2
	lui	$2,%hi(qdma_set_tx_drop_en)
	addiu	$2,$2,%lo(qdma_set_tx_drop_en)
	jalr	$2
	nop

	li	$2,1			# 0x1
	sw	$2,164($sp)
	addiu	$2,$sp,156
	move	$4,$2
	lui	$2,%hi(qdma_set_txq_dei_drop_mode)
	addiu	$2,$2,%lo(qdma_set_txq_dei_drop_mode)
	jalr	$2
	nop

	lui	$2,%hi(ecnt_driver_qdma_op)
	addiu	$4,$2,%lo(ecnt_driver_qdma_op)
	lui	$2,%hi(ecnt_register_hook)
	addiu	$2,$2,%lo(ecnt_register_hook)
	jalr	$2
	nop

	beq	$2,$0,$L574
	nop

	li	$2,-19			# 0xffffffffffffffed
	j	$L580
	nop

$L574:
	lui	$2,%hi($LC69)
	addiu	$4,$2,%lo($LC69)
	move	$5,$0
	lui	$2,%hi(proc_mkdir)
	addiu	$2,$2,%lo(proc_mkdir)
	jalr	$2
	nop

	lui	$2,%hi($LC70)
	addiu	$4,$2,%lo($LC70)
	move	$5,$0
	move	$6,$0
	lui	$2,%hi(create_proc_entry)
	addiu	$2,$2,%lo(create_proc_entry)
	jalr	$2
	nop

	sw	$2,32($sp)
	lw	$2,32($sp)
	beq	$2,$0,$L575
	nop

	lw	$2,32($sp)
	lui	$3,%hi(qdma_bm_counters_read_proc)
	addiu	$3,$3,%lo(qdma_bm_counters_read_proc)
	sw	$3,64($2)
	lw	$2,32($sp)
	lui	$3,%hi(qdma_bm_counters_write_proc)
	addiu	$3,$3,%lo(qdma_bm_counters_write_proc)
	sw	$3,68($2)
$L575:
	lui	$2,%hi($LC71)
	addiu	$4,$2,%lo($LC71)
	move	$5,$0
	move	$6,$0
	lui	$2,%hi(create_proc_entry)
	addiu	$2,$2,%lo(create_proc_entry)
	jalr	$2
	nop

	sw	$2,32($sp)
	lw	$2,32($sp)
	beq	$2,$0,$L576
	nop

	lw	$2,32($sp)
	lui	$3,%hi(qdma_bm_debug_read_proc)
	addiu	$3,$3,%lo(qdma_bm_debug_read_proc)
	sw	$3,64($2)
	lw	$2,32($sp)
	lui	$3,%hi(qdma_bm_debug_write_proc)
	addiu	$3,$3,%lo(qdma_bm_debug_write_proc)
	sw	$3,68($2)
$L576:
	lui	$2,%hi($LC22)
	addiu	$4,$2,%lo($LC22)
	move	$5,$0
	move	$6,$0
	lui	$2,%hi(create_proc_entry)
	addiu	$2,$2,%lo(create_proc_entry)
	jalr	$2
	nop

	sw	$2,32($sp)
	lw	$2,32($sp)
	beq	$2,$0,$L577
	nop

	lw	$2,32($sp)
	lui	$3,%hi(qdma_bm_ratelimit_read_proc)
	addiu	$3,$3,%lo(qdma_bm_ratelimit_read_proc)
	sw	$3,64($2)
	lw	$2,32($sp)
	lui	$3,%hi(qdma_bm_ratelimit_write_proc)
	addiu	$3,$3,%lo(qdma_bm_ratelimit_write_proc)
	sw	$3,68($2)
$L577:
	lui	$2,%hi($LC51)
	addiu	$4,$2,%lo($LC51)
	move	$5,$0
	move	$6,$0
	lui	$2,%hi(create_proc_entry)
	addiu	$2,$2,%lo(create_proc_entry)
	jalr	$2
	nop

	sw	$2,32($sp)
	lw	$2,32($sp)
	beq	$2,$0,$L578
	nop

	lw	$2,32($sp)
	lui	$3,%hi(qdma_bm_qos_read_proc)
	addiu	$3,$3,%lo(qdma_bm_qos_read_proc)
	sw	$3,64($2)
	lw	$2,32($sp)
	lui	$3,%hi(qdma_bm_qos_write_proc)
	addiu	$3,$3,%lo(qdma_bm_qos_write_proc)
	sw	$3,68($2)
$L578:
	lui	$2,%hi(qdma_dvt_init)
	addiu	$2,$2,%lo(qdma_dvt_init)
	jalr	$2
	nop

	sw	$2,28($sp)
	lw	$2,28($sp)
	beq	$2,$0,$L579
	nop

	lw	$2,28($sp)
	j	$L580
	nop

$L579:
	move	$2,$0
$L580:
	lw	$31,188($sp)
	lw	$16,184($sp)
	addiu	$sp,$sp,192
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	qdma_module_init
	.size	qdma_module_init, .-qdma_module_init
	.globl	init_module
	init_module = qdma_module_init
	.local	__param_perm_check_physical_size
	.comm	__param_perm_check_physical_size,4,4
	.rdata
	.align	2
	.type	__param_str_physical_size, @object
	.size	__param_str_physical_size, 14
__param_str_physical_size:
	.ascii	"physical_size\000"
	.section	__param,"a",@progbits
	.align	2
	.type	__param_physical_size, @object
	.size	__param_physical_size, 16
__param_physical_size:
	.word	__param_str_physical_size
	.word	param_ops_uint
	.half	0
	.half	0
	.word	physical_size
	.section	.modinfo,"a",@progbits
	.align	2
	.type	__mod_physical_sizetype3082, @object
	.size	__mod_physical_sizetype3082, 28
__mod_physical_sizetype3082:
	.ascii	"parmtype=physical_size:uint\000"
	.local	__param_perm_check_macType
	.comm	__param_perm_check_macType,4,4
	.rdata
	.align	2
	.type	__param_str_macType, @object
	.size	__param_str_macType, 8
__param_str_macType:
	.ascii	"macType\000"
	.section	__param
	.align	2
	.type	__param_macType, @object
	.size	__param_macType, 16
__param_macType:
	.word	__param_str_macType
	.word	param_ops_uint
	.half	0
	.half	0
	.word	macType
	.section	.modinfo
	.align	2
	.type	__mod_macTypetype3083, @object
	.size	__mod_macTypetype3083, 22
__mod_macTypetype3083:
	.ascii	"parmtype=macType:uint\000"
	.ident	"GCC: (Buildroot 2015.08.1) 4.9.3"
