/*
 *  Image processing micro library
 *  @author Andrei Polzounov
 */
#include <iostream>

#include <boost/gil/extension/io/jpeg_io.hpp>
#include <boost/gil/extension/io/png_io.hpp>
#include <boost/gil/extension/io/tiff_io.hpp>
#include <boost/gil/gil_all.hpp>

int main(int /*argc*/, char** /*argv*/)
{
  // Test read jpeg
  boost::gil::rgb16_image_t image1;
  boost::gil::jpeg_read_and_convert_image("/home/vauser2/dev/instrumental-cv/images/lena.jpg", image1);
  std::cout << "lena: " << image1.width() << "x" << image1.height() << std::endl;

  boost::gil::rgb16_image_t image3;
  boost::gil::tiff_read_and_convert_image("/home/vauser2/dev/instrumental-cv/images/square.tiff", image3);
  auto dims3 = image3.dimensions();
  std::cout << "square: " << image3.width() << "x" << image3.height() << std::endl;

  boost::gil::rgb16_image_t image2;
  boost::gil::png_read_and_convert_image("/home/vauser2/dev/instrumental-cv/images/tux.png", image2);
  auto dims2 = image2.dimensions();
  std::cout << "tux: " << image2.width() << "x" << image2.height() << std::endl;

  return 0;
}
