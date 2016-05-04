/*
 * Copyright (C) 1998-2016 ALPS Collaboration. See COPYRIGHT.TXT
 * All rights reserved. Use is subject to license terms. See LICENSE.TXT
 * For use in publications, see ACKNOWLEDGE.TXT
 */

#pragma once
#include <alps/hdf5/archive.hpp>
#include <alps/utilities/cast.hpp>
#include <Eigen/Core>
#include <algorithm>

//TODO: make a complete implementation of Eigen::vector and Eigen::Matrix
namespace alps {
    namespace hdf5 {
        template<> struct is_continuous<Eigen::VectorXd>
            : public is_continuous<Eigen::VectorXd::Scalar>
        {};
        template<> struct is_continuous<Eigen::VectorXd const >
            : public is_continuous<Eigen::VectorXd::Scalar>
        {};
        namespace detail {
          template<> struct get_extent<Eigen::VectorXd> {
                  static std::vector<std::size_t> apply(Eigen::VectorXd const & value) {
                      using alps::hdf5::get_extent;
                      std::vector<std::size_t> result(1, value.size());
                      if (value.size()) {
                          std::vector<std::size_t> first(get_extent(value[0]));
                          if (true)
                              for(std::size_t i=1;i<value.size();i++) {
                                  std::vector<std::size_t> size(get_extent(value[i]));
                                  if (
                                         first.size() != size.size()
                                      || !std::equal(first.begin(), first.end(), size.begin())
                                  )
                                      throw archive_error("no rectangular matrix" + ALPS_STACKTRACE);
                              }
                          std::copy(first.begin(), first.end(), std::back_inserter(result));
                      }
                      return result;
                  }
          };
        }
        void save(
            archive &ar
          , std::string const & path
          , Eigen::VectorXd &value
          , std::vector<std::size_t> size = std::vector<std::size_t>()
          , std::vector<std::size_t> chunk =std::vector<std::size_t>()
          , std::vector<std::size_t> offset = std::vector<std::size_t>()
       ) {
            using alps::cast;
            if (ar.is_group(path))
                ar.delete_group(path);
            if (is_continuous<Eigen::VectorXd>::value && value.size() == 0)
                ar.write(path, static_cast<Eigen::VectorXd::Scalar const *>(NULL), std::vector<std::size_t>());
            else if (is_continuous<Eigen::VectorXd>::value) {
                std::vector<std::size_t> extent(get_extent(value));
                std::copy(extent.begin(), extent.end(), std::back_inserter(size));
                std::copy(extent.begin(), extent.end(), std::back_inserter(chunk));
                std::fill_n(std::back_inserter(offset), extent.size(), 0);
                ar.write(path, value.data(), size, chunk, offset);

            } else if (value.size() == 0)
                ar.write(path, static_cast<int const *>(NULL), std::vector<std::size_t>());
        }
    }
}
