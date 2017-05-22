//
//　3Dオブジェクトのクラス
//

//　ヘッダファイルのインクルード
#include "Obj3d.h"

using namespace DirectX;
using namespace SimpleMath;

//―――――――――――――――――――――――――――――――――
//　静的メンバの実体
//―――――――――――――――――――――――――――――――――

//　カメラ
Camera*											m_pCamera;
//　汎用ステート
std::unique_ptr<DirectX::CommonStates>			m_states;
//　デバイス
Microsoft::WRL::ComPtr<ID3D11Device>			m_d3dDevice;
//　コンテキスト
Microsoft::WRL::ComPtr<ID3D11DeviceContext>		m_d3dContext;
//　エフェクトファクトリ
std::unique_ptr<DirectX::EffectFactory>			m_factory;


void Obj3d::InitializeStatic(Camera * pCamera, 
	Microsoft::WRL::ComPtr<ID3D11Device> d3dDevice, 
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> d3dContext)
{
	m_pCamera = pCamera;
	m_d3dDevice = d3dDevice;
	m_d3dContext = d3dContext;

	m_states = std::make_unique<CommonStates>(m_d3dDevice.Get());
	m_factory = std::make_unique<EffectFactory>(m_d3dDevice.Get());
	m_factory->SetDirectory(L"Resources");
}

