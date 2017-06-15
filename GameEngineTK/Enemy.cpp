//--------------------------------------------------------------------------------------
// ファイル名: Enemy.h
// 作成者:
// 作成日:
// 説明:
//--------------------------------------------------------------------------------------

#include "Enemy.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

/*==================================
目的の角度への最短角度を取得（ラジアン）
引数	_angle0	ベースとなる角度
_angle1	目標とする角度
戻り値	差分角度
角度０から角度１に最短コースで向かう際に加算する角度を取得する
===================================*/
static float GetShortAngleRad(float _angle0, float _angle1)
{
	float angle_sub;

	angle_sub = _angle1 - _angle0;	// 角度の差
									// 差が１８０度(π）より大きかった場合、逆回転の方が近いので、マイナスに変換
									// 最終的に-180～+180度の範囲に。
	if (angle_sub > XM_PI)
	{
		angle_sub -= XM_2PI;
	}
	else if (angle_sub < -XM_PI)
	{
		angle_sub += XM_2PI;
	}

	return angle_sub;
}

//-----------------------------------------------------------------------------
// コンストラクタ
//-----------------------------------------------------------------------------
Enemy::Enemy()
	: m_cycle(0.0f)
	, m_Death(false)
{
	m_Timer = 60;
}

//-----------------------------------------------------------------------------
// デストラクタ
//-----------------------------------------------------------------------------
Enemy::~Enemy()
{
}

//-----------------------------------------------------------------------------
// 初期化
//-----------------------------------------------------------------------------
void Enemy::Initialize()
{
	m_Obj.resize(PARTS_NUM);
	m_Obj[PARTS_BODY].LoadModel(L"Resources/Tank.cmo");
	m_Obj[PARTS_BATTERY].LoadModel(L"Resources/Battery.cmo");
	m_Obj[PARTS_CANNON].LoadModel(L"Resources/Burst.cmo");
	m_Obj[PARTS_ENGINE].LoadModel(L"Resources/Engine.cmo");

	// パーツの親子関係をセット
	m_Obj[PARTS_BATTERY].SetObjectParent(
		&m_Obj[PARTS_BODY]);

	m_Obj[PARTS_CANNON].SetObjectParent(
		&m_Obj[PARTS_BODY]);

	m_Obj[PARTS_ENGINE].SetObjectParent(
		&m_Obj[PARTS_BODY]);


	// 親からのオフセット（座標のずらし分）をセット
	//　親からずらす
	m_Obj[PARTS_BODY].SetTranslation(Vector3(0, 0.0, 0));
	m_Obj[PARTS_BATTERY].SetTranslation(Vector3(0, 0.5, 0));
	m_Obj[PARTS_CANNON].SetTranslation(Vector3(0, 0.65, 0));
	m_Obj[PARTS_ENGINE].SetTranslation(Vector3(0, 0.65, 0.5));
	


	//
	m_Obj[PARTS_BATTERY].SetScale(Vector3(0.6, 0.6, 0.6));
	m_Obj[PARTS_CANNON].SetScale(Vector3(3, 1, 1));

	//
	m_Obj[PARTS_CANNON].SetRotation(Vector3(0, XMConvertToRadians(-90), 0));

	// 初期配置ランダム
	Vector3 pos;
	pos.x = (float)rand() / RAND_MAX * 20.0f - 10.0f;
	pos.y = 0.0f;
	pos.z = (float)rand() / RAND_MAX * 20.0f - 10.0f;
	m_Obj[0].SetTranslation(pos);

	m_angle = (float)(rand() % 360);

	m_Obj[0].SetRotation(Vector3(0, XMConvertToRadians(m_angle), 0));

	// 当たり判定
	/*m_CollisionNodeBody.Initialize();
	m_CollisionNodeBody.SetParent(&m_Obj[0]);
	m_CollisionNodeBody.SetTrans(Vector3(0, 0.3f, 0));
	m_CollisionNodeBody.SetLocalRadius(1.0f);*/

	//m_ObjShadow.LoadModel(L"Resources/shadow.cmo");
	//m_ObjShadow.SetTranslation(Vector3(0, -0.4f, 0));
	//m_ObjShadow.SetObjectParent(&m_Obj[0]);
}

//-----------------------------------------------------------------------------
// 更新
//-----------------------------------------------------------------------------
void Enemy::Update()
{
	// 死んでいたら何もしない
	if (m_Death) return;

	// 定期的に進行方向を変える
	m_Timer--;
	if (m_Timer < 0)
	{
		m_Timer = 60;

		// 角度を変更
		float rnd = (float)rand() / RAND_MAX - 0.5f;
		rnd *= 180.0f;
		m_angle += rnd;
	}

	// じわじわと角度を反映
	{
		Vector3 rotv = m_Obj[0].GetRotation();
		float angle = GetShortAngleRad(rotv.y, XMConvertToRadians(m_angle));
		rotv.y += angle*0.01f;
		m_Obj[0].SetRotation(rotv);
	}

	// 機体の向いている方向に進む
	{
		// 今の座標を取得
		Vector3 trans = m_Obj[0].GetTranslation();

		Vector3 move(0, 0, -0.05f);
		Vector3 rotv = m_Obj[0].GetRotation();
		Matrix rotm = Matrix::CreateRotationY(rotv.y);
		move = Vector3::TransformNormal(move, rotm);
		// 座標を移動
		trans += move;
		// 移動後の座標をセット
		m_Obj[0].SetTranslation(trans);
	}

	// 移動を反映して行列更新
	Calc();

//	m_CollisionNodeBody.Update();
}

//-----------------------------------------------------------------------------
// 行列更新
//-----------------------------------------------------------------------------
void Enemy::Calc()
{
	// 死んでいたら何もしない
	if (m_Death) return;

	// 全パーツ分行列更新
	for (int i = 0; i < PARTS_NUM; i++)
	{
		m_Obj[i].Update();
	}

	//m_ObjShadow.Update();
}

//-----------------------------------------------------------------------------
// 描画
//-----------------------------------------------------------------------------
void Enemy::Draw()
{
	// 死んでいたら何もしない
	if (m_Death) return;

	// 全パーツ分描画
	for (int i = 0; i < PARTS_NUM; i++)
	{
		m_Obj[i].Draw();
	}

	//m_CollisionNodeBody.Draw();

	//// 影を減算描画
	//m_ObjShadow.DrawSubtractive();
}