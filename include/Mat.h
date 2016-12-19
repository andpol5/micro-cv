#pragma once
/*
 *  Micro Computer Vision library
 *  @author Andrei Polzounov
 */
#include <cstdint>
#include <vector>

namespace MicroCv
{
/*
 * Mat is a simple container for an RGB or grayscale image
 */
class Mat
{
public:
  Mat();
  Mat(const Mat& rhs);
  Mat(int width, int height, int channels);
  virtual ~Mat();

  Mat& operator=(const Mat& rhs);
  bool operator==(const Mat& rhs) const;

  bool isGrayscale() const;

  // Pixel data accessors
  uint8_t* data();
  const uint8_t* data() const;

  int width() const;
  int height() const;
  int channels() const;

  std::vector<uint8_t>* vectorPtr();
  void resize(int width, int height, int channels);
  void reserve(int width, int height, int channels);

private:
  std::vector<uint8_t> data_;
  int width_;
  int height_;
  int channels_;
};

};
