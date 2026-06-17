/*!
* @brief	�L�����N�^�̃R���W�����R���g���[���B
*/

#include "k2EngineLowPreCompile.h"
#include "CharacterController.h"

namespace nsK2EngineLow {
	namespace {
		void Vector3CopyFrom(Vector3& vDst, btVector3& vSrc)
		{
			vDst.x = vSrc.x();
			vDst.y = vSrc.y();
			vDst.z = vSrc.z();
		}
		//�Փ˂����Ƃ��ɌĂ΂��֐��I�u�W�F�N�g(�n�ʗp)
		struct SweepResultGround : public btCollisionWorld::ConvexResultCallback
		{
			bool isHit = false;									//�Փ˃t���O�B
			Vector3 hitPos = Vector3(0.0f, -FLT_MAX, 0.0f);		//�Փ˓_�B
			Vector3 startPos;									//���C�̎n�_�B
			Vector3 hitNormal;									//�Փ˓_�̖@���B
			btCollisionObject* me = nullptr;					//�������g�B�������g�Ƃ̏Փ˂����O���邽�߂̃����o�B
			float dist = FLT_MAX;								//�Փ˓_�܂ł̋����B��ԋ߂��Փ˓_�����߂邽�߁BFLT_MAX�͒P���x�̕��������_����肤��ő�̒l�B

																//�Փ˂����Ƃ��ɌĂ΂��R�[���o�b�N�֐��B
			virtual	btScalar	addSingleResult(btCollisionWorld::LocalConvexResult& convexResult, bool normalInWorldSpace)
			{
				if (convexResult.m_hitCollisionObject == me
					|| convexResult.m_hitCollisionObject->getUserIndex() == enCollisionAttr_Character
					|| convexResult.m_hitCollisionObject->getInternalType() == btCollisionObject::CO_GHOST_OBJECT
					|| !me->checkCollideWith(convexResult.m_hitCollisionObject)
					) {
					//�����ɏՓ˂����Bor �L�����N�^�����̃R���W�����ƏՓ˂����B
					return 0.0f;
				}
				//�Փ˓_�̖@�������������Ă���B
				Vector3 hitNormalTmp = *(Vector3*)&convexResult.m_hitNormalLocal;
				//������Ɩ@���̂Ȃ��p�x�����߂�B
				float angle = hitNormalTmp.y;
				angle = fabsf(acosf(angle));
				if (angle < Math::PI * 0.3f		//�n�ʂ̌X�΂�54�x��菬�����̂Œn�ʂƂ݂Ȃ��B
					|| convexResult.m_hitCollisionObject->getUserIndex() == enCollisionAttr_Ground //�������̓R���W�����������n�ʂƎw�肳��Ă���B
					) {
					//�Փ˂��Ă���B
					isHit = true;
					Vector3 hitPosTmp = *(Vector3*)&convexResult.m_hitPointLocal;
					//�Փ˓_�̋��������߂�B�B
					Vector3 vDist;
					vDist.Subtract(hitPosTmp, startPos);
					float distTmp = vDist.Length();
					if (dist > distTmp) {
						//���̏Փ˓_�̕����߂��̂ŁA�ŋߖT�̏Փ˓_���X�V����B
						hitPos = hitPosTmp;
						hitNormal = *(Vector3*)&convexResult.m_hitNormalLocal;
						dist = distTmp;
					}
				}
				return 0.0f;
			}
		};

		//�Փ˂����Ƃ��ɌĂ΂��֐��I�u�W�F�N�g(�Ǘp)
		struct SweepResultWall : public btCollisionWorld::ConvexResultCallback
		{
			bool isHit = false;						//�Փ˃t���O�B
			Vector3 hitPos;							//�Փ˓_�B
			Vector3 startPos;						//���C�̎n�_�B
			float dist = FLT_MAX;					//�Փ˓_�܂ł̋����B��ԋ߂��Փ˓_�����߂邽�߁BFLT_MAX�͒P���x�̕��������_����肤��ő�̒l�B
			Vector3 hitNormal;						//�Փ˓_�̖@���B
			btCollisionObject* me = nullptr;		//�������g�B�������g�Ƃ̏Փ˂����O���邽�߂̃����o�B
													//�Փ˂����Ƃ��ɌĂ΂��R�[���o�b�N�֐��B
			virtual	btScalar	addSingleResult(btCollisionWorld::LocalConvexResult& convexResult, bool normalInWorldSpace)
			{
				if (convexResult.m_hitCollisionObject == me
					|| convexResult.m_hitCollisionObject->getInternalType() == btCollisionObject::CO_GHOST_OBJECT
					|| !me->checkCollideWith(convexResult.m_hitCollisionObject)
					) {
					//�����ɏՓ˂����Bor �n�ʂɏՓ˂����B
					return 0.0f;
				}
				//�Փ˓_�̖@�������������Ă���B
				Vector3 hitNormalTmp;
				Vector3CopyFrom(hitNormalTmp, convexResult.m_hitNormalLocal);

				//������ƏՓ˓_�̖@���̂Ȃ��p�x�����߂�B
				float angle = fabsf(acosf(hitNormalTmp.y));
				if (angle >= Math::PI * 0.3f		//�n�ʂ̌X�΂�54�x�ȏ�Ȃ̂ŕǂƂ݂Ȃ��B
					|| convexResult.m_hitCollisionObject->getUserIndex() == enCollisionAttr_Character	//�������̓R���W�����������L�����N�^�Ȃ̂ŕǂƂ݂Ȃ��B
					) {
					isHit = true;
					Vector3 hitPosTmp;
					Vector3CopyFrom(hitPosTmp, convexResult.m_hitPointLocal);

					//��_�Ƃ̋����𒲂ׂ�B
					Vector3 vDist;
					vDist.Subtract(hitPosTmp, startPos);
					vDist.y = 0.0f;
					float distTmp = vDist.Length();
					if (distTmp < dist) {
						//���̏Փ˓_�̕����߂��̂ŁA�ŋߖT�̏Փ˓_���X�V����B
						hitPos = hitPosTmp;
						dist = distTmp;
						hitNormal = hitNormalTmp;
					}
				}
				return 0.0f;
			}
		};
	}


