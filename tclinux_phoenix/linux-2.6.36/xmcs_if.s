	.file	1 "xmcs_if.c"
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
$LC1:
	.ascii	"This function doesn't implement yet!\012\000"
#NO_APP
	.section	.text.xmcs_null,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	xmcs_null
	.type	xmcs_null, @function
xmcs_null:
	.frame	$sp,24,$31		# vars= 0, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-24
	sw	$31,20($sp)
	sw	$4,24($sp)
	lui	$2,%hi($LC1)
	addiu	$4,$2,%lo($LC1)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	move	$2,$0
	lw	$31,20($sp)
	addiu	$sp,$sp,24
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	xmcs_null
	.size	xmcs_null, .-xmcs_null
	.section	.text.xmcs_set_qos_weight_config,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	xmcs_set_qos_weight_config
	.type	xmcs_set_qos_weight_config, @function
xmcs_set_qos_weight_config:
	.frame	$sp,80,$31		# vars= 56, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-80
	sw	$31,76($sp)
	sw	$4,80($sp)
	lw	$2,80($sp)
	lw	$2,0($2)
	bne	$2,$0,$L4
	nop

	sw	$0,16($sp)
	j	$L5
	nop

$L4:
	lw	$2,80($sp)
	lw	$3,0($2)
	li	$2,1			# 0x1
	bne	$3,$2,$L6
	nop

	li	$2,1			# 0x1
	sw	$2,16($sp)
	j	$L5
	nop

$L6:
	li	$2,-22			# 0xffffffffffffffea
	j	$L7
	nop

$L5:
	lw	$2,80($sp)
	lw	$3,0($2)
	li	$2,1			# 0x1
	bne	$3,$2,$L8
	nop

	lw	$2,80($sp)
	lw	$2,4($2)
	bne	$2,$0,$L9
	nop

	sw	$0,20($sp)
	j	$L8
	nop

$L9:
	lw	$2,80($sp)
	lw	$3,4($2)
	li	$2,1			# 0x1
	bne	$3,$2,$L10
	nop

	li	$2,1			# 0x1
	sw	$2,20($sp)
	j	$L8
	nop

$L10:
	li	$2,-22			# 0xffffffffffffffea
	j	$L7
	nop

$L8:
	li	$2,16			# 0x10
	sw	$2,24($sp)
	lw	$2,16($sp)
	sw	$2,28($sp)
	lw	$2,20($sp)
	sw	$2,32($sp)
	sw	$0,36($sp)
	li	$2,11			# 0xb
	sw	$2,40($sp)
	lw	$2,28($sp)
	sw	$2,48($sp)
	lw	$2,32($sp)
	sw	$2,52($sp)
	addiu	$2,$sp,40
	lw	$4,24($sp)
	move	$5,$0
	move	$6,$2
	lui	$2,%hi(__ECNT_HOOK)
	addiu	$2,$2,%lo(__ECNT_HOOK)
	jalr	$2
	nop

	sw	$2,36($sp)
	lw	$3,36($sp)
	li	$2,-1			# 0xffffffffffffffff
	beq	$3,$2,$L11
	nop

	lw	$2,44($sp)
	j	$L14
	nop

$L11:
	li	$2,-1			# 0xffffffffffffffff
$L14:
	nop
$L7:
	lw	$31,76($sp)
	addiu	$sp,$sp,80
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	xmcs_set_qos_weight_config
	.size	xmcs_set_qos_weight_config, .-xmcs_set_qos_weight_config
	.section	.text.xmcs_get_qos_weight_config,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	xmcs_get_qos_weight_config
	.type	xmcs_get_qos_weight_config, @function
xmcs_get_qos_weight_config:
	.frame	$sp,80,$31		# vars= 56, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-80
	sw	$31,76($sp)
	sw	$4,80($sp)
	sw	$0,16($sp)
	li	$2,16			# 0x10
	sw	$2,20($sp)
	addiu	$2,$sp,36
	sw	$2,24($sp)
	addiu	$2,$sp,40
	sw	$2,28($sp)
	sw	$0,32($sp)
	li	$2,12			# 0xc
	sw	$2,44($sp)
	addiu	$2,$sp,44
	lw	$4,20($sp)
	move	$5,$0
	move	$6,$2
	lui	$2,%hi(__ECNT_HOOK)
	addiu	$2,$2,%lo(__ECNT_HOOK)
	jalr	$2
	nop

	sw	$2,32($sp)
	lw	$3,52($sp)
	lw	$2,24($sp)
	sw	$3,0($2)
	lw	$3,56($sp)
	lw	$2,28($sp)
	sw	$3,0($2)
	lw	$3,32($sp)
	li	$2,-1			# 0xffffffffffffffff
	beq	$3,$2,$L16
	nop

	lw	$2,48($sp)
	j	$L18
	nop

$L16:
	li	$2,-1			# 0xffffffffffffffff
$L18:
	sw	$2,16($sp)
	lw	$2,16($sp)
	beq	$2,$0,$L19
	nop

	lw	$2,16($sp)
	j	$L21
	nop

$L19:
	lw	$2,36($sp)
	sltu	$2,$0,$2
	andi	$2,$2,0x00ff
	move	$3,$2
	lw	$2,80($sp)
	sw	$3,0($2)
	lw	$2,40($sp)
	sltu	$2,$0,$2
	andi	$2,$2,0x00ff
	move	$3,$2
	lw	$2,80($sp)
	sw	$3,4($2)
	move	$2,$0
$L21:
	lw	$31,76($sp)
	addiu	$sp,$sp,80
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	xmcs_get_qos_weight_config
	.size	xmcs_get_qos_weight_config, .-xmcs_get_qos_weight_config
	.section	.text.xmcs_set_channel_scheduler,"ax",@progbits
	.align	2
	.globl	xmcs_set_channel_scheduler
	.set	nomips16
	.set	nomicromips
	.ent	xmcs_set_channel_scheduler
	.type	xmcs_set_channel_scheduler, @function
xmcs_set_channel_scheduler:
	.frame	$sp,88,$31		# vars= 64, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-88
	sw	$31,84($sp)
	sw	$4,88($sp)
	lw	$2,88($sp)
	lbu	$2,0($2)
	sltu	$2,$2,32
	bne	$2,$0,$L23
	nop

	li	$2,-22			# 0xffffffffffffffea
	j	$L39
	nop

$L23:
	lw	$2,88($sp)
	lbu	$2,0($2)
	sb	$2,32($sp)
	lw	$2,88($sp)
	lw	$2,4($2)
	bne	$2,$0,$L25
	nop

	sw	$0,36($sp)
	j	$L26
	nop

$L25:
	lw	$2,88($sp)
	lw	$3,4($2)
	li	$2,1			# 0x1
	bne	$3,$2,$L27
	nop

	li	$2,1			# 0x1
	sw	$2,36($sp)
	j	$L26
	nop

$L27:
	lw	$2,88($sp)
	lw	$3,4($2)
	li	$2,2			# 0x2
	bne	$3,$2,$L28
	nop

	li	$2,2			# 0x2
	sw	$2,36($sp)
	j	$L26
	nop

$L28:
	lw	$2,88($sp)
	lw	$3,4($2)
	li	$2,3			# 0x3
	bne	$3,$2,$L29
	nop

	li	$2,3			# 0x3
	sw	$2,36($sp)
	j	$L26
	nop

$L29:
	lw	$2,88($sp)
	lw	$3,4($2)
	li	$2,4			# 0x4
	bne	$3,$2,$L30
	nop

	li	$2,4			# 0x4
	sw	$2,36($sp)
	j	$L26
	nop

$L30:
	lw	$2,88($sp)
	lw	$3,4($2)
	li	$2,5			# 0x5
	bne	$3,$2,$L31
	nop

	li	$2,5			# 0x5
	sw	$2,36($sp)
	j	$L26
	nop

$L31:
	lw	$2,88($sp)
	lw	$3,4($2)
	li	$2,6			# 0x6
	bne	$3,$2,$L32
	nop

	li	$2,6			# 0x6
	sw	$2,36($sp)
	j	$L26
	nop

$L32:
	lw	$2,88($sp)
	lw	$3,4($2)
	li	$2,7			# 0x7
	bne	$3,$2,$L33
	nop

	li	$2,7			# 0x7
	sw	$2,36($sp)
	j	$L26
	nop

$L33:
	li	$2,-22			# 0xffffffffffffffea
	j	$L39
	nop

$L26:
	sw	$0,16($sp)
	j	$L34
	nop

$L35:
	lw	$3,88($sp)
	lw	$2,16($sp)
	addu	$2,$3,$2
	lbu	$3,8($2)
	lw	$2,16($sp)
	addiu	$4,$sp,16
	addu	$2,$4,$2
	sb	$3,24($2)
	lw	$2,16($sp)
	addiu	$2,$2,1
	sw	$2,16($sp)
$L34:
	lw	$2,16($sp)
	slt	$2,$2,8
	bne	$2,$0,$L35
	nop

	li	$2,16			# 0x10
	sw	$2,20($sp)
	addiu	$2,$sp,32
	sw	$2,24($sp)
	sw	$0,28($sp)
	li	$2,13			# 0xd
	sw	$2,48($sp)
	lw	$2,24($sp)
	sw	$2,64($sp)
	addiu	$2,$sp,48
	lw	$4,20($sp)
	move	$5,$0
	move	$6,$2
	lui	$2,%hi(__ECNT_HOOK)
	addiu	$2,$2,%lo(__ECNT_HOOK)
	jalr	$2
	nop

	sw	$2,28($sp)
	lw	$3,28($sp)
	li	$2,-1			# 0xffffffffffffffff
	beq	$3,$2,$L36
	nop

	lw	$2,52($sp)
	j	$L39
	nop

$L36:
	li	$2,-1			# 0xffffffffffffffff
	nop
$L39:
	lw	$31,84($sp)
	addiu	$sp,$sp,88
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	xmcs_set_channel_scheduler
	.size	xmcs_set_channel_scheduler, .-xmcs_set_channel_scheduler
	.section	__ksymtab_strings,"a",@progbits
	.type	__kstrtab_xmcs_set_channel_scheduler, @object
	.size	__kstrtab_xmcs_set_channel_scheduler, 27
__kstrtab_xmcs_set_channel_scheduler:
	.ascii	"xmcs_set_channel_scheduler\000"
	.section	__ksymtab,"a",@progbits
	.align	2
	.type	__ksymtab_xmcs_set_channel_scheduler, @object
	.size	__ksymtab_xmcs_set_channel_scheduler, 8
__ksymtab_xmcs_set_channel_scheduler:
	.word	xmcs_set_channel_scheduler
	.word	__kstrtab_xmcs_set_channel_scheduler
	.section	.text.xmcs_get_channel_scheduler,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	xmcs_get_channel_scheduler
	.type	xmcs_get_channel_scheduler, @function
xmcs_get_channel_scheduler:
	.frame	$sp,88,$31		# vars= 64, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-88
	sw	$31,84($sp)
	sw	$4,88($sp)
	sw	$0,20($sp)
	lw	$2,88($sp)
	lbu	$2,0($2)
	sltu	$2,$2,32
	bne	$2,$0,$L41
	nop

	li	$2,-22			# 0xffffffffffffffea
	j	$L57
	nop

$L41:
	lw	$2,88($sp)
	lbu	$2,0($2)
	sb	$2,36($sp)
	li	$2,16			# 0x10
	sw	$2,24($sp)
	addiu	$2,$sp,36
	sw	$2,28($sp)
	sw	$0,32($sp)
	li	$2,14			# 0xe
	sw	$2,52($sp)
	lw	$2,28($sp)
	sw	$2,68($sp)
	addiu	$2,$sp,52
	lw	$4,24($sp)
	move	$5,$0
	move	$6,$2
	lui	$2,%hi(__ECNT_HOOK)
	addiu	$2,$2,%lo(__ECNT_HOOK)
	jalr	$2
	nop

	sw	$2,32($sp)
	lw	$3,32($sp)
	li	$2,-1			# 0xffffffffffffffff
	beq	$3,$2,$L43
	nop

	lw	$2,56($sp)
	j	$L45
	nop

$L43:
	li	$2,-1			# 0xffffffffffffffff
$L45:
	sw	$2,20($sp)
	lw	$2,20($sp)
	beq	$2,$0,$L46
	nop

	lw	$2,20($sp)
	j	$L57
	nop

$L46:
	lw	$2,40($sp)
	bne	$2,$0,$L47
	nop

	lw	$2,88($sp)
	sw	$0,4($2)
	j	$L48
	nop

$L47:
	lw	$3,40($sp)
	li	$2,1			# 0x1
	bne	$3,$2,$L49
	nop

	lw	$2,88($sp)
	li	$3,1			# 0x1
	sw	$3,4($2)
	j	$L48
	nop

$L49:
	lw	$3,40($sp)
	li	$2,2			# 0x2
	bne	$3,$2,$L50
	nop

	lw	$2,88($sp)
	li	$3,2			# 0x2
	sw	$3,4($2)
	j	$L48
	nop

$L50:
	lw	$3,40($sp)
	li	$2,3			# 0x3
	bne	$3,$2,$L51
	nop

	lw	$2,88($sp)
	li	$3,3			# 0x3
	sw	$3,4($2)
	j	$L48
	nop

$L51:
	lw	$3,40($sp)
	li	$2,4			# 0x4
	bne	$3,$2,$L52
	nop

	lw	$2,88($sp)
	li	$3,4			# 0x4
	sw	$3,4($2)
	j	$L48
	nop

$L52:
	lw	$3,40($sp)
	li	$2,5			# 0x5
	bne	$3,$2,$L53
	nop

	lw	$2,88($sp)
	li	$3,5			# 0x5
	sw	$3,4($2)
	j	$L48
	nop

$L53:
	lw	$3,40($sp)
	li	$2,6			# 0x6
	bne	$3,$2,$L54
	nop

	lw	$2,88($sp)
	li	$3,6			# 0x6
	sw	$3,4($2)
	j	$L48
	nop

$L54:
	lw	$3,40($sp)
	li	$2,7			# 0x7
	bne	$3,$2,$L48
	nop

	lw	$2,88($sp)
	li	$3,7			# 0x7
	sw	$3,4($2)
$L48:
	sw	$0,16($sp)
	j	$L55
	nop

$L56:
	lw	$2,16($sp)
	addiu	$3,$sp,16
	addu	$2,$3,$2
	lbu	$3,28($2)
	lw	$4,88($sp)
	lw	$2,16($sp)
	addu	$2,$4,$2
	sb	$3,8($2)
	lw	$2,16($sp)
	addiu	$2,$2,1
	sw	$2,16($sp)
$L55:
	lw	$2,16($sp)
	slt	$2,$2,8
	bne	$2,$0,$L56
	nop

	lw	$2,20($sp)
$L57:
	lw	$31,84($sp)
	addiu	$sp,$sp,88
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	xmcs_get_channel_scheduler
	.size	xmcs_get_channel_scheduler, .-xmcs_get_channel_scheduler
	.section	.text.xmcs_set_trtcm_congest_mode,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	xmcs_set_trtcm_congest_mode
	.type	xmcs_set_trtcm_congest_mode, @function
xmcs_set_trtcm_congest_mode:
	.frame	$sp,8,$31		# vars= 8, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-8
	sw	$4,8($sp)
	lw	$2,8($sp)
	lw	$2,0($2)
	xori	$2,$2,0x1
	sltu	$2,$2,1
	andi	$2,$2,0x00ff
	sw	$2,0($sp)
	move	$2,$0
	addiu	$sp,$sp,8
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	xmcs_set_trtcm_congest_mode
	.size	xmcs_set_trtcm_congest_mode, .-xmcs_set_trtcm_congest_mode
	.section	.text.xmcs_set_dei_congest_mode,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	xmcs_set_dei_congest_mode
	.type	xmcs_set_dei_congest_mode, @function
xmcs_set_dei_congest_mode:
	.frame	$sp,80,$31		# vars= 56, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-80
	sw	$31,76($sp)
	sw	$4,80($sp)
	lw	$2,80($sp)
	lw	$2,4($2)
	xori	$2,$2,0x1
	sltu	$2,$2,1
	andi	$2,$2,0x00ff
	sw	$2,16($sp)
	li	$2,16			# 0x10
	sw	$2,20($sp)
	lw	$2,16($sp)
	sw	$2,24($sp)
	sw	$0,28($sp)
	li	$2,51			# 0x33
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
	move	$2,$0
	lw	$31,76($sp)
	addiu	$sp,$sp,80
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	xmcs_set_dei_congest_mode
	.size	xmcs_set_dei_congest_mode, .-xmcs_set_dei_congest_mode
	.section	.text.xmcs_set_threshold_congest_mode,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	xmcs_set_threshold_congest_mode
	.type	xmcs_set_threshold_congest_mode, @function
xmcs_set_threshold_congest_mode:
	.frame	$sp,32,$31		# vars= 8, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-32
	sw	$31,28($sp)
	sw	$4,32($sp)
	lw	$2,32($sp)
	lw	$3,8($2)
	li	$2,3			# 0x3
	bne	$3,$2,$L66
	nop

	lui	$2,%hi(gpWanPriv)
	lw	$3,%lo(gpWanPriv)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	lw	$2,-31788($2)
	ext	$2,$2,14,1
	andi	$2,$2,0x00ff
	move	$4,$2
	lui	$2,%hi(xpon_set_qdma_qos)
	addiu	$2,$2,%lo(xpon_set_qdma_qos)
	jalr	$2
	nop

	j	$L67
	nop

$L66:
	lw	$2,32($sp)
	lw	$2,8($2)
	xori	$2,$2,0x1
	sltu	$2,$2,1
	andi	$2,$2,0x00ff
	sw	$2,16($sp)
$L67:
	move	$2,$0
	lw	$31,28($sp)
	addiu	$sp,$sp,32
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	xmcs_set_threshold_congest_mode
	.size	xmcs_set_threshold_congest_mode, .-xmcs_set_threshold_congest_mode
	.section	.text.xmcs_set_congest_scale,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	xmcs_set_congest_scale
	.type	xmcs_set_congest_scale, @function
xmcs_set_congest_scale:
	.frame	$sp,8,$31		# vars= 8, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-8
	sw	$4,8($sp)
	lw	$2,8($sp)
	lw	$2,12($2)
	bne	$2,$0,$L70
	nop

	sw	$0,0($sp)
	j	$L71
	nop

$L70:
	lw	$2,8($sp)
	lw	$3,12($2)
	li	$2,1			# 0x1
	bne	$3,$2,$L72
	nop

	li	$2,1			# 0x1
	sw	$2,0($sp)
	j	$L71
	nop

$L72:
	lw	$2,8($sp)
	lw	$3,12($2)
	li	$2,2			# 0x2
	bne	$3,$2,$L73
	nop

	li	$2,2			# 0x2
	sw	$2,0($sp)
	j	$L71
	nop

$L73:
	lw	$2,8($sp)
	lw	$3,12($2)
	li	$2,3			# 0x3
	bne	$3,$2,$L74
	nop

	li	$2,3			# 0x3
	sw	$2,0($sp)
	j	$L71
	nop

$L74:
	li	$2,-22			# 0xffffffffffffffea
	j	$L81
	nop

$L71:
	lw	$2,8($sp)
	lw	$2,16($2)
	bne	$2,$0,$L76
	nop

	sw	$0,4($sp)
	j	$L77
	nop

$L76:
	lw	$2,8($sp)
	lw	$3,16($2)
	li	$2,1			# 0x1
	bne	$3,$2,$L78
	nop

	li	$2,1			# 0x1
	sw	$2,4($sp)
	j	$L77
	nop

$L78:
	lw	$2,8($sp)
	lw	$3,16($2)
	li	$2,2			# 0x2
	bne	$3,$2,$L79
	nop

	li	$2,2			# 0x2
	sw	$2,4($sp)
	j	$L77
	nop

