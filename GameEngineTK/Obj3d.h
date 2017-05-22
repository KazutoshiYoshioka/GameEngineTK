//
//　３Ｄオブジェクトのクラス
//

//　多重インクルードの防止
#pragma once

//　ヘッダファイルのインクルード
#include <windows.h>
#include <wrl/client.h>
#include <memory>
#include <Effects.h>
#include <CommonStates.h>
#include <SimpleMath.h>
#include <Model.h>

#include "Camera.h"

class Obj3d
{
	//　静的メンバ
	public:
		static void InitializeStatic(Camera* pCamera
			,Microsoft::WRL::ComPtr<ID3D11Device> d3dDevice
			,Microsoft::WRL::ComPtr<ID3D11DeviceContext> d3dContext);

	private:
		//　カメラ
		static Camera*											m_pCamera;
		//　汎用ステート
		static std::unique_ptr<DirectX::CommonStates>			m_states;
		//　デバイス
		static Microsoft::WRL::ComPtr<ID3D11Device>				m_d3dDevice;
		//　コンテキスト
		static Microsoft::WRL::ComPtr<ID3D11DeviceContext>		m_d3dContext;
		//　エフェクトファクトリ
		static std::unique_ptr<DirectX::EffectFactory>			m_factory;

	public:
		//　コンストラクタ
		Obj3d();

		//　モデルの読み込み
		void LoadModel(const wchar_t *filename);

		//―――――――――――――――――――――――――――――――――
		//　setter
		//―――――――――――――――――――――――――――――――――

		//　スケーリング用

		//　平行移動用

		//　回転角用

		//―――――――――――――――――――――――――――――――――
		//　getter
		//―――――――――――――――――――――――――――――――――

		//　ワールド行列を取得

	private:
		//　モデルのユニークポインタ

		//　スケーリング

		//　平行移動

		//　回転角

		//　ワールド行列

		//　親となる３Ｄオブジェクトのクラスのポインタ
		Obj3d* m_pObj_Parent;
};