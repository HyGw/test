	.file	1 "gpon_dvt.c"
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
$LC4:
	.ascii	"Set GEM port:%d, ID:%d Encrypt:%d failed\012\000"
	.align	2
$LC5:
	.ascii	"Get GEM port ID:%d failed\012\000"
	.align	2
$LC6:
	.ascii	"The GEM port ID:%d, field mismatch.\012\000"
	.align	2
$LC7:
	.ascii	"Repeat to verification Gem Port CSR (%d times)\012\000"
	.align	2
$LC8:
	.ascii	"Gem Port CSR verification successful (%d times)\012\000"
#NO_APP
	.section	.text.gpon_dvt_gemport,"ax",@progbits
	.align	2
	.globl	gpon_dvt_gemport
	.set	nomips16
	.set	nomicromips
	.ent	gpon_dvt_gemport
	.type	gpon_dvt_gemport, @function
gpon_dvt_gemport:
	.frame	$sp,1072,$31		# vars= 1048, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-1072
	sw	$31,1068($sp)
	sw	$4,1072($sp)
	sw	$0,24($sp)
	sw	$0,16($sp)
	j	$L2
	nop

$L15:
	sw	$0,24($sp)
	sw	$0,20($sp)
	j	$L3
	nop

$L8:
	lw	$2,20($sp)
	bne	$2,$0,$L4
	nop

	lui	$2,%hi(random32)
	addiu	$2,$2,%lo(random32)
	jalr	$2
	nop

	andi	$2,$2,0xf
	j	$L5
	nop

$L4:
	lui	$2,%hi(random32)
	addiu	$2,$2,%lo(random32)
	jalr	$2
	nop

	andi	$2,$2,0xf
	addiu	$2,$2,1
$L5:
	lw	$3,24($sp)
	addu	$2,$2,$3
	sw	$2,24($sp)
	lw	$2,24($sp)
	andi	$3,$2,0xffff
	lw	$2,20($sp)
	sll	$2,$2,2
	addiu	$4,$sp,16
	addu	$2,$4,$2
	sh	$3,16($2)
	lw	$2,20($sp)
	sll	$2,$2,2
	addiu	$3,$sp,16
	addu	$2,$3,$2
	li	$3,1			# 0x1
	sb	$3,18($2)
	lui	$2,%hi(random32)
	addiu	$2,$2,%lo(random32)
	jalr	$2
	nop

	andi	$2,$2,0x1
	sltu	$2,$0,$2
	andi	$2,$2,0x00ff
	move	$4,$2
	lw	$2,20($sp)
	sll	$2,$2,2
	addiu	$3,$sp,16
	addu	$2,$3,$2
	sb	$4,19($2)
	lw	$2,20($sp)
	sll	$2,$2,2
	addiu	$3,$sp,16
	addu	$2,$3,$2
	lhu	$2,16($2)
	move	$4,$2
	lw	$2,20($sp)
	sll	$2,$2,2
	addiu	$3,$sp,16
	addu	$2,$3,$2
	lbu	$2,18($2)
	move	$5,$2
	lw	$2,20($sp)
	sll	$2,$2,2
	addiu	$3,$sp,16
	addu	$2,$3,$2
	lbu	$2,19($2)
	move	$6,$2
	lui	$2,%hi(gponDevSetGemInfo)
	addiu	$2,$2,%lo(gponDevSetGemInfo)
	jalr	$2
	nop

	sw	$2,28($sp)
	lw	$2,28($sp)
	beq	$2,$0,$L6
	nop

	lw	$2,20($sp)
	sll	$2,$2,2
	addiu	$3,$sp,16
	addu	$2,$3,$2
	lbu	$2,19($2)
	move	$3,$2
	lui	$2,%hi($LC4)
	addiu	$4,$2,%lo($LC4)
	lw	$5,20($sp)
	lw	$6,24($sp)
	move	$7,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	li	$2,-14			# 0xfffffffffffffff2
	j	$L16
	nop

$L6:
	lw	$2,20($sp)
	addiu	$2,$2,1
	sw	$2,20($sp)
$L3:
	lw	$2,20($sp)
	slt	$2,$2,256
	bne	$2,$0,$L8
	nop

	sw	$0,20($sp)
	j	$L9
	nop

$L13:
	lw	$2,20($sp)
	sll	$2,$2,2
	addiu	$3,$sp,16
	addu	$2,$3,$2
	lhu	$2,16($2)
	move	$4,$2
	addiu	$3,$sp,1056
	addiu	$2,$sp,1057
	move	$5,$3
	move	$6,$2
	lui	$2,%hi(gponDevGetGemInfo)
	addiu	$2,$2,%lo(gponDevGetGemInfo)
	jalr	$2
	nop

	sw	$2,28($sp)
	lw	$2,28($sp)
	beq	$2,$0,$L10
	nop

	lui	$2,%hi($LC5)
	addiu	$4,$2,%lo($LC5)
	lw	$5,20($sp)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	li	$2,-14			# 0xfffffffffffffff2
	j	$L16
	nop

$L10:
	lw	$2,20($sp)
	sll	$2,$2,2
	addiu	$3,$sp,16
	addu	$2,$3,$2
	lbu	$3,18($2)
	lbu	$2,1056($sp)
	bne	$3,$2,$L11
	nop

	lw	$2,20($sp)
	sll	$2,$2,2
	addiu	$3,$sp,16
	addu	$2,$3,$2
	lbu	$3,19($2)
	lbu	$2,1057($sp)
	beq	$3,$2,$L12
	nop

$L11:
	lui	$2,%hi($LC6)
	addiu	$4,$2,%lo($LC6)
	lw	$5,20($sp)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	li	$2,-14			# 0xfffffffffffffff2
	j	$L16
	nop

$L12:
	lw	$2,20($sp)
	addiu	$2,$2,1
	sw	$2,20($sp)
$L9:
	lw	$2,20($sp)
	slt	$2,$2,256
	bne	$2,$0,$L13
	nop

	lw	$4,16($sp)
	li	$2,274857984			# 0x10620000
	ori	$2,$2,0x4dd3
	mult	$4,$2
	mfhi	$2
	sra	$3,$2,6
	sra	$2,$4,31
	subu	$3,$3,$2
	move	$5,$3
	sll	$2,$5,2
	move	$5,$2
	sll	$2,$5,5
	subu	$2,$2,$5
	addu	$2,$2,$3
	sll	$2,$2,3
	subu	$3,$4,$2
	li	$2,999			# 0x3e7
	bne	$3,$2,$L14
	nop

	lui	$2,%hi($LC7)
	addiu	$4,$2,%lo($LC7)
	lw	$5,20($sp)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L14:
	lw	$2,16($sp)
	addiu	$2,$2,1
	sw	$2,16($sp)
$L2:
	lw	$3,16($sp)
	lw	$2,1072($sp)
	slt	$2,$3,$2
	bne	$2,$0,$L15
	nop

	lui	$2,%hi($LC8)
	addiu	$4,$2,%lo($LC8)
	lw	$5,1072($sp)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	move	$2,$0
$L16:
	lw	$31,1068($sp)
	addiu	$sp,$sp,1072
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	gpon_dvt_gemport
	.size	gpon_dvt_gemport, .-gpon_dvt_gemport
	.rdata
	.align	2
$LC9:
	.ascii	"%d:ONU activation failed. The current state:O%d.\012\000"
	.align	2
$LC10:
	.ascii	"%d:Finish ONU activation. The current state:O%d and rece"
	.ascii	"ive %d OMCI messages\012\000"
	.section	.text.gpon_dvt_onu_connection,"ax",@progbits
	.align	2
	.globl	gpon_dvt_onu_connection
	.set	nomips16
	.set	nomicromips
	.ent	gpon_dvt_onu_connection
	.type	gpon_dvt_onu_connection, @function
gpon_dvt_onu_connection:
	.frame	$sp,48,$31		# vars= 24, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-48
	sw	$31,44($sp)
	sw	$4,48($sp)
	sw	$0,16($sp)
	sw	$0,20($sp)
	lui	$2,%hi(gpWanPriv)
	lw	$2,%lo(gpWanPriv)($2)
	lw	$2,4($2)
	sw	$2,36($sp)
	lw	$2,36($sp)
	addiu	$2,$2,928
	sw	$2,32($sp)
	lui	$2,%hi(gpGponPriv)
	lw	$3,%lo(gpGponPriv)($2)
	lhu	$2,448($3)
	li	$4,1			# 0x1
	ins	$2,$4,13,1
	sh	$2,448($3)
	j	$L19
	nop

$L31:
	lui	$2,%hi(gpon_enable)
	addiu	$2,$2,%lo(gpon_enable)
	jalr	$2
	nop

	sw	$0,24($sp)
	j	$L20
	nop

