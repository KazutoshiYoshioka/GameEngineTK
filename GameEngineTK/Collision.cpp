#include "Collision.h"

using namespace DirectX::SimpleMath;

bool CheckSphere2Sphere(const Sphere& sphereA, const Sphere& sphereB)
{
	//　球AとBの中心座標の差を計算する
	Vector3 sub = sphereB.Center - sphereA.Center;

	//　三平方の定理で、ベクトルの長さを計算する
	//　差分ベクトルの長さは、２点間の距離
	float distanceSquare = sub.x * sub.x + sub.y * sub.y + sub.z * sub.z;

	float radiusSquare = sphereA.Radius + sphereB.Radius;
	radiusSquare = radiusSquare * radiusSquare;

	//　距離が半径の和より大きければ当たっていない
	if (distanceSquare > sphereA.Radius + sphereB.Radius)
	{
		return false;
	}
	return true;
}
