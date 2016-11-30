	.file	1 "xmcs_gpon.c"
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
	.ascii	"[%lu0ms]SN:%s, PASSWD:%s\012\000"
#NO_APP
	.section	.text.xmcs_set_sn_passwd,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	xmcs_set_sn_passwd
	.type	xmcs_set_sn_passwd, @function
xmcs_set_sn_passwd:
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
	addiu	$2,$2,205
	move	$4,$2
	move	$5,$0
	li	$6,8			# 0x8
	lui	$2,%hi(memset)
	addiu	$2,$2,%lo(memset)
	jalr	$2
	nop

	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	addiu	$3,$2,205
	lw	$2,24($sp)
	move	$4,$3
	move	$5,$2
	li	$6,8			# 0x8
	lui	$2,%hi(memcpy)
	addiu	$2,$2,%lo(memcpy)
	jalr	$2
	nop

	lw	$2,24($sp)
	move	$4,$2
	lui	$2,%hi(gponDevSetSerialNumber)
	addiu	$2,$2,%lo(gponDevSetSerialNumber)
	jalr	$2
	nop

	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	addiu	$2,$2,216
	move	$4,$2
	move	$5,$0
	li	$6,10			# 0xa
	lui	$2,%hi(memset)
	addiu	$2,$2,%lo(memset)
	jalr	$2
	nop

	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	addiu	$3,$2,216
	lw	$2,24($sp)
	addiu	$2,$2,8
	move	$4,$3
	move	$5,$2
	li	$6,10			# 0xa
	lui	$2,%hi(memcpy)
	addiu	$2,$2,%lo(memcpy)
	jalr	$2
	nop

	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lw	$3,24($sp)
	lbu	$3,19($3)
	sb	$3,213($2)
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lw	$3,24($sp)
	lbu	$3,20($3)
	sb	$3,214($2)
	lui	$2,%hi(gpGponPriv)
	lw	$3,%lo(gpGponPriv)($2)
	lw	$2,24($sp)
	lbu	$2,18($2)
	andi	$2,$2,0x1
	andi	$4,$2,0x00ff
	lbu	$2,464($3)
	ins	$2,$4,5,1
	sb	$2,464($3)
	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	lhu	$2,-30816($2)
	andi	$2,$2,0x40
	beq	$2,$0,$L2
	nop

	lui	$2,%hi(jiffies)
	lw	$5,%lo(jiffies)($2)
	lw	$6,24($sp)
	lw	$2,24($sp)
	addiu	$3,$2,8
	lui	$2,%hi($LC1)
	addiu	$4,$2,%lo($LC1)
	move	$7,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L2:
	move	$2,$0
	lw	$31,20($sp)
	addiu	$sp,$sp,24
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	xmcs_set_sn_passwd
	.size	xmcs_set_sn_passwd, .-xmcs_set_sn_passwd
	.section	.text.xmcs_set_sn,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	xmcs_set_sn
	.type	xmcs_set_sn, @function
xmcs_set_sn:
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
	addiu	$2,$2,205
	move	$4,$2
	move	$5,$0
	li	$6,8			# 0x8
	lui	$2,%hi(memset)
	addiu	$2,$2,%lo(memset)
	jalr	$2
	nop

	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	addiu	$3,$2,205
	lw	$2,24($sp)
	move	$4,$3
	move	$5,$2
	li	$6,8			# 0x8
	lui	$2,%hi(memcpy)
	addiu	$2,$2,%lo(memcpy)
	jalr	$2
	nop

	lw	$2,24($sp)
	move	$4,$2
	lui	$2,%hi(gponDevSetSerialNumber)
	addiu	$2,$2,%lo(gponDevSetSerialNumber)
	jalr	$2
	nop

	move	$2,$0
	lw	$31,20($sp)
	addiu	$sp,$sp,24
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	xmcs_set_sn
	.size	xmcs_set_sn, .-xmcs_set_sn
	.section	.text.xmcs_set_passwd,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	xmcs_set_passwd
	.type	xmcs_set_passwd, @function
xmcs_set_passwd:
	.frame	$sp,40,$31		# vars= 16, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	addiu	$sp,$sp,-40
	sw	$31,36($sp)
	sw	$4,40($sp)
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	addiu	$2,$2,216
	move	$4,$2
	move	$5,$0
	li	$6,10			# 0xa
	lui	$2,%hi(memset)
	addiu	$2,$2,%lo(memset)
	jalr	$2
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	addiu	$3,$2,216
	lw	$2,40($sp)
	addiu	$2,$2,8
	sw	$3,16($sp)
	sw	$2,20($sp)
	li	$2,10			# 0xa
	sw	$2,24($sp)
	lw	$2,16($sp)
	sw	$2,28($sp)
	lw	$2,24($sp)
	beq	$2,$0,$L8
	lw	$4,16($sp)
	lw	$3,20($sp)
	lw	$2,24($sp)
#APP
 # 52 "/opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/string.h" 1
	.set	noreorder
	.set	noat
1:	lbu	$1,($3)
	subu	$2,1
	sb	$1,($4)
	beqz	$1,2f
	addiu	$4,1
	bnez	$2,1b
	addiu	$3,1
2:
	.set	at
	.set	reorder
 # 0 "" 2
#NO_APP
	sw	$4,16($sp)
	sw	$3,20($sp)
	sw	$2,24($sp)
$L8:
	move	$2,$0
	lw	$31,36($sp)
	addiu	$sp,$sp,40
	j	$31
	.end	xmcs_set_passwd
	.size	xmcs_set_passwd, .-xmcs_set_passwd
	.section	.text.xmcs_set_emergency_state,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	xmcs_set_emergency_state
	.type	xmcs_set_emergency_state, @function
xmcs_set_emergency_state:
	.frame	$sp,24,$31		# vars= 0, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-24
	sw	$31,20($sp)
	sw	$4,24($sp)
	lui	$2,%hi(gpGponPriv)
	lw	$3,%lo(gpGponPriv)($2)
	lw	$2,24($sp)
	andi	$2,$2,0x00ff
	andi	$2,$2,0x1
	andi	$4,$2,0x00ff
	lbu	$2,464($3)
	ins	$2,$4,5,1
	sb	$2,464($3)
	lw	$3,24($sp)
	li	$2,1			# 0x1
	bne	$3,$2,$L11
	nop

	li	$4,7			# 0x7
	lui	$2,%hi(gpon_act_change_gpon_state)
	addiu	$2,$2,%lo(gpon_act_change_gpon_state)
	jalr	$2
	nop

	li	$4,4			# 0x4
	lui	$2,%hi(gpon_record_emergence_info)
	addiu	$2,$2,%lo(gpon_record_emergence_info)
	jalr	$2
	nop

$L11:
	move	$2,$0
	lw	$31,20($sp)
	addiu	$sp,$sp,24
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	xmcs_set_emergency_state
	.size	xmcs_set_emergency_state, .-xmcs_set_emergency_state
	.rdata
	.align	2
$LC2:
	.ascii	"[%s %d]ERR: copy_from_user\012\000"
	.section	.text.xmcs_set_act_timer,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	xmcs_set_act_timer
	.type	xmcs_set_act_timer, @function
xmcs_set_act_timer:
	.frame	$sp,64,$31		# vars= 40, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	addiu	$sp,$sp,-64
	sw	$31,60($sp)
	sw	$4,64($sp)
	addiu	$2,$sp,44
	sw	$2,20($sp)
	lw	$2,64($sp)
	sw	$2,24($sp)
	li	$2,8			# 0x8
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
	beq	$2,$0,$L14
	lw	$4,20($sp)
	lw	$5,24($sp)
	lw	$6,16($sp)
#APP
 # 71 "/opt/tclinux_phoenix/modules/private/xpon/src/xmcs/xmcs_gpon.c" 1
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
$L14:
	lw	$2,16($sp)
	beq	$2,$0,$L15
	lui	$2,%hi($LC2)
	addiu	$4,$2,%lo($LC2)
	lui	$2,%hi(__func__.30835)
	addiu	$5,$2,%lo(__func__.30835)
	li	$6,72			# 0x48
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	li	$2,-22			# 0xffffffffffffffea
	j	$L17
$L15:
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lw	$3,44($sp)
	sw	$3,256($2)
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lui	$3,%hi(gpGponPriv)
	lw	$3,%lo(gpGponPriv)($3)
	lw	$3,256($3)
	sw	$3,28($2)
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lw	$3,48($sp)
	sw	$3,260($2)
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lui	$3,%hi(gpGponPriv)
	lw	$3,%lo(gpGponPriv)($3)
	lw	$3,260($3)
	sw	$3,56($2)
	move	$2,$0
$L17:
	lw	$31,60($sp)
	addiu	$sp,$sp,64
	j	$31
	.end	xmcs_set_act_timer
	.size	xmcs_set_act_timer, .-xmcs_set_act_timer
	.section	.text.xmcs_set_act_timer_to1,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	xmcs_set_act_timer_to1
	.type	xmcs_set_act_timer_to1, @function
xmcs_set_act_timer_to1:
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	sw	$4,0($sp)
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lw	$3,0($sp)
	sw	$3,256($2)
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lui	$3,%hi(gpGponPriv)
	lw	$3,%lo(gpGponPriv)($3)
	lw	$3,256($3)
	sw	$3,28($2)
	move	$2,$0
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	xmcs_set_act_timer_to1
	.size	xmcs_set_act_timer_to1, .-xmcs_set_act_timer_to1
	.section	.text.xmcs_set_act_timer_to2,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	xmcs_set_act_timer_to2
	.type	xmcs_set_act_timer_to2, @function
xmcs_set_act_timer_to2:
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	sw	$4,0($sp)
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lw	$3,0($sp)
	sw	$3,260($2)
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lui	$3,%hi(gpGponPriv)
	lw	$3,%lo(gpGponPriv)($3)
	lw	$3,260($3)
	sw	$3,56($2)
	move	$2,$0
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	xmcs_set_act_timer_to2
	.size	xmcs_set_act_timer_to2, .-xmcs_set_act_timer_to2
	.section	.text.xmcs_get_csr,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	xmcs_get_csr
	.type	xmcs_get_csr, @function
xmcs_get_csr:
	.frame	$sp,24,$31		# vars= 0, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-24
	sw	$31,20($sp)
	lui	$2,%hi(gponDevDumpCsr)
	addiu	$2,$2,%lo(gponDevDumpCsr)
	jalr	$2
	nop

	lw	$31,20($sp)
	addiu	$sp,$sp,24
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	xmcs_get_csr
	.size	xmcs_get_csr, .-xmcs_get_csr
	.section	.text.xmcs_get_gem_table_info,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	xmcs_get_gem_table_info
	.type	xmcs_get_gem_table_info, @function
xmcs_get_gem_table_info:
	.frame	$sp,24,$31		# vars= 0, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-24
	sw	$31,20($sp)
	lui	$2,%hi(gponDevDumpGemInfo)
	addiu	$2,$2,%lo(gponDevDumpGemInfo)
	jalr	$2
	nop

	lw	$31,20($sp)
	addiu	$sp,$sp,24
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	xmcs_get_gem_table_info
	.size	xmcs_get_gem_table_info, .-xmcs_get_gem_table_info
	.section	.text.xmcs_get_tcont_table_info,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	xmcs_get_tcont_table_info
	.type	xmcs_get_tcont_table_info, @function
xmcs_get_tcont_table_info:
	.frame	$sp,24,$31		# vars= 0, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-24
	sw	$31,20($sp)
	lui	$2,%hi(gponDevDumpTcontInfo)
	addiu	$2,$2,%lo(gponDevDumpTcontInfo)
	jalr	$2
	nop

	lw	$31,20($sp)
	addiu	$sp,$sp,24
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	xmcs_get_tcont_table_info
	.size	xmcs_get_tcont_table_info, .-xmcs_get_tcont_table_info
	.section	.text.xmcs_set_idle_gem,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	xmcs_set_idle_gem
	.type	xmcs_set_idle_gem, @function
xmcs_set_idle_gem:
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
	lhu	$3,24($sp)
	sh	$3,296($2)
	lhu	$2,24($sp)
	move	$4,$2
	lui	$2,%hi(gponDevSetIdleGemThreshold)
	addiu	$2,$2,%lo(gponDevSetIdleGemThreshold)
	jalr	$2
	nop

	lw	$31,20($sp)
	addiu	$sp,$sp,24
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	xmcs_set_idle_gem
	.size	xmcs_set_idle_gem, .-xmcs_set_idle_gem
	.section	.text.xmcs_get_idle_gem,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	xmcs_get_idle_gem
	.type	xmcs_get_idle_gem, @function
xmcs_get_idle_gem:
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	sw	$4,0($sp)
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lhu	$3,296($2)
	lw	$2,0($sp)
	sh	$3,0($2)
	move	$2,$0
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	xmcs_get_idle_gem
	.size	xmcs_get_idle_gem, .-xmcs_get_idle_gem
	.section	.text.xmcs_get_onu_info,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	xmcs_get_onu_info
	.type	xmcs_get_onu_info, @function
xmcs_get_onu_info:
	.frame	$sp,24,$31		# vars= 0, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-24
	sw	$31,20($sp)
	sw	$4,24($sp)
	lw	$4,24($sp)
	move	$5,$0
	li	$6,60			# 0x3c
	lui	$2,%hi(memset)
	addiu	$2,$2,%lo(memset)
	jalr	$2
	nop

	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lbu	$3,204($2)
	lw	$2,24($sp)
	sb	$3,0($2)
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lw	$2,4($2)
	andi	$3,$2,0x00ff
	lw	$2,24($sp)
	sb	$3,1($2)
	lw	$2,24($sp)
	addiu	$3,$2,2
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	addiu	$2,$2,205
	move	$4,$3
	move	$5,$2
	li	$6,8			# 0x8
	lui	$2,%hi(memcpy)
	addiu	$2,$2,%lo(memcpy)
	jalr	$2
	nop

	lw	$2,24($sp)
	addiu	$3,$2,12
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	addiu	$2,$2,216
	move	$4,$3
	move	$5,$2
	li	$6,10			# 0xa
	lui	$2,%hi(memcpy)
	addiu	$2,$2,%lo(memcpy)
	jalr	$2
	nop

	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lbu	$3,213($2)
	lw	$2,24($sp)
	sb	$3,10($2)
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lbu	$3,214($2)
	lw	$2,24($sp)
	sb	$3,11($2)
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lbu	$3,226($2)
	lw	$2,24($sp)
	sb	$3,22($2)
	lw	$2,24($sp)
	addiu	$3,$2,23
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	addiu	$2,$2,227
	move	$4,$3
	move	$5,$2
	li	$6,24			# 0x18
	lui	$2,%hi(memcpy)
	addiu	$2,$2,%lo(memcpy)
	jalr	$2
	nop

	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lw	$3,256($2)
	lw	$2,24($sp)
	sw	$3,48($2)
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lw	$3,260($2)
	lw	$2,24($sp)
	sw	$3,52($2)
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lhu	$3,252($2)
	lw	$2,24($sp)
	sh	$3,56($2)
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lw	$2,464($2)
	ext	$2,$2,29,1
	andi	$2,$2,0x00ff
	move	$3,$2
	lw	$2,24($sp)
	sb	$3,58($2)
	move	$2,$0
	lw	$31,20($sp)
	addiu	$sp,$sp,24
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	xmcs_get_onu_info
	.size	xmcs_get_onu_info, .-xmcs_get_onu_info
	.section	.text.xmcs_set_gpon_block_size,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	xmcs_set_gpon_block_size
	.type	xmcs_set_gpon_block_size, @function
xmcs_set_gpon_block_size:
	.frame	$sp,24,$31		# vars= 0, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-24
	sw	$31,20($sp)
	move	$2,$4
	sh	$2,24($sp)
	lhu	$2,24($sp)
	move	$4,$2
	lui	$2,%hi(gponDevSetDBABlockSize)
	addiu	$2,$2,%lo(gponDevSetDBABlockSize)
	jalr	$2
	nop

	move	$2,$0
	lw	$31,20($sp)
	addiu	$sp,$sp,24
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	xmcs_set_gpon_block_size
	.size	xmcs_set_gpon_block_size, .-xmcs_set_gpon_block_size
	.section	.text.xmcs_get_gpon_block_size,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	xmcs_get_gpon_block_size
	.type	xmcs_get_gpon_block_size, @function
xmcs_get_gpon_block_size:
	.frame	$sp,24,$31		# vars= 0, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-24
	sw	$31,20($sp)
	sw	$4,24($sp)
	lw	$4,24($sp)
	lui	$2,%hi(gponDevGetDBABlockSize)
	addiu	$2,$2,%lo(gponDevGetDBABlockSize)
	jalr	$2
	nop

	move	$2,$0
	lw	$31,20($sp)
	addiu	$sp,$sp,24
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	xmcs_get_gpon_block_size
	.size	xmcs_get_gpon_block_size, .-xmcs_get_gpon_block_size
	.section	.text.xmcs_set_gpon_trtcm_mode,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	xmcs_set_gpon_trtcm_mode
	.type	xmcs_set_gpon_trtcm_mode, @function
xmcs_set_gpon_trtcm_mode:
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
	.end	xmcs_set_gpon_trtcm_mode
	.size	xmcs_set_gpon_trtcm_mode, .-xmcs_set_gpon_trtcm_mode
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
	.section	.text.xmcs_set_gpon_trtcm_scale,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	xmcs_set_gpon_trtcm_scale
	.type	xmcs_set_gpon_trtcm_scale, @function
xmcs_set_gpon_trtcm_scale:
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
	lw	$2,4($2)
	sltu	$2,$2,16
	bne	$2,$0,$L41
	nop

	li	$2,-22			# 0xffffffffffffffea
	j	$L43
	nop

