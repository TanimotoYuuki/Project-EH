#include "stdafx.h"
#include "StageLoadContext.h"
#include "k2EngineLow.h"

namespace
{
	/**
	 * @brief ステージIDからモデルパスを解決する。
	 * @param stageID ステージID。
	 * @return モデルパス。未対応なら空文字。
	 */
	std::string ResolveStageModelPath(nsApp::nsStage::StageID stageID)
	{
		using nsApp::nsStage::StageID;

		/* LoadStageData::InitStage1 と同じパスを使用する。*/
		if (stageID == StageID::stage1)
			return "Assets/modelData/stage/seaStage.tkm";

		return {};
	}
} // namespace

namespace nsApp
{
	namespace nsStage
	{
		std::mutex StageLoadContext::mutex_;
		nsK2EngineLow::TkmFile* StageLoadContext::tkmFile_ = nullptr;
		std::string StageLoadContext::modelPath_;
		bool StageLoadContext::isReady_ = false;
		bool StageLoadContext::isRegistered_ = false;


		void StageLoadContext::Reset()
		{
			std::lock_guard<std::mutex> lock(mutex_);

			/* バンク未登録ならこちらで破棄する。登録済みはバンクが所有する。*/
			if (tkmFile_ != nullptr && !isRegistered_)
				delete tkmFile_;

			tkmFile_ = nullptr;
			modelPath_.clear();
			isReady_ = false;
			isRegistered_ = false;
		}


		void StageLoadContext::PrepareOnWorker(StageID stageID)
		{
			/* ステージIDからモデルパスを解決する。*/
			std::string path = ResolveStageModelPath(stageID);
			nsK2EngineLow::TkmFile* tkm = nullptr;
			bool ok = false;

			/* パスが有効なら TkmFile を Load する（GPU 作成は Model::Init 側）。*/
			if (!path.empty())
			{
				tkm = new nsK2EngineLow::TkmFile;
				/* isOptimize=false, isLoadTexture=true（DDS バイトまで読み込む）。*/
				ok = tkm->Load(path.c_str(), false, true);
				if (!ok)
				{
					delete tkm;
					tkm = nullptr;
				}
			}

			/* 結果を共有領域へ書き込む。*/
			std::lock_guard<std::mutex> lock(mutex_);

			/* 前回の未登録分があれば破棄する。*/
			if (tkmFile_ != nullptr && !isRegistered_)
				delete tkmFile_;

			tkmFile_ = tkm;
			modelPath_ = std::move(path);
			isReady_ = (tkmFile_ != nullptr);
			isRegistered_ = false;
		}


		void StageLoadContext::RegisterToBankOnMain()
		{
			std::lock_guard<std::mutex> lock(mutex_);

			/* 先読み未完了、または既に登録済みなら何もしない。*/
			if (!isReady_ || tkmFile_ == nullptr || modelPath_.empty() || isRegistered_)
				return;

			/* 既に同パスがバンクにある場合は、新規分を破棄して終了する（リーク防止）。*/
			if (g_engine->GetTkmFileFromBank(modelPath_.c_str()) != nullptr)
			{
				delete tkmFile_;
				tkmFile_ = nullptr;
				isReady_ = true;
				isRegistered_ = true;
				return;
			}

			/* メインスレッドでバンクへ登録する。所有権はバンク側へ移る。*/
			g_engine->RegistTkmFileToBank(modelPath_.c_str(), tkmFile_);
			isRegistered_ = true;
		}


		bool StageLoadContext::IsReady()
		{
			std::lock_guard<std::mutex> lock(mutex_);
			return isReady_;
		}


		std::string StageLoadContext::GetModelPath()
		{
			std::lock_guard<std::mutex> lock(mutex_);
			return modelPath_;
		}
	} // namespace nsStage
} // namespace nsApp