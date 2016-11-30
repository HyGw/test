	.file	1 "gpon_wan.c"
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
	.globl	g_service_change_flag
	.section	.bss,"aw",@nobits
	.align	2
	.type	g_service_change_flag, @object
	.size	g_service_change_flag, 4
g_service_change_flag:
	.space	4
	.rdata
	.align	2
$LC1:
	.ascii	"[%lu0ms]Message integrity check length fault\012\000"
	.section	.text.remove_omci_crc_if_exist,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	remove_omci_crc_if_exist
	.type	remove_omci_crc_if_exist, @function
remove_omci_crc_if_exist:
	.frame	$sp,32,$31		# vars= 8, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-32
	sw	$31,28($sp)
	sw	$4,32($sp)
	sw	$0,16($sp)
	sh	$0,20($sp)
	sh	$0,22($sp)
	lw	$2,32($sp)
	lw	$2,256($2)
	sw	$2,16($sp)
	lw	$2,32($sp)
	lw	$2,80($2)
	sltu	$2,$2,10
	andi	$2,$2,0x00ff
	beq	$2,$0,$L2
	nop

	li	$2,-1			# 0xffffffffffffffff
	j	$L3
	nop

$L2:
	lw	$2,16($sp)
	lbu	$3,3($2)
	li	$2,10			# 0xa
	bne	$3,$2,$L4
	nop

	lw	$2,32($sp)
	lw	$3,80($2)
	li	$2,48			# 0x30
	bne	$3,$2,$L5
	nop

	lw	$2,32($sp)
	lw	$2,80($2)
	addiu	$3,$2,-4
	lw	$2,32($sp)
	sw	$3,80($2)
	j	$L6
	nop

$L5:
	lw	$2,32($sp)
	lw	$3,80($2)
	li	$2,44			# 0x2c
	beq	$3,$2,$L6
	nop

	li	$2,-1			# 0xffffffffffffffff
	j	$L3
	nop

$L4:
	lw	$2,16($sp)
	lbu	$3,3($2)
	li	$2,11			# 0xb
	bne	$3,$2,$L6
	nop

	lw	$2,16($sp)
	lbu	$3,8($2)
	sll	$3,$3,8
	lbu	$2,9($2)
	or	$2,$2,$3
	andi	$2,$2,0xffff
	addiu	$2,$2,10
	sh	$2,20($sp)
	lw	$2,32($sp)
	lw	$3,80($2)
	lhu	$2,20($sp)
	sltu	$2,$2,$3
	beq	$2,$0,$L6
	nop

	lw	$2,32($sp)
	lw	$2,80($2)
	andi	$3,$2,0xffff
	lhu	$2,20($sp)
	subu	$2,$3,$2
	sh	$2,22($sp)
	lhu	$2,22($sp)
	xori	$2,$2,0x4
	sltu	$2,$0,$2
	andi	$2,$2,0x00ff
	beq	$2,$0,$L7
	nop

	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	lhu	$2,-30816($2)
	andi	$2,$2,0x1
	beq	$2,$0,$L7
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

$L7:
	lw	$2,32($sp)
	lw	$3,80($2)
	lhu	$2,22($sp)
	subu	$3,$3,$2
	lw	$2,32($sp)
	sw	$3,80($2)
$L6:
	move	$2,$0
$L3:
	lw	$31,28($sp)
	addiu	$sp,$sp,32
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	remove_omci_crc_if_exist
	.size	remove_omci_crc_if_exist, .-remove_omci_crc_if_exist
	.rdata
	.align	2
$LC2:
	.ascii	"%s:%d !!!!XPON SPLIT DEBUG!!!====>%s:%d\012\000"
	.align	2
$LC3:
	.ascii	"[%lu0ms]TX SKB LEN:%d, TX MSG: %.8x, %.8x\000"
	.align	2
$LC4:
	.ascii	"[%lu0ms]pwan_net_start_xmit:---gemid = 0x%x\012\000"
	.align	2
$LC5:
	.ascii	"[%lu0ms]pwan_net_start_xmit:---xpon_info->channel = 0x%x"
	.ascii	"\012\000"
	.section	.text.gwan_prepare_tx_message,"ax",@progbits
	.align	2
	.globl	gwan_prepare_tx_message
	.set	nomips16
	.set	nomicromips
	.ent	gwan_prepare_tx_message
	.type	gwan_prepare_tx_message, @function
gwan_prepare_tx_message:
	.frame	$sp,56,$31		# vars= 24, regs= 1/0, args= 24, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-56
	sw	$31,52($sp)
	sw	$4,56($sp)
	move	$2,$5
	sw	$6,64($sp)
	sw	$7,68($sp)
	sb	$2,60($sp)
	sw	$0,24($sp)
	li	$2,-1078984704			# 0xffffffffbfb00000
	ori	$2,$2,0x64
	lw	$3,0($2)
	li	$2,-65536			# 0xffffffffffff0000
	and	$3,$3,$2
	li	$2,458752			# 0x70000
	beq	$3,$2,$L9
	nop

	li	$2,-1078984704			# 0xffffffffbfb00000
	ori	$2,$2,0x64
	lw	$3,0($2)
	li	$2,-65536			# 0xffffffffffff0000
	and	$3,$3,$2
	li	$2,524288			# 0x80000
	bne	$3,$2,$L10
	nop

$L9:
	li	$2,255			# 0xff
	sw	$2,28($sp)
	li	$2,7			# 0x7
	sw	$2,32($sp)
	j	$L11
	nop

$L10:
	li	$2,15			# 0xf
	sw	$2,28($sp)
	li	$2,15			# 0xf
	sw	$2,32($sp)
$L11:
	li	$2,1			# 0x1
	sw	$2,36($sp)
	lw	$3,56($sp)
	lw	$2,4($3)
	li	$4,2			# 0x2
	ins	$2,$4,19,3
	sw	$2,4($3)
	lbu	$3,60($sp)
	li	$2,1			# 0x1
	bne	$3,$2,$L12
	nop

	lw	$3,56($sp)
	lw	$2,0($3)
	li	$4,1			# 0x1
	ins	$2,$4,11,1
	sw	$2,0($3)
	lw	$2,64($sp)
	move	$4,$2
	lui	$2,%hi(remove_omci_crc_if_exist)
	addiu	$2,$2,%lo(remove_omci_crc_if_exist)
	jalr	$2
	nop

	beq	$2,$0,$L13
	nop

	lui	$2,%hi(drop_print_flag)
	lw	$2,%lo(drop_print_flag)($2)
	beq	$2,$0,$L14
	nop

	li	$2,610			# 0x262
	sw	$2,16($sp)
	lui	$2,%hi($LC2)
	addiu	$4,$2,%lo($LC2)
	lui	$2,%hi(__func__.38927)
	addiu	$5,$2,%lo(__func__.38927)
	li	$6,610			# 0x262
	lui	$2,%hi(__func__.38927)
	addiu	$7,$2,%lo(__func__.38927)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L14:
	li	$2,-1			# 0xffffffffffffffff
	j	$L46
	nop

$L13:
	lw	$3,56($sp)
	lw	$2,0($3)
	ins	$2,$0,3,8
	sw	$2,0($3)
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lhu	$2,252($2)
	andi	$2,$2,0xfff
	andi	$4,$2,0xffff
	lw	$3,56($sp)
	lw	$2,0($3)
	ins	$2,$4,12,12
	sw	$2,0($3)
	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	lhu	$2,-30816($2)
	andi	$2,$2,0x20
	beq	$2,$0,$L18
	nop

	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	lhu	$2,-30816($2)
	andi	$2,$2,0x20
	beq	$2,$0,$L17
	nop

	lui	$2,%hi(jiffies)
	lw	$5,%lo(jiffies)($2)
	lw	$2,64($sp)
	lw	$6,80($2)
	lw	$2,56($sp)
	lw	$3,0($2)
	lw	$2,56($sp)
	lw	$2,4($2)
	sw	$2,16($sp)
	lui	$2,%hi($LC3)
	addiu	$4,$2,%lo($LC3)
	move	$7,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L17:
	lw	$3,64($sp)
	lw	$2,64($sp)
	lw	$2,80($2)
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(__dump_skb)
	addiu	$2,$2,%lo(__dump_skb)
	jalr	$2
	nop

	j	$L18
	nop

$L12:
	lw	$3,56($sp)
	lw	$2,0($3)
	ins	$2,$0,11,1
	sw	$2,0($3)
	lw	$2,64($sp)
	lbu	$2,122($2)
	andi	$2,$2,0x2
	beq	$2,$0,$L19
	nop

	lui	$2,%hi(restore_offload_info_hook)
	lw	$2,%lo(restore_offload_info_hook)($2)
	beq	$2,$0,$L20
	nop

	lui	$2,%hi(restore_offload_info_hook)
	lw	$2,%lo(restore_offload_info_hook)($2)
	lw	$3,64($sp)
	move	$4,$3
	lw	$5,72($sp)
	li	$6,29306			# 0x727a
	jalr	$2
	nop

$L20:
	lw	$2,72($sp)
	lhu	$2,4($2)
	andi	$2,$2,0xfff
	andi	$4,$2,0xffff
	lw	$3,56($sp)
	lw	$2,0($3)
	ins	$2,$4,12,12
	sw	$2,0($3)
	lw	$2,72($sp)
	lw	$2,0($2)
	ext	$2,$2,20,8
	andi	$3,$2,0x00ff
	lw	$2,28($sp)
	andi	$2,$2,0x00ff
	and	$2,$3,$2
	andi	$4,$2,0x00ff
	lw	$3,56($sp)
	lw	$2,0($3)
	ins	$2,$4,3,8
	sw	$2,0($3)
	lw	$2,64($sp)
	lw	$3,64($sp)
	lbu	$4,122($3)
	li	$3,-3			# 0xfffffffffffffffd
	and	$3,$4,$3
	andi	$3,$3,0x00ff
	sb	$3,122($2)
	move	$2,$0
	j	$L46
	nop

$L19:
	lui	$2,%hi(xpon_sfu_multicast_protocol_hook)
	lw	$2,%lo(xpon_sfu_multicast_protocol_hook)($2)
	beq	$2,$0,$L21
	nop

	lui	$2,%hi(xpon_sfu_multicast_protocol_hook)
	lw	$2,%lo(xpon_sfu_multicast_protocol_hook)($2)
	lw	$3,64($sp)
	move	$4,$3
	jalr	$2
	nop

	beq	$2,$0,$L21
	nop

	lui	$2,%hi(get_pon_link_type)
	addiu	$2,$2,%lo(get_pon_link_type)
	jalr	$2
	nop

	move	$3,$2
	li	$2,1			# 0x1
	bne	$3,$2,$L21
	nop

	lui	$2,%hi(isVlanOperationInMulticastModule_hook)
	lw	$2,%lo(isVlanOperationInMulticastModule_hook)($2)
	beq	$2,$0,$L21
	nop

	lui	$2,%hi(isVlanOperationInMulticastModule_hook)
	lw	$2,%lo(isVlanOperationInMulticastModule_hook)($2)
	lw	$3,64($sp)
	move	$4,$3
	jalr	$2
	nop

	beq	$2,$0,$L21
	nop

	li	$2,1			# 0x1
	sw	$2,24($sp)
$L21:
	lw	$2,24($sp)
	beq	$2,$0,$L22
	nop

	lui	$2,%hi(xpon_sfu_up_multicast_vlan_hook)
	lw	$2,%lo(xpon_sfu_up_multicast_vlan_hook)($2)
	beq	$2,$0,$L23
	nop

	lui	$2,%hi(xpon_sfu_up_multicast_vlan_hook)
	lw	$2,%lo(xpon_sfu_up_multicast_vlan_hook)($2)
	lw	$3,64($sp)
	move	$4,$3
	move	$5,$0
	jalr	$2
	nop

	move	$3,$2
	li	$2,-1			# 0xffffffffffffffff
	bne	$3,$2,$L23
	nop

	lui	$2,%hi(drop_print_flag)
	lw	$2,%lo(drop_print_flag)($2)
	beq	$2,$0,$L24
	nop

	li	$2,658			# 0x292
	sw	$2,16($sp)
	lui	$2,%hi($LC2)
	addiu	$4,$2,%lo($LC2)
	lui	$2,%hi(__func__.38927)
	addiu	$5,$2,%lo(__func__.38927)
	li	$6,658			# 0x292
	lui	$2,%hi(__func__.38927)
	addiu	$7,$2,%lo(__func__.38927)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L24:
	li	$2,-1			# 0xffffffffffffffff
	j	$L46
	nop

$L23:
	lui	$2,%hi(xpon_up_igmp_uni_vlan_filter_hook)
	lw	$2,%lo(xpon_up_igmp_uni_vlan_filter_hook)($2)
	beq	$2,$0,$L22
	nop

	lui	$2,%hi(xpon_up_igmp_uni_vlan_filter_hook)
	lw	$2,%lo(xpon_up_igmp_uni_vlan_filter_hook)($2)
	lw	$3,64($sp)
	move	$4,$3
	jalr	$2
	nop

	move	$3,$2
	li	$2,-1			# 0xffffffffffffffff
	bne	$3,$2,$L22
	nop

	lui	$2,%hi(drop_print_flag)
	lw	$2,%lo(drop_print_flag)($2)
	beq	$2,$0,$L25
	nop

	li	$2,663			# 0x297
	sw	$2,16($sp)
	lui	$2,%hi($LC2)
	addiu	$4,$2,%lo($LC2)
	lui	$2,%hi(__func__.38927)
	addiu	$5,$2,%lo(__func__.38927)
	li	$6,663			# 0x297
	lui	$2,%hi(__func__.38927)
	addiu	$7,$2,%lo(__func__.38927)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L25:
	li	$2,-1			# 0xffffffffffffffff
	j	$L46
	nop

$L22:
	lui	$2,%hi(gpon_mapping_hook)
	lw	$2,%lo(gpon_mapping_hook)($2)
	beq	$2,$0,$L26
	nop

	lui	$2,%hi(gpon_mapping_hook)
	lw	$2,%lo(gpon_mapping_hook)($2)
	lw	$3,64($sp)
	move	$4,$3
	jalr	$2
	nop

	move	$3,$2
	li	$2,-1			# 0xffffffffffffffff
	bne	$3,$2,$L26
	nop

	lui	$2,%hi(drop_print_flag)
	lw	$2,%lo(drop_print_flag)($2)
	beq	$2,$0,$L27
	nop

	li	$2,671			# 0x29f
	sw	$2,16($sp)
	lui	$2,%hi($LC2)
	addiu	$4,$2,%lo($LC2)
	lui	$2,%hi(__func__.38927)
	addiu	$5,$2,%lo(__func__.38927)
	li	$6,671			# 0x29f
	lui	$2,%hi(__func__.38927)
	addiu	$7,$2,%lo(__func__.38927)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L27:
	li	$2,-1			# 0xffffffffffffffff
	j	$L46
	nop

$L26:
	lw	$2,64($sp)
	lw	$2,220($2)
	andi	$2,$2,0x7ff8
	beq	$2,$0,$L28
	nop

	lui	$2,%hi(gpWanPriv)
	lw	$3,%lo(gpWanPriv)($2)
	lw	$2,64($sp)
	lw	$2,220($2)
	ext	$2,$2,3,12
	andi	$2,$2,0xffff
	addiu	$2,$2,44
	sll	$2,$2,1
	addu	$2,$3,$2
	lhu	$2,4($2)
	andi	$2,$2,0x7fff
	j	$L29
	nop

$L28:
	li	$2,256			# 0x100
