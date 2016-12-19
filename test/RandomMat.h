/*
 *  Micro Computer Vision library
 *  @author Andrei Polzounov
 */
#include <iostream>
#include <random>

#include "Mat.h"

namespace MicroCv
{
/*
 * Helper used for testing - it fills a Mat with noisy pixels
 */
class RandomMat : public Mat
{
public:
  RandomMat(int width, int height, int channels)
  {
    reserve(width, height, channels);
    const int numPixels = width * height * channels;
    for(int pixel = 0; pixel < numPixels; pixel++)
    {
      vectorPtr()->push_back(randomPixel());
    }
  }

private:
  uint8_t randomPixel()
  {
    std::mt19937 rng;
    rng.seed(std::random_device()());
    std::uniform_int_distribution<std::mt19937::result_type> dist(0, 255);
    return static_cast<uint8_t>(dist(rng));
  }
};

};
