	.file	1 "epon_wan.c"
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
	.ascii	"[%lu0ms]Transmit frame to invalid LLID %d.\012\000"
	.align	2
$LC1:
	.ascii	"[%lu0ms]ewan_hwnat_hook_tx:---llid = 0x%x\012\000"
#NO_APP
	.section	.text.ewan_prepare_tx_message,"ax",@progbits
	.align	2
	.globl	ewan_prepare_tx_message
	.set	nomips16
	.set	nomicromips
	.ent	ewan_prepare_tx_message
	.type	ewan_prepare_tx_message, @function
ewan_prepare_tx_message:
	.frame	$sp,40,$31		# vars= 16, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-40
	sw	$31,36($sp)
	sw	$4,40($sp)
	move	$2,$5
	sw	$6,48($sp)
	sw	$7,52($sp)
	sb	$2,44($sp)
	li	$2,-1078984704			# 0xffffffffbfb00000
	ori	$2,$2,0x64
	lw	$3,0($2)
	li	$2,-65536			# 0xffffffffffff0000
	and	$3,$3,$2
	li	$2,458752			# 0x70000
	beq	$3,$2,$L2
	nop

	li	$2,-1078984704			# 0xffffffffbfb00000
	ori	$2,$2,0x64
	lw	$3,0($2)
	li	$2,-65536			# 0xffffffffffff0000
	and	$3,$3,$2
	li	$2,524288			# 0x80000
	bne	$3,$2,$L3
	nop

$L2:
	li	$2,255			# 0xff
	sw	$2,16($sp)
	li	$2,7			# 0x7
	sw	$2,20($sp)
	j	$L4
	nop

$L3:
	li	$2,15			# 0xf
	sw	$2,16($sp)
	li	$2,15			# 0xf
	sw	$2,20($sp)
$L4:
	lw	$3,40($sp)
	lw	$2,4($3)
	li	$4,2			# 0x2
	ins	$2,$4,19,3
	sw	$2,4($3)
	lbu	$2,44($sp)
	bne	$2,$0,$L5
	nop

	lw	$2,48($sp)
	lw	$2,256($2)
	sw	$2,28($sp)
	lw	$2,28($sp)
	lhu	$2,0($2)
	andi	$4,$2,0x00ff
	lw	$3,40($sp)
	lw	$2,0($3)
	ins	$2,$4,3,8
	sw	$2,0($3)
	lw	$3,40($sp)
	lw	$2,0($3)
	li	$4,1			# 0x1
	ins	$2,$4,11,1
	sw	$2,0($3)
	lw	$4,48($sp)
	li	$5,2			# 0x2
	lui	$2,%hi(skb_pull)
	addiu	$2,$2,%lo(skb_pull)
	jalr	$2
	nop

	j	$L6
	nop

$L5:
	lw	$2,48($sp)
	lbu	$2,122($2)
	andi	$2,$2,0x2
	beq	$2,$0,$L7
	nop

	lw	$3,40($sp)
	lw	$2,0($3)
	ins	$2,$0,11,1
	sw	$2,0($3)
	lui	$2,%hi(restore_offload_info_hook)
	lw	$2,%lo(restore_offload_info_hook)($2)
	beq	$2,$0,$L8
	nop

	lui	$2,%hi(restore_offload_info_hook)
	lw	$2,%lo(restore_offload_info_hook)($2)
	lw	$4,48($sp)
	lw	$5,56($sp)
	li	$6,29305			# 0x7279
	jalr	$2
	nop

$L8:
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
	lw	$2,56($sp)
	lw	$2,0($2)
	ext	$2,$2,20,8
	andi	$3,$2,0x00ff
	lw	$2,16($sp)
	andi	$2,$2,0x00ff
	and	$2,$3,$2
	andi	$4,$2,0x00ff
	lw	$3,40($sp)
	lw	$2,0($3)
	ins	$2,$4,3,8
	sw	$2,0($3)
	j	$L11
	nop

