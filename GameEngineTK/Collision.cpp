#include "Collision.h"

using namespace DirectX::SimpleMath;

// ベクトルの長さの二乗を計算
float VectorLengthSQ(const Vector3& v)
{
	float lengthsq;

	// 三平方の定理
	lengthsq = v.x * v.x + v.y * v.y + v.z * v.z;

	return lengthsq;
}

// ２点間の距離の二乗を計算
float DistanceSQ3D(const Vector3& p1, const Vector3& p2)
{
	Vector3 sub;

	sub = p1 - p2;

	// ベクトルの長さを計算する
	float distanceSQ = VectorLengthSQ(sub);

	return distanceSQ;
}

bool CheckSphere2Sphere(const Sphere& sphereA, const Sphere& sphereB, Vector3* inter)
{
	//　球AとBの中心座標の差を計算する
	Vector3 sub = sphereB.Center - sphereA.Center;

	//　三平方の定理で、ベクトルの長さを計算する
	float distanceSQ = DistanceSQ3D(sphereA.Center, sphereB.Center);

	// 半径の和
	float radius_sum = sphereA.Radius + sphereB.Radius;
	float radius_sumSQ = radius_sum * radius_sum;

	// 距離の二乗が半径の和の二乗より大きければ当たっていない
	if (distanceSQ > radius_sumSQ)
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


/// <summary>
/// 三点の座標から三角形を計算（反時計回りに頂点を指定）
/// </summary>
/// <param name="_p0">点０</param>
/// <param name="_p1">点１</param>
/// <param name="_p2">点２</param>
/// <param name="_triangle">三角形（結果出力用）</param>
void ComputeTriangle(const Vector3& _p0, const Vector3& _p1, const Vector3& _p2,Triangle* _triangle)
{
	//　座標をコピー
	_triangle->P0 = _p0;
	_triangle->P1 = _p1;
	_triangle->P2 = _p2;

	//　法線ベクトルを計算
	Vector3 P0_P1 = _triangle->P1 - _triangle->P0;
	Vector3 P1_P2 = _triangle->P2 - _triangle->P1;

	//　外積で２辺に直角なベクトルを算出
	_triangle->Normal = P0_P1.Cross(P1_P2);
	//　単位ベクトルにする
	_triangle->Normal.Normalize();
}