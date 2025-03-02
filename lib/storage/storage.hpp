#pragma once
#include <FFat.h>
#include <USB.h>
#include <USBMSC.h>
#include <cstddef>
#include <esp_partition.h>
#include <optional>
#include <string>
#include <vector>

class Storage
{
  public:
    Storage(bool formatFsOnFail = false, const char *partitionLabel = FFAT_PARTITION_LABEL, ESPUSB &usb = USB,
            fs::F_Fat &fs = FFat, std::size_t blockSize = 512);
    bool isFileSystemReady() const;
    void setCallbackFsReady();
    void setCallbackFsBusy();
    std::size_t size() const;
    std::string getFileContents(const char *path) const;
    ~Storage();

    static constexpr std::string rootPath = "/";

  protected:
    fs::FS &getFileSystem();

  private:
    const esp_partition_t *partition;
    /**
     * Interface to USB mass storage class.
     */
    USBMSC usbMsc;
    const std::size_t blockSize;
    fs::F_Fat &fs;
    ESPUSB &usb;
    static std::vector<Storage *> callbackSubscribers;
    static esp_event_handler_t callbackUsbStarted_s;
    void callbackUsbStarted();
    static esp_event_handler_t callbackUsbStopped_s;
    void callbackUsbStopped();
};
