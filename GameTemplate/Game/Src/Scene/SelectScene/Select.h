#pragma once
/**
 * @file Select.h。
 * @brief 選択クラス。
 * @author Tanimoto。
 * @date 2026/03/16。
 */
namespace nsApp
{
	namespace nsOption
	{
		class Option;
	}

	namespace nsSelect
	{
		/*選択クラス。*/
		class QuestSelect;
		class MemberSelect;
		class RoleSelect;
		class ConfirmationSelect;
		class Select : public IGameObject
		{
		public:
			Select() {};/*コンストラクタ。*/
			~Select();/*デストラクタ。*/

		public:
			bool Start();/*開始処理。*/
			void Update();/*更新処理。*/
			void Render(RenderContext& rc);/*描画処理。*/

		private:/*メンバ関数。*/

			/**
			* @brief 選択シーンの生成。
			*/
			void CreateSelectScene();

			/**
			* @brief スプライトの初期化。
			*/
			void InitSprite();

			/**
			* @brief 背景の初期化。
			*/
			void InitBackGround();

			/**
			* @brief スプライトの更新処理。
			*/
			void UpdateSprite();

		public:/*メンバ関数。*/

			/**
			* @brief クエスト選択用のインスタンスの取得。
			* @return クエスト選択用のインスタンス。
			*/
			QuestSelect* GetQuestSelectInstance()
			{
				return m_questSelect; 
			}

			/**
			* @brief メンバー選択用のインスタンスの取得。
			* @return メンバー選択用のインスタンス。
			*/
			MemberSelect* GetMemberSelectInstance()
			{
				return m_memberSelect;
			}

			/**
			* @brief 役職選択用のインスタンスの取得。
			* @return 役職選択用のインスタンス。
			*/
			RoleSelect* GetRoleSelectInstance()
			{
				return m_roleSelect;
			}

			/**
			* @brief 確認選択用のインスタンスの取得。
			* @return 確認選択用のインスタンス。
			*/
			ConfirmationSelect* GetConfirmationSelectInstance()
			{
				return m_confirmationSelect;
			}

			/**
			* @brief 設定用のインスタンスの取得。
			* @return 設定用のインスタンス。
			*/
			nsApp::nsOption::Option* GetOptionInstance()
			{
				return m_option;
			}

		private:/*メンバ変数。*/
			SpriteRender m_backGround;/*背景。*/
			QuestSelect* m_questSelect = nullptr;/*クエスト選択用のインスタンス。*/
			MemberSelect* m_memberSelect = nullptr;/*メンバー選択用のインスタンス。*/
			RoleSelect* m_roleSelect = nullptr;/*役職選択用のインスタンス。*/
			ConfirmationSelect* m_confirmationSelect = nullptr;/*確認選択用のインスタンス。*/
			nsApp::nsOption::Option* m_option = nullptr;/*設定用のインスタンス。*/

		private:/*スプライトを表示するファイルパス用のメンバ変数。*/
			std::string m_backGroundFilePath = "Assets/sprite/select/background/background.dds";
		};
	}
}

