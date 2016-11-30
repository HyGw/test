/** \file vp_thermal_ringing.h
 * vp_thermal_ringing.h
 *
 *  Header file that defines the data structureds related to the 
 *  adaptive ringing power management routines
 *
 * Copyright (c) 2011, Microsemi Corporation
 *
 * $Revision: 9497 $
 * $LastChangedDate: 2012-02-06 17:03:04 -0600 (Mon, 06 Feb 2012) $
 */
 
#ifndef VP_THERMAL_RINGING_H
#define VP_THERMAL_RINGING_H

#include "vp886_registers.h"
#include "vp886_math.h"

/* Thermal Ringing Timer duration in ms. This needs to be short enough
 * so that the SADC and VADC buffers are emptied often enough not
 * to overflow. At the same time, the more often the timer fires,
 * the greater amount of SPI traffic to read the buffers. It's
 * a tradeoff that is system dependant. Choose wisely.
 */
#define VP_886_THERMAL_RINGING_TIMER_DURATION 10

/* Timer to keep ILR "high long enough" when entering ringing. */
#define VP_886_THERMAL_RINGING_DEBOUNCE_DURATION 200

/* Specifies the sampling rate for the SADC and VADC */
#define VP_886_THERMAL_RINGING_SADC_DRATE VP886_R_SADC_DRATE_GROUP_500HZ
#define VP_886_THERMAL_RINGING_VADC_DRATE VP886_R_VADC_DRATE_500HZ

/* In order to easily re-use the NGSLAC code, we scale the inputs to use
 * that of the NGSLAC */
#define VAB_INPUT_FULL_SCALE 240
#define VAB_DESIRED_FULL_SCALE 400

#define VBAT_INPUT_FULL_SCALE 240
#define VBAT_DESIRED_FULL_SCALE 400

#define IMT_INPUT_FULL_SCALE 1191
#define IMT_DESIRED_FULL_SCALE 1000

/* Size of data buffers used to hold data samples
 * obtained from the ADCs. This is arbitrary at the
 * moment and will likely be tweaked as necessary
 * to avoid line object bloat while still holding
 * enough samples for the algorithm to process.
 * This could simply be just the size of the HW
 * ADC buffers (12) or maybe some more just in-case
 * a delay in processing is required.
 */
#define VP_886_THERMAL_RINGING_BUF_SIZE 32

/**
 * This structure defines a pair of filter delay elements, including one for the
 * input path and one for the output path. The client must allocate an array of
 * these, sized according to the desired filter order. For example, a 2nd-order
 * filter requires a 2 element array of this object.
 *
 * \ingroup DF1_FILTER
 */
typedef struct DF1FilterDelayPairStruct {
    frac16 x;
    frac16 y;
} DF1FilterDelayPair;

/**
 * This structure defines a pair of filter coefficients, including one for the
 * input path and one for the output path. The client must provide an
 * initialized array of filter coefficients with size equal to the filter order
 * (max(N,M)) \b plus \b 1. The "plus 1" is required because the first
 * coefficient pair in the array contains the coefficients for the present input
 * and output sample. The second coefficient pair contains the gains for the
 * first input and output delay elements and so on. Note that \em b coefficients
 * are for the input path and \em a coefficients are for the output path.
 *
 * \note    coeff[0].a is \b not interpreted as a signed fractional gain like
 *          all other coefficients. Instead it is interpreted as the base-2 log
 *          of the filter output gain, which can be though of as a binary
 *          exponent correction factor applied to the filter output. Positive
 *          and negative integers are allowed, as is zero (which is the most
 *          common case).
 *
 * \ingroup DF1_FILTER
 */
typedef struct DF1FilterCoeffPairStruct {
    frac16 b;
    frac16 a;
} DF1FilterCoeffPair;

/**
 * "Hybrid" (mixed single/double-precision) version of ::DF1FilterDelayPair.
 *
 * \ingroup DF1_FILTER
 */
typedef struct DF1HybridFilterDelayPairStruct {
    frac16 x;
    frac32 y;
} DF1HybridFilterDelayPair;

/**
 * "Hybrid" (mixed single/double-precision) version of ::DF1FilterCoeffPair.
 *
 * \ingroup DF1_FILTER
 */
typedef struct DF1HybridFilterCoeffPairStruct {
    frac16 b;
    frac32 a;
} DF1HybridFilterCoeffPair;

/**
 * This struct defines a single-precision filter object, which includes a pointer
 * to delay element pairs, a pointer to coefficient pairs, and the filter order.
 * The client must allocate storage for one of these for each single-precision
 * filter instance.
 *
 * \ingroup DF1_FILTER
 */
typedef struct DF1FilterStruct {
    DF1FilterDelayPair* pData;
    const DF1FilterCoeffPair* pCoef;
    unsigned int order;
} DF1Filter;

/**
 * This struct defines a "hybrid" mixed-precision filter object, which includes
 * a pointer to delay element pairs, a pointer to coefficient pairs, and the
 * filter order. The client must allocate storage for one of these for each
 * hybrid filter instance.
 *
 * \ingroup DF1_FILTER
 */
typedef struct DF1HybridFilterStruct {
    DF1HybridFilterDelayPair* pData;
    const DF1HybridFilterCoeffPair* pCoef;
    unsigned int order;
} DF1HybridFilter;

/* SLIC power calculation struct */
typedef struct SlicPowerIntegratorStruct {
    int16 samples;
    frac32 batPwr;
    frac32 loadPwr;
    frac32 imtSqr;
} SlicPowerIntegrator;

/* There will an instance on this struct in each line object */
typedef struct RingPowerAdaptChannelDataStruct {
    const unsigned short chan;
    int16 halfCycCount;
    int16 halfCycIdx;
    SlicPowerIntegrator integrator;
    SlicPowerIntegrator halfCyc[2];
    struct {
        DF1HybridFilter object;
        DF1HybridFilterDelayPair data[1];
    } filter;
    struct {
        DF1HybridFilterDelayPair filterData[1];
        frac16 ringGain;
    } saved;
    frac16 ringGain;
} RingPowerAdaptChannelData;

/* Need to define a struct that contains an instance of the above
 * along with the necessary buffers and indicies for the SADC
 * VADC data collection for Vring, Vbat, and IMT
 */
typedef struct RingPowerAdaptModuleStruct {
    RingPowerAdaptChannelData rpaChanData;
    int16 vtrBuf[VP_886_THERMAL_RINGING_BUF_SIZE];
    int16 vbatBuf[VP_886_THERMAL_RINGING_BUF_SIZE];
    int16 imtBuf[VP_886_THERMAL_RINGING_BUF_SIZE];
    int16 vtrBufSWWrtIdx;
    int16 vtrBufSWRdIdx;
    int16 vbatBufSWWrtIdx;
    int16 vbatBufSWRdIdx;
    int16 imtBufSWWrtIdx;
    int16 imtBufSWRdIdx;
    int16 halfCycleTickCount;
    int16 halfCycleTicks[2];
    int16 rspt;
    int16 rsptLow;
    int16 rsptHigh;
    int16 rtth;
    int16 vtrDelay;
    int16 vbatDelay;
    int16 imtDelay;
    uint16 rFuse;
    int16 samplesAvail;
    bool firstBufferThrownOut;
    bool crunchTheNumbers;
    uint8 ringGenParams[VP886_R_RINGGEN_LEN];
    uint8 loopSup[VP886_R_LOOPSUP_LEN];
    uint16 tStamp;
    bool exceedPower;
    bool debouncing;
} RingPowerAdaptModuleType;

#endif /* VP_THERMAL_RINGING_H */
