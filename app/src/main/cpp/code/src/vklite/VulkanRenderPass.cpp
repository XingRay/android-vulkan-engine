//
// Created by leixing on 2024/12/26.
//

#include "VulkanRenderPass.h"
#include "vklite/VulkanUtil.h"

namespace vklite {

    VulkanRenderPass::VulkanRenderPass(const VulkanDevice &vulkanDevice, const VulkanSwapchain &vulkanSwapchain) : mDevice(vulkanDevice) {
        bool enableMsaa = true;
        bool enableDepth = true;

        std::vector<vk::AttachmentDescription> attachments;
        uint32_t index = 0;

        std::vector<vk::AttachmentReference> presentColorAttachmentReferences;
        std::vector<vk::AttachmentReference> depthAttachmentReferences;
        std::vector<vk::AttachmentReference> msaaColorAttachmentReferences;

        LOG_D("vulkanDevice.getMsaaSamples():%s", VulkanUtil::sampleCountFlagsToString(vulkanDevice.getMsaaSamples()).c_str());

        if (enableMsaa) {
            vk::AttachmentDescription msaaColorAttachmentDescription{};
            msaaColorAttachmentDescription
                    .setFormat(vulkanSwapchain.getDisplayFormat())
                    .setSamples(vulkanDevice.getMsaaSamples())
                            //载入图像前将帧缓冲清0
                    .setLoadOp(vk::AttachmentLoadOp::eClear)
                            // 渲染图像之后将图像数据保存
                    .setStoreOp(vk::AttachmentStoreOp::eStore)
                            // 模版缓冲, 这里不关注
                    .setStencilLoadOp(vk::AttachmentLoadOp::eDontCare)
                    .setStencilStoreOp(vk::AttachmentStoreOp::eDontCare)
                            // 常见的布局
                            //
                            //VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL：用作彩色附件的图像
                            //VK_IMAGE_LAYOUT_PRESENT_SRC_KHR：要在交换链中呈现的图像
                            //VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL：用作内存复制操作目标的图像
                            //
                            // initialLayout 渲染通道开始之前图像将具有的布局
                            // finalLayout 渲染通道完成时自动转换到的布局
                            //
                            // 使用 VK_IMAGE_LAYOUT_UNDEFINED 意味着我们不关心图像以前的布局
                            // 这个特殊值的警告是图像的内容不能保证被保留，但这并不重要，因为我们无论如何要清除
                    .setInitialLayout(vk::ImageLayout::eUndefined)
                    .setFinalLayout(vk::ImageLayout::eColorAttachmentOptimal);
            attachments.push_back(msaaColorAttachmentDescription);

            vk::AttachmentReference msaaColorAttachmentReference{};
            msaaColorAttachmentReference
                    .setAttachment(index)
                    .setLayout(vk::ImageLayout::eColorAttachmentOptimal);

            msaaColorAttachmentReferences.push_back(msaaColorAttachmentReference);

            index++;
        }

        if (enableDepth) {
            vk::SampleCountFlagBits sampleCountFlagBits = vk::SampleCountFlagBits::e1;
            if (enableMsaa) {
                sampleCountFlagBits = vulkanDevice.getMsaaSamples();
            }
            vk::AttachmentDescription depthAttachmentDescription{};
            depthAttachmentDescription
                    .setFormat(findDepthFormat(vulkanDevice.getPhysicalDevice()))
                    .setSamples(sampleCountFlagBits)
                    .setLoadOp(vk::AttachmentLoadOp::eClear)
                    .setStoreOp(vk::AttachmentStoreOp::eDontCare)
                    .setStencilLoadOp(vk::AttachmentLoadOp::eDontCare)
                    .setStencilStoreOp(vk::AttachmentStoreOp::eDontCare)
                    .setInitialLayout(vk::ImageLayout::eUndefined)
                    .setFinalLayout(vk::ImageLayout::eDepthStencilAttachmentOptimal);
            attachments.push_back(depthAttachmentDescription);

            vk::AttachmentReference depthAttachmentReference{};
            depthAttachmentReference
                    .setAttachment(index)
                    .setLayout(vk::ImageLayout::eDepthStencilAttachmentOptimal);

            depthAttachmentReferences.push_back(depthAttachmentReference);

            index++;
        }

        vk::AttachmentDescription presentColorAttachmentDescription{};
        presentColorAttachmentDescription
                .setFormat(vulkanSwapchain.getDisplayFormat())
                .setSamples(vk::SampleCountFlagBits::e1)
                .setLoadOp(vk::AttachmentLoadOp::eDontCare)
                .setStoreOp(vk::AttachmentStoreOp::eStore)
                .setStencilLoadOp(vk::AttachmentLoadOp::eDontCare)
                .setStencilStoreOp(vk::AttachmentStoreOp::eDontCare)
                .setInitialLayout(vk::ImageLayout::eUndefined)
                .setFinalLayout(vk::ImageLayout::ePresentSrcKHR);
        attachments.push_back(presentColorAttachmentDescription);

        vk::AttachmentReference presentColorAttachmentReference{};
        presentColorAttachmentReference
                .setAttachment(index)
                .setLayout(vk::ImageLayout::eColorAttachmentOptimal);
        presentColorAttachmentReferences.push_back(presentColorAttachmentReference);

        index++;


        vk::SubpassDescription subPassDescription{};
        subPassDescription.setPipelineBindPoint(vk::PipelineBindPoint::eGraphics);
        if (enableMsaa) {
            subPassDescription
                    .setColorAttachments(msaaColorAttachmentReferences)
                    .setResolveAttachments(presentColorAttachmentReferences);
        } else {
            subPassDescription
                    .setColorAttachments(presentColorAttachmentReferences);

        }
        if (enableDepth) {
            subPassDescription
                    .setPDepthStencilAttachment(&depthAttachmentReferences[0]);
        }

        std::array<vk::SubpassDescription, 1> subPassDescriptions{subPassDescription};

//        std::vector<vk::SubpassDependency> subPassDependencies;
//
//        vk::SubpassDependency startingPass{};
//        startingPass
//                .setSrcSubpass(vk::SubpassExternal)
//                .setDstSubpass(0)
//                .setSrcStageMask(vk::PipelineStageFlagBits::eBottomOfPipe)
//                .setDstStageMask(vk::PipelineStageFlagBits::eColorAttachmentOutput | vk::PipelineStageFlagBits::eEarlyFragmentTests)
//                .setSrcAccessMask(vk::AccessFlagBits::eMemoryRead)
//                .setDstAccessMask(vk::AccessFlagBits::eColorAttachmentWrite | vk::AccessFlagBits::eDepthStencilAttachmentWrite)
//                .setDependencyFlags(vk::DependencyFlagBits::eByRegion);
//        subPassDependencies.push_back(startingPass);
//
//        vk::SubpassDependency endingPass;
//        endingPass
//                .setSrcSubpass(0)
//                .setDstSubpass(vk::SubpassExternal)
//                .setSrcStageMask(vk::PipelineStageFlagBits::eColorAttachmentOutput | vk::PipelineStageFlagBits::eEarlyFragmentTests)
//                .setDstStageMask(vk::PipelineStageFlagBits::eBottomOfPipe)
//                .setSrcAccessMask(vk::AccessFlagBits::eColorAttachmentWrite | vk::AccessFlagBits::eDepthStencilAttachmentWrite)
//                .setDstAccessMask(vk::AccessFlagBits::eMemoryRead)
//                .setDependencyFlags(vk::DependencyFlagBits::eByRegion);
//        subPassDependencies.push_back(endingPass);

        vk::SubpassDependency subPassDependency = vk::SubpassDependency{}
                .setSrcSubpass(vk::SubpassExternal)
                .setDstSubpass(0)
                .setSrcStageMask(vk::PipelineStageFlagBits::eColorAttachmentOutput | vk::PipelineStageFlagBits::eEarlyFragmentTests)
                .setSrcAccessMask(vk::AccessFlagBits::eNone)
                .setDstStageMask(vk::PipelineStageFlagBits::eColorAttachmentOutput | vk::PipelineStageFlagBits::eEarlyFragmentTests)
                .setDstAccessMask(vk::AccessFlagBits::eColorAttachmentWrite | vk::AccessFlagBits::eDepthStencilAttachmentWrite);
        std::array<vk::SubpassDependency, 1> subPassDependencies = {subPassDependency};

        vk::RenderPassCreateInfo renderPassCreateInfo{};
        renderPassCreateInfo
                .setFlags(vk::RenderPassCreateFlags{})
                .setAttachments(attachments)
                .setSubpasses(subPassDescriptions)
                .setDependencies(subPassDependencies);

        mRenderPass = vulkanDevice.getDevice().createRenderPass(renderPassCreateInfo);
    }

