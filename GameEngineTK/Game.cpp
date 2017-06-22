//
// Game.cpp
//

//ヘッダファイルのインクルード
#include "pch.h"
#include "Game.h"
#include <time.h>



extern void ExitGame();

using namespace DirectX;
using namespace DirectX::SimpleMath;

using Microsoft::WRL::ComPtr;

Game::Game() :
    m_window(0),
    m_outputWidth(800),
    m_outputHeight(600),
    m_featureLevel(D3D_FEATURE_LEVEL_9_1)
{
	srand((unsigned int)time(nullptr));
}

// Initialize the Direct3D resources required to run.
void Game::Initialize(HWND window, int width, int height)
{
    m_window = window;
    m_outputWidth = std::max(width, 1);
    m_outputHeight = std::max(height, 1);

    CreateDevice();

    CreateResources();

    // TODO: Change the timer settings if you want something other than the default variable timestep mode.
    // e.g. for 60 FPS fixed timestep update logic, call:
    /*
    m_timer.SetFixedTimeStep(true);
    m_timer.SetTargetElapsedSeconds(1.0 / 60);
    */

	m_primitiveBatch = std::make_unique<PrimitiveBatch<VertexPositionNormal>>(m_d3dContext.Get());//←コンストラクタの引数

	m_basicEffect = std::make_unique<BasicEffect>(m_d3dDevice.Get());

	m_basicEffect->SetProjection(XMMatrixOrthographicOffCenterRH(0,
		m_outputWidth, m_outputHeight, 0, 0, 1));
	m_basicEffect->SetVertexColorEnabled(true);
	
	void const* shaderByteCode;
	size_t byteCodeLength;

	m_basicEffect->GetVertexShaderBytecode(&shaderByteCode, &byteCodeLength);

	m_d3dDevice->CreateInputLayout(VertexPositionColor::InputElements,
		VertexPositionColor::InputElementCount,
		shaderByteCode, byteCodeLength,
		m_inputLayout.GetAddressOf());

	//初期化はここに書く

	//　デバッグカメラの生成
	m_debugcamera = std::make_unique<DebugCamera>(m_outputWidth, m_outputHeight);

	//　キーボード作成
	m_key = std::make_unique<Keyboard>();

	m_camera = std::make_unique<FollowCamera>(m_outputWidth, m_outputHeight);
	m_camera->SetKeyboard(m_key.get());

	Obj3d::InitializeStatic(
		m_camera.get()
		, m_d3dDevice
		, m_d3dContext
	);

	//　エフェクトファクトリーの生成
	m_factory = std::make_unique<EffectFactory>(m_d3dDevice.Get());

	

	//　テクスチャの読み込みフォルダを指定
	m_factory->SetDirectory(L"Resources");

	//　モデルの読み込み
	m_ObjSkydoom.LoadModel(L"Resources/Skydoom.cmo");

	m_modelGround = Model::CreateFromCMO(m_d3dDevice.Get(), L"Resources/ground200m.cmo", *m_factory);
	m_modelBall = Model::CreateFromCMO(m_d3dDevice.Get(), L"Resources/ball.cmo", *m_factory);
	m_modelSmallTank = Model::CreateFromCMO(m_d3dDevice.Get(), L"Resources/Tank.cmo", *m_factory);

	m_modelTank = Model::CreateFromCMO(m_d3dDevice.Get(), L"Resources/sangou.cmo", *m_factory);

	m_TankRot = 0.0f;


	m_Player = std::make_unique<Player>(m_key.get());

	m_Enemies.resize(ENEMY_NUM);
	for (int i = 0; i < ENEMY_NUM; i++)
	{
		m_Enemies[i] = std::make_unique<Enemy>();
		m_Enemies[i]->Initialize();
	}

	m_view = Matrix::CreateLookAt(Vector3(2.f, 2.f, 2.f),
		Vector3::Zero, Vector3::UnitY);
	m_proj = Matrix::CreatePerspectiveFieldOfView(XM_PI / 4.f,
		float(m_outputWidth) / float(m_outputHeight), 0.1f, 1000.f);

	m_ObjTank.LoadModel(L"Resources/sangou.cmo");
	m_ObjTank.SetScale(Vector3(0.03f, 0.03f, 0.03f));

	// 指定範囲をランダムで返すラムダ式
	auto rand_value = [](float min, float max)
	{
		return (max - min) * (float)rand() / RAND_MAX + min;
	};
}

