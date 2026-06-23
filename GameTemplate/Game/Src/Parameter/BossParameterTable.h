#pragma once
/**
 * @file   BossParameterTable.h
 * @brief  TSVファイルからボスパラメータを読み込み管理するクラス。
 */

#include "Src/Actor/Character/Boss/Status/BossManager.h"
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
            static BossPersonality   ConvertPersonality(const std::string& name);
            static bool              ConvertBossType(const std::string& name, CharacterModelType& outType);
            static BossTypeParameters CreateParameterFromRow(const TSVTable& table, int rowIndex);
            static void              CreateDefaultTable(
                std::unordered_map<CharacterModelType, BossTypeParameters>& table);


        private:
            static std::unordered_map<CharacterModelType, BossTypeParameters> m_table;
        };
    }
}