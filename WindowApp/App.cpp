#include "App.h"
#include "GraphicsError.h"

#include "d3d12.h"
#include <wrl.h>
#include <dxgi1_6.h>
#include "d3dx12.h"

namespace Neodot::App
{
	using Microsoft::WRL::ComPtr;

    int Update(window::IWindow& window)
    {
        constexpr UINT width = 1280;
        constexpr UINT height = 720;
        constexpr UINT bufferCount = 2;

        // Enable Debug Layer
        {
            ComPtr<ID3D12Debug> debugController;
            D3D12GetDebugInterface(IID_PPV_ARGS(&debugController)) >> checkToken;
            debugController->EnableDebugLayer();
        }

        // DXGI Factory
        ComPtr<IDXGIFactory4> dxgiFactory;
        CreateDXGIFactory2(DXGI_CREATE_FACTORY_DEBUG, IID_PPV_ARGS(&dxgiFactory)) >> checkToken;

        // Device
        ComPtr<ID3D12Device2> device;
        D3D12CreateDevice(nullptr, D3D_FEATURE_LEVEL_12_0, IID_PPV_ARGS(&device)) >> checkToken;

        // Command Queue
        ComPtr<ID3D12CommandQueue> commandQueue;
        {
            const D3D12_COMMAND_QUEUE_DESC desc = {
                .Type = D3D12_COMMAND_LIST_TYPE_DIRECT,
                .Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL,
                .Flags = D3D12_COMMAND_QUEUE_FLAG_NONE,
                .NodeMask = 0,
            };
            device->CreateCommandQueue(&desc, IID_PPV_ARGS(&commandQueue)) >> checkToken;
        }

        // Swap Chain
        ComPtr<IDXGISwapChain4> swapChain;
        {
            const DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {
                .Width = width,
                .Height = height,
                .Format = DXGI_FORMAT_R8G8B8A8_UNORM,
                .Stereo = FALSE,
                .SampleDesc = {.Count = 1, .Quality = 0 },
                .BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT,
                .BufferCount = bufferCount,
                .Scaling = DXGI_SCALING_STRETCH,
                .SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD,
                .AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED,
                .Flags = 0,
            };
            ComPtr<IDXGISwapChain1> swapChain1;
            dxgiFactory->CreateSwapChainForHwnd(
                commandQueue.Get(), window.GetHandle(), &swapChainDesc, nullptr, nullptr, &swapChain1) >> checkToken;
            swapChain1.As(&swapChain) >> checkToken;
        }

        // RTV Descriptor Heap
        ComPtr<ID3D12DescriptorHeap> rtvDescriptorHeap;
        {
            const D3D12_DESCRIPTOR_HEAP_DESC desc = {
                .Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV,
                .NumDescriptors = bufferCount,
            };
            device->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&rtvDescriptorHeap)) >> checkToken;
        }
        const auto rtvDescriptorSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

        // RTV Descriptors / Back Buffers
        ComPtr<ID3D12Resource> backBuffers[bufferCount];
        {
            CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(rtvDescriptorHeap->GetCPUDescriptorHandleForHeapStart());
            for (UINT i = 0; i < bufferCount; ++i) {
                swapChain->GetBuffer(i, IID_PPV_ARGS(&backBuffers[i])) >> checkToken;
                device->CreateRenderTargetView(backBuffers[i].Get(), nullptr, rtvHandle);
                rtvHandle.Offset(1, rtvDescriptorSize);
            }
        }

        // Command Allocator
        ComPtr<ID3D12CommandAllocator> commandAllocator;
        device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&commandAllocator)) >> checkToken;

        // Command List
        ComPtr<ID3D12GraphicsCommandList> commandList;
        device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, commandAllocator.Get(), nullptr, IID_PPV_ARGS(&commandList)) >> checkToken;
        commandList->Close() >> checkToken;

        // Fence
        ComPtr<ID3D12Fence> fence;
        uint64_t fenceValue = 0;
        device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence)) >> checkToken;

        // Fence Event
        HANDLE fenceEvent = CreateEventW(nullptr, FALSE, FALSE, nullptr);
        if (!fenceEvent) {
            GetLastError() >> checkToken;
            throw std::runtime_error("Failed to create fence signal event");
        }

        // Render Loop
        CD3DX12_VIEWPORT viewport(0.0f, 0.0f, (float)width, (float)height);
        CD3DX12_RECT scissorRect(0, 0, width, height);
        UINT curBackBufferIndex;
        while (!window.IsClosing()) {
            curBackBufferIndex = swapChain->GetCurrentBackBufferIndex();
            auto& backBuffer = backBuffers[curBackBufferIndex];

            commandAllocator->Reset() >> checkToken;
            commandList->Reset(commandAllocator.Get(), nullptr) >> checkToken;

            // Set viewport and scissor
            commandList->RSSetViewports(1, &viewport);
            commandList->RSSetScissorRects(1, &scissorRect);

            // Transition to render target
            const auto barrierToRT = CD3DX12_RESOURCE_BARRIER::Transition(
                backBuffer.Get(), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
            commandList->ResourceBarrier(1, &barrierToRT);

            // Clear render target
            FLOAT clearColor[] = { 0.5f, 0.5f, 0.5f, 1.0f };
            const CD3DX12_CPU_DESCRIPTOR_HANDLE rtv(
                rtvDescriptorHeap->GetCPUDescriptorHandleForHeapStart(), (INT)curBackBufferIndex, rtvDescriptorSize);
            commandList->ClearRenderTargetView(rtv, clearColor, 0, nullptr);

            // Transition to present
            const auto barrierToPresent = CD3DX12_RESOURCE_BARRIER::Transition(
                backBuffer.Get(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
            commandList->ResourceBarrier(1, &barrierToPresent);

            // Submit
            commandList->Close() >> checkToken;
            ID3D12CommandList* const commandLists[] = { commandList.Get() };
            commandQueue->ExecuteCommandLists((UINT)std::size(commandLists), commandLists);

            // Signal and present
            commandQueue->Signal(fence.Get(), fenceValue++) >> checkToken;
            swapChain->Present(1, 0) >> checkToken;

            // Wait
            fence->SetEventOnCompletion(fenceValue - 1, fenceEvent) >> checkToken;
            if (WaitForSingleObject(fenceEvent, INFINITE) == WAIT_FAILED) {
                GetLastError() >> checkToken;
            }
        }

        // Flush queue
        commandQueue->Signal(fence.Get(), fenceValue) >> checkToken;
        fence->SetEventOnCompletion(fenceValue, fenceEvent);
        if (WAIT_FAILED == WaitForSingleObject(fenceEvent, 2000)) {
            GetLastError() >> checkToken;
        }

        // cleanup
        CloseHandle(fenceEvent);

        return 0;
    }
}