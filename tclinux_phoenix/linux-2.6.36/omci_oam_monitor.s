	.file	1 "omci_oam_monitor.c"
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
	.local	gpMonitorParams
	.comm	gpMonitorParams,4,4
	.local	gpMonitor
	.comm	gpMonitor,4,4
	.section	.text.stop_omci_oam_monitor_not_online,"ax",@progbits
	.align	2
	.globl	stop_omci_oam_monitor_not_online
	.set	nomips16
	.set	nomicromips
	.ent	stop_omci_oam_monitor_not_online
	.type	stop_omci_oam_monitor_not_online, @function
stop_omci_oam_monitor_not_online:
	.frame	$sp,24,$31		# vars= 0, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-24
	sw	$31,20($sp)
	lui	$2,%hi(gpMonitor)
	lw	$2,%lo(gpMonitor)($2)
	sw	$0,172($2)
	move	$2,$28
	lw	$2,20($2)
	move	$3,$2
	li	$2,134152192			# 0x7ff0000
	ori	$2,$2,0xff00
	and	$2,$3,$2
	beq	$2,$0,$L2
	nop

	lui	$2,%hi(gpMonitor)
	lw	$2,%lo(gpMonitor)($2)
	addiu	$2,$2,184
	move	$4,$2
	lui	$2,%hi(del_timer)
	addiu	$2,$2,%lo(del_timer)
	jalr	$2
	nop

	j	$L1
	nop

$L2:
	lui	$2,%hi(gpMonitor)
	lw	$2,%lo(gpMonitor)($2)
	addiu	$2,$2,184
	move	$4,$2
	lui	$2,%hi(del_timer_sync)
	addiu	$2,$2,%lo(del_timer_sync)
	jalr	$2
	nop

$L1:
	lw	$31,20($sp)
	addiu	$sp,$sp,24
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	stop_omci_oam_monitor_not_online
	.size	stop_omci_oam_monitor_not_online, .-stop_omci_oam_monitor_not_online
	.section	.text.stop_omci_oam_monitor,"ax",@progbits
	.align	2
	.globl	stop_omci_oam_monitor
	.set	nomips16
	.set	nomicromips
	.ent	stop_omci_oam_monitor
	.type	stop_omci_oam_monitor, @function
stop_omci_oam_monitor:
	.frame	$sp,24,$31		# vars= 0, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-24
	sw	$31,20($sp)
	lui	$2,%hi(gpMonitor)
	lw	$2,%lo(gpMonitor)($2)
	sw	$0,172($2)
	move	$2,$28
	lw	$2,20($2)
	move	$3,$2
	li	$2,134152192			# 0x7ff0000
	ori	$2,$2,0xff00
	and	$2,$3,$2
	beq	$2,$0,$L5
	nop

	lui	$2,%hi(gpMonitor)
	lw	$2,%lo(gpMonitor)($2)
	addiu	$2,$2,184
	move	$4,$2
	lui	$2,%hi(del_timer)
	addiu	$2,$2,%lo(del_timer)
	jalr	$2
	nop

	j	$L4
	nop

$L5:
	lui	$2,%hi(gpMonitor)
	lw	$2,%lo(gpMonitor)($2)
	addiu	$2,$2,184
	move	$4,$2
	lui	$2,%hi(del_timer_sync)
	addiu	$2,$2,%lo(del_timer_sync)
	jalr	$2
	nop

$L4:
	lw	$31,20($sp)
	addiu	$sp,$sp,24
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	stop_omci_oam_monitor
	.size	stop_omci_oam_monitor, .-stop_omci_oam_monitor
	.rdata
	.align	2
$LC0:
	.ascii	"%s:%d start monitor omci/oam pkts!\012\000"
	.align	2
$LC1:
	.ascii	"omci\000"
	.align	2
$LC2:
	.ascii	"oam\000"
	.section	.text.start_omci_oam_monitor,"ax",@progbits
	.align	2
	.globl	start_omci_oam_monitor
	.set	nomips16
	.set	nomicromips
	.ent	start_omci_oam_monitor
	.type	start_omci_oam_monitor, @function
start_omci_oam_monitor:
	.frame	$sp,40,$31		# vars= 16, regs= 2/0, args= 16, gp= 0
	.mask	0x80010000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-40
	sw	$31,36($sp)
	sw	$16,32($sp)
	lui	$2,%hi(gpMonitor)
	lw	$2,%lo(gpMonitor)($2)
	addiu	$2,$2,52
	move	$4,$2
	move	$5,$0
	li	$6,120			# 0x78
	lui	$2,%hi(memset)
	addiu	$2,$2,%lo(memset)
	jalr	$2
	nop

	lui	$2,%hi(gpMonitor)
	lw	$2,%lo(gpMonitor)($2)
	sb	$0,40($2)
	lui	$2,%hi(gpMonitor)
	lw	$2,%lo(gpMonitor)($2)
	sw	$0,44($2)
	lui	$2,%hi(gpMonitor)
	lw	$2,%lo(gpMonitor)($2)
	sw	$0,48($2)
	lui	$2,%hi(gpMonitor)
	lw	$2,%lo(gpMonitor)($2)
	sw	$0,180($2)
	lui	$2,%hi(gpPhyData)
	lw	$2,%lo(gpPhyData)($2)
	lw	$3,60($2)
	li	$2,100663296			# 0x6000000
	and	$3,$3,$2
	li	$2,33554432			# 0x2000000
	bne	$3,$2,$L8
	nop

	lui	$2,%hi(gpMonitor)
	lw	$2,%lo(gpMonitor)($2)
	j	$L9
	nop

$L8:
	lui	$2,%hi(gpMonitor)
	lw	$2,%lo(gpMonitor)($2)
	addiu	$2,$2,20
$L9:
	lui	$3,%hi(gpMonitorParams)
	sw	$2,%lo(gpMonitorParams)($3)
	lui	$2,%hi(gpMonitor)
	lw	$2,%lo(gpMonitor)($2)
	lui	$3,%hi(gpMonitorParams)
	lw	$3,%lo(gpMonitorParams)($3)
	lw	$3,8($3)
	sw	$3,204($2)
	lui	$2,%hi(xpon_mac_print_open)
	lw	$2,%lo(xpon_mac_print_open)($2)
	beq	$2,$0,$L10
	nop

	lui	$2,%hi($LC0)
	addiu	$4,$2,%lo($LC0)
	lui	$2,%hi(__func__.30823)
	addiu	$5,$2,%lo(__func__.30823)
	li	$6,68			# 0x44
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L10:
	lui	$2,%hi(gpMonitor)
	lw	$16,%lo(gpMonitor)($2)
	lui	$2,%hi(gpPhyData)
	lw	$2,%lo(gpPhyData)($2)
	lw	$3,60($2)
	li	$2,100663296			# 0x6000000
	and	$3,$3,$2
	li	$2,33554432			# 0x2000000
	bne	$3,$2,$L11
	nop

	lui	$2,%hi($LC1)
	addiu	$2,$2,%lo($LC1)
	j	$L12
	nop

