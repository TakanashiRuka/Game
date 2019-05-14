#include "..\model\model_import.h"

#include "..\framework\window.h"
#include "..\framework\utility.h"

#include <time.h>

#include <fstream>

constexpr char* model_file_path = "data/model/";
constexpr char* model_file_format = ".model";

void ReadChild(unsigned int& cs, Node& child, std::ifstream& input_file)
{
	for (unsigned int c = 0; c < cs; ++c)
	{
		unsigned int ns;
		input_file.read(reinterpret_cast<char*>(&ns), sizeof(ns));
		char buf[128];
		input_file.read(buf, sizeof(char) * ns);
		std::string name(buf, ns);
		Node node;
		input_file.read(reinterpret_cast<char*>(&node.transformation), sizeof(node.transformation));
		child.children.emplace(name, node);
		unsigned int ccs;
		input_file.read(reinterpret_cast<char*>(&ccs), sizeof(ccs));
		ReadChild(ccs, child.children[name], input_file);
	}
}

Model ModelImport::UIData(float x, float y, float width, float height, float uv_x, float uv_y, float uv_w, float uv_h)
{
	Model model;

	float vx = -1.0f + x * 2.0f / Window::GetWidth();
	float vy = 1.0f - y * 2.0f / Window::GetHeight();
	float vw = -1.0f + (x + width) * 2.0f / Window::GetWidth();
	float vh = 1.0f - (y + height) * 2.0f / Window::GetHeight();

	std::vector<Mesh> meshes;

	Mesh mesh;
	mesh.vertices.resize(4);
	mesh.indices.resize(4);

	mesh.vertices[0].pos = DirectX::XMFLOAT3(vx, vh, 0);
	mesh.vertices[0].uv = DirectX::XMFLOAT2(uv_x, uv_y + uv_h);

	mesh.vertices[1].pos = DirectX::XMFLOAT3(vx, vy, 0);
	mesh.vertices[1].uv = DirectX::XMFLOAT2(uv_x, uv_y);

	mesh.vertices[2].pos = DirectX::XMFLOAT3(vw, vh, 0);
	mesh.vertices[2].uv = DirectX::XMFLOAT2(uv_x + uv_w, uv_y + uv_h);

	mesh.vertices[3].pos = DirectX::XMFLOAT3(vw, vy, 0);
	mesh.vertices[3].uv = DirectX::XMFLOAT2(uv_x + uv_w, uv_y);

	mesh.indices[0] = 0U;
	mesh.indices[1] = 1U;
	mesh.indices[2] = 2U;
	mesh.indices[3] = 3U;

	meshes.emplace_back(mesh);
	model.meshes = meshes;

	return model;
}

Model ModelImport::Number(float x, float y, float width, float height, unsigned int number, unsigned int digit, bool zero_fill)
{
	Model model;

	float vx = -1.0f + x * 2.0f / Window::GetWidth<float>();
	float vy = 1.0f - y * 2.0f / Window::GetHeight<float>();
	float vw = -1.0f + (x + width) * 2.0f / Window::GetWidth<float>();
	float vh = 1.0f - (y + height) * 2.0f / Window::GetHeight<float>();

	float w = (vw - vx) / digit;

	unsigned int num = number;

	std::vector<Mesh> meshes;
	meshes.clear();

	for (unsigned int d = 0; d < digit; ++d)
	{
		Mesh mesh;

		mesh.vertices.resize(4);
		mesh.indices.resize(4);

		float uv_w = 0.1f;
		float uv_x = num % 10 * uv_w;
		if (d != 0 && num == 0)
		{
			if (!zero_fill)
			{
				uv_x = 0;
				uv_w = 0;
			}
		}

		mesh.vertices[0].pos = DirectX::XMFLOAT3(vw - w, vh, 0);
		mesh.vertices[0].uv = DirectX::XMFLOAT2(uv_x, 1);

		mesh.vertices[1].pos = DirectX::XMFLOAT3(vw - w, vy, 0);
		mesh.vertices[1].uv = DirectX::XMFLOAT2(uv_x, 0);

		mesh.vertices[2].pos = DirectX::XMFLOAT3(vw, vh, 0);
		mesh.vertices[2].uv = DirectX::XMFLOAT2(uv_x + uv_w, 1);

		mesh.vertices[3].pos = DirectX::XMFLOAT3(vw, vy, 0);
		mesh.vertices[3].uv = DirectX::XMFLOAT2(uv_x + uv_w, 0);

		mesh.indices[0] = 0U;
		mesh.indices[1] = 1U;
		mesh.indices[2] = 2U;
		mesh.indices[3] = 3U;

		meshes.emplace_back(mesh);

		num /= 10;
		vw -= w;
	}

	model.meshes = meshes;

	return model;
}

