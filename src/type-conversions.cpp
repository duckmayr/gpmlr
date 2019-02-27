#include <Rcpp.h>
#include <oct.h> // For basic Octave types
#include <ov-struct.h> // For octave_map

// CONVERSIONS BETWEEN MATRICES

// Technically we only have to deal with column vectors and matrices here.
// From an Rcpp perspective, numeric matrices are numeric vectors,
// just with a "dim" attribute. From an Octave perspective, column vectors
// are also arrays (with n rows and one column). So, we can simplify a bit
// here and not deal with general cases.
Rcpp::NumericVector octave_matrix_to_rcpp(const Matrix& x) {
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

Matrix rcpp_matrix_to_octave(const Rcpp::NumericVector& x) {
    int n, m;
    if ( x.hasAttribute("dim") ) {
        Rcpp::IntegerVector dims = x.attr("dim");
        n = dims(0);
        m = dims(1);
    }
    else {
        n = x.size();
        m = 1;
    }
    Matrix result(n, m);
    int N = x.size();
    for ( int i = 0; i < N; ++i ) {
        result(i) = x(i);
    }
    return result;
}

// CONVERSIONS BETWEEN LISTS AND MAPS

octave_map list_to_map(const Rcpp::List& x) {
    // XXX : THIS ONLY HANDLES LISTS OF NUMERICVECTORS
    Rcpp::CharacterVector rcpp_xnames = x.names();
    std::vector<std::string> xnames = Rcpp::as<std::vector<std::string> >(rcpp_xnames);
    octave_map result;
    for ( int i = 0; i < x.size(); ++i ) {
        std::string tmp_name = xnames[i];
        Rcpp::NumericVector rcpp_tmp_value = x[tmp_name];
        octave_value octave_tmp_value = rcpp_matrix_to_octave(rcpp_tmp_value);
        result.assign(tmp_name, octave_tmp_value);
    }
    return result;
}

Rcpp::List octave_map_to_rcpp(const octave_scalar_map& x) {
    // FIXME: Compiles fine, but when called from gpml1(), we get
    // error: octave_base_value::map_value(): wrong type argument 'cell'
    string_vector octave_xnames = x.fieldnames();
    int n = octave_xnames.nelem();
    Rcpp::List result(n);
    Rcpp::CharacterVector rcpp_names(n);
    for ( int i = 0; i < n; ++i ) {
        std::string this_field_name = octave_xnames(i);
        octave_value octave_tmp_val = x.getfield(this_field_name);
        Matrix octave_mat_tmp = octave_tmp_val.matrix_value();
        Rcpp::NumericVector rcpp_tmp = octave_matrix_to_rcpp(octave_mat_tmp);
        result(i) = rcpp_tmp;
        rcpp_names(i) = this_field_name;
    }
    result.names() = rcpp_names;
    return result;
}