$L11:
	lui	$2,%hi($LC2)
	addiu	$2,$2,%lo($LC2)
$L12:
	sw	$2,16($sp)
	lui	$2,%hi(init_net)
	addiu	$4,$2,%lo(init_net)
	lw	$5,16($sp)
	lui	$2,%hi(dev_get_by_name)
	addiu	$2,$2,%lo(dev_get_by_name)
	jalr	$2
	nop

	sw	$2,20($sp)
	lw	$2,20($sp)
	sw	$2,24($sp)
	lw	$2,24($sp)
	addiu	$2,$2,928
	sw	$2,28($sp)
	lw	$2,28($sp)
	lw	$2,48($2)
	sw	$2,176($16)
	lui	$2,%hi(gpMonitor)
	lw	$2,%lo(gpMonitor)($2)
	li	$3,1			# 0x1
	sw	$3,172($2)
	lui	$2,%hi(gpMonitor)
	lw	$2,%lo(gpMonitor)($2)
	addiu	$4,$2,184
	lui	$2,%hi(gpMonitor)
	lw	$2,%lo(gpMonitor)($2)
	lw	$2,204($2)
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

	lw	$31,36($sp)
	lw	$16,32($sp)
	addiu	$sp,$sp,40
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	start_omci_oam_monitor
	.size	start_omci_oam_monitor, .-start_omci_oam_monitor
	.rdata
	.align	2
$LC3:
	.ascii	"%s:%d == omci/oam conf done! ==\012\000"
	.section	.text.monitor_done,"ax",@progbits
	.align	2
	.set	nomips16
	.set	nomicromips
	.ent	monitor_done
	.type	monitor_done, @function
monitor_done:
	.frame	$sp,32,$31		# vars= 8, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-32
	sw	$31,28($sp)
	sw	$0,16($sp)
	lui	$2,%hi(gpPhyData)
	lw	$2,%lo(gpPhyData)($2)
	lw	$3,60($2)
	li	$2,100663296			# 0x6000000
	and	$3,$3,$2
	li	$2,33554432			# 0x2000000
	bne	$3,$2,$L16
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

$L16:
	lui	$2,%hi(ra_sw_nat_hook_clean_table)
	lw	$2,%lo(ra_sw_nat_hook_clean_table)($2)
	beq	$2,$0,$L17
	nop

	lui	$2,%hi(ra_sw_nat_hook_clean_table)
	lw	$2,%lo(ra_sw_nat_hook_clean_table)($2)
	jalr	$2
	nop

$L17:
	lui	$2,%hi(xpon_mac_print_open)
	lw	$2,%lo(xpon_mac_print_open)($2)
	beq	$2,$0,$L18
	nop

	lui	$2,%hi($LC3)
	addiu	$4,$2,%lo($LC3)
	lui	$2,%hi(__func__.30828)
	addiu	$5,$2,%lo(__func__.30828)
	li	$6,95			# 0x5f
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L18:
	lui	$2,%hi(gpMonitor)
	lw	$2,%lo(gpMonitor)($2)
	li	$3,2			# 0x2
	sw	$3,172($2)
	lui	$2,%hi(gpPonSysData)
	lw	$4,%lo(gpPonSysData)($2)
	lui	$2,%hi(jiffies)
	lw	$3,%lo(jiffies)($2)
	li	$2,65536			# 0x10000
	addu	$2,$4,$2
	sw	$3,-30196($2)
	lw	$31,28($sp)
	addiu	$sp,$sp,32
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	monitor_done
	.size	monitor_done, .-monitor_done
	.rdata
	.align	2
$LC4:
	.ascii	"%s:%d timeout cnt reach max value: %u\012\000"
	.align	2
$LC5:
	.ascii	"%s:%d == cnt_diff_total: %u\012\000"
	.section	.text.xpon_count_rx_pkt,"ax",@progbits
	.align	2
	.globl	xpon_count_rx_pkt
	.set	nomips16
	.set	nomicromips
	.ent	xpon_count_rx_pkt
	.type	xpon_count_rx_pkt, @function
xpon_count_rx_pkt:
	.frame	$sp,48,$31		# vars= 24, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-48
	sw	$31,44($sp)
	sw	$0,16($sp)
	sw	$0,20($sp)
	lui	$2,%hi(gpMonitor)
	lw	$2,%lo(gpMonitor)($2)
	lw	$3,172($2)
	li	$2,1			# 0x1
	beq	$3,$2,$L20
	nop

	j	$L19
	nop

$L20:
	lui	$2,%hi(gpMonitor)
	lw	$2,%lo(gpMonitor)($2)
	lw	$3,44($2)
	addiu	$3,$3,1
	sw	$3,44($2)
	lui	$2,%hi(gpMonitor)
	lw	$2,%lo(gpMonitor)($2)
	lw	$3,44($2)
	lui	$2,%hi(gpMonitorParams)
	lw	$2,%lo(gpMonitorParams)($2)
	lw	$2,16($2)
	sltu	$2,$2,$3
	beq	$2,$0,$L22
	nop

	lui	$2,%hi(xpon_mac_print_open)
	lw	$2,%lo(xpon_mac_print_open)($2)
	beq	$2,$0,$L23
	nop

	lui	$2,%hi(gpMonitorParams)
	lw	$2,%lo(gpMonitorParams)($2)
	lw	$3,16($2)
	lui	$2,%hi($LC4)
	addiu	$4,$2,%lo($LC4)
	lui	$2,%hi(__func__.30834)
	addiu	$5,$2,%lo(__func__.30834)
	li	$6,121			# 0x79
	move	$7,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L23:
	lui	$2,%hi(monitor_done)
	addiu	$2,$2,%lo(monitor_done)
	jalr	$2
	nop

	j	$L19
	nop

$L22:
	lui	$2,%hi(gpPhyData)
	lw	$2,%lo(gpPhyData)($2)
	lw	$3,60($2)
	li	$2,100663296			# 0x6000000
	and	$3,$3,$2
	li	$2,33554432			# 0x2000000
	bne	$3,$2,$L24
	nop

	lui	$2,%hi($LC1)
	addiu	$2,$2,%lo($LC1)
	j	$L25
	nop

$L24:
	lui	$2,%hi($LC2)
	addiu	$2,$2,%lo($LC2)
