	.file	1 "xpon_netif.c"
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
	.data
	.type	qos_stop_enable, @object
	.size	qos_stop_enable, 1
qos_stop_enable:
	.byte	1
	.align	2
	.type	qos_stop_num, @object
	.size	qos_stop_num, 4
qos_stop_num:
	.word	1000
	.globl	hw_qos_enable
	.section	.bss,"aw",@nobits
	.align	2
	.type	hw_qos_enable, @object
	.size	hw_qos_enable, 4
hw_qos_enable:
	.space	4
	.globl	trtcmEnable
	.align	2
	.type	trtcmEnable, @object
	.size	trtcmEnable, 4
trtcmEnable:
	.space	4
	.globl	trtcmTsid
	.align	2
	.type	trtcmTsid, @object
	.size	trtcmTsid, 4
trtcmTsid:
	.space	4
	.globl	priPktChkLen
	.data
	.align	2
	.type	priPktChkLen, @object
	.size	priPktChkLen, 4
priPktChkLen:
	.word	100
	.globl	priPktChk
	.align	2
	.type	priPktChk, @object
	.size	priPktChk, 4
priPktChk:
	.word	1
	.globl	qosFlag
	.type	qosFlag, @object
	.size	qosFlag, 1
qosFlag:
	.byte	-1
	.local	qos_wrr_info
	.comm	qos_wrr_info,20,4
	.type	maxPrio, @object
	.size	maxPrio, 1
maxPrio:
	.byte	3
	.local	pwanStats
	.comm	pwanStats,8,4
	.rdata
	.align	2
$LC2:
	.ascii	" \012%.4x: \000"
	.align	2
$LC3:
	.ascii	" \000"
	.align	2
$LC4:
	.ascii	"%.2x \000"
	.align	2
$LC5:
	.ascii	"\012\000"
	.section	.text.__dump_skb,"ax",@progbits
	.align	2
	.globl	__dump_skb
	.set	nomips16
	.set	nomicromips
	.ent	__dump_skb
	.type	__dump_skb, @function
__dump_skb:
	.frame	$sp,40,$31		# vars= 16, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-40
	sw	$31,36($sp)
	sw	$4,40($sp)
	sw	$5,44($sp)
	lw	$2,40($sp)
	lw	$2,256($2)
	sw	$2,16($sp)
	sw	$0,20($sp)
	j	$L2
	nop

$L5:
	lw	$2,20($sp)
	andi	$2,$2,0x00ff
	andi	$2,$2,0xf
	sb	$2,24($sp)
	lbu	$2,24($sp)
	bne	$2,$0,$L3
	nop

	lui	$2,%hi($LC2)
	addiu	$4,$2,%lo($LC2)
	lw	$5,20($sp)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	j	$L4
	nop

$L3:
	lbu	$3,24($sp)
	li	$2,8			# 0x8
	bne	$3,$2,$L4
	nop

	lui	$2,%hi($LC3)
	addiu	$4,$2,%lo($LC3)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L4:
	lw	$2,16($sp)
	addiu	$3,$2,1
	sw	$3,16($sp)
	lbu	$2,0($2)
	move	$3,$2
	lui	$2,%hi($LC4)
	addiu	$4,$2,%lo($LC4)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lw	$2,20($sp)
	addiu	$2,$2,1
	sw	$2,20($sp)
$L2:
	lw	$3,20($sp)
	lw	$2,44($sp)
	sltu	$2,$3,$2
	bne	$2,$0,$L5
	nop

	lui	$2,%hi($LC5)
	addiu	$4,$2,%lo($LC5)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lw	$31,36($sp)
	addiu	$sp,$sp,40
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	__dump_skb
	.size	__dump_skb, .-__dump_skb
	.section	.text.enable_cpu_us_traffic,"ax",@progbits
	.align	2
	.globl	enable_cpu_us_traffic
	.set	nomips16
	.set	nomicromips
	.ent	enable_cpu_us_traffic
	.type	enable_cpu_us_traffic, @function
enable_cpu_us_traffic:
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$3,$3,$2
	lbu	$2,-30412($3)
	li	$4,1			# 0x1
	ins	$2,$4,5,1
	sb	$2,-30412($3)
	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$3,$3,$2
	lbu	$2,-30412($3)
	li	$4,1			# 0x1
	ins	$2,$4,4,1
	sb	$2,-30412($3)
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	enable_cpu_us_traffic
	.size	enable_cpu_us_traffic, .-enable_cpu_us_traffic
	.section	.text.disable_cpu_us_traffic,"ax",@progbits
	.align	2
	.globl	disable_cpu_us_traffic
	.set	nomips16
	.set	nomicromips
	.ent	disable_cpu_us_traffic
	.type	disable_cpu_us_traffic, @function
disable_cpu_us_traffic:
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$3,$3,$2
	lbu	$2,-30412($3)
	ins	$2,$0,5,1
	sb	$2,-30412($3)
	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$3,$3,$2
	lbu	$2,-30412($3)
	ins	$2,$0,4,1
	sb	$2,-30412($3)
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	disable_cpu_us_traffic
	.size	disable_cpu_us_traffic, .-disable_cpu_us_traffic
	.rdata
	.align	2
$LC6:
	.ascii	"\015\012[%s][%d] skb or len error.\015\012\000"
	.align	2
$LC7:
	.ascii	"\015\012[%s][%d]alloc skb failed.\015\012\000"
	.section	.text.replace_oam_pkt,"ax",@progbits
	.align	2
	.globl	replace_oam_pkt
	.set	nomips16
	.set	nomicromips
	.ent	replace_oam_pkt
	.type	replace_oam_pkt, @function
replace_oam_pkt:
	.frame	$sp,32,$31		# vars= 8, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-32
	sw	$31,28($sp)
	sw	$4,32($sp)
	sw	$5,36($sp)
	sw	$0,16($sp)
	lw	$2,32($sp)
	beq	$2,$0,$L9
	nop

	lw	$2,36($sp)
	bgez	$2,$L10
	nop

$L9:
	lui	$2,%hi($LC6)
	addiu	$4,$2,%lo($LC6)
	lui	$2,%hi(__func__.36650)
	addiu	$5,$2,%lo(__func__.36650)
	li	$6,185			# 0xb9
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lw	$2,32($sp)
	j	$L11
	nop

$L10:
	lw	$2,36($sp)
	move	$4,$2
	lui	$2,%hi(dev_alloc_skb)
	addiu	$2,$2,%lo(dev_alloc_skb)
	jalr	$2
	nop

	sw	$2,16($sp)
	lw	$2,16($sp)
	bne	$2,$0,$L12
	nop

	lui	$2,%hi($LC7)
	addiu	$4,$2,%lo($LC7)
	lui	$2,%hi(__func__.36650)
	addiu	$5,$2,%lo(__func__.36650)
	li	$6,191			# 0xbf
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lw	$2,32($sp)
	j	$L11
	nop

$L12:
	lw	$2,32($sp)
	lw	$2,80($2)
	lw	$4,16($sp)
	move	$5,$2
	lui	$2,%hi(skb_put)
	addiu	$2,$2,%lo(skb_put)
	jalr	$2
	nop

	lw	$2,16($sp)
	lw	$4,256($2)
	lw	$2,32($sp)
	lw	$3,256($2)
	lw	$2,36($sp)
	move	$5,$3
	move	$6,$2
	lui	$2,%hi(memcpy)
	addiu	$2,$2,%lo(memcpy)
	jalr	$2
	nop

	lw	$4,32($sp)
	lui	$2,%hi(consume_skb)
	addiu	$2,$2,%lo(consume_skb)
	jalr	$2
	nop

	lw	$2,16($sp)
$L11:
	lw	$31,28($sp)
	addiu	$sp,$sp,32
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	replace_oam_pkt
	.size	replace_oam_pkt, .-replace_oam_pkt
	.rdata
	.align	2
$LC8:
	.ascii	"%d %d %d %d %d\012\000"
	.section	.text.pon_qoswrr_read_proc,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	pon_qoswrr_read_proc
	.type	pon_qoswrr_read_proc, @function
pon_qoswrr_read_proc:
	.frame	$sp,32,$31		# vars= 0, regs= 1/0, args= 24, gp= 0
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
	lui	$2,%hi(qos_wrr_info)
	lw	$5,%lo(qos_wrr_info)($2)
	lui	$2,%hi(qos_wrr_info)
	addiu	$2,$2,%lo(qos_wrr_info)
	lw	$6,4($2)
	lui	$2,%hi(qos_wrr_info)
	addiu	$2,$2,%lo(qos_wrr_info)
	lw	$7,8($2)
	lui	$2,%hi(qos_wrr_info)
	addiu	$2,$2,%lo(qos_wrr_info)
	lw	$3,12($2)
	lui	$2,%hi(qos_wrr_info)
	addiu	$2,$2,%lo(qos_wrr_info)
	lw	$2,16($2)
	sw	$3,16($sp)
	sw	$2,20($sp)
	lui	$2,%hi($LC8)
	addiu	$4,$2,%lo($LC8)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	move	$2,$0
	lw	$31,28($sp)
	addiu	$sp,$sp,32
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	pon_qoswrr_read_proc
	.size	pon_qoswrr_read_proc, .-pon_qoswrr_read_proc
	.rdata
	.align	2
$LC9:
	.ascii	"%d %d %d %d %d\000"
	.align	2
$LC10:
	.ascii	"\012FAILED(%s): qdma strict priority setting for path%d\012"
	.ascii	"\000"
	.align	2
$LC11:
	.ascii	"\012SP setting for txq0~3 is done\012\012\000"
	.align	2
$LC12:
	.ascii	"\012FAILED(%s): qdma wrr setting for path%d\012\000"
	.align	2
$LC13:
	.ascii	"\012WRR setting for txq0~3 is done:\012(Type:%d) Q3:%d, "
	.ascii	"Q2:%d, Q1:%d, Q0:%d\012\012\000"
	.section	.text.pon_qoswrr_write_proc,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	pon_qoswrr_write_proc
	.type	pon_qoswrr_write_proc, @function
pon_qoswrr_write_proc:
	.frame	$sp,272,$31		# vars= 232, regs= 1/0, args= 32, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	addiu	$sp,$sp,-272
	sw	$31,268($sp)
	sw	$4,272($sp)
	sw	$5,276($sp)
	sw	$6,280($sp)
	sw	$7,284($sp)
	sw	$0,40($sp)
	lw	$2,280($sp)
	sltu	$2,$2,33
	bne	$2,$0,$L16
	li	$2,32			# 0x20
	sw	$2,32($sp)
	j	$L17
$L16:
	lw	$2,280($sp)
	sw	$2,32($sp)
$L17:
	addiu	$2,$sp,120
	sw	$2,56($sp)
	lw	$2,276($sp)
	sw	$2,60($sp)
	lw	$2,32($sp)
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
	beq	$2,$0,$L18
	lw	$4,56($sp)
	lw	$5,60($sp)
	lw	$6,52($sp)
#APP
 # 245 "/opt/tclinux_phoenix/modules/private/xpon/src/pwan/xpon_netif.c" 1
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
$L18:
	lw	$2,52($sp)
	beq	$2,$0,$L19
	li	$2,-14			# 0xfffffffffffffff2
	j	$L44
$L19:
	lw	$2,32($sp)
	addiu	$3,$sp,32
	addu	$2,$3,$2
	sb	$0,88($2)
	addiu	$3,$sp,120
	lui	$2,%hi(qos_wrr_info+8)
	addiu	$2,$2,%lo(qos_wrr_info+8)
	sw	$2,16($sp)
	lui	$2,%hi(qos_wrr_info+12)
	addiu	$2,$2,%lo(qos_wrr_info+12)
	sw	$2,20($sp)
	lui	$2,%hi(qos_wrr_info+16)
	addiu	$2,$2,%lo(qos_wrr_info+16)
	sw	$2,24($sp)
	move	$4,$3
	lui	$2,%hi($LC9)
	addiu	$5,$2,%lo($LC9)
	lui	$2,%hi(qos_wrr_info)
	addiu	$6,$2,%lo(qos_wrr_info)
	lui	$2,%hi(qos_wrr_info+4)
	addiu	$7,$2,%lo(qos_wrr_info+4)
	lui	$2,%hi(sscanf)
	addiu	$2,$2,%lo(sscanf)
	jalr	$2
	move	$3,$2
	li	$2,5			# 0x5
	beq	$3,$2,$L21
	lw	$2,280($sp)
	j	$L44
$L21:
	sw	$0,36($sp)
	j	$L22
$L24:
	lw	$2,36($sp)
	addiu	$3,$2,1
	lui	$2,%hi(qos_wrr_info)
	sll	$3,$3,2
	addiu	$2,$2,%lo(qos_wrr_info)
	addu	$2,$3,$2
	lw	$3,0($2)
	lw	$2,40($sp)
	slt	$2,$2,$3
	beq	$2,$0,$L23
	lw	$2,36($sp)
	addiu	$3,$2,1
	lui	$2,%hi(qos_wrr_info)
	sll	$3,$3,2
	addiu	$2,$2,%lo(qos_wrr_info)
	addu	$2,$3,$2
	lw	$2,0($2)
	sw	$2,40($sp)
	lw	$2,36($sp)
	andi	$2,$2,0x00ff
	li	$3,3			# 0x3
	subu	$2,$3,$2
	andi	$3,$2,0x00ff
	lui	$2,%hi(maxPrio)
	sb	$3,%lo(maxPrio)($2)
$L23:
	lw	$2,36($sp)
	addiu	$2,$2,1
	sw	$2,36($sp)
$L22:
	lw	$2,36($sp)
	slt	$2,$2,4
	bne	$2,$0,$L24
	lui	$2,%hi(qos_wrr_info)
	lw	$2,%lo(qos_wrr_info)($2)
	bne	$2,$0,$L25
	sw	$0,44($sp)
	j	$L26
$L31:
	addiu	$2,$sp,152
	move	$4,$2
	move	$5,$0
	li	$6,16			# 0x10
	lui	$2,%hi(memset)
	addiu	$2,$2,%lo(memset)
	jalr	$2
	lw	$2,44($sp)
	andi	$2,$2,0x00ff
	sb	$2,152($sp)
	li	$2,1			# 0x1
	sw	$2,156($sp)
	li	$2,16			# 0x10
	sw	$2,84($sp)
	addiu	$2,$sp,152
	sw	$2,88($sp)
	sw	$0,92($sp)
	li	$2,13			# 0xd
	sw	$2,224($sp)
	lw	$2,88($sp)
	sw	$2,240($sp)
	addiu	$2,$sp,224
	lw	$4,84($sp)
	move	$5,$0
	move	$6,$2
	lui	$2,%hi(__ECNT_HOOK)
	addiu	$2,$2,%lo(__ECNT_HOOK)
	jalr	$2
	sw	$2,92($sp)
	lw	$3,92($sp)
	li	$2,-1			# 0xffffffffffffffff
	beq	$3,$2,$L27
	lw	$2,228($sp)
	j	$L29
$L27:
	li	$2,-1			# 0xffffffffffffffff
$L29:
	sw	$2,80($sp)
	lw	$2,80($sp)
	beq	$2,$0,$L30
	lui	$2,%hi($LC10)
	addiu	$4,$2,%lo($LC10)
	lui	$2,%hi(__func__.36689)
	addiu	$5,$2,%lo(__func__.36689)
	lw	$6,44($sp)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	li	$2,-1			# 0xffffffffffffffff
	j	$L44
$L30:
	lw	$2,44($sp)
	addiu	$2,$2,1
	sw	$2,44($sp)
$L26:
	lw	$2,44($sp)
	slt	$2,$2,8
	bne	$2,$0,$L31
	lui	$2,%hi($LC11)
	addiu	$4,$2,%lo($LC11)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	j	$L32
$L25:
	sw	$0,44($sp)
	j	$L33
$L40:
	addiu	$2,$sp,152
	move	$4,$2
	move	$5,$0
	li	$6,16			# 0x10
	lui	$2,%hi(memset)
	addiu	$2,$2,%lo(memset)
	jalr	$2
	lw	$2,44($sp)
	andi	$2,$2,0x00ff
	sb	$2,152($sp)
	li	$2,5			# 0x5
	sw	$2,156($sp)
	sw	$0,48($sp)
	j	$L34
$L35:
	li	$3,4			# 0x4
	lw	$2,48($sp)
	subu	$3,$3,$2
	lui	$2,%hi(qos_wrr_info)
	sll	$3,$3,2
	addiu	$2,$2,%lo(qos_wrr_info)
	addu	$2,$3,$2
	lw	$2,0($2)
	andi	$3,$2,0x00ff
	lw	$2,48($sp)
	addiu	$4,$sp,32
	addu	$2,$4,$2
	sb	$3,128($2)
	lw	$2,48($sp)
	addiu	$2,$2,1
	sw	$2,48($sp)
$L34:
	lw	$2,48($sp)
	slt	$2,$2,4
	bne	$2,$0,$L35
	li	$2,16			# 0x10
	sw	$2,96($sp)
	addiu	$2,$sp,152
	sw	$2,100($sp)
	sw	$0,104($sp)
	li	$2,13			# 0xd
	sw	$2,196($sp)
	lw	$2,100($sp)
	sw	$2,212($sp)
	addiu	$2,$sp,196
	lw	$4,96($sp)
	move	$5,$0
	move	$6,$2
	lui	$2,%hi(__ECNT_HOOK)
	addiu	$2,$2,%lo(__ECNT_HOOK)
	jalr	$2
	sw	$2,104($sp)
	lw	$3,104($sp)
	li	$2,-1			# 0xffffffffffffffff
	beq	$3,$2,$L36
	lw	$2,200($sp)
	j	$L38
$L36:
	li	$2,-1			# 0xffffffffffffffff
$L38:
	sw	$2,80($sp)
	lw	$2,80($sp)
	beq	$2,$0,$L39
	lui	$2,%hi($LC12)
	addiu	$4,$2,%lo($LC12)
	lui	$2,%hi(__func__.36689)
	addiu	$5,$2,%lo(__func__.36689)
	lw	$6,44($sp)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	li	$2,-1			# 0xffffffffffffffff
	j	$L44
$L39:
	lw	$2,44($sp)
	addiu	$2,$2,1
	sw	$2,44($sp)
$L33:
	lw	$2,44($sp)
	slt	$2,$2,8
	bne	$2,$0,$L40
	addiu	$2,$sp,152
	move	$4,$2
	move	$5,$0
	li	$6,16			# 0x10
	lui	$2,%hi(memset)
	addiu	$2,$2,%lo(memset)
	jalr	$2
	sb	$0,152($sp)
	li	$2,16			# 0x10
	sw	$2,108($sp)
	addiu	$2,$sp,152
	sw	$2,112($sp)
	sw	$0,116($sp)
	li	$2,14			# 0xe
	sw	$2,168($sp)
	lw	$2,112($sp)
	sw	$2,184($sp)
	addiu	$2,$sp,168
	lw	$4,108($sp)
	move	$5,$0
	move	$6,$2
	lui	$2,%hi(__ECNT_HOOK)
	addiu	$2,$2,%lo(__ECNT_HOOK)
	jalr	$2
	sw	$2,116($sp)
	lw	$3,156($sp)
	lbu	$2,163($sp)
	move	$6,$2
	lbu	$2,162($sp)
	move	$7,$2
	lbu	$2,161($sp)
	move	$4,$2
	lbu	$2,160($sp)
	sw	$4,16($sp)
	sw	$2,20($sp)
	lui	$2,%hi($LC13)
	addiu	$4,$2,%lo($LC13)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
$L32:
	lw	$2,32($sp)
$L44:
	lw	$31,268($sp)
	addiu	$sp,$sp,272
	j	$31
	.end	pon_qoswrr_write_proc
	.size	pon_qoswrr_write_proc, .-pon_qoswrr_write_proc
	.rdata
	.align	2
$LC14:
	.ascii	"\012ERROR: cp is NULL at isPriorityPkt\012\000"
	.align	2
$LC15:
	.ascii	"\012ERROR: priority is NULL at isPriorityPkt\012\000"
	.section	.text.isPriorityPkt,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	isPriorityPkt
	.type	isPriorityPkt, @function
isPriorityPkt:
	.frame	$sp,32,$31		# vars= 8, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-32
	sw	$31,28($sp)
	sw	$4,32($sp)
	sw	$5,36($sp)
	lw	$2,32($sp)
	bne	$2,$0,$L46
	nop

	lui	$2,%hi($LC14)
	addiu	$4,$2,%lo($LC14)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	move	$2,$0
	j	$L47
	nop

$L46:
	lw	$2,36($sp)
	bne	$2,$0,$L48
	nop

	lui	$2,%hi($LC15)
	addiu	$4,$2,%lo($LC15)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	move	$2,$0
	j	$L47
	nop

$L48:
	lw	$2,32($sp)
	addiu	$2,$2,12
	sw	$2,32($sp)
	lw	$2,32($sp)
	lhu	$2,0($2)
	sh	$2,16($sp)
	lw	$2,32($sp)
	addiu	$2,$2,2
	sw	$2,32($sp)
	lhu	$3,16($sp)
	li	$2,33024			# 0x8100
	bne	$3,$2,$L49
	nop

	lw	$2,32($sp)
	addiu	$2,$2,2
	sw	$2,32($sp)
	lw	$2,32($sp)
	lhu	$2,0($2)
	sh	$2,16($sp)
	lw	$2,32($sp)
	addiu	$2,$2,2
	sw	$2,32($sp)
