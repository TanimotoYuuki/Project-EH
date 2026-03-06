#include "stdafx.h"
#include "Title.h"
#include "SceneManager.h"

namespace {
	//背景
	const float BACK_GROUND_WIDTH = 1920;//背景の幅

	const float BACK_GROUND_HEIGHT = 1080;//背景の高さ

	//タイトル名UI
	const float TITLE_NAME_UI_WIDTH = 1024.0f;//タイトル名UIの幅

	const float TITLE_NAME_UI_HEIGHT = 512.0f;//タイトル名UIの高さ

	const Vector3 TITLE_NAME_UI_INIT_POSITION = { 0.0f,150.0f,0.0f };//タイトル名UIの初期位置

	const Vector3 TITLE_NAME_UI_INIT_SCALE = { 1.5f,1.5f,1.0f };//タイトル名UIの初期大きさ

	//Aボタンを押してくださいを促すUI
	const float PRESS_A_BUTTON_UI_WIDTH = 1024.0f;//Aボタンを押してくださいを促すUIの幅

	const float PRESS_A_BUTTON_UI_HEIGHT = 256.0f;//Aボタンを押してくださいを促すUIの高さ

	const Vector3 PRESS_A_BUTTON_UI_INIT_POSITION = { 0.0f,-150.0f,0.0f };//Aボタンを押してくださいを促すUIの初期位置

	const Vector3 PRESS_A_BUTTON_UI_INIT_SCALE = { 1.0f,1.0f,1.0f };//Aボタンを押してくださいを促すUIの初期大きさ
}

//開始処理
bool Title::Start()
{
	//スプライトの初期化
	InitSprite();

	return true;
}

//更新処理
void Title::Update()
{
	if (g_pad[0]->IsTrigger(enButtonA))
	{
		SceneManager::GetInstance()->ChangeScene(SceneManager::EnSceneID::enSceneID_InGame);
	}

	//背景の更新処理
	m_backGround.Update();

	//タイトル名UIの更新処理
	m_titleNameUI.Update();

	//Aボタンを押してくださいを促すUIの更新処理
	m_pressAButtonUI.Update();
}

//描画処理
void Title::Render(RenderContext& rc)
{
	//背景の描画
	m_backGround.Draw(rc);

	//タイトル名を表示するUIの描画
	m_titleNameUI.Draw(rc);

	//Aボタンを押してくださいを促すUIの描画
	m_pressAButtonUI.Draw(rc);
}

//スプライトの初期化
void Title::InitSprite()
{
	//背景
	InitBackGround();

	//タイトル名
	InitTitleNameUI();

	//Aボタンを押してくださいを促すUI
	InitPressAButtonUI();
}

void Title::InitBackGround()
{
	m_backGround.Init(m_backGroundFilePath.c_str(), BACK_GROUND_WIDTH, BACK_GROUND_HEIGHT);
}

//タイトル名UIの初期化
void Title::InitTitleNameUI()
{
	m_titleNameUIPosition = TITLE_NAME_UI_INIT_POSITION;
	m_titleNameUIScale = TITLE_NAME_UI_INIT_SCALE;

	m_titleNameUI.Init(m_titleNameUIFilePath.c_str(), TITLE_NAME_UI_WIDTH, TITLE_NAME_UI_HEIGHT);//初期化
	m_titleNameUI.SetPosition(m_titleNameUIPosition);//位置設定
	m_titleNameUI.SetScale(m_titleNameUIScale);//大きさ設定
	m_titleNameUI.Update();//更新処理
}

//Aボタンを押してくださいを促すUIの初期化
void Title::InitPressAButtonUI()
{
	m_pressAButtonUIPosition = PRESS_A_BUTTON_UI_INIT_POSITION;
	m_pressAButtonUIScale = PRESS_A_BUTTON_UI_INIT_SCALE;

	m_pressAButtonUI.Init(m_pressAButtonUIFilePath.c_str(), PRESS_A_BUTTON_UI_WIDTH, PRESS_A_BUTTON_UI_HEIGHT);//初期化
	m_pressAButtonUI.SetPosition(m_pressAButtonUIPosition);//位置設定
	m_pressAButtonUI.SetScale(m_pressAButtonUIScale);//大きさ設定
	m_pressAButtonUI.Update();//更新処理
}