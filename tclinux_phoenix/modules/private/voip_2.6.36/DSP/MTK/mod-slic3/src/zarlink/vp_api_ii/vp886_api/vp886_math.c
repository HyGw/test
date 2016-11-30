
/** \file vp886_math.c
 * vp886_math.c
 *
 * This file contains support functions used by other functions in the
 * API library.  The functions in this file are math related and are
 * shared between line tests and the VP886 Thermal Ringing routines
 *
 * The functions herein are used internally by the API and should not be
 * called directly by the application.
 *
 * Copyright (c) 2011, Microsemi Corporation
 *
 * $Revision: 9999 $
 * $LastChangedDate: 2012-05-14 10:57:37 -0500 (Mon, 14 May 2012) $
 */


#include "vp_api.h"

/* Compile only if required */
#if defined(VP886_INCLUDE_TESTLINE_CODE) || defined(VP886_INCLUDE_THERMAL_RINGING) 

#include "vp_api_int.h"
#include "vp886_math.h"
#include "vp_debug.h"

/* Simple min and max functions */
int16 Vp886max16 (int16 var1, int16 var2) {return (var1>var2)?var1:var2;}
int16 Vp886min16 (int16 var1, int16 var2) {return (var1<var2)?var1:var2;}

/*___________________________________________________________________________
 |                                                                           |
 |   Function Name : add                                                     |
 |                                                                           |
 |   Purpose :                                                               |
 |                                                                           |
 |    Performs the addition (var1+var2) with overflow control and saturation;|
 |    the 16 bit result is set at +32767 when overflow occurs or at -32768   |
 |    when underflow occurs.                                                 |
 |                                                                           |
 |   Complexity weight : 1                                                   |
 |                                                                           |
 |   Inputs :                                                                |
 |                                                                           |
 |    var1                                                                   |
 |             16 bit short signed integer (Word16) whose value falls in the |
 |             range : 0xffff 8000 <= var1 <= 0x0000 7fff.                   |
 |                                                                           |
 |    var2                                                                   |
 |             16 bit short signed integer (Word16) whose value falls in the |
 |             range : 0xffff 8000 <= var1 <= 0x0000 7fff.                   |
 |                                                                           |
 |   Outputs :                                                               |
 |                                                                           |
 |    none                                                                   |
 |                                                                           |
 |   Return Value :                                                          |
 |                                                                           |
 |    var_out                                                                |
 |             16 bit short signed integer (Word16) whose value falls in the |
 |             range : 0xffff 8000 <= var_out <= 0x0000 7fff.                |
 |___________________________________________________________________________|
*/

int16 Vp886add (int16 var1, int16 var2)
{
    int16 var_out;
    int32 L_sum;

    L_sum = (int32) var1 + var2;
    var_out = Vp886saturate (L_sum);
    return (var_out);
}  /*  add */


/*___________________________________________________________________________
 |                                                                           |
 |   Function Name : sub                                                     |
 |                                                                           |
 |   Purpose :                                                               |
 |                                                                           |
 |    Performs the subtraction (var1+var2) with overflow control and satu-   |
 |    ration; the 16 bit result is set at +32767 when overflow occurs or at  |
 |    -32768 when underflow occurs.                                          |
 |                                                                           |
 |   Complexity weight : 1                                                   |
 |                                                                           |
 |   Inputs :                                                                |
 |                                                                           |
 |    var1                                                                   |
 |             16 bit short signed integer (Word16) whose value falls in the |
 |             range : 0xffff 8000 <= var1 <= 0x0000 7fff.                   |
 |                                                                           |
 |    var2                                                                   |
 |             16 bit short signed integer (Word16) whose value falls in the |
 |             range : 0xffff 8000 <= var1 <= 0x0000 7fff.                   |
 |                                                                           |
 |   Outputs :                                                               |
 |                                                                           |
 |    none                                                                   |
 |                                                                           |
 |   Return Value :                                                          |
 |                                                                           |
 |    var_out                                                                |
 |             16 bit short signed integer (Word16) whose value falls in the |
 |             range : 0xffff 8000 <= var_out <= 0x0000 7fff.                |
 |___________________________________________________________________________|
*/

