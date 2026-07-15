#include "stdafx.h"
#include "CharacterLoadContext.h"
#include "k2EngineLow.h"

#include "Src/Actor/Character/Common/CharacterAnimation.h"
#include "Src/Actor/Character/Common/CharacterModel.h"

#include <unordered_set>

namespace
{
	/**
	 * @brief 武器タイプからプレイヤーの CharacterModelType を解決する。
	 * @note  CharacterByWeapon 各クラスの m_modelType 割り当てに合わせる。
	 *        TwinGun→1P / Hammer→2P / GreatSword→3P / Wand→4P。
	 */
	nsApp::CharacterModelType ResolvePlayerModelType(nsApp::WeaponType weaponType)
	{
		using nsApp::WeaponType;
		using nsApp::CharacterModelType;

		switch (weaponType)
		{
		case WeaponType::TwinGun:
			return CharacterModelType::Player_1P;
		case WeaponType::Hammer:
			return CharacterModelType::Player_2P;
		case WeaponType::GreatSword:
			return CharacterModelType::Player_3P;
		case WeaponType::Wand:
			return CharacterModelType::Player_4P;
		default:
			return CharacterModelType::Player_1P;
		}
	}

	/**
	 * @brief 武器タイプから武器の CharacterModelType を解決する。
	 */
	nsApp::CharacterModelType ResolveWeaponModelType(nsApp::WeaponType weaponType)
	{
		using nsApp::WeaponType;
		using nsApp::CharacterModelType;

		switch (weaponType)
		{
		case WeaponType::GreatSword:
			return CharacterModelType::Weapon_GreatSword;
		case WeaponType::Hammer:
			return CharacterModelType::Weapon_Hammer;
		case WeaponType::Wand:
			return CharacterModelType::Weapon_Wand;
		case WeaponType::TwinGun:
			return CharacterModelType::Weapon_TwinGun;
		default:
			return CharacterModelType::Weapon_GreatSword;
		}
	}

	/**
	 * @brief リクエストから先読み対象パス一覧を作る（重複除去）。
	 * @note  パス文字列は CharacterModel の m_filePathList を唯一のソースにする。
	 */
	std::vector<std::string> BuildPrefetchPathList(const nsApp::InGameBuildRequest& request)
	{
		/* コンストラクタで m_filePathList が埋まる。ModelRender の Init はしない。*/
		nsApp::CharacterModel pathTable;

		std::unordered_set<std::string> uniquePaths;
		std::vector<std::string> paths;

		auto addType = [&](nsApp::CharacterModelType type)
			{
				const std::string path = pathTable.GetModelFilePath(type);
				if (path.empty())
					return;
				if (uniquePaths.insert(path).second)
					paths.push_back(path);
			};

		/* 4 人分のプレイヤーモデル＋武器モデル。*/
		for (int i = 0; i < 4; ++i)
		{
			const auto weaponType = static_cast<nsApp::WeaponType>(request.characterRole[i]);
			if (weaponType == nsApp::WeaponType::None)
				continue;

			addType(ResolvePlayerModelType(weaponType));
			addType(ResolveWeaponModelType(weaponType));
		}

		/* ボスモデル（BuildBoss の bossType + TutorialBoss と同じ）。*/
		const auto bossType = static_cast<nsApp::CharacterModelType>(
			request.bossType + static_cast<int>(nsApp::CharacterModelType::TutorialBoss));
		addType(bossType);

		return paths;
	}
} // namespace

namespace nsApp
{
	std::mutex CharacterLoadContext::mutex_;
	std::vector<CharacterLoadContext::PrefetchEntry> CharacterLoadContext::entries_;
	bool CharacterLoadContext::isReady_ = false;


	void CharacterLoadContext::Reset()
	{
		std::lock_guard<std::mutex> lock(mutex_);

		/* バンク未登録ならこちらで破棄する。登録済みはバンクが所有する。*/
		for (auto& entry : entries_)
		{
			if (entry.tkmFile != nullptr && !entry.isRegistered)
				delete entry.tkmFile;
			entry.tkmFile = nullptr;
		}

		entries_.clear();
		isReady_ = false;
	}


	void CharacterLoadContext::PrepareOnWorker(const InGameBuildRequest& request)
	{
		/* 先読み対象パスを組み立てる（CharacterModel 経由）。*/
		const std::vector<std::string> paths = BuildPrefetchPathList(request);
		std::vector<PrefetchEntry> loadedEntries;
		loadedEntries.reserve(paths.size());

		bool allOk = !paths.empty();

		/* パスごとに TkmFile を Load する（GPU 作成は Model::Init 側）。*/
		for (const auto& path : paths)
		{
			PrefetchEntry entry;
			entry.path = path;

			auto* tkm = new nsK2EngineLow::TkmFile;
			/* isOptimize=false, isLoadTexture=true（DDS バイトまで読み込む）。*/
			const bool ok = tkm->Load(path.c_str(), false, true);
			if (!ok)
			{
				delete tkm;
				tkm = nullptr;
				allOk = false;
			}

			entry.tkmFile = tkm;
			entry.isRegistered = false;
			loadedEntries.push_back(entry);
		}

		/* 結果を共有領域へ書き込む。*/
		std::lock_guard<std::mutex> lock(mutex_);

		/* 前回の未登録分があれば破棄する。*/
		for (auto& entry : entries_)
		{
			if (entry.tkmFile != nullptr && !entry.isRegistered)
				delete entry.tkmFile;
		}

		entries_ = std::move(loadedEntries);
		isReady_ = allOk;
	}


	void CharacterLoadContext::RegisterToBankOnMain()
	{
		std::lock_guard<std::mutex> lock(mutex_);

		/* 先読み未完了なら何もしない。*/
		if (!isReady_ || entries_.empty())
			return;

		for (auto& entry : entries_)
		{
			/* 既に登録済み、または Load 失敗分はスキップ。*/
			if (entry.isRegistered || entry.tkmFile == nullptr || entry.path.empty())
				continue;

			/* 既に同パスがバンクにある場合は、新規分を破棄して成功扱い。*/
			if (g_engine->GetTkmFileFromBank(entry.path.c_str()) != nullptr)
			{
				delete entry.tkmFile;
				entry.tkmFile = nullptr;
				entry.isRegistered = true;
				continue;
			}

			/* メインスレッドでバンクへ登録する。所有権はバンク側へ移る。*/
			g_engine->RegistTkmFileToBank(entry.path.c_str(), entry.tkmFile);
			entry.isRegistered = true;
		}
	}


	bool CharacterLoadContext::IsReady()
	{
		std::lock_guard<std::mutex> lock(mutex_);
		return isReady_;
	}
} // namespace nsApp