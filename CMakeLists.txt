cmake_minimum_required (VERSION 2.8)
project(EI_View)

# required for Ogre 1.11+
set(CMAKE_CXX_STANDARD 11)

## [discover_ogre]
# specify which version and components you need
find_package(OGRE 1.11 REQUIRED COMPONENTS Bites RTShaderSystem)

# copy resource.cfg next to our binaries where OGRE looks for it
file(COPY ${OGRE_CONFIG_DIR}/resources.cfg DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

# add the source files as usual
add_executable(ei_view src/ei_view.cpp)

# this also sets the includes and pulls third party dependencies
target_link_libraries(ei_view OgreBites OgreRTShaderSystem)
# ${OGRE_LIBRARIES}

# add the source files as usual
add_executable(resutil src/resutil.cpp)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