int16 Vp886sub (int16 var1, int16 var2)
{
    int16 var_out;
    int32 L_diff;

    L_diff = (int32) var1 - var2;
    var_out = Vp886saturate (L_diff);
    return (var_out);
}   /* sub */

/*___________________________________________________________________________
 |                                                                           |
 |   Function Name : mult_r                                                  |
 |                                                                           |
 |   Purpose :                                                               |
 |                                                                           |
 |   Same as mult with rounding, i.e.:                                       |
 |     mult_r(var1,var2) = extract_l(L_shr(((var1 * var2) + 16384),15)) and  |
 |     mult_r(-32768,-32768) = 32767.                                        |
 |                                                                           |
 |   Complexity weight : 2                                                   |
 |                                                                           |
 |   Inputs :                                                                |
 |                                                                           |
 |    var1                                                                   |
 |             16 bit short signed integer (Word16) whose value falls in the |
 |             range : 0xffff 8000 <= var1 <= 0x0000 7fff.                   |
 |                                                                           |
 |    var2                                                                   |
 |             16 bit short signed integer (Word16) whose value falls in the |
 |             range : 0xffff 8000 <= var1 <= 0x0000 7fff.                   |
 |                                                                           |
 |   Outputs :                                                               |
 |                                                                           |
 |    none                                                                   |
 |                                                                           |
 |   Return Value :                                                          |
 |                                                                           |
 |    var_out                                                                |
 |             16 bit short signed integer (Word16) whose value falls in the |
 |             range : 0xffff 8000 <= var_out <= 0x0000 7fff.                |
 |___________________________________________________________________________|
*/

int16 Vp886mult_r (int16 var1, int16 var2)
{
    int16 var_out;
    int32 L_product_arr;

    L_product_arr = (int32) var1 *(int32) var2;       /* product */
    L_product_arr += (int32) 0x00004000L;      /* round */
    L_product_arr &= (int32) 0xffff8000L;
    L_product_arr >>= 15;       /* shift */

    if (L_product_arr & (int32) 0x00010000L)  /* sign extend when necessary */
      L_product_arr |= (int32) 0xffff0000L;
    var_out = Vp886saturate (L_product_arr);
    return (var_out);
}

/**
 * saturate()
 *  Limits 32-bit argument to the range of a 16-bit word..
*/ 

int16
Vp886saturate(
    int32 inVal)
{
    int16 sResult;

    if (inVal < 0) {
        sResult = (int16) (inVal < (int32)0xFFFF8000) ? (int32)0xFFFF8000 : inVal;
    }else {
        sResult = (int16) (inVal > 0x7FFF) ? 0x7FFF : inVal;
    }
    /* look into using either VP_INT16_MAX or SHRT_MAX */
    return sResult;
}  /* saturate */

/*___________________________________________________________________________
 |                                                                           |
 |   Function Name : shl                                                     |
 |                                                                           |
 |   Purpose :                                                               |
 |                                                                           |
 |   Arithmetically shift the 16 bit input var1 left var2 positions.Zero fill|
 |   the var2 LSB of the result. If var2 is negative, arithmetically shift   |
 |   var1 right by -var2 with sign extension. Saturate the result in case of |
 |   underflows or overflows.                                                |
 |                                                                           |
 |   Complexity weight : 1                                                   |
 |                                                                           |
 |   Inputs :                                                                |
 |                                                                           |
 |    var1                                                                   |
 |             16 bit short signed integer (Word16) whose value falls in the |
 |             range : 0xffff 8000 <= var1 <= 0x0000 7fff.                   |
 |                                                                           |
 |    var2                                                                   |
 |             16 bit short signed integer (Word16) whose value falls in the |
 |             range : 0xffff 8000 <= var1 <= 0x0000 7fff.                   |
 |                                                                           |
 |   Outputs :                                                               |
 |                                                                           |
 |    none                                                                   |
 |                                                                           |
 |   Return Value :                                                          |
 |                                                                           |
 |    var_out                                                                |
 |             16 bit short signed integer (Word16) whose value falls in the |
 |             range : 0xffff 8000 <= var_out <= 0x0000 7fff.                |
 |___________________________________________________________________________|
*/

