#ifndef GPMLR_H
#define GPMLR_H

#include <Rcpp.h>
#include <octave/oct.h> // For basic Octave types
#include <octave/ov-struct.h> // For octave_map
#include <octave/parse.h> // To call M files
#include "define-version.h"


// ----------------------- Octave embedding ----------------------------------
// Checks whether Octave is currently embedded:
bool octave_is_embedded();
bool octave_has_ever_been_embedded();
// Embeds Octave:
bool embed_octave(bool verbose = true, bool force = false);
// Shuts down Octave:
bool exit_octave(bool verbose = true);


// ------------------- Calling Octave functions ------------------------------
// The Octave C++ API provides the function feval() to call Octave functions.
// Eventually they move it to the Octave namespace.
// So, we define our own OCT() function that calls octave::feval()
// for some Octave versions and feval() otherwise.
#ifdef OCTAVE_4_4_OR_HIGHER
    inline octave_value_list OCT(const std::string& name,
                                 const octave_value_list& args = octave_value_list(),
                                 int nargout = 0) {
        return octave::feval(name, args, nargout);
    }
#else
    inline octave_value_list OCT(const std::string& name,
                                 const octave_value_list& args = octave_value_list(),
                                 int nargout = 0) {
        return feval(name, args, nargout);
    }
#endif


// ------------- Viewing and manipulating Octave's load path -----------------
// Prints the Octave load path:
void print_path();
// Adds directories to the Octave load path:
void add_to_path(Rcpp::StringVector x);


// ------------ Converting data between Octave and Rcpp types ----------------
Rcpp::NumericVector octmat_to_rcppmat(const Matrix& x);
Matrix rcppmat_to_octmat(const Rcpp::NumericVector& x);
octave_map list_to_map(const Rcpp::List& x);
Rcpp::List map_to_list(const octave_scalar_map& x);
Cell list_to_cell(const Rcpp::List& x);

#endif

