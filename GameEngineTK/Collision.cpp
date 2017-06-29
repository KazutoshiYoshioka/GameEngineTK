#include "Collision.h"

using namespace DirectX::SimpleMath;

// �x�N�g���̒����̓����v�Z
float VectorLengthSQ(const Vector3& v)
{
	float lengthsq;

	// �O�����̒藝
	lengthsq = v.x * v.x + v.y * v.y + v.z * v.z;

	return lengthsq;
}

// �Q�_�Ԃ̋����̓����v�Z
float DistanceSQ3D(const Vector3& p1, const Vector3& p2)
{
	Vector3 sub;

	sub = p1 - p2;

	// �x�N�g���̒������v�Z����
	float distanceSQ = VectorLengthSQ(sub);

	return distanceSQ;
}

bool CheckSphere2Sphere(const Sphere& sphereA, const Sphere& sphereB, Vector3* inter)
{
	//�@��A��B�̒��S���W�̍����v�Z����
	Vector3 sub = sphereB.Center - sphereA.Center;

	//�@�O�����̒藝�ŁA�x�N�g���̒������v�Z����
	float distanceSQ = DistanceSQ3D(sphereA.Center, sphereB.Center);

	// ���a�̘a
	float radius_sum = sphereA.Radius + sphereB.Radius;
	float radius_sumSQ = radius_sum * radius_sum;

	// �����̓�悪���a�̘a�̓����傫����Γ������Ă��Ȃ�
	if (distanceSQ > radius_sumSQ)
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


/// <summary>
/// �O�_�̍��W����O�p�`���v�Z�i�����v���ɒ��_���w��j
/// </summary>
/// <param name="_p0">�_�O</param>
/// <param name="_p1">�_�P</param>
/// <param name="_p2">�_�Q</param>
/// <param name="_triangle">�O�p�`�i���ʏo�͗p�j</param>
void ComputeTriangle(const Vector3& _p0, const Vector3& _p1, const Vector3& _p2,Triangle* _triangle)
{
	//�@���W���R�s�[
	_triangle->P0 = _p0;
	_triangle->P1 = _p1;
	_triangle->P2 = _p2;

	//�@�@���x�N�g�����v�Z
	Vector3 P0_P1 = _triangle->P1 - _triangle->P0;
	Vector3 P1_P2 = _triangle->P2 - _triangle->P1;

	//�@�O�ςłQ�ӂɒ��p�ȃx�N�g�����Z�o
	_triangle->Normal = P0_P1.Cross(P1_P2);
	//�@�P�ʃx�N�g���ɂ���
	_triangle->Normal.Normalize();
}