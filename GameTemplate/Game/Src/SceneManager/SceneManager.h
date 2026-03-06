#pragma once

namespace nsApp
{
	/// <summary>
	/// シーンベースクラス。
	/// </summary>
	class IScene : public IGameObject
	{
	public:
		IScene() {};//コンストラクタ。
		virtual~IScene() {};//仮想純粋デストラクタ。

	public://列挙型。

		//シーンID。
		enum EnSceneID
		{
			enSceneID_Title,//タイトルシーン。
			enSceneID_StageSelect,//ステージ選択シーン。
			enSceneID_InGame,//インゲームシーン。
			enSceneID_Result,//リザルトシーン。
			enSceneID_None,//シーンなし。
			enSceneID_Num,//シーンの最大数。
		};

	public://メンバ変数。
		EnSceneID m_sceneID = enSceneID_Num;//シーンID。
		EnSceneID m_changeSceneID = enSceneID_None;//切り替えるシーンID。
		EnSceneID m_previousSceneID = enSceneID_None;//前のシーンID。
		EnSceneID m_currentSceneID = enSceneID_None;//現在のシーンID。
	};

	namespace nsTitle
	{
		/// <summary>
		/// タイトルシーンクラス。
		/// </summary>
		class Title;
		class TitleScene : public IScene
		{
		public:
			TitleScene() {};//コンストラクタ。
			~TitleScene();//デストラクタ。

			bool Start() override;//開始処理。
			void Update() override;//更新処理。

		private://メンバ変数。
			Title* m_title = nullptr;//タイトルシーン用のインスタンス。
		};
	}

	namespace nsStageSelect
	{
		/// <summary>
		/// ステージ選択シーンクラス。
		/// </summary>
		class StageSelectScene : public IScene
		{
		public:
			StageSelectScene() {};//コンストラクタ。
			~StageSelectScene();//デストラクタ。

			bool Start() override;//開始処理。
			void Update() override;//更新処理。

		private://メンバ変数。
			//TODO:現在はまだ未実装なので後で追加する。
		};
	}

	namespace nsGame
	{
		/// <summary>
		/// インゲーム選択シーンクラス。
		/// </summary>
		class Game;
		class InGameScene : public IScene
		{
		public:
			InGameScene() {};//コンストラクタ。
			~InGameScene();//デストラクタ。

			bool Start() override;//開始処理。
			void Update() override;//更新処理。

		private://メンバ変数。
			Game* m_game = nullptr;//インゲーム用のインスタンス。
		};
	}

	namespace nsResult
	{
		/// <summary>
		/// リザルト選択シーンクラス。
		/// </summary>
		class ResultScene : public IScene
		{
		public:
			ResultScene() {};//コンストラクタ。
			~ResultScene();//デストラクタ。

			bool Start() override;//開始処理。
			void Update() override;//更新処理。

		private://メンバ変数。
			//TODO:現在はまだ未実装なので後で追加する。
		};
	}

	namespace nsScene
	{
		/// <summary>
		/// シーン全体を管理するためのマネージャークラス。
		/// </summary>
		class SceneManager : public IScene
		{
		private:
			SceneManager() {};//コンストラクタ。

		public:
			~SceneManager() {};//デストラクタ。
			bool Start();//開始処理。
			void Update();//更新処理。

		public://メンバ関数。

			/// <summary>
			/// シーンの切り替え。
			/// </summary>
			/// <param name="enSceneID">切り替えるシーンID。</param>
			void ChangeScene(EnSceneID enSceneID)
			{
				m_changeSceneID = enSceneID;
			}

			/// <summary>
			/// 現在のシーンIDの取得。
			/// </summary>
			/// <returns>現在のシーンID。</returns>
			EnSceneID GetCurrentSceneID() const
			{
				return m_currentSceneID;
			}

		public://シングルトン用の関数。

			/// <summary>
			/// シングルトンインスタンスの生成。
			/// </summary>
			static void CreateInstance()
			{
				if (m_instance != nullptr) return;
				m_instance = new SceneManager();
			}

			/// <summary>
			/// シングルトンインスタンスの破棄。
			/// </summary>
			static void DestroyInstance()
			{
				if (m_instance == nullptr) return;
				delete m_instance;
				m_instance = nullptr;
			}

			/// <summary>
			/// シングルトンインスタンスの取得。
			/// </summary>
			/// <returns>シングルトンインスタンス。</returns>
			static SceneManager* GetInstance()
			{
				if (m_instance == nullptr) return nullptr;
				return m_instance;
			}

		private://メンバ変数。
			IScene* m_scene = nullptr;//シーン用のインスタンス。
			static SceneManager* m_instance;//シングルトンインスタンス。
		};
	}
}

