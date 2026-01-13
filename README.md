# OER.Room.Monsters.AudioPlayer

ESP32-S3 audio player controller for the Monsters escape room.

## Purpose

Controls a DY-HV20T audio module via ESP-NOW commands from the AudioController. Receives play, stop, and volume commands and responds with ACK for connection monitoring.

## Hardware

**Target Board:** ESP32-S3-DevKitC-1 (N8R2 variant)

### Wiring

| GPIO | Function | Notes |
|------|----------|-------|
| 16 | DY-HV20T RX | ESP32 RX <- DY TX (IO0) |
| 17 | DY-HV20T TX | ESP32 TX -> DY RX (IO1) |
| 48 | Heartbeat LED | Onboard RGB (status indicator) |

### DY-HV20T Wiring

```
DY-HV20T Pin   ESP32-S3 Pin    Notes
-----------    ------------    -----
VIN            5V              Power supply (6-35VDC)
GND            GND             Common ground
IO0 (TX)       GPIO16 (RX2)    Serial data from player
IO1 (RX)       GPIO17 (TX2)    Serial data to player
SPK+           Speaker +       4-8 ohm speaker (up to 20W)
SPK-           Speaker -       4-8 ohm speaker

CON1           GND             \
CON2           GND              } One-line serial mode
CON3           3.3V            /
```

Note: ESP32 is 3.3V logic, DY-HV20T is 5V tolerant on IO pins. No level shifter needed.

## Features

- **DY-HV20T control**: Play, stop, volume adjustment
- **Persistent volume**: Saved to flash, restored on startup
- **ESP-NOW receiver**: Accepts commands from AudioController
- **ACK responses**: Confirms command receipt for connection monitoring
- **Heartbeat LED**: Shows device is running normally

## ESP-NOW Commands

Supported commands (case-insensitive):

| Command | Action |
|---------|--------|
| `PING` | Responds with ACK (connection check) |
| `PLAY` / `START` | Play/resume audio |
| `STOP` | Stop playback |
| `RESTART` | Restart track from beginning |
| `VOL_UP` / `VOLUP` / `V+` | Increase volume |
| `VOL_DOWN` / `VOLDOWN` / `V-` | Decrease volume |
| `RESET` | Reset to default state |

All commands respond with `ACK`.

## Audio Settings

Configured in `config.h`:

| Setting | Default | Description |
|---------|---------|-------------|
| `DYPLAYER_VOLUME` | 18 | Default volume (0-30), used on first boot |
| `DYPLAYER_VOLUME_STEP` | 6 | Volume change per button press |
| `DYPLAYER_TRACK` | 1 | Track number to play (00001.mp3) |

### Volume Persistence

Volume is saved to flash memory and restored on startup. The volume step of 6 gives these levels:

```
0 → 6 → 12 → 18 → 24 → 30
```

5 clicks from mute to maximum.

## Modules Enabled

- ESP-NOW (Client mode - listens to host)
- Heartbeat (RGB LED status indicator)

## Build Commands

```bash
pio run                  # Build
pio run --target upload  # Upload to board
pio run --target clean   # Clean build
pio device monitor       # Serial monitor (115200 baud)
```

## Related Projects

- [OER.Room.Monsters.AudioController](https://github.com/TenMoreMinutesProductions/OER.Room.Monsters.AudioController) - Button interface for remote control
