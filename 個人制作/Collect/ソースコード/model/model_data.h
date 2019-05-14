#pragma once

#include <DirectXMath.h>

#include <string>
#include <vector>
#include <unordered_map>

constexpr unsigned int vertex_weight_max = 4;

struct Vertex
{
	DirectX::XMFLOAT3 pos = { 0, 0, 0 };
	DirectX::XMFLOAT3 normal = { 0, 0, 0 };
	DirectX::XMFLOAT2 uv = { 0, 0 };
	unsigned int vertex_id[vertex_weight_max];
	float weight[vertex_weight_max];
};

struct Mesh
{
	std::vector<Vertex> vertices;
	std::unordered_map<std::string, unsigned int> bones_map;
	std::vector<DirectX::XMFLOAT4X4> bones_offset;
	std::vector<unsigned int> indices;
};

struct Node
{
	DirectX::XMFLOAT4X4 transformation;
	std::unordered_map<std::string, Node> children;
};

struct KeyVector3
{
	double time;
	DirectX::XMFLOAT3 value;
};

struct KeyQuaternion
{
	double time;
	DirectX::XMFLOAT4 value;
};

struct Channel
{
	std::vector<KeyVector3> position;
	std::vector<KeyQuaternion> rotation;
	std::vector<KeyVector3> scaling;
};

struct Animation
{
	double duration;
	double ticks_per_second;
	std::unordered_map<std::string, Channel> channels;
};

struct Model
{
	std::vector<Mesh> meshes;
	std::string root_node_name;
	Node root_node;
	std::unordered_map<std::string, Animation> animations;
};