$L29:
	sw	$2,40($sp)
	lw	$2,40($sp)
	sltu	$2,$2,256
	beq	$2,$0,$L30
	nop

	lui	$2,%hi(gpWanPriv)
	lw	$3,%lo(gpWanPriv)($2)
	lw	$2,40($sp)
	sll	$2,$2,5
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	lw	$2,8284($2)
	ext	$2,$2,2,1
	andi	$2,$2,0x00ff
	beq	$2,$0,$L30
	nop

	lui	$2,%hi(gpWanPriv)
	lw	$3,%lo(gpWanPriv)($2)
	lw	$2,40($sp)
	sll	$2,$2,5
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	lw	$2,8284($2)
	ext	$2,$2,14,6
	andi	$3,$2,0x00ff
	li	$2,32			# 0x20
	beq	$3,$2,$L30
	nop

	lui	$2,%hi(gpWanPriv)
	lw	$3,%lo(gpWanPriv)($2)
	lw	$2,40($sp)
	sll	$2,$2,5
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	lw	$2,8284($2)
	ext	$2,$2,14,6
	andi	$3,$2,0x00ff
	li	$2,33			# 0x21
	beq	$3,$2,$L30
	nop

	lui	$2,%hi(gpWanPriv)
	lw	$3,%lo(gpWanPriv)($2)
	lw	$2,40($sp)
	sll	$2,$2,5
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	lw	$2,8284($2)
	ext	$2,$2,5,9
	andi	$2,$2,0xffff
	slt	$2,$2,256
	bne	$2,$0,$L31
	nop

$L30:
	lui	$2,%hi(drop_print_flag)
	lw	$2,%lo(drop_print_flag)($2)
	beq	$2,$0,$L32
	nop

	li	$2,690			# 0x2b2
	sw	$2,16($sp)
	lui	$2,%hi($LC2)
	addiu	$4,$2,%lo($LC2)
	lui	$2,%hi(__func__.38927)
	addiu	$5,$2,%lo(__func__.38927)
	li	$6,690			# 0x2b2
	lui	$2,%hi(__func__.38927)
	addiu	$7,$2,%lo(__func__.38927)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L32:
	li	$2,-1			# 0xffffffffffffffff
	j	$L46
	nop

$L31:
	lw	$3,64($sp)
	lui	$2,%hi(gpWanPriv)
	lw	$4,%lo(gpWanPriv)($2)
	lw	$2,40($sp)
	sll	$2,$2,5
	sll	$5,$2,2
	subu	$2,$5,$2
	addu	$2,$4,$2
	lw	$2,8284($2)
	ext	$2,$2,5,9
	andi	$2,$2,0xffff
	andi	$2,$2,0x00ff
	sb	$2,225($3)
	lw	$2,24($sp)
	beq	$2,$0,$L33
	nop

	lui	$2,%hi(xpon_up_igmp_ani_vlan_filter_hook)
	lw	$2,%lo(xpon_up_igmp_ani_vlan_filter_hook)($2)
	beq	$2,$0,$L33
	nop

	lui	$2,%hi(xpon_up_igmp_ani_vlan_filter_hook)
	lw	$2,%lo(xpon_up_igmp_ani_vlan_filter_hook)($2)
	lw	$3,64($sp)
	move	$4,$3
	jalr	$2
	nop

	move	$3,$2
	li	$2,-1			# 0xffffffffffffffff
	bne	$3,$2,$L33
	nop

	lui	$2,%hi(drop_print_flag)
	lw	$2,%lo(drop_print_flag)($2)
	beq	$2,$0,$L34
	nop

	li	$2,700			# 0x2bc
	sw	$2,16($sp)
	lui	$2,%hi($LC2)
	addiu	$4,$2,%lo($LC2)
	lui	$2,%hi(__func__.38927)
	addiu	$5,$2,%lo(__func__.38927)
	li	$6,700			# 0x2bc
	lui	$2,%hi(__func__.38927)
	addiu	$7,$2,%lo(__func__.38927)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L34:
	li	$2,-1			# 0xffffffffffffffff
	j	$L46
	nop

$L33:
	lw	$2,24($sp)
	bne	$2,$0,$L35
	nop

	lw	$2,64($sp)
	lw	$3,64($sp)
	lw	$3,200($3)
	ori	$3,$3,0x100
	sw	$3,200($2)
	lui	$2,%hi(pon_insert_tag_hook)
	lw	$2,%lo(pon_insert_tag_hook)($2)
	beq	$2,$0,$L35
	nop

	lui	$2,%hi(pon_insert_tag_hook)
	lw	$2,%lo(pon_insert_tag_hook)($2)
	addiu	$3,$sp,64
	move	$4,$3
	jalr	$2
	nop

	move	$3,$2
	li	$2,-1			# 0xffffffffffffffff
	bne	$3,$2,$L35
	nop

	lui	$2,%hi(drop_print_flag)
	lw	$2,%lo(drop_print_flag)($2)
	beq	$2,$0,$L36
	nop

	li	$2,716			# 0x2cc
	sw	$2,16($sp)
	lui	$2,%hi($LC2)
	addiu	$4,$2,%lo($LC2)
	lui	$2,%hi(__func__.38927)
	addiu	$5,$2,%lo(__func__.38927)
	li	$6,716			# 0x2cc
	lui	$2,%hi(__func__.38927)
	addiu	$7,$2,%lo(__func__.38927)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L36:
	li	$2,-1			# 0xffffffffffffffff
	j	$L46
	nop

$L35:
	lw	$2,64($sp)
	lw	$3,64($sp)
	lw	$3,208($3)
	ori	$3,$3,0x4
	sw	$3,208($2)
	lui	$2,%hi(pon_check_mac_hook)
	lw	$2,%lo(pon_check_mac_hook)($2)
	beq	$2,$0,$L37
	nop

	lui	$2,%hi(pon_check_mac_hook)
	lw	$2,%lo(pon_check_mac_hook)($2)
	lw	$3,64($sp)
	move	$4,$3
	jalr	$2
	nop

	move	$3,$2
	li	$2,-1			# 0xffffffffffffffff
	bne	$3,$2,$L37
	nop

	lui	$2,%hi(drop_print_flag)
	lw	$2,%lo(drop_print_flag)($2)
	beq	$2,$0,$L38
	nop

	li	$2,728			# 0x2d8
	sw	$2,16($sp)
	lui	$2,%hi($LC2)
	addiu	$4,$2,%lo($LC2)
	lui	$2,%hi(__func__.38927)
	addiu	$5,$2,%lo(__func__.38927)
	li	$6,728			# 0x2d8
	lui	$2,%hi(__func__.38927)
	addiu	$7,$2,%lo(__func__.38927)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L38:
	li	$2,-1			# 0xffffffffffffffff
	j	$L46
	nop

$L37:
	lui	$2,%hi(xpon_sfu_multicast_protocol_hook)
	lw	$2,%lo(xpon_sfu_multicast_protocol_hook)($2)
	beq	$2,$0,$L39
	nop

	lui	$2,%hi(xpon_sfu_multicast_protocol_hook)
	lw	$2,%lo(xpon_sfu_multicast_protocol_hook)($2)
	lw	$3,64($sp)
	move	$4,$3
	jalr	$2
	nop

	beq	$2,$0,$L39
	nop

	lui	$2,%hi(xpon_sfu_up_multicast_incoming_hook)
	lw	$2,%lo(xpon_sfu_up_multicast_incoming_hook)($2)
	beq	$2,$0,$L39
	nop

	lui	$2,%hi(xpon_sfu_up_multicast_incoming_hook)
	lw	$2,%lo(xpon_sfu_up_multicast_incoming_hook)($2)
	lw	$3,64($sp)
	move	$4,$3
	li	$5,1			# 0x1
	jalr	$2
	nop

	bgtz	$2,$L39
	nop

	lui	$2,%hi(drop_print_flag)
	lw	$2,%lo(drop_print_flag)($2)
	beq	$2,$0,$L40
	nop

	li	$2,738			# 0x2e2
	sw	$2,16($sp)
	lui	$2,%hi($LC2)
	addiu	$4,$2,%lo($LC2)
	lui	$2,%hi(__func__.38927)
	addiu	$5,$2,%lo(__func__.38927)
	li	$6,738			# 0x2e2
	lui	$2,%hi(__func__.38927)
	addiu	$7,$2,%lo(__func__.38927)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L40:
	li	$2,-1			# 0xffffffffffffffff
	j	$L46
	nop

$L39:
	lui	$2,%hi(gpWanPriv)
	lw	$3,%lo(gpWanPriv)($2)
	lw	$2,40($sp)
	sll	$2,$2,5
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	lw	$2,8284($2)
	ext	$2,$2,20,12
	andi	$4,$2,0xffff
	lw	$3,56($sp)
	lw	$2,0($3)
	ins	$2,$4,12,12
	sw	$2,0($3)
	lui	$2,%hi(gpWanPriv)
	lw	$3,%lo(gpWanPriv)($2)
	lw	$2,40($sp)
	sll	$2,$2,5
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	lw	$2,8284($2)
	ext	$2,$2,14,6
	andi	$2,$2,0x00ff
	move	$4,$2
	lw	$3,56($sp)
	lw	$2,0($3)
	ins	$2,$4,3,8
	sw	$2,0($3)
	lui	$2,%hi(ra_sw_nat_hook_magic)
	lw	$2,%lo(ra_sw_nat_hook_magic)($2)
	beq	$2,$0,$L41
	nop

	lui	$2,%hi(ra_sw_nat_hook_magic)
	lw	$2,%lo(ra_sw_nat_hook_magic)($2)
	lw	$3,64($sp)
	move	$4,$3
	li	$5,29302			# 0x7276
	jalr	$2
	nop

	beq	$2,$0,$L41
	nop

	lw	$3,56($sp)
	lw	$2,4($3)
	li	$4,4			# 0x4
	ins	$2,$4,19,3
	sw	$2,4($3)
$L41:
	lui	$2,%hi(ra_sw_nat_hook_tx)
	lw	$2,%lo(ra_sw_nat_hook_tx)($2)
	beq	$2,$0,$L18
	nop

	lw	$2,72($sp)
	li	$3,29306			# 0x727a
	sh	$3,6($2)
	lw	$2,56($sp)
	lw	$2,0($2)
	ext	$2,$2,12,12
	andi	$2,$2,0xffff
	move	$3,$2
	lw	$2,72($sp)
	sh	$3,4($2)
	lw	$2,56($sp)
	lw	$2,0($2)
	ext	$2,$2,3,8
	andi	$3,$2,0x00ff
	lw	$2,28($sp)
	andi	$2,$2,0x00ff
	and	$2,$3,$2
	andi	$4,$2,0x00ff
	lw	$3,72($sp)
	lw	$2,0($3)
	ins	$2,$4,20,8
	sw	$2,0($3)
	addiu	$2,$sp,44
	move	$4,$2
	lui	$2,%hi(xmcs_get_onu_type)
	addiu	$2,$2,%lo(xmcs_get_onu_type)
	jalr	$2
	nop

	lw	$2,36($sp)
	beq	$2,$0,$L42
	nop

	lw	$3,36($sp)
	li	$2,1			# 0x1
	bne	$3,$2,$L43
	nop

	lw	$3,44($sp)
	li	$2,1			# 0x1
	bne	$3,$2,$L43
	nop

$L42:
	lw	$3,72($sp)
	lw	$2,0($3)
	li	$4,1			# 0x1
	ins	$2,$4,4,1
	sw	$2,0($3)
	lw	$2,64($sp)
	lw	$2,148($2)
	andi	$2,$2,0xf0
	srl	$2,$2,4
	andi	$3,$2,0x00ff
	lw	$2,32($sp)
	andi	$2,$2,0x00ff
	and	$2,$3,$2
	andi	$2,$2,0x00ff
	andi	$2,$2,0xf
	andi	$4,$2,0x00ff
	lw	$3,72($sp)
	lw	$2,0($3)
	ins	$2,$4,28,4
	sw	$2,0($3)
	j	$L44
	nop

$L43:
	lw	$3,72($sp)
	lw	$2,0($3)
	ins	$2,$0,4,1
	sw	$2,0($3)
$L44:
	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	lhu	$2,-30816($2)
	andi	$2,$2,0x40
	beq	$2,$0,$L45
	nop

	lui	$2,%hi(jiffies)
	lw	$3,%lo(jiffies)($2)
	lw	$2,72($sp)
	lhu	$2,4($2)
	move	$6,$2
	lui	$2,%hi($LC4)
	addiu	$4,$2,%lo($LC4)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L45:
	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	lhu	$2,-30816($2)
	andi	$2,$2,0x40
	beq	$2,$0,$L18
	nop

	lui	$2,%hi(jiffies)
	lw	$3,%lo(jiffies)($2)
	lw	$2,72($sp)
	lw	$2,0($2)
	ext	$2,$2,20,8
	andi	$2,$2,0x00ff
	move	$6,$2
	lui	$2,%hi($LC5)
	addiu	$4,$2,%lo($LC5)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L18:
	move	$2,$0
$L46:
	lw	$31,52($sp)
	addiu	$sp,$sp,56
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	gwan_prepare_tx_message
	.size	gwan_prepare_tx_message, .-gwan_prepare_tx_message
	.section	.text.isBroadcastPkt,"ax",@progbits
	.align	2
	.globl	isBroadcastPkt
	.set	nomips16
	.set	nomicromips
	.ent	isBroadcastPkt
	.type	isBroadcastPkt, @function
isBroadcastPkt:
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	sw	$4,0($sp)
	lw	$2,0($sp)
	bne	$2,$0,$L48
	nop

	li	$2,-1			# 0xffffffffffffffff
	j	$L49
	nop

$L48:
	lw	$2,0($sp)
	lbu	$3,0($2)
	li	$2,255			# 0xff
	bne	$3,$2,$L50
	nop

	lw	$2,0($sp)
	addiu	$2,$2,1
	lbu	$3,0($2)
	li	$2,255			# 0xff
	bne	$3,$2,$L50
	nop

	lw	$2,0($sp)
	addiu	$2,$2,2
	lbu	$3,0($2)
	li	$2,255			# 0xff
	bne	$3,$2,$L50
	nop

	lw	$2,0($sp)
	addiu	$2,$2,3
	lbu	$3,0($2)
	li	$2,255			# 0xff
	bne	$3,$2,$L50
	nop

	lw	$2,0($sp)
	addiu	$2,$2,4
	lbu	$3,0($2)
	li	$2,255			# 0xff
	bne	$3,$2,$L50
	nop

	lw	$2,0($sp)
	addiu	$2,$2,5
	lbu	$3,0($2)
	li	$2,255			# 0xff
	bne	$3,$2,$L50
	nop

	move	$2,$0
	j	$L49
	nop

$L50:
	li	$2,-1			# 0xffffffffffffffff
$L49:
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	isBroadcastPkt
	.size	isBroadcastPkt, .-isBroadcastPkt
	.section	.text.isUnknownUnicastPkt,"ax",@progbits
	.align	2
	.globl	isUnknownUnicastPkt
	.set	nomips16
	.set	nomicromips
	.ent	isUnknownUnicastPkt
	.type	isUnknownUnicastPkt, @function
isUnknownUnicastPkt:
	.frame	$sp,8,$31		# vars= 8, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-8
	sw	$4,8($sp)
	lui	$2,%hi(gpWanPriv)
	lw	$2,%lo(gpWanPriv)($2)
	lw	$2,8($2)
	lw	$2,364($2)
	sw	$2,0($sp)
	lw	$2,8($sp)
	bne	$2,$0,$L52
	nop

	li	$2,-1			# 0xffffffffffffffff
	j	$L53
	nop

