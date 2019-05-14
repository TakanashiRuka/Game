#include "..\draw\draw_indexed_instanced.h"

#include "..\..\framework\entity.h"
#include "..\..\framework\data.h"
#include "..\..\graphics\directx11.h"
#include "..\..\model\model_data.h"

DrawIndexedInstanced::DrawIndexedInstanced(Entity* parent, unsigned int mesh_size) : Component(parent), mesh_size_(mesh_size)
{

}

void DrawIndexedInstanced::Draw(Graphics* graphics, ID3D11Buffer* constant_buffer, std::vector<ID3D11Buffer*> vertex_buffers, std::vector<ID3D11Buffer*> vertex_buffers_instance, std::vector<ID3D11Buffer*> index_buffers, std::vector<unsigned int> index_size, unsigned int instance_num)
{
	UINT stride = sizeof(Vertex);
	UINT stride_i = sizeof(VertexBufferInstance);
	UINT offset = 0;

	graphics->device_context_->VSSetConstantBuffers(0, 1, &constant_buffer);
	graphics->device_context_->PSSetConstantBuffers(0, 1, &constant_buffer);
	graphics->device_context_->IASetVertexBuffers(1, 1, &vertex_buffers_instance[0], &stride_i, &offset);
	for (unsigned int size = 0; size < this->mesh_size_; ++size)
	{
		graphics->device_context_->IASetVertexBuffers(0, 1, &vertex_buffers[size], &stride, &offset);
		graphics->device_context_->IASetIndexBuffer(index_buffers[size], DXGI_FORMAT_R32_UINT, 0);
		graphics->device_context_->DrawIndexedInstanced(index_size[size], instance_num, 0, 0, 0);
	}
	ID3D11Buffer* null = nullptr;
	graphics->device_context_->IASetVertexBuffers(1, 1, &null, &stride_i, &offset);
}