$L49:
	lhu	$3,16($sp)
	li	$2,34916			# 0x8864
	bne	$3,$2,$L50
	nop

	lw	$2,32($sp)
	addiu	$2,$2,6
	sw	$2,32($sp)
	lw	$2,32($sp)
	lhu	$2,0($2)
	sh	$2,18($sp)
	lhu	$3,18($sp)
	li	$2,49185			# 0xc021
	bne	$3,$2,$L51
	nop

	lw	$2,36($sp)
	li	$3,3			# 0x3
	sw	$3,0($2)
	li	$2,1			# 0x1
	j	$L47
	nop

$L51:
	lhu	$3,18($sp)
	li	$2,33			# 0x21
	beq	$3,$2,$L52
	nop

	move	$2,$0
	j	$L47
	nop

$L52:
	lw	$2,32($sp)
	addiu	$2,$2,2
	sw	$2,32($sp)
	j	$L53
	nop

$L50:
	lhu	$3,16($sp)
	li	$2,34915			# 0x8863
	bne	$3,$2,$L54
	nop

	lw	$2,36($sp)
	li	$3,3			# 0x3
	sw	$3,0($2)
	li	$2,1			# 0x1
	j	$L47
	nop

$L54:
	lhu	$3,16($sp)
	li	$2,2048			# 0x800
	beq	$3,$2,$L53
	nop

	move	$2,$0
	j	$L47
	nop

$L53:
	lw	$2,32($sp)
	lbu	$2,0($2)
	sb	$2,20($sp)
	lbu	$2,20($sp)
	andi	$3,$2,0xf0
	li	$2,64			# 0x40
	beq	$3,$2,$L55
	nop

	move	$2,$0
	j	$L47
	nop

$L55:
	lw	$2,32($sp)
	lbu	$2,9($2)
	sb	$2,21($sp)
	lbu	$3,21($sp)
	li	$2,1			# 0x1
	beq	$3,$2,$L56
	nop

	lbu	$3,21($sp)
	li	$2,2			# 0x2
	bne	$3,$2,$L57
	nop

$L56:
	lw	$2,36($sp)
	li	$3,3			# 0x3
	sw	$3,0($2)
	li	$2,1			# 0x1
	j	$L47
	nop

$L57:
	lbu	$3,21($sp)
	li	$2,6			# 0x6
	beq	$3,$2,$L58
	nop

	move	$2,$0
	j	$L47
	nop

$L58:
	lbu	$2,20($sp)
	andi	$2,$2,0xf
	sll	$2,$2,2
	move	$3,$2
	lw	$2,32($sp)
	addu	$2,$2,$3
	sw	$2,32($sp)
	lw	$2,32($sp)
	lbu	$2,13($2)
	sb	$2,22($sp)
	lbu	$2,22($sp)
	andi	$2,$2,0x10
	bne	$2,$0,$L59
	nop

	lbu	$3,22($sp)
	li	$2,2			# 0x2
	bne	$3,$2,$L60
	nop

$L59:
	lw	$2,36($sp)
	li	$3,3			# 0x3
	sw	$3,0($2)
	li	$2,1			# 0x1
	j	$L47
	nop

$L60:
	move	$2,$0
$L47:
	lw	$31,28($sp)
	addiu	$sp,$sp,32
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	isPriorityPkt
	.size	isPriorityPkt, .-isPriorityPkt
	.rdata
	.align	2
$LC16:
	.ascii	"\012pon_trafficShapingSet_write_proc EN7521 is not suppo"
	.ascii	"rt\012\000"
	.section	.text.pon_trafficShapingSet_write_proc,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	pon_trafficShapingSet_write_proc
	.type	pon_trafficShapingSet_write_proc, @function
pon_trafficShapingSet_write_proc:
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
	lui	$2,%hi($LC16)
	addiu	$4,$2,%lo($LC16)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lw	$2,32($sp)
	lw	$31,20($sp)
	addiu	$sp,$sp,24
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	pon_trafficShapingSet_write_proc
	.size	pon_trafficShapingSet_write_proc, .-pon_trafficShapingSet_write_proc
	.rdata
	.align	2
$LC17:
	.ascii	"\012 pon_trafficShapingSet_read_proc EN7521 is not suppo"
	.ascii	"rt\012\000"
	.section	.text.pon_trafficShapingSet_read_proc,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	pon_trafficShapingSet_read_proc
	.type	pon_trafficShapingSet_read_proc, @function
pon_trafficShapingSet_read_proc:
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
	sw	$0,16($sp)
	lui	$2,%hi($LC17)
	addiu	$4,$2,%lo($LC17)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lw	$2,16($sp)
	lw	$31,28($sp)
	addiu	$sp,$sp,32
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	pon_trafficShapingSet_read_proc
	.size	pon_trafficShapingSet_read_proc, .-pon_trafficShapingSet_read_proc
	.rdata
	.align	2
$LC18:
	.ascii	"HWWRR\000"
	.align	2
$LC19:
	.ascii	"\012qos discipline is HW WRR.\012\012\000"
	.align	2
$LC20:
	.ascii	"HWPQ\000"
	.align	2
$LC21:
	.ascii	"\012qos discipline is HW PQ.\012\012\000"
	.align	2
$LC22:
	.ascii	"\012qos discipline is disabled.\012\012\000"
	.section	.text.pon_tcqos_write_proc,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	pon_tcqos_write_proc
	.type	pon_tcqos_write_proc, @function
pon_tcqos_write_proc:
	.frame	$sp,96,$31		# vars= 72, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	addiu	$sp,$sp,-96
	sw	$31,92($sp)
	sw	$4,96($sp)
	sw	$5,100($sp)
	sw	$6,104($sp)
	sw	$7,108($sp)
	lw	$2,104($sp)
	sltu	$2,$2,11
	bne	$2,$0,$L66
	li	$2,10			# 0xa
	sw	$2,16($sp)
	j	$L67
$L66:
	lw	$2,104($sp)
	sw	$2,16($sp)
$L67:
	addiu	$2,$sp,72
	move	$4,$2
	move	$5,$0
	li	$6,10			# 0xa
	lui	$2,%hi(memset)
	addiu	$2,$2,%lo(memset)
	jalr	$2
	addiu	$2,$sp,72
	sw	$2,24($sp)
	lw	$2,100($sp)
	sw	$2,28($sp)
	lw	$2,16($sp)
	addiu	$2,$2,-1
	sw	$2,20($sp)
	lw	$2,28($sp)
	sw	$2,32($sp)
	lw	$2,20($sp)
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
	beq	$2,$0,$L68
	lw	$4,24($sp)
	lw	$5,28($sp)
	lw	$6,20($sp)
#APP
 # 692 "/opt/tclinux_phoenix/modules/private/xpon/src/pwan/xpon_netif.c" 1
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
	sw	$2,20($sp)
$L68:
	lw	$2,20($sp)
	beq	$2,$0,$L69
	li	$2,-14			# 0xfffffffffffffff2
	j	$L76
$L69:
	lw	$2,16($sp)
	addiu	$3,$sp,16
	addu	$2,$3,$2
	sb	$0,56($2)
	addiu	$2,$sp,72
	sw	$2,48($sp)
	lui	$2,%hi($LC18)
	addiu	$2,$2,%lo($LC18)
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
	bne	$2,$0,$L72
	lui	$2,%hi(qosFlag)
	li	$3,3			# 0x3
	sb	$3,%lo(qosFlag)($2)
	lui	$2,%hi($LC19)
	addiu	$4,$2,%lo($LC19)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	j	$L73
$L72:
	addiu	$2,$sp,72
	sw	$2,60($sp)
	lui	$2,%hi($LC20)
	addiu	$2,$2,%lo($LC20)
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
	bne	$2,$0,$L75
	lui	$2,%hi(qosFlag)
	li	$3,4			# 0x4
	sb	$3,%lo(qosFlag)($2)
	lui	$2,%hi($LC21)
	addiu	$4,$2,%lo($LC21)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	j	$L73
$L75:
	lui	$2,%hi(qosFlag)
	li	$3,-1			# 0xffffffffffffffff
	sb	$3,%lo(qosFlag)($2)
	lui	$2,%hi($LC22)
	addiu	$4,$2,%lo($LC22)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
$L73:
	lw	$2,16($sp)
$L76:
	lw	$31,92($sp)
	addiu	$sp,$sp,96
	j	$31
	.end	pon_tcqos_write_proc
	.size	pon_tcqos_write_proc, .-pon_tcqos_write_proc
	.section	.text.pon_hw_qos_enable_write_proc,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	pon_hw_qos_enable_write_proc
	.type	pon_hw_qos_enable_write_proc, @function
pon_hw_qos_enable_write_proc:
	.frame	$sp,128,$31		# vars= 104, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	addiu	$sp,$sp,-128
	sw	$31,124($sp)
	sw	$4,128($sp)
	sw	$5,132($sp)
	sw	$6,136($sp)
	sw	$7,140($sp)
	sw	$0,20($sp)
	sw	$0,24($sp)
	lw	$2,136($sp)
	sltu	$2,$2,64
	bne	$2,$0,$L78
	li	$2,-14			# 0xfffffffffffffff2
	sw	$2,24($sp)
	lw	$2,24($sp)
	j	$L84
$L78:
	addiu	$2,$sp,52
	move	$4,$2
	move	$5,$0
	li	$6,64			# 0x40
	lui	$2,%hi(memset)
	addiu	$2,$2,%lo(memset)
	jalr	$2
	addiu	$2,$sp,52
	sw	$2,28($sp)
	lw	$2,132($sp)
	sw	$2,32($sp)
	lw	$2,136($sp)
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
	beq	$2,$0,$L80
	lw	$4,28($sp)
	lw	$5,32($sp)
	lw	$6,16($sp)
#APP
 # 853 "/opt/tclinux_phoenix/modules/private/xpon/src/pwan/xpon_netif.c" 1
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
$L80:
	lw	$2,16($sp)
	beq	$2,$0,$L81
	li	$2,-14			# 0xfffffffffffffff2
	sw	$2,24($sp)
	lw	$2,24($sp)
	j	$L84
$L81:
	addiu	$3,$sp,52
	addiu	$2,$sp,116
	move	$4,$3
	move	$5,$2
	li	$6,10			# 0xa
	lui	$2,%hi(simple_strtol)
	addiu	$2,$2,%lo(simple_strtol)
	jalr	$2
	move	$3,$2
	lui	$2,%hi(hw_qos_enable)
	sw	$3,%lo(hw_qos_enable)($2)
	lui	$2,%hi(hw_qos_enable)
	lw	$2,%lo(hw_qos_enable)($2)
	beq	$2,$0,$L82
	li	$4,1			# 0x1
	lui	$2,%hi(xpon_set_qdma_qos)
	addiu	$2,$2,%lo(xpon_set_qdma_qos)
	jalr	$2
	j	$L83
$L82:
	move	$4,$0
	lui	$2,%hi(xpon_set_qdma_qos)
	addiu	$2,$2,%lo(xpon_set_qdma_qos)
	jalr	$2
$L83:
	lw	$2,136($sp)
$L84:
	lw	$31,124($sp)
	addiu	$sp,$sp,128
	j	$31
	.end	pon_hw_qos_enable_write_proc
	.size	pon_hw_qos_enable_write_proc, .-pon_hw_qos_enable_write_proc
	.rdata
	.align	2
$LC23:
	.ascii	"%d\012\000"
	.section	.text.pon_hw_qos_enable_read_proc,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	pon_hw_qos_enable_read_proc
	.type	pon_hw_qos_enable_read_proc, @function
pon_hw_qos_enable_read_proc:
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
	lui	$2,%hi(hw_qos_enable)
	lw	$3,%lo(hw_qos_enable)($2)
	lui	$2,%hi($LC23)
	addiu	$4,$2,%lo($LC23)
	move	$5,$3
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
	.end	pon_hw_qos_enable_read_proc
	.size	pon_hw_qos_enable_read_proc, .-pon_hw_qos_enable_read_proc
	.section	.text.pwan_net_timer,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	pwan_net_timer
	.type	pwan_net_timer, @function
pwan_net_timer:
	.frame	$sp,200,$31		# vars= 176, regs= 2/0, args= 16, gp= 0
	.mask	0x80010000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-200
	sw	$31,196($sp)
	sw	$16,192($sp)
	sw	$4,200($sp)
	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	lw	$3,-30412($2)
	li	$2,1073741824			# 0x40000000
	and	$2,$3,$2
	beq	$2,$0,$L88
	nop

	j	$L89
	nop

$L88:
	lui	$2,%hi(gpPonSysData)
	lw	$2,%lo(gpPonSysData)($2)
	lw	$3,0($2)
	li	$2,1			# 0x1
	beq	$3,$2,$L90
	nop

	lui	$2,%hi(gpPonSysData)
	lw	$2,%lo(gpPonSysData)($2)
	lw	$3,0($2)
	li	$2,2			# 0x2
	bne	$3,$2,$L89
	nop

$L90:
	lui	$2,%hi(internet_hwnat_timer_switch)
	lw	$2,%lo(internet_hwnat_timer_switch)($2)
	beq	$2,$0,$L89
	nop

	lui	$2,%hi(pwanStats)
	lw	$2,%lo(pwanStats)($2)
	sw	$2,16($sp)
	lui	$2,%hi(pwanStats)
	addiu	$2,$2,%lo(pwanStats)
	lw	$2,4($2)
	sw	$2,20($sp)
	li	$2,1			# 0x1
	sw	$2,32($sp)
	lui	$2,%hi(pwanStats)
	addiu	$2,$2,%lo(pwanStats)
	sw	$2,36($sp)
	sw	$0,40($sp)
	li	$2,41			# 0x29
	sw	$2,56($sp)
	lw	$2,32($sp)
	sw	$2,76($sp)
	addiu	$2,$sp,56
	li	$4,18			# 0x12
	move	$5,$0
	move	$6,$2
	lui	$2,%hi(__ECNT_HOOK)
	addiu	$2,$2,%lo(__ECNT_HOOK)
	jalr	$2
	nop

	sw	$2,40($sp)
	lw	$3,96($sp)
	lw	$2,36($sp)
	sw	$3,0($2)
	sw	$0,44($sp)
	lui	$2,%hi(pwanStats+4)
	addiu	$2,$2,%lo(pwanStats+4)
	sw	$2,48($sp)
	sw	$0,52($sp)
	li	$2,41			# 0x29
	sw	$2,56($sp)
	lw	$2,44($sp)
	sw	$2,76($sp)
	addiu	$2,$sp,56
	li	$4,18			# 0x12
	move	$5,$0
	move	$6,$2
	lui	$2,%hi(__ECNT_HOOK)
	addiu	$2,$2,%lo(__ECNT_HOOK)
	jalr	$2
	nop

	sw	$2,52($sp)
	lw	$3,96($sp)
	lw	$2,48($sp)
	sw	$3,0($2)
	lui	$2,%hi(pwanStats)
	lw	$3,%lo(pwanStats)($2)
	lw	$2,16($sp)
	subu	$2,$3,$2
	sw	$2,24($sp)
	lui	$2,%hi(pwanStats)
	addiu	$2,$2,%lo(pwanStats)
	lw	$3,4($2)
	lw	$2,20($sp)
	subu	$2,$3,$2
	sw	$2,28($sp)
	lui	$2,%hi(internet_hwnat_pktnum)
	lw	$2,%lo(internet_hwnat_pktnum)($2)
	move	$3,$2
	lw	$2,24($sp)
	sltu	$2,$3,$2
	bne	$2,$0,$L97
	nop

	lui	$2,%hi(internet_hwnat_pktnum)
	lw	$2,%lo(internet_hwnat_pktnum)($2)
	move	$3,$2
	lw	$2,28($sp)
	sltu	$2,$3,$2
	beq	$2,$0,$L89
	nop

$L97:
	lui	$2,%hi(internet_led_on)
	lw	$2,%lo(internet_led_on)($2)
	beq	$2,$0,$L98
	nop

	li	$4,34			# 0x22
	lui	$2,%hi(ledTurnOn)
	addiu	$2,$2,%lo(ledTurnOn)
	jalr	$2
	nop

	j	$L89
	nop

$L98:
	lui	$2,%hi(internet_trying_led_on)
	lw	$2,%lo(internet_trying_led_on)($2)
	bne	$2,$0,$L89
	nop

	li	$4,33			# 0x21
	lui	$2,%hi(ledTurnOff)
	addiu	$2,$2,%lo(ledTurnOff)
	jalr	$2
	nop

	li	$4,38			# 0x26
	lui	$2,%hi(ledTurnOff)
	addiu	$2,$2,%lo(ledTurnOff)
	jalr	$2
	nop

$L89:
	lui	$2,%hi(gpWanPriv)
	lw	$16,%lo(gpWanPriv)($2)
	li	$4,250			# 0xfa
	lui	$2,%hi(msecs_to_jiffies)
	addiu	$2,$2,%lo(msecs_to_jiffies)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(jiffies)
	lw	$2,%lo(jiffies)($2)
	addu	$3,$3,$2
	li	$2,65536			# 0x10000
	addu	$2,$16,$2
	sw	$3,-31868($2)
	lui	$2,%hi(gpWanPriv)
	lw	$3,%lo(gpWanPriv)($2)
	li	$2,33660			# 0x837c
	addu	$16,$3,$2
	li	$4,250			# 0xfa
	lui	$2,%hi(msecs_to_jiffies)
	addiu	$2,$2,%lo(msecs_to_jiffies)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(jiffies)
	lw	$2,%lo(jiffies)($2)
	addu	$2,$3,$2
	move	$4,$16
	move	$5,$2
	lui	$2,%hi(mod_timer)
	addiu	$2,$2,%lo(mod_timer)
	jalr	$2
	nop

	lw	$31,196($sp)
	lw	$16,192($sp)
	addiu	$sp,$sp,200
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	pwan_net_timer
	.size	pwan_net_timer, .-pwan_net_timer
	.rdata
	.align	2
$LC24:
	.ascii	"[%lu0ms]%s entered\012\000"
	.align	2
$LC25:
	.ascii	"pon\000"
	.section	.text.pwan_net_open,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	pwan_net_open
	.type	pwan_net_open, @function
pwan_net_open:
	.frame	$sp,120,$31		# vars= 96, regs= 2/0, args= 16, gp= 0
	.mask	0x80010000,-4
	.fmask	0x00000000,0
	addiu	$sp,$sp,-120
	sw	$31,116($sp)
	sw	$16,112($sp)
	sw	$4,120($sp)
	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	lhu	$2,-30816($2)
	andi	$2,$2,0x40
	beq	$2,$0,$L100
	lui	$2,%hi(jiffies)
	lw	$3,%lo(jiffies)($2)
	lui	$2,%hi($LC24)
	addiu	$4,$2,%lo($LC24)
	move	$5,$3
	lui	$2,%hi(__func__.36798)
	addiu	$6,$2,%lo(__func__.36798)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
$L100:
	lw	$2,120($sp)
	sw	$2,20($sp)
	lw	$2,20($sp)
	addiu	$2,$2,928
	sw	$2,16($sp)
	lw	$2,16($sp)
	addiu	$2,$2,4
	sw	$2,24($sp)
	lw	$2,24($sp)
	addiu	$2,$2,8
	sw	$0,28($sp)
	sw	$2,32($sp)
	lw	$2,28($sp)
	bgez	$2,$L102
	addiu	$2,$2,31
$L102:
	sra	$2,$2,5
	sll	$2,$2,2
	lw	$3,32($sp)
	addu	$2,$3,$2
	lw	$3,0($2)
	lw	$2,28($sp)
	andi	$2,$2,0x1f
	srl	$2,$3,$2
	andi	$2,$2,0x1
	sltu	$2,$2,1
	andi	$2,$2,0x00ff
	sw	$2,36($sp)
	lw	$2,36($sp)
#APP
 # 29 "/opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/bug.h" 1
	tne $0, $2, 512
 # 0 "" 2
 # 470 "include/linux/netdevice.h" 1
			

 # 0 "" 2
#NO_APP
	lw	$2,24($sp)
	addiu	$2,$2,8
	sw	$0,40($sp)
	sw	$2,44($sp)
	lw	$2,40($sp)
	srl	$2,$2,5
	sll	$2,$2,2
	lw	$3,44($sp)
	addu	$2,$3,$2
	sw	$2,48($sp)
	lw	$2,40($sp)
	andi	$2,$2,0xffff
	andi	$2,$2,0x1f
	sh	$2,52($sp)
	lhu	$2,52($sp)
	li	$3,1			# 0x1
	sll	$2,$3,$2
	nor	$3,$0,$2
	lw	$4,48($sp)
	lw	$2,48($sp)
#APP
 # 153 "/opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/bitops.h" 1
		.set	mips3					
1:	ll	$5, 0($4)			# clear_bit	
	and	$5, $3					
	sc	$5, 0($4)					
	beqz	$5, 2f					
	.subsection 2					