$L52:
	lw	$2,8($sp)
	lbu	$3,0($2)
	lw	$2,0($sp)
	lbu	$2,0($2)
	bne	$3,$2,$L54
	nop

	lw	$2,8($sp)
	addiu	$2,$2,1
	lbu	$3,0($2)
	lw	$2,0($sp)
	addiu	$2,$2,1
	lbu	$2,0($2)
	bne	$3,$2,$L54
	nop

	lw	$2,8($sp)
	addiu	$2,$2,2
	lbu	$3,0($2)
	lw	$2,0($sp)
	addiu	$2,$2,2
	lbu	$2,0($2)
	bne	$3,$2,$L54
	nop

	lw	$2,8($sp)
	addiu	$2,$2,3
	lbu	$3,0($2)
	lw	$2,0($sp)
	addiu	$2,$2,3
	lbu	$2,0($2)
	bne	$3,$2,$L54
	nop

	lw	$2,8($sp)
	addiu	$2,$2,4
	lbu	$3,0($2)
	lw	$2,0($sp)
	addiu	$2,$2,4
	lbu	$2,0($2)
	bne	$3,$2,$L54
	nop

	lw	$2,8($sp)
	addiu	$2,$2,5
	lbu	$3,0($2)
	lw	$2,0($sp)
	addiu	$2,$2,5
	lbu	$2,0($2)
	bne	$3,$2,$L54
	nop

	li	$2,-1			# 0xffffffffffffffff
	j	$L53
	nop

$L54:
	move	$2,$0
$L53:
	addiu	$sp,$sp,8
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	isUnknownUnicastPkt
	.size	isUnknownUnicastPkt, .-isUnknownUnicastPkt
	.section	.text.isMulticastPkt,"ax",@progbits
	.align	2
	.globl	isMulticastPkt
	.set	nomips16
	.set	nomicromips
	.ent	isMulticastPkt
	.type	isMulticastPkt, @function
isMulticastPkt:
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	sw	$4,0($sp)
	lw	$2,0($sp)
	bne	$2,$0,$L56
	nop

	li	$2,-1			# 0xffffffffffffffff
	j	$L57
	nop

$L56:
	lw	$2,0($sp)
	lbu	$3,0($2)
	li	$2,1			# 0x1
	bne	$3,$2,$L58
	nop

	move	$2,$0
	j	$L57
	nop

$L58:
	li	$2,-1			# 0xffffffffffffffff
$L57:
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	isMulticastPkt
	.size	isMulticastPkt, .-isMulticastPkt
	.rdata
	.align	2
$LC6:
	.ascii	"[%lu0ms]%u Reset Drop Timer to %us\012\000"
	.section	.text.gwan_omci_check_start,"ax",@progbits
	.align	2
	.globl	gwan_omci_check_start
	.set	nomips16
	.set	nomicromips
	.ent	gwan_omci_check_start
	.type	gwan_omci_check_start, @function
gwan_omci_check_start:
	.frame	$sp,32,$31		# vars= 8, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-32
	sw	$31,28($sp)
	sw	$4,32($sp)
	lw	$2,32($sp)
	lw	$2,256($2)
	addiu	$2,$2,2
	sw	$2,16($sp)
	lui	$2,%hi(gpWanPriv)
	lw	$3,%lo(gpWanPriv)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	lw	$2,-31788($2)
	andi	$2,$2,0x2000
	beq	$2,$0,$L59
	nop

	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lw	$3,4($2)
	li	$2,5			# 0x5
	bne	$3,$2,$L59
	nop

	lw	$2,16($sp)
	addiu	$2,$2,1
	lbu	$3,0($2)
	li	$2,10			# 0xa
	bne	$3,$2,$L61
	nop

	lw	$2,16($sp)
	addiu	$2,$2,2
	lbu	$2,0($2)
	bne	$2,$0,$L61
	nop

	lw	$2,16($sp)
	addiu	$2,$2,3
	lbu	$3,0($2)
	li	$2,2			# 0x2
	beq	$3,$2,$L62
	nop

$L61:
	lw	$2,16($sp)
	lbu	$2,0($2)
	andi	$3,$2,0xf
	li	$2,8			# 0x8
	beq	$3,$2,$L62
	nop

	lw	$2,16($sp)
	lbu	$2,0($2)
	andi	$3,$2,0xf
	li	$2,4			# 0x4
	bne	$3,$2,$L59
	nop

$L62:
	lui	$2,%hi(cnt.38942)
	lw	$3,%lo(cnt.38942)($2)
	addiu	$4,$3,1
	lui	$2,%hi(cnt.38942)
	sw	$4,%lo(cnt.38942)($2)
	li	$2,1717960704			# 0x66660000
	ori	$2,$2,0x6667
	mult	$3,$2
	mfhi	$2
	sra	$4,$2,3
	sra	$2,$3,31
	subu	$2,$4,$2
	sll	$2,$2,2
	sll	$4,$2,2
	addu	$2,$2,$4
	subu	$2,$3,$2
	bne	$2,$0,$L63
	nop

	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	lhu	$2,-30816($2)
	andi	$2,$2,0x100
	beq	$2,$0,$L63
	nop

	lui	$2,%hi(jiffies)
	lw	$5,%lo(jiffies)($2)
	lui	$2,%hi(cnt.38942)
	lw	$6,%lo(cnt.38942)($2)
	lui	$2,%hi(gpWanPriv)
	lw	$3,%lo(gpWanPriv)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	lw	$3,-31828($2)
	li	$2,274857984			# 0x10620000
	ori	$2,$2,0x4dd3
	multu	$3,$2
	mfhi	$2
	srl	$3,$2,6
	lui	$2,%hi($LC6)
	addiu	$4,$2,%lo($LC6)
	move	$7,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L63:
	lui	$2,%hi(gpWanPriv)
	lw	$3,%lo(gpWanPriv)($2)
	li	$2,33688			# 0x8398
	addu	$4,$3,$2
	lui	$2,%hi(gpWanPriv)
	lw	$3,%lo(gpWanPriv)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	lw	$2,-31828($2)
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

$L59:
	lw	$31,28($sp)
	addiu	$sp,$sp,32
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	gwan_omci_check_start
	.size	gwan_omci_check_start, .-gwan_omci_check_start
	.rdata
	.align	2
$LC7:
	.ascii	"[%lu0ms]Packet Ethernet CRC Error:%.8x, %.8x, %.8x, %.8x"
	.ascii	"\012\000"
	.align	2
$LC8:
	.ascii	"[%lu0ms]RX PKT LEN: %d, Rx Msg: %.8x, %.8x\000"
	.align	2
$LC9:
	.ascii	"[%lu0ms]Packet Sniffer Gemport Id: %x\012\000"
	.align	2
$LC10:
	.ascii	"[%lu0ms]packet sniffer for skb alloc fail\012\000"
	.align	2
$LC11:
	.ascii	"[%lu0ms]Receive frame from multicast GEM port %d.\012\000"
	.align	2
$LC12:
	.ascii	"[%lu0ms]Receive frame from invalid GEM port %d.\012\000"
	.align	2
$LC0:
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	1
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	2
	.byte	-120
	.byte	-75
	.section	.text.gwan_process_rx_message,"ax",@progbits
	.align	2
	.globl	gwan_process_rx_message
	.set	nomips16
	.set	nomicromips
	.ent	gwan_process_rx_message
	.type	gwan_process_rx_message, @function
gwan_process_rx_message:
	.frame	$sp,104,$31		# vars= 72, regs= 2/0, args= 24, gp= 0
	.mask	0x80010000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-104
	sw	$31,100($sp)
	sw	$16,96($sp)
	sw	$4,104($sp)
	sw	$5,108($sp)
	sw	$6,112($sp)
	sw	$7,116($sp)
	sw	$0,36($sp)
	sh	$0,52($sp)
	sb	$0,40($sp)
	lw	$2,104($sp)
	lw	$2,0($2)
	andi	$2,$2,0x1
	beq	$2,$0,$L65
	nop

	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	lhu	$2,-30816($2)
	andi	$2,$2,0x1
	beq	$2,$0,$L65
	nop

	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	lhu	$2,-30816($2)
	andi	$2,$2,0x1
	beq	$2,$0,$L66
	nop

	lui	$2,%hi(jiffies)
	lw	$5,%lo(jiffies)($2)
	lw	$2,104($sp)
	lw	$6,0($2)
	lw	$2,104($sp)
	lw	$7,4($2)
	lw	$2,104($sp)
	lw	$3,8($2)
	lw	$2,104($sp)
	lw	$2,12($2)
	sw	$3,16($sp)
	sw	$2,20($sp)
	lui	$2,%hi($LC7)
	addiu	$4,$2,%lo($LC7)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L66:
	lw	$4,108($sp)
	lw	$5,112($sp)
	lui	$2,%hi(__dump_skb)
	addiu	$2,$2,%lo(__dump_skb)
	jalr	$2
	nop

$L65:
	lw	$2,104($sp)
	lw	$2,0($2)
	andi	$2,$2,0x800
	beq	$2,$0,$L67
	nop

	li	$2,1			# 0x1
	sw	$2,24($sp)
	lui	$2,%hi(gpWanPriv)
	lw	$3,%lo(gpWanPriv)($2)
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lhu	$2,252($2)
	addiu	$2,$2,44
	sll	$2,$2,1
	addu	$2,$3,$2
	lhu	$2,4($2)
	andi	$2,$2,0x7fff
	sw	$2,56($sp)
	lui	$2,%hi(gpWanPriv)
	lw	$3,%lo(gpWanPriv)($2)
	lw	$2,56($sp)
	sll	$2,$2,5
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	lw	$2,8288($2)
	addiu	$4,$2,1
	lw	$2,56($sp)
	sll	$2,$2,5
	sll	$5,$2,2
	subu	$2,$5,$2
	addu	$2,$3,$2
	sw	$4,8288($2)
	lui	$2,%hi(gpWanPriv)
	lw	$4,%lo(gpWanPriv)($2)
	lui	$2,%hi(gpWanPriv)
	lw	$3,%lo(gpWanPriv)($2)
	lw	$2,56($sp)
	sll	$2,$2,5
	sll	$5,$2,2
	subu	$2,$5,$2
	addu	$2,$3,$2
	lw	$3,8296($2)
	lw	$2,112($sp)
	addu	$3,$3,$2
	lw	$2,56($sp)
	sll	$2,$2,5
	sll	$5,$2,2
	subu	$2,$5,$2
	addu	$2,$4,$2
	sw	$3,8296($2)
	lw	$4,108($sp)
	lui	$2,%hi(gwan_omci_check_start)
	addiu	$2,$2,%lo(gwan_omci_check_start)
	jalr	$2
	nop

	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	lhu	$2,-30816($2)
	andi	$2,$2,0x20
	beq	$2,$0,$L70
	nop

	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	lhu	$2,-30816($2)
	andi	$2,$2,0x20
	beq	$2,$0,$L69
	nop

	lui	$2,%hi(jiffies)
	lw	$5,%lo(jiffies)($2)
	lw	$2,104($sp)
	lw	$3,0($2)
	lw	$2,104($sp)
	lw	$2,4($2)
	sw	$2,16($sp)
	lui	$2,%hi($LC8)
	addiu	$4,$2,%lo($LC8)
	lw	$6,112($sp)
	move	$7,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L69:
	lw	$4,108($sp)
	lw	$5,112($sp)
	lui	$2,%hi(__dump_skb)
	addiu	$2,$2,%lo(__dump_skb)
	jalr	$2
	nop

	j	$L70
	nop

$L67:
	li	$2,2			# 0x2
	sw	$2,24($sp)
	lw	$2,108($sp)
	lw	$2,256($2)
	sw	$2,36($sp)
	lw	$4,36($sp)
	lui	$2,%hi(isBroadcastPkt)
	addiu	$2,$2,%lo(isBroadcastPkt)
	jalr	$2
	nop

	bne	$2,$0,$L71
	nop

	lw	$2,24($sp)
	addiu	$2,$2,16
	sw	$2,24($sp)
	j	$L72
	nop

$L71:
	lw	$4,36($sp)
	lui	$2,%hi(isMulticastPkt)
	addiu	$2,$2,%lo(isMulticastPkt)
	jalr	$2
	nop

	bne	$2,$0,$L73
	nop

	lw	$2,24($sp)
	addiu	$2,$2,32
	sw	$2,24($sp)
	j	$L72
	nop

$L73:
	lw	$4,36($sp)
	lui	$2,%hi(isUnknownUnicastPkt)
	addiu	$2,$2,%lo(isUnknownUnicastPkt)
	jalr	$2
	nop

	bne	$2,$0,$L72
	nop

	lw	$2,24($sp)
	addiu	$2,$2,48
	sw	$2,24($sp)
$L72:
	lw	$2,104($sp)
	lw	$2,0($2)
	ext	$2,$2,12,12
	andi	$2,$2,0xffff
	sw	$2,60($sp)
	lui	$2,%hi(gpWanPriv)
	lw	$3,%lo(gpWanPriv)($2)
	lw	$2,60($sp)
	addiu	$2,$2,44
	sll	$2,$2,1
	addu	$2,$3,$2
	lhu	$2,4($2)
	andi	$2,$2,0x7fff
	sw	$2,56($sp)
	lw	$2,56($sp)
	sltu	$2,$2,256
	beq	$2,$0,$L74
	nop

	lui	$2,%hi(gpWanPriv)
	lw	$3,%lo(gpWanPriv)($2)
	lw	$2,56($sp)
	sll	$2,$2,5
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	lw	$2,8284($2)
	ext	$2,$2,2,1
	andi	$2,$2,0x00ff
	beq	$2,$0,$L74
	nop

	lui	$2,%hi(gpWanPriv)
	lw	$3,%lo(gpWanPriv)($2)
	lw	$2,56($sp)
	sll	$2,$2,5
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	lw	$2,8284($2)
	ext	$2,$2,5,9
	andi	$2,$2,0xffff
	slt	$2,$2,256
	bne	$2,$0,$L75
	nop

$L74:
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lw	$3,448($2)
	li	$2,67108864			# 0x4000000
	and	$2,$3,$2
	beq	$2,$0,$L76
	nop

	sw	$0,44($sp)
	li	$2,6			# 0x6
	sw	$2,48($sp)
	lui	$2,%hi($LC0)
	addiu	$3,$2,%lo($LC0)
	lwl	$4,%lo($LC0)($2)
	move	$5,$4
	lwr	$5,3($3)
	addiu	$3,$2,%lo($LC0)
	lwl	$4,4($3)
	lwr	$4,7($3)
	addiu	$3,$2,%lo($LC0)
	lwl	$6,8($3)
	move	$7,$6
	lwr	$7,11($3)
	move	$3,$7
	sw	$5,80($sp)
	sw	$4,84($sp)
	sw	$3,88($sp)
	addiu	$2,$2,%lo($LC0)
	lbu	$3,12($2)
	sb	$3,92($sp)
	lbu	$2,13($2)
	sb	$2,93($sp)
	lw	$2,112($sp)
	xori	$2,$2,0x2c
	sltu	$2,$2,1
	andi	$2,$2,0x00ff
	sw	$2,64($sp)
	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	lhu	$2,-30816($2)
	andi	$2,$2,0x2
	beq	$2,$0,$L77
	nop

	lui	$2,%hi(jiffies)
	lw	$3,%lo(jiffies)($2)
	lui	$2,%hi($LC9)
	addiu	$4,$2,%lo($LC9)
	move	$5,$3
	lw	$6,60($sp)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L77:
	lw	$2,64($sp)
	beq	$2,$0,$L78
	nop

	lw	$2,48($sp)
	addiu	$2,$2,14
	sw	$2,48($sp)
