# bt_esp32_mouse_ble
Make esp32 work as bluetooth mouse.

Tested on esp-idf v4.3

# Disclaimer
This is not a product level code that ignores some Bluetooth security features intentionally.

For PnP ID, this component uses a famous private prototyping value as the USB vendor/product ID. You may have to undefine the service (yes, it works for most cases) or register your own ID before reusing this code.

# License 
Licensed under Apache License 2.0

# Build
Setup esp-idf

Configure sdkconfig, enable Bluetooth, enable NimBLE stack.

Run "idf.py build"


# References
USB Vendor ID 
https://f055.io/

GATT service types - 16-bit UUIDs
and Appearance Values
https://www.bluetooth.com/specifications/assigned-numbers/ 

HID over GATT (HOGP) spec
https://www.bluetooth.com/specifications/specs/hid-over-gatt-profile-1-0/
HID service (HIDS) spec 
https://www.bluetooth.com/specifications/specs/hid-service-1-0/
Battery service (BAS) spec
https://www.bluetooth.com/specifications/specs/battery-service-1-0/
Device Information service (DIS) spec
https://www.bluetooth.com/specifications/specs/device-information-service-1-1/

HID Descriptor for Win8
https://docs.microsoft.com/en-us/windows-hardware/design/component-guidelines/supporting-usages-in-digitizer-report-descriptors