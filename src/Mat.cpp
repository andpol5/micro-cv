/*
 *  Micro Computer Vision library
 *  @author Andrei Polzounov
 */
#include <iostream>

#include "Mat.h"

using namespace MicroCv;

Mat::Mat()
: width_(0)
, height_(0)
, channels_(0)
{
}

Mat::Mat(const Mat& rhs)
: data_(rhs.data_)
, width_(rhs.width_)
, height_(rhs.height_)
, channels_(rhs.channels_)
{
}

Mat::Mat(int width, int height, int channels)
{
  resize(width, height, channels);
}

Mat::~Mat()
{
}

Mat& Mat::operator=(const Mat& rhs)
{
  data_ = rhs.data_;
  width_ = rhs.width_;
  height_ = rhs.height_;
  channels_ = rhs.channels_;
  return *this;
}

bool Mat::operator==(const Mat& rhs) const
{
  return (width_ == rhs.width_) && (height_ == rhs.height_)
      && (channels_ == rhs.channels_) && (data_ == rhs.data_);
}

bool Mat::isGrayscale() const
{
  return channels_ == 1;
}

uint8_t* Mat::data()
{
  return data_.data();
}

const uint8_t* Mat::data() const
{
  return data_.data();
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

std::vector<uint8_t>* Mat::vectorPtr()
{
  return &data_;
}

void Mat::resize(int width, int height, int channels)
{
  data_.clear();
  width_ = width;
  height_ = height;
  channels_ = channels;
  data_.resize(width_* height_*channels_);
}

void Mat::reserve(int width, int height, int channels)
{
  data_.clear();
  width_ = width;
  height_ = height;
  channels_ = channels;
  data_.reserve(width_* height_*channels_);
}
