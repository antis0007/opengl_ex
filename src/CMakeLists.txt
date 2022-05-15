set(IMGUI_SRC
        imgui/imgui_impl_opengl3.h
        imgui/imgui_impl_opengl3.cpp
        imgui/imgui_impl_opengl3_loader.h
        imgui/imgui_impl_glfw.h
        imgui/imgui_impl_glfw.cpp
        imgui/imgui.h
        imgui/imgui.cpp
        imgui/imgui_draw.cpp
        imgui/imgui_tables.cpp
        imgui/imgui_widgets.cpp
        imgui/imgui_demo.cpp
        )

IF (WIN32)
  add_executable(${PROJECT_NAME}
  #WIN32
  #Uncomment Above for Hidden Console on WIN32
  main.cpp
  shader.h
  gl.c
  ${IMGUI_SRC}
  )
ELSE()
  add_executable(${PROJECT_NAME}
  main.cpp
  shader.h
  gl.c
  ${IMGUI_SRC}
  )
ENDIF()
