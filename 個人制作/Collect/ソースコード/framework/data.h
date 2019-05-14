#pragma once

#include "..\model\model_data.h"
#include "..\framework\texture.h"

#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>

#include <vector>
#include <unordered_map>
#include <string>

constexpr unsigned int bone_max = 128;

enum class InputLayoutType
{
	Simple,
	SimpleInstance,
	SkinMeshAnimation,
};

enum class ConstantBufferType
{
	Simple,
	SimpleShadowAndOutline,
	BumpMapping,
	Animation,
	AnimationShadow,
	Color,
	Camera,
	Light,
};

enum class VertexBufferType
{
	MeshField,
	Model,
	UI,
};

enum class SamplerStateType
{
	Simple,
	Shadow,
};

struct VertexBufferInstance
{
	DirectX::XMFLOAT4 color;
	DirectX::XMMATRIX world;
};

struct CBufferSimple
{
	DirectX::XMMATRIX world;
};

struct CBufferSimpleShadowAndOutline
{
	DirectX::XMMATRIX world;
	DirectX::XMFLOAT4 texel_size = { 0, 0, 0, 0 };
	DirectX::XMFLOAT4 out_line_color = { 0, 0, 0, 0 };
	float border_width = 0;
};

struct CBufferBumpMapping
{
	DirectX::XMMATRIX world;
	DirectX::XMMATRIX wit;
};

struct CBufferAnimation
{
	DirectX::XMMATRIX world;
	DirectX::XMMATRIX bones[bone_max];
};

struct CBufferAnimationShadow
{
	DirectX::XMMATRIX world;
	DirectX::XMMATRIX bones[bone_max];
	DirectX::XMFLOAT4 texel_size = { 0, 0, 0, 0 };
};

struct CBufferColor
{
	DirectX::XMFLOAT4 color = { 1, 1, 1, 1 };
};

struct CBufferCamera
{
	DirectX::XMMATRIX view;
	DirectX::XMMATRIX projection;
	DirectX::XMMATRIX view_inverse;

	DirectX::XMFLOAT3 eye;
};

struct CBufferLight
{
	DirectX::XMMATRIX light_view;
	DirectX::XMMATRIX light_projection;

	DirectX::XMFLOAT4 light = { 0, 0, 0, 0 };
	DirectX::XMFLOAT4 ambient = { 0.4f, 0.4f, 0.4f, 1 };
	DirectX::XMFLOAT4 diffuse = { 0.7f, 0.7f, 0.7f, 1 };
	DirectX::XMFLOAT4 specular = { 0.5f, 0.5f, 0.5f, 1 };
};

struct MeshBones
{
	std::unordered_map<std::string, unsigned int> bones_map;
	std::vector<DirectX::XMFLOAT4X4> bones_offset;
};

struct OldChannel
{
	KeyVector3 scaling;
	KeyQuaternion rotation;
	KeyVector3 position;
};

struct BoneTransform
{
	std::vector<MeshBones> mesh_bones;
	std::string root_node_name;
	Node root_node;
	std::unordered_map<std::string, Animation> animations;
	std::string old_animations_key;
	std::unordered_map<std::string, OldChannel> old_channel;
};

class Graphics;

class Data
{
private:
	Graphics* graphics_ = nullptr;

	std::unordered_map<std::string, ID3D11VertexShader*> vertex_shader_;
	std::unordered_map<InputLayoutType, ID3D11InputLayout*> input_layout_;
	std::unordered_map<std::string, ID3D11PixelShader*> pixel_shader_;
	std::unordered_map<std::string, ID3D11GeometryShader*> geometry_shader_;
	std::unordered_map<std::string, std::vector<ID3D11Buffer*>> vertex_buffers_;
	std::unordered_map<std::string, std::vector<ID3D11Buffer*>> index_buffers_;
	std::unordered_map<std::string, std::vector<unsigned int>> index_size_;
	std::unordered_map<SamplerStateType, ID3D11SamplerState*> sampler_state_;
	std::unordered_map<Texture, ID3D11ShaderResourceView*> texture_;
	std::unordered_map<std::string, BoneTransform> bone_transform_;

public:
	Data();
	~Data();

public:
	void CreateVertexShaderAndInputLayout(std::string name, InputLayoutType type);
	void CreatePixelShader(std::string name);
	void CreateGeometryShader(std::string name);
	void CreateConstantBuffer(ID3D11Buffer*& constant_buffer, ConstantBufferType type);
	void CreateConstantBuffers(std::vector<ID3D11Buffer*>& constant_buffers, ConstantBufferType type, unsigned int size);
	void CreateVertexBuffers(std::string name, VertexBufferType type, Model& model);
	void CreateVertexBufferInstanced(std::string name, void* data, unsigned int instance_num);
	void CreateIndexBuffers(std::string name, Model& model);
	void CreateSamplerState(SamplerStateType type);
	void CreateTexture(Texture texture);
	void CreateAnimation(std::string name, Model& model);

public:
	ID3D11VertexShader* const GetVertexShader(std::string name);
	ID3D11InputLayout* const GetInputLayout(InputLayoutType type);
	ID3D11PixelShader* const GetPixelShader(std::string name);
	ID3D11GeometryShader* const GetGeometryShader(std::string name);
	std::vector<ID3D11Buffer*> const GetVertexBuffers(std::string name);
	std::vector<ID3D11Buffer*> const GetIndexBuffers(std::string name);
	std::vector<unsigned int> const GetIndexSize(std::string name);
	ID3D11SamplerState** const GetSamplerState(SamplerStateType type);
	ID3D11ShaderResourceView** const GetTexture(Texture texture);

public:
	void UpdateConstantBuffer(ID3D11Buffer* constant_buffer, const void* data);
	void UpdateVertexBuffers(std::string name, Model& model);
	void UpdateVertexBufferInstanced(std::string name, void* data);
	void UpdateAnimation(std::string name, std::string animations_key, double& animation_running_time, DirectX::XMMATRIX(&bones)[bone_max], unsigned int& size);

private:
	void ReadNode(DirectX::XMMATRIX(&bones)[bone_max], Animation& animation, BoneTransform& bone_transform, const char* node_name, Node& node, MeshBones& mesh_bones, double& time, DirectX::XMMATRIX& parent);
};