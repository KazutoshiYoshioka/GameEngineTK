#include "Collision.h"

using namespace DirectX::SimpleMath;

bool CheckSphere2Sphere(const Sphere& sphereA, const Sphere& sphereB)
{
	//�@��A��B�̒��S���W�̍����v�Z����
	Vector3 sub = sphereB.Center - sphereA.Center;

	//�@�O�����̒藝�ŁA�x�N�g���̒������v�Z����
	//�@�����x�N�g���̒����́A�Q�_�Ԃ̋���
	float distanceSquare = sub.x * sub.x + sub.y * sub.y + sub.z * sub.z;

	float radiusSquare = sphereA.Radius + sphereB.Radius;
	radiusSquare = radiusSquare * radiusSquare;

	//�@���������a�̘a���傫����Γ������Ă��Ȃ�
	if (distanceSquare > sphereA.Radius + sphereB.Radius)
	{
		return false;
	}
	return true;
}
