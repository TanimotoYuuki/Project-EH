#pragma once
/**
 * @file   SoundLister.h
 * @brief  BGM/SEを管理するクラス。
 * @author YamaguchiHayato
 * @date   2026/03/21
*/

#include "BGMList.h"
#include "SEList.h"


namespace nsApp
{
	namespace nsSound
	{

		class BGMList;
		class SEList;

		class SoundLister : public IGameObject
		{
		public:
			/* コンストラクタとデストラクタ。*/
			SoundLister() = default;
			~SoundLister() = default;


		public:
			bool Start();
			void Update();


		/* ゲッター。*/
		public:
			
			/**
			* @brief BGMの音量の割合の設定。
			* @param rate BGMの音量の割合。
			*/
			inline void SetBGMVolumeRate(int rate)
			{
				m_bgmVolumeRate = rate;
			}

			/**
			* @brief BGMの音量の割合の取得。
			* @return BGMの音量の割合。
			*/
			inline int GetBGMVolumeRate() const
			{
				return m_bgmVolumeRate;
			}

			/**
			* @brief SEの音量の割合の設定。
			* @param rate SEの音量の割合。
			*/
			inline void SetSEVolumeRate(int rate)
			{
				m_seVolumeRate = rate;
			}

			/**
			* @brief SEの音量の割合の取得。
			* @return SEの音量の割合。
			*/
			inline int GetSEVolumeRate() const
			{
				return m_seVolumeRate;
			}

			/**
			* @brief マスターボリュームの割合の設定。
			* @param rate マスターボリュームの割合。
			*/
			inline void SetMasterVolumeRate(int rate)
			{
				m_masterVolumeRate = rate;
			}

			/**
			* @brief マスターボリュームの割合の取得。
			* @return マスターボリュームの割合。
			*/
			inline int GetMasterVolumeRate() const
			{
				return m_masterVolumeRate;
			}

		/* ゲッター。*/
		public:
			/* BGMのリストを取得する。*/
			inline BGMList& GetBGMList()
			{
				return m_bgmList;
			}

			/* SEのリストを取得する。*/
			inline SEList& GetSEList()
			{
				return m_seList;
			}


		private:
			nsSound::BGMList m_bgmList; //! BGM担当。
			nsSound::SEList m_seList;   //! SE担当。

			int m_bgmVolumeRate = 100;    //! BGMの音量の割合。
			int m_seVolumeRate = 100;     //! SEの音量の割合。
			int m_masterVolumeRate = 100; //! マスターボリュームの割合。
		};
	}
}