2:	b	1b					
	.previous					
	.set	mips0					

 # 0 "" 2
#NO_APP
	move	$2,$5
	sw	$2,56($sp)
	lw	$2,120($sp)
	sw	$2,60($sp)
	lw	$2,60($sp)
	sw	$2,64($sp)
	sw	$0,68($sp)
	lw	$2,64($sp)
	lw	$3,544($2)
	lw	$2,68($sp)
	sll	$2,$2,5
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	sw	$2,72($sp)
	lw	$2,72($sp)
	addiu	$2,$2,8
	sw	$0,76($sp)
	sw	$2,80($sp)
	lw	$2,76($sp)
	srl	$2,$2,5
	sll	$2,$2,2
	lw	$3,80($sp)
	addu	$2,$3,$2
	sw	$2,84($sp)
	lw	$2,76($sp)
	andi	$2,$2,0xffff
	andi	$2,$2,0x1f
	sh	$2,88($sp)
	lhu	$2,88($sp)
	li	$3,1			# 0x1
	sll	$2,$3,$2
	nor	$3,$0,$2
	lw	$4,84($sp)
	lw	$2,84($sp)
#APP
 # 153 "/opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/bitops.h" 1
		.set	mips3					
1:	ll	$5, 0($4)			# clear_bit	
	and	$5, $3					
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
	lw	$2,120($sp)
	sw	$2,96($sp)
	lui	$2,%hi($LC25)
	addiu	$2,$2,%lo($LC25)
	sw	$2,100($sp)
	lw	$3,96($sp)
	lw	$2,100($sp)
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
	sw	$3,96($sp)
	sw	$2,100($sp)
	sw	$4,104($sp)
	lw	$2,104($sp)
	bne	$2,$0,$L107
	lui	$2,%hi(gpWanPriv)
	lw	$3,%lo(gpWanPriv)($2)
	li	$2,33660			# 0x837c
	addu	$2,$3,$2
	move	$4,$2
	move	$5,$0
	move	$6,$0
	lui	$2,%hi(init_timer_key)
	addiu	$2,$2,%lo(init_timer_key)
	jalr	$2
	lui	$2,%hi(gpWanPriv)
	lw	$16,%lo(gpWanPriv)($2)
	li	$4,250			# 0xfa
	lui	$2,%hi(msecs_to_jiffies)
	addiu	$2,$2,%lo(msecs_to_jiffies)
	jalr	$2
	move	$3,$2
	lui	$2,%hi(jiffies)
	lw	$2,%lo(jiffies)($2)
	addu	$3,$3,$2
	li	$2,65536			# 0x10000
	addu	$2,$16,$2
	sw	$3,-31868($2)
	lui	$2,%hi(gpWanPriv)
	lw	$3,%lo(gpWanPriv)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	lui	$3,%hi(pwan_net_timer)
	addiu	$3,$3,%lo(pwan_net_timer)
	sw	$3,-31860($2)
	lui	$2,%hi(gpWanPriv)
	lw	$3,%lo(gpWanPriv)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	sw	$0,-31856($2)
	lui	$2,%hi(gpWanPriv)
	lw	$3,%lo(gpWanPriv)($2)
	li	$2,33660			# 0x837c
	addu	$2,$3,$2
	move	$4,$2
	lui	$2,%hi(add_timer)
	addiu	$2,$2,%lo(add_timer)
	jalr	$2
	lui	$2,%hi(pwanStats)
	addiu	$4,$2,%lo(pwanStats)
	move	$5,$0
	li	$6,8			# 0x8
	lui	$2,%hi(memset)
	addiu	$2,$2,%lo(memset)
	jalr	$2
$L107:
	move	$2,$0
	lw	$31,116($sp)
	lw	$16,112($sp)
	addiu	$sp,$sp,120
	j	$31
	.end	pwan_net_open
	.size	pwan_net_open, .-pwan_net_open
	.section	.text.pwan_net_stop,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	pwan_net_stop
	.type	pwan_net_stop, @function
pwan_net_stop:
	.frame	$sp,152,$31		# vars= 128, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	addiu	$sp,$sp,-152
	sw	$31,148($sp)
	sw	$4,152($sp)
	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	lhu	$2,-30816($2)
	andi	$2,$2,0x40
	beq	$2,$0,$L110
	lui	$2,%hi(jiffies)
	lw	$3,%lo(jiffies)($2)
	lui	$2,%hi($LC24)
	addiu	$4,$2,%lo($LC24)
	move	$5,$3
	lui	$2,%hi(__func__.36803)
	addiu	$6,$2,%lo(__func__.36803)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
$L110:
	lw	$2,152($sp)
	sw	$2,20($sp)
	lui	$2,%hi($LC25)
	addiu	$2,$2,%lo($LC25)
	sw	$2,24($sp)
	lw	$3,20($sp)
	lw	$2,24($sp)
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
	sw	$3,20($sp)
	sw	$2,24($sp)
	sw	$4,28($sp)
	lw	$2,28($sp)
	bne	$2,$0,$L112
	lw	$2,152($sp)
	lw	$2,224($2)
	andi	$2,$2,0x1
	beq	$2,$0,$L112
	lui	$2,%hi(gpWanPriv)
	lw	$3,%lo(gpWanPriv)($2)
	li	$2,33660			# 0x837c
	addu	$2,$3,$2
	move	$4,$2
	lui	$2,%hi(del_timer_sync)
	addiu	$2,$2,%lo(del_timer_sync)
	jalr	$2
$L112:
	lw	$2,152($sp)
	sw	$2,32($sp)
	lw	$2,32($sp)
	sw	$2,36($sp)
	sw	$0,40($sp)
	lw	$2,36($sp)
	lw	$3,544($2)
	lw	$2,40($sp)
	sll	$2,$2,5
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	sw	$2,44($sp)
	lw	$2,44($sp)
	addiu	$2,$2,8
	sw	$0,48($sp)
	sw	$2,52($sp)
	lw	$2,48($sp)
	srl	$2,$2,5
	sll	$2,$2,2
	lw	$3,52($sp)
	addu	$2,$3,$2
	sw	$2,56($sp)
	lw	$2,48($sp)
	andi	$2,$2,0xffff
	andi	$2,$2,0x1f
	sh	$2,60($sp)
	lhu	$2,60($sp)
	li	$3,1			# 0x1
	sll	$3,$3,$2
	lw	$4,56($sp)
	lw	$2,56($sp)
#APP
 # 88 "/opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/bitops.h" 1
		.set	mips3					
1:	ll	$5, 0($4)			# set_bit	
	or	$5, $3					
	sc	$5, 0($4)					
	beqz	$5, 2f					
	.subsection 2					
2:	b	1b					
	.previous					
	.set	mips0					

 # 0 "" 2
#NO_APP
	move	$2,$5
	sw	$2,64($sp)
	lw	$2,152($sp)
	sw	$2,68($sp)
	lw	$2,68($sp)
	addiu	$2,$2,928
	sw	$2,16($sp)
	lw	$2,16($sp)
	addiu	$2,$2,4
	sw	$2,72($sp)
	lw	$2,72($sp)
	addiu	$2,$2,8
	li	$3,1			# 0x1
	sw	$3,76($sp)
	sw	$2,80($sp)
	lw	$2,76($sp)
	srl	$2,$2,5
	sll	$2,$2,2
	lw	$3,80($sp)
	addu	$2,$3,$2
	sw	$2,84($sp)
	lw	$2,76($sp)
	andi	$2,$2,0xffff
	andi	$2,$2,0x1f
	sh	$2,88($sp)
	lhu	$2,88($sp)
	li	$3,1			# 0x1
	sll	$3,$3,$2
	lw	$4,84($sp)
	lw	$2,84($sp)
#APP
 # 88 "/opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/bitops.h" 1
		.set	mips3					
1:	ll	$6, 0($4)			# set_bit	
	or	$6, $3					
	sc	$6, 0($4)					
	beqz	$6, 2f					
	.subsection 2					
2:	b	1b					
	.previous					
	.set	mips0					

 # 0 "" 2
#NO_APP
	move	$2,$6
	sw	$2,92($sp)
	j	$L115
$L117:
	li	$4,1			# 0x1
	lui	$2,%hi(msleep)
	addiu	$2,$2,%lo(msleep)
	jalr	$2
$L115:
	lw	$2,72($sp)
	addiu	$2,$2,8
	sw	$0,96($sp)
	sw	$2,100($sp)
	lw	$2,96($sp)
	andi	$2,$2,0xffff
	andi	$2,$2,0x1f
	sh	$2,104($sp)
#APP
 # 261 "/opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/bitops.h" 1
			

 # 0 "" 2
#NO_APP
	lw	$2,96($sp)
	srl	$2,$2,5
	sll	$2,$2,2
	lw	$3,100($sp)
	addu	$2,$3,$2
	sw	$2,108($sp)
	lhu	$2,104($sp)
	li	$3,1			# 0x1
	sll	$2,$3,$2
	lw	$5,108($sp)
	lw	$3,108($sp)
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
	sw	$4,112($sp)
	sw	$3,116($sp)
#APP
 # 312 "/opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/bitops.h" 1
			

 # 0 "" 2
#NO_APP
	lw	$2,116($sp)
	sltu	$2,$0,$2
	andi	$2,$2,0x00ff
	bne	$2,$0,$L117
	lw	$2,72($sp)
	addiu	$2,$2,8
	li	$3,1			# 0x1
	sw	$3,120($sp)
	sw	$2,124($sp)
	lw	$2,120($sp)
	srl	$2,$2,5
	sll	$2,$2,2
	lw	$3,124($sp)
	addu	$2,$3,$2
	sw	$2,128($sp)
	lw	$2,120($sp)
	andi	$2,$2,0xffff
	andi	$2,$2,0x1f
	sh	$2,132($sp)
	lhu	$2,132($sp)
	li	$3,1			# 0x1
	sll	$2,$3,$2
	nor	$3,$0,$2
	lw	$4,128($sp)
	lw	$2,128($sp)
#APP
 # 153 "/opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/bitops.h" 1
		.set	mips3					
1:	ll	$5, 0($4)			# clear_bit	
	and	$5, $3					
	sc	$5, 0($4)					
	beqz	$5, 2f					
	.subsection 2					
2:	b	1b					
	.previous					
	.set	mips0					

 # 0 "" 2
#NO_APP
	move	$2,$5
	sw	$2,136($sp)
	move	$2,$0
	lw	$31,148($sp)
	addiu	$sp,$sp,152
	j	$31
	.end	pwan_net_stop
	.size	pwan_net_stop, .-pwan_net_stop
	.section	.text.get_pon_link_type,"ax",@progbits
	.align	2
	.globl	get_pon_link_type
	.set	nomips16
	.set	nomicromips
	.ent	get_pon_link_type
	.type	get_pon_link_type, @function
get_pon_link_type:
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	lui	$2,%hi(gpPonSysData)
	lw	$2,%lo(gpPonSysData)($2)
	lw	$2,0($2)
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	get_pon_link_type
	.size	get_pon_link_type, .-get_pon_link_type
	.section	__ksymtab_strings,"a",@progbits
	.type	__kstrtab_get_pon_link_type, @object
	.size	__kstrtab_get_pon_link_type, 18
__kstrtab_get_pon_link_type:
	.ascii	"get_pon_link_type\000"
	.section	__ksymtab,"a",@progbits
	.align	2
	.type	__ksymtab_get_pon_link_type, @object
	.size	__ksymtab_get_pon_link_type, 8
__ksymtab_get_pon_link_type:
	.word	get_pon_link_type
	.word	__kstrtab_get_pon_link_type
	.section	.text.isHighestPriorityPacket,"ax",@progbits
	.align	2
	.globl	isHighestPriorityPacket
	.set	nomips16
	.set	nomicromips
	.ent	isHighestPriorityPacket
	.type	isHighestPriorityPacket, @function
isHighestPriorityPacket:
	.frame	$sp,16,$31		# vars= 16, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-16
	sw	$4,16($sp)
	sw	$0,0($sp)
	sh	$0,4($sp)
	sh	$0,8($sp)
	sh	$0,6($sp)
	sw	$0,12($sp)
	lw	$2,16($sp)
	lw	$2,256($2)
	sw	$2,0($sp)
	lw	$2,0($sp)
	addiu	$2,$2,12
	sw	$2,0($sp)
	lw	$2,0($sp)
	lhu	$2,0($2)
	sh	$2,4($sp)
	lw	$2,0($sp)
	addiu	$2,$2,2
	sw	$2,0($sp)
	j	$L122
	nop

$L123:
	lw	$2,0($sp)
	addiu	$2,$2,2
	sw	$2,0($sp)
	lw	$2,0($sp)
	lhu	$2,0($2)
	sh	$2,4($sp)
	lw	$2,0($sp)
	addiu	$2,$2,2
	sw	$2,0($sp)
$L122:
	lhu	$3,4($sp)
	li	$2,33024			# 0x8100
	beq	$3,$2,$L123
	nop

	lhu	$3,4($sp)
	li	$2,34984			# 0x88a8
	beq	$3,$2,$L123
	nop

	lhu	$3,4($sp)
	li	$2,37120			# 0x9100
	beq	$3,$2,$L123
	nop

	lhu	$3,4($sp)
	li	$2,2054			# 0x806
	beq	$3,$2,$L124
	nop

	lhu	$3,4($sp)
	li	$2,34915			# 0x8863
	bne	$3,$2,$L125
	nop

$L124:
	li	$2,1			# 0x1
	sh	$2,6($sp)
	j	$L126
	nop

$L125:
	lhu	$3,4($sp)
	li	$2,34916			# 0x8864
	bne	$3,$2,$L127
	nop

	lw	$2,0($sp)
	addiu	$2,$2,6
	sw	$2,0($sp)
	lw	$2,0($sp)
	lhu	$2,0($2)
	sh	$2,8($sp)
	lhu	$3,8($sp)
	li	$2,49185			# 0xc021
	beq	$3,$2,$L128
	nop

	lhu	$3,8($sp)
	li	$2,49187			# 0xc023
	beq	$3,$2,$L128
	nop

	lhu	$3,8($sp)
	li	$2,49189			# 0xc025
	beq	$3,$2,$L128
	nop

	lhu	$3,8($sp)
	li	$2,32801			# 0x8021
	beq	$3,$2,$L128
	nop

	lhu	$3,8($sp)
	li	$2,49699			# 0xc223
	bne	$3,$2,$L126
	nop

$L128:
	li	$2,1			# 0x1
	sh	$2,6($sp)
	j	$L126
	nop

$L127:
	lhu	$3,4($sp)
	li	$2,2048			# 0x800
	bne	$3,$2,$L126
	nop

	lw	$2,0($sp)
	sw	$2,12($sp)
	lw	$2,12($sp)
	lbu	$3,9($2)
	li	$2,2			# 0x2
	bne	$3,$2,$L126
	nop

	li	$2,1			# 0x1
	sh	$2,6($sp)
$L126:
	lhu	$2,6($sp)
	addiu	$sp,$sp,16
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	isHighestPriorityPacket
	.size	isHighestPriorityPacket, .-isHighestPriorityPacket
	.rdata
	.align	2
$LC26:
	.ascii	"\012FAIL(%s): wan2lan allocation\012\000"
	.section	.text.copy_pwan_skb,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	copy_pwan_skb
	.type	copy_pwan_skb, @function
copy_pwan_skb:
	.frame	$sp,48,$31		# vars= 24, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-48
	sw	$31,44($sp)
	sw	$4,48($sp)
	move	$2,$5
	sb	$2,52($sp)
	sw	$0,16($sp)
	li	$2,6			# 0x6
	sw	$2,20($sp)
	lbu	$3,52($sp)
	li	$2,1			# 0x1
	bne	$3,$2,$L132
	nop

	lw	$2,20($sp)
	addiu	$2,$2,14
	sw	$2,20($sp)
$L132:
	lw	$2,48($sp)
	sw	$2,24($sp)
	lw	$2,24($sp)
	lw	$2,256($2)
	move	$3,$2
	lw	$2,24($sp)
	lw	$2,252($2)
	subu	$2,$3,$2
	move	$3,$2
	lw	$2,20($sp)
	sltu	$2,$3,$2
	beq	$2,$0,$L134
	nop

	lw	$2,48($sp)
	sw	$2,28($sp)
	lw	$2,28($sp)
	sw	$2,32($sp)
	lw	$2,32($sp)
	lw	$2,84($2)
	bne	$2,$0,$L136
	nop

	lw	$2,28($sp)
	lw	$2,248($2)
	move	$3,$2
	lw	$2,28($sp)
	lw	$2,244($2)
	subu	$2,$3,$2
	j	$L138
	nop

$L136:
	move	$2,$0
$L138:
	lw	$4,48($sp)
	lw	$5,20($sp)
	move	$6,$2
	li	$7,32			# 0x20
	lui	$2,%hi(skb_copy_expand)
	addiu	$2,$2,%lo(skb_copy_expand)
	jalr	$2
	nop

	sw	$2,16($sp)
	j	$L139
	nop

$L134:
	lw	$4,48($sp)
	li	$5,32			# 0x20
	lui	$2,%hi(skb_copy)
	addiu	$2,$2,%lo(skb_copy)
	jalr	$2
	nop

	sw	$2,16($sp)
$L139:
	lui	$2,%hi(ra_sw_nat_hook_xfer)
	lw	$2,%lo(ra_sw_nat_hook_xfer)($2)
	beq	$2,$0,$L140
	nop

	lui	$2,%hi(ra_sw_nat_hook_xfer)
	lw	$2,%lo(ra_sw_nat_hook_xfer)($2)
	lw	$4,48($sp)
	lw	$5,16($sp)
	jalr	$2
	nop

$L140:
	lw	$2,16($sp)
	sltu	$2,$2,1
	andi	$2,$2,0x00ff
	beq	$2,$0,$L141
	nop

	lui	$2,%hi($LC26)
	addiu	$4,$2,%lo($LC26)
	lui	$2,%hi(__func__.36841)
	addiu	$5,$2,%lo(__func__.36841)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	move	$2,$0
	j	$L142
	nop

$L141:
	lbu	$3,52($sp)
	li	$2,1			# 0x1
	bne	$3,$2,$L143
	nop

	lw	$4,16($sp)
	li	$5,14			# 0xe
	lui	$2,%hi(skb_push)
	addiu	$2,$2,%lo(skb_push)
	jalr	$2
	nop

	lw	$2,16($sp)
	lw	$2,256($2)
	move	$4,$2
	lui	$2,%hi(omciHdr.36840)
	addiu	$5,$2,%lo(omciHdr.36840)
	li	$6,14			# 0xe
	lui	$2,%hi(memcpy)
	addiu	$2,$2,%lo(memcpy)
	jalr	$2
	nop

$L143:
	lw	$2,16($sp)
	lw	$3,148($2)
	li	$2,67108864			# 0x4000000
	or	$3,$3,$2
	lw	$2,16($sp)
	sw	$3,148($2)
	lw	$2,16($sp)
$L142:
	lw	$31,44($sp)
	addiu	$sp,$sp,48
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	copy_pwan_skb
	.size	copy_pwan_skb, .-copy_pwan_skb
	.rdata
	.align	2
$LC27:
	.ascii	"[%lu0ms]Start to transmit a packets.\012\000"
	.align	2
$LC28:
	.ascii	"ERR %s skb_unshare failed! \012\000"
	.align	2
$LC29:
	.ascii	"%s:%d !!!!XPON SPLIT DEBUG!!!====>%s:%d\012\000"
	.align	2
$LC30:
	.ascii	"(%s)802.1p remark failure\015\012\000"
	.align	2
$LC31:
	.ascii	"[%lu0ms]The xPON link status is off.\012\000"
	.align	2
$LC32:
	.ascii	"[%lu0ms]TX SKB LEN:%d, TX MSG: %.8x, %.8x\000"
	.align	2
$LC33:
	.ascii	"[%lu0ms]%s(%d)sysLinkStatus = %d, netIdx=%d.\012\000"
	.align	2
$LC34:
	.ascii	"[%lu0ms]Transmit packet to QDMA failed.\012\000"
	.section	.text.pwan_net_start_xmit,"ax",@progbits
	.align	2
	.globl	pwan_net_start_xmit
	.set	nomips16
	.set	nomicromips
	.ent	pwan_net_start_xmit
	.type	pwan_net_start_xmit, @function
