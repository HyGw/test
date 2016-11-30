/** \file vp886_ring_pow_adapt.c
 * vp886_ring_pow_adapt.c
 *
 *  This file contains the Ringing Thermal Power Management functions for
 *  the Vp886 device API.
 *
 * Copyright (c) 2011, Microsemi Corporation
 *
 * $Revision: 10916 $
 * $LastChangedDate: 2013-03-28 10:32:30 -0500 (Thu, 28 Mar 2013) $
 */

#include "vp_api_cfg.h"

#ifdef VP886_INCLUDE_THERMAL_RINGING

#if defined (VP_CC_886_SERIES)

/* INCLUDES */
#include "vp_api_types.h"
#include "vp_pulse_decode.h"
#include "vp_hal.h"
#include "vp_api_int.h"
#include "vp886_api.h"
#include "vp886_api_int.h"
#include "sys_service.h"
#include "vp886_math.h"

/* As a note for the reader:
 *
 * In the comments, the terms:
 *
 * Thermal Ringing
 * Power Adaptation
 * Ringing Power
 * The algorithm
 * .
 * .
 *
 * or any combination or subset thereof are all referring to the same thing
 *
 * Also, Vab, Vtr, Vtip-ring are all referring to the same thing
 */

/* Uncomment to display warning messages on the console if SADC/VADC
 * data buffers overflow.
 */
#define INCLUDE_BUFFER_OVERFLOW_WARNINGS

/* See comments below for a description of this struct */
/* Lifted from NGSLAC */
static struct RingPowerAdaptConfigStruct {
    int16 filterPreScale;
    frac16 filterInMax;
    frac16 filterInMin;
    int16 filterPostScale;
    int16 tConsiderSwitchToSlowMode;
    int16 tForceSwitchToSlowMode;
    int16 tConsiderSwitchToFastMode;
    frac16 fastToSlowThld;
    frac16 slowToFastThld;
    frac16 heavyRingAttnThld;
    DF1HybridFilterCoeffPair fastAdaptCoeffs[2];
    DF1HybridFilterCoeffPair slowAdaptCoeffs[2];
} ringPowerAdaptConfig = {
    /* In the comments below, "error term" refers to (Pt-Pm)/Pt, where Pt is
     * host-specified target power and Pm is measured SLIC power dissipation. */

    /* 'filterPreScale' determines the exponential gain (*2^N) applied to error
     * term samples before they are limited for filter input. This contributes
     * to the overall control loop gain. Yadu determined experimentally that a
     * gain of 16 here seems to work nicely. */
    4,

    /* 'filterInMax' determines the upper bound applied to error term samples
     * before the filter input. Clamping the error term to <= 0 prevents the
     * filter from charging to large positive values when the measured power
     * is much less than the target power. This is desirable to decrease
     * reaction time when the measured power increases suddenly. However, this
     * also slows the recovery of the error term to "no correction" (zero)
     * when the measured power drops below the target power again. Yadu likes
     * this value anyway. */
    0,

    /* 'filterInMin' determines the lower bound applied to error term samples
     * before the filter input. This can be used in conjunction with the filter
     * post-scale to set the minimum error term (maximum attenuation) applied to
     * the ringing amplitude. However, setting this to anything greater than
     * negative full-scale somewhat reduces how quickly the filter output moves
     * toward negative values (applying attenuation) when measured power greatly
     * exceeds the target power. For this reason, I am not limiting negative
     * inputs to the filter, and instead limiting the final attenuation output
     * of the control loop. This differs slightly from Yadu's strategy. */
    -32768,

    /* 'filterPostScale' determines the exponential gain (*2^N) applied to error
     * term samples on exit from the loop filter. So far we've had no reason to
     * change this from unity (*2^0). */
    0,

    /* 'tConsiderSwitchToSlowMode' is the time after which the adaptation
     * algorithm considers changing from fast to slow mode based on the measured
     * vs. target SLIC power dissipation. This time is measured in "ticks" of
     * the feedback loop, which occur at zero-crossings in the ringing waveform,
     * and therefore are essentially ringing half-cycles. The half-cycle counter
     * is reset at channel (or device) reset and any time the adaptation
     * algorithm state machine decides to return to fast mode due to measured
     * power changing significantly. */
    10,

    /* 'tForceSwitchToSlowMode' is the time after which the adaptation algorithm
     * unconditionally changes from fast to slow adaptation mode, regardless of
     * the measured SLIC power dissipation. This time is in units of ringing
     * half-cycles. This is essentially the maximum duration of fast mode in
     * ringing half-cycles. */
    100,

    /* 'tConsiderSwitchToFastMode' is the time after which the adaptation
     * algorithm considers changing from slow to fast mode based on the measured
     * vs. target SLIC power dissipation. This time is also in units of ringing
     * half-cycles. Since the half-cycle counter is always equal to
     * 'tForcSwitchToSlowMode' when the algorithm first enters slow adapation
     * mode (whether due to measured power threshold crossing or forced switch),
     * the *minimum* time spent in slow adaptation mode (in ringing half-cycles)
     * is equal to 'tConsiderSwitchToFastMode' minus 'tForceSwitchToSlowMode'.
     * The objective of blindly staying in slow adaptation mode for some time is
     * to prevent the algorithm from oscillating between slow and fast mode.
     * Hopefully, allowing the algorithm to settle in slow mode will avoid such
     * oscillations, without ignoring significant SLIC power changes for too
     * many half-cycles. */
    120, /* tForceSwitchToSlowMode + 20 */

    /* 'fastToSlowThld' is the threshold below which the difference between the
     * measured and target power (Pm-Pt) must drop in order for the ring power
     * adaptation algorithm to switch from fast to slow adaptation mode. In
     * other words, the adaptation algorithm remains in fast mode until the
     * measured power exceeds the target power by at least this many Watts, or
     * until a fast mode timer expires. */
    410, /* 0.5/40 Watts * 32768 */

    /* 'slowToFastThld' is the threshold above which the difference between the
     * measured and target power (Pm-Pt) must rise in order for the ring power
     * adaptation algorithm to switch from slow to fast adaptation mode. In
     * other words, the adaptation algorithm changes from slow to fast mode when
     * the measured power exceeds the target power by at least this many Watts.
     *
     * This threshold also determines when the algorithm switches from slow to
     * fast mode because the measured power drops *below* the target power. In
     * other words, when (Pt-Pm) is greater than this and other conditions are
     * met (below) the adapatation algorithm changes from slow to fast mode. */
    1229, /* 1.5/40 Watts * 32768 */

    /* 'heavyRingAttnThld' is the ringing signal gain below which the signal is
     * considered "heavily" attenuated. This theshold helps determine when it is
     * worthwhile to switch to (or remain in) the fast adaptation mode when the
     * measured SLIC power is significantly less than the target SLIC power. If
     * the ringing signal gain is greater than this, we will not switch to (or
     * remain in) the fast adaptation mode, regardless of how low the measured
     * SLIC power is. */
     26214, /* 80% */

    /* These are the coefficients for the control loop filter when in the fast
     * adaptation mode. The normalized cut-off frequency is fixed at 0.025
     * (1/40). Remember that the sample frequency is twice the ringing frequency
     * because the filter is clocked at the zero-cross. Thus, the actual cut-off
     * frequency (Fc) in Hz varies according to the relation Fc = Wn * (Fs / 2),
     * where Fs is the sample frequency in Hz and Wn is the normalized cut-off
     * frequency. For example:
     *  Fring (Hz)      Fs (Hz)     Fc (Hz)     T (sec)
     *  16              32          0.4000      0.3979
     *  20              40          0.5000      0.3183
     *  25              50          0.6250      0.2546
     *
     * According to a Yadu email, the desired time constant (T) is somewhere in the
     * neighborhood of 300 ms, so Wn of 0.025 seems a reasonable guess. */
    {
        {   1239,           0 },    /* 0.0378047541708965,  1.000000000000000 */
        {   1239, -1985113465 },    /* 0.0378047541708965, -0.924390491658207 */
    },

    /* These are the coefficients for the control loop filter when in the slow
     * adaptation mode. The normalized cut-off frequency is fixed at 0.0025
     * (1/400).
     *  Fring (Hz)      Fs (Hz)     Fc (Hz)     T (sec)
     *  16              32          0.0400      3.9789
     *  20              40          0.0500      3.1831
     *  25              50          0.0625      2.5465 */
    {
        {    128,           0 },    /* 0.0039116499112466,  1.0               */
        {    128, -2130683240 },    /* 0.0039116499112466, -0.992176700177507 */
    },
};

/* Prototypes for filter functions */
static VpStatusType
DF1HybridFilterInit(
    VpLineCtxType *pLineCtx,
    DF1HybridFilter* pFilter,
    DF1HybridFilterDelayPair* pDelayBuf,
    const DF1HybridFilterCoeffPair* pCoeffs,
    uint16 order);

static void
DF1HybridFilterReset(
    VpLineCtxType *pLineCtx,
    const DF1HybridFilter* pFilter,
    frac16 value);

static frac32
DF1HybridFilterClock(
    VpLineCtxType *pLineCtx,
    const DF1HybridFilter* pFilter,
    frac16 sample);

/* Prototypes for internal helper functions */
static VpStatusType
Vp886ThermalRingingADCCfg(
    VpLineCtxType *pLineCtx);

static VpStatusType
Vp886ThermalRingingGetSADCData(
    VpLineCtxType *pLineCtx);

static void
Vp886ThermalRingingProcessData(
    VpLineCtxType *pLineCtx);

static void
ComputeRingGainFromSlicPower(
    VpLineCtxType *pLineCtx,
    RingPowerAdaptChannelData* pRPACD,
    frac32 slicPwrInt,
    int16 samples);

static void
MeasureRingPower(
    VpLineCtxType *pLineCtx);

static void
RingPowerAdaptionLoopIteration(
    VpLineCtxType *pLineCtx,
    void* arg);

static void
RingPowerAdaptChanReset(
    VpLineCtxType *pLineCtx);

static int16
Vp886ThermalRingingScaleData(
    int16 inputVal,
    int16 origFullScale,
    int16 newFullscale);

static void
Vp886ThermalRingingSetTickCount(
    VpLineCtxType *pLineCtx);

static void
Vp886ThermalRingingSetFuseResistance(
    VpLineCtxType *pLineCtx,
    int16 rFuse);

static uint8
RingTripUpdateRtth(VpLineCtxType *pLineCtx,
    frac16 ringGain,
    bool   fastMode
);

static bool
RingPowerAdaptIsFast(
    VpLineCtxType *pLineCtx
);

/*#define DBG_BUFF_DATA*/
#ifdef DBG_BUFF_DATA
int16 vtrDbgBuf[1024], vbatDbgBuf[1024], imtDbgBuf[1024];
int16 vtrDbgIdx, vbatDbgIdx, imtDbgIdx;
#endif

/**
 * RingPowerAdaptInit()
 *
 * This function initializes thermal ringing power adaptation for the
 * specified line, most notably the lines RingPowerAdaptChannelData data
 * structure. This includes data related to the filters and state machine.
 *
 * This function is called from with VpInitLineInit()
 *
 * \param[in]   VpLineCtxType *pLineCtx        the line's line context
 *
 * \retval      VP_STATUS_SUCCESS
 */