$L25:
	sw	$2,24($sp)
	lui	$2,%hi(init_net)
	addiu	$4,$2,%lo(init_net)
	lw	$5,24($sp)
	lui	$2,%hi(dev_get_by_name)
	addiu	$2,$2,%lo(dev_get_by_name)
	jalr	$2
	nop

	sw	$2,28($sp)
	lw	$2,28($sp)
	sw	$2,32($sp)
	lw	$2,32($sp)
	addiu	$2,$2,928
	sw	$2,36($sp)
	lw	$2,36($sp)
	lw	$2,48($2)
	sw	$2,16($sp)
	lui	$2,%hi(gpMonitor)
	lw	$2,%lo(gpMonitor)($2)
	lw	$2,176($2)
	lw	$3,16($sp)
	subu	$2,$3,$2
	sw	$2,20($sp)
	lui	$2,%hi(gpMonitor)
	lw	$2,%lo(gpMonitor)($2)
	lbu	$2,40($2)
	bne	$2,$0,$L28
	nop

	lw	$2,20($sp)
	beq	$2,$0,$L29
	nop

	lui	$2,%hi(gpMonitor)
	lw	$2,%lo(gpMonitor)($2)
	li	$3,1			# 0x1
	sb	$3,40($2)
	lui	$2,%hi(gpMonitor)
	lw	$2,%lo(gpMonitor)($2)
	li	$3,1			# 0x1
	sw	$3,44($2)
	j	$L28
	nop

$L29:
	j	$L30
	nop

$L28:
	lui	$2,%hi(gpMonitor)
	lw	$2,%lo(gpMonitor)($2)
	lw	$3,44($2)
	lui	$2,%hi(gpMonitorParams)
	lw	$2,%lo(gpMonitorParams)($2)
	lw	$2,0($2)
	sltu	$2,$2,$3
	beq	$2,$0,$L31
	nop

	lui	$2,%hi(gpMonitor)
	lw	$2,%lo(gpMonitor)($2)
	lw	$3,48($2)
	lui	$2,%hi(gpMonitorParams)
	lw	$2,%lo(gpMonitorParams)($2)
	lw	$2,0($2)
	sltu	$2,$3,$2
	bne	$2,$0,$L32
	nop

	lui	$2,%hi(gpMonitor)
	lw	$2,%lo(gpMonitor)($2)
	lui	$3,%hi(gpMonitor)
	lw	$3,%lo(gpMonitor)($3)
	lw	$4,48($3)
	lui	$3,%hi(gpMonitorParams)
	lw	$3,%lo(gpMonitorParams)($3)
	lw	$3,0($3)
	subu	$3,$4,$3
	sw	$3,48($2)
$L32:
	lui	$2,%hi(gpMonitor)
	lw	$2,%lo(gpMonitor)($2)
	lui	$3,%hi(gpMonitor)
	lw	$3,%lo(gpMonitor)($3)
	lw	$4,180($3)
	lui	$3,%hi(gpMonitor)
	lw	$5,%lo(gpMonitor)($3)
	lui	$3,%hi(gpMonitor)
	lw	$3,%lo(gpMonitor)($3)
	lw	$3,48($3)
	addiu	$3,$3,12
	sll	$3,$3,2
	addu	$3,$5,$3
	lw	$3,4($3)
	subu	$3,$4,$3
	sw	$3,180($2)
$L31:
	lui	$2,%hi(gpMonitor)
	lw	$2,%lo(gpMonitor)($2)
	lui	$3,%hi(gpMonitor)
	lw	$3,%lo(gpMonitor)($3)
	lw	$4,180($3)
	lw	$3,20($sp)
	addu	$3,$4,$3
	sw	$3,180($2)
	lui	$2,%hi(gpMonitor)
	lw	$3,%lo(gpMonitor)($2)
	lui	$2,%hi(gpMonitor)
	lw	$2,%lo(gpMonitor)($2)
	lw	$2,48($2)
	addiu	$2,$2,12
	sll	$2,$2,2
	addu	$2,$3,$2
	lw	$3,20($sp)
	sw	$3,4($2)
	lui	$2,%hi(gpMonitor)
	lw	$2,%lo(gpMonitor)($2)
	lw	$3,16($sp)
	sw	$3,176($2)
	lui	$2,%hi(gpMonitor)
	lw	$2,%lo(gpMonitor)($2)
	lw	$3,44($2)
	lui	$2,%hi(gpMonitorParams)
	lw	$2,%lo(gpMonitorParams)($2)
	lw	$2,0($2)
	sltu	$2,$3,$2
	bne	$2,$0,$L33
	nop

	lui	$2,%hi(xpon_mac_print_open)
	lw	$2,%lo(xpon_mac_print_open)($2)
	beq	$2,$0,$L34
	nop

	lui	$2,%hi(gpMonitor)
	lw	$2,%lo(gpMonitor)($2)
	lw	$3,180($2)
	lui	$2,%hi($LC5)
	addiu	$4,$2,%lo($LC5)
	lui	$2,%hi(__func__.30834)
	addiu	$5,$2,%lo(__func__.30834)
	li	$6,155			# 0x9b
	move	$7,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L34:
	lui	$2,%hi(gpMonitor)
	lw	$2,%lo(gpMonitor)($2)
	lw	$3,180($2)
	lui	$2,%hi(gpMonitorParams)
	lw	$2,%lo(gpMonitorParams)($2)
	lw	$2,4($2)
	sltu	$2,$3,$2
	beq	$2,$0,$L33
	nop

	lui	$2,%hi(monitor_done)
	addiu	$2,$2,%lo(monitor_done)
	jalr	$2
	nop

	j	$L19
	nop

$L33:
	lui	$2,%hi(gpMonitor)
	lw	$2,%lo(gpMonitor)($2)
	lw	$3,48($2)
	addiu	$3,$3,1
	sw	$3,48($2)
$L30:
	lui	$2,%hi(gpMonitor)
	lw	$2,%lo(gpMonitor)($2)
	addiu	$4,$2,184
	lui	$2,%hi(gpMonitor)
	lw	$2,%lo(gpMonitor)($2)
	lw	$2,204($2)
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

$L19:
	lw	$31,44($sp)
	addiu	$sp,$sp,48
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	xpon_count_rx_pkt
	.size	xpon_count_rx_pkt, .-xpon_count_rx_pkt
	.rdata
	.align	2
$LC6:
	.ascii	"%d\012\000"
	.section	.text.xpon_omai_oam_monitor_read_proc,"ax",@progbits
	.align	2
	.globl	xpon_omai_oam_monitor_read_proc
	.set	nomips16
	.set	nomicromips
	.ent	xpon_omai_oam_monitor_read_proc
	.type	xpon_omai_oam_monitor_read_proc, @function
