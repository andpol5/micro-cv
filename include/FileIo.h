#pragma once
/*
 *  Micro Computer Vision library
 *  @author Andrei Polzounov
 */
#include <cstdint>
#include <string>

#include "Mat.h"

namespace MicroCv
{
  enum ImageFileType
  {
    Jpeg,
    Png,
    Tiff,
    Unsupported
  };
  // Specific file types
  Mat readMatFromFile(const std::string& filename, ImageFileType type, bool& readOk);
  bool writeMatToFile(const std::string& filename, const Mat& mat, ImageFileType type);

  // Try to figure out filetype from string
  ImageFileType imageTypeFromFilename(const std::string& filename);
};
