cmake_minimum_required(VERSION 3.16)
project(siyah1)

set(CMAKE_CXX_STANDARD 14)
SET(CMAKE_CXX_FLAGS -pthread)

add_executable(Partner PartnerMain.cpp src/modules/ThreadedModule.cpp src/modules/ThreadedModule.h src/modules/components/SafeQueue.h src/modules/ThreadedTCPServer.cpp src/modules/ThreadedTCPServer.h lib/json.hpp src/modules/helpers/ParseHelper.h src/modules/components/CircularBuffer.h src/modules/JsonThreadedTCPServer.cpp src/modules/JsonThreadedTCPServer.h src/modules/ThreadedTCPClient.cpp src/modules/ThreadedTCPClient.h)
add_executable(Supervisor SupervisorMain.cpp src/modules/ThreadedModule.cpp src/modules/ThreadedModule.h src/modules/components/SafeQueue.h src/modules/ThreadedTCPServer.cpp src/modules/ThreadedTCPServer.h lib/json.hpp src/modules/helpers/ParseHelper.h src/modules/components/CircularBuffer.h src/modules/ThreadedTCPServerKeepPartnersAlive.cpp src/modules/ThreadedTCPServerKeepPartnersAlive.h src/Supervisor.cpp src/Supervisor.h)
