# packets_qt
Multiplatform (Windows, Linux) TCP/UDP packets analyser with Qt display

Note: requires administrator privileges 
<br>
Analysis packets and displays protocol (TCP/UDP), source IP address, destination IP address
and packe size in a Qt framework display table

```c++
struct packet_info_t
{
  std::string timestamp;
  std::string source_ip;
  std::string dest_ip;
  std::string protocol;
  int length;
};
```

## build

Dependencies: CMake, Qt 

```bash
git clone https://github.com/pedro-vicente/packets_qt
cd packets_qt
./build.cmake.sh
```

## run

```bash
cd build/packets
sudo ./packets
```



![Image](https://github.com/user-attachments/assets/adfc70e6-cebd-4030-8566-d0d9e628c876)