$L78:
	lw	$4,108($sp)
	lw	$5,112($sp)
	lui	$2,%hi(skb_put)
	addiu	$2,$2,%lo(skb_put)
	jalr	$2
	nop

	lw	$2,108($sp)
	sw	$2,68($sp)
	lw	$2,68($sp)
	lw	$2,256($2)
	move	$3,$2
	lw	$2,68($sp)
	lw	$2,252($2)
	subu	$2,$3,$2
	move	$3,$2
	lw	$2,48($sp)
	sltu	$2,$3,$2
	beq	$2,$0,$L80
	nop

	lw	$2,108($sp)
	sw	$2,72($sp)
	lw	$2,72($sp)
	sw	$2,76($sp)
	lw	$2,76($sp)
	lw	$2,84($2)
	bne	$2,$0,$L82
	nop

	lw	$2,72($sp)
	lw	$2,248($2)
	move	$3,$2
	lw	$2,72($sp)
	lw	$2,244($2)
	subu	$2,$3,$2
	j	$L84
	nop

$L82:
	move	$2,$0
$L84:
	lw	$4,108($sp)
	lw	$5,48($sp)
	move	$6,$2
	li	$7,32			# 0x20
	lui	$2,%hi(skb_copy_expand)
	addiu	$2,$2,%lo(skb_copy_expand)
	jalr	$2
	nop

	sw	$2,44($sp)
	j	$L85
	nop

$L80:
	lw	$4,108($sp)
	li	$5,32			# 0x20
	lui	$2,%hi(skb_copy)
	addiu	$2,$2,%lo(skb_copy)
	jalr	$2
	nop

	sw	$2,44($sp)
$L85:
	lw	$2,44($sp)
	bne	$2,$0,$L86
	nop

	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	lhu	$2,-30816($2)
	andi	$2,$2,0x1
	beq	$2,$0,$L88
	nop

	lui	$2,%hi(jiffies)
	lw	$3,%lo(jiffies)($2)
	lui	$2,%hi($LC10)
	addiu	$4,$2,%lo($LC10)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	j	$L88
	nop

$L86:
	lw	$2,64($sp)
	beq	$2,$0,$L89
	nop

	lw	$4,44($sp)
	li	$5,14			# 0xe
	lui	$2,%hi(skb_push)
	addiu	$2,$2,%lo(skb_push)
	jalr	$2
	nop

	lw	$2,44($sp)
	lw	$3,256($2)
	addiu	$2,$sp,80
	move	$4,$3
	move	$5,$2
	li	$6,14			# 0xe
	lui	$2,%hi(memcpy)
	addiu	$2,$2,%lo(memcpy)
	jalr	$2
	nop

$L89:
	lw	$2,44($sp)
	lw	$3,148($2)
	li	$2,67108864			# 0x4000000
	or	$3,$3,$2
	lw	$2,44($sp)
	sw	$3,148($2)
	li	$4,-2147483648			# 0xffffffff80000000
	lw	$5,44($sp)
	lui	$2,%hi(macSend)
	addiu	$2,$2,%lo(macSend)
	jalr	$2
	nop

$L88:
	lw	$2,108($sp)
	lw	$3,80($2)
	lw	$2,112($sp)
	subu	$2,$3,$2
	lw	$4,108($sp)
	move	$5,$2
	lui	$2,%hi(skb_trim)
	addiu	$2,$2,%lo(skb_trim)
	jalr	$2
	nop

	j	$L90
	nop

$L76:
	j	$L90
	nop

$L75:
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lbu	$2,280($2)
	beq	$2,$0,$L91
	nop

	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lbu	$2,280($2)
	move	$4,$2
	lw	$5,108($sp)
	lui	$2,%hi(gpon_dvt_pcp_check)
	addiu	$2,$2,%lo(gpon_dvt_pcp_check)
	jalr	$2
	nop

$L91:
	lui	$2,%hi(gpWanPriv)
	lw	$3,%lo(gpWanPriv)($2)
	lw	$2,56($sp)
	sll	$2,$2,5
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	lw	$2,8288($2)
	addiu	$4,$2,1
	lw	$2,56($sp)
	sll	$2,$2,5
	sll	$5,$2,2
	subu	$2,$5,$2
	addu	$2,$3,$2
	sw	$4,8288($2)
	lui	$2,%hi(gpWanPriv)
	lw	$4,%lo(gpWanPriv)($2)
	lui	$2,%hi(gpWanPriv)
	lw	$3,%lo(gpWanPriv)($2)
	lw	$2,56($sp)
	sll	$2,$2,5
	sll	$5,$2,2
	subu	$2,$5,$2
	addu	$2,$3,$2
	lw	$3,8296($2)
	lw	$2,112($sp)
	addu	$3,$3,$2
	lw	$2,56($sp)
	sll	$2,$2,5
	sll	$5,$2,2
	subu	$2,$5,$2
	addu	$2,$4,$2
	sw	$3,8296($2)
	lui	$2,%hi(gpWanPriv)
	lw	$3,%lo(gpWanPriv)($2)
	lw	$2,56($sp)
	sll	$2,$2,5
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	lw	$2,8284($2)
	ext	$2,$2,14,6
	andi	$3,$2,0x00ff
	li	$2,32			# 0x20
	bne	$3,$2,$L92
	nop

	lw	$3,108($sp)
	lhu	$2,222($3)
	li	$4,1			# 0x1
	ins	$2,$4,15,1
	sh	$2,222($3)
	j	$L93
	nop

$L92:
	lw	$3,108($sp)
	lhu	$2,222($3)
	ins	$2,$0,15,1
	sh	$2,222($3)
$L93:
	lw	$2,60($sp)
	andi	$2,$2,0xffff
	andi	$2,$2,0xfff
	andi	$4,$2,0xffff
	lw	$3,108($sp)
	lhu	$2,222($3)
	ins	$2,$4,3,12
	sh	$2,222($3)
	lui	$2,%hi(gpWanPriv)
	lw	$3,%lo(gpWanPriv)($2)
	lw	$2,56($sp)
	sll	$2,$2,5
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	lw	$2,8284($2)
	ext	$2,$2,5,9
	andi	$2,$2,0xffff
	andi	$3,$2,0x00ff
	lw	$2,108($sp)
	sb	$3,225($2)
	lw	$2,108($sp)
	lbu	$3,224($2)
	li	$2,-128			# 0xffffffffffffff80
	or	$2,$3,$2
	andi	$3,$2,0x00ff
	lw	$2,108($sp)
	sb	$3,224($2)
	lw	$2,108($sp)
	lw	$2,208($2)
	ori	$3,$2,0x40
	lw	$2,108($sp)
	sw	$3,208($2)
	lw	$2,108($sp)
	lw	$2,256($2)
	addiu	$2,$2,12
	sw	$2,36($sp)
	j	$L94
	nop

$L95:
	lbu	$2,40($sp)
	addiu	$2,$2,1
	sb	$2,40($sp)
	lw	$2,36($sp)
	addiu	$2,$2,4
	sw	$2,36($sp)
$L94:
	lw	$2,36($sp)
	lbu	$2,0($2)
	andi	$2,$2,0xffff
	sll	$2,$2,8
	andi	$3,$2,0xffff
	lw	$2,36($sp)
	addiu	$2,$2,1
	lbu	$2,0($2)
	andi	$2,$2,0xffff
	addu	$2,$3,$2
	sh	$2,52($sp)
	lhu	$3,52($sp)
	li	$2,33024			# 0x8100
	beq	$3,$2,$L95
	nop

	lhu	$3,52($sp)
	li	$2,34984			# 0x88a8
	beq	$3,$2,$L95
	nop

	lhu	$3,52($sp)
	li	$2,37120			# 0x9100
	beq	$3,$2,$L95
	nop

	lw	$2,108($sp)
	lbu	$3,40($sp)
	sb	$3,196($2)
	lui	$2,%hi(pon_check_mac_hook)
	lw	$2,%lo(pon_check_mac_hook)($2)
	beq	$2,$0,$L96
	nop

	lui	$2,%hi(pon_check_mac_hook)
	lw	$2,%lo(pon_check_mac_hook)($2)
	lw	$4,108($sp)
	jalr	$2
	nop

	move	$3,$2
	li	$2,-1			# 0xffffffffffffffff
	bne	$3,$2,$L96
	nop

	j	$L90
	nop

$L96:
	lw	$2,108($sp)
	lw	$3,208($2)
	li	$2,-65			# 0xffffffffffffffbf
	and	$3,$3,$2
	lw	$2,108($sp)
	sw	$3,208($2)
	lw	$2,108($sp)
	sb	$0,196($2)
	lui	$2,%hi(gpWanPriv)
	lw	$3,%lo(gpWanPriv)($2)
	lw	$2,56($sp)
	sll	$2,$2,5
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	lw	$2,8284($2)
	ext	$2,$2,4,1
	andi	$2,$2,0x00ff
	beq	$2,$0,$L70
	nop

	lui	$2,%hi(gpWanPriv)
	lw	$3,%lo(gpWanPriv)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	lw	$2,-31788($2)
	andi	$2,$2,0x8000
	beq	$2,$0,$L97
	nop

	lui	$2,%hi(random32)
	addiu	$2,$2,%lo(random32)
	jalr	$2
	nop

	move	$4,$2
	lui	$2,%hi(gpWanPriv)
	lw	$3,%lo(gpWanPriv)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	lw	$2,-32676($2)
	divu	$0,$4,$2
	mfhi	$2
	sw	$2,32($sp)
	sw	$0,28($sp)
	j	$L98
	nop

$L102:
	lui	$2,%hi(gpWanPriv)
	lw	$3,%lo(gpWanPriv)($2)
	lw	$2,28($sp)
	sll	$2,$2,5
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	lw	$2,8284($2)
	ext	$2,$2,2,1
	andi	$2,$2,0x00ff
	beq	$2,$0,$L99
	nop

	lw	$2,32($sp)
	bne	$2,$0,$L100
	nop

	lui	$2,%hi(gpWanPriv)
	lw	$3,%lo(gpWanPriv)($2)
	lw	$2,28($sp)
	sll	$2,$2,5
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	lw	$2,8284($2)
	ext	$2,$2,20,12
	andi	$4,$2,0xffff
	lw	$3,108($sp)
	lhu	$2,222($3)
	ins	$2,$4,3,12
	sh	$2,222($3)
	lw	$2,108($sp)
	lw	$3,148($2)
	li	$2,-241			# 0xffffffffffffff0f
	and	$16,$3,$2
	lui	$2,%hi(random32)
	addiu	$2,$2,%lo(random32)
	jalr	$2
	nop

	andi	$2,$2,0x7
	sll	$2,$2,4
	or	$3,$16,$2
	lw	$2,108($sp)
	sw	$3,148($2)
	j	$L101
	nop

$L100:
	lw	$2,32($sp)
	addiu	$2,$2,-1
	sw	$2,32($sp)
$L99:
	lw	$2,28($sp)
	addiu	$2,$2,1
	sw	$2,28($sp)
$L98:
	lw	$2,28($sp)
	slt	$2,$2,256
	bne	$2,$0,$L102
	nop

$L101:
	lw	$3,28($sp)
	li	$2,256			# 0x100
	bne	$3,$2,$L103
	nop

	lw	$2,108($sp)
	lw	$3,148($2)
	li	$2,-241			# 0xffffffffffffff0f
	and	$16,$3,$2
	lui	$2,%hi(random32)
	addiu	$2,$2,%lo(random32)
	jalr	$2
	nop

	andi	$2,$2,0x7
	sll	$2,$2,4
	or	$3,$16,$2
	lw	$2,108($sp)
	sw	$3,148($2)
	lw	$2,60($sp)
	andi	$2,$2,0xffff
	andi	$2,$2,0xfff
	andi	$4,$2,0xffff
	lw	$3,108($sp)
	lhu	$2,222($3)
	ins	$2,$4,3,12
	sh	$2,222($3)
$L103:
	lw	$2,116($sp)
	li	$3,1			# 0x1
	sb	$3,0($2)
	j	$L70
	nop

$L97:
	lui	$2,%hi(gpWanPriv)
	lw	$3,%lo(gpWanPriv)($2)
	lw	$2,56($sp)
	sll	$2,$2,5
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	lw	$2,8284($2)
	ext	$2,$2,14,6
	andi	$2,$2,0x00ff
	bltz	$2,$L104
	nop

	lui	$2,%hi(gpWanPriv)
	lw	$3,%lo(gpWanPriv)($2)
	lw	$2,56($sp)
	sll	$2,$2,5
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	lw	$2,8284($2)
	ext	$2,$2,14,6
	andi	$2,$2,0x00ff
	slt	$2,$2,32
	beq	$2,$0,$L104
	nop

	lw	$2,60($sp)
	andi	$2,$2,0xffff
	andi	$2,$2,0xfff
	andi	$4,$2,0xffff
	lw	$3,108($sp)
	lhu	$2,222($3)
	ins	$2,$4,3,12
	sh	$2,222($3)
	lw	$2,116($sp)
	li	$3,1			# 0x1
	sb	$3,0($2)
	j	$L70
	nop

$L104:
	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	lhu	$2,-30816($2)
	andi	$2,$2,0x2
	beq	$2,$0,$L70
	nop

	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	lhu	$2,-30816($2)
	andi	$2,$2,0x1
	beq	$2,$0,$L105
	nop

	lui	$2,%hi(jiffies)
	lw	$3,%lo(jiffies)($2)
	lui	$2,%hi($LC11)
	addiu	$4,$2,%lo($LC11)
	move	$5,$3
	lw	$6,60($sp)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L105:
	lw	$4,108($sp)
	lw	$5,112($sp)
	lui	$2,%hi(__dump_skb)
	addiu	$2,$2,%lo(__dump_skb)
	jalr	$2
	nop

$L70:
	lw	$2,24($sp)
	j	$L106
	nop

$L90:
	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	lhu	$2,-30816($2)
	andi	$2,$2,0x2
	beq	$2,$0,$L107
	nop

	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	lhu	$2,-30816($2)
	andi	$2,$2,0x2
	beq	$2,$0,$L108
	nop

	lui	$2,%hi(jiffies)
	lw	$3,%lo(jiffies)($2)
	lui	$2,%hi($LC12)
	addiu	$4,$2,%lo($LC12)
	move	$5,$3
	lw	$6,60($sp)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L108:
	lw	$4,108($sp)
	lw	$5,112($sp)
	lui	$2,%hi(__dump_skb)
	addiu	$2,$2,%lo(__dump_skb)
	jalr	$2
	nop

$L107:
	lw	$2,24($sp)
	nor	$2,$0,$2
$L106:
	lw	$31,100($sp)
	lw	$16,96($sp)
	addiu	$sp,$sp,104
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	gwan_process_rx_message
	.size	gwan_process_rx_message, .-gwan_process_rx_message
	.section	.text.gwan_update_gem_mib_table,"ax",@progbits
	.align	2
	.globl	gwan_update_gem_mib_table
	.set	nomips16
	.set	nomicromips
	.ent	gwan_update_gem_mib_table
	.type	gwan_update_gem_mib_table, @function
