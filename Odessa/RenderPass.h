#pragma once
class RenderPass
{
public:
	RenderPass(ID3D11Device* device, ID3D11DeviceContext* context);
	virtual ~RenderPass();

	virtual void Execute() = 0;
	virtual void RecordExecution() = 0;

protected:
	ID3D11Device* mDeviceRef;
	ID3D11DeviceContext* mDeviceContextRef;

	ID3D11CommandList* mCmdList;
	ID3D11DeviceContext* mContext;
};

