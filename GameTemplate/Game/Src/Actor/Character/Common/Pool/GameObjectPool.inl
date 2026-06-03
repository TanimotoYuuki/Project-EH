#pragma once
#include "GameObjectPool.h"

namespace nsApp
{
    template<class PoolObject>
    void GameObjectPool<PoolObject>::Initialize(int poolSize)
    {
        /* プール用の領域を事前に確保する。*/
        m_objects.reserve(poolSize);

        /* プールのサイズ分、オブジェクトを生成する。*/
        for (int i = 0; i < poolSize; ++i)
        {
            /* プール対象のオブジェクトを生成する。*/
            PoolObject* object = NewGO<PoolObject>(0);

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
        for (auto* object : m_objects)
        {
            /* オブジェクトが存在しない場合は次へ。*/
            if (object == nullptr)
                continue;

            /* 未使用のオブジェクトを取得する。*/
            if (!object->IsActive())
            {
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