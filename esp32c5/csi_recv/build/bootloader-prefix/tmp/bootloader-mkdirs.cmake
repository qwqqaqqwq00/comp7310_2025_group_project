# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

# If CMAKE_DISABLE_SOURCE_CHANGES is set to true and the source directory is an
# existing directory in our source tree, calling file(MAKE_DIRECTORY) on it
# would cause a fatal error, even though it would be a no-op.
if(NOT EXISTS "E:/Application/ESP-IDF/components/bootloader/subproject")
  file(MAKE_DIRECTORY "E:/Application/ESP-IDF/components/bootloader/subproject")
endif()
file(MAKE_DIRECTORY
  "E:/_MyWork/2025-2025/hku/COMP7310-AIoT/comp7310_2025_group_project/esp32c5/csi_recv/build/bootloader"
  "E:/_MyWork/2025-2025/hku/COMP7310-AIoT/comp7310_2025_group_project/esp32c5/csi_recv/build/bootloader-prefix"
  "E:/_MyWork/2025-2025/hku/COMP7310-AIoT/comp7310_2025_group_project/esp32c5/csi_recv/build/bootloader-prefix/tmp"
  "E:/_MyWork/2025-2025/hku/COMP7310-AIoT/comp7310_2025_group_project/esp32c5/csi_recv/build/bootloader-prefix/src/bootloader-stamp"
  "E:/_MyWork/2025-2025/hku/COMP7310-AIoT/comp7310_2025_group_project/esp32c5/csi_recv/build/bootloader-prefix/src"
  "E:/_MyWork/2025-2025/hku/COMP7310-AIoT/comp7310_2025_group_project/esp32c5/csi_recv/build/bootloader-prefix/src/bootloader-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "E:/_MyWork/2025-2025/hku/COMP7310-AIoT/comp7310_2025_group_project/esp32c5/csi_recv/build/bootloader-prefix/src/bootloader-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "E:/_MyWork/2025-2025/hku/COMP7310-AIoT/comp7310_2025_group_project/esp32c5/csi_recv/build/bootloader-prefix/src/bootloader-stamp${cfgdir}") # cfgdir has leading slash
endif()
