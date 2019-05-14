#include "..\entity\result_high_score.h"

#include "..\graphics\directx11.h"
#include "..\framework\data.h"
#include "..\model\model_import.h"
#include "..\component\draw\draw_indexed.h"

#include <fstream>

constexpr char* hlsl = "ui";
constexpr InputLayoutType input_layout = InputLayoutType::Simple;
constexpr SamplerStateType sampler_state = SamplerStateType::Simple;
constexpr Texture texture = Texture::T_Number;

constexpr char* save_file = "data/save/high_score.save";

ResultHighScore::ResultHighScore(Scene* parent) : Entity(parent)
{
	this->tag_ = "result high score";

	std::ifstream input_file(save_file, std::ios::in | std::ios::binary);
	if (input_file.is_open())
	{
		input_file.read(reinterpret_cast<char*>(&this->high_score_), sizeof(this->high_score_));
	}
	else
	{
		std::ofstream output_file(save_file, std::ios::out | std::ios::binary);
		this->high_score_ = 1000;
		output_file.write(reinterpret_cast<char*>(&this->high_score_), sizeof(this->high_score_));
	}
	auto model = ModelImport::Number(22 * 8, 0, 22 * 8, 32, this->high_score_, 8, false);

	this->data_->CreateVertexShaderAndInputLayout(hlsl, input_layout);
	this->data_->CreatePixelShader(hlsl);
	this->data_->CreateConstantBuffer(this->constant_buffer_, ConstantBufferType::Simple);
	this->data_->CreateVertexBuffers(this->tag_, VertexBufferType::UI, model);
	this->data_->CreateIndexBuffers(this->tag_, model);
	this->data_->CreateSamplerState(sampler_state);
	this->data_->CreateTexture(texture);

	this->AddComponent<DrawIndexed>(model.meshes.size());
}

ResultHighScore::~ResultHighScore()
{
	std::ofstream output_file(save_file, std::ios::out | std::ios::binary);
	output_file.write(reinterpret_cast<char*>(&this->high_score_), sizeof(this->high_score_));
}

void ResultHighScore::Update()
{
	auto model = ModelImport::Number(735, 253, 44 * 8, 64, this->high_score_, 8, false);
	this->data_->UpdateVertexBuffers(this->tag_, model);
}

void ResultHighScore::Draw(RenderingPass pass)
{
	const auto& dc = this->graphics_->device_context_;

	switch (pass)
	{
	case RenderingPass::UnableDepthStencil:
	{
		this->graphics_->SetBlendState(BlendState::AlphaToCoverage);

		dc->VSSetShader(this->data_->GetVertexShader(hlsl), nullptr, 0);
		dc->PSSetShader(this->data_->GetPixelShader(hlsl), nullptr, 0);
		dc->IASetInputLayout(this->data_->GetInputLayout(input_layout));
		dc->PSSetSamplers(0, 1, this->data_->GetSamplerState(sampler_state));
		dc->PSSetShaderResources(0, 1, this->data_->GetTexture(texture));
		dc->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
		this->GetComponent<DrawIndexed>()->Draw(this->graphics_, this->constant_buffer_, this->data_->GetVertexBuffers(this->tag_), this->data_->GetIndexBuffers(this->tag_), this->data_->GetIndexSize(this->tag_));

		this->graphics_->SetBlendState(BlendState::Default);
	}
	break;
	}
}

unsigned int& ResultHighScore::GetHighScore()
{
	return this->high_score_;
}