EXTERN VpStatusType
RingPowerAdaptInit(
    VpLineCtxType *pLineCtx)
{
    Vp886LineObjectType*            pLineObj = pLineCtx->pLineObj;
    int8                                chan = pLineObj->channelId;
    RingPowerAdaptChannelData*        pRPACD = &pLineObj->ringPowerAdapt.rpaChanData;
    VpStatusType                      status = VP_STATUS_SUCCESS;

    VP_API_FUNC_INT(VpLineCtxType, pLineCtx, ("RingPowerAdaptInit+"));

    /* Init the otherwise constant channel ID for each array element. */
    *(unsigned short*)&pRPACD->chan = chan;

    status = DF1HybridFilterInit(
        pLineCtx,
        &pRPACD->filter.object,
        &pRPACD->filter.data[0],
        &ringPowerAdaptConfig.fastAdaptCoeffs[0],
        1);

    RingPowerAdaptChanReset(pLineCtx);

    VP_API_FUNC_INT(VpLineCtxType, pLineCtx, ("RingPowerAdaptInit-"));

    return status;

} /* RingPowerAdaptInit() */

/**
 * RingPowerAdaptChanReset()
 *
 * This function does some of the actual work of the RingPowerAdaptInit()
 * above, namely initializing the line's RingPowerAdaptChannelData struct
 *
 * This function is called from with RingPowerAdaptInit()
 *
 * \param[in]   VpLineCtxType *pLineCtx        the line's line context
 *
 * \retval      void
 */
static void
RingPowerAdaptChanReset(
    VpLineCtxType *pLineCtx)
{
    Vp886LineObjectType*          pLineObj = pLineCtx->pLineObj;
    RingPowerAdaptModuleType*         pRPA = &pLineObj->ringPowerAdapt;
    RingPowerAdaptChannelData*      pRPACD = &pRPA->rpaChanData;

    VP_API_FUNC_INT(VpLineCtxType, pLineCtx, ("RingPowerAdaptChanReset+"));

    /* Init state machine related vars */
    pRPACD->halfCycCount = 0;
    pRPACD->halfCycIdx = -1; /* IDLE */

    /* Clear out filter and integrator related vars */
    VpMemSet(&pRPACD->integrator, 0, sizeof(pRPACD->integrator));
    VpMemSet(&pRPACD->halfCyc[0], 0, sizeof(pRPACD->halfCyc));
    DF1HybridFilterReset(pLineCtx,&pRPACD->filter.object, 0);
    VpMemSet(&pRPACD->saved.filterData[0], 0, sizeof(pRPACD->saved.filterData));
    pRPACD->filter.object.pCoef = &ringPowerAdaptConfig.fastAdaptCoeffs[0];

    /* Default ring gain to unity to start out */
    pRPACD->ringGain = 0x7FFF;
    pRPACD->saved.ringGain = 0x7FFF;

    /* Set the fuse resistance */
    Vp886ThermalRingingSetFuseResistance(pLineCtx, pLineObj->lineTopology.rInsideDcSense);

    /* Be sure to update ring param only on zero cross */
    if (!(pLineObj->registers.ssCfg[1] & VP886_R_SSCFG_RING_UPDATE)) {
        pLineObj->registers.ssCfg[1] |= VP886_R_SSCFG_RING_UPDATE;
        VpSlacRegWrite(NULL, pLineCtx, VP886_R_SSCFG_WRT, VP886_R_SSCFG_LEN, pLineObj->registers.ssCfg);
    }

    /* Save off the original RTTH value */
    pRPA->rtth = pLineObj->registers.loopSup[2] & VP886_R_LOOPSUP_RTRIP_THRESH;

    VP_API_FUNC_INT(VpLineCtxType, pLineCtx, ("RingPowerAdaptChanReset-"));

} /* RingPowerAdaptChanReset() */

/**
 * RingPowerAdaptIsFast()
 *
 * This function indicates whether the Ring Power Adaptation feedback loop is in
 * "fast" adaptation mode.
 *
 * \param[in]   VpLineCtxType *pLineCtx        the line's line context
 *
 * \retval  TRUE    The specified channel is in fast adapt mode.
 * \retval  FALSE   The specified channel is not in fast adapt mode.
 */
static bool
RingPowerAdaptIsFast(
    VpLineCtxType *pLineCtx)
{
    Vp886LineObjectType*        pLineObj = pLineCtx->pLineObj;

    return (pLineObj->ringPowerAdapt.rpaChanData.filter.object.pCoef ==
            &ringPowerAdaptConfig.fastAdaptCoeffs[0]) ? TRUE : FALSE;

} /* RingPowerAdaptIsFast() */

/**
 * MeasureRingPower()
 *
 * \todo Document me!
 *
 * This gets called after the SADC and VADC buffers have been read,
 * and data collected and calibration applied
 *
 *  \param[in]   VpLineCtxType *pLineCtx        the line's line context
 *
 */

/* This the equivalent to RingPowerMeasurementJob(void* arg) in the NGSLAC. There, it
 * is set up as a 2kHz Job. Here, we will just have it run at 500Hz as the SADC/VADC samples
 * come in. For now, it will called in Vp886ThermalRingingProcessData() via Vp886ThermalRingingHandler()
 */
static void
MeasureRingPower(
    VpLineCtxType *pLineCtx)
{
    Vp886LineObjectType*          pLineObj = pLineCtx->pLineObj;
    RingPowerAdaptModuleType*         pRPA = &pLineObj->ringPowerAdapt;
    RingPowerAdaptChannelData*      pRPACD = &pRPA->rpaChanData;

    VP_API_FUNC_INT(VpLineCtxType, pLineCtx, ("MeasureRingPower+"));

    /* For all available samples .. */
    while(pRPA->samplesAvail > 0) {
        frac16 vab, imt;
        SlicPowerIntegrator acc;

        /* The half-cycle index variable serves two purposes:
         *
         *  1.  It is a single-bit counter, incremented at each half-cycle, that
         *      determines which one of two half-cycle buffers the integrator
         *      data is copied into.
         *
         *  2.  It is the state variable for a very simple state machine, used
         *      to detect the beginning and end of ringing bursts, where special
         *      actions are taken to condition the power adaptation loop filter.
         *      The state diagram follows.
         *
         *      +------+  state is ringing   +------+                   +------+
         *      |      |                     |      |------------------>|      |
         *      | IDLE |-------------------->| EVEN |  half-cycle end   |  ODD |
         *      | (-1) |                     |  (0) |<------------------| (+1) |
         *      +------+                     +------+                   +------+
         *          ^                            |                          |
         *          |    state is not ringing    |                          |
         *          \----------------------------+--------------------------/
         */

        /* It should be impossible for the code to even take this route if
         * the half cycle index is -1. But if it does, bail out now */
        if (pRPACD->halfCycIdx < 0) break;

        pRPA->samplesAvail--;

        /* Grab the VAB and the IMT. Scale them to match that of the original
         * NGSLAC code */
        vab = Vp886ThermalRingingScaleData(pRPA->vtrBuf[pRPA->vtrBufSWRdIdx++],
            VAB_INPUT_FULL_SCALE, VAB_DESIRED_FULL_SCALE);
        pRPA->vtrBufSWRdIdx %= VP_886_THERMAL_RINGING_BUF_SIZE;

        imt = Vp886ThermalRingingScaleData(pRPA->imtBuf[pRPA->imtBufSWRdIdx++],
            IMT_INPUT_FULL_SCALE, IMT_DESIRED_FULL_SCALE);
        pRPA->imtBufSWRdIdx %= VP_886_THERMAL_RINGING_BUF_SIZE;

        #ifdef DBG_BUFF_DATA
        vtrDbgBuf[vtrDbgIdx++] = vab;
        if (vtrDbgIdx >= 1024) vtrDbgIdx = 1023;
        imtDbgBuf[imtDbgIdx++] = imt;
        if (imtDbgIdx >= 1024) imtDbgIdx = 1023;
        #endif

        /* Sample the active battery voltage, compute the instantaneous battery
         * power output, and accumulate. */
        {
            frac16 vBat;
            frac32 batPwr;

            /* Grab the VAB and scale to match NGSLAC code */
            vBat = Vp886ThermalRingingScaleData(pRPA->vbatBuf[pRPA->vbatBufSWRdIdx++],
                VBAT_INPUT_FULL_SCALE, VBAT_DESIRED_FULL_SCALE);
            pRPA->vbatBufSWRdIdx %= VP_886_THERMAL_RINGING_BUF_SIZE;

            #ifdef DBG_BUFF_DATA
            vbatDbgBuf[vbatDbgIdx++] = vBat;
            if (vbatDbgIdx >= 1024) vbatDbgIdx = 1023;
            #endif

            /* Note that battery voltages are converted from +/- 200V full-scale
             * to +/- 400V full-scale for two reasons:
             *  + so that the voltage between rails can be calculated for any
             *    battery voltages without saturating (e.g. +200 - -200 = 400)
             *  + so that battery power samples are in same scale as load power,
             *    which makes net power calculations easy. */
            batPwr = Vp886L_mult(Vp886abs_s(vBat), Vp886abs_s(imt));
            batPwr = Vp886L_shr_r(batPwr, 7); /* batPwr /= 128 */
            acc.batPwr = Vp886L_add(pRPACD->integrator.batPwr, batPwr);

        }

        /* Compute and accumulate the instantaneous load power. */
        {
            frac32 loadPwr;

            /*loadPwr = L_mult(abs_s(vab), abs_s(imt));*/
            loadPwr = Vp886L_mult(Vp886abs_s(vab), Vp886abs_s(imt));
            loadPwr = Vp886L_shr_r(loadPwr, 7); /* loadPwr /= 128 */
            acc.loadPwr = Vp886L_add(pRPACD->integrator.loadPwr, loadPwr);
        }

        /* Compute and accumulate the instantaneous squared load current. */
        {
            frac32 imtSqr;

            imtSqr = Vp886L_mult(imt, imt);
            imtSqr = Vp886L_shr_r(imtSqr, 7); /* imtSqr /= 128 ---- jby: consider making this configable */
            acc.imtSqr = Vp886L_add(pRPACD->integrator.imtSqr, imtSqr);
        }

        /* Count the samples accumulated thus far. Note the saturating add here
         * to avoid counter overflow in the unexpected case where the ringing
         * half-cycle is insanely long. */
        acc.samples = Vp886add(pRPACD->integrator.samples, 1);

        /* Check for a ringing half cycle */
        pRPA->halfCycleTickCount--;

        if (pRPA->halfCycleTickCount == 0) {
            int16 halfCycIdx;

            halfCycIdx = pRPACD->halfCycIdx;

            /* Copy local accumulator state to one of two half-cycle buffers and
             * clear the local accumulator. */
            VpMemCpy(&pRPACD->halfCyc[halfCycIdx], &acc,
                sizeof(pRPACD->halfCyc[halfCycIdx]));
            VpMemSet(&acc, 0, sizeof(acc));

            /* Setup the half-cycle index to write the other buffer the next
             * time around. */
            pRPACD->halfCycIdx = (int16)(halfCycIdx ^ 1);

            pRPA->halfCycleTickCount = pRPA->halfCycleTicks[halfCycIdx];

            /* Execute an interation of the control loop filter */
            RingPowerAdaptionLoopIteration(pLineCtx, (void*)pRPACD);
        }

        /* Copy local accumulator state to static integrator. */
        VpMemCpy(&pRPACD->integrator, &acc, sizeof(pRPACD->integrator));
    }

    VP_API_FUNC_INT(VpLineCtxType, pLineCtx, ("MeasureRingPower-"));

} /* MeasureRingPower() */