$L41:
	move	$2,$0
$L43:
	lw	$31,84($sp)
	addiu	$sp,$sp,88
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	xmcs_set_gpon_trtcm_scale
	.size	xmcs_set_gpon_trtcm_scale, .-xmcs_set_gpon_trtcm_scale
	.section	.text.xmcs_get_gpon_trtcm_config,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	xmcs_get_gpon_trtcm_config
	.type	xmcs_get_gpon_trtcm_config, @function
xmcs_get_gpon_trtcm_config:
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
	.end	xmcs_get_gpon_trtcm_config
	.size	xmcs_get_gpon_trtcm_config, .-xmcs_get_gpon_trtcm_config
	.section	.text.xmcs_set_gpon_trtcm_params,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	xmcs_set_gpon_trtcm_params
	.type	xmcs_set_gpon_trtcm_params, @function
xmcs_set_gpon_trtcm_params:
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
	bne	$2,$0,$L47
	nop

	li	$2,-22			# 0xffffffffffffffea
	j	$L52
	nop

$L47:
	lw	$2,88($sp)
	lbu	$2,0($2)
	sb	$2,28($sp)
	lw	$2,88($sp)
	lhu	$2,2($2)
	sw	$2,32($sp)
	lw	$2,88($sp)
	lhu	$2,4($2)
	sw	$2,36($sp)
	lw	$2,88($sp)
	lhu	$2,6($2)
	sw	$2,40($sp)
	lw	$2,88($sp)
	lhu	$2,8($2)
	sw	$2,44($sp)
	li	$2,16			# 0x10
	sw	$2,16($sp)
	addiu	$2,$sp,28
	sw	$2,20($sp)
	sw	$0,24($sp)
	li	$2,40			# 0x28
	sw	$2,48($sp)
	lw	$2,20($sp)
	sw	$2,56($sp)
	addiu	$2,$sp,48
	lw	$4,16($sp)
	move	$5,$0
	move	$6,$2
	lui	$2,%hi(__ECNT_HOOK)
	addiu	$2,$2,%lo(__ECNT_HOOK)
	jalr	$2
	nop

	sw	$2,24($sp)
	lw	$3,24($sp)
	li	$2,-1			# 0xffffffffffffffff
	beq	$3,$2,$L49
	nop

	lw	$2,52($sp)
	j	$L52
	nop

$L49:
	li	$2,-1			# 0xffffffffffffffff
	nop
$L52:
	lw	$31,84($sp)
	addiu	$sp,$sp,88
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	xmcs_set_gpon_trtcm_params
	.size	xmcs_set_gpon_trtcm_params, .-xmcs_set_gpon_trtcm_params
	.section	.text.xmcs_get_gpon_trtcm_params,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	xmcs_get_gpon_trtcm_params
	.type	xmcs_get_gpon_trtcm_params, @function
xmcs_get_gpon_trtcm_params:
	.frame	$sp,24,$31		# vars= 24, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-24
	sw	$4,24($sp)
	sw	$0,0($sp)
	lw	$2,24($sp)
	lbu	$2,0($2)
	sltu	$2,$2,32
	bne	$2,$0,$L54
	nop

	li	$2,-22			# 0xffffffffffffffea
	j	$L56
	nop

$L54:
	lw	$2,24($sp)
	lbu	$2,0($2)
	sb	$2,4($sp)
	lw	$2,8($sp)
	andi	$3,$2,0xffff
	lw	$2,24($sp)
	sh	$3,2($2)
	lw	$2,12($sp)
	andi	$3,$2,0xffff
	lw	$2,24($sp)
	sh	$3,4($2)
	lw	$2,16($sp)
	andi	$3,$2,0xffff
	lw	$2,24($sp)
	sh	$3,6($2)
	lw	$2,20($sp)
	andi	$3,$2,0xffff
	lw	$2,24($sp)
	sh	$3,8($2)
	move	$2,$0
$L56:
	addiu	$sp,$sp,24
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	xmcs_get_gpon_trtcm_params
	.size	xmcs_get_gpon_trtcm_params, .-xmcs_get_gpon_trtcm_params
	.section	.text.xmcs_set_gpon_tod,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	xmcs_set_gpon_tod
	.type	xmcs_set_gpon_tod, @function
xmcs_set_gpon_tod:
	.frame	$sp,24,$31		# vars= 0, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-24
	sw	$31,20($sp)
	sw	$4,24($sp)
	lw	$2,24($sp)
	lw	$4,0($2)
	lw	$2,24($sp)
	lw	$3,4($2)
	lw	$2,24($sp)
	lw	$2,8($2)
	move	$5,$3
	move	$6,$2
	lui	$2,%hi(gponDevSetNewTod)
	addiu	$2,$2,%lo(gponDevSetNewTod)
	jalr	$2
	nop

	move	$2,$0
	lw	$31,20($sp)
	addiu	$sp,$sp,24
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	xmcs_set_gpon_tod
	.size	xmcs_set_gpon_tod, .-xmcs_set_gpon_tod
	.section	.text.xmcs_get_gpon_tod,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	xmcs_get_gpon_tod
	.type	xmcs_get_gpon_tod, @function
xmcs_get_gpon_tod:
	.frame	$sp,24,$31		# vars= 0, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-24
	sw	$31,20($sp)
	sw	$4,24($sp)
	lw	$2,24($sp)
	addiu	$3,$2,4
	lw	$2,24($sp)
	addiu	$2,$2,8
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(gponDevGetCurrentTod)
	addiu	$2,$2,%lo(gponDevGetCurrentTod)
	jalr	$2
	nop

	move	$2,$0
	lw	$31,20($sp)
	addiu	$sp,$sp,24
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	xmcs_get_gpon_tod
	.size	xmcs_get_gpon_tod, .-xmcs_get_gpon_tod
	.section	.text.xmcs_get_gpon_new_tod,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	xmcs_get_gpon_new_tod
	.type	xmcs_get_gpon_new_tod, @function
xmcs_get_gpon_new_tod:
	.frame	$sp,24,$31		# vars= 0, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-24
	sw	$31,20($sp)
	sw	$4,24($sp)
	lw	$2,24($sp)
	addiu	$3,$2,4
	lw	$2,24($sp)
	addiu	$2,$2,8
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(gponDevGetNewTod)
	addiu	$2,$2,%lo(gponDevGetNewTod)
	jalr	$2
	nop

	move	$2,$0
	lw	$31,20($sp)
	addiu	$sp,$sp,24
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	xmcs_get_gpon_new_tod
	.size	xmcs_get_gpon_new_tod, .-xmcs_get_gpon_new_tod
	.section	.text.get_counter_from_reg,"ax",@progbits
	.align	2
	.globl	get_counter_from_reg
	.set	nomips16
	.set	nomicromips
	.ent	get_counter_from_reg
	.type	get_counter_from_reg, @function
get_counter_from_reg:
	.frame	$sp,40,$31		# vars= 16, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-40
	sw	$31,36($sp)
	sw	$4,40($sp)
	lui	$2,%hi(gpWanPriv)
	lw	$3,%lo(gpWanPriv)($2)
	lw	$2,40($sp)
	lhu	$2,0($2)
	addiu	$2,$2,44
	sll	$2,$2,1
	addu	$2,$3,$2
	lhu	$2,4($2)
	andi	$2,$2,0x7fff
	sh	$2,16($sp)
	lui	$2,%hi(gpWanPriv)
	lw	$4,%lo(gpWanPriv)($2)
	lhu	$2,16($sp)
	move	$3,$2
	sll	$2,$3,5
	move	$3,$2
	sll	$2,$3,2
	subu	$2,$2,$3
	addiu	$2,$2,8280
	addu	$2,$4,$2
	addiu	$2,$2,4
	sw	$2,20($sp)
	lw	$2,20($sp)
	lw	$2,0($2)
	andi	$2,$2,0x4
	bne	$2,$0,$L64
	nop

	li	$2,-1			# 0xffffffffffffffff
	j	$L65
	nop

$L64:
	lw	$2,20($sp)
	lw	$2,0($2)
	ext	$2,$2,20,12
	andi	$2,$2,0xffff
	sh	$2,24($sp)
	lhu	$4,24($sp)
	lw	$2,40($sp)
	addiu	$3,$2,4
	lw	$2,40($sp)
	addiu	$2,$2,8
	move	$5,$0
	move	$6,$3
	move	$7,$2
	lui	$2,%hi(gponDevGetGemPortCounter)
	addiu	$2,$2,%lo(gponDevGetGemPortCounter)
	jalr	$2
	nop

	beq	$2,$0,$L66
	nop

	li	$2,-1			# 0xffffffffffffffff
	j	$L65
	nop

$L66:
	lhu	$4,24($sp)
	lw	$2,40($sp)
	addiu	$3,$2,12
	lw	$2,40($sp)
	addiu	$2,$2,16
	li	$5,1			# 0x1
	move	$6,$3
	move	$7,$2
	lui	$2,%hi(gponDevGetGemPortCounter)
	addiu	$2,$2,%lo(gponDevGetGemPortCounter)
	jalr	$2
	nop

	beq	$2,$0,$L67
	nop

	li	$2,-1			# 0xffffffffffffffff
	j	$L65
	nop

$L67:
	lhu	$4,24($sp)
	lw	$2,40($sp)
	addiu	$3,$2,20
	lw	$2,40($sp)
	addiu	$2,$2,24
	li	$5,2			# 0x2
	move	$6,$3
	move	$7,$2
	lui	$2,%hi(gponDevGetGemPortCounter)
	addiu	$2,$2,%lo(gponDevGetGemPortCounter)
	jalr	$2
	nop

	beq	$2,$0,$L68
	nop

	li	$2,-1			# 0xffffffffffffffff
	j	$L65
	nop

$L68:
	lhu	$4,24($sp)
	lw	$2,40($sp)
	addiu	$3,$2,28
	lw	$2,40($sp)
	addiu	$2,$2,32
	li	$5,3			# 0x3
	move	$6,$3
	move	$7,$2
	lui	$2,%hi(gponDevGetGemPortCounter)
	addiu	$2,$2,%lo(gponDevGetGemPortCounter)
	jalr	$2
	nop

	beq	$2,$0,$L69
	nop

	li	$2,-1			# 0xffffffffffffffff
	j	$L65
	nop

$L69:
	move	$2,$0
$L65:
	lw	$31,36($sp)
	addiu	$sp,$sp,40
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	get_counter_from_reg
	.size	get_counter_from_reg, .-get_counter_from_reg
	.section	.text.xmcs_get_tod_switch_time,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	xmcs_get_tod_switch_time
	.type	xmcs_get_tod_switch_time, @function
xmcs_get_tod_switch_time:
	.frame	$sp,24,$31		# vars= 0, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-24
	sw	$31,20($sp)
	sw	$4,24($sp)
	lw	$2,24($sp)
	move	$4,$2
	lui	$2,%hi(gponDevGetTodSwitchTime)
	addiu	$2,$2,%lo(gponDevGetTodSwitchTime)
	jalr	$2
	nop

	move	$2,$0
	lw	$31,20($sp)
	addiu	$sp,$sp,24
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	xmcs_get_tod_switch_time
	.size	xmcs_get_tod_switch_time, .-xmcs_get_tod_switch_time
	.section	.text.xmcs_get_gem_counter,"ax",@progbits
	.align	2
	.globl	xmcs_get_gem_counter
	.set	nomips16
	.set	nomicromips
	.ent	xmcs_get_gem_counter
	.type	xmcs_get_gem_counter, @function
xmcs_get_gem_counter:
	.frame	$sp,120,$31		# vars= 96, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	addiu	$sp,$sp,-120
	sw	$31,116($sp)
	sw	$4,120($sp)
	sw	$0,24($sp)
	addiu	$2,$sp,76
	move	$4,$2
	move	$5,$0
	li	$6,36			# 0x24
	lui	$2,%hi(memset)
	addiu	$2,$2,%lo(memset)
	jalr	$2
	addiu	$2,$sp,76
	sw	$2,28($sp)
	lw	$2,120($sp)
	sw	$2,32($sp)
	li	$2,36			# 0x24
	sw	$2,16($sp)
	lw	$2,32($sp)
	sw	$2,36($sp)
	lw	$2,16($sp)
	sw	$2,40($sp)
	move	$2,$28
	lw	$2,24($2)
	sw	$2,44($sp)
	lw	$3,36($sp)
	lw	$2,40($sp)
	addu	$3,$3,$2
	lw	$2,36($sp)
	or	$3,$3,$2
	lw	$2,40($sp)
	or	$3,$3,$2
	lw	$2,44($sp)
	and	$2,$3,$2
	sw	$2,48($sp)
	lw	$2,48($sp)
	sltu	$2,$2,1
	andi	$2,$2,0x00ff
	sltu	$2,$0,$2
	andi	$2,$2,0x00ff
	beq	$2,$0,$L73
	lw	$4,28($sp)
	lw	$5,32($sp)
	lw	$6,16($sp)
#APP
 # 421 "/opt/tclinux_phoenix/modules/private/xpon/src/xmcs/xmcs_gpon.c" 1
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
$L73:
	lw	$2,16($sp)
	sw	$2,24($sp)
	lw	$2,24($sp)
	beq	$2,$0,$L74
	li	$2,-22			# 0xffffffffffffffea
	j	$L79
$L74:
	addiu	$2,$sp,76
	move	$4,$2
	lui	$2,%hi(get_counter_from_reg)
	addiu	$2,$2,%lo(get_counter_from_reg)
	jalr	$2
	sw	$2,24($sp)
	lw	$2,24($sp)
	beq	$2,$0,$L76
	li	$2,-22			# 0xffffffffffffffea
	j	$L79
$L76:
	lw	$2,120($sp)
	sw	$2,52($sp)
	addiu	$2,$sp,76
	sw	$2,56($sp)
	li	$2,36			# 0x24
	sw	$2,20($sp)
	lw	$2,52($sp)
	sw	$2,60($sp)
	lw	$2,20($sp)
	sw	$2,64($sp)
	move	$2,$28
	lw	$2,24($2)
	sw	$2,68($sp)
	lw	$3,60($sp)
	lw	$2,64($sp)
	addu	$3,$3,$2
	lw	$2,60($sp)
	or	$3,$3,$2
	lw	$2,64($sp)
	or	$3,$3,$2
	lw	$2,68($sp)
	and	$2,$3,$2
	sw	$2,72($sp)
	lw	$2,72($sp)
	sltu	$2,$2,1
	andi	$2,$2,0x00ff
	sltu	$2,$0,$2
	andi	$2,$2,0x00ff
	beq	$2,$0,$L77
	lw	$4,52($sp)
	lw	$5,56($sp)
	lw	$6,20($sp)
#APP
 # 433 "/opt/tclinux_phoenix/modules/private/xpon/src/xmcs/xmcs_gpon.c" 1
	.set	noat
	la	$1, __copy_user
	jalr	$1
	.set	at
	
 # 0 "" 2
#NO_APP
	move	$2,$6
	sw	$2,20($sp)
$L77:
	lw	$2,20($sp)
	sw	$2,24($sp)
	lw	$2,24($sp)
	beq	$2,$0,$L78
	li	$2,-22			# 0xffffffffffffffea
	j	$L79
$L78:
	lw	$2,24($sp)
$L79:
	lw	$31,116($sp)
	addiu	$sp,$sp,120
	j	$31
	.end	xmcs_get_gem_counter
	.size	xmcs_get_gem_counter, .-xmcs_get_gem_counter
	.section	__ksymtab_strings,"a",@progbits
	.type	__kstrtab_xmcs_get_gem_counter, @object
	.size	__kstrtab_xmcs_get_gem_counter, 21
__kstrtab_xmcs_get_gem_counter:
	.ascii	"xmcs_get_gem_counter\000"
	.section	__ksymtab,"a",@progbits
	.align	2
	.type	__ksymtab_xmcs_get_gem_counter, @object
	.size	__ksymtab_xmcs_get_gem_counter, 8
__ksymtab_xmcs_get_gem_counter:
	.word	xmcs_get_gem_counter
	.word	__kstrtab_xmcs_get_gem_counter
	.section	.text.xmcs_set_eqd_offset_flag,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	xmcs_set_eqd_offset_flag
	.type	xmcs_set_eqd_offset_flag, @function
xmcs_set_eqd_offset_flag:
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	move	$2,$4
	sb	$2,0($sp)
	lui	$2,%hi(gpGponPriv)
	lw	$3,%lo(gpGponPriv)($2)
	lb	$2,0($sp)
	xori	$2,$2,0x2d
	sltu	$2,$2,1
	andi	$4,$2,0x00ff
	lhu	$2,448($3)
	ins	$2,$4,12,1
	sh	$2,448($3)
	move	$2,$0
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	xmcs_set_eqd_offset_flag
	.size	xmcs_set_eqd_offset_flag, .-xmcs_set_eqd_offset_flag
	.section	.text.xmcs_set_eqd_offset_o4,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	xmcs_set_eqd_offset_o4
	.type	xmcs_set_eqd_offset_o4, @function
xmcs_set_eqd_offset_o4:
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	move	$2,$4
	sb	$2,0($sp)
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lbu	$3,0($sp)
	sb	$3,277($2)
	move	$2,$0
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	xmcs_set_eqd_offset_o4
	.size	xmcs_set_eqd_offset_o4, .-xmcs_set_eqd_offset_o4
	.section	.text.xmcs_set_eqd_offset_o5,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	xmcs_set_eqd_offset_o5
	.type	xmcs_set_eqd_offset_o5, @function
