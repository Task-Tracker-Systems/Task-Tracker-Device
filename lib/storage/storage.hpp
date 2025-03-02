#pragma once
#include <FFat.h>
#include <FS.h>
#include <USBMSC.h>
#include <cstddef>
#include <esp_event_base.h>
#include <string>

class Storage
{
  public:
    static bool begin(bool formatFsOnFail = false, const char *partitionLabel = FFAT_PARTITION_LABEL);
    static void end();
    static bool isFileSystemReady();
    static std::size_t size();
    static std::string getFileContent(const char *path);

    /**
     * Switch from application mode (file system) to USB MSC.
     */
    static void switchToUsbMode();
    /**
     * Switch from USB MSC to application mode (file system).
     */
    static void switchToApplicationMode();

  protected:
    static fs::FS &getFileSystem();
};
