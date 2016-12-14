#pragma once
/*
 *  Image processing micro library
 *  @author Andrei Polzounov
 */
#include <string>

#include "Matrix.h"

namespace MicroCv
{
  template<typename DataT>
  Matrix<DataT> readFromFile(const std::string& filename)
  {
    Matrix<DataT> mat;
    return mat;
  }
};
