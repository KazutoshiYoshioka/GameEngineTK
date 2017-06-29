//
//�@���f���G�t�F�N�g
//

#pragma once

#include <d3d11_1.h>
#include <SimpleMath.h>
#include <list>
#include "Obj3D.h"

class ModelEffect
{
	public:
		using Vector3 = DirectX::SimpleMath::Vector3;

		//�@�R���X�g���N�^
		ModelEffect();

		void Initialize(const wchar_t* filename, int Endframe, Vector3 Startrotation, Vector3 Endrotation, Vector3 StartScale, Vector3 EndScale,Vector3 Pos);

		// ����������
		bool Entry();
		
		//�@�X�V����
		void Update();

		//�@�`��
		void Draw();

		////�@�e���Z�b�g
		//void SetParent(Obj3d* parent) { m_Obj.SetObjectParent(parent); }
		//// �e����̃I�t�Z�b�g
		//void SetTrans(DirectX::SimpleMath::Vector3& trans) { m_Translation = trans; }

	protected:
		// �R�c�I�u�W�F�N�g
		Obj3d	m_Obj;

		// ���݃t���[��
		int		m_Frame;
		// �I���t���[��
		int		m_EndFrame;

		//�@��]
		Vector3	m_StartRotation;

		Vector3 m_EndRotation;

		//�@�����g�嗦
		Vector3	m_StartScale;

		//�@�ŏI�g�嗦
		Vector3	m_EndScale;

		Vector3 m_Pos;
};