gwan_update_gem_mib_table:
	.frame	$sp,40,$31		# vars= 16, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-40
	sw	$31,36($sp)
	sw	$4,40($sp)
	sw	$0,16($sp)
	lui	$2,%hi(gponDevGemMibTablesInit)
	addiu	$2,$2,%lo(gponDevGemMibTablesInit)
	jalr	$2
	nop

	move	$4,$0
	move	$5,$0
	lui	$2,%hi(gponDevUpdateGemMibIdxTable)
	addiu	$2,$2,%lo(gponDevUpdateGemMibIdxTable)
	jalr	$2
	nop

	li	$2,1			# 0x1
	sw	$2,24($sp)
	sw	$0,20($sp)
	j	$L110
	nop

$L113:
	lui	$2,%hi(gpWanPriv)
	lw	$3,%lo(gpWanPriv)($2)
	lw	$2,20($sp)
	addiu	$2,$2,44
	sll	$2,$2,1
	addu	$2,$3,$2
	lhu	$2,4($2)
	andi	$2,$2,0x7fff
	sw	$2,28($sp)
	lw	$2,28($sp)
	sltu	$2,$2,256
	beq	$2,$0,$L111
	nop

	lui	$2,%hi(gpWanPriv)
	lw	$3,%lo(gpWanPriv)($2)
	lw	$2,28($sp)
	sll	$2,$2,5
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	lw	$2,8284($2)
	ext	$2,$2,2,1
	andi	$2,$2,0x00ff
	beq	$2,$0,$L111
	nop

	lw	$2,16($sp)
	addiu	$2,$2,1
	sw	$2,16($sp)
$L111:
	lw	$2,20($sp)
	andi	$3,$2,0xf
	li	$2,15			# 0xf
	bne	$3,$2,$L112
	nop

	lw	$3,24($sp)
	li	$2,256			# 0x100
	beq	$3,$2,$L112
	nop

	lw	$2,24($sp)
	andi	$2,$2,0x00ff
	move	$3,$2
	lw	$2,16($sp)
	andi	$2,$2,0xffff
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(gponDevUpdateGemMibIdxTable)
	addiu	$2,$2,%lo(gponDevUpdateGemMibIdxTable)
	jalr	$2
	nop

	lw	$2,24($sp)
	addiu	$2,$2,1
	sw	$2,24($sp)
$L112:
	lw	$2,20($sp)
	addiu	$2,$2,1
	sw	$2,20($sp)
$L110:
	lw	$2,20($sp)
	slt	$2,$2,4096
	bne	$2,$0,$L113
	nop

	lw	$31,36($sp)
	addiu	$sp,$sp,40
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	gwan_update_gem_mib_table
	.size	gwan_update_gem_mib_table, .-gwan_update_gem_mib_table
	.section	.text.gwan_create_new_tcont,"ax",@progbits
	.align	2
	.globl	gwan_create_new_tcont
	.set	nomips16
	.set	nomicromips
	.ent	gwan_create_new_tcont
	.type	gwan_create_new_tcont, @function
gwan_create_new_tcont:
	.frame	$sp,208,$31		# vars= 184, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-208
	sw	$31,204($sp)
	move	$2,$4
	sh	$2,208($sp)
	sw	$0,16($sp)
	lhu	$2,208($sp)
	move	$4,$2
	lui	$2,%hi(gponDevEnableTCont)
	addiu	$2,$2,%lo(gponDevEnableTCont)
	jalr	$2
	nop

	sb	$2,20($sp)
	lb	$2,20($sp)
	bgez	$2,$L115
	nop

	lb	$2,20($sp)
	slt	$2,$2,32
	beq	$2,$0,$L116
	nop

$L115:
	lbu	$2,20($sp)
	sb	$2,21($sp)
	lui	$2,%hi(gpWanPriv)
	lw	$3,%lo(gpWanPriv)($2)
	lbu	$2,21($sp)
	addiu	$2,$2,12
	sll	$2,$2,1
	addu	$2,$3,$2
	lhu	$3,208($sp)
	sh	$3,4($2)
	lbu	$2,21($sp)
	li	$3,1			# 0x1
	sw	$3,24($sp)
	sw	$0,28($sp)
	sb	$2,32($sp)
	li	$2,1			# 0x1
	sw	$2,36($sp)
	sw	$0,40($sp)
	li	$2,1			# 0x1
	sw	$2,60($sp)
	lw	$2,24($sp)
	sw	$2,64($sp)
	lw	$2,28($sp)
	sw	$2,80($sp)
	lbu	$2,32($sp)
	sw	$2,88($sp)
	lw	$2,36($sp)
	sw	$2,112($sp)
	addiu	$2,$sp,60
	li	$4,18			# 0x12
	move	$5,$0
	move	$6,$2
	lui	$2,%hi(__ECNT_HOOK)
	addiu	$2,$2,%lo(__ECNT_HOOK)
	jalr	$2
	nop

	sw	$2,40($sp)
	lbu	$2,21($sp)
	li	$3,1			# 0x1
	sw	$3,44($sp)
	sb	$2,48($sp)
	li	$2,1			# 0x1
	sw	$2,52($sp)
	sw	$0,56($sp)
	li	$2,3			# 0x3
	sw	$2,60($sp)
	lw	$2,44($sp)
	sw	$2,68($sp)
	lbu	$2,48($sp)
	sw	$2,88($sp)
	lw	$2,52($sp)
	sw	$2,112($sp)
	addiu	$2,$sp,60
	li	$4,18			# 0x12
	move	$5,$0
	move	$6,$2
	lui	$2,%hi(__ECNT_HOOK)
	addiu	$2,$2,%lo(__ECNT_HOOK)
	jalr	$2
	nop

	sw	$2,56($sp)
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lbu	$2,204($2)
	andi	$2,$2,0xffff
	lhu	$3,208($sp)
	beq	$3,$2,$L123
	nop

	lhu	$2,208($sp)
	move	$4,$2
	lui	$2,%hi(gpon_recover_backup_allocId)
	addiu	$2,$2,%lo(gpon_recover_backup_allocId)
	jalr	$2
	nop

$L123:
	move	$2,$0
	j	$L124
	nop

$L116:
	lb	$2,20($sp)
$L124:
	lw	$31,204($sp)
	addiu	$sp,$sp,208
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	gwan_create_new_tcont
	.size	gwan_create_new_tcont, .-gwan_create_new_tcont
	.section	.text.gwan_remove_tcont,"ax",@progbits
	.align	2
	.globl	gwan_remove_tcont
	.set	nomips16
	.set	nomicromips
	.ent	gwan_remove_tcont
	.type	gwan_remove_tcont, @function
gwan_remove_tcont:
	.frame	$sp,208,$31		# vars= 184, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-208
	sw	$31,204($sp)
	move	$2,$4
	sh	$2,208($sp)
	sw	$0,24($sp)
	sb	$0,20($sp)
	sw	$0,16($sp)
	j	$L126
	nop

$L138:
	lui	$2,%hi(gpWanPriv)
	lw	$3,%lo(gpWanPriv)($2)
	lw	$2,16($sp)
	addiu	$2,$2,12
	sll	$2,$2,1
	addu	$2,$3,$2
	lhu	$2,4($2)
	lhu	$3,208($sp)
	bne	$3,$2,$L127
	nop

	lui	$2,%hi(gpWanPriv)
	lw	$2,%lo(gpWanPriv)($2)
	lw	$3,20($2)
	li	$2,2			# 0x2
	bne	$3,$2,$L128
	nop

	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lbu	$2,204($2)
	andi	$2,$2,0xffff
	lhu	$3,208($sp)
	beq	$3,$2,$L128
	nop

	li	$4,1			# 0x1
	lui	$2,%hi(xpon_set_qdma_qos)
	addiu	$2,$2,%lo(xpon_set_qdma_qos)
	jalr	$2
	nop

	li	$2,1			# 0x1
	sb	$2,20($sp)
$L128:
	lui	$2,%hi(gpWanPriv)
	lw	$3,%lo(gpWanPriv)($2)
	lw	$2,16($sp)
	addiu	$2,$2,12
	sll	$2,$2,1
	addu	$2,$3,$2
	li	$3,255			# 0xff
	sh	$3,4($2)
	lw	$2,16($sp)
	andi	$2,$2,0x00ff
	move	$3,$2
	li	$2,1			# 0x1
	sw	$2,28($sp)
	sw	$0,32($sp)
	sb	$3,36($sp)
	sw	$0,40($sp)
	sw	$0,44($sp)
	li	$2,1			# 0x1
	sw	$2,64($sp)
	lw	$2,28($sp)
	sw	$2,68($sp)
	lw	$2,32($sp)
	sw	$2,84($sp)
	lbu	$2,36($sp)
	sw	$2,92($sp)
	lw	$2,40($sp)
	sw	$2,116($sp)
	addiu	$2,$sp,64
	li	$4,18			# 0x12
	move	$5,$0
	move	$6,$2
	lui	$2,%hi(__ECNT_HOOK)
	addiu	$2,$2,%lo(__ECNT_HOOK)
	jalr	$2
	nop

	sw	$2,44($sp)
	lw	$2,16($sp)
	andi	$2,$2,0x00ff
	move	$3,$2
	li	$2,1			# 0x1
	sw	$2,48($sp)
	sb	$3,52($sp)
	sw	$0,56($sp)
	sw	$0,60($sp)
	li	$2,3			# 0x3
	sw	$2,64($sp)
	lw	$2,48($sp)
	sw	$2,72($sp)
	lbu	$2,52($sp)
	sw	$2,92($sp)
	lw	$2,56($sp)
	sw	$2,116($sp)
	addiu	$2,$sp,64
	li	$4,18			# 0x12
	move	$5,$0
	move	$6,$2
	lui	$2,%hi(__ECNT_HOOK)
	addiu	$2,$2,%lo(__ECNT_HOOK)
	jalr	$2
	nop

	sw	$2,60($sp)
	lhu	$2,208($sp)
	move	$4,$2
	lui	$2,%hi(gponDevDisableTCont)
	addiu	$2,$2,%lo(gponDevDisableTCont)
	jalr	$2
	nop

	lbu	$2,20($sp)
	beq	$2,$0,$L135
	nop

	lui	$2,%hi(g_service_change_flag)
	li	$3,1			# 0x1
	sw	$3,%lo(g_service_change_flag)($2)
	sb	$0,20($sp)
$L135:
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lbu	$2,204($2)
	andi	$2,$2,0xffff
	lhu	$3,208($sp)
	beq	$3,$2,$L136
	nop

	lhu	$2,208($sp)
	move	$4,$2
	lui	$2,%hi(gpon_recover_delete_allocId)
	addiu	$2,$2,%lo(gpon_recover_delete_allocId)
	jalr	$2
	nop

$L136:
	move	$2,$0
	j	$L137
	nop

$L127:
	lw	$2,16($sp)
	addiu	$2,$2,1
	sw	$2,16($sp)
$L126:
	lw	$2,16($sp)
	slt	$2,$2,32
	bne	$2,$0,$L138
	nop

	li	$2,-2			# 0xfffffffffffffffe
$L137:
	lw	$31,204($sp)
	addiu	$sp,$sp,208
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	gwan_remove_tcont
	.size	gwan_remove_tcont, .-gwan_remove_tcont
	.section	.text.gwan_remove_all_tcont,"ax",@progbits
	.align	2
	.globl	gwan_remove_all_tcont
	.set	nomips16
	.set	nomicromips
	.ent	gwan_remove_all_tcont
	.type	gwan_remove_all_tcont, @function
gwan_remove_all_tcont:
	.frame	$sp,8,$31		# vars= 8, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-8
	sw	$0,0($sp)
	j	$L140
	nop

$L141:
	lui	$2,%hi(gpWanPriv)
	lw	$3,%lo(gpWanPriv)($2)
	lw	$2,0($sp)
	addiu	$2,$2,12
	sll	$2,$2,1
	addu	$2,$3,$2
	li	$3,255			# 0xff
	sh	$3,4($2)
	lw	$2,0($sp)
	addiu	$2,$2,1
	sw	$2,0($sp)
$L140:
	lw	$2,0($sp)
	slt	$2,$2,32
	bne	$2,$0,$L141
	nop

	move	$2,$0
	addiu	$sp,$sp,8
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	gwan_remove_all_tcont
	.size	gwan_remove_all_tcont, .-gwan_remove_all_tcont
	.section	.text.gwanCheckAllocIdExist,"ax",@progbits
	.align	2
	.globl	gwanCheckAllocIdExist
	.set	nomips16
	.set	nomicromips
	.ent	gwanCheckAllocIdExist
	.type	gwanCheckAllocIdExist, @function
gwanCheckAllocIdExist:
	.frame	$sp,8,$31		# vars= 8, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-8
	move	$2,$4
	sh	$2,8($sp)
	sw	$0,0($sp)
	sw	$0,0($sp)
	j	$L144
	nop

$L147:
	lui	$2,%hi(gpWanPriv)
	lw	$3,%lo(gpWanPriv)($2)
	lw	$2,0($sp)
	addiu	$2,$2,12
	sll	$2,$2,1
	addu	$2,$3,$2
	lhu	$2,4($2)
	lhu	$3,8($sp)
	bne	$3,$2,$L145
	nop

	li	$2,1			# 0x1
	j	$L146
	nop

$L145:
	lw	$2,0($sp)
	addiu	$2,$2,1
	sw	$2,0($sp)
$L144:
	lw	$2,0($sp)
	slt	$2,$2,32
	bne	$2,$0,$L147
	nop

	move	$2,$0
$L146:
	addiu	$sp,$sp,8
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	gwanCheckAllocIdExist
	.size	gwanCheckAllocIdExist, .-gwanCheckAllocIdExist
	.section	.text.gwan_create_new_gemport,"ax",@progbits
	.align	2
	.globl	gwan_create_new_gemport
	.set	nomips16
	.set	nomicromips
	.ent	gwan_create_new_gemport
	.type	gwan_create_new_gemport, @function
gwan_create_new_gemport:
	.frame	$sp,40,$31		# vars= 16, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-40
	sw	$31,36($sp)
	move	$3,$5
	move	$2,$6
	sh	$4,40($sp)
	sb	$3,44($sp)
	sb	$2,48($sp)
	sw	$0,16($sp)
	lhu	$2,40($sp)
	sltu	$2,$2,4096
	beq	$2,$0,$L149
	nop

	lbu	$2,44($sp)
	sltu	$2,$2,32
	bne	$2,$0,$L150
	nop

	lbu	$3,44($sp)
	li	$2,32			# 0x20
	beq	$3,$2,$L150
	nop

	lbu	$3,44($sp)
	li	$2,33			# 0x21
	beq	$3,$2,$L150
	nop

$L149:
	li	$2,-22			# 0xffffffffffffffea
	j	$L151
	nop

$L150:
	lui	$2,%hi(gpWanPriv)
	lw	$3,%lo(gpWanPriv)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	lw	$2,-32676($2)
	sltu	$2,$2,256
	bne	$2,$0,$L152
	nop

	li	$2,-28			# 0xffffffffffffffe4
	j	$L151
	nop

$L152:
	sw	$0,16($sp)
	j	$L153
	nop

$L162:
	lui	$2,%hi(gpWanPriv)
	lw	$3,%lo(gpWanPriv)($2)
	lw	$2,16($sp)
	sll	$2,$2,5
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	lw	$2,8284($2)
	ext	$2,$2,2,1
	andi	$2,$2,0x00ff
	beq	$2,$0,$L154
	nop

	lw	$2,16($sp)
	addiu	$2,$2,1
	sw	$2,16($sp)
	j	$L153
	nop

