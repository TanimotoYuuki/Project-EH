#pragma once

/**
 * @file   Camera.h
 * @brief  ��ʂ��Ƃ�Strategy�Ő؂�ւ��J�����B
 * @author Yamaguchi Hayato
 * @date   2026/6/09: ���t�@�N�^�����O�B
 */

#include <memory>
#include "Src/Camera/ICameraStrategy.h"
#include "Src/Camera/LoadingCameraStrategy.h"


namespace nsApp
{
	/* �z�C���N���[�h����̂��ߑO���錾���g�p����B*/
	class BattleCameraStrategy;


	/**
	 * @class Camera
	 * @brief Strategy�p�^�[���ŏ�ʂ��ƂɃJ������؂�ւ���N���X�B
	 */
	class Camera : public IGameObject
	{
	public:
		/* �R���X�g���N�^�ƃf�X�g���N�^�B*/
		Camera() = default;
		virtual ~Camera() = default;


	public:
		/**
		 * @brief �J�n�����B
		 * @return ����ɏ��������ꂽ�ꍇ true�B
		 */
		bool Start();

		/**
		 * @brief �X�V�����B
		 */
		void Update();

		/**
		 * @brief ���[�f�B���O�p�J�����ɐ؂�ւ���B
		 */
		void ChangeToLoading();

		/**
		 * @brief �퓬�p�J�����ɐ؂�ւ���B
		 */
		void ChangeToBattle();

		/**
		 * @brief �J�����V�F�C�N���J�n����B�퓬�J�����ȊO�ł͖����B
		 * @param duration  �V�F�C�N���鎞�ԁi�b�j�B
		 * @param intensity �V�F�C�N�̋����B
		 */
		void StartShake(float duration, float intensity);


	private:
		/**
		 * @brief �J����Strategy��ύX����B
		 * @param strategy �V����Strategy�B
		 */
		void ChangeStrategy(std::unique_ptr<ICameraStrategy> strategy);


	private:
		std::unique_ptr<ICameraStrategy> m_strategy = nullptr; //! ���݂̃J����Strategy�B
		BattleCameraStrategy* m_battleCameraStrategy = nullptr; //! �퓬�J����Strategy�̐��|�C���^�i�V�F�C�N�p�j�B
	};
}