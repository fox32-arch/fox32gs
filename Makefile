TARGET = linux
ifeq ($(TARGET),linux)
# default is used for CC
SDL2_CONFIG = sdl2-config
CFLAGS += -g -Ofast -std=c99 -Wall -Wextra `$(SDL2_CONFIG) --cflags`
LDFLAGS += `$(SDL2_CONFIG) --libs`
else
ifeq ($(TARGET),mingw)
CC = x86_64-w64-mingw32-gcc
CFLAGS += -g -Ofast -std=c99 -Wall -Wextra -DWINDOWS
LDFLAGS += -lmingw32 -lSDL2main -lSDL2
TARGET_FILE_EXTENSION = .exe
else
ifeq ($(TARGET),wasm)
CC = emcc
CFLAGS += -O3 -std=c99 -Wall -Wextra
LDFLAGS += -s TOTAL_MEMORY=70057984 -sALLOW_MEMORY_GROWTH=1 -sUSE_SDL=2 --preload-file fox32os.img
TARGET_EXTRADEPS = fox32os.img
TARGET_FILE_EXTENSION = .html
else
$(error unknown TARGET)
endif
endif
endif

CFILES = src/main.c \
		src/bus.c \
		src/buttons.c \
		src/cd-rom.c \
		src/cpu.c \
		src/framebuffer.c \
		src/memory-card.c \
		src/mmu.c \
		src/screen.c \
		src/serial.c

OBJS = $(addsuffix .o, $(basename $(CFILES)))

.PHONY: all
all: fox32gs$(TARGET_FILE_EXTENSION)

LIBGS_IN = libgs.rom
LIBGS_OUT = libgs.h

$(LIBGS_OUT): $(wildcard libgs/firmware/*.okm)
	cd libgs; make
	xxd -i $(LIBGS_IN) $(LIBGS_OUT)

fox32gs$(TARGET_FILE_EXTENSION): $(TARGET_EXTRADEPS) $(OBJS) $(LIBGS_OUT)
	$(CC) -o $@ $(OBJS) $(LDFLAGS)

%.o: %.c $(LIBGS_OUT)
	$(CC) -o $@ -c $< $(CFLAGS)

clean:
	rm -rf fox32gs fox32gs.exe fox32gs.wasm fox32gs.html fox32gs.data fox32gs.js libgs.h $(OBJS)