xmcs_set_eqd_offset_o5:
	.frame	$sp,24,$31		# vars= 0, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-24
	sw	$31,20($sp)
	move	$2,$4
	sb	$2,24($sp)
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lbu	$3,24($sp)
	sb	$3,278($2)
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lw	$2,268($2)
	move	$4,$2
	lui	$2,%hi(gpon_dvt_eqd_adjustment)
	addiu	$2,$2,%lo(gpon_dvt_eqd_adjustment)
	jalr	$2
	nop

	move	$2,$0
	lw	$31,20($sp)
	addiu	$sp,$sp,24
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	xmcs_set_eqd_offset_o5
	.size	xmcs_set_eqd_offset_o5, .-xmcs_set_eqd_offset_o5
	.section	.text.xmcs_get_eqd_offset,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	xmcs_get_eqd_offset
	.type	xmcs_get_eqd_offset, @function
xmcs_get_eqd_offset:
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	sw	$4,0($sp)
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lbu	$3,277($2)
	lw	$2,0($sp)
	sb	$3,0($2)
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lbu	$3,278($2)
	lw	$2,0($sp)
	sb	$3,1($2)
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lw	$2,448($2)
	ext	$2,$2,28,1
	andi	$2,$2,0x00ff
	move	$3,$2
	lw	$2,0($sp)
	sb	$3,2($2)
	move	$2,$0
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	xmcs_get_eqd_offset
	.size	xmcs_get_eqd_offset, .-xmcs_get_eqd_offset
	.section	.text.xmcs_set_counter_type,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	xmcs_set_counter_type
	.type	xmcs_set_counter_type, @function
xmcs_set_counter_type:
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
	lw	$3,24($sp)
	sw	$3,300($2)
	lw	$4,24($sp)
	lui	$2,%hi(gponDevSetCounterType)
	addiu	$2,$2,%lo(gponDevSetCounterType)
	jalr	$2
	nop

	move	$2,$0
	lw	$31,20($sp)
	addiu	$sp,$sp,24
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	xmcs_set_counter_type
	.size	xmcs_set_counter_type, .-xmcs_set_counter_type
	.section	.text.xmcs_get_counter_type,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	xmcs_get_counter_type
	.type	xmcs_get_counter_type, @function
xmcs_get_counter_type:
	.frame	$sp,24,$31		# vars= 0, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-24
	sw	$31,20($sp)
	sw	$4,24($sp)
	lw	$4,24($sp)
	lui	$2,%hi(gponDevGetCounterType)
	addiu	$2,$2,%lo(gponDevGetCounterType)
	jalr	$2
	nop

	move	$2,$0
	lw	$31,20($sp)
	addiu	$sp,$sp,24
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	xmcs_get_counter_type
	.size	xmcs_get_counter_type, .-xmcs_get_counter_type
	.section	.text.xmcs_set_response_time,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	xmcs_set_response_time
	.type	xmcs_set_response_time, @function
xmcs_set_response_time:
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
	lhu	$3,24($sp)
	sh	$3,292($2)
	lhu	$2,24($sp)
	move	$4,$2
	lui	$2,%hi(gponDevSetResponseTime)
	addiu	$2,$2,%lo(gponDevSetResponseTime)
	jalr	$2
	nop

	move	$2,$0
	lw	$31,20($sp)
	addiu	$sp,$sp,24
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	xmcs_set_response_time
	.size	xmcs_set_response_time, .-xmcs_set_response_time
	.section	.text.xmcs_get_response_time,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	xmcs_get_response_time
	.type	xmcs_get_response_time, @function
xmcs_get_response_time:
	.frame	$sp,24,$31		# vars= 0, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-24
	sw	$31,20($sp)
	sw	$4,24($sp)
	lw	$4,24($sp)
	lui	$2,%hi(gponDevGetResponseTime)
	addiu	$2,$2,%lo(gponDevGetResponseTime)
	jalr	$2
	nop

	move	$2,$0
	lw	$31,20($sp)
	addiu	$sp,$sp,24
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	xmcs_get_response_time
	.size	xmcs_get_response_time, .-xmcs_get_response_time
	.section	.text.xmcs_set_internal_delay_fine_tune,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	xmcs_set_internal_delay_fine_tune
	.type	xmcs_set_internal_delay_fine_tune, @function
xmcs_set_internal_delay_fine_tune:
	.frame	$sp,24,$31		# vars= 0, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-24
	sw	$31,20($sp)
	move	$2,$4
	sb	$2,24($sp)
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lbu	$3,24($sp)
	sb	$3,294($2)
	lbu	$2,24($sp)
	move	$4,$2
	lui	$2,%hi(gponDevSetInternalDelayFineTune)
	addiu	$2,$2,%lo(gponDevSetInternalDelayFineTune)
	jalr	$2
	nop

	move	$2,$0
	lw	$31,20($sp)
	addiu	$sp,$sp,24
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	xmcs_set_internal_delay_fine_tune
	.size	xmcs_set_internal_delay_fine_tune, .-xmcs_set_internal_delay_fine_tune
	.section	.text.xmcs_get_internal_delay_fine_tune,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	xmcs_get_internal_delay_fine_tune
	.type	xmcs_get_internal_delay_fine_tune, @function
xmcs_get_internal_delay_fine_tune:
	.frame	$sp,24,$31		# vars= 0, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-24
	sw	$31,20($sp)
	sw	$4,24($sp)
	lw	$4,24($sp)
	lui	$2,%hi(gponDevGetInternalDelayFineTune)
	addiu	$2,$2,%lo(gponDevGetInternalDelayFineTune)
	jalr	$2
	nop

	move	$2,$0
	lw	$31,20($sp)
	addiu	$sp,$sp,24
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	xmcs_get_internal_delay_fine_tune
	.size	xmcs_get_internal_delay_fine_tune, .-xmcs_get_internal_delay_fine_tune
	.section	.text.xmcs_set_burst_mode_overhead,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	xmcs_set_burst_mode_overhead
	.type	xmcs_set_burst_mode_overhead, @function
xmcs_set_burst_mode_overhead:
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	sw	$4,0($sp)
	lui	$2,%hi(gpGponPriv)
	lw	$3,%lo(gpGponPriv)($2)
	lw	$2,0($sp)
	andi	$2,$2,0x00ff
	andi	$2,$2,0x1
	andi	$4,$2,0x00ff
	lhu	$2,448($3)
	ins	$2,$4,11,1
	sh	$2,448($3)
	move	$2,$0
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	xmcs_set_burst_mode_overhead
	.size	xmcs_set_burst_mode_overhead, .-xmcs_set_burst_mode_overhead
	.section	.text.xmcs_get_burst_mode_overhead,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	xmcs_get_burst_mode_overhead
	.type	xmcs_get_burst_mode_overhead, @function
xmcs_get_burst_mode_overhead:
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	sw	$4,0($sp)
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lw	$2,448($2)
	ext	$2,$2,27,1
	andi	$2,$2,0x00ff
	move	$3,$2
	lw	$2,0($sp)
	sw	$3,0($2)
	move	$2,$0
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	xmcs_get_burst_mode_overhead
	.size	xmcs_get_burst_mode_overhead, .-xmcs_get_burst_mode_overhead
	.section	.text.xmcs_set_clear_counter,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	xmcs_set_clear_counter
	.type	xmcs_set_clear_counter, @function
xmcs_set_clear_counter:
	.frame	$sp,24,$31		# vars= 0, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-24
	sw	$31,20($sp)
	sw	$4,24($sp)
	lw	$2,24($sp)
	li	$3,1			# 0x1
	beq	$2,$3,$L106
	nop

	sltu	$3,$2,1
	bne	$3,$0,$L107
	nop

	li	$3,2			# 0x2
	beq	$2,$3,$L108
	nop

	j	$L111
	nop

$L107:
	lui	$2,%hi(gponDevClearSwCounter)
	addiu	$2,$2,%lo(gponDevClearSwCounter)
	jalr	$2
	nop

	j	$L109
	nop

$L106:
	lui	$2,%hi(gponDevClearHwCounter)
	addiu	$2,$2,%lo(gponDevClearHwCounter)
	jalr	$2
	nop

	j	$L109
	nop

$L108:
	lui	$2,%hi(gponDevClearSwCounter)
	addiu	$2,$2,%lo(gponDevClearSwCounter)
	jalr	$2
	nop

	lui	$2,%hi(gponDevClearHwCounter)
	addiu	$2,$2,%lo(gponDevClearHwCounter)
	jalr	$2
	nop

	j	$L109
	nop

$L111:
	li	$2,-1			# 0xffffffffffffffff
	j	$L110
	nop

$L109:
	move	$2,$0
$L110:
	lw	$31,20($sp)
	addiu	$sp,$sp,24
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	xmcs_set_clear_counter
	.size	xmcs_set_clear_counter, .-xmcs_set_clear_counter
	.section	.text.xmcs_set_key_switch_time,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	xmcs_set_key_switch_time
	.type	xmcs_set_key_switch_time, @function
xmcs_set_key_switch_time:
	.frame	$sp,24,$31		# vars= 0, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-24
	sw	$31,20($sp)
	sw	$4,24($sp)
	lw	$4,24($sp)
	lui	$2,%hi(gponDevSetKeySwithTime)
	addiu	$2,$2,%lo(gponDevSetKeySwithTime)
	jalr	$2
	nop

	lw	$31,20($sp)
	addiu	$sp,$sp,24
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	xmcs_set_key_switch_time
	.size	xmcs_set_key_switch_time, .-xmcs_set_key_switch_time
	.section	.text.xmcs_set_encrypt_key,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	xmcs_set_encrypt_key
	.type	xmcs_set_encrypt_key, @function
xmcs_set_encrypt_key:
	.frame	$sp,24,$31		# vars= 0, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-24
	sw	$31,20($sp)
	sw	$4,24($sp)
	lw	$4,24($sp)
	lui	$2,%hi(gponDevSetEncryptKey)
	addiu	$2,$2,%lo(gponDevSetEncryptKey)
	jalr	$2
	nop

	lw	$31,20($sp)
	addiu	$sp,$sp,24
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	xmcs_set_encrypt_key
	.size	xmcs_set_encrypt_key, .-xmcs_set_encrypt_key
	.section	.text.xmcs_get_encrypt_key_info,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	xmcs_get_encrypt_key_info
	.type	xmcs_get_encrypt_key_info, @function
xmcs_get_encrypt_key_info:
	.frame	$sp,24,$31		# vars= 0, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-24
	sw	$31,20($sp)
	sw	$4,24($sp)
	lw	$4,24($sp)
	lui	$2,%hi(gponDevGetEncryptKey)
	addiu	$2,$2,%lo(gponDevGetEncryptKey)
	jalr	$2
	nop

	lw	$31,20($sp)
	addiu	$sp,$sp,24
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	xmcs_get_encrypt_key_info
	.size	xmcs_get_encrypt_key_info, .-xmcs_get_encrypt_key_info
	.section	.text.xmcs_set_dying_gasp_mode,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	xmcs_set_dying_gasp_mode
	.type	xmcs_set_dying_gasp_mode, @function
xmcs_set_dying_gasp_mode:
	.frame	$sp,24,$31		# vars= 0, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-24
	sw	$31,20($sp)
	sw	$4,24($sp)
	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	lw	$2,24($sp)
	andi	$2,$2,0x00ff
	andi	$2,$2,0x1
	andi	$4,$2,0x00ff
	li	$2,65536			# 0x10000
	addu	$3,$3,$2
	lbu	$2,-30828($3)
	ins	$2,$4,7,1
	sb	$2,-30828($3)
	lw	$4,24($sp)
	lui	$2,%hi(gponDevHardwareDyingGasp)
	addiu	$2,$2,%lo(gponDevHardwareDyingGasp)
	jalr	$2
	nop

	lw	$31,20($sp)
	addiu	$sp,$sp,24
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	xmcs_set_dying_gasp_mode
	.size	xmcs_set_dying_gasp_mode, .-xmcs_set_dying_gasp_mode
	.section	.text.xmcs_get_dying_gasp_mode,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	xmcs_get_dying_gasp_mode
	.type	xmcs_get_dying_gasp_mode, @function
xmcs_get_dying_gasp_mode:
	.frame	$sp,24,$31		# vars= 0, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-24
	sw	$31,20($sp)
	sw	$4,24($sp)
	lw	$4,24($sp)
	lui	$2,%hi(gponDevGetDyingGaspMode)
	addiu	$2,$2,%lo(gponDevGetDyingGaspMode)
	jalr	$2
	nop

	lw	$31,20($sp)
	addiu	$sp,$sp,24
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	xmcs_get_dying_gasp_mode
	.size	xmcs_get_dying_gasp_mode, .-xmcs_get_dying_gasp_mode
	.section	.text.xmcs_set_dying_gasp_num,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	xmcs_set_dying_gasp_num
	.type	xmcs_set_dying_gasp_num, @function
xmcs_set_dying_gasp_num:
	.frame	$sp,24,$31		# vars= 0, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-24
	sw	$31,20($sp)
	sw	$4,24($sp)
	lw	$4,24($sp)
	lui	$2,%hi(gponDevSetDyingGaspNum)
	addiu	$2,$2,%lo(gponDevSetDyingGaspNum)
	jalr	$2
	nop

	lw	$31,20($sp)
	addiu	$sp,$sp,24
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	xmcs_set_dying_gasp_num
	.size	xmcs_set_dying_gasp_num, .-xmcs_set_dying_gasp_num
	.section	.text.xmcs_get_dying_gasp_num,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	xmcs_get_dying_gasp_num
	.type	xmcs_get_dying_gasp_num, @function
xmcs_get_dying_gasp_num:
	.frame	$sp,24,$31		# vars= 0, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-24
	sw	$31,20($sp)
	sw	$4,24($sp)
	lw	$4,24($sp)
	lui	$2,%hi(gponDevGetDyingGaspNum)
	addiu	$2,$2,%lo(gponDevGetDyingGaspNum)
	jalr	$2
	nop

	lw	$31,20($sp)
	addiu	$sp,$sp,24
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	xmcs_get_dying_gasp_num
	.size	xmcs_get_dying_gasp_num, .-xmcs_get_dying_gasp_num
	.section	.text.xmcs_set_up_traffic,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	xmcs_set_up_traffic
	.type	xmcs_set_up_traffic, @function
xmcs_set_up_traffic:
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	sw	$4,0($sp)
	lw	$2,0($sp)
	lw	$2,0($2)
	beq	$2,$0,$L127
	nop

	lw	$2,0($sp)
	lw	$3,0($2)
	li	$2,1			# 0x1
	beq	$3,$2,$L127
	nop

	li	$2,-1			# 0xffffffffffffffff
	j	$L128
	nop

$L127:
	lw	$2,0($sp)
	lw	$2,4($2)
	beq	$2,$0,$L129
	nop

	lw	$2,0($sp)
	lw	$3,4($2)
	li	$2,1			# 0x1
	beq	$3,$2,$L129
	nop

	li	$2,-1			# 0xffffffffffffffff
	j	$L128
	nop

$L129:
	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	lw	$2,0($sp)
	lw	$2,0($2)
	andi	$2,$2,0x00ff
	andi	$2,$2,0x1
	andi	$4,$2,0x00ff
	li	$2,65536			# 0x10000
	addu	$3,$3,$2
	lbu	$2,-30412($3)
	ins	$2,$4,4,1
	sb	$2,-30412($3)
	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	lw	$2,0($sp)
	lw	$2,4($2)
	andi	$2,$2,0x00ff
	andi	$2,$2,0x1
	andi	$4,$2,0x00ff
	li	$2,65536			# 0x10000
	addu	$3,$3,$2
	lbu	$2,-30412($3)
	ins	$2,$4,5,1
	sb	$2,-30412($3)
	move	$2,$0
$L128:
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	xmcs_set_up_traffic
	.size	xmcs_set_up_traffic, .-xmcs_set_up_traffic
	.section	.text.xmcs_get_up_traffic,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	xmcs_get_up_traffic
	.type	xmcs_get_up_traffic, @function
xmcs_get_up_traffic:
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
	lw	$2,-30412($2)
	ext	$2,$2,28,1
	andi	$2,$2,0x00ff
	move	$3,$2
	lw	$2,0($sp)
	sw	$3,0($2)
	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	lw	$2,-30412($2)
	ext	$2,$2,29,1
	andi	$2,$2,0x00ff
	move	$3,$2
	lw	$2,0($sp)
	sw	$3,4($2)
	move	$2,$0
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	xmcs_get_up_traffic
	.size	xmcs_get_up_traffic, .-xmcs_get_up_traffic
	.section	.text.xmcs_get_int_mask,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	xmcs_get_int_mask
	.type	xmcs_get_int_mask, @function
xmcs_get_int_mask:
	.frame	$sp,24,$31		# vars= 0, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-24
	sw	$31,20($sp)
	sw	$4,24($sp)
	lw	$4,24($sp)
	lui	$2,%hi(gponDevGetIntMask)
	addiu	$2,$2,%lo(gponDevGetIntMask)
	jalr	$2
	nop

	lw	$31,20($sp)
	addiu	$sp,$sp,24
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	xmcs_get_int_mask
	.size	xmcs_get_int_mask, .-xmcs_get_int_mask
	.rdata
	.align	2
$LC3:
	.ascii	"%s %d line remove gem err\012\000"
	.align	2
$LC4:
	.ascii	"%s %d line remove tcont err\012\000"
	.section	.text.xmcs_gpon_reset_service,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	xmcs_gpon_reset_service
	.type	xmcs_gpon_reset_service, @function
xmcs_gpon_reset_service:
	.frame	$sp,32,$31		# vars= 8, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-32
	sw	$31,28($sp)
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lw	$2,480($2)
	sw	$2,16($sp)
	j	$L135
	nop

