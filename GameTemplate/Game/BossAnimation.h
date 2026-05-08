#pragma once
#include <array>
#include <string>

namespace nsApp
{
	namespace nsActor
	{
		enum class BossAnimationID :uint8_t
		{
			/*移動。*/
			Idle,
			Walk,
			TakeOff,
			Land,

			/*攻撃。*/
			BiteAttack,
			TailAttack,
			FireAttack,

			/*リアクション。*/
			GetHit,
			Death,

			/*演出。*/
			Scream,
			Sleep,

			Max,
		};

		class BossAnimation
		{

		public:
			void Init(const char* type);
			/*アニメーションを再生。*/
			void PlayAnimation(BossAnimationID id, ModelRender& model);

			/*modelに渡す。*/
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