#pragma once

namespace nsApp
{
    class UIInput : public IGameObject
    {
    public:
        UIInput() = default;
        ~UIInput() = default;

        bool Start() override;
        void Update() override;

        /*各UIシーンからキー入力を検知するためのゲッター。*/
        bool IsMoveUP() const { return m_isMoveUp; }
        bool IsMoveDown() const { return m_isMoveDown; }
        bool IsDecide() const { return m_isDecide; }
        bool IsCancel() const { return m_isCancel; }

    private:
        /*内部判定用フラグ。*/
        bool m_isMoveUp = false;
        bool m_isMoveDown = false;
        bool m_isDecide = false;
        bool m_isCancel = false;

        /*パッドの数を監視。*/
        int m_playerIndex = 0;
    };
}