/*
 *  Image processing micro library
 *  @author Andrei Polzounov
 */
#include <iostream>
#include <string>

#include "Matrix.h"

using namespace MicroCv;

template<typename DataT>
Matrix<DataT>::Matrix(const std::string& filename)
: filename_(filename)
, data_(NULL)
{
}

template<typename DataT>
Matrix<DataT>::~Matrix()
{
}
