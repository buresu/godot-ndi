if(WIN32)
  if(DEFINED ENV{NDI_SDK_DIR})
    set(NDI_FOUND TRUE)
    set(NDI_DIR $ENV{NDI_SDK_DIR})
    string(REPLACE "\\" "/" NDI_DIR "${NDI_DIR}")
    if(CMAKE_SIZEOF_VOID_P EQUAL 4)
      set(NDI_ARCH "x86")
    else()
      set(NDI_ARCH "x64")
    endif()
    set(NDI_INCLUDE_DIR "${NDI_DIR}/Include")
    set(NDI_LIBRARY_DIR "${NDI_DIR}/Lib/${NDI_ARCH}")
    set(NDI_LICENSE_DIR "${NDI_DIR}/Bin/${NDI_ARCH}")
    set(NDI_LIBS "Processing.NDI.Lib.${NDI_ARCH}")
  else()
    set(NDI_FOUND FALSE)
  endif()
elseif(APPLE)
  if(EXISTS "/Library/NDI SDK for Apple/include/Processing.NDI.Lib.h")
    set(NDI_FOUND TRUE)
    set(NDI_DIR "/Library/NDI SDK for Apple")
    set(NDI_INCLUDE_DIR "${NDI_DIR}/include")
    set(NDI_LIBRARY_DIR "${NDI_DIR}/lib/macOS")
    set(NDI_LICENSE_DIR "${NDI_DIR}/licenses")
    file(GLOB NDI_LIBS "${NDI_LIBRARY_DIR}/*.dylib")
  else()
    set(NDI_FOUND FALSE)
  endif()
elseif(UNIX)
  if(EXISTS "${NDI_SDK_DIR}/include/Processing.NDI.Lib.h")
    set(NDI_FOUND TRUE)
    set(NDI_DIR ${NDI_SDK_DIR})
    if(CMAKE_SIZEOF_VOID_P EQUAL 4)
      set(NDI_ARCH "i686")
    else()
      set(NDI_ARCH "x86_64")
    endif()
    set(NDI_INCLUDE_DIR "${NDI_DIR}/include")
    set(NDI_LIBRARY_DIR "${NDI_DIR}/lib/${NDI_ARCH}-linux-gnu")
    set(NDI_LICENSE_DIR "${NDI_DIR}/licenses")
    set(NDI_LIBS "ndi")
  elseif(EXISTS "/usr/include/Processing.NDI.Lib.h")
    set(NDI_FOUND TRUE)
    set(NDI_DIR "/usr")
    set(NDI_INCLUDE_DIR "${NDI_DIR}/include")
    set(NDI_LIBRARY_DIR "${NDI_DIR}/lib")
    set(NDI_LICENSE_DIR "${NDI_DIR}/share/licenses/ndi-sdk")
    set(NDI_LIBS "ndi")
  else()
    set(NDI_FOUND FALSE)
  endif()
endif()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(NDI DEFAULT_MSG NDI_DIR ${NDI_FOUND})