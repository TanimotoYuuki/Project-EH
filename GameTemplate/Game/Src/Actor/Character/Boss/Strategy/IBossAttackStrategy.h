#pragma once
/**
 * @file   IBossAttackStrategy.h
 * @brief  ボス攻撃ストラテジーのインターフェース。
 */

namespace nsApp
{
	namespace nsActor
	{
		class Boss;
	}

	namespace nsAI
	{
		/**
		 * @class IBossAttackStrategy
		 * @brief ボス攻撃（噛み・尻尾・火炎など）の共通インターフェース。
		 */
		class IBossAttackStrategy
		{
		public:
			/* コンストラクタ。*/
			virtual ~IBossAttackStrategy() = default;

			/**
			 * @brief 初期化処理。
			 * @param boss　攻撃主体のボス。攻撃開始前に Boss::Start() から呼ぶ。
			 */
			virtual void Enter(nsActor::Boss* boss) = 0;

			/**
			 * @brief 攻撃更新処理。攻撃中は Boss::Update() から毎フレーム呼ぶ。
			 * @param boss　攻撃主体のボス。攻撃中は Boss::Update() から毎フレーム呼ぶ。
			 */
			virtual void Update(nsActor::Boss* boss) = 0;

			/**
			 * @brief 終了処理。
			 * @param boss　攻撃主体のボス。攻撃終了後に Boss::Update() から呼ぶ。
			 */
			virtual void Exit(nsActor::Boss* boss) = 0;

			/**
			 * @brief 攻撃終了処理。
			 * @return 攻撃が終了しているか。Boss::Update() から呼ぶ。攻撃終了後は Boss::Update() から呼ぶべきではない。
			 */
			virtual bool IsEnd() = 0;
		};
	}
}