$L79:
	lw	$2,8($sp)
	lw	$3,16($2)
	li	$2,3			# 0x3
	bne	$3,$2,$L80
	nop

	li	$2,3			# 0x3
	sw	$2,4($sp)
	j	$L77
	nop

$L80:
	li	$2,-22			# 0xffffffffffffffea
	j	$L81
	nop

$L77:
	move	$2,$0
$L81:
	addiu	$sp,$sp,8
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	xmcs_set_congest_scale
	.size	xmcs_set_congest_scale, .-xmcs_set_congest_scale
	.section	.text.xmcs_set_congest_threshold,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	xmcs_set_congest_threshold
	.type	xmcs_set_congest_threshold, @function
xmcs_set_congest_threshold:
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	sw	$4,0($sp)
	move	$2,$0
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	xmcs_set_congest_threshold
	.size	xmcs_set_congest_threshold, .-xmcs_set_congest_threshold
	.section	.text.xmcs_get_congest_info,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	xmcs_get_congest_info
	.type	xmcs_get_congest_info, @function
xmcs_get_congest_info:
	.frame	$sp,24,$31		# vars= 24, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-24
	sw	$4,24($sp)
	sw	$0,0($sp)
	lw	$2,0($sp)
	addiu	$sp,$sp,24
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	xmcs_get_congest_info
	.size	xmcs_get_congest_info, .-xmcs_get_congest_info
	.rdata
	.align	2
$LC0:
	.word	0
	.word	1
	.word	2
	.word	3
	.word	4
	.word	5
	.word	6
	.word	7
	.word	8
	.word	9
	.word	10
	.word	11
	.word	12
	.word	13
	.word	14
	.word	15
	.section	.text.xmcs_set_tx_trtcm_scale,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	xmcs_set_tx_trtcm_scale
	.type	xmcs_set_tx_trtcm_scale, @function
xmcs_set_tx_trtcm_scale:
	.frame	$sp,88,$31		# vars= 64, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-88
	sw	$31,84($sp)
	sw	$4,88($sp)
	lui	$2,%hi($LC0)
	addiu	$4,$sp,16
	addiu	$3,$2,%lo($LC0)
	li	$2,64			# 0x40
	move	$5,$3
	move	$6,$2
	lui	$2,%hi(memcpy)
	addiu	$2,$2,%lo(memcpy)
	jalr	$2
	nop

	lw	$2,88($sp)
	lw	$2,0($2)
	sltu	$2,$2,16
	bne	$2,$0,$L87
	nop

	li	$2,-22			# 0xffffffffffffffea
	j	$L89
	nop

$L87:
	move	$2,$0
$L89:
	lw	$31,84($sp)
	addiu	$sp,$sp,88
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	xmcs_set_tx_trtcm_scale
	.size	xmcs_set_tx_trtcm_scale, .-xmcs_set_tx_trtcm_scale
	.section	.text.xmcs_get_tx_trtcm_scale,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	xmcs_get_tx_trtcm_scale
	.type	xmcs_get_tx_trtcm_scale, @function
xmcs_get_tx_trtcm_scale:
	.frame	$sp,96,$31		# vars= 72, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-96
	sw	$31,92($sp)
	sw	$4,96($sp)
	lui	$2,%hi($LC0)
	addiu	$4,$sp,20
	addiu	$3,$2,%lo($LC0)
	li	$2,64			# 0x40
	move	$5,$3
	move	$6,$2
	lui	$2,%hi(memcpy)
	addiu	$2,$2,%lo(memcpy)
	jalr	$2
	nop

	lw	$2,16($sp)
	sltu	$2,$2,16
	bne	$2,$0,$L91
	nop

	li	$2,-14			# 0xfffffffffffffff2
	j	$L93
	nop

$L91:
	lw	$2,16($sp)
	sll	$2,$2,2
	addiu	$3,$sp,16
	addu	$2,$3,$2
	lw	$3,4($2)
	lw	$2,96($sp)
	sw	$3,0($2)
	move	$2,$0
$L93:
	lw	$31,92($sp)
	addiu	$sp,$sp,96
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	xmcs_get_tx_trtcm_scale
	.size	xmcs_get_tx_trtcm_scale, .-xmcs_get_tx_trtcm_scale
	.section	.text.xmcs_set_tx_trtcm_params,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	xmcs_set_tx_trtcm_params
	.type	xmcs_set_tx_trtcm_params, @function
xmcs_set_tx_trtcm_params:
	.frame	$sp,16,$31		# vars= 16, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-16
	sw	$4,16($sp)
	lw	$2,16($sp)
	lbu	$2,0($2)
	sltu	$2,$2,32
	bne	$2,$0,$L95
	nop

	li	$2,-22			# 0xffffffffffffffea
	j	$L97
	nop

$L95:
	lw	$2,16($sp)
	lbu	$2,0($2)
	sb	$2,0($sp)
	lw	$2,16($sp)
	lhu	$2,2($2)
	sh	$2,2($sp)
	lw	$2,16($sp)
	lhu	$2,4($2)
	sh	$2,4($sp)
	lw	$2,16($sp)
	lhu	$2,6($2)
	sh	$2,6($sp)
	lw	$2,16($sp)
	lhu	$2,8($2)
	sh	$2,8($sp)
	move	$2,$0
$L97:
	addiu	$sp,$sp,16
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	xmcs_set_tx_trtcm_params
	.size	xmcs_set_tx_trtcm_params, .-xmcs_set_tx_trtcm_params
	.section	.text.xmcs_get_tx_trtcm_params,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	xmcs_get_tx_trtcm_params
	.type	xmcs_get_tx_trtcm_params, @function
xmcs_get_tx_trtcm_params:
	.frame	$sp,16,$31		# vars= 16, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-16
	sw	$4,16($sp)
	sw	$0,0($sp)
	lw	$2,16($sp)
	lbu	$2,0($2)
	sltu	$2,$2,32
	bne	$2,$0,$L99
	nop

	li	$2,-22			# 0xffffffffffffffea
	j	$L102
	nop

$L99:
	lw	$2,16($sp)
	lbu	$2,0($2)
	sb	$2,4($sp)
	lw	$2,0($sp)
	beq	$2,$0,$L101
	nop

	lw	$2,0($sp)
	j	$L102
	nop

$L101:
	lhu	$3,6($sp)
	lw	$2,16($sp)
	sh	$3,2($2)
	lhu	$3,8($sp)
	lw	$2,16($sp)
	sh	$3,4($2)
	lhu	$3,10($sp)
	lw	$2,16($sp)
	sh	$3,6($2)
	lhu	$3,12($sp)
	lw	$2,16($sp)
	sh	$3,8($2)
	move	$2,$0
$L102:
	addiu	$sp,$sp,16
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	xmcs_get_tx_trtcm_params
	.size	xmcs_get_tx_trtcm_params, .-xmcs_get_tx_trtcm_params
	.section	.text.xmcs_set_pcp_config,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	xmcs_set_pcp_config
	.type	xmcs_set_pcp_config, @function
xmcs_set_pcp_config:
	.frame	$sp,8,$31		# vars= 8, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-8
	sw	$4,8($sp)
	lw	$2,8($sp)
	lw	$2,0($2)
	bne	$2,$0,$L104
	nop

	sw	$0,0($sp)
	j	$L105
	nop

$L104:
	lw	$2,8($sp)
	lw	$3,0($2)
	li	$2,1			# 0x1
	bne	$3,$2,$L106
	nop

	li	$2,1			# 0x1
	sw	$2,0($sp)
	j	$L105
	nop

$L106:
	lw	$2,8($sp)
	lw	$3,0($2)
	li	$2,2			# 0x2
	bne	$3,$2,$L107
	nop

	li	$2,2			# 0x2
	sw	$2,0($sp)
	j	$L105
	nop

$L107:
	li	$2,-22			# 0xffffffffffffffea
	j	$L103
	nop

$L105:
	lw	$2,8($sp)
	lw	$2,4($2)
	bne	$2,$0,$L109
	nop

	sw	$0,4($sp)
	j	$L110
	nop

$L109:
	lw	$2,8($sp)
	lw	$3,4($2)
	li	$2,1			# 0x1
	bne	$3,$2,$L111
	nop

	li	$2,1			# 0x1
	sw	$2,4($sp)
	j	$L110
	nop

$L111:
	lw	$2,8($sp)
	lw	$3,4($2)
	li	$2,2			# 0x2
	bne	$3,$2,$L112
	nop

	li	$2,2			# 0x2
	sw	$2,4($sp)
	j	$L110
	nop

$L112:
	lw	$2,8($sp)
	lw	$3,4($2)
	li	$2,3			# 0x3
	bne	$3,$2,$L113
	nop

	li	$2,4			# 0x4
	sw	$2,4($sp)
	j	$L110
	nop

$L113:
	lw	$2,8($sp)
	lw	$3,4($2)
	li	$2,4			# 0x4
	bne	$3,$2,$L114
	nop

	li	$2,8			# 0x8
	sw	$2,4($sp)
	j	$L110
	nop

$L114:
	li	$2,-22			# 0xffffffffffffffea
	j	$L103
	nop

$L110:
$L103:
	addiu	$sp,$sp,8
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	xmcs_set_pcp_config
	.size	xmcs_set_pcp_config, .-xmcs_set_pcp_config
	.section	.text.xmcs_get_pcp_config,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	xmcs_get_pcp_config
	.type	xmcs_get_pcp_config, @function
xmcs_get_pcp_config:
	.frame	$sp,8,$31		# vars= 8, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-8
	sw	$4,8($sp)
	lw	$2,8($sp)
	lw	$2,0($2)
	bne	$2,$0,$L116
	nop

	sw	$0,0($sp)
	j	$L117
	nop

$L116:
	lw	$2,8($sp)
	lw	$3,0($2)
	li	$2,1			# 0x1
	bne	$3,$2,$L118
	nop

	li	$2,1			# 0x1
	sw	$2,0($sp)
	j	$L117
	nop

$L118:
	lw	$2,8($sp)
	lw	$3,0($2)
	li	$2,2			# 0x2
	bne	$3,$2,$L119
	nop

	li	$2,2			# 0x2
	sw	$2,0($sp)
	j	$L117
	nop

$L119:
	li	$2,-22			# 0xffffffffffffffea
	j	$L120
	nop

$L117:
	lw	$2,4($sp)
	bne	$2,$0,$L121
	nop

	lw	$2,8($sp)
	sw	$0,4($2)
	j	$L122
	nop

$L121:
	lw	$3,4($sp)
	li	$2,1			# 0x1
	bne	$3,$2,$L123
	nop

	lw	$2,8($sp)
	li	$3,1			# 0x1
	sw	$3,4($2)
	j	$L122
	nop

$L123:
	lw	$3,4($sp)
	li	$2,2			# 0x2
	bne	$3,$2,$L124
	nop

	lw	$2,8($sp)
	li	$3,2			# 0x2
	sw	$3,4($2)
	j	$L122
	nop

$L124:
	lw	$3,4($sp)
	li	$2,4			# 0x4
	bne	$3,$2,$L125
	nop

	lw	$2,8($sp)
	li	$3,3			# 0x3
	sw	$3,4($2)
	j	$L122
	nop

$L125:
	lw	$3,4($sp)
	li	$2,8			# 0x8
	bne	$3,$2,$L126
	nop

	lw	$2,8($sp)
	li	$3,4			# 0x4
	sw	$3,4($2)
	j	$L122
	nop

$L126:
	li	$2,-14			# 0xfffffffffffffff2
	j	$L120
	nop

$L122:
	move	$2,$0
$L120:
	addiu	$sp,$sp,8
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	xmcs_get_pcp_config
	.size	xmcs_get_pcp_config, .-xmcs_get_pcp_config
	.rdata
	.align	2
$LC2:
	.ascii	"ECNT_HOOK_ERROR occur with cmd_id:0x%x\012\000"
	.align	2
$LC3:
	.ascii	"No such API with type:%d, cmd_id:0x%x\012\000"
	.section	.text.prepare_epon,"ax",@progbits
	.align	2
	.globl	prepare_epon
	.set	nomips16
	.set	nomicromips
	.ent	prepare_epon
	.type	prepare_epon, @function
prepare_epon:
	.frame	$sp,96,$31		# vars= 72, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-96
	sw	$31,92($sp)
	sw	$0,68($sp)
	li	$2,5			# 0x5
	sw	$2,16($sp)
	addiu	$2,$sp,68
	sw	$2,20($sp)
	sw	$0,72($sp)
	sw	$0,76($sp)
	sw	$0,80($sp)
	sw	$0,84($sp)
	li	$2,1			# 0x1
	sw	$2,72($sp)
	lw	$2,16($sp)
	sw	$2,76($sp)
	lw	$2,20($sp)
	sw	$2,84($sp)
	addiu	$2,$sp,72
	sw	$2,24($sp)
	li	$4,25			# 0x19
	move	$5,$0
	lw	$6,24($sp)
	lui	$2,%hi(__ECNT_HOOK)
	addiu	$2,$2,%lo(__ECNT_HOOK)
	jalr	$2
	nop

	move	$3,$2
	li	$2,-1			# 0xffffffffffffffff
	bne	$3,$2,$L128
	nop

	lw	$2,24($sp)
	lw	$3,4($2)
	lui	$2,%hi($LC2)
	addiu	$4,$2,%lo($LC2)
	move	$5,$3
	lui	$2,%hi(panic)
	addiu	$2,$2,%lo(panic)
	jalr	$2
	nop

$L128:
	lw	$2,24($sp)
	lw	$3,8($2)
	li	$2,-1			# 0xffffffffffffffff
	bne	$3,$2,$L129
	nop

	lw	$2,24($sp)
	lw	$5,0($2)
	lw	$2,24($sp)
	lw	$3,4($2)
	lui	$2,%hi($LC3)
	addiu	$4,$2,%lo($LC3)
	move	$6,$3
	lui	$2,%hi(panic)
	addiu	$2,$2,%lo(panic)
	jalr	$2
	nop

$L129:
	move	$4,$0
	lui	$2,%hi(select_xpon_wan)
	addiu	$2,$2,%lo(select_xpon_wan)
	jalr	$2
	nop

	move	$4,$0
	lui	$2,%hi(XPON_PHY_SET_MODE)
	addiu	$2,$2,%lo(XPON_PHY_SET_MODE)
	jalr	$2
	nop

	li	$2,1			# 0x1
	sw	$2,48($sp)
	li	$2,5			# 0x5
	sw	$2,28($sp)
	addiu	$2,$sp,48
	sw	$2,32($sp)
	sw	$0,52($sp)
	sw	$0,56($sp)
	sw	$0,60($sp)
	sw	$0,64($sp)
	li	$2,1			# 0x1
	sw	$2,52($sp)
	lw	$2,28($sp)
	sw	$2,56($sp)
	lw	$2,32($sp)
	sw	$2,64($sp)
	addiu	$2,$sp,52
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
	bne	$3,$2,$L133
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

$L133:
	lw	$2,36($sp)
	lw	$3,8($2)
	li	$2,-1			# 0xffffffffffffffff
	bne	$3,$2,$L134
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

$L134:
	li	$2,-1078657024			# 0xffffffffbfb50000
	ori	$2,$2,0x9640
	sw	$2,40($sp)
	sw	$0,44($sp)
	lw	$2,40($sp)
	lw	$3,44($sp)
	sw	$3,0($2)
	lw	$31,92($sp)
	addiu	$sp,$sp,96
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	prepare_epon
	.size	prepare_epon, .-prepare_epon
	.rdata
	.align	2
$LC4:
	.ascii	"%s:%d mode=%d\012\000"
	.align	2
$LC5:
	.ascii	"%s:%d gpPhyData->working_mode: %d\012\000"
	.align	2
$LC6:
	.ascii	"Can't recognize phy working mode: %d\012\000"
	.section	.text.xmcs_set_connection_start,"ax",@progbits
	.align	2
	.globl	xmcs_set_connection_start
	.set	nomips16
	.set	nomicromips
	.ent	xmcs_set_connection_start
	.type	xmcs_set_connection_start, @function
xmcs_set_connection_start:
	.frame	$sp,248,$31		# vars= 224, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-248
	sw	$31,244($sp)
	sw	$4,248($sp)
	lui	$2,%hi(xpon_mac_print_open)
	lw	$2,%lo(xpon_mac_print_open)($2)
	beq	$2,$0,$L139
	nop

	lui	$2,%hi($LC4)
	addiu	$4,$2,%lo($LC4)
	lui	$2,%hi(__func__.33634)
	addiu	$5,$2,%lo(__func__.33634)
	li	$6,883			# 0x373
	lw	$7,248($sp)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L139:
	lw	$3,248($sp)
	li	$2,1			# 0x1
	bne	$3,$2,$L140
	nop

	li	$2,1			# 0x1
	sw	$2,16($sp)
	lw	$2,16($sp)
	sw	$2,20($sp)
	sw	$0,24($sp)
	sw	$0,208($sp)
	sw	$0,212($sp)
	sw	$0,216($sp)
	sw	$0,220($sp)
	li	$2,1			# 0x1
	sw	$2,208($sp)
	lw	$2,20($sp)
	sw	$2,212($sp)
	lw	$2,24($sp)
	sw	$2,220($sp)
	addiu	$2,$sp,208
	sw	$2,28($sp)
	li	$4,25			# 0x19
	move	$5,$0
	lw	$6,28($sp)
	lui	$2,%hi(__ECNT_HOOK)
	addiu	$2,$2,%lo(__ECNT_HOOK)
	jalr	$2
	nop

	move	$3,$2
	li	$2,-1			# 0xffffffffffffffff
	bne	$3,$2,$L141
	nop

	lw	$2,28($sp)
	lw	$3,4($2)
	lui	$2,%hi($LC2)
	addiu	$4,$2,%lo($LC2)
	move	$5,$3
	lui	$2,%hi(panic)
	addiu	$2,$2,%lo(panic)
	jalr	$2
	nop

$L141:
	lw	$2,28($sp)
	lw	$3,8($2)
	li	$2,-1			# 0xffffffffffffffff
	bne	$3,$2,$L142
	nop

	lw	$2,28($sp)
	lw	$5,0($2)
	lw	$2,28($sp)
	lw	$3,4($2)
	lui	$2,%hi($LC3)
	addiu	$4,$2,%lo($LC3)
	move	$6,$3
	lui	$2,%hi(panic)
	addiu	$2,$2,%lo(panic)
	jalr	$2
	nop

$L142:
	lui	$2,%hi(gpPonSysData)
	lw	$2,%lo(gpPonSysData)($2)
	sw	$0,8($2)
	lui	$2,%hi(gpPhyData)
	lw	$2,%lo(gpPhyData)($2)
	lw	$3,60($2)
	li	$2,100663296			# 0x6000000
	and	$3,$3,$2
	li	$2,33554432			# 0x2000000
	bne	$3,$2,$L146
	nop

	lui	$2,%hi(gpMcsPriv)
	lw	$2,%lo(gpMcsPriv)($2)
	lw	$3,404($2)
	li	$2,-2147483648			# 0xffffffff80000000
	and	$2,$3,$2
	bne	$2,$0,$L147
	nop

	li	$2,32769			# 0x8001
	sw	$2,32($sp)
	lw	$2,32($sp)
	sw	$2,36($sp)
	sw	$0,40($sp)
	sw	$0,192($sp)
	sw	$0,196($sp)
	sw	$0,200($sp)
	sw	$0,204($sp)
	sw	$0,192($sp)
	lw	$2,36($sp)
	sw	$2,196($sp)
	lw	$2,40($sp)
	sw	$2,204($sp)
	addiu	$2,$sp,192
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
	bne	$3,$2,$L148
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

$L148:
	lw	$2,44($sp)
	lw	$3,8($2)
	li	$2,-1			# 0xffffffffffffffff
	bne	$3,$2,$L149
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

$L149:
	lw	$2,44($sp)
	lw	$3,8($2)
	li	$2,1			# 0x1
	bne	$3,$2,$L146
	nop

