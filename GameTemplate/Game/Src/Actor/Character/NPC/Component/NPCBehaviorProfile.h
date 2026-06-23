#pragma once

namespace nsApp
{
	/**
	 * @struct NPCBehaviorProfile
	 * @brief 武器別の NPC 行動個性。Brain が参照し、State は直接触らない。
	 */
	struct NPCBehaviorProfile
	{
		float preferredAttackRange = 170.0f;	 //! 攻撃を開始するまでの距離。
		float evadeThreshold = 0.0f;			 //! 回避行動を開始するまでの距離。
		float evadeExitThreshold = 0.0f;		 //! 回避行動を終了するまでの距離。
		int   evadeReactionDelayMin = 0;	     //! 回避行動を開始するまでの遅延時間の最小値。
		int   evadeReactionDelayMax = 0;		 //! 回避行動を開始するまでの遅延時間の最大値。
		float threatMarginScale = 1.0f;			 //! 敵の脅威度を計算する際のマージン倍率。1.0f で通常、1.5f で 50% 増し、0.5f で半分になる。
		float aggression = 1.0f;				 //! 攻撃性。1.0f で通常、1.5f で 50% 増し、0.5f で半分になる。

		float rangedChaseEnterOffset = 40.0f;    //! 攻撃距離に入るまでの距離。敵が攻撃距離に入る前に、少し余裕を持たせるためのオフセット。
		float rangedChaseExitOffset = 15.0f;     //! 攻撃距離から出るまでの距離。敵が攻撃距離から出る前に、少し余裕を持たせるためのオフセット。
		float rangedRetreatEnterOffset = 50.0f;  //! 攻撃距離から退避するまでの距離。敵が攻撃距離から退避する前に、少し余裕を持たせるためのオフセット。
		float rangedRetreatExitOffset = 35.0f;   //! 攻撃距離から退避するまでの距離。敵が攻撃距離から退避する前に、少し余裕を持たせるためのオフセット。

		bool  preferRunInEvade = true;			 //! 回避行動中に走ることを優先するかどうか。true で優先、false で優先しない。
	};
}