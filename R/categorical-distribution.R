

#' Categorical distribution
#'
#' Probability mass function, distribution function, quantile function and random generation
#' for the categorical distribution.
#'
#' @param x,q	            vector of quantiles.
#' @param p	              vector of probabilities.
#' @param n	              number of observations. If \code{length(n) > 1},
#'                        the length is taken to be the number required.
#' @param prob            vector of length \eqn{k}, or \eqn{k}-column matrix
#'                        of probabilities. Probabilities need to sum up to 1.
#' @param log,log.p	      logical; if TRUE, probabilities p are given as log(p).
#' @param lower.tail	    logical; if TRUE (default), probabilities are \eqn{P[X \le x]}
#'                        otherwise, \eqn{P[X > x]}.
#' @param labels          if provided, labeled \code{factor} vector is returned.
#'                        Number of labels needs to be the same as
#'                        number of categories (number of columns in prob).
#'
#' @examples 
#' 
#' # Generating 10 random draws from categorical distribution
#' # with k=3 categories occuring with equal probabilities
#' # parametrized using a vector
#' 
#' rcat(10, c(1/3, 1/3, 1/3))
#' 
#' # or with k=5 categories parametrized using a matrix of probabilities
#' # (generated from Dirichlet distribution)
#' 
#' p <- rdirichlet(10, c(1, 1, 1, 1, 1))
#' rcat(10, p)
#' 
#' x <- rcat(1e5, c(0.2, 0.4, 0.3, 0.1))
#' plot(prop.table(table(x)), type = "h")
#' lines(0:5, dcat(0:5, c(0.2, 0.4, 0.3, 0.1)), col = "red")
#' 
#' p <- rdirichlet(1, rep(1, 20))
#' x <- rcat(1e5, matrix(rep(p, 2), nrow = 2, byrow = TRUE))
#' xx <- 0:21
#' plot(prop.table(table(x)))
#' lines(xx, dcat(xx, p), col = "red")
#' 
#' xx <- seq(0, 21, by = 0.01)
#' plot(ecdf(x))
#' lines(xx, pcat(xx, p), col = "red", lwd = 2)
#' 
#' pp <- seq(0, 1, by = 0.001)
#' plot(ecdf(x))
#' lines(qcat(pp, p), pp, col = "red", lwd = 2)
#'
#' @name Categorical
#' @aliases Categorical
#' @aliases dcat
#' @keywords distribution
#'
#' @export

dcat <- function(x, prob, log = FALSE) {
  if (is.vector(prob))
    prob <- matrix(prob, nrow = 1)
  else if (!is.matrix(prob))
    prob <- as.matrix(prob)
  cpp_dcat(as.numeric(x), prob, log)
}


#' @rdname Categorical
#' @export

pcat <- function(q, prob, lower.tail = TRUE, log.p = FALSE) {
  if (is.vector(prob))
    prob <- matrix(prob, nrow = 1)
  else if (!is.matrix(prob))
    prob <- as.matrix(prob)
  cpp_pcat(as.numeric(q), prob, lower.tail, log.p)
}


#' @rdname Categorical
#' @export

qcat <- function(p, prob, lower.tail = TRUE, log.p = FALSE, labels) {
  if (is.vector(prob))
    prob <- matrix(prob, nrow = 1)
  else if (!is.matrix(prob))
    prob <- as.matrix(prob)
  
  x <- cpp_qcat(p, prob, lower.tail, log.p)
  
  if (!missing(labels)) {
    if (length(labels) != ncol(prob))
      warning("Wrong number of labels.")
    else
      return(factor(x, levels = 1:ncol(prob), labels = labels))
  }
  
  return(x)
}


#' @rdname Categorical
#' @export

rcat <- function(n, prob, labels) {
  if (length(n) > 1) n <- length(n)
  
  if (is.vector(prob)) {
    k <- length(prob)
    if (anyNA(prob) || any(prob < 0)) {
      warning("NAs produced")
      x <- rep(NA, n)
    } else {
      x <- sample.int(k, size = n, replace = TRUE, prob = prob)
    }
  } else {
    k <- ncol(prob)
    x <- cpp_rcat(n, prob)
  }
  
  if (!missing(labels)) {
    if (length(labels) != k)
      warning("Wrong number of labels.")
    else
      return(factor(x, levels = 1:k, labels = labels))
  }
  
  return(x)
}

