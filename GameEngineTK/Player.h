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

		// �p�[�c�𔭎�
		void FireBullet();

		//�@�e�̍đ��U
		void ReloadBullet();

		//�@�`��
		void Draw();

		//�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\
		//�@Getter
		//�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\
		//�@���s�ړ����擾
		const DirectX::SimpleMath::Vector3& GetTranslation() { return m_ObjPlayer[PLAYER_BODY].GetTranslation(); }
		//�@���[���h�s����擾
		const DirectX::SimpleMath::Matrix& GetWorld() { return m_ObjPlayer[PLAYER_BODY].GetWorld(); }
		//�@��]���擾
		const DirectX::SimpleMath::Vector3& GetRotation() { return m_ObjPlayer[PLAYER_BODY].GetRotation(); }

		//�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\
		//�@Setter
		//�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\
		// ���W��ݒ�
		void SetTranslation(const DirectX::SimpleMath::Vector3& trans) { m_ObjPlayer[PLAYER_BODY].SetTranslation(trans); }
		// ��]��ݒ�
		void SetRotation(const DirectX::SimpleMath::Vector3& rot) { m_ObjPlayer[PLAYER_BODY].SetRotation(rot); }
		//�@�e�ۂ̓����蔻�苅
		const SphereNode& GetCollisionNodeBullet() { return m_CollisionNodeBullet; }

	protected:
		std::vector<Obj3d> m_ObjPlayer;

		// �L�[�{�[�h
		DirectX::Keyboard* m_pKeyboard;
		DirectX::Keyboard::KeyboardStateTracker m_KeyboardTracker;

		bool m_FireFlag;
		DirectX::SimpleMath::Vector3 m_BulletVel;
		int m_FireCount;

		//�@�����蔻��
		SphereNode m_CollisionNodeBullet;
};