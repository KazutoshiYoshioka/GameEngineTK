//
//�@3D�I�u�W�F�N�g�̃N���X
//

//�@�w�b�_�t�@�C���̃C���N���[�h
#include "Obj3d.h"

using namespace DirectX;
using namespace SimpleMath;

//�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\
//�@�ÓI�����o�̎���
//�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\

//�@�J����
Camera*											m_pCamera;
//�@�ėp�X�e�[�g
std::unique_ptr<DirectX::CommonStates>			m_states;
//�@�f�o�C�X
Microsoft::WRL::ComPtr<ID3D11Device>			m_d3dDevice;
//�@�R���e�L�X�g
Microsoft::WRL::ComPtr<ID3D11DeviceContext>		m_d3dContext;
//�@�G�t�F�N�g�t�@�N�g��
std::unique_ptr<DirectX::EffectFactory>			m_factory;


void Obj3d::InitializeStatic(Camera * pCamera, 
	Microsoft::WRL::ComPtr<ID3D11Device> d3dDevice, 
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> d3dContext)
{
	m_pCamera = pCamera;
	m_d3dDevice = d3dDevice;
	m_d3dContext = d3dContext;

	m_states = std::make_unique<CommonStates>(m_d3dDevice.Get());
	m_factory = std::make_unique<EffectFactory>(m_d3dDevice.Get());
	m_factory->SetDirectory(L"Resources");
}

