	.file	1 "gpon_qos.c"
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
	.section	.text.gpon_save_cpu_protection,"ax",@progbits
	.align	2
	.globl	gpon_save_cpu_protection
	.set	nomips16
	.set	nomicromips
	.ent	gpon_save_cpu_protection
	.type	gpon_save_cpu_protection, @function
gpon_save_cpu_protection:
	.frame	$sp,216,$31		# vars= 192, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-216
	sw	$31,212($sp)
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	addiu	$2,$2,368
	li	$3,17			# 0x11
	sw	$3,16($sp)
	sw	$2,20($sp)
	sw	$0,24($sp)
	li	$2,48			# 0x30
	sw	$2,148($sp)
	lw	$2,20($sp)
	sw	$2,156($sp)
	addiu	$2,$sp,148
	lw	$4,16($sp)
	move	$5,$0
	move	$6,$2
	lui	$2,%hi(__ECNT_HOOK)
	addiu	$2,$2,%lo(__ECNT_HOOK)
	jalr	$2
	nop

	sw	$2,24($sp)
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	sb	$0,384($2)
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	addiu	$2,$2,384
	li	$3,17			# 0x11
	sw	$3,28($sp)
	sw	$2,32($sp)
	sw	$0,36($sp)
	li	$2,50			# 0x32
	sw	$2,120($sp)
	lw	$2,32($sp)
	sw	$2,128($sp)
	addiu	$2,$sp,120
	lw	$4,28($sp)
	move	$5,$0
	move	$6,$2
	lui	$2,%hi(__ECNT_HOOK)
	addiu	$2,$2,%lo(__ECNT_HOOK)
	jalr	$2
	nop

	sw	$2,36($sp)
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	addiu	$2,$2,344
	li	$3,16			# 0x10
	sw	$3,40($sp)
	sw	$2,44($sp)
	sw	$0,48($sp)
	li	$2,48			# 0x30
	sw	$2,92($sp)
	lw	$2,44($sp)
	sw	$2,100($sp)
	addiu	$2,$sp,92
	lw	$4,40($sp)
	move	$5,$0
	move	$6,$2
	lui	$2,%hi(__ECNT_HOOK)
	addiu	$2,$2,%lo(__ECNT_HOOK)
	jalr	$2
	nop

	sw	$2,48($sp)
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	sb	$0,360($2)
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	addiu	$2,$2,360
	li	$3,16			# 0x10
	sw	$3,52($sp)
	sw	$2,56($sp)
	sw	$0,60($sp)
	li	$2,50			# 0x32
	sw	$2,64($sp)
	lw	$2,56($sp)
	sw	$2,72($sp)
	addiu	$2,$sp,64
	lw	$4,52($sp)
	move	$5,$0
	move	$6,$2
	lui	$2,%hi(__ECNT_HOOK)
	addiu	$2,$2,%lo(__ECNT_HOOK)
	jalr	$2
	nop

	sw	$2,60($sp)
	lw	$31,212($sp)
	addiu	$sp,$sp,216
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	gpon_save_cpu_protection
	.size	gpon_save_cpu_protection, .-gpon_save_cpu_protection
	.section	.text.gpon_enable_cpu_protection,"ax",@progbits
	.align	2
	.globl	gpon_enable_cpu_protection
	.set	nomips16
	.set	nomicromips
	.ent	gpon_enable_cpu_protection
	.type	gpon_enable_cpu_protection, @function
