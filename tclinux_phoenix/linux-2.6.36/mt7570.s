	.file	1 "mt7570.c"
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
	.globl	mt7570_version
	.data
	.align	2
	.type	mt7570_version, @object
	.size	mt7570_version, 4
mt7570_version:
	.word	107
	.globl	internal_DDMI
	.align	2
	.type	internal_DDMI, @object
	.size	internal_DDMI, 4
internal_DDMI:
	.word	1
	.globl	fast_DDMI
	.section	.bss,"aw",@nobits
	.align	2
	.type	fast_DDMI, @object
	.size	fast_DDMI, 4
fast_DDMI:
	.space	4
	.globl	TEC
	.data
	.align	2
	.type	TEC, @object
	.size	TEC, 4
TEC:
	.word	1
	.globl	TEC_switch
	.section	.bss
	.align	2
	.type	TEC_switch, @object
	.size	TEC_switch, 4
TEC_switch:
	.space	4
	.globl	xPON_mode
	.data
	.align	2
	.type	xPON_mode, @object
	.size	xPON_mode, 4
xPON_mode:
	.word	-1
	.globl	mt7570_select
	.section	.bss
	.align	2
	.type	mt7570_select, @object
	.size	mt7570_select, 4
mt7570_select:
	.space	4
	.globl	vEN7570
	.data
	.align	2
	.type	vEN7570, @object
	.size	vEN7570, 4
vEN7570:
	.word	1
	.globl	BG1V76
	.section	.bss
	.align	2
	.type	BG1V76, @object
	.size	BG1V76, 4
BG1V76:
	.space	4
	.globl	BG0V875
	.align	2
	.type	BG0V875, @object
	.size	BG0V875, 4
BG0V875:
	.space	4
	.globl	RSSI_Vref
	.align	2
	.type	RSSI_Vref, @object
	.size	RSSI_Vref, 4
RSSI_Vref:
	.space	4
	.globl	RSSI_V
	.align	2
	.type	RSSI_V, @object
	.size	RSSI_V, 4
RSSI_V:
	.space	4
	.globl	ETC
	.align	2
	.type	ETC, @object
	.size	ETC, 4
ETC:
	.space	4
	.globl	FiberPlug
	.align	2
	.type	FiberPlug, @object
	.size	FiberPlug, 4
FiberPlug:
	.space	4
	.globl	PatternEnabled
	.align	2
	.type	PatternEnabled, @object
	.size	PatternEnabled, 4
PatternEnabled:
	.space	4
	.globl	RSSI_factor
	.align	2
	.type	RSSI_factor, @object
	.size	RSSI_factor, 4
RSSI_factor:
	.space	4
	.globl	delay_cnt
	.align	2
	.type	delay_cnt, @object
	.size	delay_cnt, 4
delay_cnt:
	.space	4
	.globl	cnt7570
	.align	2
	.type	cnt7570, @object
	.size	cnt7570, 4
cnt7570:
	.space	4
	.globl	MPD_current_cnt
	.align	2
	.type	MPD_current_cnt, @object
	.size	MPD_current_cnt, 4
MPD_current_cnt:
	.space	4
	.globl	T0C_extension
	.align	2
	.type	T0C_extension, @object
	.size	T0C_extension, 4
T0C_extension:
	.space	4
	.globl	IC_temperature
	.data
	.align	2
	.type	IC_temperature, @object
	.size	IC_temperature, 4
IC_temperature:
	.word	1103626240
	.globl	BOSA_temperature
	.align	2
	.type	BOSA_temperature, @object
	.size	BOSA_temperature, 4
BOSA_temperature:
	.word	1101004800
	.globl	Env_temperature
	.align	2
	.type	Env_temperature, @object
	.size	Env_temperature, 4
Env_temperature:
	.word	1103626240
	.globl	APD_voltage
	.align	2
	.type	APD_voltage, @object
	.size	APD_voltage, 4
APD_voltage:
	.word	1108082688
	.globl	ADC_slope
	.section	.bss
	.align	2
	.type	ADC_slope, @object
	.size	ADC_slope, 4
ADC_slope:
	.space	4
	.globl	ADC_offset
	.align	2
	.type	ADC_offset, @object
	.size	ADC_offset, 4
ADC_offset:
	.space	4
	.globl	global_temperature_code
	.align	2
	.type	global_temperature_code, @object
	.size	global_temperature_code, 4
global_temperature_code:
	.space	4
	.globl	global_vcc_code
	.align	2
	.type	global_vcc_code, @object
	.size	global_vcc_code, 4
global_vcc_code:
	.space	4
	.globl	global_bias_current
	.align	2
	.type	global_bias_current, @object
	.size	global_bias_current, 4
global_bias_current:
	.space	4
	.globl	global_MPDL
	.align	2
	.type	global_MPDL, @object
	.size	global_MPDL, 4
global_MPDL:
	.space	4
	.globl	global_MPDH
	.align	2
	.type	global_MPDH, @object
	.size	global_MPDH, 4
global_MPDH:
	.space	4
	.globl	global_modulation_current
	.align	2
	.type	global_modulation_current, @object
	.size	global_modulation_current, 4
global_modulation_current:
	.space	4
	.globl	global_rssi_current
	.align	2
	.type	global_rssi_current, @object
	.size	global_rssi_current, 4
global_rssi_current:
	.space	4
	.globl	global_mpd_current
	.align	2
	.type	global_mpd_current, @object
	.size	global_mpd_current, 4
global_mpd_current:
	.space	4
	.globl	global_RSSI_current
	.align	2
	.type	global_RSSI_current, @object
	.size	global_RSSI_current, 4
global_RSSI_current:
	.space	4
	.globl	global_MPD_current
	.align	2
	.type	global_MPD_current, @object
	.size	global_MPD_current, 4
global_MPD_current:
	.space	4
	.globl	global_MPD_current_var
	.align	2
	.type	global_MPD_current_var, @object
	.size	global_MPD_current_var, 4
global_MPD_current_var:
	.space	4
	.globl	global_DDMI_rx_cal_flag
	.align	2
	.type	global_DDMI_rx_cal_flag, @object
	.size	global_DDMI_rx_cal_flag, 4
global_DDMI_rx_cal_flag:
	.space	4
	.globl	global_supply_voltage_8472
	.align	1
	.type	global_supply_voltage_8472, @object
	.size	global_supply_voltage_8472, 2
global_supply_voltage_8472:
	.space	2
	.globl	global_IC_temperature_8472
	.align	1
	.type	global_IC_temperature_8472, @object
	.size	global_IC_temperature_8472, 2
global_IC_temperature_8472:
	.space	2
	.globl	global_bias_current_8472
	.align	1
	.type	global_bias_current_8472, @object
	.size	global_bias_current_8472, 2
global_bias_current_8472:
	.space	2
	.globl	DDMI_voltage
	.align	1
	.type	DDMI_voltage, @object
	.size	DDMI_voltage, 2
DDMI_voltage:
	.space	2
	.globl	DDMI_current
	.align	1
	.type	DDMI_current, @object
	.size	DDMI_current, 2
DDMI_current:
	.space	2
	.globl	DDMI_temperature
	.align	1
	.type	DDMI_temperature, @object
	.size	DDMI_temperature, 2
DDMI_temperature:
	.space	2
	.globl	DDMI_tx_power
	.align	1
	.type	DDMI_tx_power, @object
	.size	DDMI_tx_power, 2
DDMI_tx_power:
	.space	2
	.globl	DDMI_rx_power
	.align	1
	.type	DDMI_rx_power, @object
	.size	DDMI_rx_power, 2
DDMI_rx_power:
	.space	2
	.globl	Ibias_outputlow_flag
	.align	2
	.type	Ibias_outputlow_flag, @object
	.size	Ibias_outputlow_flag, 4
Ibias_outputlow_flag:
	.space	4
	.globl	MPD_current_offset
	.align	2
	.type	MPD_current_offset, @object
	.size	MPD_current_offset, 4
MPD_current_offset:
	.space	4
	.globl	T_APD
	.data
	.align	2
	.type	T_APD, @object
	.size	T_APD, 4
T_APD:
	.word	600
	.globl	flash_matrix
	.section	.bss
	.align	2
	.type	flash_matrix, @object
	.size	flash_matrix, 400
flash_matrix:
	.space	400
	.globl	mt7570_alarm
	.align	2
	.type	mt7570_alarm, @object
	.size	mt7570_alarm, 4
mt7570_alarm:
	.space	4
	.globl	SCL
	.align	2
	.type	SCL, @object
	.size	SCL, 4
SCL:
	.space	4
	.globl	DOL
	.align	2
	.type	DOL, @object
	.size	DOL, 4
DOL:
	.space	4
	.globl	BiasTracking_switch
	.align	2
	.type	BiasTracking_switch, @object
	.size	BiasTracking_switch, 4
BiasTracking_switch:
	.space	4
	.globl	BOSA_Ith
	.data
	.align	2
	.type	BOSA_Ith, @object
	.size	BOSA_Ith, 4
BOSA_Ith:
	.word	3000
	.globl	LUT_Ibias_Imod
	.align	2
	.type	LUT_Ibias_Imod, @object
	.size	LUT_Ibias_Imod, 512
LUT_Ibias_Imod:
	.word	262
	.word	946
	.word	266
	.word	946
	.word	271
	.word	950
	.word	280
	.word	955
	.word	285
	.word	955
	.word	289
	.word	960
	.word	298
	.word	964
	.word	303
	.word	964
	.word	307
	.word	969
	.word	316
	.word	973
	.word	321
	.word	973
	.word	326
	.word	978
	.word	334
	.word	982
	.word	343
	.word	984
	.word	356
	.word	996
	.word	369
	.word	1005
	.word	382
	.word	1014
	.word	395
	.word	1023
	.word	408
	.word	1032
	.word	421
	.word	1041
	.word	429
	.word	1046
	.word	442
	.word	1055
	.word	455
	.word	1064
	.word	468
	.word	1073
	.word	481
	.word	1082
	.word	494
	.word	1092
	.word	507
	.word	1101
	.word	532
	.word	1114
	.word	547
	.word	1128
	.word	566
	.word	1142
	.word	581
	.word	1160
	.word	600
	.word	1173
	.word	616
	.word	1187
	.word	631
	.word	1201
	.word	650
	.word	1219
	.word	665
	.word	1233
	.word	684
	.word	1246
	.word	699
	.word	1260
	.word	719
	.word	1278
	.word	766
	.word	1296
	.word	818
	.word	1319
	.word	866
	.word	1342
	.word	918
	.word	1365
	.word	966
	.word	1387
	.word	1018
	.word	1410
	.word	1131
	.word	1465
	.word	1245
	.word	1519
	.word	1358
	.word	1574
	.word	1476
	.word	1628
	.word	1589
	.word	1683
	.word	1638
	.word	1742
	.word	1638
	.word	1742
	.word	1638
	.word	1742
	.word	1638
	.word	1742
	.word	1638
	.word	1742
	.word	1638
	.word	1742
	.word	1638
	.word	1742
	.word	1638
	.word	1742
	.word	1638
	.word	1742
	.word	1638
	.word	1742
	.word	1638
	.word	1742
	.word	1638
	.word	1742
	.word	1638
	.word	1742
	.word	1638
	.word	1742
	.globl	i2c_protect
	.section	.bss
	.align	2
	.type	i2c_protect, @object
	.size	i2c_protect, 4
i2c_protect:
	.space	4
	.section	.text.i2c_access_protect,"ax",@progbits
	.align	2
	.globl	i2c_access_protect
	.set	nomips16
	.set	nomicromips
	.ent	i2c_access_protect
	.type	i2c_access_protect, @function
i2c_access_protect:
	.frame	$sp,32,$31		# vars= 8, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-32
	sw	$31,28($sp)
	li	$2,10000			# 0x2710
	sw	$2,16($sp)
	j	$L2
	nop

$L5:
	li	$4,1			# 0x1
	lui	$2,%hi(__udelay)
	addiu	$2,$2,%lo(__udelay)
	jalr	$2
	nop

	lw	$2,16($sp)
	bne	$2,$0,$L3
	nop

	li	$2,-1			# 0xffffffffffffffff
	j	$L4
	nop

$L3:
	lw	$2,16($sp)
	addiu	$2,$2,-1
	sw	$2,16($sp)
$L2:
	lui	$2,%hi(i2c_protect)
	addiu	$2,$2,%lo(i2c_protect)
	lw	$3,0($2)
	li	$2,1			# 0x1
	beq	$3,$2,$L5
	nop

	move	$2,$0
$L4:
	lw	$31,28($sp)
	addiu	$sp,$sp,32
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	i2c_access_protect
	.size	i2c_access_protect, .-i2c_access_protect
	.rdata
	.align	2
$LC0:
	.ascii	"EN7570 reg 0x%.3x\000"
	.align	2
$LC1:
	.ascii	"\011%.8x\012\000"
	.section	.text.mt7570_register_dump,"ax",@progbits
	.align	2
	.globl	mt7570_register_dump
	.set	nomips16
	.set	nomicromips
	.ent	mt7570_register_dump
	.type	mt7570_register_dump, @function
mt7570_register_dump:
	.frame	$sp,56,$31		# vars= 16, regs= 1/0, args= 32, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-56
	sw	$31,52($sp)
	sw	$4,56($sp)
	sw	$0,32($sp)
	sw	$0,36($sp)
	sw	$0,32($sp)
	j	$L7
	nop

$L8:
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,116($2)
	move	$3,$2
	lw	$2,32($sp)
	sll	$2,$2,2
	sw	$2,16($sp)
	addiu	$2,$sp,40
	sw	$2,20($sp)
	li	$2,4			# 0x4
	sw	$2,24($sp)
	move	$4,$0
	move	$5,$3
	li	$6,112			# 0x70
	li	$7,2			# 0x2
	lui	$2,%hi(phy_I2C_read)
	addiu	$2,$2,%lo(phy_I2C_read)
	jalr	$2
	nop

	lbu	$2,40($sp)
	move	$3,$2
	lbu	$2,41($sp)
	sll	$2,$2,8
	or	$3,$3,$2
	lbu	$2,42($sp)
	sll	$2,$2,16
	or	$3,$3,$2
	lbu	$2,43($sp)
	sll	$2,$2,24
	or	$2,$3,$2
	sw	$2,36($sp)
	lw	$2,32($sp)
	sll	$3,$2,2
	lui	$2,%hi($LC0)
	addiu	$4,$2,%lo($LC0)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi($LC1)
	addiu	$4,$2,%lo($LC1)
	lw	$5,36($sp)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lw	$2,32($sp)
	addiu	$2,$2,1
	sw	$2,32($sp)
$L7:
	lw	$3,32($sp)
	lw	$2,56($sp)
	slt	$2,$3,$2
	bne	$2,$0,$L8
	nop

	lw	$31,52($sp)
	addiu	$sp,$sp,56
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	mt7570_register_dump
	.size	mt7570_register_dump, .-mt7570_register_dump
	.section	.text.get_flash_register,"ax",@progbits
	.align	2
	.globl	get_flash_register
	.set	nomips16
	.set	nomicromips
	.ent	get_flash_register
	.type	get_flash_register, @function
get_flash_register:
	.frame	$sp,8,$31		# vars= 8, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-8
	sw	$4,8($sp)
	sw	$0,0($sp)
	lw	$2,8($sp)
	sra	$3,$2,2
	lui	$2,%hi(flash_matrix)
	sll	$3,$3,2
	addiu	$2,$2,%lo(flash_matrix)
	addu	$2,$3,$2
	lw	$2,0($2)
	sw	$2,0($sp)
	lw	$2,0($sp)
	addiu	$sp,$sp,8
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	get_flash_register
	.size	get_flash_register, .-get_flash_register
	.rdata
	.align	2
$LC2:
	.ascii	"/tmp/7570_bob.conf\000"
	.align	2
$LC3:
	.ascii	"--> Error opening \012\000"
	.section	.text.get_flash_matrix,"ax",@progbits
	.align	2
	.globl	get_flash_matrix
	.set	nomips16
	.set	nomicromips
	.ent	get_flash_matrix
	.type	get_flash_matrix, @function
get_flash_matrix:
	.frame	$sp,40,$31		# vars= 16, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-40
	sw	$31,36($sp)
	sw	$0,16($sp)
	sw	$0,20($sp)
	lui	$2,%hi($LC2)
	addiu	$2,$2,%lo($LC2)
	sw	$2,20($sp)
	move	$2,$28
	lw	$2,24($2)
	sw	$2,28($sp)
	move	$2,$28
	sw	$0,24($2)
	lw	$2,20($sp)
	beq	$2,$0,$L12
	nop

	lw	$2,20($sp)
	lb	$2,0($2)
	beq	$2,$0,$L12
	nop

	lw	$4,20($sp)
	move	$5,$0
	move	$6,$0
	lui	$2,%hi(filp_open)
	addiu	$2,$2,%lo(filp_open)
	jalr	$2
	nop

	sw	$2,16($sp)
	lw	$2,16($sp)
	sw	$2,24($sp)
	lw	$2,24($sp)
	sltu	$2,$2,-4095
	xori	$2,$2,0x1
	andi	$2,$2,0x00ff
	beq	$2,$0,$L14
	nop

	lui	$2,%hi($LC3)
	addiu	$4,$2,%lo($LC3)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	nop
$L15 = .
	move	$3,$28
	lw	$2,28($sp)
	sw	$2,24($3)
	li	$2,-1			# 0xffffffffffffffff
	j	$L17
	nop

$L14:
	lw	$2,16($sp)
	lw	$2,16($2)
	lw	$2,8($2)
	lw	$3,16($sp)
	addiu	$7,$3,40
	lw	$4,16($sp)
	lui	$3,%hi(flash_matrix)
	addiu	$5,$3,%lo(flash_matrix)
	li	$6,400			# 0x190
	jalr	$2
	nop

	lw	$4,16($sp)
	move	$5,$0
	lui	$2,%hi(filp_close)
	addiu	$2,$2,%lo(filp_close)
	jalr	$2
	nop

$L12:
	move	$3,$28
	lw	$2,28($sp)
	sw	$2,24($3)
	move	$2,$0
$L17:
	lw	$31,36($sp)
	addiu	$sp,$sp,40
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	get_flash_matrix
	.size	get_flash_matrix, .-get_flash_matrix
	.section	.text.set_flash_register,"ax",@progbits
	.align	2
	.globl	set_flash_register
	.set	nomips16
	.set	nomicromips
	.ent	set_flash_register
	.type	set_flash_register, @function
set_flash_register:
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	sw	$4,0($sp)
	sw	$5,4($sp)
	lw	$2,4($sp)
	srl	$3,$2,2
	lui	$2,%hi(flash_matrix)
	sll	$3,$3,2
	addiu	$2,$2,%lo(flash_matrix)
	addu	$2,$3,$2
	lw	$3,0($sp)
	sw	$3,0($2)
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	set_flash_register
	.size	set_flash_register, .-set_flash_register
	.section	.text.set_flash_register_default,"ax",@progbits
	.align	2
	.globl	set_flash_register_default
	.set	nomips16
	.set	nomicromips
	.ent	set_flash_register_default
	.type	set_flash_register_default, @function
set_flash_register_default:
	.frame	$sp,8,$31		# vars= 8, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-8
	sw	$0,0($sp)
	sw	$0,0($sp)
	j	$L20
	nop

$L21:
	lui	$2,%hi(flash_matrix)
	lw	$3,0($sp)
	sll	$3,$3,2
	addiu	$2,$2,%lo(flash_matrix)
	addu	$2,$3,$2
	li	$3,-1			# 0xffffffffffffffff
	sw	$3,0($2)
	lw	$2,0($sp)
	addiu	$2,$2,1
	sw	$2,0($sp)
$L20:
	lw	$2,0($sp)
	slt	$2,$2,40
	bne	$2,$0,$L21
	nop

	addiu	$sp,$sp,8
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	set_flash_register_default
	.size	set_flash_register_default, .-set_flash_register_default
	.rdata
	.align	2
$LC4:
	.ascii	"Ibias = 0x%x\012\000"
	.align	2
$LC5:
	.ascii	"Imod = 0x%x\012\000"
	.section	.text.set_flash_register_BiasModCurrent,"ax",@progbits
	.align	2
	.globl	set_flash_register_BiasModCurrent
	.set	nomips16
	.set	nomicromips
	.ent	set_flash_register_BiasModCurrent
	.type	set_flash_register_BiasModCurrent, @function
set_flash_register_BiasModCurrent:
	.frame	$sp,32,$31		# vars= 8, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-32
	sw	$31,28($sp)
	li	$4,3			# 0x3
	lui	$2,%hi(mt7570_information_output)
	addiu	$2,$2,%lo(mt7570_information_output)
	jalr	$2
	nop

	sw	$2,16($sp)
	li	$4,4			# 0x4
	lui	$2,%hi(mt7570_information_output)
	addiu	$2,$2,%lo(mt7570_information_output)
	jalr	$2
	nop

	sw	$2,20($sp)
	lw	$4,16($sp)
	move	$5,$0
	lui	$2,%hi(set_flash_register)
	addiu	$2,$2,%lo(set_flash_register)
	jalr	$2
	nop

	lw	$4,20($sp)
	li	$5,4			# 0x4
	lui	$2,%hi(set_flash_register)
	addiu	$2,$2,%lo(set_flash_register)
	jalr	$2
	nop

	lui	$2,%hi($LC4)
	addiu	$4,$2,%lo($LC4)
	lw	$5,16($sp)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi($LC5)
	addiu	$4,$2,%lo($LC5)
	lw	$5,20($sp)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lw	$31,28($sp)
	addiu	$sp,$sp,32
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	set_flash_register_BiasModCurrent
	.size	set_flash_register_BiasModCurrent, .-set_flash_register_BiasModCurrent
	.rdata
	.align	2
$LC6:
	.ascii	"P0 = 0x%x\012\000"
	.align	2
$LC7:
	.ascii	"P1 = 0x%x\012\000"
	.section	.text.set_flash_register_P0P1,"ax",@progbits
	.align	2
	.globl	set_flash_register_P0P1
	.set	nomips16
	.set	nomicromips
	.ent	set_flash_register_P0P1
	.type	set_flash_register_P0P1, @function
set_flash_register_P0P1:
	.frame	$sp,32,$31		# vars= 8, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-32
	sw	$31,28($sp)
	li	$4,5			# 0x5
	lui	$2,%hi(mt7570_information_output)
	addiu	$2,$2,%lo(mt7570_information_output)
	jalr	$2
	nop

	sw	$2,16($sp)
	li	$4,6			# 0x6
	lui	$2,%hi(mt7570_information_output)
	addiu	$2,$2,%lo(mt7570_information_output)
	jalr	$2
	nop

	sw	$2,20($sp)
	lw	$4,16($sp)
	li	$5,8			# 0x8
	lui	$2,%hi(set_flash_register)
	addiu	$2,$2,%lo(set_flash_register)
	jalr	$2
	nop

	lw	$4,20($sp)
	li	$5,12			# 0xc
	lui	$2,%hi(set_flash_register)
	addiu	$2,$2,%lo(set_flash_register)
	jalr	$2
	nop

	lui	$2,%hi($LC6)
	addiu	$4,$2,%lo($LC6)
	lw	$5,16($sp)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi($LC7)
	addiu	$4,$2,%lo($LC7)
	lw	$5,20($sp)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lw	$31,28($sp)
	addiu	$sp,$sp,32
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	set_flash_register_P0P1
	.size	set_flash_register_P0P1, .-set_flash_register_P0P1
	.rdata
	.align	2
$LC8:
	.ascii	"slope up = 0x%x\012\000"
	.align	2
$LC9:
	.ascii	"slope down = 0x%x\012\000"
	.align	2
$LC10:
	.ascii	"VAPD@25C = 0x%x\012\000"
	.section	.text.set_flash_register_APD,"ax",@progbits
	.align	2
	.globl	set_flash_register_APD
	.set	nomips16
	.set	nomicromips
	.ent	set_flash_register_APD
	.type	set_flash_register_APD, @function
set_flash_register_APD:
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
	lw	$4,24($sp)
	li	$5,16			# 0x10
	lui	$2,%hi(set_flash_register)
	addiu	$2,$2,%lo(set_flash_register)
	jalr	$2
	nop

	lw	$4,28($sp)
	li	$5,20			# 0x14
	lui	$2,%hi(set_flash_register)
	addiu	$2,$2,%lo(set_flash_register)
	jalr	$2
	nop

	lw	$4,32($sp)
	li	$5,24			# 0x18
	lui	$2,%hi(set_flash_register)
	addiu	$2,$2,%lo(set_flash_register)
	jalr	$2
	nop

	lui	$2,%hi($LC8)
	addiu	$4,$2,%lo($LC8)
	lw	$5,24($sp)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi($LC9)
	addiu	$4,$2,%lo($LC9)
	lw	$5,28($sp)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi($LC10)
	addiu	$4,$2,%lo($LC10)
	lw	$5,32($sp)
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
	.end	set_flash_register_APD
	.size	set_flash_register_APD, .-set_flash_register_APD
	.rdata
	.align	2
$LC11:
	.ascii	"LOS low = 0x%x\012\000"
	.align	2
$LC12:
	.ascii	"LOS high = 0x%x\012\000"
	.section	.text.set_flash_register_LOS,"ax",@progbits
	.align	2
	.globl	set_flash_register_LOS
	.set	nomips16
	.set	nomicromips
	.ent	set_flash_register_LOS
	.type	set_flash_register_LOS, @function
set_flash_register_LOS:
	.frame	$sp,48,$31		# vars= 8, regs= 1/0, args= 32, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-48
	sw	$31,44($sp)
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,116($2)
	move	$3,$2
	li	$2,284			# 0x11c
	sw	$2,16($sp)
	addiu	$2,$sp,32
	sw	$2,20($sp)
	li	$2,4			# 0x4
	sw	$2,24($sp)
	move	$4,$0
	move	$5,$3
	li	$6,112			# 0x70
	li	$7,2			# 0x2
	lui	$2,%hi(phy_I2C_read)
	addiu	$2,$2,%lo(phy_I2C_read)
	jalr	$2
	nop

	lbu	$2,35($sp)
	andi	$2,$2,0x7f
	andi	$2,$2,0x00ff
	sb	$2,35($sp)
	lbu	$2,34($sp)
	andi	$2,$2,0x7f
	andi	$2,$2,0x00ff
	sb	$2,34($sp)
	lbu	$2,35($sp)
	move	$4,$2
	li	$5,36			# 0x24
	lui	$2,%hi(set_flash_register)
	addiu	$2,$2,%lo(set_flash_register)
	jalr	$2
	nop

	lbu	$2,34($sp)
	move	$4,$2
	li	$5,32			# 0x20
	lui	$2,%hi(set_flash_register)
	addiu	$2,$2,%lo(set_flash_register)
	jalr	$2
	nop

	lbu	$2,35($sp)
	move	$3,$2
	lui	$2,%hi($LC11)
	addiu	$4,$2,%lo($LC11)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lbu	$2,34($sp)
	move	$3,$2
	lui	$2,%hi($LC12)
	addiu	$4,$2,%lo($LC12)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lw	$31,44($sp)
	addiu	$sp,$sp,48
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	set_flash_register_LOS
	.size	set_flash_register_LOS, .-set_flash_register_LOS
	.rdata
	.align	2
$LC13:
	.ascii	"0x%08x\012\000"
	.section	.text.set_flash_register_DDMI_TxPower,"ax",@progbits
	.align	2
	.globl	set_flash_register_DDMI_TxPower
	.set	nomips16
	.set	nomicromips
	.ent	set_flash_register_DDMI_TxPower
	.type	set_flash_register_DDMI_TxPower, @function
set_flash_register_DDMI_TxPower:
	.frame	$sp,32,$31		# vars= 8, regs= 2/0, args= 16, gp= 0
	.mask	0x80010000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-32
	sw	$31,28($sp)
	sw	$16,24($sp)
	sw	$4,32($sp)
	sw	$5,36($sp)
	sw	$0,16($sp)
	lw	$2,32($sp)
	sll	$16,$2,16
	lui	$2,%hi(mt7570_MPD_current)
	addiu	$2,$2,%lo(mt7570_MPD_current)
	jalr	$2
	nop

	andi	$2,$2,0xffff
	or	$2,$16,$2
	sw	$2,16($sp)
	lw	$4,16($sp)
	lw	$5,36($sp)
	lui	$2,%hi(set_flash_register)
	addiu	$2,$2,%lo(set_flash_register)
	jalr	$2
	nop

	lui	$2,%hi(vEN7570)
	lw	$2,%lo(vEN7570)($2)
	bne	$2,$0,$L27
	nop

	lw	$3,36($sp)
	li	$2,64			# 0x40
	bne	$3,$2,$L27
	nop

	lw	$2,16($sp)
	andi	$2,$2,0xffff
	move	$4,$2
	li	$5,136			# 0x88
	lui	$2,%hi(set_flash_register)
	addiu	$2,$2,%lo(set_flash_register)
	jalr	$2
	nop

$L27:
	lui	$2,%hi($LC13)
	addiu	$4,$2,%lo($LC13)
	lw	$5,16($sp)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lw	$31,28($sp)
	lw	$16,24($sp)
	addiu	$sp,$sp,32
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	set_flash_register_DDMI_TxPower
	.size	set_flash_register_DDMI_TxPower, .-set_flash_register_DDMI_TxPower
	.section	.text.set_flash_register_DDMI_RxPower,"ax",@progbits
	.align	2
	.globl	set_flash_register_DDMI_RxPower
	.set	nomips16
	.set	nomicromips
	.ent	set_flash_register_DDMI_RxPower
	.type	set_flash_register_DDMI_RxPower, @function
set_flash_register_DDMI_RxPower:
	.frame	$sp,32,$31		# vars= 8, regs= 2/0, args= 16, gp= 0
	.mask	0x80010000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-32
	sw	$31,28($sp)
	sw	$16,24($sp)
	sw	$4,32($sp)
	sw	$5,36($sp)
	sw	$0,16($sp)
	lw	$2,32($sp)
	sll	$16,$2,16
	lui	$2,%hi(mt7570_RSSI_current)
	addiu	$2,$2,%lo(mt7570_RSSI_current)
	jalr	$2
	nop

	andi	$2,$2,0xffff
	or	$2,$16,$2
	sw	$2,16($sp)
	lw	$4,16($sp)
	lw	$5,36($sp)
	lui	$2,%hi(set_flash_register)
	addiu	$2,$2,%lo(set_flash_register)
	jalr	$2
	nop

	lui	$2,%hi($LC13)
	addiu	$4,$2,%lo($LC13)
	lw	$5,16($sp)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi(global_DDMI_rx_cal_flag)
	sw	$0,%lo(global_DDMI_rx_cal_flag)($2)
	lw	$31,28($sp)
	lw	$16,24($sp)
	addiu	$sp,$sp,32
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	set_flash_register_DDMI_RxPower
	.size	set_flash_register_DDMI_RxPower, .-set_flash_register_DDMI_RxPower
	.rdata
	.align	2
$LC14:
	.ascii	"TIAGAIN = 0x%x\012\000"
	.section	.text.set_flash_register_TIAGAIN,"ax",@progbits
	.align	2
	.globl	set_flash_register_TIAGAIN
	.set	nomips16
	.set	nomicromips
	.ent	set_flash_register_TIAGAIN
	.type	set_flash_register_TIAGAIN, @function
set_flash_register_TIAGAIN:
	.frame	$sp,48,$31		# vars= 8, regs= 1/0, args= 32, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-48
	sw	$31,44($sp)
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,116($2)
	move	$3,$2
	li	$2,1			# 0x1
	sw	$2,16($sp)
	addiu	$2,$sp,32
	sw	$2,20($sp)
	li	$2,1			# 0x1
	sw	$2,24($sp)
	move	$4,$0
	move	$5,$3
	li	$6,112			# 0x70
	li	$7,2			# 0x2
	lui	$2,%hi(phy_I2C_read)
	addiu	$2,$2,%lo(phy_I2C_read)
	jalr	$2
	nop

	lbu	$2,32($sp)
	srl	$2,$2,6
	andi	$2,$2,0x00ff
	sb	$2,32($sp)
	lbu	$2,32($sp)
	move	$4,$2
	li	$5,40			# 0x28
	lui	$2,%hi(set_flash_register)
	addiu	$2,$2,%lo(set_flash_register)
	jalr	$2
	nop

	lbu	$2,32($sp)
	move	$3,$2
	lui	$2,%hi($LC14)
	addiu	$4,$2,%lo($LC14)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lw	$31,44($sp)
	addiu	$sp,$sp,48
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	set_flash_register_TIAGAIN
	.size	set_flash_register_TIAGAIN, .-set_flash_register_TIAGAIN
	.rdata
	.align	2
$LC15:
	.ascii	"Env_temperature = %d.%.1dC\012\000"
	.globl	__mulsf3
	.globl	__fixunssfsi
	.align	2
$LC17:
	.ascii	"Env_temperature_offset = %d.%.1dC\012\000"
	.section	.text.set_flash_register_EnvTemp,"ax",@progbits
	.align	2
	.globl	set_flash_register_EnvTemp
	.set	nomips16
	.set	nomicromips
	.ent	set_flash_register_EnvTemp
	.type	set_flash_register_EnvTemp, @function
set_flash_register_EnvTemp:
	.frame	$sp,32,$31		# vars= 8, regs= 2/0, args= 16, gp= 0
	.mask	0x80010000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-32
	sw	$31,28($sp)
	sw	$16,24($sp)
	sw	$4,32($sp)
	sw	$0,16($sp)
	sw	$0,20($sp)
	lw	$4,32($sp)
	li	$5,96			# 0x60
	lui	$2,%hi(set_flash_register)
	addiu	$2,$2,%lo(set_flash_register)
	jalr	$2
	nop

	lw	$3,32($sp)
	li	$2,-859045888			# 0xffffffffcccc0000
	ori	$2,$2,0xcccd
	multu	$3,$2
	mfhi	$2
	srl	$5,$2,3
	lw	$3,32($sp)
	li	$2,-859045888			# 0xffffffffcccc0000
	ori	$2,$2,0xcccd
	multu	$3,$2
	mfhi	$2
	srl	$2,$2,3
	sll	$2,$2,1
	sll	$4,$2,2
	addu	$2,$2,$4
	subu	$2,$3,$2
	lui	$3,%hi($LC15)
	addiu	$4,$3,%lo($LC15)
	move	$6,$2
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi(mt7570_temperature_get)
	addiu	$2,$2,%lo(mt7570_temperature_get)
	jalr	$2
	nop

	lui	$2,%hi(IC_temperature)
	lw	$4,%lo(IC_temperature)($2)
	lui	$3,%hi($LC16)
	lui	$2,%hi(__mulsf3)
	addiu	$2,$2,%lo(__mulsf3)
	lw	$5,%lo($LC16)($3)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(__fixunssfsi)
	addiu	$2,$2,%lo(__fixunssfsi)
	move	$4,$3
	jalr	$2
	nop

	move	$3,$2
	lw	$2,32($sp)
	subu	$2,$3,$2
	sw	$2,20($sp)
	lw	$3,20($sp)
	li	$2,-859045888			# 0xffffffffcccc0000
	ori	$2,$2,0xcccd
	multu	$3,$2
	mfhi	$2
	srl	$5,$2,3
	lw	$3,20($sp)
	li	$2,-859045888			# 0xffffffffcccc0000
	ori	$2,$2,0xcccd
	multu	$3,$2
	mfhi	$2
	srl	$2,$2,3
	sll	$2,$2,1
	sll	$4,$2,2
	addu	$2,$2,$4
	subu	$2,$3,$2
	lui	$3,%hi($LC17)
	addiu	$4,$3,%lo($LC17)
	move	$6,$2
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lw	$2,20($sp)
	sll	$16,$2,16
	li	$4,132			# 0x84
	lui	$2,%hi(get_flash_register)
	addiu	$2,$2,%lo(get_flash_register)
	jalr	$2
	nop

	andi	$2,$2,0xffff
	or	$2,$16,$2
	sw	$2,16($sp)
	lw	$4,16($sp)
	li	$5,132			# 0x84
	lui	$2,%hi(set_flash_register)
	addiu	$2,$2,%lo(set_flash_register)
	jalr	$2
	nop

	lw	$31,28($sp)
	lw	$16,24($sp)
	addiu	$sp,$sp,32
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	set_flash_register_EnvTemp
	.size	set_flash_register_EnvTemp, .-set_flash_register_EnvTemp
	.rdata
	.align	2
$LC18:
	.ascii	"Please input temperature value in range : 0 ~ 0x500\012\000"
	.align	2
$LC19:
	.ascii	"Temperature slope = 0x%x\012\000"
	.globl	__floatunsisf
	.globl	__subsf3
	.globl	__extendsfdf2
	.globl	__adddf3
	.globl	__fixunsdfsi
	.align	2
$LC21:
	.ascii	"Temperature offset = 0x%x\012\000"
	.section	.text.set_flash_register_temperature_offset,"ax",@progbits
	.align	2
	.globl	set_flash_register_temperature_offset
	.set	nomips16
	.set	nomicromips
	.ent	set_flash_register_temperature_offset
	.type	set_flash_register_temperature_offset, @function
set_flash_register_temperature_offset:
	.frame	$sp,32,$31		# vars= 8, regs= 2/0, args= 16, gp= 0
	.mask	0x80010000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-32
	sw	$31,28($sp)
	sw	$16,24($sp)
	sw	$4,32($sp)
	sw	$5,36($sp)
	lw	$2,32($sp)
	sltu	$2,$2,1281
	bne	$2,$0,$L32
	nop

	lui	$2,%hi($LC18)
	addiu	$4,$2,%lo($LC18)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	j	$L31
	nop

$L32:
	lw	$2,36($sp)
	bne	$2,$0,$L34
	nop

	li	$2,65535			# 0xffff
	sw	$2,36($sp)
	j	$L35
	nop

$L34:
	lui	$2,%hi($LC19)
	addiu	$4,$2,%lo($LC19)
	lw	$5,36($sp)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L35:
	lw	$2,36($sp)
	sll	$2,$2,16
	ori	$2,$2,0xffff
	sw	$2,16($sp)
	lw	$4,16($sp)
	li	$5,128			# 0x80
	lui	$2,%hi(set_flash_register)
	addiu	$2,$2,%lo(set_flash_register)
	jalr	$2
	nop

	lui	$2,%hi(mt7570_temperature_get)
	addiu	$2,$2,%lo(mt7570_temperature_get)
	jalr	$2
	nop

	lui	$2,%hi(__floatunsisf)
	addiu	$2,$2,%lo(__floatunsisf)
	lw	$4,32($sp)
	jalr	$2
	nop

	move	$16,$2
	lui	$2,%hi(IC_temperature)
	lw	$4,%lo(IC_temperature)($2)
	lui	$3,%hi($LC16)
	lui	$2,%hi(__mulsf3)
	addiu	$2,$2,%lo(__mulsf3)
	lw	$5,%lo($LC16)($3)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(__subsf3)
	addiu	$2,$2,%lo(__subsf3)
	move	$4,$16
	move	$5,$3
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(__extendsfdf2)
	addiu	$2,$2,%lo(__extendsfdf2)
	move	$4,$3
	jalr	$2
	nop

	move	$5,$3
	move	$4,$2
	lui	$3,%hi($LC20)
	lui	$2,%hi(__adddf3)
	addiu	$2,$2,%lo(__adddf3)
	lw	$7,%lo($LC20+4)($3)
	lw	$6,%lo($LC20)($3)
	jalr	$2
	nop

	move	$5,$3
	move	$4,$2
	lui	$2,%hi(__fixunsdfsi)
	addiu	$2,$2,%lo(__fixunsdfsi)
	jalr	$2
	nop

	sw	$2,16($sp)
	lui	$2,%hi($LC21)
	addiu	$4,$2,%lo($LC21)
	lw	$5,16($sp)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lw	$2,16($sp)
	andi	$3,$2,0xffff
	lw	$2,36($sp)
	sll	$2,$2,16
	or	$2,$3,$2
	sw	$2,16($sp)
	lw	$4,16($sp)
	li	$5,128			# 0x80
	lui	$2,%hi(set_flash_register)
	addiu	$2,$2,%lo(set_flash_register)
	jalr	$2
	nop

$L31:
	lw	$31,28($sp)
	lw	$16,24($sp)
	addiu	$sp,$sp,32
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	set_flash_register_temperature_offset
	.size	set_flash_register_temperature_offset, .-set_flash_register_temperature_offset
	.rdata
	.align	2
$LC22:
	.ascii	"T0/T1 delay = 0x%x\012\000"
	.section	.text.set_flash_register_T0T1delay,"ax",@progbits
	.align	2
	.globl	set_flash_register_T0T1delay
	.set	nomips16
	.set	nomicromips
	.ent	set_flash_register_T0T1delay
	.type	set_flash_register_T0T1delay, @function
set_flash_register_T0T1delay:
	.frame	$sp,48,$31		# vars= 8, regs= 1/0, args= 32, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-48
	sw	$31,44($sp)
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,116($2)
	move	$3,$2
	li	$2,8			# 0x8
	sw	$2,16($sp)
	addiu	$2,$sp,32
	sw	$2,20($sp)
	li	$2,1			# 0x1
	sw	$2,24($sp)
	move	$4,$0
	move	$5,$3
	li	$6,112			# 0x70
	li	$7,2			# 0x2
	lui	$2,%hi(phy_I2C_read)
	addiu	$2,$2,%lo(phy_I2C_read)
	jalr	$2
	nop

	lbu	$2,32($sp)
	move	$4,$2
	li	$5,152			# 0x98
	lui	$2,%hi(set_flash_register)
	addiu	$2,$2,%lo(set_flash_register)
	jalr	$2
	nop

	lbu	$2,32($sp)
	move	$3,$2
	lui	$2,%hi($LC22)
	addiu	$4,$2,%lo($LC22)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lw	$31,44($sp)
	addiu	$sp,$sp,48
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	set_flash_register_T0T1delay
	.size	set_flash_register_T0T1delay, .-set_flash_register_T0T1delay
	.rdata
	.align	2
$LC23:
	.ascii	"T0C = 0x%x\012T1C = 0x%x\012\000"
	.section	.text.set_flash_register_T0CT1C,"ax",@progbits
	.align	2
	.globl	set_flash_register_T0CT1C
	.set	nomips16
	.set	nomicromips
	.ent	set_flash_register_T0CT1C
	.type	set_flash_register_T0CT1C, @function
set_flash_register_T0CT1C:
	.frame	$sp,48,$31		# vars= 8, regs= 1/0, args= 32, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-48
	sw	$31,44($sp)
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,116($2)
	move	$3,$2
	li	$2,8			# 0x8
	sw	$2,16($sp)
	addiu	$2,$sp,32
	sw	$2,20($sp)
	li	$2,4			# 0x4
	sw	$2,24($sp)
	move	$4,$0
	move	$5,$3
	li	$6,112			# 0x70
	li	$7,2			# 0x2
	lui	$2,%hi(phy_I2C_read)
	addiu	$2,$2,%lo(phy_I2C_read)
	jalr	$2
	nop

	lbu	$2,33($sp)
	andi	$2,$2,0x7f
	andi	$2,$2,0x00ff
	sb	$2,33($sp)
	lbu	$2,34($sp)
	andi	$2,$2,0x7f
	andi	$2,$2,0x00ff
	sb	$2,34($sp)
	lbu	$2,33($sp)
	sll	$2,$2,16
	lbu	$3,34($sp)
	or	$2,$2,$3
	move	$4,$2
	li	$5,156			# 0x9c
	lui	$2,%hi(set_flash_register)
	addiu	$2,$2,%lo(set_flash_register)
	jalr	$2
	nop

	lbu	$2,34($sp)
	move	$5,$2
	lbu	$2,33($sp)
	move	$3,$2
	lui	$2,%hi($LC23)
	addiu	$4,$2,%lo($LC23)
	move	$6,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lw	$31,44($sp)
	addiu	$sp,$sp,48
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	set_flash_register_T0CT1C
	.size	set_flash_register_T0CT1C, .-set_flash_register_T0CT1C
	.section	.text.set_flash_register_Tx_data,"ax",@progbits
	.align	2
	.globl	set_flash_register_Tx_data
	.set	nomips16
	.set	nomicromips
	.ent	set_flash_register_Tx_data
	.type	set_flash_register_Tx_data, @function
set_flash_register_Tx_data:
	.frame	$sp,24,$31		# vars= 0, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-24
	sw	$31,20($sp)
	lui	$2,%hi(set_flash_register_BiasModCurrent)
	addiu	$2,$2,%lo(set_flash_register_BiasModCurrent)
	jalr	$2
	nop

	lui	$2,%hi(set_flash_register_P0P1)
	addiu	$2,$2,%lo(set_flash_register_P0P1)
	jalr	$2
	nop

	lui	$2,%hi(set_flash_register_TIAGAIN)
	addiu	$2,$2,%lo(set_flash_register_TIAGAIN)
	jalr	$2
	nop

	lui	$2,%hi(set_flash_register_T0T1delay)
	addiu	$2,$2,%lo(set_flash_register_T0T1delay)
	jalr	$2
	nop

	lui	$2,%hi(set_flash_register_T0CT1C)
	addiu	$2,$2,%lo(set_flash_register_T0CT1C)
	jalr	$2
	nop

	lw	$31,20($sp)
	addiu	$sp,$sp,24
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	set_flash_register_Tx_data
	.size	set_flash_register_Tx_data, .-set_flash_register_Tx_data
	.rdata
	.align	2
$LC24:
	.ascii	"--> Error write \012\000"
	.section	.text.save_flash_matrix,"ax",@progbits
	.align	2
	.globl	save_flash_matrix
	.set	nomips16
	.set	nomicromips
	.ent	save_flash_matrix
	.type	save_flash_matrix, @function
save_flash_matrix:
	.frame	$sp,40,$31		# vars= 16, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-40
	sw	$31,36($sp)
	sw	$0,16($sp)
	sw	$0,20($sp)
	lui	$2,%hi($LC2)
	addiu	$2,$2,%lo($LC2)
	sw	$2,20($sp)
	move	$2,$28
	lw	$2,24($2)
	sw	$2,28($sp)
	move	$2,$28
	sw	$0,24($2)
	lw	$2,20($sp)
	beq	$2,$0,$L40
	nop

	lw	$2,20($sp)
	lb	$2,0($2)
	beq	$2,$0,$L40
	nop

	lw	$4,20($sp)
	li	$5,258			# 0x102
	move	$6,$0
	lui	$2,%hi(filp_open)
	addiu	$2,$2,%lo(filp_open)
	jalr	$2
	nop

	sw	$2,16($sp)
	lw	$2,16($sp)
	sw	$2,24($sp)
	lw	$2,24($sp)
	sltu	$2,$2,-4095
	xori	$2,$2,0x1
	andi	$2,$2,0x00ff
	beq	$2,$0,$L42
	nop

	lui	$2,%hi($LC3)
	addiu	$4,$2,%lo($LC3)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	j	$L43
	nop

$L42:
	lw	$2,16($sp)
	move	$5,$0
	move	$4,$0
	sw	$5,44($2)
	sw	$4,40($2)
	lw	$2,16($sp)
	lw	$2,16($2)
	lw	$2,12($2)
	lw	$3,16($sp)
	addiu	$7,$3,40
	lw	$4,16($sp)
	lui	$3,%hi(flash_matrix)
	addiu	$5,$3,%lo(flash_matrix)
	li	$6,400			# 0x190
	jalr	$2
	nop

	bgtz	$2,$L44
	nop

	lui	$2,%hi($LC24)
	addiu	$4,$2,%lo($LC24)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lw	$4,16($sp)
	move	$5,$0
	lui	$2,%hi(filp_close)
	addiu	$2,$2,%lo(filp_close)
	jalr	$2
	nop

	j	$L43
	nop

$L44:
	lw	$4,16($sp)
	move	$5,$0
	lui	$2,%hi(filp_close)
	addiu	$2,$2,%lo(filp_close)
	jalr	$2
	nop

$L40:
	move	$3,$28
	lw	$2,28($sp)
	sw	$2,24($3)
	move	$2,$0
	j	$L46
	nop

$L43:
	move	$3,$28
	lw	$2,28($sp)
	sw	$2,24($3)
	li	$2,-1			# 0xffffffffffffffff
$L46:
	lw	$31,36($sp)
	addiu	$sp,$sp,40
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	save_flash_matrix
	.size	save_flash_matrix, .-save_flash_matrix
	.rdata
	.align	2
$LC25:
	.ascii	"FLASH address 0x%x\000"
	.align	2
$LC26:
	.ascii	"\011\000"
	.section	.text.flash_dump,"ax",@progbits
	.align	2
	.globl	flash_dump
	.set	nomips16
	.set	nomicromips
	.ent	flash_dump
	.type	flash_dump, @function
flash_dump:
	.frame	$sp,32,$31		# vars= 8, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-32
	sw	$31,28($sp)
	sw	$0,16($sp)
	sw	$0,20($sp)
	sw	$0,16($sp)
	j	$L48
	nop

$L49:
	lw	$2,16($sp)
	sll	$2,$2,2
	sw	$2,20($sp)
	lui	$2,%hi($LC25)
	addiu	$4,$2,%lo($LC25)
	lw	$5,20($sp)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi($LC26)
	addiu	$4,$2,%lo($LC26)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lw	$4,20($sp)
	lui	$2,%hi(get_flash_register)
	addiu	$2,$2,%lo(get_flash_register)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi($LC13)
	addiu	$4,$2,%lo($LC13)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lw	$2,16($sp)
	addiu	$2,$2,1
	sw	$2,16($sp)
$L48:
	lw	$2,16($sp)
	slt	$2,$2,40
	bne	$2,$0,$L49
	nop

	lw	$31,28($sp)
	addiu	$sp,$sp,32
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	flash_dump
	.size	flash_dump, .-flash_dump
	.rdata
	.align	2
$LC27:
	.ascii	"FLASH\000"
	.align	2
$LC28:
	.ascii	"temperature\000"
	.align	2
$LC29:
	.ascii	"Value\012\000"
	.align	2
$LC30:
	.ascii	"0x%x\000"
	.align	2
$LC31:
	.ascii	"%d\000"
	.section	.text.DOL_flash_dump,"ax",@progbits
	.align	2
	.globl	DOL_flash_dump
	.set	nomips16
	.set	nomicromips
	.ent	DOL_flash_dump
	.type	DOL_flash_dump, @function
DOL_flash_dump:
	.frame	$sp,32,$31		# vars= 8, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-32
	sw	$31,28($sp)
	sw	$0,16($sp)
	sw	$0,20($sp)
	lui	$2,%hi($LC27)
	addiu	$4,$2,%lo($LC27)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi($LC26)
	addiu	$4,$2,%lo($LC26)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi($LC28)
	addiu	$4,$2,%lo($LC28)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi($LC26)
	addiu	$4,$2,%lo($LC26)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi($LC29)
	addiu	$4,$2,%lo($LC29)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	sw	$0,16($sp)
	j	$L51
	nop

$L52:
	lw	$2,16($sp)
	addiu	$2,$2,40
	sll	$2,$2,2
	sw	$2,20($sp)
	lui	$2,%hi($LC30)
	addiu	$4,$2,%lo($LC30)
	lw	$5,20($sp)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi($LC26)
	addiu	$4,$2,%lo($LC26)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lw	$2,16($sp)
	sll	$2,$2,1
	sll	$3,$2,2
	addu	$2,$2,$3
	addiu	$3,$2,-40
	lui	$2,%hi($LC31)
	addiu	$4,$2,%lo($LC31)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi($LC26)
	addiu	$4,$2,%lo($LC26)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi($LC26)
	addiu	$4,$2,%lo($LC26)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lw	$4,20($sp)
	lui	$2,%hi(get_flash_register)
	addiu	$2,$2,%lo(get_flash_register)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi($LC13)
	addiu	$4,$2,%lo($LC13)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lw	$2,16($sp)
	addiu	$2,$2,1
	sw	$2,16($sp)
$L51:
	lw	$2,16($sp)
	slt	$2,$2,16
	bne	$2,$0,$L52
	nop

	lw	$31,28($sp)
	addiu	$sp,$sp,32
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	DOL_flash_dump
	.size	DOL_flash_dump, .-DOL_flash_dump
	.rdata
	.align	2
$LC32:
	.ascii	"GPON_Tx_calibration done\012\000"
	.section	.text.GPON_Tx_calibration,"ax",@progbits
	.align	2
	.globl	GPON_Tx_calibration
	.set	nomips16
	.set	nomicromips
	.ent	GPON_Tx_calibration
	.type	GPON_Tx_calibration, @function
GPON_Tx_calibration:
	.frame	$sp,24,$31		# vars= 0, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-24
	sw	$31,20($sp)
	lui	$2,%hi(mt7570_ERC_filter)
	addiu	$2,$2,%lo(mt7570_ERC_filter)
	jalr	$2
	nop

	lui	$2,%hi(mt7570_load_init_current)
	addiu	$2,$2,%lo(mt7570_load_init_current)
	jalr	$2
	nop

	li	$4,1			# 0x1
	li	$5,1			# 0x1
	lui	$2,%hi(phy_mode_config)
	addiu	$2,$2,%lo(phy_mode_config)
	jalr	$2
	nop

	li	$4,1			# 0x1
	lui	$2,%hi(mt7570_TGEN)
	addiu	$2,$2,%lo(mt7570_TGEN)
	jalr	$2
	nop

	li	$4,1			# 0x1
	lui	$2,%hi(mt7570_calibration_pattern)
	addiu	$2,$2,%lo(mt7570_calibration_pattern)
	jalr	$2
	nop

	lui	$2,%hi(mt7570_safe_circuit_reset)
	addiu	$2,$2,%lo(mt7570_safe_circuit_reset)
	jalr	$2
	nop

	lui	$2,%hi($LC32)
	addiu	$4,$2,%lo($LC32)
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
	.end	GPON_Tx_calibration
	.size	GPON_Tx_calibration, .-GPON_Tx_calibration
	.rdata
	.align	2
$LC33:
	.ascii	"EPON_Tx_calibration done\012\000"
	.section	.text.EPON_Tx_calibration,"ax",@progbits
	.align	2
	.globl	EPON_Tx_calibration
	.set	nomips16
	.set	nomicromips
	.ent	EPON_Tx_calibration
	.type	EPON_Tx_calibration, @function
EPON_Tx_calibration:
	.frame	$sp,24,$31		# vars= 0, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-24
	sw	$31,20($sp)
	lui	$2,%hi(mt7570_ERC_filter)
	addiu	$2,$2,%lo(mt7570_ERC_filter)
	jalr	$2
	nop

	lui	$2,%hi(mt7570_load_init_current)
	addiu	$2,$2,%lo(mt7570_load_init_current)
	jalr	$2
	nop

	move	$4,$0
	li	$5,1			# 0x1
	lui	$2,%hi(phy_mode_config)
	addiu	$2,$2,%lo(phy_mode_config)
	jalr	$2
	nop

	move	$4,$0
	lui	$2,%hi(mt7570_TGEN)
	addiu	$2,$2,%lo(mt7570_TGEN)
	jalr	$2
	nop

	move	$4,$0
	lui	$2,%hi(mt7570_calibration_pattern)
	addiu	$2,$2,%lo(mt7570_calibration_pattern)
	jalr	$2
	nop

	lui	$2,%hi(mt7570_safe_circuit_reset)
	addiu	$2,$2,%lo(mt7570_safe_circuit_reset)
	jalr	$2
	nop

	lui	$2,%hi($LC33)
	addiu	$4,$2,%lo($LC33)
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
	.end	EPON_Tx_calibration
	.size	EPON_Tx_calibration, .-EPON_Tx_calibration
	.rdata
	.align	2
$LC34:
	.ascii	"ECNT_HOOK_ERROR occur with event id:%d. %s:%d\012\000"
	.align	2
$LC35:
	.ascii	"Pattern Aligned\012\000"
	.align	2
$LC36:
	.ascii	"Aligned Failed\012\000"
	.section	.text.GPON_BER,"ax",@progbits
	.align	2
	.globl	GPON_BER
	.set	nomips16
	.set	nomicromips
	.ent	GPON_BER
	.type	GPON_BER, @function
GPON_BER:
	.frame	$sp,72,$31		# vars= 48, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-72
	sw	$31,68($sp)
	sw	$4,72($sp)
	sw	$0,28($sp)
	li	$4,1			# 0x1
	lui	$2,%hi(mt7570_CDR)
	addiu	$2,$2,%lo(mt7570_CDR)
	jalr	$2
	nop

	li	$2,259			# 0x103
	sw	$2,32($sp)
	sw	$0,44($sp)
	sw	$0,48($sp)
	sw	$0,52($sp)
	sw	$0,56($sp)
	sw	$0,60($sp)
	lw	$2,32($sp)
	sw	$2,48($sp)
	sw	$0,52($sp)
	addiu	$2,$sp,44
	sw	$2,56($sp)
	addiu	$2,$sp,52
	li	$4,24			# 0x18
	move	$5,$0
	move	$6,$2
	lui	$2,%hi(__ECNT_HOOK)
	addiu	$2,$2,%lo(__ECNT_HOOK)
	jalr	$2
	nop

	move	$3,$2
	li	$2,-1			# 0xffffffffffffffff
	bne	$3,$2,$L56
	nop

	lui	$2,%hi($LC34)
	addiu	$4,$2,%lo($LC34)
	lw	$5,32($sp)
	lui	$2,%hi(__func__.24637)
	addiu	$6,$2,%lo(__func__.24637)
	li	$7,107			# 0x6b
	lui	$2,%hi(panic)
	addiu	$2,$2,%lo(panic)
	jalr	$2
	nop

$L56:
	li	$2,100			# 0x64
	sw	$2,16($sp)
	j	$L57
	nop

$L58:
	li	$4,1000			# 0x3e8
	lui	$2,%hi(__udelay)
	addiu	$2,$2,%lo(__udelay)
	jalr	$2
	nop

$L57:
	lw	$2,16($sp)
	addiu	$3,$2,-1
	sw	$3,16($sp)
	bne	$2,$0,$L58
	nop

	li	$4,1			# 0x1
	li	$5,1			# 0x1
	lui	$2,%hi(phy_mode_config)
	addiu	$2,$2,%lo(phy_mode_config)
	jalr	$2
	nop

	li	$2,100			# 0x64
	sw	$2,20($sp)
	j	$L59
	nop

$L60:
	li	$4,1000			# 0x3e8
	lui	$2,%hi(__udelay)
	addiu	$2,$2,%lo(__udelay)
	jalr	$2
	nop

$L59:
	lw	$2,20($sp)
	addiu	$3,$2,-1
	sw	$3,20($sp)
	bne	$2,$0,$L60
	nop

	move	$4,$0
	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$5,$2,0x4a0
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	lw	$2,72($sp)
	li	$3,6			# 0x6
	beq	$2,$3,$L62
	nop

	li	$3,7			# 0x7
	beq	$2,$3,$L63
	nop

	li	$3,5			# 0x5
	beq	$2,$3,$L64
	nop

	j	$L65
	nop

$L64:
	li	$2,8454144			# 0x810000
	ori	$4,$2,0xfd53
	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$5,$2,0x2a0
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	li	$2,16711680			# 0xff0000
	ori	$4,$2,0xffff
	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$5,$2,0x2a4
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	li	$4,5			# 0x5
	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$5,$2,0x4a0
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	li	$4,1			# 0x1
	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$5,$2,0x4a4
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	j	$L65
	nop

$L62:
	li	$2,7405568			# 0x710000
	ori	$4,$2,0xfd53
	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$5,$2,0x2a0
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	li	$2,8323072			# 0x7f0000
	ori	$4,$2,0xffff
	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$5,$2,0x2a4
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	li	$4,6			# 0x6
	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$5,$2,0x4a0
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	li	$4,1			# 0x1
	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$5,$2,0x4a4
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	j	$L65
	nop

$L63:
	li	$2,267452416			# 0xff10000
	ori	$4,$2,0xfd53
	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$5,$2,0x2a0
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	li	$2,134152192			# 0x7ff0000
	ori	$4,$2,0xffff
	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$5,$2,0x2a4
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	li	$4,7			# 0x7
	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$5,$2,0x4a0
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	li	$4,1			# 0x1
	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$5,$2,0x4a4
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	nop
$L65:
	li	$2,250			# 0xfa
	sw	$2,24($sp)
	j	$L66
	nop

$L67:
	li	$4,1000			# 0x3e8
	lui	$2,%hi(__udelay)
	addiu	$2,$2,%lo(__udelay)
	jalr	$2
	nop

$L66:
	lw	$2,24($sp)
	addiu	$3,$2,-1
	sw	$3,24($sp)
	bne	$2,$0,$L67
	nop

	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$2,$2,0x2c4
	sw	$2,36($sp)
	lw	$2,36($sp)
	lw	$2,0($2)
	sw	$2,40($sp)
	lw	$2,36($sp)
	lw	$2,0($2)
	sw	$2,40($sp)
	lw	$2,40($sp)
	sw	$2,28($sp)
	lw	$3,28($sp)
	li	$2,5			# 0x5
	bne	$3,$2,$L69
	nop

	lui	$2,%hi($LC35)
	addiu	$4,$2,%lo($LC35)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	j	$L70
	nop

$L69:
	lui	$2,%hi($LC36)
	addiu	$4,$2,%lo($LC36)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L70:
	lw	$2,28($sp)
	lw	$31,68($sp)
	addiu	$sp,$sp,72
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	GPON_BER
	.size	GPON_BER, .-GPON_BER
	.section	.text.EPON_BER,"ax",@progbits
	.align	2
	.globl	EPON_BER
	.set	nomips16
	.set	nomicromips
	.ent	EPON_BER
	.type	EPON_BER, @function
EPON_BER:
	.frame	$sp,72,$31		# vars= 48, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-72
	sw	$31,68($sp)
	sw	$4,72($sp)
	sw	$0,28($sp)
	li	$4,1			# 0x1
	lui	$2,%hi(mt7570_CDR)
	addiu	$2,$2,%lo(mt7570_CDR)
	jalr	$2
	nop

	li	$2,259			# 0x103
	sw	$2,32($sp)
	sw	$0,44($sp)
	sw	$0,48($sp)
	sw	$0,52($sp)
	sw	$0,56($sp)
	sw	$0,60($sp)
	lw	$2,32($sp)
	sw	$2,48($sp)
	sw	$0,52($sp)
	addiu	$2,$sp,44
	sw	$2,56($sp)
	addiu	$2,$sp,52
	li	$4,24			# 0x18
	move	$5,$0
	move	$6,$2
	lui	$2,%hi(__ECNT_HOOK)
	addiu	$2,$2,%lo(__ECNT_HOOK)
	jalr	$2
	nop

	move	$3,$2
	li	$2,-1			# 0xffffffffffffffff
	bne	$3,$2,$L73
	nop

	lui	$2,%hi($LC34)
	addiu	$4,$2,%lo($LC34)
	lw	$5,32($sp)
	lui	$2,%hi(__func__.24637)
	addiu	$6,$2,%lo(__func__.24637)
	li	$7,107			# 0x6b
	lui	$2,%hi(panic)
	addiu	$2,$2,%lo(panic)
	jalr	$2
	nop

$L73:
	li	$2,100			# 0x64
	sw	$2,16($sp)
	j	$L74
	nop

$L75:
	li	$4,1000			# 0x3e8
	lui	$2,%hi(__udelay)
	addiu	$2,$2,%lo(__udelay)
	jalr	$2
	nop

$L74:
	lw	$2,16($sp)
	addiu	$3,$2,-1
	sw	$3,16($sp)
	bne	$2,$0,$L75
	nop

	move	$4,$0
	li	$5,1			# 0x1
	lui	$2,%hi(phy_mode_config)
	addiu	$2,$2,%lo(phy_mode_config)
	jalr	$2
	nop

	li	$2,100			# 0x64
	sw	$2,20($sp)
	j	$L76
	nop

$L77:
	li	$4,1000			# 0x3e8
	lui	$2,%hi(__udelay)
	addiu	$2,$2,%lo(__udelay)
	jalr	$2
	nop

$L76:
	lw	$2,20($sp)
	addiu	$3,$2,-1
	sw	$3,20($sp)
	bne	$2,$0,$L77
	nop

	move	$4,$0
	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$5,$2,0x4a0
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	lw	$2,72($sp)
	li	$3,6			# 0x6
	beq	$2,$3,$L79
	nop

	li	$3,7			# 0x7
	beq	$2,$3,$L80
	nop

	li	$3,5			# 0x5
	beq	$2,$3,$L81
	nop

	j	$L82
	nop

$L81:
	li	$2,8454144			# 0x810000
	ori	$4,$2,0xfd53
	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$5,$2,0x2a0
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	li	$2,16711680			# 0xff0000
	ori	$4,$2,0xffff
	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$5,$2,0x2a4
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	li	$4,5			# 0x5
	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$5,$2,0x4a0
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	li	$4,1			# 0x1
	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$5,$2,0x4a4
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	j	$L82
	nop

$L79:
	li	$2,7405568			# 0x710000
	ori	$4,$2,0xfd53
	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$5,$2,0x2a0
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	li	$2,8323072			# 0x7f0000
	ori	$4,$2,0xffff
	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$5,$2,0x2a4
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	li	$4,6			# 0x6
	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$5,$2,0x4a0
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	li	$4,1			# 0x1
	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$5,$2,0x4a4
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	j	$L82
	nop

$L80:
	li	$2,267452416			# 0xff10000
	ori	$4,$2,0xfd53
	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$5,$2,0x2a0
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	li	$2,134152192			# 0x7ff0000
	ori	$4,$2,0xffff
	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$5,$2,0x2a4
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	li	$4,7			# 0x7
	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$5,$2,0x4a0
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	li	$4,1			# 0x1
	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$5,$2,0x4a4
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	nop
$L82:
	li	$2,250			# 0xfa
	sw	$2,24($sp)
	j	$L83
	nop

$L84:
	li	$4,1000			# 0x3e8
	lui	$2,%hi(__udelay)
	addiu	$2,$2,%lo(__udelay)
	jalr	$2
	nop

$L83:
	lw	$2,24($sp)
	addiu	$3,$2,-1
	sw	$3,24($sp)
	bne	$2,$0,$L84
	nop

	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$2,$2,0x2c4
	sw	$2,36($sp)
	lw	$2,36($sp)
	lw	$2,0($2)
	sw	$2,40($sp)
	lw	$2,36($sp)
	lw	$2,0($2)
	sw	$2,40($sp)
	lw	$2,40($sp)
	sw	$2,28($sp)
	lw	$3,28($sp)
	li	$2,5			# 0x5
	bne	$3,$2,$L86
	nop

	lui	$2,%hi($LC35)
	addiu	$4,$2,%lo($LC35)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	j	$L87
	nop

$L86:
	lui	$2,%hi($LC36)
	addiu	$4,$2,%lo($LC36)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L87:
	lw	$2,28($sp)
	lw	$31,68($sp)
	addiu	$sp,$sp,72
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	EPON_BER
	.size	EPON_BER, .-EPON_BER
	.rdata
	.align	2
$LC37:
	.ascii	"/opt/tclinux_phoenix/modules/private/xpon_phy/src/mt7570"
	.ascii	".c\000"
	.align	2
$LC38:
	.ascii	"[%lu]0ms[%s:%d]\012  ERC restarted\012\000"
	.section	.text.mt7570_restart_ERC,"ax",@progbits
	.align	2
	.globl	mt7570_restart_ERC
	.set	nomips16
	.set	nomicromips
	.ent	mt7570_restart_ERC
	.type	mt7570_restart_ERC, @function
mt7570_restart_ERC:
	.frame	$sp,48,$31		# vars= 8, regs= 2/0, args= 32, gp= 0
	.mask	0x80010000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-48
	sw	$31,44($sp)
	sw	$16,40($sp)
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,116($2)
	move	$3,$2
	li	$2,316			# 0x13c
	sw	$2,16($sp)
	addiu	$2,$sp,32
	sw	$2,20($sp)
	li	$2,4			# 0x4
	sw	$2,24($sp)
	move	$4,$0
	move	$5,$3
	li	$6,112			# 0x70
	li	$7,2			# 0x2
	lui	$2,%hi(phy_I2C_read)
	addiu	$2,$2,%lo(phy_I2C_read)
	jalr	$2
	nop

	lbu	$3,32($sp)
	li	$2,-2			# 0xfffffffffffffffe
	and	$2,$3,$2
	andi	$2,$2,0x00ff
	sb	$2,32($sp)
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,116($2)
	move	$3,$2
	li	$2,316			# 0x13c
	sw	$2,16($sp)
	addiu	$2,$sp,32
	sw	$2,20($sp)
	li	$2,4			# 0x4
	sw	$2,24($sp)
	move	$4,$0
	move	$5,$3
	li	$6,112			# 0x70
	li	$7,2			# 0x2
	lui	$2,%hi(phy_I2C_write)
	addiu	$2,$2,%lo(phy_I2C_write)
	jalr	$2
	nop

	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,116($2)
	move	$3,$2
	li	$2,332			# 0x14c
	sw	$2,16($sp)
	addiu	$2,$sp,32
	sw	$2,20($sp)
	li	$2,4			# 0x4
	sw	$2,24($sp)
	move	$4,$0
	move	$5,$3
	li	$6,112			# 0x70
	li	$7,2			# 0x2
	lui	$2,%hi(phy_I2C_read)
	addiu	$2,$2,%lo(phy_I2C_read)
	jalr	$2
	nop

	lbu	$3,32($sp)
	li	$2,-2			# 0xfffffffffffffffe
	and	$2,$3,$2
	andi	$2,$2,0x00ff
	sb	$2,32($sp)
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,116($2)
	move	$3,$2
	li	$2,332			# 0x14c
	sw	$2,16($sp)
	addiu	$2,$sp,32
	sw	$2,20($sp)
	li	$2,4			# 0x4
	sw	$2,24($sp)
	move	$4,$0
	move	$5,$3
	li	$6,112			# 0x70
	li	$7,2			# 0x2
	lui	$2,%hi(phy_I2C_write)
	addiu	$2,$2,%lo(phy_I2C_write)
	jalr	$2
	nop

	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,116($2)
	move	$3,$2
	li	$2,316			# 0x13c
	sw	$2,16($sp)
	addiu	$2,$sp,32
	sw	$2,20($sp)
	li	$2,4			# 0x4
	sw	$2,24($sp)
	move	$4,$0
	move	$5,$3
	li	$6,112			# 0x70
	li	$7,2			# 0x2
	lui	$2,%hi(phy_I2C_read)
	addiu	$2,$2,%lo(phy_I2C_read)
	jalr	$2
	nop

	lbu	$2,32($sp)
	ori	$2,$2,0x1
	andi	$2,$2,0x00ff
	sb	$2,32($sp)
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,116($2)
	move	$3,$2
	li	$2,316			# 0x13c
	sw	$2,16($sp)
	addiu	$2,$sp,32
	sw	$2,20($sp)
	li	$2,4			# 0x4
	sw	$2,24($sp)
	move	$4,$0
	move	$5,$3
	li	$6,112			# 0x70
	li	$7,2			# 0x2
	lui	$2,%hi(phy_I2C_write)
	addiu	$2,$2,%lo(phy_I2C_write)
	jalr	$2
	nop

	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,116($2)
	move	$3,$2
	li	$2,332			# 0x14c
	sw	$2,16($sp)
	addiu	$2,$sp,32
	sw	$2,20($sp)
	li	$2,4			# 0x4
	sw	$2,24($sp)
	move	$4,$0
	move	$5,$3
	li	$6,112			# 0x70
	li	$7,2			# 0x2
	lui	$2,%hi(phy_I2C_read)
	addiu	$2,$2,%lo(phy_I2C_read)
	jalr	$2
	nop

	lbu	$2,32($sp)
	ori	$2,$2,0x1
	andi	$2,$2,0x00ff
	sb	$2,32($sp)
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,116($2)
	move	$3,$2
	li	$2,332			# 0x14c
	sw	$2,16($sp)
	addiu	$2,$sp,32
	sw	$2,20($sp)
	li	$2,4			# 0x4
	sw	$2,24($sp)
	move	$4,$0
	move	$5,$3
	li	$6,112			# 0x70
	li	$7,2			# 0x2
	lui	$2,%hi(phy_I2C_write)
	addiu	$2,$2,%lo(phy_I2C_write)
	jalr	$2
	nop

	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,136($2)
	andi	$2,$2,0x10
	beq	$2,$0,$L89
	nop

	lui	$2,%hi(jiffies)
	lw	$16,%lo(jiffies)($2)
	lui	$2,%hi($LC37)
	addiu	$4,$2,%lo($LC37)
	li	$5,47			# 0x2f
	lui	$2,%hi(strrchr)
	addiu	$2,$2,%lo(strrchr)
	jalr	$2
	nop

	addiu	$3,$2,1
	lui	$2,%hi($LC38)
	addiu	$4,$2,%lo($LC38)
	move	$5,$16
	move	$6,$3
	li	$7,973			# 0x3cd
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L89:
	lw	$31,44($sp)
	lw	$16,40($sp)
	addiu	$sp,$sp,48
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	mt7570_restart_ERC
	.size	mt7570_restart_ERC, .-mt7570_restart_ERC
	.rdata
	.align	2
$LC39:
	.ascii	"PRBS23 enabled\012\000"
	.align	2
$LC40:
	.ascii	"8B/10B pattern enabled\012\000"
	.section	.text.mt7570_calibration_pattern,"ax",@progbits
	.align	2
	.globl	mt7570_calibration_pattern
	.set	nomips16
	.set	nomicromips
	.ent	mt7570_calibration_pattern
	.type	mt7570_calibration_pattern, @function
mt7570_calibration_pattern:
	.frame	$sp,48,$31		# vars= 24, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-48
	sw	$31,44($sp)
	sw	$4,48($sp)
	li	$2,259			# 0x103
	sw	$2,16($sp)
	sw	$0,20($sp)
	sw	$0,24($sp)
	sw	$0,28($sp)
	sw	$0,32($sp)
	sw	$0,36($sp)
	lw	$2,16($sp)
	sw	$2,24($sp)
	sw	$0,28($sp)
	addiu	$2,$sp,20
	sw	$2,32($sp)
	addiu	$2,$sp,28
	li	$4,24			# 0x18
	move	$5,$0
	move	$6,$2
	lui	$2,%hi(__ECNT_HOOK)
	addiu	$2,$2,%lo(__ECNT_HOOK)
	jalr	$2
	nop

	move	$3,$2
	li	$2,-1			# 0xffffffffffffffff
	bne	$3,$2,$L92
	nop

	lui	$2,%hi($LC34)
	addiu	$4,$2,%lo($LC34)
	lw	$5,16($sp)
	lui	$2,%hi(__func__.24637)
	addiu	$6,$2,%lo(__func__.24637)
	li	$7,107			# 0x6b
	lui	$2,%hi(panic)
	addiu	$2,$2,%lo(panic)
	jalr	$2
	nop

$L92:
	move	$4,$0
	lui	$2,%hi(mt7570_CDR)
	addiu	$2,$2,%lo(mt7570_CDR)
	jalr	$2
	nop

	lw	$3,48($sp)
	li	$2,1			# 0x1
	bne	$3,$2,$L93
	nop

	li	$4,6			# 0x6
	lui	$2,%hi(phy_tx_test_pattern)
	addiu	$2,$2,%lo(phy_tx_test_pattern)
	jalr	$2
	nop

	li	$4,1			# 0x1
	li	$5,1			# 0x1
	lui	$2,%hi(phy_mode_config)
	addiu	$2,$2,%lo(phy_mode_config)
	jalr	$2
	nop

	lui	$2,%hi($LC39)
	addiu	$4,$2,%lo($LC39)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	j	$L94
	nop

$L93:
	lw	$2,48($sp)
	bne	$2,$0,$L94
	nop

	li	$4,11			# 0xb
	lui	$2,%hi(phy_tx_test_pattern)
	addiu	$2,$2,%lo(phy_tx_test_pattern)
	jalr	$2
	nop

	move	$4,$0
	li	$5,1			# 0x1
	lui	$2,%hi(phy_mode_config)
	addiu	$2,$2,%lo(phy_mode_config)
	jalr	$2
	nop

	lui	$2,%hi($LC40)
	addiu	$4,$2,%lo($LC40)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L94:
	lui	$2,%hi(PatternEnabled)
	li	$3,1			# 0x1
	sw	$3,%lo(PatternEnabled)($2)
	lw	$31,44($sp)
	addiu	$sp,$sp,48
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	mt7570_calibration_pattern
	.size	mt7570_calibration_pattern, .-mt7570_calibration_pattern
	.rdata
	.align	2
$LC41:
	.ascii	"Pattern disabled\012\000"
	.section	.text.mt7570_disable_pattern,"ax",@progbits
	.align	2
	.globl	mt7570_disable_pattern
	.set	nomips16
	.set	nomicromips
	.ent	mt7570_disable_pattern
	.type	mt7570_disable_pattern, @function
mt7570_disable_pattern:
	.frame	$sp,48,$31		# vars= 24, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-48
	sw	$31,44($sp)
	move	$4,$0
	lui	$2,%hi(phy_tx_test_pattern)
	addiu	$2,$2,%lo(phy_tx_test_pattern)
	jalr	$2
	nop

	li	$4,12			# 0xc
	lui	$2,%hi(phy_tx_test_pattern)
	addiu	$2,$2,%lo(phy_tx_test_pattern)
	jalr	$2
	nop

	li	$4,1			# 0x1
	lui	$2,%hi(mt7570_CDR)
	addiu	$2,$2,%lo(mt7570_CDR)
	jalr	$2
	nop

	li	$2,260			# 0x104
	sw	$2,16($sp)
	sw	$0,20($sp)
	sw	$0,24($sp)
	sw	$0,28($sp)
	sw	$0,32($sp)
	sw	$0,36($sp)
	lw	$2,16($sp)
	sw	$2,24($sp)
	sw	$0,28($sp)
	addiu	$2,$sp,20
	sw	$2,32($sp)
	addiu	$2,$sp,28
	li	$4,24			# 0x18
	move	$5,$0
	move	$6,$2
	lui	$2,%hi(__ECNT_HOOK)
	addiu	$2,$2,%lo(__ECNT_HOOK)
	jalr	$2
	nop

	move	$3,$2
	li	$2,-1			# 0xffffffffffffffff
	bne	$3,$2,$L96
	nop

	lui	$2,%hi($LC34)
	addiu	$4,$2,%lo($LC34)
	lw	$5,16($sp)
	lui	$2,%hi(__func__.24637)
	addiu	$6,$2,%lo(__func__.24637)
	li	$7,107			# 0x6b
	lui	$2,%hi(panic)
	addiu	$2,$2,%lo(panic)
	jalr	$2
	nop

$L96:
	lui	$2,%hi($LC41)
	addiu	$4,$2,%lo($LC41)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi(PatternEnabled)
	sw	$0,%lo(PatternEnabled)($2)
	lw	$31,44($sp)
	addiu	$sp,$sp,48
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	mt7570_disable_pattern
	.size	mt7570_disable_pattern, .-mt7570_disable_pattern
	.rdata
	.align	2
$LC42:
	.ascii	"CDR disabled\012\000"
	.align	2
$LC43:
	.ascii	"CDR enabled\012\000"
	.section	.text.mt7570_CDR,"ax",@progbits
	.align	2
	.globl	mt7570_CDR
	.set	nomips16
	.set	nomicromips
	.ent	mt7570_CDR
	.type	mt7570_CDR, @function
mt7570_CDR:
	.frame	$sp,48,$31		# vars= 24, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-48
	sw	$31,44($sp)
	sw	$4,48($sp)
	sw	$0,16($sp)
	lw	$2,48($sp)
	bne	$2,$0,$L98
	nop

	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$2,$2,0x100
	sw	$2,20($sp)
	lw	$2,20($sp)
	lw	$2,0($2)
	sw	$2,24($sp)
	lw	$2,20($sp)
	lw	$2,0($2)
	sw	$2,24($sp)
	lw	$2,24($sp)
	sw	$2,16($sp)
	lw	$3,16($sp)
	li	$2,16777216			# 0x1000000
	or	$2,$3,$2
	move	$4,$2
	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$5,$2,0x100
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	lui	$2,%hi($LC42)
	addiu	$4,$2,%lo($LC42)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	j	$L97
	nop

$L98:
	lw	$3,48($sp)
	li	$2,1			# 0x1
	bne	$3,$2,$L97
	nop

	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$2,$2,0x100
	sw	$2,28($sp)
	lw	$2,28($sp)
	lw	$2,0($2)
	sw	$2,32($sp)
	lw	$2,28($sp)
	lw	$2,0($2)
	sw	$2,32($sp)
	lw	$2,32($sp)
	sw	$2,16($sp)
	lw	$3,16($sp)
	li	$2,-16842752			# 0xfffffffffeff0000
	ori	$2,$2,0xffff
	and	$2,$3,$2
	move	$4,$2
	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$5,$2,0x100
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	lui	$2,%hi($LC43)
	addiu	$4,$2,%lo($LC43)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L97:
	lw	$31,44($sp)
	addiu	$sp,$sp,48
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	mt7570_CDR
	.size	mt7570_CDR, .-mt7570_CDR
	.rdata
	.align	2
$LC44:
	.ascii	"i2c_access_queue\000"
	.align	2
$LC45:
	.ascii	"FLASH matrix got\012\000"
	.align	2
$LC46:
	.ascii	"Start GPON Tx Calibration\012\000"
	.align	2
$LC47:
	.ascii	"APD update period is too short, reset to 10s\012\000"
	.align	2
$LC48:
	.ascii	"Start EPON Tx Calibration\012\000"
	.align	2
$LC49:
	.ascii	"Internal DDMI Disabled\012\000"
	.align	2
$LC50:
	.ascii	"Internal DDMI Enabled\012\000"
	.align	2
$LC51:
	.ascii	"TEC Disabled\012\000"
	.align	2
$LC52:
	.ascii	"TEC Enabled, BOSA's Ith = %duA\012\000"
	.align	2
$LC53:
	.ascii	"TEC Enabled\012\000"
	.align	2
$LC54:
	.ascii	"ETC = 1\012\000"
	.align	2
$LC55:
	.ascii	"ETC = 2\012\000"
	.align	2
$LC56:
	.ascii	"ETC = 3\012\000"
	.align	2
$LC57:
	.ascii	"BiasTracking Enabled\012\000"
	.align	2
$LC58:
	.ascii	"ETC Disabled\012\000"
	.align	2
$LC59:
	.ascii	"EN7570 Initialization Done!\012\000"
	.section	.text.mt7570_init,"ax",@progbits
	.align	2
	.globl	mt7570_init
	.set	nomips16
	.set	nomicromips
	.ent	mt7570_init
	.type	mt7570_init, @function
mt7570_init:
	.frame	$sp,48,$31		# vars= 16, regs= 1/0, args= 24, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-48
	sw	$31,44($sp)
	sw	$0,24($sp)
	sw	$0,16($sp)
	lui	$2,%hi($LC44)
	addiu	$4,$2,%lo($LC44)
	li	$5,8			# 0x8
	li	$6,1			# 0x1
	move	$7,$0
	lui	$2,%hi(__alloc_workqueue_key)
	addiu	$2,$2,%lo(__alloc_workqueue_key)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(i2c_access_queue)
	sw	$3,%lo(i2c_access_queue)($2)
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	addiu	$2,$2,188
	sw	$2,28($sp)
	sw	$0,32($sp)
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	li	$3,1280			# 0x500
	sw	$3,188($2)
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	addiu	$2,$2,192
	sw	$2,36($sp)
	lw	$2,36($sp)
	lw	$3,36($sp)
	sw	$3,0($2)
	lw	$2,36($sp)
	lw	$3,36($sp)
	sw	$3,4($2)
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lui	$3,%hi(mt7570_safe_circuit_reset_for_work_queue)
	addiu	$3,$3,%lo(mt7570_safe_circuit_reset_for_work_queue)
	sw	$3,200($2)
	lui	$2,%hi(get_flash_matrix)
	addiu	$2,$2,%lo(get_flash_matrix)
	jalr	$2
	nop

	bne	$2,$0,$L103
	nop

	lui	$2,%hi($LC45)
	addiu	$4,$2,%lo($LC45)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	j	$L104
	nop

$L103:
	lui	$2,%hi(set_flash_register_default)
	addiu	$2,$2,%lo(set_flash_register_default)
	jalr	$2
	nop

$L104:
	lui	$2,%hi(mt7570_sw_reset)
	addiu	$2,$2,%lo(mt7570_sw_reset)
	jalr	$2
	nop

	lui	$2,%hi(mt7570_TIAGAIN_set)
	addiu	$2,$2,%lo(mt7570_TIAGAIN_set)
	jalr	$2
	nop

	lui	$2,%hi(mt7570_ERC_filter)
	addiu	$2,$2,%lo(mt7570_ERC_filter)
	jalr	$2
	nop

	lui	$2,%hi(mt7570_MPD_current_calibration)
	addiu	$2,$2,%lo(mt7570_MPD_current_calibration)
	jalr	$2
	nop

	lui	$2,%hi(mt7570_ADC_calibration)
	addiu	$2,$2,%lo(mt7570_ADC_calibration)
	jalr	$2
	nop

	lui	$2,%hi(mt7570_RSSI_calibration)
	addiu	$2,$2,%lo(mt7570_RSSI_calibration)
	jalr	$2
	nop

	li	$4,148			# 0x94
	lui	$2,%hi(get_flash_register)
	addiu	$2,$2,%lo(get_flash_register)
	jalr	$2
	nop

	sw	$2,24($sp)
	lw	$3,24($sp)
	li	$2,117768192			# 0x7050000
	ori	$2,$2,0x700
	bne	$3,$2,$L105
	nop

	lui	$2,%hi($LC46)
	addiu	$4,$2,%lo($LC46)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	li	$4,1			# 0x1
	li	$5,1			# 0x1
	lui	$2,%hi(phy_mode_config)
	addiu	$2,$2,%lo(phy_mode_config)
	jalr	$2
	nop

	lui	$2,%hi(xPON_mode)
	li	$3,1			# 0x1
	sw	$3,%lo(xPON_mode)($2)
	lui	$2,%hi(mt7570_RSSI_gain_init)
	addiu	$2,$2,%lo(mt7570_RSSI_gain_init)
	jalr	$2
	nop

	lui	$2,%hi(mt7570_LOS_level_set)
	addiu	$2,$2,%lo(mt7570_LOS_level_set)
	jalr	$2
	nop

	li	$4,271			# 0x10f
	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$5,$2,0x138
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	li	$4,1			# 0x1
	lui	$2,%hi(mt7570_TGEN)
	addiu	$2,$2,%lo(mt7570_TGEN)
	jalr	$2
	nop

	lui	$2,%hi(mt7570_load_init_current)
	addiu	$2,$2,%lo(mt7570_load_init_current)
	jalr	$2
	nop

	lui	$2,%hi(mt7570_load_MPDL_MPDH)
	addiu	$2,$2,%lo(mt7570_load_MPDL_MPDH)
	jalr	$2
	nop

	lui	$2,%hi(mt7570_TxSD_level_set)
	addiu	$2,$2,%lo(mt7570_TxSD_level_set)
	jalr	$2
	nop

	lui	$2,%hi(mt7570_APD_initialization)
	addiu	$2,$2,%lo(mt7570_APD_initialization)
	jalr	$2
	nop

	li	$4,28			# 0x1c
	lui	$2,%hi(get_flash_register)
	addiu	$2,$2,%lo(get_flash_register)
	jalr	$2
	nop

	move	$3,$2
	li	$2,-1			# 0xffffffffffffffff
	beq	$3,$2,$L106
	nop

	li	$4,28			# 0x1c
	lui	$2,%hi(get_flash_register)
	addiu	$2,$2,%lo(get_flash_register)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(T_APD)
	sw	$3,%lo(T_APD)($2)
	lui	$2,%hi(T_APD)
	lw	$2,%lo(T_APD)($2)
	sltu	$2,$2,10
	beq	$2,$0,$L106
	nop

	lui	$2,%hi($LC47)
	addiu	$4,$2,%lo($LC47)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi(T_APD)
	li	$3,10			# 0xa
	sw	$3,%lo(T_APD)($2)
$L106:
	lui	$2,%hi(mt7570_APD_control)
	addiu	$2,$2,%lo(mt7570_APD_control)
	jalr	$2
	nop

	j	$L107
	nop

$L105:
	lw	$3,24($sp)
	li	$2,-419102720			# 0xffffffffe7050000
	ori	$2,$2,0x700
	bne	$3,$2,$L108
	nop

	lui	$2,%hi($LC48)
	addiu	$4,$2,%lo($LC48)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	move	$4,$0
	li	$5,1			# 0x1
	lui	$2,%hi(phy_mode_config)
	addiu	$2,$2,%lo(phy_mode_config)
	jalr	$2
	nop

	lui	$2,%hi(xPON_mode)
	sw	$0,%lo(xPON_mode)($2)
	lui	$2,%hi(mt7570_RSSI_gain_init)
	addiu	$2,$2,%lo(mt7570_RSSI_gain_init)
	jalr	$2
	nop

	lui	$2,%hi(mt7570_LOS_level_set)
	addiu	$2,$2,%lo(mt7570_LOS_level_set)
	jalr	$2
	nop

	li	$4,271			# 0x10f
	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$5,$2,0x138
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	move	$4,$0
	lui	$2,%hi(mt7570_TGEN)
	addiu	$2,$2,%lo(mt7570_TGEN)
	jalr	$2
	nop

	lui	$2,%hi(mt7570_load_init_current)
	addiu	$2,$2,%lo(mt7570_load_init_current)
	jalr	$2
	nop

	lui	$2,%hi(mt7570_load_MPDL_MPDH)
	addiu	$2,$2,%lo(mt7570_load_MPDL_MPDH)
	jalr	$2
	nop

	lui	$2,%hi(mt7570_TxSD_level_set)
	addiu	$2,$2,%lo(mt7570_TxSD_level_set)
	jalr	$2
	nop

	j	$L107
	nop

$L108:
	lui	$2,%hi(internal_DDMI)
	sw	$0,%lo(internal_DDMI)($2)
	lui	$2,%hi($LC49)
	addiu	$4,$2,%lo($LC49)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	move	$2,$0
	j	$L109
	nop

$L107:
	li	$4,112			# 0x70
	lui	$2,%hi(get_flash_register)
	addiu	$2,$2,%lo(get_flash_register)
	jalr	$2
	nop

	move	$3,$2
	li	$2,-2147483648			# 0xffffffff80000000
	bne	$3,$2,$L110
	nop

	lui	$2,%hi(SCL)
	li	$3,1			# 0x1
	sw	$3,%lo(SCL)($2)
	lui	$2,%hi(mt7570_SingleClosedLoopMode)
	addiu	$2,$2,%lo(mt7570_SingleClosedLoopMode)
	jalr	$2
	nop

	j	$L111
	nop

$L110:
	li	$4,112			# 0x70
	lui	$2,%hi(get_flash_register)
	addiu	$2,$2,%lo(get_flash_register)
	jalr	$2
	nop

	move	$3,$2
	li	$2,-1073741824			# 0xffffffffc0000000
	bne	$3,$2,$L111
	nop

	lui	$2,%hi(DOL)
	li	$3,1			# 0x1
	sw	$3,%lo(DOL)($2)
	lui	$2,%hi(mt7570_LUT_recover)
	addiu	$2,$2,%lo(mt7570_LUT_recover)
	jalr	$2
	nop

	lui	$2,%hi(mt7570_OpenLoopMode)
	addiu	$2,$2,%lo(mt7570_OpenLoopMode)
	jalr	$2
	nop

$L111:
	lui	$2,%hi(mt7570_RougeONU_clear)
	addiu	$2,$2,%lo(mt7570_RougeONU_clear)
	jalr	$2
	nop

	lui	$2,%hi(mt7570_safe_circuit_reset)
	addiu	$2,$2,%lo(mt7570_safe_circuit_reset)
	jalr	$2
	nop

	li	$4,44			# 0x2c
	lui	$2,%hi(get_flash_register)
	addiu	$2,$2,%lo(get_flash_register)
	jalr	$2
	nop

	bne	$2,$0,$L112
	nop

	lui	$2,%hi(internal_DDMI)
	sw	$0,%lo(internal_DDMI)($2)
	lui	$2,%hi($LC49)
	addiu	$4,$2,%lo($LC49)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	j	$L113
	nop

$L112:
	lui	$2,%hi($LC50)
	addiu	$4,$2,%lo($LC50)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	li	$4,44			# 0x2c
	lui	$2,%hi(get_flash_register)
	addiu	$2,$2,%lo(get_flash_register)
	jalr	$2
	nop

	move	$3,$2
	li	$2,2			# 0x2
	bne	$3,$2,$L113
	nop

	lui	$2,%hi(fast_DDMI)
	li	$3,1			# 0x1
	sw	$3,%lo(fast_DDMI)($2)
$L113:
	li	$4,140			# 0x8c
	lui	$2,%hi(get_flash_register)
	addiu	$2,$2,%lo(get_flash_register)
	jalr	$2
	nop

	bne	$2,$0,$L114
	nop

	lui	$2,%hi(TEC)
	sw	$0,%lo(TEC)($2)
	lui	$2,%hi($LC51)
	addiu	$4,$2,%lo($LC51)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	j	$L115
	nop

$L114:
	li	$4,140			# 0x8c
	lui	$2,%hi(get_flash_register)
	addiu	$2,$2,%lo(get_flash_register)
	jalr	$2
	nop

	move	$3,$2
	li	$2,-1			# 0xffffffffffffffff
	beq	$3,$2,$L116
	nop

	li	$4,140			# 0x8c
	lui	$2,%hi(get_flash_register)
	addiu	$2,$2,%lo(get_flash_register)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(BOSA_Ith)
	sw	$3,%lo(BOSA_Ith)($2)
	lui	$2,%hi(BOSA_Ith)
	lw	$3,%lo(BOSA_Ith)($2)
	lui	$2,%hi($LC52)
	addiu	$4,$2,%lo($LC52)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	j	$L115
	nop

$L116:
	lui	$2,%hi($LC53)
	addiu	$4,$2,%lo($LC53)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L115:
	li	$4,124			# 0x7c
	lui	$2,%hi(get_flash_register)
	addiu	$2,$2,%lo(get_flash_register)
	jalr	$2
	nop

	move	$3,$2
	li	$2,1			# 0x1
	bne	$3,$2,$L117
	nop

	lui	$2,%hi(ETC)
	li	$3,1			# 0x1
	sw	$3,%lo(ETC)($2)
	lui	$2,%hi($LC54)
	addiu	$4,$2,%lo($LC54)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	j	$L118
	nop

$L117:
	li	$4,124			# 0x7c
	lui	$2,%hi(get_flash_register)
	addiu	$2,$2,%lo(get_flash_register)
	jalr	$2
	nop

	move	$3,$2
	li	$2,2			# 0x2
	bne	$3,$2,$L119
	nop

	lui	$2,%hi(ETC)
	li	$3,2			# 0x2
	sw	$3,%lo(ETC)($2)
	lui	$2,%hi($LC55)
	addiu	$4,$2,%lo($LC55)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	j	$L118
	nop

$L119:
	li	$4,124			# 0x7c
	lui	$2,%hi(get_flash_register)
	addiu	$2,$2,%lo(get_flash_register)
	jalr	$2
	nop

	move	$3,$2
	li	$2,3			# 0x3
	bne	$3,$2,$L120
	nop

	lui	$2,%hi(ETC)
	li	$3,3			# 0x3
	sw	$3,%lo(ETC)($2)
	lui	$2,%hi(mt7570_LUT_recover)
	addiu	$2,$2,%lo(mt7570_LUT_recover)
	jalr	$2
	nop

	lui	$2,%hi(mt7570_temperature_get)
	addiu	$2,$2,%lo(mt7570_temperature_get)
	jalr	$2
	nop

	lui	$2,%hi(mt7570_BiasTracking)
	addiu	$2,$2,%lo(mt7570_BiasTracking)
	jalr	$2
	nop

	lui	$2,%hi($LC56)
	addiu	$4,$2,%lo($LC56)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi($LC57)
	addiu	$4,$2,%lo($LC57)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	j	$L118
	nop

$L120:
	lui	$2,%hi($LC58)
	addiu	$4,$2,%lo($LC58)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L118:
	lui	$2,%hi($LC59)
	addiu	$4,$2,%lo($LC59)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	move	$2,$0
$L109:
	lw	$31,44($sp)
	addiu	$sp,$sp,48
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	mt7570_init
	.size	mt7570_init, .-mt7570_init
	.section	.text.mt7570_sw_reset,"ax",@progbits
	.align	2
	.globl	mt7570_sw_reset
	.set	nomips16
	.set	nomicromips
	.ent	mt7570_sw_reset
	.type	mt7570_sw_reset, @function
mt7570_sw_reset:
	.frame	$sp,48,$31		# vars= 8, regs= 1/0, args= 32, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-48
	sw	$31,44($sp)
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,116($2)
	move	$3,$2
	li	$2,768			# 0x300
	sw	$2,16($sp)
	addiu	$2,$sp,32
	sw	$2,20($sp)
	li	$2,4			# 0x4
	sw	$2,24($sp)
	move	$4,$0
	move	$5,$3
	li	$6,112			# 0x70
	li	$7,2			# 0x2
	lui	$2,%hi(phy_I2C_read)
	addiu	$2,$2,%lo(phy_I2C_read)
	jalr	$2
	nop

	lbu	$2,32($sp)
	seb	$3,$2
	li	$2,-8			# 0xfffffffffffffff8
	and	$2,$3,$2
	seb	$2,$2
	ori	$2,$2,0x1
	seb	$2,$2
	andi	$2,$2,0x00ff
	sb	$2,32($sp)
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,116($2)
	move	$3,$2
	li	$2,768			# 0x300
	sw	$2,16($sp)
	addiu	$2,$sp,32
	sw	$2,20($sp)
	li	$2,4			# 0x4
	sw	$2,24($sp)
	move	$4,$0
	move	$5,$3
	li	$6,112			# 0x70
	li	$7,2			# 0x2
	lui	$2,%hi(phy_I2C_write)
	addiu	$2,$2,%lo(phy_I2C_write)
	jalr	$2
	nop

	lw	$31,44($sp)
	addiu	$sp,$sp,48
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	mt7570_sw_reset
	.size	mt7570_sw_reset, .-mt7570_sw_reset
	.section	.text.mt7570_RSSI_gain_init,"ax",@progbits
	.align	2
	.globl	mt7570_RSSI_gain_init
	.set	nomips16
	.set	nomicromips
	.ent	mt7570_RSSI_gain_init
	.type	mt7570_RSSI_gain_init, @function
mt7570_RSSI_gain_init:
	.frame	$sp,48,$31		# vars= 8, regs= 1/0, args= 32, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-48
	sw	$31,44($sp)
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,116($2)
	move	$3,$2
	li	$2,20			# 0x14
	sw	$2,16($sp)
	addiu	$2,$sp,32
	sw	$2,20($sp)
	li	$2,4			# 0x4
	sw	$2,24($sp)
	move	$4,$0
	move	$5,$3
	li	$6,112			# 0x70
	li	$7,2			# 0x2
	lui	$2,%hi(phy_I2C_read)
	addiu	$2,$2,%lo(phy_I2C_read)
	jalr	$2
	nop

	lbu	$2,34($sp)
	seb	$3,$2
	li	$2,-8			# 0xfffffffffffffff8
	and	$2,$3,$2
	seb	$2,$2
	ori	$2,$2,0x5
	seb	$2,$2
	andi	$2,$2,0x00ff
	sb	$2,34($sp)
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,116($2)
	move	$3,$2
	li	$2,20			# 0x14
	sw	$2,16($sp)
	addiu	$2,$sp,32
	sw	$2,20($sp)
	li	$2,4			# 0x4
	sw	$2,24($sp)
	move	$4,$0
	move	$5,$3
	li	$6,112			# 0x70
	li	$7,2			# 0x2
	lui	$2,%hi(phy_I2C_write)
	addiu	$2,$2,%lo(phy_I2C_write)
	jalr	$2
	nop

	lw	$31,44($sp)
	addiu	$sp,$sp,48
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	mt7570_RSSI_gain_init
	.size	mt7570_RSSI_gain_init, .-mt7570_RSSI_gain_init
	.rdata
	.align	2
$LC60:
	.ascii	"T0/T1 delay = 0x%02x\012\000"
	.align	2
$LC61:
	.ascii	"T0/T1 delay = 0x9a\012\000"
	.align	2
$LC62:
	.ascii	"T0/T1 delay = 0x47\012\000"
	.align	2
$LC63:
	.ascii	"RGS_T0C = 0x%x\012RGS_T1C = 0x%x\012\000"
	.align	2
$LC64:
	.ascii	"TGEN done\012\000"
	.section	.text.mt7570_TGEN,"ax",@progbits
	.align	2
	.globl	mt7570_TGEN
	.set	nomips16
	.set	nomicromips
	.ent	mt7570_TGEN
	.type	mt7570_TGEN, @function
mt7570_TGEN:
	.frame	$sp,80,$31		# vars= 40, regs= 1/0, args= 32, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-80
	sw	$31,76($sp)
	sw	$4,80($sp)
	sw	$0,32($sp)
	sw	$0,36($sp)
	sw	$0,40($sp)
	move	$4,$0
	lui	$2,%hi(mt7570_CDR)
	addiu	$2,$2,%lo(mt7570_CDR)
	jalr	$2
	nop

	li	$4,6			# 0x6
	lui	$2,%hi(phy_tx_test_pattern)
	addiu	$2,$2,%lo(phy_tx_test_pattern)
	jalr	$2
	nop

	li	$2,10			# 0xa
	sw	$2,44($sp)
	j	$L124
	nop

$L125:
	li	$4,1000			# 0x3e8
	lui	$2,%hi(__udelay)
	addiu	$2,$2,%lo(__udelay)
	jalr	$2
	nop

$L124:
	lw	$2,44($sp)
	addiu	$3,$2,-1
	sw	$3,44($sp)
	bne	$2,$0,$L125
	nop

	sw	$0,40($sp)
	j	$L126
	nop

$L129:
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,116($2)
	move	$3,$2
	li	$2,8			# 0x8
	sw	$2,16($sp)
	addiu	$2,$sp,68
	sw	$2,20($sp)
	li	$2,4			# 0x4
	sw	$2,24($sp)
	move	$4,$0
	move	$5,$3
	li	$6,112			# 0x70
	li	$7,2			# 0x2
	lui	$2,%hi(phy_I2C_read)
	addiu	$2,$2,%lo(phy_I2C_read)
	jalr	$2
	nop

	lbu	$3,71($sp)
	li	$2,-9			# 0xfffffffffffffff7
	and	$2,$3,$2
	andi	$2,$2,0x00ff
	sb	$2,71($sp)
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,116($2)
	move	$3,$2
	li	$2,8			# 0x8
	sw	$2,16($sp)
	addiu	$2,$sp,68
	sw	$2,20($sp)
	li	$2,4			# 0x4
	sw	$2,24($sp)
	move	$4,$0
	move	$5,$3
	li	$6,112			# 0x70
	li	$7,2			# 0x2
	lui	$2,%hi(phy_I2C_write)
	addiu	$2,$2,%lo(phy_I2C_write)
	jalr	$2
	nop

	li	$4,2			# 0x2
	lui	$2,%hi(__udelay)
	addiu	$2,$2,%lo(__udelay)
	jalr	$2
	nop

	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,116($2)
	move	$3,$2
	li	$2,8			# 0x8
	sw	$2,16($sp)
	addiu	$2,$sp,68
	sw	$2,20($sp)
	li	$2,4			# 0x4
	sw	$2,24($sp)
	move	$4,$0
	move	$5,$3
	li	$6,112			# 0x70
	li	$7,2			# 0x2
	lui	$2,%hi(phy_I2C_read)
	addiu	$2,$2,%lo(phy_I2C_read)
	jalr	$2
	nop

	li	$2,102			# 0x66
	sb	$2,68($sp)
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,116($2)
	move	$3,$2
	li	$2,8			# 0x8
	sw	$2,16($sp)
	addiu	$2,$sp,68
	sw	$2,20($sp)
	li	$2,4			# 0x4
	sw	$2,24($sp)
	move	$4,$0
	move	$5,$3
	li	$6,112			# 0x70
	li	$7,2			# 0x2
	lui	$2,%hi(phy_I2C_write)
	addiu	$2,$2,%lo(phy_I2C_write)
	jalr	$2
	nop

	li	$4,2			# 0x2
	lui	$2,%hi(__udelay)
	addiu	$2,$2,%lo(__udelay)
	jalr	$2
	nop

	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,116($2)
	move	$3,$2
	li	$2,8			# 0x8
	sw	$2,16($sp)
	addiu	$2,$sp,68
	sw	$2,20($sp)
	li	$2,4			# 0x4
	sw	$2,24($sp)
	move	$4,$0
	move	$5,$3
	li	$6,112			# 0x70
	li	$7,2			# 0x2
	lui	$2,%hi(phy_I2C_read)
	addiu	$2,$2,%lo(phy_I2C_read)
	jalr	$2
	nop

	li	$2,127			# 0x7f
	sb	$2,69($sp)
	li	$2,127			# 0x7f
	sb	$2,70($sp)
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,116($2)
	move	$3,$2
	li	$2,8			# 0x8
	sw	$2,16($sp)
	addiu	$2,$sp,68
	sw	$2,20($sp)
	li	$2,4			# 0x4
	sw	$2,24($sp)
	move	$4,$0
	move	$5,$3
	li	$6,112			# 0x70
	li	$7,2			# 0x2
	lui	$2,%hi(phy_I2C_write)
	addiu	$2,$2,%lo(phy_I2C_write)
	jalr	$2
	nop

	li	$4,2			# 0x2
	lui	$2,%hi(__udelay)
	addiu	$2,$2,%lo(__udelay)
	jalr	$2
	nop

	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,116($2)
	move	$3,$2
	li	$2,8			# 0x8
	sw	$2,16($sp)
	addiu	$2,$sp,68
	sw	$2,20($sp)
	li	$2,4			# 0x4
	sw	$2,24($sp)
	move	$4,$0
	move	$5,$3
	li	$6,112			# 0x70
	li	$7,2			# 0x2
	lui	$2,%hi(phy_I2C_read)
	addiu	$2,$2,%lo(phy_I2C_read)
	jalr	$2
	nop

	lbu	$2,71($sp)
	ori	$2,$2,0x20
	andi	$2,$2,0x00ff
	sb	$2,71($sp)
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,116($2)
	move	$3,$2
	li	$2,8			# 0x8
	sw	$2,16($sp)
	addiu	$2,$sp,68
	sw	$2,20($sp)
	li	$2,4			# 0x4
	sw	$2,24($sp)
	move	$4,$0
	move	$5,$3
	li	$6,112			# 0x70
	li	$7,2			# 0x2
	lui	$2,%hi(phy_I2C_write)
	addiu	$2,$2,%lo(phy_I2C_write)
	jalr	$2
	nop

	li	$4,2			# 0x2
	lui	$2,%hi(__udelay)
	addiu	$2,$2,%lo(__udelay)
	jalr	$2
	nop

	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,116($2)
	move	$3,$2
	li	$2,8			# 0x8
	sw	$2,16($sp)
	addiu	$2,$sp,68
	sw	$2,20($sp)
	li	$2,4			# 0x4
	sw	$2,24($sp)
	move	$4,$0
	move	$5,$3
	li	$6,112			# 0x70
	li	$7,2			# 0x2
	lui	$2,%hi(phy_I2C_read)
	addiu	$2,$2,%lo(phy_I2C_read)
	jalr	$2
	nop

	lbu	$3,71($sp)
	li	$2,-33			# 0xffffffffffffffdf
	and	$2,$3,$2
	andi	$2,$2,0x00ff
	sb	$2,71($sp)
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,116($2)
	move	$3,$2
	li	$2,8			# 0x8
	sw	$2,16($sp)
	addiu	$2,$sp,68
	sw	$2,20($sp)
	li	$2,4			# 0x4
	sw	$2,24($sp)
	move	$4,$0
	move	$5,$3
	li	$6,112			# 0x70
	li	$7,2			# 0x2
	lui	$2,%hi(phy_I2C_write)
	addiu	$2,$2,%lo(phy_I2C_write)
	jalr	$2
	nop

	li	$4,2			# 0x2
	lui	$2,%hi(__udelay)
	addiu	$2,$2,%lo(__udelay)
	jalr	$2
	nop

	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,116($2)
	move	$3,$2
	li	$2,8			# 0x8
	sw	$2,16($sp)
	addiu	$2,$sp,68
	sw	$2,20($sp)
	li	$2,4			# 0x4
	sw	$2,24($sp)
	move	$4,$0
	move	$5,$3
	li	$6,112			# 0x70
	li	$7,2			# 0x2
	lui	$2,%hi(phy_I2C_read)
	addiu	$2,$2,%lo(phy_I2C_read)
	jalr	$2
	nop

	lbu	$2,71($sp)
	ori	$2,$2,0x1
	andi	$2,$2,0x00ff
	sb	$2,71($sp)
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,116($2)
	move	$3,$2
	li	$2,8			# 0x8
	sw	$2,16($sp)
	addiu	$2,$sp,68
	sw	$2,20($sp)
	li	$2,4			# 0x4
	sw	$2,24($sp)
	move	$4,$0
	move	$5,$3
	li	$6,112			# 0x70
	li	$7,2			# 0x2
	lui	$2,%hi(phy_I2C_write)
	addiu	$2,$2,%lo(phy_I2C_write)
	jalr	$2
	nop

	li	$4,2			# 0x2
	lui	$2,%hi(__udelay)
	addiu	$2,$2,%lo(__udelay)
	jalr	$2
	nop

	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,116($2)
	move	$3,$2
	li	$2,8			# 0x8
	sw	$2,16($sp)
	addiu	$2,$sp,68
	sw	$2,20($sp)
	li	$2,4			# 0x4
	sw	$2,24($sp)
	move	$4,$0
	move	$5,$3
	li	$6,112			# 0x70
	li	$7,2			# 0x2
	lui	$2,%hi(phy_I2C_read)
	addiu	$2,$2,%lo(phy_I2C_read)
	jalr	$2
	nop

	lbu	$3,71($sp)
	li	$2,-2			# 0xfffffffffffffffe
	and	$2,$3,$2
	andi	$2,$2,0x00ff
	sb	$2,71($sp)
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,116($2)
	move	$3,$2
	li	$2,8			# 0x8
	sw	$2,16($sp)
	addiu	$2,$sp,68
	sw	$2,20($sp)
	li	$2,4			# 0x4
	sw	$2,24($sp)
	move	$4,$0
	move	$5,$3
	li	$6,112			# 0x70
	li	$7,2			# 0x2
	lui	$2,%hi(phy_I2C_write)
	addiu	$2,$2,%lo(phy_I2C_write)
	jalr	$2
	nop

	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,116($2)
	move	$3,$2
	li	$2,16			# 0x10
	sw	$2,16($sp)
	addiu	$2,$sp,68
	sw	$2,20($sp)
	li	$2,4			# 0x4
	sw	$2,24($sp)
	move	$4,$0
	move	$5,$3
	li	$6,112			# 0x70
	li	$7,2			# 0x2
	lui	$2,%hi(phy_I2C_read)
	addiu	$2,$2,%lo(phy_I2C_read)
	jalr	$2
	nop

	lbu	$2,68($sp)
	move	$3,$2
	lw	$2,32($sp)
	sltu	$2,$2,$3
	beq	$2,$0,$L127
	nop

	lbu	$2,68($sp)
	sw	$2,32($sp)
$L127:
	lbu	$2,69($sp)
	move	$3,$2
	lw	$2,36($sp)
	sltu	$2,$2,$3
	beq	$2,$0,$L128
	nop

	lbu	$2,69($sp)
	sw	$2,36($sp)
$L128:
	lw	$2,40($sp)
	addiu	$2,$2,1
	sw	$2,40($sp)
$L126:
	lw	$2,40($sp)
	slt	$2,$2,32
	bne	$2,$0,$L129
	nop

	li	$2,10			# 0xa
	sw	$2,48($sp)
	j	$L130
	nop

$L131:
	li	$4,1000			# 0x3e8
	lui	$2,%hi(__udelay)
	addiu	$2,$2,%lo(__udelay)
	jalr	$2
	nop

$L130:
	lw	$2,48($sp)
	addiu	$3,$2,-1
	sw	$3,48($sp)
	bne	$2,$0,$L131
	nop

	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,116($2)
	move	$3,$2
	li	$2,8			# 0x8
	sw	$2,16($sp)
	addiu	$2,$sp,68
	sw	$2,20($sp)
	li	$2,4			# 0x4
	sw	$2,24($sp)
	move	$4,$0
	move	$5,$3
	li	$6,112			# 0x70
	li	$7,2			# 0x2
	lui	$2,%hi(phy_I2C_read)
	addiu	$2,$2,%lo(phy_I2C_read)
	jalr	$2
	nop

	li	$4,152			# 0x98
	lui	$2,%hi(get_flash_register)
	addiu	$2,$2,%lo(get_flash_register)
	jalr	$2
	nop

	move	$3,$2
	li	$2,-1			# 0xffffffffffffffff
	beq	$3,$2,$L132
	nop

	li	$4,152			# 0x98
	lui	$2,%hi(get_flash_register)
	addiu	$2,$2,%lo(get_flash_register)
	jalr	$2
	nop

	andi	$2,$2,0x00ff
	sb	$2,68($sp)
	li	$4,152			# 0x98
	lui	$2,%hi(get_flash_register)
	addiu	$2,$2,%lo(get_flash_register)
	jalr	$2
	nop

	andi	$3,$2,0xff
	lui	$2,%hi($LC60)
	addiu	$4,$2,%lo($LC60)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	j	$L133
	nop

$L132:
	lw	$3,80($sp)
	li	$2,1			# 0x1
	bne	$3,$2,$L134
	nop

	li	$2,-102			# 0xffffffffffffff9a
	sb	$2,68($sp)
	lui	$2,%hi($LC61)
	addiu	$4,$2,%lo($LC61)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	j	$L133
	nop

$L134:
	lw	$2,80($sp)
	bne	$2,$0,$L133
	nop

	li	$2,71			# 0x47
	sb	$2,68($sp)
	lui	$2,%hi($LC62)
	addiu	$4,$2,%lo($LC62)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L133:
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,116($2)
	move	$3,$2
	li	$2,8			# 0x8
	sw	$2,16($sp)
	addiu	$2,$sp,68
	sw	$2,20($sp)
	li	$2,4			# 0x4
	sw	$2,24($sp)
	move	$4,$0
	move	$5,$3
	li	$6,112			# 0x70
	li	$7,2			# 0x2
	lui	$2,%hi(phy_I2C_write)
	addiu	$2,$2,%lo(phy_I2C_write)
	jalr	$2
	nop

	li	$2,10			# 0xa
	sw	$2,52($sp)
	j	$L135
	nop

$L136:
	li	$4,1000			# 0x3e8
	lui	$2,%hi(__udelay)
	addiu	$2,$2,%lo(__udelay)
	jalr	$2
	nop

$L135:
	lw	$2,52($sp)
	addiu	$3,$2,-1
	sw	$3,52($sp)
	bne	$2,$0,$L136
	nop

	li	$4,156			# 0x9c
	lui	$2,%hi(get_flash_register)
	addiu	$2,$2,%lo(get_flash_register)
	jalr	$2
	nop

	move	$3,$2
	li	$2,-1			# 0xffffffffffffffff
	beq	$3,$2,$L137
	nop

	li	$4,156			# 0x9c
	lui	$2,%hi(get_flash_register)
	addiu	$2,$2,%lo(get_flash_register)
	jalr	$2
	nop

	andi	$2,$2,0xffff
	sw	$2,32($sp)
	li	$4,156			# 0x9c
	lui	$2,%hi(get_flash_register)
	addiu	$2,$2,%lo(get_flash_register)
	jalr	$2
	nop

	srl	$2,$2,16
	sw	$2,36($sp)
$L137:
	lui	$2,%hi($LC63)
	addiu	$4,$2,%lo($LC63)
	lw	$5,32($sp)
	lw	$6,36($sp)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,116($2)
	move	$3,$2
	li	$2,8			# 0x8
	sw	$2,16($sp)
	addiu	$2,$sp,68
	sw	$2,20($sp)
	li	$2,4			# 0x4
	sw	$2,24($sp)
	move	$4,$0
	move	$5,$3
	li	$6,112			# 0x70
	li	$7,2			# 0x2
	lui	$2,%hi(phy_I2C_read)
	addiu	$2,$2,%lo(phy_I2C_read)
	jalr	$2
	nop

	lw	$2,36($sp)
	andi	$2,$2,0x00ff
	sb	$2,69($sp)
	lw	$2,32($sp)
	andi	$2,$2,0x00ff
	sb	$2,70($sp)
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,116($2)
	move	$3,$2
	li	$2,8			# 0x8
	sw	$2,16($sp)
	addiu	$2,$sp,68
	sw	$2,20($sp)
	li	$2,4			# 0x4
	sw	$2,24($sp)
	move	$4,$0
	move	$5,$3
	li	$6,112			# 0x70
	li	$7,2			# 0x2
	lui	$2,%hi(phy_I2C_write)
	addiu	$2,$2,%lo(phy_I2C_write)
	jalr	$2
	nop

	li	$2,10			# 0xa
	sw	$2,56($sp)
	j	$L138
	nop

$L139:
	li	$4,1000			# 0x3e8
	lui	$2,%hi(__udelay)
	addiu	$2,$2,%lo(__udelay)
	jalr	$2
	nop

$L138:
	lw	$2,56($sp)
	addiu	$3,$2,-1
	sw	$3,56($sp)
	bne	$2,$0,$L139
	nop

	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,116($2)
	move	$3,$2
	li	$2,8			# 0x8
	sw	$2,16($sp)
	addiu	$2,$sp,68
	sw	$2,20($sp)
	li	$2,4			# 0x4
	sw	$2,24($sp)
	move	$4,$0
	move	$5,$3
	li	$6,112			# 0x70
	li	$7,2			# 0x2
	lui	$2,%hi(phy_I2C_read)
	addiu	$2,$2,%lo(phy_I2C_read)
	jalr	$2
	nop

	lbu	$2,71($sp)
	ori	$2,$2,0x20
	andi	$2,$2,0x00ff
	sb	$2,71($sp)
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,116($2)
	move	$3,$2
	li	$2,8			# 0x8
	sw	$2,16($sp)
	addiu	$2,$sp,68
	sw	$2,20($sp)
	li	$2,4			# 0x4
	sw	$2,24($sp)
	move	$4,$0
	move	$5,$3
	li	$6,112			# 0x70
	li	$7,2			# 0x2
	lui	$2,%hi(phy_I2C_write)
	addiu	$2,$2,%lo(phy_I2C_write)
	jalr	$2
	nop

	li	$2,10			# 0xa
	sw	$2,60($sp)
	j	$L140
	nop

$L141:
	li	$4,1000			# 0x3e8
	lui	$2,%hi(__udelay)
	addiu	$2,$2,%lo(__udelay)
	jalr	$2
	nop

$L140:
	lw	$2,60($sp)
	addiu	$3,$2,-1
	sw	$3,60($sp)
	bne	$2,$0,$L141
	nop

	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,116($2)
	move	$3,$2
	li	$2,8			# 0x8
	sw	$2,16($sp)
	addiu	$2,$sp,68
	sw	$2,20($sp)
	li	$2,4			# 0x4
	sw	$2,24($sp)
	move	$4,$0
	move	$5,$3
	li	$6,112			# 0x70
	li	$7,2			# 0x2
	lui	$2,%hi(phy_I2C_read)
	addiu	$2,$2,%lo(phy_I2C_read)
	jalr	$2
	nop

	lbu	$3,71($sp)
	li	$2,-33			# 0xffffffffffffffdf
	and	$2,$3,$2
	andi	$2,$2,0x00ff
	sb	$2,71($sp)
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,116($2)
	move	$3,$2
	li	$2,8			# 0x8
	sw	$2,16($sp)
	addiu	$2,$sp,68
	sw	$2,20($sp)
	li	$2,4			# 0x4
	sw	$2,24($sp)
	move	$4,$0
	move	$5,$3
	li	$6,112			# 0x70
	li	$7,2			# 0x2
	lui	$2,%hi(phy_I2C_write)
	addiu	$2,$2,%lo(phy_I2C_write)
	jalr	$2
	nop

	li	$2,10			# 0xa
	sw	$2,64($sp)
	j	$L142
	nop

$L143:
	li	$4,1000			# 0x3e8
	lui	$2,%hi(__udelay)
	addiu	$2,$2,%lo(__udelay)
	jalr	$2
	nop

$L142:
	lw	$2,64($sp)
	addiu	$3,$2,-1
	sw	$3,64($sp)
	bne	$2,$0,$L143
	nop

	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,116($2)
	move	$3,$2
	li	$2,8			# 0x8
	sw	$2,16($sp)
	addiu	$2,$sp,68
	sw	$2,20($sp)
	li	$2,4			# 0x4
	sw	$2,24($sp)
	move	$4,$0
	move	$5,$3
	li	$6,112			# 0x70
	li	$7,2			# 0x2
	lui	$2,%hi(phy_I2C_read)
	addiu	$2,$2,%lo(phy_I2C_read)
	jalr	$2
	nop

	lbu	$2,71($sp)
	ori	$2,$2,0x8
	andi	$2,$2,0x00ff
	sb	$2,71($sp)
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,116($2)
	move	$3,$2
	li	$2,8			# 0x8
	sw	$2,16($sp)
	addiu	$2,$sp,68
	sw	$2,20($sp)
	li	$2,4			# 0x4
	sw	$2,24($sp)
	move	$4,$0
	move	$5,$3
	li	$6,112			# 0x70
	li	$7,2			# 0x2
	lui	$2,%hi(phy_I2C_write)
	addiu	$2,$2,%lo(phy_I2C_write)
	jalr	$2
	nop

	li	$4,1			# 0x1
	lui	$2,%hi(mt7570_CDR)
	addiu	$2,$2,%lo(mt7570_CDR)
	jalr	$2
	nop

	move	$4,$0
	lui	$2,%hi(phy_tx_test_pattern)
	addiu	$2,$2,%lo(phy_tx_test_pattern)
	jalr	$2
	nop

	lui	$2,%hi($LC64)
	addiu	$4,$2,%lo($LC64)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lw	$31,76($sp)
	addiu	$sp,$sp,80
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	mt7570_TGEN
	.size	mt7570_TGEN, .-mt7570_TGEN
	.rdata
	.align	2
$LC65:
	.ascii	"Rx LOS is set\012\000"
	.section	.text.mt7570_LOS_level_set,"ax",@progbits
	.align	2
	.globl	mt7570_LOS_level_set
	.set	nomips16
	.set	nomicromips
	.ent	mt7570_LOS_level_set
	.type	mt7570_LOS_level_set, @function
mt7570_LOS_level_set:
	.frame	$sp,48,$31		# vars= 8, regs= 2/0, args= 32, gp= 0
	.mask	0x80010000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-48
	sw	$31,44($sp)
	sw	$16,40($sp)
	lui	$2,%hi(mt7570_LOS_init)
	addiu	$2,$2,%lo(mt7570_LOS_init)
	jalr	$2
	nop

	li	$4,32			# 0x20
	lui	$2,%hi(get_flash_register)
	addiu	$2,$2,%lo(get_flash_register)
	jalr	$2
	nop

	move	$3,$2
	li	$2,-1			# 0xffffffffffffffff
	beq	$3,$2,$L145
	nop

	li	$4,36			# 0x24
	lui	$2,%hi(get_flash_register)
	addiu	$2,$2,%lo(get_flash_register)
	jalr	$2
	nop

	move	$3,$2
	li	$2,-1			# 0xffffffffffffffff
	beq	$3,$2,$L145
	nop

	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,116($2)
	move	$3,$2
	li	$2,284			# 0x11c
	sw	$2,16($sp)
	addiu	$2,$sp,32
	sw	$2,20($sp)
	li	$2,4			# 0x4
	sw	$2,24($sp)
	move	$4,$0
	move	$5,$3
	li	$6,112			# 0x70
	li	$7,2			# 0x2
	lui	$2,%hi(phy_I2C_read)
	addiu	$2,$2,%lo(phy_I2C_read)
	jalr	$2
	nop

	lbu	$2,35($sp)
	seb	$3,$2
	li	$2,-128			# 0xffffffffffffff80
	and	$2,$3,$2
	seb	$16,$2
	li	$4,36			# 0x24
	lui	$2,%hi(get_flash_register)
	addiu	$2,$2,%lo(get_flash_register)
	jalr	$2
	nop

	seb	$2,$2
	or	$2,$16,$2
	seb	$2,$2
	andi	$2,$2,0x00ff
	sb	$2,35($sp)
	lbu	$2,34($sp)
	seb	$3,$2
	li	$2,-128			# 0xffffffffffffff80
	and	$2,$3,$2
	seb	$16,$2
	li	$4,32			# 0x20
	lui	$2,%hi(get_flash_register)
	addiu	$2,$2,%lo(get_flash_register)
	jalr	$2
	nop

	seb	$2,$2
	or	$2,$16,$2
	seb	$2,$2
	andi	$2,$2,0x00ff
	sb	$2,34($sp)
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,116($2)
	move	$3,$2
	li	$2,284			# 0x11c
	sw	$2,16($sp)
	addiu	$2,$sp,32
	sw	$2,20($sp)
	li	$2,4			# 0x4
	sw	$2,24($sp)
	move	$4,$0
	move	$5,$3
	li	$6,112			# 0x70
	li	$7,2			# 0x2
	lui	$2,%hi(phy_I2C_write)
	addiu	$2,$2,%lo(phy_I2C_write)
	jalr	$2
	nop

	j	$L146
	nop

$L145:
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,116($2)
	move	$3,$2
	li	$2,284			# 0x11c
	sw	$2,16($sp)
	addiu	$2,$sp,32
	sw	$2,20($sp)
	li	$2,4			# 0x4
	sw	$2,24($sp)
	move	$4,$0
	move	$5,$3
	li	$6,112			# 0x70
	li	$7,2			# 0x2
	lui	$2,%hi(phy_I2C_read)
	addiu	$2,$2,%lo(phy_I2C_read)
	jalr	$2
	nop

	lbu	$2,35($sp)
	seb	$3,$2
	li	$2,-128			# 0xffffffffffffff80
	and	$2,$3,$2
	seb	$2,$2
	ori	$2,$2,0x20
	seb	$2,$2
	andi	$2,$2,0x00ff
	sb	$2,35($sp)
	lbu	$2,34($sp)
	seb	$3,$2
	li	$2,-128			# 0xffffffffffffff80
	and	$2,$3,$2
	seb	$2,$2
	ori	$2,$2,0x30
	seb	$2,$2
	andi	$2,$2,0x00ff
	sb	$2,34($sp)
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,116($2)
	move	$3,$2
	li	$2,284			# 0x11c
	sw	$2,16($sp)
	addiu	$2,$sp,32
	sw	$2,20($sp)
	li	$2,4			# 0x4
	sw	$2,24($sp)
	move	$4,$0
	move	$5,$3
	li	$6,112			# 0x70
	li	$7,2			# 0x2
	lui	$2,%hi(phy_I2C_write)
	addiu	$2,$2,%lo(phy_I2C_write)
	jalr	$2
	nop

$L146:
	lui	$2,%hi($LC65)
	addiu	$4,$2,%lo($LC65)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lw	$31,44($sp)
	lw	$16,40($sp)
	addiu	$sp,$sp,48
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	mt7570_LOS_level_set
	.size	mt7570_LOS_level_set, .-mt7570_LOS_level_set
	.globl	__floatunsidf
	.globl	__muldf3
	.globl	__floatsidf
	.rdata
	.align	2
$LC68:
	.ascii	"Tx SD set\012\000"
	.align	2
$LC69:
	.ascii	"Tx SD error\012\000"
	.section	.text.mt7570_TxSD_level_set,"ax",@progbits
	.align	2
	.globl	mt7570_TxSD_level_set
	.set	nomips16
	.set	nomicromips
	.ent	mt7570_TxSD_level_set
	.type	mt7570_TxSD_level_set, @function
mt7570_TxSD_level_set:
	.frame	$sp,72,$31		# vars= 24, regs= 3/0, args= 32, gp= 0
	.mask	0x80030000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-72
	sw	$31,68($sp)
	sw	$17,64($sp)
	sw	$16,60($sp)
	sw	$0,32($sp)
	sw	$0,36($sp)
	sw	$0,40($sp)
	sw	$0,44($sp)
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,116($2)
	move	$3,$2
	li	$2,4			# 0x4
	sw	$2,16($sp)
	addiu	$2,$sp,48
	sw	$2,20($sp)
	li	$2,4			# 0x4
	sw	$2,24($sp)
	move	$4,$0
	move	$5,$3
	li	$6,112			# 0x70
	li	$7,2			# 0x2
	lui	$2,%hi(phy_I2C_read)
	addiu	$2,$2,%lo(phy_I2C_read)
	jalr	$2
	nop

	lbu	$2,49($sp)
	sll	$2,$2,8
	lbu	$3,48($sp)
	or	$2,$2,$3
	andi	$2,$2,0x3ff
	sw	$2,40($sp)
	lbu	$2,51($sp)
	sll	$2,$2,8
	lbu	$3,50($sp)
	or	$2,$2,$3
	andi	$2,$2,0x3ff
	sw	$2,44($sp)
	lui	$2,%hi(mt7570_TxSD_level_calibration)
	addiu	$2,$2,%lo(mt7570_TxSD_level_calibration)
	jalr	$2
	nop

	sw	$2,32($sp)
	lw	$2,32($sp)
	bltz	$2,$L148
	nop

	lw	$2,44($sp)
	srl	$3,$2,2
	lw	$2,40($sp)
	addu	$3,$3,$2
	lui	$2,%hi(__floatunsidf)
	addiu	$2,$2,%lo(__floatunsidf)
	move	$4,$3
	jalr	$2
	nop

	move	$5,$3
	move	$4,$2
	lui	$3,%hi($LC66)
	lui	$2,%hi(__muldf3)
	addiu	$2,$2,%lo(__muldf3)
	lw	$7,%lo($LC66+4)($3)
	lw	$6,%lo($LC66)($3)
	jalr	$2
	nop

	move	$17,$3
	move	$16,$2
	lui	$2,%hi(__floatsidf)
	addiu	$2,$2,%lo(__floatsidf)
	lw	$4,32($sp)
	jalr	$2
	nop

	move	$5,$3
	move	$4,$2
	lui	$3,%hi($LC67)
	lui	$2,%hi(__muldf3)
	addiu	$2,$2,%lo(__muldf3)
	lw	$7,%lo($LC67+4)($3)
	lw	$6,%lo($LC67)($3)
	jalr	$2
	nop

	move	$7,$3
	move	$6,$2
	lui	$2,%hi(__adddf3)
	addiu	$2,$2,%lo(__adddf3)
	move	$5,$17
	move	$4,$16
	jalr	$2
	nop

	move	$5,$3
	move	$4,$2
	lui	$2,%hi(__fixunsdfsi)
	addiu	$2,$2,%lo(__fixunsdfsi)
	jalr	$2
	nop

	sw	$2,36($sp)
	lw	$2,36($sp)
	andi	$2,$2,0x00ff
	sb	$2,48($sp)
	lw	$2,36($sp)
	srl	$2,$2,8
	andi	$2,$2,0x00ff
	andi	$2,$2,0x1
	andi	$2,$2,0x00ff
	sb	$2,49($sp)
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,116($2)
	move	$3,$2
	li	$2,12			# 0xc
	sw	$2,16($sp)
	addiu	$2,$sp,48
	sw	$2,20($sp)
	li	$2,2			# 0x2
	sw	$2,24($sp)
	move	$4,$0
	move	$5,$3
	li	$6,112			# 0x70
	li	$7,2			# 0x2
	lui	$2,%hi(phy_I2C_write)
	addiu	$2,$2,%lo(phy_I2C_write)
	jalr	$2
	nop

	lui	$2,%hi($LC68)
	addiu	$4,$2,%lo($LC68)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	j	$L147
	nop

$L148:
	lui	$2,%hi($LC69)
	addiu	$4,$2,%lo($LC69)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L147:
	lw	$31,68($sp)
	lw	$17,64($sp)
	lw	$16,60($sp)
	addiu	$sp,$sp,72
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	mt7570_TxSD_level_set
	.size	mt7570_TxSD_level_set, .-mt7570_TxSD_level_set
	.section	.text.mt7570_TxSD_level_calibration,"ax",@progbits
	.align	2
	.globl	mt7570_TxSD_level_calibration
	.set	nomips16
	.set	nomicromips
	.ent	mt7570_TxSD_level_calibration
	.type	mt7570_TxSD_level_calibration, @function
mt7570_TxSD_level_calibration:
	.frame	$sp,56,$31		# vars= 16, regs= 1/0, args= 32, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-56
	sw	$31,52($sp)
	sw	$0,36($sp)
	sw	$0,40($sp)
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,116($2)
	move	$3,$2
	sw	$0,16($sp)
	addiu	$2,$sp,44
	sw	$2,20($sp)
	li	$2,1			# 0x1
	sw	$2,24($sp)
	move	$4,$0
	move	$5,$3
	li	$6,112			# 0x70
	li	$7,2			# 0x2
	lui	$2,%hi(phy_I2C_read)
	addiu	$2,$2,%lo(phy_I2C_read)
	jalr	$2
	nop

	lbu	$2,44($sp)
	seb	$3,$2
	li	$2,-15			# 0xfffffffffffffff1
	and	$2,$3,$2
	seb	$2,$2
	ori	$2,$2,0x8
	seb	$2,$2
	andi	$2,$2,0x00ff
	sb	$2,44($sp)
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,116($2)
	move	$3,$2
	sw	$0,16($sp)
	addiu	$2,$sp,44
	sw	$2,20($sp)
	li	$2,1			# 0x1
	sw	$2,24($sp)
	move	$4,$0
	move	$5,$3
	li	$6,112			# 0x70
	li	$7,2			# 0x2
	lui	$2,%hi(phy_I2C_write)
	addiu	$2,$2,%lo(phy_I2C_write)
	jalr	$2
	nop

	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,116($2)
	move	$3,$2
	li	$2,38			# 0x26
	sw	$2,16($sp)
	addiu	$2,$sp,44
	sw	$2,20($sp)
	li	$2,1			# 0x1
	sw	$2,24($sp)
	move	$4,$0
	move	$5,$3
	li	$6,112			# 0x70
	li	$7,2			# 0x2
	lui	$2,%hi(phy_I2C_read)
	addiu	$2,$2,%lo(phy_I2C_read)
	jalr	$2
	nop

	lbu	$2,44($sp)
	ori	$2,$2,0x10
	andi	$2,$2,0x00ff
	sb	$2,44($sp)
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,116($2)
	move	$3,$2
	li	$2,38			# 0x26
	sw	$2,16($sp)
	addiu	$2,$sp,44
	sw	$2,20($sp)
	li	$2,1			# 0x1
	sw	$2,24($sp)
	move	$4,$0
	move	$5,$3
	li	$6,112			# 0x70
	li	$7,2			# 0x2
	lui	$2,%hi(phy_I2C_write)
	addiu	$2,$2,%lo(phy_I2C_write)
	jalr	$2
	nop

	li	$4,1000			# 0x3e8
	lui	$2,%hi(__udelay)
	addiu	$2,$2,%lo(__udelay)
	jalr	$2
	nop

	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,116($2)
	move	$3,$2
	li	$2,36			# 0x24
	sw	$2,16($sp)
	addiu	$2,$sp,44
	sw	$2,20($sp)
	li	$2,1			# 0x1
	sw	$2,24($sp)
	move	$4,$0
	move	$5,$3
	li	$6,112			# 0x70
	li	$7,2			# 0x2
	lui	$2,%hi(phy_I2C_read)
	addiu	$2,$2,%lo(phy_I2C_read)
	jalr	$2
	nop

	lbu	$2,44($sp)
	seb	$3,$2
	li	$2,-31			# 0xffffffffffffffe1
	and	$2,$3,$2
	seb	$2,$2
	ori	$2,$2,0x4
	seb	$2,$2
	andi	$2,$2,0x00ff
	sb	$2,44($sp)
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,116($2)
	move	$3,$2
	li	$2,36			# 0x24
	sw	$2,16($sp)
	addiu	$2,$sp,44
	sw	$2,20($sp)
	li	$2,1			# 0x1
	sw	$2,24($sp)
	move	$4,$0
	move	$5,$3
	li	$6,112			# 0x70
	li	$7,2			# 0x2
	lui	$2,%hi(phy_I2C_write)
	addiu	$2,$2,%lo(phy_I2C_write)
	jalr	$2
	nop

	li	$4,1000			# 0x3e8
	lui	$2,%hi(__udelay)
	addiu	$2,$2,%lo(__udelay)
	jalr	$2
	nop

	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,116($2)
	move	$3,$2
	li	$2,345			# 0x159
	sw	$2,16($sp)
	addiu	$2,$sp,44
	sw	$2,20($sp)
	li	$2,1			# 0x1
	sw	$2,24($sp)
	move	$4,$0
	move	$5,$3
	li	$6,112			# 0x70
	li	$7,2			# 0x2
	lui	$2,%hi(phy_I2C_read)
	addiu	$2,$2,%lo(phy_I2C_read)
	jalr	$2
	nop

	lbu	$2,44($sp)
	ori	$2,$2,0x10
	andi	$2,$2,0x00ff
	sb	$2,44($sp)
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,116($2)
	move	$3,$2
	li	$2,345			# 0x159
	sw	$2,16($sp)
	addiu	$2,$sp,44
	sw	$2,20($sp)
	li	$2,1			# 0x1
	sw	$2,24($sp)
	move	$4,$0
	move	$5,$3
	li	$6,112			# 0x70
	li	$7,2			# 0x2
	lui	$2,%hi(phy_I2C_write)
	addiu	$2,$2,%lo(phy_I2C_write)
	jalr	$2
	nop

	li	$4,1000			# 0x3e8
	lui	$2,%hi(__udelay)
	addiu	$2,$2,%lo(__udelay)
	jalr	$2
	nop

	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,116($2)
	move	$3,$2
	li	$2,340			# 0x154
	sw	$2,16($sp)
	addiu	$2,$sp,44
	sw	$2,20($sp)
	li	$2,2			# 0x2
	sw	$2,24($sp)
	move	$4,$0
	move	$5,$3
	li	$6,112			# 0x70
	li	$7,2			# 0x2
	lui	$2,%hi(phy_I2C_read)
	addiu	$2,$2,%lo(phy_I2C_read)
	jalr	$2
	nop

	lbu	$2,44($sp)
	move	$3,$2
	lbu	$2,45($sp)
	sll	$2,$2,8
	or	$2,$3,$2
	sw	$2,36($sp)
	li	$2,10			# 0xa
	sw	$2,32($sp)
	j	$L151
	nop

$L152:
	li	$4,1000			# 0x3e8
	lui	$2,%hi(__udelay)
	addiu	$2,$2,%lo(__udelay)
	jalr	$2
	nop

$L151:
	lw	$2,32($sp)
	addiu	$3,$2,-1
	sw	$3,32($sp)
	bne	$2,$0,$L152
	nop

	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,116($2)
	move	$3,$2
	sw	$0,16($sp)
	addiu	$2,$sp,44
	sw	$2,20($sp)
	li	$2,1			# 0x1
	sw	$2,24($sp)
	move	$4,$0
	move	$5,$3
	li	$6,112			# 0x70
	li	$7,2			# 0x2
	lui	$2,%hi(phy_I2C_read)
	addiu	$2,$2,%lo(phy_I2C_read)
	jalr	$2
	nop

	lbu	$2,44($sp)
	seb	$3,$2
	li	$2,-15			# 0xfffffffffffffff1
	and	$2,$3,$2
	seb	$2,$2
	ori	$2,$2,0x2
	seb	$2,$2
	andi	$2,$2,0x00ff
	sb	$2,44($sp)
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,116($2)
	move	$3,$2
	sw	$0,16($sp)
	addiu	$2,$sp,44
	sw	$2,20($sp)
	li	$2,1			# 0x1
	sw	$2,24($sp)
	move	$4,$0
	move	$5,$3
	li	$6,112			# 0x70
	li	$7,2			# 0x2
	lui	$2,%hi(phy_I2C_write)
	addiu	$2,$2,%lo(phy_I2C_write)
	jalr	$2
	nop

	li	$4,1000			# 0x3e8
	lui	$2,%hi(__udelay)
	addiu	$2,$2,%lo(__udelay)
	jalr	$2
	nop

	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,116($2)
	move	$3,$2
	li	$2,12			# 0xc
	sw	$2,16($sp)
	addiu	$2,$sp,44
	sw	$2,20($sp)
	li	$2,2			# 0x2
	sw	$2,24($sp)
	move	$4,$0
	move	$5,$3
	li	$6,112			# 0x70
	li	$7,2			# 0x2
	lui	$2,%hi(phy_I2C_read)
	addiu	$2,$2,%lo(phy_I2C_read)
	jalr	$2
	nop

	sb	$0,44($sp)
	lbu	$3,45($sp)
	li	$2,-2			# 0xfffffffffffffffe
	and	$2,$3,$2
	andi	$2,$2,0x00ff
	sb	$2,45($sp)
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,116($2)
	move	$3,$2
	li	$2,12			# 0xc
	sw	$2,16($sp)
	addiu	$2,$sp,44
	sw	$2,20($sp)
	li	$2,2			# 0x2
	sw	$2,24($sp)
	move	$4,$0
	move	$5,$3
	li	$6,112			# 0x70
	li	$7,2			# 0x2
	lui	$2,%hi(phy_I2C_write)
	addiu	$2,$2,%lo(phy_I2C_write)
	jalr	$2
	nop

	li	$4,1000			# 0x3e8
	lui	$2,%hi(__udelay)
	addiu	$2,$2,%lo(__udelay)
	jalr	$2
	nop

	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,116($2)
	move	$3,$2
	li	$2,345			# 0x159
	sw	$2,16($sp)
	addiu	$2,$sp,44
	sw	$2,20($sp)
	li	$2,1			# 0x1
	sw	$2,24($sp)
	move	$4,$0
	move	$5,$3
	li	$6,112			# 0x70
	li	$7,2			# 0x2
	lui	$2,%hi(phy_I2C_read)
	addiu	$2,$2,%lo(phy_I2C_read)
	jalr	$2
	nop

	lbu	$2,44($sp)
	ori	$2,$2,0x10
	andi	$2,$2,0x00ff
	sb	$2,44($sp)
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,116($2)
	move	$3,$2
	li	$2,345			# 0x159
	sw	$2,16($sp)
	addiu	$2,$sp,44
	sw	$2,20($sp)
	li	$2,1			# 0x1
	sw	$2,24($sp)
	move	$4,$0
	move	$5,$3
	li	$6,112			# 0x70
	li	$7,2			# 0x2
	lui	$2,%hi(phy_I2C_write)
	addiu	$2,$2,%lo(phy_I2C_write)
	jalr	$2
	nop

	li	$4,1000			# 0x3e8
	lui	$2,%hi(__udelay)
	addiu	$2,$2,%lo(__udelay)
	jalr	$2
	nop

	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,116($2)
	move	$3,$2
	li	$2,340			# 0x154
	sw	$2,16($sp)
	addiu	$2,$sp,44
	sw	$2,20($sp)
	li	$2,2			# 0x2
	sw	$2,24($sp)
	move	$4,$0
	move	$5,$3
	li	$6,112			# 0x70
	li	$7,2			# 0x2
	lui	$2,%hi(phy_I2C_read)
	addiu	$2,$2,%lo(phy_I2C_read)
	jalr	$2
	nop

	lbu	$2,44($sp)
	move	$3,$2
	lbu	$2,45($sp)
	sll	$2,$2,8
	or	$2,$3,$2
	sw	$2,40($sp)
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,116($2)
	move	$3,$2
	li	$2,38			# 0x26
	sw	$2,16($sp)
	addiu	$2,$sp,44
	sw	$2,20($sp)
	li	$2,1			# 0x1
	sw	$2,24($sp)
	move	$4,$0
	move	$5,$3
	li	$6,112			# 0x70
	li	$7,2			# 0x2
	lui	$2,%hi(phy_I2C_read)
	addiu	$2,$2,%lo(phy_I2C_read)
	jalr	$2
	nop

	lbu	$3,44($sp)
	li	$2,-17			# 0xffffffffffffffef
	and	$2,$3,$2
	andi	$2,$2,0x00ff
	sb	$2,44($sp)
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,116($2)
	move	$3,$2
	li	$2,38			# 0x26
	sw	$2,16($sp)
	addiu	$2,$sp,44
	sw	$2,20($sp)
	li	$2,1			# 0x1
	sw	$2,24($sp)
	move	$4,$0
	move	$5,$3
	li	$6,112			# 0x70
	li	$7,2			# 0x2
	lui	$2,%hi(phy_I2C_write)
	addiu	$2,$2,%lo(phy_I2C_write)
	jalr	$2
	nop

	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,116($2)
	move	$3,$2
	li	$2,36			# 0x24
	sw	$2,16($sp)
	addiu	$2,$sp,44
	sw	$2,20($sp)
	li	$2,1			# 0x1
	sw	$2,24($sp)
	move	$4,$0
	move	$5,$3
	li	$6,112			# 0x70
	li	$7,2			# 0x2
	lui	$2,%hi(phy_I2C_read)
	addiu	$2,$2,%lo(phy_I2C_read)
	jalr	$2
	nop

	lbu	$3,44($sp)
	li	$2,-31			# 0xffffffffffffffe1
	and	$2,$3,$2
	andi	$2,$2,0x00ff
	sb	$2,44($sp)
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,116($2)
	move	$3,$2
	li	$2,36			# 0x24
	sw	$2,16($sp)
	addiu	$2,$sp,44
	sw	$2,20($sp)
	li	$2,1			# 0x1
	sw	$2,24($sp)
	move	$4,$0
	move	$5,$3
	li	$6,112			# 0x70
	li	$7,2			# 0x2
	lui	$2,%hi(phy_I2C_write)
	addiu	$2,$2,%lo(phy_I2C_write)
	jalr	$2
	nop

	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,116($2)
	move	$3,$2
	sw	$0,16($sp)
	addiu	$2,$sp,44
	sw	$2,20($sp)
	li	$2,1			# 0x1
	sw	$2,24($sp)
	move	$4,$0
	move	$5,$3
	li	$6,112			# 0x70
	li	$7,2			# 0x2
	lui	$2,%hi(phy_I2C_read)
	addiu	$2,$2,%lo(phy_I2C_read)
	jalr	$2
	nop

	lbu	$2,44($sp)
	seb	$3,$2
	li	$2,-15			# 0xfffffffffffffff1
	and	$2,$3,$2
	seb	$2,$2
	ori	$2,$2,0x8
	seb	$2,$2
	andi	$2,$2,0x00ff
	sb	$2,44($sp)
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,116($2)
	move	$3,$2
	sw	$0,16($sp)
	addiu	$2,$sp,44
	sw	$2,20($sp)
	li	$2,1			# 0x1
	sw	$2,24($sp)
	move	$4,$0
	move	$5,$3
	li	$6,112			# 0x70
	li	$7,2			# 0x2
	lui	$2,%hi(phy_I2C_write)
	addiu	$2,$2,%lo(phy_I2C_write)
	jalr	$2
	nop

	lw	$3,36($sp)
	lw	$2,40($sp)
	subu	$2,$3,$2
	lw	$31,52($sp)
	addiu	$sp,$sp,56
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	mt7570_TxSD_level_calibration
	.size	mt7570_TxSD_level_calibration, .-mt7570_TxSD_level_calibration
	.rdata
	.align	2
$LC70:
	.ascii	"\015\012 atomic, [%s : %d]\015\012\000"
	.align	2
$LC71:
	.ascii	"[%lu]0ms[%s:%d]\012  BG 1V76 = 0x%x\012BG 0V875 =0x%x\012"
	.ascii	"\000"
	.align	2
$LC72:
	.ascii	"ADC calibration fail\012\000"
	.globl	__floatsisf
	.globl	__divdf3
	.globl	__truncdfsf2
	.globl	__subdf3
	.section	.text.mt7570_ADC_calibration,"ax",@progbits
	.align	2
	.globl	mt7570_ADC_calibration
	.set	nomips16
	.set	nomicromips
	.ent	mt7570_ADC_calibration
	.type	mt7570_ADC_calibration, @function
mt7570_ADC_calibration:
	.frame	$sp,64,$31		# vars= 24, regs= 2/0, args= 32, gp= 0
	.mask	0x80010000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-64
	sw	$31,60($sp)
	sw	$16,56($sp)
	sw	$0,40($sp)
	li	$2,8			# 0x8
	sw	$2,44($sp)
	sw	$0,32($sp)
	sw	$0,36($sp)
	lui	$2,%hi(i2c_access_protect)
	addiu	$2,$2,%lo(i2c_access_protect)
	jalr	$2
	nop

	move	$3,$2
	li	$2,-1			# 0xffffffffffffffff
	bne	$3,$2,$L155
	nop

	lui	$2,%hi($LC70)
	addiu	$4,$2,%lo($LC70)
	lui	$2,%hi(__func__.25267)
	addiu	$5,$2,%lo(__func__.25267)
	li	$6,1676			# 0x68c
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	j	$L154
	nop

$L155:
	lui	$2,%hi(i2c_protect)
	li	$3,1			# 0x1
	sw	$3,%lo(i2c_protect)($2)
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,116($2)
	move	$3,$2
	li	$2,36			# 0x24
	sw	$2,16($sp)
	addiu	$2,$sp,48
	sw	$2,20($sp)
	li	$2,1			# 0x1
	sw	$2,24($sp)
	move	$4,$0
	move	$5,$3
	li	$6,112			# 0x70
	li	$7,2			# 0x2
	lui	$2,%hi(phy_I2C_read)
	addiu	$2,$2,%lo(phy_I2C_read)
	jalr	$2
	nop

	lbu	$2,48($sp)
	sw	$2,40($sp)
	lbu	$2,48($sp)
	seb	$3,$2
	li	$2,-31			# 0xffffffffffffffe1
	and	$2,$3,$2
	seb	$2,$2
	ori	$2,$2,0x6
	seb	$2,$2
	andi	$2,$2,0x00ff
	sb	$2,48($sp)
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,116($2)
	move	$3,$2
	li	$2,36			# 0x24
	sw	$2,16($sp)
	addiu	$2,$sp,48
	sw	$2,20($sp)
	li	$2,1			# 0x1
	sw	$2,24($sp)
	move	$4,$0
	move	$5,$3
	li	$6,112			# 0x70
	li	$7,2			# 0x2
	lui	$2,%hi(phy_I2C_write)
	addiu	$2,$2,%lo(phy_I2C_write)
	jalr	$2
	nop

	sw	$0,32($sp)
	j	$L157
	nop

$L158:
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,116($2)
	move	$3,$2
	li	$2,345			# 0x159
	sw	$2,16($sp)
	addiu	$2,$sp,48
	sw	$2,20($sp)
	li	$2,1			# 0x1
	sw	$2,24($sp)
	move	$4,$0
	move	$5,$3
	li	$6,112			# 0x70
	li	$7,2			# 0x2
	lui	$2,%hi(phy_I2C_read)
	addiu	$2,$2,%lo(phy_I2C_read)
	jalr	$2
	nop

	lbu	$2,48($sp)
	ori	$2,$2,0x10
	andi	$2,$2,0x00ff
	sb	$2,48($sp)
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,116($2)
	move	$3,$2
	li	$2,345			# 0x159
	sw	$2,16($sp)
	addiu	$2,$sp,48
	sw	$2,20($sp)
	li	$2,1			# 0x1
	sw	$2,24($sp)
	move	$4,$0
	move	$5,$3
	li	$6,112			# 0x70
	li	$7,2			# 0x2
	lui	$2,%hi(phy_I2C_write)
	addiu	$2,$2,%lo(phy_I2C_write)
	jalr	$2
	nop

	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,116($2)
	move	$3,$2
	li	$2,340			# 0x154
	sw	$2,16($sp)
	addiu	$2,$sp,48
	sw	$2,20($sp)
	li	$2,2			# 0x2
	sw	$2,24($sp)
	move	$4,$0
	move	$5,$3
	li	$6,112			# 0x70
	li	$7,2			# 0x2
	lui	$2,%hi(phy_I2C_read)
	addiu	$2,$2,%lo(phy_I2C_read)
	jalr	$2
	nop

	lbu	$2,48($sp)
	move	$3,$2
	lbu	$2,49($sp)
	sll	$2,$2,8
	or	$2,$3,$2
	move	$3,$2
	lw	$2,36($sp)
	addu	$2,$2,$3
	sw	$2,36($sp)
	lw	$2,32($sp)
	addiu	$2,$2,1
	sw	$2,32($sp)
$L157:
	lw	$3,32($sp)
	lw	$2,44($sp)
	sltu	$2,$3,$2
	bne	$2,$0,$L158
	nop

	lw	$2,40($sp)
	andi	$2,$2,0x00ff
	sb	$2,48($sp)
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,116($2)
	move	$3,$2
	li	$2,36			# 0x24
	sw	$2,16($sp)
	addiu	$2,$sp,48
	sw	$2,20($sp)
	li	$2,1			# 0x1
	sw	$2,24($sp)
	move	$4,$0
	move	$5,$3
	li	$6,112			# 0x70
	li	$7,2			# 0x2
	lui	$2,%hi(phy_I2C_write)
	addiu	$2,$2,%lo(phy_I2C_write)
	jalr	$2
	nop

	lw	$2,36($sp)
	srl	$2,$2,2
	addiu	$2,$2,1
	srl	$2,$2,1
	move	$3,$2
	lui	$2,%hi(BG1V76)
	sw	$3,%lo(BG1V76)($2)
	sw	$0,36($sp)
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,116($2)
	move	$3,$2
	li	$2,37			# 0x25
	sw	$2,16($sp)
	addiu	$2,$sp,48
	sw	$2,20($sp)
	li	$2,1			# 0x1
	sw	$2,24($sp)
	move	$4,$0
	move	$5,$3
	li	$6,112			# 0x70
	li	$7,2			# 0x2
	lui	$2,%hi(phy_I2C_read)
	addiu	$2,$2,%lo(phy_I2C_read)
	jalr	$2
	nop

	lbu	$2,48($sp)
	sw	$2,40($sp)
	lbu	$2,48($sp)
	ori	$2,$2,0x4c
	andi	$2,$2,0x00ff
	sb	$2,48($sp)
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,116($2)
	move	$3,$2
	li	$2,37			# 0x25
	sw	$2,16($sp)
	addiu	$2,$sp,48
	sw	$2,20($sp)
	li	$2,1			# 0x1
	sw	$2,24($sp)
	move	$4,$0
	move	$5,$3
	li	$6,112			# 0x70
	li	$7,2			# 0x2
	lui	$2,%hi(phy_I2C_write)
	addiu	$2,$2,%lo(phy_I2C_write)
	jalr	$2
	nop

	sw	$0,32($sp)
	j	$L159
	nop

$L160:
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,116($2)
	move	$3,$2
	li	$2,345			# 0x159
	sw	$2,16($sp)
	addiu	$2,$sp,48
	sw	$2,20($sp)
	li	$2,1			# 0x1
	sw	$2,24($sp)
	move	$4,$0
	move	$5,$3
	li	$6,112			# 0x70
	li	$7,2			# 0x2
	lui	$2,%hi(phy_I2C_read)
	addiu	$2,$2,%lo(phy_I2C_read)
	jalr	$2
	nop

	lbu	$2,48($sp)
	ori	$2,$2,0x10
	andi	$2,$2,0x00ff
	sb	$2,48($sp)
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,116($2)
	move	$3,$2
	li	$2,345			# 0x159
	sw	$2,16($sp)
	addiu	$2,$sp,48
	sw	$2,20($sp)
	li	$2,1			# 0x1
	sw	$2,24($sp)
	move	$4,$0
	move	$5,$3
	li	$6,112			# 0x70
	li	$7,2			# 0x2
	lui	$2,%hi(phy_I2C_write)
	addiu	$2,$2,%lo(phy_I2C_write)
	jalr	$2
	nop

	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,116($2)
	move	$3,$2
	li	$2,340			# 0x154
	sw	$2,16($sp)
	addiu	$2,$sp,48
	sw	$2,20($sp)
	li	$2,2			# 0x2
	sw	$2,24($sp)
	move	$4,$0
	move	$5,$3
	li	$6,112			# 0x70
	li	$7,2			# 0x2
	lui	$2,%hi(phy_I2C_read)
	addiu	$2,$2,%lo(phy_I2C_read)
	jalr	$2
	nop

	lbu	$2,48($sp)
	move	$3,$2
	lbu	$2,49($sp)
	sll	$2,$2,8
	or	$2,$3,$2
	move	$3,$2
	lw	$2,36($sp)
	addu	$2,$2,$3
	sw	$2,36($sp)
	lw	$2,32($sp)
	addiu	$2,$2,1
	sw	$2,32($sp)
$L159:
	lw	$3,32($sp)
	lw	$2,44($sp)
	sltu	$2,$3,$2
	bne	$2,$0,$L160
	nop

	lw	$2,40($sp)
	andi	$2,$2,0x00ff
	sb	$2,48($sp)
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,116($2)
	move	$3,$2
	li	$2,37			# 0x25
	sw	$2,16($sp)
	addiu	$2,$sp,48
	sw	$2,20($sp)
	li	$2,1			# 0x1
	sw	$2,24($sp)
	move	$4,$0
	move	$5,$3
	li	$6,112			# 0x70
	li	$7,2			# 0x2
	lui	$2,%hi(phy_I2C_write)
	addiu	$2,$2,%lo(phy_I2C_write)
	jalr	$2
	nop

	lui	$2,%hi(i2c_protect)
	sw	$0,%lo(i2c_protect)($2)
	lw	$2,36($sp)
	srl	$2,$2,2
	addiu	$2,$2,1
	srl	$2,$2,1
	move	$3,$2
	lui	$2,%hi(BG0V875)
	sw	$3,%lo(BG0V875)($2)
	sw	$0,36($sp)
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,136($2)
	andi	$2,$2,0x10
	beq	$2,$0,$L161
	nop

	lui	$2,%hi(jiffies)
	lw	$16,%lo(jiffies)($2)
	lui	$2,%hi($LC37)
	addiu	$4,$2,%lo($LC37)
	li	$5,47			# 0x2f
	lui	$2,%hi(strrchr)
	addiu	$2,$2,%lo(strrchr)
	jalr	$2
	nop

	addiu	$6,$2,1
	lui	$2,%hi(BG1V76)
	lw	$3,%lo(BG1V76)($2)
	lui	$2,%hi(BG0V875)
	lw	$2,%lo(BG0V875)($2)
	sw	$3,16($sp)
	sw	$2,20($sp)
	lui	$2,%hi($LC71)
	addiu	$4,$2,%lo($LC71)
	move	$5,$16
	li	$7,1727			# 0x6bf
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L161:
	lui	$2,%hi(BG1V76)
	lw	$3,%lo(BG1V76)($2)
	lui	$2,%hi(BG0V875)
	lw	$2,%lo(BG0V875)($2)
	subu	$2,$3,$2
	bgtz	$2,$L162
	nop

	lui	$2,%hi($LC72)
	addiu	$4,$2,%lo($LC72)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	j	$L154
	nop

$L162:
	lui	$2,%hi(BG1V76)
	lw	$3,%lo(BG1V76)($2)
	lui	$2,%hi(BG0V875)
	lw	$2,%lo(BG0V875)($2)
	subu	$3,$3,$2
	lui	$2,%hi(__floatsisf)
	addiu	$2,$2,%lo(__floatsisf)
	move	$4,$3
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(__extendsfdf2)
	addiu	$2,$2,%lo(__extendsfdf2)
	move	$4,$3
	jalr	$2
	nop

	move	$7,$3
	move	$6,$2
	lui	$3,%hi($LC73)
	lui	$2,%hi(__divdf3)
	addiu	$2,$2,%lo(__divdf3)
	lw	$5,%lo($LC73+4)($3)
	lw	$4,%lo($LC73)($3)
	jalr	$2
	nop

	move	$5,$3
	move	$4,$2
	lui	$2,%hi(__truncdfsf2)
	addiu	$2,$2,%lo(__truncdfsf2)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(ADC_slope)
	sw	$3,%lo(ADC_slope)($2)
	lui	$2,%hi(BG1V76)
	lw	$3,%lo(BG1V76)($2)
	lui	$2,%hi(__floatsisf)
	addiu	$2,$2,%lo(__floatsisf)
	move	$4,$3
	jalr	$2
	nop

	move	$4,$2
	lui	$2,%hi(ADC_slope)
	lw	$3,%lo(ADC_slope)($2)
	lui	$2,%hi(__mulsf3)
	addiu	$2,$2,%lo(__mulsf3)
	move	$5,$3
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(__extendsfdf2)
	addiu	$2,$2,%lo(__extendsfdf2)
	move	$4,$3
	jalr	$2
	nop

	move	$7,$3
	move	$6,$2
	lui	$3,%hi($LC74)
	lui	$2,%hi(__subdf3)
	addiu	$2,$2,%lo(__subdf3)
	lw	$5,%lo($LC74+4)($3)
	lw	$4,%lo($LC74)($3)
	jalr	$2
	nop

	move	$5,$3
	move	$4,$2
	lui	$2,%hi(__truncdfsf2)
	addiu	$2,$2,%lo(__truncdfsf2)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(ADC_offset)
	sw	$3,%lo(ADC_offset)($2)
$L154:
	lw	$31,60($sp)
	lw	$16,56($sp)
	addiu	$sp,$sp,64
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	mt7570_ADC_calibration
	.size	mt7570_ADC_calibration, .-mt7570_ADC_calibration
	.rdata
	.align	2
$LC75:
	.ascii	"APD initialization done\012\000"
	.section	.text.mt7570_APD_initialization,"ax",@progbits
	.align	2
	.globl	mt7570_APD_initialization
	.set	nomips16
	.set	nomicromips
	.ent	mt7570_APD_initialization
	.type	mt7570_APD_initialization, @function
mt7570_APD_initialization:
	.frame	$sp,48,$31		# vars= 8, regs= 1/0, args= 32, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-48
	sw	$31,44($sp)
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,116($2)
	move	$3,$2
	li	$2,48			# 0x30
	sw	$2,16($sp)
	addiu	$2,$sp,32
	sw	$2,20($sp)
	li	$2,4			# 0x4
	sw	$2,24($sp)
	move	$4,$0
	move	$5,$3
	li	$6,112			# 0x70
	li	$7,2			# 0x2
	lui	$2,%hi(phy_I2C_read)
	addiu	$2,$2,%lo(phy_I2C_read)
	jalr	$2
	nop

	lbu	$2,34($sp)
	ori	$2,$2,0x20
	andi	$2,$2,0x00ff
	sb	$2,34($sp)
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,116($2)
	move	$3,$2
	li	$2,48			# 0x30
	sw	$2,16($sp)
	addiu	$2,$sp,32
	sw	$2,20($sp)
	li	$2,4			# 0x4
	sw	$2,24($sp)
	move	$4,$0
	move	$5,$3
	li	$6,112			# 0x70
	li	$7,2			# 0x2
	lui	$2,%hi(phy_I2C_write)
	addiu	$2,$2,%lo(phy_I2C_write)
	jalr	$2
	nop

	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,116($2)
	move	$3,$2
	li	$2,48			# 0x30
	sw	$2,16($sp)
	addiu	$2,$sp,32
	sw	$2,20($sp)
	li	$2,4			# 0x4
	sw	$2,24($sp)
	move	$4,$0
	move	$5,$3
	li	$6,112			# 0x70
	li	$7,2			# 0x2
	lui	$2,%hi(phy_I2C_read)
	addiu	$2,$2,%lo(phy_I2C_read)
	jalr	$2
	nop

	lbu	$2,33($sp)
	ori	$2,$2,0x1
	andi	$2,$2,0x00ff
	sb	$2,33($sp)
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,116($2)
	move	$3,$2
	li	$2,48			# 0x30
	sw	$2,16($sp)
	addiu	$2,$sp,32
	sw	$2,20($sp)
	li	$2,4			# 0x4
	sw	$2,24($sp)
	move	$4,$0
	move	$5,$3
	li	$6,112			# 0x70
	li	$7,2			# 0x2
	lui	$2,%hi(phy_I2C_write)
	addiu	$2,$2,%lo(phy_I2C_write)
	jalr	$2
	nop

	lui	$2,%hi($LC75)
	addiu	$4,$2,%lo($LC75)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lw	$31,44($sp)
	addiu	$sp,$sp,48
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	mt7570_APD_initialization
	.size	mt7570_APD_initialization, .-mt7570_APD_initialization
	.globl	__divsf3
	.globl	__lesf2
	.rdata
	.align	2
$LC85:
	.ascii	"APD slope error\012\000"
	.globl	__gtsf2
	.globl	__addsf3
	.globl	__fixsfsi
	.align	2
$LC87:
	.ascii	"[%lu]0ms[%s:%d]\012  BOSA temperature = %d.%dC\012\000"
	.align	2
$LC88:
	.ascii	"[%lu]0ms[%s:%d]\012  APD Voltage = %d.%dV\012\000"
	.globl	__ltsf2
	.align	2
$LC90:
	.ascii	"[%lu]0ms[%s:%d]\012  APD slope 1\012\000"
	.align	2
$LC91:
	.ascii	"[%lu]0ms[%s:%d]\012  APD slope 2\012\000"
	.align	2
$LC92:
	.ascii	"[%lu]0ms[%s:%d]\012  APD slope 3\012\000"
	.align	2
$LC93:
	.ascii	"[%lu]0ms[%s:%d]\012  APD = 0x%x\012\000"
	.section	.text.mt7570_APD_control,"ax",@progbits
	.align	2
	.globl	mt7570_APD_control
	.set	nomips16
	.set	nomicromips
	.ent	mt7570_APD_control
	.type	mt7570_APD_control, @function
mt7570_APD_control:
	.frame	$sp,120,$31		# vars= 88, regs= 2/0, args= 24, gp= 0
	.mask	0x80010000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-120
	sw	$31,116($sp)
	sw	$16,112($sp)
	li	$4,48			# 0x30
	lui	$2,%hi(get_flash_register)
	addiu	$2,$2,%lo(get_flash_register)
	jalr	$2
	nop

	move	$3,$2
	li	$2,65536			# 0x10000
	slt	$2,$3,$2
	bne	$2,$0,$L166
	nop

	li	$4,52			# 0x34
	lui	$2,%hi(get_flash_register)
	addiu	$2,$2,%lo(get_flash_register)
	jalr	$2
	nop

	move	$3,$2
	li	$2,65536			# 0x10000
	slt	$2,$3,$2
	bne	$2,$0,$L166
	nop

	sw	$0,72($sp)
	sw	$0,76($sp)
	lui	$2,%hi($LC76)
	lw	$2,%lo($LC76)($2)
	sw	$2,24($sp)
	lui	$2,%hi($LC77)
	lw	$2,%lo($LC77)($2)
	sw	$2,28($sp)
	lui	$2,%hi($LC78)
	lw	$2,%lo($LC78)($2)
	sw	$2,32($sp)
	lui	$2,%hi($LC79)
	lw	$2,%lo($LC79)($2)
	sw	$2,80($sp)
	lui	$2,%hi($LC80)
	lw	$2,%lo($LC80)($2)
	sw	$2,36($sp)
	lui	$2,%hi($LC81)
	lw	$2,%lo($LC81)($2)
	sw	$2,40($sp)
	lui	$2,%hi($LC82)
	lw	$2,%lo($LC82)($2)
	sw	$2,44($sp)
	lui	$2,%hi($LC83)
	lw	$2,%lo($LC83)($2)
	sw	$2,48($sp)
	li	$4,16			# 0x10
	lui	$2,%hi(get_flash_register)
	addiu	$2,$2,%lo(get_flash_register)
	jalr	$2
	nop

	move	$3,$2
	li	$2,-1			# 0xffffffffffffffff
	beq	$3,$2,$L167
	nop

	li	$4,16			# 0x10
	lui	$2,%hi(get_flash_register)
	addiu	$2,$2,%lo(get_flash_register)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(__floatsisf)
	addiu	$2,$2,%lo(__floatsisf)
	move	$4,$3
	jalr	$2
	nop

	move	$4,$2
	lui	$3,%hi($LC84)
	lui	$2,%hi(__divsf3)
	addiu	$2,$2,%lo(__divsf3)
	lw	$5,%lo($LC84)($3)
	jalr	$2
	nop

	sw	$2,24($sp)
$L167:
	li	$4,20			# 0x14
	lui	$2,%hi(get_flash_register)
	addiu	$2,$2,%lo(get_flash_register)
	jalr	$2
	nop

	move	$3,$2
	li	$2,-1			# 0xffffffffffffffff
	beq	$3,$2,$L168
	nop

	li	$4,20			# 0x14
	lui	$2,%hi(get_flash_register)
	addiu	$2,$2,%lo(get_flash_register)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(__floatsisf)
	addiu	$2,$2,%lo(__floatsisf)
	move	$4,$3
	jalr	$2
	nop

	move	$4,$2
	lui	$3,%hi($LC84)
	lui	$2,%hi(__divsf3)
	addiu	$2,$2,%lo(__divsf3)
	lw	$5,%lo($LC84)($3)
	jalr	$2
	nop

	sw	$2,28($sp)
$L168:
	li	$4,24			# 0x18
	lui	$2,%hi(get_flash_register)
	addiu	$2,$2,%lo(get_flash_register)
	jalr	$2
	nop

	move	$3,$2
	li	$2,-1			# 0xffffffffffffffff
	beq	$3,$2,$L169
	nop

	li	$4,24			# 0x18
	lui	$2,%hi(get_flash_register)
	addiu	$2,$2,%lo(get_flash_register)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(__floatsisf)
	addiu	$2,$2,%lo(__floatsisf)
	move	$4,$3
	jalr	$2
	nop

	move	$4,$2
	lui	$3,%hi($LC84)
	lui	$2,%hi(__divsf3)
	addiu	$2,$2,%lo(__divsf3)
	lw	$5,%lo($LC84)($3)
	jalr	$2
	nop

	sw	$2,32($sp)
$L169:
	li	$4,48			# 0x30
	lui	$2,%hi(get_flash_register)
	addiu	$2,$2,%lo(get_flash_register)
	jalr	$2
	nop

	move	$16,$2
	li	$4,52			# 0x34
	lui	$2,%hi(get_flash_register)
	addiu	$2,$2,%lo(get_flash_register)
	jalr	$2
	nop

	or	$3,$16,$2
	li	$2,-1			# 0xffffffffffffffff
	beq	$3,$2,$L170
	nop

	li	$4,48			# 0x30
	lui	$2,%hi(get_flash_register)
	addiu	$2,$2,%lo(get_flash_register)
	jalr	$2
	nop

	srl	$3,$2,16
	lui	$2,%hi(__floatunsisf)
	addiu	$2,$2,%lo(__floatunsisf)
	move	$4,$3
	jalr	$2
	nop

	move	$4,$2
	lui	$3,%hi($LC16)
	lui	$2,%hi(__divsf3)
	addiu	$2,$2,%lo(__divsf3)
	lw	$5,%lo($LC16)($3)
	jalr	$2
	nop

	sw	$2,36($sp)
	li	$4,48			# 0x30
	lui	$2,%hi(get_flash_register)
	addiu	$2,$2,%lo(get_flash_register)
	jalr	$2
	nop

	andi	$3,$2,0xffff
	lui	$2,%hi(__floatsisf)
	addiu	$2,$2,%lo(__floatsisf)
	move	$4,$3
	jalr	$2
	nop

	move	$4,$2
	lui	$3,%hi($LC16)
	lui	$2,%hi(__divsf3)
	addiu	$2,$2,%lo(__divsf3)
	lw	$5,%lo($LC16)($3)
	jalr	$2
	nop

	sw	$2,40($sp)
	li	$4,52			# 0x34
	lui	$2,%hi(get_flash_register)
	addiu	$2,$2,%lo(get_flash_register)
	jalr	$2
	nop

	srl	$3,$2,16
	lui	$2,%hi(__floatunsisf)
	addiu	$2,$2,%lo(__floatunsisf)
	move	$4,$3
	jalr	$2
	nop

	move	$4,$2
	lui	$3,%hi($LC16)
	lui	$2,%hi(__divsf3)
	addiu	$2,$2,%lo(__divsf3)
	lw	$5,%lo($LC16)($3)
	jalr	$2
	nop

	sw	$2,44($sp)
	li	$4,52			# 0x34
	lui	$2,%hi(get_flash_register)
	addiu	$2,$2,%lo(get_flash_register)
	jalr	$2
	nop

	andi	$3,$2,0xffff
	lui	$2,%hi(__floatsisf)
	addiu	$2,$2,%lo(__floatsisf)
	move	$4,$3
	jalr	$2
	nop

	move	$4,$2
	lui	$3,%hi($LC16)
	lui	$2,%hi(__divsf3)
	addiu	$2,$2,%lo(__divsf3)
	lw	$5,%lo($LC16)($3)
	jalr	$2
	nop

	sw	$2,48($sp)
	lui	$2,%hi(__lesf2)
	addiu	$2,$2,%lo(__lesf2)
	lw	$4,40($sp)
	lw	$5,36($sp)
	jalr	$2
	nop

	blez	$2,$L171
	nop

	lui	$2,%hi(__lesf2)
	addiu	$2,$2,%lo(__lesf2)
	lw	$4,44($sp)
	lw	$5,40($sp)
	jalr	$2
	nop

	blez	$2,$L171
	nop

	lui	$2,%hi(__lesf2)
	addiu	$2,$2,%lo(__lesf2)
	lw	$4,48($sp)
	lw	$5,44($sp)
	jalr	$2
	nop

	bgtz	$2,$L170
	nop

$L171:
	lui	$2,%hi($LC85)
	addiu	$4,$2,%lo($LC85)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	j	$L165
	nop

$L170:
	lui	$2,%hi(BOSA_temperature)
	lw	$4,%lo(BOSA_temperature)($2)
	lui	$3,%hi($LC86)
	lui	$2,%hi(__gtsf2)
	addiu	$2,$2,%lo(__gtsf2)
	lw	$5,%lo($LC86)($3)
	jalr	$2
	nop

	blez	$2,$L208
	nop

	lui	$2,%hi(BOSA_temperature)
	lw	$4,%lo(BOSA_temperature)($2)
	lui	$3,%hi($LC86)
	lui	$2,%hi(__subsf3)
	addiu	$2,$2,%lo(__subsf3)
	lw	$5,%lo($LC86)($3)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(__mulsf3)
	addiu	$2,$2,%lo(__mulsf3)
	move	$4,$3
	lw	$5,24($sp)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(__addsf3)
	addiu	$2,$2,%lo(__addsf3)
	move	$4,$3
	lw	$5,32($sp)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(APD_voltage)
	sw	$3,%lo(APD_voltage)($2)
	j	$L176
	nop

$L208:
	lui	$2,%hi(BOSA_temperature)
	lw	$5,%lo(BOSA_temperature)($2)
	lui	$3,%hi($LC86)
	lui	$2,%hi(__subsf3)
	addiu	$2,$2,%lo(__subsf3)
	lw	$4,%lo($LC86)($3)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(__mulsf3)
	addiu	$2,$2,%lo(__mulsf3)
	move	$4,$3
	lw	$5,28($sp)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(__subsf3)
	addiu	$2,$2,%lo(__subsf3)
	lw	$4,32($sp)
	move	$5,$3
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(APD_voltage)
	sw	$3,%lo(APD_voltage)($2)
$L176:
	lui	$2,%hi(BOSA_temperature)
	lw	$3,%lo(BOSA_temperature)($2)
	lui	$2,%hi(__fixsfsi)
	addiu	$2,$2,%lo(__fixsfsi)
	move	$4,$3
	jalr	$2
	nop

	sw	$2,72($sp)
	lui	$2,%hi(BOSA_temperature)
	lw	$4,%lo(BOSA_temperature)($2)
	lui	$3,%hi($LC16)
	lui	$2,%hi(__mulsf3)
	addiu	$2,$2,%lo(__mulsf3)
	lw	$5,%lo($LC16)($3)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(__fixsfsi)
	addiu	$2,$2,%lo(__fixsfsi)
	move	$4,$3
	jalr	$2
	nop

	move	$3,$2
	li	$2,1717960704			# 0x66660000
	ori	$2,$2,0x6667
	mult	$3,$2
	mfhi	$2
	sra	$4,$2,2
	sra	$2,$3,31
	subu	$2,$4,$2
	sll	$2,$2,1
	sll	$4,$2,2
	addu	$2,$2,$4
	subu	$2,$3,$2
	sw	$2,84($sp)
	lw	$2,84($sp)
	bgez	$2,$L177
	nop

	subu	$2,$0,$2
$L177:
	sw	$2,76($sp)
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,136($2)
	andi	$2,$2,0x10
	beq	$2,$0,$L178
	nop

	lui	$2,%hi(jiffies)
	lw	$16,%lo(jiffies)($2)
	lui	$2,%hi($LC37)
	addiu	$4,$2,%lo($LC37)
	li	$5,47			# 0x2f
	lui	$2,%hi(strrchr)
	addiu	$2,$2,%lo(strrchr)
	jalr	$2
	nop

	addiu	$3,$2,1
	lw	$2,72($sp)
	sw	$2,16($sp)
	lw	$2,76($sp)
	sw	$2,20($sp)
	lui	$2,%hi($LC87)
	addiu	$4,$2,%lo($LC87)
	move	$5,$16
	move	$6,$3
	li	$7,1829			# 0x725
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L178:
	lui	$2,%hi(APD_voltage)
	lw	$3,%lo(APD_voltage)($2)
	lui	$2,%hi(__fixsfsi)
	addiu	$2,$2,%lo(__fixsfsi)
	move	$4,$3
	jalr	$2
	nop

	sw	$2,72($sp)
	lui	$2,%hi(APD_voltage)
	lw	$4,%lo(APD_voltage)($2)
	lui	$3,%hi($LC16)
	lui	$2,%hi(__mulsf3)
	addiu	$2,$2,%lo(__mulsf3)
	lw	$5,%lo($LC16)($3)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(__fixsfsi)
	addiu	$2,$2,%lo(__fixsfsi)
	move	$4,$3
	jalr	$2
	nop

	move	$3,$2
	li	$2,1717960704			# 0x66660000
	ori	$2,$2,0x6667
	mult	$3,$2
	mfhi	$2
	sra	$4,$2,2
	sra	$2,$3,31
	subu	$2,$4,$2
	sll	$2,$2,1
	sll	$4,$2,2
	addu	$2,$2,$4
	subu	$2,$3,$2
	sw	$2,76($sp)
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,136($2)
	andi	$2,$2,0x10
	beq	$2,$0,$L179
	nop

	lui	$2,%hi(jiffies)
	lw	$16,%lo(jiffies)($2)
	lui	$2,%hi($LC37)
	addiu	$4,$2,%lo($LC37)
	li	$5,47			# 0x2f
	lui	$2,%hi(strrchr)
	addiu	$2,$2,%lo(strrchr)
	jalr	$2
	nop

	addiu	$3,$2,1
	lw	$2,72($sp)
	sw	$2,16($sp)
	lw	$2,76($sp)
	sw	$2,20($sp)
	lui	$2,%hi($LC88)
	addiu	$4,$2,%lo($LC88)
	move	$5,$16
	move	$6,$3
	li	$7,1833			# 0x729
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L179:
	lui	$2,%hi(APD_voltage)
	lw	$3,%lo(APD_voltage)($2)
	lui	$2,%hi(__ltsf2)
	addiu	$2,$2,%lo(__ltsf2)
	move	$4,$3
	lw	$5,44($sp)
	jalr	$2
	nop

	bgez	$2,$L209
	nop

	lui	$2,%hi(APD_voltage)
	lw	$3,%lo(APD_voltage)($2)
	lui	$2,%hi(__ltsf2)
	addiu	$2,$2,%lo(__ltsf2)
	move	$4,$3
	lw	$5,40($sp)
	jalr	$2
	nop

	bgez	$2,$L210
	nop

	lui	$2,%hi(__subsf3)
	addiu	$2,$2,%lo(__subsf3)
	lw	$4,40($sp)
	lw	$5,36($sp)
	jalr	$2
	nop

	move	$4,$2
	lui	$3,%hi($LC89)
	lui	$2,%hi(__divsf3)
	addiu	$2,$2,%lo(__divsf3)
	lw	$5,%lo($LC89)($3)
	jalr	$2
	nop

	sw	$2,80($sp)
	lui	$2,%hi(APD_voltage)
	lw	$3,%lo(APD_voltage)($2)
	lui	$2,%hi(__subsf3)
	addiu	$2,$2,%lo(__subsf3)
	move	$4,$3
	lw	$5,36($sp)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(__divsf3)
	addiu	$2,$2,%lo(__divsf3)
	move	$4,$3
	lw	$5,80($sp)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(__fixsfsi)
	addiu	$2,$2,%lo(__fixsfsi)
	move	$4,$3
	jalr	$2
	nop

	andi	$2,$2,0x00ff
	sb	$2,100($sp)
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,136($2)
	andi	$2,$2,0x10
	beq	$2,$0,$L186
	nop

	lui	$2,%hi(jiffies)
	lw	$16,%lo(jiffies)($2)
	lui	$2,%hi($LC37)
	addiu	$4,$2,%lo($LC37)
	li	$5,47			# 0x2f
	lui	$2,%hi(strrchr)
	addiu	$2,$2,%lo(strrchr)
	jalr	$2
	nop

	addiu	$3,$2,1
	lui	$2,%hi($LC90)
	addiu	$4,$2,%lo($LC90)
	move	$5,$16
	move	$6,$3
	li	$7,1842			# 0x732
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	j	$L186
	nop

$L210:
	lui	$2,%hi(__subsf3)
	addiu	$2,$2,%lo(__subsf3)
	lw	$4,44($sp)
	lw	$5,40($sp)
	jalr	$2
	nop

	move	$4,$2
	lui	$3,%hi($LC89)
	lui	$2,%hi(__divsf3)
	addiu	$2,$2,%lo(__divsf3)
	lw	$5,%lo($LC89)($3)
	jalr	$2
	nop

	sw	$2,80($sp)
	lui	$2,%hi(APD_voltage)
	lw	$3,%lo(APD_voltage)($2)
	lui	$2,%hi(__subsf3)
	addiu	$2,$2,%lo(__subsf3)
	move	$4,$3
	lw	$5,40($sp)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(__divsf3)
	addiu	$2,$2,%lo(__divsf3)
	move	$4,$3
	lw	$5,80($sp)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(__fixsfsi)
	addiu	$2,$2,%lo(__fixsfsi)
	move	$4,$3
	jalr	$2
	nop

	andi	$2,$2,0x00ff
	addiu	$2,$2,64
	andi	$2,$2,0x00ff
	sb	$2,100($sp)
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,136($2)
	andi	$2,$2,0x10
	beq	$2,$0,$L186
	nop

	lui	$2,%hi(jiffies)
	lw	$16,%lo(jiffies)($2)
	lui	$2,%hi($LC37)
	addiu	$4,$2,%lo($LC37)
	li	$5,47			# 0x2f
	lui	$2,%hi(strrchr)
	addiu	$2,$2,%lo(strrchr)
	jalr	$2
	nop

	addiu	$3,$2,1
	lui	$2,%hi($LC91)
	addiu	$4,$2,%lo($LC91)
	move	$5,$16
	move	$6,$3
	li	$7,1848			# 0x738
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	j	$L186
	nop

$L209:
	lui	$2,%hi(__subsf3)
	addiu	$2,$2,%lo(__subsf3)
	lw	$4,48($sp)
	lw	$5,44($sp)
	jalr	$2
	nop

	move	$4,$2
	lui	$3,%hi($LC89)
	lui	$2,%hi(__divsf3)
	addiu	$2,$2,%lo(__divsf3)
	lw	$5,%lo($LC89)($3)
	jalr	$2
	nop

	sw	$2,80($sp)
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,136($2)
	andi	$2,$2,0x10
	beq	$2,$0,$L187
	nop

	lui	$2,%hi(jiffies)
	lw	$16,%lo(jiffies)($2)
	lui	$2,%hi($LC37)
	addiu	$4,$2,%lo($LC37)
	li	$5,47			# 0x2f
	lui	$2,%hi(strrchr)
	addiu	$2,$2,%lo(strrchr)
	jalr	$2
	nop

	addiu	$3,$2,1
	lui	$2,%hi($LC92)
	addiu	$4,$2,%lo($LC92)
	move	$5,$16
	move	$6,$3
	li	$7,1855			# 0x73f
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L187:
	lui	$2,%hi(APD_voltage)
	lw	$3,%lo(APD_voltage)($2)
	lui	$2,%hi(__ltsf2)
	addiu	$2,$2,%lo(__ltsf2)
	move	$4,$3
	lw	$5,48($sp)
	jalr	$2
	nop

	bgez	$2,$L211
	nop

	lui	$2,%hi(APD_voltage)
	lw	$3,%lo(APD_voltage)($2)
	lui	$2,%hi(__subsf3)
	addiu	$2,$2,%lo(__subsf3)
	move	$4,$3
	lw	$5,44($sp)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(__divsf3)
	addiu	$2,$2,%lo(__divsf3)
	move	$4,$3
	lw	$5,80($sp)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(__fixsfsi)
	addiu	$2,$2,%lo(__fixsfsi)
	move	$4,$3
	jalr	$2
	nop

	andi	$2,$2,0x00ff
	addiu	$2,$2,-128
	andi	$2,$2,0x00ff
	sb	$2,100($sp)
	j	$L186
	nop

$L211:
	lui	$2,%hi(APD_voltage)
	lw	$3,%lo(APD_voltage)($2)
	lui	$2,%hi(__subsf3)
	addiu	$2,$2,%lo(__subsf3)
	move	$4,$3
	lw	$5,48($sp)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(__divsf3)
	addiu	$2,$2,%lo(__divsf3)
	move	$4,$3
	lw	$5,80($sp)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(__fixsfsi)
	addiu	$2,$2,%lo(__fixsfsi)
	move	$4,$3
	jalr	$2
	nop

	andi	$2,$2,0x00ff
	addiu	$2,$2,-64
	andi	$2,$2,0x00ff
	sb	$2,100($sp)
$L186:
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,136($2)
	andi	$2,$2,0x10
	beq	$2,$0,$L190
	nop

	lui	$2,%hi(jiffies)
	lw	$16,%lo(jiffies)($2)
	lui	$2,%hi($LC37)
	addiu	$4,$2,%lo($LC37)
	li	$5,47			# 0x2f
	lui	$2,%hi(strrchr)
	addiu	$2,$2,%lo(strrchr)
	jalr	$2
	nop

	addiu	$3,$2,1
	lbu	$2,100($sp)
	sw	$2,16($sp)
	lui	$2,%hi($LC93)
	addiu	$4,$2,%lo($LC93)
	move	$5,$16
	move	$6,$3
	li	$7,1867			# 0x74b
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L190:
	addiu	$2,$sp,100
	move	$4,$2
	lui	$2,%hi(mt7570_APD_I2C_write)
	addiu	$2,$2,%lo(mt7570_APD_I2C_write)
	jalr	$2
	nop

	j	$L165
	nop

$L166:
	sw	$0,88($sp)
	sw	$0,92($sp)
	lui	$2,%hi($LC76)
	lw	$2,%lo($LC76)($2)
	sw	$2,52($sp)
	lui	$2,%hi($LC77)
	lw	$2,%lo($LC77)($2)
	sw	$2,56($sp)
	lui	$2,%hi($LC78)
	lw	$2,%lo($LC78)($2)
	sw	$2,60($sp)
	lui	$2,%hi($LC80)
	lw	$2,%lo($LC80)($2)
	sw	$2,64($sp)
	lui	$2,%hi($LC79)
	lw	$2,%lo($LC79)($2)
	sw	$2,68($sp)
	li	$4,16			# 0x10
	lui	$2,%hi(get_flash_register)
	addiu	$2,$2,%lo(get_flash_register)
	jalr	$2
	nop

	move	$3,$2
	li	$2,-1			# 0xffffffffffffffff
	beq	$3,$2,$L191
	nop

	li	$4,16			# 0x10
	lui	$2,%hi(get_flash_register)
	addiu	$2,$2,%lo(get_flash_register)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(__floatsisf)
	addiu	$2,$2,%lo(__floatsisf)
	move	$4,$3
	jalr	$2
	nop

	move	$4,$2
	lui	$3,%hi($LC84)
	lui	$2,%hi(__divsf3)
	addiu	$2,$2,%lo(__divsf3)
	lw	$5,%lo($LC84)($3)
	jalr	$2
	nop

	sw	$2,52($sp)
$L191:
	li	$4,20			# 0x14
	lui	$2,%hi(get_flash_register)
	addiu	$2,$2,%lo(get_flash_register)
	jalr	$2
	nop

	move	$3,$2
	li	$2,-1			# 0xffffffffffffffff
	beq	$3,$2,$L192
	nop

	li	$4,20			# 0x14
	lui	$2,%hi(get_flash_register)
	addiu	$2,$2,%lo(get_flash_register)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(__floatsisf)
	addiu	$2,$2,%lo(__floatsisf)
	move	$4,$3
	jalr	$2
	nop

	move	$4,$2
	lui	$3,%hi($LC84)
	lui	$2,%hi(__divsf3)
	addiu	$2,$2,%lo(__divsf3)
	lw	$5,%lo($LC84)($3)
	jalr	$2
	nop

	sw	$2,56($sp)
$L192:
	li	$4,24			# 0x18
	lui	$2,%hi(get_flash_register)
	addiu	$2,$2,%lo(get_flash_register)
	jalr	$2
	nop

	move	$3,$2
	li	$2,-1			# 0xffffffffffffffff
	beq	$3,$2,$L193
	nop

	li	$4,24			# 0x18
	lui	$2,%hi(get_flash_register)
	addiu	$2,$2,%lo(get_flash_register)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(__floatsisf)
	addiu	$2,$2,%lo(__floatsisf)
	move	$4,$3
	jalr	$2
	nop

	move	$4,$2
	lui	$3,%hi($LC84)
	lui	$2,%hi(__divsf3)
	addiu	$2,$2,%lo(__divsf3)
	lw	$5,%lo($LC84)($3)
	jalr	$2
	nop

	sw	$2,60($sp)
$L193:
	li	$4,52			# 0x34
	lui	$2,%hi(get_flash_register)
	addiu	$2,$2,%lo(get_flash_register)
	jalr	$2
	nop

	move	$3,$2
	li	$2,-1			# 0xffffffffffffffff
	beq	$3,$2,$L194
	nop

	li	$4,52			# 0x34
	lui	$2,%hi(get_flash_register)
	addiu	$2,$2,%lo(get_flash_register)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(__floatsisf)
	addiu	$2,$2,%lo(__floatsisf)
	move	$4,$3
	jalr	$2
	nop

	move	$4,$2
	lui	$3,%hi($LC84)
	lui	$2,%hi(__divsf3)
	addiu	$2,$2,%lo(__divsf3)
	lw	$5,%lo($LC84)($3)
	jalr	$2
	nop

	sw	$2,64($sp)
$L194:
	li	$4,48			# 0x30
	lui	$2,%hi(get_flash_register)
	addiu	$2,$2,%lo(get_flash_register)
	jalr	$2
	nop

	move	$3,$2
	li	$2,-1			# 0xffffffffffffffff
	beq	$3,$2,$L195
	nop

	li	$4,48			# 0x30
	lui	$2,%hi(get_flash_register)
	addiu	$2,$2,%lo(get_flash_register)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(__floatsisf)
	addiu	$2,$2,%lo(__floatsisf)
	move	$4,$3
	jalr	$2
	nop

	move	$4,$2
	lui	$3,%hi($LC94)
	lui	$2,%hi(__divsf3)
	addiu	$2,$2,%lo(__divsf3)
	lw	$5,%lo($LC94)($3)
	jalr	$2
	nop

	sw	$2,68($sp)
$L195:
	lui	$2,%hi(BOSA_temperature)
	lw	$4,%lo(BOSA_temperature)($2)
	lui	$3,%hi($LC86)
	lui	$2,%hi(__gtsf2)
	addiu	$2,$2,%lo(__gtsf2)
	lw	$5,%lo($LC86)($3)
	jalr	$2
	nop

	blez	$2,$L212
	nop

	lui	$2,%hi(BOSA_temperature)
	lw	$4,%lo(BOSA_temperature)($2)
	lui	$3,%hi($LC86)
	lui	$2,%hi(__subsf3)
	addiu	$2,$2,%lo(__subsf3)
	lw	$5,%lo($LC86)($3)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(__mulsf3)
	addiu	$2,$2,%lo(__mulsf3)
	move	$4,$3
	lw	$5,52($sp)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(__addsf3)
	addiu	$2,$2,%lo(__addsf3)
	move	$4,$3
	lw	$5,60($sp)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(APD_voltage)
	sw	$3,%lo(APD_voltage)($2)
	j	$L198
	nop

$L212:
	lui	$2,%hi(BOSA_temperature)
	lw	$5,%lo(BOSA_temperature)($2)
	lui	$3,%hi($LC86)
	lui	$2,%hi(__subsf3)
	addiu	$2,$2,%lo(__subsf3)
	lw	$4,%lo($LC86)($3)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(__mulsf3)
	addiu	$2,$2,%lo(__mulsf3)
	move	$4,$3
	lw	$5,56($sp)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(__subsf3)
	addiu	$2,$2,%lo(__subsf3)
	lw	$4,60($sp)
	move	$5,$3
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(APD_voltage)
	sw	$3,%lo(APD_voltage)($2)
$L198:
	lui	$2,%hi(BOSA_temperature)
	lw	$3,%lo(BOSA_temperature)($2)
	lui	$2,%hi(__fixsfsi)
	addiu	$2,$2,%lo(__fixsfsi)
	move	$4,$3
	jalr	$2
	nop

	sw	$2,88($sp)
	lui	$2,%hi(BOSA_temperature)
	lw	$4,%lo(BOSA_temperature)($2)
	lui	$3,%hi($LC16)
	lui	$2,%hi(__mulsf3)
	addiu	$2,$2,%lo(__mulsf3)
	lw	$5,%lo($LC16)($3)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(__fixsfsi)
	addiu	$2,$2,%lo(__fixsfsi)
	move	$4,$3
	jalr	$2
	nop

	move	$3,$2
	li	$2,1717960704			# 0x66660000
	ori	$2,$2,0x6667
	mult	$3,$2
	mfhi	$2
	sra	$4,$2,2
	sra	$2,$3,31
	subu	$2,$4,$2
	sll	$2,$2,1
	sll	$4,$2,2
	addu	$2,$2,$4
	subu	$2,$3,$2
	sw	$2,96($sp)
	lw	$2,96($sp)
	bgez	$2,$L199
	nop

	subu	$2,$0,$2
$L199:
	sw	$2,92($sp)
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,136($2)
	andi	$2,$2,0x10
	beq	$2,$0,$L200
	nop

	lui	$2,%hi(jiffies)
	lw	$16,%lo(jiffies)($2)
	lui	$2,%hi($LC37)
	addiu	$4,$2,%lo($LC37)
	li	$5,47			# 0x2f
	lui	$2,%hi(strrchr)
	addiu	$2,$2,%lo(strrchr)
	jalr	$2
	nop

	addiu	$3,$2,1
	lw	$2,88($sp)
	sw	$2,16($sp)
	lw	$2,92($sp)
	sw	$2,20($sp)
	lui	$2,%hi($LC87)
	addiu	$4,$2,%lo($LC87)
	move	$5,$16
	move	$6,$3
	li	$7,1906			# 0x772
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L200:
	lui	$2,%hi(APD_voltage)
	lw	$3,%lo(APD_voltage)($2)
	lui	$2,%hi(__fixsfsi)
	addiu	$2,$2,%lo(__fixsfsi)
	move	$4,$3
	jalr	$2
	nop

	sw	$2,88($sp)
	lui	$2,%hi(APD_voltage)
	lw	$4,%lo(APD_voltage)($2)
	lui	$3,%hi($LC16)
	lui	$2,%hi(__mulsf3)
	addiu	$2,$2,%lo(__mulsf3)
	lw	$5,%lo($LC16)($3)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(__fixsfsi)
	addiu	$2,$2,%lo(__fixsfsi)
	move	$4,$3
	jalr	$2
	nop

	move	$3,$2
	li	$2,1717960704			# 0x66660000
	ori	$2,$2,0x6667
	mult	$3,$2
	mfhi	$2
	sra	$4,$2,2
	sra	$2,$3,31
	subu	$2,$4,$2
	sll	$2,$2,1
	sll	$4,$2,2
	addu	$2,$2,$4
	subu	$2,$3,$2
	sw	$2,92($sp)
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,136($2)
	andi	$2,$2,0x10
	beq	$2,$0,$L201
	nop

	lui	$2,%hi(jiffies)
	lw	$16,%lo(jiffies)($2)
	lui	$2,%hi($LC37)
	addiu	$4,$2,%lo($LC37)
	li	$5,47			# 0x2f
	lui	$2,%hi(strrchr)
	addiu	$2,$2,%lo(strrchr)
	jalr	$2
	nop

	addiu	$3,$2,1
	lw	$2,88($sp)
	sw	$2,16($sp)
	lw	$2,92($sp)
	sw	$2,20($sp)
	lui	$2,%hi($LC88)
	addiu	$4,$2,%lo($LC88)
	move	$5,$16
	move	$6,$3
	li	$7,1910			# 0x776
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L201:
	lui	$2,%hi(APD_voltage)
	lw	$3,%lo(APD_voltage)($2)
	lui	$2,%hi(__subsf3)
	addiu	$2,$2,%lo(__subsf3)
	move	$4,$3
	lw	$5,64($sp)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(__divsf3)
	addiu	$2,$2,%lo(__divsf3)
	move	$4,$3
	lw	$5,68($sp)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(__fixsfsi)
	addiu	$2,$2,%lo(__fixsfsi)
	move	$4,$3
	jalr	$2
	nop

	andi	$2,$2,0x00ff
	sb	$2,104($sp)
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,136($2)
	andi	$2,$2,0x10
	beq	$2,$0,$L202
	nop

	lui	$2,%hi(jiffies)
	lw	$16,%lo(jiffies)($2)
	lui	$2,%hi($LC37)
	addiu	$4,$2,%lo($LC37)
	li	$5,47			# 0x2f
	lui	$2,%hi(strrchr)
	addiu	$2,$2,%lo(strrchr)
	jalr	$2
	nop

	addiu	$3,$2,1
	lbu	$2,104($sp)
	sw	$2,16($sp)
	lui	$2,%hi($LC93)
	addiu	$4,$2,%lo($LC93)
	move	$5,$16
	move	$6,$3
	li	$7,1914			# 0x77a
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L202:
	addiu	$2,$sp,104
	move	$4,$2
	lui	$2,%hi(mt7570_APD_I2C_write)
	addiu	$2,$2,%lo(mt7570_APD_I2C_write)
	jalr	$2
	nop

$L165:
	lw	$31,116($sp)
	lw	$16,112($sp)
	addiu	$sp,$sp,120
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	mt7570_APD_control
	.size	mt7570_APD_control, .-mt7570_APD_control
	.rdata
	.align	2
$LC95:
	.ascii	"[%lu]0ms[%s:%d]\012  APD in mt7570_APD_I2C_write = 0x%x\012"
	.ascii	"\000"
	.section	.text.mt7570_APD_I2C_write,"ax",@progbits
	.align	2
	.globl	mt7570_APD_I2C_write
	.set	nomips16
	.set	nomicromips
	.ent	mt7570_APD_I2C_write
	.type	mt7570_APD_I2C_write, @function
mt7570_APD_I2C_write:
	.frame	$sp,40,$31		# vars= 0, regs= 2/0, args= 32, gp= 0
	.mask	0x80010000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-40
	sw	$31,36($sp)
	sw	$16,32($sp)
	sw	$4,40($sp)
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,136($2)
	andi	$2,$2,0x10
	beq	$2,$0,$L214
	nop

	lui	$2,%hi(jiffies)
	lw	$16,%lo(jiffies)($2)
	lui	$2,%hi($LC37)
	addiu	$4,$2,%lo($LC37)
	li	$5,47			# 0x2f
	lui	$2,%hi(strrchr)
	addiu	$2,$2,%lo(strrchr)
	jalr	$2
	nop

	addiu	$3,$2,1
	lw	$2,40($sp)
	lbu	$2,0($2)
	sw	$2,16($sp)
	lui	$2,%hi($LC95)
	addiu	$4,$2,%lo($LC95)
	move	$5,$16
	move	$6,$3
	li	$7,1937			# 0x791
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L214:
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,116($2)
	move	$3,$2
	li	$2,48			# 0x30
	sw	$2,16($sp)
	lw	$2,40($sp)
	sw	$2,20($sp)
	li	$2,1			# 0x1
	sw	$2,24($sp)
	move	$4,$0
	move	$5,$3
	li	$6,112			# 0x70
	li	$7,2			# 0x2
	lui	$2,%hi(phy_I2C_write)
	addiu	$2,$2,%lo(phy_I2C_write)
	jalr	$2
	nop

	lw	$31,36($sp)
	lw	$16,32($sp)
	addiu	$sp,$sp,40
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	mt7570_APD_I2C_write
	.size	mt7570_APD_I2C_write, .-mt7570_APD_I2C_write
	.section	.text.mt7570_information_output,"ax",@progbits
	.align	2
	.globl	mt7570_information_output
	.set	nomips16
	.set	nomicromips
	.ent	mt7570_information_output
	.type	mt7570_information_output, @function
mt7570_information_output:
	.frame	$sp,48,$31		# vars= 8, regs= 1/0, args= 32, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-48
	sw	$31,44($sp)
	sw	$4,48($sp)
	sw	$0,32($sp)
	lw	$3,48($sp)
	li	$2,3			# 0x3
	bne	$3,$2,$L216
	nop

	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,116($2)
	move	$3,$2
	li	$2,316			# 0x13c
	sw	$2,16($sp)
	addiu	$2,$sp,36
	sw	$2,20($sp)
	li	$2,4			# 0x4
	sw	$2,24($sp)
	move	$4,$0
	move	$5,$3
	li	$6,112			# 0x70
	li	$7,2			# 0x2
	lui	$2,%hi(phy_I2C_read)
	addiu	$2,$2,%lo(phy_I2C_read)
	jalr	$2
	nop

	lbu	$2,38($sp)
	move	$3,$2
	lbu	$2,39($sp)
	andi	$2,$2,0xf
	sll	$2,$2,8
	or	$2,$3,$2
	sw	$2,32($sp)
	lui	$2,%hi(global_bias_current)
	lw	$3,32($sp)
	sw	$3,%lo(global_bias_current)($2)
	j	$L217
	nop

$L216:
	lw	$3,48($sp)
	li	$2,4			# 0x4
	bne	$3,$2,$L218
	nop

	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,116($2)
	move	$3,$2
	li	$2,332			# 0x14c
	sw	$2,16($sp)
	addiu	$2,$sp,36
	sw	$2,20($sp)
	li	$2,4			# 0x4
	sw	$2,24($sp)
	move	$4,$0
	move	$5,$3
	li	$6,112			# 0x70
	li	$7,2			# 0x2
	lui	$2,%hi(phy_I2C_read)
	addiu	$2,$2,%lo(phy_I2C_read)
	jalr	$2
	nop

	lbu	$2,38($sp)
	move	$3,$2
	lbu	$2,39($sp)
	andi	$2,$2,0xf
	sll	$2,$2,8
	or	$2,$3,$2
	sw	$2,32($sp)
	lui	$2,%hi(global_modulation_current)
	lw	$3,32($sp)
	sw	$3,%lo(global_modulation_current)($2)
	j	$L217
	nop

$L218:
	lw	$3,48($sp)
	li	$2,5			# 0x5
	bne	$3,$2,$L219
	nop

	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,116($2)
	move	$3,$2
	li	$2,4			# 0x4
	sw	$2,16($sp)
	addiu	$2,$sp,36
	sw	$2,20($sp)
	li	$2,4			# 0x4
	sw	$2,24($sp)
	move	$4,$0
	move	$5,$3
	li	$6,112			# 0x70
	li	$7,2			# 0x2
	lui	$2,%hi(phy_I2C_read)
	addiu	$2,$2,%lo(phy_I2C_read)
	jalr	$2
	nop

	lbu	$2,38($sp)
	move	$3,$2
	lbu	$2,39($sp)
	andi	$2,$2,0x3
	sll	$2,$2,8
	or	$2,$3,$2
	sw	$2,32($sp)
	lui	$2,%hi(global_MPDL)
	lw	$3,32($sp)
	sw	$3,%lo(global_MPDL)($2)
	j	$L217
	nop

$L219:
	lw	$3,48($sp)
	li	$2,6			# 0x6
	bne	$3,$2,$L217
	nop

	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,116($2)
	move	$3,$2
	li	$2,4			# 0x4
	sw	$2,16($sp)
	addiu	$2,$sp,36
	sw	$2,20($sp)
	li	$2,4			# 0x4
	sw	$2,24($sp)
	move	$4,$0
	move	$5,$3
	li	$6,112			# 0x70
	li	$7,2			# 0x2
	lui	$2,%hi(phy_I2C_read)
	addiu	$2,$2,%lo(phy_I2C_read)
	jalr	$2
	nop

	lbu	$2,36($sp)
	move	$3,$2
	lbu	$2,37($sp)
	andi	$2,$2,0x3
	sll	$2,$2,8
	or	$2,$3,$2
	sw	$2,32($sp)
	lui	$2,%hi(global_MPDH)
	lw	$3,32($sp)
	sw	$3,%lo(global_MPDH)($2)
$L217:
	lw	$2,32($sp)
	lw	$31,44($sp)
	addiu	$sp,$sp,48
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	mt7570_information_output
	.size	mt7570_information_output, .-mt7570_information_output
	.section	.text.mt7570_ADC_temperature_get,"ax",@progbits
	.align	2
	.globl	mt7570_ADC_temperature_get
	.set	nomips16
	.set	nomicromips
	.ent	mt7570_ADC_temperature_get
	.type	mt7570_ADC_temperature_get, @function
mt7570_ADC_temperature_get:
	.frame	$sp,56,$31		# vars= 16, regs= 1/0, args= 32, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-56
	sw	$31,52($sp)
	li	$2,8			# 0x8
	sw	$2,40($sp)
	sw	$0,32($sp)
	sw	$0,36($sp)
	lui	$2,%hi(i2c_access_protect)
	addiu	$2,$2,%lo(i2c_access_protect)
	jalr	$2
	nop

	move	$3,$2
	li	$2,-1			# 0xffffffffffffffff
	bne	$3,$2,$L222
	nop

	lui	$2,%hi($LC70)
	addiu	$4,$2,%lo($LC70)
	lui	$2,%hi(__func__.25319)
	addiu	$5,$2,%lo(__func__.25319)
	li	$6,2004			# 0x7d4
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi(global_temperature_code)
	lw	$2,%lo(global_temperature_code)($2)
	j	$L226
	nop

$L222:
	lui	$2,%hi(i2c_protect)
	li	$3,1			# 0x1
	sw	$3,%lo(i2c_protect)($2)
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,116($2)
	move	$3,$2
	li	$2,36			# 0x24
	sw	$2,16($sp)
	addiu	$2,$sp,44
	sw	$2,20($sp)
	li	$2,1			# 0x1
	sw	$2,24($sp)
	move	$4,$0
	move	$5,$3
	li	$6,112			# 0x70
	li	$7,2			# 0x2
	lui	$2,%hi(phy_I2C_read)
	addiu	$2,$2,%lo(phy_I2C_read)
	jalr	$2
	nop

	lbu	$2,44($sp)
	seb	$3,$2
	li	$2,-31			# 0xffffffffffffffe1
	and	$2,$3,$2
	seb	$2,$2
	ori	$2,$2,0x8
	seb	$2,$2
	andi	$2,$2,0x00ff
	sb	$2,44($sp)
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,116($2)
	move	$3,$2
	li	$2,36			# 0x24
	sw	$2,16($sp)
	addiu	$2,$sp,44
	sw	$2,20($sp)
	li	$2,1			# 0x1
	sw	$2,24($sp)
	move	$4,$0
	move	$5,$3
	li	$6,112			# 0x70
	li	$7,2			# 0x2
	lui	$2,%hi(phy_I2C_write)
	addiu	$2,$2,%lo(phy_I2C_write)
	jalr	$2
	nop

	sw	$0,32($sp)
	j	$L224
	nop

$L225:
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,116($2)
	move	$3,$2
	li	$2,345			# 0x159
	sw	$2,16($sp)
	addiu	$2,$sp,44
	sw	$2,20($sp)
	li	$2,1			# 0x1
	sw	$2,24($sp)
	move	$4,$0
	move	$5,$3
	li	$6,112			# 0x70
	li	$7,2			# 0x2
	lui	$2,%hi(phy_I2C_read)
	addiu	$2,$2,%lo(phy_I2C_read)
	jalr	$2
	nop

	lbu	$2,44($sp)
	ori	$2,$2,0x10
	andi	$2,$2,0x00ff
	sb	$2,44($sp)
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,116($2)
	move	$3,$2
	li	$2,345			# 0x159
	sw	$2,16($sp)
	addiu	$2,$sp,44
	sw	$2,20($sp)
	li	$2,1			# 0x1
	sw	$2,24($sp)
	move	$4,$0
	move	$5,$3
	li	$6,112			# 0x70
	li	$7,2			# 0x2
	lui	$2,%hi(phy_I2C_write)
	addiu	$2,$2,%lo(phy_I2C_write)
	jalr	$2
	nop

	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,116($2)
	move	$3,$2
	li	$2,340			# 0x154
	sw	$2,16($sp)
	addiu	$2,$sp,44
	sw	$2,20($sp)
	li	$2,2			# 0x2
	sw	$2,24($sp)
	move	$4,$0
	move	$5,$3
	li	$6,112			# 0x70
	li	$7,2			# 0x2
	lui	$2,%hi(phy_I2C_read)
	addiu	$2,$2,%lo(phy_I2C_read)
	jalr	$2
	nop

	lbu	$2,44($sp)
	move	$3,$2
	lbu	$2,45($sp)
	sll	$2,$2,8
	or	$2,$3,$2
	move	$3,$2
	lw	$2,36($sp)
	addu	$2,$2,$3
	sw	$2,36($sp)
	lw	$2,32($sp)
	addiu	$2,$2,1
	sw	$2,32($sp)
$L224:
	lw	$3,32($sp)
	lw	$2,40($sp)
	sltu	$2,$3,$2
	bne	$2,$0,$L225
	nop

	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,116($2)
	move	$3,$2
	li	$2,36			# 0x24
	sw	$2,16($sp)
	addiu	$2,$sp,44
	sw	$2,20($sp)
	li	$2,1			# 0x1
	sw	$2,24($sp)
	move	$4,$0
	move	$5,$3
	li	$6,112			# 0x70
	li	$7,2			# 0x2
	lui	$2,%hi(phy_I2C_read)
	addiu	$2,$2,%lo(phy_I2C_read)
	jalr	$2
	nop

	lbu	$3,44($sp)
	li	$2,-31			# 0xffffffffffffffe1
	and	$2,$3,$2
	andi	$2,$2,0x00ff
	sb	$2,44($sp)
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,116($2)
	move	$3,$2
	li	$2,36			# 0x24
	sw	$2,16($sp)
	addiu	$2,$sp,44
	sw	$2,20($sp)
	li	$2,1			# 0x1
	sw	$2,24($sp)
	move	$4,$0
	move	$5,$3
	li	$6,112			# 0x70
	li	$7,2			# 0x2
	lui	$2,%hi(phy_I2C_write)
	addiu	$2,$2,%lo(phy_I2C_write)
	jalr	$2
	nop

	lui	$2,%hi(i2c_protect)
	sw	$0,%lo(i2c_protect)($2)
	lw	$2,36($sp)
	srl	$2,$2,2
	addiu	$2,$2,1
	srl	$2,$2,1
	move	$3,$2
	lui	$2,%hi(global_temperature_code)
	sw	$3,%lo(global_temperature_code)($2)
	sw	$0,36($sp)
	lui	$2,%hi(global_temperature_code)
	lw	$2,%lo(global_temperature_code)($2)
$L226:
	lw	$31,52($sp)
	addiu	$sp,$sp,56
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	mt7570_ADC_temperature_get
	.size	mt7570_ADC_temperature_get, .-mt7570_ADC_temperature_get
	.section	.text.mt7570_ADC_voltage_get,"ax",@progbits
	.align	2
	.globl	mt7570_ADC_voltage_get
	.set	nomips16
	.set	nomicromips
	.ent	mt7570_ADC_voltage_get
	.type	mt7570_ADC_voltage_get, @function
mt7570_ADC_voltage_get:
	.frame	$sp,48,$31		# vars= 8, regs= 1/0, args= 32, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-48
	sw	$31,44($sp)
	sw	$0,32($sp)
	lui	$2,%hi(i2c_access_protect)
	addiu	$2,$2,%lo(i2c_access_protect)
	jalr	$2
	nop

	move	$3,$2
	li	$2,-1			# 0xffffffffffffffff
	bne	$3,$2,$L228
	nop

	lui	$2,%hi($LC70)
	addiu	$4,$2,%lo($LC70)
	lui	$2,%hi(__func__.25328)
	addiu	$5,$2,%lo(__func__.25328)
	li	$6,2057			# 0x809
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi(global_vcc_code)
	lw	$2,%lo(global_vcc_code)($2)
	j	$L230
	nop

$L228:
	lui	$2,%hi(i2c_protect)
	li	$3,1			# 0x1
	sw	$3,%lo(i2c_protect)($2)
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,116($2)
	move	$3,$2
	li	$2,20			# 0x14
	sw	$2,16($sp)
	addiu	$2,$sp,36
	sw	$2,20($sp)
	li	$2,1			# 0x1
	sw	$2,24($sp)
	move	$4,$0
	move	$5,$3
	li	$6,112			# 0x70
	li	$7,2			# 0x2
	lui	$2,%hi(phy_I2C_read)
	addiu	$2,$2,%lo(phy_I2C_read)
	jalr	$2
	nop

	lbu	$2,36($sp)
	ori	$2,$2,0x10
	andi	$2,$2,0x00ff
	sb	$2,36($sp)
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,116($2)
	move	$3,$2
	li	$2,20			# 0x14
	sw	$2,16($sp)
	addiu	$2,$sp,36
	sw	$2,20($sp)
	li	$2,1			# 0x1
	sw	$2,24($sp)
	move	$4,$0
	move	$5,$3
	li	$6,112			# 0x70
	li	$7,2			# 0x2
	lui	$2,%hi(phy_I2C_write)
	addiu	$2,$2,%lo(phy_I2C_write)
	jalr	$2
	nop

	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,116($2)
	move	$3,$2
	li	$2,36			# 0x24
	sw	$2,16($sp)
	addiu	$2,$sp,36
	sw	$2,20($sp)
	li	$2,1			# 0x1
	sw	$2,24($sp)
	move	$4,$0
	move	$5,$3
	li	$6,112			# 0x70
	li	$7,2			# 0x2
	lui	$2,%hi(phy_I2C_read)
	addiu	$2,$2,%lo(phy_I2C_read)
	jalr	$2
	nop

	lbu	$2,36($sp)
	sw	$2,32($sp)
	lbu	$3,36($sp)
	li	$2,-31			# 0xffffffffffffffe1
	and	$2,$3,$2
	andi	$2,$2,0x00ff
	sb	$2,36($sp)
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,116($2)
	move	$3,$2
	li	$2,36			# 0x24
	sw	$2,16($sp)
	addiu	$2,$sp,36
	sw	$2,20($sp)
	li	$2,1			# 0x1
	sw	$2,24($sp)
	move	$4,$0
	move	$5,$3
	li	$6,112			# 0x70
	li	$7,2			# 0x2
	lui	$2,%hi(phy_I2C_write)
	addiu	$2,$2,%lo(phy_I2C_write)
	jalr	$2
	nop

	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,116($2)
	move	$3,$2
	li	$2,345			# 0x159
	sw	$2,16($sp)
	addiu	$2,$sp,36
	sw	$2,20($sp)
	li	$2,1			# 0x1
	sw	$2,24($sp)
	move	$4,$0
	move	$5,$3
	li	$6,112			# 0x70
	li	$7,2			# 0x2
	lui	$2,%hi(phy_I2C_read)
	addiu	$2,$2,%lo(phy_I2C_read)
	jalr	$2
	nop

	lbu	$2,36($sp)
	ori	$2,$2,0x10
	andi	$2,$2,0x00ff
	sb	$2,36($sp)
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,116($2)
	move	$3,$2
	li	$2,345			# 0x159
	sw	$2,16($sp)
	addiu	$2,$sp,36
	sw	$2,20($sp)
	li	$2,1			# 0x1
	sw	$2,24($sp)
	move	$4,$0
	move	$5,$3
	li	$6,112			# 0x70
	li	$7,2			# 0x2
	lui	$2,%hi(phy_I2C_write)
	addiu	$2,$2,%lo(phy_I2C_write)
	jalr	$2
	nop

	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,116($2)
	move	$3,$2
	li	$2,340			# 0x154
	sw	$2,16($sp)
	addiu	$2,$sp,36
	sw	$2,20($sp)
	li	$2,2			# 0x2
	sw	$2,24($sp)
	move	$4,$0
	move	$5,$3
	li	$6,112			# 0x70
	li	$7,2			# 0x2
	lui	$2,%hi(phy_I2C_read)
	addiu	$2,$2,%lo(phy_I2C_read)
	jalr	$2
	nop

	lbu	$2,36($sp)
	move	$3,$2
	lbu	$2,37($sp)
	sll	$2,$2,8
	or	$3,$3,$2
	lui	$2,%hi(global_vcc_code)
	sw	$3,%lo(global_vcc_code)($2)
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,116($2)
	move	$3,$2
	li	$2,20			# 0x14
	sw	$2,16($sp)
	addiu	$2,$sp,36
	sw	$2,20($sp)
	li	$2,1			# 0x1
	sw	$2,24($sp)
	move	$4,$0
	move	$5,$3
	li	$6,112			# 0x70
	li	$7,2			# 0x2
	lui	$2,%hi(phy_I2C_read)
	addiu	$2,$2,%lo(phy_I2C_read)
	jalr	$2
	nop

	lbu	$3,36($sp)
	li	$2,-17			# 0xffffffffffffffef
	and	$2,$3,$2
	andi	$2,$2,0x00ff
	sb	$2,36($sp)
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,116($2)
	move	$3,$2
	li	$2,20			# 0x14
	sw	$2,16($sp)
	addiu	$2,$sp,36
	sw	$2,20($sp)
	li	$2,1			# 0x1
	sw	$2,24($sp)
	move	$4,$0
	move	$5,$3
	li	$6,112			# 0x70
	li	$7,2			# 0x2
	lui	$2,%hi(phy_I2C_write)
	addiu	$2,$2,%lo(phy_I2C_write)
	jalr	$2
	nop

	lw	$2,32($sp)
	andi	$2,$2,0x00ff
	sb	$2,36($sp)
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,116($2)
	move	$3,$2
	li	$2,36			# 0x24
	sw	$2,16($sp)
	addiu	$2,$sp,36
	sw	$2,20($sp)
	li	$2,1			# 0x1
	sw	$2,24($sp)
	move	$4,$0
	move	$5,$3
	li	$6,112			# 0x70
	li	$7,2			# 0x2
	lui	$2,%hi(phy_I2C_write)
	addiu	$2,$2,%lo(phy_I2C_write)
	jalr	$2
	nop

	lui	$2,%hi(i2c_protect)
	sw	$0,%lo(i2c_protect)($2)
	lui	$2,%hi(global_vcc_code)
	lw	$2,%lo(global_vcc_code)($2)
$L230:
	lw	$31,44($sp)
	addiu	$sp,$sp,48
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	mt7570_ADC_voltage_get
	.size	mt7570_ADC_voltage_get, .-mt7570_ADC_voltage_get
	.rdata
	.align	2
$LC96:
	.ascii	"Initial bias/mod current loaded from FLASH\012\000"
	.globl	__fixdfsi
	.align	2
$LC99:
	.ascii	"Initial bias/mod current loaded from LUT\012\000"
	.section	.text.mt7570_load_init_current,"ax",@progbits
	.align	2
	.globl	mt7570_load_init_current
	.set	nomips16
	.set	nomicromips
	.ent	mt7570_load_init_current
	.type	mt7570_load_init_current, @function
mt7570_load_init_current:
	.frame	$sp,56,$31		# vars= 16, regs= 1/0, args= 32, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-56
	sw	$31,52($sp)
	sw	$0,36($sp)
	sw	$0,32($sp)
	move	$4,$0
	lui	$2,%hi(get_flash_register)
	addiu	$2,$2,%lo(get_flash_register)
	jalr	$2
	nop

	move	$3,$2
	li	$2,-1			# 0xffffffffffffffff
	beq	$3,$2,$L232
	nop

	li	$4,4			# 0x4
	lui	$2,%hi(get_flash_register)
	addiu	$2,$2,%lo(get_flash_register)
	jalr	$2
	nop

	move	$3,$2
	li	$2,-1			# 0xffffffffffffffff
	beq	$3,$2,$L232
	nop

	move	$4,$0
	lui	$2,%hi(get_flash_register)
	addiu	$2,$2,%lo(get_flash_register)
	jalr	$2
	nop

	sw	$2,36($sp)
	lw	$2,36($sp)
	andi	$2,$2,0xfff
	sw	$2,36($sp)
	lw	$2,36($sp)
	andi	$2,$2,0x00ff
	sb	$2,40($sp)
	lw	$2,36($sp)
	srl	$2,$2,8
	andi	$2,$2,0x00ff
	sb	$2,41($sp)
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,116($2)
	move	$3,$2
	li	$2,312			# 0x138
	sw	$2,16($sp)
	addiu	$2,$sp,40
	sw	$2,20($sp)
	li	$2,2			# 0x2
	sw	$2,24($sp)
	move	$4,$0
	move	$5,$3
	li	$6,112			# 0x70
	li	$7,2			# 0x2
	lui	$2,%hi(phy_I2C_write)
	addiu	$2,$2,%lo(phy_I2C_write)
	jalr	$2
	nop

	li	$4,4			# 0x4
	lui	$2,%hi(get_flash_register)
	addiu	$2,$2,%lo(get_flash_register)
	jalr	$2
	nop

	sw	$2,36($sp)
	lw	$2,36($sp)
	andi	$2,$2,0xfff
	sw	$2,36($sp)
	lw	$2,36($sp)
	andi	$2,$2,0x00ff
	sb	$2,40($sp)
	lw	$2,36($sp)
	srl	$2,$2,8
	andi	$2,$2,0x00ff
	sb	$2,41($sp)
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,116($2)
	move	$3,$2
	li	$2,328			# 0x148
	sw	$2,16($sp)
	addiu	$2,$sp,40
	sw	$2,20($sp)
	li	$2,2			# 0x2
	sw	$2,24($sp)
	move	$4,$0
	move	$5,$3
	li	$6,112			# 0x70
	li	$7,2			# 0x2
	lui	$2,%hi(phy_I2C_write)
	addiu	$2,$2,%lo(phy_I2C_write)
	jalr	$2
	nop

	lui	$2,%hi($LC96)
	addiu	$4,$2,%lo($LC96)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	j	$L231
	nop

$L232:
	lui	$2,%hi(mt7570_temperature_get)
	addiu	$2,$2,%lo(mt7570_temperature_get)
	jalr	$2
	nop

	lui	$2,%hi(BOSA_temperature)
	lw	$4,%lo(BOSA_temperature)($2)
	lui	$3,%hi($LC97)
	lui	$2,%hi(__addsf3)
	addiu	$2,$2,%lo(__addsf3)
	lw	$5,%lo($LC97)($3)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(__extendsfdf2)
	addiu	$2,$2,%lo(__extendsfdf2)
	move	$4,$3
	jalr	$2
	nop

	move	$5,$3
	move	$4,$2
	lui	$3,%hi($LC98)
	lui	$2,%hi(__divdf3)
	addiu	$2,$2,%lo(__divdf3)
	lw	$7,%lo($LC98+4)($3)
	lw	$6,%lo($LC98)($3)
	jalr	$2
	nop

	move	$5,$3
	move	$4,$2
	lui	$2,%hi(__fixdfsi)
	addiu	$2,$2,%lo(__fixdfsi)
	jalr	$2
	nop

	sw	$2,32($sp)
	lw	$2,32($sp)
	bgez	$2,$L234
	nop

	sw	$0,32($sp)
$L234:
	lui	$2,%hi(LUT_Ibias_Imod)
	lw	$3,32($sp)
	sll	$3,$3,3
	addiu	$2,$2,%lo(LUT_Ibias_Imod)
	addu	$2,$3,$2
	lw	$2,0($2)
	sw	$2,36($sp)
	lw	$2,36($sp)
	andi	$2,$2,0x00ff
	sb	$2,40($sp)
	lw	$2,36($sp)
	srl	$2,$2,8
	andi	$2,$2,0x00ff
	sb	$2,41($sp)
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,116($2)
	move	$3,$2
	li	$2,312			# 0x138
	sw	$2,16($sp)
	addiu	$2,$sp,40
	sw	$2,20($sp)
	li	$2,2			# 0x2
	sw	$2,24($sp)
	move	$4,$0
	move	$5,$3
	li	$6,112			# 0x70
	li	$7,2			# 0x2
	lui	$2,%hi(phy_I2C_write)
	addiu	$2,$2,%lo(phy_I2C_write)
	jalr	$2
	nop

	lui	$2,%hi(LUT_Ibias_Imod)
	lw	$3,32($sp)
	sll	$3,$3,3
	addiu	$2,$2,%lo(LUT_Ibias_Imod)
	addu	$2,$3,$2
	lw	$2,4($2)
	sw	$2,36($sp)
	lw	$2,36($sp)
	andi	$2,$2,0x00ff
	sb	$2,40($sp)
	lw	$2,36($sp)
	srl	$2,$2,8
	andi	$2,$2,0x00ff
	sb	$2,41($sp)
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,116($2)
	move	$3,$2
	li	$2,328			# 0x148
	sw	$2,16($sp)
	addiu	$2,$sp,40
	sw	$2,20($sp)
	li	$2,2			# 0x2
	sw	$2,24($sp)
	move	$4,$0
	move	$5,$3
	li	$6,112			# 0x70
	li	$7,2			# 0x2
	lui	$2,%hi(phy_I2C_write)
	addiu	$2,$2,%lo(phy_I2C_write)
	jalr	$2
	nop

	lui	$2,%hi($LC99)
	addiu	$4,$2,%lo($LC99)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L231:
	lw	$31,52($sp)
	addiu	$sp,$sp,56
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	mt7570_load_init_current
	.size	mt7570_load_init_current, .-mt7570_load_init_current
	.section	.text.mt7570_temperature_get,"ax",@progbits
	.align	2
	.globl	mt7570_temperature_get
	.set	nomips16
	.set	nomicromips
	.ent	mt7570_temperature_get
	.type	mt7570_temperature_get, @function
mt7570_temperature_get:
	.frame	$sp,48,$31		# vars= 24, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-48
	sw	$31,44($sp)
	sw	$0,32($sp)
	lui	$2,%hi($LC100)
	lw	$2,%lo($LC100)($2)
	sw	$2,16($sp)
	lui	$2,%hi($LC16)
	lw	$2,%lo($LC16)($2)
	sw	$2,20($sp)
	lui	$2,%hi($LC101)
	lw	$2,%lo($LC101)($2)
	sw	$2,24($sp)
	lui	$2,%hi($LC102)
	lw	$2,%lo($LC102)($2)
	sw	$2,28($sp)
	li	$4,128			# 0x80
	lui	$2,%hi(get_flash_register)
	addiu	$2,$2,%lo(get_flash_register)
	jalr	$2
	nop

	move	$3,$2
	li	$2,-65536			# 0xffffffffffff0000
	and	$3,$3,$2
	li	$2,-65536			# 0xffffffffffff0000
	beq	$3,$2,$L236
	nop

	li	$4,128			# 0x80
	lui	$2,%hi(get_flash_register)
	addiu	$2,$2,%lo(get_flash_register)
	jalr	$2
	nop

	sw	$2,32($sp)
	lw	$2,32($sp)
	srl	$3,$2,16
	lui	$2,%hi(__floatunsisf)
	addiu	$2,$2,%lo(__floatunsisf)
	move	$4,$3
	jalr	$2
	nop

	move	$4,$2
	lui	$3,%hi($LC16)
	lui	$2,%hi(__divsf3)
	addiu	$2,$2,%lo(__divsf3)
	lw	$5,%lo($LC16)($3)
	jalr	$2
	nop

	sw	$2,24($sp)
$L236:
	li	$4,128			# 0x80
	lui	$2,%hi(get_flash_register)
	addiu	$2,$2,%lo(get_flash_register)
	jalr	$2
	nop

	andi	$3,$2,0xffff
	li	$2,65535			# 0xffff
	beq	$3,$2,$L237
	nop

	li	$4,128			# 0x80
	lui	$2,%hi(get_flash_register)
	addiu	$2,$2,%lo(get_flash_register)
	jalr	$2
	nop

	sw	$2,32($sp)
	lw	$2,32($sp)
	andi	$3,$2,0xffff
	lui	$2,%hi(__floatunsisf)
	addiu	$2,$2,%lo(__floatunsisf)
	move	$4,$3
	jalr	$2
	nop

	move	$4,$2
	lui	$3,%hi($LC16)
	lui	$2,%hi(__divsf3)
	addiu	$2,$2,%lo(__divsf3)
	lw	$5,%lo($LC16)($3)
	jalr	$2
	nop

	sw	$2,28($sp)
$L237:
	lui	$2,%hi(mt7570_ADC_temperature_get)
	addiu	$2,$2,%lo(mt7570_ADC_temperature_get)
	jalr	$2
	nop

	lui	$2,%hi(global_temperature_code)
	lw	$3,%lo(global_temperature_code)($2)
	lui	$2,%hi(__floatsisf)
	addiu	$2,$2,%lo(__floatsisf)
	move	$4,$3
	jalr	$2
	nop

	move	$4,$2
	lui	$2,%hi(ADC_slope)
	lw	$3,%lo(ADC_slope)($2)
	lui	$2,%hi(__mulsf3)
	addiu	$2,$2,%lo(__mulsf3)
	move	$5,$3
	jalr	$2
	nop

	move	$4,$2
	lui	$2,%hi(ADC_offset)
	lw	$3,%lo(ADC_offset)($2)
	lui	$2,%hi(__addsf3)
	addiu	$2,$2,%lo(__addsf3)
	move	$5,$3
	jalr	$2
	nop

	sw	$2,36($sp)
	lui	$2,%hi(__mulsf3)
	addiu	$2,$2,%lo(__mulsf3)
	lw	$4,24($sp)
	lw	$5,36($sp)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(__subsf3)
	addiu	$2,$2,%lo(__subsf3)
	lw	$4,28($sp)
	move	$5,$3
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(IC_temperature)
	sw	$3,%lo(IC_temperature)($2)
	li	$4,132			# 0x84
	lui	$2,%hi(get_flash_register)
	addiu	$2,$2,%lo(get_flash_register)
	jalr	$2
	nop

	sw	$2,32($sp)
	lw	$2,32($sp)
	andi	$3,$2,0xffff
	li	$2,65535			# 0xffff
	beq	$3,$2,$L238
	nop

	lw	$2,32($sp)
	andi	$3,$2,0xffff
	lui	$2,%hi(__floatunsisf)
	addiu	$2,$2,%lo(__floatunsisf)
	move	$4,$3
	jalr	$2
	nop

	sw	$2,16($sp)
$L238:
	lw	$2,32($sp)
	srl	$3,$2,16
	li	$2,65535			# 0xffff
	beq	$3,$2,$L239
	nop

	lw	$2,32($sp)
	srl	$3,$2,16
	lui	$2,%hi(__floatunsisf)
	addiu	$2,$2,%lo(__floatunsisf)
	move	$4,$3
	jalr	$2
	nop

	move	$4,$2
	lui	$3,%hi($LC16)
	lui	$2,%hi(__divsf3)
	addiu	$2,$2,%lo(__divsf3)
	lw	$5,%lo($LC16)($3)
	jalr	$2
	nop

	sw	$2,20($sp)
$L239:
	lui	$2,%hi(IC_temperature)
	lw	$3,%lo(IC_temperature)($2)
	lui	$2,%hi(__subsf3)
	addiu	$2,$2,%lo(__subsf3)
	move	$4,$3
	lw	$5,16($sp)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(BOSA_temperature)
	sw	$3,%lo(BOSA_temperature)($2)
	lui	$2,%hi(IC_temperature)
	lw	$3,%lo(IC_temperature)($2)
	lui	$2,%hi(__subsf3)
	addiu	$2,$2,%lo(__subsf3)
	move	$4,$3
	lw	$5,20($sp)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(Env_temperature)
	sw	$3,%lo(Env_temperature)($2)
	lw	$31,44($sp)
	addiu	$sp,$sp,48
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	mt7570_temperature_get
	.size	mt7570_temperature_get, .-mt7570_temperature_get
	.rdata
	.align	2
$LC103:
	.ascii	"MPDL/MPDH loaded\012\000"
	.section	.text.mt7570_load_MPDL_MPDH,"ax",@progbits
	.align	2
	.globl	mt7570_load_MPDL_MPDH
	.set	nomips16
	.set	nomicromips
	.ent	mt7570_load_MPDL_MPDH
	.type	mt7570_load_MPDL_MPDH, @function
mt7570_load_MPDL_MPDH:
	.frame	$sp,48,$31		# vars= 8, regs= 2/0, args= 32, gp= 0
	.mask	0x80010000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-48
	sw	$31,44($sp)
	sw	$16,40($sp)
	sw	$0,32($sp)
	li	$4,12			# 0xc
	lui	$2,%hi(get_flash_register)
	addiu	$2,$2,%lo(get_flash_register)
	jalr	$2
	nop

	move	$3,$2
	li	$2,-1			# 0xffffffffffffffff
	beq	$3,$2,$L240
	nop

	li	$4,8			# 0x8
	lui	$2,%hi(get_flash_register)
	addiu	$2,$2,%lo(get_flash_register)
	jalr	$2
	nop

	move	$3,$2
	li	$2,-1			# 0xffffffffffffffff
	beq	$3,$2,$L240
	nop

	li	$4,12			# 0xc
	lui	$2,%hi(get_flash_register)
	addiu	$2,$2,%lo(get_flash_register)
	jalr	$2
	nop

	andi	$16,$2,0x3ff
	li	$4,8			# 0x8
	lui	$2,%hi(get_flash_register)
	addiu	$2,$2,%lo(get_flash_register)
	jalr	$2
	nop

	andi	$2,$2,0x3ff
	sll	$2,$2,16
	or	$2,$16,$2
	sw	$2,32($sp)
	lw	$2,32($sp)
	andi	$2,$2,0x00ff
	sb	$2,36($sp)
	lw	$2,32($sp)
	srl	$2,$2,8
	andi	$2,$2,0x00ff
	sb	$2,37($sp)
	lw	$2,32($sp)
	srl	$2,$2,16
	andi	$2,$2,0x00ff
	sb	$2,38($sp)
	lw	$2,32($sp)
	srl	$2,$2,24
	andi	$2,$2,0x00ff
	sb	$2,39($sp)
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,116($2)
	move	$3,$2
	li	$2,4			# 0x4
	sw	$2,16($sp)
	addiu	$2,$sp,36
	sw	$2,20($sp)
	li	$2,4			# 0x4
	sw	$2,24($sp)
	move	$4,$0
	move	$5,$3
	li	$6,112			# 0x70
	li	$7,2			# 0x2
	lui	$2,%hi(phy_I2C_write)
	addiu	$2,$2,%lo(phy_I2C_write)
	jalr	$2
	nop

	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,116($2)
	move	$3,$2
	li	$2,316			# 0x13c
	sw	$2,16($sp)
	addiu	$2,$sp,36
	sw	$2,20($sp)
	li	$2,4			# 0x4
	sw	$2,24($sp)
	move	$4,$0
	move	$5,$3
	li	$6,112			# 0x70
	li	$7,2			# 0x2
	lui	$2,%hi(phy_I2C_read)
	addiu	$2,$2,%lo(phy_I2C_read)
	jalr	$2
	nop

	lbu	$2,36($sp)
	ori	$2,$2,0x1
	andi	$2,$2,0x00ff
	sb	$2,36($sp)
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,116($2)
	move	$3,$2
	li	$2,316			# 0x13c
	sw	$2,16($sp)
	addiu	$2,$sp,36
	sw	$2,20($sp)
	li	$2,4			# 0x4
	sw	$2,24($sp)
	move	$4,$0
	move	$5,$3
	li	$6,112			# 0x70
	li	$7,2			# 0x2
	lui	$2,%hi(phy_I2C_write)
	addiu	$2,$2,%lo(phy_I2C_write)
	jalr	$2
	nop

	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,116($2)
	move	$3,$2
	li	$2,332			# 0x14c
	sw	$2,16($sp)
	addiu	$2,$sp,36
	sw	$2,20($sp)
	li	$2,4			# 0x4
	sw	$2,24($sp)
	move	$4,$0
	move	$5,$3
	li	$6,112			# 0x70
	li	$7,2			# 0x2
	lui	$2,%hi(phy_I2C_read)
	addiu	$2,$2,%lo(phy_I2C_read)
	jalr	$2
	nop

	lbu	$2,36($sp)
	ori	$2,$2,0x1
	andi	$2,$2,0x00ff
	sb	$2,36($sp)
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,116($2)
	move	$3,$2
	li	$2,332			# 0x14c
	sw	$2,16($sp)
	addiu	$2,$sp,36
	sw	$2,20($sp)
	li	$2,4			# 0x4
	sw	$2,24($sp)
	move	$4,$0
	move	$5,$3
	li	$6,112			# 0x70
	li	$7,2			# 0x2
	lui	$2,%hi(phy_I2C_write)
	addiu	$2,$2,%lo(phy_I2C_write)
	jalr	$2
	nop

	lui	$2,%hi($LC103)
	addiu	$4,$2,%lo($LC103)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L240:
	lw	$31,44($sp)
	lw	$16,40($sp)
	addiu	$sp,$sp,48
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	mt7570_load_MPDL_MPDH
	.size	mt7570_load_MPDL_MPDH, .-mt7570_load_MPDL_MPDH
	.rdata
	.align	2
$LC105:
	.ascii	"[%lu]0ms[%s:%d]\012  IC temperature = %d.%dC\012\000"
	.section	.text.mt7570_temperature_get_8472,"ax",@progbits
	.align	2
	.globl	mt7570_temperature_get_8472
	.set	nomips16
	.set	nomicromips
	.ent	mt7570_temperature_get_8472
	.type	mt7570_temperature_get_8472, @function
mt7570_temperature_get_8472:
	.frame	$sp,48,$31		# vars= 16, regs= 2/0, args= 24, gp= 0
	.mask	0x80010000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-48
	sw	$31,44($sp)
	sw	$16,40($sp)
	sw	$0,24($sp)
	sw	$0,28($sp)
	lui	$2,%hi(mt7570_temperature_get)
	addiu	$2,$2,%lo(mt7570_temperature_get)
	jalr	$2
	nop

	lui	$2,%hi(IC_temperature)
	lw	$3,%lo(IC_temperature)($2)
	lui	$2,%hi(__gtsf2)
	addiu	$2,$2,%lo(__gtsf2)
	move	$4,$3
	move	$5,$0
	jalr	$2
	nop

	blez	$2,$L250
	nop

	lui	$2,%hi(IC_temperature)
	lw	$4,%lo(IC_temperature)($2)
	lui	$3,%hi($LC104)
	lui	$2,%hi(__mulsf3)
	addiu	$2,$2,%lo(__mulsf3)
	lw	$5,%lo($LC104)($3)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(__fixunssfsi)
	addiu	$2,$2,%lo(__fixunssfsi)
	move	$4,$3
	jalr	$2
	nop

	andi	$3,$2,0xffff
	lui	$2,%hi(global_IC_temperature_8472)
	sh	$3,%lo(global_IC_temperature_8472)($2)
	j	$L245
	nop

$L250:
	lui	$2,%hi(IC_temperature)
	lw	$3,%lo(IC_temperature)($2)
	li	$2,-2147483648			# 0xffffffff80000000
	xor	$4,$3,$2
	lui	$3,%hi($LC104)
	lui	$2,%hi(__mulsf3)
	addiu	$2,$2,%lo(__mulsf3)
	lw	$5,%lo($LC104)($3)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(__fixunssfsi)
	addiu	$2,$2,%lo(__fixunssfsi)
	move	$4,$3
	jalr	$2
	nop

	andi	$2,$2,0xffff
	subu	$2,$0,$2
	andi	$3,$2,0xffff
	lui	$2,%hi(global_IC_temperature_8472)
	sh	$3,%lo(global_IC_temperature_8472)($2)
$L245:
	lui	$2,%hi(IC_temperature)
	lw	$3,%lo(IC_temperature)($2)
	lui	$2,%hi(__fixsfsi)
	addiu	$2,$2,%lo(__fixsfsi)
	move	$4,$3
	jalr	$2
	nop

	sw	$2,24($sp)
	lui	$2,%hi(IC_temperature)
	lw	$4,%lo(IC_temperature)($2)
	lui	$3,%hi($LC16)
	lui	$2,%hi(__mulsf3)
	addiu	$2,$2,%lo(__mulsf3)
	lw	$5,%lo($LC16)($3)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(__fixsfsi)
	addiu	$2,$2,%lo(__fixsfsi)
	move	$4,$3
	jalr	$2
	nop

	move	$3,$2
	li	$2,1717960704			# 0x66660000
	ori	$2,$2,0x6667
	mult	$3,$2
	mfhi	$2
	sra	$4,$2,2
	sra	$2,$3,31
	subu	$2,$4,$2
	sll	$2,$2,1
	sll	$4,$2,2
	addu	$2,$2,$4
	subu	$2,$3,$2
	sw	$2,32($sp)
	lw	$2,32($sp)
	bgez	$2,$L246
	nop

	subu	$2,$0,$2
$L246:
	sw	$2,28($sp)
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,136($2)
	andi	$2,$2,0x10
	beq	$2,$0,$L247
	nop

	lui	$2,%hi(jiffies)
	lw	$16,%lo(jiffies)($2)
	lui	$2,%hi($LC37)
	addiu	$4,$2,%lo($LC37)
	li	$5,47			# 0x2f
	lui	$2,%hi(strrchr)
	addiu	$2,$2,%lo(strrchr)
	jalr	$2
	nop

	addiu	$3,$2,1
	lw	$2,24($sp)
	sw	$2,16($sp)
	lw	$2,28($sp)
	sw	$2,20($sp)
	lui	$2,%hi($LC105)
	addiu	$4,$2,%lo($LC105)
	move	$5,$16
	move	$6,$3
	li	$7,2301			# 0x8fd
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L247:
	lui	$2,%hi(global_IC_temperature_8472)
	lhu	$2,%lo(global_IC_temperature_8472)($2)
	lw	$31,44($sp)
	lw	$16,40($sp)
	addiu	$sp,$sp,48
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	mt7570_temperature_get_8472
	.size	mt7570_temperature_get_8472, .-mt7570_temperature_get_8472
	.section	.text.mt7570_supply_voltage_get_8472,"ax",@progbits
	.align	2
	.globl	mt7570_supply_voltage_get_8472
	.set	nomips16
	.set	nomicromips
	.ent	mt7570_supply_voltage_get_8472
	.type	mt7570_supply_voltage_get_8472, @function
mt7570_supply_voltage_get_8472:
	.frame	$sp,24,$31		# vars= 0, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-24
	sw	$31,20($sp)
	lui	$2,%hi(mt7570_ADC_voltage_get)
	addiu	$2,$2,%lo(mt7570_ADC_voltage_get)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(__floatsisf)
	addiu	$2,$2,%lo(__floatsisf)
	move	$4,$3
	jalr	$2
	nop

	move	$4,$2
	lui	$2,%hi(ADC_slope)
	lw	$3,%lo(ADC_slope)($2)
	lui	$2,%hi(__mulsf3)
	addiu	$2,$2,%lo(__mulsf3)
	move	$5,$3
	jalr	$2
	nop

	move	$4,$2
	lui	$2,%hi(ADC_offset)
	lw	$3,%lo(ADC_offset)($2)
	lui	$2,%hi(__addsf3)
	addiu	$2,$2,%lo(__addsf3)
	move	$5,$3
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(__addsf3)
	addiu	$2,$2,%lo(__addsf3)
	move	$4,$3
	move	$5,$3
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(__extendsfdf2)
	addiu	$2,$2,%lo(__extendsfdf2)
	move	$4,$3
	jalr	$2
	nop

	move	$5,$3
	move	$4,$2
	lui	$3,%hi($LC106)
	lui	$2,%hi(__divdf3)
	addiu	$2,$2,%lo(__divdf3)
	lw	$7,%lo($LC106+4)($3)
	lw	$6,%lo($LC106)($3)
	jalr	$2
	nop

	move	$5,$3
	move	$4,$2
	lui	$2,%hi(__fixunsdfsi)
	addiu	$2,$2,%lo(__fixunsdfsi)
	jalr	$2
	nop

	andi	$3,$2,0xffff
	lui	$2,%hi(global_supply_voltage_8472)
	sh	$3,%lo(global_supply_voltage_8472)($2)
	lui	$2,%hi(global_supply_voltage_8472)
	lhu	$2,%lo(global_supply_voltage_8472)($2)
	lw	$31,20($sp)
	addiu	$sp,$sp,24
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	mt7570_supply_voltage_get_8472
	.size	mt7570_supply_voltage_get_8472, .-mt7570_supply_voltage_get_8472
	.rdata
	.align	2
$LC110:
	.ascii	"[%lu]0ms[%s:%d]\012  Fiber isn't recovered\012\000"
	.align	2
$LC111:
	.ascii	"[%lu]0ms[%s:%d]\012  Fiber plugged\012\000"
	.section	.text.mt7570_bias_current_get_8472,"ax",@progbits
	.align	2
	.globl	mt7570_bias_current_get_8472
	.set	nomips16
	.set	nomicromips
	.ent	mt7570_bias_current_get_8472
	.type	mt7570_bias_current_get_8472, @function
mt7570_bias_current_get_8472:
	.frame	$sp,32,$31		# vars= 8, regs= 2/0, args= 16, gp= 0
	.mask	0x80010000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-32
	sw	$31,28($sp)
	sw	$16,24($sp)
	sh	$0,16($sp)
	li	$4,3			# 0x3
	lui	$2,%hi(mt7570_information_output)
	addiu	$2,$2,%lo(mt7570_information_output)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(__floatsidf)
	addiu	$2,$2,%lo(__floatsidf)
	move	$4,$3
	jalr	$2
	nop

	move	$5,$3
	move	$4,$2
	lui	$3,%hi($LC107)
	lui	$2,%hi(__muldf3)
	addiu	$2,$2,%lo(__muldf3)
	lw	$7,%lo($LC107+4)($3)
	lw	$6,%lo($LC107)($3)
	jalr	$2
	nop

	move	$5,$3
	move	$4,$2
	lui	$3,%hi($LC108)
	lui	$2,%hi(__muldf3)
	addiu	$2,$2,%lo(__muldf3)
	lw	$7,%lo($LC108+4)($3)
	lw	$6,%lo($LC108)($3)
	jalr	$2
	nop

	move	$5,$3
	move	$4,$2
	lui	$3,%hi($LC109)
	lui	$2,%hi(__divdf3)
	addiu	$2,$2,%lo(__divdf3)
	lw	$7,%lo($LC109+4)($3)
	lw	$6,%lo($LC109)($3)
	jalr	$2
	nop

	move	$5,$3
	move	$4,$2
	lui	$2,%hi(__fixunsdfsi)
	addiu	$2,$2,%lo(__fixunsdfsi)
	jalr	$2
	nop

	sh	$2,16($sp)
	lui	$2,%hi(FiberPlug)
	lw	$3,%lo(FiberPlug)($2)
	li	$2,1			# 0x1
	bne	$3,$2,$L254
	nop

	lhu	$2,16($sp)
	sltu	$2,$2,500
	beq	$2,$0,$L254
	nop

	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,136($2)
	andi	$2,$2,0x10
	beq	$2,$0,$L255
	nop

	lui	$2,%hi(jiffies)
	lw	$16,%lo(jiffies)($2)
	lui	$2,%hi($LC37)
	addiu	$4,$2,%lo($LC37)
	li	$5,47			# 0x2f
	lui	$2,%hi(strrchr)
	addiu	$2,$2,%lo(strrchr)
	jalr	$2
	nop

	addiu	$3,$2,1
	lui	$2,%hi($LC110)
	addiu	$4,$2,%lo($LC110)
	move	$5,$16
	move	$6,$3
	li	$7,2391			# 0x957
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	j	$L256
	nop

$L255:
	j	$L256
	nop

$L254:
	lui	$2,%hi(global_bias_current_8472)
	lhu	$2,%lo(global_bias_current_8472)($2)
	move	$3,$2
	lhu	$2,16($sp)
	subu	$2,$3,$2
	slt	$2,$2,501
	bne	$2,$0,$L257
	nop

	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,136($2)
	andi	$2,$2,0x10
	beq	$2,$0,$L258
	nop

	lui	$2,%hi(jiffies)
	lw	$16,%lo(jiffies)($2)
	lui	$2,%hi($LC37)
	addiu	$4,$2,%lo($LC37)
	li	$5,47			# 0x2f
	lui	$2,%hi(strrchr)
	addiu	$2,$2,%lo(strrchr)
	jalr	$2
	nop

	addiu	$3,$2,1
	lui	$2,%hi($LC111)
	addiu	$4,$2,%lo($LC111)
	move	$5,$16
	move	$6,$3
	li	$7,2397			# 0x95d
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L258:
	lui	$2,%hi(FiberPlug)
	li	$3,1			# 0x1
	sw	$3,%lo(FiberPlug)($2)
	j	$L256
	nop

$L257:
	lui	$2,%hi(FiberPlug)
	sw	$0,%lo(FiberPlug)($2)
$L256:
	lui	$2,%hi(global_bias_current_8472)
	lhu	$3,16($sp)
	sh	$3,%lo(global_bias_current_8472)($2)
	lui	$2,%hi(global_bias_current_8472)
	lhu	$2,%lo(global_bias_current_8472)($2)
	lw	$31,28($sp)
	lw	$16,24($sp)
	addiu	$sp,$sp,32
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	mt7570_bias_current_get_8472
	.size	mt7570_bias_current_get_8472, .-mt7570_bias_current_get_8472
	.rdata
	.align	2
$LC112:
	.ascii	"RSSI_Vref = 0x%x\012\000"
	.align	2
$LC113:
	.ascii	"RSSI_V = 0x%x\012\000"
	.align	2
$LC115:
	.ascii	"RSSI calibration fail\012\000"
	.section	.text.mt7570_RSSI_calibration,"ax",@progbits
	.align	2
	.globl	mt7570_RSSI_calibration
	.set	nomips16
	.set	nomicromips
	.ent	mt7570_RSSI_calibration
	.type	mt7570_RSSI_calibration, @function
mt7570_RSSI_calibration:
	.frame	$sp,48,$31		# vars= 8, regs= 1/0, args= 32, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-48
	sw	$31,44($sp)
	sw	$0,32($sp)
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,116($2)
	move	$3,$2
	li	$2,20			# 0x14
	sw	$2,16($sp)
	addiu	$2,$sp,36
	sw	$2,20($sp)
	li	$2,2			# 0x2
	sw	$2,24($sp)
	move	$4,$0
	move	$5,$3
	li	$6,112			# 0x70
	li	$7,2			# 0x2
	lui	$2,%hi(phy_I2C_read)
	addiu	$2,$2,%lo(phy_I2C_read)
	jalr	$2
	nop

	lbu	$2,37($sp)
	ori	$2,$2,0x10
	andi	$2,$2,0x00ff
	sb	$2,37($sp)
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,116($2)
	move	$3,$2
	li	$2,20			# 0x14
	sw	$2,16($sp)
	addiu	$2,$sp,36
	sw	$2,20($sp)
	li	$2,2			# 0x2
	sw	$2,24($sp)
	move	$4,$0
	move	$5,$3
	li	$6,112			# 0x70
	li	$7,2			# 0x2
	lui	$2,%hi(phy_I2C_write)
	addiu	$2,$2,%lo(phy_I2C_write)
	jalr	$2
	nop

	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,116($2)
	move	$3,$2
	li	$2,20			# 0x14
	sw	$2,16($sp)
	addiu	$2,$sp,36
	sw	$2,20($sp)
	li	$2,2			# 0x2
	sw	$2,24($sp)
	move	$4,$0
	move	$5,$3
	li	$6,112			# 0x70
	li	$7,2			# 0x2
	lui	$2,%hi(phy_I2C_read)
	addiu	$2,$2,%lo(phy_I2C_read)
	jalr	$2
	nop

	lbu	$2,37($sp)
	ori	$2,$2,0x40
	andi	$2,$2,0x00ff
	sb	$2,37($sp)
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,116($2)
	move	$3,$2
	li	$2,20			# 0x14
	sw	$2,16($sp)
	addiu	$2,$sp,36
	sw	$2,20($sp)
	li	$2,2			# 0x2
	sw	$2,24($sp)
	move	$4,$0
	move	$5,$3
	li	$6,112			# 0x70
	li	$7,2			# 0x2
	lui	$2,%hi(phy_I2C_write)
	addiu	$2,$2,%lo(phy_I2C_write)
	jalr	$2
	nop

	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,116($2)
	move	$3,$2
	li	$2,36			# 0x24
	sw	$2,16($sp)
	addiu	$2,$sp,36
	sw	$2,20($sp)
	li	$2,1			# 0x1
	sw	$2,24($sp)
	move	$4,$0
	move	$5,$3
	li	$6,112			# 0x70
	li	$7,2			# 0x2
	lui	$2,%hi(phy_I2C_read)
	addiu	$2,$2,%lo(phy_I2C_read)
	jalr	$2
	nop

	lbu	$2,36($sp)
	sw	$2,32($sp)
	lbu	$2,36($sp)
	seb	$3,$2
	li	$2,-31			# 0xffffffffffffffe1
	and	$2,$3,$2
	seb	$2,$2
	ori	$2,$2,0x2
	seb	$2,$2
	andi	$2,$2,0x00ff
	sb	$2,36($sp)
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,116($2)
	move	$3,$2
	li	$2,36			# 0x24
	sw	$2,16($sp)
	addiu	$2,$sp,36
	sw	$2,20($sp)
	li	$2,1			# 0x1
	sw	$2,24($sp)
	move	$4,$0
	move	$5,$3
	li	$6,112			# 0x70
	li	$7,2			# 0x2
	lui	$2,%hi(phy_I2C_write)
	addiu	$2,$2,%lo(phy_I2C_write)
	jalr	$2
	nop

	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,116($2)
	move	$3,$2
	li	$2,345			# 0x159
	sw	$2,16($sp)
	addiu	$2,$sp,36
	sw	$2,20($sp)
	li	$2,1			# 0x1
	sw	$2,24($sp)
	move	$4,$0
	move	$5,$3
	li	$6,112			# 0x70
	li	$7,2			# 0x2
	lui	$2,%hi(phy_I2C_read)
	addiu	$2,$2,%lo(phy_I2C_read)
	jalr	$2
	nop

	lbu	$2,36($sp)
	ori	$2,$2,0x10
	andi	$2,$2,0x00ff
	sb	$2,36($sp)
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,116($2)
	move	$3,$2
	li	$2,345			# 0x159
	sw	$2,16($sp)
	addiu	$2,$sp,36
	sw	$2,20($sp)
	li	$2,1			# 0x1
	sw	$2,24($sp)
	move	$4,$0
	move	$5,$3
	li	$6,112			# 0x70
	li	$7,2			# 0x2
	lui	$2,%hi(phy_I2C_write)
	addiu	$2,$2,%lo(phy_I2C_write)
	jalr	$2
	nop

	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,116($2)
	move	$3,$2
	li	$2,340			# 0x154
	sw	$2,16($sp)
	addiu	$2,$sp,36
	sw	$2,20($sp)
	li	$2,2			# 0x2
	sw	$2,24($sp)
	move	$4,$0
	move	$5,$3
	li	$6,112			# 0x70
	li	$7,2			# 0x2
	lui	$2,%hi(phy_I2C_read)
	addiu	$2,$2,%lo(phy_I2C_read)
	jalr	$2
	nop

	lbu	$2,36($sp)
	move	$3,$2
	lbu	$2,37($sp)
	sll	$2,$2,8
	or	$3,$3,$2
	lui	$2,%hi(RSSI_Vref)
	sw	$3,%lo(RSSI_Vref)($2)
	lui	$2,%hi(RSSI_Vref)
	lw	$3,%lo(RSSI_Vref)($2)
	lui	$2,%hi($LC112)
	addiu	$4,$2,%lo($LC112)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,116($2)
	move	$3,$2
	li	$2,20			# 0x14
	sw	$2,16($sp)
	addiu	$2,$sp,36
	sw	$2,20($sp)
	li	$2,2			# 0x2
	sw	$2,24($sp)
	move	$4,$0
	move	$5,$3
	li	$6,112			# 0x70
	li	$7,2			# 0x2
	lui	$2,%hi(phy_I2C_read)
	addiu	$2,$2,%lo(phy_I2C_read)
	jalr	$2
	nop

	lbu	$3,37($sp)
	li	$2,-65			# 0xffffffffffffffbf
	and	$2,$3,$2
	andi	$2,$2,0x00ff
	sb	$2,37($sp)
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,116($2)
	move	$3,$2
	li	$2,20			# 0x14
	sw	$2,16($sp)
	addiu	$2,$sp,36
	sw	$2,20($sp)
	li	$2,2			# 0x2
	sw	$2,24($sp)
	move	$4,$0
	move	$5,$3
	li	$6,112			# 0x70
	li	$7,2			# 0x2
	lui	$2,%hi(phy_I2C_write)
	addiu	$2,$2,%lo(phy_I2C_write)
	jalr	$2
	nop

	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,116($2)
	move	$3,$2
	li	$2,345			# 0x159
	sw	$2,16($sp)
	addiu	$2,$sp,36
	sw	$2,20($sp)
	li	$2,1			# 0x1
	sw	$2,24($sp)
	move	$4,$0
	move	$5,$3
	li	$6,112			# 0x70
	li	$7,2			# 0x2
	lui	$2,%hi(phy_I2C_read)
	addiu	$2,$2,%lo(phy_I2C_read)
	jalr	$2
	nop

	lbu	$2,36($sp)
	ori	$2,$2,0x10
	andi	$2,$2,0x00ff
	sb	$2,36($sp)
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,116($2)
	move	$3,$2
	li	$2,345			# 0x159
	sw	$2,16($sp)
	addiu	$2,$sp,36
	sw	$2,20($sp)
	li	$2,1			# 0x1
	sw	$2,24($sp)
	move	$4,$0
	move	$5,$3
	li	$6,112			# 0x70
	li	$7,2			# 0x2
	lui	$2,%hi(phy_I2C_write)
	addiu	$2,$2,%lo(phy_I2C_write)
	jalr	$2
	nop

	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,116($2)
	move	$3,$2
	li	$2,340			# 0x154
	sw	$2,16($sp)
	addiu	$2,$sp,36
	sw	$2,20($sp)
	li	$2,2			# 0x2
	sw	$2,24($sp)
	move	$4,$0
	move	$5,$3
	li	$6,112			# 0x70
	li	$7,2			# 0x2
	lui	$2,%hi(phy_I2C_read)
	addiu	$2,$2,%lo(phy_I2C_read)
	jalr	$2
	nop

	lbu	$2,36($sp)
	move	$3,$2
	lbu	$2,37($sp)
	sll	$2,$2,8
	or	$3,$3,$2
	lui	$2,%hi(RSSI_V)
	sw	$3,%lo(RSSI_V)($2)
	lui	$2,%hi(RSSI_V)
	lw	$3,%lo(RSSI_V)($2)
	lui	$2,%hi($LC113)
	addiu	$4,$2,%lo($LC113)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,116($2)
	move	$3,$2
	li	$2,20			# 0x14
	sw	$2,16($sp)
	addiu	$2,$sp,36
	sw	$2,20($sp)
	li	$2,2			# 0x2
	sw	$2,24($sp)
	move	$4,$0
	move	$5,$3
	li	$6,112			# 0x70
	li	$7,2			# 0x2
	lui	$2,%hi(phy_I2C_read)
	addiu	$2,$2,%lo(phy_I2C_read)
	jalr	$2
	nop

	lbu	$3,37($sp)
	li	$2,-17			# 0xffffffffffffffef
	and	$2,$3,$2
	andi	$2,$2,0x00ff
	sb	$2,37($sp)
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,116($2)
	move	$3,$2
	li	$2,20			# 0x14
	sw	$2,16($sp)
	addiu	$2,$sp,36
	sw	$2,20($sp)
	li	$2,2			# 0x2
	sw	$2,24($sp)
	move	$4,$0
	move	$5,$3
	li	$6,112			# 0x70
	li	$7,2			# 0x2
	lui	$2,%hi(phy_I2C_write)
	addiu	$2,$2,%lo(phy_I2C_write)
	jalr	$2
	nop

	lw	$2,32($sp)
	andi	$2,$2,0x00ff
	sb	$2,36($sp)
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,116($2)
	move	$3,$2
	li	$2,36			# 0x24
	sw	$2,16($sp)
	addiu	$2,$sp,36
	sw	$2,20($sp)
	li	$2,1			# 0x1
	sw	$2,24($sp)
	move	$4,$0
	move	$5,$3
	li	$6,112			# 0x70
	li	$7,2			# 0x2
	lui	$2,%hi(phy_I2C_write)
	addiu	$2,$2,%lo(phy_I2C_write)
	jalr	$2
	nop

	lui	$2,%hi(RSSI_V)
	lw	$3,%lo(RSSI_V)($2)
	lui	$2,%hi(RSSI_Vref)
	lw	$2,%lo(RSSI_Vref)($2)
	slt	$2,$2,$3
	beq	$2,$0,$L261
	nop

	lui	$2,%hi(RSSI_V)
	lw	$3,%lo(RSSI_V)($2)
	lui	$2,%hi(RSSI_Vref)
	lw	$2,%lo(RSSI_Vref)($2)
	subu	$3,$3,$2
	lui	$2,%hi(__floatsisf)
	addiu	$2,$2,%lo(__floatsisf)
	move	$4,$3
	jalr	$2
	nop

	move	$4,$2
	lui	$2,%hi(ADC_slope)
	lw	$3,%lo(ADC_slope)($2)
	lui	$2,%hi(__mulsf3)
	addiu	$2,$2,%lo(__mulsf3)
	move	$5,$3
	jalr	$2
	nop

	move	$4,$2
	lui	$2,%hi(ADC_offset)
	lw	$3,%lo(ADC_offset)($2)
	lui	$2,%hi(__addsf3)
	addiu	$2,$2,%lo(__addsf3)
	move	$5,$3
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(__extendsfdf2)
	addiu	$2,$2,%lo(__extendsfdf2)
	move	$4,$3
	jalr	$2
	nop

	move	$5,$3
	move	$4,$2
	lui	$3,%hi($LC114)
	lui	$2,%hi(__divdf3)
	addiu	$2,$2,%lo(__divdf3)
	lw	$7,%lo($LC114+4)($3)
	lw	$6,%lo($LC114)($3)
	jalr	$2
	nop

	move	$5,$3
	move	$4,$2
	lui	$2,%hi(__truncdfsf2)
	addiu	$2,$2,%lo(__truncdfsf2)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(RSSI_factor)
	sw	$3,%lo(RSSI_factor)($2)
	j	$L260
	nop

$L261:
	lui	$2,%hi($LC115)
	addiu	$4,$2,%lo($LC115)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L260:
	lw	$31,44($sp)
	addiu	$sp,$sp,48
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	mt7570_RSSI_calibration
	.size	mt7570_RSSI_calibration, .-mt7570_RSSI_calibration
	.rdata
	.align	2
$LC116:
	.ascii	"[%lu]0ms[%s:%d]\012  RSSI ADC code lower than 0.5V, not "
	.ascii	"precise!\012\000"
	.align	2
$LC117:
	.ascii	"[%lu]0ms[%s:%d]\012  RSSI GAIN = %d, RSSI_voltage = %x, "
	.ascii	"\012\000"
	.section	.text.mt7570_RSSI_current,"ax",@progbits
	.align	2
	.globl	mt7570_RSSI_current
	.set	nomips16
	.set	nomicromips
	.ent	mt7570_RSSI_current
	.type	mt7570_RSSI_current, @function
mt7570_RSSI_current:
	.frame	$sp,80,$31		# vars= 40, regs= 2/0, args= 32, gp= 0
	.mask	0x80010000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-80
	sw	$31,76($sp)
	sw	$16,72($sp)
	sw	$0,56($sp)
	sw	$0,32($sp)
	sw	$0,36($sp)
	sw	$0,40($sp)
	li	$2,4			# 0x4
	sw	$2,60($sp)
	sw	$0,44($sp)
	sw	$0,48($sp)
	sw	$0,52($sp)
	lui	$2,%hi(i2c_access_protect)
	addiu	$2,$2,%lo(i2c_access_protect)
	jalr	$2
	nop

	move	$3,$2
	li	$2,-1			# 0xffffffffffffffff
	bne	$3,$2,$L264
	nop

	lui	$2,%hi($LC70)
	addiu	$4,$2,%lo($LC70)
	lui	$2,%hi(__func__.25380)
	addiu	$5,$2,%lo(__func__.25380)
	li	$6,2502			# 0x9c6
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi(global_RSSI_current)
	lw	$2,%lo(global_RSSI_current)($2)
	j	$L283
	nop

$L264:
	lui	$2,%hi(i2c_protect)
	li	$3,1			# 0x1
	sw	$3,%lo(i2c_protect)($2)
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,116($2)
	move	$3,$2
	li	$2,36			# 0x24
	sw	$2,16($sp)
	addiu	$2,$sp,64
	sw	$2,20($sp)
	li	$2,1			# 0x1
	sw	$2,24($sp)
	move	$4,$0
	move	$5,$3
	li	$6,112			# 0x70
	li	$7,2			# 0x2
	lui	$2,%hi(phy_I2C_read)
	addiu	$2,$2,%lo(phy_I2C_read)
	jalr	$2
	nop

	lbu	$2,64($sp)
	sw	$2,56($sp)
	lbu	$2,64($sp)
	seb	$3,$2
	li	$2,-31			# 0xffffffffffffffe1
	and	$2,$3,$2
	seb	$2,$2
	ori	$2,$2,0x2
	seb	$2,$2
	andi	$2,$2,0x00ff
	sb	$2,64($sp)
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,116($2)
	move	$3,$2
	li	$2,36			# 0x24
	sw	$2,16($sp)
	addiu	$2,$sp,64
	sw	$2,20($sp)
	li	$2,1			# 0x1
	sw	$2,24($sp)
	move	$4,$0
	move	$5,$3
	li	$6,112			# 0x70
	li	$7,2			# 0x2
	lui	$2,%hi(phy_I2C_write)
	addiu	$2,$2,%lo(phy_I2C_write)
	jalr	$2
	nop

	sw	$0,52($sp)
	j	$L266
	nop

$L271:
	li	$3,5			# 0x5
	lw	$2,52($sp)
	subu	$2,$3,$2
	sw	$2,36($sp)
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,116($2)
	move	$3,$2
	li	$2,20			# 0x14
	sw	$2,16($sp)
	addiu	$2,$sp,64
	sw	$2,20($sp)
	li	$2,4			# 0x4
	sw	$2,24($sp)
	move	$4,$0
	move	$5,$3
	li	$6,112			# 0x70
	li	$7,2			# 0x2
	lui	$2,%hi(phy_I2C_read)
	addiu	$2,$2,%lo(phy_I2C_read)
	jalr	$2
	nop

	lbu	$3,66($sp)
	li	$2,-8			# 0xfffffffffffffff8
	and	$2,$3,$2
	andi	$3,$2,0x00ff
	lw	$2,36($sp)
	andi	$2,$2,0x00ff
	or	$2,$3,$2
	andi	$2,$2,0x00ff
	sb	$2,66($sp)
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,116($2)
	move	$3,$2
	li	$2,20			# 0x14
	sw	$2,16($sp)
	addiu	$2,$sp,64
	sw	$2,20($sp)
	li	$2,4			# 0x4
	sw	$2,24($sp)
	move	$4,$0
	move	$5,$3
	li	$6,112			# 0x70
	li	$7,2			# 0x2
	lui	$2,%hi(phy_I2C_write)
	addiu	$2,$2,%lo(phy_I2C_write)
	jalr	$2
	nop

	sw	$0,44($sp)
	sw	$0,48($sp)
	j	$L267
	nop

$L268:
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,116($2)
	move	$3,$2
	li	$2,345			# 0x159
	sw	$2,16($sp)
	addiu	$2,$sp,64
	sw	$2,20($sp)
	li	$2,1			# 0x1
	sw	$2,24($sp)
	move	$4,$0
	move	$5,$3
	li	$6,112			# 0x70
	li	$7,2			# 0x2
	lui	$2,%hi(phy_I2C_read)
	addiu	$2,$2,%lo(phy_I2C_read)
	jalr	$2
	nop

	lbu	$2,64($sp)
	ori	$2,$2,0x10
	andi	$2,$2,0x00ff
	sb	$2,64($sp)
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,116($2)
	move	$3,$2
	li	$2,345			# 0x159
	sw	$2,16($sp)
	addiu	$2,$sp,64
	sw	$2,20($sp)
	li	$2,1			# 0x1
	sw	$2,24($sp)
	move	$4,$0
	move	$5,$3
	li	$6,112			# 0x70
	li	$7,2			# 0x2
	lui	$2,%hi(phy_I2C_write)
	addiu	$2,$2,%lo(phy_I2C_write)
	jalr	$2
	nop

	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,116($2)
	move	$3,$2
	li	$2,340			# 0x154
	sw	$2,16($sp)
	addiu	$2,$sp,64
	sw	$2,20($sp)
	li	$2,2			# 0x2
	sw	$2,24($sp)
	move	$4,$0
	move	$5,$3
	li	$6,112			# 0x70
	li	$7,2			# 0x2
	lui	$2,%hi(phy_I2C_read)
	addiu	$2,$2,%lo(phy_I2C_read)
	jalr	$2
	nop

	lbu	$2,64($sp)
	move	$3,$2
	lbu	$2,65($sp)
	sll	$2,$2,8
	or	$2,$3,$2
	move	$3,$2
	lw	$2,44($sp)
	addu	$2,$2,$3
	sw	$2,44($sp)
	lw	$2,48($sp)
	addiu	$2,$2,1
	sw	$2,48($sp)
$L267:
	lw	$3,48($sp)
	lw	$2,60($sp)
	sltu	$2,$3,$2
	bne	$2,$0,$L268
	nop

	lw	$2,44($sp)
	srl	$2,$2,1
	addiu	$2,$2,1
	srl	$2,$2,1
	sw	$2,32($sp)
	lui	$2,%hi(RSSI_Vref)
	lw	$2,%lo(RSSI_Vref)($2)
	addiu	$2,$2,-50
	move	$3,$2
	lw	$2,32($sp)
	sltu	$2,$2,$3
	beq	$2,$0,$L269
	nop

	j	$L270
	nop

$L269:
	lw	$2,52($sp)
	addiu	$2,$2,1
	sw	$2,52($sp)
$L266:
	lw	$2,52($sp)
	slt	$2,$2,6
	bne	$2,$0,$L271
	nop

$L270:
	lw	$2,32($sp)
	sltu	$2,$2,183
	beq	$2,$0,$L272
	nop

	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,136($2)
	andi	$2,$2,0x10
	beq	$2,$0,$L272
	nop

	lui	$2,%hi(jiffies)
	lw	$16,%lo(jiffies)($2)
	lui	$2,%hi($LC37)
	addiu	$4,$2,%lo($LC37)
	li	$5,47			# 0x2f
	lui	$2,%hi(strrchr)
	addiu	$2,$2,%lo(strrchr)
	jalr	$2
	nop

	addiu	$3,$2,1
	lui	$2,%hi($LC116)
	addiu	$4,$2,%lo($LC116)
	move	$5,$16
	move	$6,$3
	li	$7,2538			# 0x9ea
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L272:
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,136($2)
	andi	$2,$2,0x10
	beq	$2,$0,$L273
	nop

	lui	$2,%hi(jiffies)
	lw	$16,%lo(jiffies)($2)
	lui	$2,%hi($LC37)
	addiu	$4,$2,%lo($LC37)
	li	$5,47			# 0x2f
	lui	$2,%hi(strrchr)
	addiu	$2,$2,%lo(strrchr)
	jalr	$2
	nop

	addiu	$3,$2,1
	lw	$2,36($sp)
	sw	$2,16($sp)
	lw	$2,32($sp)
	sw	$2,20($sp)
	lui	$2,%hi($LC117)
	addiu	$4,$2,%lo($LC117)
	move	$5,$16
	move	$6,$3
	li	$7,2540			# 0x9ec
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L273:
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,116($2)
	move	$3,$2
	li	$2,20			# 0x14
	sw	$2,16($sp)
	addiu	$2,$sp,64
	sw	$2,20($sp)
	li	$2,4			# 0x4
	sw	$2,24($sp)
	move	$4,$0
	move	$5,$3
	li	$6,112			# 0x70
	li	$7,2			# 0x2
	lui	$2,%hi(phy_I2C_read)
	addiu	$2,$2,%lo(phy_I2C_read)
	jalr	$2
	nop

	lbu	$2,66($sp)
	seb	$3,$2
	li	$2,-8			# 0xfffffffffffffff8
	and	$2,$3,$2
	seb	$2,$2
	ori	$2,$2,0x5
	seb	$2,$2
	andi	$2,$2,0x00ff
	sb	$2,66($sp)
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,116($2)
	move	$3,$2
	li	$2,20			# 0x14
	sw	$2,16($sp)
	addiu	$2,$sp,64
	sw	$2,20($sp)
	li	$2,4			# 0x4
	sw	$2,24($sp)
	move	$4,$0
	move	$5,$3
	li	$6,112			# 0x70
	li	$7,2			# 0x2
	lui	$2,%hi(phy_I2C_write)
	addiu	$2,$2,%lo(phy_I2C_write)
	jalr	$2
	nop

	lw	$2,56($sp)
	andi	$2,$2,0x00ff
	sb	$2,64($sp)
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,116($2)
	move	$3,$2
	li	$2,36			# 0x24
	sw	$2,16($sp)
	addiu	$2,$sp,64
	sw	$2,20($sp)
	li	$2,1			# 0x1
	sw	$2,24($sp)
	move	$4,$0
	move	$5,$3
	li	$6,112			# 0x70
	li	$7,2			# 0x2
	lui	$2,%hi(phy_I2C_write)
	addiu	$2,$2,%lo(phy_I2C_write)
	jalr	$2
	nop

	lui	$2,%hi(i2c_protect)
	sw	$0,%lo(i2c_protect)($2)
	lw	$2,36($sp)
	sltu	$2,$2,6
	beq	$2,$0,$L274
	nop

	lw	$2,36($sp)
	sll	$3,$2,2
	lui	$2,%hi($L276)
	addiu	$2,$2,%lo($L276)
	addu	$2,$3,$2
	lw	$2,0($2)
	j	$2
	nop

	.rdata
	.align	2
	.align	2
$L276:
	.word	$L275
	.word	$L277
	.word	$L278
	.word	$L279
	.word	$L280
	.word	$L281
	.section	.text.mt7570_RSSI_current
$L275:
	li	$2,1			# 0x1
	sw	$2,40($sp)
	j	$L274
	nop

$L277:
	li	$2,4			# 0x4
	sw	$2,40($sp)
	j	$L274
	nop

$L278:
	li	$2,16			# 0x10
	sw	$2,40($sp)
	j	$L274
	nop

$L279:
	li	$2,64			# 0x40
	sw	$2,40($sp)
	j	$L274
	nop

$L280:
	li	$2,128			# 0x80
	sw	$2,40($sp)
	j	$L274
	nop

$L281:
	li	$2,256			# 0x100
	sw	$2,40($sp)
	nop
$L274:
	lui	$2,%hi(RSSI_Vref)
	lw	$2,%lo(RSSI_Vref)($2)
	move	$3,$2
	lw	$2,32($sp)
	sltu	$2,$3,$2
	bne	$2,$0,$L282
	nop

	lui	$2,%hi(RSSI_Vref)
	lw	$2,%lo(RSSI_Vref)($2)
	move	$3,$2
	lw	$2,32($sp)
	subu	$3,$3,$2
	lw	$2,40($sp)
	mul	$2,$3,$2
	move	$3,$2
	lui	$2,%hi(global_RSSI_current)
	sw	$3,%lo(global_RSSI_current)($2)
	lui	$2,%hi(global_RSSI_current)
	lw	$2,%lo(global_RSSI_current)($2)
	j	$L283
	nop

$L282:
	lui	$2,%hi(global_RSSI_current)
	sw	$0,%lo(global_RSSI_current)($2)
	lui	$2,%hi(global_RSSI_current)
	lw	$2,%lo(global_RSSI_current)($2)
$L283:
	lw	$31,76($sp)
	lw	$16,72($sp)
	addiu	$sp,$sp,80
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	mt7570_RSSI_current
	.size	mt7570_RSSI_current, .-mt7570_RSSI_current
	.rdata
	.align	2
$LC118:
	.ascii	"RSSI voltage = %x\012\000"
	.section	.text.mt7570_dark_current,"ax",@progbits
	.align	2
	.globl	mt7570_dark_current
	.set	nomips16
	.set	nomicromips
	.ent	mt7570_dark_current
	.type	mt7570_dark_current, @function
mt7570_dark_current:
	.frame	$sp,64,$31		# vars= 24, regs= 1/0, args= 32, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-64
	sw	$31,60($sp)
	sw	$0,40($sp)
	sw	$0,32($sp)
	sw	$0,44($sp)
	li	$2,64			# 0x40
	sw	$2,48($sp)
	sw	$0,36($sp)
	lui	$2,%hi(i2c_access_protect)
	addiu	$2,$2,%lo(i2c_access_protect)
	jalr	$2
	nop

	move	$3,$2
	li	$2,-1			# 0xffffffffffffffff
	bne	$3,$2,$L285
	nop

	lui	$2,%hi($LC70)
	addiu	$4,$2,%lo($LC70)
	lui	$2,%hi(__func__.25403)
	addiu	$5,$2,%lo(__func__.25403)
	li	$6,2610			# 0xa32
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	j	$L286
	nop

$L285:
	lui	$2,%hi(i2c_protect)
	li	$3,1			# 0x1
	sw	$3,%lo(i2c_protect)($2)
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,116($2)
	move	$3,$2
	li	$2,36			# 0x24
	sw	$2,16($sp)
	addiu	$2,$sp,52
	sw	$2,20($sp)
	li	$2,1			# 0x1
	sw	$2,24($sp)
	move	$4,$0
	move	$5,$3
	li	$6,112			# 0x70
	li	$7,2			# 0x2
	lui	$2,%hi(phy_I2C_read)
	addiu	$2,$2,%lo(phy_I2C_read)
	jalr	$2
	nop

	lbu	$2,52($sp)
	sw	$2,40($sp)
	lbu	$2,52($sp)
	seb	$3,$2
	li	$2,-31			# 0xffffffffffffffe1
	and	$2,$3,$2
	seb	$2,$2
	ori	$2,$2,0x2
	seb	$2,$2
	andi	$2,$2,0x00ff
	sb	$2,52($sp)
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,116($2)
	move	$3,$2
	li	$2,36			# 0x24
	sw	$2,16($sp)
	addiu	$2,$sp,52
	sw	$2,20($sp)
	li	$2,1			# 0x1
	sw	$2,24($sp)
	move	$4,$0
	move	$5,$3
	li	$6,112			# 0x70
	li	$7,2			# 0x2
	lui	$2,%hi(phy_I2C_write)
	addiu	$2,$2,%lo(phy_I2C_write)
	jalr	$2
	nop

	sw	$0,44($sp)
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,116($2)
	move	$3,$2
	li	$2,20			# 0x14
	sw	$2,16($sp)
	addiu	$2,$sp,52
	sw	$2,20($sp)
	li	$2,4			# 0x4
	sw	$2,24($sp)
	move	$4,$0
	move	$5,$3
	li	$6,112			# 0x70
	li	$7,2			# 0x2
	lui	$2,%hi(phy_I2C_read)
	addiu	$2,$2,%lo(phy_I2C_read)
	jalr	$2
	nop

	lbu	$3,54($sp)
	li	$2,-8			# 0xfffffffffffffff8
	and	$2,$3,$2
	andi	$3,$2,0x00ff
	lw	$2,44($sp)
	andi	$2,$2,0x00ff
	or	$2,$3,$2
	andi	$2,$2,0x00ff
	sb	$2,54($sp)
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,116($2)
	move	$3,$2
	li	$2,20			# 0x14
	sw	$2,16($sp)
	addiu	$2,$sp,52
	sw	$2,20($sp)
	li	$2,4			# 0x4
	sw	$2,24($sp)
	move	$4,$0
	move	$5,$3
	li	$6,112			# 0x70
	li	$7,2			# 0x2
	lui	$2,%hi(phy_I2C_write)
	addiu	$2,$2,%lo(phy_I2C_write)
	jalr	$2
	nop

	sw	$0,36($sp)
	j	$L287
	nop

$L288:
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,116($2)
	move	$3,$2
	li	$2,345			# 0x159
	sw	$2,16($sp)
	addiu	$2,$sp,52
	sw	$2,20($sp)
	li	$2,1			# 0x1
	sw	$2,24($sp)
	move	$4,$0
	move	$5,$3
	li	$6,112			# 0x70
	li	$7,2			# 0x2
	lui	$2,%hi(phy_I2C_read)
	addiu	$2,$2,%lo(phy_I2C_read)
	jalr	$2
	nop

	lbu	$2,52($sp)
	ori	$2,$2,0x10
	andi	$2,$2,0x00ff
	sb	$2,52($sp)
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,116($2)
	move	$3,$2
	li	$2,345			# 0x159
	sw	$2,16($sp)
	addiu	$2,$sp,52
	sw	$2,20($sp)
	li	$2,1			# 0x1
	sw	$2,24($sp)
	move	$4,$0
	move	$5,$3
	li	$6,112			# 0x70
	li	$7,2			# 0x2
	lui	$2,%hi(phy_I2C_write)
	addiu	$2,$2,%lo(phy_I2C_write)
	jalr	$2
	nop

	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,116($2)
	move	$3,$2
	li	$2,340			# 0x154
	sw	$2,16($sp)
	addiu	$2,$sp,52
	sw	$2,20($sp)
	li	$2,2			# 0x2
	sw	$2,24($sp)
	move	$4,$0
	move	$5,$3
	li	$6,112			# 0x70
	li	$7,2			# 0x2
	lui	$2,%hi(phy_I2C_read)
	addiu	$2,$2,%lo(phy_I2C_read)
	jalr	$2
	nop

	lbu	$2,52($sp)
	move	$3,$2
	lbu	$2,53($sp)
	sll	$2,$2,8
	or	$2,$3,$2
	move	$3,$2
	lw	$2,32($sp)
	addu	$2,$2,$3
	sw	$2,32($sp)
	lw	$2,36($sp)
	addiu	$2,$2,1
	sw	$2,36($sp)
$L287:
	lw	$3,36($sp)
	lw	$2,48($sp)
	sltu	$2,$3,$2
	bne	$2,$0,$L288
	nop

	lui	$2,%hi($LC118)
	addiu	$4,$2,%lo($LC118)
	lw	$5,32($sp)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,116($2)
	move	$3,$2
	li	$2,20			# 0x14
	sw	$2,16($sp)
	addiu	$2,$sp,52
	sw	$2,20($sp)
	li	$2,4			# 0x4
	sw	$2,24($sp)
	move	$4,$0
	move	$5,$3
	li	$6,112			# 0x70
	li	$7,2			# 0x2
	lui	$2,%hi(phy_I2C_read)
	addiu	$2,$2,%lo(phy_I2C_read)
	jalr	$2
	nop

	lbu	$2,54($sp)
	seb	$3,$2
	li	$2,-8			# 0xfffffffffffffff8
	and	$2,$3,$2
	seb	$2,$2
	ori	$2,$2,0x5
	seb	$2,$2
	andi	$2,$2,0x00ff
	sb	$2,54($sp)
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,116($2)
	move	$3,$2
	li	$2,20			# 0x14
	sw	$2,16($sp)
	addiu	$2,$sp,52
	sw	$2,20($sp)
	li	$2,4			# 0x4
	sw	$2,24($sp)
	move	$4,$0
	move	$5,$3
	li	$6,112			# 0x70
	li	$7,2			# 0x2
	lui	$2,%hi(phy_I2C_write)
	addiu	$2,$2,%lo(phy_I2C_write)
	jalr	$2
	nop

	lw	$2,40($sp)
	andi	$2,$2,0x00ff
	sb	$2,52($sp)
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,116($2)
	move	$3,$2
	li	$2,36			# 0x24
	sw	$2,16($sp)
	addiu	$2,$sp,52
	sw	$2,20($sp)
	li	$2,1			# 0x1
	sw	$2,24($sp)
	move	$4,$0
	move	$5,$3
	li	$6,112			# 0x70
	li	$7,2			# 0x2
	lui	$2,%hi(phy_I2C_write)
	addiu	$2,$2,%lo(phy_I2C_write)
	jalr	$2
	nop

	lui	$2,%hi(i2c_protect)
	sw	$0,%lo(i2c_protect)($2)
$L286:
	move	$2,$0
	lw	$31,60($sp)
	addiu	$sp,$sp,64
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	mt7570_dark_current
	.size	mt7570_dark_current, .-mt7570_dark_current
	.rdata
	.align	2
$LC119:
	.ascii	"Rogue ONU clear\012\000"
	.section	.text.mt7570_RougeONU_clear,"ax",@progbits
	.align	2
	.globl	mt7570_RougeONU_clear
	.set	nomips16
	.set	nomicromips
	.ent	mt7570_RougeONU_clear
	.type	mt7570_RougeONU_clear, @function
mt7570_RougeONU_clear:
	.frame	$sp,48,$31		# vars= 8, regs= 1/0, args= 32, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-48
	sw	$31,44($sp)
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,116($2)
	move	$3,$2
	li	$2,361			# 0x169
	sw	$2,16($sp)
	addiu	$2,$sp,32
	sw	$2,20($sp)
	li	$2,1			# 0x1
	sw	$2,24($sp)
	move	$4,$0
	move	$5,$3
	li	$6,112			# 0x70
	li	$7,2			# 0x2
	lui	$2,%hi(phy_I2C_read)
	addiu	$2,$2,%lo(phy_I2C_read)
	jalr	$2
	nop

	lbu	$2,32($sp)
	ori	$2,$2,0x1
	andi	$2,$2,0x00ff
	sb	$2,32($sp)
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,116($2)
	move	$3,$2
	li	$2,361			# 0x169
	sw	$2,16($sp)
	addiu	$2,$sp,32
	sw	$2,20($sp)
	li	$2,1			# 0x1
	sw	$2,24($sp)
	move	$4,$0
	move	$5,$3
	li	$6,112			# 0x70
	li	$7,2			# 0x2
	lui	$2,%hi(phy_I2C_write)
	addiu	$2,$2,%lo(phy_I2C_write)
	jalr	$2
	nop

	lui	$2,%hi($LC119)
	addiu	$4,$2,%lo($LC119)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lw	$31,44($sp)
	addiu	$sp,$sp,48
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	mt7570_RougeONU_clear
	.size	mt7570_RougeONU_clear, .-mt7570_RougeONU_clear
	.rdata
	.align	2
$LC120:
	.ascii	"MPD Current Offset = 0x%x\012\000"
	.section	.text.mt7570_MPD_current_calibration,"ax",@progbits
	.align	2
	.globl	mt7570_MPD_current_calibration
	.set	nomips16
	.set	nomicromips
	.ent	mt7570_MPD_current_calibration
	.type	mt7570_MPD_current_calibration, @function
mt7570_MPD_current_calibration:
	.frame	$sp,48,$31		# vars= 8, regs= 1/0, args= 32, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-48
	sw	$31,44($sp)
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,116($2)
	move	$3,$2
	sw	$0,16($sp)
	addiu	$2,$sp,32
	sw	$2,20($sp)
	li	$2,1			# 0x1
	sw	$2,24($sp)
	move	$4,$0
	move	$5,$3
	li	$6,112			# 0x70
	li	$7,2			# 0x2
	lui	$2,%hi(phy_I2C_read)
	addiu	$2,$2,%lo(phy_I2C_read)
	jalr	$2
	nop

	lbu	$2,32($sp)
	seb	$3,$2
	li	$2,-15			# 0xfffffffffffffff1
	and	$2,$3,$2
	seb	$2,$2
	ori	$2,$2,0x8
	seb	$2,$2
	andi	$2,$2,0x00ff
	sb	$2,32($sp)
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,116($2)
	move	$3,$2
	sw	$0,16($sp)
	addiu	$2,$sp,32
	sw	$2,20($sp)
	li	$2,1			# 0x1
	sw	$2,24($sp)
	move	$4,$0
	move	$5,$3
	li	$6,112			# 0x70
	li	$7,2			# 0x2
	lui	$2,%hi(phy_I2C_write)
	addiu	$2,$2,%lo(phy_I2C_write)
	jalr	$2
	nop

	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,116($2)
	move	$3,$2
	li	$2,36			# 0x24
	sw	$2,16($sp)
	addiu	$2,$sp,32
	sw	$2,20($sp)
	li	$2,1			# 0x1
	sw	$2,24($sp)
	move	$4,$0
	move	$5,$3
	li	$6,112			# 0x70
	li	$7,2			# 0x2
	lui	$2,%hi(phy_I2C_read)
	addiu	$2,$2,%lo(phy_I2C_read)
	jalr	$2
	nop

	lbu	$2,32($sp)
	seb	$3,$2
	li	$2,-31			# 0xffffffffffffffe1
	and	$2,$3,$2
	seb	$2,$2
	ori	$2,$2,0x4
	seb	$2,$2
	andi	$2,$2,0x00ff
	sb	$2,32($sp)
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,116($2)
	move	$3,$2
	li	$2,36			# 0x24
	sw	$2,16($sp)
	addiu	$2,$sp,32
	sw	$2,20($sp)
	li	$2,1			# 0x1
	sw	$2,24($sp)
	move	$4,$0
	move	$5,$3
	li	$6,112			# 0x70
	li	$7,2			# 0x2
	lui	$2,%hi(phy_I2C_write)
	addiu	$2,$2,%lo(phy_I2C_write)
	jalr	$2
	nop

	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,116($2)
	move	$3,$2
	li	$2,345			# 0x159
	sw	$2,16($sp)
	addiu	$2,$sp,32
	sw	$2,20($sp)
	li	$2,1			# 0x1
	sw	$2,24($sp)
	move	$4,$0
	move	$5,$3
	li	$6,112			# 0x70
	li	$7,2			# 0x2
	lui	$2,%hi(phy_I2C_read)
	addiu	$2,$2,%lo(phy_I2C_read)
	jalr	$2
	nop

	lbu	$2,32($sp)
	ori	$2,$2,0x10
	andi	$2,$2,0x00ff
	sb	$2,32($sp)
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,116($2)
	move	$3,$2
	li	$2,345			# 0x159
	sw	$2,16($sp)
	addiu	$2,$sp,32
	sw	$2,20($sp)
	li	$2,1			# 0x1
	sw	$2,24($sp)
	move	$4,$0
	move	$5,$3
	li	$6,112			# 0x70
	li	$7,2			# 0x2
	lui	$2,%hi(phy_I2C_write)
	addiu	$2,$2,%lo(phy_I2C_write)
	jalr	$2
	nop

	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,116($2)
	move	$3,$2
	li	$2,340			# 0x154
	sw	$2,16($sp)
	addiu	$2,$sp,32
	sw	$2,20($sp)
	li	$2,2			# 0x2
	sw	$2,24($sp)
	move	$4,$0
	move	$5,$3
	li	$6,112			# 0x70
	li	$7,2			# 0x2
	lui	$2,%hi(phy_I2C_read)
	addiu	$2,$2,%lo(phy_I2C_read)
	jalr	$2
	nop

	lbu	$2,32($sp)
	move	$3,$2
	lbu	$2,33($sp)
	sll	$2,$2,8
	or	$2,$3,$2
	move	$3,$2
	lui	$2,%hi(MPD_current_offset)
	sw	$3,%lo(MPD_current_offset)($2)
	lui	$2,%hi(MPD_current_offset)
	lw	$2,%lo(MPD_current_offset)($2)
	move	$3,$2
	lui	$2,%hi(global_MPD_current)
	sw	$3,%lo(global_MPD_current)($2)
	lui	$2,%hi(MPD_current_offset)
	lw	$3,%lo(MPD_current_offset)($2)
	lui	$2,%hi($LC120)
	addiu	$4,$2,%lo($LC120)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,116($2)
	move	$3,$2
	sw	$0,16($sp)
	addiu	$2,$sp,32
	sw	$2,20($sp)
	li	$2,1			# 0x1
	sw	$2,24($sp)
	move	$4,$0
	move	$5,$3
	li	$6,112			# 0x70
	li	$7,2			# 0x2
	lui	$2,%hi(phy_I2C_read)
	addiu	$2,$2,%lo(phy_I2C_read)
	jalr	$2
	nop

	lbu	$2,32($sp)
	seb	$3,$2
	li	$2,-15			# 0xfffffffffffffff1
	and	$2,$3,$2
	seb	$2,$2
	ori	$2,$2,0x8
	seb	$2,$2
	andi	$2,$2,0x00ff
	sb	$2,32($sp)
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,116($2)
	move	$3,$2
	sw	$0,16($sp)
	addiu	$2,$sp,32
	sw	$2,20($sp)
	li	$2,1			# 0x1
	sw	$2,24($sp)
	move	$4,$0
	move	$5,$3
	li	$6,112			# 0x70
	li	$7,2			# 0x2
	lui	$2,%hi(phy_I2C_write)
	addiu	$2,$2,%lo(phy_I2C_write)
	jalr	$2
	nop

	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,116($2)
	move	$3,$2
	li	$2,36			# 0x24
	sw	$2,16($sp)
	addiu	$2,$sp,32
	sw	$2,20($sp)
	li	$2,1			# 0x1
	sw	$2,24($sp)
	move	$4,$0
	move	$5,$3
	li	$6,112			# 0x70
	li	$7,2			# 0x2
	lui	$2,%hi(phy_I2C_read)
	addiu	$2,$2,%lo(phy_I2C_read)
	jalr	$2
	nop

	lbu	$3,32($sp)
	li	$2,-31			# 0xffffffffffffffe1
	and	$2,$3,$2
	andi	$2,$2,0x00ff
	sb	$2,32($sp)
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,116($2)
	move	$3,$2
	li	$2,36			# 0x24
	sw	$2,16($sp)
	addiu	$2,$sp,32
	sw	$2,20($sp)
	li	$2,1			# 0x1
	sw	$2,24($sp)
	move	$4,$0
	move	$5,$3
	li	$6,112			# 0x70
	li	$7,2			# 0x2
	lui	$2,%hi(phy_I2C_write)
	addiu	$2,$2,%lo(phy_I2C_write)
	jalr	$2
	nop

	lw	$31,44($sp)
	addiu	$sp,$sp,48
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	mt7570_MPD_current_calibration
	.size	mt7570_MPD_current_calibration, .-mt7570_MPD_current_calibration
	.rdata
	.align	2
$LC121:
	.ascii	"[%lu]0ms[%s:%d]\012  MPD temp = 0x%x\012\000"
	.align	2
$LC122:
	.ascii	"[%lu]0ms[%s:%d]\012  No Tx power, return %x\012\000"
	.align	2
$LC123:
	.ascii	"Invalid content of FLASH 0x88\012\000"
	.align	2
$LC124:
	.ascii	"[%lu]0ms[%s:%d]\012  GPON mode\012\000"
	.align	2
$LC125:
	.ascii	"[%lu]0ms[%s:%d]\012  EPON mode\012\000"
	.align	2
$LC127:
	.ascii	"[%lu]0ms[%s:%d]\012  MPD current var = 0x%x\012\000"
	.align	2
$LC128:
	.ascii	"[%lu]0ms[%s:%d]\012  MPD in zone 1, Tx_power_8472 = %d\012"
	.ascii	"\000"
	.align	2
$LC129:
	.ascii	"[%lu]0ms[%s:%d]\012  temp(var) = 0x%x\012\000"
	.align	2
$LC130:
	.ascii	"[%lu]0ms[%s:%d]\012  MPD in zone 1a, Tx_power_8472 = %d,"
	.ascii	" return %x\012\000"
	.align	2
$LC131:
	.ascii	"[%lu]0ms[%s:%d]\012  MPD in zone 1b, Tx_power_8472 = %d,"
	.ascii	" return %x\012\000"
	.align	2
$LC132:
	.ascii	"[%lu]0ms[%s:%d]\012  MPD in zone 2, Tx_power_8472 = %d, "
	.ascii	"return %x\012\000"
	.align	2
$LC133:
	.ascii	"[%lu]0ms[%s:%d]\012  MPD in zone 3, Tx_power_8472 = %d, "
	.ascii	"return %x\012\000"
	.section	.text.mt7570_MPD_current,"ax",@progbits
	.align	2
	.globl	mt7570_MPD_current
	.set	nomips16
	.set	nomicromips
	.ent	mt7570_MPD_current
	.type	mt7570_MPD_current, @function
mt7570_MPD_current:
	.frame	$sp,104,$31		# vars= 64, regs= 2/0, args= 32, gp= 0
	.mask	0x80010000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-104
	sw	$31,100($sp)
	sw	$16,96($sp)
	sw	$0,56($sp)
	sw	$0,32($sp)
	sw	$0,36($sp)
	sw	$0,40($sp)
	lui	$2,%hi(vEN7570)
	lw	$2,%lo(vEN7570)($2)
	bne	$2,$0,$L293
	nop

	lui	$2,%hi(TEC_switch)
	lw	$2,%lo(TEC_switch)($2)
	beq	$2,$0,$L293
	nop

	li	$2,10			# 0xa
	sw	$2,40($sp)
	j	$L294
	nop

$L293:
	li	$2,1			# 0x1
	sw	$2,40($sp)
$L294:
	lui	$2,%hi(i2c_access_protect)
	addiu	$2,$2,%lo(i2c_access_protect)
	jalr	$2
	nop

	move	$3,$2
	li	$2,-1			# 0xffffffffffffffff
	bne	$3,$2,$L295
	nop

	lui	$2,%hi($LC70)
	addiu	$4,$2,%lo($LC70)
	lui	$2,%hi(__func__.25423)
	addiu	$5,$2,%lo(__func__.25423)
	li	$6,2754			# 0xac2
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi(global_MPD_current)
	lw	$2,%lo(global_MPD_current)($2)
	j	$L322
	nop

$L295:
	lui	$2,%hi(i2c_protect)
	li	$3,1			# 0x1
	sw	$3,%lo(i2c_protect)($2)
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,116($2)
	move	$3,$2
	sw	$0,16($sp)
	addiu	$2,$sp,88
	sw	$2,20($sp)
	li	$2,1			# 0x1
	sw	$2,24($sp)
	move	$4,$0
	move	$5,$3
	li	$6,112			# 0x70
	li	$7,2			# 0x2
	lui	$2,%hi(phy_I2C_read)
	addiu	$2,$2,%lo(phy_I2C_read)
	jalr	$2
	nop

	lbu	$3,88($sp)
	li	$2,-15			# 0xfffffffffffffff1
	and	$2,$3,$2
	andi	$2,$2,0x00ff
	sb	$2,88($sp)
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,116($2)
	move	$3,$2
	sw	$0,16($sp)
	addiu	$2,$sp,88
	sw	$2,20($sp)
	li	$2,1			# 0x1
	sw	$2,24($sp)
	move	$4,$0
	move	$5,$3
	li	$6,112			# 0x70
	li	$7,2			# 0x2
	lui	$2,%hi(phy_I2C_write)
	addiu	$2,$2,%lo(phy_I2C_write)
	jalr	$2
	nop

	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,116($2)
	move	$3,$2
	li	$2,36			# 0x24
	sw	$2,16($sp)
	addiu	$2,$sp,88
	sw	$2,20($sp)
	li	$2,1			# 0x1
	sw	$2,24($sp)
	move	$4,$0
	move	$5,$3
	li	$6,112			# 0x70
	li	$7,2			# 0x2
	lui	$2,%hi(phy_I2C_read)
	addiu	$2,$2,%lo(phy_I2C_read)
	jalr	$2
	nop

	lbu	$2,88($sp)
	seb	$3,$2
	li	$2,-31			# 0xffffffffffffffe1
	and	$2,$3,$2
	seb	$2,$2
	ori	$2,$2,0x4
	seb	$2,$2
	andi	$2,$2,0x00ff
	sb	$2,88($sp)
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,116($2)
	move	$3,$2
	li	$2,36			# 0x24
	sw	$2,16($sp)
	addiu	$2,$sp,88
	sw	$2,20($sp)
	li	$2,1			# 0x1
	sw	$2,24($sp)
	move	$4,$0
	move	$5,$3
	li	$6,112			# 0x70
	li	$7,2			# 0x2
	lui	$2,%hi(phy_I2C_write)
	addiu	$2,$2,%lo(phy_I2C_write)
	jalr	$2
	nop

	sw	$0,36($sp)
	j	$L297
	nop

$L300:
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,116($2)
	move	$3,$2
	li	$2,345			# 0x159
	sw	$2,16($sp)
	addiu	$2,$sp,88
	sw	$2,20($sp)
	li	$2,1			# 0x1
	sw	$2,24($sp)
	move	$4,$0
	move	$5,$3
	li	$6,112			# 0x70
	li	$7,2			# 0x2
	lui	$2,%hi(phy_I2C_read)
	addiu	$2,$2,%lo(phy_I2C_read)
	jalr	$2
	nop

	lbu	$2,88($sp)
	ori	$2,$2,0x10
	andi	$2,$2,0x00ff
	sb	$2,88($sp)
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,116($2)
	move	$3,$2
	li	$2,345			# 0x159
	sw	$2,16($sp)
	addiu	$2,$sp,88
	sw	$2,20($sp)
	li	$2,1			# 0x1
	sw	$2,24($sp)
	move	$4,$0
	move	$5,$3
	li	$6,112			# 0x70
	li	$7,2			# 0x2
	lui	$2,%hi(phy_I2C_write)
	addiu	$2,$2,%lo(phy_I2C_write)
	jalr	$2
	nop

	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,116($2)
	move	$3,$2
	li	$2,340			# 0x154
	sw	$2,16($sp)
	addiu	$2,$sp,88
	sw	$2,20($sp)
	li	$2,2			# 0x2
	sw	$2,24($sp)
	move	$4,$0
	move	$5,$3
	li	$6,112			# 0x70
	li	$7,2			# 0x2
	lui	$2,%hi(phy_I2C_read)
	addiu	$2,$2,%lo(phy_I2C_read)
	jalr	$2
	nop

	lbu	$2,88($sp)
	move	$3,$2
	lbu	$2,89($sp)
	sll	$2,$2,8
	or	$2,$3,$2
	sw	$2,56($sp)
	lw	$2,32($sp)
	bne	$2,$0,$L298
	nop

	lw	$2,56($sp)
	sw	$2,32($sp)
	j	$L299
	nop

$L298:
	lw	$3,32($sp)
	lw	$2,56($sp)
	sltu	$2,$3,$2
	beq	$2,$0,$L299
	nop

	lw	$2,56($sp)
	sw	$2,32($sp)
$L299:
	lw	$2,36($sp)
	addiu	$2,$2,1
	sw	$2,36($sp)
$L297:
	lw	$3,36($sp)
	lw	$2,40($sp)
	slt	$2,$3,$2
	bne	$2,$0,$L300
	nop

	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,116($2)
	move	$3,$2
	li	$2,36			# 0x24
	sw	$2,16($sp)
	addiu	$2,$sp,88
	sw	$2,20($sp)
	li	$2,1			# 0x1
	sw	$2,24($sp)
	move	$4,$0
	move	$5,$3
	li	$6,112			# 0x70
	li	$7,2			# 0x2
	lui	$2,%hi(phy_I2C_read)
	addiu	$2,$2,%lo(phy_I2C_read)
	jalr	$2
	nop

	lbu	$3,88($sp)
	li	$2,-31			# 0xffffffffffffffe1
	and	$2,$3,$2
	andi	$2,$2,0x00ff
	sb	$2,88($sp)
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,116($2)
	move	$3,$2
	li	$2,36			# 0x24
	sw	$2,16($sp)
	addiu	$2,$sp,88
	sw	$2,20($sp)
	li	$2,1			# 0x1
	sw	$2,24($sp)
	move	$4,$0
	move	$5,$3
	li	$6,112			# 0x70
	li	$7,2			# 0x2
	lui	$2,%hi(phy_I2C_write)
	addiu	$2,$2,%lo(phy_I2C_write)
	jalr	$2
	nop

	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,116($2)
	move	$3,$2
	sw	$0,16($sp)
	addiu	$2,$sp,88
	sw	$2,20($sp)
	li	$2,1			# 0x1
	sw	$2,24($sp)
	move	$4,$0
	move	$5,$3
	li	$6,112			# 0x70
	li	$7,2			# 0x2
	lui	$2,%hi(phy_I2C_read)
	addiu	$2,$2,%lo(phy_I2C_read)
	jalr	$2
	nop

	lbu	$2,88($sp)
	seb	$3,$2
	li	$2,-15			# 0xfffffffffffffff1
	and	$2,$3,$2
	seb	$2,$2
	ori	$2,$2,0x8
	seb	$2,$2
	andi	$2,$2,0x00ff
	sb	$2,88($sp)
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,116($2)
	move	$3,$2
	sw	$0,16($sp)
	addiu	$2,$sp,88
	sw	$2,20($sp)
	li	$2,1			# 0x1
	sw	$2,24($sp)
	move	$4,$0
	move	$5,$3
	li	$6,112			# 0x70
	li	$7,2			# 0x2
	lui	$2,%hi(phy_I2C_write)
	addiu	$2,$2,%lo(phy_I2C_write)
	jalr	$2
	nop

	lui	$2,%hi(i2c_protect)
	sw	$0,%lo(i2c_protect)($2)
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,136($2)
	andi	$2,$2,0x10
	beq	$2,$0,$L301
	nop

	lui	$2,%hi(jiffies)
	lw	$16,%lo(jiffies)($2)
	lui	$2,%hi($LC37)
	addiu	$4,$2,%lo($LC37)
	li	$5,47			# 0x2f
	lui	$2,%hi(strrchr)
	addiu	$2,$2,%lo(strrchr)
	jalr	$2
	nop

	addiu	$3,$2,1
	lw	$2,32($sp)
	sw	$2,16($sp)
	lui	$2,%hi($LC121)
	addiu	$4,$2,%lo($LC121)
	move	$5,$16
	move	$6,$3
	li	$7,2800			# 0xaf0
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L301:
	lui	$2,%hi(MPD_current_offset)
	lw	$2,%lo(MPD_current_offset)($2)
	lw	$3,32($sp)
	sltu	$2,$3,$2
	beq	$2,$0,$L302
	nop

	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,136($2)
	andi	$2,$2,0x10
	beq	$2,$0,$L303
	nop

	lui	$2,%hi(jiffies)
	lw	$16,%lo(jiffies)($2)
	lui	$2,%hi($LC37)
	addiu	$4,$2,%lo($LC37)
	li	$5,47			# 0x2f
	lui	$2,%hi(strrchr)
	addiu	$2,$2,%lo(strrchr)
	jalr	$2
	nop

	addiu	$3,$2,1
	lui	$2,%hi(MPD_current_offset)
	lw	$2,%lo(MPD_current_offset)($2)
	sw	$2,16($sp)
	lui	$2,%hi($LC122)
	addiu	$4,$2,%lo($LC122)
	move	$5,$16
	move	$6,$3
	li	$7,2803			# 0xaf3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L303:
	lui	$2,%hi(MPD_current_offset)
	lw	$2,%lo(MPD_current_offset)($2)
	move	$3,$2
	lui	$2,%hi(global_MPD_current)
	sw	$3,%lo(global_MPD_current)($2)
	lui	$2,%hi(global_MPD_current)
	lw	$2,%lo(global_MPD_current)($2)
	j	$L322
	nop

$L302:
	lui	$2,%hi(vEN7570)
	lw	$2,%lo(vEN7570)($2)
	bne	$2,$0,$L304
	nop

	lui	$2,%hi(TEC_switch)
	lw	$2,%lo(TEC_switch)($2)
	beq	$2,$0,$L304
	nop

	sw	$0,60($sp)
	li	$4,136			# 0x88
	lui	$2,%hi(get_flash_register)
	addiu	$2,$2,%lo(get_flash_register)
	jalr	$2
	nop

	sw	$2,64($sp)
	lui	$2,%hi(MPD_current_offset)
	lw	$2,%lo(MPD_current_offset)($2)
	sw	$2,68($sp)
	move	$2,$0
	sw	$2,72($sp)
	move	$2,$0
	sw	$2,76($sp)
	sw	$0,44($sp)
	sw	$0,80($sp)
	sh	$0,84($sp)
	sw	$0,48($sp)
	sw	$0,52($sp)
	lw	$3,64($sp)
	lw	$2,68($sp)
	subu	$2,$3,$2
	sw	$2,60($sp)
	lw	$2,60($sp)
	bne	$2,$0,$L305
	nop

	lui	$2,%hi($LC123)
	addiu	$4,$2,%lo($LC123)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lw	$3,32($sp)
	lui	$2,%hi(global_MPD_current)
	sw	$3,%lo(global_MPD_current)($2)
	lui	$2,%hi(global_MPD_current)
	lw	$2,%lo(global_MPD_current)($2)
	j	$L322
	nop

$L305:
	lui	$2,%hi(xPON_mode)
	lw	$3,%lo(xPON_mode)($2)
	li	$2,1			# 0x1
	bne	$3,$2,$L306
	nop

	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,136($2)
	andi	$2,$2,0x10
	beq	$2,$0,$L307
	nop

	lui	$2,%hi(jiffies)
	lw	$16,%lo(jiffies)($2)
	lui	$2,%hi($LC37)
	addiu	$4,$2,%lo($LC37)
	li	$5,47			# 0x2f
	lui	$2,%hi(strrchr)
	addiu	$2,$2,%lo(strrchr)
	jalr	$2
	nop

	addiu	$3,$2,1
	lui	$2,%hi($LC124)
	addiu	$4,$2,%lo($LC124)
	move	$5,$16
	move	$6,$3
	li	$7,2832			# 0xb10
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L307:
	li	$2,177			# 0xb1
	sw	$2,44($sp)
	li	$2,25118			# 0x621e
	sw	$2,48($sp)
	li	$2,12589			# 0x312d
	sw	$2,52($sp)
	j	$L308
	nop

$L306:
	lui	$2,%hi(xPON_mode)
	lw	$2,%lo(xPON_mode)($2)
	bne	$2,$0,$L308
	nop

	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,136($2)
	andi	$2,$2,0x10
	beq	$2,$0,$L309
	nop

	lui	$2,%hi(jiffies)
	lw	$16,%lo(jiffies)($2)
	lui	$2,%hi($LC37)
	addiu	$4,$2,%lo($LC37)
	li	$5,47			# 0x2f
	lui	$2,%hi(strrchr)
	addiu	$2,$2,%lo(strrchr)
	jalr	$2
	nop

	addiu	$3,$2,1
	lui	$2,%hi($LC125)
	addiu	$4,$2,%lo($LC125)
	move	$5,$16
	move	$6,$3
	li	$7,2839			# 0xb17
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L309:
	li	$2,158			# 0x9e
	sw	$2,44($sp)
	li	$2,22387			# 0x5773
	sw	$2,48($sp)
	li	$2,11220			# 0x2bd4
	sw	$2,52($sp)
$L308:
	lw	$3,44($sp)
	lw	$2,80($sp)
	subu	$3,$3,$2
	lui	$2,%hi(__floatsisf)
	addiu	$2,$2,%lo(__floatsisf)
	move	$4,$3
	jalr	$2
	nop

	move	$16,$2
	lw	$3,64($sp)
	lw	$2,68($sp)
	subu	$3,$3,$2
	lui	$2,%hi(__floatsisf)
	addiu	$2,$2,%lo(__floatsisf)
	move	$4,$3
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(__divsf3)
	addiu	$2,$2,%lo(__divsf3)
	move	$4,$16
	move	$5,$3
	jalr	$2
	nop

	sw	$2,72($sp)
	lw	$2,68($sp)
	lw	$3,32($sp)
	subu	$3,$3,$2
	lui	$2,%hi(__floatunsisf)
	addiu	$2,$2,%lo(__floatunsisf)
	move	$4,$3
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(__mulsf3)
	addiu	$2,$2,%lo(__mulsf3)
	move	$4,$3
	lw	$5,72($sp)
	jalr	$2
	nop

	move	$16,$2
	lui	$2,%hi(__floatsisf)
	addiu	$2,$2,%lo(__floatsisf)
	lw	$4,80($sp)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(__addsf3)
	addiu	$2,$2,%lo(__addsf3)
	move	$4,$16
	move	$5,$3
	jalr	$2
	nop

	sw	$2,76($sp)
	lui	$3,%hi($LC16)
	lui	$2,%hi(__mulsf3)
	addiu	$2,$2,%lo(__mulsf3)
	lw	$4,76($sp)
	lw	$5,%lo($LC16)($3)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(__fixsfsi)
	addiu	$2,$2,%lo(__fixsfsi)
	move	$4,$3
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(__floatsidf)
	addiu	$2,$2,%lo(__floatsidf)
	move	$4,$3
	jalr	$2
	nop

	move	$5,$3
	move	$4,$2
	lui	$3,%hi($LC126)
	lui	$2,%hi(__divdf3)
	addiu	$2,$2,%lo(__divdf3)
	lw	$7,%lo($LC126+4)($3)
	lw	$6,%lo($LC126)($3)
	jalr	$2
	nop

	move	$5,$3
	move	$4,$2
	lui	$2,%hi(__fixunsdfsi)
	addiu	$2,$2,%lo(__fixunsdfsi)
	jalr	$2
	nop

	sh	$2,84($sp)
	lui	$2,%hi(global_MPD_current_var)
	lw	$2,%lo(global_MPD_current_var)($2)
	bne	$2,$0,$L310
	nop

	lhu	$3,84($sp)
	lw	$2,48($sp)
	slt	$2,$2,$3
	beq	$2,$0,$L310
	nop

	lui	$2,%hi(MPD_current_cnt)
	lw	$2,%lo(MPD_current_cnt)($2)
	addiu	$3,$2,1
	lui	$2,%hi(MPD_current_cnt)
	sw	$3,%lo(MPD_current_cnt)($2)
	lui	$2,%hi(MPD_current_cnt)
	lw	$3,%lo(MPD_current_cnt)($2)
	li	$2,2			# 0x2
	bne	$3,$2,$L310
	nop

	li	$4,136			# 0x88
	lui	$2,%hi(get_flash_register)
	addiu	$2,$2,%lo(get_flash_register)
	jalr	$2
	nop

	move	$3,$2
	lw	$2,32($sp)
	subu	$2,$2,$3
	move	$3,$2
	lui	$2,%hi(global_MPD_current_var)
	sw	$3,%lo(global_MPD_current_var)($2)
$L310:
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,136($2)
	andi	$2,$2,0x10
	beq	$2,$0,$L311
	nop

	lui	$2,%hi(jiffies)
	lw	$16,%lo(jiffies)($2)
	lui	$2,%hi($LC37)
	addiu	$4,$2,%lo($LC37)
	li	$5,47			# 0x2f
	lui	$2,%hi(strrchr)
	addiu	$2,$2,%lo(strrchr)
	jalr	$2
	nop

	addiu	$3,$2,1
	lui	$2,%hi(global_MPD_current_var)
	lw	$2,%lo(global_MPD_current_var)($2)
	sw	$2,16($sp)
	lui	$2,%hi($LC127)
	addiu	$4,$2,%lo($LC127)
	move	$5,$16
	move	$6,$3
	li	$7,2855			# 0xb27
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L311:
	lhu	$3,84($sp)
	lw	$2,48($sp)
	slt	$2,$2,$3
	beq	$2,$0,$L312
	nop

	lui	$2,%hi(global_MPD_current_var)
	lw	$2,%lo(global_MPD_current_var)($2)
	beq	$2,$0,$L312
	nop

	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,136($2)
	andi	$2,$2,0x10
	beq	$2,$0,$L313
	nop

	lui	$2,%hi(jiffies)
	lw	$16,%lo(jiffies)($2)
	lui	$2,%hi($LC37)
	addiu	$4,$2,%lo($LC37)
	li	$5,47			# 0x2f
	lui	$2,%hi(strrchr)
	addiu	$2,$2,%lo(strrchr)
	jalr	$2
	nop

	addiu	$3,$2,1
	lhu	$2,84($sp)
	sw	$2,16($sp)
	lui	$2,%hi($LC128)
	addiu	$4,$2,%lo($LC128)
	move	$5,$16
	move	$6,$3
	li	$7,2859			# 0xb2b
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L313:
	lui	$2,%hi(global_MPD_current_var)
	lw	$2,%lo(global_MPD_current_var)($2)
	move	$3,$2
	lw	$2,32($sp)
	subu	$2,$2,$3
	sw	$2,32($sp)
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,136($2)
	andi	$2,$2,0x10
	beq	$2,$0,$L314
	nop

	lui	$2,%hi(jiffies)
	lw	$16,%lo(jiffies)($2)
	lui	$2,%hi($LC37)
	addiu	$4,$2,%lo($LC37)
	li	$5,47			# 0x2f
	lui	$2,%hi(strrchr)
	addiu	$2,$2,%lo(strrchr)
	jalr	$2
	nop

	addiu	$3,$2,1
	lw	$2,32($sp)
	sw	$2,16($sp)
	lui	$2,%hi($LC129)
	addiu	$4,$2,%lo($LC129)
	move	$5,$16
	move	$6,$3
	li	$7,2861			# 0xb2d
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L314:
	lw	$2,68($sp)
	lw	$3,32($sp)
	subu	$3,$3,$2
	lui	$2,%hi(__floatunsisf)
	addiu	$2,$2,%lo(__floatunsisf)
	move	$4,$3
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(__mulsf3)
	addiu	$2,$2,%lo(__mulsf3)
	move	$4,$3
	lw	$5,72($sp)
	jalr	$2
	nop

	move	$16,$2
	lui	$2,%hi(__floatsisf)
	addiu	$2,$2,%lo(__floatsisf)
	lw	$4,80($sp)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(__addsf3)
	addiu	$2,$2,%lo(__addsf3)
	move	$4,$16
	move	$5,$3
	jalr	$2
	nop

	sw	$2,76($sp)
	lui	$3,%hi($LC16)
	lui	$2,%hi(__mulsf3)
	addiu	$2,$2,%lo(__mulsf3)
	lw	$4,76($sp)
	lw	$5,%lo($LC16)($3)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(__fixsfsi)
	addiu	$2,$2,%lo(__fixsfsi)
	move	$4,$3
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(__floatsidf)
	addiu	$2,$2,%lo(__floatsidf)
	move	$4,$3
	jalr	$2
	nop

	move	$5,$3
	move	$4,$2
	lui	$3,%hi($LC126)
	lui	$2,%hi(__divdf3)
	addiu	$2,$2,%lo(__divdf3)
	lw	$7,%lo($LC126+4)($3)
	lw	$6,%lo($LC126)($3)
	jalr	$2
	nop

	move	$5,$3
	move	$4,$2
	lui	$2,%hi(__fixunsdfsi)
	addiu	$2,$2,%lo(__fixunsdfsi)
	jalr	$2
	nop

	sh	$2,84($sp)
	lhu	$3,84($sp)
	lw	$2,48($sp)
	slt	$2,$2,$3
	bne	$2,$0,$L315
	nop

	lhu	$3,84($sp)
	lw	$2,52($sp)
	slt	$2,$3,$2
	beq	$2,$0,$L316
	nop

$L315:
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,136($2)
	andi	$2,$2,0x10
	beq	$2,$0,$L317
	nop

	lui	$2,%hi(jiffies)
	lw	$16,%lo(jiffies)($2)
	lui	$2,%hi($LC37)
	addiu	$4,$2,%lo($LC37)
	li	$5,47			# 0x2f
	lui	$2,%hi(strrchr)
	addiu	$2,$2,%lo(strrchr)
	jalr	$2
	nop

	addiu	$6,$2,1
	lhu	$3,84($sp)
	lui	$2,%hi(global_MPD_current)
	lw	$2,%lo(global_MPD_current)($2)
	sw	$3,16($sp)
	sw	$2,20($sp)
	lui	$2,%hi($LC130)
	addiu	$4,$2,%lo($LC130)
	move	$5,$16
	li	$7,2866			# 0xb32
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L317:
	lui	$2,%hi(global_MPD_current_var)
	sw	$0,%lo(global_MPD_current_var)($2)
	lui	$2,%hi(MPD_current_cnt)
	li	$3,1			# 0x1
	sw	$3,%lo(MPD_current_cnt)($2)
	lui	$2,%hi(global_MPD_current)
	lw	$2,%lo(global_MPD_current)($2)
	j	$L322
	nop

$L316:
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,136($2)
	andi	$2,$2,0x10
	beq	$2,$0,$L318
	nop

	lui	$2,%hi(jiffies)
	lw	$16,%lo(jiffies)($2)
	lui	$2,%hi($LC37)
	addiu	$4,$2,%lo($LC37)
	li	$5,47			# 0x2f
	lui	$2,%hi(strrchr)
	addiu	$2,$2,%lo(strrchr)
	jalr	$2
	nop

	addiu	$3,$2,1
	lhu	$2,84($sp)
	sw	$2,16($sp)
	lw	$2,32($sp)
	sw	$2,20($sp)
	lui	$2,%hi($LC131)
	addiu	$4,$2,%lo($LC131)
	move	$5,$16
	move	$6,$3
	li	$7,2873			# 0xb39
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L318:
	lw	$3,32($sp)
	lui	$2,%hi(global_MPD_current)
	sw	$3,%lo(global_MPD_current)($2)
	lui	$2,%hi(global_MPD_current)
	lw	$2,%lo(global_MPD_current)($2)
	j	$L322
	nop

$L312:
	lhu	$3,84($sp)
	lw	$2,48($sp)
	slt	$2,$2,$3
	beq	$2,$0,$L319
	nop

	lui	$2,%hi(global_MPD_current_var)
	lw	$2,%lo(global_MPD_current_var)($2)
	bne	$2,$0,$L319
	nop

	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,136($2)
	andi	$2,$2,0x10
	beq	$2,$0,$L320
	nop

	lui	$2,%hi(jiffies)
	lw	$16,%lo(jiffies)($2)
	lui	$2,%hi($LC37)
	addiu	$4,$2,%lo($LC37)
	li	$5,47			# 0x2f
	lui	$2,%hi(strrchr)
	addiu	$2,$2,%lo(strrchr)
	jalr	$2
	nop

	addiu	$3,$2,1
	lhu	$2,84($sp)
	sw	$2,16($sp)
	lw	$2,64($sp)
	sw	$2,20($sp)
	lui	$2,%hi($LC132)
	addiu	$4,$2,%lo($LC132)
	move	$5,$16
	move	$6,$3
	li	$7,2880			# 0xb40
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L320:
	lui	$2,%hi(global_MPD_current)
	lw	$3,64($sp)
	sw	$3,%lo(global_MPD_current)($2)
	lui	$2,%hi(global_MPD_current)
	lw	$2,%lo(global_MPD_current)($2)
	j	$L322
	nop

$L319:
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,136($2)
	andi	$2,$2,0x10
	beq	$2,$0,$L321
	nop

	lui	$2,%hi(jiffies)
	lw	$16,%lo(jiffies)($2)
	lui	$2,%hi($LC37)
	addiu	$4,$2,%lo($LC37)
	li	$5,47			# 0x2f
	lui	$2,%hi(strrchr)
	addiu	$2,$2,%lo(strrchr)
	jalr	$2
	nop

	addiu	$3,$2,1
	lhu	$2,84($sp)
	sw	$2,16($sp)
	lw	$2,32($sp)
	sw	$2,20($sp)
	lui	$2,%hi($LC133)
	addiu	$4,$2,%lo($LC133)
	move	$5,$16
	move	$6,$3
	li	$7,2886			# 0xb46
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L321:
	lw	$3,32($sp)
	lui	$2,%hi(global_MPD_current)
	sw	$3,%lo(global_MPD_current)($2)
	lui	$2,%hi(global_MPD_current)
	lw	$2,%lo(global_MPD_current)($2)
	j	$L322
	nop

$L304:
	lw	$3,32($sp)
	lui	$2,%hi(global_MPD_current)
	sw	$3,%lo(global_MPD_current)($2)
	lui	$2,%hi(global_MPD_current)
	lw	$2,%lo(global_MPD_current)($2)
$L322:
	lw	$31,100($sp)
	lw	$16,96($sp)
	addiu	$sp,$sp,104
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	mt7570_MPD_current
	.size	mt7570_MPD_current, .-mt7570_MPD_current
	.globl	__eqsf2
	.rdata
	.align	2
$LC134:
	.ascii	"[%lu]0ms[%s:%d]\012  Invalid DDMI Rx power-RSSI mapping "
	.ascii	"curve \012\000"
	.align	2
$LC137:
	.ascii	"[%lu]0ms[%s:%d]\012  x *10^12 = %d, y*10^6 = %d, z*10 = "
	.ascii	"%d\012\000"
	.align	2
$LC138:
	.ascii	"[%lu]0ms[%s:%d]\012  Rx power in zone 1\012\000"
	.globl	__nesf2
	.align	2
$LC139:
	.ascii	"[%lu]0ms[%s:%d]\012  Invalid Rx power point\012\000"
	.align	2
$LC140:
	.ascii	"[%lu]0ms[%s:%d]\012  Rx power in zone 2\012\000"
	.align	2
$LC141:
	.ascii	"[%lu]0ms[%s:%d]\012  Rx power in zone 3\012\000"
	.align	2
$LC142:
	.ascii	"[%lu]0ms[%s:%d]\012  Rx_power_8472 = %d\012\000"
	.section	.text.mt7570_RxPower_get_8472,"ax",@progbits
	.align	2
	.globl	mt7570_RxPower_get_8472
	.set	nomips16
	.set	nomicromips
	.ent	mt7570_RxPower_get_8472
	.type	mt7570_RxPower_get_8472, @function
mt7570_RxPower_get_8472:
	.frame	$sp,128,$31		# vars= 72, regs= 5/0, args= 32, gp= 0
	.mask	0x800f0000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-128
	sw	$31,124($sp)
	sw	$19,120($sp)
	sw	$18,116($sp)
	sw	$17,112($sp)
	sw	$16,108($sp)
	sw	$0,60($sp)
	sw	$0,64($sp)
	sw	$0,32($sp)
	sh	$0,68($sp)
	move	$2,$0
	sw	$2,72($sp)
	move	$2,$0
	sw	$2,76($sp)
	sw	$0,36($sp)
	sw	$0,40($sp)
	sw	$0,44($sp)
	sw	$0,48($sp)
	sw	$0,52($sp)
	sw	$0,56($sp)
	li	$4,92			# 0x5c
	lui	$2,%hi(get_flash_register)
	addiu	$2,$2,%lo(get_flash_register)
	jalr	$2
	nop

	andi	$3,$2,0xf
	li	$2,1			# 0x1
	bne	$3,$2,$L324
	nop

	lui	$2,%hi(global_DDMI_rx_cal_flag)
	lw	$2,%lo(global_DDMI_rx_cal_flag)($2)
	bne	$2,$0,$L325
	nop

	li	$4,80			# 0x50
	lui	$2,%hi(get_flash_register)
	addiu	$2,$2,%lo(get_flash_register)
	jalr	$2
	nop

	sw	$2,60($sp)
	lw	$2,60($sp)
	andi	$3,$2,0xffff
	lui	$2,%hi(__floatsisf)
	addiu	$2,$2,%lo(__floatsisf)
	move	$4,$3
	jalr	$2
	nop

	sw	$2,80($sp)
	lw	$2,60($sp)
	srl	$3,$2,16
	lui	$2,%hi(__floatunsisf)
	addiu	$2,$2,%lo(__floatunsisf)
	move	$4,$3
	jalr	$2
	nop

	sw	$2,84($sp)
	li	$4,84			# 0x54
	lui	$2,%hi(get_flash_register)
	addiu	$2,$2,%lo(get_flash_register)
	jalr	$2
	nop

	sw	$2,60($sp)
	lw	$2,60($sp)
	andi	$3,$2,0xffff
	lui	$2,%hi(__floatsisf)
	addiu	$2,$2,%lo(__floatsisf)
	move	$4,$3
	jalr	$2
	nop

	sw	$2,88($sp)
	lw	$2,60($sp)
	srl	$3,$2,16
	lui	$2,%hi(__floatunsisf)
	addiu	$2,$2,%lo(__floatunsisf)
	move	$4,$3
	jalr	$2
	nop

	sw	$2,92($sp)
	li	$4,88			# 0x58
	lui	$2,%hi(get_flash_register)
	addiu	$2,$2,%lo(get_flash_register)
	jalr	$2
	nop

	sw	$2,60($sp)
	lw	$2,60($sp)
	andi	$3,$2,0xffff
	lui	$2,%hi(__floatsisf)
	addiu	$2,$2,%lo(__floatsisf)
	move	$4,$3
	jalr	$2
	nop

	sw	$2,96($sp)
	lw	$2,60($sp)
	srl	$3,$2,16
	lui	$2,%hi(__floatunsisf)
	addiu	$2,$2,%lo(__floatunsisf)
	move	$4,$3
	jalr	$2
	nop

	sw	$2,100($sp)
	lui	$2,%hi(__mulsf3)
	addiu	$2,$2,%lo(__mulsf3)
	lw	$4,80($sp)
	lw	$5,80($sp)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(__mulsf3)
	addiu	$2,$2,%lo(__mulsf3)
	move	$4,$3
	lw	$5,88($sp)
	jalr	$2
	nop

	move	$16,$2
	lui	$2,%hi(__mulsf3)
	addiu	$2,$2,%lo(__mulsf3)
	lw	$4,80($sp)
	lw	$5,96($sp)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(__mulsf3)
	addiu	$2,$2,%lo(__mulsf3)
	move	$4,$3
	lw	$5,96($sp)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(__addsf3)
	addiu	$2,$2,%lo(__addsf3)
	move	$4,$16
	move	$5,$3
	jalr	$2
	nop

	move	$16,$2
	lui	$2,%hi(__mulsf3)
	addiu	$2,$2,%lo(__mulsf3)
	lw	$4,88($sp)
	lw	$5,88($sp)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(__mulsf3)
	addiu	$2,$2,%lo(__mulsf3)
	move	$4,$3
	lw	$5,96($sp)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(__addsf3)
	addiu	$2,$2,%lo(__addsf3)
	move	$4,$16
	move	$5,$3
	jalr	$2
	nop

	move	$16,$2
	lui	$2,%hi(__mulsf3)
	addiu	$2,$2,%lo(__mulsf3)
	lw	$4,88($sp)
	lw	$5,96($sp)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(__mulsf3)
	addiu	$2,$2,%lo(__mulsf3)
	move	$4,$3
	lw	$5,96($sp)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(__subsf3)
	addiu	$2,$2,%lo(__subsf3)
	move	$4,$16
	move	$5,$3
	jalr	$2
	nop

	move	$16,$2
	lui	$2,%hi(__mulsf3)
	addiu	$2,$2,%lo(__mulsf3)
	lw	$4,80($sp)
	lw	$5,88($sp)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(__mulsf3)
	addiu	$2,$2,%lo(__mulsf3)
	move	$4,$3
	lw	$5,88($sp)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(__subsf3)
	addiu	$2,$2,%lo(__subsf3)
	move	$4,$16
	move	$5,$3
	jalr	$2
	nop

	move	$16,$2
	lui	$2,%hi(__mulsf3)
	addiu	$2,$2,%lo(__mulsf3)
	lw	$4,80($sp)
	lw	$5,80($sp)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(__mulsf3)
	addiu	$2,$2,%lo(__mulsf3)
	move	$4,$3
	lw	$5,96($sp)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(__subsf3)
	addiu	$2,$2,%lo(__subsf3)
	move	$4,$16
	move	$5,$3
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(d.25458)
	sw	$3,%lo(d.25458)($2)
	lui	$2,%hi(__mulsf3)
	addiu	$2,$2,%lo(__mulsf3)
	lw	$4,84($sp)
	lw	$5,88($sp)
	jalr	$2
	nop

	move	$16,$2
	lui	$2,%hi(__mulsf3)
	addiu	$2,$2,%lo(__mulsf3)
	lw	$4,80($sp)
	lw	$5,100($sp)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(__addsf3)
	addiu	$2,$2,%lo(__addsf3)
	move	$4,$16
	move	$5,$3
	jalr	$2
	nop

	move	$16,$2
	lui	$2,%hi(__mulsf3)
	addiu	$2,$2,%lo(__mulsf3)
	lw	$4,92($sp)
	lw	$5,96($sp)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(__addsf3)
	addiu	$2,$2,%lo(__addsf3)
	move	$4,$16
	move	$5,$3
	jalr	$2
	nop

	move	$16,$2
	lui	$2,%hi(__mulsf3)
	addiu	$2,$2,%lo(__mulsf3)
	lw	$4,88($sp)
	lw	$5,100($sp)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(__subsf3)
	addiu	$2,$2,%lo(__subsf3)
	move	$4,$16
	move	$5,$3
	jalr	$2
	nop

	move	$16,$2
	lui	$2,%hi(__mulsf3)
	addiu	$2,$2,%lo(__mulsf3)
	lw	$4,80($sp)
	lw	$5,92($sp)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(__subsf3)
	addiu	$2,$2,%lo(__subsf3)
	move	$4,$16
	move	$5,$3
	jalr	$2
	nop

	move	$16,$2
	lui	$2,%hi(__mulsf3)
	addiu	$2,$2,%lo(__mulsf3)
	lw	$4,84($sp)
	lw	$5,96($sp)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(__subsf3)
	addiu	$2,$2,%lo(__subsf3)
	move	$4,$16
	move	$5,$3
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(e.25459)
	sw	$3,%lo(e.25459)($2)
	lui	$2,%hi(__mulsf3)
	addiu	$2,$2,%lo(__mulsf3)
	lw	$4,80($sp)
	lw	$5,80($sp)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(__mulsf3)
	addiu	$2,$2,%lo(__mulsf3)
	move	$4,$3
	lw	$5,92($sp)
	jalr	$2
	nop

	move	$16,$2
	lui	$2,%hi(__mulsf3)
	addiu	$2,$2,%lo(__mulsf3)
	lw	$4,84($sp)
	lw	$5,96($sp)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(__mulsf3)
	addiu	$2,$2,%lo(__mulsf3)
	move	$4,$3
	lw	$5,96($sp)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(__addsf3)
	addiu	$2,$2,%lo(__addsf3)
	move	$4,$16
	move	$5,$3
	jalr	$2
	nop

	move	$16,$2
	lui	$2,%hi(__mulsf3)
	addiu	$2,$2,%lo(__mulsf3)
	lw	$4,88($sp)
	lw	$5,88($sp)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(__mulsf3)
	addiu	$2,$2,%lo(__mulsf3)
	move	$4,$3
	lw	$5,100($sp)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(__addsf3)
	addiu	$2,$2,%lo(__addsf3)
	move	$4,$16
	move	$5,$3
	jalr	$2
	nop

	move	$16,$2
	lui	$2,%hi(__mulsf3)
	addiu	$2,$2,%lo(__mulsf3)
	lw	$4,92($sp)
	lw	$5,96($sp)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(__mulsf3)
	addiu	$2,$2,%lo(__mulsf3)
	move	$4,$3
	lw	$5,96($sp)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(__subsf3)
	addiu	$2,$2,%lo(__subsf3)
	move	$4,$16
	move	$5,$3
	jalr	$2
	nop

	move	$16,$2
	lui	$2,%hi(__mulsf3)
	addiu	$2,$2,%lo(__mulsf3)
	lw	$4,84($sp)
	lw	$5,88($sp)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(__mulsf3)
	addiu	$2,$2,%lo(__mulsf3)
	move	$4,$3
	lw	$5,88($sp)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(__subsf3)
	addiu	$2,$2,%lo(__subsf3)
	move	$4,$16
	move	$5,$3
	jalr	$2
	nop

	move	$16,$2
	lui	$2,%hi(__mulsf3)
	addiu	$2,$2,%lo(__mulsf3)
	lw	$4,80($sp)
	lw	$5,80($sp)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(__mulsf3)
	addiu	$2,$2,%lo(__mulsf3)
	move	$4,$3
	lw	$5,100($sp)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(__subsf3)
	addiu	$2,$2,%lo(__subsf3)
	move	$4,$16
	move	$5,$3
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(f.25460)
	sw	$3,%lo(f.25460)($2)
	lui	$2,%hi(__mulsf3)
	addiu	$2,$2,%lo(__mulsf3)
	lw	$4,80($sp)
	lw	$5,80($sp)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(__mulsf3)
	addiu	$2,$2,%lo(__mulsf3)
	move	$4,$3
	lw	$5,88($sp)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(__mulsf3)
	addiu	$2,$2,%lo(__mulsf3)
	move	$4,$3
	lw	$5,100($sp)
	jalr	$2
	nop

	move	$16,$2
	lui	$2,%hi(__mulsf3)
	addiu	$2,$2,%lo(__mulsf3)
	lw	$4,80($sp)
	lw	$5,92($sp)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(__mulsf3)
	addiu	$2,$2,%lo(__mulsf3)
	move	$4,$3
	lw	$5,96($sp)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(__mulsf3)
	addiu	$2,$2,%lo(__mulsf3)
	move	$4,$3
	lw	$5,96($sp)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(__addsf3)
	addiu	$2,$2,%lo(__addsf3)
	move	$4,$16
	move	$5,$3
	jalr	$2
	nop

	move	$16,$2
	lui	$2,%hi(__mulsf3)
	addiu	$2,$2,%lo(__mulsf3)
	lw	$4,84($sp)
	lw	$5,88($sp)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(__mulsf3)
	addiu	$2,$2,%lo(__mulsf3)
	move	$4,$3
	lw	$5,88($sp)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(__mulsf3)
	addiu	$2,$2,%lo(__mulsf3)
	move	$4,$3
	lw	$5,96($sp)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(__addsf3)
	addiu	$2,$2,%lo(__addsf3)
	move	$4,$16
	move	$5,$3
	jalr	$2
	nop

	move	$16,$2
	lui	$2,%hi(__mulsf3)
	addiu	$2,$2,%lo(__mulsf3)
	lw	$4,84($sp)
	lw	$5,88($sp)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(__mulsf3)
	addiu	$2,$2,%lo(__mulsf3)
	move	$4,$3
	lw	$5,96($sp)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(__mulsf3)
	addiu	$2,$2,%lo(__mulsf3)
	move	$4,$3
	lw	$5,96($sp)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(__subsf3)
	addiu	$2,$2,%lo(__subsf3)
	move	$4,$16
	move	$5,$3
	jalr	$2
	nop

	move	$16,$2
	lui	$2,%hi(__mulsf3)
	addiu	$2,$2,%lo(__mulsf3)
	lw	$4,80($sp)
	lw	$5,88($sp)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(__mulsf3)
	addiu	$2,$2,%lo(__mulsf3)
	move	$4,$3
	lw	$5,88($sp)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(__mulsf3)
	addiu	$2,$2,%lo(__mulsf3)
	move	$4,$3
	lw	$5,100($sp)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(__subsf3)
	addiu	$2,$2,%lo(__subsf3)
	move	$4,$16
	move	$5,$3
	jalr	$2
	nop

	move	$16,$2
	lui	$2,%hi(__mulsf3)
	addiu	$2,$2,%lo(__mulsf3)
	lw	$4,80($sp)
	lw	$5,80($sp)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(__mulsf3)
	addiu	$2,$2,%lo(__mulsf3)
	move	$4,$3
	lw	$5,92($sp)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(__mulsf3)
	addiu	$2,$2,%lo(__mulsf3)
	move	$4,$3
	lw	$5,96($sp)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(__subsf3)
	addiu	$2,$2,%lo(__subsf3)
	move	$4,$16
	move	$5,$3
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(g.25461)
	sw	$3,%lo(g.25461)($2)
	lui	$2,%hi(d.25458)
	lw	$3,%lo(d.25458)($2)
	lui	$2,%hi(__eqsf2)
	addiu	$2,$2,%lo(__eqsf2)
	move	$4,$3
	move	$5,$0
	jalr	$2
	nop

	bne	$2,$0,$L355
	nop

	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,136($2)
	andi	$2,$2,0x10
	beq	$2,$0,$L329
	nop

	lui	$2,%hi(jiffies)
	lw	$16,%lo(jiffies)($2)
	lui	$2,%hi($LC37)
	addiu	$4,$2,%lo($LC37)
	li	$5,47			# 0x2f
	lui	$2,%hi(strrchr)
	addiu	$2,$2,%lo(strrchr)
	jalr	$2
	nop

	addiu	$3,$2,1
	lui	$2,%hi($LC134)
	addiu	$4,$2,%lo($LC134)
	move	$5,$16
	move	$6,$3
	li	$7,2958			# 0xb8e
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	j	$L329
	nop

$L355:
	lui	$2,%hi(e.25459)
	lw	$4,%lo(e.25459)($2)
	lui	$2,%hi(d.25458)
	lw	$3,%lo(d.25458)($2)
	lui	$2,%hi(__divsf3)
	addiu	$2,$2,%lo(__divsf3)
	move	$5,$3
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(x.25462)
	sw	$3,%lo(x.25462)($2)
	lui	$2,%hi(f.25460)
	lw	$4,%lo(f.25460)($2)
	lui	$2,%hi(d.25458)
	lw	$3,%lo(d.25458)($2)
	lui	$2,%hi(__divsf3)
	addiu	$2,$2,%lo(__divsf3)
	move	$5,$3
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(y.25463)
	sw	$3,%lo(y.25463)($2)
	lui	$2,%hi(g.25461)
	lw	$4,%lo(g.25461)($2)
	lui	$2,%hi(d.25458)
	lw	$3,%lo(d.25458)($2)
	lui	$2,%hi(__divsf3)
	addiu	$2,$2,%lo(__divsf3)
	move	$5,$3
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(z.25464)
	sw	$3,%lo(z.25464)($2)
$L329:
	lui	$2,%hi(global_DDMI_rx_cal_flag)
	li	$3,1			# 0x1
	sw	$3,%lo(global_DDMI_rx_cal_flag)($2)
$L325:
	lui	$2,%hi(mt7570_RSSI_current)
	addiu	$2,$2,%lo(mt7570_RSSI_current)
	jalr	$2
	nop

	sw	$2,64($sp)
	lui	$2,%hi(__floatsisf)
	addiu	$2,$2,%lo(__floatsisf)
	lw	$4,64($sp)
	jalr	$2
	nop

	move	$16,$2
	lui	$2,%hi(__floatsisf)
	addiu	$2,$2,%lo(__floatsisf)
	lw	$4,64($sp)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(__mulsf3)
	addiu	$2,$2,%lo(__mulsf3)
	move	$4,$16
	move	$5,$3
	jalr	$2
	nop

	move	$4,$2
	lui	$2,%hi(x.25462)
	lw	$3,%lo(x.25462)($2)
	lui	$2,%hi(__mulsf3)
	addiu	$2,$2,%lo(__mulsf3)
	move	$5,$3
	jalr	$2
	nop

	move	$16,$2
	lui	$2,%hi(__floatsisf)
	addiu	$2,$2,%lo(__floatsisf)
	lw	$4,64($sp)
	jalr	$2
	nop

	move	$4,$2
	lui	$2,%hi(y.25463)
	lw	$3,%lo(y.25463)($2)
	lui	$2,%hi(__mulsf3)
	addiu	$2,$2,%lo(__mulsf3)
	move	$5,$3
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(__addsf3)
	addiu	$2,$2,%lo(__addsf3)
	move	$4,$16
	move	$5,$3
	jalr	$2
	nop

	move	$4,$2
	lui	$2,%hi(z.25464)
	lw	$3,%lo(z.25464)($2)
	lui	$2,%hi(__addsf3)
	addiu	$2,$2,%lo(__addsf3)
	move	$5,$3
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(__fixsfsi)
	addiu	$2,$2,%lo(__fixsfsi)
	move	$4,$3
	jalr	$2
	nop

	sw	$2,32($sp)
	lw	$2,32($sp)
	bgez	$2,$L330
	nop

	sw	$0,32($sp)
$L330:
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,136($2)
	andi	$2,$2,0x10
	beq	$2,$0,$L332
	nop

	lui	$2,%hi(jiffies)
	lw	$17,%lo(jiffies)($2)
	lui	$2,%hi($LC37)
	addiu	$4,$2,%lo($LC37)
	li	$5,47			# 0x2f
	lui	$2,%hi(strrchr)
	addiu	$2,$2,%lo(strrchr)
	jalr	$2
	nop

	addiu	$16,$2,1
	lui	$2,%hi(x.25462)
	lw	$4,%lo(x.25462)($2)
	lui	$3,%hi($LC135)
	lui	$2,%hi(__mulsf3)
	addiu	$2,$2,%lo(__mulsf3)
	lw	$5,%lo($LC135)($3)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(__fixsfsi)
	addiu	$2,$2,%lo(__fixsfsi)
	move	$4,$3
	jalr	$2
	nop

	move	$19,$2
	lui	$2,%hi(y.25463)
	lw	$4,%lo(y.25463)($2)
	lui	$3,%hi($LC136)
	lui	$2,%hi(__mulsf3)
	addiu	$2,$2,%lo(__mulsf3)
	lw	$5,%lo($LC136)($3)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(__fixsfsi)
	addiu	$2,$2,%lo(__fixsfsi)
	move	$4,$3
	jalr	$2
	nop

	move	$18,$2
	lui	$2,%hi(z.25464)
	lw	$4,%lo(z.25464)($2)
	lui	$3,%hi($LC16)
	lui	$2,%hi(__mulsf3)
	addiu	$2,$2,%lo(__mulsf3)
	lw	$5,%lo($LC16)($3)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(__fixsfsi)
	addiu	$2,$2,%lo(__fixsfsi)
	move	$4,$3
	jalr	$2
	nop

	sw	$19,16($sp)
	sw	$18,20($sp)
	sw	$2,24($sp)
	lui	$2,%hi($LC137)
	addiu	$4,$2,%lo($LC137)
	move	$5,$17
	move	$6,$16
	li	$7,2975			# 0xb9f
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	j	$L332
	nop

$L324:
	lui	$2,%hi(mt7570_RSSI_current)
	addiu	$2,$2,%lo(mt7570_RSSI_current)
	jalr	$2
	nop

	sw	$2,64($sp)
	li	$4,80			# 0x50
	lui	$2,%hi(get_flash_register)
	addiu	$2,$2,%lo(get_flash_register)
	jalr	$2
	nop

	move	$3,$2
	li	$2,-1			# 0xffffffffffffffff
	beq	$3,$2,$L333
	nop

	li	$4,80			# 0x50
	lui	$2,%hi(get_flash_register)
	addiu	$2,$2,%lo(get_flash_register)
	jalr	$2
	nop

	sw	$2,60($sp)
	lw	$2,60($sp)
	andi	$2,$2,0xffff
	sw	$2,36($sp)
	lw	$2,60($sp)
	srl	$2,$2,16
	sw	$2,48($sp)
$L333:
	li	$4,84			# 0x54
	lui	$2,%hi(get_flash_register)
	addiu	$2,$2,%lo(get_flash_register)
	jalr	$2
	nop

	move	$3,$2
	li	$2,-1			# 0xffffffffffffffff
	beq	$3,$2,$L334
	nop

	li	$4,84			# 0x54
	lui	$2,%hi(get_flash_register)
	addiu	$2,$2,%lo(get_flash_register)
	jalr	$2
	nop

	sw	$2,60($sp)
	lw	$2,60($sp)
	andi	$2,$2,0xffff
	sw	$2,40($sp)
	lw	$2,60($sp)
	srl	$2,$2,16
	sw	$2,52($sp)
$L334:
	li	$4,88			# 0x58
	lui	$2,%hi(get_flash_register)
	addiu	$2,$2,%lo(get_flash_register)
	jalr	$2
	nop

	move	$3,$2
	li	$2,-1			# 0xffffffffffffffff
	beq	$3,$2,$L335
	nop

	li	$4,88			# 0x58
	lui	$2,%hi(get_flash_register)
	addiu	$2,$2,%lo(get_flash_register)
	jalr	$2
	nop

	sw	$2,60($sp)
	lw	$2,60($sp)
	andi	$2,$2,0xffff
	sw	$2,44($sp)
	lw	$2,60($sp)
	srl	$2,$2,16
	sw	$2,56($sp)
$L335:
	lw	$3,64($sp)
	lw	$2,44($sp)
	slt	$2,$2,$3
	bne	$2,$0,$L336
	nop

	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,136($2)
	andi	$2,$2,0x10
	beq	$2,$0,$L337
	nop

	lui	$2,%hi(jiffies)
	lw	$16,%lo(jiffies)($2)
	lui	$2,%hi($LC37)
	addiu	$4,$2,%lo($LC37)
	li	$5,47			# 0x2f
	lui	$2,%hi(strrchr)
	addiu	$2,$2,%lo(strrchr)
	jalr	$2
	nop

	addiu	$3,$2,1
	lui	$2,%hi($LC138)
	addiu	$4,$2,%lo($LC138)
	move	$5,$16
	move	$6,$3
	li	$7,3002			# 0xbba
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L337:
	lui	$2,%hi(__floatsisf)
	addiu	$2,$2,%lo(__floatsisf)
	lw	$4,44($sp)
	jalr	$2
	nop

	sw	$2,72($sp)
	lui	$2,%hi(__nesf2)
	addiu	$2,$2,%lo(__nesf2)
	lw	$4,72($sp)
	move	$5,$0
	jalr	$2
	nop

	beq	$2,$0,$L356
	nop

	lui	$2,%hi(__floatsisf)
	addiu	$2,$2,%lo(__floatsisf)
	lw	$4,56($sp)
	jalr	$2
	nop

	move	$16,$2
	lui	$2,%hi(__floatsisf)
	addiu	$2,$2,%lo(__floatsisf)
	lw	$4,44($sp)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(__divsf3)
	addiu	$2,$2,%lo(__divsf3)
	move	$4,$16
	move	$5,$3
	jalr	$2
	nop

	sw	$2,76($sp)
	lui	$2,%hi(__floatsisf)
	addiu	$2,$2,%lo(__floatsisf)
	lw	$4,64($sp)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(__mulsf3)
	addiu	$2,$2,%lo(__mulsf3)
	move	$4,$3
	lw	$5,76($sp)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(__fixsfsi)
	addiu	$2,$2,%lo(__fixsfsi)
	move	$4,$3
	jalr	$2
	nop

	sw	$2,32($sp)
	j	$L332
	nop

$L356:
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,136($2)
	andi	$2,$2,0x10
	beq	$2,$0,$L332
	nop

	lui	$2,%hi(jiffies)
	lw	$16,%lo(jiffies)($2)
	lui	$2,%hi($LC37)
	addiu	$4,$2,%lo($LC37)
	li	$5,47			# 0x2f
	lui	$2,%hi(strrchr)
	addiu	$2,$2,%lo(strrchr)
	jalr	$2
	nop

	addiu	$3,$2,1
	lui	$2,%hi($LC139)
	addiu	$4,$2,%lo($LC139)
	move	$5,$16
	move	$6,$3
	li	$7,3010			# 0xbc2
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	j	$L332
	nop

$L336:
	lw	$3,44($sp)
	lw	$2,64($sp)
	slt	$2,$3,$2
	beq	$2,$0,$L341
	nop

	lw	$3,64($sp)
	lw	$2,40($sp)
	slt	$2,$3,$2
	beq	$2,$0,$L341
	nop

	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,136($2)
	andi	$2,$2,0x10
	beq	$2,$0,$L342
	nop

	lui	$2,%hi(jiffies)
	lw	$16,%lo(jiffies)($2)
	lui	$2,%hi($LC37)
	addiu	$4,$2,%lo($LC37)
	li	$5,47			# 0x2f
	lui	$2,%hi(strrchr)
	addiu	$2,$2,%lo(strrchr)
	jalr	$2
	nop

	addiu	$3,$2,1
	lui	$2,%hi($LC140)
	addiu	$4,$2,%lo($LC140)
	move	$5,$16
	move	$6,$3
	li	$7,3014			# 0xbc6
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L342:
	lw	$3,40($sp)
	lw	$2,44($sp)
	subu	$3,$3,$2
	lui	$2,%hi(__floatsisf)
	addiu	$2,$2,%lo(__floatsisf)
	move	$4,$3
	jalr	$2
	nop

	sw	$2,72($sp)
	lui	$2,%hi(__nesf2)
	addiu	$2,$2,%lo(__nesf2)
	lw	$4,72($sp)
	move	$5,$0
	jalr	$2
	nop

	beq	$2,$0,$L357
	nop

	lw	$3,52($sp)
	lw	$2,56($sp)
	subu	$3,$3,$2
	lui	$2,%hi(__floatsisf)
	addiu	$2,$2,%lo(__floatsisf)
	move	$4,$3
	jalr	$2
	nop

	move	$16,$2
	lw	$3,40($sp)
	lw	$2,44($sp)
	subu	$3,$3,$2
	lui	$2,%hi(__floatsisf)
	addiu	$2,$2,%lo(__floatsisf)
	move	$4,$3
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(__divsf3)
	addiu	$2,$2,%lo(__divsf3)
	move	$4,$16
	move	$5,$3
	jalr	$2
	nop

	sw	$2,76($sp)
	lw	$3,64($sp)
	lw	$2,44($sp)
	subu	$3,$3,$2
	lui	$2,%hi(__floatsisf)
	addiu	$2,$2,%lo(__floatsisf)
	move	$4,$3
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(__mulsf3)
	addiu	$2,$2,%lo(__mulsf3)
	move	$4,$3
	lw	$5,76($sp)
	jalr	$2
	nop

	move	$16,$2
	lui	$2,%hi(__floatsisf)
	addiu	$2,$2,%lo(__floatsisf)
	lw	$4,56($sp)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(__addsf3)
	addiu	$2,$2,%lo(__addsf3)
	move	$4,$16
	move	$5,$3
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(__fixsfsi)
	addiu	$2,$2,%lo(__fixsfsi)
	move	$4,$3
	jalr	$2
	nop

	sw	$2,32($sp)
	j	$L345
	nop

$L357:
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,136($2)
	andi	$2,$2,0x10
	beq	$2,$0,$L345
	nop

	lui	$2,%hi(jiffies)
	lw	$16,%lo(jiffies)($2)
	lui	$2,%hi($LC37)
	addiu	$4,$2,%lo($LC37)
	li	$5,47			# 0x2f
	lui	$2,%hi(strrchr)
	addiu	$2,$2,%lo(strrchr)
	jalr	$2
	nop

	addiu	$3,$2,1
	lui	$2,%hi($LC139)
	addiu	$4,$2,%lo($LC139)
	move	$5,$16
	move	$6,$3
	li	$7,3022			# 0xbce
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	j	$L332
	nop

$L345:
	j	$L332
	nop

$L341:
	lw	$3,40($sp)
	lw	$2,64($sp)
	slt	$2,$2,$3
	bne	$2,$0,$L332
	nop

	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,136($2)
	andi	$2,$2,0x10
	beq	$2,$0,$L346
	nop

	lui	$2,%hi(jiffies)
	lw	$16,%lo(jiffies)($2)
	lui	$2,%hi($LC37)
	addiu	$4,$2,%lo($LC37)
	li	$5,47			# 0x2f
	lui	$2,%hi(strrchr)
	addiu	$2,$2,%lo(strrchr)
	jalr	$2
	nop

	addiu	$3,$2,1
	lui	$2,%hi($LC141)
	addiu	$4,$2,%lo($LC141)
	move	$5,$16
	move	$6,$3
	li	$7,3026			# 0xbd2
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L346:
	lw	$3,36($sp)
	lw	$2,40($sp)
	subu	$3,$3,$2
	lui	$2,%hi(__floatsisf)
	addiu	$2,$2,%lo(__floatsisf)
	move	$4,$3
	jalr	$2
	nop

	sw	$2,72($sp)
	lui	$2,%hi(__nesf2)
	addiu	$2,$2,%lo(__nesf2)
	lw	$4,72($sp)
	move	$5,$0
	jalr	$2
	nop

	beq	$2,$0,$L358
	nop

	lw	$3,48($sp)
	lw	$2,52($sp)
	subu	$3,$3,$2
	lui	$2,%hi(__floatsisf)
	addiu	$2,$2,%lo(__floatsisf)
	move	$4,$3
	jalr	$2
	nop

	move	$16,$2
	lw	$3,36($sp)
	lw	$2,40($sp)
	subu	$3,$3,$2
	lui	$2,%hi(__floatsisf)
	addiu	$2,$2,%lo(__floatsisf)
	move	$4,$3
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(__divsf3)
	addiu	$2,$2,%lo(__divsf3)
	move	$4,$16
	move	$5,$3
	jalr	$2
	nop

	sw	$2,76($sp)
	lw	$3,64($sp)
	lw	$2,40($sp)
	subu	$3,$3,$2
	lui	$2,%hi(__floatsisf)
	addiu	$2,$2,%lo(__floatsisf)
	move	$4,$3
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(__mulsf3)
	addiu	$2,$2,%lo(__mulsf3)
	move	$4,$3
	lw	$5,76($sp)
	jalr	$2
	nop

	move	$16,$2
	lui	$2,%hi(__floatsisf)
	addiu	$2,$2,%lo(__floatsisf)
	lw	$4,52($sp)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(__addsf3)
	addiu	$2,$2,%lo(__addsf3)
	move	$4,$16
	move	$5,$3
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(__fixsfsi)
	addiu	$2,$2,%lo(__fixsfsi)
	move	$4,$3
	jalr	$2
	nop

	sw	$2,32($sp)
	j	$L332
	nop

$L358:
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,136($2)
	andi	$2,$2,0x10
	beq	$2,$0,$L332
	nop

	lui	$2,%hi(jiffies)
	lw	$16,%lo(jiffies)($2)
	lui	$2,%hi($LC37)
	addiu	$4,$2,%lo($LC37)
	li	$5,47			# 0x2f
	lui	$2,%hi(strrchr)
	addiu	$2,$2,%lo(strrchr)
	jalr	$2
	nop

	addiu	$3,$2,1
	lui	$2,%hi($LC139)
	addiu	$4,$2,%lo($LC139)
	move	$5,$16
	move	$6,$3
	li	$7,3034			# 0xbda
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L332:
	lw	$2,32($sp)
	li	$3,1717960704			# 0x66660000
	ori	$3,$3,0x6667
	mult	$2,$3
	mfhi	$3
	sra	$3,$3,2
	sra	$2,$2,31
	subu	$2,$3,$2
	sh	$2,68($sp)
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,136($2)
	andi	$2,$2,0x10
	beq	$2,$0,$L349
	nop

	lui	$2,%hi(jiffies)
	lw	$16,%lo(jiffies)($2)
	lui	$2,%hi($LC37)
	addiu	$4,$2,%lo($LC37)
	li	$5,47			# 0x2f
	lui	$2,%hi(strrchr)
	addiu	$2,$2,%lo(strrchr)
	jalr	$2
	nop

	addiu	$3,$2,1
	lhu	$2,68($sp)
	sw	$2,16($sp)
	lui	$2,%hi($LC142)
	addiu	$4,$2,%lo($LC142)
	move	$5,$16
	move	$6,$3
	li	$7,3040			# 0xbe0
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L349:
	lhu	$2,68($sp)
	lw	$31,124($sp)
	lw	$19,120($sp)
	lw	$18,116($sp)
	lw	$17,112($sp)
	lw	$16,108($sp)
	addiu	$sp,$sp,128
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	mt7570_RxPower_get_8472
	.size	mt7570_RxPower_get_8472, .-mt7570_RxPower_get_8472
	.section	.text.mt7570_safe_circuit_reset,"ax",@progbits
	.align	2
	.globl	mt7570_safe_circuit_reset
	.set	nomips16
	.set	nomicromips
	.ent	mt7570_safe_circuit_reset
	.type	mt7570_safe_circuit_reset, @function
mt7570_safe_circuit_reset:
	.frame	$sp,48,$31		# vars= 8, regs= 1/0, args= 32, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-48
	sw	$31,44($sp)
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,116($2)
	move	$3,$2
	li	$2,256			# 0x100
	sw	$2,16($sp)
	addiu	$2,$sp,32
	sw	$2,20($sp)
	li	$2,4			# 0x4
	sw	$2,24($sp)
	move	$4,$0
	move	$5,$3
	li	$6,112			# 0x70
	li	$7,2			# 0x2
	lui	$2,%hi(phy_I2C_read)
	addiu	$2,$2,%lo(phy_I2C_read)
	jalr	$2
	nop

	lbu	$2,33($sp)
	ori	$2,$2,0x40
	andi	$2,$2,0x00ff
	sb	$2,33($sp)
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,116($2)
	move	$3,$2
	li	$2,256			# 0x100
	sw	$2,16($sp)
	addiu	$2,$sp,32
	sw	$2,20($sp)
	li	$2,4			# 0x4
	sw	$2,24($sp)
	move	$4,$0
	move	$5,$3
	li	$6,112			# 0x70
	li	$7,2			# 0x2
	lui	$2,%hi(phy_I2C_write)
	addiu	$2,$2,%lo(phy_I2C_write)
	jalr	$2
	nop

	lw	$31,44($sp)
	addiu	$sp,$sp,48
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	mt7570_safe_circuit_reset
	.size	mt7570_safe_circuit_reset, .-mt7570_safe_circuit_reset
	.rdata
	.align	2
$LC143:
	.ascii	"[%lu]0ms[%s:%d]\012  MPD_current = 0x%x\012\000"
	.align	2
$LC144:
	.ascii	"[%lu]0ms[%s:%d]\012  Invalid Tx power point\012\000"
	.align	2
$LC145:
	.ascii	"[%lu]0ms[%s:%d]\012  Tx_power_8472 = %d\012\000"
	.section	.text.mt7570_TxPower_get_8472,"ax",@progbits
	.align	2
	.globl	mt7570_TxPower_get_8472
	.set	nomips16
	.set	nomicromips
	.ent	mt7570_TxPower_get_8472
	.type	mt7570_TxPower_get_8472, @function
mt7570_TxPower_get_8472:
	.frame	$sp,72,$31		# vars= 40, regs= 2/0, args= 24, gp= 0
	.mask	0x80010000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-72
	sw	$31,68($sp)
	sw	$16,64($sp)
	sw	$0,44($sp)
	sw	$0,48($sp)
	sw	$0,52($sp)
	sw	$0,24($sp)
	lui	$2,%hi(MPD_current_offset)
	lw	$2,%lo(MPD_current_offset)($2)
	sw	$2,28($sp)
	move	$2,$0
	sw	$2,56($sp)
	move	$2,$0
	sw	$2,32($sp)
	sw	$0,36($sp)
	sw	$0,40($sp)
	sh	$0,60($sp)
	lui	$2,%hi(mt7570_MPD_current)
	addiu	$2,$2,%lo(mt7570_MPD_current)
	jalr	$2
	nop

	sw	$2,52($sp)
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,136($2)
	andi	$2,$2,0x10
	beq	$2,$0,$L361
	nop

	lui	$2,%hi(jiffies)
	lw	$16,%lo(jiffies)($2)
	lui	$2,%hi($LC37)
	addiu	$4,$2,%lo($LC37)
	li	$5,47			# 0x2f
	lui	$2,%hi(strrchr)
	addiu	$2,$2,%lo(strrchr)
	jalr	$2
	nop

	addiu	$3,$2,1
	lw	$2,52($sp)
	sw	$2,16($sp)
	lui	$2,%hi($LC143)
	addiu	$4,$2,%lo($LC143)
	move	$5,$16
	move	$6,$3
	li	$7,3092			# 0xc14
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L361:
	li	$4,64			# 0x40
	lui	$2,%hi(get_flash_register)
	addiu	$2,$2,%lo(get_flash_register)
	jalr	$2
	nop

	move	$3,$2
	li	$2,-1			# 0xffffffffffffffff
	beq	$3,$2,$L362
	nop

	li	$4,64			# 0x40
	lui	$2,%hi(get_flash_register)
	addiu	$2,$2,%lo(get_flash_register)
	jalr	$2
	nop

	sw	$2,48($sp)
	lw	$2,48($sp)
	andi	$2,$2,0xffff
	sw	$2,24($sp)
	lw	$2,48($sp)
	srl	$2,$2,16
	sw	$2,36($sp)
$L362:
	li	$4,68			# 0x44
	lui	$2,%hi(get_flash_register)
	addiu	$2,$2,%lo(get_flash_register)
	jalr	$2
	nop

	move	$3,$2
	li	$2,-1			# 0xffffffffffffffff
	beq	$3,$2,$L363
	nop

	li	$4,68			# 0x44
	lui	$2,%hi(get_flash_register)
	addiu	$2,$2,%lo(get_flash_register)
	jalr	$2
	nop

	sw	$2,48($sp)
	lw	$2,48($sp)
	andi	$2,$2,0xffff
	sw	$2,28($sp)
	lw	$2,48($sp)
	srl	$2,$2,16
	sw	$2,40($sp)
$L363:
	lw	$3,24($sp)
	lw	$2,28($sp)
	subu	$2,$3,$2
	sw	$2,44($sp)
	lw	$2,44($sp)
	beq	$2,$0,$L364
	nop

	lw	$3,36($sp)
	lw	$2,40($sp)
	subu	$3,$3,$2
	lui	$2,%hi(__floatsisf)
	addiu	$2,$2,%lo(__floatsisf)
	move	$4,$3
	jalr	$2
	nop

	move	$16,$2
	lw	$3,24($sp)
	lw	$2,28($sp)
	subu	$3,$3,$2
	lui	$2,%hi(__floatsisf)
	addiu	$2,$2,%lo(__floatsisf)
	move	$4,$3
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(__divsf3)
	addiu	$2,$2,%lo(__divsf3)
	move	$4,$16
	move	$5,$3
	jalr	$2
	nop

	sw	$2,56($sp)
	lw	$3,52($sp)
	lw	$2,28($sp)
	subu	$3,$3,$2
	lui	$2,%hi(__floatsisf)
	addiu	$2,$2,%lo(__floatsisf)
	move	$4,$3
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(__mulsf3)
	addiu	$2,$2,%lo(__mulsf3)
	move	$4,$3
	lw	$5,56($sp)
	jalr	$2
	nop

	move	$16,$2
	lui	$2,%hi(__floatsisf)
	addiu	$2,$2,%lo(__floatsisf)
	lw	$4,40($sp)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(__addsf3)
	addiu	$2,$2,%lo(__addsf3)
	move	$4,$16
	move	$5,$3
	jalr	$2
	nop

	sw	$2,32($sp)
	j	$L365
	nop

$L364:
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,136($2)
	andi	$2,$2,0x10
	beq	$2,$0,$L365
	nop

	lui	$2,%hi(jiffies)
	lw	$16,%lo(jiffies)($2)
	lui	$2,%hi($LC37)
	addiu	$4,$2,%lo($LC37)
	li	$5,47			# 0x2f
	lui	$2,%hi(strrchr)
	addiu	$2,$2,%lo(strrchr)
	jalr	$2
	nop

	addiu	$3,$2,1
	lui	$2,%hi($LC144)
	addiu	$4,$2,%lo($LC144)
	move	$5,$16
	move	$6,$3
	li	$7,3113			# 0xc29
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L365:
	lui	$3,%hi($LC16)
	lui	$2,%hi(__mulsf3)
	addiu	$2,$2,%lo(__mulsf3)
	lw	$4,32($sp)
	lw	$5,%lo($LC16)($3)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(__fixsfsi)
	addiu	$2,$2,%lo(__fixsfsi)
	move	$4,$3
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(__floatsidf)
	addiu	$2,$2,%lo(__floatsidf)
	move	$4,$3
	jalr	$2
	nop

	move	$5,$3
	move	$4,$2
	lui	$3,%hi($LC126)
	lui	$2,%hi(__divdf3)
	addiu	$2,$2,%lo(__divdf3)
	lw	$7,%lo($LC126+4)($3)
	lw	$6,%lo($LC126)($3)
	jalr	$2
	nop

	move	$5,$3
	move	$4,$2
	lui	$2,%hi(__fixunsdfsi)
	addiu	$2,$2,%lo(__fixunsdfsi)
	jalr	$2
	nop

	sh	$2,60($sp)
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,136($2)
	andi	$2,$2,0x10
	beq	$2,$0,$L366
	nop

	lui	$2,%hi(jiffies)
	lw	$16,%lo(jiffies)($2)
	lui	$2,%hi($LC37)
	addiu	$4,$2,%lo($LC37)
	li	$5,47			# 0x2f
	lui	$2,%hi(strrchr)
	addiu	$2,$2,%lo(strrchr)
	jalr	$2
	nop

	addiu	$3,$2,1
	lhu	$2,60($sp)
	sw	$2,16($sp)
	lui	$2,%hi($LC145)
	addiu	$4,$2,%lo($LC145)
	move	$5,$16
	move	$6,$3
	li	$7,3117			# 0xc2d
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L366:
	lhu	$2,60($sp)
	lw	$31,68($sp)
	lw	$16,64($sp)
	addiu	$sp,$sp,72
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	mt7570_TxPower_get_8472
	.size	mt7570_TxPower_get_8472, .-mt7570_TxPower_get_8472
	.section	.text.mt7570_internal_clock,"ax",@progbits
	.align	2
	.globl	mt7570_internal_clock
	.set	nomips16
	.set	nomicromips
	.ent	mt7570_internal_clock
	.type	mt7570_internal_clock, @function
mt7570_internal_clock:
	.frame	$sp,24,$31		# vars= 0, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-24
	sw	$31,20($sp)
	lui	$2,%hi(internal_DDMI)
	lw	$3,%lo(internal_DDMI)($2)
	li	$2,1			# 0x1
	bne	$3,$2,$L369
	nop

	lui	$2,%hi(cnt7570)
	lw	$3,%lo(cnt7570)($2)
	lui	$2,%hi(T_APD)
	lw	$2,%lo(T_APD)($2)
	divu	$0,$3,$2
	mfhi	$2
	move	$3,$2
	lui	$2,%hi(T_APD)
	lw	$2,%lo(T_APD)($2)
	addiu	$2,$2,-1
	bne	$3,$2,$L370
	nop

	lui	$2,%hi(xPON_mode)
	lw	$3,%lo(xPON_mode)($2)
	li	$2,1			# 0x1
	bne	$3,$2,$L370
	nop

	lui	$2,%hi(mt7570_APD_control)
	addiu	$2,$2,%lo(mt7570_APD_control)
	jalr	$2
	nop

$L370:
	lui	$2,%hi(cnt7570)
	lw	$3,%lo(cnt7570)($2)
	li	$2,-859045888			# 0xffffffffcccc0000
	ori	$2,$2,0xcccd
	multu	$3,$2
	mfhi	$2
	srl	$2,$2,3
	sll	$2,$2,1
	sll	$4,$2,2
	addu	$2,$2,$4
	subu	$2,$3,$2
	beq	$2,$0,$L371
	nop

	lui	$2,%hi(fast_DDMI)
	lw	$2,%lo(fast_DDMI)($2)
	beq	$2,$0,$L372
	nop

$L371:
	lui	$2,%hi(mt7570_supply_voltage_get_8472)
	addiu	$2,$2,%lo(mt7570_supply_voltage_get_8472)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(DDMI_voltage)
	sh	$3,%lo(DDMI_voltage)($2)
$L372:
	lui	$2,%hi(mt7570_bias_current_get_8472)
	addiu	$2,$2,%lo(mt7570_bias_current_get_8472)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(DDMI_current)
	sh	$3,%lo(DDMI_current)($2)
	lui	$2,%hi(cnt7570)
	lw	$3,%lo(cnt7570)($2)
	li	$2,-859045888			# 0xffffffffcccc0000
	ori	$2,$2,0xcccd
	multu	$3,$2
	mfhi	$2
	srl	$2,$2,3
	sll	$2,$2,1
	sll	$4,$2,2
	addu	$2,$2,$4
	subu	$2,$3,$2
	li	$3,4			# 0x4
	beq	$2,$3,$L373
	nop

	lui	$2,%hi(fast_DDMI)
	lw	$2,%lo(fast_DDMI)($2)
	beq	$2,$0,$L374
	nop

$L373:
	lui	$2,%hi(mt7570_temperature_get_8472)
	addiu	$2,$2,%lo(mt7570_temperature_get_8472)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(DDMI_temperature)
	sh	$3,%lo(DDMI_temperature)($2)
$L374:
	lui	$2,%hi(cnt7570)
	lw	$3,%lo(cnt7570)($2)
	li	$2,-859045888			# 0xffffffffcccc0000
	ori	$2,$2,0xcccd
	multu	$3,$2
	mfhi	$2
	srl	$2,$2,3
	sll	$2,$2,1
	sll	$4,$2,2
	addu	$2,$2,$4
	subu	$2,$3,$2
	li	$3,6			# 0x6
	beq	$2,$3,$L375
	nop

	lui	$2,%hi(fast_DDMI)
	lw	$2,%lo(fast_DDMI)($2)
	beq	$2,$0,$L376
	nop

$L375:
	lui	$2,%hi(mt7570_TxPower_get_8472)
	addiu	$2,$2,%lo(mt7570_TxPower_get_8472)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(DDMI_tx_power)
	sh	$3,%lo(DDMI_tx_power)($2)
	lui	$2,%hi(mt7570_tx_power_alarm_get)
	addiu	$2,$2,%lo(mt7570_tx_power_alarm_get)
	jalr	$2
	nop

$L376:
	lui	$2,%hi(cnt7570)
	lw	$3,%lo(cnt7570)($2)
	li	$2,-859045888			# 0xffffffffcccc0000
	ori	$2,$2,0xcccd
	multu	$3,$2
	mfhi	$2
	srl	$2,$2,3
	sll	$2,$2,1
	sll	$4,$2,2
	addu	$2,$2,$4
	subu	$2,$3,$2
	li	$3,8			# 0x8
	beq	$2,$3,$L377
	nop

	lui	$2,%hi(fast_DDMI)
	lw	$2,%lo(fast_DDMI)($2)
	beq	$2,$0,$L369
	nop

$L377:
	lui	$2,%hi(mt7570_RxPower_get_8472)
	addiu	$2,$2,%lo(mt7570_RxPower_get_8472)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(DDMI_rx_power)
	sh	$3,%lo(DDMI_rx_power)($2)
	lui	$2,%hi(mt7570_rx_power_alarm_get)
	addiu	$2,$2,%lo(mt7570_rx_power_alarm_get)
	jalr	$2
	nop

$L369:
	lui	$2,%hi(SCL)
	lw	$3,%lo(SCL)($2)
	li	$2,1			# 0x1
	bne	$3,$2,$L378
	nop

	lui	$2,%hi(cnt7570)
	lw	$5,%lo(cnt7570)($2)
	li	$2,-1925382144			# 0xffffffff8d3d0000
	ori	$2,$2,0xcb09
	multu	$5,$2
	mfhi	$2
	srl	$3,$2,4
	move	$4,$3
	sll	$2,$4,2
	move	$4,$2
	sll	$2,$4,3
	subu	$2,$2,$4
	addu	$2,$2,$3
	subu	$3,$5,$2
	bne	$3,$0,$L380
	nop

	lui	$2,%hi(ETC)
	lw	$3,%lo(ETC)($2)
	li	$2,3			# 0x3
	bne	$3,$2,$L380
	nop

	lui	$2,%hi(mt7570_BiasTracking)
	addiu	$2,$2,%lo(mt7570_BiasTracking)
	jalr	$2
	nop

	j	$L380
	nop

$L378:
	lui	$2,%hi(DOL)
	lw	$3,%lo(DOL)($2)
	li	$2,1			# 0x1
	bne	$3,$2,$L381
	nop

	lui	$2,%hi(cnt7570)
	lw	$4,%lo(cnt7570)($2)
	li	$2,-859045888			# 0xffffffffcccc0000
	ori	$2,$2,0xcccd
	multu	$4,$2
	mfhi	$2
	srl	$3,$2,2
	move	$2,$3
	sll	$2,$2,2
	addu	$2,$2,$3
	subu	$3,$4,$2
	bne	$3,$0,$L380
	nop

	lui	$2,%hi(mt7570_OpenLoopControl)
	addiu	$2,$2,%lo(mt7570_OpenLoopControl)
	jalr	$2
	nop

	j	$L380
	nop

$L381:
	lui	$2,%hi(xPON_mode)
	lw	$3,%lo(xPON_mode)($2)
	li	$2,-1			# 0xffffffffffffffff
	beq	$3,$2,$L383
	nop

	lui	$2,%hi(FiberPlug)
	lw	$3,%lo(FiberPlug)($2)
	li	$2,1			# 0x1
	bne	$3,$2,$L383
	nop

	lui	$2,%hi(PatternEnabled)
	lw	$3,%lo(PatternEnabled)($2)
	li	$2,1			# 0x1
	bne	$3,$2,$L383
	nop

	lui	$2,%hi(mt7570_FiberPlug_Protect)
	addiu	$2,$2,%lo(mt7570_FiberPlug_Protect)
	jalr	$2
	nop

$L383:
	lui	$2,%hi(cnt7570)
	lw	$4,%lo(cnt7570)($2)
	li	$2,-2004353024			# 0xffffffff88880000
	ori	$2,$2,0x8889
	multu	$4,$2
	mfhi	$2
	srl	$2,$2,4
	move	$3,$2
	sll	$2,$3,1
	move	$3,$2
	sll	$2,$3,4
	subu	$2,$2,$3
	subu	$2,$4,$2
	li	$3,29			# 0x1d
	bne	$2,$3,$L384
	nop

	lui	$2,%hi(ETC)
	lw	$3,%lo(ETC)($2)
	li	$2,1			# 0x1
	bne	$3,$2,$L385
	nop

	lui	$2,%hi(mt7570_ERTemperatureCompensation)
	addiu	$2,$2,%lo(mt7570_ERTemperatureCompensation)
	jalr	$2
	nop

	j	$L384
	nop

$L385:
	lui	$2,%hi(ETC)
	lw	$3,%lo(ETC)($2)
	li	$2,2			# 0x2
	beq	$3,$2,$L384
	nop

	lui	$2,%hi(ETC)
	lw	$3,%lo(ETC)($2)
	li	$2,3			# 0x3
	bne	$3,$2,$L384
	nop

	lui	$2,%hi(PatternEnabled)
	lw	$2,%lo(PatternEnabled)($2)
	bne	$2,$0,$L386
	nop

	lui	$2,%hi(BiasTracking_switch)
	lw	$2,%lo(BiasTracking_switch)($2)
	beq	$2,$0,$L384
	nop

$L386:
	lui	$2,%hi(mt7570_BiasTracking)
	addiu	$2,$2,%lo(mt7570_BiasTracking)
	jalr	$2
	nop

$L384:
	lui	$2,%hi(TEC)
	lw	$2,%lo(TEC)($2)
	beq	$2,$0,$L380
	nop

	lui	$2,%hi(TEC_switch)
	lw	$3,%lo(TEC_switch)($2)
	li	$2,1			# 0x1
	bne	$3,$2,$L380
	nop

	lui	$2,%hi(mt7570_TxEyeCorrection)
	addiu	$2,$2,%lo(mt7570_TxEyeCorrection)
	jalr	$2
	nop

$L380:
	lui	$2,%hi(cnt7570)
	lw	$4,%lo(cnt7570)($2)
	li	$2,-2004353024			# 0xffffffff88880000
	ori	$2,$2,0x8889
	multu	$4,$2
	mfhi	$2
	srl	$2,$2,6
	move	$3,$2
	sll	$2,$3,3
	move	$3,$2
	sll	$2,$3,4
	subu	$2,$2,$3
	subu	$2,$4,$2
	li	$3,119			# 0x77
	bne	$2,$3,$L387
	nop

	lui	$2,%hi(xPON_mode)
	lw	$3,%lo(xPON_mode)($2)
	li	$2,-1			# 0xffffffffffffffff
	beq	$3,$2,$L387
	nop

	lui	$2,%hi(mt7570_ADC_calibration)
	addiu	$2,$2,%lo(mt7570_ADC_calibration)
	jalr	$2
	nop

$L387:
	lui	$2,%hi(cnt7570)
	lw	$2,%lo(cnt7570)($2)
	addiu	$3,$2,1
	lui	$2,%hi(cnt7570)
	sw	$3,%lo(cnt7570)($2)
	lw	$31,20($sp)
	addiu	$sp,$sp,24
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	mt7570_internal_clock
	.size	mt7570_internal_clock, .-mt7570_internal_clock
	.section	.text.mt7570_param_status_real,"ax",@progbits
	.align	2
	.globl	mt7570_param_status_real
	.set	nomips16
	.set	nomicromips
	.ent	mt7570_param_status_real
	.type	mt7570_param_status_real, @function
mt7570_param_status_real:
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	sw	$4,0($sp)
	lui	$2,%hi(DDMI_temperature)
	lhu	$3,%lo(DDMI_temperature)($2)
	lw	$2,0($sp)
	sh	$3,0($2)
	lui	$2,%hi(DDMI_voltage)
	lhu	$3,%lo(DDMI_voltage)($2)
	lw	$2,0($sp)
	sh	$3,2($2)
	lui	$2,%hi(DDMI_current)
	lhu	$3,%lo(DDMI_current)($2)
	lw	$2,0($sp)
	sh	$3,4($2)
	lui	$2,%hi(DDMI_tx_power)
	lhu	$3,%lo(DDMI_tx_power)($2)
	lw	$2,0($sp)
	sh	$3,6($2)
	lui	$2,%hi(DDMI_rx_power)
	lhu	$3,%lo(DDMI_rx_power)($2)
	lw	$2,0($sp)
	sh	$3,8($2)
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	mt7570_param_status_real
	.size	mt7570_param_status_real, .-mt7570_param_status_real
	.rdata
	.align	2
$LC146:
	.ascii	"Supply Voltage = %05d\012\000"
	.align	2
$LC147:
	.ascii	"Tx Bias Current = %05d\012\000"
	.align	2
$LC148:
	.ascii	"Temperature = %05d\012\000"
	.align	2
$LC149:
	.ascii	"Tx power = %05d\012\000"
	.align	2
$LC150:
	.ascii	"Rx Power = %05d\012\000"
	.section	.text.DDMI_check_8472,"ax",@progbits
	.align	2
	.globl	DDMI_check_8472
	.set	nomips16
	.set	nomicromips
	.ent	DDMI_check_8472
	.type	DDMI_check_8472, @function
DDMI_check_8472:
	.frame	$sp,24,$31		# vars= 0, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-24
	sw	$31,20($sp)
	lui	$2,%hi(mt7570_supply_voltage_get_8472)
	addiu	$2,$2,%lo(mt7570_supply_voltage_get_8472)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(DDMI_voltage)
	sh	$3,%lo(DDMI_voltage)($2)
	lui	$2,%hi(mt7570_bias_current_get_8472)
	addiu	$2,$2,%lo(mt7570_bias_current_get_8472)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(DDMI_current)
	sh	$3,%lo(DDMI_current)($2)
	lui	$2,%hi(mt7570_temperature_get_8472)
	addiu	$2,$2,%lo(mt7570_temperature_get_8472)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(DDMI_temperature)
	sh	$3,%lo(DDMI_temperature)($2)
	lui	$2,%hi(mt7570_TxPower_get_8472)
	addiu	$2,$2,%lo(mt7570_TxPower_get_8472)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(DDMI_tx_power)
	sh	$3,%lo(DDMI_tx_power)($2)
	lui	$2,%hi(mt7570_RxPower_get_8472)
	addiu	$2,$2,%lo(mt7570_RxPower_get_8472)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(DDMI_rx_power)
	sh	$3,%lo(DDMI_rx_power)($2)
	lui	$2,%hi(DDMI_voltage)
	lhu	$2,%lo(DDMI_voltage)($2)
	move	$3,$2
	lui	$2,%hi($LC146)
	addiu	$4,$2,%lo($LC146)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi(DDMI_current)
	lhu	$2,%lo(DDMI_current)($2)
	move	$3,$2
	lui	$2,%hi($LC147)
	addiu	$4,$2,%lo($LC147)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi(DDMI_temperature)
	lhu	$2,%lo(DDMI_temperature)($2)
	move	$3,$2
	lui	$2,%hi($LC148)
	addiu	$4,$2,%lo($LC148)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi(DDMI_tx_power)
	lhu	$2,%lo(DDMI_tx_power)($2)
	move	$3,$2
	lui	$2,%hi($LC149)
	addiu	$4,$2,%lo($LC149)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi(DDMI_rx_power)
	lhu	$2,%lo(DDMI_rx_power)($2)
	move	$3,$2
	lui	$2,%hi($LC150)
	addiu	$4,$2,%lo($LC150)
	move	$5,$3
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
	.end	DDMI_check_8472
	.size	DDMI_check_8472, .-DDMI_check_8472
	.section	.text.mt7570_trans_model_setting,"ax",@progbits
	.align	2
	.globl	mt7570_trans_model_setting
	.set	nomips16
	.set	nomicromips
	.ent	mt7570_trans_model_setting
	.type	mt7570_trans_model_setting, @function
mt7570_trans_model_setting:
	.frame	$sp,24,$31		# vars= 0, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-24
	sw	$31,20($sp)
	li	$4,271			# 0x10f
	li	$2,-1079050240			# 0xffffffffbfaf0000
	ori	$5,$2,0x138
	lui	$2,%hi(iowrite32)
	addiu	$2,$2,%lo(iowrite32)
	jalr	$2
	nop

	lui	$2,%hi(i2c_access_queue)
	lw	$2,%lo(i2c_access_queue)($2)
	beq	$2,$0,$L390
	nop

	lui	$2,%hi(i2c_access_queue)
	lw	$2,%lo(i2c_access_queue)($2)
	move	$4,$2
	lui	$2,%hi(work_mt7570_safe_circuit_reset)
	addiu	$5,$2,%lo(work_mt7570_safe_circuit_reset)
	lui	$2,%hi(queue_work)
	addiu	$2,$2,%lo(queue_work)
	jalr	$2
	nop

$L390:
	lw	$31,20($sp)
	addiu	$sp,$sp,24
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	mt7570_trans_model_setting
	.size	mt7570_trans_model_setting, .-mt7570_trans_model_setting
	.rdata
	.align	2
$LC151:
	.ascii	"[%lu]0ms[%s:%d]\012  rx power high alarm threshold: %.8x"
	.ascii	"\012\000"
	.align	2
$LC152:
	.ascii	"[%lu]0ms[%s:%d]\012  rx power low alarm threshold: %.8x\012"
	.ascii	"\000"
	.align	2
$LC153:
	.ascii	"[%lu]0ms[%s:%d]\012  tx power high alarm threshold: %.8x"
	.ascii	"\012\000"
	.section	.text.mt7570_trans_alarm_getting,"ax",@progbits
	.align	2
	.globl	mt7570_trans_alarm_getting
	.set	nomips16
	.set	nomicromips
	.ent	mt7570_trans_alarm_getting
	.type	mt7570_trans_alarm_getting, @function
mt7570_trans_alarm_getting:
	.frame	$sp,32,$31		# vars= 0, regs= 2/0, args= 24, gp= 0
	.mask	0x80010000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-32
	sw	$31,28($sp)
	sw	$16,24($sp)
	sw	$4,32($sp)
	lw	$2,32($sp)
	sh	$0,0($2)
	lw	$2,32($sp)
	sh	$0,2($2)
	lw	$2,32($sp)
	sh	$0,4($2)
	lw	$2,32($sp)
	sh	$0,6($2)
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,136($2)
	andi	$2,$2,0x10
	beq	$2,$0,$L393
	nop

	lui	$2,%hi(jiffies)
	lw	$16,%lo(jiffies)($2)
	lui	$2,%hi($LC37)
	addiu	$4,$2,%lo($LC37)
	li	$5,47			# 0x2f
	lui	$2,%hi(strrchr)
	addiu	$2,$2,%lo(strrchr)
	jalr	$2
	nop

	addiu	$3,$2,1
	lw	$2,32($sp)
	lhu	$2,0($2)
	sw	$2,16($sp)
	lui	$2,%hi($LC151)
	addiu	$4,$2,%lo($LC151)
	move	$5,$16
	move	$6,$3
	li	$7,3324			# 0xcfc
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L393:
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,136($2)
	andi	$2,$2,0x10
	beq	$2,$0,$L394
	nop

	lui	$2,%hi(jiffies)
	lw	$16,%lo(jiffies)($2)
	lui	$2,%hi($LC37)
	addiu	$4,$2,%lo($LC37)
	li	$5,47			# 0x2f
	lui	$2,%hi(strrchr)
	addiu	$2,$2,%lo(strrchr)
	jalr	$2
	nop

	addiu	$3,$2,1
	lw	$2,32($sp)
	lhu	$2,2($2)
	sw	$2,16($sp)
	lui	$2,%hi($LC152)
	addiu	$4,$2,%lo($LC152)
	move	$5,$16
	move	$6,$3
	li	$7,3325			# 0xcfd
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L394:
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,136($2)
	andi	$2,$2,0x10
	beq	$2,$0,$L395
	nop

	lui	$2,%hi(jiffies)
	lw	$16,%lo(jiffies)($2)
	lui	$2,%hi($LC37)
	addiu	$4,$2,%lo($LC37)
	li	$5,47			# 0x2f
	lui	$2,%hi(strrchr)
	addiu	$2,$2,%lo(strrchr)
	jalr	$2
	nop

	addiu	$3,$2,1
	lw	$2,32($sp)
	lhu	$2,4($2)
	sw	$2,16($sp)
	lui	$2,%hi($LC153)
	addiu	$4,$2,%lo($LC153)
	move	$5,$16
	move	$6,$3
	li	$7,3326			# 0xcfe
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L395:
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,136($2)
	andi	$2,$2,0x10
	beq	$2,$0,$L392
	nop

	lui	$2,%hi(jiffies)
	lw	$16,%lo(jiffies)($2)
	lui	$2,%hi($LC37)
	addiu	$4,$2,%lo($LC37)
	li	$5,47			# 0x2f
	lui	$2,%hi(strrchr)
	addiu	$2,$2,%lo(strrchr)
	jalr	$2
	nop

	addiu	$3,$2,1
	lw	$2,32($sp)
	lhu	$2,6($2)
	sw	$2,16($sp)
	lui	$2,%hi($LC152)
	addiu	$4,$2,%lo($LC152)
	move	$5,$16
	move	$6,$3
	li	$7,3327			# 0xcff
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L392:
	lw	$31,28($sp)
	lw	$16,24($sp)
	addiu	$sp,$sp,32
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	mt7570_trans_alarm_getting
	.size	mt7570_trans_alarm_getting, .-mt7570_trans_alarm_getting
	.section	.text.mt7570_tx_power_alarm_get,"ax",@progbits
	.align	2
	.globl	mt7570_tx_power_alarm_get
	.set	nomips16
	.set	nomicromips
	.ent	mt7570_tx_power_alarm_get
	.type	mt7570_tx_power_alarm_get, @function
mt7570_tx_power_alarm_get:
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	lui	$2,%hi(DDMI_tx_power)
	lhu	$2,%lo(DDMI_tx_power)($2)
	sltu	$2,$2,10000
	beq	$2,$0,$L398
	nop

	lui	$2,%hi(mt7570_alarm)
	lw	$2,%lo(mt7570_alarm)($2)
	ori	$3,$2,0x2
	lui	$2,%hi(mt7570_alarm)
	sw	$3,%lo(mt7570_alarm)($2)
$L398:
	lui	$2,%hi(DDMI_tx_power)
	lhu	$3,%lo(DDMI_tx_power)($2)
	li	$2,35482			# 0x8a9a
	sltu	$2,$3,$2
	bne	$2,$0,$L399
	nop

	lui	$2,%hi(mt7570_alarm)
	lw	$2,%lo(mt7570_alarm)($2)
	ori	$3,$2,0x1
	lui	$2,%hi(mt7570_alarm)
	sw	$3,%lo(mt7570_alarm)($2)
$L399:
	lui	$2,%hi(DDMI_current)
	lhu	$2,%lo(DDMI_current)($2)
	sltu	$2,$2,500
	beq	$2,$0,$L400
	nop

	lui	$2,%hi(mt7570_alarm)
	lw	$2,%lo(mt7570_alarm)($2)
	ori	$3,$2,0x8
	lui	$2,%hi(mt7570_alarm)
	sw	$3,%lo(mt7570_alarm)($2)
$L400:
	lui	$2,%hi(DDMI_current)
	lhu	$3,%lo(DDMI_current)($2)
	li	$2,50001			# 0xc351
	sltu	$2,$3,$2
	bne	$2,$0,$L397
	nop

	lui	$2,%hi(mt7570_alarm)
	lw	$2,%lo(mt7570_alarm)($2)
	ori	$3,$2,0x4
	lui	$2,%hi(mt7570_alarm)
	sw	$3,%lo(mt7570_alarm)($2)
$L397:
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	mt7570_tx_power_alarm_get
	.size	mt7570_tx_power_alarm_get, .-mt7570_tx_power_alarm_get
	.section	.text.mt7570_rx_power_alarm_get,"ax",@progbits
	.align	2
	.globl	mt7570_rx_power_alarm_get
	.set	nomips16
	.set	nomicromips
	.ent	mt7570_rx_power_alarm_get
	.type	mt7570_rx_power_alarm_get, @function
mt7570_rx_power_alarm_get:
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	lui	$2,%hi(DDMI_rx_power)
	lhu	$2,%lo(DDMI_rx_power)($2)
	sltu	$2,$2,10
	beq	$2,$0,$L403
	nop

	lui	$2,%hi(mt7570_alarm)
	lw	$2,%lo(mt7570_alarm)($2)
	ori	$3,$2,0x20
	lui	$2,%hi(mt7570_alarm)
	sw	$3,%lo(mt7570_alarm)($2)
$L403:
	lui	$2,%hi(DDMI_rx_power)
	lhu	$2,%lo(DDMI_rx_power)($2)
	sltu	$2,$2,2512
	bne	$2,$0,$L402
	nop

	lui	$2,%hi(mt7570_alarm)
	lw	$2,%lo(mt7570_alarm)($2)
	ori	$3,$2,0x10
	lui	$2,%hi(mt7570_alarm)
	sw	$3,%lo(mt7570_alarm)($2)
$L402:
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	mt7570_rx_power_alarm_get
	.size	mt7570_rx_power_alarm_get, .-mt7570_rx_power_alarm_get
	.section	.text.mt7570_EN7570_detection,"ax",@progbits
	.align	2
	.globl	mt7570_EN7570_detection
	.set	nomips16
	.set	nomicromips
	.ent	mt7570_EN7570_detection
	.type	mt7570_EN7570_detection, @function
mt7570_EN7570_detection:
	.frame	$sp,48,$31		# vars= 8, regs= 1/0, args= 32, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-48
	sw	$31,44($sp)
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,116($2)
	move	$3,$2
	li	$2,368			# 0x170
	sw	$2,16($sp)
	addiu	$2,$sp,32
	sw	$2,20($sp)
	li	$2,1			# 0x1
	sw	$2,24($sp)
	move	$4,$0
	move	$5,$3
	li	$6,112			# 0x70
	li	$7,2			# 0x2
	lui	$2,%hi(phy_I2C_read)
	addiu	$2,$2,%lo(phy_I2C_read)
	jalr	$2
	nop

	lbu	$3,32($sp)
	li	$2,3			# 0x3
	bne	$3,$2,$L406
	nop

	lui	$2,%hi(mt7570_select)
	li	$3,1			# 0x1
	sw	$3,%lo(mt7570_select)($2)
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,116($2)
	move	$3,$2
	li	$2,348			# 0x15c
	sw	$2,16($sp)
	addiu	$2,$sp,32
	sw	$2,20($sp)
	li	$2,1			# 0x1
	sw	$2,24($sp)
	move	$4,$0
	move	$5,$3
	li	$6,112			# 0x70
	li	$7,2			# 0x2
	lui	$2,%hi(phy_I2C_read)
	addiu	$2,$2,%lo(phy_I2C_read)
	jalr	$2
	nop

	lbu	$2,32($sp)
	move	$3,$2
	lui	$2,%hi(vEN7570)
	sw	$3,%lo(vEN7570)($2)
	li	$2,1			# 0x1
	j	$L408
	nop

$L406:
	lui	$2,%hi(mt7570_select)
	sw	$0,%lo(mt7570_select)($2)
	move	$2,$0
$L408:
	lw	$31,44($sp)
	addiu	$sp,$sp,48
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	mt7570_EN7570_detection
	.size	mt7570_EN7570_detection, .-mt7570_EN7570_detection
	.rdata
	.align	2
$LC154:
	.ascii	"%s done\012\000"
	.section	.text.LOS_calibration,"ax",@progbits
	.align	2
	.globl	LOS_calibration
	.set	nomips16
	.set	nomicromips
	.ent	LOS_calibration
	.type	LOS_calibration, @function
LOS_calibration:
	.frame	$sp,48,$31		# vars= 8, regs= 1/0, args= 32, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-48
	sw	$31,44($sp)
	sw	$4,48($sp)
	sw	$5,52($sp)
	lui	$2,%hi(mt7570_LOS_init)
	addiu	$2,$2,%lo(mt7570_LOS_init)
	jalr	$2
	nop

	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,116($2)
	move	$3,$2
	li	$2,284			# 0x11c
	sw	$2,16($sp)
	addiu	$2,$sp,32
	sw	$2,20($sp)
	li	$2,4			# 0x4
	sw	$2,24($sp)
	move	$4,$0
	move	$5,$3
	li	$6,112			# 0x70
	li	$7,2			# 0x2
	lui	$2,%hi(phy_I2C_read)
	addiu	$2,$2,%lo(phy_I2C_read)
	jalr	$2
	nop

	lbu	$2,35($sp)
	seb	$3,$2
	li	$2,-128			# 0xffffffffffffff80
	and	$2,$3,$2
	seb	$3,$2
	lw	$2,52($sp)
	seb	$2,$2
	or	$2,$3,$2
	seb	$2,$2
	andi	$2,$2,0x00ff
	sb	$2,35($sp)
	lbu	$2,34($sp)
	seb	$3,$2
	li	$2,-128			# 0xffffffffffffff80
	and	$2,$3,$2
	seb	$3,$2
	lw	$2,48($sp)
	seb	$2,$2
	or	$2,$3,$2
	seb	$2,$2
	andi	$2,$2,0x00ff
	sb	$2,34($sp)
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,116($2)
	move	$3,$2
	li	$2,284			# 0x11c
	sw	$2,16($sp)
	addiu	$2,$sp,32
	sw	$2,20($sp)
	li	$2,4			# 0x4
	sw	$2,24($sp)
	move	$4,$0
	move	$5,$3
	li	$6,112			# 0x70
	li	$7,2			# 0x2
	lui	$2,%hi(phy_I2C_write)
	addiu	$2,$2,%lo(phy_I2C_write)
	jalr	$2
	nop

	lui	$2,%hi($LC154)
	addiu	$4,$2,%lo($LC154)
	lui	$2,%hi(__func__.25513)
	addiu	$5,$2,%lo(__func__.25513)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lw	$31,44($sp)
	addiu	$sp,$sp,48
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	LOS_calibration
	.size	LOS_calibration, .-LOS_calibration
	.rdata
	.align	2
$LC155:
	.ascii	"ERC filter set\012\000"
	.section	.text.mt7570_ERC_filter,"ax",@progbits
	.align	2
	.globl	mt7570_ERC_filter
	.set	nomips16
	.set	nomicromips
	.ent	mt7570_ERC_filter
	.type	mt7570_ERC_filter, @function
mt7570_ERC_filter:
	.frame	$sp,48,$31		# vars= 8, regs= 1/0, args= 32, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-48
	sw	$31,44($sp)
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,116($2)
	move	$3,$2
	li	$2,364			# 0x16c
	sw	$2,16($sp)
	addiu	$2,$sp,32
	sw	$2,20($sp)
	li	$2,4			# 0x4
	sw	$2,24($sp)
	move	$4,$0
	move	$5,$3
	li	$6,112			# 0x70
	li	$7,2			# 0x2
	lui	$2,%hi(phy_I2C_read)
	addiu	$2,$2,%lo(phy_I2C_read)
	jalr	$2
	nop

	li	$2,88			# 0x58
	sb	$2,34($sp)
	li	$2,-89			# 0xffffffffffffffa7
	sb	$2,33($sp)
	li	$2,-1			# 0xffffffffffffffff
	sb	$2,32($sp)
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,116($2)
	move	$3,$2
	li	$2,364			# 0x16c
	sw	$2,16($sp)
	addiu	$2,$sp,32
	sw	$2,20($sp)
	li	$2,4			# 0x4
	sw	$2,24($sp)
	move	$4,$0
	move	$5,$3
	li	$6,112			# 0x70
	li	$7,2			# 0x2
	lui	$2,%hi(phy_I2C_write)
	addiu	$2,$2,%lo(phy_I2C_write)
	jalr	$2
	nop

	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,116($2)
	move	$3,$2
	li	$2,364			# 0x16c
	sw	$2,16($sp)
	addiu	$2,$sp,32
	sw	$2,20($sp)
	li	$2,4			# 0x4
	sw	$2,24($sp)
	move	$4,$0
	move	$5,$3
	li	$6,112			# 0x70
	li	$7,2			# 0x2
	lui	$2,%hi(phy_I2C_read)
	addiu	$2,$2,%lo(phy_I2C_read)
	jalr	$2
	nop

	lbu	$2,35($sp)
	ori	$2,$2,0x1
	andi	$2,$2,0x00ff
	sb	$2,35($sp)
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,116($2)
	move	$3,$2
	li	$2,364			# 0x16c
	sw	$2,16($sp)
	addiu	$2,$sp,32
	sw	$2,20($sp)
	li	$2,4			# 0x4
	sw	$2,24($sp)
	move	$4,$0
	move	$5,$3
	li	$6,112			# 0x70
	li	$7,2			# 0x2
	lui	$2,%hi(phy_I2C_write)
	addiu	$2,$2,%lo(phy_I2C_write)
	jalr	$2
	nop

	lui	$2,%hi($LC155)
	addiu	$4,$2,%lo($LC155)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lw	$31,44($sp)
	addiu	$sp,$sp,48
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	mt7570_ERC_filter
	.size	mt7570_ERC_filter, .-mt7570_ERC_filter
	.rdata
	.align	2
$LC156:
	.ascii	"TIAGAIN set 0x%x\012\000"
	.section	.text.mt7570_TIAGAIN_set,"ax",@progbits
	.align	2
	.globl	mt7570_TIAGAIN_set
	.set	nomips16
	.set	nomicromips
	.ent	mt7570_TIAGAIN_set
	.type	mt7570_TIAGAIN_set, @function
mt7570_TIAGAIN_set:
	.frame	$sp,48,$31		# vars= 8, regs= 1/0, args= 32, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-48
	sw	$31,44($sp)
	sw	$0,32($sp)
	li	$4,40			# 0x28
	lui	$2,%hi(get_flash_register)
	addiu	$2,$2,%lo(get_flash_register)
	jalr	$2
	nop

	move	$3,$2
	li	$2,-1			# 0xffffffffffffffff
	beq	$3,$2,$L411
	nop

	li	$4,40			# 0x28
	lui	$2,%hi(get_flash_register)
	addiu	$2,$2,%lo(get_flash_register)
	jalr	$2
	nop

	andi	$2,$2,0x3
	sw	$2,32($sp)
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,116($2)
	move	$3,$2
	li	$2,1			# 0x1
	sw	$2,16($sp)
	addiu	$2,$sp,36
	sw	$2,20($sp)
	li	$2,1			# 0x1
	sw	$2,24($sp)
	move	$4,$0
	move	$5,$3
	li	$6,112			# 0x70
	li	$7,2			# 0x2
	lui	$2,%hi(phy_I2C_read)
	addiu	$2,$2,%lo(phy_I2C_read)
	jalr	$2
	nop

	lbu	$2,36($sp)
	andi	$2,$2,0x3f
	andi	$3,$2,0x00ff
	lw	$2,32($sp)
	andi	$2,$2,0x00ff
	sll	$2,$2,6
	andi	$2,$2,0x00ff
	or	$2,$3,$2
	andi	$2,$2,0x00ff
	sb	$2,36($sp)
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,116($2)
	move	$3,$2
	li	$2,1			# 0x1
	sw	$2,16($sp)
	addiu	$2,$sp,36
	sw	$2,20($sp)
	li	$2,1			# 0x1
	sw	$2,24($sp)
	move	$4,$0
	move	$5,$3
	li	$6,112			# 0x70
	li	$7,2			# 0x2
	lui	$2,%hi(phy_I2C_write)
	addiu	$2,$2,%lo(phy_I2C_write)
	jalr	$2
	nop

	lui	$2,%hi($LC156)
	addiu	$4,$2,%lo($LC156)
	lw	$5,32($sp)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L411:
	lw	$31,44($sp)
	addiu	$sp,$sp,48
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	mt7570_TIAGAIN_set
	.size	mt7570_TIAGAIN_set, .-mt7570_TIAGAIN_set
	.rdata
	.align	2
$LC157:
	.ascii	"LOS status = %d\012\000"
	.align	2
$LC158:
	.ascii	"Rouge ONU status = %d\012\000"
	.align	2
$LC159:
	.ascii	"Tx SD status = %d\012\000"
	.align	2
$LC160:
	.ascii	"Tx fault status = %d\012\000"
	.align	2
$LC161:
	.ascii	"I bias = %d.%.2dmA\012\000"
	.align	2
$LC163:
	.ascii	"I mod = %d.%.2dmA\012\000"
	.align	2
$LC164:
	.ascii	"IC temperature = %d.%dC\012\000"
	.align	2
$LC165:
	.ascii	"BOSA temperature = %d.%dC\012\000"
	.align	2
$LC166:
	.ascii	"Enviroment temperature = %d.%dC\012\000"
	.align	2
$LC167:
	.ascii	"APD Voltage = %d.%dV\012\000"
	.align	2
$LC168:
	.ascii	"VCC = %d.%.4dV\012\000"
	.align	2
$LC169:
	.ascii	"Tx power = -40 dBm\012\000"
	.align	2
$LC176:
	.ascii	"Tx power = %d.%.2d dBm\012\000"
	.globl	__gesf2
	.align	2
$LC177:
	.ascii	"Rx power = -40 dBm\012\000"
	.align	2
$LC180:
	.ascii	"Rx power = %d.%.2d dBm\012\000"
	.section	.text.show_BoB_information,"ax",@progbits
	.align	2
	.globl	show_BoB_information
	.set	nomips16
	.set	nomicromips
	.ent	show_BoB_information
	.type	show_BoB_information, @function
show_BoB_information:
	.frame	$sp,80,$31		# vars= 40, regs= 2/0, args= 32, gp= 0
	.mask	0x80010000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-80
	sw	$31,76($sp)
	sw	$16,72($sp)
	move	$2,$0
	sw	$2,32($sp)
	sw	$0,40($sp)
	sw	$0,44($sp)
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,116($2)
	move	$3,$2
	li	$2,304			# 0x130
	sw	$2,16($sp)
	addiu	$2,$sp,64
	sw	$2,20($sp)
	li	$2,4			# 0x4
	sw	$2,24($sp)
	move	$4,$0
	move	$5,$3
	li	$6,112			# 0x70
	li	$7,2			# 0x2
	lui	$2,%hi(phy_I2C_read)
	addiu	$2,$2,%lo(phy_I2C_read)
	jalr	$2
	nop

	lbu	$2,67($sp)
	andi	$2,$2,0x1
	andi	$2,$2,0x00ff
	sb	$2,67($sp)
	lbu	$2,67($sp)
	move	$3,$2
	lui	$2,%hi($LC157)
	addiu	$4,$2,%lo($LC157)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,116($2)
	move	$3,$2
	li	$2,360			# 0x168
	sw	$2,16($sp)
	addiu	$2,$sp,64
	sw	$2,20($sp)
	li	$2,2			# 0x2
	sw	$2,24($sp)
	move	$4,$0
	move	$5,$3
	li	$6,112			# 0x70
	li	$7,2			# 0x2
	lui	$2,%hi(phy_I2C_read)
	addiu	$2,$2,%lo(phy_I2C_read)
	jalr	$2
	nop

	lbu	$2,65($sp)
	srl	$2,$2,2
	andi	$2,$2,0x00ff
	andi	$2,$2,0x1
	andi	$2,$2,0x00ff
	sb	$2,65($sp)
	lbu	$2,65($sp)
	move	$3,$2
	lui	$2,%hi($LC158)
	addiu	$4,$2,%lo($LC158)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,116($2)
	move	$3,$2
	li	$2,360			# 0x168
	sw	$2,16($sp)
	addiu	$2,$sp,64
	sw	$2,20($sp)
	li	$2,2			# 0x2
	sw	$2,24($sp)
	move	$4,$0
	move	$5,$3
	li	$6,112			# 0x70
	li	$7,2			# 0x2
	lui	$2,%hi(phy_I2C_read)
	addiu	$2,$2,%lo(phy_I2C_read)
	jalr	$2
	nop

	lbu	$2,65($sp)
	srl	$2,$2,3
	andi	$2,$2,0x00ff
	andi	$2,$2,0x1
	andi	$2,$2,0x00ff
	sb	$2,65($sp)
	lbu	$2,65($sp)
	move	$3,$2
	lui	$2,%hi($LC159)
	addiu	$4,$2,%lo($LC159)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,116($2)
	move	$3,$2
	li	$2,256			# 0x100
	sw	$2,16($sp)
	addiu	$2,$sp,64
	sw	$2,20($sp)
	li	$2,2			# 0x2
	sw	$2,24($sp)
	move	$4,$0
	move	$5,$3
	li	$6,112			# 0x70
	li	$7,2			# 0x2
	lui	$2,%hi(phy_I2C_read)
	addiu	$2,$2,%lo(phy_I2C_read)
	jalr	$2
	nop

	lbu	$2,65($sp)
	srl	$2,$2,7
	andi	$2,$2,0x00ff
	sb	$2,65($sp)
	lbu	$2,65($sp)
	move	$3,$2
	lui	$2,%hi($LC160)
	addiu	$4,$2,%lo($LC160)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	li	$4,3			# 0x3
	lui	$2,%hi(mt7570_information_output)
	addiu	$2,$2,%lo(mt7570_information_output)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(__floatsisf)
	addiu	$2,$2,%lo(__floatsisf)
	move	$4,$3
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(__extendsfdf2)
	addiu	$2,$2,%lo(__extendsfdf2)
	move	$4,$3
	jalr	$2
	nop

	move	$5,$3
	move	$4,$2
	lui	$3,%hi($LC107)
	lui	$2,%hi(__muldf3)
	addiu	$2,$2,%lo(__muldf3)
	lw	$7,%lo($LC107+4)($3)
	lw	$6,%lo($LC107)($3)
	jalr	$2
	nop

	move	$5,$3
	move	$4,$2
	lui	$2,%hi(__truncdfsf2)
	addiu	$2,$2,%lo(__truncdfsf2)
	jalr	$2
	nop

	sw	$2,32($sp)
	lui	$2,%hi(__fixsfsi)
	addiu	$2,$2,%lo(__fixsfsi)
	lw	$4,32($sp)
	jalr	$2
	nop

	sw	$2,40($sp)
	lui	$3,%hi($LC84)
	lui	$2,%hi(__mulsf3)
	addiu	$2,$2,%lo(__mulsf3)
	lw	$4,32($sp)
	lw	$5,%lo($LC84)($3)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(__fixsfsi)
	addiu	$2,$2,%lo(__fixsfsi)
	move	$4,$3
	jalr	$2
	nop

	move	$3,$2
	li	$2,1374355456			# 0x51eb0000
	ori	$2,$2,0x851f
	mult	$3,$2
	mfhi	$2
	sra	$4,$2,5
	sra	$2,$3,31
	subu	$2,$4,$2
	sll	$2,$2,2
	sll	$4,$2,2
	addu	$2,$2,$4
	sll	$4,$2,2
	addu	$2,$2,$4
	subu	$2,$3,$2
	sw	$2,44($sp)
	lui	$2,%hi($LC161)
	addiu	$4,$2,%lo($LC161)
	lw	$5,40($sp)
	lw	$6,44($sp)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	li	$4,4			# 0x4
	lui	$2,%hi(mt7570_information_output)
	addiu	$2,$2,%lo(mt7570_information_output)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(__floatsisf)
	addiu	$2,$2,%lo(__floatsisf)
	move	$4,$3
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(__extendsfdf2)
	addiu	$2,$2,%lo(__extendsfdf2)
	move	$4,$3
	jalr	$2
	nop

	move	$5,$3
	move	$4,$2
	lui	$3,%hi($LC162)
	lui	$2,%hi(__muldf3)
	addiu	$2,$2,%lo(__muldf3)
	lw	$7,%lo($LC162+4)($3)
	lw	$6,%lo($LC162)($3)
	jalr	$2
	nop

	move	$5,$3
	move	$4,$2
	lui	$2,%hi(__truncdfsf2)
	addiu	$2,$2,%lo(__truncdfsf2)
	jalr	$2
	nop

	sw	$2,32($sp)
	lui	$2,%hi(__fixsfsi)
	addiu	$2,$2,%lo(__fixsfsi)
	lw	$4,32($sp)
	jalr	$2
	nop

	sw	$2,40($sp)
	lui	$3,%hi($LC84)
	lui	$2,%hi(__mulsf3)
	addiu	$2,$2,%lo(__mulsf3)
	lw	$4,32($sp)
	lw	$5,%lo($LC84)($3)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(__fixsfsi)
	addiu	$2,$2,%lo(__fixsfsi)
	move	$4,$3
	jalr	$2
	nop

	move	$3,$2
	li	$2,1374355456			# 0x51eb0000
	ori	$2,$2,0x851f
	mult	$3,$2
	mfhi	$2
	sra	$4,$2,5
	sra	$2,$3,31
	subu	$2,$4,$2
	sll	$2,$2,2
	sll	$4,$2,2
	addu	$2,$2,$4
	sll	$4,$2,2
	addu	$2,$2,$4
	subu	$2,$3,$2
	sw	$2,44($sp)
	lui	$2,%hi($LC163)
	addiu	$4,$2,%lo($LC163)
	lw	$5,40($sp)
	lw	$6,44($sp)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi(IC_temperature)
	lw	$3,%lo(IC_temperature)($2)
	lui	$2,%hi(__fixsfsi)
	addiu	$2,$2,%lo(__fixsfsi)
	move	$4,$3
	jalr	$2
	nop

	sw	$2,40($sp)
	lui	$2,%hi(IC_temperature)
	lw	$4,%lo(IC_temperature)($2)
	lui	$3,%hi($LC16)
	lui	$2,%hi(__mulsf3)
	addiu	$2,$2,%lo(__mulsf3)
	lw	$5,%lo($LC16)($3)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(__fixsfsi)
	addiu	$2,$2,%lo(__fixsfsi)
	move	$4,$3
	jalr	$2
	nop

	move	$3,$2
	li	$2,1717960704			# 0x66660000
	ori	$2,$2,0x6667
	mult	$3,$2
	mfhi	$2
	sra	$4,$2,2
	sra	$2,$3,31
	subu	$2,$4,$2
	sll	$2,$2,1
	sll	$4,$2,2
	addu	$2,$2,$4
	subu	$2,$3,$2
	sw	$2,48($sp)
	lw	$2,48($sp)
	bgez	$2,$L414
	nop

	subu	$2,$0,$2
$L414:
	sw	$2,44($sp)
	lui	$2,%hi($LC164)
	addiu	$4,$2,%lo($LC164)
	lw	$5,40($sp)
	lw	$6,44($sp)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi(BOSA_temperature)
	lw	$3,%lo(BOSA_temperature)($2)
	lui	$2,%hi(__fixsfsi)
	addiu	$2,$2,%lo(__fixsfsi)
	move	$4,$3
	jalr	$2
	nop

	sw	$2,40($sp)
	lui	$2,%hi($LC165)
	addiu	$4,$2,%lo($LC165)
	lw	$5,40($sp)
	lw	$6,44($sp)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi(Env_temperature)
	lw	$3,%lo(Env_temperature)($2)
	lui	$2,%hi(__fixsfsi)
	addiu	$2,$2,%lo(__fixsfsi)
	move	$4,$3
	jalr	$2
	nop

	sw	$2,40($sp)
	lui	$2,%hi(Env_temperature)
	lw	$4,%lo(Env_temperature)($2)
	lui	$3,%hi($LC16)
	lui	$2,%hi(__mulsf3)
	addiu	$2,$2,%lo(__mulsf3)
	lw	$5,%lo($LC16)($3)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(__fixsfsi)
	addiu	$2,$2,%lo(__fixsfsi)
	move	$4,$3
	jalr	$2
	nop

	move	$3,$2
	li	$2,1717960704			# 0x66660000
	ori	$2,$2,0x6667
	mult	$3,$2
	mfhi	$2
	sra	$4,$2,2
	sra	$2,$3,31
	subu	$2,$4,$2
	sll	$2,$2,1
	sll	$4,$2,2
	addu	$2,$2,$4
	subu	$2,$3,$2
	sw	$2,52($sp)
	lw	$2,52($sp)
	bgez	$2,$L415
	nop

	subu	$2,$0,$2
$L415:
	sw	$2,44($sp)
	lui	$2,%hi($LC166)
	addiu	$4,$2,%lo($LC166)
	lw	$5,40($sp)
	lw	$6,44($sp)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi(APD_voltage)
	lw	$3,%lo(APD_voltage)($2)
	lui	$2,%hi(__fixsfsi)
	addiu	$2,$2,%lo(__fixsfsi)
	move	$4,$3
	jalr	$2
	nop

	sw	$2,40($sp)
	lui	$2,%hi(APD_voltage)
	lw	$4,%lo(APD_voltage)($2)
	lui	$3,%hi($LC16)
	lui	$2,%hi(__mulsf3)
	addiu	$2,$2,%lo(__mulsf3)
	lw	$5,%lo($LC16)($3)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(__fixsfsi)
	addiu	$2,$2,%lo(__fixsfsi)
	move	$4,$3
	jalr	$2
	nop

	move	$3,$2
	li	$2,1717960704			# 0x66660000
	ori	$2,$2,0x6667
	mult	$3,$2
	mfhi	$2
	sra	$4,$2,2
	sra	$2,$3,31
	subu	$2,$4,$2
	sll	$2,$2,1
	sll	$4,$2,2
	addu	$2,$2,$4
	subu	$2,$3,$2
	sw	$2,44($sp)
	lui	$2,%hi($LC167)
	addiu	$4,$2,%lo($LC167)
	lw	$5,40($sp)
	lw	$6,44($sp)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi(global_supply_voltage_8472)
	lhu	$3,%lo(global_supply_voltage_8472)($2)
	li	$2,-776536064			# 0xffffffffd1b70000
	ori	$2,$2,0x1759
	multu	$3,$2
	mfhi	$2
	srl	$2,$2,13
	andi	$2,$2,0xffff
	sw	$2,40($sp)
	lui	$2,%hi(global_supply_voltage_8472)
	lhu	$2,%lo(global_supply_voltage_8472)($2)
	li	$3,-776536064			# 0xffffffffd1b70000
	ori	$3,$3,0x1759
	multu	$2,$3
	mfhi	$3
	srl	$4,$3,13
	li	$3,10000			# 0x2710
	mul	$3,$4,$3
	subu	$2,$2,$3
	andi	$2,$2,0xffff
	sw	$2,44($sp)
	lui	$2,%hi($LC168)
	addiu	$4,$2,%lo($LC168)
	lw	$5,40($sp)
	lw	$6,44($sp)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi(DDMI_tx_power)
	lhu	$3,%lo(DDMI_tx_power)($2)
	lui	$2,%hi(__floatunsisf)
	addiu	$2,$2,%lo(__floatunsisf)
	move	$4,$3
	jalr	$2
	nop

	move	$4,$2
	lui	$3,%hi($LC94)
	lui	$2,%hi(__divsf3)
	addiu	$2,$2,%lo(__divsf3)
	lw	$5,%lo($LC94)($3)
	jalr	$2
	nop

	move	$4,$2
	lui	$3,%hi($LC16)
	lui	$2,%hi(__divsf3)
	addiu	$2,$2,%lo(__divsf3)
	lw	$5,%lo($LC16)($3)
	jalr	$2
	nop

	sw	$2,32($sp)
	lui	$2,%hi(__eqsf2)
	addiu	$2,$2,%lo(__eqsf2)
	lw	$4,32($sp)
	move	$5,$0
	jalr	$2
	nop

	bne	$2,$0,$L433
	nop

	lui	$2,%hi($LC169)
	addiu	$4,$2,%lo($LC169)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	j	$L418
	nop

$L433:
	lui	$3,%hi($LC170)
	lui	$2,%hi(__subsf3)
	addiu	$2,$2,%lo(__subsf3)
	lw	$4,32($sp)
	lw	$5,%lo($LC170)($3)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(__divsf3)
	addiu	$2,$2,%lo(__divsf3)
	move	$4,$3
	lw	$5,32($sp)
	jalr	$2
	nop

	sw	$2,32($sp)
	lui	$2,%hi(__mulsf3)
	addiu	$2,$2,%lo(__mulsf3)
	lw	$4,32($sp)
	lw	$5,32($sp)
	jalr	$2
	nop

	move	$4,$2
	lui	$3,%hi($LC171)
	lui	$2,%hi(__divsf3)
	addiu	$2,$2,%lo(__divsf3)
	lw	$5,%lo($LC171)($3)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(__addsf3)
	addiu	$2,$2,%lo(__addsf3)
	move	$4,$3
	lw	$5,32($sp)
	jalr	$2
	nop

	move	$16,$2
	lui	$2,%hi(__mulsf3)
	addiu	$2,$2,%lo(__mulsf3)
	lw	$4,32($sp)
	lw	$5,32($sp)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(__mulsf3)
	addiu	$2,$2,%lo(__mulsf3)
	move	$4,$3
	lw	$5,32($sp)
	jalr	$2
	nop

	move	$4,$2
	lui	$3,%hi($LC172)
	lui	$2,%hi(__divsf3)
	addiu	$2,$2,%lo(__divsf3)
	lw	$5,%lo($LC172)($3)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(__addsf3)
	addiu	$2,$2,%lo(__addsf3)
	move	$4,$16
	move	$5,$3
	jalr	$2
	nop

	move	$16,$2
	lui	$2,%hi(__mulsf3)
	addiu	$2,$2,%lo(__mulsf3)
	lw	$4,32($sp)
	lw	$5,32($sp)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(__mulsf3)
	addiu	$2,$2,%lo(__mulsf3)
	move	$4,$3
	lw	$5,32($sp)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(__mulsf3)
	addiu	$2,$2,%lo(__mulsf3)
	move	$4,$3
	lw	$5,32($sp)
	jalr	$2
	nop

	move	$4,$2
	lui	$3,%hi($LC173)
	lui	$2,%hi(__divsf3)
	addiu	$2,$2,%lo(__divsf3)
	lw	$5,%lo($LC173)($3)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(__addsf3)
	addiu	$2,$2,%lo(__addsf3)
	move	$4,$16
	move	$5,$3
	jalr	$2
	nop

	move	$16,$2
	lui	$2,%hi(__mulsf3)
	addiu	$2,$2,%lo(__mulsf3)
	lw	$4,32($sp)
	lw	$5,32($sp)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(__mulsf3)
	addiu	$2,$2,%lo(__mulsf3)
	move	$4,$3
	lw	$5,32($sp)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(__mulsf3)
	addiu	$2,$2,%lo(__mulsf3)
	move	$4,$3
	lw	$5,32($sp)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(__mulsf3)
	addiu	$2,$2,%lo(__mulsf3)
	move	$4,$3
	lw	$5,32($sp)
	jalr	$2
	nop

	move	$4,$2
	lui	$3,%hi($LC100)
	lui	$2,%hi(__divsf3)
	addiu	$2,$2,%lo(__divsf3)
	lw	$5,%lo($LC100)($3)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(__addsf3)
	addiu	$2,$2,%lo(__addsf3)
	move	$4,$16
	move	$5,$3
	jalr	$2
	nop

	move	$16,$2
	lui	$2,%hi(__mulsf3)
	addiu	$2,$2,%lo(__mulsf3)
	lw	$4,32($sp)
	lw	$5,32($sp)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(__mulsf3)
	addiu	$2,$2,%lo(__mulsf3)
	move	$4,$3
	lw	$5,32($sp)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(__mulsf3)
	addiu	$2,$2,%lo(__mulsf3)
	move	$4,$3
	lw	$5,32($sp)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(__mulsf3)
	addiu	$2,$2,%lo(__mulsf3)
	move	$4,$3
	lw	$5,32($sp)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(__mulsf3)
	addiu	$2,$2,%lo(__mulsf3)
	move	$4,$3
	lw	$5,32($sp)
	jalr	$2
	nop

	move	$4,$2
	lui	$3,%hi($LC174)
	lui	$2,%hi(__divsf3)
	addiu	$2,$2,%lo(__divsf3)
	lw	$5,%lo($LC174)($3)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(__addsf3)
	addiu	$2,$2,%lo(__addsf3)
	move	$4,$16
	move	$5,$3
	jalr	$2
	nop

	sw	$2,32($sp)
	lui	$2,%hi(__extendsfdf2)
	addiu	$2,$2,%lo(__extendsfdf2)
	lw	$4,32($sp)
	jalr	$2
	nop

	move	$5,$3
	move	$4,$2
	lui	$3,%hi($LC175)
	lui	$2,%hi(__divdf3)
	addiu	$2,$2,%lo(__divdf3)
	lw	$7,%lo($LC175+4)($3)
	lw	$6,%lo($LC175)($3)
	jalr	$2
	nop

	move	$5,$3
	move	$4,$2
	lui	$2,%hi(__truncdfsf2)
	addiu	$2,$2,%lo(__truncdfsf2)
	jalr	$2
	nop

	sw	$2,32($sp)
	lui	$3,%hi($LC16)
	lui	$2,%hi(__mulsf3)
	addiu	$2,$2,%lo(__mulsf3)
	lw	$4,32($sp)
	lw	$5,%lo($LC16)($3)
	jalr	$2
	nop

	sw	$2,32($sp)
	lui	$2,%hi(__fixsfsi)
	addiu	$2,$2,%lo(__fixsfsi)
	lw	$4,32($sp)
	jalr	$2
	nop

	sw	$2,40($sp)
	lui	$3,%hi($LC84)
	lui	$2,%hi(__mulsf3)
	addiu	$2,$2,%lo(__mulsf3)
	lw	$4,32($sp)
	lw	$5,%lo($LC84)($3)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(__fixsfsi)
	addiu	$2,$2,%lo(__fixsfsi)
	move	$4,$3
	jalr	$2
	nop

	move	$3,$2
	li	$2,1374355456			# 0x51eb0000
	ori	$2,$2,0x851f
	mult	$3,$2
	mfhi	$2
	sra	$4,$2,5
	sra	$2,$3,31
	subu	$2,$4,$2
	sll	$2,$2,2
	sll	$4,$2,2
	addu	$2,$2,$4
	sll	$4,$2,2
	addu	$2,$2,$4
	subu	$2,$3,$2
	sw	$2,56($sp)
	lw	$2,56($sp)
	bgez	$2,$L419
	nop

	subu	$2,$0,$2
$L419:
	sw	$2,44($sp)
	lui	$2,%hi($LC176)
	addiu	$4,$2,%lo($LC176)
	lw	$5,40($sp)
	lw	$6,44($sp)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L418:
	lui	$2,%hi(DDMI_rx_power)
	lhu	$3,%lo(DDMI_rx_power)($2)
	lui	$2,%hi(__floatunsisf)
	addiu	$2,$2,%lo(__floatunsisf)
	move	$4,$3
	jalr	$2
	nop

	sw	$2,32($sp)
	sw	$0,36($sp)
	j	$L420
	nop

$L425:
	lui	$3,%hi($LC100)
	lui	$2,%hi(__gesf2)
	addiu	$2,$2,%lo(__gesf2)
	lw	$4,32($sp)
	lw	$5,%lo($LC100)($3)
	jalr	$2
	nop

	bgez	$2,$L431
	nop

	j	$L424
	nop

$L431:
	lui	$3,%hi($LC16)
	lui	$2,%hi(__divsf3)
	addiu	$2,$2,%lo(__divsf3)
	lw	$4,32($sp)
	lw	$5,%lo($LC16)($3)
	jalr	$2
	nop

	sw	$2,32($sp)
	lw	$2,36($sp)
	addiu	$2,$2,1
	sw	$2,36($sp)
$L420:
	lw	$2,36($sp)
	slt	$2,$2,7
	bne	$2,$0,$L425
	nop

$L424:
	lui	$2,%hi(__eqsf2)
	addiu	$2,$2,%lo(__eqsf2)
	lw	$4,32($sp)
	move	$5,$0
	jalr	$2
	nop

	bne	$2,$0,$L434
	nop

	lui	$2,%hi($LC177)
	addiu	$4,$2,%lo($LC177)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	j	$L413
	nop

$L434:
	lui	$3,%hi($LC170)
	lui	$2,%hi(__subsf3)
	addiu	$2,$2,%lo(__subsf3)
	lw	$4,32($sp)
	lw	$5,%lo($LC170)($3)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(__divsf3)
	addiu	$2,$2,%lo(__divsf3)
	move	$4,$3
	lw	$5,32($sp)
	jalr	$2
	nop

	sw	$2,32($sp)
	lui	$2,%hi(__mulsf3)
	addiu	$2,$2,%lo(__mulsf3)
	lw	$4,32($sp)
	lw	$5,32($sp)
	jalr	$2
	nop

	move	$4,$2
	lui	$3,%hi($LC171)
	lui	$2,%hi(__divsf3)
	addiu	$2,$2,%lo(__divsf3)
	lw	$5,%lo($LC171)($3)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(__addsf3)
	addiu	$2,$2,%lo(__addsf3)
	move	$4,$3
	lw	$5,32($sp)
	jalr	$2
	nop

	move	$16,$2
	lui	$2,%hi(__mulsf3)
	addiu	$2,$2,%lo(__mulsf3)
	lw	$4,32($sp)
	lw	$5,32($sp)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(__mulsf3)
	addiu	$2,$2,%lo(__mulsf3)
	move	$4,$3
	lw	$5,32($sp)
	jalr	$2
	nop

	move	$4,$2
	lui	$3,%hi($LC172)
	lui	$2,%hi(__divsf3)
	addiu	$2,$2,%lo(__divsf3)
	lw	$5,%lo($LC172)($3)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(__addsf3)
	addiu	$2,$2,%lo(__addsf3)
	move	$4,$16
	move	$5,$3
	jalr	$2
	nop

	move	$16,$2
	lui	$2,%hi(__mulsf3)
	addiu	$2,$2,%lo(__mulsf3)
	lw	$4,32($sp)
	lw	$5,32($sp)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(__mulsf3)
	addiu	$2,$2,%lo(__mulsf3)
	move	$4,$3
	lw	$5,32($sp)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(__mulsf3)
	addiu	$2,$2,%lo(__mulsf3)
	move	$4,$3
	lw	$5,32($sp)
	jalr	$2
	nop

	move	$4,$2
	lui	$3,%hi($LC173)
	lui	$2,%hi(__divsf3)
	addiu	$2,$2,%lo(__divsf3)
	lw	$5,%lo($LC173)($3)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(__addsf3)
	addiu	$2,$2,%lo(__addsf3)
	move	$4,$16
	move	$5,$3
	jalr	$2
	nop

	move	$16,$2
	lui	$2,%hi(__mulsf3)
	addiu	$2,$2,%lo(__mulsf3)
	lw	$4,32($sp)
	lw	$5,32($sp)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(__mulsf3)
	addiu	$2,$2,%lo(__mulsf3)
	move	$4,$3
	lw	$5,32($sp)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(__mulsf3)
	addiu	$2,$2,%lo(__mulsf3)
	move	$4,$3
	lw	$5,32($sp)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(__mulsf3)
	addiu	$2,$2,%lo(__mulsf3)
	move	$4,$3
	lw	$5,32($sp)
	jalr	$2
	nop

	move	$4,$2
	lui	$3,%hi($LC100)
	lui	$2,%hi(__divsf3)
	addiu	$2,$2,%lo(__divsf3)
	lw	$5,%lo($LC100)($3)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(__addsf3)
	addiu	$2,$2,%lo(__addsf3)
	move	$4,$16
	move	$5,$3
	jalr	$2
	nop

	move	$16,$2
	lui	$2,%hi(__mulsf3)
	addiu	$2,$2,%lo(__mulsf3)
	lw	$4,32($sp)
	lw	$5,32($sp)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(__mulsf3)
	addiu	$2,$2,%lo(__mulsf3)
	move	$4,$3
	lw	$5,32($sp)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(__mulsf3)
	addiu	$2,$2,%lo(__mulsf3)
	move	$4,$3
	lw	$5,32($sp)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(__mulsf3)
	addiu	$2,$2,%lo(__mulsf3)
	move	$4,$3
	lw	$5,32($sp)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(__mulsf3)
	addiu	$2,$2,%lo(__mulsf3)
	move	$4,$3
	lw	$5,32($sp)
	jalr	$2
	nop

	move	$4,$2
	lui	$3,%hi($LC174)
	lui	$2,%hi(__divsf3)
	addiu	$2,$2,%lo(__divsf3)
	lw	$5,%lo($LC174)($3)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(__addsf3)
	addiu	$2,$2,%lo(__addsf3)
	move	$4,$16
	move	$5,$3
	jalr	$2
	nop

	move	$16,$2
	lui	$2,%hi(__mulsf3)
	addiu	$2,$2,%lo(__mulsf3)
	lw	$4,32($sp)
	lw	$5,32($sp)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(__mulsf3)
	addiu	$2,$2,%lo(__mulsf3)
	move	$4,$3
	lw	$5,32($sp)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(__mulsf3)
	addiu	$2,$2,%lo(__mulsf3)
	move	$4,$3
	lw	$5,32($sp)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(__mulsf3)
	addiu	$2,$2,%lo(__mulsf3)
	move	$4,$3
	lw	$5,32($sp)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(__mulsf3)
	addiu	$2,$2,%lo(__mulsf3)
	move	$4,$3
	lw	$5,32($sp)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(__mulsf3)
	addiu	$2,$2,%lo(__mulsf3)
	move	$4,$3
	lw	$5,32($sp)
	jalr	$2
	nop

	move	$4,$2
	lui	$3,%hi($LC178)
	lui	$2,%hi(__divsf3)
	addiu	$2,$2,%lo(__divsf3)
	lw	$5,%lo($LC178)($3)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(__addsf3)
	addiu	$2,$2,%lo(__addsf3)
	move	$4,$16
	move	$5,$3
	jalr	$2
	nop

	move	$16,$2
	lui	$2,%hi(__mulsf3)
	addiu	$2,$2,%lo(__mulsf3)
	lw	$4,32($sp)
	lw	$5,32($sp)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(__mulsf3)
	addiu	$2,$2,%lo(__mulsf3)
	move	$4,$3
	lw	$5,32($sp)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(__mulsf3)
	addiu	$2,$2,%lo(__mulsf3)
	move	$4,$3
	lw	$5,32($sp)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(__mulsf3)
	addiu	$2,$2,%lo(__mulsf3)
	move	$4,$3
	lw	$5,32($sp)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(__mulsf3)
	addiu	$2,$2,%lo(__mulsf3)
	move	$4,$3
	lw	$5,32($sp)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(__mulsf3)
	addiu	$2,$2,%lo(__mulsf3)
	move	$4,$3
	lw	$5,32($sp)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(__mulsf3)
	addiu	$2,$2,%lo(__mulsf3)
	move	$4,$3
	lw	$5,32($sp)
	jalr	$2
	nop

	move	$4,$2
	lui	$3,%hi($LC179)
	lui	$2,%hi(__divsf3)
	addiu	$2,$2,%lo(__divsf3)
	lw	$5,%lo($LC179)($3)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(__addsf3)
	addiu	$2,$2,%lo(__addsf3)
	move	$4,$16
	move	$5,$3
	jalr	$2
	nop

	sw	$2,32($sp)
	lui	$2,%hi(__extendsfdf2)
	addiu	$2,$2,%lo(__extendsfdf2)
	lw	$4,32($sp)
	jalr	$2
	nop

	move	$5,$3
	move	$4,$2
	lui	$3,%hi($LC175)
	lui	$2,%hi(__divdf3)
	addiu	$2,$2,%lo(__divdf3)
	lw	$7,%lo($LC175+4)($3)
	lw	$6,%lo($LC175)($3)
	jalr	$2
	nop

	move	$5,$3
	move	$4,$2
	lui	$2,%hi(__truncdfsf2)
	addiu	$2,$2,%lo(__truncdfsf2)
	jalr	$2
	nop

	sw	$2,32($sp)
	li	$3,4			# 0x4
	lw	$2,36($sp)
	subu	$3,$3,$2
	lui	$2,%hi(__floatsisf)
	addiu	$2,$2,%lo(__floatsisf)
	move	$4,$3
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(__subsf3)
	addiu	$2,$2,%lo(__subsf3)
	lw	$4,32($sp)
	move	$5,$3
	jalr	$2
	nop

	sw	$2,32($sp)
	lui	$3,%hi($LC16)
	lui	$2,%hi(__mulsf3)
	addiu	$2,$2,%lo(__mulsf3)
	lw	$4,32($sp)
	lw	$5,%lo($LC16)($3)
	jalr	$2
	nop

	sw	$2,32($sp)
	lui	$2,%hi(__fixsfsi)
	addiu	$2,$2,%lo(__fixsfsi)
	lw	$4,32($sp)
	jalr	$2
	nop

	sw	$2,40($sp)
	lui	$3,%hi($LC84)
	lui	$2,%hi(__mulsf3)
	addiu	$2,$2,%lo(__mulsf3)
	lw	$4,32($sp)
	lw	$5,%lo($LC84)($3)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(__fixsfsi)
	addiu	$2,$2,%lo(__fixsfsi)
	move	$4,$3
	jalr	$2
	nop

	move	$3,$2
	li	$2,1374355456			# 0x51eb0000
	ori	$2,$2,0x851f
	mult	$3,$2
	mfhi	$2
	sra	$4,$2,5
	sra	$2,$3,31
	subu	$2,$4,$2
	sll	$2,$2,2
	sll	$4,$2,2
	addu	$2,$2,$4
	sll	$4,$2,2
	addu	$2,$2,$4
	subu	$2,$3,$2
	sw	$2,60($sp)
	lw	$2,60($sp)
	bgez	$2,$L429
	nop

	subu	$2,$0,$2
$L429:
	sw	$2,44($sp)
	lui	$2,%hi($LC180)
	addiu	$4,$2,%lo($LC180)
	lw	$5,40($sp)
	lw	$6,44($sp)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L413:
	lw	$31,76($sp)
	lw	$16,72($sp)
	addiu	$sp,$sp,80
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	show_BoB_information
	.size	show_BoB_information, .-show_BoB_information
	.rdata
	.align	2
$LC181:
	.ascii	"TEC done\012\000"
	.section	.text.mt7570_TxEyeCorrection,"ax",@progbits
	.align	2
	.globl	mt7570_TxEyeCorrection
	.set	nomips16
	.set	nomicromips
	.ent	mt7570_TxEyeCorrection
	.type	mt7570_TxEyeCorrection, @function
mt7570_TxEyeCorrection:
	.frame	$sp,48,$31		# vars= 8, regs= 1/0, args= 32, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-48
	sw	$31,44($sp)
	sw	$0,32($sp)
	lui	$2,%hi(TEC_cnt.25547)
	lw	$2,%lo(TEC_cnt.25547)($2)
	slt	$2,$2,8
	beq	$2,$0,$L435
	nop

	lui	$2,%hi(global_bias_current_8472)
	lhu	$2,%lo(global_bias_current_8472)($2)
	sll	$2,$2,1
	move	$3,$2
	lui	$2,%hi(BOSA_Ith)
	lw	$2,%lo(BOSA_Ith)($2)
	sltu	$2,$3,$2
	beq	$2,$0,$L435
	nop

	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,116($2)
	move	$3,$2
	li	$2,316			# 0x13c
	sw	$2,16($sp)
	addiu	$2,$sp,36
	sw	$2,20($sp)
	li	$2,1			# 0x1
	sw	$2,24($sp)
	move	$4,$0
	move	$5,$3
	li	$6,112			# 0x70
	li	$7,2			# 0x2
	lui	$2,%hi(phy_I2C_read)
	addiu	$2,$2,%lo(phy_I2C_read)
	jalr	$2
	nop

	lbu	$2,36($sp)
	andi	$3,$2,0xf
	li	$2,5			# 0x5
	bne	$3,$2,$L435
	nop

	li	$4,5			# 0x5
	lui	$2,%hi(mt7570_information_output)
	addiu	$2,$2,%lo(mt7570_information_output)
	jalr	$2
	nop

	addiu	$2,$2,4
	sw	$2,32($sp)
	lw	$2,32($sp)
	andi	$2,$2,0x00ff
	sb	$2,36($sp)
	lw	$2,32($sp)
	srl	$2,$2,8
	andi	$2,$2,0x00ff
	sb	$2,37($sp)
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,116($2)
	move	$3,$2
	li	$2,6			# 0x6
	sw	$2,16($sp)
	addiu	$2,$sp,36
	sw	$2,20($sp)
	li	$2,2			# 0x2
	sw	$2,24($sp)
	move	$4,$0
	move	$5,$3
	li	$6,112			# 0x70
	li	$7,2			# 0x2
	lui	$2,%hi(phy_I2C_write)
	addiu	$2,$2,%lo(phy_I2C_write)
	jalr	$2
	nop

	lui	$2,%hi(mt7570_restart_ERC_P0)
	addiu	$2,$2,%lo(mt7570_restart_ERC_P0)
	jalr	$2
	nop

	lui	$2,%hi($LC181)
	addiu	$4,$2,%lo($LC181)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi(TEC_cnt.25547)
	lw	$2,%lo(TEC_cnt.25547)($2)
	addiu	$3,$2,1
	lui	$2,%hi(TEC_cnt.25547)
	sw	$3,%lo(TEC_cnt.25547)($2)
$L435:
	lw	$31,44($sp)
	addiu	$sp,$sp,48
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	mt7570_TxEyeCorrection
	.size	mt7570_TxEyeCorrection, .-mt7570_TxEyeCorrection
	.rdata
	.align	2
$LC182:
	.ascii	"Switch to Open-Loop Mode\012\000"
	.section	.text.mt7570_OpenLoopMode,"ax",@progbits
	.align	2
	.globl	mt7570_OpenLoopMode
	.set	nomips16
	.set	nomicromips
	.ent	mt7570_OpenLoopMode
	.type	mt7570_OpenLoopMode, @function
mt7570_OpenLoopMode:
	.frame	$sp,48,$31		# vars= 8, regs= 1/0, args= 32, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-48
	sw	$31,44($sp)
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,116($2)
	move	$3,$2
	li	$2,316			# 0x13c
	sw	$2,16($sp)
	addiu	$2,$sp,32
	sw	$2,20($sp)
	li	$2,1			# 0x1
	sw	$2,24($sp)
	move	$4,$0
	move	$5,$3
	li	$6,112			# 0x70
	li	$7,2			# 0x2
	lui	$2,%hi(phy_I2C_read)
	addiu	$2,$2,%lo(phy_I2C_read)
	jalr	$2
	nop

	lbu	$2,32($sp)
	seb	$3,$2
	li	$2,-4			# 0xfffffffffffffffc
	and	$2,$3,$2
	seb	$2,$2
	ori	$2,$2,0x2
	seb	$2,$2
	andi	$2,$2,0x00ff
	sb	$2,32($sp)
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,116($2)
	move	$3,$2
	li	$2,316			# 0x13c
	sw	$2,16($sp)
	addiu	$2,$sp,32
	sw	$2,20($sp)
	li	$2,1			# 0x1
	sw	$2,24($sp)
	move	$4,$0
	move	$5,$3
	li	$6,112			# 0x70
	li	$7,2			# 0x2
	lui	$2,%hi(phy_I2C_write)
	addiu	$2,$2,%lo(phy_I2C_write)
	jalr	$2
	nop

	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,116($2)
	move	$3,$2
	li	$2,332			# 0x14c
	sw	$2,16($sp)
	addiu	$2,$sp,32
	sw	$2,20($sp)
	li	$2,1			# 0x1
	sw	$2,24($sp)
	move	$4,$0
	move	$5,$3
	li	$6,112			# 0x70
	li	$7,2			# 0x2
	lui	$2,%hi(phy_I2C_read)
	addiu	$2,$2,%lo(phy_I2C_read)
	jalr	$2
	nop

	lbu	$2,32($sp)
	seb	$3,$2
	li	$2,-4			# 0xfffffffffffffffc
	and	$2,$3,$2
	seb	$2,$2
	ori	$2,$2,0x2
	seb	$2,$2
	andi	$2,$2,0x00ff
	sb	$2,32($sp)
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,116($2)
	move	$3,$2
	li	$2,332			# 0x14c
	sw	$2,16($sp)
	addiu	$2,$sp,32
	sw	$2,20($sp)
	li	$2,1			# 0x1
	sw	$2,24($sp)
	move	$4,$0
	move	$5,$3
	li	$6,112			# 0x70
	li	$7,2			# 0x2
	lui	$2,%hi(phy_I2C_write)
	addiu	$2,$2,%lo(phy_I2C_write)
	jalr	$2
	nop

	lui	$2,%hi($LC182)
	addiu	$4,$2,%lo($LC182)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lw	$31,44($sp)
	addiu	$sp,$sp,48
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	mt7570_OpenLoopMode
	.size	mt7570_OpenLoopMode, .-mt7570_OpenLoopMode
	.rdata
	.align	2
$LC183:
	.ascii	"tempature %d degree save done\012\000"
	.align	2
$LC184:
	.ascii	"Ibias current = %d.%.2dmA \012\000"
	.section	.text.Ibias_temperature_set,"ax",@progbits
	.align	2
	.globl	Ibias_temperature_set
	.set	nomips16
	.set	nomicromips
	.ent	Ibias_temperature_set
	.type	Ibias_temperature_set, @function
Ibias_temperature_set:
	.frame	$sp,64,$31		# vars= 24, regs= 1/0, args= 32, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-64
	sw	$31,60($sp)
	sw	$0,32($sp)
	sw	$0,36($sp)
	sw	$0,40($sp)
	move	$2,$0
	sw	$2,44($sp)
	lui	$2,%hi(mt7570_temperature_get)
	addiu	$2,$2,%lo(mt7570_temperature_get)
	jalr	$2
	nop

	lui	$2,%hi(IC_temperature)
	lw	$3,%lo(IC_temperature)($2)
	lui	$2,%hi(__fixsfsi)
	addiu	$2,$2,%lo(__fixsfsi)
	move	$4,$3
	jalr	$2
	nop

	sw	$2,32($sp)
	li	$4,96			# 0x60
	lui	$2,%hi(get_flash_register)
	addiu	$2,$2,%lo(get_flash_register)
	jalr	$2
	nop

	sw	$2,40($sp)
	lw	$3,40($sp)
	li	$2,-65536			# 0xffffffffffff0000
	and	$3,$3,$2
	lw	$2,32($sp)
	or	$2,$3,$2
	sw	$2,40($sp)
	lw	$2,40($sp)
	move	$4,$2
	li	$5,96			# 0x60
	lui	$2,%hi(set_flash_register)
	addiu	$2,$2,%lo(set_flash_register)
	jalr	$2
	nop

	lui	$2,%hi($LC183)
	addiu	$4,$2,%lo($LC183)
	lw	$5,32($sp)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,116($2)
	move	$3,$2
	li	$2,312			# 0x138
	sw	$2,16($sp)
	addiu	$2,$sp,48
	sw	$2,20($sp)
	li	$2,2			# 0x2
	sw	$2,24($sp)
	move	$4,$0
	move	$5,$3
	li	$6,112			# 0x70
	li	$7,2			# 0x2
	lui	$2,%hi(phy_I2C_read)
	addiu	$2,$2,%lo(phy_I2C_read)
	jalr	$2
	nop

	li	$4,96			# 0x60
	lui	$2,%hi(get_flash_register)
	addiu	$2,$2,%lo(get_flash_register)
	jalr	$2
	nop

	sw	$2,40($sp)
	lw	$2,40($sp)
	andi	$3,$2,0xffff
	lbu	$2,48($sp)
	sll	$2,$2,16
	or	$3,$3,$2
	lbu	$2,49($sp)
	sll	$2,$2,24
	or	$2,$3,$2
	sw	$2,40($sp)
	lw	$2,40($sp)
	move	$4,$2
	li	$5,96			# 0x60
	lui	$2,%hi(set_flash_register)
	addiu	$2,$2,%lo(set_flash_register)
	jalr	$2
	nop

	li	$4,3			# 0x3
	lui	$2,%hi(mt7570_information_output)
	addiu	$2,$2,%lo(mt7570_information_output)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(__floatsisf)
	addiu	$2,$2,%lo(__floatsisf)
	move	$4,$3
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(__extendsfdf2)
	addiu	$2,$2,%lo(__extendsfdf2)
	move	$4,$3
	jalr	$2
	nop

	move	$5,$3
	move	$4,$2
	lui	$3,%hi($LC107)
	lui	$2,%hi(__muldf3)
	addiu	$2,$2,%lo(__muldf3)
	lw	$7,%lo($LC107+4)($3)
	lw	$6,%lo($LC107)($3)
	jalr	$2
	nop

	move	$5,$3
	move	$4,$2
	lui	$2,%hi(__truncdfsf2)
	addiu	$2,$2,%lo(__truncdfsf2)
	jalr	$2
	nop

	sw	$2,44($sp)
	lui	$2,%hi(__fixsfsi)
	addiu	$2,$2,%lo(__fixsfsi)
	lw	$4,44($sp)
	jalr	$2
	nop

	sw	$2,32($sp)
	lui	$3,%hi($LC84)
	lui	$2,%hi(__mulsf3)
	addiu	$2,$2,%lo(__mulsf3)
	lw	$4,44($sp)
	lw	$5,%lo($LC84)($3)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(__fixsfsi)
	addiu	$2,$2,%lo(__fixsfsi)
	move	$4,$3
	jalr	$2
	nop

	move	$3,$2
	li	$2,1374355456			# 0x51eb0000
	ori	$2,$2,0x851f
	mult	$3,$2
	mfhi	$2
	sra	$4,$2,5
	sra	$2,$3,31
	subu	$2,$4,$2
	sll	$2,$2,2
	sll	$4,$2,2
	addu	$2,$2,$4
	sll	$4,$2,2
	addu	$2,$2,$4
	subu	$2,$3,$2
	sw	$2,36($sp)
	lui	$2,%hi($LC184)
	addiu	$4,$2,%lo($LC184)
	lw	$5,32($sp)
	lw	$6,36($sp)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lw	$31,60($sp)
	addiu	$sp,$sp,64
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	Ibias_temperature_set
	.size	Ibias_temperature_set, .-Ibias_temperature_set
	.rdata
	.align	2
$LC185:
	.ascii	"Switch to Dual Closed-Loop Mode\012\000"
	.section	.text.mt7570_DualClosedLoopMode,"ax",@progbits
	.align	2
	.globl	mt7570_DualClosedLoopMode
	.set	nomips16
	.set	nomicromips
	.ent	mt7570_DualClosedLoopMode
	.type	mt7570_DualClosedLoopMode, @function
mt7570_DualClosedLoopMode:
	.frame	$sp,48,$31		# vars= 8, regs= 1/0, args= 32, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-48
	sw	$31,44($sp)
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,116($2)
	move	$3,$2
	li	$2,316			# 0x13c
	sw	$2,16($sp)
	addiu	$2,$sp,32
	sw	$2,20($sp)
	li	$2,1			# 0x1
	sw	$2,24($sp)
	move	$4,$0
	move	$5,$3
	li	$6,112			# 0x70
	li	$7,2			# 0x2
	lui	$2,%hi(phy_I2C_read)
	addiu	$2,$2,%lo(phy_I2C_read)
	jalr	$2
	nop

	lbu	$2,32($sp)
	seb	$3,$2
	li	$2,-4			# 0xfffffffffffffffc
	and	$2,$3,$2
	seb	$2,$2
	ori	$2,$2,0x1
	seb	$2,$2
	andi	$2,$2,0x00ff
	sb	$2,32($sp)
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,116($2)
	move	$3,$2
	li	$2,316			# 0x13c
	sw	$2,16($sp)
	addiu	$2,$sp,32
	sw	$2,20($sp)
	li	$2,1			# 0x1
	sw	$2,24($sp)
	move	$4,$0
	move	$5,$3
	li	$6,112			# 0x70
	li	$7,2			# 0x2
	lui	$2,%hi(phy_I2C_write)
	addiu	$2,$2,%lo(phy_I2C_write)
	jalr	$2
	nop

	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,116($2)
	move	$3,$2
	li	$2,332			# 0x14c
	sw	$2,16($sp)
	addiu	$2,$sp,32
	sw	$2,20($sp)
	li	$2,1			# 0x1
	sw	$2,24($sp)
	move	$4,$0
	move	$5,$3
	li	$6,112			# 0x70
	li	$7,2			# 0x2
	lui	$2,%hi(phy_I2C_read)
	addiu	$2,$2,%lo(phy_I2C_read)
	jalr	$2
	nop

	lbu	$2,32($sp)
	seb	$3,$2
	li	$2,-4			# 0xfffffffffffffffc
	and	$2,$3,$2
	seb	$2,$2
	ori	$2,$2,0x1
	seb	$2,$2
	andi	$2,$2,0x00ff
	sb	$2,32($sp)
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,116($2)
	move	$3,$2
	li	$2,332			# 0x14c
	sw	$2,16($sp)
	addiu	$2,$sp,32
	sw	$2,20($sp)
	li	$2,1			# 0x1
	sw	$2,24($sp)
	move	$4,$0
	move	$5,$3
	li	$6,112			# 0x70
	li	$7,2			# 0x2
	lui	$2,%hi(phy_I2C_write)
	addiu	$2,$2,%lo(phy_I2C_write)
	jalr	$2
	nop

	lui	$2,%hi($LC185)
	addiu	$4,$2,%lo($LC185)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lw	$31,44($sp)
	addiu	$sp,$sp,48
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	mt7570_DualClosedLoopMode
	.size	mt7570_DualClosedLoopMode, .-mt7570_DualClosedLoopMode
	.rdata
	.align	2
$LC187:
	.ascii	"V_ADC = 0x%x\012\000"
	.section	.text.mt7570_Vtemp_ADC_get,"ax",@progbits
	.align	2
	.globl	mt7570_Vtemp_ADC_get
	.set	nomips16
	.set	nomicromips
	.ent	mt7570_Vtemp_ADC_get
	.type	mt7570_Vtemp_ADC_get, @function
mt7570_Vtemp_ADC_get:
	.frame	$sp,32,$31		# vars= 8, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-32
	sw	$31,28($sp)
	lui	$2,%hi(mt7570_ADC_temperature_get)
	addiu	$2,$2,%lo(mt7570_ADC_temperature_get)
	jalr	$2
	nop

	lui	$2,%hi(global_temperature_code)
	lw	$3,%lo(global_temperature_code)($2)
	lui	$2,%hi(__floatsisf)
	addiu	$2,$2,%lo(__floatsisf)
	move	$4,$3
	jalr	$2
	nop

	move	$4,$2
	lui	$2,%hi(ADC_slope)
	lw	$3,%lo(ADC_slope)($2)
	lui	$2,%hi(__mulsf3)
	addiu	$2,$2,%lo(__mulsf3)
	move	$5,$3
	jalr	$2
	nop

	move	$4,$2
	lui	$2,%hi(ADC_offset)
	lw	$3,%lo(ADC_offset)($2)
	lui	$2,%hi(__addsf3)
	addiu	$2,$2,%lo(__addsf3)
	move	$5,$3
	jalr	$2
	nop

	sw	$2,16($sp)
	lui	$3,%hi($LC186)
	lui	$2,%hi(__mulsf3)
	addiu	$2,$2,%lo(__mulsf3)
	lw	$4,16($sp)
	lw	$5,%lo($LC186)($3)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(__fixunssfsi)
	addiu	$2,$2,%lo(__fixunssfsi)
	move	$4,$3
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi($LC187)
	addiu	$4,$2,%lo($LC187)
	move	$5,$3
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lw	$31,28($sp)
	addiu	$sp,$sp,32
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	mt7570_Vtemp_ADC_get
	.size	mt7570_Vtemp_ADC_get, .-mt7570_Vtemp_ADC_get
	.rdata
	.align	2
$LC188:
	.ascii	"Ibias output lower\012\000"
	.align	2
$LC189:
	.ascii	"ETC\012\000"
	.section	.text.mt7570_ERTemperatureCompensation,"ax",@progbits
	.align	2
	.globl	mt7570_ERTemperatureCompensation
	.set	nomips16
	.set	nomicromips
	.ent	mt7570_ERTemperatureCompensation
	.type	mt7570_ERTemperatureCompensation, @function
mt7570_ERTemperatureCompensation:
	.frame	$sp,80,$31		# vars= 32, regs= 3/0, args= 32, gp= 0
	.mask	0x80030000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-80
	sw	$31,76($sp)
	sw	$17,72($sp)
	sw	$16,68($sp)
	sw	$0,56($sp)
	li	$2,40			# 0x28
	sw	$2,32($sp)
	sw	$0,36($sp)
	sw	$0,40($sp)
	sw	$0,44($sp)
	sw	$0,48($sp)
	sw	$0,52($sp)
	lui	$2,%hi(SCL)
	lw	$3,%lo(SCL)($2)
	li	$2,1			# 0x1
	beq	$3,$2,$L444
	nop

	lui	$2,%hi(DOL)
	lw	$3,%lo(DOL)($2)
	li	$2,1			# 0x1
	bne	$3,$2,$L445
	nop

$L444:
	j	$L443
	nop

$L445:
	lui	$2,%hi(TEC)
	lw	$2,%lo(TEC)($2)
	beq	$2,$0,$L447
	nop

	lui	$2,%hi(TEC_switch)
	lw	$3,%lo(TEC_switch)($2)
	li	$2,1			# 0x1
	bne	$3,$2,$L447
	nop

	lui	$2,%hi(Ibias_outputlow_flag)
	lw	$3,%lo(Ibias_outputlow_flag)($2)
	li	$2,1			# 0x1
	bne	$3,$2,$L448
	nop

	lui	$2,%hi($LC188)
	addiu	$4,$2,%lo($LC188)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	j	$L443
	nop

$L448:
	j	$L449
	nop

$L447:
	lui	$2,%hi(Ibias_outputlow_flag)
	sw	$0,%lo(Ibias_outputlow_flag)($2)
$L449:
	lui	$2,%hi($LC189)
	addiu	$4,$2,%lo($LC189)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	li	$4,96			# 0x60
	lui	$2,%hi(get_flash_register)
	addiu	$2,$2,%lo(get_flash_register)
	jalr	$2
	nop

	move	$3,$2
	li	$2,-1			# 0xffffffffffffffff
	beq	$3,$2,$L450
	nop

	li	$4,96			# 0x60
	lui	$2,%hi(get_flash_register)
	addiu	$2,$2,%lo(get_flash_register)
	jalr	$2
	nop

	andi	$2,$2,0xff
	sw	$2,32($sp)
$L450:
	li	$4,100			# 0x64
	lui	$2,%hi(get_flash_register)
	addiu	$2,$2,%lo(get_flash_register)
	jalr	$2
	nop

	andi	$3,$2,0xff
	li	$2,255			# 0xff
	beq	$3,$2,$L451
	nop

	li	$4,100			# 0x64
	lui	$2,%hi(get_flash_register)
	addiu	$2,$2,%lo(get_flash_register)
	jalr	$2
	nop

	andi	$2,$2,0xff
	sw	$2,40($sp)
$L451:
	li	$4,100			# 0x64
	lui	$2,%hi(get_flash_register)
	addiu	$2,$2,%lo(get_flash_register)
	jalr	$2
	nop

	sra	$2,$2,8
	andi	$3,$2,0xff
	li	$2,255			# 0xff
	beq	$3,$2,$L452
	nop

	li	$4,100			# 0x64
	lui	$2,%hi(get_flash_register)
	addiu	$2,$2,%lo(get_flash_register)
	jalr	$2
	nop

	sra	$2,$2,8
	andi	$2,$2,0xff
	sw	$2,44($sp)
$L452:
	li	$4,100			# 0x64
	lui	$2,%hi(get_flash_register)
	addiu	$2,$2,%lo(get_flash_register)
	jalr	$2
	nop

	sra	$2,$2,16
	andi	$3,$2,0xff
	li	$2,255			# 0xff
	beq	$3,$2,$L453
	nop

	li	$4,100			# 0x64
	lui	$2,%hi(get_flash_register)
	addiu	$2,$2,%lo(get_flash_register)
	jalr	$2
	nop

	sra	$2,$2,16
	andi	$2,$2,0xff
	sw	$2,48($sp)
$L453:
	li	$4,100			# 0x64
	lui	$2,%hi(get_flash_register)
	addiu	$2,$2,%lo(get_flash_register)
	jalr	$2
	nop

	srl	$3,$2,24
	li	$2,255			# 0xff
	beq	$3,$2,$L454
	nop

	li	$4,100			# 0x64
	lui	$2,%hi(get_flash_register)
	addiu	$2,$2,%lo(get_flash_register)
	jalr	$2
	nop

	srl	$2,$2,24
	sw	$2,52($sp)
$L454:
	lui	$2,%hi(IC_temperature)
	lw	$3,%lo(IC_temperature)($2)
	lui	$2,%hi(__fixsfsi)
	addiu	$2,$2,%lo(__fixsfsi)
	move	$4,$3
	jalr	$2
	nop

	move	$3,$2
	lw	$2,32($sp)
	slt	$2,$2,$3
	beq	$2,$0,$L455
	nop

	lw	$3,40($sp)
	lw	$2,44($sp)
	addu	$2,$3,$2
	beq	$2,$0,$L455
	nop

	lui	$2,%hi(IC_temperature)
	lw	$3,%lo(IC_temperature)($2)
	lui	$2,%hi(__fixsfsi)
	addiu	$2,$2,%lo(__fixsfsi)
	move	$4,$3
	jalr	$2
	nop

	move	$3,$2
	lw	$2,32($sp)
	addiu	$2,$2,25
	slt	$2,$2,$3
	beq	$2,$0,$L456
	nop

	lw	$2,40($sp)
	sw	$2,36($sp)
	j	$L457
	nop

$L456:
	lui	$2,%hi(IC_temperature)
	lw	$3,%lo(IC_temperature)($2)
	lui	$2,%hi(__fixsfsi)
	addiu	$2,$2,%lo(__fixsfsi)
	move	$4,$3
	jalr	$2
	nop

	move	$3,$2
	lw	$2,32($sp)
	subu	$3,$3,$2
	lui	$2,%hi(__floatsidf)
	addiu	$2,$2,%lo(__floatsidf)
	move	$4,$3
	jalr	$2
	nop

	move	$17,$3
	move	$16,$2
	lui	$2,%hi(__floatunsisf)
	addiu	$2,$2,%lo(__floatunsisf)
	lw	$4,40($sp)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(__extendsfdf2)
	addiu	$2,$2,%lo(__extendsfdf2)
	move	$4,$3
	jalr	$2
	nop

	move	$5,$3
	move	$4,$2
	lui	$3,%hi($LC190)
	lui	$2,%hi(__divdf3)
	addiu	$2,$2,%lo(__divdf3)
	lw	$7,%lo($LC190+4)($3)
	lw	$6,%lo($LC190)($3)
	jalr	$2
	nop

	move	$7,$3
	move	$6,$2
	lui	$2,%hi(__muldf3)
	addiu	$2,$2,%lo(__muldf3)
	move	$5,$17
	move	$4,$16
	jalr	$2
	nop

	move	$5,$3
	move	$4,$2
	lui	$2,%hi(__fixdfsi)
	addiu	$2,$2,%lo(__fixdfsi)
	jalr	$2
	nop

	sw	$2,36($sp)
$L457:
	li	$4,8			# 0x8
	lui	$2,%hi(get_flash_register)
	addiu	$2,$2,%lo(get_flash_register)
	jalr	$2
	nop

	andi	$2,$2,0x3ff
	sw	$2,56($sp)
	lw	$2,36($sp)
	lw	$3,56($sp)
	addu	$2,$3,$2
	sw	$2,56($sp)
	lw	$2,56($sp)
	andi	$2,$2,0x00ff
	sb	$2,60($sp)
	lw	$2,56($sp)
	srl	$2,$2,8
	andi	$2,$2,0x00ff
	sb	$2,61($sp)
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,116($2)
	move	$3,$2
	li	$2,6			# 0x6
	sw	$2,16($sp)
	addiu	$2,$sp,60
	sw	$2,20($sp)
	li	$2,2			# 0x2
	sw	$2,24($sp)
	move	$4,$0
	move	$5,$3
	li	$6,112			# 0x70
	li	$7,2			# 0x2
	lui	$2,%hi(phy_I2C_write)
	addiu	$2,$2,%lo(phy_I2C_write)
	jalr	$2
	nop

	lui	$2,%hi(IC_temperature)
	lw	$3,%lo(IC_temperature)($2)
	lui	$2,%hi(__fixsfsi)
	addiu	$2,$2,%lo(__fixsfsi)
	move	$4,$3
	jalr	$2
	nop

	move	$3,$2
	lw	$2,32($sp)
	addiu	$2,$2,25
	slt	$2,$2,$3
	beq	$2,$0,$L458
	nop

	lw	$2,44($sp)
	sw	$2,36($sp)
	j	$L459
	nop

$L458:
	lui	$2,%hi(IC_temperature)
	lw	$3,%lo(IC_temperature)($2)
	lui	$2,%hi(__fixsfsi)
	addiu	$2,$2,%lo(__fixsfsi)
	move	$4,$3
	jalr	$2
	nop

	move	$3,$2
	lw	$2,32($sp)
	subu	$3,$3,$2
	lui	$2,%hi(__floatsidf)
	addiu	$2,$2,%lo(__floatsidf)
	move	$4,$3
	jalr	$2
	nop

	move	$17,$3
	move	$16,$2
	lui	$2,%hi(__floatunsisf)
	addiu	$2,$2,%lo(__floatunsisf)
	lw	$4,44($sp)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(__extendsfdf2)
	addiu	$2,$2,%lo(__extendsfdf2)
	move	$4,$3
	jalr	$2
	nop

	move	$5,$3
	move	$4,$2
	lui	$3,%hi($LC190)
	lui	$2,%hi(__divdf3)
	addiu	$2,$2,%lo(__divdf3)
	lw	$7,%lo($LC190+4)($3)
	lw	$6,%lo($LC190)($3)
	jalr	$2
	nop

	move	$7,$3
	move	$6,$2
	lui	$2,%hi(__muldf3)
	addiu	$2,$2,%lo(__muldf3)
	move	$5,$17
	move	$4,$16
	jalr	$2
	nop

	move	$5,$3
	move	$4,$2
	lui	$2,%hi(__fixdfsi)
	addiu	$2,$2,%lo(__fixdfsi)
	jalr	$2
	nop

	sw	$2,36($sp)
$L459:
	li	$4,12			# 0xc
	lui	$2,%hi(get_flash_register)
	addiu	$2,$2,%lo(get_flash_register)
	jalr	$2
	nop

	andi	$2,$2,0x3ff
	sw	$2,56($sp)
	lw	$2,36($sp)
	lw	$3,56($sp)
	addu	$2,$3,$2
	sw	$2,56($sp)
	lw	$2,56($sp)
	andi	$2,$2,0x00ff
	sb	$2,60($sp)
	lw	$2,56($sp)
	srl	$2,$2,8
	andi	$2,$2,0x00ff
	sb	$2,61($sp)
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,116($2)
	move	$3,$2
	li	$2,4			# 0x4
	sw	$2,16($sp)
	addiu	$2,$sp,60
	sw	$2,20($sp)
	li	$2,2			# 0x2
	sw	$2,24($sp)
	move	$4,$0
	move	$5,$3
	li	$6,112			# 0x70
	li	$7,2			# 0x2
	lui	$2,%hi(phy_I2C_write)
	addiu	$2,$2,%lo(phy_I2C_write)
	jalr	$2
	nop

	lui	$2,%hi(mt7570_TxSD_level_set)
	addiu	$2,$2,%lo(mt7570_TxSD_level_set)
	jalr	$2
	nop

	j	$L460
	nop

$L455:
	lui	$2,%hi(IC_temperature)
	lw	$3,%lo(IC_temperature)($2)
	lui	$2,%hi(__fixsfsi)
	addiu	$2,$2,%lo(__fixsfsi)
	move	$4,$3
	jalr	$2
	nop

	move	$3,$2
	lw	$2,32($sp)
	addiu	$2,$2,-20
	slt	$2,$3,$2
	beq	$2,$0,$L461
	nop

	lw	$3,48($sp)
	lw	$2,52($sp)
	addu	$2,$3,$2
	beq	$2,$0,$L461
	nop

	lui	$2,%hi(IC_temperature)
	lw	$3,%lo(IC_temperature)($2)
	lui	$2,%hi(__fixsfsi)
	addiu	$2,$2,%lo(__fixsfsi)
	move	$4,$3
	jalr	$2
	nop

	move	$3,$2
	lw	$2,32($sp)
	addiu	$2,$2,-30
	slt	$2,$3,$2
	beq	$2,$0,$L462
	nop

	lw	$2,48($sp)
	subu	$2,$0,$2
	sw	$2,36($sp)
	j	$L463
	nop

$L462:
	lui	$2,%hi(IC_temperature)
	lw	$3,%lo(IC_temperature)($2)
	lui	$2,%hi(__fixsfsi)
	addiu	$2,$2,%lo(__fixsfsi)
	move	$4,$3
	jalr	$2
	nop

	move	$4,$2
	li	$3,20			# 0x14
	lw	$2,32($sp)
	subu	$2,$3,$2
	addu	$3,$4,$2
	lui	$2,%hi(__floatsidf)
	addiu	$2,$2,%lo(__floatsidf)
	move	$4,$3
	jalr	$2
	nop

	move	$17,$3
	move	$16,$2
	lui	$2,%hi(__floatunsisf)
	addiu	$2,$2,%lo(__floatunsisf)
	lw	$4,48($sp)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(__extendsfdf2)
	addiu	$2,$2,%lo(__extendsfdf2)
	move	$4,$3
	jalr	$2
	nop

	move	$5,$3
	move	$4,$2
	lui	$3,%hi($LC191)
	lui	$2,%hi(__divdf3)
	addiu	$2,$2,%lo(__divdf3)
	lw	$7,%lo($LC191+4)($3)
	lw	$6,%lo($LC191)($3)
	jalr	$2
	nop

	move	$7,$3
	move	$6,$2
	lui	$2,%hi(__muldf3)
	addiu	$2,$2,%lo(__muldf3)
	move	$5,$17
	move	$4,$16
	jalr	$2
	nop

	move	$5,$3
	move	$4,$2
	lui	$2,%hi(__fixdfsi)
	addiu	$2,$2,%lo(__fixdfsi)
	jalr	$2
	nop

	sw	$2,36($sp)
$L463:
	li	$4,8			# 0x8
	lui	$2,%hi(get_flash_register)
	addiu	$2,$2,%lo(get_flash_register)
	jalr	$2
	nop

	andi	$2,$2,0x3ff
	sw	$2,56($sp)
	lw	$2,36($sp)
	lw	$3,56($sp)
	addu	$2,$3,$2
	sw	$2,56($sp)
	lw	$2,56($sp)
	andi	$2,$2,0x00ff
	sb	$2,60($sp)
	lw	$2,56($sp)
	srl	$2,$2,8
	andi	$2,$2,0x00ff
	sb	$2,61($sp)
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,116($2)
	move	$3,$2
	li	$2,6			# 0x6
	sw	$2,16($sp)
	addiu	$2,$sp,60
	sw	$2,20($sp)
	li	$2,2			# 0x2
	sw	$2,24($sp)
	move	$4,$0
	move	$5,$3
	li	$6,112			# 0x70
	li	$7,2			# 0x2
	lui	$2,%hi(phy_I2C_write)
	addiu	$2,$2,%lo(phy_I2C_write)
	jalr	$2
	nop

	lui	$2,%hi(IC_temperature)
	lw	$3,%lo(IC_temperature)($2)
	lui	$2,%hi(__fixsfsi)
	addiu	$2,$2,%lo(__fixsfsi)
	move	$4,$3
	jalr	$2
	nop

	move	$3,$2
	lw	$2,32($sp)
	addiu	$2,$2,-30
	slt	$2,$3,$2
	beq	$2,$0,$L464
	nop

	lw	$2,52($sp)
	subu	$2,$0,$2
	sw	$2,36($sp)
	j	$L465
	nop

$L464:
	lui	$2,%hi(IC_temperature)
	lw	$3,%lo(IC_temperature)($2)
	lui	$2,%hi(__fixsfsi)
	addiu	$2,$2,%lo(__fixsfsi)
	move	$4,$3
	jalr	$2
	nop

	move	$4,$2
	li	$3,20			# 0x14
	lw	$2,32($sp)
	subu	$2,$3,$2
	addu	$3,$4,$2
	lui	$2,%hi(__floatsidf)
	addiu	$2,$2,%lo(__floatsidf)
	move	$4,$3
	jalr	$2
	nop

	move	$17,$3
	move	$16,$2
	lui	$2,%hi(__floatunsisf)
	addiu	$2,$2,%lo(__floatunsisf)
	lw	$4,52($sp)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(__extendsfdf2)
	addiu	$2,$2,%lo(__extendsfdf2)
	move	$4,$3
	jalr	$2
	nop

	move	$5,$3
	move	$4,$2
	lui	$3,%hi($LC191)
	lui	$2,%hi(__divdf3)
	addiu	$2,$2,%lo(__divdf3)
	lw	$7,%lo($LC191+4)($3)
	lw	$6,%lo($LC191)($3)
	jalr	$2
	nop

	move	$7,$3
	move	$6,$2
	lui	$2,%hi(__muldf3)
	addiu	$2,$2,%lo(__muldf3)
	move	$5,$17
	move	$4,$16
	jalr	$2
	nop

	move	$5,$3
	move	$4,$2
	lui	$2,%hi(__fixdfsi)
	addiu	$2,$2,%lo(__fixdfsi)
	jalr	$2
	nop

	sw	$2,36($sp)
$L465:
	li	$4,12			# 0xc
	lui	$2,%hi(get_flash_register)
	addiu	$2,$2,%lo(get_flash_register)
	jalr	$2
	nop

	andi	$2,$2,0x3ff
	sw	$2,56($sp)
	lw	$2,36($sp)
	lw	$3,56($sp)
	addu	$2,$3,$2
	sw	$2,56($sp)
	lw	$2,56($sp)
	andi	$2,$2,0x00ff
	sb	$2,60($sp)
	lw	$2,56($sp)
	srl	$2,$2,8
	andi	$2,$2,0x00ff
	sb	$2,61($sp)
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,116($2)
	move	$3,$2
	li	$2,4			# 0x4
	sw	$2,16($sp)
	addiu	$2,$sp,60
	sw	$2,20($sp)
	li	$2,2			# 0x2
	sw	$2,24($sp)
	move	$4,$0
	move	$5,$3
	li	$6,112			# 0x70
	li	$7,2			# 0x2
	lui	$2,%hi(phy_I2C_write)
	addiu	$2,$2,%lo(phy_I2C_write)
	jalr	$2
	nop

	lui	$2,%hi(mt7570_TxSD_level_set)
	addiu	$2,$2,%lo(mt7570_TxSD_level_set)
	jalr	$2
	nop

	j	$L460
	nop

$L461:
	li	$4,12			# 0xc
	lui	$2,%hi(get_flash_register)
	addiu	$2,$2,%lo(get_flash_register)
	jalr	$2
	nop

	andi	$16,$2,0x3ff
	li	$4,8			# 0x8
	lui	$2,%hi(get_flash_register)
	addiu	$2,$2,%lo(get_flash_register)
	jalr	$2
	nop

	andi	$2,$2,0x3ff
	sll	$2,$2,16
	or	$2,$16,$2
	sw	$2,56($sp)
	lw	$2,56($sp)
	andi	$2,$2,0x00ff
	sb	$2,60($sp)
	lw	$2,56($sp)
	srl	$2,$2,8
	andi	$2,$2,0x00ff
	sb	$2,61($sp)
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,116($2)
	move	$3,$2
	li	$2,6			# 0x6
	sw	$2,16($sp)
	addiu	$2,$sp,60
	sw	$2,20($sp)
	li	$2,2			# 0x2
	sw	$2,24($sp)
	move	$4,$0
	move	$5,$3
	li	$6,112			# 0x70
	li	$7,2			# 0x2
	lui	$2,%hi(phy_I2C_write)
	addiu	$2,$2,%lo(phy_I2C_write)
	jalr	$2
	nop

	lw	$2,56($sp)
	srl	$2,$2,16
	andi	$2,$2,0x00ff
	sb	$2,62($sp)
	lw	$2,56($sp)
	srl	$2,$2,24
	andi	$2,$2,0x00ff
	sb	$2,63($sp)
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,116($2)
	move	$3,$2
	li	$2,4			# 0x4
	sw	$2,16($sp)
	addiu	$2,$sp,60
	sw	$2,20($sp)
	li	$2,4			# 0x4
	sw	$2,24($sp)
	move	$4,$0
	move	$5,$3
	li	$6,112			# 0x70
	li	$7,2			# 0x2
	lui	$2,%hi(phy_I2C_write)
	addiu	$2,$2,%lo(phy_I2C_write)
	jalr	$2
	nop

	lui	$2,%hi(mt7570_TxSD_level_set)
	addiu	$2,$2,%lo(mt7570_TxSD_level_set)
	jalr	$2
	nop

	j	$L443
	nop

$L460:
$L443:
	lw	$31,76($sp)
	lw	$17,72($sp)
	lw	$16,68($sp)
	addiu	$sp,$sp,80
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	mt7570_ERTemperatureCompensation
	.size	mt7570_ERTemperatureCompensation, .-mt7570_ERTemperatureCompensation
	.rdata
	.align	2
$LC192:
	.ascii	"SOL wrong flash setting \012\000"
	.align	2
$LC193:
	.ascii	"SOL Ibias equal to 0 , not correct \012\000"
	.align	2
$LC194:
	.ascii	"turning point range error\012\000"
	.align	2
$LC195:
	.ascii	"Calibration tempature too lower , Wrong setting \012\000"
	.align	2
$LC196:
	.ascii	"SOL mode, Ibias = %d.%.2dmA\000"
	.section	.text.SOL_mt7570_ERTemperatureCompensation,"ax",@progbits
	.align	2
	.globl	SOL_mt7570_ERTemperatureCompensation
	.set	nomips16
	.set	nomicromips
	.ent	SOL_mt7570_ERTemperatureCompensation
	.type	SOL_mt7570_ERTemperatureCompensation, @function
SOL_mt7570_ERTemperatureCompensation:
	.frame	$sp,144,$31		# vars= 96, regs= 3/0, args= 32, gp= 0
	.mask	0x80030000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-144
	sw	$31,140($sp)
	sw	$17,136($sp)
	sw	$16,132($sp)
	sw	$0,32($sp)
	li	$2,40			# 0x28
	sw	$2,36($sp)
	sw	$0,40($sp)
	move	$2,$0
	sw	$2,96($sp)
	sw	$0,44($sp)
	sw	$0,48($sp)
	sw	$0,52($sp)
	sw	$0,56($sp)
	sw	$0,60($sp)
	sw	$0,64($sp)
	sw	$0,100($sp)
	sw	$0,68($sp)
	sw	$0,72($sp)
	sw	$0,76($sp)
	sw	$0,80($sp)
	sw	$0,84($sp)
	sw	$0,88($sp)
	sw	$0,104($sp)
	sw	$0,108($sp)
	sw	$0,92($sp)
	li	$4,96			# 0x60
	lui	$2,%hi(get_flash_register)
	addiu	$2,$2,%lo(get_flash_register)
	jalr	$2
	nop

	move	$16,$2
	li	$4,112			# 0x70
	lui	$2,%hi(get_flash_register)
	addiu	$2,$2,%lo(get_flash_register)
	jalr	$2
	nop

	or	$16,$16,$2
	li	$4,116			# 0x74
	lui	$2,%hi(get_flash_register)
	addiu	$2,$2,%lo(get_flash_register)
	jalr	$2
	nop

	or	$16,$16,$2
	li	$4,12			# 0xc
	lui	$2,%hi(get_flash_register)
	addiu	$2,$2,%lo(get_flash_register)
	jalr	$2
	nop

	or	$3,$16,$2
	li	$2,-1			# 0xffffffffffffffff
	bne	$3,$2,$L468
	nop

	lui	$2,%hi($LC192)
	addiu	$4,$2,%lo($LC192)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	j	$L467
	nop

$L468:
	li	$4,96			# 0x60
	lui	$2,%hi(get_flash_register)
	addiu	$2,$2,%lo(get_flash_register)
	jalr	$2
	nop

	move	$3,$2
	li	$2,-1			# 0xffffffffffffffff
	beq	$3,$2,$L470
	nop

	li	$4,96			# 0x60
	lui	$2,%hi(get_flash_register)
	addiu	$2,$2,%lo(get_flash_register)
	jalr	$2
	nop

	andi	$2,$2,0xff
	sw	$2,36($sp)
$L470:
	li	$4,112			# 0x70
	lui	$2,%hi(get_flash_register)
	addiu	$2,$2,%lo(get_flash_register)
	jalr	$2
	nop

	andi	$3,$2,0xff
	li	$2,255			# 0xff
	beq	$3,$2,$L471
	nop

	li	$4,112			# 0x70
	lui	$2,%hi(get_flash_register)
	addiu	$2,$2,%lo(get_flash_register)
	jalr	$2
	nop

	andi	$2,$2,0xff
	sw	$2,44($sp)
$L471:
	li	$4,112			# 0x70
	lui	$2,%hi(get_flash_register)
	addiu	$2,$2,%lo(get_flash_register)
	jalr	$2
	nop

	sra	$2,$2,8
	andi	$3,$2,0xff
	li	$2,255			# 0xff
	beq	$3,$2,$L472
	nop

	li	$4,112			# 0x70
	lui	$2,%hi(get_flash_register)
	addiu	$2,$2,%lo(get_flash_register)
	jalr	$2
	nop

	sra	$2,$2,8
	andi	$2,$2,0xff
	sw	$2,48($sp)
$L472:
	li	$4,112			# 0x70
	lui	$2,%hi(get_flash_register)
	addiu	$2,$2,%lo(get_flash_register)
	jalr	$2
	nop

	sra	$2,$2,16
	andi	$3,$2,0xff
	li	$2,255			# 0xff
	beq	$3,$2,$L473
	nop

	li	$4,112			# 0x70
	lui	$2,%hi(get_flash_register)
	addiu	$2,$2,%lo(get_flash_register)
	jalr	$2
	nop

	sra	$2,$2,16
	andi	$2,$2,0xff
	sw	$2,52($sp)
$L473:
	li	$4,116			# 0x74
	lui	$2,%hi(get_flash_register)
	addiu	$2,$2,%lo(get_flash_register)
	jalr	$2
	nop

	andi	$3,$2,0xff
	li	$2,255			# 0xff
	beq	$3,$2,$L474
	nop

	li	$4,116			# 0x74
	lui	$2,%hi(get_flash_register)
	addiu	$2,$2,%lo(get_flash_register)
	jalr	$2
	nop

	andi	$2,$2,0xff
	sw	$2,68($sp)
$L474:
	li	$4,116			# 0x74
	lui	$2,%hi(get_flash_register)
	addiu	$2,$2,%lo(get_flash_register)
	jalr	$2
	nop

	sra	$2,$2,8
	andi	$3,$2,0xff
	li	$2,255			# 0xff
	beq	$3,$2,$L475
	nop

	li	$4,116			# 0x74
	lui	$2,%hi(get_flash_register)
	addiu	$2,$2,%lo(get_flash_register)
	jalr	$2
	nop

	sra	$2,$2,8
	andi	$2,$2,0xff
	sw	$2,72($sp)
$L475:
	li	$4,116			# 0x74
	lui	$2,%hi(get_flash_register)
	addiu	$2,$2,%lo(get_flash_register)
	jalr	$2
	nop

	sra	$2,$2,16
	andi	$3,$2,0xff
	li	$2,255			# 0xff
	beq	$3,$2,$L476
	nop

	li	$4,116			# 0x74
	lui	$2,%hi(get_flash_register)
	addiu	$2,$2,%lo(get_flash_register)
	jalr	$2
	nop

	sra	$2,$2,16
	andi	$2,$2,0xff
	sw	$2,76($sp)
$L476:
	li	$4,116			# 0x74
	lui	$2,%hi(get_flash_register)
	addiu	$2,$2,%lo(get_flash_register)
	jalr	$2
	nop

	srl	$3,$2,24
	li	$2,255			# 0xff
	beq	$3,$2,$L477
	nop

	li	$4,116			# 0x74
	lui	$2,%hi(get_flash_register)
	addiu	$2,$2,%lo(get_flash_register)
	jalr	$2
	nop

	srl	$2,$2,24
	sw	$2,80($sp)
$L477:
	li	$4,96			# 0x60
	lui	$2,%hi(get_flash_register)
	addiu	$2,$2,%lo(get_flash_register)
	jalr	$2
	nop

	sra	$2,$2,16
	andi	$3,$2,0xfff
	li	$2,4095			# 0xfff
	beq	$3,$2,$L478
	nop

	li	$4,96			# 0x60
	lui	$2,%hi(get_flash_register)
	addiu	$2,$2,%lo(get_flash_register)
	jalr	$2
	nop

	sra	$2,$2,16
	andi	$2,$2,0xfff
	sw	$2,100($sp)
	li	$4,120			# 0x78
	lui	$2,%hi(get_flash_register)
	addiu	$2,$2,%lo(get_flash_register)
	jalr	$2
	nop

	andi	$3,$2,0xff
	li	$2,255			# 0xff
	beq	$3,$2,$L481
	nop

	j	$L509
	nop

$L478:
	lui	$2,%hi($LC193)
	addiu	$4,$2,%lo($LC193)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	j	$L467
	nop

$L509:
	li	$4,120			# 0x78
	lui	$2,%hi(get_flash_register)
	addiu	$2,$2,%lo(get_flash_register)
	jalr	$2
	nop

	andi	$2,$2,0xff
	sw	$2,88($sp)
$L481:
	li	$4,120			# 0x78
	lui	$2,%hi(get_flash_register)
	addiu	$2,$2,%lo(get_flash_register)
	jalr	$2
	nop

	sra	$2,$2,16
	andi	$3,$2,0xff
	li	$2,255			# 0xff
	beq	$3,$2,$L482
	nop

	li	$4,120			# 0x78
	lui	$2,%hi(get_flash_register)
	addiu	$2,$2,%lo(get_flash_register)
	jalr	$2
	nop

	sra	$2,$2,16
	andi	$2,$2,0xff
	sw	$2,84($sp)
$L482:
	lui	$2,%hi(IC_temperature)
	lw	$3,%lo(IC_temperature)($2)
	lui	$2,%hi(__fixsfsi)
	addiu	$2,$2,%lo(__fixsfsi)
	move	$4,$3
	jalr	$2
	nop

	move	$3,$2
	lw	$2,36($sp)
	slt	$2,$2,$3
	beq	$2,$0,$L483
	nop

	lui	$2,%hi(IC_temperature)
	lw	$3,%lo(IC_temperature)($2)
	lui	$2,%hi(__fixsfsi)
	addiu	$2,$2,%lo(__fixsfsi)
	move	$4,$3
	jalr	$2
	nop

	slt	$2,$2,61
	bne	$2,$0,$L484
	nop

	lw	$2,84($sp)
	sw	$2,40($sp)
	j	$L485
	nop

$L484:
	lui	$2,%hi(IC_temperature)
	lw	$3,%lo(IC_temperature)($2)
	lui	$2,%hi(__fixsfsi)
	addiu	$2,$2,%lo(__fixsfsi)
	move	$4,$3
	jalr	$2
	nop

	move	$3,$2
	lw	$2,36($sp)
	subu	$3,$3,$2
	lui	$2,%hi(__floatsisf)
	addiu	$2,$2,%lo(__floatsisf)
	move	$4,$3
	jalr	$2
	nop

	move	$16,$2
	lui	$2,%hi(__floatunsisf)
	addiu	$2,$2,%lo(__floatunsisf)
	lw	$4,84($sp)
	jalr	$2
	nop

	move	$17,$2
	li	$3,60			# 0x3c
	lw	$2,36($sp)
	subu	$3,$3,$2
	lui	$2,%hi(__floatsisf)
	addiu	$2,$2,%lo(__floatsisf)
	move	$4,$3
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(__divsf3)
	addiu	$2,$2,%lo(__divsf3)
	move	$4,$17
	move	$5,$3
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(__mulsf3)
	addiu	$2,$2,%lo(__mulsf3)
	move	$4,$16
	move	$5,$3
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(__fixsfsi)
	addiu	$2,$2,%lo(__fixsfsi)
	move	$4,$3
	jalr	$2
	nop

	sw	$2,40($sp)
$L485:
	li	$4,12			# 0xc
	lui	$2,%hi(get_flash_register)
	addiu	$2,$2,%lo(get_flash_register)
	jalr	$2
	nop

	andi	$2,$2,0x3ff
	sw	$2,32($sp)
	lw	$2,40($sp)
	lw	$3,32($sp)
	addu	$2,$3,$2
	sw	$2,32($sp)
	lw	$2,32($sp)
	andi	$2,$2,0x00ff
	sb	$2,124($sp)
	lw	$2,32($sp)
	srl	$2,$2,8
	andi	$2,$2,0x00ff
	sb	$2,125($sp)
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,116($2)
	move	$3,$2
	li	$2,4			# 0x4
	sw	$2,16($sp)
	addiu	$2,$sp,124
	sw	$2,20($sp)
	li	$2,2			# 0x2
	sw	$2,24($sp)
	move	$4,$0
	move	$5,$3
	li	$6,112			# 0x70
	li	$7,2			# 0x2
	lui	$2,%hi(phy_I2C_write)
	addiu	$2,$2,%lo(phy_I2C_write)
	jalr	$2
	nop

	lui	$2,%hi(mt7570_TxSD_level_set)
	addiu	$2,$2,%lo(mt7570_TxSD_level_set)
	jalr	$2
	nop

	j	$L486
	nop

$L483:
	lui	$2,%hi(IC_temperature)
	lw	$3,%lo(IC_temperature)($2)
	lui	$2,%hi(__fixsfsi)
	addiu	$2,$2,%lo(__fixsfsi)
	move	$4,$3
	jalr	$2
	nop

	move	$3,$2
	lw	$2,36($sp)
	slt	$2,$3,$2
	beq	$2,$0,$L487
	nop

	li	$4,12			# 0xc
	lui	$2,%hi(get_flash_register)
	addiu	$2,$2,%lo(get_flash_register)
	jalr	$2
	nop

	andi	$2,$2,0x3ff
	sw	$2,32($sp)
	lui	$2,%hi(IC_temperature)
	lw	$3,%lo(IC_temperature)($2)
	lui	$2,%hi(__fixsfsi)
	addiu	$2,$2,%lo(__fixsfsi)
	move	$4,$3
	jalr	$2
	nop

	slt	$2,$2,-10
	beq	$2,$0,$L488
	nop

	lw	$2,88($sp)
	sw	$2,40($sp)
	lw	$2,40($sp)
	lw	$3,32($sp)
	subu	$2,$3,$2
	sw	$2,32($sp)
	j	$L489
	nop

$L488:
	lui	$2,%hi(IC_temperature)
	lw	$3,%lo(IC_temperature)($2)
	lui	$2,%hi(__fixsfsi)
	addiu	$2,$2,%lo(__fixsfsi)
	move	$4,$3
	jalr	$2
	nop

	bgez	$2,$L490
	nop

	lui	$2,%hi(IC_temperature)
	lw	$3,%lo(IC_temperature)($2)
	lui	$2,%hi(__fixsfsi)
	addiu	$2,$2,%lo(__fixsfsi)
	move	$4,$3
	jalr	$2
	nop

	sw	$2,112($sp)
	lw	$2,112($sp)
	bgez	$2,$L491
	nop

	subu	$2,$0,$2
$L491:
	move	$3,$2
	lw	$2,36($sp)
	addu	$3,$3,$2
	lui	$2,%hi(__floatsisf)
	addiu	$2,$2,%lo(__floatsisf)
	move	$4,$3
	jalr	$2
	nop

	move	$16,$2
	lui	$2,%hi(__floatunsisf)
	addiu	$2,$2,%lo(__floatunsisf)
	lw	$4,88($sp)
	jalr	$2
	nop

	move	$17,$2
	lw	$2,36($sp)
	addiu	$3,$2,10
	lui	$2,%hi(__floatsisf)
	addiu	$2,$2,%lo(__floatsisf)
	move	$4,$3
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(__divsf3)
	addiu	$2,$2,%lo(__divsf3)
	move	$4,$17
	move	$5,$3
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(__mulsf3)
	addiu	$2,$2,%lo(__mulsf3)
	move	$4,$16
	move	$5,$3
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(__fixsfsi)
	addiu	$2,$2,%lo(__fixsfsi)
	move	$4,$3
	jalr	$2
	nop

	sw	$2,40($sp)
	lw	$2,40($sp)
	lw	$3,32($sp)
	subu	$2,$3,$2
	sw	$2,32($sp)
	j	$L489
	nop

$L490:
	lui	$2,%hi(IC_temperature)
	lw	$3,%lo(IC_temperature)($2)
	lui	$2,%hi(__fixsfsi)
	addiu	$2,$2,%lo(__fixsfsi)
	move	$4,$3
	jalr	$2
	nop

	move	$3,$2
	lw	$2,36($sp)
	subu	$3,$3,$2
	lui	$2,%hi(__floatsisf)
	addiu	$2,$2,%lo(__floatsisf)
	move	$4,$3
	jalr	$2
	nop

	move	$16,$2
	lui	$2,%hi(__floatunsisf)
	addiu	$2,$2,%lo(__floatunsisf)
	lw	$4,88($sp)
	jalr	$2
	nop

	move	$17,$2
	lw	$2,36($sp)
	addiu	$3,$2,10
	lui	$2,%hi(__floatsisf)
	addiu	$2,$2,%lo(__floatsisf)
	move	$4,$3
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(__divsf3)
	addiu	$2,$2,%lo(__divsf3)
	move	$4,$17
	move	$5,$3
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(__mulsf3)
	addiu	$2,$2,%lo(__mulsf3)
	move	$4,$16
	move	$5,$3
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(__fixsfsi)
	addiu	$2,$2,%lo(__fixsfsi)
	move	$4,$3
	jalr	$2
	nop

	sw	$2,40($sp)
	lw	$2,40($sp)
	lw	$3,32($sp)
	addu	$2,$3,$2
	sw	$2,32($sp)
$L489:
	lw	$2,32($sp)
	andi	$2,$2,0x00ff
	sb	$2,124($sp)
	lw	$2,32($sp)
	srl	$2,$2,8
	andi	$2,$2,0x00ff
	sb	$2,125($sp)
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,116($2)
	move	$3,$2
	li	$2,4			# 0x4
	sw	$2,16($sp)
	addiu	$2,$sp,124
	sw	$2,20($sp)
	li	$2,2			# 0x2
	sw	$2,24($sp)
	move	$4,$0
	move	$5,$3
	li	$6,112			# 0x70
	li	$7,2			# 0x2
	lui	$2,%hi(phy_I2C_write)
	addiu	$2,$2,%lo(phy_I2C_write)
	jalr	$2
	nop

	lui	$2,%hi(mt7570_TxSD_level_set)
	addiu	$2,$2,%lo(mt7570_TxSD_level_set)
	jalr	$2
	nop

	j	$L486
	nop

$L487:
	li	$4,12			# 0xc
	lui	$2,%hi(get_flash_register)
	addiu	$2,$2,%lo(get_flash_register)
	jalr	$2
	nop

	andi	$2,$2,0x3ff
	sw	$2,32($sp)
	lw	$2,32($sp)
	andi	$2,$2,0x00ff
	sb	$2,124($sp)
	lw	$2,32($sp)
	srl	$2,$2,8
	andi	$2,$2,0x00ff
	sb	$2,125($sp)
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,116($2)
	move	$3,$2
	li	$2,4			# 0x4
	sw	$2,16($sp)
	addiu	$2,$sp,124
	sw	$2,20($sp)
	li	$2,2			# 0x2
	sw	$2,24($sp)
	move	$4,$0
	move	$5,$3
	li	$6,112			# 0x70
	li	$7,2			# 0x2
	lui	$2,%hi(phy_I2C_write)
	addiu	$2,$2,%lo(phy_I2C_write)
	jalr	$2
	nop

	lui	$2,%hi(mt7570_TxSD_level_set)
	addiu	$2,$2,%lo(mt7570_TxSD_level_set)
	jalr	$2
	nop

$L486:
	lw	$3,48($sp)
	lw	$2,44($sp)
	slt	$2,$3,$2
	bne	$2,$0,$L492
	nop

	lw	$3,48($sp)
	lw	$2,52($sp)
	slt	$2,$2,$3
	beq	$2,$0,$L493
	nop

$L492:
	lui	$2,%hi($LC194)
	addiu	$4,$2,%lo($LC194)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	j	$L467
	nop

$L493:
	lw	$3,36($sp)
	lw	$2,48($sp)
	slt	$2,$2,$3
	beq	$2,$0,$L494
	nop

	lw	$3,36($sp)
	lw	$2,52($sp)
	slt	$2,$3,$2
	beq	$2,$0,$L494
	nop

	lw	$3,36($sp)
	lw	$2,48($sp)
	subu	$2,$3,$2
	move	$3,$2
	lw	$2,76($sp)
	mul	$2,$3,$2
	lw	$3,100($sp)
	subu	$2,$3,$2
	sw	$2,60($sp)
	lw	$3,52($sp)
	lw	$2,36($sp)
	subu	$2,$3,$2
	move	$3,$2
	lw	$2,76($sp)
	mul	$3,$3,$2
	lw	$2,100($sp)
	addu	$2,$3,$2
	sw	$2,64($sp)
	lw	$3,60($sp)
	lw	$4,48($sp)
	lw	$2,44($sp)
	subu	$2,$4,$2
	move	$4,$2
	lw	$2,72($sp)
	mul	$2,$4,$2
	subu	$2,$3,$2
	sw	$2,56($sp)
	j	$L495
	nop

$L494:
	lw	$3,48($sp)
	lw	$2,36($sp)
	slt	$2,$2,$3
	beq	$2,$0,$L496
	nop

	lw	$3,36($sp)
	lw	$2,44($sp)
	slt	$2,$2,$3
	beq	$2,$0,$L496
	nop

	lw	$3,48($sp)
	lw	$2,36($sp)
	subu	$2,$3,$2
	move	$3,$2
	lw	$2,72($sp)
	mul	$3,$3,$2
	lw	$2,100($sp)
	addu	$2,$3,$2
	sw	$2,60($sp)
	lw	$3,36($sp)
	lw	$2,44($sp)
	subu	$2,$3,$2
	move	$3,$2
	lw	$2,72($sp)
	mul	$2,$3,$2
	lw	$3,100($sp)
	subu	$2,$3,$2
	sw	$2,56($sp)
	lw	$3,52($sp)
	lw	$2,48($sp)
	subu	$2,$3,$2
	move	$3,$2
	lw	$2,76($sp)
	mul	$3,$3,$2
	lw	$2,60($sp)
	addu	$2,$3,$2
	sw	$2,64($sp)
	j	$L495
	nop

$L496:
	lw	$3,48($sp)
	lw	$2,36($sp)
	bne	$3,$2,$L497
	nop

	lw	$2,100($sp)
	sw	$2,60($sp)
	lw	$3,60($sp)
	lw	$4,48($sp)
	lw	$2,44($sp)
	subu	$2,$4,$2
	move	$4,$2
	lw	$2,72($sp)
	mul	$2,$4,$2
	subu	$2,$3,$2
	sw	$2,56($sp)
	lw	$3,52($sp)
	lw	$2,48($sp)
	subu	$2,$3,$2
	move	$3,$2
	lw	$2,76($sp)
	mul	$3,$3,$2
	lw	$2,60($sp)
	addu	$2,$3,$2
	sw	$2,64($sp)
	j	$L495
	nop

$L497:
	lw	$3,52($sp)
	lw	$2,36($sp)
	bne	$3,$2,$L498
	nop

	lw	$2,100($sp)
	sw	$2,64($sp)
	lw	$3,52($sp)
	lw	$2,48($sp)
	subu	$2,$3,$2
	move	$3,$2
	lw	$2,76($sp)
	mul	$2,$3,$2
	lw	$3,100($sp)
	subu	$2,$3,$2
	sw	$2,60($sp)
	lw	$3,60($sp)
	lw	$4,48($sp)
	lw	$2,44($sp)
	subu	$2,$4,$2
	move	$4,$2
	lw	$2,72($sp)
	mul	$2,$4,$2
	subu	$2,$3,$2
	sw	$2,56($sp)
	j	$L495
	nop

$L498:
	lw	$3,36($sp)
	lw	$2,52($sp)
	slt	$2,$2,$3
	beq	$2,$0,$L499
	nop

	lw	$3,36($sp)
	lw	$2,52($sp)
	subu	$2,$3,$2
	move	$3,$2
	lw	$2,80($sp)
	mul	$2,$3,$2
	lw	$3,100($sp)
	subu	$2,$3,$2
	sw	$2,64($sp)
	lw	$3,52($sp)
	lw	$2,48($sp)
	subu	$2,$3,$2
	move	$3,$2
	lw	$2,76($sp)
	mul	$2,$3,$2
	lw	$3,100($sp)
	subu	$2,$3,$2
	sw	$2,60($sp)
	lw	$3,60($sp)
	lw	$4,48($sp)
	lw	$2,44($sp)
	subu	$2,$4,$2
	move	$4,$2
	lw	$2,72($sp)
	mul	$2,$4,$2
	subu	$2,$3,$2
	sw	$2,56($sp)
	j	$L495
	nop

$L499:
	lw	$3,36($sp)
	lw	$2,44($sp)
	slt	$2,$2,$3
	bne	$2,$0,$L495
	nop

	lui	$2,%hi($LC195)
	addiu	$4,$2,%lo($LC195)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	j	$L467
	nop

$L495:
	lui	$2,%hi(IC_temperature)
	lw	$3,%lo(IC_temperature)($2)
	lui	$2,%hi(__fixsfsi)
	addiu	$2,$2,%lo(__fixsfsi)
	move	$4,$3
	jalr	$2
	nop

	move	$3,$2
	lw	$2,52($sp)
	slt	$2,$3,$2
	bne	$2,$0,$L500
	nop

	lui	$2,%hi(IC_temperature)
	lw	$3,%lo(IC_temperature)($2)
	lui	$2,%hi(__fixsfsi)
	addiu	$2,$2,%lo(__fixsfsi)
	move	$4,$3
	jalr	$2
	nop

	move	$3,$2
	lw	$2,52($sp)
	subu	$2,$3,$2
	move	$3,$2
	lw	$2,80($sp)
	mul	$3,$3,$2
	lw	$2,64($sp)
	addu	$2,$3,$2
	sw	$2,92($sp)
	j	$L501
	nop

$L500:
	lui	$2,%hi(IC_temperature)
	lw	$3,%lo(IC_temperature)($2)
	lui	$2,%hi(__fixsfsi)
	addiu	$2,$2,%lo(__fixsfsi)
	move	$4,$3
	jalr	$2
	nop

	move	$3,$2
	lw	$2,48($sp)
	slt	$2,$3,$2
	xori	$2,$2,0x1
	andi	$16,$2,0x00ff
	lui	$2,%hi(IC_temperature)
	lw	$3,%lo(IC_temperature)($2)
	lui	$2,%hi(__fixsfsi)
	addiu	$2,$2,%lo(__fixsfsi)
	move	$4,$3
	jalr	$2
	nop

	move	$3,$2
	lw	$2,52($sp)
	slt	$2,$3,$2
	andi	$2,$2,0x00ff
	and	$2,$16,$2
	andi	$2,$2,0x00ff
	beq	$2,$0,$L502
	nop

	lui	$2,%hi(IC_temperature)
	lw	$3,%lo(IC_temperature)($2)
	lui	$2,%hi(__fixsfsi)
	addiu	$2,$2,%lo(__fixsfsi)
	move	$4,$3
	jalr	$2
	nop

	move	$3,$2
	lw	$2,48($sp)
	subu	$2,$3,$2
	move	$3,$2
	lw	$2,76($sp)
	mul	$3,$3,$2
	lw	$2,60($sp)
	addu	$2,$3,$2
	sw	$2,92($sp)
	j	$L501
	nop

$L502:
	lui	$2,%hi(IC_temperature)
	lw	$3,%lo(IC_temperature)($2)
	lui	$2,%hi(__fixsfsi)
	addiu	$2,$2,%lo(__fixsfsi)
	move	$4,$3
	jalr	$2
	nop

	move	$3,$2
	lw	$2,48($sp)
	slt	$2,$3,$2
	andi	$16,$2,0x00ff
	lui	$2,%hi(IC_temperature)
	lw	$3,%lo(IC_temperature)($2)
	lui	$2,%hi(__fixsfsi)
	addiu	$2,$2,%lo(__fixsfsi)
	move	$4,$3
	jalr	$2
	nop

	move	$3,$2
	lw	$2,44($sp)
	slt	$2,$3,$2
	xori	$2,$2,0x1
	andi	$2,$2,0x00ff
	and	$2,$16,$2
	andi	$2,$2,0x00ff
	beq	$2,$0,$L503
	nop

	lui	$2,%hi(IC_temperature)
	lw	$3,%lo(IC_temperature)($2)
	lui	$2,%hi(__fixsfsi)
	addiu	$2,$2,%lo(__fixsfsi)
	move	$4,$3
	jalr	$2
	nop

	move	$3,$2
	lw	$2,44($sp)
	subu	$2,$3,$2
	move	$3,$2
	lw	$2,72($sp)
	mul	$3,$3,$2
	lw	$2,56($sp)
	addu	$2,$3,$2
	sw	$2,92($sp)
	j	$L501
	nop

$L503:
	lui	$2,%hi(IC_temperature)
	lw	$3,%lo(IC_temperature)($2)
	lui	$2,%hi(__fixsfsi)
	addiu	$2,$2,%lo(__fixsfsi)
	move	$4,$3
	jalr	$2
	nop

	move	$3,$2
	lw	$2,44($sp)
	slt	$2,$3,$2
	beq	$2,$0,$L501
	nop

	lui	$2,%hi(IC_temperature)
	lw	$3,%lo(IC_temperature)($2)
	lui	$2,%hi(__fixsfsi)
	addiu	$2,$2,%lo(__fixsfsi)
	move	$4,$3
	jalr	$2
	nop

	bgez	$2,$L504
	nop

	lw	$16,56($sp)
	lui	$2,%hi(IC_temperature)
	lw	$3,%lo(IC_temperature)($2)
	lui	$2,%hi(__fixsfsi)
	addiu	$2,$2,%lo(__fixsfsi)
	move	$4,$3
	jalr	$2
	nop

	sw	$2,116($sp)
	lw	$2,116($sp)
	bgez	$2,$L505
	nop

	subu	$2,$0,$2
$L505:
	move	$3,$2
	lw	$2,44($sp)
	addu	$2,$3,$2
	move	$3,$2
	lw	$2,68($sp)
	mul	$2,$3,$2
	subu	$2,$16,$2
	sw	$2,92($sp)
	j	$L501
	nop

$L504:
	lui	$2,%hi(IC_temperature)
	lw	$3,%lo(IC_temperature)($2)
	lui	$2,%hi(__fixsfsi)
	addiu	$2,$2,%lo(__fixsfsi)
	move	$4,$3
	jalr	$2
	nop

	move	$3,$2
	lw	$2,44($sp)
	subu	$2,$3,$2
	move	$3,$2
	lw	$2,68($sp)
	mul	$3,$3,$2
	lw	$2,56($sp)
	addu	$2,$3,$2
	sw	$2,92($sp)
$L501:
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,116($2)
	move	$3,$2
	li	$2,316			# 0x13c
	sw	$2,16($sp)
	addiu	$2,$sp,124
	sw	$2,20($sp)
	li	$2,4			# 0x4
	sw	$2,24($sp)
	move	$4,$0
	move	$5,$3
	li	$6,112			# 0x70
	li	$7,2			# 0x2
	lui	$2,%hi(phy_I2C_read)
	addiu	$2,$2,%lo(phy_I2C_read)
	jalr	$2
	nop

	lbu	$2,124($sp)
	seb	$3,$2
	li	$2,-4			# 0xfffffffffffffffc
	and	$2,$3,$2
	seb	$2,$2
	ori	$2,$2,0x2
	seb	$2,$2
	andi	$2,$2,0x00ff
	sb	$2,124($sp)
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,116($2)
	move	$3,$2
	li	$2,316			# 0x13c
	sw	$2,16($sp)
	addiu	$2,$sp,124
	sw	$2,20($sp)
	li	$2,4			# 0x4
	sw	$2,24($sp)
	move	$4,$0
	move	$5,$3
	li	$6,112			# 0x70
	li	$7,2			# 0x2
	lui	$2,%hi(phy_I2C_write)
	addiu	$2,$2,%lo(phy_I2C_write)
	jalr	$2
	nop

	lw	$2,92($sp)
	slt	$2,$2,37
	beq	$2,$0,$L506
	nop

	li	$2,37			# 0x25
	sw	$2,92($sp)
$L506:
	lw	$2,92($sp)
	sw	$2,32($sp)
	lw	$2,32($sp)
	andi	$2,$2,0x00ff
	sb	$2,124($sp)
	lw	$2,32($sp)
	srl	$2,$2,8
	andi	$2,$2,0x00ff
	sb	$2,125($sp)
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,116($2)
	move	$3,$2
	li	$2,312			# 0x138
	sw	$2,16($sp)
	addiu	$2,$sp,124
	sw	$2,20($sp)
	li	$2,2			# 0x2
	sw	$2,24($sp)
	move	$4,$0
	move	$5,$3
	li	$6,112			# 0x70
	li	$7,2			# 0x2
	lui	$2,%hi(phy_I2C_write)
	addiu	$2,$2,%lo(phy_I2C_write)
	jalr	$2
	nop

	lui	$2,%hi(global_bias_current)
	lw	$3,%lo(global_bias_current)($2)
	lui	$2,%hi(__floatunsisf)
	addiu	$2,$2,%lo(__floatunsisf)
	move	$4,$3
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(__extendsfdf2)
	addiu	$2,$2,%lo(__extendsfdf2)
	move	$4,$3
	jalr	$2
	nop

	move	$5,$3
	move	$4,$2
	lui	$3,%hi($LC107)
	lui	$2,%hi(__muldf3)
	addiu	$2,$2,%lo(__muldf3)
	lw	$7,%lo($LC107+4)($3)
	lw	$6,%lo($LC107)($3)
	jalr	$2
	nop

	move	$5,$3
	move	$4,$2
	lui	$2,%hi(__truncdfsf2)
	addiu	$2,$2,%lo(__truncdfsf2)
	jalr	$2
	nop

	sw	$2,96($sp)
	lui	$2,%hi(__fixsfsi)
	addiu	$2,$2,%lo(__fixsfsi)
	lw	$4,96($sp)
	jalr	$2
	nop

	sw	$2,104($sp)
	lui	$3,%hi($LC84)
	lui	$2,%hi(__mulsf3)
	addiu	$2,$2,%lo(__mulsf3)
	lw	$4,96($sp)
	lw	$5,%lo($LC84)($3)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(__fixsfsi)
	addiu	$2,$2,%lo(__fixsfsi)
	move	$4,$3
	jalr	$2
	nop

	move	$3,$2
	li	$2,1374355456			# 0x51eb0000
	ori	$2,$2,0x851f
	mult	$3,$2
	mfhi	$2
	sra	$4,$2,5
	sra	$2,$3,31
	subu	$2,$4,$2
	sll	$2,$2,2
	sll	$4,$2,2
	addu	$2,$2,$4
	sll	$4,$2,2
	addu	$2,$2,$4
	subu	$2,$3,$2
	sw	$2,108($sp)
	lui	$2,%hi($LC196)
	addiu	$4,$2,%lo($LC196)
	lw	$5,104($sp)
	lw	$6,108($sp)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi($LC26)
	addiu	$4,$2,%lo($LC26)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi(IC_temperature)
	lw	$3,%lo(IC_temperature)($2)
	lui	$2,%hi(__fixsfsi)
	addiu	$2,$2,%lo(__fixsfsi)
	move	$4,$3
	jalr	$2
	nop

	sw	$2,104($sp)
	lui	$2,%hi(IC_temperature)
	lw	$4,%lo(IC_temperature)($2)
	lui	$3,%hi($LC16)
	lui	$2,%hi(__mulsf3)
	addiu	$2,$2,%lo(__mulsf3)
	lw	$5,%lo($LC16)($3)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(__fixsfsi)
	addiu	$2,$2,%lo(__fixsfsi)
	move	$4,$3
	jalr	$2
	nop

	move	$3,$2
	li	$2,1717960704			# 0x66660000
	ori	$2,$2,0x6667
	mult	$3,$2
	mfhi	$2
	sra	$4,$2,2
	sra	$2,$3,31
	subu	$2,$4,$2
	sll	$2,$2,1
	sll	$4,$2,2
	addu	$2,$2,$4
	subu	$2,$3,$2
	sw	$2,120($sp)
	lw	$2,120($sp)
	bgez	$2,$L507
	nop

	subu	$2,$0,$2
$L507:
	sw	$2,108($sp)
	lui	$2,%hi($LC164)
	addiu	$4,$2,%lo($LC164)
	lw	$5,104($sp)
	lw	$6,108($sp)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L467:
	lw	$31,140($sp)
	lw	$17,136($sp)
	lw	$16,132($sp)
	addiu	$sp,$sp,144
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	SOL_mt7570_ERTemperatureCompensation
	.size	SOL_mt7570_ERTemperatureCompensation, .-SOL_mt7570_ERTemperatureCompensation
	.section	.text.mt7570_FiberPlug_Protect,"ax",@progbits
	.align	2
	.globl	mt7570_FiberPlug_Protect
	.set	nomips16
	.set	nomicromips
	.ent	mt7570_FiberPlug_Protect
	.type	mt7570_FiberPlug_Protect, @function
mt7570_FiberPlug_Protect:
	.frame	$sp,24,$31		# vars= 0, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-24
	sw	$31,20($sp)
	lui	$2,%hi(mt7570_restart_ERC)
	addiu	$2,$2,%lo(mt7570_restart_ERC)
	jalr	$2
	nop

	lw	$31,20($sp)
	addiu	$sp,$sp,24
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	mt7570_FiberPlug_Protect
	.size	mt7570_FiberPlug_Protect, .-mt7570_FiberPlug_Protect
	.rdata
	.align	2
$LC197:
	.ascii	"[%lu]0ms[%s:%d]\012  P0 ERC restarted\012\000"
	.section	.text.mt7570_restart_ERC_P0,"ax",@progbits
	.align	2
	.globl	mt7570_restart_ERC_P0
	.set	nomips16
	.set	nomicromips
	.ent	mt7570_restart_ERC_P0
	.type	mt7570_restart_ERC_P0, @function
mt7570_restart_ERC_P0:
	.frame	$sp,48,$31		# vars= 8, regs= 2/0, args= 32, gp= 0
	.mask	0x80010000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-48
	sw	$31,44($sp)
	sw	$16,40($sp)
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,116($2)
	move	$3,$2
	li	$2,316			# 0x13c
	sw	$2,16($sp)
	addiu	$2,$sp,32
	sw	$2,20($sp)
	li	$2,4			# 0x4
	sw	$2,24($sp)
	move	$4,$0
	move	$5,$3
	li	$6,112			# 0x70
	li	$7,2			# 0x2
	lui	$2,%hi(phy_I2C_read)
	addiu	$2,$2,%lo(phy_I2C_read)
	jalr	$2
	nop

	lbu	$3,32($sp)
	li	$2,-2			# 0xfffffffffffffffe
	and	$2,$3,$2
	andi	$2,$2,0x00ff
	sb	$2,32($sp)
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,116($2)
	move	$3,$2
	li	$2,316			# 0x13c
	sw	$2,16($sp)
	addiu	$2,$sp,32
	sw	$2,20($sp)
	li	$2,4			# 0x4
	sw	$2,24($sp)
	move	$4,$0
	move	$5,$3
	li	$6,112			# 0x70
	li	$7,2			# 0x2
	lui	$2,%hi(phy_I2C_write)
	addiu	$2,$2,%lo(phy_I2C_write)
	jalr	$2
	nop

	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,116($2)
	move	$3,$2
	li	$2,316			# 0x13c
	sw	$2,16($sp)
	addiu	$2,$sp,32
	sw	$2,20($sp)
	li	$2,4			# 0x4
	sw	$2,24($sp)
	move	$4,$0
	move	$5,$3
	li	$6,112			# 0x70
	li	$7,2			# 0x2
	lui	$2,%hi(phy_I2C_read)
	addiu	$2,$2,%lo(phy_I2C_read)
	jalr	$2
	nop

	lbu	$2,32($sp)
	ori	$2,$2,0x1
	andi	$2,$2,0x00ff
	sb	$2,32($sp)
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,116($2)
	move	$3,$2
	li	$2,316			# 0x13c
	sw	$2,16($sp)
	addiu	$2,$sp,32
	sw	$2,20($sp)
	li	$2,4			# 0x4
	sw	$2,24($sp)
	move	$4,$0
	move	$5,$3
	li	$6,112			# 0x70
	li	$7,2			# 0x2
	lui	$2,%hi(phy_I2C_write)
	addiu	$2,$2,%lo(phy_I2C_write)
	jalr	$2
	nop

	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,136($2)
	andi	$2,$2,0x10
	beq	$2,$0,$L511
	nop

	lui	$2,%hi(jiffies)
	lw	$16,%lo(jiffies)($2)
	lui	$2,%hi($LC37)
	addiu	$4,$2,%lo($LC37)
	li	$5,47			# 0x2f
	lui	$2,%hi(strrchr)
	addiu	$2,$2,%lo(strrchr)
	jalr	$2
	nop

	addiu	$3,$2,1
	lui	$2,%hi($LC197)
	addiu	$4,$2,%lo($LC197)
	move	$5,$16
	move	$6,$3
	li	$7,4298			# 0x10ca
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L511:
	lw	$31,44($sp)
	lw	$16,40($sp)
	addiu	$sp,$sp,48
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	mt7570_restart_ERC_P0
	.size	mt7570_restart_ERC_P0, .-mt7570_restart_ERC_P0
	.rdata
	.align	2
$LC198:
	.ascii	"[%lu]0ms[%s:%d]\012  Target bias loaded, 0x%x\012\000"
	.align	2
$LC199:
	.ascii	"[%lu]0ms[%s:%d]\012  Target Ibias = %d.%.2dmA\012\000"
	.align	2
$LC200:
	.ascii	"[%lu]0ms[%s:%d]\012  Bias tracking done\012\000"
	.section	.text.mt7570_BiasTracking,"ax",@progbits
	.align	2
	.globl	mt7570_BiasTracking
	.set	nomips16
	.set	nomicromips
	.ent	mt7570_BiasTracking
	.type	mt7570_BiasTracking, @function
mt7570_BiasTracking:
	.frame	$sp,72,$31		# vars= 32, regs= 2/0, args= 32, gp= 0
	.mask	0x80010000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-72
	sw	$31,68($sp)
	sw	$16,64($sp)
	sw	$0,36($sp)
	sw	$0,40($sp)
	sw	$0,44($sp)
	sw	$0,32($sp)
	sw	$0,48($sp)
	sw	$0,52($sp)
	move	$2,$0
	sw	$2,56($sp)
	lui	$2,%hi(Env_temperature)
	lw	$4,%lo(Env_temperature)($2)
	lui	$3,%hi($LC97)
	lui	$2,%hi(__addsf3)
	addiu	$2,$2,%lo(__addsf3)
	lw	$5,%lo($LC97)($3)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(__extendsfdf2)
	addiu	$2,$2,%lo(__extendsfdf2)
	move	$4,$3
	jalr	$2
	nop

	move	$5,$3
	move	$4,$2
	lui	$3,%hi($LC98)
	lui	$2,%hi(__divdf3)
	addiu	$2,$2,%lo(__divdf3)
	lw	$7,%lo($LC98+4)($3)
	lw	$6,%lo($LC98)($3)
	jalr	$2
	nop

	move	$5,$3
	move	$4,$2
	lui	$2,%hi(__fixdfsi)
	addiu	$2,$2,%lo(__fixdfsi)
	jalr	$2
	nop

	sw	$2,32($sp)
	lw	$2,32($sp)
	bgez	$2,$L514
	nop

	sw	$0,32($sp)
$L514:
	lui	$2,%hi(LUT_Ibias_Imod)
	lw	$3,32($sp)
	sll	$3,$3,3
	addiu	$2,$2,%lo(LUT_Ibias_Imod)
	addu	$2,$3,$2
	lw	$2,0($2)
	sw	$2,36($sp)
	lw	$3,36($sp)
	li	$2,4095			# 0xfff
	beq	$3,$2,$L513
	nop

	lw	$2,36($sp)
	andi	$2,$2,0x00ff
	sb	$2,60($sp)
	lw	$2,36($sp)
	srl	$2,$2,8
	andi	$2,$2,0x00ff
	sb	$2,61($sp)
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,116($2)
	move	$3,$2
	li	$2,312			# 0x138
	sw	$2,16($sp)
	addiu	$2,$sp,60
	sw	$2,20($sp)
	li	$2,2			# 0x2
	sw	$2,24($sp)
	move	$4,$0
	move	$5,$3
	li	$6,112			# 0x70
	li	$7,2			# 0x2
	lui	$2,%hi(phy_I2C_write)
	addiu	$2,$2,%lo(phy_I2C_write)
	jalr	$2
	nop

	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,136($2)
	andi	$2,$2,0x10
	beq	$2,$0,$L517
	nop

	lui	$2,%hi(jiffies)
	lw	$16,%lo(jiffies)($2)
	lui	$2,%hi($LC37)
	addiu	$4,$2,%lo($LC37)
	li	$5,47			# 0x2f
	lui	$2,%hi(strrchr)
	addiu	$2,$2,%lo(strrchr)
	jalr	$2
	nop

	addiu	$3,$2,1
	lw	$2,36($sp)
	sw	$2,16($sp)
	lui	$2,%hi($LC198)
	addiu	$4,$2,%lo($LC198)
	move	$5,$16
	move	$6,$3
	li	$7,4343			# 0x10f7
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L517:
	lui	$2,%hi(__floatunsisf)
	addiu	$2,$2,%lo(__floatunsisf)
	lw	$4,36($sp)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(__extendsfdf2)
	addiu	$2,$2,%lo(__extendsfdf2)
	move	$4,$3
	jalr	$2
	nop

	move	$5,$3
	move	$4,$2
	lui	$3,%hi($LC107)
	lui	$2,%hi(__muldf3)
	addiu	$2,$2,%lo(__muldf3)
	lw	$7,%lo($LC107+4)($3)
	lw	$6,%lo($LC107)($3)
	jalr	$2
	nop

	move	$5,$3
	move	$4,$2
	lui	$2,%hi(__truncdfsf2)
	addiu	$2,$2,%lo(__truncdfsf2)
	jalr	$2
	nop

	sw	$2,56($sp)
	lui	$2,%hi(__fixsfsi)
	addiu	$2,$2,%lo(__fixsfsi)
	lw	$4,56($sp)
	jalr	$2
	nop

	sw	$2,48($sp)
	lui	$3,%hi($LC84)
	lui	$2,%hi(__mulsf3)
	addiu	$2,$2,%lo(__mulsf3)
	lw	$4,56($sp)
	lw	$5,%lo($LC84)($3)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(__fixsfsi)
	addiu	$2,$2,%lo(__fixsfsi)
	move	$4,$3
	jalr	$2
	nop

	move	$3,$2
	li	$2,1374355456			# 0x51eb0000
	ori	$2,$2,0x851f
	mult	$3,$2
	mfhi	$2
	sra	$4,$2,5
	sra	$2,$3,31
	subu	$2,$4,$2
	sll	$2,$2,2
	sll	$4,$2,2
	addu	$2,$2,$4
	sll	$4,$2,2
	addu	$2,$2,$4
	subu	$2,$3,$2
	sw	$2,52($sp)
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,136($2)
	andi	$2,$2,0x10
	beq	$2,$0,$L518
	nop

	lui	$2,%hi(jiffies)
	lw	$16,%lo(jiffies)($2)
	lui	$2,%hi($LC37)
	addiu	$4,$2,%lo($LC37)
	li	$5,47			# 0x2f
	lui	$2,%hi(strrchr)
	addiu	$2,$2,%lo(strrchr)
	jalr	$2
	nop

	addiu	$3,$2,1
	lw	$2,48($sp)
	sw	$2,16($sp)
	lw	$2,52($sp)
	sw	$2,20($sp)
	lui	$2,%hi($LC199)
	addiu	$4,$2,%lo($LC199)
	move	$5,$16
	move	$6,$3
	li	$7,4347			# 0x10fb
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L518:
	lui	$2,%hi(global_bias_current)
	lw	$2,%lo(global_bias_current)($2)
	sw	$2,44($sp)
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,116($2)
	move	$3,$2
	li	$2,316			# 0x13c
	sw	$2,16($sp)
	addiu	$2,$sp,60
	sw	$2,20($sp)
	li	$2,1			# 0x1
	sw	$2,24($sp)
	move	$4,$0
	move	$5,$3
	li	$6,112			# 0x70
	li	$7,2			# 0x2
	lui	$2,%hi(phy_I2C_read)
	addiu	$2,$2,%lo(phy_I2C_read)
	jalr	$2
	nop

	lbu	$2,60($sp)
	andi	$3,$2,0xf
	li	$2,5			# 0x5
	bne	$3,$2,$L513
	nop

	lw	$3,36($sp)
	lw	$2,44($sp)
	sltu	$2,$2,$3
	beq	$2,$0,$L520
	nop

	lw	$3,36($sp)
	lw	$2,44($sp)
	subu	$2,$3,$2
	sltu	$2,$2,21
	bne	$2,$0,$L513
	nop

	li	$4,5			# 0x5
	lui	$2,%hi(mt7570_information_output)
	addiu	$2,$2,%lo(mt7570_information_output)
	jalr	$2
	nop

	addiu	$2,$2,4
	sw	$2,40($sp)
	lw	$2,40($sp)
	andi	$2,$2,0x00ff
	sb	$2,60($sp)
	lw	$2,40($sp)
	srl	$2,$2,8
	andi	$2,$2,0x00ff
	sb	$2,61($sp)
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,116($2)
	move	$3,$2
	li	$2,6			# 0x6
	sw	$2,16($sp)
	addiu	$2,$sp,60
	sw	$2,20($sp)
	li	$2,2			# 0x2
	sw	$2,24($sp)
	move	$4,$0
	move	$5,$3
	li	$6,112			# 0x70
	li	$7,2			# 0x2
	lui	$2,%hi(phy_I2C_write)
	addiu	$2,$2,%lo(phy_I2C_write)
	jalr	$2
	nop

	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,136($2)
	andi	$2,$2,0x10
	beq	$2,$0,$L513
	nop

	lui	$2,%hi(jiffies)
	lw	$16,%lo(jiffies)($2)
	lui	$2,%hi($LC37)
	addiu	$4,$2,%lo($LC37)
	li	$5,47			# 0x2f
	lui	$2,%hi(strrchr)
	addiu	$2,$2,%lo(strrchr)
	jalr	$2
	nop

	addiu	$3,$2,1
	lui	$2,%hi($LC200)
	addiu	$4,$2,%lo($LC200)
	move	$5,$16
	move	$6,$3
	li	$7,4364			# 0x110c
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	j	$L513
	nop

$L520:
	lw	$3,36($sp)
	lw	$2,44($sp)
	sltu	$2,$3,$2
	beq	$2,$0,$L513
	nop

	lw	$3,44($sp)
	lw	$2,36($sp)
	subu	$2,$3,$2
	sltu	$2,$2,21
	bne	$2,$0,$L513
	nop

	li	$4,5			# 0x5
	lui	$2,%hi(mt7570_information_output)
	addiu	$2,$2,%lo(mt7570_information_output)
	jalr	$2
	nop

	addiu	$2,$2,-4
	sw	$2,40($sp)
	lw	$2,40($sp)
	andi	$2,$2,0x00ff
	sb	$2,60($sp)
	lw	$2,40($sp)
	srl	$2,$2,8
	andi	$2,$2,0x00ff
	sb	$2,61($sp)
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,116($2)
	move	$3,$2
	li	$2,6			# 0x6
	sw	$2,16($sp)
	addiu	$2,$sp,60
	sw	$2,20($sp)
	li	$2,2			# 0x2
	sw	$2,24($sp)
	move	$4,$0
	move	$5,$3
	li	$6,112			# 0x70
	li	$7,2			# 0x2
	lui	$2,%hi(phy_I2C_write)
	addiu	$2,$2,%lo(phy_I2C_write)
	jalr	$2
	nop

	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,136($2)
	andi	$2,$2,0x10
	beq	$2,$0,$L513
	nop

	lui	$2,%hi(jiffies)
	lw	$16,%lo(jiffies)($2)
	lui	$2,%hi($LC37)
	addiu	$4,$2,%lo($LC37)
	li	$5,47			# 0x2f
	lui	$2,%hi(strrchr)
	addiu	$2,$2,%lo(strrchr)
	jalr	$2
	nop

	addiu	$3,$2,1
	lui	$2,%hi($LC200)
	addiu	$4,$2,%lo($LC200)
	move	$5,$16
	move	$6,$3
	li	$7,4375			# 0x1117
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L513:
	lw	$31,68($sp)
	lw	$16,64($sp)
	addiu	$sp,$sp,72
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	mt7570_BiasTracking
	.size	mt7570_BiasTracking, .-mt7570_BiasTracking
	.rdata
	.align	2
$LC202:
	.ascii	"Lack of bias-current table\012\000"
	.align	2
$LC203:
	.ascii	"Bias offset = 0x%x\012\000"
	.align	2
$LC204:
	.ascii	"LUT recover failed\012\000"
	.section	.text.mt7570_LUT_recover,"ax",@progbits
	.align	2
	.globl	mt7570_LUT_recover
	.set	nomips16
	.set	nomicromips
	.ent	mt7570_LUT_recover
	.type	mt7570_LUT_recover, @function
mt7570_LUT_recover:
	.frame	$sp,88,$31		# vars= 56, regs= 3/0, args= 16, gp= 0
	.mask	0x80030000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-88
	sw	$31,84($sp)
	sw	$17,80($sp)
	sw	$16,76($sp)
	sw	$0,36($sp)
	sw	$0,40($sp)
	sw	$0,44($sp)
	sw	$0,48($sp)
	sw	$0,52($sp)
	sw	$0,56($sp)
	sw	$0,16($sp)
	sw	$0,20($sp)
	sw	$0,60($sp)
	sw	$0,24($sp)
	sw	$0,28($sp)
	lui	$2,%hi($LC86)
	lw	$2,%lo($LC86)($2)
	sw	$2,32($sp)
	move	$2,$0
	sw	$2,64($sp)
	lui	$2,%hi(DOL)
	lw	$3,%lo(DOL)($2)
	li	$2,1			# 0x1
	bne	$3,$2,$L523
	nop

	sw	$0,28($sp)
	j	$L524
	nop

$L537:
	lui	$2,%hi(__floatsisf)
	addiu	$2,$2,%lo(__floatsisf)
	lw	$4,28($sp)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(__extendsfdf2)
	addiu	$2,$2,%lo(__extendsfdf2)
	move	$4,$3
	jalr	$2
	nop

	move	$5,$3
	move	$4,$2
	lui	$3,%hi($LC98)
	lui	$2,%hi(__muldf3)
	addiu	$2,$2,%lo(__muldf3)
	lw	$7,%lo($LC98+4)($3)
	lw	$6,%lo($LC98)($3)
	jalr	$2
	nop

	move	$5,$3
	move	$4,$2
	lui	$3,%hi($LC201)
	lui	$2,%hi(__subdf3)
	addiu	$2,$2,%lo(__subdf3)
	lw	$7,%lo($LC201+4)($3)
	lw	$6,%lo($LC201)($3)
	jalr	$2
	nop

	move	$5,$3
	move	$4,$2
	lui	$2,%hi(__truncdfsf2)
	addiu	$2,$2,%lo(__truncdfsf2)
	jalr	$2
	nop

	sw	$2,64($sp)
	lui	$3,%hi($LC97)
	lui	$2,%hi(__addsf3)
	addiu	$2,$2,%lo(__addsf3)
	lw	$4,64($sp)
	lw	$5,%lo($LC97)($3)
	jalr	$2
	nop

	move	$4,$2
	lui	$3,%hi($LC16)
	lui	$2,%hi(__divsf3)
	addiu	$2,$2,%lo(__divsf3)
	lw	$5,%lo($LC16)($3)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(__fixsfsi)
	addiu	$2,$2,%lo(__fixsfsi)
	move	$4,$3
	jalr	$2
	nop

	sw	$2,24($sp)
	lw	$2,24($sp)
	bgez	$2,$L525
	nop

	sw	$0,24($sp)
	j	$L526
	nop

$L525:
	lw	$2,24($sp)
	slt	$2,$2,16
	bne	$2,$0,$L526
	nop

	li	$2,15			# 0xf
	sw	$2,24($sp)
$L526:
	lw	$2,24($sp)
	addiu	$2,$2,40
	sll	$2,$2,2
	move	$4,$2
	lui	$2,%hi(get_flash_register)
	addiu	$2,$2,%lo(get_flash_register)
	jalr	$2
	nop

	move	$3,$2
	li	$2,268369920			# 0xfff0000
	and	$2,$3,$2
	sra	$2,$2,16
	sw	$2,40($sp)
	lw	$2,24($sp)
	addiu	$2,$2,41
	sll	$2,$2,2
	move	$4,$2
	lui	$2,%hi(get_flash_register)
	addiu	$2,$2,%lo(get_flash_register)
	jalr	$2
	nop

	move	$3,$2
	li	$2,268369920			# 0xfff0000
	and	$2,$3,$2
	sra	$2,$2,16
	sw	$2,44($sp)
	lw	$2,24($sp)
	addiu	$2,$2,40
	sll	$2,$2,2
	move	$4,$2
	lui	$2,%hi(get_flash_register)
	addiu	$2,$2,%lo(get_flash_register)
	jalr	$2
	nop

	andi	$2,$2,0xfff
	sw	$2,48($sp)
	lw	$2,24($sp)
	addiu	$2,$2,41
	sll	$2,$2,2
	move	$4,$2
	lui	$2,%hi(get_flash_register)
	addiu	$2,$2,%lo(get_flash_register)
	jalr	$2
	nop

	andi	$2,$2,0xfff
	sw	$2,52($sp)
	lw	$3,40($sp)
	li	$2,4095			# 0xfff
	beq	$3,$2,$L527
	nop

	lw	$3,44($sp)
	li	$2,4095			# 0xfff
	bne	$3,$2,$L528
	nop

$L527:
	lui	$2,%hi(LUT_Ibias_Imod)
	lw	$3,28($sp)
	sll	$3,$3,3
	addiu	$2,$2,%lo(LUT_Ibias_Imod)
	addu	$2,$3,$2
	li	$3,4095			# 0xfff
	sw	$3,0($2)
	j	$L529
	nop

$L528:
	lw	$2,24($sp)
	slt	$2,$2,4
	beq	$2,$0,$L530
	nop

	lui	$2,%hi(__floatunsisf)
	addiu	$2,$2,%lo(__floatunsisf)
	lw	$4,40($sp)
	jalr	$2
	nop

	move	$16,$2
	lui	$3,%hi($LC97)
	lui	$2,%hi(__addsf3)
	addiu	$2,$2,%lo(__addsf3)
	lw	$4,64($sp)
	lw	$5,%lo($LC97)($3)
	jalr	$2
	nop

	move	$17,$2
	lw	$2,24($sp)
	sll	$2,$2,1
	sll	$3,$2,2
	addu	$3,$2,$3
	lui	$2,%hi(__floatsisf)
	addiu	$2,$2,%lo(__floatsisf)
	move	$4,$3
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(__subsf3)
	addiu	$2,$2,%lo(__subsf3)
	move	$4,$17
	move	$5,$3
	jalr	$2
	nop

	move	$17,$2
	lw	$3,44($sp)
	lw	$2,40($sp)
	subu	$3,$3,$2
	lui	$2,%hi(__floatunsisf)
	addiu	$2,$2,%lo(__floatunsisf)
	move	$4,$3
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(__mulsf3)
	addiu	$2,$2,%lo(__mulsf3)
	move	$4,$17
	move	$5,$3
	jalr	$2
	nop

	move	$4,$2
	lui	$3,%hi($LC16)
	lui	$2,%hi(__divsf3)
	addiu	$2,$2,%lo(__divsf3)
	lw	$5,%lo($LC16)($3)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(__addsf3)
	addiu	$2,$2,%lo(__addsf3)
	move	$4,$16
	move	$5,$3
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(__fixunssfsi)
	addiu	$2,$2,%lo(__fixunssfsi)
	move	$4,$3
	jalr	$2
	nop

	sw	$2,16($sp)
	j	$L531
	nop

$L530:
	lui	$2,%hi(__floatunsisf)
	addiu	$2,$2,%lo(__floatunsisf)
	lw	$4,40($sp)
	jalr	$2
	nop

	move	$16,$2
	lw	$2,24($sp)
	sll	$2,$2,1
	sll	$3,$2,2
	addu	$2,$2,$3
	addiu	$3,$2,-40
	lui	$2,%hi(__floatsisf)
	addiu	$2,$2,%lo(__floatsisf)
	move	$4,$3
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(__subsf3)
	addiu	$2,$2,%lo(__subsf3)
	lw	$4,64($sp)
	move	$5,$3
	jalr	$2
	nop

	move	$17,$2
	lw	$3,44($sp)
	lw	$2,40($sp)
	subu	$3,$3,$2
	lui	$2,%hi(__floatunsisf)
	addiu	$2,$2,%lo(__floatunsisf)
	move	$4,$3
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(__mulsf3)
	addiu	$2,$2,%lo(__mulsf3)
	move	$4,$17
	move	$5,$3
	jalr	$2
	nop

	move	$4,$2
	lui	$3,%hi($LC16)
	lui	$2,%hi(__divsf3)
	addiu	$2,$2,%lo(__divsf3)
	lw	$5,%lo($LC16)($3)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(__addsf3)
	addiu	$2,$2,%lo(__addsf3)
	move	$4,$16
	move	$5,$3
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(__fixunssfsi)
	addiu	$2,$2,%lo(__fixunssfsi)
	move	$4,$3
	jalr	$2
	nop

	sw	$2,16($sp)
$L531:
	lui	$2,%hi(LUT_Ibias_Imod)
	lw	$3,28($sp)
	sll	$3,$3,3
	addiu	$2,$2,%lo(LUT_Ibias_Imod)
	addu	$2,$3,$2
	lw	$3,16($sp)
	sw	$3,0($2)
$L529:
	lw	$3,48($sp)
	li	$2,4095			# 0xfff
	beq	$3,$2,$L532
	nop

	lw	$3,52($sp)
	li	$2,4095			# 0xfff
	bne	$3,$2,$L533
	nop

$L532:
	lui	$2,%hi(LUT_Ibias_Imod)
	lw	$3,28($sp)
	sll	$3,$3,3
	addiu	$2,$2,%lo(LUT_Ibias_Imod)
	addu	$2,$3,$2
	li	$3,4095			# 0xfff
	sw	$3,4($2)
	j	$L534
	nop

$L533:
	lw	$2,24($sp)
	slt	$2,$2,4
	beq	$2,$0,$L535
	nop

	lui	$2,%hi(__floatunsisf)
	addiu	$2,$2,%lo(__floatunsisf)
	lw	$4,48($sp)
	jalr	$2
	nop

	move	$16,$2
	lui	$3,%hi($LC97)
	lui	$2,%hi(__addsf3)
	addiu	$2,$2,%lo(__addsf3)
	lw	$4,64($sp)
	lw	$5,%lo($LC97)($3)
	jalr	$2
	nop

	move	$17,$2
	lw	$2,24($sp)
	sll	$2,$2,1
	sll	$3,$2,2
	addu	$3,$2,$3
	lui	$2,%hi(__floatsisf)
	addiu	$2,$2,%lo(__floatsisf)
	move	$4,$3
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(__subsf3)
	addiu	$2,$2,%lo(__subsf3)
	move	$4,$17
	move	$5,$3
	jalr	$2
	nop

	move	$17,$2
	lw	$3,52($sp)
	lw	$2,48($sp)
	subu	$3,$3,$2
	lui	$2,%hi(__floatunsisf)
	addiu	$2,$2,%lo(__floatunsisf)
	move	$4,$3
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(__mulsf3)
	addiu	$2,$2,%lo(__mulsf3)
	move	$4,$17
	move	$5,$3
	jalr	$2
	nop

	move	$4,$2
	lui	$3,%hi($LC16)
	lui	$2,%hi(__divsf3)
	addiu	$2,$2,%lo(__divsf3)
	lw	$5,%lo($LC16)($3)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(__addsf3)
	addiu	$2,$2,%lo(__addsf3)
	move	$4,$16
	move	$5,$3
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(__fixunssfsi)
	addiu	$2,$2,%lo(__fixunssfsi)
	move	$4,$3
	jalr	$2
	nop

	sw	$2,20($sp)
	j	$L536
	nop

$L535:
	lui	$2,%hi(__floatunsisf)
	addiu	$2,$2,%lo(__floatunsisf)
	lw	$4,48($sp)
	jalr	$2
	nop

	move	$16,$2
	lw	$2,24($sp)
	sll	$2,$2,1
	sll	$3,$2,2
	addu	$2,$2,$3
	addiu	$3,$2,-40
	lui	$2,%hi(__floatsisf)
	addiu	$2,$2,%lo(__floatsisf)
	move	$4,$3
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(__subsf3)
	addiu	$2,$2,%lo(__subsf3)
	lw	$4,64($sp)
	move	$5,$3
	jalr	$2
	nop

	move	$17,$2
	lw	$3,52($sp)
	lw	$2,48($sp)
	subu	$3,$3,$2
	lui	$2,%hi(__floatunsisf)
	addiu	$2,$2,%lo(__floatunsisf)
	move	$4,$3
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(__mulsf3)
	addiu	$2,$2,%lo(__mulsf3)
	move	$4,$17
	move	$5,$3
	jalr	$2
	nop

	move	$4,$2
	lui	$3,%hi($LC16)
	lui	$2,%hi(__divsf3)
	addiu	$2,$2,%lo(__divsf3)
	lw	$5,%lo($LC16)($3)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(__addsf3)
	addiu	$2,$2,%lo(__addsf3)
	move	$4,$16
	move	$5,$3
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(__fixunssfsi)
	addiu	$2,$2,%lo(__fixunssfsi)
	move	$4,$3
	jalr	$2
	nop

	sw	$2,20($sp)
$L536:
	lui	$2,%hi(LUT_Ibias_Imod)
	lw	$3,28($sp)
	sll	$3,$3,3
	addiu	$2,$2,%lo(LUT_Ibias_Imod)
	addu	$2,$3,$2
	lw	$3,20($sp)
	sw	$3,4($2)
$L534:
	lw	$2,28($sp)
	addiu	$2,$2,1
	sw	$2,28($sp)
$L524:
	lw	$2,28($sp)
	slt	$2,$2,64
	bne	$2,$0,$L537
	nop

	sw	$0,24($sp)
	j	$L538
	nop

$L541:
	lw	$2,24($sp)
	sll	$2,$2,2
	sw	$2,28($sp)
	lw	$2,24($sp)
	addiu	$2,$2,40
	sll	$2,$2,2
	move	$4,$2
	lui	$2,%hi(get_flash_register)
	addiu	$2,$2,%lo(get_flash_register)
	jalr	$2
	nop

	move	$3,$2
	li	$2,268369920			# 0xfff0000
	and	$2,$3,$2
	sra	$3,$2,16
	li	$2,4095			# 0xfff
	beq	$3,$2,$L539
	nop

	lw	$2,24($sp)
	addiu	$2,$2,40
	sll	$2,$2,2
	move	$4,$2
	lui	$2,%hi(get_flash_register)
	addiu	$2,$2,%lo(get_flash_register)
	jalr	$2
	nop

	move	$3,$2
	li	$2,268369920			# 0xfff0000
	and	$2,$3,$2
	sra	$2,$2,16
	move	$4,$2
	lui	$2,%hi(LUT_Ibias_Imod)
	lw	$3,28($sp)
	sll	$3,$3,3
	addiu	$2,$2,%lo(LUT_Ibias_Imod)
	addu	$2,$3,$2
	sw	$4,0($2)
$L539:
	lw	$2,24($sp)
	addiu	$2,$2,40
	sll	$2,$2,2
	move	$4,$2
	lui	$2,%hi(get_flash_register)
	addiu	$2,$2,%lo(get_flash_register)
	jalr	$2
	nop

	andi	$3,$2,0xfff
	li	$2,4095			# 0xfff
	beq	$3,$2,$L540
	nop

	lw	$2,24($sp)
	addiu	$2,$2,40
	sll	$2,$2,2
	move	$4,$2
	lui	$2,%hi(get_flash_register)
	addiu	$2,$2,%lo(get_flash_register)
	jalr	$2
	nop

	andi	$3,$2,0xfff
	lui	$2,%hi(LUT_Ibias_Imod)
	lw	$4,28($sp)
	sll	$4,$4,3
	addiu	$2,$2,%lo(LUT_Ibias_Imod)
	addu	$2,$4,$2
	sw	$3,4($2)
$L540:
	lw	$2,24($sp)
	addiu	$2,$2,1
	sw	$2,24($sp)
$L538:
	lw	$2,24($sp)
	slt	$2,$2,16
	bne	$2,$0,$L541
	nop

	j	$L522
	nop

$L523:
	li	$4,96			# 0x60
	lui	$2,%hi(get_flash_register)
	addiu	$2,$2,%lo(get_flash_register)
	jalr	$2
	nop

	move	$3,$2
	li	$2,-1			# 0xffffffffffffffff
	beq	$3,$2,$L543
	nop

	li	$4,96			# 0x60
	lui	$2,%hi(get_flash_register)
	addiu	$2,$2,%lo(get_flash_register)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(__floatsisf)
	addiu	$2,$2,%lo(__floatsisf)
	move	$4,$3
	jalr	$2
	nop

	move	$4,$2
	lui	$3,%hi($LC16)
	lui	$2,%hi(__divsf3)
	addiu	$2,$2,%lo(__divsf3)
	lw	$5,%lo($LC16)($3)
	jalr	$2
	nop

	sw	$2,32($sp)
$L543:
	lui	$3,%hi($LC97)
	lui	$2,%hi(__addsf3)
	addiu	$2,$2,%lo(__addsf3)
	lw	$4,32($sp)
	lw	$5,%lo($LC97)($3)
	jalr	$2
	nop

	move	$4,$2
	lui	$3,%hi($LC16)
	lui	$2,%hi(__divsf3)
	addiu	$2,$2,%lo(__divsf3)
	lw	$5,%lo($LC16)($3)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(__fixsfsi)
	addiu	$2,$2,%lo(__fixsfsi)
	move	$4,$3
	jalr	$2
	nop

	sw	$2,24($sp)
	lw	$2,24($sp)
	addiu	$2,$2,40
	sll	$2,$2,2
	move	$4,$2
	lui	$2,%hi(get_flash_register)
	addiu	$2,$2,%lo(get_flash_register)
	jalr	$2
	nop

	move	$3,$2
	li	$2,-1			# 0xffffffffffffffff
	bne	$3,$2,$L544
	nop

	lui	$2,%hi($LC202)
	addiu	$4,$2,%lo($LC202)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	j	$L522
	nop

$L544:
	lw	$2,24($sp)
	addiu	$2,$2,40
	sll	$2,$2,2
	move	$4,$2
	lui	$2,%hi(get_flash_register)
	addiu	$2,$2,%lo(get_flash_register)
	jalr	$2
	nop

	move	$3,$2
	li	$2,268369920			# 0xfff0000
	and	$2,$3,$2
	sra	$2,$2,16
	sw	$2,40($sp)
	lw	$2,24($sp)
	addiu	$2,$2,41
	sll	$2,$2,2
	move	$4,$2
	lui	$2,%hi(get_flash_register)
	addiu	$2,$2,%lo(get_flash_register)
	jalr	$2
	nop

	move	$3,$2
	li	$2,268369920			# 0xfff0000
	and	$2,$3,$2
	sra	$2,$2,16
	sw	$2,44($sp)
	lui	$2,%hi(__floatunsisf)
	addiu	$2,$2,%lo(__floatunsisf)
	lw	$4,40($sp)
	jalr	$2
	nop

	move	$16,$2
	lw	$2,24($sp)
	sll	$2,$2,1
	sll	$3,$2,2
	addu	$2,$2,$3
	addiu	$3,$2,-40
	lui	$2,%hi(__floatsisf)
	addiu	$2,$2,%lo(__floatsisf)
	move	$4,$3
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(__subsf3)
	addiu	$2,$2,%lo(__subsf3)
	lw	$4,32($sp)
	move	$5,$3
	jalr	$2
	nop

	move	$17,$2
	lw	$3,44($sp)
	lw	$2,40($sp)
	subu	$3,$3,$2
	lui	$2,%hi(__floatunsisf)
	addiu	$2,$2,%lo(__floatunsisf)
	move	$4,$3
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(__mulsf3)
	addiu	$2,$2,%lo(__mulsf3)
	move	$4,$17
	move	$5,$3
	jalr	$2
	nop

	move	$4,$2
	lui	$3,%hi($LC16)
	lui	$2,%hi(__divsf3)
	addiu	$2,$2,%lo(__divsf3)
	lw	$5,%lo($LC16)($3)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(__addsf3)
	addiu	$2,$2,%lo(__addsf3)
	move	$4,$16
	move	$5,$3
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(__fixunssfsi)
	addiu	$2,$2,%lo(__fixunssfsi)
	move	$4,$3
	jalr	$2
	nop

	sw	$2,60($sp)
	move	$4,$0
	lui	$2,%hi(get_flash_register)
	addiu	$2,$2,%lo(get_flash_register)
	jalr	$2
	nop

	sw	$2,36($sp)
	lw	$3,36($sp)
	lw	$2,60($sp)
	subu	$2,$3,$2
	sw	$2,56($sp)
	lui	$2,%hi($LC203)
	addiu	$4,$2,%lo($LC203)
	lw	$5,56($sp)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	sw	$0,28($sp)
	j	$L545
	nop

$L554:
	lui	$2,%hi(__floatsisf)
	addiu	$2,$2,%lo(__floatsisf)
	lw	$4,28($sp)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(__extendsfdf2)
	addiu	$2,$2,%lo(__extendsfdf2)
	move	$4,$3
	jalr	$2
	nop

	move	$5,$3
	move	$4,$2
	lui	$3,%hi($LC98)
	lui	$2,%hi(__muldf3)
	addiu	$2,$2,%lo(__muldf3)
	lw	$7,%lo($LC98+4)($3)
	lw	$6,%lo($LC98)($3)
	jalr	$2
	nop

	move	$5,$3
	move	$4,$2
	lui	$3,%hi($LC201)
	lui	$2,%hi(__subdf3)
	addiu	$2,$2,%lo(__subdf3)
	lw	$7,%lo($LC201+4)($3)
	lw	$6,%lo($LC201)($3)
	jalr	$2
	nop

	move	$5,$3
	move	$4,$2
	lui	$2,%hi(__truncdfsf2)
	addiu	$2,$2,%lo(__truncdfsf2)
	jalr	$2
	nop

	sw	$2,64($sp)
	lui	$3,%hi($LC97)
	lui	$2,%hi(__addsf3)
	addiu	$2,$2,%lo(__addsf3)
	lw	$4,64($sp)
	lw	$5,%lo($LC97)($3)
	jalr	$2
	nop

	move	$4,$2
	lui	$3,%hi($LC16)
	lui	$2,%hi(__divsf3)
	addiu	$2,$2,%lo(__divsf3)
	lw	$5,%lo($LC16)($3)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(__fixsfsi)
	addiu	$2,$2,%lo(__fixsfsi)
	move	$4,$3
	jalr	$2
	nop

	sw	$2,24($sp)
	lw	$2,24($sp)
	bgez	$2,$L546
	nop

	sw	$0,24($sp)
	j	$L547
	nop

$L546:
	lw	$2,24($sp)
	slt	$2,$2,16
	bne	$2,$0,$L547
	nop

	li	$2,15			# 0xf
	sw	$2,24($sp)
$L547:
	lw	$2,24($sp)
	addiu	$2,$2,40
	sll	$2,$2,2
	move	$4,$2
	lui	$2,%hi(get_flash_register)
	addiu	$2,$2,%lo(get_flash_register)
	jalr	$2
	nop

	move	$3,$2
	li	$2,268369920			# 0xfff0000
	and	$2,$3,$2
	sra	$2,$2,16
	sw	$2,40($sp)
	lw	$2,24($sp)
	addiu	$2,$2,41
	sll	$2,$2,2
	move	$4,$2
	lui	$2,%hi(get_flash_register)
	addiu	$2,$2,%lo(get_flash_register)
	jalr	$2
	nop

	move	$3,$2
	li	$2,268369920			# 0xfff0000
	and	$2,$3,$2
	sra	$2,$2,16
	sw	$2,44($sp)
	lw	$3,40($sp)
	li	$2,4095			# 0xfff
	beq	$3,$2,$L548
	nop

	lw	$3,44($sp)
	li	$2,4095			# 0xfff
	bne	$3,$2,$L549
	nop

$L548:
	lui	$2,%hi(LUT_Ibias_Imod)
	lw	$3,28($sp)
	sll	$3,$3,3
	addiu	$2,$2,%lo(LUT_Ibias_Imod)
	addu	$2,$3,$2
	li	$3,4095			# 0xfff
	sw	$3,0($2)
	j	$L550
	nop

$L549:
	lw	$2,24($sp)
	slt	$2,$2,4
	beq	$2,$0,$L551
	nop

	lui	$2,%hi(__floatunsisf)
	addiu	$2,$2,%lo(__floatunsisf)
	lw	$4,40($sp)
	jalr	$2
	nop

	move	$16,$2
	lui	$3,%hi($LC97)
	lui	$2,%hi(__addsf3)
	addiu	$2,$2,%lo(__addsf3)
	lw	$4,64($sp)
	lw	$5,%lo($LC97)($3)
	jalr	$2
	nop

	move	$17,$2
	lw	$2,24($sp)
	sll	$2,$2,1
	sll	$3,$2,2
	addu	$3,$2,$3
	lui	$2,%hi(__floatsisf)
	addiu	$2,$2,%lo(__floatsisf)
	move	$4,$3
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(__subsf3)
	addiu	$2,$2,%lo(__subsf3)
	move	$4,$17
	move	$5,$3
	jalr	$2
	nop

	move	$17,$2
	lw	$3,44($sp)
	lw	$2,40($sp)
	subu	$3,$3,$2
	lui	$2,%hi(__floatunsisf)
	addiu	$2,$2,%lo(__floatunsisf)
	move	$4,$3
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(__mulsf3)
	addiu	$2,$2,%lo(__mulsf3)
	move	$4,$17
	move	$5,$3
	jalr	$2
	nop

	move	$4,$2
	lui	$3,%hi($LC16)
	lui	$2,%hi(__divsf3)
	addiu	$2,$2,%lo(__divsf3)
	lw	$5,%lo($LC16)($3)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(__addsf3)
	addiu	$2,$2,%lo(__addsf3)
	move	$4,$16
	move	$5,$3
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(__fixunssfsi)
	addiu	$2,$2,%lo(__fixunssfsi)
	move	$4,$3
	jalr	$2
	nop

	sw	$2,16($sp)
	j	$L552
	nop

$L551:
	lui	$2,%hi(__floatunsisf)
	addiu	$2,$2,%lo(__floatunsisf)
	lw	$4,40($sp)
	jalr	$2
	nop

	move	$16,$2
	lw	$2,24($sp)
	sll	$2,$2,1
	sll	$3,$2,2
	addu	$2,$2,$3
	addiu	$3,$2,-40
	lui	$2,%hi(__floatsisf)
	addiu	$2,$2,%lo(__floatsisf)
	move	$4,$3
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(__subsf3)
	addiu	$2,$2,%lo(__subsf3)
	lw	$4,64($sp)
	move	$5,$3
	jalr	$2
	nop

	move	$17,$2
	lw	$3,44($sp)
	lw	$2,40($sp)
	subu	$3,$3,$2
	lui	$2,%hi(__floatunsisf)
	addiu	$2,$2,%lo(__floatunsisf)
	move	$4,$3
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(__mulsf3)
	addiu	$2,$2,%lo(__mulsf3)
	move	$4,$17
	move	$5,$3
	jalr	$2
	nop

	move	$4,$2
	lui	$3,%hi($LC16)
	lui	$2,%hi(__divsf3)
	addiu	$2,$2,%lo(__divsf3)
	lw	$5,%lo($LC16)($3)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(__addsf3)
	addiu	$2,$2,%lo(__addsf3)
	move	$4,$16
	move	$5,$3
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(__fixunssfsi)
	addiu	$2,$2,%lo(__fixunssfsi)
	move	$4,$3
	jalr	$2
	nop

	sw	$2,16($sp)
$L552:
	lw	$3,16($sp)
	lw	$2,56($sp)
	addu	$3,$3,$2
	lui	$2,%hi(LUT_Ibias_Imod)
	lw	$4,28($sp)
	sll	$4,$4,3
	addiu	$2,$2,%lo(LUT_Ibias_Imod)
	addu	$2,$4,$2
	sw	$3,0($2)
$L550:
	lui	$2,%hi(LUT_Ibias_Imod)
	lw	$3,28($sp)
	sll	$3,$3,3
	addiu	$2,$2,%lo(LUT_Ibias_Imod)
	addu	$2,$3,$2
	lw	$2,0($2)
	bne	$2,$0,$L553
	nop

	lui	$2,%hi($LC204)
	addiu	$4,$2,%lo($LC204)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	j	$L522
	nop

$L553:
	lw	$2,28($sp)
	addiu	$2,$2,1
	sw	$2,28($sp)
$L545:
	lw	$2,28($sp)
	slt	$2,$2,64
	bne	$2,$0,$L554
	nop

	sw	$0,24($sp)
	j	$L555
	nop

$L557:
	lw	$2,24($sp)
	sll	$2,$2,2
	sw	$2,28($sp)
	lw	$2,24($sp)
	addiu	$2,$2,40
	sll	$2,$2,2
	move	$4,$2
	lui	$2,%hi(get_flash_register)
	addiu	$2,$2,%lo(get_flash_register)
	jalr	$2
	nop

	move	$3,$2
	li	$2,268369920			# 0xfff0000
	and	$2,$3,$2
	sra	$3,$2,16
	li	$2,4095			# 0xfff
	beq	$3,$2,$L556
	nop

	lw	$2,24($sp)
	addiu	$2,$2,40
	sll	$2,$2,2
	move	$4,$2
	lui	$2,%hi(get_flash_register)
	addiu	$2,$2,%lo(get_flash_register)
	jalr	$2
	nop

	move	$3,$2
	li	$2,268369920			# 0xfff0000
	and	$2,$3,$2
	sra	$2,$2,16
	move	$3,$2
	lw	$2,56($sp)
	addu	$3,$3,$2
	lui	$2,%hi(LUT_Ibias_Imod)
	lw	$4,28($sp)
	sll	$4,$4,3
	addiu	$2,$2,%lo(LUT_Ibias_Imod)
	addu	$2,$4,$2
	sw	$3,0($2)
$L556:
	lw	$2,24($sp)
	addiu	$2,$2,1
	sw	$2,24($sp)
$L555:
	lw	$2,24($sp)
	slt	$2,$2,16
	bne	$2,$0,$L557
	nop

$L522:
	lw	$31,84($sp)
	lw	$17,80($sp)
	lw	$16,76($sp)
	addiu	$sp,$sp,88
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	mt7570_LUT_recover
	.size	mt7570_LUT_recover, .-mt7570_LUT_recover
	.rdata
	.align	2
$LC205:
	.ascii	"Temperature\000"
	.align	2
$LC206:
	.ascii	"Ibias\000"
	.align	2
$LC207:
	.ascii	"Imod\012\000"
	.align	2
$LC208:
	.ascii	"%03d.%01d\011\0110x%03x\0110x%03x\012\000"
	.section	.text.mt7570_show_LUT,"ax",@progbits
	.align	2
	.globl	mt7570_show_LUT
	.set	nomips16
	.set	nomicromips
	.ent	mt7570_show_LUT
	.type	mt7570_show_LUT, @function
mt7570_show_LUT:
	.frame	$sp,40,$31		# vars= 8, regs= 1/0, args= 24, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-40
	sw	$31,36($sp)
	sw	$0,24($sp)
	lui	$2,%hi($LC205)
	addiu	$4,$2,%lo($LC205)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi($LC26)
	addiu	$4,$2,%lo($LC26)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi($LC206)
	addiu	$4,$2,%lo($LC206)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi($LC26)
	addiu	$4,$2,%lo($LC26)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lui	$2,%hi($LC207)
	addiu	$4,$2,%lo($LC207)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	sw	$0,24($sp)
	j	$L559
	nop

$L561:
	lw	$4,24($sp)
	move	$3,$4
	sll	$2,$3,3
	move	$3,$2
	sll	$2,$3,2
	subu	$2,$2,$3
	addu	$2,$2,$4
	addiu	$2,$2,-400
	li	$3,1717960704			# 0x66660000
	ori	$3,$3,0x6667
	mult	$2,$3
	mfhi	$3
	sra	$3,$3,2
	sra	$2,$2,31
	subu	$5,$3,$2
	lw	$4,24($sp)
	move	$3,$4
	sll	$2,$3,3
	move	$3,$2
	sll	$2,$3,2
	subu	$2,$2,$3
	addu	$2,$2,$4
	addiu	$2,$2,-400
	sw	$2,28($sp)
	lw	$2,28($sp)
	bgez	$2,$L560
	nop

	subu	$2,$0,$2
$L560:
	move	$3,$2
	li	$2,1717960704			# 0x66660000
	ori	$2,$2,0x6667
	mult	$3,$2
	mfhi	$2
	sra	$4,$2,2
	sra	$2,$3,31
	subu	$2,$4,$2
	sll	$2,$2,1
	sll	$4,$2,2
	addu	$2,$2,$4
	subu	$2,$3,$2
	lui	$3,%hi(LUT_Ibias_Imod)
	lw	$4,24($sp)
	sll	$4,$4,3
	addiu	$3,$3,%lo(LUT_Ibias_Imod)
	addu	$3,$4,$3
	lw	$7,0($3)
	lui	$3,%hi(LUT_Ibias_Imod)
	lw	$4,24($sp)
	sll	$4,$4,3
	addiu	$3,$3,%lo(LUT_Ibias_Imod)
	addu	$3,$4,$3
	lw	$3,4($3)
	sw	$3,16($sp)
	lui	$3,%hi($LC208)
	addiu	$4,$3,%lo($LC208)
	move	$6,$2
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lw	$2,24($sp)
	addiu	$2,$2,1
	sw	$2,24($sp)
$L559:
	lw	$2,24($sp)
	slt	$2,$2,64
	bne	$2,$0,$L561
	nop

	lw	$31,36($sp)
	addiu	$sp,$sp,40
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	mt7570_show_LUT
	.size	mt7570_show_LUT, .-mt7570_show_LUT
	.rdata
	.align	2
$LC209:
	.ascii	"Switch to Single-closed Loop Mode\012\000"
	.section	.text.mt7570_SingleClosedLoopMode,"ax",@progbits
	.align	2
	.globl	mt7570_SingleClosedLoopMode
	.set	nomips16
	.set	nomicromips
	.ent	mt7570_SingleClosedLoopMode
	.type	mt7570_SingleClosedLoopMode, @function
mt7570_SingleClosedLoopMode:
	.frame	$sp,48,$31		# vars= 8, regs= 1/0, args= 32, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-48
	sw	$31,44($sp)
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,116($2)
	move	$3,$2
	li	$2,316			# 0x13c
	sw	$2,16($sp)
	addiu	$2,$sp,32
	sw	$2,20($sp)
	li	$2,1			# 0x1
	sw	$2,24($sp)
	move	$4,$0
	move	$5,$3
	li	$6,112			# 0x70
	li	$7,2			# 0x2
	lui	$2,%hi(phy_I2C_read)
	addiu	$2,$2,%lo(phy_I2C_read)
	jalr	$2
	nop

	lbu	$2,32($sp)
	seb	$3,$2
	li	$2,-4			# 0xfffffffffffffffc
	and	$2,$3,$2
	seb	$2,$2
	ori	$2,$2,0x2
	seb	$2,$2
	andi	$2,$2,0x00ff
	sb	$2,32($sp)
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,116($2)
	move	$3,$2
	li	$2,316			# 0x13c
	sw	$2,16($sp)
	addiu	$2,$sp,32
	sw	$2,20($sp)
	li	$2,1			# 0x1
	sw	$2,24($sp)
	move	$4,$0
	move	$5,$3
	li	$6,112			# 0x70
	li	$7,2			# 0x2
	lui	$2,%hi(phy_I2C_write)
	addiu	$2,$2,%lo(phy_I2C_write)
	jalr	$2
	nop

	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,116($2)
	move	$3,$2
	li	$2,332			# 0x14c
	sw	$2,16($sp)
	addiu	$2,$sp,32
	sw	$2,20($sp)
	li	$2,1			# 0x1
	sw	$2,24($sp)
	move	$4,$0
	move	$5,$3
	li	$6,112			# 0x70
	li	$7,2			# 0x2
	lui	$2,%hi(phy_I2C_read)
	addiu	$2,$2,%lo(phy_I2C_read)
	jalr	$2
	nop

	lbu	$2,32($sp)
	seb	$3,$2
	li	$2,-4			# 0xfffffffffffffffc
	and	$2,$3,$2
	seb	$2,$2
	ori	$2,$2,0x1
	seb	$2,$2
	andi	$2,$2,0x00ff
	sb	$2,32($sp)
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,116($2)
	move	$3,$2
	li	$2,332			# 0x14c
	sw	$2,16($sp)
	addiu	$2,$sp,32
	sw	$2,20($sp)
	li	$2,1			# 0x1
	sw	$2,24($sp)
	move	$4,$0
	move	$5,$3
	li	$6,112			# 0x70
	li	$7,2			# 0x2
	lui	$2,%hi(phy_I2C_write)
	addiu	$2,$2,%lo(phy_I2C_write)
	jalr	$2
	nop

	lui	$2,%hi($LC209)
	addiu	$4,$2,%lo($LC209)
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

	lw	$31,44($sp)
	addiu	$sp,$sp,48
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	mt7570_SingleClosedLoopMode
	.size	mt7570_SingleClosedLoopMode, .-mt7570_SingleClosedLoopMode
	.rdata
	.align	2
$LC210:
	.ascii	"[%lu]0ms[%s:%d]\012  Target mod loaded, 0x%x\012\000"
	.align	2
$LC211:
	.ascii	"[%lu]0ms[%s:%d]\012  Target Imod = %d.%.2dmA\012\000"
	.section	.text.mt7570_OpenLoopControl,"ax",@progbits
	.align	2
	.globl	mt7570_OpenLoopControl
	.set	nomips16
	.set	nomicromips
	.ent	mt7570_OpenLoopControl
	.type	mt7570_OpenLoopControl, @function
mt7570_OpenLoopControl:
	.frame	$sp,64,$31		# vars= 24, regs= 2/0, args= 32, gp= 0
	.mask	0x80010000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-64
	sw	$31,60($sp)
	sw	$16,56($sp)
	sw	$0,36($sp)
	sw	$0,32($sp)
	sw	$0,40($sp)
	sw	$0,44($sp)
	move	$2,$0
	sw	$2,48($sp)
	lui	$2,%hi(Env_temperature)
	lw	$4,%lo(Env_temperature)($2)
	lui	$3,%hi($LC97)
	lui	$2,%hi(__addsf3)
	addiu	$2,$2,%lo(__addsf3)
	lw	$5,%lo($LC97)($3)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(__extendsfdf2)
	addiu	$2,$2,%lo(__extendsfdf2)
	move	$4,$3
	jalr	$2
	nop

	move	$5,$3
	move	$4,$2
	lui	$3,%hi($LC98)
	lui	$2,%hi(__divdf3)
	addiu	$2,$2,%lo(__divdf3)
	lw	$7,%lo($LC98+4)($3)
	lw	$6,%lo($LC98)($3)
	jalr	$2
	nop

	move	$5,$3
	move	$4,$2
	lui	$2,%hi(__fixdfsi)
	addiu	$2,$2,%lo(__fixdfsi)
	jalr	$2
	nop

	sw	$2,32($sp)
	lw	$2,32($sp)
	bgez	$2,$L564
	nop

	sw	$0,32($sp)
$L564:
	lui	$2,%hi(LUT_Ibias_Imod)
	lw	$3,32($sp)
	sll	$3,$3,3
	addiu	$2,$2,%lo(LUT_Ibias_Imod)
	addu	$2,$3,$2
	lw	$2,0($2)
	sw	$2,36($sp)
	lw	$3,36($sp)
	li	$2,4095			# 0xfff
	bne	$3,$2,$L565
	nop

	j	$L563
	nop

$L565:
	lw	$2,36($sp)
	andi	$2,$2,0x00ff
	sb	$2,52($sp)
	lw	$2,36($sp)
	srl	$2,$2,8
	andi	$2,$2,0x00ff
	sb	$2,53($sp)
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,116($2)
	move	$3,$2
	li	$2,312			# 0x138
	sw	$2,16($sp)
	addiu	$2,$sp,52
	sw	$2,20($sp)
	li	$2,2			# 0x2
	sw	$2,24($sp)
	move	$4,$0
	move	$5,$3
	li	$6,112			# 0x70
	li	$7,2			# 0x2
	lui	$2,%hi(phy_I2C_write)
	addiu	$2,$2,%lo(phy_I2C_write)
	jalr	$2
	nop

	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,136($2)
	andi	$2,$2,0x10
	beq	$2,$0,$L567
	nop

	lui	$2,%hi(jiffies)
	lw	$16,%lo(jiffies)($2)
	lui	$2,%hi($LC37)
	addiu	$4,$2,%lo($LC37)
	li	$5,47			# 0x2f
	lui	$2,%hi(strrchr)
	addiu	$2,$2,%lo(strrchr)
	jalr	$2
	nop

	addiu	$3,$2,1
	lw	$2,36($sp)
	sw	$2,16($sp)
	lui	$2,%hi($LC198)
	addiu	$4,$2,%lo($LC198)
	move	$5,$16
	move	$6,$3
	li	$7,4660			# 0x1234
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L567:
	lui	$2,%hi(__floatunsisf)
	addiu	$2,$2,%lo(__floatunsisf)
	lw	$4,36($sp)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(__extendsfdf2)
	addiu	$2,$2,%lo(__extendsfdf2)
	move	$4,$3
	jalr	$2
	nop

	move	$5,$3
	move	$4,$2
	lui	$3,%hi($LC107)
	lui	$2,%hi(__muldf3)
	addiu	$2,$2,%lo(__muldf3)
	lw	$7,%lo($LC107+4)($3)
	lw	$6,%lo($LC107)($3)
	jalr	$2
	nop

	move	$5,$3
	move	$4,$2
	lui	$2,%hi(__truncdfsf2)
	addiu	$2,$2,%lo(__truncdfsf2)
	jalr	$2
	nop

	sw	$2,48($sp)
	lui	$2,%hi(__fixsfsi)
	addiu	$2,$2,%lo(__fixsfsi)
	lw	$4,48($sp)
	jalr	$2
	nop

	sw	$2,40($sp)
	lui	$3,%hi($LC84)
	lui	$2,%hi(__mulsf3)
	addiu	$2,$2,%lo(__mulsf3)
	lw	$4,48($sp)
	lw	$5,%lo($LC84)($3)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(__fixsfsi)
	addiu	$2,$2,%lo(__fixsfsi)
	move	$4,$3
	jalr	$2
	nop

	move	$3,$2
	li	$2,1374355456			# 0x51eb0000
	ori	$2,$2,0x851f
	mult	$3,$2
	mfhi	$2
	sra	$4,$2,5
	sra	$2,$3,31
	subu	$2,$4,$2
	sll	$2,$2,2
	sll	$4,$2,2
	addu	$2,$2,$4
	sll	$4,$2,2
	addu	$2,$2,$4
	subu	$2,$3,$2
	sw	$2,44($sp)
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,136($2)
	andi	$2,$2,0x10
	beq	$2,$0,$L568
	nop

	lui	$2,%hi(jiffies)
	lw	$16,%lo(jiffies)($2)
	lui	$2,%hi($LC37)
	addiu	$4,$2,%lo($LC37)
	li	$5,47			# 0x2f
	lui	$2,%hi(strrchr)
	addiu	$2,$2,%lo(strrchr)
	jalr	$2
	nop

	addiu	$3,$2,1
	lw	$2,40($sp)
	sw	$2,16($sp)
	lw	$2,44($sp)
	sw	$2,20($sp)
	lui	$2,%hi($LC199)
	addiu	$4,$2,%lo($LC199)
	move	$5,$16
	move	$6,$3
	li	$7,4664			# 0x1238
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L568:
	lui	$2,%hi(LUT_Ibias_Imod)
	lw	$3,32($sp)
	sll	$3,$3,3
	addiu	$2,$2,%lo(LUT_Ibias_Imod)
	addu	$2,$3,$2
	lw	$2,4($2)
	sw	$2,36($sp)
	lw	$3,36($sp)
	li	$2,4095			# 0xfff
	bne	$3,$2,$L569
	nop

	j	$L563
	nop

$L569:
	lw	$2,36($sp)
	andi	$2,$2,0x00ff
	sb	$2,52($sp)
	lw	$2,36($sp)
	srl	$2,$2,8
	andi	$2,$2,0x00ff
	sb	$2,53($sp)
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,116($2)
	move	$3,$2
	li	$2,328			# 0x148
	sw	$2,16($sp)
	addiu	$2,$sp,52
	sw	$2,20($sp)
	li	$2,2			# 0x2
	sw	$2,24($sp)
	move	$4,$0
	move	$5,$3
	li	$6,112			# 0x70
	li	$7,2			# 0x2
	lui	$2,%hi(phy_I2C_write)
	addiu	$2,$2,%lo(phy_I2C_write)
	jalr	$2
	nop

	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,136($2)
	andi	$2,$2,0x10
	beq	$2,$0,$L570
	nop

	lui	$2,%hi(jiffies)
	lw	$16,%lo(jiffies)($2)
	lui	$2,%hi($LC37)
	addiu	$4,$2,%lo($LC37)
	li	$5,47			# 0x2f
	lui	$2,%hi(strrchr)
	addiu	$2,$2,%lo(strrchr)
	jalr	$2
	nop

	addiu	$3,$2,1
	lw	$2,36($sp)
	sw	$2,16($sp)
	lui	$2,%hi($LC210)
	addiu	$4,$2,%lo($LC210)
	move	$5,$16
	move	$6,$3
	li	$7,4677			# 0x1245
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L570:
	lui	$2,%hi(__floatunsisf)
	addiu	$2,$2,%lo(__floatunsisf)
	lw	$4,36($sp)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(__extendsfdf2)
	addiu	$2,$2,%lo(__extendsfdf2)
	move	$4,$3
	jalr	$2
	nop

	move	$5,$3
	move	$4,$2
	lui	$3,%hi($LC162)
	lui	$2,%hi(__muldf3)
	addiu	$2,$2,%lo(__muldf3)
	lw	$7,%lo($LC162+4)($3)
	lw	$6,%lo($LC162)($3)
	jalr	$2
	nop

	move	$5,$3
	move	$4,$2
	lui	$2,%hi(__truncdfsf2)
	addiu	$2,$2,%lo(__truncdfsf2)
	jalr	$2
	nop

	sw	$2,48($sp)
	lui	$2,%hi(__fixsfsi)
	addiu	$2,$2,%lo(__fixsfsi)
	lw	$4,48($sp)
	jalr	$2
	nop

	sw	$2,40($sp)
	lui	$3,%hi($LC84)
	lui	$2,%hi(__mulsf3)
	addiu	$2,$2,%lo(__mulsf3)
	lw	$4,48($sp)
	lw	$5,%lo($LC84)($3)
	jalr	$2
	nop

	move	$3,$2
	lui	$2,%hi(__fixsfsi)
	addiu	$2,$2,%lo(__fixsfsi)
	move	$4,$3
	jalr	$2
	nop

	move	$3,$2
	li	$2,1374355456			# 0x51eb0000
	ori	$2,$2,0x851f
	mult	$3,$2
	mfhi	$2
	sra	$4,$2,5
	sra	$2,$3,31
	subu	$2,$4,$2
	sll	$2,$2,2
	sll	$4,$2,2
	addu	$2,$2,$4
	sll	$4,$2,2
	addu	$2,$2,$4
	subu	$2,$3,$2
	sw	$2,44($sp)
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,136($2)
	andi	$2,$2,0x10
	beq	$2,$0,$L563
	nop

	lui	$2,%hi(jiffies)
	lw	$16,%lo(jiffies)($2)
	lui	$2,%hi($LC37)
	addiu	$4,$2,%lo($LC37)
	li	$5,47			# 0x2f
	lui	$2,%hi(strrchr)
	addiu	$2,$2,%lo(strrchr)
	jalr	$2
	nop

	addiu	$3,$2,1
	lw	$2,40($sp)
	sw	$2,16($sp)
	lw	$2,44($sp)
	sw	$2,20($sp)
	lui	$2,%hi($LC211)
	addiu	$4,$2,%lo($LC211)
	move	$5,$16
	move	$6,$3
	li	$7,4681			# 0x1249
	lui	$2,%hi(printk)
	addiu	$2,$2,%lo(printk)
	jalr	$2
	nop

$L563:
	lw	$31,60($sp)
	lw	$16,56($sp)
	addiu	$sp,$sp,64
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	mt7570_OpenLoopControl
	.size	mt7570_OpenLoopControl, .-mt7570_OpenLoopControl
	.section	.text.mt7570_LOS_init,"ax",@progbits
	.align	2
	.globl	mt7570_LOS_init
	.set	nomips16
	.set	nomicromips
	.ent	mt7570_LOS_init
	.type	mt7570_LOS_init, @function
mt7570_LOS_init:
	.frame	$sp,48,$31		# vars= 8, regs= 1/0, args= 32, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-48
	sw	$31,44($sp)
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,116($2)
	move	$3,$2
	li	$2,284			# 0x11c
	sw	$2,16($sp)
	addiu	$2,$sp,32
	sw	$2,20($sp)
	li	$2,4			# 0x4
	sw	$2,24($sp)
	move	$4,$0
	move	$5,$3
	li	$6,112			# 0x70
	li	$7,2			# 0x2
	lui	$2,%hi(phy_I2C_read)
	addiu	$2,$2,%lo(phy_I2C_read)
	jalr	$2
	nop

	lbu	$2,32($sp)
	ori	$2,$2,0x1
	andi	$2,$2,0x00ff
	sb	$2,32($sp)
	lbu	$2,33($sp)
	ori	$2,$2,0x1f
	andi	$2,$2,0x00ff
	sb	$2,33($sp)
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,116($2)
	move	$3,$2
	li	$2,284			# 0x11c
	sw	$2,16($sp)
	addiu	$2,$sp,32
	sw	$2,20($sp)
	li	$2,4			# 0x4
	sw	$2,24($sp)
	move	$4,$0
	move	$5,$3
	li	$6,112			# 0x70
	li	$7,2			# 0x2
	lui	$2,%hi(phy_I2C_write)
	addiu	$2,$2,%lo(phy_I2C_write)
	jalr	$2
	nop

	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,116($2)
	move	$3,$2
	li	$2,36			# 0x24
	sw	$2,16($sp)
	addiu	$2,$sp,32
	sw	$2,20($sp)
	li	$2,4			# 0x4
	sw	$2,24($sp)
	move	$4,$0
	move	$5,$3
	li	$6,112			# 0x70
	li	$7,2			# 0x2
	lui	$2,%hi(phy_I2C_read)
	addiu	$2,$2,%lo(phy_I2C_read)
	jalr	$2
	nop

	lbu	$2,35($sp)
	ori	$2,$2,0x4
	andi	$2,$2,0x00ff
	sb	$2,35($sp)
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,116($2)
	move	$3,$2
	li	$2,36			# 0x24
	sw	$2,16($sp)
	addiu	$2,$sp,32
	sw	$2,20($sp)
	li	$2,4			# 0x4
	sw	$2,24($sp)
	move	$4,$0
	move	$5,$3
	li	$6,112			# 0x70
	li	$7,2			# 0x2
	lui	$2,%hi(phy_I2C_write)
	addiu	$2,$2,%lo(phy_I2C_write)
	jalr	$2
	nop

	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,116($2)
	move	$3,$2
	li	$2,36			# 0x24
	sw	$2,16($sp)
	addiu	$2,$sp,32
	sw	$2,20($sp)
	li	$2,4			# 0x4
	sw	$2,24($sp)
	move	$4,$0
	move	$5,$3
	li	$6,112			# 0x70
	li	$7,2			# 0x2
	lui	$2,%hi(phy_I2C_read)
	addiu	$2,$2,%lo(phy_I2C_read)
	jalr	$2
	nop

	lbu	$2,34($sp)
	ori	$2,$2,0x40
	andi	$2,$2,0x00ff
	sb	$2,34($sp)
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,116($2)
	move	$3,$2
	li	$2,36			# 0x24
	sw	$2,16($sp)
	addiu	$2,$sp,32
	sw	$2,20($sp)
	li	$2,4			# 0x4
	sw	$2,24($sp)
	move	$4,$0
	move	$5,$3
	li	$6,112			# 0x70
	li	$7,2			# 0x2
	lui	$2,%hi(phy_I2C_write)
	addiu	$2,$2,%lo(phy_I2C_write)
	jalr	$2
	nop

	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,116($2)
	move	$3,$2
	li	$2,288			# 0x120
	sw	$2,16($sp)
	addiu	$2,$sp,32
	sw	$2,20($sp)
	li	$2,4			# 0x4
	sw	$2,24($sp)
	move	$4,$0
	move	$5,$3
	li	$6,112			# 0x70
	li	$7,2			# 0x2
	lui	$2,%hi(phy_I2C_read)
	addiu	$2,$2,%lo(phy_I2C_read)
	jalr	$2
	nop

	lbu	$2,33($sp)
	ori	$2,$2,0x1f
	andi	$2,$2,0x00ff
	sb	$2,33($sp)
	lbu	$2,32($sp)
	seb	$3,$2
	li	$2,-128			# 0xffffffffffffff80
	and	$2,$3,$2
	seb	$2,$2
	ori	$2,$2,0x5
	seb	$2,$2
	andi	$2,$2,0x00ff
	sb	$2,32($sp)
	lui	$2,%hi(gpPhyPriv)
	lw	$2,%lo(gpPhyPriv)($2)
	lhu	$2,116($2)
	move	$3,$2
	li	$2,288			# 0x120
	sw	$2,16($sp)
	addiu	$2,$sp,32
	sw	$2,20($sp)
	li	$2,4			# 0x4
	sw	$2,24($sp)
	move	$4,$0
	move	$5,$3
	li	$6,112			# 0x70
	li	$7,2			# 0x2
	lui	$2,%hi(phy_I2C_write)
	addiu	$2,$2,%lo(phy_I2C_write)
	jalr	$2
	nop

	lw	$31,44($sp)
	addiu	$sp,$sp,48
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	mt7570_LOS_init
	.size	mt7570_LOS_init, .-mt7570_LOS_init
	.rdata
	.align	2
	.type	__func__.24637, @object
	.size	__func__.24637, 20
__func__.24637:
	.ascii	"REPORT_EVENT_TO_MAC\000"
	.align	2
	.type	__func__.25267, @object
	.size	__func__.25267, 23
__func__.25267:
	.ascii	"mt7570_ADC_calibration\000"
	.align	2
	.type	__func__.25319, @object
	.size	__func__.25319, 27
__func__.25319:
	.ascii	"mt7570_ADC_temperature_get\000"
	.align	2
	.type	__func__.25328, @object
	.size	__func__.25328, 23
__func__.25328:
	.ascii	"mt7570_ADC_voltage_get\000"
	.align	2
	.type	__func__.25380, @object
	.size	__func__.25380, 20
__func__.25380:
	.ascii	"mt7570_RSSI_current\000"
	.align	2
	.type	__func__.25403, @object
	.size	__func__.25403, 20
__func__.25403:
	.ascii	"mt7570_dark_current\000"
	.align	2
	.type	__func__.25423, @object
	.size	__func__.25423, 19
__func__.25423:
	.ascii	"mt7570_MPD_current\000"
	.local	d.25458
	.comm	d.25458,4,4
	.local	e.25459
	.comm	e.25459,4,4
	.local	f.25460
	.comm	f.25460,4,4
	.local	g.25461
	.comm	g.25461,4,4
	.local	x.25462
	.comm	x.25462,4,4
	.local	y.25463
	.comm	y.25463,4,4
	.local	z.25464
	.comm	z.25464,4,4
	.align	2
	.type	__func__.25513, @object
	.size	__func__.25513, 16
__func__.25513:
	.ascii	"LOS_calibration\000"
	.local	TEC_cnt.25547
	.comm	TEC_cnt.25547,4,4
	.align	2
$LC16:
	.word	1092616192
	.align	3
$LC20:
	.word	1085496832
	.word	0
	.align	3
$LC66:
	.word	1068079513
	.word	-1717986918
	.align	3
$LC67:
	.word	1073916586
	.word	-1431655765
	.align	3
$LC73:
	.word	1072452075
	.word	-2061584302
	.align	3
$LC74:
	.word	1073490165
	.word	-1030792151
	.align	2
$LC76:
	.word	1036831949
	.align	2
$LC77:
	.word	1032805417
	.align	2
$LC78:
	.word	1108082688
	.align	2
$LC79:
	.word	1035993088
	.align	2
$LC80:
	.word	1106247680
	.align	2
$LC81:
	.word	1108344832
	.align	2
$LC82:
	.word	1109917696
	.align	2
$LC83:
	.word	1110861414
	.align	2
$LC84:
	.word	1120403456
	.align	2
$LC86:
	.word	1103626240
	.align	2
$LC89:
	.word	1115684864
	.align	2
$LC94:
	.word	1148846080
	.align	2
$LC97:
	.word	1109393408
	.align	3
$LC98:
	.word	1074003968
	.word	0
	.align	2
$LC100:
	.word	1084227584
	.align	2
$LC101:
	.word	1134804992
	.align	2
$LC102:
	.word	1140319846
	.align	2
$LC104:
	.word	1132462080
	.align	3
$LC106:
	.word	1058682594
	.word	-350469331
	.align	3
$LC107:
	.word	1066992014
	.word	1970874593
	.align	3
$LC108:
	.word	1083129856
	.word	0
	.align	3
$LC109:
	.word	1073741824
	.word	0
	.align	3
$LC114:
	.word	1071015526
	.word	1717986918
	.align	3
$LC126:
	.word	1069128089
	.word	-1717986918
	.align	2
$LC135:
	.word	1399379109
	.align	2
$LC136:
	.word	1232348160
	.align	3
$LC162:
	.word	1066828268
	.word	-727052064
	.align	2
$LC170:
	.word	1065353216
	.align	2
$LC171:
	.word	1073741824
	.align	2
$LC172:
	.word	1077936128
	.align	2
$LC173:
	.word	1082130432
	.align	2
$LC174:
	.word	1086324736
	.align	3
$LC175:
	.word	1073900465
	.word	-1145730699
	.align	2
$LC178:
	.word	1088421888
	.align	2
$LC179:
	.word	1090519040
	.align	2
$LC186:
	.word	1176256512
	.align	3
$LC190:
	.word	1077477376
	.word	0
	.align	3
$LC191:
	.word	1076101120
	.word	0
	.align	3
$LC201:
	.word	1078198272
	.word	0
	.ident	"GCC: (Buildroot 2015.08.1) 4.9.3"