$L10:
	lw	$2,56($sp)
	lhu	$2,4($2)
	andi	$3,$2,0x00ff
	lw	$2,16($sp)
	andi	$2,$2,0x00ff
	and	$2,$3,$2
	andi	$4,$2,0x00ff
	lw	$3,40($sp)
	lw	$2,0($3)
	ins	$2,$4,3,8
	sw	$2,0($3)
	lw	$2,56($sp)
	lw	$2,0($2)
	ext	$2,$2,28,4
	andi	$2,$2,0x00ff
	move	$3,$2
	lw	$2,20($sp)
	andi	$2,$2,0x00ff
	and	$2,$3,$2
	andi	$2,$2,0x00ff
	andi	$2,$2,0x7
	andi	$4,$2,0x00ff
	lw	$3,40($sp)
	lw	$2,0($3)
	ins	$2,$4,0,3
	sw	$2,0($3)
$L11:
	lw	$2,56($sp)
	lw	$2,0($2)
	ext	$2,$2,12,8
	andi	$2,$2,0x00ff
	andi	$2,$2,0x1
	andi	$4,$2,0x00ff
	lw	$3,40($sp)
	lw	$2,0($3)
	ins	$2,$4,25,1
	sw	$2,0($3)
	lw	$2,56($sp)
	lw	$2,0($2)
	ext	$2,$2,12,8
	andi	$2,$2,0x00ff
	srl	$2,$2,1
	andi	$2,$2,0x00ff
	andi	$2,$2,0x1f
	andi	$4,$2,0x00ff
	lw	$3,40($sp)
	lw	$2,0($3)
	ins	$2,$4,26,5
	sw	$2,0($3)
	lw	$2,48($sp)
	lbu	$3,122($2)
	li	$2,-3			# 0xfffffffffffffffd
	and	$2,$3,$2
	andi	$3,$2,0x00ff
	lw	$2,48($sp)
	sb	$3,122($2)
	move	$2,$0
	j	$L12
	nop

$L7:
	lui	$2,%hi(epon_mapping_hook)
	lw	$2,%lo(epon_mapping_hook)($2)
	beq	$2,$0,$L13
	nop

	lui	$2,%hi(epon_mapping_hook)
	lw	$2,%lo(epon_mapping_hook)($2)
	lw	$4,48($sp)
	jalr	$2
	nop

$L13:
	lw	$2,48($sp)
	lbu	$2,225($2)
	sw	$2,24($sp)
	lw	$2,24($sp)
	sltu	$2,$2,8
	beq	$2,$0,$L14
	nop

	lui	$2,%hi(gpWanPriv)
	lw	$3,%lo(gpWanPriv)($2)
	lw	$2,24($sp)
	sll	$2,$2,5
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	li	$3,65536			# 0x10000
	addu	$2,$3,$2
	lw	$2,-32644($2)
	ext	$2,$2,8,1
	andi	$2,$2,0x00ff
	bne	$2,$0,$L15
	nop

$L14:
	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	lhu	$2,-30816($2)
	andi	$2,$2,0x1
	beq	$2,$0,$L16
	nop

	lui	$2,%hi(jiffies)
	lw	$5,%lo(jiffies)($2)
	lui	$2,%hi(gpWanPriv)
	lw	$3,%lo(gpWanPriv)($2)
	lw	$2,24($sp)
	sll	$2,$2,5
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	li	$3,65536			# 0x10000
	addu	$2,$3,$2
	lhu	$2,-32644($2)
	move	$3,$2
	lui	$2,%hi($LC0)
	addiu	$4,$2,%lo($LC0)
	move	$6,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L16:
	li	$2,-1			# 0xffffffffffffffff
	j	$L12
	nop

$L15:
	lui	$2,%hi(xpon_sfu_multicast_protocol_hook)
	lw	$2,%lo(xpon_sfu_multicast_protocol_hook)($2)
	beq	$2,$0,$L17
	nop

	lui	$2,%hi(xpon_sfu_multicast_protocol_hook)
	lw	$2,%lo(xpon_sfu_multicast_protocol_hook)($2)
	lw	$4,48($sp)
	jalr	$2
	nop

	beq	$2,$0,$L17
	nop

	lui	$2,%hi(xpon_sfu_up_multicast_vlan_hook)
	lw	$2,%lo(xpon_sfu_up_multicast_vlan_hook)($2)
	beq	$2,$0,$L18
	nop

	lui	$2,%hi(xpon_sfu_up_multicast_vlan_hook)
	lw	$2,%lo(xpon_sfu_up_multicast_vlan_hook)($2)
	lw	$4,48($sp)
	move	$5,$0
	jalr	$2
	nop

	bgtz	$2,$L18
	nop

	li	$2,-1			# 0xffffffffffffffff
	j	$L12
	nop

