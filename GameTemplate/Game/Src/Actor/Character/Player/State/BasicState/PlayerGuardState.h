#pragma once
#include "Src/Actor/Character/Common/IState.h"
#include "Src/Actor/Character/Player/Player.h"
#include "Src/Effect/EffectList.h"

namespace nsK2EngineLow
{
	class EffectEmitter;
}

namespace nsApp
{
	namespace nsState
	{
		/**
		 * @class PlayerGuardState
		 * @brief プレイヤーのガード状態を管理する。
		 */
		class PlayerGuardState : public IState<nsActor::Actor>
		{
		public:
			/* コンストラクタとデストラクタ。*/
			PlayerGuardState() = default;
			virtual ~PlayerGuardState() = default;


		public:
			/**
			 * @brief ガード状態に入るときの処理。
			 */
			void Enter() override;

			/**
			 * @brief ガード状態の毎フレーム更新処理。ガード時間の減少、エフェクトの位置更新、ガード解除条件の判定などを行う。
			 */
			void Update() override;

			/**
			 * @brief ガード状態から抜けるときの処理。ガードエフェクトの停止、オーナーのリセットなどを行う。
			 */
			void Exit() override;

			/**
			 * @brief この状態が要求するステートIDを返す。ガード状態は特定のIDを要求しないため、常に false を返す。
			 * @param id 要求するステートIDを格納する変数への参照。ガード状態は特定のIDを要求しないため、この変数は変更されない。
			 * @return この状態がステートIDを要求する場合は true を返し、要求しない場合は false を返す。ガード状態は特定のIDを要求しないため、常に false を返す。
			 */
			bool RequestID(uint8_t& id) override { return false; }


		private:
			/**
			 * @brief プレイヤーのモデルタイプに応じたガードエフェクトのIDを解決する関数。プレイヤーが1Pまたは2Pの場合は青いガードエフェクトのIDを、3Pまたは4Pの場合は赤いガードエフェクトのIDを返す。プレイヤーがnullptrの場合はデフォルトで青いガードエフェクトのIDを返す。
			 * @return ガードエフェクトのID。
			 */
			nsEffect::Effect_ID ResolveGuardEffectId() const;

			/**
			 * @brief ガードエフェクトの位置・回転・スケールを更新する関数。
			 *        プレイヤーの論理座標と表示座標（Y/Z オフセット）を分け、横視点カメラでも見える位置に配置する。
			 *        回転は Quaternion::Identity、スケールは一様正値とする。
			 *        ガード状態が続く限り毎フレーム呼び出し、プレイヤー移動に追従させる。
			 */
			void UpdateGuardEffectTransform();

			/**
			 * @brief ガードエフェクトを停止する関数。ガード状態から抜けるときに呼び出され、再生中のガードエフェクトがある場合はそれを停止し、リモコンを破棄する。これにより、ガード状態が終了したときにエフェクトが残らないようになる。
			 */
			void StopGuardEffect();


		private:
			nsActor::Player* m_player = nullptr;                      //! Player へのポインタ。
			nsK2EngineLow::EffectEmitter* m_guardEffect = nullptr;    //! ガードエフェクト（Guard_Blue / Guard_Red）のリモコン。
			Vector3 m_effectPos = Vector3::Zero;                       //! ガードエフェクト表示位置（論理座標 + Y/Z オフセット）。
		};
	}
}