$L147:
	li	$4,1			# 0x1
	li	$5,10			# 0xa
	li	$6,1			# 0x1
	lui	$2,%hi(xmcs_report_event)
	addiu	$2,$2,%lo(xmcs_report_event)
	jalr	$2
	nop

$L146:
	lui	$2,%hi(gpPhyData)
	lw	$2,%lo(gpPhyData)($2)
	addiu	$4,$2,64
	lui	$2,%hi(gpPhyData)
	lw	$2,%lo(gpPhyData)($2)
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

	lui	$2,%hi(gpPhyData)
	lw	$2,%lo(gpPhyData)($2)
	lw	$3,60($2)
	li	$2,100663296			# 0x6000000
	and	$3,$3,$2
	li	$2,67108864			# 0x4000000
	bne	$3,$2,$L153
	nop

	lui	$2,%hi(gpPhyData)
	lw	$3,%lo(gpPhyData)($2)
	lhu	$2,60($3)
	ins	$2,$0,11,2
	sh	$2,60($3)
	lui	$2,%hi(gpPhyData)
	lw	$2,%lo(gpPhyData)($2)
	addiu	$4,$2,4
	lui	$2,%hi(gpPhyData)
	lw	$2,%lo(gpPhyData)($2)
	lw	$2,24($2)
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

	j	$L165
	nop

$L153:
	lui	$2,%hi(xpon_mac_print_open)
	lw	$2,%lo(xpon_mac_print_open)($2)
	beq	$2,$0,$L155
	nop

	lui	$2,%hi(gpPhyData)
	lw	$2,%lo(gpPhyData)($2)
	lw	$2,60($2)
	ext	$2,$2,25,2
	andi	$2,$2,0x00ff
	move	$3,$2
	lui	$2,%hi($LC5)
	addiu	$4,$2,%lo($LC5)
	lui	$2,%hi(__func__.33634)
	addiu	$5,$2,%lo(__func__.33634)
	li	$6,900			# 0x384
	move	$7,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L155:
	lui	$2,%hi(gpPhyData)
	lw	$2,%lo(gpPhyData)($2)
	lw	$2,60($2)
	ext	$2,$2,25,2
	andi	$2,$2,0x00ff
	beq	$2,$0,$L157
	nop

	li	$3,1			# 0x1
	bne	$2,$3,$L197
	nop

	lui	$2,%hi(prepare_gpon)
	addiu	$2,$2,%lo(prepare_gpon)
	jalr	$2
	nop

	j	$L159
	nop

$L157:
	lui	$2,%hi(prepare_epon)
	addiu	$2,$2,%lo(prepare_epon)
	jalr	$2
	nop

	j	$L159
	nop

$L197:
	lui	$2,%hi(dump_stack)
	addiu	$2,$2,%lo(dump_stack)
	jalr	$2
	nop

	lui	$2,%hi(gpPhyData)
	lw	$2,%lo(gpPhyData)($2)
	lw	$2,60($2)
	ext	$2,$2,25,2
	andi	$2,$2,0x00ff
	move	$3,$2
	lui	$2,%hi($LC6)
	addiu	$4,$2,%lo($LC6)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	nop
$L159:
	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	lw	$3,-30416($2)
	li	$2,1			# 0x1
	bne	$3,$2,$L165
	nop

	li	$2,1			# 0x1
	sw	$2,172($sp)
	li	$2,19			# 0x13
	sw	$2,48($sp)
	addiu	$2,$sp,172
	sw	$2,52($sp)
	sw	$0,176($sp)
	sw	$0,180($sp)
	sw	$0,184($sp)
	sw	$0,188($sp)
	li	$2,1			# 0x1
	sw	$2,176($sp)
	lw	$2,48($sp)
	sw	$2,180($sp)
	lw	$2,52($sp)
	sw	$2,188($sp)
	addiu	$2,$sp,176
	sw	$2,56($sp)
	li	$4,25			# 0x19
	move	$5,$0
	lw	$6,56($sp)
	lui	$2,%hi(__ECNT_HOOK)
	addiu	$2,$2,%lo(__ECNT_HOOK)
	jalr	$2
	nop

	move	$3,$2
	li	$2,-1			# 0xffffffffffffffff
	bne	$3,$2,$L160
	nop

	lw	$2,56($sp)
	lw	$3,4($2)
	lui	$2,%hi($LC2)
	addiu	$4,$2,%lo($LC2)
	move	$5,$3
	lui	$2,%hi(panic)
	addiu	$2,$2,%lo(panic)
	jalr	$2
	nop

$L160:
	lw	$2,56($sp)
	lw	$3,8($2)
	li	$2,-1			# 0xffffffffffffffff
	bne	$3,$2,$L165
	nop

	lw	$2,56($sp)
	lw	$5,0($2)
	lw	$2,56($sp)
	lw	$3,4($2)
	lui	$2,%hi($LC3)
	addiu	$4,$2,%lo($LC3)
	move	$6,$3
	lui	$2,%hi(panic)
	addiu	$2,$2,%lo(panic)
	jalr	$2
	nop

$L140:
	lw	$2,248($sp)
	beq	$2,$0,$L166
	nop

	lw	$3,248($sp)
	li	$2,2			# 0x2
	bne	$3,$2,$L165
	nop

$L166:
	lui	$2,%hi(gpPhyData)
	lw	$2,%lo(gpPhyData)($2)
	lw	$3,60($2)
	li	$2,402653184			# 0x18000000
	and	$3,$3,$2
	li	$2,134217728			# 0x8000000
	bne	$3,$2,$L167
	nop

	lui	$2,%hi(gpPhyData)
	lw	$3,%lo(gpPhyData)($2)
	lhu	$2,60($3)
	li	$4,3			# 0x3
	ins	$2,$4,11,2
	sh	$2,60($3)
$L167:
	li	$2,2			# 0x2
	sw	$2,60($sp)
	lw	$2,60($sp)
	sw	$2,64($sp)
	sw	$0,68($sp)
	sw	$0,224($sp)
	sw	$0,228($sp)
	sw	$0,232($sp)
	sw	$0,236($sp)
	li	$2,1			# 0x1
	sw	$2,224($sp)
	lw	$2,64($sp)
	sw	$2,228($sp)
	lw	$2,68($sp)
	sw	$2,236($sp)
	addiu	$2,$sp,224
	sw	$2,72($sp)
	li	$4,25			# 0x19
	move	$5,$0
	lw	$6,72($sp)
	lui	$2,%hi(__ECNT_HOOK)
	addiu	$2,$2,%lo(__ECNT_HOOK)
	jalr	$2
	nop

	move	$3,$2
	li	$2,-1			# 0xffffffffffffffff
	bne	$3,$2,$L168
	nop

	lw	$2,72($sp)
	lw	$3,4($2)
	lui	$2,%hi($LC2)
	addiu	$4,$2,%lo($LC2)
	move	$5,$3
	lui	$2,%hi(panic)
	addiu	$2,$2,%lo(panic)
	jalr	$2
	nop

$L168:
	lw	$2,72($sp)
	lw	$3,8($2)
	li	$2,-1			# 0xffffffffffffffff
	bne	$3,$2,$L169
	nop

	lw	$2,72($sp)
	lw	$5,0($2)
	lw	$2,72($sp)
	lw	$3,4($2)
	lui	$2,%hi($LC3)
	addiu	$4,$2,%lo($LC3)
	move	$6,$3
	lui	$2,%hi(panic)
	addiu	$2,$2,%lo(panic)
	jalr	$2
	nop

$L169:
	lui	$2,%hi(gpPonSysData)
	lw	$2,%lo(gpPonSysData)($2)
	li	$3,1			# 0x1
	sw	$3,8($2)
	lui	$2,%hi(gpPhyData)
	lw	$3,%lo(gpPhyData)($2)
	lhu	$2,60($3)
	ins	$2,$0,5,3
	sh	$2,60($3)
	lui	$2,%hi(gpPhyData)
	lw	$2,%lo(gpPhyData)($2)
	lw	$2,60($2)
	ext	$2,$2,25,2
	andi	$2,$2,0x00ff
	beq	$2,$0,$L174
	nop

	li	$3,1			# 0x1
	beq	$2,$3,$L175
	nop

	j	$L177
	nop

$L175:
	move	$4,$0
	lui	$2,%hi(gpon_disable_with_option)
	addiu	$2,$2,%lo(gpon_disable_with_option)
	jalr	$2
	nop

	j	$L177
	nop

$L174:
	lui	$2,%hi(eponStop)
	addiu	$2,$2,%lo(eponStop)
	jalr	$2
	nop

	nop
$L177:
	lui	$2,%hi(xpon_stop_timer)
	addiu	$2,$2,%lo(xpon_stop_timer)
	jalr	$2
	nop

$L165:
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lw	$3,464($2)
	li	$2,536870912			# 0x20000000
	and	$2,$3,$2
	beq	$2,$0,$L178
	nop

	sw	$0,152($sp)
	li	$2,12			# 0xc
	sw	$2,76($sp)
	addiu	$2,$sp,152
	sw	$2,80($sp)
	sw	$0,156($sp)
	sw	$0,160($sp)
	sw	$0,164($sp)
	sw	$0,168($sp)
	li	$2,1			# 0x1
	sw	$2,156($sp)
	lw	$2,76($sp)
	sw	$2,160($sp)
	lw	$2,80($sp)
	sw	$2,168($sp)
	addiu	$2,$sp,156
	sw	$2,84($sp)
	li	$4,25			# 0x19
	move	$5,$0
	lw	$6,84($sp)
	lui	$2,%hi(__ECNT_HOOK)
	addiu	$2,$2,%lo(__ECNT_HOOK)
	jalr	$2
	nop

	move	$3,$2
	li	$2,-1			# 0xffffffffffffffff
	bne	$3,$2,$L179
	nop

	lw	$2,84($sp)
	lw	$3,4($2)
	lui	$2,%hi($LC2)
	addiu	$4,$2,%lo($LC2)
	move	$5,$3
	lui	$2,%hi(panic)
	addiu	$2,$2,%lo(panic)
	jalr	$2
	nop

$L179:
	lw	$2,84($sp)
	lw	$3,8($2)
	li	$2,-1			# 0xffffffffffffffff
	bne	$3,$2,$L180
	nop

	lw	$2,84($sp)
	lw	$5,0($2)
	lw	$2,84($sp)
	lw	$3,4($2)
	lui	$2,%hi($LC3)
	addiu	$4,$2,%lo($LC3)
	move	$6,$3
	lui	$2,%hi(panic)
	addiu	$2,$2,%lo(panic)
	jalr	$2
	nop

$L180:
	li	$4,7			# 0x7
	lui	$2,%hi(gpon_act_change_gpon_state)
	addiu	$2,$2,%lo(gpon_act_change_gpon_state)
	jalr	$2
	nop

	li	$4,3			# 0x3
	lui	$2,%hi(gpon_record_emergence_info)
	addiu	$2,$2,%lo(gpon_record_emergence_info)
	jalr	$2
	nop

	j	$L184
	nop

$L178:
	lw	$3,248($sp)
	li	$2,2			# 0x2
	bne	$3,$2,$L185
	nop

	sw	$0,132($sp)
	li	$2,12			# 0xc
	sw	$2,88($sp)
	addiu	$2,$sp,132
	sw	$2,92($sp)
	sw	$0,136($sp)
	sw	$0,140($sp)
	sw	$0,144($sp)
	sw	$0,148($sp)
	li	$2,1			# 0x1
	sw	$2,136($sp)
	lw	$2,88($sp)
	sw	$2,140($sp)
	lw	$2,92($sp)
	sw	$2,148($sp)
	addiu	$2,$sp,136
	sw	$2,96($sp)
	li	$4,25			# 0x19
	move	$5,$0
	lw	$6,96($sp)
	lui	$2,%hi(__ECNT_HOOK)
	addiu	$2,$2,%lo(__ECNT_HOOK)
	jalr	$2
	nop

	move	$3,$2
	li	$2,-1			# 0xffffffffffffffff
	bne	$3,$2,$L186
	nop

	lw	$2,96($sp)
	lw	$3,4($2)
	lui	$2,%hi($LC2)
	addiu	$4,$2,%lo($LC2)
	move	$5,$3
	lui	$2,%hi(panic)
	addiu	$2,$2,%lo(panic)
	jalr	$2
	nop

$L186:
	lw	$2,96($sp)
	lw	$3,8($2)
	li	$2,-1			# 0xffffffffffffffff
	bne	$3,$2,$L184
	nop

	lw	$2,96($sp)
	lw	$5,0($2)
	lw	$2,96($sp)
	lw	$3,4($2)
	lui	$2,%hi($LC3)
	addiu	$4,$2,%lo($LC3)
	move	$6,$3
	lui	$2,%hi(panic)
	addiu	$2,$2,%lo(panic)
	jalr	$2
	nop

$L185:
	li	$2,1			# 0x1
	sw	$2,112($sp)
	li	$2,12			# 0xc
	sw	$2,100($sp)
	addiu	$2,$sp,112
	sw	$2,104($sp)
	sw	$0,116($sp)
	sw	$0,120($sp)
	sw	$0,124($sp)
	sw	$0,128($sp)
	li	$2,1			# 0x1
	sw	$2,116($sp)
	lw	$2,100($sp)
	sw	$2,120($sp)
	lw	$2,104($sp)
	sw	$2,128($sp)
	addiu	$2,$sp,116
	sw	$2,108($sp)
	li	$4,25			# 0x19
	move	$5,$0
	lw	$6,108($sp)
	lui	$2,%hi(__ECNT_HOOK)
	addiu	$2,$2,%lo(__ECNT_HOOK)
	jalr	$2
	nop

	move	$3,$2
	li	$2,-1			# 0xffffffffffffffff
	bne	$3,$2,$L191
	nop

	lw	$2,108($sp)
	lw	$3,4($2)
	lui	$2,%hi($LC2)
	addiu	$4,$2,%lo($LC2)
	move	$5,$3
	lui	$2,%hi(panic)
	addiu	$2,$2,%lo(panic)
	jalr	$2
	nop

$L191:
	lw	$2,108($sp)
	lw	$3,8($2)
	li	$2,-1			# 0xffffffffffffffff
	bne	$3,$2,$L184
	nop

	lw	$2,108($sp)
	lw	$5,0($2)
	lw	$2,108($sp)
	lw	$3,4($2)
	lui	$2,%hi($LC3)
	addiu	$4,$2,%lo($LC3)
	move	$6,$3
	lui	$2,%hi(panic)
	addiu	$2,$2,%lo(panic)
	jalr	$2
	nop

$L184:
	move	$2,$0
	lw	$31,244($sp)
	addiu	$sp,$sp,248
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	xmcs_set_connection_start
	.size	xmcs_set_connection_start, .-xmcs_set_connection_start
	.section	.text.xmcs_get_onu_type,"ax",@progbits
	.align	2
	.globl	xmcs_get_onu_type
	.set	nomips16
	.set	nomicromips
	.ent	xmcs_get_onu_type
	.type	xmcs_get_onu_type, @function
xmcs_get_onu_type:
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	sw	$4,0($sp)
	lw	$2,0($sp)
	bne	$2,$0,$L199
	nop

	li	$2,-22			# 0xffffffffffffffea
	j	$L200
	nop

$L199:
	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	lw	$3,-30824($2)
	li	$2,2			# 0x2
	bne	$3,$2,$L201
	nop

	lw	$2,0($sp)
	li	$3,2			# 0x2
	sw	$3,0($2)
	j	$L202
	nop

$L201:
	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	lw	$3,-30824($2)
	li	$2,1			# 0x1
	bne	$3,$2,$L203
	nop

	lw	$2,0($sp)
	li	$3,1			# 0x1
	sw	$3,0($2)
	j	$L202
	nop

$L203:
	lw	$2,0($sp)
	sw	$0,0($2)
$L202:
	move	$2,$0
$L200:
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	xmcs_get_onu_type
	.size	xmcs_get_onu_type, .-xmcs_get_onu_type
	.section	__ksymtab_strings
	.type	__kstrtab_xmcs_get_onu_type, @object
	.size	__kstrtab_xmcs_get_onu_type, 18
__kstrtab_xmcs_get_onu_type:
	.ascii	"xmcs_get_onu_type\000"
	.section	__ksymtab
	.align	2
	.type	__ksymtab_xmcs_get_onu_type, @object
	.size	__ksymtab_xmcs_get_onu_type, 8
__ksymtab_xmcs_get_onu_type:
	.word	xmcs_get_onu_type
	.word	__kstrtab_xmcs_get_onu_type
	.rdata
	.align	2
$LC7:
	.ascii	"%s:%d detection: %d\012\000"
	.section	.text.xmcs_set_link_detection,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	xmcs_set_link_detection
	.type	xmcs_set_link_detection, @function
xmcs_set_link_detection:
	.frame	$sp,24,$31		# vars= 0, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-24
	sw	$31,20($sp)
	sw	$4,24($sp)
	lui	$2,%hi(gpPonSysData)
	lw	$2,%lo(gpPonSysData)($2)
	lw	$2,8($2)
	bne	$2,$0,$L205
	nop

	move	$2,$0
	j	$L206
	nop

$L205:
	lui	$2,%hi(xpon_mac_print_open)
	lw	$2,%lo(xpon_mac_print_open)($2)
	beq	$2,$0,$L207
	nop

	lui	$2,%hi($LC7)
	addiu	$4,$2,%lo($LC7)
	lui	$2,%hi(__func__.33655)
	addiu	$5,$2,%lo(__func__.33655)
	li	$6,1004			# 0x3ec
	lw	$7,24($sp)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L207:
	lw	$2,24($sp)
	li	$3,1			# 0x1
	beq	$2,$3,$L209
	nop

	sltu	$3,$2,1
	bne	$3,$0,$L210
	nop

	li	$3,2			# 0x2
	beq	$2,$3,$L211
	nop

	j	$L213
	nop

$L209:
	lui	$2,%hi(gpPhyData)
	lw	$3,%lo(gpPhyData)($2)
	lhu	$2,60($3)
	li	$4,1			# 0x1
	ins	$2,$4,15,1
	sh	$2,60($3)
	lui	$2,%hi(gpPhyData)
	lw	$3,%lo(gpPhyData)($2)
	lhu	$2,60($3)
	li	$4,1			# 0x1
	ins	$2,$4,9,2
	sh	$2,60($3)
	j	$L212
	nop

$L211:
	lui	$2,%hi(gpPhyData)
	lw	$3,%lo(gpPhyData)($2)
	lhu	$2,60($3)
	li	$4,1			# 0x1
	ins	$2,$4,15,1
	sh	$2,60($3)
	lui	$2,%hi(gpPhyData)
	lw	$3,%lo(gpPhyData)($2)
	lhu	$2,60($3)
	ins	$2,$0,9,2
	sh	$2,60($3)
	j	$L212
	nop

$L210:
	lui	$2,%hi(gpPhyData)
	lw	$3,%lo(gpPhyData)($2)
	lhu	$2,60($3)
	ins	$2,$0,15,1
	sh	$2,60($3)
	j	$L212
	nop

$L213:
	li	$2,-22			# 0xffffffffffffffea
	j	$L206
	nop

$L212:
	move	$2,$0
$L206:
	lw	$31,20($sp)
	addiu	$sp,$sp,24
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	xmcs_set_link_detection
	.size	xmcs_set_link_detection, .-xmcs_set_link_detection
	.section	.text.xmcs_get_wan_link_status,"ax",@progbits
	.align	2
	.globl	xmcs_get_wan_link_status
	.set	nomips16
	.set	nomicromips
	.ent	xmcs_get_wan_link_status
	.type	xmcs_get_wan_link_status, @function
