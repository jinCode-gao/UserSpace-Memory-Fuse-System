# UserSpace-Memory-Fuse-System


## About

UserSpace-Memory-Fuse-System (UMFS) is a memory-based userspace file system developed using the libfuse library, the storage medium is memory, and it has the characteristics of fast read and write speed.It can be used for temporary file systems with high-speed read and write requirements.

## Getting Started Guide

Before running UMFS, make sure you have libfuse installed on your system.
```
sudo apt-get install libfuse3-dev
```
Open terminal and navigate to the folder where UMFS source code is located. Use the make command to compile the program:
```
make
```
Moutpoint is the mounted folder.To run UMFS and mount it, use the following command:
```
./UMFS <mountpoint>
```
Now UMFS can be used under mountpoint.

## Supported Features

The following commands are supported in UMFS:
- touch
- mkdir
- mv
- rm
- echo
- cat
- ls
- cd
