#pragma once
#include "Includes.h"
#include "windowfinder.h"
#include <dwmapi.h>

void Theme()
{
    ImGuiStyle* style = &ImGui::GetStyle();

    ImGui::GetStyle().WindowMenuButtonPosition = ImGuiDir_None;

    style->WindowPadding = ImVec2(8, 8);
    style->FramePadding = ImVec2(4, 2);
    style->ItemSpacing = ImVec2(6, 4);
    style->ItemInnerSpacing = ImVec2(6, 4);
    style->IndentSpacing = 25.0f;
    style->ScrollbarSize = 18.0f;
    style->GrabMinSize = 14.0f;
    style->GrabRounding = 3.0f;
    style->WindowBorderSize = 0.0f;
    style->ChildBorderSize = 1.0f;
    style->PopupBorderSize = 1.0f;
    style->FrameBorderSize = 0.0f;
    style->WindowRounding = 4.0f;
    style->ChildRounding = 4.0f;
    style->FrameRounding = 4.0f;
    style->PopupRounding = 2.0f;
    style->ScrollbarRounding = 3.0f;
    style->GrabRounding = 2.0f;

    style->FramePadding = ImVec2(8, 6);

    style->Colors[ImGuiCol_TitleBg] = ImVec4(0.09f, 0.09f, 0.09f, 1.00f);
    style->Colors[ImGuiCol_TitleBgActive] = ImVec4(0.13f, 0.13f, 0.13f, 1.00f);
    style->Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.09f, 0.09f, 0.09f, 1.00f);

    style->ChildRounding = (5);
    style->Colors[ImGuiCol_Border] = ImVec4(0.13f, 0.13f, 0.13f, 1.00f);

    style->Colors[ImGuiCol_SliderGrab] = ImVec4(0.24f, 0.40f, 0.95f, 1.00f);
    style->Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);

    style->Colors[ImGuiCol_Button] = ImVec4(0.24f, 0.40f, 0.95f, 1.00f);
    style->Colors[ImGuiCol_ButtonActive] = ImColor(0, 100, 200, 255);
    style->Colors[ImGuiCol_ButtonHovered] = ImColor(0, 140, 230, 255);

    style->Colors[ImGuiCol_CheckMark] = ImVec4(0.24f, 0.40f, 0.95f, 1.00f);

    style->Colors[ImGuiCol_WindowBg] = ImVec4(0.06f, 0.06f, 0.06f, 0.94f);

    style->Colors[ImGuiCol_ChildBg] = ImVec4(0.15f, 0.15f, 0.15f, 1.00f);

    style->Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.24f, 0.40f, 0.95f, 1.00f);
    style->Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.33f, 0.33f, 0.33f, 1.00f);
    style->Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.24f, 0.40f, 0.95f, 1.00f);

    style->Colors[ImGuiCol_Separator] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
    style->Colors[ImGuiCol_SeparatorActive] = ImVec4(0.24f, 0.40f, 0.95f, 1.00f);
    style->Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.24f, 0.52f, 0.88f, 1.00f);


    style->Colors[ImGuiCol_FrameBg] = ImVec4(0.24f, 0.23f, 0.23f, 1.00f);
    style->Colors[ImGuiCol_FrameBgActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
    style->Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.56f, 0.56f, 0.58f, 0.50f);

    style->Colors[ImGuiCol_Header] = ImVec4(0.24f, 0.23f, 0.23f, 1.00f);
    style->Colors[ImGuiCol_HeaderActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
    style->Colors[ImGuiCol_HeaderHovered] = ImVec4(0.56f, 0.56f, 0.58f, 0.50f);

    style->Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.24f, 0.52f, 0.88f, 0.35f);
    style->Colors[ImGuiCol_ResizeGrip] = ImVec4(0.24f, 0.52f, 0.88f, 0.50f);
    style->Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.24f, 0.52f, 0.88f, 0.67f);
    style->Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.24f, 0.52f, 0.88f, 0.95f);
    style->Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.24f, 0.52f, 0.88f, 1.00f);

}

static ID3D11Device* g_pd3dDevice = nullptr;
static ID3D11DeviceContext* g_pd3dDeviceContext = nullptr;
static IDXGISwapChain* g_pSwapChain = nullptr;
static ID3D11RenderTargetView* g_mainRenderTargetView = nullptr;
static ID3D11BlendState* g_blendstate = nullptr;

void CreateRenderTarget()
{
    ID3D11Texture2D* pBackBuffer;
    g_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
    g_pd3dDevice->CreateRenderTargetView(pBackBuffer, NULL, &g_mainRenderTargetView);
    pBackBuffer->Release();
}

void CleanupRenderTarget()
{
    if (g_mainRenderTargetView) { g_mainRenderTargetView->Release(); g_mainRenderTargetView = NULL; }
}

void CleanupDeviceD3D()
{
    CleanupRenderTarget();
    if (g_pSwapChain) { g_pSwapChain->Release(); g_pSwapChain = NULL; }
    if (g_pd3dDeviceContext) { g_pd3dDeviceContext->Release(); g_pd3dDeviceContext = NULL; }
    if (g_pd3dDevice) { g_pd3dDevice->Release(); g_pd3dDevice = NULL; }
}

