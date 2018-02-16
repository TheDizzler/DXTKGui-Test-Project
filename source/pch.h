#pragma once
/* Global variables and includes */
#include <Windows.h>
#include <Dbt.h>
#include <wrl.h>
#include <comdef.h>
#include <algorithm> 
#include <cctype>
#include <sstream>
#include <vector>
#include <list>
#include <deque>
#include <WinSDKVer.h>		// these are necessary for XAudio2.8
#define _WIN32_WINNT 0x0602	// ^
#include <SDKDDKVer.h>		// ^

#pragma comment (lib, "D3D11.lib")
#pragma comment(lib, "DXGI.lib")

#include <d3d11_1.h>
#include <dxgi1_2.h>
#include <SpriteBatch.h>
#include <Audio.h>
#include <pugixml.hpp>

#include "../DXTKGui/GUIFactory.h"
#include "Engine/Input.h"

using namespace std;
using namespace pugi;
using namespace DirectX;
using namespace Microsoft::WRL;