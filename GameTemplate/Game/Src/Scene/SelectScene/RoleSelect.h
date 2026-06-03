#pragma once
#include "Src/UIAnimation/UIAnimation.h"
/**
 * @file RoleSelect.h。
 * @brief 役割選択クラス。
 * @author Tanimoto。
 * @date 2026/04/17。
 */
namespace nsApp
{
	namespace nsSelect
	{
		/*役職選択クラス。*/
		class RoleSelect : public IGameObject
		{
		public:
			RoleSelect() {};/*コンストラクタ。*/
			~RoleSelect() {};/*デストラクタ。*/

		public:
			bool Start() override;/*開始処理。*/
			void Update() override;/*更新処理。*/
			void Render(RenderContext& rc) override;/*描画処理。*/

		public:/*列挙型。*/

			/*役割。*/
			enum EnRole : uint8_t
			{
				enRole_Sword,/*ソード。*/
				enRole_Hammer,/*ハンマー。*/
				enRole_Mage,/*メイジ。*/
				enRole_Gunner,/*ガンナー。*/
				enRole_Num,/*役割の数。*/
			};

			/*矢印UI。*/
			enum EnArrowUI : uint8_t
			{
				enArrowUI_Left,/*左。*/
				enArrowUI_Right,/*右。*/
				enArrowUI_Num,/*矢印UIの数。*/
			};

			/*大きさ。*/
			enum EnScale : uint8_t
			{
				enScale_Down,/*大きくする。*/
				enScale_Up,/*小さくする。*/
				enScale_Num/*大きさを変更する種類。*/
			};

			/*色の変更。*/
			enum EnChangeColor : uint8_t
			{
				enChangeColor_Before,/*変更前の色。*/
				enChangeColor_After,/*変更後の色。*/
				enChangeColor_Num/*色の変更の種類。*/
			};

			/*方向。*/
			enum EnDirection : uint8_t
			{
				enDirection_Left,/*左。*/
				enDirection_Right,/*右。*/
				enDirection_Num,/*方向の数。*/
			};

			/*位置。*/
			enum EnPosition : uint8_t
			{
				enPosition_Down,/*下降。*/
				enPosition_Up,/*上昇。*/
				enPosition_Num/*位置を変更する種類。*/
			};

			/*選択したときの演出UIアニメーションに適用するスプライト。*/
			enum EnSelectDirectionUIAnimationSprite : uint8_t
			{
				enSelectDirectionUIAnimationSprite_WhiteFrameUI,/*白枠UI。*/
				enSelectDirectionUIAnimationSprite_SwordRoleUI,/*ソード役割UI。*/
				enSelectDirectionUIAnimationSprite_HammerRoleUI,/*ハンマー役割UI。*/
				enSelectDirectionUIAnimationSprite_MageRoleUI,/*メイジ役割UI。*/
				enSelectDirectionUIAnimationSprite_GunnerRoleUI,/*ガンナー役割UI。*/
				enSelectDirectionUIAnimationSprite_LeftArrowUI,/*左矢印UI。*/
				enSelectDirectionUIAnimationSprite_RightArrowUI,/*右矢印UI。*/
				enSelectDirectionUIAnimationSprite_Num/*適用するスプライト数。*/
			};

		private:/*メンバ関数。*/

			/**
			* @brief スプライトの初期化。
			*/
			void InitSprite();

			/**
			* @brief 白枠UIの初期化。
			*/
			void InitWhiteFrameUI();

			/**
			* @brief 役職UIの初期化。
			* @param role 役職の種類。
			*/
			void InitRoleUI(EnRole role);

			/**
			* @brief 矢印UIの初期化。
			* @param arrowUI 矢印UIの種類。
			*/
			void InitArrowUI(EnArrowUI arrowUI);

			/**
			* @brief UIアニメーションの初期化。
			*/
			void InitUIAnimation();

			/**
			* @brief 役割UIの大きさを変えるアニメーションの初期化。
			*/
			void InitScaleRoleUIAnimation();

			/**
			* @brief 矢印UIの大きさを変えるアニメーションの初期化。
			*/
			void InitScaleArrowUIAnimation();

			/**
			* @brief 矢印UIの色を変えるアニメーションの初期化。
			*/
			void InitColorArrowUIAnimation();

			/**
			* @brief 選択したときの演出UIアニメーションの初期化。
			*/
			void InitSelectDirectionUIAnimation();

			/**
			* @brief 選択の更新処理。
			*/
			void UpdateSelect();

			/**
			* @brief UIアニメーションの更新処理。
			*/
			void UpdateUIAnimation();

			/**
			* @brief スプライトの更新処理。
			*/
			void UpdateSprite();

		public:/*メンバ関数。*/

			/**
			* @brief 役割UIの大きさを変えるアニメーションのリセット処理。
			*/
			void ResetScaleRoleUIAnimation();

			/**
			* @brief 矢印UIの大きさを変えるアニメーションのリセット処理。
			*/
			void ResetScaleArrowUIAnimation();

			/**
			* @brief 矢印UIの色を変えるアニメーションのリセット処理。
			*/
			void ResetColorArrowUIAnimation();

			/**
			* @brief 選択したときの演出UIアニメーションのリセット処理。
			*/
			void ResetSelectDirectionUIAnimation();

			/**
			* @brief UIを配置する基準位置の設定。
			* @param referencePosition UIを配置する基準位置。
			*/
			inline void SetReferencePosition(const Vector3& referencePosition)
			{
				m_referencePosition = referencePosition;
			}

			/**
			* @brief 現在の役割の取得。
			* @return 現在の役割。
			*/
			inline EnRole GetCurrentRole() const
			{
				return (EnRole)m_currentRole;
			}

