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
# We have to make sure Octave is terminated before we quit R,
# or it causes a segfault. At first I tried to do this in .onUnload(),
# but apparently that's not good enough. So, we'll use reg.finalizer()
# (which I found out about reading help("quit") trying to figure out
# why .onUnload() wasn't working). First we set up an environment,
# even an empty one, that reg.finalizer can be called on, and a
# function that will be triggered on its garbage collection (either
# at the end of the R session, or when the namespace is unloaded)
# (see help("reg.finalizer")):
.gpmlr_package_loaded <- environment()
.exit_octave_on_quit <- function(e) try(.exit_octave(FALSE), silent = TRUE)
# Note that I've wrapped it in try().
# This is because in some (rare, I hope?) situations,
# library.dynam.unload('gpmlr', libpath) could be called before
# .exit_octave_on_quit(.gpmlr_package_loaded).
# I have only observed this happen when using devtools functions that
# reload the package using reload(); related traceback:
# Error in .Call("_gpmlr_exit_octave", PACKAGE = "gpmlr", verbose) :
#   "_gpmlr_exit_octave" not available for .Call() for package "gpmlr"
# 7. .exit_octave(FALSE) at gpmlr-package.R#23
# 6. (function (e)
#    .exit_octave(FALSE))(<environment>)
# 5. gc()
# 4. unload_dll(package)
# 3. pkgload::unload(pkg$package)
# 2. reload(pkg, quiet = quiet)
# 1. devtools::install()
# I don't expect this to become a real issue, but it's something to
# keep an eye on moving forward, test, and find a best solution
# Then we call reg.finalizer() in .onLoad()
.onLoad <- function(libname, pkgname) {
    reg.finalizer(.gpmlr_package_loaded, .exit_octave_on_quit, onexit = TRUE)
}
.onUnload <- function(libpath) {
    library.dynam.unload('gpmlr', libpath)
}