$L137:
	lw	$2,16($sp)
	lw	$2,2176($2)
	addiu	$2,$2,-1
	lw	$3,16($sp)
	addiu	$2,$2,16
	sll	$2,$2,3
	addu	$2,$3,$2
	lhu	$2,0($2)
	move	$4,$2
	lui	$2,%hi(gwan_remove_gemport)
	addiu	$2,$2,%lo(gwan_remove_gemport)
	jalr	$2
	nop

	beq	$2,$0,$L135
	nop

	lui	$2,%hi(gponRecovery)
	addiu	$2,$2,%lo(gponRecovery)
	lbu	$2,2184($2)
	beq	$2,$0,$L136
	nop

	lui	$2,%hi($LC3)
	addiu	$4,$2,%lo($LC3)
	lui	$2,%hi(__func__.31020)
	addiu	$5,$2,%lo(__func__.31020)
	li	$6,682			# 0x2aa
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L136:
	lw	$2,16($sp)
	lw	$2,2176($2)
	addiu	$3,$2,-1
	lw	$2,16($sp)
	sw	$3,2176($2)
	nop
$L135:
	lw	$2,16($sp)
	lw	$2,2176($2)
	bne	$2,$0,$L137
	nop

	j	$L138
	nop

$L140:
	lw	$2,16($sp)
	lw	$2,2180($2)
	addiu	$2,$2,-1
	lw	$3,16($sp)
	sll	$2,$2,2
	addu	$2,$3,$2
	lw	$2,0($2)
	andi	$2,$2,0xffff
	move	$4,$2
	lui	$2,%hi(gwan_remove_tcont)
	addiu	$2,$2,%lo(gwan_remove_tcont)
	jalr	$2
	nop

	beq	$2,$0,$L138
	nop

	lui	$2,%hi(gponRecovery)
	addiu	$2,$2,%lo(gponRecovery)
	lbu	$2,2184($2)
	beq	$2,$0,$L139
	nop

	lui	$2,%hi($LC4)
	addiu	$4,$2,%lo($LC4)
	lui	$2,%hi(__func__.31020)
	addiu	$5,$2,%lo(__func__.31020)
	li	$6,694			# 0x2b6
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L139:
	lw	$2,16($sp)
	lw	$2,2180($2)
	addiu	$3,$2,-1
	lw	$2,16($sp)
	sw	$3,2180($2)
	nop
$L138:
	lw	$2,16($sp)
	lw	$2,2180($2)
	bne	$2,$0,$L140
	nop

	move	$2,$0
	lw	$31,28($sp)
	addiu	$sp,$sp,32
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	xmcs_gpon_reset_service
	.size	xmcs_gpon_reset_service, .-xmcs_gpon_reset_service
	.section	.text.xmcs_set_sniffer_mode,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	xmcs_set_sniffer_mode
	.type	xmcs_set_sniffer_mode, @function
xmcs_set_sniffer_mode:
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
	addiu	$2,$2,312
	move	$4,$2
	lw	$5,24($sp)
	li	$6,32			# 0x20
	lui	$2,%hi(memcpy)
	addiu	$2,$2,%lo(memcpy)
	jalr	$2
	nop

	lw	$4,24($sp)
	lui	$2,%hi(gponDevSetSniffMode)
	addiu	$2,$2,%lo(gponDevSetSniffMode)
	jalr	$2
	nop

	lw	$31,20($sp)
	addiu	$sp,$sp,24
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	xmcs_set_sniffer_mode
	.size	xmcs_set_sniffer_mode, .-xmcs_set_sniffer_mode
	.section	.text.xmcs_get_sniffer_mode,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	xmcs_get_sniffer_mode
	.type	xmcs_get_sniffer_mode, @function
xmcs_get_sniffer_mode:
	.frame	$sp,24,$31		# vars= 0, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-24
	sw	$31,20($sp)
	sw	$4,24($sp)
	lw	$4,24($sp)
	lui	$2,%hi(gponDevGetSniffMode)
	addiu	$2,$2,%lo(gponDevGetSniffMode)
	jalr	$2
	nop

	lw	$31,20($sp)
	addiu	$sp,$sp,24
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	xmcs_get_sniffer_mode
	.size	xmcs_get_sniffer_mode, .-xmcs_get_sniffer_mode
	.section	.text.xmcs_set_ext_bst_len_ploamd_filter,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	xmcs_set_ext_bst_len_ploamd_filter
	.type	xmcs_set_ext_bst_len_ploamd_filter, @function
xmcs_set_ext_bst_len_ploamd_filter:
	.frame	$sp,24,$31		# vars= 0, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-24
	sw	$31,20($sp)
	sw	$4,24($sp)
	lw	$4,24($sp)
	lui	$2,%hi(gponDevSetFilterExtBurstLengthPLOAM)
	addiu	$2,$2,%lo(gponDevSetFilterExtBurstLengthPLOAM)
	jalr	$2
	nop

	lw	$31,20($sp)
	addiu	$sp,$sp,24
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	xmcs_set_ext_bst_len_ploamd_filter
	.size	xmcs_set_ext_bst_len_ploamd_filter, .-xmcs_set_ext_bst_len_ploamd_filter
	.section	.text.xmcs_get_ext_bst_len_ploamd_filter,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	xmcs_get_ext_bst_len_ploamd_filter
	.type	xmcs_get_ext_bst_len_ploamd_filter, @function
xmcs_get_ext_bst_len_ploamd_filter:
	.frame	$sp,24,$31		# vars= 0, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-24
	sw	$31,20($sp)
	sw	$4,24($sp)
	lw	$4,24($sp)
	lui	$2,%hi(gponDevGetFilterExtBurstLengthPLOAM)
	addiu	$2,$2,%lo(gponDevGetFilterExtBurstLengthPLOAM)
	jalr	$2
	nop

	lw	$31,20($sp)
	addiu	$sp,$sp,24
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	xmcs_get_ext_bst_len_ploamd_filter
	.size	xmcs_get_ext_bst_len_ploamd_filter, .-xmcs_get_ext_bst_len_ploamd_filter
	.section	.text.xmcs_set_up_overhead_ploamd_filter,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	xmcs_set_up_overhead_ploamd_filter
	.type	xmcs_set_up_overhead_ploamd_filter, @function
xmcs_set_up_overhead_ploamd_filter:
	.frame	$sp,24,$31		# vars= 0, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-24
	sw	$31,20($sp)
	sw	$4,24($sp)
	lw	$4,24($sp)
	lui	$2,%hi(gponDevSetFilterUpstreamOverheadPLOAM)
	addiu	$2,$2,%lo(gponDevSetFilterUpstreamOverheadPLOAM)
	jalr	$2
	nop

	lw	$31,20($sp)
	addiu	$sp,$sp,24
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	xmcs_set_up_overhead_ploamd_filter
	.size	xmcs_set_up_overhead_ploamd_filter, .-xmcs_set_up_overhead_ploamd_filter
	.section	.text.xmcs_get_up_overhead_ploamd_filter,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	xmcs_get_up_overhead_ploamd_filter
	.type	xmcs_get_up_overhead_ploamd_filter, @function
xmcs_get_up_overhead_ploamd_filter:
	.frame	$sp,24,$31		# vars= 0, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-24
	sw	$31,20($sp)
	sw	$4,24($sp)
	lw	$4,24($sp)
	lui	$2,%hi(gponDevGetFilterUpstreamOverheadPLOAM)
	addiu	$2,$2,%lo(gponDevGetFilterUpstreamOverheadPLOAM)
	jalr	$2
	nop

	lw	$31,20($sp)
	addiu	$sp,$sp,24
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	xmcs_get_up_overhead_ploamd_filter
	.size	xmcs_get_up_overhead_ploamd_filter, .-xmcs_get_up_overhead_ploamd_filter
	.section	.text.xmcs_set_tx_4bytes_align,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	xmcs_set_tx_4bytes_align
	.type	xmcs_set_tx_4bytes_align, @function
xmcs_set_tx_4bytes_align:
	.frame	$sp,24,$31		# vars= 0, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-24
	sw	$31,20($sp)
	sw	$4,24($sp)
	lw	$4,24($sp)
	lui	$2,%hi(gponDevSetTx4bytesAlign)
	addiu	$2,$2,%lo(gponDevSetTx4bytesAlign)
	jalr	$2
	nop

	lw	$31,20($sp)
	addiu	$sp,$sp,24
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	xmcs_set_tx_4bytes_align
	.size	xmcs_set_tx_4bytes_align, .-xmcs_set_tx_4bytes_align
	.section	.text.xmcs_get_tx_4bytes_align,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	xmcs_get_tx_4bytes_align
	.type	xmcs_get_tx_4bytes_align, @function
xmcs_get_tx_4bytes_align:
	.frame	$sp,24,$31		# vars= 0, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-24
	sw	$31,20($sp)
	sw	$4,24($sp)
	lw	$4,24($sp)
	lui	$2,%hi(gponDevGetTx4bytesAlign)
	addiu	$2,$2,%lo(gponDevGetTx4bytesAlign)
	jalr	$2
	nop

	lw	$31,20($sp)
	addiu	$sp,$sp,24
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	xmcs_get_tx_4bytes_align
	.size	xmcs_get_tx_4bytes_align, .-xmcs_get_tx_4bytes_align
	.section	.text.xmcs_set_dba_backdoor,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	xmcs_set_dba_backdoor
	.type	xmcs_set_dba_backdoor, @function
xmcs_set_dba_backdoor:
	.frame	$sp,24,$31		# vars= 0, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-24
	sw	$31,20($sp)
	sw	$4,24($sp)
	lw	$4,24($sp)
	lui	$2,%hi(gponDevSetDbaBackdoor)
	addiu	$2,$2,%lo(gponDevSetDbaBackdoor)
	jalr	$2
	nop

	lw	$31,20($sp)
	addiu	$sp,$sp,24
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	xmcs_set_dba_backdoor
	.size	xmcs_set_dba_backdoor, .-xmcs_set_dba_backdoor
	.section	.text.xmcs_set_dba_backdoor_total,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	xmcs_set_dba_backdoor_total
	.type	xmcs_set_dba_backdoor_total, @function
xmcs_set_dba_backdoor_total:
	.frame	$sp,24,$31		# vars= 0, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-24
	sw	$31,20($sp)
	sw	$4,24($sp)
	lw	$4,24($sp)
	lui	$2,%hi(gponDevSetDbaBackdoorTotal)
	addiu	$2,$2,%lo(gponDevSetDbaBackdoorTotal)
	jalr	$2
	nop

	lw	$31,20($sp)
	addiu	$sp,$sp,24
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	xmcs_set_dba_backdoor_total
	.size	xmcs_set_dba_backdoor_total, .-xmcs_set_dba_backdoor_total
	.section	.text.xmcs_set_dba_backdoor_green,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	xmcs_set_dba_backdoor_green
	.type	xmcs_set_dba_backdoor_green, @function
xmcs_set_dba_backdoor_green:
	.frame	$sp,24,$31		# vars= 0, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-24
	sw	$31,20($sp)
	sw	$4,24($sp)
	lw	$4,24($sp)
	lui	$2,%hi(gponDevSetDbaBackdoorGreen)
	addiu	$2,$2,%lo(gponDevSetDbaBackdoorGreen)
	jalr	$2
	nop

	lw	$31,20($sp)
	addiu	$sp,$sp,24
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	xmcs_set_dba_backdoor_green
	.size	xmcs_set_dba_backdoor_green, .-xmcs_set_dba_backdoor_green
	.section	.text.xmcs_set_dba_backdoor_yellow,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	xmcs_set_dba_backdoor_yellow
	.type	xmcs_set_dba_backdoor_yellow, @function
xmcs_set_dba_backdoor_yellow:
	.frame	$sp,24,$31		# vars= 0, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-24
	sw	$31,20($sp)
	sw	$4,24($sp)
	lw	$4,24($sp)
	lui	$2,%hi(gponDevSetDbaBackdoorYellow)
	addiu	$2,$2,%lo(gponDevSetDbaBackdoorYellow)
	jalr	$2
	nop

	lw	$31,20($sp)
	addiu	$sp,$sp,24
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	xmcs_set_dba_backdoor_yellow
	.size	xmcs_set_dba_backdoor_yellow, .-xmcs_set_dba_backdoor_yellow
	.section	.text.xmcs_get_dba_backdoor,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	xmcs_get_dba_backdoor
	.type	xmcs_get_dba_backdoor, @function
xmcs_get_dba_backdoor:
	.frame	$sp,24,$31		# vars= 0, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-24
	sw	$31,20($sp)
	sw	$4,24($sp)
	lw	$4,24($sp)
	lui	$2,%hi(gponDevGetDbaBackdoor)
	addiu	$2,$2,%lo(gponDevGetDbaBackdoor)
	jalr	$2
	nop

	lw	$31,20($sp)
	addiu	$sp,$sp,24
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	xmcs_get_dba_backdoor
	.size	xmcs_get_dba_backdoor, .-xmcs_get_dba_backdoor
	.section	.text.xmcs_set_dba_slight_modify,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	xmcs_set_dba_slight_modify
	.type	xmcs_set_dba_slight_modify, @function
xmcs_set_dba_slight_modify:
	.frame	$sp,24,$31		# vars= 0, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-24
	sw	$31,20($sp)
	sw	$4,24($sp)
	lw	$4,24($sp)
	lui	$2,%hi(gponDevSetDbaSlightModify)
	addiu	$2,$2,%lo(gponDevSetDbaSlightModify)
	jalr	$2
	nop

	lw	$31,20($sp)
	addiu	$sp,$sp,24
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	xmcs_set_dba_slight_modify
	.size	xmcs_set_dba_slight_modify, .-xmcs_set_dba_slight_modify
	.section	.text.xmcs_set_dba_slight_modify_total,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	xmcs_set_dba_slight_modify_total
	.type	xmcs_set_dba_slight_modify_total, @function
xmcs_set_dba_slight_modify_total:
	.frame	$sp,24,$31		# vars= 0, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-24
	sw	$31,20($sp)
	move	$2,$4
	sh	$2,24($sp)
	lhu	$2,24($sp)
	move	$4,$2
	lui	$2,%hi(gponDevSetDbaSlightModifyTotal)
	addiu	$2,$2,%lo(gponDevSetDbaSlightModifyTotal)
	jalr	$2
	nop

	lw	$31,20($sp)
	addiu	$sp,$sp,24
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	xmcs_set_dba_slight_modify_total
	.size	xmcs_set_dba_slight_modify_total, .-xmcs_set_dba_slight_modify_total
	.section	.text.xmcs_set_dba_slight_modify_green,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	xmcs_set_dba_slight_modify_green
	.type	xmcs_set_dba_slight_modify_green, @function
xmcs_set_dba_slight_modify_green:
	.frame	$sp,24,$31		# vars= 0, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-24
	sw	$31,20($sp)
	move	$2,$4
	sh	$2,24($sp)
	lhu	$2,24($sp)
	move	$4,$2
	lui	$2,%hi(gponDevSetDbaSlightModifyGreen)
	addiu	$2,$2,%lo(gponDevSetDbaSlightModifyGreen)
	jalr	$2
	nop

	lw	$31,20($sp)
	addiu	$sp,$sp,24
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	xmcs_set_dba_slight_modify_green
	.size	xmcs_set_dba_slight_modify_green, .-xmcs_set_dba_slight_modify_green
	.section	.text.xmcs_set_dba_slight_modify_yellow,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	xmcs_set_dba_slight_modify_yellow
	.type	xmcs_set_dba_slight_modify_yellow, @function
xmcs_set_dba_slight_modify_yellow:
	.frame	$sp,24,$31		# vars= 0, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-24
	sw	$31,20($sp)
	move	$2,$4
	sh	$2,24($sp)
	lhu	$2,24($sp)
	move	$4,$2
	lui	$2,%hi(gponDevSetDbaSlightModifyYellow)
	addiu	$2,$2,%lo(gponDevSetDbaSlightModifyYellow)
	jalr	$2
	nop

	lw	$31,20($sp)
	addiu	$sp,$sp,24
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	xmcs_set_dba_slight_modify_yellow
	.size	xmcs_set_dba_slight_modify_yellow, .-xmcs_set_dba_slight_modify_yellow
	.section	.text.xmcs_get_dba_slight_modify,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	xmcs_get_dba_slight_modify
	.type	xmcs_get_dba_slight_modify, @function
xmcs_get_dba_slight_modify:
	.frame	$sp,24,$31		# vars= 0, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-24
	sw	$31,20($sp)
	sw	$4,24($sp)
	lw	$4,24($sp)
	lui	$2,%hi(gponDevGetDbaSlightModify)
	addiu	$2,$2,%lo(gponDevGetDbaSlightModify)
	jalr	$2
	nop

	lw	$31,20($sp)
	addiu	$sp,$sp,24
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	xmcs_get_dba_slight_modify
	.size	xmcs_get_dba_slight_modify, .-xmcs_get_dba_slight_modify
	.section	.text.xmcs_set_o3_o4_ploam_ctrl,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	xmcs_set_o3_o4_ploam_ctrl
	.type	xmcs_set_o3_o4_ploam_ctrl, @function
xmcs_set_o3_o4_ploam_ctrl:
	.frame	$sp,24,$31		# vars= 0, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-24
	sw	$31,20($sp)
	sw	$4,24($sp)
	lw	$4,24($sp)
	lui	$2,%hi(gponDevSetO3O4PloamCtrl)
	addiu	$2,$2,%lo(gponDevSetO3O4PloamCtrl)
	jalr	$2
	nop

	lw	$31,20($sp)
	addiu	$sp,$sp,24
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	xmcs_set_o3_o4_ploam_ctrl
	.size	xmcs_set_o3_o4_ploam_ctrl, .-xmcs_set_o3_o4_ploam_ctrl
	.section	.text.xmcs_get_o3_o4_ploam_ctrl,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	xmcs_get_o3_o4_ploam_ctrl
	.type	xmcs_get_o3_o4_ploam_ctrl, @function