pwan_net_start_xmit:
	.frame	$sp,192,$31		# vars= 160, regs= 1/0, args= 24, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-192
	sw	$31,188($sp)
	sw	$4,192($sp)
	sw	$5,196($sp)
	sw	$0,24($sp)
	sw	$0,136($sp)
	sw	$0,140($sp)
	sh	$0,36($sp)
	sh	$0,28($sp)
	sh	$0,30($sp)
	sb	$0,38($sp)
	sw	$0,40($sp)
	sw	$0,44($sp)
	sw	$0,48($sp)
	sw	$0,144($sp)
	sw	$0,148($sp)
	sw	$0,32($sp)
	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	lhu	$2,-30816($2)
	andi	$2,$2,0x40
	beq	$2,$0,$L145
	nop

	lui	$2,%hi(jiffies)
	lw	$3,%lo(jiffies)($2)
	lui	$2,%hi($LC27)
	addiu	$4,$2,%lo($LC27)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L145:
	lw	$2,196($sp)
	sw	$2,56($sp)
	lw	$2,56($sp)
	addiu	$2,$2,928
	sw	$2,52($sp)
	lw	$2,192($sp)
	lw	$3,196($sp)
	sw	$3,20($2)
	sb	$0,38($sp)
	lbu	$2,38($sp)
	bne	$2,$0,$L147
	nop

	lw	$2,192($sp)
	sw	$2,60($sp)
	li	$2,32			# 0x20
	sw	$2,64($sp)
	lw	$2,60($sp)
	sw	$2,68($sp)
	lw	$2,68($sp)
	lw	$3,100($2)
	li	$2,1073741824			# 0x40000000
	and	$2,$3,$2
	beq	$2,$0,$L148
	nop

	lw	$2,68($sp)
	sw	$2,72($sp)
	lw	$2,72($sp)
	lw	$2,248($2)
	addiu	$2,$2,40
	lw	$2,0($2)
	andi	$3,$2,0xffff
	li	$2,1			# 0x1
	beq	$3,$2,$L148
	nop

	li	$2,1			# 0x1
	j	$L151
	nop

$L148:
	move	$2,$0
$L151:
	beq	$2,$0,$L152
	nop

	lw	$4,60($sp)
	lw	$5,64($sp)
	lui	$2,%hi(skb_copy)
	addiu	$2,$2,%lo(skb_copy)
	jalr	$2
	nop

	sw	$2,76($sp)
	lw	$4,60($sp)
	lui	$2,%hi(kfree_skb)
	addiu	$2,$2,%lo(kfree_skb)
	jalr	$2
	nop

	lw	$2,76($sp)
	sw	$2,60($sp)
$L152:
	lw	$2,60($sp)
	sw	$2,48($sp)
	lw	$2,48($sp)
	bne	$2,$0,$L154
	nop

	lui	$2,%hi($LC28)
	addiu	$4,$2,%lo($LC28)
	lui	$2,%hi(__func__.36861)
	addiu	$5,$2,%lo(__func__.36861)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi(drop_print_flag)
	lw	$2,%lo(drop_print_flag)($2)
	beq	$2,$0,$L155
	nop

	li	$2,1341			# 0x53d
	sw	$2,16($sp)
	lui	$2,%hi($LC29)
	addiu	$4,$2,%lo($LC29)
	lui	$2,%hi(__func__.36861)
	addiu	$5,$2,%lo(__func__.36861)
	li	$6,1341			# 0x53d
	lui	$2,%hi(__func__.36861)
	addiu	$7,$2,%lo(__func__.36861)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	j	$L221
	nop

$L155:
	j	$L221
	nop

$L154:
	lw	$2,48($sp)
	sw	$2,192($sp)
$L147:
	lbu	$2,38($sp)
	bne	$2,$0,$L157
	nop

	move	$2,$0
	beq	$2,$0,$L157
	nop

	lw	$2,52($sp)
	lbu	$2,0($2)
	lw	$4,192($sp)
	move	$5,$2
	lui	$2,%hi(copy_pwan_skb)
	addiu	$2,$2,%lo(copy_pwan_skb)
	jalr	$2
	nop

	sw	$2,32($sp)
	lw	$2,32($sp)
	beq	$2,$0,$L157
	nop

	li	$4,-2147483648			# 0xffffffff80000000
	lw	$5,32($sp)
	lui	$2,%hi(macSend)
	addiu	$2,$2,%lo(macSend)
	jalr	$2
	nop

	sw	$0,32($sp)
$L157:
	lw	$2,52($sp)
	lbu	$2,1($2)
	beq	$2,$0,$L159
	nop

	lw	$4,192($sp)
	lw	$5,196($sp)
	lui	$2,%hi(eth_type_trans)
	addiu	$2,$2,%lo(eth_type_trans)
	jalr	$2
	nop

	move	$3,$2
	lw	$2,192($sp)
	sh	$3,102($2)
	lw	$3,192($sp)
	lhu	$2,100($3)
	li	$4,1			# 0x1
	ins	$2,$4,12,2
	sh	$2,100($3)
	lui	$2,%hi(jiffies)
	lw	$3,%lo(jiffies)($2)
	lw	$2,196($sp)
	sw	$3,360($2)
	lw	$4,192($sp)
	lui	$2,%hi(netif_rx)
	addiu	$2,$2,%lo(netif_rx)
	jalr	$2
	nop

	j	$L160
	nop

$L159:
	lui	$2,%hi(gpPonSysData)
	lw	$2,%lo(gpPonSysData)($2)
	lw	$2,0($2)
	bne	$2,$0,$L161
	nop

	lui	$2,%hi(drop_print_flag)
	lw	$2,%lo(drop_print_flag)($2)
	beq	$2,$0,$L162
	nop

	li	$2,1367			# 0x557
	sw	$2,16($sp)
	lui	$2,%hi($LC29)
	addiu	$4,$2,%lo($LC29)
	lui	$2,%hi(__func__.36861)
	addiu	$5,$2,%lo(__func__.36861)
	li	$6,1367			# 0x557
	lui	$2,%hi(__func__.36861)
	addiu	$7,$2,%lo(__func__.36861)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	j	$L221
	nop

$L162:
	j	$L221
	nop

$L161:
	lw	$2,52($sp)
	lbu	$3,0($2)
	li	$2,2			# 0x2
	bne	$3,$2,$L163
	nop

	lui	$2,%hi(sw_pktqosEnqueue)
	lw	$2,%lo(sw_pktqosEnqueue)($2)
	beq	$2,$0,$L163
	nop

	lui	$2,%hi(sw_pktqosEnqueue)
	lw	$2,%lo(sw_pktqosEnqueue)($2)
	lw	$4,192($sp)
	jalr	$2
	nop

	move	$3,$2
	li	$2,1			# 0x1
	bne	$3,$2,$L164
	nop

	move	$2,$0
	j	$L224
	nop

$L164:
	lw	$2,192($sp)
	lbu	$2,122($2)
	andi	$2,$2,0x2
	bne	$2,$0,$L166
	nop

	lw	$2,192($sp)
	lbu	$3,122($2)
	lw	$2,192($sp)
	lw	$2,148($2)
	andi	$4,$2,0x00ff
	li	$2,-16			# 0xfffffffffffffff0
	and	$2,$4,$2
	andi	$2,$2,0x00ff
	or	$2,$3,$2
	andi	$3,$2,0x00ff
	lw	$2,192($sp)
	sb	$3,122($2)
$L166:
	lw	$2,192($sp)
	lbu	$2,122($2)
	andi	$2,$2,0x2
	beq	$2,$0,$L163
	nop

	lw	$2,192($sp)
	lw	$3,148($2)
	lw	$2,192($sp)
	lbu	$2,122($2)
	andi	$2,$2,0xf0
	or	$3,$3,$2
	lw	$2,192($sp)
	sw	$3,148($2)
$L163:
	lw	$2,192($sp)
	sw	$2,80($sp)
	sw	$0,84($sp)
	sw	$0,88($sp)
	sb	$0,92($sp)
	sb	$0,93($sp)
	sh	$0,94($sp)
	sw	$0,96($sp)
	lw	$2,80($sp)
	lw	$2,148($2)
	andi	$2,$2,0xf00
	beq	$2,$0,$L167
	nop

	lw	$2,80($sp)
	lw	$2,256($2)
	addiu	$2,$2,12
	sw	$2,88($sp)
	lw	$2,80($sp)
	lw	$2,148($2)
	andi	$2,$2,0xf00
	srl	$2,$2,8
	sb	$2,92($sp)
	lbu	$2,92($sp)
	sltu	$2,$2,8
	beq	$2,$0,$L168
	nop

	lbu	$2,92($sp)
	sb	$2,93($sp)
	j	$L169
	nop

$L168:
	lbu	$3,92($sp)
	li	$2,8			# 0x8
	bne	$3,$2,$L170
	nop

	sb	$0,93($sp)
	j	$L169
	nop

$L170:
	lw	$2,80($sp)
	j	$L171
	nop

$L169:
	lw	$2,88($sp)
	lhu	$3,0($2)
	li	$2,33024			# 0x8100
	bne	$3,$2,$L172
	nop

	lw	$2,88($sp)
	addiu	$2,$2,2
	lhu	$2,0($2)
	andi	$2,$2,0xfff
	sh	$2,94($sp)
	lw	$2,80($sp)
	lw	$2,256($2)
	addiu	$2,$2,12
	sw	$2,84($sp)
	lw	$2,84($sp)
	li	$3,-32512			# 0xffffffffffff8100
	sh	$3,0($2)
	lw	$2,84($sp)
	addiu	$2,$2,2
	sw	$2,84($sp)
	lbu	$2,93($sp)
	sll	$2,$2,13
	seh	$3,$2
	lh	$2,94($sp)
	or	$2,$3,$2
	seh	$2,$2
	andi	$3,$2,0xffff
	lw	$2,84($sp)
	sh	$3,0($2)
	lw	$2,80($sp)
	lw	$2,236($2)
	addiu	$3,$2,-4
	lw	$2,80($sp)
	sw	$3,236($2)
	lw	$2,80($sp)
	lw	$2,240($2)
	addiu	$3,$2,-4
	lw	$2,80($sp)
	sw	$3,240($2)
	j	$L167
	nop

$L172:
	lw	$2,80($sp)
	j	$L171
	nop

$L167:
	lw	$2,80($sp)
$L171:
	sw	$2,192($sp)
	lw	$2,192($sp)
	bne	$2,$0,$L173
	nop

	lui	$2,%hi($LC30)
	addiu	$4,$2,%lo($LC30)
	lui	$2,%hi(__func__.36861)
	addiu	$5,$2,%lo(__func__.36861)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	move	$2,$0
	j	$L224
	nop

$L173:
	lui	$2,%hi(gpPonSysData)
	lw	$2,%lo(gpPonSysData)($2)
	lw	$3,0($2)
	li	$2,1			# 0x1
	bne	$3,$2,$L174
	nop

	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lw	$3,4($2)
	li	$2,5			# 0x5
	bne	$3,$2,$L174
	nop

	lw	$2,52($sp)
	lbu	$2,0($2)
	move	$5,$2
	addiu	$3,$sp,136
	addiu	$2,$sp,144
	sw	$2,16($sp)
	move	$4,$3
	lw	$6,192($sp)
	lw	$7,44($sp)
	lui	$2,%hi(gwan_prepare_tx_message)
	addiu	$2,$2,%lo(gwan_prepare_tx_message)
	jalr	$2
	nop

	beq	$2,$0,$L175
	nop

	lui	$2,%hi(drop_print_flag)
	lw	$2,%lo(drop_print_flag)($2)
	beq	$2,$0,$L176
	nop

	li	$2,1417			# 0x589
	sw	$2,16($sp)
	lui	$2,%hi($LC29)
	addiu	$4,$2,%lo($LC29)
	lui	$2,%hi(__func__.36861)
	addiu	$5,$2,%lo(__func__.36861)
	li	$6,1417			# 0x589
	lui	$2,%hi(__func__.36861)
	addiu	$7,$2,%lo(__func__.36861)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	j	$L221
	nop

$L176:
	j	$L221
	nop

$L175:
	lw	$2,192($sp)
	lw	$2,220($2)
	ext	$2,$2,3,12
	andi	$2,$2,0xffff
	sh	$2,28($sp)
	j	$L177
	nop

$L174:
	lui	$2,%hi(gpPonSysData)
	lw	$2,%lo(gpPonSysData)($2)
	lw	$3,0($2)
	li	$2,2			# 0x2
	bne	$3,$2,$L178
	nop

	lw	$2,52($sp)
	lbu	$2,0($2)
	move	$5,$2
	addiu	$3,$sp,136
	addiu	$2,$sp,144
	sw	$2,16($sp)
	move	$4,$3
	lw	$6,192($sp)
	lw	$7,44($sp)
	lui	$2,%hi(ewan_prepare_tx_message)
	addiu	$2,$2,%lo(ewan_prepare_tx_message)
	jalr	$2
	nop

	beq	$2,$0,$L177
	nop

	lui	$2,%hi(drop_print_flag)
	lw	$2,%lo(drop_print_flag)($2)
	beq	$2,$0,$L179
	nop

	li	$2,1426			# 0x592
	sw	$2,16($sp)
	lui	$2,%hi($LC29)
	addiu	$4,$2,%lo($LC29)
	lui	$2,%hi(__func__.36861)
	addiu	$5,$2,%lo(__func__.36861)
	li	$6,1426			# 0x592
	lui	$2,%hi(__func__.36861)
	addiu	$7,$2,%lo(__func__.36861)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	j	$L221
	nop

$L179:
	j	$L221
	nop

$L178:
	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	lhu	$2,-30816($2)
	andi	$2,$2,0x1
	beq	$2,$0,$L180
	nop

	lui	$2,%hi(jiffies)
	lw	$3,%lo(jiffies)($2)
	lui	$2,%hi($LC31)
	addiu	$4,$2,%lo($LC31)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L180:
	lui	$2,%hi(drop_print_flag)
	lw	$2,%lo(drop_print_flag)($2)
	beq	$2,$0,$L181
	nop

	li	$2,1432			# 0x598
	sw	$2,16($sp)
	lui	$2,%hi($LC29)
	addiu	$4,$2,%lo($LC29)
	lui	$2,%hi(__func__.36861)
	addiu	$5,$2,%lo(__func__.36861)
	li	$6,1432			# 0x598
	lui	$2,%hi(__func__.36861)
	addiu	$7,$2,%lo(__func__.36861)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	j	$L221
	nop

$L181:
	j	$L221
	nop

$L177:
	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	lhu	$2,-30816($2)
	andi	$2,$2,0x80
	beq	$2,$0,$L182
	nop

	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	lhu	$2,-30816($2)
	andi	$2,$2,0x80
	beq	$2,$0,$L183
	nop

	lui	$2,%hi(jiffies)
	lw	$5,%lo(jiffies)($2)
	lw	$2,192($sp)
	lw	$6,80($2)
	lw	$3,136($sp)
	lw	$2,140($sp)
	sw	$2,16($sp)
	lui	$2,%hi($LC32)
	addiu	$4,$2,%lo($LC32)
	move	$7,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L183:
	lw	$2,192($sp)
	lw	$2,80($2)
	lw	$4,192($sp)
	move	$5,$2
	lui	$2,%hi(__dump_skb)
	addiu	$2,$2,%lo(__dump_skb)
	jalr	$2
	nop

$L182:
	lw	$2,52($sp)
	lbu	$3,0($2)
	li	$2,2			# 0x2
	beq	$3,$2,$L184
	nop

	lui	$2,%hi(gpPonSysData)
	lw	$2,%lo(gpPonSysData)($2)
	lw	$3,0($2)
	li	$2,2			# 0x2
	bne	$3,$2,$L185
	nop

	lw	$2,52($sp)
	lbu	$2,0($2)
	beq	$2,$0,$L186
	nop

$L185:
	lui	$2,%hi(gpPonSysData)
	lw	$2,%lo(gpPonSysData)($2)
	lw	$3,0($2)
	li	$2,1			# 0x1
	bne	$3,$2,$L187
	nop

	lw	$2,52($sp)
	lbu	$3,0($2)
	li	$2,1			# 0x1
	bne	$3,$2,$L187
	nop

$L186:
	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	lw	$3,-30412($2)
	li	$2,268435456			# 0x10000000
	and	$2,$3,$2
	bne	$2,$0,$L188
	nop

	lui	$2,%hi(drop_print_flag)
	lw	$2,%lo(drop_print_flag)($2)
	beq	$2,$0,$L189
	nop

	li	$2,1451			# 0x5ab
	sw	$2,16($sp)
	lui	$2,%hi($LC29)
	addiu	$4,$2,%lo($LC29)
	lui	$2,%hi(__func__.36861)
	addiu	$5,$2,%lo(__func__.36861)
	li	$6,1451			# 0x5ab
	lui	$2,%hi(__func__.36861)
	addiu	$7,$2,%lo(__func__.36861)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	j	$L221
	nop

$L189:
	j	$L221
	nop

$L188:
	nop
	j	$L194
	nop

$L187:
	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	lhu	$2,-30816($2)
	andi	$2,$2,0x80
	beq	$2,$0,$L191
	nop

	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	lhu	$2,-30816($2)
	andi	$2,$2,0x80
	beq	$2,$0,$L192
	nop

	lui	$2,%hi(jiffies)
	lw	$5,%lo(jiffies)($2)
	lui	$2,%hi(gpPonSysData)
	lw	$2,%lo(gpPonSysData)($2)
	lw	$2,0($2)
	lw	$3,52($sp)
	lbu	$3,0($3)
	sw	$2,16($sp)
	sw	$3,20($sp)
	lui	$2,%hi($LC33)
	addiu	$4,$2,%lo($LC33)
	lui	$2,%hi(__func__.36861)
	addiu	$6,$2,%lo(__func__.36861)
	li	$7,1458			# 0x5b2
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L192:
	lw	$2,192($sp)
	lw	$2,80($2)
	lw	$4,192($sp)
	move	$5,$2
	lui	$2,%hi(__dump_skb)
	addiu	$2,$2,%lo(__dump_skb)
	jalr	$2
	nop

$L191:
	lui	$2,%hi(drop_print_flag)
	lw	$2,%lo(drop_print_flag)($2)
	beq	$2,$0,$L193
	nop

	li	$2,1462			# 0x5b6
	sw	$2,16($sp)
	lui	$2,%hi($LC29)
	addiu	$4,$2,%lo($LC29)
	lui	$2,%hi(__func__.36861)
	addiu	$5,$2,%lo(__func__.36861)
	li	$6,1462			# 0x5b6
	lui	$2,%hi(__func__.36861)
	addiu	$7,$2,%lo(__func__.36861)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	j	$L221
	nop

$L193:
	j	$L221
	nop

$L184:
	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	lw	$3,-30412($2)
	li	$2,536870912			# 0x20000000
	and	$2,$3,$2
	bne	$2,$0,$L195
	nop

	lui	$2,%hi(drop_print_flag)
	lw	$2,%lo(drop_print_flag)($2)
	beq	$2,$0,$L196
	nop

	li	$2,1466			# 0x5ba
	sw	$2,16($sp)
	lui	$2,%hi($LC29)
	addiu	$4,$2,%lo($LC29)
	lui	$2,%hi(__func__.36861)
	addiu	$5,$2,%lo(__func__.36861)
	li	$6,1466			# 0x5ba
	lui	$2,%hi(__func__.36861)
	addiu	$7,$2,%lo(__func__.36861)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	j	$L221
	nop

$L196:
	j	$L221
	nop

$L195:
	lw	$2,192($sp)
	lw	$2,80($2)
	sltu	$2,$2,60
	andi	$2,$2,0x00ff
	beq	$2,$0,$L194
	nop

	lw	$2,192($sp)
	sw	$2,100($sp)
	li	$2,60			# 0x3c
	sw	$2,104($sp)
	lw	$2,100($sp)
	lw	$2,80($2)
	sw	$2,108($sp)
	lw	$3,104($sp)
	lw	$2,108($sp)
	sltu	$2,$2,$3
	xori	$2,$2,0x1
	andi	$2,$2,0x00ff
	beq	$2,$0,$L197
	nop

	move	$2,$0
	j	$L198
	nop

$L197:
	lw	$3,104($sp)
	lw	$2,108($sp)
	subu	$2,$3,$2
	lw	$4,100($sp)
	move	$5,$2
	lui	$2,%hi(skb_pad)
	addiu	$2,$2,%lo(skb_pad)
	jalr	$2
	nop

$L198:
	beq	$2,$0,$L199
	nop

	lui	$2,%hi(drop_print_flag)
	lw	$2,%lo(drop_print_flag)($2)
	beq	$2,$0,$L200
	nop

	li	$2,1489			# 0x5d1
	sw	$2,16($sp)
	lui	$2,%hi($LC29)
	addiu	$4,$2,%lo($LC29)
	lui	$2,%hi(__func__.36861)
	addiu	$5,$2,%lo(__func__.36861)
	li	$6,1489			# 0x5d1
	lui	$2,%hi(__func__.36861)
	addiu	$7,$2,%lo(__func__.36861)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	j	$L221
	nop

$L200:
	j	$L221
	nop

$L199:
	lw	$2,192($sp)
	lw	$2,80($2)
	li	$3,60			# 0x3c
	subu	$2,$3,$2
	lw	$4,192($sp)
	move	$5,$2
	lui	$2,%hi(skb_put)
	addiu	$2,$2,%lo(skb_put)
	jalr	$2
	nop

$L194:
	lbu	$2,38($sp)
	bne	$2,$0,$L201
	nop

	lui	$2,%hi(masko_on_off)
	lw	$2,%lo(masko_on_off)($2)
	beq	$2,$0,$L202
	nop

	li	$2,1			# 0x1
	j	$L203
	nop

$L202:
	move	$2,$0
