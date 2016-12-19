#pragma once
/*
 *  Micro Computer Vision library
 *  @author Andrei Polzounov
 */

#include "Mat.h"

namespace MicroCv
{
  // Crop image
  void cropMat(Mat& mat, int x1, int y1, int x2, int y2);

  // Color space conversions
  Mat rgbToGray(const Mat& inputMat);
  Mat grayToRgb(const Mat& inputMat);

  // Edge detection (high-pass filter)
  Mat sobelEdgeDetector(const Mat& inputMat);
};
