% Generated by roxygen2: do not edit by hand
% Please edit documentation in R/non-standard-t-distribution.R
\name{NonStandardT}
\alias{NonStandardT}
\alias{dnst}
\alias{pnst}
\alias{qnst}
\alias{rnst}
\title{Non-standard t-distribution}
\usage{
dnst(x, df, mu = 0, sigma = 1, log = FALSE)

pnst(q, df, mu = 0, sigma = 1, lower.tail = TRUE, log.p = FALSE)

qnst(p, df, mu = 0, sigma = 1, lower.tail = TRUE, log.p = FALSE)

rnst(n, df, mu = 0, sigma = 1)
}
\arguments{
\item{x, q}{vector of quantiles.}

\item{df}{degrees of freedom (> 0, maybe non-integer). \code{df = Inf} is allowed.}

\item{mu}{vector of locations}

\item{sigma}{vector of positive valued scale parameters.}

\item{log, log.p}{logical; if TRUE, probabilities p are given as log(p).}

\item{lower.tail}{logical; if TRUE (default), probabilities are \eqn{P[X \le x]}
otherwise, \eqn{P[X > x]}.}

\item{p}{vector of probabilities.}

\item{n}{number of observations. If \code{length(n) > 1},
the length is taken to be the number required.}
}
\description{
Probability mass function, distribution function and random generation
for non-standard t-distribution. Non-standard t-distribution besides
degrees of freedom \eqn{\nu}, is parametrized using additional parameters
\eqn{\mu} for location and \eqn{\sigma} for scale (\eqn{\mu=0} and
\eqn{\sigma = 1} for standard t-distribution).
}
\examples{

x <- rnst(1e5, 1000, 5, 13)
xx <- seq(-60, 60, by = 0.01)
hist(x, 100, freq = FALSE)
lines(xx-0.5, dnst(xx, 1000, 5, 13), col = "red")
hist(pnst(x, 1000, 5, 13))
plot(ecdf(x))
lines(xx, pnst(xx, 1000, 5, 13), col = "red", lwd = 2)

}
\seealso{
\code{\link[stats]{TDist}}
}
\keyword{distribution}

