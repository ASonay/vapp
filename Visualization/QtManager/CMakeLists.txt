
# Define the library
add_library(QtManager SHARED
    src/QtManager.cpp
    QtManager/QtManager.h
)

# Linked libraries
target_link_libraries(QtManager Config AlgoBase Qt5::Widgets Qt5::Core Qt5::3DCore Qt5::3DRender Qt5::3DInput Qt5::3DExtras)

# Specify include directories
target_include_directories(QtManager PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/QtManager>
    $<INSTALL_INTERFACE:include>
)

# Install the library target and specify public headers
install(TARGETS QtManager
    LIBRARY DESTINATION lib
    PUBLIC_HEADER DESTINATION QtManager
)
