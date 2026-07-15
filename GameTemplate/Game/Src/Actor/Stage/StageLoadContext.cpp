#include "stdafx.h"
#include <fstream>
#include "StageLoadContext.h"

namespace
{
	/**
	 * @brief バイナリファイルを丸ごと読み込む。
	 * @param path ファイルパス。
	 * @return 読み込みデータ。失敗時は空。
	 */
	std::vector<char> ReadFileBinary(const std::string& path)
	{
		std::ifstream ifs(path, std::ios::binary | std::ios::ate);
		if (!ifs)
			return {};

		const std::streamsize size = ifs.tellg();
		if (size <= 0)
			return {};

		std::vector<char> buffer(static_cast<size_t>(size));
		ifs.seekg(0, std::ios::beg);
		ifs.read(buffer.data(), size);

		if (!ifs)
			return {};

		return buffer;
	}

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
		std::vector<char> StageLoadContext::modelBuffer_;
		std::string StageLoadContext::modelPath_;
		bool StageLoadContext::isReady_ = false;


		void StageLoadContext::Reset()
		{
			std::lock_guard<std::mutex> lock(mutex_);
			modelBuffer_.clear();
			modelPath_.clear();
			isReady_ = false;
		}


		void StageLoadContext::PrepareOnWorker(StageID stageID)
		{
			/* ステージIDからモデルパスを解決する。*/
			std::string path = ResolveStageModelPath(stageID);
			std::vector<char> buffer;

			/* パスが有効ならバイナリを読み込む。*/
			if (!path.empty())
				buffer = ReadFileBinary(path);

			/* 結果を共有領域へ書き込む。*/
			std::lock_guard<std::mutex> lock(mutex_);
			modelPath_ = std::move(path);
			modelBuffer_ = std::move(buffer);
			isReady_ = !modelBuffer_.empty();
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


		const std::vector<char>& StageLoadContext::GetModelBuffer()
		{
			std::lock_guard<std::mutex> lock(mutex_);
			return modelBuffer_;
		}
	} // namespace nsStage
} // namespace nsApp