$L24:
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lw	$3,4($2)
	li	$2,5			# 0x5
	beq	$3,$2,$L21
	nop

	li	$4,1000			# 0x3e8
	lui	$2,%hi(msleep)
	addiu	$2,$2,%lo(msleep)
	jalr	$2
	nop

	j	$L33
	nop

$L21:
	j	$L23
	nop

$L33:
	lw	$2,24($sp)
	addiu	$2,$2,1
	sw	$2,24($sp)
$L20:
	lw	$2,24($sp)
	slt	$2,$2,300
	bne	$2,$0,$L24
	nop

$L23:
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lw	$3,4($2)
	li	$2,5			# 0x5
	beq	$3,$2,$L25
	nop

	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lw	$3,4($2)
	lui	$2,%hi($LC9)
	addiu	$4,$2,%lo($LC9)
	lw	$5,16($sp)
	move	$6,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	j	$L26
	nop

$L25:
	sw	$0,28($sp)
	j	$L27
	nop

$L29:
	lw	$3,20($sp)
	lw	$2,32($sp)
	lw	$2,52($2)
	beq	$3,$2,$L28
	nop

	lw	$2,32($sp)
	lw	$2,52($2)
	sw	$2,20($sp)
	sw	$0,28($sp)
$L28:
	li	$4,1000			# 0x3e8
	lui	$2,%hi(msleep)
	addiu	$2,$2,%lo(msleep)
	jalr	$2
	nop

	lw	$2,28($sp)
	addiu	$2,$2,1
	sw	$2,28($sp)
$L27:
	lw	$2,28($sp)
	slt	$2,$2,10
	bne	$2,$0,$L29
	nop

	lw	$2,16($sp)
	addiu	$2,$2,1
	sw	$2,16($sp)
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lw	$6,4($2)
	lw	$2,32($sp)
	lw	$3,52($2)
	lui	$2,%hi($LC10)
	addiu	$4,$2,%lo($LC10)
	lw	$5,16($sp)
	move	$7,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lw	$2,32($sp)
	sw	$0,52($2)
	lw	$2,32($sp)
	sw	$0,60($2)
	move	$4,$0
	lui	$2,%hi(gpon_disable_with_option)
	addiu	$2,$2,%lo(gpon_disable_with_option)
	jalr	$2
	nop

	li	$4,1000			# 0x3e8
	lui	$2,%hi(msleep)
	addiu	$2,$2,%lo(msleep)
	jalr	$2
	nop

	lw	$2,48($sp)
	beq	$2,$0,$L19
	nop

	lw	$3,16($sp)
	lw	$2,48($sp)
	bne	$3,$2,$L19
	nop

	lui	$2,%hi(gpGponPriv)
	lw	$3,%lo(gpGponPriv)($2)
	lhu	$2,448($3)
	ins	$2,$0,13,1
	sh	$2,448($3)
$L19:
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lw	$3,448($2)
	li	$2,536870912			# 0x20000000
	and	$2,$3,$2
	bne	$2,$0,$L31
	nop

$L26:
	move	$2,$0
	lw	$31,44($sp)
	addiu	$sp,$sp,48
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	gpon_dvt_onu_connection
	.size	gpon_dvt_onu_connection, .-gpon_dvt_onu_connection
	.rdata
	.align	2
$LC11:
	.ascii	"%d:ONU activation failed. The gpon tx packet is %d.\012\000"
	.align	2
$LC12:
	.ascii	"%d:ONU activation successful. The gpon tx packet is %d.\012"
	.ascii	"\000"
	.section	.text.gpon_dvt_transceiver_on_off_connection,"ax",@progbits
	.align	2
	.globl	gpon_dvt_transceiver_on_off_connection
	.set	nomips16
	.set	nomicromips
	.ent	gpon_dvt_transceiver_on_off_connection
	.type	gpon_dvt_transceiver_on_off_connection, @function
gpon_dvt_transceiver_on_off_connection:
	.frame	$sp,48,$31		# vars= 24, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-48
	sw	$31,44($sp)
	sw	$4,48($sp)
	li	$2,-1078001664			# 0xffffffffbfbf0000
	ori	$4,$2,0x200
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	sw	$2,24($sp)
	lw	$3,24($sp)
	li	$2,65536			# 0x10000
	or	$2,$3,$2
	sw	$2,24($sp)
	lw	$4,24($sp)
	li	$2,-1078001664			# 0xffffffffbfbf0000
	ori	$5,$2,0x200
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	li	$2,-1078001664			# 0xffffffffbfbf0000
	ori	$4,$2,0x214
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	sw	$2,24($sp)
	lw	$2,24($sp)
	ori	$2,$2,0x100
	sw	$2,24($sp)
	lw	$4,24($sp)
	li	$2,-1078001664			# 0xffffffffbfbf0000
	ori	$5,$2,0x214
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	sw	$0,16($sp)
	j	$L35
	nop

$L49:
	li	$2,-1078001664			# 0xffffffffbfbf0000
	ori	$4,$2,0x204
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	sw	$2,24($sp)
	lw	$3,24($sp)
	li	$2,-257			# 0xfffffffffffffeff
	and	$2,$3,$2
	sw	$2,24($sp)
	lw	$4,24($sp)
	li	$2,-1078001664			# 0xffffffffbfbf0000
	ori	$5,$2,0x204
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16896
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	sw	$2,28($sp)
	lw	$2,28($sp)
	li	$3,1			# 0x1
	ins	$2,$3,26,1
	sw	$2,28($sp)
	lw	$3,28($sp)
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16896
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	sw	$0,20($sp)
	j	$L36
	nop

$L40:
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lw	$3,4($2)
	li	$2,5			# 0x5
	beq	$3,$2,$L37
	nop

	li	$4,200			# 0xc8
	lui	$2,%hi(msleep)
	addiu	$2,$2,%lo(msleep)
	jalr	$2
	nop

	j	$L51
	nop

$L37:
	j	$L39
	nop

$L51:
	lw	$2,20($sp)
	addiu	$2,$2,1
	sw	$2,20($sp)
$L36:
	lw	$2,20($sp)
	slt	$2,$2,1500
	bne	$2,$0,$L40
	nop

$L39:
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lw	$3,4($2)
	li	$2,5			# 0x5
	beq	$3,$2,$L41
	nop

	lw	$2,16($sp)
	addiu	$5,$2,1
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lw	$3,4($2)
	lui	$2,%hi($LC9)
	addiu	$4,$2,%lo($LC9)
	move	$6,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	j	$L42
	nop

$L41:
	li	$4,3000			# 0xbb8
	lui	$2,%hi(msleep)
	addiu	$2,$2,%lo(msleep)
	jalr	$2
	nop

	sw	$0,20($sp)
	j	$L43
	nop

$L47:
	lui	$2,%hi(gpWanPriv)
	lw	$2,%lo(gpWanPriv)($2)
	lw	$2,8380($2)
	ext	$2,$2,20,12
	andi	$2,$2,0xffff
	move	$4,$2
	addiu	$3,$sp,36
	addiu	$2,$sp,32
	li	$5,2			# 0x2
	move	$6,$3
	move	$7,$2
	lui	$2,%hi(gponDevGetGemPortCounter)
	addiu	$2,$2,%lo(gponDevGetGemPortCounter)
	jalr	$2
	nop

	lw	$3,32($sp)
	li	$2,458752			# 0x70000
	ori	$2,$2,0xa120
	sltu	$2,$3,$2
	beq	$2,$0,$L44
	nop

	li	$4,200			# 0xc8
	lui	$2,%hi(msleep)
	addiu	$2,$2,%lo(msleep)
	jalr	$2
	nop

	j	$L52
	nop

$L44:
	j	$L46
	nop

$L52:
	lw	$2,20($sp)
	addiu	$2,$2,1
	sw	$2,20($sp)
$L43:
	lw	$2,20($sp)
	slt	$2,$2,40
	bne	$2,$0,$L47
	nop

$L46:
	lw	$3,20($sp)
	li	$2,10			# 0xa
	bne	$3,$2,$L48
	nop

	lw	$2,16($sp)
	addiu	$5,$2,1
	lw	$3,32($sp)
	lui	$2,%hi($LC11)
	addiu	$4,$2,%lo($LC11)
	move	$6,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	j	$L42
	nop

$L48:
	lw	$2,16($sp)
	addiu	$5,$2,1
	lw	$3,32($sp)
	lui	$2,%hi($LC12)
	addiu	$4,$2,%lo($LC12)
	move	$6,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	li	$2,-1078001664			# 0xffffffffbfbf0000
	ori	$4,$2,0x204
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	sw	$2,24($sp)
	lw	$2,24($sp)
	ori	$2,$2,0x100
	sw	$2,24($sp)
	lw	$4,24($sp)
	li	$2,-1078001664			# 0xffffffffbfbf0000
	ori	$5,$2,0x204
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	li	$4,2000			# 0x7d0
	lui	$2,%hi(msleep)
	addiu	$2,$2,%lo(msleep)
	jalr	$2
	nop

	lw	$2,16($sp)
	addiu	$2,$2,1
	sw	$2,16($sp)
