#pragma once

namespace nsApp
{
	namespace nsStage
	{
		/* ステージIDを管理する列挙型。*/
		enum class StageID : uint8_t
		{
			stage1,
			stage2,
			stageNum,
			Invalid
		};

		/* ステージオブジェクトのデータ。*/ 
		struct StageObjectConfig
		{
			std::string modelFilePath;               // モデルのファイル名。
			Vector3 position;                        // 位置。
			Vector3 scale;                           // 拡大率。
			Quaternion angle;                        // 角度。
		};

		/* ステージのデータ。*/ 
		struct StageData
		{
			StageID stageID;
			std::string stageName;                   // ステージ名。
			std::string stageBackGroundModel;        // ステージ背景モデルのファイルパス。
			Vector3 playerInitPosition;              // プレイヤーの初期位置。
		};
	}
}
