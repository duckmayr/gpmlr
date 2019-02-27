#' gpmlr
#'
#' Allows the 'GPML' (Gaussian Processes for Machine Learning) Matlab function
#' for Gaussian process inference and prediction to be called from R, using R
#' syntax and data structures.
#'
#' @name gpmlr
#' @docType package
#' @author  JB Duck-Mayr
#' @useDynLib gpmlr
#' @importFrom Rcpp sourceCpp
NULL
.onUnload <- function (libpath) {
    .exit_octave(FALSE)
    library.dynam.unload('gpmlr', libpath)
}
