#pragma once
#include <cstddef>
#include <memory>

namespace fs
{
class FS;
}

/**
 * @brief Provides internal non-volatile as filesystem and USB mass storage device.
 * 
 * This adapter has two modes:
 * Either filesystem for internal use or USB mass storage device.
 * 
 * Initializes a filesystem for internal use.
 * Initializes a USB mass storage class service.
 * 
 * Access to the filesystem is provided as smart pointer.
 * Until the deleter of the smart pointer is (implicitly)
 * called, the storage is locked for filesystem use.
 * 
 * When the USB Mass Storage Class (USB MSC) is started
 * (typically by connecting a host device) the storage will
 * present the memory as mass storage device to the host device#
 * as soon as the storage is not locked for filesystem use.
 */
struct Storage
{
    /**
     * @brief Initializes storage
     * 
     * Initializes filesystem and USB MSC service.
     * 
     * @post storage may be used afterwards
     * @warning do not call again unless end() was called after before
     */
    static void begin();

    /**
     * @brief Disengages filesystem and USB
     */
    static void end();

    /**
     * @brief Get locking access to filesystem.
     * 
     * @pre storage must be initialized first
     * @return smart pointer, locking storage to filesystem until deleted
     */
    static std::shared_ptr<fs::FS> getFileSystem_locking();

    /**
     * @brief Gets the total size of memory.
     * 
     * @pre storage must be initialized first
     * @return size in bytes
     */
    static std::size_t size();
};
