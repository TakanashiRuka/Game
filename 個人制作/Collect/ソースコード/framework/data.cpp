#include "..\framework\data.h"

#include "..\framework\game.h"
#include "..\framework\utility.h"
#include "..\graphics\directx11.h"
#include "..\DirectXTK\WICTextureLoader.h"

#pragma comment(lib, "d3dCompiler.lib")

#if defined(DEBUG) || defined(_DEBUG)
constexpr DWORD SHADER_FLAGS = D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_DEBUG;
#else
constexpr DWORD SHADER_FLAGS = D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_OPTIMIZATION_LEVEL3;
#endif

constexpr char* model_file_pass = "data/model/";
constexpr char* texture_file_pass = "data/texture/";
constexpr char* hlsl_file_pass = "data/hlsl/";
constexpr char* hlsl_file_format = ".hlsl";

using namespace DirectX;

Data::Data()
{
	this->graphics_ = Game::GetInstance()->GetGraphics();
}

Data::~Data()
{
	for (auto& vertex_shader : this->vertex_shader_)
		SafeRelease(vertex_shader.second);

	for (auto& input_layout : this->input_layout_)
		SafeRelease(input_layout.second);

	for (auto& pixel_shader : this->pixel_shader_)
		SafeRelease(pixel_shader.second);

	for (auto& geometry_shader : this->geometry_shader_)
		SafeRelease(geometry_shader.second);

	for (auto& vertex_buffer : this->vertex_buffers_)
	{
		for (auto& vb : vertex_buffer.second)
			SafeRelease(vb);
	}

	for (auto& index_buffer : this->index_buffers_)
	{
		for (auto& ib : index_buffer.second)
			SafeRelease(ib);
	}

	for (auto& sampler_state : this->sampler_state_)
		SafeRelease(sampler_state.second);

	for (auto& texture : this->texture_)
		SafeRelease(texture.second);
}