int16 Vp886shl (int16 var1, int16 var2)
{
    int16 var_out;
    int32 result;

    if (var2 < 0)
      var_out = Vp886shr (var1, -var2);
    else
    {
        result = (int32) var1 *((int32) 1 << var2);

        if ((var2 > 15 && var1 != 0) || (result != (int32) ((int16) result)))
        {
            var_out = (var1 > 0) ? MAX_16 : MIN_16;
        }
        else
          var_out = Vp886extract_l (result);
    }
    return (var_out);
} /* shl */

/*___________________________________________________________________________
 |                                                                           |
 |   Function Name : shr                                                     |
 |                                                                           |
 |   Purpose :                                                               |
 |                                                                           |
 |   Arithmetically shift the 16 bit input var1 right var2 positions with    |
 |   sign extension. If var2 is negative, arithmetically shift var1 left by  |
 |   -var2 with sign extension. Saturate the result in case of underflows or |
 |   overflows.                                                              |
 |                                                                           |
 |   Complexity weight : 1                                                   |
 |                                                                           |
 |   Inputs :                                                                |
 |                                                                           |
 |    var1                                                                   |
 |             16 bit short signed integer (Word16) whose value falls in the |
 |             range : 0xffff 8000 <= var1 <= 0x0000 7fff.                   |
 |                                                                           |
 |    var2                                                                   |
 |             16 bit short signed integer (Word16) whose value falls in the |
 |             range : 0xffff 8000 <= var1 <= 0x0000 7fff.                   |
 |                                                                           |
 |   Outputs :                                                               |
 |                                                                           |
 |    none                                                                   |
 |                                                                           |
 |   Return Value :                                                          |
 |                                                                           |
 |    var_out                                                                |
 |             16 bit short signed integer (Word16) whose value falls in the |
 |             range : 0xffff 8000 <= var_out <= 0x0000 7fff.                |
 |___________________________________________________________________________|
*/

int16 Vp886shr (int16 var1, int16 var2)
{
    int16 var_out;

    if (var2 < 0)
      var_out = Vp886shl (var1, -var2);
    else if (var2 >= 15)
      var_out = (var1 < 0) ? -1 : 0;
    else if (var1 < 0)
      var_out = ~((~var1) >> var2);
    else
      var_out = var1 >> var2;

    return (var_out);
} /* shr */

/*___________________________________________________________________________
 |                                                                           |
 |   Function Name : extract_l                                               |
 |                                                                           |
 |   Purpose :                                                               |
 |                                                                           |
 |   Return the 16 LSB of L_var1.                                            |
 |                                                                           |
 |   Complexity weight : 1                                                   |
 |                                                                           |
 |   Inputs :                                                                |
 |                                                                           |
 |    L_var1                                                                 |
 |             32 bit long signed integer (Word32 ) whose value falls in the |
 |             range : 0x8000 0000 <= L_var1 <= 0x7fff ffff.                 |
 |                                                                           |
 |   Outputs :                                                               |
 |                                                                           |
 |    none                                                                   |
 |                                                                           |
 |   Return Value :                                                          |
 |                                                                           |
 |    var_out                                                                |
 |             16 bit short signed integer (Word16) whose value falls in the |
 |             range : 0xffff 8000 <= var_out <= 0x0000 7fff.                |
 |___________________________________________________________________________|
*/

int16 Vp886extract_l (int32 L_var1)
{
    int16 var_out;

    var_out = (int16) L_var1;
    return (var_out);
} /* extract_l */

/*___________________________________________________________________________
 |                                                                           |
 |   Function Name : extract_h                                               |
 |                                                                           |
 |   Purpose :                                                               |
 |                                                                           |
 |   Return the 16 MSB of L_var1.                                            |
 |                                                                           |
 |   Complexity weight : 1                                                   |
 |                                                                           |
 |   Inputs :                                                                |
 |                                                                           |
 |    L_var1                                                                 |
 |             32 bit long signed integer (Word32 ) whose value falls in the |
 |             range : 0x8000 0000 <= L_var1 <= 0x7fff ffff.                 |
 |                                                                           |
 |   Outputs :                                                               |
 |                                                                           |
 |    none                                                                   |
 |                                                                           |
 |   Return Value :                                                          |
 |                                                                           |
 |    var_out                                                                |
 |             16 bit short signed integer (Word16) whose value falls in the |
 |             range : 0xffff 8000 <= var_out <= 0x0000 7fff.                |
 |___________________________________________________________________________|
*/

