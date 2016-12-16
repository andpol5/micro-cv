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
  data_.reset(new uint16_t[width*height*channels]);
}

void Matrix::toGrayscale()
{
  // If not already grayscale
  if(channels_ > 1)
  {
    // Copy current data to a temporary buffer
    int numPixels = width_ * height_* channels_;
    uint16_t buffer[numPixels];
    memcpy(buffer, data(), numPixels * sizeof(uint16_t));

    // Resize the old data to new size
    data_.reset(new uint16_t[width_* height_]);

    // Add up each color from each channel and divide by number of channels
    uint16_t* buffPtr = buffer;
    uint16_t* dataPtr = data();
    for(int pixel = 0; pixel < numPixels; pixel+=channels_, buffPtr+=channels_, dataPtr++)
    {
      double newVal = static_cast<double>(*buffPtr) +
          static_cast<double>(*(buffPtr+1)) + static_cast<double>(*(buffPtr+2));
      *dataPtr = static_cast<uint16_t>(newVal / 3.0);
    }
  }
}

uint16_t* Matrix::data()
{
  return data_.get();
}

const uint16_t* Matrix::data() const
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