xpon_omai_oam_monitor_read_proc:
	.frame	$sp,160,$31		# vars= 136, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-160
	sw	$31,156($sp)
	sw	$4,160($sp)
	sw	$5,164($sp)
	sw	$6,168($sp)
	sw	$7,172($sp)
	sw	$0,16($sp)
	li	$2,-1078984704			# 0xffffffffbfb00000
	ori	$2,$2,0x64
	sw	$2,20($sp)
	lw	$2,20($sp)
	lw	$2,0($2)
	andi	$3,$2,0xf
	li	$2,3			# 0x3
	bne	$3,$2,$L37
	nop

	li	$2,-1078657024			# 0xffffffffbfb50000
	ori	$2,$2,0xfffc
	sw	$2,24($sp)
	lw	$2,24($sp)
	lw	$2,0($2)
	andi	$2,$2,0xf
	beq	$2,$0,$L37
	nop

	li	$2,-1078984704			# 0xffffffffbfb00000
	ori	$2,$2,0xf8
	sw	$2,28($sp)
	lw	$2,28($sp)
	lw	$2,0($2)
	andi	$3,$2,0x3
	li	$2,2			# 0x2
	bne	$3,$2,$L40
	nop

	li	$2,-1078984704			# 0xffffffffbfb00000
	ori	$2,$2,0x64
	sw	$2,32($sp)
	lw	$2,32($sp)
	lw	$3,0($2)
	li	$2,-65536			# 0xffffffffffff0000
	and	$3,$3,$2
	li	$2,327680			# 0x50000
	bne	$3,$2,$L40
	nop

	li	$2,-1078001664			# 0xffffffffbfbf0000
	ori	$2,$2,0x8214
	sw	$2,36($sp)
	lw	$2,36($sp)
	lw	$2,0($2)
	andi	$2,$2,0x8
	beq	$2,$0,$L43
	nop

	li	$2,-1078001664			# 0xffffffffbfbf0000
	ori	$2,$2,0x8214
	sw	$2,40($sp)
	lw	$2,40($sp)
	lw	$2,0($2)
	andi	$2,$2,0x20
	bne	$2,$0,$L45
	nop

	j	$L40
	nop

$L43:
	li	$2,-1078001664			# 0xffffffffbfbf0000
	ori	$2,$2,0x8214
	sw	$2,44($sp)
	lw	$2,44($sp)
	lw	$2,0($2)
	andi	$2,$2,0x2
	beq	$2,$0,$L40
	nop

$L45:
	li	$2,-1078001664			# 0xffffffffbfbf0000
	ori	$2,$2,0x8214
	sw	$2,48($sp)
	lw	$2,48($sp)
	lw	$2,0($2)
	andi	$2,$2,0x8
	beq	$2,$0,$L48
	nop

	li	$2,-1078001664			# 0xffffffffbfbf0000
	ori	$2,$2,0x8214
	sw	$2,52($sp)
	lw	$2,52($sp)
	lw	$2,0($2)
	andi	$2,$2,0x40
	beq	$2,$0,$L50
	nop

	j	$L40
	nop

$L48:
	li	$2,-1078001664			# 0xffffffffbfbf0000
	ori	$2,$2,0x8214
	sw	$2,56($sp)
	lw	$2,56($sp)
	lw	$2,0($2)
	andi	$2,$2,0x4
	beq	$2,$0,$L50
	nop

$L40:
	li	$2,-1078984704			# 0xffffffffbfb00000
	ori	$2,$2,0xf8
	sw	$2,60($sp)
	lw	$2,60($sp)
	lw	$2,0($2)
	andi	$3,$2,0x3
	li	$2,3			# 0x3
	bne	$3,$2,$L53
	nop

	li	$2,-1078984704			# 0xffffffffbfb00000
	ori	$2,$2,0x64
	sw	$2,64($sp)
	lw	$2,64($sp)
	lw	$3,0($2)
	li	$2,-65536			# 0xffffffffffff0000
	and	$3,$3,$2
	li	$2,327680			# 0x50000
	bne	$3,$2,$L53
	nop

	li	$2,-1078001664			# 0xffffffffbfbf0000
	ori	$2,$2,0x8214
	sw	$2,68($sp)
	lw	$2,68($sp)
	lw	$2,0($2)
	andi	$2,$2,0x8
	beq	$2,$0,$L56
	nop

	li	$2,-1078001664			# 0xffffffffbfbf0000
	ori	$2,$2,0x8214
	sw	$2,72($sp)
	lw	$2,72($sp)
	lw	$2,0($2)
	andi	$2,$2,0x20
	bne	$2,$0,$L50
	nop

	j	$L53
	nop

$L56:
	li	$2,-1078001664			# 0xffffffffbfbf0000
	ori	$2,$2,0x8214
	sw	$2,76($sp)
	lw	$2,76($sp)
	lw	$2,0($2)
	andi	$2,$2,0x2
	bne	$2,$0,$L50
	nop

$L53:
	li	$2,-1078984704			# 0xffffffffbfb00000
	ori	$2,$2,0xf8
	sw	$2,80($sp)
	lw	$2,80($sp)
	lw	$2,0($2)
	andi	$3,$2,0x3
	li	$2,2			# 0x2
	bne	$3,$2,$L60
	nop

	li	$2,-1078984704			# 0xffffffffbfb00000
	ori	$2,$2,0x64
	sw	$2,84($sp)
	lw	$2,84($sp)
	lw	$3,0($2)
	li	$2,-65536			# 0xffffffffffff0000
	and	$3,$3,$2
	li	$2,327680			# 0x50000
	bne	$3,$2,$L60
	nop

	li	$2,-1078001664			# 0xffffffffbfbf0000
	ori	$2,$2,0x8214
	sw	$2,88($sp)
	lw	$2,88($sp)
	lw	$2,0($2)
	andi	$2,$2,0x8
	beq	$2,$0,$L63
	nop

	li	$2,-1078001664			# 0xffffffffbfbf0000
	ori	$2,$2,0x8214
	sw	$2,92($sp)
	lw	$2,92($sp)
	lw	$2,0($2)
	andi	$2,$2,0x20
	beq	$2,$0,$L65
	nop

	j	$L60
	nop

$L63:
	li	$2,-1078001664			# 0xffffffffbfbf0000
	ori	$2,$2,0x8214
	sw	$2,96($sp)
	lw	$2,96($sp)
	lw	$2,0($2)
	andi	$2,$2,0x2
	bne	$2,$0,$L60
	nop

$L65:
	li	$2,-1078001664			# 0xffffffffbfbf0000
	ori	$2,$2,0x8214
	sw	$2,100($sp)
	lw	$2,100($sp)
	lw	$2,0($2)
	andi	$2,$2,0x8
	beq	$2,$0,$L68
	nop

	li	$2,-1078001664			# 0xffffffffbfbf0000
	ori	$2,$2,0x8214
	sw	$2,104($sp)
	lw	$2,104($sp)
	lw	$2,0($2)
	andi	$2,$2,0x40
	beq	$2,$0,$L50
	nop

	j	$L60
	nop