$L18:
	lui	$2,%hi(xpon_sfu_up_multicast_incoming_hook)
	lw	$2,%lo(xpon_sfu_up_multicast_incoming_hook)($2)
	beq	$2,$0,$L17
	nop

	lui	$2,%hi(xpon_sfu_up_multicast_incoming_hook)
	lw	$2,%lo(xpon_sfu_up_multicast_incoming_hook)($2)
	lw	$4,48($sp)
	li	$5,1			# 0x1
	jalr	$2
	nop

	bgtz	$2,$L17
	nop

	li	$2,-1			# 0xffffffffffffffff
	j	$L12
	nop

$L17:
	lui	$2,%hi(gpWanPriv)
	lw	$3,%lo(gpWanPriv)($2)
	lw	$2,24($sp)
	sll	$2,$2,5
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	li	$3,65536			# 0x10000
	addu	$2,$3,$2
	lw	$2,-32644($2)
	ext	$2,$2,9,1
	andi	$2,$2,0x00ff
	beq	$2,$0,$L19
	nop

	lui	$2,%hi(gpWanPriv)
	lw	$3,%lo(gpWanPriv)($2)
	lw	$2,24($sp)
	sll	$2,$2,5
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	li	$4,65536			# 0x10000
	addu	$2,$4,$2
	lw	$2,-32612($2)
	addiu	$4,$2,1
	lw	$2,24($sp)
	sll	$2,$2,5
	sll	$5,$2,2
	subu	$2,$5,$2
	addu	$2,$3,$2
	li	$3,65536			# 0x10000
	addu	$2,$3,$2
	sw	$4,-32612($2)
	li	$2,-1			# 0xffffffffffffffff
	j	$L12
	nop

$L19:
	lw	$3,40($sp)
	lw	$2,0($3)
	ins	$2,$0,11,1
	sw	$2,0($3)
	lw	$2,48($sp)
	lbu	$2,224($2)
	andi	$2,$2,0x20
	sltu	$2,$0,$2
	andi	$4,$2,0x00ff
	lw	$3,40($sp)
	lw	$2,0($3)
	ins	$2,$4,25,1
	sw	$2,0($3)
	lw	$2,40($sp)
	lw	$3,0($2)
	li	$2,33554432			# 0x2000000
	and	$2,$3,$2
	beq	$2,$0,$L20
	nop

	lw	$2,48($sp)
	lbu	$2,224($2)
	andi	$2,$2,0x1f
	andi	$4,$2,0x00ff
	j	$L21
	nop

$L20:
	move	$4,$0
$L21:
	lw	$3,40($sp)
	lw	$2,0($3)
	ins	$2,$4,26,5
	sw	$2,0($3)
	lw	$2,48($sp)
	lbu	$4,225($2)
	lw	$3,40($sp)
	lw	$2,0($3)
	ins	$2,$4,3,8
	sw	$2,0($3)
	lui	$2,%hi(ra_sw_nat_hook_magic)
	lw	$2,%lo(ra_sw_nat_hook_magic)($2)
	beq	$2,$0,$L22
	nop

	lui	$2,%hi(ra_sw_nat_hook_magic)
	lw	$2,%lo(ra_sw_nat_hook_magic)($2)
	lw	$4,48($sp)
	li	$5,29302			# 0x7276
	jalr	$2
	nop

	beq	$2,$0,$L22
	nop

	lw	$3,40($sp)
	lw	$2,4($3)
	li	$4,4			# 0x4
	ins	$2,$4,19,3
	sw	$2,4($3)
