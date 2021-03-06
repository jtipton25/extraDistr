

#' Half-normal distribution
#'
#' Density, distribution function, quantile function and random generation
#' for the half-normal distribution.
#'
#' @param x,q	            vector of quantiles.
#' @param p	              vector of probabilities.
#' @param n	              number of observations. If \code{length(n) > 1},
#'                        the length is taken to be the number required.
#' @param sigma	          positive valued scale parameter.
#' @param log,log.p	      logical; if TRUE, probabilities p are given as log(p).
#' @param lower.tail	    logical; if TRUE (default), probabilities are \eqn{P[X \le x]}
#'                        otherwise, \eqn{P[X > x]}.
#' 
#' @details
#' If \eqn{X} follows normal distribution centered at 0 and parametrized
#' by scale \eqn{\sigma}, then \eqn{|X|} follows half-normal distribution
#' parametrized by scale \eqn{\sigma}. Half-t distribution with \eqn{\nu=\infty}
#' degrees of freedom converges to half-normal distribution.
#' 
#' @references
#' Gelman, A. (2006). Prior distributions for variance parameters in hierarchical
#' models (comment on article by Browne and Draper).
#' Bayesian analysis, 1(3), 515-534.
#' 
#' @references 
#' Jacob, E. and Jayakumar, K. (2012).
#' On Half-Cauchy Distribution and Process.
#' International Journal of Statistika and Mathematika, 3(2), 77-81.
#' 
#' @seealso \code{\link{HalfT}}
#' 
#' @examples 
#' 
#' x <- rhnorm(1e5, 2)
#' xx <- seq(-1, 100, by = 0.01)
#' hist(x, 100, freq = FALSE)
#' lines(xx, dhnorm(xx, 2), col = "red")
#' hist(phnorm(x, 2))
#' plot(ecdf(x))
#' lines(xx, phnorm(xx, 2), col = "red", lwd = 2)
#'
#' @name HalfNormal
#' @aliases HalfNormal
#' @aliases dhnorm
#' @keywords distribution
#'
#' @export

dhnorm <- function(x, sigma = 1, log = FALSE) {
  cpp_dhnorm(x, sigma, log)
}


#' @rdname HalfNormal
#' @export

phnorm <- function(q, sigma = 1, lower.tail = TRUE, log.p = FALSE) {
  cpp_phnorm(q, sigma, lower.tail, log.p)
}


#' @rdname HalfNormal
#' @export

qhnorm <- function(p, sigma = 1, lower.tail = TRUE, log.p = FALSE) {
  cpp_qhnorm(p, sigma, lower.tail, log.p)
}


#' @rdname HalfNormal
#' @export

rhnorm <- function(n, sigma = 1) {
  if (length(n) > 1) n <- length(n)
  cpp_rhnorm(n, sigma)
}

