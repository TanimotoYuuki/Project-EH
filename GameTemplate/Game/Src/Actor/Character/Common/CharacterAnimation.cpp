#include "stdafx.h"
#include "CharacterAnimation.h"


namespace nsApp
{
	void CharacterAnimation::Initialize(WeaponType kind)
	{		
		switch (kind)
		{
		case WeaponType::GreatSword:
		{
			 /* ソードキャラクターのアニメーションのファイルパスを初期化。*/
             /* 基本動作用。*/
			 InitGreatSwordBasicAnimationFilePath();
			 /* 攻撃動作用。*/
			 InitGreatSwordAnimationFilePath();
			 break;
		}

		case WeaponType::Hammer:
		{
			/* ハンマーキャラクターのアニメーションのファイルパスを初期化。*/
			/* 基本動作用。*/
			InitHammerBasicAnimationFilePath();
			/* 攻撃動作用。*/
			InitHammerAnimationFilePath();
			break;
		}

		case WeaponType::Wand:
		{
			/* 杖キャラクターのアニメーションファイルパスを初期化。*/
            /* 基本動作用。*/
			InitWandBasicAnimationFilePath();
			/* 攻撃動作用。*/
			InitWandAttackAnimationFilePath();
			break;
		}

		case WeaponType::TwinGun:
		{
			/* 銃キャラクターのアニメーションファイルパスを初期化。*/
			/* 基本動作用。*/
			InitTwinGunBasicAnimationFilePath();
			/* 攻撃動作用。*/ 
			InitTwinGunAttackAnimationFilePath();
			break;
		}

		default:
			break;
		}
	}


	void CharacterAnimation::InitGreatSwordBasicAnimationFilePath()
	{
		/* 基本動作用のアニメーションのファイルパスを登録。。*/
		/* Idleアニメーション。*/
		m_basicAnimationFilePathList[CharacterBasicAnimationList::Idle] = GetBasicAnimationFilePath("GreatSword/Idle");

        /* Walkアニメーション。*/
		m_basicAnimationFilePathList[CharacterBasicAnimationList::Walk] = GetBasicAnimationFilePath("GreatSword/Walk");

		/* Runアニメーション。*/
		m_basicAnimationFilePathList[CharacterBasicAnimationList::Run] = GetBasicAnimationFilePath("GreatSword/Run");

		/* Jumpアニメーション。*/
		m_basicAnimationFilePathList[CharacterBasicAnimationList::Jump] = GetBasicAnimationFilePath("GreatSword/Jump");

		/* Hit(吹っ飛び)アニメーション。*/
		m_basicAnimationFilePathList[CharacterBasicAnimationList::Hit_Fly] = GetBasicAnimationFilePath("GreatSword/Hit_Fly");

		/* Hit(起き上がり)アニメーション。*/
		m_basicAnimationFilePathList[CharacterBasicAnimationList::Hit_UP] = GetBasicAnimationFilePath("GreatSword/Hit_Up");

		/* Deathアニメーション。*/ 
		m_basicAnimationFilePathList[CharacterBasicAnimationList::Death] = GetBasicAnimationFilePath("GreatSword/Deth");

       /* Guardアニメーション。*/
		m_basicAnimationFilePathList[CharacterBasicAnimationList::Guard] = GetBasicAnimationFilePath("GreatSword/Guard");

        /* Helpアニメーション。*/
		m_basicAnimationFilePathList[CharacterBasicAnimationList::Help] = GetBasicAnimationFilePath("GreatSword/Help");
	}


