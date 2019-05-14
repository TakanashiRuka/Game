#pragma once

#include "..\model\model_data.h"

#include <DirectXMath.h>

#include <vector>

constexpr float epsilon = 1.0e-5f;

class CollisionMath
{
public:
	static float GetDistance2(float x1, float x2, float y1, float y2)
	{
		return std::sqrtf(std::powf(x1 - x2, 2) + std::powf(y1 - y2, 2));
	}

	static bool ViewFrustum(DirectX::XMFLOAT3 position, float radius, DirectX::XMMATRIX& view, float fov, float aspect, float z_near, float z_far)
	{
		DirectX::XMVECTOR pos = DirectX::XMVector3TransformCoord(DirectX::XMVectorSet(position.x, position.y, position.z, 0), view);
		float angle_half = DirectX::XMConvertToRadians(fov) / 2;

		DirectX::XMVECTOR top, bottom, left, right;
		DirectX::XMVECTOR point1, point2, point3;

		point1.m128_f32[0] = point1.m128_f32[1] = point1.m128_f32[2] = 0;
		point2.m128_f32[0] = -z_far * (tanf(angle_half) * aspect);
		point2.m128_f32[1] = z_far * tanf(angle_half);
		point2.m128_f32[2] = z_far;
		point3.m128_f32[0] = -point2.m128_f32[0];
		point3.m128_f32[1] = point2.m128_f32[1];
		point3.m128_f32[2] = point2.m128_f32[2];
		top = DirectX::XMPlaneFromPoints(point1, point2, point3);

		point1.m128_f32[0] = point1.m128_f32[1] = point1.m128_f32[2] = 0;
		point2.m128_f32[0] = z_far * (tanf(angle_half) * aspect);
		point2.m128_f32[1] = -z_far * tanf(angle_half);
		point2.m128_f32[2] = z_far;
		point3.m128_f32[0] = -point2.m128_f32[0];
		point3.m128_f32[1] = point2.m128_f32[1];
		point3.m128_f32[2] = point2.m128_f32[2];
		bottom = DirectX::XMPlaneFromPoints(point1, point2, point3);

		point1.m128_f32[0] = point1.m128_f32[1] = point1.m128_f32[2] = 0;
		point2.m128_f32[0] = -z_far * (tanf(angle_half) * aspect);
		point2.m128_f32[1] = -z_far * tanf(angle_half);
		point2.m128_f32[2] = z_far;
		point3.m128_f32[0] = point2.m128_f32[0];
		point3.m128_f32[1] = -point2.m128_f32[1];
		point3.m128_f32[2] = point2.m128_f32[2];
		left = DirectX::XMPlaneFromPoints(point1, point2, point3);

		point1.m128_f32[0] = point1.m128_f32[1] = point1.m128_f32[2] = 0;
		point2.m128_f32[0] = z_far * (tanf(angle_half) * aspect);
		point2.m128_f32[1] = z_far * tanf(angle_half);
		point2.m128_f32[2] = z_far;
		point3.m128_f32[0] = point2.m128_f32[0];
		point3.m128_f32[1] = -point2.m128_f32[1];
		point3.m128_f32[2] = point2.m128_f32[2];
		right = DirectX::XMPlaneFromPoints(point1, point2, point3);

		if ((pos.m128_f32[2] + radius) < z_near)
			return false;
		if ((pos.m128_f32[2] - radius) > z_far)
			return false;
		if ((pos.m128_f32[1] * top.m128_f32[1] + pos.m128_f32[2] * top.m128_f32[2]) > radius)
			return false;
		if ((pos.m128_f32[1] * bottom.m128_f32[1] + pos.m128_f32[2] * bottom.m128_f32[2]) > radius)
			return false;
		if ((pos.m128_f32[0] * left.m128_f32[0] + pos.m128_f32[2] * left.m128_f32[2]) > radius)
			return false;
		if ((pos.m128_f32[0] * right.m128_f32[0] + pos.m128_f32[2] * right.m128_f32[2]) > radius)
			return false;

		return true;
	}