gpon_enable_cpu_protection:
	.frame	$sp,328,$31		# vars= 304, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-328
	sw	$31,324($sp)
	sw	$4,328($sp)
	li	$2,1			# 0x1
	sw	$2,16($sp)
	li	$2,17			# 0x11
	sw	$2,20($sp)
	lw	$2,16($sp)
	sw	$2,24($sp)
	sw	$0,28($sp)
	li	$2,47			# 0x2f
	sw	$2,240($sp)
	lw	$2,24($sp)
	sw	$2,248($sp)
	addiu	$2,$sp,240
	lw	$4,20($sp)
	move	$5,$0
	move	$6,$2
	lui	$2,%hi(__ECNT_HOOK)
	addiu	$2,$2,%lo(__ECNT_HOOK)
	jalr	$2
	nop

	sw	$2,28($sp)
	sb	$0,92($sp)
	lw	$2,328($sp)
	sw	$2,96($sp)
	li	$2,17			# 0x11
	sw	$2,32($sp)
	addiu	$2,$sp,92
	sw	$2,36($sp)
	sw	$0,40($sp)
	li	$2,49			# 0x31
	sw	$2,212($sp)
	lw	$2,36($sp)
	sw	$2,220($sp)
	addiu	$2,$sp,212
	lw	$4,32($sp)
	move	$5,$0
	move	$6,$2
	lui	$2,%hi(__ECNT_HOOK)
	addiu	$2,$2,%lo(__ECNT_HOOK)
	jalr	$2
	nop

	sw	$2,40($sp)
	li	$2,17			# 0x11
	sw	$2,44($sp)
	li	$2,1			# 0x1
	sw	$2,48($sp)
	sw	$0,52($sp)
	li	$2,46			# 0x2e
	sw	$2,184($sp)
	lw	$2,48($sp)
	sw	$2,192($sp)
	addiu	$2,$sp,184
	lw	$4,44($sp)
	move	$5,$0
	move	$6,$2
	lui	$2,%hi(__ECNT_HOOK)
	addiu	$2,$2,%lo(__ECNT_HOOK)
	jalr	$2
	nop

	sw	$2,52($sp)
	li	$2,1			# 0x1
	sw	$2,16($sp)
	li	$2,16			# 0x10
	sw	$2,56($sp)
	lw	$2,16($sp)
	sw	$2,60($sp)
	sw	$0,64($sp)
	li	$2,47			# 0x2f
	sw	$2,156($sp)
	lw	$2,60($sp)
	sw	$2,164($sp)
	addiu	$2,$sp,156
	lw	$4,56($sp)
	move	$5,$0
	move	$6,$2
	lui	$2,%hi(__ECNT_HOOK)
	addiu	$2,$2,%lo(__ECNT_HOOK)
	jalr	$2
	nop

	sw	$2,64($sp)
	sb	$0,92($sp)
	lw	$2,328($sp)
	sw	$2,96($sp)
	li	$2,16			# 0x10
	sw	$2,68($sp)
	addiu	$2,$sp,92
	sw	$2,72($sp)
	sw	$0,76($sp)
	li	$2,49			# 0x31
	sw	$2,128($sp)
	lw	$2,72($sp)
	sw	$2,136($sp)
	addiu	$2,$sp,128
	lw	$4,68($sp)
	move	$5,$0
	move	$6,$2
	lui	$2,%hi(__ECNT_HOOK)
	addiu	$2,$2,%lo(__ECNT_HOOK)
	jalr	$2
	nop

	sw	$2,76($sp)
	li	$2,16			# 0x10
	sw	$2,80($sp)
	li	$2,1			# 0x1
	sw	$2,84($sp)
	sw	$0,88($sp)
	li	$2,46			# 0x2e
	sw	$2,100($sp)
	lw	$2,84($sp)
	sw	$2,108($sp)
	addiu	$2,$sp,100
	lw	$4,80($sp)
	move	$5,$0
	move	$6,$2
	lui	$2,%hi(__ECNT_HOOK)
	addiu	$2,$2,%lo(__ECNT_HOOK)
	jalr	$2
	nop

	sw	$2,88($sp)
	lw	$31,324($sp)
	addiu	$sp,$sp,328
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	gpon_enable_cpu_protection
	.size	gpon_enable_cpu_protection, .-gpon_enable_cpu_protection
	.section	.text.gpon_restore_cpu_protection,"ax",@progbits
	.align	2
	.globl	gpon_restore_cpu_protection
	.set	nomips16
	.set	nomicromips
	.ent	gpon_restore_cpu_protection
	.type	gpon_restore_cpu_protection, @function
