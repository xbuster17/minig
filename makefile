CC = gcc
EXEC = minig
CFLAG = -O3
LFLAG = $(CFLAG)
LIBS = -lm -lSDL2 -lSDL2_mixer -lGLEW -lGL
#sources-----------------------------------------------------------------------#
MAIN = ./src/main.c

MINIG = $(wildcard ./src/*.c)
SRCS = $(MINIG)
OBJS = $(subst .c,.o,$(subst $(MAIN), ,$(SRCS)))
CLEAN = $(subst $(NOCLEAN), ,$(OBJS))
NOCLEAN = ./src/lodepng.o

#rules-------------------------------------------------------------------------#
all: alert $(SRCS) $(EXEC)
	$(DONETAG)
#------------------------------------------------------------------------------#
$(EXEC): $(MAIN) $(OBJS)
	$(Ltag) $(EXEC) $(Rtag)
	@$(CC) $(LFLAG) $(MAIN) $(OBJS) $(LIBS) -o $(EXEC)
#------------------------------------------------------------------------------#
%.o: %.c %.h
	$(Ctag) $< $(Rtag)
	@$(CC) -c $(CFLAG) $< $(LIBS) -std=c11 -o $@
%.opp: %.cpp %.hpp
	$(C++tag) $< $(Rtag)
	@$(CC) -c $(CFLAG) $< $(LIBS) -std=c++11 -lstdc++ -o $@

#------------------------------------------------------------------------------#
rm: alert clean
	$(DONETAG)
clean:
	$(REDTAG)"    rm \e[36m$(CLEAN)"$(Rtag)
	@rm $(CLEAN)
#------------------------------------------------------------------------------#
bak: alert backup
	$(DONETAG)
backup:
	$(REDTAG)"    creating backup of src files into: "$(GREENTAG)$(SRCDIR)"/"$(BAKDIR)$(Rtag)
	@ mkdir -p $(SRCDIR)/.bak
	@ cd $(SRCDIR) && mkdir $(BAKDIR) && cp -r * $(BAKDIR)
SRCDIR = ./src
BAKDIR := .bak/`date -u +%F_%H-%M-%S`

#------------------------------------------------------------------------------#
droid:
	$(ALERTTAG)
	@ln -f -s -r ./assets ./android/
	@printf $(mk_d_p) > $(mk_d_path)
	@echo $(mk_d_src) >> $(mk_d_path)
	@printf $(mk_d_s) >> $(mk_d_path)
	@touch -d "1970-01-01 00:00:00" $(mk_d_path)
	@cd android && make
	$(DONETAG)

#droid makefile:--------------------------------------------------------------#
mk_d_path = ./android/jni/src/Android.mk

mk_d_src = $(addprefix ../../../,$(SRCS))

define mk_d_p #droid's makefile prefix
'LOCAL_PATH := $$(call my-dir)\n\
include $$(CLEAR_VARS)\n\
LOCAL_MODULE := main\n\
SDL_PATH := ../SDL2\n\
SDL_MIXER_PATH := ../SDL2_mixer\n\
LOCAL_C_INCLUDES := $$(LOCAL_PATH)/$$(SDL_PATH)/include \
$$(LOCAL_PATH)/$$(SDL_MIXER_PATH)/\n\
LOCAL_SRC_FILES := $$(SDL_PATH)/src/main/android/SDL_android_main.c '
endef

define mk_d_s #droid's makefile suffix
'LOCAL_SHARED_LIBRARIES := SDL2 SDL2_mixer\n\
LOCAL_LDLIBS := -ldl -lGLESv2 -llog \n\
LOCAL_CFLAGS := -Wall -O3 -Wextra \n\
APP_CFLAGS += -O3\n\
include $$(BUILD_SHARED_LIBRARY)\n'
endef

#eye-candy---------------------------------------------------------------------#
alert:
	$(ALERTTAG)

C++tag= @echo -e "\e[0;36m    COMPILE:"
Ctag= @echo -e "\e[0;32m    COMPILE:"
Ltag= @echo -e "\e[33m        LINK:"
REDTAG= @echo -e "\e[91m"
GREENTAG= "\e[92m"
PURPLETAG= "\e[95m"
ALERTTAG= @echo -e "\e[0;95mMAKE: \t~~\t$(EXEC)"$(Rtag)
DONETAG= @echo -e "\e[0;95m~~\t~~\tDONE!"$(Rtag)
COLORTAG= @echo -e "\e[91m"
Rtag= "\e[0m"

## TODO :
# fix NOCLEAN to accept multiple files to ignore removing
