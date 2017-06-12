#pragma once

#include <d3d11_1.h>
#include <SimpleMath.h>
#include <Keyboard.h>
#include <vector>

#include "Obj3d.h"

class Player
{		
	public:
		enum 
		{
			PLAYER_BODY,	//　機体
			PLAYER_SHIP,	//　船体
			PLAYER_WING,	//　翼
			PLAYER_ENGINE,	//　エンジン
			PLAYER_CANNON,	//　砲塔

			PLAYER_PARTS_NUM
		};


		//　コンストラクタ
		Player(DirectX::Keyboard* pKey);

		//デストラクタ
		//~Player();

		//　初期化
		void Initialize();

		//　更新処理
		void Update();

		//　行列の計算
		void Calc();

		//　描画
		void Draw();

		//――――――――――――――――――――
		//　Getter
		//――――――――――――――――――――
		//　平行移動を取得
		const DirectX::SimpleMath::Vector3& GetTranslation() { return m_ObjPlayer[PLAYER_BODY].GetTranslation(); }
		//　ワールド行列を取得
		const DirectX::SimpleMath::Matrix& GetWorld() { return m_ObjPlayer[PLAYER_BODY].GetWorld(); }
		//　回転を取得
		const DirectX::SimpleMath::Vector3& GetRotation() { return m_ObjPlayer[PLAYER_BODY].GetRotation(); }

		//――――――――――――――――――――
		//　Setter
		//――――――――――――――――――――
		// 座標を設定
		void SetTranslation(const DirectX::SimpleMath::Vector3& trans) { m_ObjPlayer[PLAYER_BODY].SetTranslation(trans); }
		// 回転を設定
		void SetRotation(const DirectX::SimpleMath::Vector3& rot) { m_ObjPlayer[PLAYER_BODY].SetRotation(rot); }

	protected:
		std::vector<Obj3d> m_ObjPlayer;

		// キーボード
		DirectX::Keyboard* m_pKeyboard;
		DirectX::Keyboard::KeyboardStateTracker m_KeyboardTracker;

};