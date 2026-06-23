#pragma once

/**
 * @file   NPCBrain.h
 * @brief  NPC の脳幹。FSM・Threat・3 Mind を束ね、State へ API を提供する。
 * @author Yamaguchi Hayato。
 * @date   2026/04/27
 */

#include "Src/Actor/Character/Common/IState.h"
#include "Src/Actor/Character/Common/ICharacter.h"
#include "Src/Actor/Character/NPC/Threat/ThreatCollector.h"
#include "Src/Actor/Character/NPC/Threat/BossMeleeThreatProvider.h"
#include "Src/Actor/Character/NPC/Navigation/AvoidPathResult.h"
#include "Src/Actor/Character/NPC/Component/NPCBehaviorProfile.h"
#include "Src/Actor/Character/NPC/Threat/BossFireThreatProvider.h"
#include "Src/Actor/Character/NPC/Brain/NPCMindContext.h"
#include "Src/Actor/Character/NPC/Brain/NPCCombatMind.h"
#include "Src/Actor/Character/NPC/Brain/NPCLocomotonMind.h"
#include "Src/Actor/Character/NPC/Brain/NPCSupportMind.h"

namespace nsApp
{
	namespace nsActor {
		class Player;
	}

	class VirtualInputAdapter;

	/**
	 * @class NPCBrain
	 * @brief NPC の思考中枢。State から呼ばれる判断 API の窓口。
	 */
	class NPCBrain
	{
	public:
		NPCBrain() = default;
		virtual ~NPCBrain();

	public:
		/**
		 * @brief 毎フレームの更新。インターバル・Threat・救助対象・FSM を進める。
		 */
		void Update();

		/**
		 * @brief NPC ステートを切り替える。
		 * @param nextState 遷移先ステート。
		 */
		inline void ChangeState(nsState::IState<NPCBrain>* nextState)
		{
			if (m_npcStateMachine)
				m_npcStateMachine->ChangeState(nextState);
		}

		/**
		 * @brief 戦闘ターゲット（ボス）を探索する。
		 * @return 有効なボス。いなければ nullptr。
		 */
		nsActor::ICharacter* SearchTarget();

		/**
		 * @brief 攻撃インターバルが経過し、攻撃可能か。
		 * @return 攻撃可能なら true。
		 */
		bool CanAttack() const;

		/**
		 * @brief 攻撃インターバルタイマーを 1 フレーム減算する。
		 */
		void UpdateAttackInterval();

		/**
		 * @brief 最寄りのダウン味方を探索する。
		 * @return 救助対象。いなければ nullptr。
		 */
		nsActor::Player* SearchHelpTarget() const;

		/**
		 * @brief 現在位置が危険か（Evade ステートへ遷移すべきか）。
		 * @note  ThreatCollector の危険度とプロファイル閾値で判定。反応遅延あり。
		 */
		bool ShouldEvade();

		/**
		 * @brief 今フレーム有効な危険ゾーン一覧。
		 * @note  Pathfinder が参照する窓口。
		 */
		inline const std::vector<nsNPC::IThreatZone*>& GetActiveThreatZones() const
		{
			return m_threatCollector.GetActiveZones();
		}

		/**
		 * @brief 回避移動方向を返す。
		 * @note  SimpleAvoidPathfinder へ委譲。
		 */
		nsNPC::AvoidPathResult GetEvadeDirection() const;

		/**
		 * @brief 対象が救助が必要なダウン状態か。
		 * @param target 判定対象のプレイヤー。
		 * @return 救助が必要なら true。
		 */
		bool IsDownedAllyNeedingHelp(nsActor::Player* target) const;

		/**
		 * @brief この NPC が救助役か（HelpPriority + パーティ番号で決定）。
		 * @return 自分が行くべきなら true。
		 */
		bool ShouldRespondToHelp() const;

		/**
		 * @brief 自分の HP が低く、自己回復すべきか（Wand 専用）。
		 * @return 自己回復すべきなら true。
		 */
		bool ShouldHealSelf() const;

		/**
		 * @brief 味方の HP が低く、回復魔法を使うべきか（Wand 専用）。
		 * @note  救助中は false。致命圏チェックは Brain 側で行う。
		 * @return 味方回復すべきなら true。
		 */
		bool ShouldHealAlly() const;

		/**
		 * @brief 回復対象の味方（最も HP が低い者）。
		 * @return 対象。いなければ nullptr。
		 */
		nsActor::Player* FindAllyNeedingHeal() const;

		/**
		 * @brief ボスが被ダメ硬直中か（追い打ちチャンス）。
		 * @return 攻撃チャンスなら true。
		 */
		bool IsBossAttackWindow() const;


	public:
		/**
		 * @brief 初期化。ステートマシン・Threat・行動プロファイルをセットアップする。
		 * @param outer この Brain を持つ NPC（Player）。
		 */
		void Init(nsActor::Player* outer);

