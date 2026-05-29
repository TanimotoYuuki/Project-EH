#pragma once
/**
* @file  　Camera.h
* @brief 　ゲームに範囲を扱うカメラ。
* @author　Yamaguchi Hayato
* @date  　2026/03/14
*/


namespace nsApp
{
	class Camera : public IGameObject
	{
	public:
		/* コンストラクタとデストラクタ。*/
		Camera() = default;
		virtual ~Camera() = default;


	public:
		/* 初期化処理。*/
		bool Start();
	};
}

