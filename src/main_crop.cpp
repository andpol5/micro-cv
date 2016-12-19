/*
 *  Micro Computer Vision library
 *  @author Andrei Polzounov
 */
#include <iostream>

#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>

#include "FileIo.h"
#include "ImageProcessing.h"
#include "Mat.h"

void getCmdProgramOptions(int argc, char** argv,
    std::string& inFilename, std::string& outFilename, int& x1, int& y1, int& x2, int& y2)
{
  namespace po = boost::program_options;
  po::options_description description("Options");

  try
  {
    description.add_options()
        ("help", "Print help message and exit")
        ("in_file", po::value<std::string>()->required(), "Image input filename")
        ("out_file", po::value<std::string>()->required(), "Image output filename")
        ("x1", po::value<int>()->default_value(0),
            "X coordinate from which to crop")
        ("y1", po::value<int>()->default_value(0),
            "Y coordinate from which to crop")
        // Going to assume that -1 here is the end pixel (like in Python)
        ("x2", po::value<int>()->default_value(-1),
            "X coordinate to crop to (-1 crops to last pixel)")
        ("y2", po::value<int>()->default_value(-1),
            "Y coordinate to crop to (-1 crops to last pixel)");

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, description), vm);

    if(vm.count("help"))
    {
      std::cout << description << std::endl;
      exit(1);
    }
    po::notify(vm);
    inFilename = vm["in_file"].as<std::string>();
    outFilename = vm["out_file"].as<std::string>();
    x1 = vm["x1"].as<int>();
    y1 = vm["y1"].as<int>();
    x2 = vm["x2"].as<int>();
    y2 = vm["y2"].as<int>();
  }
  catch(po::error& e)
  {
    std::cerr << e.what() << std::endl << description << std::endl;
    exit(1);
  }
}

inline void clampIntToRange(int& val, int min, int max)
{
  if(val < min)
  {
    val = min;
  }
  else if(val > max)
  {
    val = max;
  }
}

int main(int argc, char** argv)
{
  std::string inFilename, outFilename;
  int x1, x2, y1, y2;
  getCmdProgramOptions(argc, argv, inFilename, outFilename, x1, y1, x2, y2);

  // Check if file exists
  if(!boost::filesystem::exists(inFilename))
  {
    std::cerr << "Input file not found: " << inFilename << std::endl;
    return 1;
  }

  // Check file types
  MicroCv::ImageFileType inFileType = MicroCv::imageTypeFromFilename(inFilename);
  MicroCv::ImageFileType outFileType = MicroCv::imageTypeFromFilename(inFilename);
  if(inFileType == MicroCv::Unsupported)
  {
    std::cerr << "File type: " << boost::filesystem::extension(inFilename) << " is not supported" << std::endl;
    return 1;
  }
  if(outFileType == MicroCv::Unsupported)
  {
    std::cerr << "File type: " << boost::filesystem::extension(outFilename) << " is not supported" << std::endl;
    return 1;
  }

  bool readOk;
  MicroCv::Mat inputMat = MicroCv::readMatFromFile(inFilename, inFileType, readOk);

  if(readOk)
  {
    // Fix invalid crop input values
    if(x2 == -1)
      x2 = inputMat.width()-1;
    if(y2 == -1)
      y2 = inputMat.height()-1;
    clampIntToRange(x1, 0, inputMat.width()-1);
    clampIntToRange(y1, 0, inputMat.height()-1);
    clampIntToRange(x2, 0, inputMat.width()-1);
    clampIntToRange(y2, 0, inputMat.height()-1);

    std::cout << "Crop points - top-left: (" << x1 << ", " << y1 << ") bottom-right: ("
        << x2 << ", " << y2 << ")" << std::endl;

    MicroCv::Mat croppedMat = inputMat;
    MicroCv::cropMat(croppedMat, x1, y1, x2, y2);

    bool writeOk = MicroCv::writeMatToFile(outFilename, croppedMat, MicroCv::imageTypeFromFilename(outFilename));
    if(writeOk)
    {
      std::cout << outFilename << " successfully cropped!" << std::endl;
    }
    else
    {
      std::cerr << "Could not write output file: " << outFilename << std::endl;
      return 1;
    }
  }
  else
  {
    std::cerr << "Could not read input file: " << inFilename << std::endl;
    return 1;
  }
  return 0;
}
