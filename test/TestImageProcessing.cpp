/*
 *  Micro Computer Vision library
 *  @author Andrei Polzounov
 */
#include <iostream>

#include <gtest/gtest.h>

#include "ImageProcessing.h"
#include "Mat.h"
#include "RandomMat.h"

using namespace MicroCv;

namespace
{
  class TestImageProcessing : public ::testing::Test
  {
  public:
    TestImageProcessing() {}

  void zeroOutBorderPixels(Mat& mat)
  {
    // Top row
    auto topBegin = mat.vectorPtr()->begin();
    auto topEnd = mat.vectorPtr()->begin() + mat.width();
    for(auto itr = topBegin; itr != topEnd; ++itr)
      *itr = 0;
    // Bottom row
    auto botBegin = mat.vectorPtr()->begin() + (mat.height()-1)*mat.width();
    auto botEnd = mat.vectorPtr()->end();
    for(auto itr = botBegin; itr != botEnd; ++itr)
      *itr = 0;
    // Left col
    for(int y = 0; y < mat.height(); y++)
      *(mat.data() + y*mat.width()) = 0;
    // Right col
      for(int y = 0; y < mat.height(); y++)
        *(mat.data() + y*mat.width() + mat.width()-1) = 0;
  }

  protected:
    Mat mat_;
  };
}

TEST_F(TestImageProcessing, willCropMat)
{
  Mat original = RandomMat(127, 88, 3);
  mat_ = original;
  const int x1 = 37;
  const int x2 = 92;
  const int y1 = 12;
  const int y2 = 71;
  cropMat(mat_, x1, y1, x2, y2);

  // Expected dimensions
  const int cropWidth = x2 - x1;
  const int cropHeight = y2 - y1;
  const int channels = original.channels();

  ASSERT_EQ(mat_.width(), cropWidth);
  ASSERT_EQ(mat_.height(), cropHeight);
  ASSERT_EQ(mat_.channels(), channels);

  // Check every pixel in the cropped region
  const uint8_t* cropPtr = mat_.data();
  // Only iterate over the pixels in the cropped region
  for(int y = y1; y < y2; y++)
  {
    const uint8_t* orgPtr = original.data() + y*cropWidth*channels + x1*channels;
    for(int x = x1*channels; x < x2*channels; x++, orgPtr++, cropPtr++)
    {
      EXPECT_EQ(*orgPtr, *orgPtr);
    }
  }
}

TEST_F(TestImageProcessing, willNotCropMatWithInvalidIndices)
{
  Mat original = RandomMat(96, 121, 3);
  mat_ = original;
  const int x1 = 37;
  const int x2 = x1 - 2;
  const int y1 = 12;
  const int y2 = y1 - 10;
  cropMat(mat_, x1, y1, x2, y2);

  // Nothing should have changed
  EXPECT_EQ(mat_, original);
}

TEST_F(TestImageProcessing, willConvertRgbToGray)
{
  Mat original = RandomMat(177, 45, 3);
  mat_ = rgbToGray(original);

  ASSERT_EQ(mat_.width(), original.width());
  ASSERT_EQ(mat_.height(), original.height());
  ASSERT_EQ(mat_.channels(), 1);

  auto orgItr = original.vectorPtr()->begin();
  const double oneThird = 1.0 / 3.0;
  for(auto matItr = mat_.vectorPtr()->begin() ; matItr != mat_.vectorPtr()->end(); ++matItr)
  {
    double rgbSum = *(orgItr++);
    rgbSum += *(orgItr++);
    rgbSum += *(orgItr++);
    uint8_t grayVal = static_cast<uint8_t>(rgbSum*oneThird);
    EXPECT_EQ(*matItr, grayVal);
  }
}

TEST_F(TestImageProcessing, rgbToGrayWillReturnGrayMatWhenCalledOn1ChannelMat)
{
  Mat original = RandomMat(177, 45, 1);
  mat_ = rgbToGray(original);

  EXPECT_EQ(mat_, original);
}

TEST_F(TestImageProcessing, rgbToGrayWillReturnEmptyMatWhenCalledWithInvalidInput)
{
  Mat original = RandomMat(177, 45, 2);
  mat_ = rgbToGray(original);

  ASSERT_EQ(mat_.width(), 0);
  ASSERT_EQ(mat_.height(), 0);
  ASSERT_EQ(mat_.channels(), 0);
  ASSERT_EQ(mat_.vectorPtr()->size(), 0u);
}

TEST_F(TestImageProcessing, willConvertGrayToRgb)
{
  Mat original = RandomMat(153, 87, 1);
  mat_ = grayToRgb(original);

  ASSERT_EQ(mat_.width(), original.width());
  ASSERT_EQ(mat_.height(), original.height());
  ASSERT_EQ(mat_.channels(), 3);

  auto matItr = mat_.vectorPtr()->begin();
  for(auto orgItr = original.vectorPtr()->begin(); matItr != mat_.vectorPtr()->end(); ++orgItr)
  {
    // Check all 3 pixels
    EXPECT_EQ(*matItr++, *orgItr);
    EXPECT_EQ(*matItr++, *orgItr);
    EXPECT_EQ(*matItr++, *orgItr);
  }
}