	void CharacterAnimation::InitGreatSwordAnimationFilePath()
	{
		/* GreatSwordのアニメーションファイルパスを登録。*/
		/* 待機状態。*/
		m_greatSwordData.weaponAnimationList[AttackType::NormalAttack] = GetWeaponAnimationFilePath("GreatSword/GreatSword_NormalAttack");

		/* チャージ中。*/
		m_greatSwordData.weaponAnimationList[AttackType::Charging] = GetWeaponAnimationFilePath("GreatSword/GreatSword_Charge");

		/* チャージ攻撃。*/
		m_greatSwordData.weaponAnimationList[AttackType::ChargeAttack] = GetWeaponAnimationFilePath("GreatSword/GreatSword_ChargeAttack");

		/* 空中攻撃。*/
		m_greatSwordData.weaponAnimationList[AttackType::AirAttack] = GetWeaponAnimationFilePath("GreatSword/GreatSword_AirAttack");

		/* 連続攻撃。*/
		m_greatSwordData.weaponAnimationList[AttackType::RushAttack_Start] = GetWeaponAnimationFilePath("GreatSword/GreatSword_Rush_Start");
		m_greatSwordData.weaponAnimationList[AttackType::RushAttack_End] = GetWeaponAnimationFilePath("GreatSword/GreatSword_Rush_End");

		/* 斬り上げ攻撃。*/
		m_greatSwordData.weaponAnimationList[AttackType::SlashUp] = GetWeaponAnimationFilePath("GreatSword/GreatSword_SlashUpward");

		/* 突き進む攻撃。*/
		m_greatSwordData.weaponAnimationList[AttackType::PushForward] = GetWeaponAnimationFilePath("GreatSword/GreatSword_Push");

		/* 登録データをリストに登録する。*/
		m_weaponDataList[WeaponType::GreatSword] = m_greatSwordData;
	}


	void CharacterAnimation::InitHammerBasicAnimationFilePath()
	{
		/* Hammerのアニメーションを登録。*/
        /* 待機状態。*/
		m_basicAnimationFilePathList[CharacterBasicAnimationList::Idle] = GetBasicAnimationFilePath("Hammer/Idle");

		/* 歩き状態。*/
		m_basicAnimationFilePathList[CharacterBasicAnimationList::Walk] = GetBasicAnimationFilePath("Hammer/Walk");

		/* 走り状態。*/
		m_basicAnimationFilePathList[CharacterBasicAnimationList::Run] = GetBasicAnimationFilePath("Hammer/Run");

		/* ジャンプ状態。*/
		m_basicAnimationFilePathList[CharacterBasicAnimationList::Jump] = GetBasicAnimationFilePath("Hammer/Jump");

		/* 被弾状態。*/
		m_basicAnimationFilePathList[CharacterBasicAnimationList::Hit_Fly] = GetBasicAnimationFilePath("Hammer/Fly_Hit");

		/* 起き上がり状態。*/
		m_basicAnimationFilePathList[CharacterBasicAnimationList::Hit_UP] = GetBasicAnimationFilePath("Hammer/Fly_GetUp");

		/* 死亡状態。*/
		m_basicAnimationFilePathList[CharacterBasicAnimationList::Death] = GetBasicAnimationFilePath("Hammer/Deth");

		/* ガード状態。*/
		m_basicAnimationFilePathList[CharacterBasicAnimationList::Guard] = GetBasicAnimationFilePath("Hammer/Guard");

		/* 助ける状態。*/
		m_basicAnimationFilePathList[CharacterBasicAnimationList::Help] = GetBasicAnimationFilePath("Hammer/Help");

		m_weaponDataList[WeaponType::Hammer] = m_hammerData;
	}


	void CharacterAnimation::InitHammerAnimationFilePath()
	{
		/* Hammerのアニメーションファイルパスを格納。*/
        /* 通常攻撃。*/
		m_hammerData.weaponAnimationList[AttackType::NormalAttack] = GetWeaponAnimationFilePath("Hammer/NormalAttack");

		/* 強攻撃。*/
		m_hammerData.weaponAnimationList[AttackType::HeavyAttack] = GetWeaponAnimationFilePath("Hammer/HeavyAttack");

		/* チャージ中。*/
	    m_hammerData.weaponAnimationList[AttackType::Charging] = GetWeaponAnimationFilePath("Hammer/Charging");

		/* チャージ攻撃。*/
	    m_hammerData.weaponAnimationList[AttackType::ChargeAttack] = GetWeaponAnimationFilePath("Hammer/HeavyAttack");

		/* 空中攻撃。*/
		m_hammerData.weaponAnimationList[AttackType::AirAttack] = GetWeaponAnimationFilePath("Hammer/AirAttack");

		/* 回転攻撃。*/
		m_hammerData.weaponAnimationList[AttackType::PushForward] = GetWeaponAnimationFilePath("Hammer/Spinning");



		/* 登録データをリストに登録。*/
		m_weaponDataList[WeaponType::Hammer] = m_hammerData;
	}