$L35:
	lw	$3,16($sp)
	lw	$2,48($sp)
	slt	$2,$3,$2
	bne	$2,$0,$L49
	nop

$L42:
	li	$2,-1078001664			# 0xffffffffbfbf0000
	ori	$4,$2,0x204
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	sw	$2,24($sp)
	lw	$2,24($sp)
	ori	$2,$2,0x100
	sw	$2,24($sp)
	lw	$4,24($sp)
	li	$2,-1078001664			# 0xffffffffbfbf0000
	ori	$5,$2,0x204
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	move	$2,$0
	lw	$31,44($sp)
	addiu	$sp,$sp,48
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	gpon_dvt_transceiver_on_off_connection
	.size	gpon_dvt_transceiver_on_off_connection, .-gpon_dvt_transceiver_on_off_connection
	.rdata
	.align	2
$LC13:
	.ascii	"%s, %s PCP auto check correct counters:%d, error counter"
	.ascii	"s:%d\012\000"
	.align	2
$LC0:
	.ascii	"CDM Tx\000"
	.ascii	"CDM Rx\000"
	.ascii	"GDM Rx\000"
	.align	2
$LC1:
	.ascii	"8P0D\000"
	.ascii	"7P1D\000"
	.ascii	"6P2D\000"
	.ascii	"5P3D\000"
	.section	.text.gpon_dvt_pcp_auto_testing,"ax",@progbits
	.align	2
	.globl	gpon_dvt_pcp_auto_testing
	.set	nomips16
	.set	nomicromips
	.ent	gpon_dvt_pcp_auto_testing
	.type	gpon_dvt_pcp_auto_testing, @function
gpon_dvt_pcp_auto_testing:
	.frame	$sp,88,$31		# vars= 56, regs= 1/0, args= 24, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-88
	sw	$31,84($sp)
	lui	$2,%hi($LC0)
	addiu	$3,$2,%lo($LC0)
	lwl	$4,%lo($LC0)($2)
	move	$7,$4
	lwr	$7,3($3)
	addiu	$3,$2,%lo($LC0)
	lwl	$4,4($3)
	move	$6,$4
	lwr	$6,7($3)
	addiu	$3,$2,%lo($LC0)
	lwl	$4,8($3)
	move	$5,$4
	lwr	$5,11($3)
	addiu	$3,$2,%lo($LC0)
	lwl	$4,12($3)
	lwr	$4,15($3)
	addiu	$3,$2,%lo($LC0)
	lwl	$8,16($3)
	move	$9,$8
	lwr	$9,19($3)
	move	$3,$9
	sw	$7,32($sp)
	sw	$6,36($sp)
	sw	$5,40($sp)
	sw	$4,44($sp)
	sw	$3,48($sp)
	addiu	$2,$2,%lo($LC0)
	lbu	$2,20($2)
	sb	$2,52($sp)
	lui	$2,%hi($LC1)
	addiu	$3,$2,%lo($LC1)
	lwl	$4,%lo($LC1)($2)
	move	$6,$4
	lwr	$6,3($3)
	addiu	$3,$2,%lo($LC1)
	lwl	$4,4($3)
	move	$5,$4
	lwr	$5,7($3)
	addiu	$3,$2,%lo($LC1)
	lwl	$4,8($3)
	lwr	$4,11($3)
	addiu	$3,$2,%lo($LC1)
	lwl	$7,12($3)
	move	$8,$7
	lwr	$8,15($3)
	move	$3,$8
	addiu	$2,$2,%lo($LC1)
	lwl	$7,16($2)
	move	$9,$7
	lwr	$9,19($2)
	move	$2,$9
	sw	$6,56($sp)
	sw	$5,60($sp)
	sw	$4,64($sp)
	sw	$3,68($sp)
	sw	$2,72($sp)
	sw	$0,24($sp)
	j	$L54
	nop

$L57:
	sw	$0,28($sp)
	j	$L55
	nop

$L56:
	lw	$2,24($sp)
	sll	$3,$2,2
	lw	$2,28($sp)
	addu	$2,$3,$2
	li	$3,1			# 0x1
	sll	$2,$3,$2
	move	$4,$2
	li	$2,-1078657024			# 0xffffffffbfb50000
	ori	$5,$2,0x1514
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lw	$3,24($sp)
	addiu	$3,$3,4
	li	$4,1			# 0x1
	sll	$3,$4,$3
	seb	$4,$3
	li	$5,1			# 0x1
	lw	$3,28($sp)
	sll	$3,$5,$3
	seb	$3,$3
	or	$3,$4,$3
	seb	$3,$3
	andi	$3,$3,0x00ff
	sb	$3,280($2)
	li	$2,65536			# 0x10000
	ori	$4,$2,0xd4c0
	lui	$2,%hi(msleep)
	addiu	$2,$2,%lo(msleep)
	jalr	$2
	nop

	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	sb	$0,280($2)
	li	$4,10000			# 0x2710
	lui	$2,%hi(msleep)
	addiu	$2,$2,%lo(msleep)
	jalr	$2
	nop

	addiu	$4,$sp,32
	lw	$3,24($sp)
	move	$2,$3
	sll	$2,$2,3
	subu	$2,$2,$3
	addu	$5,$4,$2
	addiu	$4,$sp,56
	lw	$3,28($sp)
	move	$2,$3
	sll	$2,$2,2
	addu	$2,$2,$3
	addu	$6,$4,$2
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lw	$3,284($2)
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lw	$2,288($2)
	sw	$2,16($sp)
	lui	$2,%hi($LC13)
	addiu	$4,$2,%lo($LC13)
	move	$7,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	sw	$0,284($2)
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	sw	$0,288($2)
	lw	$2,28($sp)
	addiu	$2,$2,1
	sw	$2,28($sp)
$L55:
	lw	$2,28($sp)
	slt	$2,$2,4
	bne	$2,$0,$L56
	nop

	lw	$2,24($sp)
	addiu	$2,$2,1
	sw	$2,24($sp)
$L54:
	lw	$2,24($sp)
	slt	$2,$2,3
	bne	$2,$0,$L57
	nop

	move	$4,$0
	li	$2,-1078657024			# 0xffffffffbfb50000
	ori	$5,$2,0x1514
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	move	$2,$0
	lw	$31,84($sp)
	addiu	$sp,$sp,88
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	gpon_dvt_pcp_auto_testing
	.size	gpon_dvt_pcp_auto_testing, .-gpon_dvt_pcp_auto_testing
	.rdata
	.align	2
$LC14:
	.ascii	"Check the PCP CDM Tx Encode error, %d, %d\012\000"
	.align	2
$LC15:
	.ascii	"CDM\000"
	.align	2
$LC16:
	.ascii	"GDM\000"
	.align	2
$LC17:
	.ascii	"Check the PCP %s Rx decode error, %d, %d\012\000"
	.align	2
$LC2:
	.byte	0
	.byte	2
	.byte	4
	.byte	6
	.byte	8
	.byte	10
	.byte	12
	.byte	14
	.byte	0
	.byte	2
	.byte	4
	.byte	6
	.byte	9
	.byte	8
	.byte	12
	.byte	14
	.byte	0
	.byte	2
	.byte	5
	.byte	4
	.byte	9
	.byte	8
	.byte	12
	.byte	14
	.byte	1
	.byte	0
	.byte	5
	.byte	4
	.byte	9
	.byte	8
	.byte	12
	.byte	14
	.align	2
$LC3:
	.byte	0
	.byte	2
	.byte	4
	.byte	6
	.byte	8
	.byte	10
	.byte	12
	.byte	14
	.byte	0
	.byte	2
	.byte	4
	.byte	6
	.byte	8
	.byte	10
	.byte	12
	.byte	14
	.byte	0
	.byte	2
	.byte	4
	.byte	6
	.byte	10
	.byte	10
	.byte	12
	.byte	14
	.byte	0
	.byte	2
	.byte	4
	.byte	6
	.byte	8
	.byte	8
	.byte	12
	.byte	14
	.byte	0
	.byte	2
	.byte	6
	.byte	6
	.byte	10
	.byte	10
	.byte	12
	.byte	14
	.byte	0
	.byte	2
	.byte	4
	.byte	4
	.byte	8
	.byte	8
	.byte	12
	.byte	14
	.byte	2
	.byte	2
	.byte	6
	.byte	6
	.byte	10
	.byte	10
	.byte	12
	.byte	14
	.byte	0
	.byte	0
	.byte	4
	.byte	4
	.byte	8
	.byte	8
	.byte	12
	.byte	14
	.section	.text.gpon_dvt_pcp_check,"ax",@progbits
	.align	2
	.globl	gpon_dvt_pcp_check
	.set	nomips16
	.set	nomicromips
	.ent	gpon_dvt_pcp_check
	.type	gpon_dvt_pcp_check, @function
