#pragma once
/*
 *  Image processing micro library
 *  @author Andrei Polzounov
 */
#include <memory>
#include <string>
#include <vector>

#include <boost/scoped_ptr.hpp>

namespace MicroCv
{

/*
 * The Matrix is a container for an RGB image
 */
class Matrix
{
public:
  Matrix();
  virtual ~Matrix();

  void resize(int width, int height, int channels);

  // RGB->Gray and vice versa
  void rgbToGray();
  void grayToRgb();
  bool isGrayscale() const;

  // Crop image
  bool crop(int x1, int y1, int x2, int y2);

  // Sobel edge detector
  // https://en.wikipedia.org/wiki/Sobel_operator
  bool sobelEdges();

  uint8_t* data();
  const uint8_t* data() const;

  int width() const;
  int height() const;
  int channels() const;

  void setData(const std::vector<uint8_t>& vector, int width, int height, int channels);

private:
  boost::scoped_ptr<uint8_t> data_;
  int width_;
  int height_;
  int channels_;
};

};
