
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
	m_FireFlag = false;
	m_isJump = false;

	//　モデルのロード
	m_ObjPlayer.resize(PLAYER_PARTS_NUM);
	m_ObjPlayer[PLAYER_BODY].LoadModel(L"Resources/Tank.cmo");
	m_ObjPlayer[PLAYER_SHIP].LoadModel(L"Resources/Battery.cmo");
	m_ObjPlayer[PLAYER_WING].LoadModel(L"Resources/Wing.cmo");
	m_ObjPlayer[PLAYER_ENGINE].LoadModel(L"Resources/Engine.cmo");
	m_ObjPlayer[PLAYER_CANNON].LoadModel(L"Resources/Burst.cmo");

	//　親子関係
	m_ObjPlayer[PLAYER_SHIP].SetObjectParent(&m_ObjPlayer[PLAYER_BODY]);
	m_ObjPlayer[PLAYER_WING].SetObjectParent(&m_ObjPlayer[PLAYER_BODY]);
	m_ObjPlayer[PLAYER_ENGINE].SetObjectParent(&m_ObjPlayer[PLAYER_BODY]);
	m_ObjPlayer[PLAYER_CANNON].SetObjectParent(&m_ObjPlayer[PLAYER_BODY]);

	//　親からずらす
	m_ObjPlayer[PLAYER_BODY].SetTranslation(Vector3(0, 0.0, 0));
	m_ObjPlayer[PLAYER_SHIP].SetTranslation(Vector3(0, 0.5, 0));
	m_ObjPlayer[PLAYER_WING].SetTranslation(Vector3(1, 30, 0));
	m_ObjPlayer[PLAYER_ENGINE].SetTranslation(Vector3(0, 0.65, 0.5));
	m_ObjPlayer[PLAYER_CANNON].SetTranslation(Vector3(0, 0.65, 0));


	//
	m_ObjPlayer[PLAYER_SHIP].SetScale(Vector3(0.6,0.6,0.6));
	m_ObjPlayer[PLAYER_CANNON].SetScale(Vector3(3, 1, 1));

	//
	m_ObjPlayer[PLAYER_CANNON].SetRotation(Vector3(0, XMConvertToRadians(-90), 0));

	//　初期化
	m_CollisionNodeBullet.Initialize();
	//　自機のベースとなるパーツにぶら下げる
	m_CollisionNodeBullet.SetParent(&m_ObjPlayer[PLAYER_ENGINE]);
	//　武器からのオフセット
	m_CollisionNodeBullet.SetTrans(Vector3(0, 0, 0.3));
	//　あたり判定の半径
	m_CollisionNodeBullet.SetLocalRadius(0.3f);

	// 当たり判定
	m_CollisionNodeBody.Initialize();
	m_CollisionNodeBody.SetParent(&m_ObjPlayer[0]);
	m_CollisionNodeBody.SetLocalRadius(0.5f);
	m_CollisionNodeBody.SetTrans(Vector3(0, 0.5f, 0));
}

void Player::Update()
{
	Keyboard::State keystate = m_pKeyboard->GetState();
	m_KeyboardTracker.Update(keystate);

	//　スペースを押したらジャンプ
	if (m_KeyboardTracker.IsKeyPressed(Keyboard::Keys::Space))
	{
		//　ジャンプ開始
		StartJump();
	}
	//　ジャンプ中なら
	if (m_isJump)
	{
		//　下方向に加速
		m_Velocity.y -= GRAVITY_ACC;
		if (m_Velocity.y <= -JUMP_SPEED_MAX)
		{
			m_Velocity.y = -JUMP_SPEED_MAX;
		}
	}
	Vector3 trans = this->GetTranslation();
	trans += m_Velocity;
	this->SetTranslation(trans);

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
		Vector3 moveV(0, 0, -0.1f);
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

	if (m_KeyboardTracker.IsKeyPressed(Keyboard::Keys::F))
	{
		if (m_FireFlag)
		{
			ReloadBullet();
		}
		else
		{
			FireBullet();
		}
	}

	if (m_FireFlag)
	{
		Vector3 trans = m_ObjPlayer[PLAYER_ENGINE].GetTranslation();

		trans += m_BulletVel;

		m_ObjPlayer[PLAYER_ENGINE].SetTranslation(trans);

		if (--m_FireCount < 0)
		{
			ReloadBullet();
		}
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
	//　あたり判定の更新
	m_CollisionNodeBullet.Update();
	m_CollisionNodeBody.Update();
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
	m_CollisionNodeBullet.Draw();
	m_CollisionNodeBody.Draw();
}


//

//
void Player::FireBullet()
{
	//　既に発射中
	if (m_FireFlag)return;

	m_FireFlag = true;

	Matrix worldm = m_ObjPlayer[PLAYER_ENGINE].GetWorld();

	Vector3 scale;
	Quaternion rotq;
	Vector3 pos;

	Vector3* m0 = (Vector3*)&worldm.m[0];
	Vector3* m1 = (Vector3*)&worldm.m[1];
	Vector3* m2 = (Vector3*)&worldm.m[2];
	Vector3* m3 = (Vector3*)&worldm.m[3];

	pos = *m3;

	scale = Vector3(m0->Length(), m1->Length(), m2->Length());

	m0->Normalize();
	m1->Normalize();
	m2->Normalize();

	rotq = Quaternion::CreateFromRotationMatrix(worldm);

	m_ObjPlayer[PLAYER_ENGINE].SetObjectParent(nullptr);
	m_ObjPlayer[PLAYER_ENGINE].SetScale(scale);
	m_ObjPlayer[PLAYER_ENGINE].SetRotationQ(rotq);
	m_ObjPlayer[PLAYER_ENGINE].SetTranslation(pos);

	m_BulletVel = Vector3(0, 0.0f, -0.3f);

	m_BulletVel = Vector3::Transform(m_BulletVel, rotq);

	m_FireCount = 120;
}

//
//
//
void Player::ReloadBullet()
{
	if (!m_FireFlag)return;

	m_ObjPlayer[PLAYER_ENGINE].SetObjectParent(&m_ObjPlayer[PLAYER_BODY]);

	m_ObjPlayer[PLAYER_ENGINE].SetScale(Vector3(1, 1, 1));
	m_ObjPlayer[PLAYER_ENGINE].SetRotation(Vector3(0, 0, 0));
	m_ObjPlayer[PLAYER_ENGINE].SetTranslation(Vector3(0, 0.65, 0.5));

	m_FireFlag = false;
}


// ジャンプを開始する
void Player::StartJump()
{
	// ジャンプ中でないか
	if (!m_isJump)
	{
		// 上方向の初速を設定
		m_Velocity.y = JUMP_SPEED_FIRST;
		// ジャンプフラグを立てる
		m_isJump = true;
	}
}

// ジャンプを開始する
void Player::StartFall()
{
	// ジャンプ中でないか
	if (!m_isJump)
	{
		// 上方向の初速を設定
		m_Velocity.y = 0.0f;
		// ジャンプフラグを立てる
		m_isJump = true;
	}
}

//　ジャンプ終了
void Player::StopJump()
{
	m_isJump = false;
	m_Velocity = Vector3::Zero;
}

