#include <Rcpp.h>
#include <octave/oct.h> // For basic Octave types
#include <octave/octave.h> // For octave_main or interpreter class (embedding)
#include <octave/version.h> // We need to do different things for Octave 4.2 or later than Octave < 4.2
#if OCTAVE_MAJOR_VERSION >= 4 && OCTAVE_MINOR_VERSION >= 2
    #include <octave/interpreter.h> // To exit embedded Octave
#else
    #include <octave/toplev.h> // To exit embedded Octave
#endif // Octave version >= 4.2.0
#include <octave/symtab.h>

// These variables will keep track for us whether Octave is
// (or has ever been) embedded. That way we can prevent trying
// to call Octave functions when it isn't currently embedded
// (which causes segfault) and can avoid accidentally embedding
// a second time (which fills your console with warnings and may
// have consequences that I am currently unaware of).
bool is_octave_embedded = false;
bool octave_has_ever_been_embedded = false;

// This just checks if Octave is currently embedded
// [[Rcpp::export(.octave_is_embedded)]]
bool octave_is_embedded() {
    return is_octave_embedded;
}

// This embeds Octave. It does nothing (but return true) if Octave is
// currently embedded, and will not embed a second time unless force is
// true. It "toggles on" our indicator for whether Octave is currently
// embedded, as well as the indicator for whether it has *ever* been.
// ever been embedded.
// [[Rcpp::export(.embed_octave)]]
bool embed_octave(bool verbose, bool force) {
    if ( octave_is_embedded() ) {
        if ( verbose ) {
            Rcpp::Rcout << "Octave is already embedded, skipping embedding.\n";
        }
        return true;
    }
    else if ( octave_has_ever_been_embedded ) {
        if ( verbose ) {
            Rcpp::Rcout << "Octave was previously embedded.\n"
                        << "Embedding again will cause a stream of warnings, "
                        << "and is not generally supported.\n"
                        << "To do it anyway, try again with force = TRUE.\n";
        }
    }
    else {
        if ( verbose ) {
            Rcpp::Rcout << "Embedding Octave...\n";
        }
        int n_args = 2;
        string_vector embed_args(n_args);
        embed_args(0) = "gpmlr"; // The name of our application
        embed_args(1) = "-q"; // We want to embed quietly
        #if OCTAVE_MAJOR_VERSION >= 4 && OCTAVE_MINOR_VERSION >= 2
            octave::cmdline_options embed_opts(n_args, embed_args.c_str_vec());
            static octave::embedded_application interpreter(embed_opts);
            interpreter.execute();
        #else
            octave_main(n_args, embed_args.c_str_vec(), 1);
        #endif
        is_octave_embedded = true;
        octave_has_ever_been_embedded = true;
    }
    return is_octave_embedded;
}

// This exits from Octave and "toggles off" our indicator for whether
// Octave is currently embedded.
// [[Rcpp::export(.exit_octave)]]
bool exit_octave(bool verbose) {
    if ( octave_is_embedded() ) {
        octave_exit = 0;
        is_octave_embedded = false;
        if ( verbose ) {
            Rcpp::Rcout << "Exited Octave.\n";
        }
    }
    return is_octave_embedded;
}

