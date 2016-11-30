
/** \file vp886_math.h
 * vp886_math.h
 *
 * Header file for various common functions, including many 64-bit math
 * functions.
 *
 * Copyright (c) 2011, Microsemi Corporation
 *
 * $Revision: 9999 $
 * $LastChangedDate: 2012-05-14 10:57:37 -0500 (Mon, 14 May 2012) $
 */

#ifndef VP886_MATH_H
#define VP886_MATH_H

#define Vp886L_mult(v1,v2)           ((v1 * v2) << 1)
#define Vp886L_mac(v1,v2,v3)         (v1 + ((v2 * v3) << 1))

#define MAX_32 (int32)0x7fffffffL
#define MIN_32 (int32)0x80000000L
#define MAX_16 (int16)0x7fff
#define MIN_16 (int16)0x8000

/* These fixed-point fractional types should be used in place of the usual
 * integer types where appropriate. The location of the decimal point is not
 * implied by the type name. It is up to the developer to document accordingly.
 */ 
typedef signed short    frac16;
typedef signed long     frac32;
typedef unsigned short  ufrac16;
typedef unsigned long   ufrac32;

typedef struct {
    int32 high32;
    uint32 low32;
} int64fxp;

/* ====================================
    Prototypes for Top-Level Functions
   ==================================== */
EXTERN int32 Vp886L_shl(int32 L_var1, int16 var2);
EXTERN int32 Vp886L_shr(int32 L_var1, int16 var2);
EXTERN int32 Vp886L_shr_r(int32 L_var1, int16 var2);
EXTERN int16 Vp886max16 (int16 var1, int16 var2);
EXTERN int16 Vp886min16 (int16 var1, int16 var2);
EXTERN int16 Vp886add (int16 var1, int16 var2);
EXTERN int16 Vp886sub (int16 var1, int16 var2);
EXTERN int16 Vp886mult_r (int16 var1, int16 var2);
EXTERN int16 Vp886saturate(int32 inVal);
EXTERN int16 Vp886shl (int16 var1, int16 var2);
EXTERN int16 Vp886shr (int16 var1, int16 var2);
EXTERN int16 Vp886extract_l (int32 L_var1);
EXTERN int16 Vp886extract_h (int32 L_var1);
EXTERN int32 Vp886L_add(int32 x, int32 y);
EXTERN int32 Vp886L_sub(int32 x, int32 y);
EXTERN int32 Vp886L_mult_ls(int32 var1, int16 var2);
EXTERN int32 Vp886L_shl_nosat (int32 L_var1, int16 var2);
EXTERN int32 Vp886L_negate (int32 L_var1);
EXTERN int16 Vp886norm_l (int32 myValue);
EXTERN int32 Vp886Int32Div(int32 numer, int32 denom);
EXTERN int16 Vp886abs_s (int16 var1);
EXTERN int16 Vp886round32to16 (int32 L_var1);
EXTERN int32 Vp886L_deposit_h (int16 var1);
EXTERN int32 Vp886D_mult_msp (int32 L_var1, int32 L_var2 );

#endif  /* VP886_MATH_H */
