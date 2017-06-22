
#include "CollisionNode.h"

using namespace DirectX::SimpleMath;

//
void CollisionNode::SetParent(Obj3d* parent)
{
	m_Obj.SetObjectParent(parent);
}


//　コンストラクタ
SphereNode::SphereNode()
{
	//　デフォルト半径
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
}


void SphereNode::Draw()
{
	m_Obj.Draw();
}