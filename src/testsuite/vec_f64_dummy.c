/*
 Copyright (c) [2017-18] IBM Corporation.

 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.

 vec_int64_dummy.c

 Contributors:
      IBM Corporation, Steven Munroe
      Created on: Jul 31, 2017
 */

#include <stdint.h>
#include <stdio.h>
#include <fenv.h>
#include <float.h>
#ifndef PVECLIB_DISABLE_F128MATH
/* Disable for __clang__ because of bug involving <math.h>/<floatn.h>
   incombination with -mcpu=power9 -mfloat128 */
#include <math.h>
#endif

//#define __DEBUG_PRINT__

#include <pveclib/vec_f64_ppc.h>
#include <testsuite/arith128_test_f64.h>

vf64_t
test_vec_copysignf64 (vf64_t x, vf64_t y)
{
  return vec_copysignf64 (x, y);
}

vui64_t
test_vec_const64_f64bias ()
{
  return vec_const64_f64bias ();
}

vui64_t
test_vec_const64_f64maxe ()
{
  return vec_const64_f64maxe ();
}

vui64_t
test_vec_const64_f64naninf ()
{
  return vec_const64_f64naninf ();
}

vui64_t
test_vec_mask64_f64sign ()
{
  return vec_mask64_f64sign ();
}

vui64_t
test_vec_mask64_f64mag ()
{
  return vec_mask64_f64mag ();
}

vui64_t
test_vec_mask64_f64sig ()
{
  return vec_mask64_f64sig ();
}

vui64_t
test_vec_mask64_f64exp ()
{
  return vec_mask64_f64exp ();
}

vui64_t
test_vec_mask64_f64hidden ()
{
  return vec_mask64_f64hidden ();
}

vui64_t
test_vec_mask64_f64hidden_V2 ()
{
  vui32_t v1 = vec_splat_u32(1);
  return vec_sldi ((vui64_t) v1, 52);
}

vui64_t
test_vec_mask64_f64hidden_V1 ()
{
  const vui64_t signmask = vec_mask64_f64sign ();
  // Min normal exp same as hidden bit.
  return vec_srdi (signmask, 11);
}

vf64_t
test_vec_xviexpdp (vui64_t sig, vui64_t exp)
{
  return vec_xviexpdp (sig, exp);
}

vui64_t
test_vec_xvxexpdp (vf64_t f64)
{
  return vec_xvxexpdp (f64);
}

vui64_t
test_vec_xvxsigdp (vf64_t f64)
{
  return vec_xvxsigdp (f64);
}

vb64_t
test_vec_iszerof64 (vf64_t vf64)
{
  return vec_iszerof64 (vf64);
}

vb64_t
test_vec_issubnormalf64 (vf64_t vf64)
{
  return vec_issubnormalf64 (vf64);
}

vb64_t
test_vec_isnormalf64 (vf64_t vf64)
{
  return vec_isnormalf64 (vf64);
}

vb64_t
test_vec_isnanf64 (vf64_t vf64)
{
  return vec_isnanf64 (vf64);
}

vb64_t
test_vec_isinff64 (vf64_t vf64)
{
  return vec_isinff64 (vf64);
}

vb64_t
test_vec_isfinitef64 (vf64_t vf64)
{
  return vec_isfinitef64 (vf64);
}

vb64_t
__test_setb_dp (vf64_t d)
{
  return vec_setb_dp (d);
}

void
test_stvsfso (vf64_t data, double *array, const long long offset0,
		     const long long offset1)
{
  vec_vsstfdso (data, array, offset0, offset1);
}

void
test_stvsfso_032 (vf64_t data, double *array)
{
  vec_vsstfdso (data, array, 0, 32);
}

void
test_stvsfdo (vf64_t data, double *array, vi64_t vra)
{
  vec_vsstfddo (data, array, vra);
}

void
test_stvsfdx (vf64_t data, double *array, vi64_t vra)
{
  vec_vsstfddx (data, array, vra);
}

void
test_stvsfdsx (vf64_t data, double *array, vi64_t vra)
{
  vec_vsstfddsx (data, array, vra, 4);
}

void
test_vstfdux (vf64_t data, double *array, signed long offset)
{
  vec_vstxsfdx (data, offset, array);
}

void
test_vstfdux_c0 (vf64_t data, double *array)
{
  vec_vstxsfdx (data, 0, array);
}

