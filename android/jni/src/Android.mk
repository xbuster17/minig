LOCAL_PATH := $(call my-dir)
 include $(CLEAR_VARS)
 LOCAL_MODULE := main
 SDL_PATH := ../SDL2
 SDL_MIXER_PATH := ../SDL2_mixer
 LOCAL_C_INCLUDES := $(LOCAL_PATH)/$(SDL_PATH)/include $(LOCAL_PATH)/$(SDL_MIXER_PATH)/
 LOCAL_SRC_FILES := $(SDL_PATH)/src/main/android/SDL_android_main.c ../../.././src/texture.c ../../.././src/soundtest.c ../../.././src/gc_projectiles.c ../../.././src/game_grid_critters.c ../../.././src/gc_hud.c ../../.././src/gc_gridlings.c ../../.././src/entity.c ../../.././src/minig.c ../../.././src/tests.c ../../.././src/gc_sound.c ../../.././src/gc_grid.c ../../.././src/menu.c ../../.././src/main.c ../../.././src/lodepng.c ../../.././src/events.c ../../.././src/touch.c ../../.././src/sprite.c ../../.././src/joystick.c ../../.././src/gc_players.c ../../.././src/audio_test.c ../../.././src/shader.c
LOCAL_SHARED_LIBRARIES := SDL2 SDL2_mixer
 LOCAL_LDLIBS := -ldl -lGLESv2 -llog 
 LOCAL_CFLAGS := -Wall -O3 -Wextra 
 APP_CFLAGS += -O3
 include $(BUILD_SHARED_LIBRARY)
