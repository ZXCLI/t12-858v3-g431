//#############################################################################
//
// FILE:       emavg.h
//
// TITLE:      Contains the public interface to the Exponential Moving 
//             Average (EMAVG)
//
//#############################################################################
// $TI Release: Utilities Library v1.02.00.00 $
// $Release Date: Thu Dec 14 13:13:04 CST 2023 $
// $Copyright:
// Copyright (C) 2023 Texas Instruments Incorporated - http://www.ti.com/
//
// ALL RIGHTS RESERVED
// $
//#############################################################################

#ifndef EMAVG_H
#define EMAVG_H

#ifdef __cplusplus
extern "C" {
#endif

//*****************************************************************************
//
//! \addtogroup EMAVG
//! @{
//
//*****************************************************************************

//
// Included Files
//
//
// Included Files
//
#include <stdint.h>
#ifndef __TMS320C28XX_CLA__
#include <math.h>
#else
#include <CLAmath.h>
#endif

//#############################################################################
//
// Macro Definitions
//
//#############################################################################
#ifndef C2000_IEEE754_TYPES
#define C2000_IEEE754_TYPES
#ifdef __TI_EABI__
typedef float         float32_t;
typedef double        float64_t;
#else // TI COFF
typedef float         float32_t;
typedef long double   float64_t;
#endif // __TI_EABI__
#endif // C2000_IEEE754_TYPES

//
// Typedefs
//

//! \brief          Defines the Exponential Moving Average (EMAVG) structure
//!
//! \details        The emavg can be used to perform exponential moving
//!                 average calculation
//!
typedef struct {
    float32_t out;
    float32_t multiplier;
} EMAVG;

//! \brief      resets internal storage data
//! \param v    The EMAVG structure
//!
static inline void EMAVG_reset(EMAVG *v)
{
    v->out = 0;
}

//! \brief      configures EMAVG module
//! \param v    The EMAVG structure
//! \param multiplier Multiplier value
//!
static inline void EMAVG_config(EMAVG *v,
                               float32_t multiplier)
{
    v->multiplier = multiplier;
}

//! \brief      Run EMAVG module
//! \param v    The EMAVG structure
//! \param in   Input
//!
static inline void EMAVG_run(EMAVG *v,float in)
{
    v->out = ((in - v->out)*v->multiplier) + v->out;
}

//*****************************************************************************
//
// Close the Doxygen group.
//! @}
//
//*****************************************************************************

#ifdef __cplusplus
}
#endif // extern "C"

#endif // end of  _EMAVG_H_ definition

//
// End of File
//