/**
 * RingPowerAdaptionLoopIteration()
 *
 * \todo Document me!
 *
 * This function is called every half-ringing cycle
 *
 *  \param[in]   VpLineCtxType *pLineCtx        the line's line context
 *  \param[in]   void* arg                      void pointer that gets
 *                                              casted as a pointer to this
 *                                              line's RingPowerAdaptChannelData
 *
 */
static void
RingPowerAdaptionLoopIteration(
    VpLineCtxType *pLineCtx,
    void* arg)
{
    Vp886LineObjectType*            pLineObj = pLineCtx->pLineObj;
    RingPowerAdaptModuleType*           pRPA = &pLineObj->ringPowerAdapt;
    RingPowerAdaptChannelData*        pRPACD = (RingPowerAdaptChannelData*)arg;

    frac32 batPwrInt, loadPwrInt, fusePwrInt, slicPwrInt;
    int16 samples;


    VP_API_FUNC_INT(VpLineCtxType, pLineCtx, ("RingPowerAdaptionLoopIteration+"));

    /* Calculate the integration period, which is the number of samples
     * taken through a complete ringing cycle.
     *
     * NOTE: In the case of the very first half-cycle of a ringing pulse,
     * only one of the two half-cycle integrators contains meaningful data.
     * The net sample count will be 1/2 of the ringing period, so the first
     * half-cycle is effectively doubled. */
    samples = Vp886add(pRPACD->halfCyc[0].samples, pRPACD->halfCyc[1].samples);

    /* Samples have been pre-scaled by a factor of 128, which should be
     * sufficient to collect one full-cycle of data for ringing signals
     * down to 16 Hz (2000 Hz / 16 Hz = 125 samples). If we exceed 128
     * samples then the accumulators could theoretically overflow. */
    /* The above can probably be modified since we are samplting at 500Hz */

    /* Compute the battery power for the full cycle as the sum of the battery
     * power for the last two half-cycles. */
    batPwrInt = Vp886L_add(pRPACD->halfCyc[0].batPwr, pRPACD->halfCyc[1].batPwr);

    /* Compute the load power for the full cycle as the sum of the load
     * power for the last two half-cycles. */
    loadPwrInt = Vp886L_add(pRPACD->halfCyc[0].loadPwr, pRPACD->halfCyc[1].loadPwr);

    /* Compute power dissipated through the fuse resistors as IMT^2 * 2 * RF.
     * RF is assumed constant during ringing. RF is 32-bit signed integer
     * in the range of 0 to 100 Ohms. The conversion function is derived below.
     *
     * Let 'imt' be the real metallic current in Amps.
     * Let 'IMT' be the signed fractional 1.15 current from the AFE driver.
     *
     *  imt = IMT * 0.1 * 2^-15                                         (1)
     *
     * Instantaneous power through the fuse resistor is calculated as
     *
     *  pf = imt^2 * rf                                             (2)
     *
     * Let real 'rf' equal firmware 'RF', and substitute eq 1 for imt.
     *
     *  pf = (IMT * 0.1 * 2^-15)^2 * RF
     *  pf = IMT^2 * 0.1^2 * 2^(-15*2) * RF
     *  pf = IMT^2 * 0.01 * 2^-30 * RF                              (3)
     *
     * Recall that the common conversion from "real" power (pptc) in Watts
     * to "firmware" power (PPTC) in signed fractional 1.31 is
     *
     *  pf = PF * 40 * 2^-31                                        (4)
     *
     * Solving equations 3 and 4 for PPTC gives
     *
     *  PF = IMT^2 * 0.01 * 2^-30 * RF * 1/40 * 2^31
     *  PF = IMT^2 * RF * 1/2000                                    (5)
     *
     * Equation 5 is what we want to calculate for every IMT sample, taking
     * the average of this instantaneous power over a complete ring period.
     * However, it is more efficient to accumulate the sum of IMT^2 and
     * multiply this sum by RF * 1/2000 once for each full ring period.
     * Remember that the power numbers we've computed thus far are the sum
     * (integral) of power over a ring cycle. PTC power is effectively
     * handled the same way.
     *
     * NOTE: The fractional mult() intrinsics have an implicit multiply by
     * two (left shift by one), so we need NOT explicitly multiply RF by
     * two here to account for the power dissipated in both PTC devices. */
    {
        frac32 imtSqrInt;
        frac16 rFuseLocal;

        /* Compute RF/2000 as signed fractional 1.15. The result must be
         * between 0 and 32767 (+0.99997). */
        rFuseLocal = Vp886saturate(
            Vp886Int32Div(pRPA->rFuse * 32768, 2000));

        imtSqrInt = Vp886L_add(pRPACD->halfCyc[0].imtSqr, pRPACD->halfCyc[1].imtSqr);
        fusePwrInt = Vp886L_mult_ls(imtSqrInt, rFuseLocal);
    }

   /* Compute the integral of the power dissipated by the SLIC. */
    slicPwrInt = Vp886L_sub(batPwrInt, Vp886L_add(loadPwrInt, fusePwrInt));

    /* Power debug output */
#if defined(VP_DEBUG) && defined(VP_DBG_RPM)
    {
        frac32 batPwrDbg, loadPwrDbg, fusePwrDbg, slicPwrDbg;
        int16 fScale = Vp886Int32Div(620L, pRPA->halfCycleTicks[0] + pRPA->halfCycleTicks[1]);
        uint16 tStamp = Vp886GetTimestamp(pLineCtx->pDevCtx);

        batPwrDbg = (batPwrInt * fScale) >> 18;
        loadPwrDbg = (loadPwrInt * fScale) >> 18;
        fusePwrDbg = (fusePwrInt * fScale) >> 18;
        slicPwrDbg = (slicPwrInt * fScale) >> 18;

        /* Display a debug message every 200ms */
        if (ABS(tStamp - pRPA->tStamp) >= 400) {
            pRPA->tStamp = tStamp;
            VP_RPM(VpLineCtxType, pLineCtx, ("B = %4ld mW, L = %4ld mW, F = %4ld mW, S = %4ld mW, ringV = %6ld mV, treshold = %d, ts = %u",
                batPwrDbg, loadPwrDbg, fusePwrDbg, slicPwrDbg, ((pLineObj->ringAmplitudeCal * pRPACD->ringGain) >> 15) * 473L / 100, pRPA->rspt * 12 / 10, tStamp));
        }
    }
#endif

    ComputeRingGainFromSlicPower(pLineCtx, pRPACD, slicPwrInt, samples);

    VP_API_FUNC_INT(VpLineCtxType, pLineCtx, ("RingPowerAdaptionLoopIteration-"));

} /* RingPowerAdaptionLoopIteration */


/**
 * ComputeRingGainFromSlicPower()
 *
 * \todo Document me!
 *
 * This function is called from above.
 *
 *  \param[in]   VpLineCtxType *pLineCtx            the line's line context
 *  \param[in]   RingPowerAdaptChannelData* pRPACD  thisline's RingPowerAdaptChannelData
 *  \param[in]   frac32 slicPwrInt                  integral of the power dissipated by the SLIC
 *  \param[in]   int16 samples                      number of cycles integrated?
 *
 */
