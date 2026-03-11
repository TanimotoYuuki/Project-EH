#pragma once
/**
 * @file SceneLoaer.h。
 * @brief シーンを生成・破棄するクラス。
 * @author Tanimoto。
 * @date 2026/03/05。
 */
namespace nsApp
{
	/**
	*  @brief シーンベースクラス。
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
			enSceneID_InGame,/*インゲームシーン。*/
			enSceneID_Result,/*リザルトシーン。*/
			enSceneID_None,/*シーンなし。*/
			enSceneID_Num,/*シーンの最大数。*/
		};

	public:/*メンバ変数。*/
		EnSceneID m_sceneID = enSceneID_Num;/*シーンID。*/
	};

	namespace nsTitle
	{
		/**
		*  @brief タイトルシーンクラス。
		*/
		class Title;
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
		};
	}

	namespace nsSelect
	{
		/**
		*  @brief 選択シーンクラス。
		*/
		class SelectScene : public IScene
		{
		public:
			SelectScene() {};/*コンストラクタ。*/
			~SelectScene();/*デストラクタ。*/

		public:
			bool Start() override;/*開始処理。*/
			void Update() override;/*更新処理。*/

		private://メンバ変数。
			/*TODO:現在はまだ未実装なので後で追加する。*/
		};
	}

	namespace nsGame
	{
		/**
		*  @brief インゲーム選択シーンクラス。
		*/
		class Game;
		class InGameScene : public IScene
		{
		public:
			InGameScene() {};/*コンストラクタ。*/
			~InGameScene();/*デストラクタ。*/

		public:
			bool Start() override;/*開始処理。*/
			void Update() override;/*更新処理。*/

		private:/*メンバ変数。*/
			Game* m_game = nullptr;/*インゲーム用のインスタンス。*/
		};
	}

	namespace nsResult
	{
		/**
		* @brief リザルト選択シーンクラス。
		*/
		class ResultScene : public IScene
		{
		public:
			ResultScene() {};/*コンストラクタ。*/
			~ResultScene();/*デストラクタ。*/

			bool Start() override;/*開始処理。*/
			void Update() override;/*更新処理。*/

		private:/*メンバ変数。*/
			/*TODO:現在はまだ未実装なので後で追加する。*/
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
			static SceneLoader* m_instance;/*シングルトンインスタンス。*/
		};
	}
}

