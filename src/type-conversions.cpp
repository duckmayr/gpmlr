#include "gpmlr.h"
#include <ov-cell.h>
#include <ov-fcn-handle.h>

// String conversions
string_vector to_string_vector(const Rcpp::CharacterVector& x) {
    return Rcpp::as<std::vector<std::string> >(x);
}

// CONVERSIONS BETWEEN MATRICES

// Technically we only have to deal with column vectors and matrices here.
// From an Rcpp perspective, numeric matrices are numeric vectors,
// just with a "dim" attribute. From an Octave perspective, column vectors
// are also arrays (with n rows and one column). So, we can simplify a bit
// here and not deal with general cases.
Rcpp::NumericVector octmat_to_rcppmat(const Matrix& x) {
    int n = x.rows();
    int m = x.cols();
    Rcpp::NumericMatrix result(n, m);
    for ( int j = 0; j < m; ++j ) {
        for ( int i = 0; i < n; ++i ) {
            result(i, j) = x(i, j);
        }
    }
    return result;
}

Matrix rcppmat_to_octmat(const Rcpp::NumericVector& x) {
    Rcpp::NumericMatrix mat_in;
    if ( x.size() == 0 ) {  // If x is empty
                            // skip the next couple blocks
    }
    else if ( x.hasAttribute("dim") ) { // If x is really a NumericMatrix
        mat_in = Rcpp::as<Rcpp::NumericMatrix>(x);
    }
    else { // If x is really just a NumericVector
        int N = x.size();
        mat_in = Rcpp::NumericMatrix(N, 1);
        for ( int i = 0; i < N; ++i ) {
            mat_in(i, 0) = x(i);
        }
    }
    int n = mat_in.nrow();
    int m = mat_in.ncol();
    Matrix result(n, m);
    for ( int j = 0; j < m; ++j ) {
        for ( int i = 0; i < n; ++i ) {
            result(i, j) = mat_in(i, j);
        }
    }
    return result;
}

// CONVERSIONS TO AND FROM LISTS

octave_map list_to_map(const Rcpp::List& x) {
    // TODO: For now, this just deals with lists of numeric vectors/matrices.
    //       That is all I think we need for dealing with hyperparameters,
    //       but if we need something more general this could be fixed up
    //       easily enough along the same lines as list_to_cell.
    //       I do not yet implement that in case a user could trip up Octave
    //       by taking advantage of such generality.
    // GPML's gp() needs the field names
    Rcpp::CharacterVector rcpp_xnames = x.names();
    // But we can't use a CharacterVector to set them; need a string_vector
    string_vector xnames = to_string_vector(rcpp_xnames);
    octave_map result;
    for ( int i = 0; i < x.size(); ++i ) {
        // Get this element's name
        std::string tmp_name = xnames[i];
        // Get the NumericVector
        Rcpp::NumericVector tmp_vec = Rcpp::as<Rcpp::NumericVector>(x(i));
        // Convert it to a value Octave understands
        octave_value octave_tmp_value = rcppmat_to_octmat(tmp_vec);
        // Then we use the element name, octave_value, and octave_map member
        // function assign() to put the value in the map with the fieldname
        result.assign(tmp_name, octave_tmp_value);
    }
    return result;
}


Rcpp::List map_to_list(const octave_scalar_map& x) {
    // Get the field names
    string_vector octave_xnames = x.fieldnames();
    // To determine the number of elements, we use length() in versions < 4.2
    // or numel() in versions >= 4.2 to avoid additional deprecated warnings
    #ifdef OCTAVE_4_2_OR_HIGHER
        int n = octave_xnames.numel();
    #else
        int n = octave_xnames.length();
    #endif
    // Set up Rcpp objects to store things in
    Rcpp::List result(n);
    Rcpp::CharacterVector rcpp_names(n);
    for ( int i = 0; i < n; ++i ) { // for each element
        // Get this element and its field name
        std::string this_field_name = octave_xnames(i);
        octave_value octave_tmp_val = x.getfield(this_field_name);
        // It should usually be a matrix/column vector, but some inference
        // methods return a function, so we have to deal with that.
        // Get the value type
        std::string tmp_val_type = octave_tmp_val.type_name();
        if ( tmp_val_type == "matrix" || tmp_val_type == "scalar" ) {
            // If it's a Matrix, convert it to a NumericVector & put in result
            Matrix oct_mat_tmp = octave_tmp_val.matrix_value();
            Rcpp::NumericVector rcpp_tmp = octmat_to_rcppmat(oct_mat_tmp);
            result(i) = rcpp_tmp;
        }
        // Otherwise it should be a function handle
        else if ( tmp_val_type == "function handle" ) {
            // It won't have a name, but we can get its string representation
            octave_fcn_handle* octave_tmp = octave_tmp_val.fcn_handle_value();
            std::ostringstream os;
            octave_tmp->print(os);
            std::string returned_fcn = os.str();
            // The only issue is it usually (always?) has a trailling newline
            returned_fcn.erase(returned_fcn.find_last_not_of("\n") + 1);
            result(i) = returned_fcn;
        }
        // If neither of those occur, this element will simply be a named NULL
        rcpp_names(i) = this_field_name;
    }
    result.names() = rcpp_names;
    return result;
}

// This is an easy but perhaps inefficient solution for now to deal with
// the fact that we could have nested cell arrays of function names
Cell list_to_cell(const Rcpp::List& x) {
    int n = x.size();
    octave_value_list tmp_oct_object(n);
    for ( int i = 0; i < n; ++i ) {
        Rcpp::RObject tmp_R_object = x[i];
        int sexp_type = tmp_R_object.sexp_type();
        switch ( sexp_type ) {
            case 13 : case 14 : {
                // Turn numeric or integer vector (constant) into Matrix
                Rcpp::NumericVector list_val;
                list_val = Rcpp::as<Rcpp::NumericVector>(tmp_R_object);
                Matrix cell_val = rcppmat_to_octmat(list_val);
                tmp_oct_object(i) = cell_val;
                break;
            }
            case 16 : {
                // Turn character vector w/ function name into string_vector
                Rcpp::CharacterVector list_val;
                list_val = Rcpp::as<Rcpp::CharacterVector>(tmp_R_object);
                string_vector cell_val = to_string_vector(list_val);
                tmp_oct_object(i) = cell_val;
                break;
            }
            case 19 : {
                // If we run into a list of lists, recurse
                Rcpp::List tmp_list = Rcpp::as<Rcpp::List>(tmp_R_object);
                tmp_oct_object(i) = list_to_cell(tmp_list);
                break;
            }
            default :
                // If it's not one of those things, we don't know what to do
                Rcpp::stop("Failed conversion from Rcpp::List to Cell");
        }
    }
    Cell result(tmp_oct_object);
    return result;
}