int16 Vp886extract_h (
    int32 L_var1)
{
    int16 var_out;

    var_out = (int16) (L_var1 >> 16);
    return (var_out);
} /* extract_h */


/***
 * L_add()
 * Performs long add of L_var1 and L_var2, with overflow control and saturation. 
 * If overflow occurs, result is +2147483647; if underflow occurs, result is 
 * -2147483648. 
 */ 
int32 Vp886L_add(
    int32 x, 
    int32 y) 
{
    /* Add the addends */
    int32 sum = x + y;

    /* If both addends are positive, but the result is negative, saturate positive */
    if ((x>0) && (y>0) && (sum<0)) sum=0x7FFFFFFF;
    /* If both addends are negative, but the result is positive, saturate negative */
    if ((x<0) && (y<0) && (sum>0)) sum=0x80000000;

    return sum;
} /* L_add */

/***
 * L_sub()
 * Performs long subtraction of L_var2 from L_var1, with overflow control and saturation. 
 * If overflow occurs, 32-bit result is +2147483647; if underflow occurs, 32-bit result is 
 * -2147483648 
 */
int32 Vp886L_sub(
    int32 x, 
    int32 y) 
{
    /* Subtract subtrahend from the minuend */
    int32 difference = x - y;

    /* If the minuend is negative and the subtrahend is positive and the difference is positive, saturate negative */
    if ((x<0) && (y>0) && (difference>0)) difference=0x80000000;
    /* If the minuend is positive and the subtrahend is negative and the difference is nagative, saturate positive */
    if ((x>0) && (y<0) && (difference<0)) difference=0x7FFFFFFF;

    return difference;
} /* L_sub */


/***
 * L_mult_ls()
 * Performs optimized 32*16 fractional multiplication. 
 * (Result differs from standard 32*16 multiplication in the least significant bit - 2** -31 error.) 
 */
int32 Vp886L_mult_ls(
    int32 var1, 
    int16 var2)
{
    int32 L_var_out;

    L_var_out = (int32)(unsigned short)(Vp886extract_l(var1)) * (int32) var2;
    L_var_out = L_var_out >> 15;
    L_var_out = Vp886L_mac(L_var_out, Vp886extract_h(var1), var2);

    return L_var_out;
} /* L_mult_ls */

/*___________________________________________________________________________
 |                                                                           |
 |   Function Name : abs_s                                                   |
 |                                                                           |
 |   Purpose :                                                               |
 |                                                                           |
 |    Absolute value of var1; abs_s(-32768) = 32767.                         |
 |                                                                           |
 |   Complexity weight : 1                                                   |
 |                                                                           |
 |   Inputs :                                                                |
 |                                                                           |
 |    var1                                                                   |
 |             16 bit short signed integer (Word16) whose value falls in the |
 |             range : 0xffff 8000 <= var1 <= 0x0000 7fff.                   |
 |                                                                           |
 |   Outputs :                                                               |
 |                                                                           |
 |    none                                                                   |
 |                                                                           |
 |   Return Value :                                                          |
 |                                                                           |
 |    var_out                                                                |
 |             16 bit short signed integer (Word16) whose value falls in the |
 |             range : 0x0000 0000 <= var_out <= 0x0000 7fff.                |
 |___________________________________________________________________________|
*/

int16 Vp886abs_s (int16 var1)
{
    int16 var_out;

    if (var1 == (int16) 0X8000) {
        var_out = MAX_16;
    } else {
        if (var1 < 0) {
            var_out = -var1;
        } else {
            var_out = var1;
        }
    }
    return (var_out);
} /* abs_s */

