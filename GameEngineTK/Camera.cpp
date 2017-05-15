
#include "Camera.h"

using namespace DirectX;
using namespace SimpleMath;


/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="value">何かの数値</param>
/// <returns>計算結果を返す</returns>

Camera::Camera(int windowWidth,int windowHeight)
{
	//　メンバ変数の初期化
	m_eyePos = Vector3(0.0f, 1.0f, 1.0f);
	m_refPos = Vector3(0.0f, 0.0f, 0.0f);
	m_upVec = Vector3(0.0f, 1.0f, 5.0f);
	m_upVec.Normalize();

	m_fovY = XMConvertToRadians(60.0f);
	m_aspect = (float)windowWidth / windowHeight;
	m_nearclip = 0.1f;
	m_farclip = 1000.0f;

	//　ビュー行列を生成
	m_viewMatrix = Matrix::Identity;
	//　射影行列を生成
	m_projMatrix = Matrix::Identity;
}

Camera::~Camera()
{

}


void Camera::Update()
{
	//　ビュー行列を生成
	m_viewMatrix = Matrix::CreateLookAt(m_eyePos, m_refPos, m_upVec);

	//　射影行列を生成
	m_projMatrix = Matrix::CreatePerspectiveFieldOfView(m_fovY, m_aspect, m_nearclip, m_farclip);
}

Matrix Camera::GetViewMatrix()
{
	return m_viewMatrix;
}


Matrix Camera::GetProjectionMatrix()
{
	return m_projMatrix;
}

void Camera::SetEyePos(Vector3 eyePos)
{
	m_eyePos = eyePos;
}

void Camera::SetRefPos(Vector3 refPos)
{
	m_refPos = refPos;
}