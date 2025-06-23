# Overview of Build Options for ESP32-S3

See also [documentation on USB options](https://docs.espressif.com/projects/arduino-esp32/en/latest/guides/tools_menu.html#usb-options).

## 1. USB Mode (`ARDUINO_USB_MODE`)

| Value | Label |
|------|----------------------------|
| `0`  | USB-OTG (TinyUSB) (Default) |
| `1`  | Hardware CDC and JTAG |

**Default value:** `0`  
**Macro:** `-DARDUINO_USB_MODE={build.usb_mode}`

## 2. USB CDC On Boot (`ARDUINO_USB_CDC_ON_BOOT`)

| Value | Label |
|------|------------|
| `0`  | Disabled (Default) |
| `1`  | Enabled |

**Default value:** `0`  
**Macro:** `-DARDUINO_USB_CDC_ON_BOOT={build.cdc_on_boot}`

## 3. USB Firmware MSC On Boot (`ARDUINO_USB_MSC_ON_BOOT`)

| Value | Label |
|------|--------------------------------|
| `0`  | Disabled (Default) |
| `1`  | Enabled (Requires USB-OTG Mode) |

**Default value:** `0`  
**Macro:** `-DARDUINO_USB_MSC_ON_BOOT={build.msc_on_boot}`

## 4. USB DFU On Boot (`ARDUINO_USB_DFU_ON_BOOT`)

| Value | Label |
|------|--------------------------------|
| `0`  | Disabled (Default) |
| `1`  | Enabled (Requires USB-OTG Mode) |

**Default value:** `0`  
**Macro:** `-DARDUINO_USB_DFU_ON_BOOT={build.dfu_on_boot}`