void
test_vstfdux_c1 (vf64_t data, double *array)
{
  vec_vstxsfdx (data, 8, array);
}

void
test_vstfdux_c2 (vf64_t data, double *array)
{
  vec_vstxsfdx (data, 32760, array);
}

void
test_vstfdux_c3 (vf64_t data, double *array)
{
  vec_vstxsfdx (data, 32768, array);
}

void
test_vstfdux_c5 (vf64_t data, double *array)
{
  vec_vstxsfdx (data, -32768, array);
}

void
test_vstfdux_c4 (vf64_t data, double *array)
{
  vf64_t data1;

  data1 = (vf64_t) vec_xxspltd ((vui64_t) data, 1);
  vec_vstxsfdx (data, 16, array);
  vec_vstxsfdx (data1, 48, array);
}


vf64_t
test_vlfdux (double *array, unsigned long long offset)
{
  return vec_vlxsfdx (offset, array);
}

vf64_t
test_vldfdux_c0 (double *array)
{
  return vec_vlxsfdx (0, array);
}

vf64_t
test_vldfdux_c1 (double *array)
{
  return vec_vlxsfdx (8, array);
}

vf64_t
test_vlfsdux_c2 (double *array)
{
  return vec_vlxsfdx (32768, array);
}

vf64_t
test_vlfsdux_c4 (double *array)
{
  return vec_vlxsfdx (-32768, array);
}

vf64_t
test_vldfdux_c3 (double *array)
{
  vf64_t rese0, rese1;

  rese0 = vec_vlxsfdx (8, array);
  rese1 = vec_vlxsfdx (40, array);
  return (vf64_t) vec_permdi ((vui64_t) rese0, (vui64_t) rese1, 0);
}

vf64_t
test_vec_lvgfso (double *array, const long long offset0,
		     const long long offset1)
{
  return vec_vglfdso (array, offset0, offset1);
}

vf64_t
test_vec_lvgfso_032 (double *array)
{
  return vec_vglfdso (array, 0, 32);
}

vf64_t
test_vec_lvgfdo (double *array, vi64_t vra)
{
  return vec_vglfddo (array, vra);
}

vf64_t
test_vec_lvgfdx (double *array, vi64_t vra)
{
  return vec_vglfddx (array, vra);
}

vf64_t
test_vec_lvgfdsx (double *array, vi64_t vra)
{
  return vec_vglfddsx (array, vra, 4);
}

int
test512_all_f64_nan (vf64_t val0, vf64_t val1, vf64_t val2, vf64_t val3)
{
  const vb64_t alltrue = { -1, -1 };
  vb64_t nan0, nan1, nan2, nan3;

  nan0 = vec_isnanf64 (val0);
  nan1 = vec_isnanf64 (val1);
  nan2 = vec_isnanf64 (val2);
  nan3 = vec_isnanf64 (val3);
/* Only newest compilers support vec_and for vector bool long long.
   So cast to vector bool int then back as a compiler work around.
   Here we just want to see what the various compilers will do.  */
  nan0 = (vb64_t)vec_and ((vb32_t)nan0, (vb32_t)nan1);
  nan2 = (vb64_t)vec_and ((vb32_t)nan2, (vb32_t)nan3);
  nan0 = (vb64_t)vec_and ((vb32_t)nan2, (vb32_t)nan0);

#ifdef _ARCH_PWR8
  return vec_all_eq(nan0, alltrue);
#else
  return vec_cmpud_all_eq((vui64_t)nan0, (vui64_t)alltrue);
#endif
}

int
test_all_f64_finite (vf64_t value)
{
  return (vec_all_isfinitef64 (value));
}

int
test_all_f64_inf (vf64_t value)
{
  return (vec_all_isinff64 (value));
}

int
test_all_f64_nan (vf64_t value)
{
  return (vec_all_isnanf64 (value));
}

int
test_all_f64_norm (vf64_t value)
{
  return (vec_all_isnormalf64 (value));
}

int
test_all_f64_subnorm (vf64_t value)
{
  return (vec_all_issubnormalf64 (value));
}

int
test_all_f64_zero (vf64_t value)
{
  return (vec_all_iszerof64 (value));
}

int
test_any_f64_finite (vf64_t value)
{
  return (vec_any_isfinitef64 (value));
}
int
test_any_f64_inf (vf64_t value)
{
  return (vec_any_isinff64 (value));
}

