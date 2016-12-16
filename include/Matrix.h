#pragma once
/*
 *  Image processing micro library
 *  @author Andrei Polzounov
 */
#include <memory>
#include <string>

#include <boost/scoped_ptr.hpp>

namespace MicroCv
{

/*
 * The Matrix is an N-dimensional container of the image
 */
class Matrix
{
public:
  Matrix();
  virtual ~Matrix();

  void resize(int width, int height, int channels);
  void toGrayscale();

  uint16_t* data();
  const uint16_t* data() const;

  int width() const;
  int height() const;
  int channels() const;

private:
  boost::scoped_ptr<uint16_t> data_;
  int width_;
  int height_;
  int channels_;
};

};