gpon_restore_cpu_protection:
	.frame	$sp,312,$31		# vars= 288, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-312
	sw	$31,308($sp)
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	addiu	$2,$2,384
	li	$3,17			# 0x11
	sw	$3,16($sp)
	sw	$2,20($sp)
	sw	$0,24($sp)
	li	$2,49			# 0x31
	sw	$2,228($sp)
	lw	$2,20($sp)
	sw	$2,236($sp)
	addiu	$2,$sp,228
	lw	$4,16($sp)
	move	$5,$0
	move	$6,$2
	lui	$2,%hi(__ECNT_HOOK)
	addiu	$2,$2,%lo(__ECNT_HOOK)
	jalr	$2
	nop

	sw	$2,24($sp)
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lw	$2,372($2)
	li	$3,17			# 0x11
	sw	$3,28($sp)
	sw	$2,32($sp)
	sw	$0,36($sp)
	li	$2,47			# 0x2f
	sw	$2,200($sp)
	lw	$2,32($sp)
	sw	$2,208($sp)
	addiu	$2,$sp,200
	lw	$4,28($sp)
	move	$5,$0
	move	$6,$2
	lui	$2,%hi(__ECNT_HOOK)
	addiu	$2,$2,%lo(__ECNT_HOOK)
	jalr	$2
	nop

	sw	$2,36($sp)
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lbu	$2,368($2)
	move	$3,$2
	li	$2,17			# 0x11
	sw	$2,40($sp)
	sw	$3,44($sp)
	sw	$0,48($sp)
	li	$2,46			# 0x2e
	sw	$2,172($sp)
	lw	$2,44($sp)
	sw	$2,180($sp)
	addiu	$2,$sp,172
	lw	$4,40($sp)
	move	$5,$0
	move	$6,$2
	lui	$2,%hi(__ECNT_HOOK)
	addiu	$2,$2,%lo(__ECNT_HOOK)
	jalr	$2
	nop

	sw	$2,48($sp)
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	addiu	$2,$2,360
	li	$3,16			# 0x10
	sw	$3,52($sp)
	sw	$2,56($sp)
	sw	$0,60($sp)
	li	$2,49			# 0x31
	sw	$2,144($sp)
	lw	$2,56($sp)
	sw	$2,152($sp)
	addiu	$2,$sp,144
	lw	$4,52($sp)
	move	$5,$0
	move	$6,$2
	lui	$2,%hi(__ECNT_HOOK)
	addiu	$2,$2,%lo(__ECNT_HOOK)
	jalr	$2
	nop

	sw	$2,60($sp)
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lw	$2,348($2)
	li	$3,17			# 0x11
	sw	$3,64($sp)
	sw	$2,68($sp)
	sw	$0,72($sp)
	li	$2,47			# 0x2f
	sw	$2,116($sp)
	lw	$2,68($sp)
	sw	$2,124($sp)
	addiu	$2,$sp,116
	lw	$4,64($sp)
	move	$5,$0
	move	$6,$2
	lui	$2,%hi(__ECNT_HOOK)
	addiu	$2,$2,%lo(__ECNT_HOOK)
	jalr	$2
	nop

	sw	$2,72($sp)
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lbu	$2,344($2)
	move	$3,$2
	li	$2,16			# 0x10
	sw	$2,76($sp)
	sw	$3,80($sp)
	sw	$0,84($sp)
	li	$2,46			# 0x2e
	sw	$2,88($sp)
	lw	$2,80($sp)
	sw	$2,96($sp)
	addiu	$2,$sp,88
	lw	$4,76($sp)
	move	$5,$0
	move	$6,$2
	lui	$2,%hi(__ECNT_HOOK)
	addiu	$2,$2,%lo(__ECNT_HOOK)
	jalr	$2
	nop

	sw	$2,84($sp)
	lw	$31,308($sp)
	addiu	$sp,$sp,312
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	gpon_restore_cpu_protection
	.size	gpon_restore_cpu_protection, .-gpon_restore_cpu_protection
	.rdata
	.align	2
$LC0:
	.ascii	"[%lu0ms]failed to set the GPON trTCM parameters.\012\000"
	.section	.text.gpon_qos_init,"ax",@progbits
	.align	2
	.globl	gpon_qos_init
	.set	nomips16
	.set	nomicromips
	.ent	gpon_qos_init
	.type	gpon_qos_init, @function
