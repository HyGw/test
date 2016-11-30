	.file	1 "gpon_power_management.c"
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
	.ascii	"sleep count:%.8X\012\000"
#NO_APP
	.section	.text.gpon_power_management_read_proc,"ax",@progbits
	.align	2
	.globl	gpon_power_management_read_proc
	.set	nomips16
	.set	nomicromips
	.ent	gpon_power_management_read_proc
	.type	gpon_power_management_read_proc, @function
gpon_power_management_read_proc:
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
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,17320
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	sw	$2,28($sp)
	lw	$2,16($sp)
	lw	$3,40($sp)
	addu	$2,$3,$2
	lw	$3,28($sp)
	move	$4,$2
	lui	$2,%hi($LC0)
	addiu	$5,$2,%lo($LC0)
	move	$6,$3
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
	beq	$2,$0,$L2
	nop

	sw	$0,16($sp)
	lw	$2,24($sp)
	sw	$2,20($sp)
$L2:
	lw	$3,48($sp)
	lw	$2,52($sp)
	addu	$3,$3,$2
	lw	$2,24($sp)
	slt	$2,$3,$2
	beq	$2,$0,$L3
	nop

	j	$L4
	nop

$L3:
	lw	$2,56($sp)
	li	$3,1			# 0x1
	sw	$3,0($2)
$L4:
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
	bgez	$2,$L5
	nop

	sw	$0,16($sp)
$L5:
	lw	$3,16($sp)
	lw	$2,52($sp)
	slt	$2,$2,$3
	beq	$2,$0,$L6
	nop

	lw	$2,52($sp)
	sw	$2,16($sp)
$L6:
	lw	$2,16($sp)
	lw	$31,36($sp)
	addiu	$sp,$sp,40
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	gpon_power_management_read_proc
	.size	gpon_power_management_read_proc, .-gpon_power_management_read_proc
	.rdata
	.align	2
$LC1:
	.ascii	"%s %x\000"
	.align	2
$LC2:
	.ascii	"sleep_count\000"
	.align	2
$LC3:
	.ascii	"success\000"
	.align	2
$LC4:
	.ascii	"fail\000"
	.align	2
$LC5:
	.ascii	"Modify Sleep count:%.8X %s.\012\000"
	.align	2
$LC6:
	.ascii	"usage\000"
	.align	2
$LC7:
	.ascii	"sleep_count <count (HEX)> \012\000"
	.section	.text.gpon_power_management_write_proc,"ax",@progbits
	.align	2
	.globl	gpon_power_management_write_proc
	.set	nomips16
	.set	nomicromips
	.ent	gpon_power_management_write_proc
	.type	gpon_power_management_write_proc, @function
gpon_power_management_write_proc:
	.frame	$sp,184,$31		# vars= 160, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	addiu	$sp,$sp,-184
	sw	$31,180($sp)
	sw	$4,184($sp)
	sw	$5,188($sp)
	sw	$6,192($sp)
	sw	$7,196($sp)
	lw	$2,192($sp)
	sltu	$2,$2,64
	bne	$2,$0,$L9
	li	$2,-22			# 0xffffffffffffffea
	j	$L19
$L9:
	addiu	$2,$sp,72
	sw	$2,20($sp)
	lw	$2,188($sp)
	sw	$2,24($sp)
	lw	$2,192($sp)
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
	beq	$2,$0,$L11
	lw	$4,20($sp)
	lw	$5,24($sp)
	lw	$6,16($sp)
#APP
 # 116 "/opt/tclinux_phoenix/modules/private/xpon/src/gpon/gpon_power_management.c" 1
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
$L11:
	lw	$2,16($sp)
	beq	$2,$0,$L12
	li	$2,-14			# 0xfffffffffffffff2
	j	$L19
