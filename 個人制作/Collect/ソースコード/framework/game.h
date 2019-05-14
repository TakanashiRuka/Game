#pragma once

#include "..\framework\window.h"
#include "..\graphics\directx11.h"
#include "..\framework\scene.h"
#include "..\framework\debug.h"
#include "..\sound\x_audio2.h"

class Game
{
private:
	Window* window_ = nullptr;
	Graphics* graphics_ = nullptr;
	Scene* current_scene_ = nullptr;
	Scene* next_scene_ = nullptr;

	unsigned int fps_ = 0;
	double running_time_ = 0;

	bool loop_ = true;
	bool pause_ = false;
	bool debug_ = false;

private:
	Game() = default;
	~Game() = default;

public:
	Game(const Game&) = delete;
	Game& operator=(const Game&) = delete;
	Game(Game&&) = delete;
	Game& operator=(Game&&) = delete;

public:
	template<class T>
	void Begin(const char* name, unsigned int fps);
	void Loop();
	void End();

public:
	template<class T>
	void SetScene();

public:
	Graphics* const GetGraphics();
	double GetRunningTime();
	void Quit();
	void Pause(bool pause);

public:
	static Game* const GetInstance();
};

template<class T>
void Game::Begin(const char* name, unsigned int fps)
{
		this->window_ = new Window(name);
		this->graphics_ = new Graphics(Window::GetWidth(), Window::GetHeight(), fps, this->window_->GetHwnd());
		XAudio2::Begin();
		this->next_scene_ = new T;
		this->fps_ = fps;

		Debug::Init(this->window_->GetHwnd());
#ifdef _DEBUG
		this->debug_ = true;
#endif // _DEBUG
}

template<class T>
void Game::SetScene()
{
	delete this->next_scene_;
	XAudio2::SourceVoicesDestroy();
	this->next_scene_ = new T;
}