xmcs_get_o3_o4_ploam_ctrl:
	.frame	$sp,24,$31		# vars= 0, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-24
	sw	$31,20($sp)
	sw	$4,24($sp)
	lw	$4,24($sp)
	lui	$2,%hi(gponDevGetO3O4PloamCtrl)
	addiu	$2,$2,%lo(gponDevGetO3O4PloamCtrl)
	jalr	$2
	nop

	lw	$31,20($sp)
	addiu	$sp,$sp,24
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	xmcs_get_o3_o4_ploam_ctrl
	.size	xmcs_get_o3_o4_ploam_ctrl, .-xmcs_get_o3_o4_ploam_ctrl
	.section	.text.xmcs_get_tx_sync_offset,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	xmcs_get_tx_sync_offset
	.type	xmcs_get_tx_sync_offset, @function
xmcs_get_tx_sync_offset:
	.frame	$sp,24,$31		# vars= 0, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-24
	sw	$31,20($sp)
	sw	$4,24($sp)
	lw	$4,24($sp)
	lui	$2,%hi(gponDevGetTxSyncOffset)
	addiu	$2,$2,%lo(gponDevGetTxSyncOffset)
	jalr	$2
	nop

	lw	$31,20($sp)
	addiu	$sp,$sp,24
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	xmcs_get_tx_sync_offset
	.size	xmcs_get_tx_sync_offset, .-xmcs_get_tx_sync_offset
	.section	.text.xmcs_set_aes_key_switch_by_sw,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	xmcs_set_aes_key_switch_by_sw
	.type	xmcs_set_aes_key_switch_by_sw, @function
xmcs_set_aes_key_switch_by_sw:
	.frame	$sp,24,$31		# vars= 0, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-24
	sw	$31,20($sp)
	lui	$2,%hi(gpon_dvt_aes_key_switch_by_sw)
	addiu	$2,$2,%lo(gpon_dvt_aes_key_switch_by_sw)
	jalr	$2
	nop

	lw	$31,20($sp)
	addiu	$sp,$sp,24
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	xmcs_set_aes_key_switch_by_sw
	.size	xmcs_set_aes_key_switch_by_sw, .-xmcs_set_aes_key_switch_by_sw
	.section	.text.xmcs_set_sw_reset,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	xmcs_set_sw_reset
	.type	xmcs_set_sw_reset, @function
xmcs_set_sw_reset:
	.frame	$sp,24,$31		# vars= 0, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-24
	sw	$31,20($sp)
	lui	$2,%hi(gpon_dvt_sw_reset)
	addiu	$2,$2,%lo(gpon_dvt_sw_reset)
	jalr	$2
	nop

	lw	$31,20($sp)
	addiu	$sp,$sp,24
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	xmcs_set_sw_reset
	.size	xmcs_set_sw_reset, .-xmcs_set_sw_reset
	.section	.text.xmcs_set_sw_resync,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	xmcs_set_sw_resync
	.type	xmcs_set_sw_resync, @function
xmcs_set_sw_resync:
	.frame	$sp,24,$31		# vars= 0, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-24
	sw	$31,20($sp)
	lui	$2,%hi(gpon_dvt_sw_resync)
	addiu	$2,$2,%lo(gpon_dvt_sw_resync)
	jalr	$2
	nop

	lw	$31,20($sp)
	addiu	$sp,$sp,24
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	xmcs_set_sw_resync
	.size	xmcs_set_sw_resync, .-xmcs_set_sw_resync
	.section	.text.xmcs_set_1pps_h_w,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	xmcs_set_1pps_h_w
	.type	xmcs_set_1pps_h_w, @function
xmcs_set_1pps_h_w:
	.frame	$sp,24,$31		# vars= 0, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-24
	sw	$31,20($sp)
	sw	$4,24($sp)
	lw	$4,24($sp)
	lui	$2,%hi(gponDevSet1ppsHighWidth)
	addiu	$2,$2,%lo(gponDevSet1ppsHighWidth)
	jalr	$2
	nop

	lw	$31,20($sp)
	addiu	$sp,$sp,24
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	xmcs_set_1pps_h_w
	.size	xmcs_set_1pps_h_w, .-xmcs_set_1pps_h_w
	.section	.text.xmcs_get_1pps_h_w,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	xmcs_get_1pps_h_w
	.type	xmcs_get_1pps_h_w, @function
xmcs_get_1pps_h_w:
	.frame	$sp,24,$31		# vars= 0, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-24
	sw	$31,20($sp)
	sw	$4,24($sp)
	lw	$4,24($sp)
	lui	$2,%hi(gponDevGet1ppsHighWidth)
	addiu	$2,$2,%lo(gponDevGet1ppsHighWidth)
	jalr	$2
	nop

	lw	$31,20($sp)
	addiu	$sp,$sp,24
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	xmcs_get_1pps_h_w
	.size	xmcs_get_1pps_h_w, .-xmcs_get_1pps_h_w
	.section	.text.xmcs_set_send_ploamu_wait_mode,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	xmcs_set_send_ploamu_wait_mode
	.type	xmcs_set_send_ploamu_wait_mode, @function
xmcs_set_send_ploamu_wait_mode:
	.frame	$sp,24,$31		# vars= 0, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-24
	sw	$31,20($sp)
	sw	$4,24($sp)
	lw	$4,24($sp)
	lui	$2,%hi(gponDevSetSendPloamuWaitMode)
	addiu	$2,$2,%lo(gponDevSetSendPloamuWaitMode)
	jalr	$2
	nop

	lw	$31,20($sp)
	addiu	$sp,$sp,24
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	xmcs_set_send_ploamu_wait_mode
	.size	xmcs_set_send_ploamu_wait_mode, .-xmcs_set_send_ploamu_wait_mode
	.section	.text.xmcs_get_send_ploamu_wait_mode,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	xmcs_get_send_ploamu_wait_mode
	.type	xmcs_get_send_ploamu_wait_mode, @function
xmcs_get_send_ploamu_wait_mode:
	.frame	$sp,24,$31		# vars= 0, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-24
	sw	$31,20($sp)
	sw	$4,24($sp)
	lw	$4,24($sp)
	lui	$2,%hi(gponDevGetSendPloamuWaitMode)
	addiu	$2,$2,%lo(gponDevGetSendPloamuWaitMode)
	jalr	$2
	nop

	lw	$31,20($sp)
	addiu	$sp,$sp,24
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	xmcs_get_send_ploamu_wait_mode
	.size	xmcs_get_send_ploamu_wait_mode, .-xmcs_get_send_ploamu_wait_mode
	.section	.text.xmcs_test_send_ploam_sleep_request,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	xmcs_test_send_ploam_sleep_request
	.type	xmcs_test_send_ploam_sleep_request, @function
xmcs_test_send_ploam_sleep_request:
	.frame	$sp,24,$31		# vars= 0, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-24
	sw	$31,20($sp)
	sw	$4,24($sp)
	lw	$4,24($sp)
	lui	$2,%hi(ploam_send_sleep_request_msg)
	addiu	$2,$2,%lo(ploam_send_sleep_request_msg)
	jalr	$2
	nop

	lw	$31,20($sp)
	addiu	$sp,$sp,24
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	xmcs_test_send_ploam_sleep_request
	.size	xmcs_test_send_ploam_sleep_request, .-xmcs_test_send_ploam_sleep_request
	.section	.text.xmcs_test_send_ploam_rei,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	xmcs_test_send_ploam_rei
	.type	xmcs_test_send_ploam_rei, @function
xmcs_test_send_ploam_rei:
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
	addiu	$2,$2,254
	lw	$4,24($sp)
	move	$5,$2
	lui	$2,%hi(ploam_send_rei_msg)
	addiu	$2,$2,%lo(ploam_send_rei_msg)
	jalr	$2
	nop

	lw	$31,20($sp)
	addiu	$sp,$sp,24
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	xmcs_test_send_ploam_rei
	.size	xmcs_test_send_ploam_rei, .-xmcs_test_send_ploam_rei
	.section	.text.xmcs_test_send_ploam_dying_gasp,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	xmcs_test_send_ploam_dying_gasp
	.type	xmcs_test_send_ploam_dying_gasp, @function
xmcs_test_send_ploam_dying_gasp:
	.frame	$sp,32,$31		# vars= 8, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-32
	sw	$31,28($sp)
	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	lw	$3,-30828($2)
	li	$2,-2147483648			# 0xffffffff80000000
	and	$2,$3,$2
	beq	$2,$0,$L203
	nop

	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16924
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	sw	$2,16($sp)
	lw	$2,16($sp)
	li	$3,1			# 0x1
	ins	$2,$3,16,1
	sw	$2,16($sp)
	lw	$3,16($sp)
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,16924
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	j	$L204
	nop

$L203:
	lui	$2,%hi(ploam_send_dying_gasp)
	addiu	$2,$2,%lo(ploam_send_dying_gasp)
	jalr	$2
	nop

$L204:
	move	$2,$0
	lw	$31,28($sp)
	addiu	$sp,$sp,32
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	xmcs_test_send_ploam_dying_gasp
	.size	xmcs_test_send_ploam_dying_gasp, .-xmcs_test_send_ploam_dying_gasp
	.section	.text.xmcs_test_tod,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	xmcs_test_tod
	.type	xmcs_test_tod, @function
xmcs_test_tod:
	.frame	$sp,24,$31		# vars= 0, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-24
	sw	$31,20($sp)
	sw	$4,24($sp)
	lw	$4,24($sp)
	lui	$2,%hi(gpon_dvt_tod)
	addiu	$2,$2,%lo(gpon_dvt_tod)
	jalr	$2
	nop

	lw	$31,20($sp)
	addiu	$sp,$sp,24
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	xmcs_test_tod
	.size	xmcs_test_tod, .-xmcs_test_tod
	.section	.text.xmcs_get_rx_ethernet_frame_counter,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	xmcs_get_rx_ethernet_frame_counter
	.type	xmcs_get_rx_ethernet_frame_counter, @function
xmcs_get_rx_ethernet_frame_counter:
	.frame	$sp,24,$31		# vars= 0, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-24
	sw	$31,20($sp)
	sw	$4,24($sp)
	lw	$2,24($sp)
	sw	$0,0($2)
	lw	$2,24($sp)
	li	$3,2			# 0x2
	sw	$3,4($2)
	li	$2,-1078657024			# 0xffffffffbfb50000
	ori	$4,$2,0x1668
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	move	$3,$2
	lw	$2,24($sp)
	sw	$3,8($2)
	li	$2,-1078657024			# 0xffffffffbfb50000
	ori	$4,$2,0x1664
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	move	$3,$2
	lw	$2,24($sp)
	sw	$3,12($2)
	li	$2,-1078657024			# 0xffffffffbfb50000
	ori	$4,$2,0x1660
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	move	$3,$2
	lw	$2,24($sp)
	sw	$3,16($2)
	li	$2,-1078657024			# 0xffffffffbfb50000
	ori	$4,$2,0x166c
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	move	$3,$2
	lw	$2,24($sp)
	sw	$3,20($2)
	li	$2,-1078657024			# 0xffffffffbfb50000
	ori	$4,$2,0x1670
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	move	$3,$2
	lw	$2,24($sp)
	sw	$3,24($2)
	li	$2,-1078657024			# 0xffffffffbfb50000
	ori	$4,$2,0x1674
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	move	$3,$2
	lw	$2,24($sp)
	sw	$3,28($2)
	li	$2,-1078657024			# 0xffffffffbfb50000
	ori	$4,$2,0x1680
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	move	$3,$2
	lw	$2,24($sp)
	sw	$3,32($2)
	li	$2,-1078657024			# 0xffffffffbfb50000
	ori	$4,$2,0x1684
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	move	$3,$2
	lw	$2,24($sp)
	sw	$3,36($2)
	li	$2,-1078657024			# 0xffffffffbfb50000
	ori	$4,$2,0x1688
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	move	$3,$2
	lw	$2,24($sp)
	sw	$3,40($2)
	li	$2,-1078657024			# 0xffffffffbfb50000
	ori	$4,$2,0x168c
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	move	$3,$2
	lw	$2,24($sp)
	sw	$3,44($2)
	li	$2,-1078657024			# 0xffffffffbfb50000
	ori	$4,$2,0x1690
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	move	$3,$2
	lw	$2,24($sp)
	sw	$3,48($2)
	li	$2,-1078657024			# 0xffffffffbfb50000
	ori	$4,$2,0x1694
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	move	$3,$2
	lw	$2,24($sp)
	sw	$3,52($2)
	li	$2,-1078657024			# 0xffffffffbfb50000
	ori	$4,$2,0x1698
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	move	$3,$2
	lw	$2,24($sp)
	sw	$3,56($2)
	li	$2,-1078657024			# 0xffffffffbfb50000
	ori	$4,$2,0x169c
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	move	$3,$2
	lw	$2,24($sp)
	sw	$3,60($2)
	move	$2,$0
	lw	$31,20($sp)
	addiu	$sp,$sp,24
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	xmcs_get_rx_ethernet_frame_counter
	.size	xmcs_get_rx_ethernet_frame_counter, .-xmcs_get_rx_ethernet_frame_counter
	.section	.text.xmcs_get_gpon_traffic_state,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	xmcs_get_gpon_traffic_state
	.type	xmcs_get_gpon_traffic_state, @function
xmcs_get_gpon_traffic_state:
	.frame	$sp,80,$31		# vars= 56, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	addiu	$sp,$sp,-80
	sw	$31,76($sp)
	sw	$4,80($sp)
	lui	$2,%hi(gwan_get_traffic_status)
	addiu	$2,$2,%lo(gwan_get_traffic_status)
	jalr	$2
	bne	$2,$0,$L211
	lw	$2,80($sp)
	sw	$2,24($sp)
	sw	$0,28($sp)
	li	$2,-14			# 0xfffffffffffffff2
	sw	$2,16($sp)
	lw	$2,24($sp)
	sw	$2,32($sp)
	li	$2,4			# 0x4
	sw	$2,36($sp)
	move	$2,$28
	lw	$2,24($2)
	sw	$2,40($sp)
	lw	$3,32($sp)
	lw	$2,36($sp)
	addu	$3,$3,$2
	lw	$2,32($sp)
	or	$3,$3,$2
	lw	$2,36($sp)
	or	$3,$3,$2
	lw	$2,40($sp)
	and	$2,$3,$2
	sw	$2,44($sp)
	lw	$2,44($sp)
	sltu	$2,$2,1
	andi	$2,$2,0x00ff
	sltu	$2,$0,$2
	andi	$2,$2,0x00ff
	beq	$2,$0,$L212
	move	$2,$0
	lw	$3,28($sp)
	lw	$4,24($sp)
#APP
 # 970 "/opt/tclinux_phoenix/modules/private/xpon/src/xmcs/xmcs_gpon.c" 1
	1:	sw	$3, 0($4)		# __put_user_asm
2:							
	.section	.fixup,"ax"			
3:	li	$2, -14					
	j	2b					
	.previous					
	.section	__ex_table,"a"		
	.word	1b, 3b				
	.previous					

 # 0 "" 2
#NO_APP
	sw	$2,16($sp)
	.set	noreorder
	nop
	.set	reorder
$L212:
	move	$2,$0
	j	$L213
$L211:
	lw	$2,80($sp)
	sw	$2,48($sp)
	li	$2,1			# 0x1
	sw	$2,52($sp)
	li	$2,-14			# 0xfffffffffffffff2
	sw	$2,20($sp)
	lw	$2,48($sp)
	sw	$2,56($sp)
	li	$2,4			# 0x4
	sw	$2,60($sp)
	move	$2,$28
	lw	$2,24($2)
	sw	$2,64($sp)
	lw	$3,56($sp)
	lw	$2,60($sp)
	addu	$3,$3,$2
	lw	$2,56($sp)
	or	$3,$3,$2
	lw	$2,60($sp)
	or	$3,$3,$2
	lw	$2,64($sp)
	and	$2,$3,$2
	sw	$2,68($sp)
	lw	$2,68($sp)
	sltu	$2,$2,1
	andi	$2,$2,0x00ff
	sltu	$2,$0,$2
	andi	$2,$2,0x00ff
	beq	$2,$0,$L214
	move	$2,$0
	lw	$3,52($sp)
	lw	$4,48($sp)
#APP
 # 975 "/opt/tclinux_phoenix/modules/private/xpon/src/xmcs/xmcs_gpon.c" 1
	1:	sw	$3, 0($4)		# __put_user_asm
2:							
	.section	.fixup,"ax"			
3:	li	$2, -14					
	j	2b					
	.previous					
	.section	__ex_table,"a"		
	.word	1b, 3b				
	.previous					

 # 0 "" 2
#NO_APP
	sw	$2,20($sp)
	.set	noreorder
	nop
	.set	reorder
$L214:
	move	$2,$0
$L213:
	lw	$31,76($sp)
	addiu	$sp,$sp,80
	j	$31
	.end	xmcs_get_gpon_traffic_state
	.size	xmcs_get_gpon_traffic_state, .-xmcs_get_gpon_traffic_state
	.section	.text.xmcs_test_gpon_hotplug,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	xmcs_test_gpon_hotplug
	.type	xmcs_test_gpon_hotplug, @function
xmcs_test_gpon_hotplug:
	.frame	$sp,8,$31		# vars= 8, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-8
	sw	$4,8($sp)
	li	$2,-1078984704			# 0xffffffffbfb00000
	ori	$2,$2,0x64
	lw	$3,0($2)
	li	$2,-65536			# 0xffffffffffff0000
	and	$3,$3,$2
	li	$2,524288			# 0x80000
	bne	$3,$2,$L216
	nop

	li	$2,-1078984704			# 0xffffffffbfb00000
	ori	$2,$2,0x9c
	lw	$2,0($2)
	andi	$2,$2,0x1
	sltu	$2,$2,1
	andi	$2,$2,0x00ff
	j	$L217
	nop