int
test_any_f64_nan (vf64_t value)
{
  return (vec_any_isnanf64 (value));
}

int
test_any_f64_norm (vf64_t value)
{
  return (vec_any_isnormalf64 (value));
}

int
test_any_f64_subnorm (vf64_t value)
{
  return (vec_any_issubnormalf64 (value));
}

int
test_any_f64_zero (vf64_t value)
{
  return (vec_any_iszerof64 (value));
}

vb64_t
test_pred_f64_finite (vf64_t value)
{
  return (vec_isfinitef64 (value));
}

vb64_t
test_pred_f64_inf (vf64_t value)
{
  return (vec_isinff64 (value));
}

vb64_t
test_pred_f64_nan (vf64_t value)
{
  return (vec_isnanf64 (value));
}

vb64_t
test_pred_f64_normal (vf64_t value)
{
  return (vec_isnormalf64 (value));
}

vb64_t
test_pred_f64_subnormal (vf64_t value)
{
  return (vec_issubnormalf64 (value));
}

vb64_t
test_pred_f64_zero (vf64_t value)
{
  return (vec_iszerof64 (value));
}

#ifndef PVECLIB_DISABLE_F128MATH
vui64_t
test_fpclassify_f64 (vf64_t value)
{
  const vui64_t VFP_NAN =
    { FP_NAN, FP_NAN };
  const vui64_t VFP_INFINITE =
    { FP_INFINITE, FP_INFINITE };
  const vui64_t VFP_ZERO =
    { FP_ZERO, FP_ZERO };
  const vui64_t VFP_SUBNORMAL =
    { FP_SUBNORMAL, FP_SUBNORMAL };
  const vui64_t VFP_NORMAL =
    { FP_NORMAL, FP_NORMAL };
  /* FP_NAN should be 0.  */
  vui64_t result = VFP_NAN;
  vui64_t mask;

  mask = (vui64_t) vec_isinff64 (value);
  result = vec_sel (result, VFP_INFINITE, mask);
  mask = (vui64_t) vec_iszerof64 (value);
  result = vec_sel (result, VFP_ZERO, mask);
  mask = (vui64_t) vec_issubnormalf64 (value);
  result = vec_sel (result, VFP_SUBNORMAL, mask);
  mask = (vui64_t) vec_isnormalf64 (value);
  result = vec_sel (result, VFP_NORMAL, mask);

  return result;
}
#endif

/* dummy sinf64 example. From Posix:
 * If value is NaN then return a NaN.
 * If value is +-0.0 then return value.
 * If value is subnormal then return value.
 * If value is +-Inf then return a NaN.
 * Otherwise compute and return sin(value).
 */
vf64_t
test_vec_sinf64 (vf64_t value)
{
  const vf64_t vec_f0 = { 0.0, 0.0 };
  const vui64_t vec_f64_qnan =
    { 0x7ff8000000000000, 0x7ff8000000000000 };
  vf64_t result;
  vb64_t normmask, infmask;

  normmask = vec_isnormalf64 (value);
  if (vec_any_isnormalf64 (value))
    {
      /* replace non-normal input values with safe values.  */
      vf64_t safeval = vec_sel (vec_f0, value, normmask);
      /* body of vec_sin(safeval) computation elided for this example.  */
      result = vec_mul (safeval, safeval);
    }
  else
    result = value;

  /* merge non-normal input values back into result */
  result = vec_sel (value, result, normmask);
  /* Inf input value elements return quiet-nan.  */
  infmask = vec_isinff64 (value);
  result = vec_sel (result, (vf64_t) vec_f64_qnan, infmask);

  return result;
}

/* dummy cosf64 example. From Posix:
 * If value is NaN then return a NaN.
 * If value is +-0.0 then return 1.0.
 * If value is +-Inf then return a NaN.
 * Otherwise compute and return sin(value).
 */
