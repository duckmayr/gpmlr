#' Set Hyperparameters
#'
#' \code{set_hyperparameters} allows the user to use GPML's Matlab function
#' \code{minimize()} to set the hyperparameters by optimizing the likelihood.
#' 
#' @param hyp A list of length three giving initial hyperparameters for the
#'   mean, covariance, and likelihood functions
#' @param inf A character vector or list giving the inference method
#' @param mean A character vector or list giving the mean function
#' @param cov A character vector or list giving the covariance function
#' @param lik A character vector or list giving the likelihood function
#' @param x A numeric vector or matrix of training inputs
#' @param y A numeric vector of training outcomes
#' @param n_evals An integer vector of length one giving the maximum number
#'   of function evaluations (default is 100)
#'
#' @return A list giving the hyperparameters
#' @examples
#' ## This example is from on the GPML website.
#' ## Here's how you can run it from R.
#' set.seed(123)
#' x <- rnorm(20, 0.8, 1)
#' y <- sin(3 * x) + 0.1 * rnorm(20, 0.9, 1)
#' hyp <- list(mean = numeric(), cov = c(0, 0), lik = -1)
#' set_hyperparameters(hyp, "infExact", "", "covSEiso", "likGauss", x, y)
#' @export
set_hyperparameters <- function(hyp, inf, mean, cov, lik, x, y,
                                n_evals = 100) {
    # Make sure Octave is embedded and set up.
    # If gpmlr is attached, this shouldn't be an issue,
    # but we check in case gpmlr::gp() is called without attaching.
    if ( !.octave_is_embedded() ) {
        suppressPackageStartupMessages(setup_Octave())
        message("Octave embedded.")
    }
    # Do some processing of the parameters
    inf <- listfix(inf)
    mean <- listfix(mean)
    cov <- listfix(cov)
    lik <- listfix(lik)
    if ( mean[[1]] == "" ) {
        mean[[1]] <- "meanZero"
    }
    # Workaround for GPML bug -- make sure it's called from GPML directory
    wd <- getwd()
    .set_wd(system.file("gpml", package = "gpmlr"))
    # Set the hyperparameters
    result <- .set_hyperparameters(hyp, inf, mean, cov, lik, x, y, -n_evals)
    # Reset working directory and return
    setwd(wd)
    return(result)
}

