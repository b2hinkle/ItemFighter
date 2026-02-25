
#include <Source/GameEntitySystemComponent.h>

#include <xXGameProjectNameXx/xXGameProjectNameXxTypeIds.h>
#include <AzCore/Serialization/SerializeContext.h>
#include <O3deUtils/Misc/MultiplayerUtils.h>
#include <O3deUtils/Core/AzFrameworkUtils.h>
#include <AzCore/Console/ILogger.h>
#include <AzCore/Debug/Trace.h>
#include <Multiplayer/MultiplayerTypes.h>
#include <xXGameProjectNameXx/SettingsRegistryAccessors.h>
#include <AzCore/Asset/AssetManagerBus.h>
#include <Multiplayer/NetworkEntity/INetworkEntityManager.h>
#include <LmbrCentral/Scripting/TagComponentBus.h>

namespace xXGameProjectNameXx
{
    bool IsGameEntity(const AZ::EntityId& entityId)
    {
        // @Christian: TODO: [todo][techdebt][tag] Use a proper "gameplay tag" system instead of LmbrCentral's tag component.
        bool result{};
        LmbrCentral::TagComponentRequestBus::EventResult(result, entityId, &LmbrCentral::TagComponentRequestBus::Handler::HasTag, GameEntityTag);
        return result;
    }

    AZ_COMPONENT_IMPL(GameEntitySystemComponent, "GameEntitySystemComponent",
        GetTypeId<GameEntitySystemComponent>());

    void GameEntitySystemComponent::Reflect(AZ::ReflectContext* context)
    {
        if (auto serializeContext = azrtti_cast<AZ::SerializeContext*>(context))
        {
            serializeContext->Class<GameEntitySystemComponent, AZ::Component>()
                ->Version(0)
                ;
        }
    }

    void GameEntitySystemComponent::GetProvidedServices(AZ::ComponentDescriptor::DependencyArrayType& provided)
    {
        provided.push_back(AZ_CRC_CE("GameEntityService"));
    }

    void GameEntitySystemComponent::GetIncompatibleServices(AZ::ComponentDescriptor::DependencyArrayType& incompatible)
    {
        incompatible.push_back(AZ_CRC_CE("GameEntityService"));
    }

    void GameEntitySystemComponent::GetRequiredServices([[maybe_unused]] AZ::ComponentDescriptor::DependencyArrayType& required)
    {
        required.push_back(AZ_CRC_CE("MultiplayerService"));
    }

    void GameEntitySystemComponent::GetDependentServices([[maybe_unused]] AZ::ComponentDescriptor::DependencyArrayType& dependent)
    {
        dependent.push_back(AZ_CRC_CE("MultiplayerService"));
    }

    GameEntitySystemComponent::GameEntitySystemComponent()
    {
        if (GameEntityInterface::Get() == nullptr)
        {
            GameEntityInterface::Register(this);
        }
    }

    GameEntitySystemComponent::~GameEntitySystemComponent()
    {
        if (GameEntityInterface::Get() == this)
        {
            GameEntityInterface::Unregister(this);
        }
    }

    void GameEntitySystemComponent::Init()
    {
    }

    void GameEntitySystemComponent::Activate()
    {
        GameEntityRequestBus::Handler::BusConnect();
        AzFramework::RootSpawnableNotificationBus::Handler::BusConnect();

        O3deUtils::GetNetworkEntityManagerAsserted().AddControllersActivatedHandler(m_onNetworkEntityControllersActivatedHandler);
        O3deUtils::GetNetworkEntityManagerAsserted().AddControllersDeactivatedHandler(m_onNetworkEntityControllersDeactivatedHandler);
    }

    void GameEntitySystemComponent::Deactivate()
    {
        GameEntityRequestBus::Handler::BusDisconnect();
        AzFramework::RootSpawnableNotificationBus::Handler::BusDisconnect();

        m_onNetworkEntityControllersActivatedHandler.Disconnect();
        m_onNetworkEntityControllersDeactivatedHandler.Disconnect();
    }

