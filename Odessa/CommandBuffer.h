#pragma once
class CommandBuffer
{
public:
	CommandBuffer(VkDevice device, VkSurfaceKHR surface, VkPhysicalDevice GPU);
	~CommandBuffer();

	void Initialize();
	void Record(VkRenderPass RenderPass, std::vector<VkFramebuffer>* pFramebuffer);

	std::vector<VkCommandBuffer>* GetCommandBuffer();

protected:
	void CreateCommandPool();
	void CreateCommandBuffer();
	void RecordCommandBuffer();

private:
	const VkDevice mDeviceRef;
	const VkSurfaceKHR mSurfaceRef;
	const VkPhysicalDevice mGPURef;

	VkRenderPass mRenderPassRef;
	std::vector<VkFramebuffer>* mFramebufferRef;

	VkCommandPool mCommandPool;
	std::vector<VkCommandBuffer> mCommandBuffers;
};

