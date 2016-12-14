/*
 *  Image processing micro library
 *  @author Andrei Polzounov
 */
#include <chrono>
#include <iostream>

#include <boost/asio.hpp>
#include <boost/thread.hpp>

#include "../include/CommsNodeMulticastSender.h"
#include "CommsNodeAsyncTcpServer.h"
#include "CommsNodeDelayedTcpServer.h"

using boost::asio::ip;
using boost::asio::ip::tcp;

void serverThread(const ip::address& multicastAddress)
{
  try
  {
    boost::asio::io_service ioService;
    CommsNodeAsyncTcpServer server(ioService, multicastAddress);
    ioService.run();
  }
  catch (std::exception& e)
  {
    std::cerr << e.what() << std::endl;
  }
}

void clientThread(const ip::address& multicastAddress)
{
  try
  {
    boost::asio::io_service ioService;
    CommsNodeMulticastSender multiCastSender(ioService, multicastAddress);
    ioService.run();
  }
  catch (std::exception& e)
  {
    std::cerr << e.what() << std::endl;
  }
}

int main(int argc, char* argv[])
{
  if (argc != 2)
  {
    std::cerr << "Usage: " << argv[0] << " <multicast_address>" << std::endl;
    std::cerr << "  For IPv4, try:" << std::endl;
    std::cerr << "    " << argv[0] << " 239.255.0.1" << std::endl;
    std::cerr << "  For IPv6, try:" << std::endl;
    std::cerr << "    " << argv[0] << " ff31::8000:1234" << std::endl;
    return 1;
  }

  boost::system::error_code ipError;
  ip::address multicastAddress = ip::address::from_string(argv[1], ipError);
  if (ipError)
  {
    std::cerr << ipError.message() << std::endl;
    return 1;
  }

  // Using boost::thread for Windows compatibility as the MinGW compiler
  // does not yet support std::thread
  boost::thread sThread(&serverThread, multicastAddress);
  boost::thread cThread(&clientThread, multicastAddress);

  sThread.join();
  cThread.join();
  return 0;
}
