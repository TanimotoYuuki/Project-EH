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
		m_basicAnimationFilePathList[CharacterBasicAnimationList::Idle] = GetBasicAnimationFilePath("Idle");

        /* Walkアニメーション。*/
		m_basicAnimationFilePathList[CharacterBasicAnimationList::Walk] = GetBasicAnimationFilePath("Walk");

		/* Runアニメーション。*/
		m_basicAnimationFilePathList[CharacterBasicAnimationList::Run] = GetBasicAnimationFilePath("Run");

		/* Jumpアニメーション。*/
		m_basicAnimationFilePathList[CharacterBasicAnimationList::Jump] = GetBasicAnimationFilePath("Jump");

		/* Hitアニメーション。*/
		m_basicAnimationFilePathList[CharacterBasicAnimationList::Hit] = GetBasicAnimationFilePath("Hit");

		/* Deathアニメーション。*/ 
		m_basicAnimationFilePathList[CharacterBasicAnimationList::Death] = GetBasicAnimationFilePath("Death");
	}


	void CharacterAnimation::InitGreatSwordAnimationFilePath()
	{
		/* GreatSwordのアニメーションファイルパスを登録。*/
		/* 通常攻撃。*/
		m_greatSwordData.weaponModelPath = GetWeaponAnimationFilePath("GreatSword/GreatSword_Idle");
		/* チャージ攻撃。*/
		m_greatSwordData.weaponModelPath = GetWeaponAnimationFilePath("GreatSword/GreatSword_ChargeAttack");
		/* コンボ攻撃。*/
		/* 1段目コンボ。*/
		m_greatSwordData.weaponModelPath = GetWeaponAnimationFilePath("GreatSword/GreatSword_Combo_01");
		/* 2段目コンボ。*/
		m_greatSwordData.weaponModelPath = GetWeaponAnimationFilePath("GreatSword/GreatSword_Combo_02");
		/* 3段目コンボ。*/
		m_greatSwordData.weaponModelPath = GetWeaponAnimationFilePath("GreatSword/GreatSword_Combo_03");
	}


	void CharacterAnimation::LoadAnimation(WeaponType weaponType)
	{
		/* アニメーションを解放する。*/
		m_animationClipList.clear();
		m_basicIndexMap.clear();
		m_attackIndexMap.clear();

		/* アニメーションの箱を初期化する。*/
		m_currentIndex = 0;

		/* 基本動作アニメーションを動作。*/
		for (auto& pair : m_basicAnimationFilePathList)
		{
			/* 配列を加算する。*/
			/* 基本動作アニメーションはループさせるため true。*/
			m_basicIndexMap[pair.first] = SetAnimationClip(pair.second, true);
		}

		/* 武器用のアニメーションをロードする。*/
		if (m_weaponDataList.count(weaponType) > 0)
		{
			/* 武器の種類を配列にセットする。*/
			auto& weaponData = m_weaponDataList[weaponType];
			/* リストを登録する。*/
			for (auto& pair : weaponData.weaponAnimationList)
			{
				/* 配列を加算する。*/
				/* 攻撃アニメーションはループさせないため false。*/
				m_attackIndexMap[pair.first] = SetAnimationClip(pair.second, false);
			}
		}
	}
}