$L154:
	lui	$2,%hi(gpWanPriv)
	lw	$4,%lo(gpWanPriv)($2)
	lw	$2,16($sp)
	move	$3,$2
	sll	$2,$3,5
	move	$3,$2
	sll	$2,$3,2
	subu	$2,$2,$3
	addiu	$2,$2,8280
	addu	$2,$4,$2
	addiu	$2,$2,4
	move	$4,$2
	move	$5,$0
	li	$6,4			# 0x4
	lui	$2,%hi(memset)
	addiu	$2,$2,%lo(memset)
	jalr	$2
	nop

	lui	$2,%hi(gpWanPriv)
	lw	$4,%lo(gpWanPriv)($2)
	lw	$2,16($sp)
	move	$3,$2
	sll	$2,$3,5
	move	$3,$2
	sll	$2,$3,2
	subu	$2,$2,$3
	addiu	$2,$2,8280
	addu	$2,$4,$2
	addiu	$2,$2,8
	move	$4,$2
	move	$5,$0
	li	$6,92			# 0x5c
	lui	$2,%hi(memset)
	addiu	$2,$2,%lo(memset)
	jalr	$2
	nop

	lui	$2,%hi(gpWanPriv)
	lw	$3,%lo(gpWanPriv)($2)
	lhu	$2,40($sp)
	addiu	$2,$2,44
	sll	$2,$2,1
	addu	$2,$3,$2
	lhu	$3,4($2)
	li	$2,-32768			# 0xffffffffffff8000
	and	$2,$3,$2
	sh	$2,22($sp)
	lui	$2,%hi(gpWanPriv)
	lw	$4,%lo(gpWanPriv)($2)
	lhu	$2,40($sp)
	lw	$3,16($sp)
	andi	$5,$3,0xffff
	lhu	$3,22($sp)
	addu	$3,$5,$3
	andi	$3,$3,0xffff
	addiu	$2,$2,44
	sll	$2,$2,1
	addu	$2,$4,$2
	sh	$3,4($2)
	lui	$2,%hi(gpWanPriv)
	lw	$3,%lo(gpWanPriv)($2)
	lhu	$2,40($sp)
	andi	$2,$2,0xfff
	andi	$4,$2,0xffff
	lw	$2,16($sp)
	sll	$2,$2,5
	sll	$5,$2,2
	subu	$2,$5,$2
	addu	$2,$3,$2
	lw	$3,8284($2)
	ins	$3,$4,20,12
	sw	$3,8284($2)
	lui	$2,%hi(gpWanPriv)
	lw	$3,%lo(gpWanPriv)($2)
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lhu	$2,252($2)
	lhu	$4,40($sp)
	bne	$4,$2,$L155
	nop

	li	$4,511			# 0x1ff
	j	$L156
	nop

$L155:
	li	$4,256			# 0x100
$L156:
	lw	$2,16($sp)
	sll	$2,$2,5
	sll	$5,$2,2
	subu	$2,$5,$2
	addu	$2,$3,$2
	lw	$3,8284($2)
	ins	$3,$4,5,9
	sw	$3,8284($2)
	lui	$2,%hi(gpWanPriv)
	lw	$3,%lo(gpWanPriv)($2)
	lbu	$2,44($sp)
	andi	$2,$2,0x3f
	andi	$4,$2,0x00ff
	lw	$2,16($sp)
	sll	$2,$2,5
	sll	$5,$2,2
	subu	$2,$5,$2
	addu	$2,$3,$2
	lw	$3,8284($2)
	ins	$3,$4,14,6
	sw	$3,8284($2)
	lui	$2,%hi(gpWanPriv)
	lw	$3,%lo(gpWanPriv)($2)
	lbu	$2,48($sp)
	bne	$2,$0,$L157
	nop

	lhu	$2,22($sp)
	beq	$2,$0,$L158
	nop

$L157:
	li	$2,1			# 0x1
	j	$L159
	nop

$L158:
	move	$2,$0
$L159:
	andi	$2,$2,0x00ff
	andi	$2,$2,0x1
	andi	$4,$2,0x00ff
	lw	$2,16($sp)
	sll	$2,$2,5
	sll	$5,$2,2
	subu	$2,$5,$2
	addu	$2,$3,$2
	lw	$3,8284($2)
	ins	$3,$4,3,1
	sw	$3,8284($2)
	lui	$2,%hi(gpWanPriv)
	lw	$3,%lo(gpWanPriv)($2)
	lw	$2,16($sp)
	sll	$2,$2,5
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	lw	$3,8284($2)
	li	$4,1			# 0x1
	ins	$3,$4,2,1
	sw	$3,8284($2)
	lui	$2,%hi(gpWanPriv)
	lw	$2,%lo(gpWanPriv)($2)
	li	$3,65536			# 0x10000
	addu	$3,$2,$3
	lw	$3,-32676($3)
	addiu	$3,$3,1
	li	$4,65536			# 0x10000
	addu	$2,$2,$4
	sw	$3,-32676($2)
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lhu	$2,252($2)
	lhu	$3,40($sp)
	beq	$3,$2,$L160
	nop

	li	$2,255			# 0xff
	sh	$2,20($sp)
	lbu	$3,44($sp)
	li	$2,32			# 0x20
	beq	$3,$2,$L161
	nop

	lui	$2,%hi(gpWanPriv)
	lw	$3,%lo(gpWanPriv)($2)
	lbu	$2,44($sp)
	addiu	$2,$2,12
	sll	$2,$2,1
	addu	$2,$3,$2
	lhu	$2,4($2)
	sh	$2,20($sp)
$L161:
	sw	$0,24($sp)
	sw	$0,28($sp)
	lhu	$2,20($sp)
	sh	$2,26($sp)
	lhu	$2,40($sp)
	sh	$2,24($sp)
	lbu	$2,44($sp)
	sb	$2,28($sp)
	lbu	$2,48($sp)
	sb	$2,29($sp)
	addiu	$2,$sp,24
	move	$4,$2
	lui	$2,%hi(gpon_recover_backup_gemport)
	addiu	$2,$2,%lo(gpon_recover_backup_gemport)
	jalr	$2
	nop

$L160:
	lhu	$5,40($sp)
	lui	$2,%hi(gpWanPriv)
	lw	$3,%lo(gpWanPriv)($2)
	lw	$2,16($sp)
	sll	$2,$2,5
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	lw	$2,8284($2)
	ext	$2,$2,3,1
	andi	$2,$2,0x00ff
	move	$4,$5
	li	$5,1			# 0x1
	move	$6,$2
	lui	$2,%hi(gponDevSetGemInfo)
	addiu	$2,$2,%lo(gponDevSetGemInfo)
	jalr	$2
	nop

	lui	$2,%hi(gpWanPriv)
	lw	$3,%lo(gpWanPriv)($2)
	li	$2,32864			# 0x8060
	addu	$3,$3,$2
	lui	$2,%hi(jiffies)
	lw	$2,%lo(jiffies)($2)
	addiu	$2,$2,200
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(mod_timer)
	addiu	$2,$2,%lo(mod_timer)
	jalr	$2
	nop

	move	$2,$0
	j	$L151
	nop

$L153:
	lw	$2,16($sp)
	slt	$2,$2,256
	bne	$2,$0,$L162
	nop

	li	$2,-2			# 0xfffffffffffffffe
$L151:
	lw	$31,36($sp)
	addiu	$sp,$sp,40
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	gwan_create_new_gemport
	.size	gwan_create_new_gemport, .-gwan_create_new_gemport
	.rdata
	.align	2
$LC13:
	.ascii	"%s %d line change channel err\012\000"
	.align	2
$LC14:
	.ascii	"%s %d line change encryption err\012\000"
	.section	.text.gwan_config_gemport,"ax",@progbits
	.align	2
	.globl	gwan_config_gemport
	.set	nomips16
	.set	nomicromips
	.ent	gwan_config_gemport
	.type	gwan_config_gemport, @function
gwan_config_gemport:
	.frame	$sp,48,$31		# vars= 24, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-48
	sw	$31,44($sp)
	move	$2,$4
	sw	$5,52($sp)
	sw	$6,56($sp)
	sh	$2,48($sp)
	sw	$0,16($sp)
	lhu	$2,48($sp)
	sltu	$2,$2,4096
	bne	$2,$0,$L164
	nop

	li	$2,-22			# 0xffffffffffffffea
	j	$L165
	nop

$L164:
	lui	$2,%hi(gpWanPriv)
	lw	$3,%lo(gpWanPriv)($2)
	lhu	$2,48($sp)
	addiu	$2,$2,44
	sll	$2,$2,1
	addu	$2,$3,$2
	lhu	$2,4($2)
	andi	$2,$2,0x7fff
	sw	$2,20($sp)
	lw	$2,20($sp)
	sltu	$2,$2,256
	beq	$2,$0,$L166
	nop

	lui	$2,%hi(gpWanPriv)
	lw	$3,%lo(gpWanPriv)($2)
	lw	$2,20($sp)
	sll	$2,$2,5
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	lw	$2,8284($2)
	ext	$2,$2,2,1
	andi	$2,$2,0x00ff
	beq	$2,$0,$L166
	nop

	lui	$2,%hi(gpWanPriv)
	lw	$3,%lo(gpWanPriv)($2)
	lw	$2,20($sp)
	sll	$2,$2,5
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	lw	$2,8284($2)
	ext	$2,$2,20,12
	andi	$2,$2,0xffff
	move	$3,$2
	lhu	$2,48($sp)
	beq	$3,$2,$L167
	nop

$L166:
	li	$2,-2			# 0xfffffffffffffffe
	j	$L165
	nop

$L167:
	lhu	$2,48($sp)
	sh	$2,24($sp)
	sw	$0,28($sp)
	sw	$0,32($sp)
	sw	$0,28($sp)
	j	$L168
	nop

$L171:
	lui	$2,%hi(gponRecovery)
	lw	$3,28($sp)
	addiu	$3,$3,16
	sll	$3,$3,3
	addiu	$2,$2,%lo(gponRecovery)
	addu	$2,$3,$2
	lhu	$2,0($2)
	lhu	$3,24($sp)
	bne	$3,$2,$L169
	nop

	lw	$2,28($sp)
	addiu	$2,$2,16
	sll	$3,$2,3
	lui	$2,%hi(gponRecovery)
	addiu	$2,$2,%lo(gponRecovery)
	addu	$2,$3,$2
	sw	$2,32($sp)
	j	$L170
	nop

$L169:
	lw	$2,28($sp)
	addiu	$2,$2,1
	sw	$2,28($sp)
$L168:
	lui	$2,%hi(gponRecovery)
	addiu	$2,$2,%lo(gponRecovery)
	lw	$2,2176($2)
	lw	$3,28($sp)
	slt	$2,$3,$2
	bne	$2,$0,$L171
	nop

$L170:
	lw	$2,32($sp)
	sw	$2,16($sp)
	lw	$2,52($sp)
	li	$3,2			# 0x2
	beq	$2,$3,$L174
	nop

	sltu	$3,$2,3
	beq	$3,$0,$L175
	nop

	li	$3,1			# 0x1
	beq	$2,$3,$L176
	nop

	j	$L173
	nop

$L175:
	li	$3,3			# 0x3
	beq	$2,$3,$L177
	nop

	li	$3,4			# 0x4
	beq	$2,$3,$L178
	nop

	j	$L173
	nop

$L176:
	lui	$2,%hi(gpWanPriv)
	lw	$3,%lo(gpWanPriv)($2)
	lw	$5,56($sp)
	lw	$4,56($sp)
	li	$2,256			# 0x100
	sltu	$5,$5,257
	movn	$2,$4,$5
	andi	$2,$2,0xffff
	andi	$2,$2,0x1ff
	andi	$4,$2,0xffff
	lw	$2,20($sp)
	sll	$2,$2,5
	sll	$5,$2,2
	subu	$2,$5,$2
	addu	$2,$3,$2
	lw	$3,8284($2)
	ins	$3,$4,5,9
	sw	$3,8284($2)
	j	$L179
	nop

$L174:
	lw	$2,56($sp)
	sltu	$2,$2,32
	bne	$2,$0,$L180
	nop

	lw	$3,56($sp)
	li	$2,32			# 0x20
	beq	$3,$2,$L180
	nop

	li	$2,-22			# 0xffffffffffffffea
	j	$L165
	nop

$L180:
	lui	$2,%hi(gpWanPriv)
	lw	$3,%lo(gpWanPriv)($2)
	lw	$2,56($sp)
	andi	$2,$2,0x00ff
	andi	$2,$2,0x3f
	andi	$4,$2,0x00ff
	lw	$2,20($sp)
	sll	$2,$2,5
	sll	$5,$2,2
	subu	$2,$5,$2
	addu	$2,$3,$2
	lw	$3,8284($2)
	ins	$3,$4,14,6
	sw	$3,8284($2)
	lw	$2,16($sp)
	beq	$2,$0,$L181
	nop

	lw	$3,56($sp)
	li	$2,32			# 0x20
	bne	$3,$2,$L182
	nop

	lw	$2,16($sp)
	li	$3,255			# 0xff
	sh	$3,2($2)
	j	$L183
	nop

$L182:
	lui	$2,%hi(gpWanPriv)
	lw	$3,%lo(gpWanPriv)($2)
	lw	$2,56($sp)
	addiu	$2,$2,12
	sll	$2,$2,1
	addu	$2,$3,$2
	lhu	$3,4($2)
	lw	$2,16($sp)
	sh	$3,2($2)
$L183:
	lw	$2,56($sp)
	andi	$3,$2,0x00ff
	lw	$2,16($sp)
	sb	$3,4($2)
	j	$L184
	nop

$L181:
	lui	$2,%hi(gponRecovery)
	addiu	$2,$2,%lo(gponRecovery)
	lbu	$2,2184($2)
	beq	$2,$0,$L184
	nop

	lui	$2,%hi($LC13)
	addiu	$4,$2,%lo($LC13)
	lui	$2,%hi(__func__.39032)
	addiu	$5,$2,%lo(__func__.39032)
	li	$6,1369			# 0x559
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	j	$L179
	nop

$L184:
	j	$L179
	nop

$L177:
	lui	$2,%hi(gpWanPriv)
	lw	$3,%lo(gpWanPriv)($2)
	lw	$2,20($sp)
	sll	$2,$2,5
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	lw	$2,8284($2)
	ext	$2,$2,14,6
	andi	$3,$2,0x00ff
	li	$2,32			# 0x20
	beq	$3,$2,$L185
	nop

	lui	$2,%hi(gpWanPriv)
	lw	$3,%lo(gpWanPriv)($2)
	lw	$2,20($sp)
	sll	$2,$2,5
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	lw	$2,8284($2)
	ext	$2,$2,14,6
	andi	$3,$2,0x00ff
	li	$2,33			# 0x21
	bne	$3,$2,$L186
	nop

$L185:
	li	$2,-22			# 0xffffffffffffffea
	j	$L165
	nop

$L186:
	lui	$2,%hi(gpWanPriv)
	lw	$3,%lo(gpWanPriv)($2)
	lw	$2,56($sp)
	sltu	$2,$0,$2
	andi	$4,$2,0x00ff
	lw	$2,20($sp)
	sll	$2,$2,5
	sll	$5,$2,2
	subu	$2,$5,$2
	addu	$2,$3,$2
	lw	$3,8284($2)
	ins	$3,$4,3,1
	sw	$3,8284($2)
	lhu	$3,48($sp)
	lw	$2,56($sp)
	sltu	$2,$0,$2
	andi	$2,$2,0x00ff
	move	$4,$3
	li	$5,1			# 0x1
	move	$6,$2
	lui	$2,%hi(gponDevSetGemInfo)
	addiu	$2,$2,%lo(gponDevSetGemInfo)
	jalr	$2
	nop

	lw	$2,16($sp)
	beq	$2,$0,$L187
	nop

	lw	$2,56($sp)
	sltu	$2,$0,$2
	andi	$2,$2,0x00ff
	move	$3,$2
	lw	$2,16($sp)
	sb	$3,5($2)
	j	$L188
	nop

