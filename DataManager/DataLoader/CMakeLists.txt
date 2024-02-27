
# Define the library
add_library(DataLoader SHARED
    src/Cloud.cpp
    DataLoader/Cloud.h
)

# Specify include directories
target_include_directories(DataLoader PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/DataLoader>
    $<INSTALL_INTERFACE:include>
)

# Install the library target and specify public headers
install(TARGETS DataLoader
    LIBRARY DESTINATION lib
    PUBLIC_HEADER DESTINATION DataLoader
)
