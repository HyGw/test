	.file	1 "gpon_ploam.c"
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
	.ascii	"[%lu0ms]PLOAM: main path EqD in O5, %x, deltaEqd = %x.\012"
	.ascii	"\000"
	.align	2
$LC1:
	.ascii	"[%lu0ms]PLOAM: Setting the main path EqD in O5, ByteDela"
	.ascii	"y:%x, BitDelay:%x. (A:%x, B:%x, byte_dly:%x, a:%x)\012\000"
	.align	2
$LC2:
	.ascii	"ECNT_HOOK_ERROR occur with cmd_id:0x%x\012\000"
	.align	2
$LC3:
	.ascii	"No such API with type:%d, cmd_id:0x%x\012\000"
#NO_APP
	.section	.text.ploam_eqd_adjustment,"ax",@progbits
	.align	2
	.globl	ploam_eqd_adjustment
	.set	nomips16
	.set	nomicromips
	.ent	ploam_eqd_adjustment
	.type	ploam_eqd_adjustment, @function
ploam_eqd_adjustment:
	.frame	$sp,112,$31		# vars= 72, regs= 1/0, args= 32, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-112
	sw	$31,108($sp)
	sw	$4,112($sp)
	sw	$0,48($sp)
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lw	$2,268($2)
	lw	$3,112($sp)
	subu	$2,$3,$2
	sw	$2,32($sp)
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lbu	$2,278($2)
	bne	$2,$0,$L2
	nop

	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lw	$3,112($sp)
	sw	$3,268($2)
	j	$L3
	nop

$L2:
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lw	$3,448($2)
	li	$2,268435456			# 0x10000000
	and	$2,$3,$2
	beq	$2,$0,$L4
	nop

	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lbu	$2,278($2)
	move	$3,$2
	lw	$2,32($sp)
	subu	$2,$2,$3
	sw	$2,32($sp)
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lui	$3,%hi(gpGponPriv)
	lw	$3,%lo(gpGponPriv)($3)
	lbu	$3,278($3)
	move	$4,$3
	lw	$3,112($sp)
	subu	$3,$3,$4
	sw	$3,268($2)
	j	$L3
	nop

$L4:
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lbu	$2,278($2)
	move	$3,$2
	lw	$2,32($sp)
	addu	$2,$2,$3
	sw	$2,32($sp)
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lui	$3,%hi(gpGponPriv)
	lw	$3,%lo(gpGponPriv)($3)
	lbu	$3,278($3)
	move	$4,$3
	lw	$3,112($sp)
	addu	$3,$4,$3
	sw	$3,268($2)
$L3:
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,17244
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	sw	$2,76($sp)
	lw	$2,76($sp)
	ext	$2,$2,0,2
	andi	$2,$2,0x00ff
	sw	$2,52($sp)
	lw	$2,32($sp)
	blez	$2,$L5
	nop

	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lbu	$2,276($2)
	move	$3,$2
	lw	$2,32($sp)
	addu	$2,$3,$2
	sw	$2,56($sp)
	lw	$2,56($sp)
	sra	$2,$2,3
	sw	$2,36($sp)
	lw	$2,56($sp)
	andi	$2,$2,0x7
	sw	$2,40($sp)
	lw	$2,36($sp)
	andi	$3,$2,0x3
	lw	$2,52($sp)
	addu	$2,$3,$2
	slt	$2,$2,4
	beq	$2,$0,$L6
	nop

	move	$2,$0
	j	$L7
	nop

$L6:
	li	$2,8			# 0x8
$L7:
	lw	$4,36($sp)
	li	$3,-4			# 0xfffffffffffffffc
	and	$3,$4,$3
	addu	$3,$2,$3
	lw	$2,36($sp)
	andi	$2,$2,0x3
	subu	$2,$3,$2
	sw	$2,44($sp)
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lui	$3,%hi(gpGponPriv)
	lw	$3,%lo(gpGponPriv)($3)
	lw	$3,272($3)
	lw	$4,44($sp)
	sll	$4,$4,3
	addu	$3,$3,$4
	sw	$3,272($2)
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lw	$3,40($sp)
	andi	$3,$3,0x00ff
	sb	$3,276($2)
	j	$L8
	nop

$L5:
	lw	$2,32($sp)
	bgez	$2,$L1
	nop

	lw	$2,32($sp)
	subu	$2,$0,$2
	lui	$3,%hi(gpGponPriv)
	lw	$3,%lo(gpGponPriv)($3)
	lbu	$3,276($3)
	subu	$2,$2,$3
	sw	$2,56($sp)
	lw	$2,56($sp)
	blez	$2,$L10
	nop

	lw	$2,56($sp)
	sra	$2,$2,3
	lw	$3,56($sp)
	andi	$3,$3,0x7
	sltu	$3,$0,$3
	andi	$3,$3,0x00ff
	addu	$2,$2,$3
	j	$L11
	nop

$L10:
	move	$2,$0
$L11:
	sw	$2,36($sp)
	lw	$2,36($sp)
	sll	$3,$2,3
	lw	$2,56($sp)
	subu	$2,$3,$2
	sw	$2,40($sp)
	lw	$2,36($sp)
	andi	$3,$2,0x3
	lw	$2,52($sp)
	slt	$2,$2,$3
	bne	$2,$0,$L12
	nop

	move	$2,$0
	j	$L13
	nop

$L12:
	li	$2,8			# 0x8
$L13:
	lw	$4,36($sp)
	li	$3,-4			# 0xfffffffffffffffc
	and	$3,$4,$3
	addu	$3,$2,$3
	lw	$2,36($sp)
	andi	$2,$2,0x3
	subu	$2,$3,$2
	sw	$2,44($sp)
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lui	$3,%hi(gpGponPriv)
	lw	$3,%lo(gpGponPriv)($3)
	lw	$3,272($3)
	lw	$4,44($sp)
	sll	$4,$4,3
	subu	$3,$3,$4
	sw	$3,272($2)
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lw	$3,40($sp)
	andi	$3,$3,0x00ff
	sb	$3,276($2)
$L8:
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lbu	$2,278($2)
	beq	$2,$0,$L15
	nop

	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	lhu	$2,-30816($2)
	andi	$2,$2,0x10
	beq	$2,$0,$L15
	nop

	lui	$2,%hi(jiffies)
	lw	$5,%lo(jiffies)($2)
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lw	$3,268($2)
	lui	$2,%hi($LC0)
	addiu	$4,$2,%lo($LC0)
	move	$6,$3
	lw	$7,32($sp)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L15:
	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	lhu	$2,-30816($2)
	andi	$2,$2,0x10
	beq	$2,$0,$L16
	nop

	lui	$2,%hi(jiffies)
	lw	$5,%lo(jiffies)($2)
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lw	$3,272($2)
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lbu	$2,276($2)
	move	$7,$2
	lw	$2,36($sp)
	sw	$2,16($sp)
	lw	$2,40($sp)
	sw	$2,20($sp)
	lw	$2,52($sp)
	sw	$2,24($sp)
	lw	$2,44($sp)
	sw	$2,28($sp)
	lui	$2,%hi($LC1)
	addiu	$4,$2,%lo($LC1)
	move	$6,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L16:
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16552
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	sw	$2,72($sp)
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lw	$2,272($2)
	sw	$2,72($sp)
	lw	$3,72($sp)
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16552
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lbu	$2,276($2)
	sw	$2,80($sp)
	li	$2,10			# 0xa
	sw	$2,60($sp)
	addiu	$2,$sp,80
	sw	$2,64($sp)
	sw	$0,84($sp)
	sw	$0,88($sp)
	sw	$0,92($sp)
	sw	$0,96($sp)
	li	$2,1			# 0x1
	sw	$2,84($sp)
	lw	$2,60($sp)
	sw	$2,88($sp)
	lw	$2,64($sp)
	sw	$2,96($sp)
	addiu	$2,$sp,84
	sw	$2,68($sp)
	li	$4,25			# 0x19
	move	$5,$0
	lw	$6,68($sp)
	lui	$2,%hi(__ECNT_HOOK)
	addiu	$2,$2,%lo(__ECNT_HOOK)
	jalr	$2
	nop

	move	$3,$2
	li	$2,-1			# 0xffffffffffffffff
	bne	$3,$2,$L17
	nop

	lw	$2,68($sp)
	lw	$3,4($2)
	lui	$2,%hi($LC2)
	addiu	$4,$2,%lo($LC2)
	move	$5,$3
	lui	$2,%hi(panic)
	addiu	$2,$2,%lo(panic)
	jalr	$2
	nop

$L17:
	lw	$2,68($sp)
	lw	$3,8($2)
	li	$2,-1			# 0xffffffffffffffff
	bne	$3,$2,$L1
	nop

	lw	$2,68($sp)
	lw	$5,0($2)
	lw	$2,68($sp)
	lw	$3,4($2)
	lui	$2,%hi($LC3)
	addiu	$4,$2,%lo($LC3)
	move	$6,$3
	lui	$2,%hi(panic)
	addiu	$2,$2,%lo(panic)
	jalr	$2
	nop

$L1:
	lw	$31,108($sp)
	addiu	$sp,$sp,112
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	ploam_eqd_adjustment
	.size	ploam_eqd_adjustment, .-ploam_eqd_adjustment
	.rdata
	.align	2
$LC4:
	.ascii	"[%lu0ms]PLOAM: receive unknow message.(%.8X %.8X %.8X)\012"
	.ascii	"\000"
	.section	.text.ploam_parser_down_message,"ax",@progbits
	.align	2
	.globl	ploam_parser_down_message
	.set	nomips16
	.set	nomicromips
	.ent	ploam_parser_down_message
	.type	ploam_parser_down_message, @function
ploam_parser_down_message:
	.frame	$sp,48,$31		# vars= 16, regs= 1/0, args= 24, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-48
	sw	$31,44($sp)
	sw	$4,48($sp)
	li	$2,-14			# 0xfffffffffffffff2
	sw	$2,24($sp)
	lw	$2,48($sp)
	lbu	$2,1($2)
	sw	$2,28($sp)
	lw	$2,28($sp)
	sltu	$2,$2,25
	beq	$2,$0,$L23
	nop

	lui	$2,%hi(gpGponPriv)
	lw	$3,%lo(gpGponPriv)($2)
	lw	$2,28($sp)
	addiu	$2,$2,22
	sll	$2,$2,2
	addu	$2,$3,$2
	lw	$2,4($2)
	bne	$2,$0,$L24
	nop

$L23:
	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	lhu	$2,-30816($2)
	andi	$2,$2,0x11
	beq	$2,$0,$L25
	nop

	lui	$2,%hi(jiffies)
	lw	$5,%lo(jiffies)($2)
	lw	$2,48($sp)
	lw	$6,0($2)
	lw	$2,48($sp)
	lw	$3,4($2)
	lw	$2,48($sp)
	lw	$2,8($2)
	sw	$2,16($sp)
	lui	$2,%hi($LC4)
	addiu	$4,$2,%lo($LC4)
	move	$7,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L25:
	li	$2,-14			# 0xfffffffffffffff2
	j	$L26
	nop

$L24:
	lui	$2,%hi(gpGponPriv)
	lw	$3,%lo(gpGponPriv)($2)
	lw	$2,28($sp)
	addiu	$2,$2,22
	sll	$2,$2,2
	addu	$2,$3,$2
	lw	$2,4($2)
	sw	$2,32($sp)
	lw	$2,32($sp)
	lw	$4,48($sp)
	jalr	$2
	nop

	sw	$2,24($sp)
	lw	$2,24($sp)
$L26:
	lw	$31,44($sp)
	addiu	$sp,$sp,48
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	ploam_parser_down_message
	.size	ploam_parser_down_message, .-ploam_parser_down_message
	.rdata
	.align	2
$LC5:
	.ascii	"[%lu0ms]PLOAM: Send password message.(%.8X %.8X %.8X)\012"
	.ascii	"\000"
	.section	.text.ploam_send_passwd_msg,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	ploam_send_passwd_msg
	.type	ploam_send_passwd_msg, @function
ploam_send_passwd_msg:
	.frame	$sp,48,$31		# vars= 16, regs= 1/0, args= 24, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-48
	sw	$31,44($sp)
	sw	$4,48($sp)
	move	$2,$5
	sb	$2,52($sp)
	addiu	$2,$sp,24
	move	$4,$2
	move	$5,$0
	li	$6,12			# 0xc
	lui	$2,%hi(memset)
	addiu	$2,$2,%lo(memset)
	jalr	$2
	nop

	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lbu	$2,204($2)
	sb	$2,24($sp)
	li	$2,2			# 0x2
	sb	$2,25($sp)
	lbu	$2,52($sp)
	addiu	$3,$sp,26
	move	$4,$3
	lw	$5,48($sp)
	move	$6,$2
	lui	$2,%hi(memcpy)
	addiu	$2,$2,%lo(memcpy)
	jalr	$2
	nop

	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	lhu	$2,-30816($2)
	andi	$2,$2,0x10
	beq	$2,$0,$L28
	nop

	lui	$2,%hi(jiffies)
	lw	$5,%lo(jiffies)($2)
	lw	$6,24($sp)
	lw	$3,28($sp)
	lw	$2,32($sp)
	sw	$2,16($sp)
	lui	$2,%hi($LC5)
	addiu	$4,$2,%lo($LC5)
	move	$7,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L28:
	addiu	$2,$sp,24
	move	$4,$2
	li	$5,3			# 0x3
	lui	$2,%hi(gponDevSendPloamMsg)
	addiu	$2,$2,%lo(gponDevSendPloamMsg)
	jalr	$2
	nop

	move	$2,$0
	lw	$31,44($sp)
	addiu	$sp,$sp,48
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	ploam_send_passwd_msg
	.size	ploam_send_passwd_msg, .-ploam_send_passwd_msg
	.rdata
	.align	2
$LC6:
	.ascii	"[%lu0ms]PLOAM: Send dying gasp message.(%.8X %.8X %.8X)\012"
	.ascii	"\000"
	.section	.text.ploam_send_dying_gasp,"ax",@progbits
	.align	2
	.globl	ploam_send_dying_gasp
	.set	nomips16
	.set	nomicromips
	.ent	ploam_send_dying_gasp
	.type	ploam_send_dying_gasp, @function
ploam_send_dying_gasp:
	.frame	$sp,48,$31		# vars= 16, regs= 1/0, args= 24, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-48
	sw	$31,44($sp)
	addiu	$2,$sp,24
	move	$4,$2
	move	$5,$0
	li	$6,12			# 0xc
	lui	$2,%hi(memset)
	addiu	$2,$2,%lo(memset)
	jalr	$2
	nop

	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lbu	$2,204($2)
	sb	$2,24($sp)
	li	$2,3			# 0x3
	sb	$2,25($sp)
	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	lhu	$2,-30816($2)
	andi	$2,$2,0x10
	beq	$2,$0,$L31
	nop

	lui	$2,%hi(jiffies)
	lw	$5,%lo(jiffies)($2)
	lw	$6,24($sp)
	lw	$3,28($sp)
	lw	$2,32($sp)
	sw	$2,16($sp)
	lui	$2,%hi($LC6)
	addiu	$4,$2,%lo($LC6)
	move	$7,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L31:
	addiu	$2,$sp,24
	move	$4,$2
	li	$5,3			# 0x3
	lui	$2,%hi(gponDevSendPloamMsg)
	addiu	$2,$2,%lo(gponDevSendPloamMsg)
	jalr	$2
	nop

	move	$2,$0
	lw	$31,44($sp)
	addiu	$sp,$sp,48
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	ploam_send_dying_gasp
	.size	ploam_send_dying_gasp, .-ploam_send_dying_gasp
	.rdata
	.align	2
$LC7:
	.ascii	"[%lu0ms]PLOAM: Send encryption key message.(%.8X %.8X %."
	.ascii	"8X)\012\000"
	.section	.text.ploam_send_encryption_key,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	ploam_send_encryption_key
	.type	ploam_send_encryption_key, @function
ploam_send_encryption_key:
	.frame	$sp,48,$31		# vars= 16, regs= 1/0, args= 24, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-48
	sw	$31,44($sp)
	move	$2,$4
	sw	$5,52($sp)
	sb	$2,48($sp)
	addiu	$2,$sp,28
	move	$4,$2
	move	$5,$0
	li	$6,12			# 0xc
	lui	$2,%hi(memset)
	addiu	$2,$2,%lo(memset)
	jalr	$2
	nop

	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lbu	$2,204($2)
	sb	$2,28($sp)
	li	$2,5			# 0x5
	sb	$2,29($sp)
	sw	$0,24($sp)
	j	$L34
	nop

