% Generated by roxygen2: do not edit by hand
% Please edit documentation in R/triangular-distribution.R
\name{Triangular}
\alias{Triangular}
\alias{dtriang}
\alias{ptriang}
\alias{qtriang}
\alias{rtriang}
\title{Triangular distribution}
\usage{
dtriang(x, a = -1, b = 1, c = (a + b)/2, log = FALSE)

ptriang(q, a = -1, b = 1, c = (a + b)/2, lower.tail = TRUE,
  log.p = FALSE)

qtriang(p, a = -1, b = 1, c = (a + b)/2, lower.tail = TRUE,
  log.p = FALSE)

rtriang(n, a = -1, b = 1, c = (a + b)/2)
}
\arguments{
\item{x, q}{vector of quantiles.}

\item{a, b, c}{minimum, maximum and mode of the distribution.}

\item{log, log.p}{logical; if TRUE, probabilities p are given as log(p).}

\item{lower.tail}{logical; if TRUE (default), probabilities are \eqn{P[X \le x]}
otherwise, \eqn{P[X > x]}.}

\item{p}{vector of probabilities.}

\item{n}{number of observations. If \code{length(n) > 1},
the length is taken to be the number required.}
}
\description{
Density, distribution function, quantile function and random generation
for the triangular distribution.
}
\details{
Probability density function
\deqn{
f(x) = \left\{\begin{array}{ll}
\frac{2(x-a)}{(b-a)(c-a)} & x < c \\
\frac{2}{b-a}             & x = c \\
\frac{2(b-x)}{(b-a)(b-c)} & x > c
\end{array}\right.
}{
f(x) = [if x < c:] (2*(x-a)) / ((b-a)*(c-a))
       [if x = c:] 2/(b-a)
       [if x >= c:] (2*(b-x)) / ((b-a)*(b-c))
}

Cumulative distribution function
\deqn{
F(x) = \left\{\begin{array}{ll}
\frac{(x-a)^2}{(b-a)(c-a)}     & x \leq c \\
1 - \frac{(b-x)^2}{(b-a)(b-c)} & x > c
\end{array}\right.
}{
F(x) = [if x <= c:] (x-a)^2 / ((b-a)*(c-a))
       [if x > c:] 1 - ((b-x)^2 / ((b-a)*(b-c)))
}

Quantile function
\deqn{
F^{-1}(p) = \left\{\begin{array}{ll}
a + \sqrt{p \times (b-a)(c-a)} & p \leq \frac{c-a}{b-a} \\
b - \sqrt{(1-p)(b-a)(b-c)}     & p > \frac{c-a}{b-a}
\end{array}\right.
}{
F^-1(p) = [if p < (c-a)/(b-a):] a + sqrt(p*(b-a)*(c-a))
          [else:] b - sqrt((1-p)*(b-a)*(b-c))
}

For random generation MINMAX method described by
Stein and Keblis (2009) is used.
}
\examples{

x <- rtriang(1e5, 5, 7, 6)
xx <- seq(-10, 10, by = 0.001)
hist(x, 100, freq = FALSE)
lines(xx, dtriang(xx, 5, 7, 6), col = "red")
hist(ptriang(x, 5, 7, 6))
plot(ecdf(x))
lines(xx, ptriang(xx, 5, 7, 6), col = "red", lwd = 2)

}
\references{
Forbes, C., Evans, M. Hastings, N., & Peacock, B. (2011).
Statistical Distributions. John Wiley & Sons.

Stein, W. E., & Keblis, M. F. (2009).
A new method to simulate the triangular distribution.
Mathematical and computer modelling, 49(5), 1143-1147.
}
\keyword{distribution}