$L216:
	li	$2,-1078984704			# 0xffffffffbfb00000
	ori	$2,$2,0x64
	lw	$3,0($2)
	li	$2,-65536			# 0xffffffffffff0000
	and	$3,$3,$2
	li	$2,458752			# 0x70000
	beq	$3,$2,$L218
	nop

	li	$2,-1078984704			# 0xffffffffbfb00000
	ori	$2,$2,0x64
	lw	$3,0($2)
	li	$2,-65536			# 0xffffffffffff0000
	and	$3,$3,$2
	li	$2,524288			# 0x80000
	bne	$3,$2,$L219
	nop

$L218:
	li	$2,-1078984704			# 0xffffffffbfb00000
	ori	$2,$2,0x8c
	sw	$2,0($sp)
	lw	$2,0($sp)
	lw	$3,0($2)
	li	$2,536870912			# 0x20000000
	and	$2,$3,$2
	sltu	$2,$2,1
	andi	$2,$2,0x00ff
	j	$L217
	nop

$L219:
	li	$2,-1078984704			# 0xffffffffbfb00000
	ori	$2,$2,0x8c
	sw	$2,4($sp)
	lw	$2,4($sp)
	lw	$2,0($2)
	srl	$2,$2,31
	andi	$2,$2,0x00ff
$L217:
	beq	$2,$0,$L223
	nop

	lui	$2,%hi(gpGponPriv)
	lw	$3,%lo(gpGponPriv)($2)
	lw	$2,8($sp)
	andi	$2,$2,0x00ff
	andi	$2,$2,0x1
	andi	$4,$2,0x00ff
	lhu	$2,448($3)
	ins	$2,$4,9,1
	sh	$2,448($3)
$L223:
	move	$2,$0
	addiu	$sp,$sp,8
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	xmcs_test_gpon_hotplug
	.size	xmcs_test_gpon_hotplug, .-xmcs_test_gpon_hotplug
	.section	.text.xmcs_get_gpon_online_duration,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	xmcs_get_gpon_online_duration
	.type	xmcs_get_gpon_online_duration, @function
xmcs_get_gpon_online_duration:
	.frame	$sp,88,$31		# vars= 64, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	addiu	$sp,$sp,-88
	sw	$31,84($sp)
	sw	$4,88($sp)
	sw	$0,24($sp)
	lui	$2,%hi(gwan_get_traffic_status)
	addiu	$2,$2,%lo(gwan_get_traffic_status)
	jalr	$2
	bne	$2,$0,$L226
	lui	$2,%hi(jiffies)
	lw	$3,%lo(jiffies)($2)
	lui	$2,%hi(gpPonSysData)
	lw	$4,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$2,$4,$2
	lw	$2,-30196($2)
	subu	$3,$3,$2
	li	$2,1374355456			# 0x51eb0000
	ori	$2,$2,0x851f
	multu	$3,$2
	mfhi	$2
	srl	$2,$2,5
	sw	$2,24($sp)
	lw	$2,88($sp)
	sw	$2,28($sp)
	lw	$2,24($sp)
	sw	$2,32($sp)
	li	$2,-14			# 0xfffffffffffffff2
	sw	$2,16($sp)
	lw	$2,28($sp)
	sw	$2,36($sp)
	li	$2,4			# 0x4
	sw	$2,40($sp)
	move	$2,$28
	lw	$2,24($2)
	sw	$2,44($sp)
	lw	$3,36($sp)
	lw	$2,40($sp)
	addu	$3,$3,$2
	lw	$2,36($sp)
	or	$3,$3,$2
	lw	$2,40($sp)
	or	$3,$3,$2
	lw	$2,44($sp)
	and	$2,$3,$2
	sw	$2,48($sp)
	lw	$2,48($sp)
	sltu	$2,$2,1
	andi	$2,$2,0x00ff
	sltu	$2,$0,$2
	andi	$2,$2,0x00ff
	beq	$2,$0,$L227
	move	$2,$0
	lw	$3,32($sp)
	lw	$4,28($sp)
#APP
 # 1015 "/opt/tclinux_phoenix/modules/private/xpon/src/xmcs/xmcs_gpon.c" 1
	1:	sw	$3, 0($4)		# __put_user_asm
2:							
	.section	.fixup,"ax"			
3:	li	$2, -14					
	j	2b					
	.previous					
	.section	__ex_table,"a"		
	.word	1b, 3b				
	.previous					

 # 0 "" 2
#NO_APP
	sw	$2,16($sp)
	.set	noreorder
	nop
	.set	reorder
$L227:
	move	$2,$0
	j	$L228
$L226:
	lw	$2,88($sp)
	sw	$2,52($sp)
	lw	$2,24($sp)
	sw	$2,56($sp)
	li	$2,-14			# 0xfffffffffffffff2
	sw	$2,20($sp)
	lw	$2,52($sp)
	sw	$2,60($sp)
	li	$2,4			# 0x4
	sw	$2,64($sp)
	move	$2,$28
	lw	$2,24($2)
	sw	$2,68($sp)
	lw	$3,60($sp)
	lw	$2,64($sp)
	addu	$3,$3,$2
	lw	$2,60($sp)
	or	$3,$3,$2
	lw	$2,64($sp)
	or	$3,$3,$2
	lw	$2,68($sp)
	and	$2,$3,$2
	sw	$2,72($sp)
	lw	$2,72($sp)
	sltu	$2,$2,1
	andi	$2,$2,0x00ff
	sltu	$2,$0,$2
	andi	$2,$2,0x00ff
	beq	$2,$0,$L229
	move	$2,$0
	lw	$3,56($sp)
	lw	$4,52($sp)
#APP
 # 1020 "/opt/tclinux_phoenix/modules/private/xpon/src/xmcs/xmcs_gpon.c" 1
	1:	sw	$3, 0($4)		# __put_user_asm
2:							
	.section	.fixup,"ax"			
3:	li	$2, -14					
	j	2b					
	.previous					
	.section	__ex_table,"a"		
	.word	1b, 3b				
	.previous					

 # 0 "" 2
#NO_APP
	sw	$2,20($sp)
	.set	noreorder
	nop
	.set	reorder
$L229:
	move	$2,$0
$L228:
	lw	$31,84($sp)
	addiu	$sp,$sp,88
	j	$31
	.end	xmcs_get_gpon_online_duration
	.size	xmcs_get_gpon_online_duration, .-xmcs_get_gpon_online_duration
	.rdata
	.align	2
$LC5:
	.ascii	"[%lu0ms]No such I/O command, cmd: %x\012\000"
	.section	.text.gpon_cmd_proc,"ax",@progbits
	.align	2
	.globl	gpon_cmd_proc
	.set	nomips16
	.set	nomicromips
	.ent	gpon_cmd_proc
	.type	gpon_cmd_proc, @function
gpon_cmd_proc:
	.frame	$sp,32,$31		# vars= 8, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-32
	sw	$31,28($sp)
	sw	$4,32($sp)
	sw	$5,36($sp)
	li	$2,-1			# 0xffffffffffffffff
	sw	$2,16($sp)
	lw	$2,32($sp)
	li	$3,-2147352576			# 0xffffffff80020000
	ori	$3,$3,0xd934
	beq	$2,$3,$L232
	nop

	li	$3,-2147352576			# 0xffffffff80020000
	ori	$3,$3,0xd935
	sltu	$3,$2,$3
	beq	$3,$0,$L233
	nop

	li	$3,1074003968			# 0x40040000
	ori	$3,$3,0xd93b
	beq	$2,$3,$L234
	nop

	li	$3,1074003968			# 0x40040000
	ori	$3,$3,0xd93c
	sltu	$3,$2,$3
	beq	$3,$0,$L235
	nop

	li	$3,1074003968			# 0x40040000
	ori	$3,$3,0xd921
	beq	$2,$3,$L236
	nop

	li	$3,1074003968			# 0x40040000
	ori	$3,$3,0xd922
	sltu	$3,$2,$3
	beq	$3,$0,$L237
	nop

	li	$3,1074003968			# 0x40040000
	ori	$3,$3,0xd90d
	beq	$2,$3,$L238
	nop

	li	$3,1074003968			# 0x40040000
	ori	$3,$3,0xd90e
	sltu	$3,$2,$3
	beq	$3,$0,$L239
	nop

	li	$3,1074003968			# 0x40040000
	ori	$3,$3,0xd909
	beq	$2,$3,$L240
	nop

	li	$3,1074003968			# 0x40040000
	ori	$3,$3,0xd90a
	sltu	$3,$2,$3
	beq	$3,$0,$L241
	nop

	li	$3,1074003968			# 0x40040000
	ori	$3,$3,0xd908
	beq	$2,$3,$L242
	nop

	j	$L231
	nop

$L241:
	li	$3,1074003968			# 0x40040000
	ori	$3,$3,0xd90a
	beq	$2,$3,$L243
	nop

	li	$3,1074003968			# 0x40040000
	ori	$3,$3,0xd90b
	beq	$2,$3,$L244
	nop

	j	$L231
	nop

$L239:
	li	$3,1074003968			# 0x40040000
	ori	$3,$3,0xd914
	beq	$2,$3,$L245
	nop

	li	$3,1074003968			# 0x40040000
	ori	$3,$3,0xd915
	sltu	$3,$2,$3
	beq	$3,$0,$L246
	nop

	li	$3,1074003968			# 0x40040000
	ori	$3,$3,0xd910
	beq	$2,$3,$L247
	nop

	li	$3,1074003968			# 0x40040000
	ori	$3,$3,0xd911
	beq	$2,$3,$L248
	nop

	j	$L231
	nop

$L246:
	li	$3,1074003968			# 0x40040000
	ori	$3,$3,0xd915
	beq	$2,$3,$L249
	nop

	li	$3,1074003968			# 0x40040000
	ori	$3,$3,0xd920
	beq	$2,$3,$L250
	nop

	j	$L231
	nop

$L237:
	li	$3,1074003968			# 0x40040000
	ori	$3,$3,0xd933
	beq	$2,$3,$L251
	nop

	li	$3,1074003968			# 0x40040000
	ori	$3,$3,0xd934
	sltu	$3,$2,$3
	beq	$3,$0,$L252
	nop

	li	$3,1074003968			# 0x40040000
	ori	$3,$3,0xd923
	beq	$2,$3,$L253
	nop

	li	$3,1074003968			# 0x40040000
	ori	$3,$3,0xd923
	sltu	$3,$2,$3
	bne	$3,$0,$L254
	nop

	li	$3,1074003968			# 0x40040000
	ori	$3,$3,0xd930
	beq	$2,$3,$L255
	nop

	li	$3,1074003968			# 0x40040000
	ori	$3,$3,0xd932
	beq	$2,$3,$L256
	nop

	j	$L231
	nop

$L252:
	li	$3,1074003968			# 0x40040000
	ori	$3,$3,0xd936
	beq	$2,$3,$L257
	nop

	li	$3,1074003968			# 0x40040000
	ori	$3,$3,0xd937
	sltu	$3,$2,$3
	beq	$3,$0,$L258
	nop

	li	$3,1074003968			# 0x40040000
	ori	$3,$3,0xd934
	beq	$2,$3,$L259
	nop

	li	$3,1074003968			# 0x40040000
	ori	$3,$3,0xd935
	beq	$2,$3,$L260
	nop

	j	$L231
	nop

$L258:
	li	$3,1074003968			# 0x40040000
	ori	$3,$3,0xd939
	beq	$2,$3,$L261
	nop

	li	$3,1074003968			# 0x40040000
	ori	$3,$3,0xd93a
	beq	$2,$3,$L262
	nop

	j	$L231
	nop

$L235:
	li	$3,1074003968			# 0x40040000
	ori	$3,$3,0xd955
	beq	$2,$3,$L263
	nop

	li	$3,1074003968			# 0x40040000
	ori	$3,$3,0xd956
	sltu	$3,$2,$3
	beq	$3,$0,$L264
	nop

	li	$3,1074003968			# 0x40040000
	ori	$3,$3,0xd940
	beq	$2,$3,$L265
	nop

	li	$3,1074003968			# 0x40040000
	ori	$3,$3,0xd941
	sltu	$3,$2,$3
	beq	$3,$0,$L266
	nop

	li	$3,1074003968			# 0x40040000
	ori	$3,$3,0xd93d
	beq	$2,$3,$L267
	nop

	li	$3,1074003968			# 0x40040000
	ori	$3,$3,0xd93d
	sltu	$3,$2,$3
	bne	$3,$0,$L268
	nop

	li	$3,1074003968			# 0x40040000
	ori	$3,$3,0xd93e
	beq	$2,$3,$L269
	nop

	li	$3,1074003968			# 0x40040000
	ori	$3,$3,0xd93f
	beq	$2,$3,$L270
	nop

	j	$L231
	nop

$L266:
	li	$3,1074003968			# 0x40040000
	ori	$3,$3,0xd949
	beq	$2,$3,$L271
	nop

	li	$3,1074003968			# 0x40040000
	ori	$3,$3,0xd94a
	sltu	$3,$2,$3
	beq	$3,$0,$L272
	nop

	li	$3,1074003968			# 0x40040000
	ori	$3,$3,0xd941
	beq	$2,$3,$L273
	nop

	li	$3,1074003968			# 0x40040000
	ori	$3,$3,0xd945
	beq	$2,$3,$L274
	nop

	j	$L231
	nop

$L272:
	li	$3,1074003968			# 0x40040000
	ori	$3,$3,0xd94a
	beq	$2,$3,$L275
	nop

	li	$3,1074003968			# 0x40040000
	ori	$3,$3,0xd954
	beq	$2,$3,$L276
	nop

	j	$L231
	nop

$L264:
	li	$3,-2147418112			# 0xffffffff80010000
	ori	$3,$3,0xd924
	beq	$2,$3,$L277
	nop

	li	$3,-2147418112			# 0xffffffff80010000
	ori	$3,$3,0xd925
	sltu	$3,$2,$3
	beq	$3,$0,$L278
	nop

	li	$3,1074003968			# 0x40040000
	ori	$3,$3,0xd961
	beq	$2,$3,$L279
	nop

	li	$3,1074003968			# 0x40040000
	ori	$3,$3,0xd962
	sltu	$3,$2,$3
	beq	$3,$0,$L280
	nop

	li	$3,1074003968			# 0x40040000
	ori	$3,$3,0xd958
	beq	$2,$3,$L281
	nop

	j	$L231
	nop

$L280:
	li	$3,1074003968			# 0x40040000
	ori	$3,$3,0xd963
	beq	$2,$3,$L282
	nop

	li	$3,-2147418112			# 0xffffffff80010000
	ori	$3,$3,0xd923
	beq	$2,$3,$L283
	nop

	j	$L231
	nop

$L278:
	li	$3,-2147352576			# 0xffffffff80020000
	ori	$3,$3,0xd90c
	beq	$2,$3,$L284
	nop

	li	$3,-2147352576			# 0xffffffff80020000
	ori	$3,$3,0xd90d
	sltu	$3,$2,$3
	beq	$3,$0,$L285
	nop

	li	$3,-2147418112			# 0xffffffff80010000
	ori	$3,$3,0xd925
	beq	$2,$3,$L286
	nop

	li	$3,-2147418112			# 0xffffffff80010000
	ori	$3,$3,0xd935
	beq	$2,$3,$L287
	nop

	j	$L231
	nop

$L285:
	li	$3,-2147352576			# 0xffffffff80020000
	ori	$3,$3,0xd911
	beq	$2,$3,$L288
	nop

	li	$3,-2147352576			# 0xffffffff80020000
	ori	$3,$3,0xd930
	beq	$2,$3,$L289
	nop

	j	$L231
	nop

$L233:
	li	$3,-2147221504			# 0xffffffff80040000
	ori	$3,$3,0xd93c
	beq	$2,$3,$L290
	nop

	li	$3,-2147221504			# 0xffffffff80040000
	ori	$3,$3,0xd93d
	sltu	$3,$2,$3
	beq	$3,$0,$L291
	nop

	li	$3,-2147221504			# 0xffffffff80040000
	ori	$3,$3,0xd912
	beq	$2,$3,$L292
	nop

	li	$3,-2147221504			# 0xffffffff80040000
	ori	$3,$3,0xd913
	sltu	$3,$2,$3
	beq	$3,$0,$L293
	nop

	li	$3,-2147221504			# 0xffffffff80040000
	ori	$3,$3,0xd902
	beq	$2,$3,$L294
	nop

	li	$3,-2147221504			# 0xffffffff80040000
	ori	$3,$3,0xd903
	sltu	$3,$2,$3
	beq	$3,$0,$L295
	nop

	li	$3,-2147352576			# 0xffffffff80020000
	ori	$3,$3,0xd947
	beq	$2,$3,$L296
	nop

	li	$3,-2147352576			# 0xffffffff80020000
	ori	$3,$3,0xd948
	sltu	$3,$2,$3
	beq	$3,$0,$L297
	nop

	li	$3,-2147352576			# 0xffffffff80020000
	ori	$3,$3,0xd946
	beq	$2,$3,$L298
	nop

	j	$L231
	nop

$L297:
	li	$3,-2147352576			# 0xffffffff80020000
	ori	$3,$3,0xd948
	beq	$2,$3,$L299
	nop

	li	$3,-2147221504			# 0xffffffff80040000
	ori	$3,$3,0xd901
	beq	$2,$3,$L300
	nop

	j	$L231
	nop

