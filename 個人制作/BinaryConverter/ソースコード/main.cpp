#pragma warning(disable : 4819)

#include <assimp\Importer.hpp>
#include <assimp\scene.h>
#include <assimp\postprocess.h>

#include <Windows.h>
#include <DirectXMath.h>

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>

#include "timer.h"

#define WEIGHT_WORKAROUND
#define USE_FULL_PATH

using namespace DirectX;
using namespace std;

constexpr unsigned int vertex_weight_max = 4;
constexpr unsigned int buf_max = 256;
constexpr char* output_file_format = ".model";

struct Vertex
{
	XMFLOAT3 pos = { 0, 0, 0 };
	XMFLOAT3 normal = { 0, 0, 0 };
	XMFLOAT2 uv = { 0, 0 };
	unsigned int vertex_id[vertex_weight_max];
	float weight[vertex_weight_max];
};

struct Mesh
{
	vector<Vertex> vertices;
	unordered_map<string, unsigned int> bones_map;
	vector<XMFLOAT4X4> bones_offset;
	vector<unsigned int> indices;
};

struct Node
{
	XMFLOAT4X4 transformation;
	unordered_map<string, Node> children;
};

struct KeyVector3
{
	double time;
	XMFLOAT3 value;
};

struct KeyQuaternion
{
	double time;
	XMFLOAT4 value;
};

struct Channel
{
	vector<KeyVector3> position;
	vector<KeyQuaternion> rotation;
	vector<KeyVector3> scaling;
};

struct Animation
{
	double duration;
	double ticks_per_second;
	unordered_map<string, Channel> channels;
};

struct ModelData
{
	vector<Mesh> meshes;
	string root_node_name;
	Node root_node;
	unordered_map<string, Animation> animations;
};

void ReadNode(aiNode* s_node, Node& child)
{
	for (unsigned int c = 0; c < s_node->mNumChildren; ++c)
	{
		auto& s_child = s_node->mChildren[c];
		string name = s_child->mName.data;
		Node node;
		node.transformation._11 = s_child->mTransformation.a1;
		node.transformation._12 = s_child->mTransformation.a2;
		node.transformation._13 = s_child->mTransformation.a3;
		node.transformation._14 = s_child->mTransformation.a4;
		node.transformation._21 = s_child->mTransformation.b1;
		node.transformation._22 = s_child->mTransformation.b2;
		node.transformation._23 = s_child->mTransformation.b3;
		node.transformation._24 = s_child->mTransformation.b4;
		node.transformation._31 = s_child->mTransformation.c1;
		node.transformation._32 = s_child->mTransformation.c2;
		node.transformation._33 = s_child->mTransformation.c3;
		node.transformation._34 = s_child->mTransformation.c4;
		node.transformation._41 = s_child->mTransformation.d1;
		node.transformation._42 = s_child->mTransformation.d2;
		node.transformation._43 = s_child->mTransformation.d3;
		node.transformation._44 = s_child->mTransformation.d4;
		XMStoreFloat4x4(&node.transformation, XMMatrixTranspose(XMLoadFloat4x4(&node.transformation)));
		child.children.emplace(name, node);

		ReadNode(s_child, child.children[name]);
	}
}

void WriteNode(Node& child, ofstream& output_file)
{
	for (auto& c : child.children)
	{
		string name = c.first;
		unsigned int ns = name.size();
		output_file.write(reinterpret_cast<char*>(&ns), sizeof(ns));
		output_file.write(name.data(), sizeof(char) * ns);
		output_file.write(reinterpret_cast<char*>(&c.second.transformation), sizeof(c.second.transformation));
		unsigned int ccs = c.second.children.size();
		output_file.write(reinterpret_cast<char*>(&ccs), sizeof(ccs));

		WriteNode(c.second, output_file);
	}
}