$L187:
	lui	$2,%hi(gponRecovery)
	addiu	$2,$2,%lo(gponRecovery)
	lbu	$2,2184($2)
	beq	$2,$0,$L188
	nop

	lui	$2,%hi($LC14)
	addiu	$4,$2,%lo($LC14)
	lui	$2,%hi(__func__.39032)
	addiu	$5,$2,%lo(__func__.39032)
	li	$6,1387			# 0x56b
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	j	$L179
	nop

$L188:
	j	$L179
	nop

$L178:
	lui	$2,%hi(gpWanPriv)
	lw	$3,%lo(gpWanPriv)($2)
	lw	$2,20($sp)
	sll	$2,$2,5
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	lw	$2,8284($2)
	ext	$2,$2,14,6
	andi	$3,$2,0x00ff
	li	$2,32			# 0x20
	beq	$3,$2,$L189
	nop

	lui	$2,%hi(gpWanPriv)
	lw	$3,%lo(gpWanPriv)($2)
	lw	$2,20($sp)
	sll	$2,$2,5
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	lw	$2,8284($2)
	ext	$2,$2,14,6
	andi	$3,$2,0x00ff
	li	$2,33			# 0x21
	bne	$3,$2,$L190
	nop

$L189:
	li	$2,-22			# 0xffffffffffffffea
	j	$L165
	nop

$L190:
	lui	$2,%hi(gpWanPriv)
	lw	$3,%lo(gpWanPriv)($2)
	lw	$2,56($sp)
	sltu	$2,$0,$2
	andi	$4,$2,0x00ff
	lw	$2,20($sp)
	sll	$2,$2,5
	sll	$5,$2,2
	subu	$2,$5,$2
	addu	$2,$3,$2
	lw	$3,8284($2)
	ins	$3,$4,4,1
	sw	$3,8284($2)
	j	$L179
	nop

$L173:
	li	$2,-22			# 0xffffffffffffffea
	j	$L165
	nop

$L179:
	move	$2,$0
$L165:
	lw	$31,44($sp)
	addiu	$sp,$sp,48
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	gwan_config_gemport
	.size	gwan_config_gemport, .-gwan_config_gemport
	.section	.text.gwan_remove_gemport,"ax",@progbits
	.align	2
	.globl	gwan_remove_gemport
	.set	nomips16
	.set	nomicromips
	.ent	gwan_remove_gemport
	.type	gwan_remove_gemport, @function
gwan_remove_gemport:
	.frame	$sp,32,$31		# vars= 8, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-32
	sw	$31,28($sp)
	move	$2,$4
	sh	$2,32($sp)
	lhu	$2,32($sp)
	sltu	$2,$2,4096
	beq	$2,$0,$L192
	nop

	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lhu	$2,252($2)
	lhu	$3,32($sp)
	bne	$3,$2,$L193
	nop

$L192:
	li	$2,-22			# 0xffffffffffffffea
	j	$L194
	nop

$L193:
	lui	$2,%hi(gpWanPriv)
	lw	$3,%lo(gpWanPriv)($2)
	lhu	$2,32($sp)
	addiu	$2,$2,44
	sll	$2,$2,1
	addu	$2,$3,$2
	lhu	$2,4($2)
	andi	$2,$2,0x7fff
	sw	$2,16($sp)
	lw	$2,16($sp)
	sltu	$2,$2,256
	beq	$2,$0,$L195
	nop

	lui	$2,%hi(gpWanPriv)
	lw	$3,%lo(gpWanPriv)($2)
	lw	$2,16($sp)
	sll	$2,$2,5
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	lw	$2,8284($2)
	ext	$2,$2,2,1
	andi	$2,$2,0x00ff
	bne	$2,$0,$L196
	nop

$L195:
	li	$2,-2			# 0xfffffffffffffffe
	j	$L194
	nop

$L196:
	lui	$2,%hi(gpWanPriv)
	lw	$4,%lo(gpWanPriv)($2)
	lw	$2,16($sp)
	move	$3,$2
	sll	$2,$3,5
	move	$3,$2
	sll	$2,$3,2
	subu	$2,$2,$3
	addiu	$2,$2,8280
	addu	$2,$4,$2
	addiu	$2,$2,4
	move	$4,$2
	move	$5,$0
	li	$6,4			# 0x4
	lui	$2,%hi(memset)
	addiu	$2,$2,%lo(memset)
	jalr	$2
	nop

	lui	$2,%hi(gpWanPriv)
	lw	$4,%lo(gpWanPriv)($2)
	lw	$2,16($sp)
	move	$3,$2
	sll	$2,$3,5
	move	$3,$2
	sll	$2,$3,2
	subu	$2,$2,$3
	addiu	$2,$2,8280
	addu	$2,$4,$2
	addiu	$2,$2,8
	move	$4,$2
	move	$5,$0
	li	$6,92			# 0x5c
	lui	$2,%hi(memset)
	addiu	$2,$2,%lo(memset)
	jalr	$2
	nop

	lui	$2,%hi(gpWanPriv)
	lw	$4,%lo(gpWanPriv)($2)
	lhu	$2,32($sp)
	lui	$3,%hi(gpWanPriv)
	lw	$5,%lo(gpWanPriv)($3)
	lhu	$3,32($sp)
	addiu	$3,$3,44
	sll	$3,$3,1
	addu	$3,$5,$3
	lhu	$3,4($3)
	ori	$3,$3,0x7fff
	andi	$3,$3,0xffff
	addiu	$2,$2,44
	sll	$2,$2,1
	addu	$2,$4,$2
	sh	$3,4($2)
	lui	$2,%hi(gpWanPriv)
	lw	$2,%lo(gpWanPriv)($2)
	li	$3,65536			# 0x10000
	addu	$3,$2,$3
	lw	$3,-32676($3)
	addiu	$3,$3,-1
	li	$4,65536			# 0x10000
	addu	$2,$2,$4
	sw	$3,-32676($2)
	lhu	$2,32($sp)
	move	$4,$2
	move	$5,$0
	move	$6,$0
	lui	$2,%hi(gponDevSetGemInfo)
	addiu	$2,$2,%lo(gponDevSetGemInfo)
	jalr	$2
	nop

	lui	$2,%hi(gpWanPriv)
	lw	$3,%lo(gpWanPriv)($2)
	li	$2,32864			# 0x8060
	addu	$3,$3,$2
	lui	$2,%hi(jiffies)
	lw	$2,%lo(jiffies)($2)
	addiu	$2,$2,200
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(mod_timer)
	addiu	$2,$2,%lo(mod_timer)
	jalr	$2
	nop

	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lhu	$2,252($2)
	lhu	$3,32($sp)
	beq	$3,$2,$L197
	nop

	lhu	$2,32($sp)
	move	$4,$2
	lui	$2,%hi(gpon_recover_delete_gemport)
	addiu	$2,$2,%lo(gpon_recover_delete_gemport)
	jalr	$2
	nop

$L197:
	move	$2,$0
$L194:
	lw	$31,28($sp)
	addiu	$sp,$sp,32
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	gwan_remove_gemport
	.size	gwan_remove_gemport, .-gwan_remove_gemport
	.section	.text.gwan_remove_all_gemport_for_disable,"ax",@progbits
	.align	2
	.globl	gwan_remove_all_gemport_for_disable
	.set	nomips16
	.set	nomicromips
	.ent	gwan_remove_all_gemport_for_disable
	.type	gwan_remove_all_gemport_for_disable, @function
gwan_remove_all_gemport_for_disable:
	.frame	$sp,32,$31		# vars= 8, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-32
	sw	$31,28($sp)
	sw	$0,16($sp)
	j	$L199
	nop

$L201:
	lui	$2,%hi(gpWanPriv)
	lw	$3,%lo(gpWanPriv)($2)
	lw	$2,16($sp)
	sll	$2,$2,5
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	lw	$2,8284($2)
	ext	$2,$2,2,1
	andi	$2,$2,0x00ff
	beq	$2,$0,$L200
	nop

	lui	$2,%hi(gpWanPriv)
	lw	$3,%lo(gpWanPriv)($2)
	lw	$2,16($sp)
	sll	$2,$2,5
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	lw	$2,8284($2)
	ext	$2,$2,20,12
	andi	$2,$2,0xffff
	move	$3,$2
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lhu	$2,252($2)
	beq	$3,$2,$L200
	nop

	lui	$2,%hi(gpWanPriv)
	lw	$3,%lo(gpWanPriv)($2)
	lw	$2,16($sp)
	sll	$2,$2,5
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	lw	$2,8284($2)
	ext	$2,$2,20,12
	andi	$2,$2,0xffff
	sh	$2,20($sp)
	lui	$2,%hi(gpWanPriv)
	lw	$3,%lo(gpWanPriv)($2)
	lhu	$2,20($sp)
	addiu	$2,$2,44
	sll	$2,$2,1
	addu	$2,$3,$2
	li	$3,32767			# 0x7fff
	sh	$3,4($2)
	lui	$2,%hi(gpWanPriv)
	lw	$4,%lo(gpWanPriv)($2)
	lw	$2,16($sp)
	move	$3,$2
	sll	$2,$3,5
	move	$3,$2
	sll	$2,$3,2
	subu	$2,$2,$3
	addiu	$2,$2,8280
	addu	$2,$4,$2
	addiu	$2,$2,4
	move	$4,$2
	move	$5,$0
	li	$6,4			# 0x4
	lui	$2,%hi(memset)
	addiu	$2,$2,%lo(memset)
	jalr	$2
	nop

	lui	$2,%hi(gpWanPriv)
	lw	$4,%lo(gpWanPriv)($2)
	lw	$2,16($sp)
	move	$3,$2
	sll	$2,$3,5
	move	$3,$2
	sll	$2,$3,2
	subu	$2,$2,$3
	addiu	$2,$2,8280
	addu	$2,$4,$2
	addiu	$2,$2,8
	move	$4,$2
	move	$5,$0
	li	$6,92			# 0x5c
	lui	$2,%hi(memset)
	addiu	$2,$2,%lo(memset)
	jalr	$2
	nop

$L200:
	lw	$2,16($sp)
	addiu	$2,$2,1
	sw	$2,16($sp)
$L199:
	lw	$2,16($sp)
	slt	$2,$2,256
	bne	$2,$0,$L201
	nop

	lui	$2,%hi(gpWanPriv)
	lw	$3,%lo(gpWanPriv)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	sw	$0,-32676($2)
	move	$4,$0
	lui	$2,%hi(gwan_update_gem_mib_table)
	addiu	$2,$2,%lo(gwan_update_gem_mib_table)
	jalr	$2
	nop

	move	$2,$0
	lw	$31,28($sp)
	addiu	$sp,$sp,32
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	gwan_remove_all_gemport_for_disable
	.size	gwan_remove_all_gemport_for_disable, .-gwan_remove_all_gemport_for_disable
	.section	.text.gwan_remove_all_gemport,"ax",@progbits
	.align	2
	.globl	gwan_remove_all_gemport
	.set	nomips16
	.set	nomicromips
	.ent	gwan_remove_all_gemport
	.type	gwan_remove_all_gemport, @function
gwan_remove_all_gemport:
	.frame	$sp,32,$31		# vars= 8, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-32
	sw	$31,28($sp)
	sw	$0,16($sp)
	j	$L204
	nop

$L206:
	lui	$2,%hi(gpWanPriv)
	lw	$3,%lo(gpWanPriv)($2)
	lw	$2,16($sp)
	sll	$2,$2,5
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	lw	$2,8284($2)
	ext	$2,$2,2,1
	andi	$2,$2,0x00ff
	beq	$2,$0,$L205
	nop

	lui	$2,%hi(gpWanPriv)
	lw	$3,%lo(gpWanPriv)($2)
	lw	$2,16($sp)
	sll	$2,$2,5
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	lw	$2,8284($2)
	ext	$2,$2,20,12
	andi	$2,$2,0xffff
	move	$3,$2
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lhu	$2,252($2)
	beq	$3,$2,$L205
	nop

	lui	$2,%hi(gpWanPriv)
	lw	$3,%lo(gpWanPriv)($2)
	lw	$2,16($sp)
	sll	$2,$2,5
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	lw	$2,8284($2)
	ext	$2,$2,20,12
	andi	$2,$2,0xffff
	sh	$2,20($sp)
	lui	$2,%hi(gpWanPriv)
	lw	$3,%lo(gpWanPriv)($2)
	lhu	$2,20($sp)
	addiu	$2,$2,44
	sll	$2,$2,1
	addu	$2,$3,$2
	li	$3,32767			# 0x7fff
	sh	$3,4($2)
	lui	$2,%hi(gpWanPriv)
	lw	$4,%lo(gpWanPriv)($2)
	lw	$2,16($sp)
	move	$3,$2
	sll	$2,$3,5
	move	$3,$2
	sll	$2,$3,2
	subu	$2,$2,$3
	addiu	$2,$2,8280
	addu	$2,$4,$2
	addiu	$2,$2,4
	move	$4,$2
	move	$5,$0
	li	$6,4			# 0x4
	lui	$2,%hi(memset)
	addiu	$2,$2,%lo(memset)
	jalr	$2
	nop

	lui	$2,%hi(gpWanPriv)
	lw	$4,%lo(gpWanPriv)($2)
	lw	$2,16($sp)
	move	$3,$2
	sll	$2,$3,5
	move	$3,$2
	sll	$2,$3,2
	subu	$2,$2,$3
	addiu	$2,$2,8280
	addu	$2,$4,$2
	addiu	$2,$2,8
	move	$4,$2
	move	$5,$0
	li	$6,92			# 0x5c
	lui	$2,%hi(memset)
	addiu	$2,$2,%lo(memset)
	jalr	$2
	nop

	lhu	$2,20($sp)
	move	$4,$2
	move	$5,$0
	move	$6,$0
	lui	$2,%hi(gponDevSetGemInfo)
	addiu	$2,$2,%lo(gponDevSetGemInfo)
	jalr	$2
	nop

$L205:
	lw	$2,16($sp)
	addiu	$2,$2,1
	sw	$2,16($sp)
$L204:
	lw	$2,16($sp)
	slt	$2,$2,256
	bne	$2,$0,$L206
	nop

	lui	$2,%hi(gpWanPriv)
	lw	$3,%lo(gpWanPriv)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	sw	$0,-32676($2)
	lui	$2,%hi(gponDevResetGemInfo)
	addiu	$2,$2,%lo(gponDevResetGemInfo)
	jalr	$2
	nop

	move	$4,$0
	lui	$2,%hi(gwan_update_gem_mib_table)
	addiu	$2,$2,%lo(gwan_update_gem_mib_table)
	jalr	$2
	nop

	move	$2,$0
	lw	$31,28($sp)
	addiu	$sp,$sp,32
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	gwan_remove_all_gemport
	.size	gwan_remove_all_gemport, .-gwan_remove_all_gemport
	.section	.text.gwan_is_gemport_valid,"ax",@progbits
	.align	2
	.globl	gwan_is_gemport_valid
	.set	nomips16
	.set	nomicromips
	.ent	gwan_is_gemport_valid
	.type	gwan_is_gemport_valid, @function
gwan_is_gemport_valid:
	.frame	$sp,8,$31		# vars= 8, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-8
	move	$2,$4
	sh	$2,8($sp)
	lhu	$2,8($sp)
	sltu	$2,$2,4096
	bne	$2,$0,$L209
	nop

	li	$2,-22			# 0xffffffffffffffea
	j	$L210
	nop

