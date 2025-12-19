import os

input_file = 'download.png'
output_file = 'src/logo_image.h'

with open(input_file, 'rb') as f:
    data = f.read()

with open(output_file, 'w') as f:
    f.write('#include <pgmspace.h>\n\n')
    f.write('const uint8_t gspro_logo[] PROGMEM = {\n')
    
    for i, byte in enumerate(data):
        if i % 16 == 0:
            f.write('  ')
        f.write(f'0x{byte:02X}, ')
        if (i + 1) % 16 == 0:
            f.write('\n')
            
    f.write('\n};\n\n')
    f.write(f'const uint32_t gspro_logo_len = {len(data)};\n')

print(f"Converted {input_file} to {output_file}")