$L36:
	lbu	$2,48($sp)
	sb	$2,30($sp)
	lw	$2,24($sp)
	andi	$2,$2,0x00ff
	sb	$2,31($sp)
	lw	$2,24($sp)
	sll	$2,$2,3
	move	$3,$2
	lw	$2,52($sp)
	addu	$3,$2,$3
	addiu	$2,$sp,28
	addiu	$2,$2,4
	move	$4,$2
	move	$5,$3
	li	$6,8			# 0x8
	lui	$2,%hi(memcpy)
	addiu	$2,$2,%lo(memcpy)
	jalr	$2
	nop

	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	lhu	$2,-30816($2)
	andi	$2,$2,0x10
	beq	$2,$0,$L35
	nop

	lui	$2,%hi(jiffies)
	lw	$5,%lo(jiffies)($2)
	lw	$6,28($sp)
	lw	$3,32($sp)
	lw	$2,36($sp)
	sw	$2,16($sp)
	lui	$2,%hi($LC7)
	addiu	$4,$2,%lo($LC7)
	move	$7,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L35:
	addiu	$2,$sp,28
	move	$4,$2
	li	$5,3			# 0x3
	lui	$2,%hi(gponDevSendPloamMsg)
	addiu	$2,$2,%lo(gponDevSendPloamMsg)
	jalr	$2
	nop

	lw	$2,24($sp)
	addiu	$2,$2,1
	sw	$2,24($sp)
$L34:
	lw	$2,24($sp)
	slt	$2,$2,2
	bne	$2,$0,$L36
	nop

	move	$2,$0
	lw	$31,44($sp)
	addiu	$sp,$sp,48
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	ploam_send_encryption_key
	.size	ploam_send_encryption_key, .-ploam_send_encryption_key
	.rdata
	.align	2
$LC8:
	.ascii	"[%lu0ms]PLOAM: Send PEE message.(%.8X %.8X %.8X)\012\000"
	.section	.text.ploam_send_pee_msg,"ax",@progbits
	.align	2
	.globl	ploam_send_pee_msg
	.set	nomips16
	.set	nomicromips
	.ent	ploam_send_pee_msg
	.type	ploam_send_pee_msg, @function
ploam_send_pee_msg:
	.frame	$sp,48,$31		# vars= 16, regs= 1/0, args= 24, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-48
	sw	$31,44($sp)
	addiu	$2,$sp,24
	move	$4,$2
	move	$5,$0
	li	$6,12			# 0xc
	lui	$2,%hi(memset)
	addiu	$2,$2,%lo(memset)
	jalr	$2
	nop

	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lbu	$2,204($2)
	sb	$2,24($sp)
	li	$2,6			# 0x6
	sb	$2,25($sp)
	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	lhu	$2,-30816($2)
	andi	$2,$2,0x10
	beq	$2,$0,$L39
	nop

	lui	$2,%hi(jiffies)
	lw	$5,%lo(jiffies)($2)
	lw	$6,24($sp)
	lw	$3,28($sp)
	lw	$2,32($sp)
	sw	$2,16($sp)
	lui	$2,%hi($LC8)
	addiu	$4,$2,%lo($LC8)
	move	$7,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L39:
	addiu	$2,$sp,24
	move	$4,$2
	li	$5,1			# 0x1
	lui	$2,%hi(gponDevSendPloamMsg)
	addiu	$2,$2,%lo(gponDevSendPloamMsg)
	jalr	$2
	nop

	move	$2,$0
	lw	$31,44($sp)
	addiu	$sp,$sp,48
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	ploam_send_pee_msg
	.size	ploam_send_pee_msg, .-ploam_send_pee_msg
	.rdata
	.align	2
$LC9:
	.ascii	"[%lu0ms]PLOAM: Send PST message.(%.8X %.8X %.8X)\012\000"
	.section	.text.ploam_send_pst_msg,"ax",@progbits
	.align	2
	.globl	ploam_send_pst_msg
	.set	nomips16
	.set	nomicromips
	.ent	ploam_send_pst_msg
	.type	ploam_send_pst_msg, @function
ploam_send_pst_msg:
	.frame	$sp,48,$31		# vars= 16, regs= 1/0, args= 24, gp= 0
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
	addiu	$2,$sp,24
	move	$4,$2
	move	$5,$0
	li	$6,12			# 0xc
	lui	$2,%hi(memset)
	addiu	$2,$2,%lo(memset)
	jalr	$2
	nop

	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lbu	$2,204($2)
	sb	$2,24($sp)
	li	$2,7			# 0x7
	sb	$2,25($sp)
	lbu	$2,48($sp)
	sb	$2,26($sp)
	lbu	$2,52($sp)
	sb	$2,27($sp)
	lbu	$2,56($sp)
	sb	$2,28($sp)
	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	lhu	$2,-30816($2)
	andi	$2,$2,0x10
	beq	$2,$0,$L42
	nop

	lui	$2,%hi(jiffies)
	lw	$5,%lo(jiffies)($2)
	lw	$6,24($sp)
	lw	$3,28($sp)
	lw	$2,32($sp)
	sw	$2,16($sp)
	lui	$2,%hi($LC9)
	addiu	$4,$2,%lo($LC9)
	move	$7,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L42:
	addiu	$2,$sp,24
	move	$4,$2
	li	$5,1			# 0x1
	lui	$2,%hi(gponDevSendPloamMsg)
	addiu	$2,$2,%lo(gponDevSendPloamMsg)
	jalr	$2
	nop

	move	$2,$0
	lw	$31,44($sp)
	addiu	$sp,$sp,48
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	ploam_send_pst_msg
	.size	ploam_send_pst_msg, .-ploam_send_pst_msg
	.rdata
	.align	2
$LC10:
	.ascii	"[%lu0ms]PLOAM: Send REI message.(%.8X %.8X %.8X)\012\000"
	.section	.text.ploam_send_rei_msg,"ax",@progbits
	.align	2
	.globl	ploam_send_rei_msg
	.set	nomips16
	.set	nomicromips
	.ent	ploam_send_rei_msg
	.type	ploam_send_rei_msg, @function
ploam_send_rei_msg:
	.frame	$sp,48,$31		# vars= 16, regs= 1/0, args= 24, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-48
	sw	$31,44($sp)
	sw	$4,48($sp)
	sw	$5,52($sp)
	addiu	$2,$sp,24
	move	$4,$2
	move	$5,$0
	li	$6,12			# 0xc
	lui	$2,%hi(memset)
	addiu	$2,$2,%lo(memset)
	jalr	$2
	nop

	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lbu	$2,204($2)
	sb	$2,24($sp)
	li	$2,8			# 0x8
	sb	$2,25($sp)
	lw	$2,48($sp)
	srl	$2,$2,24
	andi	$2,$2,0x00ff
	sb	$2,26($sp)
	lw	$2,48($sp)
	srl	$2,$2,16
	andi	$2,$2,0x00ff
	sb	$2,27($sp)
	lw	$2,48($sp)
	srl	$2,$2,8
	andi	$2,$2,0x00ff
	sb	$2,28($sp)
	lw	$2,48($sp)
	andi	$2,$2,0x00ff
	sb	$2,29($sp)
	lw	$2,52($sp)
	lbu	$2,0($2)
	andi	$2,$2,0xf
	andi	$3,$2,0x00ff
	lbu	$2,30($sp)
	ins	$2,$3,0,4
	sb	$2,30($sp)
	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	lhu	$2,-30816($2)
	andi	$2,$2,0x10
	beq	$2,$0,$L45
	nop

	lui	$2,%hi(jiffies)
	lw	$5,%lo(jiffies)($2)
	lw	$6,24($sp)
	lw	$3,28($sp)
	lw	$2,32($sp)
	sw	$2,16($sp)
	lui	$2,%hi($LC10)
	addiu	$4,$2,%lo($LC10)
	move	$7,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L45:
	addiu	$2,$sp,24
	move	$4,$2
	li	$5,1			# 0x1
	lui	$2,%hi(gponDevSendPloamMsg)
	addiu	$2,$2,%lo(gponDevSendPloamMsg)
	jalr	$2
	nop

	lw	$2,52($sp)
	lbu	$2,0($2)
	addiu	$2,$2,1
	andi	$2,$2,0x00ff
	andi	$2,$2,0xf
	andi	$3,$2,0x00ff
	lw	$2,52($sp)
	sb	$3,0($2)
	move	$2,$0
	lw	$31,44($sp)
	addiu	$sp,$sp,48
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	ploam_send_rei_msg
	.size	ploam_send_rei_msg, .-ploam_send_rei_msg
	.rdata
	.align	2
$LC11:
	.ascii	"[%lu0ms]PLOAM: Send acknowledge PLOAM message.\012\000"
	.align	2
$LC12:
	.ascii	"[%lu0ms]PLOAM: Send acknowledge message.(%.8X %.8X %.8X)"
	.ascii	"\012\000"
	.section	.text.ploam_send_acknowledge_msg,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	ploam_send_acknowledge_msg
	.type	ploam_send_acknowledge_msg, @function
ploam_send_acknowledge_msg:
	.frame	$sp,48,$31		# vars= 16, regs= 1/0, args= 24, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-48
	sw	$31,44($sp)
	move	$2,$4
	sw	$5,52($sp)
	sb	$2,48($sp)
	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	lhu	$2,-30816($2)
	andi	$2,$2,0x10
	beq	$2,$0,$L48
	nop

	lui	$2,%hi(jiffies)
	lw	$3,%lo(jiffies)($2)
	lui	$2,%hi($LC11)
	addiu	$4,$2,%lo($LC11)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L48:
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lbu	$2,204($2)
	sb	$2,24($sp)
	li	$2,9			# 0x9
	sb	$2,25($sp)
	lbu	$2,48($sp)
	sb	$2,26($sp)
	addiu	$2,$sp,27
	move	$4,$2
	lw	$5,52($sp)
	li	$6,9			# 0x9
	lui	$2,%hi(memcpy)
	addiu	$2,$2,%lo(memcpy)
	jalr	$2
	nop

	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	lhu	$2,-30816($2)
	andi	$2,$2,0x10
	beq	$2,$0,$L49
	nop

	lui	$2,%hi(jiffies)
	lw	$5,%lo(jiffies)($2)
	lw	$6,24($sp)
	lw	$3,28($sp)
	lw	$2,32($sp)
	sw	$2,16($sp)
	lui	$2,%hi($LC12)
	addiu	$4,$2,%lo($LC12)
	move	$7,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L49:
	addiu	$2,$sp,24
	move	$4,$2
	li	$5,3			# 0x3
	lui	$2,%hi(gponDevSendPloamMsg)
	addiu	$2,$2,%lo(gponDevSendPloamMsg)
	jalr	$2
	nop

	move	$2,$0
	lw	$31,44($sp)
	addiu	$sp,$sp,48
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	ploam_send_acknowledge_msg
	.size	ploam_send_acknowledge_msg, .-ploam_send_acknowledge_msg
	.rdata
	.align	2
$LC13:
	.ascii	"[%lu0ms]PLOAM: Send Sleep_Request message.(%.8X %.8X %.8"
	.ascii	"X)\012\000"
	.section	.text.ploam_send_sleep_request_msg,"ax",@progbits
	.align	2
	.globl	ploam_send_sleep_request_msg
	.set	nomips16
	.set	nomicromips
	.ent	ploam_send_sleep_request_msg
	.type	ploam_send_sleep_request_msg, @function
ploam_send_sleep_request_msg:
	.frame	$sp,48,$31		# vars= 16, regs= 1/0, args= 24, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-48
	sw	$31,44($sp)
	sw	$4,48($sp)
	addiu	$2,$sp,24
	move	$4,$2
	move	$5,$0
	li	$6,12			# 0xc
	lui	$2,%hi(memset)
	addiu	$2,$2,%lo(memset)
	jalr	$2
	nop

	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lbu	$2,204($2)
	sb	$2,24($sp)
	li	$2,10			# 0xa
	sb	$2,25($sp)
	lw	$2,48($sp)
	andi	$2,$2,0x00ff
	andi	$2,$2,0x3
	andi	$3,$2,0x00ff
	lbu	$2,26($sp)
	ins	$2,$3,0,2
	sb	$2,26($sp)
	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	lhu	$2,-30816($2)
	andi	$2,$2,0x10
	beq	$2,$0,$L52
	nop

	lui	$2,%hi(jiffies)
	lw	$5,%lo(jiffies)($2)
	lw	$6,24($sp)
	lw	$3,28($sp)
	lw	$2,32($sp)
	sw	$2,16($sp)
	lui	$2,%hi($LC13)
	addiu	$4,$2,%lo($LC13)
	move	$7,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L52:
	addiu	$2,$sp,24
	move	$4,$2
	li	$5,1			# 0x1
	lui	$2,%hi(gponDevSendPloamMsg)
	addiu	$2,$2,%lo(gponDevSendPloamMsg)
	jalr	$2
	nop

	move	$2,$0
	lw	$31,44($sp)
	addiu	$sp,$sp,48
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	ploam_send_sleep_request_msg
	.size	ploam_send_sleep_request_msg, .-ploam_send_sleep_request_msg
	.rdata
	.align	2
$LC14:
	.ascii	"[%lu0ms]PLOAM: Rx Upstream_Overhead.(%.8X %.8X %.8X),sta"
	.ascii	"te O%d\012\000"
	.align	2
$LC15:
	.ascii	"[%lu0ms]The dest. address(%.2x) of PLOAM message is inco"
	.ascii	"rrect.\012\000"
	.section	.text.ploam_recv_upstream_overhead,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	ploam_recv_upstream_overhead
	.type	ploam_recv_upstream_overhead, @function
ploam_recv_upstream_overhead:
	.frame	$sp,168,$31		# vars= 136, regs= 2/0, args= 24, gp= 0
	.mask	0x80010000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-168
	sw	$31,164($sp)
	sw	$16,160($sp)
	sw	$4,168($sp)
	sw	$0,80($sp)
	sw	$0,84($sp)
	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	lhu	$2,-30816($2)
	andi	$2,$2,0x10
	beq	$2,$0,$L55
	nop

	lui	$2,%hi(jiffies)
	lw	$5,%lo(jiffies)($2)
	lw	$2,168($sp)
	lw	$6,0($2)
	lw	$2,168($sp)
	lw	$7,4($2)
	lw	$2,168($sp)
	lw	$3,8($2)
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lw	$2,4($2)
	sw	$3,16($sp)
	sw	$2,20($sp)
	lui	$2,%hi($LC14)
	addiu	$4,$2,%lo($LC14)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L55:
	lw	$2,168($sp)
	lbu	$3,0($2)
	li	$2,255			# 0xff
	beq	$3,$2,$L56
	nop

	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	lhu	$2,-30816($2)
	andi	$2,$2,0x11
	beq	$2,$0,$L57
	nop

	lui	$2,%hi(jiffies)
	lw	$3,%lo(jiffies)($2)
	lw	$2,168($sp)
	lbu	$2,0($2)
	move	$6,$2
	lui	$2,%hi($LC15)
	addiu	$4,$2,%lo($LC15)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L57:
	li	$2,-1			# 0xffffffffffffffff
	j	$L74
	nop

$L56:
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lw	$3,4($2)
	li	$2,2			# 0x2
	bne	$3,$2,$L59
	nop

	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lw	$3,464($2)
	li	$2,1073741824			# 0x40000000
	and	$2,$3,$2
	bne	$2,$0,$L60
	nop

	li	$2,1			# 0x1
	sw	$2,136($sp)
	li	$2,12			# 0xc
	sw	$2,32($sp)
	addiu	$2,$sp,136
	sw	$2,36($sp)
	sw	$0,140($sp)
	sw	$0,144($sp)
	sw	$0,148($sp)
	sw	$0,152($sp)
	li	$2,1			# 0x1
	sw	$2,140($sp)
	lw	$2,32($sp)
	sw	$2,144($sp)
	lw	$2,36($sp)
	sw	$2,152($sp)
	addiu	$2,$sp,140
	sw	$2,40($sp)
	li	$4,25			# 0x19
	move	$5,$0
	lw	$6,40($sp)
	lui	$2,%hi(__ECNT_HOOK)
	addiu	$2,$2,%lo(__ECNT_HOOK)
	jalr	$2
	nop

	move	$3,$2
	li	$2,-1			# 0xffffffffffffffff
	bne	$3,$2,$L61
	nop

	lw	$2,40($sp)
	lw	$3,4($2)
	lui	$2,%hi($LC2)
	addiu	$4,$2,%lo($LC2)
	move	$5,$3
	lui	$2,%hi(panic)
	addiu	$2,$2,%lo(panic)
	jalr	$2
	nop

