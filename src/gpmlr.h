#ifndef GPMLR_H
#define GPMLR_H

#include <Rcpp.h>
#include <octave/oct.h> // For basic Octave types
#include <octave/ov-struct.h> // For octave_map


// ----------------------- Octave embedding ----------------------------------
// Checks whether Octave is currently embedded:
bool octave_is_embedded();
// Embeds Octave:
bool embed_octave(bool verbose = true, bool force = false);
// Shuts down Octave:
bool exit_octave(bool verbose = true);


// ------------- Viewing and manipulating Octave's load path -----------------
// Prints the Octave load path:
void print_path();
// Adds directories to the Octave load path:
void add_to_path(Rcpp::StringVector x);

// ------------ Converting data between Octave and Rcpp types ----------------
/*
Rcpp::NumericMatrix octave_matrix_to_rcpp(const Matrix& x);
Matrix rcpp_matrix_to_octave(const Rcpp::NumericMatrix& x);
Rcpp::NumericVector octave_vector_to_rcpp(const ColumnVector& x);
ColumnVector rcpp_vector_to_octave(const Rcpp::NumericVector& x);
*/
Rcpp::NumericVector octave_matrix_to_rcpp(const Matrix& x);
Matrix rcpp_matrix_to_octave(const Rcpp::NumericVector& x);
octave_map list_to_map(const Rcpp::List& x);
Rcpp::List octave_map_to_rcpp(const octave_scalar_map& x);

#endif

