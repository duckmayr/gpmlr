# Helper function to process data for gp()
# (our C++ to Octave type converter assumes function names are in lists)
listfix <- function(x) {
    if ( !is.list(x) ) {
        x <- list(x)
    }
    return(x)
}

#' Gaussian Process Inference and Prediction
#'
#' \code{gp} allows the user to call GPML's Matlab function for Gaussian
#' process inference and prediction.
#' 
#' GPML provides several inference methods, as well as several mean,
#' covariance, and likelihood functions to be used in its \code{gp()}
#' Matlab function. Their detailed manual is available at
#' \url{http://www.gaussianprocess.org/gpml/code/matlab/doc/manual.pdf}.
#' 
#' @param hyp A list of length three giving the hyperparameters for the mean,
#'   covariance, and likelihood functions
#' @param inf A character vector or list giving the inference method
#' @param mean A character vector or list giving the mean function
#' @param cov A character vector or list giving the covariance function
#' @param lik A character vector or list giving the likelihood function
#' @param x A numeric vector or matrix of training inputs
#' @param y A numeric vector of training outcomes
#' @param xs A numeric vector or matrix of testing inputs
#' @param ys A numeric vector of testing outcomes
#'
#' @return A list whose elements depend on the arguments provided to the
#'   function call:
#'   \itemize{
#'       \item If called in training mode, that is, with y, xs, and ys missing,
#'           the returned list has three elements: NLZ, the negative log
#'           marginal likelihood, DNLZ, a list of vectors giving the partial
#'           derivatives of the negative log marginal likelihood with respect
#'           to the mean, covariance, and likelihood parameters, and POST,
#'           a list of three elements, alpha, sW, and L, giving a
#'           representation of the approximate posterior.
#'       \item If called in prediction mode with xs supplied by ys missing,
#'           the returned list has five elements: a vector YMU giving the
#'           predictive output means, a vector YS2 giving the predictive
#'           output variances, a vector FMU giving the predictive latent means,
#'           a vector FS2 giving the perdictive latent variances, and POST,
#'           a list of three elements, alpha, sW, and L, giving a
#'           representation of the approximate posterior.
#'       \item If called in prediction mode with all arguments supplied,
#'           the returned list has six elements: the five elements given when
#'           ys is missing, as well as a vector LP of logged predictive
#'           probabilities.
#'   }
#' @examples
#' ## This example are given on the GPML website.
#' ## Here's how you can run them from R.
#' ## First consider an example with simple mean and covariance functions:
#' set.seed(123)
#' x <- rnorm(20, 0.8, 1)
#' y <- sin(3 * x) + 0.1 * rnorm(20, 0.9, 1)
#' xs <- seq(-3, 3, length.out = 61)
#' hyp <- list(mean = numeric(), cov = c(0, 0), lik = -1)
#' gp_result <- gp(hyp, "infExact", "", "covSEiso", "likGauss", x, y, xs)
#' str(gp_result)
#' plot(xs, gp_result$YMU, type = "l",
#'      xlab = "x", ylab = "Predictive Output Mean")
#' @export
gp <- function(hyp, inf, mean, cov, lik, x, y, xs, ys) {
    # Make sure Octave is embedded and set up.
    # If gpmlr is attached, this shouldn't be an issue,
    # but we check in case gpmlr::gp() is called without attaching.
    if ( !.octave_is_embedded() ) {
        suppressPackageStartupMessages(setup_Octave())
        message("Octave embedded. Calling gp().")
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
    # Call the appropriate form of gp()
    if ( missing(xs) ) {
        result <- .gpml1(hyp, inf, mean, cov, lik, x, y)
    } else if ( missing(ys) ) {
        result <- .gpml2(hyp, inf, mean, cov, lik, x, y, xs)
    } else {
        result <- .gpml3(hyp, inf, mean, cov, lik, x, y, xs, ys)
    }
    # Reset working directory and return
    setwd(wd)
    return(result)
}

