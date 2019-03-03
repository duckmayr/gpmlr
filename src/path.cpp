#include "gpmlr.h"

// We need to be able to manipulate the Octave load path, as Octave can't call
// functions from an M file unless the directory it's in is on the load path

// Prints the Octave load path to stdout
// (after checking whether Octave is embedded first to avoid segfault)
// [[Rcpp::export(.print_path)]]
void print_path() {
    if ( octave_is_embedded() ) {
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

