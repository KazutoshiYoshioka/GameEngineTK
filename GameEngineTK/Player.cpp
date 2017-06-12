
#include "Player.h"

using namespace DirectX;
using namespace SimpleMath;

Player::Player(Keyboard* pKey)
{
	m_pKeyboard = pKey;

	Initialize();
}

void Player::Initialize()
{
	//　モデルのロード
	m_ObjPlayer.resize(PLAYER_PARTS_NUM);
	m_ObjPlayer[PLAYER_BODY].LoadModel(L"Resources/Body.cmo");
	m_ObjPlayer[PLAYER_SHIP].LoadModel(L"Resources/Ship.cmo");
	m_ObjPlayer[PLAYER_WING].LoadModel(L"Resources/Wing.cmo");
	m_ObjPlayer[PLAYER_ENGINE].LoadModel(L"Resources/Engine.cmo");
	m_ObjPlayer[PLAYER_CANNON].LoadModel(L"Resources/Cannon.cmo");

	//　親子関係
	m_ObjPlayer[PLAYER_SHIP].SetObjectParent(&m_ObjPlayer[PLAYER_BODY]);
	m_ObjPlayer[PLAYER_WING].SetObjectParent(&m_ObjPlayer[PLAYER_BODY]);
	m_ObjPlayer[PLAYER_ENGINE].SetObjectParent(&m_ObjPlayer[PLAYER_BODY]);
	m_ObjPlayer[PLAYER_CANNON].SetObjectParent(&m_ObjPlayer[PLAYER_BODY]);

	//　親からずらす
	m_ObjPlayer[PLAYER_BODY].SetTranslation(Vector3(0, 1.0, 0));
	m_ObjPlayer[PLAYER_SHIP].SetTranslation(Vector3(0, -0.5, 0));
	m_ObjPlayer[PLAYER_WING].SetTranslation(Vector3(1, 0, 0));
	m_ObjPlayer[PLAYER_ENGINE].SetTranslation(Vector3(1, 0.35, 0));
	m_ObjPlayer[PLAYER_CANNON].SetTranslation(Vector3(1, 0.35, 0));
}

void Player::Update()
{
	Keyboard::State keystate = m_pKeyboard->GetState();
	m_KeyboardTracker.Update(keystate);


	Vector3 pos = m_ObjPlayer[PLAYER_BODY].GetTranslation();
	
	m_ObjPlayer[PLAYER_BODY].SetTranslation(pos);

	// １フレームでの旋回速度<ラジアン>
	const float ROT_SPEED = 0.03f;

	//　旋回処理
	if (keystate.IsKeyDown(Keyboard::Keys::A))
	{
		// 現在の角度を取得
		Vector3 rot = m_ObjPlayer[PLAYER_BODY].GetRotation();
		rot.y += ROT_SPEED;
		//　回転後の角度を反映
		m_ObjPlayer[PLAYER_BODY].SetRotation(rot);
	}
	if (keystate.IsKeyDown(Keyboard::Keys::D))
	{
		// 現在の角度を取得
		Vector3 rot =m_ObjPlayer[PLAYER_BODY].GetRotation();
		rot.y -= ROT_SPEED;
		//	 回転後の角度を反映
		m_ObjPlayer[PLAYER_BODY].SetRotation(rot);
	}

	//　前進、後退
	if (keystate.IsKeyDown(Keyboard::Keys::W))
	{
		//　現在の座標・回転角を取得
		Vector3 trans = m_ObjPlayer[PLAYER_BODY].GetTranslation();
		float rot_y = m_ObjPlayer[PLAYER_BODY].GetRotation().y;
		//　移動ベクトル（Z座標前進）
		Vector3 moveV(0, 0, -1.0f);
		Matrix rotm = Matrix::CreateRotationY(rot_y);
		//　移動ベクトルを回転する
		moveV = Vector3::TransformNormal(moveV, rotm);
		//　移動
		trans += moveV;
		//　移動した座標を反映
		m_ObjPlayer[PLAYER_BODY].SetTranslation(trans);
	}
	if (keystate.IsKeyDown(Keyboard::Keys::S))
	{
		// 現在の座標・回転角を取得
		Vector3 trans = m_ObjPlayer[PLAYER_BODY].GetTranslation();
		float rot_y = m_ObjPlayer[PLAYER_BODY].GetRotation().y;
		// 移動ベクトル(Z座標後退)
		Vector3 moveV(0, 0, +0.1f);
		Matrix rotm = Matrix::CreateRotationY(rot_y);
		// 移動ベクトルを回転する
		moveV = Vector3::TransformNormal(moveV, rotm);
		// 移動
		trans += moveV;
		// 移動した座標を反映
		m_ObjPlayer[PLAYER_BODY].SetTranslation(trans);
	}

	Calc();
}

//-----------------------------------------------------------------------------
// 行列更新
//-----------------------------------------------------------------------------
void Player::Calc()
{
	// 全パーツ分行列更新
	for (int i = 0; i < PLAYER_PARTS_NUM; i++)
	{
		m_ObjPlayer[i].Update();
	}
}

//-----------------------------------------------------------------------------
// 描画
//-----------------------------------------------------------------------------
void Player::Draw()
{
	// 全パーツ分描画
	for (int i = 0; i < PLAYER_PARTS_NUM; i++)
	{
		m_ObjPlayer[i].Draw();
	}
}