static void
ComputeRingGainFromSlicPower(
    VpLineCtxType *pLineCtx,
    RingPowerAdaptChannelData* pRPACD,
    frac32 slicPwrInt,
    int16 samples)
{
    VpDevCtxType*                pDevCtx = pLineCtx->pDevCtx;
    Vp886DeviceObjectType*       pDevObj = pDevCtx->pDevObj;
    Vp886LineObjectType*        pLineObj = pLineCtx->pLineObj;
    RingPowerAdaptModuleType*       pRPA = &pLineObj->ringPowerAdapt;

    frac32 tgtSlicPwrInt, errSlicPwrInt;
    frac16 errRatio, ringGain, finalOutMax, finalOutMin;
    int16 halfCycCount;
    int32 newRingAmp;

    VP_API_FUNC_INT(VpLineCtxType, pLineCtx, ("ComputeRingGainFromSlicPower+"));

    /* Convert target SLIC power to same terms as integrated measured SLIC
     * power. Remember that power samples are pre-scaled by 2^-7 (1/128).
     *
     *  target integral = target * samples * 1/128
     *
     * Remember that L_mult has a built-in multiply-by-two, so
     *
     *  target integral = L_mult(target, samples) * 1/256
     *
     * However, we also want to put result in same format as the SLIC power
     * integral, which is signed 1.31, so we must left shift the result by 16.
     *
     *  target integral = L_mult(target, samples) * 1/256 * 2^16
     *  target integral = L_mult(target, samples) * 2^8 */
    /* The above can probably me modified since we are samplting at 500Hz */
    tgtSlicPwrInt = Vp886L_mult(pRPA->rspt, samples);
    tgtSlicPwrInt = Vp886L_shl(tgtSlicPwrInt, 8);

    /* Compute the difference between the target SLIC power multiplied by the
     * number of samples and the integral of measured SLIC power. This is equal
     * to (target SLIC power - average SLIC power) * samples. */
    errSlicPwrInt = Vp886L_sub(tgtSlicPwrInt, slicPwrInt);

    /* If we exceed the power limit, reduce the power limit to prevent battery hunting */
    if ((errSlicPwrInt < 0) && (pRPA->rspt > pRPA->rsptLow) && !(pRPA->debouncing)) {
        pRPA->rspt = pRPA->rsptLow;
    /* If we are below the power limit, increase the power limit to prevent battery hunting */
    } else if ((errSlicPwrInt > 0) && (pRPA->rspt == pRPA->rsptLow) && !(pRPA->debouncing)) {
        pRPA->rspt = pRPA->rsptHigh;
    }
    tgtSlicPwrInt = Vp886L_mult(pRPA->rspt, samples);
    tgtSlicPwrInt = Vp886L_shl(tgtSlicPwrInt, 8);
    errSlicPwrInt = Vp886L_sub(tgtSlicPwrInt, slicPwrInt);

    /* Run the state machine. The half-cycle counter is effectively the state
     * variable. Note that the saturating addition below is important. */
    halfCycCount = Vp886add(pRPACD->halfCycCount, 1);

    /* 'finalOutMax' determines the upper bound for the ring signal attenuation.
     * This should probably always be set to positive full-scale. */
    finalOutMax = 32767;

    /* 'finalOutMin' determines the lower bound for the ring signal attenuation.
     * This should never be less than zero. */
    finalOutMin = 32767L * pDevObj->options.adaptiveRinging.minVoltagePercent / 100;

    if (halfCycCount < ringPowerAdaptConfig.tConsiderSwitchToSlowMode) {
        /* Blindly stay in fast adaptation mode for now. */
    } else if (halfCycCount < ringPowerAdaptConfig.tForceSwitchToSlowMode) {
        frac32 fastAdaptExitThld;

        /* Convert the threshold for switching from fast to slow adaptation to
         * same units as integrated measured SLIC power. See above. */
        fastAdaptExitThld = Vp886L_mult(ringPowerAdaptConfig.fastToSlowThld, samples);
        fastAdaptExitThld = Vp886L_shl(fastAdaptExitThld, 8);

        if ((errSlicPwrInt > Vp886L_negate(fastAdaptExitThld)) &&
            ((pRPACD->ringGain > ringPowerAdaptConfig.heavyRingAttnThld) ||
             (errSlicPwrInt < fastAdaptExitThld))) {
            /* Measured power is less than target power plus some headroom
             *  -AND-
             * Ringing signal is close enough to full scale -OR- measured power
             * is greater than target power minus some headroom.
             * Switch to slow adaptation mode. */
            halfCycCount = ringPowerAdaptConfig.tForceSwitchToSlowMode;
        }

    } else if (halfCycCount < ringPowerAdaptConfig.tConsiderSwitchToFastMode) {
        /* Blindly stay in slow adaptation mode for now. */
    } else {
        frac32 slowAdaptExitThld;

        /* Convert the threshold for switching from slow to fast adaptation to
         * same units as integrated measured SLIC power. See above. */
        slowAdaptExitThld = Vp886L_mult(ringPowerAdaptConfig.slowToFastThld, samples);
        slowAdaptExitThld = Vp886L_shl(slowAdaptExitThld, 8);

        if (errSlicPwrInt < Vp886L_negate(slowAdaptExitThld)) {
            /* Measured power significantly exceeds the threshold. Switch to
             * fast adaptation mode. */
            halfCycCount = 1;
        }


        if ((errSlicPwrInt > slowAdaptExitThld) &&
            (pRPACD->ringGain < ringPowerAdaptConfig.heavyRingAttnThld)) {
            /* Measured power is significantly less than the threshold -AND- we
             * are significantly attenuating the ringing signal (gain < 80%).
             * Switch to fast adaptation mode. */
            halfCycCount = 1;
        }
    }

    /* Evaluate the output of the state machine, which is basically the loop
     * filter coefficient selection. */
    if (halfCycCount < ringPowerAdaptConfig.tForceSwitchToSlowMode) {
        pRPACD->filter.object.pCoef = &ringPowerAdaptConfig.fastAdaptCoeffs[0];
    } else {
        pRPACD->filter.object.pCoef = &ringPowerAdaptConfig.slowAdaptCoeffs[0];
    }

    /* Save the updated state for the next iteration. */
    pRPACD->halfCycCount = halfCycCount;

    /* Compute gain adjustment as (Pt-Pm)/Pt. Note that Pt and Pm are
     * both integral of power over some number of samples. The integral
     * (multilpy-by-samples samples) in both terms cancel each other out
     * so we effectively compute (target-average)/target.
     *
     * Note that the filter pre-scale gain is also applied here, prior to
     * saturating the 32-bit division result to 16 bits. */
    {
        frac32 errRatio32;
        int16 n;

        n = Vp886norm_l(errSlicPwrInt);
        n = Vp886min16(n, Vp886norm_l(tgtSlicPwrInt));
        errSlicPwrInt = Vp886L_shl_nosat(errSlicPwrInt, n);
        tgtSlicPwrInt = Vp886L_shl_nosat(tgtSlicPwrInt, Vp886sub(n, 15));
        errRatio32 = Vp886Int32Div(errSlicPwrInt, tgtSlicPwrInt);
        errRatio32 = Vp886L_shl(errRatio32, ringPowerAdaptConfig.filterPreScale);
        errRatio = Vp886saturate(errRatio32);
    }

    /* Apply configurable clamp to filter input. */
    errRatio = Vp886min16(errRatio, ringPowerAdaptConfig.filterInMax);
    errRatio = Vp886max16(errRatio, ringPowerAdaptConfig.filterInMin);

    /* Low-pass filter the error feedback. */
    errRatio = (frac16)Vp886round32to16((int32)DF1HybridFilterClock(pLineCtx,&pRPACD->filter.object, errRatio));

    /* Apply filter post-scale gain. */
    errRatio = Vp886shl(errRatio, ringPowerAdaptConfig.filterPostScale);

    /* Add 1.0 to error term so that, when error term is zero, ring
     * amplitude is unchanged (no attenuation). */
    ringGain = Vp886sub(errRatio, -1 << 15); /* ringGain = errRatio - -1.0 */

    /* Apply configurable clamp to the final output. */

    ringGain = Vp886min16(ringGain, finalOutMax);
    ringGain = Vp886max16(ringGain, finalOutMin);

    /* Remember when the power limit is reached (aka SLIC power will exceed the specified limit) */
    if ((ringGain == finalOutMin) || pRPA->debouncing) {
        pRPA->exceedPower = TRUE;
    } else {
        pRPA->exceedPower = FALSE;
    }

    /* If the amplitude is changing and we're doing fixed battery and VP_ADAPT_RING_SHARED_TRACKER
       ringing on a tracker device, update the switcher ringing voltage */
    if (VP886_IS_TRACKER(pDevObj) && (pDevObj->options.adaptiveRinging.mode == VP_ADAPT_RING_SHARED_TRACKER) &&
        ((pLineObj->registers.swParam[0] & VP886_R_SWPARAM_RING_TRACKING) == VP886_R_SWPARAM_RING_TRACKING_DIS))
    {
        int32 battery_V;
        int16 new_battery_V;
        int16 new_battery_steps;
        uint8 swParam[VP886_R_SWPARAM_LEN];

        battery_V = ((pLineObj->registers.swParam[1] & VP886_R_SWPARAM_RINGING_V) * 5 + 5);
        new_battery_V = (battery_V * ringGain + 16384) >> 15;

        new_battery_steps = (new_battery_V - 5 + 4) / 5;
        new_battery_V = new_battery_steps * 5 + 5;

        /* Prevent the newly computed battery to be higher than the target */
        if (new_battery_V > battery_V) {
            new_battery_V = battery_V;
        }

        /* Prevent the battery to go below 60V */
        if (new_battery_V < 60) {
            new_battery_V = 60;
        }

        /* Update the ringing floor voltage if needed */
        if (ABS(new_battery_V - pLineObj->fixedRingBat) >= 5) {
            swParam[0] = pLineObj->registers.swParam[0];
            swParam[1] = pLineObj->registers.swParam[1] & ~VP886_R_SWPARAM_RINGING_V;
            swParam[1] |= new_battery_steps;
            swParam[2] = pLineObj->registers.swParam[2];
            VpSlacRegWrite(NULL, pLineCtx, VP886_R_SWPARAM_WRT, VP886_R_SWPARAM_LEN, swParam);

            /* Save the programmed ringing floor */
            pLineObj->fixedRingBat = new_battery_V;
        }
    }

    /* Calculate the new gain to be written into the sig gen register */
    /* This needs to be initialized somewhere. From the ringing profile ? */
    newRingAmp = pLineObj->ringAmplitudeCal * (int32)ringGain;
    newRingAmp += 0x00004000;
    newRingAmp >>= 15;

    /* Update the amplitude field of the ring gen params register */
    pRPA->ringGenParams[5] = (uint8)((newRingAmp & 0x0000FF00) >> 8);
    pRPA->ringGenParams[6] = (uint8)(newRingAmp & 0x000000FF);
    VpSlacRegWrite(NULL, pLineCtx, VP886_R_RINGGEN_WRT, VP886_R_RINGGEN_LEN, pRPA->ringGenParams);

    switch (pDevObj->options.adaptiveRinging.mode) {
        case VP_ADAPT_RING_POWER_SLIC:
            /* Update the ring trip threshold */
            pRPA->loopSup[2] &= ~(VP886_R_LOOPSUP_RTRIP_THRESH);
            pRPA->loopSup[2] |= RingTripUpdateRtth(pLineCtx, ringGain, RingPowerAdaptIsFast(pLineCtx));
            break;

        case VP_ADAPT_RING_SHARED_TRACKER:
            /* Loaded mode:
                   - Program a high ILR to prevent tripping (44mA)
                   - Max out RTTH (31.75mA)

               Open circuit mode:
                   - Program a medium ILR (31mA)
                   - Max out RTTH (31.75mA) */
            if (pLineObj->lowIlr) {
                if (pRPA->exceedPower) {
                    pRPA->loopSup[2] |= VP886_R_LOOPSUP_RTRIP_THRESH;
                    pRPA->loopSup[3] &= ~VP886_R_LOOPSUP_RING_CUR_LIM;
                    pRPA->loopSup[3] |= 0x16;
                } else {
                    pRPA->loopSup[2] |= VP886_R_LOOPSUP_RTRIP_THRESH;
                    pRPA->loopSup[3] &= ~VP886_R_LOOPSUP_RING_CUR_LIM;
                    pRPA->loopSup[3] |= 0x09;
                }
            }
            break;

        default:
            VP_ERROR(VpLineCtxType, pLineCtx, ("Adaptive ringing: Invalid mode %d", pDevObj->options.adaptiveRinging.mode));
            break;
    }

    VpSlacRegWrite(NULL, pLineCtx, VP886_R_LOOPSUP_WRT, VP886_R_LOOPSUP_LEN, pRPA->loopSup);

    /* Save the calculated gain away */
    pRPACD->ringGain = ringGain;

    VP_API_FUNC_INT(VpLineCtxType, pLineCtx, ("ComputeRingGainFromSlicPower-"));
}  /* ComputeRingGainFromSlicPower */


/**
 * Vp886ThermalRingingPrepare()
 *
 * Thermal Ringing Prepare. Should reset/intialize the thermal ringing variables,
 * data structures, etc. Should initialize the SADC block. Should start the
 * initial timer.
 *
 * This is called from Vp886SetLineStateFxsInt(). This may also be called when the
 * "make" part of a ring cadence is started (if it is not done with the above mentioned
 * function). This may also be called if a line state is returning to ringing upon
 * completion of a line test.
 *
 * There needs to be a flag to make sure this isn't called during a line test.
 *
 * There will probably be other scenarios and cases where calling this function
 * may or may not be allowed. TBD.
 *
 * Inputs:
 *  VpLineCtxType *pLineCtx         pointer to this line's line context
 *
 * Outputs:
 *  status
 */
/* Note: This function is always in flux and will settle down soon */

#define VTR_DELAY   2
#define VBAT_DELAY  2
#define IMT_DELAY   0

