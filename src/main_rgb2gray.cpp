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

void getCmdProgramOptions(int argc, char** argv, std::string& inFilename, std::string& outFilename)
{
  namespace po = boost::program_options;
  po::options_description description("Options");

  try
  {
    description.add_options()
        ("help", "Print help message and exit")
        ("in_file", po::value<std::string>()->required(), "Image input filename")
        ("out_file", po::value<std::string>()->required(), "Image output filename");

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
  }
  catch(po::error& e)
  {
    std::cerr << e.what() << std::endl << description << std::endl;
    exit(1);
  }
}

int main(int argc, char** argv)
{
  std::string inFilename, outFilename;
  getCmdProgramOptions(argc, argv, inFilename, outFilename);

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
  MicroCv::Mat mat = MicroCv::readMatFromFile(inFilename, inFileType, readOk);

  if(readOk)
  {
    mat = MicroCv::rgbToGray(mat);

    bool writeOk = MicroCv::writeMatToFile(outFilename, mat, MicroCv::imageTypeFromFilename(outFilename));
    if(writeOk)
    {
      std::cout << outFilename << " saved successfully." << std::endl;
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
