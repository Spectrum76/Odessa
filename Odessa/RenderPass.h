#pragma once
class RenderPass
{
public:
	RenderPass(VkDevice device, std::vector<VkImageView>* pImageViews);
	~RenderPass();

	void Initialize();

	VkRenderPass GetRenderPass();
	std::vector<VkFramebuffer>* GetFramebuffer();

protected:
	void CreateRenderPass();
	void CreateFramebuffer();
	void CreatePipelineLayout();
	void CreatePipeline();

private:
	VkRenderPass  mRenderPass;
	
	std::vector<VkFramebuffer> mFramebuffer;

	const VkDevice mDeviceRef;

	VkPipelineLayout mPipelineLayout;
	VkPipeline       mPipeline;

	std::vector<VkImageView>* mSwapChainImageViews;
};