$L22:
	lui	$2,%hi(ra_sw_nat_hook_tx)
	lw	$2,%lo(ra_sw_nat_hook_tx)($2)
	beq	$2,$0,$L6
	nop

	lw	$2,56($sp)
	li	$3,29305			# 0x7279
	sh	$3,6($2)
	lw	$2,40($sp)
	lw	$2,0($2)
	ext	$2,$2,3,8
	andi	$3,$2,0x00ff
	lw	$2,16($sp)
	andi	$2,$2,0x00ff
	and	$2,$3,$2
	andi	$4,$2,0x00ff
	lw	$3,56($sp)
	lw	$2,0($3)
	ins	$2,$4,20,8
	sw	$2,0($3)
	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	lhu	$2,-30816($2)
	andi	$2,$2,0x40
	beq	$2,$0,$L6
	nop

	lui	$2,%hi(jiffies)
	lw	$3,%lo(jiffies)($2)
	lw	$2,56($sp)
	lw	$2,0($2)
	ext	$2,$2,20,8
	andi	$2,$2,0x00ff
	move	$6,$2
	lui	$2,%hi($LC1)
	addiu	$4,$2,%lo($LC1)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L6:
	move	$2,$0
$L12:
	lw	$31,36($sp)
	addiu	$sp,$sp,40
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	ewan_prepare_tx_message
	.size	ewan_prepare_tx_message, .-ewan_prepare_tx_message
	.globl	noRxDscpInt
	.section	.bss,"aw",@nobits
	.type	noRxDscpInt, @object
	.size	noRxDscpInt, 1
noRxDscpInt:
	.space	1
	.rdata
	.align	2
$LC2:
	.ascii	"[%lu0ms]isDHCPPacket:received DHCP packet!\012\000"
	.section	.text.isDHCPPacket,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	isDHCPPacket
	.type	isDHCPPacket, @function
isDHCPPacket:
	.frame	$sp,32,$31		# vars= 8, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-32
	sw	$31,28($sp)
	sw	$4,32($sp)
	lw	$2,32($sp)
	bne	$2,$0,$L24
	nop

	move	$2,$0
	j	$L25
	nop

$L24:
	lw	$2,32($sp)
	lhu	$2,22($2)
	sh	$2,16($sp)
	lw	$2,32($sp)
	lhu	$2,20($2)
	sh	$2,18($sp)
	lhu	$3,16($sp)
	li	$2,68			# 0x44
	beq	$3,$2,$L26
	nop

	lhu	$3,18($sp)
	li	$2,68			# 0x44
	beq	$3,$2,$L26
	nop

	lhu	$3,16($sp)
	li	$2,67			# 0x43
	beq	$3,$2,$L26
	nop

	lhu	$3,18($sp)
	li	$2,67			# 0x43
	bne	$3,$2,$L27
	nop

$L26:
	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	lhu	$2,-30816($2)
	andi	$2,$2,0x100
	beq	$2,$0,$L28
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

$L28:
	li	$2,1			# 0x1
	j	$L25
	nop

$L27:
	move	$2,$0
$L25:
	lw	$31,28($sp)
	addiu	$sp,$sp,32
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	isDHCPPacket
	.size	isDHCPPacket, .-isDHCPPacket
	.rdata
	.align	2
$LC3:
	.ascii	"[%lu0ms]isPPPPacket:received pppoe discovery packet!\012"
	.ascii	"\000"
	.align	2
$LC4:
	.ascii	"[%lu0ms]isPPPPacket:received pppoe session packet!\012\000"
	.section	.text.isPPPPacket,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	isPPPPacket
	.type	isPPPPacket, @function
isPPPPacket:
	.frame	$sp,32,$31		# vars= 8, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-32
	sw	$31,28($sp)
	sw	$4,32($sp)
	lw	$2,32($sp)
	bne	$2,$0,$L30
	nop

	move	$2,$0
	j	$L31
	nop

$L30:
	lw	$2,32($sp)
	lhu	$2,0($2)
	sh	$2,16($sp)
	lhu	$3,16($sp)
	li	$2,34915			# 0x8863
	bne	$3,$2,$L32
	nop

	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	lhu	$2,-30816($2)
	andi	$2,$2,0x100
	beq	$2,$0,$L33
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

$L33:
	li	$2,1			# 0x1
	j	$L31
	nop

