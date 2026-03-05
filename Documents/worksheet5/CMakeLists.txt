cmake_minimum_required(VERSION 3.16)
project(worksheet5 LANGUAGES CXX)

if(WIN32)
  set(CPACK_GENERATOR "NSIS")
else()
  set(CPACK_GENERATOR "ZIP")
endif()

add_library(maths STATIC adder.cpp)
target_compile_definitions(maths PUBLIC maths_STATIC)

add_executable(calc calc.cpp)
target_link_libraries(calc PRIVATE maths)

install(TARGETS calc maths
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib/static
)

include(CPack)
