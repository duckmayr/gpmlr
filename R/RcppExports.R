# Generated by using Rcpp::compileAttributes() -> do not edit by hand
# Generator token: 10BE3573-1514-4C36-9D1C-5A225CD40393

.octave_is_embedded <- function() {
    .Call(`_gpmlr_octave_is_embedded`)
}

.octave_has_ever_been_embedded <- function() {
    .Call(`_gpmlr_octave_has_ever_been_embedded`)
}

.embed_octave <- function(verbose, force) {
    .Call(`_gpmlr_embed_octave`, verbose, force)
}

.exit_octave <- function(verbose) {
    .Call(`_gpmlr_exit_octave`, verbose)
}

.gpml1 <- function(hyperparameters, inffunc, meanfunc, covfunc, likfunc, x, y) {
    .Call(`_gpmlr_gpml1`, hyperparameters, inffunc, meanfunc, covfunc, likfunc, x, y)
}

.gpml2 <- function(hyperparameters, inffunc, meanfunc, covfunc, likfunc, training_x, training_y, testing_x) {
    .Call(`_gpmlr_gpml2`, hyperparameters, inffunc, meanfunc, covfunc, likfunc, training_x, training_y, testing_x)
}

.gpml3 <- function(hyperparameters, inffunc, meanfunc, covfunc, likfunc, training_x, training_y, testing_x, testing_y) {
    .Call(`_gpmlr_gpml3`, hyperparameters, inffunc, meanfunc, covfunc, likfunc, training_x, training_y, testing_x, testing_y)
}

.print_path <- function() {
    invisible(.Call(`_gpmlr_print_path`))
}

.add_to_path <- function(x) {
    invisible(.Call(`_gpmlr_add_to_path`, x))
}

.set_wd <- function(x) {
    invisible(.Call(`_gpmlr_set_wd`, x))
}

.set_hyperparameters <- function(hyp, inf, mean, cov, lik, x, y, n_evals) {
    .Call(`_gpmlr_set_hyperparameters`, hyp, inf, mean, cov, lik, x, y, n_evals)
}

