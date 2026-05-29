#include "stdafx.h"
#include "CommentaryUI.h"

namespace
{
	const Vector3 FLAME_POSITION( 630.0f, 460.0f, 0.0f);
}

namespace nsApp
{
	namespace nsUI
	{
		void CommentaryUI::Init()
		{
			/* フレーム画像は1枚だけ読み込む。 */
			m_frameSprite.Init("Assets/sprite/inGame/battle/gameAnnounceUI.DDS",700.0f,200.0f);
			m_frameSprite.SetPivot(Vector2(0.5f, 0.5f));

			for (auto& fontRender : m_fontRender)
			{
				fontRender.SetScale(0.8f);
				fontRender.SetColor(1.0f, 1.0f, 1.0f, 1.0f);
				fontRender.SetPivot(0.0f, 0.5f);
				fontRender.SetShadowParam( true, 2.0f,Vector4(0.0f, 0.0f, 0.0f, 1.0f));
			}
		}

		void CommentaryUI::Render(RenderContext& rc, const std::vector<CommentaryMessage>& messages)
		{
			/* フレームは常に1枚だけ描画する。 */
			m_frameSprite.SetPosition(FLAME_POSITION);
			m_frameSprite.SetScale(Vector3::One);
			m_frameSprite.Update();
			m_frameSprite.Draw(rc);

			/* メッセージがない場合、文字だけ描画しない。 */
			if (messages.empty())
				return;

			const size_t drawCount = (std::min)(messages.size(),static_cast<size_t>(MAX_MESSAGE_COUNT));

			/* フレーム内の文字開始位置。必要に応じてここだけ調整。 */
			const float textStartX = FLAME_POSITION.x - 250.0f;
			const float textStartY = FLAME_POSITION.y + 30.0f;
			const float intervalY = 85.0f;

			for (size_t i = 0; i < drawCount; i++)
			{
				const Vector3 textPos(textStartX,textStartY - intervalY * static_cast<float>(i),0.0f);

				m_fontRender[i].SetText(messages[i].text.c_str());
				m_fontRender[i].SetScale(1.0f);
				m_fontRender[i].SetPosition(textPos);
				m_fontRender[i].Draw(rc);
			}
		}
	}
}