$L295:
	li	$3,-2147221504			# 0xffffffff80040000
	ori	$3,$3,0xd905
	beq	$2,$3,$L301
	nop

	li	$3,-2147221504			# 0xffffffff80040000
	ori	$3,$3,0xd906
	sltu	$3,$2,$3
	beq	$3,$0,$L302
	nop

	li	$3,-2147221504			# 0xffffffff80040000
	ori	$3,$3,0xd903
	beq	$2,$3,$L303
	nop

	li	$3,-2147221504			# 0xffffffff80040000
	ori	$3,$3,0xd904
	beq	$2,$3,$L304
	nop

	j	$L231
	nop

$L302:
	li	$3,-2147221504			# 0xffffffff80040000
	ori	$3,$3,0xd906
	beq	$2,$3,$L305
	nop

	li	$3,-2147221504			# 0xffffffff80040000
	ori	$3,$3,0xd907
	beq	$2,$3,$L306
	nop

	j	$L231
	nop

$L293:
	li	$3,-2147221504			# 0xffffffff80040000
	ori	$3,$3,0xd936
	beq	$2,$3,$L307
	nop

	li	$3,-2147221504			# 0xffffffff80040000
	ori	$3,$3,0xd937
	sltu	$3,$2,$3
	beq	$3,$0,$L308
	nop

	li	$3,-2147221504			# 0xffffffff80040000
	ori	$3,$3,0xd915
	beq	$2,$3,$L309
	nop

	li	$3,-2147221504			# 0xffffffff80040000
	ori	$3,$3,0xd916
	sltu	$3,$2,$3
	beq	$3,$0,$L310
	nop

	li	$3,-2147221504			# 0xffffffff80040000
	ori	$3,$3,0xd913
	beq	$2,$3,$L311
	nop

	j	$L231
	nop

$L310:
	li	$3,-2147221504			# 0xffffffff80040000
	ori	$3,$3,0xd920
	beq	$2,$3,$L312
	nop

	li	$3,-2147221504			# 0xffffffff80040000
	ori	$3,$3,0xd933
	beq	$2,$3,$L313
	nop

	j	$L231
	nop

$L308:
	li	$3,-2147221504			# 0xffffffff80040000
	ori	$3,$3,0xd939
	beq	$2,$3,$L314
	nop

	li	$3,-2147221504			# 0xffffffff80040000
	ori	$3,$3,0xd93a
	sltu	$3,$2,$3
	beq	$3,$0,$L315
	nop

	li	$3,-2147221504			# 0xffffffff80040000
	ori	$3,$3,0xd937
	beq	$2,$3,$L316
	nop

	li	$3,-2147221504			# 0xffffffff80040000
	ori	$3,$3,0xd938
	beq	$2,$3,$L317
	nop

	j	$L231
	nop

$L315:
	li	$3,-2147221504			# 0xffffffff80040000
	ori	$3,$3,0xd93a
	beq	$2,$3,$L318
	nop

	li	$3,-2147221504			# 0xffffffff80040000
	ori	$3,$3,0xd93b
	beq	$2,$3,$L319
	nop

	j	$L231
	nop

$L291:
	li	$3,-2147221504			# 0xffffffff80040000
	ori	$3,$3,0xd94b
	beq	$2,$3,$L320
	nop

	li	$3,-2147221504			# 0xffffffff80040000
	ori	$3,$3,0xd94c
	sltu	$3,$2,$3
	beq	$3,$0,$L321
	nop

	li	$3,-2147221504			# 0xffffffff80040000
	ori	$3,$3,0xd941
	beq	$2,$3,$L322
	nop

	li	$3,-2147221504			# 0xffffffff80040000
	ori	$3,$3,0xd942
	sltu	$3,$2,$3
	beq	$3,$0,$L323
	nop

	li	$3,-2147221504			# 0xffffffff80040000
	ori	$3,$3,0xd93e
	beq	$2,$3,$L324
	nop

	li	$3,-2147221504			# 0xffffffff80040000
	ori	$3,$3,0xd93e
	sltu	$3,$2,$3
	bne	$3,$0,$L325
	nop

	li	$3,-2147221504			# 0xffffffff80040000
	ori	$3,$3,0xd93f
	beq	$2,$3,$L326
	nop

	li	$3,-2147221504			# 0xffffffff80040000
	ori	$3,$3,0xd940
	beq	$2,$3,$L327
	nop

	j	$L231
	nop

$L323:
	li	$3,-2147221504			# 0xffffffff80040000
	ori	$3,$3,0xd944
	beq	$2,$3,$L328
	nop

	li	$3,-2147221504			# 0xffffffff80040000
	ori	$3,$3,0xd945
	sltu	$3,$2,$3
	beq	$3,$0,$L329
	nop

	li	$3,-2147221504			# 0xffffffff80040000
	ori	$3,$3,0xd942
	beq	$2,$3,$L330
	nop

	li	$3,-2147221504			# 0xffffffff80040000
	ori	$3,$3,0xd943
	beq	$2,$3,$L331
	nop

	j	$L231
	nop

$L329:
	li	$3,-2147221504			# 0xffffffff80040000
	ori	$3,$3,0xd945
	beq	$2,$3,$L332
	nop

	li	$3,-2147221504			# 0xffffffff80040000
	ori	$3,$3,0xd949
	beq	$2,$3,$L333
	nop

	j	$L231
	nop

$L321:
	li	$3,-2147221504			# 0xffffffff80040000
	ori	$3,$3,0xd951
	beq	$2,$3,$L334
	nop

	li	$3,-2147221504			# 0xffffffff80040000
	ori	$3,$3,0xd952
	sltu	$3,$2,$3
	beq	$3,$0,$L335
	nop

	li	$3,-2147221504			# 0xffffffff80040000
	ori	$3,$3,0xd94d
	beq	$2,$3,$L336
	nop

	li	$3,-2147221504			# 0xffffffff80040000
	ori	$3,$3,0xd94d
	sltu	$3,$2,$3
	bne	$3,$0,$L337
	nop

	li	$3,-2147221504			# 0xffffffff80040000
	ori	$3,$3,0xd94e
	beq	$2,$3,$L338
	nop

	li	$3,-2147221504			# 0xffffffff80040000
	ori	$3,$3,0xd950
	beq	$2,$3,$L339
	nop

	j	$L231
	nop

$L335:
	li	$3,-2147221504			# 0xffffffff80040000
	ori	$3,$3,0xd957
	beq	$2,$3,$L340
	nop

	li	$3,-2147221504			# 0xffffffff80040000
	ori	$3,$3,0xd958
	sltu	$3,$2,$3
	beq	$3,$0,$L341
	nop

	li	$3,-2147221504			# 0xffffffff80040000
	ori	$3,$3,0xd952
	beq	$2,$3,$L342
	nop

	li	$3,-2147221504			# 0xffffffff80040000
	ori	$3,$3,0xd953
	beq	$2,$3,$L343
	nop

	j	$L231
	nop

$L341:
	li	$3,-2147221504			# 0xffffffff80040000
	ori	$3,$3,0xd960
	beq	$2,$3,$L344
	nop

	li	$3,-2147221504			# 0xffffffff80040000
	ori	$3,$3,0xd962
	beq	$2,$3,$L345
	nop

	j	$L231
	nop

$L300:
	lw	$2,36($sp)
	move	$4,$2
	lui	$2,%hi(xmcs_set_sn_passwd)
	addiu	$2,$2,%lo(xmcs_set_sn_passwd)
	jalr	$2
	nop

	sw	$2,16($sp)
	j	$L346
	nop

$L294:
	lw	$2,36($sp)
	move	$4,$2
	lui	$2,%hi(xmcs_set_sn)
	addiu	$2,$2,%lo(xmcs_set_sn)
	jalr	$2
	nop

	sw	$2,16($sp)
	j	$L346
	nop

$L303:
	lw	$2,36($sp)
	move	$4,$2
	lui	$2,%hi(xmcs_set_passwd)
	addiu	$2,$2,%lo(xmcs_set_passwd)
	jalr	$2
	nop

	sw	$2,16($sp)
	j	$L346
	nop

$L304:
	lw	$4,36($sp)
	lui	$2,%hi(xmcs_set_emergency_state)
	addiu	$2,$2,%lo(xmcs_set_emergency_state)
	jalr	$2
	nop

	sw	$2,16($sp)
	j	$L346
	nop

$L301:
	lw	$2,36($sp)
	move	$4,$2
	lui	$2,%hi(xmcs_set_act_timer)
	addiu	$2,$2,%lo(xmcs_set_act_timer)
	jalr	$2
	nop

	sw	$2,16($sp)
	j	$L346
	nop

$L305:
	lw	$4,36($sp)
	lui	$2,%hi(xmcs_set_act_timer_to1)
	addiu	$2,$2,%lo(xmcs_set_act_timer_to1)
	jalr	$2
	nop

	sw	$2,16($sp)
	j	$L346
	nop

$L306:
	lw	$4,36($sp)
	lui	$2,%hi(xmcs_set_act_timer_to2)
	addiu	$2,$2,%lo(xmcs_set_act_timer_to2)
	jalr	$2
	nop

	sw	$2,16($sp)
	j	$L346
	nop

$L242:
	lui	$2,%hi(xmcs_get_csr)
	addiu	$2,$2,%lo(xmcs_get_csr)
	jalr	$2
	nop

	sw	$2,16($sp)
	j	$L346
	nop

$L243:
	lui	$2,%hi(xmcs_get_gem_table_info)
	addiu	$2,$2,%lo(xmcs_get_gem_table_info)
	jalr	$2
	nop

	sw	$2,16($sp)
	j	$L346
	nop

$L244:
	lui	$2,%hi(xmcs_get_tcont_table_info)
	addiu	$2,$2,%lo(xmcs_get_tcont_table_info)
	jalr	$2
	nop

	sw	$2,16($sp)
	j	$L346
	nop

$L284:
	lw	$2,36($sp)
	andi	$2,$2,0xffff
	move	$4,$2
	lui	$2,%hi(xmcs_set_idle_gem)
	addiu	$2,$2,%lo(xmcs_set_idle_gem)
	jalr	$2
	nop

	sw	$2,16($sp)
	j	$L346
	nop

$L238:
	lw	$2,36($sp)
	move	$4,$2
	lui	$2,%hi(xmcs_get_idle_gem)
	addiu	$2,$2,%lo(xmcs_get_idle_gem)
	jalr	$2
	nop

	sw	$2,16($sp)
	j	$L346
	nop

$L247:
	lw	$2,36($sp)
	move	$4,$2
	lui	$2,%hi(xmcs_get_onu_info)
	addiu	$2,$2,%lo(xmcs_get_onu_info)
	jalr	$2
	nop

	sw	$2,16($sp)
	j	$L346
	nop

$L288:
	lw	$2,36($sp)
	andi	$2,$2,0xffff
	move	$4,$2
	lui	$2,%hi(xmcs_set_gpon_block_size)
	addiu	$2,$2,%lo(xmcs_set_gpon_block_size)
	jalr	$2
	nop

	sw	$2,16($sp)
	j	$L346
	nop

$L248:
	lw	$2,36($sp)
	move	$4,$2
	lui	$2,%hi(xmcs_get_gpon_block_size)
	addiu	$2,$2,%lo(xmcs_get_gpon_block_size)
	jalr	$2
	nop

	sw	$2,16($sp)
	j	$L346
	nop

$L292:
	lw	$2,36($sp)
	move	$4,$2
	lui	$2,%hi(xmcs_set_gpon_trtcm_mode)
	addiu	$2,$2,%lo(xmcs_set_gpon_trtcm_mode)
	jalr	$2
	nop

	sw	$2,16($sp)
	j	$L346
	nop

$L311:
	lw	$2,36($sp)
	move	$4,$2
	lui	$2,%hi(xmcs_set_gpon_trtcm_scale)
	addiu	$2,$2,%lo(xmcs_set_gpon_trtcm_scale)
	jalr	$2
	nop

	sw	$2,16($sp)
	j	$L346
	nop

$L245:
	lw	$2,36($sp)
	move	$4,$2
	lui	$2,%hi(xmcs_get_gpon_trtcm_config)
	addiu	$2,$2,%lo(xmcs_get_gpon_trtcm_config)
	jalr	$2
	nop

	sw	$2,16($sp)
	j	$L346
	nop

$L309:
	lw	$2,36($sp)
	move	$4,$2
	lui	$2,%hi(xmcs_set_gpon_trtcm_params)
	addiu	$2,$2,%lo(xmcs_set_gpon_trtcm_params)
	jalr	$2
	nop

	sw	$2,16($sp)
	j	$L346
	nop

$L249:
	lw	$2,36($sp)
	move	$4,$2
	lui	$2,%hi(xmcs_get_gpon_trtcm_params)
	addiu	$2,$2,%lo(xmcs_get_gpon_trtcm_params)
	jalr	$2
	nop

	sw	$2,16($sp)
	j	$L346
	nop

$L312:
	lw	$2,36($sp)
	move	$4,$2
	lui	$2,%hi(xmcs_set_gpon_tod)
	addiu	$2,$2,%lo(xmcs_set_gpon_tod)
	jalr	$2
	nop

	sw	$2,16($sp)
	j	$L346
	nop

$L250:
	lw	$2,36($sp)
	move	$4,$2
	lui	$2,%hi(xmcs_get_gpon_tod)
	addiu	$2,$2,%lo(xmcs_get_gpon_tod)
	jalr	$2
	nop

	sw	$2,16($sp)
	j	$L346
	nop

$L236:
	lw	$2,36($sp)
	move	$4,$2
	lui	$2,%hi(xmcs_get_gpon_new_tod)
	addiu	$2,$2,%lo(xmcs_get_gpon_new_tod)
	jalr	$2
	nop

	sw	$2,16($sp)
	j	$L346
	nop

$L254:
	lw	$2,36($sp)
	move	$4,$2
	lui	$2,%hi(xmcs_get_tod_switch_time)
	addiu	$2,$2,%lo(xmcs_get_tod_switch_time)
	jalr	$2
	nop

	sw	$2,16($sp)
	j	$L346
	nop

$L255:
	lw	$2,36($sp)
	move	$4,$2
	lui	$2,%hi(xmcs_get_gem_counter)
	addiu	$2,$2,%lo(xmcs_get_gem_counter)
	jalr	$2
	nop

	sw	$2,16($sp)
	j	$L346
	nop

$L289:
	lw	$2,36($sp)
	andi	$2,$2,0xffff
	move	$4,$2
	lui	$2,%hi(gponDevClearGemPortCounter)
	addiu	$2,$2,%lo(gponDevClearGemPortCounter)
	jalr	$2
	nop

	sw	$2,16($sp)
	j	$L346
	nop

$L283:
	lw	$2,36($sp)
	seb	$2,$2
	move	$4,$2
	lui	$2,%hi(xmcs_set_eqd_offset_flag)
	addiu	$2,$2,%lo(xmcs_set_eqd_offset_flag)
	jalr	$2
	nop

	sw	$2,16($sp)
	j	$L346
	nop

$L277:
	lw	$2,36($sp)
	andi	$2,$2,0x00ff
	move	$4,$2
	lui	$2,%hi(xmcs_set_eqd_offset_o4)
	addiu	$2,$2,%lo(xmcs_set_eqd_offset_o4)
	jalr	$2
	nop

	sw	$2,16($sp)
	j	$L346
	nop

$L286:
	lw	$2,36($sp)
	andi	$2,$2,0x00ff
	move	$4,$2
	lui	$2,%hi(xmcs_set_eqd_offset_o5)
	addiu	$2,$2,%lo(xmcs_set_eqd_offset_o5)
	jalr	$2
	nop

	sw	$2,16($sp)
	j	$L346
	nop

$L253:
	lw	$2,36($sp)
	move	$4,$2
	lui	$2,%hi(xmcs_get_eqd_offset)
	addiu	$2,$2,%lo(xmcs_get_eqd_offset)
	jalr	$2
	nop

	sw	$2,16($sp)
	j	$L346
	nop

$L256:
	lw	$2,36($sp)
	move	$4,$2
	lui	$2,%hi(gponDevGetSuperframe)
	addiu	$2,$2,%lo(gponDevGetSuperframe)
	jalr	$2
	nop

	sw	$2,16($sp)
	j	$L346
	nop

$L313:
	lw	$4,36($sp)
	lui	$2,%hi(xmcs_set_counter_type)
	addiu	$2,$2,%lo(xmcs_set_counter_type)
	jalr	$2
	nop

	sw	$2,16($sp)
	j	$L346
	nop

$L251:
	lw	$2,36($sp)
	move	$4,$2
	lui	$2,%hi(xmcs_get_counter_type)
	addiu	$2,$2,%lo(xmcs_get_counter_type)
	jalr	$2
	nop

	sw	$2,16($sp)
	j	$L346
	nop

$L232:
	lw	$2,36($sp)
	andi	$2,$2,0xffff
	move	$4,$2
	lui	$2,%hi(xmcs_set_response_time)
	addiu	$2,$2,%lo(xmcs_set_response_time)
	jalr	$2
	nop

	sw	$2,16($sp)
	j	$L346
	nop

$L259:
	lw	$2,36($sp)
	move	$4,$2
	lui	$2,%hi(xmcs_get_response_time)
	addiu	$2,$2,%lo(xmcs_get_response_time)
	jalr	$2
	nop

	sw	$2,16($sp)
	j	$L346
	nop

$L287:
	lw	$2,36($sp)
	andi	$2,$2,0x00ff
	move	$4,$2
	lui	$2,%hi(xmcs_set_internal_delay_fine_tune)
	addiu	$2,$2,%lo(xmcs_set_internal_delay_fine_tune)
	jalr	$2
	nop

	sw	$2,16($sp)
	j	$L346
	nop

