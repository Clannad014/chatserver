
#设置ui和qrc文件的自动编译选项
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#搜索所需要qt模块
find_package(Qt5 COMPONENTS Widgets Core Gui Network REQUIRED)

#添加资源文件
set(QRC_SOURCE_FILES ${PROJECT_SOURCE_DIR}/qrc/pic.qrc)
qt5_add_resources(QRC_FILES ${QRC_SOURCE_FILES})

#获取头文件、源文件、ui文件
file(GLOB_RECURSE SOURCE_FILES "./*.cpp")
file(GLOB_RECURSE HEADER_FILES "${PROJECT_SOURCE_DIR}/include/client/*.h")
file(GLOB_RECURSE UI_FILES "${PROJECT_SOURCE_DIR}/ui/*.ui")

#设置ui文件自动编译的搜索路径
set(CMAKE_AUTOUIC_SEARCH_PATHS ${PROJECT_SOURCE_DIR}/ui)

#添加头文件
include_directories(${PROJECT_SOURCE_DIR}/include/client)

#添加第三方静态库
add_library(
    libcomponents
    STATIC
    IMPORTED
)
set_target_properties(
    libcomponents
    PROPERTIES IMPORTED_LOCATION
    ${CMAKE_SOURCE_DIR}/libs/libcomponents.a
)

#指定生成可执行文件
add_executable(ChatClient ${SOURCE_FILES} ${QRC_FILES} ${UI_FILES} ${HEADER_FILES})

#链接qt模块和静态库
target_link_libraries(ChatClient libcomponents) #该静态库依赖于Widgets模块，必须在该模块链接之前链接，否则会报错
target_link_libraries(ChatClient Qt5::Widgets Qt5::Core Qt5::Gui Qt5::Network)