$L61:
	lw	$2,40($sp)
	lw	$3,8($2)
	li	$2,-1			# 0xffffffffffffffff
	bne	$3,$2,$L60
	nop

	lw	$2,40($sp)
	lw	$5,0($2)
	lw	$2,40($sp)
	lw	$3,4($2)
	lui	$2,%hi($LC3)
	addiu	$4,$2,%lo($LC3)
	move	$6,$3
	lui	$2,%hi(panic)
	addiu	$2,$2,%lo(panic)
	jalr	$2
	nop

$L60:
	addiu	$2,$sp,88
	sw	$2,24($sp)
	lw	$2,24($sp)
	li	$3,15			# 0xf
	sw	$3,12($2)
	lw	$2,24($sp)
	li	$3,24			# 0x18
	sb	$3,0($2)
	lw	$2,168($sp)
	lbu	$3,4($2)
	lw	$2,24($sp)
	sb	$3,1($2)
	lw	$2,168($sp)
	lbu	$3,3($2)
	lw	$2,24($sp)
	sb	$3,2($2)
	lw	$2,168($sp)
	lbu	$3,5($2)
	lw	$2,24($sp)
	sb	$3,3($2)
	li	$2,17			# 0x11
	sw	$2,44($sp)
	lw	$2,24($sp)
	sw	$2,48($sp)
	sw	$0,120($sp)
	sw	$0,124($sp)
	sw	$0,128($sp)
	sw	$0,132($sp)
	li	$2,1			# 0x1
	sw	$2,120($sp)
	lw	$2,44($sp)
	sw	$2,124($sp)
	lw	$2,48($sp)
	sw	$2,132($sp)
	addiu	$2,$sp,120
	sw	$2,52($sp)
	li	$4,25			# 0x19
	move	$5,$0
	lw	$6,52($sp)
	lui	$2,%hi(__ECNT_HOOK)
	addiu	$2,$2,%lo(__ECNT_HOOK)
	jalr	$2
	nop

	move	$3,$2
	li	$2,-1			# 0xffffffffffffffff
	bne	$3,$2,$L66
	nop

	lw	$2,52($sp)
	lw	$3,4($2)
	lui	$2,%hi($LC2)
	addiu	$4,$2,%lo($LC2)
	move	$5,$3
	lui	$2,%hi(panic)
	addiu	$2,$2,%lo(panic)
	jalr	$2
	nop

$L66:
	lw	$2,52($sp)
	lw	$3,8($2)
	li	$2,-1			# 0xffffffffffffffff
	bne	$3,$2,$L67
	nop

	lw	$2,52($sp)
	lw	$5,0($2)
	lw	$2,52($sp)
	lw	$3,4($2)
	lui	$2,%hi($LC3)
	addiu	$4,$2,%lo($LC3)
	move	$6,$3
	lui	$2,%hi(panic)
	addiu	$2,$2,%lo(panic)
	jalr	$2
	nop

$L67:
	lw	$2,168($sp)
	lbu	$2,6($2)
	sll	$3,$2,16
	lw	$2,168($sp)
	lbu	$2,7($2)
	sll	$2,$2,8
	or	$2,$3,$2
	lw	$3,168($sp)
	lbu	$3,8($3)
	or	$2,$2,$3
	sw	$2,28($sp)
	lw	$2,28($sp)
	sw	$2,80($sp)
	li	$2,-86			# 0xffffffffffffffaa
	sb	$2,84($sp)
	li	$2,14			# 0xe
	sw	$2,56($sp)
	addiu	$2,$sp,80
	sw	$2,60($sp)
	sw	$0,104($sp)
	sw	$0,108($sp)
	sw	$0,112($sp)
	sw	$0,116($sp)
	li	$2,1			# 0x1
	sw	$2,104($sp)
	lw	$2,56($sp)
	sw	$2,108($sp)
	lw	$2,60($sp)
	sw	$2,116($sp)
	addiu	$2,$sp,104
	sw	$2,64($sp)
	li	$4,25			# 0x19
	move	$5,$0
	lw	$6,64($sp)
	lui	$2,%hi(__ECNT_HOOK)
	addiu	$2,$2,%lo(__ECNT_HOOK)
	jalr	$2
	nop

	move	$3,$2
	li	$2,-1			# 0xffffffffffffffff
	bne	$3,$2,$L70
	nop

	lw	$2,64($sp)
	lw	$3,4($2)
	lui	$2,%hi($LC2)
	addiu	$4,$2,%lo($LC2)
	move	$5,$3
	lui	$2,%hi(panic)
	addiu	$2,$2,%lo(panic)
	jalr	$2
	nop

$L70:
	lw	$2,64($sp)
	lw	$3,8($2)
	li	$2,-1			# 0xffffffffffffffff
	bne	$3,$2,$L71
	nop

	lw	$2,64($sp)
	lw	$5,0($2)
	lw	$2,64($sp)
	lw	$3,4($2)
	lui	$2,%hi($LC3)
	addiu	$4,$2,%lo($LC3)
	move	$6,$3
	lui	$2,%hi(panic)
	addiu	$2,$2,%lo(panic)
	jalr	$2
	nop

$L71:
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16532
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	sw	$2,68($sp)
	li	$2,24			# 0x18
	sb	$2,71($sp)
	lw	$3,68($sp)
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16532
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lw	$3,68($sp)
	sw	$3,428($2)
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16536
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	sw	$2,72($sp)
	lw	$2,168($sp)
	lbu	$2,3($2)
	sb	$2,75($sp)
	lw	$2,168($sp)
	lbu	$2,4($2)
	sb	$2,74($sp)
	lw	$3,72($sp)
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16536
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lw	$3,72($sp)
	sw	$3,436($2)
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16548
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	sw	$2,76($sp)
	lw	$2,168($sp)
	lw	$2,8($2)
	ext	$2,$2,21,1
	andi	$3,$2,0x00ff
	lw	$2,76($sp)
	ins	$2,$3,31,1
	sw	$2,76($sp)
	lw	$2,168($sp)
	lbu	$2,10($2)
	sll	$2,$2,8
	seh	$3,$2
	lw	$2,168($sp)
	lbu	$2,11($2)
	seh	$2,$2
	or	$2,$3,$2
	seh	$2,$2
	andi	$2,$2,0xffff
	sh	$2,78($sp)
	lw	$3,76($sp)
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16548
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lw	$3,76($sp)
	sw	$3,444($2)
	lui	$2,%hi(gpGponPriv)
	lw	$16,%lo(gpGponPriv)($2)
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16544
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	sw	$2,432($16)
	li	$4,3			# 0x3
	lui	$2,%hi(gpon_act_change_gpon_state)
	addiu	$2,$2,%lo(gpon_act_change_gpon_state)
	jalr	$2
	nop

$L59:
	move	$2,$0
$L74:
	lw	$31,164($sp)
	lw	$16,160($sp)
	addiu	$sp,$sp,168
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	ploam_recv_upstream_overhead
	.size	ploam_recv_upstream_overhead, .-ploam_recv_upstream_overhead
	.rdata
	.align	2
$LC16:
	.ascii	"[%lu0ms]PLOAM: Receive Assign_ONU_ID message.(%.8X %.8X "
	.ascii	"%.8X)\012\000"
	.align	2
$LC17:
	.ascii	"[%lu0ms]The serial number of PLOAM message is incorrect\012"
	.ascii	"\000"
	.section	.text.ploam_recv_assign_onu_id,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	ploam_recv_assign_onu_id
	.type	ploam_recv_assign_onu_id, @function
ploam_recv_assign_onu_id:
	.frame	$sp,40,$31		# vars= 8, regs= 1/0, args= 24, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-40
	sw	$31,36($sp)
	sw	$4,40($sp)
	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	lhu	$2,-30816($2)
	andi	$2,$2,0x10
	beq	$2,$0,$L76
	nop

	lui	$2,%hi(jiffies)
	lw	$5,%lo(jiffies)($2)
	lw	$2,40($sp)
	lw	$6,0($2)
	lw	$2,40($sp)
	lw	$3,4($2)
	lw	$2,40($sp)
	lw	$2,8($2)
	sw	$2,16($sp)
	lui	$2,%hi($LC16)
	addiu	$4,$2,%lo($LC16)
	move	$7,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L76:
	lw	$2,40($sp)
	lbu	$3,0($2)
	li	$2,255			# 0xff
	beq	$3,$2,$L77
	nop

	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	lhu	$2,-30816($2)
	andi	$2,$2,0x11
	beq	$2,$0,$L78
	nop

	lui	$2,%hi(jiffies)
	lw	$3,%lo(jiffies)($2)
	lw	$2,40($sp)
	lbu	$2,0($2)
	move	$6,$2
	lui	$2,%hi($LC15)
	addiu	$4,$2,%lo($LC15)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L78:
	li	$2,-1			# 0xffffffffffffffff
	j	$L83
	nop

$L77:
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	addiu	$3,$2,205
	lw	$2,40($sp)
	addiu	$2,$2,3
	move	$4,$3
	move	$5,$2
	li	$6,8			# 0x8
	lui	$2,%hi(memcmp)
	addiu	$2,$2,%lo(memcmp)
	jalr	$2
	nop

	beq	$2,$0,$L80
	nop

	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	lhu	$2,-30816($2)
	andi	$2,$2,0x11
	beq	$2,$0,$L81
	nop

	lui	$2,%hi(jiffies)
	lw	$3,%lo(jiffies)($2)
	lui	$2,%hi($LC17)
	addiu	$4,$2,%lo($LC17)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L81:
	li	$2,-1			# 0xffffffffffffffff
	j	$L83
	nop

$L80:
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lw	$3,4($2)
	li	$2,3			# 0x3
	bne	$3,$2,$L82
	nop

	li	$4,4			# 0x4
	lui	$2,%hi(gpon_act_change_gpon_state)
	addiu	$2,$2,%lo(gpon_act_change_gpon_state)
	jalr	$2
	nop

	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16384
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	sw	$2,24($sp)
	lw	$2,24($sp)
	li	$3,1			# 0x1
	ins	$2,$3,15,1
	sw	$2,24($sp)
	lw	$2,40($sp)
	lbu	$2,2($2)
	sb	$2,27($sp)
	lw	$3,24($sp)
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16384
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lw	$3,40($sp)
	lbu	$3,2($3)
	sb	$3,204($2)
	lui	$2,%hi(gpWanPriv)
	lw	$2,%lo(gpWanPriv)($2)
	lui	$3,%hi(gpGponPriv)
	lw	$3,%lo(gpGponPriv)($3)
	lbu	$3,204($3)
	andi	$3,$3,0xffff
	sh	$3,28($2)
	lui	$2,%hi(gpWanPriv)
	lw	$2,%lo(gpWanPriv)($2)
	li	$3,1			# 0x1
	sw	$3,20($2)
$L82:
	move	$2,$0
$L83:
	lw	$31,36($sp)
	addiu	$sp,$sp,40
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	ploam_recv_assign_onu_id
	.size	ploam_recv_assign_onu_id, .-ploam_recv_assign_onu_id
	.rdata
	.align	2
$LC18:
	.ascii	"[%lu0ms]PLOAM: Receive Ranging_Time message.(%.8X %.8X %"
	.ascii	".8X)\012\000"
	.align	2
$LC19:
	.ascii	"[%lu0ms]PLOAM: Receive the main path EqD, %x.\012\000"
	.align	2
$LC20:
	.ascii	"[%lu0ms]PLOAM: Adjust the main path EqD in O4, Offset:%x"
	.ascii	".\012\000"
	.align	2
$LC21:
	.ascii	"[%lu0ms]PLOAM: Setting the main path EqD in O4, ByteDela"
	.ascii	"y:%x, BitDelay:%x.\012\000"
	.align	2
$LC22:
	.ascii	"[%lu0ms]PLOAM: Setting the protection path EqD.\012\000"
	.section	.text.ploam_recv_ranging_time,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	ploam_recv_ranging_time
	.type	ploam_recv_ranging_time, @function
ploam_recv_ranging_time:
	.frame	$sp,88,$31		# vars= 56, regs= 1/0, args= 24, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-88
	sw	$31,84($sp)
	sw	$4,88($sp)
	sw	$0,28($sp)
	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	lhu	$2,-30816($2)
	andi	$2,$2,0x10
	beq	$2,$0,$L85
	nop

	lui	$2,%hi(jiffies)
	lw	$5,%lo(jiffies)($2)
	lw	$2,88($sp)
	lw	$6,0($2)
	lw	$2,88($sp)
	lw	$3,4($2)
	lw	$2,88($sp)
	lw	$2,8($2)
	sw	$2,16($sp)
	lui	$2,%hi($LC18)
	addiu	$4,$2,%lo($LC18)
	move	$7,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L85:
	lw	$2,88($sp)
	lbu	$3,0($2)
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lbu	$2,204($2)
	beq	$3,$2,$L86
	nop

	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	lhu	$2,-30816($2)
	andi	$2,$2,0x11
	beq	$2,$0,$L87
	nop

	lui	$2,%hi(jiffies)
	lw	$3,%lo(jiffies)($2)
	lw	$2,88($sp)
	lbu	$2,0($2)
	move	$6,$2
	lui	$2,%hi($LC15)
	addiu	$4,$2,%lo($LC15)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L87:
	li	$2,-1			# 0xffffffffffffffff
	j	$L106
	nop

$L86:
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lw	$3,4($2)
	li	$2,4			# 0x4
	beq	$3,$2,$L89
	nop

	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lw	$3,4($2)
	li	$2,5			# 0x5
	bne	$3,$2,$L90
	nop

$L89:
	lw	$2,88($sp)
	lw	$2,0($2)
	andi	$2,$2,0x100
	bne	$2,$0,$L91
	nop

	lw	$2,88($sp)
	lbu	$2,3($2)
	sll	$3,$2,24
	lw	$2,88($sp)
	lbu	$2,4($2)
	sll	$2,$2,16
	or	$3,$3,$2
	lw	$2,88($sp)
	lbu	$2,5($2)
	sll	$2,$2,8
	or	$2,$3,$2
	lw	$3,88($sp)
	lbu	$3,6($3)
	or	$2,$2,$3
	sw	$2,24($sp)
	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	lhu	$2,-30816($2)
	andi	$2,$2,0x210
	beq	$2,$0,$L92
	nop

	lui	$2,%hi(jiffies)
	lw	$3,%lo(jiffies)($2)
	lui	$2,%hi($LC19)
	addiu	$4,$2,%lo($LC19)
	move	$5,$3
	lw	$6,24($sp)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L92:
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lw	$3,4($2)
	li	$2,4			# 0x4
	bne	$3,$2,$L93
	nop

	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lw	$3,24($sp)
	sw	$3,268($2)
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lbu	$2,277($2)
	beq	$2,$0,$L94
	nop

	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lw	$3,448($2)
	li	$2,268435456			# 0x10000000
	and	$2,$3,$2
	beq	$2,$0,$L95
	nop

	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lbu	$2,277($2)
	move	$3,$2
	lw	$2,24($sp)
	subu	$2,$2,$3
	sw	$2,24($sp)
	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	lhu	$2,-30816($2)
	andi	$2,$2,0x10
	beq	$2,$0,$L94
	nop

	lui	$2,%hi(jiffies)
	lw	$3,%lo(jiffies)($2)
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lbu	$2,277($2)
	move	$6,$2
	lui	$2,%hi($LC20)
	addiu	$4,$2,%lo($LC20)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	j	$L94
	nop

