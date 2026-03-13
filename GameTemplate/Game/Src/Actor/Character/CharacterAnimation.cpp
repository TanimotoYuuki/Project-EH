#include "stdafx.h"
#include "CharacterAnimation.h"

namespace nsApp
{
	void CharacterAnimation::Initialize()
	{
		/* 基本動作用のアニメーションのファイルパスを初期化。*/
		InitBasicAnimationFilePath();
		
		/* 大剣用のアニメーションのファイルパスを初期化。*/
//		InitGreatSwordAnimationFilePath();
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
		/* 通常攻撃。*/
		m_greatSwordData.weaponAnimationList[AttackType::NormalAttack] = GetWeaponAnimationFilePath("GreatSword/GreatSword_Idle");
		/* チャージ攻撃。*/
		m_greatSwordData.weaponAnimationList[AttackType::ChargeAttack] = GetWeaponAnimationFilePath("GreatSword/GreatSword_ChargeAttack");
		/* コンボ攻撃。*/
		/* 1段目コンボ。*/
		m_greatSwordData.weaponAnimationList[AttackType::ComboAttack1] = GetWeaponAnimationFilePath("GreatSword/GreatSword_Combo_01");
		/* 2段目コンボ。*/
		m_greatSwordData.weaponAnimationList[AttackType::ComboAttack2] = GetWeaponAnimationFilePath("GreatSword/GreatSword_Combo_02");
		/* 3段目コンボ。*/
		m_greatSwordData.weaponAnimationList[AttackType::ComboAttack3] = GetWeaponAnimationFilePath("GreatSword/GreatSword_Combo_03");
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
			m_basicIndexMap[pair.first] = SetAnimationClip(pair.second, true);
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