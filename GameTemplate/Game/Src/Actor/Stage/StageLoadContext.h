#pragma once

/**
 * @file   StageLoadContext.h
 * @brief  ステージモデルのワーカー先読み結果をメインスレッドへ渡す。
 * @note   第1段はファイルバイナリの先読みのみ。Initialize は従来どおりパスから行う。
 */

#include <mutex>
#include <string>
#include <vector>

#include "Src/Actor/Stage/StageData.h"

namespace nsApp
{
	namespace nsStage
	{
		/**
		 * @class StageLoadContext
		 * @brief ステージ遷移／構築前に Worker でモデルを先読みする静的コンテキスト。
		 */
		class StageLoadContext
		{
		public:
			/**
			 * @brief 先読み結果を破棄する。
			 * @note  ロード開始時に呼ぶ。
			 */
			static void Reset();

			/**
			 * @brief ワーカースレッドでステージモデルを先読みする。
			 * @param stageID 先読み対象のステージID。
			 */
			static void PrepareOnWorker(StageID stageID);

			/**
			 * @brief 先読みが成功したか。
			 * @return 成功していれば true。
			 */
			static bool IsReady();

			/**
			 * @brief 先読みしたモデルパスを取得する。
			 * @return モデルファイルパス。
			 */
			static std::string GetModelPath();

			/**
			 * @brief 先読みしたバイナリを取得する。
			 * @return モデルバイナリ（第1段では参照用。Initialize では未使用）。
			 */
			static const std::vector<char>& GetModelBuffer();


		private:
			static std::mutex mutex_;					//! 共有データの排他制御。
			static std::vector<char> modelBuffer_;		//! 先読みしたモデルバイナリ。
			static std::string modelPath_;				//! 先読みしたモデルパス。
			static bool isReady_;						//! 先読み成功フラグ。
		};
	} // namespace nsStage
} // namespace nsApp