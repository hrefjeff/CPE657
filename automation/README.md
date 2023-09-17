# CPE657 OS Group Automation Tools

## How to use

Install Ansible on your machine.

```bash
sudo apt update && sudo apt upgrade -y
sudo apt install ansible
```

Then run the following command:

```bash
ansible-playbook main.yaml
```

## How to build QEMU on WSL

1. `mkdir /path/to/qemu/build-softmmu`
2. `mkdir /path/to/qemu/build-linux-user`
3. `cd /path/to/qemu/build-softmmu`
4. `../configure --enable-debug --target-list=x86_64-softmmu`
5. `make -j2` (# adjust the number of threads to your liking)
6. `mkdir /path/to/qemu/build-linux-user`
7. `cd /path/to/qemu/build-linux-user`
8. `../configure --enable-debug --target-list=x86_64-linux-user`
9. `make -j2` (# adjust the number of threads to your liking)

## How to create image and install Bohdi

1. `qemu-img create -f qcow2 bohdi.qcow2 15G`
2. Install bohdi using the following command:

```
qemu-system-x86_64 \
-m 2G   \
-smp 1  \
-hda vdisktest.qcow2    \
-boot d \
-cdrom /path/to/bodhi-7.0.0-64.iso                 \
-netdev user,id=net0,net=192.168.0.0/24,dhcpstart=192.168.0.9  \
-device virtio-net-pci,netdev=net0                             \
-vga qxl                                                       \
-device AC97
```
