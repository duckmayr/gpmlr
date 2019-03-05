#include <Rcpp.h>
#include <octave/oct.h>             // For basic Octave types
#include <octave/octave.h>          // Some of the embedding code
#include "define-version.h"         // To handle Octave version differences
#ifdef OCTAVE_4_2_OR_HIGHER
    #include <octave/interpreter.h> // The rest of the embedding/exiting code
#else 
    #include <octave/toplev.h>      // Ditto (for older Octave versions)
#endif
#include <octave/symtab.h>


// We must keep track of whether Octave is or has ever been embedded, since:
// (1) calling Octave functions if Octave isn't embedded causes segfault; and
// (2) re-embedding causes *so many* warnings and may have other consequences.
// So, we keep track of those things with these variables:

bool is_octave_embedded = false;
bool has_octave_ever_been_embedded = false;

// And can check their value, even from R, with these functions:

// [[Rcpp::export(.octave_is_embedded)]]
bool octave_is_embedded() {
    return is_octave_embedded;
}
// [[Rcpp::export(.octave_has_ever_been_embedded)]]
bool octave_has_ever_been_embedded() {
    return has_octave_ever_been_embedded;
}


// embed_octave() embeds Octave if appropriate and returns is_octave_embedded.
// If Octave is embedded, it does not attempt to embed.
// If it is not, but has been previously, it only re-embeds if force is true.
// If it has never been embedded, it embeds Octave.

// [[Rcpp::export(.embed_octave)]]
bool embed_octave(bool verbose, bool force) {
    // If currently embedded, note that if verbose & return is_octave_embedded
    if ( octave_is_embedded() ) {
        if ( verbose ) {
            Rcpp::Rcout << "Octave is already embedded, skipping embedding.\n";
        }
        return is_octave_embedded;
    }
    // If not embedded but had been previously, and force is false, warn about
    // effects of re-embedding if verbose is true & return is_octave_embedded
    else if ( octave_has_ever_been_embedded() && !force ) {
        if ( verbose ) {
            Rcpp::Rcout << "Octave was previously embedded.\n"
                        << "Embedding again will cause a stream of warnings, "
                        << "and is not generally supported.\n"
                        << "To do it anyway, try again with force = TRUE.\n";
        }
        return is_octave_embedded;
    }
    else {
        // Start the process of embedding, noisily if with verbose
        if ( verbose ) {
            Rcpp::Rcout << "Embedding Octave...\n";
        }
        // Set embedding arguments:
        int n_args = 2;
        string_vector args(n_args);
        args(0) = "gpmlr"; // The name of our application
        args(1) = "-q";    // We want to embed quietly
        // We'll set is_octave_embedded w/ Octave's embedding function's retval
        #ifdef OCTAVE_4_2_OR_HIGHER
            #ifdef OCTAVE_4_4_OR_HIGHER
                // In Octave >= 4.4, we use octave::interpreter
                static octave::interpreter interpreter;
                // Return of 0 means successful embedding
                is_octave_embedded = !interpreter.execute();
            #else
                // In Octave >= 4.2 & < 4.4 we use octave::embedded_application
                octave::cmdline_options opts(n_args, args.c_str_vec());
                static octave::embedded_application interpreter(opts);
                // Return of 1 means successful embedding
                is_octave_embedded = interpreter.execute();
            #endif
        #else
            // In Octave < 4.2 we use octave_main
            // Return of 1 means successful embedding
            is_octave_embedded = octave_main(n_args, args.c_str_vec(), true);
        #endif
    }
    if ( octave_is_embedded() ) {
        has_octave_ever_been_embedded = true;
        if ( verbose ) {
            Rcpp::Rcout << "Octave embedded.\n";
        }
    }
    return is_octave_embedded;
}


// This exits Octave and "toggles off" our indicator for whether it's embedded.

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