bool CreateDeviceD3D(HWND hWnd)
{
    DXGI_SWAP_CHAIN_DESC sd;
    ZeroMemory(&sd, sizeof(DXGI_SWAP_CHAIN_DESC));
    sd.BufferCount = 2;
    sd.BufferDesc.Width = 0;
    sd.BufferDesc.Height = 0;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = hWnd;
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.Windowed = TRUE;
    sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

    UINT createDeviceFlags = 0;

    D3D_FEATURE_LEVEL featureLevel;
    const D3D_FEATURE_LEVEL featureLevelArray[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0, };


    if (D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &featureLevel, &g_pd3dDeviceContext) != S_OK)
        return false;

    D3D11_BLEND_DESC desc;
    ZeroMemory(&desc, sizeof(desc));
    desc.AlphaToCoverageEnable = false;
    desc.RenderTarget[0].BlendEnable = true;
    desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
    desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
    desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
    desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_INV_DEST_ALPHA; 
    desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE; 
    desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
    desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
    g_pd3dDevice->CreateBlendState(&desc, &g_blendstate);

    CreateRenderTarget();

    return true;
}

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
        return true;

    switch (msg)
    {
    case WM_SIZE:
        if (g_pd3dDevice != NULL && wParam != SIZE_MINIMIZED)
        {
            CleanupRenderTarget();
            g_pSwapChain->ResizeBuffers(0, (UINT)LOWORD(lParam), (UINT)HIWORD(lParam), DXGI_FORMAT_UNKNOWN, 0);
            CreateRenderTarget();
        }
        return 0;
    case WM_SYSCOMMAND:
        if ((wParam & 0xfff0) == SC_KEYMENU)
            return 0;
        break;
    case WM_CLOSE:
        g_pSwapChain->Release();
        g_pd3dDevice->Release();
        ExitProcess(0);
        break;
        

    return 0;
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }
    return ::DefWindowProc(hWnd, msg, wParam, lParam);
}

HWND hwnd;
bool dragging = false;

POINT CurrentWindowPos;
int WindowMoveGap = 20;
namespace Render
{

	inline void Start()
	{
        WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle(NULL), NULL, LoadCursor(NULL, IDC_ARROW), (HBRUSH)GetStockObject(WHITE_BRUSH), NULL, _T("ImGui Example"), NULL };
        ::RegisterClassEx(&wc);
        DWORD style = WS_OVERLAPPEDWINDOW & ~WS_CAPTION;
        HWND hwnd = ::CreateWindow(wc.lpszClassName, _T(""), style, 100, 100, 1600, 900, NULL, NULL, wc.hInstance, NULL);
        if (!CreateDeviceD3D(hwnd))
        {
            CleanupDeviceD3D();
            UnregisterClassA(wc.lpszClassName, wc.hInstance);
            return;
        }

        ShowWindow(GetConsoleWindow(), SW_HIDE);
        ShowWindow(hwnd, SW_SHOWDEFAULT);
        UpdateWindow(hwnd);

        MARGINS margins = { -1 };
        DwmExtendFrameIntoClientArea(hwnd, &margins);

        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        io.Fonts->AddFontFromFileTTF("roboto.ttf", 18);
        io.IniFilename = nullptr;
        io.LogFilename = nullptr;
        io.WantSaveIniSettings = false;

        ImGui_ImplWin32_Init(hwnd);
        ImGui_ImplDX11_Init(g_pd3dDevice, g_pd3dDeviceContext);

        Theme();

        EnumWindows(EnumWindowsProc, 0);

        while (true)
        {
            MSG msg;
            while (PeekMessageA(&msg, NULL, 0U, 0U, PM_REMOVE))
            {
                TranslateMessage(&msg);
                DispatchMessageA(&msg);

                switch (msg.message)
                {
                case WM_QUIT:
                    break;
                }
            }

            RECT rect;
            if (GetWindowRect(hwnd, &rect))
            {
                CurrentWindowPos.x = rect.left;
                CurrentWindowPos.y = rect.top;
            }

            ImGui_ImplDX11_NewFrame();
            ImGui_ImplWin32_NewFrame();
            ImGui::NewFrame();

            WindowFinder(hwnd); // start window finder

            ImGui::Render();
            g_pd3dDeviceContext->OMSetRenderTargets(1, &g_mainRenderTargetView, NULL);
            float clearColor[4] = { 0.0f,0.0f,0.0f,0.0f };
            g_pd3dDeviceContext->ClearRenderTargetView(g_mainRenderTargetView, clearColor);
            ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
            g_pSwapChain->Present(1, 0);
        }
        ImGui_ImplDX11_Shutdown();
        ImGui_ImplWin32_Shutdown();
        ImGui::DestroyContext();
        CleanupDeviceD3D();
        DestroyWindow(hwnd);
        UnregisterClassA(wc.lpszClassName, wc.hInstance);
        return;
	}
}

