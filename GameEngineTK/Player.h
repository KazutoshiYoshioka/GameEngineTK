#pragma once

#include <d3d11_1.h>
#include <SimpleMath.h>
#include <Keyboard.h>
#include <vector>

#include "Obj3d.h"
#include "CollisionNode.h"

class Player
{		
	public:
		//　重力加速度
		const float GRAVITY_ACC = 0.03f;
		//　ジャンプ初速
		const float JUMP_SPEED_FIRST = 0.5f;
		//　ジャンプ最高速
		const float JUMP_SPEED_MAX = 0.3f;

		//　パーツ
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

		// パーツを発射
		void FireBullet();

		//　弾の再装填
		void ReloadBullet();

		//　ジャンプ開始
		void StartJump();
		//　重力
		void StartFall();
		//　
		void StopJump();

		//――――――――――――――――――――
		//　Getter
		//――――――――――――――――――――
		//　平行移動を取得
		const DirectX::SimpleMath::Vector3& GetTranslation() { return m_ObjPlayer[PLAYER_BODY].GetTranslation(); }
		//　ワールド行列を取得
		const DirectX::SimpleMath::Matrix& GetWorld() { return m_ObjPlayer[PLAYER_BODY].GetWorld(); }
		//　回転を取得
		const DirectX::SimpleMath::Vector3& GetRotation() { return m_ObjPlayer[PLAYER_BODY].GetRotation(); }
		//　弾丸の当たり判定球
		const SphereNode& GetCollisionNodeBullet() { return m_CollisionNodeBullet; }
		// 弾丸の当たり判定球を取得
		const SphereNode& GetCollisionNodeBody() { return m_CollisionNodeBody; }
		//　速度を取得
		const DirectX::SimpleMath::Vector3& GetVelocity() { return m_Velocity; }

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

		bool m_FireFlag;
		DirectX::SimpleMath::Vector3 m_BulletVel;
		int m_FireCount;

		//　攻撃用あたり判定
		SphereNode m_CollisionNodeBullet;

		//　全体の当たり判定
		SphereNode m_CollisionNodeBody;

		//　速度ベクトル
		DirectX::SimpleMath::Vector3 m_Velocity;
		//　ジャンプ中フラグ
		bool m_isJump;
};