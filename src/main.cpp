/*
 *  Image processing micro library
 *  @author Andrei Polzounov
 */
#include <cstdint>
#include <iostream>
#include <vector>

#include <boost/gil/extension/io/jpeg_io.hpp>
#include <boost/gil/extension/io/png_io.hpp>
#include <boost/gil/extension/io/tiff_io.hpp>
#include <boost/gil/gil_all.hpp>

#include <Matrix.h>

//struct PixelInserter
//{
//  PixelInserter(std::vector<uint16_t>* s): storage(s) {}
//  void operator() (boost::gil::rgb16_pixel_t p) const
//  {
//    storage->push_back(boost::gil::at_c<0>(p));
//    storage->push_back(boost::gil::at_c<1>(p));
//    storage->push_back(boost::gil::at_c<2>(p));
//  }
//  std::vector<uint16_t>* storage;
//};

struct PixelInserter
{
  PixelInserter(uint16_t* s): storagePtr(s) {}
  void operator() (boost::gil::rgb16_pixel_t p) const
  {
//    storage->push_back(boost::gil::at_c<2>(p));
//    storage->push_back(boost::gil::at_c<1>(p));
//    storage->push_back(boost::gil::at_c<0>(p));
    *storagePtr = boost::gil::at_c<0>(p);
    *(storagePtr + 1) = boost::gil::at_c<1>(p);
    *(storagePtr + 2) = boost::gil::at_c<2>(p);
  }
  uint16_t* storagePtr;
};

int main(int /*argc*/, char** /*argv*/)
{
  using namespace boost::gil;
  // Test read jpeg
  rgb16_image_t image1;
  jpeg_read_and_convert_image("/home/vauser2/dev/instrumental-cv/images/lena.jpg", image1);
  std::cout << "lena: " << image1.width() << "x" << image1.height() << std::endl;

  rgb16_image_t image3;
  tiff_read_and_convert_image("/home/vauser2/dev/instrumental-cv/images/square.tiff", image3);
  auto dims3 = image3.dimensions();
  std::cout << "square: " << image3.width() << "x" << image3.height() << std::endl;

  rgb16_image_t image2;
  png_read_and_convert_image("/home/vauser2/dev/instrumental-cv/images/tux.png", image2);
  auto dims2 = image2.dimensions();
  std::cout << "tux: " << image2.width() << "x" << image2.height() << std::endl;

  boost::gil::rgb16_view_t view = boost::gil::view(image2);
//  std::vector<uint16_t> st;
//  st.reserve(view.width() * view.height() * view.num_channels());
//  std::cout << "reserved: " << view.width() * view.height() * view.num_channels() << "\n";
//  boost::gil::for_each_pixel(view, PixelReverser(&st));
//  std::cout << "size: " << st.size() << "\n";

  MicroCv::Matrix mat;
  mat.resize(view.width(), view.height(), view.num_channels());
  uint16_t* dataPtr = mat.data();
  boost::gil::for_each_pixel(view, PixelInserter(dataPtr));

  // Test to grayscale
  mat.toGrayscale();
  auto view2 = nth_channel_view(view, 1);
  boost::gil::png_write_view("../images/gil.png", view2);

  // Test crop
  int xmin = 50;
  int ymin = 50;
  int xmax = 250;
  int ymax = 250;
  boost::gil::rgb16_view_t view3 = boost::gil::view(image1);
  auto view4 = subimage_view(view3, xmin, ymin, xmax, ymax);
  boost::gil::tiff_write_view("../images/gil2.tiff", view4);

  // Test writing back to file
//  gray16_view_t view2 = view()

//  boost::gil::rgb16s_view_t view = boost::gil::const_view(image2);
//
//  uint16_t data[view.width() * view.height() * view.num_channels()];
//  uint16_t* cursor = data;
//
//  boost::gil::rgb16_planar_ptr_t ptr;

  return 0;
}
