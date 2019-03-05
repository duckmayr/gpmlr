context("Octave embedding and gp()")

## We need to make sure that Octave embeds OK, that we exit OK, that we don't
## attempt to reembed, that all the messages we expect are printed, and that
## we get all the right output from the gp() function.
## First I'll set up some data we'll need:
set.seed(123)
x <- rnorm(20, 0.8, 1)
y <- sin(3 * x) + 0.1 * rnorm(20, 0.9, 1)
xs <- seq(-3, 3, length.out = 61)
ys <- sin(3 * xs) + 0.1 * rnorm(length(xs), 0.9, 1)
hyp <- list(mean = numeric(), cov = c(0, 0), lik = -1)
gp_res_training_ev <- readRDS("gp_res_training_ev.rds")
gp_res_pred1_ev <- readRDS("gp_res_pred1_ev.rds")
gp_res_pred2_ev <- readRDS("gp_res_pred2_ev.rds")

## Then we run the tests:
test_that("Octave sets up properly on attach", {
    # attach_message is created in testthat.R
    expect_equal(attach_message,
                 c("Embedding Octave...", "Octave successfully embedded.",
                   "Octave load path correctly set.", "gp() is safe to call."))
})
test_that("We do not attempt re-embedding while embedded", {
    expect_equal(capture.output(gpmlr:::.embed_octave(TRUE, FALSE)),
                 c("Octave is already embedded, skipping embedding.",
                   "[1] TRUE"))
})
gp_res_training <- gp(hyp, "infGaussLik", "", "likGauss", "covSEiso", x, y)
gp_res_pred1 <- gp(hyp, "infGaussLik", "", "likGauss", "covSEiso", x, y, xs)
gp_res_pred2 <- gp(hyp, "infGaussLik", "", "likGauss", "covSEiso", x, y, xs, ys)
test_that("The gp function works properly while Octave is embedded", {
    expect_equal(gp_res_training, gp_res_training_ev)
    expect_equal(gp_res_pred1, gp_res_pred1_ev)
    expect_equal(gp_res_pred2, gp_res_pred2_ev)
})
test_that("The Octave exit function works properly", {
    expect_equal(capture.output(gpmlr:::.exit_octave(TRUE)),
                 c("Exited Octave.", "[1] FALSE"))
})
test_that("gp() operates safely when Octave is not embedded", {
    l <- list()
    expect_error(gp(l, "", "", "", "", 1, 1, 1), "Octave failed to embed")
    expect_error(gpmlr:::.gpml1(l, "", "", "", "", 1, 1), "embed_octave")
    expect_error(gpmlr:::.gpml2(l, "", "", "", "", 1, 1, 1), "embed_octave")
    expect_error(gpmlr:::.gpml3(l, "", "", "", "", 1, 1, 1, 1), "embed_octave")
})
test_that("Path functions operate safely when Octave is not embedded", {
    expect_error(gpmlr:::.print_path(), "embed_octave")
    expect_error(gpmlr:::.add_to_path(""), "embed_octave")
})
test_that("We do not attempt re-embedding after exiting Octave", {
    expect_equal(capture.output(gpmlr:::.embed_octave(TRUE, FALSE)),
                 c("Octave was previously embedded.",
                   paste("Embedding again will cause a stream of warnings,",
                         "and is not generally supported."),
                   "To do it anyway, try again with force = TRUE.",
                   "[1] FALSE"))
})

