#ifndef OCTAVE_VERSION_DEFINED
#define OCTAVE_VERSION_DEFINED

// We need to do different things for differenct Octave versions
#include <octave/version.h>
#if OCTAVE_MAJOR_VERSION >= 4 && OCTAVE_MINOR_VERSION >= 2
    #define OCTAVE_4_2_OR_HIGHER
    #if OCTAVE_MINOR_VERSION >= 4
        #define OCTAVE_4_4_OR_HIGHER
    #endif // Octave version >= 4.4.0
#else
    #define OCTAVE_BELOW_4_2
#endif // Octave version >= 4.2.0

#endif // OCTAVE_VERSION_DEFINED

