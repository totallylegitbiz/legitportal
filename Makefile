.DEFAULT_GOAL := default
upload: 
	platformio run --target upload -e ${ENV} --upload-port ${PORT}
monitor: 
	platformio device monitor --port ${PORT}
upload-monitor: 
	make upload && make monitor
list:
	platformio device list
watch-sketch:
	onchange --await-write-finish 2000 sketches/TLCv1.fzz -- git commit -m "Auto Save" sketches/TLCv1.fzz
default:
	echo Sup.