Model ModelImport::BillBoardData(float width, float height, float uv_x, float uv_y, float uv_w, float uv_h)
{
	Model model;

	float vx = -width * 0.5f;
	float vy = height * 0.5f;
	float vw = vx + width;
	float vh = vy - height;

	std::vector<Mesh> meshes;

	Mesh mesh;
	mesh.vertices.resize(4);
	mesh.indices.resize(4);

	mesh.vertices[0].pos = DirectX::XMFLOAT3(vx, vh, 0);
	mesh.vertices[0].uv = DirectX::XMFLOAT2(uv_x, uv_y + uv_h);

	mesh.vertices[1].pos = DirectX::XMFLOAT3(vx, vy, 0);
	mesh.vertices[1].uv = DirectX::XMFLOAT2(uv_x, uv_y);

	mesh.vertices[2].pos = DirectX::XMFLOAT3(vw, vh, 0);
	mesh.vertices[2].uv = DirectX::XMFLOAT2(uv_x + uv_w, uv_y + uv_h);

	mesh.vertices[3].pos = DirectX::XMFLOAT3(vw, vy, 0);
	mesh.vertices[3].uv = DirectX::XMFLOAT2(uv_x + uv_w, uv_y);

	mesh.indices[0] = 0U;
	mesh.indices[1] = 1U;
	mesh.indices[2] = 2U;
	mesh.indices[3] = 3U;

	meshes.emplace_back(mesh);
	model.meshes = meshes;

	return model;
}

