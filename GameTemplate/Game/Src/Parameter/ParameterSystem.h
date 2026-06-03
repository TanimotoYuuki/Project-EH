#pragma once
/** 
 * @file   ParameterSystem.h
 * @brief  パラメータシステムクラスの宣言
 * @author Yamaguchi Hayato	
 */
#include <string>

namespace nsApp
{
	class ParameterSystem
	{
	public:
		/* コンストラクタとデストラクタ。*/
		ParameterSystem() = default;
		~ParameterSystem() = default;


	public:
		/**
		 * @brief 全てのパラメーターを読み込む。
		 * @return　全てのパラメーターの読み込みに成功したか。
		 */
		bool LoadAll();

		/**
		 * @brief 全てのパラメーターテーブルを読み込む。
		 * @return　全てのパラメーターテーブルの読み込みに成功したか。
		 */
		bool LoadAllParameterTables();


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
