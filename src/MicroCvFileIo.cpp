/*
 *  Micro Computer Vision library
 *  @author Andrei Polzounov
 */
#include <iostream>
#include <string>

#include <boost/filesystem.hpp>
#include <boost/gil/extension/io/dynamic_io.hpp>
#include <boost/gil/extension/io/jpeg_io.hpp>
#include <boost/gil/extension/io/png_io.hpp>
#include <boost/gil/extension/io/tiff_io.hpp>
#include <boost/gil/gil_all.hpp>

#include "MicroCvFileIo.h"

using namespace MicroCv;

namespace
{
  // https://en.wikipedia.org/wiki/JPEG
  const char* jpgExtensions[6] = {".jpg", ".jpeg", ".jpe", ".jif", ".jfif", ".jfi"};
  const char* pngExtension = ".png";
  const char* tiffExtensions[2] = {".tif", ".tiff"};

  struct PixelRgbReader
  {
    PixelRgbReader(std::vector<uint8_t>* s) : storage(s) {}
    void operator()(boost::gil::rgb8_pixel_t p) const {
      using boost::gil::at_c;
      storage->push_back(at_c<0>(p));
      storage->push_back(at_c<1>(p));
      storage->push_back(at_c<2>(p));
    }
    std::vector<uint8_t>* storage;
  };

  struct PixelWriter
  {
    PixelWriter(const uint8_t* pixels) : pixelPtr(pixels) {}

    void operator()(boost::gil::rgb8_pixel_t& p)
    {
      using boost::gil::at_c;
      at_c<0>(p) = *pixelPtr++;
      at_c<1>(p) = *pixelPtr++;
      at_c<2>(p) = *pixelPtr++;
    }

    void operator()(boost::gil::gray8_pixel_t& p)
    {
      using boost::gil::at_c;
      at_c<0>(p) = *pixelPtr++;
    }
    const uint8_t* pixelPtr;
  };

  void readJpeg(const std::string& filename, Mat& mat, bool& /*readOk*/)
  {
    using namespace boost::gil;
    rgb8_image_t image;
    jpeg_read_and_convert_image(filename, image);
    auto imageView = boost::gil::view(image);
    mat.resize(imageView.width(), imageView.height(), imageView.num_channels());

    std::vector<uint8_t>* ptr = mat.vectorPtr();
    boost::gil::for_each_pixel(imageView, PixelRgbReader(ptr));
  }

  void readPng(const std::string& filename, Mat& mat, bool& /*readOk*/)
  {
    using namespace boost::gil;
    rgb8_image_t image;
    png_read_and_convert_image(filename, image);
    auto imageView = boost::gil::view(image);
    mat.resize(imageView.width(), imageView.height(), imageView.num_channels());

    std::vector<uint8_t>* ptr = mat.vectorPtr();
    boost::gil::for_each_pixel(imageView, PixelRgbReader(ptr));
  }

  void readTiff(const std::string& filename, Mat& mat, bool& /*readOk*/)
  {
    using namespace boost::gil;
    rgb8_image_t image;
    tiff_read_and_convert_image(filename, image);
    auto imageView = boost::gil::view(image);
    mat.resize(imageView.width(), imageView.height(), imageView.num_channels());

    std::vector<uint8_t>* ptr = mat.vectorPtr();
    boost::gil::for_each_pixel(imageView, PixelRgbReader(ptr));
  }
}

Mat readFromFile(const std::string& filename, bool& readOk)
{
  // Get the filename extension
  std::string extension = boost::filesystem::extension(filename);
  std::cout << "extension: " << extension << std::endl;

  Mat mat;

  bool isJpeg = false;
  for(int i = 0; i < 6; i++)
  {
    if(extension == std::string(jpgExtensions[i]))
    {
      isJpeg = true;
    }
  }
  if(isJpeg)
  {
    readJpeg(filename, mat, readOk);
    return mat;
  }

  if(extension == std::string(pngExtension))
  {
    readPng(filename, mat, readOk);
    return mat;
  }

  bool isTiff = false;
  for(int i = 0; i < 6; i++)
  {
    if(extension == std::string(tiffExtensions[i]))
    {
      isTiff = true;
    }
  }
  if(isTiff)
  {
    readTiff(filename, mat, readOk);
    return mat;
  }

  readOk = false;
  return mat;
}

bool writeJpeg(const std::string& filename, const Mat& mat)
{
  using namespace boost::gil;
  rgb8_image_t imgRgb(mat.width(), mat.height(), mat.channels());
  rgb8_view_t viewRgb = boost::gil::view(imgRgb);
  const uint8_t* dataPtr = mat.data();
  boost::gil::for_each_pixel(viewRgb, PixelWriter(dataPtr));
  boost::gil::jpeg_write_view(filename, viewRgb);
  return true;
}

bool writePng(const std::string& /*filename*/, const Mat& /*mat*/)
{
  return true;
}

bool writeTiff(const std::string& /*filename*/, const Mat& /*mat*/)
{
  return true;
}
