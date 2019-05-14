#pragma once

#include <xaudio2.h>

#include <unordered_map>
#include <string>

enum class SoundType
{
	BGM,
	SE,
};

class XAudio2
{
	friend class Game;

private:
	static IXAudio2* x_audio2_;
	static IXAudio2MasteringVoice* mastering_voice_;
	static std::unordered_map<std::string, XAUDIO2_BUFFER> xaudio2_buffer_;
	static std::unordered_map<std::string, IXAudio2SourceVoice*> source_voices_;

public:
	static void AddSourceVoice(std::string name, SoundType type);
	static void StartSourceVoice(std::string name);
	static void StopSourceVoice(std::string name);

private:
	static void Begin();
	static void End();
	static void SourceVoicesDestroy();
};