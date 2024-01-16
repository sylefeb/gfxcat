cmake_minimum_required(VERSION 3.5)

project(gfxcat)

file(GLOB sources framework/* framework/GL/*.c framework/GL/*.h)

if (EMSCRIPTEN)

  set(CMAKE_EXECUTABLE_SUFFIX ".html")
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -s DISABLE_EXCEPTION_CATCHING=1 -s EXIT_RUNTIME=0")
	set(CMAKE_EXE_LINKER_FLAGS "-s LEGACY_GL_EMULATION -s WASM=1 -s USE_GLFW=3 -s USE_WEBGL2=1 -s NO_EXIT_RUNTIME=1 -s STRICT=1  -s INITIAL_MEMORY=8MB -s INCOMING_MODULE_JS_API=canvas,noExitRuntime --shell-file ${CMAKE_CURRENT_LIST_DIR}/framework/ems/page.html")

  set(LIBS )

else()

  set(CMAKE_BUILD_TYPE Release)
  find_package(glfw3 CONFIG REQUIRED)
  set(LIBS opengl32 glfw)

endif()

file(GLOB dirs RELATIVE ${CMAKE_CURRENT_LIST_DIR} ${CMAKE_CURRENT_LIST_DIR}/*)
list(REMOVE_ITEM dirs framework .git)

foreach(dirname ${dirs})
  if (IS_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/${dirname})
    if (EXISTS ${CMAKE_CURRENT_LIST_DIR}/${dirname}/${dirname}.c)
      message("adding project ${dirname}")
      add_executable(gfxcat_${dirname} ${sources} ${dirname}/${dirname}.c)
      target_link_libraries(gfxcat_${dirname} ${LIBS})
      target_include_directories(gfxcat_${dirname} PRIVATE "${CMAKE_CURRENT_LIST_DIR}/framework")
      target_include_directories(gfxcat_${dirname} PRIVATE "${CMAKE_CURRENT_LIST_DIR}/framework/GL")
      if (EMSCRIPTEN)
      install(FILES ${CMAKE_CURRENT_BINARY_DIR}/gfxcat_${dirname}.js   DESTINATION ${CMAKE_CURRENT_LIST_DIR}/runtime/)
      install(FILES ${CMAKE_CURRENT_BINARY_DIR}/gfxcat_${dirname}.html DESTINATION ${CMAKE_CURRENT_LIST_DIR}/runtime/)
      install(FILES ${CMAKE_CURRENT_BINARY_DIR}/gfxcat_${dirname}.wasm DESTINATION ${CMAKE_CURRENT_LIST_DIR}/runtime/)
      endif()
    endif()
  endif()
endforeach()