$L203:
	beq	$2,$0,$L201
	nop

	lw	$2,52($sp)
	lbu	$2,0($2)
	lw	$4,192($sp)
	move	$5,$2
	lui	$2,%hi(copy_pwan_skb)
	addiu	$2,$2,%lo(copy_pwan_skb)
	jalr	$2
	nop

	sw	$2,32($sp)
$L201:
	lw	$2,192($sp)
	lw	$2,80($2)
	sh	$2,36($sp)
	lw	$3,136($sp)
	lw	$2,140($sp)
	li	$4,16			# 0x10
	sw	$4,112($sp)
	lw	$4,192($sp)
	sw	$4,116($sp)
	sw	$3,120($sp)
	sw	$2,124($sp)
	addiu	$2,$sp,144
	sw	$2,128($sp)
	sw	$0,132($sp)
	li	$2,10			# 0xa
	sw	$2,152($sp)
	lw	$2,116($sp)
	sw	$2,160($sp)
	lw	$2,120($sp)
	sw	$2,164($sp)
	lw	$2,124($sp)
	sw	$2,168($sp)
	lw	$2,128($sp)
	sw	$2,172($sp)
	addiu	$2,$sp,152
	lw	$4,112($sp)
	move	$5,$0
	move	$6,$2
	lui	$2,%hi(__ECNT_HOOK)
	addiu	$2,$2,%lo(__ECNT_HOOK)
	jalr	$2
	nop

	sw	$2,132($sp)
	lw	$3,132($sp)
	li	$2,-1			# 0xffffffffffffffff
	beq	$3,$2,$L204
	nop

	lw	$2,156($sp)
	j	$L206
	nop

$L204:
	li	$2,-1			# 0xffffffffffffffff
$L206:
	sw	$2,24($sp)
	lw	$2,24($sp)
	beq	$2,$0,$L207
	nop

	lw	$2,24($sp)
	bgez	$2,$L208
	nop

	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	lhu	$2,-30816($2)
	andi	$2,$2,0x1
	beq	$2,$0,$L208
	nop

	lui	$2,%hi(jiffies)
	lw	$3,%lo(jiffies)($2)
	lui	$2,%hi($LC34)
	addiu	$4,$2,%lo($LC34)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L208:
	lw	$2,52($sp)
	lbu	$3,0($2)
	li	$2,2			# 0x2
	bne	$3,$2,$L209
	nop

	lui	$2,%hi(drop_print_flag)
	lw	$2,%lo(drop_print_flag)($2)
	beq	$2,$0,$L210
	nop

	li	$2,1532			# 0x5fc
	sw	$2,16($sp)
	lui	$2,%hi($LC29)
	addiu	$4,$2,%lo($LC29)
	lui	$2,%hi(__func__.36861)
	addiu	$5,$2,%lo(__func__.36861)
	li	$6,1532			# 0x5fc
	lui	$2,%hi(__func__.36861)
	addiu	$7,$2,%lo(__func__.36861)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	j	$L221
	nop

$L210:
	j	$L221
	nop

$L209:
	lui	$2,%hi(drop_print_flag)
	lw	$2,%lo(drop_print_flag)($2)
	beq	$2,$0,$L211
	nop

	li	$2,1535			# 0x5ff
	sw	$2,16($sp)
	lui	$2,%hi($LC29)
	addiu	$4,$2,%lo($LC29)
	lui	$2,%hi(__func__.36861)
	addiu	$5,$2,%lo(__func__.36861)
	li	$6,1535			# 0x5ff
	lui	$2,%hi(__func__.36861)
	addiu	$7,$2,%lo(__func__.36861)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	j	$L221
	nop

$L211:
	j	$L221
	nop

$L207:
	lw	$2,52($sp)
	lw	$2,52($2)
	addiu	$3,$2,1
	lw	$2,52($sp)
	sw	$3,52($2)
	lw	$2,52($sp)
	lw	$3,60($2)
	lhu	$2,36($sp)
	addu	$3,$3,$2
	lw	$2,52($sp)
	sw	$3,60($2)
	lui	$2,%hi(gpPonSysData)
	lw	$2,%lo(gpPonSysData)($2)
	lw	$3,0($2)
	li	$2,1			# 0x1
	bne	$3,$2,$L160
	nop

	lw	$2,52($sp)
	lbu	$3,0($2)
	li	$2,1			# 0x1
	bne	$3,$2,$L212
	nop

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
	sh	$2,30($sp)
	j	$L213
	nop

$L212:
	lhu	$2,28($sp)
	beq	$2,$0,$L214
	nop

	lui	$2,%hi(gpWanPriv)
	lw	$3,%lo(gpWanPriv)($2)
	lhu	$2,28($sp)
	addiu	$2,$2,44
	sll	$2,$2,1
	addu	$2,$3,$2
	lhu	$2,4($2)
	andi	$2,$2,0x7fff
	andi	$2,$2,0xffff
	j	$L215
	nop

$L214:
	li	$2,256			# 0x100
$L215:
	sh	$2,30($sp)
$L213:
	lhu	$2,30($sp)
	sltu	$2,$2,256
	beq	$2,$0,$L160
	nop

	lui	$2,%hi(gpWanPriv)
	lw	$3,%lo(gpWanPriv)($2)
	lhu	$5,30($sp)
	move	$2,$5
	sll	$2,$2,5
	sll	$4,$2,2
	subu	$2,$4,$2
	addu	$2,$3,$2
	lw	$2,8292($2)
	addiu	$4,$2,1
	move	$2,$5
	sll	$2,$2,5
	sll	$5,$2,2
	subu	$2,$5,$2
	addu	$2,$3,$2
	sw	$4,8292($2)
	lui	$2,%hi(gpWanPriv)
	lw	$4,%lo(gpWanPriv)($2)
	lhu	$6,30($sp)
	lui	$2,%hi(gpWanPriv)
	lw	$3,%lo(gpWanPriv)($2)
	lhu	$2,30($sp)
	sll	$2,$2,5
	sll	$5,$2,2
	subu	$2,$5,$2
	addu	$2,$3,$2
	lw	$3,8300($2)
	lhu	$2,36($sp)
	addu	$3,$3,$2
	move	$2,$6
	sll	$2,$2,5
	sll	$5,$2,2
	subu	$2,$5,$2
	addu	$2,$4,$2
	sw	$3,8300($2)
$L160:
	lbu	$2,38($sp)
	bne	$2,$0,$L216
	nop

	lui	$2,%hi(masko_on_off)
	lw	$2,%lo(masko_on_off)($2)
	beq	$2,$0,$L217
	nop

	li	$2,1			# 0x1
	j	$L218
	nop

$L217:
	move	$2,$0
$L218:
	beq	$2,$0,$L216
	nop

	lw	$2,32($sp)
	beq	$2,$0,$L216
	nop

	li	$4,-2147483648			# 0xffffffff80000000
	lw	$5,32($sp)
	lui	$2,%hi(macSend)
	addiu	$2,$2,%lo(macSend)
	jalr	$2
	nop

	sw	$0,32($sp)
$L216:
	lw	$2,52($sp)
	lbu	$3,0($2)
	li	$2,2			# 0x2
	bne	$3,$2,$L219
	nop

	lui	$2,%hi(internet_led_on)
	lw	$2,%lo(internet_led_on)($2)
	beq	$2,$0,$L220
	nop

	li	$4,34			# 0x22
	lui	$2,%hi(ledTurnOn)
	addiu	$2,$2,%lo(ledTurnOn)
	jalr	$2
	nop

	j	$L219
	nop

$L220:
	lui	$2,%hi(internet_trying_led_on)
	lw	$2,%lo(internet_trying_led_on)($2)
	bne	$2,$0,$L219
	nop

	li	$4,33			# 0x21
	lui	$2,%hi(ledTurnOff)
	addiu	$2,$2,%lo(ledTurnOff)
	jalr	$2
	nop

	li	$4,38			# 0x26
	lui	$2,%hi(ledTurnOff)
	addiu	$2,$2,%lo(ledTurnOff)
	jalr	$2
	nop

$L219:
	lui	$2,%hi(jiffies)
	lw	$3,%lo(jiffies)($2)
	lw	$2,196($sp)
	sw	$3,568($2)
	lw	$2,24($sp)
	j	$L224
	nop

$L156 = .
$L221:
	lw	$2,52($sp)
	lw	$2,76($2)
	addiu	$3,$2,1
	lw	$2,52($sp)
	sw	$3,76($2)
	lw	$2,192($sp)
	beq	$2,$0,$L222
	nop

	lbu	$2,38($sp)
	bne	$2,$0,$L222
	nop

	lw	$4,192($sp)
	lui	$2,%hi(dev_kfree_skb_any)
	addiu	$2,$2,%lo(dev_kfree_skb_any)
	jalr	$2
	nop

$L222:
	lw	$2,32($sp)
	beq	$2,$0,$L223
	nop

	lw	$4,32($sp)
	lui	$2,%hi(dev_kfree_skb_any)
	addiu	$2,$2,%lo(dev_kfree_skb_any)
	jalr	$2
	nop

$L223:
	lw	$2,24($sp)
$L224:
	lw	$31,188($sp)
	addiu	$sp,$sp,192
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	pwan_net_start_xmit
	.size	pwan_net_start_xmit, .-pwan_net_start_xmit
	.section	__ksymtab_strings
	.type	__kstrtab_pwan_net_start_xmit, @object
	.size	__kstrtab_pwan_net_start_xmit, 20
__kstrtab_pwan_net_start_xmit:
	.ascii	"pwan_net_start_xmit\000"
	.section	__ksymtab
	.align	2
	.type	__ksymtab_pwan_net_start_xmit, @object
	.size	__ksymtab_pwan_net_start_xmit, 8
__ksymtab_pwan_net_start_xmit:
	.word	pwan_net_start_xmit
	.word	__kstrtab_pwan_net_start_xmit
	.section	.text.pwan_net_get_stats,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	pwan_net_get_stats
	.type	pwan_net_get_stats, @function
pwan_net_get_stats:
	.frame	$sp,8,$31		# vars= 8, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-8
	sw	$4,8($sp)
	lw	$2,8($sp)
	sw	$2,4($sp)
	lw	$2,4($sp)
	addiu	$2,$2,928
	sw	$2,0($sp)
	lw	$2,0($sp)
	addiu	$2,$2,48
	addiu	$sp,$sp,8
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	pwan_net_get_stats
	.size	pwan_net_get_stats, .-pwan_net_get_stats
	.section	.text.pwan_net_set_multicast_list,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	pwan_net_set_multicast_list
	.type	pwan_net_set_multicast_list, @function
pwan_net_set_multicast_list:
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	sw	$4,0($sp)
	nop
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	pwan_net_set_multicast_list
	.size	pwan_net_set_multicast_list, .-pwan_net_set_multicast_list
	.section	.text.pwan_net_ioctl,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	pwan_net_ioctl
	.type	pwan_net_ioctl, @function
pwan_net_ioctl:
	.frame	$sp,8,$31		# vars= 8, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-8
	sw	$4,8($sp)
	sw	$5,12($sp)
	sw	$6,16($sp)
	sw	$0,0($sp)
	nop
	lw	$2,0($sp)
	addiu	$sp,$sp,8
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	pwan_net_ioctl
	.size	pwan_net_ioctl, .-pwan_net_ioctl
	.section	.text.pwan_net_set_macaddr,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	pwan_net_set_macaddr
	.type	pwan_net_set_macaddr, @function
pwan_net_set_macaddr:
	.frame	$sp,40,$31		# vars= 16, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-40
	sw	$31,36($sp)
	sw	$4,40($sp)
	sw	$5,44($sp)
	lw	$2,44($sp)
	sw	$2,16($sp)
	lw	$2,16($sp)
	addiu	$2,$2,2
	sw	$2,20($sp)
	lw	$2,20($sp)
	sw	$2,24($sp)
	lw	$2,24($sp)
	lbu	$2,0($2)
	andi	$2,$2,0x1
	bne	$2,$0,$L234
	nop

	lw	$2,20($sp)
	sw	$2,28($sp)
	lw	$2,28($sp)
	lbu	$3,0($2)
	lw	$2,28($sp)
	addiu	$2,$2,1
	lbu	$2,0($2)
	or	$2,$3,$2
	andi	$3,$2,0x00ff
	lw	$2,28($sp)
	addiu	$2,$2,2
	lbu	$2,0($2)
	or	$2,$3,$2
	andi	$3,$2,0x00ff
	lw	$2,28($sp)
	addiu	$2,$2,3
	lbu	$2,0($2)
	or	$2,$3,$2
	andi	$3,$2,0x00ff
	lw	$2,28($sp)
	addiu	$2,$2,4
	lbu	$2,0($2)
	or	$2,$3,$2
	andi	$3,$2,0x00ff
	lw	$2,28($sp)
	addiu	$2,$2,5
	lbu	$2,0($2)
	or	$2,$3,$2
	andi	$2,$2,0x00ff
	sltu	$2,$2,1
	andi	$2,$2,0x00ff
	bne	$2,$0,$L234
	nop

	li	$2,1			# 0x1
	j	$L237
	nop

$L234:
	move	$2,$0
$L237:
	bne	$2,$0,$L238
	nop

	li	$2,-5			# 0xfffffffffffffffb
	j	$L239
	nop

$L238:
	lw	$2,40($sp)
	lw	$4,364($2)
	lw	$2,16($sp)
	addiu	$3,$2,2
	lw	$2,40($sp)
	lbu	$2,289($2)
	move	$5,$3
	move	$6,$2
	lui	$2,%hi(memcpy)
	addiu	$2,$2,%lo(memcpy)
	jalr	$2
	nop

	move	$2,$0
$L239:
	lw	$31,36($sp)
	addiu	$sp,$sp,40
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	pwan_net_set_macaddr
	.size	pwan_net_set_macaddr, .-pwan_net_set_macaddr
	.section	.text.pwan_net_poll,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	pwan_net_poll
	.type	pwan_net_poll, @function
pwan_net_poll:
	.frame	$sp,152,$31		# vars= 128, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-152
	sw	$31,148($sp)
	sw	$4,152($sp)
	sw	$5,156($sp)
	sw	$0,16($sp)
	sw	$0,20($sp)
	lw	$2,156($sp)
	sw	$2,24($sp)
	li	$2,16			# 0x10
	sw	$2,32($sp)
	lw	$2,24($sp)
	sw	$2,36($sp)
	sw	$0,40($sp)
	li	$2,9			# 0x9
	sw	$2,100($sp)
	lw	$2,36($sp)
	sw	$2,108($sp)
	addiu	$2,$sp,100
	lw	$4,32($sp)
	move	$5,$0
	move	$6,$2
	lui	$2,%hi(__ECNT_HOOK)
	addiu	$2,$2,%lo(__ECNT_HOOK)
	jalr	$2
	nop

	sw	$2,40($sp)
	lw	$3,40($sp)
	li	$2,-1			# 0xffffffffffffffff
	beq	$3,$2,$L241
	nop

	lw	$2,104($sp)
	j	$L243
	nop

$L241:
	li	$2,-1			# 0xffffffffffffffff
$L243:
	sw	$2,16($sp)
	lw	$3,16($sp)
	lw	$2,24($sp)
	slt	$2,$3,$2
	beq	$2,$0,$L244
	nop

	li	$2,1			# 0x1
	sw	$2,20($sp)
	lui	$2,%hi(gpWanPriv)
	lw	$3,%lo(gpWanPriv)($2)
	li	$2,33752			# 0x83d8
	addu	$2,$3,$2
	sw	$2,44($sp)
	lw	$2,44($sp)
	move	$4,$2
	lui	$2,%hi(_raw_spin_lock_irqsave)
	addiu	$2,$2,%lo(_raw_spin_lock_irqsave)
	jalr	$2
	nop

	sw	$2,28($sp)
	lw	$4,152($sp)
	lui	$2,%hi(__napi_complete)
	addiu	$2,$2,%lo(__napi_complete)
	jalr	$2
	nop

	li	$2,16			# 0x10
	sw	$2,48($sp)
	sw	$0,52($sp)
	li	$2,7			# 0x7
	sw	$2,72($sp)
	addiu	$2,$sp,72
	lw	$4,48($sp)
	move	$5,$0
	move	$6,$2
	lui	$2,%hi(__ECNT_HOOK)
	addiu	$2,$2,%lo(__ECNT_HOOK)
	jalr	$2
	nop

	sw	$2,52($sp)
	lui	$2,%hi(gpWanPriv)
	lw	$3,%lo(gpWanPriv)($2)
	li	$2,33752			# 0x83d8
	addu	$2,$3,$2
	sw	$2,56($sp)
	lw	$2,28($sp)
	sw	$2,60($sp)
	lw	$2,56($sp)
	move	$4,$2
	lw	$5,60($sp)
	lui	$2,%hi(_raw_spin_unlock_irqrestore)
	addiu	$2,$2,%lo(_raw_spin_unlock_irqrestore)
	jalr	$2
	nop

	j	$L249
	nop

$L244:
	sw	$0,20($sp)
$L249:
	lw	$2,16($sp)
	lw	$31,148($sp)
	addiu	$sp,$sp,152
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	pwan_net_poll
	.size	pwan_net_poll, .-pwan_net_poll
	.globl	xpon_netdev_ops
	.rdata
	.align	2
	.type	xpon_netdev_ops, @object
	.size	xpon_netdev_ops, 108
xpon_netdev_ops:
	.space	8
	.word	pwan_net_open
	.word	pwan_net_stop
	.word	pwan_net_start_xmit
	.space	12
	.word	pwan_net_set_multicast_list
	.word	pwan_net_set_macaddr
	.space	4
	.word	pwan_net_ioctl
	.space	20
	.word	pwan_net_get_stats
	.space	36
	.align	2
$LC35:
	.ascii	"<6>\012The MAC address in flash is null! Use default!\012"
	.ascii	"\000"
	.align	2
$LC36:
	.ascii	"oam\000"
	.align	2
$LC37:
	.ascii	"omci\000"
	.align	2
$LC38:
	.ascii	"[%lu0ms]Alloc net interface device %d (alloc_netdev()) f"
	.ascii	"ailed\012\000"
	.align	2
$LC39:
	.ascii	"[%lu0ms]Register net device %s failed\012\000"
	.align	2
$LC0:
	.byte	0
	.byte	12
	.byte	-25
	.byte	1
	.byte	35
	.byte	69
	.section	.text.pwan_create_net_interface,"ax",@progbits
	.align	2
	.globl	pwan_create_net_interface
	.set	nomips16
	.set	nomicromips
	.ent	pwan_create_net_interface
	.type	pwan_create_net_interface, @function
pwan_create_net_interface:
	.frame	$sp,80,$31		# vars= 48, regs= 4/0, args= 16, gp= 0
	.mask	0x800d0000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-80
	sw	$31,76($sp)
	sw	$19,72($sp)
	sw	$18,68($sp)
	sw	$16,64($sp)
	sw	$4,80($sp)
	lui	$2,%hi($LC0)
	addiu	$3,$2,%lo($LC0)
	lwl	$4,%lo($LC0)($2)
	move	$5,$4
	lwr	$5,3($3)
	move	$3,$5
	sw	$3,28($sp)
	addiu	$2,$2,%lo($LC0)
	lbu	$3,4($2)
	sb	$3,32($sp)
	lbu	$2,5($2)
	sb	$2,33($sp)
	lw	$2,80($sp)
	sltu	$2,$2,3
	bne	$2,$0,$L252
	nop

	li	$2,-22			# 0xffffffffffffffea
	j	$L273
	nop

$L252:
	lui	$2,%hi(gpWanPriv)
	lw	$3,%lo(gpWanPriv)($2)
	lw	$2,80($sp)
	sll	$2,$2,2
	addu	$2,$3,$2
	lw	$2,0($2)
	beq	$2,$0,$L254
	nop

	li	$2,-17			# 0xffffffffffffffef
	j	$L273
	nop

$L254:
	sb	$0,16($sp)
	j	$L255
	nop

$L260:
	lbu	$16,16($sp)
	li	$2,-1079967744			# 0xffffffffbfa10000
	ori	$2,$2,0x114
	lw	$2,0($2)
	andi	$2,$2,0x2
	beq	$2,$0,$L256
	nop

	lui	$2,%hi(ranand_read_byte)
	lw	$2,%lo(ranand_read_byte)($2)
	beq	$2,$0,$L257
	nop

	lui	$2,%hi(ranand_read_byte)
	lw	$2,%lo(ranand_read_byte)($2)
	lbu	$4,16($sp)
	lui	$3,%hi(flash_base)
	lw	$3,%lo(flash_base)($3)
	addu	$4,$4,$3
	li	$3,65352			# 0xff48
	addu	$3,$4,$3
	move	$19,$3
	move	$18,$0
	move	$5,$19
	move	$4,$18
	jalr	$2
	nop

	andi	$2,$2,0x00ff
	j	$L259
	nop

$L257:
	li	$2,255			# 0xff
	j	$L259
	nop

$L256:
	lbu	$3,16($sp)
	lui	$2,%hi(flash_base)
	lw	$2,%lo(flash_base)($2)
	addu	$3,$3,$2
	li	$2,65352			# 0xff48
	addu	$2,$3,$2
	move	$4,$2
	lui	$2,%hi(ReadSPIByte)
	addiu	$2,$2,%lo(ReadSPIByte)
	jalr	$2
	nop