$L68:
	li	$2,-1078001664			# 0xffffffffbfbf0000
	ori	$2,$2,0x8214
	sw	$2,108($sp)
	lw	$2,108($sp)
	lw	$2,0($2)
	andi	$2,$2,0x4
	beq	$2,$0,$L50
	nop

$L60:
	li	$2,-1078984704			# 0xffffffffbfb00000
	ori	$2,$2,0xf8
	sw	$2,112($sp)
	lw	$2,112($sp)
	lw	$2,0($2)
	andi	$3,$2,0x3
	li	$2,3			# 0x3
	bne	$3,$2,$L72
	nop

	li	$2,-1078984704			# 0xffffffffbfb00000
	ori	$2,$2,0x64
	sw	$2,116($sp)
	lw	$2,116($sp)
	lw	$3,0($2)
	li	$2,-65536			# 0xffffffffffff0000
	and	$3,$3,$2
	li	$2,327680			# 0x50000
	bne	$3,$2,$L72
	nop

	li	$2,-1078001664			# 0xffffffffbfbf0000
	ori	$2,$2,0x8214
	sw	$2,120($sp)
	lw	$2,120($sp)
	lw	$2,0($2)
	andi	$2,$2,0x8
	beq	$2,$0,$L75
	nop

	li	$2,-1078001664			# 0xffffffffbfbf0000
	ori	$2,$2,0x8214
	sw	$2,124($sp)
	lw	$2,124($sp)
	lw	$2,0($2)
	andi	$2,$2,0x20
	beq	$2,$0,$L50
	nop

	j	$L72
	nop

$L75:
	li	$2,-1078001664			# 0xffffffffbfbf0000
	ori	$2,$2,0x8214
	sw	$2,128($sp)
	lw	$2,128($sp)
	lw	$2,0($2)
	andi	$2,$2,0x2
	beq	$2,$0,$L50
	nop

$L72:
	li	$2,-1078984704			# 0xffffffffbfb00000
	ori	$2,$2,0xf8
	sw	$2,132($sp)
	lw	$2,132($sp)
	lw	$2,0($2)
	andi	$3,$2,0x3
	li	$2,2			# 0x2
	bne	$3,$2,$L37
	nop

	li	$2,-1078984704			# 0xffffffffbfb00000
	ori	$2,$2,0x64
	sw	$2,136($sp)
	lw	$2,136($sp)
	lw	$3,0($2)
	li	$2,-65536			# 0xffffffffffff0000
	and	$3,$3,$2
	li	$2,327680			# 0x50000
	bne	$3,$2,$L37
	nop

	li	$2,-1078001664			# 0xffffffffbfbf0000
	ori	$2,$2,0x8214
	sw	$2,140($sp)
	lw	$2,140($sp)
	lw	$2,0($2)
	andi	$2,$2,0x8
	beq	$2,$0,$L81
	nop

	li	$2,-1078001664			# 0xffffffffbfbf0000
	ori	$2,$2,0x8214
	sw	$2,144($sp)
	lw	$2,144($sp)
	lw	$2,0($2)
	andi	$2,$2,0x40
	bne	$2,$0,$L50
	nop

	j	$L37
	nop

$L81:
	li	$2,-1078001664			# 0xffffffffbfbf0000
	ori	$2,$2,0x8214
	sw	$2,148($sp)
	lw	$2,148($sp)
	lw	$2,0($2)
	andi	$2,$2,0x4
	beq	$2,$0,$L37
	nop

$L50:
	li	$3,1			# 0x1
	j	$L84
	nop

$L37:
	move	$3,$0
$L84:
	lw	$4,160($sp)
	lui	$2,%hi($LC6)
	addiu	$5,$2,%lo($LC6)
	move	$6,$3
	lui	$2,%hi(sprintf)
	addiu	$2,$2,%lo(sprintf)
	jalr	$2
	nop

	sw	$2,16($sp)
	lw	$3,168($sp)
	lw	$2,172($sp)
	addu	$3,$3,$2
	lw	$2,16($sp)
	slt	$2,$2,$3
	beq	$2,$0,$L85
	nop

	lw	$2,176($sp)
	li	$3,1			# 0x1
	sw	$3,0($2)
$L85:
	lw	$3,16($sp)
	lw	$2,168($sp)
	subu	$2,$3,$2
	sw	$2,16($sp)
	lw	$2,168($sp)
	lw	$3,160($sp)
	addu	$3,$3,$2
	lw	$2,164($sp)
	sw	$3,0($2)
	lw	$3,16($sp)
	lw	$2,172($sp)
	slt	$2,$2,$3
	beq	$2,$0,$L86
	nop

	lw	$2,172($sp)
	sw	$2,16($sp)
$L86:
	lw	$2,16($sp)
	bgez	$2,$L87
	nop

	sw	$0,16($sp)
$L87:
	lw	$2,16($sp)
	lw	$31,156($sp)
	addiu	$sp,$sp,160
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	xpon_omai_oam_monitor_read_proc
	.size	xpon_omai_oam_monitor_read_proc, .-xpon_omai_oam_monitor_read_proc
	.rdata
	.align	2
$LC7:
	.ascii	"Command needed!\012\000"
	.align	2
$LC8:
	.ascii	"Command e.g.:\012    help        # show this message\012"
	.ascii	"    [gpon|epon] maxCntIdx   # config maximum number for "
	.ascii	"counting rx packets\012    [gpon|epon] rxPktLmt    # con"
	.ascii	"fig rx packets limit\012    [gpon|epon] interval    # co"
	.ascii	"nfig timer interval\012    [gpon|epon] maxWaitTime # max"
	.ascii	" wait time before wan traffic on (unit ms)\000"
	.align	2
$LC9:
	.ascii	"help\000"
	.align	2
$LC10:
	.ascii	"showCfg\000"
	.align	2
$LC11:
	.ascii	"gpon\000"
	.align	2
$LC12:
	.ascii	"epon\000"
	.align	2
$LC13:
	.ascii	"Unknow command %s!\012\000"
	.align	2
$LC14:
	.ascii	"maxCntIdx\000"
	.align	2
$LC15:
	.ascii	"Error! \"maxCntIdx\" command takes 1 integer param!\012\000"
	.align	2
$LC16:
	.ascii	"%u\000"
	.align	2
$LC17:
	.ascii	"rxPktLmt\000"
	.align	2
$LC18:
	.ascii	"Error! \"rxPktLmt\" command takes 1 integer param!\012\000"
	.align	2
$LC19:
	.ascii	"interval\000"
	.align	2
