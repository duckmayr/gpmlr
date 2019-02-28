#' Gaussian Process Inference and Prediction
#'
#' \code{gp} allows the user to call GPML's Matlab function for Gaussian
#' process inference and prediction.
#'
#' @param hyp A list of length three giving the hyperparameters for the mean,
#'   covariance, and likelihood functions
#' @param inf A character vector of length one giving the name of an inference
#'   method (see details)
#' @param mean A character vector of length one giving the name of an mean
#'   function (see details)
#' @param cov A character vector of length one giving the name of an mean
#'   function (see details)
#' @param lik A character vector of length one giving the name of an mean
#'   function (see details)
#' @param x A numeric vector or matrix of training inputs
#' @param y A numeric vector of training outcomes
#' @param xs A numeric vector or matrix of testing inputs
#' @param ys A numeric vector of testing outcomes
#'
#' @return A list whose elements depend on the arguments provided to the
#'   function call:
#'   \itemize{
#'       \item If called in training mode, that is, with y, xs, and ys missing,
#'           the returned list has two elements: NLZ, the negative log marginal
#'           likelihood, and DNLZ, a list of vectors giving the partial
#'           derivatives of the negative log marginal likelihood with respect
#'           to the mean, covariance, and likelihood parameters.
#'       \item If called in prediction mode with xs supplied by ys missing,
#'           the returned list has four elements: a vector YMU giving the
#'           predictive output means, a vector YS2 giving the predictive
#'           output variances, a vector FMU giving the predictive latent means,
#'           and a vector FS2 giving the perdictive latent variances.
#'       \item If called in prediction mode with all arguments supplied,
#'           the returned list has five elements: the four elements given when
#'           ys is missing, as well as a vector LP of logged predictive
#'           probabilities.
#'   }
#' @examples
#' ## This example is given on the GPML website.
#' ## Here's how we'd run the same example from R.
#' set.seed(123)
#' x <- rnorm(20, 0.8, 1)
#' y <- sin(3 * x) + 0.1 * rnorm(20, 0.9, 1)
#' xs <- seq(-3, 3, length.out = 61)
#' hyp <- list(mean = numeric(), cov = c(0, 0), lik = -1)
#' gp_res <- gp(hyp, "infGaussLik", "", "likGauss", "covSEiso", x, y, xs)
#' str(gp_res)
#' plot(xs, gp_res$YMU)
#' @export
gp <- function(hyp, inf, mean, cov, lik, x, y, xs, ys) {
    if ( !.octave_is_embedded() ) {
        .embed_octave(FALSE, FALSE)
        sub_dirs <- c("cov", "doc", "inf", "lik", "mean", "prior", "util")
        dirs_to_add <- system.file(paste0("gpml", c("", paste0("/", sub_dirs))),
                                   package = "gpmlr")
        .add_to_path(dirs_to_add)
    }
    if ( missing(xs) ) {
        return(.gpml1(hyp, inf, mean, cov, lik, x, y))
    }
    if ( missing(ys) ) {
        return(.gpml2(hyp, inf, mean, cov, lik, x, y, xs))
    }
    return(.gpml3(hyp, inf, mean, cov, lik, x, y, xs, ys))
}