$L95:
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lbu	$2,277($2)
	move	$3,$2
	lw	$2,24($sp)
	addu	$2,$2,$3
	sw	$2,24($sp)
	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	lhu	$2,-30816($2)
	andi	$2,$2,0x10
	beq	$2,$0,$L94
	nop

	lui	$2,%hi(jiffies)
	lw	$3,%lo(jiffies)($2)
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lbu	$2,277($2)
	move	$6,$2
	lui	$2,%hi($LC20)
	addiu	$4,$2,%lo($LC20)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L94:
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lw	$4,24($sp)
	li	$3,-8			# 0xfffffffffffffff8
	and	$3,$4,$3
	sw	$3,272($2)
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lw	$3,24($sp)
	andi	$3,$3,0x00ff
	andi	$3,$3,0x7
	andi	$3,$3,0x00ff
	sb	$3,276($2)
	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	lhu	$2,-30816($2)
	andi	$2,$2,0x10
	beq	$2,$0,$L97
	nop

	lui	$2,%hi(jiffies)
	lw	$5,%lo(jiffies)($2)
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lw	$3,272($2)
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lbu	$2,276($2)
	move	$7,$2
	lui	$2,%hi($LC21)
	addiu	$4,$2,%lo($LC21)
	move	$6,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L97:
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16552
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	sw	$2,44($sp)
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lw	$2,272($2)
	sw	$2,44($sp)
	lw	$3,44($sp)
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16552
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lbu	$2,276($2)
	sw	$2,60($sp)
	li	$2,10			# 0xa
	sw	$2,32($sp)
	addiu	$2,$sp,60
	sw	$2,36($sp)
	sw	$0,64($sp)
	sw	$0,68($sp)
	sw	$0,72($sp)
	sw	$0,76($sp)
	li	$2,1			# 0x1
	sw	$2,64($sp)
	lw	$2,32($sp)
	sw	$2,68($sp)
	lw	$2,36($sp)
	sw	$2,76($sp)
	addiu	$2,$sp,64
	sw	$2,40($sp)
	li	$4,25			# 0x19
	move	$5,$0
	lw	$6,40($sp)
	lui	$2,%hi(__ECNT_HOOK)
	addiu	$2,$2,%lo(__ECNT_HOOK)
	jalr	$2
	nop

	move	$3,$2
	li	$2,-1			# 0xffffffffffffffff
	bne	$3,$2,$L98
	nop

	lw	$2,40($sp)
	lw	$3,4($2)
	lui	$2,%hi($LC2)
	addiu	$4,$2,%lo($LC2)
	move	$5,$3
	lui	$2,%hi(panic)
	addiu	$2,$2,%lo(panic)
	jalr	$2
	nop

$L98:
	lw	$2,40($sp)
	lw	$3,8($2)
	li	$2,-1			# 0xffffffffffffffff
	bne	$3,$2,$L90
	nop

	lw	$2,40($sp)
	lw	$5,0($2)
	lw	$2,40($sp)
	lw	$3,4($2)
	lui	$2,%hi($LC3)
	addiu	$4,$2,%lo($LC3)
	move	$6,$3
	lui	$2,%hi(panic)
	addiu	$2,$2,%lo(panic)
	jalr	$2
	nop

$L93:
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lw	$3,4($2)
	li	$2,5			# 0x5
	bne	$3,$2,$L90
	nop

	lw	$4,24($sp)
	lui	$2,%hi(ploam_eqd_adjustment)
	addiu	$2,$2,%lo(ploam_eqd_adjustment)
	jalr	$2
	nop

	j	$L90
	nop

$L91:
	lw	$2,88($sp)
	lw	$2,0($2)
	andi	$2,$2,0x100
	beq	$2,$0,$L90
	nop

	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	lhu	$2,-30816($2)
	andi	$2,$2,0x10
	beq	$2,$0,$L90
	nop

	lui	$2,%hi(jiffies)
	lw	$3,%lo(jiffies)($2)
	lui	$2,%hi($LC22)
	addiu	$4,$2,%lo($LC22)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L90:
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lw	$3,4($2)
	li	$2,4			# 0x4
	bne	$3,$2,$L104
	nop

	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16388
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	sw	$2,48($sp)
	lw	$2,48($sp)
	li	$3,1			# 0x1
	ins	$2,$3,16,1
	sw	$2,48($sp)
	lw	$3,48($sp)
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16388
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lw	$3,464($2)
	li	$2,-2147483648			# 0xffffffff80000000
	and	$2,$3,$2
	bne	$2,$0,$L105
	nop

	move	$4,$0
	lui	$2,%hi(gponDevMbiStop)
	addiu	$2,$2,%lo(gponDevMbiStop)
	jalr	$2
	nop

	lui	$2,%hi(enable_cpu_us_traffic)
	addiu	$2,$2,%lo(enable_cpu_us_traffic)
	jalr	$2
	nop

	lui	$2,%hi(start_omci_oam_monitor)
	addiu	$2,$2,%lo(start_omci_oam_monitor)
	jalr	$2
	nop

$L105:
	li	$4,5			# 0x5
	lui	$2,%hi(gpon_act_change_gpon_state)
	addiu	$2,$2,%lo(gpon_act_change_gpon_state)
	jalr	$2
	nop

	lui	$2,%hi(gpGponPriv)
	lw	$3,%lo(gpGponPriv)($2)
	lbu	$2,464($3)
	ins	$2,$0,7,1
	sb	$2,464($3)
	li	$4,1			# 0x1
	li	$5,3			# 0x3
	move	$6,$0
	lui	$2,%hi(xmcs_report_event)
	addiu	$2,$2,%lo(xmcs_report_event)
	jalr	$2
	nop

$L104:
	move	$2,$0
$L106:
	lw	$31,84($sp)
	addiu	$sp,$sp,88
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	ploam_recv_ranging_time
	.size	ploam_recv_ranging_time, .-ploam_recv_ranging_time
	.rdata
	.align	2
$LC23:
	.ascii	"[%lu0ms]PLOAM: Receive Deactivate_OnuID message.(%.8X %."
	.ascii	"8X %.8X), SPF:0x%08x\012\000"
	.align	2
$LC24:
	.ascii	"[%lu0ms]one bit err cnt:0x%08x\012\000"
	.align	2
$LC25:
	.ascii	"[%lu0ms]two bits err cnt:0x%08x\012\000"
	.align	2
$LC26:
	.ascii	"[%lu0ms]uncorrectable err cnt:0x%08x\012\000"
	.section	.text.ploam_recv_deactivate_onu,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	ploam_recv_deactivate_onu
	.type	ploam_recv_deactivate_onu, @function
ploam_recv_deactivate_onu:
	.frame	$sp,40,$31		# vars= 8, regs= 2/0, args= 24, gp= 0
	.mask	0x80010000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-40
	sw	$31,36($sp)
	sw	$16,32($sp)
	sw	$4,40($sp)
	addiu	$2,$sp,24
	move	$4,$2
	lui	$2,%hi(gponDevGetSuperframe)
	addiu	$2,$2,%lo(gponDevGetSuperframe)
	jalr	$2
	nop

	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	lhu	$2,-30816($2)
	andi	$2,$2,0x10
	beq	$2,$0,$L108
	nop

	lui	$2,%hi(jiffies)
	lw	$5,%lo(jiffies)($2)
	lw	$2,40($sp)
	lw	$6,0($2)
	lw	$2,40($sp)
	lw	$7,4($2)
	lw	$2,40($sp)
	lw	$3,8($2)
	lw	$2,24($sp)
	sw	$3,16($sp)
	sw	$2,20($sp)
	lui	$2,%hi($LC23)
	addiu	$4,$2,%lo($LC23)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L108:
	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	lhu	$2,-30816($2)
	andi	$2,$2,0x1
	beq	$2,$0,$L109
	nop

	lui	$2,%hi(jiffies)
	lw	$16,%lo(jiffies)($2)
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,17200
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi($LC24)
	addiu	$4,$2,%lo($LC24)
	move	$5,$16
	move	$6,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L109:
	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	lhu	$2,-30816($2)
	andi	$2,$2,0x1
	beq	$2,$0,$L110
	nop

	lui	$2,%hi(jiffies)
	lw	$16,%lo(jiffies)($2)
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,17204
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi($LC25)
	addiu	$4,$2,%lo($LC25)
	move	$5,$16
	move	$6,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L110:
	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	lhu	$2,-30816($2)
	andi	$2,$2,0x1
	beq	$2,$0,$L111
	nop

	lui	$2,%hi(jiffies)
	lw	$16,%lo(jiffies)($2)
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,17208
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi($LC26)
	addiu	$4,$2,%lo($LC26)
	move	$5,$16
	move	$6,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L111:
	lw	$2,40($sp)
	lbu	$3,0($2)
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lbu	$2,204($2)
	beq	$3,$2,$L112
	nop

	lw	$2,40($sp)
	lbu	$3,0($2)
	li	$2,255			# 0xff
	beq	$3,$2,$L112
	nop

	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	lhu	$2,-30816($2)
	andi	$2,$2,0x11
	beq	$2,$0,$L113
	nop

	lui	$2,%hi(jiffies)
	lw	$3,%lo(jiffies)($2)
	lw	$2,40($sp)
	lbu	$2,0($2)
	move	$6,$2
	lui	$2,%hi($LC15)
	addiu	$4,$2,%lo($LC15)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L113:
	li	$2,-1			# 0xffffffffffffffff
	j	$L118
	nop

$L112:
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lw	$3,4($2)
	li	$2,4			# 0x4
	beq	$3,$2,$L115
	nop

	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lw	$3,4($2)
	li	$2,5			# 0x5
	beq	$3,$2,$L115
	nop

	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lw	$3,4($2)
	li	$2,6			# 0x6
	bne	$3,$2,$L116
	nop

$L115:
	li	$4,1			# 0x1
	lui	$2,%hi(xpon_set_qdma_qos)
	addiu	$2,$2,%lo(xpon_set_qdma_qos)
	jalr	$2
	nop

	li	$4,1			# 0x1
	li	$5,2			# 0x2
	move	$6,$0
	lui	$2,%hi(xmcs_report_event)
	addiu	$2,$2,%lo(xmcs_report_event)
	jalr	$2
	nop

	lui	$2,%hi(stop_omci_oam_monitor)
	addiu	$2,$2,%lo(stop_omci_oam_monitor)
	jalr	$2
	nop

	move	$4,$0
	lui	$2,%hi(gpon_disable_with_option)
	addiu	$2,$2,%lo(gpon_disable_with_option)
	jalr	$2
	nop

$L116:
	move	$2,$0
$L118:
	lw	$31,36($sp)
	lw	$16,32($sp)
	addiu	$sp,$sp,40
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	ploam_recv_deactivate_onu
	.size	ploam_recv_deactivate_onu, .-ploam_recv_deactivate_onu
	.rdata
	.align	2
$LC27:
	.ascii	"[%lu0ms]PLOAM: Receive Disable_SN message.(%.8X %.8X %.8"
	.ascii	"X)\012\000"
	.section	.text.ploam_recv_disable_serial_number,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	ploam_recv_disable_serial_number
	.type	ploam_recv_disable_serial_number, @function
ploam_recv_disable_serial_number:
	.frame	$sp,96,$31		# vars= 64, regs= 1/0, args= 24, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-96
	sw	$31,92($sp)
	sw	$4,96($sp)
	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	lhu	$2,-30816($2)
	andi	$2,$2,0x10
	beq	$2,$0,$L120
	nop

	lui	$2,%hi(jiffies)
	lw	$5,%lo(jiffies)($2)
	lw	$2,96($sp)
	lw	$6,0($2)
	lw	$2,96($sp)
	lw	$3,4($2)
	lw	$2,96($sp)
	lw	$2,8($2)
	sw	$2,16($sp)
	lui	$2,%hi($LC27)
	addiu	$4,$2,%lo($LC27)
	move	$7,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L120:
	lw	$2,96($sp)
	lbu	$3,0($2)
	li	$2,255			# 0xff
	beq	$3,$2,$L121
	nop

	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	lhu	$2,-30816($2)
	andi	$2,$2,0x11
	beq	$2,$0,$L122
	nop

	lui	$2,%hi(jiffies)
	lw	$3,%lo(jiffies)($2)
	lw	$2,96($sp)
	lbu	$2,0($2)
	move	$6,$2
	lui	$2,%hi($LC15)
	addiu	$4,$2,%lo($LC15)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L122:
	li	$2,-1			# 0xffffffffffffffff
	j	$L123
	nop

$L121:
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lw	$3,4($2)
	li	$2,7			# 0x7
	bne	$3,$2,$L124
	nop

	lw	$2,96($sp)
	lbu	$3,2($2)
	li	$2,15			# 0xf
	beq	$3,$2,$L125
	nop

	lw	$2,96($sp)
	lbu	$2,2($2)
	bne	$2,$0,$L132
	nop

	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	addiu	$3,$2,205
	lw	$2,96($sp)
	addiu	$2,$2,3
	move	$4,$3
	move	$5,$2
	li	$6,8			# 0x8
	lui	$2,%hi(memcmp)
	addiu	$2,$2,%lo(memcmp)
	jalr	$2
	nop

	bne	$2,$0,$L132
	nop

$L125:
	lui	$2,%hi(gpGponPriv)
	lw	$3,%lo(gpGponPriv)($2)
	lbu	$2,464($3)
	ins	$2,$0,5,1
	sb	$2,464($3)
	li	$4,2			# 0x2
	lui	$2,%hi(gpon_act_change_gpon_state)
	addiu	$2,$2,%lo(gpon_act_change_gpon_state)
	jalr	$2
	nop

	li	$2,1			# 0x1
	sw	$2,68($sp)
	li	$2,12			# 0xc
	sw	$2,24($sp)
	addiu	$2,$sp,68
	sw	$2,28($sp)
	sw	$0,72($sp)
	sw	$0,76($sp)
	sw	$0,80($sp)
	sw	$0,84($sp)
	li	$2,1			# 0x1
	sw	$2,72($sp)
	lw	$2,24($sp)
	sw	$2,76($sp)
	lw	$2,28($sp)
	sw	$2,84($sp)
	addiu	$2,$sp,72
	sw	$2,32($sp)
	li	$4,25			# 0x19
	move	$5,$0
	lw	$6,32($sp)
	lui	$2,%hi(__ECNT_HOOK)
	addiu	$2,$2,%lo(__ECNT_HOOK)
	jalr	$2
	nop

	move	$3,$2
	li	$2,-1			# 0xffffffffffffffff
	bne	$3,$2,$L127
	nop

	lw	$2,32($sp)
	lw	$3,4($2)
	lui	$2,%hi($LC2)
	addiu	$4,$2,%lo($LC2)
	move	$5,$3
	lui	$2,%hi(panic)
	addiu	$2,$2,%lo(panic)
	jalr	$2
	nop

$L127:
	lw	$2,32($sp)
	lw	$3,8($2)
	li	$2,-1			# 0xffffffffffffffff
	bne	$3,$2,$L128
	nop

	lw	$2,32($sp)
	lw	$5,0($2)
	lw	$2,32($sp)
	lw	$3,4($2)
	lui	$2,%hi($LC3)
	addiu	$4,$2,%lo($LC3)
	move	$6,$3
	lui	$2,%hi(panic)
	addiu	$2,$2,%lo(panic)
	jalr	$2
	nop

$L128:
	li	$4,1			# 0x1
	li	$5,9			# 0x9
	move	$6,$0
	lui	$2,%hi(xmcs_report_event)
	addiu	$2,$2,%lo(xmcs_report_event)
	jalr	$2
	nop

	j	$L132
	nop

$L124:
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lw	$3,4($2)
	li	$2,1			# 0x1
	beq	$3,$2,$L132
	nop

	lw	$2,96($sp)
	lbu	$3,2($2)
	li	$2,240			# 0xf0
	beq	$3,$2,$L133
	nop

	lw	$2,96($sp)
	lbu	$3,2($2)
	li	$2,255			# 0xff
	bne	$3,$2,$L132
	nop

	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	addiu	$3,$2,205
	lw	$2,96($sp)
	addiu	$2,$2,3
	move	$4,$3
	move	$5,$2
	li	$6,8			# 0x8
	lui	$2,%hi(memcmp)
	addiu	$2,$2,%lo(memcmp)
	jalr	$2
	nop

	bne	$2,$0,$L132
	nop

$L133:
	li	$4,7			# 0x7
	lui	$2,%hi(gpon_act_change_gpon_state)
	addiu	$2,$2,%lo(gpon_act_change_gpon_state)
	jalr	$2
	nop

	sw	$0,48($sp)
	li	$2,12			# 0xc
	sw	$2,36($sp)
	addiu	$2,$sp,48
	sw	$2,40($sp)
	sw	$0,52($sp)
	sw	$0,56($sp)
	sw	$0,60($sp)
	sw	$0,64($sp)
	li	$2,1			# 0x1
	sw	$2,52($sp)
	lw	$2,36($sp)
	sw	$2,56($sp)
	lw	$2,40($sp)
	sw	$2,64($sp)
	addiu	$2,$sp,52
	sw	$2,44($sp)
	li	$4,25			# 0x19
	move	$5,$0
	lw	$6,44($sp)
	lui	$2,%hi(__ECNT_HOOK)
	addiu	$2,$2,%lo(__ECNT_HOOK)
	jalr	$2
	nop

	move	$3,$2
	li	$2,-1			# 0xffffffffffffffff
	bne	$3,$2,$L134
	nop

	lw	$2,44($sp)
	lw	$3,4($2)
	lui	$2,%hi($LC2)
	addiu	$4,$2,%lo($LC2)
	move	$5,$3
	lui	$2,%hi(panic)
	addiu	$2,$2,%lo(panic)
	jalr	$2
	nop

