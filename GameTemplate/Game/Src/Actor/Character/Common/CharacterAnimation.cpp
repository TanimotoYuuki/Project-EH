#include "stdafx.h"
#include "CharacterAnimation.h"


namespace nsApp
{
	void CharacterAnimation::Initialize()
	{
		/* 基本動作用のアニメーションのファイルパスを初期化。*/
		InitBasicAnimationFilePath();
		
		/* 大剣用のアニメーションのファイルパスを初期化。*/
		InitGreatSwordAnimationFilePath();
	}


	void CharacterAnimation::InitBasicAnimationFilePath()
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
				pair.first == CharacterBasicAnimationList::Hit_UP)
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