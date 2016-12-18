#pragma once
/*
 *  Micro Computer Vision library
 *  @author Andrei Polzounov
 */
#include <vector>

namespace MicroCv
{

/*
 * The Matrix is a simple container for an RGB or grayscale image
 */
class Mat
{
public:
  Mat();
  Mat(const Mat& mat);
  Mat(int width, int height, int channels);

  virtual ~Mat();

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

  // Pixel data accessors
  uint8_t* data();
  const uint8_t* data() const;

  int width() const;
  int height() const;
  int channels() const;

  std::vector<uint8_t>* vectorPtr();

private:
  std::vector<uint8_t>* data_;
  int width_;
  int height_;
  int channels_;
};

};
