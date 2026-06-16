#pragma once
/**
 * @file   BossAnimation.h
 * @brief  ボス種別ごとのアニメーションクリップ管理。
 */

#include <array>
#include <string>

namespace nsApp
{
	namespace nsActor
	{
		/**
		 * @enum BossAnimationID
		 * @brief ボスアニメーションの識別子。
		 */
		enum class BossAnimationID : uint8_t
		{
			Idle,       //! 待機。
			Walk,       //! 歩行。
			TakeOff,    //! 離陸。
			Land,       //! 着陸。

			BiteAttack, //! 噛みつき攻撃。
			TailAttack, //! 尻尾攻撃。
			FireAttack, //! 火炎攻撃。

			GetHit,     //! 被弾。
			Death,      //! 死亡。

			Scream,     //! 咆哮。
			Sleep,      //! 睡眠。

			Max,		//! 最大数。
		};

		/**
		 * @class BossAnimation
		 * @brief ボスタイプに応じた .tka アニメーションの読み込みと再生管理。
		 */
		class BossAnimation
		{
		public:
			/** @brief ボスタイプ名を指定して全クリップを読み込む。 */
			void Init(const char* type);

			/** @brief 指定アニメーションをモデルに再生させる。 */
			void PlayAnimation(BossAnimationID id, ModelRender& model);

			/** @brief Model へ渡す AnimationClip 配列を返す。 */
			AnimationClip* GetAnimationClips()
			{
				return m_clips;
			}

		private:
			static AnimationClip m_clips[(int)BossAnimationID::Max];

			std::string m_type;
			std::string GetAnimName(BossAnimationID id);
			std::string GetPath(BossAnimationID id);

			bool IsLoop(BossAnimationID id);
		};
	}
}