vf64_t
test_vec_cosf64 (vf64_t value)
{
  vf64_t result;
  const vf64_t vec_f0 = { 0.0, 0.0 };
  const vf64_t vec_f1 = { 1.0, 1.0 };
  const vui64_t vec_f64_qnan =
    { 0x7ff8000000000000, 0x7ff8000000000000 };
  vb64_t finitemask, infmask, zeromask;

  finitemask = vec_isfinitef64 (value);
  if (vec_any_isfinitef64 (value))
    {
      /* replace non-finite input values with safe values.  */
      vf64_t safeval = vec_sel (vec_f0, value, finitemask);
      /* body of vec_sin(safeval) computation elided for this example.  */
      result = vec_mul (safeval, safeval);
    }
  else
    result = value;

  /* merge non-finite input values back into result */
  result = vec_sel (value, result, finitemask);
  /* Set +-0.0 input elements to exactly 1.0 in result.  */
  zeromask = vec_iszerof64 (value);
  result = vec_sel (result, vec_f1, zeromask);
  /* Set Inf input elements to quiet-nan in result.  */
  infmask = vec_isinff64 (value);
  result = vec_sel (result, (vf64_t) vec_f64_qnan, infmask);

  return result;
}

/* compiler scalar inline tests.  */
vf64_t
test_load_vf64 ( vf64_t *val)
{
  return *val;
}

int
test__builtin_f64_isfinite (double value)
{
  return (__builtin_isfinite (value));
}

int
test__builtin_f64_isinf (double value)
{
  return (__builtin_isinf (value));
}

int
test__builtin_f64_isnan (double value)
{
  return (__builtin_isnan (value));
}

int
test__builtin_f64_isnormal (double value)
{
  return (__builtin_isnormal (value));
}

vf64_t
test_ibm128_vf64_vec (long double lval)
{
  return (vec_unpack_longdouble (lval));
}

long double
test_vf64_ibm128_vec (vf64_t lval)
{
  return (vec_pack_longdouble (lval));
}

#ifndef __clang__
/* Disable for clang becuase does not support the asm constraints
   (%L) to access 2nd FPR of FPR pair. */
vf64_t
test_ibm128_vf64_asm (long double lval)
{
#ifdef _ARCH_PWR7
  vf64_t t;
  __asm__(
      "xxmrghd %x0,%x1,%L1;\n"
      : "=wa" (t)
      : "wa" (lval)
      : );
  return (t);
#else
  U_128 t;
  t.ldbl128 = lval;
  return (t.vf2);
#endif
}

long double
test_vf64_ibm128_asm (vf64_t lval)
{
#ifdef _ARCH_PWR7
  long double t;
  __asm__(
      "xxlor %0,%x1,%x1;\n"
      "\txxswapd %L0,%x1;\n"
      : "=wa" (t)
      : "wa" (lval)
      : );
  return (t);
#else
  U_128 t;
  t.vf2 = lval;
  return (t.ldbl128);
#endif
}
#endif

#ifdef _ARCH_PWR8
/* POWER 64-bit (vector double) compiler tests.  */

vb64_t
__test_cmpeqdp (vf64_t a, vf64_t b)
{
  return vec_cmpeq (a, b);
}

vb64_t
__test_cmpgtdp (vf64_t a, vf64_t b)
{
  return vec_cmpgt (a, b);
}

vb64_t
__test_cmpltdp (vf64_t a, vf64_t b)
{
  return vec_cmplt (a, b);
}

vb64_t
__test_cmpgedp (vf64_t a, vf64_t b)
{
  return vec_cmpge (a, b);
}

vb64_t
__test_cmpledp (vf64_t a, vf64_t b)
{
  return vec_cmple (a, b);
}
#endif

void
test_vec_xviexpdp_loop (vf64_t* out, vui64_t* sig, vui64_t* exp, int N)
{
  int i;
  for (i=0; i<N; i++)
    {
      out[i] = vec_xviexpdp (sig[i], exp[i]);
    }
}

void
test_vec_xvxexpdp_loop (vui64_t* out, vf64_t* in, int N)
{
  int i;
  for (i=0; i<N; i++)
    {
      out[i] = vec_xvxexpdp (in[i]);
    }
}

void
test_vec_xvxsigdp_loop (vui64_t* out, vf64_t* in, int N)
{
  int i;
  for (i=0; i<N; i++)
    {
      out[i] = vec_xvxsigdp (in[i]);
    }
}

/*
 * The following are both compile tests for Gather/Scatter operations
 * and performance kernels for performance tests.
 */

double matrix_f64[MN][MN] __attribute__ ((aligned (128)));

