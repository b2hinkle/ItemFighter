
#pragma once

#include <AzCore/Component/Component.h>

#include <xXGameProjectNameXx/GameEntityBus.h>
#include <AzFramework/Spawnable/RootSpawnableInterface.h>
#include <Multiplayer/NetworkEntity/NetworkEntityHandle.h>
#include <Multiplayer/MultiplayerTypes.h>
#include <Multiplayer/NetworkEntity/INetworkEntityManager.h>
#include <LmbrCentral/Scripting/TagComponentBus.h>
#include <AzCore/Component/EntityId.h>

namespace xXGameProjectNameXx
{
    static bool IsGameEntity(const AZ::EntityId& entityId);

    // @Christian: TODO: [todo][techdebt][tag] Use a proper "gameplay tag" system instead of LmbrCentral's tag component.
    constexpr LmbrCentral::Tag GameEntityTag = AZ_CRC_CE("xXGameProjectNameXx.GameEntity");

    //! @brief
    //! @todo @Christian: TODO: [todo][naming] Rename "game entity" to something that doesn't conflict with the engine's existing
    //!       terminology for what a "game" entity already is ("game" entities are basically just non-editor entities).
    class GameEntitySystemComponent
        : public AZ::Component
        , protected GameEntityRequestBus::Handler
        , protected AzFramework::RootSpawnableNotificationBus::Handler
    {
    public:

        AZ_COMPONENT_DECL(GameEntitySystemComponent);

        GameEntitySystemComponent();
        ~GameEntitySystemComponent() override;

        //! Component descriptor interface.
        //! @{
        static void Reflect(AZ::ReflectContext* context);
        static void GetProvidedServices(AZ::ComponentDescriptor::DependencyArrayType& provided);
        static void GetIncompatibleServices(AZ::ComponentDescriptor::DependencyArrayType& incompatible);
        static void GetRequiredServices(AZ::ComponentDescriptor::DependencyArrayType& required);
        static void GetDependentServices(AZ::ComponentDescriptor::DependencyArrayType& dependent);
        //! @}

    protected:

        //! AZ::Component protected overrides.
        //! @{
        void Init() override;
        void Activate() override;
        void Deactivate() override;
        //! @}

        //! AzFramework::RootSpawnableNotificationBus::Handler protected overrides.
        //! @{
        void OnRootSpawnableAssigned(AZ::Data::Asset<AzFramework::Spawnable> rootSpawnable, uint32_t generation) override;
        void OnRootSpawnableReady(AZ::Data::Asset<AzFramework::Spawnable> rootSpawnable, uint32_t generation) override;
        void OnRootSpawnableReleased(uint32_t generation) override;
        //! @}

        void OnNetworkEntityControllersActivated(const Multiplayer::ConstNetworkEntityHandle& entityHandle, Multiplayer::EntityIsMigrating entityIsMigrating);
        void OnNetworkEntityControllersDeactivated(const Multiplayer::ConstNetworkEntityHandle& entityHandle, Multiplayer::EntityIsMigrating entityIsMigrating);

#if AZ_TRAIT_SERVER
        bool HasAuthority() const;
        void SpawnNewGameEntity();
        Multiplayer::NetworkEntityHandle CreateGameEntity() const;
#endif // #if AZ_TRAIT_SERVER

    private:

        Multiplayer::ControllersActivatedEvent::Handler m_onNetworkEntityControllersActivatedHandler{
            [this](const Multiplayer::ConstNetworkEntityHandle& entityHandle, Multiplayer::EntityIsMigrating entityIsMigrating)
            {
                OnNetworkEntityControllersActivated(entityHandle, entityIsMigrating);
            }
        };

        Multiplayer::ControllersDeactivatedEvent::Handler m_onNetworkEntityControllersDeactivatedHandler{
            [this](const Multiplayer::ConstNetworkEntityHandle& entityHandle, Multiplayer::EntityIsMigrating entityIsMigrating)
            {
                OnNetworkEntityControllersDeactivated(entityHandle, entityIsMigrating);
            }
        };

        // @Christian: TODO: Combine these together in a struct and encapsulate them to ensure they both always get assigned and
        // don't get out of sync. Idea: We can make it a struct of references so that we aren't forcing a memory layout on people.
        Multiplayer::ConstNetworkEntityHandle m_currentGameEntityHandle{};
        AZ::EntityId m_currentGameEntityId{};
    };
}
