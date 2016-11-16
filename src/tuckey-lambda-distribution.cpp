#include <Rcpp.h>
#include "shared.h"

using std::pow;
using std::sqrt;
using std::abs;
using std::exp;
using std::log;
using std::floor;
using std::ceil;
using std::sin;
using std::cos;
using std::tan;
using std::atan;
using Rcpp::IntegerVector;
using Rcpp::NumericVector;
using Rcpp::NumericMatrix;

/*
Joiner, B.L., & Rosenblatt, J.R. (1971).
Some properties of the range in samples from Tukey's symmetric lambda distributions.
Journal of the American Statistical Association, 66(334), 394-399.

Hastings Jr, C., Mosteller, F., Tukey, J.W., & Winsor, C.P. (1947).
Low moments for small samples: a comparative study of order statistics.
The Annals of Mathematical Statistics, 413-426.
*/


double invcdf_tlambda(double p, double lambda) {
  if (ISNAN(p) || ISNAN(lambda))
    return NA_REAL;
  if (p < 0.0 || p > 1.0) {
    Rcpp::warning("NaNs produced");
    return NAN;
  }
  if (lambda == 0.0)
    return log(p) - log(1.0 - p);
  return (pow(p, lambda) - pow(1.0 - p, lambda))/lambda;
}


// [[Rcpp::export]]
NumericVector cpp_qtlambda(
    const NumericVector& p,
    const NumericVector& lambda,
    const bool& lower_tail = true,
    const bool& log_prob = false
  ) {
  
  std::vector<int> dims;
  dims.push_back(p.length());
  dims.push_back(lambda.length());
  int Nmax = *std::max_element(dims.begin(), dims.end());
  NumericVector q(Nmax);
  NumericVector pp = Rcpp::clone(p);
  
  if (log_prob)
    pp = exp(pp);
  
  if (!lower_tail)
    pp = 1.0 - pp;
  
  for (int i = 0; i < Nmax; i++)
    q[i] = invcdf_tlambda(pp[i % dims[0]], lambda[i % dims[1]]);
  
  return q;
}


// [[Rcpp::export]]
NumericVector cpp_rtlambda(
    const int& n,
    const NumericVector& lambda
  ) {
  
  int dims = lambda.length();
  NumericVector x(n);
  double u;
    
  for (int i = 0; i < n; i++) {
    u = rng_unif();
    x[i] = invcdf_tlambda(u, lambda[i % dims]);
  }
  
  return x;
}
