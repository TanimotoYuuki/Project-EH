#include "stdafx.h"
#include "UIInput.h"
#include "Src/Sound/SoundLister.h"
#include "Src/Scene/TitleScene/Title.h"

namespace nsApp
{
    bool UIInput::Start()
    {
        return true;
    }

    void UIInput::Update()
    {
        /*毎フレームフラグをリセット。*/
        m_isMoveUp = false;
        m_isMoveDown = false;
        m_isDecide = false;
        m_isCancel = false;

        auto *pad = g_pad[m_playerIndex];

        /*ポインタが安全に取得できない場合のみスキップ。*/
        if (pad == nullptr)
        {
            return;
        }

        /*SEを検索。*/
        auto *SoundManager = FindGO<nsSound::SoundLister>("SoundManager");

        /*カーソルの上判定。*/
        if (pad->IsTrigger(enButtonUp))
        {
            m_isMoveUp = true;
        }

        /*カーソルの下判定。*/
        if (pad->IsTrigger(enButtonDown))
        {
            m_isMoveDown = true;
        }

        /*決定判定。*/
        if (pad->IsTrigger(enButtonA))
        {
            m_isDecide = true;
        }

        /*キャンセル判定。*/
        if (pad->IsTrigger(enButtonB))
        {
            m_isCancel = true;
        }
    }
}