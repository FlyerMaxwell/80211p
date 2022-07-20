cmake_minimum_required(VERSION 3.22)
project(80211_INFOCOM)

set(CMAKE_CXX_STANDARD 14)

add_executable(80211_INFOCOM main.cpp parameters.cpp parameters.h vehicle.cpp vehicle.h common.cpp common.h UpLocation.cpp UpLocation.h Protocol_80211.cpp Protocol_80211.h packets.cpp packets.h)