VpStatusType
Vp886ThermalRingingPrepare(
    VpLineCtxType *pLineCtx)
{
    VpDevCtxType*                pDevCtx = pLineCtx->pDevCtx;
    Vp886DeviceObjectType*       pDevObj = pDevCtx->pDevObj;
    Vp886LineObjectType*        pLineObj = pLineCtx->pLineObj;
    RingPowerAdaptModuleType*       pRPA = &pLineObj->ringPowerAdapt;
    RingPowerAdaptChannelData*    pRPACD = &pRPA->rpaChanData;

    VP_RPM(VpLineCtxType, pLineCtx, ("Adaptive ringing start"));

    VP_API_FUNC_INT(VpLineCtxType, pLineCtx, ("Vp886ThermalRingingPrepare+"));

    if (pLineCtx == NULL) {
        /* If this channel doesn't exist, leave */
        VP_API_FUNC_INT(VpLineCtxType, pLineCtx, ("Vp886ThermalRingingPrepare-"));
        return VP_STATUS_INVALID_ARG;
    }

    Vp886ThermalRingingSetTickCount(pLineCtx);

    /* Get the current values in the ring gen register so we don't
     * have to do a read-mod-write every time we update the amplitude
     * for a gain change. We will just do the write.
     */
    VpSlacRegRead(NULL, pLineCtx, VP886_R_RINGGEN_RD, VP886_R_RINGGEN_LEN, pRPA->ringGenParams);

    /* Configure the Supervision and Voice ADCs */
    Vp886ThermalRingingADCCfg(pLineCtx);

    /* Initialize the SADC buffer read and write indicies.
     * Are we going to carry over power info from one
     * ring burst to the next? If so, we probably don't
     * want to reinitialize the indicies here.
     */
    pRPA->vtrBufSWWrtIdx      = pRPA->vtrBufSWRdIdx     = 0;
    pRPA->vbatBufSWWrtIdx     = pRPA->vbatBufSWRdIdx    = 0;
    pRPA->imtBufSWWrtIdx      = pRPA->imtBufSWRdIdx     = 0;

    /* By delaying the Vtr, Vbat, Imt samples by various amount,
     * we can achieve better phase alignment. These numbers
     * were determined experimentally
     */
    pRPA->vtrDelay = VTR_DELAY;
    pRPA->vbatDelay = VBAT_DELAY;
    pRPA->imtDelay = IMT_DELAY;

    #ifdef DBG_BUFF_DATA
    vtrDbgIdx = vbatDbgIdx = imtDbgIdx = 0;
    #endif

    VpMemSet(&pRPA->vtrBuf[0], 0, sizeof(pRPA->vtrBuf));
    VpMemSet(&pRPA->vbatBuf[0], 0, sizeof(pRPA->vbatBuf));
    VpMemSet(&pRPA->imtBuf[0], 0, sizeof(pRPA->imtBuf));

    /* Set up a timer to come back and read the SADC data */
    Vp886AddTimerMs(NULL, pLineCtx, VP886_TIMERID_THERMAL_RINGING,
        VP_886_THERMAL_RINGING_TIMER_DURATION, pLineObj->channelId, VP886_THERMAL_RINGING_CADENCE);

    /* We want to throw out the first buffers worth of data from the SADC/VADC */
    pRPA->firstBufferThrownOut = FALSE;
    pRPA->crunchTheNumbers = FALSE;

    /* Set the flag in the line object to indicate that the thermal ringing algorithms
     * are running
     */
    pLineObj->thermalRinging = TRUE;

    /* Restore the previously saved state of the feedback loop
     * filter. Most of the time this will be the same as the present
     * state of the feedback loop filter. The only time it will
     * differ is if the last ring burst was terminated due to ring
     * trip. */
    VpMemCpy(&pRPACD->filter.data[0], &pRPACD->saved.filterData[0],
        sizeof(pRPACD->filter.data));

    /* Also restore the saved ring gain output. This is essential
     * because the restored loop filter data does not actually
     * affect the ring generator output until after the first full
     * cycle of the ringing burst! */
    pRPACD->ringGain = pRPACD->saved.ringGain;

    /* Change ring power measurement state to EVEN. */
    pRPACD->halfCycIdx = 0;

    /* Backup the current loop supervision register */
    VpMemCpy(pRPA->loopSup, pLineObj->registers.loopSup, VP886_R_LOOPSUP_LEN);

    switch (pDevObj->options.adaptiveRinging.mode) {
        case VP_ADAPT_RING_POWER_SLIC:
            /* Keep ILR and RTTH as is */
            break;

        case VP_ADAPT_RING_SHARED_TRACKER:
            /* - Program a high ILR to prevent tripping (48mA)
               - Max out RTTH (31.75mA) */
            if (pLineObj->lowIlr) {
                pRPA->loopSup[2] |= VP886_R_LOOPSUP_RTRIP_THRESH;
                pRPA->loopSup[3] &= ~VP886_R_LOOPSUP_RING_CUR_LIM;
                pRPA->loopSup[3] |= 0x24;
                VpSlacRegWrite(NULL, pLineCtx, VP886_R_LOOPSUP_WRT, VP886_R_LOOPSUP_LEN, pRPA->loopSup);
            }
            break;

        default:
            VP_ERROR(VpLineCtxType, pLineCtx, ("Vp886ThermalRingingPrepare(): Invalid mode %d",
                pDevObj->options.adaptiveRinging.mode));
            break;
    }

    /* Set a timer to keep ILR "high long enough" when entering ringing */
    Vp886AddTimerMs(NULL, pLineCtx, VP886_TIMERID_THERMAL_RINGING,
        VP_886_THERMAL_RINGING_DEBOUNCE_DURATION, pLineObj->channelId, VP886_THERMAL_RINGING_DEBOUNCE);

    /* In the meantime debounce */
    pRPA->debouncing = TRUE;

    VP_API_FUNC_INT(VpLineCtxType, pLineCtx, ("Vp886ThermalRingingPrepare-"));
    return VP_STATUS_SUCCESS;

} /* Vp886ThermalRingingPrepare */

/**
 * Vp886ThermalRingingADCCfg()
 *
 * Configures the Supervision ADC to monitor the Tip-Ring (Ringing) voltage,
 * and the battery/switcher voltage. It configures the VADC to measure the
 * metallic current.
 *
 * It is called from Vp886ThermalRingingPrepare
 *
 * Inputs:
 *  VpLineCtxType *pLineCtx         pointer to this line's line context
 *
 */
/* Note: This function is always in flux and will settle down soon */

#define VTR_SLOT    1
#define VBAT_SLOT   2

VpStatusType
Vp886ThermalRingingADCCfg(
    VpLineCtxType *pLineCtx)
{
    VpDevCtxType*                pDevCtx = pLineCtx->pDevCtx;
    Vp886DeviceObjectType*       pDevObj = pDevCtx->pDevObj;
    Vp886LineObjectType*        pLineObj = pLineCtx->pLineObj;
    int8                            chan = pLineObj->channelId;

    uint8 sadcCtrl[10];
    uint8 vadcCtrl[6];
    uint16 batterySelection;

    VP_API_FUNC_INT(VpLineCtxType, pLineCtx, ("Vp886ThermalRingingSADCCfg+"));

    if (VP886_IS_TRACKER(pDevObj)) {
        batterySelection = (chan == 0) ? VP886_R_SADC_SEL_SWY : VP886_R_SADC_SEL_SWZ;
    } else {    /* ABS */
        batterySelection = VP886_R_SADC_SEL_SWZ;
    }

    if (pLineCtx == NULL) {
        /* If this channel doesn't exist, leave */
        VP_API_FUNC_INT(VpLineCtxType, pLineCtx, ("Vp886ThermalRingingSADCCfg-"));
        return VP_STATUS_INVALID_ARG;
    }

    /* Supervision ADC Setup */
    /* No interupt. Raw data. Group mode. 500Hz. Don't enable yet */
    sadcCtrl[0] = VP886_R_SADC_GROUP_MODE | VP_886_THERMAL_RINGING_SADC_DRATE | VP886_R_SADC_ENABLE;
    /* Default all SADC slots to none */
    sadcCtrl[1] = sadcCtrl[2] = sadcCtrl[3] =
    sadcCtrl[4] = sadcCtrl[5] = VP886_R_SADC_SEL_ADC_OFFSET;
    /* Configure the specified slot for Tip-Ring Voltage */
    sadcCtrl[VTR_SLOT] = VP886_R_SADC_SEL_TIP_RING_DC_V;
    /* Configure the specified slot for Switcher/Battery SWY or SWZ */
    sadcCtrl[VBAT_SLOT] = batterySelection;
    /* Continuous Mode */
    sadcCtrl[6] = sadcCtrl[7] = 0;
    /* Must skip at least 2 samples for a HW bug */
    sadcCtrl[8] = 0; sadcCtrl[9] = 2;


    /* Voice ADC Setup */
    /* 500 Hz */
    vadcCtrl[0] = VP_886_THERMAL_RINGING_VADC_DRATE;
    /* ADC Source --> Tip - Ring metallic current */
    vadcCtrl[1] = VP886_R_VADC_SEL_METALLIC_CUR;
    /* Continuous Mode */
    vadcCtrl[2] = vadcCtrl[3] = 0;
    /* Don't skip any samples */
    vadcCtrl[4] = 0; vadcCtrl[5] = 2;


    VpSlacRegWrite(NULL, pLineCtx, VP886_R_SADC_WRT, VP886_R_SADC_LEN, sadcCtrl);
    VpSlacRegWrite(NULL, pLineCtx, VP886_R_VADC_WRT, VP886_R_VADC_LEN, vadcCtrl);

    VP_API_FUNC_INT(VpLineCtxType, pLineCtx, ("Vp886ThermalRingingSADCCfg-"));

    return VP_STATUS_SUCCESS;


} /* Vp886ThermalRingingADCCfg */


/**
 * Vp886ThermalRingingGetSADCData()
 *
 * Reads the data from the SADC and VADC buffers. Applies calibration and stores
 * the data software buffers for later processing.
 *
 * It is called from the thermal ringing Vp886ThermalRingingHandler().
 *
 * Inputs:
 *  VpLineCtxType *pLineCtx         pointer to this line's line context
 *
 * Outputs:
 *  none
 */
/* Note: This function is always in flux and will settle down soon */
#define VP886_APPLY_SADC_CAL(X) (int16)VpRoundedDivide(((int32)X+(int32)(sadcOffset))*(int32)(sadcGain),1000L)
#define VP886_APPLY_VADC_CAL(X) (int16)VpRoundedDivide(((int32)X+(int32)(vadcOffset))*(int32)(vadcGain),1000L)

#define VTR_B_RD    (VP886_R_B1_RD+2*(VTR_SLOT-1))
#define VBAT_B_RD   (VP886_R_B1_RD+2*(VBAT_SLOT-1))

#define VTR_B_LEN   VP886_R_B1_LEN
#define VBAT_B_LEN  VP886_R_B1_LEN

