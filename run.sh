#!/bin/bash

# Create disk.img if it doesn't exist (10 MiB)
if [ ! -f disk.img ]; then
    echo "disk.img not found, creating 10MiB disk..."
    dd if=/dev/zero of=disk.img bs=1M count=10 status=none
fi

# Run QEMU
qemu-system-i386 \
    -fda build/main_floppy.img \
    -drive file=disk.img,format=raw,if=ide \
    -net nic,model=rtl8139 \
    -net user \
    -audiodev pa,id=speaker \
    -machine pcspk-audiodev=speaker \
    -device sb16,audiodev=speaker \
    -serial stdio \
    -vga std
