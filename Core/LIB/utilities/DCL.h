#ifndef DCL_H__
#define DCL_H__

#include "emavg.h"

typedef struct dcl_pi {
    float32_t Kp;       //!< Proportional gain
    float32_t Ki;       //!< Integral gain
    float32_t i10;      //!< I storage
    float32_t Umax;     //!< Upper control saturation limit
    float32_t Umin;     //!< Lower control saturation limit
    float32_t i6;       //!< Saturation storage
    float32_t i11;      //!< I storage
    float32_t Imax;     //!< Upper integrator saturation limit
    float32_t Imin;     //!< Lower integrator saturation limit
    // DCL_PI_SPS *sps;    //!< Pointer to the shadow parameter set
    // DCL_CSS *css;       //!< Pointer to the common support structure
} DCL_PI;

//! \brief  Defines default values to initialize the PI structure
//!
#define PI_DEFAULTS { 1.0f, 0.0f, 0.0f, 1.0f, -1.0f, 1.0f, 0.0f, 1.0f, -1.0f, \
                      }


//! \brief          Resets PID internal storage data
//! \param[in] p    Pointer to the DCL_PI structure
//! \return         None
//!
static inline void DCL_resetPI(DCL_PI *p)
{
    p->i10 = 0.0f;
    p->i6 = 1.0f;
    p->i11 = 0.0f;
}

//! \brief          Executes a parallel form PI controller on the FPU32
//!                 Implemented as inline C function
//! \param[in] p    Pointer to the DCL_PI structure
//! \param[in] rk   The controller set-point reference
//! \param[in] yk   The measured feedback value
//! \return         The control effort
//!
static inline float32_t DCL_runPI_C3(DCL_PI *p, float32_t rk, float32_t yk)
{
    float32_t v1, v2, v4, v5, v9;

    v1 = rk - yk;
    v2 = p->Kp * v1;
    v4 = (v1 * p->Ki * p->i6) + p->i10;
    p->i10 = v4;
    v5 = v2 + v4;
    v9 = (v5 > p->Umax) ? p->Umax : v5;
    v9 = (v9 < p->Umin) ? p->Umin : v9;
    p->i6 = (v5 == v9) ? 1.0f : 0.0f;

#ifdef DCL_TESTPOINTS_ENABLED
    p->css->tpt = v5;
#endif

    return(v9);
}

static inline float32_t DCL_runPI_C2(DCL_PI *p, float32_t rk, float32_t yk)
{
    float32_t v2, v4, v5, v9;

    v2 = p->Kp * (rk - yk);
    v4 = p->i10 + (p->Ki * p->i6 * v2);
    v5 = v2 + v4;
    v9 = (v5 > p->Umax) ? p->Umax : v5;
    v9 = (v9 < p->Umin) ? p->Umin : v9;
    p->i10 = v4;
    p->i6 = (v5 == v9) ? 1.0f : 0.0f;

#ifdef DCL_TESTPOINTS_ENABLED
    p->css->tpt = v5;
#endif

    return(v9);
}

//! \brief          Executes an parallel form PI controller on the FPU32
//!                 Contains enhanced AWR logic
//! \param[in] p    Pointer to the DCL_PI structure
//! \param[in] rk   The controller set-point reference
//! \param[in] yk   The measured feedback value
//! \return         The control effort
//!
static inline float32_t DCL_runPI_C5(DCL_PI *p, float32_t rk, float32_t yk)
{
    float32_t v1, v5, v7, v8;
    uint16_t l11, l12, l14, l17, l18, l19;

    v1 = rk - yk;
    v5 = (v1 * p->Ki * p->i6) + p->i10;
    p->i10 = v5;
    v7 = (v1 * p->Kp) + v5;
    v8 = (v7 > p->Umax) ? p->Umax : v7;
    v8 = (v8 < p->Umin) ? p->Umin : v8;
    l17 = ((v7 - v8) == 0) ? 1U : 0U;
    l11 = (v5 >= p->Imax) ? 1U : 0U;
    l12 = (v5 <= p->Imin) ? 1U : 0U;
    l19 = (v5 > 0) ? 1U : 0U;
    l14 = (v1 > 0) ? 1U : 0U;
    l18 = l17 & (!(l11 | l12) | (l19 ^ l14));
    p->i6 = (l18 == 0U) ? 0.0f : 1.0f;

#ifdef DCL_TESTPOINTS_ENABLED
    p->css->tpt = v7;
#endif

    return(v8);
}