VpStatusType
Vp886ThermalRingingGetSADCData(
    VpLineCtxType *pLineCtx)
{
    VpDevCtxType*                pDevCtx = pLineCtx->pDevCtx;
    Vp886DeviceObjectType*       pDevObj = pDevCtx->pDevObj;
    Vp886LineObjectType*        pLineObj = pLineCtx->pLineObj;
    RingPowerAdaptModuleType*       pRPA = &pLineObj->ringPowerAdapt;
    int8                            chan = pLineObj->channelId;
    int16                       sadcGain = pDevObj->calData[chan].cmn.sadc.gain;
    int16                     sadcOffset = pDevObj->calData[chan].cmn.sadc.offset;
    int16                       vadcGain = pDevObj->calData[chan].cmn.vadcActive.gain;
    int16                     vadcOffset = pDevObj->calData[chan].cmn.vadcActive.offset;

    uint8 vtrSADCBuf[VP886_R_B1_LEN], vbatSADCBuf[VP886_R_B2_LEN], imtVADCBuf[VP886_R_VBUFFER_LEN];
    int16 vtrPtr, vbatPtr, imtPtr;
    uint8 sampleIdx, samplesAvail;
    uint8* vtrHWBufPtr;
    uint8* vbatHWBufPtr;
    uint8* imtHWBufPtr;
    int16 vtrLessSampAvail, vbatLessSampAvail, imtLessSampAvail;

    VP_API_FUNC_INT(VpLineCtxType, pLineCtx, ("Vp886ThermalRingingGetSADCData+"));

    if (pLineCtx == NULL) {
        /* If this channel doesn't exist, leave */
        VP_API_FUNC_INT(VpLineCtxType, pLineCtx, ("Vp886ThermalRingingGetSADCData-"));
        return VP_STATUS_INVALID_ARG;
    }

    /* Read the appropriate SADC data buffers and the VADC data buffer */
    VpSlacRegRead(NULL, pLineCtx, VP886_R_VBUFFER_RD, VP886_R_VBUFFER_LEN, imtVADCBuf);
    VpSlacRegRead(NULL, pLineCtx, VTR_B_RD, VTR_B_LEN, vtrSADCBuf);
    VpSlacRegRead(NULL, pLineCtx, VBAT_B_RD, VBAT_B_LEN, vbatSADCBuf);

    /* If this is the first buffer of data, toss it */
    if (pRPA->firstBufferThrownOut == FALSE) {
        pRPA->crunchTheNumbers = FALSE;
        pRPA->firstBufferThrownOut = TRUE;
        VP_API_FUNC_INT(VpLineCtxType, pLineCtx, ("Vp886ThermalRingingGetSADCData-"));
        return VP_STATUS_SUCCESS;

    } else {
        pRPA->crunchTheNumbers = TRUE;
    }

    /* Extract the last write pointer for each buffer. This is effectively a count
     * of the number of 16-bit samples that are ready */
    vtrPtr = vtrSADCBuf[0] & VP886_R_BX_PTR;
    vtrHWBufPtr = &vtrSADCBuf[1];
    vbatPtr = vbatSADCBuf[0] & VP886_R_BX_PTR;
    vbatHWBufPtr = &vbatSADCBuf[1];
    imtPtr = imtVADCBuf[0] & VP886_R_VBUFFER_POINTER;
    imtHWBufPtr = &imtVADCBuf[1];

#ifdef INCLUDE_BUFFER_OVERFLOW_WARNINGS
    if (vtrSADCBuf[0] & VP886_R_BX_OVFL) {
        VP_WARNING(VpLineCtxType, pLineCtx, ("Vp886ThermalRingingGetSADCData(): Vtr Buffer Overflow"));
    }
    if (vbatSADCBuf[0] & VP886_R_BX_OVFL) {
        VP_WARNING(VpLineCtxType, pLineCtx, ("Vp886ThermalRingingGetSADCData(): Vbat Buffer Overflow"));
    }
    if (imtVADCBuf[0] & VP886_R_VBUFFER_OVERFLOW) {
        VP_WARNING(VpLineCtxType, pLineCtx, ("Vp886ThermalRingingGetSADCData(): Imt Buffer Overflow"));
    }
#endif

    vtrLessSampAvail = pRPA->vtrDelay;
    vbatLessSampAvail = pRPA->vbatDelay;
    imtLessSampAvail = pRPA->imtDelay;

    /**************************************************************
     *
     * How the buffer indicies are used, incremented, wrapped, etc.
     *
     * SADC/VADC SW Read Index:
     *  Used elsewhere when the data is extracted to be used
     *  for the power calculations. Gets saved each iteration and is
     *  wrapped if necessary when the end of the buffer is reached.
     *
     * SADC/VADC SW Write Index:
     *  Used here to write the data extracted from the HW buffer into the
     *  SW buffer. It is saved and wrapped as necessary.
     *
     ************************************************************/

    /* The data *bytes* from the HW are used to construct 16-bit words */
    /* For the Vtr data */
    for (sampleIdx = 0; sampleIdx  < vtrPtr; sampleIdx++) {
        pRPA->vtrBuf[pRPA->vtrBufSWWrtIdx++] = VP886_APPLY_SADC_CAL((int16)((vtrHWBufPtr[sampleIdx*2]<<8)|(vtrHWBufPtr[sampleIdx*2+1]&0xFF)));
        if (pRPA->vtrDelay != 0) {
            pRPA->vtrDelay--;
            pRPA->vtrBufSWWrtIdx--;
        }
        pRPA->vtrBufSWWrtIdx %= VP_886_THERMAL_RINGING_BUF_SIZE;
    }

    /* For the Vbat data */
    for (sampleIdx = 0; sampleIdx  < vbatPtr; sampleIdx++) {
        pRPA->vbatBuf[pRPA->vbatBufSWWrtIdx++] = VP886_APPLY_SADC_CAL((int16)((vbatHWBufPtr[sampleIdx*2]<<8)|(vbatHWBufPtr[sampleIdx*2+1]&0xFF)));
        if (pRPA->vbatDelay != 0) {
            pRPA->vbatDelay--;
            pRPA->vbatBufSWWrtIdx--;
        }
        pRPA->vbatBufSWWrtIdx %= VP_886_THERMAL_RINGING_BUF_SIZE;
    }

    /* For the Imt data */
    for (sampleIdx = 0; sampleIdx  < imtPtr; sampleIdx++) {
        /* In low ILR mode, imt is halved */
        if (pLineObj->lowIlr) {
            pRPA->imtBuf[pRPA->imtBufSWWrtIdx++] = -1*VP886_APPLY_VADC_CAL((int16)((imtHWBufPtr[sampleIdx*2]<<8)|(imtHWBufPtr[sampleIdx*2+1]&0xFF)))/2;
        } else {
            pRPA->imtBuf[pRPA->imtBufSWWrtIdx++] = -1*VP886_APPLY_VADC_CAL((int16)((imtHWBufPtr[sampleIdx*2]<<8)|(imtHWBufPtr[sampleIdx*2+1]&0xFF)));
        }
        if (pRPA->imtDelay != 0) {
            pRPA->imtDelay--;
            pRPA->imtBufSWWrtIdx--;
        }
        pRPA->imtBufSWWrtIdx %= VP_886_THERMAL_RINGING_BUF_SIZE;
    }

    /* Update the number of samples available */
    samplesAvail = Vp886min16(Vp886min16(vtrPtr-vtrLessSampAvail,
        vbatPtr-vbatLessSampAvail), imtPtr-imtLessSampAvail);
    pRPA->samplesAvail += samplesAvail;

    VP_API_FUNC_INT(VpLineCtxType, pLineCtx, ("Vp886ThermalRingingGetSADCData-"));

    return VP_STATUS_SUCCESS;

} /* Vp886ThermalRingingGetSADCData */


/**
 * Vp886ThermalRingingHandler()
 *
 *  Calls the routine which reads the data from the SADC data buffer.
 *  After applying calibration, it passes the data over to the routines
 *  that perform the power calculations, run the control loop,
 *  calculate the new ring gain, etc.
 *
 *  This function is called either from a timer handler or from the
 *  API tick.
 *
 *  It then sets a timer to start all over again.
 *  How do we corrdinate this with ring enter/exit and ringtrip>
 *  How/when to the aplitude corrections get applied?
 *
 *
 * Inputs:
 *  pLineCtx: pointer to this line's line context
 *  overrun:  not sure how relevant this is here
 *
 * Outputs:
 *  none
 */
void
Vp886ThermalRingingHandler(
    VpLineCtxType *pLineCtx)
{
    Vp886LineObjectType*        pLineObj = pLineCtx->pLineObj;
    RingPowerAdaptModuleType*       pRPA = &pLineObj->ringPowerAdapt;

    VP_API_FUNC_INT(VpLineCtxType, pLineCtx, ("Vp886ThermalRingingHandler+"));

    /* Maybe add a check to verify that thermal ringing is actually running ? */
    /* Just in case there was some sort of race condition with timers, flags,
     * etc. and this got through */
    if (pLineObj->thermalRinging == FALSE) {
        /* What should be done here ? */
        VP_API_FUNC_INT(VpLineCtxType, pLineCtx, ("Vp886ThermalRingingHandler-"));
    }

    /* Set a timer to do it again */
    Vp886AddTimerMs(NULL, pLineCtx, VP886_TIMERID_THERMAL_RINGING,
        VP_886_THERMAL_RINGING_TIMER_DURATION, pLineObj->channelId, VP886_THERMAL_RINGING_CADENCE);

    /* Get the data from the SADC buffer */
    Vp886ThermalRingingGetSADCData(pLineCtx);

    /* Crunch the numbers */
    if (pRPA->crunchTheNumbers) {
        Vp886ThermalRingingProcessData(pLineCtx);
    }

    VP_API_FUNC_INT(VpLineCtxType, pLineCtx, ("Vp886ThermalRingingHandler-"));
    return;

} /* Vp886ThermalRingingHandler */


/**
 * Vp886ThermalRingingStop()
 * Thermal Ringing Stop.
 *
 * This function stops the thermal ringing algorithm from running. It kills
 * the timer if one is running. It turns off the SADC. It clears the flag
 * indicating that thermal ringing is running.
 *
 * This is called from Vp886SetLineStateFxsInt(). This may also be called when the
 * "break" part of a ring cadence is started (if it is not done with the above mentioned
 * function). This may also be called if a line state starting a line test.
 *
 *
 * There will probably be other scenarios and cases where calling this function
 * may or may not be allowed. TBD.
 *
 * Inputs:
 * VpLineCtxType *pLineCtx pointer to this line's line context
 *
 * Outputs:
 *  status
 */
/* Should we just zero out the whole data struct? Are there power and filter
 * data that need to be carried over from the previous run? If so, we probably
 * shouldn't zero out the whole thing
 */
VpStatusType
Vp886ThermalRingingStop(
    VpLineCtxType *pLineCtx)
{
    Vp886LineObjectType*       pLineObj = pLineCtx->pLineObj;
    RingPowerAdaptChannelData*   pRPACD = &pLineObj->ringPowerAdapt.rpaChanData;

    uint8 sadcCtrl[10];

    VP_RPM(VpLineCtxType, pLineCtx, ("Adaptive ringing stop"));

    VP_API_FUNC_INT(VpLineCtxType, pLineCtx, ("Vp886ThermalRingingStop+"));

    if (pLineObj->thermalRinging == FALSE) {
        /* If this function gets called when thermal ringing is not running, leave */
        VP_API_FUNC_INT(VpLineCtxType, pLineCtx, ("Vp886ThermalRingingStop-"));
        return VP_STATUS_INVALID_ARG;
    }

    if (pLineCtx == NULL) {
        /* If this channel doesn't exist, leave */
        VP_API_FUNC_INT(VpLineCtxType, pLineCtx, ("Vp886ThermalRingingStop-"));
        return VP_STATUS_INVALID_ARG;
    }

    /* Cancel any thermal ringing timers */
    Vp886CancelTimer(NULL, pLineCtx, VP886_TIMERID_THERMAL_RINGING, 0, FALSE);

    /* No interupt. Raw data. Group mode. 500kHz. Make sure SADC in not enabled */
    sadcCtrl[0] = VP886_R_SADC_GROUP_MODE | VP_886_THERMAL_RINGING_SADC_DRATE;
    /* Turn off all SADC measurements */
    sadcCtrl[1] = VP886_R_SADC_SEL_ADC_OFFSET;
    sadcCtrl[2] = VP886_R_SADC_SEL_ADC_OFFSET;
    sadcCtrl[3] = VP886_R_SADC_SEL_ADC_OFFSET;
    sadcCtrl[4] = VP886_R_SADC_SEL_ADC_OFFSET;
    sadcCtrl[5] = VP886_R_SADC_SEL_ADC_OFFSET;
    /* Continuous Mode */
    sadcCtrl[6] = sadcCtrl[7] = 0;
    /* Don't skip any samples */
    sadcCtrl[8] = sadcCtrl[9] = 0;

    /* Write to part */
    VpSlacRegWrite(NULL, pLineCtx, VP886_R_SADC_WRT, VP886_R_SADC_LEN, sadcCtrl);

    /* Indicate that we are NOT in thermal ringing anymore */
    pLineObj->thermalRinging = FALSE;

    /* When the drive state changes to non-ringing, reset the ring
     * power measurement block, including the zero-cross detector,
     * integrator, and integrated half-cycle data storage. Note that
     * we DO NOT reset the filter state here. */
    /* Again, do we have to perform zero-cross checking here ? */
    VpMemSet(&pRPACD->integrator, 0, sizeof(pRPACD->integrator));
    VpMemSet(&pRPACD->halfCyc[0], 0, sizeof(pRPACD->halfCyc));

    /* If the ringing burst was NOT terminated because of ring trip
     * detection, save the state of the feedback loop filter and the
     * final ring signal gain as the starting point for the next
     * ringing burst.
     *
     * NOTE: It is possible (but unlikely) that the sync drive state
     * change to non-ringing occurs before the ring power adaptation
     * job gets a chance to analyze the last half-cycle in the
     * ringing burst. In this case the saved filter state will
     * contain slightly stale data, i.e. it will not include the
     * very last half-cycle. This error is insignificant because the
     * algorithm has likely switched to slow adaptation mode by that
     * time, and in that mode the changes to the filter state at
     * each half-cycle are probably minor.*/
    if ((pLineObj->lineState.condition & VP_CSLAC_HOOK) != VP_CSLAC_HOOK) {
        VpMemCpy(&pRPACD->saved.filterData[0], &pRPACD->filter.data[0],
            sizeof(pRPACD->saved.filterData));
        pRPACD->saved.ringGain = pRPACD->ringGain;
    }

    /* Change ring power measurement state to IDLE. */
    pRPACD->halfCycIdx = -1;

    VP_API_FUNC_INT(VpLineCtxType, pLineCtx, ("Vp886ThermalRingingStop-"));
    return VP_STATUS_SUCCESS;

} /* Vp886ThermalRingingStop */

