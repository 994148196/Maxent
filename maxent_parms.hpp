/*****************************************************************************
*
* ALPS Project Applications
*
* Copyright (C) 2010 by Sebastian Fuchs <fuchs@comp-phys.org>
*                       Thomas Pruschke <pruschke@comp-phys.org>
*                       Matthias Troyer <troyer@comp-phys.org>
*
* This software is part of the ALPS Applications, published under the ALPS
* Application License; you can use, redistribute it and/or modify it under
* the terms of the license, either version 1 or (at your option) any later
* version.
* 
* You should have received a copy of the ALPS Application License along with
* the ALPS Applications; see the file LICENSE.txt. If not, the license is also
* available from http://alps.comp-phys.org/.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR 
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
* FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT 
* SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE 
* FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE, 
* ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER 
* DEALINGS IN THE SOFTWARE.
*
*****************************************************************************/

#ifndef ALPS_TOOL_MAXENT_PARMS_HPP
#define ALPS_TOOL_MAXENT_PARMS_HPP

#include <alps/config.h> // needed to set up correct bindings
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/bindings/ublas.hpp>
#include <boost/numeric/ublas/io.hpp>
#include "default_model.hpp"
#include "maxent_grid.hpp"

///This class has all the information about general analytic continuation things. It does not know about specifics of maxent.
class ContiParameters {

public:
  
  typedef boost::numeric::ublas::matrix<double, boost::numeric::ublas::column_major> matrix_type;
  typedef boost::numeric::ublas::vector<double> vector_type;
  typedef boost::numeric::ublas::vector<std::complex<double> > complex_vector_type;
  typedef std::pair<vector_type, complex_vector_type> omega_complex_type;

  ///constructs the kernel and grid from the parameters p. Also reads in the data.
  ContiParameters(const alps::params& p);
  
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
  ///number of fitting data points / size of y
  int ndat_;
  ///number of real frequencies
  const int nfreq_;

  void read_data_from_text_file(const alps::params& p);
  void read_data_from_hdf5_file(const alps::params& p);
  void read_data_from_param_file(const alps::params& p);

protected:

  void setup_kernel(const alps::params& p, const int ntab, const vector_type& freq);
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
};



///This class contains all of the maxent specific parameters
class MaxEntParameters : public ContiParameters
{
public:
  ///constructs the maxent specific parameters out of parameters p
  MaxEntParameters(const alps::params& p);
  
  const matrix_type& U() const { return U_; }
  const matrix_type& Vt() const { return Vt_; }
  const matrix_type& Sigma() const { return Sigma_; }
  double omega_coord(const int i) const { return omega_coord_[i]; }
  const vector_type& omega_coord() const { return omega_coord_; }
  double delta_omega(const int i) const { return delta_omega_[i]; }
  int ns() const { return ns_; }
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
};

    
#endif
