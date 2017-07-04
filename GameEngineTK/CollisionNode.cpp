
#include "CollisionNode.h"

using namespace DirectX::SimpleMath;

bool CollisionNode::m_DebugVisible;

//
void CollisionNode::SetParent(Obj3d* parent)
{
	m_Obj.SetObjectParent(parent);
}


//�@�R���X�g���N�^
SphereNode::SphereNode()
{
	//�@�f�t�H���g���a
	m_LocalRadius = 1.0f;
	
}

void SphereNode::Initialize()
{
	m_Obj.LoadModel(L"Resources/SphereNode.cmo");
}


void SphereNode::Update()
{
	m_Obj.SetTranslation(m_Translation);
	m_Obj.SetScale(Vector3(m_LocalRadius));

	m_Obj.Update();

	{// ���苅�̗v�f���v�Z
		const Matrix& worldm = m_Obj.GetWorld();

		// ���f�����W�n�ł̒��S�_
		Vector3 center(0, 0, 0);
		Vector3 right(1, 0, 0);

		// ���[���h���W�ɕϊ�
		center = Vector3::Transform(center, worldm);
		right = Vector3::Transform(right, worldm);

		// ���苅�̗v�f����
		Sphere::Center = center;
		Sphere::Radius = Vector3::Distance(center, right);
	}
}


void SphereNode::Draw()
{
	m_Obj.Draw();
}