	void CharacterController::Init(float radius, float height, const Vector3& position)
	{
		m_position = position;
		//�R���W�����쐬�B
		m_radius = radius;
		m_height = height;
		m_collider.Init(radius, height);

		//���̂��������B
		RigidBodyInitData rbInfo;
		rbInfo.collider = &m_collider;
		rbInfo.mass = 0.0f;
		m_rigidBody.Init(rbInfo);
		btTransform& trans = m_rigidBody.GetBody()->getWorldTransform();
		//���̂̈ʒu���X�V�B
		trans.setOrigin(btVector3(position.x, position.y + m_height * 0.5f + m_radius, position.z));
		//@todo ���Ή��Btrans.setRotation(btQuaternion(rotation.x, rotation.y, rotation.z));
		m_rigidBody.GetBody()->setUserIndex(enCollisionAttr_Character);
		m_rigidBody.GetBody()->setCollisionFlags(btCollisionObject::CF_CHARACTER_OBJECT);

		m_isInited = true;
	}
	const Vector3& CharacterController::Execute(Vector3& moveSpeed, float deltaTime)
	{
		if (moveSpeed.y > 0.0f) {
			//������ђ��ɂ���B
			m_isJump = true;
			m_isOnGround = false;
		}
		//���̈ړ���ƂȂ���W���v�Z����B
		Vector3 nextPosition = m_position;
		//���x���炱�̃t���[���ł̈ړ��ʂ����߂�B�I�C���[�ϕ��B
		Vector3 addPos = moveSpeed;
		addPos.Scale(deltaTime);
		nextPosition.Add(addPos);
		Vector3 originalXZDir = addPos;
		originalXZDir.y = 0.0f;
		originalXZDir.Normalize();
		//XZ���ʂł̏Փˌ��o�ƏՓˉ������s���B
		{
			int loopCount = 0;
			while (true) {
				//���݂̍��W���玟�̈ړ���֌������x�N�g�������߂�B
				Vector3 addPos;
				addPos.Subtract(nextPosition, m_position);
				Vector3 addPosXZ = addPos;
				addPosXZ.y = 0.0f;
				if (addPosXZ.Length() < FLT_EPSILON) {
					//XZ���ʂœ������Ȃ��̂Œ��ׂ�K�v�Ȃ��B
					//FLT_EPSILON��1���傫���A�ŏ��̒l�Ƃ̍�����\���萔�B
					//�ƂĂ��������l�̂��Ƃł��B
					break;
				}
				//�J�v�Z���R���C�_�[�̒��S���W + ����*0.1�̍��W��posTmp�ɋ��߂�B
				Vector3 posTmp = m_position;
				posTmp.y += m_height * 0.5f + m_radius + m_height * 0.1f;
				//���C���쐬�B
				btTransform start, end;
				start.setIdentity();
				end.setIdentity();
				//�n�_�̓J�v�Z���R���C�_�[�̒��S���W + 0.2�̍��W��posTmp�ɋ��߂�B
				start.setOrigin(btVector3(posTmp.x, posTmp.y, posTmp.z));
				//�I�_�͎��̈ړ���BXZ���ʂł̏Փ˂𒲂ׂ�̂ŁAy��posTmp.y��ݒ肷��B
				end.setOrigin(btVector3(nextPosition.x, posTmp.y, nextPosition.z));

				SweepResultWall callback;
				callback.me = m_rigidBody.GetBody();
				callback.startPos = posTmp;
				//�Փˌ��o�B
				PhysicsWorld::GetInstance()->ConvexSweepTest((const btConvexShape*)m_collider.GetBody(), start, end, callback);

				if (callback.isHit) {
					//���������B
					//�ǁB
					Vector3 vT0, vT1;
					//XZ���ʏ�ł̈ړ���̍��W��vT0�ɁA��_�̍��W��vT1�ɐݒ肷��B
					vT0.Set(nextPosition.x, 0.0f, nextPosition.z);
					vT1.Set(callback.hitPos.x, 0.0f, callback.hitPos.z);
					//�߂荞�݂��������Ă���ړ��x�N�g�������߂�B
					Vector3 vMerikomi;
					vMerikomi.Subtract(vT0, vT1);
					//XZ���ʂł̏Փ˂����ǂ̖@�������߂�B�B
					Vector3 hitNormalXZ = callback.hitNormal;
					hitNormalXZ.y = 0.0f;
					hitNormalXZ.Normalize();
					//�߂荞�݃x�N�g����ǂ̖@���Ɏˉe����B
					float fT0 = hitNormalXZ.Dot(vMerikomi);
					//�����߂��Ԃ��x�N�g�������߂�B
					//�����Ԃ��x�N�g���͕ǂ̖@���Ɏˉe���ꂽ�߂荞�݃x�N�g��+���a�B
					Vector3 vOffset;
					vOffset = hitNormalXZ;
					vOffset.Scale(-fT0 + m_radius);
					nextPosition.Add(vOffset);

					Vector3 currentDir;
					currentDir.Subtract(nextPosition, m_position);
					currentDir.y = 0.0f;
					currentDir.Normalize();
					if (currentDir.Dot(originalXZDir) < 0.0f) {
						//�p�ɓ��������̃L�����N�^�̐U����h�~���邽�߂ɁA
						//�ړ��悪�t�����ɂȂ�����ړ����L�����Z������B
						nextPosition.x = m_position.x;
						nextPosition.z = m_position.z;
						break;
					}
				}
				else {
					//�ǂ��Ƃ�������Ȃ��̂ŏI���B
					break;
				}
				loopCount++;
				if (loopCount == 5) {
					break;
				}
			}
		}
		//XZ�̈ړ��͊m��B
		m_position.x = nextPosition.x;
		m_position.z = nextPosition.z;
		//�������𒲂ׂ�B
		{
			Vector3 addPos;
			addPos.Subtract(nextPosition, m_position);

			m_position = nextPosition;	//�ړ��̉��m��B
										//���C���쐬����B
			btTransform start, end;
			start.setIdentity();
			end.setIdentity();
			//�n�_�̓J�v�Z���R���C�_�[�̒��S�B
			start.setOrigin(btVector3(m_position.x, m_position.y + m_height * 0.5f + m_radius, m_position.z));
			//�I�_�͒n�ʏ�ɂ��Ȃ��ꍇ��1m��������B
			//�n�ʏ�ɂ��Ȃ��ăW�����v�ŏ㏸���̏ꍇ�͏㏸�ʂ�0.01�{��������B
			//�n�ʏ�ɂ��Ȃ��č~�����̏ꍇ�͂��̂܂ܗ�����𒲂ׂ�B
			Vector3 endPos;
			Vector3CopyFrom(endPos, start.getOrigin());

			if (m_isOnGround == false) {
				if (addPos.y > 0.0f) {
					//�W�����v���Ƃ��ŏ㏸���B
					//�㏸���ł�XZ�Ɉړ��������ʂ߂荞��ł���\��������̂ŉ��𒲂ׂ�B
					endPos.y -= addPos.y * 0.01f;
				}
				else {
					//�������Ă���ꍇ�͂��̂܂܉��𒲂ׂ�B
					endPos.y += addPos.y;
				}
			}
			else {
				//�n�ʏ�ɂ��Ȃ��ꍇ��1m��������B
				endPos.y -= 100.0f;
			}
			end.setOrigin(btVector3(endPos.x, endPos.y, endPos.z));
			SweepResultGround callback;
			callback.me = m_rigidBody.GetBody();
			Vector3CopyFrom(callback.startPos, start.getOrigin());

			//�Փˌ��o�B
			if (fabsf(endPos.y - callback.startPos.y) > FLT_EPSILON) {
				PhysicsWorld::GetInstance()->ConvexSweepTest((const btConvexShape*)m_collider.GetBody(), start, end, callback);
				if (callback.isHit) {
					//���������B
					moveSpeed.y = 0.0f;
					m_isJump = false;
					m_isOnGround = true;
					nextPosition.y = callback.hitPos.y;
				}
				else {
					//�n�ʏ�ɂ��Ȃ��B
					m_isOnGround = false;

				}
			}
		}
		//�ړ��m��B
		m_position = nextPosition;
		btRigidBody* btBody = m_rigidBody.GetBody();
		//���̂𓮂����B
		btBody->setActivationState(DISABLE_DEACTIVATION);
		btTransform& trans = btBody->getWorldTransform();
		//���̂̈ʒu���X�V�B
		trans.setOrigin(btVector3(m_position.x, m_position.y + m_height * 0.5f + m_radius, m_position.z));
		//@todo ���Ή��B trans.setRotation(btQuaternion(rotation.x, rotation.y, rotation.z));
		return m_position;
	}
	/*!
	* @brief	���S�������Ƃ�ʒm�B
	*/
	void CharacterController::RemoveRigidBoby()
	{
		PhysicsWorld::GetInstance()->RemoveRigidBody(m_rigidBody);
	}
}
