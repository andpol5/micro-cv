/*
 *  Micro Computer Vision library
 *  @author Andrei Polzounov
 */
#include <iostream>
#include <string>

#include "ImageProcessing.h"

namespace
{
  // Optimized way of setting pixel to 0 < x < 255 range
  inline uint8_t clampToPixelRange(int val)
  {
    val &= -(val >= 0);
    return static_cast<uint8_t>(val | ((255 - val) >> 31));
  }
}

using namespace MicroCv;

void MicroCv::cropMat(Mat& mat, int x1, int y1, int x2, int y2)
{
  const int width = mat.width();
  const int height = mat.height();
  const int channels = mat.channels();

  // Check bounds
  if(x1 < 0 || x1 >= x2)
    return;
  if(x2 <= x1 || x2 >= width)
    return;
  if(y1 < 0 || y1 >= y2)
    return;
  if(y2 <= y1 || y2 >= height)
    return;

  // Make a copy
  Mat inputMat = mat;

  // Resize the original mat
  mat.resize(x2 - x1, y2 - y1, channels);

  uint8_t* outPtr = mat.data();
  // Only iterate over the pixels in the cropped region
  for(int y = y1; y < y2; y++)
  {
    const uint8_t* inPtr = inputMat.data() + y*width*channels + x1*channels;
    for(int x = x1*channels; x < x2*channels; x++, inPtr++, outPtr++)
    {
      *outPtr = *inPtr;
    }
  }
}

Mat MicroCv::rgbToGray(const Mat& inputMat)
{
  Mat outputMat;
  // If not already grayscale
  int numChannels = inputMat.channels();
  if(numChannels > 1)
  {
    // Copy the const reference
    Mat in = inputMat;

    int numPixels = inputMat.width() * inputMat.height();

    // Resize the output data
    outputMat.resize(inputMat.width(), inputMat.height(), 1);

    // Add up each color from each channel and divide by number of channels
    const uint8_t* inPtr = inputMat.data();
    uint8_t* outPtr = outputMat.data();

    const double oneThird = 1.0 / 3.0;
    for(int pixel = 0; pixel < numPixels; pixel++, inPtr+=numChannels, outPtr++)
    {
      // Average the 3 consecutive RGB pixels
      double newVal = static_cast<double>(*inPtr) +
          static_cast<double>(*(inPtr+1)) + static_cast<double>(*(inPtr+2));
      *outPtr = static_cast<uint8_t>(newVal * oneThird);
    }
  }
  else if(inputMat.channels() == 1)
  {
    outputMat = inputMat;
  }
  return outputMat;
}

Mat MicroCv::grayToRgb(const Mat& inputMat)
{
  Mat outputMat;
  if(inputMat.isGrayscale())
  {
    int numChannels = 3;

    int numPixels = inputMat.width() * inputMat.height();

    // Resize the output data
    outputMat.resize(inputMat.width(), inputMat.height(), numChannels);

    // Copy the gray value to all 3 channels
    const uint8_t* inPtr = inputMat.data();
    uint8_t* outPtr = outputMat.data();
    for(int pixel = 0; pixel < numPixels; pixel++, inPtr++, outPtr+=numChannels)
    {
      *outPtr = *inPtr;
      *(outPtr+1) = *inPtr;
      *(outPtr+2) = *inPtr;
    }
  }
  else if(inputMat.channels() == 3)
  {
    outputMat = inputMat;
  }
  return outputMat;
}

Mat MicroCv::sobelEdgeDetector(const Mat& inputMat)
{
  Mat grayMat;
  Mat outputMat;
  if(!inputMat.isGrayscale())
  {
    grayMat = rgbToGray(inputMat);
  }
  else
  {
    grayMat = inputMat;
  }

  // Resize the output data
  outputMat.resize(grayMat.width(), grayMat.height(), grayMat.channels());

  const uint8_t* inPtr = grayMat.data();
  uint8_t* outPtr = outputMat.data();

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

  const int width = grayMat.width();
  const int height = grayMat.height();

  // Simple 2D convolution - this can be optimized by using CUDA api calls or by consecutive 1D convs
  for (int y = halfK; y < height - halfK; ++y) // iterate through image
  {
    for (int x = halfK; x < width - halfK; ++x)
    {
      int gxSum = 0; // Sum of the convolution is
      int gySum = 0; // sum will be the sum of input data * coeff terms

      for (int yy = - halfK; yy <= halfK; ++yy) // iterate over kernel
      {
        for (int xx = - halfK; xx <= halfK; ++xx)
        {
          int data = *(inPtr + (y+yy)*width + (x+xx));
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
      *(outPtr + y*width + x) = clampToPixelRange(gradientMagnitude);
    }
  }
  return outputMat;
}
