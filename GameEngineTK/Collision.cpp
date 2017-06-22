#include "Collision.h"

using namespace DirectX::SimpleMath;

bool CheckSphere2Sphere(const Sphere& sphereA, const Sphere& sphereB, DirectX::SimpleMath::Vector3* _inter = nullptr);

bool CheckSphere2Sphere(const Sphere& sphereA, const Sphere& sphereB, Vector3* inter)
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

	//　変数アドレスを指定していた場合のみ疑似交点を計算する
	if (inter)
	{
		//　球A、Bの中心座標を、半径の比率で内聞した点を、疑似交点とする
		//　BとAの差分ベクトル
		Vector3 sub = sphereA.Center - sphereB.Center;
		//　Bから疑似交点へのベクトル
		Vector3 BtoInter = sub * sphereB.Radius / (sphereA.Radius + sphereB.Radius);
		*inter = sphereB.Center + BtoInter;
	}

	return true;
}
