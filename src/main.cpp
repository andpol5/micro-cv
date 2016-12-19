/*
 *  Micro Computer Vision library
 *  @author Andrei Polzounov
 */
#include <cstdint>
#include <iostream>
#include <vector>

#include "FileIo.h"
#include "ImageProcessing.h"
#include "Mat.h"

//using namespace MicroCv;

int main(int /*argc*/, char** argv)
{
  std::string fileIn(argv[1]);
  std::string fileOut(argv[2]);

  bool ok;
  MicroCv::Mat inputMat  = MicroCv::readMatFromFile(fileIn, MicroCv::imageTypeFromFilename(fileIn), ok);

  if(ok)
  {
    MicroCv::Mat tmp = inputMat;
    MicroCv:: cropMat(tmp, 50, 50, tmp.width()-50, tmp.height()-50);
    tmp = MicroCv::rgbToGray(tmp);
    tmp = MicroCv::sobelEdgeDetector(tmp);

    MicroCv::writeMatToFile(fileOut, tmp, MicroCv::imageTypeFromFilename(fileOut));
  }

  return 0;
}
