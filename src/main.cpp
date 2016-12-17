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

struct PixelInserter
{
    PixelInserter(std::vector<uint8_t>* s) : storage(s) {}
    void operator()(boost::gil::rgb8_pixel_t p) const {
        using boost::gil::at_c;
        storage->push_back(at_c<0>(p));
        storage->push_back(at_c<1>(p));
        storage->push_back(at_c<2>(p));
    }
    std::vector<uint8_t>* storage;
};

//struct PixelInserter
//{
//  PixelInserter(uint8_t* pixels): pixelPtr(pixels) {}
//  void operator() (boost::gil::rgb8_pixel_t& p) const
//  {
//    *pixelPtr = boost::gil::at_c<0>(p);
//    *(pixelPtr + 1) = boost::gil::at_c<1>(p);
//    *(pixelPtr + 2) = boost::gil::at_c<2>(p);
//  }
//  uint8_t* pixelPtr;
//};

// This could probably share code with the PixelInserter
struct PixelWriter
{
    PixelWriter(const uint8_t* pixels) : pixelPtr(pixels) {}

    void operator()(boost::gil::rgb8_pixel_t& p)
    {
        using boost::gil::at_c;
        at_c<0>(p) = *pixelPtr++;
        at_c<1>(p) = *pixelPtr++;
        at_c<2>(p) = *pixelPtr++;
    }

    void operator()(boost::gil::gray8_pixel_t& p)
    {
        using boost::gil::at_c;
        at_c<0>(p) = *pixelPtr++;
    }
    const uint8_t* pixelPtr;
};

int main(int /*argc*/, char** /*argv*/)
{
  using namespace boost::gil;
  // Test read jpeg
  rgb8_image_t lena;
  jpeg_read_and_convert_image("../images/lena.jpg", lena);
  auto lenaView = boost::gil::view(lena);
  std::cout << "lena: " << lena.width() << "x" << lena.height() << "x"<< lenaView.num_channels() << std::endl;

  rgb8_image_t square;
  tiff_read_and_convert_image("../images/square.tiff", square);
  auto squarenView = boost::gil::view(square);
  std::cout << "square: " << square.width() << "x" << square.height() << "x"<< squarenView.num_channels() << std::endl;

  rgb8_image_t tux;
  png_read_and_convert_image("../images/tux.png", tux);
  auto tuxView = boost::gil::view(tux);
  std::cout << "tux: " << tux.width() << "x" << tux.height() << "x"<< tuxView.num_channels() << std::endl;



  std::vector<uint8_t> st;
  st.reserve(lenaView.width() * lenaView.height() * lenaView.num_channels());
  boost::gil::for_each_pixel(lenaView, PixelInserter(&st));

  MicroCv::Matrix mat;
//  mat.resize(view.width(), view.height(), view.num_channels());
  mat.setData(st, lenaView.width(), lenaView.height(), lenaView.num_channels());

//  boost::gil::for_each_pixel(view, PixelInserter(dataPtr));

  // Test to grayscale
  mat.rgbToGray();

  gray8_image_t img(mat.width(), mat.height());
  gray8_view_t viewGray = boost::gil::view(img);
  uint8_t* dataPtr = mat.data();
  boost::gil::for_each_pixel(viewGray, PixelWriter(dataPtr));
  boost::gil::png_write_view("../images/gil-gray.png", viewGray);


  // From grayscale
  mat.grayToRgb();

  rgb8_image_t imgRgb(mat.width(), mat.height(), mat.channels());
  rgb8_view_t viewRgb = boost::gil::view(imgRgb);
  dataPtr = mat.data();
  boost::gil::for_each_pixel(viewRgb, PixelWriter(dataPtr));
  boost::gil::png_write_view("../images/gil-rgb.png", viewRgb);



  MicroCv::Matrix mat2;
  st.clear();
  st.reserve(tuxView.width() * tuxView.height() * tuxView.num_channels());
  boost::gil::for_each_pixel(tuxView, PixelInserter(&st));
  mat2.setData(st, tuxView.width(), tuxView.height(), tuxView.num_channels());

  // Test crop
  mat2.rgbToGray();
  mat2.crop(50, 43, 232, 387);


  gray8_image_t imgRgb2(mat2.width(), mat2.height(), mat2.channels());
  gray8_view_t viewRgb2 = boost::gil::view(imgRgb2);
//  rgb8_image_t imgRgb2(mat2.width(), mat2.height(), mat2.channels());
//  rgb8_view_t viewRgb2 = boost::gil::view(imgRgb2);
  uint8_t* dataPtr2 = mat2.data();
  boost::gil::for_each_pixel(viewRgb2, PixelWriter(dataPtr2));
  boost::gil::jpeg_write_view("../images/tux-crop.jpg", viewRgb2);

//  auto view2 = nth_channel_view(view, 1);
//  boost::gil::png_write_view("../images/gil.png", view2);

  // Test crop
//  int xmin = 50;
//  int ymin = 50;
//  int xmax = 250;
//  int ymax = 250;
//  boost::gil::rgb8_view_t view3 = boost::gil::view(image1);
//  auto view4 = subimage_view(view3, xmin, ymin, xmax, ymax);
//  boost::gil::tiff_write_view("../images/gil2.tiff", view4);

  // Test writing back to file
//  gray16_view_t view2 = view()

//  boost::gil::rgb16s_view_t view = boost::gil::const_view(image2);
//
//  uint8_t data[view.width() * view.height() * view.num_channels()];
//  uint8_t* cursor = data;
//
//  boost::gil::rgb8_planar_ptr_t ptr;

  return 0;
}
