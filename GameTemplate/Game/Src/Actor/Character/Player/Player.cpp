#include "stdafx.h"
#include "Player.h"
#include "Src/Actor/Character/Player/PlayerIdleState.h"

namespace nsApp
{
	namespace nsActor
	{
		bool Player::Start()
		{
			/* アニメーションとモデルを準備する。*/
			/* アニメーションクラスの初期化処理をコール。*/
			m_playerAnimation.Initialize();
			/* 今の武器をセットする。*/
			m_playerAnimation.LoadAnimation(m_currentWeapon);

			/* アニメーションとモデルをセットする。*/
			m_model.LoadCharacterModel
			(
				CharacterModelType::Player_3P,
				m_playerAnimation.GetAnimatiocClip(),
				m_playerAnimation.GetAnimationClips()
			);

			/* 武器の種類をセットする。*/
//			m_model.LoadWeaponModel(CharacterModelType::Weapon_GreatSword);

			/* キャラスケールをセットする。*/
			m_model.SetScale(Vector3::One * 0.5f);
			m_model.SetPosition(Vector3(0.0f,50.0f, 0.0f));

			Quaternion rot = Quaternion::Identity;
			rot.AddRotationDegY(-90.0f);
			m_model.SettRotation(rot);

			/* 初期ステートをセットする。*/
			/* 待機状態ステート。*/
			m_stateMachine->ChangeState(new nsState::PlayerIdleState);

			return true;
		}


		void Player::Update()
		{
			/* ICharacterクラスの更新処理をコール。*/
			ICharacter::Update();
			m_model.Update();
		}


		void Player::Render(RenderContext& rc)
		{
			/* 描画。*/
			ICharacter::Render(rc);
		}


		void Player::PlayBasicAnimation(CharacterBasicAnimationList state)
		{
			int animIndex = m_playerAnimation.GetBasicAnimationIndex(state);
			m_model.PlayAnimation(animIndex, 0.2f);
		}
	}
}