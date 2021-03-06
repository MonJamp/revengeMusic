IF(CMAKE_VERSION VERSION_GREATER 3.6 OR CMAKE_VERSION VERSION_EQUAL 3.6)
  cmake_minimum_required (VERSION 3.6)
ELSE()
  cmake_minimum_required (VERSION 3.4)
  IF(WIN32)
    message( CMake version is less than 3.6, you must manually set revengeMusic as the default startup project for use in Visual Studio )
  ENDIF()
ENDIF()

project ("revengeMusic"
           LANGUAGES CXX
           VERSION 0.9.0)

configure_file("${CMAKE_SOURCE_DIR}/include/config.h.in"
               "${CMAKE_SOURCE_DIR}/include/config.h")

include_directories("${CMAKE_SOURCE_DIR}/include/")

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")
set(CMAKE_CONFIGURATION_TYPES "Debug;Release")

IF(MSVC)
  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /Z7 /Od /W4")
  set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /O2")
  add_definitions(/D_CRT_SECURE_NO_WARNINGS) # Ignore secure function warnings
ELSEIF(CMAKE_COMPILER_IS_GNUCXX)
  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -O0 -Wall")
  set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O2")
  SET(GCC_COVERAGE_COMPILE_FLAGS "-std=c++11")
ENDIF(MSVC)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}" )


add_executable(
  revengeMusic
  src/main.cpp
  src/MessageQueue.cpp
  src/Sound.cpp
  src/Logger.cpp
)
IF(WIN32)
  set(Boost_USE_STATIC_LIBS ON)
  set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT "revengeMusic")
ELSEIF(UNIX)
  add_custom_target (
    deb
    COMMAND ${CMAKE_SOURCE_DIR}/packages/debian/build-deb.sh ${projectVersion}
)

ENDIF(WIN32)

#Find Boost
find_package(Boost 1.58 COMPONENTS system filesystem date_time REQUIRED)
IF(Boost_FOUND)
  include_directories(SYSTEM ${Boost_INCLUDE_DIRS})
  target_link_libraries(revengeMusic ${Boost_LIBRARIES})
ENDIF(Boost_FOUND)

#Find pthreads
find_package(Threads)
IF(Threads_FOUND)
  target_link_libraries(revengeMusic ${CMAKE_THREAD_LIBS_INIT})
ENDIF(Threads_FOUND)

IF(UNIX)
    #Find LibRt
    find_package(LibRt)
    IF(LIBRT_FOUND)
      target_link_libraries(revengeMusic ${LIBRT_LIBRARIES})
    ENDIF(LIBRT_FOUND)
ENDIF(UNIX)

#Find FMOD
find_package(FMOD)
IF(FMOD_FOUND)
  include_directories(SYSTEM ${FMOD_INCLUDE_DIRS})
  target_link_libraries(revengeMusic  ${FMOD_LIBRARIES})
ENDIF(FMOD_FOUND)
