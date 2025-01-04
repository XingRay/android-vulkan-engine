//
// Created by leixing on 2025/1/5.
//

#include "FileUtil.h"


std::vector<char> FileUtil::loadFile(AAssetManager *assetManager, const char *filePath) {
    // 打开文件
    AAsset *file = AAssetManager_open(assetManager, filePath, AASSET_MODE_BUFFER);
    if (!file) {
        throw std::runtime_error("Failed to open file: " + std::string(filePath));
    }

    // 获取文件大小
    size_t fileLength = AAsset_getLength(file);

    // 读取文件内容
    std::vector<char> fileContent(fileLength); // 自动管理内存
    AAsset_read(file, fileContent.data(), fileLength);

    // 关闭文件
    AAsset_close(file);

    // 返回文件内容和大小
    return fileContent;
}