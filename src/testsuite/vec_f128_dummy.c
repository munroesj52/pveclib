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

__binary128
test_sel_bin128_2_bin128 (__binary128 vfa, __binary128 vfb, vb128_t mask)
{
  return vec_sel_bin128_2_bin128 (vfa, vfb, mask);
}

vui32_t
test_and_bin128_2_vui32t (__binary128 f128, vui32_t mask)
{
  return vec_and_bin128_2_vui32t (f128, mask);
}

vui32_t
test_andc_bin128_2_vui32t (__binary128 f128, vui32_t mask)
{
  return vec_andc_bin128_2_vui32t (f128, mask);
}

vui32_t
test_xfer_bin128_2_vui32t (__binary128 f128)
{
  return vec_xfer_bin128_2_vui32t (f128);
}

vui128_t
test_xfer_bin128_2_vui128t (__binary128 f128)
{
  return vec_xfer_bin128_2_vui128t (f128);
}

__binary128
test_xfer_vui32t_2_bin128 (vui32_t f128)
{
  return vec_xfer_vui32t_2_bin128 (f128);
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
  vb128_t age0, altb, alt0, agtb;
  vui32_t andp, andn;
  vb128_t result;
  age0 = vec_cmpgesq (vfa128, (vi128_t) zero);
  altb = vec_cmpltsq (vfa128, vfb128);
  andp = vec_and ((vui32_t) altb, (vui32_t) age0);
//  alt0 = vec_cmpltsq (vfa128, (vi128_t) zero);
  alt0 = (vb128_t) vec_nor ((vui32_t) age0, (vui32_t) age0);
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
  vb128_t result;
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
  vb128_t result;

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

#ifndef PVECLIB_DISABLE_F128MATH
#ifdef __FLOAT128__

__binary128
test_gcc_max8_f128 (__binary128 vf1, __binary128 vf2,
		    __binary128 vf3, __binary128 vf4,
		    __binary128 vf5, __binary128 vf6,
		    __binary128 vf7, __binary128 vf8)
{
  __binary128 maxres = vf1;

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
  __float128 result;
  if (vra > vrb)
    result = vra - vrb;
  else
    result = vrb - vra;
  return result;
}

// TBD will sub-in pveclib softfloat for P8 when available

__float128
test_scalar_add128 (__float128 vra, __float128 vrb)
{
  if (__builtin_cpu_supports("ieee128"))
    {
      __float128 result;
      __asm__(
	      "xsaddqp %0,%1,%2"
	      : "=v" (result)
	      : "v" (vra), "v" (vrb)
	      : );
      return result;
    }
  else
    // Generate call to __addkf3
    return (vra + vrb);
}

__float128
test_scalar_div128 (__float128 vra, __float128 vrb)
{
  if (__builtin_cpu_supports("ieee128"))
    {
      __float128 result;
      __asm__(
	      "xsdivqp %0,%1,%2"
	      : "=v" (result)
	      : "v" (vra), "v" (vrb)
	      : );
      return result;
    }
  else
    // Generate call to __divkf3
    return (vra / vrb);
}

__float128
test_scalar_mul128 (__float128 vra, __float128 vrb)
{
  if (__builtin_cpu_supports("ieee128"))
    {
      __float128 result;
      __asm__(
	      "xsmulqp %0,%1,%2"
	      : "=v" (result)
	      : "v" (vra), "v" (vrb)
	      : );
      return result;
    }
  else
    // Generate call to __mulkf3
    return (vra * vrb);
}

__float128
test_scalar_sub128 (__float128 vra, __float128 vrb)
{
  if (__builtin_cpu_supports("ieee128"))
    {
      __float128 result;
      __asm__(
	      "xssubqp %0,%1,%2"
	      : "=v" (result)
	      : "v" (vra), "v" (vrb)
	      : );
      return result;
    }
  else
    // Generate call to __subkf3
    return (vra - vrb);
}

__float128
test_scalarCC_expxsuba_128 (__float128 x, __float128 a, __float128 expa)
{
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
}
#endif
#endif

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
_test_f128_isinff128 (__Float128 value)
{
  return (vec_isinff128 (value));
}

int
_test_f128_isinf_sign (__Float128 value)
{
  return (vec_isinf_signf128 (value));
}

vb128_t
_test_f128_isnan (__Float128 value)
{
  return (vec_isnanf128 (value));
}

vb128_t
_test_pred_f128_finite (__Float128 value)
{
  return (vec_isfinitef128 (value));
}

vb128_t
_test_pred_f128_normal (__Float128 value)
{
  return (vec_isnormalf128 (value));
}

vb128_t
_test_pred_f128_subnormal (__Float128 value)
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
test_gcc_f128_signbit (__Float128 value)
  {
    return (signbit(value));
  }

int
test_gcc_f128_isinf (__Float128 value)
  {
    return (isinf(value));
  }

int
test_gcc_float128_isnan (__Float128 value)
  {
    return (isnan(value));
  }

__Float128
test_gcc_f128_copysign (__Float128 valx, __Float128 valy)
  {
    return (__builtin_copysignf128(valx, valy));
  }

int
test_glibc_f128_classify (__Float128 value)
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

