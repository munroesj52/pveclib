/*
 Copyright (c) [2017, 2018] IBM Corporation.

 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.

 arith128_test_f128.c

 Contributors:
      IBM Corporation, Steven Munroe
      Created on: Jul 31, 2017
      Commited on: Oct 10, 2018
 */

#include <stdint.h>
#include <stdio.h>
#include <fenv.h>
#include <float.h>
#ifdef __FLOAT128_TYPE__
#define __STDC_WANT_IEC_60559_TYPES_EXT__ 1
#define __STDC_WANT_IEC_60559_FUNCS_EXT__ 1
#endif
#ifndef PVECLIB_DISABLE_F128MATH
/* Disable <math.h> for __clang__ because of a bug involving <floatn.h>
   incombination with -mcpu=power9 -mfloat128. This means that ISO
   mandated <math.h> functions signbit(), isfinite(), isnormal(),
   isinf(), isnan(), etc are not available for __float128.  */
#include <math.h>
#endif

//#define __DEBUG_PRINT__
#include <pveclib/vec_f128_ppc.h>
/*! \brief A vector representation of a 256-bit unsigned integer.
 *
 *  A homogeneous aggregate of 2 x 128-bit unsigned integer fields.
 *  The low order field is named vx0, progressing to the high order
 *  field vx1.
 */
typedef struct
{
  ///@cond INTERNAL
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
  vui128_t vx0;
  vui128_t vx1;
#else
  vui128_t vx1;
  vui128_t vx0;
#endif
  ///@endcond
} __VEC_U_256;

#ifdef __FLOAT128__
__binary128
test_vec_maddqpo (__binary128, __binary128, __binary128);

#if 0
  const __float128 f128_fact0 = (1.0Q / 1.0Q);
  const __float128 f128_fact1 = (1.0Q / 2.0Q);
  const __float128 f128_fact2 = (1.0Q / 6.0Q);
  const __float128 f128_fact3 = (1.0Q / 24.0Q);
  const __float128 f128_fact4 = (1.0Q / 120.0Q);
  const __float128 f128_fact5 = (1.0Q / 720.0Q);
  const __float128 f128_fact6 = (1.0Q / 5040.0Q);
  const __float128 f128_fact7 = (1.0Q / 40320.0Q);
#endif

__float128
test_scalarlib_exp_f128 (__float128 term1st, __float128 f128_fact[])
{
  const __float128 f128_one = 1.0Q;
  __float128 term;

  // 1st 8 terms of e = 1 + 1/1! + 1/2!+ 1/3! ...
  term = test_vec_maddqpo (term1st, f128_fact[0], term1st);
  term = test_vec_maddqpo (term1st, f128_fact[1], term);
  term = test_vec_maddqpo (term1st, f128_fact[2], term);
  term = test_vec_maddqpo (term1st, f128_fact[3], term);
  term = test_vec_maddqpo (term1st, f128_fact[4], term);
  term = test_vec_maddqpo (term1st, f128_fact[5], term);
  term = test_vec_maddqpo (term1st, f128_fact[6], term);

  return test_vec_maddqpo (f128_one, f128_fact[7], term);
}

__float128
test_scalargcc_exp_f128 (__float128 term1st, __float128 f128_fact[])
{
  __float128 term;

#if defined (_ARCH_PWR9)
  const __float128 f128_one = 1.0Q;
  // 1st 8 terms of e = 1 + 1/1! + 1/2!+ 1/3! ...
  term = __builtin_fmaf128 (f128_one, f128_fact[0], term1st);
  term = __builtin_fmaf128 (f128_one, f128_fact[1], term);
  term = __builtin_fmaf128 (f128_one, f128_fact[2], term);
  term = __builtin_fmaf128 (f128_one, f128_fact[3], term);
  term = __builtin_fmaf128 (f128_one, f128_fact[4], term);
  term = __builtin_fmaf128 (f128_one, f128_fact[5], term);
  term = __builtin_fmaf128 (f128_one, f128_fact[6], term);

  return __builtin_fmaf128 (f128_one, f128_fact[7], term);
#else
  __float128 infrac;

  // 1st 8 terms of e = 1 + 1/1! + 1/2!+ 1/3! ...
  infrac = term1st * f128_fact[0];
  term =   term1st + infrac;
  infrac = term1st * f128_fact[1];
  term =   term + infrac;
  infrac = term1st * f128_fact[2];
  term =   term + infrac;
  infrac = term1st * f128_fact[3];
  term =   term + infrac;
  infrac = term1st * f128_fact[4];
  term =   term + infrac;
  infrac = term1st * f128_fact[5];
  term =   term + infrac;
  infrac = term1st * f128_fact[6];
  term =   term + infrac;
  infrac = term1st * f128_fact[7];
  return term + infrac;
#endif
}
#endif

vui128_t
test_vec_diveuqo (vui128_t x, vui128_t z)
{
  return vec_diveuqo_inline (x, z);
}

#ifdef PVECLIB_OLDTESTVERSIONS
vui128_t
test_vec_diveuqo_V0 (vui128_t x, vui128_t z)
{
  const vui64_t zeros = vec_splat_u64 (0);
  const vui128_t mone = (vui128_t) CONST_VINT128_DW(-1, -1);
#if defined (_ARCH_PWR10)
  vui128_t Q, R;
  vui128_t Rt, r1, t;
  vb128_t CC;
  // Based on vec_divdqu with parm y = 0
  Q  = vec_vdiveuq_inline (x, z);
  // R = -(Q * z)
  r1 = vec_mulluq (Q, z);
  R  = vec_subuqm ((vui128_t) zeros, r1);

  CC = vec_cmpgeuq (R, z);
  // Corrected Quotient before rounding.
  // if Q needs correction (Q+1), Bool CC is True, which is -1
  Q = vec_subuqm (Q, (vui128_t) CC);
  // Corrected Remainder
  Rt = vec_subuqm (R, z);
  R = vec_seluq (R, Rt, CC);
  // Convert nonzero remainder into a carry (=1).
  t = vec_addcuq (R, mone);
  Q = (vui128_t) vec_or ((vui32_t) Q, (vui32_t) t);
  return Q;
#else
  vui128_t x0, x1, z1, q0, k, s, t, zn;
  vui64_t zdh, qdl, qdh;
#if 1
  // For xsdivqpo x and z will always be normalized quadword
  // significands, and divisor (z) is greater than the dividend (x).
  // Shift the divisor and dividend as far left as possible
  // by re-normalizing the divisor so the MSB is 1.
  // Could use vec_clzq(), but we know  z >= 2**64, So:
  zn = (vui128_t) vec_clzd ((vui64_t) z);
  // zn = zn >> 64;, So we can use it with vec_slq ()
  zn = (vui128_t) vec_splatd ((vui64_t) zn, VEC_DW_H);
  // renormalize dividend and divisor
  x1 = vec_slq (x, zn);
  z1 = vec_slq (z, zn);
#endif

  // Check for overflow (x >= z) where the quotient can not be
  // represented in 128-bits, or zero divide
  if (__builtin_expect (
      vec_cmpuq_all_lt (x, z) && vec_cmpuq_all_ne (z, (vui128_t) zeros), 1))
    {
      // Check for x != 0
      if (__builtin_expect (vec_cmpuq_all_ne (x, (vui128_t) zeros), 1))
	{
#if 1
#if 0
	  // Here z >= 2**64, Normalize the divisor so MSB is 1
	  // Could use vec_clzq(), but we know  z >= 2**64, So:
	  zn = (vui128_t) vec_clzd ((vui64_t) z);
	  // zn = zn >> 64;, So we can use it with vec_slq ()
	  zn = (vui128_t) vec_mrgahd ((vui128_t) zeros, zn);

	  // Normalize dividend and divisor
	  x1 = vec_slq (x, zn);
	  z1 = vec_slq (z, zn);
#endif
	  zdh = vec_splatd ((vui64_t) z1, VEC_DW_H);
#else
	  zdh = vec_splatd ((vui64_t) z, VEC_DW_H);
	  zdl = vec_splatd ((vui64_t) z, VEC_DW_L);
#endif
	  // zdl == 0 is an important case.
	  // Optimize for zdl ==  0 as single (DW) digit long division
	  if (__builtin_expect (vec_cmpud_any_eq ((vui64_t) z1, zeros), 1))
	    {
#if 1
	      x = x1;
#endif
	      // Generate the 1st quotient digit
	      qdh = vec_divqud_inline (x, zdh);
	      // vec_divqud already provides the remainder in qdh[1]
	      // k = x1 - q1*z;  Simplifies to:
	      x1 = (vui128_t) vec_pasted (qdh, (vui64_t) zeros);
	      // generate the 2nd quotient digit
	      qdl = vec_divqud_inline (x1, zdh);
	      //return (vui128_t) {qlh, qdl}; After round to odd
	      q0 = (vui128_t) vec_mrgald ((vui128_t) qdh, (vui128_t) qdl);
	      s = (vui128_t) vec_mrgahd ((vui128_t) qdl, (vui128_t) zeros);
	      // Convert nonzero remainder into a carry (=1).
	      t = vec_addcuq (s, mone);
	      return (vui128_t) vec_or ((vui32_t) q0, (vui32_t) t);
	    }
	  else
	    {
	      vui128_t k1, x2, t2, q2;
	      vui128_t s, s0, s1;
	      vb128_t Bgt;
	      vb64_t Beq;
#if 0
	      // Here z >= 2**64, Normalize the divisor so MSB is 1
	      // Could use vec_clzq(), but we know  z >= 2**64, So:
	      zn = (vui128_t) vec_clzd ((vui64_t) z);
	      // zn = zn >> 64;, So we can use it with vec_slq ()
	      zn = (vui128_t) vec_mrgahd ((vui128_t) zeros, zn);

	      // Normalize dividend and divisor
	      x1 = vec_slq (x, zn);
	      z1 = vec_slq (z, zn);
#endif

	      // estimate the quotient 1st digit
	      qdh = vec_divqud_inline (x1, (vui64_t) z1);
              // detect overflow if ((x >> 64) == ((z >> 64)))
	      // a doubleword boolean true == __UINT64_MAX__
	      Beq = vec_cmpequd ((vui64_t) x1, (vui64_t) z1);
	      // Beq >> 64
	      Beq  = (vb64_t) vec_mrgahd ((vui128_t) zeros, (vui128_t) Beq);
	      // Adjust quotient (-1) for divide overflow
	      qdh = (vui64_t) vec_or ((vui32_t) Beq, (vui32_t) qdh);

	      // q0 = qdh << 64
	      q0 = (vui128_t) vec_mrgald ((vui128_t) qdh, (vui128_t) zeros);
	      // Compute 1st digit remainder
	      // {k, k1}  = vec_muludq (z1, q0);
	      { // Optimized for 128-bit by 64-bit multiply
		vui128_t l128, h128;
		vui64_t b_eud = vec_mrgald ((vui128_t) qdh, (vui128_t) qdh);
		l128 = vec_vmuloud ((vui64_t ) z1, b_eud);
		h128 = vec_vmaddeud ((vui64_t ) z1, b_eud, (vui64_t ) l128);
		// 192-bit product of v1 * q-estimate
		k  = h128;
		k1 = vec_slqi (l128, 64);
	      }
	      // Also a double QW compare for {x1 || 0} > {k || k1}
	      x2 = vec_subuqm ((vui128_t) zeros, k1);
	      t = vec_subcuq ((vui128_t) zeros, k1);
	      x0 = vec_subeuqm (x1, k, t);
	      t2 = vec_subecuq (x1, k, t);
	      // NOT carry of (x - k) -> k gt x
	      Bgt = vec_setb_ncq (t2);

	      x0 = vec_sldqi (x0, x2, 64);
	      // Adjust Q if initial guess is too high ( Q+1 )
	      // q2 = (vui128_t) vec_subudm ((vui64_t) q0, ones);
	      q2 = vec_adduqm (q0, mone);
	      //t2 = vec_subuqm (x0, (vui128_t) zdh);
	      x2 = vec_adduqm ((vui128_t) x0, z1);
	      q0 = vec_seluq (q0, q2, Bgt);
	      x0 = vec_seluq (x0, x2, Bgt);

	      qdh = (vui64_t) vec_mrgahd ((vui128_t) zeros, (vui128_t) q0);

	      qdl = vec_divqud_inline (x0, (vui64_t) z1);
	      q0 = (vui128_t) vec_mrgald ((vui128_t) qdh, (vui128_t) qdl);
	      k1 = vec_muludq (&k, q0, z1);
	      // NOT carry of (x - k) -> k gt x
	      t = vec_subcuq ((vui128_t) zeros, k1);
	      x2 = vec_subuqm ((vui128_t) zeros, k1);
	      t2 = vec_subecuq (x1, k, t);
	      // x0 = vec_subeuqm (x1, k, t);
	      Bgt = vec_setb_ncq (t2);
	      // The remainder should fit into x2 (x0 == 0) and
	      // should be less than z1.
	      // If not it will be after correction.
	      // Collect initial remainder for sticky-bits.
	      s0 = x2;
	      // Adjust Q if initial guess is too high ( Q+1 )
	      q2 = vec_adduqm (q0, mone);
	      q0 = vec_seluq (q0, q2, Bgt);
	      // Subtract divisor (z1) from initial remainder (x2)
	      // for corrected remainder.
	      // t = vec_subcuq (x2, z1);
	      x2 = vec_subuqm (x2, z1);
	      // x0 = vec_subeuqm (x0, (vui128_t) zeros, t);
	      // Collect corrected remainder for sticky-bits.
	      s1 = x2;
	      // Test for nonzero remainder and round to odd
	      // Select from initial or corrected sticky bits
	      s  = vec_seluq (s0, s1, Bgt);
	      // Convert nonzero remainder into a carry (=1).
	      t2 = vec_addcuq (s, mone);
	      q0 = (vui128_t) vec_or ((vui32_t) q0, (vui32_t) t2);
	      return q0;
	    }
	}
      else  // if (x == 0) return 0 as Quotient
	{
	  return ((vui128_t) zeros);
	}
    }
  else
    { //  undef -- overlow or zero divide
      // If the quotient cannot be represented in 128 bits, or if
      // an attempt is made divide any value by 0
      // then the results are undefined. We use __UINT128_MAX__.
      return mone;
    }
#endif
}

vui128_t
test_vec_diveuqo_V1 (vui128_t x, vui128_t z)
{
  const vui64_t zeros = vec_splat_u64 (0);
  const vui128_t mone = (vui128_t) CONST_VINT128_DW(-1, -1);
#if defined (_ARCH_PWR10)
  vui128_t Q, R;
  vui128_t Rt, r1, t;
  vb128_t CC;
  // Based on vec_divdqu with parm y = 0
  Q  = vec_vdiveuq_inline (x, z);
  // R = -(Q * z)
  r1 = vec_mulluq (Q, z);
  R  = vec_subuqm ((vui128_t) zeros, r1);

  CC = vec_cmpgeuq (R, z);
  // Corrected Quotient before rounding.
  // if Q needs correction (Q+1), Bool CC is True, which is -1
  Q = vec_subuqm (Q, (vui128_t) CC);
  // Corrected Remainder
  Rt = vec_subuqm (R, z);
  R = vec_seluq (R, Rt, CC);
  // Convert nonzero remainder into a carry (=1).
  t = vec_addcuq (R, mone);
  Q = (vui128_t) vec_or ((vui32_t) Q, (vui32_t) t);
  return Q;
#else
  vui128_t x0, x1, z1, q0, k, s, t, zn;
  vui64_t zdh, qdl, qdh;

  // For xsdivqpo x and z will always be normalized quadword
  // significands, and divisor (z) is greater than the dividend (x).
  // Shift the divisor and dividend as far left as possible
  // by re-normalizing the divisor so the MSB is 1.
  // Could use vec_clzq(), but we know  z >= 2**64, So:
  zn = (vui128_t) vec_clzd ((vui64_t) z);
  // zn = zn >> 64;, So we can use it with vec_slq ()
  zn = (vui128_t) vec_splatd ((vui64_t) zn, VEC_DW_H);
  // renormalize dividend and divisor
  x1 = vec_slq (x, zn);
  z1 = vec_slq (z, zn);

  // Check for overflow (x >= z) where the quotient can not be
  // represented in 128-bits, or zero divide
  if (__builtin_expect (
      vec_cmpuq_all_lt (x, z) && vec_cmpuq_all_ne (z, (vui128_t) zeros), 1))
    {
      // Check for x != 0
      if (__builtin_expect (vec_cmpuq_all_ne (x, (vui128_t) zeros), 1))
	{
	  zdh = vec_splatd ((vui64_t) z1, VEC_DW_H);
	  // zdl == 0 is an important case.
	  // Optimize for zdl ==  0 as single (DW) digit long division
	  if (__builtin_expect (vec_cmpud_any_eq ((vui64_t) z1, zeros), 1))
	    {
	      // Generate the 1st quotient digit
	      qdh = vec_divqud_inline (x1, zdh);
	      // vec_divqud already provides the remainder in qdh[1]
	      // k = x1 - q1*z;  Simplifies to:
	      x0 = (vui128_t) vec_pasted (qdh, (vui64_t) zeros);
	      // generate the 2nd quotient digit
	      qdl = vec_divqud_inline (x0, zdh);
	      //return (vui128_t) {qlh, qdl}; After round to odd
	      q0 = (vui128_t) vec_mrgald ((vui128_t) qdh, (vui128_t) qdl);
	      // Isolate remainder Doubleword
	      s = (vui128_t) vec_mrgahd ((vui128_t) qdl, (vui128_t) zeros);
	      // Convert nonzero remainder into a carry (=1).
	      t = vec_addcuq (s, mone);
	      return (vui128_t) vec_or ((vui32_t) q0, (vui32_t) t);
	    }
	  else
	    {
	      vui128_t k1, x2, t2, q2;
	      vb128_t Bgt;
	      vb64_t Beq;

	      // already re-normalizing so divisor MSB is 1, above
	      // estimate the quotient 1st digit
	      qdh = vec_divqud_inline (x1, (vui64_t) z1);
              // detect overflow if ((x >> 64) == ((z >> 64)))
	      // a doubleword boolean true == __UINT64_MAX__
	      Beq = vec_cmpequd ((vui64_t) x1, (vui64_t) z1);
	      // Beq >> 64
	      Beq  = (vb64_t) vec_mrgahd ((vui128_t) zeros, (vui128_t) Beq);
	      // Adjust quotient (-1) for divide overflow
	      qdh = (vui64_t) vec_or ((vui32_t) Beq, (vui32_t) qdh);

	      // Compute 1st digit remainder
	      // {k, k1}  = vec_muludq (z1, q0);
	      { // Optimized for 128-bit by 64-bit multiply
		vui128_t l128, h128;
		vui64_t b_eud = vec_mrgald ((vui128_t) qdh, (vui128_t) qdh);
		l128 = vec_vmuloud ((vui64_t ) z1, b_eud);
		h128 = vec_vmaddeud ((vui64_t ) z1, b_eud, (vui64_t ) l128);
		// 192-bit product of v1 * q-estimate
		k  = h128;
		k1 = vec_slqi (l128, 64);
	      }
	      // Also a double QW compare for {x1 || 0} > {k || k1}
	      x2 = vec_subuqm ((vui128_t) zeros, k1);
	      t = vec_subcuq ((vui128_t) zeros, k1);
	      x0 = vec_subeuqm (x1, k, t);
	      t2 = vec_subecuq (x1, k, t);
	      // NOT carry of (x - k) -> k gt x
	      Bgt = vec_setb_ncq (t2);

	      x0 = vec_sldqi (x0, x2, 64);
	      // Doubleword add will do here, only 64-bits so far
	      q2 = (vui128_t) vec_addudm (qdh, (vui64_t) mone);
	      qdh = (vui64_t) vec_seluq ((vui128_t) qdh, q2, Bgt);
	      x2 = vec_adduqm ((vui128_t) x0, z1);
	      x0 = vec_seluq (x0, x2, Bgt);

              // estimate the 2nd quotient digit
	      qdl = vec_divqud_inline (x0, (vui64_t) z1);
	      // Compute 2nd digit remainder
	      // simplify to 128x64 bit product with 64-bit qdl
	      x1 = x0;
	      {
		vui128_t l128, h128;
		vui64_t b_eud = vec_mrgald ((vui128_t) qdl, (vui128_t) qdl);
		l128 = vec_vmuloud ((vui64_t ) z1, b_eud);
		h128 = vec_vmaddeud ((vui64_t ) z1, b_eud, (vui64_t ) l128);
		// 192-bit product of v1 * qdl estimate
		k  = h128;
		k1 = vec_slqi (l128, 64);
	      }
	      // A double QW compare for {x1||0} > {k||k1}
	      // NOT carry of (x - k) -> k gt x
	      t = vec_subcuq ((vui128_t) zeros, k1);
	      t2 = vec_subecuq (x1, k, t);
	      Bgt = vec_setb_ncq (t2);

	      // corrected 2nd remainder if remainder is negative
	      x2 = vec_subuqm ((vui128_t) zeros, k1);
	      x0 = vec_subeuqm (x1, k, t);
	      // Remainder will fit into 128-bits
	      x0 = vec_sldqi (x0, x2, 64);
	      x2 = vec_adduqm ((vui128_t) x0, z1);
	      x0 = vec_seluq (x0, x2, Bgt);

	      // Correct combined quotient if 2nd remainder negative
	      q0 = (vui128_t) vec_mrgald ((vui128_t) qdh, (vui128_t) qdl);
	      q2 = vec_adduqm (q0, mone);
	      q0 = vec_seluq (q0, q2, Bgt);
	      // Convert nonzero remainder into a carry (=1).
	      t2 = vec_addcuq (x0, mone);
	      // If remainder nonzero then Round to Odd
	      q0 = (vui128_t) vec_or ((vui32_t) q0, (vui32_t) t2);
	      return q0;
	    }
	}
      else  // if (x == 0) return 0 as Quotient
	{
	  return ((vui128_t) zeros);
	}
    }
  else
    { //  undef -- overlow or zero divide
      // If the quotient cannot be represented in 128 bits, or if
      // an attempt is made divide any value by 0
      // then the results are undefined. We use __UINT128_MAX__.
      return mone;
    }
#endif
}
#endif

vui128_t
test_vec_diveuq_qpo (vui128_t x, vui128_t z)
{
  const vui64_t zeros = vec_splat_u64 (0);
  const vui128_t mone = (vui128_t) CONST_VINT128_DW(-1, -1);
#if defined (_ARCH_PWR10)
  vui128_t Q, R;
  vui128_t Rt, r1, t;
  vb128_t CC;
	// Based on vec_divdqu with parm y = 0
  Q = vec_vdiveuq_inline (x, z);
	// R = -(Q * z)
  r1 = vec_mulluq (Q, z);
  R = vec_subuqm ((vui128_t) zeros, r1);

  CC = vec_cmpgeuq (R, z);
	// Corrected Quotient before rounding.
	// if Q needs correction (Q+1), Bool CC is True, which is -1
  Q = vec_subuqm (Q, (vui128_t) CC);
	// Corrected Remainder
  Rt = vec_subuqm (R, z);
  R = vec_seluq (R, Rt, CC);
	// Convert nonzero remainder into a carry (=1).
  t = vec_addcuq (R, mone);
  Q = (vui128_t) vec_or ((vui32_t) Q, (vui32_t) t);
  return Q;
#else
  vui128_t x0, x1, z1, q0, k, s, t, zn;
  vui64_t zdh, qdl, qdh;

	// For xsdivqpo x and z will always be normalized quadword
	// significands, and divisor (z) is greater than the dividend (x).
	// Shift the divisor and dividend as far left as possible
	// by re-normalizing the divisor so the MSB is 1.
	// Could use vec_clzq(), but we know  z >= 2**64, So:
  zn = (vui128_t) vec_clzd ((vui64_t) z);
    // zn = zn >> 64;, So we can use it with vec_slq ()
  zn = (vui128_t) vec_splatd ((vui64_t) zn, VEC_DW_H);
    // renormalize dividend and divisor
  x1 = vec_slq (x, zn);
  z1 = vec_slq (z, zn);
    // >>>>
  zdh = vec_splatd ((vui64_t) z1, VEC_DW_H);
    // zdl == 0 is an important case.
    // Optimize for zdl ==  0 as single (DW) digit long division
  if (__builtin_expect (vec_cmpud_any_eq ((vui64_t) z1, zeros), 1))
  {
    // Generate the 1st quotient digit
    qdh = vec_divqud_inline (x1, zdh);
    // vec_divqud already provides the remainder in qdh[1]
    // k = x1 - q1*z;  Simplifies to:
    x0 = (vui128_t) vec_pasted (qdh, (vui64_t) zeros);
    // generate the 2nd quotient digit
    qdl = vec_divqud_inline (x0, zdh);
    //return (vui128_t) {qlh, qdl}; After round to odd
    q0 = (vui128_t) vec_mrgald ((vui128_t) qdh, (vui128_t) qdl);
    // Isolate remainder Doubleword
    s = (vui128_t) vec_mrgahd ((vui128_t) qdl, (vui128_t) zeros);
    // Convert nonzero remainder into a carry (=1).
    t = vec_addcuq (s, mone);
    return (vui128_t) vec_or ((vui32_t) q0, (vui32_t) t);
  }
  else
  {
    vui128_t k1, x2, t2, q2;
    vb128_t Bgt;
    vb64_t Beq;

    // already re-normalizing so divisor MSB is 1, above
    // estimate the quotient 1st digit
    qdh = vec_divqud_inline (x1, (vui64_t) z1);
    // detect overflow if ((x >> 64) == ((z >> 64)))
    // a doubleword boolean true == __UINT64_MAX__
    Beq = vec_cmpequd ((vui64_t) x1, (vui64_t) z1);
    // Beq >> 64
    Beq = (vb64_t) vec_mrgahd ((vui128_t) zeros, (vui128_t) Beq);
    // Adjust quotient (-1) for divide overflow
    qdh = (vui64_t) vec_or ((vui32_t) Beq, (vui32_t) qdh);

    // Compute 1st digit remainder
    // {k, k1}  = vec_muludq (z1, q0);
      { // Optimized for 128-bit by 64-bit multiply
	vui128_t l128, h128;
	vui64_t b_eud = vec_mrgald ((vui128_t) qdh, (vui128_t) qdh);
	l128 = vec_vmuloud ((vui64_t) z1, b_eud);
	h128 = vec_vmaddeud ((vui64_t) z1, b_eud, (vui64_t) l128);
	// 192-bit product of v1 * q-estimate
	k = h128;
	k1 = vec_slqi (l128, 64);
      }
    // Also a double QW compare for {x1 || 0} > {k || k1}
    x2 = vec_subuqm ((vui128_t) zeros, k1);
    t = vec_subcuq ((vui128_t) zeros, k1);
    x0 = vec_subeuqm (x1, k, t);
    t2 = vec_subecuq (x1, k, t);
    // NOT carry of (x - k) -> k gt x
    Bgt = vec_setb_ncq (t2);

    x0 = vec_sldqi (x0, x2, 64);
    // Doubleword add will do here, only 64-bits so far
    q2 = (vui128_t) vec_addudm (qdh, (vui64_t) mone);
    qdh = (vui64_t) vec_seluq ((vui128_t) qdh, q2, Bgt);
    x2 = vec_adduqm ((vui128_t) x0, z1);
    x0 = vec_seluq (x0, x2, Bgt);

    // estimate the 2nd quotient digit
    qdl = vec_divqud_inline (x0, (vui64_t) z1);
    // Compute 2nd digit remainder
    // simplify to 128x64 bit product with 64-bit qdl
    x1 = x0;
      {
	vui128_t l128, h128;
	vui64_t b_eud = vec_mrgald ((vui128_t) qdl, (vui128_t) qdl);
	l128 = vec_vmuloud ((vui64_t) z1, b_eud);
	h128 = vec_vmaddeud ((vui64_t) z1, b_eud, (vui64_t) l128);
	// 192-bit product of v1 * qdl estimate
	k = h128;
	k1 = vec_slqi (l128, 64);
      }
    // A double QW compare for {x1||0} > {k||k1}
    // NOT carry of (x - k) -> k gt x
    t = vec_subcuq ((vui128_t) zeros, k1);
    t2 = vec_subecuq (x1, k, t);
    Bgt = vec_setb_ncq (t2);

    // corrected 2nd remainder if remainder is negative
    x2 = vec_subuqm ((vui128_t) zeros, k1);
    x0 = vec_subeuqm (x1, k, t);
    // Remainder will fit into 128-bits
    x0 = vec_sldqi (x0, x2, 64);
    x2 = vec_adduqm ((vui128_t) x0, z1);
    x0 = vec_seluq (x0, x2, Bgt);

    // Correct combined quotient if 2nd remainder negative
    q0 = (vui128_t) vec_mrgald ((vui128_t) qdh, (vui128_t) qdl);
    q2 = vec_adduqm (q0, mone);
    q0 = vec_seluq (q0, q2, Bgt);
    // Convert nonzero remainder into a carry (=1).
    t2 = vec_addcuq (x0, mone);
    // If remainder nonzero then Round to Odd
    return (vui128_t) vec_or ((vui32_t) q0, (vui32_t) t2);
  }
#endif
}

vui64_t
force_eMin (vui64_t x_exp)
{
  vb64_t exp_mask;
  const vui64_t exp_dnrm = { 0, 0 };
  const vui64_t exp_min = vec_splat_u64 (1);
  // Correct exponent for zeros or denormals to E_min
  // will force 0 exponents for zero/denormal results later
  exp_mask = vec_cmpequd (x_exp, exp_dnrm);
  return (vui64_t) vec_sel ((vui32_t) x_exp, (vui32_t) exp_min, (vui32_t) exp_mask);
}

vui64_t
force_eMin_V0 (vui64_t x_exp)
{
  vb64_t exp_mask;
  const vui64_t exp_dnrm = { 0, 0 };
  const vui64_t exp_min = vec_splat_u64 (1);
  // Correct exponent for zeros or denormals to E_min
  // will force 0 exponents for zero/denormal results later
  exp_mask = vec_cmpequd (x_exp, exp_dnrm);
  return vec_sel (x_exp, exp_min, exp_mask);
}

__binary128
test_vec_xsaddqpo (__binary128 vfa, __binary128 vfb)
{
  return vec_xsaddqpo_inline (vfa, vfb);
}

__binary128
test_vec_xssubqpo (__binary128 vfa, __binary128 vfb)
{
  return vec_xssubqpo_inline (vfa, vfb);
}

vui32_t
test_const128_f128_fmax_V2(void)
{
  vui32_t mag = vec_mask128_f128mag ();
  vui32_t lbit = vec_mask128_f128Lbit ();
  return vec_andc (mag, lbit);
}

vui32_t
test_const128_f128_fmax_V1(void)
{
  vui32_t mag = vec_mask128_f128mag ();
  vui32_t lbit = vec_mask128_f128Lbit ();
  return vec_sub (mag, lbit);
}

#if (__GNUC__ > 6) || (__clang_major__ > 15)
vui32_t
test_const128_f128_fmax_V0(void)
{
#ifdef __FLOAT128__
  __binary128 maxf128 = __FLT128_MAX__;
  return vec_xfer_bin128_2_vui32t (maxf128);
#endif
}
#endif

vui64_t
test_const64_f128_16_V2(void)
{
  vui32_t mask32;

  mask32 = vec_splat_u32 (8);
  // Generate {16, 16, 16, 16, 16, 16, 16, 16} from count leading zeros of {0,..}
  mask32 = vec_add(mask32, mask32);
#if defined (_ARCH_PWR8)
  mask32 = (vui32_t) vec_unpackl ( (vi32_t) mask32 );
#else
  const vui32_t q_zero = CONST_VINT128_W (0, 0, 0, 0);
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
  mask32= vec_mergeh (mask32, q_zero);
#else
  mask32 = vec_mergel (q_zero, mask32);
#endif
#endif
  return (vui64_t) mask32;
}

vui64_t
test_const64_f128_16_V1(void)
{
  vui32_t shift32, mask32;

  shift32 = vec_splat_u32 (4);
  mask32 = vec_splat_u32 (1);
  // Generate {16, 16, 16, 16, 16, 16, 16, 16} from count leading zeros of {0,..}
  mask32 = vec_sl(mask32, shift32);
#if defined (_ARCH_PWR8)
  mask32 = (vui32_t) vec_unpackl ( (vi32_t) mask32 );
#else
  const vui32_t q_zero = CONST_VINT128_W (0, 0, 0, 0);
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
  mask32= vec_mergeh (mask32, q_zero);
#else
  mask32 = vec_mergel (q_zero, mask32);
#endif
#endif
  return (vui64_t) mask32;
}

vui64_t
test_const64_f128_16_V0(void)
{
  const vui32_t q_zero = CONST_VINT128_W (0, 0, 0, 0);
  vui32_t mask32;
  vui16_t mask16;
  // Generate {16, 16, 16, 16, 16, 16, 16, 16} from count leading zeros of {0,..}
  mask16 = vec_clzh((vui16_t) q_zero);
  mask32 = (vui32_t) vec_unpackl ((vi16_t) mask16);
#if defined (_ARCH_PWR8)
  mask32 = (vui32_t) vec_unpackl ( (vi32_t) mask32 );
#else
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
  mask32= vec_mergeh (mask32, q_zero);
#else
  mask32 = vec_mergel (q_zero, mask32);
#endif
#endif
  return (vui64_t) mask32;
}

vui64_t
test_const64_f128_63 (void)
{
  return vec_const64_f128_63 ();
}

vui64_t
test_const64_f128_127 (void)
{
  return vec_const64_f128_127 ();
}

vui64_t
test_const64_f128_112 (void)
{
  return vec_const64_f128_112 ();
}

vui64_t
test_const64_f128_112_V1 (void)
{
  return vec_splat_u64 (112);
}

vui64_t
test_const64_f128_112_V0(void)
{
  //  const vui32_t dw_128 = CONST_VINT128_W(0, 112, 0, 112};
  vui32_t shift32, mask32;

  shift32 = vec_splat_u32 (4);
  mask32 = vec_splat_u32 (7);
  // result (0x7 << 4) = 0x70
  mask32 = vec_sl(mask32, shift32);
#if defined (_ARCH_PWR8)
  mask32 = (vui32_t) vec_unpackl ( (vi32_t) mask32 );
#else
  const vui32_t q_zero = CONST_VINT128_W (0, 0, 0, 0);
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
  mask32= vec_mergeh (mask32, q_zero);
#else
  mask32 = vec_mergel (q_zero, mask32);
#endif
#endif
  return (vui64_t) mask32;
}

vui64_t
test_const64_f128_116 (void)
{
  return vec_const64_f128_116 ();
}

vui64_t
test_const64_f128_116_V1 (void)
{
  return vec_splat_u64 (116);
}

vui64_t
test_const64_f128_128 (void)
{
  return vec_const64_f128_128 ();
}

vui64_t
test_const64_f128_128_V1 (void)
{
  //  const vui32_t dw_128 = CONST_VINT128_W(0, 0, 0, 128);
  const vui32_t q_zero = CONST_VINT128_W (0, 0, 0, 0);
#if defined (_ARCH_PWR8)
  // Generate {64, 64} from count leading zeros of {0, 0}
  vui64_t dw64 = vec_clzd((vui64_t) q_zero);
  // Generate {128, 128}
  return vec_addudm (dw64, dw64);
#else
  const vui32_t q_ones = CONST_VINT128_W (-1, -1, -1, -1);
  vui32_t signmask;
  signmask = vec_sl (q_ones, q_ones);
  signmask = vec_sld (q_zero, signmask, 1);
  return vec_mrgald ((vui128_t) signmask, (vui128_t) signmask);
#endif
}

vui64_t
test_const64_f128naninf (void)
{
  return vec_const64_f128naninf ();
}

vui64_t
test_const64_f128maxe (void)
{
  return vec_const64_f128maxe ();
}

vui64_t
test_const64_f128maxe_V1 (void)
{
  const vui32_t q_ones = CONST_VINT128_W (-1, -1, -1, -1);
#if defined (_ARCH_PWR8)
  const vui32_t lbitmask = vec_splat_u32 (-2);
  vui32_t biasmask;
  biasmask = (vui32_t) vec_srdi ((vui64_t) q_ones, 49);
  biasmask = vec_and (biasmask, lbitmask);
  return (vui64_t) biasmask;
#else
  // const vui32_t biasmask = CONST_VINT128_W (0, 0x7fff, 0, 0x7fff);
  const vui32_t q_zero = CONST_VINT128_W (0, 0, 0, 0);
  vui32_t biasmask = vec_mergel (q_zero, q_ones);
  biasmask = vec_srwi (biasmask, 18);
  biasmask = vec_add (biasmask, biasmask);
  return (vui64_t) biasmask;
#endif
}

vui64_t
test_const64_f128maxe_V0 (void)
{
  const vui32_t q_ones = CONST_VINT128_W (-1, -1, -1, -1);
#if defined (_ARCH_PWR8)
  vui64_t biasmask;
  biasmask = vec_srdi ((vui64_t) q_ones, 50);
  biasmask = vec_add (biasmask, biasmask);
  return (vui64_t) biasmask;
#else
  // const vui32_t biasmask = CONST_VINT128_W (0, 0x7fff, 0, 0x7fff);
  const vui32_t q_zero = CONST_VINT128_W (0, 0, 0, 0);
  vui32_t biasmask = vec_mergel (q_zero, q_ones);
  biasmask = vec_srwi (biasmask, 18);
  biasmask = vec_add (biasmask, biasmask);
  return (vui64_t) biasmask;
#endif
}

vui64_t
test_const64_f128bias (void)
{
  return vec_const64_f128bias ();
}

vui64_t
test_const64_f128bias_127 (void)
{
  const vui64_t f128bias = vec_const64_f128bias ();
  const vui64_t v127_dw = vec_const64_f128_127 ();
  return vec_addudm (f128bias, v127_dw);
}

vui64_t
test_const64_f128bias_V0 (void)
{
  const vui32_t q_ones = CONST_VINT128_W (-1, -1, -1, -1);
  #if defined (_ARCH_PWR8)

  return vec_srdi ((vui64_t) q_ones, 50);
#else
  // const vui32_t biasmask = CONST_VINT128_W (0, 0x3fff, 0, 0x3fff);
  const vui32_t q_zero = CONST_VINT128_W (0, 0, 0, 0);
  vui32_t biasmask = vec_mergel (q_zero, q_ones);
  biasmask = vec_srwi (biasmask, 18);
  return (vui64_t) biasmask;
#endif
}

vui64_t
test_const64_f128_128_V0(void)
{
  //  const vui32_t dw_128 = CONST_VINT128_W(0, 16, 0, 128);
  const vui32_t q_zero = CONST_VINT128_W (0, 0, 0, 0);
#if defined (_ARCH_PWR8)
  // Generate {64, 64} from count leading zeros of {0, 0}
  vui64_t dw64 = vec_clzd((vui64_t) q_zero);
  // Generate {128, 128}
  return vec_addudm (dw64, dw64);
#else
  const vui32_t q_ones = CONST_VINT128_W (-1, -1, -1, -1);
  vui32_t signmask;
  signmask = vec_sl (q_ones, q_ones);
  signmask = vec_sld (q_zero, signmask, 1);
  return vec_mrgald ((vui128_t) signmask, (vui128_t) signmask);
#endif
}

vui32_t
test_const128_f128_128 (void)
{
  return vec_const128_f128_128 ();
}

vui32_t
test_const128_f128_128_V1 (void)
{
  return (vui32_t) vec_splat_u128 (128);
}

vui64_t
test_mask64_f128exp (void)
{
  return vec_mask64_f128exp ();
}

vui64_t
test_mask64_f128exp_V1 (void)
{
#if defined (_ARCH_PWR8)
  __VF_128 vunion;
  const vui64_t q_ones = (vui64_t) CONST_VINT64_DW (-1, -1);
  const vui8_t sh49 = vec_splat_u8 (-15);
  vui64_t shdw;
  vunion.vx16 = sh49;
  shdw = vunion.vx2;
  return vec_sr (q_ones, (vui64_t) sh49);
#else
  //const vui32_t expmask = CONST_VINT128_W (0, 0x7fff, 0, 0x7fff);
  const vui32_t q_zero = CONST_VINT128_W (0, 0, 0, 0);
  vui32_t expmask;
  expmask = (vui32_t) vec_splat_u8 (-8);
  expmask = vec_sld (q_zero, expmask, 4);
  return (vui64_t) vec_packpx (expmask, expmask);
#endif
}

vui64_t
test_mask64_f128exp_V0 (void)
{
#if defined (_ARCH_PWR8)
  const vui64_t q_ones = (vui64_t) CONST_VINT64_DW (-1, -1);
  const vui8_t sh49 = vec_splat_u8 (-15);
  return vec_sr (q_ones, (vui64_t) sh49);
#else
  //const vui32_t expmask = CONST_VINT128_W (0, 0x7fff, 0, 0x7fff);
  const vui32_t q_zero = CONST_VINT128_W (0, 0, 0, 0);
  vui32_t expmask;
  expmask = (vui32_t) vec_splat_u8 (-8);
  expmask = vec_sld (q_zero, expmask, 4);
  return (vui64_t) vec_packpx (expmask, expmask);
#endif
}

vui32_t
test_mask128_f128exp (void)
{
  return vec_mask128_f128exp ();
}

vui32_t
test_mask128_f128exp_v4(void)
{
  const vui32_t q_ones = CONST_VINT128_W (-1, -1, -1, -1);
  const vui32_t q_zero = CONST_VINT128_W (0, 0, 0, 0);
  const vui32_t expmask = vec_srwi (q_ones, 17);
  return vec_sld (expmask, q_zero, 14);
}

vui32_t
test_mask128_f128exp_v3(void)
{
  const vui32_t mag = vec_mask128_f128mag ();
  const vui32_t sig = vec_mask128_f128sig ();
  return vec_andc (mag, sig);
}

vui32_t
test_mask128_f128exp_V2 (void)
{
  //  const vui32_t expmask = CONST_VINT128_W (0x7fff0000, 0, 0, 0);
  const vui32_t q_zero = CONST_VINT128_W (0, 0, 0, 0);
  vui32_t expmask;
  // Genrate {0xf8f8f8f8, 0, 0, 0}
  expmask = (vui32_t) vec_splat_s8 (-8);
  expmask = vec_sld (expmask, q_zero, 12);
  // Pack to Pixel to generate {0x7fff0000, 0, 0, 0}
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
  return (vui32_t) vec_packpx (q_zero, expmask);
#else
  return (vui32_t) vec_packpx (expmask, q_zero);
#endif
}

vui32_t
test_mask128_f128exp_V1 (void)
{
  //  const vui32_t expmask = CONST_VINT128_W (0x7fff0000, 0, 0, 0);
  const vui32_t q_zero = CONST_VINT128_W (0, 0, 0, 0);
  const vui32_t q_ones = CONST_VINT128_W (-1, -1, -1, -1);
  vui32_t expmask;
  // Genrate {0x00ffffff, 0, 0, 0}
  expmask = vec_sld (q_ones, q_zero, 13);
  expmask = vec_sld (expmask, expmask, 15);
  // Pack to Pixel to generate {0x7fff0000, 0, 0, 0}
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
  return (vui32_t) vec_packpx (q_zero, expmask);
#else
  return (vui32_t) vec_packpx (expmask, q_zero);
#endif
}

vui32_t
test_mask128_f128exp_V0 (void)
{
  //  const vui32_t expmask = CONST_VINT128_W (0x7fff0000, 0, 0, 0);
  const vui32_t q_zero = CONST_VINT128_W (0, 0, 0, 0);
  const vui32_t q_ones = CONST_VINT128_W (-1, -1, -1, -1);

  vui32_t expmask;
  expmask = vec_sld (q_ones, q_zero, 14);
  expmask = (vui32_t) vec_srhi ((vui16_t) expmask, 1);
  return expmask;
}

vui32_t
test_mask128_f128bias_V0 (void)
{
  vui32_t biasmask;
#if defined (_ARCH_PWR8)
  const vui32_t q_zero = CONST_VINT128_W (0, 0, 0, 0);
  const vui32_t q_ones = CONST_VINT128_W (-1, -1, -1, -1);

  biasmask = vec_sld (q_ones, q_zero, 14);
  biasmask = (vui32_t) vec_srhi ((vui16_t) biasmask, 2);
#else
  biasmask = CONST_VINT128_W (0x3fff0000, 0, 0, 0);
#endif
  return biasmask;
}

vui32_t
test_mask128_f128mag (void)
{
  return vec_mask128_f128mag ();
}

vui32_t
test_mask128_f128mag_V0 (void)
{
  //  const vui32_t magmask = CONST_VINT128_W (0x7fffffff, -1, -1, -1);
  const vui32_t q_ones = CONST_VINT128_W (-1, -1, -1, -1);
  const vui8_t b_one = vec_splat_u8 (1);
  return (vui32_t) vec_srl ((vui8_t) q_ones, b_one);
}

vui32_t
test_mask128_f128sig (void)
{
  return vec_mask128_f128sig ();
}

vui32_t
test_mask128_f128sign(void)
{
  return vec_mask128_f128sign ();
}

vui32_t
test_mask128_f128sign_v2(void)
{
  const vui32_t mag = vec_mask128_f128mag ();
  return vec_nor (mag, mag);
}

vui32_t
test_mask128_f128sign_v1(void)
{
  const vui32_t mag = test_mask128_f128mag_V0 ();
  return vec_nor (mag, mag);
}

vui32_t
test_mask128_f128sign_V0(void)
{
  const vui32_t signmask = CONST_VINT128_W(0x80000000, 0, 0, 0);
  return signmask;
}

vui32_t
test_mask128_f128Xbits (void)
{
  return vec_mask128_f128Xbits ();
}

vui32_t
test_mask128_f128Xbits_V0 (void)
{
  const vui32_t q_ones = CONST_VINT128_W (-1, -1, -1, -1);

  vui32_t xbits;
  xbits = (vui32_t) vec_srqi ((vui128_t) q_ones, 3);
  return xbits;
}

vui32_t
test_mask128_f128Cbit (void)
{
  return vec_mask128_f128Cbit ();
}

vui32_t
test_mask128_f128Lbit (void)
{
  return vec_mask128_f128Lbit ();
}

vui32_t
test_mask128_f128Qbit (void)
{
  return vec_mask128_f128Qbit ();
}

vui32_t
test_mask128_CSE (void)
{
  vui32_t f128sig, f128mag, f128exp, f128Lbit, f128Sbit;

  f128Sbit = vec_mask128_f128sign ();
  f128Lbit = vec_mask128_f128Lbit ();
  f128sig  = vec_mask128_f128sig ();
  f128mag  = vec_mask128_f128mag ();
  f128exp  = vec_xor (f128sig, f128mag);
  f128sig  = vec_or (f128exp, f128Sbit);
  return (f128sig + f128Lbit);
}

__VEC_U_256
test_xsigqpmp (vui32_t a_mag, vui32_t b_mag, vui32_t x_exp)
{
  const vui32_t q_zero = CONST_VINT128_W (0, 0, 0, 0);
  vui32_t sigmask = vec_mask128_f128sig();
  vui32_t hidden = vec_mask128_f128Lbit();
  __VEC_U_256 result;
  vui32_t a_sig, b_sig;
  vui32_t a_norm, b_norm, x_norm;
  // Assume that the sign-bit is already masked off
  // Mask off the significands
  a_sig = vec_and (a_mag, sigmask);
  b_sig = vec_and (b_mag, sigmask);
  // Assume that exponents are already extracted and merged
  // Compare exponents for denormal, assume finite
  x_norm = (vui32_t) vec_cmpgt ((vui32_t) x_exp, q_zero);
  a_norm = vec_splat (x_norm, VEC_WE_1);
  b_norm = vec_splat (x_norm, VEC_WE_3);
  // For Normal QP insert (hidden) L-bit into significand
  a_sig =  vec_sel (a_sig, a_norm, hidden);
  b_sig =  vec_sel (b_sig, b_norm, hidden);

  result.vx0 = (vui128_t) a_sig;
  result.vx1 = (vui128_t) b_sig;
  return result;
}

vui128_t
test_xsigqpo_v2 (vui32_t a_mag, vui32_t b_mag, vui64_t x_exp)
{
  const vui32_t q_zero = CONST_VINT128_W (0, 0, 0, 0);
  const vui32_t q_ones = CONST_VINT128_W (-1, -1, -1, -1);
//  const vui32_t signmask = CONST_VINT128_W(0x80000000, 0, 0, 0);
//  const vui32_t sigmask = CONST_VINT128_W (0x0000ffff, -1, -1, -1);
  vui32_t sigmask = vec_sld (q_zero, q_ones, 14);
//  const vui32_t hidden = CONST_VINT128_W (0x00010000, 0, 0, 0);
  vui32_t hidden = vec_splat_u32 (1);
  hidden = vec_sld (hidden, q_zero, 14);
  vui128_t result;
  vui32_t a_sig, b_sig;
  vui32_t a_norm, b_norm, x_norm;
  // Assume that the sign-bit is already masked off
  // Mask off the significands
  a_sig = vec_and (a_mag, sigmask);
  b_sig = vec_and (b_mag, sigmask);
  // Assume that exponents are already extracted and merged
  // Compare exponents for denormal, assume finite
  x_norm = (vui32_t) vec_cmpgtud (x_exp, (vui64_t) q_zero);
  //a_norm = (vui32_t) vec_cmpgt (a_exp, zero);
  //b_norm = (vui32_t) vec_cmpgt (b_exp, zero);
  a_norm = (vui32_t) vec_mrgahd ((vui128_t) x_norm, (vui128_t) q_zero);
  b_norm = (vui32_t) vec_mrgald ((vui128_t) x_norm, (vui128_t) q_zero);
  // For Normal QP insert (hidden) L-bit into significand
  a_sig =  vec_sel (a_sig, a_norm, hidden);
  b_sig =  vec_sel (b_sig, b_norm, hidden);

  result = vec_adduqm ((vui128_t) a_sig, (vui128_t) b_sig);;
  return result;
}

#ifdef PVECLIB_OLDTESTVERSIONS
vui128_t
test_xsigqpo_v1 (__binary128 vfa, __binary128 vfb)
{
  const vui32_t zero = CONST_VINT128_W (0, 0, 0, 0);
  const vui32_t signmask = CONST_VINT128_W(0x80000000, 0, 0, 0);
  const vui32_t sigmask = CONST_VINT128_W (0x0000ffff, -1, -1, -1);
  const vui32_t hidden = CONST_VINT128_W (0x00010000, 0, 0, 0);
  vui128_t result;
  vui32_t a_exp, b_exp;
  vui32_t a_sig, b_sig;
  vui32_t a_norm, b_norm;
  vui32_t a_mag, b_mag;
  // Mask off sign bits so can use integers for magnitude compare.
  a_mag = vec_andc_bin128_2_vui32t (vfa, signmask);
  b_mag = vec_andc_bin128_2_vui32t (vfb, signmask);
  // Mask off the significands
  a_sig = vec_and (a_mag, sigmask);
  b_sig = vec_and (b_mag, sigmask);
  // Mask off the exponents
  a_exp = vec_andc (a_mag, sigmask);
  b_exp = vec_andc (b_mag, sigmask);
  // Compare exponents for denormal, assume finite
  a_norm = (vui32_t) vec_cmpgt (a_exp, zero);
  b_norm = (vui32_t) vec_cmpgt (b_exp, zero);
  // For Normal insert (hidden) L-bit
  a_sig =  vec_sel (a_sig, a_norm, hidden);
  b_sig =  vec_sel (b_sig, b_norm, hidden);

//  a_sig = vec_xsxsigqp (vfa);
//  b_sig = vec_xsxsigqp (vfb);
  result = vec_adduqm ((vui128_t) a_sig, (vui128_t) b_sig);;
  return result;
}
#endif

__VEC_U_256
test_xsigqpo_v0 (__binary128 vfa, __binary128 vfb)
{
  __VEC_U_256 result;
  vui128_t a_sig, b_sig;

  a_sig = vec_xsxsigqp (vfa);
  b_sig = vec_xsxsigqp (vfb);
  result.vx0 = a_sig;
  result.vx1 = b_sig;
  return result;
}

vui64_t
test_xexpqpp (__binary128 vfa, __binary128 vfb)
{
  vui64_t result;
  vui32_t tmp, rtmp;
  //const vui32_t expmask = CONST_VINT128_W (0, 0x7fff, 0, 0x7fff);
  // Reverse vfa/vfb because the left rotate will reverse them again
  tmp = (vui32_t) vec_mrgh_bin128_2_vui64t (vfb, vfa);
  rtmp = vec_sld (tmp, tmp, 10);
  result = (vui64_t) vec_and ((vui64_t) rtmp, vec_mask64_f128exp());
  return result;
}

#ifdef PVECLIB_OLDTESTVERSIONS
vui64_t
test_xexpqpp_V2 (__binary128 vfa, __binary128 vfb)
{
  vui64_t result;
  vui32_t tmp, rtmp;
//  const vui64_t expmask = (vui64_t) { 0x7fff, 0x7fff };
  const vui32_t expmask = CONST_VINT128_W (0, 0x7fff, 0, 0x7fff);

//  a_exp = (vui64_t) vec_xfer_bin128_2_vui32t (vfa);
//  b_exp = (vui64_t) vec_xfer_bin128_2_vui32t (vfb);
//  tmp = (vui32_t) vec_mrgahd ((vui128_t) a_exp, (vui128_t) b_exp);
  // Reverse vfa/vfb because the left rotate will reverse them again
  tmp = (vui32_t) vec_mrgh_bin128_2_vui64t (vfb, vfa);
  rtmp = vec_sld (tmp, tmp, 10);
  result = (vui64_t) vec_and ( rtmp, expmask);
  return result;
}

vui64_t
test_xexpqpp_V1 (__binary128 vfa, __binary128 vfb)
{
  vui64_t result;
  vui64_t a_exp, b_exp;
  vui32_t tmp;
  const vui32_t expmask = CONST_VINT128_W (0x7fff0000, 0, 0, 0);

  a_exp = (vui64_t) vec_and_bin128_2_vui32t (vfa, expmask);
  b_exp = (vui64_t) vec_and_bin128_2_vui32t (vfb, expmask);
//  tmp = (vui32_t) vec_mrgahd ((vui128_t) a_exp, (vui128_t) b_exp);
  tmp = (vui32_t) vec_mrgahd ((vui128_t) b_exp, (vui128_t) a_exp);
  result = (vui64_t) vec_sld (tmp, tmp, 10);
  return result;
}
#endif

vui64_t
test_xexpqpp_v0 (__binary128 vfa, __binary128 vfb)
{
  vui64_t result;
  vui64_t a_exp, b_exp;

  a_exp = vec_xsxexpqp (vfa);
  b_exp = vec_xsxexpqp (vfb);
  result = vec_mrgahd ((vui128_t) a_exp, (vui128_t) b_exp);
  return result;
}

__binary128
test_genqpo_v0 (__binary128 vfa, __binary128 vfb)
{
  __binary128 result;
  vui64_t a_exp, b_exp, x_exp;
  vui128_t q_sig, a_sig, b_sig;
  vui32_t q_sign,  a_sign,  b_sign;
  const vui32_t signmask = CONST_VINT128_W(0x80000000, 0, 0, 0);

//  a_mag = (vui128_t) vec_andc_bin128_2_vui32t (vfa, signmask);
//  b_mag = (vui128_t) vec_andc_bin128_2_vui32t (vfb, signmask);
  a_exp = vec_xsxexpqp (vfa);
  a_sig = vec_xsxsigqp (vfa);
  a_sign = vec_and_bin128_2_vui32t (vfa, signmask);
  b_exp = vec_xsxexpqp (vfb);
  b_sig = vec_xsxsigqp (vfb);
  b_sign = vec_and_bin128_2_vui32t (vfb, signmask);
  x_exp = vec_mrgahd ((vui128_t) a_exp, (vui128_t) b_exp);

//  q_exp = vec_addudm (a_exp, b_exp);
  q_sig = vec_adduqm (a_sig, b_sig);
  q_sign = vec_xor (a_sign, b_sign);
  // Merge sign, significand, and exponent into final result
  q_sig = (vui128_t) vec_or ((vui32_t) q_sig, q_sign);
  result = vec_xsiexpqp (q_sig, x_exp);
  return result;
}

__binary128
test_vec_addqpo (__binary128 vfa, __binary128 vfb)
{
  __binary128 result;
#if defined (_ARCH_PWR9) && (__GNUC__ > 7)
#if defined (__FLOAT128__) && (__GNUC__ > 8)
  // earlier GCC versions generate extra data moves for this.
  result = __builtin_addf128_round_to_odd (vfa, vfb);
#else
  // No extra data moves here.
  __asm__(
      "xsaddqpo %0,%1,%2"
      : "=v" (result)
      : "v" (vfa), "v" (vfb)
      : );
#endif
  return result;
#elif  defined (_ARCH_PWR7)
  vui64_t q_exp, a_exp, b_exp, x_exp;
  vui128_t q_sig, a_sig, b_sig, p_tmp, p_odd;
  vui128_t a_mag, b_mag;
  vui32_t q_sign,  a_sign,  b_sign;
  vb128_t a_lt_b;
  const vui32_t q_zero = CONST_VINT128_W (0, 0, 0, 0);
  const vui32_t q_ones = CONST_VINT128_W (-1, -1, -1, -1);
  const vui32_t magmask = vec_mask128_f128mag();
  const vui64_t exp_naninf = vec_mask64_f128exp();
  // Vector extract the exponents from vfa, vfb
  x_exp = vec_xxxexpqpp (vfa, vfb);
  // Mask off sign bits so can use integers for magnitude compare.
  a_mag = (vui128_t) vec_and_bin128_2_vui32t (vfa, magmask);
  b_mag = (vui128_t) vec_and_bin128_2_vui32t (vfb, magmask);
  a_sign = vec_andc_bin128_2_vui32t (vfa, magmask);
  b_sign = vec_andc_bin128_2_vui32t (vfb, magmask);
//  if (vec_all_isfinitef128 (vfa) && vec_all_isfinitef128 (vfb))
//  The above can be optimized to the following
  if (__builtin_expect (vec_cmpud_all_lt (x_exp, exp_naninf), 1))
    {
      const vui128_t xbitmask = vec_splat_u128 (1);
      const vui128_t grx_mask = vec_splat_u128 (7);
      const vui64_t exp_min = vec_splat_u64 (1);
      const vui8_t t_sig_L = vec_splat_u8 (7);
      const vui8_t t_sig_C = vec_splat_u8 (15);
      const vui64_t exp_one = exp_min;
      const vui64_t exp_dnrm = (vui64_t) q_zero;
      vui128_t add_sig, sub_sig;
      vui128_t s_sig, x_bits;
      vui32_t diff_sign;
      vui32_t sigmask = vec_mask128_f128sig();
      vui32_t hidden = vec_mask128_f128Lbit();
      vui32_t a_norm, b_norm, x_norm;
      vui32_t a_s32, b_s32;

      // Extract the significand
      // Assume that the sign-bit is already masked off
      // Mask off the significands
      a_s32 = vec_and ((vui32_t) a_mag, sigmask);
      b_s32 = vec_and ((vui32_t) b_mag, sigmask);
      // Assume that exponents are already extracted and merged
      // Compare exponents for denormal, assume finite
      x_norm = (vui32_t) vec_cmpgt ((vui32_t) x_exp, q_zero);
      a_norm = vec_splat (x_norm, VEC_WE_1);
      b_norm = vec_splat (x_norm, VEC_WE_3);
      // For Normal QP insert (hidden) L-bit into significand
      a_sig =  (vui128_t) vec_sel (a_s32, a_norm, hidden);
      b_sig =  (vui128_t) vec_sel (b_s32, b_norm, hidden);
      // Correct exponent for zeros or denormals to E_min
      // will force 0 exponents for zero/denormal results later
      //exp_mask = vec_cmpequd (x_exp, exp_dnrm);
      x_exp = vec_selud ( exp_min, x_exp, (vb64_t) x_norm);
      // Generation sign difference for signed 0.0
      q_sign = vec_xor (a_sign, b_sign);
      // Precondition the significands before add so the GRX bits
      // are in the least significant 3 bit.
      a_sig = vec_slqi (a_sig, 3);
      b_sig = vec_slqi (b_sig, 3);

      // If sign(vfa) != sign(vfb) will need to:
      // 1) Subtract instead of add significands
      // 2) Generate signed zeros
      diff_sign = (vui32_t) vec_setb_sq ((vi128_t) q_sign);
      // If magnitude(b) >  magnitude(a) will need to swap a/b, later
      a_lt_b = vec_cmpltuq (a_mag, b_mag);

      // Now swap operands a/b if necessary so a has greater magnitude.
      {
	vui128_t a_tmp = a_sig;
	vui128_t b_tmp = b_sig;
	vui64_t x_tmp = vec_swapd (x_exp);

	q_sign = vec_sel (a_sign, b_sign, (vui32_t) a_lt_b);

	x_exp = vec_selud (x_exp, x_tmp, (vb64_t) a_lt_b);
	a_exp = vec_splatd (x_exp, VEC_DW_H);
	b_exp = vec_splatd (x_exp, VEC_DW_L);
	q_exp = a_exp;

	a_sig = vec_seluq (a_tmp, b_tmp, (vb128_t) a_lt_b);
	b_sig = vec_seluq (b_tmp, a_tmp, (vb128_t) a_lt_b);
      }
      // At this point we can assume that:
      // The magnitude (vfa) >= magnitude (vfb)
      // 1) Exponents (a_exp, b_exp) in the range E_min -> E_max
      // 2) a_exp >= b_exp
      // 2a) If a_exp == b_exp then a_sig >= b_sig
      // 2b) If a_exp > b_exp then
      //     shift (b_sig) right by (a_exp - b_exp)
      //     any bits shifted out of b_sig are ORed into the X-bit
      if (vec_cmpud_all_lt (b_exp, a_exp))
	{
	  vui64_t d_exp, l_exp;
	  vui128_t t_sig;
	  vb128_t exp_mask;
	  const vui64_t exp_128 = vec_const64_f128_128();

	  d_exp = vec_subudm (a_exp, b_exp);
	  exp_mask = (vb128_t) vec_cmpltud (d_exp, exp_128);
	  l_exp = vec_subudm (exp_128, d_exp);
	  t_sig = vec_srq (b_sig, (vui128_t) d_exp);
	  x_bits = vec_slq (b_sig, (vui128_t) l_exp);
	  t_sig = vec_seluq ((vui128_t) q_zero, t_sig, exp_mask);
	  x_bits = vec_seluq (b_sig, x_bits, exp_mask);
	  p_odd = vec_addcuq (x_bits, (vui128_t) q_ones);
	  b_sig = (vui128_t) vec_or ((vui32_t) t_sig, (vui32_t) p_odd);
	}

      // If operands have the same sign then s_sig = a_sig + b_sig
      // Otherwise s_sig = a_sig - b_sig
      add_sig = vec_adduqm (a_sig, b_sig);
      sub_sig = vec_subuqm (a_sig, b_sig);
      s_sig = vec_seluq (add_sig, sub_sig, (vb128_t) diff_sign);

      if (__builtin_expect (vec_cmpuq_all_eq (s_sig, (vui128_t) q_zero), 0))
	{ // Special case of both zero with different sign
	  q_sign = vec_sel (a_sign, (vui32_t) q_zero, diff_sign);
	  return vec_xfer_vui32t_2_bin128 (q_sign);
	}

      // Issolate CL bits from significand too simplify the compare
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
      vui8_t t_sig = vec_splat ((vui8_t) s_sig, 14);
#else
      vui8_t t_sig = vec_splat ((vui8_t) s_sig, 1);
#endif
//      if (vec_cmpuq_all_gt (s_sig, (vui128_t) sigov))
	if (vec_all_gt (t_sig, t_sig_C))
	{ // Check for carry and adjust
	  p_odd = (vui128_t) vec_and ((vui32_t) s_sig, (vui32_t) xbitmask);
	  s_sig = vec_srqi (s_sig, 1);
	  s_sig = (vui128_t) vec_or ((vui32_t) s_sig, (vui32_t) p_odd);
	  q_exp = vec_addudm (q_exp, exp_one);
	}
      else // if (vec_cmpuq_all_le (s_sig, (vui128_t) sigovt))
	  if (vec_all_le (t_sig, t_sig_L))
	{
	  // Or the significand is below normal range.
	  // This can happen with subtraction.
	  vui64_t c_exp, d_exp;
	  vui128_t c_sig;
	  const vui64_t exp_12 = vec_splat_u64 (12);

	  c_sig = vec_clzq (s_sig);
	  c_exp = vec_splatd ((vui64_t) c_sig, VEC_DW_L);
	  // The IR has 12 leading zeros that should not effect the shift count.
	  c_exp = vec_subudm (c_exp, exp_12);
	  d_exp = vec_subudm (q_exp, (vui64_t) exp_min);
	  d_exp = vec_minud (c_exp, d_exp);
	  {
	    vb64_t nrm_mask = vec_cmpgtsd ((vi64_t) q_exp, (vi64_t) exp_min);
	    vb64_t exp_mask = vec_cmpgtud (q_exp, c_exp);

	    c_sig = vec_slq (s_sig, (vui128_t) d_exp);
	    q_exp = vec_subudm (q_exp, d_exp);
	    exp_mask = (vb64_t) vec_and ((vui32_t) exp_mask, (vui32_t) nrm_mask);
	    q_exp = vec_selud (exp_dnrm,  q_exp, exp_mask);
	    s_sig = vec_seluq (s_sig, c_sig, (vb128_t) nrm_mask);
	  }
	}
      // Round to odd from low order GRX-bits
      p_tmp = (vui128_t) vec_and ((vui32_t) s_sig, (vui32_t) grx_mask);
      p_odd = vec_addcuq (p_tmp, (vui128_t) q_ones);
      q_sig = vec_srqi (s_sig, 3);
      q_sig = (vui128_t) vec_or ((vui32_t) q_sig, (vui32_t) p_odd);
      // Check for exponent overflow -> __FLT128_MAX__
      if (__builtin_expect ((vec_cmpud_all_ge ( q_exp, exp_naninf)), 0))
	{
	  // return maximum finite exponent and significand
	  // const vui32_t f128_max = CONST_VINT128_W(0x7ffeffff, -1, -1, -1);
	  const vui32_t f128_max = vec_const128_f128_fmax ();
	  vui32_t f128_smax = vec_or ((vui32_t) f128_max, q_sign);
	  return vec_xfer_vui32t_2_bin128 (f128_smax);
	}
      // Merge sign, significand, and exponent into final result
      q_sig = (vui128_t) vec_or ((vui32_t) q_sig, q_sign);
      result = vec_xsiexpqp (q_sig, q_exp);
      return result;
    }
  else // One or both operands are NaN or Infinity
    {
      //const vui32_t q_nan = CONST_VINT128_W(0x00008000, 0, 0, 0);
      vui32_t q_nan = vec_mask128_f128Qbit ();
      // One or both operands are NaN
      if (vec_all_isnanf128 (vfa))
	{
	  // vfa is NaN, Convert vfa to QNaN and return
	  vui32_t vf128 = vec_or_bin128_2_vui32t (vfa, q_nan);
	  return vec_xfer_vui32t_2_bin128 (vf128);
	}
      else if (vec_all_isnanf128 (vfb))
	{
	  // vfb is NaN, Convert vfb to QNaN and return
	  vui32_t vf128 = vec_or_bin128_2_vui32t (vfb, q_nan);
	  return vec_xfer_vui32t_2_bin128 (vf128);
	}
      else  // Or one or both operands are Infinity
	{
	  a_exp = vec_splatd (x_exp, VEC_DW_H);
	  // b_exp = vec_splatd (x_exp, VEC_DW_L);
	  if (vec_cmpud_all_eq (x_exp, exp_naninf)
	      && vec_cmpud_any_ne ((vui64_t) a_sign, (vui64_t) b_sign))
	    { // Both operands infinity and opposite sign
	      // Inifinty + Infinity (opposite sign) is Default Quiet NaN
	      return vec_const_nanf128 ();
	    }
	  else
	    { // Either both operands infinity and same sign
	      // Or one infinity and one finite
	      if (vec_cmpud_any_eq (a_exp, exp_naninf))
		{
		  // return infinity
		  return vfa;
		}
	      else
		{
		  // return infinity
		  return vfb;
		}
	    }
	}
    }
#else // ! _ARCH_PWR8, use libgcc soft-float
  return (vfa + vfb);
#endif
  return result;
}

#ifdef PVECLIB_OLDTESTVERSIONS
__binary128
test_vec_addqpo_V5 (__binary128 vfa, __binary128 vfb)
{
  __binary128 result;
#if defined (_ARCH_PWR9) && (__GNUC__ > 7)
#if defined (__FLOAT128__) && (__GNUC__ > 8)
  // earlier GCC versions generate extra data moves for this.
  result = __builtin_addf128_round_to_odd (vfa, vfb);
#else
  // No extra data moves here.
  __asm__(
      "xsaddqpo %0,%1,%2"
      : "=v" (result)
      : "v" (vfa), "v" (vfb)
      : );
#endif
#elif  defined (_ARCH_PWR7)
  vui64_t q_exp, a_exp, b_exp, x_exp;
  vui64_t exp_naninf;
  vui128_t q_sig, a_sig, b_sig, p_tmp, p_odd;
  vui128_t a_mag, b_mag;
  vui32_t q_sign,  a_sign,  b_sign;
  vb128_t a_lt_b;
#if 1
  const vui32_t q_zero = CONST_VINT128_W (0, 0, 0, 0);
  const vui32_t q_ones = CONST_VINT128_W (-1, -1, -1, -1);
#else
  const vui32_t signmask = CONST_VINT128_W(0x80000000, 0, 0, 0);
  const vui64_t q_zero = { 0, 0 };
  const vui64_t q_ones = { -1, -1 };
#endif
//  const vui64_t exp_dnrm = (vui64_t) CONST_VINT64_DW( 0, 0 );
//  const vui64_t exp_naninf = (vui64_t) CONST_VINT64_DW( 0x7fff, 0x7fff );
//  const vui64_t exp_max = (vui64_t) CONST_VINT64_DW( 0x7ffe, 0x7ffe );
  // Let the endian swap happen, its ok. using endian sensitive splatd.
  // const vui64_t exp_naninf_max = (vui64_t) { 0x7fff, 0x7ffe };
#if 0
  a_exp = vec_xsxexpqp (vfa);
  b_exp = vec_xsxexpqp (vfb);
  x_exp = vec_mrgahd ((vui128_t) a_exp, (vui128_t) b_exp);
#else
  x_exp = vec_xxxexpqpp (vfa, vfb);
#endif
  // Mask off sign bits so can use integers for magnitude compare.
#if 1
  vui32_t magmask = vec_mask128_f128mag();
  a_mag = (vui128_t) vec_and_bin128_2_vui32t (vfa, magmask);
  b_mag = (vui128_t) vec_and_bin128_2_vui32t (vfb, magmask);
  a_sign = vec_andc_bin128_2_vui32t (vfa, magmask);
  b_sign = vec_andc_bin128_2_vui32t (vfb, magmask);
#else
  vui32_t signmask = vec_mask128_f128sign();
  a_mag = (vui128_t) vec_andc_bin128_2_vui32t (vfa, signmask);
  b_mag = (vui128_t) vec_andc_bin128_2_vui32t (vfb, signmask);
  a_sign = vec_and_bin128_2_vui32t (vfa, signmask);
  b_sign = vec_and_bin128_2_vui32t (vfb, signmask);
#endif
#if 0
  a_sig = vec_xsxsigqp (vfa);
  b_sig = vec_xsxsigqp (vfb);
#endif
#if 0
  exp_naninf = vec_splatd (exp_naninf_max, 0);
  exp_max = vec_splatd (exp_naninf_max, 1);
#else
  exp_naninf = vec_mask64_f128exp();
#endif
//  if (vec_all_isfinitef128 (vfa) && vec_all_isfinitef128 (vfb))
//  The above can be optimized to the following
  if (__builtin_expect (vec_cmpud_all_lt (x_exp, exp_naninf), 1))
    {
#if 0
      const vui32_t xbitmask = CONST_VINT128_W(0, 0, 0, 1);
      const vui32_t sigov = CONST_VINT128_W(0x000fffff, -1, -1, -1);
      const vui32_t sigovt = CONST_VINT128_W(0x0007ffff, -1, -1, -1);
      const vui64_t exp_min = (vui64_t) CONST_VINT64_DW( 1, 1 );
      const vui32_t grx_mask = CONST_VINT128_W(0, 0, 0, 7);
#else
#if 0
      vui32_t xbitmask = (vui32_t) vec_mrgald ((vui128_t) q_zero, (vui128_t) exp_min);
#else
#if 0
      vui32_t xbitmask = vec_splat_u32 (1);
      xbitmask = vec_sld (q_zero, xbitmask, 12);
      vui32_t grx_mask = vec_splat_u32(7);
      grx_mask = vec_sld (q_zero, grx_mask, 4);
#else
      const vui128_t xbitmask = vec_splat_u128 (1);
      const vui128_t grx_mask = vec_splat_u128 (7);
#endif
#endif
      const vui64_t exp_min = vec_splat_u64 (1);
      const vui8_t t_sig_L = vec_splat_u8 (7);
      const vui8_t t_sig_C = vec_splat_u8 (15);
#endif
      const vui64_t exp_one = exp_min;
      const vui64_t exp_dnrm = (vui64_t) q_zero;
      vui128_t add_sig, sub_sig;
      vui128_t s_sig, x_bits;
      // vb64_t exp_mask;
      vui32_t diff_sign;
#if 0
      a_sig = vec_xsxsigqp (vfa);
      b_sig = vec_xsxsigqp (vfb);
#else
      {
	vui32_t sigmask = vec_mask128_f128sig();
	vui32_t hidden = vec_mask128_f128Lbit();
	vui32_t a_norm, b_norm, x_norm;
	vui32_t a_s32, b_s32;
	// Assume that the sign-bit is already masked off
	// Mask off the significands
	a_s32 = vec_and ((vui32_t) a_mag, sigmask);
	b_s32 = vec_and ((vui32_t) b_mag, sigmask);
	// Assume that exponents are already extracted and merged
	// Compare exponents for denormal, assume finite
	x_norm = (vui32_t) vec_cmpgt ((vui32_t) x_exp, q_zero);
	a_norm = vec_splat (x_norm, VEC_WE_1);
	b_norm = vec_splat (x_norm, VEC_WE_3);
	// For Normal QP insert (hidden) L-bit into significand
	a_sig =  (vui128_t) vec_sel (a_s32, a_norm, hidden);
	b_sig =  (vui128_t) vec_sel (b_s32, b_norm, hidden);
	// Correct exponent for zeros or denormals to E_min
	// will force 0 exponents for zero/denormal results later
	//exp_mask = vec_cmpequd (x_exp, exp_dnrm);
	x_exp = vec_selud ( exp_min, x_exp, (vb64_t) x_norm);
      }
#endif

      q_sign = vec_xor (a_sign, b_sign);
//	  a_exp = vec_splatd (x_exp, VEC_DW_H);
//	  b_exp = vec_splatd (x_exp, VEC_DW_L);

      // Precondition the significands before add so the GRX bits
      // are in the least significant 3 bit.
      a_sig = vec_slqi (a_sig, 3);
      b_sig = vec_slqi (b_sig, 3);

      // If sign(vfa) != sign(vfb) will need to:
      // 1) Subtract instead of add significands
      // 2) Generate signed zeros
      diff_sign = (vui32_t) vec_setb_sq ((vi128_t) q_sign);
      // If magnitude(b) >  magnitude(a) will need to swap a/b, later
      a_lt_b = vec_cmpltuq (a_mag, b_mag);
#if 0
      // Correct exponent for zeros or denormals to E_min
      // will force 0 exponents for zero/denormal results later
      exp_mask = vec_cmpequd (x_exp, exp_dnrm);
      x_exp = vec_selud ( x_exp, exp_min, exp_mask);
#endif

      // Now swap operands a/b if necessary so a has greater magnitude.
	{
	  vui128_t a_tmp = a_sig;
	  vui128_t b_tmp = b_sig;
	  vui64_t x_tmp = vec_swapd (x_exp);

	  q_sign = vec_sel (a_sign, b_sign, (vui32_t) a_lt_b);

	  x_exp = vec_selud (x_exp, x_tmp, (vb64_t) a_lt_b);
	  a_exp = vec_splatd (x_exp, VEC_DW_H);
	  b_exp = vec_splatd (x_exp, VEC_DW_L);
	  q_exp = a_exp;

	  a_sig = vec_seluq (a_tmp, b_tmp, (vb128_t) a_lt_b);
	  b_sig = vec_seluq (b_tmp, a_tmp, (vb128_t) a_lt_b);
	}
      // At this point we can assume that:
      // The magnitude (vfa) >= magnitude (vfb)
      // 1) Exponents (a_exp, b_exp) in the range E_min -> E_max
      // 2) a_exp >= b_exp
      // 2a) If a_exp == b_exp then a_sig >= b_sig
      // 2b) If a_exp > b_exp then
      //     shift (b_sig) right by (a_exp - b_exp)
      //     any bits shifted out of b_sig are ORed into the X-bit
      if (vec_cmpud_all_lt (b_exp, a_exp))
	{
	  vui64_t d_exp, l_exp;
	  vui128_t t_sig;
#if 1
	  const vui64_t exp_128 = vec_const64_f128_128();
#else
	  const vui64_t exp_128 = (vui64_t) { 128, 128 };
#endif

	  d_exp = vec_subudm (a_exp, b_exp);
#if 1
	  vb128_t exp_mask = (vb128_t) vec_cmpltud (d_exp, exp_128);
	  l_exp = vec_subudm (exp_128, d_exp);
	  t_sig = vec_srq (b_sig, (vui128_t) d_exp);
	  x_bits = vec_slq (b_sig, (vui128_t) l_exp);
	  t_sig = vec_seluq ((vui128_t) q_zero, t_sig, exp_mask);
	  x_bits = vec_seluq (b_sig, x_bits, exp_mask);
#else
	  if (vec_cmpud_all_lt (d_exp, exp_128))
	    {
	      l_exp = vec_subudm (exp_128, d_exp);
	      t_sig = vec_srq (b_sig, (vui128_t) d_exp);
	      x_bits = vec_slq (b_sig, (vui128_t) l_exp);
	    }
	  else
	    {
	      x_bits = b_sig;
	      t_sig = (vui128_t) q_zero;
	    }
#endif
	  p_odd = vec_addcuq (x_bits, (vui128_t) q_ones);
	  b_sig = (vui128_t) vec_or ((vui32_t) t_sig, (vui32_t) p_odd);
	}

      // If operands have the same sign then s_sig = a_sig + b_sig
      // Otherwise s_sig = a_sig - b_sig
      add_sig = vec_adduqm (a_sig, b_sig);
      sub_sig = vec_subuqm (a_sig, b_sig);
      s_sig = vec_seluq (add_sig, sub_sig, (vb128_t) diff_sign);

      if (__builtin_expect (vec_cmpuq_all_eq (s_sig, (vui128_t) q_zero), 0))
	{ // Special case of both zero with different sign
	  q_sign = vec_sel (a_sign, (vui32_t) q_zero, diff_sign);
	  return vec_xfer_vui32t_2_bin128 (q_sign);
	}

      // Issolate CL bits from significand too simplify the compare
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
      vui8_t t_sig = vec_splat ((vui8_t) s_sig, 14);
#else
      vui8_t t_sig = vec_splat ((vui8_t) s_sig, 1);
#endif
//      if (vec_cmpuq_all_gt (s_sig, (vui128_t) sigov))
	if (vec_all_gt (t_sig, t_sig_C))
	{ // Check for carry and adjust
	  p_odd = (vui128_t) vec_and ((vui32_t) s_sig, (vui32_t) xbitmask);
	  s_sig = vec_srqi (s_sig, 1);
	  s_sig = (vui128_t) vec_or ((vui32_t) s_sig, (vui32_t) p_odd);
	  q_exp = vec_addudm (q_exp, exp_one);
	}
      else // if (vec_cmpuq_all_le (s_sig, (vui128_t) sigovt))
	  if (vec_all_le (t_sig, t_sig_L))
	{
	  // Or the significand is below normal range.
	  // This can happen with subtraction.
	  vui64_t c_exp, d_exp;
	  vui128_t c_sig;
#if 1
	  // const vui64_t exp_12 = { 12, 12 };
	  const vui64_t exp_12 = vec_splat_u64 (12);
	  c_sig = vec_clzq (s_sig);
	  c_exp = vec_splatd ((vui64_t) c_sig, VEC_DW_L);
	  // The IR has 12 leading zeros that should not effect the shift count.
	  c_exp = vec_subudm (c_exp, exp_12);
#else
	  // The IR has 12 leading zeros that should not effect the shift count.
	  c_sig = vec_slqi (s_sig, 12);
	  c_sig = vec_clzq (c_sig);
	  c_exp = vec_splatd ((vui64_t) c_sig, VEC_DW_L);
#endif
	  d_exp = vec_subudm (q_exp, (vui64_t) exp_min);
	  d_exp = vec_minud (c_exp, d_exp);
#if 0
	  if (vec_cmpsd_all_gt ((vi64_t) q_exp, (vi64_t) exp_min))
	    {
	      vb64_t exp_mask = vec_cmpgtud (q_exp, c_exp);

	      s_sig = vec_slq (s_sig, (vui128_t) d_exp);
	      q_exp = vec_subudm (q_exp, d_exp);
	      q_exp = vec_selud (exp_dnrm,  q_exp, exp_mask);
	    }
	  else
	    {
	      // Exponent is less than or equal to E_min
	      // so return denormal result.
	      q_exp = exp_dnrm;
	    }
#else
	  {
	    vb64_t nrm_mask = vec_cmpgtsd ((vi64_t) q_exp, (vi64_t) exp_min);
	    vb64_t exp_mask = vec_cmpgtud (q_exp, c_exp);

	    c_sig = vec_slq (s_sig, (vui128_t) d_exp);
	    q_exp = vec_subudm (q_exp, d_exp);
#if 0
	    q_exp = vec_selud (exp_dnrm,  q_exp, exp_mask);
	    q_exp = vec_selud (exp_dnrm,  q_exp, nrm_mask);
#else
	    exp_mask = (vb64_t) vec_and ((vui32_t) exp_mask, (vui32_t) nrm_mask);
	    q_exp = vec_selud (exp_dnrm,  q_exp, exp_mask);
#endif
	    s_sig = vec_seluq (s_sig, c_sig, (vb128_t) nrm_mask);
	  }
#endif
	}
      // Round to odd from low order GRX-bits
#if 1
      p_tmp = (vui128_t) vec_and ((vui32_t) s_sig, (vui32_t) grx_mask);
#else
      p_tmp = vec_slqi (s_sig, 125);
#endif
      p_odd = vec_addcuq (p_tmp, (vui128_t) q_ones);
      q_sig = vec_srqi (s_sig, 3);
      q_sig = (vui128_t) vec_or ((vui32_t) q_sig, (vui32_t) p_odd);

//      exp_max = vec_splatd (exp_naninf_max, 1);
      // Check for exponent overflow -> __FLT128_MAX__
#if 1
      if (__builtin_expect ((vec_cmpud_all_ge ( q_exp, exp_naninf)), 0))
#else
      //if  (vec_cmpud_all_gt ( q_exp, exp_max))
      if (__builtin_expect ((vec_cmpud_all_gt (q_exp, exp_max)), 0))
#endif
	{
	  // return maximum finite exponent and significand
	  const vui32_t f128_max = CONST_VINT128_W(0x7ffeffff, -1, -1, -1);
	  vui32_t f128_smax = vec_or ((vui32_t) f128_max, q_sign);
	  return vec_xfer_vui32t_2_bin128 (f128_smax);
	}
    }
  else // One or both operands are NaN or Infinity
    {
      //const vui32_t q_nan = CONST_VINT128_W(0x00008000, 0, 0, 0);
      vui32_t q_nan = vec_mask128_f128Qbit ();
      // One or both operands are NaN
      if (vec_all_isnanf128 (vfa))
	{
	  // vfa is NaN, Convert vfa to QNaN and return
#if 0
	  q_sign = a_sign;
	  q_sig = (vui128_t) vec_or ((vui32_t) a_sig, q_nan);
	  q_exp = a_exp;
#else
	  vui32_t vf128 = vec_or_bin128_2_vui32t (vfa, q_nan);
	  return vec_xfer_vui32t_2_bin128 (vf128);
#endif
	}
      else if (vec_all_isnanf128 (vfb))
	{
	  // vfb is NaN, Convert vfb to QNaN and return
#if 0
	  q_sign = b_sign;
	  q_sig = (vui128_t) vec_or ((vui32_t) b_sig, q_nan);
	  q_exp = b_exp;
#else
	  vui32_t vf128 = vec_or_bin128_2_vui32t (vfb, q_nan);
	  return vec_xfer_vui32t_2_bin128 (vf128);
#endif
	}
      else  // Or one or both operands are Infinity
	{
	  a_exp = vec_splatd (x_exp, VEC_DW_H);
	  // b_exp = vec_splatd (x_exp, VEC_DW_L);
	  if (vec_cmpud_all_eq (x_exp, exp_naninf)
	      && vec_cmpud_any_ne ((vui64_t) a_sign, (vui64_t) b_sign))
	    { // Both operands infinity and opposite sign
	      // Inifinty + Infinity (opposite sign) is Default Quiet NaN
	      return vec_const_nanf128 ();
	    }
	  else
	    { // Either both operands infinity and same sign
	      // Or one infinity and one finite
	      if (vec_cmpud_any_eq (a_exp, exp_naninf))
		{
		  // return infinity
		  return vfa;
		}
	      else
		{
		  // return infinity
		  return vfb;
		}
	    }
	}
    }
  // Merge sign, significand, and exponent into final result
  q_sig = (vui128_t) vec_or ((vui32_t) q_sig, q_sign);
  result = vec_xsiexpqp (q_sig, q_exp);
#else // ! _ARCH_PWR8, use libgcc soft-float
  result = vfa + vfb;
#endif
  return result;
}

#ifndef __clang__
  __binary128
  test_vec_addqpo_V4 (__binary128 vfa, __binary128 vfb)
  {
    __binary128 result;
  #if defined (_ARCH_PWR9) && (__GNUC__ > 7)
  #if defined (__FLOAT128__) && (__GNUC__ > 8)
    // earlier GCC versions generate extra data moves for this.
    result = __builtin_addf128_round_to_odd (vfa, vfb);
  #else
    // No extra data moves here.
    __asm__(
        "xsaddqpo %0,%1,%2"
        : "=v" (result)
        : "v" (vfa), "v" (vfb)
        : );
  #endif
  #elif  defined (_ARCH_PWR7)
    vui64_t q_exp, a_exp, b_exp, x_exp;
    vui64_t exp_naninf;
    vui128_t q_sig, a_sig, b_sig, p_tmp, p_odd;
    vui32_t q_sign,  a_sign,  b_sign;
    vb128_t a_lt_b;
    const vui32_t signmask = CONST_VINT128_W(0x80000000, 0, 0, 0);
    const vui64_t q_zero = { 0, 0 };
    const vui64_t q_ones = { -1, -1 };
  //  const vui64_t exp_dnrm = (vui64_t) CONST_VINT64_DW( 0, 0 );
  //  const vui64_t exp_naninf = (vui64_t) CONST_VINT64_DW( 0x7fff, 0x7fff );
  //  const vui64_t exp_max = (vui64_t) CONST_VINT64_DW( 0x7ffe, 0x7ffe );
    // Let the endian swap happen, its ok. using endian sensitive splatd.
    const vui64_t exp_naninf_max = (vui64_t) { 0x7fff, 0x7ffe };

    a_exp = vec_xsxexpqp (vfa);
    a_sign = vec_and_bin128_2_vui32t (vfa, signmask);
    b_exp = vec_xsxexpqp (vfb);
    b_sign = vec_and_bin128_2_vui32t (vfb, signmask);
  #if 0
    a_sig = vec_xsxsigqp (vfa);
    b_sig = vec_xsxsigqp (vfb);
  #endif
    x_exp = vec_mrgahd ((vui128_t) a_exp, (vui128_t) b_exp);
    exp_naninf = vec_splatd (exp_naninf_max, 0);
  //  if (vec_all_isfinitef128 (vfa) && vec_all_isfinitef128 (vfb))
  //  The above can be optimized to the following
    if (__builtin_expect (vec_cmpud_all_lt (x_exp, exp_naninf), 1))
      {
  #if 0
        const vui32_t xbitmask = CONST_VINT128_W(0, 0, 0, 1);
        const vui32_t sigov = CONST_VINT128_W(0x000fffff, -1, -1, -1);
        const vui32_t sigovt = CONST_VINT128_W(0x0007ffff, -1, -1, -1);
        const vui64_t exp_min = (vui64_t) CONST_VINT64_DW( 1, 1 );
  #else
        const vui32_t grx_mask = CONST_VINT128_W(0, 0, 0, 7);
        const vui64_t exp_min = vec_splat_u64 (1);
        const vui8_t t_sig_L = vec_splat_u8 (7);
        const vui8_t t_sig_C = vec_splat_u8 (15);
        vui32_t xbitmask = (vui32_t) vec_mrgald ((vui128_t) q_zero, (vui128_t) exp_min);
  #endif
        const vui64_t exp_one = exp_min;
        const vui64_t exp_dnrm = (vui64_t) q_zero;
        vui128_t add_sig, sub_sig;
        vui128_t a_mag, b_mag;
        vui128_t s_sig, x_bits;
        vb64_t exp_mask;
        vui32_t diff_sign;
  #if 1
    a_sig = vec_xsxsigqp (vfa);
    b_sig = vec_xsxsigqp (vfb);
  #endif

        q_sign = vec_xor (a_sign, b_sign);

        // Mask off sign bits so can use integers for magnitude compare.
        a_mag = (vui128_t) vec_andc_bin128_2_vui32t (vfa, signmask);
        b_mag = (vui128_t) vec_andc_bin128_2_vui32t (vfb, signmask);
        // Precondition the significands before add so the GRX bits
        // are in the least significant 3 bit.
        a_sig = vec_slqi (a_sig, 3);
        b_sig = vec_slqi (b_sig, 3);

        // If sign(vfa) != sign(vfb) will need to:
        // 1) Subtract instead of add significands
        // 2) Generate signed zeros
        diff_sign = (vui32_t) vec_setb_sq ((vi128_t) q_sign);
        // If magnitude(b) >  magnitude(a) will need to swap a/b, later
        a_lt_b = vec_cmpltuq (a_mag, b_mag);

        // Correct exponent for zeros or denormals to E_min
        // will force 0 exponents for zero/denormal results later
        exp_mask = vec_cmpequd (x_exp, exp_dnrm);
        x_exp = vec_selud ( x_exp, exp_min, exp_mask);

        // Now swap operands a/b if necessary so a has greater magnitude.
  	{
  	  vui128_t a_tmp = a_sig;
  	  vui128_t b_tmp = b_sig;
  	  vui64_t x_tmp = vec_swapd (x_exp);

  	  q_sign = vec_sel (a_sign, b_sign, (vui32_t) a_lt_b);

  	  x_exp = vec_selud (x_exp, x_tmp, (vb64_t) a_lt_b);
  	  a_exp = vec_splatd (x_exp, VEC_DW_H);
  	  b_exp = vec_splatd (x_exp, VEC_DW_L);
  	  q_exp = a_exp;

  	  a_sig = vec_seluq (a_tmp, b_tmp, (vb128_t) a_lt_b);
  	  b_sig = vec_seluq (b_tmp, a_tmp, (vb128_t) a_lt_b);
  	}
        // At this point we can assume that:
        // The magnitude (vfa) >= magnitude (vfb)
        // 1) Exponents (a_exp, b_exp) in the range E_min -> E_max
        // 2) a_exp >= b_exp
        // 2a) If a_exp == b_exp then a_sig >= b_sig
        // 2b) If a_exp > b_exp then
        //     shift (b_sig) right by (a_exp - b_exp)
        //     any bits shifted out of b_sig are ORed into the X-bit
        if (vec_cmpud_all_lt (b_exp, a_exp))
  	{
  	  vui64_t d_exp, l_exp;
  	  vui128_t t_sig;
  	  const vui64_t exp_128 = (vui64_t) { 128, 128 };

  	  d_exp = vec_subudm (a_exp, b_exp);
  #if 1
  	  vb128_t exp_mask = (vb128_t) vec_cmpltud (d_exp, exp_128);
  	  l_exp = vec_subudm (exp_128, d_exp);
  	  t_sig = vec_srq (b_sig, (vui128_t) d_exp);
  	  x_bits = vec_slq (b_sig, (vui128_t) l_exp);
  	  t_sig = vec_seluq ((vui128_t) q_zero, t_sig, exp_mask);
  	  x_bits = vec_seluq (b_sig, x_bits, exp_mask);
  #else
  	  if (vec_cmpud_all_lt (d_exp, exp_128))
  	    {
  	      l_exp = vec_subudm (exp_128, d_exp);
  	      t_sig = vec_srq (b_sig, (vui128_t) d_exp);
  	      x_bits = vec_slq (b_sig, (vui128_t) l_exp);
  	    }
  	  else
  	    {
  	      x_bits = b_sig;
  	      t_sig = (vui128_t) q_zero;
  	    }
  #endif
  	  p_odd = vec_addcuq (x_bits, (vui128_t) q_ones);
  	  b_sig = (vui128_t) vec_or ((vui32_t) t_sig, (vui32_t) p_odd);
  	}

        // If operands have the same sign then s_sig = a_sig + b_sig
        // Otherwise s_sig = a_sig - b_sig
        add_sig = vec_adduqm (a_sig, b_sig);
        sub_sig = vec_subuqm (a_sig, b_sig);
        s_sig = vec_seluq (add_sig, sub_sig, (vb128_t) diff_sign);

        if (__builtin_expect (vec_cmpuq_all_eq (s_sig, (vui128_t) q_zero), 0))
  	{ // Special case of both zero with different sign
  	  q_sign = vec_sel (a_sign, (vui32_t) q_zero, diff_sign);
  	  return vec_xfer_vui32t_2_bin128 (q_sign);
  	}

        // Issolate CL bits from significand too simplify the compare
  #if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
        vui8_t t_sig = vec_splat ((vui8_t) s_sig, 14);
  #else
        vui8_t t_sig = vec_splat ((vui8_t) s_sig, 1);
  #endif
  //      if (vec_cmpuq_all_gt (s_sig, (vui128_t) sigov))
  	if (vec_all_gt (t_sig, t_sig_C))
  	{ // Check for carry and adjust
  	  p_odd = (vui128_t) vec_and ((vui32_t) s_sig, xbitmask);
  	  s_sig = vec_srqi (s_sig, 1);
  	  s_sig = (vui128_t) vec_or ((vui32_t) s_sig, (vui32_t) p_odd);
  	  q_exp = vec_addudm (q_exp, exp_one);
  	}
        else // if (vec_cmpuq_all_le (s_sig, (vui128_t) sigovt))
  	  if (vec_all_le (t_sig, t_sig_L))
  	{
  	  // Or the significand is below normal range.
  	  // This can happen with subtraction.
  	  vui64_t c_exp, d_exp;
  	  vui128_t c_sig;
  #if 1
  	  // const vui64_t exp_12 = { 12, 12 };
  	  const vui64_t exp_12 = vec_splat_u64 (12);
  	  c_sig = vec_clzq (s_sig);
  	  c_exp = vec_splatd ((vui64_t) c_sig, VEC_DW_L);
  	  // The IR has 12 leading zeros that should not effect the shift count.
  	  c_exp = vec_subudm (c_exp, exp_12);
  #else
  	  // The IR has 12 leading zeros that should not effect the shift count.
  	  c_sig = vec_slqi (s_sig, 12);
  	  c_sig = vec_clzq (c_sig);
  	  c_exp = vec_splatd ((vui64_t) c_sig, VEC_DW_L);
  #endif
  	  d_exp = vec_subudm (q_exp, (vui64_t) exp_min);
  	  d_exp = vec_minud (c_exp, d_exp);
  #if 0
  	  if (vec_cmpsd_all_gt ((vi64_t) q_exp, (vi64_t) exp_min))
  	    {
  	      vb64_t exp_mask = vec_cmpgtud (q_exp, c_exp);

  	      s_sig = vec_slq (s_sig, (vui128_t) d_exp);
  	      q_exp = vec_subudm (q_exp, d_exp);
  	      q_exp = vec_selud (exp_dnrm,  q_exp, exp_mask);
  	    }
  	  else
  	    {
  	      // Exponent is less than or equal to E_min
  	      // so return denormal result.
  	      q_exp = exp_dnrm;
  	    }
  #else
  	  {
  	    vb64_t nrm_mask = vec_cmpgtsd ((vi64_t) q_exp, (vi64_t) exp_min);
  	    vb64_t exp_mask = vec_cmpgtud (q_exp, c_exp);

  	    c_sig = vec_slq (s_sig, (vui128_t) d_exp);
  	    q_exp = vec_subudm (q_exp, d_exp);
  #if 0
  	    q_exp = vec_selud (exp_dnrm,  q_exp, exp_mask);
  	    q_exp = vec_selud (exp_dnrm,  q_exp, nrm_mask);
  #else
  	    exp_mask = (vb64_t) vec_and ((vui32_t) exp_mask, (vui32_t) nrm_mask);
  	    q_exp = vec_selud (exp_dnrm,  q_exp, exp_mask);
  #endif
  	    s_sig = vec_seluq (s_sig, c_sig, (vb128_t) nrm_mask);
  	  }
  #endif
  	}
        // Round to odd from low order GRX-bits
  #if 1
        p_tmp = (vui128_t) vec_and ((vui32_t) s_sig, grx_mask);
  #else
        p_tmp = vec_slqi (s_sig, 125);
  #endif
        p_odd = vec_addcuq (p_tmp, (vui128_t) q_ones);
        q_sig = vec_srqi (s_sig, 3);
        q_sig = (vui128_t) vec_or ((vui32_t) q_sig, (vui32_t) p_odd);

  //      exp_max = vec_splatd (exp_naninf_max, 1);
        // Check for exponent overflow -> __FLT128_MAX__
  #if 1
        if (__builtin_expect ((vec_cmpud_all_ge ( q_exp, exp_naninf)), 0))
  #else
        //if  (vec_cmpud_all_gt ( q_exp, exp_max))
        if (__builtin_expect ((vec_cmpud_all_gt (q_exp, exp_max)), 0))
  #endif
  	{
  	  // return maximum finite exponent and significand
  	  const vui32_t f128_max = CONST_VINT128_W(0x7ffeffff, -1, -1, -1);
  	  vui32_t f128_smax = vec_or ((vui32_t) f128_max, q_sign);
  	  return vec_xfer_vui32t_2_bin128 (f128_smax);
  	}
      }
    else
      { // One or both operands are NaN or Infinity
  	{
  	  // One or both operands are NaN
  	  const vui32_t q_nan = CONST_VINT128_W(0x00008000, 0, 0, 0);
  	  if (vec_all_isnanf128 (vfa))
  	    {
  	      // vfa is NaN, Convert vfb to QNaN and return
  #if 0
  	      q_sign = a_sign;
  	      q_sig = (vui128_t) vec_or ((vui32_t) a_sig, q_nan);
  	      q_exp = a_exp;
  #else
  	      vui32_t vf128 = vec_or_bin128_2_vui32t (vfa, q_nan);
  	      return vec_xfer_vui32t_2_bin128 (vf128);
  #endif
  	    }
  	  else if (vec_all_isnanf128 (vfb))
  	    {
  	      // vfb is NaN, Convert vfb to QNaN and return
  #if 0
  	      q_sign = b_sign;
  	      q_sig = (vui128_t) vec_or ((vui32_t) b_sig, q_nan);
  	      q_exp = b_exp;
  #else
  	      vui32_t vf128 = vec_or_bin128_2_vui32t (vfb, q_nan);
  	      return vec_xfer_vui32t_2_bin128 (vf128);
  #endif
  	    }
  	  else  // Or one or both operands are Infinity
  	    {
  //	      a_exp = vec_splatd (a_exp, VEC_DW_H);
  //	      b_exp = vec_splatd (b_exp, VEC_DW_H);
  	      if (vec_cmpud_all_eq (x_exp, exp_naninf)
  		  && vec_cmpud_any_ne ((vui64_t) a_sign, (vui64_t) b_sign))
  		{ // Both operands infinity and opposite sign
  		  // Inifinty + Infinity (opposite sign) is Default Quiet NaN
  		  return vec_const_nanf128 ();
  		}
  	      else
  		{ // Either both operands infinity and same sign
  		  // Or one infinity and one finite
  		  if (vec_cmpud_any_eq (a_exp, exp_naninf))
  		    {
  		      // return infinity
  		      return vfa;
  		    }
  		  else
  		    {
  		      // return infinity
  		      return vfb;
  		    }
  		}
  	    }
  	}
      }
    // Merge sign, significand, and exponent into final result
    q_sig = (vui128_t) vec_or ((vui32_t) q_sig, q_sign);
    result = vec_xsiexpqp (q_sig, q_exp);
  #else // ! _ARCH_PWR8, use libgcc soft-float
    result = vfa + vfb;
  #endif
    return result;
  }

__binary128
test_vec_addqpo_V3 (__binary128 vfa, __binary128 vfb)
{
  __binary128 result;
#if defined (_ARCH_PWR9) && (__GNUC__ > 7)
#if defined (__FLOAT128__) && (__GNUC__ > 8)
  // earlier GCC versions generate extra data moves for this.
  result = __builtin_addf128_round_to_odd (vfa, vfb);
#else
  // No extra data moves here.
  __asm__(
      "xsaddqpo %0,%1,%2"
      : "=v" (result)
      : "v" (vfa), "v" (vfb)
      : );
#endif
#elif  defined (_ARCH_PWR7)
  vui64_t q_exp, a_exp, b_exp, x_exp;
  vui64_t exp_naninf, exp_max;
  vui128_t q_sig, a_sig, b_sig, p_tmp, p_odd;
  vui128_t a_mag, b_mag;
  vui128_t s_sig, x_bits;
  vui32_t q_sign,  a_sign,  b_sign;
  vb128_t a_lt_b;
  const vui32_t signmask = CONST_VINT128_W(0x80000000, 0, 0, 0);
  const vui64_t q_zero = { 0, 0 };
  const vui64_t q_ones = { -1, -1 };
  const vui64_t exp_min = (vui64_t) CONST_VINT64_DW( 1, 1 );
  const vui64_t exp_dnrm = (vui64_t) CONST_VINT64_DW( 0, 0 );
//  const vui64_t q_expnaninf = (vui64_t) CONST_VINT64_DW( 0x7fff, 0x7fff );
//  const vui64_t q_expmax = (vui64_t) CONST_VINT64_DW( 0x7ffe, 0x7ffe );
  // Let the endian swap happen, its ok. using endian sensitive splatd.
  const vui64_t exp_naninf_max = (vui64_t) { 0x7fff, 0x7ffe };
  const vui32_t sigov = CONST_VINT128_W(0x000fffff, -1, -1, -1);
  const vui32_t sigovt = CONST_VINT128_W(0x0007ffff, -1, -1, -1);
  const vui32_t xbitmask = CONST_VINT128_W(0, 0, 0, 1);

  a_exp = vec_xsxexpqp (vfa);
  a_sig = vec_xsxsigqp (vfa);
  a_sign = vec_and_bin128_2_vui32t (vfa, signmask);
  b_exp = vec_xsxexpqp (vfb);
  b_sig = vec_xsxsigqp (vfb);
  b_sign = vec_and_bin128_2_vui32t (vfb, signmask);
  x_exp = vec_mrgahd ((vui128_t) a_exp, (vui128_t) b_exp);
  exp_naninf = vec_splatd (exp_naninf_max, 0);
  exp_max = vec_splatd (exp_naninf_max, 1);
//  if (vec_all_isfinitef128 (vfa) && vec_all_isfinitef128 (vfb))
//  The above can be optimized to the following
  if (__builtin_expect (vec_cmpud_all_lt (x_exp, exp_naninf), 1))
    {
      const vui64_t q_one = exp_min;
      vui128_t add_sig, sub_sig;
      vb64_t exp_mask;
      vui32_t diff_sign;

      q_sign = vec_xor (a_sign, b_sign);

      // Mask off sign bits so can use integers for magnitude compare.
      a_mag = (vui128_t) vec_andc_bin128_2_vui32t (vfa, signmask);
      b_mag = (vui128_t) vec_andc_bin128_2_vui32t (vfb, signmask);
      // Precondition the significands before add so the GRX bits
      // are in the least significant 3 bit.
      a_sig = vec_slqi (a_sig, 3);
      b_sig = vec_slqi (b_sig, 3);

      // If sign(vfa) != sign(vfb) will need to:
      // 1) Subtract instead of add significands
      // 2) Generate signed zeros
      diff_sign = (vui32_t) vec_setb_sq ((vi128_t) q_sign);
      // If magnitude(b) >  magnitude(a) will need to swap a/b, later
      a_lt_b = vec_cmpltuq (a_mag, b_mag);

      // Correct exponent for zeros or denormals to E_min
      // will force 0 exponents for zero/denormal results later
      exp_mask = vec_cmpequd (x_exp, exp_dnrm);
      x_exp = vec_selud ( x_exp, exp_min, exp_mask);

      // Now swap operands a/b if necessary so a has greater magnitude.
	{
	  vui128_t a_tmp = a_sig;
	  vui128_t b_tmp = b_sig;
	  vui64_t x_tmp = vec_swapd (x_exp);

	  q_sign = vec_sel (a_sign, b_sign, (vui32_t) a_lt_b);

	  x_exp = vec_selud (x_exp, x_tmp, (vb64_t) a_lt_b);
	  a_exp = vec_splatd (x_exp, VEC_DW_H);
	  b_exp = vec_splatd (x_exp, VEC_DW_L);
	  q_exp = a_exp;

	  a_sig = vec_seluq (a_tmp, b_tmp, (vb128_t) a_lt_b);
	  b_sig = vec_seluq (b_tmp, a_tmp, (vb128_t) a_lt_b);
	}
      // At this point we can assume that:
      // The magnitude (vfa) >= magnitude (vfb)
      // 1) Exponents (a_exp, b_exp) in the range E_min -> E_max
      // 2) a_exp >= b_exp
      // 2a) If a_exp == b_exp then a_sig >= b_sig
      // 2b) If a_exp > b_exp then
      //     shift (b_sig) right by (a_exp - b_exp)
      //     any bits shifted out of b_sig are ORed into the X-bit
      if (vec_cmpud_all_lt (b_exp, a_exp))
	{
	  vui64_t d_exp, l_exp;
	  vui128_t t_sig;
	  const vui64_t exp_128 = (vui64_t) CONST_VINT64_DW( 128, 128 );

	  d_exp = vec_subudm (a_exp, b_exp);
	  if (vec_cmpud_all_lt (d_exp, exp_128))
	    {
	      l_exp = vec_subudm (exp_128, d_exp);
	      t_sig = vec_srq (b_sig, (vui128_t) d_exp);
	      x_bits = vec_slq (b_sig, (vui128_t) l_exp);
	    }
	  else
	    {
	      x_bits = b_sig;
	      t_sig = (vui128_t) q_zero;
	    }

	  p_odd = vec_addcuq (x_bits, (vui128_t) q_ones);
	  b_sig = (vui128_t) vec_or ((vui32_t) t_sig, (vui32_t) p_odd);
	}

      // If operands have the same sign then s_sig = a_sig + b_sig
      // Otherwise s_sig = a_sig - b_sig
      add_sig = vec_adduqm (a_sig, b_sig);
      sub_sig = vec_subuqm (a_sig, b_sig);
      s_sig = vec_seluq (add_sig, sub_sig, (vb128_t) diff_sign);

      if (vec_cmpuq_all_eq (s_sig, (vui128_t) q_zero))
	{ // Special case of both zero with different sign
	  q_sign = vec_sel (a_sign, (vui32_t) q_zero, diff_sign);
	  return vec_xfer_vui32t_2_bin128 (q_sign);
	}
      else if (vec_cmpuq_all_gt (s_sig, (vui128_t) sigov))
	{ // Check for carry and adjust
	  p_odd = (vui128_t) vec_and ((vui32_t) s_sig, xbitmask);
	  s_sig = vec_srqi (s_sig, 1);
	  s_sig = (vui128_t) vec_or ((vui32_t) s_sig, (vui32_t) p_odd);
	  q_exp = vec_addudm (q_exp, q_one);
	}
      else if (vec_cmpuq_all_le (s_sig, (vui128_t) sigovt))
	{
	  // Or the significand is below normal range.
	  // This can happen with subtraction.
	  const vui64_t exp_12 = { 12, 12 };
	  vui64_t c_exp, d_exp;
	  vui128_t c_sig;

	  c_sig = vec_clzq (s_sig);
	  c_exp = vec_splatd ((vui64_t) c_sig, VEC_DW_L);
	  c_exp = vec_subudm (c_exp, exp_12);
	  d_exp = vec_subudm (q_exp, (vui64_t) exp_min);
	  d_exp = vec_minud (c_exp, d_exp);

	  if (vec_cmpsd_all_gt ((vi64_t) q_exp, (vi64_t) exp_min))
	    {
	      vb64_t exp_mask = vec_cmpgtud (q_exp, c_exp);

	      s_sig = vec_slq (s_sig, (vui128_t) d_exp);
	      q_exp = vec_subudm (q_exp, d_exp);
	      q_exp = vec_selud (q_zero,  q_exp, exp_mask);
	    }
	  else
	    {
	      // Exponent is less than or equal to E_min
	      // so return denormal result.
	      q_exp = q_zero;
	    }
	}
      // Round to odd from low order GRX-bits
      p_tmp = vec_slqi (s_sig, 125);
      p_odd = vec_addcuq (p_tmp, (vui128_t) q_ones);
      q_sig = vec_srqi (s_sig, 3);
      q_sig = (vui128_t) vec_or ((vui32_t) q_sig, (vui32_t) p_odd);

//      exp_max = vec_splatd (exp_naninf_max, 1);
      // Check for exponent overflow -> __FLT128_MAX__
      if (vec_cmpud_all_gt (q_exp, exp_max))
	{
	  // return maximum finite exponent and significand
	  q_exp = exp_max;
	  q_sig = (vui128_t) sigov;
	}
    }
  else
    { // One or both operands are NaN or Infinity
	{
	  // One or both operands are NaN
	  const vui32_t q_nan = CONST_VINT128_W(0x00008000, 0, 0, 0);
	  if (vec_all_isnanf128 (vfa))
	    {
	      // vfa is NaN
	      q_sign = a_sign;
	      q_sig = (vui128_t) vec_or ((vui32_t) a_sig, q_nan);
	      q_exp = a_exp;
	    }
	  else if (vec_all_isnanf128 (vfb))
	    {
	      // vfb is NaN
	      q_sign = b_sign;
	      q_sig = (vui128_t) vec_or ((vui32_t) b_sig, q_nan);
	      q_exp = b_exp;
	    }
	  else  // Or one or both operands are Infinity
	    {
//	      a_exp = vec_splatd (a_exp, VEC_DW_H);
//	      b_exp = vec_splatd (b_exp, VEC_DW_H);
	      if (vec_cmpud_all_eq (x_exp, exp_naninf)
		  && vec_cmpud_any_ne ((vui64_t) a_sign, (vui64_t) b_sign))
		{ // Both operands infinity and opposite sign
		  // Inifinty + Infinity (opposite sign) is Default Quiet NaN
		  return vec_const_nanf128 ();
		}
	      else
		{ // Either both operands infinity and same sign
		  // Or one infinity and one finite
		  if (vec_cmpud_any_eq (a_exp, exp_naninf))
		    {
		      // return infinity
		      return vfa;
		    }
		  else
		    {
		      // return infinity
		      return vfb;
		    }
		}
	    }
	}
    }
  // Merge sign, significand, and exponent into final result
  q_sig = (vui128_t) vec_or ((vui32_t) q_sig, q_sign);
  result = vec_xsiexpqp (q_sig, q_exp);
#else // ! _ARCH_PWR8, use libgcc soft-float
  result = vfa + vfb;
#endif
  return result;
}

__binary128
test_vec_addqpo_V2 (__binary128 vfa, __binary128 vfb)
{
  __binary128 result;
#if defined (_ARCH_PWR9) && (__GNUC__ > 7)
#if defined (__FLOAT128__) && (__GNUC__ > 8)
  // earlier GCC versions generate extra data moves for this.
  result = __builtin_addf128_round_to_odd (vfa, vfb);
#else
  // No extra data moves here.
  __asm__(
      "xsaddqpo %0,%1,%2"
      : "=v" (result)
      : "v" (vfa), "v" (vfb)
      : );
#endif
#elif  defined (_ARCH_PWR7)
  vui64_t q_exp, a_exp, b_exp, x_exp;
  vui128_t q_sig, a_sig, b_sig, p_tmp, p_odd;
  vui128_t a_mag, b_mag;
  vui128_t s_sig, x_bits;
  vui32_t q_sign,  a_sign,  b_sign;
  vb128_t a_lt_b;
  const vui32_t signmask = CONST_VINT128_W(0x80000000, 0, 0, 0);
  const vui64_t q_zero = { 0, 0 };
  const vui64_t q_ones = { -1, -1 };
  const vui64_t exp_min = (vui64_t) CONST_VINT64_DW( 1, 1 );
  const vui64_t exp_dnrm = (vui64_t) CONST_VINT64_DW( 0, 0 );
  const vui64_t q_expnaninf = (vui64_t) CONST_VINT64_DW( 0x7fff, 0x7fff );
  const vui64_t q_expmax = (vui64_t) CONST_VINT64_DW( 0x7ffe, 0x7ffe );
  const vui32_t sigov = CONST_VINT128_W(0x000fffff, -1, -1, -1);
  const vui32_t sigovt = CONST_VINT128_W(0x0007ffff, -1, -1, -1);
  const vui32_t xbitmask = CONST_VINT128_W(0, 0, 0, 1);

  a_exp = vec_xsxexpqp (vfa);
  a_sig = vec_xsxsigqp (vfa);
  a_sign = vec_and_bin128_2_vui32t (vfa, signmask);
  b_exp = vec_xsxexpqp (vfb);
  b_sig = vec_xsxsigqp (vfb);
  b_sign = vec_and_bin128_2_vui32t (vfb, signmask);
  x_exp = vec_mrgahd ((vui128_t) a_exp, (vui128_t) b_exp);

//  if (vec_all_isfinitef128 (vfa) && vec_all_isfinitef128 (vfb))
//  The above can be optimized to the following
  if (__builtin_expect (vec_cmpud_all_lt (x_exp, q_expnaninf), 1))
    {
      const vui64_t q_one = { 1, 1 };
      vui128_t add_sig, sub_sig;
      vb64_t exp_mask;
      vui32_t diff_sign;

      q_sign = vec_xor (a_sign, b_sign);

      // Mask off sign bits so can use integers for magnitude compare.
      a_mag = (vui128_t) vec_andc_bin128_2_vui32t (vfa, signmask);
      b_mag = (vui128_t) vec_andc_bin128_2_vui32t (vfb, signmask);
      // Precondition the significands before add so the GRX bits
      // are in the least significant 3 bit.
      a_sig = vec_slqi (a_sig, 3);
      b_sig = vec_slqi (b_sig, 3);

      // If sign(vfa) != sign(vfb) will need to:
      // 1) Subtract instead of add significands
      // 2) Generate signed zeros
      diff_sign = (vui32_t) vec_setb_sq ((vi128_t) q_sign);
      // If magnitude(b) >  magnitude(a) will need to swap a/b, later
      a_lt_b = vec_cmpltuq (a_mag, b_mag);

      // Correct exponent for zeros or denormals to E_min
      // will force 0 exponents for zero/denormal results later
      exp_mask = vec_cmpequd (x_exp, exp_dnrm);
      x_exp = (vui64_t) vec_sel ((vui32_t) x_exp, (vui32_t) exp_min, (vui32_t) exp_mask);

      // Now swap operands a/b if necessary so a has greater magnitude.
	{
	  vui128_t a_tmp = a_sig;
	  vui128_t b_tmp = b_sig;
	  vui64_t x_tmp = vec_swapd (x_exp);

	  q_sign = vec_sel (a_sign, b_sign, (vui32_t) a_lt_b);

	  x_exp = vec_sel (x_exp, x_tmp, (vui64_t) a_lt_b);
	  a_exp = vec_splatd (x_exp, VEC_DW_H);
	  b_exp = vec_splatd (x_exp, VEC_DW_L);
	  q_exp = a_exp;

	  a_sig = vec_seluq (a_tmp, b_tmp, (vb128_t) a_lt_b);
	  b_sig = vec_seluq (b_tmp, a_tmp, (vb128_t) a_lt_b);
	}
      // At this point we can assume that:
      // The magnitude (vfa) >= magnitude (vfb)
      // 1) Exponents (a_exp, b_exp) in the range E_min -> E_max
      // 2) a_exp >= b_exp
      // 2a) If a_exp == b_exp then a_sig >= b_sig
      // 2b) If a_exp > b_exp then
      //     shift (b_sig) right by (a_exp - b_exp)
      //     any bits shifted out of b_sig are ORed into the X-bit
      if (vec_cmpud_all_lt (b_exp, a_exp))
	{
	  vui64_t d_exp, l_exp;
	  vui128_t t_sig;
	  const vui64_t exp_128 = (vui64_t) CONST_VINT64_DW( 128, 128 );

	  d_exp = vec_subudm (a_exp, b_exp);
	  if (vec_cmpud_all_lt (d_exp, exp_128))
	    {
	      l_exp = vec_subudm (exp_128, d_exp);
	      t_sig = vec_srq (b_sig, (vui128_t) d_exp);
	      x_bits = vec_slq (b_sig, (vui128_t) l_exp);
	    }
	  else
	    {
	      x_bits = b_sig;
	      t_sig = (vui128_t) q_zero;
	    }

	  p_odd = vec_addcuq (x_bits, (vui128_t) q_ones);
	  b_sig = (vui128_t) vec_or ((vui32_t) t_sig, (vui32_t) p_odd);
	}

      // If operands have the same sign then s_sig = a_sig + b_sig
      // Otherwise s_sig = a_sig - b_sig
      add_sig = vec_adduqm (a_sig, b_sig);
      sub_sig = vec_subuqm (a_sig, b_sig);
      s_sig = vec_seluq (add_sig, sub_sig, (vb128_t) diff_sign);

      if (vec_cmpuq_all_eq (s_sig, (vui128_t) q_zero))
	{ // Special case of both zero with different sign
	  q_sign = vec_sel (a_sign, (vui32_t) q_zero, diff_sign);
	  return vec_xfer_vui32t_2_bin128 (q_sign);
	}
      else if (vec_cmpuq_all_gt (s_sig, (vui128_t) sigov))
	{ // Check for carry and adjust
	  p_odd = (vui128_t) vec_and ((vui32_t) s_sig, xbitmask);
	  s_sig = vec_srqi (s_sig, 1);
	  s_sig = (vui128_t) vec_or ((vui32_t) s_sig, (vui32_t) p_odd);
	  q_exp = vec_addudm (q_exp, q_one);
	}
      else if (vec_cmpuq_all_le (s_sig, (vui128_t) sigovt))
	{
	  // Or the significand is below normal range.
	  // This can happen with subtraction.
	  const vui64_t exp_12 = { 12, 12 };
	  vui64_t c_exp, d_exp;
	  vui128_t c_sig;

	  c_sig = vec_clzq (s_sig);
	  c_exp = vec_splatd ((vui64_t) c_sig, VEC_DW_L);
	  c_exp = vec_subudm (c_exp, exp_12);
	  d_exp = vec_subudm (q_exp, (vui64_t) exp_min);
	  d_exp = vec_minud (c_exp, d_exp);

	  if (vec_cmpsd_all_gt ((vi64_t) q_exp, (vi64_t) exp_min))
	    {
	      vb64_t exp_mask = vec_cmpgtud (q_exp, c_exp);

	      s_sig = vec_slq (s_sig, (vui128_t) d_exp);
	      q_exp = vec_subudm (q_exp, d_exp);
	      q_exp = (vui64_t) vec_sel (q_zero, (vui64_t) q_exp, exp_mask);
	    }
	  else
	    {
	      // Exponent is less than or equal to E_min
	      // so return denormal result.
	      q_exp = q_zero;
	    }
	}
      // Round to odd from low order GRX-bits
      p_tmp = vec_slqi (s_sig, 125);
      p_odd = vec_addcuq (p_tmp, (vui128_t) q_ones);
      q_sig = vec_srqi (s_sig, 3);
      q_sig = (vui128_t) vec_or ((vui32_t) q_sig, (vui32_t) p_odd);

      // Check for exponent overflow -> __FLT128_MAX__
      if (vec_cmpud_all_gt (q_exp, q_expmax))
	{
	  // return maximum finite exponent and significand
	  q_exp = q_expmax;
	  q_sig = (vui128_t) sigov;
	}
    }
  else
    { // One or both operands are NaN or Infinity
	{
	  // One or both operands are NaN
	  const vui32_t q_nan = CONST_VINT128_W(0x00008000, 0, 0, 0);
	  if (vec_all_isnanf128 (vfa))
	    {
	      // vfa is NaN
	      q_sign = a_sign;
	      q_sig = (vui128_t) vec_or ((vui32_t) a_sig, q_nan);
	      q_exp = a_exp;
	    }
	  else if (vec_all_isnanf128 (vfb))
	    {
	      // vfb is NaN
	      q_sign = b_sign;
	      q_sig = (vui128_t) vec_or ((vui32_t) b_sig, q_nan);
	      q_exp = b_exp;
	    }
	  else  // Or one or both operands are Infinity
	    {
//	      a_exp = vec_splatd (a_exp, VEC_DW_H);
//	      b_exp = vec_splatd (b_exp, VEC_DW_H);
	      if (vec_cmpud_all_eq (x_exp, q_expnaninf)
		  && vec_cmpud_any_ne ((vui64_t) a_sign, (vui64_t) b_sign))
		{ // Both operands infinity and opposite sign
		  // Inifinty + Infinity (opposite sign) is Default Quiet NaN
		  return vec_const_nanf128 ();
		}
	      else
		{ // Either both operands infinity and same sign
		  // Or one infinity and one finite
		  if (vec_cmpud_any_eq (a_exp, q_expnaninf))
		    {
		      // return infinity
		      return vfa;
		    }
		  else
		    {
		      // return infinity
		      return vfb;
		    }
		}
	    }
	}
    }
  // Merge sign, significand, and exponent into final result
  q_sig = (vui128_t) vec_or ((vui32_t) q_sig, q_sign);
  result = vec_xsiexpqp (q_sig, q_exp);
#else // ! _ARCH_PWR8, use libgcc soft-float
  result = vfa + vfb;
#endif
  return result;
}

__binary128
test_vec_addqpo_V1 (__binary128 vfa, __binary128 vfb)
{
  __binary128 result;
#if defined (_ARCH_PWR9) && (__GNUC__ > 7)
#if defined (__FLOAT128__) && (__GNUC__ > 8)
  // earlier GCC versions generate extra data moves for this.
  result = __builtin_addf128_round_to_odd (vfa, vfb);
#else
  // No extra data moves here.
  __asm__(
      "xsaddqpo %0,%1,%2"
      : "=v" (result)
      : "v" (vfa), "v" (vfb)
      : );
#endif
#elif  defined (_ARCH_PWR7)
  vui64_t q_exp, a_exp, b_exp, x_exp;
  vui128_t q_sig, a_sig, b_sig, p_tmp, p_odd;
  vui128_t a_mag, b_mag;
  vui128_t s_sig, x_bits;
  vui32_t q_sign,  a_sign,  b_sign;
  vb128_t a_lt_b;
  const vui32_t signmask = CONST_VINT128_W(0x80000000, 0, 0, 0);
  const vui64_t q_zero = { 0, 0 };
  const vui64_t q_ones = { -1, -1 };
  const vui64_t exp_min = (vui64_t) CONST_VINT64_DW( 1, 1 );
  const vui64_t exp_dnrm = (vui64_t) CONST_VINT64_DW( 0, 0 );
  const vui64_t q_expnaninf = (vui64_t) CONST_VINT64_DW( 0x7fff, 0x7fff );
  const vui64_t q_expmax = (vui64_t) CONST_VINT64_DW( 0x7ffe, 0x7ffe );
  const vui32_t sigov = CONST_VINT128_W(0x000fffff, -1, -1, -1);
  const vui32_t sigovt = CONST_VINT128_W(0x0007ffff, -1, -1, -1);
  const vui32_t xbitmask = CONST_VINT128_W(0, 0, 0, 1);

  a_exp = vec_xsxexpqp (vfa);
  a_sig = vec_xsxsigqp (vfa);
  a_sign = vec_and_bin128_2_vui32t (vfa, signmask);
  b_exp = vec_xsxexpqp (vfb);
  b_sig = vec_xsxsigqp (vfb);
  b_sign = vec_and_bin128_2_vui32t (vfb, signmask);
  x_exp = vec_mrgahd ((vui128_t) a_exp, (vui128_t) b_exp);

//  if (vec_all_isfinitef128 (vfa) && vec_all_isfinitef128 (vfb))
//  The above can be optimized to the following
  if (__builtin_expect (vec_cmpud_all_lt (x_exp, q_expnaninf), 1))
    {
      const vui64_t q_one = { 1, 1 };
      vui128_t add_sig, sub_sig;
      vb64_t exp_mask;
      vui32_t diff_sign;

      q_sign = vec_xor (a_sign, b_sign);

      // Mask off sign bits so can use integers for magnitude compare.
      a_mag = (vui128_t) vec_andc_bin128_2_vui32t (vfa, signmask);
      b_mag = (vui128_t) vec_andc_bin128_2_vui32t (vfb, signmask);
      // Precondition the significands before add so the GRX bits
      // are in the least significant 3 bit.
      a_sig = vec_slqi (a_sig, 3);
      b_sig = vec_slqi (b_sig, 3);

      // If sign(vfa) != sign(vfb) will need to:
      // 1) Subtract instead of add significands
      // 2) Generate signed zeros
      diff_sign = (vui32_t) vec_setb_sq ((vi128_t) q_sign);
      // If magnitude(b) >  magnitude(a) will need to swap a/b, later
      a_lt_b = vec_cmpltuq (a_mag, b_mag);

      // Correct exponent for zeros or denormals to E_min
      // will force 0 exponents for zero/denormal results later
      exp_mask = vec_cmpequd (x_exp, exp_dnrm);
      // x_exp = (vui64_t) vec_sel ((vui32_t) x_exp, (vui32_t) exp_min, (vui32_t) exp_mask);
      x_exp = (vui64_t) vec_sel (x_exp, exp_min, exp_mask);

      // Now swap operands a/b if necessary so a has greater magnitude.
	{
	  vui128_t a_tmp = a_sig;
	  vui128_t b_tmp = b_sig;
	  vui64_t x_tmp = vec_swapd (x_exp);

	  q_sign = vec_sel (a_sign, b_sign, (vui32_t) a_lt_b);

	  x_exp = vec_sel (x_exp, x_tmp, (vui64_t) a_lt_b);
	  a_exp = vec_splatd (x_exp, VEC_DW_H);
	  b_exp = vec_splatd (x_exp, VEC_DW_L);
	  q_exp = a_exp;

	  a_sig = vec_seluq (a_tmp, b_tmp, (vb128_t) a_lt_b);
	  b_sig = vec_seluq (b_tmp, a_tmp, (vb128_t) a_lt_b);
	}
      // At this point we can assume that:
      // The magnitude (vfa) >= magnitude (vfb)
      // 1) Exponents (a_exp, b_exp) in the range E_min -> E_max
      // 2) a_exp >= b_exp
      // 2a) If a_exp == b_exp then a_sig >= b_sig
      // 2b) If a_exp > b_exp then
      //     shift (b_sig) right by (a_exp - b_exp)
      //     any bits shifted out of b_sig are ORed into the X-bit
      if (vec_cmpud_all_lt (b_exp, a_exp))
	{
	  vui64_t d_exp, l_exp;
	  vui128_t t_sig;
	  const vui64_t exp_128 = (vui64_t) CONST_VINT64_DW( 128, 128 );

	  d_exp = vec_subudm (a_exp, b_exp);
	  if (vec_cmpud_all_lt (d_exp, exp_128))
	    {
	      l_exp = vec_subudm (exp_128, d_exp);
	      t_sig = vec_srq (b_sig, (vui128_t) d_exp);
	      x_bits = vec_slq (b_sig, (vui128_t) l_exp);
	    }
	  else
	    {
	      x_bits = b_sig;
	      t_sig = (vui128_t) q_zero;
	    }

	  p_odd = vec_addcuq (x_bits, (vui128_t) q_ones);
	  b_sig = (vui128_t) vec_or ((vui32_t) t_sig, (vui32_t) p_odd);
	}

      // If operands have the same sign then s_sig = a_sig + b_sig
      // Otherwise s_sig = a_sig - b_sig
      add_sig = vec_adduqm (a_sig, b_sig);
      sub_sig = vec_subuqm (a_sig, b_sig);
      s_sig = vec_seluq (add_sig, sub_sig, (vb128_t) diff_sign);

      if (vec_cmpuq_all_eq (s_sig, (vui128_t) q_zero))
	{ // Special case of both zero with different sign
	  q_sign = vec_sel (a_sign, (vui32_t) q_zero, diff_sign);
	  return vec_xfer_vui32t_2_bin128 (q_sign);
	}
      else if (vec_cmpuq_all_gt (s_sig, (vui128_t) sigov))
	{ // Check for carry and adjust
	  p_odd = (vui128_t) vec_and ((vui32_t) s_sig, xbitmask);
	  s_sig = vec_srqi (s_sig, 1);
	  s_sig = (vui128_t) vec_or ((vui32_t) s_sig, (vui32_t) p_odd);
	  q_exp = vec_addudm (q_exp, q_one);
	}
      else if (vec_cmpuq_all_le (s_sig, (vui128_t) sigovt))
	{
	  // Or the significand is below normal range.
	  // This can happen with subtraction.
	  const vui64_t exp_12 = { 12, 12 };
	  vui64_t c_exp, d_exp;
	  vui128_t c_sig;

	  c_sig = vec_clzq (s_sig);
	  c_exp = vec_splatd ((vui64_t) c_sig, VEC_DW_L);
	  c_exp = vec_subudm (c_exp, exp_12);
	  d_exp = vec_subudm (q_exp, (vui64_t) exp_min);
	  d_exp = vec_minud (c_exp, d_exp);

	  if (vec_cmpsd_all_gt ((vi64_t) q_exp, (vi64_t) exp_min))
	    {
	      vb64_t exp_mask = vec_cmpgtud (q_exp, c_exp);

	      s_sig = vec_slq (s_sig, (vui128_t) d_exp);
	      q_exp = vec_subudm (q_exp, d_exp);
	      q_exp = (vui64_t) vec_sel (q_zero, (vui64_t) q_exp, exp_mask);
	    }
	  else
	    {
	      // Exponent is less than or equal to E_min
	      // so return denormal result.
	      q_exp = q_zero;
	    }
	}
      // Round to odd from low order GRX-bits
      p_tmp = vec_slqi (s_sig, 125);
      p_odd = vec_addcuq (p_tmp, (vui128_t) q_ones);
      q_sig = vec_srqi (s_sig, 3);
      q_sig = (vui128_t) vec_or ((vui32_t) q_sig, (vui32_t) p_odd);

      // Check for exponent overflow -> __FLT128_MAX__
      if (vec_cmpud_all_gt (q_exp, q_expmax))
	{
	  // return maximum finite exponent and significand
	  q_exp = q_expmax;
	  q_sig = (vui128_t) sigov;
	}
    }
  else
    { // One or both operands are NaN or Infinity
	{
	  // One or both operands are NaN
	  const vui32_t q_nan = CONST_VINT128_W(0x00008000, 0, 0, 0);
	  if (vec_all_isnanf128 (vfa))
	    {
	      // vfa is NaN
	      q_sign = a_sign;
	      q_sig = (vui128_t) vec_or ((vui32_t) a_sig, q_nan);
	      q_exp = a_exp;
	    }
	  else if (vec_all_isnanf128 (vfb))
	    {
	      // vfb is NaN
	      q_sign = b_sign;
	      q_sig = (vui128_t) vec_or ((vui32_t) b_sig, q_nan);
	      q_exp = b_exp;
	    }
	  else  // Or one or both operands are Infinity
	    {
//	      a_exp = vec_splatd (a_exp, VEC_DW_H);
//	      b_exp = vec_splatd (b_exp, VEC_DW_H);
	      if (vec_cmpud_all_eq (x_exp, q_expnaninf)
		  && vec_cmpud_any_ne ((vui64_t) a_sign, (vui64_t) b_sign))
		{ // Both operands infinity and opposite sign
		  // Inifinty + Infinity (opposite sign) is Default Quiet NaN
		  return vec_const_nanf128 ();
		}
	      else
		{ // Either both operands infinity and same sign
		  // Or one infinity and one finite
		  if (vec_cmpud_any_eq (a_exp, q_expnaninf))
		    {
		      // return infinity
		      return vfa;
		    }
		  else
		    {
		      // return infinity
		      return vfb;
		    }
		}
	    }
	}
    }
  // Merge sign, significand, and exponent into final result
  q_sig = (vui128_t) vec_or ((vui32_t) q_sig, q_sign);
  result = vec_xsiexpqp (q_sig, q_exp);
#else // ! _ARCH_PWR8, use libgcc soft-float
  result = vfa + vfb;
#endif
  return result;
}

__binary128
test_vec_addqpo_V0 (__binary128 vfa, __binary128 vfb)
{
  __binary128 result;
#if defined (_ARCH_PWR9) && (__GNUC__ > 7)
#if defined (__FLOAT128__) && (__GNUC__ > 8)
  // earlier GCC versions generate extra data moves for this.
  result = __builtin_addf128_round_to_odd (vfa, vfb);
#else
  // No extra data moves here.
  __asm__(
      "xsaddqpo %0,%1,%2"
      : "=v" (result)
      : "v" (vfa), "v" (vfb)
      : );
#endif
#elif  defined (_ARCH_PWR7)
  vui64_t q_exp, a_exp, b_exp, x_exp;
  vui128_t q_sig, a_sig, b_sig, p_tmp, p_odd;
  vui128_t a_mag, b_mag;
  vui128_t s_sig, x_bits;
  vui32_t q_sign,  a_sign,  b_sign;
  vui32_t diff_sign;
  vb128_t a_lt_b;
  const vui32_t signmask = CONST_VINT128_W(0x80000000, 0, 0, 0);
  const vui64_t q_zero = { 0, 0 };
  const vui64_t q_ones = { -1, -1 };
  const vi64_t exp_min = (vi64_t) CONST_VINT64_DW( 1, 1 );
  const vui64_t exp_dnrm = (vui64_t) CONST_VINT64_DW( 0, 0 );
  const vui64_t q_expnaninf = (vui64_t) CONST_VINT64_DW( 0x7fff, 0x7fff );
  const vui64_t q_expmax = (vui64_t) CONST_VINT64_DW( 0x7ffe, 0x7ffe );
  const vui32_t sigov = CONST_VINT128_W(0x000fffff, -1, -1, -1);
  const vui32_t sigovt = CONST_VINT128_W(0x0007ffff, -1, -1, -1);
  const vui32_t xbitmask = CONST_VINT128_W(0, 0, 0, 1);

  a_exp = vec_xsxexpqp (vfa);
  a_sig = vec_xsxsigqp (vfa);
  a_sign = vec_and_bin128_2_vui32t (vfa, signmask);
  b_exp = vec_xsxexpqp (vfb);
  b_sig = vec_xsxsigqp (vfb);
  b_sign = vec_and_bin128_2_vui32t (vfb, signmask);
  x_exp = vec_mrgahd ((vui128_t) a_exp, (vui128_t) b_exp);
  q_sign = vec_xor (a_sign, b_sign);
  diff_sign = (vui32_t) vec_cmpneuq ((vui128_t) a_sign, (vui128_t) b_sign);

//  if (vec_all_isfinitef128 (vfa) && vec_all_isfinitef128 (vfb))
//  The above can be optimized to the following
  if (__builtin_expect (vec_cmpud_all_lt (x_exp, q_expnaninf), 1))
    {
      const vui64_t q_one = { 1, 1 };
      vui128_t add_sig, sub_sig;

      a_mag = (vui128_t) vec_andc_bin128_2_vui32t (vfa, signmask);
      b_mag = (vui128_t) vec_andc_bin128_2_vui32t (vfb, signmask);
      // Precondition the significands before add so the GRX bits
      // are in the least significant 3 bit.
      a_sig = vec_slqi (a_sig, 3);
      b_sig = vec_slqi (b_sig, 3);

      // If magnitude(b) >  magnitude(a) will need to swap a/b, later
      a_lt_b = vec_cmpltuq (a_mag, b_mag);

//      if (vec_cmpud_any_eq (x_exp, exp_dnrm))
	{ // Involves zeros or denormals
	  // check for zero significands in add
	  if (vec_cmpuq_all_eq (a_sig, (vui128_t) q_zero))
	    {
	      if (vec_cmpuq_all_eq (b_sig, (vui128_t) q_zero))
		{ // Return Exact-zero-difference result.
		  q_sign = vec_sel (b_sign, (vui32_t) q_zero, diff_sign);
		  return vec_xfer_vui32t_2_bin128 (q_sign);
		}
	      else
		{
		  // Add to zero, return vfb
		  return vfb;
		}
	    }
	  else if (vec_cmpuq_all_eq (b_sig, (vui128_t) q_zero))
	    { // Add to zero, return vfa
	      return vfa;
	    }
	  else
	    {
	      vb64_t exp_mask;
	      exp_mask = vec_cmpequd (x_exp, exp_dnrm);
	      x_exp = (vui64_t) vec_sel (x_exp, (vui64_t) exp_min, exp_mask);
	    }
	}

      // Now swap a/b is necessary so a has greater magnitude.
	{
	  vui128_t a_tmp = a_sig;
	  vui128_t b_tmp = b_sig;
	  vui64_t x_tmp = vec_swapd (x_exp);

	  q_sign = vec_sel (a_sign, b_sign, (vui32_t) a_lt_b);

	  x_exp = vec_sel (x_exp, x_tmp, (vui64_t) a_lt_b);
	  a_exp = vec_splatd (x_exp, VEC_DW_H);
	  b_exp = vec_splatd (x_exp, VEC_DW_L);
	  q_exp = a_exp;

	  a_sig = vec_seluq (a_tmp, b_tmp, (vb128_t) a_lt_b);
	  b_sig = vec_seluq (b_tmp, a_tmp, (vb128_t) a_lt_b);
	}
      // At this point we can assume that:
      // The magnitude (vfa) >= magnitude (vfb)
      // 1) Exponents (a_exp, b_exp) in the range E_min -> E_max
      // 2) a_exp >= b_exp
      // 2a) If a_exp == b_exp then a_sig >= b_sig
      // 2b) If a_exp > b_exp then
      //     shift (b_sig) right by (a_exp-b_exp)

      // If (b_exp < a_exp) then
      // Shift right b_sig by (a_exp - b_exp)
      // Collect any shift out of b_sig and or them into the X-bit
      if (vec_cmpud_all_lt (b_exp, a_exp))
	{
	  vui64_t d_exp, l_exp;
	  vui128_t t_sig;
	  const vui64_t exp_128 = (vui64_t) CONST_VINT64_DW( 128, 128 );

	  d_exp = vec_subudm (a_exp, b_exp);
	  if (vec_cmpud_all_lt (d_exp, exp_128))
	    {
	      l_exp = vec_subudm (exp_128, d_exp);
	      t_sig = vec_srq (b_sig, (vui128_t) d_exp);
	      x_bits = vec_slq (b_sig, (vui128_t) l_exp);
	    }
	  else
	    {
	      x_bits = b_sig;
	      t_sig = (vui128_t) q_zero;
	    }

	  p_odd = vec_addcuq (x_bits, (vui128_t) q_ones);
	  b_sig = (vui128_t) vec_or ((vui32_t) t_sig, (vui32_t) p_odd);
	}

      add_sig = vec_adduqm (a_sig, b_sig);
      sub_sig = vec_subuqm (a_sig, b_sig);
      s_sig = vec_seluq (add_sig, sub_sig, (vb128_t) diff_sign);

      if (vec_cmpuq_all_eq (s_sig, (vui128_t) q_zero))
	{ // Return Exact-zero-difference result.
	    return vec_xfer_vui64t_2_bin128 (q_zero);
	}
      else if (vec_cmpuq_all_gt (s_sig, (vui128_t) sigov))
	{ // Check for carry and adjust
	  p_odd = (vui128_t) vec_and ((vui32_t) s_sig, xbitmask);
	  s_sig = vec_srqi (s_sig, 1);
	  s_sig = (vui128_t) vec_or ((vui32_t) s_sig, (vui32_t) p_odd);
	  q_exp = vec_addudm (q_exp, q_one);
	}
      else if (vec_cmpuq_all_le (s_sig, (vui128_t) sigovt))
	{
	  // Or the significand is below normal range.
	  // This can happen with subtraction.
	  const vui64_t exp_15 = { 12, 12 };
	  vui64_t c_exp, d_exp;
	  vui128_t c_sig;

	  c_sig = vec_clzq (s_sig);
	  c_exp = vec_splatd ((vui64_t) c_sig, VEC_DW_L);
	  c_exp = vec_subudm (c_exp, exp_15);
	  d_exp = vec_subudm (q_exp, (vui64_t) exp_min);
	  d_exp = vec_minud (c_exp, d_exp);

	  if (vec_cmpsd_all_gt ((vi64_t) q_exp, exp_min))
	    {
	      s_sig = vec_slq (s_sig, (vui128_t) d_exp);
	      if (vec_cmpud_all_le (q_exp, c_exp))
		{
		  // Intermediate result == tiny, unbiased exponent == -16382
		  // Check if sig is denormal range (L-bit is 0).
		  q_exp = q_zero;
		}
	      else
		q_exp = vec_subudm (q_exp, d_exp);

	    }
	  else
	    {
	      // Intermediate result == tiny, unbiased exponent == -16382
	      // sig is denormal range (L-bit is 0).
	      q_exp = q_zero;
	    }
	}
#if 0
      // Accumulate x_bits
      p_odd = vec_addcuq (x_bits, (vui128_t) q_ones);
      s_sig = (vui128_t) vec_or ((vui32_t) s_sig, (vui32_t) p_odd);
#endif
      // Round to odd from lower product bits
      p_tmp = vec_slqi (s_sig, 125);
      p_odd = vec_addcuq (p_tmp, (vui128_t) q_ones);
      q_sig = vec_srqi (s_sig, 3);
      q_sig = (vui128_t) vec_or ((vui32_t) q_sig, (vui32_t) p_odd);

      // Check for exponent overflow -> __FLT128_INF__
      if (vec_cmpud_all_gt (q_exp, q_expmax))
	{
	  // Intermediate result is huge, unbiased exponent > 16383
	  q_exp = q_expmax;
	  q_sig = (vui128_t) sigov;
	}
    }
  else
    { // One or both operands are NaN or Infinity
	{
	  // One or both operands are NaN
	  const vui32_t q_nan = CONST_VINT128_W(0x00008000, 0, 0, 0);
	  if (vec_all_isnanf128 (vfa))
	    {
	      // vfa is NaN
	      q_sign = a_sign;
	      q_sig = (vui128_t) vec_or ((vui32_t) a_sig, q_nan);
	      q_exp = a_exp;
	    }
	  else if (vec_all_isnanf128 (vfb))
	    {
	      // vfb is NaN
	      q_sign = b_sign;
	      q_sig = (vui128_t) vec_or ((vui32_t) b_sig, q_nan);
	      q_exp = b_exp;
	    }
	  else  // Or one or both operands are Infinity
	    {
	      a_exp = vec_splatd (a_exp, VEC_DW_H);
	      b_exp = vec_splatd (b_exp, VEC_DW_H);
	      if (vec_cmpud_all_eq (x_exp, q_expnaninf)
		  && vec_cmpuq_all_ne ((vui128_t) a_sign, (vui128_t) b_sign))
		{ // Both operands infinity and opposite sign
		  // Inifinty + Infinity (opposite sign) is Default Quiet NaN
		  return vec_const_nanf128 ();
		}
	      else
		{ // Either both operands infinity and same sign
		  // Or one infinity and one finite
		  if (vec_cmpud_all_eq (a_exp, q_expnaninf))
		    {
		      // return infinity
		      return vfa;
		    }
		  else
		    {
		      // return infinity
		      return vfb;
		    }
		}
	    }
	}
    }
  // Merge sign, significand, and exponent into final result
  q_sig = (vui128_t) vec_or ((vui32_t) q_sig, q_sign);
  result = vec_xsiexpqp (q_sig, q_exp);
#else // ! _ARCH_PWR8, use libgcc soft-float
  result = vfa + vfb;
#endif
  return result;
}
#endif // ndef __clang__
#endif

// test compile final xsdivqpo inline implementation
__binary128
test_vec_xsdivqpo(__binary128 vfa, __binary128 vfb)
{
  return vec_xsdivqpo_inline (vfa, vfb);
}

__binary128
test_vec_divqpo (__binary128 vfa, __binary128 vfb)
{
  __binary128 result;
#if defined (_ARCH_PWR9) && (__GNUC__ > 7)
#if defined (__FLOAT128__) && (__GNUC__ > 8)
  // earlier GCC versions generate extra data moves for this.
  result = __builtin_divf128_round_to_odd (vfa, vfb);
#else
  // No extra data moves here.
  __asm__(
      "xsdivqpo %0,%1,%2"
      : "=v" (result)
      : "v" (vfa), "v" (vfb)
      : );
#endif
#elif  defined (_ARCH_PWR8)
  vui64_t q_exp, a_exp, b_exp, x_exp;
  vui32_t q_sign,  a_sign,  b_sign;
  vui128_t q_sig, a_mag, b_mag;
  const vui64_t q_zero = { 0, 0 };
  const vui64_t q_ones = { -1, -1 };
  const vui64_t exp_naninf = vec_mask64_f128exp ();
  const vui32_t magmask = vec_mask128_f128mag ();

  // Vector extract the exponents from vfa, vfb
  x_exp = vec_xxxexpqpp (vfa, vfb);
  a_exp = vec_splatd (x_exp, VEC_DW_H);
  b_exp = vec_splatd (x_exp, VEC_DW_L);
  // Mask off sign bits so can use integers for magnitude compare.
  a_mag = (vui128_t) vec_and_bin128_2_vui32t (vfa, magmask);
  b_mag = (vui128_t) vec_and_bin128_2_vui32t (vfb, magmask);
  a_sign = vec_andc_bin128_2_vui32t (vfa, magmask);
  b_sign = vec_andc_bin128_2_vui32t (vfb, magmask);
  q_sign = vec_xor (a_sign, b_sign);

//  if (vec_all_isfinitef128 (vfa) && vec_all_isfinitef128 (vfb))
//  The above can be optimized to the following
  if (__builtin_expect (vec_cmpud_all_lt (x_exp, exp_naninf), 1))
    {
      const vui64_t exp_dnrm = q_zero;
      vui32_t hidden = vec_mask128_f128Lbit();
      vi64_t exp_min = vec_splat_s64 ( 1 );
      vui128_t a_sig, b_sig, p_sig_h, p_sig_l, p_odd;
      vui64_t exp_bias;
      vui32_t q_inf = vec_mask128_f128exp ();

      if (__builtin_expect (vec_cmpud_any_eq (x_exp, exp_dnrm), 0))
	{ // Involves zeros or denormals
	  // check for zero significands in Divide
	  // Can use magnitude compare here
	  if (vec_cmpuq_all_eq (b_mag, (vui128_t) q_zero))
	    { // Divide by zero, return QP Infinity OR QNAN
	      if (vec_cmpuq_all_eq (a_mag, (vui128_t) q_zero))
		{ // Divide by zero, return QP Infinity
		  result = vec_const_nanf128 ();
		}
	      else
		{ // Divide by zero, return QP Infinity
		  q_sign = vec_or (q_sign, q_inf);
		  result = vec_xfer_vui32t_2_bin128 (q_sign);
		}
	      return result;
	    }
	  else if (__builtin_expect (vec_cmpuq_all_eq (a_mag, (vui128_t) q_zero), 0))
	    { // finite divisor with zero dividend , return QP signed zero
	      return vec_xfer_vui32t_2_bin128 (q_sign);
	    }
	  else
	    {
	      // need to Normalize Denormals before divide
	      vui128_t a_tmp, b_tmp;
	      vui64_t a_adj = q_zero;
	      vui64_t b_adj = q_zero;
	      vui64_t x_adj;
	      vui32_t a_frac, b_frac;
	      // Extract the Fraction without restoring the hidden bit.
	      // This is enough for denormals before normalization.
	      a_frac = vec_andc ((vui32_t) a_mag, q_inf);
	      b_frac = vec_andc ((vui32_t) b_mag, q_inf);

	      vb64_t exp_mask;
	      exp_mask = vec_cmpequd (x_exp, exp_dnrm);
	      x_exp = (vui64_t) vec_sel (x_exp, (vui64_t) exp_min, exp_mask);

	      if  (__builtin_expect (vec_cmpud_all_eq (a_exp, exp_dnrm), 0))
		{
		  a_tmp = vec_slqi ((vui128_t) a_frac, 15);
		  a_adj = (vui64_t) vec_clzq (a_tmp);
		  a_sig = vec_slq ((vui128_t) a_frac, (vui128_t) a_adj);
		}
	      else // vfa is finite and normal, insert hidden-bit
		  a_sig = (vui128_t) vec_or (a_frac, hidden);

	      if  (__builtin_expect (vec_cmpud_all_eq (b_exp, exp_dnrm), 0 ))
		{
		  b_tmp = vec_slqi ((vui128_t) b_frac, 15);
		  b_adj = (vui64_t) vec_clzq (b_tmp);
		  b_sig = vec_slq ((vui128_t) b_frac, (vui128_t) b_adj);
		}
	      else // vfb is finite and normal, insert hidden-bit
		  b_sig = (vui128_t) vec_or (b_frac, hidden);

	      // Adjust exponents with extended range
	      x_adj = vec_mrgald ((vui128_t) a_adj, (vui128_t) b_adj);
	      x_exp = vec_subudm (x_exp, x_adj);

	      a_exp = vec_splatd (x_exp, VEC_DW_H);
	      b_exp = vec_splatd (x_exp, VEC_DW_L);
	    }
	}
      else
	{
	  // Both are finite and normal
	  vui32_t a_frac, b_frac;
	  // Extract the Fraction then set the hidden bit.
	  a_frac = vec_andc ((vui32_t) a_mag, q_inf);
	  b_frac = vec_andc ((vui32_t) b_mag, q_inf);
	  a_sig = (vui128_t) vec_or (a_frac, hidden);
	  b_sig = (vui128_t) vec_or (b_frac, hidden);
	  a_exp = vec_splatd (a_exp, VEC_DW_H);
	  b_exp = vec_splatd (b_exp, VEC_DW_H);
	}
      // Precondition the significands before multiply so that the
      // high-order 114-bits (C,L,FRACTION) of the product are right
      // adjusted in p_sig_h. And the low-order 112-bits are left
      // justified in p_sig_l.
      // Using Divide extended we are effective performing a 256-bit
      // by 128-bit divide.
      b_sig = vec_slqi (b_sig, 8);
#if  0 //defined (_ARCH_PWR9)
      p_sig_l = vec_diveuqo_inline (a_sig, b_sig);
#else
#if 1
      p_sig_l = test_vec_diveuq_qpo (a_sig, b_sig);
#else
      p_sig_l = test_vec_diveuqo_V1 (a_sig, b_sig);
#endif
#endif
      p_sig_h = (vui128_t) vec_sld ((vui8_t) q_zero, (vui8_t) p_sig_l, 15);
      p_sig_l = (vui128_t) vec_sld ((vui8_t) p_sig_l, (vui8_t) q_zero, 15);
      // Generate exp_bias while avoiding a vector load
      exp_bias = (vui64_t) vec_srhi ((vui16_t) exp_naninf, 1);
      // sum exponents
      q_exp = vec_subudm (a_exp, b_exp);
      q_exp = vec_addudm (q_exp, exp_bias);

      // There are two cases for denormal (after divide)
      // 1) The sum of unbiased exponents is less than E_min (tiny).
      // 2) The significand is less than 1.0 (C and L-bits are zero).
      //  2a) The exponent is greater than E_min
      //  2b) The exponent is equal to E_min
      //
      if (__builtin_expect (vec_cmpsd_all_lt ((vi64_t) q_exp, exp_min), 0))
	{
	    // const vui64_t exp_tinyer = (vui64_t) CONST_VINT64_DW( 116, 116 );
	    const vui64_t exp_tinyer = vec_const64_f128_116 ();
	    // const vui32_t xmask = CONST_VINT128_W(0x1fffffff, -1, -1, -1);
	    vui32_t xmask = vec_mask128_f128Xbits ();
	    vui32_t tmp;
	    // Intermediate result is tiny, unbiased exponent < -16382
	    x_exp = vec_subudm ((vui64_t) exp_min, q_exp);

	    if  (vec_cmpud_all_gt ((vui64_t) x_exp, exp_tinyer))
	      {
		// Intermediate result is too tiny, the shift will
		// zero the fraction and the GR-bit leaving only the
		// Sticky bit. The X-bit needs to include all bits
		// from p_sig_h and p_sig_l
		p_sig_l = vec_srqi (p_sig_l, 16);
		p_sig_l = (vui128_t) vec_or ((vui32_t) p_sig_l, (vui32_t) p_sig_h);
		// generate a carry into bit-2 for any nonzero bits 3-127
		p_sig_l = vec_adduqm (p_sig_l, (vui128_t) xmask);
		q_sig = (vui128_t) q_zero;
		p_sig_l = (vui128_t) vec_andc ((vui32_t) p_sig_l, xmask);
	      }
	    else
	      { // Normal tiny, right shift may lose low order bits
		// from p_sig_l. So collect any 1-bits below GRX and
		// OR them into the X-bit, before the right shift.
		vui64_t l_exp;
		// const vui64_t exp_128 = (vui64_t) CONST_VINT64_DW( 128, 128 );
		const vui64_t exp_128 = vec_const64_f128_128 ();
		// Propagate low order bits into the sticky bit
		// GRX left adjusted in p_sig_l
		// Isolate bits below GDX (bits 3-128).
		tmp = vec_and ((vui32_t) p_sig_l, xmask);
		// generate a carry into bit-2 for any nonzero bits 3-127
		tmp = (vui32_t) vec_adduqm ((vui128_t) tmp, (vui128_t) xmask);
		// Or this with the X-bit to propagate any sticky bits into X
		p_sig_l = (vui128_t) vec_or ((vui32_t) p_sig_l, tmp);
		p_sig_l = (vui128_t) vec_andc ((vui32_t) p_sig_l, xmask);

		l_exp = vec_subudm (exp_128, x_exp);

		p_sig_l = vec_sldq (p_sig_h, p_sig_l, (vui128_t) l_exp);
		p_sig_h = vec_srq (p_sig_h, (vui128_t) x_exp);
		q_sig = p_sig_h;
	      }
	    q_exp = q_zero;
	}
      else
	{
	  // const vui32_t sigovt = CONST_VINT128_W(0x0000ffff, -1, -1, -1);
	  vui32_t sigovt = vec_sld ((vui32_t) q_zero, (vui32_t) q_ones, 14);

	  // Exponent is not tiny.
	  if (vec_cmpuq_all_le (p_sig_h, (vui128_t) sigovt))
	    {
	      // But the significand is below normal range.
	      const vui64_t exp_15 = vec_splat_u64 (15);
	      vui64_t c_exp, d_exp;
	      vui128_t c_sig;

	      c_sig = vec_clzq (p_sig_h);
	      c_exp = vec_splatd ((vui64_t) c_sig, VEC_DW_L);
	      c_exp = vec_subudm (c_exp, exp_15);
	      d_exp = vec_subudm (q_exp, (vui64_t) exp_min);
	      d_exp = vec_minud (c_exp, d_exp);

	      if (vec_cmpsd_all_gt ((vi64_t) q_exp, exp_min))
		{
		  p_sig_h = vec_sldq (p_sig_h, p_sig_l, (vui128_t) d_exp);
		  p_sig_l = vec_slq (p_sig_l, (vui128_t) d_exp);
		  if (vec_cmpud_all_le (q_exp, c_exp))
		    {
		      // Intermediate result == tiny, unbiased exponent == -16382
		      // Check if sig is denormal range (L-bit is 0).
		      q_exp = q_zero;
		    }
		  else
		    q_exp = vec_subudm (q_exp, d_exp);

		}
	      else
		{
		  // Intermediate result == tiny, unbiased exponent == -16382
		  // sig is denormal range (L-bit is 0).
		  q_exp = q_zero;
		}
	    }
	  q_sig = p_sig_h;
	}

      // Round to odd from lower product bits
      p_odd = vec_addcuq (p_sig_l, (vui128_t) q_ones);
      q_sig = (vui128_t)  vec_or ((vui32_t) q_sig, (vui32_t) p_odd);

      // Check for exponent overflow -> __FLT128_MAX__ (round to odd)
      if (__builtin_expect ((vec_cmpud_all_ge (q_exp, exp_naninf)), 0))
      {
	  // Intermediate result is huge, unbiased exponent > 16383
	  // so return __FLT128_MAX__ with the appropriate sign.
	  // const vui32_t f128_max = CONST_VINT128_W(0x7ffeffff, -1, -1, -1);
	  const vui32_t f128_max = vec_const128_f128_fmax ();
	  vui32_t f128_smax = vec_or ((vui32_t) f128_max, q_sign);
	  return vec_xfer_vui32t_2_bin128 (f128_smax);
      }
    }
  else
    {
      // One or both operands are NaN or Infinity
      vui32_t q_nan = vec_mask128_f128Qbit ();
      vui32_t q_inf = vec_mask128_f128exp ();
      vui128_t a_frac, b_frac;
      // Extract the Fraction without restoring the hidden bit.
      // This is enough to distinguish NaN from Inf.
      // Note The xor with vec_mask128_f128exp () will leave nonzero
      // bits in the Exponent for finite values. This is Ok and frac
      // can distinguish Infinities from finite values.
      a_frac = (vui128_t) vec_xor ((vui32_t) a_mag, q_inf);
      b_frac = (vui128_t) vec_xor ((vui32_t) b_mag, q_inf);

      if (vec_cmpuq_all_eq (a_frac, (vui128_t) q_zero)
	  && vec_cmpuq_all_eq (b_frac, (vui128_t) q_zero))
	{
	  // Operands either infinity or zero
	  if (vec_cmpud_any_eq (x_exp, q_zero))
	    {
	      // Inifinty / Zero is Inifinty
	      vui32_t vf128 = vec_or (q_sign, (vui32_t) a_mag);
	      return vec_xfer_vui32t_2_bin128 (vf128);
	    }
	  else
	    {
	      // Infinity / Infinity == Quiet NAN
	      return vec_const_nanf128 ();
	    }
	}
      else
	{
	  // One or both operands are NaN
	  if (vec_all_isnanf128 (vfa))
	    {
	      // vfa is NaN, Convert vfa to QNaN and return
	      vui32_t vf128 = vec_or_bin128_2_vui32t (vfa, q_nan);
	      return vec_xfer_vui32t_2_bin128 (vf128);
	    }
	  else if (vec_all_isnanf128 (vfb))
	    {
	      // vfb is NaN, Convert vfb to QNaN and return
	      vui32_t vf128 = vec_or_bin128_2_vui32t (vfb, q_nan);
	      return vec_xfer_vui32t_2_bin128 (vf128);
	    }
	  else  // OR an Infinity and a Nonzero finite number
	    {
	      if (vec_cmpuq_all_eq (a_frac, (vui128_t) q_zero))
		{ // vfa is a Infinity, return signed Infinity
		  vui32_t vf128;
		  vf128 = vec_or (q_sign, q_inf);
		  return vec_xfer_vui32t_2_bin128 (vf128);
		}
	      else
		{  // vfb is a Infinity, return signed zero
		  // Exp and sig are already zero in q_sign;
		  return vec_xfer_vui32t_2_bin128 (q_sign);
		}
	    }
	}
    }
  // Merge sign, significand, and exponent into final result
  q_sig = (vui128_t) vec_or ((vui32_t) q_sig, q_sign);
  result = vec_xsiexpqp (q_sig, q_exp);
#endif
  return result;
}

#ifdef PVECLIB_OLDTESTVERSIONS
#ifndef __clang__
__binary128
test_vec_divqpo_V1 (__binary128 vfa, __binary128 vfb)
{
  __binary128 result;
#if defined (_ARCH_PWR9) && (__GNUC__ > 7)
#if defined (__FLOAT128__) && (__GNUC__ > 8)
  // earlier GCC versions generate extra data moves for this.
  result = __builtin_divf128_round_to_odd (vfa, vfb);
#else
  // No extra data moves here.
  __asm__(
      "xsdivqpo %0,%1,%2"
      : "=v" (result)
      : "v" (vfa), "v" (vfb)
      : );
#endif
#elif  defined (_ARCH_PWR8)
  vui64_t q_exp, a_exp, b_exp, x_exp;
  vui32_t q_sign,  a_sign,  b_sign;
  vui128_t q_sig, a_mag, b_mag;
  //const vui32_t signmask = CONST_VINT128_W(0x80000000, 0, 0, 0);
  const vui64_t q_zero = { 0, 0 };
  const vui64_t q_ones = { -1, -1 };
  //const vui64_t exp_naninf = (vui64_t) { 0x7fff, 0x7fff };
  const vui64_t exp_naninf = vec_mask64_f128exp ();
  //const vui64_t q_expnaninf = (vui64_t) CONST_VINT64_DW( 0x7fff, 0x7fff );
  const vui32_t magmask = vec_mask128_f128mag ();
  // const vui64_t exp_bias = (vui64_t) CONST_VINT64_DW( 0x3fff, 0x3fff );
  // const vi64_t exp_min = (vi64_t) CONST_VINT64_DW( 1, 1 );
  // const vui64_t q_expmax = (vui64_t) CONST_VINT64_DW( 0x7ffe, 0x7ffe );

  // Vector extract the exponents from vfa, vfb
  // a_exp = vec_xsxexpqp (vfa);
  // b_exp = vec_xsxexpqp (vfb);
  x_exp = vec_xxxexpqpp (vfa, vfb);
  a_exp = vec_splatd (x_exp, VEC_DW_H);
  b_exp = vec_splatd (x_exp, VEC_DW_L);
  // Mask off sign bits so can use integers for magnitude compare.
  a_mag = (vui128_t) vec_and_bin128_2_vui32t (vfa, magmask);
  b_mag = (vui128_t) vec_and_bin128_2_vui32t (vfb, magmask);
  //a_sign = vec_and_bin128_2_vui32t (vfa, signmask);
  a_sign = vec_andc_bin128_2_vui32t (vfa, magmask);
  //b_sign = vec_and_bin128_2_vui32t (vfb, signmask);
  b_sign = vec_andc_bin128_2_vui32t (vfb, magmask);
  //x_exp = vec_mrgahd ((vui128_t) a_exp, (vui128_t) b_exp);
  q_sign = vec_xor (a_sign, b_sign);

//  if (vec_all_isfinitef128 (vfa) && vec_all_isfinitef128 (vfb))
//  The above can be optimized to the following
  if (__builtin_expect (vec_cmpud_all_lt (x_exp, exp_naninf), 1))
    {
      //const vui32_t sigovt = CONST_VINT128_W(0x0000ffff, -1, -1, -1);
      const vui64_t exp_dnrm = q_zero;
      vui32_t hidden = vec_mask128_f128Lbit();
      vi64_t exp_min = vec_splat_s64 ( 1 );
      vui128_t a_sig, b_sig, p_sig_h, p_sig_l, p_odd;
      vui64_t exp_bias;
      vui32_t q_inf = vec_mask128_f128exp ();
      //a_sig = vec_xsxsigqp (vfa);
      //b_sig = vec_xsxsigqp (vfb);

      if (__builtin_expect (vec_cmpud_any_eq (x_exp, exp_dnrm), 0))
	{ // Involves zeros or denormals
	  // check for zero significands in Divide
	  // Can use magnitude compare here
	  if (vec_cmpuq_all_eq (b_mag, (vui128_t) q_zero))
	    { // Divide by zero, return QP Infinity OR QNAN
	      if (vec_cmpuq_all_eq (a_mag, (vui128_t) q_zero))
		{ // Divide by zero, return QP Infinity
		  result = vec_const_nanf128 ();
		}
	      else
		{ // Divide by zero, return QP Infinity
		  q_sign = vec_or (q_sign, q_inf);
		  result = vec_xfer_vui32t_2_bin128 (q_sign);
		}
	      return result;
	    }
	  else if (__builtin_expect (vec_cmpuq_all_eq (a_mag, (vui128_t) q_zero), 0))
	    { // finite divisor with zero dividend , return QP signed zero
	      return vec_xfer_vui32t_2_bin128 (q_sign);
	    }
	  else
	    {
	      // need to Normalize Denormals before divide
	      vui128_t a_tmp, b_tmp;
	      vui64_t a_adj = q_zero;
	      vui64_t b_adj = q_zero;
	      vui64_t x_adj;
	      vui32_t a_frac, b_frac;
	      // Extract the Fraction without restoring the hidden bit.
	      // This is enough for denormals before normalization.
	      a_frac = vec_andc ((vui32_t) a_mag, q_inf);
	      b_frac = vec_andc ((vui32_t) b_mag, q_inf);

	      vb64_t exp_mask;
	      exp_mask = vec_cmpequd (x_exp, exp_dnrm);
	      x_exp = (vui64_t) vec_sel (x_exp, (vui64_t) exp_min, exp_mask);

	      if  (__builtin_expect (vec_cmpud_all_eq (a_exp, exp_dnrm), 0))
		{
		  a_tmp = vec_slqi ((vui128_t) a_frac, 15);
		  a_adj = (vui64_t) vec_clzq (a_tmp);
		  a_sig = vec_slq ((vui128_t) a_frac, (vui128_t) a_adj);
		}
	      else
		{
		  a_sig = (vui128_t) vec_or (a_frac, hidden);
		}

	      if  (__builtin_expect (vec_cmpud_all_eq (b_exp, exp_dnrm), 0 ))
		{
		  b_tmp = vec_slqi ((vui128_t) b_frac, 15);
		  b_adj = (vui64_t) vec_clzq (b_tmp);
		  b_sig = vec_slq ((vui128_t) b_frac, (vui128_t) b_adj);
		}
	      else
		{
		  b_sig = (vui128_t) vec_or (b_frac, hidden);
		}

	      x_adj = vec_mrgald ((vui128_t) a_adj, (vui128_t) b_adj);
	      x_exp = vec_subudm (x_exp, x_adj);

	      a_exp = vec_splatd (x_exp, VEC_DW_H);
	      b_exp = vec_splatd (x_exp, VEC_DW_L);
	    }
	}
      else
	{
	  // Both are finite and normal
	  // a_sig = vec_xsxsigqp (vfa);
	  // b_sig = vec_xsxsigqp (vfb);
	  vui32_t a_frac, b_frac;
	  // Extract the Fraction then set the hidden bit.
	  a_frac = vec_andc ((vui32_t) a_mag, q_inf);
	  b_frac = vec_andc ((vui32_t) b_mag, q_inf);
	  a_sig = (vui128_t) vec_or (a_frac, hidden);
	  b_sig = (vui128_t) vec_or (b_frac, hidden);
	  a_exp = vec_splatd (a_exp, VEC_DW_H);
	  b_exp = vec_splatd (b_exp, VEC_DW_H);
	}
      // Precondition the significands before multiply so that the
      // high-order 114-bits (C,L,FRACTION) of the product are right
      // adjusted in p_sig_h. And the low-order 112-bits are left
      // justified in p_sig_l.
      // Using Divide extended we are effective performing a 256-bit
      // by 128-bit divide.
      b_sig = vec_slqi (b_sig, 8);
#if 1 //defined (_ARCH_PWR9)
      p_sig_l = vec_diveuqo_inline (a_sig, b_sig);
#else
      p_sig_l = test_vec_diveuqo (a_sig, b_sig);
#endif
      p_sig_h = (vui128_t) vec_sld ((vui8_t) q_zero, (vui8_t) p_sig_l, 15);
      p_sig_l = (vui128_t) vec_sld ((vui8_t) p_sig_l, (vui8_t) q_zero, 15);
      // Generate exp_bias while avoiding a vector load
      exp_bias = (vui64_t) vec_srhi ((vui16_t) exp_naninf, 1);
      // sum exponents
      q_exp = vec_subudm (a_exp, b_exp);
      q_exp = vec_addudm (q_exp, exp_bias);

      // There are two cases for denormal (after divide)
      // 1) The sum of unbiased exponents is less than E_min (tiny).
      // 2) The significand is less than 1.0 (C and L-bits are zero).
      //  2a) The exponent is greater than E_min
      //  2b) The exponent is equal to E_min
      //
      if (__builtin_expect (vec_cmpsd_all_lt ((vi64_t) q_exp, exp_min), 0))
	{
	    const vui64_t exp_tinyer = (vui64_t) CONST_VINT64_DW( 116, 116 );
	    // const vui32_t xmask = CONST_VINT128_W(0x1fffffff, -1, -1, -1);
	    vui32_t xmask = (vui32_t) vec_srqi ((vui128_t) q_ones, 3);
	    vui32_t tmp;
	    // Intermediate result is tiny, unbiased exponent < -16382
	    x_exp = vec_subudm ((vui64_t) exp_min, q_exp);

	    if  (vec_cmpud_all_gt ((vui64_t) x_exp, exp_tinyer))
	      {
		// Intermediate result is too tiny, the shift will
		// zero the fraction and the GR-bit leaving only the
		// Sticky bit. The X-bit needs to include all bits
		// from p_sig_h and p_sig_l
		p_sig_l = vec_srqi (p_sig_l, 16);
		p_sig_l = (vui128_t) vec_or ((vui32_t) p_sig_l, (vui32_t) p_sig_h);
		// generate a carry into bit-2 for any nonzero bits 3-127
		p_sig_l = vec_adduqm (p_sig_l, (vui128_t) xmask);
		q_sig = (vui128_t) q_zero;
		p_sig_l = (vui128_t) vec_andc ((vui32_t) p_sig_l, xmask);
	      }
	    else
	      { // Normal tiny, right shift may lose low order bits
		// from p_sig_l. So collect any 1-bits below GRX and
		// OR them into the X-bit, before the right shift.
		vui64_t l_exp;
		// const vui64_t exp_128 = (vui64_t) CONST_VINT64_DW( 128, 128 );
		const vui64_t exp_128 = vec_const64_f128_128 ();
		// Propagate low order bits into the sticky bit
		// GRX left adjusted in p_sig_l
		// Isolate bits below GDX (bits 3-128).
		tmp = vec_and ((vui32_t) p_sig_l, xmask);
		// generate a carry into bit-2 for any nonzero bits 3-127
		tmp = (vui32_t) vec_adduqm ((vui128_t) tmp, (vui128_t) xmask);
		// Or this with the X-bit to propagate any sticky bits into X
		p_sig_l = (vui128_t) vec_or ((vui32_t) p_sig_l, tmp);
		p_sig_l = (vui128_t) vec_andc ((vui32_t) p_sig_l, xmask);

		l_exp = vec_subudm (exp_128, x_exp);

		p_sig_l = vec_sldq (p_sig_h, p_sig_l, (vui128_t) l_exp);
		p_sig_h = vec_srq (p_sig_h, (vui128_t) x_exp);
		q_sig = p_sig_h;
	      }
	    q_exp = q_zero;
	}
      else
	{
	  // const vui32_t sigovt = CONST_VINT128_W(0x0000ffff, -1, -1, -1);
	  vui32_t sigovt = vec_sld ((vui32_t) q_zero, (vui32_t) q_ones, 14);

	  // Exponent is not tiny.
	  if (vec_cmpuq_all_le (p_sig_h, (vui128_t) sigovt))
	    {
	      // But the significand is below normal range.
	      // This can happen when multiplying a denormal by a
	      // normal.
	      const vui64_t exp_15 = vec_splat_u64 (15);
	      vui64_t c_exp, d_exp;
	      vui128_t c_sig;

	      c_sig = vec_clzq (p_sig_h);
	      c_exp = vec_splatd ((vui64_t) c_sig, VEC_DW_L);
	      c_exp = vec_subudm (c_exp, exp_15);
	      d_exp = vec_subudm (q_exp, (vui64_t) exp_min);
	      d_exp = vec_minud (c_exp, d_exp);

	      if (vec_cmpsd_all_gt ((vi64_t) q_exp, exp_min))
		{
		  p_sig_h = vec_sldq (p_sig_h, p_sig_l, (vui128_t) d_exp);
		  p_sig_l = vec_slq (p_sig_l, (vui128_t) d_exp);
		  if (vec_cmpud_all_le (q_exp, c_exp))
		    {
		      // Intermediate result == tiny, unbiased exponent == -16382
		      // Check if sig is denormal range (L-bit is 0).
		      q_exp = q_zero;
		    }
		  else
		    q_exp = vec_subudm (q_exp, d_exp);

		}
	      else
		{
		  // Intermediate result == tiny, unbiased exponent == -16382
		  // sig is denormal range (L-bit is 0).
		  q_exp = q_zero;
		}
	    }
	  q_sig = p_sig_h;
	}

      // Round to odd from lower product bits
      p_odd = vec_addcuq (p_sig_l, (vui128_t) q_ones);
      q_sig = (vui128_t)  vec_or ((vui32_t) q_sig, (vui32_t) p_odd);

      // Check for exponent overflow -> __FLT128_MAX__ (round to odd)
      if (__builtin_expect ((vec_cmpud_all_ge (q_exp, exp_naninf)), 0))
      {
	  // Intermediate result is huge, unbiased exponent > 16383
	  // so return __FLT128_MAX__ with the appropriate sign.
	  const vui32_t f128_max = CONST_VINT128_W(0x7ffeffff, -1, -1, -1);
	  vui32_t f128_smax = vec_or ((vui32_t) f128_max, q_sign);
	  return vec_xfer_vui32t_2_bin128 (f128_smax);
      }
    }
  else
    {
      // One or both operands are NaN or Infinity

      //const vui32_t q_nan = CONST_VINT128_W(0x00008000, 0, 0, 0);
      vui32_t q_nan = vec_mask128_f128Qbit ();
      vui32_t q_inf = vec_mask128_f128exp ();
      // vui32_t q_frac = vec_mask128_f128sig ();
      vui128_t a_frac, b_frac;
      // Extract the Fraction without restoring the hidden bit.
      // This is enough to distinguish NaN from Inf.
      // Note The xor with vec_mask128_f128exp () will leave nonzero
      // bits in the Exponent for finite values. This is Ok and frac
      // required to distinguish Infinities from finite values.
      a_frac = (vui128_t) vec_xor ((vui32_t) a_mag, q_inf);
      b_frac = (vui128_t) vec_xor ((vui32_t) b_mag, q_inf);

      if (vec_cmpuq_all_eq (a_frac, (vui128_t) q_zero)
	  && vec_cmpuq_all_eq (b_frac, (vui128_t) q_zero))
	{
	  // Operands either infinity or zero
	  if (vec_cmpud_any_eq (x_exp, q_zero))
	    {
	      // Inifinty / Zero is Inifinty
	      vui32_t vf128 = vec_xor (a_sign, b_sign);
	      vf128 = vec_or (vf128, (vui32_t) a_mag);
	      return vec_xfer_vui32t_2_bin128 (vf128);
	    }
	  else
	    {
	      // Infinity / Infinity == Quiet NAN
	      return vec_const_nanf128 ();
	    }
	}
      else
	{
	  // One or both operands are NaN
	  if (vec_all_isnanf128 (vfa))
	    {
	      // vfa is NaN, Convert vfa to QNaN and return
	      vui32_t vf128 = vec_or_bin128_2_vui32t (vfa, q_nan);
	      return vec_xfer_vui32t_2_bin128 (vf128);
	    }
	  else if (vec_all_isnanf128 (vfb))
	    {
	      // vfb is NaN, Convert vfb to QNaN and return
	      vui32_t vf128 = vec_or_bin128_2_vui32t (vfb, q_nan);
	      return vec_xfer_vui32t_2_bin128 (vf128);
	    }
	  else  // OR an Infinity and a Nonzero finite number
	    {
	      if (vec_cmpuq_all_eq (a_frac, (vui128_t) q_zero))
		{ // vfa is a Infinity, return signed Infinity
		  vui32_t vf128;
		  //q_sign = vec_xor (a_sign, b_sign);
		  vf128 = vec_or (q_sign, q_inf);
		  return vec_xfer_vui32t_2_bin128 (vf128);
		}
	      else
		{  // vfb is a Infinity, return signed zero
		  // Exp and sig are already zero in q_sign;
		  //q_sign = vec_xor (a_sign, b_sign);
		  return vec_xfer_vui32t_2_bin128 (q_sign);
		}
	    }
	}
    }
  // Merge sign, significand, and exponent into final result
  q_sig = (vui128_t) vec_or ((vui32_t) q_sig, q_sign);
  result = vec_xsiexpqp (q_sig, q_exp);
#endif
  return result;
}

__binary128
test_vec_divqpo_V0 (__binary128 vfa, __binary128 vfb)
{
  __binary128 result;
#if defined (_ARCH_PWR9) && (__GNUC__ > 7)
#if defined (__FLOAT128__) && (__GNUC__ > 8)
  // earlier GCC versions generate extra data moves for this.
  result = __builtin_divf128_round_to_odd (vfa, vfb);
#else
  // No extra data moves here.
  __asm__(
      "xsdivqpo %0,%1,%2"
      : "=v" (result)
      : "v" (vfa), "v" (vfb)
      : );
#endif
#elif  defined (_ARCH_PWR8)
  vui64_t q_exp, a_exp, b_exp, x_exp;
  vui128_t q_sig, a_sig, b_sig, p_sig_h, p_sig_l, p_odd;
  vui32_t q_sign,  a_sign,  b_sign;
  //const vui32_t signmask = CONST_VINT128_W(0x80000000, 0, 0, 0);
  const vui64_t q_zero = { 0, 0 };
  const vui64_t q_ones = { -1, -1 };
  //const vui64_t exp_naninf = (vui64_t) { 0x7fff, 0x7fff };
  const vui64_t exp_naninf = vec_mask64_f128exp ();
  //const vui64_t q_expnaninf = (vui64_t) CONST_VINT64_DW( 0x7fff, 0x7fff );
  const vui32_t magmask = vec_mask128_f128mag ();
  // const vui64_t exp_bias = (vui64_t) CONST_VINT64_DW( 0x3fff, 0x3fff );
  // const vi64_t exp_min = (vi64_t) CONST_VINT64_DW( 1, 1 );
  const vui64_t exp_dnrm = (vui64_t) CONST_VINT64_DW( 0, 0 );
  // const vui64_t q_expmax = (vui64_t) CONST_VINT64_DW( 0x7ffe, 0x7ffe );

  a_exp = vec_xsxexpqp (vfa);
  a_sig = vec_xsxsigqp (vfa);
  //a_sign = vec_and_bin128_2_vui32t (vfa, signmask);
  a_sign = vec_andc_bin128_2_vui32t (vfa, magmask);
  b_exp = vec_xsxexpqp (vfb);
  b_sig = vec_xsxsigqp (vfb);
  //b_sign = vec_and_bin128_2_vui32t (vfb, signmask);
  b_sign = vec_andc_bin128_2_vui32t (vfb, magmask);
  x_exp = vec_mrgahd ((vui128_t) a_exp, (vui128_t) b_exp);
  q_sign = vec_xor (a_sign, b_sign);

//  if (vec_all_isfinitef128 (vfa) && vec_all_isfinitef128 (vfb))
//  The above can be optimized to the following
  if (__builtin_expect (vec_cmpud_all_lt (x_exp, exp_naninf), 1))
    {
      vi64_t exp_min = vec_splat_s64 ( 1 );
      vui64_t exp_bias;

      if (__builtin_expect (vec_cmpud_any_eq (x_exp, exp_dnrm), 0))
	{ // Involves zeros or denormals
	  // check for zero significands in Divide
	  if (vec_cmpuq_all_eq (b_sig, (vui128_t) q_zero))
	    { // Divide by zero, return QP Infinity OR QNAN
	      if (vec_cmpuq_all_eq (a_sig, (vui128_t) q_zero))
		{ // Divide by zero, return QP Infinity
		  result = vec_const_nanf128 ();
		}
	      else
		{ // Divide by zero, return QP Infinity
		  //q_sign = vec_xor (a_sign, b_sign);
		  q_exp = exp_naninf;
#if 1
		  q_sig = (vui128_t) q_sign;
#else
		  q_sig = (vui128_t) q_zero;
		  q_sig = (vui128_t) vec_or ((vui32_t) q_sig, q_sign);
#endif
		  result = vec_xsiexpqp (q_sig, q_exp);
		}
	      return result;
	    }
	  else if (vec_cmpuq_all_eq (a_sig, (vui128_t) q_zero))
	    { // finite divisor with zero dividend , return QP signed zero
	      result = vec_xfer_vui32t_2_bin128 (q_sign);
	      return result;
	    }
	  else
	    {
	      // need to Normalize Denormals before divide
	      vui128_t a_tmp, b_tmp;
	      vui64_t a_adj = {0, 0};
	      vui64_t b_adj = {0, 0};
	      vui64_t x_adj;

	      vb64_t exp_mask;
	      exp_mask = vec_cmpequd (x_exp, exp_dnrm);
	      x_exp = (vui64_t) vec_sel (x_exp, (vui64_t) exp_min, exp_mask);

	      if  (vec_cmpud_all_eq (a_exp, exp_dnrm))
		{
		  a_tmp = vec_slqi (a_sig, 15);
		  a_adj = (vui64_t) vec_clzq (a_tmp);
		  a_sig = vec_slq (a_sig, (vui128_t) a_adj);
		}
	      if  (vec_cmpud_all_eq (b_exp, exp_dnrm))
		{
		  b_tmp = vec_slqi (b_sig, 15);
		  b_adj = (vui64_t) vec_clzq (b_tmp);
		  b_sig = vec_slq (b_sig, (vui128_t) b_adj);
		}
	      x_adj = vec_mrgald ((vui128_t) a_adj, (vui128_t) b_adj);
	      x_exp = vec_subudm (x_exp, x_adj);

	      a_exp = vec_splatd (x_exp, VEC_DW_H);
	      b_exp = vec_splatd (x_exp, VEC_DW_L);
	    }
	}
      else
	{
	  a_exp = vec_splatd (a_exp, VEC_DW_H);
	  b_exp = vec_splatd (b_exp, VEC_DW_H);
	}
      // Precondition the significands before multiply so that the
      // high-order 114-bits (C,L,FRACTION) of the product are right
      // adjusted in p_sig_h. And the low-order 112-bits are left
      // justified in p_sig_l.
      // Using Divide extended we are effective performing a 256-bit
      // by 128-bit divide.
      b_sig = vec_slqi (b_sig, 8);
#if 1 //defined (_ARCH_PWR9)
      p_sig_l = vec_diveuqo_inline (a_sig, b_sig);
#else
      p_sig_l = test_vec_diveuqo (a_sig, b_sig);
#endif
      p_sig_h = (vui128_t) vec_sld ((vui8_t) q_zero, (vui8_t) p_sig_l, 15);
      p_sig_l = (vui128_t) vec_sld ((vui8_t) p_sig_l, (vui8_t) q_zero, 15);
      // Generate exp_bias while avoiding a vector load
      exp_bias = (vui64_t) vec_srhi ((vui16_t) exp_naninf, 1);
      // sum exponents
      q_exp = vec_subudm (a_exp, b_exp);
      q_exp = vec_addudm (q_exp, exp_bias);

      // There are two cases for denormal (after divide)
      // 1) The sum of unbiased exponents is less than E_min (tiny).
      // 2) The significand is less than 1.0 (C and L-bits are zero).
      //  2a) The exponent is greater than E_min
      //  2b) The exponent is equal to E_min
      //
      if (__builtin_expect (vec_cmpsd_all_lt ((vi64_t) q_exp, exp_min), 0))
	{
	    const vui64_t exp_tinyer = (vui64_t) CONST_VINT64_DW( 116, 116 );
	    // const vui32_t xmask = CONST_VINT128_W(0x1fffffff, -1, -1, -1);
	    vui32_t xmask = (vui32_t) vec_srqi ((vui128_t) q_ones, 3);
	    vui32_t tmp;
	    // Intermediate result is tiny, unbiased exponent < -16382
	    x_exp = vec_subudm ((vui64_t) exp_min, q_exp);

	    if  (vec_cmpud_all_gt ((vui64_t) x_exp, exp_tinyer))
	      {
		// Intermediate result is too tiny, the shift will
		// zero the fraction and the GR-bit leaving only the
		// Sticky bit. The X-bit needs to include all bits
		// from p_sig_h and p_sig_l
		p_sig_l = vec_srqi (p_sig_l, 16);
		p_sig_l = (vui128_t) vec_or ((vui32_t) p_sig_l, (vui32_t) p_sig_h);
		// generate a carry into bit-2 for any nonzero bits 3-127
		p_sig_l = vec_adduqm (p_sig_l, (vui128_t) xmask);
		q_sig = (vui128_t) q_zero;
		p_sig_l = (vui128_t) vec_andc ((vui32_t) p_sig_l, xmask);
	      }
	    else
	      { // Normal tiny, right shift may lose low order bits
		// from p_sig_l. So collect any 1-bits below GRX and
		// OR them into the X-bit, before the right shift.
		vui64_t l_exp;
		// const vui64_t exp_128 = (vui64_t) CONST_VINT64_DW( 128, 128 );
		const vui64_t exp_128 = vec_const64_f128_128 ();
		// Propagate low order bits into the sticky bit
		// GRX left adjusted in p_sig_l
		// Isolate bits below GDX (bits 3-128).
		tmp = vec_and ((vui32_t) p_sig_l, xmask);
		// generate a carry into bit-2 for any nonzero bits 3-127
		tmp = (vui32_t) vec_adduqm ((vui128_t) tmp, (vui128_t) xmask);
		// Or this with the X-bit to propagate any sticky bits into X
		p_sig_l = (vui128_t) vec_or ((vui32_t) p_sig_l, tmp);
		p_sig_l = (vui128_t) vec_andc ((vui32_t) p_sig_l, xmask);

		l_exp = vec_subudm (exp_128, x_exp);

		p_sig_l = vec_sldq (p_sig_h, p_sig_l, (vui128_t) l_exp);
		p_sig_h = vec_srq (p_sig_h, (vui128_t) x_exp);
		q_sig = p_sig_h;
	      }
	    q_exp = q_zero;
	}
      else
	{
	  // const vui32_t sigovt = CONST_VINT128_W(0x0000ffff, -1, -1, -1);
	  vui32_t sigovt = vec_sld ((vui32_t) q_zero, (vui32_t) q_ones, 14);

	  // Exponent is not tiny.
	  if (vec_cmpuq_all_le (p_sig_h, (vui128_t) sigovt))
	    {
	      // But the significand is below normal range.
	      // This can happen when multiplying a denormal by a
	      // normal.
	      const vui64_t exp_15 = vec_splat_u64 (15);
	      vui64_t c_exp, d_exp;
	      vui128_t c_sig;

	      c_sig = vec_clzq (p_sig_h);
	      c_exp = vec_splatd ((vui64_t) c_sig, VEC_DW_L);
	      c_exp = vec_subudm (c_exp, exp_15);
	      d_exp = vec_subudm (q_exp, (vui64_t) exp_min);
	      d_exp = vec_minud (c_exp, d_exp);

	      if (vec_cmpsd_all_gt ((vi64_t) q_exp, exp_min))
		{
		  p_sig_h = vec_sldq (p_sig_h, p_sig_l, (vui128_t) d_exp);
		  p_sig_l = vec_slq (p_sig_l, (vui128_t) d_exp);
		  if (vec_cmpud_all_le (q_exp, c_exp))
		    {
		      // Intermediate result == tiny, unbiased exponent == -16382
		      // Check if sig is denormal range (L-bit is 0).
		      q_exp = q_zero;
		    }
		  else
		    q_exp = vec_subudm (q_exp, d_exp);

		}
	      else
		{
		  // Intermediate result == tiny, unbiased exponent == -16382
		  // sig is denormal range (L-bit is 0).
		  q_exp = q_zero;
		}
	    }
	  q_sig = p_sig_h;
	}

      // Round to odd from lower product bits
      p_odd = vec_addcuq (p_sig_l, (vui128_t) q_ones);
      q_sig = (vui128_t)  vec_or ((vui32_t) q_sig, (vui32_t) p_odd);

      // Check for exponent overflow -> __FLT128_MAX__ (round to odd)
      if (__builtin_expect ((vec_cmpud_all_ge (q_exp, exp_naninf)), 0))
      {
	  // Intermediate result is huge, unbiased exponent > 16383
	  // so return __FLT128_MAX__ with the appropriate sign.
	  const vui32_t f128_max = CONST_VINT128_W(0x7ffeffff, -1, -1, -1);
	  vui32_t f128_smax = vec_or ((vui32_t) f128_max, q_sign);
	  return vec_xfer_vui32t_2_bin128 (f128_smax);
      }
    }
  else
    { // One or both operands are NaN or Infinity
      if (vec_cmpuq_all_eq (a_sig, (vui128_t) q_zero)
	  && vec_cmpuq_all_eq (b_sig, (vui128_t) q_zero))
	{
	  // Both operands either infinity or zero
	  if (vec_cmpud_any_eq (x_exp, q_zero))
	    {
	      // Inifinty / Zero is Inifinty
	      q_sign = vec_xor (a_sign, b_sign);
	      q_exp = a_exp;
	      q_sig = a_sig;
	    }
	  else
	    {
	      // Infinity / Infinity == Quiet NAN
	      return vec_const_nanf128 ();
	    }
	}
      else
	{
	  // One or both operands are NaN
	  const vui32_t q_nan = CONST_VINT128_W(0x00008000, 0, 0, 0);
	  if (vec_all_isnanf128 (vfa))
	    {
	      // vfa is NaN
	      q_sign = a_sign;
	      q_sig = (vui128_t) vec_or ((vui32_t) a_sig, q_nan);
	      q_exp = a_exp;
	    }
	  else if (vec_all_isnanf128 (vfb))
	    {
	      // vfb is NaN
	      q_sign = b_sign;
	      q_sig = (vui128_t) vec_or ((vui32_t) b_sig, q_nan);
	      q_exp = b_exp;
	    }
	  else  // OR an Infinity and a Nonzero finite number
	    {
	      if (vec_cmpuq_all_eq (a_sig, (vui128_t) q_zero))
		{ // vfa is a Infinity, return signed Infinity
		  q_sign = vec_xor (a_sign, b_sign);
		  q_exp = exp_naninf;
		  q_sig = (vui128_t) q_zero;
		}
	      else
		{  // vfb is a Infinity, return signed zero
		  q_sign = vec_xor (a_sign, b_sign);
		  q_exp = (vui64_t) {0, 0};
		  q_sig = (vui128_t) q_zero;
		}
	    }
	}
    }
  // Merge sign, significand, and exponent into final result
  q_sig = (vui128_t) vec_or ((vui32_t) q_sig, q_sign);
  result = vec_xsiexpqp (q_sig, q_exp);
#endif
  return result;
}
#endif
#endif

__binary128
test_negqp_nan_v0 (__binary128 vfb)
{
  __binary128 result;
#if 1
  result = vec_self128 (vec_negf128 (vfb), vfb, vec_isnanf128(vfb));
#else
  if (vec_all_isnanf128 (vfb))
    result = vfb;
  else
    result = vec_negf128 (vfb);
#endif
  return result;
}

__binary128
test_vec_subqpo (__binary128 vfa, __binary128 vfb)
{
  __binary128 result;
#if defined (_ARCH_PWR9) && (__GNUC__ > 7)
#if defined (__FLOAT128__) && (__GNUC__ > 8)
  // earlier GCC versions generate extra data moves for this.
  result = __builtin_subf128_round_to_odd (vfa, vfb);
#else
  // No extra data moves here.
  __asm__(
      "xssubqpo %0,%1,%2"
      : "=v" (result)
      : "v" (vfa), "v" (vfb)
      : );
#endif
  return result;
#elif  defined (_ARCH_PWR7)
  vui64_t q_exp, a_exp, b_exp, x_exp;
  vui128_t q_sig, a_sig, b_sig, p_tmp, p_odd;
  vui128_t a_mag, b_mag;
  vui32_t q_sign,  a_sign,  b_sign;
  vb128_t a_lt_b;
  const vui32_t q_zero = CONST_VINT128_W (0, 0, 0, 0);
  const vui32_t q_ones = CONST_VINT128_W (-1, -1, -1, -1);
  const vui32_t magmask = vec_mask128_f128mag();
  const vui64_t exp_naninf = vec_mask64_f128exp();
  // Vector extract the exponents from vfa, vfb
  x_exp = vec_xxxexpqpp (vfa, vfb);
  // Mask off sign bits so can use integers for magnitude compare.
  a_mag = (vui128_t) vec_and_bin128_2_vui32t (vfa, magmask);
  b_mag = (vui128_t) vec_and_bin128_2_vui32t (vfb, magmask);
  a_sign = vec_andc_bin128_2_vui32t (vfa, magmask);
  b_sign = vec_andc_bin128_2_vui32t (vfb, magmask);
//  if (vec_all_isfinitef128 (vfa) && vec_all_isfinitef128 (vfb))
//  The above can be optimized to the following
  if (__builtin_expect (vec_cmpud_all_lt (x_exp, exp_naninf), 1))
    {
      const vui128_t xbitmask = vec_splat_u128 (1);
      const vui128_t grx_mask = vec_splat_u128 (7);
      const vui64_t exp_min = vec_splat_u64 (1);
      const vui8_t t_sig_L = vec_splat_u8 (7);
      const vui8_t t_sig_C = vec_splat_u8 (15);
      const vui64_t exp_one = exp_min;
      const vui64_t exp_dnrm = (vui64_t) q_zero;
      // signmask is the complement of the magmask
      const vui32_t signmask = vec_nor(magmask, magmask);
      vui128_t add_sig, sub_sig;
      vui128_t s_sig, x_bits;
      vui32_t diff_sign;
      vui32_t sigmask = vec_mask128_f128sig();
      vui32_t hidden = vec_mask128_f128Lbit();
      vui32_t a_norm, b_norm, x_norm;
      vui32_t a_s32, b_s32;

      // Extract the significand
      // Assume that the sign-bit is already masked off
      // Mask off the significands
      a_s32 = vec_and ((vui32_t) a_mag, sigmask);
      b_s32 = vec_and ((vui32_t) b_mag, sigmask);
      // Assume that exponents are already extracted and merged
      // Compare exponents for denormal, assume finite
      x_norm = (vui32_t) vec_cmpgt ((vui32_t) x_exp, q_zero);
      a_norm = vec_splat (x_norm, VEC_WE_1);
      b_norm = vec_splat (x_norm, VEC_WE_3);
      // For Normal QP insert (hidden) L-bit into significand
      a_sig =  (vui128_t) vec_sel (a_s32, a_norm, hidden);
      b_sig =  (vui128_t) vec_sel (b_s32, b_norm, hidden);
      // Correct exponent for zeros or denormals to E_min
      // will force 0 exponents for zero/denormal results later
      //exp_mask = vec_cmpequd (x_exp, exp_dnrm);
      x_exp = vec_selud ( exp_min, x_exp, (vb64_t) x_norm);
      // Negate sign for subtract, then use add logic
      b_sign = vec_xor (signmask, b_sign);
      // Generation sign difference for signed 0.0
      q_sign = vec_xor (a_sign, b_sign);
      // Precondition the significands before add so the GRX bits
      // are in the least significant 3 bit.
      a_sig = vec_slqi (a_sig, 3);
      b_sig = vec_slqi (b_sig, 3);

      // If sign(vfa) != sign(vfb) will need to:
      // 1) Subtract instead of add significands
      // 2) Generate signed zeros
      diff_sign = (vui32_t) vec_setb_sq ((vi128_t) q_sign);
      // If magnitude(b) >  magnitude(a) will need to swap a/b, later
      a_lt_b = vec_cmpltuq (a_mag, b_mag);

      // Now swap operands a/b if necessary so a has greater magnitude.
      {
	vui128_t a_tmp = a_sig;
	vui128_t b_tmp = b_sig;
	vui64_t x_tmp = vec_swapd (x_exp);

	q_sign = vec_sel (a_sign, b_sign, (vui32_t) a_lt_b);

	x_exp = vec_selud (x_exp, x_tmp, (vb64_t) a_lt_b);
	a_exp = vec_splatd (x_exp, VEC_DW_H);
	b_exp = vec_splatd (x_exp, VEC_DW_L);
	q_exp = a_exp;

	a_sig = vec_seluq (a_tmp, b_tmp, (vb128_t) a_lt_b);
	b_sig = vec_seluq (b_tmp, a_tmp, (vb128_t) a_lt_b);
      }
      // At this point we can assume that:
      // The magnitude (vfa) >= magnitude (vfb)
      // 1) Exponents (a_exp, b_exp) in the range E_min -> E_max
      // 2) a_exp >= b_exp
      // 2a) If a_exp == b_exp then a_sig >= b_sig
      // 2b) If a_exp > b_exp then
      //     shift (b_sig) right by (a_exp - b_exp)
      //     any bits shifted out of b_sig are ORed into the X-bit
      if (vec_cmpud_all_lt (b_exp, a_exp))
	{
	  vui64_t d_exp, l_exp;
	  vui128_t t_sig;
	  vb128_t exp_mask;
	  const vui64_t exp_128 = vec_const64_f128_128();

	  d_exp = vec_subudm (a_exp, b_exp);
	  exp_mask = (vb128_t) vec_cmpltud (d_exp, exp_128);
	  l_exp = vec_subudm (exp_128, d_exp);
	  t_sig = vec_srq (b_sig, (vui128_t) d_exp);
	  x_bits = vec_slq (b_sig, (vui128_t) l_exp);
	  t_sig = vec_seluq ((vui128_t) q_zero, t_sig, exp_mask);
	  x_bits = vec_seluq (b_sig, x_bits, exp_mask);
	  p_odd = vec_addcuq (x_bits, (vui128_t) q_ones);
	  b_sig = (vui128_t) vec_or ((vui32_t) t_sig, (vui32_t) p_odd);
	}

      // If operands have the same sign then s_sig = a_sig + b_sig
      // Otherwise s_sig = a_sig - b_sig
      add_sig = vec_adduqm (a_sig, b_sig);
      sub_sig = vec_subuqm (a_sig, b_sig);
      s_sig = vec_seluq (add_sig, sub_sig, (vb128_t) diff_sign);

      if (__builtin_expect (vec_cmpuq_all_eq (s_sig, (vui128_t) q_zero), 0))
	{ // Special case of both zero with different sign
	  q_sign = vec_sel (a_sign, (vui32_t) q_zero, diff_sign);
	  return vec_xfer_vui32t_2_bin128 (q_sign);
	}

      // Issolate CL bits from significand too simplify the compare
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
      vui8_t t_sig = vec_splat ((vui8_t) s_sig, 14);
#else
      vui8_t t_sig = vec_splat ((vui8_t) s_sig, 1);
#endif
//      if (vec_cmpuq_all_gt (s_sig, (vui128_t) sigov))
	if (vec_all_gt (t_sig, t_sig_C))
	{ // Check for carry and adjust
	  p_odd = (vui128_t) vec_and ((vui32_t) s_sig, (vui32_t) xbitmask);
	  s_sig = vec_srqi (s_sig, 1);
	  s_sig = (vui128_t) vec_or ((vui32_t) s_sig, (vui32_t) p_odd);
	  q_exp = vec_addudm (q_exp, exp_one);
	}
      else // if (vec_cmpuq_all_le (s_sig, (vui128_t) sigovt))
	  if (vec_all_le (t_sig, t_sig_L))
	{
	  // Or the significand is below normal range.
	  // This can happen with subtraction.
	  vui64_t c_exp, d_exp;
	  vui128_t c_sig;
	  const vui64_t exp_12 = vec_splat_u64 (12);

	  c_sig = vec_clzq (s_sig);
	  c_exp = vec_splatd ((vui64_t) c_sig, VEC_DW_L);
	  // The IR has 12 leading zeros that should not effect the shift count.
	  c_exp = vec_subudm (c_exp, exp_12);
	  d_exp = vec_subudm (q_exp, (vui64_t) exp_min);
	  d_exp = vec_minud (c_exp, d_exp);
	  {
	    vb64_t nrm_mask = vec_cmpgtsd ((vi64_t) q_exp, (vi64_t) exp_min);
	    vb64_t exp_mask = vec_cmpgtud (q_exp, c_exp);

	    c_sig = vec_slq (s_sig, (vui128_t) d_exp);
	    q_exp = vec_subudm (q_exp, d_exp);
	    exp_mask = (vb64_t) vec_and ((vui32_t) exp_mask, (vui32_t) nrm_mask);
	    q_exp = vec_selud (exp_dnrm,  q_exp, exp_mask);
	    s_sig = vec_seluq (s_sig, c_sig, (vb128_t) nrm_mask);
	  }
	}
      // Round to odd from low order GRX-bits
      p_tmp = (vui128_t) vec_and ((vui32_t) s_sig, (vui32_t) grx_mask);
      p_odd = vec_addcuq (p_tmp, (vui128_t) q_ones);
      q_sig = vec_srqi (s_sig, 3);
      q_sig = (vui128_t) vec_or ((vui32_t) q_sig, (vui32_t) p_odd);
      // Check for exponent overflow -> __FLT128_MAX__
      if (__builtin_expect ((vec_cmpud_all_ge ( q_exp, exp_naninf)), 0))
	{
	  // return maximum finite exponent and significand
	  // const vui32_t f128_max = CONST_VINT128_W(0x7ffeffff, -1, -1, -1);
	  const vui32_t f128_max = vec_const128_f128_fmax ();
	  vui32_t f128_smax = vec_or ((vui32_t) f128_max, q_sign);
	  return vec_xfer_vui32t_2_bin128 (f128_smax);
	}
      // Merge sign, significand, and exponent into final result
      q_sig = (vui128_t) vec_or ((vui32_t) q_sig, q_sign);
      result = vec_xsiexpqp (q_sig, q_exp);
      return result;
    }
  else // One or both operands are NaN or Infinity
    {
      //const vui32_t q_nan = CONST_VINT128_W(0x00008000, 0, 0, 0);
      vui32_t q_nan = vec_mask128_f128Qbit ();
      // One or both operands are NaN
      if (vec_all_isnanf128 (vfa))
	{
	  // vfa is NaN, Convert vfa to QNaN and return
	  vui32_t vf128 = vec_or_bin128_2_vui32t (vfa, q_nan);
	  return vec_xfer_vui32t_2_bin128 (vf128);
	}
      else if (vec_all_isnanf128 (vfb))
	{
	  // vfb is NaN, Convert vfb to QNaN and return
	  vui32_t vf128 = vec_or_bin128_2_vui32t (vfb, q_nan);
	  return vec_xfer_vui32t_2_bin128 (vf128);
	}
      else  // Or one or both operands are Infinity
	{
	  a_exp = vec_splatd (x_exp, VEC_DW_H);
	  // b_exp = vec_splatd (x_exp, VEC_DW_L);
	  if (vec_cmpud_all_eq (x_exp, exp_naninf)
	      && vec_cmpud_all_eq ((vui64_t) a_sign, (vui64_t) b_sign))
	    { // Both operands infinity and opposite sign
	      // Inifinty - Infinity (same sign) is Default Quiet NaN
	      return vec_const_nanf128 ();
	    }
	  else
	    { // Either both operands infinity and same sign
	      // Or one infinity and one finite
	      if (vec_cmpud_any_eq (a_exp, exp_naninf))
		{
		  // return infinity
		  return vfa;
		}
	      else
		{
		  // return infinity
		  return vec_negf128(vfb);
		}
	    }
	}
    }
#else // ! _ARCH_PWR8, use libgcc soft-float
  return (vfa + vfb);
#endif
  return result;
}

#ifdef PVECLIB_OLDTESTVERSIONS
#ifndef __clang__
__binary128
test_vec_subqpo_V2 (__binary128 vfa, __binary128 vfb)
{
  __binary128 result;
#if defined (_ARCH_PWR9) && (__GNUC__ > 7)
#if defined (__FLOAT128__) && (__GNUC__ > 8)
  // earlier GCC versions generate extra data moves for this.
  result = __builtin_subf128_round_to_odd (vfa, vfb);
#else
  // No extra data moves here.
  __asm__(
      "xssubqpo %0,%1,%2"
      : "=v" (result)
      : "v" (vfa), "v" (vfb)
      : );
#endif
#elif  defined (_ARCH_PWR7)
  vui64_t q_exp, a_exp, b_exp, x_exp;
  vui64_t exp_naninf, exp_max;
  vui128_t q_sig, a_sig, b_sig, p_tmp, p_odd;
  vui128_t a_mag, b_mag;
  vui128_t s_sig, x_bits;
  vui32_t q_sign,  a_sign,  b_sign;
  vb128_t a_lt_b;
  const vui32_t signmask = CONST_VINT128_W(0x80000000, 0, 0, 0);
  const vui64_t q_zero = { 0, 0 };
  const vui64_t q_ones = { -1, -1 };
  const vui64_t exp_min = (vui64_t) CONST_VINT64_DW( 1, 1 );
  const vui64_t exp_dnrm = (vui64_t) CONST_VINT64_DW( 0, 0 );
  // const vui64_t q_expnaninf = (vui64_t) CONST_VINT64_DW( 0x7fff, 0x7fff );
  // const vui64_t q_expmax = (vui64_t) CONST_VINT64_DW( 0x7ffe, 0x7ffe );
  // Let the endian swap happen, its ok. using endian sensitive splatd.
  const vui64_t exp_naninf_max = (vui64_t) { 0x7fff, 0x7ffe };
  const vui32_t sigov = CONST_VINT128_W(0x000fffff, -1, -1, -1);
  const vui32_t sigovt = CONST_VINT128_W(0x0007ffff, -1, -1, -1);
  const vui32_t xbitmask = CONST_VINT128_W(0, 0, 0, 1);

  a_exp = vec_xsxexpqp (vfa);
  a_sig = vec_xsxsigqp (vfa);
  a_sign = vec_and_bin128_2_vui32t (vfa, signmask);
  b_exp = vec_xsxexpqp (vfb);
  b_sig = vec_xsxsigqp (vfb);
  b_sign = vec_and_bin128_2_vui32t (vfb, signmask);
  x_exp = vec_mrgahd ((vui128_t) a_exp, (vui128_t) b_exp);
  exp_naninf = vec_splatd (exp_naninf_max, 0);
//  if (vec_all_isfinitef128 (vfa) && vec_all_isfinitef128 (vfb))
//  The above can be optimized to the following
  if (__builtin_expect (vec_cmpud_all_lt (x_exp, exp_naninf), 1))
    {
      const vui64_t q_one = exp_min;
      vui128_t add_sig, sub_sig;
      vb64_t exp_mask;
      vui32_t diff_sign;

      // Negate sign for subtract, then use add logic
      b_sign = vec_xor (signmask, b_sign);
      q_sign = vec_xor (a_sign, b_sign);

      // Mask off sign bits so can use integers for magnitude compare.
      a_mag = (vui128_t) vec_andc_bin128_2_vui32t (vfa, signmask);
      b_mag = (vui128_t) vec_andc_bin128_2_vui32t (vfb, signmask);
      // Precondition the significands before add so the GRX bits
      // are in the least significant 3 bit.
      a_sig = vec_slqi (a_sig, 3);
      b_sig = vec_slqi (b_sig, 3);

      // If sign(vfa) != sign(vfb) will need to:
      // 1) Subtract instead of add significands
      // 2) Generate signed zeros
      diff_sign = (vui32_t) vec_setb_sq ((vi128_t) q_sign);
      // If magnitude(b) >  magnitude(a) will need to swap a/b, later
      a_lt_b = vec_cmpltuq (a_mag, b_mag);

      // Correct exponent for zeros or denormals to E_min
      // will force 0 exponents for zero/denormal results later
      exp_mask = vec_cmpequd (x_exp, exp_dnrm);
      x_exp = vec_selud ( x_exp, exp_min, exp_mask);

      // Now swap operands a/b if necessary so a has greater magnitude.
	{
	  vui128_t a_tmp = a_sig;
	  vui128_t b_tmp = b_sig;
	  vui64_t x_tmp = vec_swapd (x_exp);

	  q_sign = vec_sel (a_sign, b_sign, (vui32_t) a_lt_b);

	  x_exp = vec_selud (x_exp, x_tmp, (vb64_t) a_lt_b);
	  a_exp = vec_splatd (x_exp, VEC_DW_H);
	  b_exp = vec_splatd (x_exp, VEC_DW_L);
	  q_exp = a_exp;

	  a_sig = vec_seluq (a_tmp, b_tmp, (vb128_t) a_lt_b);
	  b_sig = vec_seluq (b_tmp, a_tmp, (vb128_t) a_lt_b);
	}
      // At this point we can assume that:
      // The magnitude (vfa) >= magnitude (vfb)
      // 1) Exponents (a_exp, b_exp) in the range E_min -> E_max
      // 2) a_exp >= b_exp
      // 2a) If a_exp == b_exp then a_sig >= b_sig
      // 2b) If a_exp > b_exp then
      //     shift (b_sig) right by (a_exp - b_exp)
      //     any bits shifted out of b_sig are ORed into the X-bit
      if (vec_cmpud_all_lt (b_exp, a_exp))
	{
	  vui64_t d_exp, l_exp;
	  vui128_t t_sig;
	  const vui64_t exp_128 = (vui64_t) CONST_VINT64_DW( 128, 128 );

	  d_exp = vec_subudm (a_exp, b_exp);
	  if (vec_cmpud_all_lt (d_exp, exp_128))
	    {
	      l_exp = vec_subudm (exp_128, d_exp);
	      t_sig = vec_srq (b_sig, (vui128_t) d_exp);
	      x_bits = vec_slq (b_sig, (vui128_t) l_exp);
	    }
	  else
	    {
	      x_bits = b_sig;
	      t_sig = (vui128_t) q_zero;
	    }

	  p_odd = vec_addcuq (x_bits, (vui128_t) q_ones);
	  b_sig = (vui128_t) vec_or ((vui32_t) t_sig, (vui32_t) p_odd);
	}

      // If operands have the same sign then s_sig = a_sig + b_sig
      // Otherwise s_sig = a_sig - b_sig
      add_sig = vec_adduqm (a_sig, b_sig);
      sub_sig = vec_subuqm (a_sig, b_sig);
      s_sig = vec_seluq (add_sig, sub_sig, (vb128_t) diff_sign);

      if (vec_cmpuq_all_eq (s_sig, (vui128_t) q_zero))
	{ // Special case of both zero with different sign
	  q_sign = vec_sel (a_sign, (vui32_t) q_zero, diff_sign);
	  return vec_xfer_vui32t_2_bin128 (q_sign);
	}
      else if (vec_cmpuq_all_gt (s_sig, (vui128_t) sigov))
	{ // Check for carry and adjust
	  p_odd = (vui128_t) vec_and ((vui32_t) s_sig, xbitmask);
	  s_sig = vec_srqi (s_sig, 1);
	  s_sig = (vui128_t) vec_or ((vui32_t) s_sig, (vui32_t) p_odd);
	  q_exp = vec_addudm (q_exp, q_one);
	}
      else if (vec_cmpuq_all_le (s_sig, (vui128_t) sigovt))
	{
	  // Or the significand is below normal range.
	  // This can happen with subtraction.
	  const vui64_t exp_12 = { 12, 12 };
	  vui64_t c_exp, d_exp;
	  vui128_t c_sig;

	  c_sig = vec_clzq (s_sig);
	  c_exp = vec_splatd ((vui64_t) c_sig, VEC_DW_L);
	  c_exp = vec_subudm (c_exp, exp_12);
	  d_exp = vec_subudm (q_exp, (vui64_t) exp_min);
	  d_exp = vec_minud (c_exp, d_exp);

	  if (vec_cmpsd_all_gt ((vi64_t) q_exp, (vi64_t) exp_min))
	    {
	      vb64_t exp_mask = vec_cmpgtud (q_exp, c_exp);

	      s_sig = vec_slq (s_sig, (vui128_t) d_exp);
	      q_exp = vec_subudm (q_exp, d_exp);
	      q_exp = vec_selud (q_zero, q_exp, exp_mask);
	    }
	  else
	    {
	      // Exponent is less than or equal to E_min
	      // so return denormal result.
	      q_exp = q_zero;
	    }
	}
      // Round to odd from low order GRX-bits
      p_tmp = vec_slqi (s_sig, 125);
      p_odd = vec_addcuq (p_tmp, (vui128_t) q_ones);
      q_sig = vec_srqi (s_sig, 3);
      q_sig = (vui128_t) vec_or ((vui32_t) q_sig, (vui32_t) p_odd);

      exp_max = vec_splatd (exp_naninf_max, 1);
      // Check for exponent overflow -> __FLT128_MAX__
      if (vec_cmpud_all_gt (q_exp, exp_max))
	{
	  // return maximum finite exponent and significand
	  q_exp = exp_max;
	  q_sig = (vui128_t) sigov;
	}
    }
  else
    { // One or both operands are NaN or Infinity
	{
	  // One or both operands are NaN
	  const vui32_t q_nan = CONST_VINT128_W(0x00008000, 0, 0, 0);
	  if (vec_all_isnanf128 (vfa))
	    {
	      // vfa is NaN
	      q_sign = a_sign;
	      q_sig = (vui128_t) vec_or ((vui32_t) a_sig, q_nan);
	      q_exp = a_exp;
	    }
	  else if (vec_all_isnanf128 (vfb))
	    {
	      // vfb is NaN
	      q_sign = b_sign;
	      q_sig = (vui128_t) vec_or ((vui32_t) b_sig, q_nan);
	      q_exp = b_exp;
	    }
	  else  // Or one or both operands are Infinity
	    {
	      if (vec_cmpud_all_eq (x_exp, exp_naninf)
		  && vec_cmpud_all_eq ((vui64_t) a_sign, (vui64_t) b_sign))
		{ // Both operands infinity and opposite sign
		  // Inifinty - Infinity (same sign) is Default Quiet NaN
		  return vec_const_nanf128 ();
		}
	      else
		{ // Either both operands infinity and same sign
		  // Or one infinity and one finite
		  if (vec_cmpud_any_eq (a_exp, exp_naninf))
		    {
		      // return infinity
		      return vfa;
		    }
		  else
		    {
		      // return infinity
		      return vec_negf128(vfb);
		    }
		}
	    }
	}
    }
  // Merge sign, significand, and exponent into final result
  q_sig = (vui128_t) vec_or ((vui32_t) q_sig, q_sign);
  result = vec_xsiexpqp (q_sig, q_exp);
#else // ! _ARCH_PWR8, use libgcc soft-float
  result = vfa - vfb;
#endif
  return result;
}

__binary128
test_vec_subqpo_V1 (__binary128 vfa, __binary128 vfb)
{
  __binary128 result;
#if defined (_ARCH_PWR9) && (__GNUC__ > 7)
#if defined (__FLOAT128__) && (__GNUC__ > 8)
  // earlier GCC versions generate extra data moves for this.
  result = __builtin_subf128_round_to_odd (vfa, vfb);
#else
  // No extra data moves here.
  __asm__(
      "xssubqpo %0,%1,%2"
      : "=v" (result)
      : "v" (vfa), "v" (vfb)
      : );
#endif
#elif  defined (_ARCH_PWR7)
  vui64_t q_exp, a_exp, b_exp, x_exp;
  vui128_t q_sig, a_sig, b_sig, p_tmp, p_odd;
  vui128_t a_mag, b_mag;
  vui128_t s_sig, x_bits;
  vui32_t q_sign,  a_sign,  b_sign;
  vb128_t a_lt_b;
  const vui32_t signmask = CONST_VINT128_W(0x80000000, 0, 0, 0);
  const vui64_t q_zero = { 0, 0 };
  const vui64_t q_ones = { -1, -1 };
  const vi64_t exp_min = (vi64_t) CONST_VINT64_DW( 1, 1 );
  const vui64_t exp_dnrm = (vui64_t) CONST_VINT64_DW( 0, 0 );
  const vui64_t q_expnaninf = (vui64_t) CONST_VINT64_DW( 0x7fff, 0x7fff );
  const vui64_t q_expmax = (vui64_t) CONST_VINT64_DW( 0x7ffe, 0x7ffe );
  const vui32_t sigov = CONST_VINT128_W(0x000fffff, -1, -1, -1);
  const vui32_t sigovt = CONST_VINT128_W(0x0007ffff, -1, -1, -1);
  const vui32_t xbitmask = CONST_VINT128_W(0, 0, 0, 1);

  a_exp = vec_xsxexpqp (vfa);
  a_sig = vec_xsxsigqp (vfa);
  a_sign = vec_and_bin128_2_vui32t (vfa, signmask);
  b_exp = vec_xsxexpqp (vfb);
  b_sig = vec_xsxsigqp (vfb);
  b_sign = vec_and_bin128_2_vui32t (vfb, signmask);
  x_exp = vec_mrgahd ((vui128_t) a_exp, (vui128_t) b_exp);

//  if (vec_all_isfinitef128 (vfa) && vec_all_isfinitef128 (vfb))
//  The above can be optimized to the following
  if (__builtin_expect (vec_cmpud_all_lt (x_exp, q_expnaninf), 1))
    {
      const vui64_t q_one = { 1, 1 };
      vui128_t add_sig, sub_sig;
      vb64_t exp_mask;
      vui32_t diff_sign;

      // Negate sign for subtract, then use add logic
      b_sign = vec_xor (signmask, b_sign);
      q_sign = vec_xor (a_sign, b_sign);

      // Mask off sign bits so can use integers for magnitude compare.
      a_mag = (vui128_t) vec_andc_bin128_2_vui32t (vfa, signmask);
      b_mag = (vui128_t) vec_andc_bin128_2_vui32t (vfb, signmask);
      // Precondition the significands before add so the GRX bits
      // are in the least significant 3 bit.
      a_sig = vec_slqi (a_sig, 3);
      b_sig = vec_slqi (b_sig, 3);

      // If sign(vfa) != sign(vfb) will need to:
      // 1) Subtract instead of add significands
      // 2) Generate signed zeros
      diff_sign = (vui32_t) vec_setb_sq ((vi128_t) q_sign);
      // If magnitude(b) >  magnitude(a) will need to swap a/b, later
      a_lt_b = vec_cmpltuq (a_mag, b_mag);

      // Correct exponent for zeros or denormals to E_min
      // will force 0 exponents for zero/denormal results later
      exp_mask = vec_cmpequd (x_exp, exp_dnrm);
      x_exp = (vui64_t) vec_sel ((vui32_t) x_exp, (vui32_t) exp_min, (vui32_t) exp_mask);

      // Now swap operands a/b if necessary so a has greater magnitude.
	{
	  vui128_t a_tmp = a_sig;
	  vui128_t b_tmp = b_sig;
	  vui64_t x_tmp = vec_swapd (x_exp);

	  q_sign = vec_sel (a_sign, b_sign, (vui32_t) a_lt_b);

	  x_exp = vec_sel (x_exp, x_tmp, (vui64_t) a_lt_b);
	  a_exp = vec_splatd (x_exp, VEC_DW_H);
	  b_exp = vec_splatd (x_exp, VEC_DW_L);
	  q_exp = a_exp;

	  a_sig = vec_seluq (a_tmp, b_tmp, (vb128_t) a_lt_b);
	  b_sig = vec_seluq (b_tmp, a_tmp, (vb128_t) a_lt_b);
	}
      // At this point we can assume that:
      // The magnitude (vfa) >= magnitude (vfb)
      // 1) Exponents (a_exp, b_exp) in the range E_min -> E_max
      // 2) a_exp >= b_exp
      // 2a) If a_exp == b_exp then a_sig >= b_sig
      // 2b) If a_exp > b_exp then
      //     shift (b_sig) right by (a_exp - b_exp)
      //     any bits shifted out of b_sig are ORed into the X-bit
      if (vec_cmpud_all_lt (b_exp, a_exp))
	{
	  vui64_t d_exp, l_exp;
	  vui128_t t_sig;
	  const vui64_t exp_128 = (vui64_t) CONST_VINT64_DW( 128, 128 );

	  d_exp = vec_subudm (a_exp, b_exp);
	  if (vec_cmpud_all_lt (d_exp, exp_128))
	    {
	      l_exp = vec_subudm (exp_128, d_exp);
	      t_sig = vec_srq (b_sig, (vui128_t) d_exp);
	      x_bits = vec_slq (b_sig, (vui128_t) l_exp);
	    }
	  else
	    {
	      x_bits = b_sig;
	      t_sig = (vui128_t) q_zero;
	    }

	  p_odd = vec_addcuq (x_bits, (vui128_t) q_ones);
	  b_sig = (vui128_t) vec_or ((vui32_t) t_sig, (vui32_t) p_odd);
	}

      // If operands have the same sign then s_sig = a_sig + b_sig
      // Otherwise s_sig = a_sig - b_sig
      add_sig = vec_adduqm (a_sig, b_sig);
      sub_sig = vec_subuqm (a_sig, b_sig);
      s_sig = vec_seluq (add_sig, sub_sig, (vb128_t) diff_sign);

      if (vec_cmpuq_all_eq (s_sig, (vui128_t) q_zero))
	{ // Special case of both zero with different sign
	  q_sign = vec_sel (b_sign, (vui32_t) q_zero, diff_sign);
	  return vec_xfer_vui32t_2_bin128 (q_sign);
	}
      else if (vec_cmpuq_all_gt (s_sig, (vui128_t) sigov))
	{ // Check for carry and adjust
	  p_odd = (vui128_t) vec_and ((vui32_t) s_sig, xbitmask);
	  s_sig = vec_srqi (s_sig, 1);
	  s_sig = (vui128_t) vec_or ((vui32_t) s_sig, (vui32_t) p_odd);
	  q_exp = vec_addudm (q_exp, q_one);
	}
      else if (vec_cmpuq_all_le (s_sig, (vui128_t) sigovt))
	{
	  // Or the significand is below normal range.
	  // This can happen with subtraction.
	  const vui64_t exp_15 = { 12, 12 };
	  vui64_t c_exp, d_exp;
	  vui128_t c_sig;

	  c_sig = vec_clzq (s_sig);
	  c_exp = vec_splatd ((vui64_t) c_sig, VEC_DW_L);
	  c_exp = vec_subudm (c_exp, exp_15);
	  d_exp = vec_subudm (q_exp, (vui64_t) exp_min);
	  d_exp = vec_minud (c_exp, d_exp);

	  if (vec_cmpsd_all_gt ((vi64_t) q_exp, exp_min))
	    {
	      s_sig = vec_slq (s_sig, (vui128_t) d_exp);
	      if (vec_cmpud_all_le (q_exp, c_exp))
		{
		  // Intermediate result == tiny, unbiased exponent == -16382
		  // Check if sig is denormal range (L-bit is 0).
		  q_exp = q_zero;
		}
	      else
		q_exp = vec_subudm (q_exp, d_exp);

	    }
	  else
	    {
	      // Intermediate result == tiny, unbiased exponent == -16382
	      // sig is denormal range (L-bit is 0).
	      q_exp = q_zero;
	    }
	}
      // Round to odd from lower product bits
      p_tmp = vec_slqi (s_sig, 125);
      p_odd = vec_addcuq (p_tmp, (vui128_t) q_ones);
      q_sig = vec_srqi (s_sig, 3);
      q_sig = (vui128_t) vec_or ((vui32_t) q_sig, (vui32_t) p_odd);

      // Check for exponent overflow -> __FLT128_INF__
      if (vec_cmpud_all_gt (q_exp, q_expmax))
	{
	  // Intermediate result is huge, unbiased exponent > 16383
	  q_exp = q_expmax;
	  q_sig = (vui128_t) sigov;
	}
    }
  else
    { // One or both operands are NaN or Infinity
	{
	  // One or both operands are NaN
	  const vui32_t q_nan = CONST_VINT128_W(0x00008000, 0, 0, 0);
	  if (vec_all_isnanf128 (vfa))
	    {
	      // vfa is NaN
	      q_sign = a_sign;
	      q_sig = (vui128_t) vec_or ((vui32_t) a_sig, q_nan);
	      q_exp = a_exp;
	    }
	  else if (vec_all_isnanf128 (vfb))
	    {
	      // vfb is NaN
	      q_sign = b_sign;
	      q_sig = (vui128_t) vec_or ((vui32_t) b_sig, q_nan);
	      q_exp = b_exp;
	    }
	  else  // Or one or both operands are Infinity
	    {
	      if (vec_cmpud_all_eq (x_exp, q_expnaninf)
		  && vec_cmpud_all_eq ((vui64_t) a_sign, (vui64_t) b_sign))
		{ // Both operands infinity and same sign
		  // Inifinty - Infinity (same sign) is Default Quiet NaN
		  return vec_const_nanf128 ();
		}
	      else
		{ // Either both operands infinity and different sign
		  // Or one infinity and one finite
		  if (vec_cmpud_any_eq (a_exp, q_expnaninf))
		    {
		      // return infinity
		      return vfa;
		    }
		  else
		    {
		      // return infinity
		      return vec_negf128(vfb);
		    }
		}
	    }
	}
    }
  // Merge sign, significand, and exponent into final result
  q_sig = (vui128_t) vec_or ((vui32_t) q_sig, q_sign);
  result = vec_xsiexpqp (q_sig, q_exp);
#else // ! _ARCH_PWR8, use libgcc soft-float
  result = vfa - vfb;
#endif
  return result;
}
#endif // ndef __clang__

__binary128
test_vec_subqpo_V0 (__binary128 vfa, __binary128 vfb)
{
  __binary128 result;
#if defined (_ARCH_PWR9) && (__GNUC__ > 7)
#if defined (__FLOAT128__) && (__GNUC__ > 8)
  // earlier GCC versions generate extra data moves for this.
  result = __builtin_subf128_round_to_odd (vfa, vfb);
#else
  // No extra data moves here.
  __asm__(
      "xssubqpo %0,%1,%2"
      : "=v" (result)
      : "v" (vfa), "v" (vfb)
      : );
#endif
#else  // defined (_ARCH_PWR8)
  __binary128 nvfb;
#if __USE_SELECT__
  nvfb = vec_self128 (vec_negf128 (vfb), vfb, vec_isnanf128(vfb));
#else
  if (vec_all_isnanf128 (vfb))
    nvfb = vfb;
  else
    nvfb = vec_negf128 (vfb);
#endif
  result = test_vec_addqpo (vfa, nvfb);
#endif
  return result;
}
#endif

vui128_t
test_sld16 (vui128_t *vrh, vui128_t vh, vui128_t vl)
{
  vui128_t rh, rl;
  rh = vec_sldqi (vh,vl,16);
  rl = vec_sldqi (vl,vh,16);
  *vrh = rh;
  return rl;
}

vui128_t
test_sld15 (vui128_t *vrh, vui128_t vh, vui128_t vl)
{
  vui128_t rh, rl;
  rh = vec_sldqi (vh,vl,15);
  rl = vec_slqi (vl,15);
  *vrh = rh;
  return rl;
}

vui128_t
test_sld15x (vui128_t *vrh, vui128_t vh, vui128_t vl)
{
  vui128_t rh, rl;
  rh = vec_sldqi (vh,vl,16);
  rl = vec_sldqi (vl,vh,8);
  rh = vec_srqi (rh, 1);
  rl = vec_slqi (rl, 7);
  *vrh = rh;
  return rl;
}

vui128_t
test_srd1 (vui128_t *vrh, vui128_t vh, vui128_t vl)
{
  vui128_t rh, rl, rt;
  rt = vec_sldqi (vh, vl, 120);
  rh = vec_srqi (vh, 1);
  rl = vec_slqi (rt, 7);
  *vrh = rh;
  return rl;
}

vui128_t
test_sticky_bits (vui128_t vgrx)
{
  // GRX left adjusted in vgrx
  const vui32_t smask = vec_mask128_f128Xbits ();
  vui32_t tmp;

  tmp = vec_and ((vui32_t) vgrx, smask);
  // generate a carry into bit-2 for any nonzero bits 3-127
  tmp = (vui32_t) vec_adduqm ((vui128_t) tmp, (vui128_t) smask);
  // Or this with the X-bit to propagate any sticky bits into X
  tmp = vec_or ((vui32_t) vgrx, tmp);
  return (vui128_t) vec_andc (tmp, smask);
}

__binary128
test_vec_mulqpn (__binary128 vfa, __binary128 vfb)
{
  __binary128 result;
#if defined (_ARCH_PWR9) && (__GNUC__ > 7)
#if defined (__FLOAT128__) && (__GNUC__ > 8)
  // earlier GCC versions generate extra data moves for this.
  result = (vfa * vfb);
#else
  // No extra data moves here.
  __asm__(
      "xsmulqp %0,%1,%2"
      : "=v" (result)
      : "v" (vfa), "v" (vfb)
      : );
#endif
#elif  defined (_ARCH_PWR7)
  vui64_t q_exp, a_exp, b_exp, x_exp;
  vui128_t q_sig, a_sig, b_sig, p_sig_h, p_sig_l;
  vui32_t q_sign,  a_sign,  b_sign;
  const vui32_t signmask = vec_mask128_f128sign ();
  //const vui64_t q_zero = { 0, 0 };
  const vui64_t q_zero = vec_splat_u64 (0);
  // const vui64_t q_ones = { -1, -1 };
  const vui64_t q_ones = (vui64_t) vec_splat_s64 (-1);
  //const vui64_t q_one = (vui64_t) CONST_VINT64_DW( 1, 1 );
  //const vui64_t exp_bias = (vui64_t) CONST_VINT64_DW( 0x3fff, 0x3fff );
  //const vui64_t exp_low = (vui64_t) CONST_VINT64_DW( 0x3fff, 0x3fff );
  //const vui64_t exp_min = (vui64_t) CONST_VINT64_DW( 1, 1 );
  // const vui64_t exp_dnrm = (vui64_t) CONST_VINT64_DW( 0, 0 );
  //const vui64_t exp_dnrm = vec_splat_u64 (0);
//  const vui64_t q_naninf = (vui64_t) CONST_VINT64_DW( 0x7fff, 0x7fff );
//  const vui64_t q_expmax = (vui64_t) CONST_VINT64_DW( 0x7ffe, 0x7ffe );
  const vui64_t exp_naninf = vec_mask64_f128exp ();
//  const vui32_t sigov = CONST_VINT128_W(0x0001ffff, -1, -1, -1);

  a_exp = vec_xsxexpqp (vfa);
  a_sig = vec_xsxsigqp (vfa);
  a_sign = vec_and_bin128_2_vui32t (vfa, signmask);
  b_exp = vec_xsxexpqp (vfb);
  b_sig = vec_xsxsigqp (vfb);
  b_sign = vec_and_bin128_2_vui32t (vfb, signmask);
  x_exp = vec_mrgahd ((vui128_t) a_exp, (vui128_t) b_exp);
  q_sign = vec_xor (a_sign, b_sign);

  //  if (vec_all_isfinitef128 (vfa) && vec_all_isfinitef128 (vfb))
    if (__builtin_expect (vec_cmpud_all_lt (x_exp, exp_naninf), 1))
    {
      //      const vui32_t sigov = CONST_VINT128_W(0x0001ffff, -1, -1, -1);
      //      const vui32_t sigovt = CONST_VINT128_W(0x0000ffff, -1, -1, -1);
      // const vui64_t exp_bias = (vui64_t) { 0x3fff, 0x3fff };
      const vui64_t exp_bias = vec_const64_f128bias ();
      // const vui64_t exp_max = (vui64_t) { 0x7ffe, 0x7ffe };
      const vui64_t exp_max = vec_const64_f128maxe ();
      const vui64_t exp_dnrm = q_zero;
      vui64_t exp_min, exp_one;
      vui128_t p_tmp;
      // Precondition the significands before multiply so that the
      // high-order 114-bits (C,L,FRACTION) of the product are right
      // adjusted in p_sig_h. And the Low-order 112-bits are left
      // justified in p_sig_l.
      a_sig = vec_slqi (a_sig, 8);
      b_sig = vec_slqi (b_sig, 8);
      p_sig_l = vec_muludq (&p_sig_h, a_sig, b_sig);

      // check for zero significands in multiply
      if (__builtin_expect (
	  (vec_all_eq((vui32_t ) a_sig, (vui32_t ) q_zero)
	      || vec_all_eq((vui32_t ) b_sig, (vui32_t ) q_zero)),
	  0))
	{ // Multiply by zero, return QP signed zero
	  result = vec_xfer_vui32t_2_bin128 (q_sign);
	  return result;
	}
      // const vui64_t exp_min, exp_one = { 1, 1 };
      exp_min = exp_one = vec_splat_u64 (1);
	{
	  vb64_t exp_mask;
	  exp_mask = vec_cmpequd (x_exp, exp_dnrm);
	  x_exp = vec_selud (x_exp, exp_min, (vb64_t) exp_mask);
	  a_exp = vec_splatd (x_exp, VEC_DW_H);
	  b_exp = vec_splatd (x_exp, VEC_DW_L);
	}
      // sum exponents
      q_exp = vec_addudm (a_exp, b_exp);
      q_exp = vec_subudm (q_exp, exp_bias);

      // Check for carry and adjust exp +1
	{
	  vb128_t exp_mask;
	  vui128_t sig_h, sig_l;
	  // Test Carry-bit (greater than L-bit)
	  vui16_t sig_l_mask = vec_splat_u16(1);
	  vui16_t t_sig = vec_splat ((vui16_t) p_sig_h, VEC_HW_H);
	  exp_mask = (vb128_t) vec_cmpgt (t_sig, sig_l_mask);
	  // Shift double quadword right 1 bit
	  p_tmp = vec_sldqi (p_sig_h, p_sig_l, 120);
	  sig_h = vec_srqi (p_sig_h, 1);
	  sig_l = vec_slqi (p_tmp, 7);
	  // Increment the exponent
	  x_exp = vec_addudm (q_exp, exp_one);
	  // Select original or normalized exp/sig
	  p_sig_h = vec_seluq (p_sig_h, sig_h, exp_mask);
	  p_sig_l = vec_seluq (p_sig_l, sig_l, exp_mask);
	  q_exp = vec_selud (q_exp, x_exp, (vb64_t) exp_mask);
	}
      // There are two cases for denormal
      // 1) The sum of unbiased exponents is less the E_min (tiny).
      // 2) The significand is less then 1.0 (C and L-bits are zero).
      //  2a) The exponent is > E_min
      //  2b) The exponent is == E_min
      //
      q_sig = p_sig_h;
      if (__builtin_expect (
	  (vec_cmpsd_all_lt ((vi64_t) q_exp, (vi64_t) exp_min)), 0))
	{
	  const vui64_t too_tiny = vec_const64_f128_116();
	  const vui32_t xmask = vec_mask128_f128Xbits ();
	  vui32_t tmp;

	  // Intermediate result is tiny, unbiased exponent < -16382
	  //x_exp = vec_subudm ((vui64_t) exp_tiny, q_exp);
	  x_exp = vec_subudm (exp_min, q_exp);

	  if (vec_cmpud_all_gt ((vui64_t) x_exp, too_tiny))
	    {
	      // Intermediate result is too tiny, the shift will
	      // zero the fraction and the GR-bit leaving only the
	      // Sticky bit. The X-bit needs to include all bits
	      // from p_sig_h and p_sig_l
	      p_sig_l = vec_srqi (p_sig_l, 8);
	      p_sig_l = (vui128_t) vec_or ((vui32_t) p_sig_l,
					   (vui32_t) p_sig_h);
	      // generate a carry into bit-2 for any nonzero bits 3-127
	      p_sig_l = vec_adduqm (p_sig_l, (vui128_t) xmask);
	      q_sig = (vui128_t) q_zero;
	      p_sig_l = (vui128_t) vec_andc ((vui32_t) p_sig_l, xmask);
	    }
	  else
	    { // Normal tiny, right shift may loose low order bits
	      // from p_sig_l. So collect any 1-bits below GRX and
	      // OR them into the X-bit, before the right shift.
	      vui64_t l_exp;
	      const vui64_t exp_128 = vec_const64_f128_128 ();

	      // Propagate low order bits into the sticky bit
	      // GRX left adjusted in p_sig_l
	      // Issolate bits below GDX (bits 3-128).
	      tmp = vec_and ((vui32_t) p_sig_l, xmask);
	      // generate a carry into bit-2 for any nonzero bits 3-127
	      tmp = (vui32_t) vec_adduqm ((vui128_t) tmp, (vui128_t) xmask);
	      // Or this with the X-bit to propagate any sticky bits into X
	      p_sig_l = (vui128_t) vec_or ((vui32_t) p_sig_l, tmp);
	      p_sig_l = (vui128_t) vec_andc ((vui32_t) p_sig_l, xmask);

	      l_exp = vec_subudm (exp_128, x_exp);
	      p_sig_l = vec_sldq (p_sig_h, p_sig_l, (vui128_t) l_exp);
	      p_sig_h = vec_srq (p_sig_h, (vui128_t) x_exp);
	      q_sig = p_sig_h;
	    }
	  q_exp = exp_dnrm;
	}
      // Isolate sig CL bits and compare
      vui16_t t_sig = vec_splat ((vui16_t) p_sig_h, VEC_HW_H);
      if (__builtin_expect ((vec_all_eq(t_sig, (vui16_t ) q_zero)), 0))
	{
	  // Is below normal range. This can happen when
	  // multiplying a denormal by a normal.
	  // So try to normalize the significand.
	  //const vui64_t exp_15 = { 15, 15 };
	  const vui64_t exp_15 = vec_splat_u64 (15);
	  vui64_t c_exp, d_exp;
	  vui128_t c_sig;
	  c_sig = vec_clzq (p_sig_h);
	  c_exp = vec_splatd ((vui64_t) c_sig, VEC_DW_L);
	  c_exp = vec_subudm (c_exp, exp_15);
	  d_exp = vec_subudm (q_exp, exp_min);
	  d_exp = vec_minud (c_exp, d_exp);

	  // Intermediate result <= tiny, unbiased exponent <= -16382
	  if (vec_cmpsd_all_gt ((vi64_t) q_exp, (vi64_t) exp_min))
	    {
	      vb64_t exp_mask;
	      // Try to normalize the significand.
	      p_sig_h = vec_sldq (p_sig_h, p_sig_l, (vui128_t) d_exp);
	      p_sig_l = vec_slq (p_sig_l, (vui128_t) d_exp);
	      q_sig = p_sig_h;
	      // Compare computed exp to shift count to normalize.
	      exp_mask = vec_cmpgtud (q_exp, c_exp);
	      q_exp = vec_subudm (q_exp, d_exp);
	      q_exp = vec_selud (exp_dnrm, q_exp, exp_mask);
	    }
	  else
	    { // sig is denormal range (L-bit is 0). Set exp to zero.
	      q_exp = exp_dnrm;
	    }
	}

      // Round to nearest even from lower product bits
#if 1
      vui128_t rmask = vec_srqi ((vui128_t) q_ones, 1);
#else
      const vui32_t rmask = CONST_VINT128_W(0x7fffffff, -1, -1, -1);
#endif
      vui128_t p_rnd;
      t_sig = vec_splat ((vui16_t) q_sig, VEC_HW_H);
      // For "round to Nearest, ties to even".
      // GRX = 0b001 - 0b011; truncate
      // GRX = 0b100 and bit-127 is odd; round up, otherwise truncate
      // GRX = 0b100 - 0b111; round up
      // We can simplify by copying the low order fraction bit-127
      // and OR it with bit-X. This forces a tie to round up mode
      // if the current fraction is odd, making it even,
      // Then add 0x7fff... + q_sig.bit[127] to p_sig_l,
      // This will generate a carry into fraction for rounding.
      // if and only if GRX > 0b100 or (GRX == 0b100) && (bit-127 == 1)
#if defined (_ARCH_PWR8)
      // The PowerISA 2.07B will only use the bit-127 from VRC/q_sig
      // So no separate mask operation is required.
      p_rnd = vec_addecuq (p_sig_l, (vui128_t) rmask, q_sig);
#else
      const vui32_t onemask = CONST_VINT128_W(0, 0, 0, 1);
      p_rnd = (vui128_t) vec_and ((vui32_t) q_sig, onemask);
      p_rnd = vec_addecuq (p_sig_l, (vui128_t) rmask, p_rnd);
#endif
      q_sig = vec_adduqm (q_sig, p_rnd);

      // Isolate q_sig CL bits and compare
      vui16_t h_sig = vec_splat ((vui16_t) q_sig, VEC_HW_H);
#if 1
      // check if rounding cause a carry/change in the CL-bits
      if (__builtin_expect (vec_all_gt(h_sig, t_sig), 0))
	{
	  vui16_t sig_l_mask = vec_splat_u16(1);
#if 1
	  // Check for a carry into the C-bit
	  // This needs to be normalized via right shift
	  vb128_t sft_mask = (vb128_t) vec_cmpgt (h_sig, sig_l_mask);
	  vui128_t t_sig = vec_srqi (q_sig, 1);
	  q_sig = vec_seluq (q_sig, t_sig, sft_mask);
#else
	  // If C-bit set need to right shift
	  if (vec_all_gt (h_sig, sig_l_mask))
	  q_sig = vec_srqi (q_sig, 1);
#endif
	  // Either way need to increament the exponent by 1
	  q_exp = vec_addudm (q_exp, exp_one);
	}
#else
      // exp_mask = (vb128_t) vec_cmpgt (t_sig, sig_l_mask);
      // Check for sig overflow to carry after rounding.
      if (__builtin_expect (vec_all_gt (h_sig, sig_l_mask), 0))
	{
	  q_sig = vec_srqi (q_sig, 1);
	  q_exp = vec_addudm (q_exp, exp_one);
	}
      else
	{
	  // Check for denorm to normal after rounding.
	  if (__builtin_expect (vec_all_gt(h_sig, (vui16_t ) q_zero)
		  && (vec_cmpud_all_eq (q_exp, (vui64_t) exp_dnrm)), 0))
	    {
	      q_exp = vec_addudm (q_exp, exp_one);
	    }
	}
#endif
      // Check for exponent overflow -> __FLT128_INF__
#if 0
      if (__builtin_expect ((vec_cmpud_all_ge ( q_exp, exp_naninf)), 0))
#else
      //if  (vec_cmpud_all_gt ( q_exp, exp_max))
      if (__builtin_expect ((vec_cmpud_all_gt (q_exp, exp_max)), 0))
#endif
	{
	  // Intermediate result is huge, unbiased exponent > 16383
	  // Return a signed infinity
	  q_exp = exp_naninf;
	  return vec_xsiexpqp ((vui128_t) q_sign, q_exp);
	}
    }
  else
    { // One or both operands are NaN or Infinity
      if (vec_cmpuq_all_eq (a_sig, (vui128_t) q_zero)
	  && vec_cmpuq_all_eq (b_sig, (vui128_t) q_zero))
	{
	  // Both operands either infinity or zero
	  if (vec_cmpud_any_eq (x_exp, q_zero))
	    {
	      // Inifinty x Zero is Default Quiet NaN
	      return vec_const_nanf128 ();
	    }
	  else
	    {
	      // Infinity x Infinity == signed Infinity
	      q_sign = vec_xor (a_sign, b_sign);
	      q_exp = a_exp;
	      q_sig = a_sig;
	    }
	}
      else
	{
	  // One or both operands are NaN
	  //const vui32_t q_nan = CONST_VINT128_W(0x00008000, 0, 0, 0);
	  vui32_t q_nan = vec_mask128_f128Qbit ();
	  if (vec_all_isnanf128 (vfa))
	    {
	      // vfa is NaN
	      q_sign = a_sign;
	      q_sig = (vui128_t) vec_or ((vui32_t) a_sig, q_nan);
	      q_exp = a_exp;
	    }
	  else if (vec_all_isnanf128 (vfb))
	    {
	      // vfb is NaN
	      q_sign = b_sign;
	      q_sig = (vui128_t) vec_or ((vui32_t) b_sig, q_nan);
	      q_exp = b_exp;
	    }
	  else  // OR an Infinity and a Nonzero finite number
	    {
	      q_sign = vec_xor (a_sign, b_sign);
	      q_exp = exp_naninf;
	      q_sig = (vui128_t) q_zero;
	    }
	}
    }
  // Merge sign, significand, and exponent into final result
  q_sig = (vui128_t) vec_or ((vui32_t) q_sig, q_sign);
  result = vec_xsiexpqp (q_sig, q_exp);
#else /* Not P7/8 use libgcc runtime*/
  result = vfa * vfb;
#endif
  return result;
}

#ifdef PVECLIB_OLDTESTVERSIONS
#ifndef __clang__
__binary128
test_vec_mulqpn_V1 (__binary128 vfa, __binary128 vfb)
{
  __binary128 result;
#if defined (_ARCH_PWR9) && (__GNUC__ > 7)
#if defined (__FLOAT128__) && (__GNUC__ > 8)
  // earlier GCC versions generate extra data moves for this.
  result = (vfa * vfb);
#else
  // No extra data moves here.
  __asm__(
      "xsmulqp %0,%1,%2"
      : "=v" (result)
      : "v" (vfa), "v" (vfb)
      : );
#endif
#elif  defined (_ARCH_PWR7)
  vui64_t q_exp, a_exp, b_exp, x_exp;
  vui128_t q_sig, a_sig, b_sig, p_sig_h, p_sig_l, p_odd;
  vui32_t q_sign,  a_sign,  b_sign;
  const vui32_t signmask = CONST_VINT128_W(0x80000000, 0, 0, 0);
  const vui32_t onemask = CONST_VINT128_W(0, 0, 0, 1);
  const vui64_t q_zero = { 0, 0 };
  //const vui64_t q_ones = { -1, -1 };
  //const vui64_t q_one = (vui64_t) CONST_VINT64_DW( 1, 1 );
  const vui64_t exp_bias = (vui64_t) CONST_VINT64_DW( 0x3fff, 0x3fff );
  //const vui64_t exp_low = (vui64_t) CONST_VINT64_DW( 0x3fff, 0x3fff );
  const vui64_t exp_min = (vui64_t) CONST_VINT64_DW( 1, 1 );
  const vi64_t exp_tiny = (vi64_t) CONST_VINT64_DW( 0, 0 );
  const vui64_t exp_dnrm = (vui64_t) CONST_VINT64_DW( 0, 0 );
  const vui64_t q_naninf = (vui64_t) CONST_VINT64_DW( 0x7fff, 0x7fff );
  const vui64_t q_expmax = (vui64_t) CONST_VINT64_DW( 0x7ffe, 0x7ffe );
  const vui32_t sigov = CONST_VINT128_W(0x0001ffff, -1, -1, -1);

  a_exp = vec_xsxexpqp (vfa);
  a_sig = vec_xsxsigqp (vfa);
  a_sign = vec_and_bin128_2_vui32t (vfa, signmask);
  b_exp = vec_xsxexpqp (vfb);
  b_sig = vec_xsxsigqp (vfb);
  b_sign = vec_and_bin128_2_vui32t (vfb, signmask);
  x_exp = vec_mrgahd ((vui128_t) a_exp, (vui128_t) b_exp);
  q_sign = vec_xor (a_sign, b_sign);

//  if (vec_all_isfinitef128 (vfa) && vec_all_isfinitef128 (vfb))
  if (vec_cmpud_all_lt (x_exp, q_naninf))
    {
      //const vui64_t q_one = { 1, 1 };
      vui64_t q_one = exp_min;
      const vui32_t sigovt = CONST_VINT128_W(0x0000ffff, -1, -1, -1);
      vui128_t p_tmp;
      // Precondition the significands before multiply so that the
      // high-order 114-bits (C,L,FRACTION) of the product are right
      // adjusted in p_sig_h. And the Low-order 112-bits are left
      // justified in p_sig_l.
      a_sig = vec_slqi (a_sig, 8);
      b_sig = vec_slqi (b_sig, 8);
      p_sig_l = vec_muludq (&p_sig_h, a_sig, b_sig);
      if (vec_cmpud_any_eq (x_exp, exp_dnrm))
	{ // Involves zeros or denormals
	  // check for zero significands in multiply
	  if (vec_cmpuq_all_eq (a_sig, (vui128_t) q_zero)
	      || vec_cmpuq_all_eq (b_sig, (vui128_t) q_zero))
	    { // Multiply by zero, return QP signed zero
	      result = vec_xfer_vui32t_2_bin128 (q_sign);
	      return result;
	    }
	  else
	    {
	      vb64_t exp_mask;
	      exp_mask = vec_cmpequd (x_exp, exp_dnrm);
	      x_exp = vec_selud ( x_exp, exp_min, exp_mask);
	      a_exp = vec_splatd (x_exp, VEC_DW_H);
	      b_exp = vec_splatd (x_exp, VEC_DW_L);
	    }
	}
      else
	{
	  a_exp = vec_splatd (a_exp, VEC_DW_H);
	  b_exp = vec_splatd (b_exp, VEC_DW_H);

	  // Check for carry and adjust
	  if (vec_cmpuq_all_gt (p_sig_h, (vui128_t) sigov))
	    {
	      //const vui64_t q_one = { 1, 1 };
	      p_tmp = vec_sldqi (p_sig_h, p_sig_l, 120);
	      p_sig_h = vec_srqi (p_sig_h, 1);
	      p_sig_l = vec_slqi (p_tmp, 7);
	      a_exp = vec_addudm (a_exp, q_one);
	    }
	}
      // sum exponents (adjusting for bias)
      q_exp = vec_addudm (a_exp, b_exp);
      q_exp = vec_subudm (q_exp, exp_bias);

      // There are two cases for denormal
      // 1) The sum of unbiased exponents is less the E_min (tiny).
      // 2) The significand is less then 1.0 (C and L-bits are zero).
      //  2a) The exponent is > E_min
      //  2b) The exponent is == E_min
      //
      if (vec_cmpsd_all_lt ((vi64_t) q_exp, (vi64_t) exp_min))
	{
	    {
	      const vui64_t too_tiny = (vui64_t) CONST_VINT64_DW( 116, 116 );
	      const vui32_t xmask = CONST_VINT128_W(0x1fffffff, -1, -1, -1);
	      vui32_t tmp;
	      // Intermediate result is tiny, unbiased exponent < -16382
	      //x_exp = vec_subudm ((vui64_t) exp_tiny, q_exp);
	      x_exp = vec_subudm ((vui64_t) exp_min, q_exp);

	      if  (vec_cmpud_all_gt ((vui64_t) x_exp, too_tiny))
		{
		  // Intermediate result is too tiny, the shift will
		  // zero the fraction and the GR-bit leaving only the
		  // Sticky bit. The X-bit needs to include all bits
		  // from p_sig_h and p_sig_l
		  p_sig_l = vec_srqi (p_sig_l, 8);
		  p_sig_l = (vui128_t) vec_or ((vui32_t) p_sig_l, (vui32_t) p_sig_h);
		  // generate a carry into bit-2 for any nonzero bits 3-127
		  p_sig_l = vec_adduqm (p_sig_l, (vui128_t) xmask);
		  q_sig = (vui128_t) q_zero;
		  p_sig_l = (vui128_t) vec_andc ((vui32_t) p_sig_l, xmask);
		}
	      else
		{ // Normal tiny, right shift may loose low order bits
		  // from p_sig_l. So collect any 1-bits below GRX and
		  // OR them into the X-bit, before the right shift.
		  vui64_t l_exp;
		  const vui64_t exp_128 = (vui64_t) CONST_VINT64_DW( 128, 128 );

		  // Propagate low order bits into the sticky bit
		  // GRX left adjusted in p_sig_l
		  // Issolate bits below GDX (bits 3-128).
		  tmp = vec_and ((vui32_t) p_sig_l, xmask);
		  // generate a carry into bit-2 for any nonzero bits 3-127
		  tmp = (vui32_t) vec_adduqm ((vui128_t) tmp, (vui128_t) xmask);
		  // Or this with the X-bit to propagate any sticky bits into X
		  p_sig_l = (vui128_t) vec_or ((vui32_t) p_sig_l, tmp);
		  p_sig_l = (vui128_t) vec_andc ((vui32_t) p_sig_l, xmask);
                  // Need a Double Quadword shift here, so convert right
		  // shift into shify left double quadword for p_sig_l.
		  l_exp = vec_subudm (exp_128, x_exp);
		  p_sig_l = vec_sldq (p_sig_h, p_sig_l, (vui128_t) l_exp);
		  // Complete right shift for p_sig_h
		  p_sig_h = vec_srq (p_sig_h, (vui128_t) x_exp);
		  q_sig = p_sig_h;
		}
	      q_exp = q_zero;
	    }
	}
      else
	{
	  // Exponent is not tiny.
	  if (vec_cmpuq_all_le (p_sig_h, (vui128_t) sigovt))
	    {
	      // But the significand is below normal range.
	      // This can happen when multiplying a denormal by a
	      // normal.
	      const vui64_t exp_15 = { 15, 15 };
	      vui64_t c_exp, d_exp;
	      vui128_t c_sig;
	      c_sig = vec_clzq (p_sig_h);
	      c_exp = vec_splatd ((vui64_t) c_sig, VEC_DW_L);
	      c_exp = vec_subudm (c_exp, exp_15);
	      d_exp = vec_subudm (q_exp, exp_min);
	      d_exp = vec_minud (c_exp, d_exp);

	      if (vec_cmpsd_all_gt ((vi64_t) q_exp, (vi64_t) exp_min))
		{
		  p_sig_h = vec_sldq (p_sig_h, p_sig_l, (vui128_t) d_exp);
		  p_sig_l = vec_slq (p_sig_l, (vui128_t) d_exp);
		  if (vec_cmpud_all_le (q_exp, c_exp))
		    {
		      // Intermediate result == tiny, unbiased exponent == -16382
		      // Check if sig is denormal range (L-bit is 0).
		      q_exp = q_zero;
		    }
		  else
		    q_exp = vec_subudm (q_exp, d_exp);

		}
	      else
		{
		  // Intermediate result == tiny, unbiased exponent == -16382
		  // sig is denormal range (L-bit is 0).
		  q_exp = q_zero;
		}
	    }
	  q_sig = p_sig_h;
	}

      // Round to nearest even from lower product bits
      const vui32_t rmask = CONST_VINT128_W(0x7fffffff, -1, -1, -1);
      vui128_t p_rnd;
      // For "round to Nearest, ties to even".
      // GRX = 0b001 - 0b011; truncate
      // GRX = 0b100 and bit-127 is odd; round up, otherwise truncate
      // GRX = 0b100 - 0b111; round up
      // We can simplify by copying the low order fraction bit-127
      // and OR it with bit-X. This force a tie into round up mode
      // if the current fraction is odd, making it even,
      // Then add 0x7fff + p_odd to p_sig_l,
      // this may generate a carry into fraction.
      // if and only if GRX > 0b100 or (GRX == 0b100) && (bit-127 == 1)
      p_odd = (vui128_t) vec_and ((vui32_t) q_sig, onemask);
      p_rnd = vec_addecuq (p_sig_l, (vui128_t) rmask, p_odd);
      q_sig =  vec_adduqm (q_sig, p_rnd);

      // Check for sig overflow to carry after rounding.
      if (vec_cmpuq_all_gt (q_sig, (vui128_t) sigov))
	{
	  q_sig = vec_srqi (q_sig, 1);
	  q_exp = vec_addudm (q_exp, q_one);
	}
      else
      // Check for denorm to normal after rounding.
      if (vec_cmpuq_all_gt (q_sig, (vui128_t) sigovt)
	&& (vec_cmpud_all_eq ( q_exp, (vui64_t) exp_tiny)))
	{
	  q_exp = vec_addudm (q_exp, q_one);
	}

      // Check for exponent overflow -> __FLT128_INF__
      if  (vec_cmpud_all_gt ( q_exp, q_expmax))
      {
	// Intermediate result is huge, unbiased exponent > 16383
	q_exp = q_naninf;
	q_sig = (vui128_t) q_zero;
      }
    }
  else
    { // One or both operands are NaN or Infinity
      if (vec_cmpuq_all_eq (a_sig, (vui128_t) q_zero)
	  && vec_cmpuq_all_eq (b_sig, (vui128_t) q_zero))
	{
	  // Both operands either infinity or zero
	  if (vec_cmpud_any_eq (x_exp, q_zero))
	    {
	      // Inifinty x Zero is Default Quiet NaN
	      return vec_const_nanf128 ();
	    }
	  else
	    {
	      // Infinity x Infinity == signed Infinity
	      q_sign = vec_xor (a_sign, b_sign);
	      q_exp = a_exp;
	      q_sig = a_sig;
	    }
	}
      else
	{
	  // One or both operands are NaN
	  const vui32_t q_nan = CONST_VINT128_W(0x00008000, 0, 0, 0);
	  if (vec_all_isnanf128 (vfa))
	    {
	      // vfa is NaN
	      q_sign = a_sign;
	      q_sig = (vui128_t) vec_or ((vui32_t) a_sig, q_nan);
	      q_exp = a_exp;
	    }
	  else if (vec_all_isnanf128 (vfb))
	    {
	      // vfb is NaN
	      q_sign = b_sign;
	      q_sig = (vui128_t) vec_or ((vui32_t) b_sig, q_nan);
	      q_exp = b_exp;
	    }
	  else  // OR an Infinity and a Nonzero finite number
	    {
	      q_sign = vec_xor (a_sign, b_sign);
	      q_exp = q_naninf;
	      q_sig = (vui128_t) q_zero;
	    }
	}
    }
  // Merge sign, significand, and exponent into final result
  q_sig = (vui128_t) vec_or ((vui32_t) q_sig, q_sign);
  result = vec_xsiexpqp (q_sig, q_exp);
#else /* Not P7/8 use libgcc runtime*/
  result = vfa * vfb;
#endif
  return result;
}
#endif
#endif

__binary128
test_vec_xsmulqpo (__binary128 vfa, __binary128 vfb)
{
  return vec_xsmulqpo_inline (vfa, vfb);
}

__binary128
test_vec_xsmaddqpo (__binary128 vfa, __binary128 vfb, __binary128 vfc)
{
  return vec_xsmaddqpo_inline (vfa, vfb, vfc);
}

__binary128
test_vec_xsmsubqpo (__binary128 vfa, __binary128 vfb, __binary128 vfc)
{
  return vec_xsmsubqpo_inline (vfa, vfb, vfc);
}

int
test_check_sig_ovf (vui128_t q_sig)
{
  vui16_t sig_c_mask = vec_splat_u16 (2);
  vui16_t t_sig = vec_splat ((vui16_t) q_sig, VEC_HW_H);
  t_sig = vec_and (t_sig, sig_c_mask);
  return vec_all_eq (t_sig, sig_c_mask);
}

int
test_check_sig_ovf_V0 (vui128_t q_sig)
{
  vui16_t sig_cl_mask = vec_splat_u16 (3);
  vui16_t sig_l_mask = vec_splat_u16 (1);
  vui16_t t_sig = vec_splat ((vui16_t) q_sig, VEC_HW_H);
  t_sig = vec_and (t_sig, sig_cl_mask);
  return vec_all_gt (t_sig, sig_l_mask);
}


__binary128
test_vec_msubqpo (__binary128 vfa, __binary128 vfb, __binary128 vfc)
{
#if defined (_ARCH_PWR9) && (__GNUC__ > 7)
  __binary128 result;
#if defined (__FLOAT128__) && (__GNUC__ > 8)
  /* There is no __builtin for msubqpo, but the compiler should convert
   * this fmaf128 to xsmsubqpo */
  result = __builtin_fmaf128_round_to_odd (vfa, vfb, vec_negf128 (vfc));
#else
  __asm__(
      "xsmsubqpo %0,%1,%2"
      : "+v" (vfc)
      : "v" (vfa), "v" (vfb)
      : );
  result = vfc;
#endif
  return result;
#else
  __binary128 nsrc3;

  nsrc3 = vec_self128 (vec_negf128 (vfc), vfc, vec_isnanf128(vfc));
  return vec_xsmaddqpo_inline (vfa, vfb, nsrc3);
#endif
}


__binary128
test_vec_maddqpo (__binary128 vfa, __binary128 vfb, __binary128 vfc)
{
  __binary128 result;
#if defined (_ARCH_PWR9) && (__GNUC__ > 7)
#if defined (__FLOAT128__) && (__GNUC__ > 8)
  // earlier GCC versions generate extra data moves for this.
  result = __builtin_fmaf128_round_to_odd (vfa, vfb, vfc);
#else
  // No extra data moves here.
  __asm__(
      "xsmaddqpo %0,%1,%2"
      : "+v" (vfc)
      : "v" (vfa), "v" (vfb)
      : );
  result = vfc;
#endif
  return result;
#elif  defined (_ARCH_PWR7)
  vui64_t q_exp, /*a_exp, b_exp,*/ c_exp, x_exp;
  vui128_t q_sig, a_sig, b_sig, c_sig, c_sig_l, p_sig_h, p_sig_l, p_odd;
  vui32_t q_sign, a_sign, b_sign, c_sign;
  vui128_t a_mag, b_mag, c_mag;
  const vui32_t q_zero = CONST_VINT128_W (0, 0, 0, 0);
  const vui32_t q_ones = CONST_VINT128_W (-1, -1, -1, -1);
  //const vui64_t exp_naninf = (vui64_t) { 0x7fff, 0x7fff };
  const vui64_t exp_naninf = vec_mask64_f128exp();
  const vui32_t magmask = vec_mask128_f128mag();

  //a_exp = vec_xsxexpqp (vfa);
  //b_exp = vec_xsxexpqp (vfb);

  // Vector extract the exponents from vfa, vfb
  x_exp = vec_xxxexpqpp (vfa, vfb);
#if 1
  // Vector extract the exponents from vfc
  c_exp = vec_xxxexpqpp (vfc, vfc);
#else
  c_exp = vec_xsxexpqp (vfc);
  c_exp = vec_mrgahd ((vui128_t) c_exp, (vui128_t) c_exp);
#endif
  // Mask off sign bits so can use integers for magnitude compare.
  a_mag = (vui128_t) vec_and_bin128_2_vui32t (vfa, magmask);
  b_mag = (vui128_t) vec_and_bin128_2_vui32t (vfb, magmask);
  a_sign = vec_andc_bin128_2_vui32t (vfa, magmask);
  b_sign = vec_andc_bin128_2_vui32t (vfb, magmask);
#if 0
  c_mag = (vui128_t) vec_and_bin128_2_vui32t (vfc, magmask);
  c_sign = vec_andc_bin128_2_vui32t (vfc, magmask);
#endif
  q_sign = vec_xor (a_sign, b_sign);

//  if (vec_all_isfinitef128 (vfa) && vec_all_isfinitef128 (vfb))
  if (__builtin_expect ((vec_cmpud_all_lt (x_exp, exp_naninf)
      && vec_cmpud_all_lt (c_exp, exp_naninf)), 1))
    {
      const vui64_t exp_dnrm = (vui64_t) q_zero;
      vui64_t exp_min, exp_one, exp_bias;
      vui32_t diff_sign;
      vui128_t p_tmp;
      //vb128_t a_lt_b;
#if 0
#if 1
      c_mag = (vui128_t) vec_and_bin128_2_vui32t (vfc, magmask);
      c_sign = vec_andc_bin128_2_vui32t (vfc, magmask);
#endif
      // check for zero significands in multiply
      if (__builtin_expect (
	    (vec_all_eq((vui32_t ) a_mag, (vui32_t ) q_zero)
	  || vec_all_eq((vui32_t ) b_mag, (vui32_t ) q_zero)),
	  0))
	{ // Multiply by zero + zero, return QP signed zero
	  if (vec_all_eq((vui32_t ) c_mag, (vui32_t ) q_zero))
		{ // Special case of both zero with different sign
		  vui32_t diff_sign = vec_xor (q_sign, c_sign);
		  diff_sign = (vui32_t) vec_setb_sq ((vi128_t) diff_sign);
		  q_sign = vec_sel (q_sign, (vui32_t) q_zero, diff_sign);
		  result = vec_xfer_vui32t_2_bin128 (q_sign);
		}
	  else
	    result = vfc;

	  return result;
	}
#endif
      // const vui64_t exp_min, exp_one = { 1, 1 };
      // exp_min = exp_one = vec_splat_u64 (1);
      { // Extract the significands and insert the Hidden bit
	//const vui32_t q_zero = CONST_VINT128_W(0, 0, 0, 0);
	const vui32_t sigmask = vec_mask128_f128sig ();
	vui32_t a_s32, b_s32/*, c_s32*/;
	vui16_t a_e16, b_e16/*, c_e16*/, x_hidden;
	vb16_t a_norm, b_norm/*, c_norm*/;

	//const vui32_t hidden = vec_mask128_f128Lbit();
	x_hidden = vec_splat_u16(1);
	// Assume that the operands are finite magnitudes
	// Mask off the significands
	// Applying sigmask to orignal inputs can save 2 cycles here
	a_s32 = vec_and_bin128_2_vui32t (vfa, sigmask);
	b_s32 = vec_and_bin128_2_vui32t (vfb, sigmask);
	// But still need a/b_mag for exp extract to clear sign-bit
	// Mask off the exponents in high halfword
	a_e16 = (vui16_t) vec_andc ((vui32_t) a_mag, sigmask);
	b_e16 = (vui16_t) vec_andc ((vui32_t) b_mag, sigmask);
	// Compare exponents for finite i.e. > denomal (q_zero)
	a_norm = vec_cmpgt (a_e16, (vui16_t) q_zero);
	b_norm = vec_cmpgt (b_e16, (vui16_t) q_zero);
	// For Normal QP insert (hidden) L-bit into significand
	a_sig = (vui128_t) vec_sel ((vui16_t) a_s32, x_hidden, a_norm);
	b_sig = (vui128_t) vec_sel ((vui16_t) b_s32, x_hidden, b_norm);
      }
#if 1
#if 1
      c_mag = (vui128_t) vec_and_bin128_2_vui32t (vfc, magmask);
      c_sign = vec_andc_bin128_2_vui32t (vfc, magmask);
#endif
      { // Extract the significands and insert the Hidden bit
	//const vui32_t q_zero = CONST_VINT128_W(0, 0, 0, 0);
	const vui32_t sigmask = vec_mask128_f128sig ();
	vui32_t /*a_s32, b_s32,*/ c_s32;
	vui16_t /*a_e16, b_e16,*/ c_e16, x_hidden;
	vb16_t /*a_norm, b_norm,*/ c_norm;

	//const vui32_t hidden = vec_mask128_f128Lbit();
	x_hidden = vec_splat_u16(1);
	// Assume that the operands are finite magnitudes
	// Mask off the significands
	// Applying sigmask to orignal inputs can save 2 cycles here
	c_s32 = vec_and_bin128_2_vui32t (vfc, sigmask);
	// But still need a/b_mag for exp extract to clear sign-bit
	// Mask off the exponents in high halfword
	c_e16 = (vui16_t) vec_andc ((vui32_t) c_mag, sigmask);
	// Compare exponents for finite i.e. > denomal (q_zero)
	c_norm = vec_cmpgt (c_e16, (vui16_t) q_zero);
	// For Normal QP insert (hidden) L-bit into significand
	c_sig = (vui128_t) vec_sel ((vui16_t) c_s32, x_hidden, c_norm);
      }
#endif
#if 0
      // check for zero significands in multiply
      if (__builtin_expect (
	    (vec_all_eq((vui32_t ) a_mag, (vui32_t ) q_zero)
	  || vec_all_eq((vui32_t ) b_mag, (vui32_t ) q_zero)),
	  0))
	{ // Multiply by zero + zero, return QP signed zero
	  if (vec_all_eq((vui32_t ) c_mag, (vui32_t ) q_zero))
		{ // Special case of both zero with different sign
		  vui32_t diff_sign = vec_xor (q_sign, c_sign);
		  diff_sign = (vui32_t) vec_setb_sq ((vi128_t) diff_sign);
		  q_sign = vec_sel (q_sign, (vui32_t) q_zero, diff_sign);
		  result = vec_xfer_vui32t_2_bin128 (q_sign);
		}
	  else
	    result = vfc;

	  return result;
	}
#endif

      // Precondition the significands before multiply so that the
      // high-order 114-bits (C,L,FRACTION) of the product are right
      // adjusted in p_sig_h. And the Low-order 112-bits are left
      // justified in p_sig_l.
      // Logically this (multiply) step could be moved after the zero
      // test. But this uses a lot of registers and the compiler may
      // see this as register pressure and decide to spill and reload
      // unrelated data around this block.
      // The zero multiply is rare so on average performance is better
      // if we get this started now.
      a_sig = vec_slqi (a_sig, 8);
      b_sig = vec_slqi (b_sig, 8);
      p_sig_l = vec_muludq (&p_sig_h, a_sig, b_sig);
#if 0
#if 1
      c_mag = (vui128_t) vec_and_bin128_2_vui32t (vfc, magmask);
      c_sign = vec_andc_bin128_2_vui32t (vfc, magmask);
#endif
      { // Extract the significands and insert the Hidden bit
	//const vui32_t q_zero = CONST_VINT128_W(0, 0, 0, 0);
	const vui32_t sigmask = vec_mask128_f128sig ();
	vui32_t a_s32, b_s32, c_s32;
	vui16_t a_e16, b_e16, c_e16, x_hidden;
	vb16_t a_norm, b_norm, c_norm;

	//const vui32_t hidden = vec_mask128_f128Lbit();
	x_hidden = vec_splat_u16(1);
	// Assume that the operands are finite magnitudes
	// Mask off the significands
	// Applying sigmask to orignal inputs can save 2 cycles here
	c_s32 = vec_and_bin128_2_vui32t (vfc, sigmask);
	// But still need a/b_mag for exp extract to clear sign-bit
	// Mask off the exponents in high halfword
	c_e16 = (vui16_t) vec_andc ((vui32_t) c_mag, sigmask);
	// Compare exponents for finite i.e. > denomal (q_zero)
	c_norm = vec_cmpgt (c_e16, (vui16_t) q_zero);
	// For Normal QP insert (hidden) L-bit into significand
	c_sig = (vui128_t) vec_sel ((vui16_t) c_s32, x_hidden, c_norm);
      }
#endif
#if 1
      // check for zero significands in multiply
      if (__builtin_expect (
	    (vec_all_eq((vui32_t ) a_sig, (vui32_t ) q_zero)
	  || vec_all_eq((vui32_t ) b_sig, (vui32_t ) q_zero)),
	  0))
	{ // Multiply by zero + zero, return QP signed zero
	  if (vec_all_eq((vui32_t ) c_sig, (vui32_t ) q_zero))
		{ // Special case of both zero with different sign
		  vui32_t diff_sign = vec_xor (q_sign, c_sign);
		  diff_sign = (vui32_t) vec_setb_sq ((vi128_t) diff_sign);
		  q_sign = vec_sel (q_sign, (vui32_t) q_zero, diff_sign);
		  result = vec_xfer_vui32t_2_bin128 (q_sign);
		}
	  else
	    result = vfc;

	  return result;
	}
#endif
      // const vui64_t exp_min, exp_one = { 1, 1 };
      exp_min = exp_one = vec_splat_u64 (1);
      //const vui64_t exp_bias = (vui64_t) { 0x3fff, 0x3fff };
      exp_bias = (vui64_t) vec_srhi ((vui16_t) exp_naninf, 1);
	{ // Compute product exponent q_exp
	  // Operand exponents should >= Emin for computation
	  vb64_t exp_mask;
	  exp_mask = vec_cmpequd (x_exp, exp_dnrm);
	  x_exp = vec_selud (x_exp, exp_min, (vb64_t) exp_mask);
	  // sum exponents across x_exp
	  q_exp = vec_addudm (x_exp, vec_swapd (x_exp));
	  // Sum includes 2 x exp_bias, So subtract 1 x exp_bias
	  q_exp = vec_subudm (q_exp, exp_bias);
	  exp_mask = vec_cmpequd (c_exp, exp_dnrm);
	  c_exp = vec_selud (c_exp, exp_min, (vb64_t) exp_mask);
	}
#if 1
      // Check for carry; shift right 1 and adjust exp +1
	{
	  vb128_t carry_mask;
	  vui128_t sig_h, sig_l;
	  // Test Carry-bit (greater than L-bit)
	  vui16_t sig_l_mask = vec_splat_u16(1);
	  vui16_t t_sig = vec_splat ((vui16_t) p_sig_h, VEC_HW_H);
	  carry_mask = (vb128_t) vec_cmpgt (t_sig, sig_l_mask);
	  // Shift double quadword right 1 bit
	  p_tmp = vec_sldqi (p_sig_h, p_sig_l, 120);
	  sig_h = vec_srqi (p_sig_h, 1);
	  sig_l = vec_slqi (p_tmp, 7);
	  // Increment the exponent
	  x_exp = vec_addudm (q_exp, exp_one);
	  // Select original or normalized exp/sig
	  p_sig_h = vec_seluq (p_sig_h, sig_h, carry_mask);
	  p_sig_l = vec_seluq (p_sig_l, sig_l, carry_mask);
	  q_exp = vec_selud (q_exp, x_exp, (vb64_t) carry_mask);
	}
#endif
      // We have the product in q_sign, q_exp, p_sig_h/p_sig_l
      // And the addend (vfc) in c_sign, c_exp,  c_sig/c_sig_l
      c_sig_l = (vui128_t) q_zero;
      q_sig = p_sig_h;
      // Generation sign difference for signed 0.0

      // If sign(vfa) != sign(vfb) will need to:
      // 1) Subtract instead of add significands
      // 2) Generate signed zeros

      // If magnitude(prod) >  magnitude(c) will need to swap prod/c, later
      // a_lt_b = vec_cmpltuq (q_sig, c_sig);
      diff_sign = (vui32_t) vec_cmpneuq ((vui128_t) q_sign, (vui128_t) c_sign);
      // Simply vfc == +-0.0. Treat as p_exp == c_exp
      if (vec_cmpud_all_eq (q_exp, c_exp)
       || vec_all_eq((vui32_t ) c_sig, (vui32_t ) q_zero))
	{
	  vui128_t carry;

	  // Simply vfc == +-0.0. Treat as p_sign == c_sign
	  if (vec_all_eq (q_sign, c_sign)
	   || vec_all_eq((vui32_t ) c_sig, (vui32_t ) q_zero))
	    { // Same sign, simple add
	      q_sig = vec_adduqm (p_sig_h, c_sig);
	      p_sig_h = q_sig;
	      // q_sign = q_sign;
	    }
	  else
	    { // different sign, subtract smallest from largest magnitude
	      const vui64_t exp_112 = vec_const64_f128_112();

	      if (vec_cmpuq_all_lt (q_sig, c_sig))
		{
		  carry = vec_subcuq (c_sig_l, p_sig_l);
		  p_sig_l = vec_subuqm (c_sig_l, p_sig_l);
		  q_sig = vec_subeuqm (c_sig, p_sig_h, carry);
		  p_sig_h = q_sig;
		  q_sign = c_sign;
		}
	      else
		{
		  q_sig = vec_subuqm (q_sig, c_sig);
		  p_sig_h = q_sig;
		  // q_sign = q_sign;
		}

	      if (__builtin_expect (vec_cmpuq_all_eq (q_sig, (vui128_t) q_zero),
				    0))
		{
		  if (vec_cmpuq_all_eq (p_sig_l, (vui128_t) q_zero))
		    { // Special case of both zero with different sign
		      q_sign = vec_sel (q_sign, (vui32_t) q_zero, diff_sign);
		      //q_sign = vec_xor (q_sign, c_sign);
		      return vec_xfer_vui32t_2_bin128 (q_sign);
		    }

		  // If p_sig_h == 0 after subtract and p_sig_l != 0
		  // Need to pre-normalize if we can, because the code that
		  // follows is not expecting delta exp > 112
		  // But must maintain q_exp >= E_min
		  if (vec_cmpud_all_gt ((vui64_t) q_exp, exp_112))
		    {
		      vui128_t t_sig;
		      t_sig = (vui128_t) vec_sld ((vui32_t) p_sig_h,
						  (vui32_t) p_sig_l, 14);
		      p_sig_l = vec_slqi (p_sig_l, 112);
		      p_sig_h = t_sig;
		      q_exp = vec_subudm (q_exp, exp_112);
		      q_sig = p_sig_h;
		    }
		}
	    }
	}
      else
	{
	  // 2a) If p_exp > c_exp then
	  //if (vec_cmpuq_all_ne (c_sig, (vui128_t) q_zero))
	    { // and c_sig != zero
	      //     any bits shifted out of b_sig are ORed into the X-bit
	      if (vec_cmpud_all_lt (c_exp, q_exp))
		{ //     shift (c_sig) right by (q_exp - c_exp)
		  vui64_t d_exp, l_exp;
		  vui128_t t_sig, carry;
		  vb128_t exp_mask;
		  const vui64_t exp_128 = vec_const64_f128_128 ();
		  const vui64_t exp_256 = vec_addudm (exp_128, exp_128);

		  d_exp = vec_subudm (q_exp, c_exp);
		  exp_mask = (vb128_t) vec_cmpltud (d_exp, exp_128);
		  if (vec_cmpud_all_lt (d_exp, exp_128))
		    {
		      l_exp = vec_subudm (exp_128, d_exp);
		      t_sig = vec_srq (c_sig, (vui128_t) d_exp);
		      c_sig_l = vec_slq (c_sig, (vui128_t) l_exp);
		      t_sig = vec_seluq ((vui128_t) q_zero, t_sig, exp_mask);
		      //c_sig_l = vec_seluq (c_sig, c_sig_l, exp_mask);
		    }
		  else
		    {
		      if (vec_cmpud_all_lt (d_exp, exp_256))
			{
			  d_exp = vec_subudm (d_exp, exp_128);
			  if (vec_cmpud_all_eq (d_exp, (vui64_t) q_zero))
			    {
			      // if (d_exp == 128) then shift 128 not 0 bits
			      c_sig_l = c_sig;
			      c_sig = (vui128_t) q_zero;
			      p_odd = (vui128_t) q_zero;
			    }
			  else
			    {
			      l_exp = vec_subudm (exp_128, d_exp);
			      t_sig = vec_srq (c_sig, (vui128_t) d_exp);
			      c_sig_l = vec_slq (c_sig, (vui128_t) l_exp);
			      p_odd = vec_addcuq (c_sig_l, (vui128_t) q_ones);
			      c_sig_l = (vui128_t) vec_or ((vui32_t) t_sig,
							   (vui32_t) p_odd);
			      c_sig = (vui128_t) q_zero;
			    }
			}
		      else
			{
			  p_odd = vec_addcuq (c_sig, (vui128_t) q_ones);
			  c_sig = (vui128_t) q_zero;
			  c_sig_l = p_odd;
			}
		      t_sig = c_sig;
		    }

		  if (vec_all_eq(q_sign, c_sign))
		    { // Same sign, simple add
		      carry = vec_addcuq (c_sig_l, p_sig_l);
		      p_sig_l = vec_adduqm (c_sig_l, p_sig_l);
		      q_sig = vec_addeuqm (t_sig, p_sig_h, carry);
		      p_sig_h = q_sig;
		      // q_sign = q_sign;
		    }
		  else
		    {
		      carry = vec_subcuq (p_sig_l, c_sig_l);
		      p_sig_l = vec_subuqm (p_sig_l, c_sig_l);
		      q_sig = vec_subeuqm (p_sig_h, t_sig, carry);
		      p_sig_h = q_sig;
		      // q_sign = q_sign;
		    }

		}
	      else
		{
		  // 2b) If q_exp < c_exp
		  // Shift p_sig_h | p_sig_l right by (c_exp - q_exp)
		  vui64_t d_exp, l_exp;
		  vui128_t t_sig, carry;
		  //vb128_t exp_mask;
		  const vui64_t exp_128 = vec_const64_f128_128 ();
		  const vui64_t exp_256 = vec_addudm (exp_128, exp_128);

		  d_exp = vec_subudm (c_exp, q_exp);
		  //exp_mask = (vb128_t) vec_cmpltud (d_exp, exp_128);
		  if (vec_cmpud_all_lt (d_exp, exp_128))
		    {
		      // if (d_exp < 128) then shift right 1-127 bits
		      vui128_t p_sig_x;
		      l_exp = vec_subudm (exp_128, d_exp);
		      // Isolate bits shifted out of P_sig_l for x-bit
		      p_sig_x = vec_slq (p_sig_l, (vui128_t) l_exp);
		      t_sig = vec_srq (p_sig_h, (vui128_t) d_exp);
		      p_sig_l = vec_sldq (p_sig_h, p_sig_l, (vui128_t) l_exp);
		      p_odd = vec_addcuq (p_sig_x, (vui128_t) q_ones);
		      p_sig_l = (vui128_t) vec_or ((vui32_t) p_sig_l,
						   (vui32_t) p_odd);
		      p_sig_h = t_sig;
		    }
		  else
		    {
		      if (vec_cmpud_all_lt (d_exp, exp_256))
			{
			  d_exp = vec_subudm (d_exp, exp_128);
			  if (vec_cmpud_all_eq (d_exp, (vui64_t) q_zero))
			    {
			      // if (d_exp == 128) then shift 128 (not 0 bits)
			      p_odd = vec_addcuq (p_sig_l, (vui128_t) q_ones);
			      p_sig_l = (vui128_t) vec_or ((vui32_t) p_sig_h,
							   (vui32_t) p_odd);
			      p_sig_h = (vui128_t) q_zero;
			    }
			  else
			    {
			      // if (d_exp > 128) then shift right 128+ bits
			      vui128_t p_sig_x;
			      l_exp = vec_subudm (exp_128, d_exp);
			      t_sig = vec_srq (p_sig_h, (vui128_t) d_exp);
			      p_sig_x = vec_slq (p_sig_h, (vui128_t) l_exp);
			      p_sig_x = (vui128_t) vec_or ((vui32_t) p_sig_x,
							   (vui32_t) p_sig_l);
			      p_odd = vec_addcuq (p_sig_x, (vui128_t) q_ones);
			      p_sig_l = (vui128_t) vec_or ((vui32_t) t_sig,
							   (vui32_t) p_odd);
			      p_sig_h = (vui128_t) q_zero;
			    }
			}
		      else
			{ // Shift >= 256, Accumulate all bits from p_sig_h | p_sig_l into x-bit
			  t_sig = (vui128_t) vec_or ((vui32_t) p_sig_h,
						     (vui32_t) p_sig_l);
			  p_odd = vec_addcuq (t_sig, (vui128_t) q_ones);
			  // Zero all product bits (includeing GR) while preserving X-bit
			  p_sig_h = (vui128_t) q_zero;
			  p_sig_l = p_odd;
			}
		    }

		  c_sig_l = (vui128_t) q_zero;
		  //if (vec_cmpuq_all_ne (c_sig, (vui128_t) q_zero))
		    {
		      if (vec_all_eq(q_sign, c_sign))
			{ // Same sign, simple add
			  carry = vec_addcuq (c_sig_l, p_sig_l);
			  p_sig_l = vec_adduqm (c_sig_l, p_sig_l);
			  p_sig_h = vec_addeuqm (c_sig, p_sig_h, carry);
			  //q_sig = p_sig_h;
			  q_sign = c_sign;
			}
		      else
			{
			  carry = vec_subcuq (c_sig_l, p_sig_l);
			  p_sig_l = vec_subuqm (c_sig_l, p_sig_l);
			  p_sig_h = vec_subeuqm (c_sig, p_sig_h, carry);
			  //q_sig = p_sig_h;
			  q_sign = c_sign;
			}
		    }
		  q_sig = p_sig_h;
		  q_exp = c_exp;
		}
	    }

	}

      // check for significand overfloaw again
	{
	  vb128_t carry_mask;
	  vui128_t sig_h, sig_l;
	  // Test Carry-bit (greater than L-bit)
	  vui16_t sig_l_mask = vec_splat_u16(1);
	  vui16_t t_sig = vec_splat ((vui16_t) p_sig_h, VEC_HW_H);
	  carry_mask = (vb128_t) vec_cmpgt (t_sig, sig_l_mask);
	  // Shift double quadword right 1 bit
	  p_tmp = vec_sldqi (p_sig_h, p_sig_l, 120);
	  sig_h = vec_srqi (p_sig_h, 1);
	  sig_l = vec_slqi (p_tmp, 7);
	  // Increment the exponent
	  x_exp = vec_addudm (q_exp, exp_one);
	  // Select original or normalized exp/sig
	  p_sig_h = vec_seluq (p_sig_h, sig_h, carry_mask);
	  p_sig_l = vec_seluq (p_sig_l, sig_l, carry_mask);
	  q_exp = vec_selud (q_exp, x_exp, (vb64_t) carry_mask);
	}
      // There are two cases for denormal
      // 1) The sum of unbiased exponents is less the E_min (tiny).
      // 2) The significand is less then 1.0 (C and L-bits are zero).
      //  2a) The exponent is > E_min
      //  2b) The exponent is == E_min
      //
      // Check for Tiny exponent
      if (__builtin_expect (
	  (vec_cmpsd_all_lt ((vi64_t) q_exp, (vi64_t) exp_min)), 0))
	{
	  //const vui64_t exp_128 = (vui64_t) { 128, 128 };
	  const vui64_t exp_128 = vec_const64_f128_128();
	  // const vui64_t too_tiny = (vui64_t) { 116, 116 };
	  const vui64_t too_tiny = vec_const64_f128_116();
	  // const vui32_t xmask = CONST_VINT128_W(0x1fffffff, -1, -1, -1);
	  vui32_t xmask = vec_mask128_f128Xbits ();
	  vui32_t tmp;

	  // Intermediate result is tiny, unbiased exponent < -16382
	  //x_exp = vec_subudm ((vui64_t) exp_tiny, q_exp);
	  x_exp = vec_subudm (exp_min, q_exp);

	  if (vec_cmpud_all_gt ((vui64_t) x_exp, too_tiny))
	    {
	      // Intermediate result is too tiny, the shift will
	      // zero the fraction and the GR-bit leaving only the
	      // Sticky bit. The X-bit needs to include all bits
	      // from p_sig_h and p_sig_l
	      p_sig_l = vec_srqi (p_sig_l, 8);
	      p_sig_l = (vui128_t) vec_or ((vui32_t) p_sig_l,
					   (vui32_t) p_sig_h);
	      // generate a carry into bit-2 for any nonzero bits 3-127
	      p_sig_l = vec_adduqm (p_sig_l, (vui128_t) xmask);
	      q_sig = (vui128_t) q_zero;
	      p_sig_l = (vui128_t) vec_andc ((vui32_t) p_sig_l, xmask);
	    }
	  else
	    { // Normal tiny, right shift may loose low order bits
	      // from p_sig_l. So collect any 1-bits below GRX and
	      // OR them into the X-bit, before the right shift.
	      vui64_t l_exp;

	      // Propagate low order bits into the sticky bit
	      // GRX left adjusted in p_sig_l
	      // Issolate bits below GDX (bits 3-128).
	      tmp = vec_and ((vui32_t) p_sig_l, xmask);
	      // generate a carry into bit-2 for any nonzero bits 3-127
	      tmp = (vui32_t) vec_adduqm ((vui128_t) tmp, (vui128_t) xmask);
	      // Or this with the X-bit to propagate any sticky bits into X
	      p_sig_l = (vui128_t) vec_or ((vui32_t) p_sig_l, tmp);
	      p_sig_l = (vui128_t) vec_andc ((vui32_t) p_sig_l, xmask);

	      l_exp = vec_subudm (exp_128, x_exp);
	      p_sig_l = vec_sldq (p_sig_h, p_sig_l, (vui128_t) l_exp);
	      p_sig_h = vec_srq (p_sig_h, (vui128_t) x_exp);
	      q_sig = p_sig_h;
	    }
	  // Set the exponent for denormal
	  q_exp = exp_dnrm;
	}
      // Exponent is not tiny but significand may be denormal
      // Isolate sig CL bits and compare
      vui16_t t_sig = vec_splat ((vui16_t) p_sig_h, VEC_HW_H);
      if (__builtin_expect ((vec_all_eq(t_sig, (vui16_t ) q_zero)), 0))
	{
	  // Is below normal range. This can happen when
	  // multiplying a denormal by a normal.
	  // So try to normalize the significand.
	  //const vui64_t exp_15 = { 15, 15 };
	  const vui64_t exp_15 = vec_splat_u64 (15);
	  vui64_t c_exp, d_exp;
	  vui128_t c_sig;
	  vb64_t exp_mask;

#if 0
	  const vui64_t exp_112 = vec_const64_f128_112();
	  // Not sure this test is needed here
	  if (vec_cmpuq_all_eq (p_sig_h, (vui128_t) q_zero)
	      && vec_cmpud_all_gt ((vui64_t) q_exp, exp_112))
	    {
	      vui128_t t_sig;
	      t_sig = (vui128_t) vec_sld ((vui32_t) p_sig_h, (vui32_t) p_sig_l, 14);
	      p_sig_l = vec_slqi (p_sig_l, 112);
	      p_sig_h = t_sig;
	      q_exp = vec_subudm (q_exp, exp_112);
	      print_vint128x (" exp (adj?):", (vui128_t) q_exp);
	      print_vint128x (" sig (lsht):", (vui128_t) p_sig_h);
	      print_vint128x ("     (lsht):", (vui128_t) p_sig_l);
	      q_sig = p_sig_h;
	    }
#endif
	  c_sig = vec_clzq (p_sig_h);
	  c_exp = vec_splatd ((vui64_t) c_sig, VEC_DW_L);
	  c_exp = vec_subudm (c_exp, exp_15);
	  d_exp = vec_subudm (q_exp, exp_min);
	  d_exp = vec_minud (c_exp, d_exp);
	  exp_mask = vec_cmpgtud (q_exp, c_exp);

	  // Intermediate result <= tiny, unbiased exponent <= -16382
	  if (vec_cmpsd_all_gt ((vi64_t) q_exp, (vi64_t) exp_min))
	    {
	      // Try to normalize the significand.
	      p_sig_h = vec_sldq (p_sig_h, p_sig_l, (vui128_t) d_exp);
	      p_sig_l = vec_slq (p_sig_l, (vui128_t) d_exp);
	      q_sig = p_sig_h;
	      // Compare computed exp to shift count to normalize.
	      //exp_mask = vec_cmpgtud (q_exp, c_exp);
	      q_exp = vec_subudm (q_exp, d_exp);
	      q_exp = vec_selud (exp_dnrm, q_exp, exp_mask);
	    }
	  else
	    { // sig is denormal range (L-bit is 0). Set exp to zero.
	      q_exp = exp_dnrm;
	    }
	}

      // Round to odd from lower product bits
      p_odd = vec_addcuq (p_sig_l, (vui128_t) q_ones);
      q_sig = (vui128_t) vec_or ((vui32_t) q_sig, (vui32_t) p_odd);

      // Check for exponent overflow -> __FLT128_MAX__ (round to odd)
      if (__builtin_expect ((vec_cmpud_all_ge ( q_exp, exp_naninf)), 0))
	{
	  // Intermediate result is huge, unbiased exponent > 16383
	  // so return __FLT128_MAX__ with the appropriate sign.
	  const vui32_t f128_max = CONST_VINT128_W(0x7ffeffff, -1, -1, -1);
	  vui32_t f128_smax = vec_or ((vui32_t) f128_max, q_sign);
	  return vec_xfer_vui32t_2_bin128 (f128_smax);
	}
      else // combine sign, exp, and significand for return
	{
	  // Merge sign, significand, and exponent into final result
	  q_sig = (vui128_t) vec_or ((vui32_t) q_sig, q_sign);
	  vui32_t tmp, t128;
	  // convert DW exp_naninf to QW expmask
	  vui32_t expmask = vec_sld ((vui32_t) exp_naninf, q_zero, 14);
	  // convert q_exp from DW to QW for QP format
	  tmp = vec_sld ((vui32_t) q_exp, q_zero, 14);
	  t128 = vec_sel ((vui32_t) q_sig, tmp, expmask);
	  result = vec_xfer_vui32t_2_bin128 (t128);
	  return result;
	}
    }
  else
    { // One, two, or three operands are NaN or Infinity
      //const vui32_t q_nan = CONST_VINT128_W(0x00008000, 0, 0, 0);
      vui32_t q_nan = vec_mask128_f128Qbit ();
      vui32_t q_inf = vec_mask128_f128exp ();
#if 1
      c_mag = (vui128_t) vec_and_bin128_2_vui32t (vfc, magmask);
      c_sign = vec_andc_bin128_2_vui32t (vfc, magmask);
#endif
      // the product operands are NaN or Infinity
      if (vec_cmpud_any_eq (x_exp, exp_naninf))
	{
	  // One or more operands are NaN
	  if (vec_all_isnanf128 (vfa))
	    {
	      // vfa is NaN, Convert vfa to QNaN and return
	      vui32_t vf128 = vec_or_bin128_2_vui32t (vfa, q_nan);
	      return vec_xfer_vui32t_2_bin128 (vf128);
	    }
	  else if (vec_all_isnanf128 (vfb))
	    {
	      if (vec_all_isnanf128 (vfc))
		{
		  // vfb is NaN, Convert vfc to QNaN and return
		  vui32_t vf128 = vec_or_bin128_2_vui32t (vfc, q_nan);
		  return vec_xfer_vui32t_2_bin128 (vf128);
		}
	      else
		{
		  // vfb is NaN, Convert vfb to QNaN and return
		  vui32_t vf128 = vec_or_bin128_2_vui32t (vfb, q_nan);
		  return vec_xfer_vui32t_2_bin128 (vf128);
		}
	    }
	  else  // Or one or both operands are Infinity
	    {
	      if (vec_cmpud_all_eq (x_exp, (vui64_t) exp_naninf))
		{
		  // Infinity x Infinity == signed Infinity
		  q_sig = (vui128_t) q_inf;
		}
	      else
		{
		  // One each Infinity/Finite value, check for 0.0
		  if (vec_cmpuq_all_eq (a_mag, (vui128_t) q_zero)
		      || vec_cmpuq_all_eq (b_mag, (vui128_t) q_zero))
		    {
		      if (vec_all_isnanf128 (vfc))
			{
			      // vfb is NaN, Convert vfb to QNaN and return
			      vui32_t vf128 = vec_or_bin128_2_vui32t (vfc, q_nan);
			      return vec_xfer_vui32t_2_bin128 (vf128);
			}
		      else
		      // Inifinty x Zero is Default Quiet NaN
		        return vec_const_nanf128 ();
		    }
		  else // an Infinity and a Nonzero finite number
		    {
		      // Return Infinity with product sign.
		      q_sig = (vui128_t) q_inf;
		    }
		}
	      // At this point we have a signed infinity product
	      // Need to check addend for nan/infinity
	      if (vec_all_isnanf128 (vfc))
		{
		  // vfc is NaN, Convert vfc to QNaN and return
		  vui32_t vf128 = vec_or_bin128_2_vui32t (vfc, q_nan);
		  return vec_xfer_vui32t_2_bin128 (vf128);
		}
	      else  // Or addend is also Infinity
		{
		  if (vec_cmpud_all_eq (c_exp, exp_naninf)
		      && vec_cmpud_any_ne ((vui64_t) q_sign, (vui64_t) c_sign))
		    { // Both operands infinity and opposite sign
		      // Inifinty + Infinity (opposite sign) is Default Quiet NaN
		      return vec_const_nanf128 ();
		    }
		    }
	      // Merge sign, exp/sig into final result
	      q_sig = (vui128_t) vec_or ((vui32_t) q_sig, q_sign);
	      return vec_xfer_vui128t_2_bin128 (q_sig);
	    }
	}
      else // The product is finite but the addend is not
	{
	      if (vec_all_isnanf128 (vfc))
		{
		  // vfa is NaN, Convert vfa to QNaN and return
		  vui32_t vf128 = vec_or_bin128_2_vui32t (vfc, q_nan);
		  return vec_xfer_vui32t_2_bin128 (vf128);
		}
	      else  // Or addend is Infinity
		{
		  // return infinity
		  return vfc;
		}
	}
    }
#else
  result = vfa * vfb;
#endif
  return result;
}

__binary128
test_vec_mulqpo (__binary128 vfa, __binary128 vfb)
{
  __binary128 result;
#if defined (_ARCH_PWR9) && (__GNUC__ > 7)
#if defined (__FLOAT128__) && (__GNUC__ > 8)
  // earlier GCC versions generate extra data moves for this.
  result = __builtin_mulf128_round_to_odd (vfa, vfb);
#else
  // No extra data moves here.
  __asm__(
      "xsmulqpo %0,%1,%2"
      : "=v" (result)
      : "v" (vfa), "v" (vfb)
      : );
#endif
  return result;
#elif  defined (_ARCH_PWR7)
  vui64_t q_exp, x_exp;
  vui128_t q_sig, a_sig, b_sig, p_sig_h, p_sig_l, p_odd;
  vui32_t q_sign, a_sign, b_sign;
  vui128_t a_mag, b_mag;
  const vui32_t q_zero = CONST_VINT128_W (0, 0, 0, 0);
  const vui32_t q_ones = CONST_VINT128_W (-1, -1, -1, -1);
  //const vui64_t exp_naninf = (vui64_t) { 0x7fff, 0x7fff };
  const vui64_t exp_naninf = vec_mask64_f128exp();
  const vui32_t magmask = vec_mask128_f128mag();

  // Vector extract the exponents from vfa, vfb
  x_exp = vec_xxxexpqpp (vfa, vfb);
  // Mask off sign bits so can use integers for magnitude compare.
  a_mag = (vui128_t) vec_and_bin128_2_vui32t (vfa, magmask);
  b_mag = (vui128_t) vec_and_bin128_2_vui32t (vfb, magmask);
  a_sign = vec_andc_bin128_2_vui32t (vfa, magmask);
  b_sign = vec_andc_bin128_2_vui32t (vfb, magmask);
  q_sign = vec_xor (a_sign, b_sign);

//  if (vec_all_isfinitef128 (vfa) && vec_all_isfinitef128 (vfb))
  if (__builtin_expect (vec_cmpud_all_lt (x_exp, exp_naninf), 1))
    {
      const vui64_t exp_dnrm = (vui64_t) q_zero;
      vui64_t exp_min, exp_one, exp_bias;
      vui128_t p_tmp;
      // const vui64_t exp_min, exp_one = { 1, 1 };
      // exp_min = exp_one = vec_splat_u64 (1);
      { // Extract the significands and insert the Hidden bit
	//const vui32_t q_zero = CONST_VINT128_W(0, 0, 0, 0);
	const vui32_t sigmask = vec_mask128_f128sig ();
	vui32_t a_s32, b_s32;
	vui16_t a_e16, b_e16, x_hidden;
	vb16_t a_norm, b_norm;

	//const vui32_t hidden = vec_mask128_f128Lbit();
	x_hidden = vec_splat_u16(1);
	// Assume that the operands are finite magnitudes
	// Mask off the significands
	// Applying sigmask to orignal inputs can save 2 cycles here
	a_s32 = vec_and_bin128_2_vui32t (vfa, sigmask);
	b_s32 = vec_and_bin128_2_vui32t (vfb, sigmask);
	// But still need a/b_mag for exp extract to clear sign-bit
	// Mask off the exponents in high halfword
	a_e16 = (vui16_t) vec_andc ((vui32_t) a_mag, sigmask);
	b_e16 = (vui16_t) vec_andc ((vui32_t) b_mag, sigmask);
	// Compare exponents for finite i.e. > denomal (q_zero)
	a_norm = vec_cmpgt (a_e16, (vui16_t) q_zero);
	b_norm = vec_cmpgt (b_e16, (vui16_t) q_zero);
	// For Normal QP insert (hidden) L-bit into significand
	a_sig = (vui128_t) vec_sel ((vui16_t) a_s32, x_hidden, a_norm);
	b_sig = (vui128_t) vec_sel ((vui16_t) b_s32, x_hidden, b_norm);
      }

      // Precondition the significands before multiply so that the
      // high-order 114-bits (C,L,FRACTION) of the product are right
      // adjusted in p_sig_h. And the Low-order 112-bits are left
      // justified in p_sig_l.
      // Logically this (multiply) step could be moved after the zero
      // test. But this uses a lot of registers and the compiler may
      // see this as register pressure and decide to spill and reload
      // unrelated data around this block.
      // The zero multiply is rare so on average performance is better
      // if we get this started now.
      a_sig = vec_slqi (a_sig, 8);
      b_sig = vec_slqi (b_sig, 8);
      p_sig_l = vec_muludq (&p_sig_h, a_sig, b_sig);

      // check for zero significands in multiply
      if (__builtin_expect (
	    (vec_all_eq((vui32_t ) a_sig, (vui32_t ) q_zero)
	  || vec_all_eq((vui32_t ) b_sig, (vui32_t ) q_zero)),
	  0))
	{ // Multiply by zero, return QP signed zero
	  result = vec_xfer_vui32t_2_bin128 (q_sign);
	  return result;
	}

      // const vui64_t exp_min, exp_one = { 1, 1 };
      exp_min = exp_one = vec_splat_u64 (1);
      //const vui64_t exp_bias = (vui64_t) { 0x3fff, 0x3fff };
      exp_bias = (vui64_t) vec_srhi ((vui16_t) exp_naninf, 1);
	{ // Compute product exponent q_exp
	  // Operand exponents should >= Emin for computation
	  vb64_t exp_mask;
	  exp_mask = vec_cmpequd (x_exp, exp_dnrm);
	  x_exp = vec_selud (x_exp, exp_min, (vb64_t) exp_mask);
	  // sum exponents across x_exp
	  q_exp = vec_addudm (x_exp, vec_swapd (x_exp));
	  // Sum includes 2 x exp_bias, So subtract 1 x exp_bias
	  q_exp = vec_subudm (q_exp, exp_bias);
	}

      // Check for carry; shift right 1 and adjust exp +1
	{
	  vb128_t carry_mask;
	  vui128_t sig_h, sig_l;
	  // Test Carry-bit (greater than L-bit)
	  vui16_t sig_l_mask = vec_splat_u16(1);
	  vui16_t t_sig = vec_splat ((vui16_t) p_sig_h, VEC_HW_H);
	  carry_mask = (vb128_t) vec_cmpgt (t_sig, sig_l_mask);
	  // Shift double quadword right 1 bit
	  p_tmp = vec_sldqi (p_sig_h, p_sig_l, 120);
	  sig_h = vec_srqi (p_sig_h, 1);
	  sig_l = vec_slqi (p_tmp, 7);
	  // Increment the exponent
	  x_exp = vec_addudm (q_exp, exp_one);
	  // Select original or normalized exp/sig
	  p_sig_h = vec_seluq (p_sig_h, sig_h, carry_mask);
	  p_sig_l = vec_seluq (p_sig_l, sig_l, carry_mask);
	  q_exp = vec_selud (q_exp, x_exp, (vb64_t) carry_mask);
	}

      // There are two cases for denormal
      // 1) The sum of unbiased exponents is less the E_min (tiny).
      // 2) The significand is less then 1.0 (C and L-bits are zero).
      //  2a) The exponent is > E_min
      //  2b) The exponent is == E_min
      //
      q_sig = p_sig_h;
      // Check for Tiny exponent
      if (__builtin_expect (
	  (vec_cmpsd_all_lt ((vi64_t) q_exp, (vi64_t) exp_min)), 0))
	{
	  const vui64_t exp_128 = vec_const64_f128_128();
	  const vui64_t too_tiny = vec_const64_f128_116();
	  // const vui32_t xmask = CONST_VINT128_W(0x1fffffff, -1, -1, -1);
	  vui32_t xmask = vec_mask128_f128Xbits ();
	  vui32_t tmp;

	  // Intermediate result is tiny, unbiased exponent < -16382
	  //x_exp = vec_subudm ((vui64_t) exp_tiny, q_exp);
	  x_exp = vec_subudm (exp_min, q_exp);

	  if (vec_cmpud_all_gt ((vui64_t) x_exp, too_tiny))
	    {
	      // Intermediate result is too tiny, the shift will
	      // zero the fraction and the GR-bit leaving only the
	      // Sticky bit. The X-bit needs to include all bits
	      // from p_sig_h and p_sig_l
	      p_sig_l = vec_srqi (p_sig_l, 8);
	      p_sig_l = (vui128_t) vec_or ((vui32_t) p_sig_l,
					   (vui32_t) p_sig_h);
	      // generate a carry into bit-2 for any nonzero bits 3-127
	      p_sig_l = vec_adduqm (p_sig_l, (vui128_t) xmask);
	      q_sig = (vui128_t) q_zero;
	      p_sig_l = (vui128_t) vec_andc ((vui32_t) p_sig_l, xmask);
	    }
	  else
	    { // Normal tiny, right shift may loose low order bits
	      // from p_sig_l. So collect any 1-bits below GRX and
	      // OR them into the X-bit, before the right shift.
	      vui64_t l_exp;

	      // Propagate low order bits into the sticky bit
	      // GRX left adjusted in p_sig_l
	      // Issolate bits below GDX (bits 3-128).
	      tmp = vec_and ((vui32_t) p_sig_l, xmask);
	      // generate a carry into bit-2 for any nonzero bits 3-127
	      tmp = (vui32_t) vec_adduqm ((vui128_t) tmp, (vui128_t) xmask);
	      // Or this with the X-bit to propagate any sticky bits into X
	      p_sig_l = (vui128_t) vec_or ((vui32_t) p_sig_l, tmp);
	      p_sig_l = (vui128_t) vec_andc ((vui32_t) p_sig_l, xmask);

	      l_exp = vec_subudm (exp_128, x_exp);
	      p_sig_l = vec_sldq (p_sig_h, p_sig_l, (vui128_t) l_exp);
	      p_sig_h = vec_srq (p_sig_h, (vui128_t) x_exp);
	      q_sig = p_sig_h;
	    }
	  // Set the exponent for denormal
	  q_exp = exp_dnrm;
	}
      // Exponent is not tiny but significand may be denormal
      // Isolate sig CL bits and compare
      vui16_t t_sig = vec_splat ((vui16_t) p_sig_h, VEC_HW_H);
      if (__builtin_expect ((vec_all_eq(t_sig, (vui16_t ) q_zero)), 0))
	{
	  // Is below normal range. This can happen when
	  // multiplying a denormal by a normal.
	  // So try to normalize the significand.
	  //const vui64_t exp_15 = { 15, 15 };
	  const vui64_t exp_15 = vec_splat_u64 (15);
	  vui64_t c_exp, d_exp;
	  vui128_t c_sig;
	  vb64_t exp_mask;
	  c_sig = vec_clzq (p_sig_h);
	  c_exp = vec_splatd ((vui64_t) c_sig, VEC_DW_L);
	  c_exp = vec_subudm (c_exp, exp_15);
	  d_exp = vec_subudm (q_exp, exp_min);
	  d_exp = vec_minud (c_exp, d_exp);
	  exp_mask = vec_cmpgtud (q_exp, c_exp);

	  // Intermediate result <= tiny, unbiased exponent <= -16382
	  if (vec_cmpsd_all_gt ((vi64_t) q_exp, (vi64_t) exp_min))
	    {
	      // Try to normalize the significand.
	      p_sig_h = vec_sldq (p_sig_h, p_sig_l, (vui128_t) d_exp);
	      p_sig_l = vec_slq (p_sig_l, (vui128_t) d_exp);
	      q_sig = p_sig_h;
	      // Compare computed exp to shift count to normalize.
	      //exp_mask = vec_cmpgtud (q_exp, c_exp);
	      q_exp = vec_subudm (q_exp, d_exp);
	      q_exp = vec_selud (exp_dnrm, q_exp, exp_mask);
	    }
	  else
	    { // sig is denormal range (L-bit is 0). Set exp to zero.
	      q_exp = exp_dnrm;
	    }
	}

      // Round to odd from lower product bits
      p_odd = vec_addcuq (p_sig_l, (vui128_t) q_ones);
      q_sig = (vui128_t) vec_or ((vui32_t) q_sig, (vui32_t) p_odd);

      // Check for exponent overflow -> __FLT128_MAX__ (round to odd)
      if (__builtin_expect ((vec_cmpud_all_ge ( q_exp, exp_naninf)), 0))
	{
	  // Intermediate result is huge, unbiased exponent > 16383
	  // so return __FLT128_MAX__ with the appropriate sign.
	  const vui32_t f128_max = CONST_VINT128_W(0x7ffeffff, -1, -1, -1);
	  vui32_t f128_smax = vec_or ((vui32_t) f128_max, q_sign);
	  return vec_xfer_vui32t_2_bin128 (f128_smax);
	}
      else // combine sign, exp, and significand for return
	{
	  // Merge sign, significand, and exponent into final result
	  q_sig = (vui128_t) vec_or ((vui32_t) q_sig, q_sign);
	  vui32_t tmp, t128;
	  // convert DW exp_naninf to QW expmask
	  vui32_t expmask = vec_sld ((vui32_t) exp_naninf, q_zero, 14);
	  // convert q_exp from DW to QW for QP format
	  tmp = vec_sld ((vui32_t) q_exp, q_zero, 14);
	  t128 = vec_sel ((vui32_t) q_sig, tmp, expmask);
	  result = vec_xfer_vui32t_2_bin128 (t128);
	  return result;
	}
    }
  else
    { // One or both operands are NaN or Infinity
      //const vui32_t q_nan = CONST_VINT128_W(0x00008000, 0, 0, 0);
      vui32_t q_nan = vec_mask128_f128Qbit ();
      vui32_t q_inf = vec_mask128_f128exp ();
      // One or both operands are NaN
      if (vec_all_isnanf128 (vfa))
	{
	  // vfa is NaN, Convert vfa to QNaN and return
	  vui32_t vf128 = vec_or_bin128_2_vui32t (vfa, q_nan);
	  return vec_xfer_vui32t_2_bin128 (vf128);
	}
      else if (vec_all_isnanf128 (vfb))
	{
	  // vfb is NaN, Convert vfb to QNaN and return
	  vui32_t vf128 = vec_or_bin128_2_vui32t (vfb, q_nan);
	  return vec_xfer_vui32t_2_bin128 (vf128);
	}
      else  // Or one or both operands are Infinity
	{
	  if (vec_cmpud_all_eq (x_exp, (vui64_t) exp_naninf))
	    {
	      // Infinity x Infinity == signed Infinity
	      q_sig = (vui128_t) q_inf;
	    }
	  else
	    {
	      // One each Infinity/Finite value, check for 0.0
	      if (vec_cmpuq_all_eq (a_mag, (vui128_t) q_zero)
		  || vec_cmpuq_all_eq (b_mag, (vui128_t) q_zero))
		{
		      // Inifinty x Zero is Default Quiet NaN
		      return vec_const_nanf128 ();
		}
	      else // an Infinity and a Nonzero finite number
		{
		      // Return Infinity with product sign.
		      q_sig = (vui128_t) q_inf;
		}
	    }
	    // Merge sign, exp/sig into final result
	    q_sig = (vui128_t) vec_or ((vui32_t) q_sig, q_sign);
	    return vec_xfer_vui128t_2_bin128 (q_sig);
	}
    }
#else
  result = vfa * vfb;
#endif
  return result;
}

#ifdef PVECLIB_OLDTESTVERSIONS
#ifndef __clang__
__binary128
test_vec_mulqpo_V7 (__binary128 vfa, __binary128 vfb)
{
  __binary128 result;
#if defined (_ARCH_PWR9) && (__GNUC__ > 7)
#if defined (__FLOAT128__) && (__GNUC__ > 8)
  // earlier GCC versions generate extra data moves for this.
  result = __builtin_mulf128_round_to_odd (vfa, vfb);
#else
  // No extra data moves here.
  __asm__(
      "xsmulqpo %0,%1,%2"
      : "=v" (result)
      : "v" (vfa), "v" (vfb)
      : );
#endif
#elif  defined (_ARCH_PWR7)
  vui64_t q_exp, x_exp;
  vui128_t q_sig, a_sig, b_sig, p_sig_h, p_sig_l, p_odd;
  vui32_t q_sign, a_sign, b_sign;
  // const vui64_t q_zero = { 0, 0 };
  // const vui64_t q_zero = vec_splat_u64 (0);
  // const vui64_t q_ones = { -1, -1 };
  // const vui64_t q_ones = (vui64_t) vec_splat_s64 (-1);
  const vui32_t q_zero = CONST_VINT128_W (0, 0, 0, 0);
  const vui32_t q_ones = CONST_VINT128_W (-1, -1, -1, -1);
  //const vui64_t exp_naninf = (vui64_t) { 0x7fff, 0x7fff };
  const vui64_t exp_naninf = vec_mask64_f128exp();
  const vui32_t magmask = vec_mask128_f128mag();
  //vui32_t sigmask = vec_mask128_f128sig();

#if 1
  // Vector extract the exponents from vfa, vfb
  x_exp = vec_xxxexpqpp (vfa, vfb);
#else
  a_exp = vec_xsxexpqp (vfa);
  b_exp = vec_xsxexpqp (vfb);
  x_exp = vec_mrgahd ((vui128_t) a_exp, (vui128_t) b_exp);
#endif
#if 1
  // Mask off sign bits so can use integers for magnitude compare.
  vui128_t a_mag, b_mag;
  a_mag = (vui128_t) vec_and_bin128_2_vui32t (vfa, magmask);
  b_mag = (vui128_t) vec_and_bin128_2_vui32t (vfb, magmask);
  a_sign = vec_andc_bin128_2_vui32t (vfa, magmask);
  b_sign = vec_andc_bin128_2_vui32t (vfb, magmask);
#else
  // const vui32_t signmask = CONST_VINT128_W(0x80000000, 0, 0, 0);
  // vui32_t signmask = vec_mask128_f128sign();
  a_sig = vec_xsxsigqp (vfa);
  b_sig = vec_xsxsigqp (vfb);
  a_sign = vec_and_bin128_2_vui32t (vfa, signmask);
  b_sign = vec_and_bin128_2_vui32t (vfb, signmask);
#endif
  q_sign = vec_xor (a_sign, b_sign);

//  if (vec_all_isfinitef128 (vfa) && vec_all_isfinitef128 (vfb))
  if (__builtin_expect (vec_cmpud_all_lt (x_exp, exp_naninf), 1))
    {
      //const vui64_t exp_max = (vui64_t) { 0x7ffe, 0x7ffe };
      const vui64_t exp_dnrm = (vui64_t) q_zero;
//      vui32_t hidden = vec_mask128_f128Lbit();
//      vui32_t a_norm, b_norm, x_norm;
//      vui32_t a_s32, b_s32;
//      vui32_t q_ssig;
      vui64_t exp_min, exp_one, exp_bias;
      vui128_t p_tmp;
      // const vui64_t exp_min, exp_one = { 1, 1 };
      // exp_min = exp_one = vec_splat_u64 (1);
#if 1
    { // Extract the significands and insert the Hidden bit
      //const vui32_t q_zero = CONST_VINT128_W(0, 0, 0, 0);
      const vui32_t sigmask = vec_mask128_f128sig ();
      vui32_t a_s32, b_s32;
      vui16_t a_e16, b_e16, x_hidden;
      vb16_t a_norm, b_norm;

      //const vui32_t hidden = vec_mask128_f128Lbit();
      x_hidden = vec_splat_u16(1);
      // Assume that the operands are finite magnitudes
      // Mask off the significands
#if 1
      // Applying sigmask to orignal inputs can save 2 cycles here
      a_s32 = vec_and_bin128_2_vui32t (vfa, sigmask);
      b_s32 = vec_and_bin128_2_vui32t (vfb, sigmask);
      // But still need a/b_mag for exp extract to clear sign-bit
#else
      a_s32 = vec_and ((vui32_t) a_mag, sigmask);
      b_s32 = vec_and ((vui32_t) b_mag, sigmask);
#endif
      // Mask off the exponents in high halfword
      a_e16 = (vui16_t) vec_andc ((vui32_t) a_mag, sigmask);
      b_e16 = (vui16_t) vec_andc ((vui32_t) b_mag, sigmask);
      // Compare exponents for finite i.e. > denomal (q_zero)
      a_norm = vec_cmpgt (a_e16, (vui16_t) q_zero);
      b_norm = vec_cmpgt (b_e16, (vui16_t) q_zero);
      // For Normal QP insert (hidden) L-bit into significand
      a_sig = (vui128_t) vec_sel ((vui16_t) a_s32, x_hidden, a_norm);
      b_sig = (vui128_t) vec_sel ((vui16_t) b_s32, x_hidden, b_norm);
    }
#if 1
#else
      { // Extract the significands
	const vui32_t hidden = vec_mask128_f128Lbit();
	const  vui32_t sigmask = vec_mask128_f128sig();
	vui32_t a_norm, b_norm, x_norm;
	vui32_t a_s32, b_s32;
	// Assume that the operands are finite
	// Mask off the significands
	a_s32 = vec_and ((vui32_t) a_mag, sigmask);
	b_s32 = vec_and ((vui32_t) b_mag, sigmask);
	// Assume that exponents are already extracted and merged
	// Compare exponents for denormal, assume finite
	x_norm = (vui32_t) vec_cmpgt ((vui32_t) x_exp, q_zero);
	a_norm = vec_splat (x_norm, VEC_WE_1);
	b_norm = vec_splat (x_norm, VEC_WE_3);
	// For Normal QP insert (hidden) L-bit into significand
	a_sig =  (vui128_t) vec_sel (a_s32, a_norm, hidden);
	b_sig =  (vui128_t) vec_sel (b_s32, b_norm, hidden);
	// exponents should >= Emin for computation
	//x_exp = vec_selud (x_exp, exp_min, (vb64_t) x_norm);
      }
#endif
#else
      a_sig = vec_xsxsigqp (vfa);
      b_sig = vec_xsxsigqp (vfb);
#endif
#if 0
      // check for zero significands in multiply
      if (__builtin_expect (
	    (vec_all_eq((vui32_t ) a_mag, (vui32_t ) q_zero)
	  || vec_all_eq((vui32_t ) b_mag, (vui32_t ) q_zero)),
	  0))
	{ // Multiply by zero, return QP signed zero
	  result = vec_xfer_vui32t_2_bin128 (q_sign);
	  return result;
	}
#endif

      // Precondition the significands before multiply so that the
      // high-order 114-bits (C,L,FRACTION) of the product are right
      // adjusted in p_sig_h. And the Low-order 112-bits are left
      // justified in p_sig_l.
      // Logically this (multiply) step could be moved after the zero
      // test. But this uses a lot of registers and the compiler may
      // see this as register pressure and decide to spill and reload
      // unrelated data around this block.
      // The zero multiply is rare so on average performance is better
      // if we get this started now.
      a_sig = vec_slqi (a_sig, 8);
      b_sig = vec_slqi (b_sig, 8);
      p_sig_l = vec_muludq (&p_sig_h, a_sig, b_sig);

#if 1
      // check for zero significands in multiply
      if (__builtin_expect (
	    (vec_all_eq((vui32_t ) a_sig, (vui32_t ) q_zero)
	  || vec_all_eq((vui32_t ) b_sig, (vui32_t ) q_zero)),
	  0))
	{ // Multiply by zero, return QP signed zero
	  result = vec_xfer_vui32t_2_bin128 (q_sign);
	  return result;
	}
#endif
      // const vui64_t exp_min, exp_one = { 1, 1 };
      exp_min = exp_one = vec_splat_u64 (1);
#if 1
      //const vui64_t exp_bias = (vui64_t) { 0x3fff, 0x3fff };
      exp_bias = (vui64_t) vec_srhi ((vui16_t) exp_naninf, 1);
#else
      const vui64_t exp_bias = (vui64_t) { 0x3fff, 0x3fff };
#endif
	{ // Compute product exponent q_exp
	  // Operand exponents should >= Emin for computation
#if 1
	  vb64_t exp_mask;
	  exp_mask = vec_cmpequd (x_exp, exp_dnrm);
	  x_exp = vec_selud (x_exp, exp_min, (vb64_t) exp_mask);
#endif
#if 1
	  // sum exponents across x_exp
	  q_exp = vec_addudm (x_exp, vec_swapd (x_exp));
#else
	  a_exp = vec_splatd (x_exp, VEC_DW_H);
	  b_exp = vec_splatd (x_exp, VEC_DW_L);
	  // sum exponents
	  q_exp = vec_addudm (a_exp, b_exp);
#endif
	  q_exp = vec_subudm (q_exp, exp_bias);
	}

      // Check for carry and adjust exp +1
	{
	  vb128_t carry_mask;
	  vui128_t sig_h, sig_l;
	  // Test Carry-bit (greater than L-bit)
	  vui16_t sig_l_mask = vec_splat_u16(1);
	  vui16_t t_sig = vec_splat ((vui16_t) p_sig_h, VEC_HW_H);
	  carry_mask = (vb128_t) vec_cmpgt (t_sig, sig_l_mask);
	  // Shift double quadword right 1 bit
	  p_tmp = vec_sldqi (p_sig_h, p_sig_l, 120);
	  sig_h = vec_srqi (p_sig_h, 1);
	  sig_l = vec_slqi (p_tmp, 7);
	  // Increment the exponent
	  x_exp = vec_addudm (q_exp, exp_one);
	  // Select original or normalized exp/sig
	  p_sig_h = vec_seluq (p_sig_h, sig_h, carry_mask);
	  p_sig_l = vec_seluq (p_sig_l, sig_l, carry_mask);
	  q_exp = vec_selud (q_exp, x_exp, (vb64_t) carry_mask);
	}
      // There are two cases for denormal
      // 1) The sum of unbiased exponents is less the E_min (tiny).
      // 2) The significand is less then 1.0 (C and L-bits are zero).
      //  2a) The exponent is > E_min
      //  2b) The exponent is == E_min
      //
      q_sig = p_sig_h;
      // Check for Tiny exponent
      if (__builtin_expect (
	  (vec_cmpsd_all_lt ((vi64_t) q_exp, (vi64_t) exp_min)), 0))
	{
	  //const vui64_t exp_128 = (vui64_t) { 128, 128 };
	  const vui64_t exp_128 = vec_const64_f128_128();
	  const vui64_t too_tiny = (vui64_t) { 116, 116 };
	  // const vui32_t xmask = CONST_VINT128_W(0x1fffffff, -1, -1, -1);
	  vui32_t xmask = (vui32_t) vec_srqi ((vui128_t) q_ones, 3);
	  vui32_t tmp;

	  // Intermediate result is tiny, unbiased exponent < -16382
	  //x_exp = vec_subudm ((vui64_t) exp_tiny, q_exp);
	  x_exp = vec_subudm (exp_min, q_exp);

	  if (vec_cmpud_all_gt ((vui64_t) x_exp, too_tiny))
	    {
	      // Intermediate result is too tiny, the shift will
	      // zero the fraction and the GR-bit leaving only the
	      // Sticky bit. The X-bit needs to include all bits
	      // from p_sig_h and p_sig_l
	      p_sig_l = vec_srqi (p_sig_l, 8);
	      p_sig_l = (vui128_t) vec_or ((vui32_t) p_sig_l,
					   (vui32_t) p_sig_h);
	      // generate a carry into bit-2 for any nonzero bits 3-127
	      p_sig_l = vec_adduqm (p_sig_l, (vui128_t) xmask);
	      q_sig = (vui128_t) q_zero;
	      p_sig_l = (vui128_t) vec_andc ((vui32_t) p_sig_l, xmask);
	    }
	  else
	    { // Normal tiny, right shift may loose low order bits
	      // from p_sig_l. So collect any 1-bits below GRX and
	      // OR them into the X-bit, before the right shift.
	      vui64_t l_exp;

	      // Propagate low order bits into the sticky bit
	      // GRX left adjusted in p_sig_l
	      // Issolate bits below GDX (bits 3-128).
	      tmp = vec_and ((vui32_t) p_sig_l, xmask);
	      // generate a carry into bit-2 for any nonzero bits 3-127
	      tmp = (vui32_t) vec_adduqm ((vui128_t) tmp, (vui128_t) xmask);
	      // Or this with the X-bit to propagate any sticky bits into X
	      p_sig_l = (vui128_t) vec_or ((vui32_t) p_sig_l, tmp);
	      p_sig_l = (vui128_t) vec_andc ((vui32_t) p_sig_l, xmask);

	      l_exp = vec_subudm (exp_128, x_exp);
	      p_sig_l = vec_sldq (p_sig_h, p_sig_l, (vui128_t) l_exp);
	      p_sig_h = vec_srq (p_sig_h, (vui128_t) x_exp);
	      q_sig = p_sig_h;
	    }
	  // Set the exponent for denormal
	  q_exp = exp_dnrm;
	}
      // Exponent is not tiny but significand may be denormal
      // Isolate sig CL bits and compare
      vui16_t t_sig = vec_splat ((vui16_t) p_sig_h, VEC_HW_H);
      if (__builtin_expect ((vec_all_eq(t_sig, (vui16_t ) q_zero)), 0))
	{
	  // Is below normal range. This can happen when
	  // multiplying a denormal by a normal.
	  // So try to normalize the significand.
	  //const vui64_t exp_15 = { 15, 15 };
	  const vui64_t exp_15 = vec_splat_u64 (15);
	  vui64_t c_exp, d_exp;
	  vui128_t c_sig;
	  vb64_t exp_mask;
	  c_sig = vec_clzq (p_sig_h);
	  c_exp = vec_splatd ((vui64_t) c_sig, VEC_DW_L);
	  c_exp = vec_subudm (c_exp, exp_15);
	  d_exp = vec_subudm (q_exp, exp_min);
	  d_exp = vec_minud (c_exp, d_exp);
	  exp_mask = vec_cmpgtud (q_exp, c_exp);

	  // Intermediate result <= tiny, unbiased exponent <= -16382
	  if (vec_cmpsd_all_gt ((vi64_t) q_exp, (vi64_t) exp_min))
	    {
	      // Try to normalize the significand.
	      p_sig_h = vec_sldq (p_sig_h, p_sig_l, (vui128_t) d_exp);
	      p_sig_l = vec_slq (p_sig_l, (vui128_t) d_exp);
	      q_sig = p_sig_h;
	      // Compare computed exp to shift count to normalize.
	      //exp_mask = vec_cmpgtud (q_exp, c_exp);
	      q_exp = vec_subudm (q_exp, d_exp);
	      q_exp = vec_selud (exp_dnrm, q_exp, exp_mask);
	    }
	  else
	    { // sig is denormal range (L-bit is 0). Set exp to zero.
	      q_exp = exp_dnrm;
	    }
	}

      // Round to odd from lower product bits
      p_odd = vec_addcuq (p_sig_l, (vui128_t) q_ones);
      q_sig = (vui128_t) vec_or ((vui32_t) q_sig, (vui32_t) p_odd);

      // Check for exponent overflow -> __FLT128_MAX__ (round to odd)
#if 1
      if (__builtin_expect ((vec_cmpud_all_ge ( q_exp, exp_naninf)), 0))
#else
      //if  (vec_cmpud_all_gt ( q_exp, exp_max))
      if (__builtin_expect ((vec_cmpud_all_gt (q_exp, exp_max)), 0))
#endif
	{
	  // Intermediate result is huge, unbiased exponent > 16383
	  // so return __FLT128_MAX__ with the appropriate sign.
	  const vui32_t f128_max = CONST_VINT128_W(0x7ffeffff, -1, -1, -1);
	  vui32_t f128_smax = vec_or ((vui32_t) f128_max, q_sign);
	  return vec_xfer_vui32t_2_bin128 (f128_smax);
	}
      else
	{
	  // Merge sign, significand, and exponent into final result
	  q_sig = (vui128_t) vec_or ((vui32_t) q_sig, q_sign);
    #if 1
	  vui32_t tmp, t128;
	  // convert DW exp_naninf to QW expmask
	  vui32_t expmask = vec_sld ((vui32_t) exp_naninf, q_zero, 14);
	  // convert q_exp from DW to QW for QP format
	  tmp = vec_sld ((vui32_t) q_exp, q_zero, 14);
	  t128 = vec_sel ((vui32_t) q_sig, tmp, expmask);
	  result = vec_xfer_vui32t_2_bin128 (t128);
    #else
	  result = vec_xsiexpqp (q_sig, q_exp);
    #endif
	  return result;
	}
    }
  else
    { // One or both operands are NaN or Infinity
#if 1
      //const vui32_t q_nan = CONST_VINT128_W(0x00008000, 0, 0, 0);
      vui32_t q_nan = vec_mask128_f128Qbit ();
      // One or both operands are NaN
      if (vec_all_isnanf128 (vfa))
	{
	  // vfa is NaN, Convert vfa to QNaN and return
	  vui32_t vf128 = vec_or_bin128_2_vui32t (vfa, q_nan);
	  return vec_xfer_vui32t_2_bin128 (vf128);
	}
      else if (vec_all_isnanf128 (vfb))
	{
	  // vfb is NaN, Convert vfb to QNaN and return
	  vui32_t vf128 = vec_or_bin128_2_vui32t (vfb, q_nan);
	  return vec_xfer_vui32t_2_bin128 (vf128);
	}
      else  // Or one or both operands are Infinity
	{
	  vui128_t a_mag, b_mag;
	  const vui32_t magmask = vec_mask128_f128mag();
	  a_mag = (vui128_t) vec_and_bin128_2_vui32t (vfa, magmask);
	  b_mag = (vui128_t) vec_and_bin128_2_vui32t (vfb, magmask);
	  if (vec_cmpud_all_eq (x_exp, (vui64_t) exp_naninf))
	    {
	      // Infinity x Infinity == signed Infinity
	      //q_sign = vec_xor (a_sign, b_sign);
	      q_exp = x_exp;
	      q_sig = (vui128_t) q_zero;
	    }
	  else
	    {
	      // One Infinity and finite value, check for 0.0
	      if (vec_cmpuq_all_eq (a_mag, (vui128_t) q_zero)
		  || vec_cmpuq_all_eq (b_mag, (vui128_t) q_zero))
		{
		      // Inifinty x Zero is Default Quiet NaN
		      return vec_const_nanf128 ();
		}
	      else // an Infinity and a Nonzero finite number
		{
		      // Return Infinity with product sign.
		      q_exp = exp_naninf;
		      //q_sign = vec_xor (a_sign, b_sign);
		      q_sig = (vui128_t) q_zero;
		}
	    }
	}
#else
      //const vui32_t q_nan = CONST_VINT128_W(0x00008000, 0, 0, 0);
      const vui32_t q_nan = vec_mask128_f128Qbit ();
      a_exp = vec_splatd (x_exp, VEC_DW_H);
      //b_exp = vec_splatd (x_exp, VEC_DW_L);
      if (vec_cmpuq_all_eq (a_sig, (vui128_t) q_zero)
	  && vec_cmpuq_all_eq (b_sig, (vui128_t) q_zero))
	{
	  // Both operands either infinity or zero
	  if (vec_cmpud_any_eq (x_exp, (vui64_t) q_zero))
	    {
	      // Inifinty x Zero is Default Quiet NaN
	      return vec_const_nanf128 ();
	    }
	  else
	    {
	      // Infinity x Infinity == signed Infinity
	      //q_sign = vec_xor (a_sign, b_sign);
	      q_exp = a_exp;
	      q_sig = a_sig;
	    }
	}
      else
	{
	  // One or both operands are NaN
	  if (vec_all_isnanf128 (vfa))
	    {
	      // vfa is NaN, Convert vfa to QNaN and return
	      vui32_t vf128 = vec_or_bin128_2_vui32t (vfa, q_nan);
	      return vec_xfer_vui32t_2_bin128 (vf128);
	    }
	  else if (vec_all_isnanf128 (vfb))
	    {
	      // vfb is NaN, Convert vfb to QNaN and return
	      vui32_t vf128 = vec_or_bin128_2_vui32t (vfb, q_nan);
	      return vec_xfer_vui32t_2_bin128 (vf128);
	    }
	  else  // OR an Infinity and a Nonzero finite number
	    {
	      // Return Infinity with product sign.
	      q_exp = exp_naninf;
	      //q_sign = vec_xor (a_sign, b_sign);
	      q_sig = (vui128_t) q_zero;
	    }
	}
#endif
    }
  // Merge sign, significand, and exponent into final result
  q_sig = (vui128_t) vec_or ((vui32_t) q_sig, q_sign);
  result = vec_xsiexpqp (q_sig, q_exp);
#else
  result = vfa * vfb;
#endif
  return result;
}

__binary128
test_vec_mulqpo_V6 (__binary128 vfa, __binary128 vfb)
{
  __binary128 result;
#if defined (_ARCH_PWR9) && (__GNUC__ > 7)
#if defined (__FLOAT128__) && (__GNUC__ > 8)
  // earlier GCC versions generate extra data moves for this.
  result = __builtin_mulf128_round_to_odd (vfa, vfb);
#else
  // No extra data moves here.
  __asm__(
      "xsmulqpo %0,%1,%2"
      : "=v" (result)
      : "v" (vfa), "v" (vfb)
      : );
#endif
#elif  defined (_ARCH_PWR7)
  vui64_t q_exp, a_exp, b_exp, x_exp;
  vui128_t q_sig, a_sig, b_sig, p_sig_h, p_sig_l, p_odd;
  vui32_t q_sign, a_sign, b_sign;
  const vui32_t signmask = CONST_VINT128_W(0x80000000, 0, 0, 0);
  // const vui64_t q_zero = { 0, 0 };
  const vui64_t q_zero = vec_splat_u64 (0);
  // const vui64_t q_ones = { -1, -1 };
  const vui64_t q_ones = (vui64_t) vec_splat_s64 (-1);
  const vui64_t exp_naninf = (vui64_t) { 0x7fff, 0x7fff };

  a_exp = vec_xsxexpqp (vfa);
  a_sig = vec_xsxsigqp (vfa);
  a_sign = vec_and_bin128_2_vui32t (vfa, signmask);
  b_exp = vec_xsxexpqp (vfb);
  b_sig = vec_xsxsigqp (vfb);
  b_sign = vec_and_bin128_2_vui32t (vfb, signmask);
  x_exp = vec_mrgahd ((vui128_t) a_exp, (vui128_t) b_exp);
  q_sign = vec_xor (a_sign, b_sign);

//  if (vec_all_isfinitef128 (vfa) && vec_all_isfinitef128 (vfb))
  if (__builtin_expect (vec_cmpud_all_lt (x_exp, exp_naninf), 1))
    {
      const vui64_t exp_bias = (vui64_t) { 0x3fff, 0x3fff };
      const vui64_t exp_max = (vui64_t) { 0x7ffe, 0x7ffe };
      const vui64_t exp_dnrm = q_zero;
      vui64_t exp_min, exp_one;
      vui128_t p_tmp;
      // Precondition the significands before multiply so that the
      // high-order 114-bits (C,L,FRACTION) of the product are right
      // adjusted in p_sig_h. And the Low-order 112-bits are left
      // justified in p_sig_l.
      // Logically this (multiply) step could be moved after the zero
      // test. But this uses a lot of registers and the compiler may
      // see this as register pressure and decide to spill and reload
      // unrelated data around this block.
      // The zero multiply is rare so on average performance is better
      // if we get this started now.
      a_sig = vec_slqi (a_sig, 8);
      b_sig = vec_slqi (b_sig, 8);
      p_sig_l = vec_muludq (&p_sig_h, a_sig, b_sig);

      // check for zero significands in multiply
      if (__builtin_expect (
	    (vec_all_eq((vui32_t ) a_sig, (vui32_t ) q_zero)
	  || vec_all_eq((vui32_t ) b_sig, (vui32_t ) q_zero)),
	  0))
	{ // Multiply by zero, return QP signed zero
	  result = vec_xfer_vui32t_2_bin128 (q_sign);
	  return result;
	}
      // const vui64_t exp_min, exp_one = { 1, 1 };
      exp_min = exp_one = vec_splat_u64 (1);
	{
	  vb64_t exp_mask;
	  exp_mask = vec_cmpequd (x_exp, exp_dnrm);
	  x_exp = vec_selud (x_exp, exp_min, (vb64_t) exp_mask);
	  a_exp = vec_splatd (x_exp, VEC_DW_H);
	  b_exp = vec_splatd (x_exp, VEC_DW_L);
	}
      // sum exponents
      q_exp = vec_addudm (a_exp, b_exp);
      q_exp = vec_subudm (q_exp, exp_bias);

      // Check for carry and adjust exp +1
	{
	  vb128_t exp_mask;
	  vui128_t sig_h, sig_l;
	  // Test Carry-bit (greater than L-bit)
	  vui16_t sig_l_mask = vec_splat_u16(1);
	  vui16_t t_sig = vec_splat ((vui16_t) p_sig_h, VEC_HW_H);
	  exp_mask = (vb128_t) vec_cmpgt (t_sig, sig_l_mask);
	  // Shift double quadword right 1 bit
	  p_tmp = vec_sldqi (p_sig_h, p_sig_l, 120);
	  sig_h = vec_srqi (p_sig_h, 1);
	  sig_l = vec_slqi (p_tmp, 7);
	  // Increment the exponent
	  x_exp = vec_addudm (q_exp, exp_one);
	  // Select original or normalized exp/sig
	  p_sig_h = vec_seluq (p_sig_h, sig_h, exp_mask);
	  p_sig_l = vec_seluq (p_sig_l, sig_l, exp_mask);
	  q_exp = vec_selud (q_exp, x_exp, (vb64_t) exp_mask);
	}
      // There are two cases for denormal
      // 1) The sum of unbiased exponents is less the E_min (tiny).
      // 2) The significand is less then 1.0 (C and L-bits are zero).
      //  2a) The exponent is > E_min
      //  2b) The exponent is == E_min
      //
      q_sig = p_sig_h;
      // Check for Tiny exponent
      if (__builtin_expect (
	  (vec_cmpsd_all_lt ((vi64_t) q_exp, (vi64_t) exp_min)), 0))
	{
	  const vui64_t too_tiny = (vui64_t) { 116, 116 };
	  const vui32_t xmask = CONST_VINT128_W(0x1fffffff, -1, -1, -1);
	  vui32_t tmp;

	  // Intermediate result is tiny, unbiased exponent < -16382
	  //x_exp = vec_subudm ((vui64_t) exp_tiny, q_exp);
	  x_exp = vec_subudm (exp_min, q_exp);

	  if (vec_cmpud_all_gt ((vui64_t) x_exp, too_tiny))
	    {
	      // Intermediate result is too tiny, the shift will
	      // zero the fraction and the GR-bit leaving only the
	      // Sticky bit. The X-bit needs to include all bits
	      // from p_sig_h and p_sig_l
	      p_sig_l = vec_srqi (p_sig_l, 8);
	      p_sig_l = (vui128_t) vec_or ((vui32_t) p_sig_l,
					   (vui32_t) p_sig_h);
	      // generate a carry into bit-2 for any nonzero bits 3-127
	      p_sig_l = vec_adduqm (p_sig_l, (vui128_t) xmask);
	      q_sig = (vui128_t) q_zero;
	      p_sig_l = (vui128_t) vec_andc ((vui32_t) p_sig_l, xmask);
	    }
	  else
	    { // Normal tiny, right shift may loose low order bits
	      // from p_sig_l. So collect any 1-bits below GRX and
	      // OR them into the X-bit, before the right shift.
	      vui64_t l_exp;
	      const vui64_t exp_128 = (vui64_t) { 128, 128 };

	      // Propagate low order bits into the sticky bit
	      // GRX left adjusted in p_sig_l
	      // Issolate bits below GDX (bits 3-128).
	      tmp = vec_and ((vui32_t) p_sig_l, xmask);
	      // generate a carry into bit-2 for any nonzero bits 3-127
	      tmp = (vui32_t) vec_adduqm ((vui128_t) tmp, (vui128_t) xmask);
	      // Or this with the X-bit to propagate any sticky bits into X
	      p_sig_l = (vui128_t) vec_or ((vui32_t) p_sig_l, tmp);
	      p_sig_l = (vui128_t) vec_andc ((vui32_t) p_sig_l, xmask);

	      l_exp = vec_subudm (exp_128, x_exp);
	      p_sig_l = vec_sldq (p_sig_h, p_sig_l, (vui128_t) l_exp);
	      p_sig_h = vec_srq (p_sig_h, (vui128_t) x_exp);
	      q_sig = p_sig_h;
	    }
	  q_exp = exp_dnrm;
	}
      // Exponent is not tiny but significand may be denormal
      // Isolate sig CL bits and compare
      vui16_t t_sig = vec_splat ((vui16_t) p_sig_h, VEC_HW_H);
      if (__builtin_expect ((vec_all_eq(t_sig, (vui16_t ) q_zero)), 0))
	{
	  // Is below normal range. This can happen when
	  // multiplying a denormal by a normal.
	  // So try to normalize the significand.
	  //const vui64_t exp_15 = { 15, 15 };
	  const vui64_t exp_15 = vec_splat_u64 (15);
	  vui64_t c_exp, d_exp;
	  vui128_t c_sig;
	  vb64_t exp_mask;
	  c_sig = vec_clzq (p_sig_h);
	  c_exp = vec_splatd ((vui64_t) c_sig, VEC_DW_L);
	  c_exp = vec_subudm (c_exp, exp_15);
	  d_exp = vec_subudm (q_exp, exp_min);
	  d_exp = vec_minud (c_exp, d_exp);
	  exp_mask = vec_cmpgtud (q_exp, c_exp);

	  // Intermediate result <= tiny, unbiased exponent <= -16382
	  if (vec_cmpsd_all_gt ((vi64_t) q_exp, (vi64_t) exp_min))
	    {
	      // Try to normalize the significand.
	      p_sig_h = vec_sldq (p_sig_h, p_sig_l, (vui128_t) d_exp);
	      p_sig_l = vec_slq (p_sig_l, (vui128_t) d_exp);
	      q_sig = p_sig_h;
	      // Compare computed exp to shift count to normalize.
	      //exp_mask = vec_cmpgtud (q_exp, c_exp);
	      q_exp = vec_subudm (q_exp, d_exp);
	      q_exp = vec_selud (exp_dnrm, q_exp, exp_mask);
	    }
	  else
	    { // sig is denormal range (L-bit is 0). Set exp to zero.
	      q_exp = exp_dnrm;
	    }
	}
      // Round to odd from lower product bits
      p_odd = vec_addcuq (p_sig_l, (vui128_t) q_ones);
      q_sig = (vui128_t) vec_or ((vui32_t) q_sig, (vui32_t) p_odd);

      // Check for exponent overflow -> __FLT128_INF__
#if 0
      if (__builtin_expect ((vec_cmpud_all_ge ( q_exp, exp_naninf)), 0))
#else
      //if  (vec_cmpud_all_gt ( q_exp, exp_max))
      if (__builtin_expect ((vec_cmpud_all_gt (q_exp, exp_max)), 0))
#endif
	{
	  // Intermediate result is huge, unbiased exponent > 16383
	  // so return __FLT128_MAX__ with the appropriate sign.
	  const vui32_t f128_max = CONST_VINT128_W(0x7ffeffff, -1, -1, -1);
	  vui32_t f128_smax = vec_or ((vui32_t) f128_max, q_sign);
	  return vec_xfer_vui32t_2_bin128 (f128_smax);
	}
    }
  else
    { // One or both operands are NaN or Infinity
      if (vec_cmpuq_all_eq (a_sig, (vui128_t) q_zero)
	  && vec_cmpuq_all_eq (b_sig, (vui128_t) q_zero))
	{
	  // Both operands either infinity or zero
	  if (vec_cmpud_any_eq (x_exp, q_zero))
	    {
	      // Inifinty x Zero is Default Quiet NaN
	      return vec_const_nanf128 ();
	    }
	  else
	    {
	      // Infinity x Infinity == signed Infinity
	      q_sign = vec_xor (a_sign, b_sign);
	      q_exp = a_exp;
	      q_sig = a_sig;
	    }
	}
      else
	{
	  // One or both operands are NaN
	  const vui32_t q_nan = CONST_VINT128_W(0x00008000, 0, 0, 0);
	  if (vec_all_isnanf128 (vfa))
	    {
	      // vfa is NaN, Return vfa as Quite NaN.
	      q_exp = a_exp;
	      q_sign = a_sign;
	      q_sig = (vui128_t) vec_or ((vui32_t) a_sig, q_nan);
	    }
	  else if (vec_all_isnanf128 (vfb))
	    {
	      // vfb is NaN, Return vfb as Quite NaN.
	      q_exp = b_exp;
	      q_sign = b_sign;
	      q_sig = (vui128_t) vec_or ((vui32_t) b_sig, q_nan);
	    }
	  else  // OR an Infinity and a Nonzero finite number
	    {
	      // Return Infinity with product sign.
	      q_exp = exp_naninf;
	      q_sign = vec_xor (a_sign, b_sign);
	      q_sig = (vui128_t) q_zero;
	    }
	}
    }
  // Merge sign, significand, and exponent into final result
  q_sig = (vui128_t) vec_or ((vui32_t) q_sig, q_sign);
  result = vec_xsiexpqp (q_sig, q_exp);
#else
  result = vfa * vfb;
#endif
  return result;
}

__binary128
test_vec_mulqpo_V5 (__binary128 vfa, __binary128 vfb)
{
  __binary128 result;
#if defined (_ARCH_PWR9) && (__GNUC__ > 7)
#if defined (__FLOAT128__) && (__GNUC__ > 8)
  // earlier GCC versions generate extra data moves for this.
  result = __builtin_mulf128_round_to_odd (vfa, vfb);
#else
  // No extra data moves here.
  __asm__(
      "xsmulqpo %0,%1,%2"
      : "=v" (result)
      : "v" (vfa), "v" (vfb)
      : );
#endif
#elif  defined (_ARCH_PWR7)
  vui64_t q_exp, a_exp, b_exp, x_exp;
  vui64_t exp_naninf, exp_max;
  vui128_t q_sig, a_sig, b_sig, p_sig_h, p_sig_l, p_odd;
  vui32_t q_sign,  a_sign,  b_sign;
  const vui32_t signmask = CONST_VINT128_W(0x80000000, 0, 0, 0);
  // const vui64_t q_zero = { 0, 0 };
  const vui64_t q_zero = vec_splat_u64 (0);
  // const vui64_t q_ones = { -1, -1 };
  const vui64_t q_ones = (vui64_t) vec_splat_s64 (-1);
  // const vui64_t exp_dnrm = (vui64_t) CONST_VINT64_DW( 0, 0 );
  const vui64_t exp_dnrm = vec_splat_u64 (0);
  // Poor man's load and splat implementation
  // Let the endian swap happen, its ok. using endian sensitive splatd.
  const vui64_t exp_naninf_max = (vui64_t) { 0x7fff, 0x7ffe };
  const vui64_t exp_bias_min = (vui64_t) { 0x3fff, 0x1 };

  a_exp = vec_xsxexpqp (vfa);
  a_sig = vec_xsxsigqp (vfa);
  a_sign = vec_and_bin128_2_vui32t (vfa, signmask);
  b_exp = vec_xsxexpqp (vfb);
  b_sig = vec_xsxsigqp (vfb);
  b_sign = vec_and_bin128_2_vui32t (vfb, signmask);
  x_exp = vec_mrgahd ((vui128_t) a_exp, (vui128_t) b_exp);
  q_sign = vec_xor (a_sign, b_sign);

  exp_naninf = vec_splatd (exp_naninf_max, 0);

//  if (vec_all_isfinitef128 (vfa) && vec_all_isfinitef128 (vfb))
  if (__builtin_expect (vec_cmpud_all_lt (x_exp, exp_naninf), 1))
    {
      const vui32_t sigov = CONST_VINT128_W(0x0001ffff, -1, -1, -1);
      const vui32_t sigovt = CONST_VINT128_W(0x0000ffff, -1, -1, -1);
      vui64_t exp_bias = vec_splatd (exp_bias_min, 0);
      vui64_t exp_min = vec_splatd (exp_bias_min, 1);
      //const vui64_t q_one = { 1, 1 };
      vui64_t q_one = exp_min;

      vui128_t p_tmp;
      // Precondition the significands before multiply so that the
      // high-order 114-bits (C,L,FRACTION) of the product are right
      // adjusted in p_sig_h. And the Low-order 112-bits are left
      // justified in p_sig_l.
      a_sig = vec_slqi (a_sig, 8);
      b_sig = vec_slqi (b_sig, 8);
      p_sig_l = vec_muludq (&p_sig_h, a_sig, b_sig);
      if (vec_cmpud_any_eq (x_exp, exp_dnrm))
	{ // Involves zeros or denormals
	  // check for zero significands in multiply
	  if (vec_all_eq ((vui32_t) a_sig, (vui32_t) q_zero)
	   || vec_all_eq ((vui32_t) b_sig, (vui32_t) q_zero))
	    { // Multiply by zero, return QP signed zero
	      result = vec_xfer_vui32t_2_bin128 (q_sign);
	      return result;
	    }
	  else
	    {
	      vb64_t exp_mask;
	      exp_mask = vec_cmpequd (x_exp, exp_dnrm);
	      x_exp = vec_selud (x_exp, exp_min, (vb64_t) exp_mask);
	      a_exp = vec_splatd (x_exp, VEC_DW_H);
	      b_exp = vec_splatd (x_exp, VEC_DW_L);
	    }
	}
      else
	{
	  a_exp = vec_splatd (a_exp, VEC_DW_H);
	  b_exp = vec_splatd (b_exp, VEC_DW_H);

	  // Check for carry and adjust exp +1
	  if (vec_cmpuq_all_gt (p_sig_h, (vui128_t) sigov))
	    {
	      p_tmp = vec_sldqi (p_sig_h, p_sig_l, 120);
	      p_sig_h = vec_srqi (p_sig_h, 1);
	      p_sig_l = vec_slqi (p_tmp, 7);
	      a_exp = vec_addudm (a_exp, q_one);
	    }
	}
      // sum exponents
      q_exp = vec_addudm (a_exp, b_exp);
      q_exp = vec_subudm (q_exp, exp_bias);
      // There are two cases for denormal
      // 1) The sum of unbiased exponents is less the E_min (tiny).
      // 2) The significand is less then 1.0 (C and L-bits are zero).
      //  2a) The exponent is > E_min
      //  2b) The exponent is == E_min
      //
      if (vec_cmpsd_all_lt ((vi64_t) q_exp, (vi64_t) exp_min))
	{
	  const vui64_t too_tiny = (vui64_t) { 116, 116 };
	  const vui32_t xmask = CONST_VINT128_W(0x1fffffff, -1, -1, -1);
	  vui32_t tmp;

	  // Intermediate result is tiny, unbiased exponent < -16382
	  //x_exp = vec_subudm ((vui64_t) exp_tiny, q_exp);
	  x_exp = vec_subudm ( exp_min, q_exp);

	  if (vec_cmpud_all_gt ((vui64_t) x_exp, too_tiny))
	    {
	      // Intermediate result is too tiny, the shift will
	      // zero the fraction and the GR-bit leaving only the
	      // Sticky bit. The X-bit needs to include all bits
	      // from p_sig_h and p_sig_l
	      p_sig_l = vec_srqi (p_sig_l, 8);
	      p_sig_l = (vui128_t) vec_or ((vui32_t) p_sig_l,
					   (vui32_t) p_sig_h);
	      // generate a carry into bit-2 for any nonzero bits 3-127
	      p_sig_l = vec_adduqm (p_sig_l, (vui128_t) xmask);
	      q_sig = (vui128_t) q_zero;
	      p_sig_l = (vui128_t) vec_andc ((vui32_t) p_sig_l, xmask);
	    }
	  else
	    { // Normal tiny, right shift may loose low order bits
	      // from p_sig_l. So collect any 1-bits below GRX and
	      // OR them into the X-bit, before the right shift.
	      vui64_t l_exp;
	      const vui64_t exp_128 = (vui64_t) { 128, 128 };

	      // Propagate low order bits into the sticky bit
	      // GRX left adjusted in p_sig_l
	      // Issolate bits below GDX (bits 3-128).
	      tmp = vec_and ((vui32_t) p_sig_l, xmask);
	      // generate a carry into bit-2 for any nonzero bits 3-127
	      tmp = (vui32_t) vec_adduqm ((vui128_t) tmp, (vui128_t) xmask);
	      // Or this with the X-bit to propagate any sticky bits into X
	      p_sig_l = (vui128_t) vec_or ((vui32_t) p_sig_l, tmp);
	      p_sig_l = (vui128_t) vec_andc ((vui32_t) p_sig_l, xmask);

	      l_exp = vec_subudm (exp_128, x_exp);
	      p_sig_l = vec_sldq (p_sig_h, p_sig_l, (vui128_t) l_exp);
	      p_sig_h = vec_srq (p_sig_h, (vui128_t) x_exp);
	      q_sig = p_sig_h;
	    }
	  q_exp = q_zero;
	}
      else
	{
	  // Check is significand is in normal range.
	  if (vec_cmpuq_all_le (p_sig_h, (vui128_t) sigovt))
	    {
	      // Is below normal range. This can happen when
	      // multiplying a denormal by a normal.
	      // So try to normalize the significand.
	      //const vui64_t exp_15 = { 15, 15 };
	      const vui64_t exp_15 = vec_splat_u64 (15);
	      vui64_t c_exp, d_exp;
	      vui128_t c_sig;
	      c_sig = vec_clzq (p_sig_h);
	      c_exp = vec_splatd ((vui64_t) c_sig, VEC_DW_L);
	      c_exp = vec_subudm (c_exp, exp_15);
	      d_exp = vec_subudm (q_exp, exp_min);
	      d_exp = vec_minud (c_exp, d_exp);

	      // Intermediate result <= tiny, unbiased exponent <= -16382
	      if (vec_cmpsd_all_gt ((vi64_t) q_exp, (vi64_t) exp_min))
		{ // Try to normalize the significand.
		  p_sig_h = vec_sldq (p_sig_h, p_sig_l, (vui128_t) d_exp);
		  p_sig_l = vec_slq (p_sig_l, (vui128_t) d_exp);
		  // Compare computed exp to shift count to normalize.
		  if (vec_cmpud_all_le (q_exp, c_exp))
		    { // exp less than shift count to normalize so
		      // result is still denormal.
		      q_exp = q_zero;
		    }
		  else // Adjust exp after normalize shift left.
		    q_exp = vec_subudm (q_exp, d_exp);
		}
	      else
		{
		  // sig is denormal range (L-bit is 0). Set exp to zero.
		  q_exp = exp_dnrm;
		}
	    }
	  q_sig = p_sig_h;
	}

      // Round to odd from lower product bits
      p_odd = vec_addcuq (p_sig_l, (vui128_t) q_ones);
      q_sig = (vui128_t)  vec_or ((vui32_t) q_sig, (vui32_t) p_odd);

      exp_max = vec_splatd (exp_naninf_max, 1);
      // Check for exponent overflow -> __FLT128_INF__
      if  (vec_cmpud_all_gt ( q_exp, exp_max))
      {
	// Intermediate result is huge, unbiased exponent > 16383
	// so return __FLT128_MAX__
	q_exp = exp_max;
	q_sig = (vui128_t) sigov;
      }
    }
  else
    { // One or both operands are NaN or Infinity
      if (vec_cmpuq_all_eq (a_sig, (vui128_t) q_zero)
       && vec_cmpuq_all_eq (b_sig, (vui128_t) q_zero))
	{
	  // Both operands either infinity or zero
	  if (vec_cmpud_any_eq (x_exp, q_zero))
	    {
	      // Inifinty x Zero is Default Quiet NaN
	      return vec_const_nanf128 ();
	    }
	  else
	    {
	      // Infinity x Infinity == signed Infinity
	      q_sign = vec_xor (a_sign, b_sign);
	      q_exp = a_exp;
	      q_sig = a_sig;
	    }
	}
      else
	{
	  // One or both operands are NaN
	  const vui32_t q_nan = CONST_VINT128_W(0x00008000, 0, 0, 0);
	  if (vec_all_isnanf128 (vfa))
	    {
	      // vfa is NaN
	      q_sign = a_sign;
	      q_sig = (vui128_t) vec_or ((vui32_t) a_sig, q_nan);
	      q_exp = a_exp;
	    }
	  else if (vec_all_isnanf128 (vfb))
	    {
	      // vfb is NaN
	      q_sign = b_sign;
	      q_sig = (vui128_t) vec_or ((vui32_t) b_sig, q_nan);
	      q_exp = b_exp;
	    }
	  else  // OR an Infinity and a Nonzero finite number
	    {
	      q_sign = vec_xor (a_sign, b_sign);
	      q_exp = exp_naninf;
	      q_sig = (vui128_t) q_zero;
	    }
	}
    }
  // Merge sign, significand, and exponent into final result
  q_sig = (vui128_t) vec_or ((vui32_t) q_sig, q_sign);
  result = vec_xsiexpqp (q_sig, q_exp);
#else
  result = vfa * vfb;
#endif
  return result;
}

__binary128
test_vec_mulqpo_V4 (__binary128 vfa, __binary128 vfb)
{
  __binary128 result;
#if defined (_ARCH_PWR9) && (__GNUC__ > 7)
#if defined (__FLOAT128__) && (__GNUC__ > 8)
  // earlier GCC versions generate extra data moves for this.
  result = __builtin_mulf128_round_to_odd (vfa, vfb);
#else
  // No extra data moves here.
  __asm__(
      "xsmulqpo %0,%1,%2"
      : "=v" (result)
      : "v" (vfa), "v" (vfb)
      : );
#endif
#elif  defined (_ARCH_PWR7)
  vui64_t q_exp, a_exp, b_exp, x_exp;
  vui128_t q_sig, a_sig, b_sig, p_sig_h, p_sig_l, p_odd;
  vui32_t q_sign,  a_sign,  b_sign;
  const vui32_t signmask = CONST_VINT128_W(0x80000000, 0, 0, 0);
  // const vui64_t q_zero = { 0, 0 };
  const vui64_t q_zero = vec_splat_u64 (0);
  // const vui64_t q_ones = { -1, -1 };
  const vui64_t q_ones = (vui64_t) vec_splat_s64 (-1);
  // const vi64_t exp_min = (vi64_t) CONST_VINT64_DW( 1, 1 );
  const vi64_t exp_min = vec_splat_s64 (1);
  // const vui64_t exp_dnrm = (vui64_t) CONST_VINT64_DW( 0, 0 );
  const vui64_t exp_dnrm = vec_splat_u64 (0);
  const vui64_t q_naninf = (vui64_t) CONST_VINT64_DW( 0x7fff, 0x7fff );
#if defined (_ARCH_PWR8) && (__GNUC__ > 7)
  // const vui64_t q_one = { 1, 1 };
  // const vui64_t q_one = vec_splat_u64 (1);
  vui64_t exp_bias = vec_srdi (q_naninf, 1);
  vui64_t q_expmax = vec_sldi (exp_bias, 1);
#else
  const vui64_t exp_bias = (vui64_t) CONST_VINT64_DW( 0x3fff, 0x3fff );
  const vui64_t q_expmax = (vui64_t) CONST_VINT64_DW( 0x7ffe, 0x7ffe );
#endif
  const vui32_t sigov = CONST_VINT128_W(0x0001ffff, -1, -1, -1);

  a_exp = vec_xsxexpqp (vfa);
  a_sig = vec_xsxsigqp (vfa);
  a_sign = vec_and_bin128_2_vui32t (vfa, signmask);
  b_exp = vec_xsxexpqp (vfb);
  b_sig = vec_xsxsigqp (vfb);
  b_sign = vec_and_bin128_2_vui32t (vfb, signmask);
  x_exp = vec_mrgahd ((vui128_t) a_exp, (vui128_t) b_exp);
  q_sign = vec_xor (a_sign, b_sign);

//  if (vec_all_isfinitef128 (vfa) && vec_all_isfinitef128 (vfb))
  if (vec_cmpud_all_lt (x_exp, q_naninf))
    {
      const vui32_t sigovt = CONST_VINT128_W(0x0000ffff, -1, -1, -1);
      //const vui64_t q_one = { 1, 1 };
      const vui64_t q_one = vec_splat_u64 (1);;

      vui128_t p_tmp;
      // Precondition the significands before multiply so that the
      // high-order 114-bits (C,L,FRACTION) of the product are right
      // adjusted in p_sig_h. And the Low-order 112-bits are left
      // justified in p_sig_l.
      a_sig = vec_slqi (a_sig, 8);
      b_sig = vec_slqi (b_sig, 8);
      p_sig_l = vec_muludq (&p_sig_h, a_sig, b_sig);
      if (vec_cmpud_any_eq (x_exp, exp_dnrm))
	{ // Involves zeros or denormals
	  // check for zero significands in multiply
	  if (vec_cmpuq_all_eq (a_sig, (vui128_t) q_zero)
	      || vec_cmpuq_all_eq (b_sig, (vui128_t) q_zero))
	    { // Multiply by zero, return QP signed zero
	      result = vec_xfer_vui32t_2_bin128 (q_sign);
	      return result;
	    }
	  else
	    {
	      vb64_t exp_mask;
	      exp_mask = vec_cmpequd (x_exp, exp_dnrm);
	      x_exp = vec_selud (x_exp, (vui64_t) exp_min, (vb64_t) exp_mask);
	      a_exp = vec_splatd (x_exp, VEC_DW_H);
	      b_exp = vec_splatd (x_exp, VEC_DW_L);
	    }
	}
      else
	{
	  a_exp = vec_splatd (a_exp, VEC_DW_H);
	  b_exp = vec_splatd (b_exp, VEC_DW_H);

	  // Check for carry and adjust
	  if (vec_cmpuq_all_gt (p_sig_h, (vui128_t) sigov))
	    {
	      p_tmp = vec_sldqi (p_sig_h, p_sig_l, 120);
	      p_sig_h = vec_srqi (p_sig_h, 1);
	      p_sig_l = vec_slqi (p_tmp, 7);
	      a_exp = vec_addudm (a_exp, q_one);
	    }
	}
      // sum exponents
      q_exp = vec_addudm (a_exp, b_exp);
      q_exp = vec_subudm (q_exp, exp_bias);
      // There are two cases for denormal
      // 1) The sum of unbiased exponents is less the E_min (tiny).
      // 2) The significand is less then 1.0 (C and L-bits are zero).
      //  2a) The exponent is > E_min
      //  2b) The exponent is == E_min
      //
      if (vec_cmpsd_all_lt ((vi64_t) q_exp, exp_min))
	{
	  const vui64_t too_tiny = (vui64_t
		) CONST_VINT64_DW( 116, 116 );
	  const vui32_t xmask = CONST_VINT128_W(0x1fffffff, -1, -1, -1);
	  vui32_t tmp;

	  // Intermediate result is tiny, unbiased exponent < -16382
	  //x_exp = vec_subudm ((vui64_t) exp_tiny, q_exp);
	  x_exp = vec_subudm ((vui64_t) exp_min, q_exp);

	  if (vec_cmpud_all_gt ((vui64_t) x_exp, too_tiny))
	    {
	      // Intermediate result is too tiny, the shift will
	      // zero the fraction and the GR-bit leaving only the
	      // Sticky bit. The X-bit needs to include all bits
	      // from p_sig_h and p_sig_l
	      p_sig_l = vec_srqi (p_sig_l, 8);
	      p_sig_l = (vui128_t) vec_or ((vui32_t) p_sig_l,
					   (vui32_t) p_sig_h);
	      // generate a carry into bit-2 for any nonzero bits 3-127
	      p_sig_l = vec_adduqm (p_sig_l, (vui128_t) xmask);
	      q_sig = (vui128_t) q_zero;
	      p_sig_l = (vui128_t) vec_andc ((vui32_t) p_sig_l, xmask);
	    }
	  else
	    { // Normal tiny, right shift may loose low order bits
	      // from p_sig_l. So collect any 1-bits below GRX and
	      // OR them into the X-bit, before the right shift.
	      vui64_t l_exp;
	      const vui64_t exp_128 = (vui64_t
		    ) CONST_VINT64_DW( 128, 128 );

	      // Propagate low order bits into the sticky bit
	      // GRX left adjusted in p_sig_l
	      // Issolate bits below GDX (bits 3-128).
	      tmp = vec_and ((vui32_t) p_sig_l, xmask);
	      // generate a carry into bit-2 for any nonzero bits 3-127
	      tmp = (vui32_t) vec_adduqm ((vui128_t) tmp, (vui128_t) xmask);
	      // Or this with the X-bit to propagate any sticky bits into X
	      p_sig_l = (vui128_t) vec_or ((vui32_t) p_sig_l, tmp);
	      p_sig_l = (vui128_t) vec_andc ((vui32_t) p_sig_l, xmask);

	      l_exp = vec_subudm (exp_128, x_exp);
	      p_sig_l = vec_sldq (p_sig_h, p_sig_l, (vui128_t) l_exp);
	      p_sig_h = vec_srq (p_sig_h, (vui128_t) x_exp);
	      q_sig = p_sig_h;
	    }
	  q_exp = q_zero;
	}
      else
	{
	  // Check is significand is in normal range.
	  if (vec_cmpuq_all_le (p_sig_h, (vui128_t) sigovt))
	    {
	      // Is below normal range. This can happen when
	      // multiplying a denormal by a normal.
	      // So try to normalize the significand.
	      //const vui64_t exp_15 = { 15, 15 };
	      const vui64_t exp_15 = vec_splat_u64 (15);
	      vui64_t c_exp, d_exp;
	      vui128_t c_sig;
	      c_sig = vec_clzq (p_sig_h);
	      c_exp = vec_splatd ((vui64_t) c_sig, VEC_DW_L);
	      c_exp = vec_subudm (c_exp, exp_15);
	      d_exp = vec_subudm (q_exp, (vui64_t) exp_min);
	      d_exp = vec_minud (c_exp, d_exp);

	      // Intermediate result <= tiny, unbiased exponent <= -16382
	      if (vec_cmpsd_all_gt ((vi64_t) q_exp, exp_min))
		{ // Try to normalize the significand.
		  p_sig_h = vec_sldq (p_sig_h, p_sig_l, (vui128_t) d_exp);
		  p_sig_l = vec_slq (p_sig_l, (vui128_t) d_exp);
		  // Compare computed exp to shift count to normalize.
		  if (vec_cmpud_all_le (q_exp, c_exp))
		    { // exp less than shift count to normalize so
		      // result is still denormal.
		      q_exp = q_zero;
		    }
		  else // Adjust exp after normalize shift left.
		    q_exp = vec_subudm (q_exp, d_exp);
		}
	      else
		{
		  // sig is denormal range (L-bit is 0). Set exp to zero.
		  q_exp = q_zero;
		}
	    }
	  q_sig = p_sig_h;
	}

      // Round to odd from lower product bits
      p_odd = vec_addcuq (p_sig_l, (vui128_t) q_ones);
      q_sig = (vui128_t)  vec_or ((vui32_t) q_sig, (vui32_t) p_odd);

      // Check for exponent overflow -> __FLT128_INF__
      if  (vec_cmpud_all_gt ( q_exp, q_expmax))
      {
	// Intermediate result is huge, unbiased exponent > 16383
	// so return __FLT128_MAX__
	q_exp = q_expmax;
	q_sig = (vui128_t) sigov;
      }
    }
  else
    { // One or both operands are NaN or Infinity
      if (vec_cmpuq_all_eq (a_sig, (vui128_t) q_zero)
	  && vec_cmpuq_all_eq (b_sig, (vui128_t) q_zero))
	{
	  // Both operands either infinity or zero
	  if (vec_cmpud_any_eq (x_exp, q_zero))
	    {
	      // Inifinty x Zero is Default Quiet NaN
	      return vec_const_nanf128 ();
	    }
	  else
	    {
	      // Infinity x Infinity == signed Infinity
	      q_sign = vec_xor (a_sign, b_sign);
	      q_exp = a_exp;
	      q_sig = a_sig;
	    }
	}
      else
	{
	  // One or both operands are NaN
	  const vui32_t q_nan = CONST_VINT128_W(0x00008000, 0, 0, 0);
	  if (vec_all_isnanf128 (vfa))
	    {
	      // vfa is NaN
	      q_sign = a_sign;
	      q_sig = (vui128_t) vec_or ((vui32_t) a_sig, q_nan);
	      q_exp = a_exp;
	    }
	  else if (vec_all_isnanf128 (vfb))
	    {
	      // vfb is NaN
	      q_sign = b_sign;
	      q_sig = (vui128_t) vec_or ((vui32_t) b_sig, q_nan);
	      q_exp = b_exp;
	    }
	  else  // OR an Infinity and a Nonzero finite number
	    {
	      q_sign = vec_xor (a_sign, b_sign);
	      q_exp = q_naninf;
	      q_sig = (vui128_t) q_zero;
	    }
	}
    }
  // Merge sign, significand, and exponent into final result
  q_sig = (vui128_t) vec_or ((vui32_t) q_sig, q_sign);
  result = vec_xsiexpqp (q_sig, q_exp);
#else
  result = vfa * vfb;
#endif
  return result;
}

__binary128
test_vec_mulqpo_V3 (__binary128 vfa, __binary128 vfb)
{
  __binary128 result;
#if defined (_ARCH_PWR9) && (__GNUC__ > 7)
#if defined (__FLOAT128__) && (__GNUC__ > 8)
  // earlier GCC versions generate extra data moves for this.
  result = __builtin_mulf128_round_to_odd (vfa, vfb);
#else
  // No extra data moves here.
  __asm__(
      "xsmulqpo %0,%1,%2"
      : "=v" (result)
      : "v" (vfa), "v" (vfb)
      : );
#endif
#elif  defined (_ARCH_PWR7)
  vui64_t q_exp, a_exp, b_exp, x_exp;
  vui128_t q_sig, a_sig, b_sig, p_sig_h, p_sig_l, p_odd;
  vui32_t q_sign,  a_sign,  b_sign;
  const vui32_t signmask = CONST_VINT128_W(0x80000000, 0, 0, 0);
  const vui64_t q_zero = { 0, 0 };
  const vui64_t q_ones = { -1, -1 };
  const vui64_t exp_bias = (vui64_t) CONST_VINT64_DW( 0x3fff, 0x3fff );
  const vi64_t exp_min = (vi64_t) CONST_VINT64_DW( 1, 1 );
  const vui64_t exp_dnrm = (vui64_t) CONST_VINT64_DW( 0, 0 );
  const vui64_t q_naninf = (vui64_t) CONST_VINT64_DW( 0x7fff, 0x7fff );
  const vui64_t q_expmax = (vui64_t) CONST_VINT64_DW( 0x7ffe, 0x7ffe );
  const vui32_t sigov = CONST_VINT128_W(0x0001ffff, -1, -1, -1);

  a_exp = vec_xsxexpqp (vfa);
  a_sig = vec_xsxsigqp (vfa);
  a_sign = vec_and_bin128_2_vui32t (vfa, signmask);
  b_exp = vec_xsxexpqp (vfb);
  b_sig = vec_xsxsigqp (vfb);
  b_sign = vec_and_bin128_2_vui32t (vfb, signmask);
  x_exp = vec_mrgahd ((vui128_t) a_exp, (vui128_t) b_exp);
  q_sign = vec_xor (a_sign, b_sign);

//  if (vec_all_isfinitef128 (vfa) && vec_all_isfinitef128 (vfb))
  if (vec_cmpud_all_lt (x_exp, q_naninf))
    {
      const vui32_t sigovt = CONST_VINT128_W(0x0000ffff, -1, -1, -1);
      const vui64_t q_one = { 1, 1 };
      vui128_t p_tmp;
      // Precondition the significands before multiply so that the
      // high-order 114-bits (C,L,FRACTION) of the product are right
      // adjusted in p_sig_h. And the Low-order 112-bits are left
      // justified in p_sig_l.
      a_sig = vec_slqi (a_sig, 8);
      b_sig = vec_slqi (b_sig, 8);
      p_sig_l = vec_muludq (&p_sig_h, a_sig, b_sig);
      if (vec_cmpud_any_eq (x_exp, exp_dnrm))
	{ // Involves zeros or denormals
	  // check for zero significands in multiply
	  if (vec_cmpuq_all_eq (a_sig, (vui128_t) q_zero)
	      || vec_cmpuq_all_eq (b_sig, (vui128_t) q_zero))
	    { // Multiply by zero, return QP signed zero
	      result = vec_xfer_vui32t_2_bin128 (q_sign);
	      return result;
	    }
	  else
	    {
	      vb64_t exp_mask;
	      exp_mask = vec_cmpequd (x_exp, exp_dnrm);
	      x_exp = (vui64_t) vec_sel (x_exp, (vui64_t) exp_min, exp_mask);
	      a_exp = vec_splatd (x_exp, VEC_DW_H);
	      b_exp = vec_splatd (x_exp, VEC_DW_L);
	    }
	}
      else
	{
	  a_exp = vec_splatd (a_exp, VEC_DW_H);
	  b_exp = vec_splatd (b_exp, VEC_DW_H);

	  // Check for carry and adjust
	  if (vec_cmpuq_all_gt (p_sig_h, (vui128_t) sigov))
	    {
	      p_tmp = vec_sldqi (p_sig_h, p_sig_l, 120);
	      p_sig_h = vec_srqi (p_sig_h, 1);
	      p_sig_l = vec_slqi (p_tmp, 7);
	      a_exp = vec_addudm (a_exp, q_one);
	    }
	}
      // sum exponents
      q_exp = vec_addudm (a_exp, b_exp);
      q_exp = vec_subudm (q_exp, exp_bias);
      // There are two cases for denormal
      // 1) The sum of unbiased exponents is less the E_min (tiny).
      // 2) The significand is less then 1.0 (C and L-bits are zero).
      //  2a) The exponent is > E_min
      //  2b) The exponent is == E_min
      //
      if (vec_cmpsd_all_lt ((vi64_t) q_exp, exp_min))
	{
	  const vui64_t too_tiny = (vui64_t
		) CONST_VINT64_DW( 116, 116 );
	  const vui32_t xmask = CONST_VINT128_W(0x1fffffff, -1, -1, -1);
	  vui32_t tmp;

	  // Intermediate result is tiny, unbiased exponent < -16382
	  //x_exp = vec_subudm ((vui64_t) exp_tiny, q_exp);
	  x_exp = vec_subudm ((vui64_t) exp_min, q_exp);

	  if (vec_cmpud_all_gt ((vui64_t) x_exp, too_tiny))
	    {
	      // Intermediate result is too tiny, the shift will
	      // zero the fraction and the GR-bit leaving only the
	      // Sticky bit. The X-bit needs to include all bits
	      // from p_sig_h and p_sig_l
	      p_sig_l = vec_srqi (p_sig_l, 8);
	      p_sig_l = (vui128_t) vec_or ((vui32_t) p_sig_l,
					   (vui32_t) p_sig_h);
	      // generate a carry into bit-2 for any nonzero bits 3-127
	      p_sig_l = vec_adduqm (p_sig_l, (vui128_t) xmask);
	      q_sig = (vui128_t) q_zero;
	      p_sig_l = (vui128_t) vec_andc ((vui32_t) p_sig_l, xmask);
	    }
	  else
	    { // Normal tiny, right shift may loose low order bits
	      // from p_sig_l. So collect any 1-bits below GRX and
	      // OR them into the X-bit, before the right shift.
	      vui64_t l_exp;
	      const vui64_t exp_128 = (vui64_t
		    ) CONST_VINT64_DW( 128, 128 );

	      // Propagate low order bits into the sticky bit
	      // GRX left adjusted in p_sig_l
	      // Issolate bits below GDX (bits 3-128).
	      tmp = vec_and ((vui32_t) p_sig_l, xmask);
	      // generate a carry into bit-2 for any nonzero bits 3-127
	      tmp = (vui32_t) vec_adduqm ((vui128_t) tmp, (vui128_t) xmask);
	      // Or this with the X-bit to propagate any sticky bits into X
	      p_sig_l = (vui128_t) vec_or ((vui32_t) p_sig_l, tmp);
	      p_sig_l = (vui128_t) vec_andc ((vui32_t) p_sig_l, xmask);

	      l_exp = vec_subudm (exp_128, x_exp);
	      p_sig_l = vec_sldq (p_sig_h, p_sig_l, (vui128_t) l_exp);
	      p_sig_h = vec_srq (p_sig_h, (vui128_t) x_exp);
	      q_sig = p_sig_h;
	    }
	  q_exp = q_zero;
	}
      else
	{
	  // Check is significand is in normal range.
	  if (vec_cmpuq_all_le (p_sig_h, (vui128_t) sigovt))
	    {
	      // Is below normal range. This can happen when
	      // multiplying a denormal by a normal.
	      // So try to normalize the significand.
	      const vui64_t exp_15 = { 15, 15 };
	      vui64_t c_exp, d_exp;
	      vui128_t c_sig;
	      c_sig = vec_clzq (p_sig_h);
	      c_exp = vec_splatd ((vui64_t) c_sig, VEC_DW_L);
	      c_exp = vec_subudm (c_exp, exp_15);
	      d_exp = vec_subudm (q_exp, (vui64_t) exp_min);
	      d_exp = vec_minud (c_exp, d_exp);

	      // Intermediate result <= tiny, unbiased exponent <= -16382
	      if (vec_cmpsd_all_gt ((vi64_t) q_exp, exp_min))
		{ // Try to normalize the significand.
		  p_sig_h = vec_sldq (p_sig_h, p_sig_l, (vui128_t) d_exp);
		  p_sig_l = vec_slq (p_sig_l, (vui128_t) d_exp);
		  // Compare computed exp to shift count to normalize.
		  if (vec_cmpud_all_le (q_exp, c_exp))
		    { // exp less than shift count to normalize so
		      // result is still denormal.
		      q_exp = q_zero;
		    }
		  else // Adjust exp after normalize shift left.
		    q_exp = vec_subudm (q_exp, d_exp);
		}
	      else
		{
		  // sig is denormal range (L-bit is 0). Set exp to zero.
		  q_exp = q_zero;
		}
	    }
	  q_sig = p_sig_h;
	}

      // Round to odd from lower product bits
      p_odd = vec_addcuq (p_sig_l, (vui128_t) q_ones);
      q_sig = (vui128_t)  vec_or ((vui32_t) q_sig, (vui32_t) p_odd);

      // Check for exponent overflow -> __FLT128_INF__
      if  (vec_cmpud_all_gt ( q_exp, q_expmax))
      {
	// Intermediate result is huge, unbiased exponent > 16383
	// so return __FLT128_MAX__
	q_exp = q_expmax;
	q_sig = (vui128_t) sigov;
      }
    }
  else
    { // One or both operands are NaN or Infinity
      if (vec_cmpuq_all_eq (a_sig, (vui128_t) q_zero)
	  && vec_cmpuq_all_eq (b_sig, (vui128_t) q_zero))
	{
	  // Both operands either infinity or zero
	  if (vec_cmpud_any_eq (x_exp, q_zero))
	    {
	      // Inifinty x Zero is Default Quiet NaN
	      return vec_const_nanf128 ();
	    }
	  else
	    {
	      // Infinity x Infinity == signed Infinity
	      q_sign = vec_xor (a_sign, b_sign);
	      q_exp = a_exp;
	      q_sig = a_sig;
	    }
	}
      else
	{
	  // One or both operands are NaN
	  const vui32_t q_nan = CONST_VINT128_W(0x00008000, 0, 0, 0);
	  if (vec_all_isnanf128 (vfa))
	    {
	      // vfa is NaN
	      q_sign = a_sign;
	      q_sig = (vui128_t) vec_or ((vui32_t) a_sig, q_nan);
	      q_exp = a_exp;
	    }
	  else if (vec_all_isnanf128 (vfb))
	    {
	      // vfb is NaN
	      q_sign = b_sign;
	      q_sig = (vui128_t) vec_or ((vui32_t) b_sig, q_nan);
	      q_exp = b_exp;
	    }
	  else  // OR an Infinity and a Nonzero finite number
	    {
	      q_sign = vec_xor (a_sign, b_sign);
	      q_exp = q_naninf;
	      q_sig = (vui128_t) q_zero;
	    }
	}
    }
  // Merge sign, significand, and exponent into final result
  q_sig = (vui128_t) vec_or ((vui32_t) q_sig, q_sign);
  result = vec_xsiexpqp (q_sig, q_exp);
#else
  result = vfa * vfb;
#endif
  return result;
}

__binary128
test_vec_mulqpo_V2 (__binary128 vfa, __binary128 vfb)
{
  __binary128 result;
#if defined (_ARCH_PWR9) && (__GNUC__ > 7)
#if defined (__FLOAT128__) && (__GNUC__ > 8)
  // earlier GCC versions generate extra data moves for this.
  result = __builtin_mulf128_round_to_odd (vfa, vfb);
#else
  // No extra data moves here.
  __asm__(
      "xsmulqpo %0,%1,%2"
      : "=v" (result)
      : "v" (vfa), "v" (vfb)
      : );
#endif
#elif  defined (_ARCH_PWR7)
  vui64_t q_exp, a_exp, b_exp, x_exp;
  vui128_t q_sig, a_sig, b_sig, p_sig_h, p_sig_l, p_odd;
  vui32_t q_sign,  a_sign,  b_sign;
  const vui32_t signmask = CONST_VINT128_W(0x80000000, 0, 0, 0);
  const vui64_t q_zero = { 0, 0 };
  const vui64_t q_ones = { -1, -1 };
  const vui64_t exp_low = (vui64_t) CONST_VINT64_DW( 0x3fff, 0x3fff );
  const vi64_t exp_tiny = (vi64_t) CONST_VINT64_DW( 0, 0 );
  const vui64_t q_naninf = (vui64_t) CONST_VINT64_DW( 0x7fff, 0x7fff );
  const vui64_t q_expmax = (vui64_t) CONST_VINT64_DW( 0x7ffe, 0x7ffe );
  const vui32_t sigov = CONST_VINT128_W(0x0001ffff, -1, -1, -1);

  a_exp = vec_xsxexpqp (vfa);
  a_sig = vec_xsxsigqp (vfa);
  a_sign = vec_and_bin128_2_vui32t (vfa, signmask);
  b_exp = vec_xsxexpqp (vfb);
  b_sig = vec_xsxsigqp (vfb);
  b_sign = vec_and_bin128_2_vui32t (vfb, signmask);
  x_exp = vec_mrgahd ((vui128_t) a_exp, (vui128_t) b_exp);
  q_sign = vec_xor (a_sign, b_sign);

//  if (vec_all_isfinitef128 (vfa) && vec_all_isfinitef128 (vfb))
  if (vec_cmpud_all_lt (x_exp, q_naninf))
    {
      const vui32_t sigovt = CONST_VINT128_W(0x0000ffff, -1, -1, -1);
      const vui64_t q_one = { 1, 1 };
      vui128_t p_tmp;
      // Precondition the significands before multiply so that the
      // high-order 114-bits (C,L,FRACTION) of the product are right
      // adjusted in p_sig_h. And the Low-order 112-bits are left
      // justified in p_sig_l.
      a_sig = vec_slqi (a_sig, 8);
      b_sig = vec_slqi (b_sig, 8);
      p_sig_l = vec_muludq (&p_sig_h, a_sig, b_sig);
      a_exp = vec_splatd (a_exp, VEC_DW_H);
      b_exp = vec_splatd (b_exp, VEC_DW_H);
      // sum exponents
      q_exp = vec_addudm (a_exp, b_exp);
      q_exp = vec_subudm (q_exp, exp_low);
      if (vec_cmpuq_all_eq (a_sig, (vui128_t) q_zero)
	  || vec_cmpuq_all_eq (b_sig, (vui128_t) q_zero))
	{// Multiply by zero
	  q_sig = (vui128_t) q_zero;
	  q_exp = q_zero;
	}
      else if (vec_cmpsd_all_le ((vi64_t) q_exp, exp_tiny))
	{
	  if (vec_cmpsd_all_eq ((vi64_t) q_exp, exp_tiny))
	    {
	      // Intermediate result == tiny, unbiased exponent == -16382
	      // Check if sig exceeds denormal range (L-bit is 1).
	      if (vec_cmpuq_all_gt (p_sig_h, (vui128_t) sigovt))
		{
		  q_exp = vec_addudm (q_exp, q_one);
		}

	      q_sig = p_sig_h;
	    }
	  else
	    {
	      const vui64_t too_tiny = (vui64_t) CONST_VINT64_DW( 116, 116 );
	      const vui32_t xmask = CONST_VINT128_W(0x1fffffff, -1, -1, -1);
	      vui32_t tmp;

	      // Intermediate result is tiny, unbiased exponent < -16382
	      x_exp = vec_subudm ((vui64_t) exp_tiny, q_exp);

	      if  (vec_cmpud_all_gt ((vui64_t) x_exp, too_tiny))
		{
		  // Intermediate result is too tiny, the shift will
		  // zero the fraction and the GR-bit leaving only the
		  // Sticky bit. The X-bit needs to include all bits
		  // from p_sig_h and p_sig_l
		  p_sig_l = vec_srqi (p_sig_l, 16);
		  p_sig_l = (vui128_t) vec_or ((vui32_t) p_sig_l, (vui32_t) p_sig_h);
		  // generate a carry into bit-2 for any nonzero bits 3-127
		  p_sig_l = vec_adduqm (p_sig_l, (vui128_t) xmask);
		  q_sig = (vui128_t) q_zero;
		  p_sig_l = (vui128_t) vec_andc ((vui32_t) p_sig_l, xmask);
		}
	      else
		{ // Normal tiny, right shift may loose low order bits
		  // from p_sig_l. So collect any 1-bits below GRX and
		  // OR them into the X-bit, before the right shift
		  // Propagate low order bits into the sticky bit
		  // GRX left adjusted in p_sig_l
		  // Isolate bits below GDX (bits 3-128).
		  tmp = vec_and ((vui32_t) p_sig_l, xmask);
		  // generate a carry into bit-2 for any nonzero bits 3-127
		  tmp = (vui32_t) vec_adduqm ((vui128_t) tmp, (vui128_t) xmask);
		  // Or this with the X-bit to propagate any sticky bits into X
		  p_sig_l = (vui128_t) vec_or ((vui32_t) p_sig_l, tmp);
		  p_sig_l = (vui128_t) vec_andc ((vui32_t) p_sig_l, xmask);
		  q_sig = vec_srq (p_sig_h, (vui128_t) x_exp);
		  p_sig_l = vec_sldq (p_sig_h, p_sig_l, (vui128_t) q_exp);
		}
	      q_exp = q_zero;
	    }
	}
      else if (vec_cmpuq_all_gt (p_sig_h, (vui128_t) sigov))
	{ // We have carry out of the L-bit into the C-bit.
	  // Shift C/L/FRACTION bits right 1 bit.
	  q_sig = vec_srqi (p_sig_h, 1);
//	  p_sig_l = vec_sldqi (p_sig_h, p_sig_l, 127);
	  // Shift the low order FRACTION and GRX right 1 bit.
	  p_tmp = vec_sldqi (p_sig_h, p_sig_l, 120);
	  p_sig_l = vec_slqi (p_tmp, 7);
	  // Increment the exponent by 1
	  q_exp = vec_addudm (q_exp, q_one);
	}
      else
	  q_sig = p_sig_h;

      // Round to odd from lower product bits
      p_odd = vec_addcuq (p_sig_l, (vui128_t) q_ones);
      q_sig = (vui128_t)  vec_or ((vui32_t) q_sig, (vui32_t) p_odd);

      // Check for exponent overflow -> __FLT128_MAX__
      if  (vec_cmpud_all_gt ( q_exp, q_expmax))
      {
	// Intermediate result is huge, unbiased exponent > 16383
	q_exp = q_expmax;
	q_sig = (vui128_t) sigov;
      }
      // Merge sign, significand, and exponent into final result
      q_sig = (vui128_t) vec_or ((vui32_t) q_sig, q_sign);
      result = vec_xsiexpqp (q_sig, q_exp);
    }
  else
    { // One or both operands are NaN or Infinity
      if (vec_cmpuq_all_eq (a_sig, (vui128_t) q_zero)
	  && vec_cmpuq_all_eq (b_sig, (vui128_t) q_zero))
	{
	  // Both operands either infinity or zero
	  if (vec_cmpud_any_eq (x_exp, q_zero))
	    {
	      // Inifinty x Zero is Default Quiet NaN
	      return vec_const_nanf128 ();
	    }
	  else
	    {
	      // Infinity x Infinity == signed Infinity
	      q_sign = vec_xor (a_sign, b_sign);
	      q_exp = a_exp;
	      q_sig = a_sig;
	    }
	}
      else
	{
	  // One or both operands are NaN
	  const vui32_t q_nan = CONST_VINT128_W(0x00008000, 0, 0, 0);
	  if (vec_all_isnanf128 (vfa))
	    {
	      // vfa is NaN
	      q_sign = a_sign;
	      q_sig = (vui128_t) vec_or ((vui32_t) a_sig, q_nan);
	      q_exp = a_exp;
	    }
	  else if (vec_all_isnanf128 (vfb))
	    {
	      // vfb is NaN
	      q_sign = b_sign;
	      q_sig = (vui128_t) vec_or ((vui32_t) b_sig, q_nan);
	      q_exp = b_exp;
	    }
	  else  // OR an Infinity and a Nonzero finite number
	    {
	      q_sign = vec_xor (a_sign, b_sign);
	      q_exp = q_naninf;
	      q_sig = (vui128_t) q_zero;
	    }
	}
      // Insert exponent into significand to complete conversion to QP
      q_sig = (vui128_t) vec_or ((vui32_t) q_sig, q_sign);
      result = vec_xsiexpqp (q_sig, q_exp);
    }
#else
  result = vfa * vfb;
#endif
  return result;
}

__binary128
test_mulqpo_V1 (__binary128 vfa, __binary128 vfb)
{
  __binary128 result;
#if defined (_ARCH_PWR9) && (__GNUC__ > 7)
#if defined (__FLOAT128__) && (__GNUC__ > 8)
  // earlier GCC versions generate extra data moves for this.
  result = __builtin_mulf128_round_to_odd (vfa, vfb);
#else
  // No extra data moves here.
  __asm__(
      "xsmulqpo %0,%1,%2"
      : "=v" (result)
      : "v" (vfa), "v" (vfb)
      : );
#endif
#elif  defined (_ARCH_PWR7)
  vui64_t q_exp, a_exp, b_exp, x_exp;
  vui128_t q_sig, a_sig, b_sig, p_sig_h, p_sig_l, p_odd;
  vui32_t q_sign,  a_sign,  b_sign;
  const vui32_t signmask = CONST_VINT128_W(0x80000000, 0, 0, 0);
  const vui64_t q_zero = { 0, 0 };
  const vui64_t q_ones = { -1, -1 };
  const vui64_t exp_low = (vui64_t) CONST_VINT64_DW( 0x3fff, 0x3fff );
  const vi64_t exp_tiny = (vi64_t) CONST_VINT64_DW( 0, 0 );
  const vui64_t q_naninf = (vui64_t) CONST_VINT64_DW( 0x7fff, 0x7fff );
  const vui64_t q_expmax = (vui64_t) CONST_VINT64_DW( 0x7ffe, 0x7ffe );
  const vui32_t sigov = CONST_VINT128_W(0x0001ffff, -1, -1, -1);

  a_exp = vec_xsxexpqp (vfa);
  a_sig = vec_xsxsigqp (vfa);
  a_sign = vec_and_bin128_2_vui32t (vfa, signmask);
  b_exp = vec_xsxexpqp (vfb);
  b_sig = vec_xsxsigqp (vfb);
  b_sign = vec_and_bin128_2_vui32t (vfb, signmask);
  x_exp = vec_mrgahd ((vui128_t) a_exp, (vui128_t) b_exp);
  q_sign = vec_xor (a_sign, b_sign);

  // if (vec_all_isfinitef128 (vfa) && vec_all_isfinitef128 (vfb))
  // Simplifies to
  if (vec_cmpud_all_lt (x_exp, q_naninf))
    {
      // Both operands are finite
      const vui32_t sigovt = CONST_VINT128_W(0x0000ffff, -1, -1, -1);
      const vui64_t q_one = { 1, 1 };
      // Precondition the significands before multiply so that the
      // high-order 113-bit of the product are right adjusted in p_sig_h.
      a_sig = vec_slqi (a_sig, 8);
      b_sig = vec_slqi (b_sig, 8);
      p_sig_l = vec_muludq (&p_sig_h, a_sig, b_sig);
      a_exp = vec_splatd (a_exp, VEC_DW_H);
      b_exp = vec_splatd (b_exp, VEC_DW_H);
      // Insert exponent into significand to complete conversion to QP
      q_exp = vec_addudm (a_exp, b_exp);
      q_exp = vec_subudm (q_exp, exp_low);
      if (vec_cmpuq_all_eq (a_sig, (vui128_t) q_zero)
	  || vec_cmpuq_all_eq (b_sig, (vui128_t) q_zero))
	{// Multiply by zero
	  q_sig = (vui128_t) q_zero;
	  q_exp = q_zero;
	}
      else if (vec_cmpsd_all_le ((vi64_t) q_exp, exp_tiny))
	{
	  if (vec_cmpsd_all_eq ((vi64_t) q_exp, exp_tiny))
	    {
	      // Intermediate result == tiny, unbiased exponent == -16382
	      // Check if sig exceeds tiny exponent
	      if (vec_cmpuq_all_gt (p_sig_h, (vui128_t) sigovt))
		{
		  q_exp = vec_addudm (q_exp, q_one);
		}

	      q_sig = p_sig_h;
	    }
	  else
	    {
	      // Intermediate result is tiny, unbiased exponent < -16382
	      x_exp = vec_subudm ((vui64_t) exp_tiny, q_exp);
	      q_sig = vec_srq (p_sig_h, (vui128_t) x_exp);
	      p_sig_l = vec_sldq (p_sig_h, p_sig_l, (vui128_t) q_exp);
	      q_exp = q_zero;
	    }
	}
      else if (vec_cmpuq_all_gt (p_sig_h, (vui128_t) sigov))
	{
	  q_sig = vec_srqi (p_sig_h, 1);
	  p_sig_l = vec_sldqi (p_sig_h, p_sig_l, 127);
	  q_exp = vec_addudm (q_exp, q_one);
	}
      else
	  q_sig = p_sig_h;

      // Round to odd from lower product bits
      p_odd = vec_addcuq (p_sig_l, (vui128_t) q_ones);
      q_sig = (vui128_t)  vec_or ((vui32_t) q_sig, (vui32_t) p_odd);

      // Check for exponent overflow -> INF
      if  (vec_cmpud_all_gt ( q_exp, q_expmax))
      {
	// Intermediate result is huge, unbiased exponent > 16383
	// I would have expect Infinity, But hardware return MaxQP
	q_exp = q_expmax;
	q_sig = (vui128_t) sigov;
      }

      q_sig = (vui128_t) vec_or ((vui32_t) q_sig, q_sign);
      result = vec_xsiexpqp (q_sig, q_exp);
    }
  else
    { // One or both operands are NaN or Infinity
      if (vec_cmpuq_all_eq (a_sig, (vui128_t) q_zero)
	  && vec_cmpuq_all_eq (b_sig, (vui128_t) q_zero))
	{
	  // Both operands either infinity or zero
	  if (vec_cmpud_any_eq (x_exp, q_zero))
	    {
	      // Inifinty x Zero is Default Quiet NaN
	      return vec_const_nanf128 ();
	    }
	  else
	    {
	      // Infinity x Infinity == signed Infinity
	      q_sign = vec_xor (a_sign, b_sign);
	      q_exp = a_exp;
	      q_sig = a_sig;
	    }
	}
      else
	{
	  // One or both operands are NaN
	  const vui32_t q_nan = CONST_VINT128_W(0x00008000, 0, 0, 0);
	  if (vec_all_isnanf128 (vfa))
	    {
	      // vfa is NaN
	      q_sign = a_sign;
	      q_sig = (vui128_t) vec_or ((vui32_t) a_sig, q_nan);
	      q_exp = a_exp;
	    }
	  else if (vec_all_isnanf128 (vfb))
	    {
	      // vfb is NaN
	      q_sign = b_sign;
	      q_sig = (vui128_t) vec_or ((vui32_t) b_sig, q_nan);
	      q_exp = b_exp;
	    }
	  else  // OR an Infinity and a Nonzero finite number
	    {
	      q_sign = vec_xor (a_sign, b_sign);
	      q_exp = q_naninf;
	      q_sig = (vui128_t) q_zero;
	    }
	}
      // Insert exponent into significand to complete conversion to QP
      q_sig = (vui128_t) vec_or ((vui32_t) q_sig, q_sign);
      result = vec_xsiexpqp (q_sig, q_exp);
    }
#else
  result = vfa * vfb;
#endif
  return result;
}

__binary128
test_mulqpo_V0 (__binary128 vfa, __binary128 vfb)
{
  __binary128 result;
#if defined (_ARCH_PWR9) && (__GNUC__ > 7)
#if defined (__FLOAT128__) && (__GNUC__ > 8)
  // earlier GCC versions generate extra data moves for this.
  result = __builtin_mulf128_round_to_odd (vfa, vfb);
#else
  // No extra data moves here.
  __asm__(
      "xsmulqpo %0,%1,%2"
      : "=v" (result)
      : "v" (vfa), "v" (vfb)
      : );
#endif
#elif  defined (_ARCH_PWR7)
  vui64_t q_exp, a_exp, b_exp, x_exp;
  vui128_t q_sig, a_sig, b_sig, p_sig_h, p_sig_l, p_odd;
  vui32_t q_sign,  a_sign,  b_sign;
  const vui32_t signmask = CONST_VINT128_W(0x80000000, 0, 0, 0);
  const vui64_t q_zero = { 0, 0 };
  const vui64_t q_ones = { -1, -1 };
  const vui64_t exp_low = (vui64_t) CONST_VINT64_DW( 0x3fff, 0x3fff );
  const vi64_t exp_tiny = (vi64_t) CONST_VINT64_DW( 0, 0 );
  const vui64_t q_naninf = (vui64_t) CONST_VINT64_DW( 0x7fff, 0x7fff );
  const vui64_t q_expmax = (vui64_t) CONST_VINT64_DW( 0x7ffe, 0x7ffe );
  const vui32_t sigov = CONST_VINT128_W(0x0001ffff, -1, -1, -1);

  a_exp = vec_xsxexpqp (vfa);
  a_sig = vec_xsxsigqp (vfa);
  a_sign = vec_and_bin128_2_vui32t (vfa, signmask);
  b_exp = vec_xsxexpqp (vfb);
  b_sig = vec_xsxsigqp (vfb);
  b_sign = vec_and_bin128_2_vui32t (vfb, signmask);
  x_exp = vec_mrgahd ((vui128_t) a_exp, (vui128_t) b_exp);
  q_sign = vec_xor (a_sign, b_sign);

  if (vec_all_isfinitef128 (vfa) && vec_all_isfinitef128 (vfb))
    {
      const vui32_t sigovt = CONST_VINT128_W(0x0000ffff, -1, -1, -1);
      const vui64_t q_one = { 1, 1 };
      // Precondition the significands before multiply so that the
      // high-order 113-bit of the product are right adjusted in p_sig_h.
      a_sig = vec_slqi (a_sig, 8);
      b_sig = vec_slqi (b_sig, 8);
      p_sig_l = vec_muludq (&p_sig_h, a_sig, b_sig);
      a_exp = vec_splatd (a_exp, VEC_DW_H);
      b_exp = vec_splatd (b_exp, VEC_DW_H);
      // Insert exponent into significand to complete conversion to QP
      q_exp = vec_addudm (a_exp, b_exp);
      q_exp = vec_subudm (q_exp, exp_low);
      if (vec_cmpuq_all_eq (a_sig, (vui128_t) q_zero)
	  || vec_cmpuq_all_eq (b_sig, (vui128_t) q_zero))
	{// Multiply by zero
	  q_sig = (vui128_t) q_zero;
	  q_exp = q_zero;
	}
      else if (vec_cmpsd_all_le ((vi64_t) q_exp, exp_tiny))
	{
	  if (vec_cmpsd_all_eq ((vi64_t) q_exp, exp_tiny))
	    {
	      // Intermediate result == tiny, unbiased exponent == -16382
	      // Check if sig exceeds tiny exponent
	      if (vec_cmpuq_all_gt (p_sig_h, (vui128_t) sigovt))
		{
		  q_exp = vec_addudm (q_exp, q_one);
		}

	      q_sig = p_sig_h;
	    }
	  else
	    {
	      // Intermediate result is tiny, unbiased exponent < -16382
	      x_exp = vec_subudm ((vui64_t) exp_tiny, q_exp);
	      q_sig = vec_srq (p_sig_h, (vui128_t) x_exp);
	      p_sig_l = vec_sldq (p_sig_h, p_sig_l, (vui128_t) q_exp);
	      q_exp = q_zero;
	    }
	}
      else if (vec_cmpuq_all_gt (p_sig_h, (vui128_t) sigov))
	{
	  q_sig = vec_srqi (p_sig_h, 1);
	  p_sig_l = vec_sldqi (p_sig_h, p_sig_l, 127);
	  q_exp = vec_addudm (q_exp, q_one);
	}
      else
	  q_sig = p_sig_h;

      // Round to odd from lower product bits
      p_odd = vec_addcuq (p_sig_l, (vui128_t) q_ones);
      q_sig = (vui128_t)  vec_or ((vui32_t) q_sig, (vui32_t) p_odd);

      // Check for exponent overflow -> INF
      if  (vec_cmpud_all_gt ( q_exp, q_expmax))
      {
	// Intermediate result is huge, unbiased exponent > 16383
	// I would have expect Infinity, But hardware return MaxQP
	q_exp = q_expmax;
	q_sig = (vui128_t) sigov;
      }

      q_sig = (vui128_t) vec_or ((vui32_t) q_sig, q_sign);
      result = vec_xsiexpqp (q_sig, q_exp);
    }
  else
    { // One or both operands are NaN or Infinity
      if (vec_cmpuq_all_eq (a_sig, (vui128_t) q_zero)
	  && vec_cmpuq_all_eq (b_sig, (vui128_t) q_zero))
	{
	  // Both operands either infinity or zero
	  if (vec_cmpud_any_eq (x_exp, q_zero))
	    {
	      // Inifinty x Zero is Default Quiet NaN
	      return vec_const_nanf128 ();
	    }
	  else
	    {
	      // Infinity x Infinity == signed Infinity
	      q_sign = vec_xor (a_sign, b_sign);
	      q_exp = a_exp;
	      q_sig = a_sig;
	    }
	}
      else
	{
	  // One or both operands are NaN
	  const vui32_t q_nan = CONST_VINT128_W(0x00008000, 0, 0, 0);
	  if (vec_all_isnanf128 (vfa))
	    {
	      // vfa is NaN
	      q_sign = a_sign;
	      q_sig = (vui128_t) vec_or ((vui32_t) a_sig, q_nan);
	      q_exp = a_exp;
	    }
	  else if (vec_all_isnanf128 (vfb))
	    {
	      // vfb is NaN
	      q_sign = b_sign;
	      q_sig = (vui128_t) vec_or ((vui32_t) b_sig, q_nan);
	      q_exp = b_exp;
	    }
	  else  // OR an Infinity and a Nonzero finite number
	    {
	      q_sign = vec_xor (a_sign, b_sign);
	      q_exp = q_naninf;
	      q_sig = (vui128_t) q_zero;
	    }
	}
      // Insert exponent into significand to complete conversion to QP
      q_sig = (vui128_t) vec_or ((vui32_t) q_sig, q_sign);
      result = vec_xsiexpqp (q_sig, q_exp);
    }
#else
  result = vfa * vfb;
#endif
  return result;
}
#endif
#endif

int
test_vec_cmpqp_exp_eq (__binary128 vfa, __binary128 vfb)
{
  return vec_cmpqp_exp_eq ( vfa, vfb);
}

int
test_vec_cmpqp_exp_gt (__binary128 vfa, __binary128 vfb)
{
  return vec_cmpqp_exp_gt ( vfa, vfb);
}

int
test_vec_cmpqp_exp_lt (__binary128 vfa, __binary128 vfb)
{
  return vec_cmpqp_exp_lt ( vfa, vfb);
}

int
test_vec_cmpqp_exp_unordered (__binary128 vfa, __binary128 vfb)
{
  return vec_cmpqp_exp_unordered ( vfa, vfb);
}

int
test_vec_cmpqp_all_tone (__binary128 vfa, __binary128 vfb)
{
  return vec_cmpqp_all_tone (vfa, vfb);
}

int
test_vec_cmpqp_all_uzne (__binary128 vfa, __binary128 vfb)
{
  return vec_cmpqp_all_uzne (vfa, vfb);
}

int
test_vec_cmpqp_all_ne (__binary128 vfa, __binary128 vfb)
{
  return vec_cmpqp_all_ne (vfa, vfb);
}

int
test_vec_cmpqp_all_tole (__binary128 vfa, __binary128 vfb)
{
  return vec_cmpqp_all_tole (vfa, vfb);
}

int
test_vec_cmpqp_all_uzle (__binary128 vfa, __binary128 vfb)
{
  return vec_cmpqp_all_uzle (vfa, vfb);
}

int
test_vec_cmpqp_all_le (__binary128 vfa, __binary128 vfb)
{
  return vec_cmpqp_all_le (vfa, vfb);
}

int
test_vec_cmpqp_all_toge (__binary128 vfa, __binary128 vfb)
{
  return vec_cmpqp_all_toge (vfa, vfb);
}

int
test_vec_cmpqp_all_uzge (__binary128 vfa, __binary128 vfb)
{
  return vec_cmpqp_all_uzge (vfa, vfb);
}

int
test_vec_cmpqp_all_ge (__binary128 vfa, __binary128 vfb)
{
  return vec_cmpqp_all_ge (vfa, vfb);
}

int
test_vec_cmpqp_all_tolt (__binary128 vfa, __binary128 vfb)
{
  return vec_cmpqp_all_tolt (vfa, vfb);
}

int
test_vec_cmpqp_all_uzlt (__binary128 vfa, __binary128 vfb)
{
  return vec_cmpqp_all_uzlt (vfa, vfb);
}

int
test_vec_cmpqp_all_lt (__binary128 vfa, __binary128 vfb)
{
  return vec_cmpqp_all_lt (vfa, vfb);
}

int
test_vec_cmpqp_all_togt (__binary128 vfa, __binary128 vfb)
{
  return vec_cmpqp_all_togt (vfa, vfb);
}

int
test_vec_cmpqp_all_uzgt (__binary128 vfa, __binary128 vfb)
{
  return vec_cmpqp_all_uzgt (vfa, vfb);
}

int
test_vec_cmpqp_all_gt (__binary128 vfa, __binary128 vfb)
{
  return vec_cmpqp_all_gt (vfa, vfb);
}

int
test_vec_all_isunorderedf128 (__binary128 vfa, __binary128 vfb)
{
  return vec_all_isunorderedf128 (vfa, vfb);
}

vb128_t
test_vec_isunorderedf128 (__binary128 vfa, __binary128 vfb)
{
  return vec_isunorderedf128 (vfa, vfb);
}

int
test_vec_cmpqp_all_toeq (__binary128 vfa, __binary128 vfb)
{
  return vec_cmpqp_all_toeq (vfa, vfb);
}

int
test_vec_cmpqp_all_uzeq (__binary128 vfa, __binary128 vfb)
{
  return vec_cmpqp_all_uzeq (vfa, vfb);
}

int
test_vec_cmpqp_all_eq (__binary128 vfa, __binary128 vfb)
{
  return vec_cmpqp_all_eq (vfa, vfb);
}

int
test_scalar_test_neg (__binary128 vfa)
{
  return vec_signbitf128 (vfa);
}

vf64_t
test_vec_xscvqpdpo (__binary128 f128)
{
  return vec_xscvqpdpo_inline (f128);
}

vui64_t
test_vec_xscvqpudz (__binary128 f128)
{
  return vec_xscvqpudz_inline (f128);
}

vui128_t
test_vec_xscvqpuqz (__binary128 f128)
{
  return vec_xscvqpuqz_inline (f128);
}

// Convert Float DP to QP
__binary128
test_vec_xscvdpqp (vf64_t f64)
{
  return vec_xscvdpqp_inline (f64);
}

// Convert Integer QW to QP
__binary128
test_vec_xscvsqqp (vi128_t int128)
{
  return vec_xscvsqqp_inline (int128);
}

__binary128
test_vec_xscvuqqp (vui128_t int128)
{
  return vec_xscvuqqp_inline (int128);
}

__binary128
test_vec_xscvudqp (vui64_t int64)
{
  return vec_xscvudqp_inline (int64);
}

__binary128
test_vec_xscvsdqp (vi64_t int64)
{
  return vec_xscvsdqp_inline (int64);
}


#ifdef PVECLIB_OLDTESTVERSIONS
__binary128
test_convert_uqqpn (vui128_t int128)
{
  __binary128 result;
#if defined (_ARCH_PWR10)  && (__GNUC__ >= 10)
  __asm__(
      "xscvuqqp %0,%1"
      : "=v" (result)
      : "v" (int128)
      : );
#elif defined (_ARCH_PWR9) && defined (__FLOAT128__) && (__GNUC__ > 7)
  vui64_t int64 = (vui64_t) int128;
  __binary128 hi64, lo64;
  __binary128 two64 = 0x1.0p64;
  hi64 = int64[VEC_DW_H];
  lo64 = int64[VEC_DW_L];
  result = (hi64 * two64) + lo64;
#elif  defined (_ARCH_PWR8)
  vui64_t q_exp;
  vui128_t q_sig;
  const vui128_t q_zero = (vui128_t) { 0 };
  const vui32_t lowmask = CONST_VINT128_W( 0, 0, 0, 1);

//  int64[VEC_DW_L] = 0UL; // clear the right most element to zero.
  q_sig = int128;
  // Quick test for 0UL as this case requires a special exponent.
  if (vec_cmpuq_all_eq (q_sig, q_zero))
    {
      result = vec_xfer_vui128t_2_bin128 (q_zero);
    }
  else
    { // We need to produce a normal QP, so we treat the integer like a
      // denormal, then normalize it.
      // Start with the quad exponent bias + 127 then subtract the count of
      // leading '0's. The 128-bit sig can have 0-127 leading '0's.
      vui64_t q_expm = (vui64_t) CONST_VINT64_DW(0, (0x3fff + 127));
      vui64_t i64_clz = (vui64_t) vec_clzq (q_sig);
      q_sig = vec_slq (q_sig, (vui128_t) i64_clz);
      q_exp = vec_subudm (q_expm, i64_clz);
      // This is the part that might require rounding.
#if 1
      // The Significand (including the L-bit) is right justified in
      // are in the high-order 113-bits of q_sig.
      // The guard, round, and sticky (GRX) bits are in the low-order
      // 15 bits.
      // The sticky-bits are the last 13 bits and are logically ORed
      // (or added to 0x1fff) to produce the X-bit.
      //
      // For "round to Nearest, ties to even".
      // GRX = 0b001 - 0b011; truncate
      // GRX = 0b100 and bit-112 is odd; round up, otherwise truncate
      // GRX = 0b100 - 0b111; round up
      // We can simplify by copying bit-112 and OR it with bit-X
      // Then add 0x3fff to q_sig will generate a carry into bit-112
      // if and only if GRX > 0b100 or (GRX == 0b100) && (bit-112 == 1)
      const vui32_t RXmask = CONST_VINT128_W( 0, 0, 0, 0x3fff);
      vui128_t q_carry, q_sigc;
      vb128_t qcmask;
      vui32_t q_odd;
      // Isolate bit-112 and OR into GRX bits if q_sig is odd
      q_odd = (vui32_t) vec_srhi ((vui16_t)q_sig, 15);
      q_odd = vec_and (q_odd, lowmask);
      q_sig = (vui128_t) vec_or ((vui32_t) q_sig, q_odd);
      // We add 0x3fff to GRX which may carry into low order sig-bit
      // This may result in a carry out of bit L into bit-C.
      q_carry = vec_addcuq (q_sig, (vui128_t) RXmask);
      q_sig = vec_adduqm (q_sig, (vui128_t) RXmask);
      // Generate a bool mask from the carry to use in the vsel
      qcmask = vec_setb_cyq (q_carry);
      // Two cases; 1) We did carry so shift (double) left 112 bits
      q_sigc = vec_sldqi (q_carry, q_sig, 112);
      // 2) no carry so shift left 15 bits
      q_sig = vec_srqi ((vui128_t) q_sig, 15);
      // Select which based on carry
      q_sig = (vui128_t) vec_sel ((vui32_t) q_sig, (vui32_t) q_sigc, (vui32_t) qcmask);
      // Increment the exponent based on the carry
      q_exp = vec_addudm (q_exp, (vui64_t) q_carry);
#else
      const vui32_t q_carry = CONST_VINT128_W(0x20000, 0, 0, 0);
      const vui32_t nlmask = CONST_VINT128_W( 0x7fffffff, -1, -1, -1);
      vui32_t q_GRX, q_low;
      vui128_t q_rnd;
      // We need to separate the Significand
      // from the guard, round, and sticky (GRX) bits
      // Left justify the GRX bits
      q_GRX = (vui32_t) vec_slqi ((vui128_t) q_sig, (128-15));
      // Pre-normalize the significand with the L (implicit) bit.
      q_sig = vec_srqi ((vui128_t) q_sig, 15);
      // Separate the low order significand (even/odd) bit.
      q_low = vec_and ((vui32_t)q_sig, lowmask);
      // And merge with sticky bits.
      q_GRX = vec_or (q_GRX, q_low);
      // Use Add write carry to force carry for rounding.
      q_rnd = vec_addcuq ((vui128_t) q_GRX, (vui128_t) nlmask);
      q_sig = vec_addeuqm (q_sig, q_zero,  q_rnd);
      // Check if rounding generated a carry (C-bit) and adjust
      if (vec_all_eq ((vui32_t) q_sig, q_carry))
        {
	  q_sig = vec_srqi ((vui128_t) q_sig, 1);
          q_exp = vec_addudm (q_exp, (vui64_t) lowmask);
        }
#endif
      q_exp = vec_swapd (q_exp);
      result = vec_xsiexpqp (q_sig, q_exp);
    }
#else
  result = int128[0];
#endif
  return result;
}

__binary128
test_convert_uqqpn_V0 (vui128_t int128)
{
  __binary128 result;
#if defined (_ARCH_PWR10)  && (__GNUC__ >= 10)
  __asm__(
      "xscvuqqp %0,%1"
      : "=v" (result)
      : "v" (int128)
      : );
#elif defined (_ARCH_PWR9) && defined (__FLOAT128__) && (__GNUC__ > 7)
  vui64_t int64 = (vui64_t) int128;
  __binary128 hi64, lo64;
  __binary128 two64 = 0x1.0p64;
  hi64 = int64[VEC_DW_H];
  lo64 = int64[VEC_DW_L];
  result = (hi64 * two64) + lo64;
#elif  defined (_ARCH_PWR8)
  vui64_t q_exp;
  vui128_t q_sig;
  const vui128_t q_zero = (vui128_t) { 0 };
  const vui32_t q_carry = vec_mask128_f128Cbit ();
  const vui32_t lowmask = (vui32_t) vec_splat_u128 ( 1 );
  const vui32_t nlmask = vec_mask128_f128mag ();

//  int64[VEC_DW_L] = 0UL; // clear the right most element to zero.
  q_sig = int128;
  // Quick test for 0UL as this case requires a special exponent.
  if (vec_cmpuq_all_eq (q_sig, q_zero))
    {
      result = vec_xfer_vui128t_2_bin128 (q_zero);
    }
  else
    { // We need to produce a normal QP, so we treat the integer like a
      // denormal, then normalize it.
      // Start with the quad exponent bias + 127 then subtract the count of
      // leading '0's. The 128-bit sig can have 0-127 leading '0's.
      vui64_t q_expm = (vui64_t) CONST_VINT64_DW(0, (0x3fff + 127));
      vui64_t i64_clz = (vui64_t) vec_clzq (q_sig);
      vui32_t q_GRX, q_low;
      vui128_t q_rnd;
      q_sig = vec_slq (q_sig, (vui128_t) i64_clz);
      q_exp = vec_subudm (q_expm, i64_clz);
      // This is the part that might require rounding.
      // For "round to Nearest, ties to even".
      // We need to separate the Significand (including the L-bit)
      // from the guard, round, and sticky (GRX) bits
      // Left justify the GRX bits
      q_GRX = (vui32_t) vec_slqi ((vui128_t) q_sig, (128-15));
      // Pre-normalize the significand with the L (implicit) bit.
      q_sig = vec_srqi ((vui128_t) q_sig, 15);
      // Separate the low order significand (even/odd) bit.
      q_low = vec_and ((vui32_t)q_sig, lowmask);
      // And merge with sticky bits.
      q_GRX = vec_or (q_GRX, q_low);
      // Use Add write carry to force carry for rounding.
      q_rnd = vec_addcuq ((vui128_t) q_GRX, (vui128_t) nlmask);
      q_sig = vec_addeuqm (q_sig, q_zero,  q_rnd);
      // Check if rounding generated a carry (C-bit) and adjust
      if (vec_all_eq ((vui32_t) q_sig, q_carry))
        {
	  q_sig = vec_srqi ((vui128_t) q_sig, 1);
          q_exp = vec_addudm (q_exp, (vui64_t) lowmask);
        }
      q_exp = vec_swapd (q_exp);
      result = vec_xsiexpqp (q_sig, q_exp);
    }
#else
  result = int128[0];
#endif
  return result;
}
#endif

#ifdef PVECLIB_OLDTESTVERSIONS
__binary128
test_convert_uqqpz (vui128_t int128)
{
  __binary128 result;
#if defined (_ARCH_PWR10)  && (__GNUC__ >= 10)
  __asm__(
      "xscvuqqp %0,%1"
      : "=v" (result)
      : "v" (int128)
      : );
#elif defined (_ARCH_PWR9) && defined (__FLOAT128__) && (__GNUC__ > 7)
  vui64_t int64 = (vui64_t) int128;
  __binary128 hi64, lo64;
  __binary128 two64 = 0x1.0p64;
  hi64 = int64[VEC_DW_H];
  lo64 = int64[VEC_DW_L];
  result = (hi64 * two64) + lo64;
#elif  defined (_ARCH_PWR8)
  vui64_t q_exp;
  vui128_t q_sig;
  const vui128_t q_zero = (vui128_t) { 0 };

//  int64[VEC_DW_L] = 0UL; // clear the right most element to zero.
  q_sig = int128;
  // Quick test for 0UL as this case requires a special exponent.
  if (vec_cmpuq_all_eq (q_sig, q_zero))
    {
      result = vec_xfer_vui128t_2_bin128 (q_zero);
    }
  else
    { // We need to produce a normal QP, so we treat the integer like a
      // denormal, then normalize it.
      // Start with the quad exponent bias + 127 then subtract the count of
      // leading '0's. The 128-bit sig can have 0-127 leading '0's.
      vui64_t q_expm = (vui64_t) CONST_VINT64_DW(0, (0x3fff + 127));
      vui64_t i64_clz = (vui64_t) vec_clzq (q_sig);
      q_sig = vec_slq (q_sig, (vui128_t) i64_clz);
      q_exp = vec_subudm (q_expm, i64_clz);
      // This is the part that might require rounding.
      // The simplest is we "round toward zero" (truncate).
      q_sig = vec_srqi ( q_sig, 15 );
      q_exp = vec_swapd (q_exp);
      result = vec_xsiexpqp (q_sig, q_exp);
    }
#else
  result = int128[0];
#endif
  return result;
}

__binary128
test_convert_uqqpo (vui128_t int128)
{
  __binary128 result;
#if defined (_ARCH_PWR10)  && (__GNUC__ >= 10)
  __asm__(
      "xscvuqqp %0,%1"
      : "=v" (result)
      : "v" (int128)
      : );
#elif defined (_ARCH_PWR9) && defined (__FLOAT128__) && (__GNUC__ > 7)
  vui64_t int64 = (vui64_t) int128;
  __binary128 hi64, lo64;
  __binary128 two64 = 0x1.0p64;
  hi64 = int64[VEC_DW_H];
  lo64 = int64[VEC_DW_L];
  result = (hi64 * two64) + lo64;
#elif  defined (_ARCH_PWR8)
  vui64_t q_exp;
  vui128_t q_sig;
  const vui128_t q_zero = (vui128_t) { 0 };
  const vui32_t oddmask = CONST_VINT128_W(0, 0, 0, 0x7fff);

//  int64[VEC_DW_L] = 0UL; // clear the right most element to zero.
  q_sig = int128;
  // Quick test for 0UL as this case requires a special exponent.
  if (vec_cmpuq_all_eq (q_sig, q_zero))
    {
      result = vec_xfer_vui128t_2_bin128 (q_zero);
    }
  else
    { // We need to produce a normal QP, so we treat the integer like a
      // denormal, then normalize it.
      // Start with the quad exponent bias + 127 then subtract the count of
      // leading '0's. The 128-bit sig can have 0-127 leading '0's.
      vui64_t q_expm = (vui64_t) CONST_VINT64_DW(0, (0x3fff + 127));
      vui64_t i64_clz = (vui64_t) vec_clzq (q_sig);
      vui32_t q_odd;

      q_sig = vec_slq (q_sig, (vui128_t) i64_clz);
      q_exp = vec_subudm (q_expm, i64_clz);
      // This is the part that might require rounding.
      // Here we "round to odd". So need to detect if any
      // GRX bits (lower 15-bits) are not zero.
      // And then add with the oddmask will set the odd-bit position.
      q_odd = vec_and ((vui32_t) q_sig, oddmask);
      q_odd = vec_add (q_odd, oddmask);
      // Then or the odd-bit into the low order bit of the significand.
      q_sig = (vui128_t) vec_or  ((vui32_t) q_sig, q_odd);
      // Shift the significand into the correct position for xsiexpqp
      q_sig = vec_srqi ( q_sig, 15 );
      q_exp = vec_swapd (q_exp);
      result = vec_xsiexpqp (q_sig, q_exp);
    }
#else
  result = int128[0];
#endif
  return result;
}

__binary128
__test_convert_udqp (vui64_t int64)
{
  __binary128 result;
#if defined (_ARCH_PWR9) && defined (__FLOAT128__) && (__GNUC__ > 9)
  result = int64[VEC_DW_H];
#elif  defined (_ARCH_PWR8)
  vui64_t d_sig, q_exp;
  vui128_t q_sig;
  const vui64_t d_zero = (vui64_t) CONST_VINT64_DW( 0, 0 );

  int64[VEC_DW_L] = 0UL; // clear the right most element to zero.int64
  // Quick test for 0UL as this case requires a special exponent.
  if (vec_cmpud_all_eq (int64, d_zero))
    {
      result = vec_xfer_vui64t_2_bin128 (d_zero);
    }
  else
    { // We need to produce a normal QP, so we treat the integer like a
      // denormal, then normalize it.
      // Start with the quad exponent bias + 63 then subtract the count of
      // leading '0's. The 64-bit sig can have 0-63 leading '0's.
      vui64_t q_expm = (vui64_t) CONST_VINT64_DW((0x3fff + 63), 0 );
      vui64_t i64_clz = vec_clzd (int64);
      d_sig = vec_vsld (int64, i64_clz);
      q_exp = vec_subudm (q_expm, i64_clz);
      q_sig = vec_srqi ((vui128_t) d_sig, 15);
      result = vec_xsiexpqp (q_sig, q_exp);
    }
  // Insert exponent into significand to complete conversion to QP
  // result = vec_xsiexpqp (q_sig, q_exp);
#else
  result = int64[VEC_DW_H];
#endif
  return result;
}

__binary128
__test_convert_sdqp (vi64_t int64)
{
  __binary128 result;
#if defined (_ARCH_PWR9) && defined (__FLOAT128__) && (__GNUC__ > 9)
  result = int64[VEC_DW_H];
#elif  defined (_ARCH_PWR8)
  vui64_t d_sig, q_exp, d_sign, d_inv;
  vui128_t q_sig;
  vui32_t q_sign;
  const vui64_t d_zero = (vui64_t) CONST_VINT64_DW( 0, 0 );
  const vui32_t signmask = vec_mask128_f128sign ();

  int64[VEC_DW_L] = 0UL; // clear the right most element to zero.

  if (vec_cmpud_all_eq ((vui64_t) int64, d_zero))
    {
      result = vec_xfer_vui64t_2_bin128 (d_zero);
    }
  else
    {
      q_sign = vec_and ((vui32_t) int64, signmask);
      d_inv  = vec_subudm (d_zero, (vui64_t)int64);
      d_sign = (vui64_t) vec_cmpequd ((vui64_t) q_sign, (vui64_t) signmask);
      d_sig = (vui64_t) vec_sel ((vui32_t) int64, (vui32_t) d_inv, (vui32_t) d_sign);
      // We need to produce a normal QP, so we treat the integer as
      // denormal, Then normalize it.
      // Start with the quad exponent bias + 63 then subtract the count of
      // leading '0's. The 64-bit sig will have at 0-63 leading '0's
      vui64_t q_expm = (vui64_t) CONST_VINT64_DW((0x3fff + 63), 0 );
      vui64_t i64_clz = vec_clzd (d_sig);
      d_sig = vec_vsld (d_sig, i64_clz);
      q_exp = vec_subudm (q_expm, i64_clz);
      q_sig = vec_srqi ((vui128_t) d_sig, 15);
      // Copy Sign-bit to QP significand before insert.
      q_sig = (vui128_t) vec_or ((vui32_t) q_sig, q_sign);
      // Insert exponent into significand to complete conversion to QP
      result = vec_xsiexpqp (q_sig, q_exp);
    }
#else
  result = int64[VEC_DW_H];
#endif
  return result;
}

vui128_t
__test_convert_qpuqz (__binary128 f128)
{
  vui128_t result;
#if defined (_ARCH_PWR9) && defined (__FLOAT128__) && (__GNUC__ > 9)
  // GCC runtime does not convert directly from F128 to vector _int128
  // So convert to __int128 then xfer via union.
  __VEC_U_128 xxx;
  xxx.ui128 = f128;
  result = xxx.vx1;
#elif  defined (_ARCH_PWR8)
  vui64_t q_exp, q_delta;
  vui128_t q_sig;
  vui32_t q_sign;
  const vui128_t q_zero = { 0 };
  const vui128_t q_ones = (vui128_t) vec_splat_s32 (-1);
  const vui64_t exp_low = (vui64_t) CONST_VINT64_DW( (0x3fff), 0 );
  const vui64_t exp_high = (vui64_t) CONST_VINT64_DW( (0x3fff+128), 0 );
  const vui32_t signmask = CONST_VINT128_W(0x80000000, 0, 0, 0);
  const vui64_t q_naninf = (vui64_t) CONST_VINT64_DW( 0x7fff, 0 );

  result = q_zero;
  q_exp = vec_xsxexpqp (f128);
  q_sig = vec_xsxsigqp (f128);
  q_sign = vec_and_bin128_2_vui32t (f128, signmask);
  if (__builtin_expect (!vec_cmpuq_all_eq ((vui128_t) q_exp, (vui128_t) q_naninf), 1))
    {
      if (vec_cmpuq_all_ge ((vui128_t) q_exp, (vui128_t) exp_low))
	{ // Greater than or equal to 1.0
	  if (vec_cmpuq_all_lt ((vui128_t) q_exp, (vui128_t) exp_high))
	    { // Less than 2**128-1
#if 1
	      const vui64_t exp_127 = (vui64_t) CONST_VINT64_DW( (0x3fff+127), 0 );
	      q_sig = vec_slqi ((vui128_t) q_sig, 15);
	      q_delta = vec_subudm (exp_127, q_exp);
	      q_delta = vec_swapd (q_delta);
	      q_sig = vec_srq (q_sig, (vui128_t) q_delta);
#else
	      const vui64_t exp_112 = (vui64_t) CONST_VINT64_DW( (0x3fff+112), 0 );
	      if (vec_cmpuq_all_lt ((vui128_t) q_exp, (vui128_t) exp_112))
		{ // Less than 2**112
		  q_delta = vec_subudm (exp_112, q_exp);
		  q_sig = vec_srq (q_sig, (vui128_t) q_delta);
		}
	      else
		{ // greater then 2**112
		  q_delta = vec_subudm (q_exp, exp_112);
		  q_sig = vec_slq (q_sig, (vui128_t) q_delta);
		}
#endif
	      result = q_sig;
	    }
	  else
	    { // set result to 2**128-1
	      result = (vui128_t) q_ones;
	    }
	}
      else
	{ // less than 1.0
	  result = (vui128_t) q_zero;
	}
    }
  else
    { // isinf or isnan.
      vb128_t is_inf, is_pos;
      is_inf = vec_cmpequq (q_sig, q_zero);
      is_pos = vec_cmpneuq ((vui128_t) q_sign, (vui128_t) signmask);
      result = (vui128_t) vec_and ((vui32_t) is_inf, (vui32_t) is_pos);
      //  else NaN or -Infinity returns zero
    }

#else
  // GCC runtime does not convert directly from F128 to vector _int128
  // So convert to __int128 then xfer via union.
  __VEC_U_128 xxx;
  xxx.ui128 = f128;
  result = xxx.vx1;
#endif
  return result;
}
#endif

#ifdef PVECLIB_OLDTESTVERSIONS
vf64_t
test_convert_qpdpo_v2 (__binary128 f128)
{
  vf64_t result;
#if defined (_ARCH_PWR9) && (__GNUC__ > 7)
#if defined (__FLOAT128__) && (__GNUC__ > 9)
  // GCC runtime does not convert/round directly from __float128 to
  // vector double. So convert scalar double then copy to vector double.
  result = (vf64_t) { 0.0, 0.0 };
  result [VEC_DW_H] = __builtin_truncf128_round_to_odd (f128);
#else
  // No extra data moves here.
  __asm__(
      "xscvqpdpo %0,%1"
      : "=v" (result)
      : "v" (f128)
      : );
#endif
#else //  defined (_ARCH_PWR8)
  vui64_t d_exp, d_sig, x_exp;
  vui64_t q_exp;
  vui128_t q_sig;
  vui32_t q_sign;
  const vui128_t q_zero = { 0 };
  const vui128_t q_ones = (vui128_t) vec_splat_s32 (-1);
//  const vui64_t exp_low = (vui64_t) CONST_VINT64_DW( (0x3fff), 0 );
  const vui64_t qpdp_delta = (vui64_t) CONST_VINT64_DW( (0x3fff - 0x3ff), 0 );
  const vui64_t exp_tiny = (vui64_t) CONST_VINT64_DW( (0x3fff - 1022), (0x3fff - 1022) );
  const vui64_t exp_high = (vui64_t) CONST_VINT64_DW( (0x3fff + 1023), (0x3fff + 1023));
  const vui32_t signmask = vec_mask128_f128sign ();
  const vui64_t q_naninf = vec_mask64_f128exp ();
  const vui64_t d_naninf = (vui64_t) CONST_VINT64_DW( 0x7ff, 0 );

  q_exp = vec_xsxexpqp (f128);
  x_exp = vec_splatd (q_exp, VEC_DW_H);
  q_sig = vec_xsxsigqp (f128);
  q_sign = vec_and_bin128_2_vui32t (f128, signmask);
  if (__builtin_expect (!vec_cmpud_all_eq (x_exp, q_naninf), 1))
    {
      if (vec_cmpud_all_ge (x_exp, exp_tiny))
	{ // Greater than or equal to 2**-1022
	  if (vec_cmpud_all_le (x_exp, exp_high))
	    { // Less than or equal to 2**+1023
	      vui64_t d_X;
	      // Convert the significand to double with left shift 4
	      q_sig = vec_slqi ((vui128_t) q_sig, 4);
	      // The GRX round bits are now in bits 64-127 (DW element 1)
	      // For round-to-odd just test for any GRX bits nonzero
	      d_X = (vui64_t) vec_cmpgtud ((vui64_t) q_sig, (vui64_t) q_zero);
#if 1
	      d_X = vec_mrgald (q_zero, (vui128_t) d_X);
	      d_X = (vui64_t) vec_slqi ((vui128_t) d_X, 1);
#else
	      d_X = vec_mrgald ((vui128_t) d_X, q_zero);
	      d_X = vec_srdi (d_X, 63);
#endif
	      d_sig = (vui64_t) vec_or ((vui32_t) q_sig, (vui32_t) d_X);
	      d_exp = vec_subudm (q_exp, qpdp_delta);
	    }
	  else
	    { // To high so return infinity OR double max???
	      d_sig = (vui64_t) CONST_VINT64_DW (0x001fffffffffffff, 0);
	      d_exp = (vui64_t) CONST_VINT64_DW (0x7fe, 0);
	    }
	}
      else
	{ // tiny
	  vui64_t d_X;
	  vui64_t q_delta;
	  const vui64_t exp_tinyr = (vui64_t)
	      CONST_VINT64_DW( (0x3fff-(1022+53)), (0x3fff-(1022+53)));
	  q_delta = vec_subudm (exp_tiny, x_exp);
	  // Set double exp to denormal
	  d_exp = (vui64_t) q_zero;
	  if (vec_cmpud_all_gt (x_exp, exp_tinyr))
	    {
	      // Convert the significand to double with left shift 4
	      // The GRX round bits are now in bits 64-127 (DW element 1)
	      q_sig = vec_slqi ((vui128_t) q_sig, 4);
	      d_sig = (vui64_t) vec_srq (q_sig, (vui128_t) q_delta);
	      // For round-to-odd just test for any nonzero GRX bits.
	      d_X = (vui64_t) vec_cmpgtud ((vui64_t) d_sig, (vui64_t) q_zero);
#if 1
	      // Generate a low order 0b1 in DW[0]
	      d_X = vec_mrgald (q_zero, (vui128_t) d_X);
	      d_X = (vui64_t) vec_slqi ((vui128_t) d_X, 1);
#else
	      d_X = vec_mrgald ((vui128_t) d_X, q_zero);
	      d_X = vec_srdi (d_X, 63);
#endif
	      d_sig = (vui64_t) vec_or ((vui32_t) d_sig, (vui32_t) d_X);
	    }
	  else
	    { // tinyr
	      // For round-to-odd just test for any nonzero GRX bits.
	      d_X = (vui64_t) vec_addcuq (q_sig, q_ones);
	      d_sig = (vui64_t) vec_swapd (d_X);
	    }
	}
    }
  else
    { // isinf or isnan.
      const vui64_t q_quiet   = CONST_VINT64_DW(0x0000800000000000, 0);
#if 1
      vb128_t is_inf;
      vui128_t x_sig;
      is_inf = vec_cmpequq ((vui128_t) q_sig, (vui128_t) q_zero);
      x_sig = (vui128_t) vec_or ((vui32_t) q_sig, (vui32_t) q_quiet);
      q_sig = (vui128_t) vec_sel ((vui32_t)x_sig, (vui32_t)q_sig, (vui32_t)is_inf);
#else
      if (vec_cmpuq_all_ne ((vui128_t) q_sig, (vui128_t) q_zero))
	{
	  q_sig = (vui128_t) vec_or ((vui32_t) q_sig, (vui32_t) q_quiet);
	}
#endif
      d_sig = (vui64_t)vec_slqi (q_sig, 4);
      d_exp = d_naninf;
    }
 // q_exp = vec_swapd (q_exp);
  d_sig [VEC_DW_L] = 0UL;
  d_sig = (vui64_t) vec_or ((vui32_t) d_sig, q_sign);
  result = vec_xviexpdp (d_sig, d_exp);
#endif
  return result;
}

vf64_t
__test_convert_qpdpo (__binary128 f128)
{
  vf64_t result;
#if defined (_ARCH_PWR9) && (__GNUC__ > 7)
#if defined (__FLOAT128__) && (__GNUC__ > 9)
  // GCC runtime does not convert/round directly from __float128 to
  // vector double. So convert scalar double then copy to vector double.
  result = (vf64_t) { 0.0, 0.0 };
  result [VEC_DW_H] = __builtin_truncf128_round_to_odd (f128);
#else
  // No extra data moves here.
  __asm__(
      "xscvqpdpo %0,%1"
      : "=v" (result)
      : "v" (f128)
      : );
#endif
#else //  defined (_ARCH_PWR8)
  vui64_t d_exp, d_sig;
  vui64_t q_exp, q_delta;
  vui128_t q_sig;
  vui32_t q_sign;
  const vui128_t q_zero = { 0 };
  const vui128_t q_ones = (vui128_t) vec_splat_s32 (-1);
//  const vui64_t exp_low = (vui64_t) CONST_VINT64_DW( (0x3fff), 0 );
  const vui64_t exp_delta = (vui64_t) CONST_VINT64_DW( (0x3fff - 0x3ff), 0 );
  const vui64_t exp_tiny = (vui64_t) CONST_VINT64_DW( (0x3fff - 1022), 0 );
  const vui64_t exp_high = (vui64_t) CONST_VINT64_DW( (0x3fff + 1023), 0 );
  const vui32_t signmask = CONST_VINT128_W(0x80000000, 0, 0, 0);
  const vui64_t q_naninf = (vui64_t) CONST_VINT64_DW( 0x7fff, 0 );
  const vui64_t d_naninf = (vui64_t) CONST_VINT64_DW( 0x7ff, 0 );

  q_exp = vec_xsxexpqp (f128);
  q_sig = vec_xsxsigqp (f128);
  q_sign = vec_and_bin128_2_vui32t (f128, signmask);
  if (__builtin_expect (!vec_cmpuq_all_eq ((vui128_t) q_exp, (vui128_t) q_naninf), 1))
    {
      if (vec_cmpuq_all_ge ((vui128_t) q_exp, (vui128_t) exp_tiny))
	{ // Greater than or equal to 2**-1022
	  if (vec_cmpuq_all_le ((vui128_t) q_exp, (vui128_t) exp_high))
	    { // Less than or equal to 2**+1023
	      vui64_t d_X;
	      // Convert the significand to double with left shift 4
	      q_sig = vec_slqi ((vui128_t) q_sig, 4);
	      // The GRX round bits are now in bits 64-127 (DW element 1)
	      // For round-to-odd just test for any GRX bits nonzero
	      d_X = (vui64_t) vec_cmpgtud ((vui64_t) q_sig, (vui64_t) q_zero);
	      d_X = vec_mrgald ((vui128_t) d_X, q_zero);
	      d_X = vec_srdi (d_X, 63);
	      d_sig = (vui64_t) vec_or ((vui32_t) q_sig, (vui32_t) d_X);
	      d_exp = vec_subudm (q_exp, exp_delta);
	    }
	  else
	    { // To high so return infinity OR double max???
	      d_sig = (vui64_t) CONST_VINT64_DW (0x001fffffffffffff, 0);
	      d_exp = (vui64_t) CONST_VINT64_DW (0x7fe, 0);
	    }
	}
      else
	{ // tiny
	  vui64_t d_X;
	  const vui64_t exp_tinyr = (vui64_t)
	      CONST_VINT64_DW( (0x3fff-(1022+53) ), 0);
	  q_delta = vec_subudm (exp_tiny, q_exp);
	  q_delta = vec_swapd (q_delta);
	  if (vec_cmpuq_all_gt ((vui128_t) q_exp, (vui128_t) exp_tinyr))
	    {
	      // Convert the significand to double with left shift 4
	      // The GRX round bits are now in bits 64-127 (DW element 1)
	      q_sig = vec_slqi ((vui128_t) q_sig, 4);
	      d_sig = (vui64_t) vec_srq (q_sig, (vui128_t) q_delta);
	      d_X = (vui64_t) vec_cmpgtud ((vui64_t) d_sig, (vui64_t) q_zero);
	      d_X = vec_mrgald ((vui128_t) d_X, q_zero);
	      d_X = vec_srdi (d_X, 63);
	      d_sig = (vui64_t) vec_or ((vui32_t) d_sig, (vui32_t) d_X);
	      d_exp = (vui64_t) { 0, 0 };
	    }
	  else
	    { // tinyr
	      d_X = (vui64_t) vec_addcuq (q_sig, q_ones);
	      d_sig = (vui64_t) vec_swapd (d_X);
	      d_exp = (vui64_t) q_zero;
	    }
	}
    }
  else
    { // isinf or isnan.
      if (vec_cmpuq_all_ne ((vui128_t) q_sig, (vui128_t) q_zero))
	{
	  const vui64_t q_quiet   = CONST_VINT64_DW(0x0000800000000000, 0);
	  q_sig = (vui128_t) vec_or ((vui32_t) q_sig, (vui32_t) q_quiet);
	}
      d_sig = (vui64_t)vec_slqi (q_sig, 4);
      d_exp = d_naninf;
    }
 // q_exp = vec_swapd (q_exp);
  d_sig [VEC_DW_L] = 0UL;
  d_sig = (vui64_t) vec_or ((vui32_t) d_sig, q_sign);
  result = vec_xviexpdp (d_sig, d_exp);
#endif
  return result;
}

__binary128
test_convert_dpqp_v3 (vf64_t f64)
{
  __binary128 result;
#if defined (_ARCH_PWR9) && defined (__FLOAT128__) && (__GNUC__ > 9)
  result = f64[VEC_DW_H];
#elif  defined (_ARCH_PWR8)
  vui64_t d_exp, d_sig, q_exp;
  vui128_t q_sig;
  vui32_t q_sign;
  const vui64_t exp_delta = (vui64_t) CONST_VINT64_DW( (0x3fff - 0x3ff), 0 );
  const vui64_t d_naninf = (vui64_t) CONST_VINT64_DW( 0x7ff, 0 );
  const vui64_t d_denorm = (vui64_t) CONST_VINT64_DW( 0, 0 );
  const vui32_t signmask = CONST_VINT128_W(0x80000000, 0, 0, 0);

  f64[VEC_DW_L] = 0.0; // clear the right most element to zero.
  // Extract the exponent, significand, and sign bit.
  d_exp = vec_xvxexpdp (f64);
  d_sig = vec_xvxsigdp (f64);
  q_sign = vec_and ((vui32_t) f64, signmask);
  // The extract sig operation has already tested for finite/subnormal.
  // So avoid testing isfinite/issubnormal again by simply testing
  // the extracted exponent.
  if (__builtin_expect (!vec_cmpud_all_eq (d_exp, d_naninf), 1))
    {
      if (__builtin_expect (!vec_cmpud_all_eq (d_exp, d_denorm), 1))
	{
	  q_sig = vec_srqi ((vui128_t) d_sig, 4);
	  q_exp = vec_addudm (d_exp, exp_delta);
	}
      else
	{ // We can simplify iszero by comparing the sig to 0
	  if (vec_cmpud_all_eq (d_sig, d_denorm))
	    {
	      q_sig = (vui128_t) d_sig;
	      q_exp = (vui64_t) d_exp;
	    }
	  else
	    { // Must be subnormal but we need to produce a normal QP
	      // Need to adjust the quad exponent by the f64 denormal exponent
	      // (-1023) and that the f64 sig will have at least 12 leading '0's
	      vui64_t q_denorm = (vui64_t) CONST_VINT64_DW( (0x3fff - (1023 -12)), 0 );
	      vui64_t f64_clz;
	      //d_sig = vec_sldi (d_sig, 12);
	      f64_clz = vec_clzd (d_sig);
	      d_sig = vec_vsld (d_sig, f64_clz);
	      q_exp = vec_subudm (q_denorm, f64_clz);
	      q_sig = vec_srqi ((vui128_t) d_sig, 15);
	    }
	}
    }
  else
    { // isinf or isnan.
      q_sig = vec_srqi ((vui128_t) d_sig, 4);
      q_exp = (vui64_t) CONST_VINT64_DW(0x7fff, 0);
    }

  // Copy Sign-bit to QP significand before insert.
  q_sig = (vui128_t) vec_or ((vui32_t) q_sig, q_sign);
  // Insert exponent into significand to complete conversion to QP
  result = vec_xsiexpqp (q_sig, q_exp);
#else
  result = f64[VEC_DW_H];
#endif
  return result;
}

__binary128
test_convert_dpqp_v2 (vf64_t f64)
{
  __binary128 result;
#if defined (_ARCH_PWR9) && defined (__FLOAT128__) && (__GNUC__ > 9)
  result = f64[VEC_DW_H];
#elif  defined (_ARCH_PWR8)
  f64[VEC_DW_L] = 0.0;
  vui64_t d_exp, d_sig, q_exp;
  vui128_t q_sig;
  const vui64_t exp_delta = (vui64_t) CONST_VINT64_DW( (0x3fff - 0x3ff), 0 );
  const vui32_t signmask = CONST_VINT128_W(0x80000000, 0, 0, 0);

  d_exp = vec_xvxexpdp (f64);
  d_sig = vec_xvxsigdp (f64);
  if (__builtin_expect (vec_all_isfinitef64 (f64), 1))
    {
      if (__builtin_expect (vec_all_isnormalf64 (vec_splat (f64, VEC_DW_H)), 1))
	{
	  q_sig = vec_srqi ((vui128_t) d_sig, 4);
	  q_exp = vec_addudm (d_exp, exp_delta);
	}
      else
	{
	  if (vec_all_iszerof64 (f64))
	    {
	      q_sig = (vui128_t) d_sig;
	      q_exp = (vui64_t) d_exp;
	    }
	  else
	    { // Must be subnormal
	      vui64_t q_denorm = (vui64_t) CONST_VINT64_DW( (0x3fff - 1023), 0 );
	      vui64_t f64_clz;
	      d_sig = vec_sldi (d_sig, 12);
	      f64_clz = vec_clzd (d_sig);
	      d_sig = vec_sl (d_sig, f64_clz);
	      q_exp = vec_subudm (q_denorm, f64_clz);
	      q_sig = vec_srqi ((vui128_t) d_sig, 15);
	    }
	}
    }
  else
    { // isinf or isnan.
      q_sig = vec_srqi ((vui128_t) d_sig, 4);
      q_exp = (vui64_t) CONST_VINT64_DW(0x7fff, 0);
    }

  q_sig = (vui128_t) vec_sel ((vui32_t) q_sig, (vui32_t) f64, signmask);
  result = vec_xsiexpqp (q_sig, q_exp);
#else
  result = f64[VEC_DW_H];
#endif
  return result;
}

__binary128
__test_convert_dpqp (vf64_t f64)
{
  __binary128 result;
#if defined (_ARCH_PWR9) && defined (__FLOAT128__) && (__GNUC__ > 9)
  result = f64[VEC_DW_H];
#elif  defined (_ARCH_PWR8)
  f64[VEC_DW_L] = 0.0;
  vui64_t d_exp, d_sig, q_exp;
  vui128_t q_sig;
  const vui64_t exp_delta = {(0x3fff - 0x3ff), (0x3fff - 0x3ff)};

  d_exp = vec_xvxexpdp (f64);
  d_sig = vec_xvxsigdp (f64);
  if (vec_any_isnormalf64 (vec_splat (f64, VEC_DW_H)))
    {
      q_sig = vec_srqi ((vui128_t) d_sig, 4);
      q_exp = vec_addudm (d_exp, exp_delta);
    } else {
	if (vec_all_iszerof64 (vec_splat (f64, VEC_DW_H)))
	  {
	    q_sig = (vui128_t) d_sig;
	    q_exp = (vui64_t) d_exp;
	  }
	else
	  {
	    if (vec_all_issubnormalf64 (vec_splat (f64, VEC_DW_H)))
	      {
		vui64_t q_denorm = { (0x3fff - 1023), 0 };
		vui64_t f64_clz;
		d_sig = vec_sldi ( d_sig, 12);
		f64_clz = vec_clzd (d_sig);
		d_sig = vec_sl (d_sig, f64_clz);
		q_exp = vec_subudm (q_denorm, f64_clz);
		q_sig = vec_srqi ((vui128_t) d_sig, 15);
	      } else {
		q_sig = vec_srqi ((vui128_t) d_sig, 4);
		q_exp = (vui64_t) CONST_VINT64_DW(0x7fff, 0);
	      }
	  }
    }
  const vui32_t signmask = CONST_VINT128_W(0x80000000, 0, 0, 0);

  q_sig = (vui128_t) vec_sel ((vui32_t) q_sig, (vui32_t) f64, signmask);
  result = vec_xsiexpqp (q_sig, q_exp);
#else
  result = f64[VEC_DW_H];
#endif
  return result;
}
#endif

int
test_scalar_cmpto_exp_gt (__binary128 vfa, __binary128 vfb)
{
#if defined (_ARCH_PWR9) && defined (scalar_cmp_exp_gt) && defined (__FLOAT128__) && (__GNUC__ > 9)
  return scalar_cmp_exp_gt (vfa, vfb);
#else
  vui32_t vra, vrb;
  const vui32_t expmask = CONST_VINT128_W(0x7fff0000, 0, 0, 0);

  vra = vec_and_bin128_2_vui32t (vfa, expmask);
  vrb = vec_and_bin128_2_vui32t (vfb, expmask);
  return vec_any_gt (vra, vrb);
#endif
}

int
test_scalar_cmp_exp_gt (__binary128 vfa, __binary128 vfb)
{
#if defined (_ARCH_PWR9) && defined (scalar_cmp_exp_gt) && defined (__FLOAT128__) && (__GNUC__ > 9)
  return scalar_cmp_exp_gt (vfa, vfb);
#else
  vui32_t vra, vrb;
  const vui32_t expmask = CONST_VINT128_W(0x7fff0000, 0, 0, 0);

  if (__builtin_expect ((vec_all_isnanf128 (vfa) || vec_all_isnanf128 (vfb)), 0))
    return 0;

  vra = vec_and_bin128_2_vui32t (vfa, expmask);
  vrb = vec_and_bin128_2_vui32t (vfb, expmask);
  return vec_any_gt (vra, vrb);
#endif
}

int
test_scalar_cmp_exp_unordered (__binary128 vfa, __binary128 vfb)
{
#if defined (_ARCH_PWR9) && defined (scalar_cmp_exp_gt) && defined (__FLOAT128__) && (__GNUC__ > 9)
  return scalar_cmp_exp_unordered (vfa, vfb);
#else
  return (vec_all_isnanf128 (vfa) || vec_all_isnanf128 (vfb));
#endif
}

vb128_t
test_bool_cmp_exp_unordered (__binary128 vfa, __binary128 vfb)
{
  return (vb128_t) vec_or ((vui32_t) vec_isnanf128 (vfa),
			   (vui32_t) vec_isnanf128 (vfb));
}

vb128_t
test_bool_cmp_exp_unordered_v2 (__binary128 vfa, __binary128 vfb)
{
  vb128_t result;

  result = (vb128_t) vec_splat_u32 (0);
  if (vec_all_isnanf128 (vfa) || vec_all_isnanf128 (vfb))
    result = (vb128_t) vec_splat_s32 (-1);

  return result;
}

__binary128
test_sel_bin128_2_bin128 (__binary128 vfa, __binary128 vfb, vb128_t mask)
{
  return vec_sel_bin128_2_bin128 (vfa, vfb, mask);
}

__binary128
test_sel_bin128_2_bin128_V0 (__binary128 vfa, __binary128 vfb, vb128_t mask)
{
  __VF_128 ua, ub;
  vui32_t result;

  ua.vf1 = vfa;
  ub.vf1 = vfb;

  result = vec_sel (ua.vx4, ub.vx4, (vb32_t) mask);
  return vec_xfer_vui32t_2_bin128 (result);
}

vui32_t
test_and_bin128_2_vui32t (__binary128 f128, vui32_t mask)
{
  return vec_and_bin128_2_vui32t (f128, mask);
}

vui32_t
test_and_bin128_2_vui32t_V0 (__binary128 f128, vui32_t mask)
{
  __VF_128 vunion;

  vunion.vf1 = f128;

  return (vec_and (vunion.vx4, mask));
}

vui32_t
test_andc_bin128_2_vui32t (__binary128 f128, vui32_t mask)
{
  return vec_andc_bin128_2_vui32t (f128, mask);
}

vui32_t
test_andc_bin128_2_vui32t_V0 (__binary128 f128, vui32_t mask)
{
  __VF_128 vunion;

  vunion.vf1 = f128;

  return (vec_andc (vunion.vx4, mask));
}

vui32_t
test_or_bin128_2_vui32t (__binary128 f128, vui32_t mask)
{
  return vec_or_bin128_2_vui32t (f128, mask);
}

vui32_t
test_or_bin128_2_vui32t_V0 (__binary128 f128, vui32_t mask)
{
  __VF_128 vunion;

  vunion.vf1 = f128;

  return (vec_or (vunion.vx4, mask));
}

vui32_t
test_xor_bin128_2_vui32t (__binary128 f128, vui32_t mask)
{
  return vec_xor_bin128_2_vui32t (f128, mask);
}

vui32_t
test_xor_bin128_2_vui32t_V0 (__binary128 f128, vui32_t mask)
{
  __VF_128 vunion;

  vunion.vf1 = f128;

  return (vec_xor (vunion.vx4, mask));
}

vui32_t
test_xfer_bin128_2_vui32t (__binary128 f128)
{
  return vec_xfer_bin128_2_vui32t (f128);
}

vui32_t
test_xfer_bin128_2_vui32t_V0 (__binary128 f128)
{
  __VF_128 vunion;

  vunion.vf1 = f128;
  return vunion.vx4;
}

vui64_t
test_xfer_bin128_2_vui64t (__binary128 f128)
{
  return vec_xfer_bin128_2_vui64t (f128);
}

vui64_t
test_xfer_bin128_2_vui64t_V0 (__binary128 f128)
{
  __VF_128 vunion;

  vunion.vf1 = f128;
  return vunion.vx2;
}

vui64_t
test_mrgh_bin128_2_vui64t (__binary128 vfa, __binary128 vfb)
{
  return vec_mrgh_bin128_2_vui64t (vfa, vfb) ;
}

vui64_t
test_mrgl_bin128_2_vui64t (__binary128 vfa, __binary128 vfb)
{
  return vec_mrgl_bin128_2_vui64t (vfa, vfb) ;
}

vui64_t
test_mrgl_bin128_2_vui64t_V0 (__binary128 vfa, __binary128 vfb)
{
  __VF_128 vunion_a, vunion_b;

  vunion_a.vf1 = vfa;
  vunion_b.vf1 = vfb;

  return vec_mrgald (vunion_a.vx1, vunion_b.vx1);
}

vui64_t
test_mrgh_bin128_2_vui64t_V1 (__binary128 vfa, __binary128 vfb)
{
  vui64_t result;
#if (__BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__) \
    && ((__GNUC__ > 7) && (__GNUC__ < 12)) \
    && !defined (_ARCH_PWR9) && defined (__VSX__)
  // Work around for GCC PR 100085
  __asm__(
      "xxmrghd %x0,%x1,%x2;"
      "ori 2,2,0"
      : "=wa" (result)
      : "wa" (vfa), "wa" (vfb)
      : );
#else
  __VF_128 vunion_a, vunion_b;

  vunion_a.vf1 = vfa;
  vunion_b.vf1 = vfb;

  result = vec_mrgahd (vunion_a.vx1, vunion_b.vx1);
#endif
  return result;
}

vui64_t
test_mrgh_bin128_2_vui64t_V0 (__binary128 vfa, __binary128 vfb)
{
  vui64_t result;
#if (__BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__) && (__GNUC__ > 7) \
    && !defined (_ARCH_PWR9) && defined (__VSX__)
  // Work around for GCC PR 100085
  __asm__(
      "xxmrghd %x0,%x1,%x2"
      : "=wa" (result)
      : "wa" (vfa), "wa" (vfb)
      : );
#else
  __VF_128 vunion_a, vunion_b;

  vunion_a.vf1 = vfa;
  vunion_b.vf1 = vfb;

  result = vec_mrgahd (vunion_a.vx1, vunion_b.vx1);
#endif
  return result;
}

vui128_t
test_xfer_bin128_2_vui128t (__binary128 f128)
{
  return vec_xfer_bin128_2_vui128t (f128);
}

vui128_t
test_xfer_bin128_2_vui128t_V0 (__binary128 f128)
{
  __VF_128 vunion;

  vunion.vf1 = f128;

  return (vunion.vx1);
}

__binary128
test_xfer_vui32t_2_bin128 (vui32_t f128)
{
  return vec_xfer_vui32t_2_bin128 (f128);
}

__binary128
test_xfer_vui32t_2_bin128_V0 (vui32_t f128)
{
  __VF_128 vunion;

  vunion.vx4 = f128;

  return (vunion.vf1);
}

__binary128
test_xfer_vui8t_2_bin128 (vui8_t f128)
{
  return vec_xfer_vui8t_2_bin128 (f128);
}

__binary128
test_xfer_vui16t_2_bin128 (vui16_t f128)
{
  return vec_xfer_vui16t_2_bin128 (f128);
}

__binary128
test_xfer_vui64t_2_bin128 (vui64_t f128)
{
  return vec_xfer_vui64t_2_bin128 (f128);
}

unsigned __int128
test_xfer_bin128_2_ui128t_V0 (__binary128 f128)
{
  __VF_128 vunion;

  vunion.vf1 = f128;
  return vunion.ix1;
}

vb128_t
test_cmpltf128_v1 (vi128_t vfa128, vi128_t vfb128)
{
  const vui32_t zero = CONST_VINT128_W(0, 0, 0, 0);
  vb128_t age0, altb, alt0, agtb;
  vui32_t andp, andn;
  vb128_t result;
  age0 = vec_cmpgesq (vfa128, (vi128_t) zero);
  altb = vec_cmpltsq (vfa128, vfb128);
  andp = vec_and ((vui32_t) age0, (vui32_t) altb);
  alt0 = vec_cmpltsq (vfa128, (vi128_t) zero);
  agtb = vec_cmpgeuq ((vui128_t) vfa128, (vui128_t) vfb128);
  andn = vec_and ((vui32_t) alt0, (vui32_t) agtb);
  result = (vb128_t) vec_or (andp, andn);
  return result;
}

vb128_t
test_cmpltf128_v1b (vi128_t vfa128, vi128_t vfb128)
{
  const vui32_t zero = CONST_VINT128_W(0, 0, 0, 0);
  vb128_t age0, altb, agtb;
  vui32_t andp, andn;
  vb128_t result;
  age0 = vec_cmpgesq (vfa128, (vi128_t) zero);
  altb = vec_cmpltsq (vfa128, vfb128);
  andp = vec_and ((vui32_t) altb, (vui32_t) age0);
  agtb = vec_cmpgeuq ((vui128_t) vfa128, (vui128_t) vfb128);
  andn = vec_andc ((vui32_t) agtb, (vui32_t) age0);
  result = (vb128_t) vec_or (andp, andn);
  return result;
}

vb128_t
test_cmpltf128_v1c (vi128_t vfa128, vi128_t vfb128)
{
  vb128_t altb, agtb;
  vb128_t signbool;

  // a >= 0
  // signbool = vec_setb_qp;
  const vui8_t shift = vec_splat_u8 (7);
  vui8_t splat = vec_splat ((vui8_t) vfa128, VEC_BYTE_H);
  signbool = (vb128_t) vec_sra (splat, shift);

  altb = vec_cmpltsq (vfa128, vfb128);
  agtb = vec_cmpgtuq ((vui128_t) vfa128, (vui128_t) vfb128);
  return (vb128_t) vec_sel ((vui32_t)agtb, (vui32_t)altb, (vui32_t)signbool);
}

vb128_t
test_cmpltf128_v2 (vi128_t vfa128, vi128_t vfb128)
{
  const vui32_t zero = CONST_VINT128_W(0, 0, 0, 0);
  const vui32_t signmask = CONST_VINT128_W(0x80000000, 0, 0, 0);
  vb128_t age0, altb, alt0, agtb, nesm;
  vui32_t andp, andn, or_ab;
  vb128_t result;
  age0 = vec_cmpgesq (vfa128, (vi128_t) zero);
  altb = vec_cmpltsq (vfa128, vfb128);
  andp = vec_and ((vui32_t) age0, (vui32_t) altb);
  alt0 = vec_cmpltsq (vfa128, (vi128_t) zero);
  agtb = vec_cmpgeuq ((vui128_t) vfa128, (vui128_t) vfb128);
  andn = vec_and ((vui32_t) alt0, (vui32_t) agtb);

  or_ab = vec_or ((vui32_t) vfa128, (vui32_t) vfb128 );
  nesm = vec_cmpneuq ((vui128_t) or_ab, (vui128_t) signmask);
  andn = vec_and ((vui32_t) andn, (vui32_t) nesm);

  result = (vb128_t) vec_or (andp, andn);
  return result;
}

vb128_t
test_cmpltf128_v2b (vi128_t vfa128, vi128_t vfb128)
{
  const vui32_t zero = CONST_VINT128_W(0, 0, 0, 0);
  const vui32_t signmask = CONST_VINT128_W(0x80000000, 0, 0, 0);
  vb128_t age0, altb, alt0, agtb, nesm;
  vui32_t andp, andn, or_ab;
  vb128_t result;
  age0 = vec_cmpgesq (vfa128, (vi128_t) zero);
  altb = vec_cmpltsq (vfa128, vfb128);
  andp = vec_and ((vui32_t) age0, (vui32_t) altb);
  alt0 = vec_cmpltsq (vfa128, (vi128_t) zero);
  agtb = vec_cmpgeuq ((vui128_t) vfa128, (vui128_t) vfb128);
  andn = vec_and ((vui32_t) alt0, (vui32_t) agtb);

  or_ab = vec_or ((vui32_t) vfa128, (vui32_t) vfb128 );
  nesm = vec_cmpequq ((vui128_t) or_ab, (vui128_t) signmask);
  andn = vec_andc ((vui32_t) andn, (vui32_t) nesm);

  result = (vb128_t) vec_or (andp, andn);
  return result;
}

vb128_t
test_cmpltf128_v2c (vi128_t vfa128, vi128_t vfb128)
{
  const vui32_t signmask = CONST_VINT128_W(0x80000000, 0, 0, 0);
  vb128_t altb, agtb, nesm;
  vui32_t or_ab;
  vb128_t signbool;

  // a >= 0
  // signbool = vec_setb_qp;
  const vui8_t shift = vec_splat_u8 (7);
  vui8_t splat = vec_splat ((vui8_t) vfa128, VEC_BYTE_H);
  signbool = (vb128_t) vec_sra (splat, shift);

  altb = vec_cmpltsq (vfa128, vfb128);
  agtb = vec_cmpgtuq ((vui128_t) vfa128, (vui128_t) vfb128);

  or_ab = vec_or ((vui32_t) vfa128, (vui32_t) vfb128 );
  // For ne compare eq then and compliment
  nesm = vec_cmpequq ((vui128_t) or_ab, (vui128_t) signmask);
  agtb = (vb128_t) vec_andc ((vui32_t) agtb, (vui32_t) nesm);

  return (vb128_t) vec_sel ((vui32_t)agtb, (vui32_t)altb, (vui32_t)signbool);
}

vb128_t
test_cmpltf128_v3 (vi128_t vfa128, vi128_t vfb128)
{
  const vui32_t zero = CONST_VINT128_W(0, 0, 0, 0);
  const vui32_t signmask = CONST_VINT128_W(0x80000000, 0, 0, 0);

  vb128_t result;
  vb128_t age0, bge0;
  vi128_t vra, vrap, vran;
  vi128_t vrb, vrbp, vrbn;

  age0 = vec_cmpltsq (vfa128, (vi128_t) zero);
  vrap = (vi128_t) vec_adduqm ((vui128_t) vfa128, (vui128_t) signmask);
  vran = (vi128_t) vec_subuqm ((vui128_t) zero, (vui128_t) vfa128);
  vra  = (vi128_t) vec_sel ((vui32_t)vrap, (vui32_t)vran, (vui32_t)age0);

  bge0 = vec_cmpltsq (vfb128, (vi128_t) zero);
  vrbp = (vi128_t) vec_adduqm ((vui128_t) vfb128, (vui128_t) signmask);
  vrbn = (vi128_t) vec_subuqm ((vui128_t) zero, (vui128_t) vfb128);
  vrb  = (vi128_t) vec_sel ((vui32_t)vrbp, (vui32_t)vrbn, (vui32_t)bge0);

  result = vec_cmpltuq ((vui128_t) vra, (vui128_t) vrb);

  return result;
}

vb128_t
test_cmpltf128_v3b (vui128_t vfa128, vui128_t vfb128)
{
  const vui32_t zero = CONST_VINT128_W(0, 0, 0, 0);
  const vui32_t signmask = CONST_VINT128_W(0x80000000, 0, 0, 0);

  vb128_t result;
  vb128_t age0, bge0;
  vui128_t vra, vrap, vran;
  vui128_t vrb, vrbp, vrbn;

  age0 = vec_cmpltuq (vfa128, (vui128_t) signmask);
  vrap = (vui128_t) vec_adduqm ((vui128_t) vfa128, (vui128_t) signmask);
  vran = (vui128_t) vec_subuqm ((vui128_t) zero, (vui128_t) vfa128);
  vra  = (vui128_t) vec_sel ((vui32_t)vran, (vui32_t)vrap, (vui32_t)age0);

  bge0 = vec_cmpltuq (vfb128, (vui128_t) signmask);
  vrbp = (vui128_t) vec_adduqm ((vui128_t) vfb128, (vui128_t) signmask);
  vrbn = (vui128_t) vec_subuqm ((vui128_t) zero, (vui128_t) vfb128);
  vrb  = (vui128_t) vec_sel ((vui32_t)vrbn, (vui32_t)vrbp, (vui32_t)bge0);

  result = vec_cmpltuq ((vui128_t) vra, (vui128_t) vrb);

  return result;
}

vb128_t
test_cmpltf128_v3c (vui128_t vfa128, vui128_t vfb128)
{
  const vui32_t zero = CONST_VINT128_W(0, 0, 0, 0);
  const vui32_t signmask = CONST_VINT128_W(0x80000000, 0, 0, 0);

  vb128_t result;
  vb128_t age0, bge0;
  vui128_t vra, vrap, vran;
  vui128_t vrb, vrbp, vrbn;

  age0 = vec_cmpleuq (vfa128, (vui128_t) signmask);
  vrap = (vui128_t) vec_adduqm ((vui128_t) vfa128, (vui128_t) signmask);
  vran = (vui128_t) vec_subuqm ((vui128_t) zero, (vui128_t) vfa128);
  vra  = (vui128_t) vec_sel ((vui32_t)vran, (vui32_t)vrap, (vui32_t)age0);

  bge0 = vec_cmpleuq (vfb128, (vui128_t) signmask);
  vrbp = (vui128_t) vec_adduqm ((vui128_t) vfb128, (vui128_t) signmask);
  vrbn = (vui128_t) vec_subuqm ((vui128_t) zero, (vui128_t) vfb128);
  vrb  = (vui128_t) vec_sel ((vui32_t)vrbn, (vui32_t)vrbp, (vui32_t)bge0);

  result = vec_cmpltuq ((vui128_t) vra, (vui128_t) vrb);

  return result;
}

vb128_t
test_cmpltf128_v3d (vui128_t vfa128, vui128_t vfb128)
{
  const vui32_t zero = CONST_VINT128_W(0, 0, 0, 0);
  const vui32_t signmask = CONST_VINT128_W(0x80000000, 0, 0, 0);
  const vui8_t shift = vec_splat_u8 (7);

  vb128_t result;
  vb128_t age0, bge0;
  vui128_t vra, vrap, vran;
  vui128_t vrb, vrbp, vrbn;
  vui8_t splta, spltb;

  // signbool = vec_setb_qp;
  splta = vec_splat ((vui8_t) vfa128, VEC_BYTE_H);
  age0 = (vb128_t) vec_sra (splta, shift);

  vrap = (vui128_t) vec_xor ((vui32_t) vfa128, signmask);
  vran = (vui128_t) vec_subuqm ((vui128_t) zero, (vui128_t) vfa128);
  vra  = (vui128_t) vec_sel ((vui32_t)vrap, (vui32_t)vran, (vui32_t)age0);

  spltb = vec_splat ((vui8_t) vfb128, VEC_BYTE_H);
  bge0 = (vb128_t) vec_sra (spltb, shift);

  vrbp = (vui128_t) vec_xor ((vui32_t) vfb128, signmask);
  vrbn = (vui128_t) vec_subuqm ((vui128_t) zero, (vui128_t) vfb128);
  vrb  = (vui128_t) vec_sel ((vui32_t)vrbp, (vui32_t)vrbn, (vui32_t)bge0);

  result = vec_cmpltuq (vra, vrb);

  return result;
}

vb128_t
test_cmpeqf128_v1 (vui128_t vfa128, vui128_t vfb128)
{
  return vec_cmpequq (vfa128, vfb128);
}

vb128_t
test_cmpeqf128_v2 (vi128_t vfa128, vi128_t vfb128)
{
  const vui32_t zero = CONST_VINT128_W(0, 0, 0, 0);
  vi128_t _a = (vi128_t) vec_subuqm ((vui128_t) zero, (vui128_t) vfa128);
  vi128_t _b = (vi128_t) vec_subuqm ((vui128_t) zero, (vui128_t) vfb128);
  vb128_t eq_a, eq_b, and_eq, cmps;
  vb128_t result;

  eq_a = vec_cmpeqsq (vfa128, _a);
  eq_b = vec_cmpeqsq (vfb128, _b);
  and_eq = (vb128_t) vec_and ((vui32_t) eq_a, (vui32_t) eq_b );
  cmps = vec_cmpeqsq (vfa128, vfb128);
  result = (vb128_t) vec_or ((vui32_t) cmps, (vui32_t) and_eq);
  return result;
}

vb128_t
test_cmpeqf128_v3 (vi128_t vfa128, vi128_t vfb128)
{
  const vui32_t signmask = CONST_VINT128_W(0x80000000, 0, 0, 0);
  vb128_t cmps, or_ab, eq_s;
  vb128_t result;

  or_ab = (vb128_t) vec_or ((vui32_t) vfa128, (vui32_t) vfb128 );
  eq_s = vec_cmpequq ((vui128_t) or_ab, (vui128_t) signmask);
  cmps = vec_cmpeqsq (vfa128, vfb128);
  result = (vb128_t) vec_or ((vui32_t) cmps, (vui32_t) eq_s);
  return result;
}

vb128_t
test_cmpeqf128_v4 (vi128_t vfa128, vi128_t vfb128)
{
  const vui32_t signmask = CONST_VINT128_W(0x80000000, 0, 0, 0);
  const vui32_t zero = CONST_VINT128_W(0, 0, 0, 0);
  vb128_t cmps, or_ab, andc, eq_s;
  vb128_t result;

  or_ab = (vb128_t) vec_or ((vui32_t) vfa128, (vui32_t) vfb128 );
  andc  = (vb128_t) vec_andc ((vui32_t) or_ab, (vui32_t) signmask);
  eq_s = vec_cmpequq ((vui128_t) andc, (vui128_t) zero);
  cmps = vec_cmpeqsq (vfa128, vfb128);
  result = (vb128_t) vec_or ((vui32_t) cmps, (vui32_t) eq_s);
  return result;
}

__binary128
test_vec_max8_f128uz (__binary128 vf1, __binary128 vf2,
		    __binary128 vf3, __binary128 vf4,
		    __binary128 vf5, __binary128 vf6,
		    __binary128 vf7, __binary128 vf8)
{
  __binary128 maxres;
  vb128_t bool;

  bool = vec_cmpgtuzqp (vf2, vf1);
  maxres = vec_self128 (vf1, vf2, bool);
  bool = vec_cmpgtuzqp (vf3, maxres);
  maxres = vec_self128 (vf3, maxres, bool);
  bool = vec_cmpgtuzqp (vf4, maxres);
  maxres = vec_self128 (vf4, maxres, bool);
  bool = vec_cmpgtuzqp (vf5, maxres);
  maxres = vec_self128 (vf5, maxres, bool);
  bool = vec_cmpgtuzqp (vf6, maxres);
  maxres = vec_self128 (vf6, maxres, bool);
  bool = vec_cmpgtuzqp (vf7, maxres);
  maxres = vec_self128 (vf7, maxres, bool);
  bool = vec_cmpgtuzqp (vf8, maxres);
  maxres = vec_self128 (vf8, maxres, bool);

  return maxres;
}

__binary128
test_vec_max8_f128 (__binary128 vf1, __binary128 vf2,
		    __binary128 vf3, __binary128 vf4,
		    __binary128 vf5, __binary128 vf6,
		    __binary128 vf7, __binary128 vf8)
{
  __binary128 maxres;
  vb128_t bool;

  bool = vec_cmpgtuqp (vf2, vf1);
  maxres = vec_self128 (vf1, vf2, bool);
  bool = vec_cmpgtuqp (vf3, maxres);
  maxres = vec_self128 (vf3, maxres, bool);
  bool = vec_cmpgtuqp (vf4, maxres);
  maxres = vec_self128 (vf4, maxres, bool);
  bool = vec_cmpgtuqp (vf5, maxres);
  maxres = vec_self128 (vf5, maxres, bool);
  bool = vec_cmpgtuqp (vf6, maxres);
  maxres = vec_self128 (vf6, maxres, bool);
  bool = vec_cmpgtuqp (vf7, maxres);
  maxres = vec_self128 (vf7, maxres, bool);
  bool = vec_cmpgtuqp (vf8, maxres);
  maxres = vec_self128 (vf8, maxres, bool);

  return maxres;
}

#ifndef PVECLIB_DISABLE_F128ARITH
#ifdef __FLOAT128__

static const __float128 qpfact1 = 1.0Q;

void
test_gcc_addqpn_f128 (__binary128 * vf128,
		    __binary128 vf1, __binary128 vf2,
		    __binary128 vf3, __binary128 vf4,
		    __binary128 vf5, __binary128 vf6,
		    __binary128 vf7, __binary128 vf8)
{
  __binary128 result;

//  result = qpfact1 * vf1;

  result = qpfact1 + vf1;
  result = result + vf2;
  result = result + vf3;
  result = result + vf4;
  result = result + vf5;
  result = result + vf6;
  result = result + vf7;
  result = result + vf8;
  *vf128 = result;
}

void
test_gcc_subqpn_f128 (__binary128 * vf128,
		    __binary128 vf1, __binary128 vf2,
		    __binary128 vf3, __binary128 vf4,
		    __binary128 vf5, __binary128 vf6,
		    __binary128 vf7, __binary128 vf8)
{
  __binary128 result;

//  result = qpfact1 * vf1;

  result = qpfact1 - vf1;
  result = result - vf2;
  result = result - vf3;
  result = result - vf4;
  result = result - vf5;
  result = result - vf6;
  result = result - vf7;
  result = result - vf8;
  *vf128 = result;
}

void
test_gcc_divqpn_f128 (__binary128 * vf128,
		    __binary128 vf1, __binary128 vf2,
		    __binary128 vf3, __binary128 vf4,
		    __binary128 vf5, __binary128 vf6,
		    __binary128 vf7, __binary128 vf8)
{
  __binary128 result;

//  result = qpfact1 * vf1;

  result = qpfact1 / vf1;
  result = result / vf2;
  result = result / vf3;
  result = result / vf4;
  result = result / vf5;
  result = result / vf6;
  result = result / vf7;
  result = result / vf8;
  *vf128 = result;
}

void
test_gcc_mulqpn_f128 (__binary128 * vf128,
		    __binary128 vf1, __binary128 vf2,
		    __binary128 vf3, __binary128 vf4,
		    __binary128 vf5, __binary128 vf6,
		    __binary128 vf7, __binary128 vf8)
{
  __binary128 result;

//  result = qpfact1 * vf1;

  result = qpfact1 * vf1;
  result = result * vf2;
  result = result * vf3;
  result = result * vf4;
  result = result * vf5;
  result = result * vf6;
  result = result * vf7;
  result = result * vf8;
  *vf128 = result;
}

void
test_vec_qpdpo_f128 (vf64_t * vx64,
		    __binary128 vf1, __binary128 vf2,
		    __binary128 vf3, __binary128 vf4,
		    __binary128 vf5, __binary128 vf6,
		    __binary128 vf7, __binary128 vf8)
{
  vf64_t vxf1, vxf2, vxf3, vxf4;

  vxf1 = vec_xscvqpdpo_inline (vf1);
  vxf2 = vec_xscvqpdpo_inline (vf2);
  vxf3 = vec_xscvqpdpo_inline (vf3);
  vxf4 = vec_xscvqpdpo_inline (vf4);

  vxf1[VEC_DW_L] = vxf2[VEC_DW_H];
  vxf3[VEC_DW_L] = vxf4[VEC_DW_H];

  vx64[0] = vxf1;
  vx64[1] = vxf3;

  vxf1 = vec_xscvqpdpo_inline (vf5);
  vxf2 = vec_xscvqpdpo_inline (vf6);
  vxf3 = vec_xscvqpdpo_inline (vf7);
  vxf4 = vec_xscvqpdpo_inline (vf8);

  vxf1[VEC_DW_L] = vxf2[VEC_DW_H];
  vxf3[VEC_DW_L] = vxf4[VEC_DW_H];

  vx64[2] = vxf1;
  vx64[3] = vxf3;
}

void
test_gcc_qpdpo_f128 (vf64_t * vx64,
		    __binary128 vf1, __binary128 vf2,
		    __binary128 vf3, __binary128 vf4,
		    __binary128 vf5, __binary128 vf6,
		    __binary128 vf7, __binary128 vf8)
{
#ifdef _ARCH_PWR8
  vf64_t vxf1, vxf2, vxf3, vxf4;

  vxf1[0] = vf1;
  vxf1[1] = vf2;
  vxf2[0] = vf3;
  vxf2[1] = vf4;
  vxf3[0] = vf5;
  vxf3[1] = vf6;
  vxf4[0] = vf7;
  vxf4[1] = vf8;

  vx64[0] = vxf1;
  vx64[1] = vxf2;
  vx64[2] = vxf3;
  vx64[3] = vxf4;
#endif
}

void
test_vec_qpuq_f128 (vui128_t * vf128,
		    __binary128 vf1, __binary128 vf2,
		    __binary128 vf3, __binary128 vf4,
		    __binary128 vf5, __binary128 vf6,
		    __binary128 vf7, __binary128 vf8)
{
  vf128[0] = vec_xscvqpuqz_inline (vf1);
  vf128[1] = vec_xscvqpuqz_inline (vf2);
  vf128[2] = vec_xscvqpuqz_inline (vf3);
  vf128[3] = vec_xscvqpuqz_inline (vf4);
  vf128[4] = vec_xscvqpuqz_inline (vf5);
  vf128[5] = vec_xscvqpuqz_inline (vf6);
  vf128[6] = vec_xscvqpuqz_inline (vf7);
  vf128[7] = vec_xscvqpuqz_inline (vf8);
}

void
test_gcc_qpuq_f128 (vui128_t * vf128,
		    __binary128 vf1, __binary128 vf2,
		    __binary128 vf3, __binary128 vf4,
		    __binary128 vf5, __binary128 vf6,
		    __binary128 vf7, __binary128 vf8)
{
  unsigned __int128 *vx128 = (unsigned __int128 *)vf128;
  unsigned __int128 vi1, vi2, vi3, vi4, vi5, vi6, vi7, vi8;

  vi1 = vf1;
  vi2 = vf2;
  vi3 = vf3;
  vi4 = vf4;
  vi5 = vf5;
  vi6 = vf6;
  vi7 = vf7;
  vi8 = vf8;

  vx128[0] = vi1;
  vx128[1] = vi2;
  vx128[2] = vi3;
  vx128[3] = vi4;
  vx128[4] = vi5;
  vx128[5] = vi6;
  vx128[6] = vi7;
  vx128[7] = vi8;
}

void
test_vec_uqqp_f128 (__binary128 * vf128,
		    vui128_t vf1, vui128_t vf2,
		    vui128_t vf3, vui128_t vf4,
		    vui128_t vf5, vui128_t vf6,
		    vui128_t vf7, vui128_t vf8)
{
  vf128[0] = vec_xscvuqqp_inline (vf1);
  vf128[1] = vec_xscvuqqp_inline (vf2);
  vf128[2] = vec_xscvuqqp_inline (vf3);
  vf128[3] = vec_xscvuqqp_inline (vf4);
  vf128[4] = vec_xscvuqqp_inline (vf5);
  vf128[5] = vec_xscvuqqp_inline (vf6);
  vf128[6] = vec_xscvuqqp_inline (vf7);
  vf128[7] = vec_xscvuqqp_inline (vf8);
}

void
test_gcc_uqqp_f128 (__binary128 * vf128,
		    vui128_t vf1, vui128_t vf2,
		    vui128_t vf3, vui128_t vf4,
		    vui128_t vf5, vui128_t vf6,
		    vui128_t vf7, vui128_t vf8)
{
  vf128[0] = vf1[0];
  vf128[1] = vf2[0];
  vf128[2] = vf3[0];
  vf128[3] = vf4[0];
  vf128[4] = vf5[0];
  vf128[5] = vf6[0];
  vf128[6] = vf7[0];
  vf128[7] = vf8[0];
}

void
test_vec_dpqp_f128 (__binary128 * vf128,
		    vf64_t vf1, vf64_t vf2,
		    vf64_t vf3, vf64_t vf4,
		    vf64_t vf5)
{
  vf128[0] = vec_xscvdpqp_inline (vf1);
  vf1[VEC_DW_H] = vf1[VEC_DW_L];
  vf128[1] = vec_xscvdpqp_inline (vf1);

  vf128[2] = vec_xscvdpqp_inline (vf2);
  vf2[VEC_DW_H] = vf2[VEC_DW_L];
  vf128[3] = vec_xscvdpqp_inline (vf2);

  vf128[4] = vec_xscvdpqp_inline (vf3);
  vf3[VEC_DW_H] = vf3[VEC_DW_L];
  vf128[5] = vec_xscvdpqp_inline (vf3);

  vf128[6] = vec_xscvdpqp_inline (vf4);
  vf4[VEC_DW_H] = vf4[VEC_DW_L];
  vf128[7] = vec_xscvdpqp_inline (vf4);

  vf128[8] = vec_xscvdpqp_inline (vf5);
  vf5[VEC_DW_H] = vf5[VEC_DW_L];
  vf128[8] = vec_xscvdpqp_inline (vf5);
}

void
test_gcc_dpqp_f128 (__binary128 * vf128,
		    vf64_t vf1, vf64_t vf2,
		    vf64_t vf3, vf64_t vf4,
		    vf64_t vf5)
{
  vf128[0] = vf1[VEC_DW_H];
  vf128[1] = vf1[VEC_DW_L];
  vf128[2] = vf2[VEC_DW_H];
  vf128[3] = vf2[VEC_DW_L];
  vf128[4] = vf3[VEC_DW_H];
  vf128[5] = vf3[VEC_DW_L];
  vf128[6] = vf4[VEC_DW_H];
  vf128[7] = vf4[VEC_DW_L];
  vf128[8] = vf5[VEC_DW_H];
  vf128[9] = vf5[VEC_DW_L];
}

__binary128
test_gcc_max8_f128 (__binary128 vf1, __binary128 vf2,
		    __binary128 vf3, __binary128 vf4,
		    __binary128 vf5, __binary128 vf6,
		    __binary128 vf7, __binary128 vf8)
{
  __binary128 maxres = vf1;

#ifndef __clang__
  if (vf2 > vf1)
    maxres = vf2;
  if (vf3 > maxres)
    maxres = vf3;
  if (vf4 > maxres)
    maxres = vf4;
  if (vf5 > maxres)
    maxres = vf5;
  if (vf6 > maxres)
    maxres = vf6;
  if (vf7 > maxres)
    maxres = vf7;
  if (vf8 > maxres)
    maxres = vf8;
#endif

  return maxres;
}

vb128_t
test_vec_cmpequqp (__binary128 vfa, __binary128 vfb)
{
  return vec_cmpequqp (vfa, vfb);
}

vb128_t
test_vec_cmpequzqp (__binary128 vfa, __binary128 vfb)
{
  return vec_cmpequzqp (vfa, vfb);
}

vb128_t
test_vec_cmpeqtoqp (__binary128 vfa, __binary128 vfb)
{
  return vec_cmpeqtoqp (vfa, vfb);
}

vb128_t
test_vec_cmpneuqp (__binary128 vfa, __binary128 vfb)
{
  return vec_cmpneuqp (vfa, vfb);
}

vb128_t
test_vec_cmpneuzqp (__binary128 vfa, __binary128 vfb)
{
  return vec_cmpneuzqp (vfa, vfb);
}

vb128_t
test_vec_cmpnetoqp (__binary128 vfa, __binary128 vfb)
{
  return vec_cmpnetoqp (vfa, vfb);
}

vb128_t
test_vec_cmpleuzqp (__binary128 vfa, __binary128 vfb)
{
  return vec_cmpleuzqp (vfa, vfb);
}

vb128_t
test_vec_cmpleuqp (__binary128 vfa, __binary128 vfb)
{
  return vec_cmpleuqp (vfa, vfb);
}

vb128_t
test_vec_cmpletoqp (__binary128 vfa, __binary128 vfb)
{
  return vec_cmpletoqp (vfa, vfb);
}

vb128_t
test_vec_cmpgeuzqp (__binary128 vfa, __binary128 vfb)
{
  return vec_cmpgeuzqp (vfa, vfb);
}

vb128_t
test_vec_cmpgeuqp (__binary128 vfa, __binary128 vfb)
{
  return vec_cmpgeuqp (vfa, vfb);
}

vb128_t
test_vec_cmpgetoqp (__binary128 vfa, __binary128 vfb)
{
  return vec_cmpgetoqp (vfa, vfb);
}

vb128_t
test_vec_cmpltuzqp (__binary128 vfa, __binary128 vfb)
{
  return vec_cmpltuzqp (vfa, vfb);
}

vb128_t
test_vec_cmpltuqp (__binary128 vfa, __binary128 vfb)
{
  return vec_cmpltuqp (vfa, vfb);
}

vb128_t
test_vec_cmplttoqp (__binary128 vfa, __binary128 vfb)
{
  return vec_cmplttoqp (vfa, vfb);
}

vb128_t
test_vec_cmpgtuzqp (__binary128 vfa, __binary128 vfb)
{
  return vec_cmpgtuzqp (vfa, vfb);
}

vb128_t
test_vec_cmpgtuqp (__binary128 vfa, __binary128 vfb)
{
  return vec_cmpgtuqp (vfa, vfb);
}

vb128_t
test_vec_cmpgttoqp (__binary128 vfa, __binary128 vfb)
{
  return vec_cmpgttoqp (vfa, vfb);
}

__float128
test_absdiff (__float128 vra, __float128 vrb)
{
#ifndef __clang__
  __float128 result;
  if (vra > vrb)
    result = vra - vrb;
  else
    result = vrb - vra;
  return result;
#else
  return vra;
#endif
}

// TBD will sub-in pveclib softfloat for P8 when available

#if 0 // turn off until Round-to-odd implementation is ready
__float128
test_scalar_add128 (__float128 vra, __float128 vrb)
{
#ifndef __clang__
    // Generate call to __addkf3
    return (vra + vrb);
#else
  return vra;
#endif
}

__float128
test_scalar_div128 (__float128 vra, __float128 vrb)
{
#ifndef __clang__
    // Generate call to __divkf3
    return (vra / vrb);
#else
  return vra;
#endif
}

__float128
test_scalar_mul128 (__float128 vra, __float128 vrb)
{
#ifndef __clang__
    // Generate call to __mulkf3
    return (vra * vrb);
#else
  return vra;
#endif
}

__float128
test_scalar_sub128 (__float128 vra, __float128 vrb)
{
#ifndef __clang__
    // Generate call to __subkf3
    return (vra - vrb);
#else
  return vra;
#endif
}
#endif

__float128
test_scalarCC_expxsuba_128 (__float128 x, __float128 a, __float128 expa)
{
#ifndef __clang__
  const __float128 f128_one = 1.0Q;
  const __float128 inv_fact2 = (1.0Q / 2.0Q);
  const __float128 inv_fact3 = (1.0Q / 6.0Q);
  const __float128 inv_fact4 = (1.0Q / 24.0Q);
  const __float128 inv_fact5 = (1.0Q / 120.0Q);
  const __float128 inv_fact6 = (1.0Q / 720.0Q);
  const __float128 inv_fact7 = (1.0Q / 5040.0Q);
  const __float128 inv_fact8 = (1.0Q / 40320.0Q);

  __float128 term, xma, xma2, xmaf2;
  __float128 xma3, xmaf3, xma4, xmaf4, xma5, xmaf5;
  __float128 xma6, xmaf6, xma7, xmaf7, xma8, xmaf8;

  // 1st 8 terms of e**x = e**a * taylor( x-a )
  xma = (x - a);
  term = (f128_one + xma);
  xma2 = (xma * xma);
  xmaf2 = (xma2 * inv_fact2);
  term = (term + xmaf2);
  xma3 = (xma2 * xma);
  xmaf3 = (xma3 * inv_fact3);
  term =  (term + xmaf3);
  xma4 = (xma3 * xma);
  xmaf4 = (xma4 * inv_fact4);
  term = (term + xmaf4);
  xma5 = (xma4 * xma);
  xmaf5 = (xma5 * inv_fact5);
  term = (term + xmaf5);
  xma6 = (xma5 * xma);
  xmaf6 = (xma6 * inv_fact6);
  term = (term + xmaf6);
  xma7 = (xma6 * xma);
  xmaf7 = (xma7 * inv_fact7);
  term = (term + xmaf7);
  xma8 = (xma7 * xma);
  xmaf8 = (xma8 * inv_fact8);
  term = (term + xmaf8);
  return (expa * term);
#else
  return x;
#endif
}
#endif
#endif

#ifdef PVECLIB_OLDTESTVERSIONS
vui64_t
test_vec_xxxsigqpp_V3 (vui128_t a_mag, vui128_t b_mag)
{
  vui128_t a_sig, b_sig;
  { // Extract the significands and insert the Hidden bit
        const vui32_t q_zero = CONST_VINT128_W (0, 0, 0, 0);
	//const vui32_t hidden = vec_mask128_f128Lbit();
	const  vui32_t sigmask = vec_mask128_f128sig();
	vui32_t a_s32, b_s32;
	vui16_t a_e16, b_e16, x_hidden;
	vb16_t a_norm, b_norm;

	x_hidden = vec_splat_u16 (1);
	// Assume that the operands are finite magnitudes
	// Mask off the significands
	a_s32 = vec_and ((vui32_t) a_mag, sigmask);
	b_s32 = vec_and ((vui32_t) b_mag, sigmask);
	// Mask off the exponents in high halfword
	a_e16 = (vui16_t) vec_andc ((vui32_t) a_mag, sigmask);
	b_e16 = (vui16_t) vec_andc ((vui32_t) b_mag, sigmask);
	// Compare exponents for finite i.e. > denomal (q_zero)
	a_norm = vec_cmpgt (a_e16, (vui16_t) q_zero);
	b_norm = vec_cmpgt (b_e16, (vui16_t) q_zero);
	// For Normal QP insert (hidden) L-bit into significand
	a_sig =  (vui128_t) vec_sel ((vui16_t) a_s32, x_hidden, a_norm);
	b_sig =  (vui128_t) vec_sel ((vui16_t) b_s32, x_hidden, b_norm);
  }
  return vec_mrgahd (a_sig, b_sig);
}

vui64_t
test_vec_xxxsigqpp_V2 (vui128_t a_mag, vui128_t b_mag)
{
  vui128_t a_sig, b_sig;
  { // Extract the significands
        const vui32_t q_zero = CONST_VINT128_W (0, 0, 0, 0);
	const vui32_t hidden = vec_mask128_f128Lbit();
	const  vui32_t sigmask = vec_mask128_f128sig();
	vui32_t a_norm, b_norm;
	vui32_t a_s32, b_s32;
	vui32_t a_e32, b_e32;
	// Assume that the operands are finite
	// Mask off the significands
	a_s32 = vec_and ((vui32_t) a_mag, sigmask);
	b_s32 = vec_and ((vui32_t) b_mag, sigmask);
	// Mask off the exponents
	a_e32 = vec_andc ((vui32_t) a_mag, sigmask);
	b_e32 = vec_andc ((vui32_t) b_mag, sigmask);
	// Assume that exponents are already extracted and merged
	// Compare exponents for denormal, assume finite
	a_norm = (vui32_t) vec_cmpgt (a_e32, q_zero);
	b_norm = (vui32_t) vec_cmpgt (b_e32, q_zero);
	// For Normal QP insert (hidden) L-bit into significand
	a_sig =  (vui128_t) vec_sel (a_s32, a_norm, hidden);
	b_sig =  (vui128_t) vec_sel (b_s32, b_norm, hidden);
	// exponents should >= Emin for computation
	//x_exp = vec_selud (x_exp, exp_min, (vb64_t) x_norm);
  }
  return vec_mrgahd (a_sig, b_sig);
}

vui64_t
test_vec_xxxsigqpp_V1 (vui128_t a_mag, vui128_t b_mag, vui64_t x_exp)
{
  vui128_t a_sig, b_sig;
  { // Extract the significands
        const vui32_t q_zero = CONST_VINT128_W (0, 0, 0, 0);
	const vui32_t hidden = vec_mask128_f128Lbit();
	const  vui32_t sigmask = vec_mask128_f128sig();
	vui32_t a_norm, b_norm, x_norm;
	vui32_t a_s32, b_s32;
	// Assume that the operands are finite
	// Mask off the significands
	a_s32 = vec_and ((vui32_t) a_mag, sigmask);
	b_s32 = vec_and ((vui32_t) b_mag, sigmask);
	// Assume that exponents are already extracted and merged
	// Compare exponents for denormal, assume finite
	x_norm = (vui32_t) vec_cmpgt ((vui32_t) x_exp, q_zero);
	a_norm = vec_splat (x_norm, VEC_WE_1);
	b_norm = vec_splat (x_norm, VEC_WE_3);
	// For Normal QP insert (hidden) L-bit into significand
	a_sig =  (vui128_t) vec_sel (a_s32, a_norm, hidden);
	b_sig =  (vui128_t) vec_sel (b_s32, b_norm, hidden);
	// exponents should >= Emin for computation
	//x_exp = vec_selud (x_exp, exp_min, (vb64_t) x_norm);
  }
  return vec_mrgahd (a_sig, b_sig);
}

vui64_t
test_vec_xxxsigqpp_V0 (__binary128 f128a, __binary128 f128b)
{
  vui128_t a_sig, b_sig;
  a_sig = vec_xsxsigqp (f128a);
  b_sig = vec_xsxsigqp (f128b);
  return vec_mrgahd (a_sig, b_sig);
}
#endif

vui64_t
test_vec_xxxexpqpp (__binary128 f128a, __binary128 f128b)
{
  return vec_xxxexpqpp (f128a, f128b);
}

vui64_t
test_vec_xxxexpqpp_V2 (__binary128 vfa, __binary128 vfb)
{
  const vui32_t magmask = vec_mask128_f128mag();
  const vui32_t sigmask = vec_mask128_f128sig();
  vui32_t maga, magb;
  vui32_t expa, expb;
  vui64_t result;
  maga = vec_and_bin128_2_vui32t (vfa, magmask);
  magb = vec_and_bin128_2_vui32t (vfb, magmask);
  expa = vec_andc (maga, sigmask);
  expb = vec_andc (magb, sigmask);

  result = vec_mrgahd ((vui128_t) expa, (vui128_t) expb);
  // GCC 7 -mcpu=power7 does not handle sld  of vector long long
  result = (vui64_t) vec_sld ((vui32_t) result, (vui32_t) result, 10);
  return result;
}

vui64_t
test_vec_xxxexpqpp_V1 (__binary128 vfa, __binary128 vfb)
{
  const vui32_t magmask = vec_mask128_f128mag();
  vui32_t maga, magb;
  vui64_t result;
  maga = vec_and_bin128_2_vui32t (vfa, magmask);
  magb = vec_and_bin128_2_vui32t (vfb, magmask);

  result = vec_mrgahd ((vui128_t) maga, (vui128_t) magb);
  result = vec_srdi (result, 48);
  return result;
}

vui64_t
test_vec_xxxexpqpp_V0 (__binary128 vfa, __binary128 vfb)
{
  vui64_t result;
  vui32_t tmp, rtmp, exp_mask;
  //const vui32_t expmask = CONST_VINT128_W (0, 0x7fff, 0, 0x7fff);
  exp_mask = (vui32_t) vec_mask64_f128exp();
  tmp = (vui32_t) vec_mrgh_bin128_2_vui64t (vfa, vfb);
  rtmp = vec_sld (tmp, tmp, 10);
  result = (vui64_t) vec_and (rtmp, exp_mask);
  return result;
}

__binary128
test_vec_xsiexpqp (vui128_t sig, vui64_t exp)
{
  return vec_xsiexpqp (sig, exp);
}

vui64_t
test_vec_xsxexpqp (__binary128 f128)
{
  return vec_xsxexpqp (f128);
}

vui128_t
test_vec_xsxsigqp (__binary128 f128)
{
  return vec_xsxsigqp (f128);
}

vb128_t
__test_setb_qp (__binary128 f128)
{
  return vec_setb_qp (f128);
}

__binary128
test_vec_absf128 (__binary128 f128)
{
  return vec_absf128 (f128);
}

#ifdef __FLOAT128_HARDWARE__
__binary128
test_vec_absf128_V0 (__binary128 f128)
{
  return __builtin_fabsq (f128);
}
#endif

__binary128
test_vec_nabsf128 (__binary128 f128)
{
  return vec_nabsf128 (f128);
}

#ifdef __FLOAT128_HARDWARE__
__binary128
test_vec_nabsf128_V0 (__binary128 f128)
{
  return -__builtin_fabsq (f128);
}
#endif

__binary128
test_vec_negf128 (__binary128 f128)
{
  return vec_negf128 (f128);
}

__binary128
test_vec_negf128_V1 (__binary128 f128)
{
#ifdef __FLOAT128_HARDWARE__
  return (-f128);
#endif
}

__binary128
test_const_vec_huge_valf128 (void)
{
  return vec_const_huge_valf128 ();
}

__binary128
test_const_vec_nanf128 (void)
{
  return vec_const_nanf128 ();
}

__binary128
test_const_vec_nansf128 (void)
{
  return vec_const_nansf128 ();
}

__binary128
test_const_vec_inff128 (void)
{
  return vec_const_inff128 ();
}

__binary128
test_vec_copysignf128 (__binary128 f128x , __binary128 f128y)
{
  return vec_copysignf128 (f128x , f128y);
}

vb128_t
test_vec_isfinitef128 (__binary128 f128)
{
  return vec_isfinitef128 (f128);
}

vb128_t
test_vec_isinff128 (__binary128 value)
{
  return (vec_isinff128 (value));
}

vb128_t
test_vec_isnanf128 (__binary128 value)
{
  return (vec_isnanf128 (value));
}

vb128_t
test_vec_isnormalf128 (__binary128 f128)
{
  return vec_isnormalf128 (f128);
}

vb128_t
test_vec_issubnormalf128 (__binary128 f128)
{
  return vec_issubnormalf128 (f128);
}

vb128_t
test_vec_iszerof128 (__binary128 f128)
{
  return vec_iszerof128 (f128);
}

int
test_vec_all_finitef128 (__binary128 value)
{
  return (vec_all_isfinitef128 (value));
}

int
test_vec_all_inff128 (__binary128 value)
{
  return (vec_all_isinff128 (value));
}

int
test_vec_all_nanf128 (__binary128 value)
{
  return (vec_all_isnanf128 (value));
}

int
test_vec_all_normalf128 (__binary128 value)
{
  return (vec_all_isnormalf128 (value));
}

int
test_vec_all_subnormalf128 (__binary128 value)
{
  return (vec_all_issubnormalf128 (value));
}

int
test_vec_all_zerof128 (__binary128 value)
{
  return (vec_all_iszerof128 (value));
}

#ifdef __FLOAT128_TYPE__
/* dummy sinf128 example. From Posix:
 * If value is NaN then return a NaN.
 * If value is +-0.0 then return value.
 * If value is subnormal then return value.
 * If value is +-Inf then return a NaN.
 * Otherwise compute and return sin(value).
 */
__binary128
test_sinf128 (__binary128 value)
  {
    __binary128 result;
#ifdef __FLOAT128__
    // requires -mfloat128 to use Q const
    const __binary128 zeroF128 = 0.0Q;
#else
    const __binary128 zeroF128 = (__binary128)CONST_VINT128_W(0, 0, 0, 0);
#endif

    if (vec_all_isnormalf128 (value))
      {
	/* body of vec_sin() computation elided for this example.  */
	result = zeroF128;
      }
    else
      {
	if (vec_all_isinff128 (value))
	  result = vec_const_nanf128 ();
	else
	  result = value;
      }

    return result;
  }

/* dummy cosf128 example. From Posix:
 * If value is NaN then return a NaN.
 * If value is +-0.0 then return 1.0.
 * If value is +-Inf then return a NaN.
 * Otherwise compute and return sin(value).
 */
__binary128
test_cosf128 (__binary128 value)
  {
    __binary128 result;
#ifdef __FLOAT128__
    // requires -mfloat128 to use Q const
    const __binary128 zeroF128 = 0.0Q;
    const __binary128 oneF128 = 1.0Q;
#else
    const __binary128 zeroF128 = (__binary128)CONST_VINT128_W(0, 0, 0, 0);
    const __binary128 oneF128 = (__binary128)CONST_VINT128_W(0x3fff0000, 0, 0, 0);
#endif

    if (vec_all_isfinitef128 (value))
      {
	if (vec_all_iszerof128 (value))
	  result = oneF128;
	else
	  {
	    /* body of vec_cos() computation elided for this example.  */
            result = zeroF128;
	  }
      }
    else
      {
	if (vec_all_isinff128 (value))
	  result = vec_const_nanf128 ();
	else
	  result = value;
      }

    return result;
  }
#endif

vb128_t
_test_f128_isinff128 (__binary128 value)
{
  return (vec_isinff128 (value));
}

int
_test_f128_isinf_sign (__binary128 value)
{
  return (vec_isinf_signf128 (value));
}

vb128_t
_test_f128_isnan (__binary128 value)
{
  return (vec_isnanf128 (value));
}

vb128_t
_test_pred_f128_finite (__binary128 value)
{
  return (vec_isfinitef128 (value));
}

vb128_t
_test_pred_f128_normal (__binary128 value)
{
  return (vec_isnormalf128 (value));
}

vb128_t
_test_pred_f128_subnormal (__binary128 value)
{
  return (vec_issubnormalf128 (value));
}

vui16_t
_test_xfer_bin128_2_vui16t (__binary128 f128)
{
  return vec_xfer_bin128_2_vui16t (f128);
}

#ifdef __FLOAT128__
/* Mostly compiler and library tests follow to see what the various
 * compilers will do. */

#ifndef PVECLIB_DISABLE_F128MATH
// Enable only if math.h supports generic C11 functions for __float128
// __clang__ has a bug whenever -mfloat128 is enabled, maybe clang 10.0.1
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
int
test_gcc_f128_signbit (__binary128 value)
  {
    return (signbit(value));
  }

int
test_gcc_f128_isinf (__binary128 value)
  {
    return (isinf(value));
  }

int
test_gcc_float128_isnan (__binary128 value)
  {
    return (isnan(value));
  }

__binary128
test_gcc_f128_copysign (__binary128 valx, __binary128 valy)
  {
    return (__builtin_copysignf128(valx, valy));
  }

int
test_glibc_f128_classify (__binary128 value)
  {
    if (isfinite(value))
    return 1;

    if (isinf(value))
    return 2;

    if (isnan(value))
    return 3;
    /* finite */
    return 0;
  }
#endif
#endif
#endif

