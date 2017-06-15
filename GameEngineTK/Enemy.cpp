//--------------------------------------------------------------------------------------
// �t�@�C����: Enemy.h
// �쐬��:
// �쐬��:
// ����:
//--------------------------------------------------------------------------------------

#include "Enemy.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

/*==================================
�ړI�̊p�x�ւ̍ŒZ�p�x���擾�i���W�A���j
����	_angle0	�x�[�X�ƂȂ�p�x
_angle1	�ڕW�Ƃ���p�x
�߂�l	�����p�x
�p�x�O����p�x�P�ɍŒZ�R�[�X�Ō������ۂɉ��Z����p�x���擾����
===================================*/
static float GetShortAngleRad(float _angle0, float _angle1)
{
	float angle_sub;

	angle_sub = _angle1 - _angle0;	// �p�x�̍�
									// �����P�W�O�x(�΁j���傫�������ꍇ�A�t��]�̕����߂��̂ŁA�}�C�i�X�ɕϊ�
									// �ŏI�I��-180�`+180�x�͈̔͂ɁB
	if (angle_sub > XM_PI)
	{
		angle_sub -= XM_2PI;
	}
	else if (angle_sub < -XM_PI)
	{
		angle_sub += XM_2PI;
	}

	return angle_sub;
}

//-----------------------------------------------------------------------------
// �R���X�g���N�^
//-----------------------------------------------------------------------------
Enemy::Enemy()
	: m_cycle(0.0f)
	, m_Death(false)
{
	m_Timer = 60;
}

//-----------------------------------------------------------------------------
// �f�X�g���N�^
//-----------------------------------------------------------------------------
Enemy::~Enemy()
{
}

//-----------------------------------------------------------------------------
// ������
//-----------------------------------------------------------------------------
void Enemy::Initialize()
{
	m_Obj.resize(PARTS_NUM);
	m_Obj[PARTS_BODY].LoadModel(L"Resources/Tank.cmo");
	m_Obj[PARTS_BATTERY].LoadModel(L"Resources/Battery.cmo");
	m_Obj[PARTS_CANNON].LoadModel(L"Resources/Burst.cmo");
	m_Obj[PARTS_ENGINE].LoadModel(L"Resources/Engine.cmo");

	// �p�[�c�̐e�q�֌W���Z�b�g
	m_Obj[PARTS_BATTERY].SetObjectParent(
		&m_Obj[PARTS_BODY]);

	m_Obj[PARTS_CANNON].SetObjectParent(
		&m_Obj[PARTS_BODY]);

	m_Obj[PARTS_ENGINE].SetObjectParent(
		&m_Obj[PARTS_BODY]);


	// �e����̃I�t�Z�b�g�i���W�̂��炵���j���Z�b�g
	//�@�e���炸�炷
	m_Obj[PARTS_BODY].SetTranslation(Vector3(0, 0.0, 0));
	m_Obj[PARTS_BATTERY].SetTranslation(Vector3(0, 0.5, 0));
	m_Obj[PARTS_CANNON].SetTranslation(Vector3(0, 0.65, 0));
	m_Obj[PARTS_ENGINE].SetTranslation(Vector3(0, 0.65, 0.5));
	


	//
	m_Obj[PARTS_BATTERY].SetScale(Vector3(0.6, 0.6, 0.6));
	m_Obj[PARTS_CANNON].SetScale(Vector3(3, 1, 1));

	//
	m_Obj[PARTS_CANNON].SetRotation(Vector3(0, XMConvertToRadians(-90), 0));

	// �����z�u�����_��
	Vector3 pos;
	pos.x = (float)rand() / RAND_MAX * 20.0f - 10.0f;
	pos.y = 0.0f;
	pos.z = (float)rand() / RAND_MAX * 20.0f - 10.0f;
	m_Obj[0].SetTranslation(pos);

	m_angle = (float)(rand() % 360);

	m_Obj[0].SetRotation(Vector3(0, XMConvertToRadians(m_angle), 0));

	// �����蔻��
	/*m_CollisionNodeBody.Initialize();
	m_CollisionNodeBody.SetParent(&m_Obj[0]);
	m_CollisionNodeBody.SetTrans(Vector3(0, 0.3f, 0));
	m_CollisionNodeBody.SetLocalRadius(1.0f);*/

	//m_ObjShadow.LoadModel(L"Resources/shadow.cmo");
	//m_ObjShadow.SetTranslation(Vector3(0, -0.4f, 0));
	//m_ObjShadow.SetObjectParent(&m_Obj[0]);
}

//-----------------------------------------------------------------------------
// �X�V
//-----------------------------------------------------------------------------
void Enemy::Update()
{
	// ����ł����牽�����Ȃ�
	if (m_Death) return;

	// ����I�ɐi�s������ς���
	m_Timer--;
	if (m_Timer < 0)
	{
		m_Timer = 60;

		// �p�x��ύX
		float rnd = (float)rand() / RAND_MAX - 0.5f;
		rnd *= 180.0f;
		m_angle += rnd;
	}

	// ���킶��Ɗp�x�𔽉f
	{
		Vector3 rotv = m_Obj[0].GetRotation();
		float angle = GetShortAngleRad(rotv.y, XMConvertToRadians(m_angle));
		rotv.y += angle*0.01f;
		m_Obj[0].SetRotation(rotv);
	}

	// �@�̂̌����Ă�������ɐi��
	{
		// ���̍��W���擾
		Vector3 trans = m_Obj[0].GetTranslation();

		Vector3 move(0, 0, -0.05f);
		Vector3 rotv = m_Obj[0].GetRotation();
		Matrix rotm = Matrix::CreateRotationY(rotv.y);
		move = Vector3::TransformNormal(move, rotm);
		// ���W���ړ�
		trans += move;
		// �ړ���̍��W���Z�b�g
		m_Obj[0].SetTranslation(trans);
	}

	// �ړ��𔽉f���čs��X�V
	Calc();

//	m_CollisionNodeBody.Update();
}

//-----------------------------------------------------------------------------
// �s��X�V
//-----------------------------------------------------------------------------
void Enemy::Calc()
{
	// ����ł����牽�����Ȃ�
	if (m_Death) return;

	// �S�p�[�c���s��X�V
	for (int i = 0; i < PARTS_NUM; i++)
	{
		m_Obj[i].Update();
	}

	//m_ObjShadow.Update();
}

//-----------------------------------------------------------------------------
// �`��
//-----------------------------------------------------------------------------
void Enemy::Draw()
{
	// ����ł����牽�����Ȃ�
	if (m_Death) return;

	// �S�p�[�c���`��
	for (int i = 0; i < PARTS_NUM; i++)
	{
		m_Obj[i].Draw();
	}

	//m_CollisionNodeBody.Draw();

	//// �e�����Z�`��
	//m_ObjShadow.DrawSubtractive();
}