//
// Created by leixing on 2025/1/5.
//

#ifndef ANDROID_VULKAN_ENGINE_FILEUTIL_H
#define ANDROID_VULKAN_ENGINE_FILEUTIL_H

#include <vector>
#include <android/asset_manager.h>

class FileUtil {
public:
    static std::vector<char> loadFile(AAssetManager *assetManager, const char *filePath);
};


#endif //ANDROID_VULKAN_ENGINE_FILEUTIL_H
