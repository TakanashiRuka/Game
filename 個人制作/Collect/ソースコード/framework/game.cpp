#include "..\framework\game.h"
#include "..\input\keyboard.h"
#include "..\input\x_input.h"
#include "..\framework\utility.h"

#include <chrono>
#include <type_traits>

RenderingPass begin(RenderingPass) { return RenderingPass::Depth; }
RenderingPass end(RenderingPass) { return RenderingPass::Max; }
RenderingPass operator*(RenderingPass pass) { return pass; }
RenderingPass operator++(RenderingPass& pass) { return pass = RenderingPass(std::underlying_type<RenderingPass>::type(pass) + 1); }

void Game::Loop()
{
	MSG msg;
	std::chrono::system_clock::time_point start, stop;
	start = std::chrono::system_clock::now();

	while (Game::loop_)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{
				this->Quit();
			}
			else
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{
			stop = std::chrono::system_clock::now();
			double nanoseconds = static_cast<double>(std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start).count()) / 1000000000;

			if (nanoseconds >= (1.0 / this->fps_))
			{
				start = stop;
				this->running_time_ = nanoseconds;

				if (this->current_scene_)
				{
					KeyBoard::Update();
					XInput::Update();

					if (KeyBoard::Trigger(VK_TAB))
					{
						(this->debug_)
							? this->debug_ = false
							: this->debug_ = true;
					}

					(this->pause_)
						? this->current_scene_->Pause()
						: this->current_scene_->Update();
					this->current_scene_->Always();

					for (auto pass : RenderingPass::Max)
					{
						switch (pass)
						{
						case RenderingPass::Depth:
						case RenderingPass::Base:
						case RenderingPass::UnableDepthStencil:
						{
							this->graphics_->Begin(pass);
							this->current_scene_->Draw(pass);
						}
						break;

						case RenderingPass::Debug:
						{
							this->graphics_->Begin(pass);
							if (this->debug_ == true)
							{
								this->current_scene_->Draw(pass);
								this->graphics_->DebugDraw();
							}
						}
						break;
						}
					}
					this->graphics_->End();
				}

				if (this->next_scene_)
				{
					delete this->current_scene_;
					this->current_scene_ = this->next_scene_;
					this->next_scene_ = nullptr;
				}
			}
		}
	}
}

void Game::End()
{
	XAudio2::End();
	Debug::Uninit();
	SafeDelete(this->next_scene_);
	SafeDelete(this->current_scene_);
	SafeDelete(this->graphics_);
	SafeDelete(this->window_);
}

Graphics* const Game::GetGraphics()
{
	return this->graphics_;
}

double Game::GetRunningTime()
{
	return this->running_time_;
}

void Game::Quit()
{
	this->loop_ = false;
}

void Game::Pause(bool pause)
{
	this->pause_ = pause;
}

Game* const Game::GetInstance()
{
	static Game game;

	return &game;
}