void Data::CreateVertexShaderAndInputLayout(std::string name, InputLayoutType type)
{
	if (!this->vertex_shader_[name])
	{
		ID3DBlob* compiled_shader = nullptr;
		ID3DBlob* errors = nullptr;

		std::string file = hlsl_file_pass + name + hlsl_file_format;
		D3DCompileFromFile(std::wstring(file.begin(), file.end()).c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "VS", "vs_5_0", SHADER_FLAGS, 0, &compiled_shader, &errors);
		this->graphics_->device_->CreateVertexShader(compiled_shader->GetBufferPointer(), compiled_shader->GetBufferSize(), nullptr, &this->vertex_shader_[name]);

		if (!this->input_layout_[type])
		{
			switch (type)
			{
			case InputLayoutType::Simple:
			{
				D3D11_INPUT_ELEMENT_DESC ied[] =
				{
					{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
					{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
					{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
				};
				UINT num_elements = sizeof(ied) / sizeof(ied[0]);
				this->graphics_->device_->CreateInputLayout(ied, num_elements, compiled_shader->GetBufferPointer(), compiled_shader->GetBufferSize(), &this->input_layout_[type]);
			}
			break;

			case InputLayoutType::SimpleInstance:
			{
				D3D11_INPUT_ELEMENT_DESC ied[] =
				{
					{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
					{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
					{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
					{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 0, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
					{ "MATRIX", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
					{ "MATRIX", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
					{ "MATRIX", 2, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
					{ "MATRIX", 3, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
				};
				UINT num_elements = sizeof(ied) / sizeof(ied[0]);
				this->graphics_->device_->CreateInputLayout(ied, num_elements, compiled_shader->GetBufferPointer(), compiled_shader->GetBufferSize(), &this->input_layout_[type]);
			}
			break;

			case InputLayoutType::SkinMeshAnimation:
			{
				D3D11_INPUT_ELEMENT_DESC ied[] =
				{
					{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
					{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
					{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
					{ "INDEX", 0, DXGI_FORMAT_R32G32B32A32_UINT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
					{ "WEIGHT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
				};
				UINT num_elements = sizeof(ied) / sizeof(ied[0]);
				this->graphics_->device_->CreateInputLayout(ied, num_elements, compiled_shader->GetBufferPointer(), compiled_shader->GetBufferSize(), &this->input_layout_[type]);
			}
			break;
			}
		}
	}
}

void Data::CreatePixelShader(std::string name)
{
	if (!this->pixel_shader_[name])
	{
		ID3DBlob* compiled_shader = nullptr;
		ID3DBlob* errors = nullptr;

		std::string file = hlsl_file_pass + name + hlsl_file_format;
		D3DCompileFromFile(std::wstring(file.begin(), file.end()).c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "PS", "ps_5_0", SHADER_FLAGS, 0, &compiled_shader, &errors);
		this->graphics_->device_->CreatePixelShader(compiled_shader->GetBufferPointer(), compiled_shader->GetBufferSize(), nullptr, &this->pixel_shader_[name]);
	}
}

void Data::CreateGeometryShader(std::string name)
{
	if (!this->geometry_shader_[name])
	{
		ID3DBlob* compiled_shader = nullptr;
		ID3DBlob* errors = nullptr;

		std::string file = hlsl_file_pass + name + hlsl_file_format;
		D3DCompileFromFile(std::wstring(file.begin(), file.end()).c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "GS", "gs_5_0", SHADER_FLAGS, 0, &compiled_shader, &errors);
		this->graphics_->device_->CreateGeometryShader(compiled_shader->GetBufferPointer(), compiled_shader->GetBufferSize(), nullptr, &this->geometry_shader_[name]);
	}
}

void Data::CreateConstantBuffer(ID3D11Buffer*& constant_buffer, ConstantBufferType type)
{
	D3D11_BUFFER_DESC bd;
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

	switch (type)
	{
	case ConstantBufferType::Simple:
	{
		bd.ByteWidth = sizeof(CBufferSimple);
	}
	break;

	case ConstantBufferType::SimpleShadowAndOutline:
	{
		bd.ByteWidth = sizeof(CBufferSimpleShadowAndOutline);
	}
	break;

	case ConstantBufferType::BumpMapping:
	{
		bd.ByteWidth = sizeof(CBufferBumpMapping);
	}
	break;

	case ConstantBufferType::Animation:
	{
		bd.ByteWidth = sizeof(CBufferAnimation);
	}
	break;

	case ConstantBufferType::AnimationShadow:
	{
		bd.ByteWidth = sizeof(CBufferAnimationShadow);
	}
	break;

	case ConstantBufferType::Color:
	{
		bd.ByteWidth = sizeof(CBufferColor);
	}
	break;

	case ConstantBufferType::Camera:
	{
		bd.ByteWidth = sizeof(CBufferCamera);
	}
	break;

	case ConstantBufferType::Light:
	{
		bd.ByteWidth = sizeof(CBufferLight);
	}
	break;
	}

	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	bd.StructureByteStride = 0;
	bd.Usage = D3D11_USAGE_DEFAULT;
	this->graphics_->device_->CreateBuffer(&bd, nullptr, &constant_buffer);
}

void Data::CreateConstantBuffers(std::vector<ID3D11Buffer*>& constant_buffers, ConstantBufferType type, unsigned int size)
{
	constant_buffers.resize(size);
	for (auto& constant_buffer : constant_buffers)
	{
		D3D11_BUFFER_DESC bd;
		bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

		switch (type)
		{
		case ConstantBufferType::Simple:
		{
			bd.ByteWidth = sizeof(CBufferSimple);
		}
		break;

		case ConstantBufferType::SimpleShadowAndOutline:
		{
			bd.ByteWidth = sizeof(CBufferSimpleShadowAndOutline);
		}
		break;

		case ConstantBufferType::Animation:
		{
			bd.ByteWidth = sizeof(CBufferAnimation);
		}
		break;

		case ConstantBufferType::AnimationShadow:
		{
			bd.ByteWidth = sizeof(CBufferAnimationShadow);
		}
		break;

		case ConstantBufferType::Color:
		{
			bd.ByteWidth = sizeof(CBufferColor);
		}
		break;

		case ConstantBufferType::Camera:
		{
			bd.ByteWidth = sizeof(CBufferCamera);
		}
		break;

		case ConstantBufferType::Light:
		{
			bd.ByteWidth = sizeof(CBufferLight);
		}
		break;
		}

		bd.CPUAccessFlags = 0;
		bd.MiscFlags = 0;
		bd.StructureByteStride = 0;
		bd.Usage = D3D11_USAGE_DEFAULT;
		this->graphics_->device_->CreateBuffer(&bd, nullptr, &constant_buffer);
	}
}

void Data::CreateVertexBuffers(std::string name, VertexBufferType type, Model& model)
{
	if (!this->vertex_buffers_[name].size())
	{
		D3D11_BUFFER_DESC bd;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = 0;
		bd.MiscFlags = 0;
		bd.StructureByteStride = 0;
		bd.Usage = D3D11_USAGE_DEFAULT;

		ID3D11Buffer* vertex_buffer = nullptr;

		for (auto& mesh : model.meshes)
		{
			bd.ByteWidth = sizeof(Vertex) * mesh.vertices.size();

			switch (type)
			{
			case VertexBufferType::MeshField:
			case VertexBufferType::Model:
			{
				D3D11_SUBRESOURCE_DATA sd;
				sd.pSysMem = mesh.vertices.data();
				sd.SysMemPitch = 0;
				sd.SysMemSlicePitch = 0;
				this->graphics_->device_->CreateBuffer(&bd, &sd, &vertex_buffer);

				this->vertex_buffers_[name].emplace_back(vertex_buffer);
			}
			break;

			case VertexBufferType::UI:
			{
				this->graphics_->device_->CreateBuffer(&bd, nullptr, &vertex_buffer);

				this->vertex_buffers_[name].emplace_back(vertex_buffer);
			}
			break;
			}
		}

		switch (type)
		{
		case VertexBufferType::UI:
		{
			this->UpdateVertexBuffers(name, model);
		}
		break;
		}
	}
}

void Data::CreateVertexBufferInstanced(std::string name, void* data, unsigned int instance_num)
{
	if (!this->vertex_buffers_[name].size())
	{
		D3D11_BUFFER_DESC bd;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = 0;
		bd.MiscFlags = 0;
		bd.StructureByteStride = 0;
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(VertexBufferInstance) * instance_num;

		D3D11_SUBRESOURCE_DATA sd;
		sd.pSysMem = data;
		sd.SysMemPitch = 0;
		sd.SysMemSlicePitch = 0;

		ID3D11Buffer* vertex_buffer = nullptr;
		this->graphics_->device_->CreateBuffer(&bd, &sd, &vertex_buffer);
		this->vertex_buffers_[name].emplace_back(vertex_buffer);
	}
}

void Data::CreateIndexBuffers(std::string name, Model& model)
{
	if (!this->index_buffers_[name].size())
	{
		D3D11_BUFFER_DESC bd;
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;

		D3D11_SUBRESOURCE_DATA sd;

		ID3D11Buffer* index_buffer = nullptr;

		for (auto mesh : model.meshes)
		{
			bd.ByteWidth = sizeof(unsigned int) * mesh.indices.size();
			this->index_size_[name].emplace_back(mesh.indices.size());
			bd.CPUAccessFlags = 0;
			bd.MiscFlags = D3D11_RESOURCE_MISC_DRAWINDIRECT_ARGS;
			bd.StructureByteStride = 0;
			bd.Usage = D3D11_USAGE_DEFAULT;

			sd.pSysMem = mesh.indices.data();
			sd.SysMemPitch = 0;
			sd.SysMemSlicePitch = 0;
			this->graphics_->device_->CreateBuffer(&bd, &sd, &index_buffer);

			this->index_buffers_[name].emplace_back(index_buffer);
		}
	}
}

void Data::CreateSamplerState(SamplerStateType type)
{
	if (!this->sampler_state_[type])
	{
		D3D11_SAMPLER_DESC sd = {};

		switch (type)
		{
		case SamplerStateType::Simple:
		{
			sd.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
			sd.AddressU = sd.AddressV = sd.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		}
		break;

		case SamplerStateType::Shadow:
		{
			sd.Filter = D3D11_FILTER_COMPARISON_MIN_MAG_LINEAR_MIP_POINT;
			sd.AddressU = sd.AddressV = sd.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
			sd.ComparisonFunc = D3D11_COMPARISON_LESS;
		}
		break;
		}

		this->graphics_->device_->CreateSamplerState(&sd, &this->sampler_state_[type]);
	}
}

void Data::CreateTexture(Texture texture)
{
	if (!this->texture_[texture])
	{
		std::string file_path = texture_path[texture];
		CreateWICTextureFromFile(this->graphics_->device_, std::wstring(file_path.begin(), file_path.end()).c_str(), nullptr, &this->texture_[texture]);
	}
}

void Data::CreateAnimation(std::string name, Model& model)
{
	BoneTransform bone_transform;
	bone_transform.mesh_bones.resize(model.meshes.size());
	for (unsigned int m = 0; m < model.meshes.size(); ++m)
	{
		bone_transform.mesh_bones[m].bones_map = model.meshes[m].bones_map;
		bone_transform.mesh_bones[m].bones_offset = model.meshes[m].bones_offset;
	}
	bone_transform.root_node_name = model.root_node_name;
	bone_transform.root_node = model.root_node;
	bone_transform.animations = model.animations;

	this->bone_transform_.emplace(name, bone_transform);
}

ID3D11VertexShader* const Data::GetVertexShader(std::string name)
{
	return this->vertex_shader_[name];
}

ID3D11InputLayout* const Data::GetInputLayout(InputLayoutType type)
{
	return this->input_layout_[type];
}

ID3D11PixelShader* const Data::GetPixelShader(std::string name)
{
	return this->pixel_shader_[name];
}

ID3D11GeometryShader* const Data::GetGeometryShader(std::string name)
{
	return this->geometry_shader_[name];
}

std::vector<ID3D11Buffer*> const Data::GetVertexBuffers(std::string name)
{
	return this->vertex_buffers_[name];
}

std::vector<ID3D11Buffer*> const Data::GetIndexBuffers(std::string name)
{
	return this->index_buffers_[name];
}

std::vector<unsigned int> const Data::GetIndexSize(std::string name)
{
	return this->index_size_[name];
}

ID3D11SamplerState** const Data::GetSamplerState(SamplerStateType type)
{
	return &this->sampler_state_[type];
}

ID3D11ShaderResourceView** const Data::GetTexture(Texture texture)
{
	return &this->texture_[texture];
}

void Data::UpdateConstantBuffer(ID3D11Buffer* constant_buffer, const void* data)
{
	this->graphics_->device_context_->UpdateSubresource(constant_buffer, 0, nullptr, data, 0, 0);
}

void Data::UpdateVertexBuffers(std::string name, Model& model)
{
	unsigned int ms = model.meshes.size();
	for (unsigned int m = 0; m < ms; ++m)
	{
		this->graphics_->device_context_->UpdateSubresource(this->vertex_buffers_[name][m], 0, nullptr, model.meshes[m].vertices.data(), 0, 0);
	}
}

void Data::UpdateVertexBufferInstanced(std::string name, void* data)
{
	this->graphics_->device_context_->UpdateSubresource(this->vertex_buffers_[name][0], 0, nullptr, data, 0, 0);
}

void Data::UpdateAnimation(std::string name, std::string animations_key, double& animation_running_time, XMMATRIX(&bones)[bone_max], unsigned int& size)
{
	auto& bone_transform = this->bone_transform_[name];
	auto& animation = bone_transform.animations[animations_key];

	XMMATRIX identity = XMMatrixIdentity();
	double time = fmod(animation_running_time * animation.ticks_per_second, animation.duration);
	if (animation_running_time * animation.ticks_per_second > animation.duration)
		animation_running_time = 0;
	this->ReadNode(
		bones,
		animation,
		bone_transform,
		bone_transform.root_node_name.data(),
		bone_transform.root_node,
		bone_transform.mesh_bones[size],
		time,
		identity);
}

void Data::ReadNode(XMMATRIX(&bones)[bone_max], Animation& animation, BoneTransform& bone_transform, const char* node_name, Node& node, MeshBones& mesh_bones, double& time, XMMATRIX& parent)
{

	XMMATRIX node_transformation = XMLoadFloat4x4(&node.transformation);

	int sub_channel_size = 2;

	if (animation.channels.count(node_name))
	{
		auto& channel = animation.channels[node_name];

		DirectX::XMFLOAT3 scaling;
		if (channel.scaling.size() == 1)
		{
			scaling = channel.scaling[0].value;
		}
		else
		{
			unsigned int key = 0;
			for (; key < channel.scaling.size() - sub_channel_size; ++key)
				if (time < channel.scaling[key + 1].time)
					break;
			double delta_time = channel.scaling[key + 1].time - channel.scaling[key].time;
			float factor = static_cast<float>((time - channel.scaling[key].time) / delta_time);
			auto& start = channel.scaling[key].value;
			auto& end = channel.scaling[key + 1].value;
			scaling.x = start.x + factor * (end.x - start.x);
			scaling.y = start.y + factor * (end.y - start.y);
			scaling.z = start.z + factor * (end.z - start.z);
		}

		DirectX::XMFLOAT4 rotation;
		if (channel.rotation.size() == 1)
		{
			rotation = channel.rotation[0].value;
		}
		else
		{
			unsigned int key = 0;
			for (; key < channel.rotation.size() - sub_channel_size; ++key)
				if (time < channel.rotation[key + 1].time)
					break;
			double delta_time = channel.rotation[key + 1].time - channel.rotation[key].time;
			float factor = static_cast<float>((time - channel.rotation[key].time) / delta_time);
			auto& start = channel.rotation[key].value;
			auto& end = channel.rotation[key + 1].value;
			rotation.x = start.x + factor * (end.x - start.x);
			rotation.y = start.y + factor * (end.y - start.y);
			rotation.z = start.z + factor * (end.z - start.z);
			rotation.w = start.w + factor * (end.w - start.w);
		}

		DirectX::XMFLOAT3 position;
		if (channel.position.size() == 1)
		{
			position = channel.position[0].value;
		}
		else
		{
			unsigned int key = 0;
			for (; key < channel.position.size() - sub_channel_size; ++key)
				if (time < channel.position[key + 1].time)
					break;
			double delta_time = channel.position[key + 1].time - channel.position[key].time;
			float factor = static_cast<float>((time - channel.position[key].time) / delta_time);
			auto& start = channel.position[key].value;
			auto& end = channel.position[key + 1].value;
			position.x = start.x + factor * (end.x - start.x);
			position.y = start.y + factor * (end.y - start.y);
			position.z = start.z + factor * (end.z - start.z);
		}

		bone_transform.old_channel[node_name].position.value = position;
		bone_transform.old_channel[node_name].rotation.value = rotation;
		bone_transform.old_channel[node_name].scaling.value = scaling;

		node_transformation =
			XMMatrixScaling(scaling.x, scaling.y, scaling.z) *
			XMMatrixRotationQuaternion(XMVector4Normalize(XMVectorSet(rotation.x, rotation.y, rotation.z, rotation.w))) *
			XMMatrixTranslation(position.x, position.y, position.z);
	}

	XMMATRIX global_transformation = node_transformation * parent;

	if (mesh_bones.bones_map.count(node_name))
	{
		bones[mesh_bones.bones_map[node_name]] =
			XMLoadFloat4x4(&mesh_bones.bones_offset[mesh_bones.bones_map[node_name]]) *
			global_transformation;
	}

	for (auto& child : node.children)
		this->ReadNode(bones, animation, bone_transform, child.first.data(), child.second, mesh_bones, time, global_transformation);
}