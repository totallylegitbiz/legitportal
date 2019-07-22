.DEFAULT_GOAL := default
upload: 
	platformio run --target upload --upload-port ${PORT}
monitor: 
	platformio device monitor --port ${PORT}
upload-monitor: 
	make upload && make monitor
list:
	platformio device list
default:
	echo SDFASD