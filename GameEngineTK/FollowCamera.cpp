//
//
//


//�@�w�b�_�t�@�C���̃C���N���[�h
#include "FollowCamera.h"

using namespace DirectX;
using namespace SimpleMath;

//�@�ÓI�����o�ϐ��̏�����
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

// �J�����ݒ�@���^���N�̈ړ���������ōs��
void FollowCamera::Update()
{
	 //�@�J��������
	const float CAMERA_DISTANCE = 5.0f;
	//�@���_�A�Q�Ɠ_
	Vector3 eyepos, refpos;

	//�@�^�[�Q�b�g�̍��W�́A���@�̍��W�ɒǏ]
	refpos = m_targetPos + Vector3(0, 2.0f, 0);
	//�@�^�[�Q�b�g���W����J�������W�ւ̍���
	Vector3 cameraV(0.0f, 0.0f, CAMERA_DISTANCE);


	//�@�J�����̎��������̋t�����ɉ�]
	Matrix rotCamera = Matrix::CreateRotationY(m_targetAngle);
	cameraV = Vector3::TransformNormal(cameraV, rotCamera);

	//�@�J�������W���v�Z
	eyepos = refpos + cameraV;

	// �J�����ɏ��𑗂�
	SetEyePos(eyepos);
	SetRefPos(refpos);
	//�@�J�����̍X�V
	Camera::Update();
}