		/**
		 * @brief VirtualInputAdapter をセットする。
		 * @param virtualInput 仮想入力アダプタ。
		 */
		inline void SetVirtualInputAdapter(VirtualInputAdapter* virtualInput)
		{
			m_virtualInputAdapter = virtualInput;
		}

		/**
		 * @brief 攻撃インターバル長をセットする。
		 * @param intervalFrame インターバルフレーム数。
		 */
		void SetAttackInterval(int intervalFrame);

		/**
		 * @brief 攻撃終了後、インターバルカウントを開始する。
		 */
		void StartAttackInterval();

		/**
		 * @brief パーティ番号をセットする（救助役の同優先度分担用）。
		 * @param partyIndex パーティ番号（0 始まり）。
		 */
		void SetPartyIndex(int partyIndex);

		/**
		 * @brief この Brain を持つ NPC 本体を取得する。
		 * @return Player ポインタ。
		 */
		inline nsActor::Player* GetBody() const
		{
			return m_outer;
		}

		/**
		 * @brief VirtualInputAdapter を取得する。
		 * @return 仮想入力アダプタ。
		 */
		inline VirtualInputAdapter* GetVirtualInputAdapter() const
		{
			return m_virtualInputAdapter;
		}

		/**
		 * @brief 今フレームの救助対象を取得する。
		 * @return 救助対象。いなければ nullptr。
		 */
		inline nsActor::Player* GetHelpTarget() const
		{
			return m_helpTarget;
		}

		/**
		 * @brief 武器別行動プロファイルを取得する。
		 * @return NPCBehaviorProfile の参照。
		 */
		inline const NPCBehaviorProfile& GetBehaviorProfile() const
		{
			return m_profile;
		}

		/**
		 * @brief パーティ番号を取得する。
		 * @return パーティ番号。
		 */
		inline int GetPartyIndex() const
		{
			return m_partyIndex;
		}

		/**
         * @brief 接近中フラグ（ヒステリシス用）。
		 */
		inline bool& GetIsApproachingFlag()
		{
			return m_isApproaching;
		}

		/**
		 * @brief 後退中フラグ（ヒステリシス用）。
		 */
		inline bool& GetIsRetreatingFlag()
		{
			return m_isRetreating;
		}

		/**
		 * @brief この NPC が救助役か（HelpPriority + パーティ番号で決定）。
		 * @return 自分が行くべきなら true。
		 */
		bool ShouldGuard() const;

		/**
		 * @brief Mind 共有コンテキストを最新化する。
		 */
		void RefreshMindContext();

		/**
		 * @brief const メソッドから Mind を呼ぶためのスナップショットを作る。
		 * @return 現在の Mind コンテキスト。
		 */
		NPCMindContext BuildMindContext() const;

		/**
		 * @brief 現在位置が致命圏か（Evade 閾値超え）。
		 * @return 致命圏なら true。
		 */
		bool IsDangerous() const
		{
			return m_locomotionMind.IsDangerous(BuildMindContext());
		}

		int CountAlliesNeedingHeal() const
		{
			return m_supportMind.CountAlliesNeedingHeal(BuildMindContext());
		}

		bool ShouldPrioritizeHeal() const
		{
			return m_supportMind.ShouldPrioritizeHeal(BuildMindContext());
		}

	private:
		nsActor::Player* m_outer = nullptr;                           //! NPC 本体。
		nsActor::Player* m_helpTarget = nullptr;                      //! 今フレームの救助対象。
		nsState::StateMachine<NPCBrain>* m_npcStateMachine = nullptr; //! NPC ステートマシン。
		nsNPC::ThreatCollector m_threatCollector;                     //! 危険ゾーン集約。
		nsNPC::BossMeleeThreatProvider m_bossMeleeThreatProvider;     //! ボス近接の危険源。
		nsNPC::BossFireThreatProvider m_bossFireThreatProvider;       //! ボス火炎の危険源。
		VirtualInputAdapter* m_virtualInputAdapter = nullptr;         //! 仮想入力アダプタ。
		NPCBehaviorProfile m_profile;                                 //! 武器別行動プロファイル。

		NPCMindContext m_mindContext;								  //!  Mind 共有コンテキスト。
		NPCCombatMind m_combatMind;									  //! 左脳：攻撃。
		NPCLocomotionMind m_locomotionMind;							  //! 右脳：移動・回避。
		NPCSupportMind m_supportMind;								  //! 後頭部：救助・回復。

		int m_partyIndex = 0;										  //! パーティ番号。

		bool m_isApproaching = false;			  //! 接近中フラグ（ヒステリシス用）。
		bool m_isRetreating = false;			  //! 後退中フラグ（ヒステリシス用）。

	};
}