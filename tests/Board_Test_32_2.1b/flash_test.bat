cd /D "%~dp0"

%LocalAppData%\Arduino15\packages\esp32\tools\esptool_py\3.0.0\esptool.exe --chip esp32 --port COM3 --baud 115200 --before default_reset --after hard_reset write_flash -z --flash_mode dio --flash_freq 80m --flash_size detect 0xe000 %LocalAppData%\Arduino15\packages\esp32\hardware\esp32\1.0.5/tools/partitions/boot_app0.bin 0x1000 %LocalAppData%\Arduino15\packages\esp32\hardware\esp32\1.0.5/tools/sdk/bin/bootloader_dio_80m.bin 0x10000 ./Board_Test_32_2.1b.ino.bin 0x8000 ./Board_Test_32_2.1b.ino.partitions.bin

pause