AC_INIT(fakeshake, 0.0.2, jnbagale@gmail.com)
AM_INIT_AUTOMAKE
AC_CONFIG_SRCDIR([src/])
AM_CONFIG_HEADER([src/config.h])

AC_DEFINE([PROGNAME],["fakeshake"],["the fake shake data generator"])
AC_DEFINE([DEFAULT_HOST],["10.48.128.32"],["the default zeromq host"])
AC_DEFINE([DEFAULT_PORT],[5556],["the default zeromq host port"])
AC_DEFINE([DEFAULT_GROUP],["world"],["the group everyone can join"])

# Fake data generator settings

AC_DEFINE([DEFAULT_CLIENT_NO],[100],["the default number of fakeshake clients"])
AC_DEFINE([DEFAULT_FREQUENCY_TYPE],["random"],["the dafult fake data generation type"])
AC_DEFINE([DEFAULT_FREQUENCY_VALUE],[200],["the default fake data generation frequency"])
AC_DEFINE([DEFAULT_SAMPLE_SIZE],[7],["the default number of samples per cycle "])
AC_DEFINE([DEFAULT_SAMPLE_TYPE],["random"],["the default sample type"])
AC_DEFINE([DEFAULT_NETWORK_FREQ],[10],["the default network message pull frequency"])
AC_DEFINE([FAKE_GEN_FREQ],[100],["Writing rate of fake accelerometer data in milliseconds"])

# --enable-debug flag
AC_ARG_ENABLE(debug,
    [  --enable-debug     Turn on debugging [[default=no]]],
    enable_debug="$enableval",
    enable_debug=no)

if test x$enable_debug = xyes ; then
    AC_DEFINE([DEBUG_MODE], [], [true])
fi

       
# Checks for programs.
AC_PROG_CC

# Checks for header files.
AC_HEADER_STDC

# Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST
AC_TYPE_SIZE_T

# Checks for library functions.
AC_FUNC_MALLOC

# check for glib
PKG_CHECK_MODULES(GLIB, [glib-2.0])

#check for gthread
PKG_CHECK_MODULES(GTHREAD, [gthread-2.0] )

# check for UUID
PKG_CHECK_MODULES(UUID, [uuid])

# check for GSL
PKG_CHECK_MODULES(GSL, [gsl])

# check for ZEROMQ
PKG_CHECK_MODULES(LIBZMQ, [libzmq])

AC_CONFIG_FILES([Makefile src/Makefile])

AC_OUTPUT
