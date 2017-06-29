//
//
//


#include "ModelEffect.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

ModelEffect::ModelEffect()
{
	
}

void ModelEffect::Initialize(const wchar_t* filename, int Endframe, Vector3 Startrotation, Vector3 Endrotation, Vector3 StartScale, Vector3 EndScale, Vector3 Pos)
{
	m_Obj.LoadModel(filename);
	m_Frame = 0;
	m_EndFrame = Endframe;
	m_StartRotation = Startrotation;
	m_EndRotation = Endrotation;
	m_StartScale = StartScale;
	m_EndScale = EndScale;
	m_Pos = Pos;

}


//�@�G�t�F�N�g����
bool ModelEffect::Entry()
{
	// �o�߃t���[�������J�E���g
	m_Frame++;
	if (m_Frame > m_EndFrame) return false;

	// �i�s�x��0�`1�͈̔͂Ɋ��Z
	float f = (float)m_EndFrame / m_Frame;

	// �p�x�̐��`�⊮
	Vector3 rotation = (m_EndRotation - m_StartRotation) / f;
	rotation += m_StartRotation;
	rotation.x = XMConvertToRadians(rotation.x);
	rotation.y = XMConvertToRadians(rotation.y);
	rotation.z = XMConvertToRadians(rotation.z);

	// �X�P�[���̐��`���
	Vector3 scale = (m_EndScale - m_StartScale) / f;
	scale += m_StartScale;


	m_Obj.SetTranslation(m_Pos);
	m_Obj.SetRotation(rotation);
	m_Obj.SetScale(scale);

	return true;
}

//�@�X�V����
void ModelEffect::Update()
{
	m_Obj.Update();
	if (Entry() == false)
	{
		m_Obj.SetScale(Vector3(0, 0, 0));
	}
}

//�@�`�揈��
void ModelEffect::Draw()
{
	m_Obj.Draw();
}