$L260:
	lw	$2,36($sp)
	move	$4,$2
	lui	$2,%hi(xmcs_get_internal_delay_fine_tune)
	addiu	$2,$2,%lo(xmcs_get_internal_delay_fine_tune)
	jalr	$2
	nop

	sw	$2,16($sp)
	j	$L346
	nop

$L307:
	lw	$4,36($sp)
	lui	$2,%hi(xmcs_set_burst_mode_overhead)
	addiu	$2,$2,%lo(xmcs_set_burst_mode_overhead)
	jalr	$2
	nop

	sw	$2,16($sp)
	j	$L346
	nop

$L257:
	lw	$2,36($sp)
	move	$4,$2
	lui	$2,%hi(xmcs_get_burst_mode_overhead)
	addiu	$2,$2,%lo(xmcs_get_burst_mode_overhead)
	jalr	$2
	nop

	sw	$2,16($sp)
	j	$L346
	nop

$L316:
	lw	$4,36($sp)
	lui	$2,%hi(xmcs_set_clear_counter)
	addiu	$2,$2,%lo(xmcs_set_clear_counter)
	jalr	$2
	nop

	sw	$2,16($sp)
	j	$L346
	nop

$L317:
	lw	$4,36($sp)
	lui	$2,%hi(xmcs_set_key_switch_time)
	addiu	$2,$2,%lo(xmcs_set_key_switch_time)
	jalr	$2
	nop

	sw	$2,16($sp)
	j	$L346
	nop

$L314:
	lw	$2,36($sp)
	move	$4,$2
	lui	$2,%hi(xmcs_set_encrypt_key)
	addiu	$2,$2,%lo(xmcs_set_encrypt_key)
	jalr	$2
	nop

	sw	$2,16($sp)
	j	$L346
	nop

$L261:
	lw	$2,36($sp)
	move	$4,$2
	lui	$2,%hi(xmcs_get_encrypt_key_info)
	addiu	$2,$2,%lo(xmcs_get_encrypt_key_info)
	jalr	$2
	nop

	sw	$2,16($sp)
	j	$L346
	nop

$L318:
	lw	$4,36($sp)
	lui	$2,%hi(xmcs_set_dying_gasp_mode)
	addiu	$2,$2,%lo(xmcs_set_dying_gasp_mode)
	jalr	$2
	nop

	sw	$2,16($sp)
	j	$L346
	nop

$L262:
	lw	$2,36($sp)
	move	$4,$2
	lui	$2,%hi(xmcs_get_dying_gasp_mode)
	addiu	$2,$2,%lo(xmcs_get_dying_gasp_mode)
	jalr	$2
	nop

	sw	$2,16($sp)
	j	$L346
	nop

$L319:
	lw	$4,36($sp)
	lui	$2,%hi(xmcs_set_dying_gasp_num)
	addiu	$2,$2,%lo(xmcs_set_dying_gasp_num)
	jalr	$2
	nop

	sw	$2,16($sp)
	j	$L346
	nop

$L234:
	lw	$2,36($sp)
	move	$4,$2
	lui	$2,%hi(xmcs_get_dying_gasp_num)
	addiu	$2,$2,%lo(xmcs_get_dying_gasp_num)
	jalr	$2
	nop

	sw	$2,16($sp)
	j	$L346
	nop

$L290:
	lw	$2,36($sp)
	move	$4,$2
	lui	$2,%hi(xmcs_set_up_traffic)
	addiu	$2,$2,%lo(xmcs_set_up_traffic)
	jalr	$2
	nop

	sw	$2,16($sp)
	j	$L346
	nop

$L268:
	lw	$2,36($sp)
	move	$4,$2
	lui	$2,%hi(xmcs_get_up_traffic)
	addiu	$2,$2,%lo(xmcs_get_up_traffic)
	jalr	$2
	nop

	sw	$2,16($sp)
	j	$L346
	nop

$L240:
	lw	$2,36($sp)
	move	$4,$2
	lui	$2,%hi(xmcs_get_int_mask)
	addiu	$2,$2,%lo(xmcs_get_int_mask)
	jalr	$2
	nop

	sw	$2,16($sp)
	j	$L346
	nop

$L325:
	lw	$2,36($sp)
	move	$4,$2
	lui	$2,%hi(xmcs_set_sniffer_mode)
	addiu	$2,$2,%lo(xmcs_set_sniffer_mode)
	jalr	$2
	nop

	sw	$2,16($sp)
	j	$L346
	nop

$L267:
	lw	$2,36($sp)
	move	$4,$2
	lui	$2,%hi(xmcs_get_sniffer_mode)
	addiu	$2,$2,%lo(xmcs_get_sniffer_mode)
	jalr	$2
	nop

	sw	$2,16($sp)
	j	$L346
	nop

$L324:
	lw	$4,36($sp)
	lui	$2,%hi(xmcs_set_ext_bst_len_ploamd_filter)
	addiu	$2,$2,%lo(xmcs_set_ext_bst_len_ploamd_filter)
	jalr	$2
	nop

	sw	$2,16($sp)
	j	$L346
	nop

$L269:
	lw	$2,36($sp)
	move	$4,$2
	lui	$2,%hi(xmcs_get_ext_bst_len_ploamd_filter)
	addiu	$2,$2,%lo(xmcs_get_ext_bst_len_ploamd_filter)
	jalr	$2
	nop

	sw	$2,16($sp)
	j	$L346
	nop

$L326:
	lw	$4,36($sp)
	lui	$2,%hi(xmcs_set_up_overhead_ploamd_filter)
	addiu	$2,$2,%lo(xmcs_set_up_overhead_ploamd_filter)
	jalr	$2
	nop

	sw	$2,16($sp)
	j	$L346
	nop

$L270:
	lw	$2,36($sp)
	move	$4,$2
	lui	$2,%hi(xmcs_get_up_overhead_ploamd_filter)
	addiu	$2,$2,%lo(xmcs_get_up_overhead_ploamd_filter)
	jalr	$2
	nop

	sw	$2,16($sp)
	j	$L346
	nop

$L327:
	lw	$4,36($sp)
	lui	$2,%hi(xmcs_set_tx_4bytes_align)
	addiu	$2,$2,%lo(xmcs_set_tx_4bytes_align)
	jalr	$2
	nop

	sw	$2,16($sp)
	j	$L346
	nop

$L265:
	lw	$2,36($sp)
	move	$4,$2
	lui	$2,%hi(xmcs_get_tx_4bytes_align)
	addiu	$2,$2,%lo(xmcs_get_tx_4bytes_align)
	jalr	$2
	nop

	sw	$2,16($sp)
	j	$L346
	nop

$L322:
	lw	$4,36($sp)
	lui	$2,%hi(xmcs_set_dba_backdoor)
	addiu	$2,$2,%lo(xmcs_set_dba_backdoor)
	jalr	$2
	nop

	sw	$2,16($sp)
	j	$L346
	nop

$L330:
	lw	$4,36($sp)
	lui	$2,%hi(xmcs_set_dba_backdoor_total)
	addiu	$2,$2,%lo(xmcs_set_dba_backdoor_total)
	jalr	$2
	nop

	sw	$2,16($sp)
	j	$L346
	nop

$L331:
	lw	$4,36($sp)
	lui	$2,%hi(xmcs_set_dba_backdoor_green)
	addiu	$2,$2,%lo(xmcs_set_dba_backdoor_green)
	jalr	$2
	nop

	sw	$2,16($sp)
	j	$L346
	nop

$L328:
	lw	$4,36($sp)
	lui	$2,%hi(xmcs_set_dba_backdoor_yellow)
	addiu	$2,$2,%lo(xmcs_set_dba_backdoor_yellow)
	jalr	$2
	nop

	sw	$2,16($sp)
	j	$L346
	nop

$L273:
	lw	$2,36($sp)
	move	$4,$2
	lui	$2,%hi(xmcs_get_dba_backdoor)
	addiu	$2,$2,%lo(xmcs_get_dba_backdoor)
	jalr	$2
	nop

	sw	$2,16($sp)
	j	$L346
	nop

$L332:
	lw	$4,36($sp)
	lui	$2,%hi(xmcs_set_dba_slight_modify)
	addiu	$2,$2,%lo(xmcs_set_dba_slight_modify)
	jalr	$2
	nop

	sw	$2,16($sp)
	j	$L346
	nop

$L298:
	lw	$2,36($sp)
	andi	$2,$2,0xffff
	move	$4,$2
	lui	$2,%hi(xmcs_set_dba_slight_modify_total)
	addiu	$2,$2,%lo(xmcs_set_dba_slight_modify_total)
	jalr	$2
	nop

	sw	$2,16($sp)
	j	$L346
	nop

$L296:
	lw	$2,36($sp)
	andi	$2,$2,0xffff
	move	$4,$2
	lui	$2,%hi(xmcs_set_dba_slight_modify_green)
	addiu	$2,$2,%lo(xmcs_set_dba_slight_modify_green)
	jalr	$2
	nop

	sw	$2,16($sp)
	j	$L346
	nop

$L299:
	lw	$2,36($sp)
	andi	$2,$2,0xffff
	move	$4,$2
	lui	$2,%hi(xmcs_set_dba_slight_modify_yellow)
	addiu	$2,$2,%lo(xmcs_set_dba_slight_modify_yellow)
	jalr	$2
	nop

	sw	$2,16($sp)
	j	$L346
	nop

$L274:
	lw	$2,36($sp)
	move	$4,$2
	lui	$2,%hi(xmcs_get_dba_slight_modify)
	addiu	$2,$2,%lo(xmcs_get_dba_slight_modify)
	jalr	$2
	nop

	sw	$2,16($sp)
	j	$L346
	nop

$L333:
	lw	$4,36($sp)
	lui	$2,%hi(xmcs_set_o3_o4_ploam_ctrl)
	addiu	$2,$2,%lo(xmcs_set_o3_o4_ploam_ctrl)
	jalr	$2
	nop

	sw	$2,16($sp)
	j	$L346
	nop

$L271:
	lw	$2,36($sp)
	move	$4,$2
	lui	$2,%hi(xmcs_get_o3_o4_ploam_ctrl)
	addiu	$2,$2,%lo(xmcs_get_o3_o4_ploam_ctrl)
	jalr	$2
	nop

	sw	$2,16($sp)
	j	$L346
	nop

$L275:
	lw	$2,36($sp)
	move	$4,$2
	lui	$2,%hi(xmcs_get_tx_sync_offset)
	addiu	$2,$2,%lo(xmcs_get_tx_sync_offset)
	jalr	$2
	nop

	sw	$2,16($sp)
	j	$L346
	nop

$L320:
	lui	$2,%hi(xmcs_set_aes_key_switch_by_sw)
	addiu	$2,$2,%lo(xmcs_set_aes_key_switch_by_sw)
	jalr	$2
	nop

	sw	$2,16($sp)
	j	$L346
	nop

$L337:
	lui	$2,%hi(xmcs_set_sw_reset)
	addiu	$2,$2,%lo(xmcs_set_sw_reset)
	jalr	$2
	nop

	sw	$2,16($sp)
	j	$L346
	nop

$L336:
	lui	$2,%hi(xmcs_set_sw_resync)
	addiu	$2,$2,%lo(xmcs_set_sw_resync)
	jalr	$2
	nop

	sw	$2,16($sp)
	j	$L346
	nop

$L338:
	lui	$2,%hi(xmcs_gpon_reset_service)
	addiu	$2,$2,%lo(xmcs_gpon_reset_service)
	jalr	$2
	nop

	sw	$2,16($sp)
	j	$L346
	nop

$L344:
	li	$2,-1078984704			# 0xffffffffbfb00000
	ori	$2,$2,0x64
	lw	$3,0($2)
	li	$2,-65536			# 0xffffffffffff0000
	and	$3,$3,$2
	li	$2,524288			# 0x80000
	bne	$3,$2,$L347
	nop

	lw	$4,36($sp)
	lui	$2,%hi(xmcs_set_1pps_h_w)
	addiu	$2,$2,%lo(xmcs_set_1pps_h_w)
	jalr	$2
	nop

	sw	$2,16($sp)
	j	$L346
	nop

$L347:
	li	$2,-1			# 0xffffffffffffffff
	sw	$2,16($sp)
	j	$L346
	nop

$L279:
	li	$2,-1078984704			# 0xffffffffbfb00000
	ori	$2,$2,0x64
	lw	$3,0($2)
	li	$2,-65536			# 0xffffffffffff0000
	and	$3,$3,$2
	li	$2,524288			# 0x80000
	bne	$3,$2,$L349
	nop

	lw	$2,36($sp)
	move	$4,$2
	lui	$2,%hi(xmcs_get_1pps_h_w)
	addiu	$2,$2,%lo(xmcs_get_1pps_h_w)
	jalr	$2
	nop

	sw	$2,16($sp)
	j	$L346
	nop

$L349:
	li	$2,-1			# 0xffffffffffffffff
	sw	$2,16($sp)
	j	$L346
	nop

$L345:
	li	$2,-1078984704			# 0xffffffffbfb00000
	ori	$2,$2,0x64
	lw	$3,0($2)
	li	$2,-65536			# 0xffffffffffff0000
	and	$3,$3,$2
	li	$2,524288			# 0x80000
	bne	$3,$2,$L351
	nop

	lw	$4,36($sp)
	lui	$2,%hi(xmcs_set_send_ploamu_wait_mode)
	addiu	$2,$2,%lo(xmcs_set_send_ploamu_wait_mode)
	jalr	$2
	nop

	sw	$2,16($sp)
	j	$L346
	nop

$L351:
	li	$2,-1			# 0xffffffffffffffff
	sw	$2,16($sp)
	j	$L346
	nop

$L282:
	li	$2,-1078984704			# 0xffffffffbfb00000
	ori	$2,$2,0x64
	lw	$3,0($2)
	li	$2,-65536			# 0xffffffffffff0000
	and	$3,$3,$2
	li	$2,524288			# 0x80000
	bne	$3,$2,$L353
	nop

	lw	$2,36($sp)
	move	$4,$2
	lui	$2,%hi(xmcs_get_send_ploamu_wait_mode)
	addiu	$2,$2,%lo(xmcs_get_send_ploamu_wait_mode)
	jalr	$2
	nop

	sw	$2,16($sp)
	j	$L346
	nop

$L353:
	li	$2,-1			# 0xffffffffffffffff
	sw	$2,16($sp)
	j	$L346
	nop

$L339:
	lw	$4,36($sp)
	lui	$2,%hi(xmcs_test_send_ploam_sleep_request)
	addiu	$2,$2,%lo(xmcs_test_send_ploam_sleep_request)
	jalr	$2
	nop

	sw	$2,16($sp)
	j	$L346
	nop

$L334:
	lw	$4,36($sp)
	lui	$2,%hi(xmcs_test_send_ploam_rei)
	addiu	$2,$2,%lo(xmcs_test_send_ploam_rei)
	jalr	$2
	nop

	sw	$2,16($sp)
	j	$L346
	nop

$L342:
	lui	$2,%hi(xmcs_test_send_ploam_dying_gasp)
	addiu	$2,$2,%lo(xmcs_test_send_ploam_dying_gasp)
	jalr	$2
	nop

	sw	$2,16($sp)
	j	$L346
	nop

$L343:
	lw	$4,36($sp)
	lui	$2,%hi(xmcs_test_tod)
	addiu	$2,$2,%lo(xmcs_test_tod)
	jalr	$2
	nop

	sw	$2,16($sp)
	j	$L346
	nop

$L276:
	lw	$2,36($sp)
	move	$4,$2
	lui	$2,%hi(xmcs_get_rx_ethernet_frame_counter)
	addiu	$2,$2,%lo(xmcs_get_rx_ethernet_frame_counter)
	jalr	$2
	nop

	sw	$2,16($sp)
	j	$L346
	nop

$L263:
	lw	$2,36($sp)
	move	$4,$2
	lui	$2,%hi(xmcs_get_gpon_traffic_state)
	addiu	$2,$2,%lo(xmcs_get_gpon_traffic_state)
	jalr	$2
	nop

	sw	$2,16($sp)
	j	$L346
	nop

$L340:
	lw	$4,36($sp)
	lui	$2,%hi(xmcs_test_gpon_hotplug)
	addiu	$2,$2,%lo(xmcs_test_gpon_hotplug)
	jalr	$2
	nop

	sw	$2,16($sp)
	j	$L346
	nop

$L281:
	lw	$2,36($sp)
	move	$4,$2
	lui	$2,%hi(xmcs_get_gpon_online_duration)
	addiu	$2,$2,%lo(xmcs_get_gpon_online_duration)
	jalr	$2
	nop

	sw	$2,16($sp)
	j	$L346
	nop

$L231:
	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	lhu	$2,-30816($2)
	andi	$2,$2,0x1
	beq	$2,$0,$L355
	nop

	lui	$2,%hi(jiffies)
	lw	$3,%lo(jiffies)($2)
	lui	$2,%hi($LC5)
	addiu	$4,$2,%lo($LC5)
	move	$5,$3
	lw	$6,32($sp)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	j	$L357
	nop

$L355:
$L357:
	nop
$L346:
	lw	$2,16($sp)
	lw	$31,28($sp)
	addiu	$sp,$sp,32
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	gpon_cmd_proc
	.size	gpon_cmd_proc, .-gpon_cmd_proc
	.rdata
	.align	2
	.type	__func__.30835, @object
	.size	__func__.30835, 19
__func__.30835:
	.ascii	"xmcs_set_act_timer\000"
	.align	2
	.type	__func__.31020, @object
	.size	__func__.31020, 24
__func__.31020:
	.ascii	"xmcs_gpon_reset_service\000"
	.ident	"GCC: (Buildroot 2015.08.1) 4.9.3"
