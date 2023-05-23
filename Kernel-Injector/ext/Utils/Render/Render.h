#pragma once
#include"ImGui/imgui.h"
#include"ImGui/imgui_impl_dx9.h"
#include"ImGui/imgui_impl_win32.h"

#include<d3d9.h>
#include <dinput.h>
#include <tchar.h>
#include"../../Protection/lazy_importer.h"
#include <thread>

	static LPCWSTR WindowClass = L"WINDOW CLASS";
	static LPCWSTR WindowName = L"WINDOW NAME";
	static HWND hWnd = NULL;
	static LPDIRECT3D9		g_pD3D = NULL;
	static LPDIRECT3DDEVICE9 g_pd3dDevice = NULL;
	static D3DPRESENT_PARAMETERS g_d3dpp = {};



class Window
{
public:
	 int WindowHeight = 200;
	 int WindowWidth = 200;
	 bool CloseButton = true;
public:
	__declspec() void DrawWindow(LPCWSTR lpWindowTitle);
}; Window* pWindow = new Window();


HRESULT CreateDevice(HWND hWnd)
{
	if ((g_pD3D = Direct3DCreate9(D3D_SDK_VERSION)) == NULL) return E_FAIL;

	ZeroMemory(&g_d3dpp, sizeof(g_d3dpp));
	g_d3dpp.Windowed = TRUE;
	g_d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	g_d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
	g_d3dpp.EnableAutoDepthStencil = TRUE;
	g_d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
	//g_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_ONE;           // Present with vsync
	g_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;   // Present without vsync, maximum unthrottled framerate
	if (g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &g_d3dpp, &g_pd3dDevice) < 0) return E_FAIL;

	return S_OK;
}


void __stdcall CleanUpDevice()
{
	if (g_pd3dDevice) { g_pd3dDevice->Release(); g_pd3dDevice = NULL; }
	if (g_pD3D) { g_pD3D->Release(); g_pD3D = NULL; }
}

void __stdcall ResetDevice()
{
	ImGui_ImplDX9_InvalidateDeviceObjects();
	HRESULT result = g_pd3dDevice->Reset(&g_d3dpp);
	if (result == D3DERR_INVALIDCALL) IM_ASSERT(0);
	ImGui_ImplDX9_CreateDeviceObjects();
}

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT WINAPI WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lpParam)
{
	switch (uMsg)
	{
	case WM_SIZE:
		if (g_pd3dDevice != NULL && wParam != SIZE_MINIMIZED)
		{
			g_d3dpp.BackBufferWidth = LOWORD(lpParam);
			g_d3dpp.BackBufferHeight = HIWORD(lpParam);
			ResetDevice();
		}
		return 0;
	case WM_SYSCOMMAND:
		if ((wParam & 0xfff0) == SC_KEYMENU)return 0;
		break;
	case WM_NCHITTEST:
	{
		ImVec2 Mouse = ImGui::GetMousePos();
		if (Mouse.y < 25 && Mouse.x < pWindow->WindowWidth - 25)
		{
			LRESULT hit = DefWindowProc(hWnd, uMsg, wParam, lpParam);
			if (hit == HTCLIENT) hit = HTCAPTION;
			return hit;
		}
		else break;
	}
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	default:
		ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lpParam);
		return DefWindowProc(hWnd, uMsg, wParam, lpParam);
	}
	return DefWindowProc(hWnd, uMsg, wParam, lpParam);
}
