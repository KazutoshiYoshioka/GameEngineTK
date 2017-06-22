#include "Collision.h"

using namespace DirectX::SimpleMath;

bool CheckSphere2Sphere(const Sphere& sphereA, const Sphere& sphereB, DirectX::SimpleMath::Vector3* _inter = nullptr);

bool CheckSphere2Sphere(const Sphere& sphereA, const Sphere& sphereB, Vector3* inter)
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

	//�@�ϐ��A�h���X���w�肵�Ă����ꍇ�̂݋^����_���v�Z����
	if (inter)
	{
		//�@��A�AB�̒��S���W���A���a�̔䗦�œ��������_���A�^����_�Ƃ���
		//�@B��A�̍����x�N�g��
		Vector3 sub = sphereA.Center - sphereB.Center;
		//�@B����^����_�ւ̃x�N�g��
		Vector3 BtoInter = sub * sphereB.Radius / (sphereA.Radius + sphereB.Radius);
		*inter = sphereB.Center + BtoInter;
	}

	return true;
}
