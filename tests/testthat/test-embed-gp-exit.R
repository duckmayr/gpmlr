context("Octave embedding and gp()")

## We need to make sure that Octave embeds OK, that we exit OK, that we don't
## attempt to reembed, that all the messages we expect are printed, and that
## we get all the right output from the gp() function.
## First I collect all the output:
embed_output <- capture.output(.embed_octave(TRUE, FALSE))
sub_dirs <- c("cov", "doc", "inf", "lik", "mean", "prior", "util")
dirs_to_add <- system.file(paste0("gpml", c("", paste0("/", sub_dirs))),
                           package = "gpmlr")
.add_to_path(dirs_to_add)
reembed_while_embedded_output <- capture.output(.embed_octave(TRUE, FALSE))
set.seed(123)
x <- rnorm(20, 0.8, 1)
y <- sin(3 * x) + 0.1 * rnorm(20, 0.9, 1)
xs <- seq(-3, 3, length.out = 61)
ys <- sin(3 * xs) + 0.1 * rnorm(length(xs), 0.9, 1)
hyp <- list(mean = numeric(), cov = c(0, 0), lik = -1)
gp_res_training <- gp(hyp, "infGaussLik", "", "likGauss", "covSEiso", x, y)
gp_res_pred1 <- gp(hyp, "infGaussLik", "", "likGauss", "covSEiso", x, y, xs)
gp_res_pred2 <- gp(hyp, "infGaussLik", "", "likGauss", "covSEiso", x, y, xs, ys)
exit_output <- capture.output(.exit_octave(TRUE))
reembed_after_exit_output <- capture.output(.embed_octave(TRUE, FALSE))

## These are the GP results we expect to see:
gp_res_training_ev <- readRDS("gp_res_training_ev.rds")
gp_res_pred1_ev <- readRDS("gp_res_pred1_ev.rds")
gp_res_pred2_ev <- readRDS("gp_res_pred2_ev.rds")

## Then we run the tests:
test_that("The Octave embedded function works properly", {
    expect_equal(embed_output,
                 c("Embedding Octave...", "Octave embedded.", "[1] TRUE"))
    expect_equal(reembed_while_embedded_output,
                 c("Octave is already embedded, skipping embedding.",
                   "[1] TRUE"))
    expect_equal(reembed_after_exit_output,
                 c("Octave was previously embedded.",
                   paste("Embedding again will cause a stream of warnings,",
                         "and is not generally supported."),
                   "To do it anyway, try again with force = TRUE.",
                   "[1] FALSE"))
})
test_that("The Octave exit function works properly", {
    expect_equal(exit_output,
                 c("Exited Octave.", "[1] FALSE"))
})
test_that("The gp function works properly", {
    expect_equal(gp_res_training, gp_res_training_ev)
    expect_equal(gp_res_pred1, gp_res_pred1_ev)
    expect_equal(gp_res_pred2, gp_res_pred2_ev)
})
