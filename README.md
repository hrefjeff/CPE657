# CPE657

##  Team members

George, Amber, Harsimran, Jeffrey

## Documentation Links

- [QEMU Documentation](https://wiki.qemu.org/Hosts/Linux)
- [Bodhi Linux Download](https://drive.google.com/file/d/11w1kItSrVW-saXxTpnfTrw7358UcNS5t/view)

## Frequently Used Commands

### Creating an image in QEMU

```bash
./qemu-img create -f qcow2 <NAME OF IMG>.qcow2 16G
```

### Running archlinux in QEMU

```bash
  qemu-system-x86_64         \
  -m 2G                      \
  -smp 1                     \
  -hda <NAME OF IMG>.qcow2   \
  -boot d                    \
  -cdrom ~/archlinux-2023.08.01-x86_64.iso                       \
  -netdev user,id=net0,net=192.168.0.0/24,dhcpstart=192.168.0.9  \
  -device virtio-net-pci,netdev=net0                             \
  -vga qxl                    \
  -device AC97
```
Meanings of all the arguments:
-m 2G → 2GB RAM
-smp 1 → 1CPUs
-hda <NAME OF IMG>.qcow2 → our 20GB variable-size disk
-boot d → boots the first virtual CD drive
-cdrom archlinux-2023.08.01-x86_64.iso → Arch Linux ISO
-netdev user,id=net0,net=192.168.0.0/24,dhcpstart=192.168.0.9 → NAT with DHCP
-device virtio-net-pci,netdev=net0 → network card
-vga qxl → graphics card
-device AC97 → sound card
