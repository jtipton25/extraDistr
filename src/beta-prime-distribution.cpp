#include <Rcpp.h>
#include "shared.h"

using std::pow;
using std::sqrt;
using std::abs;
using std::exp;
using std::log;
using std::floor;
using std::ceil;
using Rcpp::NumericVector;


/*
*  Beta prime distribution
*
*  Values:
*  x > 0
*
*  Parameters:
*  alpha > 0
*  beta > 0
*  sigma > 0
*
*/

double pdf_betapr(double x, double alpha, double beta,
                  double sigma, bool& throw_warning) {
  if (ISNAN(x) || ISNAN(alpha) || ISNAN(beta) || ISNAN(sigma))
    return x+alpha+beta+sigma;
  if (alpha <= 0.0 || beta <= 0.0 || sigma <= 0.0) {
    throw_warning = true;
    return NAN;
  }
  if (x <= 0.0 || !R_FINITE(x))
    return 0.0;
  double z = x / sigma;
  return pow(z, alpha-1.0) * pow(z+1.0, -alpha-beta) / R::beta(alpha, beta) / sigma;
}

double logpdf_betapr(double x, double alpha, double beta,
                     double sigma, bool& throw_warning) {
  if (ISNAN(x) || ISNAN(alpha) || ISNAN(beta) || ISNAN(sigma))
    return x+alpha+beta+sigma;
  if (alpha <= 0.0 || beta <= 0.0 || sigma <= 0.0) {
    throw_warning = true;
    return NAN;
  }
  if (x <= 0.0 || !R_FINITE(x))
    return R_NegInf;
  double z = x / sigma;
  return log(pow(z, alpha-1.0)) + log(pow(z+1.0, -alpha-beta)) - R::lbeta(alpha, beta) - log(sigma);
}

double cdf_betapr(double x, double alpha, double beta,
                  double sigma, bool& throw_warning) {
  if (ISNAN(x) || ISNAN(alpha) || ISNAN(beta) || ISNAN(sigma))
    return x+alpha+beta+sigma;
  if (alpha <= 0.0 || beta <= 0.0 || sigma <= 0.0) {
    throw_warning = true;
    return NAN;
  }
  if (x <= 0.0)
    return 0.0;
  if (!R_FINITE(x))
    return 1.0;
  double z = x / sigma;
  return R::pbeta(z/(1.0+z), alpha, beta, true, false);
}

double invcdf_betapr(double p, double alpha, double beta,
                     double sigma, bool& throw_warning) {
  if (ISNAN(p) || ISNAN(alpha) || ISNAN(beta) || ISNAN(sigma))
    return p+alpha+beta+sigma;
  if (alpha <= 0.0 || beta <= 0.0 || sigma <= 0.0 || p < 0.0 || p > 1.0) {
    throw_warning = true;
    return NAN;
  }
  if (p == 0.0)
    return 0.0;
  if (p == 1.0)
    return R_PosInf;
  double x = R::qbeta(p, alpha, beta, true, false);
  return x/(1.0-x) * sigma;
}

double rng_betapr(double alpha, double beta,
                  double sigma, bool& throw_warning) {
  if (ISNAN(alpha) || ISNAN(beta) || ISNAN(sigma) ||
      alpha <= 0.0 || beta <= 0.0 || sigma <= 0.0) {
    throw_warning = true;
    return NA_REAL;
  }
  double x = R::rbeta(alpha, beta);
  return x/(1.0-x) * sigma;
}


// [[Rcpp::export]]
NumericVector cpp_dbetapr(
    const NumericVector& x,
    const NumericVector& alpha,
    const NumericVector& beta,
    const NumericVector& sigma,
    const bool& log_prob = false
  ) {
  
  std::vector<int> dims;
  dims.push_back(x.length());
  dims.push_back(alpha.length());
  dims.push_back(beta.length());
  dims.push_back(sigma.length());
  int Nmax = *std::max_element(dims.begin(), dims.end());
  NumericVector p(Nmax);
  
  bool throw_warning = false;
  
  for (int i = 0; i < Nmax; i++)
    p[i] = pdf_betapr(x[i % dims[0]], alpha[i % dims[1]],
                      beta[i % dims[2]], sigma[i % dims[3]],
                      throw_warning);
  
  if (log_prob)
    p = Rcpp::log(p);
  
  if (throw_warning)
    Rcpp::warning("NaNs produced");
  
  return p;
}


// [[Rcpp::export]]
NumericVector cpp_pbetapr(
    const NumericVector& x,
    const NumericVector& alpha,
    const NumericVector& beta,
    const NumericVector& sigma,
    const bool& lower_tail = true,
    const bool& log_prob = false
  ) {
  
  std::vector<int> dims;
  dims.push_back(x.length());
  dims.push_back(alpha.length());
  dims.push_back(beta.length());
  dims.push_back(sigma.length());
  int Nmax = *std::max_element(dims.begin(), dims.end());
  NumericVector p(Nmax);
  
  bool throw_warning = false;
  
  for (int i = 0; i < Nmax; i++)
    p[i] = cdf_betapr(x[i % dims[0]], alpha[i % dims[1]],
                      beta[i % dims[2]], sigma[i % dims[3]],
                      throw_warning);
  
  if (!lower_tail)
    p = 1.0 - p;
  
  if (log_prob)
    p = Rcpp::log(p);
  
  if (throw_warning)
    Rcpp::warning("NaNs produced");

  return p;
}


// [[Rcpp::export]]
NumericVector cpp_qbetapr(
    const NumericVector& p,
    const NumericVector& alpha,
    const NumericVector& beta,
    const NumericVector& sigma,
    const bool& lower_tail = true,
    const bool& log_prob = false
  ) {
  
  std::vector<int> dims;
  dims.push_back(p.length());
  dims.push_back(alpha.length());
  dims.push_back(beta.length());
  dims.push_back(sigma.length());
  int Nmax = *std::max_element(dims.begin(), dims.end());
  NumericVector q(Nmax);
  NumericVector pp = Rcpp::clone(p);
  
  bool throw_warning = false;
  
  if (log_prob)
    pp = Rcpp::exp(pp);
  
  if (!lower_tail)
    pp = 1.0 - pp;
  
  for (int i = 0; i < Nmax; i++)
    q[i] = invcdf_betapr(pp[i % dims[0]], alpha[i % dims[1]],
                         beta[i % dims[2]], sigma[i % dims[3]],
                         throw_warning);
  
  if (throw_warning)
    Rcpp::warning("NaNs produced");
  
  return q;
}


// [[Rcpp::export]]
NumericVector cpp_rbetapr(
    const int& n,
    const NumericVector& alpha,
    const NumericVector& beta,
    const NumericVector& sigma
  ) {
  
  std::vector<int> dims;
  dims.push_back(alpha.length());
  dims.push_back(beta.length());
  dims.push_back(sigma.length());
  NumericVector x(n);
  
  bool throw_warning = false;
  
  for (int i = 0; i < n; i++)
    x[i] = rng_betapr(alpha[i % dims[0]], beta[i % dims[1]],
                      sigma[i % dims[2]], throw_warning);
  
  if (throw_warning)
    Rcpp::warning("NAs produced");
  
  return x;
}

