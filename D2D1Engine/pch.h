#pragma once
#pragma warning(disable: 4244 4098 4099 4996)

//추가 예정 기능
//Particle System
//Text Alignment
//Layer-Based Collision Detection


#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#ifndef DIRECTINPUT_VERSION
#define DIRECTINPUT_VERSION 0x0800
#endif
using Radian = float;
using Degree = float;

#define C_STR_BUFFER_SIZE	256

//엔진에서 사용할 태그 설정
enum Tag {
	None,
	TagPlayer,
	TagEnemy,
	TagBullet,
	TagGround,
	TagNexus,
	TagTower,
	TagTemple,
	TagWBarrack,
	TagABarrack,
	TagGBarrack,
	TagWarrior,
	TagArcher,
	TagGuardian,
	TagUpgrade,
	TagPDown,
	TagAddWarrior,
	TagAddArcher,
	TagAddGurdion,
	TagAttackTarget,
	TagChangeBuilding,
};
//엔진에서 사용할 사운드 ID 설정
enum SoundID {
	SIDIntro,
	SIDBgm,
	SIDShoot,
};

// deltatime
#define DT ((float)(App::GetInstance()->GetTimer()->GetElapsedTime()/1000.f))

#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC

#endif
#include<crtdbg.h>
#ifdef _DEBUG
#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)
#endif
#include<Windows.h>
#include<assert.h>
#include<iostream>
#include<algorithm>
#include<functional>
#include<list>
#include<vector>
#include<map>
#include<unordered_map>
#include<random>
#include<typeindex>
#include<variant>
#include<string>
#include<iomanip>
#include<filesystem>
#include<codecvt>
#include<exception>
#include<xaudio2.h>
//#include<ogg\ogg.h>
//#include<ogg\os_types.h>
//#include<vorbis\codec.h>
//#include<vorbis\vorbisenc.h>
//#include<vorbis\vorbisfile.h>
#include<d2d1.h>
#include<d2d1effects.h>
#include<d2d1_1helper.h>
#include<d3d11.h>
#include<dxgi1_2.h>
#include<dwrite.h>
#include<dinput.h>
#include<wincodec.h>
#include<codec.h>
#include<vorbisfile.h>
#include<mmsystem.h>
#include<dsound.h>
#include"App.h"





template <typename T>
inline void SafeDelete(T*& p)
{
	delete p;
	p = static_cast<T *>(0);
};
namespace DX
{
	inline void ThrowIfFailed(HRESULT hr)
	{
		if (FAILED(hr))
		{
			// Set a breakpoint on this line to catch DirectX API errors
			throw std::exception();
		}
	}
}

inline HRESULT Trace(const std::string& file, DWORD line, HRESULT hr, const std::string& msg)
{
	std::cout << "File : " << file << "\nLine : " << std::to_string(line) << "\nResult : 0x" << std::setfill('0') << std::setw(8) << std::hex << hr << "\nMessage : " << msg << "\n";
	return hr;
}
//
#define HR(hr) if(FAILED(hr)) Trace(__FILE__, __LINE__, hr, "Error")
#define HR_MSG(hr, msg) if(FAILED(hr)) Trace(__FILE__, __LINE__, hr, msg)

#ifndef _DEBUG
template<class Facet>
struct deletable_facet : Facet
{
	template<class ...Args>
	deletable_facet(Args&& ...args) : Facet(std::forward<Args>(args)...) {}
	~deletable_facet() {}
};
#endif

inline std::wstring Widen(const std::string& str) 
{

#ifdef _DEBUG
	wchar_t wstr[C_STR_BUFFER_SIZE];
	MultiByteToWideChar(CP_ACP, NULL, str.c_str(), -1, wstr, C_STR_BUFFER_SIZE);
	return wstr;
#else
	typedef deletable_facet<std::codecvt_byname<wchar_t, char, std::mbstate_t>>
		local_facet_t; std::wstring_convert<local_facet_t> wconv(new local_facet_t(""));
	return wconv.from_bytes(str);
#endif
	
}



