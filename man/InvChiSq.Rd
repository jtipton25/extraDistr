% Generated by roxygen2: do not edit by hand
% Please edit documentation in R/inverse-chi-squared-distribution.R
\name{InvChiSq}
\alias{InvChiSq}
\alias{dinvchisq}
\alias{pinvchisq}
\alias{qinvchisq}
\alias{rinvchisq}
\title{Inverse chi-squared and scaled chi-squared distributions}
\usage{
dinvchisq(x, nu, tau, log = FALSE)

pinvchisq(q, nu, tau, lower.tail = TRUE, log.p = FALSE)

qinvchisq(p, nu, tau, lower.tail = TRUE, log.p = FALSE)

rinvchisq(n, nu, tau)
}
\arguments{
\item{x, q}{vector of quantiles.}

\item{nu}{positive valued shape parameter.}

\item{tau}{positive valued scaling parameter; if provided it
returns values for scaled chi-squared distributions.}

\item{log, log.p}{logical; if TRUE, probabilities p are given as log(p).}

\item{lower.tail}{logical; if TRUE (default), probabilities are \eqn{P[X \le x]}
otherwise, \eqn{P[X > x]}.}

\item{p}{vector of probabilities.}

\item{n}{number of observations. If \code{length(n) > 1},
the length is taken to be the number required.}
}
\description{
Density, distribution function and random generation
for the inverse chi-squared distribution and scaled chi-squared distribution.
}
\details{
If \eqn{X} follows \eqn{\chi^2 (\nu)} distribution, then \eqn{1/X} follows inverse
chi-squared distribution parametrized by \eqn{\nu}. Inverse chi-squared distribution
is a special case of inverse gamma distribution with parameters
\eqn{\alpha=\frac{\nu}{2}}{\alpha=\nu/2} and \eqn{\beta=\frac{1}{2}}{\beta=1/2};
or \eqn{\alpha=\frac{\nu}{2}}{\alpha=\nu/2} and
\eqn{\beta=\frac{\nu\tau^2}{2}}{\beta=(\nu\tau^2)/2} for scaled inverse
chi-squared distribution.
}
\examples{

x <- rinvchisq(1e5, 20)
xx <- seq(0, 6, by = 0.001)
hist(x, 100, freq = FALSE)
lines(xx, dinvchisq(xx, 20), col = "red")
hist(pinvchisq(x, 20))
plot(ecdf(x))
lines(xx, pinvchisq(xx, 20), col = "red", lwd = 2)

# scaled

x <- rinvchisq(1e5, 10, 5)
xx <- seq(0, 700, by = 0.01)
hist(x, 100, freq = FALSE)
lines(xx, dinvchisq(xx, 10, 5), col = "red")
hist(pinvchisq(x, 10, 5))
plot(ecdf(x))
lines(xx, pinvchisq(xx, 10, 5), col = "red", lwd = 2)

}
\seealso{
\code{\link[stats]{Chisquare}}, \code{\link[stats]{GammaDist}}
}
\keyword{distribution}

