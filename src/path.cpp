#include "gpmlr.h"

// We need to be able to manipulate the Octave load path, as Octave can't call
// functions from an M file unless the directory it's in is on the load path

// Prints the Octave load path to Rcout
// (after checking whether Octave is embedded first to avoid segfault)
// [[Rcpp::export(.print_path)]]
void print_path() {
    static bool cout_redirect_needed = true;
    if ( octave_is_embedded() ) {
        // Unfortunately calling Octave's path will print to cout, while we
        // would really prefer it to go to Rcout. So, we redirect cout to
        // Rcout. This approach ironically causes a 'Note' in R CMD check that
        // "Compiled code should not call entry points which might... write to
        // stdout/stderr instead of to the console....", even though we're
        // doing it for the opposite reason.
        if ( cout_redirect_needed ) { // Redirect cout if we haven't already
            std::cout.rdbuf(Rcpp::Rcout.rdbuf());
            cout_redirect_needed = false;
        }
        OCT("path");
    }
    else {
        Rcpp::stop("You must call embed_octave() before this function.\n");
    }
}

// Adds directories to the Octave load path
// (after checking whether Octave is embedded first to avoid segfault)
// [[Rcpp::export(.add_to_path)]]
void add_to_path(Rcpp::StringVector x) {
    if ( octave_is_embedded() ) {
        int n = x.size();
        octave_value octave_path_string;
        for ( int i = 0; i < n; ++i ) {
            octave_path_string = Rcpp::as<std::string>(x(i));
            OCT("addpath", octave_path_string);
        }
    }
    else {
        Rcpp::stop("You must call embed_octave() before this function.\n");
    }
}

