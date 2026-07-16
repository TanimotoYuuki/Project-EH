#include "stdafx.h"
#include "AnimationLoadContext.h"
#include "k2EngineLow.h"

#include "Src/Actor/Character/Common/CharacterAnimation.h"
#include "Src/Actor/Character/Common/CharacterModel.h"
#include "Src/Actor/Character/Boss/BossAnimation.h"

#include <unordered_set>

namespace
{
	/**
	 * @brief ボスタイプを BossAnimation::Init と同じ文字列へ変換する。
	 */
	const char* BossTypeToString(nsApp::CharacterModelType bossType)
	{
		using nsApp::CharacterModelType;

		switch (bossType)
		{
		case CharacterModelType::GrayDragon:
			return "GrayDragon";
		case CharacterModelType::GreenDragon:
			return "GreenDragon";
		case CharacterModelType::RedDragon:
			return "RedDragon";
		case CharacterModelType::TutorialBoss:
		default:
			return "TutorialBoss";
		}
	}

	/**
	 * @brief リクエストから先読み対象 .tka パス一覧を作る（重複除去）。
	 * @note  パスは CharacterAnimation / BossAnimation の登録規則をソースにする。
	 */
	std::vector<std::string> BuildPrefetchPathList(const nsApp::InGameBuildRequest& request)
	{
		std::unordered_set<std::string> uniquePaths;
		std::vector<std::string> paths;

		auto addPath = [&](const std::string& path)
			{
				if (path.empty())
					return;
				if (uniquePaths.insert(path).second)
					paths.push_back(path);
			};

		/* 4 人分の武器アニメ（基本＋攻撃）。*/
		nsApp::CharacterAnimation characterAnim;
		for (int i = 0; i < 4; ++i)
		{
			const auto weaponType = static_cast<nsApp::WeaponType>(request.characterRole[i]);
			if (weaponType == nsApp::WeaponType::None)
				continue;

			const auto weaponPaths = characterAnim.CollectAnimationFilePaths(weaponType);
			for (const auto& path : weaponPaths)
				addPath(path);
		}

		/* ボスアニメ。*/
		const auto bossType = static_cast<nsApp::CharacterModelType>(
			request.bossType + static_cast<int>(nsApp::CharacterModelType::TutorialBoss));

		nsApp::nsActor::BossAnimation bossAnim;
		const auto bossPaths = bossAnim.CollectAnimationFilePaths(BossTypeToString(bossType));
		for (const auto& path : bossPaths)
			addPath(path);

		return paths;
	}
} // namespace

namespace nsApp
{
	std::mutex AnimationLoadContext::mutex_;
	std::vector<AnimationLoadContext::PrefetchEntry> AnimationLoadContext::entries_;
	bool AnimationLoadContext::isReady_ = false;


	void AnimationLoadContext::Reset()
	{
		std::lock_guard<std::mutex> lock(mutex_);

		for (auto& entry : entries_)
		{
			if (entry.tkaFile != nullptr && !entry.isRegistered)
				delete entry.tkaFile;
			entry.tkaFile = nullptr;
		}

		entries_.clear();
		isReady_ = false;
	}


	void AnimationLoadContext::PrepareOnWorker(const InGameBuildRequest& request)
	{
		const std::vector<std::string> paths = BuildPrefetchPathList(request);
		std::vector<PrefetchEntry> loadedEntries;
		loadedEntries.reserve(paths.size());

		bool allOk = !paths.empty();

		for (const auto& path : paths)
		{
			PrefetchEntry entry;
			entry.path = path;

			auto* tka = new nsK2EngineLow::TkaFile;
			/* TkaFile::Load はディスク読み込みのみ。*/
			tka->Load(path.c_str());
			/* 失敗判定が API に無いので、パスが空でなければ成功扱い。*/
			entry.tkaFile = tka;
			entry.isRegistered = false;
			loadedEntries.push_back(entry);
		}

		std::lock_guard<std::mutex> lock(mutex_);

		for (auto& entry : entries_)
		{
			if (entry.tkaFile != nullptr && !entry.isRegistered)
				delete entry.tkaFile;
		}

		entries_ = std::move(loadedEntries);
		isReady_ = allOk;
	}


	void AnimationLoadContext::RegisterToBankOnMain()
	{
		std::lock_guard<std::mutex> lock(mutex_);

		if (!isReady_ || entries_.empty())
			return;

		for (auto& entry : entries_)
		{
			if (entry.isRegistered || entry.tkaFile == nullptr || entry.path.empty())
				continue;

			if (g_engine->GetTkaFileFromBank(entry.path.c_str()) != nullptr)
			{
				delete entry.tkaFile;
				entry.tkaFile = nullptr;
				entry.isRegistered = true;
				continue;
			}

			g_engine->RegistTkaFileToBank(entry.path.c_str(), entry.tkaFile);
			entry.isRegistered = true;
		}
	}


	bool AnimationLoadContext::IsReady()
	{
		std::lock_guard<std::mutex> lock(mutex_);
		return isReady_;
	}
} // namespace nsApp