	static float GetFieldY(std::vector<Mesh>& meshes, DirectX::XMFLOAT3& pos)
	{
		float field_y = 0;
		bool out = false;
		for (auto& mesh : meshes)
		{
			unsigned int is = mesh.indices.size() - 1;
			for (unsigned int i = 1; i < is; ++i)
			{
				auto& a = mesh.vertices[mesh.indices[i - 1]].pos;
				auto& b = mesh.vertices[mesh.indices[i]].pos;
				auto& c = mesh.vertices[mesh.indices[i + 1]].pos;
				auto& p = pos;

				DirectX::XMFLOAT3 ab(b.x - a.x, 0, b.z - a.z);
				DirectX::XMFLOAT3 bp(p.x - b.x, 0, p.z - b.z);
				DirectX::XMFLOAT3 bc(c.x - b.x, 0, c.z - b.z);
				DirectX::XMFLOAT3 cp(p.x - c.x, 0, p.z - c.z);
				DirectX::XMFLOAT3 ca(a.x - c.x, 0, a.z - c.z);
				DirectX::XMFLOAT3 ap(p.x - a.x, 0, p.z - a.z);

				float h1 = ab.x * bp.z - ab.z * bp.x;
				float h2 = bc.x * cp.z - bc.z * cp.x;
				float h3 = ca.x * ap.z - ca.z * ap.x;

				if ((h1 <= 0 && h2 <= 0 && h3 <= 0) || (h1 >= 0 && h2 >= 0 && h3 >= 0))
				{
					float BC = std::abs((p.x - c.x) * (b.z - c.z) - (b.x - c.x) * (p.z - c.z)) * 0.5f;
					float AC = std::abs((p.x - c.x) * (a.z - c.z) - (a.x - c.x) * (p.z - c.z)) * 0.5f;
					float AB = std::abs((p.x - b.x) * (a.z - b.z) - (a.x - b.x) * (p.z - b.z)) * 0.5f;

					field_y = (BC * a.y + AC * b.y + AB * c.y) / (std::abs((a.x - c.x) * (b.z - c.z) - (b.x - c.x) * (a.z - c.z)) * 0.5f);
					out = true;

					break;
				}
			}

			if (out)
				break;
		}

		return field_y;
	}

	static bool SphereAndCapsule(DirectX::XMFLOAT3& sphere_pos, float& sphere_radius, DirectX::XMFLOAT3& capsule_start_pos, DirectX::XMFLOAT3& capsule_end_pos, float& capsule_radius)
	{
		bool hit = false;
		float distance2 = 0;

		DirectX::XMFLOAT3 segment_css(sphere_pos.x - capsule_start_pos.x, sphere_pos.y - capsule_start_pos.y, sphere_pos.z - capsule_start_pos.z);
		DirectX::XMFLOAT3 segment_sce(capsule_end_pos.x - sphere_pos.x, capsule_end_pos.y - sphere_pos.y, capsule_end_pos.z - sphere_pos.z);
		DirectX::XMFLOAT3 segment_sub(capsule_end_pos.x - capsule_start_pos.x, capsule_end_pos.y - capsule_start_pos.y, capsule_end_pos.z - capsule_start_pos.z);
		float dot_subcss = 0;
		DirectX::XMStoreFloat(&dot_subcss, DirectX::XMVector3Dot(DirectX::XMLoadFloat3(&segment_sub), DirectX::XMLoadFloat3(&segment_css)));
		float dot_subsce = 0;
		DirectX::XMStoreFloat(&dot_subsce, DirectX::XMVector3Dot(DirectX::XMLoadFloat3(&segment_sub), DirectX::XMLoadFloat3(&segment_sce)));
		if (dot_subcss < epsilon)
		{
			DirectX::XMStoreFloat(&distance2, DirectX::XMVector3Dot(DirectX::XMLoadFloat3(&segment_css), DirectX::XMLoadFloat3(&segment_css)));
		}
		else if (dot_subsce < epsilon)
		{
			DirectX::XMStoreFloat(&distance2, DirectX::XMVector3Dot(DirectX::XMLoadFloat3(&segment_sce), DirectX::XMLoadFloat3(&segment_sce)));
		}
		else
		{
			DirectX::XMFLOAT3 CP;
			DirectX::XMStoreFloat3(&CP, DirectX::XMVector3Cross(DirectX::XMLoadFloat3(&segment_sub), DirectX::XMLoadFloat3(&segment_sce)));
			float dot_CP;
			DirectX::XMStoreFloat(&dot_CP, DirectX::XMVector3Dot(DirectX::XMLoadFloat3(&CP), DirectX::XMLoadFloat3(&CP)));
			float dot_segment;
			DirectX::XMStoreFloat(&dot_segment, DirectX::XMVector3Dot(DirectX::XMLoadFloat3(&segment_sub), DirectX::XMLoadFloat3(&segment_sub)));

			distance2 = dot_CP / dot_segment;
		}

		float radius = sphere_radius + capsule_radius;
		if (!(distance2 > (radius * radius)))
		{
			hit = true;
		}

		return hit;
	}
};