gpon_dvt_pcp_check:
	.frame	$sp,136,$31		# vars= 112, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-136
	sw	$31,132($sp)
	move	$2,$4
	sw	$5,140($sp)
	sb	$2,136($sp)
	lw	$2,140($sp)
	lw	$2,256($2)
	sw	$2,20($sp)
	lw	$2,20($sp)
	addiu	$2,$2,11
	lbu	$2,0($2)
	addiu	$2,$2,-1
	sb	$2,24($sp)
	lw	$2,20($sp)
	addiu	$2,$2,14
	lbu	$2,0($2)
	srl	$2,$2,4
	sb	$2,25($sp)
	sb	$0,16($sp)
	lui	$2,%hi($LC2)
	addiu	$3,$2,%lo($LC2)
	lwl	$4,%lo($LC2)($2)
	move	$9,$4
	lwr	$9,3($3)
	addiu	$3,$2,%lo($LC2)
	lwl	$4,4($3)
	move	$8,$4
	lwr	$8,7($3)
	addiu	$3,$2,%lo($LC2)
	lwl	$4,8($3)
	move	$7,$4
	lwr	$7,11($3)
	addiu	$3,$2,%lo($LC2)
	lwl	$4,12($3)
	move	$6,$4
	lwr	$6,15($3)
	addiu	$3,$2,%lo($LC2)
	lwl	$4,16($3)
	move	$5,$4
	lwr	$5,19($3)
	addiu	$3,$2,%lo($LC2)
	lwl	$4,20($3)
	lwr	$4,23($3)
	addiu	$3,$2,%lo($LC2)
	lwl	$10,24($3)
	move	$11,$10
	lwr	$11,27($3)
	move	$3,$11
	addiu	$2,$2,%lo($LC2)
	lwl	$10,28($2)
	move	$11,$10
	lwr	$11,31($2)
	move	$2,$11
	sw	$9,28($sp)
	sw	$8,32($sp)
	sw	$7,36($sp)
	sw	$6,40($sp)
	sw	$5,44($sp)
	sw	$4,48($sp)
	sw	$3,52($sp)
	sw	$2,56($sp)
	lui	$2,%hi($LC3)
	addiu	$4,$sp,60
	addiu	$3,$2,%lo($LC3)
	li	$2,64			# 0x40
	move	$5,$3
	move	$6,$2
	lui	$2,%hi(memcpy)
	addiu	$2,$2,%lo(memcpy)
	jalr	$2
	nop

	lbu	$2,136($sp)
	andi	$2,$2,0x10
	beq	$2,$0,$L60
	nop

	lbu	$2,24($sp)
	sltu	$2,$2,16
	beq	$2,$0,$L69
	nop

	lbu	$2,136($sp)
	andi	$2,$2,0x1
	beq	$2,$0,$L62
	nop

	lbu	$2,24($sp)
	addiu	$3,$sp,16
	addu	$2,$3,$2
	lbu	$2,44($2)
	sb	$2,16($sp)
	j	$L63
	nop

$L62:
	lbu	$2,136($sp)
	andi	$2,$2,0x2
	beq	$2,$0,$L64
	nop

	lbu	$2,24($sp)
	addiu	$3,$sp,16
	addu	$2,$3,$2
	lbu	$2,60($2)
	sb	$2,16($sp)
	j	$L63
	nop

$L64:
	lbu	$2,136($sp)
	andi	$2,$2,0x4
	beq	$2,$0,$L65
	nop

	lbu	$2,24($sp)
	addiu	$3,$sp,16
	addu	$2,$3,$2
	lbu	$2,76($2)
	sb	$2,16($sp)
	j	$L63
	nop

$L65:
	lbu	$2,136($sp)
	andi	$2,$2,0x8
	beq	$2,$0,$L63
	nop

	lbu	$2,24($sp)
	addiu	$3,$sp,16
	addu	$2,$3,$2
	lbu	$2,92($2)
	sb	$2,16($sp)
$L63:
	lbu	$3,25($sp)
	lbu	$2,16($sp)
	bne	$3,$2,$L66
	nop

	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lw	$3,284($2)
	addiu	$3,$3,1
	sw	$3,284($2)
	move	$2,$0
	j	$L79
	nop

$L66:
	lbu	$5,25($sp)
	lbu	$3,16($sp)
	lui	$2,%hi($LC14)
	addiu	$4,$2,%lo($LC14)
	move	$6,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	lhu	$2,-30816($2)
	andi	$2,$2,0x2
	beq	$2,$0,$L68
	nop

	lw	$4,140($sp)
	li	$5,32			# 0x20
	lui	$2,%hi(__dump_skb)
	addiu	$2,$2,%lo(__dump_skb)
	jalr	$2
	nop

$L68:
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lw	$3,288($2)
	addiu	$3,$3,1
	sw	$3,288($2)
	j	$L69
	nop

$L60:
	lbu	$2,136($sp)
	andi	$2,$2,0x20
	bne	$2,$0,$L70
	nop

	lbu	$2,136($sp)
	andi	$2,$2,0x40
	beq	$2,$0,$L69
	nop

$L70:
	lbu	$2,24($sp)
	sltu	$2,$2,8
	beq	$2,$0,$L69
	nop

	lbu	$2,136($sp)
	andi	$2,$2,0x1
	beq	$2,$0,$L71
	nop

	lbu	$2,24($sp)
	addiu	$3,$sp,16
	addu	$2,$3,$2
	lbu	$2,12($2)
	sb	$2,16($sp)
	j	$L72
	nop

$L71:
	lbu	$2,136($sp)
	andi	$2,$2,0x2
	beq	$2,$0,$L73
	nop

	lbu	$2,24($sp)
	addiu	$3,$sp,16
	addu	$2,$3,$2
	lbu	$2,20($2)
	sb	$2,16($sp)
	j	$L72
	nop

$L73:
	lbu	$2,136($sp)
	andi	$2,$2,0x4
	beq	$2,$0,$L74
	nop

	lbu	$2,24($sp)
	addiu	$3,$sp,16
	addu	$2,$3,$2
	lbu	$2,28($2)
	sb	$2,16($sp)
	j	$L72
	nop

$L74:
	lbu	$2,136($sp)
	andi	$2,$2,0x8
	beq	$2,$0,$L72
	nop

	lbu	$2,24($sp)
	addiu	$3,$sp,16
	addu	$2,$3,$2
	lbu	$2,36($2)
	sb	$2,16($sp)
$L72:
	lbu	$3,25($sp)
	lbu	$2,16($sp)
	bne	$3,$2,$L75
	nop

	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lw	$3,284($2)
	addiu	$3,$3,1
	sw	$3,284($2)
	move	$2,$0
	j	$L79
	nop

$L75:
	lbu	$2,136($sp)
	andi	$2,$2,0x20
	beq	$2,$0,$L76
	nop

	lui	$2,%hi($LC15)
	addiu	$2,$2,%lo($LC15)
	j	$L77
	nop

$L76:
	lui	$2,%hi($LC16)
	addiu	$2,$2,%lo($LC16)
$L77:
	lbu	$6,25($sp)
	lbu	$7,16($sp)
	lui	$3,%hi($LC17)
	addiu	$4,$3,%lo($LC17)
	move	$5,$2
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	lhu	$2,-30816($2)
	andi	$2,$2,0x2
	beq	$2,$0,$L78
	nop

	lw	$4,140($sp)
	li	$5,32			# 0x20
	lui	$2,%hi(__dump_skb)
	addiu	$2,$2,%lo(__dump_skb)
	jalr	$2
	nop

$L78:
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lw	$3,288($2)
	addiu	$3,$3,1
	sw	$3,288($2)
$L69:
	li	$2,-1			# 0xffffffffffffffff
$L79:
	lw	$31,132($sp)
	addiu	$sp,$sp,136
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	gpon_dvt_pcp_check
	.size	gpon_dvt_pcp_check, .-gpon_dvt_pcp_check
	.section	.text.gpon_dvt_eqd_adjustment,"ax",@progbits
	.align	2
	.globl	gpon_dvt_eqd_adjustment
	.set	nomips16
	.set	nomicromips
	.ent	gpon_dvt_eqd_adjustment
	.type	gpon_dvt_eqd_adjustment, @function
gpon_dvt_eqd_adjustment:
	.frame	$sp,24,$31		# vars= 0, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-24
	sw	$31,20($sp)
	sw	$4,24($sp)
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lw	$3,4($2)
	li	$2,5			# 0x5
	bne	$3,$2,$L81
	nop

	lw	$4,24($sp)
	lui	$2,%hi(ploam_eqd_adjustment)
	addiu	$2,$2,%lo(ploam_eqd_adjustment)
	jalr	$2
	nop

	move	$2,$0
	j	$L82
	nop