$L134:
	lw	$2,44($sp)
	lw	$3,8($2)
	li	$2,-1			# 0xffffffffffffffff
	bne	$3,$2,$L135
	nop

	lw	$2,44($sp)
	lw	$5,0($2)
	lw	$2,44($sp)
	lw	$3,4($2)
	lui	$2,%hi($LC3)
	addiu	$4,$2,%lo($LC3)
	move	$6,$3
	lui	$2,%hi(panic)
	addiu	$2,$2,%lo(panic)
	jalr	$2
	nop

$L135:
	li	$4,1			# 0x1
	li	$5,8			# 0x8
	move	$6,$0
	lui	$2,%hi(xmcs_report_event)
	addiu	$2,$2,%lo(xmcs_report_event)
	jalr	$2
	nop

	lui	$2,%hi(gpGponPriv)
	lw	$3,%lo(gpGponPriv)($2)
	lbu	$2,464($3)
	li	$4,1			# 0x1
	ins	$2,$4,5,1
	sb	$2,464($3)
	move	$4,$0
	lui	$2,%hi(gpon_record_emergence_info)
	addiu	$2,$2,%lo(gpon_record_emergence_info)
	jalr	$2
	nop

	li	$4,1			# 0x1
	lui	$2,%hi(gpon_record_emergence_info)
	addiu	$2,$2,%lo(gpon_record_emergence_info)
	jalr	$2
	nop

$L132:
	move	$2,$0
$L123:
	lw	$31,92($sp)
	addiu	$sp,$sp,96
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	ploam_recv_disable_serial_number
	.size	ploam_recv_disable_serial_number, .-ploam_recv_disable_serial_number
	.rdata
	.align	2
$LC28:
	.ascii	"[%lu0ms]PLOAM: Receive Encrypted_PortID message.(%.8X %."
	.ascii	"8X %.8X)\012\000"
	.section	.text.ploam_recv_encrypt_port_id,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	ploam_recv_encrypt_port_id
	.type	ploam_recv_encrypt_port_id, @function
ploam_recv_encrypt_port_id:
	.frame	$sp,40,$31		# vars= 8, regs= 1/0, args= 24, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-40
	sw	$31,36($sp)
	sw	$4,40($sp)
	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	lhu	$2,-30816($2)
	andi	$2,$2,0x10
	beq	$2,$0,$L140
	nop

	lui	$2,%hi(jiffies)
	lw	$5,%lo(jiffies)($2)
	lw	$2,40($sp)
	lw	$6,0($2)
	lw	$2,40($sp)
	lw	$3,4($2)
	lw	$2,40($sp)
	lw	$2,8($2)
	sw	$2,16($sp)
	lui	$2,%hi($LC28)
	addiu	$4,$2,%lo($LC28)
	move	$7,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L140:
	lw	$2,40($sp)
	lbu	$3,0($2)
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lbu	$2,204($2)
	beq	$3,$2,$L141
	nop

	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	lhu	$2,-30816($2)
	andi	$2,$2,0x11
	beq	$2,$0,$L142
	nop

	lui	$2,%hi(jiffies)
	lw	$3,%lo(jiffies)($2)
	lw	$2,40($sp)
	lbu	$2,0($2)
	move	$6,$2
	lui	$2,%hi($LC15)
	addiu	$4,$2,%lo($LC15)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L142:
	li	$2,-1			# 0xffffffffffffffff
	j	$L143
	nop

$L141:
	lw	$2,40($sp)
	lbu	$2,3($2)
	sll	$2,$2,4
	seh	$3,$2
	lw	$2,40($sp)
	lw	$2,4($2)
	ext	$2,$2,28,4
	andi	$2,$2,0x00ff
	seh	$2,$2
	or	$2,$3,$2
	seh	$2,$2
	sh	$2,24($sp)
	lui	$2,%hi(gpWanPriv)
	lw	$4,%lo(gpWanPriv)($2)
	lhu	$2,24($sp)
	lui	$3,%hi(gpWanPriv)
	lw	$5,%lo(gpWanPriv)($3)
	lhu	$3,24($sp)
	addiu	$3,$3,44
	sll	$3,$3,1
	addu	$3,$5,$3
	lhu	$3,4($3)
	andi	$3,$3,0x7fff
	andi	$3,$3,0xffff
	addiu	$2,$2,44
	sll	$2,$2,1
	addu	$2,$4,$2
	sh	$3,4($2)
	lui	$2,%hi(gpWanPriv)
	lw	$4,%lo(gpWanPriv)($2)
	lhu	$5,24($sp)
	lui	$2,%hi(gpWanPriv)
	lw	$3,%lo(gpWanPriv)($2)
	lhu	$2,24($sp)
	addiu	$2,$2,44
	sll	$2,$2,1
	addu	$2,$3,$2
	lhu	$2,4($2)
	seh	$3,$2
	lw	$2,40($sp)
	lw	$2,0($2)
	andi	$6,$2,0x300
	li	$2,768			# 0x300
	bne	$6,$2,$L144
	nop

	li	$2,-32768			# 0xffffffffffff8000
	j	$L145
	nop

$L144:
	move	$2,$0
$L145:
	or	$2,$3,$2
	seh	$2,$2
	andi	$3,$2,0xffff
	addiu	$2,$5,44
	sll	$2,$2,1
	addu	$2,$4,$2
	sh	$3,4($2)
	lhu	$2,24($sp)
	move	$4,$2
	lui	$2,%hi(gwan_is_gemport_valid)
	addiu	$2,$2,%lo(gwan_is_gemport_valid)
	jalr	$2
	nop

	move	$3,$2
	li	$2,1			# 0x1
	bne	$3,$2,$L146
	nop

	lhu	$3,24($sp)
	lw	$2,40($sp)
	lw	$2,0($2)
	andi	$2,$2,0x300
	xori	$2,$2,0x300
	sltu	$2,$2,1
	andi	$2,$2,0x00ff
	move	$4,$3
	li	$5,3			# 0x3
	move	$6,$2
	lui	$2,%hi(gwan_config_gemport)
	addiu	$2,$2,%lo(gwan_config_gemport)
	jalr	$2
	nop

$L146:
	lw	$2,40($sp)
	lbu	$2,1($2)
	move	$4,$2
	lw	$5,40($sp)
	lui	$2,%hi(ploam_send_acknowledge_msg)
	addiu	$2,$2,%lo(ploam_send_acknowledge_msg)
	jalr	$2
	nop

	move	$2,$0
$L143:
	lw	$31,36($sp)
	addiu	$sp,$sp,40
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	ploam_recv_encrypt_port_id
	.size	ploam_recv_encrypt_port_id, .-ploam_recv_encrypt_port_id
	.rdata
	.align	2
$LC29:
	.ascii	"[%lu0ms]PLOAM: Receive Request_Password message.(%.8X %."
	.ascii	"8X %.8X)\012\000"
	.section	.text.ploam_recv_request_passwd,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	ploam_recv_request_passwd
	.type	ploam_recv_request_passwd, @function
ploam_recv_request_passwd:
	.frame	$sp,32,$31		# vars= 0, regs= 1/0, args= 24, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-32
	sw	$31,28($sp)
	sw	$4,32($sp)
	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	lhu	$2,-30816($2)
	andi	$2,$2,0x10
	beq	$2,$0,$L148
	nop

	lui	$2,%hi(jiffies)
	lw	$5,%lo(jiffies)($2)
	lw	$2,32($sp)
	lw	$6,0($2)
	lw	$2,32($sp)
	lw	$3,4($2)
	lw	$2,32($sp)
	lw	$2,8($2)
	sw	$2,16($sp)
	lui	$2,%hi($LC29)
	addiu	$4,$2,%lo($LC29)
	move	$7,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L148:
	lw	$2,32($sp)
	lbu	$3,0($2)
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lbu	$2,204($2)
	beq	$3,$2,$L149
	nop

	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	lhu	$2,-30816($2)
	andi	$2,$2,0x11
	beq	$2,$0,$L150
	nop

	lui	$2,%hi(jiffies)
	lw	$3,%lo(jiffies)($2)
	lw	$2,32($sp)
	lbu	$2,0($2)
	move	$6,$2
	lui	$2,%hi($LC15)
	addiu	$4,$2,%lo($LC15)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L150:
	li	$2,-1			# 0xffffffffffffffff
	j	$L151
	nop

$L149:
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lw	$3,4($2)
	li	$2,5			# 0x5
	bne	$3,$2,$L152
	nop

	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	addiu	$2,$2,216
	move	$4,$2
	li	$5,10			# 0xa
	lui	$2,%hi(ploam_send_passwd_msg)
	addiu	$2,$2,%lo(ploam_send_passwd_msg)
	jalr	$2
	nop

$L152:
	move	$2,$0
$L151:
	lw	$31,28($sp)
	addiu	$sp,$sp,32
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	ploam_recv_request_passwd
	.size	ploam_recv_request_passwd, .-ploam_recv_request_passwd
	.rdata
	.align	2
$LC30:
	.ascii	"[%lu0ms]PLOAM: Receive Assign_AllocID message.(%.8X %.8X"
	.ascii	" %.8X)\012\000"
	.align	2
$LC31:
	.ascii	"AllocId has exceed the max num %d\012\000"
	.align	2
$LC32:
	.ascii	"[%lu0ms]PLOAM: OLT Assign ALLOC_ID %d\012\000"
	.section	.text.ploam_recv_assign_alloc_id,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	ploam_recv_assign_alloc_id
	.type	ploam_recv_assign_alloc_id, @function
ploam_recv_assign_alloc_id:
	.frame	$sp,40,$31		# vars= 8, regs= 1/0, args= 24, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-40
	sw	$31,36($sp)
	sw	$4,40($sp)
	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	lhu	$2,-30816($2)
	andi	$2,$2,0x10
	beq	$2,$0,$L154
	nop

	lui	$2,%hi(jiffies)
	lw	$5,%lo(jiffies)($2)
	lw	$2,40($sp)
	lw	$6,0($2)
	lw	$2,40($sp)
	lw	$3,4($2)
	lw	$2,40($sp)
	lw	$2,8($2)
	sw	$2,16($sp)
	lui	$2,%hi($LC30)
	addiu	$4,$2,%lo($LC30)
	move	$7,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L154:
	lw	$2,40($sp)
	lbu	$3,0($2)
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lbu	$2,204($2)
	beq	$3,$2,$L155
	nop

	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	lhu	$2,-30816($2)
	andi	$2,$2,0x11
	beq	$2,$0,$L156
	nop

	lui	$2,%hi(jiffies)
	lw	$3,%lo(jiffies)($2)
	lw	$2,40($sp)
	lbu	$2,0($2)
	move	$6,$2
	lui	$2,%hi($LC15)
	addiu	$4,$2,%lo($LC15)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L156:
	li	$2,-1			# 0xffffffffffffffff
	j	$L157
	nop

$L155:
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lw	$3,4($2)
	li	$2,5			# 0x5
	bne	$3,$2,$L158
	nop

	lw	$2,40($sp)
	lbu	$2,2($2)
	sll	$2,$2,4
	seh	$3,$2
	lw	$2,40($sp)
	lw	$2,0($2)
	ext	$2,$2,4,4
	andi	$2,$2,0x00ff
	seh	$2,$2
	or	$2,$3,$2
	seh	$2,$2
	sh	$2,24($sp)
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lbu	$2,204($2)
	andi	$2,$2,0xffff
	lhu	$3,24($sp)
	beq	$3,$2,$L159
	nop

	lw	$2,40($sp)
	lbu	$3,4($2)
	li	$2,1			# 0x1
	bne	$3,$2,$L160
	nop

	lhu	$2,24($sp)
	move	$4,$2
	lui	$2,%hi(gwanCheckAllocIdExist)
	addiu	$2,$2,%lo(gwanCheckAllocIdExist)
	jalr	$2
	nop

	bne	$2,$0,$L161
	nop

	lhu	$2,24($sp)
	move	$4,$2
	lui	$2,%hi(gwan_create_new_tcont)
	addiu	$2,$2,%lo(gwan_create_new_tcont)
	jalr	$2
	nop

	bne	$2,$0,$L161
	nop

	lui	$2,%hi(gpWanPriv)
	lw	$2,%lo(gpWanPriv)($2)
	lw	$3,20($2)
	addiu	$3,$3,1
	sw	$3,20($2)
	lui	$2,%hi(gpWanPriv)
	lw	$2,%lo(gpWanPriv)($2)
	lw	$2,20($2)
	sltu	$2,$2,33
	bne	$2,$0,$L162
	nop

	lui	$2,%hi($LC31)
	addiu	$4,$2,%lo($LC31)
	li	$5,32			# 0x20
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi(gpWanPriv)
	lw	$2,%lo(gpWanPriv)($2)
	li	$3,32			# 0x20
	sw	$3,20($2)
$L162:
	lui	$2,%hi(xpon_reset_qdma_tx_buf)
	addiu	$2,$2,%lo(xpon_reset_qdma_tx_buf)
	jalr	$2
	nop

$L161:
	lhu	$2,24($sp)
	li	$4,1			# 0x1
	li	$5,4			# 0x4
	move	$6,$2
	lui	$2,%hi(xmcs_report_event)
	addiu	$2,$2,%lo(xmcs_report_event)
	jalr	$2
	nop

	j	$L159
	nop

$L160:
	lw	$2,40($sp)
	lbu	$3,4($2)
	li	$2,255			# 0xff
	bne	$3,$2,$L159
	nop

	lhu	$2,24($sp)
	move	$4,$2
	lui	$2,%hi(gwan_remove_tcont)
	addiu	$2,$2,%lo(gwan_remove_tcont)
	jalr	$2
	nop

	bne	$2,$0,$L159
	nop

	lui	$2,%hi(gpWanPriv)
	lw	$2,%lo(gpWanPriv)($2)
	lw	$3,20($2)
	addiu	$3,$3,-1
	sw	$3,20($2)
	lui	$2,%hi(gpWanPriv)
	lw	$2,%lo(gpWanPriv)($2)
	lw	$2,20($2)
	bne	$2,$0,$L163
	nop

	lui	$2,%hi(gpWanPriv)
	lw	$2,%lo(gpWanPriv)($2)
	li	$3,1			# 0x1
	sw	$3,20($2)
$L163:
	lui	$2,%hi(xpon_reset_qdma_tx_buf)
	addiu	$2,$2,%lo(xpon_reset_qdma_tx_buf)
	jalr	$2
	nop

$L159:
	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	lhu	$2,-30816($2)
	andi	$2,$2,0x10
	beq	$2,$0,$L158
	nop

	lui	$2,%hi(jiffies)
	lw	$5,%lo(jiffies)($2)
	lhu	$3,24($sp)
	lui	$2,%hi($LC32)
	addiu	$4,$2,%lo($LC32)
	move	$6,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L158:
	lw	$2,40($sp)
	lbu	$2,1($2)
	move	$4,$2
	lw	$5,40($sp)
	lui	$2,%hi(ploam_send_acknowledge_msg)
	addiu	$2,$2,%lo(ploam_send_acknowledge_msg)
	jalr	$2
	nop

	move	$2,$0
$L157:
	lw	$31,36($sp)
	addiu	$sp,$sp,40
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	ploam_recv_assign_alloc_id
	.size	ploam_recv_assign_alloc_id, .-ploam_recv_assign_alloc_id
	.rdata
	.align	2
$LC33:
	.ascii	"[%lu0ms]PLOAM: Receive Popup message.(%.8X %.8X %.8X)\012"
	.ascii	"\000"
	.section	.text.ploam_recv_popup_msg,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	ploam_recv_popup_msg
	.type	ploam_recv_popup_msg, @function
