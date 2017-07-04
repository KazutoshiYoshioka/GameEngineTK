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

		//　更新処理
		void Update();

		//　描画処理
		void Draw();

		//―――――――――――――――――――――――――――――――――
		//　setter
		//―――――――――――――――――――――――――――――――――

		//　スケーリング用
		void SetScale(const DirectX::SimpleMath::Vector3 &scale) { m_scale = scale; }
		//　回転角用
		void SetRotation(const DirectX::SimpleMath::Vector3 &rotation) { m_rotation = rotation; m_UseQuternion = false; }
		//　回転角用（クォーターニオン用）
		void SetRotationQ(const DirectX::SimpleMath::Quaternion &rotationQ) { m_rotationQ = rotationQ; m_UseQuternion = true; }
		//　平行移動用
		void SetTranslation(const DirectX::SimpleMath::Vector3 &translation) { m_translation = translation; }
		//　ローカルワールド
		void SetWorld(const DirectX::SimpleMath::Matrix &world) { m_world = world; }
		//　親行列用
		void SetObjectParent(Obj3d *pObjParent) { m_pObjParent = pObjParent; }
		//―――――――――――――――――――――――――――――――――
		//　getter
		//―――――――――――――――――――――――――――――――――
		
		//　スケーリングを取得
		const DirectX::SimpleMath::Vector3& GetScale() { return m_scale; }
		//　回転を取得
		const DirectX::SimpleMath::Vector3& GetRotation() { return m_rotation; }
		//　平行移動を取得
		const DirectX::SimpleMath::Vector3& GetTranslation() { return m_translation; }
		//　ワールド行列を取得
		const DirectX::SimpleMath::Matrix& GetWorld() { return m_world; }
		//　親行列を取得
		Obj3d* GetObjParent() { return m_pObjParent; }
		
	private:
		//　モデルのユニークポインタ
		std::unique_ptr<DirectX::Model>	m_model;
		//　スケーリング
		DirectX::SimpleMath::Vector3	m_scale;
		//　回転角（オイラー角）
		DirectX::SimpleMath::Vector3	m_rotation;
		//　回転角（クォータニオン）
		DirectX::SimpleMath::Quaternion m_rotationQ;
		//　平行移動
		DirectX::SimpleMath::Vector3	m_translation;
		//　ワールド行列
		DirectX::SimpleMath::Matrix		m_world;
		//　親となる３Ｄオブジェクトのクラスのポインタ
		Obj3d* m_pObjParent;
		//　回転をクォータニオンで持っているフラグ
		bool m_UseQuternion;
};