// Executes the basic game loop.
void Game::Tick()
{
    m_timer.Tick([&]()
    {
        Update(m_timer);
    });

    Render();
}

// Updates the world.
void Game::Update(DX::StepTimer const& timer)
{
	float elapsedTime = float(timer.GetElapsedSeconds());

	// TODO: Add your game logic here.
	elapsedTime;

	//　ゲームの毎フレーム処理
	//　デバッグカメラの更新
	m_debugcamera->Update();

	//―――――――――――――――
	//　球のワールド行列を計算する　｜
	//―――――――――――――――
	//	スケーリング
	Matrix scalemat = Matrix::CreateScale(2.0f);

	//　ロール
	Matrix rotmatZ = Matrix::CreateRotationZ(XMConvertToRadians(15.0f));
	//　ピッチ（仰角）
	Matrix rotmatX = Matrix::CreateRotationX(XMConvertToRadians(15.0f)); //　XMConvertToRadiansで度をラジアンに変換する
	//　ヨー
	Matrix rotmatY = Matrix::CreateRotationY(XMConvertToRadians(15.0f));
	//　回転行列の集合体
	Matrix rotmat = rotmatZ * rotmatX * rotmatY;

	//平行移動
	Matrix transmat = Matrix::CreateTranslation(5.0f, 0.0f, 0.0f);

	/*for (int i = 0; i < 10000; i++)
	{
		Matrix transground = Matrix::CreateTranslation(i / 100 * 1, 0, i % 100 * 1);
		m_ground[i] = transground;
	}*/

	static float a = 0;
	a += 0.2;
	//　２０個の天球の行列
	for (int i = 0; i < 20; i++)
	{
		Matrix transball = Matrix::CreateTranslation((i + 10) / 10 * 20, 0, 0);
		Matrix rotateball = Matrix::CreateRotationY(XMConvertToRadians(i * 36));
		Matrix rotateball2;
		if (i < 10)
		{
			rotateball2 = Matrix::CreateRotationY(XMConvertToRadians(a));
		}
		else
		{
			rotateball2 = Matrix::CreateRotationY(XMConvertToRadians(a*-1));
		}
		m_worldBall[i] = transball*rotateball*rotateball2;
	}

	//行列の合成    倍率　×　回転　×　平行移動
	m_worldball = scalemat * rotmat * transmat;

	//　キーボードの情報を取得
	Keyboard::State g_key = m_key->GetState();

	

	//　旋回処理
	if (g_key.A)
	{
		m_TankRot++;
	}
	if (g_key.D)
	{
		m_TankRot--;
	}

	//　移動処理
	if (g_key.W)
	{
		//　移動ベクトル
		Vector3 moveV(0, 0, 0.1f);
		//　角度に合わせて移動ベクトルを回転させる
		//moveV = Vector3::TransformNormal(moveV, m_worldTank);
		Matrix rotmove = Matrix::CreateRotationY(m_ObjTank.GetRotation().y);
		moveV = Vector3::TransformNormal(moveV, rotmove);

		m_ObjTank.SetTranslation(m_ObjTank.GetTranslation() - moveV);
		//　自機の座標
		m_TankPos += moveV;
	}
	if (g_key.S)
	{
		//　移動ベクトル
		Vector3 moveV(0, 0, -0.1f);
		Matrix rotmove = Matrix::CreateRotationY(m_ObjTank.GetRotation().y);
		moveV = Vector3::TransformNormal(moveV, rotmove);

		m_ObjTank.SetTranslation(m_ObjTank.GetTranslation() - moveV);

		//　自機の座標
		m_TankPos += moveV;
	}

	{//　自機のワールド行列を計算
		//　平行移動行列
		Matrix transTank = Matrix::CreateTranslation(m_TankPos);
		Matrix rotateTank = Matrix::CreateRotationY(XMConvertToRadians(m_TankRot));
		Matrix changeVec = Matrix::CreateRotationY(XMConvertToRadians(180.0f));
		m_worldTank = changeVec * rotateTank * transTank;

		//　自機のパーツ２を計算
		//　回転行列（パーツ１からの回転分）
		//Matrix rotmatTank2 = Matrix::CreateRotationZ(XM_PIDIV2)*Matrix::CreateRotationY(XM_PI);
		//　平行移動行列（パーツ１からの平行移動分）
		Matrix transTank2 = Matrix::CreateTranslation(Vector3(3.0f, 0, -2.5));
		m_worldTank2 = transTank2 * m_worldTank;
	}

	m_Player->Update();

	for (std::vector<std::unique_ptr<Enemy>>::iterator it = m_Enemies.begin();
		it != m_Enemies.end();
		it++)
	{
		(*it)->Update();
	}

	// 攻撃当たり判定
	{
		// プレイヤーの攻撃当たり球
		const Sphere& sphereA = m_Player->GetCollisionNodeBullet();

		// 全ての敵について判定する
		for (std::vector<std::unique_ptr<Enemy>>::iterator it = m_Enemies.begin();
			it != m_Enemies.end();
			)
		{
			Enemy* enemy = it->get();

			// 敵の被攻撃当たり球
			const Sphere& sphereB = enemy->GetCollisionNodeBody();

			Vector3 inter;

			// 球と球の当たり
			if (CheckSphere2Sphere(sphereA, sphereB, &inter))
			{
				m_Player->ReloadBullet();

				//ModelEffectManager::getInstance()->Entry(
				//	L"Resources/HitEffect.cmo",
				//	10,
				//	inter,	// 座標
				//	Vector3(0, 0, 0),	// 速度
				//	Vector3(0, 0, 0),	// 加速度
				//	Vector3(0, 0, 0),	// 回転角（初期）
				//	Vector3(0, 0, 0),	// 回転角（最終）
				//	Vector3(0, 0, 0),	// スケール（初期）
				//	Vector3(6, 6, 6)	// スケール（最終）
				//);
				it = m_Enemies.erase(it);
			}
			else
			{
				it++;
			}
		}
	}

	
	m_camera->SetTargetPos(m_Player->GetTranslation());
	m_camera->SetTargetAngle(m_Player->GetRotation().y);

	//　カメラの更新
	m_camera->Update();
	m_view = m_camera->GetViewMatrix();
	m_proj = m_camera->GetProjectionMatrix();

	
	m_ObjSkydoom.Update();
	m_ObjTank.Update();

}