    VulkanRenderPass::~VulkanRenderPass() {
        LOG_D("VulkanRenderPass::~VulkanRenderPass");
        mDevice.getDevice().destroy(mRenderPass);
    }

    const vk::RenderPass &VulkanRenderPass::getRenderPass() const {
        return mRenderPass;
    }

    vk::Format VulkanRenderPass::findDepthFormat(const vk::PhysicalDevice &physicalDevice) {
        return findSupportedFormat(
                physicalDevice,
                {vk::Format::eD32Sfloat, vk::Format::eD32SfloatS8Uint, vk::Format::eD24UnormS8Uint},
                vk::ImageTiling::eOptimal,
                vk::FormatFeatureFlagBits::eDepthStencilAttachment
        );
    }

    vk::Format VulkanRenderPass::findSupportedFormat(const vk::PhysicalDevice &physicalDevice, const std::vector<vk::Format> &candidates, vk::ImageTiling tiling, vk::FormatFeatureFlags features) {
        for (const auto &format: candidates) {
            vk::FormatProperties properties = physicalDevice.getFormatProperties(format);
            if (tiling == vk::ImageTiling::eLinear && (properties.linearTilingFeatures & features) == features) {
                return format;
            } else if (tiling == vk::ImageTiling::eOptimal && (properties.optimalTilingFeatures & features) == features) {
                return format;
            }
        }

        throw std::runtime_error("failed to find supported format !");
    }
}