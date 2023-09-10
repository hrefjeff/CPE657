# CPE657

##  Team members

George, Amber, Harsimran, Jeffrey

## Documentation Links

- [QEMU Documentation](https://wiki.qemu.org/Hosts/Linux)

## Frequently Used Commands

### Running archlinux in QEMU

```bash
./x86_64-softmmu/qemu-system-x86_64 -m 1024 -drive if=virtio,file=test.qcow2,cache=none -cdrom /path/to/iso/archlinux-2023.08.01-x86_64.iso
```
