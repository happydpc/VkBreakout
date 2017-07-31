#include "Mesh.h"
#include "Renderer.h"

Mesh::Mesh(const std::vector<Vertex>& verts, Renderer* renderer)
	: rc(&renderer->GetVkContext())
{
	auto bufferSize = sizeof(Vertex) * verts.size();
	vertCount = verts.size();
	vkh::CreateBuffer(vBuffer,
		vBufferMemory,
		bufferSize,
		VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
		rc->lDevice.device,
		rc->gpu.device);

	void* data;
	vkMapMemory(rc->lDevice.device, vBufferMemory, 0, bufferSize, 0, &data);
	memcpy(data, verts.data(), bufferSize);
	vkUnmapMemory(rc->lDevice.device, vBufferMemory);

}

Mesh::Mesh(const std::vector<Vertex>& verts, const std::vector<UINT16> indices, class Renderer* renderer)
	: rc(&renderer->GetVkContext())
{
	//copy vert data
	auto bufferSize = sizeof(Vertex) * verts.size();
	vertCount = verts.size();
	vkh::CreateBuffer(vBuffer,
		vBufferMemory,
		bufferSize,
		VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
		rc->lDevice.device,
		rc->gpu.device);

	void* data;
	vkMapMemory(rc->lDevice.device, vBufferMemory, 0, bufferSize, 0, &data);
	memcpy(data, verts.data(), bufferSize);
	vkUnmapMemory(rc->lDevice.device, vBufferMemory);

	//copy index data
	auto indexBufSize = sizeof(UINT16) * indices.size();
	indexCount = indices.size();
	vkh::CreateBuffer(indexBuffer,
		indexBufferMemory,
		indexBufSize,
		VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
		rc->lDevice.device,
		rc->gpu.device);

	void* idata;
	vkMapMemory(rc->lDevice.device, indexBufferMemory, 0, indexBufSize, 0, &idata);
	memcpy(idata, indices.data(), indexBufSize);
	vkUnmapMemory(rc->lDevice.device, indexBufferMemory);
}

Mesh::~Mesh()
{
	vkDestroyBuffer(rc->lDevice.device, vBuffer, nullptr);
	vkFreeMemory(rc->lDevice.device, vBufferMemory, nullptr);

	vkDestroyBuffer(rc->lDevice.device, indexBuffer, nullptr);
	vkFreeMemory(rc->lDevice.device, indexBufferMemory, nullptr);


}

VkPipelineVertexInputStateCreateInfo DefaultVertexInputStateCreateInfo()
{
	VkPipelineVertexInputStateCreateInfo vertexInputInfo = {};
	vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;

	auto bindingDescription = Mesh::Vertex::getBindingDescription();
	auto attributeDescriptions = Mesh::Vertex::getAttributeDescriptions();

	vertexInputInfo.vertexBindingDescriptionCount = 1;
	vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
	vertexInputInfo.pVertexBindingDescriptions = &bindingDescription;
	vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();

	vertexInputInfo.pVertexBindingDescriptions = &bindingDescription; // Optional
	vertexInputInfo.vertexAttributeDescriptionCount = vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
	vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data(); // Optional
	return vertexInputInfo;
}