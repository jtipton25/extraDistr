#include <Rcpp.h>
#include "shared.h"
// [[Rcpp::plugins(cpp11)]]

using std::pow;
using std::sqrt;
using std::abs;
using std::exp;
using std::log;
using std::floor;
using std::ceil;
using Rcpp::NumericVector;


/*
*  Beta-binomial distribution
*
*  Values:
*  x
*
*  Parameters:
*  k > 0
*  alpha > 0
*  beta > 0
*
*  f(k) = choose(n, k) * (beta(k+alpha, n-k+beta)) / (beta(alpha, beta))
*
*/

inline double pmf_bbinom(double k, double n, double alpha,
                         double beta, bool& throw_warning) {
  if (ISNAN(k) || ISNAN(n) || ISNAN(alpha) || ISNAN(beta))
    return k+n+alpha+beta;
  if (alpha < 0.0 || beta < 0.0 || n < 0.0 || !isInteger(n, false)) {
    throw_warning = true;
    return NAN;
  }
  if (!isInteger(k) || k < 0.0 || k > n)
    return 0.0;
  return R::choose(n, k) * R::beta(k+alpha, n-k+beta) / R::beta(alpha, beta);
}

inline double logpmf_bbinom(double k, double n, double alpha,
                            double beta, bool& throw_warning) {
  if (ISNAN(k) || ISNAN(n) || ISNAN(alpha) || ISNAN(beta))
    return k+n+alpha+beta;
  if (alpha < 0.0 || beta < 0.0 || n < 0.0 || !isInteger(n, false)) {
    throw_warning = true;
    return NAN;
  }
  if (!isInteger(k) || k < 0.0 || k > n)
    return R_NegInf;
  return R::lchoose(n, k) + R::lbeta(k+alpha, n-k+beta) - R::lbeta(alpha, beta);
}

inline std::vector<double> cdf_bbinom_table(double k, double n,
                                            double alpha, double beta) {
  
  if (k < 0.0 || k > n || alpha < 0.0 || beta < 0.0)
    Rcpp::stop("inadmissible values");

  int ik = to_pos_int(k);
  std::vector<double> p_tab(ik+1);
  double nck, bab, gx, gy, gxy;
  
  bab = R::lbeta(alpha, beta);
  gxy = R::lgammafn(alpha + beta + n);
  
  // k = 0
  
  nck = 0.0;
  gx = R::lgammafn(alpha);
  gy = R::lgammafn(beta + n);
  p_tab[0] = exp(nck + gx + gy - gxy - bab);
  
  if (ik < 1)
    return p_tab;
  
  // k < 2
  
  nck += log(n);
  gx += log(alpha);
  gy -= log(n + beta - 1.0);
  p_tab[1] = p_tab[0] + exp(nck + gx + gy - gxy - bab);
  
  if (ik < 2)
    return p_tab;
  
  // k >= 1
  
  double dj;
  
  for (int j = 2; j <= ik; j++) {
    dj = to_dbl(j);
    nck += log((n + 1.0 - dj)/dj);
    gx += log(dj + alpha - 1.0);
    gy -= log(n + beta - dj);
    p_tab[j] = p_tab[j-1] + exp(nck + gx + gy - gxy - bab);
  }
  
  return p_tab;
}

inline double rng_bbinom(double n, double alpha,
                         double beta, bool& throw_warning) {
  if (ISNAN(n) || ISNAN(alpha) || ISNAN(beta) ||
      alpha < 0.0 || beta < 0.0 || n < 0.0 || !isInteger(n, false)) {
    throw_warning = true;
    return NA_REAL;
  }
  double prob = R::rbeta(alpha, beta);
  return R::rbinom(n, prob);
}


// [[Rcpp::export]]
NumericVector cpp_dbbinom(
    const NumericVector& x,
    const NumericVector& size,
    const NumericVector& alpha,
    const NumericVector& beta,
    const bool& log_prob = false
  ) {

  int Nmax = std::max({
    x.length(),
    size.length(),
    alpha.length(),
    beta.length()
  });
  NumericVector p(Nmax);
  
  bool throw_warning = false;

  for (int i = 0; i < Nmax; i++)
    p[i] = logpmf_bbinom(GETV(x, i), GETV(size, i),
                         GETV(alpha, i), GETV(beta, i),
                         throw_warning);

  if (!log_prob)
    p = Rcpp::exp(p);
  
  if (throw_warning)
    Rcpp::warning("NaNs produced");

  return p;
}


// [[Rcpp::export]]
NumericVector cpp_pbbinom(
    const NumericVector& x,
    const NumericVector& size,
    const NumericVector& alpha,
    const NumericVector& beta,
    const bool& lower_tail = true,
    const bool& log_prob = false
  ) {

  int Nmax = std::max({
    x.length(),
    size.length(),
    alpha.length(),
    beta.length()
  });
  NumericVector p(Nmax);
  
  bool throw_warning = false;
  
  std::map<std::tuple<int, int, int>, std::vector<double>> memo;
  double mx = std::min(finite_max_int(x), finite_max_int(size));
  
  for (int i = 0; i < Nmax; i++) {
    
    if (i % 1000 == 0)
      Rcpp::checkUserInterrupt();
    
    if (ISNAN(GETV(x, i)) || ISNAN(GETV(size, i)) ||
        ISNAN(GETV(alpha, i)) || ISNAN(GETV(beta, i))) {
      p[i] = GETV(x, i) + GETV(size, i) + GETV(alpha, i) + GETV(beta, i);
    } else if (GETV(alpha, i) <= 0.0 || GETV(beta, i) <= 0.0 ||
               GETV(size, i) < 0.0 || !isInteger(GETV(size, i), false)) {
      throw_warning = true;
      p[i] = NAN;
    } else if (GETV(x, i) < 0.0) {
      p[i] = 0.0;
    } else if (GETV(x, i) >= GETV(size, i)) {
      p[i] = 1.0;
    } else if (is_large_int(GETV(x, i))) {
      p[i] = NA_REAL;
      Rcpp::warning("NAs introduced by coercion to integer range");
    } else {
      
      std::vector<double>& tmp = memo[std::make_tuple(i % size.length(),
                                                      i % alpha.length(),
                                                      i % beta.length())];
      if (!tmp.size()) {
        tmp = cdf_bbinom_table(mx, GETV(size, i), GETV(alpha, i), GETV(beta, i));
      }
      p[i] = tmp[to_pos_int(GETV(x, i))];
      
    }
  }

  if (!lower_tail)
    p = 1.0 - p;
  
  if (log_prob)
    p = Rcpp::log(p);
  
  if (throw_warning)
    Rcpp::warning("NaNs produced");

  return p;
}


// [[Rcpp::export]]
NumericVector cpp_rbbinom(
    const int& n,
    const NumericVector& size,
    const NumericVector& alpha,
    const NumericVector& beta
  ) {

  NumericVector x(n);
  
  bool throw_warning = false;

  for (int i = 0; i < n; i++)
    x[i] = rng_bbinom(GETV(size, i), GETV(alpha, i), GETV(beta, i),
                      throw_warning);
  
  if (throw_warning)
    Rcpp::warning("NAs produced");

  return x;
}