xmcs_get_wan_link_status:
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	sw	$4,0($sp)
	lui	$2,%hi(gpPonSysData)
	lw	$2,%lo(gpPonSysData)($2)
	lw	$2,8($2)
	sltu	$2,$2,1
	andi	$2,$2,0x00ff
	move	$3,$2
	lw	$2,0($sp)
	sw	$3,0($2)
	lui	$2,%hi(gpPhyData)
	lw	$2,%lo(gpPhyData)($2)
	lw	$3,60($2)
	li	$2,-2147483648			# 0xffffffff80000000
	and	$2,$3,$2
	beq	$2,$0,$L215
	nop

	lui	$2,%hi(gpPhyData)
	lw	$2,%lo(gpPhyData)($2)
	lw	$3,60($2)
	li	$2,100663296			# 0x6000000
	and	$3,$3,$2
	li	$2,33554432			# 0x2000000
	bne	$3,$2,$L216
	nop

	li	$2,1			# 0x1
	j	$L217
	nop

$L216:
	li	$2,2			# 0x2
$L217:
	lw	$3,0($sp)
	sw	$2,4($3)
	j	$L218
	nop

$L215:
	lw	$2,0($sp)
	sw	$0,4($2)
$L218:
	lui	$2,%hi(gpPonSysData)
	lw	$2,%lo(gpPonSysData)($2)
	lw	$3,0($2)
	li	$2,1			# 0x1
	bne	$3,$2,$L219
	nop

	lw	$2,0($sp)
	li	$3,1			# 0x1
	sw	$3,8($2)
	j	$L220
	nop

$L219:
	lui	$2,%hi(gpPonSysData)
	lw	$2,%lo(gpPonSysData)($2)
	lw	$3,0($2)
	li	$2,2			# 0x2
	bne	$3,$2,$L221
	nop

	lw	$2,0($sp)
	li	$3,2			# 0x2
	sw	$3,8($2)
	j	$L220
	nop

$L221:
	lw	$2,0($sp)
	sw	$0,8($2)
$L220:
	move	$2,$0
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	xmcs_get_wan_link_status
	.size	xmcs_get_wan_link_status, .-xmcs_get_wan_link_status
	.section	__ksymtab_strings
	.type	__kstrtab_xmcs_get_wan_link_status, @object
	.size	__kstrtab_xmcs_get_wan_link_status, 25
__kstrtab_xmcs_get_wan_link_status:
	.ascii	"xmcs_get_wan_link_status\000"
	.section	__ksymtab
	.align	2
	.type	__ksymtab_xmcs_get_wan_link_status, @object
	.size	__ksymtab_xmcs_get_wan_link_status, 8
__ksymtab_xmcs_get_wan_link_status:
	.word	xmcs_get_wan_link_status
	.word	__kstrtab_xmcs_get_wan_link_status
	.section	.text.xmcs_set_wan_cnt_stats,"ax",@progbits
	.align	2
	.globl	xmcs_set_wan_cnt_stats
	.set	nomips16
	.set	nomicromips
	.ent	xmcs_set_wan_cnt_stats
	.type	xmcs_set_wan_cnt_stats, @function
xmcs_set_wan_cnt_stats:
	.frame	$sp,184,$31		# vars= 160, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-184
	sw	$31,180($sp)
	sw	$4,184($sp)
	lw	$2,184($sp)
	andi	$2,$2,0x1
	beq	$2,$0,$L224
	nop

	li	$2,1			# 0x1
	sw	$2,16($sp)
	sw	$0,20($sp)
	sw	$0,24($sp)
	li	$2,43			# 0x2b
	sw	$2,40($sp)
	lw	$2,16($sp)
	sw	$2,44($sp)
	lw	$2,20($sp)
	sw	$2,60($sp)
	addiu	$2,$sp,40
	li	$4,18			# 0x12
	move	$5,$0
	move	$6,$2
	lui	$2,%hi(__ECNT_HOOK)
	addiu	$2,$2,%lo(__ECNT_HOOK)
	jalr	$2
	nop

	sw	$2,24($sp)
	j	$L228
	nop

$L224:
	lw	$2,184($sp)
	andi	$2,$2,0x2
	beq	$2,$0,$L228
	nop

	li	$2,1			# 0x1
	sw	$2,28($sp)
	li	$2,1			# 0x1
	sw	$2,32($sp)
	sw	$0,36($sp)
	li	$2,43			# 0x2b
	sw	$2,40($sp)
	lw	$2,28($sp)
	sw	$2,44($sp)
	lw	$2,32($sp)
	sw	$2,60($sp)
	addiu	$2,$sp,40
	li	$4,18			# 0x12
	move	$5,$0
	move	$6,$2
	lui	$2,%hi(__ECNT_HOOK)
	addiu	$2,$2,%lo(__ECNT_HOOK)
	jalr	$2
	nop

	sw	$2,36($sp)
$L228:
	move	$2,$0
	lw	$31,180($sp)
	addiu	$sp,$sp,184
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	xmcs_set_wan_cnt_stats
	.size	xmcs_set_wan_cnt_stats, .-xmcs_set_wan_cnt_stats
	.section	.text.xmcs_get_wan_cnt_stats,"ax",@progbits
	.align	2
	.globl	xmcs_get_wan_cnt_stats
	.set	nomips16
	.set	nomicromips
	.ent	xmcs_get_wan_cnt_stats
	.type	xmcs_get_wan_cnt_stats, @function
xmcs_get_wan_cnt_stats:
	.frame	$sp,512,$31		# vars= 488, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	addiu	$sp,$sp,-512
	sw	$31,508($sp)
	sw	$4,512($sp)
	lw	$2,512($sp)
	bne	$2,$0,$L234
	li	$2,-22			# 0xffffffffffffffea
	j	$L249
$L234:
	addiu	$3,$sp,104
	li	$2,128			# 0x80
	move	$4,$3
	move	$5,$0
	move	$6,$2
	lui	$2,%hi(memset)
	addiu	$2,$2,%lo(memset)
	jalr	$2
	lui	$2,%hi(gpPonSysData)
	lw	$2,%lo(gpPonSysData)($2)
	lw	$3,0($2)
	li	$2,1			# 0x1
	bne	$3,$2,$L236
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,17200
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	sw	$2,72($sp)
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,17204
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	sw	$2,76($sp)
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,17208
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	sw	$2,80($sp)
	lw	$3,72($sp)
	lw	$2,76($sp)
	addu	$3,$3,$2
	lw	$2,80($sp)
	addu	$2,$3,$2
	sw	$2,224($sp)
	j	$L237
$L236:
	sw	$0,224($sp)
$L237:
	li	$2,32781			# 0x800d
	sw	$2,44($sp)
	addiu	$2,$sp,84
	sw	$2,48($sp)
	sw	$0,348($sp)
	sw	$0,352($sp)
	sw	$0,356($sp)
	sw	$0,360($sp)
	sw	$0,348($sp)
	lw	$2,44($sp)
	sw	$2,352($sp)
	lw	$2,48($sp)
	sw	$2,360($sp)
	addiu	$2,$sp,348
	sw	$2,52($sp)
	li	$4,25			# 0x19
	move	$5,$0
	lw	$6,52($sp)
	lui	$2,%hi(__ECNT_HOOK)
	addiu	$2,$2,%lo(__ECNT_HOOK)
	jalr	$2
	move	$3,$2
	li	$2,-1			# 0xffffffffffffffff
	bne	$3,$2,$L238
	lw	$2,52($sp)
	lw	$3,4($2)
	lui	$2,%hi($LC2)
	addiu	$4,$2,%lo($LC2)
	move	$5,$3
	lui	$2,%hi(panic)
	addiu	$2,$2,%lo(panic)
	jalr	$2
$L238:
	lw	$2,52($sp)
	lw	$3,8($2)
	li	$2,-1			# 0xffffffffffffffff
	bne	$3,$2,$L239
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
$L239:
	lw	$2,92($sp)
	sw	$2,228($sp)
	addiu	$2,$sp,232
	sw	$2,56($sp)
	sw	$0,60($sp)
	li	$2,40			# 0x28
	sw	$2,364($sp)
	addiu	$2,$sp,364
	li	$4,18			# 0x12
	move	$5,$0
	move	$6,$2
	lui	$2,%hi(__ECNT_HOOK)
	addiu	$2,$2,%lo(__ECNT_HOOK)
	jalr	$2
	sw	$2,60($sp)
	lw	$2,56($sp)
	move	$4,$2
	addiu	$3,$sp,416
	li	$2,52			# 0x34
	move	$5,$3
	move	$6,$2
	lui	$2,%hi(memcpy)
	addiu	$2,$2,%lo(memcpy)
	jalr	$2
	addiu	$2,$sp,284
	sw	$2,64($sp)
	sw	$0,68($sp)
	li	$2,39			# 0x27
	sw	$2,364($sp)
	addiu	$2,$sp,364
	li	$4,18			# 0x12
	move	$5,$0
	move	$6,$2
	lui	$2,%hi(__ECNT_HOOK)
	addiu	$2,$2,%lo(__ECNT_HOOK)
	jalr	$2
	sw	$2,68($sp)
	lw	$2,64($sp)
	move	$4,$2
	addiu	$3,$sp,416
	li	$2,64			# 0x40
	move	$5,$3
	move	$6,$2
	lui	$2,%hi(memcpy)
	addiu	$2,$2,%lo(memcpy)
	jalr	$2
	lw	$2,232($sp)
	sw	$2,108($sp)
	lw	$2,236($sp)
	sw	$2,112($sp)
	lw	$2,240($sp)
	sw	$2,116($sp)
	lw	$2,244($sp)
	sw	$2,120($sp)
	lw	$2,248($sp)
	sw	$2,124($sp)
	lw	$2,252($sp)
	sw	$2,128($sp)
	lw	$2,256($sp)
	sw	$2,132($sp)
	lw	$2,260($sp)
	sw	$2,136($sp)
	lw	$2,264($sp)
	sw	$2,140($sp)
	lw	$2,268($sp)
	sw	$2,144($sp)
	lw	$2,272($sp)
	sw	$2,148($sp)
	lw	$2,276($sp)
	sw	$2,152($sp)
	lw	$2,280($sp)
	sw	$2,156($sp)
	lw	$2,284($sp)
	sw	$2,160($sp)
	lw	$2,288($sp)
	sw	$2,164($sp)
	lw	$2,292($sp)
	sw	$2,168($sp)
	lw	$2,296($sp)
	sw	$2,172($sp)
	lw	$2,300($sp)
	sw	$2,176($sp)
	lw	$2,304($sp)
	sw	$2,180($sp)
	lw	$2,308($sp)
	sw	$2,184($sp)
	lw	$2,312($sp)
	sw	$2,188($sp)
	lw	$2,316($sp)
	sw	$2,192($sp)
	lw	$2,320($sp)
	sw	$2,196($sp)
	lw	$2,324($sp)
	sw	$2,200($sp)
	lw	$2,328($sp)
	sw	$2,204($sp)
	lw	$2,332($sp)
	sw	$2,208($sp)
	lw	$2,336($sp)
	sw	$2,212($sp)
	lw	$2,340($sp)
	sw	$2,216($sp)
	lw	$2,344($sp)
	sw	$2,220($sp)
	lw	$2,512($sp)
	sw	$2,20($sp)
	addiu	$2,$sp,104
	sw	$2,24($sp)
	li	$2,128			# 0x80
	sw	$2,16($sp)
	lw	$2,20($sp)
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
	beq	$2,$0,$L248
	lw	$4,20($sp)
	lw	$5,24($sp)
	lw	$6,16($sp)
#APP
 # 1180 "/opt/tclinux_phoenix/modules/private/xpon/src/xmcs/xmcs_if.c" 1
	.set	noat
	la	$1, __copy_user
	jalr	$1
	.set	at
	
 # 0 "" 2
#NO_APP
	move	$2,$6
	sw	$2,16($sp)
$L248:
	move	$2,$0
$L249:
	lw	$31,508($sp)
	addiu	$sp,$sp,512
	j	$31
	.end	xmcs_get_wan_cnt_stats
	.size	xmcs_get_wan_cnt_stats, .-xmcs_get_wan_cnt_stats
	.section	.text.xpon_reset_qdma_tx_buf,"ax",@progbits
	.align	2
	.globl	xpon_reset_qdma_tx_buf
	.set	nomips16
	.set	nomicromips
	.ent	xpon_reset_qdma_tx_buf
	.type	xpon_reset_qdma_tx_buf, @function
xpon_reset_qdma_tx_buf:
	.frame	$sp,88,$31		# vars= 64, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-88
	sw	$31,84($sp)
	li	$2,1			# 0x1
	sw	$2,28($sp)
	li	$2,32			# 0x20
	sw	$2,64($sp)
	lui	$2,%hi(gpWanPriv)
	lw	$2,%lo(gpWanPriv)($2)
	lw	$2,20($2)
	li	$3,128			# 0x80
	divu	$0,$3,$2
	mfhi	$3
	mflo	$2
	sw	$2,68($sp)
	lw	$2,64($sp)
	move	$3,$2
	lw	$2,68($sp)
	sltu	$4,$2,$3
	movz	$2,$3,$4
	andi	$2,$2,0x00ff
	sb	$2,32($sp)
	li	$2,-128			# 0xffffffffffffff80
	sb	$2,33($sp)
	li	$2,16			# 0x10
	sw	$2,16($sp)
	addiu	$2,$sp,28
	sw	$2,20($sp)
	sw	$0,24($sp)
	li	$2,17			# 0x11
	sw	$2,36($sp)
	lw	$2,20($sp)
	sw	$2,44($sp)
	addiu	$2,$sp,36
	lw	$4,16($sp)
	move	$5,$0
	move	$6,$2
	lui	$2,%hi(__ECNT_HOOK)
	addiu	$2,$2,%lo(__ECNT_HOOK)
	jalr	$2
	nop

	sw	$2,24($sp)
	move	$2,$0
	lw	$31,84($sp)
	addiu	$sp,$sp,88
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	xpon_reset_qdma_tx_buf
	.size	xpon_reset_qdma_tx_buf, .-xpon_reset_qdma_tx_buf
	.section	__ksymtab_strings
	.type	__kstrtab_xpon_reset_qdma_tx_buf, @object
	.size	__kstrtab_xpon_reset_qdma_tx_buf, 23
__kstrtab_xpon_reset_qdma_tx_buf:
	.ascii	"xpon_reset_qdma_tx_buf\000"
	.section	__ksymtab
	.align	2
	.type	__ksymtab_xpon_reset_qdma_tx_buf, @object
	.size	__ksymtab_xpon_reset_qdma_tx_buf, 8
__ksymtab_xpon_reset_qdma_tx_buf:
	.word	xpon_reset_qdma_tx_buf
	.word	__kstrtab_xpon_reset_qdma_tx_buf
	.globl	green_drop_flag
	.data
	.type	green_drop_flag, @object
	.size	green_drop_flag, 1
green_drop_flag:
	.byte	1
	.section	.text.xpon_set_qdma_qos,"ax",@progbits
	.align	2
	.globl	xpon_set_qdma_qos
	.set	nomips16
	.set	nomicromips
	.ent	xpon_set_qdma_qos
	.type	xpon_set_qdma_qos, @function
xpon_set_qdma_qos:
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	sw	$4,0($sp)
	lw	$2,0($sp)
	bne	$2,$0,$L256
	nop

	lui	$2,%hi(gpWanPriv)
	lw	$3,%lo(gpWanPriv)($2)
	li	$2,65536			# 0x10000
	addu	$3,$3,$2
	lbu	$2,-31786($3)
	ins	$2,$0,6,1
	sb	$2,-31786($3)
	j	$L257
	nop

$L256:
	lui	$2,%hi(gpWanPriv)
	lw	$3,%lo(gpWanPriv)($2)
	li	$2,65536			# 0x10000
	addu	$3,$3,$2
	lbu	$2,-31786($3)
	li	$4,1			# 0x1
	ins	$2,$4,6,1
	sb	$2,-31786($3)
$L257:
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	xpon_set_qdma_qos
	.size	xpon_set_qdma_qos, .-xpon_set_qdma_qos
	.section	__ksymtab_strings
	.type	__kstrtab_xpon_set_qdma_qos, @object
	.size	__kstrtab_xpon_set_qdma_qos, 18
__kstrtab_xpon_set_qdma_qos:
	.ascii	"xpon_set_qdma_qos\000"
	.section	__ksymtab
	.align	2
	.type	__ksymtab_xpon_set_qdma_qos, @object
	.size	__ksymtab_xpon_set_qdma_qos, 8
__ksymtab_xpon_set_qdma_qos:
	.word	xpon_set_qdma_qos
	.word	__kstrtab_xpon_set_qdma_qos
	.rdata
	.align	2
$LC8:
	.ascii	"xpon_set_qos:wrong value\012\000"
	.section	.text.xpon_set_qos,"ax",@progbits
	.align	2
	.globl	xpon_set_qos
	.set	nomips16
	.set	nomicromips
	.ent	xpon_set_qos
	.type	xpon_set_qos, @function
xpon_set_qos:
	.frame	$sp,24,$31		# vars= 0, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-24
	sw	$31,20($sp)
	move	$3,$4
	move	$2,$5
	sb	$3,24($sp)
	sb	$2,28($sp)
	lbu	$2,24($sp)
	bne	$2,$0,$L259
	nop

	lui	$2,%hi(gpWanPriv)
	lw	$3,%lo(gpWanPriv)($2)
	li	$2,65536			# 0x10000
	addu	$3,$3,$2
	lbu	$2,-31786($3)
	ins	$2,$0,6,1
	sb	$2,-31786($3)
	move	$4,$0
	lui	$2,%hi(xpon_set_qdma_qos)
	addiu	$2,$2,%lo(xpon_set_qdma_qos)
	jalr	$2
	nop

	j	$L260
	nop

$L259:
	lbu	$3,24($sp)
	li	$2,1			# 0x1
	bne	$3,$2,$L261
	nop

	lui	$2,%hi(gpWanPriv)
	lw	$3,%lo(gpWanPriv)($2)
	li	$2,65536			# 0x10000
	addu	$3,$3,$2
	lbu	$2,-31786($3)
	li	$4,1			# 0x1
	ins	$2,$4,6,1
	sb	$2,-31786($3)
	lbu	$2,28($sp)
	sltu	$2,$2,9
	beq	$2,$0,$L262
	nop

	lbu	$2,28($sp)
	sltu	$2,$2,5
	bne	$2,$0,$L262
	nop

	lui	$2,%hi(gpWanPriv)
	lw	$2,%lo(gpWanPriv)($2)
	lbu	$3,28($sp)
	li	$4,64			# 0x40
	div	$0,$4,$3
	mfhi	$4
	mflo	$3
	andi	$3,$3,0x00ff
	sb	$3,24($2)
$L262:
	li	$4,1			# 0x1
	lui	$2,%hi(xpon_set_qdma_qos)
	addiu	$2,$2,%lo(xpon_set_qdma_qos)
	jalr	$2
	nop

	j	$L260
	nop

$L261:
	lui	$2,%hi($LC8)
	addiu	$4,$2,%lo($LC8)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L260:
	move	$2,$0
	lw	$31,20($sp)
	addiu	$sp,$sp,24
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	xpon_set_qos
	.size	xpon_set_qos, .-xpon_set_qos
	.section	__ksymtab_strings
	.type	__kstrtab_xpon_set_qos, @object
	.size	__kstrtab_xpon_set_qos, 13
__kstrtab_xpon_set_qos:
	.ascii	"xpon_set_qos\000"
	.section	__ksymtab
	.align	2
	.type	__ksymtab_xpon_set_qos, @object
	.size	__ksymtab_xpon_set_qos, 8
__ksymtab_xpon_set_qos:
	.word	xpon_set_qos
	.word	__kstrtab_xpon_set_qos
	.globl	mulitcast_ani
	.data
	.align	2
	.type	mulitcast_ani, @object
	.size	mulitcast_ani, 4
mulitcast_ani:
	.word	-1
	.section	__ksymtab_strings
	.type	__kstrtab_mulitcast_ani, @object
	.size	__kstrtab_mulitcast_ani, 14
