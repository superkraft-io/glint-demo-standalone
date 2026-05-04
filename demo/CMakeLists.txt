cmake_minimum_required(VERSION 3.25)
project(glint_demo VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

get_filename_component(GLINT_REPO_ROOT "${CMAKE_CURRENT_LIST_DIR}/.." ABSOLUTE)
set(_GLINT_DEFAULT_DEPS_DIR "${GLINT_REPO_ROOT}/third_party/skia")

include("${CMAKE_CURRENT_SOURCE_DIR}/project.cmake" OPTIONAL)

if(NOT DEFINED GLINT_DEPS_DIR OR
   NOT EXISTS "${GLINT_DEPS_DIR}/src/skia/include/core/SkCanvas.h")
  set(GLINT_DEPS_DIR "${_GLINT_DEFAULT_DEPS_DIR}" CACHE PATH
    "Skia dependency root for the standalone glint shared library." FORCE)
endif()

if(NOT DEFINED GLINT_USER_CODE_DIR)
  set(GLINT_USER_CODE_DIR "${GLINT_REPO_ROOT}/glint_user_code")
endif()

if(NOT DEFINED GLINT_USER_SHADERS_DIR)
  set(GLINT_USER_SHADERS_DIR "${GLINT_USER_CODE_DIR}/cpp/shaders")
endif()

set(_GLINT_DEFAULT_USER_SHADER_HEADER "${GLINT_USER_SHADERS_DIR}/glint_user_shaders.h")

if(NOT DEFINED GLINT_USER_SHADER_HEADER_PATH)
  if(EXISTS "${_GLINT_DEFAULT_USER_SHADER_HEADER}")
    set(GLINT_USER_SHADER_HEADER_PATH "${_GLINT_DEFAULT_USER_SHADER_HEADER}")
  endif()
endif()

add_subdirectory("${GLINT_REPO_ROOT}/third_party/glint" "${CMAKE_CURRENT_BINARY_DIR}/glint")

# Match Skia's runtime: Debug builds use /MTd, Release builds use /MT.
if(WIN32)
  foreach(_glint_tgt glint_core glint_host glint_host_win32 glint_host_win32_support)
    if(TARGET ${_glint_tgt})
      set_property(TARGET ${_glint_tgt} PROPERTY INTERFACE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
    endif()
  endforeach()
  set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif()

if(GLINT_USER_SHADER_HEADER_PATH)
  if(NOT EXISTS "${GLINT_USER_SHADER_HEADER_PATH}")
    message(FATAL_ERROR
      "GLINT_USER_SHADER_HEADER_PATH does not exist: ${GLINT_USER_SHADER_HEADER_PATH}")
  endif()

  get_filename_component(_GLINT_USER_SHADER_INCLUDE_DIR
    "${GLINT_USER_SHADER_HEADER_PATH}" DIRECTORY)
  get_filename_component(_GLINT_USER_SHADER_HEADER_NAME
    "${GLINT_USER_SHADER_HEADER_PATH}" NAME)
endif()

if(WIN32)
  add_executable(glint_demo WIN32
    demo_main.cpp
  )

  set_property(TARGET glint_demo PROPERTY
    MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
else()
  add_executable(glint_demo
    demo_main.cpp
    $<$<PLATFORM_ID:Darwin>:demo_mac_main.mm>
  )
  if(APPLE)
    set_source_files_properties(demo_mac_main.mm PROPERTIES
      COMPILE_FLAGS "-x objective-c++")
  endif()
endif()

target_include_directories(glint_demo PRIVATE
  "${GLINT_REPO_ROOT}"
  "${GLINT_REPO_ROOT}/third_party/glint/shaders"
  $<$<BOOL:${GLINT_USER_SHADER_HEADER_PATH}>:${_GLINT_USER_SHADER_INCLUDE_DIR}>
)

if(WIN32)
  target_link_libraries(glint_demo PRIVATE glint::host_win32)
  glint_configure_win32_app(glint_demo)
elseif(APPLE)
  target_link_libraries(glint_demo PRIVATE glint::host_mac)
else()
  message(FATAL_ERROR "glint_demo currently supports Windows and macOS only.")
endif()

if(GLINT_USER_SHADER_HEADER_PATH)
  target_compile_definitions(glint_demo PRIVATE
    SK_Glint_UserDefinedShaders="${_GLINT_USER_SHADER_HEADER_NAME}")
endif()

if(GLINT_BUNDLE_SHALLOW)
  target_compile_definitions(glint_demo PRIVATE GLINT_BUNDLE_SHALLOW)

  set(_GLINT_BUNDLE_DATA_DIR "${GLINT_USER_CODE_DIR}/cpp/bundle/shallow/groups/data")
  file(GLOB _GLINT_BUNDLE_BINS "${_GLINT_BUNDLE_DATA_DIR}/*.bin")
  foreach(_bin IN LISTS _GLINT_BUNDLE_BINS)
    add_custom_command(TARGET glint_demo POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_FILE_DIR:glint_demo>/bundle"
      COMMAND ${CMAKE_COMMAND} -E copy_if_different
              "${_bin}"
              "$<TARGET_FILE_DIR:glint_demo>/bundle/"
      VERBATIM)
  endforeach()
endif()

if(GLINT_BUNDLE_DEEP)
  target_compile_definitions(glint_demo PRIVATE GLINT_BUNDLE_DEEP)

  file(GLOB _GLINT_BUNDLE_DEEP_SRCS
    "${GLINT_USER_CODE_DIR}/cpp/bundle/deep/groups/*.cpp")
  target_sources(glint_demo PRIVATE ${_GLINT_BUNDLE_DEEP_SRCS})
endif()