$L259:
	addiu	$3,$sp,16
	addu	$3,$3,$16
	sb	$2,20($3)
	lbu	$2,16($sp)
	addiu	$2,$2,1
	sb	$2,16($sp)
$L255:
	lbu	$2,16($sp)
	sltu	$2,$2,6
	bne	$2,$0,$L260
	nop

	lbu	$2,36($sp)
	bne	$2,$0,$L261
	nop

	lbu	$2,37($sp)
	bne	$2,$0,$L261
	nop

	lbu	$2,38($sp)
	bne	$2,$0,$L261
	nop

	lbu	$2,39($sp)
	bne	$2,$0,$L261
	nop

	lbu	$2,40($sp)
	bne	$2,$0,$L261
	nop

	lbu	$2,41($sp)
	bne	$2,$0,$L261
	nop

	lui	$2,%hi($LC35)
	addiu	$4,$2,%lo($LC35)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	j	$L262
	nop

$L261:
	addiu	$3,$sp,28
	addiu	$2,$sp,36
	move	$4,$3
	move	$5,$2
	li	$6,6			# 0x6
	lui	$2,%hi(memcpy)
	addiu	$2,$2,%lo(memcpy)
	jalr	$2
	nop

$L262:
	addiu	$2,$sp,44
	move	$4,$2
	move	$5,$0
	li	$6,16			# 0x10
	lui	$2,%hi(memset)
	addiu	$2,$2,%lo(memset)
	jalr	$2
	nop

	lw	$2,80($sp)
	bne	$2,$0,$L263
	nop

	addiu	$2,$sp,44
	move	$4,$2
	lui	$2,%hi($LC36)
	addiu	$5,$2,%lo($LC36)
	lui	$2,%hi(sprintf)
	addiu	$2,$2,%lo(sprintf)
	jalr	$2
	nop

	j	$L264
	nop

$L263:
	lw	$3,80($sp)
	li	$2,1			# 0x1
	bne	$3,$2,$L265
	nop

	addiu	$2,$sp,44
	move	$4,$2
	lui	$2,%hi($LC37)
	addiu	$5,$2,%lo($LC37)
	lui	$2,%hi(sprintf)
	addiu	$2,$2,%lo(sprintf)
	jalr	$2
	nop

	j	$L264
	nop

$L265:
	addiu	$2,$sp,44
	move	$4,$2
	lui	$2,%hi($LC25)
	addiu	$5,$2,%lo($LC25)
	lui	$2,%hi(sprintf)
	addiu	$2,$2,%lo(sprintf)
	jalr	$2
	nop

$L264:
	lui	$2,%hi(gpWanPriv)
	lw	$16,%lo(gpWanPriv)($2)
	addiu	$2,$sp,44
	li	$4,140			# 0x8c
	move	$5,$2
	lui	$2,%hi(ether_setup)
	addiu	$6,$2,%lo(ether_setup)
	li	$7,1			# 0x1
	lui	$2,%hi(alloc_netdev_mq)
	addiu	$2,$2,%lo(alloc_netdev_mq)
	jalr	$2
	nop

	move	$3,$2
	lw	$2,80($sp)
	sll	$2,$2,2
	addu	$2,$16,$2
	sw	$3,0($2)
	lui	$2,%hi(gpWanPriv)
	lw	$3,%lo(gpWanPriv)($2)
	lw	$2,80($sp)
	sll	$2,$2,2
	addu	$2,$3,$2
	lw	$2,0($2)
	bne	$2,$0,$L266
	nop

	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	lhu	$2,-30816($2)
	andi	$2,$2,0x1
	beq	$2,$0,$L267
	nop

	lui	$2,%hi(jiffies)
	lw	$3,%lo(jiffies)($2)
	lui	$2,%hi($LC38)
	addiu	$4,$2,%lo($LC38)
	move	$5,$3
	lw	$6,80($sp)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L267:
	li	$2,-12			# 0xfffffffffffffff4
	j	$L273
	nop

$L266:
	lui	$2,%hi(gpWanPriv)
	lw	$3,%lo(gpWanPriv)($2)
	lw	$2,80($sp)
	sll	$2,$2,2
	addu	$2,$3,$2
	lw	$2,0($2)
	sw	$2,24($sp)
	lw	$2,24($sp)
	addiu	$2,$2,928
	sw	$2,20($sp)
	lw	$4,20($sp)
	move	$5,$0
	li	$6,140			# 0x8c
	lui	$2,%hi(memset)
	addiu	$2,$2,%lo(memset)
	jalr	$2
	nop

	lw	$2,80($sp)
	andi	$3,$2,0x00ff
	lw	$2,20($sp)
	sb	$3,0($2)
	lw	$2,20($sp)
	sb	$0,1($2)
	lw	$2,20($sp)
	addiu	$2,$2,48
	move	$4,$2
	move	$5,$0
	li	$6,92			# 0x5c
	lui	$2,%hi(memset)
	addiu	$2,$2,%lo(memset)
	jalr	$2
	nop

	lui	$2,%hi(gpWanPriv)
	lw	$3,%lo(gpWanPriv)($2)
	lw	$2,80($sp)
	sll	$2,$2,2
	addu	$2,$3,$2
	lw	$2,0($2)
	li	$3,6			# 0x6
	sb	$3,289($2)
	lui	$2,%hi(gpWanPriv)
	lw	$3,%lo(gpWanPriv)($2)
	lw	$2,80($sp)
	sll	$2,$2,2
	addu	$2,$3,$2
	lw	$2,0($2)
	lw	$3,364($2)
	addiu	$2,$sp,28
	move	$4,$3
	move	$5,$2
	li	$6,6			# 0x6
	lui	$2,%hi(memcpy)
	addiu	$2,$2,%lo(memcpy)
	jalr	$2
	nop

	lui	$2,%hi(gpWanPriv)
	lw	$3,%lo(gpWanPriv)($2)
	lw	$2,80($sp)
	sll	$2,$2,2
	addu	$2,$3,$2
	lw	$2,0($2)
	li	$3,2000			# 0x7d0
	sw	$3,240($2)
	lui	$2,%hi(gpWanPriv)
	lw	$3,%lo(gpWanPriv)($2)
	lw	$2,80($sp)
	sll	$2,$2,2
	addu	$2,$3,$2
	lw	$2,0($2)
	lui	$3,%hi(xpon_netdev_ops)
	addiu	$3,$3,%lo(xpon_netdev_ops)
	sw	$3,212($2)
	lw	$2,20($sp)
	li	$3,128			# 0x80
	sw	$3,16($2)
	lui	$2,%hi(gpWanPriv)
	lw	$3,%lo(gpWanPriv)($2)
	lw	$2,80($sp)
	sll	$2,$2,2
	addu	$2,$3,$2
	lw	$3,0($2)
	lw	$2,20($sp)
	addiu	$2,$2,4
	move	$4,$3
	move	$5,$2
	lui	$2,%hi(pwan_net_poll)
	addiu	$6,$2,%lo(pwan_net_poll)
	li	$7,128			# 0x80
	lui	$2,%hi(netif_napi_add)
	addiu	$2,$2,%lo(netif_napi_add)
	jalr	$2
	nop

	lw	$2,80($sp)
	beq	$2,$0,$L269
	nop

	lw	$3,80($sp)
	li	$2,1			# 0x1
	bne	$3,$2,$L270
	nop

$L269:
	lui	$2,%hi(gpWanPriv)
	lw	$3,%lo(gpWanPriv)($2)
	lw	$2,80($sp)
	sll	$2,$2,2
	addu	$2,$3,$2
	lw	$2,0($2)
	lui	$3,%hi(gpWanPriv)
	lw	$4,%lo(gpWanPriv)($3)
	lw	$3,80($sp)
	sll	$3,$3,2
	addu	$3,$4,$3
	lw	$3,0($3)
	lw	$3,224($3)
	ori	$3,$3,0x80
	sw	$3,224($2)
	lui	$2,%hi(gpWanPriv)
	lw	$3,%lo(gpWanPriv)($2)
	lw	$2,80($sp)
	sll	$2,$2,2
	addu	$2,$3,$2
	lw	$2,0($2)
	lui	$3,%hi(gpWanPriv)
	lw	$4,%lo(gpWanPriv)($3)
	lw	$3,80($sp)
	sll	$3,$3,2
	addu	$3,$4,$3
	lw	$3,0($3)
	lw	$3,224($3)
	ori	$3,$3,0x2
	sw	$3,224($2)
	lui	$2,%hi(gpWanPriv)
	lw	$3,%lo(gpWanPriv)($2)
	lw	$2,80($sp)
	sll	$2,$2,2
	addu	$2,$3,$2
	lw	$2,0($2)
	lui	$3,%hi(gpWanPriv)
	lw	$4,%lo(gpWanPriv)($3)
	lw	$3,80($sp)
	sll	$3,$3,2
	addu	$3,$4,$3
	lw	$3,0($3)
	lw	$4,224($3)
	li	$3,-4097			# 0xffffffffffffefff
	and	$3,$4,$3
	sw	$3,224($2)
	lui	$2,%hi(gpWanPriv)
	lw	$3,%lo(gpWanPriv)($2)
	lw	$2,80($sp)
	sll	$2,$2,2
	addu	$2,$3,$2
	lw	$2,0($2)
	lui	$3,%hi(gpWanPriv)
	lw	$4,%lo(gpWanPriv)($3)
	lw	$3,80($sp)
	sll	$3,$3,2
	addu	$3,$4,$3
	lw	$3,0($3)
	lw	$3,100($3)
	ori	$3,$3,0x4
	sw	$3,100($2)
$L270:
	lui	$2,%hi(gpWanPriv)
	lw	$3,%lo(gpWanPriv)($2)
	lw	$2,80($sp)
	sll	$2,$2,2
	addu	$2,$3,$2
	lw	$2,0($2)
	move	$4,$2
	lui	$2,%hi(register_netdev)
	addiu	$2,$2,%lo(register_netdev)
	jalr	$2
	nop

	beq	$2,$0,$L271
	nop

	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	lhu	$2,-30816($2)
	andi	$2,$2,0x1
	beq	$2,$0,$L272
	nop

	lui	$2,%hi(jiffies)
	lw	$5,%lo(jiffies)($2)
	addiu	$3,$sp,44
	lui	$2,%hi($LC39)
	addiu	$4,$2,%lo($LC39)
	move	$6,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L272:
	lui	$2,%hi(gpWanPriv)
	lw	$3,%lo(gpWanPriv)($2)
	lw	$2,80($sp)
	sll	$2,$2,2
	addu	$2,$3,$2
	lw	$2,0($2)
	move	$4,$2
	lui	$2,%hi(free_netdev)
	addiu	$2,$2,%lo(free_netdev)
	jalr	$2
	nop

	lui	$2,%hi(gpWanPriv)
	lw	$3,%lo(gpWanPriv)($2)
	lw	$2,80($sp)
	sll	$2,$2,2
	addu	$2,$3,$2
	sw	$0,0($2)
	li	$2,-14			# 0xfffffffffffffff2
	j	$L273
	nop

$L271:
	move	$2,$0
$L273:
	lw	$31,76($sp)
	lw	$19,72($sp)
	lw	$18,68($sp)
	lw	$16,64($sp)
	addiu	$sp,$sp,80
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	pwan_create_net_interface
	.size	pwan_create_net_interface, .-pwan_create_net_interface
	.section	.text.pwan_net_del,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	pwan_net_del
	.type	pwan_net_del, @function
pwan_net_del:
	.frame	$sp,48,$31		# vars= 24, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	addiu	$sp,$sp,-48
	sw	$31,44($sp)
	sw	$4,48($sp)
	lw	$2,48($sp)
	sw	$2,20($sp)
	lui	$2,%hi($LC25)
	addiu	$2,$2,%lo($LC25)
	sw	$2,24($sp)
	lw	$3,20($sp)
	lw	$2,24($sp)
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
	sw	$3,20($sp)
	sw	$2,24($sp)
	sw	$4,28($sp)
	lw	$2,28($sp)
	bne	$2,$0,$L276
	lw	$2,48($sp)
	lw	$2,224($2)
	andi	$2,$2,0x1
	beq	$2,$0,$L276
	lui	$2,%hi(gpWanPriv)
	lw	$3,%lo(gpWanPriv)($2)
	li	$2,33660			# 0x837c
	addu	$2,$3,$2
	move	$4,$2
	lui	$2,%hi(del_timer_sync)
	addiu	$2,$2,%lo(del_timer_sync)
	jalr	$2
$L276:
	lw	$2,48($sp)
	sw	$2,32($sp)
	lw	$2,32($sp)
	addiu	$2,$2,928
	sw	$2,16($sp)
	lw	$2,16($sp)
	addiu	$2,$2,4
	move	$4,$2
	lui	$2,%hi(netif_napi_del)
	addiu	$2,$2,%lo(netif_napi_del)
	jalr	$2
	move	$2,$0
	lw	$31,44($sp)
	addiu	$sp,$sp,48
	j	$31
	.end	pwan_net_del
	.size	pwan_net_del, .-pwan_net_del
	.section	.text.pwan_delete_net_interface,"ax",@progbits
	.align	2
	.globl	pwan_delete_net_interface
	.set	nomips16
	.set	nomicromips
	.ent	pwan_delete_net_interface
	.type	pwan_delete_net_interface, @function
pwan_delete_net_interface:
	.frame	$sp,24,$31		# vars= 0, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-24
	sw	$31,20($sp)
	sw	$4,24($sp)
	lw	$2,24($sp)
	sltu	$2,$2,3
	bne	$2,$0,$L280
	nop

	li	$2,-14			# 0xfffffffffffffff2
	j	$L281
	nop

$L280:
	lui	$2,%hi(gpWanPriv)
	lw	$3,%lo(gpWanPriv)($2)
	lw	$2,24($sp)
	sll	$2,$2,2
	addu	$2,$3,$2
	lw	$2,0($2)
	bne	$2,$0,$L282
	nop

	li	$2,-17			# 0xffffffffffffffef
	j	$L281
	nop

$L282:
	lui	$2,%hi(gpWanPriv)
	lw	$3,%lo(gpWanPriv)($2)
	lw	$2,24($sp)
	sll	$2,$2,2
	addu	$2,$3,$2
	lw	$2,0($2)
	move	$4,$2
	lui	$2,%hi(pwan_net_del)
	addiu	$2,$2,%lo(pwan_net_del)
	jalr	$2
	nop

	lui	$2,%hi(gpWanPriv)
	lw	$3,%lo(gpWanPriv)($2)
	lw	$2,24($sp)
	sll	$2,$2,2
	addu	$2,$3,$2
	lw	$2,0($2)
	move	$4,$2
	lui	$2,%hi(unregister_netdev)
	addiu	$2,$2,%lo(unregister_netdev)
	jalr	$2
	nop

	lui	$2,%hi(gpWanPriv)
	lw	$3,%lo(gpWanPriv)($2)
	lw	$2,24($sp)
	sll	$2,$2,2
	addu	$2,$3,$2
	lw	$2,0($2)
	move	$4,$2
	lui	$2,%hi(free_netdev)
	addiu	$2,$2,%lo(free_netdev)
	jalr	$2
	nop

	lui	$2,%hi(gpWanPriv)
	lw	$3,%lo(gpWanPriv)($2)
	lw	$2,24($sp)
	sll	$2,$2,2
	addu	$2,$3,$2
	sw	$0,0($2)
	move	$2,$0
$L281:
	lw	$31,20($sp)
	addiu	$sp,$sp,24
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	pwan_delete_net_interface
	.size	pwan_delete_net_interface, .-pwan_delete_net_interface
	.section	.text.getVlanType,"ax",@progbits
	.align	2
	.globl	getVlanType
	.set	nomips16
	.set	nomicromips
	.ent	getVlanType
	.type	getVlanType, @function
getVlanType:
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	move	$2,$4
	sh	$2,0($sp)
	lhu	$2,0($sp)
	li	$3,34984			# 0x88a8
	beq	$2,$3,$L285
	nop

	li	$3,34985			# 0x88a9
	slt	$3,$2,$3
	beq	$3,$0,$L286
	nop

	li	$3,33024			# 0x8100
	beq	$2,$3,$L287
	nop

	j	$L284
	nop

$L286:
	li	$3,37120			# 0x9100
	beq	$2,$3,$L288
	nop

	li	$3,43707			# 0xaabb
	beq	$2,$3,$L289
	nop

	j	$L284
	nop

$L287:
	li	$2,33024			# 0x8100
	j	$L290
	nop

$L285:
	li	$2,33024			# 0x8100
	j	$L290
	nop

$L288:
	li	$2,33024			# 0x8100
	j	$L290
	nop

$L289:
	li	$2,33024			# 0x8100
	j	$L290
	nop

$L284:
	li	$2,-1			# 0xffffffffffffffff
$L290:
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	getVlanType
	.size	getVlanType, .-getVlanType
	.section	.text.fb_pwan_rx_vlan_proc,"ax",@progbits
	.align	2
	.globl	fb_pwan_rx_vlan_proc
	.set	nomips16
	.set	nomicromips
	.ent	fb_pwan_rx_vlan_proc
	.type	fb_pwan_rx_vlan_proc, @function
fb_pwan_rx_vlan_proc:
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
	.end	fb_pwan_rx_vlan_proc
	.size	fb_pwan_rx_vlan_proc, .-fb_pwan_rx_vlan_proc
	.section	.text.fb_pwan_tx_vlan_proc,"ax",@progbits
	.align	2
	.globl	fb_pwan_tx_vlan_proc
	.set	nomips16
	.set	nomicromips
	.ent	fb_pwan_tx_vlan_proc
	.type	fb_pwan_tx_vlan_proc, @function
fb_pwan_tx_vlan_proc:
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
	.end	fb_pwan_tx_vlan_proc
	.size	fb_pwan_tx_vlan_proc, .-fb_pwan_tx_vlan_proc
	.rdata
	.align	2
$LC40:
	.ascii	"[%lu0ms]RX PKT LEN: %d, Rx Msg: %.8x, %.8x\000"
	.align	2
$LC41:
	.ascii	"[%lu0ms]pwan_cb_rx_packet: netIdx = %d line = %d\012\000"
	.align	2
$LC42:
	.ascii	"\012FAILED: wan2lan skb2 allocation in pon rx direction."
	.ascii	"\012\000"
	.align	2
$LC1:
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
	.section	.text.pwan_cb_rx_packet,"ax",@progbits
	.align	2
	.globl	pwan_cb_rx_packet
	.set	nomips16
	.set	nomicromips
	.ent	pwan_cb_rx_packet
	.type	pwan_cb_rx_packet, @function
pwan_cb_rx_packet:
	.frame	$sp,128,$31		# vars= 96, regs= 1/0, args= 24, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-128
	sw	$31,124($sp)
	sw	$4,128($sp)
	sw	$5,132($sp)
	sw	$6,136($sp)
	sw	$7,140($sp)
	li	$2,-1			# 0xffffffffffffffff
	sb	$2,24($sp)
	sw	$0,44($sp)
	lw	$2,128($sp)
	sw	$2,48($sp)
	sw	$0,52($sp)
	sb	$0,84($sp)
	sw	$0,28($sp)
	sb	$0,56($sp)
	addiu	$2,$sp,88
	sw	$2,60($sp)
	lw	$4,60($sp)
	lw	$5,48($sp)
	li	$6,16			# 0x10
	lui	$2,%hi(memcpy)
	addiu	$2,$2,%lo(memcpy)
	jalr	$2
	nop

	sw	$0,32($sp)
	lw	$2,60($sp)
	lw	$2,0($2)
	andi	$2,$2,0x800
	beq	$2,$0,$L296
	nop

	lw	$2,140($sp)
	lw	$4,136($sp)
	move	$5,$2
	lui	$2,%hi(replace_oam_pkt)
	addiu	$2,$2,%lo(replace_oam_pkt)
	jalr	$2
	nop

	sw	$2,136($sp)
$L296:
	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	lhu	$2,-30816($2)
	andi	$2,$2,0x80
	beq	$2,$0,$L297
	nop

	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	lhu	$2,-30816($2)
	andi	$2,$2,0x80
	beq	$2,$0,$L298
	nop

	lui	$2,%hi(jiffies)
	lw	$5,%lo(jiffies)($2)
	lw	$2,60($sp)
	lw	$3,0($2)
	lw	$2,60($sp)
	lw	$2,4($2)
	sw	$2,16($sp)
	lui	$2,%hi($LC40)
	addiu	$4,$2,%lo($LC40)
	lw	$6,140($sp)
	move	$7,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L298:
	lw	$4,136($sp)
	lw	$5,140($sp)
	lui	$2,%hi(__dump_skb)
	addiu	$2,$2,%lo(__dump_skb)
	jalr	$2
	nop