// Draws the scene.
void Game::Render()
{
    // Don't try to render anything before the first Update.
    if (m_timer.GetFrameCount() == 0)
    {
        return;
    }

    Clear();

    // TODO: Add your rendering code here.
	//　描画はここに書く

	//　頂点インデックス
	uint16_t indices[] =
	{
		0,1,2,
		2,1,3,
	};

	//　頂点座標
	VertexPositionNormal vertices[] =
	{//				頂点座標			色
		{ Vector3(-1.0f, 1.0f, 0.0f),Colors::Red },	
		{ Vector3(-1.0f,-1.0f, 0.0f),Colors::Blue },
		{ Vector3( 1.0f, 1.0f, 0.0f),Colors::Green },
		{ Vector3( 1.0f,-1.0f, 0.0f),Colors::Yellow },
	};

	m_states = std::make_unique<CommonStates>(m_d3dDevice.Get());

	m_d3dContext->OMSetBlendState(m_states->Opaque(), nullptr, 0xFFFFFFFF);
	m_d3dContext->OMSetDepthStencilState(m_states->DepthNone(), 0);
	m_d3dContext->RSSetState(m_states->CullClockwise());
	
	//　デバッグカメラからビュー行列を取得
	//m_view = m_debugcamera->GetCameraMatrix();

	////　カメラの位置（視点座標）
	//Vector3 eyepos(0.0f, 0.5f, 0.0f);
	////　どこを見るのか（注視点、参照点）
	//Vector3 refpos(0.0f, 0.0f, 0.0f);
	////　上方向ベクトル
	//Vector3 upvec(0.0f, -1.0f, 0.0f);
	//upvec.Normalize();
	////　ビュー行列を作成
	//m_view = Matrix::CreateLookAt(eyepos, refpos, upvec);

	////　垂直方向視野角
	//float fovY = XMConvertToRadians(60.0f);
	////　画面横幅と縦幅の比率
	//float aspect = (float)m_outputWidth / m_outputHeight;
	////　手前の表示限界距離
	//float nearclip = 0.1f;
	//// 奥の表示限界距離
	//float farclip = 1000.0f;
	////射影行列を生成
	//m_proj = Matrix::CreatePerspectiveFieldOfView(fovY, aspect, nearclip, farclip);
	

	//m_view = Matrix::CreateLookAt(Vector3(2.f, 2.f, 2.f),
	//	Vector3(0, 0, 0), Vector3(0, 0, 0));
	
	/*m_proj = Matrix::CreatePerspectiveFieldOfView(XM_PI / 4.f,
		float(m_outputWidth) / float(m_outputHeight), 0.1f, 500.f);*/

	m_basicEffect->SetView(m_view);
	m_basicEffect->SetProjection(m_proj);

	m_basicEffect->Apply(m_d3dContext.Get());
	m_d3dContext->IASetInputLayout(m_inputLayout.Get());

	//　天球を描画
	m_ObjSkydoom.Draw();

	m_ObjTank.Draw();

	//　地面を描画
	m_modelGround->Draw(m_d3dContext.Get(), *m_states, Matrix::Identity, m_view, m_proj);
	/*for (int i = 0; i < 10000; i++)
	{
		m_modelGround->Draw(m_d3dContext.Get(), *m_states, m_ground[i], m_view, m_proj);
	}*/

	//　ボールを描画
	//m_modelBall->Draw(m_d3dContext.Get(), *m_states, m_worldball, m_view, m_proj);
	Matrix scalemat = Matrix::CreateScale(0.01f);

	//m_modelTank->Draw(m_d3dContext.Get(), *m_states, scalemat*m_worldTank, m_view, m_proj);

	//m_modelTank->Draw(m_d3dContext.Get(), *m_states, scalemat*m_worldTank2, m_view, m_proj);

	//　戦車描画
	//m_modelTank->Draw(m_d3dContext.Get(), *m_states, m_worldTank, m_view, m_proj);

	m_Player->Draw();

	for (std::vector<std::unique_ptr<Enemy>>::iterator it = m_Enemies.begin();
		it != m_Enemies.end();
		it++)
	{
		(*it)->Draw();
	}

	//　ボールを描画
	for (int i = 0; i < 20; i++)
	{
		m_modelBall->Draw(m_d3dContext.Get(), *m_states, m_worldBall[i], m_view, m_proj);
	}
	m_primitiveBatch->Begin();

	m_primitiveBatch->DrawIndexed(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST, indices, 6, vertices, 4);

	m_primitiveBatch->End();

    Present();
}