$LC20:
	.ascii	"Error! \"interval\" command takes 1 integer param!\012\000"
	.align	2
$LC21:
	.ascii	"maxWaitTime\000"
	.align	2
$LC22:
	.ascii	"Unknown sub command: \"%s\"\012\000"
	.align	2
$LC23:
	.ascii	"GPON Current Confing:\012    Max number for counting rx "
	.ascii	"packets: %u\012    Limit number for rx diff packets: %u\012"
	.ascii	"    Timer interval: %u\012    max wait time: %u\012\012E"
	.ascii	"PON Current Confing:\012    Max number for counting rx p"
	.ascii	"ackets: %u\012    Limit number for rx diff packets: %u\012"
	.ascii	"    Timer interval: %u\012    max wait time: %u\012\000"
	.section	.text.xpon_omci_oam_monitor_write_proc,"ax",@progbits
	.align	2
	.globl	xpon_omci_oam_monitor_write_proc
	.set	nomips16
	.set	nomicromips
	.ent	xpon_omci_oam_monitor_write_proc
	.type	xpon_omci_oam_monitor_write_proc, @function
xpon_omci_oam_monitor_write_proc:
	.frame	$sp,264,$31		# vars= 216, regs= 1/0, args= 40, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	addiu	$sp,$sp,-264
	sw	$31,260($sp)
	sw	$4,264($sp)
	sw	$5,268($sp)
	sw	$6,272($sp)
	sw	$7,276($sp)
	addiu	$3,$sp,168
	li	$2,64			# 0x40
	move	$4,$3
	move	$5,$0
	move	$6,$2
	lui	$2,%hi(memset)
	addiu	$2,$2,%lo(memset)
	jalr	$2
	sw	$0,248($sp)
	sb	$0,44($sp)
	lw	$2,272($sp)
	sltu	$2,$2,64
	bne	$2,$0,$L90
	li	$2,-22			# 0xffffffffffffffea
	j	$L120
$L90:
	addiu	$2,$sp,168
	sw	$2,48($sp)
	lw	$2,268($sp)
	sw	$2,52($sp)
	lw	$2,272($sp)
	sw	$2,40($sp)
	lw	$2,52($sp)
	sw	$2,56($sp)
	lw	$2,40($sp)
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
	beq	$2,$0,$L92
	lw	$4,48($sp)
	lw	$5,52($sp)
	lw	$6,40($sp)
#APP
 # 208 "/opt/tclinux_phoenix/modules/private/xpon/src/omci_oam_monitor.c" 1
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
	sw	$2,40($sp)
$L92:
	lw	$2,40($sp)
	beq	$2,$0,$L93
	li	$2,-14			# 0xfffffffffffffff2
	j	$L120
$L93:
	lw	$2,272($sp)
	addiu	$3,$sp,40
	addu	$2,$3,$2
	sb	$0,128($2)
	addiu	$4,$sp,168
	addiu	$3,$sp,248
	addiu	$2,$sp,232
	move	$5,$3
	move	$6,$2
	lui	$2,%hi(xpon_prepare_params)
	addiu	$2,$2,%lo(xpon_prepare_params)
	jalr	$2
	lw	$2,248($sp)
	bne	$2,$0,$L94
	lui	$2,%hi($LC7)
	addiu	$4,$2,%lo($LC7)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	lui	$2,%hi($LC8)
	addiu	$4,$2,%lo($LC8)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	lw	$2,272($sp)
	j	$L120
$L94:
	lw	$2,232($sp)
	lui	$3,%hi($LC9)
	addiu	$3,$3,%lo($LC9)
	sw	$3,72($sp)
	sw	$2,76($sp)
	lw	$3,72($sp)
	lw	$2,76($sp)
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
	sw	$3,72($sp)
	sw	$2,76($sp)
	sw	$4,80($sp)
	lw	$2,80($sp)
	bne	$2,$0,$L96
	lui	$2,%hi($LC8)
	addiu	$4,$2,%lo($LC8)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	lw	$2,272($sp)
	j	$L120
$L96:
	lw	$2,232($sp)
	lui	$3,%hi($LC10)
	addiu	$3,$3,%lo($LC10)
	sw	$3,84($sp)
	sw	$2,88($sp)
	lw	$3,84($sp)
	lw	$2,88($sp)
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
	sw	$3,84($sp)
	sw	$2,88($sp)
	sw	$4,92($sp)
	lw	$2,92($sp)
	bne	$2,$0,$L98
	j	$L99
$L98:
	lw	$2,232($sp)
	lui	$3,%hi($LC11)
	addiu	$3,$3,%lo($LC11)
	sw	$3,96($sp)
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
	bne	$2,$0,$L101
	lui	$2,%hi(gpMonitor)
	lw	$2,%lo(gpMonitor)($2)
	move	$3,$2
	lui	$2,%hi(gpMonitorParams)
	sw	$3,%lo(gpMonitorParams)($2)
	j	$L102
$L101:
	lw	$2,232($sp)
	lui	$3,%hi($LC12)
	addiu	$3,$3,%lo($LC12)
	sw	$3,108($sp)
	sw	$2,112($sp)
	lw	$3,108($sp)
	lw	$2,112($sp)
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
	sw	$3,108($sp)
	sw	$2,112($sp)
	sw	$4,116($sp)
	lw	$2,116($sp)
	bne	$2,$0,$L104
	lui	$2,%hi(gpMonitor)
	lw	$2,%lo(gpMonitor)($2)
	addiu	$3,$2,20
	lui	$2,%hi(gpMonitorParams)
	sw	$3,%lo(gpMonitorParams)($2)
	j	$L102
$L104:
	lw	$3,232($sp)
	lui	$2,%hi($LC13)
	addiu	$4,$2,%lo($LC13)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	lui	$2,%hi($LC8)
	addiu	$4,$2,%lo($LC8)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	lw	$2,272($sp)
	j	$L120
$L102:
	lw	$2,236($sp)
	lui	$3,%hi($LC14)
	addiu	$3,$3,%lo($LC14)
	sw	$3,120($sp)
	sw	$2,124($sp)
	lw	$3,120($sp)
	lw	$2,124($sp)
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
	sw	$3,120($sp)
	sw	$2,124($sp)
	sw	$4,128($sp)
	lw	$2,128($sp)
	bne	$2,$0,$L106
	lw	$3,248($sp)
	li	$2,3			# 0x3
	beq	$3,$2,$L107
	lui	$2,%hi($LC15)
	addiu	$4,$2,%lo($LC15)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	lui	$2,%hi($LC8)
	addiu	$4,$2,%lo($LC8)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	j	$L99