$L297:
	lui	$2,%hi(gpPonSysData)
	lw	$2,%lo(gpPonSysData)($2)
	lw	$3,0($2)
	li	$2,1			# 0x1
	bne	$3,$2,$L299
	nop

	addiu	$2,$sp,84
	lw	$4,60($sp)
	lw	$5,136($sp)
	lw	$6,140($sp)
	move	$7,$2
	lui	$2,%hi(gwan_process_rx_message)
	addiu	$2,$2,%lo(gwan_process_rx_message)
	jalr	$2
	nop

	sb	$2,24($sp)
	lb	$2,24($sp)
	bltz	$2,$L300
	nop

	lb	$3,24($sp)
	li	$2,1			# 0x1
	beq	$3,$2,$L301
	nop

	lbu	$2,24($sp)
	andi	$3,$2,0x7
	li	$2,2			# 0x2
	beq	$3,$2,$L301
	nop

$L300:
	lb	$2,24($sp)
	addiu	$2,$2,3
	bgez	$2,$L302
	nop

	lb	$2,24($sp)
	addiu	$2,$2,3
	sw	$2,28($sp)
	j	$L303
	nop

$L302:
	j	$L303
	nop

$L301:
	li	$2,29306			# 0x727a
	sw	$2,32($sp)
	j	$L304
	nop

$L299:
	lui	$2,%hi(gpPonSysData)
	lw	$2,%lo(gpPonSysData)($2)
	lw	$3,0($2)
	li	$2,2			# 0x2
	bne	$3,$2,$L305
	nop

	addiu	$2,$sp,84
	lw	$4,60($sp)
	lw	$5,136($sp)
	lw	$6,140($sp)
	move	$7,$2
	lui	$2,%hi(ewan_process_rx_message)
	addiu	$2,$2,%lo(ewan_process_rx_message)
	jalr	$2
	nop

	sb	$2,24($sp)
	lb	$2,24($sp)
	bltz	$2,$L303
	nop

	lb	$2,24($sp)
	beq	$2,$0,$L306
	nop

	lb	$3,24($sp)
	li	$2,2			# 0x2
	bne	$3,$2,$L303
	nop

$L306:
	li	$2,29305			# 0x7279
	sw	$2,32($sp)
	j	$L304
	nop

$L305:
	j	$L303
	nop

$L304:
	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	lhu	$2,-30816($2)
	andi	$2,$2,0x40
	beq	$2,$0,$L307
	nop

	lui	$2,%hi(jiffies)
	lw	$5,%lo(jiffies)($2)
	lb	$3,24($sp)
	lui	$2,%hi($LC41)
	addiu	$4,$2,%lo($LC41)
	move	$6,$3
	li	$7,2449			# 0x991
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L307:
	lb	$2,24($sp)
	slt	$2,$2,17
	bne	$2,$0,$L308
	nop

	lb	$2,24($sp)
	addiu	$2,$2,-2
	sw	$2,28($sp)
	li	$2,2			# 0x2
	sb	$2,24($sp)
$L308:
	lui	$2,%hi(gpWanPriv)
	lw	$3,%lo(gpWanPriv)($2)
	lb	$2,24($sp)
	sll	$2,$2,2
	addu	$2,$3,$2
	lw	$2,0($2)
	sw	$2,64($sp)
	lw	$2,64($sp)
	sw	$2,68($sp)
	lw	$2,68($sp)
	addiu	$2,$2,928
	sw	$2,44($sp)
	lbu	$2,84($sp)
	beq	$2,$0,$L310
	nop

	lw	$4,136($sp)
	lw	$5,140($sp)
	lui	$2,%hi(skb_put)
	addiu	$2,$2,%lo(skb_put)
	jalr	$2
	nop

	lw	$4,136($sp)
	lw	$5,64($sp)
	lui	$2,%hi(pwan_net_start_xmit)
	addiu	$2,$2,%lo(pwan_net_start_xmit)
	jalr	$2
	nop

	j	$L311
	nop

$L310:
	lw	$4,136($sp)
	lw	$5,140($sp)
	lui	$2,%hi(skb_put)
	addiu	$2,$2,%lo(skb_put)
	jalr	$2
	nop

	lui	$2,%hi(masko_on_off)
	lw	$2,%lo(masko_on_off)($2)
	beq	$2,$0,$L312
	nop

	sw	$0,36($sp)
	li	$2,6			# 0x6
	sw	$2,40($sp)
	lui	$2,%hi($LC1)
	addiu	$3,$2,%lo($LC1)
	lwl	$4,%lo($LC1)($2)
	move	$5,$4
	lwr	$5,3($3)
	addiu	$3,$2,%lo($LC1)
	lwl	$4,4($3)
	lwr	$4,7($3)
	addiu	$3,$2,%lo($LC1)
	lwl	$6,8($3)
	move	$7,$6
	lwr	$7,11($3)
	move	$3,$7
	sw	$5,104($sp)
	sw	$4,108($sp)
	sw	$3,112($sp)
	addiu	$2,$2,%lo($LC1)
	lbu	$3,12($2)
	sb	$3,116($sp)
	lbu	$2,13($2)
	sb	$2,117($sp)
	lb	$3,24($sp)
	li	$2,1			# 0x1
	bne	$3,$2,$L313
	nop

	lw	$2,40($sp)
	addiu	$2,$2,14
	sw	$2,40($sp)
$L313:
	lw	$2,136($sp)
	sw	$2,72($sp)
	lw	$2,72($sp)
	lw	$2,256($2)
	move	$3,$2
	lw	$2,72($sp)
	lw	$2,252($2)
	subu	$2,$3,$2
	move	$3,$2
	lw	$2,40($sp)
	sltu	$2,$3,$2
	beq	$2,$0,$L315
	nop

	lw	$2,136($sp)
	sw	$2,76($sp)
	lw	$2,76($sp)
	sw	$2,80($sp)
	lw	$2,80($sp)
	lw	$2,84($2)
	bne	$2,$0,$L317
	nop

	lw	$2,76($sp)
	lw	$2,248($2)
	move	$3,$2
	lw	$2,76($sp)
	lw	$2,244($2)
	subu	$2,$3,$2
	j	$L319
	nop

$L317:
	move	$2,$0
$L319:
	lw	$4,136($sp)
	lw	$5,40($sp)
	move	$6,$2
	li	$7,32			# 0x20
	lui	$2,%hi(skb_copy_expand)
	addiu	$2,$2,%lo(skb_copy_expand)
	jalr	$2
	nop

	sw	$2,36($sp)
	j	$L320
	nop

$L315:
	lw	$4,136($sp)
	li	$5,32			# 0x20
	lui	$2,%hi(skb_copy)
	addiu	$2,$2,%lo(skb_copy)
	jalr	$2
	nop

	sw	$2,36($sp)
$L320:
	lw	$2,36($sp)
	bne	$2,$0,$L321
	nop

	lui	$2,%hi($LC42)
	addiu	$4,$2,%lo($LC42)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	j	$L312
	nop

$L321:
	lb	$2,24($sp)
	bne	$2,$0,$L323
	nop

	lw	$4,36($sp)
	li	$5,2			# 0x2
	lui	$2,%hi(skb_pull)
	addiu	$2,$2,%lo(skb_pull)
	jalr	$2
	nop

	j	$L324
	nop

$L323:
	lb	$3,24($sp)
	li	$2,1			# 0x1
	bne	$3,$2,$L324
	nop

	lw	$4,36($sp)
	li	$5,14			# 0xe
	lui	$2,%hi(skb_push)
	addiu	$2,$2,%lo(skb_push)
	jalr	$2
	nop

	lw	$2,36($sp)
	lw	$3,256($2)
	addiu	$2,$sp,104
	move	$4,$3
	move	$5,$2
	li	$6,14			# 0xe
	lui	$2,%hi(memcpy)
	addiu	$2,$2,%lo(memcpy)
	jalr	$2
	nop

$L324:
	lw	$2,36($sp)
	lw	$3,148($2)
	li	$2,67108864			# 0x4000000
	or	$3,$3,$2
	lw	$2,36($sp)
	sw	$3,148($2)
	li	$4,-2147483648			# 0xffffffff80000000
	lw	$5,36($sp)
	lui	$2,%hi(macSend)
	addiu	$2,$2,%lo(macSend)
	jalr	$2
	nop

$L312:
	lw	$2,44($sp)
	lbu	$3,0($2)
	li	$2,2			# 0x2
	bne	$3,$2,$L325
	nop

	lui	$2,%hi(internet_led_on)
	lw	$2,%lo(internet_led_on)($2)
	beq	$2,$0,$L326
	nop

	li	$4,34			# 0x22
	lui	$2,%hi(ledTurnOn)
	addiu	$2,$2,%lo(ledTurnOn)
	jalr	$2
	nop

	j	$L325
	nop

$L326:
	lui	$2,%hi(internet_trying_led_on)
	lw	$2,%lo(internet_trying_led_on)($2)
	bne	$2,$0,$L325
	nop

	li	$4,33			# 0x21
	lui	$2,%hi(ledTurnOff)
	addiu	$2,$2,%lo(ledTurnOff)
	jalr	$2
	nop

	li	$4,38			# 0x26
	lui	$2,%hi(ledTurnOff)
	addiu	$2,$2,%lo(ledTurnOff)
	jalr	$2
	nop

$L325:
	lw	$2,136($sp)
	lw	$3,64($sp)
	sw	$3,20($2)
	lui	$2,%hi(jiffies)
	lw	$3,%lo(jiffies)($2)
	lw	$2,64($sp)
	sw	$3,360($2)
	lw	$4,136($sp)
	lw	$5,64($sp)
	lui	$2,%hi(eth_type_trans)
	addiu	$2,$2,%lo(eth_type_trans)
	jalr	$2
	nop

	move	$3,$2
	lw	$2,136($sp)
	sh	$3,102($2)
	lw	$3,136($sp)
	lhu	$2,100($3)
	li	$4,1			# 0x1
	ins	$2,$4,12,2
	sh	$2,100($3)
	lb	$3,24($sp)
	li	$2,1			# 0x1
	bne	$3,$2,$L327
	nop

	lw	$2,136($sp)
	sh	$0,102($2)
$L327:
	lui	$2,%hi(ra_sw_nat_hook_rxinfo)
	lw	$2,%lo(ra_sw_nat_hook_rxinfo)($2)
	beq	$2,$0,$L328
	nop

	lw	$2,32($sp)
	beq	$2,$0,$L328
	nop

	lui	$2,%hi(ra_sw_nat_hook_rxinfo)
	lw	$2,%lo(ra_sw_nat_hook_rxinfo)($2)
	lw	$3,60($sp)
	addiu	$3,$3,4
	lw	$4,136($sp)
	lw	$5,32($sp)
	move	$6,$3
	li	$7,4			# 0x4
	jalr	$2
	nop

$L328:
	lui	$2,%hi(ra_sw_nat_hook_rx)
	lw	$2,%lo(ra_sw_nat_hook_rx)($2)
	beq	$2,$0,$L329
	nop

	lui	$2,%hi(ra_sw_nat_hook_rx)
	lw	$2,%lo(ra_sw_nat_hook_rx)($2)
	lw	$4,136($sp)
	jalr	$2
	nop

	beq	$2,$0,$L311
	nop

	lw	$4,136($sp)
	lui	$2,%hi(netif_receive_skb)
	addiu	$2,$2,%lo(netif_receive_skb)
	jalr	$2
	nop

	j	$L311
	nop

$L329:
	lw	$4,136($sp)
	lui	$2,%hi(netif_receive_skb)
	addiu	$2,$2,%lo(netif_receive_skb)
	jalr	$2
	nop

$L311:
	lw	$2,44($sp)
	lw	$2,48($2)
	addiu	$3,$2,1
	lw	$2,44($sp)
	sw	$3,48($2)
	lw	$2,44($sp)
	lw	$3,56($2)
	lw	$2,140($sp)
	addu	$3,$3,$2
	lw	$2,44($sp)
	sw	$3,56($2)
	lw	$2,28($sp)
	j	$L332
	nop

$L303:
	lw	$4,136($sp)
	lui	$2,%hi(consume_skb)
	addiu	$2,$2,%lo(consume_skb)
	jalr	$2
	nop

	lui	$2,%hi(gpWanPriv)
	lw	$2,%lo(gpWanPriv)($2)
	lw	$3,12($2)
	addiu	$3,$3,1
	sw	$3,12($2)
	lw	$2,28($sp)
	addiu	$2,$2,-14
$L332:
	lw	$31,124($sp)
	addiu	$sp,$sp,128
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	pwan_cb_rx_packet
	.size	pwan_cb_rx_packet, .-pwan_cb_rx_packet
	.section	.text.pwan_cb_event_handler,"ax",@progbits
	.align	2
	.globl	pwan_cb_event_handler
	.set	nomips16
	.set	nomicromips
	.ent	pwan_cb_event_handler
	.type	pwan_cb_event_handler, @function
pwan_cb_event_handler:
	.frame	$sp,120,$31		# vars= 96, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	addiu	$sp,$sp,-120
	sw	$31,116($sp)
	sw	$4,120($sp)
	lw	$2,120($sp)
	li	$3,1			# 0x1
	beq	$2,$3,$L353
	sltu	$3,$2,1
	bne	$3,$0,$L336
	li	$3,2			# 0x2
	beq	$2,$3,$L354
	j	$L352
$L336:
	lui	$2,%hi(gpWanPriv)
	lw	$2,%lo(gpWanPriv)($2)
	lw	$2,8($2)
	sw	$2,16($sp)
	lw	$2,16($sp)
	sw	$2,24($sp)
	lw	$2,24($sp)
	addiu	$2,$2,928
	sw	$2,20($sp)
	lw	$2,20($sp)
	bne	$2,$0,$L339
	li	$2,1			# 0x1
	j	$L340
$L339:
	lw	$2,20($sp)
	addiu	$2,$2,4
	sw	$2,28($sp)
	lw	$2,28($sp)
	sw	$2,32($sp)
	lw	$2,32($sp)
	addiu	$2,$2,8
	li	$3,1			# 0x1
	sw	$3,36($sp)
	sw	$2,40($sp)
	lw	$2,36($sp)
	bgez	$2,$L341
	addiu	$2,$2,31
$L341:
	sra	$2,$2,5
	sll	$2,$2,2
	lw	$3,40($sp)
	addu	$2,$3,$2
	lw	$3,0($2)
	lw	$2,36($sp)
	andi	$2,$2,0x1f
	srl	$2,$3,$2
	andi	$2,$2,0x1
	bne	$2,$0,$L344
	lw	$2,28($sp)
	addiu	$2,$2,8
	sw	$0,44($sp)
	sw	$2,48($sp)
	lw	$2,44($sp)
	andi	$2,$2,0xffff
	andi	$2,$2,0x1f
	sh	$2,52($sp)
#APP
 # 261 "/opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/bitops.h" 1
			

 # 0 "" 2
#NO_APP
	lw	$2,44($sp)
	srl	$2,$2,5
	sll	$2,$2,2
	lw	$3,48($sp)
	addu	$2,$3,$2
	sw	$2,56($sp)
	lhu	$2,52($sp)
	li	$3,1			# 0x1
	sll	$2,$3,$2
	lw	$5,56($sp)
	lw	$3,56($sp)
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
	sw	$4,60($sp)
	sw	$3,64($sp)
#APP
 # 312 "/opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/bitops.h" 1
			

 # 0 "" 2
#NO_APP
	lw	$2,64($sp)
	sltu	$2,$0,$2
	andi	$2,$2,0x00ff
	bne	$2,$0,$L344
	li	$2,1			# 0x1
	j	$L347
$L344:
	move	$2,$0
$L347:
	beq	$2,$0,$L348
	li	$2,16			# 0x10
	sw	$2,68($sp)
	sw	$0,72($sp)
	li	$2,8			# 0x8
	sw	$2,76($sp)
	addiu	$2,$sp,76
	lw	$4,68($sp)
	move	$5,$0
	move	$6,$2
	lui	$2,%hi(__ECNT_HOOK)
	addiu	$2,$2,%lo(__ECNT_HOOK)
	jalr	$2
	sw	$2,72($sp)
	lw	$2,20($sp)
	addiu	$2,$2,4
	move	$4,$2
	lui	$2,%hi(__napi_schedule)
	addiu	$2,$2,%lo(__napi_schedule)
	jalr	$2
	j	$L352
$L348:
	j	$L352
$L353:
	.set	noreorder
	nop
	.set	reorder
	j	$L352
$L354:
	.set	noreorder
	nop
	.set	reorder
$L352:
	move	$2,$0
$L340:
	lw	$31,116($sp)
	addiu	$sp,$sp,120
	j	$31
	.end	pwan_cb_event_handler
	.size	pwan_cb_event_handler, .-pwan_cb_event_handler
	.section	.text.pon_qos_stop_read_proc,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	pon_qos_stop_read_proc
	.type	pon_qos_stop_read_proc, @function
pon_qos_stop_read_proc:
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
	lui	$2,%hi(qos_stop_enable)
	lbu	$2,%lo(qos_stop_enable)($2)
	move	$3,$2
	lui	$2,%hi($LC23)
	addiu	$4,$2,%lo($LC23)
	move	$5,$3
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
	.end	pon_qos_stop_read_proc
	.size	pon_qos_stop_read_proc, .-pon_qos_stop_read_proc
	.rdata
	.align	2
$LC43:
	.ascii	"%d\000"
	.section	.text.pon_qos_stop_write_proc,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	pon_qos_stop_write_proc
	.type	pon_qos_stop_write_proc, @function
pon_qos_stop_write_proc:
	.frame	$sp,64,$31		# vars= 40, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	addiu	$sp,$sp,-64
	sw	$31,60($sp)
	sw	$4,64($sp)
	sw	$5,68($sp)
	sw	$6,72($sp)
	sw	$7,76($sp)
	lw	$2,72($sp)
	sltu	$2,$2,9
	bne	$2,$0,$L358
	li	$2,8			# 0x8
	sw	$2,16($sp)
	j	$L359
$L358:
	lw	$2,72($sp)
	sw	$2,16($sp)
$L359:
	addiu	$2,$sp,48
	move	$4,$2
	move	$5,$0
	li	$6,8			# 0x8
	lui	$2,%hi(memset)
	addiu	$2,$2,%lo(memset)
	jalr	$2
	addiu	$2,$sp,48
	sw	$2,24($sp)
	lw	$2,68($sp)
	sw	$2,28($sp)
	lw	$2,16($sp)
	addiu	$2,$2,-1
	sw	$2,20($sp)
	lw	$2,28($sp)
	sw	$2,32($sp)
	lw	$2,20($sp)
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
	beq	$2,$0,$L360
	lw	$4,24($sp)
	lw	$5,28($sp)
	lw	$6,20($sp)
#APP
 # 2998 "/opt/tclinux_phoenix/modules/private/xpon/src/pwan/xpon_netif.c" 1
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
	sw	$2,20($sp)
$L360:
	lw	$2,20($sp)
	beq	$2,$0,$L361
	li	$2,-14			# 0xfffffffffffffff2
	j	$L363
$L361:
	lw	$2,16($sp)
	addiu	$3,$sp,16
	addu	$2,$3,$2
	sb	$0,32($2)
	addiu	$2,$sp,48
	move	$4,$2
	lui	$2,%hi($LC43)
	addiu	$5,$2,%lo($LC43)
	lui	$2,%hi(qos_stop_enable)
	addiu	$6,$2,%lo(qos_stop_enable)
	lui	$2,%hi(sscanf)
	addiu	$2,$2,%lo(sscanf)
	jalr	$2
	lw	$2,16($sp)
$L363:
	lw	$31,60($sp)
	addiu	$sp,$sp,64
	j	$31
	.end	pon_qos_stop_write_proc
	.size	pon_qos_stop_write_proc, .-pon_qos_stop_write_proc
	.section	.text.pon_qos_stop_num_read_proc,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	pon_qos_stop_num_read_proc
	.type	pon_qos_stop_num_read_proc, @function
pon_qos_stop_num_read_proc:
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
	lui	$2,%hi(qos_stop_num)
	lw	$3,%lo(qos_stop_num)($2)
	lui	$2,%hi($LC23)
	addiu	$4,$2,%lo($LC23)
	move	$5,$3
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
	.end	pon_qos_stop_num_read_proc
	.size	pon_qos_stop_num_read_proc, .-pon_qos_stop_num_read_proc
	.section	.text.pon_qos_stop_num_write_proc,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	pon_qos_stop_num_write_proc
	.type	pon_qos_stop_num_write_proc, @function
pon_qos_stop_num_write_proc:
	.frame	$sp,64,$31		# vars= 40, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	addiu	$sp,$sp,-64
	sw	$31,60($sp)
	sw	$4,64($sp)
	sw	$5,68($sp)
	sw	$6,72($sp)
	sw	$7,76($sp)
	lw	$2,72($sp)
	sltu	$2,$2,9
	bne	$2,$0,$L367
	li	$2,8			# 0x8
	sw	$2,16($sp)
	j	$L368
$L367:
	lw	$2,72($sp)
	sw	$2,16($sp)
$L368:
	addiu	$2,$sp,48
	move	$4,$2
	move	$5,$0
	li	$6,8			# 0x8
	lui	$2,%hi(memset)
	addiu	$2,$2,%lo(memset)
	jalr	$2
	addiu	$2,$sp,48
	sw	$2,24($sp)
	lw	$2,68($sp)
	sw	$2,28($sp)
	lw	$2,16($sp)
	addiu	$2,$2,-1
	sw	$2,20($sp)
	lw	$2,28($sp)
	sw	$2,32($sp)
	lw	$2,20($sp)
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
	beq	$2,$0,$L369
	lw	$4,24($sp)
	lw	$5,28($sp)
	lw	$6,20($sp)