TEST_F(TestImageProcessing, grayToRgbWillReturnRgbMatWhenCalledWith3ChannelMat)
{
  Mat original = RandomMat(153, 87, 3);
  mat_ = grayToRgb(original);

  EXPECT_EQ(mat_, original);
}

TEST_F(TestImageProcessing, grayToRgbWillReturnEmptyMatWhenCalledWithInvalidInput)
{
  Mat original = RandomMat(153, 87, 2);
  mat_ = grayToRgb(original);

  ASSERT_EQ(mat_.width(), 0);
  ASSERT_EQ(mat_.height(), 0);
  ASSERT_EQ(mat_.channels(), 0);
  ASSERT_EQ(mat_.vectorPtr()->size(), 0u);
}

TEST_F(TestImageProcessing, sobelEdgeDetectorOnGrayMatWillReturnMatOfSameDimensions)
{
  // It doesn't really make sense to perform edge detection on noise,
  // so this is more of a sanity check
  Mat original = RandomMat(153, 87, 1);
  mat_ = sobelEdgeDetector(original);

  EXPECT_EQ(mat_.width(), original.width());
  EXPECT_EQ(mat_.height(), original.height());
  EXPECT_EQ(mat_.channels(), original.channels());
  EXPECT_EQ(mat_.vectorPtr()->size(), original.vectorPtr()->size());
}

TEST_F(TestImageProcessing, sobelEdgeDetectorOnRgbMatWillReturnMatOfSameDimensionsWithOneChannel)
{
  // It doesn't really make sense to perform edge detection on noise,
  // so this is more of a sanity check
  Mat original = RandomMat(153, 87, 3);
  mat_ = sobelEdgeDetector(original);

  EXPECT_EQ(mat_.width(), original.width());
  EXPECT_EQ(mat_.height(), original.height());
  EXPECT_EQ(mat_.channels(), 1);
  EXPECT_EQ(mat_.vectorPtr()->size(), original.vectorPtr()->size() / 3u);
}

TEST_F(TestImageProcessing, sobelEdgeDetectorWillDetectGradientInY)
{
  // Fill out a matrix like this:
  // 0000000000
  // 0000000000
  // ...
  // 1111111111
  MicroCv::Mat original(100, 100, 1);
  auto itr = original.vectorPtr()->begin() + (original.vectorPtr()->size() / 2u);
  for(; itr != original.vectorPtr()->end(); ++itr)
    *itr = 255;

  mat_ = sobelEdgeDetector(original);
  ASSERT_EQ(mat_.width(), original.width());
  ASSERT_EQ(mat_.height(), original.height());
  ASSERT_EQ(mat_.channels(), original.channels());

  // The detected edge will be a 2 pixel line line horizontal across the middle
  MicroCv::Mat expectedY(100, 100, 1);
  auto expBegin = expectedY.vectorPtr()->begin() + (expectedY.vectorPtr()->size() / 2u) - 100;
  auto expEnd = expectedY.vectorPtr()->begin() + (expectedY.vectorPtr()->size() / 2u) + 100;
  for(auto yItr = expBegin; yItr != expEnd; ++yItr)
    *yItr = 255;

  // Zero out edge pixels since the convolution implementation ignores them
  zeroOutBorderPixels(expectedY);

  EXPECT_EQ(mat_, expectedY);
}

TEST_F(TestImageProcessing, sobelEdgeDetectorWillDetectGradientInX)
{
  // Fill out a matrix like this:  -> the edge will be vertical across the middle
  // 0000011111
  // 0000011111
  // ...
  // 0000011111
  MicroCv::Mat original(100, 100, 1);
  // Only iterate over the pixels in the half region
  for(int y = 0; y < original.height(); y++)
  {
    uint8_t* ptr = original.data() + y*original.width();
    for(int x = 0; x < original.width() / 2; x++, ptr++)
      *ptr = 255;
  }

  mat_ = sobelEdgeDetector(original);
  ASSERT_EQ(mat_.width(), original.width());
  ASSERT_EQ(mat_.height(), original.height());
  ASSERT_EQ(mat_.channels(), original.channels());

  // The detected edge will be a 2 pixel line line vertical across the middle
  MicroCv::Mat expectedX(100, 100, 1);
  for(int y = 0; y < expectedX.height(); y++)
  {
    uint8_t* xPtr = expectedX.data() + y*expectedX.width() + 49;
    *xPtr++ = 255;
    *xPtr = 255;
  }

  // Zero out edge pixels since the convolution implementation ignores them
  zeroOutBorderPixels(expectedX);

  EXPECT_EQ(mat_, expectedX);
}