ploam_recv_popup_msg:
	.frame	$sp,32,$31		# vars= 0, regs= 1/0, args= 24, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-32
	sw	$31,28($sp)
	sw	$4,32($sp)
	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	lhu	$2,-30816($2)
	andi	$2,$2,0x10
	beq	$2,$0,$L165
	nop

	lui	$2,%hi(jiffies)
	lw	$5,%lo(jiffies)($2)
	lw	$2,32($sp)
	lw	$6,0($2)
	lw	$2,32($sp)
	lw	$3,4($2)
	lw	$2,32($sp)
	lw	$2,8($2)
	sw	$2,16($sp)
	lui	$2,%hi($LC33)
	addiu	$4,$2,%lo($LC33)
	move	$7,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L165:
	lw	$2,32($sp)
	lbu	$3,0($2)
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lbu	$2,204($2)
	beq	$3,$2,$L166
	nop

	lw	$2,32($sp)
	lbu	$3,0($2)
	li	$2,255			# 0xff
	beq	$3,$2,$L166
	nop

	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	lhu	$2,-30816($2)
	andi	$2,$2,0x11
	beq	$2,$0,$L167
	nop

	lui	$2,%hi(jiffies)
	lw	$3,%lo(jiffies)($2)
	lw	$2,32($sp)
	lbu	$2,0($2)
	move	$6,$2
	lui	$2,%hi($LC15)
	addiu	$4,$2,%lo($LC15)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L167:
	li	$2,-1			# 0xffffffffffffffff
	j	$L168
	nop

$L166:
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lw	$3,4($2)
	li	$2,6			# 0x6
	bne	$3,$2,$L169
	nop

	lui	$2,%hi(gpGponPriv)
	lw	$3,%lo(gpGponPriv)($2)
	lbu	$2,464($3)
	li	$4,1			# 0x1
	ins	$2,$4,7,1
	sb	$2,464($3)
	lw	$2,32($sp)
	lbu	$3,0($2)
	li	$2,255			# 0xff
	bne	$3,$2,$L170
	nop

	li	$4,4			# 0x4
	lui	$2,%hi(gpon_act_change_gpon_state)
	addiu	$2,$2,%lo(gpon_act_change_gpon_state)
	jalr	$2
	nop

	j	$L169
	nop

$L170:
	li	$4,5			# 0x5
	lui	$2,%hi(gpon_act_change_gpon_state)
	addiu	$2,$2,%lo(gpon_act_change_gpon_state)
	jalr	$2
	nop

$L169:
	move	$2,$0
$L168:
	lw	$31,28($sp)
	addiu	$sp,$sp,32
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	ploam_recv_popup_msg
	.size	ploam_recv_popup_msg, .-ploam_recv_popup_msg
	.rdata
	.align	2
$LC34:
	.ascii	"[%lu0ms]PLOAM: Receive Request_Key message.(%.8X %.8X %."
	.ascii	"8X)\012\000"
	.section	.text.ploam_recv_request_key,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	ploam_recv_request_key
	.type	ploam_recv_request_key, @function
ploam_recv_request_key:
	.frame	$sp,32,$31		# vars= 0, regs= 1/0, args= 24, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-32
	sw	$31,28($sp)
	sw	$4,32($sp)
	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	lhu	$2,-30816($2)
	andi	$2,$2,0x10
	beq	$2,$0,$L172
	nop

	lui	$2,%hi(jiffies)
	lw	$5,%lo(jiffies)($2)
	lw	$2,32($sp)
	lw	$6,0($2)
	lw	$2,32($sp)
	lw	$3,4($2)
	lw	$2,32($sp)
	lw	$2,8($2)
	sw	$2,16($sp)
	lui	$2,%hi($LC34)
	addiu	$4,$2,%lo($LC34)
	move	$7,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L172:
	lw	$2,32($sp)
	lbu	$3,0($2)
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lbu	$2,204($2)
	beq	$3,$2,$L173
	nop

	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	lhu	$2,-30816($2)
	andi	$2,$2,0x11
	beq	$2,$0,$L174
	nop

	lui	$2,%hi(jiffies)
	lw	$3,%lo(jiffies)($2)
	lw	$2,32($sp)
	lbu	$2,0($2)
	move	$6,$2
	lui	$2,%hi($LC15)
	addiu	$4,$2,%lo($LC15)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L174:
	li	$2,-1			# 0xffffffffffffffff
	j	$L175
	nop

$L173:
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lw	$3,4($2)
	li	$2,5			# 0x5
	bne	$3,$2,$L176
	nop

	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	addiu	$2,$2,227
	move	$4,$2
	li	$5,24			# 0x18
	lui	$2,%hi(get_random_bytes)
	addiu	$2,$2,%lo(get_random_bytes)
	jalr	$2
	nop

	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lui	$3,%hi(gpGponPriv)
	lw	$3,%lo(gpGponPriv)($3)
	lbu	$3,226($3)
	sltu	$3,$3,1
	andi	$3,$3,0x00ff
	sb	$3,226($2)
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	addiu	$2,$2,227
	move	$4,$2
	lui	$2,%hi(gponDevSetEncryptKey)
	addiu	$2,$2,%lo(gponDevSetEncryptKey)
	jalr	$2
	nop

	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lbu	$2,226($2)
	move	$3,$2
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	addiu	$2,$2,227
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(ploam_send_encryption_key)
	addiu	$2,$2,%lo(ploam_send_encryption_key)
	jalr	$2
	nop

	lui	$2,%hi(gpGponPriv)
	lw	$3,%lo(gpGponPriv)($2)
	lhu	$2,448($3)
	li	$4,1			# 0x1
	ins	$2,$4,15,1
	sh	$2,448($3)
$L176:
	move	$2,$0
$L175:
	lw	$31,28($sp)
	addiu	$sp,$sp,32
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	ploam_recv_request_key
	.size	ploam_recv_request_key, .-ploam_recv_request_key
	.rdata
	.align	2
$LC35:
	.ascii	"[%lu0ms]PLOAM: Receive Configure_PortID message.(%.8X %."
	.ascii	"8X %.8X)\012\000"
	.section	.text.ploam_recv_config_port_id,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	ploam_recv_config_port_id
	.type	ploam_recv_config_port_id, @function
ploam_recv_config_port_id:
	.frame	$sp,40,$31		# vars= 8, regs= 1/0, args= 24, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-40
	sw	$31,36($sp)
	sw	$4,40($sp)
	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	lhu	$2,-30816($2)
	andi	$2,$2,0x10
	beq	$2,$0,$L178
	nop

	lui	$2,%hi(jiffies)
	lw	$5,%lo(jiffies)($2)
	lw	$2,40($sp)
	lw	$6,0($2)
	lw	$2,40($sp)
	lw	$3,4($2)
	lw	$2,40($sp)
	lw	$2,8($2)
	sw	$2,16($sp)
	lui	$2,%hi($LC35)
	addiu	$4,$2,%lo($LC35)
	move	$7,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L178:
	lw	$2,40($sp)
	lbu	$3,0($2)
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lbu	$2,204($2)
	beq	$3,$2,$L179
	nop

	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	lhu	$2,-30816($2)
	andi	$2,$2,0x11
	beq	$2,$0,$L180
	nop

	lui	$2,%hi(jiffies)
	lw	$3,%lo(jiffies)($2)
	lw	$2,40($sp)
	lbu	$2,0($2)
	move	$6,$2
	lui	$2,%hi($LC15)
	addiu	$4,$2,%lo($LC15)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L180:
	li	$2,-1			# 0xffffffffffffffff
	j	$L185
	nop

$L179:
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lw	$3,4($2)
	li	$2,5			# 0x5
	bne	$3,$2,$L182
	nop

	lw	$2,40($sp)
	lbu	$2,3($2)
	sll	$2,$2,4
	seh	$3,$2
	lw	$2,40($sp)
	lw	$2,4($2)
	ext	$2,$2,28,4
	andi	$2,$2,0x00ff
	seh	$2,$2
	or	$2,$3,$2
	seh	$2,$2
	sh	$2,24($sp)
	lw	$2,40($sp)
	lw	$2,0($2)
	andi	$2,$2,0x100
	beq	$2,$0,$L183
	nop

	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lhu	$3,24($sp)
	sh	$3,252($2)
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16456
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	sw	$2,28($sp)
	lw	$2,28($sp)
	li	$3,1			# 0x1
	ins	$2,$3,16,1
	sw	$2,28($sp)
	lhu	$2,24($sp)
	andi	$2,$2,0xfff
	andi	$3,$2,0xffff
	lw	$2,28($sp)
	ins	$2,$3,0,12
	sw	$2,28($sp)
	lw	$3,28($sp)
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16456
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lbu	$2,204($2)
	move	$4,$2
	lui	$2,%hi(gwan_create_new_tcont)
	addiu	$2,$2,%lo(gwan_create_new_tcont)
	jalr	$2
	nop

	lhu	$2,24($sp)
	move	$4,$2
	move	$5,$0
	move	$6,$0
	lui	$2,%hi(gwan_create_new_gemport)
	addiu	$2,$2,%lo(gwan_create_new_gemport)
	jalr	$2
	nop

	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lw	$3,448($2)
	li	$2,-2147483648			# 0xffffffff80000000
	and	$2,$3,$2
	bne	$2,$0,$L184
	nop

	move	$4,$0
	lui	$2,%hi(gponDevSetKeySwithTime)
	addiu	$2,$2,%lo(gponDevSetKeySwithTime)
	jalr	$2
	nop

	lui	$2,%hi(gpGponPriv)
	lw	$3,%lo(gpGponPriv)($2)
	lhu	$2,448($3)
	li	$4,1			# 0x1
	ins	$2,$4,15,1
	sh	$2,448($3)
$L184:
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lbu	$2,204($2)
	li	$4,1			# 0x1
	li	$5,4			# 0x4
	move	$6,$2
	lui	$2,%hi(xmcs_report_event)
	addiu	$2,$2,%lo(xmcs_report_event)
	jalr	$2
	nop

	j	$L182
	nop

$L183:
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	li	$3,-1			# 0xffffffffffffffff
	sh	$3,252($2)
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16456
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	sw	$2,28($sp)
	lw	$2,28($sp)
	ins	$2,$0,16,1
	sw	$2,28($sp)
	lw	$3,28($sp)
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16456
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	lhu	$2,24($sp)
	move	$4,$2
	lui	$2,%hi(gwan_remove_gemport)
	addiu	$2,$2,%lo(gwan_remove_gemport)
	jalr	$2
	nop

	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lbu	$2,204($2)
	move	$4,$2
	lui	$2,%hi(gwan_remove_tcont)
	addiu	$2,$2,%lo(gwan_remove_tcont)
	jalr	$2
	nop

$L182:
	lw	$2,40($sp)
	lbu	$2,1($2)
	move	$4,$2
	lw	$5,40($sp)
	lui	$2,%hi(ploam_send_acknowledge_msg)
	addiu	$2,$2,%lo(ploam_send_acknowledge_msg)
	jalr	$2
	nop

	move	$2,$0
$L185:
	lw	$31,36($sp)
	addiu	$sp,$sp,40
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	ploam_recv_config_port_id
	.size	ploam_recv_config_port_id, .-ploam_recv_config_port_id
	.rdata
	.align	2
$LC36:
	.ascii	"[%lu0ms]PLOAM: Receive PEE message.(%.8X %.8X %.8X)\012\000"
	.section	.text.ploam_recv_pee_msg,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	ploam_recv_pee_msg
	.type	ploam_recv_pee_msg, @function
ploam_recv_pee_msg:
	.frame	$sp,32,$31		# vars= 0, regs= 1/0, args= 24, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-32
	sw	$31,28($sp)
	sw	$4,32($sp)
	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	lhu	$2,-30816($2)
	andi	$2,$2,0x10
	beq	$2,$0,$L187
	nop

	lui	$2,%hi(jiffies)
	lw	$5,%lo(jiffies)($2)
	lw	$2,32($sp)
	lw	$6,0($2)
	lw	$2,32($sp)
	lw	$3,4($2)
	lw	$2,32($sp)
	lw	$2,8($2)
	sw	$2,16($sp)
	lui	$2,%hi($LC36)
	addiu	$4,$2,%lo($LC36)
	move	$7,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L187:
	lw	$2,32($sp)
	lbu	$3,0($2)
	li	$2,255			# 0xff
	beq	$3,$2,$L188
	nop

	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	lhu	$2,-30816($2)
	andi	$2,$2,0x11
	beq	$2,$0,$L189
	nop

	lui	$2,%hi(jiffies)
	lw	$3,%lo(jiffies)($2)
	lw	$2,32($sp)
	lbu	$2,0($2)
	move	$6,$2
	lui	$2,%hi($LC15)
	addiu	$4,$2,%lo($LC15)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L189:
	li	$2,-1			# 0xffffffffffffffff
	j	$L190
	nop

$L188:
	move	$2,$0
$L190:
	lw	$31,28($sp)
	addiu	$sp,$sp,32
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	ploam_recv_pee_msg
	.size	ploam_recv_pee_msg, .-ploam_recv_pee_msg
	.rdata
	.align	2
$LC37:
	.ascii	"[%lu0ms]PLOAM: Receive CPL message.(%.8X %.8X %.8X)\012\000"
	.section	.text.ploam_recv_cpl_msg,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	ploam_recv_cpl_msg
	.type	ploam_recv_cpl_msg, @function
ploam_recv_cpl_msg:
	.frame	$sp,48,$31		# vars= 16, regs= 1/0, args= 24, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-48
	sw	$31,44($sp)
	sw	$4,48($sp)
	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	lhu	$2,-30816($2)
	andi	$2,$2,0x10
	beq	$2,$0,$L192
	nop

	lui	$2,%hi(jiffies)
	lw	$5,%lo(jiffies)($2)
	lw	$2,48($sp)
	lw	$6,0($2)
	lw	$2,48($sp)
	lw	$3,4($2)
	lw	$2,48($sp)
	lw	$2,8($2)
	sw	$2,16($sp)
	lui	$2,%hi($LC37)
	addiu	$4,$2,%lo($LC37)
	move	$7,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L192:
	lw	$2,48($sp)
	lbu	$3,0($2)
	li	$2,255			# 0xff
	beq	$3,$2,$L193
	nop

	lw	$2,48($sp)
	lbu	$3,0($2)
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lbu	$2,204($2)
	beq	$3,$2,$L193
	nop

	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	lhu	$2,-30816($2)
	andi	$2,$2,0x11
	beq	$2,$0,$L194
	nop

	lui	$2,%hi(jiffies)
	lw	$3,%lo(jiffies)($2)
	lw	$2,48($sp)
	lbu	$2,0($2)
	move	$6,$2
	lui	$2,%hi($LC15)
	addiu	$4,$2,%lo($LC15)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L194:
	li	$2,-1			# 0xffffffffffffffff
	j	$L195
	nop

$L193:
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lw	$3,4($2)
	li	$2,4			# 0x4
	beq	$3,$2,$L196
	nop

	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lw	$3,4($2)
	li	$2,5			# 0x5
$L196:
	move	$2,$0
$L195:
	lw	$31,44($sp)
	addiu	$sp,$sp,48
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	ploam_recv_cpl_msg
	.size	ploam_recv_cpl_msg, .-ploam_recv_cpl_msg
	.rdata
	.align	2
$LC38:
	.ascii	"[%lu0ms]PLOAM: Receive PST message.(%.8X %.8X %.8X)\012\000"
	.section	.text.ploam_recv_pst_msg,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	ploam_recv_pst_msg
	.type	ploam_recv_pst_msg, @function
ploam_recv_pst_msg:
	.frame	$sp,32,$31		# vars= 0, regs= 1/0, args= 24, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-32
	sw	$31,28($sp)
	sw	$4,32($sp)
	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	lhu	$2,-30816($2)
	andi	$2,$2,0x10
	beq	$2,$0,$L198
	nop

	lui	$2,%hi(jiffies)
	lw	$5,%lo(jiffies)($2)
	lw	$2,32($sp)
	lw	$6,0($2)
	lw	$2,32($sp)
	lw	$3,4($2)
	lw	$2,32($sp)
	lw	$2,8($2)
	sw	$2,16($sp)
	lui	$2,%hi($LC38)
	addiu	$4,$2,%lo($LC38)
	move	$7,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L198:
	lw	$2,32($sp)
	lbu	$3,0($2)
	li	$2,255			# 0xff
	beq	$3,$2,$L199
	nop

	lw	$2,32($sp)
	lbu	$3,0($2)
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lbu	$2,204($2)
	beq	$3,$2,$L199
	nop

	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	lhu	$2,-30816($2)
	andi	$2,$2,0x11
	beq	$2,$0,$L200
	nop

	lui	$2,%hi(jiffies)
	lw	$3,%lo(jiffies)($2)
	lw	$2,32($sp)
	lbu	$2,0($2)
	move	$6,$2
	lui	$2,%hi($LC15)
	addiu	$4,$2,%lo($LC15)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L200:
	li	$2,-1			# 0xffffffffffffffff
	j	$L201
	nop

$L199:
	move	$2,$0
$L201:
	lw	$31,28($sp)
	addiu	$sp,$sp,32
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	ploam_recv_pst_msg
	.size	ploam_recv_pst_msg, .-ploam_recv_pst_msg
	.rdata
	.align	2
