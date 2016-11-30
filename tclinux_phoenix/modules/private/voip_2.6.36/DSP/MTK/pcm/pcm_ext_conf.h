/*
 * PCMdriver.h
 *
 *  Created on: 2008/9/19
 *      Author: pork
 */
#ifndef PCM_EXT_CONF_H
#define PCM_EXT_CONF_H

/*Variable Definition*/

#define LOOPBACK_OFF 0
#define LOOPBACK_ON 1

#define LB_GARBAGE_OFF 0
#define LB_GARBAGE_ON 1

#define LITTLE_ENDIAN 0
#define BIG_ENDIAN 1

#define LSB 0
#define MSB 1

#define FRAME_SYNC_EDGE_RISING 0
#define FRAME_SYNC_EDGE_FALLING 1

#define FRAME_SYNC_LEN_1 0
#define FRAME_SYNC_LEN_8 2
#define FRAME_SYNC_LEN_16 3

#define PCM_SAMPLE_CLOCK_8 0
#define PCM_SAMPLE_CLOCK_16 1

#define PCM_BIT_CLOCK_256 0
#define PCM_BIT_CLOCK_512 1
#define PCM_BIT_CLOCK_1024 2
#define PCM_BIT_CLOCK_2048 3
#define PCM_BIT_CLOCK_4096 4
#define PCM_BIT_CLOCK_8192 5

#define MASTER_MODE 0
#define SLAVE_MODE 1

#define BITWIDTH_8 0
#define BITWIDTH_16 1

#define ZERO_BIT_DELAY 0
#define ONE_BIT_DELAY 1

#define RISING_DATA_EDGE 0
#define FALLING_DATA_EDGE 1

#define TX_BUF_TYPE 0
#define RX_BUF_TYPE 1


#define TCIDENTIFIER "TC3262"

//#define RX_INTERVAL_EVALUATE 0

/*SLIC control flag*/
#define RESET		0
#define ENABLE		1
#define BUF_HEADER_SIZE 32

typedef union {
	struct {
		unsigned int lbGarbageEnable :1;
		unsigned int probing :3;
		unsigned int reserved2 :1;
		unsigned int cfgValid :1;
		unsigned int loopBack :1;
		unsigned int softReset :1;
		unsigned int reserved3 :1;
		unsigned int frameCount :5;
		unsigned int byteOrder :1;
		unsigned int bitOrder :1;
		unsigned int reserved4 :3;
		unsigned int delay :1;
		unsigned int dataEdge :1;
		unsigned int fsEdge :1;
		unsigned int fsLen :2;
		unsigned int reserved5 :3;
		unsigned int sampleClock :1;
		unsigned int bitClock :3;
		unsigned int pcmMode :1;
	} bits;
	unsigned int value;
} pcmCtrl_t;

/*Data Structure*/
typedef struct configNode_s{
	unsigned int loopbackMode;
	unsigned char frameCount;
	unsigned char byteOrder;
	unsigned char bitOrder;
	unsigned char fsEdge;
	unsigned char fsLen;
	unsigned char sampleClock;
	unsigned char bitClock;
	unsigned char pcmMode;
	unsigned char chNum;
	unsigned int maxRxQueueSize;
	unsigned char bitWidth[8];
	unsigned int rxSampleSize;
	unsigned int debugLevel;
	unsigned int debugCategory;
}configNode_t;

typedef struct callBackParam_s{
	unsigned long int rxSampleSize;
	unsigned long int magicNum;
#if (RX_INTERVAL_EVALUATE == 1)
	unsigned long int timestamp[10];
#endif
	unsigned int pcm;
	unsigned char* next;
	char reserved[20];
}callBackParam_t;

#endif
