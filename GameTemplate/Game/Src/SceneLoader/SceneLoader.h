#pragma once
/**
 * @file SceneLoaer.h。
 * @brief シーンを生成・破棄するクラス。
 * @author Tanimoto。
 * @date 2026/03/05。
 */

#include "Src/AsyncLoad/AsyncLoadManager.h"
#include "Src/Scene/Loading/LoadingSceneController.h"

namespace nsApp
{
	/**
	 * @brief シーンベースクラス。
	 */
	class IScene : public IGameObject
	{
	public:
		IScene() {};/*コンストラクタ。*/
		virtual~IScene() {};/*仮想純粋デストラクタ。*/

	public:/*列挙型。*/

		/*シーンID。*/
		enum EnSceneID : uint8_t
		{
			enSceneID_Title,/*タイトルシーン。*/
			enSceneID_Select,/*選択シーン。*/
			enSceneID_Loading, //! ローディングシーン。
			enSceneID_InGame,/*インゲームシーン。*/
			enSceneID_Result,/*リザルトシーン。*/
			enSceneID_None,/*シーンなし。*/
			enSceneID_Num,/*シーンの最大数。*/
		};

	public:/*メンバ関数。*/

		/**
		* @brief ボスの種類の設定。
		*/
		void SetBossType(int boss)
		{
			m_bossType = boss;
		}

		/**
		* @brief ボスの種類の取得。
		* @return ボスの種類。
		*/
		int GetBossType() const
		{
			return m_bossType;
		}

		/**
		* @brief 各キャラクターの役割の設定。
		* @param playerIndex プレイヤーのインデックス。
		*/
		inline void SetCharacterRole(int playerIndex, int role)
		{
			m_characterRole[playerIndex] = role;
		}

		/**
		* @brief 各キャラクターの役割の取得。
		* @param playerIndex プレイヤーのインデックス。
		* @return キャラクターの役割。
		*/
		inline int GetCharacterRole(int playerIndex) const
		{
			return m_characterRole[playerIndex];
		}

		/**
		* @brief プレイヤーがキャラクターを操作するかの設定。
		* @param playerIndex プレイヤーのインデックス。
		* @param isControl プレイヤーがキャラクターを操作するか？
		*/
		inline void SetPlayerControle(int playerIndex, bool isControl)
		{
			m_isPlayerControle[playerIndex] = isControl;
		}

		/**
		* @brief プレイヤーがキャラクターを操作するかの取得。
		* @param playerIndex プレイヤーのインデックス。
		*/
		inline bool GetPlayerControle(int playerIndex) const
		{
			return m_isPlayerControle[playerIndex];
		}

		/**
		* @brief 音量の割合の設定。
		* @param index 音量のインデックス。
		* @param rate 音量の割合。
		*/
		inline void SetVolumeRate(int index, int rate)
		{
			m_volumeRate[index] = rate;
		}

		/**
		* @brief 音量の割合の取得。
		*/
		inline int GetVolumeRate(int index) const
		{
			return m_volumeRate[index];
		}

	public:/*メンバ変数。*/
		EnSceneID m_sceneID = enSceneID_Num;/*シーンID。*/
		int m_bossType = 0;/*ボスの種類。*/
		int m_characterRole[4] = { 0 };/*キャラクターごとの役割。*/
		bool m_isPlayerControle[4] = { true };/*プレイヤーがキャラクターを操作するか？*/
		int m_volumeRate[3] = { 100,100,100 };/*各音量の割合。*/
	};

	namespace nsOption
	{
		class Option;
	}

	namespace nsTitle
	{
		/**
		*  @brief タイトルシーンクラス。
		*/
		class Title;
		class TitleSelect;
		class TitleBackGround;
		class TitleScene : public IScene
		{
		public:
			TitleScene() {};/*コンストラクタ。*/
			~TitleScene();/*デストラクタ。*/

		public:
			bool Start() override;/*開始処理。*/
			void Update() override;/*更新処理。*/

		private:/*メンバ変数。*/
			Title* m_title = nullptr;/*タイトルシーン用のインスタンス。*/
			TitleSelect* m_titleSelect = nullptr;/*タイトル選択用のインスタンス。*/
			TitleBackGround* m_titleBackGround = nullptr;/*タイトル背景用のインスタンス。*/
			nsApp::nsOption::Option* m_option = nullptr;/*設定用のインスタンス。*/
		};
	}


	namespace nsHowToPlay
	{
		class HowToPlay;
	}

	class GameEndSelect;
	namespace nsSelect
	{
		/**
		*  @brief 選択シーンクラス。
		*/
		class Select;
		class QuestSelect;
		class MemberSelect;
		class RoleSelect;
		class ConfirmationSelect;
		class SelectScene : public IScene
		{
		public:
			SelectScene() {};/*コンストラクタ。*/
			~SelectScene();/*デストラクタ。*/

		public:
			bool Start() override;/*開始処理。*/
			void Update() override;/*更新処理。*/

		private://メンバ変数。
			Select* m_select = nullptr;/*選択シーン用のインスタンス。*/
			QuestSelect* m_questSelect = nullptr;/*クエスト選択用のインスタンス。*/
			MemberSelect* m_memberSelect = nullptr;/*メンバー選択用のインスタンス。*/
			RoleSelect* m_roleSelect = nullptr;/*役割選択用のインスタンス。*/
			ConfirmationSelect* m_confirmationSelect = nullptr;/*確認選択用のインスタンス。*/
			nsApp::nsOption::Option* m_option = nullptr;/*設定用のインスタンス。*/
			nsApp::nsHowToPlay::HowToPlay* m_howToPlay = nullptr;/*操作方法用のインスタンス。*/
		};
	}

