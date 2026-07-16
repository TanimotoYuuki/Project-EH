#pragma once
/**
 * @file   ParameterSystem.h
 * @brief  パラメータシステムクラスの宣言
 * @author Yamaguchi Hayato
 */
#include <string>
#include <functional>

namespace nsApp
{
	class ParameterSystem
	{
	public:
		/* コンストラクタとデストラクタ。*/
		ParameterSystem() = default;
		virtual ~ParameterSystem() = default;


	public:
		/**
		 * @brief 全てのパラメーターを読み込む。
		 * @param onProgress 進捗コールバック（0.0f ～ 1.0f）。不要なら空でよい。
		 * @return　全てのパラメーターの読み込みに成功したか。
		 */
		bool LoadAll(const std::function<void(float)>& onProgress = {});

		/**
		 * @brief 全てのパラメーターテーブルを読み込む。
		 * @param onProgress 進捗コールバック（0.0f ～ 1.0f）。不要なら空でよい。
		 * @return　全てのパラメーターテーブルの読み込みに成功したか。
		 */
		bool LoadAllParameterTables(const std::function<void(float)>& onProgress = {});


	private:
		/**
		 * @brief パラメーターのファイルパスを生成する。
		 * @param fileName ファイル名
		 * @return 生成されたファイルパス
		 */
		std::string MakeParameterPath(const char* fileName) const;


	private:
		const std::string m_parameterDirectory = "Assets/Parameter/"; //! パラメーターのディレクトリ
		bool m_isSuccess = true;									  //! パラメーターの読み込みに成功したか
	};
}