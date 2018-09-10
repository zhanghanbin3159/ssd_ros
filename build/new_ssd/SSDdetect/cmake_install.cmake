# Install script for directory: /media/ziwei/Harddisk02/HanBin/TOOL/workspace_C++/Detect/src/new_ssd/SSDdetect

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/media/ziwei/Harddisk02/HanBin/TOOL/workspace_C++/Detect/install")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "1")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/ssddetect/msg" TYPE FILE FILES
    "/media/ziwei/Harddisk02/HanBin/TOOL/workspace_C++/Detect/src/new_ssd/SSDdetect/msg/BoundingBox.msg"
    "/media/ziwei/Harddisk02/HanBin/TOOL/workspace_C++/Detect/src/new_ssd/SSDdetect/msg/BoundingBoxes.msg"
    )
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/ssddetect/cmake" TYPE FILE FILES "/media/ziwei/Harddisk02/HanBin/TOOL/workspace_C++/Detect/build/new_ssd/SSDdetect/catkin_generated/installspace/ssddetect-msg-paths.cmake")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE DIRECTORY FILES "/media/ziwei/Harddisk02/HanBin/TOOL/workspace_C++/Detect/devel/include/ssddetect")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/roseus/ros" TYPE DIRECTORY FILES "/media/ziwei/Harddisk02/HanBin/TOOL/workspace_C++/Detect/devel/share/roseus/ros/ssddetect")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/common-lisp/ros" TYPE DIRECTORY FILES "/media/ziwei/Harddisk02/HanBin/TOOL/workspace_C++/Detect/devel/share/common-lisp/ros/ssddetect")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/gennodejs/ros" TYPE DIRECTORY FILES "/media/ziwei/Harddisk02/HanBin/TOOL/workspace_C++/Detect/devel/share/gennodejs/ros/ssddetect")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  execute_process(COMMAND "/usr/bin/python" -m compileall "/media/ziwei/Harddisk02/HanBin/TOOL/workspace_C++/Detect/devel/lib/python2.7/dist-packages/ssddetect")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/python2.7/dist-packages" TYPE DIRECTORY FILES "/media/ziwei/Harddisk02/HanBin/TOOL/workspace_C++/Detect/devel/lib/python2.7/dist-packages/ssddetect")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/pkgconfig" TYPE FILE FILES "/media/ziwei/Harddisk02/HanBin/TOOL/workspace_C++/Detect/build/new_ssd/SSDdetect/catkin_generated/installspace/ssddetect.pc")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/ssddetect/cmake" TYPE FILE FILES "/media/ziwei/Harddisk02/HanBin/TOOL/workspace_C++/Detect/build/new_ssd/SSDdetect/catkin_generated/installspace/ssddetect-msg-extras.cmake")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/ssddetect/cmake" TYPE FILE FILES
    "/media/ziwei/Harddisk02/HanBin/TOOL/workspace_C++/Detect/build/new_ssd/SSDdetect/catkin_generated/installspace/ssddetectConfig.cmake"
    "/media/ziwei/Harddisk02/HanBin/TOOL/workspace_C++/Detect/build/new_ssd/SSDdetect/catkin_generated/installspace/ssddetectConfig-version.cmake"
    )
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/ssddetect" TYPE FILE FILES "/media/ziwei/Harddisk02/HanBin/TOOL/workspace_C++/Detect/src/new_ssd/SSDdetect/package.xml")
endif()

