#include "gpmlr.h"

// [[Rcpp::export(.set_hyperparameters)]]
Rcpp::List set_hyperparameters(Rcpp::List hyp,
                               Rcpp::List inf,
                               Rcpp::List mean,
                               Rcpp::List cov,
                               Rcpp::List lik,
                               Rcpp::NumericVector x,
                               Rcpp::NumericVector y,
                               int n_evals) {
    // Convert the arguments to values Octave can understand
    octave_map octave_hyperparameters = list_to_map(hyp);
    Cell inf_func = list_to_cell(inf);
    Cell mean_func = list_to_cell(mean);
    Cell lik_func = list_to_cell(lik);
    Cell cov_func = list_to_cell(cov);
    Matrix octave_x = rcppmat_to_octmat(x);
    Matrix octave_y = rcppmat_to_octmat(y);
    // Create the list of arguments going into the Octave function
    octave_value_list in;
    in(0) = octave_value(octave_hyperparameters);
    in(1) = octave_value("gp");
    in(2) = octave_value(n_evals);
    in(3) = octave_value(inf_func);
    in(4) = octave_value(mean_func);
    in(5) = octave_value(cov_func);
    in(6) = octave_value(lik_func);
    in(7) = octave_value(octave_x);
    in(8) = octave_value(octave_y);
    // Call GPML's Octave function minimize() (quietly)
    std::cout.setstate(std::ios_base::failbit);
    octave_value_list octave_result = OCT("minimize", in, 1);
    std::cout.clear();
    // Convert the result into an Rcpp::List and update hyp
    octave_value tmp = octave_result(0);
    octave_scalar_map HYP = tmp.scalar_map_value();
    Rcpp::List result = map_to_list(HYP);
    return result;
}

