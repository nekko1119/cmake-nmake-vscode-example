// !!! THIS FILE IS A COPY OF DIRECTX TUTORIAL SAMPLE !!!

//-----------------------------------------------------------------------------
// File: Vertices.cpp
//
// Desc: In this tutorial, we are rendering some vertices. This introduces the
//       concept of the vertex buffer, a Direct3D object used to store
//       vertices. Vertices can be defined any way we want by defining a
//       custom structure and a custom FVF (flexible vertex format). In this
//       tutorial, we are using vertices that are transformed (meaning they
//       are already in 2D window coordinates) and lit (meaning we are not
//       using Direct3D lighting, but are supplying our own colors).
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//-----------------------------------------------------------------------------
#include <d3d9.h>

#include <array>
#pragma warning(disable : 4996) // disable deprecated warning
#include <strsafe.h>
#pragma warning(default : 4996)

#pragma comment(lib, "d3d9.lib")

//-----------------------------------------------------------------------------
// Global variables
//-----------------------------------------------------------------------------
LPDIRECT3D9 g_p_d3_d = nullptr; // Used to create the D3DDevice
LPDIRECT3DDEVICE9 g_pd3d_device = nullptr; // Our rendering device
LPDIRECT3DVERTEXBUFFER9 g_p_vb = nullptr; // Buffer to hold vertices

// A structure for our custom vertex type
struct CUSTOMVERTEX {
  FLOAT x, y, z, rhw; // The transformed position for the vertex
  DWORD color; // The vertex color
};

// Our custom FVF, which describes our custom vertex structure
constexpr auto D3DFVF_CUSTOMVERTEX = (D3DFVF_XYZRHW | D3DFVF_DIFFUSE);

//-----------------------------------------------------------------------------
// Name: InitD3D()
// Desc: Initializes Direct3D
//-----------------------------------------------------------------------------
HRESULT
init_d3_d(HWND hWnd) {
  // Create the D3D object.
  if (nullptr == (g_p_d3_d = Direct3DCreate9(D3D_SDK_VERSION))) {
    return E_FAIL;
  }

  // Set up the structure used to create the D3DDevice
  D3DPRESENT_PARAMETERS d3dpp;
  ZeroMemory(&d3dpp, sizeof(d3dpp));
  d3dpp.Windowed = TRUE;
  d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
  d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;

  // Create the D3DDevice
  if (FAILED(g_p_d3_d->CreateDevice(
        D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &g_pd3d_device))) {
    return E_FAIL;
  }

  // Device state would normally be set here

  return S_OK;
}

//-----------------------------------------------------------------------------
// Name: InitVB()
// Desc: Creates a vertex buffer and fills it with our vertices. The vertex
//       buffer is basically just a chuck of memory that holds vertices. After
//       creating it, we must Lock()/Unlock() it to fill it. For indices, D3D
//       also uses index buffers. The special thing about vertex and index
//       buffers is that they can be created in device memory, allowing some
//       cards to process them in hardware, resulting in a dramatic
//       performance gain.
//-----------------------------------------------------------------------------
HRESULT init_vb() {
  // Initialize three vertices for rendering a triangle
  std::array<CUSTOMVERTEX, 3> vertices = {{
    {
      150.0F,
      50.0F,
      0.5F,
      1.0F,
      0xffff0000,
    }, // x, y, z, rhw, color
    {
      250.0F,
      250.0F,
      0.5F,
      1.0F,
      0xff00ff00,
    },
    {
      50.0F,
      250.0F,
      0.5F,
      1.0F,
      0xff00ffff,
    },
  }};

  // Create the vertex buffer. Here we are allocating enough memory
  // (from the default pool) to hold all our 3 custom vertices. We also
  // specify the FVF, so the vertex buffer knows what data it contains.
  auto hr = g_pd3d_device->CreateVertexBuffer(
    3 * sizeof(CUSTOMVERTEX), /*Usage=*/0, D3DFVF_CUSTOMVERTEX, D3DPOOL_DEFAULT, &g_p_vb, /*pSharedHandle=*/nullptr);
  if (FAILED(hr)) {
    return E_FAIL;
  }

  // Now we fill the vertex buffer. To do this, we need to Lock() the VB to
  // gain access to the vertices. This mechanism is required becuase vertex
  // buffers may be in device memory.
  VOID* p_vertices;
  hr = g_p_vb->Lock(/*OffsetToLock=*/0, sizeof(vertices), &p_vertices, /*Flags=*/0);
  if (FAILED(hr)) {
    return E_FAIL;
  }
  memcpy(p_vertices, vertices.data(), sizeof(vertices));
  g_p_vb->Unlock();

  return S_OK;
}

