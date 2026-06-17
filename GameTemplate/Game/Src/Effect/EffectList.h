#pragma once
/**
 * @file   EffectList.h
 * @brief  �G�t�F�N�g���Ǘ����郊�X�g�N���X�B
 * @author YamaguchiHayato�B
 * @date   2026/03/23
 */

namespace nsApp
{
	namespace nsEffect
	{
		/*
		 * @struct EffectInfo�B
		 * @brief �Đ�����G�t�F�N�g�̏����Ǘ�����B
		 */
		struct EffectInfo
		{
			nsK2EngineLow::EffectEmitter *emitter; //! �G�t�F�N�g�̃G�~�b�^�B
			float lifeTime;						   //! �G�t�F�N�g�̎����B
			float currentTime;					   //! �G�t�F�N�g�̌��݂̌o�ߎ��ԁB
		};

		/**
		 * @enum  EffectID�B
		 * @brief �G�t�F�N�g�����ʎq�ŊǗ�����p�̗񋓌^�B
		 */
		enum Effect_ID : uint8_t
		{
			Attack,	   //! �U���G�t�F�N�g�B
			Charge,	   //! �`���[�W�G�t�F�N�g�B
			Fire,	   //! �{�X�����e�G�t�F�N�g�B
			ShockWave, //! �Ռ��g�G�t�F�N�g�B
			Hit,	   //! ��e�G�t�F�N�g�B

			NormalMagic,		//! �ʏ햂�@�G�t�F�N�g�B
			RushMagic,			//! �A�Ŗ��@�G�t�F�N�g�B
			AirMagic,			//! �󒆖��@�G�t�F�N�g�B
			HeelMagic,			//! �񕜃G�t�F�N�g�B
			HeelMagic_Particle, //! �񕜃G�t�F�N�g�̃p�[�e�B�N���B
			MagicAttack,		//! ���@�U���G�t�F�N�g�B
			Shot,				//! �e�ۃG�t�F�N�g�B

			FireBall,		//! �{�X�t�@�C�A�{�[�����ăG�t�F�N�g�B
			BossFireAttack, //! �{�X�����e�G�t�F�N�g�i�����j�B
		};

		class EffectList
		{
		public:
			/* �R���X�g���N�^�ƃf�X�g���N�^�B*/
			EffectList() = default;
			virtual ~EffectList();

		public:
			/* �G�t�F�N�g���������B*/
			void Init();

			/*
			 * @brief �G�t�F�N�g���X�V����B
			 * @param deltaTime: �O�t���[������̌o�ߎ��ԁB
			 */
			void Update(float deltaTime);

			/*
			 * @brief�@�G�t�F�N�g�̃L���b�V�����������B
			 */
			void Clear();

			/*
			 * @brief �w�肵���G�t�F�N�g���~/�폜����B
			 * @param effect: ��~/�폜����G�t�F�N�g�̃C���X�^���X�B
			 */
			void StopEffect(nsK2EngineLow::EffectEmitter *effect);

			/*
			 * @def �G�t�F�N�g���Đ��B
			 * @param id: �G�t�F�N�g�̎��ʎq�B
			 * @param position: �G�t�F�N�g�̏o���ʒu�B
			 * @param angle: �G�t�F�N�g�̉�]�p�x�B
			 * @param scale: �G�t�F�N�g�̊g�嗦�B
			 * @param lifeTime: �G�t�F�N�g�����b�`�悷�邩�B
			 */
			nsK2EngineLow::EffectEmitter *PlayEffect(Effect_ID id, const Vector3 &position, const Quaternion &angle = Quaternion::Identity, const Vector3 &scale = Vector3::One, float lifeTime = 2.0f);

		public:
			/*
			 * @def �t�@�C���p�X��o�^�B
			 * @pararm name: �G�t�F�N�g�̃t�@�C�����B
			 */
			inline const std::u16string GetEffectFilePath(const std::u16string name)
			{
				const std::u16string path = u"Assets/effect/" + name + u".efk";
				return path;
			}

		private:
			/* ���킲�ƂɃG�t�F�N�g��o�^�����𕪂���B*/
			/* Sword�B*/
			void StorageGreatSwordEffect();

			/* Hammer�B*/
			void StorageHammerEffect();

			/* Wand�B*/
			void StorageWandEffect();

			/* TwinGun�B*/
			void StorageTwinGunEffect();

			/* Boss�B*/
			void StorageBossEffect();

		private:
			nsK2EngineLow::EffectEmitter *m_effectEmitter; //! �G�t�F�N�g�̃G�~�b�^�̃C���X�^���X�B

		private:
			std::unordered_map<Effect_ID, std::u16string> m_effectPathList; //! �G�t�F�N�g�̎��ʎq�ƃt�@�C���p�X���Ǘ�����}�b�v�B

			std::vector<EffectInfo> m_playingEffects; //! ���ݍĐ����̃G�t�F�N�g�̏����Ǘ����郊�X�g�B

			EffectInfo m_info; //! �G�t�F�N�g�̏����Ǘ�����C���X�^���X�B
		};
	}
}