/*___________________________________________________________________________
 |                                                                           |
 |   Function Name : round32to16                                             |
 |                                                                           |
 |   Purpose :                                                               |
 |                                                                           |
 |   Round the lower 16 bits of the 32 bit input number into the MS 16 bits  |
 |   with saturation. Shift the resulting bits right by 16 and return the 16 |
 |   bit number:                                                             |
 |               round(L_var1) = extract_h(L_add(L_var1,32768))              |
 |                                                                           |
 |   Complexity weight : 1                                                   |
 |                                                                           |
 |   Inputs :                                                                |
 |                                                                           |
 |    L_var1                                                                 |
 |             32 bit long signed integer (Word32 ) whose value falls in the |
 |             range : 0x8000 0000 <= L_var1 <= 0x7fff ffff.                 |
 |                                                                           |
 |   Outputs :                                                               |
 |                                                                           |
 |    none                                                                   |
 |                                                                           |
 |   Return Value :                                                          |
 |                                                                           |
 |    var_out                                                                |
 |             16 bit short signed integer (Word16) whose value falls in the |
 |             range : 0xffff 8000 <= var_out <= 0x0000 7fff.                |
 |___________________________________________________________________________|
*/

int16 Vp886round32to16 (int32 L_var1)
{
    int16 var_out;
    int32 L_rounded;

    L_rounded = Vp886L_add (L_var1, (int32) 0x00008000L);
    var_out = Vp886extract_h (L_rounded);
    return (var_out);
} /* round32to16 */

/*___________________________________________________________________________
 |                                                                           |
 |   Function Name : L_deposit_h                                             |
 |                                                                           |
 |   Purpose :                                                               |
 |                                                                           |
 |   Deposit the 16 bit var1 into the 16 MS bits of the 32 bit output. The   |
 |   16 LS bits of the output are zeroed.                                    |
 |                                                                           |
 |   Complexity weight : 2                                                   |
 |                                                                           |
 |   Inputs :                                                                |
 |                                                                           |
 |    var1                                                                   |
 |             16 bit short signed integer (Word16) whose value falls in the |
 |             range : 0xffff 8000 <= var1 <= 0x0000 7fff.                   |
 |                                                                           |
 |   Outputs :                                                               |
 |                                                                           |
 |    none                                                                   |
 |                                                                           |
 |   Return Value :                                                          |
 |                                                                           |
 |    L_var_out                                                              |
 |             32 bit long signed integer (Word32) whose value falls in the  |
 |             range : 0x8000 0000 <= var_out <= 0x7fff 0000.                |
 |___________________________________________________________________________|
*/

int32 Vp886L_deposit_h (int16 var1)
{
    int32 L_var_out;

    L_var_out = (int32) var1 << 16;
    return (L_var_out);
} /* L_deposit_h */


/***
 * D_mult_msp()
 * Performs double-precision multiply of the Q31 most significant parts of a 64-bit word, 
 * storing the result in a Q31.
 */
int32 Vp886D_mult_msp (int32 L_var1, int32 L_var2 )
{
    int64fxp local;
    int32 l1,l2;
    int    mid;

    l1 = (int32)(unsigned short)(Vp886extract_l(L_var1)) *
       (int32)(Vp886extract_h(L_var2));
    l2 = (int32)(unsigned short)(Vp886extract_l(L_var2)) *
       (int32)(Vp886extract_h(L_var1));
    mid = Vp886L_add(l1,l2);

    l1 = (int32)(unsigned short)(Vp886extract_l(L_var1)) *
       (int32)(unsigned short)(Vp886extract_l(L_var2));
    l2 = (int32)(Vp886extract_h(L_var1)) *
       (int32)(Vp886extract_h(L_var2));

    local.low32 = Vp886L_add(l1,((int32)Vp886extract_l(mid))<<16);
    local.high32 = Vp886L_add(l2, (int32)Vp886extract_h(mid));

    local.high32 <<= 1;
    if ( local.low32 & 0x80000000 ) local.high32 += 1;
    local.low32 <<= 1;

    return local.high32;

} /* D_mult_msp */