//! \brief          Executes a series form PI controller with Tustin integrator
//!                 on the FPU32.
//! \param[in] p    Pointer to the DCL_PI structure
//! \param[in] rk   The controller set-point reference
//! \param[in] yk   The measured feedback value
//! \return         The control effort
//!
static inline float32_t DCL_runPI_C6(DCL_PI *p, float32_t rk, float32_t yk)
{
    float32_t v2, v4, v5, v8, v9;

    v2 = (rk - yk) * p->Kp;
    v8 = v2 * p->Ki * p->i6;
    v4 = v8 + p->i11 + p->i10;
    v5 = v2 + v4;
    p->i10 = v4;
    p->i11 = v8;
    v9 = (v5 > p->Umax) ? p->Umax : v5;
    v9 = (v9 < p->Umin) ? p->Umin : v9;
    p->i6 = (v5 == v9) ? 1.0f : 0.0f;

#ifdef DCL_TESTPOINTS_ENABLED
    p->css->tpt = v4;
#endif

    return(v9);
}

//! \brief          Defines the DCL_PID active controller structure
//!
typedef struct dcl_pid {
    float32_t Kp;       //!< Proportional gain
    float32_t Ki;       //!< Integral gain
    float32_t Kd;       //!< Derivative gain
    float32_t Kr;       //!< Set point weight
    float32_t c1;       //!< D path filter coefficient 1
    float32_t c2;       //!< D path filter coefficient 2
    float32_t d2;       //!< D path filter intermediate storage 1
    float32_t d3;       //!< D path filter intermediate storage 2
    float32_t i10;      //!< I path intermediate storage
    float32_t i14;      //!< Intermediate saturation storage
    float32_t Umax;     //!< Upper saturation limit
    float32_t Umin;     //!< Lower saturation limit
    // DCL_PID_SPS *sps;   //!< Pointer to the shadow parameter set
    // DCL_CSS *css;       //!< Pointer to the common support structure
} DCL_PID;

//! \brief          Defines default values to initialize the DCL_PID structure
//!
#define PID_DEFAULTS {  1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, \
                        0.0f, 1.0f, 1.0f, -1.0f}

//! \brief          Resets PID internal storage data
//! \param[in] p    Pointer to the DCL_PID structure
//! \return         None
//!
static inline void DCL_resetPID(DCL_PID *p)
{
    p->d2 = p->d3 = p->i10 = 0.0f;
    p->i14 = 1.0f;
}

//! \brief          Executes an ideal form PID controller on the FPU32
//! \param[in] p    Pointer to the DCL_PID structure
//! \param[in] rk   The controller set-point reference
//! \param[in] yk   The measured feedback value
//! \param[in] lk   External output clamp flag
//! \return         The control effort
//!
static inline float32_t DCL_runPID_C2(DCL_PID *p, float32_t rk, float32_t yk, float32_t lk)
{
    float32_t v1, v4, v5, v8, v9, v10, v12;

    v5 = (p->Kr * rk) - yk;
    v8 = ((rk - yk) * p->Ki * p->Kp * p->i14) + p->i10;
    p->i10 = v8;
    v1 = yk * p->Kd * p->c1;
    v4 = v1 - p->d2 - p->d3;
    p->d2 = v1;
    p->d3 = v4 * p->c2;
    v9 = ((v5 - v4) * p->Kp) + v8;
    v10 = (v9 > p->Umax) ? p->Umax : v9;
    v10 = (v10 < p->Umin) ? p->Umin : v10;
    v12 = (v10 == v9) ? 1.0f : 0.0f;
    p->i14 = v12 * lk;

#ifdef DCL_TESTPOINTS_ENABLED
    p->css->tpt = v4;
#endif

    return(v10);
}

#endif //DCL_H__