#include "pch.h"
#include "CommandBuffer.h"
#include "Utilities.h"

CommandBuffer::CommandBuffer(VkDevice device, VkSurfaceKHR surface, VkPhysicalDevice GPU) :
	mDeviceRef(device), mSurfaceRef(surface), mGPURef(GPU),
	mRenderPassRef(VK_NULL_HANDLE), mFramebufferRef(nullptr)
{
	mCommandPool = VK_NULL_HANDLE;
}

CommandBuffer::~CommandBuffer()
{
	vkDestroyCommandPool(mDeviceRef, mCommandPool, nullptr);
}

void CommandBuffer::Initialize()
{
	CreateCommandPool();
	CreateCommandBuffer();
}

void CommandBuffer::Record(VkRenderPass RenderPass, std::vector<VkFramebuffer>* pFramebuffer)
{
	mRenderPassRef = RenderPass;
	mFramebufferRef = pFramebuffer;

	RecordCommandBuffer();
}

std::vector<VkCommandBuffer>* CommandBuffer::GetCommandBuffer()
{
	return &mCommandBuffers;
}

void CommandBuffer::CreateCommandPool()
{
	Utilities::QueueFamilyIndices queueFamilyIndices = Utilities::FindQueueFamilies(mGPURef, mSurfaceRef);

	VkCommandPoolCreateInfo poolInfo{};
	poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value();

	if (vkCreateCommandPool(mDeviceRef, &poolInfo, nullptr, &mCommandPool) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create command pool!");
	}
}

void CommandBuffer::CreateCommandBuffer()
{
	mCommandBuffers.resize(ImageCount);

	VkCommandBufferAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.commandPool = mCommandPool;
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandBufferCount = (uint32_t)mCommandBuffers.size();

	if (vkAllocateCommandBuffers(mDeviceRef, &allocInfo, mCommandBuffers.data()) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to allocate command buffers!");
	}
}

void CommandBuffer::RecordCommandBuffer()
{
	for (size_t i = 0; i < ImageCount; i++)
	{
		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

		if (vkBeginCommandBuffer(mCommandBuffers[i], &beginInfo) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to begin recording command buffer!");
		}

		VkRenderPassBeginInfo renderPassInfo{};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassInfo.renderPass = mRenderPassRef;
		renderPassInfo.framebuffer = (*mFramebufferRef)[i];
		renderPassInfo.renderArea.offset = { 0, 0 };
		renderPassInfo.renderArea.extent = VkExtent2D{ WIDTH, HEIGHT };

		VkClearValue clearColor = { 1.0f, 0.0f, 0.0f, 1.0f };
		renderPassInfo.clearValueCount = 1;
		renderPassInfo.pClearValues = &clearColor;

		vkCmdBeginRenderPass(mCommandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

		vkCmdEndRenderPass(mCommandBuffers[i]);

		if (vkEndCommandBuffer(mCommandBuffers[i]) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to record command buffer!");
		}
	}
}
