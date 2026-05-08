#include "stdafx.h"
#include "Select.h"
#include "QuestSelect.h"
#include "MemberSelect.h"
#include "ConfirmationSelect.h"
#include "Src/Fade/Fade.h"

namespace {
	/*背景。*/
	const float BACK_GROUND_WIDTH = 1920;/*背景の幅。*/

	const float BACK_GORUND_HEIGHT = 1080;/*背景の高さ。*/
}

namespace nsApp
{
	namespace nsSelect 
	{
		/*デストラクタ。*/
		Select::~Select()
		{
			DeleteGO(m_questSelect);
			DeleteGO(m_memberSelect);
			DeleteGO(m_confirmationSelect);
		}

		/*開始処理。*/
		bool Select::Start()
		{
			/*選択シーンの生成。*/
			CreateSelectScene();

			/*スプライトの初期化。*/
			InitSprite();

			/*フェードインに切り替える。*/
			nsApp::nsFade::Fade::GetInstance()->ChangeFadeType(nsApp::nsFade::Fade::EnFadeType::enFadeType_FadeIn);

			return true;
		}

		/*更新処理。*/
		void Select::Update()
		{
			/*スプライト。*/
			UpdateSprite();
		}

		/*描画処理。*/
		void Select::Render(RenderContext& rc)
		{
			/*背景の描画。*/
			m_backGround.Draw(rc);
		}

		/*選択シーンの生成。*/
		void Select::CreateSelectScene()
		{
			m_questSelect = NewGO<QuestSelect>(0, "questSelect");
			m_memberSelect = NewGO<MemberSelect>(0, "memberSelect");
			m_confirmationSelect = NewGO<ConfirmationSelect>(2, "confirmationSelect");
		}

		/*スプライトの初期化。*/
		void Select::InitSprite()
		{
			/*背景。*/
			InitBackGround();
		}

		/*背景の初期化。*/
		void Select::InitBackGround()
		{
			m_backGround.Init(m_backGroundFilePath.c_str(), BACK_GROUND_WIDTH, BACK_GORUND_HEIGHT);/*初期化。*/
		}

		/*スプライトの更新処理。*/
		void Select::UpdateSprite()
		{
			/*背景。*/
			m_backGround.Update();
		}
	}
}