$LC39:
	.ascii	"[%lu0ms]PLOAM: Receive BER_Interval message.(%.8X %.8X %"
	.ascii	".8X)\012\000"
	.section	.text.ploam_recv_ber_interval,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	ploam_recv_ber_interval
	.type	ploam_recv_ber_interval, @function
ploam_recv_ber_interval:
	.frame	$sp,72,$31		# vars= 40, regs= 1/0, args= 24, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-72
	sw	$31,68($sp)
	sw	$4,72($sp)
	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	lhu	$2,-30816($2)
	andi	$2,$2,0x10
	beq	$2,$0,$L203
	nop

	lui	$2,%hi(jiffies)
	lw	$5,%lo(jiffies)($2)
	lw	$2,72($sp)
	lw	$6,0($2)
	lw	$2,72($sp)
	lw	$3,4($2)
	lw	$2,72($sp)
	lw	$2,8($2)
	sw	$2,16($sp)
	lui	$2,%hi($LC39)
	addiu	$4,$2,%lo($LC39)
	move	$7,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L203:
	lw	$2,72($sp)
	lbu	$3,0($2)
	li	$2,255			# 0xff
	beq	$3,$2,$L204
	nop

	lw	$2,72($sp)
	lbu	$3,0($2)
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lbu	$2,204($2)
	beq	$3,$2,$L204
	nop

	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	lhu	$2,-30816($2)
	andi	$2,$2,0x11
	beq	$2,$0,$L205
	nop

	lui	$2,%hi(jiffies)
	lw	$3,%lo(jiffies)($2)
	lw	$2,72($sp)
	lbu	$2,0($2)
	move	$6,$2
	lui	$2,%hi($LC15)
	addiu	$4,$2,%lo($LC15)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L205:
	li	$2,-1			# 0xffffffffffffffff
	j	$L206
	nop

$L204:
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lw	$3,4($2)
	li	$2,5			# 0x5
	bne	$3,$2,$L207
	nop

	lw	$2,72($sp)
	lbu	$2,2($2)
	sll	$3,$2,24
	lw	$2,72($sp)
	lbu	$2,3($2)
	sll	$2,$2,16
	or	$3,$3,$2
	lw	$2,72($sp)
	lbu	$2,4($2)
	sll	$2,$2,8
	or	$2,$3,$2
	lw	$3,72($sp)
	lbu	$3,5($3)
	or	$2,$2,$3
	sw	$2,24($sp)
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lw	$3,24($sp)
	srl	$3,$3,3
	sw	$3,264($2)
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lui	$3,%hi(gpGponPriv)
	lw	$3,%lo(gpGponPriv)($3)
	lw	$3,264($3)
	sw	$3,84($2)
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lw	$2,264($2)
	beq	$2,$0,$L207
	nop

	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	addiu	$4,$2,64
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lw	$2,84($2)
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

	li	$2,2			# 0x2
	sw	$2,40($sp)
	li	$2,9			# 0x9
	sw	$2,28($sp)
	addiu	$2,$sp,40
	sw	$2,32($sp)
	sw	$0,44($sp)
	sw	$0,48($sp)
	sw	$0,52($sp)
	sw	$0,56($sp)
	li	$2,1			# 0x1
	sw	$2,44($sp)
	lw	$2,28($sp)
	sw	$2,48($sp)
	lw	$2,32($sp)
	sw	$2,56($sp)
	addiu	$2,$sp,44
	sw	$2,36($sp)
	li	$4,25			# 0x19
	move	$5,$0
	lw	$6,36($sp)
	lui	$2,%hi(__ECNT_HOOK)
	addiu	$2,$2,%lo(__ECNT_HOOK)
	jalr	$2
	nop

	move	$3,$2
	li	$2,-1			# 0xffffffffffffffff
	bne	$3,$2,$L208
	nop

	lw	$2,36($sp)
	lw	$3,4($2)
	lui	$2,%hi($LC2)
	addiu	$4,$2,%lo($LC2)
	move	$5,$3
	lui	$2,%hi(panic)
	addiu	$2,$2,%lo(panic)
	jalr	$2
	nop

$L208:
	lw	$2,36($sp)
	lw	$3,8($2)
	li	$2,-1			# 0xffffffffffffffff
	bne	$3,$2,$L207
	nop

	lw	$2,36($sp)
	lw	$5,0($2)
	lw	$2,36($sp)
	lw	$3,4($2)
	lui	$2,%hi($LC3)
	addiu	$4,$2,%lo($LC3)
	move	$6,$3
	lui	$2,%hi(panic)
	addiu	$2,$2,%lo(panic)
	jalr	$2
	nop

$L207:
	lw	$2,72($sp)
	lbu	$2,1($2)
	move	$4,$2
	lw	$5,72($sp)
	lui	$2,%hi(ploam_send_acknowledge_msg)
	addiu	$2,$2,%lo(ploam_send_acknowledge_msg)
	jalr	$2
	nop

	move	$2,$0
$L206:
	lw	$31,68($sp)
	addiu	$sp,$sp,72
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	ploam_recv_ber_interval
	.size	ploam_recv_ber_interval, .-ploam_recv_ber_interval
	.rdata
	.align	2
$LC40:
	.ascii	"[%lu0ms]PLOAM: Receive Key_Switching_Time message.(%.8X "
	.ascii	"%.8X %.8X)\012\000"
	.section	.text.ploam_recv_key_switch_time,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	ploam_recv_key_switch_time
	.type	ploam_recv_key_switch_time, @function
ploam_recv_key_switch_time:
	.frame	$sp,40,$31		# vars= 8, regs= 1/0, args= 24, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-40
	sw	$31,36($sp)
	sw	$4,40($sp)
	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	lhu	$2,-30816($2)
	andi	$2,$2,0x10
	beq	$2,$0,$L213
	nop

	lui	$2,%hi(jiffies)
	lw	$5,%lo(jiffies)($2)
	lw	$2,40($sp)
	lw	$6,0($2)
	lw	$2,40($sp)
	lw	$3,4($2)
	lw	$2,40($sp)
	lw	$2,8($2)
	sw	$2,16($sp)
	lui	$2,%hi($LC40)
	addiu	$4,$2,%lo($LC40)
	move	$7,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L213:
	lw	$2,40($sp)
	lbu	$3,0($2)
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lbu	$2,204($2)
	beq	$3,$2,$L214
	nop

	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	lhu	$2,-30816($2)
	andi	$2,$2,0x11
	beq	$2,$0,$L215
	nop

	lui	$2,%hi(jiffies)
	lw	$3,%lo(jiffies)($2)
	lw	$2,40($sp)
	lbu	$2,0($2)
	move	$6,$2
	lui	$2,%hi($LC15)
	addiu	$4,$2,%lo($LC15)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L215:
	li	$2,-1			# 0xffffffffffffffff
	j	$L216
	nop

$L214:
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lw	$3,4($2)
	li	$2,5			# 0x5
	bne	$3,$2,$L217
	nop

	lw	$2,40($sp)
	lbu	$2,2($2)
	sll	$3,$2,24
	lw	$2,40($sp)
	lbu	$2,3($2)
	sll	$2,$2,16
	or	$3,$3,$2
	lw	$2,40($sp)
	lbu	$2,4($2)
	sll	$2,$2,8
	or	$2,$3,$2
	lw	$3,40($sp)
	lbu	$3,5($3)
	or	$2,$2,$3
	sw	$2,24($sp)
	lw	$4,24($sp)
	lui	$2,%hi(gponDevSetKeySwithTime)
	addiu	$2,$2,%lo(gponDevSetKeySwithTime)
	jalr	$2
	nop

$L217:
	lw	$2,40($sp)
	lbu	$2,1($2)
	move	$4,$2
	lw	$5,40($sp)
	lui	$2,%hi(ploam_send_acknowledge_msg)
	addiu	$2,$2,%lo(ploam_send_acknowledge_msg)
	jalr	$2
	nop

	move	$2,$0
$L216:
	lw	$31,36($sp)
	addiu	$sp,$sp,40
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	ploam_recv_key_switch_time
	.size	ploam_recv_key_switch_time, .-ploam_recv_key_switch_time
	.rdata
	.align	2
$LC41:
	.ascii	"[%lu0ms]PLOAM: Receive Extended_Burst_Length message.(%."
	.ascii	"8X %.8X %.8X)\012\000"
	.align	2
$LC42:
	.ascii	"Enable\000"
	.align	2
$LC43:
	.ascii	"Disable\000"
	.align	2
$LC44:
	.ascii	"[%lu0ms]PLOAM: Setting the O3/O4 type 3 preamble. preamb"
	.ascii	"leFlag:%s, o3Overhead:%d, O5Overhead:%d\012\000"
	.align	2
$LC45:
	.ascii	"[%lu0ms]PLOAM: Setting the O3/O4 type 3 preamble. Overfl"
	.ascii	"ow: %d, Rec. Value: %d, Set Value: %d\012\000"
	.section	.text.ploam_recv_extended_burst_length,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	ploam_recv_extended_burst_length
	.type	ploam_recv_extended_burst_length, @function
ploam_recv_extended_burst_length:
	.frame	$sp,112,$31		# vars= 80, regs= 1/0, args= 24, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-112
	sw	$31,108($sp)
	sw	$4,112($sp)
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lbu	$2,279($2)
	sw	$2,32($sp)
	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	lhu	$2,-30816($2)
	andi	$2,$2,0x10
	beq	$2,$0,$L219
	nop

	lui	$2,%hi(jiffies)
	lw	$5,%lo(jiffies)($2)
	lw	$2,112($sp)
	lw	$6,0($2)
	lw	$2,112($sp)
	lw	$3,4($2)
	lw	$2,112($sp)
	lw	$2,8($2)
	sw	$2,16($sp)
	lui	$2,%hi($LC41)
	addiu	$4,$2,%lo($LC41)
	move	$7,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L219:
	lw	$2,112($sp)
	lbu	$3,0($2)
	li	$2,255			# 0xff
	beq	$3,$2,$L220
	nop

	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	lhu	$2,-30816($2)
	andi	$2,$2,0x11
	beq	$2,$0,$L221
	nop

	lui	$2,%hi(jiffies)
	lw	$3,%lo(jiffies)($2)
	lw	$2,112($sp)
	lbu	$2,0($2)
	move	$6,$2
	lui	$2,%hi($LC15)
	addiu	$4,$2,%lo($LC15)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L221:
	li	$2,-1			# 0xffffffffffffffff
	j	$L238
	nop

$L220:
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lw	$3,4($2)
	li	$2,3			# 0x3
	bne	$3,$2,$L223
	nop

	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16532
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	sw	$2,60($sp)
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16536
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	sw	$2,64($sp)
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16544
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	sw	$2,68($sp)
	lbu	$2,63($sp)
	move	$3,$2
	lbu	$2,67($sp)
	addu	$2,$3,$2
	lbu	$3,66($sp)
	addu	$2,$2,$3
	lbu	$3,71($sp)
	addu	$2,$2,$3
	sra	$2,$2,3
	sw	$2,36($sp)
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lw	$3,448($2)
	li	$2,134217728			# 0x8000000
	and	$2,$3,$2
	beq	$2,$0,$L224
	nop

	lw	$2,112($sp)
	lbu	$2,2($2)
	sw	$2,24($sp)
	lw	$2,112($sp)
	lbu	$2,3($2)
	sw	$2,28($sp)
	j	$L225
	nop

$L224:
	lw	$2,112($sp)
	lbu	$2,2($2)
	move	$3,$2
	lw	$2,36($sp)
	addu	$2,$3,$2
	sltu	$2,$2,129
	bne	$2,$0,$L226
	nop

	li	$3,128			# 0x80
	lw	$2,36($sp)
	subu	$2,$3,$2
	j	$L227
	nop

$L226:
	lw	$2,112($sp)
	lbu	$2,2($2)
$L227:
	sw	$2,24($sp)
	lw	$2,112($sp)
	lbu	$2,3($2)
	move	$3,$2
	lw	$2,36($sp)
	addu	$2,$3,$2
	sltu	$2,$2,129
	bne	$2,$0,$L228
	nop

	li	$3,128			# 0x80
	lw	$2,36($sp)
	subu	$2,$3,$2
	j	$L229
	nop

$L228:
	lw	$2,112($sp)
	lbu	$2,3($2)
$L229:
	sw	$2,28($sp)
$L225:
	addiu	$2,$sp,72
	sw	$2,40($sp)
	lw	$2,40($sp)
	li	$3,240			# 0xf0
	sw	$3,12($2)
	lw	$2,24($sp)
	andi	$3,$2,0x00ff
	lw	$2,32($sp)
	andi	$2,$2,0x00ff
	subu	$2,$3,$2
	andi	$3,$2,0x00ff
	lw	$2,40($sp)
	sb	$3,4($2)
	lw	$2,28($sp)
	andi	$3,$2,0x00ff
	lw	$2,40($sp)
	sb	$3,5($2)
	lw	$2,40($sp)
	li	$3,1			# 0x1
	sb	$3,6($2)
	lw	$2,40($sp)
	li	$3,2			# 0x2
	sb	$3,7($2)
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lw	$3,448($2)
	li	$2,134217728			# 0x8000000
	and	$2,$3,$2
	beq	$2,$0,$L230
	nop

	lw	$2,40($sp)
	lw	$2,12($2)
	ori	$3,$2,0x100
	lw	$2,40($sp)
	sw	$3,12($2)
	lw	$2,40($sp)
	li	$3,1			# 0x1
	sb	$3,9($2)
	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	lhu	$2,-30816($2)
	andi	$2,$2,0x10
	beq	$2,$0,$L230
	nop

	lui	$2,%hi(jiffies)
	lw	$5,%lo(jiffies)($2)
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lw	$3,448($2)
	li	$2,134217728			# 0x8000000
	and	$2,$3,$2
	beq	$2,$0,$L231
	nop

	lui	$2,%hi($LC42)
	addiu	$2,$2,%lo($LC42)
	j	$L232
	nop

$L231:
	lui	$2,%hi($LC43)
	addiu	$2,$2,%lo($LC43)
$L232:
	lw	$3,112($sp)
	lbu	$3,2($3)
	move	$4,$3
	lw	$3,36($sp)
	addu	$7,$4,$3
	lw	$3,112($sp)
	lbu	$3,3($3)
	move	$4,$3
	lw	$3,36($sp)
	addu	$3,$4,$3
	sw	$3,16($sp)
	lui	$3,%hi($LC44)
	addiu	$4,$3,%lo($LC44)
	move	$6,$2
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L230:
	li	$2,18			# 0x12
	sw	$2,44($sp)
	lw	$2,40($sp)
	sw	$2,48($sp)
	sw	$0,88($sp)
	sw	$0,92($sp)
	sw	$0,96($sp)
	sw	$0,100($sp)
	li	$2,1			# 0x1
	sw	$2,88($sp)
	lw	$2,44($sp)
	sw	$2,92($sp)
	lw	$2,48($sp)
	sw	$2,100($sp)
	addiu	$2,$sp,88
	sw	$2,52($sp)
	li	$4,25			# 0x19
	move	$5,$0
	lw	$6,52($sp)
	lui	$2,%hi(__ECNT_HOOK)
	addiu	$2,$2,%lo(__ECNT_HOOK)
	jalr	$2
	nop

	move	$3,$2
	li	$2,-1			# 0xffffffffffffffff
	bne	$3,$2,$L233
	nop

	lw	$2,52($sp)
	lw	$3,4($2)
	lui	$2,%hi($LC2)
	addiu	$4,$2,%lo($LC2)
	move	$5,$3
	lui	$2,%hi(panic)
	addiu	$2,$2,%lo(panic)
	jalr	$2
	nop

$L233:
	lw	$2,52($sp)
	lw	$3,8($2)
	li	$2,-1			# 0xffffffffffffffff
	bne	$3,$2,$L234
	nop

	lw	$2,52($sp)
	lw	$5,0($2)
	lw	$2,52($sp)
	lw	$3,4($2)
	lui	$2,%hi($LC3)
	addiu	$4,$2,%lo($LC3)
	move	$6,$3
	lui	$2,%hi(panic)
	addiu	$2,$2,%lo(panic)
	jalr	$2
	nop

