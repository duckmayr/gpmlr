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
x2 <- matrix(rnorm(40, 0.8, 1), ncol = 2)


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

gp_train <- gp(hyp, "infExact", "", "covSEiso", "likGauss", x, y)
gp_pred1 <- gp(hyp, "infExact", "", "covSEiso", "likGauss", x, y, xs)
gp_pred2 <- gp(hyp, "infExact", "", "covSEiso", "likGauss", x, y, xs, ys)

test_that("The gp function works properly while Octave is embedded", {
    expect_setequal(names(gp_train), c("NLZ", "DNLZ", "POST"))
    expect_setequal(names(gp_train$POST), c("L", "alpha", "sW"))
    expect_setequal(sapply(gp_train, class), c("matrix", "list"))
    expect_setequal(sapply(gp_train, length), c(1, 3))
    expect_setequal(sapply(gp_train$DNLZ, class), "matrix")
    expect_setequal(sapply(gp_train$DNLZ, length), 0:2)
    expect_setequal(sapply(gp_train$POST, class), "matrix")
    expect_setequal(sapply(gp_train$POST, length), c(20, 400))
    expect_setequal(names(gp_pred1), c("YMU", "YS2", "FMU", "FS2", "POST"))
    expect_setequal(names(gp_pred1$POST), c("L", "alpha", "sW"))
    expect_setequal(sapply(gp_pred1, class), c("matrix", "list"))
    expect_setequal(sapply(gp_pred1, length), c(61, 3))
    expect_setequal(sapply(gp_pred1$POST, class), "matrix")
    expect_setequal(sapply(gp_pred1$POST, length), c(20, 400))
    expect_setequal(names(gp_pred2), c("YMU", "YS2", "FMU", "FS2", "LP", "POST"))
    expect_setequal(names(gp_pred2$POST), c("L", "alpha", "sW"))
    expect_setequal(sapply(gp_pred2, class), c("matrix", "list"))
    expect_setequal(sapply(gp_pred2, length), c(61, 3))
    expect_setequal(sapply(gp_pred2$POST, class), "matrix")
    expect_setequal(sapply(gp_pred2$POST, length), c(20, 400))
    expect_error(gp(hyp, "infExact", "", "covSEiso", "likGauss", x2, y), NA)
    expect_error(gp(hyp, "infExact", "", NA, "likGauss", x, y), "Failed")
})

set.seed(12321)
num_points <- 200
pairs <- t(combn(1:num_points, 2))
num_pairs <- nrow(pairs)
ind <- sample(num_pairs)
pairs <- pairs[ind, ]
# ground truth: sample in model
f <- matrix(rnorm(num_points), ncol = 1)
# sample a label for a given pair
oracle <- function(pair) 2 * (runif(1) < pnorm(f[pair[1]] - f[pair[2]])) - 1
# create test set containing one observation of each pair
y_test <- matrix(0, nrow = num_pairs, ncol = 1)
for ( i in 1:num_pairs ) {
  y_test[i] <- oracle(pairs[i, ]);
}
mean_function       <- list("meanPref", list("meanZero"))
covariance_function <- list("covPref", list("covNoise"))
likelihood          <- "likErf"
inference_method    <- "infLaplace"
theta <- list(mean = numeric(), cov = log(1))
# sample first observation randomly
train_ind <- sample(x = 1:num_pairs, size = 1);
x <- pairs[train_ind, , drop = FALSE]
y <- apply(x, 1, oracle)
# run gp()
res <- gp(theta, inference_method, mean_function, covariance_function,
          likelihood, x, y, pairs)
mu <- res$FMU
sigma2 <- res$FS2
bald_scores <- bald_score(mu, sigma2)

test_that("bald_score works properly", {
    expect_error(bald_score(mu, sigma2), NA)
    expect_equal(class(bald_score(mu, sigma2)), "matrix")
    expect_equal(dim(bald_score(mu, sigma2)), c(19900, 1))
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