int32
Vp886L_shr(
    int32 L_var1,
    int16 var2)
{
    int32 L_var_out;
    if(var2 < 0)
        L_var_out = Vp886L_shl(L_var1, -var2);

    else if(var2 >= 31)
        L_var_out = (L_var1 < 0L) ? -1 : 0;

    else if(L_var1 < 0)
        L_var_out = ~((~L_var1) >> var2);

    else
        L_var_out = L_var1 >> var2;
    return (L_var_out);
}       /*  L_shr()  */

int32
Vp886L_shr_r(
    int32 L_var1,
    int16 var2)
{
    int32 L_var_out;
    if(var2 > 31)
         {
        L_var_out = 0;
        }

    else
         {
        L_var_out = Vp886L_shr(L_var1, var2);
        if(var2 > 0)
             {
            if((L_var1 & ((int32) 1 << (var2 - 1))) != 0)
                 {
                L_var_out++;
                }
            }
        }
    return (L_var_out);
}       /*  L_shr_r()  */

int32
Vp886L_shl(
    int32 L_var1,
    int16 var2)
{
    int32 L_var_out;
    if(var2 <= 0) {
        L_var_out = Vp886L_shr(L_var1, -var2);
    } else {
        for(; var2 > 0; var2--)
             {
            if(L_var1 > (int32) 0X3fffffffL)
                 {
                L_var_out = 0x7fffffffL;
                break;
                }

            else
                 {
                if(L_var1 < (int32) 0xc0000000L)
                     {
                    L_var_out = 0x80000000L;
                    break;
                    }
                }
            L_var1 *= 2;
            L_var_out = L_var1;
            }
    }
    return (L_var_out);
}       /*  L_shl()  */

/***
 * L_shl_nosat()
 * Performs long shift left without saturation. Like L_shl, 
 * but does not saturate result in case of underflow or overflow. 
 */
int32 Vp886L_shl_nosat (
    int32 L_var1, 
    int16 var2)
{
    if (var2 <= 0)
        return (L_var1 >> -var2);
    else
        return (L_var1 << var2);
} /* L_shl_nosat() */

/*___________________________________________________________________________
 |                                                                           |
 |   Function Name : L_negate                                                |
 |                                                                           |
 |   Purpose :                                                               |
 |                                                                           |
 |   Negate the 32 bit variable L_var1 with saturation; saturate in the case |
 |   where input is -2147483648 (0x8000 0000).                               |
 |                                                                           |
 |   Complexity weight : 2                                                   |
 |                                                                           |
 |   Inputs :                                                                |
 |                                                                           |
 |    L_var1   32 bit long signed integer (Word32) whose value falls in the  |
 |             range : 0x8000 0000 <= L_var3 <= 0x7fff ffff.                 |
 |                                                                           |
 |   Outputs :                                                               |
 |                                                                           |
 |    none                                                                   |
 |                                                                           |
 |   Return Value :                                                          |
 |                                                                           |
 |    L_var_out                                                              |
 |             32 bit long signed integer (Word32) whose value falls in the  |
 |             range : 0x8000 0000 <= L_var_out <= 0x7fff ffff.              |
 |___________________________________________________________________________|
*/

int32 Vp886L_negate (int32 L_var1)
{
    int32 L_var_out;

    L_var_out = (L_var1 == MIN_32) ? MAX_32 : -L_var1;
    return (L_var_out);
} /* L_negate() */

/***
 * norm_l()
 * Normalizes 32-bit fractional argument: shifts left enough times to normalize a positive 
 * L_var1 (interval 1073741824 - 2147483647), or to normalize a negative 
 * L_var 1 (interval -2147483648 - -1073741824). 
 */
int16 Vp886norm_l (int32 myValue)
{
    int16 redundantSignBits;

    if (myValue == 0) {
        redundantSignBits = 0;
    } else if (myValue == (int32) 0xffffffffL) {
            redundantSignBits = 31;
    } else {
        if (myValue < 0) {
            myValue = ~myValue;
        }
        for (redundantSignBits = 0; myValue < (int32) 0x40000000L; redundantSignBits++) {
            myValue <<= 1;
        }
    }
    return (redundantSignBits);
} /* norm_l */

