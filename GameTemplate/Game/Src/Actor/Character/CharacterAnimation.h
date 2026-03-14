#pragma once
/**
* @file   CharacterAnimation.h
* @brief  キャラクターのアニメーションを管理するクラス。
* @author Yamaguchi Hayato。
* @date   2026/03/11
*/


namespace nsApp
{
	/* キャラクターの基本動作用アニメーションを管理する列挙型。*/
	enum class CharacterBasicAnimationList : uint8_t
	{
		Idle,       /* 待機。*/
		Walk,       /* 歩き。*/
		Run,        /* 走り。*/
		Jump,       /* ジャンプ。*/
		Hit_Fly,    /* 被弾。*/
		Hit_UP,     /* 起き上がり。*/
		Death,      /* 死亡。*/
	};

	/* 攻撃の種類を管理する列挙型。*/
	enum class AttackType : uint8_t
	{
		NormalAttack,   /* 通常攻撃。*/
		ChargeAttack,   /* チャージ攻撃。*/
		AirAttack,      /* 空中攻撃。*/
		ComboAttack1,   /* コンボ攻撃1撃目。*/
		ComboAttack2,   /* コンボ攻撃2撃目。*/
		ComboAttack3,   /* コンボ攻撃3撃目。*/
		None,		    /* 攻撃なし。*/
	};

	/* 武器のタイプを設定。*/
	enum class WeaponType : uint8_t
	{
		GreatSword, /* 大剣。*/
		TwinSword,  /* 双剣。*/
		Axe,        /* 斧。*/
		Wand,       /* 杖。*/
		Parasol,    /* 傘。*/
		None,       /* 武器なし。*/
	};

	/* 武器ごとのアニメーションと表示のズレをまとめた構造体。*/
	struct WeaponData
	{
		/* 武器モデルの描画を管理。*/
		std::string weaponModelPath; /* 武器モデルのファイルパス。*/
		std::wstring attackBoneName; /* 攻撃の基準となるボーンの名前。*/

		/* 攻撃の武器の動きのみを管理する配列。*/
		std::unordered_map<AttackType, std::string> weaponAnimationList; /* 攻撃の種類ごとの武器アニメーションのファイルパス。*/
	};



	class CharacterAnimation
	{
	public:
		/* コンストラクタとデストラクタ。*/
		CharacterAnimation() = default;
		virtual ~CharacterAnimation() = default;


	public:
		/* 初期化処理。*/
		void Initialize();

		/* 武器を持ち替えたときに呼んで必要なアニメーションを読み込む。
		* @param weaponType 持ち替える武器のタイプ。
		*/
		void LoadAnimation(WeaponType weaponType);

		
	/* ゲッター。*/
	public:
		/* 再生したいアニメーションの状態を渡す処理。*/
		inline int GetBasicAnimationIndex(CharacterBasicAnimationList state)
		{
			return m_basicIndexMap.count(state) ? m_basicIndexMap[state] : 0;
		}

	    /* 渡したい配列の要素数を数える処理。*/
		inline int GetAttackAnimationIndex(AttackType attack)
		{
			return m_attackIndexMap.count(attack) ? m_attackIndexMap[attack] : 0;
		}

		/* 基本動作用アニメーションのファイルパスを取得する処理。*/
		inline const std::string GetBasicAnimationFilePath(const std::string filePath)
		{
			const std::string basicAnimation = m_basicAnimationFilePath + filePath + m_animationExtension;
			return basicAnimation;
		}

		/* 武器専用のアニメーションのファイルパスを取得する処理。*/
		inline const std::string GetWeaponAnimationFilePath(const std::string filePath)
		{
			const std::string weaponAnimation = m_weaponAnimationFilePath + filePath + m_animationExtension;
			return weaponAnimation;
		}

		/* ModelRenderにアニメーションを渡す配列。*/
		inline AnimationClip* GetAnimatiocClip()
		{
			return m_animationClipList.get();
		}

		/* AnimationClipの数を取得。*/
		inline int GetAnimationClips() const
		{
			return m_animationNum;
		}

		/* 武器のデータを取得。*/
		inline const WeaponData& GetWeaponData(WeaponType type)const
		{
			return m_weaponDataList.at(type);
		}

	/* セッター。*/
	public:
		/* アニメーションの共通設定項目をセット。*/
		int SetAnimationClip(const std::string filePath, bool isLoop)
		{
			/* アニメーションをロード。*/
			m_animationClipList[m_currentIndex].Load(filePath.c_str());
			/* アニメーションをループするか設定。*/
			m_animationClipList[m_currentIndex].SetLoopFlag(isLoop);
			/* 配列を加算する。*/
			return m_currentIndex++;
		}


	private:
		/* BaseAnimationのファイルパスを格納。*/
		void InitBasicAnimationFilePath();

		/* GreatSwordのアニメーションファイルパスを格納。*/
		void InitGreatSwordAnimationFilePath();


	private:
		std::unordered_map<CharacterBasicAnimationList, std::string> m_basicAnimationFilePathList;                  /* 基本動作用アニメーションのファイルパスを管理するマップ。*/
		std::unordered_map<WeaponType, WeaponData> m_weaponDataList;                                                /* 武器ごとのアニメーションと表示のズレを管理するマップ。*/

        /* 読み込んだアニメーションの要素数を代入する変数。*/
		std::unordered_map<CharacterBasicAnimationList, int> m_basicIndexMap;
		std::unordered_map<AttackType, int> m_attackIndexMap;

		std::unique_ptr<AnimationClip[]> m_animationClipList;                                                       /* 読み込んだアニメーションを管理する配列。*/


		/* ファイルパスを定数化するための変数群。*/
		const std::string m_basicAnimationFilePath = "Assets/animData/Player/BasicAnimation/";                      /* 基本動作用アニメーションのファイルパスの共通部分。*/
		const std::string m_weaponAnimationFilePath = "Assets/animData/Player/";                                    /* 武器ごとのアニメーションのファイルパスの共通部分。*/
		const std::string m_animationExtension = ".tka";                                                            /* アニメーションファイルの拡張子。*/

		/* 武器の種類ごとに代入用変数を設定。*/
		WeaponData m_greatSwordData;                                                                                /* 大剣のアニメーションと表示のズレを管理する変数。*/

		int m_currentIndex = 0;
		int m_animationNum = 0;                                                                                     /* 読み込んだアニメーションの数を管理する変数。*/
		
	};
}

