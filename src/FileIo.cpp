/*
 *  Micro Computer Vision library
 *  @author Andrei Polzounov
 */
#include <iostream>
#include <string>
#include <vector>

#include <boost/filesystem.hpp>
#include <boost/gil/extension/io/jpeg_io.hpp>
#include <boost/gil/extension/io/png_io.hpp>
#include <boost/gil/extension/io/tiff_io.hpp>
#include <boost/gil/gil_all.hpp>

#include "FileIo.h"

using namespace MicroCv;

namespace
{
  // Possible JPEG extensions from: https://en.wikipedia.org/wiki/JPEG
  const std::vector<std::string> JPEG_EXTENSIONS = {".jpg", ".jpeg", ".jpe", ".jif", ".jfif", ".jfi"};
  const std::vector<std::string> TIFF_EXTENSIONS = {".tif", ".tiff"};
  const std::string PNG_EXTENSION = ".png";

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

}

Mat MicroCv::readMatFromFile(const std::string& filename, ImageFileType type, bool& readOk)
{
  using namespace boost::gil;
  Mat mat;

  // Check if file exists
  if(!boost::filesystem::exists(filename))
  {
    std::cout << "File: " << filename << " not found" << std::endl;
    readOk = false;
    return mat;
  }

  // Read into RGB image (unsigned char type)
  rgb8_image_t image;
  try
  {
    if(type == ImageFileType::Jpeg)
    {
      jpeg_read_and_convert_image(filename, image);
    }
    else if(type == ImageFileType::Png)
    {
      png_read_and_convert_image(filename, image);
    }
    else if(type == ImageFileType::Tiff)
    {
      tiff_read_and_convert_image(filename, image);
    }
    else
    {
      readOk = false;
      std::cout << "File format: " << boost::filesystem::extension(filename)
          << " not supported" << std::endl;
    }
  }
  catch(std::exception& e)
  {
    std::cerr << "ERROR: " << e.what() << std::endl;
    readOk = false;
  }
  auto imageView = boost::gil::view(image);

  mat.reserve(imageView.width(), imageView.height(), imageView.num_channels());
  std::vector<uint8_t>* data = mat.vectorPtr();
  boost::gil::for_each_pixel(imageView, PixelRgbReader(data));
  readOk = true;
  return mat;
}

bool MicroCv::writeMatToFile(const std::string& filename, const Mat& mat, ImageFileType type)
{
  using namespace boost::gil;
  if(mat.channels() == 3)
  {
    rgb8_image_t image(mat.width(), mat.height(), mat.channels());
    rgb8_view_t viewRgb = boost::gil::view(image);
    const uint8_t* dataPtr = mat.data();
    boost::gil::for_each_pixel(viewRgb, PixelWriter(dataPtr));
    if(type == ImageFileType::Jpeg)
    {
      boost::gil::jpeg_write_view(filename, viewRgb);
    }
    else if(type == ImageFileType::Png)
    {
      boost::gil::png_write_view(filename, viewRgb);
    }
    else if(type == ImageFileType::Tiff)
    {
      boost::gil::tiff_write_view(filename, viewRgb);
    }
    else
    {
      std::cout << "File format: " << boost::filesystem::extension(filename)
          << " not supported" << std::endl;
      return false;
    }
  }
  else if(mat.channels() == 1)
  {
    gray8_image_t image(mat.width(), mat.height(), mat.channels());
    gray8_view_t viewGray = boost::gil::view(image);
    const uint8_t* dataPtr = mat.data();
    boost::gil::for_each_pixel(viewGray, PixelWriter(dataPtr));
    if(type == ImageFileType::Jpeg)
    {
      boost::gil::jpeg_write_view(filename, viewGray);
    }
    else if(type == ImageFileType::Png)
    {
      boost::gil::png_write_view(filename, viewGray);
    }
    else
    {
      boost::gil::tiff_write_view(filename, viewGray);
    }
  }
  else
  {
    std::cout << "Writing " << mat.channels() << " channel images is not supported" << std::endl;
  }
  return true;
}

MicroCv::ImageFileType MicroCv::imageTypeFromFilename(const std::string& filename)
{
  // Get the filename extension
  std::string extension = boost::filesystem::extension(filename);

  // Check JPEG
  for(auto itr = JPEG_EXTENSIONS.begin(); itr != JPEG_EXTENSIONS.end(); ++itr)
  {
    if(extension == std::string(*itr))
    {
      return ImageFileType::Jpeg;
    }
  }

  // Check PNG
  if(extension == PNG_EXTENSION)
  {
    return ImageFileType::Png;
  }

  // Check TIFF
  for(auto itr = TIFF_EXTENSIONS.begin(); itr != TIFF_EXTENSIONS.end(); ++itr)
  {
    if(extension == std::string(*itr))
    {
      return ImageFileType::Tiff;
    }
  }

  return ImageFileType::Unsupported;
}
