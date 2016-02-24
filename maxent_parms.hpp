/*
 * Copyright (C) 1998-2016 ALPS Collaboration. See COPYRIGHT.TXT
 * All rights reserved. Use is subject to license terms. See LICENSE.TXT
 * For use in publications, see ACKNOWLEDGE.TXT
 */

#pragma once

#include "default_model.hpp"
#include "maxent_grid.hpp"
#include "maxent_kernel.hpp"
#include "maxent_matrix_def.hpp"

///This class has all the information about general analytic continuation things. It does not know about specifics of maxent.
class ContiParameters {

public:
  
  ///constructs the kernel and grid from the parameters p. Also reads in the data.
  ContiParameters(alps::params& p);
  
  ///value of the Matsubara data at index i
  double y(const int i) const { return y_[i]; }
  ///value of the Matsubara data
  const vector_type& y() const { return y_; }
  ///value of the covariance matrix
  double cov(const int i,const int j) const { return cov_(i,j); }
  ///value of the error (if covariance not given)
  double sigma(const int i) const { return sigma_[i]; }
  ///value of the inverse temperature
  double T() const { return T_; }
  ///number of data points
  int ndat() const { return ndat_; }
  ///number of frequency points on the real axis
  int nfreq() const { return nfreq_; }
  ///value of the Kernel matrix K(i,j).
  double K(const int i, const int j) const {return K_(i,j);}
  ///returns the entire kernel matrix
  const matrix_type& K() const { return K_; }

private:
  ///temperature
  const double T_;

  void read_data_from_text_file(const alps::params& p);
  void read_data_from_hdf5_file(const alps::params& p);
  void read_data_from_param_file(const alps::params& p);
  void read_covariance_matrix_from_text_file(const std::string& fname);

protected:

  void decompose_covariance_matrix(const alps::params& p);
  ///This function scales both the y (data) and the kernel with the errors
  void scale_data_with_error(const int ntab);
  ///This function removes the last element from the kernel and replaces it with a condition that enforces a strict normalization
  void enforce_strict_normalization(double sigma_normalization, double norm, const int ntab);

  ///number of fitting data points / size of y
  int ndat_; //TODO: find a better way of changing this. If we use Gl, we change the size of y
  ///number of real frequencies
  const int nfreq_;
  ///vector of Matsubara data
  vector_type y_;
  ///vector of errors on y
  vector_type sigma_;
  ///Kernel matrix
  matrix_type K_;
  ///covariance matrix
  matrix_type cov_;
  ///real frequency grid
  grid grid_;
  ///vector containing input matsubara or tau data
  vector_type inputGrid_;
};



///This class contains all of the maxent specific parameters, along with the singular value decomposed kernel.
class MaxEntParameters : public ContiParameters
{
public:
  ///constructs the maxent specific parameters out of parameters p
  MaxEntParameters(alps::params& p);
  
  const matrix_type& U() const { return U_; }
  const matrix_type& Vt() const { return Vt_; }
  const matrix_type& Sigma() const { return Sigma_; }
  double omega_coord(const int i) const { return omega_coord_[i]; }
  const vector_type& omega_coord() const { return omega_coord_; }
  double delta_omega(const int i) const { return delta_omega_[i]; }
  ///getter function for the number of singular values
  int ns() const { return ns_; }
  ///getter function for the default model
  const DefaultModel& Default() const { return *Default_; }

private:
  ///The default model
  boost::shared_ptr<DefaultModel> Default_;
  matrix_type U_;
  matrix_type Vt_;
  matrix_type Sigma_;
  vector_type omega_coord_;
  vector_type delta_omega_;
  ///the number of singular values
  int ns_;
  ///computed l cut off of legendre transform
  int lmax;
  ///compute the minimal chi2 that is possible given the SVD of the kernel
  void compute_minimal_chi2() const;
  ///reduce the matrices to the number of non-zero singular values
  void truncate_to_singular_space(const vector_type& S);
  ///take the kernel and compute its singular value decomposition
  void singular_value_decompose_kernel(bool verbose, vector_type& S);
  ///check G~-1/iw_{n} and default model back continues same limit
  void check_high_frequency_limit(const vector_type& y, const kernel_type kt);
};