/**
 * Vp886ThermalRingingProcessData()
 *
 * Processes the voltage and current data retrieved from the device.
 * It does so by just passing the data over to the power calculation
 * routines.
 *
 * Inputs:
 *  pLineCtx: pointer to this channels Line Context
 *
 * Outputs:
 *  none
 */
void
Vp886ThermalRingingProcessData(
    VpLineCtxType *pLineCtx)

{
    VP_API_FUNC_INT(VpLineCtxType, pLineCtx, ("Vp886ThermalRingingProcessData+"));

    /* Send over to the power calculation routines */
    MeasureRingPower(pLineCtx);

    VP_API_FUNC_INT(VpLineCtxType, pLineCtx, ("Vp886ThermalRingingProcessData-"));

}   /* Vp886ThermalRingingProcessData */

/******************************************************************************
 *
 * The code and comments below are lifted from the NGSLAC
 */

/**
 * This function initializes a Direct Form I hybrid filter object. It simply
 * records the pointers and filter order provided by the caller in the filter
 * object allocated by the caller.
 *
 * \param[out]  pFilter     contains the address of the client's filter object
 *                          initialized by this function.
 * \param[in]   pDelayBuf   contains the address of the client's filter delay
 *                          buffer. This buffer is typically an array of
 *                          ::DF1HybridFilterDelayPair type with array size
 *                          equal to the desired filter order.
 * \param[in]   pCoeffs     contains the address of the client's filter
 *                          coefficients. The filter coefficients should be an
 *                          array of ::DFHybrid1FilterCoeffPair type with array
 *                          size equal to the filter order plus 1. \a pCoeffs[0].b
 *                          is the coefficient applied to the input sample when
 *                          the filter is clocked. \a pCoeff[0].a is integer
 *                          (not fractional) base-2 log of the output gain.
 *                          See the definition of ::DF1HybridFilterCoeffPair for
 *                          details.
 * \param[in]   order       indicates te desired filter order and must not equal
 *                          zero. Filter order is equal to the largest exponent
 *                          in the numerator and denominator of the filter's
 *                          transfer function, or the max of N and M where N is
 *                          the number of delay elements in the output path and
 *                          M is the number of delay elements in the input path.
 *
 * \note    This function also calls the DF1HybridFilterReset() function to
 *          reset the filter's delay elements to zero. If zero is not an
 *          acceptable value for the filter's initial state, then the client
 *          must initialize the filter afterwards either by manually setting
 *          the filter state vars or by calling DF1HybridFilterReset().
 *
 * \ingroup DF1_FILTER
 */
static VpStatusType
DF1HybridFilterInit(
    VpLineCtxType *pLineCtx,
    DF1HybridFilter* pFilter,
    DF1HybridFilterDelayPair* pDelayBuf,
    const DF1HybridFilterCoeffPair* pCoeffs,
    uint16 order)
{
    VpStatusType             status = VP_STATUS_SUCCESS;

    VP_API_FUNC_INT(VpLineCtxType, pLineCtx, ("DF1HybridFilterInit+"));

    /* The following NULL checks replace the asserts that were
     * in the NGSLAC code. These are fatal errors and the
     * code must not be allowed to continue.
     */
    if (pFilter == NULL) status = VP_STATUS_FAILURE;
    if (order == 0) status = VP_STATUS_FAILURE;
    if (pDelayBuf == NULL) status = VP_STATUS_FAILURE;
    if (pCoeffs == NULL) status = VP_STATUS_FAILURE;
    if (status == VP_STATUS_SUCCESS) {
        pFilter->pData = pDelayBuf;
        pFilter->pCoef = pCoeffs;
        pFilter->order = order;

        DF1HybridFilterReset(pLineCtx, pFilter, 0);
    }

    VP_API_FUNC_INT(VpLineCtxType, pLineCtx, ("DF1HybridFilterInit-"));

    return status;

} /* DF1HybridFilterInit() */


/**
 * This function simply resets a DF1 hybrid filter's input/output memory to the
 * specified value. That is, every element in the filter's delay buffer is set
 * equal to the specified value. This is useful for artificially "charging" a
 * filter to a particular state.
 *
 * \param[in]   pFilter     contains the address of the client's filter object
 *                          reset by this function.
 * \param[in]   value       contains the value written to the filter's memory.
 *
 * \note    The hybrid filter retains its output memory in 32-bit variables.
 *          Therefore, the 16-bit reset value is expanded to 32-bits when it is
 *          written to the output delay buffers.
 *
 * \ingroup DF1_FILTER
 */
static void
DF1HybridFilterReset(
    VpLineCtxType *pLineCtx,
    const DF1HybridFilter* pFilter,
    frac16 value)
{
    uint16 u;
    frac32 value32 = Vp886L_deposit_h(value);

    VP_API_FUNC_INT(VpLineCtxType, pLineCtx, ("DF1HybridFilterReset+"));

    if (pFilter != NULL) {
        for (u = 0; u < pFilter->order; u++) {
            pFilter->pData[u].x = value;
            pFilter->pData[u].y = value32;
        }
    }

    VP_API_FUNC_INT(VpLineCtxType, pLineCtx, ("DF1HybridFilterReset-"));
} /* DF1HybridFilterReset */

/**
 * This function "clocks" a DF1 hybrid mixed-precision filter object with a new
 * 16-bit sample input and returns a new 32-bit output. The "hybrid" filter
 * impelementation essentially uses single-precision 16-bit math for the input
 * samples and filter coefficients and uses double-precision 32-bit math for the
 * output samples and filter coefficients. This solution reduces the impact of
 * limit-cycle issues by maintaining the output history in 32-bit variables, yet
 * it is nearly as fast as the single-precision implementation.
 *
 * \param[in]   pFilter     contains the address of the client's filter object
 *                          clocked by this function.
 *
 * \param[in]   sample      contains the signed 16-bit sample clocked into the
 *                          filter.
 *
 * \return  This function returns the signed 32-bit filter output. Note that the
 *          caller is responsible for rounding the 32-bit result to a 16-bit
 *          result if desired.
 *
 * \note    The DF1 filter structure is naturally immune to fixed-point overflow
 *          conditions. Thus overflow conditions are not expected or handled.
 *
 * \warning This function is not reentrant if it operates on global filter data.
 *          The application should not change the filter's coefficients while
 *          this function is executing.
 *
 * \ingroup DF1_FILTER
 */
static frac32
DF1HybridFilterClock(
    VpLineCtxType *pLineCtx,
    const DF1HybridFilter* pFilter,
    frac16 sample)
{
    uint16 order;
    uint16 dataIdx;
    uint16 coefIdx;
    DF1HybridFilterDelayPair* pData;
    const DF1HybridFilterCoeffPair* pCoef;
    frac32 acc;

    VP_API_FUNC_INT(VpLineCtxType, pLineCtx, ("DF1HybridFilterClock+"));

    /* Copy these locally for convenience. */
    order = pFilter->order;
    pData = pFilter->pData;
    pCoef = pFilter->pCoef;

    /* Run the filter. */
    acc = Vp886L_mult(sample, pCoef[0].b);

    for (coefIdx = 1, dataIdx = 0; dataIdx < order; coefIdx++, dataIdx++) {
        /* 16-bit input sample * 16-bit coeff -> 32-bit accumulator */
        acc = Vp886L_mac(acc, pData[dataIdx].x, pCoef[coefIdx].b);
        /* 32-bit output sample * 32-bit coeff -> 32-bit accumulator
         * Note that D_mult_msp() is an optimized double-precision multiply
         * that discards the least-significant 32-bits of the 64-bit result
         * of a 32-bit * 32-bit multiply operation. */
        acc = Vp886L_sub(acc, Vp886D_mult_msp(pData[dataIdx].y, pCoef[coefIdx].a));
    }

    /* Apply 2^(a0) gain to output (with saturation). */
    acc = Vp886L_shl(acc, (int16)pCoef[0].a);

    /* Push the samples through the delay buffer.
     *
     * Note that we're literally moving the in/out samples to the next locations
     * in the delay buffer. I've attempted to optimize this using circular
     * buffers and found that to be less efficient for small filter orders and
     * more difficult to comprehend. Further, we can't take advantage of modulo
     * addressing mode compiler optimizations here because the buffer length
     * (filter order in this case) is not known at compile-time. Dumb data copy
     * is good enough for now. */

    /* Note that dataIdx == order at this point because of loop exit above. */
    for (dataIdx--; dataIdx != 0; dataIdx--) {
        pData[dataIdx].x = pData[dataIdx - 1].x;
        pData[dataIdx].y = pData[dataIdx - 1].y;
    }


    /* Store present in and out in the delay buffer. */
    pData[0].x = sample;
    pData[0].y = acc;

    VP_API_FUNC_INT(VpLineCtxType, pLineCtx, ("DF1HybridFilterClock-"));

    return acc;
} /* DF1HybridFilterClock */

/**
 * Vp886ThermalRingingScaleData()
 *
 *  Adjusts the fullscale of a value from one to another.
 *
 * TODO: Make this more robust for overflow handling.
 *
 *
 *
 * Inputs:
 *  inputVal:           value to be rescaled
 *  origFullScale:      value's current full-scale
 *  newFullscale:       value's desired full-scale
 *
 * Outputs:
 *  rescaled value
 */
int16
Vp886ThermalRingingScaleData(
    int16 inputVal,
    int16 origFullScale,
    int16 newFullscale)
{
    int32 tempLocal;
    int16 result;

    /* Multiply by orig full-scale first to keep as many bits of precision as possible */
    tempLocal = inputVal * origFullScale;
    /* divide by desired full-scale */
    tempLocal /= newFullscale;

    /* Make sure the result stays within 16-bits */
    result = Vp886saturate(tempLocal);

    return result;

} /* Vp886ThermalRingingScaleData */