void main(unsigned int argc, char* argv[])
{
	//argc = 2;
	//argv[1] = "eric.fbx";
	if (argc > 1)
	{
		Timer timer;
		timer.Start();

		for (unsigned int cnt = 1; cnt < argc; ++cnt)
		{
			cout << "ReadFile:		" << argv[cnt] << endl;
			cout << "Start reading:		";
			Assimp::Importer impoter;
			const aiScene* scene = impoter.ReadFile(argv[cnt], aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_CalcTangentSpace | aiProcess_ConvertToLeftHanded);

			if (scene)
			{
				cout << "Success" << endl;

				ModelData model;

				cout << "NumMeshes:		" << scene->mNumMeshes << endl;
				if (scene->mNumMeshes)
				{
					model.meshes.resize(scene->mNumMeshes);
					for (unsigned int m = 0; m < scene->mNumMeshes; ++m)
					{
						auto& mesh = model.meshes[m];
						auto& s_mesh = scene->mMeshes[m];

						cout << "NumVertices:		" << s_mesh->mNumVertices << endl;
						if (s_mesh->mNumVertices)
						{
							mesh.vertices.resize(s_mesh->mNumVertices);

							for (unsigned int v = 0; v < s_mesh->mNumVertices; ++v)
							{
								auto& vertex = mesh.vertices[v];

								vertex.pos.x = s_mesh->mVertices[v].x;
								vertex.pos.y = s_mesh->mVertices[v].y;
								vertex.pos.z = s_mesh->mVertices[v].z;

								if (s_mesh->mNormals)
								{
									vertex.normal.x = s_mesh->mNormals[v].x;
									vertex.normal.y = s_mesh->mNormals[v].y;
									vertex.normal.z = s_mesh->mNormals[v].z;
								}

								if (s_mesh->mTextureCoords[0])
								{
									vertex.uv.x = s_mesh->mTextureCoords[0][v].x;
									vertex.uv.y = s_mesh->mTextureCoords[0][v].y;
								}

								if (s_mesh->mTangents)
								{
									s_mesh->mTangents[v].x;
									s_mesh->mTangents[v].y;
									s_mesh->mTangents[v].z;
								}
							}
						}

						cout << "NumBones:		" << s_mesh->mNumBones << endl;
						if (s_mesh->mNumBones)
						{
							mesh.bones_offset.resize(s_mesh->mNumBones);

							unsigned int num_bone = 0;
							for (unsigned int b = 0; b < s_mesh->mNumBones; ++b)
							{
								auto& s_bone = s_mesh->mBones[b];

								unsigned int bone_index = 0;
								string bone_name = s_bone->mName.data;

								if (mesh.bones_map.find(bone_name) == mesh.bones_map.end())
								{
									auto& bone_offset = mesh.bones_offset;

									bone_index = num_bone;
									++num_bone;
									bone_offset[bone_index]._11 = s_bone->mOffsetMatrix.a1;
									bone_offset[bone_index]._12 = s_bone->mOffsetMatrix.a2;
									bone_offset[bone_index]._13 = s_bone->mOffsetMatrix.a3;
									bone_offset[bone_index]._14 = s_bone->mOffsetMatrix.a4;
									bone_offset[bone_index]._21 = s_bone->mOffsetMatrix.b1;
									bone_offset[bone_index]._22 = s_bone->mOffsetMatrix.b2;
									bone_offset[bone_index]._23 = s_bone->mOffsetMatrix.b3;
									bone_offset[bone_index]._24 = s_bone->mOffsetMatrix.b4;
									bone_offset[bone_index]._31 = s_bone->mOffsetMatrix.c1;
									bone_offset[bone_index]._32 = s_bone->mOffsetMatrix.c2;
									bone_offset[bone_index]._33 = s_bone->mOffsetMatrix.c3;
									bone_offset[bone_index]._34 = s_bone->mOffsetMatrix.c4;
									bone_offset[bone_index]._41 = s_bone->mOffsetMatrix.d1;
									bone_offset[bone_index]._42 = s_bone->mOffsetMatrix.d2;
									bone_offset[bone_index]._43 = s_bone->mOffsetMatrix.d3;
									bone_offset[bone_index]._44 = s_bone->mOffsetMatrix.d4;
									XMStoreFloat4x4(&bone_offset[bone_index], XMMatrixTranspose(XMLoadFloat4x4(&bone_offset[bone_index])));
									mesh.bones_map[bone_name] = bone_index;
								}
								else
								{
									bone_index = mesh.bones_map[bone_name];
								}

								for (unsigned int w = 0; w < s_bone->mNumWeights; ++w)
								{
									unsigned int v = s_bone->mWeights[w].mVertexId;
									float weight = s_bone->mWeights[w].mWeight;

									for (unsigned int vw = 0; vw < vertex_weight_max; ++vw)
									{
										auto& vertex = mesh.vertices[v];
										if (vertex.weight[vw] == 0.0f)
										{
											vertex.vertex_id[vw] = bone_index;
											vertex.weight[vw] = weight;
											break;
										}
									}
								}
							}
						}

						cout << "NumFaces:		" << s_mesh->mNumFaces << endl;
						if (s_mesh->mNumFaces)
						{
							for (unsigned int f = 0; f < s_mesh->mNumFaces; ++f)
							{
								auto& face = s_mesh->mFaces[f];
								for (unsigned int i = 0; i < face.mNumIndices; ++i)
								{
									mesh.indices.emplace_back(face.mIndices[i]);
								}
							}
						}
					}
				}

				auto& s_node = scene->mRootNode;
				model.root_node_name = s_node->mName.data;
				model.root_node.transformation._11 = s_node->mTransformation.a1;
				model.root_node.transformation._12 = s_node->mTransformation.a2;
				model.root_node.transformation._13 = s_node->mTransformation.a3;
				model.root_node.transformation._14 = s_node->mTransformation.a4;
				model.root_node.transformation._21 = s_node->mTransformation.b1;
				model.root_node.transformation._22 = s_node->mTransformation.b2;
				model.root_node.transformation._23 = s_node->mTransformation.b3;
				model.root_node.transformation._24 = s_node->mTransformation.b4;
				model.root_node.transformation._31 = s_node->mTransformation.c1;
				model.root_node.transformation._32 = s_node->mTransformation.c2;
				model.root_node.transformation._33 = s_node->mTransformation.c3;
				model.root_node.transformation._34 = s_node->mTransformation.c4;
				model.root_node.transformation._41 = s_node->mTransformation.d1;
				model.root_node.transformation._42 = s_node->mTransformation.d2;
				model.root_node.transformation._43 = s_node->mTransformation.d3;
				model.root_node.transformation._44 = s_node->mTransformation.d4;
				XMStoreFloat4x4(&model.root_node.transformation, XMMatrixTranspose(XMLoadFloat4x4(&model.root_node.transformation)));
				for (unsigned int c = 0; c < s_node->mNumChildren; ++c)
				{
					auto& s_child = s_node->mChildren[c];
					string key = s_child->mName.data;
					Node node;
					node.transformation._11 = s_child->mTransformation.a1;
					node.transformation._12 = s_child->mTransformation.a2;
					node.transformation._13 = s_child->mTransformation.a3;
					node.transformation._14 = s_child->mTransformation.a4;
					node.transformation._21 = s_child->mTransformation.b1;
					node.transformation._22 = s_child->mTransformation.b2;
					node.transformation._23 = s_child->mTransformation.b3;
					node.transformation._24 = s_child->mTransformation.b4;
					node.transformation._31 = s_child->mTransformation.c1;
					node.transformation._32 = s_child->mTransformation.c2;
					node.transformation._33 = s_child->mTransformation.c3;
					node.transformation._34 = s_child->mTransformation.c4;
					node.transformation._41 = s_child->mTransformation.d1;
					node.transformation._42 = s_child->mTransformation.d2;
					node.transformation._43 = s_child->mTransformation.d3;
					node.transformation._44 = s_child->mTransformation.d4;
					XMStoreFloat4x4(&node.transformation, XMMatrixTranspose(XMLoadFloat4x4(&node.transformation)));
					model.root_node.children.emplace(key, node);
					ReadNode(s_child, model.root_node.children[key]);
				}

				cout << "NumAnimations:		" << scene->mNumAnimations << endl;
				if (scene->mNumAnimations)
				{
					for (unsigned int a = 0; a < scene->mNumAnimations; ++a)
					{
						auto& s_animation = scene->mAnimations[a];
						Animation animation;
						cout << "AnimationName:		" << s_animation->mName.data << endl;
						cout << " Channels:		" << s_animation->mNumChannels << endl;
						cout << " Duration:		" << s_animation->mDuration << endl;
						animation.duration = s_animation->mDuration;
						cout << " TicksPerSecond:	" << s_animation->mTicksPerSecond << endl;
						animation.ticks_per_second = s_animation->mTicksPerSecond;
						for (unsigned int c = 0; c < s_animation->mNumChannels; ++c)
						{
							auto& s_channel = s_animation->mChannels[c];
							Channel channel;
							channel.position.resize(s_channel->mNumPositionKeys);
							for (unsigned int s = 0; s < s_channel->mNumPositionKeys; ++s)
							{
								channel.position[s].time = s_channel->mPositionKeys[s].mTime;
								channel.position[s].value.x = s_channel->mPositionKeys[s].mValue.x;
								channel.position[s].value.y = s_channel->mPositionKeys[s].mValue.y;
								channel.position[s].value.z = s_channel->mPositionKeys[s].mValue.z;
							}

							channel.rotation.resize(s_channel->mNumRotationKeys);
							for (unsigned int s = 0; s < s_channel->mNumRotationKeys; ++s)
							{
								channel.rotation[s].time = s_channel->mRotationKeys[s].mTime;
								channel.rotation[s].value.x = s_channel->mRotationKeys[s].mValue.x;
								channel.rotation[s].value.y = s_channel->mRotationKeys[s].mValue.y;
								channel.rotation[s].value.z = s_channel->mRotationKeys[s].mValue.z;
								channel.rotation[s].value.w = s_channel->mRotationKeys[s].mValue.w;
							}

							channel.scaling.resize(s_channel->mNumScalingKeys);
							for (unsigned int s = 0; s < s_channel->mNumScalingKeys; ++s)
							{
								channel.scaling[s].time = s_channel->mScalingKeys[s].mTime;
								channel.scaling[s].value.x = s_channel->mScalingKeys[s].mValue.x;
								channel.scaling[s].value.y = s_channel->mScalingKeys[s].mValue.y;
								channel.scaling[s].value.z = s_channel->mScalingKeys[s].mValue.z;
							}
							animation.channels.emplace(s_channel->mNodeName.data, channel);
						}

						model.animations.emplace(s_animation->mName.data, animation);
					}
				}

				string filename = argv[cnt];
				unsigned int path = filename.find_last_of('\\');
				if (path != string::npos)
					filename.erase(0, path + 1);
				unsigned int format = filename.find_last_of('.');
				if (format != string::npos)
					filename.erase(format, filename.size() - format);
				string exe_path = argv[0];
				path = exe_path.find_last_of('\\');
				if (path != string::npos)
					exe_path.erase(path, exe_path.size() - path);
				string file = exe_path + "\\" + filename + output_file_format;
				cout << "WriteFile:		" << file << endl;
				ofstream output_file(file, ios::out | ios::binary);

				unsigned int ms = model.meshes.size();
				output_file.write(reinterpret_cast<char*>(&ms), sizeof(ms));
				for (unsigned int m = 0; m < ms; ++m)
				{
					cout << "MeshCount:		" << m << endl;
					auto& mesh = model.meshes[m];

					cout << "Write Vertices:		";
					unsigned int vs = mesh.vertices.size();
					output_file.write(reinterpret_cast<char*>(&vs), sizeof(vs));
					for (unsigned int v = 0; v < vs; ++v)
						output_file.write(reinterpret_cast<char*>(&mesh.vertices[v]), sizeof(mesh.vertices[v]));
					cout << "Success" << endl;

					cout << "Write BonesMap:		";
					unsigned int bms = mesh.bones_map.size();
					output_file.write(reinterpret_cast<char*>(&bms), sizeof(bms));
					for (auto& bone_map : mesh.bones_map)
					{
						unsigned int ks = bone_map.first.size();
						output_file.write(reinterpret_cast<char*>(&ks), sizeof(ks));
						output_file.write(bone_map.first.data(), sizeof(char) * ks);
						output_file.write(reinterpret_cast<char*>(&bone_map.second), sizeof(bone_map.second));
					}
					cout << "Success" << endl;

					cout << "Write BonesOffset:	";
					unsigned int bos = mesh.bones_offset.size();
					output_file.write(reinterpret_cast<char*>(&bos), sizeof(bos));
					for (unsigned int bo = 0; bo < bos; ++bo)
						output_file.write(reinterpret_cast<char*>(&mesh.bones_offset[bo]), sizeof(mesh.bones_offset[bo]));
					cout << "Success" << endl;

					cout << "Write Indices:		";
					unsigned int is = mesh.indices.size();
					output_file.write(reinterpret_cast<char*>(&is), sizeof(is));
					for (unsigned int i = 0; i < is; ++i)
						output_file.write(reinterpret_cast<char*>(&mesh.indices[i]), sizeof(mesh.indices[i]));
					cout << "Success" << endl;
				}
				cout << "Write Nodes:		";
				unsigned int rns = model.root_node_name.size();
				output_file.write(reinterpret_cast<char*>(&rns), sizeof(rns));
				output_file.write(model.root_node_name.data(), sizeof(char) * rns);
				output_file.write(reinterpret_cast<char*>(&model.root_node.transformation), sizeof(model.root_node.transformation));
				unsigned int cs = model.root_node.children.size();
				output_file.write(reinterpret_cast<char*>(&cs), sizeof(cs));
				for (auto& child : model.root_node.children)
				{
					string name = child.first;
					unsigned int ns = name.size();
					output_file.write(reinterpret_cast<char*>(&ns), sizeof(ns));
					output_file.write(name.data(), sizeof(char) * ns);
					output_file.write(reinterpret_cast<char*>(&child.second.transformation), sizeof(child.second.transformation));
					unsigned int ccs = child.second.children.size();
					output_file.write(reinterpret_cast<char*>(&ccs), sizeof(ccs));
					WriteNode(child.second, output_file);
				}
				cout << "Success" << endl;

				cout << "Write Animation:	";
				unsigned int as = model.animations.size();
				output_file.write(reinterpret_cast<char*>(&as), sizeof(as));
				for (auto& animation : model.animations)
				{
					unsigned int ks = animation.first.size();
					output_file.write(reinterpret_cast<char*>(&ks), sizeof(ks));
					output_file.write(animation.first.data(), sizeof(char) * ks);
					output_file.write(reinterpret_cast<char*>(&animation.second.ticks_per_second), sizeof(animation.second.ticks_per_second));
					output_file.write(reinterpret_cast<char*>(&animation.second.duration), sizeof(animation.second.duration));
					unsigned int cs = animation.second.channels.size();
					output_file.write(reinterpret_cast<char*>(&cs), sizeof(cs));
					for (auto& channel:animation.second.channels)
					{
						ks = channel.first.size();
						output_file.write(reinterpret_cast<char*>(&ks), sizeof(ks));
						output_file.write(channel.first.data(), sizeof(char) * ks);
						unsigned int ps = channel.second.position.size();
						output_file.write(reinterpret_cast<char*>(&ps), sizeof(ps));
						for (unsigned int p = 0; p < ps; ++p)
						{
							output_file.write(reinterpret_cast<char*>(&channel.second.position[p].time), sizeof(channel.second.position[p].time));
							output_file.write(reinterpret_cast<char*>(&channel.second.position[p].value), sizeof(channel.second.position[p].value));
						}
						unsigned int rs = channel.second.rotation.size();
						output_file.write(reinterpret_cast<char*>(&rs), sizeof(rs));
						for (unsigned int r = 0; r < rs; ++r)
						{
							output_file.write(reinterpret_cast<char*>(&channel.second.rotation[r].time), sizeof(channel.second.rotation[r].time));
							output_file.write(reinterpret_cast<char*>(&channel.second.rotation[r].value), sizeof(channel.second.rotation[r].value));
						}
						unsigned int ss = channel.second.scaling.size();
						output_file.write(reinterpret_cast<char*>(&ss), sizeof(ss));
						for (unsigned int s = 0; s < ss; ++s)
						{
							output_file.write(reinterpret_cast<char*>(&channel.second.scaling[s].time), sizeof(channel.second.scaling[s].time));
							output_file.write(reinterpret_cast<char*>(&channel.second.scaling[s].value), sizeof(channel.second.scaling[s].value));
						}
					}
				}
				cout << "Success" << endl;

				timer.Stop();
				cout << "ElapsedTime:		" << timer.GetTime() << endl;
			}
			else
			{
				cout << "Error" << endl;
				cout << "Possible cause of error:";
				cout << impoter.GetErrorString() << endl;
			}
		}
	}

	getchar();
}