$L12:
	addiu	$2,$sp,72
	addiu	$6,$sp,136
	addiu	$3,$sp,168
	move	$4,$2
	lui	$2,%hi($LC1)
	addiu	$5,$2,%lo($LC1)
	move	$7,$3
	lui	$2,%hi(sscanf)
	addiu	$2,$2,%lo(sscanf)
	jalr	$2
	addiu	$2,$sp,136
	sw	$2,48($sp)
	lui	$2,%hi($LC2)
	addiu	$2,$2,%lo($LC2)
	sw	$2,52($sp)
	lw	$3,48($sp)
	lw	$2,52($sp)
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
	sw	$3,48($sp)
	sw	$2,52($sp)
	sw	$4,56($sp)
	lw	$2,56($sp)
	bne	$2,$0,$L14
	lw	$2,168($sp)
	move	$4,$2
	lui	$2,%hi(modify_sleep_count)
	addiu	$2,$2,%lo(modify_sleep_count)
	jalr	$2
	sw	$2,44($sp)
	lw	$5,168($sp)
	lw	$2,44($sp)
	bne	$2,$0,$L15
	lui	$2,%hi($LC3)
	addiu	$2,$2,%lo($LC3)
	j	$L16
$L15:
	lui	$2,%hi($LC4)
	addiu	$2,$2,%lo($LC4)
$L16:
	lui	$3,%hi($LC5)
	addiu	$4,$3,%lo($LC5)
	move	$6,$2
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	j	$L17
$L14:
	addiu	$2,$sp,136
	sw	$2,60($sp)
	lui	$2,%hi($LC6)
	addiu	$2,$2,%lo($LC6)
	sw	$2,64($sp)
	lw	$3,60($sp)
	lw	$2,64($sp)
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
	sw	$3,60($sp)
	sw	$2,64($sp)
	sw	$4,68($sp)
	lw	$2,68($sp)
	bne	$2,$0,$L17
	lui	$2,%hi($LC7)
	addiu	$4,$2,%lo($LC7)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
$L17:
	lw	$2,192($sp)
$L19:
	lw	$31,180($sp)
	addiu	$sp,$sp,184
	j	$31
	.end	gpon_power_management_write_proc
	.size	gpon_power_management_write_proc, .-gpon_power_management_write_proc
	.section	.text.modify_sleep_count,"ax",@progbits
	.align	2
	.globl	modify_sleep_count
	.set	nomips16
	.set	nomicromips
	.ent	modify_sleep_count
	.type	modify_sleep_count, @function
modify_sleep_count:
	.frame	$sp,32,$31		# vars= 8, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-32
	sw	$31,28($sp)
	sw	$4,32($sp)
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lw	$3,32($sp)
	sw	$3,304($2)
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,17320
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	sw	$2,16($sp)
	lw	$2,32($sp)
	sw	$2,16($sp)
	lw	$3,16($sp)
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,17320
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,17316
	move	$4,$2
	lui	$2,%hi(ioread32)
	addiu	$2,$2,%lo(ioread32)
	jalr	$2
	nop

	sw	$2,20($sp)
	lw	$2,20($sp)
	li	$3,1			# 0x1
	ins	$2,$3,30,1
	sw	$2,20($sp)
	lw	$3,20($sp)
	lui	$2,%hi(g_gpon_mac_reg_BASE)
	lw	$2,%lo(g_gpon_mac_reg_BASE)($2)
	addiu	$2,$2,17316
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	move	$2,$0
	lw	$31,28($sp)
	addiu	$sp,$sp,32
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	modify_sleep_count
	.size	modify_sleep_count, .-modify_sleep_count
	.section	.text.gpon_power_management_init,"ax",@progbits
	.align	2
	.globl	gpon_power_management_init
	.set	nomips16
	.set	nomicromips
	.ent	gpon_power_management_init
	.type	gpon_power_management_init, @function
gpon_power_management_init:
	.frame	$sp,24,$31		# vars= 0, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-24
	sw	$31,20($sp)
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lw	$2,304($2)
	move	$4,$2
	lui	$2,%hi(modify_sleep_count)
	addiu	$2,$2,%lo(modify_sleep_count)
	jalr	$2
	nop

	move	$2,$0
	lw	$31,20($sp)
	addiu	$sp,$sp,24
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	gpon_power_management_init
	.size	gpon_power_management_init, .-gpon_power_management_init
	.section	.text.xpon_power_management_deinit,"ax",@progbits
	.align	2
	.globl	xpon_power_management_deinit
	.set	nomips16
	.set	nomicromips
	.ent	xpon_power_management_deinit
	.type	xpon_power_management_deinit, @function
xpon_power_management_deinit:
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	xpon_power_management_deinit
	.size	xpon_power_management_deinit, .-xpon_power_management_deinit
	.ident	"GCC: (Buildroot 2015.08.1) 4.9.3"