// Helper method to clear the back buffers.
void Game::Clear()
{
    // Clear the views.
    m_d3dContext->ClearRenderTargetView(m_renderTargetView.Get(), Colors::CornflowerBlue);
    m_d3dContext->ClearDepthStencilView(m_depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

    m_d3dContext->OMSetRenderTargets(1, m_renderTargetView.GetAddressOf(), m_depthStencilView.Get());

    // Set the viewport.
    CD3D11_VIEWPORT viewport(0.0f, 0.0f, static_cast<float>(m_outputWidth), static_cast<float>(m_outputHeight));
    m_d3dContext->RSSetViewports(1, &viewport);
}

// Presents the back buffer contents to the screen.
void Game::Present()
{
    // The first argument instructs DXGI to block until VSync, putting the application
    // to sleep until the next VSync. This ensures we don't waste any cycles rendering
    // frames that will never be displayed to the screen.
    HRESULT hr = m_swapChain->Present(1, 0);

    // If the device was reset we must completely reinitialize the renderer.
    if (hr == DXGI_ERROR_DEVICE_REMOVED || hr == DXGI_ERROR_DEVICE_RESET)
    {
        OnDeviceLost();
    }
    else
    {
        DX::ThrowIfFailed(hr);
    }
}

// Message handlers
void Game::OnActivated()
{
    // TODO: Game is becoming active window.
}

void Game::OnDeactivated()
{
    // TODO: Game is becoming background window.
}

void Game::OnSuspending()
{
    // TODO: Game is being power-suspended (or minimized).
}

void Game::OnResuming()
{
    m_timer.ResetElapsedTime();

    // TODO: Game is being power-resumed (or returning from minimize).
}

void Game::OnWindowSizeChanged(int width, int height)
{
    m_outputWidth = std::max(width, 1);
    m_outputHeight = std::max(height, 1);

    CreateResources();

    // TODO: Game window is being resized.
}

// Properties
void Game::GetDefaultSize(int& width, int& height) const
{
    // TODO: Change to desired default window size (note minimum size is 320x200).
    width = 800;
    height = 600;
}

// These are the resources that depend on the device.
void Game::CreateDevice()
{
    UINT creationFlags = 0;

#ifdef _DEBUG
    creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

    static const D3D_FEATURE_LEVEL featureLevels [] =
    {
        // TODO: Modify for supported Direct3D feature levels (see code below related to 11.1 fallback handling).
        D3D_FEATURE_LEVEL_11_1,
        D3D_FEATURE_LEVEL_11_0,
        D3D_FEATURE_LEVEL_10_1,
        D3D_FEATURE_LEVEL_10_0,
        D3D_FEATURE_LEVEL_9_3,
        D3D_FEATURE_LEVEL_9_2,
        D3D_FEATURE_LEVEL_9_1,
    };

    // Create the DX11 API device object, and get a corresponding context.
    HRESULT hr = D3D11CreateDevice(
        nullptr,                                // specify nullptr to use the default adapter
        D3D_DRIVER_TYPE_HARDWARE,
        nullptr,
        creationFlags,
        featureLevels,
        _countof(featureLevels),
        D3D11_SDK_VERSION,
        m_d3dDevice.ReleaseAndGetAddressOf(),   // returns the Direct3D device created
        &m_featureLevel,                        // returns feature level of device created
        m_d3dContext.ReleaseAndGetAddressOf()   // returns the device immediate context
        );

    if (hr == E_INVALIDARG)
    {
        // DirectX 11.0 platforms will not recognize D3D_FEATURE_LEVEL_11_1 so we need to retry without it.
        hr = D3D11CreateDevice(nullptr,
            D3D_DRIVER_TYPE_HARDWARE,
            nullptr,
            creationFlags,
            &featureLevels[1],
            _countof(featureLevels) - 1,
            D3D11_SDK_VERSION,
            m_d3dDevice.ReleaseAndGetAddressOf(),
            &m_featureLevel,
            m_d3dContext.ReleaseAndGetAddressOf()
            );
    }

    DX::ThrowIfFailed(hr);

#ifndef NDEBUG
    ComPtr<ID3D11Debug> d3dDebug;
    if (SUCCEEDED(m_d3dDevice.As(&d3dDebug)))
    {
        ComPtr<ID3D11InfoQueue> d3dInfoQueue;
        if (SUCCEEDED(d3dDebug.As(&d3dInfoQueue)))
        {
#ifdef _DEBUG
            d3dInfoQueue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_CORRUPTION, true);
            d3dInfoQueue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_ERROR, true);
#endif
            D3D11_MESSAGE_ID hide [] =
            {
                D3D11_MESSAGE_ID_SETPRIVATEDATA_CHANGINGPARAMS,
                // TODO: Add more message IDs here as needed.
            };
            D3D11_INFO_QUEUE_FILTER filter = {};
            filter.DenyList.NumIDs = _countof(hide);
            filter.DenyList.pIDList = hide;
            d3dInfoQueue->AddStorageFilterEntries(&filter);
        }
    }
