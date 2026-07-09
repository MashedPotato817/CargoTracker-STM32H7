# Code Review Fix Log 1

> Date: 2026-07-09
> Scope: MQTT/Air780E, W25Q128 cache, PN532 frame parsing, SHT31/Power RTOS delay

## Goal

Scan the current code against `doc/code-review-20260709.md` and fix confirmed high-risk issues that can be repaired without CubeMX regeneration or changing public module APIs.

## Code Changes

Files changed:
- `test1/Core/Src/air780e/mqtt.c`
- `test1/Core/Src/flash/w25q128.c`
- `test1/Core/Src/nfc/pn532.c`
- `test1/Core/Src/sensor/sht31.c`
- `test1/Core/Src/app/power.c`

Fixes:
- `mqtt.c`
  - Added a CMSIS-RTOS mutex around USART1 MQTT access.
  - `MQTT_PollCommand()` now skips the current poll if MQTT publish/init owns the UART.
  - `TCP_SendBinary()` now returns failure when `SEND OK` is not observed.
  - MQTT CONNACK now checks return code `0x00` before setting `mqtt_ready`.
  - SUBSCRIBE packet is built from `MQTT_TOPIC_CMD` instead of hardcoded topic length bytes.
  - Cloud command matching now requires JSON `"cmd"` and quoted command values, reducing false positives in binary payloads.
- `w25q128.c`
  - Write Enable now verifies the WEL status bit.
  - Page Program rejects writes crossing a 256-byte page boundary.
  - Startup scans telemetry cache slots and writes only to erased slots.
  - Reboot no longer erases sector 0 before the first telemetry cache write.
  - Flush success clears only the record magic field; it no longer erases the whole sector and destroys other cached records.
- `pn532.c`
  - Ready polling delay now uses `osDelay()`.
  - Response parsing now validates LEN/LCS bounds, DCS checksum, and postamble before accepting payload.
- `sht31.c`
  - Sensor conversion wait now uses `osDelay()`.
- `power.c`
  - Air780E PWRKEY pulse and boot wait now use `osDelay()` instead of blocking the state-machine task with `HAL_Delay()`.

## Verification

Completed:

```powershell
gcc -fsyntax-only -std=c99 -DSTM32H7A3xxQ -DUSE_HAL_DRIVER ... \
  test1/Core/Src/air780e/mqtt.c \
  test1/Core/Src/app/power.c \
  test1/Core/Src/flash/w25q128.c \
  test1/Core/Src/nfc/pn532.c \
  test1/Core/Src/sensor/sht31.c
```

Result:
- All five changed source files passed syntax check.
- Only CMSIS pointer-width warnings appeared from the x86 MinGW check environment.

Static checks:
- User modules no longer contain the targeted `HAL_Delay()` calls.
- `TCP_SendBinary()` no longer has the old "assume sent" success path.
- `w25q128.c` no longer erases the whole sector inside `W25Q128_FlushCache()`.
- `FreeRTOSConfig.h` still has `configENABLE_FPU=1` and `configENABLE_MPU=0`.

Not fully verified:
- Full Keil build could not be run because the current workspace still lacks:
  - `test1/test1.ioc`
  - `test1/MDK-ARM/test1.uvprojx`

## Remaining Risk

- USART1 still uses polling reads; the stronger long-term fix is DMA circular RX or an ISR/stream-buffer based receiver.
- `printf()` output is still not line-atomic across tasks.
- W25Q128 cache reuse is conservative: if unsent records fill the sector, new cache writes fail instead of erasing old data.
- Hardware behavior still needs board validation after Keil project files are restored.
