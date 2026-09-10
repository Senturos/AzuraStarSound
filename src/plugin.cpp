#include <RE/Skyrim.h>
#include <SKSE/SKSE.h>

#include <mutex>
#include <unordered_map>

namespace
{
    constexpr RE::FormID kAzurasStarBaseFormID = 0x00063B27;
    constexpr auto kSoundEditorID = "GosAzuraStarSound";

    std::mutex g_lock;
    std::unordered_map<RE::FormID, RE::BSSoundHandle> g_handles;

    bool IsAzurasStar(RE::TESObjectREFR* a_ref)
    {
        if (!a_ref) {
            return false;
        }
        auto* base = a_ref->GetBaseObject();
        return base && base->GetFormID() == kAzurasStarBaseFormID;
    }

    void StopSound(RE::FormID a_refID)
    {
        std::scoped_lock lock(g_lock);
        auto it = g_handles.find(a_refID);
        if (it != g_handles.end()) {
            if (it->second.IsValid()) {
                it->second.Stop();
            }
            g_handles.erase(it);
        }
    }

    void StartSound(RE::TESObjectREFR* a_ref)
    {
        if (!IsAzurasStar(a_ref)) {
            return;
        }

        const auto refID = a_ref->GetFormID();
        StopSound(refID);

        auto* descriptor = RE::TESForm::LookupByEditorID<RE::BGSSoundDescriptorForm>(kSoundEditorID);
        if (!descriptor) {
            SKSE::log::error("Could not find sound descriptor {}", kSoundEditorID);
            return;
        }

        RE::BSSoundHandle handle;
        auto* audio = RE::BSAudioManager::GetSingleton();
        if (!audio || !audio->BuildSoundDataFromDescriptor(handle, descriptor)) {
            SKSE::log::error("Could not build sound handle for {}", kSoundEditorID);
            return;
        }

        if (auto* node = a_ref->Get3D()) {
            handle.SetObjectToFollow(node);
        } else {
            handle.SetPosition(a_ref->GetPosition());
        }

        if (handle.Play()) {
            std::scoped_lock lock(g_lock);
            g_handles.emplace(refID, handle);
            SKSE::log::info("Started Azura's Star sound on {:08X}", refID);
        }
    }

    class ObjectLoadedSink final : public RE::BSTEventSink<RE::TESObjectLoadedEvent>
    {
    public:
        RE::BSEventNotifyControl ProcessEvent(
            const RE::TESObjectLoadedEvent* a_event,
            RE::BSTEventSource<RE::TESObjectLoadedEvent>*) override
        {
            if (!a_event) {
                return RE::BSEventNotifyControl::kContinue;
            }

            auto* form = RE::TESForm::LookupByID(a_event->formID);
            auto* ref = form ? form->As<RE::TESObjectREFR>() : nullptr;
            if (!ref || !IsAzurasStar(ref)) {
                return RE::BSEventNotifyControl::kContinue;
            }

            if (a_event->loaded) {
                StartSound(ref);
            } else {
                StopSound(a_event->formID);
            }

            return RE::BSEventNotifyControl::kContinue;
        }
    };

    ObjectLoadedSink g_objectLoadedSink;

    void MessageHandler(SKSE::MessagingInterface::Message* a_message)
    {
        if (!a_message) {
            return;
        }

        if (a_message->type == SKSE::MessagingInterface::kDataLoaded) {
            if (auto* holder = RE::ScriptEventSourceHolder::GetSingleton()) {
                holder->AddEventSink<RE::TESObjectLoadedEvent>(&g_objectLoadedSink);
                SKSE::log::info("AzuraStarSound event sink registered");
            }
        }
    }
}

SKSEPluginLoad(const SKSE::LoadInterface* a_skse)
{
    SKSE::Init(a_skse);
    SKSE::GetMessagingInterface()->RegisterListener(MessageHandler);
    return true;
}
