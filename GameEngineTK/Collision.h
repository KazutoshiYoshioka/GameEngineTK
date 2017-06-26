//
//�@�T�v�F�Փ˔���
//
//�@���t�F
//
//�@���O
//�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\

#pragma once

#include <d3d11_1.h>
#include <SimpleMath.h>

//�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\
//�@��
//�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\
class Sphere
{
	public:
		DirectX::SimpleMath::Vector3 Center;	//�@���S���W
		float Radius;							//�@���a

		//�@�R���X�g���N�^
		Sphere()
		{
			Radius = 1.0f;
		}
};


//�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\
//�@����
//�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\
class Segment
{
	public:
		DirectX::SimpleMath::Vector3 Start;	//�@�n�_���W
		DirectX::SimpleMath::Vector3 End;	//�@�I�_���W
};

//�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\
//�@�J�v�Z���i�����X�E�B�[�u�����`��j
//�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\
class Capsule
{
	public:
		Segment Segment;	//�@�c��
		float	Radius;		//�@���a
		//�@�R���X�g���N�^
		Capsule()
		{
			Segment.Start = DirectX::SimpleMath::Vector3(0, 0, 0);
			Segment.End = DirectX::SimpleMath::Vector3(0, 1, 0);
			Radius = 1.0f;
		}

};

bool CheckSphere2Sphere(const Sphere& sphereA, const Sphere& sphereB, DirectX::SimpleMath::Vector3* _inter = nullptr);