__kstrtab_mulitcast_ani:
	.ascii	"mulitcast_ani\000"
	.section	__ksymtab
	.align	2
	.type	__ksymtab_mulitcast_ani, @object
	.size	__ksymtab_mulitcast_ani, 8
__ksymtab_mulitcast_ani:
	.word	mulitcast_ani
	.word	__kstrtab_mulitcast_ani
	.section	.text.xmcs_assign_gem_port,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	xmcs_assign_gem_port
	.type	xmcs_assign_gem_port, @function
xmcs_assign_gem_port:
	.frame	$sp,40,$31		# vars= 16, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-40
	sw	$31,36($sp)
	sw	$4,40($sp)
	lw	$2,40($sp)
	beq	$2,$0,$L265
	nop

	lw	$2,40($sp)
	lhu	$2,1024($2)
	sltu	$2,$2,256
	bne	$2,$0,$L266
	nop

$L265:
	li	$2,-22			# 0xffffffffffffffea
	j	$L267
	nop

$L266:
	sw	$0,16($sp)
	j	$L268
	nop

$L273:
	lw	$3,40($sp)
	lw	$2,16($sp)
	sll	$2,$2,2
	addu	$2,$3,$2
	lhu	$2,2($2)
	sltu	$2,$2,256
	bne	$2,$0,$L269
	nop

	li	$2,-22			# 0xffffffffffffffea
	j	$L267
	nop

$L269:
	lui	$2,%hi(gpWanPriv)
	lw	$2,%lo(gpWanPriv)($2)
	lw	$2,8($2)
	bne	$2,$0,$L270
	nop

	li	$2,-22			# 0xffffffffffffffea
	j	$L267
	nop

$L270:
	lw	$3,40($sp)
	lw	$2,16($sp)
	sll	$2,$2,2
	addu	$2,$3,$2
	lhu	$2,0($2)
	sltu	$2,$2,4096
	beq	$2,$0,$L271
	nop

	lw	$3,40($sp)
	lw	$2,16($sp)
	sll	$2,$2,2
	addu	$2,$3,$2
	lhu	$3,0($2)
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lhu	$2,252($2)
	beq	$3,$2,$L271
	nop

	lw	$3,40($sp)
	lw	$2,16($sp)
	sll	$2,$2,2
	addu	$2,$3,$2
	lhu	$2,0($2)
	move	$4,$2
	lw	$3,40($sp)
	lw	$2,16($sp)
	sll	$2,$2,2
	addu	$2,$3,$2
	lhu	$2,2($2)
	li	$5,1			# 0x1
	move	$6,$2
	lui	$2,%hi(gwan_config_gemport)
	addiu	$2,$2,%lo(gwan_config_gemport)
	jalr	$2
	nop

	sw	$2,20($sp)
	lw	$2,20($sp)
	beq	$2,$0,$L271
	nop

	lw	$2,20($sp)
	j	$L267
	nop

$L271:
	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	lw	$3,-30820($2)
	li	$2,2			# 0x2
	bne	$3,$2,$L272
	nop

	lw	$3,40($sp)
	lw	$2,16($sp)
	sll	$2,$2,2
	addu	$2,$3,$2
	lhu	$2,0($2)
	sh	$2,24($sp)
	lhu	$2,24($sp)
	sltu	$2,$2,4096
	beq	$2,$0,$L272
	nop

	lui	$2,%hi(gpWanPriv)
	lw	$3,%lo(gpWanPriv)($2)
	lhu	$2,24($sp)
	addiu	$2,$2,44
	sll	$2,$2,1
	addu	$2,$3,$2
	lhu	$2,4($2)
	sb	$2,26($sp)
	lui	$2,%hi(gpWanPriv)
	lw	$3,%lo(gpWanPriv)($2)
	lbu	$2,26($sp)
	sll	$2,$2,5
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	lw	$2,8284($2)
	ext	$2,$2,2,1
	andi	$2,$2,0x00ff
	beq	$2,$0,$L272
	nop

	lui	$2,%hi(gpWanPriv)
	lw	$3,%lo(gpWanPriv)($2)
	lbu	$2,26($sp)
	sll	$2,$2,5
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	lw	$2,8284($2)
	ext	$2,$2,20,12
	andi	$2,$2,0xffff
	move	$3,$2
	lhu	$2,24($sp)
	bne	$3,$2,$L272
	nop

	lui	$2,%hi(gpWanPriv)
	lw	$3,%lo(gpWanPriv)($2)
	lbu	$2,26($sp)
	sll	$2,$2,5
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	lw	$2,8284($2)
	ext	$2,$2,14,6
	andi	$3,$2,0x00ff
	li	$2,32			# 0x20
	bne	$3,$2,$L272
	nop

	lw	$3,40($sp)
	lw	$2,16($sp)
	sll	$2,$2,2
	addu	$2,$3,$2
	lhu	$2,2($2)
	move	$3,$2
	lui	$2,%hi(mulitcast_ani)
	sw	$3,%lo(mulitcast_ani)($2)
$L272:
	lw	$2,16($sp)
	addiu	$2,$2,1
	sw	$2,16($sp)
$L268:
	lw	$2,40($sp)
	lhu	$2,1024($2)
	move	$3,$2
	lw	$2,16($sp)
	slt	$2,$2,$3
	bne	$2,$0,$L273
	nop

	move	$2,$0
$L267:
	lw	$31,36($sp)
	addiu	$sp,$sp,40
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	xmcs_assign_gem_port
	.size	xmcs_assign_gem_port, .-xmcs_assign_gem_port
	.section	.text.assignGemportId,"ax",@progbits
	.align	2
	.globl	assignGemportId
	.set	nomips16
	.set	nomicromips
	.ent	assignGemportId
	.type	assignGemportId, @function
assignGemportId:
	.frame	$sp,1056,$31		# vars= 1032, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-1056
	sw	$31,1052($sp)
	move	$2,$4
	sh	$2,1056($sp)
	li	$2,1			# 0x1
	sh	$2,1040($sp)
	lhu	$2,1056($sp)
	sh	$2,16($sp)
	li	$2,1			# 0x1
	sh	$2,18($sp)
	addiu	$2,$sp,16
	move	$4,$2
	lui	$2,%hi(xmcs_assign_gem_port)
	addiu	$2,$2,%lo(xmcs_assign_gem_port)
	jalr	$2
	nop

	lw	$31,1052($sp)
	addiu	$sp,$sp,1056
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	assignGemportId
	.size	assignGemportId, .-assignGemportId
	.section	.text.xmcs_unassign_gem_port,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	xmcs_unassign_gem_port
	.type	xmcs_unassign_gem_port, @function
xmcs_unassign_gem_port:
	.frame	$sp,40,$31		# vars= 16, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-40
	sw	$31,36($sp)
	sw	$4,40($sp)
	lw	$2,40($sp)
	beq	$2,$0,$L276
	nop

	lw	$2,40($sp)
	lhu	$2,1024($2)
	sltu	$2,$2,256
	bne	$2,$0,$L277
	nop

$L276:
	li	$2,-22			# 0xffffffffffffffea
	j	$L278
	nop

$L277:
	sw	$0,16($sp)
	j	$L279
	nop

$L282:
	lw	$3,40($sp)
	lw	$2,16($sp)
	sll	$2,$2,2
	addu	$2,$3,$2
	lhu	$2,0($2)
	sltu	$2,$2,4096
	beq	$2,$0,$L280
	nop

	lw	$3,40($sp)
	lw	$2,16($sp)
	sll	$2,$2,2
	addu	$2,$3,$2
	lhu	$3,0($2)
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lhu	$2,252($2)
	beq	$3,$2,$L280
	nop

	lw	$3,40($sp)
	lw	$2,16($sp)
	sll	$2,$2,2
	addu	$2,$3,$2
	lhu	$2,0($2)
	move	$4,$2
	li	$5,1			# 0x1
	li	$6,256			# 0x100
	lui	$2,%hi(gwan_config_gemport)
	addiu	$2,$2,%lo(gwan_config_gemport)
	jalr	$2
	nop

	sw	$2,20($sp)
	lw	$2,20($sp)
	beq	$2,$0,$L281
	nop

	lw	$2,20($sp)
	j	$L278
	nop

$L281:
	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	lw	$3,-30820($2)
	li	$2,2			# 0x2
	bne	$3,$2,$L280
	nop

	lw	$3,40($sp)
	lw	$2,16($sp)
	sll	$2,$2,2
	addu	$2,$3,$2
	lhu	$2,0($2)
	sh	$2,24($sp)
	lui	$2,%hi(gpWanPriv)
	lw	$3,%lo(gpWanPriv)($2)
	lhu	$2,24($sp)
	addiu	$2,$2,44
	sll	$2,$2,1
	addu	$2,$3,$2
	lhu	$2,4($2)
	sb	$2,26($sp)
	lui	$2,%hi(gpWanPriv)
	lw	$3,%lo(gpWanPriv)($2)
	lbu	$2,26($sp)
	sll	$2,$2,5
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	lw	$2,8284($2)
	ext	$2,$2,2,1
	andi	$2,$2,0x00ff
	beq	$2,$0,$L280
	nop

	lui	$2,%hi(gpWanPriv)
	lw	$3,%lo(gpWanPriv)($2)
	lbu	$2,26($sp)
	sll	$2,$2,5
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	lw	$2,8284($2)
	ext	$2,$2,20,12
	andi	$2,$2,0xffff
	move	$3,$2
	lhu	$2,24($sp)
	bne	$3,$2,$L280
	nop

	lui	$2,%hi(gpWanPriv)
	lw	$3,%lo(gpWanPriv)($2)
	lbu	$2,26($sp)
	sll	$2,$2,5
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	lw	$2,8284($2)
	ext	$2,$2,14,6
	andi	$3,$2,0x00ff
	li	$2,32			# 0x20
	bne	$3,$2,$L280
	nop

	lui	$2,%hi(mulitcast_ani)
	li	$3,-1			# 0xffffffffffffffff
	sw	$3,%lo(mulitcast_ani)($2)
$L280:
	lw	$2,16($sp)
	addiu	$2,$2,1
	sw	$2,16($sp)
$L279:
	lw	$2,40($sp)
	lhu	$2,1024($2)
	move	$3,$2
	lw	$2,16($sp)
	slt	$2,$2,$3
	bne	$2,$0,$L282
	nop

	move	$2,$0
$L278:
	lw	$31,36($sp)
	addiu	$sp,$sp,40
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	xmcs_unassign_gem_port
	.size	xmcs_unassign_gem_port, .-xmcs_unassign_gem_port
	.section	.text.xmcs_create_gem_port,"ax",@progbits
	.align	2
	.globl	xmcs_create_gem_port
	.set	nomips16
	.set	nomicromips
	.ent	xmcs_create_gem_port
	.type	xmcs_create_gem_port, @function
xmcs_create_gem_port:
	.frame	$sp,40,$31		# vars= 16, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-40
	sw	$31,36($sp)
	sw	$4,40($sp)
	li	$2,32			# 0x20
	sb	$2,16($sp)
	sw	$0,24($sp)
	sb	$0,28($sp)
	lw	$2,40($sp)
	lhu	$2,4($2)
	sltu	$2,$2,4096
	beq	$2,$0,$L284
	nop

	lw	$2,40($sp)
	lhu	$3,4($2)
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lhu	$2,252($2)
	bne	$3,$2,$L285
	nop

$L284:
	li	$2,-22			# 0xffffffffffffffea
	j	$L286
	nop

$L285:
	lw	$2,40($sp)
	lw	$2,0($2)
	bne	$2,$0,$L287
	nop

	lw	$2,40($sp)
	lhu	$2,6($2)
	sltu	$2,$2,4096
	bne	$2,$0,$L288
	nop

	li	$2,-22			# 0xffffffffffffffea
	j	$L286
	nop

$L288:
	sw	$0,20($sp)
	j	$L289
	nop

$L292:
	lui	$2,%hi(gpWanPriv)
	lw	$3,%lo(gpWanPriv)($2)
	lw	$2,20($sp)
	addiu	$2,$2,12
	sll	$2,$2,1
	addu	$2,$3,$2
	lhu	$3,4($2)
	lw	$2,40($sp)
	lhu	$2,6($2)
	bne	$3,$2,$L290
	nop

	lw	$2,20($sp)
	sb	$2,16($sp)
	j	$L291
	nop

$L290:
	lw	$2,20($sp)
	addiu	$2,$2,1
	sw	$2,20($sp)
$L289:
	lw	$2,20($sp)
	slt	$2,$2,32
	bne	$2,$0,$L292
	nop

$L291:
	lw	$3,20($sp)
	li	$2,32			# 0x20
	bne	$3,$2,$L294
	nop

	sb	$0,16($sp)
	j	$L294
	nop

$L287:
	lw	$2,40($sp)
	lw	$3,0($2)
	li	$2,1			# 0x1
	bne	$3,$2,$L295
	nop

	li	$2,32			# 0x20
	sb	$2,16($sp)
	j	$L294
	nop

$L295:
	li	$2,-22			# 0xffffffffffffffea
	j	$L286
	nop

$L294:
	lui	$2,%hi(gpWanPriv)
	lw	$3,%lo(gpWanPriv)($2)
	lw	$2,40($sp)
	lhu	$2,4($2)
	addiu	$2,$2,44
	sll	$2,$2,1
	addu	$2,$3,$2
	lhu	$2,4($2)
	sb	$2,29($sp)
	lui	$2,%hi(gpWanPriv)
	lw	$3,%lo(gpWanPriv)($2)
	lbu	$2,29($sp)
	sll	$2,$2,5
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	lw	$2,8284($2)
	ext	$2,$2,2,1
	andi	$2,$2,0x00ff
	beq	$2,$0,$L296
	nop

	lui	$2,%hi(gpWanPriv)
	lw	$3,%lo(gpWanPriv)($2)
	lbu	$2,29($sp)
	sll	$2,$2,5
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	lw	$2,8284($2)
	ext	$2,$2,20,12
	andi	$2,$2,0xffff
	move	$3,$2
	lw	$2,40($sp)
	lhu	$2,4($2)
	bne	$3,$2,$L296
	nop

	lw	$2,40($sp)
	lhu	$2,4($2)
	move	$3,$2
	lbu	$2,16($sp)
	move	$4,$3
	li	$5,2			# 0x2
	move	$6,$2
	lui	$2,%hi(gwan_config_gemport)
	addiu	$2,$2,%lo(gwan_config_gemport)
	jalr	$2
	nop

	j	$L286
	nop

$L296:
	lui	$2,%hi(gpWanPriv)
	lw	$3,%lo(gpWanPriv)($2)
	lw	$2,40($sp)
	lhu	$2,4($2)
	addiu	$2,$2,44
	sll	$2,$2,1
	addu	$2,$3,$2
	lhu	$2,4($2)
	seh	$2,$2
	andi	$2,$2,0xffff
	srl	$2,$2,15
	andi	$2,$2,0x00ff
	sb	$2,28($sp)
	lw	$2,40($sp)
	lhu	$2,4($2)
	move	$4,$2
	lbu	$3,16($sp)
	lbu	$2,28($sp)
	move	$5,$3
	move	$6,$2
	lui	$2,%hi(gwan_create_new_gemport)
	addiu	$2,$2,%lo(gwan_create_new_gemport)
	jalr	$2
	nop

	sw	$2,24($sp)
	lw	$2,24($sp)
$L286:
	lw	$31,36($sp)
	addiu	$sp,$sp,40
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	xmcs_create_gem_port
	.size	xmcs_create_gem_port, .-xmcs_create_gem_port
	.section	__ksymtab_strings
	.type	__kstrtab_xmcs_create_gem_port, @object
	.size	__kstrtab_xmcs_create_gem_port, 21
__kstrtab_xmcs_create_gem_port:
	.ascii	"xmcs_create_gem_port\000"
	.section	__ksymtab
	.align	2
	.type	__ksymtab_xmcs_create_gem_port, @object
	.size	__ksymtab_xmcs_create_gem_port, 8
__ksymtab_xmcs_create_gem_port:
	.word	xmcs_create_gem_port
	.word	__kstrtab_xmcs_create_gem_port
	.section	.text.xmcs_remove_gem_port,"ax",@progbits
	.align	2
	.globl	xmcs_remove_gem_port
	.set	nomips16
	.set	nomicromips
	.ent	xmcs_remove_gem_port
	.type	xmcs_remove_gem_port, @function
xmcs_remove_gem_port:
	.frame	$sp,24,$31		# vars= 0, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-24
	sw	$31,20($sp)
	move	$2,$4
	sh	$2,24($sp)
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lhu	$2,252($2)
	lhu	$3,24($sp)
	bne	$3,$2,$L298
	nop

	li	$2,-22			# 0xffffffffffffffea
	j	$L299
	nop

$L298:
	lhu	$2,24($sp)
	move	$4,$2
	lui	$2,%hi(gwan_remove_gemport)
	addiu	$2,$2,%lo(gwan_remove_gemport)
	jalr	$2
	nop

$L299:
	lw	$31,20($sp)
	addiu	$sp,$sp,24
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	xmcs_remove_gem_port
	.size	xmcs_remove_gem_port, .-xmcs_remove_gem_port
	.section	__ksymtab_strings
	.type	__kstrtab_xmcs_remove_gem_port, @object
	.size	__kstrtab_xmcs_remove_gem_port, 21
__kstrtab_xmcs_remove_gem_port:
	.ascii	"xmcs_remove_gem_port\000"
	.section	__ksymtab
	.align	2
	.type	__ksymtab_xmcs_remove_gem_port, @object
	.size	__ksymtab_xmcs_remove_gem_port, 8
__ksymtab_xmcs_remove_gem_port:
	.word	xmcs_remove_gem_port
	.word	__kstrtab_xmcs_remove_gem_port
	.section	.text.xmcs_remove_all_gem_port,"ax",@progbits
	.align	2
	.globl	xmcs_remove_all_gem_port
	.set	nomips16
	.set	nomicromips
	.ent	xmcs_remove_all_gem_port
	.type	xmcs_remove_all_gem_port, @function
xmcs_remove_all_gem_port:
	.frame	$sp,24,$31		# vars= 0, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-24
	sw	$31,20($sp)
	lui	$2,%hi(gwan_remove_all_gemport)
	addiu	$2,$2,%lo(gwan_remove_all_gemport)
	jalr	$2
	nop

	lw	$31,20($sp)
	addiu	$sp,$sp,24
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	xmcs_remove_all_gem_port
	.size	xmcs_remove_all_gem_port, .-xmcs_remove_all_gem_port
	.section	__ksymtab_strings
	.type	__kstrtab_xmcs_remove_all_gem_port, @object
	.size	__kstrtab_xmcs_remove_all_gem_port, 25
__kstrtab_xmcs_remove_all_gem_port:
	.ascii	"xmcs_remove_all_gem_port\000"
	.section	__ksymtab
	.align	2
	.type	__ksymtab_xmcs_remove_all_gem_port, @object
	.size	__ksymtab_xmcs_remove_all_gem_port, 8
__ksymtab_xmcs_remove_all_gem_port:
	.word	xmcs_remove_all_gem_port
	.word	__kstrtab_xmcs_remove_all_gem_port
	.section	.text.xmcs_set_gem_port_loopback,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	xmcs_set_gem_port_loopback
	.type	xmcs_set_gem_port_loopback, @function
xmcs_set_gem_port_loopback:
	.frame	$sp,24,$31		# vars= 0, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-24
	sw	$31,20($sp)
	sw	$4,24($sp)
	lw	$2,24($sp)
	lhu	$2,0($2)
	sltu	$2,$2,4096
	beq	$2,$0,$L303
	nop

	lw	$2,24($sp)
	lhu	$3,0($2)
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lhu	$2,252($2)
	bne	$3,$2,$L304
	nop

$L303:
	li	$2,-22			# 0xffffffffffffffea
	j	$L305
	nop

