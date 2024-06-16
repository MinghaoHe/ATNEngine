# Copyright (C) by MinghaoHe - minghaohe@tencent.com
# This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)

FIND_PACKAGE(Vulkan REQUIRED)

INCLUDE_DIRECTORIES(
        ${PROJECT_ROOT_DIR}
        ${SOURCE_ROOT_DIR}
        ${Vulkan_INCLUDE_DIR}
        ${THIRD_PARTY_DIR}
        ${THIRD_PARTY_DIR}/glm)

ADD_DEFINITIONS(
  -DGLFW_INCLUDE_VULKAN
)

SET(LIBS ${Vulkan_LIBRARIES} glfw tinyobjloader)

LINK_LIBRARIES(${LIBS})

SET(PROJECT_SOURCE_FILES
        # atn
        main.cc

        # atn/engine
        atn/engine.h
        atn/engine.cc

        atn/engine/define.h
        atn/engine/canvas.h
        atn/engine/canvas.cc
        atn/engine/world.h
        atn/engine/world.cc

        # atn/engine/canvas
        atn/engine/canvas/define.h
        atn/engine/canvas/glfw.h
        atn/engine/canvas/glfw.cc

        # atn/engine/world
        atn/engine/world/prime_player.h
        atn/engine/world/prime_player.cc
        atn/engine/world/tick_lock.h
        atn/engine/world/tick_lock.cc

        # atn/engine/ecs
        atn/engine/ecs/define.h
        atn/engine/ecs/entity.h
        atn/engine/ecs/entity.cc
        atn/engine/ecs/component.h
        atn/engine/ecs/component.cc
        atn/engine/ecs/system.h
        atn/engine/ecs/system.cc

        # atn/engine/render
        atn/engine/render/component.h
        atn/engine/render/component.cc
        atn/engine/render/system.h
        atn/engine/render/system.cc
        atn/engine/render/config.h

        # atn/engine/logic
        atn/engine/logic/component.h
        atn/engine/logic/component.cc
        atn/engine/logic/system.h
        atn/engine/logic/system.cc

        # atn/engine/physics
        atn/engine/physics/component.h
        atn/engine/physics/component.cc
        atn/engine/physics/system.h
        atn/engine/physics/system.cc
)

SET(PROJECT_SHADER_FILES
        atn/engine/render/shader/deemo.vert
        atn/engine/render/shader/deemo.frag
)

ADD_EXECUTABLE(${PROJECT} ${PROJECT_SOURCE_FILES} ${PROJECT_SHADER_FILES})
SOURCE_GROUP(TREE ${PROJECT_ROOT_DIR} FILES ${PROJECT_SOURCE_FILES} ${PROJECT_SHADER_FILES})


ADD_CUSTOM_COMMAND(
    TARGET ${PROJECT}
    PRE_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory  ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/$<CONFIGURATION>/shader)
FIND_PROGRAM(GLSLC_PROGRAM glslc REQUIRED)
FILE(GLOB SHADERS 
    ${SOURCE_ROOT_DIR}/atn/engine/render/shader/*.vert
    ${SOURCE_ROOT_DIR}/atn/engine/render/shader/*.frag)
FOREACH (SHADER ${SHADERS})
  GET_FILENAME_COMPONENT(SPRIV ${SHADER} NAME)
  ADD_CUSTOM_COMMAND(
      TARGET ${PROJECT}
      PRE_BUILD
      COMMAND ${GLSLC_PROGRAM} ${SHADER} -o ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/$<CONFIGURATION>/shader/${SPRIV}.spv)
ENDFOREACH ()

