context("Octave embedding and gp()")

## We need to make sure that Octave embeds OK, that we exit OK, that we don't
## attempt to reembed, that all the messages we expect are printed, and that
## we get all the right output from the gp() function.
## First I'll set up an example with simple functions:
set.seed(123)
x <- rnorm(20, 0.8, 1)
y <- sin(3 * x) + 0.1 * rnorm(20, 0.9, 1)
xs <- seq(-3, 3, length.out = 61)
ys <- sin(3 * xs) + 0.1 * rnorm(length(xs), 0.9, 1)
hyp <- list(mean = numeric(), cov = c(0, 0), lik = -1)
gp_train_ev <- readRDS("gp_res_training_ev.rds")
gp_pred1_ev <- readRDS("gp_res_pred1_ev.rds")
gp_pred2_ev <- readRDS("gp_res_pred2_ev.rds")
x2 <- matrix(rnorm(40, 0.8, 1), ncol = 2)
## Now a more complex example:
## (both this and the above example are from
## http://www.gaussianprocess.org/gpml/code/matlab/doc/)
n1 <- 80
n2 <- 40
S1 <- diag(2)
S2 <- matrix(c(1, 0.95, 0.95, 1), nrow = 2, byrow = TRUE)
m1 <- c( 0.75, 0)
m2 <- c(-0.75, 0)
set.seed(42)
tmp <- t(chol(S1)) %*% matrix(rnorm(160, sd = 0.2), nrow = 2, ncol = n1)
X1 <- apply(tmp, 2, '+', m1)
tmp <- t(chol(S2)) %*% matrix(rnorm(160, sd = 0.3), nrow = 2, ncol = n2)
X2 <- apply(tmp, 2, '+', m1)
X <- t(cbind(X1, X2))
Y <- matrix(c(rep(-1, n1), rep(1, n2)), ncol = 1)
tt <- as.matrix(expand.grid(seq(-4, 4, 0.1), seq(-4, 4, 0.1)))
u <- as.matrix(expand.grid(-2:2, -2:2))
n <- prod(dim(tt))
hyp2 <- list(mean = -3, cov = c(-0.2, -0.05, 1.25))
inffunc <- "infLaplace"
meanfunc <- "meanConst"
covfunc <- list("apxSparse", list("covSEard"), u)
likfunc <- "likErf"
gp_complex_ev <- readRDS("gp_res_complex_ev.rds")


## Then we run the tests:

test_that("Octave sets up properly on attach", {
    # attach_message is created in testthat.R
    expect_equal(attach_message,
                 c("Embedding Octave...", "Octave successfully embedded.",
                   "Octave load path correctly set.", "gp() is safe to call."))
    Octave_load_path <- capture.output(gpmlr:::.print_path())
    expect_match(Octave_load_path, "gpmlr", all = FALSE)
})

test_that("We do not attempt re-embedding while embedded", {
    expect_equal(capture.output(gpmlr:::.embed_octave(TRUE, FALSE)),
                 c("Octave is already embedded, skipping embedding.",
                   "[1] TRUE"))
})

gp_train <- gp(hyp, "infGaussLik", "", "covSEiso", "likGauss", x, y)
gp_pred1 <- gp(hyp, "infGaussLik", "", "covSEiso", "likGauss", x, y, xs)
gp_pred2 <- gp(hyp, "infGaussLik", "", "covSEiso", "likGauss", x, y, xs, ys)
gp_complex <- gp(hyp2, inffunc, meanfunc, covfunc, likfunc, X, Y, tt, rep(1, n))

test_that("The gp function works properly while Octave is embedded", {
    expect_equal(gp_train, gp_train_ev)
    expect_equal(gp_pred1, gp_pred1_ev)
    expect_equal(gp_pred2, gp_pred2_ev)
    expect_error(gp(hyp, "infGaussLik", "", "covSEiso", "likGauss", x2, y), NA)
    expect_equal(gp_complex, gp_complex_ev)
    expect_error(gp(hyp, "infGaussLik", "", NA, "likGauss", x, y), "Failed")
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