/**
 * This function divides a signed 32-bit integer numerator by a signed 32-bit
 * integer denominator and returns a signed 32-bit integer result. The result
 * is rounded in the natural way. That is, if the fractional part of the result
 * is >= +0.5 then the result is rounded-up to the next positive integer.
 * Similarly, if the fractional part of the result is <= -0.5 then the result
 * is rounded-down to the next negative integer. For example:
 *  -  -4 / +2 = -2.0 = -2
 *  -  -3 / +2 = -1.5 = -2
 *  -  -2 / +2 = -1.0 = -1
 *  -  -1 / +2 = -0.5 = -1
 *  -  +0 / +2 =  0.0 = +0
 *  -  +1 / +2 = +0.5 = +1
 *  -  +2 / +2 = +1.0 = +1
 *  -  +3 / +2 = +1.5 = +2
 *  -  +4 / +2 = +2.0 = +2
 *
 * "C, A Reference Manual," Harbison and Steele, 1995, Prentice-Hall, p. 202: \n
 * Division: For integral operands, if the mathematical quotient of the operands
 * is not an exact integer, then the result will be one of the two integers
 * closest to the mathematical quotient of the operands. Of those two integers,
 * the one closer to 0 must be chosen if both operands are positive (that is,
 * division of positive integers is truncating division). Note that this
 * completely specifies the behavior of division for unsigned operands. If
 * either operand is negative, then the choice is left to the discretion of the
 * implementor. For maximum portability, programs should therefore avoid
 * depending on the behavior of the division operator when applied to negative
 * integral operands.
 *
 * For this reason, we divide the absolute value of the numerator by the
 * absolute value of the denominator and adjust the sign of the result according
 * to the sign of the inputs.
 *
 * \param[in] numer     contains the numerator of the division operation as a
 *                      signed 32-bit integer.
 *
 * \param[in] denom     contains the denominator of the division operation as a
 *                      signed 32-bit integer.
 *
 * \return  Returns the result of (\a numer / \a denom) rounded toward toward
 *          the nearest integer value.
 *
 * \note    This function handles divide-by-zero by returning the maximum or
 *          minimum signed integer value depending on the expected sign of the
 *          result. These magic numbers are defined in int_math.h.
 *
 * \warning Division is a computationally expensive operation. The unsigned
 *          division operation (/=) alone takes about 130 instructions on
 *          StarCore to complete. Use with care!
 *
 * \todo    Consider adding application callback for divide-by-zero.
 */
#define POS_INFINITY ((int32)0x7FFFFFFF)
#define NEG_INFINITY ((int32)0x80000000)

EXTERN int32
Vp886Int32Div(
    int32 numer,
    int32 denom)
{
    uint32 n;
    uint32 d;
    signed char negResult = 0;

    /* Both operands of integer division should be positive for positive for
     * predictable truncation behavior. Also, forcing numerator and denominator
     * positive is required for the rounding algorithm below. Negate negative
     * inputs here and track whether a final negation of the output is required.
     * If negResult is non-zero (+1 or -1) then the input significands have
     * opposite signs, and therefore the output significand must be negated.
     *
     * Note that signed numerator and denominator operands are copied into
     * unsigned temporaries to prevent overflow (change of sign) when they are
     * multipled by two below. */

    if (numer < 0) {
        n = (uint32)(-numer);
        negResult++;
    } else {
        n = (uint32)numer;
    }

    if (denom < 0) {
        d = (uint32)(-denom);
        negResult--;
    } else {
        /* Denominator is either positive or zero. Test for divide-by-zero here.
         * Return the largest positive or smallest negative number as result,
         * depending on output sign. */

        if (denom == 0) {
            if (negResult) {
                return NEG_INFINITY;
            }

            return POS_INFINITY;
        } else {
            d = (uint32)denom;
        }
    }

    /* Round result toward +/- infinity (away from zero).
     * result = ( numer + denom / 2 ) / denom
     * result = ( numer * 2 + denom ) / ( 2 * denom ) */
    n *= 2;
    n += d;
    d *= 2;
    n /= d;

    /* Negate the output if the inputs had opposite signs, as indicated by
     * negResult being non-zero. */
    if (negResult) {
        numer = -(int32)n;
    } else {
        numer = (int32)n;
    }

    return numer;
}

#endif  /* VP792_INCLUDE_TESTLINE_CODE */
