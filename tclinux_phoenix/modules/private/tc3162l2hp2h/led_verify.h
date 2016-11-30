

#define RG_GPIO_MODE 0xbfa2007c
typedef struct _mode_setting_para
{
	/* 
		0-parallel
		1-serial
	*/
	unsigned char type;
	/* 
		0-normal mode
		1-flash mode
		2-flash cycle mode
		3-flash immediate mode
	*/
	unsigned char mode;
	/*gpio number is only used by parallel*/
	unsigned int gpio_no;
}mode_setting_para;

typedef struct _flash_period_para
{
	unsigned char num;
	unsigned char hi_or_low;
	unsigned long value;
}flash_period_para;

typedef struct _flash_setting_para
{
		/* 
		when flag == 0
			0-parallel
			1-serial
		when flag != 0
			0-74164
			1-74595
	*/
	unsigned char type;
	/* 
		0-not care flash parameters
		1-flash map Id config
		2-flash map Enable config
		3-flash peroid
	*/
	unsigned char flag;
	union
	{
		unsigned long mapid;
		unsigned long mapswitch;
		flash_period_para mapperiod;
	}flash_para;
	
	unsigned int gpio_no;
	
}flash_setting_para;

typedef struct _divide_para
{
	/*0---stop; 1---run*/
	unsigned char flag;
	unsigned char dividenum;//for divide register
	unsigned int data;//for serial data register
}divide_para;
	
enum led_paratype{
	LED_CONTROLLER_MODEPARA = 0,
	LED_CONTROLLER_FLASHPARA,
	LED_CONTROLLER_DIVIDEPARA,
};

enum led_modetype{
	LED_CONTROLLER_NORMAL_MODE = 0,
	LED_CONTROLLER_FLASH_MODE,
	LED_CONTROLLER_FLASH_CYCLE_MODE,
	LED_CONTROLLER_FLASH_IMMEDIATE_MODE,
	LED_CONTROLLER_FLASH_MODE_END,
};

enum led_flashtype{
	LED_CONTROLLER_NOT_USED = 0,
	LED_CONTROLLER_MAP_ID,
	LED_CONTROLLER_MAP_SWITCH,
	LED_CONTROLLER_PERIOD,
};

enum led_controller_mode{
	LED_CONTROLLER_PARALLEL = 0,
	LED_CONTROLLER_SERIAL,
};

enum led_controller_operation{
	LED_CONTROLLER_DUMP = 0,
	LED_CONTROLLER_LEDON,
	LED_CONTROLLER_LEDOFF,
	LED_CONTROLLER_BLINK,
	LED_CONTROLLER_END,
};


void ledSetFlashPeriod(uint8 num, uint8 HL, uint32 value);
void ledSetParallelFlashMapID(uint8 gpio_no, uint32 flash_ID); 
void ledSetParallelFlashMapEnable(uint8 enable, uint8 gpio_no); 
void ledSetSerialFlashMapID(uint8 gpio_no, uint32 flash_ID); 
void ledSetSerialFlashMapEnable(uint8 enable, uint8 gpio_no); 



 
