#pragma once

/**
 * @file   StageLoadContext.h
 * @brief  ステージモデルのワーカー先読み結果をメインスレッドへ渡す。
 * @note   第2段は Worker で TkmFile::Load、Main でバンク登録する。
 *         Initialize は従来どおりパス指定のまま（Model::Init がバンク命中）。
 */

#include <mutex>
#include <string>

#include "Src/Actor/Stage/StageData.h"

namespace nsK2EngineLow
{
	class TkmFile;
}

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
			 *        バンク未登録の TkmFile のみ delete する。
			 */
			static void Reset();

			/**
			 * @brief ワーカースレッドでステージモデルを先読みする。
			 * @param stageID 先読み対象のステージID。
			 * @note  TkmFile::Load まで行い、バンク登録はしない（Main 側で行う）。
			 */
			static void PrepareOnWorker(StageID stageID);

			/**
			 * @brief 先読みした TkmFile をリソースバンクへ登録する。
			 * @note  メインスレッド（FinalizeOnMainThread）から呼ぶ。
			 *        登録後の所有権はバンク側になる。
			 */
			static void RegisterToBankOnMain();

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


		private:
			static std::mutex mutex_;					//! 共有データの排他制御。
			static nsK2EngineLow::TkmFile* tkmFile_;	//! Worker が Load した TkmFile。登録後はバンクが所有。
			static std::string modelPath_;				//! 先読みしたモデルパス。
			static bool isReady_;						//! 先読み成功フラグ。
			static bool isRegistered_;					//! バンク登録済みフラグ（Reset 時の二重解放防止）。
		};
	} // namespace nsStage
} // namespace nsApp