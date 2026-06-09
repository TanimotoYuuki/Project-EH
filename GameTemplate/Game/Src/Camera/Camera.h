#pragma once
/**
 * @file   Camera.h
 * @brief  場面ごとのStrategyで切り替えられるカメラ。
 * @author Yamaguchi Hayato
 * @date   2026/6/09: リファクタリング。
 */

#include <memory>
#include "Src/Camera/ICameraStrategy.h"
#include "Src/Camera/BattleCameraStrategy.h"
#include "Src/Camera/LoadingCameraStrategy.h"


namespace nsApp
{
	class Camera : public IGameObject
	{
	public:
		/* コンストラクタとデストラクタ。*/
		Camera() = default;
		virtual ~Camera() = default;


	public:
		/**
		 * @brief 初期化処理。
		 */
		bool Start();

		/**
		 * @brief 更新処理。
		 */
		void Update();

		/**
		 * @brief ローディング用カメラへ切り替える。
		 */
		void ChangeToLoading();

		/**
		 * @brief 戦闘用カメラへ切り替える。
		 */
		void ChangeToBattle();


	private:
		/**
		 * @brief カメラStrategyを変更する。
		 * @param strategy 新しいStrategy。
		 */
		void ChangeStrategy(std::unique_ptr<ICameraStrategy> strategy);


	private:
		std::unique_ptr<ICameraStrategy> m_strategy = nullptr; //! 現在のカメラStrategy。
	};
}