#endif

    // DirectX 11.1 if present
    if (SUCCEEDED(m_d3dDevice.As(&m_d3dDevice1)))
        (void)m_d3dContext.As(&m_d3dContext1);

    // TODO: Initialize device dependent objects here (independent of window size).
}

// Allocate all memory resources that change on a window SizeChanged event.
void Game::CreateResources()
{
    // Clear the previous window size specific context.
    ID3D11RenderTargetView* nullViews [] = { nullptr };
    m_d3dContext->OMSetRenderTargets(_countof(nullViews), nullViews, nullptr);
    m_renderTargetView.Reset();
    m_depthStencilView.Reset();
    m_d3dContext->Flush();

    UINT backBufferWidth = static_cast<UINT>(m_outputWidth);
    UINT backBufferHeight = static_cast<UINT>(m_outputHeight);
    DXGI_FORMAT backBufferFormat = DXGI_FORMAT_B8G8R8A8_UNORM;
    DXGI_FORMAT depthBufferFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
    UINT backBufferCount = 2;

    // If the swap chain already exists, resize it, otherwise create one.
    if (m_swapChain)
    {
        HRESULT hr = m_swapChain->ResizeBuffers(backBufferCount, backBufferWidth, backBufferHeight, backBufferFormat, 0);

        if (hr == DXGI_ERROR_DEVICE_REMOVED || hr == DXGI_ERROR_DEVICE_RESET)
        {
            // If the device was removed for any reason, a new device and swap chain will need to be created.
            OnDeviceLost();

            // Everything is set up now. Do not continue execution of this method. OnDeviceLost will reenter this method 
            // and correctly set up the new device.
            return;
        }
        else
        {
            DX::ThrowIfFailed(hr);
        }
    }
    else
    {
        // First, retrieve the underlying DXGI Device from the D3D Device.
        ComPtr<IDXGIDevice1> dxgiDevice;
        DX::ThrowIfFailed(m_d3dDevice.As(&dxgiDevice));

        // Identify the physical adapter (GPU or card) this device is running on.
        ComPtr<IDXGIAdapter> dxgiAdapter;
        DX::ThrowIfFailed(dxgiDevice->GetAdapter(dxgiAdapter.GetAddressOf()));

        // And obtain the factory object that created it.
        ComPtr<IDXGIFactory1> dxgiFactory;
        DX::ThrowIfFailed(dxgiAdapter->GetParent(IID_PPV_ARGS(dxgiFactory.GetAddressOf())));

        ComPtr<IDXGIFactory2> dxgiFactory2;
        if (SUCCEEDED(dxgiFactory.As(&dxgiFactory2)))
        {
            // DirectX 11.1 or later

            // Create a descriptor for the swap chain.
            DXGI_SWAP_CHAIN_DESC1 swapChainDesc = { 0 };
            swapChainDesc.Width = backBufferWidth;
            swapChainDesc.Height = backBufferHeight;
            swapChainDesc.Format = backBufferFormat;
            swapChainDesc.SampleDesc.Count = 1;
            swapChainDesc.SampleDesc.Quality = 0;
            swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
            swapChainDesc.BufferCount = backBufferCount;

            DXGI_SWAP_CHAIN_FULLSCREEN_DESC fsSwapChainDesc = { 0 };
            fsSwapChainDesc.Windowed = TRUE;

            // Create a SwapChain from a Win32 window.
            DX::ThrowIfFailed(dxgiFactory2->CreateSwapChainForHwnd(
                m_d3dDevice.Get(),
                m_window,
                &swapChainDesc,
                &fsSwapChainDesc,
                nullptr,
                m_swapChain1.ReleaseAndGetAddressOf()
                ));

            DX::ThrowIfFailed(m_swapChain1.As(&m_swapChain));
        }
        else
        {
            DXGI_SWAP_CHAIN_DESC swapChainDesc = { 0 };
            swapChainDesc.BufferCount = backBufferCount;
            swapChainDesc.BufferDesc.Width = backBufferWidth;
            swapChainDesc.BufferDesc.Height = backBufferHeight;
            swapChainDesc.BufferDesc.Format = backBufferFormat;
            swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
            swapChainDesc.OutputWindow = m_window;
            swapChainDesc.SampleDesc.Count = 1;
            swapChainDesc.SampleDesc.Quality = 0;
            swapChainDesc.Windowed = TRUE;

            DX::ThrowIfFailed(dxgiFactory->CreateSwapChain(m_d3dDevice.Get(), &swapChainDesc, m_swapChain.ReleaseAndGetAddressOf()));
        }

        // This template does not support exclusive fullscreen mode and prevents DXGI from responding to the ALT+ENTER shortcut.
        DX::ThrowIfFailed(dxgiFactory->MakeWindowAssociation(m_window, DXGI_MWA_NO_ALT_ENTER));
    }

    // Obtain the backbuffer for this window which will be the final 3D rendertarget.
    ComPtr<ID3D11Texture2D> backBuffer;
    DX::ThrowIfFailed(m_swapChain->GetBuffer(0, IID_PPV_ARGS(backBuffer.GetAddressOf())));

    // Create a view interface on the rendertarget to use on bind.
    DX::ThrowIfFailed(m_d3dDevice->CreateRenderTargetView(backBuffer.Get(), nullptr, m_renderTargetView.ReleaseAndGetAddressOf()));

    // Allocate a 2-D surface as the depth/stencil buffer and
    // create a DepthStencil view on this surface to use on bind.
    CD3D11_TEXTURE2D_DESC depthStencilDesc(depthBufferFormat, backBufferWidth, backBufferHeight, 1, 1, D3D11_BIND_DEPTH_STENCIL);

    ComPtr<ID3D11Texture2D> depthStencil;
    DX::ThrowIfFailed(m_d3dDevice->CreateTexture2D(&depthStencilDesc, nullptr, depthStencil.GetAddressOf()));

    CD3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc(D3D11_DSV_DIMENSION_TEXTURE2D);
    DX::ThrowIfFailed(m_d3dDevice->CreateDepthStencilView(depthStencil.Get(), &depthStencilViewDesc, m_depthStencilView.ReleaseAndGetAddressOf()));

    // TODO: Initialize windows-size dependent objects here.
}

void Game::OnDeviceLost()
{
    // TODO: Add Direct3D resource cleanup here.

    m_depthStencilView.Reset();
    m_renderTargetView.Reset();
    m_swapChain1.Reset();
    m_swapChain.Reset();
    m_d3dContext1.Reset();
    m_d3dContext.Reset();
    m_d3dDevice1.Reset();
    m_d3dDevice.Reset();

    CreateDevice();

    CreateResources();
}