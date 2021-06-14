cmake_minimum_required(VERSION 3.16)
project(siyah1)

set(CMAKE_CXX_STANDARD 14)
SET(CMAKE_CXX_FLAGS -pthread)

add_executable(Partner PartnerMain.cpp src/modules/ThreadedModule.cpp src/modules/headers/servers/ThreadedModule.h src/modules/headers/components/SafeQueue.h src/modules/ThreadedTCPServer.cpp src/modules/headers/servers/ThreadedTCPServer.h lib/json.hpp src/modules/headers/helpers/ParseHelper.h src/modules/headers/components/CircularBuffer.h src/modules/JsonThreadedTCPServer.cpp src/modules/headers/servers/JsonThreadedTCPServer.h src/modules/ThreadedTCPClient.cpp src/modules/headers/clients/ThreadedTCPClient.h)
add_executable(Supervisor SupervisorMain.cpp src/modules/ThreadedModule.cpp src/modules/headers/servers/ThreadedModule.h src/modules/headers/components/SafeQueue.h src/modules/ThreadedTCPServer.cpp src/modules/headers/servers/ThreadedTCPServer.h lib/json.hpp src/modules/headers/helpers/ParseHelper.h src/modules/headers/components/CircularBuffer.h src/modules/ThreadedTCPServerKeepPartnersAlive.cpp src/modules/headers/servers/ThreadedTCPServerKeepPartnersAlive.h)