$L304:
	lw	$2,24($sp)
	lw	$2,4($2)
	bne	$2,$0,$L306
	nop

	lw	$2,24($sp)
	lhu	$2,0($2)
	move	$4,$2
	li	$5,4			# 0x4
	move	$6,$0
	lui	$2,%hi(gwan_config_gemport)
	addiu	$2,$2,%lo(gwan_config_gemport)
	jalr	$2
	nop

	j	$L305
	nop

$L306:
	lw	$2,24($sp)
	lhu	$2,0($2)
	move	$4,$2
	li	$5,4			# 0x4
	li	$6,1			# 0x1
	lui	$2,%hi(gwan_config_gemport)
	addiu	$2,$2,%lo(gwan_config_gemport)
	jalr	$2
	nop

$L305:
	lw	$31,20($sp)
	addiu	$sp,$sp,24
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	xmcs_set_gem_port_loopback
	.size	xmcs_set_gem_port_loopback, .-xmcs_set_gem_port_loopback
	.section	.text.xmcs_get_gem_port_info,"ax",@progbits
	.align	2
	.globl	xmcs_get_gem_port_info
	.set	nomips16
	.set	nomicromips
	.ent	xmcs_get_gem_port_info
	.type	xmcs_get_gem_port_info, @function
xmcs_get_gem_port_info:
	.frame	$sp,8,$31		# vars= 8, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-8
	sw	$4,8($sp)
	lw	$2,8($sp)
	sh	$0,5120($2)
	sw	$0,0($sp)
	j	$L308
	nop

$L314:
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
	beq	$2,$0,$L309
	nop

	lui	$2,%hi(gpWanPriv)
	lw	$3,%lo(gpWanPriv)($2)
	lw	$2,0($sp)
	sll	$2,$2,5
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	lw	$2,8284($2)
	ext	$2,$2,14,6
	andi	$2,$2,0x00ff
	sb	$2,4($sp)
	lbu	$3,4($sp)
	li	$2,32			# 0x20
	bne	$3,$2,$L310
	nop

	lw	$2,8($sp)
	lhu	$2,5120($2)
	lw	$3,8($sp)
	sll	$2,$2,2
	sll	$4,$2,2
	addu	$2,$2,$4
	addu	$2,$3,$2
	li	$3,1			# 0x1
	sw	$3,0($2)
	lw	$2,8($sp)
	lhu	$2,5120($2)
	lw	$3,8($sp)
	sll	$2,$2,2
	sll	$4,$2,2
	addu	$2,$2,$4
	addu	$2,$3,$2
	li	$3,255			# 0xff
	sh	$3,6($2)
	j	$L311
	nop

$L310:
	lw	$2,8($sp)
	lhu	$2,5120($2)
	lw	$3,8($sp)
	sll	$2,$2,2
	sll	$4,$2,2
	addu	$2,$2,$4
	addu	$2,$3,$2
	sw	$0,0($2)
	lw	$2,8($sp)
	lhu	$2,5120($2)
	move	$5,$2
	lbu	$3,4($sp)
	li	$2,33			# 0x21
	beq	$3,$2,$L312
	nop

	lui	$2,%hi(gpWanPriv)
	lw	$3,%lo(gpWanPriv)($2)
	lbu	$2,4($sp)
	addiu	$2,$2,12
	sll	$2,$2,1
	addu	$2,$3,$2
	lhu	$3,4($2)
	j	$L313
	nop

$L312:
	li	$3,255			# 0xff
$L313:
	lw	$4,8($sp)
	move	$2,$5
	sll	$2,$2,2
	sll	$5,$2,2
	addu	$2,$2,$5
	addu	$2,$4,$2
	sh	$3,6($2)
$L311:
	lw	$2,8($sp)
	lhu	$2,5120($2)
	move	$6,$2
	lui	$2,%hi(gpWanPriv)
	lw	$3,%lo(gpWanPriv)($2)
	lw	$2,0($sp)
	sll	$2,$2,5
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	lw	$2,8284($2)
	ext	$2,$2,20,12
	andi	$2,$2,0xffff
	move	$5,$2
	lw	$3,8($sp)
	move	$2,$6
	sll	$2,$2,2
	sll	$4,$2,2
	addu	$2,$2,$4
	addu	$2,$3,$2
	sh	$5,4($2)
	lw	$2,8($sp)
	lhu	$2,5120($2)
	move	$6,$2
	lui	$2,%hi(gpWanPriv)
	lw	$3,%lo(gpWanPriv)($2)
	lw	$2,0($sp)
	sll	$2,$2,5
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	lw	$2,8284($2)
	ext	$2,$2,5,9
	andi	$2,$2,0xffff
	move	$5,$2
	lw	$3,8($sp)
	move	$2,$6
	sll	$2,$2,2
	sll	$4,$2,2
	addu	$2,$2,$4
	addu	$2,$3,$2
	sh	$5,8($2)
	lw	$2,8($sp)
	lhu	$2,5120($2)
	move	$6,$2
	lui	$2,%hi(gpWanPriv)
	lw	$3,%lo(gpWanPriv)($2)
	lw	$2,0($sp)
	sll	$2,$2,5
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	lw	$2,8284($2)
	ext	$2,$2,4,1
	andi	$2,$2,0x00ff
	move	$5,$2
	lw	$3,8($sp)
	move	$2,$6
	sll	$2,$2,2
	sll	$4,$2,2
	addu	$2,$2,$4
	addu	$2,$3,$2
	sw	$5,12($2)
	lw	$2,8($sp)
	lhu	$2,5120($2)
	move	$6,$2
	lui	$2,%hi(gpWanPriv)
	lw	$3,%lo(gpWanPriv)($2)
	lw	$2,0($sp)
	sll	$2,$2,5
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	lw	$2,8284($2)
	ext	$2,$2,3,1
	andi	$2,$2,0x00ff
	move	$5,$2
	lw	$3,8($sp)
	move	$2,$6
	sll	$2,$2,2
	sll	$4,$2,2
	addu	$2,$2,$4
	addu	$2,$3,$2
	sw	$5,16($2)
	lw	$2,8($sp)
	lhu	$2,5120($2)
	addiu	$2,$2,1
	andi	$3,$2,0xffff
	lw	$2,8($sp)
	sh	$3,5120($2)
$L309:
	lw	$2,0($sp)
	addiu	$2,$2,1
	sw	$2,0($sp)
$L308:
	lw	$2,0($sp)
	slt	$2,$2,256
	bne	$2,$0,$L314
	nop

	move	$2,$0
	addiu	$sp,$sp,8
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	xmcs_get_gem_port_info
	.size	xmcs_get_gem_port_info, .-xmcs_get_gem_port_info
	.section	__ksymtab_strings
	.type	__kstrtab_xmcs_get_gem_port_info, @object
	.size	__kstrtab_xmcs_get_gem_port_info, 23
__kstrtab_xmcs_get_gem_port_info:
	.ascii	"xmcs_get_gem_port_info\000"
	.section	__ksymtab
	.align	2
	.type	__ksymtab_xmcs_get_gem_port_info, @object
	.size	__ksymtab_xmcs_get_gem_port_info, 8
__ksymtab_xmcs_get_gem_port_info:
	.word	xmcs_get_gem_port_info
	.word	__kstrtab_xmcs_get_gem_port_info
	.section	.text.xmcs_get_tcont_info,"ax",@progbits
	.align	2
	.globl	xmcs_get_tcont_info
	.set	nomips16
	.set	nomicromips
	.ent	xmcs_get_tcont_info
	.type	xmcs_get_tcont_info, @function
xmcs_get_tcont_info:
	.frame	$sp,8,$31		# vars= 8, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-8
	sw	$4,8($sp)
	lw	$2,8($sp)
	sh	$0,128($2)
	sw	$0,0($sp)
	j	$L317
	nop

$L319:
	lui	$2,%hi(gpWanPriv)
	lw	$3,%lo(gpWanPriv)($2)
	lw	$2,0($sp)
	addiu	$2,$2,12
	sll	$2,$2,1
	addu	$2,$3,$2
	lhu	$3,4($2)
	li	$2,255			# 0xff
	beq	$3,$2,$L318
	nop

	lw	$2,8($sp)
	lhu	$2,128($2)
	move	$5,$2
	lui	$2,%hi(gpWanPriv)
	lw	$3,%lo(gpWanPriv)($2)
	lw	$2,0($sp)
	addiu	$2,$2,12
	sll	$2,$2,1
	addu	$2,$3,$2
	lhu	$3,4($2)
	lw	$4,8($sp)
	sll	$2,$5,2
	addu	$2,$4,$2
	sh	$3,0($2)
	lw	$2,8($sp)
	lhu	$2,128($2)
	lw	$3,0($sp)
	andi	$3,$3,0x00ff
	lw	$4,8($sp)
	sll	$2,$2,2
	addu	$2,$4,$2
	sb	$3,2($2)
	lw	$2,8($sp)
	lhu	$2,128($2)
	addiu	$2,$2,1
	andi	$3,$2,0xffff
	lw	$2,8($sp)
	sh	$3,128($2)
$L318:
	lw	$2,0($sp)
	addiu	$2,$2,1
	sw	$2,0($sp)
$L317:
	lw	$2,0($sp)
	slt	$2,$2,32
	bne	$2,$0,$L319
	nop

	move	$2,$0
	addiu	$sp,$sp,8
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	xmcs_get_tcont_info
	.size	xmcs_get_tcont_info, .-xmcs_get_tcont_info
	.section	__ksymtab_strings
	.type	__kstrtab_xmcs_get_tcont_info, @object
	.size	__kstrtab_xmcs_get_tcont_info, 20
__kstrtab_xmcs_get_tcont_info:
	.ascii	"xmcs_get_tcont_info\000"
	.section	__ksymtab
	.align	2
	.type	__ksymtab_xmcs_get_tcont_info, @object
	.size	__ksymtab_xmcs_get_tcont_info, 8
__ksymtab_xmcs_get_tcont_info:
	.word	xmcs_get_tcont_info
	.word	__kstrtab_xmcs_get_tcont_info
	.section	.text.xmcs_remove_tcont_info,"ax",@progbits
	.align	2
	.globl	xmcs_remove_tcont_info
	.set	nomips16
	.set	nomicromips
	.ent	xmcs_remove_tcont_info
	.type	xmcs_remove_tcont_info, @function
xmcs_remove_tcont_info:
	.frame	$sp,24,$31		# vars= 0, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-24
	sw	$31,20($sp)
	move	$2,$4
	sh	$2,24($sp)
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lbu	$2,204($2)
	andi	$2,$2,0xffff
	lhu	$3,24($sp)
	beq	$3,$2,$L322
	nop

	lhu	$2,24($sp)
	move	$4,$2
	lui	$2,%hi(gwan_remove_tcont)
	addiu	$2,$2,%lo(gwan_remove_tcont)
	jalr	$2
	nop

	bne	$2,$0,$L322
	nop

	lui	$2,%hi(gpWanPriv)
	lw	$2,%lo(gpWanPriv)($2)
	lw	$3,20($2)
	addiu	$3,$3,-1
	sw	$3,20($2)
	lui	$2,%hi(gpWanPriv)
	lw	$2,%lo(gpWanPriv)($2)
	lw	$2,20($2)
	bne	$2,$0,$L323
	nop

	lui	$2,%hi(gpWanPriv)
	lw	$2,%lo(gpWanPriv)($2)
	li	$3,1			# 0x1
	sw	$3,20($2)
$L323:
	lui	$2,%hi(xpon_reset_qdma_tx_buf)
	addiu	$2,$2,%lo(xpon_reset_qdma_tx_buf)
	jalr	$2
	nop

	move	$2,$0
	j	$L324
	nop

$L322:
	move	$2,$0
$L324:
	lw	$31,20($sp)
	addiu	$sp,$sp,24
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	xmcs_remove_tcont_info
	.size	xmcs_remove_tcont_info, .-xmcs_remove_tcont_info
	.section	.text.xmcs_create_llid,"ax",@progbits
	.align	2
	.globl	xmcs_create_llid
	.set	nomips16
	.set	nomicromips
	.ent	xmcs_create_llid
	.type	xmcs_create_llid, @function
xmcs_create_llid:
	.frame	$sp,8,$31		# vars= 8, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-8
	sw	$4,8($sp)
	lw	$2,8($sp)
	lbu	$2,0($2)
	sb	$2,0($sp)
	lbu	$2,0($sp)
	sltu	$2,$2,8
	bne	$2,$0,$L326
	nop

	li	$2,-22			# 0xffffffffffffffea
	j	$L327
	nop

$L326:
	lui	$2,%hi(gpWanPriv)
	lw	$3,%lo(gpWanPriv)($2)
	lbu	$2,0($sp)
	sll	$2,$2,5
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	li	$3,65536			# 0x10000
	addu	$2,$3,$2
	lw	$2,-32644($2)
	ext	$2,$2,8,1
	andi	$2,$2,0x00ff
	beq	$2,$0,$L328
	nop

	li	$2,-17			# 0xffffffffffffffef
	j	$L327
	nop

$L328:
	lui	$2,%hi(gpWanPriv)
	lw	$4,%lo(gpWanPriv)($2)
	lbu	$5,0($sp)
	lw	$2,8($sp)
	lhu	$3,2($2)
	move	$2,$5
	sll	$2,$2,5
	sll	$5,$2,2
	subu	$2,$5,$2
	addu	$2,$4,$2
	li	$4,65536			# 0x10000
	addu	$2,$4,$2
	sh	$3,-32644($2)
	lui	$2,%hi(gpWanPriv)
	lw	$3,%lo(gpWanPriv)($2)
	lbu	$5,0($sp)
	lbu	$2,0($sp)
	andi	$2,$2,0xf
	andi	$4,$2,0x00ff
	move	$2,$5
	sll	$2,$2,5
	sll	$5,$2,2
	subu	$2,$5,$2
	addu	$2,$3,$2
	li	$3,65536			# 0x10000
	addu	$3,$3,$2
	lw	$2,-32644($3)
	ins	$2,$4,12,4
	sw	$2,-32644($3)
	lui	$2,%hi(gpWanPriv)
	lw	$3,%lo(gpWanPriv)($2)
	lbu	$2,0($sp)
	sll	$2,$2,5
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	li	$3,65536			# 0x10000
	addu	$3,$3,$2
	lw	$2,-32644($3)
	li	$4,1			# 0x1
	ins	$2,$4,8,1
	sw	$2,-32644($3)
	move	$2,$0
$L327:
	addiu	$sp,$sp,8
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	xmcs_create_llid
	.size	xmcs_create_llid, .-xmcs_create_llid
	.section	__ksymtab_strings
	.type	__kstrtab_xmcs_create_llid, @object
	.size	__kstrtab_xmcs_create_llid, 17
__kstrtab_xmcs_create_llid:
	.ascii	"xmcs_create_llid\000"
	.section	__ksymtab
	.align	2
	.type	__ksymtab_xmcs_create_llid, @object
	.size	__ksymtab_xmcs_create_llid, 8
__ksymtab_xmcs_create_llid:
	.word	xmcs_create_llid
	.word	__kstrtab_xmcs_create_llid
	.section	.text.xmcs_remove_llid,"ax",@progbits
	.align	2
	.globl	xmcs_remove_llid
	.set	nomips16
	.set	nomicromips
	.ent	xmcs_remove_llid
	.type	xmcs_remove_llid, @function
xmcs_remove_llid:
	.frame	$sp,24,$31		# vars= 0, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-24
	sw	$31,20($sp)
	move	$2,$4
	sb	$2,24($sp)
	lbu	$2,24($sp)
	sltu	$2,$2,8
	bne	$2,$0,$L330
	nop

	li	$2,-22			# 0xffffffffffffffea
	j	$L331
	nop

$L330:
	lui	$2,%hi(gpWanPriv)
	lw	$3,%lo(gpWanPriv)($2)
	lbu	$2,24($sp)
	sll	$2,$2,5
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	li	$3,65536			# 0x10000
	addu	$2,$3,$2
	lw	$2,-32644($2)
	ext	$2,$2,8,1
	andi	$2,$2,0x00ff
	bne	$2,$0,$L332
	nop

	lui	$2,%hi(gpWanPriv)
	lw	$4,%lo(gpWanPriv)($2)
	lbu	$2,24($sp)
	move	$3,$2
	sll	$2,$3,5
	move	$3,$2
	sll	$2,$3,2
	subu	$2,$2,$3
	li	$3,32888			# 0x8078
	addu	$2,$2,$3
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
	lbu	$2,24($sp)
	move	$3,$2
	sll	$2,$3,5
	move	$3,$2
	sll	$2,$3,2
	subu	$2,$2,$3
	li	$3,32888			# 0x8078
	addu	$2,$2,$3
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
	lbu	$2,24($sp)
	sll	$2,$2,5
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	li	$3,65536			# 0x10000
	addu	$3,$3,$2
	lw	$2,-32644($3)
	li	$4,1			# 0x1
	ins	$2,$4,11,1
	sw	$2,-32644($3)
	lui	$2,%hi(gpWanPriv)
	lw	$3,%lo(gpWanPriv)($2)
	lbu	$2,24($sp)
	sll	$2,$2,5
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	li	$3,65536			# 0x10000
	addu	$3,$3,$2
	lw	$2,-32644($3)
	li	$4,1			# 0x1
	ins	$2,$4,9,1
	sw	$2,-32644($3)
	move	$2,$0
	j	$L331
	nop

$L332:
	li	$2,-22			# 0xffffffffffffffea
$L331:
	lw	$31,20($sp)
	addiu	$sp,$sp,24
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	xmcs_remove_llid
	.size	xmcs_remove_llid, .-xmcs_remove_llid
	.section	__ksymtab_strings
	.type	__kstrtab_xmcs_remove_llid, @object
	.size	__kstrtab_xmcs_remove_llid, 17
__kstrtab_xmcs_remove_llid:
	.ascii	"xmcs_remove_llid\000"
	.section	__ksymtab
	.align	2
	.type	__ksymtab_xmcs_remove_llid, @object
	.size	__ksymtab_xmcs_remove_llid, 8
__ksymtab_xmcs_remove_llid:
	.word	xmcs_remove_llid
	.word	__kstrtab_xmcs_remove_llid
	.section	.text.xmcs_set_epon_rx_config,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	xmcs_set_epon_rx_config
	.type	xmcs_set_epon_rx_config, @function
xmcs_set_epon_rx_config:
	.frame	$sp,8,$31		# vars= 8, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-8
	sw	$4,8($sp)
	lw	$2,8($sp)
	lbu	$2,0($2)
	sb	$2,0($sp)
	lbu	$2,0($sp)
	sltu	$2,$2,8
	bne	$2,$0,$L334
	nop

	li	$2,-22			# 0xffffffffffffffea
	j	$L335
	nop

$L334:
	lui	$2,%hi(gpWanPriv)
	lw	$3,%lo(gpWanPriv)($2)
	lbu	$2,0($sp)
	sll	$2,$2,5
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	li	$3,65536			# 0x10000
	addu	$2,$3,$2
	lw	$2,-32644($2)
	ext	$2,$2,8,1
	andi	$2,$2,0x00ff
	beq	$2,$0,$L336
	nop

	lw	$2,8($sp)
	lw	$3,4($2)
	li	$2,1			# 0x1
	bne	$3,$2,$L337
	nop

	lui	$2,%hi(gpWanPriv)
	lw	$3,%lo(gpWanPriv)($2)
	lbu	$2,0($sp)
	sll	$2,$2,5
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	li	$3,65536			# 0x10000
	addu	$3,$3,$2
	lw	$2,-32644($3)
	li	$4,1			# 0x1
	ins	$2,$4,11,1
	sw	$2,-32644($3)
	lui	$2,%hi(gpWanPriv)
	lw	$3,%lo(gpWanPriv)($2)
	lbu	$2,0($sp)
	sll	$2,$2,5
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	li	$3,65536			# 0x10000
	addu	$3,$3,$2
	lw	$2,-32644($3)
	ins	$2,$0,10,1
	sw	$2,-32644($3)
	j	$L340
	nop

