ARDUINO_DIR = /Applications/Arduino.app/Contents/Resources/Java
ARDMK_DIR = ~/dev/Arduino-Makefile/
ARDUINO_LIBS = Wire

MONITOR_PORT = /dev/tty.usbserial*
BOARD_TAG = atmega168

include $(ARDMK_DIR)/arduino-mk/Arduino.mk
