#include <Rcpp.h>
#include <octave/oct.h> // For basic Octave types
#include <octave/octave.h> // For octave_main or interpreter class (embedding)
#include "define-version.h" // To handle Octave version differences
#ifdef OCTAVE_4_2_OR_HIGHER
    #include <octave/interpreter.h> // To exit embedded Octave
#else
    #include <octave/toplev.h> // To exit embedded Octave
#endif
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
    // If currently embedded, note that and return true
    if ( octave_is_embedded() ) {
        if ( verbose ) {
            Rcpp::Rcout << "Octave is already embedded, skipping embedding.\n";
        }
        return true;
    }
    // If not currently embedded, but had been previously, and force is false,
    else if ( octave_has_ever_been_embedded && !force ) {
        // warn about its effects if verbose is true,
        if ( verbose ) {
            Rcpp::Rcout << "Octave was previously embedded.\n"
                        << "Embedding again will cause a stream of warnings, "
                        << "and is not generally supported.\n"
                        << "To do it anyway, try again with force = TRUE.\n";
        }
        // and return false.
        return false;
    }
    else {
        // Start the process of embedding, noisily if with verbose
        if ( verbose ) {
            Rcpp::Rcout << "Embedding Octave...\n";
        }
        // For Octave >= 4.4, we can call octave_main with no real arguments
        #ifdef OCTAVE_4_4_OR_HIGHER
            char** dummy_options;
            octave_main(0, dummy_options, true);
        #else
            // Otherwise we need actual arguments
            int n_args = 2;
            string_vector embed_args(n_args);
            embed_args(0) = "gpmlr"; // The name of our application
            embed_args(1) = "-q"; // We want to embed quietly
            #ifdef OCTAVE_4_2_OR_HIGHER
                // >= 4.2 & < 4.4 uses embedded_application + interpreter
                octave::cmdline_options opts(n_args, embed_args.c_str_vec());
                static octave::embedded_application interpreter(opts);
                interpreter.execute();
            #else
                // < 4.2 uses octave_main
                octave_main(n_args, embed_args.c_str_vec(), true);
            #endif
        #endif
        // toggle our indicators
        is_octave_embedded = true;
        octave_has_ever_been_embedded = true;
    }
    // If Octave is embedded, let the user know
    if ( verbose && octave_is_embedded() ) {
        Rcpp::Rcout << "Octave embedded.\n";
    }
    return is_octave_embedded;
}

// This exits from Octave and "toggles off" our indicator for whether
// Octave is currently embedded.
// [[Rcpp::export(.exit_octave)]]
bool exit_octave(bool verbose) {
    // (Only) if Octave is embedded
    if ( octave_is_embedded() ) {
        // Exit it
        #ifdef OCTAVE_4_4_OR_HIGHER
            // Starting with 4.4, we use octave_quit()
            octave_quit();
        #else
            // Otherwise we just set octave_exit to 0.
            // We also have to deal with a bug that caused segfault on exit
            // for some versions of Octave with symbol_table::clear_all()
            symbol_table::clear_all(true);
            octave_exit = 0;
        #endif
        // toggle indicator
        is_octave_embedded = false;
        // speak up if verbose
        if ( verbose ) {
            Rcpp::Rcout << "Exited Octave.\n";
        }
    }
    // return state of embedding
    return is_octave_embedded;
}