void
test_f64_Imatrix_init (double * array)
{
  long i, j, k;
  long rows, columns;

  rows = columns = MN;

#ifdef __DEBUG_PRINT__
  printf ("init_indentity array[%d,%d]\n",
	  rows, columns);
#endif

  for ( i=0; i<rows; i++ )
  {
    for ( j=0; j<columns; j++ )
      {
	k = (i * columns) + j;
	if (i == j)
	  {
	    array [k] = 1.0;
#ifdef __DEBUG_PRINT__
	    printf ("init_indentity array[%d,%d] is %f\n",
			i, j, array [k]);
#endif
	  }
	else
	  {
	    array [k] = 0.0;
	  }
      }
  }
}

void
#if !defined(__clang__)
__attribute__ ((optimize ("unroll-loops")))
#endif
test_f64_matrix_transpose (double * tm, double * m)
{
  long i, j, k, l;
  long rows, columns;

  rows = columns = MN;

  for ( i=0; i<rows; i++ )
  {
    for ( j=0; j<columns; j++ )
      {
	k = (i * columns) + j;
	l = (j * columns) + i;
	tm[l] = m[k];
      }
  }
}

void
test_f64_matrix_gather_transpose (double * tm, double * m)
{
  vi64_t vra = { 0, MN * 8 };
  vui64_t stride = { MN * 8 * 2, MN * 8 * 2 };
  long i, j;
  long rows, columns;

  rows = columns = MN;

  for (i = 0; i < rows; i++)
    {
      double *cadr = &m[i];
      vf64_t *radr = (vf64_t*)&tm[(i * columns)];
      vra = (vi64_t) { 0, MN*8};
      for (j = 0; j < columns/2; j++)
	{
	  radr[j] = vec_vglfddo (cadr, vra);
	  vra = (vi64_t) vec_addudm ((vui64_t) vra, stride);
	}
    }
}

void
test_f64_matrix_gatherx2_transpose (double * tm, double * m)
{
  vi64_t vra =
    { 0, MN * 8 };
  vui64_t stride =
    { MN * 8 * 2, MN * 8 * 2 };
  long i, j;
  long rows, columns;

  rows = columns = MN;

  for (i = 0; i < rows; i+=2)
    {
      double *cadr = &m[i];
      double *cadr1 = &m[i+1];
      vf64_t *radr = (vf64_t*)&tm[(i * columns)];
      vf64_t *radr1 = (vf64_t*)&tm[((i+1) * columns)];

      vra = (vi64_t) { 0, MN*8};
      for (j = 0; j < columns/2; j++)
	{
	  vf64_t vrow0, vrow1;
	  vrow0 = vec_vglfddo (cadr, vra);
	  vrow1 = vec_vglfddo (cadr1, vra);
	  radr[j] = vrow0;
	  radr1[j] = vrow1;
	  vra = (vi64_t) vec_addudm ((vui64_t) vra, stride);
	}
    }
}

void
test_f64_matrix_gatherx4_transpose (double * tm, double * m)
{
  vi64_t vra =
    { 0, MN * 8 };
  vui64_t stride =
    { MN * 8 * 2, MN * 8 * 2 };
  long i, j;
  long rows, columns;

  rows = columns = MN;

  for (i = 0; i < rows; i+=4)
    {
      double *cadr = &m[i];
      double *cadr1 = &m[i+1];
      double *cadr2 = &m[i+2];
      double *cadr3 = &m[i+3];
      vf64_t *radr = (vf64_t*)&tm[(i * columns)];
      vf64_t *radr1 = (vf64_t*)&tm[((i+1) * columns)];
      vf64_t *radr2 = (vf64_t*)&tm[((i+2) * columns)];
      vf64_t *radr3 = (vf64_t*)&tm[((i+3) * columns)];

      vra = (vi64_t) { 0, MN*8};
      for (j = 0; j < columns/2; j++)
	{
	  vf64_t vrow0, vrow1, vrow2, vrow3;
	  vrow0 = vec_vglfddo (cadr, vra);
	  vrow1 = vec_vglfddo (cadr1, vra);
	  vrow2 = vec_vglfddo (cadr2, vra);
	  vrow3 = vec_vglfddo (cadr3, vra);
	  radr[j] = vrow0;
	  radr1[j] = vrow1;
	  radr2[j] = vrow2;
	  radr3[j] = vrow3;
	  vra = (vi64_t) vec_addudm ((vui64_t) vra, stride);
	}
    }
}
