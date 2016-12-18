/*
 *  Micro Computer Vision library
 *  @author Andrei Polzounov
 */
#include <cstdio>
#include <cstring>
#include <iostream>
#include <string>

#include "Mat.h"

using namespace MicroCv;

namespace
{
  // Optimized way of setting pixel to 0 < x < 255 range
  inline uint8_t clampToPixelRange(int val)
  {
    val &= -(val >= 0);
    return static_cast<uint8_t>(val | ((255 - val) >> 31));
  }

}

Mat::Mat()
: data_(new std::vector<uint8_t>())
, width_(0)
, height_(0)
, channels_(0)
{
}

Mat::Mat(const Mat& mat)
: width_(mat.width_)
, height_(mat.height_)
, channels_(mat.channels_)
{
  data_ = new std::vector<uint8_t>(*(mat.data_));
}

Mat::Mat(int width, int height, int channels)
{
  resize(width, height, channels);
}

//Mat::Mat(std::vector<uint8_t>* data, int width, int height, int channels)
//{
//  // Takes posession of the data
//  setData(data, width, height, channels);
//}

Mat::~Mat()
{
  delete data_;
}

void Mat::resize(int width, int height, int channels)
{
  data_->clear();
  width_ = width;
  height_ = height;
  channels_ = channels;
  data_->resize(width*height*channels);
}

void Mat::rgbToGray()
{
  // If not already grayscale
  if(channels_ > 1)
  {
    // Copy current data to a temporary buffer
    int numPixels = width_ * height_* channels_;
    uint8_t buffer[numPixels];
    memcpy(buffer, data(), numPixels * sizeof(uint8_t));

    // Resize the old data to new size
    data_->clear();
    data_->resize(width_* height_);

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

void Mat::grayToRgb()
{
  if(isGrayscale())
  {
    channels_ = 3;

    // Copy current data to a temporary buffer
    int numPixels = width_ * height_;
    uint8_t buffer[numPixels];
    memcpy(buffer, data(), numPixels* sizeof(uint8_t));

    // Resize the old data to new size
    data_->clear();
    data_->resize(width_* height_*channels_);

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

bool Mat::isGrayscale() const
{
  return channels_ == 1;
}

bool Mat::crop(int x1, int y1, int x2, int y2)
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

bool Mat::sobelEdges()
{
  if(!isGrayscale())
  {
    return false;
  }

  // Copy current data to a temporary buffer
  int numPixels = width_ * height_*channels_;
  uint8_t buffer[numPixels];
  memcpy(buffer, data(), numPixels* sizeof(uint8_t));

  uint8_t* dataPtr = data();

  // Define x and y derivative kernels (kernel size == 3)
  const int K = 3;
  const int halfK = K / 2;
  static const int8_t Gx[9] =
    {-1, 0, 1,
     -2, 0, 2,
     -1, 0, 1};

  static const int8_t Gy[9] =
    {1,  2,  1,
     0,  0,  0,
    -1, -2, -1};

  // Simple 2D convolution - this can be optimized by using CUDA api calls or by consecutive 1D convs
  for (int y = halfK; y < height_ - halfK; ++y) // iterate through image
  {
    for (int x = halfK; x < width_ - halfK; ++x)
    {
      int gxSum = 0; // Sum of the convolution is
      int gySum = 0; // sum will be the sum of input data * coeff terms

      for (int yy = - halfK; yy <= halfK; ++yy) // iterate over kernel
      {
        for (int xx = - halfK; xx <= halfK; ++xx)
        {
          int data = *(buffer + (y+yy)*width_ + (x+xx));
          int gxCoeff = *(Gx + (yy + halfK)*K + xx + halfK);
          int gyCoeff = *(Gy + (yy + halfK)*K + xx + halfK);

          gxSum += data * gxCoeff;
          gySum += data * gyCoeff;
        }
      }
      // The sobel gradient magnituted is the sum of the absolute magnitudes of
      // the derivates in x and y (Gx + Gy)
      // Technically, it is G = sqrt(Gx^2 + Gy^x), but the sum of absval is easier to calculate
      int gradientMagnitude = abs(gxSum) + abs(gySum);

      // This value may be outside of the pixel range 0 < x < 255
      *(dataPtr + y*width_ + x) = clampToPixelRange(gradientMagnitude);
    }
  }

  return true;
}

uint8_t* Mat::data()
{
  return data_->data();
}

const uint8_t* Mat::data() const
{
  return data_->data();
}

int Mat::width() const
{
  return width_;
}

int Mat::height() const
{
  return height_;
}

int Mat::channels() const
{
  return channels_;
}

//void Mat::setData(std::vector<uint8_t>* data, int width, int height, int channels)
//{
//  width_ = width;
//  height_ = height;
//  channels_ = channels;
//  // Takes possession of the vector
//  data_ = data;
//}

std::vector<uint8_t>* Mat::vectorPtr()
{
  return data_;
}
