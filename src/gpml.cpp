#include "gpmlr.h"
#include <octave/parse.h> // To call M files

// First usage: training.
// Returns "the negative log marginal likelihood" and "a struct of column 
// vectors of partial derivatives of the negative log marginal likelihood 
// w.r.t. mean/cov/lik hyperparameters" (from gp.m)
// [[Rcpp::export(.gpml1)]]
Rcpp::List gpml1(Rcpp::List hyperparameters,
                 std::string inffunc,
                 std::string meanfunc,
                 std::string likfunc,
                 std::string covfunc,
                 Rcpp::NumericVector x,
                 Rcpp::NumericVector y) {
    if ( !octave_is_embedded() ) {
        Rcpp::stop("You must call embed_octave() before this function.\n");
    }
    octave_map octave_hyperparameters = list_to_map(hyperparameters);
    Matrix octave_x = rcpp_matrix_to_octave(x);
    Matrix octave_y = rcpp_matrix_to_octave(y);
    octave_value_list in;
    in(0) = octave_value(octave_hyperparameters);
    in(1) = octave_value(inffunc);
    in(2) = octave_value(meanfunc);
    in(3) = octave_value(covfunc);
    in(4) = octave_value(likfunc);
    in(5) = octave_value(octave_x);
    in(6) = octave_value(octave_y);
    octave_value_list octave_result = feval("gp", in, 2);
    int N = octave_result.length();
    ColumnVector NLZ = octave_result(0).column_vector_value();
    Rcpp::NumericVector nlz = octave_matrix_to_rcpp(NLZ);
    octave_value tmp = octave_result(1);
    octave_scalar_map DNLZ = tmp.scalar_map_value();
    Rcpp::List dnlz = octave_map_to_rcpp(DNLZ);
    tmp = octave_result(2);
    octave_scalar_map POST = tmp.scalar_map_value();
    Rcpp::List post = octave_map_to_rcpp(POST);
    return Rcpp::List::create(Rcpp::_["NLZ"] = nlz,
                              Rcpp::_["DNLZ"] = dnlz,
                              Rcpp::_["POST"] = post);
}

// Second usage: prediction.
// Returns column vectors of predictive output means, predictive output
// variances, predictive latent means, and predictive latent variances
// (from gp.m)
// [[Rcpp::export(.gpml2)]]
Rcpp::List gpml2(Rcpp::List hyperparameters,
                 std::string inffunc,
                 std::string meanfunc,
                 std::string likfunc,
                 std::string covfunc,
                 Rcpp::NumericVector training_x,
                 Rcpp::NumericVector training_y,
                 Rcpp::NumericVector testing_x) {
    if ( !octave_is_embedded() ) {
        Rcpp::stop("You must call embed_octave() before this function.\n");
    }
    octave_map octave_hyperparameters = list_to_map(hyperparameters);
    Matrix octave_training_x = rcpp_matrix_to_octave(training_x);
    Matrix octave_training_y = rcpp_matrix_to_octave(training_y);
    Matrix octave_testing_x = rcpp_matrix_to_octave(testing_x);
    octave_value_list in;
    in(0) = octave_value(octave_hyperparameters);
    in(1) = octave_value(inffunc);
    in(2) = octave_value(meanfunc);
    in(3) = octave_value(covfunc);
    in(4) = octave_value(likfunc);
    in(5) = octave_value(octave_training_x);
    in(6) = octave_value(octave_training_y);
    in(7) = octave_value(octave_testing_x);
    octave_value_list octave_result = feval("gp", in, 1);
    ColumnVector YMU = octave_result(0).column_vector_value();
    Rcpp::NumericVector ymu = octave_matrix_to_rcpp(YMU);
    ColumnVector YS2 = octave_result(1).column_vector_value();
    Rcpp::NumericVector ys2 = octave_matrix_to_rcpp(YS2);
    ColumnVector FMU = octave_result(2).column_vector_value();
    Rcpp::NumericVector fmu = octave_matrix_to_rcpp(FMU);
    ColumnVector FS2 = octave_result(3).column_vector_value();
    Rcpp::NumericVector fs2 = octave_matrix_to_rcpp(FS2);
    // The fifth value (element 4) will be empty
    octave_value tmp = octave_result(5);
    octave_scalar_map POST = tmp.scalar_map_value();
    Rcpp::List post = octave_map_to_rcpp(POST);
    return Rcpp::List::create(Rcpp::_["YMU"] = ymu,
                              Rcpp::_["YS2"] = ys2,
                              Rcpp::_["FMU"] = fmu,
                              Rcpp::_["FS2"] = fs2,
                              Rcpp::_["POST"] = post);
}

// Third usage: prediction.
// Returns column vectors of predictive output means, predictive output
// variances, predictive latent means, predictive latent variances, and
// log predictive probabilities (from gp.m)
// [[Rcpp::export(.gpml3)]]
Rcpp::List gpml3(Rcpp::List hyperparameters,
                 std::string inffunc,
                 std::string meanfunc,
                 std::string likfunc,
                 std::string covfunc,
                 Rcpp::NumericVector training_x,
                 Rcpp::NumericVector training_y,
                 Rcpp::NumericVector testing_x,
                 Rcpp::NumericVector testing_y) {
    if ( !octave_is_embedded() ) {
        Rcpp::stop("You must call embed_octave() before this function.\n");
    }
    octave_map octave_hyperparameters = list_to_map(hyperparameters);
    Matrix octave_training_x = rcpp_matrix_to_octave(training_x);
    Matrix octave_training_y = rcpp_matrix_to_octave(training_y);
    Matrix octave_testing_x = rcpp_matrix_to_octave(testing_x);
    Matrix octave_testing_y = rcpp_matrix_to_octave(testing_y);
    octave_value_list in;
    in(0) = octave_value(octave_hyperparameters);
    in(1) = octave_value(inffunc);
    in(2) = octave_value(meanfunc);
    in(3) = octave_value(covfunc);
    in(4) = octave_value(likfunc);
    in(5) = octave_value(octave_training_x);
    in(6) = octave_value(octave_training_y);
    in(7) = octave_value(octave_testing_x);
    in(8) = octave_value(octave_testing_y);
    octave_value_list octave_result = feval("gp", in, 1);
    ColumnVector YMU = octave_result(0).column_vector_value();
    Rcpp::NumericVector ymu = octave_matrix_to_rcpp(YMU);
    ColumnVector YS2 = octave_result(1).column_vector_value();
    Rcpp::NumericVector ys2 = octave_matrix_to_rcpp(YS2);
    ColumnVector FMU = octave_result(2).column_vector_value();
    Rcpp::NumericVector fmu = octave_matrix_to_rcpp(FMU);
    ColumnVector FS2 = octave_result(3).column_vector_value();
    Rcpp::NumericVector fs2 = octave_matrix_to_rcpp(FS2);
    ColumnVector LP = octave_result(4).column_vector_value();
    Rcpp::NumericVector lp = octave_matrix_to_rcpp(LP);
    octave_value tmp = octave_result(5);
    octave_scalar_map POST = tmp.scalar_map_value();
    Rcpp::List post = octave_map_to_rcpp(POST);
    return Rcpp::List::create(Rcpp::_["YMU"] = ymu,
                              Rcpp::_["YS2"] = ys2,
                              Rcpp::_["FMU"] = fmu,
                              Rcpp::_["FS2"] = fs2,
                              Rcpp::_["LP"] = lp,
                              Rcpp::_["POST"] = post);
}

