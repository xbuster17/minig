#***Android***
DEVELOPER = xbuster17
PROJECT = minig
APackage = org.$(DEVELOPER).$(PROJECT)

APK = $(PROJECT)-debug.apk
APKDIR = bin/
AFolder = android

all: build pack uninstall install-debug

build:
	ndk-build

pack:
	ant debug

uninstall:
	adb uninstall $(APackage)

install-debug:
	adb install $(APKDIR)$(APK)

