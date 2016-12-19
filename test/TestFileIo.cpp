/*
 *  Micro Computer Vision library
 *  @author Andrei Polzounov
 */
#include <iostream>

#include <boost/filesystem.hpp>

#include <gtest/gtest.h>

#include "FileIo.h"
#include "ImageProcessing.h"
#include "Mat.h"
#include "RandomMat.h"

using namespace MicroCv;

TEST(TestFileIo, willReadValidJpeg)
{
  bool ok;
  std::string filename = "../images/lena.jpg";
  Mat lena = MicroCv::readMatFromFile(filename, MicroCv::imageTypeFromFilename(filename), ok);

  ASSERT_TRUE(ok);
  EXPECT_EQ(lena.width(), 512);
  EXPECT_EQ(lena.height(), 512);
  EXPECT_EQ(lena.channels(), 3);
}

TEST(TestFileIo, willReadValidPng)
{
  bool ok;
  std::string filename = "../images/tux.png";
  Mat tux = MicroCv::readMatFromFile(filename, MicroCv::imageTypeFromFilename(filename), ok);

  ASSERT_TRUE(ok);
  EXPECT_EQ(tux.width(), 400);
  EXPECT_EQ(tux.height(), 479);
  EXPECT_EQ(tux.channels(), 3);
}

TEST(TestFileIo, willReadValidTiff)
{
  bool ok;
  std::string filename = "../images/square.tiff";
  Mat square = readMatFromFile(filename, MicroCv::imageTypeFromFilename(filename), ok);

  ASSERT_TRUE(ok);
  EXPECT_EQ(square.width(), 300);
  EXPECT_EQ(square.height(), 300);
  EXPECT_EQ(square.channels(), 3);
}

TEST(TestFileIo, willWriteJpeg)
{
  std::string filename = "../images/test.jpg";
  RandomMat randMat(47, 208, 3);
  bool writeOk = writeMatToFile(filename, randMat, ImageFileType::Jpeg);

  ASSERT_TRUE(writeOk);
  bool readOk;
  Mat readMat = readMatFromFile(filename, ImageFileType::Jpeg, readOk);
  ASSERT_TRUE(readOk);
  // JPEG is a lossy format so pixel values being the same are not guaranteed
  EXPECT_EQ(readMat.width(), randMat.width());
  EXPECT_EQ(readMat.height(), randMat.height());
  EXPECT_EQ(readMat.channels(), randMat.channels());

  boost::filesystem::remove(filename);
}

TEST(TestFileIo, willWritePng)
{
  std::string filename = "../images/test.png";
  RandomMat randMat(77, 161, 1);
  bool writeOk = writeMatToFile(filename, randMat, ImageFileType::Png);

  ASSERT_TRUE(writeOk);
  bool readOk;
  Mat readMat = readMatFromFile(filename, ImageFileType::Png, readOk);
  ASSERT_TRUE(readOk);
  // PNG is lossless, but MicroCv always reads in RGB images
  // so convert the original from grayscale before comparison
  Mat originalRgb = MicroCv::grayToRgb(randMat);
  EXPECT_EQ(readMat, originalRgb);

  boost::filesystem::remove(filename);
}

TEST(TestFileIo, willWriteTiff)
{
  std::string filename = "../images/test.tiff";
  RandomMat randMat(127, 88, 3);
  bool writeOk = writeMatToFile(filename, randMat, ImageFileType::Tiff);

  ASSERT_TRUE(writeOk);
  bool readOk;
  Mat readMat = readMatFromFile(filename, ImageFileType::Tiff, readOk);
  ASSERT_TRUE(readOk);
  // TIFF is also lossless
  EXPECT_EQ(readMat, randMat);

  boost::filesystem::remove(filename);
}

TEST(TestFileIo, willRecognizeFileExtensions)
{
  const std::vector<std::string> filenames = {
      "never.jpg", "gonna.jpeg", "give.jpe", "you.jif",
      "up.jfif", "never.jfi", "gonna.tif", "let.tiff",
      "you.png", "down.bmp", "and.gif", "desert.svg", "you.raw"};

  const std::vector<ImageFileType> types = {
      ImageFileType::Jpeg, ImageFileType::Jpeg, ImageFileType::Jpeg, ImageFileType::Jpeg,
      ImageFileType::Jpeg, ImageFileType::Jpeg, ImageFileType::Tiff, ImageFileType::Tiff,
      ImageFileType::Png, ImageFileType::Unsupported, ImageFileType::Unsupported,
      ImageFileType::Unsupported, ImageFileType::Unsupported};

  ASSERT_EQ(filenames.size(), types.size());
  auto tItr = types.begin();
  for(auto fItr = filenames.begin(); fItr != filenames.end(); ++fItr, ++tItr)
  {
    EXPECT_EQ(MicroCv::imageTypeFromFilename(*fItr), *tItr);
  }
}
