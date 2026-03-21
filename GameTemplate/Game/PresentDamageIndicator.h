#pragma once
/**
* @file  PresentDamageIndicator.h
* @brief ボスに与えるダメージ数を即座に描画/削除を行うクラス。
* @author Yamaguchi Hayato
* @date  2026/03/19
*/

/* @TODO: 継承関係はUI担当から指示を受ける
* 今回はとりあえずIGameでクラスを作成し、ダメージ数の描画/削除システムを実装する。
*/



namespace nsApp
{
	class PresentDamageIndicator : public IGameObject
	{
	public:
		/* コンストラクタとデストラクタ。*/
		PresentDamageIndicator() = default;
		virtual ~PresentDamageIndicator() = default;


	public:
		/**
		* ダメージ量と描画位置を受け取り初期化する。
		* @param damageValue ダメージ量。
		* @param drawPosition 描画位置。
		*/
		void Init(int damageValue, const Vector3& drawPosition);

		/* 
		 * 更新処理。
		 */
		void Update();

		/* 
		 * 描画処理。
		 */ 
		void Render(RenderContext& rc);


	private:
		/*
		 * パラメータを設定。
		 * @param alphaValue 描画の透明度。
		 * @param scaleValue テキストの拡大率。
		 * @param upSpeed テキストを上昇させる速度。
		 */
		inline void SetTextParameter(float alphaValue, float scaleValue, float upSpeed)
		{
			m_alphaValue = alphaValue;
			m_scaleValue = scaleValue;
			m_moveUpSeed = upSpeed;
		}


	private:	
		int m_damageValue = 0;                     //! ダメージ量。

		float m_alphaValue = 0.0;                  //! 描画の透明度。
		float m_moveUpSeed = 0.0;                  //! 上昇のシード値。
		float m_scaleValue;                        //! 描画の拡大率。

		Vector3 m_drawPosition = Vector3::Zero;    //! 描画位置。

		FontRender m_damageFont;                   //! ダメージ量を描画するためのフォント。

		std::wstring m_damageText;                 //! 描画するダメージ量のテキスト。
	};
}