    void GameEntitySystemComponent::OnRootSpawnableAssigned([[maybe_unused]] AZ::Data::Asset<AzFramework::Spawnable> rootSpawnable, [[maybe_unused]] uint32_t generation)
    {
    }

    void GameEntitySystemComponent::OnRootSpawnableReady([[maybe_unused]] AZ::Data::Asset<AzFramework::Spawnable> rootSpawnable, [[maybe_unused]] uint32_t generation)
    {
#if AZ_TRAIT_SERVER
        if (HasAuthority())
        {
            // Whenever the root spawnable is ready (i.e., when a level has finished loading) spawn the "game entity".
            SpawnNewGameEntity();
        }
#endif // #if AZ_TRAIT_SERVER
    }

    void GameEntitySystemComponent::OnRootSpawnableReleased([[maybe_unused]] uint32_t generation)
    {
    }

    void GameEntitySystemComponent::OnNetworkEntityControllersActivated(
        [[maybe_unused]] const Multiplayer::ConstNetworkEntityHandle& entityHandle,
        [[maybe_unused]] Multiplayer::EntityIsMigrating entityIsMigrating)
    {
        // @Christian: TODO: [todo][engine][temporary] Use the asserted version of the get entity id function once we fix the
        // engine to make sure this event gets called while the entity handle is still valid (i.e., still exists in the network entity tracker).
#if 0
        const AZ::EntityId entityLocalId = O3deUtils::GetEntityIdByNetEntityIdAsserted(entityHandle.GetNetEntityId());
#endif // #if 0

        const AZ::EntityId entityLocalId = O3deUtils::TryGetEntityIdByNetEntityId(entityHandle.GetNetEntityId());
        if (IsGameEntity(entityLocalId))
        {
            {
                AZStd::fixed_string<256> logString;

                logString += '`';
                logString += __func__;
                logString += "`: ";
                logString += "Handling game entity on controllers activated. We will set the current game entity handle.";

                AZLOG_INFO(logString.data());
            }

            O3deUtils::ConstNetworkEntityHandleWithId currentGameEntityReference = GetCurrentGameEntityReference();

            if (O3deUtils::IsNetworkEntityHandleSet(currentGameEntityReference.GetNetworkEntityHandle()))
            {
                // It's okay if it's already set by an earlier point. I.e., set by `SpawnNewGameEntity`.
                AZ_Assert(currentGameEntityReference.GetNetworkEntityHandle() == entityHandle, "A second game entity is attempting to be used! Ignoring it.");
            }
            else
            {
                currentGameEntityReference.Set(entityHandle, entityLocalId);
            }
        }
    }

    void GameEntitySystemComponent::OnNetworkEntityControllersDeactivated(
        [[maybe_unused]] const Multiplayer::ConstNetworkEntityHandle& entityHandle,
        [[maybe_unused]] Multiplayer::EntityIsMigrating entityIsMigrating)
    {
        const AZ::EntityId entityLocalId = O3deUtils::GetEntityIdByNetEntityIdAsserted(entityHandle.GetNetEntityId());
        if (IsGameEntity(entityLocalId))
        {
            {
                AZStd::fixed_string<256> logString;

                logString += '`';
                logString += __func__;
                logString += "`: ";
                logString += "Handling game entity on controllers deactivated. We will reset the current game entity handle.";

                AZLOG_INFO(logString.data());
            }

            GetCurrentGameEntityReference().Reset();
        }
    }

#if AZ_TRAIT_SERVER
    bool GameEntitySystemComponent::HasAuthority() const
    {
        return O3deUtils::IsHosting();
    }