$L234:
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16540
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	sw	$2,56($sp)
	lw	$2,56($sp)
	li	$3,1			# 0x1
	ins	$2,$3,24,1
	sw	$2,56($sp)
	lw	$2,28($sp)
	andi	$2,$2,0x00ff
	sb	$2,58($sp)
	lw	$2,24($sp)
	andi	$3,$2,0x00ff
	lw	$2,32($sp)
	andi	$2,$2,0x00ff
	subu	$2,$3,$2
	andi	$2,$2,0x00ff
	sb	$2,59($sp)
	lw	$3,56($sp)
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16540
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lw	$3,56($sp)
	sw	$3,440($2)
	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	lhu	$2,-30816($2)
	andi	$2,$2,0x10
	beq	$2,$0,$L223
	nop

	lui	$2,%hi(jiffies)
	lw	$5,%lo(jiffies)($2)
	lw	$2,112($sp)
	lbu	$2,2($2)
	move	$3,$2
	lw	$2,36($sp)
	addu	$2,$3,$2
	sltu	$2,$2,129
	xori	$2,$2,0x1
	andi	$2,$2,0x00ff
	move	$6,$2
	lw	$2,112($sp)
	lbu	$2,2($2)
	move	$7,$2
	lw	$2,32($sp)
	lw	$3,24($sp)
	subu	$2,$3,$2
	sw	$2,16($sp)
	lui	$2,%hi($LC45)
	addiu	$4,$2,%lo($LC45)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L223:
	move	$2,$0
$L238:
	lw	$31,108($sp)
	addiu	$sp,$sp,112
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	ploam_recv_extended_burst_length
	.size	ploam_recv_extended_burst_length, .-ploam_recv_extended_burst_length
	.rdata
	.align	2
$LC46:
	.ascii	"[%lu0ms]PLOAM: Receive PonID message.(%.8X %.8X %.8X)\012"
	.ascii	"\000"
	.section	.text.ploam_recv_pon_id_msg,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	ploam_recv_pon_id_msg
	.type	ploam_recv_pon_id_msg, @function
ploam_recv_pon_id_msg:
	.frame	$sp,32,$31		# vars= 0, regs= 1/0, args= 24, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-32
	sw	$31,28($sp)
	sw	$4,32($sp)
	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	lhu	$2,-30816($2)
	andi	$2,$2,0x10
	beq	$2,$0,$L240
	nop

	lui	$2,%hi(jiffies)
	lw	$5,%lo(jiffies)($2)
	lw	$2,32($sp)
	lw	$6,0($2)
	lw	$2,32($sp)
	lw	$3,4($2)
	lw	$2,32($sp)
	lw	$2,8($2)
	sw	$2,16($sp)
	lui	$2,%hi($LC46)
	addiu	$4,$2,%lo($LC46)
	move	$7,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L240:
	lw	$2,32($sp)
	lbu	$3,0($2)
	li	$2,255			# 0xff
	beq	$3,$2,$L241
	nop

	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	lhu	$2,-30816($2)
	andi	$2,$2,0x11
	beq	$2,$0,$L242
	nop

	lui	$2,%hi(jiffies)
	lw	$3,%lo(jiffies)($2)
	lw	$2,32($sp)
	lbu	$2,0($2)
	move	$6,$2
	lui	$2,%hi($LC15)
	addiu	$4,$2,%lo($LC15)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L242:
	li	$2,-1			# 0xffffffffffffffff
	j	$L243
	nop

$L241:
	move	$2,$0
$L243:
	lw	$31,28($sp)
	addiu	$sp,$sp,32
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	ploam_recv_pon_id_msg
	.size	ploam_recv_pon_id_msg, .-ploam_recv_pon_id_msg
	.rdata
	.align	2
$LC47:
	.ascii	"[%lu0ms]PLOAM: Receive Swift_Popup message.(%.8X %.8X %."
	.ascii	"8X)\012\000"
	.section	.text.ploam_recv_swift_popup,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	ploam_recv_swift_popup
	.type	ploam_recv_swift_popup, @function
ploam_recv_swift_popup:
	.frame	$sp,32,$31		# vars= 0, regs= 1/0, args= 24, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-32
	sw	$31,28($sp)
	sw	$4,32($sp)
	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	lhu	$2,-30816($2)
	andi	$2,$2,0x10
	beq	$2,$0,$L245
	nop

	lui	$2,%hi(jiffies)
	lw	$5,%lo(jiffies)($2)
	lw	$2,32($sp)
	lw	$6,0($2)
	lw	$2,32($sp)
	lw	$3,4($2)
	lw	$2,32($sp)
	lw	$2,8($2)
	sw	$2,16($sp)
	lui	$2,%hi($LC47)
	addiu	$4,$2,%lo($LC47)
	move	$7,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L245:
	lw	$2,32($sp)
	lbu	$3,0($2)
	li	$2,255			# 0xff
	beq	$3,$2,$L246
	nop

	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	lhu	$2,-30816($2)
	andi	$2,$2,0x11
	beq	$2,$0,$L247
	nop

	lui	$2,%hi(jiffies)
	lw	$3,%lo(jiffies)($2)
	lw	$2,32($sp)
	lbu	$2,0($2)
	move	$6,$2
	lui	$2,%hi($LC15)
	addiu	$4,$2,%lo($LC15)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L247:
	li	$2,-1			# 0xffffffffffffffff
	j	$L248
	nop

$L246:
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lw	$3,4($2)
	li	$2,6			# 0x6
	bne	$3,$2,$L249
	nop

	li	$4,5			# 0x5
	lui	$2,%hi(gpon_act_change_gpon_state)
	addiu	$2,$2,%lo(gpon_act_change_gpon_state)
	jalr	$2
	nop

$L249:
	move	$2,$0
$L248:
	lw	$31,28($sp)
	addiu	$sp,$sp,32
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	ploam_recv_swift_popup
	.size	ploam_recv_swift_popup, .-ploam_recv_swift_popup
	.rdata
	.align	2
$LC48:
	.ascii	"[%lu0ms]PLOAM: Receive Ranging_Adjustment message.(%.8X "
	.ascii	"%.8X %.8X)\012\000"
	.section	.text.ploam_recv_ranging_adjustment,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	ploam_recv_ranging_adjustment
	.type	ploam_recv_ranging_adjustment, @function
ploam_recv_ranging_adjustment:
	.frame	$sp,40,$31		# vars= 8, regs= 1/0, args= 24, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-40
	sw	$31,36($sp)
	sw	$4,40($sp)
	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	lhu	$2,-30816($2)
	andi	$2,$2,0x10
	beq	$2,$0,$L251
	nop

	lui	$2,%hi(jiffies)
	lw	$5,%lo(jiffies)($2)
	lw	$2,40($sp)
	lw	$6,0($2)
	lw	$2,40($sp)
	lw	$3,4($2)
	lw	$2,40($sp)
	lw	$2,8($2)
	sw	$2,16($sp)
	lui	$2,%hi($LC48)
	addiu	$4,$2,%lo($LC48)
	move	$7,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L251:
	lw	$2,40($sp)
	lbu	$3,0($2)
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lbu	$2,204($2)
	beq	$3,$2,$L252
	nop

	lw	$2,40($sp)
	lbu	$3,0($2)
	li	$2,255			# 0xff
	beq	$3,$2,$L252
	nop

	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	lhu	$2,-30816($2)
	andi	$2,$2,0x11
	beq	$2,$0,$L253
	nop

	lui	$2,%hi(jiffies)
	lw	$3,%lo(jiffies)($2)
	lw	$2,40($sp)
	lbu	$2,0($2)
	move	$6,$2
	lui	$2,%hi($LC15)
	addiu	$4,$2,%lo($LC15)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L253:
	li	$2,-1			# 0xffffffffffffffff
	j	$L254
	nop

$L252:
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lw	$3,4($2)
	li	$2,5			# 0x5
	bne	$3,$2,$L255
	nop

	lw	$2,40($sp)
	lbu	$2,3($2)
	sll	$3,$2,24
	lw	$2,40($sp)
	lbu	$2,4($2)
	sll	$2,$2,16
	or	$3,$3,$2
	lw	$2,40($sp)
	lbu	$2,5($2)
	sll	$2,$2,8
	or	$2,$3,$2
	lw	$3,40($sp)
	lbu	$3,6($3)
	or	$2,$2,$3
	sw	$2,24($sp)
	lw	$2,24($sp)
	beq	$2,$0,$L255
	nop

	lw	$2,40($sp)
	lw	$2,0($2)
	andi	$2,$2,0x200
	bne	$2,$0,$L256
	nop

	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lw	$3,268($2)
	lw	$2,24($sp)
	addu	$2,$3,$2
	move	$4,$2
	lui	$2,%hi(ploam_eqd_adjustment)
	addiu	$2,$2,%lo(ploam_eqd_adjustment)
	jalr	$2
	nop

	j	$L255
	nop

$L256:
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lw	$3,268($2)
	lw	$2,24($sp)
	subu	$2,$3,$2
	move	$4,$2
	lui	$2,%hi(ploam_eqd_adjustment)
	addiu	$2,$2,%lo(ploam_eqd_adjustment)
	jalr	$2
	nop

$L255:
	lw	$2,40($sp)
	lbu	$2,1($2)
	move	$4,$2
	lw	$5,40($sp)
	lui	$2,%hi(ploam_send_acknowledge_msg)
	addiu	$2,$2,%lo(ploam_send_acknowledge_msg)
	jalr	$2
	nop

	move	$2,$0
$L254:
	lw	$31,36($sp)
	addiu	$sp,$sp,40
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	ploam_recv_ranging_adjustment
	.size	ploam_recv_ranging_adjustment, .-ploam_recv_ranging_adjustment
	.rdata
	.align	2
$LC49:
	.ascii	"[%lu0ms]PLOAM: Receive Sleep_Allow message.(%.8X %.8X %."
	.ascii	"8X)\012\000"
	.section	.text.ploam_recv_Sleep_Allow,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	ploam_recv_Sleep_Allow
	.type	ploam_recv_Sleep_Allow, @function
ploam_recv_Sleep_Allow:
	.frame	$sp,32,$31		# vars= 0, regs= 1/0, args= 24, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-32
	sw	$31,28($sp)
	sw	$4,32($sp)
	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	lhu	$2,-30816($2)
	andi	$2,$2,0x10
	beq	$2,$0,$L258
	nop

	lui	$2,%hi(jiffies)
	lw	$5,%lo(jiffies)($2)
	lw	$2,32($sp)
	lw	$6,0($2)
	lw	$2,32($sp)
	lw	$3,4($2)
	lw	$2,32($sp)
	lw	$2,8($2)
	sw	$2,16($sp)
	lui	$2,%hi($LC49)
	addiu	$4,$2,%lo($LC49)
	move	$7,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L258:
	lw	$2,32($sp)
	lbu	$3,0($2)
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lbu	$2,204($2)
	beq	$3,$2,$L259
	nop

	lw	$2,32($sp)
	lbu	$3,0($2)
	li	$2,255			# 0xff
	beq	$3,$2,$L259
	nop

	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	lhu	$2,-30816($2)
	andi	$2,$2,0x11
	beq	$2,$0,$L260
	nop

	lui	$2,%hi(jiffies)
	lw	$3,%lo(jiffies)($2)
	lw	$2,32($sp)
	lbu	$2,0($2)
	move	$6,$2
	lui	$2,%hi($LC15)
	addiu	$4,$2,%lo($LC15)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L260:
	li	$2,-1			# 0xffffffffffffffff
	j	$L261
	nop

$L259:
	move	$2,$0
$L261:
	lw	$31,28($sp)
	addiu	$sp,$sp,32
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	ploam_recv_Sleep_Allow
	.size	ploam_recv_Sleep_Allow, .-ploam_recv_Sleep_Allow
	.section	.text.ploam_init,"ax",@progbits
	.align	2
	.globl	ploam_init
	.set	nomips16
	.set	nomicromips
	.ent	ploam_init
	.type	ploam_init, @function
ploam_init:
	.frame	$sp,24,$31		# vars= 0, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-24
	sw	$31,20($sp)
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	addiu	$2,$2,92
	move	$4,$2
	move	$5,$0
	li	$6,100			# 0x64
	lui	$2,%hi(memset)
	addiu	$2,$2,%lo(memset)
	jalr	$2
	nop

	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lui	$3,%hi(ploam_recv_upstream_overhead)
	addiu	$3,$3,%lo(ploam_recv_upstream_overhead)
	sw	$3,96($2)
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lui	$3,%hi(ploam_recv_assign_onu_id)
	addiu	$3,$3,%lo(ploam_recv_assign_onu_id)
	sw	$3,104($2)
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lui	$3,%hi(ploam_recv_ranging_time)
	addiu	$3,$3,%lo(ploam_recv_ranging_time)
	sw	$3,108($2)
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lui	$3,%hi(ploam_recv_deactivate_onu)
	addiu	$3,$3,%lo(ploam_recv_deactivate_onu)
	sw	$3,112($2)
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lui	$3,%hi(ploam_recv_disable_serial_number)
	addiu	$3,$3,%lo(ploam_recv_disable_serial_number)
	sw	$3,116($2)
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lui	$3,%hi(ploam_recv_encrypt_port_id)
	addiu	$3,$3,%lo(ploam_recv_encrypt_port_id)
	sw	$3,124($2)
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lui	$3,%hi(ploam_recv_request_passwd)
	addiu	$3,$3,%lo(ploam_recv_request_passwd)
	sw	$3,128($2)
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lui	$3,%hi(ploam_recv_assign_alloc_id)
	addiu	$3,$3,%lo(ploam_recv_assign_alloc_id)
	sw	$3,132($2)
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lui	$3,%hi(ploam_recv_popup_msg)
	addiu	$3,$3,%lo(ploam_recv_popup_msg)
	sw	$3,140($2)
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lui	$3,%hi(ploam_recv_request_key)
	addiu	$3,$3,%lo(ploam_recv_request_key)
	sw	$3,144($2)
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lui	$3,%hi(ploam_recv_config_port_id)
	addiu	$3,$3,%lo(ploam_recv_config_port_id)
	sw	$3,148($2)
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lui	$3,%hi(ploam_recv_pee_msg)
	addiu	$3,$3,%lo(ploam_recv_pee_msg)
	sw	$3,152($2)
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lui	$3,%hi(ploam_recv_cpl_msg)
	addiu	$3,$3,%lo(ploam_recv_cpl_msg)
	sw	$3,156($2)
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lui	$3,%hi(ploam_recv_pst_msg)
	addiu	$3,$3,%lo(ploam_recv_pst_msg)
	sw	$3,160($2)
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lui	$3,%hi(ploam_recv_ber_interval)
	addiu	$3,$3,%lo(ploam_recv_ber_interval)
	sw	$3,164($2)
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lui	$3,%hi(ploam_recv_key_switch_time)
	addiu	$3,$3,%lo(ploam_recv_key_switch_time)
	sw	$3,168($2)
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lui	$3,%hi(ploam_recv_extended_burst_length)
	addiu	$3,$3,%lo(ploam_recv_extended_burst_length)
	sw	$3,172($2)
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lui	$3,%hi(ploam_recv_pon_id_msg)
	addiu	$3,$3,%lo(ploam_recv_pon_id_msg)
	sw	$3,176($2)
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lui	$3,%hi(ploam_recv_swift_popup)
	addiu	$3,$3,%lo(ploam_recv_swift_popup)
	sw	$3,180($2)
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lui	$3,%hi(ploam_recv_ranging_adjustment)
	addiu	$3,$3,%lo(ploam_recv_ranging_adjustment)
	sw	$3,184($2)
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lui	$3,%hi(ploam_recv_Sleep_Allow)
	addiu	$3,$3,%lo(ploam_recv_Sleep_Allow)
	sw	$3,188($2)
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	addiu	$2,$2,192
	move	$4,$2
	move	$5,$0
	li	$6,12			# 0xc
	lui	$2,%hi(memset)
	addiu	$2,$2,%lo(memset)
	jalr	$2
	nop

	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	addiu	$2,$2,64
	move	$4,$2
	move	$5,$0
	move	$6,$0
	lui	$2,%hi(init_timer_key)
	addiu	$2,$2,%lo(init_timer_key)
	jalr	$2
	nop

	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lui	$3,%hi(gpGponPriv)
	lw	$3,%lo(gpGponPriv)($3)
	lw	$3,264($3)
	sw	$3,84($2)
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lui	$3,%hi(gpon_ber_interval_expires)
	addiu	$3,$3,%lo(gpon_ber_interval_expires)
	sw	$3,80($2)
	move	$2,$0
	lw	$31,20($sp)
	addiu	$sp,$sp,24
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	ploam_init
	.size	ploam_init, .-ploam_init
	.ident	"GCC: (Buildroot 2015.08.1) 4.9.3"
