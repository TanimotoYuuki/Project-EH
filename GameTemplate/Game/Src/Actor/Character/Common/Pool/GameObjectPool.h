#pragma once

#include <type_traits>
#include <vector>
#include "IPoolable.h"

namespace nsApp
{
    template<class PoolObject>
    class GameObjectPool final
    {
        static_assert(
            std::is_base_of<IPoolable, PoolObject>::value,
            "PoolObject must inherit IPoolable."
            );


    public:
        /* コンストラクタとデストラクタ。*/
        GameObjectPool() = default;
        virtual ~GameObjectPool() = default;

		/* コピームーブを禁止。*/
        GameObjectPool(const GameObjectPool&) = delete;
        GameObjectPool& operator=(const GameObjectPool&) = delete;


    public:
        /**
		 * @brief プールを初期化する。
		 * @param poolSize プールのサイズ。
         */
        void Initialize(int poolSize);


        /**
		 * @brief プールからオブジェクトを取得する。
		 * @return プールから取得したオブジェクト。プールに空きがない場合はnullptr。
         */
        PoolObject* Spawn();

        /**
		 * @brief オブジェクトをプールに戻す。
		 * @param object プールに戻すオブジェクト。s
         */
        void Despawn(PoolObject* object);


    private:
        std::vector<PoolObject*> m_objects;
    };
}

#include "GameObjectPool.inl"