$L32:
	lhu	$3,16($sp)
	li	$2,34916			# 0x8864
	bne	$3,$2,$L34
	nop

	lw	$2,32($sp)
	lhu	$2,8($2)
	sh	$2,18($sp)
	lhu	$3,18($sp)
	li	$2,49185			# 0xc021
	beq	$3,$2,$L35
	nop

	lhu	$3,18($sp)
	li	$2,49699			# 0xc223
	beq	$3,$2,$L35
	nop

	lhu	$3,18($sp)
	li	$2,49187			# 0xc023
	beq	$3,$2,$L35
	nop

	lhu	$3,18($sp)
	li	$2,32801			# 0x8021
	bne	$3,$2,$L34
	nop

$L35:
	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	lhu	$2,-30816($2)
	andi	$2,$2,0x100
	beq	$2,$0,$L36
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

$L36:
	li	$2,1			# 0x1
	j	$L31
	nop

$L34:
	move	$2,$0
$L31:
	lw	$31,28($sp)
	addiu	$sp,$sp,32
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	isPPPPacket
	.size	isPPPPacket, .-isPPPPacket
	.section	.text.isPPPorDHCP,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	isPPPorDHCP
	.type	isPPPorDHCP, @function
isPPPorDHCP:
	.frame	$sp,32,$31		# vars= 8, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-32
	sw	$31,28($sp)
	sw	$4,32($sp)
	sw	$0,20($sp)
	sw	$0,16($sp)
	lw	$2,32($sp)
	lw	$2,256($2)
	sw	$2,20($sp)
	lw	$2,20($sp)
	beq	$2,$0,$L38
	nop

	lw	$2,20($sp)
	addiu	$2,$2,12
	sw	$2,16($sp)
	lw	$2,16($sp)
	lhu	$3,0($2)
	li	$2,33024			# 0x8100
	bne	$3,$2,$L38
	nop

	lw	$2,16($sp)
	addiu	$2,$2,4
	sw	$2,16($sp)
	lw	$2,16($sp)
	lhu	$3,0($2)
	li	$2,33024			# 0x8100
	bne	$3,$2,$L38
	nop

	lw	$2,16($sp)
	addiu	$2,$2,4
	sw	$2,16($sp)
$L38:
	lw	$4,16($sp)
	lui	$2,%hi(isDHCPPacket)
	addiu	$2,$2,%lo(isDHCPPacket)
	jalr	$2
	nop

	bne	$2,$0,$L39
	nop

	lw	$4,16($sp)
	lui	$2,%hi(isPPPPacket)
	addiu	$2,$2,%lo(isPPPPacket)
	jalr	$2
	nop

	beq	$2,$0,$L40
	nop

$L39:
	li	$2,1			# 0x1
	j	$L41
	nop

$L40:
	move	$2,$0
$L41:
	lw	$31,28($sp)
	addiu	$sp,$sp,32
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	isPPPorDHCP
	.size	isPPPorDHCP, .-isPPPorDHCP
	.rdata
	.align	2
$LC5:
	.ascii	"[%lu0ms]Drop TCP sync flood frame for epon test.\012\000"
	.align	2
$LC6:
	.ascii	"[%lu0ms]Receive frame from invalid LLID %d.\012\000"
	.section	.text.ewan_process_rx_message,"ax",@progbits
	.align	2
	.globl	ewan_process_rx_message
	.set	nomips16
	.set	nomicromips
	.ent	ewan_process_rx_message
	.type	ewan_process_rx_message, @function
ewan_process_rx_message:
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
	lw	$2,40($sp)
	lw	$2,0($2)
	ext	$2,$2,3,8
	andi	$2,$2,0x00ff
	sw	$2,20($sp)
	lw	$2,40($sp)
	lw	$2,0($2)
	andi	$2,$2,0x800
	beq	$2,$0,$L43
	nop

	lw	$4,44($sp)
	li	$5,2			# 0x2
	lui	$2,%hi(skb_push)
	addiu	$2,$2,%lo(skb_push)
	jalr	$2
	nop

	lw	$2,44($sp)
	lw	$2,256($2)
	sw	$2,24($sp)
	lw	$2,20($sp)
	andi	$3,$2,0xffff
	lw	$2,24($sp)
	sh	$3,0($2)
	sw	$0,16($sp)
	j	$L44
	nop

