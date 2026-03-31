#pragma once
#include "Src/UIAnimation/UIAnimation.h"
/**
 * @file QuestSelect.h。
 * @brief クエスト選択クラス。
 * @author Tanimoto。
 * @date 2026/03/06。
 */
namespace nsApp
{
	namespace nsSelect
	{
		/*クエスト選択クラス。*/
		class QuestSelect : public IGameObject
		{
		public:
			QuestSelect() {};/*コンストラクタ。*/
			~QuestSelect() {};/*デストラクタ。*/

		public:
			bool Start() override;/*開始処理。*/
			void Update() override;/*更新処理。*/
			void Render(RenderContext& rc);/*描画処理。*/

		public:/*列挙型。*/
			
			/*スライドさせる方向。*/
			enum EnSlide : uint8_t
			{
				enSlide_Left,/*左。*/
				enSlide_Right,/*右。*/
				enSlide_Num/*スライドさせる方向数。*/
			};

			/*UIをスライドさせるアニメーションに適用するスプライト。*/
			enum EnSlideUIAnimationSprite : uint8_t
			{
				enSlideUIAnimationSprite_TargetUI,/*ターゲットUI。*/
				enSlideUIAnimationSprite_TargetSelectUI,/*ターゲット選択UI。*/
				enSlideUIAnimationSprite_TargetTextUI,/*ターゲットテキストUI。*/
				enSlideUIAnimationSprite_Num/*適用するスプライト数。*/
			};

		private:/*メンバ関数。*/

			/**
			* @brief スプライトの初期化。
			*/
			void InitSprite();

			/**
			* @brief クエスト選択テキストUIの初期化。
			*/
			void InitQuestSelectTextUI();

			/**
			* @brief ターゲットUIの初期化。
			*/
			void InitTargetUI();

			/**
			* @brief ターゲット選択UIの初期化。
			*/
			void InitTargetSelectUI();

			/**
			* @brief ターゲットテキストUIの初期化。
			*/
			void InitTargetTextUI();

			/**
			* @brief UIアニメーションの初期化。
			*/
			void InitUIAnimation();

			/**
			* @brief UIをスライドさせるアニメーションの初期化。
			*/
			void InitSlideUIAnimation();

			/**
			* @brief UIの透明度を変えるアニメーションの初期化。
			*/
			void InitAlphaUIAnimation();

			/**
			* @brief 演出の更新処理。
			*/
			void UpdateDirection();

			/**
			* @brief スプライトの更新処理。
			*/
			void UpdateSprite();

		public:/*メンバ関数。*/

			/**
			* @brief UIをスライドさせるアニメーションのリセット処理。
			* @param slide スライドさせる方向。
			*/
			void ResetSlideUIAnimation(EnSlide slide);

			/**
			* @brief UIの透明度を変えるアニメーションのリセット処理。
			*/
			inline void ResetAlphaUIAnimation()
			{
				m_alphaUIAnimation->Reset();
			}

			/**
			* @brief UIをスライドさせるアニメーション再生終了したか？
			* @param slide スライドさせる方向。
			* @param slideUIAnimationSprite アニメーションさせるスプライト。
			* @return trueなら再生終了している。
			*/
			bool IsEndSlideUIAnimation(EnSlide slide, EnSlideUIAnimationSprite slideUIAnimationSprite) const
			{
				return m_slideUIAnimation[slide][slideUIAnimationSprite]->IsEnd();
			}

			/**
			* @brief UIの透明度を変えるアニメーション再生終了したか？
			* @return trueなら再生終了している。
			*/
			bool IsEndAlphaUIAnimation() const
			{
				return m_alphaUIAnimation->IsEnd();
			}

			/**
			* @brief 演出できる状態にする。
			*/
			inline void EnableDirection()
			{
				m_isDirection = true;
			}

			/**
			* @brief 演出できない状態にする。
			*/
			inline void DisableDirection()
			{
				m_isDirection = false;
			}

			/**
			* @brief 演出中？
			* @return trueなら演出中。
			*/
			inline bool IsDirection() const
			{
				return m_isDirection;
			}

			/**
			* @brief スライドさせる方向の変更。
			*/
			inline void ChangeSlide(EnSlide slide)
			{
				m_currentSlide = slide;
			}

			/**
			* @brief 選択できる状態にする。
			*/
			inline void EnableSelect()
			{
				m_didSelect = true;
			}

			/**
			* @brief 選択できない状態にする。
			*/
			inline void DisableSelect()
			{
				m_didSelect = false;
			}

			/**
			* @brief 選択できたか？
			* @return trueなら選択できている。
			*/
			inline bool DidSelect() const
			{
				return m_didSelect;
			}

		private:/*メンバ変数。*/
			SpriteRender m_questSelectTextUI;/*クエスト選択テキストUI。*/
			Vector3 m_questSelectTextUIPosition;/*クエスト選択テキストUIの位置。*/
			SpriteRender m_targetUI;/*ターゲットUI。*/
			Vector3 m_targetUIPosition;/*ターゲットUIの位置。*/
			SpriteRender m_targetSelectUI;/*ターゲット選択UI。*/
			Vector3 m_targetSelectUIPosition;/*ターゲット選択UIの位置。*/
			SpriteRender m_targetTextUI;/*ターゲットテキストUI。*/
			Vector3 m_targetTextUIPosition;/*ターゲットテキストUIの位置。*/
			int m_currentSlide = enSlide_Left;/*現在のスライドさせる方向。*/
			std::unique_ptr<nsApp::nsUI::PositionUIAnimation> m_slideUIAnimation[enSlide_Num][enSlideUIAnimationSprite_Num];/*UIをスライドさせるアニメーション。*/
			std::unique_ptr<nsApp::nsUI::AlphaUIAnimation> m_alphaUIAnimation;/*透明度を変えるアニメーション。*/
			bool m_isDirection = false;/*演出中？*/
			bool m_didSelect = false;/*選択できたか？*/

		private:/*スプライトを表示するファイルパス用のメンバ変数。*/
			std::string m_questSelectTextUIFilePath = "Assets/sprite/select/questSelect/text/redBer/questSelect.dds";/*クエスト選択テキストUIのファイルパス。*/
			std::string m_targetUIFilePath = "Assets/sprite/select/questSelect/target/target.dds";/*ターゲットUIのファイルパス。*/
			std::string m_targetSelectUIFilePath = "Assets/sprite/select/questSelect/select/targetSelect.dds";/*ターゲット選択UIのファイルパス。*/
			std::string m_targetTextUIFilePath = "Assets/sprite/select/questSelect/text/whiteBer/target.dds";/*ターゲットテキストUIのファイルパス。*/
		};
	}
}

