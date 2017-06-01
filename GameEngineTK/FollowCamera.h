//
//　自機に追従するカメラクラス
//


//　多重インクルードの防止
#pragma once

//　ヘッダファイルのインクルード
#include "Camera.h"
#include <Keyboard.h>

class FollowCamera :public Camera
{
	public:
		//　カメラと自機の座標
		static const float CAMERA_DISTANCE;
		//　コンストラクタ
		FollowCamera(int windowWidth, int windowHeight);
		//　追従対象座標をセット
		void SetTargetPos(const DirectX::SimpleMath::Vector3 &targetpos);
		//　追従対象角度をセット
		void SetTargetAngle(const float angle);
		//　毎フレーム更新
		void Update()override;
		//　TPSカメラの初期化
		void InitializeTPS();
		//	キーボードセット
		void SetKeyboard(DirectX::Keyboard *keyboard);

	protected:
		//　追従対象の座標
		DirectX::SimpleMath::Vector3 m_targetPos;
		//　追従対象の座標
		float m_targetAngle;
		// キーボード
		DirectX::Keyboard *m_keyboard;
		DirectX::Keyboard::KeyboardStateTracker m_keyboardTracker;
		//　カメラフラグ
		bool m_isFPS;
};

