cmake_minimum_required(VERSION 3.16)
project(siyah1)

set(CMAKE_CXX_STANDARD 14)
SET(CMAKE_CXX_FLAGS -pthread)

add_executable(Partner PartnerMain.cpp src/ThreadedModule.cpp src/headers/modules/ThreadedModule.h src/headers/components/SafeQueue.h src/ThreadedTCPServer.cpp src/headers/servers/ThreadedTCPServer.h lib/json.hpp src/headers/helpers/ParseHelper.h src/headers/components/CircularBuffer.h src/JsonThreadedTCPServer.cpp src/headers/servers/JsonThreadedTCPServer.h src/ThreadedTCPClient.cpp src/headers/clients/ThreadedTCPClient.h src/headers/components/Partner.h src/ThreadedPollerModule.cpp src/headers/modules/ThreadedPollerModule.h)
add_executable(Supervisor SupervisorMain.cpp src/ThreadedModule.cpp src/headers/modules/ThreadedModule.h src/headers/components/SafeQueue.h src/ThreadedTCPServer.cpp src/headers/servers/ThreadedTCPServer.h lib/json.hpp src/headers/helpers/ParseHelper.h src/headers/components/CircularBuffer.h src/ThreadedTCPServerKeepPartnersAlive.cpp src/headers/servers/ThreadedTCPServerKeepPartnersAlive.h src/ThreadedPollerModule.cpp src/headers/modules/ThreadedPollerModule.h)
