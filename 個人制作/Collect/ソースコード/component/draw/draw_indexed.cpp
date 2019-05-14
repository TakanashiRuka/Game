#include "..\..\component\draw\draw_indexed.h"

#include "..\..\framework\entity.h"
#include "..\..\graphics\directx11.h"
#include "..\..\model\model_data.h"

DrawIndexed::DrawIndexed(Entity* parent, unsigned int mesh_size) : Component(parent), mesh_size_(mesh_size)
{

}

void DrawIndexed::Draw(Graphics* graphics, ID3D11Buffer* constant_buffer, std::vector<ID3D11Buffer*> vertex_buffers, std::vector<ID3D11Buffer*> index_buffers, std::vector<unsigned int> index_size)
{
	UINT stride = sizeof(Vertex);
	UINT offset = 0;

	graphics->device_context_->VSSetConstantBuffers(0, 1, &constant_buffer);
	graphics->device_context_->PSSetConstantBuffers(0, 1, &constant_buffer);
	for (unsigned int size = 0; size < this->mesh_size_; ++size)
	{
		graphics->device_context_->IASetVertexBuffers(0, 1, &vertex_buffers[size], &stride, &offset);
		graphics->device_context_->IASetIndexBuffer(index_buffers[size], DXGI_FORMAT_R32_UINT, 0);
		graphics->device_context_->DrawIndexed(index_size[size], 0, 0);
	}
}

void DrawIndexed::DrawAnimation(Graphics* graphics, std::vector<ID3D11Buffer*> constant_buffers, std::vector<ID3D11Buffer*> vertex_buffers, std::vector<ID3D11Buffer*> index_buffers, std::vector<unsigned int> index_size)
{
	UINT stride = sizeof(Vertex);
	UINT offset = 0;

	for (unsigned int size = 0; size < this->mesh_size_; ++size)
	{
		graphics->device_context_->VSSetConstantBuffers(0, 1, &constant_buffers[size]);
		graphics->device_context_->PSSetConstantBuffers(0, 1, &constant_buffers[size]);
		graphics->device_context_->IASetVertexBuffers(0, 1, &vertex_buffers[size], &stride, &offset);
		graphics->device_context_->IASetIndexBuffer(index_buffers[size], DXGI_FORMAT_R32_UINT, 0);
		graphics->device_context_->DrawIndexed(index_size[size], 0, 0);
	}
}