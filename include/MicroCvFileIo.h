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
  Mat readFromFile(const std::string& filename, bool& readOk);

  bool writeJpeg(const std::string& filename, const Mat& mat);
  bool writePng(const std::string& filename, const Mat& mat);
  bool writeTiff(const std::string& filename, const Mat& mat);
};