$L337:
	lw	$2,8($sp)
	lw	$3,4($2)
	li	$2,2			# 0x2
	bne	$3,$2,$L339
	nop

	lui	$2,%hi(gpWanPriv)
	lw	$3,%lo(gpWanPriv)($2)
	lbu	$2,0($sp)
	sll	$2,$2,5
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	li	$3,65536			# 0x10000
	addu	$3,$3,$2
	lw	$2,-32644($3)
	ins	$2,$0,11,1
	sw	$2,-32644($3)
	lui	$2,%hi(gpWanPriv)
	lw	$3,%lo(gpWanPriv)($2)
	lbu	$2,0($sp)
	sll	$2,$2,5
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	li	$3,65536			# 0x10000
	addu	$3,$3,$2
	lw	$2,-32644($3)
	li	$4,1			# 0x1
	ins	$2,$4,10,1
	sw	$2,-32644($3)
	j	$L340
	nop

$L339:
	lui	$2,%hi(gpWanPriv)
	lw	$3,%lo(gpWanPriv)($2)
	lbu	$2,0($sp)
	sll	$2,$2,5
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	li	$3,65536			# 0x10000
	addu	$3,$3,$2
	lw	$2,-32644($3)
	ins	$2,$0,11,1
	sw	$2,-32644($3)
	lui	$2,%hi(gpWanPriv)
	lw	$3,%lo(gpWanPriv)($2)
	lbu	$2,0($sp)
	sll	$2,$2,5
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	li	$3,65536			# 0x10000
	addu	$3,$3,$2
	lw	$2,-32644($3)
	ins	$2,$0,10,1
	sw	$2,-32644($3)
	j	$L340
	nop

$L336:
	li	$2,-2			# 0xfffffffffffffffe
	j	$L335
	nop

$L340:
	move	$2,$0
$L335:
	addiu	$sp,$sp,8
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	xmcs_set_epon_rx_config
	.size	xmcs_set_epon_rx_config, .-xmcs_set_epon_rx_config
	.section	.text.xmcs_set_epon_tx_config,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	xmcs_set_epon_tx_config
	.type	xmcs_set_epon_tx_config, @function
xmcs_set_epon_tx_config:
	.frame	$sp,8,$31		# vars= 8, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-8
	sw	$4,8($sp)
	lw	$2,8($sp)
	lbu	$2,0($2)
	sb	$2,0($sp)
	lbu	$2,0($sp)
	sltu	$2,$2,8
	bne	$2,$0,$L342
	nop

	li	$2,-22			# 0xffffffffffffffea
	j	$L343
	nop

$L342:
	lui	$2,%hi(gpWanPriv)
	lw	$3,%lo(gpWanPriv)($2)
	lbu	$2,0($sp)
	sll	$2,$2,5
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	li	$3,65536			# 0x10000
	addu	$2,$3,$2
	lw	$2,-32644($2)
	ext	$2,$2,8,1
	andi	$2,$2,0x00ff
	beq	$2,$0,$L344
	nop

	lw	$2,8($sp)
	lw	$3,4($2)
	li	$2,1			# 0x1
	bne	$3,$2,$L345
	nop

	lui	$2,%hi(gpWanPriv)
	lw	$3,%lo(gpWanPriv)($2)
	lbu	$2,0($sp)
	sll	$2,$2,5
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	li	$3,65536			# 0x10000
	addu	$3,$3,$2
	lw	$2,-32644($3)
	li	$4,1			# 0x1
	ins	$2,$4,9,1
	sw	$2,-32644($3)
	j	$L347
	nop

$L345:
	lui	$2,%hi(gpWanPriv)
	lw	$3,%lo(gpWanPriv)($2)
	lbu	$2,0($sp)
	sll	$2,$2,5
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	li	$3,65536			# 0x10000
	addu	$3,$3,$2
	lw	$2,-32644($3)
	ins	$2,$0,9,1
	sw	$2,-32644($3)
	j	$L347
	nop

$L344:
	li	$2,-2			# 0xfffffffffffffffe
	j	$L343
	nop

$L347:
	move	$2,$0
$L343:
	addiu	$sp,$sp,8
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	xmcs_set_epon_tx_config
	.size	xmcs_set_epon_tx_config, .-xmcs_set_epon_tx_config
	.rdata
	.align	2
$LC9:
	.ascii	"xmcs set llid %d txmod %d rxmod %d\012\000"
	.section	.text.xmcs_set_epon_llid_config,"ax",@progbits
	.align	2
	.globl	xmcs_set_epon_llid_config
	.set	nomips16
	.set	nomicromips
	.ent	xmcs_set_epon_llid_config
	.type	xmcs_set_epon_llid_config, @function
xmcs_set_epon_llid_config:
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
	sw	$0,24($sp)
	sw	$0,28($sp)
	lw	$2,40($sp)
	andi	$2,$2,0x00ff
	sb	$2,16($sp)
	sw	$0,20($sp)
	addiu	$2,$sp,16
	move	$4,$2
	lui	$2,%hi(xmcs_set_epon_tx_config)
	addiu	$2,$2,%lo(xmcs_set_epon_tx_config)
	jalr	$2
	nop

	lw	$2,40($sp)
	andi	$2,$2,0x00ff
	sb	$2,24($sp)
	sw	$0,28($sp)
	addiu	$2,$sp,24
	move	$4,$2
	lui	$2,%hi(xmcs_set_epon_rx_config)
	addiu	$2,$2,%lo(xmcs_set_epon_rx_config)
	jalr	$2
	nop

	lw	$6,20($sp)
	lw	$3,28($sp)
	lui	$2,%hi($LC9)
	addiu	$4,$2,%lo($LC9)
	lw	$5,40($sp)
	move	$7,$3
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
	.end	xmcs_set_epon_llid_config
	.size	xmcs_set_epon_llid_config, .-xmcs_set_epon_llid_config
	.section	.text.xmcs_get_llid_info,"ax",@progbits
	.align	2
	.globl	xmcs_get_llid_info
	.set	nomips16
	.set	nomicromips
	.ent	xmcs_get_llid_info
	.type	xmcs_get_llid_info, @function
xmcs_get_llid_info:
	.frame	$sp,8,$31		# vars= 8, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-8
	sw	$4,8($sp)
	lw	$2,8($sp)
	sh	$0,128($2)
	sw	$0,0($sp)
	j	$L351
	nop

$L358:
	lui	$2,%hi(gpWanPriv)
	lw	$3,%lo(gpWanPriv)($2)
	lw	$2,8($sp)
	lhu	$2,128($2)
	sll	$2,$2,5
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	li	$3,65536			# 0x10000
	addu	$2,$3,$2
	lw	$2,-32644($2)
	ext	$2,$2,8,1
	andi	$2,$2,0x00ff
	beq	$2,$0,$L352
	nop

	lw	$2,8($sp)
	lhu	$2,128($2)
	lw	$3,0($sp)
	andi	$3,$3,0x00ff
	lw	$4,8($sp)
	sll	$2,$2,4
	addu	$2,$4,$2
	sb	$3,0($2)
	lw	$2,8($sp)
	lhu	$2,128($2)
	move	$5,$2
	lui	$2,%hi(gpWanPriv)
	lw	$3,%lo(gpWanPriv)($2)
	lw	$2,0($sp)
	sll	$2,$2,5
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	li	$3,65536			# 0x10000
	addu	$2,$3,$2
	lhu	$3,-32644($2)
	lw	$4,8($sp)
	sll	$2,$5,4
	addu	$2,$4,$2
	sh	$3,2($2)
	lw	$2,8($sp)
	lhu	$2,128($2)
	move	$5,$2
	lui	$2,%hi(gpWanPriv)
	lw	$3,%lo(gpWanPriv)($2)
	lw	$2,0($sp)
	sll	$2,$2,5
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	li	$3,65536			# 0x10000
	addu	$2,$3,$2
	lw	$2,-32644($2)
	ext	$2,$2,12,4
	andi	$2,$2,0x00ff
	move	$4,$2
	lw	$3,8($sp)
	sll	$2,$5,4
	addu	$2,$3,$2
	sb	$4,4($2)
	lui	$2,%hi(gpWanPriv)
	lw	$3,%lo(gpWanPriv)($2)
	lw	$2,0($sp)
	sll	$2,$2,5
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	li	$3,65536			# 0x10000
	addu	$2,$3,$2
	lw	$2,-32644($2)
	ext	$2,$2,11,1
	andi	$2,$2,0x00ff
	beq	$2,$0,$L353
	nop

	lw	$2,8($sp)
	lhu	$2,128($2)
	lw	$3,8($sp)
	sll	$2,$2,4
	addu	$2,$3,$2
	li	$3,1			# 0x1
	sw	$3,8($2)
	j	$L354
	nop

$L353:
	lui	$2,%hi(gpWanPriv)
	lw	$3,%lo(gpWanPriv)($2)
	lw	$2,0($sp)
	sll	$2,$2,5
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	li	$3,65536			# 0x10000
	addu	$2,$3,$2
	lw	$2,-32644($2)
	ext	$2,$2,10,1
	andi	$2,$2,0x00ff
	beq	$2,$0,$L355
	nop

	lw	$2,8($sp)
	lhu	$2,128($2)
	lw	$3,8($sp)
	sll	$2,$2,4
	addu	$2,$3,$2
	li	$3,2			# 0x2
	sw	$3,8($2)
	j	$L354
	nop

$L355:
	lw	$2,8($sp)
	lhu	$2,128($2)
	lw	$3,8($sp)
	sll	$2,$2,4
	addu	$2,$3,$2
	sw	$0,8($2)
$L354:
	lui	$2,%hi(gpWanPriv)
	lw	$3,%lo(gpWanPriv)($2)
	lw	$2,0($sp)
	sll	$2,$2,5
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	li	$3,65536			# 0x10000
	addu	$2,$3,$2
	lw	$2,-32644($2)
	ext	$2,$2,9,1
	andi	$2,$2,0x00ff
	beq	$2,$0,$L356
	nop

	lw	$2,8($sp)
	lhu	$2,128($2)
	lw	$3,8($sp)
	sll	$2,$2,4
	addu	$2,$3,$2
	li	$3,1			# 0x1
	sw	$3,12($2)
	j	$L357
	nop

$L356:
	lw	$2,8($sp)
	lhu	$2,128($2)
	lw	$3,8($sp)
	sll	$2,$2,4
	addu	$2,$3,$2
	sw	$0,12($2)
$L357:
	lw	$2,8($sp)
	lhu	$2,128($2)
	addiu	$2,$2,1
	andi	$3,$2,0xffff
	lw	$2,8($sp)
	sh	$3,128($2)
$L352:
	lw	$2,0($sp)
	addiu	$2,$2,1
	sw	$2,0($sp)
$L351:
	lw	$2,0($sp)
	slt	$2,$2,8
	bne	$2,$0,$L358
	nop

	move	$2,$0
	addiu	$sp,$sp,8
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	xmcs_get_llid_info
	.size	xmcs_get_llid_info, .-xmcs_get_llid_info
	.section	__ksymtab_strings
	.type	__kstrtab_xmcs_get_llid_info, @object
	.size	__kstrtab_xmcs_get_llid_info, 19
__kstrtab_xmcs_get_llid_info:
	.ascii	"xmcs_get_llid_info\000"
	.section	__ksymtab
	.align	2
	.type	__ksymtab_xmcs_get_llid_info, @object
	.size	__ksymtab_xmcs_get_llid_info, 8
__ksymtab_xmcs_get_llid_info:
	.word	xmcs_get_llid_info
	.word	__kstrtab_xmcs_get_llid_info
	.rdata
	.align	2
$LC10:
	.ascii	"xmcs_set_storm_ctrl_config: ----- threld = %d\015\012\000"
	.align	2
$LC11:
	.ascii	"xmcs_set_storm_ctrl_config: ----- timer = %d\015\012\000"
	.section	.text.xmcs_set_storm_ctrl_config,"ax",@progbits
	.align	2
	.globl	xmcs_set_storm_ctrl_config
	.set	nomips16
	.set	nomicromips
	.ent	xmcs_set_storm_ctrl_config
	.type	xmcs_set_storm_ctrl_config, @function
xmcs_set_storm_ctrl_config:
	.frame	$sp,32,$31		# vars= 8, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-32
	sw	$31,28($sp)
	sw	$4,32($sp)
	lw	$2,32($sp)
	lw	$2,4($2)
	sw	$2,16($sp)
	lw	$2,32($sp)
	lw	$2,8($2)
	sw	$2,20($sp)
	lw	$2,16($sp)
	sltu	$2,$2,129
	bne	$2,$0,$L361
	nop

	li	$2,-22			# 0xffffffffffffffea
	j	$L362
	nop

$L361:
	lui	$2,%hi($LC10)
	addiu	$4,$2,%lo($LC10)
	lw	$5,16($sp)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi($LC11)
	addiu	$4,$2,%lo($LC11)
	lw	$5,20($sp)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lw	$3,16($sp)
	lui	$2,%hi(storm_ctrl_shrehold_wan)
	sw	$3,%lo(storm_ctrl_shrehold_wan)($2)
	lw	$3,20($sp)
	lui	$2,%hi(qdma_wan_fwd_timer)
	sw	$3,%lo(qdma_wan_fwd_timer)($2)
	move	$2,$0
$L362:
	lw	$31,28($sp)
	addiu	$sp,$sp,32
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	xmcs_set_storm_ctrl_config
	.size	xmcs_set_storm_ctrl_config, .-xmcs_set_storm_ctrl_config
	.section	__ksymtab_strings
	.type	__kstrtab_xmcs_set_storm_ctrl_config, @object
	.size	__kstrtab_xmcs_set_storm_ctrl_config, 27
__kstrtab_xmcs_set_storm_ctrl_config:
	.ascii	"xmcs_set_storm_ctrl_config\000"
	.section	__ksymtab
	.align	2
	.type	__ksymtab_xmcs_set_storm_ctrl_config, @object
	.size	__ksymtab_xmcs_set_storm_ctrl_config, 8
__ksymtab_xmcs_set_storm_ctrl_config:
	.word	xmcs_set_storm_ctrl_config
	.word	__kstrtab_xmcs_set_storm_ctrl_config
	.section	.text.xmcs_set_dbg_level,"ax",@progbits
	.align	2
	.globl	xmcs_set_dbg_level
	.set	nomips16
	.set	nomicromips
	.ent	xmcs_set_dbg_level
	.type	xmcs_set_dbg_level, @function
xmcs_set_dbg_level:
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	sw	$4,0($sp)
	lw	$2,0($sp)
	lw	$3,4($2)
	li	$2,1			# 0x1
	bne	$3,$2,$L364
	nop

	lui	$2,%hi(gpPonSysData)
	lw	$4,%lo(gpPonSysData)($2)
	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	lhu	$3,-30816($2)
	lw	$2,0($sp)
	lw	$2,0($2)
	andi	$2,$2,0xffff
	or	$2,$3,$2
	andi	$3,$2,0xffff
	li	$2,65536			# 0x10000
	addu	$2,$4,$2
	sh	$3,-30816($2)
	j	$L365
	nop

$L364:
	lw	$2,0($sp)
	lw	$2,4($2)
	bne	$2,$0,$L366
	nop

	lui	$2,%hi(gpPonSysData)
	lw	$4,%lo(gpPonSysData)($2)
	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	lhu	$3,-30816($2)
	lw	$2,0($sp)
	lw	$2,0($2)
	andi	$2,$2,0xffff
	nor	$2,$0,$2
	andi	$2,$2,0xffff
	and	$2,$3,$2
	andi	$3,$2,0xffff
	li	$2,65536			# 0x10000
	addu	$2,$4,$2
	sh	$3,-30816($2)
	j	$L365
	nop

$L366:
	li	$2,-1			# 0xffffffffffffffff
	j	$L367
	nop

$L365:
	move	$2,$0
$L367:
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	xmcs_set_dbg_level
	.size	xmcs_set_dbg_level, .-xmcs_set_dbg_level
	.rdata
	.align	2
$LC12:
	.ascii	"xmcs_get_storm_ctrl_config: ----- storm_ctrl_shrehold = "
	.ascii	"%d\015\012\000"
	.align	2
$LC13:
	.ascii	"xmcs_get_storm_ctrl_config: ----- qdma_fwd_timer = %d\015"
	.ascii	"\012\000"
	.section	.text.xmcs_get_storm_ctrl_config,"ax",@progbits
	.align	2
	.globl	xmcs_get_storm_ctrl_config
	.set	nomips16
	.set	nomicromips
	.ent	xmcs_get_storm_ctrl_config
	.type	xmcs_get_storm_ctrl_config, @function
xmcs_get_storm_ctrl_config:
	.frame	$sp,24,$31		# vars= 0, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-24
	sw	$31,20($sp)
	sw	$4,24($sp)
	lui	$2,%hi(storm_ctrl_shrehold_wan)
	lw	$3,%lo(storm_ctrl_shrehold_wan)($2)
	lui	$2,%hi($LC12)
	addiu	$4,$2,%lo($LC12)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi(qdma_wan_fwd_timer)
	lw	$3,%lo(qdma_wan_fwd_timer)($2)
	lui	$2,%hi($LC13)
	addiu	$4,$2,%lo($LC13)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi(storm_ctrl_shrehold_wan)
	lw	$2,%lo(storm_ctrl_shrehold_wan)($2)
	move	$3,$2
	lw	$2,24($sp)
	sw	$3,4($2)
	lui	$2,%hi(qdma_wan_fwd_timer)
	lw	$2,%lo(qdma_wan_fwd_timer)($2)
	move	$3,$2
	lw	$2,24($sp)
	sw	$3,8($2)
	move	$2,$0
	lw	$31,20($sp)
	addiu	$sp,$sp,24
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	xmcs_get_storm_ctrl_config
	.size	xmcs_get_storm_ctrl_config, .-xmcs_get_storm_ctrl_config
	.section	__ksymtab_strings
	.type	__kstrtab_xmcs_get_storm_ctrl_config, @object
	.size	__kstrtab_xmcs_get_storm_ctrl_config, 27
__kstrtab_xmcs_get_storm_ctrl_config:
	.ascii	"xmcs_get_storm_ctrl_config\000"
	.section	__ksymtab
	.align	2
	.type	__ksymtab_xmcs_get_storm_ctrl_config, @object
	.size	__ksymtab_xmcs_get_storm_ctrl_config, 8
__ksymtab_xmcs_get_storm_ctrl_config:
	.word	xmcs_get_storm_ctrl_config
	.word	__kstrtab_xmcs_get_storm_ctrl_config
	.section	.text.xmcs_get_dbg_level,"ax",@progbits
	.align	2
	.globl	xmcs_get_dbg_level
	.set	nomips16
	.set	nomicromips
	.ent	xmcs_get_dbg_level
	.type	xmcs_get_dbg_level, @function
xmcs_get_dbg_level:
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	sw	$4,0($sp)
	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	lhu	$3,-30816($2)
	lw	$2,0($sp)
	sh	$3,0($2)
	move	$2,$0
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	xmcs_get_dbg_level
	.size	xmcs_get_dbg_level, .-xmcs_get_dbg_level
	.rdata
	.align	2
$LC14:
	.ascii	"[%lu0ms]No such I/O command, cmd: %x\012\000"
	.section	.text.if_cmd_proc,"ax",@progbits
	.align	2
	.globl	if_cmd_proc
	.set	nomips16
	.set	nomicromips
	.ent	if_cmd_proc
	.type	if_cmd_proc, @function
if_cmd_proc:
	.frame	$sp,32,$31		# vars= 8, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-32
	sw	$31,28($sp)
	sw	$4,32($sp)
	sw	$5,36($sp)
	li	$2,-22			# 0xffffffffffffffea
	sw	$2,16($sp)
	lw	$2,32($sp)
	srl	$3,$2,29
	li	$2,4			# 0x4
	bne	$3,$2,$L373
	nop

	lw	$3,32($sp)
	li	$2,2147155968			# 0x7ffb0000
	ori	$2,$2,0x25ff
	addu	$2,$3,$2
	sltu	$3,$2,132
	beq	$3,$0,$L374
	nop

	sll	$3,$2,2
	lui	$2,%hi($L376)
	addiu	$2,$2,%lo($L376)
	addu	$2,$3,$2
	lw	$2,0($2)
	j	$2
	nop

	.rdata
	.align	2
	.align	2