	void CharacterAnimation::InitWandBasicAnimationFilePath()
	{
		/* 待機状態。*/
		m_basicAnimationFilePathList[CharacterBasicAnimationList::Idle] = GetBasicAnimationFilePath("Wand/Idle");

		/* 歩き状態。*/
		m_basicAnimationFilePathList[CharacterBasicAnimationList::Walk] = GetBasicAnimationFilePath("Wand/Walk");

		/* 走り状態。*/
		m_basicAnimationFilePathList[CharacterBasicAnimationList::Run] = GetBasicAnimationFilePath("Wand/Run");

		/* ジャンプ状態。*/
		m_basicAnimationFilePathList[CharacterBasicAnimationList::Jump] = GetBasicAnimationFilePath("Wand/Jump");

		/* 被弾状態。*/
		m_basicAnimationFilePathList[CharacterBasicAnimationList::Hit_Fly] = GetBasicAnimationFilePath("Wand/Hit");

		/* 起き上がり状態。*/
		m_basicAnimationFilePathList[CharacterBasicAnimationList::Hit_UP] = GetBasicAnimationFilePath("Wand/GetUp");

		/* 死亡状態。*/
		m_basicAnimationFilePathList[CharacterBasicAnimationList::Death] = GetBasicAnimationFilePath("Wand/Deth");

		/* ガード状態。*/
		m_basicAnimationFilePathList[CharacterBasicAnimationList::Guard] = GetBasicAnimationFilePath("Wand/Guard");

		/* 助ける状態。*/
		m_basicAnimationFilePathList[CharacterBasicAnimationList::Help] = GetBasicAnimationFilePath("Wand/Help");

		/* 登録したアニメーションをリストに登録。*/
		m_weaponDataList[WeaponType::Wand] = m_wandData;
	}


	void CharacterAnimation::InitWandAttackAnimationFilePath()
	{
		/* 通常攻撃。*/
		m_wandData.weaponAnimationList[AttackType::NormalAttack] = GetWeaponAnimationFilePath("Wand/NormalAttack");

        /* 連打攻撃。*/
		m_wandData.weaponAnimationList[AttackType::RushAttack_Start] = GetWeaponAnimationFilePath("Wand/RushAttack");

		/* 連打終了(仮)*/
		m_wandData.weaponAnimationList[AttackType::RushAttack_End] = GetWeaponAnimationFilePath("Wand/RushAttack");

       /* 空中攻撃。*/
		m_wandData.weaponAnimationList[AttackType::AirAttack] = GetWeaponAnimationFilePath("Wand/ChargeAttack");

		/* 斬り上げ枠。*/
		m_wandData.weaponAnimationList[AttackType::SlashUp] = GetWeaponAnimationFilePath("Wand/AirAttack");

        /* チャージ中。*/
		m_wandData.weaponAnimationList[AttackType::Charging] = GetWeaponAnimationFilePath("Wand/Charging");

		/* チャージ攻撃。*/
		m_wandData.weaponAnimationList[AttackType::ChargeAttack] = GetWeaponAnimationFilePath("Wand/ChargeAttack");

		/* 魔法攻撃。*/
		m_wandData.weaponAnimationList[AttackType::MagicAttack] = GetWeaponAnimationFilePath("Wand/MagicAttack");

		/* 回復魔法。*/
		m_wandData.weaponAnimationList[AttackType::HeelMagic] = GetWeaponAnimationFilePath("Wand/HeelMagic");

		/* アニメーションデータをリストに登録。*/
		m_weaponDataList[WeaponType::Wand] = m_wandData;
	}