Model ModelImport::FieldData(unsigned int width, unsigned int height, float size_w, float size_h, float max_y)
{
	srand((unsigned int)time(nullptr));
	int y = static_cast<int>(max_y * 10.f);

	Model model;

	Mesh mesh;

	for (unsigned int h = height; -1 < (int)h; --h)
	{
		for (unsigned int w = 0; w <= width; ++w)
		{
			Vertex vertex;

			vertex.pos = DirectX::XMFLOAT3(w * size_w, ((float)(rand() % y + 1) / 10), h * size_h);
			vertex.normal = DirectX::XMFLOAT3(0, 1, 0);
			vertex.uv = DirectX::XMFLOAT2((float)w, (float)height - h);

			mesh.vertices.emplace_back(vertex);
		}
	}

	mesh.indices.resize((width + 1) * 2 * height + (height - 1) * 2);

	{
		unsigned int i = 0;
		for (unsigned int h = 0; h < height; ++h)
		{
			for (unsigned int w = 0; w <= width; ++w)
			{
				mesh.indices[i] = w + (width + 1) * (h + 1);
				++i;
				mesh.indices[i] = w + (width + 1) * h;
				++i;

				if (w == width && h < height - 1)
				{
					mesh.indices[i] = mesh.indices[i - 1];
					++i;
					mesh.indices[i] = (width + 1) * (h + 2);
					++i;
				}
			}
		}
	}

	//std::vector<DirectX::XMFLOAT3> normal;
	//normal.resize(mesh.vertices.size());
	//unsigned int is = mesh.indices.size() - 1;
	//for (unsigned int i = 1; i < is; ++i)
	//{
	//	DirectX::XMVECTOR posA = DirectX::XMLoadFloat3(&mesh.vertices[mesh.indices[i - 1]].pos);
	//	DirectX::XMVECTOR posB = DirectX::XMLoadFloat3(&mesh.vertices[mesh.indices[i]].pos);
	//	DirectX::XMVECTOR posC = DirectX::XMLoadFloat3(&mesh.vertices[mesh.indices[i + 1]].pos);

	//	DirectX::XMVECTOR vecAB = DirectX::XMVectorSubtract(posB, posA);
	//	DirectX::XMVECTOR vecBC = DirectX::XMVectorSubtract(posC, posB);

	//	DirectX::XMVECTOR surface = DirectX::XMVectorZero();
	//	surface = DirectX::XMVector3Normalize(DirectX::XMVector3Cross(vecAB, vecBC));

	//	DirectX::XMFLOAT3 n;
	//	DirectX::XMStoreFloat3(&n, surface);
	//	if (n.y <= 0)
	//	{
	//		surface = DirectX::XMVectorMultiply(surface, DirectX::XMVectorSet(-1, -1, -1, 1));
	//	}

	//	DirectX::XMStoreFloat3(&normal[mesh.indices[i - 1]], DirectX::XMVectorAdd(DirectX::XMLoadFloat3(&normal[mesh.indices[i - 1]]), surface));
	//	DirectX::XMStoreFloat3(&normal[mesh.indices[i]], DirectX::XMVectorAdd(DirectX::XMLoadFloat3(&normal[mesh.indices[i]]), surface));
	//	DirectX::XMStoreFloat3(&normal[mesh.indices[i + 1]], DirectX::XMVectorAdd(DirectX::XMLoadFloat3(&normal[mesh.indices[i + 1]]), surface));
	//}

	//unsigned int vs = mesh.vertices.size();
	//for (unsigned int v = 0; v < vs; ++v)
	//{
	//	DirectX::XMStoreFloat3(&normal[v], DirectX::XMVector3Normalize(DirectX::XMLoadFloat3(&normal[v])));
	//	mesh.vertices[v].normal = normal[v];
	//}

	model.meshes.emplace_back(mesh);

	return model;
}

