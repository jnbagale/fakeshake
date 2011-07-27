AC_INIT(fakeshake, 0.0.1, jnbagale@gmail.com)
AM_INIT_AUTOMAKE
AC_CONFIG_SRCDIR([src/])
AM_CONFIG_HEADER([src/config.h])

AC_DEFINE([PROGNAME],["fakeshake"],["the fake shake data generator"])
AC_DEFINE([DEFAULT_HOST],["theclashingrocks.org"],["the default spread server"])
AC_DEFINE([DEFAULT_PORT],[4803],["the default spread server port"])
AC_DEFINE([DEFAULT_GROUP],["world"],["the group everyone can join"])

AC_DEFINE([DEFAULT_FREQ_TYPE],["random"],["the dafult shake frequency type"])
AC_DEFINE([DEFAULT_FREQ_VALUE],[2000],["the default shake frequency value"])
AC_DEFINE([DEFAULT_QUAKE_SIZE],["medium"],["the default shake intensity value"])


# Fake data generator settings
AC_DEFINE([FAKE_GEN_FREQ],[10],["Writing rate of fake accelerometer data in milliseconds"])
AC_DEFINE([FAKE_POSITION_FILE],["fakeposition.txt"],["Fake accelerometer"])
AC_DEFINE([SAMPLE_SIZE],[100],["Number of fake accelerometer samples allowed"])

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


# Check for spread library and header
AC_CHECK_LIB(spread, SP_connect, , AC_MSG_ERROR(*** libspread not found))
AC_CHECK_HEADERS(sp.h)

# check for glib
PKG_CHECK_MODULES(GLIB, [glib-2.0])

# check for UUID
PKG_CHECK_MODULES(UUID, [uuid])

# check for SDL
PKG_CHECK_MODULES(GSL, [gsl])

# check for SDL
#PKG_CHECK_MODULES(SDL, [sdl])

#check for SDl_gfx
#PKG_CHECK_MODULES(SDL_gfx, [SDL_gfx])

AC_CONFIG_FILES([Makefile src/Makefile])

AC_OUTPUT