#APP
 # 3031 "/opt/tclinux_phoenix/modules/private/xpon/src/pwan/xpon_netif.c" 1
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
	sw	$2,20($sp)
$L369:
	lw	$2,20($sp)
	beq	$2,$0,$L370
	li	$2,-14			# 0xfffffffffffffff2
	j	$L372
$L370:
	lw	$2,16($sp)
	addiu	$3,$sp,16
	addu	$2,$3,$2
	sb	$0,32($2)
	addiu	$2,$sp,48
	move	$4,$2
	lui	$2,%hi($LC43)
	addiu	$5,$2,%lo($LC43)
	lui	$2,%hi(qos_stop_num)
	addiu	$6,$2,%lo(qos_stop_num)
	lui	$2,%hi(sscanf)
	addiu	$2,$2,%lo(sscanf)
	jalr	$2
	lw	$2,16($sp)
$L372:
	lw	$31,60($sp)
	addiu	$sp,$sp,64
	j	$31
	.end	pon_qos_stop_num_write_proc
	.size	pon_qos_stop_num_write_proc, .-pon_qos_stop_num_write_proc
	.rdata
	.align	2
$LC44:
	.ascii	"[%lu0ms]PWAN: Data drop timer for OMCI done, clean HWNAT"
	.ascii	"!\012\000"
	.section	.text.pwan_tx_drop_timer,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	pwan_tx_drop_timer
	.type	pwan_tx_drop_timer, @function
pwan_tx_drop_timer:
	.frame	$sp,32,$31		# vars= 8, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-32
	sw	$31,28($sp)
	sw	$4,32($sp)
	lui	$2,%hi(gpWanPriv)
	lw	$3,%lo(gpWanPriv)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	lw	$2,-31788($2)
	ext	$2,$2,13,1
	andi	$2,$2,0x00ff
	sw	$2,16($sp)
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lw	$3,4($2)
	li	$2,5			# 0x5
	bne	$3,$2,$L373
	nop

	lw	$3,16($sp)
	li	$2,1			# 0x1
	bne	$3,$2,$L373
	nop

	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	lhu	$2,-30816($2)
	andi	$2,$2,0x8
	beq	$2,$0,$L375
	nop

	lui	$2,%hi(jiffies)
	lw	$3,%lo(jiffies)($2)
	lui	$2,%hi($LC44)
	addiu	$4,$2,%lo($LC44)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L375:
	lui	$2,%hi(gpWanPriv)
	lw	$3,%lo(gpWanPriv)($2)
	li	$2,65536			# 0x10000
	addu	$3,$3,$2
	lbu	$2,-31786($3)
	ins	$2,$0,5,1
	sb	$2,-31786($3)
	lui	$2,%hi(gpGponPriv)
	lw	$2,%lo(gpGponPriv)($2)
	lw	$3,448($2)
	li	$2,33554432			# 0x2000000
	and	$2,$3,$2
	sltu	$2,$2,1
	andi	$2,$2,0x00ff
	beq	$2,$0,$L376
	nop

	lui	$2,%hi(is_hwnat_dont_clean)
	sw	$0,%lo(is_hwnat_dont_clean)($2)
$L376:
	lui	$2,%hi(ra_sw_nat_hook_clean_table)
	lw	$2,%lo(ra_sw_nat_hook_clean_table)($2)
	beq	$2,$0,$L377
	nop

	lui	$2,%hi(ra_sw_nat_hook_clean_table)
	lw	$2,%lo(ra_sw_nat_hook_clean_table)($2)
	jalr	$2
	nop

$L377:
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

$L373:
	lw	$31,28($sp)
	addiu	$sp,$sp,32
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	pwan_tx_drop_timer
	.size	pwan_tx_drop_timer, .-pwan_tx_drop_timer
	.rdata
	.align	2
$LC45:
	.ascii	"qos/pon_hw_qos_enable\000"
	.align	2
$LC46:
	.ascii	"qos\000"
	.align	2
$LC47:
	.ascii	"tc3162/pon_trafficShapingSet\000"
	.align	2
$LC48:
	.ascii	"tc3162/pon_qoswrr\000"
	.align	2
$LC49:
	.ascii	"tc3162/pon_tcqos_disc\000"
	.align	2
$LC50:
	.ascii	"tc3162/qos_stop_enable\000"
	.align	2
$LC51:
	.ascii	"tc3162/qos_stop_num\000"
	.align	2
$LC52:
	.ascii	"tc3162/service_change\000"
	.section	.text.pwan_destroy,"ax",@progbits
	.align	2
	.globl	pwan_destroy
	.set	nomips16
	.set	nomicromips
	.ent	pwan_destroy
	.type	pwan_destroy, @function
pwan_destroy:
	.frame	$sp,32,$31		# vars= 8, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-32
	sw	$31,28($sp)
	sw	$0,16($sp)
	j	$L379
	nop

$L380:
	lw	$2,16($sp)
	move	$4,$2
	lui	$2,%hi(pwan_delete_net_interface)
	addiu	$2,$2,%lo(pwan_delete_net_interface)
	jalr	$2
	nop

	lw	$2,16($sp)
	addiu	$2,$2,1
	sw	$2,16($sp)
$L379:
	lw	$2,16($sp)
	slt	$2,$2,3
	bne	$2,$0,$L380
	nop

	lui	$2,%hi($LC45)
	addiu	$4,$2,%lo($LC45)
	move	$5,$0
	lui	$2,%hi(remove_proc_entry)
	addiu	$2,$2,%lo(remove_proc_entry)
	jalr	$2
	nop

	lui	$2,%hi($LC46)
	addiu	$4,$2,%lo($LC46)
	move	$5,$0
	lui	$2,%hi(remove_proc_entry)
	addiu	$2,$2,%lo(remove_proc_entry)
	jalr	$2
	nop

	lui	$2,%hi($LC47)
	addiu	$4,$2,%lo($LC47)
	move	$5,$0
	lui	$2,%hi(remove_proc_entry)
	addiu	$2,$2,%lo(remove_proc_entry)
	jalr	$2
	nop

	lui	$2,%hi($LC48)
	addiu	$4,$2,%lo($LC48)
	move	$5,$0
	lui	$2,%hi(remove_proc_entry)
	addiu	$2,$2,%lo(remove_proc_entry)
	jalr	$2
	nop

	lui	$2,%hi($LC49)
	addiu	$4,$2,%lo($LC49)
	move	$5,$0
	lui	$2,%hi(remove_proc_entry)
	addiu	$2,$2,%lo(remove_proc_entry)
	jalr	$2
	nop

	lui	$2,%hi($LC50)
	addiu	$4,$2,%lo($LC50)
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

	lui	$2,%hi($LC52)
	addiu	$4,$2,%lo($LC52)
	move	$5,$0
	lui	$2,%hi(remove_proc_entry)
	addiu	$2,$2,%lo(remove_proc_entry)
	jalr	$2
	nop

	lw	$31,28($sp)
	addiu	$sp,$sp,32
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	pwan_destroy
	.size	pwan_destroy, .-pwan_destroy
	.rdata
	.align	2
$LC53:
	.ascii	"[%lu0ms]GPON WAN initialization failed\012\000"
	.align	2
$LC54:
	.ascii	"[%lu0ms]EPON WAN initialization failed\012\000"
	.align	2
$LC55:
	.ascii	"\012FAILED: create proc for pon_hw_qos_enable\012\000"
	.align	2
$LC56:
	.ascii	"\012FAILED: create proc for pon_trafficShapingSet\012\000"
	.align	2
$LC57:
	.ascii	"\012FAILED: create proc for pon_qoswrr\012\000"
	.align	2
$LC58:
	.ascii	"\012FAILED: create proc for pon_tcqos_disc\012\000"
	.align	2
$LC59:
	.ascii	"\012FAILED: create proc for qos_stop_enable\012\000"
	.align	2
$LC60:
	.ascii	"\012FAILED: create proc for qos_stop_num\012\000"
	.align	2
$LC61:
	.ascii	"[%lu0ms]PON WAN interface initialization done\012\000"
	.section	.text.pwan_init,"ax",@progbits
	.align	2
	.globl	pwan_init
	.set	nomips16
	.set	nomicromips
	.ent	pwan_init
	.type	pwan_init, @function
pwan_init:
	.frame	$sp,40,$31		# vars= 16, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-40
	sw	$31,36($sp)
	lui	$2,%hi($LC46)
	addiu	$4,$2,%lo($LC46)
	move	$5,$0
	lui	$2,%hi(proc_mkdir)
	addiu	$2,$2,%lo(proc_mkdir)
	jalr	$2
	nop

	sw	$0,16($sp)
	j	$L382
	nop

$L383:
	lui	$2,%hi(gpWanPriv)
	lw	$3,%lo(gpWanPriv)($2)
	lw	$2,16($sp)
	sll	$2,$2,2
	addu	$2,$3,$2
	sw	$0,0($2)
	lw	$2,16($sp)
	addiu	$2,$2,1
	sw	$2,16($sp)
$L382:
	lw	$2,16($sp)
	slt	$2,$2,3
	bne	$2,$0,$L383
	nop

	lui	$2,%hi(gpWanPriv)
	lw	$2,%lo(gpWanPriv)($2)
	sw	$0,12($2)
	lui	$2,%hi(gpWanPriv)
	lw	$2,%lo(gpWanPriv)($2)
	sw	$0,16($2)
	lui	$2,%hi(gpWanPriv)
	lw	$2,%lo(gpWanPriv)($2)
	li	$3,1			# 0x1
	sw	$3,20($2)
	lui	$2,%hi(gpWanPriv)
	lw	$2,%lo(gpWanPriv)($2)
	li	$3,15			# 0xf
	sb	$3,24($2)
	lui	$2,%hi(gpWanPriv)
	lw	$2,%lo(gpWanPriv)($2)
	addiu	$2,$2,28
	move	$4,$2
	lui	$2,%hi(gwan_init)
	addiu	$2,$2,%lo(gwan_init)
	jalr	$2
	nop

	sw	$2,20($sp)
	lw	$2,20($sp)
	beq	$2,$0,$L384
	nop

	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	lhu	$2,-30816($2)
	andi	$2,$2,0x1
	beq	$2,$0,$L385
	nop

	lui	$2,%hi(jiffies)
	lw	$3,%lo(jiffies)($2)
	lui	$2,%hi($LC53)
	addiu	$4,$2,%lo($LC53)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L385:
	lw	$2,20($sp)
	j	$L386
	nop

$L384:
	lui	$2,%hi(gpWanPriv)
	lw	$3,%lo(gpWanPriv)($2)
	li	$2,32892			# 0x807c
	addu	$2,$3,$2
	move	$4,$2
	lui	$2,%hi(ewan_init)
	addiu	$2,$2,%lo(ewan_init)
	jalr	$2
	nop

	sw	$2,20($sp)
	lw	$2,20($sp)
	beq	$2,$0,$L387
	nop

	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	lhu	$2,-30816($2)
	andi	$2,$2,0x1
	beq	$2,$0,$L388
	nop

	lui	$2,%hi(jiffies)
	lw	$3,%lo(jiffies)($2)
	lui	$2,%hi($LC54)
	addiu	$4,$2,%lo($LC54)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L388:
	lw	$2,20($sp)
	j	$L386
	nop

$L387:
	li	$4,2			# 0x2
	lui	$2,%hi(pwan_create_net_interface)
	addiu	$2,$2,%lo(pwan_create_net_interface)
	jalr	$2
	nop

	lui	$2,%hi($LC45)
	addiu	$4,$2,%lo($LC45)
	move	$5,$0
	move	$6,$0
	lui	$2,%hi(create_proc_entry)
	addiu	$2,$2,%lo(create_proc_entry)
	jalr	$2
	nop

	sw	$2,24($sp)
	lw	$2,24($sp)
	bne	$2,$0,$L389
	nop

	lui	$2,%hi($LC55)
	addiu	$4,$2,%lo($LC55)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	li	$2,-12			# 0xfffffffffffffff4
	j	$L386
	nop

$L389:
	lw	$2,24($sp)
	lui	$3,%hi(pon_hw_qos_enable_write_proc)
	addiu	$3,$3,%lo(pon_hw_qos_enable_write_proc)
	sw	$3,68($2)
	lw	$2,24($sp)
	lui	$3,%hi(pon_hw_qos_enable_read_proc)
	addiu	$3,$3,%lo(pon_hw_qos_enable_read_proc)
	sw	$3,64($2)
	lui	$2,%hi($LC47)
	addiu	$4,$2,%lo($LC47)
	move	$5,$0
	move	$6,$0
	lui	$2,%hi(create_proc_entry)
	addiu	$2,$2,%lo(create_proc_entry)
	jalr	$2
	nop

	sw	$2,24($sp)
	lw	$2,24($sp)
	bne	$2,$0,$L390
	nop

	lui	$2,%hi($LC56)
	addiu	$4,$2,%lo($LC56)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	li	$2,-12			# 0xfffffffffffffff4
	j	$L386
	nop

$L390:
	lw	$2,24($sp)
	lui	$3,%hi(pon_trafficShapingSet_write_proc)
	addiu	$3,$3,%lo(pon_trafficShapingSet_write_proc)
	sw	$3,68($2)
	lw	$2,24($sp)
	lui	$3,%hi(pon_trafficShapingSet_read_proc)
	addiu	$3,$3,%lo(pon_trafficShapingSet_read_proc)
	sw	$3,64($2)
	lui	$2,%hi($LC48)
	addiu	$4,$2,%lo($LC48)
	move	$5,$0
	move	$6,$0
	lui	$2,%hi(create_proc_entry)
	addiu	$2,$2,%lo(create_proc_entry)
	jalr	$2
	nop

	sw	$2,24($sp)
	lw	$2,24($sp)
	bne	$2,$0,$L391
	nop

	lui	$2,%hi($LC57)
	addiu	$4,$2,%lo($LC57)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	li	$2,-12			# 0xfffffffffffffff4
	j	$L386
	nop

$L391:
	lw	$2,24($sp)
	lui	$3,%hi(pon_qoswrr_read_proc)
	addiu	$3,$3,%lo(pon_qoswrr_read_proc)
	sw	$3,64($2)
	lw	$2,24($sp)
	lui	$3,%hi(pon_qoswrr_write_proc)
	addiu	$3,$3,%lo(pon_qoswrr_write_proc)
	sw	$3,68($2)
	lui	$2,%hi($LC49)
	addiu	$4,$2,%lo($LC49)
	move	$5,$0
	move	$6,$0
	lui	$2,%hi(create_proc_entry)
	addiu	$2,$2,%lo(create_proc_entry)
	jalr	$2
	nop

	sw	$2,24($sp)
	lw	$2,24($sp)
	bne	$2,$0,$L392
	nop

	lui	$2,%hi($LC58)
	addiu	$4,$2,%lo($LC58)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	li	$2,-12			# 0xfffffffffffffff4
	j	$L386
	nop

$L392:
	lw	$2,24($sp)
	lui	$3,%hi(pon_tcqos_write_proc)
	addiu	$3,$3,%lo(pon_tcqos_write_proc)
	sw	$3,68($2)
	lui	$2,%hi($LC50)
	addiu	$4,$2,%lo($LC50)
	move	$5,$0
	move	$6,$0
	lui	$2,%hi(create_proc_entry)
	addiu	$2,$2,%lo(create_proc_entry)
	jalr	$2
	nop

	sw	$2,24($sp)
	lw	$2,24($sp)
	bne	$2,$0,$L393
	nop

	lui	$2,%hi($LC59)
	addiu	$4,$2,%lo($LC59)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	li	$2,-12			# 0xfffffffffffffff4
	j	$L386
	nop

$L393:
	lw	$2,24($sp)
	lui	$3,%hi(pon_qos_stop_read_proc)
	addiu	$3,$3,%lo(pon_qos_stop_read_proc)
	sw	$3,64($2)
	lw	$2,24($sp)
	lui	$3,%hi(pon_qos_stop_write_proc)
	addiu	$3,$3,%lo(pon_qos_stop_write_proc)
	sw	$3,68($2)
	lui	$2,%hi($LC51)
	addiu	$4,$2,%lo($LC51)
	move	$5,$0
	move	$6,$0
	lui	$2,%hi(create_proc_entry)
	addiu	$2,$2,%lo(create_proc_entry)
	jalr	$2
	nop

	sw	$2,24($sp)
	lw	$2,24($sp)
	bne	$2,$0,$L394
	nop

	lui	$2,%hi($LC60)
	addiu	$4,$2,%lo($LC60)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	li	$2,-12			# 0xfffffffffffffff4
	j	$L386
	nop

$L394:
	lw	$2,24($sp)
	lui	$3,%hi(pon_qos_stop_num_read_proc)
	addiu	$3,$3,%lo(pon_qos_stop_num_read_proc)
	sw	$3,64($2)
	lw	$2,24($sp)
	lui	$3,%hi(pon_qos_stop_num_write_proc)
	addiu	$3,$3,%lo(pon_qos_stop_num_write_proc)
	sw	$3,68($2)
	lui	$2,%hi(gpWanPriv)
	lw	$3,%lo(gpWanPriv)($2)
	li	$2,33750			# 0x83d6
	addu	$2,$3,$2
	move	$4,$2
	move	$5,$0
	li	$6,1			# 0x1
	lui	$2,%hi(memset)
	addiu	$2,$2,%lo(memset)
	jalr	$2
	nop

	lui	$2,%hi(gpWanPriv)
	lw	$3,%lo(gpWanPriv)($2)
	li	$2,65536			# 0x10000
	addu	$3,$3,$2
	lbu	$2,-31786($3)
	ins	$2,$0,6,1
	sb	$2,-31786($3)
	lui	$2,%hi(gpWanPriv)
	lw	$3,%lo(gpWanPriv)($2)
	li	$2,65536			# 0x10000
	addu	$3,$3,$2
	lbu	$2,-31786($3)
	ins	$2,$0,5,1
	sb	$2,-31786($3)
	lui	$2,%hi(gpWanPriv)
	lw	$3,%lo(gpWanPriv)($2)
	li	$2,33688			# 0x8398
	addu	$2,$3,$2
	move	$4,$2
	move	$5,$0
	move	$6,$0
	lui	$2,%hi(init_timer_key)
	addiu	$2,$2,%lo(init_timer_key)
	jalr	$2
	nop

	lui	$2,%hi(gpWanPriv)
	lw	$3,%lo(gpWanPriv)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	lui	$3,%hi(pwan_tx_drop_timer)
	addiu	$3,$3,%lo(pwan_tx_drop_timer)
	sw	$3,-31832($2)
	lui	$2,%hi(gpWanPriv)
	lw	$3,%lo(gpWanPriv)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	li	$3,5000			# 0x1388
	sw	$3,-31828($2)
	lui	$2,%hi(gpWanPriv)
	lw	$3,%lo(gpWanPriv)($2)
	li	$2,33752			# 0x83d8
	addu	$2,$3,$2
	sw	$2,28($sp)
	lui	$2,%hi(gpWanPriv)
	lw	$3,%lo(gpWanPriv)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	sw	$0,-31784($2)
	lui	$2,%hi(gpPonSysData)
	lw	$3,%lo(gpPonSysData)($2)
	li	$2,65536			# 0x10000
	addu	$2,$3,$2
	lhu	$2,-30816($2)
	andi	$2,$2,0x40
	beq	$2,$0,$L396
	nop

	lui	$2,%hi(jiffies)
	lw	$3,%lo(jiffies)($2)
	lui	$2,%hi($LC61)
	addiu	$4,$2,%lo($LC61)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L396:
	move	$2,$0
$L386:
	lw	$31,36($sp)
	addiu	$sp,$sp,40
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	pwan_init
	.size	pwan_init, .-pwan_init
	.rdata
	.align	2
	.type	__func__.36650, @object
	.size	__func__.36650, 16
__func__.36650:
	.ascii	"replace_oam_pkt\000"
	.align	2
	.type	__func__.36689, @object
	.size	__func__.36689, 22
__func__.36689:
	.ascii	"pon_qoswrr_write_proc\000"
	.align	2
	.type	__func__.36798, @object
	.size	__func__.36798, 14
__func__.36798:
	.ascii	"pwan_net_open\000"
	.align	2
	.type	__func__.36803, @object
	.size	__func__.36803, 14
__func__.36803:
	.ascii	"pwan_net_stop\000"
	.align	2
	.type	__func__.36841, @object
	.size	__func__.36841, 14
__func__.36841:
	.ascii	"copy_pwan_skb\000"
	.align	2
	.type	omciHdr.36840, @object
	.size	omciHdr.36840, 14
omciHdr.36840:
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	2
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	1
	.byte	-120
	.byte	-75
	.align	2
	.type	__func__.36861, @object
	.size	__func__.36861, 20
__func__.36861:
	.ascii	"pwan_net_start_xmit\000"
	.ident	"GCC: (Buildroot 2015.08.1) 4.9.3"