	void CharacterAnimation::InitTwinGunBasicAnimationFilePath()
	{
		/* 待機状態。*/ 
		m_basicAnimationFilePathList[CharacterBasicAnimationList::Idle] = GetBasicAnimationFilePath("Gun/Idle");

		/* 歩き状態。*/
		m_basicAnimationFilePathList[CharacterBasicAnimationList::Walk] = GetBasicAnimationFilePath("Gun/Walk");

		/* 走り状態。*/
		m_basicAnimationFilePathList[CharacterBasicAnimationList::Run] = GetBasicAnimationFilePath("Gun/Run");

		/* ジャンプ状態。*/
		m_basicAnimationFilePathList[CharacterBasicAnimationList::Jump] = GetBasicAnimationFilePath("Gun/Jump");

		/* 被弾状態。*/
		m_basicAnimationFilePathList[CharacterBasicAnimationList::Hit_Fly] = GetBasicAnimationFilePath("Gun/Hit");

		/* 起き上がり状態。*/
		m_basicAnimationFilePathList[CharacterBasicAnimationList::Hit_UP] = GetBasicAnimationFilePath("Gun/GetUp");

		/* 死亡状態。*/
		m_basicAnimationFilePathList[CharacterBasicAnimationList::Death] = GetBasicAnimationFilePath("Gun/Deth");

		/* ガード状態。*/
		m_basicAnimationFilePathList[CharacterBasicAnimationList::Guard] = GetBasicAnimationFilePath("Gun/Guard");

		/* 助ける状態。*/
		m_basicAnimationFilePathList[CharacterBasicAnimationList::Help] = GetBasicAnimationFilePath("Gun/Help");

		/* 登録したアニメーションをリストに登録。*/
		m_weaponDataList[WeaponType::TwinGun] = m_greatSwordData;
	}


	void CharacterAnimation::InitTwinGunAttackAnimationFilePath()
	{
		/* 通常攻撃。*/
		m_twinGunData.weaponAnimationList[AttackType::NormalAttack] = GetWeaponAnimationFilePath("TwinGun/NormalAttack");

		/* チャージ攻撃。*/
		m_twinGunData.weaponAnimationList[AttackType::ChargeAttack] = GetWeaponAnimationFilePath("TwinGun/ChargeAttack");

		/* 乱射開始。*/
		m_twinGunData.weaponAnimationList[AttackType::RushAttack_Start] = GetWeaponAnimationFilePath("TwinGun/Rush_Start");

		/* 乱射終了。*/
		m_twinGunData.weaponAnimationList[AttackType::RushAttack_End] = GetWeaponAnimationFilePath("TwinGun/Rush_End");

		/* 空中攻撃。*/
		m_twinGunData.weaponAnimationList[AttackType::AirAttack] = GetWeaponAnimationFilePath("TwinGun/AirAttack");

		/* 重攻撃。*/
		m_twinGunData.weaponAnimationList[AttackType::HeavyAttack] = GetWeaponAnimationFilePath("TwinGun/HeavyAttack");

		/* 登録したアニメーションをリストに登録。*/
		m_weaponDataList[WeaponType::TwinGun] = m_twinGunData;
	}


	void CharacterAnimation::LoadAnimation(WeaponType weaponType)
	{
		/* アニメーションを読み込む前に箱をリセット。*/
		m_basicIndexMap.clear();
		m_attackIndexMap.clear();
		m_currentIndex = 0;

		/* 必要なアニメーションの合計数を計算する。*/
		m_animationNum = static_cast<int>(m_basicAnimationFilePathList.size());
		if (m_weaponDataList.count(weaponType) > 0)
			m_animationNum += static_cast<int>(m_weaponDataList[weaponType].weaponAnimationList.size());

		/* 合計数と同じになるように配列を組む。*/
		m_animationClipList = std::make_unique<AnimationClip[]>(m_animationNum);

		/* 基本動作をロード */
		for (auto& pair : m_basicAnimationFilePathList)
		{
			/* 特定のアニメーションは再生ループをオフにする。
			 * 今回はダメージアニメーション。  
			 */
			if (pair.first == CharacterBasicAnimationList::Hit_Fly ||
				pair.first == CharacterBasicAnimationList::Hit_UP ||
				pair.first == CharacterBasicAnimationList::Help)
				/* ダメージと死亡はループさせない。*/
				/* true だと ループ。*/
				m_isLoop = false;

			else
				/* それ以外はループするように。*/
				m_isLoop = true;

			/* ループ方式を m_isLoopに任せる。*/
			m_basicIndexMap[pair.first] = SetAnimationClip(pair.second, m_isLoop);
		}

		/* 武器用アニメーションをロード */
		if (m_weaponDataList.count(weaponType) > 0)
		{
			auto& weaponData = m_weaponDataList[weaponType];
			for (auto& pair : weaponData.weaponAnimationList)
			{
				// 攻撃はループさせないので false
				m_attackIndexMap[pair.first] = SetAnimationClip(pair.second, false);
			}
		}
	}
}