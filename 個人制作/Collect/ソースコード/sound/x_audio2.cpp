#include "..\sound\x_audio2.h"

#include "..\framework\utility.h"

#pragma comment(lib, "xaudio2.lib")

IXAudio2* XAudio2::x_audio2_ = nullptr;
IXAudio2MasteringVoice* XAudio2::mastering_voice_ = nullptr;
std::unordered_map<std::string, XAUDIO2_BUFFER> XAudio2::xaudio2_buffer_;
std::unordered_map<std::string, IXAudio2SourceVoice*> XAudio2::source_voices_;

constexpr DWORD FFIR = 'FFIR';
constexpr DWORD tmf = ' tmf';
constexpr DWORD atad = 'atad';

constexpr char* sound_file_path = "data/sound/";
constexpr char* sound_file_format = ".wav";

HRESULT FindChunk(HANDLE hFile, DWORD fourcc, DWORD& dwChunkSize, DWORD& dwChunkDataPosition)
{
	HRESULT hr = S_OK;
	if (INVALID_SET_FILE_POINTER == SetFilePointer(hFile, 0, NULL, FILE_BEGIN))
		return HRESULT_FROM_WIN32(GetLastError());

	DWORD dwChunkType;
	DWORD dwChunkDataSize;
	DWORD dwRIFFDataSize = 0;
	DWORD dwFileType;
	DWORD bytesRead = 0;
	DWORD dwOffset = 0;

	while (hr == S_OK)
	{
		DWORD dwRead;
		if (0 == ReadFile(hFile, &dwChunkType, sizeof(DWORD), &dwRead, NULL))
			hr = HRESULT_FROM_WIN32(GetLastError());

		if (0 == ReadFile(hFile, &dwChunkDataSize, sizeof(DWORD), &dwRead, NULL))
			hr = HRESULT_FROM_WIN32(GetLastError());

		switch (dwChunkType)
		{
		case FFIR:
			dwRIFFDataSize = dwChunkDataSize;
			dwChunkDataSize = 4;
			if (0 == ReadFile(hFile, &dwFileType, sizeof(DWORD), &dwRead, NULL))
				hr = HRESULT_FROM_WIN32(GetLastError());
			break;

		default:
			if (INVALID_SET_FILE_POINTER == SetFilePointer(hFile, dwChunkDataSize, NULL, FILE_CURRENT))
				return HRESULT_FROM_WIN32(GetLastError());
		}

		dwOffset += sizeof(DWORD) * 2;

		if (dwChunkType == fourcc)
		{
			dwChunkSize = dwChunkDataSize;
			dwChunkDataPosition = dwOffset;
			return S_OK;
		}

		dwOffset += dwChunkDataSize;
		if (bytesRead >= dwRIFFDataSize) return S_FALSE;
	}

	return S_OK;
}

HRESULT ReadChunkData(HANDLE hFile, void* buffer, DWORD buffersize, DWORD bufferoffset)
{
	HRESULT hr = S_OK;
	if (INVALID_SET_FILE_POINTER == SetFilePointer(hFile, bufferoffset, NULL, FILE_BEGIN))
		return HRESULT_FROM_WIN32(GetLastError());
	DWORD dwRead;
	if (0 == ReadFile(hFile, buffer, buffersize, &dwRead, NULL))
		hr = HRESULT_FROM_WIN32(GetLastError());
	return hr;
}

void XAudio2::AddSourceVoice(std::string name, SoundType type)
{
	if (!XAudio2::source_voices_[name])
	{
		std::string file_path = sound_file_path;
		switch (type)
		{
		case SoundType::BGM:
		{
			file_path += "bgm/" + name + sound_file_format;
		}
		break;

		case SoundType::SE:
		{
			file_path += "se/" + name + sound_file_format;
		}
		break;
		}

		HANDLE file;
		file = CreateFile(file_path.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
		SetFilePointer(file, 0, NULL, FILE_BEGIN);

		DWORD chunk_size = 0;
		DWORD chunk_position = 0;
		FindChunk(file, FFIR, chunk_size, chunk_position);
		DWORD file_type;
		ReadChunkData(file, &file_type, sizeof(DWORD), chunk_position);

		FindChunk(file, tmf, chunk_size, chunk_position);
		WAVEFORMATEXTENSIBLE wfe = { 0 };
		ReadChunkData(file, &wfe, chunk_size, chunk_position);

		FindChunk(file, atad, chunk_size, chunk_position);
		BYTE* data_buffer = new BYTE[chunk_size];
		ReadChunkData(file, data_buffer, chunk_size, chunk_position);

		ReadChunkData(file, &wfe, chunk_size, chunk_position);
		XAudio2::x_audio2_->CreateSourceVoice(&XAudio2::source_voices_[name], &(wfe.Format));

		XAudio2::xaudio2_buffer_[name].AudioBytes = chunk_size;
		XAudio2::xaudio2_buffer_[name].pAudioData = data_buffer;
		XAudio2::xaudio2_buffer_[name].Flags = XAUDIO2_END_OF_STREAM;
		switch (type)
		{
		case SoundType::BGM:
		{
			XAudio2::xaudio2_buffer_[name].LoopCount = XAUDIO2_LOOP_INFINITE;
		}
		break;

		case SoundType::SE:
		{
			XAudio2::xaudio2_buffer_[name].LoopCount = XAUDIO2_NO_LOOP_REGION;
		}
		break;
		}
		XAudio2::source_voices_[name]->SubmitSourceBuffer(&XAudio2::xaudio2_buffer_[name]);
	}
}

void XAudio2::StartSourceVoice(std::string name)
{
	XAudio2::StopSourceVoice(name);
	XAudio2::source_voices_[name]->Start();
}

void XAudio2::StopSourceVoice(std::string name)
{
	XAudio2::source_voices_[name]->Stop();
	XAudio2::source_voices_[name]->FlushSourceBuffers();
	XAudio2::source_voices_[name]->SubmitSourceBuffer(&XAudio2::xaudio2_buffer_[name]);
}

void XAudio2::Begin()
{
	CoInitializeEx(nullptr, COINITBASE_MULTITHREADED);
	XAudio2Create(&XAudio2::x_audio2_, 0U);
	XAudio2::x_audio2_->CreateMasteringVoice(&XAudio2::mastering_voice_);
}

void XAudio2::End()
{
	XAudio2::SourceVoicesDestroy();

	if (XAudio2::mastering_voice_ != nullptr)
	{
		XAudio2::mastering_voice_->DestroyVoice();
		XAudio2::mastering_voice_ = nullptr;
	}
	SafeRelease(XAudio2::x_audio2_);
	CoUninitialize();
}

void XAudio2::SourceVoicesDestroy()
{
	for (auto& source_voice : XAudio2::source_voices_)
	{
		if (source_voice.second)
		{
			source_voice.second->Stop();
			source_voice.second->DestroyVoice();
			source_voice.second = nullptr;
		}
	}

	for (auto& xb : XAudio2::xaudio2_buffer_)
	{
		if (xb.second.pAudioData)
		{
			delete xb.second.pAudioData;
			xb.second.pAudioData = nullptr;
		}
	}
}