#!/usr/bin/env python3
import sys
from intelhex import IntelHex

def merge_hex(bootloader_hex, application_hex, combined_hex):
    # Load the bootloader and application HEX files
    ih_boot = IntelHex(bootloader_hex)
    ih_app = IntelHex(application_hex)

    # Remove the start address from the application hex
    if ih_app.start_addr is not None:
        print("Ignoring application start address: ", ih_app.start_addr)
        ih_app.start_addr = None

    # Merge the application into the bootloader image.
    # Use overlap='ignore' (or 'replace' if you expect overlapping regions and want the application's data to win)
    ih_boot.merge(ih_app, overlap='error')

    # Write out the combined HEX file.
    ih_boot.write_hex_file(combined_hex)
    print("Merged HEX file written to", combined_hex)

if __name__ == "__main__":
    if len(sys.argv) != 4:
        print("Usage: merge_hex.py bootloader.hex application.hex combined.hex")
        sys.exit(1)
    merge_hex(sys.argv[1], sys.argv[2], sys.argv[3])
