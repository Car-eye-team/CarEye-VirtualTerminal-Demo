cmake_minimum_required(VERSION 3.12)
PROJECT(JTT808VirtualTerminal)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-std=c++17 -DHPSOCKET_STATIC_LIB")
SET(LIB_SRC AutoIncressContent.cpp CommandClasses.cpp JT808Controller.cpp JTT808Head.cpp RTPPusher.cpp
    stdafx.cpp BlokingRingBuffer.cpp JTT808Packer.cpp Semaphore.cpp utils.cpp AutoIncressContent.h cmddeclare.h
    JT808Controller.h JTT808Packer.h Semaphore.h targetver.h BlockingRingBuffer.h CommandClasses.h
    JT808VirutalTerminal.h JTT808Head.h RTPPusher.h stdafx.h utils.h libhpsocket.a)
ADD_LIBRARY(JTT808VirtualTerminal SHARED ${LIB_SRC})
ADD_LIBRARY(JTT808VirtualTerminal_static STATIC ${LIB_SRC})
SET_TARGET_PROPERTIES(JTT808VirtualTerminal_static PROPERTIES OUTPUT_NAME "JTT808VirtualTerminal")
SET_TARGET_PROPERTIES(JTT808VirtualTerminal_static PROPERTIES CLEAN_DIRECT_OUTPUT 1)
SET_TARGET_PROPERTIES(JTT808VirtualTerminal PROPERTIES CLEAN_DIRECT_OUTPUT 1)
SET_TARGET_PROPERTIES(JTT808VirtualTerminal PROPERTIES VERSION 1 SOVERSION 1)
