cmake_minimum_required(VERSION 3.22.1)
project("vulkan_demo")
set(CMAKE_CXX_STANDARD 20)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror -DVK_USE_PLATFORM_ANDROID_KHR -std=c++20")

# glm
# https://github.com/g-truc/glm
set(GLM_INSTALL_DIR D:/develop/opengl/glm/glm-1.0.1-light)


# vulkan
# https://www.vulkan.org/
#set(VULKAN_INSTALL_DIR D:/develop/vulkan/VulkanSDK/1.3.296.0)
#set(VULKAN_INCLUDE_DIR ${VULKAN_INSTALL_DIR}/Include)

# Vulkan-Headers
# https://github.com/KhronosGroup/Vulkan-Headers
set(VULKAN_HEADERS_INSTALL_DIR D:/develop/vulkan/Vulkan-Headers/Vulkan-Headers-1.3.275)
set(VULKAN_HEADERS_INCLUDE_DIR ${VULKAN_HEADERS_INSTALL_DIR}/include)


# stb
set(STB_INSTALL_DIR D:/develop/opengl/stb)
set(STB_INCLUDE_DIR ${STB_INSTALL_DIR})

# tinyobjloader
set(TINYOBJLOADER_INSTALL_DIR D:/develop/opengl/tinyobjloader)
set(TINYOBJLOADER_INCLUE_DIR ${TINYOBJLOADER_INSTALL_DIR})

# Integrate GameActivity, refer to
#     https://developer.android.com/games/agdk/game-activity/get-started#static-library
# for the detailed instructions.
#find_package(game-activity REQUIRED CONFIG)

# ncnn
# https:/github.com/Tencent/ncnn
set(NCNN_INSTALL_DIR D:/develop/ncnn/master/install/android)
set(NCNN_INCLUDE ${NCNN_INSTALL_DIR}/arm64-v8a/include)
set(ncnn_DIR ${NCNN_INSTALL_DIR}/arm64-v8a/lib/cmake/ncnn)
find_package(ncnn REQUIRED)
link_directories("${NCNN_INSTALL_DIR}/arm64-v8a/lib")

# opencv
# https://github.com/opencv/opencv
set(OPENCV_INSTALL_DIR D:/develop/opencv/4.11.0/sdk/android/OpenCV-android-sdk)
set(OpenCV_DIR ${OPENCV_INSTALL_DIR}/sdk/native/jni)
find_package(OpenCV REQUIRED)
set(OPENCV_INCLUDE ${OPENCV_INSTALL_DIR}/sdk/native/jni/include)

# mnn
# https://github.com/alibaba/MNN
set(MNN_INSTALL_DIR D:/develop/mnn/master/install/android_arm64-v8a_vulkan)
set(MNN_INCLUDE ${MNN_INSTALL_DIR}/include)
set(MNN_LIB_DIR ${MNN_INSTALL_DIR}/lib)

link_directories(${MNN_LIB_DIR})

file(GLOB_RECURSE PROJECT_SOURCES
        "code/src/*.c"
        "code/src/*.cpp"
        "code/src/*.cc"
        "code/third_party/game_activity/*.c"
        "code/third_party/game_activity/*.cpp"
        "code/third_party/game_activity/*.cc"
)

add_library(${PROJECT_NAME} SHARED
        ${PROJECT_SOURCES}
)

target_include_directories(${PROJECT_NAME} PUBLIC
        "${LIB_MPTASK_DIR}/include"
        "code/include"
        "code/src"
        "code/src/ncv"
        "code/third_party/game_activity"

        # vulkan
        # ${VULKAN_INCLUDE_DIR}

        # Vulkan-Hpp
        ${VULKAN_HEADERS_INCLUDE_DIR}

        # glm
        ${GLM_INSTALL_DIR}

        # STB
        ${STB_INCLUDE_DIR}

        # tinyobjloader
        ${TINYOBJLOADER_INCLUE_DIR}

        # ncnn
        ${NCNN_INCLUDE}

        # OpenCV
        ${OPENCV_INCLUDE}

        # mnn
        ${MNN_INCLUDE}
)

target_link_libraries(${PROJECT_NAME}
        android
        camera2ndk
        mediandk
        log
        vulkan
        ncnn
        ${OpenCV_LIBS}
        ${MNN_LIB_DIR}/libMNN.so
        ${MNN_LIB_DIR}/libMNN_Express.so
)