#pragma once
/**
 * @file Fade.h。
 * @brief フェードを管理するクラス。
 * @author Tanimoto。
 * @date 2026/03/11。
 */
namespace nsApp {
	namespace nsFade
	{
		/*フェードを管理するクラス。*/
		class Fade : public IGameObject
		{
		public:
			Fade() {};/*コンストラクタ。*/
			~Fade() {};/*デストラクタ。*/

		public:
			bool Start() override;/*開始関数。*/
			void Update() override;/*更新関数。*/
			void Render(RenderContext& rc) override;/*描画関数。*/

		public:/*列挙型。*/
			
			/*フェードの種類。*/
			enum EnFadeType : uint8_t
			{
				enFadeType_FadeIn,/*フェードイン。*/
				enFadeType_FadeOut,/*フェードアウト。*/
				enFadeType_HalfFadeIn,/*半透明フェードイン。*/
				enFadeType_HalfFadeOut,/*半透明フェードアウト。*/
				enFadeType_End,/*フェード処理終了。*/
				enFadeType_Num,/*フェードの種類の数。*/
			};

		private:/*メンバ関数。*/

			/**
			* @brief スプライトの初期化。
			*/
			void InitSprite();

			/**
			* @brief フェード用の背景の初期化。
			*/
			void InitFadeBackGround();

		public:/*メンバ関数。*/

			/**
			* @brief フェードの種類を変更。
			* @param fadeType フェードの種類。
			*/
			inline void ChangeFadeType(EnFadeType fadeType)
			{ 
				if (m_currentFadeType == m_previousSetFadeType) { return; }
				m_currentFadeType = fadeType; 
			}

			/**
			* @brief フェードイン中？
			* @return trueならフェードイン中。
			*/
			inline bool IsFadeIn() const
			{
				return m_currentFadeType == enFadeType_FadeIn;
			}

			/**
			* @brief フェードアウト中？
			* @return trueならフェードアウト中。
			*/
			inline bool IsFadeOut() const
			{
				return m_currentFadeType == enFadeType_FadeOut;
			}

			/**
			* @brief 半透明フェードイン中？
			* @return trueなら半透明フェードイン中。
			*/
			inline bool IsHalfFadeIn() const
			{
				return m_currentFadeType == enFadeType_HalfFadeIn;
			}

			/**
			* @brief 半透明フェードアウト？
			* @return trueなら半透明フェードアウト。
			*/
			inline bool IsHalfFadeOut() const
			{
				return m_currentFadeType == enFadeType_HalfFadeOut;
			}

			/**
			* @brief フェード処理終了したか？
			* @return trueならフェード処理終了している。
			*/
			inline bool IsEnd() const 
			{
				return m_currentFadeType == enFadeType_End;
			}

		public:/*シングルトン用の関数。*/

			/**
			* @brief シングルトンインスタンスの生成。
			*/
			inline static void CreateInstance()
			{
				if (m_instance != nullptr) return;
				m_instance = NewGO<Fade>(1, "fade");
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
			inline static Fade* GetInstance()
			{
				if (m_instance == nullptr) return nullptr;
				return m_instance;
			}

		private:/*メンバ変数。*/
			SpriteRender m_fadeBackGround;/*フェード用の背景。*/
			EnFadeType m_previousSetFadeType = enFadeType_Num;/*前に設定したフェードの種類。*/
			EnFadeType m_currentFadeType = enFadeType_FadeIn;/*現在のフェードの種類。*/
			float m_currentAlpha = 0.0f;/*現在の透明度。*/
			static Fade* m_instance;/*シングルトンインスタンス。*/

		private:/*スプライトを表示するファイルパス用のメンバ変数。*/
			std::string m_fadeBackGroundFilePath="Assets/sprite/fade/fade.dds";/*フェード用の背景のファイルパス。*/
		};
	}
}

