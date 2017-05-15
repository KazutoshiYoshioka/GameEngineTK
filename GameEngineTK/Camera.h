//――――――――――――――
//　カメラを制御するクラス
//――――――――――――――

//　多重インクルードの防止
#pragma once

//　ヘッダファイルのインクルード
#include <d3d11.h>
#include <SimpleMath.h>


class Camera
{
	public:
		//　メンバ関数を置くところ
		Camera(int windowWidth,int windowHeight);

		virtual ~Camera();

		//　更新
		virtual void Update();

		// ビュー座標を取得
		DirectX::SimpleMath::Matrix GetViewMatrix();

		//　射影行列を取得
		DirectX::SimpleMath::Matrix GetProjectionMatrix();

		//　視点座標をセット
		void SetEyePos(DirectX::SimpleMath::Vector3 &eyepos);
		//　注視点座標をセット
		void SetRefPos(DirectX::SimpleMath::Vector3 &refpos);
		//　上方向ベクトルをセット
		void SetUpVec(DirectX::SimpleMath::Vector3 &upVec);
		//　垂直方向視野角をセット
		void SetFovY(float fovY);
		//　アスペクト比をセット
		void SetAspect(int windowWidth,int windowHeight);
		//　手前の限界距離をセット
		void SetNearClip(float nearclip);
		//　奥の限界距離をセット
		void SetFarClip(float farclip);

	protected:
		//　メンバ変数を置くところ

		//　ビュー行列
		DirectX::SimpleMath::Matrix m_viewMatrix;
		//　視点座標
		DirectX::SimpleMath::Vector3 m_eyePos;
		//　注視点座標
		DirectX::SimpleMath::Vector3 m_refPos;
		//　上方向ベクトル
		DirectX::SimpleMath::Vector3 m_upVec;


		//　射影行列
		DirectX::SimpleMath::Matrix m_projMatrix;
		//　垂直方向視野角
		float m_fovY;
		//　画面横幅と縦幅の比率（アスペクト比）
		float m_aspect;
		//　手前の限界距離
		float m_nearclip;
		//　奥の限界距離
		float m_farclip;
};
