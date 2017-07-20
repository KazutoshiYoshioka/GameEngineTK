#pragma once

#include <d3d11_1.h>
#include <SimpleMath.h>
#include <Keyboard.h>
#include <vector>

#include "Obj3d.h"
#include "CollisionNode.h"

class Player
{		
	public:
		//�@�d�͉����x
		const float GRAVITY_ACC = 0.03f;
		//�@�W�����v����
		const float JUMP_SPEED_FIRST = 0.5f;
		//�@�W�����v�ō���
		const float JUMP_SPEED_MAX = 0.3f;

		//�@�p�[�c
		enum 
		{
			PLAYER_BODY,	//�@�@��
			PLAYER_SHIP,	//�@�D��
			PLAYER_WING,	//�@��
			PLAYER_ENGINE,	//�@�G���W��
			PLAYER_CANNON,	//�@�C��

			PLAYER_PARTS_NUM
		};


		//�@�R���X�g���N�^
		Player(DirectX::Keyboard* pKey);

		//�f�X�g���N�^
		//~Player();

		//�@������
		void Initialize();

		//�@�X�V����
		void Update();

		//�@�s��̌v�Z
		void Calc();

		//�@�`��
		void Draw();

		// �p�[�c�𔭎�
		void FireBullet();

		//�@�e�̍đ��U
		void ReloadBullet();

		//�@�W�����v�J�n
		void StartJump();
		//�@�d��
		void StartFall();
		//�@
		void StopJump();

		//�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\
		//�@Getter
		//�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\
		//�@���s�ړ����擾
		const DirectX::SimpleMath::Vector3& GetTranslation() { return m_ObjPlayer[PLAYER_BODY].GetTranslation(); }
		//�@���[���h�s����擾
		const DirectX::SimpleMath::Matrix& GetWorld() { return m_ObjPlayer[PLAYER_BODY].GetWorld(); }
		//�@��]���擾
		const DirectX::SimpleMath::Vector3& GetRotation() { return m_ObjPlayer[PLAYER_BODY].GetRotation(); }
		//�@�e�ۂ̓����蔻�苅
		const SphereNode& GetCollisionNodeBullet() { return m_CollisionNodeBullet; }
		// �e�ۂ̓����蔻�苅���擾
		const SphereNode& GetCollisionNodeBody() { return m_CollisionNodeBody; }
		//�@���x���擾
		const DirectX::SimpleMath::Vector3& GetVelocity() { return m_Velocity; }

		//�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\
		//�@Setter
		//�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\
		// ���W��ݒ�
		void SetTranslation(const DirectX::SimpleMath::Vector3& trans) { m_ObjPlayer[PLAYER_BODY].SetTranslation(trans); }
		// ��]��ݒ�
		void SetRotation(const DirectX::SimpleMath::Vector3& rot) { m_ObjPlayer[PLAYER_BODY].SetRotation(rot); }

	protected:
		std::vector<Obj3d> m_ObjPlayer;

		// �L�[�{�[�h
		DirectX::Keyboard* m_pKeyboard;
		DirectX::Keyboard::KeyboardStateTracker m_KeyboardTracker;

		bool m_FireFlag;
		DirectX::SimpleMath::Vector3 m_BulletVel;
		int m_FireCount;

		//�@�U���p�����蔻��
		SphereNode m_CollisionNodeBullet;

		//�@�S�̂̓����蔻��
		SphereNode m_CollisionNodeBody;

		//�@���x�x�N�g��
		DirectX::SimpleMath::Vector3 m_Velocity;
		//�@�W�����v���t���O
		bool m_isJump;
};