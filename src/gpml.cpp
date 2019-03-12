#include "gpmlr.h"

// First usage: training.
// [[Rcpp::export(.gpml1)]]
Rcpp::List gpml1(Rcpp::List hyperparameters,
                 Rcpp::List inffunc,
                 Rcpp::List meanfunc,
                 Rcpp::List covfunc,
                 Rcpp::List likfunc,
                 Rcpp::NumericVector x,
                 Rcpp::NumericVector y) {
    // Make sure Octave is embedded
    if ( !octave_is_embedded() ) {
        Rcpp::stop("You must call embed_octave() before this function.\n");
    }
    // Convert the arguments to values Octave can understand
    octave_map octave_hyperparameters = list_to_map(hyperparameters);
    Cell inf_func = list_to_cell(inffunc);
    Cell mean_func = list_to_cell(meanfunc);
    Cell lik_func = list_to_cell(likfunc);
    Cell cov_func = list_to_cell(covfunc);
    Matrix octave_x = rcppmat_to_octmat(x);
    Matrix octave_y = rcppmat_to_octmat(y);
    // Create the list of arguments going into the Octave function
    octave_value_list in;
    in(0) = octave_value(octave_hyperparameters);
    in(1) = octave_value(inf_func);
    in(2) = octave_value(mean_func);
    in(3) = octave_value(cov_func);
    in(4) = octave_value(lik_func);
    in(5) = octave_value(octave_x);
    in(6) = octave_value(octave_y);
    // Call GPML's Octave function gp()
    octave_value_list octave_result = OCT("gp", in, 2);
    // Convert the elements of the resulting octave_value_list
    // into objects that R will understand
    ColumnVector NLZ = octave_result(0).column_vector_value();
    Rcpp::NumericVector nlz = octmat_to_rcppmat(NLZ);
    octave_value tmp = octave_result(1);
    octave_scalar_map DNLZ = tmp.scalar_map_value();
    Rcpp::List dnlz = map_to_list(DNLZ);
    tmp = octave_result(2);
    octave_scalar_map POST = tmp.scalar_map_value();
    Rcpp::List post = map_to_list(POST);
    return Rcpp::List::create(Rcpp::_["NLZ"] = nlz,
                              Rcpp::_["DNLZ"] = dnlz,
                              Rcpp::_["POST"] = post);
}

// Second usage: prediction with test inputs.
// [[Rcpp::export(.gpml2)]]
Rcpp::List gpml2(Rcpp::List hyperparameters,
                 Rcpp::List inffunc,
                 Rcpp::List meanfunc,
                 Rcpp::List covfunc,
                 Rcpp::List likfunc,
                 Rcpp::NumericVector training_x,
                 Rcpp::NumericVector training_y,
                 Rcpp::NumericVector testing_x) {
    // Make sure Octave is embedded
    if ( !octave_is_embedded() ) {
        Rcpp::stop("You must call embed_octave() before this function.\n");
    }
    // Convert the arguments to values Octave can understand
    octave_map octave_hyperparameters = list_to_map(hyperparameters);
    Cell inf_func = list_to_cell(inffunc);
    Cell mean_func = list_to_cell(meanfunc);
    Cell lik_func = list_to_cell(likfunc);
    Cell cov_func = list_to_cell(covfunc);
    Matrix octave_training_x = rcppmat_to_octmat(training_x);
    Matrix octave_training_y = rcppmat_to_octmat(training_y);
    Matrix octave_testing_x = rcppmat_to_octmat(testing_x);
    // Create the list of arguments going into the Octave function
    octave_value_list in;
    in(0) = octave_value(octave_hyperparameters);
    in(1) = octave_value(inf_func);
    in(2) = octave_value(mean_func);
    in(3) = octave_value(cov_func);
    in(4) = octave_value(lik_func);
    in(5) = octave_value(octave_training_x);
    in(6) = octave_value(octave_training_y);
    in(7) = octave_value(octave_testing_x);
    // Call GPML's Octave function gp()
    octave_value_list octave_result = OCT("gp", in, 1);
    // Convert the elements of the resulting octave_value_list
    // into objects that R will understand
    ColumnVector YMU = octave_result(0).column_vector_value();
    Rcpp::NumericVector ymu = octmat_to_rcppmat(YMU);
    ColumnVector YS2 = octave_result(1).column_vector_value();
    Rcpp::NumericVector ys2 = octmat_to_rcppmat(YS2);
    ColumnVector FMU = octave_result(2).column_vector_value();
    Rcpp::NumericVector fmu = octmat_to_rcppmat(FMU);
    ColumnVector FS2 = octave_result(3).column_vector_value();
    Rcpp::NumericVector fs2 = octmat_to_rcppmat(FS2);
    // The fifth value (element 4) will be empty
    octave_value tmp = octave_result(5);
    octave_scalar_map POST = tmp.scalar_map_value();
    Rcpp::List post = map_to_list(POST);
    return Rcpp::List::create(Rcpp::_["YMU"] = ymu,
                              Rcpp::_["YS2"] = ys2,
                              Rcpp::_["FMU"] = fmu,
                              Rcpp::_["FS2"] = fs2,
                              Rcpp::_["POST"] = post);
}

