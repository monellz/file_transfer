# File Transfer

## Build

```bash
mkdir build
cd build
cmake ..
make
```

## Usage

server port is 5678(defined in ```tools/util.h```)

```bash
# client
# ./client <Server IPaddress> <Upload/Download: 1/2> <File Name> <NotContinue/Continue: 0/1
./client 127.0.0.1 2 test.txt 1

# server
./server
```

