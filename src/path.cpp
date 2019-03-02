#include "gpmlr.h"

// [[Rcpp::export(.print_path)]]
void print_path() {
    if ( octave_is_embedded() ) {
        OCT("path");
    }
    else {
        Rcpp::stop("You must call embed_octave() before this function.\n");
    }
}

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

