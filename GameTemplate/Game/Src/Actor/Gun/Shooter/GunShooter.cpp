#include "stdafx.h"
#include "GunShooter.h"
#include "Src/Actor/Gun/Factory/BulletFactory.h"

namespace nsApp
{
	void GunShooter::Subscribe(IBulletFireListener* listener)
	{
		if (listener)
			m_listeners.push_back(listener);
	}

	void GunShooter::Fire(const BulletFireRequest& request)
	{
		/* ’eŠÛ‚Ì¶¬B*/
		BulletFactory::CreateBullet(request.kind, request.basePosition, request.direction);

		// “o˜^‚³‚ê‚Ä‚¢‚éƒŠƒXƒi[‚É’Ê’mB
		for (auto* listener : m_listeners)
			listener->OnBulletFired(request);
	}
}