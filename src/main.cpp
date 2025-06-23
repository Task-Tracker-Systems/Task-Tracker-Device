/**
 * \file
 */

#include <FS.h>
#include <cassert>
#include <chrono>
#include <esp32-hal-log.h>
#include <esp_err.h>
#include <iostream>
#include <serial_interface/Protocol.hpp>
#include <serial_interface/serial_port.hpp>
#include <storage/storage.hpp>
#include <tasks/Task.hpp>
#include <thread>
#include <user_interaction/Menu.hpp>
#include <user_interaction/Presenter.hpp>
#include <user_interaction/ProcessHmiInputs.hpp>
#include <user_interaction/guiEngine_factory_interface.hpp>
#include <user_interaction/keypad_factory_interface.hpp>
#include <user_interaction/statusindicators_factory_interface.hpp>
#include <version.hpp>

#if ARDUINO_USB_CDC_ON_BOOT == 1
#define HWSerial Serial0
#else
#define HWSerial Serial
#endif

static const char *const TAG = "MAIN";

/**
 * Lists files and directories at path.
 */
static void listFiles(const char *const dirname, const std::shared_ptr<fs::FS> fs)
{
    std::cout << "Directory: '" << dirname << "'" << std::endl;
    File root = fs->open(dirname);
    if (!root || !root.isDirectory())
    {
        ESP_LOGE(TAG, "Error: '%s' is not a directory!\n", dirname);
        return;
    }

    File file = root.openNextFile();
    while (file)
    {
        std::cout << "\t" << file.name() << " (" << (file.isDirectory() ? "d" : "f") << ", " << file.size() << " Bytes)"
                  << std::endl;
        file.close();
        file = root.openNextFile();
    }
    file.close();
    root.close();
}

void setup()
{
    HWSerial.begin(115200);
    HWSerial.setDebugOutput(true);
    delay(3000); // in order to give the serial monitor time to start
    serial_port::initialize();
    serial_port::cout << "\x1b[20h"; // Tell the terminal to use CR/LF for newlines instead of just CR.
    serial_port::cout
        << std::endl
        << " begin program version '" << vcsId.value_or("unknown") << "'" << std::endl;
    serial_port::setCallbackForLineReception([](const serial_port::String &commandLine) {
        ProtocolHandler::execute(commandLine.c_str());
    });
    ESP_LOGE(TAG, "Example error");
    ESP_LOGW(TAG, "Example warning");
    ESP_LOGI(TAG, "Example info");
    ESP_LOGD(TAG, "Example debug");
    ESP_LOGV(TAG, "Example verbose");
    assert(Storage::begin());
}

void loop()
{
    static Menu singleMenu(board::getGuiEngine(), board::getKeypad());
    static Presenter presenter(singleMenu, board::getStatusIndicators());
    static ProcessHmiInputs processHmiInputs(presenter, board::getKeypad());

    serial_port::readAndHandleInput();

    std::this_thread::yield();
    using namespace std::chrono_literals;
    std::this_thread::sleep_for(100ms);

    presenter.loop();

    const auto fileSystemHandle = Storage::getFileSystem_locking(3s);
    if (fileSystemHandle)
    {
        listFiles("/", fileSystemHandle.value());
    }
    std::this_thread::sleep_for(3s);
}
