#pragma once
/*
 *  Image processing micro library
 *  @author Andrei Polzounov
 */
#include <string>


#include <boost/mpl/vector.hpp>
#include <boost/gil/gil_all.hpp>

namespace MicroCv
{

/*
 * The Matrix is an N-dimensional container of the image
 */
template<typename DataT>
class Matrix
{
public:
  Matrix(const std::string& filename)
  : filename_(filename), data_(NULL)
  {
    typedef boost::mpl::vector<boost::gil::rgb8_image_t,
        boost::gil::rgb16_image_t> my_img_types;
    boost::gil::rgb16_image_t image;
    boost::gil::jpeg_read_and_convert_image(filename.c_str(), image);
  }
  virtual ~Matrix()
  {
  }


private:
  const std::string& filename_;
  DataT* data_;
};

};
