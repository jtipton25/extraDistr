

#' Gumbel distribution
#'
#' Density, distribution function, quantile function and random generation
#' for the Gumbel distribution.
#'
#' @param x,q	            vector of quantiles.
#' @param p	              vector of probabilities.
#' @param n	              number of observations. If \code{length(n) > 1},
#'                        the length is taken to be the number required.
#' @param mu,sigma        location and scale parameters. Scale must be positive.
#' @param log,log.p	      logical; if TRUE, probabilities p are given as log(p).
#' @param lower.tail	    logical; if TRUE (default), probabilities are \eqn{P[X \le x]}
#'                        otherwise, \eqn{P[X > x]}.
#'
#' @details
#'
#' Probability density function
#' \deqn{
#' f(x) = \frac{1}{\sigma} \exp\left(-\left(\frac{x-\mu}{\sigma} + \exp\left(-\frac{x-\mu}{\sigma}\right)\right)\right)
#' }{
#' f(x) = 1/\sigma * exp(-((x-\mu)/\sigma + exp(-(x-\mu)/\sigma)))
#' }
#'
#' Cumulative distribution function
#' \deqn{
#' F(x) = \exp\left(-\exp\left(-\frac{x-\mu}{\sigma}\right)\right)
#' }{
#' F(x) = exp(-exp(-(x-\mu)/\sigma))
#' }
#'
#' Quantile function
#' \deqn{
#' F^{-1}(p) = \mu - \sigma \log(-\log(p))
#' }{
#' F^-1(p) = \mu - \sigma * log(-log(p))
#' }
#'
#' @references
#' Bury, K. (1999). Statistical Distributions in Engineering.
#' Cambridge University Press.
#' 
#' @examples 
#' 
#' x <- rgumbel(1e5, 5, 2)
#' xx <- seq(0, 1000, by = 0.1)
#' hist(x, 100, freq = FALSE)
#' lines(xx, dgumbel(xx, 5, 2), col = "red")
#' hist(pgumbel(x, 5, 2))
#' plot(ecdf(x))
#' lines(xx, pgumbel(xx, 5, 2), col = "red", lwd = 2)
#'
#' @name Gumbel
#' @aliases Gumbel
#' @aliases dgumbel
#' @keywords distribution
#'
#' @export

dgumbel <- function(x, mu = 0, sigma = 1, log = FALSE) {
  cpp_dgumbel(x, mu, sigma, log)
}


#' @rdname Gumbel
#' @export

pgumbel <- function(q, mu = 0, sigma = 1, lower.tail = TRUE, log.p = FALSE) {
  cpp_pgumbel(q, mu, sigma, lower.tail, log.p)
}


#' @rdname Gumbel
#' @export

qgumbel <- function(p, mu = 0, sigma = 1, lower.tail = TRUE, log.p = FALSE) {
  cpp_qgumbel(p, mu, sigma, lower.tail, log.p)
}


#' @rdname Gumbel
#' @export

rgumbel <- function(n, mu = 0, sigma = 1) {
  if (length(n) > 1) n <- length(n)
  cpp_rgumbel(n, mu, sigma)
}