//-----------------------------------------------------------------------------
// Name: Cleanup()
// Desc: Releases all previously initialized objects
//-----------------------------------------------------------------------------
VOID cleanup() {
  if (g_p_vb != nullptr) {
    g_p_vb->Release();
  }

  if (g_pd3d_device != nullptr) {
    g_pd3d_device->Release();
  }

  if (g_p_d3_d != nullptr) {
    g_p_d3_d->Release();
  }
}

//-----------------------------------------------------------------------------
// Name: Render()
// Desc: Draws the scene
//-----------------------------------------------------------------------------
VOID render() {
  // Clear the backbuffer to a blue color
  g_pd3d_device->Clear(
    /*Count=*/0, /*pRects=*/nullptr, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 255), /*Z=*/1.0F, /*Stencil=*/0);

  // Begin the scene
  if (SUCCEEDED(g_pd3d_device->BeginScene())) {
    // Draw the triangles in the vertex buffer. This is broken into a few
    // steps. We are passing the vertices down a "stream", so first we need
    // to specify the source of that stream, which is our vertex buffer. Then
    // we need to let D3D know what vertex shader to use. Full, custom vertex
    // shaders are an advanced topic, but in most cases the vertex shader is
    // just the FVF, so that D3D knows what type of vertices we are dealing
    // with. Finally, we call DrawPrimitive() which does the actual rendering
    // of our geometry (in this case, just one triangle).
    g_pd3d_device->SetStreamSource(/*StreamNumber=*/0, g_p_vb, /*OffsetInBytes=*/0, sizeof(CUSTOMVERTEX));
    g_pd3d_device->SetFVF(D3DFVF_CUSTOMVERTEX);
    g_pd3d_device->DrawPrimitive(D3DPT_TRIANGLELIST, /*StartVertex=*/0, /*PrimitiveCount=*/1);

    // End the scene
    g_pd3d_device->EndScene();
  }

  // Present the backbuffer contents to the display
  g_pd3d_device->Present(
    /*pSourceRect=*/nullptr, /*pDestRect=*/nullptr, /*hDestWindowOverride=*/nullptr, /*pDirtyRegion=*/nullptr);
}

//-----------------------------------------------------------------------------
// Name: MsgProc()
// Desc: The window's message handler
//-----------------------------------------------------------------------------
LRESULT WINAPI msg_proc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
  // NOLINTNEXTLINE(hicpp-multiway-paths-covered)
  switch (msg) {
    case WM_DESTROY:
      cleanup();
      PostQuitMessage(/*nExitCode=*/0);
      return 0;
  }

  return DefWindowProc(hWnd, msg, wParam, lParam);
}

//-----------------------------------------------------------------------------
// Name: wWinMain()
// Desc: The application's entry point
//-----------------------------------------------------------------------------
// NOLINTNEXTLINE(readability-identifier-naming, readability-inconsistent-declaration-parameter-name)
INT WINAPI WinMain(HINSTANCE hInst, HINSTANCE /*unused*/, LPSTR /*unused*/, int /*unused*/) {
  UNREFERENCED_PARAMETER(hInst);

  // Register the window class
  WNDCLASSEX wc = {sizeof(WNDCLASSEX),
                   CS_CLASSDC,
                   msg_proc,
                   0L,
                   0L,
                   GetModuleHandle(/*lpModuleName=*/nullptr),
                   LoadIcon(hInst, TEXT("MAINICON")),
                   nullptr,
                   nullptr,
                   nullptr,
                   TEXT("D3D Tutorial"),
                   nullptr};
  RegisterClassEx(&wc);

  // Create the application's window
  HWND h_wnd = CreateWindow(
    TEXT("D3D Tutorial"),
    TEXT("D3D Tutorial 02: Vertices"),
    WS_OVERLAPPEDWINDOW,
    100,
    100,
    300,
    300,
    nullptr,
    nullptr,
    wc.hInstance,
    nullptr);

  // Initialize Direct3D
  if (SUCCEEDED(init_d3_d(h_wnd))) {
    // Create the vertex buffer
    if (SUCCEEDED(init_vb())) {
      // Show the window
      ShowWindow(h_wnd, SW_SHOWDEFAULT);
      UpdateWindow(h_wnd);

      // Enter the message loop
      MSG msg;
      ZeroMemory(&msg, sizeof(msg));
      while (msg.message != WM_QUIT) {
        if (PeekMessage(&msg, /*hWnd=*/nullptr, /*wMsgFilterMin=*/0U, /*wMsgFilterMax=*/0U, PM_REMOVE)) {
          TranslateMessage(&msg);
          DispatchMessage(&msg);
        } else {
          render();
        }
      }
    }
  }
  UnregisterClass(TEXT("D3D Tutorial"), wc.hInstance);
  return 0;
}
