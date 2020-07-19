cmake_minimum_required(VERSION 3.12)

project(Traced)

add_subdirectory(src)

include(FetchContent)
FetchContent_Declare(
  graphics-math
  GIT_REPOSITORY https://github.com/jvmnielsen/graphics-math.git
  GIT_TAG dev
)
FetchContent_MakeAvailable(graphics-math)