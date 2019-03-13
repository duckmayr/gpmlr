#' Bald Score Calculation for Determining Potential Information Gain
#'
#' \code{bald_score} uses results from \code{gp} to calculate the bald scores
#' capturing potential information gain from a binary comparison.
#' 
#' This function takes as input results from a preference learning GP.
#' It then calculates the potential information gain of comparing two
#' inputs. For active learning, the maximum bald score indicates which
#' comparison to use next to improve the information.
#'  
#' @param mu A vector of latent means for the possible comparisons, as returned
#'   by the \code{FMU} slot of a \code{gp} call.
#' @param sigma2 A vector of latent standard deviations for the possible
#'   comparison, as returned by the \code{FS2} slot of a \code{gp} call.
#'
#' @return A vector of calculated bald scores for each possible comparison
#' @examples
#' set.seed(12321)
#' num_points <- 200
#' pairs <- t(combn(1:num_points, 2))
#' num_pairs <- nrow(pairs)
#' ind <- sample(num_pairs)
#' pairs <- pairs[ind, ]
#' # ground truth: sample in model
#' f <- matrix(rnorm(num_points), ncol = 1)
#' # sample a label for a given pair
#' oracle <- function(pair) 2 * (runif(1) < pnorm(f[pair[1]] - f[pair[2]])) - 1
#' # create test set containing one observation of each pair
#' y_test <- matrix(0, nrow = num_pairs, ncol = 1)
#' for ( i in 1:num_pairs ) {
#'   y_test[i] <- oracle(pairs[i, ]);
#' }
#' mean_function       <- list("meanPref", list("meanZero"))
#' covariance_function <- list("covPref", list("covNoise"))
#' likelihood          <- "likErf"
#' inference_method    <- "infLaplace"
#' theta <- list(mean = numeric(), cov = log(1))
#' # sample first observation randomly
#' train_ind <- sample(x = 1:num_pairs, size = 1);
#' x <- pairs[train_ind, , drop = FALSE]
#' y <- apply(x, 1, oracle)
#' # run gp()
#' res <- gp(theta, inference_method, mean_function, covariance_function,
#'           likelihood, x, y, pairs)
#' mu <- res$FMU
#' sigma2 <- res$FS2
#' bald_scores <- bald_score(mu, sigma2)
#' # which pair to compare next?
#' pairs[which.max(bald_scores), ]
#' 
#' @seealso \code{\link{gp}}
#' 
#' @export
bald_score <- function(mu, sigma2){
    C <- sqrt(pi * log(2) / 2)
    h <- function(p) { -p * log2(p) - (1 - p) * log2(1 - p) }
    bald <- h(pnorm(mu / sqrt(1 + sigma2)))
    bald <- bald - C / sqrt(sigma2 + C^2) * exp(-mu^2 / (2 * (sigma2 + C^2)))
    return(bald)
}

