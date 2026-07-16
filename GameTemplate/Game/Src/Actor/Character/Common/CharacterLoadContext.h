#pragma once

/**
 * @file   CharacterLoadContext.h
 * @brief  キャラ／ボス／武器モデルのワーカー先読み結果をメインスレッドへ渡す。
 * @note   Worker で TkmFile::Load、Main でバンク登録する。
 *         パス解決は CharacterModel の登録表を唯一のソースにする。
 *         Player / Boss の Init(path) は従来どおり（Model::Init がバンク命中）。
 */

#include <mutex>
#include <string>
#include <vector>

#include "Src/Build/InGameBuildHelper.h"

namespace nsK2EngineLow
{
	class TkmFile;
}

namespace nsApp
{
	/**
	 * @class CharacterLoadContext
	 * @brief InGame 構築前に Worker でキャラ関連 tkm を先読みする静的コンテキスト。
	 */
	class CharacterLoadContext
	{
	public:
		/**
		 * @brief 先読み結果を破棄する。
		 * @note  ロード開始時に呼ぶ。
		 *        バンク未登録の TkmFile のみ delete する。
		 */
		static void Reset();

		/**
		 * @brief ワーカースレッドでキャラ／ボス／武器モデルを先読みする。
		 * @param request InGame 構築リクエスト（役割・ボスタイプ）。
		 * @note  TkmFile::Load まで行い、バンク登録はしない（Main 側で行う）。
		 */
		static void PrepareOnWorker(const InGameBuildRequest& request);

		/**
		 * @brief 先読みした TkmFile をリソースバンクへ登録する。
		 * @note  メインスレッド（FinalizeOnMainThread）から呼ぶ。
		 *        登録後の所有権はバンク側になる。
		 */
		static void RegisterToBankOnMain();

		/**
		 * @brief 先読みが成功したか。
		 * @return 必要なモデルをすべて Load できていれば true。
		 */
		static bool IsReady();


	private:
		/**
		 * @struct PrefetchEntry
		 * @brief  1 ファイル分の先読みエントリ。
		 */
		struct PrefetchEntry
		{
			std::string path;								//! モデルファイルパス。
			nsK2EngineLow::TkmFile* tkmFile = nullptr;		//! Worker が Load した TkmFile。登録後はバンクが所有。
			bool isRegistered = false;						//! バンク登録済みフラグ（Reset 時の二重解放防止）。
		};

		static std::mutex mutex_;							//! 共有データの排他制御。
		static std::vector<PrefetchEntry> entries_;			//! 先読みエントリ一覧。
		static bool isReady_;								//! 先読み成功フラグ。
	};
} // namespace nsApp