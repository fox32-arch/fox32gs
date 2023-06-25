OKAMERON := meta/okameron/okameron.lua
ASMFX := meta/xloff-sdk/asmfx/asmfx.lua
XOFTOOL := meta/xloff-sdk/xoftool/xoftool.lua

all: ../libgs.rom

FIRMWARE_INPUT_FILES = \
	firmware/Main.okm

../libgs.rom: $(FIRMWARE_INPUT_FILES)
	lua $(OKAMERON) -arch=fox32 -xrsdk -startup=firmware/start.asm $(FIRMWARE_INPUT_FILES) > firmware/firmware.asm
	luajit $(ASMFX) target=fox32 firmware/firmware.asm firmware/firmware.o
	luajit $(XOFTOOL) move firmware/firmware.o text=0xF0000000,rodata=text+text_size,bss=0x00000800
	luajit $(XOFTOOL) binary -nobss firmware/firmware.o
	mv firmware/firmware.o $@
	rm firmware/firmware.asm

clean:
	rm -f ../libgs.rom