$L43:
	lui	$2,%hi(attackEnhance)
	lbu	$3,%lo(attackEnhance)($2)
	li	$2,1			# 0x1
	bne	$3,$2,$L45
	nop

	lui	$2,%hi(noRxDscpInt)
	lbu	$3,%lo(noRxDscpInt)($2)
	li	$2,1			# 0x1
	bne	$3,$2,$L45
	nop

	lw	$4,44($sp)
	lui	$2,%hi(isPPPorDHCP)
	addiu	$2,$2,%lo(isPPPorDHCP)
	jalr	$2
	nop

	move	$3,$2
	li	$2,1			# 0x1
	beq	$3,$2,$L45
	nop

	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	lhu	$2,-30816($2)
	andi	$2,$2,0x100
	beq	$2,$0,$L46
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

$L46:
	li	$2,-1			# 0xffffffffffffffff
	j	$L47
	nop

$L45:
	lui	$2,%hi(gpWanPriv)
	lw	$3,%lo(gpWanPriv)($2)
	lw	$2,20($sp)
	sll	$2,$2,5
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	li	$3,65536			# 0x10000
	addu	$2,$3,$2
	lw	$2,-32644($2)
	ext	$2,$2,8,1
	andi	$2,$2,0x00ff
	beq	$2,$0,$L48
	nop

	lw	$2,40($sp)
	lw	$2,0($2)
	ext	$2,$2,3,8
	andi	$3,$2,0x00ff
	lw	$2,44($sp)
	sb	$3,225($2)
	lui	$2,%hi(gpWanPriv)
	lw	$3,%lo(gpWanPriv)($2)
	lw	$2,20($sp)
	sll	$2,$2,5
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	li	$3,65536			# 0x10000
	addu	$2,$3,$2
	lw	$2,-32644($2)
	ext	$2,$2,11,1
	andi	$2,$2,0x00ff
	beq	$2,$0,$L49
	nop

	lui	$2,%hi(gpWanPriv)
	lw	$3,%lo(gpWanPriv)($2)
	lw	$2,20($sp)
	sll	$2,$2,5
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	li	$4,65536			# 0x10000
	addu	$2,$4,$2
	lw	$2,-32616($2)
	addiu	$4,$2,1
	lw	$2,20($sp)
	sll	$2,$2,5
	sll	$5,$2,2
	subu	$2,$5,$2
	addu	$2,$3,$2
	li	$3,65536			# 0x10000
	addu	$2,$3,$2
	sw	$4,-32616($2)
	li	$2,-1			# 0xffffffffffffffff
	j	$L47
	nop

$L49:
	lui	$2,%hi(gpWanPriv)
	lw	$3,%lo(gpWanPriv)($2)
	lw	$2,20($sp)
	sll	$2,$2,5
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	li	$3,65536			# 0x10000
	addu	$2,$3,$2
	lw	$2,-32644($2)
	ext	$2,$2,10,1
	andi	$2,$2,0x00ff
	beq	$2,$0,$L50
	nop

	lw	$2,44($sp)
	lw	$3,148($2)
	li	$2,-241			# 0xffffffffffffff0f
	and	$3,$3,$2
	lw	$2,44($sp)
	sw	$3,148($2)
	lw	$2,52($sp)
	li	$3,1			# 0x1
	sb	$3,0($2)
