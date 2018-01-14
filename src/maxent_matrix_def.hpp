/*
 * Copyright (C) 1998-2018 ALPS Collaboration.
 * All rights reserved. Use is subject to license terms. See LICENSE.TXT
 * For use in publications, see ACKNOWLEDGE.TXT
 */

#pragma once

#include <alps/config.hpp> // needed to set up correct bindings
#include "maxent_config.hpp"
#include <Eigen/Core>

typedef Eigen::MatrixXd matrix_type;
typedef Eigen::MatrixXcd complex_matrix_type;
typedef Eigen::VectorXd vector_type;
typedef Eigen::MatrixXcd complex_vector_type;
typedef std::pair<vector_type, complex_vector_type> omega_complex_type;

///matrix-vector multiplication. 
inline vector_type maxent_prec_prod(const matrix_type &p, const vector_type &q) {
	return p*q;
}
///matrix-vector multiplication of transpose of matrix. 
inline vector_type maxent_prec_prod_trans(const matrix_type &p, const vector_type &q) {
	return p.transpose()*q;
}
///matrix-matrix multiplication. 
inline matrix_type maxent_prec_prod(const matrix_type &p, const matrix_type &q) {
	return p*q;
}
///matrix-matrix multiplication of transpose(p) with q. 
inline matrix_type maxent_prec_prod_trans(const matrix_type &p, const matrix_type &q) {
	return p.transpose()*q; 
}
