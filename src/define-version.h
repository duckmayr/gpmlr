#ifndef OCTAVE_VERSION_DEFINED
#define OCTAVE_VERSION_DEFINED

// We need to do different things for differenct Octave versions
#include <octave/version.h>

// We temporarily make some abbreviations
#define MAJOR OCTAVE_MAJOR_VERSION
#define MINOR OCTAVE_MINOR_VERSION

// Figure out whether the version is past certain cutpoints
#if MAJOR > 4 || (MAJOR == 4 && MINOR >= 2)
    #define OCTAVE_4_2_OR_HIGHER
    #if MAJOR > 4 || (MAJOR == 4 && MINOR >= 4)
        #define OCTAVE_4_4_OR_HIGHER
    #endif // Octave version >= 4.4.0
#else
    #define OCTAVE_BELOW_4_2
#endif // Octave version >= 4.2.0

// Undefine our abbreviations
#undef MAJOR
#undef MINOR

#endif // OCTAVE_VERSION_DEFINED

