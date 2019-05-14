#include "..\framework\debug.h"
#include "..\ImGui\imgui.h"
#include "..\ImGui\imgui_impl_dx11.h"

#include "..\framework\game.h"
#include "..\framework\scene.h"
#include "..\graphics\directx11.h"
#include "..\scene\title.h"
#include "..\scene\stage\forest.h"
#include "..\scene\result.h"
#include "..\input\keyboard.h"

void Debug::Init(HWND hwnd)
{
	const auto& graphics = Game::GetInstance()->GetGraphics();

	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.WantCaptureMouse = true;

	ImGui_ImplDX11_Init(hwnd, graphics->device_, graphics->device_context_);

	ImGui::StyleColorsDark();

	ImGui::PushStyleVar(ImGuiStyleVar_::ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_::ImGuiStyleVar_FrameRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_::ImGuiStyleVar_FrameBorderSize, 1.0f);

	ImVec4 primary = ImVec4((float)0x1c / 255, (float)0xfe / 255, (float)0xff / 255, (float)0xcc / 255);
	ImVec4 primary_a99 = ImVec4((float)0x1c / 255, (float)0xfe / 255, (float)0xff / 255, (float)0x99 / 255);
	ImVec4 primary_a66 = ImVec4((float)0x1c / 255, (float)0xfe / 255, (float)0xff / 255, (float)0x66 / 255);
	ImVec4 primary_a33 = ImVec4((float)0x1c / 255, (float)0xfe / 255, (float)0xff / 255, (float)0x33 / 255);
	ImVec4 primary_dark = ImVec4((float)0x03 / 255, (float)0x19 / 255, (float)0x1a / 255, (float)0xbb / 255);

	ImVec4 accent = ImVec4((float)0xff / 255, (float)0xa1 / 255, (float)0x00 / 255, (float)0xee / 255);
	ImVec4 accent_acc = ImVec4((float)0xff / 255, (float)0xa1 / 255, (float)0x00 / 255, (float)0xcc / 255);
	ImVec4 accent_a99 = ImVec4((float)0xff / 255, (float)0xa1 / 255, (float)0x00 / 255, (float)0x99 / 255);

	ImVec4 white = ImVec4((float)0xdd / 255, (float)0xdd / 255, (float)0xdd / 255, (float)0xcc / 255);
	ImVec4 black_a55 = ImVec4((float)0x11 / 255, (float)0x11 / 255, (float)0x11 / 255, (float)0x55 / 255);

	ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_MenuBarBg, primary_a33);
	ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_TitleBg, primary_dark);
	ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_TitleBgCollapsed, primary_dark);
	ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_TitleBgActive, primary_a99);
	ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_WindowBg, primary_dark);
	ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_Border, primary_a99);
	ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_FrameBg, primary_a33);
	ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_FrameBgHovered, accent_acc);
	ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_FrameBgActive, accent);
	ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_ScrollbarBg, primary_a33);
	ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_ScrollbarGrab, primary_a99);
	ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_ScrollbarGrabHovered, primary_a99);
	ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_ScrollbarGrabActive, primary);
	ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_CheckMark, accent);
	ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_SliderGrab, primary_a99);
	ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_SliderGrabActive, primary);
	ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_Button, primary_a33);
	ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_ButtonHovered, accent_acc);
	ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_ButtonActive, accent);
	ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_Header, accent_a99);
	ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_HeaderHovered, accent_acc);
	ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_HeaderActive, accent);
	ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_Column, black_a55);
	ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_ColumnHovered, accent_acc);
	ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_ColumnActive, accent);
	ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_PlotLines, primary_a99);
	ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_PlotLinesHovered, primary);
	ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_PlotHistogram, primary_a99);
	ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_PlotHistogramHovered, primary);
	ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_Text, primary);
	ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_TextDisabled, primary_a66);
	ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_TextSelectedBg, accent);
	ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_PopupBg, primary_dark);
}

void Debug::Uninit()
{
	ImGui::PopStyleVar();
	ImGui::PopStyleVar();
	ImGui::PopStyleVar();

	ImGui::PopStyleColor();
	ImGui::PopStyleColor();
	ImGui::PopStyleColor();
	ImGui::PopStyleColor();
	ImGui::PopStyleColor();
	ImGui::PopStyleColor();
	ImGui::PopStyleColor();
	ImGui::PopStyleColor();
	ImGui::PopStyleColor();
	ImGui::PopStyleColor();
	ImGui::PopStyleColor();
	ImGui::PopStyleColor();
	ImGui::PopStyleColor();
	ImGui::PopStyleColor();
	ImGui::PopStyleColor();
	ImGui::PopStyleColor();
	ImGui::PopStyleColor();
	ImGui::PopStyleColor();
	ImGui::PopStyleColor();
	ImGui::PopStyleColor();
	ImGui::PopStyleColor();
	ImGui::PopStyleColor();
	ImGui::PopStyleColor();
	ImGui::PopStyleColor();
	ImGui::PopStyleColor();
	ImGui::PopStyleColor();
	ImGui::PopStyleColor();
	ImGui::PopStyleColor();
	ImGui::PopStyleColor();
	ImGui::PopStyleColor();
	ImGui::PopStyleColor();
	ImGui::PopStyleColor();
	ImGui::PopStyleColor();

	ImGui_ImplDX11_Shutdown();
	ImGui::DestroyContext();
}

void Debug::Begin()
{
	ImGui_ImplDX11_NewFrame();
}

void Debug::End()
{
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	ImGui::EndFrame();
}

