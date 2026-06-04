#pragma once
#include <unordered_map>
#include <string>

namespace nsApp
{
	class TSVTable;

	/* NPCの行動の種類。*/
	enum class NPCActionType
	{
		Help,   //! 助ける行動 
		Chase,  //! 追いかける行動
		Attack, //! 攻撃行動。
		Idle,   //! 待機行動。
		None	//! 行動なし
	};

	/**
	 * @struct NPCActionParameter
	 * @brief  NPCの行動に関するパラメータを格納する構造体。
	 */
	struct NPCActionParameter
	{
		float moveSpeed = 1.0f;     //! 移動速度。
		float dashSpeed = 1.0f;     //! ダッシュ速度。
		float searchRange = 900.0f; //! 対象を探索する範囲。
		float attackRange = 150.0f; //! 攻撃範囲。
		float helpRange = 3.0f;     //! 助ける対象を探索する範囲。
		float coolTime = 0.0f;      //! 行動のクールタイム。
	};


	class NPCActionParameterTable
	{
	public:
		/**
		 * @brief TSVファイルからNPCの行動パラメータを読み込む。
		 * @param filePath TSVファイルのパス。
		 * @return 読み込みが成功した場合はtrue、失敗した場合はfalse。
		 */
		static bool LoadTSVFile(const char* filePath);

		/**
		 * @brief 指定された行動タイプに対応するNPCの行動パラメータを取得する。
		 * @param actionType NPCの行動タイプ。
		 * @return 指定された行動タイプに対応するNPCの行動パラメータ。存在しない場合はデフォルトのパラメータが返される。
		 */
		static const NPCActionParameter& GetParameter(NPCActionType actionType);


	private:
		/**
		 * @brief 行動名をNPCActionTypeに変換する。
		 * @param actionName 行動名の文字列。
		 * @return 対応するNPCActionType。対応する行動が存在しない場合はNPCActionType::Noneが返される。
		 */
		static NPCActionType ConvertActionType(const std::string& actionName);

		/**
		 * @brief TSVテーブルの指定された行からNPCActionParameterを作成する。
		 * @param table TSVテーブルの参照。
		 * @param rowIndex NPCActionParameterを作成する行のインデックス。
		 * @return 
		 */
		static NPCActionParameter CreateParameterFromRow(const TSVTable& table, int rowIndex);


	private:
		static std::unordered_map<NPCActionType, NPCActionParameter> m_table; //! NPCの行動タイプとそのパラメータを格納するテーブル。
	};
}