	namespace nsSelect
	{
		class ConfirmationSelect;
	}

	namespace nsGame
	{
		/**
		*  @brief インゲーム選択シーンクラス。
		*/
		class Game;
		class Game2;
		class GameClearDirection;
		class Pause;
		class InGameScene : public IScene
		{
		public:
			InGameScene() {};/*コンストラクタ。*/
			~InGameScene();/*デストラクタ。*/

		public:
			bool Start() override;/*開始処理。*/
			void Update() override;/*更新処理。*/

		private:/*メンバ変数。*/
//			Game* m_game = nullptr;/*インゲーム用のインスタンス。*/
			Game2* m_game2 = nullptr;/*インゲーム用のインスタンス。*/
			GameClearDirection* m_gameClearDirection = nullptr;/*ゲームクリア演出用のインスタンス。*/
			GameEndSelect* m_gameEndSelect = nullptr;/*ゲーム終了選択用のインスタンス。*/
			Pause* m_pause = nullptr;/*ポーズ用のインスタンス。*/
			nsApp::nsSelect::ConfirmationSelect* m_confirmationSelect = nullptr;/*確認選択用のインスタンス。*/
			nsApp::nsHowToPlay::HowToPlay* m_howToPlay = nullptr;/*操作方法用のインスタンス。*/
		};
	}

	namespace nsResult
	{
		/**
		* @brief リザルト選択シーンクラス。
		*/
		class Result;
		class ResultScene : public IScene
		{
		public:
			ResultScene() {};/*コンストラクタ。*/
			~ResultScene();/*デストラクタ。*/

			bool Start() override;/*開始処理。*/
			void Update() override;/*更新処理。*/

		private:/*メンバ変数。*/
			Result* m_result = nullptr;/*リザルト用のインスタンス。*/
			GameEndSelect* m_gameEndSelect = nullptr;/*ゲーム終了選択用のインスタンス。*/
		};
	}

	
	namespace nsLoading
	{
		/**
		 * @brief   ローディングシーンクラス。
		 * @author  Yamagucghi Hayato。
		 * @date    2026/06/03: クラス作成日。
		 * @details シーンの切り替えの際に、次のシーンのリソースを読み込むためのシーン。
		 */
		class LoadingScene : public IScene
		{
		public:
			/* コンストラクタとデストラクタ。*/
			LoadingScene() = default;
			virtual ~LoadingScene() = default;


		public:
			/**
			 * @brief 開始処理。
			 * @return 処理が成功したかどうか。
			 */
			bool Start() override;

			/**
			 * @brief 更新処理。
			 */
			void Update() override;

			/**
			 * @brief 描画処理。
			 */
			void Render(RenderContext& rc);


		private:
			LoadingSceneController m_loadingSceneController;  //! ローディングシーンのコントローラー。
			InGameBuildRequest m_request;					  //! インゲーム生成に必要なリクエスト情報。

			std::string m_errorMessage;						  //! ローディングに失敗したときのエラーメッセージ。

			bool m_isChangesScene = false;                    //! シーン切り替えが完了したかどうか。
		};
	}


	namespace nsScene
	{
		/**
		* @brief シーンを生成・破棄するクラス。
		*/
		class SceneLoader
		{
		private:
			SceneLoader() {};/*コンストラクタ。*/

		public:
			~SceneLoader() {};/*デストラクタ。*/

		public:
			bool Start();/*開始処理。*/
			void Update();/*更新処理。*/

		public:/*メンバ関数。*/

			/**
			* @brief シーンの切り替え。
			* @param enSceneID 切り替えるシーンID。
			*/
			inline void ChangeScene(IScene::EnSceneID enSceneID)
			{
				m_changeSceneID = enSceneID;
			}

			/**
			* @brief 現在のシーンIDの取得。
			* @return 現在のシーンID。
			*/
			inline IScene::EnSceneID GetCurrentSceneID() const
			{
				return m_currentSceneID;
			}

		public:/*シングルトン用の関数。*/

			/**
			* @brief シングルトンインスタンスの生成。
			*/
			inline static void CreateInstance()
			{
				if (m_instance != nullptr) return;
				m_instance = new SceneLoader();
			}

			/**
			* @brief シングルトンインスタンスの破棄。
			*/
			inline static void DestroyInstance()
			{
				if (m_instance == nullptr) return;
				delete m_instance;
				m_instance = nullptr;
			}

			/**
			* @brief シングルトンインスタンスの取得。
			* @return シングルトンインスタンス。
			*/
			inline static SceneLoader* GetInstance()
			{
				if (m_instance == nullptr) return nullptr;
				return m_instance;
			}

		private:/*メンバ変数。*/
			IScene* m_currentScene = nullptr;/*現在のシーンを表す用のインスタンス。*/
			IScene::EnSceneID m_changeSceneID = IScene::enSceneID_None;/*切り替えるシーンID。*/
			IScene::EnSceneID m_previousSceneID = IScene::enSceneID_None;/*前のシーンID。*/
			IScene::EnSceneID m_currentSceneID = IScene::enSceneID_None;/*現在のシーンID。*/
			int m_bossType = 0;/*ボスの種類。*/
			int m_characterRole[4] = { 0 };/*キャラクターごとの役割。*/
			bool m_isPlayerControle[4] = { true };/*プレイヤーがキャラクターを操作するか？*/
			int m_volumeRate[3] = { 100,100,100 };/*各音量の割合。*/
			static SceneLoader* m_instance;/*シングルトンインスタンス。*/
		};
	}
}

