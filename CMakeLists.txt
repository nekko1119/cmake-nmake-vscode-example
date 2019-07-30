cmake_minimum_required(VERSION 3.15)

project(vscodecpp)

file(GLOB_RECURSE FILES ./src/*.cpp)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ../bin)

add_compile_definitions(UNICODE _UNICODE)
include_directories("$ENV{DXSDK_DIR}/Include")
link_directories("$ENV{DXSDK_DIR}/Lib/x64")

if (MSVC)
  # compiler options
  # common
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /EHsc /WX /sdl /MP /permissive-")

  # release and debug options

  # optimization
  if (CMAKE_CXX_FLAGS_RELEASE MATCHES "/O[dx12]")
    string(REGEX REPLACE "/O[dx12]" "/O2" CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE}")
  else()
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /O2")
  endif()

  set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /O2")


  if (CMAKE_CXX_FLAGS_DEBUG MATCHES "/O[dx12]")
    string(REGEX REPLACE "/O[dx12]" "/Od" CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG}")
  else()
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_RELEASE} /Od")
  endif()

  # warning level
  if(CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
    string(REGEX REPLACE "/W[0-4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
  else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
  endif()

  # runtime library
  if(CMAKE_CXX_FLAGS MATCHES "/M[DT]")
    string(REGEX REPLACE "/M[DT]" "/MT" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
  endif()
  if(CMAKE_CXX_FLAGS MATCHES "/M[DT]d")
    string(REGEX REPLACE "/M[DT]d" "/MTd" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
  endif()

  # linker options

  # architecture
  # this configuration may not need. cmake determines architecture from environment
  if(CMAKE_EXE_LINKER_FLAGS MATCHES "/machine:(X86|x86|X64|x64)")
    string(REGEX REPLACE "/machine:(X86|x86|X64|x64)" "/machine:x64" CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS}")
  else()
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /machine:x64")
  endif()

  set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINER_FLAGS} /OPT:REF /OPT:ICF")
  set(CMAKE_SHARED_LINKER_FLAGS_RELEASE "${CMAKE_SHARED_LINER_FLAGS} /OPT:REF /OPT:ICF")
else()
  message(FATAL_ERROR "this project supports msvc only")
endif()

message(STATUS ${CMAKE_CXX_FLAGS})
message(STATUS ${CMAKE_CXX_FLAGS_RELEASE})

add_executable(vscodecpp WIN32 ${FILES} ./src/resource.rc)
