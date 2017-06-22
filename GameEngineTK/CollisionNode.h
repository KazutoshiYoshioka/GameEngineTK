#pragma once


#include "Collision.h"
#include "Obj3d.h"

//�@�����蔻��m�[�h

class CollisionNode
{
	public:
		//�@������
		virtual void Initialize() = 0;
		//�@���t���[���X�V
		virtual void Update() = 0;
		//�@�`��
		virtual void Draw() = 0;
		//�@�e���Z�b�g
		void SetParent(Obj3d* parent);
		// �e����̃I�t�Z�b�g
		void SetTrans(DirectX::SimpleMath::Vector3& trans) { m_Translation = trans; }
	protected:
		//�@�f�o�b�O�\���I�u�W�F�N�g
		Obj3d m_Obj;
		//�@�e����̃I�t�Z�b�g
		DirectX::SimpleMath::Vector3 m_Translation;
};

class SphereNode :public CollisionNode, public Sphere
{
	public:
		SphereNode();
		//�@������
		void Initialize();
		//�@���t���[���X�V
		void Update();
		//�@�`��
		void Draw();
		//�@���a�̃Z�b�^�[
		void SetLocalRadius(float Radius) { m_LocalRadius = Radius; }
	protected:
		//�@���[�J���̔��a
		float m_LocalRadius;
};