gpon_qos_init:
	.frame	$sp,96,$31		# vars= 72, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-96
	sw	$31,92($sp)
	sw	$0,16($sp)
	j	$L53
	nop

$L60:
	lw	$2,16($sp)
	andi	$2,$2,0x00ff
	sb	$2,36($sp)
	li	$2,65535			# 0xffff
	sw	$2,40($sp)
	li	$2,32768			# 0x8000
	sw	$2,44($sp)
	li	$2,65535			# 0xffff
	sw	$2,48($sp)
	li	$2,65535			# 0xffff
	sw	$2,52($sp)
	li	$2,16			# 0x10
	sw	$2,24($sp)
	addiu	$2,$sp,36
	sw	$2,28($sp)
	sw	$0,32($sp)
	li	$2,40			# 0x28
	sw	$2,56($sp)
	lw	$2,28($sp)
	sw	$2,64($sp)
	addiu	$2,$sp,56
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
	beq	$3,$2,$L54
	nop

	lw	$2,60($sp)
	j	$L56
	nop

$L54:
	li	$2,-1			# 0xffffffffffffffff
$L56:
	sw	$2,20($sp)
	lw	$2,20($sp)
	bgez	$2,$L57
	nop

	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	lhu	$2,-30816($2)
	andi	$2,$2,0x1
	beq	$2,$0,$L58
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

$L58:
	li	$2,-1			# 0xffffffffffffffff
	j	$L61
	nop

$L57:
	lw	$2,16($sp)
	addiu	$2,$2,1
	sw	$2,16($sp)
$L53:
	lw	$2,16($sp)
	slt	$2,$2,32
	bne	$2,$0,$L60
	nop

	move	$2,$0
$L61:
	lw	$31,92($sp)
	addiu	$sp,$sp,96
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	gpon_qos_init
	.size	gpon_qos_init, .-gpon_qos_init
	.section	.text.gpon_qos_deinit,"ax",@progbits
	.align	2
	.globl	gpon_qos_deinit
	.set	nomips16
	.set	nomicromips
	.ent	gpon_qos_deinit
	.type	gpon_qos_deinit, @function
gpon_qos_deinit:
	.frame	$sp,96,$31		# vars= 72, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-96
	sw	$31,92($sp)
	sw	$0,16($sp)
	j	$L63
	nop

$L70:
	lw	$2,16($sp)
	andi	$2,$2,0x00ff
	sb	$2,36($sp)
	sw	$0,40($sp)
	sw	$0,48($sp)
	li	$2,16			# 0x10
	sw	$2,24($sp)
	addiu	$2,$sp,36
	sw	$2,28($sp)
	sw	$0,32($sp)
	li	$2,40			# 0x28
	sw	$2,56($sp)
	lw	$2,28($sp)
	sw	$2,64($sp)
	addiu	$2,$sp,56
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
	beq	$3,$2,$L64
	nop

	lw	$2,60($sp)
	j	$L66
	nop

$L64:
	li	$2,-1			# 0xffffffffffffffff
$L66:
	sw	$2,20($sp)
	lw	$2,20($sp)
	bgez	$2,$L67
	nop

	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	lhu	$2,-30816($2)
	andi	$2,$2,0x1
	beq	$2,$0,$L68
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

$L68:
	li	$2,-1			# 0xffffffffffffffff
	j	$L71
	nop

$L67:
	lw	$2,16($sp)
	addiu	$2,$2,1
	sw	$2,16($sp)
$L63:
	lw	$2,16($sp)
	slt	$2,$2,32
	bne	$2,$0,$L70
	nop

	li	$4,1			# 0x1
	lui	$2,%hi(xpon_set_qdma_qos)
	addiu	$2,$2,%lo(xpon_set_qdma_qos)
	jalr	$2
	nop

	move	$2,$0
$L71:
	lw	$31,92($sp)
	addiu	$sp,$sp,96
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	gpon_qos_deinit
	.size	gpon_qos_deinit, .-gpon_qos_deinit
	.ident	"GCC: (Buildroot 2015.08.1) 4.9.3"
