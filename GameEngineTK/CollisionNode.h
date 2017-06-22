#pragma once


#include "Collision.h"
#include "Obj3d.h"

//　あたり判定ノード

class CollisionNode
{
	public:
		//　初期化
		virtual void Initialize() = 0;
		//　毎フレーム更新
		virtual void Update() = 0;
		//　描画
		virtual void Draw() = 0;
		//　親をセット
		void SetParent(Obj3d* parent);
		// 親からのオフセット
		void SetTrans(DirectX::SimpleMath::Vector3& trans) { m_Translation = trans; }
	protected:
		//　デバッグ表示オブジェクト
		Obj3d m_Obj;
		//　親からのオフセット
		DirectX::SimpleMath::Vector3 m_Translation;
};

class SphereNode :public CollisionNode, public Sphere
{
	public:
		SphereNode();
		//　初期化
		void Initialize();
		//　毎フレーム更新
		void Update();
		//　描画
		void Draw();
		//　半径のセッター
		void SetLocalRadius(float Radius) { m_LocalRadius = Radius; }
	protected:
		//　ローカルの半径
		float m_LocalRadius;
};