$L81:
	li	$2,-1			# 0xffffffffffffffff
$L82:
	lw	$31,20($sp)
	addiu	$sp,$sp,24
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	gpon_dvt_eqd_adjustment
	.size	gpon_dvt_eqd_adjustment, .-gpon_dvt_eqd_adjustment
	.rdata
	.align	2
$LC18:
	.ascii	"AES CFG aes_spf_cnt(%08x):%08x\012\000"
	.align	2
$LC19:
	.ascii	"AES SHADOW KEY0(%08x):%08x\012\000"
	.align	2
$LC20:
	.ascii	"AES SHADOW KEY1(%08x):%08x\012\000"
	.align	2
$LC21:
	.ascii	"AES SHADOW KEY2(%08x):%08x\012\000"
	.align	2
$LC22:
	.ascii	"AES SHADOW KEY3(%08x):%08x\012\000"
	.align	2
$LC23:
	.ascii	"AES ACTIVE KEY0(%08x):%08x\012\000"
	.align	2
$LC24:
	.ascii	"AES ACTIVE KEY1(%08x):%08x\012\000"
	.align	2
$LC25:
	.ascii	"AES ACTIVE KEY2(%08x):%08x\012\000"
	.align	2
$LC26:
	.ascii	"AES ACTIVE KEY3(%08x):%08x\012\000"
	.section	.text.gpon_dvt_show_aes_key,"ax",@progbits
	.align	2
	.globl	gpon_dvt_show_aes_key
	.set	nomips16
	.set	nomicromips
	.ent	gpon_dvt_show_aes_key
	.type	gpon_dvt_show_aes_key, @function
gpon_dvt_show_aes_key:
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
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$5,$2,16480
	lw	$3,24($sp)
	lui	$2,%hi($LC18)
	addiu	$4,$2,%lo($LC18)
	move	$6,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$5,$2,16500
	lw	$3,44($sp)
	lui	$2,%hi($LC19)
	addiu	$4,$2,%lo($LC19)
	move	$6,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$5,$2,16504
	lw	$3,48($sp)
	lui	$2,%hi($LC20)
	addiu	$4,$2,%lo($LC20)
	move	$6,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$5,$2,16508
	lw	$3,52($sp)
	lui	$2,%hi($LC21)
	addiu	$4,$2,%lo($LC21)
	move	$6,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$5,$2,16512
	lw	$3,56($sp)
	lui	$2,%hi($LC22)
	addiu	$4,$2,%lo($LC22)
	move	$6,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$5,$2,16484
	lw	$3,28($sp)
	lui	$2,%hi($LC23)
	addiu	$4,$2,%lo($LC23)
	move	$6,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$5,$2,16488
	lw	$3,32($sp)
	lui	$2,%hi($LC24)
	addiu	$4,$2,%lo($LC24)
	move	$6,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$5,$2,16492
	lw	$3,36($sp)
	lui	$2,%hi($LC25)
	addiu	$4,$2,%lo($LC25)
	move	$6,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$5,$2,16496
	lw	$3,40($sp)
	lui	$2,%hi($LC26)
	addiu	$4,$2,%lo($LC26)
	move	$6,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lw	$31,20($sp)
	addiu	$sp,$sp,24
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	gpon_dvt_show_aes_key
	.size	gpon_dvt_show_aes_key, .-gpon_dvt_show_aes_key
	.rdata
	.align	2
$LC27:
	.ascii	"Before setting AES key switch by SW.\012\000"
	.align	2
$LC28:
	.ascii	"Setting new AES key.\012\000"
	.align	2
$LC29:
	.ascii	"Enable AES key switch by SW.\012\000"
	.align	2
$LC30:
	.ascii	"[%lu0ms]There is something wrong with set AES key switch"
	.ascii	" by SW\012\000"
	.align	2
$LC31:
	.ascii	"After enable AES key switch by SW.\012\000"
	.section	.text.gpon_dvt_aes_key_switch_by_sw,"ax",@progbits
	.align	2
	.globl	gpon_dvt_aes_key_switch_by_sw
	.set	nomips16
	.set	nomicromips
	.ent	gpon_dvt_aes_key_switch_by_sw
	.type	gpon_dvt_aes_key_switch_by_sw, @function
gpon_dvt_aes_key_switch_by_sw:
	.frame	$sp,120,$31		# vars= 72, regs= 1/0, args= 40, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-120
	sw	$31,116($sp)
	li	$2,3072			# 0xc00
	sw	$2,40($sp)
	lui	$2,%hi($LC27)
	addiu	$4,$2,%lo($LC27)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	addiu	$2,$sp,48
	move	$4,$2
	lui	$2,%hi(gponDevGetEncryptKey)
	addiu	$2,$2,%lo(gponDevGetEncryptKey)
	jalr	$2
	nop

	lw	$6,64($sp)
	lw	$5,68($sp)
	lw	$4,72($sp)
	lw	$3,76($sp)
	lw	$2,80($sp)
	sw	$6,16($sp)
	sw	$5,20($sp)
	sw	$4,24($sp)
	sw	$3,28($sp)
	sw	$2,32($sp)
	lw	$4,48($sp)
	lw	$5,52($sp)
	lw	$6,56($sp)
	lw	$7,60($sp)
	lui	$2,%hi(gpon_dvt_show_aes_key)
	addiu	$2,$2,%lo(gpon_dvt_show_aes_key)
	jalr	$2
	nop

	lui	$2,%hi($LC28)
	addiu	$4,$2,%lo($LC28)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	addiu	$2,$sp,84
	move	$4,$2
	li	$5,24			# 0x18
	lui	$2,%hi(get_random_bytes)
	addiu	$2,$2,%lo(get_random_bytes)
	jalr	$2
	nop

	addiu	$2,$sp,84
	move	$4,$2
	lui	$2,%hi(gponDevSetEncryptKey)
	addiu	$2,$2,%lo(gponDevSetEncryptKey)
	jalr	$2
	nop

	addiu	$2,$sp,48
	move	$4,$2
	lui	$2,%hi(gponDevGetEncryptKey)
	addiu	$2,$2,%lo(gponDevGetEncryptKey)
	jalr	$2
	nop

	lw	$6,64($sp)
	lw	$5,68($sp)
	lw	$4,72($sp)
	lw	$3,76($sp)
	lw	$2,80($sp)
	sw	$6,16($sp)
	sw	$5,20($sp)
	sw	$4,24($sp)
	sw	$3,28($sp)
	sw	$2,32($sp)
	lw	$4,48($sp)
	lw	$5,52($sp)
	lw	$6,56($sp)
	lw	$7,60($sp)
	lui	$2,%hi(gpon_dvt_show_aes_key)
	addiu	$2,$2,%lo(gpon_dvt_show_aes_key)
	jalr	$2
	nop

	lui	$2,%hi($LC29)
	addiu	$4,$2,%lo($LC29)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi(gponDevSetAesKeySwitchBySw)
	addiu	$2,$2,%lo(gponDevSetAesKeySwitchBySw)
	jalr	$2
	nop

	sw	$2,44($sp)
	lw	$2,44($sp)
	bne	$2,$0,$L85
	nop

	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	lhu	$2,-30816($2)
	andi	$2,$2,0x1
	beq	$2,$0,$L86
	nop

	lui	$2,%hi(jiffies)
	lw	$3,%lo(jiffies)($2)
	lui	$2,%hi($LC30)
	addiu	$4,$2,%lo($LC30)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L86:
	li	$2,-1			# 0xffffffffffffffff
	j	$L88
	nop

$L85:
	lui	$2,%hi($LC31)
	addiu	$4,$2,%lo($LC31)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	addiu	$2,$sp,48
	move	$4,$2
	lui	$2,%hi(gponDevGetEncryptKey)
	addiu	$2,$2,%lo(gponDevGetEncryptKey)
	jalr	$2
	nop

	lw	$6,64($sp)
	lw	$5,68($sp)
	lw	$4,72($sp)
	lw	$3,76($sp)
	lw	$2,80($sp)
	sw	$6,16($sp)
	sw	$5,20($sp)
	sw	$4,24($sp)
	sw	$3,28($sp)
	sw	$2,32($sp)
	lw	$4,48($sp)
	lw	$5,52($sp)
	lw	$6,56($sp)
	lw	$7,60($sp)
	lui	$2,%hi(gpon_dvt_show_aes_key)
	addiu	$2,$2,%lo(gpon_dvt_show_aes_key)
	jalr	$2
	nop

	move	$2,$0