$L376:
	.word	$L375
	.word	$L377
	.word	$L378
	.word	$L379
	.word	$L380
	.word	$L381
	.word	$L374
	.word	$L382
	.word	$L374
	.word	$L383
	.word	$L384
	.word	$L385
	.word	$L374
	.word	$L374
	.word	$L374
	.word	$L374
	.word	$L374
	.word	$L374
	.word	$L374
	.word	$L374
	.word	$L374
	.word	$L374
	.word	$L374
	.word	$L374
	.word	$L374
	.word	$L374
	.word	$L374
	.word	$L374
	.word	$L374
	.word	$L374
	.word	$L374
	.word	$L386
	.word	$L387
	.word	$L374
	.word	$L374
	.word	$L388
	.word	$L389
	.word	$L374
	.word	$L374
	.word	$L374
	.word	$L390
	.word	$L374
	.word	$L374
	.word	$L374
	.word	$L374
	.word	$L374
	.word	$L374
	.word	$L374
	.word	$L374
	.word	$L374
	.word	$L374
	.word	$L374
	.word	$L374
	.word	$L374
	.word	$L374
	.word	$L374
	.word	$L374
	.word	$L374
	.word	$L374
	.word	$L374
	.word	$L374
	.word	$L374
	.word	$L374
	.word	$L374
	.word	$L374
	.word	$L391
	.word	$L392
	.word	$L393
	.word	$L394
	.word	$L395
	.word	$L396
	.word	$L374
	.word	$L374
	.word	$L374
	.word	$L397
	.word	$L374
	.word	$L374
	.word	$L374
	.word	$L374
	.word	$L374
	.word	$L374
	.word	$L374
	.word	$L374
	.word	$L374
	.word	$L374
	.word	$L374
	.word	$L374
	.word	$L374
	.word	$L374
	.word	$L374
	.word	$L374
	.word	$L374
	.word	$L374
	.word	$L374
	.word	$L374
	.word	$L374
	.word	$L374
	.word	$L374
	.word	$L374
	.word	$L374
	.word	$L374
	.word	$L374
	.word	$L374
	.word	$L374
	.word	$L374
	.word	$L374
	.word	$L374
	.word	$L374
	.word	$L374
	.word	$L374
	.word	$L374
	.word	$L374
	.word	$L374
	.word	$L374
	.word	$L374
	.word	$L374
	.word	$L374
	.word	$L374
	.word	$L374
	.word	$L374
	.word	$L374
	.word	$L374
	.word	$L374
	.word	$L374
	.word	$L374
	.word	$L374
	.word	$L374
	.word	$L374
	.word	$L398
	.word	$L399
	.word	$L400
	.word	$L401
	.section	.text.if_cmd_proc
$L375:
	lw	$2,36($sp)
	move	$4,$2
	lui	$2,%hi(xmcs_set_qos_weight_config)
	addiu	$2,$2,%lo(xmcs_set_qos_weight_config)
	jalr	$2
	nop

	sw	$2,16($sp)
	j	$L402
	nop

$L377:
	lw	$2,36($sp)
	move	$4,$2
	lui	$2,%hi(xmcs_set_channel_scheduler)
	addiu	$2,$2,%lo(xmcs_set_channel_scheduler)
	jalr	$2
	nop

	sw	$2,16($sp)
	j	$L402
	nop

$L378:
	lw	$2,36($sp)
	move	$4,$2
	lui	$2,%hi(xmcs_set_trtcm_congest_mode)
	addiu	$2,$2,%lo(xmcs_set_trtcm_congest_mode)
	jalr	$2
	nop

	sw	$2,16($sp)
	j	$L402
	nop

$L379:
	lw	$2,36($sp)
	move	$4,$2
	lui	$2,%hi(xmcs_set_dei_congest_mode)
	addiu	$2,$2,%lo(xmcs_set_dei_congest_mode)
	jalr	$2
	nop

	sw	$2,16($sp)
	j	$L402
	nop

$L380:
	lw	$2,36($sp)
	move	$4,$2
	lui	$2,%hi(xmcs_set_threshold_congest_mode)
	addiu	$2,$2,%lo(xmcs_set_threshold_congest_mode)
	jalr	$2
	nop

	sw	$2,16($sp)
	j	$L402
	nop

$L381:
	lw	$2,36($sp)
	move	$4,$2
	lui	$2,%hi(xmcs_set_congest_scale)
	addiu	$2,$2,%lo(xmcs_set_congest_scale)
	jalr	$2
	nop

	sw	$2,16($sp)
	j	$L402
	nop

$L382:
	lw	$2,36($sp)
	move	$4,$2
	lui	$2,%hi(xmcs_set_congest_threshold)
	addiu	$2,$2,%lo(xmcs_set_congest_threshold)
	jalr	$2
	nop

	sw	$2,16($sp)
	j	$L402
	nop

$L383:
	lw	$2,36($sp)
	move	$4,$2
	lui	$2,%hi(xmcs_set_tx_trtcm_scale)
	addiu	$2,$2,%lo(xmcs_set_tx_trtcm_scale)
	jalr	$2
	nop

	sw	$2,16($sp)
	j	$L402
	nop

$L384:
	lw	$2,36($sp)
	move	$4,$2
	lui	$2,%hi(xmcs_set_tx_trtcm_params)
	addiu	$2,$2,%lo(xmcs_set_tx_trtcm_params)
	jalr	$2
	nop

	sw	$2,16($sp)
	j	$L402
	nop

$L385:
	lw	$2,36($sp)
	move	$4,$2
	lui	$2,%hi(xmcs_set_pcp_config)
	addiu	$2,$2,%lo(xmcs_set_pcp_config)
	jalr	$2
	nop

	sw	$2,16($sp)
	j	$L402
	nop

$L386:
	lw	$4,36($sp)
	lui	$2,%hi(xmcs_set_connection_start)
	addiu	$2,$2,%lo(xmcs_set_connection_start)
	jalr	$2
	nop

	sw	$2,16($sp)
	j	$L402
	nop

$L387:
	lw	$4,36($sp)
	lui	$2,%hi(xmcs_set_link_detection)
	addiu	$2,$2,%lo(xmcs_set_link_detection)
	jalr	$2
	nop

	sw	$2,16($sp)
	j	$L402
	nop

$L388:
	lw	$4,36($sp)
	lui	$2,%hi(xmcs_set_wan_cnt_stats)
	addiu	$2,$2,%lo(xmcs_set_wan_cnt_stats)
	jalr	$2
	nop

	sw	$2,16($sp)
	j	$L402
	nop

$L389:
	lw	$2,36($sp)
	move	$4,$2
	lui	$2,%hi(xmcs_set_storm_ctrl_config)
	addiu	$2,$2,%lo(xmcs_set_storm_ctrl_config)
	jalr	$2
	nop

	sw	$2,16($sp)
	j	$L402
	nop

$L390:
	lw	$2,36($sp)
	move	$4,$2
	lui	$2,%hi(xmcs_set_dbg_level)
	addiu	$2,$2,%lo(xmcs_set_dbg_level)
	jalr	$2
	nop

	sw	$2,16($sp)
	j	$L402
	nop

$L391:
	lw	$2,36($sp)
	move	$4,$2
	lui	$2,%hi(xmcs_assign_gem_port)
	addiu	$2,$2,%lo(xmcs_assign_gem_port)
	jalr	$2
	nop

	sw	$2,16($sp)
	j	$L402
	nop

$L392:
	lw	$2,36($sp)
	move	$4,$2
	lui	$2,%hi(xmcs_unassign_gem_port)
	addiu	$2,$2,%lo(xmcs_unassign_gem_port)
	jalr	$2
	nop

	sw	$2,16($sp)
	j	$L402
	nop

$L393:
	lw	$2,36($sp)
	move	$4,$2
	lui	$2,%hi(xmcs_create_gem_port)
	addiu	$2,$2,%lo(xmcs_create_gem_port)
	jalr	$2
	nop

	sw	$2,16($sp)
	j	$L402
	nop

$L394:
	lw	$2,36($sp)
	andi	$2,$2,0xffff
	move	$4,$2
	lui	$2,%hi(xmcs_remove_gem_port)
	addiu	$2,$2,%lo(xmcs_remove_gem_port)
	jalr	$2
	nop

	sw	$2,16($sp)
	j	$L402
	nop

$L395:
	lui	$2,%hi(xmcs_remove_all_gem_port)
	addiu	$2,$2,%lo(xmcs_remove_all_gem_port)
	jalr	$2
	nop

	sw	$2,16($sp)
	j	$L402
	nop

$L396:
	lw	$2,36($sp)
	move	$4,$2
	lui	$2,%hi(xmcs_set_gem_port_loopback)
	addiu	$2,$2,%lo(xmcs_set_gem_port_loopback)
	jalr	$2
	nop

	sw	$2,16($sp)
	j	$L402
	nop

$L397:
	lw	$2,36($sp)
	andi	$2,$2,0xffff
	move	$4,$2
	lui	$2,%hi(xmcs_remove_tcont_info)
	addiu	$2,$2,%lo(xmcs_remove_tcont_info)
	jalr	$2
	nop

	sw	$2,16($sp)
	j	$L402
	nop

$L398:
	lw	$2,36($sp)
	move	$4,$2
	lui	$2,%hi(xmcs_create_llid)
	addiu	$2,$2,%lo(xmcs_create_llid)
	jalr	$2
	nop

	sw	$2,16($sp)
	j	$L402
	nop

$L399:
	lw	$2,36($sp)
	andi	$2,$2,0x00ff
	move	$4,$2
	lui	$2,%hi(xmcs_remove_llid)
	addiu	$2,$2,%lo(xmcs_remove_llid)
	jalr	$2
	nop

	sw	$2,16($sp)
	j	$L402
	nop

$L400:
	lw	$2,36($sp)
	move	$4,$2
	lui	$2,%hi(xmcs_set_epon_rx_config)
	addiu	$2,$2,%lo(xmcs_set_epon_rx_config)
	jalr	$2
	nop

	sw	$2,16($sp)
	j	$L402
	nop

$L401:
	lw	$2,36($sp)
	move	$4,$2
	lui	$2,%hi(xmcs_set_epon_tx_config)
	addiu	$2,$2,%lo(xmcs_set_epon_tx_config)
	jalr	$2
	nop

	sw	$2,16($sp)
	j	$L402
	nop

$L374:
	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	lhu	$2,-30816($2)
	andi	$2,$2,0x1
	beq	$2,$0,$L403
	nop

	lui	$2,%hi(jiffies)
	lw	$3,%lo(jiffies)($2)
	lui	$2,%hi($LC14)
	addiu	$4,$2,%lo($LC14)
	move	$5,$3
	lw	$6,32($sp)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	j	$L425
	nop

$L403:
$L425:
	nop
$L402:
	j	$L404
	nop

$L373:
	lw	$2,32($sp)
	srl	$3,$2,29
	li	$2,2			# 0x2
	bne	$3,$2,$L405
	nop

	lw	$3,32($sp)
	li	$2,-1074069504			# 0xffffffffbffb0000
	ori	$2,$2,0x25ff
	addu	$2,$3,$2
	sltu	$3,$2,133
	beq	$3,$0,$L406
	nop

	sll	$3,$2,2
	lui	$2,%hi($L408)
	addiu	$2,$2,%lo($L408)
	addu	$2,$3,$2
	lw	$2,0($2)
	j	$2
	nop

	.rdata
	.align	2
	.align	2
$L408:
	.word	$L407
	.word	$L409
	.word	$L406
	.word	$L406
	.word	$L406
	.word	$L406
	.word	$L406
	.word	$L406
	.word	$L410
	.word	$L411
	.word	$L412
	.word	$L413
	.word	$L406
	.word	$L406
	.word	$L406
	.word	$L406
	.word	$L406
	.word	$L406
	.word	$L406
	.word	$L406
	.word	$L406
	.word	$L406
	.word	$L406
	.word	$L406
	.word	$L406
	.word	$L406
	.word	$L406
	.word	$L406
	.word	$L406
	.word	$L406
	.word	$L406
	.word	$L406
	.word	$L406
	.word	$L414
	.word	$L415
	.word	$L416
	.word	$L417
	.word	$L406
	.word	$L406
	.word	$L406
	.word	$L418
	.word	$L406
	.word	$L406
	.word	$L406
	.word	$L406
	.word	$L406
	.word	$L406
	.word	$L406
	.word	$L406
	.word	$L406
	.word	$L406
	.word	$L406
	.word	$L406
	.word	$L406
	.word	$L406
	.word	$L406
	.word	$L406
	.word	$L406
	.word	$L406
	.word	$L406
	.word	$L406
	.word	$L406
	.word	$L406
	.word	$L406
	.word	$L406
	.word	$L406
	.word	$L406
	.word	$L406
	.word	$L406
	.word	$L406
	.word	$L406
	.word	$L419
	.word	$L420
	.word	$L406
	.word	$L406
	.word	$L406
	.word	$L406
	.word	$L406
	.word	$L406
	.word	$L406
	.word	$L406
	.word	$L406
	.word	$L406
	.word	$L406
	.word	$L406
	.word	$L406
	.word	$L406
	.word	$L406
	.word	$L406
	.word	$L406
	.word	$L406
	.word	$L406
	.word	$L406
	.word	$L406
	.word	$L406
	.word	$L406
	.word	$L406
	.word	$L406
	.word	$L406
	.word	$L406
	.word	$L406
	.word	$L406
	.word	$L406
	.word	$L406
	.word	$L406
	.word	$L406
	.word	$L406
	.word	$L406
	.word	$L406
	.word	$L406
	.word	$L406
	.word	$L406
	.word	$L406
	.word	$L406
	.word	$L406
	.word	$L406
	.word	$L406
	.word	$L406
	.word	$L406
	.word	$L406
	.word	$L406
	.word	$L406
	.word	$L406
	.word	$L406
	.word	$L406
	.word	$L406
	.word	$L406
	.word	$L406
	.word	$L406
	.word	$L406
	.word	$L406
	.word	$L406
	.word	$L421
	.section	.text.if_cmd_proc
$L407:
	lw	$2,36($sp)
	move	$4,$2
	lui	$2,%hi(xmcs_get_qos_weight_config)
	addiu	$2,$2,%lo(xmcs_get_qos_weight_config)
	jalr	$2
	nop

	sw	$2,16($sp)
	j	$L422
	nop

$L409:
	lw	$2,36($sp)
	move	$4,$2
	lui	$2,%hi(xmcs_get_channel_scheduler)
	addiu	$2,$2,%lo(xmcs_get_channel_scheduler)
	jalr	$2
	nop

	sw	$2,16($sp)
	j	$L422
	nop

$L410:
	lw	$2,36($sp)
	move	$4,$2
	lui	$2,%hi(xmcs_get_congest_info)
	addiu	$2,$2,%lo(xmcs_get_congest_info)
	jalr	$2
	nop

	sw	$2,16($sp)
	j	$L422
	nop

$L411:
	lw	$2,36($sp)
	move	$4,$2
	lui	$2,%hi(xmcs_get_tx_trtcm_scale)
	addiu	$2,$2,%lo(xmcs_get_tx_trtcm_scale)
	jalr	$2
	nop

	sw	$2,16($sp)
	j	$L422
	nop

$L412:
	lw	$2,36($sp)
	move	$4,$2
	lui	$2,%hi(xmcs_get_tx_trtcm_params)
	addiu	$2,$2,%lo(xmcs_get_tx_trtcm_params)
	jalr	$2
	nop

	sw	$2,16($sp)
	j	$L422
	nop

$L413:
	lw	$2,36($sp)
	move	$4,$2
	lui	$2,%hi(xmcs_get_pcp_config)
	addiu	$2,$2,%lo(xmcs_get_pcp_config)
	jalr	$2
	nop

	sw	$2,16($sp)
	j	$L422
	nop

$L414:
	lw	$2,36($sp)
	move	$4,$2
	lui	$2,%hi(xmcs_get_wan_link_status)
	addiu	$2,$2,%lo(xmcs_get_wan_link_status)
	jalr	$2
	nop

	sw	$2,16($sp)
	j	$L422
	nop

$L415:
	lw	$2,36($sp)
	move	$4,$2
	lui	$2,%hi(xmcs_get_onu_type)
	addiu	$2,$2,%lo(xmcs_get_onu_type)
	jalr	$2
	nop

	sw	$2,16($sp)
	j	$L422
	nop

$L416:
	lw	$2,36($sp)
	move	$4,$2
	lui	$2,%hi(xmcs_get_wan_cnt_stats)
	addiu	$2,$2,%lo(xmcs_get_wan_cnt_stats)
	jalr	$2
	nop

	sw	$2,16($sp)
	j	$L422
	nop

$L417:
	lw	$2,36($sp)
	move	$4,$2
	lui	$2,%hi(xmcs_get_storm_ctrl_config)
	addiu	$2,$2,%lo(xmcs_get_storm_ctrl_config)
	jalr	$2
	nop

	sw	$2,16($sp)
	j	$L422
	nop

$L418:
	lw	$2,36($sp)
	move	$4,$2
	lui	$2,%hi(xmcs_get_dbg_level)
	addiu	$2,$2,%lo(xmcs_get_dbg_level)
	jalr	$2
	nop

	sw	$2,16($sp)
	j	$L422
	nop

$L419:
	lw	$2,36($sp)
	move	$4,$2
	lui	$2,%hi(xmcs_get_gem_port_info)
	addiu	$2,$2,%lo(xmcs_get_gem_port_info)
	jalr	$2
	nop

	sw	$2,16($sp)
	j	$L422
	nop

$L420:
	lw	$2,36($sp)
	move	$4,$2
	lui	$2,%hi(xmcs_get_tcont_info)
	addiu	$2,$2,%lo(xmcs_get_tcont_info)
	jalr	$2
	nop

	sw	$2,16($sp)
	j	$L422
	nop

$L421:
	lw	$2,36($sp)
	move	$4,$2
	lui	$2,%hi(xmcs_get_llid_info)
	addiu	$2,$2,%lo(xmcs_get_llid_info)
	jalr	$2
	nop

	sw	$2,16($sp)
	j	$L422
	nop

$L406:
	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	lhu	$2,-30816($2)
	andi	$2,$2,0x1
	beq	$2,$0,$L423
	nop

	lui	$2,%hi(jiffies)
	lw	$3,%lo(jiffies)($2)
	lui	$2,%hi($LC14)
	addiu	$4,$2,%lo($LC14)
	move	$5,$3
	lw	$6,32($sp)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	j	$L426
	nop

$L423:
$L426:
	nop
$L422:
	j	$L404
	nop

$L405:
	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	lhu	$2,-30816($2)
	andi	$2,$2,0x1
	beq	$2,$0,$L404
	nop

	lui	$2,%hi(jiffies)
	lw	$3,%lo(jiffies)($2)
	lui	$2,%hi($LC14)
	addiu	$4,$2,%lo($LC14)
	move	$5,$3
	lw	$6,32($sp)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L404:
	lw	$2,16($sp)
	lw	$31,28($sp)
	addiu	$sp,$sp,32
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	if_cmd_proc
	.size	if_cmd_proc, .-if_cmd_proc
	.rdata
	.align	2
	.type	__func__.33634, @object
	.size	__func__.33634, 26
__func__.33634:
	.ascii	"xmcs_set_connection_start\000"
	.align	2
	.type	__func__.33655, @object
	.size	__func__.33655, 24
__func__.33655:
	.ascii	"xmcs_set_link_detection\000"
	.ident	"GCC: (Buildroot 2015.08.1) 4.9.3"
