#pragma once
#include "GameObjectPool.h"
/**
 * @file GameObjectPool.inl
 * @brief GameObjectPoolクラスのインライン関数定義。
 * @author Yamaguchi Hayato
 * @date 2026/05/29
 */

namespace
{
	const auto LOOP_INIT_VALUE = 0; //! ループの初期値
}

namespace nsApp
{
    template<class PoolObject>
    void GameObjectPool<PoolObject>::Initialize(int poolSize)
    {
        /* プール用の領域を事前に確保する。*/
        m_objects.reserve(poolSize);

        /* プールのサイズ分、オブジェクトを生成する。*/
        for (int i = LOOP_INIT_VALUE; i < poolSize; ++i)
        {
            /* プール対象のオブジェクトを生成する。*/
            PoolObject* object = NewGO<PoolObject>(0);

			/* オブジェクトが存在しない場合は次へ。*/
            if (object == nullptr)
                continue;

            /* 初期状態では未使用状態にする。*/
            object->OnRelease();

            /* プールに追加する。*/
            m_objects.push_back(object);
        }
    }


    template<class PoolObject>
    PoolObject* GameObjectPool<PoolObject>::Spawn()
    {
		/* プール内のオブジェクトを順番に確認する。*/
        for (auto* object : m_objects)
        {
            /* オブジェクトが存在しない場合は次へ。*/
            if (object == nullptr)
                continue;

            /* 未使用のオブジェクトを取得する。*/
            if (!object->IsActive())
            {
				/* オブジェクトを使用状態にする。*/
                object->OnAcquire();
                return object;
            }
        }

        return nullptr;
    }


    template<class PoolObject>
    void GameObjectPool<PoolObject>::Despawn(PoolObject* object)
    {
        /* オブジェクトが存在しない場合は処理しない。*/
        if (object == nullptr)
            return;

        /* オブジェクトを未使用状態に戻す。*/
        object->OnRelease();
    }
}