$L88:
	lw	$31,116($sp)
	addiu	$sp,$sp,120
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	gpon_dvt_aes_key_switch_by_sw
	.size	gpon_dvt_aes_key_switch_by_sw, .-gpon_dvt_aes_key_switch_by_sw
	.section	.text.test_gem_info,"ax",@progbits
	.align	2
	.globl	test_gem_info
	.set	nomips16
	.set	nomicromips
	.ent	test_gem_info
	.type	test_gem_info, @function
test_gem_info:
	.frame	$sp,32,$31		# vars= 8, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-32
	sw	$31,28($sp)
	sw	$0,16($sp)
	j	$L90
	nop

$L93:
	lui	$2,%hi(random32)
	addiu	$2,$2,%lo(random32)
	jalr	$2
	nop

	andi	$2,$2,0x00ff
	andi	$2,$2,0x1
	sb	$2,20($sp)
	lui	$2,%hi(random32)
	addiu	$2,$2,%lo(random32)
	jalr	$2
	nop

	andi	$2,$2,0x00ff
	andi	$2,$2,0x1
	sb	$2,21($sp)
	lui	$2,%hi(random32)
	addiu	$2,$2,%lo(random32)
	jalr	$2
	nop

	andi	$2,$2,0xffff
	andi	$2,$2,0xfff
	sh	$2,22($sp)
	lhu	$4,22($sp)
	lbu	$3,21($sp)
	lbu	$2,20($sp)
	move	$5,$3
	move	$6,$2
	lui	$2,%hi(gponDevSetGemInfo)
	addiu	$2,$2,%lo(gponDevSetGemInfo)
	jalr	$2
	nop

	beq	$2,$0,$L91
	nop

	li	$2,-1			# 0xffffffffffffffff
	j	$L92
	nop

$L91:
	lw	$2,16($sp)
	addiu	$2,$2,1
	sw	$2,16($sp)
$L90:
	lw	$2,16($sp)
	slt	$2,$2,4096
	bne	$2,$0,$L93
	nop

	move	$2,$0
$L92:
	lw	$31,28($sp)
	addiu	$sp,$sp,32
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	test_gem_info
	.size	test_gem_info, .-test_gem_info
	.section	.text.test_tcont_info,"ax",@progbits
	.align	2
	.globl	test_tcont_info
	.set	nomips16
	.set	nomicromips
	.ent	test_tcont_info
	.type	test_tcont_info, @function
test_tcont_info:
	.frame	$sp,40,$31		# vars= 16, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-40
	sw	$31,36($sp)
	sw	$0,16($sp)
	j	$L95
	nop

$L98:
	lui	$2,%hi(random32)
	addiu	$2,$2,%lo(random32)
	jalr	$2
	nop

	andi	$2,$2,0x1
	sw	$2,20($sp)
	lui	$2,%hi(random32)
	addiu	$2,$2,%lo(random32)
	jalr	$2
	nop

	andi	$2,$2,0xffff
	andi	$2,$2,0xfff
	sh	$2,24($sp)
	lhu	$2,24($sp)
	lw	$4,20($sp)
	lw	$5,16($sp)
	move	$6,$2
	lui	$2,%hi(gponDevSetTCont)
	addiu	$2,$2,%lo(gponDevSetTCont)
	jalr	$2
	nop

	beq	$2,$0,$L96
	nop

	li	$2,-1			# 0xffffffffffffffff
	j	$L97
	nop

$L96:
	lw	$2,16($sp)
	addiu	$2,$2,1
	sw	$2,16($sp)
$L95:
	lw	$2,16($sp)
	slt	$2,$2,32
	bne	$2,$0,$L98
	nop

	move	$2,$0
$L97:
	lw	$31,36($sp)
	addiu	$sp,$sp,40
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	test_tcont_info
	.size	test_tcont_info, .-test_tcont_info
	.section	.text.test_gpon_mac_reg,"ax",@progbits
	.align	2
	.globl	test_gpon_mac_reg
	.set	nomips16
	.set	nomicromips
	.ent	test_gpon_mac_reg
	.type	test_gpon_mac_reg, @function
test_gpon_mac_reg:
	.frame	$sp,24,$31		# vars= 0, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-24
	sw	$31,20($sp)
	lui	$2,%hi(test_gem_info)
	addiu	$2,$2,%lo(test_gem_info)
	jalr	$2
	nop

	beq	$2,$0,$L100
	nop

	li	$2,-1			# 0xffffffffffffffff
	j	$L101
	nop

$L100:
	lui	$2,%hi(test_tcont_info)
	addiu	$2,$2,%lo(test_tcont_info)
	jalr	$2
	nop

	beq	$2,$0,$L102
	nop

	li	$2,-1			# 0xffffffffffffffff
	j	$L101
	nop