void Debug::DrawLight(DirectX::XMFLOAT4& light, DirectX::XMFLOAT4& ambient, DirectX::XMFLOAT4& diffuse, DirectX::XMFLOAT4& specular)
{
	ImGui::SetNextWindowPos(ImVec2(10, 110), ImGuiSetCond_Once);
	ImGui::SetNextWindowSize(ImVec2(300, 125), ImGuiSetCond_Once);
	ImGui::Begin("Light");
	ImGui::DragFloat3("light", &light.x, 0.1f);
	ImGui::DragFloat4("ambiend", &ambient.x, 0.1f);
	ImGui::DragFloat4("diffuse", &diffuse.x, 0.1f);
	ImGui::DragFloat4("specular", &specular.x, 0.1f);
	ImGui::End();
}

void Debug::DrawCameraTPS(DirectX::XMFLOAT3& eye, DirectX::XMFLOAT3& at, DirectX::XMFLOAT3& up, DirectX::XMFLOAT3& pos, DirectX::XMFLOAT2& rot, bool& free)
{
	ImGui::SetNextWindowPos(ImVec2(10, 245), ImGuiSetCond_Once);
	ImGui::SetNextWindowSize(ImVec2(300, 195), ImGuiSetCond_Once);
	ImGui::Begin("CameraTPS");
	ImGui::DragFloat3("eye", &eye.x, 0.1f);
	ImGui::DragFloat3("at", &at.x, 0.1f);
	ImGui::DragFloat3("up", &up.x, 0.1f);
	ImGui::DragFloat3("position", &pos.x, 0.1f);
	ImGui::SliderAngle("rotation X", &rot.x);
	ImGui::SliderAngle("rotation Y", &rot.y);
	ImGui::Checkbox("FreeMode", &free);
	ImGui::End();
}

void Debug::DrawObject(std::string object, DirectX::XMFLOAT3& pos)
{
	ImGui::SetNextWindowPos(ImVec2(10, 450), ImGuiSetCond_Once);
	ImGui::SetNextWindowSize(ImVec2(410, 250), ImGuiSetCond_Once);
	ImGui::Begin("Object");
	ImGui::SetNextTreeNodeOpen(false, ImGuiSetCond_Once);
	if (ImGui::TreeNode(object.c_str()))
	{
		ImGui::DragFloat3("position", &pos.x, 0.1f);
		ImGui::TreePop();
	}
	ImGui::End();
}

void Debug::DrawDepth(void* shader_resource_view, float width, float height)
{
	ImGui::SetNextWindowPos(ImVec2(430, 10), ImGuiSetCond_Once);
	ImGui::SetNextWindowSize(ImVec2(width, width + 35), ImGuiSetCond_Once);
	ImGui::Begin("Depth");
	ImGui::Image(shader_resource_view, ImVec2(width, height));
	ImGui::End();
}

void Debug::DrawInput()
{
	ImGui::SetNextWindowPos(ImVec2(320, 10), ImGuiSetCond_Once);
	ImGui::SetNextWindowSize(ImVec2(100, 430), ImGuiSetCond_Once);
	ImGui::Begin("Input");
	bool keyboard[8];
	keyboard[0] = KeyBoard::Press('W');
	keyboard[1] = KeyBoard::Press('S');
	keyboard[2] = KeyBoard::Press('A');
	keyboard[3] = KeyBoard::Press('D');
	keyboard[4] = KeyBoard::Press(VK_UP);
	keyboard[5] = KeyBoard::Press(VK_DOWN);
	keyboard[6] = KeyBoard::Press(VK_LEFT);
	keyboard[7] = KeyBoard::Press(VK_RIGHT);

	ImGui::SetNextTreeNodeOpen(true, ImGuiSetCond_Once);
	if (ImGui::TreeNode("Movement"))
	{
		ImGui::Checkbox("front", &keyboard[0]);
		ImGui::Checkbox("back", &keyboard[1]);
		ImGui::Checkbox("left", &keyboard[2]);
		ImGui::Checkbox("right", &keyboard[3]);
		ImGui::TreePop();
	}

	ImGui::SetNextTreeNodeOpen(true, ImGuiSetCond_Once);
	if (ImGui::TreeNode("Camera"))
	{
		ImGui::Checkbox("up", &keyboard[4]);
		ImGui::Checkbox("down", &keyboard[5]);
		ImGui::Checkbox("left", &keyboard[6]);
		ImGui::Checkbox("right", &keyboard[7]);
		ImGui::TreePop();
	}
	ImGui::End();
}

void Debug::DrawWindow(float* color)
{
	ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiSetCond_Once);
	ImGui::SetNextWindowSize(ImVec2(300, 90), ImGuiSetCond_Once);
	ImGui::Begin("Window");
	ImGui::Text("DisplaySize: %.f x %.f", ImGui::GetIO().DisplaySize.x, ImGui::GetIO().DisplaySize.y);
	ImGui::Text("fps: %.f(%.3fms/frame)", ImGui::GetIO().Framerate, 1000.0f / ImGui::GetIO().Framerate);
	ImGui::ColorEdit3("ClearColor", color);
	ImGui::End();

	if (KeyBoard::Trigger('1'))
		Game::GetInstance()->SetScene<Title>();
	else if (KeyBoard::Trigger('2'))
		Game::GetInstance()->SetScene<Forest>();
	else if (KeyBoard::Trigger('3'))
		Game::GetInstance()->SetScene<Result>();
}