/**
 * Vp886ThermalRingingSetTickCount()
 *
 * Determines how many ticks are in a half ringing cycle
 * and sets the appropriate varibles
 *
 * The Sinusoidal case: SINTRAP == 0
 *
 * From the Command Set Doc --> FRQA is an unsigned number with a 0.3662 Hz step size
 *
 * Fring                    = FRQA * 0.3662 Hz
 * Period in seconds        = 1 / Fring = 1 / (FRQA * 0.3662)
 *
 * Since we are performing the power integration calculations
 * at a rate of 500Hz, we want the period in 2ms ticks.
 *
 * Period in 2ms ticks      = 500 / Fring = 500 / (FRQA * 0.3662)
 *                          = 1365.3741125068268705625341343528 / FRQA
 *                          = (21845.985800109229929000546149645 / (FRQA)) / 16
 *
 * For fixed point purposes, round 21845.985800109229929000546149645 -> 21846
 *
 * So,
 *
 * Period in 2ms ticks      = (21846 / FRQA) / 16
 *                          = (21846 / FRQA) >> 4
 *
 * But, we should round, so
 *
 * Period in 2ms ticks      = ((21846 / FRQA) + 0x0008) >> 4
 *
 * But, since we are executing the control loop every half-cycle
 * we whould downshift one more time.
 *
 * So,
 *
 * Half-cycle in 2ms ticks  = ((21846 / FRQA) + 0x0010) >> 5
 *
 * If the half-cycle isn't an integer number of 2ms ticks, we would effectively
 * be running the control loop at a slightly higher frequency than the correct
 * frequency. To attempt to combat this problem, we will have two different
 * half cycle tick counts. The half cycle tick count will alternate between
 * these two values so that on average, the half cycle loop is executed at the
 * right frequency.
 *
 * To do this, we first perform the above operation, but only downshift by 4.
 * If bit 0 is set, the half cycle tick count will alternate between
 * (((21846 / FRQA) + 0x0008) >> 5) and (((21846 / FRQA) + 0x0008) >> 5) + 1
 *
 * If bit 0 is not set, the half cycle count will be ((21846 / FRQA) + 0x0008) >> 5
 * everytime.
 *
 * This isn't perfect, but it is better.
 *
 * Example, for a 20Hz ringing signal, there are 25 2ms ticks, leaving
 * 12.5 2 ms ticks per half cycle. We could just let it run every 12 cycles
 * which would give an execution frequency of 20.83 Hz. Or we could
 * employ some mechanism by which the control loop runs at an alternating
 * 12 then 13 samples.
 *
 * The Trapezoidal case: SINTRAP == 1
 *
 * From the Command Set Doc --> FRQA is a signed number which sets the rise
 * time according to the following formula:
 *
 * FRQA = 8000/Fring
 *
 * Fring                    = 8000/FRQA
 *
 * Period in seconds        = 1/Fring
 *                          = FRQA/8000
 *
 * Since we are performing the power integration calculations
 * at a rate of 500Hz, we want the period in 2ms ticks.
 *
 * Period in 2ms ticks      = 500 / Fring = 500 / (8000/FRQA)
 *                          = FRQA * 500/8000
 *                          = FRQA / 16
 *                          = FRQA >> 4
 *
 * But, we should round, so
 *
 * Period in 2ms ticks      = (FRQA + 0x0008) >> 4
 *
 * But, since we are executing the control loop every half-cycle
 * we whould downshift one more time.
 *
 * So,
 *
 * Half-cycle in 2ms ticks  = (FRQA + 0x0010) >> 5
 *
 * If the half-cycle isn't an integer number of 2ms ticks, we would effectively
 * be running the control loop at a slightly higher frequency than the correct
 * frequency. To attempt to combat this problem, we will have two different
 * half cycle tick counts. The half cycle tick count will alternate between
 * these two values so that on average, the half cycle loop is executed at the
 * right frequency.
 *
 * To do this, we first perform the above operation, but only downshift by 4.
 * If bit 0 is set, the half cycle tick count will alternate between
 * (FRQA + 0x0008) >> 5 and (FRQA + 0x0008) >> 5 + 1
 *
 * If bit 0 is not set, the half cycle count will be (FRQA + 0x0008) >> 5
 * everytime.
 *
 * This isn't perfect, but it is better.
 *
 *
 *
 */
void
Vp886ThermalRingingSetTickCount(
    VpLineCtxType *pLineCtx)
{
    Vp886LineObjectType*        pLineObj = pLineCtx->pLineObj;
    RingPowerAdaptModuleType*       pRPA = &pLineObj->ringPowerAdapt;

    uint8 ringGenParams[VP886_R_RINGGEN_LEN];
    int16 frqa;
    int16 tickTmp;

    VpSlacRegRead(NULL, pLineCtx, VP886_R_RINGGEN_RD, VP886_R_RINGGEN_LEN, ringGenParams);
    frqa = (int16)(((ringGenParams[3] << 8) & 0x7F00) | (ringGenParams[4] & 0x00FF));

    /* If this is sinusoidal ringing */
    if ((ringGenParams[0] & VP886_R_RINGGEN_WAVE) == VP886_R_RINGGEN_WAVE_SINE) {

        /* Perform the division and the round, but only downshift by 4 */
        tickTmp = ((21846 / frqa) + 0x0008) >> 4;

    } else {

        /* For trapezoidal round but only downshift by 4 */
        tickTmp = (frqa + 0x0008) >> 4;

    }

    pRPA->halfCycleTicks[0] = tickTmp >> 1;
    pRPA->halfCycleTicks[1] = tickTmp >> 1;

    /* If bit 0 is set, add an extra tick to the second tick count */
    if ((tickTmp & 0x0001) == 0x0001) {
        pRPA->halfCycleTicks[1] += 1;
    }

    pRPA->halfCycleTickCount = pRPA->halfCycleTicks[0];

} /* Vp886ThermalRingingSetTickCount */


/**
 * Vp886ThermalRingingSetTargetSlicPower()
 *
 *  Allows the Target SLIC power to be adjusted
 *  by some outside entity. The can be used
 *  if a different target power is desired while
 *  the algorithm is runnung. If one line is in
 *  ringing and then another line goes into ringing, etc.
 *
 *
 * Inputs:
 *  VpLineCtxType *pLineCtx
 *  int16 targetSlicPower
 *
 *
 * Outputs:
 *  none
 */
EXTERN void
Vp886ThermalRingingSetTargetSlicPower(
    VpLineCtxType *pLineCtx,
    int16 targetSlicPower)
{
    VpDevCtxType*                pDevCtx = pLineCtx->pDevCtx;
    Vp886DeviceObjectType*       pDevObj = pDevCtx->pDevObj;
    Vp886LineObjectType*        pLineObj = pLineCtx->pLineObj;
    RingPowerAdaptModuleType*       pRPA = &pLineObj->ringPowerAdapt;

    int16 rspt;

    VP_API_FUNC_INT(VpLineCtxType, pLineCtx, ("Vp886ThermalRingingAdjustTargetSlicPower-"));

    /* Convert the power from 40W to 10W full scale (39mW per steps) */
    rspt = (int32)targetSlicPower * 3225 / 100;

    switch (pDevObj->options.adaptiveRinging.mode) {
        case VP_ADAPT_RING_POWER_SLIC:
            /* No hysteresis in power SLIC mode */
            pRPA->rspt = pRPA->rsptLow = pRPA->rsptHigh = rspt;
            break;

        case VP_ADAPT_RING_SHARED_TRACKER:
            /* High hysteresis threshold */
            pRPA->rsptHigh = rspt;

            /* Low hysteresis threshold (50% of the specified threshold) */
            pRPA->rsptLow = rspt * 50L / 100;

            /* Preserve the power limit target if already set */
            if ((pRPA->rspt != pRPA->rsptLow) && (pRPA->rspt != pRPA->rsptHigh)) {
                pRPA->rspt = rspt;
            }
            break;

        default:
            pRPA->rspt = pRPA->rsptLow = pRPA->rsptHigh = rspt;
            VP_ERROR(VpLineCtxType, pLineCtx, ("Vp886ThermalRingingSetTargetSlicPower(): Invalid mode %d",
                pDevObj->options.adaptiveRinging.mode));
            break;
    }

    VP_RPM(VpLineCtxType, pLineCtx, ("Power limit allocated to the channel = %li mW", pRPA->rspt * 122L / 100));

    VP_API_FUNC_INT(VpLineCtxType, pLineCtx, ("Vp886ThermalRingingAdjustTargetSlicPower-"));

} /* Vp886ThermalRingingSetTargetSlicPower */

/**
 * Vp886ThermalRingingSetFuseResistance()
 *
 *  Sets the fuse resistance. The fuse resistance will be provided
 *  by some mechanism yet to be determined.
 *
 *
 * Inputs:
 *  VpLineCtxType *pLineCtx
 *  int16 targetSlicPower
 *
 *
 * Outputs:
 *  none
 */
static void
Vp886ThermalRingingSetFuseResistance(
    VpLineCtxType *pLineCtx,
    int16 rFuse)
{
    Vp886LineObjectType*        pLineObj = pLineCtx->pLineObj;
    RingPowerAdaptModuleType*       pRPA = &pLineObj->ringPowerAdapt;

    VP_API_FUNC_INT(VpLineCtxType, pLineCtx, ("Vp886ThermalRingingAdjustTargetSlicPower-"));

    pRPA->rFuse = rFuse;

    VP_API_FUNC_INT(VpLineCtxType, pLineCtx, ("Vp886ThermalRingingAdjustTargetSlicPower-"));

} /* Vp886ThermalRingingSetFuseResistance */

/*
 * This function updates the RTTH parameter by scaling it with the gain
 * provided by the ring generator's power adaptation algorithm. This gain
 * is applied if the mode of convergence is not fast.
 *
 * \param[in] channelNum    Channel number.
 * \param[in] ringGain      Adaptive gain to be applied to the RTTH parameter.
 * \param[in] fastMode      Boolean variable to indicate if the ringing power
 *                          adaptation algorithm is in the slow/fast mode of
 *                          convergence.
 *
 */
#define ringTripRtthFracScaleFactorDflt (frac16)0x1999
#define ringTripRtthOverallScaleFactorLoLim (frac16)0x0CCD
static uint8
RingTripUpdateRtth(VpLineCtxType *pLineCtx,
    frac16 ringGain,
    bool   fastMode)
{
    Vp886LineObjectType*          pLineObj = pLineCtx->pLineObj;
    RingPowerAdaptModuleType*         pRPA = &pLineObj->ringPowerAdapt;
    frac16 overallSF = 0;
    uint8 rtthOut;

    if (fastMode == FALSE) {
        /* The overall scaling factor is (1.SF * RG) where SF is the user
         * programmed scaling factor and RG is the ringing gain provided
         * by the power adaptation algorithm.
         * Overall scaling factor = RG * 1.SF
         *                        = RG + (RG * 0.SF)
         * \note The saturating 'add' operation will cap the upper limit
         * of the above result to 0.999969482421875 (0x7FFF).
         */
        overallSF = Vp886add(ringGain, Vp886mult_r(ringGain, ringTripRtthFracScaleFactorDflt));

        /* Cap the lower limit of the overall scaling factor to the lower
         * threshold. */
        if (overallSF < ringTripRtthOverallScaleFactorLoLim) {
            overallSF = ringTripRtthOverallScaleFactorLoLim;
        }

        /* Get the host programmed value of RTTH and scale it by the above
         * scaling factor. */
        rtthOut = (Vp886mult_r(overallSF, (frac16)(pRPA->rtth << 8))) >> 8;
    } else {
        /* The power adaptation algorithm is in the fast mode of convergence.
         * Use the user programmed RTTH without any scaling. */
        rtthOut = pRPA->rtth;
    }

    return (uint8)(rtthOut&0x7F);

} /* RingTripUpdateRtth */

#endif /* defined (VP_CC_886_SERIES) */

#endif /* VP886_INCLUDE_THERMAL_RINGING */
