//
//　概要：衝突判定
//
//　日付：
//
//　名前
//―――――――――――――――――――――――――――――――――――――――

#pragma once

#include <d3d11_1.h>
#include <SimpleMath.h>

//―――――――――――――――――
//　球
//―――――――――――――――――
class Sphere
{
	public:
		DirectX::SimpleMath::Vector3 Center;	//　中心座標
		float Radius;							//　半径

		//　コンストラクタ
		Sphere()
		{
			Radius = 1.0f;
		}
};


//――――――――――――――――――
//　線分
//――――――――――――――――――
class Segment
{
	public:
		DirectX::SimpleMath::Vector3 Start;	//　始点座標
		DirectX::SimpleMath::Vector3 End;	//　終点座標
};

//――――――――――――――――――
//　カプセル（球をスウィーブした形状）
//――――――――――――――――――
class Capsule
{
	public:
		Segment Segment;	//　芯線
		float	Radius;		//　半径
		//　コンストラクタ
		Capsule()
		{
			Segment.Start = DirectX::SimpleMath::Vector3(0, 0, 0);
			Segment.End = DirectX::SimpleMath::Vector3(0, 1, 0);
			Radius = 1.0f;
		}

};

bool CheckSphere2Sphere(const Sphere& sphereA, const Sphere& sphereB, DirectX::SimpleMath::Vector3* _inter = nullptr);