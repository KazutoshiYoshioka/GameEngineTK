
#include "Camera.h"

using namespace DirectX;
using namespace SimpleMath;


/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="value">�����̐��l</param>
/// <returns>�v�Z���ʂ�Ԃ�</returns>

Camera::Camera(int windowWidth,int windowHeight)
{
	//�@�����o�ϐ��̏�����
	m_eyePos = Vector3(0.0f, 1.0f, 1.0f);
	m_refPos = Vector3(0.0f, 0.0f, 0.0f);
	m_upVec = Vector3(0.0f, 1.0f, 5.0f);
	m_upVec.Normalize();

	m_fovY = XMConvertToRadians(60.0f);
	m_aspect = (float)windowWidth / windowHeight;
	m_nearclip = 0.1f;
	m_farclip = 1000.0f;

	//�@�r���[�s��𐶐�
	m_viewMatrix = Matrix::Identity;
	//�@�ˉe�s��𐶐�
	m_projMatrix = Matrix::Identity;
}

Camera::~Camera()
{

}


void Camera::Update()
{
	//�@�r���[�s��𐶐�
	m_viewMatrix = Matrix::CreateLookAt(m_eyePos, m_refPos, m_upVec);

	//�@�ˉe�s��𐶐�
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