$L209:
	lui	$2,%hi(gpWanPriv)
	lw	$3,%lo(gpWanPriv)($2)
	lhu	$2,8($sp)
	addiu	$2,$2,44
	sll	$2,$2,1
	addu	$2,$3,$2
	lhu	$2,4($2)
	andi	$2,$2,0x7fff
	sw	$2,0($sp)
	lw	$2,0($sp)
	sltu	$2,$2,256
	beq	$2,$0,$L211
	nop

	lui	$2,%hi(gpWanPriv)
	lw	$3,%lo(gpWanPriv)($2)
	lw	$2,0($sp)
	sll	$2,$2,5
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	lw	$2,8284($2)
	ext	$2,$2,2,1
	andi	$2,$2,0x00ff
	j	$L213
	nop

$L211:
	li	$2,-1			# 0xffffffffffffffff
$L213:
	nop
$L210:
	addiu	$sp,$sp,8
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	gwan_is_gemport_valid
	.size	gwan_is_gemport_valid, .-gwan_is_gemport_valid
	.rdata
	.align	2
$LC15:
	.ascii	"%d\012\000"
	.section	.text.service_change_read_proc,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	service_change_read_proc
	.type	service_change_read_proc, @function
service_change_read_proc:
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
	lui	$2,%hi(g_service_change_flag)
	lw	$3,%lo(g_service_change_flag)($2)
	lw	$4,32($sp)
	lui	$2,%hi($LC15)
	addiu	$5,$2,%lo($LC15)
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
	beq	$2,$0,$L215
	nop

	lw	$2,44($sp)
	sw	$2,16($sp)
	j	$L216
	nop

$L215:
	lw	$2,48($sp)
	li	$3,1			# 0x1
	sw	$3,0($2)
$L216:
	lw	$2,16($sp)
	bgez	$2,$L217
	nop

	sw	$0,16($sp)
$L217:
	lw	$2,16($sp)
	lw	$31,28($sp)
	addiu	$sp,$sp,32
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	service_change_read_proc
	.size	service_change_read_proc, .-service_change_read_proc
	.rdata
	.align	2
$LC16:
	.ascii	"%d\000"
	.section	.text.service_change_write_proc,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	service_change_write_proc
	.type	service_change_write_proc, @function
service_change_write_proc:
	.frame	$sp,64,$31		# vars= 40, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	addiu	$sp,$sp,-64
	sw	$31,60($sp)
	sw	$4,64($sp)
	sw	$5,68($sp)
	sw	$6,72($sp)
	sw	$7,76($sp)
	sw	$0,44($sp)
	sw	$0,48($sp)
	sw	$0,52($sp)
	lw	$2,72($sp)
	sltu	$2,$2,8
	bne	$2,$0,$L220
	li	$2,-22			# 0xffffffffffffffea
	j	$L224
$L220:
	addiu	$2,$sp,44
	sw	$2,20($sp)
	lw	$2,68($sp)
	sw	$2,24($sp)
	lw	$2,72($sp)
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
	beq	$2,$0,$L222
	lw	$4,20($sp)
	lw	$5,24($sp)
	lw	$6,16($sp)
#APP
 # 1532 "/opt/tclinux_phoenix/modules/private/xpon/src/pwan/gpon_wan.c" 1
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
$L222:
	lw	$2,16($sp)
	beq	$2,$0,$L223
	li	$2,-14			# 0xfffffffffffffff2
	j	$L224
$L223:
	lw	$2,72($sp)
	addiu	$3,$sp,16
	addu	$2,$3,$2
	sb	$0,28($2)
	addiu	$2,$sp,44
	addiu	$3,$sp,52
	move	$4,$2
	lui	$2,%hi($LC16)
	addiu	$5,$2,%lo($LC16)
	move	$6,$3
	lui	$2,%hi(sscanf)
	addiu	$2,$2,%lo(sscanf)
	jalr	$2
	lw	$2,52($sp)
	move	$3,$2
	lui	$2,%hi(g_service_change_flag)
	sw	$3,%lo(g_service_change_flag)($2)
	lw	$2,72($sp)
$L224:
	lw	$31,60($sp)
	addiu	$sp,$sp,64
	j	$31
	.end	service_change_write_proc
	.size	service_change_write_proc, .-service_change_write_proc
	.rdata
	.align	2
$LC17:
	.ascii	"tc3162/service_change\000"
	.align	2
$LC18:
	.ascii	"creat service change proc faile.\012\000"
	.section	.text.service_change_proc_init,"ax",@progbits
	.align	2
	.globl	service_change_proc_init
	.set	nomips16
	.set	nomicromips
	.ent	service_change_proc_init
	.type	service_change_proc_init, @function
service_change_proc_init:
	.frame	$sp,32,$31		# vars= 8, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-32
	sw	$31,28($sp)
	lui	$2,%hi(g_service_change_flag)
	sw	$0,%lo(g_service_change_flag)($2)
	sw	$0,16($sp)
	lui	$2,%hi($LC17)
	addiu	$4,$2,%lo($LC17)
	move	$5,$0
	move	$6,$0
	lui	$2,%hi(create_proc_entry)
	addiu	$2,$2,%lo(create_proc_entry)
	jalr	$2
	nop

	sw	$2,16($sp)
	lw	$2,16($sp)
	bne	$2,$0,$L226
	nop

	lui	$2,%hi($LC18)
	addiu	$4,$2,%lo($LC18)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	li	$2,-1			# 0xffffffffffffffff
	j	$L227
	nop

$L226:
	lw	$2,16($sp)
	lui	$3,%hi(service_change_read_proc)
	addiu	$3,$3,%lo(service_change_read_proc)
	sw	$3,64($2)
	lw	$2,16($sp)
	lui	$3,%hi(service_change_write_proc)
	addiu	$3,$3,%lo(service_change_write_proc)
	sw	$3,68($2)
	move	$2,$0
$L227:
	lw	$31,28($sp)
	addiu	$sp,$sp,32
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	service_change_proc_init
	.size	service_change_proc_init, .-service_change_proc_init
	.section	.text.gwan_get_traffic_status,"ax",@progbits
	.align	2
	.globl	gwan_get_traffic_status
	.set	nomips16
	.set	nomicromips
	.ent	gwan_get_traffic_status
	.type	gwan_get_traffic_status, @function
gwan_get_traffic_status:
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lw	$3,464($2)
	li	$2,-2147483648			# 0xffffffff80000000
	and	$2,$3,$2
	bne	$2,$0,$L229
	nop

	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lw	$3,4($2)
	li	$2,6			# 0x6
	bne	$3,$2,$L230
	nop

$L229:
	move	$2,$0
	j	$L231
	nop

$L230:
	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	lw	$3,-30236($2)
	li	$2,2			# 0x2
	bne	$3,$2,$L232
	nop

	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lw	$3,4($2)
	li	$2,5			# 0x5
	beq	$3,$2,$L233
	nop

$L232:
	li	$2,1			# 0x1
	j	$L231
	nop

$L233:
	move	$2,$0
$L231:
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	gwan_get_traffic_status
	.size	gwan_get_traffic_status, .-gwan_get_traffic_status
	.section	.text.gwan_init,"ax",@progbits
	.align	2
	.globl	gwan_init
	.set	nomips16
	.set	nomicromips
	.ent	gwan_init
	.type	gwan_init, @function
gwan_init:
	.frame	$sp,256,$31		# vars= 232, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-256
	sw	$31,252($sp)
	sw	$4,256($sp)
	sw	$0,16($sp)
	j	$L235
	nop

$L236:
	lw	$3,256($sp)
	lw	$2,16($sp)
	sll	$2,$2,1
	addu	$2,$3,$2
	li	$3,255			# 0xff
	sh	$3,0($2)
	lw	$2,16($sp)
	addiu	$2,$2,1
	sw	$2,16($sp)
$L235:
	lw	$2,16($sp)
	slt	$2,$2,32
	bne	$2,$0,$L236
	nop

	sw	$0,16($sp)
	j	$L237
	nop

$L238:
	lw	$3,256($sp)
	lw	$2,16($sp)
	addiu	$2,$2,32
	sll	$2,$2,1
	addu	$2,$3,$2
	li	$3,32767			# 0x7fff
	sh	$3,0($2)
	lw	$2,16($sp)
	addiu	$2,$2,1
	sw	$2,16($sp)
$L237:
	lw	$2,16($sp)
	slt	$2,$2,4096
	bne	$2,$0,$L238
	nop

	sw	$0,16($sp)
	j	$L239
	nop

$L240:
	lw	$2,16($sp)
	move	$3,$2
	sll	$2,$3,5
	move	$3,$2
	sll	$2,$3,2
	subu	$2,$2,$3
	addiu	$2,$2,8256
	lw	$3,256($sp)
	addu	$2,$3,$2
	move	$4,$2
	move	$5,$0
	li	$6,4			# 0x4
	lui	$2,%hi(memset)
	addiu	$2,$2,%lo(memset)
	jalr	$2
	nop

	lw	$2,16($sp)
	move	$3,$2
	sll	$2,$3,5
	move	$3,$2
	sll	$2,$3,2
	subu	$2,$2,$3
	addiu	$2,$2,8256
	lw	$3,256($sp)
	addu	$2,$3,$2
	addiu	$2,$2,4
	move	$4,$2
	move	$5,$0
	li	$6,92			# 0x5c
	lui	$2,%hi(memset)
	addiu	$2,$2,%lo(memset)
	jalr	$2
	nop

	lw	$2,16($sp)
	addiu	$2,$2,1
	sw	$2,16($sp)
$L239:
	lw	$2,16($sp)
	slt	$2,$2,256
	bne	$2,$0,$L240
	nop

	lw	$3,256($sp)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	sw	$0,-32704($2)
	lw	$3,256($sp)
	li	$2,32836			# 0x8044
	addu	$2,$3,$2
	move	$4,$2
	move	$5,$0
	move	$6,$0
	lui	$2,%hi(init_timer_key)
	addiu	$2,$2,%lo(init_timer_key)
	jalr	$2
	nop

	lw	$3,256($sp)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	lui	$3,%hi(gwan_update_gem_mib_table)
	addiu	$3,$3,%lo(gwan_update_gem_mib_table)
	sw	$3,-32684($2)
	lui	$2,%hi(service_change_proc_init)
	addiu	$2,$2,%lo(service_change_proc_init)
	jalr	$2
	nop

	li	$4,1			# 0x1
	lui	$2,%hi(pwan_create_net_interface)
	addiu	$2,$2,%lo(pwan_create_net_interface)
	jalr	$2
	nop

	sw	$0,16($sp)
	j	$L241
	nop

$L245:
	lw	$2,16($sp)
	andi	$2,$2,0x00ff
	move	$3,$2
	li	$2,1			# 0x1
	sw	$2,20($sp)
	sw	$0,24($sp)
	sb	$3,28($sp)
	sw	$0,32($sp)
	sw	$0,36($sp)
	li	$2,1			# 0x1
	sw	$2,96($sp)
	lw	$2,20($sp)
	sw	$2,100($sp)
	lw	$2,24($sp)
	sw	$2,116($sp)
	lbu	$2,28($sp)
	sw	$2,124($sp)
	lw	$2,32($sp)
	sw	$2,148($sp)
	addiu	$2,$sp,96
	li	$4,18			# 0x12
	move	$5,$0
	move	$6,$2
	lui	$2,%hi(__ECNT_HOOK)
	addiu	$2,$2,%lo(__ECNT_HOOK)
	jalr	$2
	nop

	sw	$2,36($sp)
	lw	$2,16($sp)
	addiu	$2,$2,1
	sw	$2,16($sp)
$L241:
	lw	$2,16($sp)
	slt	$2,$2,32
	bne	$2,$0,$L245
	nop

	sw	$0,16($sp)
	j	$L246
	nop

$L250:
	lw	$2,16($sp)
	andi	$2,$2,0x00ff
	move	$3,$2
	li	$2,1			# 0x1
	sw	$2,40($sp)
	li	$2,1			# 0x1
	sw	$2,44($sp)
	sb	$3,48($sp)
	sw	$0,52($sp)
	sw	$0,56($sp)
	li	$2,1			# 0x1
	sw	$2,96($sp)
	lw	$2,40($sp)
	sw	$2,100($sp)
	lw	$2,44($sp)
	sw	$2,116($sp)
	lbu	$2,48($sp)
	sw	$2,124($sp)
	lw	$2,52($sp)
	sw	$2,148($sp)
	addiu	$2,$sp,96
	li	$4,18			# 0x12
	move	$5,$0
	move	$6,$2
	lui	$2,%hi(__ECNT_HOOK)
	addiu	$2,$2,%lo(__ECNT_HOOK)
	jalr	$2
	nop

	sw	$2,56($sp)
	lw	$2,16($sp)
	addiu	$2,$2,1
	sw	$2,16($sp)
$L246:
	lw	$2,16($sp)
	slt	$2,$2,16
	bne	$2,$0,$L250
	nop

	sw	$0,16($sp)
	j	$L251
	nop

$L255:
	lw	$2,16($sp)
	andi	$2,$2,0x00ff
	move	$3,$2
	li	$2,1			# 0x1
	sw	$2,60($sp)
	li	$2,1			# 0x1
	sw	$2,64($sp)
	sb	$3,68($sp)
	li	$2,1			# 0x1
	sw	$2,72($sp)
	sw	$0,76($sp)
	li	$2,1			# 0x1
	sw	$2,96($sp)
	lw	$2,60($sp)
	sw	$2,100($sp)
	lw	$2,64($sp)
	sw	$2,116($sp)
	lbu	$2,68($sp)
	sw	$2,124($sp)
	lw	$2,72($sp)
	sw	$2,148($sp)
	addiu	$2,$sp,96
	li	$4,18			# 0x12
	move	$5,$0
	move	$6,$2
	lui	$2,%hi(__ECNT_HOOK)
	addiu	$2,$2,%lo(__ECNT_HOOK)
	jalr	$2
	nop

	sw	$2,76($sp)
	lw	$2,16($sp)
	addiu	$2,$2,1
	sw	$2,16($sp)
$L251:
	lw	$2,16($sp)
	slt	$2,$2,2
	bne	$2,$0,$L255
	nop

	li	$2,1			# 0x1
	sw	$2,80($sp)
	li	$2,2000			# 0x7d0
	sw	$2,84($sp)
	li	$2,60			# 0x3c
	sw	$2,88($sp)
	sw	$0,92($sp)
	sw	$0,96($sp)
	lw	$2,80($sp)
	sw	$2,100($sp)
	lw	$2,84($sp)
	sw	$2,148($sp)
	lw	$2,88($sp)
	sw	$2,152($sp)
	addiu	$2,$sp,96
	li	$4,18			# 0x12
	move	$5,$0
	move	$6,$2
	lui	$2,%hi(__ECNT_HOOK)
	addiu	$2,$2,%lo(__ECNT_HOOK)
	jalr	$2
	nop

	sw	$2,92($sp)
	move	$2,$0
	lw	$31,252($sp)
	addiu	$sp,$sp,256
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	gwan_init
	.size	gwan_init, .-gwan_init
	.rdata
	.align	2
	.type	__func__.38927, @object
	.size	__func__.38927, 24
__func__.38927:
	.ascii	"gwan_prepare_tx_message\000"
	.local	cnt.38942
	.comm	cnt.38942,4,4
	.align	2
	.type	__func__.39032, @object
	.size	__func__.39032, 20
__func__.39032:
	.ascii	"gwan_config_gemport\000"
	.ident	"GCC: (Buildroot 2015.08.1) 4.9.3"
