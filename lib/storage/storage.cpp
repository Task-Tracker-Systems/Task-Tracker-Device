#include "storage.hpp"
#include <iostream>

const esp_partition_t *check_ffat_partition(const char *label); // defined in FFat.cpp

Storage::Storage(const bool formatFsOnFail, const char *const partitionLabel, ESPUSB &usb, fs::F_Fat &fs, const std::size_t blockSize):
partition(check_ffat_partition(partitionLabel)),
blockSize(blockSize),
fs(fs),
usb(usb)
{
  if(!partition){
    std::cerr << "Error with partition!" << std::endl;
    return;
  }

  // initialize file system
  const auto basePath = rootPath + partitionLabel;
  constexpr auto maxOpenFiles = 10;
  if(!fs.begin(formatFsOnFail, basePath.c_str(), maxOpenFiles, partitionLabel)){
    std::cerr << "File-system initialization failed!" << std::endl;
    return;
  }
  std::cout << "Storage has a size of " << size() << " bytes." << std::endl;

  // subscribe to USB events
  callbackSubscribers.push_back(this);
  usb.onEvent(ARDUINO_USB_STARTED_EVENT, callbackUsbStarted_s);
  usb.onEvent(ARDUINO_USB_STOPPED_EVENT, callbackUsbStopped_s);
}

std::size_t Storage::size() const
{
  return fs.totalBytes();
}
