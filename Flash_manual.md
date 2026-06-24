# Flash Manual

This manual describes how to flash a `.hex` file to the CJMCU ATmega32U4 board using the provided `flash.sh` script under Linux. The board uses the Caterina bootloader.

## Prerequisites

You need to have `avrdude` installed on your Linux system to flash the device. You can install it using your package manager. For example, on Debian/Ubuntu-based systems:

```bash
sudo apt update
sudo apt install avrdude
```

## Usage

1. **Make the script executable:**
   Before running the script for the first time, ensure it has executable permissions:

   ```bash
   chmod +x flash.sh
   ```

2. **Run the script:**
   Pass the path to your `.hex` file as an argument to the script:

   ```bash
   ./flash.sh <datei.hex>
   ```

3. **Enter Bootloader Mode:**
   The script will prompt you to put the board into bootloader mode.
   Briefly short the `RST` and `GND` pins (or press the reset button if available). The script will wait for the device to appear under `/dev/ttyACM*` and automatically begin flashing once detected.

## Troubleshooting Permissions

If you encounter permission denied errors when the script attempts to flash or access the port:

- **Use `sudo`:** You can run the script with root privileges:
  ```bash
  sudo ./flash.sh <datei.hex>
  ```
- **udev Rules:** Alternatively, you can configure your system's `udev` rules to grant your user access to `/dev/ttyACM*` devices, which allows flashing without `sudo`. You can typically do this by adding your user to the `dialout` group:
  ```bash
  sudo usermod -aG dialout $USER
  ```
  *(You may need to log out and log back in for group changes to take effect.)*
