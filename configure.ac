dnl Process this file with autoconf to produce a configure script.

AC_PREREQ(2.63)
AC_INIT([pveclib], [1.0.3], munroesj52@gmail.com pc@us.ibm.com, , https://github.com/open-power-sdk/pveclib)

AC_CANONICAL_TARGET
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_SRCDIR([src/pveclib/vec_common_ppc.h])
AM_INIT_AUTOMAKE(subdir-objects)
AM_MAINTAINER_MODE([disable])
AX_AM_MACROS
AC_LANG(C)

AC_PROG_CC
AC_PROG_LIBTOOL
AC_PROG_GREP
AC_PROG_EGREP

# Checks for header files.
AC_CHECK_HEADERS([altivec.h stddef.h stdint.h unistd.h stdio.h fenv.h float.h math.h])

# Doxygen support
DX_HTML_FEATURE(ON)
DX_MAN_FEATURE(OFF)
DX_PDF_FEATURE(OFF)
DX_PS_FEATURE(OFF)
DX_INIT_DOXYGEN($PACKAGE_NAME, [ doc/pveclib-doxygen-pveclib.doxy ])

dnl ############################################################
dnl Save user flags

saved_CFLAGS="$CFLAGS"
saved_LDFLAGS="$LDFLAGS"

dnl ############################################################
dnl Options

dnl *** Warnings ***
have_wall=`echo $AM_CFLAGS $saved_CFLAGS | $EGREP -c '\-Wall|\-Wextra'`
if test "$have_wall" = "0"; then
  CFLAGS="-Wall"
  AC_MSG_CHECKING([for option $CFLAGS])
  AC_COMPILE_IFELSE(
    [
      AC_LANG_PROGRAM([])
    ],
    [
      AC_MSG_RESULT([yes])
      AM_CFLAGS="$AM_CFLAGS $CFLAGS"
    ],
    [
      AC_MSG_RESULT([no])
    ]
  )
fi

dnl *** Symbols ***
have_syms=`echo $AM_CFLAGS $saved_CFLAGS | $EGREP -c '\-g|\-g1|\-g2|\-g3|\-ggdb'`
if test "$have_syms" = "0"; then
  CFLAGS="-g"
  AC_MSG_CHECKING([for option $CFLAGS])
  AC_COMPILE_IFELSE(
    [
      AC_LANG_PROGRAM([])
    ],
    [
      AC_MSG_RESULT([yes])
      AM_CFLAGS="$AM_CFLAGS $CFLAGS"
    ],
    [
      AC_MSG_RESULT([no])
    ]
  )
fi

dnl *** Optimizations ***
have_opt=`echo $AM_CFLAGS $saved_CFLAGS | $EGREP -c '\-O0|\-O1|\-O2|\-O3|\-O4|\-O5|\-Os|\-Og|\-Ofast'`
if test "$have_opt" = "0"; then
  CFLAGS="-O3"
  AC_MSG_CHECKING([for option $CFLAGS])
  AC_COMPILE_IFELSE(
    [
      AC_LANG_PROGRAM([])
    ],
    [
      AC_MSG_RESULT([yes])
      AM_CFLAGS="$AM_CFLAGS $CFLAGS"
    ],
    [
      AC_MSG_RESULT([no])
    ]
  )
fi

dnl *** IBM XLC ***
CFLAGS="-qarch=pwr8 -qaltivec"
AC_MSG_CHECKING([for POWER8 option $CFLAGS])
AC_COMPILE_IFELSE(
  [
    AC_LANG_SOURCE([[
      #include <altivec.h>
      int main(int argc, char** argv)
      {
        __vector unsigned long v1 = {0,0};
        __vector unsigned long v2 = {1,1};
        __vector unsigned long r = vec_add(v1, v2);
        return 0;
      }
    ]])
  ],
  [
    AC_MSG_RESULT([yes])
    xlc_opt="$CFLAGS"
  ],
  [
    AC_MSG_RESULT([no])
  ]
)

dnl *** GCC and Clang ***
CFLAGS="-mcpu=power8"
AC_MSG_CHECKING([for POWER8 option $CFLAGS])
AC_COMPILE_IFELSE(
  [
    AC_LANG_SOURCE([[
      #include <altivec.h>
      int main(int argc, char** argv)
      {
        __vector unsigned long v1 = {0,0};
        __vector unsigned long v2 = {1,1};
        __vector unsigned long r = vec_add(v1, v2);
        return 0;
      }
    ]])
  ],
  [
    AC_MSG_RESULT([yes])
    gcc_opt="$CFLAGS"
  ],
  [
    AC_MSG_RESULT([no])
  ]
)

if test x"$gcc_opt" != "x"; then
  AM_CFLAGS="$AM_CFLAGS $gcc_opt"
elif test x"$xlc_opt" != "x"; then
  AM_CFLAGS="$AM_CFLAGS $xlc_opt"
fi

dnl *** Pipes ***
have_pipe=`echo $AM_CFLAGS $saved_CFLAGS | $EGREP -c '\-pipe'`
if test "$have_pipe" = "0"; then
  CFLAGS="-pipe"
  AC_MSG_CHECKING([for option $CFLAGS])
  AC_COMPILE_IFELSE(
    [
      AC_LANG_PROGRAM([])
    ],
    [
      AC_MSG_RESULT([yes])
      AM_CFLAGS="$AM_CFLAGS $CFLAGS"
    ],
    [
      AC_MSG_RESULT([no])
    ]
  )
fi

dnl *** Stripping ***
have_function_section=`echo $AM_CFLAGS $saved_CFLAGS | $GREP -c '\-ffunction-sections'`
if test "$have_function_section" = "0"; then
  CFLAGS="-ffunction-sections"
  AC_MSG_CHECKING([for option $CFLAGS])
  AC_COMPILE_IFELSE(
    [
      AC_LANG_PROGRAM([])
    ],
    [
      AC_MSG_RESULT([yes])
      AM_CFLAGS="$AM_CFLAGS $CFLAGS"
    ],
    [
      AC_MSG_RESULT([no])
    ]
  )
fi

dnl *** Stripping ***
have_data_section=`echo $AM_CFLAGS $saved_CFLAGS | $GREP -c '\-fdata-sections'`
if test "$have_data_section" = "0"; then
  CFLAGS="-fdata-sections"
  AC_MSG_CHECKING([for option $CFLAGS])
  AC_COMPILE_IFELSE(
    [
      AC_LANG_PROGRAM([])
    ],
    [
      AC_MSG_RESULT([yes])
      AM_CFLAGS="$AM_CFLAGS $CFLAGS"
    ],
    [
      AC_MSG_RESULT([no])
    ]
  )
fi

dnl *** Stripping ***
have_garbage_collection=`echo $AM_LDFLAGS $saved_LDFLAGS | $GREP -c '\-Wl,--gc-sections'`
if test "$have_garbage_collection" = "0"; then
  LDFLAGS="-Wl,--gc-sections"
  AC_MSG_CHECKING([for option $LDFLAGS])
  AC_LINK_IFELSE(
    [
      AC_LANG_PROGRAM([])
    ],
    [
      AC_MSG_RESULT([yes])
      AM_LDFLAGS="$AM_LDFLAGS $LDFLAGS"
    ],
    [
      AC_MSG_RESULT([no])
    ]
  )
fi

dnl *** Stripping ***
have_bgc=`echo $AM_LDFLAGS $saved_LDFLAGS | $GREP -c '\-Wl,-bgc'`
if test "$have_bgc" = "0"; then
  LDFLAGS="-Wl,-bgc"
  AC_MSG_CHECKING([for option $LDFLAGS])
  AC_LINK_IFELSE(
    [
      AC_LANG_PROGRAM([])
    ],
    [
      AC_MSG_RESULT([yes])
      AM_LDFLAGS="$AM_LDFLAGS $LDFLAGS"
    ],
    [
      AC_MSG_RESULT([no])
    ]
  )
fi

dnl *** Stripping ***
have_as_needed=`echo $AM_LDFLAGS $saved_LDFLAGS | $GREP -c '\-Wl,--as-needed'`
if test "$have_as_needed" = "0"; then
  LDFLAGS="-Wl,--as-needed"
  AC_MSG_CHECKING([for option $LDFLAGS])
  AC_LINK_IFELSE(
    [
      AC_LANG_PROGRAM([])
    ],
    [
      AC_MSG_RESULT([yes])
      AM_LDFLAGS="$AM_LDFLAGS $LDFLAGS"
    ],
    [
      AC_MSG_RESULT([no])
    ]
  )
fi

dnl *** Stripping ***
have_exclude_libs=`echo $AM_LDFLAGS $saved_LDFLAGS | $GREP -c '\-Wl,--exclude-libs,ALL'`
if test "$have_exclude_libs" = "0"; then
  LDFLAGS="-Wl,--exclude-libs,ALL"
  AC_MSG_CHECKING([for option $LDFLAGS])
  AC_LINK_IFELSE(
    [
      AC_LANG_PROGRAM([])
    ],
    [
      AC_MSG_RESULT([yes])
      AM_LDFLAGS="$AM_LDFLAGS $LDFLAGS"
    ],
    [
      AC_MSG_RESULT([no])
    ]
  )
fi

dnl ############################################################
dnl Trim unneeded whitespace

AM_CFLAGS=`printf "%s" "$AM_CFLAGS" | sed -e 's/^[ \t]*//'`
AM_LDFLAGS=`printf "%s" "$AM_LDFLAGS" | sed -e 's/^[ \t]*//'`

dnl ############################################################
dnl Restore user flags

CFLAGS="$saved_CFLAGS"
LDFLAGS="$saved_LDFLAGS"

dnl ############################################################
dnl Write configuration

AC_SUBST([AM_CFLAGS])
AC_SUBST([AM_LDFLAGS])

AC_CONFIG_FILES(Makefile src/Makefile)
AC_OUTPUT

dnl ############################################################
dnl Print a summary of information

echo ""
echo "Auto-configuration is complete. A summary of options are below. If"
echo "something looks wrong then please modify config.h and please report"
echo "it at https://github.com/munroesj52/pveclib."

echo ""
echo "Automake flags (can be overridden by user flags):"
echo "AM_CFLAGS: $AM_CFLAGS"
echo "AM_LDFLAGS: $AM_LDFLAGS"

echo ""
echo "User flags (overrides Automake flags on conflict):"
echo "CFLAGS: $CFLAGS"
echo "LDFLAGS: $LDFLAGS"

have_O3=`echo $AM_CFLAGS $CFLAGS | $EGREP -c '\-O3|\-O4|\-O5'`
if test "$have_O3" = "0"; then
   echo ""
   echo "CFLAGS does not include -O3. You should consider building at -O3"
   echo "to engage compiler vectorizations and enhance performance."
fi
