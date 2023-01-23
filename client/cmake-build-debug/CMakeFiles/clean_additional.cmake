# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles/hangman_autogen.dir/AutogenUsed.txt"
  "CMakeFiles/hangman_autogen.dir/ParseCache.txt"
  "hangman_autogen"
  )
endif()
