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
}

void FollowCamera::SetTargetPos(Vector3 &targetPos)
{
	m_targetPos = targetPos;
}

void FollowCamera::SetTargetAngle(float targetAngle)
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
	//　カメラの更新
	Camera::Update();
}

