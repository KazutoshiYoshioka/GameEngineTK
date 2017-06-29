//
//　モデルエフェクト
//

#pragma once

#include <d3d11_1.h>
#include <SimpleMath.h>
#include <list>
#include "Obj3D.h"

class ModelEffect
{
	public:
		using Vector3 = DirectX::SimpleMath::Vector3;

		//　コンストラクタ
		ModelEffect();

		void Initialize(const wchar_t* filename, int Endframe, Vector3 Startrotation, Vector3 Endrotation, Vector3 StartScale, Vector3 EndScale,Vector3 Pos);

		// 発生させる
		bool Entry();
		
		//　更新処理
		void Update();

		//　描画
		void Draw();

		////　親をセット
		//void SetParent(Obj3d* parent) { m_Obj.SetObjectParent(parent); }
		//// 親からのオフセット
		//void SetTrans(DirectX::SimpleMath::Vector3& trans) { m_Translation = trans; }

	protected:
		// ３Ｄオブジェクト
		Obj3d	m_Obj;

		// 現在フレーム
		int		m_Frame;
		// 終了フレーム
		int		m_EndFrame;

		//　回転
		Vector3	m_StartRotation;

		Vector3 m_EndRotation;

		//　初期拡大率
		Vector3	m_StartScale;

		//　最終拡大率
		Vector3	m_EndScale;

		Vector3 m_Pos;
};