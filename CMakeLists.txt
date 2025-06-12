cmake_minimum_required(VERSION 3.10)
project(UdpRawSocketSim CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_BUILD_TYPE Release)

add_executable(udp-raw-socket-sim src/main.cc src/RawUdpSender.cc src/RawUdpReceiver.cc)

target_include_directories(udp-raw-socket-sim PRIVATE
    ${CMAKE_SOURCE_DIR}/third_party/ns3/build/include
    ${CMAKE_SOURCE_DIR}/src
)

set(NS3_LIB_DIR ${CMAKE_SOURCE_DIR}/third_party/ns3/build/lib)
find_library(NS3_CORE_LIB ns3-dev-core-default PATHS ${NS3_LIB_DIR} NO_DEFAULT_PATH)
find_library(NS3_NETWORK_LIB ns3-dev-network-default PATHS ${NS3_LIB_DIR} NO_DEFAULT_PATH)
find_library(NS3_CSMA_LIB ns3-dev-csma-default PATHS ${NS3_LIB_DIR} NO_DEFAULT_PATH)
find_library(NS3_INTERNET_LIB ns3-dev-internet-default PATHS ${NS3_LIB_DIR} NO_DEFAULT_PATH)
find_library(NS3_BRIDGE_LIB ns3-dev-bridge-default PATHS ${NS3_LIB_DIR} NO_DEFAULT_PATH)
find_library(NS3_APPLICATIONS_LIB ns3-dev-applications-default PATHS ${NS3_LIB_DIR} NO_DEFAULT_PATH)

if(NOT NS3_CORE_LIB OR NOT NS3_NETWORK_LIB OR NOT NS3_CSMA_LIB OR NOT NS3_INTERNET_LIB OR NOT NS3_BRIDGE_LIB OR NOT NS3_APPLICATIONS_LIB)
    message(FATAL_ERROR "One or more NS3 libraries not found in ${NS3_LIB_DIR}. Ensure NS3 is built correctly using ./configure.sh.")
endif()

target_link_libraries(udp-raw-socket-sim
    ${NS3_CORE_LIB}
    ${NS3_NETWORK_LIB}
    ${NS3_CSMA_LIB}
    ${NS3_INTERNET_LIB}
    ${NS3_BRIDGE_LIB}
    ${NS3_APPLICATIONS_LIB}
)
