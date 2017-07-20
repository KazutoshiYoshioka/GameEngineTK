
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
	m_FireFlag = false;
	m_isJump = false;

	//�@���f���̃��[�h
	m_ObjPlayer.resize(PLAYER_PARTS_NUM);
	m_ObjPlayer[PLAYER_BODY].LoadModel(L"Resources/Tank.cmo");
	m_ObjPlayer[PLAYER_SHIP].LoadModel(L"Resources/Battery.cmo");
	m_ObjPlayer[PLAYER_WING].LoadModel(L"Resources/Wing.cmo");
	m_ObjPlayer[PLAYER_ENGINE].LoadModel(L"Resources/Engine.cmo");
	m_ObjPlayer[PLAYER_CANNON].LoadModel(L"Resources/Burst.cmo");

	//�@�e�q�֌W
	m_ObjPlayer[PLAYER_SHIP].SetObjectParent(&m_ObjPlayer[PLAYER_BODY]);
	m_ObjPlayer[PLAYER_WING].SetObjectParent(&m_ObjPlayer[PLAYER_BODY]);
	m_ObjPlayer[PLAYER_ENGINE].SetObjectParent(&m_ObjPlayer[PLAYER_BODY]);
	m_ObjPlayer[PLAYER_CANNON].SetObjectParent(&m_ObjPlayer[PLAYER_BODY]);

	//�@�e���炸�炷
	m_ObjPlayer[PLAYER_BODY].SetTranslation(Vector3(0, 0.0, 0));
	m_ObjPlayer[PLAYER_SHIP].SetTranslation(Vector3(0, 0.5, 0));
	m_ObjPlayer[PLAYER_WING].SetTranslation(Vector3(1, 30, 0));
	m_ObjPlayer[PLAYER_ENGINE].SetTranslation(Vector3(0, 0.65, 0.5));
	m_ObjPlayer[PLAYER_CANNON].SetTranslation(Vector3(0, 0.65, 0));


	//
	m_ObjPlayer[PLAYER_SHIP].SetScale(Vector3(0.6,0.6,0.6));
	m_ObjPlayer[PLAYER_CANNON].SetScale(Vector3(3, 1, 1));

	//
	m_ObjPlayer[PLAYER_CANNON].SetRotation(Vector3(0, XMConvertToRadians(-90), 0));

	//�@������
	m_CollisionNodeBullet.Initialize();
	//�@���@�̃x�[�X�ƂȂ�p�[�c�ɂԂ牺����
	m_CollisionNodeBullet.SetParent(&m_ObjPlayer[PLAYER_ENGINE]);
	//�@���킩��̃I�t�Z�b�g
	m_CollisionNodeBullet.SetTrans(Vector3(0, 0, 0.3));
	//�@�����蔻��̔��a
	m_CollisionNodeBullet.SetLocalRadius(0.3f);

	// �����蔻��
	m_CollisionNodeBody.Initialize();
	m_CollisionNodeBody.SetParent(&m_ObjPlayer[0]);
	m_CollisionNodeBody.SetLocalRadius(0.5f);
	m_CollisionNodeBody.SetTrans(Vector3(0, 0.5f, 0));
}

void Player::Update()
{
	Keyboard::State keystate = m_pKeyboard->GetState();
	m_KeyboardTracker.Update(keystate);

	//�@�X�y�[�X����������W�����v
	if (m_KeyboardTracker.IsKeyPressed(Keyboard::Keys::Space))
	{
		//�@�W�����v�J�n
		StartJump();
	}
	//�@�W�����v���Ȃ�
	if (m_isJump)
	{
		//�@�������ɉ���
		m_Velocity.y -= GRAVITY_ACC;
		if (m_Velocity.y <= -JUMP_SPEED_MAX)
		{
			m_Velocity.y = -JUMP_SPEED_MAX;
		}
	}
	Vector3 trans = this->GetTranslation();
	trans += m_Velocity;
	this->SetTranslation(trans);

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
		Vector3 moveV(0, 0, -0.1f);
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

	if (m_KeyboardTracker.IsKeyPressed(Keyboard::Keys::F))
	{
		if (m_FireFlag)
		{
			ReloadBullet();
		}
		else
		{
			FireBullet();
		}
	}

	if (m_FireFlag)
	{
		Vector3 trans = m_ObjPlayer[PLAYER_ENGINE].GetTranslation();

		trans += m_BulletVel;

		m_ObjPlayer[PLAYER_ENGINE].SetTranslation(trans);

		if (--m_FireCount < 0)
		{
			ReloadBullet();
		}
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
	//�@�����蔻��̍X�V
	m_CollisionNodeBullet.Update();
	m_CollisionNodeBody.Update();
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
	m_CollisionNodeBullet.Draw();
	m_CollisionNodeBody.Draw();
}


//

//
void Player::FireBullet()
{
	//�@���ɔ��˒�
	if (m_FireFlag)return;

	m_FireFlag = true;

	Matrix worldm = m_ObjPlayer[PLAYER_ENGINE].GetWorld();

	Vector3 scale;
	Quaternion rotq;
	Vector3 pos;

	Vector3* m0 = (Vector3*)&worldm.m[0];
	Vector3* m1 = (Vector3*)&worldm.m[1];
	Vector3* m2 = (Vector3*)&worldm.m[2];
	Vector3* m3 = (Vector3*)&worldm.m[3];

	pos = *m3;

	scale = Vector3(m0->Length(), m1->Length(), m2->Length());

	m0->Normalize();
	m1->Normalize();
	m2->Normalize();

	rotq = Quaternion::CreateFromRotationMatrix(worldm);

	m_ObjPlayer[PLAYER_ENGINE].SetObjectParent(nullptr);
	m_ObjPlayer[PLAYER_ENGINE].SetScale(scale);
	m_ObjPlayer[PLAYER_ENGINE].SetRotationQ(rotq);
	m_ObjPlayer[PLAYER_ENGINE].SetTranslation(pos);

	m_BulletVel = Vector3(0, 0.0f, -0.3f);

	m_BulletVel = Vector3::Transform(m_BulletVel, rotq);

	m_FireCount = 120;
}

//
//
//
void Player::ReloadBullet()
{
	if (!m_FireFlag)return;

	m_ObjPlayer[PLAYER_ENGINE].SetObjectParent(&m_ObjPlayer[PLAYER_BODY]);

	m_ObjPlayer[PLAYER_ENGINE].SetScale(Vector3(1, 1, 1));
	m_ObjPlayer[PLAYER_ENGINE].SetRotation(Vector3(0, 0, 0));
	m_ObjPlayer[PLAYER_ENGINE].SetTranslation(Vector3(0, 0.65, 0.5));

	m_FireFlag = false;
}


// �W�����v���J�n����
void Player::StartJump()
{
	// �W�����v���łȂ���
	if (!m_isJump)
	{
		// ������̏�����ݒ�
		m_Velocity.y = JUMP_SPEED_FIRST;
		// �W�����v�t���O�𗧂Ă�
		m_isJump = true;
	}
}

// �W�����v���J�n����
void Player::StartFall()
{
	// �W�����v���łȂ���
	if (!m_isJump)
	{
		// ������̏�����ݒ�
		m_Velocity.y = 0.0f;
		// �W�����v�t���O�𗧂Ă�
		m_isJump = true;
	}
}

//�@�W�����v�I��
void Player::StopJump()
{
	m_isJump = false;
	m_Velocity = Vector3::Zero;
}

