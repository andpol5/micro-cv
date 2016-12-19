/*
 *  Micro Computer Vision library
 *  @author Andrei Polzounov
 */
#include <iostream>
#include <random>

#include <gtest/gtest.h>

#include "Mat.h"
#include "RandomMat.h"

using namespace MicroCv;

namespace
{
  class TestMat : public ::testing::Test
  {
  public:
    TestMat() {}

  protected:
    Mat mat_;
  };
}

TEST_F(TestMat, willInitEmptyMat)
{
  EXPECT_EQ(mat_.width(), 0);
  EXPECT_EQ(mat_.height(), 0);
  EXPECT_EQ(mat_.channels(), 0);
  EXPECT_EQ(mat_.vectorPtr()->size(), 0u);
}

TEST_F(TestMat, willResizeMat)
{
  mat_.resize(17, 4, 24);

  EXPECT_EQ(mat_.width(), 17);
  EXPECT_EQ(mat_.height(), 4);
  EXPECT_EQ(mat_.channels(), 24);
  EXPECT_EQ(mat_.vectorPtr()->size(), static_cast<size_t>(17*4*24));
}

TEST_F(TestMat, willReturnIsGrayScaleForOneChannel)
{
  EXPECT_FALSE(mat_.isGrayscale());

  mat_.resize(17, 4, 3);
  EXPECT_FALSE(mat_.isGrayscale());

  mat_.resize(17, 4, 1);
  EXPECT_TRUE(mat_.isGrayscale());
}

TEST_F(TestMat, willCorrectlyUseCopyConstructorAndAssignmentOperator)
{
  Mat other = RandomMat(107, 163, 3);
  mat_ = Mat(other);

  EXPECT_EQ(mat_.width(), other.width());
  EXPECT_EQ(mat_.height(), other.height());
  EXPECT_EQ(mat_.channels(), other.channels());
  // Fail out of this assertion so the bellow loop doesn't go out of memory bounds
  ASSERT_EQ(mat_.vectorPtr()->size(), other.vectorPtr()->size());

  auto itr = mat_.vectorPtr()->begin();
  auto otherItr = other.vectorPtr()->begin();
  for( ; itr != mat_.vectorPtr()->end(); ++itr, ++otherItr)
  {
    EXPECT_EQ(*itr, *otherItr);
  }

  EXPECT_EQ(mat_, other);
}
