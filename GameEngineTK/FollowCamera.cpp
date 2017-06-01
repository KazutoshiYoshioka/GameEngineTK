//
//
//


//　ヘッダファイルのインクルード
#include "FollowCamera.h"

using namespace DirectX;
using namespace SimpleMath;

//　静的メンバ変数の初期化
const float CAMERA_DISTANCE = 5.0f;

FollowCamera::FollowCamera(int windowWidth, int windowHeight)
	:Camera(windowWidth, windowHeight)
{
	m_targetPos = Vector3::Zero;
	m_targetAngle = 0.0f;
	m_keyboard = nullptr;
	m_isFPS = false;
	InitializeTPS();
}

void FollowCamera::SetTargetPos(const Vector3 &targetPos)
{
	m_targetPos = targetPos;
}

void FollowCamera::SetTargetAngle(const float targetAngle)
{
	m_targetAngle = targetAngle;
}

// カメラ設定　※タンクの移動処理より後で行う
void FollowCamera::Update()
{
	 //　カメラ距離
	const float CAMERA_DISTANCE = 5.0f;
	//　視点、参照点
	Vector3 eyepos, refpos;

	//　キーボードの取得
	Keyboard::State keystate = m_keyboard->GetState();
	m_keyboardTracker.Update(keystate);

	if (m_keyboardTracker.IsKeyPressed(Keyboard::Keyboard::C))
	{
		// フラグを切り替える
		m_isFPS = !m_isFPS;
		if (!m_isFPS)
		{
			InitializeTPS();
		}
	}

	if (m_isFPS == true)
	{//FPSカメラ
		Vector3 position;
		//　自機の上方0.2mの位置にカメラを置く
		position = m_targetPos + Vector3(0, 1.6f, 0);
		//　参照点から視点への差分
		Vector3 cameraV(0, 0, -CAMERA_DISTANCE);
		//　自機の後ろに回り込むための回転
		Matrix rotCamera = Matrix::CreateRotationY(m_targetAngle);
		//　差分ベクトルを回転させる
		cameraV = Vector3::TransformNormal(cameraV, rotCamera);
		eyepos = position + cameraV * 0.1f;
		//　参照点座標を計算
		refpos = eyepos + cameraV;
	}

	if (m_isFPS == false)
	{//TPSカメラ
		//　ターゲットの座標は、自機の座標に追従
		refpos = m_targetPos + Vector3(0, 2.0f, 0);
		//　ターゲット座標からカメラ座標への差分
		Vector3 cameraV(0.0f, 0.0f, CAMERA_DISTANCE);


		//　カメラの視線方向の逆方向に回転
		Matrix rotCamera = Matrix::CreateRotationY(m_targetAngle);
		cameraV = Vector3::TransformNormal(cameraV, rotCamera);

		//　カメラ座標を計算
		eyepos = refpos + cameraV;

		eyepos = m_eyePos + (eyepos - m_eyePos)*0.05;
		refpos = m_refPos + (refpos - m_refPos)*0.05;
	}

	

	// カメラに情報を送る
	SetEyePos(eyepos);
	SetRefPos(refpos);
	//　カメラの更新
	Camera::Update();
}

void FollowCamera::SetKeyboard(DirectX::Keyboard *keyboard)
{
	m_keyboard = keyboard;
}

void FollowCamera::InitializeTPS()
{
	//　カメラ距離
	const float CAMERA_DISTANCE = 5.0f;
	Vector3 eyepos, refpos;

	//　ターゲットの座標は、自機の座標に追従
	refpos = m_targetPos + Vector3(0, 2.0f, 0);
	//　ターゲット座標からカメラ座標への差分
	Vector3 cameraV(0.0f, 0.0f, CAMERA_DISTANCE);


	//　カメラの視線方向の逆方向に回転
	Matrix rotCamera = Matrix::CreateRotationY(m_targetAngle);
	cameraV = Vector3::TransformNormal(cameraV, rotCamera);

	//　カメラ座標を計算
	eyepos = refpos + cameraV;

	// カメラに情報を送る
	SetEyePos(eyepos);
	SetRefPos(refpos);
}