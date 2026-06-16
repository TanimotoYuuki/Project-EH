#pragma once

/**
 * @file   BattleCameraStrategy.h
 * @brief  �퓬���̃J�������Ǘ�����N���X�B
 * @author Yamaguchi Hayato
 * @date   2026/06/08: �N���X�쐬�B
 * @detail StrategyPattern�Ő݌v�B�J�����V�F�C�N�@�\�����B
 */

#include "Src/Camera/ICameraStrategy.h"


namespace nsApp
{
	/**
	 * @class BattleCameraStrategy
	 * @brief �퓬�V�[����p�J�����헪�B�Œ�ʒu�Ǐ]�ƃJ�����V�F�C�N���T�|�[�g����B
	 */
	class BattleCameraStrategy : public ICameraStrategy
	{
	public:
		/* �R���X�g���N�^�ƃf�X�g���N�^�B*/
		BattleCameraStrategy() = default;
		virtual ~BattleCameraStrategy() = default;


	public:
		/**
		 * @brief �X�V�����B
		 */
		void Update() override;

		/**
		 * @brief �J�����V�F�C�N���J�n����B
		 * @param duration  �V�F�C�N���鎞�ԁi�b�j�B
		 * @param intensity �V�F�C�N�̋����B
		 */
		void StartShake(float duration, float intensity);


	public:
		/**
		 * @brief �V�F�C�N�����ǂ�����Ԃ��B
		 * @return �V�F�C�N���̏ꍇ true�B
		 */
		inline bool IsShaking() const
		{
			return m_shakeDuration > 0.0f;
		}


	private:
		static constexpr float SHAKE_FREQUENCY_X = 35.0f; //! X���V�F�C�N�̎��g���B
		static constexpr float SHAKE_FREQUENCY_Y = 28.0f; //! Y���V�F�C�N�̎��g���B
		static constexpr float SHAKE_Y_RATIO = 0.6f; //! Y���V�F�C�N�̐U���䗦�B
		static constexpr float TARGET_DAMP_RATIO = 0.3f; //! �����_�ւ̃V�F�C�N�`�d���B


	private:
		float m_shakeDuration = 0.0f; //! �c��V�F�C�N���ԁi�b�j�B
		float m_shakeIntensity = 0.0f; //! �V�F�C�N�̋����B
		float m_shakeTimer = 0.0f; //! �V�F�C�N�o�ߎ��ԁisin�v�Z�p�j�B
	};
}