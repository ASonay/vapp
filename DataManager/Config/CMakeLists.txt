# Define the library
add_library(Config SHARED
    src/Config.cpp
    Config/Config.h
)

# Linked libraries
target_link_libraries(Config DataLoader)

# Specify include directories
target_include_directories(Config PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Config>
    $<INSTALL_INTERFACE:include>
)

# Install the library target and specify public headers
install(TARGETS Config
    LIBRARY DESTINATION lib
    PUBLIC_HEADER DESTINATION Config
)