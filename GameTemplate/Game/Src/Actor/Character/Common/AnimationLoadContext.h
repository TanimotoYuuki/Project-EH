#pragma once

/**
 * @file   AnimationLoadContext.h
 * @brief  キャラ／ボスアニメ（.tka）のワーカー先読み結果をメインスレッドへ渡す。
 * @note   Worker で TkaFile::Load、Main でバンク登録する。
 *         AnimationClip::Load は従来どおり（バンク命中で再読込しない）。
 */

#include <mutex>
#include <string>
#include <vector>

#include "Src/Build/InGameBuildHelper.h"

namespace nsK2EngineLow
{
	class TkaFile;
}

namespace nsApp
{
	/**
	 * @class AnimationLoadContext
	 * @brief InGame 構築前に Worker で .tka を先読みする静的コンテキスト。
	 */
	class AnimationLoadContext
	{
	public:
		/**
		 * @brief 先読み結果を破棄する。
		 * @note  ロード開始時に呼ぶ。バンク未登録の TkaFile のみ delete する。
		 */
		static void Reset();

		/**
		 * @brief ワーカースレッドでアニメーションを先読みする。
		 * @param request InGame 構築リクエスト。
		 * @note  TkaFile::Load まで行い、バンク登録はしない（Main 側で行う）。
		 */
		static void PrepareOnWorker(const InGameBuildRequest& request);

		/**
		 * @brief 先読みした TkaFile をリソースバンクへ登録する。
		 * @note  メインスレッド（FinalizeOnMainThread）から呼ぶ。
		 */
		static void RegisterToBankOnMain();

		/**
		 * @brief 先読みが成功したか。
		 * @return 対象パスをすべて Load できていれば true。
		 */
		static bool IsReady();


	private:
		/**
		 * @struct PrefetchEntry
		 * @brief  1 ファイル分の先読みエントリ。
		 */
		struct PrefetchEntry
		{
			std::string path;								//! アニメーションファイルパス。
			nsK2EngineLow::TkaFile* tkaFile = nullptr;		//! Worker が Load した TkaFile。登録後はバンクが所有。
			bool isRegistered = false;						//! バンク登録済みフラグ。
		};

		static std::mutex mutex_;
		static std::vector<PrefetchEntry> entries_;
		static bool isReady_;
	};
} // namespace nsApp