$L102:
	lui	$2,%hi(random32)
	addiu	$2,$2,%lo(random32)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16384
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	lui	$2,%hi(random32)
	addiu	$2,$2,%lo(random32)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16388
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	lui	$2,%hi(random32)
	addiu	$2,$2,%lo(random32)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16392
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	lui	$2,%hi(random32)
	addiu	$2,$2,%lo(random32)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16396
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	lui	$2,%hi(random32)
	addiu	$2,$2,%lo(random32)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16456
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	lui	$2,%hi(random32)
	addiu	$2,$2,%lo(random32)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16480
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	lui	$2,%hi(random32)
	addiu	$2,$2,%lo(random32)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16484
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	lui	$2,%hi(random32)
	addiu	$2,$2,%lo(random32)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16488
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	lui	$2,%hi(random32)
	addiu	$2,$2,%lo(random32)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16492
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	lui	$2,%hi(random32)
	addiu	$2,$2,%lo(random32)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16496
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	lui	$2,%hi(random32)
	addiu	$2,$2,%lo(random32)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16500
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	lui	$2,%hi(random32)
	addiu	$2,$2,%lo(random32)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16504
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	lui	$2,%hi(random32)
	addiu	$2,$2,%lo(random32)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16508
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	lui	$2,%hi(random32)
	addiu	$2,$2,%lo(random32)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16512
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	lui	$2,%hi(random32)
	addiu	$2,$2,%lo(random32)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16516
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	lui	$2,%hi(random32)
	addiu	$2,$2,%lo(random32)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16528
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	lui	$2,%hi(random32)
	addiu	$2,$2,%lo(random32)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16532
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	lui	$2,%hi(random32)
	addiu	$2,$2,%lo(random32)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16536
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	lui	$2,%hi(random32)
	addiu	$2,$2,%lo(random32)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16540
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	lui	$2,%hi(random32)
	addiu	$2,$2,%lo(random32)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16544
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	lui	$2,%hi(random32)
	addiu	$2,$2,%lo(random32)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16548
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	lui	$2,%hi(random32)
	addiu	$2,$2,%lo(random32)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16552
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	lui	$2,%hi(random32)
	addiu	$2,$2,%lo(random32)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16556
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	lui	$2,%hi(random32)
	addiu	$2,$2,%lo(random32)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16560
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	lui	$2,%hi(random32)
	addiu	$2,$2,%lo(random32)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16564
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	lui	$2,%hi(random32)
	addiu	$2,$2,%lo(random32)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16568
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	lui	$2,%hi(random32)
	addiu	$2,$2,%lo(random32)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16572
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	lui	$2,%hi(random32)
	addiu	$2,$2,%lo(random32)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16592
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	lui	$2,%hi(random32)
	addiu	$2,$2,%lo(random32)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16596
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	lui	$2,%hi(random32)
	addiu	$2,$2,%lo(random32)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16600
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	lui	$2,%hi(random32)
	addiu	$2,$2,%lo(random32)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16604
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	lui	$2,%hi(random32)
	addiu	$2,$2,%lo(random32)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16608
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	lui	$2,%hi(random32)
	addiu	$2,$2,%lo(random32)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16612
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	lui	$2,%hi(random32)
	addiu	$2,$2,%lo(random32)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16640
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	lui	$2,%hi(random32)
	addiu	$2,$2,%lo(random32)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16672
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	lui	$2,%hi(random32)
	addiu	$2,$2,%lo(random32)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16676
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	lui	$2,%hi(random32)
	addiu	$2,$2,%lo(random32)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16680
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	lui	$2,%hi(random32)
	addiu	$2,$2,%lo(random32)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16684
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	lui	$2,%hi(random32)
	addiu	$2,$2,%lo(random32)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16688
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	lui	$2,%hi(random32)
	addiu	$2,$2,%lo(random32)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16692
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	lui	$2,%hi(random32)
	addiu	$2,$2,%lo(random32)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16704
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	lui	$2,%hi(random32)
	addiu	$2,$2,%lo(random32)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16708
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	lui	$2,%hi(random32)
	addiu	$2,$2,%lo(random32)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16712
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	lui	$2,%hi(random32)
	addiu	$2,$2,%lo(random32)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16736
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	lui	$2,%hi(random32)
	addiu	$2,$2,%lo(random32)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16896
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	lui	$2,%hi(random32)
	addiu	$2,$2,%lo(random32)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16900
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	lui	$2,%hi(random32)
	addiu	$2,$2,%lo(random32)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16904
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	lui	$2,%hi(random32)
	addiu	$2,$2,%lo(random32)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16908
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	lui	$2,%hi(random32)
	addiu	$2,$2,%lo(random32)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16912
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	lui	$2,%hi(random32)
	addiu	$2,$2,%lo(random32)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16916
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	lui	$2,%hi(random32)
	addiu	$2,$2,%lo(random32)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16920
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	lui	$2,%hi(random32)
	addiu	$2,$2,%lo(random32)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16924
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	lui	$2,%hi(random32)
	addiu	$2,$2,%lo(random32)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16928
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	lui	$2,%hi(random32)
	addiu	$2,$2,%lo(random32)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16932
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	lui	$2,%hi(random32)
	addiu	$2,$2,%lo(random32)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16936
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	lui	$2,%hi(random32)
	addiu	$2,$2,%lo(random32)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16940
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	lui	$2,%hi(random32)
	addiu	$2,$2,%lo(random32)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16944
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	lui	$2,%hi(random32)
	addiu	$2,$2,%lo(random32)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16948
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	lui	$2,%hi(random32)
	addiu	$2,$2,%lo(random32)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16976
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	lui	$2,%hi(random32)
	addiu	$2,$2,%lo(random32)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16992
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	lui	$2,%hi(random32)
	addiu	$2,$2,%lo(random32)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16996
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	lui	$2,%hi(random32)
	addiu	$2,$2,%lo(random32)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,17152
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	lui	$2,%hi(random32)
	addiu	$2,$2,%lo(random32)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,17156
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	lui	$2,%hi(random32)
	addiu	$2,$2,%lo(random32)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,17160
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	lui	$2,%hi(random32)
	addiu	$2,$2,%lo(random32)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,17164
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	lui	$2,%hi(random32)
	addiu	$2,$2,%lo(random32)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,17168
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	lui	$2,%hi(random32)
	addiu	$2,$2,%lo(random32)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,17200
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	lui	$2,%hi(random32)
	addiu	$2,$2,%lo(random32)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,17204
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	lui	$2,%hi(random32)
	addiu	$2,$2,%lo(random32)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,17208
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	lui	$2,%hi(random32)
	addiu	$2,$2,%lo(random32)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,17216
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	lui	$2,%hi(random32)
	addiu	$2,$2,%lo(random32)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,17220
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	lui	$2,%hi(random32)
	addiu	$2,$2,%lo(random32)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,17224
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	lui	$2,%hi(random32)
	addiu	$2,$2,%lo(random32)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,17228
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	lui	$2,%hi(random32)
	addiu	$2,$2,%lo(random32)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,17232
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	lui	$2,%hi(random32)
	addiu	$2,$2,%lo(random32)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,17236
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	lui	$2,%hi(random32)
	addiu	$2,$2,%lo(random32)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,17240
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	lui	$2,%hi(random32)
	addiu	$2,$2,%lo(random32)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,17244
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	lui	$2,%hi(random32)
	addiu	$2,$2,%lo(random32)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,17248
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	lui	$2,%hi(random32)
	addiu	$2,$2,%lo(random32)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,17252
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	lui	$2,%hi(random32)
	addiu	$2,$2,%lo(random32)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,17256
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	lui	$2,%hi(random32)
	addiu	$2,$2,%lo(random32)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,17260
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	lui	$2,%hi(random32)
	addiu	$2,$2,%lo(random32)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,17264
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	lui	$2,%hi(random32)
	addiu	$2,$2,%lo(random32)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,17268
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	lui	$2,%hi(random32)
	addiu	$2,$2,%lo(random32)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,17272
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	lui	$2,%hi(random32)
	addiu	$2,$2,%lo(random32)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,17276
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	lui	$2,%hi(random32)
	addiu	$2,$2,%lo(random32)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,17280
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	lui	$2,%hi(random32)
	addiu	$2,$2,%lo(random32)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,17284
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	lui	$2,%hi(random32)
	addiu	$2,$2,%lo(random32)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,17288
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	lui	$2,%hi(random32)
	addiu	$2,$2,%lo(random32)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,17292
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	lui	$2,%hi(random32)
	addiu	$2,$2,%lo(random32)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,17296
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	lui	$2,%hi(random32)
	addiu	$2,$2,%lo(random32)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,17300
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	lui	$2,%hi(random32)
	addiu	$2,$2,%lo(random32)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,17304
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	lui	$2,%hi(random32)
	addiu	$2,$2,%lo(random32)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,17308
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	lui	$2,%hi(random32)
	addiu	$2,$2,%lo(random32)
	jalr	$2
	nop

	ori	$3,$2,0x1
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,17312
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	lui	$2,%hi(random32)
	addiu	$2,$2,%lo(random32)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,17316
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	lui	$2,%hi(random32)
	addiu	$2,$2,%lo(random32)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,17320
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	lui	$2,%hi(random32)
	addiu	$2,$2,%lo(random32)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,17344
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	lui	$2,%hi(random32)
	addiu	$2,$2,%lo(random32)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,17348
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	li	$2,-1078984704			# 0xffffffffbfb00000
	ori	$2,$2,0x64
	lw	$3,0($2)
	li	$2,-65536			# 0xffffffffffff0000
	and	$3,$3,$2
	li	$2,524288			# 0x80000
	bne	$3,$2,$L103
	nop

	lui	$2,%hi(random32)
	addiu	$2,$2,%lo(random32)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,17352
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

$L103:
	move	$2,$0
$L101:
	lw	$31,20($sp)
	addiu	$sp,$sp,24
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	test_gpon_mac_reg
	.size	test_gpon_mac_reg, .-test_gpon_mac_reg
	.rdata
	.align	2
$LC32:
	.ascii	"before sw reset, dump CSR\012\000"
	.align	2
$LC33:
	.ascii	"before sw reset, dump hw gemport table info\012\000"
	.align	2
$LC34:
	.ascii	"before sw reset, dump hw tcont table info\012\000"
	.align	2
$LC35:
	.ascii	"after sw reset, dump CSR\012\000"
	.align	2
$LC36:
	.ascii	"after sw reset, dump hw gemport table info\012\000"
	.align	2
$LC37:
	.ascii	"after sw reset, dump hw tcont table info\012\000"
	.section	.text.gpon_dvt_sw_reset,"ax",@progbits
	.align	2
	.globl	gpon_dvt_sw_reset
	.set	nomips16
	.set	nomicromips
	.ent	gpon_dvt_sw_reset
	.type	gpon_dvt_sw_reset, @function
gpon_dvt_sw_reset:
	.frame	$sp,24,$31		# vars= 0, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-24
	sw	$31,20($sp)
	lui	$2,%hi(test_gpon_mac_reg)
	addiu	$2,$2,%lo(test_gpon_mac_reg)
	jalr	$2
	nop

	beq	$2,$0,$L105
	nop

	li	$2,-1			# 0xffffffffffffffff
	j	$L106
	nop

$L105:
	lui	$2,%hi($LC32)
	addiu	$4,$2,%lo($LC32)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi(gponDevDumpCsr)
	addiu	$2,$2,%lo(gponDevDumpCsr)
	jalr	$2
	nop

	beq	$2,$0,$L107
	nop

	li	$2,-1			# 0xffffffffffffffff
	j	$L106
	nop

$L107:
	lui	$2,%hi($LC33)
	addiu	$4,$2,%lo($LC33)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi(gponDevDumpGemInfo)
	addiu	$2,$2,%lo(gponDevDumpGemInfo)
	jalr	$2
	nop

	beq	$2,$0,$L108
	nop

	li	$2,-1			# 0xffffffffffffffff
	j	$L106
	nop

$L108:
	lui	$2,%hi($LC34)
	addiu	$4,$2,%lo($LC34)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi(gponDevDumpTcontInfo)
	addiu	$2,$2,%lo(gponDevDumpTcontInfo)
	jalr	$2
	nop

	beq	$2,$0,$L109
	nop

	li	$2,-1			# 0xffffffffffffffff
	j	$L106
	nop

$L109:
	lui	$2,%hi(gponDevSwReset)
	addiu	$2,$2,%lo(gponDevSwReset)
	jalr	$2
	nop

	beq	$2,$0,$L110
	nop

	li	$2,-1			# 0xffffffffffffffff
	j	$L106
	nop

$L110:
	lui	$2,%hi($LC35)
	addiu	$4,$2,%lo($LC35)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi(gponDevDumpCsr)
	addiu	$2,$2,%lo(gponDevDumpCsr)
	jalr	$2
	nop

	beq	$2,$0,$L111
	nop

	li	$2,-1			# 0xffffffffffffffff
	j	$L106
	nop

