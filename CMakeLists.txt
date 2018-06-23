cmake_minimum_required(VERSION 3.9)
project(EvoSim)

set(CMAKE_CXX_STANDARD 17)

add_executable(EvoSim main.cpp agent.cpp agent.h configuration.cpp configuration.h neuralnetwork.cpp neuralnetwork.h methods.cpp methods.h header.h manager.cpp manager.h)


set(CMAKE_INCLUDE_PATH /Cpp_Libraries/boost_1_66_0)
include_directories(/Cpp_Libraries/boost_1_66_0)

#message(STATUS "adding boost (External Libary)")
#set(Boost_USE_STATIC_LIBS OFF) 
#set(Boost_USE_MULTITHREADED ON)  
#set(Boost_USE_STATIC_RUNTIME OFF) 
#find_package(Boost 1.45.0 COMPONENTS System) 

#if(Boost_FOUND)
#	message(STATUS "boost found")
#    include_directories(${Boost_INCLUDE_DIRS}) 
#    add_executable(progname file1.cxx file2.cxx) 
#    target_link_libraries(progname ${Boost_LIBRARIES})
#endif()