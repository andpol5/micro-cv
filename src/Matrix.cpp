/*
 *  Image processing micro library
 *  @author Andrei Polzounov
 */
#include <cstdio>
#include <cstring>
#include <iostream>
#include <string>

#include "Matrix.h"

using namespace MicroCv;

Matrix::Matrix()
: width_(0)
, height_(0)
, channels_(0)
{
}

Matrix::~Matrix()
{
  data_.reset();
}

void Matrix::resize(int width, int height, int channels)
{
  width_ = width;
  height_ = height;
  channels_ = channels;
  data_.reset(new uint8_t[width*height*channels]);
}

void Matrix::rgbToGray()
{
  // If not already grayscale
  if(channels_ > 1)
  {
    // Copy current data to a temporary buffer
    int numPixels = width_ * height_* channels_;
    uint8_t buffer[numPixels];
    memcpy(buffer, data(), numPixels * sizeof(uint8_t));

    // Resize the old data to new size
    data_.reset(new uint8_t[width_* height_]);

    // Add up each color from each channel and divide by number of channels
    uint8_t* buffPtr = buffer;
    uint8_t* dataPtr = data();
    for(int pixel = 0; pixel < numPixels; pixel+=channels_, buffPtr+=channels_, dataPtr++)
    {
      double newVal = static_cast<double>(*buffPtr) +
          static_cast<double>(*(buffPtr+1)) + static_cast<double>(*(buffPtr+2));
      *dataPtr = static_cast<uint8_t>(newVal / 3.0);
    }
    channels_ = 1;
  }
}

void Matrix::grayToRgb()
{
  if(isGrayscale())
  {
    channels_ = 3;

    // Copy current data to a temporary buffer
    int numPixels = width_ * height_;
    uint8_t buffer[numPixels];
    memcpy(buffer, data(), numPixels* sizeof(uint8_t));

    // Resize the old data to new size
    data_.reset(new uint8_t[width_* height_*channels_]);

    // Add up each color from each channel and divide by number of channels
    uint8_t* buffPtr = buffer;
    uint8_t* dataPtr = data();
    for(int pixel = 0; pixel < numPixels; pixel++, buffPtr++, dataPtr+=channels_)
    {
      *dataPtr = *buffPtr;
      *(dataPtr+1) = *buffPtr;
      *(dataPtr+2) = *buffPtr;
    }
  }
}

bool Matrix::isGrayscale() const
{
  return channels_ == 1;
}

bool Matrix::crop(int x1, int y1, int x2, int y2)
{
  // Check bounds
  if(x1 < 0 || x1 >= x2)
    return false;
  if(x2 <= x1 || x2 >= width_)
    return false;
  if(y1 < 0 || y1 >= y2)
    return false;
  if(y2 <= y1 || y2 >= height_)
    return false;

  // Copy current data to a temporary buffer
  int numPixels = width_ * height_*channels_;
  uint8_t buffer[numPixels];
  memcpy(buffer, data(), numPixels* sizeof(uint8_t));

  // Old width is needed to keep track of pixel indices
  int oldWidth = width_;

  // Resize memory
  resize(x2 - x1, y2 - y1, channels_);

  uint8_t* dataPtr = data();
  for(int y = y1; y < y2; y++)
  {
    uint8_t* buffPtr = buffer + y*oldWidth*channels_ + x1*channels_;
    for(int x = x1*channels_; x < x2*channels_; x++, buffPtr++, dataPtr++)
    {
      *dataPtr = *buffPtr;
    }
  }
  return true;
}

bool Matrix::sobelEdges()
{
  if(!isGrayscale())
  {
    return false;
  }

  // Copy current data to a temporary buffer
  int numPixels = width_ * height_*channels_;
  uint8_t buffer[numPixels];
  memcpy(buffer, data(), numPixels* sizeof(uint8_t));

  // Temporary output
  int numPixels = width_ * height_;
  std::vector<int16_t> tmp;
  tmp.resize(numPixels);

  uint8_t* ptr = data();
  for(auto itr = tmp.begin(), end = tmp.end(); itr != end; ++itr, ++ptr)
  {
    *itr = *ptr;
  }

  // Define x and y derivative kernels (kernel size == 3)
  const int K = 3;
  static const int8_t Gx[3][3] =
    {{-1, 0, 1},
     {-2, 0, 2},
     {-1, 0, 1}};

  static const int8_t Gy[3][3] =
    {{1, 2, 1},
     {0, 0, 0},
     {-1, -2, -1}};

  // Simple convolution
  for (int i = K / 2; i < height_ - K / 2; ++i) // iterate through image
  {
    for (int j = K / 2; j < width_ - K / 2; ++j)
    {
      int sum = 0; // sum will be the sum of input data * coeff terms

      for (int ii = - K / 2; ii <= K / 2; ++ii) // iterate over kernel
      {
        for (int jj = - K / 2; jj <= K / 2; ++jj)
        {
          int data = buffer[i + ii][j +jj];
          int coeff = Gx[ii + K / 2][jj + K / 2];

          sum += data * coeff;
        }
      }
      out[i][j] = sum / scale; // scale sum of convolution products and store in output
    }




  return true;
}

uint8_t* Matrix::data()
{
  return data_.get();
}

const uint8_t* Matrix::data() const
{
  return data_.get();
}

int Matrix::width() const
{
  return width_;
}

int Matrix::height() const
{
  return height_;
}

int Matrix::channels() const
{
  return channels_;
}

void Matrix::setData(const std::vector<uint8_t>& vector, int width, int height, int channels)
{
  resize(width, height, channels);
  memcpy(data(), vector.data(), width*height*channels*sizeof(uint8_t));
}
