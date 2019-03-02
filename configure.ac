AC_INIT([gpmlr], 0.1.0)

# Make sure we can deal with C++
AC_LANG(C++)
AC_REQUIRE_CPP

# Check for Octave; will expand this message later to tell the user
# how to install what they need to install
AC_MSG_CHECKING([for Octave])
AC_CHECK_PROG(HAS_OCTAVE, mkoctfile, found)
if test x"$HAS_OCTAVE" != x"found" ; then
    AC_MSG_ERROR([Please install Octave before installing.])
fi

# We need to find the Octave API header files
# We'll do it by stealing the flags from mkoctfile
AC_MSG_CHECKING([necessary additions to PKG_CPPFLAGS for the Octave API])
octave_cppflags=$(mkoctfile --print INCFLAGS)
AC_MSG_RESULT([])
AC_MSG_CHECKING([necessary -l additions to PKG_LIBS for the Octave API])
octave_libs=$(mkoctfile --print OCTAVE_LIBS)
AC_MSG_RESULT([])
AC_MSG_CHECKING([necessary -L additions to PKG_LIBS for the Octave API])
octave_lflags=$(mkoctfile --print LFLAGS)
AC_MSG_RESULT([])

# Substitute things where appropriate in Makevars
AC_SUBST([OCTAVE_CPPFLAGS], ["${octave_cppflags}"])
AC_SUBST([OCTAVE_LIBS], ["${octave_libs}"])
AC_SUBST([OCTAVE_LFLAGS], ["${octave_lflags}"])
AC_CONFIG_FILES([src/Makevars])
AC_OUTPUT

