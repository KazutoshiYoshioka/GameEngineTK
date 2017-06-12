
#include "Player.h"

using namespace DirectX;
using namespace SimpleMath;

Player::Player(Keyboard* pKey)
{
	m_pKeyboard = pKey;

	Initialize();
}

void Player::Initialize()
{
	//�@���f���̃��[�h
	m_ObjPlayer.resize(PLAYER_PARTS_NUM);
	m_ObjPlayer[PLAYER_BODY].LoadModel(L"Resources/Body.cmo");
	m_ObjPlayer[PLAYER_SHIP].LoadModel(L"Resources/Ship.cmo");
	m_ObjPlayer[PLAYER_WING].LoadModel(L"Resources/Wing.cmo");
	m_ObjPlayer[PLAYER_ENGINE].LoadModel(L"Resources/Engine.cmo");
	m_ObjPlayer[PLAYER_CANNON].LoadModel(L"Resources/Cannon.cmo");

	//�@�e�q�֌W
	m_ObjPlayer[PLAYER_SHIP].SetObjectParent(&m_ObjPlayer[PLAYER_BODY]);
	m_ObjPlayer[PLAYER_WING].SetObjectParent(&m_ObjPlayer[PLAYER_BODY]);
	m_ObjPlayer[PLAYER_ENGINE].SetObjectParent(&m_ObjPlayer[PLAYER_BODY]);
	m_ObjPlayer[PLAYER_CANNON].SetObjectParent(&m_ObjPlayer[PLAYER_BODY]);

	//�@�e���炸�炷
	m_ObjPlayer[PLAYER_BODY].SetTranslation(Vector3(0, 1.0, 0));
	m_ObjPlayer[PLAYER_SHIP].SetTranslation(Vector3(0, -0.5, 0));
	m_ObjPlayer[PLAYER_WING].SetTranslation(Vector3(1, 0, 0));
	m_ObjPlayer[PLAYER_ENGINE].SetTranslation(Vector3(1, 0.35, 0));
	m_ObjPlayer[PLAYER_CANNON].SetTranslation(Vector3(1, 0.35, 0));
}

void Player::Update()
{
	Keyboard::State keystate = m_pKeyboard->GetState();
	m_KeyboardTracker.Update(keystate);


	Vector3 pos = m_ObjPlayer[PLAYER_BODY].GetTranslation();
	
	m_ObjPlayer[PLAYER_BODY].SetTranslation(pos);

	// �P�t���[���ł̐��񑬓x<���W�A��>
	const float ROT_SPEED = 0.03f;

	//�@���񏈗�
	if (keystate.IsKeyDown(Keyboard::Keys::A))
	{
		// ���݂̊p�x���擾
		Vector3 rot = m_ObjPlayer[PLAYER_BODY].GetRotation();
		rot.y += ROT_SPEED;
		//�@��]��̊p�x�𔽉f
		m_ObjPlayer[PLAYER_BODY].SetRotation(rot);
	}
	if (keystate.IsKeyDown(Keyboard::Keys::D))
	{
		// ���݂̊p�x���擾
		Vector3 rot =m_ObjPlayer[PLAYER_BODY].GetRotation();
		rot.y -= ROT_SPEED;
		//	 ��]��̊p�x�𔽉f
		m_ObjPlayer[PLAYER_BODY].SetRotation(rot);
	}

	//�@�O�i�A���
	if (keystate.IsKeyDown(Keyboard::Keys::W))
	{
		//�@���݂̍��W�E��]�p���擾
		Vector3 trans = m_ObjPlayer[PLAYER_BODY].GetTranslation();
		float rot_y = m_ObjPlayer[PLAYER_BODY].GetRotation().y;
		//�@�ړ��x�N�g���iZ���W�O�i�j
		Vector3 moveV(0, 0, -1.0f);
		Matrix rotm = Matrix::CreateRotationY(rot_y);
		//�@�ړ��x�N�g������]����
		moveV = Vector3::TransformNormal(moveV, rotm);
		//�@�ړ�
		trans += moveV;
		//�@�ړ��������W�𔽉f
		m_ObjPlayer[PLAYER_BODY].SetTranslation(trans);
	}
	if (keystate.IsKeyDown(Keyboard::Keys::S))
	{
		// ���݂̍��W�E��]�p���擾
		Vector3 trans = m_ObjPlayer[PLAYER_BODY].GetTranslation();
		float rot_y = m_ObjPlayer[PLAYER_BODY].GetRotation().y;
		// �ړ��x�N�g��(Z���W���)
		Vector3 moveV(0, 0, +0.1f);
		Matrix rotm = Matrix::CreateRotationY(rot_y);
		// �ړ��x�N�g������]����
		moveV = Vector3::TransformNormal(moveV, rotm);
		// �ړ�
		trans += moveV;
		// �ړ��������W�𔽉f
		m_ObjPlayer[PLAYER_BODY].SetTranslation(trans);
	}

	Calc();
}

//-----------------------------------------------------------------------------
// �s��X�V
//-----------------------------------------------------------------------------
void Player::Calc()
{
	// �S�p�[�c���s��X�V
	for (int i = 0; i < PLAYER_PARTS_NUM; i++)
	{
		m_ObjPlayer[i].Update();
	}
}

//-----------------------------------------------------------------------------
// �`��
//-----------------------------------------------------------------------------
void Player::Draw()
{
	// �S�p�[�c���`��
	for (int i = 0; i < PLAYER_PARTS_NUM; i++)
	{
		m_ObjPlayer[i].Draw();
	}
}

