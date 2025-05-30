import sys
import os
import numpy as np
from barcode import Code39
from barcode.writer import ImageWriter
from PIL import Image

# Converts 24-bit RGB888 to 16-bit RGB565 (big endian)
def rgb888_to_rgb565(r, g, b):
    return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3)

# Generates a Code39 barcode and saves it as a PNG
def generate_code39_barcode(text, output_png):
    writer = ImageWriter()
    writer.set_options({
        'write_text': False,
        'quiet_zone': 1,
        'module_height': 20,
        'module_width': 0.4,
        'foreground': 'black',
        'background': 'white'
    })

    filename = Code39(text, writer=writer, add_checksum=False).save(output_png.replace('.png', ''))

    # Load, resize, and paste onto white background to avoid artifacts
    img = Image.open(filename).convert("RGB")
    try:
        resample_filter = Image.Resampling.LANCZOS
    except AttributeError:
        resample_filter = Image.LANCZOS

    img = img.resize((140, 60), resample_filter)
    white_bg = Image.new("RGB", img.size, "white")
    white_bg.paste(img)

    white_bg.save(output_png)
    return output_png

# Converts the PNG to raw RGB565 binary file (big endian)
def save_as_rgb565_raw(image_path, output_raw_file):
    img = Image.open(image_path).convert("RGB")
    pixels = np.array(img)
    height, width, _ = pixels.shape

    with open(output_raw_file, "wb") as f:
        for y in range(height):
            for x in range(width):
                r, g, b = pixels[y, x]
                rgb565 = rgb888_to_rgb565(r, g, b)
                f.write(int(rgb565).to_bytes(2, byteorder="little"))

def main():
    output_dir = "/home/barcode"
    output_raw = os.path.join(output_dir, "image.raw")

    if os.path.exists(output_raw):
        print(f"[!] Barcode already exists at {output_raw}. Skipping generation.")
        return

    if len(sys.argv) != 2:
        print("Usage: python3 barcode_generator.py <SERIAL_NUMBER>")
        sys.exit(1)

    serial_number = sys.argv[1]
    os.makedirs(output_dir, exist_ok=True)

    output_png = os.path.join(output_dir, "image.png")

    print(f"[+] Generating barcode for: {serial_number}")
    generate_code39_barcode(serial_number, output_png)

    print(f"[+] Converting to raw RGB565 format: {output_raw}")
    save_as_rgb565_raw(output_png, output_raw)

    print(f"[✓] Done! Raw image saved to: {output_raw}")

if __name__ == "__main__":
    main()