$L107:
	lw	$3,240($sp)
	lui	$2,%hi(gpMonitorParams)
	lw	$2,%lo(gpMonitorParams)($2)
	move	$6,$2
	move	$4,$3
	lui	$2,%hi($LC16)
	addiu	$5,$2,%lo($LC16)
	lui	$2,%hi(sscanf)
	addiu	$2,$2,%lo(sscanf)
	jalr	$2
	lui	$2,%hi(gpMonitorParams)
	lw	$2,%lo(gpMonitorParams)($2)
	lw	$2,0($2)
	sltu	$2,$2,31
	bne	$2,$0,$L108
	lui	$2,%hi(gpMonitorParams)
	lw	$2,%lo(gpMonitorParams)($2)
	li	$3,30			# 0x1e
	sw	$3,0($2)
	j	$L99
$L108:
	j	$L99
$L106:
	lw	$2,236($sp)
	lui	$3,%hi($LC17)
	addiu	$3,$3,%lo($LC17)
	sw	$3,132($sp)
	sw	$2,136($sp)
	lw	$3,132($sp)
	lw	$2,136($sp)
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
	sw	$3,132($sp)
	sw	$2,136($sp)
	sw	$4,140($sp)
	lw	$2,140($sp)
	bne	$2,$0,$L110
	lw	$3,248($sp)
	li	$2,3			# 0x3
	beq	$3,$2,$L111
	lui	$2,%hi($LC18)
	addiu	$4,$2,%lo($LC18)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	lui	$2,%hi($LC8)
	addiu	$4,$2,%lo($LC8)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	j	$L99
$L111:
	lw	$4,240($sp)
	lui	$2,%hi(gpMonitorParams)
	lw	$2,%lo(gpMonitorParams)($2)
	addiu	$3,$2,4
	lui	$2,%hi($LC16)
	addiu	$5,$2,%lo($LC16)
	move	$6,$3
	lui	$2,%hi(sscanf)
	addiu	$2,$2,%lo(sscanf)
	jalr	$2
	j	$L99
$L110:
	lw	$2,236($sp)
	lui	$3,%hi($LC19)
	addiu	$3,$3,%lo($LC19)
	sw	$3,144($sp)
	sw	$2,148($sp)
	lw	$3,144($sp)
	lw	$2,148($sp)
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
	sw	$3,144($sp)
	sw	$2,148($sp)
	sw	$4,152($sp)
	lw	$2,152($sp)
	bne	$2,$0,$L113
	lw	$3,248($sp)
	li	$2,3			# 0x3
	beq	$3,$2,$L114
	lui	$2,%hi($LC20)
	addiu	$4,$2,%lo($LC20)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	lui	$2,%hi($LC8)
	addiu	$4,$2,%lo($LC8)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	j	$L99
$L114:
	lw	$4,240($sp)
	lui	$2,%hi(gpMonitorParams)
	lw	$2,%lo(gpMonitorParams)($2)
	addiu	$3,$2,8
	lui	$2,%hi($LC16)
	addiu	$5,$2,%lo($LC16)
	move	$6,$3
	lui	$2,%hi(sscanf)
	addiu	$2,$2,%lo(sscanf)
	jalr	$2
	lui	$2,%hi(gpMonitorParams)
	lw	$2,%lo(gpMonitorParams)($2)
	lw	$2,8($2)
	bne	$2,$0,$L115
	lui	$2,%hi(gpPhyData)
	lw	$2,%lo(gpPhyData)($2)
	lw	$3,60($2)
	li	$2,100663296			# 0x6000000
	and	$3,$3,$2
	li	$2,33554432			# 0x2000000
	bne	$3,$2,$L116
	lui	$2,%hi(gpMonitorParams)
	lw	$2,%lo(gpMonitorParams)($2)
	li	$3,200			# 0xc8
	sw	$3,8($2)
	j	$L115
$L116:
	lui	$2,%hi(gpMonitorParams)
	lw	$2,%lo(gpMonitorParams)($2)
	li	$3,600			# 0x258
	sw	$3,8($2)
$L115:
	lui	$2,%hi(gpMonitor)
	lw	$2,%lo(gpMonitor)($2)
	lui	$3,%hi(gpMonitorParams)
	lw	$3,%lo(gpMonitorParams)($3)
	lw	$3,8($3)
	sw	$3,204($2)
	lui	$2,%hi(gpMonitorParams)
	lw	$2,%lo(gpMonitorParams)($2)
	lui	$3,%hi(gpMonitorParams)
	lw	$3,%lo(gpMonitorParams)($3)
	lw	$4,12($3)
	lui	$3,%hi(gpMonitorParams)
	lw	$3,%lo(gpMonitorParams)($3)
	lw	$3,8($3)
	divu	$0,$4,$3
	mfhi	$4
	mflo	$3
	sw	$3,16($2)
	j	$L99
$L113:
	lw	$2,236($sp)
	lui	$3,%hi($LC21)
	addiu	$3,$3,%lo($LC21)
	sw	$3,156($sp)
	sw	$2,160($sp)
	lw	$3,156($sp)
	lw	$2,160($sp)
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
	sw	$3,156($sp)
	sw	$2,160($sp)
	sw	$4,164($sp)
	lw	$2,164($sp)
	bne	$2,$0,$L118
	lw	$3,248($sp)
	li	$2,3			# 0x3
	beq	$3,$2,$L119
	lui	$2,%hi($LC20)
	addiu	$4,$2,%lo($LC20)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	lui	$2,%hi($LC8)
	addiu	$4,$2,%lo($LC8)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	j	$L99
$L119:
	lw	$4,240($sp)
	lui	$2,%hi(gpMonitorParams)
	lw	$2,%lo(gpMonitorParams)($2)
	addiu	$3,$2,12
	lui	$2,%hi($LC16)
	addiu	$5,$2,%lo($LC16)
	move	$6,$3
	lui	$2,%hi(sscanf)
	addiu	$2,$2,%lo(sscanf)
	jalr	$2
	lui	$2,%hi(gpMonitorParams)
	lw	$2,%lo(gpMonitorParams)($2)
	lui	$3,%hi(gpMonitorParams)
	lw	$3,%lo(gpMonitorParams)($3)
	lw	$4,12($3)
	lui	$3,%hi(gpMonitorParams)
	lw	$3,%lo(gpMonitorParams)($3)
	lw	$3,8($3)
	divu	$0,$4,$3
	mfhi	$4
	mflo	$3
	sw	$3,16($2)
	j	$L99
$L118:
	lw	$3,236($sp)
	lui	$2,%hi($LC22)
	addiu	$4,$2,%lo($LC22)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	lui	$2,%hi($LC8)
	addiu	$4,$2,%lo($LC8)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	lw	$2,272($sp)
	j	$L120
