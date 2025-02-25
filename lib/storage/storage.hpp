#pragma once
#include <FFat.h>
#include <USBMSC.h>
#include <cstddef>
#include <esp_partition.h>

class Storage
{
  public:
    void begin(const char *partitionLabel = FFAT_PARTITION_LABEL);
    void end();
    bool isFileSystemReady() const;
    void setCallbackFsReady();
    void setCallbackFsBusy();

  private:
    const esp_partition_t *partition;
    /**
     * Interface to USB mass storage class.
     */
    USBMSC usbMsc;
    const std::size_t blockSize;
};