Model ModelImport::ModelData(std::string name)
{
	std::ifstream input_file(model_file_path + name + model_file_format, std::ios::in | std::ios::binary);
	Model model;

	if (input_file.is_open())
	{
		//メッシュの読み込み
		unsigned int ms;
		input_file.read(reinterpret_cast<char*>(&ms), sizeof(ms));
		model.meshes.resize(ms);
		for (unsigned int m = 0; m < ms; ++m)
		{
			auto& mesh = model.meshes[m];

			//バーテックスの読み込み
			unsigned int vs;
			input_file.read(reinterpret_cast<char*>(&vs), sizeof(vs));
			mesh.vertices.resize(vs);
			for (unsigned int v = 0; v < vs; ++v)
				input_file.read(reinterpret_cast<char*>(&mesh.vertices[v]), sizeof(mesh.vertices[v]));

			//ボーンマップの読み込み
			unsigned int bms;
			input_file.read(reinterpret_cast<char*>(&bms), sizeof(bms));
			for (unsigned int bm = 0; bm < bms; ++bm)
			{
				unsigned int ks;
				input_file.read(reinterpret_cast<char*>(&ks), sizeof(ks));
				char buf[128];
				input_file.read(buf, sizeof(char) * ks);
				std::string key(buf, ks);
				unsigned int value;
				input_file.read(reinterpret_cast<char*>(&value), sizeof(value));
				mesh.bones_map.emplace(key, value);
			}

			//ボーンの読み込み
			unsigned int bos;
			input_file.read(reinterpret_cast<char*>(&bos), sizeof(bos));
			mesh.bones_offset.resize(bos);
			for (unsigned int bo = 0; bo < bos; ++bo)
				input_file.read(reinterpret_cast<char*>(&mesh.bones_offset[bo]), sizeof(mesh.bones_offset[bo]));

			//インデックスの読み込み
			unsigned int is;
			input_file.read(reinterpret_cast<char*>(&is), sizeof(is));
			mesh.indices.resize(is);
			for (unsigned int i = 0; i < is; ++i)
				input_file.read(reinterpret_cast<char*>(&mesh.indices[i]), sizeof(mesh.indices[i]));
		}

		//ノードの読み込み
		unsigned int rns;
		input_file.read(reinterpret_cast<char*>(&rns), sizeof(rns));
		char buf[128];
		input_file.read(buf, sizeof(char) * rns);
		std::string root_node_name(buf, rns);
		model.root_node_name = root_node_name;
		input_file.read(reinterpret_cast<char*>(&model.root_node.transformation), sizeof(model.root_node.transformation));
		unsigned int child_s;
		input_file.read(reinterpret_cast<char*>(&child_s), sizeof(child_s));
		for (unsigned int child = 0; child < child_s; ++child)
		{
			unsigned int ns;
			input_file.read(reinterpret_cast<char*>(&ns), sizeof(ns));
			input_file.read(buf, sizeof(char) * ns);
			std::string key(buf, ns);
			Node node;
			input_file.read(reinterpret_cast<char*>(&node.transformation), sizeof(node.transformation));
			model.root_node.children.emplace(key, node);
			unsigned int cs;
			input_file.read(reinterpret_cast<char*>(&cs), sizeof(cs));
			ReadChild(cs, model.root_node.children[key], input_file);
		}

		//アニメーションの読み込み
		unsigned int as;
		input_file.read(reinterpret_cast<char*>(&as), sizeof(as));
		for (unsigned int a = 0; a < as; ++a)
		{
			unsigned int ks;
			input_file.read(reinterpret_cast<char*>(&ks), sizeof(ks));
			input_file.read(buf, sizeof(char) * ks);
			std::string key(buf, ks);
			Animation animation;
			input_file.read(reinterpret_cast<char*>(&animation.ticks_per_second), sizeof(animation.ticks_per_second));
			input_file.read(reinterpret_cast<char*>(&animation.duration), sizeof(animation.duration));
			unsigned int cs;
			input_file.read(reinterpret_cast<char*>(&cs), sizeof(cs));
			for (unsigned int c = 0; c < cs; ++c)
			{
				Channel channel;
				input_file.read(reinterpret_cast<char*>(&ks), sizeof(ks));
				input_file.read(buf, sizeof(char) * ks);
				std::string key_c(buf, ks);
				unsigned int ps;
				input_file.read(reinterpret_cast<char*>(&ps), sizeof(ps));
				channel.position.resize(ps);
				for (unsigned int p = 0; p < ps; ++p)
				{
					input_file.read(reinterpret_cast<char*>(&channel.position[p].time), sizeof(channel.position[p].time));
					input_file.read(reinterpret_cast<char*>(&channel.position[p].value), sizeof(channel.position[p].value));
				}
				unsigned int rs;
				input_file.read(reinterpret_cast<char*>(&rs), sizeof(rs));
				channel.rotation.resize(rs);
				for (unsigned int r = 0; r < rs; ++r)
				{
					input_file.read(reinterpret_cast<char*>(&channel.rotation[r].time), sizeof(channel.rotation[r].time));
					input_file.read(reinterpret_cast<char*>(&channel.rotation[r].value), sizeof(channel.rotation[r].value));
				}
				unsigned int ss;
				input_file.read(reinterpret_cast<char*>(&ss), sizeof(ss));
				channel.scaling.resize(ss);
				for (unsigned int s = 0; s < ss; ++s)
				{
					input_file.read(reinterpret_cast<char*>(&channel.scaling[s].time), sizeof(channel.scaling[s].time));
					input_file.read(reinterpret_cast<char*>(&channel.scaling[s].value), sizeof(channel.scaling[s].value));
				}

				animation.channels.emplace(key_c, channel);
			}

			model.animations.emplace(key, animation);
		}
	}
	else
	{
		ErrorMessage(name + model_file_format + "が見つかりませんでした");
	}

	return model;
}