			/**
			* @brief 表示する役割UIの変更。
			* @param role 表示する役割UI。
			*/
			inline void ChangeDisplayRoleUI(EnRole role)
			{
				m_displayRoleUI = role;
				m_currentRole = role;
			}

			/**
			* @brief UIを描画する状態にする。
			*/
			inline void EnableDrawingUI() 
			{
				m_isDrawingUI = true; 
			}

			/**
			* @brief UIを描画しない状態にする。
			*/
			inline void DisableDrawingUI()
			{ 
				m_isDrawingUI = false; 
			}

			/**
			* @brief UIを描画しているか？
			* @return trueならUIを描画している。
			*/
			inline bool IsDrawingUI()
			{ 
				return m_isDrawingUI;
			}

			/**
			* @brief スライド演出できる状態にする。
			*/
			inline void EnableSlideDirection()
			{
				m_isSlideDirection = true;
			}

			/**
			* @brief スライド演出できない状態にする。
			*/
			inline void DisableSlideDirection()
			{
				m_isSlideDirection = false;
			}

			/**
			* @brief スライド演出中？
			* @return trueならスライド演出中。
			*/
			inline bool IsSlideDirection() const
			{
				return m_isSlideDirection;
			}

			/**
			* @brief 選択演出できる状態にする。
			*/
			inline void EnableSelectDirection()
			{
				m_isSelectDirection = true;
			}

			/**
			* @brief 選択演出できない状態にする。
			*/
			inline void DisableSelectDirection()
			{
				m_isSelectDirection = false;
			}

			/**
			* @brief 選択演出中？
			* @return trueなら選択演出中。
			*/
			inline bool IsSelectDirection() const
			{
				return m_isSelectDirection;
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
			inline void DiableSelect()
			{
				m_didSelect = false;
			}

			/**
			* @brief 選択できたか？
			* @return trueなら選択できている。
			*/
			inline bool DidSelect()
			{
				return m_didSelect;
			}

			/**
			* @brief 前の選択に戻る状態にする。
			*/
			inline void EnableBackSelect()
			{
				m_isBackSelect = true;
			}

			/**
			* @brief 前の選択に戻らない状態にする。
			*/
			inline void DisableBackSelect()
			{
				m_isBackSelect = false;
			}

			/**
			* @brief 前の選択に戻るか？
			* @return trueなら前の選択に戻る。
			*/
			inline bool IsBackSelect() const
			{
				return m_isBackSelect;
			}

		private:/*メンバ変数。*/
			SpriteRender m_whiteFrameUI;/*白枠UI。*/
			SpriteRender m_roleUI[enRole_Num];/*役割UI。*/
			SpriteRender m_arrowUI[enArrowUI_Num];/*矢印UI。*/
			Vector3 m_referencePosition = Vector3::Zero;/*UIを配置する基準位置。*/
			Vector3 m_selectDirectionUIAnimationBasePosition[enSelectDirectionUIAnimationSprite_Num];/*選択したときの演出UIアニメーションの元の位置。*/
			Vector3 m_selectDirectionUIAnimationTargetPosition[enSelectDirectionUIAnimationSprite_Num];/*選択したときの演出UIアニメーションのターゲットの位置。*/
			int m_previousRole = enRole_Num;/*前の役割。*/
			int m_currentRole = enRole_Sword;/*現在の役割。*/
			int m_displayRoleUI = enRole_Sword;/*表示する役割UI。*/
			int m_inputDirection = enDirection_Num;/*入力された方向。*/
			std::unique_ptr<nsApp::nsUI::ScaleUIAnimation> m_scaleRoleUIAnimation[enScale_Num][enRole_Num];/*役割UIの大きさを変えるアニメーション。*/
			std::unique_ptr<nsApp::nsUI::ScaleUIAnimation> m_scaleArrowUIAnimation[enScale_Num][enArrowUI_Num];/*矢印UIの大きさを変えるアニメーション。*/
			std::unique_ptr<nsApp::nsUI::ColorUIAnimation> m_colorArrowUIAnimation[enChangeColor_Num][enArrowUI_Num];/*矢印UIの色を変えるアニメーション。*/
			std::unique_ptr<nsApp::nsUI::PositionUIAnimation> m_selectDirectionUIAnimation[enPosition_Num][enSelectDirectionUIAnimationSprite_Num];/*選択したときの演出UIアニメーション。*/
			bool m_isDrawingUI = false;/*UIを描画しているか？*/
			bool m_isSlideDirection = false;/*スライド演出中？*/
			bool m_isSelectDirection = false;/*選択演出中？*/
			bool m_didSelect = false;/*選択できたか？*/
			bool m_isBackSelect = false;/*前の選択に戻るか？*/

		private:/*スプライトを表示するファイルパス用のメンバ変数。*/
			std::string m_whiteFrameUIFilePath = "Assets/sprite/select/roleSelect/frame/whiteFrame.dds";/*白枠UIのファイルパス。*/
			std::string m_roleUIFilePath[enRole_Num] = {
				"Assets/sprite/select/roleSelect/role/sword.dds",/*ソード。*/
				"Assets/sprite/select/roleSelect/role/hammer.dds",/*ハンマー。*/
				"Assets/sprite/select/roleSelect/role/mage.dds",/*メイジ。*/
				"Assets/sprite/select/roleSelect/role/gunner.dds",/*ガンナー。*/
			};/*役割UIのファイルパス。*/
			std::string m_arrowUIFilePath[enArrowUI_Num] = {
				"Assets/sprite/select/roleSelect/arrow/left.dds",/*左。*/
				"Assets/sprite/select/roleSelect/arrow/right.dds",/*右。*/
			};/*矢印UIのファイルパス。*/
		};
	}
}

