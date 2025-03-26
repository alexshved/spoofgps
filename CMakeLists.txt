cmake_minimum_required(VERSION 3.27)
project(spoofgps CXX C)
set(CMAKE_C_STANDARD 17)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}  ")
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
find_package(UHD "3.10.0")
find_package(PkgConfig REQUIRED)
pkg_check_modules(libusb REQUIRED libusb-1.0)
include_directories(${UHD_INCLUDE_DIRS} )
message("UHD_INCLUDE_DIRS ${UHD_INCLUDE_DIRS} ")
set(PROEXTS_HEADERS
    ./header/com.h
    ./header/until.h
    ./header/auch.h
    ./header/header.h
    ./header/usrp.h
    ./header/getopt.h
    ./header/gpssim.h
    ./header/thread.h
    ./header/subframe.h
    ./header/calcdata.h
    ./header/rinex.h
    ./header/rinexold.h
    ./header/comportgps.h
    ./header/testtheadsgps.h
    ./header/gpsuniit.h
    )
set(PROECTS_SOURCES
    ./main.c
    ./until.c
    ./com.c
    ./usrp.c
    ./getopt.c
    ./gpssim.c
    ./thread.c
    ./subframe.c
    ./calcdata.c
    ./rinex.c
    ./rinexold.c
    ./comportgps.c
    ./testtheadsgps.c
    ./gpsuniit.c
        )
add_executable(spoofgps ${PROEXTS_HEADERS} ${PROECTS_SOURCES})
target_link_libraries(spoofgps PRIVATE wsock32 ws2_32 "c:/msys64/mingw64/lib/libusb-1.0.a" ${UHD_LIBRARIES} ${libusb}  )
