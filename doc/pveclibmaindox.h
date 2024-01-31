/*
 Copyright (c) 2017 IBM Corporation.

 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.

 Contributors:
      IBM Corporation, Steven Munroe
 */

#ifndef __PVECLIB_MAIN_DOX_H
#define __PVECLIB_MAIN_DOX_H

/** \mainpage POWER Vector Library (pveclib)
* \brief A library of useful vector functions for POWER. This library
* fills in the gap between the instructions defined in the POWER
* Instruction Set Architecture (<B>PowerISA</B>) and higher level
* library APIs.  The intent is to improve the productivity of
* application developers who need to optimize their applications or
* dependent libraries for POWER.
*
*  \authors Steven Munroe
*
*  \copyright 2017-2018 IBM Corporation.
*  Licensed under the Apache License, Version 2.0 (the "License");
*  you may not use this file except in compliance with the License.
*  You may obtain a copy of the License at:
*  http://www.apache.org/licenses/LICENSE-2.0 .
*
*  Unless required by applicable law or agreed to in writing, software
*  and documentation
*  distributed under the License is distributed on an "AS IS" BASIS,
*  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*  See the License for the specific language governing permissions and
*  limitations under the License.
*
*  \section mainpage_notices Notices
*
*  IBM, the IBM logo, and ibm.com are trademarks or registered trademarks
*  of International Business Machines Corp., registered in many
*  jurisdictions worldwide. Other product and service names might
*  be trademarks of IBM or other companies. A current list of IBM
*  trademarks is available on the Web at “Copyright and trademark
*  information” at http:www.ibm.com/legal/copytrade.shtml.
*
*  The following terms are trademarks or registered trademarks licensed
*  by Power.org in the United States and/or other countries:
*  Power ISA<SUP>TM</SUP>, Power Architecture<SUP>TM</SUP>.
*  Information on the list of U.S.
*  trademarks licensed by Power.org may be found at
*  http:www.power.org/about/brand-center/.
*
*  The following terms are trademarks or registered trademarks of
*  Freescale Semiconductor in the United States and/or other countries:
*  AltiVec<SUP>TM</SUP>. Information on the list of U.S. trademarks owned by
*  Freescale Semiconductor may be found at
*  http://www.freescale.com/files/abstract/help_page/TERMSOFUSE.html.
*
*  \subsection mainpage_ref_docs Reference Documentation
*
*  - Power Instruction Set Architecture, Versions
*  <a href="https://ibm.ent.box.com/s/jd5w15gz301s5b5dt375mshpq9c3lh4u">
*  2.07B</a>,
*  <a href="https://ibm.ent.box.com/s/1hzcwkwf8rbju5h9iyf44wm94amnlcrv">
*  3.0B</a>, and
*  <a href="https://ibm.ent.box.com/s/hhjfw0x0lrbtyzmiaffnbxh2fuo0fog0">
*  3.1</a>,
*  IBM, 2013-2020. Available from the
*  <a href="https://www-355.ibm.com/systems/power/openpower/">
*  IBM Portal for OpenPOWER</a> under the <B>Public Documents</B> tab.
*   - Publicly available PowerISA docs for older processors are hard
*   to find. But here is a link to
*   <a href="http://citeseerx.ist.psu.edu/viewdoc/download;jsessionid=995FB78240B0A62F1629AB3454C3DFB7?doi=10.1.1.175.7365&rep=rep1&type=pdf">
*   PowerISA-2.06B</a> for POWER7.
*  - <a href="http://www.freescale.com/files/32bit/doc/ref_manual/ALTIVECPIM.pdf">
*  ALTIVEC PIM</a>: AltiVecTM Technology Programming Interface Manual,
*  Freescale Semiconductor, 1999.
*  - <a href="http://refspecs.linuxfoundation.org/ELF/ppc64/PPC-elf64abi.html">
*  64-bit PowerPC ELF Application Binary Interface (ABI)</a> Supplement 1.9.
*  - <a href="http://openpowerfoundation.org/wp-content/uploads/resources/leabi/leabi-20170510.pdf">
*  OpenPOWER ELF V2 application binary interface (ABI)</a>,
*  OpenPOWER Foundation, 2017.
*  - <a href="https://gcc.gnu.org/onlinedocs/">
*  Using the GNU Compiler Collection (GCC)</a>, Free Software Foundation, 1988-2018.
*  - <a href="https://sourceware.org/glibc/wiki/GNU_IFUNC">
*  What is an indirect function (IFUNC)?</a>, glibc wiki.
*  - <a href="https://ibm.ent.box.com/s/649rlau0zjcc0yrulqf4cgx5wk3pgbfk">
*  POWER8 Processor User’s Manual</a> for the Single-Chip Module.
*  - <a href="https://ibm.ent.box.com/s/8uj02ysel62meji4voujw29wwkhsz6a4">
*  POWER9 Processor User’s Manual</a>.
*  -  Warren, Henry S. Jr, Hacker's Delight, 2nd Edition, Upper Saddle River, NJ:
*  Addison Wesley, 2013.
*
*  \subsection mainpage_devel_history Release history
*
*  \subsubsection mainpage_rel_next Next Release
*
*  Proposed features:
*  - Enable and exploit Power10 ISA instructions for both new operations
*  and optimizations for existing operations.
*    - Quadword integer shift/rotate.
*    - Quadword integer signed/unsigned compare.
*    - Expand mask byte/halfword/word/doubleword/quadword.
*    - Extract/Insert exponent/significand for single/double/quad-precision
*  - Configure and build Power10 specific runtime libraries.
*  - Provide Vector Gather/Scatter operations.
*  - Provide access to the Quad-Precision operations from POWER9/10
*  vector implementations for POWER8
*
*  \subsubsection mainpage_rel_1_4 Release 1.0.4
*
*  Tagged v1.0.4 Release. This version is included in Fedora 33 and EPEL 7/8.
*  - Operations Implemented: 452
*  - Runtime library Symbols: 14
*  - POWER9 Specific cases: 122
*  - POWER8 Specific cases: 119
*  - GCC version specific cases: 63
*  - Clang specific cases: 26
*  - Endian Specific cases: 121
*
*  This version adds run-time libraries for large order integer multiplies
*  (512x512, 1024x1024, and 2048x2048) with interfaces defined in
*  vec_int512_ppc.h. These libraries support static linkage
*  (libpvecstatic.a) to platform specific implementations using
*  platform suffixes (ie vec_mul2048x2048_PWR9)
*  and dynamic linkage (libpvec.so) with IFUNC platform binding
*  (simply vec_mul2048x2048).
*
*  \paragraph mainpage_rel_1_4_clang Using the CLANG compiler
*  Application can compile with CLANG and use (most of the) the PVECLIB APIs.
*  The APIs for vec_f128_ppc.h and vec_bcd_ppc.h are disabled or limited
*  as CLANG does not support _Float128 nor Decimal Float types.
*  Also CLANG can not be used to build the PVECLIB runtime libraries
*  as it is missing the source attributes associated with the
*  <B>STTGNUIFUNC symbol type extension</B>. But CLANG compiled
*  applications can still link to and use these runtime functions.
*
*  \subsubsection mainpage_rel_1_3 Release 1.0.3
*
*  Tagged v1.0.3 for release. This version is included as package in Fedora 31.
*  - Operations Implemented: 386
*  - POWER9 Specific cases: 112
*  - POWER8 Specific cases: 112
*  - GCC version specific cases: 59
*  - Endian Specific cases: 87
*
*  Includes updates for vector BCD arithmetic and conversions.
*  Also vector quadword divide/modulo by 10**31 and 10**32.
*
*  \section mainpage_rationale Rationale
*
*  The C/C++ language compilers (that support PowerISA) may implement
*  vector intrinsic functions (compiler built-ins as embodied by
*  altivec.h). These vector intrinsics offer an alternative to
*  assembler programming, but do little to reduce the complexity
*  of the underlying PowerISA.
*  Higher level vector intrinsic operations are needed to improve
*  productivity and encourage developers to optimize their applications
*  for PowerISA.  Another key goal is to smooth over the complexity
*  of the evolving PowerISA and compiler support.
*
*  For example: the PowerISA 2.07 (POWER8) provides population count
*  and count leading zero operations on vectors of byte, halfword,
*  word, and doubleword elements but not on the whole vector as a
*  __int128 value. Before PowerISA 2.07, neither operation was
*  supported, for any element size.
*
*  Another example: The original <B>Altivec</B>
*  (AKA Vector Multimedia Extension (<B>VMX</B>))
*  provided Vector Multiply Odd / Even operations for signed / unsigned
*  byte and halfword elements.  The PowerISA 2.07 added Vector Multiply
*  Even/Odd operations for signed / unsigned word elements.  This
*  release also added a Vector Multiply Unsigned Word Modulo operation.
*  This was important to allow auto vectorization of C loops using
*  32-bit (int) multiply.
*
*  But PowerISA 2.07 did not add support for doubleword or quadword
*  (__int128) multiply directly.  Nor did it fill in the missing
*  multiply modulo operations for byte and halfword.  However it
*  did add support for doubleword and quadword add / subtract modulo,
*  This can be helpful, if you are willing to apply grade school
*  arithmetic (add, carry the 1) to vector elements.
*
*  PowerISA 3.0 (POWER9) adds a Vector Multiply-Sum Unsigned
*  Doubleword Modulo operation.  With this instruction (and a generated
*  vector of zeros as input) you can effectively implement the simple
*  doubleword integer multiply modulo operation in a few instructions.
*  Similarly for Vector Multiply-Sum Unsigned Halfword Modulo.
*  But this may not be obvious.
*
*  PowerISA 3.1 (POWER10) adds SIMD-equivalent forms of the FXU
*  multiply, divide, and modulo instructions. Also additional
*  128-bit divide, modulo, rotate, shift, and conversion operations.
*
*  This history embodies a set of trade-offs negotiated between the
*  Software and Processor design architects at specific points in time.
*  But most programmers would prefer to use a set of operators applied
*  across the supported element types/sizes while letting the
*  compiler/runtime deal with the instruction level details.
*
*  \subsection mainpage_sub0 POWER Vector Library Goals
*
*  Obviously many useful operations can be constructed from existing
*  PowerISA operations and GCC <altivec.h> built-ins but the
*  implementation may not be obvious. The optimum sequence will
*  vary across the PowerISA levels as new instructions are added.
*  And finally the compiler's built-in support for new PowerISA
*  instructions evolves with the compiler's release cycle.
*
*  So the goal of this project is to provide well crafted
*  implementations of useful vector and large number operations.
*
*  - Provide equivalent functions across versions of the PowerISA.
*  This includes some of the most useful vector instructions added to
*  POWER9 (PowerISA 3.0B) and POWER10 (PowerISA 3.1).
*  Many of these operations can be implemented as inline function in
*  a few vector instructions on earlier PowerISA versions.
*  - Provide equivalent functions across versions of the compiler.
*  For example built-ins provided in later versions of the compiler
*  can be implemented as inline functions with inline asm in earlier
*  compiler versions.
*  - Provide complete arithmetic operations across supported C types.
*  For example multiply modulo and even/odd for int, long, and __int128.
*  - Provide complete extended arithmetic (carry / extend /
*  multiple high) operations across supported C types.
*  For example add / subtract with carry and extend for int, long,
*  and __int128.
*  - Provide higher order functions not provided directly by the PowerISA.
*  For example:
*    - Vector SIMD implementation for ASCII __isalpha, etc.
*    - Vector Binary Code Decimal (BCD) Multiply/Divide/Convert.
*    - Vector __int128 implementations of Count Leading/Trailing Zeros,
*  Population Count, Shift left/right immediate.
*    - Large integer (128-bit and greater) multiply/divide.
*    - Vector Gather/Scatter.
*  - Most implementations should be small enough to inline and allow
*  the compiler opportunity to apply common optimization techniques.
*  - Larger Implementations should be built into platform specific
*  object archives and dynamic shared objects.
*  Shared objects should use <B>IFUNC resolvers</B> to bind the
*  dynamic symbol to best implementation for the platform
*  (see \ref main_libary_issues_0_0).
*
*  \subsubsection mainpage_sub0_1 POWER Vector Library Intrinsic headers
*
*  The POWER Vector Library will be primarily delivered as C language
*  inline functions in headers files.
*  - vec_common_ppc.h Typedefs and helper macros
*  - vec_int512_ppc.h Operations on multiple precision integer values
*  - vec_int128_ppc.h Operations on vector __int128 values
*  - vec_int64_ppc.h Operations on vector long int (64-bit) values
*  - vec_int32_ppc.h Operations on vector int (32-bit) values
*  - vec_int16_ppc.h Operations on vector short int (16-bit) values
*  - vec_char_ppc.h Operations on vector char (values) values
*  - vec_bcd_ppc.h Operations on vectors of Binary Code Decimal
*  and Zoned Decimal values
*  - vec_f128_ppc.h Operations on vector _Float128 values
*  - vec_f64_ppc.h Operations on vector double values
*  - vec_f32_ppc.h Operations on vector float values
*
*  \note The list above is complete in the current public github as a
*  first pass. A backlog of functions remain to be implemented
*  across these headers. Development continues while we work on the
*  backlog listed in:
*  <a href="https://github.com/open-power-sdk/pveclib/issues/13">
*  Issue #13 TODOs</a>
*
*
*  The goal is to provide high quality implementations that adapt to
*  the specifics of the compile target (-mcpu=) and compiler
*  (<altivec.h>) version you are using. Initially pveclib will focus on
*  the GCC compiler and -mcpu=[power7|power8|power9] for Linux.
*  Testing will focus on Little Endian (<B>powerpc64le</B> for power8
*  and power9 targets.  Any testing for Big Endian (<B>powerpc64</B>
*  will be initially restricted to power7 and power8 targets.
*
*  Expanding pveclib support beyond this list to include:
*  - additional compilers (ie Clang)
*  - additional PPC platforms (970, power6, ...)
*  - Larger functions that just happen to use vector registers
*  (Checksum, Crypto, compress/decompress, lower precision neural networks, ...)
*  .
*  will largely depend on additional skilled practitioners joining this
*  project and contributing (code and platform testing)
*  on a sustained basis.
*
*  \subsection mainpage_sub1 How pveclib is different from compiler vector built-ins
*
*  The PowerPC vector built-ins evolved from the original
*  <a href="https://www.nxp.com/docs/en/reference-manual/ALTIVECPIM.pdf">
*  AltiVec (TM) Technology Programming Interface Manual</a> (PIM).
*  The PIM defined the minimal extensions to the application binary
*  interface (ABI) required to support the Vector Facility.
*  This included new keywords (vector, pixel, bool) for defining
*  new vector types, and new operators (built-in functions) required
*  for any supporting and compliant C language compiler.
*
*  The vector built-in function support included:
*  - generic AltiVec operations, like vec_add()
*  - specific AltiVec operations (instructions, like vec_vaddubm())
*  - predicates computed from AltiVec operations, like vec_all_eq()
*  which are also generic
*
*  See \ref mainpage_sub2 for more details.
*
*  There are clear advantages with the compiler implementing the
*  vector operations as built-ins:
*  - The compiler can access the C language type information and
*  vector extensions to implement the function overloading
*  required to process generic operations.
*  - Built-ins can be generated inline, which eliminates function
*  call overhead and allows more compact code generation.
*  - The compiler can then apply higher order optimization across
*  built-ins including:
*  Local and global register allocation.
*  Global common subexpression elimination.
*  Loop-invariant code motion.
*  - The compiler can automatically select the best instructions
*  for the <I>target</I> processor ISA level
*  (from the -mcpu compiler option).
*
*  While this is an improvement over writing assembler code,  it does
*  not provide much function beyond the specific operations specified
*  in the PowerISA. As a result the generic operations were not
*  uniformly applied across vector element types. And this situation
*  often persisted long after the PowerISA added instructions for wider
*  elements. Some examples:
*  - Initially vec_add / vec_sub applied to float, int, short and char.
*  - Later compilers added support for double
*  (with POWER7 and the Vector Scalar Extensions (VSX) facility)
*  - Later still, integer long (64-bit) and __int128 support
*  (with POWER8 and PowerISA 2.07B).
*
*  But vec_mul / vec_div did not:
*  - Initially vec_mul applied to vector float only.
*  Later vector double was supported for POWER7 VSX.
*  Much later integer multiply modulo under the generic
*  vec_mul intrinsic.
*  - vec_mule / vec_mulo (Multiply even / odd elements)
*  applied to [signed | unsigned] integer short and char.
*  Later compilers added support for vector int after
*  POWER8 added vector multiply word instructions.
*  - vec_div was not included in the original PIM as
*  Altivec (VMX) only included vector reciprocal estimate for float
*  and no vector integer divide for any size.
*  Later compilers added support for vec_div float / double after
*  POWER7 (VSX) added vector divide single/double-precision
*  instructions.
*
*  \note While the processor you (plan to) use,  may support the
*  specific instructions you want to exploit,  the compiler you are
*  using may not support,  the generic or specific vector operations,
*  for the element size/types, you want to use.
*  This is common for GCC versions installed by "Enterprise Linux"
*  distributions. They tend to freeze the GCC version early and
*  maintain that GCC version for long term stability.
*  One solution is to use the
*  <a href="https://developer.ibm.com/linuxonpower/advance-toolchain/">
*  IBM Advance toolchain for Linux on Power</a> (AT).
*  AT is free for download and new AT versions are released yearly
*  (usually in August) with the latest stable GCC from that spring.
*
*  This can be a frustrating situation unless you are familiar with:
*  - the PowerISA and how it has evolved.
*  - the history and philosophy behind the implementation of
*  <altivec.h>.
*  - The specific level of support provided by the compiler(s) you are
*  using.
*
*  And to be fair, this author believes, this too much to ask from your
*  average library or application developer. A higher level and more
*  intuitive API is needed.
*
*  \subsubsection mainpage_sub_1_1 What can we do about this?
*
*  A lot can be done to improve this situation.
*  For older compilers we substitute inline assembler for missing
*  <altivec.h> operations.
*  For older processors we can substitute short instruction sequences
*  as equivalents for new instructions.
*  And useful higher level (and more intuitive) operations can be
*  written and shared.
*  All can be collected and provided in headers and libraries.
*
*  \paragraph mainpage_sub_1_1_1 Use inline assembler carefully
*
*  First the Binutils assembler is usually updated within weeks of the
*  public release of the PowerISA document. So while your compiler
*  may not support the latest vector operations as built-in operations,
*  an older compiler with an updated assembler,
*  may support the instructions as inline assembler.
*
*  Sequences of inline assembler instructions can be wrapped within
*  C language static inline functions and placed in a header files
*  for shared use. If you are careful with the input / output register
*  <I>constraints</I> the GCC compiler can provide local register
*  allocation and minimize parameter marshaling overhead. This is very
*  close (in function) to a specific Altivec (built-in) operation.
*
*  \note Using GCC's inline assembler can be challenging even for the
*  experienced programmer. The register constraints have grown in
*  complexity as new facilities and categories were added.
*  The fact that some (VMX) instructions are restricted to the original
*  32 Vector Registers (<B>VRs</B>) (the high half of the Vector-Scalar
*  Registers <B>VSRs</B>), while others (Binary and Decimal
*  Floating-Point) are restricted to the original 32 Floating-Point
*  Registers (<B>FPRs</B> (overlapping the low half of the VSRs), and
*  the new VSX instructions can access all 64 VSRs, is just one source
*  of complexity.
*  So it is very important to get your input/output constraints correct
*  if you want inline assembler code to work correctly.
*
*  In-line assembler should be
*  reserved for the first implementation using the latest PowerISA.
*  Where possible you should use existing vector built-ins to implement
*  specific operations for wider element types, support older hardware,
*  or higher order operations.
*  Again wrapping these implementations in static inline functions for
*  collection in header files for reuse and distribution is recommended.
*
*  \paragraph mainpage_sub_1_1_2 Define multi-instruction sequences to fill in gaps
*
*  The PowerISA vector facility has all the instructions you need to
*  implement extended precision operations for add, subtract,
*  and multiply. Add / subtract with carry-out and permute or
*  double vector shift and grade-school arithmetic is all you need.
*
*  For example the Vector Add Unsigned Quadword Modulo introduced in
*  POWER8 (PowerISA 2.07B) can be implemented for POWER7 and earlier
*  machines in 10-11 instructions. This uses a combination of
*  Vector Add Unsigned Word Modulo (vadduwm), Vector Add and Write
*  Carry-Out Unsigned Word (vaddcuw), and Vector Shift Left Double by
*  Octet Immediate (vsldoi), to propagate the word carries through the
*  quadword.
*
*  For POWER8 and later, C vector integer (modulo) multiply can be
*  implemented in a single Vector Unsigned Word Modulo (<B>vmuluwm</B>)
*  instruction. This was added explicitly to address vectorizing loops
*  using int multiply in C language code.  And some newer compilers do
*  support generic vec_mul() for vector int. But this is not
*  documented. Similarly for char (byte) and short (halfword) elements.
*
*  POWER8 also introduced Vector Multiply Even Signed|Unsigned Word
*  (<B>vmulesw</B>|<B>vmuleuw</B>)
*  and Vector Multiply Odd Signed|Unsigned Word
*  (<B>vmulosw</B>|<B>vmulouw</B>) instructions.
*  So you would expect the generic
*  vec_mule and vec_mulo operations to be extended to support
*  <I>vector int</I>,  as these operations have long been supported for
*  char and short.  Sadly this is not supported as of GCC 7.3 and inline
*  assembler is required for this case.
*  This support was added for GCC 8.
*
*  So what will the compiler do for vector multiply int (modulo, even,
*  or odd) for targeting power7?  Older compilers will reject this as a
*  <I>invalid parameter combination ...</I>.  A newer compiler may
*  implement the equivalent function in a short sequence of VMX
*  instructions from PowerISA 2.06 or earlier.
*  And GCC 7.3 does support vec_mul (modulo) for element types char,
*  short, and int. These sequences are in the 2-7 instruction range
*  depending on the operation and element type. This includes some
*  constant loads and permute control vectors that can be factored
*  and reused across operations.  See vec_muluwm() code for details.
*
*  Once the pattern is understood it is not hard to write equivalent
*  sequences using operations from the original <altivec.h>.  With a
*  little care these sequences will be compatible with older compilers
*  and older PowerISA versions.
*
*  \paragraph mainpage_sub_1_1_3 Define new and useful operations
*
*  These concepts can be extended to operations that PowerISA and the
*  compiler does not support yet.  For example; a processor that may
*  not have multiply even/odd/modulo of the required width (word,
*  doubleword, or quadword). This might take 10-12 instructions to
*  implement the next element size bigger then the current processor.
*  A full 128-bit by 128-bit multiply with 256-bit result only
*  requires 36 instructions on POWER8 (using multiple word even/odd)
*  and 15 instructions on POWER9 (using vmsumudm).
*
*  Other examples include Vector Scatter/Gather operations.
*  The PowerISA does not provide Scatter/Gather instructions.
*  It does provide instructions to directly store/load single vector
*  elements to/from storage.
*  For example; vec_vlxsfdx() and vec_vstxsfdx().
*  Batches (in groups of 2-4) of these,
*  combined with appropriate vector splat/merge operations,
*  provide the effective Scatter/Gather operations:
*  - Storing multiple vector elements to disjoint storage locations.
*  - Loading multiple vector elements from disjoint storage locations.
*
*  The PowerISA does not provide for effective address computation
*  from vector registers or elements. All Load/store instructions
*  require scalar GPRs for Base Address and Index (offset).
*  For 64-bit PowerISA, effective address (EA) calculations use 64-bit
*  two's compliment addition.
*
*  This is not a serious limitation as often the element offsets are
*  scalar constants or variables. So using multiple integer scalars as
*  offsets for Scatter/Gather operation is reasonable
*  (and highest performing) option.
*  For example; vec_vglfdso() and vec_vsstfdso().
*
*  However there are times when it is useful to use vector elements as
*  load/store offsets or array indexes. This requires a transfer of
*  elements from a vector to scalar GPRs.
*  When using smaller (than doubleword)
*  elements, they are extended (signed or unsigned) to
*  64-bit (doubleword) before use in storage EA calculates.
*  For example; vec_vglfddo(), and vec_vsstfddo().
*
*  \note This behavior is defined by PowerISA section 1.10.3
*  Effective Address Calculation.
*
*  If left shifts are required (to convert array indexes to offsets),
*  64-bit shifts are applied after the element is extended.
*  For example; vec_vglfddsx(), vec_vglfddx(),
*  vec_vsstfddsx(), and vec_vsstfddx().
*
*  \note Similar gather/scatter operations are provided for doubleword
*  integer elements (vec_int64_ppc.h) and word integer/float elements
*  (vec_int64_ppc.h, vec_f32_ppc.h).
*
*  These integer extension and left shift operations can be on vector
*  elements (before transfer) or scalar values (after transfer).
*  The best (performing) sequence will depend on the compile target's
*  PowerISA version and micro-architecture.
*
*  Starting with Power8 the ISA provides for direct transfers from
*  vector elements to GRPs (<B>Move From VSR Doubleword</B>).
*  Power9 adds <B>Move From VSR Lower Doubleword</B> simplifying
*  access to the whole (both doublewords of the) 128-bit VSR.
*
*
*  \paragraph mainpage_sub_1_1_4 Leverage other PowerISA facilities
*
*  Also many of the operations missing from the vector facility,
*  exist in the Fixed-point, Floating-point,
*  or Decimal Floating-point scalar facilities.
*  There will be some loss of efficiency in the data transfer but
*  compared to a complex operation like divide or decimal conversions,
*  this can be a workable solution.
*  On older POWER processors (before power7/8) transfers between
*  register banks (GPR, FPR, VR) had to go through memory.
*  But with the VSX facility (POWER7) FPRs and VRs overlap with the
*  lower and upper halves of the 64 VSR registers.
*  So FPR <-> VSR transfer are 0-2 cycles latency.
*  And with power8 we have direct transfer (GPR <-> FPR | VR | VSR)
*  instructions in the 4-5 cycle latency range.
*
*  For example POWER8 added Decimal (<B>BCD</B>) Add/Subtract Modulo
*  (<B>bcdadd</B>, <B>bcdsub</B>) instructions for signed 31 digit
*  vector values.
*  POWER9 added Decimal Convert From/To Signed Quadword
*  (<B>bcdcfsq</B>, <B>bcdctsq</B>) instructions.
*  So far vector unit does not support BCD multiply / divide.
*  But the Decimal Floating-Point (<B>DFP</B>) facility (introduced
*  with PowerISA 2.05 and Power6) supports up to 34-digit
*  (__Decimal128) precision and all the expected
*  (add/subtract/multiply/divide/...) arithmetic operations. DFP also
*  supports conversion to/from 31-digit BCD and __Decimal128 precision.
*  This is all supported with a hardware Decimal Floating-Point Unit
*  (<B>DFU</B>).
*
*  So we can implement vec_bcdadd() and vec_bcdsub() with single instructions
*  on POWER8, and 10-11 instructions for Power6/7.
*  This count include the VSR <-> FPRp transfers,
*  BCD <-> DFP conversions, and DFP add/sub.
*  Similarly for vec_bcdcfsq() and vec_bcdctsq(). The POWER8 and earlier
*  implementations are a bit bigger (83 and 32 instruction respectively)
*  but even the POWER9 hardware implementation runs 37 and 23 cycles
*  (respectively).
*
*  The vec_bcddiv() and vec_bcdmul() operations are implement by
*  transfer/conversion to __Decimal128 and execute in the DFU.
*  This is slightly complicated by the requirement to preserve correct
*  fix-point alignment/truncation in the floating-point format.
*  The operation timing runs ~100-200 cycles mostly driven the DFP
*  multiply/divide and the number of digits involved.
*
*  \note So why does anybody care about BCD and DFP? Sometimes you get
*  large numbers in decimal that you need converted to binary for
*  extended computation. Sometimes you need to display the results of
*  your extended binary computation in decimal. The multiply by 10 and
*  BCD vector operations help simplify and speed-up these conversions.
*
*  \paragraph mainpage_sub_1_1_5 Use clever tricks
*
*  And finally: Henry S. Warren's wonderful book Hacker's Delight
*  provides inspiration for SIMD versions of; count leading zeros,
*  population count, parity, etc.
*
*  \subsubsection  mainpage_sub_1_2 So what can the Power Vector Library project do?
*
*  Clearly the PowerISA provides multiple, extensive, and powerful
*  computational facilities that continue to evolve and grow.
*  But the best instruction sequence for a specific computation depends
*  on which POWER processor(s) you have or plan to support.
*  It can also depend on the specific compiler version you use, unless
*  you are willing to write some of your application code in assembler.
*  Even then you need to be aware of the PowerISA versions and when
*  specific instructions where introduced.  This can be frustrating if
*  you just want to port your application to POWER for a quick
*  evaluation.
*
*  So you would like to start evaluating how to leverage this power
*  for key algorithms at the heart of your application.
*  - But you are working with an older POWER processor
*  (until the latest POWER box is delivered).
*  - Or the latest POWER machine just arrived at your site (or cloud)
*  but you are stuck using an older/stable Linux distro version
*  (with an older distro compiler).
*  - Or you need extended precision multiply for your crypto code
*  but you are not really an assembler level programmer
*  (or don't want to be).
*  - Or you would like to program with higher level operations to
*  improve your own productivity.
*
*  Someone with the right background (knowledge of the PowerISA,
*  assembler level programming, compilers and the vector built-ins,
*  ...) can solve any of the issues described above. But you don't
*  have time for this.
*
*  There should be an easier way to exploit the POWER vector hardware
*  without getting lost in the details. And this extends beyond
*  classical vector (Single Instruction Multiple Data (SIMD))
*  programming to exploiting larger data width (128-bit and beyond),
*  and larger register space (64 x 128 Vector Scalar Registers)
*
*  \paragraph mainpage_para_1_2_0 Vector Add Unsigned Quadword Modulo example
*  Here is an example of what can be done: \code
static inline vui128_t
vec_adduqm (vui128_t a, vui128_t b)
{
  vui32_t t;
#ifdef _ARCH_PWR8
#ifndef vec_vadduqm
  __asm__(
      "vadduqm %0,%1,%2;"
      : "=v" (t)
      : "v" (a),
      "v" (b)
      : );
#else
  t = (vui32_t) vec_vadduqm (a, b);
#endif
#else
  vui32_t c, c2;
  vui32_t z= { 0,0,0,0};

  c = vec_vaddcuw ((vui32_t)a, (vui32_t)b);
  t = vec_vadduwm ((vui32_t)a, (vui32_t)b);
  c = vec_sld (c, z, 4);
  c2 = vec_vaddcuw (t, c);
  t = vec_vadduwm (t, c);
  c = vec_sld (c2, z, 4);
  c2 = vec_vaddcuw (t, c);
  t = vec_vadduwm (t, c);
  c = vec_sld (c2, z, 4);
  t = vec_vadduwm (t, c);
#endif
  return ((vui128_t) t);
}
*  \endcode
*
*  The <B>_ARCH_PWR8</B> macro is defined by the compiler when it targets
*  POWER8 (PowerISA 2.07) or later. This is the first processor and
*  PowerISA level to support vector quadword add/subtract. Otherwise we
*  need to use the vector word add modulo and vector word add and
*  write carry-out word, to add 32-bit chunks and propagate the
*  carries through the quadword.
*
*  One little detail remains. Support for vec_vadduqm was added to GCC
*  in March of 2014, after GCC 4.8 was released and GCC 4.9's feature
*  freeze.  So the only guarantee is that this feature is in GCC
*  5.0 and later.  At some point this change was backported to GCC 4.8
*  and 4.9 as it is included in the current GCC 4.8/4.9 documentation.
*  When or if these backports where propagated to a specific Linux
*  Distro version or update is difficult to determine.
*  So support for this vector built-in dependes on the specific
*  version of the GCC compiler, or if specific Distro update includes
*  these specific backports for the GCC 4.8/4.9 compiler they support.
*  The: \code
*  #ifndef vec_vadduqm
*  \endcode
*  C preprocessor conditional checks if the <B>vec_vadduqm</B>
*  is defined in <altivec.h>. If defined we can assume that the
*  compiler implements <B>__builtin_vec_vadduqm</B> and that
*  <altivec.h> includes the macro definition: \code
#define vec_vadduqm __builtin_vec_vadduqm
*  \endcode
*  For <B>_ARCH_PWR7</B> and earlier we need a little grade school
*  arithmetic using Vector Add Unsigned Word Modulo (<B>vadduwm</B>)
*  and Vector Add and Write Carry-Out Unsigned Word (<B>vaddcuw</B>).
*  This treats the vector __int128 as 4 32-bit binary digits.
*  The first instruction sums each (32-bit digit) column and the second
*  records the carry out of the high order bit of each word.
*  This leaves the carry bit in the original (word) column,
*  so a shift left 32-bits is needed to line up
*  the carries with the next higher word.
*
*  To propagate any carries across all 4 (word) digits,  repeat this
*  (add / carry / shift) sequence three times.
*  Then a final add modulo word to complete the 128-bit add.
*  This sequence requires 10-11 instructions. The 11th instruction is
*  a vector splat word 0 immediate, which in needed in the shift left
*  (vsldoi) instructions. This is common in vector codes and the
*  compiler can usually reuse this register across several blocks of
*  code and inline functions.
*
*  For POWER7/8 these instructions are all 2 cycle latency and 2 per
*  cycle throughput.  The vadduwm / vaddcuw instruction pairs should
*  issue in the same cycle and execute in parallel.
*  So the expected latency for this sequence is 14 cycles.
*  For POWER8 the vadduqm instruction has a 4 cycle latency.
*
*  Similarly for the carry / extend forms which can be combined to
*  support wider (256, 512, 1024, ...) extended arithmetic.
*  \sa vec_addcuq, vec_addeuqm, and vec_addecuq
*
*  \paragraph mainpage_para_1_2_1 Vector Multiply-by-10 Unsigned Quadword example
*
*  PowerISA 3.0 (POWER9) added this instruction and it's extend / carry
*  forms to speed up decimal to binary conversion for large numbers.
*  But this operation is generally useful and not that hard to implement
*  for earlier processors.
*  \code
static inline vui128_t
vec_mul10uq (vui128_t a)
{
  vui32_t t;
#ifdef _ARCH_PWR9
  __asm__(
      "vmul10uq %0,%1;\n"
      : "=v" (t)
      : "v" (a)
      : );
#else
  vui16_t ts = (vui16_t) a;
  vui16_t t10;
  vui32_t t_odd, t_even;
  vui32_t z = { 0, 0, 0, 0 };
  t10 = vec_splat_u16(10);
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
  t_even = vec_vmulouh (ts, t10);
  t_odd = vec_vmuleuh (ts, t10);
#else
  t_even = vec_vmuleuh(ts, t10);
  t_odd = vec_vmulouh(ts, t10);
#endif
  t_even = vec_sld (t_even, z, 2);
#ifdef _ARCH_PWR8
  t = (vui32_t) vec_vadduqm ((vui128_t) t_even, (vui128_t) t_odd);
#else
  t = (vui32_t) vec_adduqm ((vui128_t) t_even, (vui128_t) t_odd);
#endif
#endif
  return ((vui128_t) t);
}
*  \endcode
*
*
*  Notice that under the <B>_ARCH_PWR9</B> conditional, there is no
*  check for the specific <B>vec_vmul10uq</B> built-in.  As of this
*  writing <B>vec_vmul10uq</B> is not included in the
*  <I>OpenPOWER ELF2 ABI</I> documentation nor in the latest GCC trunk
*  source code.
*
*  \note The <I>OpenPOWER ELF2 ABI</I> does define <B>bcd_mul10</B>
*  which (from the description) will actually generate Decimal Shift
*  (<B>bcds</B>). This instruction shifts 4-bit nibbles (BCD digits)
*  left or right while preserving the BCD sign nibble in bits 124-127,
*  While this is a handy instruction to have, it is not the same
*  operation as <B>vec_vmul10uq</B>, which is a true 128-bit binary
*  multiply by 10.  As of this writing <B>bcd_mul10</B> support is not
*  included in the latest GCC trunk source code.
*
*  For <B>_ARCH_PWR8</B> and earlier we need a little grade school
*  arithmetic using <B>Vector Multiply Even/Odd Unsigned Halfword</B>.
*  This treats the vector __int128 as 8 16-bit binary digits.  We
*  multiply each of these 16-bit digits by 10, which is done in two
*  (even and odd) parts. The result is 4 32-bit (2 16-bit digits)
*  partial products for the even digits and 4 32-bit products for the
*  odd digits. The vector register (independent of endian);
*  the even product elements are higher order and odd product elements
*  are lower order.
*
*  The even digit partial products are offset right by 16-bits in the
*  register.  If we shift the even products left 1 (16-bit) digit,
*  the even digits are lined up in columns with the odd digits.  Now
*  we can sum across partial products to get the final 128 bit product.
*
*  Notice also the conditional code for endian around the
*  <B>vec_vmulouh</B> and <B>vec_vmuleuh</B> built-ins: \code
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
*  \endcode
*
*  Little endian (<B>LE</B>) changes the element numbering.
*  This also changes the meaning of even / odd and this
*  effects the code generated by compilers. But the relationship
*  of high and low order bytes, within multiplication products,
*  is defined by the hardware and does not change.
*  (See: \ref mainpage_endian_issues_1_1)
*  So the pveclib implementation needs to pre-swap the even/odd
*  partial product multiplies for LE.  This in effect nullifies the
*  even / odd swap hidden in the compilers <B>LE</B> code generation
*  and the resulting code gives the correct results.
*
*  Now we are ready to sum the partial product <I>digits</I> while
*  propagating the digit carries across the 128-bit product.
*  For <B>_ARCH_PWR8</B> we can use <B>Vector Add Unsigned Quadword
*  Modulo</B> which handles all the internal carries in hardware.
*  Before <B>_ARCH_PWR8</B> we only have <B>Vector Add Unsigned Word
*  Modulo</B> and <B>Vector Add and Write Carry-Out Unsigned Word</B>.
*
*  We see these instructions used in the <B>else</B> leg of the
*  pveclib <B>vec_adduqm</B> implementation above. We can assume that
*  this implementation is correct and tested for supported platforms.
*  So here we use another pveclib function to complete the
*  implementation of <B>Vector Multiply-by-10 Unsigned Quadword</B>.
*
*  Again similarly for the carry / extend forms which can be combined
*  to support wider (256, 512,  1024, ...) extended decimal to binary
*  conversions.
*  \sa vec_mul10cuq, vec_mul10euq, and vec_mul10ecuq
*
*  And similarly for full 128-bit x 128-bit multiply which combined
*  with the add quadword carry / extended forms above can be used to
*  implement wider (256, 512, 1024, ...) multiply operations.
*  \sa vec_mulluq and vec_muludq
*  \sa \ref i32_example_0_0_0
*  \sa \ref i32_example_0_0_1
*
*  \subsubsection mainpage_sub3 pveclib is not a matrix math library
*
*  The pveclib does not implement general purpose matrix math operations.
*  These should continue to be developed and improved within existing
*  projects (ie LAPACK, OpenBLAS, ATLAS, etc).
*  We believe that pveclib will be helpful to implementors of matrix
*  math libraries by providing a higher level, more portable,
*  and more consistent vector interface for the PowerISA.
*
*  The decision is still pending on:
*  extended arithmetic, cryptographic,
*  compression/decompression, pattern matching / search
*  and small vector libraries (libmvec).
*  This author believes that the small vector math
*  implementation should be part of GLIBC (libmvec).
*  But the lack of optimized implementations or even good documentation
*  and examples for these topics is a concern.
*  This may be something that PVECLIB can address by providing enabling
*  kernels or examples.
*
*  \subsection  mainpage_sub_2x Practical considerations.
*
*  \subsubsection mainpage_endian_issues_1_1 General Endian Issues
*
*  For POWER8, IBM made the explicit decision to support Little Endian
*  (<B>LE</B>) data format in the Linux ecosystem.  The goal was to
*  enhance application code portability across Linux platforms.
*  This goal was integrated into the OpenPOWER ELF V2
*  Application Binary Interface <B>ABI</B> specification.
*
*  The POWER8 processor architecturally supports an <I>Endian Mode</I>
*  and supports both BE and LE storage access in hardware.  However,
*  register to register operations are not effected by endian mode.
*  The ABI extends the LE storage format to vector register (logical)
*  element numbering.
*  See OpenPOWER ABI specification
*  <a href="http://openpowerfoundation.org/wp-content/uploads/resources/leabi/content/dbdoclet.50655244_pgfId-1095944.html"
*  >Chapter 6. Vector Programming Interfaces</a> for details.
*
*  This has no effect for most altivec.h
*  operations where the input elements and the results "stay in their
*  lanes".  For operations of the form (T[n] = A[n] op B[n]),
*  it does not matter if elements are numbered [0, 1, 2, 3] or
*  [3, 2, 1, 0].
*
*  But there are cases where element renumbering can change the
*  results.  Changing element numbering does change the even / odd
*  relationship for merge and integer multiply.  For <B>LE</B>
*  targets, operations accessing even vector elements are
*  implemented using the equivalent odd instruction (and visa versa)
*  and inputs are swapped.  Similarly for high and low merges.
*  Inputs are also swapped for Pack, Unpack, and Permute operations
*  and the permute select vector is inverted.
*  The above is just a sampling of a larger list of
*  <I>LE transforms</I>.
*  The OpenPOWER ABI specification provides a helpful table of
*  <a href="http://openpowerfoundation.org/wp-content/uploads/resources/leabi/content/dbdoclet.50655244_90667.html"
*  >Endian-Sensitive Operations</a>.
*
*  \note This means that the vector built-ins provided by altivec.h
*  may not generate the instructions you expect.
*
*  This does matter when doing extended precision arithmetic.
*  Here we need to maintain most-to-least significant byte order and
*  align "digit" columns for summing partial products.  Many of these
*  operations where defined long before Little Endian was seriously
*  considered and are decidedly Big Endian in register format.
*  Basically, any operation where the element changes size
*  (truncated, extended, converted, subsetted) from input to output
*  is suspect for <B>LE</B> targets.
*
*  The coding for these higher level operations
*  is complicated by <I>Little Endian</I> (LE) support as
*  specified in the OpenPOWER ABI and as implemented in the compilers.
*  Little Endian changes the effective vector element numbering and
*  the location of even and odd elements.
*
*  This is a general problem for using vectors to implement extended
*  precision arithmetic.
*  The multiply even/odd operations being the primary example.
*  The products are double-wide and in BE order in the vector register.
*  This is reinforced by the Vector Add/Subtract Unsigned
*  Doubleword/Quadword instructions.
*  And the products from multiply even instructions are always
*  <I>numerically</I> higher digits then multiply odd products.
*  The pack, unpack, and sum operations have similar issues.
*
*  This matters when you need to align (shift) the partial products
*  or select the <I>numerically</I> high or lower portion of the
*  products.
*  The (high to low) order of elements for the multiply has to match
*  the order of the largest element size used in accumulating partial
*  sums. This is normally a quadword (vadduqm instruction).
*
*  So the element order is fixed while
*  the element numbering and the partial products (between even and
*  odd) will change between BE and LE. This effects splatting and octet
*  shift operations required to align partial product for summing.
*  These are the places where careful programming is
*  required, to nullify the compiler's LE transforms, so we will get
*  the correct numerical answer.
*
*  So what can the Power Vector Library do to help?
*  - Be aware of these mandated LE transforms and if required provide
*  compliant inline assembler implementations for LE.
*  - Where required for correctness provide LE specific implementations
*  that have the effect of nullifying the unwanted transforms.
*  - Provide higher level operations that help pveclib and
*  applications code in an endian neutral way and get correct results.
*
*  \sa \ref i32_endian_issues_0_0
*  \sa \ref mainpage_para_1_2_1
*
*  \subsubsection  mainpage_sub_1_3 Returning extended quadword results.
*
*  Extended quadword add, subtract and multiply results can exceed the
*  width of a single 128-bit vector. A 128-bit add can produce 129-bit
*  results. A unsigned 128-bit by 128-bit multiply result can produce
*  256-bit results. This is simplified for the <I>modulo</I> case where
*  any result bits above the low order 128 can be discarded.
*  But extended arithmetic requires returning the full precision
*  result.  Returning double wide quadword results are a complication
*  for both RISC processor and C language library design.
*
*  \paragraph mainpage_sub_1_3_1 PowerISA and Implementation.
*
*  For a RISC processor, encoding multiple return registers forces hard
*  trade-offs in a fixed sized instruction format.  Also building a
*  vector register file that can support at least one (or more) double
*  wide register writes per cycle is challenging.  For a super-scalar
*  machine with multiple vector execution pipelines, the processor can
*  issue and complete multiple instructions per cycle. As most
*  operations return single vector results, this is a higher
*  priority than optimizing for double wide results.
*
*  The PowerISA addresses this by splitting these operations into two
*  instructions that execute independently. Here independent means that
*  given the same inputs, one instruction does not depend on the result
*  of the other. Independent instructions can execute out-of-order,
*  or if the processor has multiple vector execution pipelines,
*  can execute (issue and complete) concurrently.
*
*  The original VMX implementation had Vector Add/Subtract Unsigned
*  Word Modulo (<B>vadduwm</B> / <B>vsubuwm</B>), paired with
*  Vector Add/Subtract and Write Carry-out Unsigned Word
*  (<B>vaddcuw</B> / <B>vsubcuw</B>).
*  Most usage ignores the carry-out and only uses the add/sub modulo
*  instructions.  Applications requiring extended precision,
*  pair the add/sub modulo with add/sub write carry-out, to capture
*  the carry and propagate it to higher order bits.
*
*  The (four word) carries are generated into the same <I>word lane</I>
*  as the source addends and modulo result.
*  Propagating the carries require a separate shift (to align the
*  carry-out with the low order (carry-in) bit of the next higher word)
*  and another add word modulo.
*
*  POWER8 (PowerISA 2.07B) added full Vector Add/Subtract Unsigned
*  Quadword Modulo (<B>vadduqm</B> / <B>vsubuqm</B>) instructions,
*  paired with corresponding Write Carry-out instructions.
*  (<B>vaddcuq</B> / <B>vsubcuq</B>).
*  A further improvement over the word instructions was the addition
*  of three operand <I>Extend</I> forms which combine add/subtract
*  with carry-in (<B>vaddeuqm</B>, <B>vsubeuqm</B>,
*  <B>vaddecuq</B> and <B>vsubecuq</B>).
*  This simplifies propagating the carry-out into higher quadword
*  operations.
*  \sa vec_adduqm, vec_addcuq, vec_addeuqm, vec_addecuq
*
*  POWER9 (PowerISA 3.0B) added Vector Multiply-by-10 Unsigned
*  Quadword (Modulo is implied), paired with Vector Multiply-by-10 and
*  Write Carry-out Unsigned Quadword
*  (<B>vmul10uq</B> / <B>vmul10cuq</B>).
*  And the <I>Extend</I> forms (<B>vmul10euq</B> / <B>vmul10ecuq</B>)
*  simplifies the digit (0-9) carry-in for extended precision decimal
*  to binary conversions.
*  \sa vec_mul10uq, vec_mul10cuq, vec_mul10euq, vec_mul10ecuq
*
*  The VMX integer multiply operations are split into multiply even/odd
*  instructions by element size.
*  The product requires the next larger element size
*  (twice as many bits).
*  So a vector multiply byte would generate 16 halfword products
*  (256-bits in total).
*  Requiring separate even and odd multiply instructions cuts the
*  total generated product bits (per instruction) in half.
*  It also simplifies the hardware design by keeping the generated
*  product in adjacent element lanes.
*  So each vector multiply even or odd byte operation generates 8
*  halfword products (128-bits) per instruction.
*
*  This multiply even/odd technique applies to most element sizes from
*  byte up to doubleword.  The original VMX supports multiply even/odd
*  byte and halfword operations.  In the original VMX, arithmetic
*  operations where restricted to byte, halfword, and word elements.
*  Multiply halfword products fit within the integer word element.
*  No multiply byte/halfword modulo instructions were provided, but
*  could be implemented via a vmule, vmulo, vperm sequence.
*
*  POWER8 (PowerISA 2.07B) added multiply even/odd word and multiply
*  modulo word instructions.
*  \sa vec_muleuw, vec_mulouw, vec_muluwm
*
*  The latest PowerISA (3.0B for POWER9) does add a doubleword integer
*  multiply via
*  <B>Vector Multiply-Sum unsigned Doubleword Modulo</B>.
*  This is a departure from the Multiply even/odd byte/halfword/word
*  instructions available in earlier Power processors.
*  But careful conditioning of the inputs can generate the equivalent
*  of multiply even/odd unsigned doubleword.
*  \sa vec_msumudm, vec_muleud, vec_muloud
*
*  This (multiply even/odd) technique breaks down when the input
*  element size is quadword or larger.
*  A quadword integer multiply forces a different split.
*  The easiest next step would be a high/low split (like the
*  Fixed-point integer multiply).
*  A multiply low (modulo) quadword would be a useful function.
*  Paired with multiply high quadword provides the double quadword
*  product. This would provide the basis for higher (multi-quadword)
*  precision multiplies.
*  \sa vec_mulluq, vec_muludq
*
*  \paragraph mainpage_sub_1_3_2 C Language restrictions.
*
*  The Power Vector Library is implemented using C language (inline)
*  functions and this imposes its own restrictions.
*  Standard C language allows an
*  arbitrary number of formal parameters and one return value per
*  function.  Parameters and return values with simple C types are
*  normally transfered (passed / returned) efficiently in local
*  (high performance) hardware registers.
*  Aggregate types (struct, union, and arrays of arbitrary size)
*  are normally handled by pointer indirection.
*  The details are defined in the appropriate Application Binary
*  Interface (ABI) documentation.
*
*  The POWER processor provides lots of registers (96) so we want to
*  use registers wherever possible.  Especially when our application is
*  composed of collections of small functions.  And more especially
*  when these functions are small enough to inline and we want the
*  compiler to perform local register allocation and common
*  subexpression elimination optimizations across these functions.
*  The PowerISA defines 3 kinds of registers;
*  - General Purpose Registers (GPRs),
*  - Floating-point Registers (FPRs),
*  - Vector registers (VRs),
*
*  with 32 of each kind.
*  We will ignore the various special registers for now.
*
*  The PowerPC64 64-bit ELF (and OpenPOWER ELF V2) ABIs normally
*  pass simple arguments and return values in a single register
*  (of the appropriate kind) per value.
*  Arguments of aggregate types are passed as storage pointers in
*  General Purpose Registers (GPRs).
*
*  The language specification, the language implementation, and the
*  ABI provide some exceptions.
*  The C99 language adds _Complex floating types which are composed of
*  real and imaginary parts.  GCC adds _Complex integer types.
*  For PowerPC ABIs complex values are held in a pair of registers of
*  the appropriate kind.
*  C99 also adds double word integers as the <I>long long int</I> type.
*  This only matters for PowerPC 32-bit ABIs.
*  For PowerPC64 ABIs <I>long long</I> and <I>long</I> are both 64-bit
*  integers and are held in 64-bit GPRs.
*
*  GCC also adds the __int128 type for some targets including the
*  PowerPC64 ABIs. Values of __int128 type are held (for operations,
*  parameter passing and function return) in 64-bit GPR pairs.
*  Starting with version 4.9 GCC supports the vector signed/unsigned
*  __int128 type.
*  This is passed and returned as a single vector register and should
*  be used for all 128-bit integer types
*  (bool/signed/unsigned).
*
*  GCC supports __ibm128 and _Decimal128 floating point types which are
*  held in Floating-point Registers pairs.
*  These are distinct types from vector double and oriented differently
*  in the VXS register file.
*  But the doubleword halves can be moved between types using the VSX
*  permute double word immediate instructions (xxpermdi).
*  This useful for type conversions and implementing some vector
*  BCD operations.
*
*  GCC recently added the __float128 floating point type which are held
*  in single vector register. The compiler considers this to be floating
*  scalar and is not cast compatible with any vector type. To access the
*  __float128 value as a vector it must be passed through a union.
*
*  \note The implementation will need to provide transfer functions between
*  vectors and other 128-bit types.
*
*  GCC defines Generic Vector Extensions that allow typedefs for
*  vectors of various element sizes/types and generic SIMD
*  (arithmetic, logical, and element indexing) operations.
*  For PowerPC64 ABIs this is currently restricted to 16-byte vectors
*  as defined in <altivec.h>.
*  For currently available compilers attempts to define vector types
*  with larger (32 or 64 byte) <I>vector_size</I> values are treated
*  as arrays of scalar elements.  Only vector_size(16) variables are
*  passed and returned in vector registers.
*
*  The OpenPOWER 64-Bit ELF V2 ABI Specification makes specific
*  provisions for passing/returning <I>homogeneous aggregates</I>
*  of multiple like (scalar/vector) data types.
*  Such aggregates can be passed/returned
*  as up to eight floating-point or vector registers.
*  A parameter list may include multiple <I>homogeneous aggregates</I>
*  with up to a total of twelve parameter registers.
*
*  This is defined for the Little Endian ELF V2 ABI and is
*  not applicable to Big Endian ELF V1 targets.
*  Also GCC versions before GCC8, do not fully implement this ABI
*  feature, and revert to old ABI structure passing
*  (passing through storage).
*
*  Passing large <I>homogeneous aggregates</I> becomes the preferred
*  solution as PVECLIB starts to address wider (256 and 512-bit)
*  vector operations. For example the ABI allows passing up to 3
*  512-bit parameters and return a 1024-bit result in vector registers
*  (as in vec_madd512x512a512_inline()). For large multi-quadword
*  precision operations the only practical solution uses reference
*  parameters to arrays or structs in storage
*  (as in vec_mul2048x2048()). See vec_int512_ppc.h for more examples.
*
*  So we have shown that there are mechanisms for functions to return
*  multiple vector register values.
*
*  \paragraph mainpage_sub_1_3_3 Subsetting the problem.
*
*  We can simplify this problem by remembering that:
*  - Only a subset of the pveclib functions need to return more than
*  one 128-bit vector.
*  - The PowerISA normally splits these cases into multiple
*  instructions anyway.
*  - Most of these functions are small and fully inlined.
*  - The exception will be the multiple quadword precision arithmetic
*  operations.
*
*  So we have two (or three) options given the current state of GCC
*  compilers in common use:
*  - Mimic the PowerISA and split the operation into two functions,
*  where each function only returns (up to) 128-bits of the result.
*  - Use pointer parameters to return a second vector value in
*  addition to the function return.
*  - Support both options above and let the user decide which works
*  best.
*  - With a availability of GCC 8/9 compilers, pass/return 256, 512
*  and 1024-bit vectors as <I>homogeneous aggregates</I>.
*
*  The add/subtract quadword operations provide good examples.
*  For exmaple adding two 256-bit unsigned integer values and returning
*  the 257-bit (the high / low sum and the carry)result looks like this: \code
s1 = vec_vadduqm (a1, b1); // sum low 128-bits a1+b1
c1 = vec_vaddcuq (a1, b1); // write-carry from low a1+b1
s0 = vec_vaddeuqm (a0, b0, c1); // Add-extend high 128-bits a0+b0+c1
c0 = vec_vaddecuq (a0, b0, c1); // write-carry from high a0+b0+c1
*  \endcode
*  This sequence uses the built-ins from <altivec.h> and generates
*  instructions that will execute on POWER8 and POWER9.
*  The compiler must target POWER8 (-mcpu=power8) or higher.
*  In fact the compile will fail if the target is POWER7.
*
*  Now let's look at the pveclib version of these operations from
*  <vec_int128_ppc.h>: \code
s1 = vec_adduqm (a1, b1); // sum low 128-bits a1+b1
c1 = vec_addcuq (a1, b1); // write-carry from low a1+b1
s0 = vec_addeuqm (a0, b0, c1); // Add-extend high 128-bits a0+b0+c1
c0 = vec_addecuq (a0, b0, c1); // write-carry from high a0+b0+c1
*  \endcode
*  Looks almost the same but the operations do not use the 'v' prefix
*  on the operation name.
*  This sequence generates the same instructions for (-mcpu=power8)
*  as the <altivec.h> version above.
*  It will also generate a different (slightly longer) instruction
*  sequence for (-mcpu=power7) which is functionally equivalent.
*
*  The pveclib <vec_int128_ppc.h> header also provides a coding style
*  alternative: \code
s1 = vec_addcq (&c1, a1, b1);
s0 = vec_addeq (&c0, a0, b0, c1);
*  \endcode
*  Here vec_addcq combines the adduqm/addcuq operations into a
*  <I>add and carry quadword</I> operation.
*  The first parameter is a pointer to vector to receive the carry-out
*  while the 128-bit modulo sum is the function return value.
*  Similarly vec_addeq combines the addeuqm/addecuq operations
*  into a <I>add with extend and carry quadword</I> operation.
*
*  As these functions are inlined by the compiler the implied
*  store / reload of the carry can be converted into a simple
*  register assignment.
*  For (-mcpu=power8) the compiler should generate the same
*  instruction sequence as the two previous examples.
*
*  For (-mcpu=power7) these functions will expand into a different
*  (slightly longer) instruction sequence which is functionally
*  equivalent to the instruction sequence generated for (-mcpu=power8).
*
*  For older processors (power7 and earlier) and under some
*  circumstances instructions generated for this "combined form"
*  may perform better than the "split form" equivalent from the
*  second example.
*  Here the compiler may not recognize all the common subexpressions,
*  as the "split forms" are expanded before optimization.
*
*  \section mainpage_sub2 Background on the evolution  of <altivec.h>
*
*  The original
*  <a href="https://www.nxp.com/docs/en/reference-manual/ALTIVECPIM.pdf">
*  AltiVec (TM) Technology Programming Interface Manual</a>
*  defined the minimal vector extensions to the application binary
*  interface (ABI), new keywords (vector, pixel, bool) for defining
*  new vector types, and new operators (built-in functions).
*
*  - generic AltiVec operations, like vec_add()
*  - specific AltiVec operations (instructions, like vec_addubm())
*  - predicates computed from a AltiVec operation like vec_all_eq()
*
*  A generic operation generates specific instructions based on
*  the types of the actual parameters.
*  So a generic vec_add operation, with vector char parameters,
*  will generate the (specific) vector add unsigned byte modulo
*  (vaddubm) instruction.
*  Predicates are used within if statement conditional clauses
*  to access the condition code from vector operations that set
*  Condition Register 6 (vector SIMD compares and Decimal Integer
*  arithmetic and format conversions).
*
*  The PIM defined a set of compiler built-ins for vector instructions
*  (see section "4.4 Generic and Specific AltiVec Operations")
*  that compilers should support.
*  The document suggests that any required typedefs and supporting
*  macro definitions be collected into an include file named
*  <altivec.h>.
*
*  The built-ins defined by the PIM closely match the vector
*  instructions of the underlying PowerISA.
*  For example: vec_mul, vec_mule / vec_mulo, and vec_muleub / vec_muloub.
*  - vec_mul is defined for float and double and will (usually)
*  generate a single instruction for the type. This is a simpler case
*  as floating point operations usually stay in their lanes
*  (result elements are the same size as the input operand elements).
*  - vec_mule / vec_mulo (multiply even / odd) are defined for
*  integer multiply as integer products require twice as many bits
*  as the inputs (the results don't stay in their lane).
*
*  The RISC philosophy resists and POWER Architecture avoids
*  instructions that write to more than one register.
*  So the hardware and PowerISA vector integer multiply generate
*  even and odd product results (from even and odd input elements)
*  from two instructions executing separately.
*  The PIM defines and compiler supports
*  these operations as overloaded built-ins
*  and selects the specific instructions based on the operand
*  (char or short) type.
*
*  As the PowerISA evolved adding new vector (VMX) instructions,
*  new facilities (Vector Scalar Extended (VSX)),
*  and specialized vector categories (little endian, AES, SHA2, RAID),
*  some of these new operators were added to <altivec.h>.
*  This included some new specific and generic operations and
*  additional vector element types (long (64-bit) int, __int128,
*  double and quad precision (__Float128) float).
*  This support was <I>staged</I> across multiple compiler releases
*  in response to perceived need and stake-holder requests.
*
*  The result was a patchwork of <altivec.h> built-ins support versus
*  new instructions in the PowerISA and shipped hardware.
*  The original Altivec (VMX) provided Vector Multiply (Even / Odd)
*  operations for byte (char) and halfword (short) integers.
*  Vector Multiply Even / Odd Word (int) instructions were not
*  introduced until PowerISA V2.07 (POWER8) under the generic
*  built-ins vec_mule, vec_mulo.
*  PowerISA 2.07 also introduced Vector Multiply Word Modulo under the
*  generic built-in vec_mul. Both where first available in GCC 8.
*  Specific built-in forms (vec_vmuleuw, vec_vmulouw,
*  vec_vmuluwm) where not provided.
*  PowerISA V3.0 (POWER9) added Multiply-Sum Unsigned Doubleword Modulo
*  but neither generic (vec_msum) or specific (vec_msumudm) forms
*  have been provided (so far as of GCC 9).
*
*  However the original PIM documents were primarily focused on embedded
*  processors and were not updated to include the vector extensions
*  implemented by the server processors.
*  So any documentation for new vector operations were relegated to
*  the various compilers.  This was a haphazard process and some
*  divergence in operation naming did occur between compilers.
*
*  In the run up to the POWER8 launch and the OpenPOWER initiative it
*  was recognized that switching to Little Endian would require and
*  new and well documented Application Binary Interface (<B>ABI</B>).
*  It was also recognized that new <altivec.h> extensions needed to be
*  documented in a common place so the various compilers could
*  implement a common vector built-in API. So ...
*
*  \subsection mainpage_sub2_1 The ABI is evolving
*
*  The
*  <a href="https://openpowerfoundation.org/?resource_lib=64-bit-elf-v2-abi-specification-power-architecture">
*  OpenPOWER ELF V2 application binary interface (ABI)</a>:
*  Chapter 6. <B>Vector Programming Interfaces</B> and <B>Appendix A.
*  Predefined Functions for Vector Programming</B> document the
*  current and proposed vector built-ins we expect all C/C++
*  compilers to implement for the PowerISA.
*
*  The ABI defined generic operations as overloaded built-in functions.
*  Here the ABI suggests a specific PowerISA
*  implementation based on the operand (vector element) types.
*  The ABI also defines the (big/little) endian behavior and the ABI
*  may suggests different instructions based on the endianness
*  of the target.
*
*  This is an important point as the vector element numbering
*  changes between big and little endian, and so does the meaning of
*  even and odd. Both affect what the compiler supports and the
*  instruction sequence generated.
*  - <B>vec_mule</B> and <B>vec_mulo</B> (multiply even / odd
*  are examples of generic built-ins  defined by the ABI.
*  One would assume these built-ins will generate the matching
*  instruction based only on the input vector type,
*  however the GCC compiler will adjust the generated
*  instruction based on the target endianness
*  (reversing even / odd for little endian).
*  - Similarly for the merge (even/odd high/low) operations.
*  For little endian the compiler reverses even/odd (high/low) and
*  swaps operands as well.
*  - See <B>Table 6.1. Endian-Sensitive Operations</B> for details.
*
*  The many existing specific built-ins (where the name includes
*  explicit type and signed/unsigned notation) are included in the
*  ABI but listed as deprecated.
*  Specifically the Appendix <B>A.6. Deprecated Compatibility Functions</B>
*  and <B>Table A.8. Functions Provided for Compatibility</B>.
*
*  This reflects an explicit decision by the ABI and compiler
*  maintainers that a generic only interface would be smaller/easier
*  to implement and document as the PowewrISA evolves.
*
*  Certainly the addition of VSX to POWER7 and the many vector
*  extensions added to POWER8 and POWER9 added hundreds of vector
*  instructions. Many of these new instructions needed build-ins to:
*  - Enable early library exploitations.
*  For example new floating point element sizes (double and Float128).
*  - Support specialized operations not generally supported in the
*  language.  For example detecting Not-a-Number and Infinities without
*  triggering exceptions. These are needed in the POSIX library
*  implementation.
*  - Supporting wider integer element sizes can result in large
*  multiples of specific built-ins if you include variants for:
*    - signed and unsigned
*    - saturated
*    - even, odd, modulo, write-carry, and extend
*    - high and low
*    - and additional associated  merge, pack, unpack, splat, operations
*
*  So implementing new instructions as generic built-ins first, and
*  delaying the specific built-in permutations,
*  is a wonderful simplification. This moves naturally from tactical
*  to strategy to plan quickly.
*  Dropping the specific built-ins for new instructions and deprecating
*  the existing specific built-ins saves a lot of work.
*
*  As the ABI places more emphasis on generic built-in operations,
*  we are seeing more cases
*  where the compiler generates multiple instruction sequences.
*  The first example was vec_abs (vector absolute value) from the
*  original Altivec PIM. There was no vector absolute instruction for
*  any of the supported types (including vector float at the time).
*  But this  could be implemented in a 3 instruction sequence.
*  This generic operation was extended to vector double for
*  VSX (PowerISA 2.06) which introduced hardware instructions for
*  absolute value of single and double precision vectors. But
*  vec_abs remains a multiple instruction sequence for integer
*  elements.
*
*  Another example is vec_mul. POWER8 (PowerISA 2.07) introduced
*  Vector Multiply Unsigned Word Modulo (vmuluwm). This was included
*  in the ISA as it simplified vectorizing C language (int) loops.
*  This also allowed a single instruction implementation for vec_mul
*  for vector (signed/unsigned) int.
*  The PowerISA does not provide direct vector multiply modulo
*  instructions for char, short, or long. Again this requires a
*  multiple-instruction sequence to implement.
*
*  \subsection mainpage_sub2_2 The current <altivec.h> is a mixture
*
*  The current vector ABI implementation in the compiler and
*  <altivec.h> is mixture of old and new.
*  - Many new instruction (since PowerISA 2.06) are supported
*  only under existing built-ins (with new element types; vec_mul,
*  vec_mule, vec_mulo).
*  Or as newly defined generic built-ins (vec_eqv. vec_nand, vec_orc).
*   - Specific types/element sizes under these generic built-ins may be
*   marked <I>phased in</I>.
*  - Some new instructions are supported with both generic (vec_popcnt)
*  and specific built-ins (vec_vpopcntb, vec_vpopcntd, vec_vpopcnth,
*  vec_vpopcntw).
*  - Other new instructions are only supported with specific built-ins
*  (vec_vaddcuq, vec_vaddecuq, vec_vaddeuqm, vec_vsubcuq, vec_vsubecuq,
*  vec_vsubeuqm). To be fair only the quadword element supports the
*  write-carry and extend variants.
*  - Endian sensitivity may be applied in surprising ways.
*   - <B>vec_muleub</B> and <B>vec_muloub</B> (multiply even / odd
*  unsigned byte) are examples of non-overloaded built-ins provided by
*  the GCC compiler but not defined in the ABI.
*  One would assume these built-ins will generate the matching
*  instruction, however the GCC compiler will adjust the generated
*  instruction based on the target endianness
*  (even / odd is reversed for little endian).
*   - <B>vec_sld</B>, <B>vec_sldw</B>, <B>vec_sll</B>, and
*   <B>vec_slo</B> (vector shift left) are <B>not</B> endian sensitive.
*   Historically, these built-ins are often used to shift by amounts
*   not a multiple of the element size, across types.
*  - A number of built-ins are defined in the ABI and marked
*  (all or in part) as <I>phased in</I>.
*  This implies that compilers <B>shall</B> implement
*  these built-ins (eventually) in <altivec.h>.
*  However the specific compiler version you are using many not have
*  implemented it yet.
*
*  \subsection mainpage_sub2_3 Best practices
*
*  This is a small sample of the complexity we encounter programming
*  at this low level (vector intrinsic) API.
*  This is also an opportunity for a project like the Power Vector
*  Library (PVECLIB) to smooth off the rough edges and simplify
*  software development for the OpenPOWER ecosystem.
*
*  If the generic vector built-in operation you need:
*  - is defined in the ABI, and
*  - defined in the PowerISA across the processor versions you need
*    to support, and
*  - defined in <altivec.h> for the compilers and compiler versions
*    you expect to use, and
*  - implemented for the vector types/element sizes you need for the
*    compilers and compiler versions you expect to use.
*
*  Then use the generic vector built-in from <altivec.h> in your
*  application/library.
*
*  Otherwise if the specific vector built-in operation you need is
*  defined in <altivec.h>:
*  - For the vector types/element sizes you need, and
*  - defined in the PowerISA across the processor versions you need
*  to support, and
*  - implemented for the compilers and compiler versions you expect
*  to use.
*
*  Then use the specific vector built-in from <altivec.h> in your
*  application/library.
*
*  Otherwise if the vector operation you need is defined in PVECLIB.
*  - For the vector types/element sizes you need.
*
*  Then use the vector operation from PVECLIB in your
*  application/library.
*
*  Otherwise
*  - Check on https://github.com/open-power-sdk/pveclib and see if
*  there is newer version of PVECLIB.
*  - Open an issue on https://github.com/open-power-sdk/pveclib/issues
*  for the operation you would like to see.
*  - Look at source for PVECLIB for examples similar to what you are
*  trying to do.
*
* \section main_libary_issues_0_0 Putting the Library into PVECLIB
*
* Until recently (as of v1.0.3) PVECLIB operations were
* <B>static inline</B> only. This was reasonable as most operations
* were small (one to a few vector instructions).
* This offered the compiler opportunity for:
* - Better register allocation.
* - Identifying common subexpressions and factoring them across
* operation instances.
* - Better instruction scheduling across operations.
*
* Even then, a few operations
* (quadword multiply, BCD multiply, BCD <-> binary conversions,
* and some POWER8/7 implementations of POWER9 instructions)
* were getting uncomfortably large (10s of instructions).
* But it was the multiple quadword precision operations that
* forced the issue as they can run to 100s and sometimes 1000s of
* instructions.
* So, we need to build some functions from pveclib into a
* static archive and/or a dynamic library (DSO).
*
* \subsection main_libary_issues_0_0_0 Building Multi-target Libraries
*
* Building libraries of compiled binaries is not that difficult.
* The challenge is effectively supporting multiple processor
* (POWER7/8/9) targets, as many PVECLIB operations have different
* implementations for each target.
* This is especially evident on the multiply integer word, doubleword,
* and quadword operations (see; vec_muludq(), vec_mulhuq(), vec_mulluq(),
* vec_vmuleud(), vec_vmuloud(), vec_msumudm(), vec_muleuw(),
* vec_mulouw()).
*
* This is dictated by both changes in the PowerISA and in
* the micro-architecture as it evolved across processor generations.
* So an implementation to run on a POWER7 is necessarily restricted to
* the instructions of PowerISA 2.06.
* But if we are running on a POWER9, leveraging new instructions from
* PowerISA 3.0 can yield better performance than the POWER7
* compatible implementation. When we are dealing with larger operations
* (10s and 100s of instructions) the compiler can schedule instruction
* sequences based on the platform (-mtune=) for better performance.
*
* So, we need to deliver multiple implementations for some operations
* and we need to provide mechanisms to select a specific target
* implementation statically at compile/build or dynamically at
* runtime. First we need to compile multiple version of these
* operations, as unique functions, each with a different effective
* compile target (-mcpu= options).
*
* Obviously, creating multiple source files implementing the same large
* operation, each supporting a different specific target platform,
* is a possibility.
* However, this could cause maintenance problems where changes to a
* operation must be coordinated across multiple source files.
* This is also inconsistent with the current PVECLIB coding style
* where a file contains an operation's complete implementation,
* including documentation and target specific implementation variants.
*
* The current PVECLIB implementation makes extensive use of C
* Preprocessor (<B>CPP</B>) conditional source code.
* These includes testing for; compiler version,
* target endianness, and current target processor, then selects the
* appropriate source code snippet (\ref mainpage_sub_1_2).
* This was intended to simplify the application/library developer's
* life were they could use the PVECLIB API and not worry about
* these details.
*
* So far, this works as intended (single vector source for multiple
* PowerISA VMX/VSX targets) when the entire application is compiled
* for a single target.
* However, this dependence on CPP conditionals is mixed blessing then
* the application needs to support multiple platforms in a single package.
*
* \subsubsection main_libary_issues_0_0_0_0 The mechanisms available
* The compiler and ABI offer options that at first glance
* seem to allow multiple target specific binaries from a single source.
* Besides the compiler's command level target options a number of
* source level mechanisms to change the target.
* These include:
* - __ attribute __ (target ("cpu=power8"))
* - __ attribute __ (target_clones ("cpu=power9,default"))
* - \#pragma GCC target ("cpu=power8")
* - multiple compiles with different command line options (i.e. -mcpu=)
*
* The target and target_clones attributes are function attributes
* (apply to single function).
* The target attribute overrides the command line -mcpu= option.
* However it is not clear which version of GCC added explicit support
* for (target ("cpu="). This was not explicitly documented until GCC 5.
* The target_clones attribute will cause GCC will create two (or more)
* function clones, one (or more) compiled with the specified cpu=
* target and another with the default (or command line -mcpu=) target.
* It also creates a resolver function that dynamically selects a clone
* implementation suitable for current platform architecture. This
* PowerPC specific variant was not explicitly documented until GCC 8.
*
* There are a few issues with function attributes:
* - The Doxygen preprocessor can not parse function attributes without
* a lot of intervention.
* - The availability of these attributes seems to be limited to the
* latest GCC compilers.
*
* \note The Clang/LLVM compilers don't provide equivalents to attribute
* (target) or \#pragma target.
*
* But there is a deeper problem related to the usage of CPP conditionals.
* Many PVECLIB operation implementations depend on GCC/compiler
* predefined macros including:
* - __ GNUC __
* - __ GNUC_MINOR __
* - __ BYTE_ORDER __
* - __ ORDER_LITTLE_ENDIAN __
* - __ ORDER_BIG_ENDIAN __
*
* PVECLIB also depends on many system-specific predefined macros including:
* - __ ALTIVEC __
* - __ VSX __
* - __ FLOAT128 __
* - _ARCH_PWR9
* - _ARCH_PWR8
* - _ARCH_PWR7
*
* PVECLIB also depends on the <altivec.h> include file which provides
* the mapping between the ABI defined intrinsics and compiler defined
* built-ins. In some places PVECLIB conditionally tests if specific
* built-in is defined and substitutes an in-line assembler
* implementation if not.
* Altivec.h also depends on system-specific predefined macros to
* enable/disable blocks of intrinsic built-ins based on PowerISA level
* of the compile target.
*
* \subsubsection main_libary_issues_0_0_0_1 Some things just do not work
* This issue is the compiler (GCC at least) only expands the
* compiler and system-specific predefined macros once per source file.
* The preprocessed source does not change due to embedded function
* attributes that change the target.
* So the following does not work as expected.
*
* \code
#include <altivec.h>
#include <pveclib/vec_int128_ppc.h>
#include <pveclib/vec_int512_ppc.h>

// Defined in vec_int512_ppc.h but included here for clarity.
static inline __VEC_U_256
vec_mul128x128_inline (vui128_t a, vui128_t b)
{
  __VEC_U_256 result;
  // vec_muludq is defined in vec_int128_ppc.h
  result.vx0 = vec_muludq (&result.vx1, a, b);
  return result;
}

__VEC_U_256 __attribute__(target ("cpu=power7"))
vec_mul128x128_PWR7 (vui128_t m1l, vui128_t m2l)
{
  return vec_mul128x128_inline (m1l, m2l);
}

__VEC_U_256 __attribute__(target ("cpu=power8"))
vec_mul128x128_PWR8 (vui128_t m1l, vui128_t m2l)
{
  return vec_mul128x128_inline (m1l, m2l);
}

__VEC_U_256 __attribute__(target ("cpu=power9"))
vec_mul128x128_PWR9 (vui128_t m1l, vui128_t m2l)
{
  return vec_mul128x128_inline (m1l, m2l);
}
* \endcode
*
* For example if we assume that the compiler default is (or the
* command line specifies) -mcpu=power8 the compiler will use this to
* generate the system-specific predefined macros.
* This is done before the first include file is processed.
* In this case <altivec.h>, vec_int128_ppc.h, and vec_int512_ppc.h
* source will be expanded for power8 (PowerISA-2.07).
* The result is the vec_muludq and vec_muludq inline source
* implementations will be the power8 specific version.
*
* This will all be established before the compiler starts to parse
* and generate code for vec_mul128x128_PWR7.
* This compile is likely to fail because we are
* trying to compile code containing power8 instructions for a
* -mcpu=power7 target.
*
* The compilation of vec_mul128x128_PWR8 should work as we are
* compiling power8 code with a -mcpu=power8 target.
* The compilation of vec_mul128x128_PWR9 will compile without error
* but will generate essentially the same code as vec_mul128x128_PWR8.
* The target("cpu=power9") allows that compiler to use power9
* instructions but the expanded source coded from vec_muludq and
* vec_mul128x128_inline will not contain any power9 intrinsic
* built-ins.
*
* \note The GCC attribute <B>target_clone</B> has the same issue.
*
* Pragma GCC target has a similar issue if you try to change the
* target multiple times within the same source file.
*
* \code
#include <altivec.h>
#include <pveclib/vec_int128_ppc.h>
#include <pveclib/vec_int512_ppc.h>

// Defined in vec_int512_ppc.h but included here for clarity.
static inline __VEC_U_256
vec_mul128x128_inline (vui128_t a, vui128_t b)
{
  __VEC_U_256 result;
  // vec_muludq is defined in vec_int128_ppc.h
  result.vx0 = vec_muludq (&result.vx1, a, b);
  return result;
}

#pragma GCC push_options
#pragma GCC target ("cpu=power7")

__VEC_U_256
vec_mul128x128_PWR7 (vui128_t m1l, vui128_t m2l)
{
  return vec_mul128x128_inline (m1l, m2l);
}

#pragma GCC pop_options
#pragma GCC push_options
#pragma GCC target ("cpu=power8")

__VEC_U_256
vec_mul128x128_PWR8 (vui128_t m1l, vui128_t m2l)
{
  return vec_mul128x128_inline (m1l, m2l);
}

#pragma GCC pop_options
#pragma GCC push_options
#pragma GCC target ("cpu=power9")

__VEC_U_256
vec_mul128x128_PWR9 (vui128_t m1l, vui128_t m2l)
{
  return vec_mul128x128_inline (m1l, m2l);
}
* \endcode
* This has the same issues as the target attribute example above.
* However you can use \#pragma GCC target if;
* - it proceeds the first \#include in the source file.
* - there is only one target \#pragma in the file.
*
* For example:
* \code
#pragma GCC target ("cpu=power9")
#include <altivec.h>
#include <pveclib/vec_int128_ppc.h>
#include <pveclib/vec_int512_ppc.h>

// vec_mul128x128_inline is defined in vec_int512_ppc.h
__VEC_U_256
vec_mul128x128_PWR9 (vui128_t m1l, vui128_t m2l)
{
  return vec_mul128x128_inline (m1l, m2l);
}
* \endcode
* In this case the cpu=power9 option is applied before the compiler
* reads the first include file and initializes the system-specific
* predefined macros. So the CPP source expansion reflects the power9
* target.
*
* \note So far the techniques described only work reliably for C/C++
* codes, compiled with GCC, that don't use <altivec.h> intrinsics or
* use CPP conditionals.
*
* The implication is we need a build system that allows
* source files to be compiled multiple times, each with different
* compile targets.
*
* \subsubsection main_libary_issues_0_0_0_2 Some tricks to build targeted runtime objects.
* We need a unique compiled object implementation for each target processor.
* We still prefer a single file implementation for each function
* to improve maintenance. So we need a way to separate setting the platform
* target from the implementation source. Also we need to provide a
* unique external symbol for each target specific implementation of a function.
*
* This can be handled with a simple macro to append a suffix based on
* system-specific predefined macro settings.
*
* \code
#ifdef _ARCH_PWR9
#define __VEC_PWR_IMP(FNAME) FNAME ## _PWR9
#else
#ifdef _ARCH_PWR8
#define __VEC_PWR_IMP(FNAME) FNAME ## _PWR8
#else
#define __VEC_PWR_IMP(FNAME) FNAME ## _PWR7
#endif
#endif
* \endcode
* Then use __VEC_PWR_IMP() as function name wrapper in the implementation source file.
*
*
* \code
 //
 //  \file  vec_int512_runtime.c
 //

#include <altivec.h>
#include <pveclib/vec_int128_ppc.h>
#include <pveclib/vec_int512_ppc.h>

// vec_mul128x128_inline is defined in vec_int512_ppc.h
__VEC_U_256
__VEC_PWR_IMP (vec_mul128x128) (vui128_t m1l, vui128_t m2l)
{
  return vec_mul128x128_inline (m1l, m2l);
}
* \endcode
* Then the use __VEC_PWR_IMP() function wrapper for any calling function
* that is linked statically to that library function.
* \code
__VEC_U_1024
__VEC_PWR_IMP (vec_mul512x512) (__VEC_U_512 m1, __VEC_U_512 m2)
{
  __VEC_U_1024 result;
  __VEC_U_512x1 mp3, mp2, mp1, mp0;

  mp0.x640 = __VEC_PWR_IMP(vec_mul512x128) (m1, m2.vx0);
  result.vx0 = mp0.x3.v1x128;
  mp1.x640 = __VEC_PWR_IMP(vec_madd512x128a512) (m1, m2.vx1, mp0.x3.v0x512);
  result.vx1 = mp1.x3.v1x128;
  mp2.x640 = __VEC_PWR_IMP(vec_madd512x128a512) (m1, m2.vx2, mp1.x3.v0x512);
  result.vx2 = mp2.x3.v1x128;
  mp3.x640 = __VEC_PWR_IMP(vec_madd512x128a512) (m1, m2.vx3, mp2.x3.v0x512);
  result.vx3 = mp3.x3.v1x128;
  result.vx4 = mp3.x3.v0x512.vx0;
  result.vx5 = mp3.x3.v0x512.vx1;
  result.vx6 = mp3.x3.v0x512.vx2;
  result.vx7 = mp3.x3.v0x512.vx3;
  return result;
}
* \endcode
*
* The <B>runtime</B> library implementation is in a separate file from
* the <B>inline</B> implementation. The vec_int512_ppc.h file contains:
* - static inline implementations and associated doxygen interface
* descriptions.  These are still small enough to used directly by
* application codes and as building blocks for larger library
* implementations.
* - extern function declarations and associated doxygen interface
* descriptions. These names are for the dynamic shared object
* (<B>DSO</B>) function implementations. The functions are not
* qualified with inline or target suffixes. The expectation is the
* dynamic linker mechanism with bind to the appropriate implementation.
* - extern function declarations qualified with a target suffix.
* These names are for the statically linked (<B>archive</B>)
* function implementations.
* The suffix is applied by the __VEC_PWR_IMP() macro for the current
* (default) target processor. These have no doxygen descriptions
* as using the __VEC_PWR_IMP() macro interferes with the doxygen
* scanner. But the interface is the same as the unqualified extern for
* the DSO implementation of the same name.
*
* The runtime source file (for example vec_int512_runtime.c) contains
* the common implementations for all the target qualified
* static interfaces.
* - Again the function names are target qualified via
* the __VEC_PWR_IMP() macro.
* - The runtime implementation can use any of
* the PVECLIB inline operations
* (see: vec_mul128x128() and vec_mul256x256())
* as well as other function implementations from the same file
* (see: vec_mul512x512() and vec_mul2048x2048()).
* - At the -O3 optimization level the compiler will attempt to inline
* functions referenced from the same file. Compiler heuristics will
* limit this based on estimates for the final generated object size.
* GCC also supports the function __ attribute __ ((flatten)) which
* overrides the in-lining size heuristics.
* - These implementations can also use target specific CPP
* conditional codes to manually tweak code optimization or generated
* code size for specific targets.
*
* This simple strategy allows the collection of the larger function
* implementations into a single source file and build object files
* for multiple platform targets. For example collect all the multiple
* precision quadword implementations into a source file named
* <B>vec_int512_runtime.c</B>.
*
* \subsection main_libary_issues_0_0_1 Building static runtime libraries
* This source file can be compiled multiple times for different
* platform targets. The resulting object files have unique function
* symbols due to the platform specific suffix provided by the
* __VEC_PWR_IMP() macro.
* There are a number of build strategies for this.
*
* For example, create a small source file named
* <B>vec_runtime_PWR8.c</B> that starts with the
* target pragma and includes the multi-platform source file.
* \code
//  \file  vec_runtime_PWR8.c

#pragma GCC target ("cpu=power8")

#include "vec_int512_runtime.c"
* \endcode
* Similarly for <B>vec_runtime_PWR7.c</B>, <B>vec_runtime_PWR9.c</B>
* with appropriate changes for "cpu='.
* Additional runtime source files can be included as needed.
* Other multiple precision functions supporting BCD and BCD <-> binary
* conversions are likely candidates.
*
* \note Current Clang compilers silently ignore "\#pragme GCC target".
* This causes all such targeted runtimes to revert to the compiler
* default target or configure CFLAGS "-mcpu=". In this case the
* __VEC_PWR_IMP()
* macro will apply the same suffix to all functions across the targeted
* runtime builds. As a result linking these targeted runtime objects
* into the DSO will fail with duplicate symbols.
*
* Projects using autotools (like PVECLIB) can use Makefile.am rules to
* associate rumtime source files with a library. For example:
* \code
libpvec_la_SOURCES = vec_runtime_PWR9.c \
	vec_runtime_PWR8.c \
	vec_runtime_PWR7.c
* \endcode
* If compiling with GCC this is sufficient for automake to generate
* Makefiles to compile each of the runtime sources and combine them
* into a single static archive named libpvec.a. However it is not that
* simple, especially if the build uses a different compiler.
*
* We would like to use Makefile.am rules to specify different -mcpu=
* compile options. This eliminates the \#pragma GCC target and
* simplifies the platform source files too something like:
* \code
 //
 //  \file  vec_runtime_PWR8.c
 //

#include "vec_int512_runtime.c"
 * \endcode
 * This requires splitting the target specific runtimes into distinct
 * automake libraries.
 * \code
libpveccommon_la_SOURCES = tipowof10.c decpowof2.c
libpvecPWR9_la_SOURCES = vec_runtime_PWR9.c
libpvecPWR8_la_SOURCES = vec_runtime_PWR8.c
libpvecPWR7_la_SOURCES = vec_runtime_PWR7.c
* \endcode
* Then add the -mcpu compile option to runtime library CFLAGS
* \code
libpvecPWR9_la_CFLAGS = -mcpu=power9
libpvecPWR8_la_CFLAGS = -mcpu=power8
libpvecPWR7_la_CFLAGS = -mcpu=power7
* \endcode
* Then use additional automake rules to combine these targeted runtimes
* into a single static archive library.
* \code
libpvecstatic_la_LIBADD = libpveccommon.la
libpvecstatic_la_LIBADD += libpvecPWR9.la
libpvecstatic_la_LIBADD += libpvecPWR8.la
libpvecstatic_la_LIBADD += libpvecPWR7.la
* \endcode
*
* However this does not work if the user (build configure) specifies
* flag variables (i.e. CFLAGS) containing -mcpu= options internal use
* of target options.
*
* \note Automake/libtool will always apply the user CFLAGS  after any
* AM_CFLAGS or yourlib_la_CFLAGS (See:
* <a href="https://www.gnu.org/software/automake/manual/html_node/Flag-Variables-Ordering.html">
* Automake documentation: Flag Variables Ordering</a>)
* and the last -mcpu option always wins.
* This has the same affect as the compiler ignoring the
* \#pragma GCC target options described above.
*
* \subsubsection main_libary_issues_0_0_0_4 A deeper look at library Makefiles
* This requires a deeper dive into the black arts of automake and
* libtools.
* In this case the libtool macro LTCOMPILE expands the various flag
* variables in a specific order (with $CFLAGS last) for all --tag=CC
* --mode=compile commands. In this case we need to either:
* - locally edit CFLAGS to eliminates any -mcpu= (or -O) options so
* that our internal build targets are applied.
* - provide our own alternative to the LTCOMPILE macro and use our own
* explicit make rules. (See ./pveclib/src/Makefile.am for examples.)
*
* So lets take a look at LTCOMPILE:
* \code
LTCOMPILE = $(LIBTOOL) $(AM_V_lt) --tag=CC $(AM_LIBTOOLFLAGS) \
	$(LIBTOOLFLAGS) --mode=compile $(CC) $(DEFS) \
	$(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) \
	$(AM_CFLAGS) $(CFLAGS)
* \endcode
* \note "$(CFLAGS)" is always applied after all other <I>FLAGS</I>.
*
* The generated Makefile.in includes rules that depend on LTCOMPILE.
* For example the general rule for compile .c source to .lo objects.
* \code
.c.lo:
@am__fastdepCC_TRUE@	$(AM_V_CC)depbase=`echo $@ | sed 's|[^/]*$$|$(DEPDIR)/&|;s|\.lo$$||'`;\
@am__fastdepCC_TRUE@	$(LTCOMPILE) -MT $@ -MD -MP -MF $$depbase.Tpo -c -o $@ $< &&\
@am__fastdepCC_TRUE@	$(am__mv) $$depbase.Tpo $$depbase.Plo
@AMDEP_TRUE@@am__fastdepCC_FALSE@	$(AM_V_CC)source='$<' object='$@' libtool=yes @AMDEPBACKSLASH@
@AMDEP_TRUE@@am__fastdepCC_FALSE@	DEPDIR=$(DEPDIR) $(CCDEPMODE) $(depcomp) @AMDEPBACKSLASH@
@am__fastdepCC_FALSE@	$(AM_V_CC@am__nodep@)$(LTCOMPILE) -c -o $@ $<
* \endcode
* Or the more specific rule to compile the vec_runtime_PWR9.c for the
* \-mcpu=power9 target:
* \code
 libpvecPWR9_la-vec_runtime_PWR9.lo: vec_runtime_PWR9.c
@am__fastdepCC_TRUE@	$(AM_V_CC)$(LIBTOOL) $(AM_V_lt) --tag=CC $(AM_LIBTOOLFLAGS) \
 $(LIBTOOLFLAGS) --mode=compile $(CC) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) \
 $(AM_CPPFLAGS) $(CPPFLAGS) $(libpvecPWR9_la_CFLAGS) $(CFLAGS) \
 -MT libpvecPWR9_la-vec_runtime_PWR9.lo -MD -MP -MF \
 $(DEPDIR)/libpvecPWR9_la-vec_runtime_PWR9.Tpo -c -o libpvecPWR9_la-vec_runtime_PWR9.lo \
 `test -f 'vec_runtime_PWR9.c' || echo '$(srcdir)/'`vec_runtime_PWR9.c
@am__fastdepCC_TRUE@	$(AM_V_at)$(am__mv) $(DEPDIR)/libpvecPWR9_la-vec_runtime_PWR9.Tpo \
 $(DEPDIR)/libpvecPWR9_la-vec_runtime_PWR9.Plo
@AMDEP_TRUE@@am__fastdepCC_FALSE@	$(AM_V_CC)source='vec_runtime_PWR9.c' \
 object='libpvecPWR9_la-vec_runtime_PWR9.lo' libtool=yes @AMDEPBACKSLASH@
@AMDEP_TRUE@@am__fastdepCC_FALSE@	DEPDIR=$(DEPDIR) $(CCDEPMODE) \
 $(depcomp) @AMDEPBACKSLASH@
@am__fastdepCC_FALSE@	$(AM_V_CC@am__nodep@)$(LIBTOOL) $(AM_V_lt) --tag=CC \
 $(AM_LIBTOOLFLAGS) $(LIBTOOLFLAGS) --mode=compile $(CC) $(DEFS) $(DEFAULT_INCLUDES) \
 $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(libpvecPWR9_la_CFLAGS) $(CFLAGS) -c \
 -o libpvecPWR9_la-vec_runtime_PWR9.lo `test -f 'vec_runtime_PWR9.c' \
 || echo '$(srcdir)/'`vec_runtime_PWR9.c
* \endcode
* Which is eventually generated into the Makefile as:
* \code
libpvecPWR9_la-vec_runtime_PWR9.lo: vec_runtime_PWR9.c
	$(AM_V_CC)$(LIBTOOL) $(AM_V_lt) --tag=CC $(AM_LIBTOOLFLAGS) $(LIBTOOLFLAGS) \
	--mode=compile $(CC) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) \
	$(CPPFLAGS) $(libpvecPWR9_la_CFLAGS) $(CFLAGS) -MT libpvecPWR9_la-vec_runtime_PWR9.lo \
	-MD -MP -MF $(DEPDIR)/libpvecPWR9_la-vec_runtime_PWR9.Tpo -c -o \
	libpvecPWR9_la-vec_runtime_PWR9.lo `test -f 'vec_runtime_PWR9.c' || \
	echo '$(srcdir)/'`vec_runtime_PWR9.c
	$(AM_V_at)$(am__mv) $(DEPDIR)/libpvecPWR9_la-vec_runtime_PWR9.Tpo \
	$(DEPDIR)/libpvecPWR9_la-vec_runtime_PWR9.Plo
#	$(AM_V_CC)source='vec_runtime_PWR9.c' object='libpvecPWR9_la-vec_runtime_PWR9.lo' \
#       libtool=yes DEPDIR=$(DEPDIR) $(CCDEPMODE) $(depcomp) \
#	$(AM_V_CC_no)$(LIBTOOL) $(AM_V_lt) --tag=CC $(AM_LIBTOOLFLAGS) $(LIBTOOLFLAGS) \
#       --mode=compile $(CC) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) \
#       $(CPPFLAGS) $(libpvecPWR9_la_CFLAGS) $(CFLAGS) -c -o libpvecPWR9_la-vec_runtime_PWR9.lo \
#       `test -f 'vec_runtime_PWR9.c' || echo '$(srcdir)/'`vec_runtime_PWR9.c
* \endcode
* Somehow in the internal struggle for the dark soul of
* automake/libtools,
* the <I>\@am__fastdepCC_TRUE\@</I> conditional wins out over
* <I>\@AMDEP_TRUE\@\@am__fastdepCC_FALSE\@</I> ,
* and the alternate rule was commented out as the
* Makefile was generated.
*
* However this still leaves a problem. While we see that
* \$(libpvecPWR9_la_CFLAGS) applies the "-mcpu=power9" target option,
* it is immediately followed by \$(CFLAGS). And it CFLAGS contains
* any "-mcpu=" option the last "-mcpu=" option always wins.
* The result will a broken library archives with duplicate symbols.
*
* \note The techniques described work reliably for most codes and
* compilers as long as the user does not override target (-mcpu=) with
* CFLAGS on configure.
*
* \subsubsection main_libary_issues_0_0_0_5 Adding our own Makefile magic
*
* \todo Is there a way for automake to compile vec_int512_runtime.c
* with -mcpu=power9 and -o vec_runtime_PWR9.o?
* And similarly for PWR7/PWR8.
*
* Once we get a glimpse of the underlying automake/libtool rule
* generation we have a template for how to solve this problem.
* However while we need to workaround some automake/libtool constraints
* we also want fit into overall flow.
*
* First we need an alternative to <B>LTCOMPILE</B> where we can bypass
* user provided <B>CFLAGS</B>. For example:
* \code
PVECCOMPILE = $(LIBTOOL) $(AM_V_lt) --tag=CC $(AM_LIBTOOLFLAGS) \
	$(LIBTOOLFLAGS) --mode=compile $(CC) $(DEFS) \
	$(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) \
	$(AM_CFLAGS)
* \endcode
* In this variant (<B>PVECCOMPILE</B>) we simply leave \$(CFLAGS) off
* the end of the macro.
*
* Now we can use the generated rule above as an example to provide our
* own Makefile rules. These rules will be passed directly to the
* generated Makefile. For example:
* \code{.m4}
vec_staticrt_PWR9.lo: vec_runtime_PWR9.c $(pveclibinclude_HEADERS)
if am__fastdepCC
	$(PVECCOMPILE) $(PVECLIB_POWER9_CFLAGS) -MT $@ -MD -MP -MF \
	$(DEPDIR)/$*.Tpo -c -o $@ $(srcdir)/vec_runtime_PWR9.c
	mv -f $(DEPDIR)/$*.Tpo $(DEPDIR)/$*.Plo
else
if AMDEP
	source='vec_runtime_PWR9.c' object='$@' libtool=yes @AMDEPBACKSLASH@
	DEPDIR=$(DEPDIR) $(CCDEPMODE) $(depcomp) @AMDEPBACKSLASH@
endif
	$(PVECCOMPILE) $(PVECLIB_POWER9_CFLAGS) -c -o $@ $(srcdir)/vec_runtime_PWR9.c
endif
* \endcode
* We change the target (vec_staticrt_PWR9.lo) of the rule to indicate
* that this object is intended for a <I>static</I> runtime archive.
* And we list prerequisites vec_runtime_PWR9.c and
* \$(pveclibinclude_HEADERS)
*
* For the recipe we expand both clauses (am__fastdepCC and AMDEP) from
* the example. We don't know exactly what they represent or do, but
* assume they both are needed for some configurations.
* We use the alternative PVECCOMPILE to provide all the
* libtool commands and options we need without the CFLAGS.
* We use new PVECLIB_POWER9_CFLAGS macro to provide all the platform
* specific target options we need. The automatic variable \$\@ provides
* the file name of the target object (vec_staticrt_PWR9.lo).
* And we specify the \$(srcdir) qualified source file
* (vec_runtime_PWR9.c) as input to the compile.
* We can provide similar rules for the other processor targets
* (PWR8/PWR7).
*
* With this technique we control the compilation of specific targets
* without requiring unique LTLIBRARIES. This was only required before
* so libtool would allow target specific CFLAGS. So we can eliminate
* libpvecPWR9.la, libpvecPWR8.la, and libpvecPWR7.la from
* lib_LTLIBRARIES.
*
* Continuing the theme of separating the static archive elements from
* DSO elements we rename libpveccommon.la to libpvecstatic.la.
* We can add the common (none target specific) source files and CFLAGS
* to <I>libpvecstatic_la</I>.
* \code
libpvecstatic_la_SOURCES = tipowof10.c decpowof2.c

libpvecstatic_la_CFLAGS = $(AM_CPPFLAGS) $(PVECLIB_DEFAULT_CFLAGS) $(AM_CFLAGS)
* \endcode
* We still need to add the target specific objects generated by the
* rules above to the libpvecstatic.a archive.
* \code
# libpvecstatic_la already includes tipowof10.c decpowof2.c.
# Now add the name qualified -mcpu= target runtimes.
libpvecstatic_la_LIBADD = vec_staticrt_PWR9.lo
libpvecstatic_la_LIBADD += vec_staticrt_PWR8.lo
libpvecstatic_la_LIBADD += vec_staticrt_PWR7.lo
* \endcode
* \note the libpvecstatic archive will contain 2 or 3 implementations
* of each target specific function
* (i.e. the function vec_mul128x128() will have implementations
* vec_mul128x128_PWR7() and vec_mul128x128_PWR8(),
* vec_mul128x128_PWR9()).
* This OK because because the target suffix insures the name is unique
* within the archive.
* When an application calls function with the appropriate target suffix
* (using the __VEC_PWR_IMP() wrapper macro) and links to libpvecstatic,
* the linker will extract only the matching implementations and include
* them in the static program image.
*
*
* \subsection main_libary_issues_0_0_2 Building dynamic runtime libraries
*
* Building objects for dynamic runtime libraries is a bit more
* complicated than building static archives. For one dynamic libraries
* requires position independent code (<B>PIC</B>) while static code
* does not. Second we want to leverage the Dynamic Linker/Loader's
* GNU Indirect Function
* (See: <a href="https://sourceware.org/glibc/wiki/GNU_IFUNC">
* What is an indirect function (IFUNC)?</a>) binding mechanism.
*
* PIC functions require a more complicated call linkage or
* function prologue. This usually requires the -fpic compiler option.
* This is the case for the OpenPOWER ELF V2 ABI. Any PIC function must
* assume that the caller may be from an different execution unit
* (library or main executable).
* So the called function needs to establish the Table of Contents
* (<B>TOC</B>) base address for itself. This is the case if
* the called function needs to reference static or const storage
* variables or calls to functions in other dynamic libraries.
* So it is normal to compile library runtime codes separately for
* static archives and DSOs.
*
* \note The details of how the <B>TOC</B> is established differs
* between the ELF V1 ABI (Big Endian POWER) and the
* ELF V2 ABI (Little Endian POWER). This should not be
* an issue if compile options (-fpic) are used correctly.
*
* There are additional differences associated with dynamic selection of
* function Implementations for different processor targets.
* The Linux dynamic linker/loader (ld64.so) provides general mechanism
* for target specific binding of function call linkage.
*
* The dynamic linker employees a user supplied resolver mechanism as
* function calls are dynamically bound to to an implementation.
* The DSO exports function symbols that externally look like a normal
* <I>extern</I>. For example:
* \code
extern __VEC_U_256
vec_mul128x128 (vui128_t, vui128_t);
* \endcode
* This symbol's implementation has a special <B>STT_GNU_IFUNC</B>
* attribute recognized by the dynamic linker which associates this
* symbol with the corresponding runtime resolver function.
* So in addition to any platform specific implementations we need to
* provide the resolver function referenced by the <I>IFUNC</I> symbol.
* For example:
* \code
 //
 //  \file  vec_runtime_DYN.c
 //
extern __VEC_U_256
vec_mul128x128_PWR7 (vui128_t, vui128_t);

extern __VEC_U_256
vec_mul128x128_PWR8 (vui128_t, vui128_t);

extern __VEC_U_256
vec_mul128x128_PWR9 (vui128_t, vui128_t);

static
__VEC_U_256
(*resolve_vec_mul128x128 (void))(vui128_t, vui128_t)
{
#ifdef  __BUILTIN_CPU_SUPPORTS__
  if (__builtin_cpu_is ("power9"))
    return vec_mul128x128_PWR9;
  else
    {
      if (__builtin_cpu_is ("power8"))
        return vec_mul128x128_PWR8;
      else
        return vec_mul128x128_PWR7;
    }
#else // ! __BUILTIN_CPU_SUPPORTS__
    return vec_mul128x128_PWR7;
#endif
}

__VEC_U_256
vec_mul128x128 (vui128_t, vui128_t)
__attribute__ ((ifunc ("resolve_vec_mul128x128")));
* \endcode
* For convince we collect the:
* - IFUNC symbols
* - corresponging resolver functions
* - and externs to target specific implementations
*
* into one or more source files (For example: vec_runtime_DYN.c).
*
* On the program's first call to an <I>IFUNC</I> symbol, the dynamic
* linker calls the resolver function associated with that symbol.
* The resolver function performs a runtime check to determine the platform,
* selects the (closest) matching platform specific function,
* then returns that function pointer to the dynamic linker.
*
* The dynamic linker stores this function pointer in the callers
* Procedure Linkage Tables (PLT) before forwarding the call to the
* resolved implementation.  Any subsequent calls to this
* function symbol branch (via the PLT) directly to the appropriate
* platform specific implementation.
*
* \note The platform specific implementations we use here are compiled
* from the same source files we used to build the static library
* archive.
*
* Like the static libraries we need to build multiple target specific
* implementations of the functions. So we can leverage the example of
* explicit Makefile rules we used for the static archive but with some
* minor differences. For example:
* \code
vec_dynrt_PWR9.lo: vec_runtime_PWR9.c $(pveclibinclude_HEADERS)
if am__fastdepCC
	$(PVECCOMPILE) -fpic $(PVECLIB_POWER9_CFLAGS) -MT $@ -MD -MP -MF \
	$(DEPDIR)/$*.Tpo -c -o $@ $(srcdir)/vec_runtime_PWR9.c
	mv -f $(DEPDIR)/$*.Tpo $(DEPDIR)/$*.Plo
else
if AMDEP
	source='vec_runtime_PWR9.c' object='$@' libtool=yes @AMDEPBACKSLASH@
	DEPDIR=$(DEPDIR) $(CCDEPMODE) $(depcomp) @AMDEPBACKSLASH@
endif
	$(PVECCOMPILE) -fpic $(PVECLIB_POWER9_CFLAGS) -c -o $@ \
	$(srcdir)/vec_runtime_PWR9.c
endif
* \endcode
* Again we change the rule target (vec_dynrt_PWR9.lo) of the rule to
* indicate that this object is intended for a <I>DSO</I> runtime.
* And we list the same prerequisites vec_runtime_PWR9.c and
* \$(pveclibinclude_HEADERS)
*
* For the recipe we expand both clauses (am__fastdepCC and AMDEP) from
* the example.
* We use the alternative PVECCOMPILE to provide all the
* libtool commands and options we need without the CFLAGS.
* But we insert the \-fpic option so the compiler will  will generate
* position independent code.
* We use a new PVECLIB_POWER9_CFLAGS macro to provide all the platform
* specific target options we need. The automatic variable \$\@ provides
* the file name of the target object (vec_dynrt_PWR9.lo).
* And we specify the same \$(srcdir) qualified source file
* (vec_runtime_PWR9.c) we used for the static library.
* We can provide similar rules for the other processor targets
* (PWR8/PWR7). We also build an \-fpic version of vec_runtime_common.c.
*
* Continuing the theme of separating the static archive elements from
* DSO elements, we use libpvec.la as the libtool name for libpvec.so.
* Here we add the source files for the IFUNC resolvers and add \-fpic
* as library specific CFLAGS to <I>libpvec_la</I>.
* \code
libpvec_la_SOURCES = vec_runtime_DYN.c

libpvec_la_CFLAGS = $(AM_CPPFLAGS) -fpic $(PVECLIB_DEFAULT_CFLAGS) $(AM_CFLAGS)
* \endcode
* We still need to add the target specific and common objects generated
* by the rules above to the libpvec library.
* \code
# libpvec_la already includes vec_runtime_DYN.c compiled compiled -fpic
# for IFUNC resolvers.
# Now adding the -fpic -mcpu= target built runtimes.
libpvec_la_LDFLAGS = -version-info $(PVECLIB_SO_VERSION)
libpvec_la_LIBADD = vec_dynrt_PWR9.lo
libpvec_la_LIBADD += vec_dynrt_PWR8.lo
libpvec_la_LIBADD += vec_dynrt_PWR7.lo
libpvec_la_LIBADD += vec_dynrt_common.lo
libpvec_la_LIBADD += -lc
* \endcode
*
* \subsection make_libary_issues_0_0_3 Calling Multi-platform functions
*
* The next step is to provide mechanisms for applications to call
* these functions via static or dynamic linkage.
* For static linkage the application needs to reference a specific
* platform variant of the functions name.
* For dynamic linkage we will use <B>STT_GNU_IFUNC</B> symbol resolution
* (a symbol type extension to the ELF standard).
*
* \subsubsection main_libary_issues_0_0_1_1 Static linkage to platform specific functions
* For static linkage the application is compiled for a specific
* platform target (via -mcpu=). So function calls should be bound to
* the matching platform specific implementations. The application
* may select the platform specific function directly by defining
* a <I>extern</I> and invoking the platform qualified function.
*
* Or simply use the __VEC_PWR_IMP() macro as wrapper for the function
* name in the application.
* This selects the appropriate platform specific implementation based
* on the -mcpu= specified for the application compile.
* For example.
* \code
  k = __VEC_PWR_IMP (vec_mul128x128)(i, j);
* \endcode
*
* The vec_int512_ppc.h header provides the default platform qualified
* <I>extern</I> declarations for this and related functions based on
* the \-mcpu= specified for the compile of application including this
* header. For example.
* \code
extern __VEC_U_256
__VEC_PWR_IMP (vec_mul128x128) (vui128_t, vui128_t);
* \endcode
* For example if the applications calling vec_mul128x128() is itself
* compiled with -mcpu=power8, then the __VEC_PWR_IMP() will insure
* that:
* - The vec_int512_ppc.h header will define an extern for
* vec_mul128x128_PWR8.
* - That application's calls to __VEC_PWR_IMP (vec_mul128x128) will
* reference vec_mul128x128_PWR8.
*
* The application should then link to the libpvecstatic.a archive.
* Where the application references PVECLIB functions with the
* appropriate target suffix, the linker will extract only the matching
* implementations and include them in the program image.
*
* \subsubsection main_libary_issues_0_0_1_2 Dynamic linkage to platform specific functions
* Applications using dynamic linkage will call the unqualified
* function symbol.
* For example:
* \code
extern __VEC_U_256
vec_mul128x128 (vui128_t, vui128_t);
* \endcode
*
* This symbol's implementation (in libpvec.so) has a special
* <B>STT_GNU_IFUNC</B>
* attribute recognized by the dynamic linker which associates this
* symbol with the corresponding runtime resolver function.
* The application simply calls the (unqualified) function and the
* dynamic linker (with the help of PVECLIB's IFUNC resolvers) handles
* the details.
*
* \section perf_data Performance data.
*
* It is useful to provide basic performance data for each pveclib
* function.  This is challenging as these functions are small and
* intended to be in-lined within larger functions (algorithms).
* As such they are subject to both the compiler's instruction
* scheduling and common subexpression optimizations plus the
* processors super-scalar and out-of-order execution design features.
*
* As pveclib functions are normally only a few
* instructions, the actual timing will depend on the context it
* is in (the instructions that it depends on for data and instructions
* that proceed them in the pipelines).
*
* The simplest approach is to use the same performance metrics as the
* Power Processor Users Manuals Performance Profile.
* This is normally per instruction latency in cycles and
* throughput in instructions issued per cycle. There may also be
* additional information for special conditions that may apply.
*
* For example the vector float absolute value function.
* For recent PowerISA implementations this a single
* (VSX <B>xvabssp</B>) instruction which we can look up in the
* POWER8 / POWER9 Processor User's Manuals (<B>UM</B>).
*
*  |processor|Latency|Throughput|
*  |--------:|:-----:|:---------|
*  |power8   | 6-7   | 2/cycle  |
*  |power9   | 2     | 2/cycle  |
*
* The POWER8 UM specifies a latency of
* <I>"6 cycles to FPU (+1 cycle to other VSU ops"</I>
* for this class of VSX single precision FPU instructions.
* So the minimum latency is 6 cycles if the register result is input
* to another VSX single precision FPU instruction.
* Otherwise if the result is input to a VSU logical or integer
* instruction then the latency is 7 cycles.
* The POWER9 UM shows the pipeline improvement of 2 cycles latency
* for simple FPU instructions like this.
* Both processors support dual pipelines for a 2/cycle throughput
* capability.
*
* A more complicated example: \code
static inline vb32_t
vec_isnanf32 (vf32_t vf32)
{
vui32_t tmp2;
const vui32_t expmask = CONST_VINT128_W(0x7f800000, 0x7f800000, 0x7f800000,
					0x7f800000);
#if _ARCH_PWR9
// P9 has a 2 cycle xvabssp and eliminates a const load.
tmp2 = (vui32_t) vec_abs (vf32);
#else
const vui32_t signmask = CONST_VINT128_W(0x80000000, 0x80000000, 0x80000000,
					 0x80000000);
tmp2 = vec_andc ((vui32_t)vf32, signmask);
#endif
return vec_cmpgt (tmp2, expmask);
}
* \endcode
* Here we want to test for <I>Not A Number</I> without triggering any
* of the associate floating-point exceptions (VXSNAN or VXVC).
* For this test the sign bit does not effect the result so we need to
* zero the sign bit before the actual test. The vector abs would work
* for this, but we know from the example above that this instruction
* has a high latency as we are definitely passing the result to a
* non-FPU instruction (vector compare greater than unsigned word).
*
* So the code needs to load two constant vectors masks, then vector
* and-compliment to clear the sign-bit, before comparing each word
* for greater then infinity. The generated code should look
* something like this: \code
      addis   r9,r2,.rodata.cst16+0x10@ha
      addis   r10,r2,.rodata.cst16+0x20@ha
      addi    r9,r9,.rodata.cst16+0x10@l
      addi    r10,r10,.rodata.cst16+0x20@l
      lvx     v0,0,r10	# load vector const signmask
      lvx     v12,0,r9	# load vector const expmask
      xxlandc vs34,vs34,vs32
      vcmpgtuw v2,v2,v12
* \endcode
* So six instructions to load the const masks and two instructions
* for the actual vec_isnanf32 function. The first six instructions
* are only needed once for each containing function, can be hoisted
* out of loops and into the function prologue, can be <I>commoned</I>
* with the same constant for other pveclib functions, or executed
* out-of-order and early by the processor.
*
* Most of the time, constant setup does not contribute measurably to
* the over all performance of vec_isnanf32. When it does it is
* limited by the longest (in cycles latency) of the various
* independent paths that load constants.  In this case the const load
* sequence is composed of three pairs of instructions that can issue
* and execute in parallel. The addis/addi FXU instructions supports
* throughput of 6/cycle and the lvx load supports 2/cycle.
* So the two vector constant load sequences can execute
* in parallel and the latency is same as a single const load.
*
* For POWER8 it appears to be (2+2+5=) 9 cycles latency for the const
* load. While the core vec_isnanf32 function (xxlandc/vcmpgtuw) is a
* dependent sequence and runs (2+2) 4 cycles latency.
* Similar analysis for POWER9 where the addis/addi/lvx sequence is
* still listed as (2+2+5) 9 cycles latency. While the xxlandc/vcmpgtuw
* sequence increases to (2+3) 5 cycles.
*
* The next interesting question is what can we say about throughput
* (if anything) for this example.  The thought experiment is "what
* would happen if?";
* - two or more instances of vec_isnanf32 are used within a single
* function,
* - in close proximity in the code,
* - with independent data as input,
*
* could the generated instructions execute in parallel and to what
* extent. This illustrated by the following (contrived) example:
* \code
int
test512_all_f32_nan (vf32_t val0, vf32_t val1, vf32_t val2, vf32_t val3)
{
const vb32_t alltrue = { -1, -1, -1, -1 };
vb32_t nan0, nan1, nan2, nan3;

nan0 = vec_isnanf32 (val0);
nan1 = vec_isnanf32 (val1);
nan2 = vec_isnanf32 (val2);
nan3 = vec_isnanf32 (val3);

nan0 = vec_and (nan0, nan1);
nan2 = vec_and (nan2, nan3);
nan0 = vec_and (nan2, nan0);

return vec_all_eq(nan0, alltrue);
}
* \endcode
* which tests 4 X vector float (16 X float) values and returns true
* if all 16 floats are NaN. Recent compilers will generates something
* like the following PowerISA code:
* \code
   addis   r9,r2,-2
   addis   r10,r2,-2
   vspltisw v13,-1	# load vector const alltrue
   addi    r9,r9,21184
   addi    r10,r10,-13760
   lvx     v0,0,r9	# load vector const signmask
   lvx     v1,0,r10	# load vector const expmask
   xxlandc vs35,vs35,vs32
   xxlandc vs34,vs34,vs32
   xxlandc vs37,vs37,vs32
   xxlandc vs36,vs36,vs32
   vcmpgtuw v3,v3,v1	# nan1 = vec_isnanf32 (val1);
   vcmpgtuw v2,v2,v1	# nan0 = vec_isnanf32 (val0);
   vcmpgtuw v5,v5,v1	# nan3 = vec_isnanf32 (val3);
   vcmpgtuw v4,v4,v1	# nan2 = vec_isnanf32 (val2);
   xxland  vs35,vs35,vs34	# nan0 = vec_and (nan0, nan1);
   xxland  vs36,vs37,vs36	# nan2 = vec_and (nan2, nan3);
   xxland  vs36,vs35,vs36	# nan0 = vec_and (nan2, nan0);
   vcmpequw. v4,v4,v13	# vec_all_eq(nan0, alltrue);
...
* \endcode
* first the generated code loading the vector constants for signmask,
* expmask, and alltrue. We see that the code is generated only once
* for each constant. Then the compiler generate the core vec_isnanf32
* function four times and interleaves the instructions. This enables
* parallel pipeline execution where conditions allow. Finally the 16X
* isnan results are reduced to 8X, then 4X, then to a single
* condition code.
*
* For this exercise we will ignore the constant load as in any
* realistic usage it will be <I>commoned</I> across several pveclib
* functions and hoisted out of any loops. The reduction code is not
* part of the vec_isnanf32 implementation and also ignored.
* The sequence of 4X xxlandc and 4X vcmpgtuw in the middle it the
* interesting part.
*
* For POWER8 both xxlandc and vcmpgtuw are listed as 2 cycles
* latency and throughput of 2 per cycle. So we can assume that (only)
* the first two xxlandc will issue in the same cycle (assuming the
* input vectors are ready).  The issue of the next two xxlandc
* instructions will be delay by 1 cycle. The following vcmpgtuw
* instruction are dependent on the xxlandc results and will not
* execute until their input vectors are ready. The first two vcmpgtuw
* instruction will execute 2 cycles (latency) after the first two
* xxlandc instructions execute. Execution of the second two vcmpgtuw
* instructions will be delayed 1 cycle due to the issue delay in the
* second pair of xxlandc instructions.
*
* So at least for this example and this set of simplifying assumptions
* we suggest that the throughput metric for vec_isnanf32 is 2/cycle.
* For latency metric we offer the range with the latency for the core
* function (without and constant load overhead) first. Followed by the
* total latency (the sum of the constant load and core function
* latency). For the vec_isnanf32 example the metrics are:
*
*  |processor|Latency|Throughput|
*  |--------:|:-----:|:---------|
*  |power8   | 4-13  | 2/cycle  |
*  |power9   | 5-14  | 2/cycle  |
*
* Looking at a slightly more complicated example where core functions
* implementation can execute more then one instruction per cycle.
* Consider:
* \code
static inline vb32_t
vec_isnormalf32 (vf32_t vf32)
{
vui32_t tmp, tmp2;
const vui32_t expmask = CONST_VINT128_W(0x7f800000, 0x7f800000, 0x7f800000,
					0x7f800000);
const vui32_t minnorm = CONST_VINT128_W(0x00800000, 0x00800000, 0x00800000,
					0x00800000);
#if _ARCH_PWR9
// P9 has a 2 cycle xvabssp and eliminates a const load.
tmp2 = (vui32_t) vec_abs (vf32);
#else
const vui32_t signmask = CONST_VINT128_W(0x80000000, 0x80000000, 0x80000000,
					 0x80000000);
tmp2 = vec_andc ((vui32_t)vf32, signmask);
#endif
tmp = vec_and ((vui32_t) vf32, expmask);
tmp2 = (vui32_t) vec_cmplt (tmp2, minnorm);
tmp = (vui32_t) vec_cmpeq (tmp, expmask);

return (vb32_t )vec_nor (tmp, tmp2);
}
* \endcode
* which requires two (independent) masking operations (sign and exponent),
* two (independent) compares that are dependent on the masking operations,
* and a final <I>not OR</I> operation dependent on the compare results.
*
* The generated POWER8 code looks like this: \code
   addis   r10,r2,-2
   addis   r8,r2,-2
   addi    r10,r10,21184
   addi    r8,r8,-13760
   addis   r9,r2,-2
   lvx     v13,0,r8
   addi    r9,r9,21200
   lvx     v1,0,r10
   lvx     v0,0,r9
   xxland  vs33,vs33,vs34
   xxlandc vs34,vs45,vs34
   vcmpgtuw v0,v0,v1
   vcmpequw v2,v2,v13
   xxlnor  vs34,vs32,vs34
* \endcode
* Note this this sequence needs to load 3 vector constants. In
* previous examples we have noted that POWER8 lvx supports 2/cycle
* throughput. But with good scheduling, the 3rd vector constant load,
* will only add 1 additional cycle to the timing (10 cycles).
*
* Once the constant masks are loaded the xxland/xxlandc instructions
* can execute in parallel. The vcmpgtuw/vcmpequw  can also execute
* in parallel but are delayed waiting for the results of masking
* operations. Finally the xxnor is dependent on the data from both
* compare instructions.
*
* For POWER8 the latencies are 2 cycles each, and assuming parallel
* execution of xxland/xxlandc and vcmpgtuw/vcmpequw we can assume
* (2+2+2=) 6 cycles minimum latency and another 10 cycles for the
* constant loads (if needed).
*
* While the POWER8 core has ample resources (10 issue ports across
* 16 execution units), this specific sequence is restricted to the
* two <I>issue ports and VMX execution units</I> for this class of
* (simple vector integer and logical) instructions.
* For vec_isnormalf32 this allows for a lower latency
* (6 cycles vs the expected 10, over 5 instructions),
* it also implies that both of the POWER8 cores
* <I>VMX execution units</I> are busy for 2 out of the 6 cycles.
*
* So while the individual instructions have can have a throughput of
* 2/cycle, vec_isnormalf32 can not.  It is plausible for two
* executions of vec_isnormalf32 to interleave with a delay of 1 cycle
* for the second sequence.  To keep the table information simple for
* now, just say the throughput of vec_isnormalf32 is 1/cycle.
*
* After that it gets complicated. For example after the first two
* instances of vec_isnormalf32 are issued, both
* <I>VMX execution units</I> are busy for 4 cycles. So either the
* first instructions of the third vec_isnormalf32 will be delayed
* until the fifth cycle.  Or the compiler scheduler will interleave
* instructions across the instances of vec_isnormalf32 and the
* latencies of individual vec_isnormalf32 results will increase.
* This is too complicated to put in a simple table.
*
* For POWER9 the sequence is slightly different \code
   addis   r10,r2,-2
   addis   r9,r2,-2
   xvabssp vs45,vs34
   addi    r10,r10,-14016
   addi    r9,r9,-13920
   lvx     v1,0,r10
   lvx     v0,0,r9
   xxland  vs34,vs34,vs33
   vcmpgtuw v0,v0,v13
   vcmpequw v2,v2,v1
   xxlnor  vs34,vs32,vs34
* \endcode
* We use vec_abs (xvabssp) to replace the sigmask and vec_andc
* and so only need to load two vector constants.
* So the constant load overhead is reduced to 9 cycles.
* However the the vector compares are now 3 cycles for (2+3+2=)
* 7 cycles for the core sequence. The final table for vec_isnormalf32:
*
*  |processor|Latency|Throughput|
*  |--------:|:-----:|:---------|
*  |power8   | 6-16  | 1/cycle  |
*  |power9   | 7-16  | 1/cycle  |
*
* \subsection  perf_data_sub_1 Additional analysis and tools.
*
* The overview above is simplified analysis based on the instruction
* latency and throughput numbers published in the
* Processor User's Manuals (see \ref mainpage_ref_docs).
* These values are <I>best case</I> (input data is ready, SMT1 mode,
* no cache misses, mispredicted branches, or other hazards) for each
* instruction in isolation.
*
* \note This information is intended as a guide for compiler and
* application developers wishing to optimize for the platform.
* Any performance tables provided for pveclib functions are in this
* spirit.
*
* Of course the actual performance is complicated by the overall
* environment and how the pveclib functions are used. It would be
* unusual for pveclib functions to be used in isolation. The compiler
* will in-line pveclib functions and look for sub-expressions it can
* hoist out of loops or share across pveclib function instances. The
* The compiler will also model the processor and schedule instructions
* across the larger containing function. So in actual use the
* instruction sequences for the examples above are likely to be
* interleaved with instructions from other pvevlib functions
* and user written code.
*
* Larger functions that use pveclib and even some of the more
* complicated pveclib functions (like vec_muludq) defy simple
* analysis. For these cases it is better to use POWER specific
* analysis tools. To understand the overall pipeline flows and
* identify hazards the instruction trace driven performance simulator
* is recommended.
*
* The
* <a href="https://developer.ibm.com/linuxonpower/advance-toolchain/">IBM Advance Toolchain</a>
* includes an updated (POWER enabled)
* Valgrind tool and instruction trace plug-in (itrace). The itrace
* tool (--tool=itrace) collects instruction traces for the whole
* program or specific functions (via --fnname= option).
*
* \note The Valgrind package provided by the Linux Distro may not be
* enabled for the latest POWER processor. Nor will it include the
* itrace plug-in or the associated vgi2qt conversion tool.
*
* Instruction trace files are processed by the
* <a href="https://developer.ibm.com/linuxonpower/sdk-packages/">Performance Simulator</a>
* (sim_ppc) models. Performance simulators are specific to each
* processor generation (POWER7-9) and provides a cycle accurate
* modeling for instruction trace streams. The results of the model
* (a pipe file) can viewed via one the interactive display tools
* (scrollpv, jviewer) or passed to an analysis tool like
* <a href="https://developer.ibm.com/linuxonpower/sdk-packages/">pipestat</a>.
*
**/

#endif /* __PVECLIB_MAIN_DOX_H  */
