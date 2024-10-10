# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\Calender_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\Calender_autogen.dir\\ParseCache.txt"
  "Calender_autogen"
  )
endif()