$L99:
	lui	$2,%hi(gpMonitor)
	lw	$2,%lo(gpMonitor)($2)
	lw	$9,0($2)
	lui	$2,%hi(gpMonitor)
	lw	$2,%lo(gpMonitor)($2)
	lw	$8,4($2)
	lui	$2,%hi(gpMonitor)
	lw	$2,%lo(gpMonitor)($2)
	lw	$7,8($2)
	lui	$2,%hi(gpMonitor)
	lw	$2,%lo(gpMonitor)($2)
	lw	$6,12($2)
	lui	$2,%hi(gpMonitor)
	lw	$2,%lo(gpMonitor)($2)
	lw	$5,20($2)
	lui	$2,%hi(gpMonitor)
	lw	$2,%lo(gpMonitor)($2)
	lw	$4,24($2)
	lui	$2,%hi(gpMonitor)
	lw	$2,%lo(gpMonitor)($2)
	lw	$3,28($2)
	lui	$2,%hi(gpMonitor)
	lw	$2,%lo(gpMonitor)($2)
	lw	$2,32($2)
	sw	$6,16($sp)
	sw	$5,20($sp)
	sw	$4,24($sp)
	sw	$3,28($sp)
	sw	$2,32($sp)
	lui	$2,%hi($LC23)
	addiu	$4,$2,%lo($LC23)
	move	$5,$9
	move	$6,$8
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	lw	$2,272($sp)
$L120:
	lw	$31,260($sp)
	addiu	$sp,$sp,264
	j	$31
	.end	xpon_omci_oam_monitor_write_proc
	.size	xpon_omci_oam_monitor_write_proc, .-xpon_omci_oam_monitor_write_proc
	.section	.text.omci_oam_monitor_init,"ax",@progbits
	.align	2
	.globl	omci_oam_monitor_init
	.set	nomips16
	.set	nomicromips
	.ent	omci_oam_monitor_init
	.type	omci_oam_monitor_init, @function
omci_oam_monitor_init:
	.frame	$sp,24,$31		# vars= 0, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-24
	sw	$31,20($sp)
	sw	$4,24($sp)
	lw	$2,24($sp)
	bne	$2,$0,$L122
	nop

	j	$L121
	nop

$L122:
	lui	$2,%hi(gpMonitor)
	lw	$3,24($sp)
	sw	$3,%lo(gpMonitor)($2)
	lui	$2,%hi(gpMonitor)
	lw	$2,%lo(gpMonitor)($2)
	move	$4,$2
	move	$5,$0
	li	$6,212			# 0xd4
	lui	$2,%hi(memset)
	addiu	$2,$2,%lo(memset)
	jalr	$2
	nop

	lui	$2,%hi(gpMonitor)
	lw	$2,%lo(gpMonitor)($2)
	addiu	$2,$2,184
	move	$4,$2
	move	$5,$0
	move	$6,$0
	lui	$2,%hi(init_timer_key)
	addiu	$2,$2,%lo(init_timer_key)
	jalr	$2
	nop

	lui	$2,%hi(gpMonitor)
	lw	$2,%lo(gpMonitor)($2)
	li	$3,1000			# 0x3e8
	sw	$3,204($2)
	lui	$2,%hi(gpMonitor)
	lw	$2,%lo(gpMonitor)($2)
	lui	$3,%hi(xpon_count_rx_pkt)
	addiu	$3,$3,%lo(xpon_count_rx_pkt)
	sw	$3,200($2)
	lui	$2,%hi(gpMonitor)
	lw	$2,%lo(gpMonitor)($2)
	li	$3,15			# 0xf
	sw	$3,0($2)
	lui	$2,%hi(gpMonitor)
	lw	$2,%lo(gpMonitor)($2)
	li	$3,1			# 0x1
	sw	$3,4($2)
	lui	$2,%hi(gpMonitor)
	lw	$2,%lo(gpMonitor)($2)
	li	$3,200			# 0xc8
	sw	$3,8($2)
	lui	$2,%hi(gpMonitor)
	lw	$2,%lo(gpMonitor)($2)
	li	$3,35000			# 0x88b8
	sw	$3,12($2)
	lui	$2,%hi(gpMonitor)
	lw	$2,%lo(gpMonitor)($2)
	lui	$3,%hi(gpMonitor)
	lw	$3,%lo(gpMonitor)($3)
	lw	$4,12($3)
	lui	$3,%hi(gpMonitor)
	lw	$3,%lo(gpMonitor)($3)
	lw	$3,8($3)
	divu	$0,$4,$3
	mfhi	$4
	mflo	$3
	sw	$3,16($2)
	lui	$2,%hi(gpMonitor)
	lw	$2,%lo(gpMonitor)($2)
	li	$3,10			# 0xa
	sw	$3,20($2)
	lui	$2,%hi(gpMonitor)
	lw	$2,%lo(gpMonitor)($2)
	li	$3,30			# 0x1e
	sw	$3,24($2)
	lui	$2,%hi(gpMonitor)
	lw	$2,%lo(gpMonitor)($2)
	li	$3,600			# 0x258
	sw	$3,28($2)
	lui	$2,%hi(gpMonitor)
	lw	$2,%lo(gpMonitor)($2)
	li	$3,15000			# 0x3a98
	sw	$3,32($2)
	lui	$2,%hi(gpMonitor)
	lw	$2,%lo(gpMonitor)($2)
	lui	$3,%hi(gpMonitor)
	lw	$3,%lo(gpMonitor)($3)
	lw	$4,32($3)
	lui	$3,%hi(gpMonitor)
	lw	$3,%lo(gpMonitor)($3)
	lw	$3,28($3)
	divu	$0,$4,$3
	mfhi	$4
	mflo	$3
	sw	$3,36($2)
	lui	$2,%hi(gpMonitor)
	lw	$2,%lo(gpMonitor)($2)
	sw	$0,172($2)
	lui	$2,%hi(gpMonitor)
	lw	$2,%lo(gpMonitor)($2)
	move	$3,$2
	lui	$2,%hi(gpMonitorParams)
	sw	$3,%lo(gpMonitorParams)($2)
$L121:
	lw	$31,20($sp)
	addiu	$sp,$sp,24
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	omci_oam_monitor_init
	.size	omci_oam_monitor_init, .-omci_oam_monitor_init
	.rdata
	.align	2
	.type	__func__.30823, @object
	.size	__func__.30823, 23
__func__.30823:
	.ascii	"start_omci_oam_monitor\000"
	.align	2
	.type	__func__.30828, @object
	.size	__func__.30828, 13
__func__.30828:
	.ascii	"monitor_done\000"
	.align	2
	.type	__func__.30834, @object
	.size	__func__.30834, 18
__func__.30834:
	.ascii	"xpon_count_rx_pkt\000"
	.ident	"GCC: (Buildroot 2015.08.1) 4.9.3"
