# IOT Projects with ESP32
=======================

### Projects made with [Arduino IDE](https://www.arduino.cc/en/Main/Software) and [Espressif arduino-esp32](https://github.com/espressif/arduino-esp32) libraries.

Contents
--------

-	[Installation Instructions](#installation-instructions)
	-	[Arduino IDE](#arduino-ide-windows-10-installation)
	-	[Espressif arduino-esp32 libraries](#espressif-arduino-esp32-libraries)
	-	[Testing](#testing)
- [Projects](#projects)
	- [OTA WebUpdater 2.0](ota-webupdater-2.0)

Installation Instructions
-------------------------

### Arduino IDE (Windows 10 Installation)

-	Go to the [Arduino IDE official website](https://www.arduino.cc/en/Main/Software), in the "Download the Arduino IDE" section, choose the Windows installer option

![Arduino IDE installer download](docs/ArduinoIDE.png)

-	Launch the Installer, agree with the License Agreement

![Arduino IDE License Agreement](docs/ArduinoIDE-LicenseAgreement.png)

-	Leave all the installation options marked then click "Next"

![Arduino IDE Installation Options](docs/ArduinoIDE-InstallOptions.png)

-	Select an installation folder and click "Install"

![Arduino IDE Installation Folder](docs/ArduinoIDE-InstallationFolder.png)

-	Wait until the installation is completed then click "Close"

![Arduino IDE Installed](docs/ArduinoIDE-installing.png)

-	In the Windows 10 search bar, search for "Arduino", launch the .exe found then the Arduino IDE will launch

![Arduino IDE Launcher](docs/ArduinoIDE-Launching.png) ![Arduino IDE Default Sketch](docs/ArduinoIDE-DefaultSketch.png)

If the Arduino IDE doesn't launch properly, contact the Arduino IDE development team or community with [one of these methods](https://www.arduino.cc/en/Main/ContactUs).

### Espressif arduino-esp32 libraries

We're going to use Arduino IDE's Board Manager to install the Espressif arduino-32 libraries.

-	Open the Arduino IDE, go to File->Preferences , then paste this in "Board Manager Additional URL's": [https://dl.espressif.com/dl/package_esp32_index.json](https://dl.espressif.com/dl/package_esp32_index.json) ; then click "OK"

![Arduino IDE Preferences](docs/ArduinoIDE-Preferences.png)

-	Back at Arduino IDE, go to Tools->Board->Board Manager , in the search bar type "esp32", the first result should be "esp32 by Espressif Systems". Install this board package

![Arduino IDE Board Manager](docs/ArduinoIDE-BoardManager.png)

-	Now, you should be able to select an ESP32 board from the package in Tools->Board

![Arduino IDE ESP32 Boards](docs/ArduinoIDE-esp32boards.png)

Projects
--------

### OTA WebUpdater 2.0

The default WebUpdater avaliable at Espressif libraries for Arduino IDE is a bare bones tool that serves it's function of providing a starting point to development.<br>
In this project, I inserted some features that may provide a better experience developing OTA through a Network.