$L50:
	lui	$2,%hi(gpWanPriv)
	lw	$3,%lo(gpWanPriv)($2)
	lw	$2,20($sp)
	sll	$2,$2,5
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	li	$4,65536			# 0x10000
	addu	$2,$4,$2
	lw	$2,-32640($2)
	addiu	$4,$2,1
	lw	$2,20($sp)
	sll	$2,$2,5
	sll	$5,$2,2
	subu	$2,$5,$2
	addu	$2,$3,$2
	li	$3,65536			# 0x10000
	addu	$2,$3,$2
	sw	$4,-32640($2)
	lui	$2,%hi(gpWanPriv)
	lw	$4,%lo(gpWanPriv)($2)
	lui	$2,%hi(gpWanPriv)
	lw	$3,%lo(gpWanPriv)($2)
	lw	$2,20($sp)
	sll	$2,$2,5
	sll	$5,$2,2
	subu	$2,$5,$2
	addu	$2,$3,$2
	li	$3,65536			# 0x10000
	addu	$2,$3,$2
	lw	$3,-32632($2)
	lw	$2,48($sp)
	addu	$3,$3,$2
	lw	$2,20($sp)
	sll	$2,$2,5
	sll	$5,$2,2
	subu	$2,$5,$2
	addu	$2,$4,$2
	li	$4,65536			# 0x10000
	addu	$2,$4,$2
	sw	$3,-32632($2)
	li	$2,2			# 0x2
	sw	$2,16($sp)
	j	$L44
	nop

$L48:
	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	lhu	$2,-30816($2)
	andi	$2,$2,0x1
	beq	$2,$0,$L52
	nop

	lui	$2,%hi(jiffies)
	lw	$5,%lo(jiffies)($2)
	lui	$2,%hi(gpWanPriv)
	lw	$3,%lo(gpWanPriv)($2)
	lw	$2,20($sp)
	sll	$2,$2,5
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	li	$3,65536			# 0x10000
	addu	$2,$3,$2
	lhu	$2,-32644($2)
	move	$3,$2
	lui	$2,%hi($LC6)
	addiu	$4,$2,%lo($LC6)
	move	$6,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L52:
	li	$2,-1			# 0xffffffffffffffff
	j	$L47
	nop

$L44:
	lw	$2,16($sp)
$L47:
	lw	$31,36($sp)
	addiu	$sp,$sp,40
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	ewan_process_rx_message
	.size	ewan_process_rx_message, .-ewan_process_rx_message
	.section	.text.ewan_init,"ax",@progbits
	.align	2
	.globl	ewan_init
	.set	nomips16
	.set	nomicromips
	.ent	ewan_init
	.type	ewan_init, @function
ewan_init:
	.frame	$sp,32,$31		# vars= 8, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-32
	sw	$31,28($sp)
	sw	$4,32($sp)
	sw	$0,16($sp)
	j	$L54
	nop

$L55:
	lw	$2,16($sp)
	move	$3,$2
	sll	$2,$3,5
	move	$3,$2
	sll	$2,$3,2
	subu	$2,$2,$3
	lw	$3,32($sp)
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
	lw	$3,32($sp)
	addu	$2,$3,$2
	addiu	$2,$2,4
	move	$4,$2
	move	$5,$0
	li	$6,92			# 0x5c
	lui	$2,%hi(memset)
	addiu	$2,$2,%lo(memset)
	jalr	$2
	nop

	lw	$4,32($sp)
	lw	$2,16($sp)
	move	$3,$2
	sll	$2,$3,5
	move	$3,$2
	sll	$2,$3,2
	subu	$2,$2,$3
	addu	$3,$4,$2
	lw	$2,0($3)
	li	$4,1			# 0x1
	ins	$2,$4,11,1
	sw	$2,0($3)
	lw	$4,32($sp)
	lw	$2,16($sp)
	move	$3,$2
	sll	$2,$3,5
	move	$3,$2
	sll	$2,$3,2
	subu	$2,$2,$3
	addu	$3,$4,$2
	lw	$2,0($3)
	li	$4,1			# 0x1
	ins	$2,$4,9,1
	sw	$2,0($3)
	lw	$2,16($sp)
	addiu	$2,$2,1
	sw	$2,16($sp)
$L54:
	lw	$2,16($sp)
	slt	$2,$2,8
	bne	$2,$0,$L55
	nop

	move	$4,$0
	lui	$2,%hi(pwan_create_net_interface)
	addiu	$2,$2,%lo(pwan_create_net_interface)
	jalr	$2
	nop

	move	$2,$0
	lw	$31,28($sp)
	addiu	$sp,$sp,32
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	ewan_init
	.size	ewan_init, .-ewan_init
	.ident	"GCC: (Buildroot 2015.08.1) 4.9.3"
