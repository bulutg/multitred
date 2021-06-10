cmake_minimum_required(VERSION 3.19)
project(siyah1)

set(CMAKE_CXX_STANDARD 14)

add_executable(Partner Partner.cpp modules/ThreadedModule.cpp modules/ThreadedModule.h modules/components/SafeQueue.h modules/ThreadedTCPServer.cpp modules/ThreadedTCPServer.h lib/json.hpp modules/helpers/ParseHelper.h modules/components/CircularBuffer.h modules/JsonThreadedTCPServer.cpp modules/JsonThreadedTCPServer.h modules/ThreadedTCPClient.cpp modules/ThreadedTCPClient.h)
add_executable(Supervisor Supervisor.cpp modules/ThreadedModule.cpp modules/ThreadedModule.h modules/components/SafeQueue.h modules/ThreadedTCPServer.cpp modules/ThreadedTCPServer.h lib/json.hpp modules/helpers/ParseHelper.h modules/components/CircularBuffer.h modules/ThreadedTCPServerKeepPartnerAlive.cpp modules/ThreadedTCPServerKeepPartnerAlive.h)