    void GameEntitySystemComponent::SpawnNewGameEntity()
    {
        AZ_Assert(HasAuthority(), "Should only be called on authority.");
        AZ_Assert(!O3deUtils::IsNetworkEntityHandleSet(GetCurrentGameEntityReference().GetNetworkEntityHandle()), "The current game entity handle should not already be set.");

        {
            AZStd::fixed_string<256> logString;

            logString += '`';
            logString += __func__;
            logString += "`: ";
            logString += "Spawning new game entity and assigning it as the current.";

            AZLOG_INFO(logString.data());
        }

        // @Christian: TODO: [todo] This assignment of the current entity references is redundant on the server.

        Multiplayer::ConstNetworkEntityHandle newGameEntityHandle = CreateGameEntity();
        AZ::EntityId newGameEntityId = O3deUtils::TryGetEntityIdByNetEntityId(newGameEntityHandle.GetNetEntityId());

        GetCurrentGameEntityReference().Set(newGameEntityHandle, newGameEntityId);
    }

    Multiplayer::NetworkEntityHandle GameEntitySystemComponent::CreateGameEntity() const
    {
        AZ_Assert(HasAuthority(), "Should only be called on authority.");

        {
            AZStd::fixed_string<256> logString;

            logString += '`';
            logString += __func__;
            logString += "`: ";
            logString += "Spawning entities.";

            AZLOG_INFO(logString.data());
        }

        AZ_Assert(O3deUtils::IsRootSpawnableReady(), "The level should be fully loaded at this point, because we are about to sample the transform from an entity reference from the level.");

        AZStd::optional<AZ::Data::Asset<AzFramework::Spawnable>> spawnableAsset = SettingsRegistryAccessors::TryGetGameEntityPrefabAsset();
        if (!spawnableAsset.has_value())
        {
            AZStd::fixed_string<256> logString;

            logString += '`';
            logString += __func__;
            logString += "`: ";
            logString += "No spawnable asset found from settings registry. Returning invalid entity handle.";

            AZLOG_INFO(logString.data());
            return {};
        }

        constexpr Multiplayer::NetEntityRole netEntityRole = Multiplayer::NetEntityRole::Authority;

        // Only spawn a single entity from the prefab, as this system expects only one entity.
        Multiplayer::PrefabEntityId prefabEntityId = O3deUtils::MakeSinglePrefabEntityIdFromSpawnableAsset(*spawnableAsset);

        {
            AZStd::fixed_string<256> logString;

            logString += '`';
            logString += __func__;
            logString += "`: ";
            logString += "Spawning prefab '";
            logString += prefabEntityId.m_prefabName.GetStringView();
            logString += "'.";

            AZLOG_INFO(logString.data());
        }

        Multiplayer::INetworkEntityManager::EntityList createdEntities =
            O3deUtils::GetNetworkEntityManagerAsserted().CreateEntitiesImmediate(
                AZStd::move(prefabEntityId),
                netEntityRole,
                AZ::Transform::Identity());

        if (createdEntities.empty())
        {
            AZStd::fixed_string<256> logString;

            logString += '`';
            logString += __func__;
            logString += "`: ";
            logString += "Attempt to spawn prefab '";
            logString += spawnableAsset->GetHint();
            logString += "' failed. No entities were spawned.";
            logString += ' ';
            logString += "Ensure that the prefab contains a single entity that is network enabled with a network binding component.";

            AZLOG_ERROR(logString.data());
            return {};
        }

        Multiplayer::NetworkEntityHandle& spawnedEntityHandle = createdEntities[0];

        const AZ::Entity* spawnedEntity = spawnedEntityHandle.GetEntity();
        if (!spawnedEntity)
        {
            AZStd::fixed_string<256> logString;

            logString += '`';
            logString += __func__;
            logString += "`: ";
            logString += "The spawned entity is null.";

            AZLOG_ERROR(logString.data());
        }

        return AZStd::move(spawnedEntityHandle);
    }
#endif // #if AZ_TRAIT_SERVER
}