$L111:
	lui	$2,%hi($LC36)
	addiu	$4,$2,%lo($LC36)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi(gponDevDumpGemInfo)
	addiu	$2,$2,%lo(gponDevDumpGemInfo)
	jalr	$2
	nop

	beq	$2,$0,$L112
	nop

	li	$2,-1			# 0xffffffffffffffff
	j	$L106
	nop

$L112:
	lui	$2,%hi($LC37)
	addiu	$4,$2,%lo($LC37)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi(gponDevDumpTcontInfo)
	addiu	$2,$2,%lo(gponDevDumpTcontInfo)
	jalr	$2
	nop

	beq	$2,$0,$L113
	nop

	li	$2,-1			# 0xffffffffffffffff
	j	$L106
	nop

$L113:
	move	$2,$0
$L106:
	lw	$31,20($sp)
	addiu	$sp,$sp,24
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	gpon_dvt_sw_reset
	.size	gpon_dvt_sw_reset, .-gpon_dvt_sw_reset
	.rdata
	.align	2
$LC38:
	.ascii	"before sw resync, dump CSR\012\000"
	.align	2
$LC39:
	.ascii	"before sw resync, dump hw gemport table info\012\000"
	.align	2
$LC40:
	.ascii	"before sw resync, dump hw tcont table info\012\000"
	.align	2
$LC41:
	.ascii	"after sw resync, dump CSR\012\000"
	.align	2
$LC42:
	.ascii	"after sw resync, dump hw gemport table info\012\000"
	.align	2
$LC43:
	.ascii	"after sw resync, dump hw tcont table info\012\000"
	.section	.text.gpon_dvt_sw_resync,"ax",@progbits
	.align	2
	.globl	gpon_dvt_sw_resync
	.set	nomips16
	.set	nomicromips
	.ent	gpon_dvt_sw_resync
	.type	gpon_dvt_sw_resync, @function
gpon_dvt_sw_resync:
	.frame	$sp,24,$31		# vars= 0, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-24
	sw	$31,20($sp)
	lui	$2,%hi($LC38)
	addiu	$4,$2,%lo($LC38)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi(gponDevDumpCsr)
	addiu	$2,$2,%lo(gponDevDumpCsr)
	jalr	$2
	nop

	beq	$2,$0,$L115
	nop

	li	$2,-1			# 0xffffffffffffffff
	j	$L116
	nop

$L115:
	lui	$2,%hi($LC39)
	addiu	$4,$2,%lo($LC39)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi(gponDevDumpGemInfo)
	addiu	$2,$2,%lo(gponDevDumpGemInfo)
	jalr	$2
	nop

	beq	$2,$0,$L117
	nop

	li	$2,-1			# 0xffffffffffffffff
	j	$L116
	nop

$L117:
	lui	$2,%hi($LC40)
	addiu	$4,$2,%lo($LC40)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi(gponDevDumpTcontInfo)
	addiu	$2,$2,%lo(gponDevDumpTcontInfo)
	jalr	$2
	nop

	beq	$2,$0,$L118
	nop

	li	$2,-1			# 0xffffffffffffffff
	j	$L116
	nop

$L118:
	lui	$2,%hi(gponDevSwResync)
	addiu	$2,$2,%lo(gponDevSwResync)
	jalr	$2
	nop

	beq	$2,$0,$L119
	nop

	li	$2,-1			# 0xffffffffffffffff
	j	$L116
	nop

$L119:
	lui	$2,%hi($LC41)
	addiu	$4,$2,%lo($LC41)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi(gponDevDumpCsr)
	addiu	$2,$2,%lo(gponDevDumpCsr)
	jalr	$2
	nop

	beq	$2,$0,$L120
	nop

	li	$2,-1			# 0xffffffffffffffff
	j	$L116
	nop

$L120:
	lui	$2,%hi($LC42)
	addiu	$4,$2,%lo($LC42)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi(gponDevDumpGemInfo)
	addiu	$2,$2,%lo(gponDevDumpGemInfo)
	jalr	$2
	nop

	beq	$2,$0,$L121
	nop

	li	$2,-1			# 0xffffffffffffffff
	j	$L116
	nop

$L121:
	lui	$2,%hi($LC43)
	addiu	$4,$2,%lo($LC43)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi(gponDevDumpTcontInfo)
	addiu	$2,$2,%lo(gponDevDumpTcontInfo)
	jalr	$2
	nop

	beq	$2,$0,$L122
	nop

	li	$2,-1			# 0xffffffffffffffff
	j	$L116
	nop

$L122:
	move	$2,$0
$L116:
	lw	$31,20($sp)
	addiu	$sp,$sp,24
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	gpon_dvt_sw_resync
	.size	gpon_dvt_sw_resync, .-gpon_dvt_sw_resync
	.rdata
	.align	2
$LC44:
	.ascii	"super frame counter[4358] = %x\012\000"
	.align	2
$LC45:
	.ascii	"Current ToD Information:\012\000"
	.align	2
$LC46:
	.ascii	" Switch time       [40D0]:0x%.8x\012\000"
	.align	2
$LC47:
	.ascii	" Current second    [40DC]:%u\012\000"
	.align	2
$LC48:
	.ascii	" Current nanosecond[40E0]:%u\012\000"
	.align	2
$LC49:
	.ascii	"New ToD Information:\012\000"
	.align	2
$LC50:
	.ascii	" Switch time   [40D0]:0x%.8x\012\000"
	.align	2
$LC51:
	.ascii	" New second    [40D4]:%u\012\000"
	.align	2
$LC52:
	.ascii	" New nanosecond[40D8]:%u\012\000"
	.section	.text.gpon_dvt_tod,"ax",@progbits
	.align	2
	.globl	gpon_dvt_tod
	.set	nomips16
	.set	nomicromips
	.ent	gpon_dvt_tod
	.type	gpon_dvt_tod, @function
gpon_dvt_tod:
	.frame	$sp,40,$31		# vars= 16, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-40
	sw	$31,36($sp)
	sw	$4,40($sp)
	addiu	$2,$sp,28
	move	$4,$2
	lui	$2,%hi(gponDevGetSuperframe)
	addiu	$2,$2,%lo(gponDevGetSuperframe)
	jalr	$2
	nop

	lw	$3,28($sp)
	lui	$2,%hi($LC44)
	addiu	$4,$2,%lo($LC44)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	addiu	$2,$sp,16
	move	$4,$2
	lui	$2,%hi(gponDevGetTodSwitchTime)
	addiu	$2,$2,%lo(gponDevGetTodSwitchTime)
	jalr	$2
	nop

	addiu	$3,$sp,20
	addiu	$2,$sp,24
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(gponDevGetCurrentTod)
	addiu	$2,$2,%lo(gponDevGetCurrentTod)
	jalr	$2
	nop

	lui	$2,%hi($LC45)
	addiu	$4,$2,%lo($LC45)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lw	$3,16($sp)
	lui	$2,%hi($LC46)
	addiu	$4,$2,%lo($LC46)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lw	$3,20($sp)
	lui	$2,%hi($LC47)
	addiu	$4,$2,%lo($LC47)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lw	$3,24($sp)
	lui	$2,%hi($LC48)
	addiu	$4,$2,%lo($LC48)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lw	$4,40($sp)
	move	$3,$4
	sll	$2,$3,2
	move	$3,$2
	sll	$2,$3,5
	subu	$2,$2,$3
	addu	$2,$2,$4
	sll	$2,$2,6
	move	$3,$2
	lw	$2,28($sp)
	addu	$2,$3,$2
	sw	$2,16($sp)
	lui	$2,%hi(random32)
	addiu	$2,$2,%lo(random32)
	jalr	$2
	nop

	sw	$2,20($sp)
	lui	$2,%hi(random32)
	addiu	$2,$2,%lo(random32)
	jalr	$2
	nop

	sw	$2,24($sp)
	lw	$4,16($sp)
	lw	$3,20($sp)
	lw	$2,24($sp)
	move	$5,$3
	move	$6,$2
	lui	$2,%hi(gponDevSetNewTod)
	addiu	$2,$2,%lo(gponDevSetNewTod)
	jalr	$2
	nop

	lui	$2,%hi($LC49)
	addiu	$4,$2,%lo($LC49)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lw	$3,16($sp)
	lui	$2,%hi($LC50)
	addiu	$4,$2,%lo($LC50)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lw	$3,20($sp)
	lui	$2,%hi($LC51)
	addiu	$4,$2,%lo($LC51)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lw	$3,24($sp)
	lui	$2,%hi($LC52)
	addiu	$4,$2,%lo($LC52)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	move	$2,$0
	lw	$31,36($sp)
	addiu	$sp,$sp,40
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	gpon_dvt_tod
	.size	gpon_dvt_tod, .-gpon_dvt_tod
	.ident	"GCC: (Buildroot 2015.08.1) 4.9.3"
