
# Define the library
add_library(AlgoBase SHARED
    src/AlgoBase.cpp
    AlgoBase/AlgoBase.h
)

# Linked libraries
target_link_libraries(AlgoBase DataLoader Config)

# Specify include directories
target_include_directories(AlgoBase PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/AlgoBase>
    $<INSTALL_INTERFACE:include>
)

# Install the library target and specify public headers
install(TARGETS AlgoBase
    LIBRARY DESTINATION lib
    PUBLIC_HEADER DESTINATION AlgoBase
)