// Third usage: prediction with test inputs and targets.
// [[Rcpp::export(.gpml3)]]
Rcpp::List gpml3(Rcpp::List hyperparameters,
                 Rcpp::List inffunc,
                 Rcpp::List meanfunc,
                 Rcpp::List covfunc,
                 Rcpp::List likfunc,
                 Rcpp::NumericVector training_x,
                 Rcpp::NumericVector training_y,
                 Rcpp::NumericVector testing_x,
                 Rcpp::NumericVector testing_y) {
    // Make sure Octave is embedded
    if ( !octave_is_embedded() ) {
        Rcpp::stop("You must call embed_octave() before this function.\n");
    }
    // Convert the arguments to values Octave can understand
    octave_map octave_hyperparameters = list_to_map(hyperparameters);
    Cell inf_func = list_to_cell(inffunc);
    Cell mean_func = list_to_cell(meanfunc);
    Cell lik_func = list_to_cell(likfunc);
    Cell cov_func = list_to_cell(covfunc);
    Matrix octave_training_x = rcppmat_to_octmat(training_x);
    Matrix octave_training_y = rcppmat_to_octmat(training_y);
    Matrix octave_testing_x = rcppmat_to_octmat(testing_x);
    Matrix octave_testing_y = rcppmat_to_octmat(testing_y);
    // Create the list of arguments going into the Octave function
    octave_value_list in;
    in(0) = octave_value(octave_hyperparameters);
    in(1) = octave_value(inf_func);
    in(2) = octave_value(mean_func);
    in(3) = octave_value(cov_func);
    in(4) = octave_value(lik_func);
    in(5) = octave_value(octave_training_x);
    in(6) = octave_value(octave_training_y);
    in(7) = octave_value(octave_testing_x);
    in(8) = octave_value(octave_testing_y);
    // Call GPML's Octave function gp()
    octave_value_list octave_result = OCT("gp", in, 1);
    // Convert the elements of the resulting octave_value_list
    // into objects that R will understand
    ColumnVector YMU = octave_result(0).column_vector_value();
    Rcpp::NumericVector ymu = octmat_to_rcppmat(YMU);
    ColumnVector YS2 = octave_result(1).column_vector_value();
    Rcpp::NumericVector ys2 = octmat_to_rcppmat(YS2);
    ColumnVector FMU = octave_result(2).column_vector_value();
    Rcpp::NumericVector fmu = octmat_to_rcppmat(FMU);
    ColumnVector FS2 = octave_result(3).column_vector_value();
    Rcpp::NumericVector fs2 = octmat_to_rcppmat(FS2);
    ColumnVector LP = octave_result(4).column_vector_value();
    Rcpp::NumericVector lp = octmat_to_rcppmat(LP);
    octave_value tmp = octave_result(5);
    octave_scalar_map POST = tmp.scalar_map_value();
    Rcpp::List post = map_to_list(POST);
    return Rcpp::List::create(Rcpp::_["YMU"] = ymu,
                              Rcpp::_["YS2"] = ys2,
                              Rcpp::_["FMU"] = fmu,
                              Rcpp::_["FS2"] = fs2,
                              Rcpp::_["LP"] = lp,
                              Rcpp::_["POST"] = post);
}

