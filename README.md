# Kasa (harbour-kasa)

Kasa is a native Sailfish OS application to control TP-Link Kasa smarthome devices.

>![](icons/128x128/harbour-kasa.png)

### Supported Devices:

- HS100
- HS110
- KL110 (thanks to 24mu13 for his help)

### Supported Features

- Add device by IP/Hostname
- Turn on/off
- Turn LED on/off
- Show device info and energy consumption (HS110 only)
- Restart the device
- Change cloud url to prevent the device from calling home
- Change MAC address
- Change device alias
- Cloud registration active / inactive
- Energy consumption chart for actual month / year (HS110 only) (WIP)

### Planned Features

- Autodetect of devices in the network
- Charts for energy consumption (use device internal data / HS110 only)
- Reset device
- Setup device (factory new /reset)
- Schedule support
- Change WiFi settings

## Development Support

If you want to support the development of this app, especially if you have an unsupported TP-Link device, please enable debugging in the app settings.  
Please send me the debug log data in "Documents/harbour-kasa" folder on your device.

For testing unkown Devices I need you help. I will provide you some commands wich you can test inside the app, so please contact me for this. 
This option you find in the context menu of the device in device list page.
You need to add the device via IP address first.
Before sending the command in the app, it is a good idea to delete old debugging logs first.
After sending the commands, send me the files in "Documents/harbour-kasa".
  
## Translations

- English
- German
- Polish by [Atlochowski](https://github.com/atlochowski)
- Russian by [potap14108](https://github.com/potap14108)
  
Your language is not available? You are welcome to support this project by translating it on my self hosted Weblate server:

[![https://weblate.nubecula.org/engage/harbour-kasa/](https://weblate.nubecula.org/widgets/harbour-kasa/-/multi-auto.svg)](https://weblate.nubecula.org/engage/harbour-kasa/)
  
## Special Thanks

- To the guys from https://www.softscheck.com for the reverse engineering of the device
- To milosolutions for the QML Chart.js wrapper
- To the guys behind Chart.js

## Donations

If you like my work, you can buy me a beer! 

[![PayPal](https://www.paypalobjects.com/en_US/i/btn/btn_donate_LG.gif) ](https://www.paypal.com/paypalme/nubecula/1)

[![Liberapay](https://liberapay.com/assets/widgets/donate.svg)](https://liberapay.com/black-sheep-dev/donate)