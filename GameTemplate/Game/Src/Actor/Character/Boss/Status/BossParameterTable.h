#pragma once
/**
 * @file   BossParameterTable.h
 * @brief  TSVファイルからボスパラメータを読み込み管理するクラス。
 */

#include "BossManager.h"
#include <unordered_map>
#include "Src/Actor/Character/Common/CharacterModel.h"
#include <string>

namespace nsApp
{
    class TSVTable;

    namespace nsAI
    {
        /**
         * @class BossParameterTable
         * @brief BossType.tsv を読み込み、ボスタイプごとのパラメータを提供する。
         */
        class BossParameterTable
        {
        public:
            /**
             * @brief TSVファイルを読み込む。
             * @param filePath TSVファイルのパス。
             * @return 読み込み成功でtrue。
             */
            static bool LoadTSVFile(const char* filePath);

            /**
             * @brief ボスタイプに対応するパラメータを返す。
             * @param bossType ボスタイプ。
             * @return ボスパラメータの参照。
             */
            static const BossTypeParameters& GetParameters(CharacterModelType bossType);


        private:
            /**
			 * @brief TSVの文字列を対応する列挙型や構造体に変換するヘルパー関数。
			 * @param name 変換元の文字列。
			 * @return 変換結果。CharacterModelType 以外は、変換に失敗した場合のデフォルト値を返す。
             */
            static BossPersonality   ConvertPersonality(const std::string& name);

            /**
			 * @brief TSVの文字列を CharacterModelType に変換するヘルパー関数。
			 * @param name 変換元の文字列。
			 * @param outType 変換結果を格納する参照。変換に成功した場合は対応する CharacterModelType を、失敗した場合は CharacterModelType::TutorialBoss をセットする。
			 * @return 変換成功でtrue、失敗でfalse。失敗した場合は outType に CharacterModelType::TutorialBoss がセットされる。
             */
            static bool ConvertBossType(const std::string& name, CharacterModelType& outType);

            /**
			 * @brief TSVの行から BossTypeParameters を生成するヘルパー関数。
			 * @param table 読み込んだTSVテーブル。
			 * @param rowIndex BossTypeParameters を生成する行のインデックス。行が存在しない場合は、関数内でエラーハンドリングを行い、デフォルト値を返す。
			 * @return 生成された BossTypeParameters。行が存在しない場合は、関数内でエラーハンドリングを行い、デフォルト値を返す。
             */
            static BossTypeParameters CreateParameterFromRow(const TSVTable& table, int rowIndex);

            /**
			 * @brief TSVの内容をもとに、ボスタイプごとのパラメータテーブルを生成するヘルパー関数。
			 * @param table 読み込んだTSVテーブル。テーブルの内容が不正な場合は、関数内でエラーハンドリングを行い、空のテーブルを生成する。
             */
            static void CreateDefaultTable(std::unordered_map<CharacterModelType, BossTypeParameters>& table);


        private:
			static std::unordered_map<CharacterModelType, BossTypeParameters> m_table; //! ボスタイプごとのパラメータテーブル
        };
    }
}