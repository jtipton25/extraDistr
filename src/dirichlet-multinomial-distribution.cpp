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
using Rcpp::NumericMatrix;


/*
*  Dirichlet-multinomial (multivariate Polya) distribution
*
*  Values:
*  x > 0
*
*  Parameters:
*  n > 0
*  alpha > 0    (R^k where k >= 2)
*  
*  where:
*  sum(x) == n
*
*/


// [[Rcpp::export]]
NumericVector cpp_ddirmnom(
    const NumericMatrix& x,
    const NumericVector& size,
    const NumericMatrix& alpha,
    const bool& log_prob = false
  ) {
  
  std::vector<int> dims;
  dims.push_back(x.nrow());
  dims.push_back(alpha.nrow());
  dims.push_back(size.length());
  int Nmax = *std::max_element(dims.begin(), dims.end());
  
  int m = x.ncol();
  int k = alpha.ncol();
  k = std::min(m, k);
  NumericVector p(Nmax);
  
  bool throw_warning = false;
  
  if (k < 2)
    Rcpp::stop("number of columns in alpha should be >= 2");
  if (m != k)
    Rcpp::stop("number of columns in x does not equal number of columns in alpha");
  
  double prod_tmp, sum_alpha, sum_x;
  bool wrong_x, wrong_param;
  
  for (int i = 0; i < Nmax; i++) {
    
    prod_tmp = 0.0;
    sum_alpha = 0.0;
    sum_x = 0.0;
    wrong_x = false;
    wrong_param = false;
    
    for (int j = 0; j < k; j++) {
      if (alpha(i % dims[1], j) <= 0.0)
        wrong_param = true;
      if (x(i % dims[0], j) < 0.0 || !isInteger(x(i % dims[0], j)))
        wrong_x = true;
      
      sum_x += x(i % dims[0], j);
      sum_alpha += alpha(i % dims[1], j);
    }
    
    if (ISNAN(sum_x + sum_alpha + size[i % dims[2]])) {
      p[i] = sum_x + sum_alpha + size[i % dims[2]];
      continue;
    } 
    
    if (wrong_param || size[i % dims[2]] < 0.0 || !isInteger(size[i % dims[2]], false)) {
      throw_warning = true;
      p[i] = NAN;
      continue;
    }
    
    if (sum_x < 0.0 || sum_x != size[i % dims[2]] || wrong_x) {
      p[i] = R_NegInf;
    } else {
      
      for (int j = 0; j < k; j++) {
        prod_tmp += R::lgammafn(x(i % dims[0], j) + alpha(i % dims[1], j)) -
          (lfactorial(x(i % dims[0], j)) + R::lgammafn(alpha(i % dims[1], j)));
      }
      
      p[i] = (lfactorial(size[i % dims[2]]) + R::lgammafn(sum_alpha)) -
        R::lgammafn(size[i % dims[2]] + sum_alpha) + prod_tmp;
    }
  }
  
  if (!log_prob)
    p = Rcpp::exp(p);
  
  if (throw_warning)
    Rcpp::warning("NaNs produced");
  
  return p;
}


// [[Rcpp::export]]
NumericMatrix cpp_rdirmnom(
    const int& n,
    const NumericVector& size,
    const NumericMatrix& alpha
  ) {
  
  int k = alpha.ncol();
  std::vector<int> dims;
  dims.push_back(alpha.nrow());
  dims.push_back(size.length());
  NumericMatrix x(n, k);
  
  bool throw_warning = false;
  
  if (k < 2)
    Rcpp::stop("Number of columns in alpha should be >= 2");
  
  double size_left, row_sum, sum_p, p_tmp, sum_alpha;
  bool wrong_values;
  
  for (int i = 0; i < n; i++) {
    size_left = size[i % dims[1]];
    row_sum = 0.0;
    wrong_values = false;
    NumericVector pi(k);
    sum_alpha = 0.0;
    
    for (int j = 0; j < k; j++) {
      sum_alpha += alpha(i % dims[0], j);
      
      if (alpha(i % dims[0], j) <= 0.0) {
        wrong_values = true;
        break;
      }

      pi[j] = R::rgamma(alpha(i % dims[0], j), 1.0);
      row_sum += pi[j];
    }
    
    if (wrong_values || ISNAN(sum_alpha + size[i % dims[1]]) ||
        size[i % dims[1]] < 0.0 || !isInteger(size[i % dims[1]], false)) {
      throw_warning = true;
      for (int j = 0; j < k; j++)
        x(i, j) = NA_REAL;
      continue;
    }
    
    if (size[i % dims[1]] == 0.0) {
      for (int j = 0; j < k; j++)
        x(i, j) = 0.0;
      continue;
    } 
    
    sum_p = 1.0;
    
    for (int j = 0; j < k-1; j++) {
      p_tmp = pi[j] / row_sum;
      x(i, j) = R::rbinom(size_left, p_tmp/sum_p);
      size_left -= x(i, j);
      sum_p -= p_tmp;
    }
    
    x(i, k-1) = size_left;
    
  }
  
  if (throw_warning)
    Rcpp::warning("NAs produced");
  
  return x;
}

