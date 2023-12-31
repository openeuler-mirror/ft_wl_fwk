/*
 * Copyright (c) 2023 Huawei Technologies Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <mutex>
#include <thread>
#include "wayland_seat.h"

#include "wayland_objects_pool.h"
#include "version.h"
#include <struct_multimodal.h>

using namespace OHOS::MMI;
namespace FT {
namespace Wayland {
namespace {
    constexpr HiLogLabel LABEL = {LOG_CORE, HILOG_DOMAIN_WAYLAND, "WaylandSeat"};
}

static OHOS::sptr<WaylandSeat> wl_seat_global = nullptr;
std::mutex wl_seat_global_mutex_;

struct wl_seat_interface IWaylandSeat::impl_ = {
    .get_pointer = GetPointer,
    .get_keyboard = GetKeyboard,
    .get_touch = GetTouch,
    .release = WaylandResourceObject::DefaultDestroyResource,};

void IWaylandSeat::GetPointer(struct wl_client *client, struct wl_resource *resource, uint32_t id)
{
    CAST_OBJECT_AND_CALL_FUNC(WaylandSeatObject, resource,
        "IWaylandSeat::GetPointer: failed to find object.", GetPointer, id);
}

void IWaylandSeat::GetKeyboard(struct wl_client *client, struct wl_resource *resource, uint32_t id)
{
    CAST_OBJECT_AND_CALL_FUNC(WaylandSeatObject, resource,
        "IWaylandSeat::GetKeyboard: failed to find object.", GetKeyboard, id);
}

void IWaylandSeat::GetTouch(struct wl_client *client, struct wl_resource *resource, uint32_t id)
{
    CAST_OBJECT_AND_CALL_FUNC(WaylandSeatObject, resource,
        "IWaylandSeat::GetTouch: failed to find object.", GetTouch, id);
}

OHOS::sptr<WaylandSeat> WaylandSeat::Create(struct wl_display *display)
{
    std::lock_guard<std::mutex> lock(wl_seat_global_mutex_);
    if (display == nullptr) {
        LOG_ERROR("Display is nullptr");
        return nullptr;
    }

    if (wl_seat_global != nullptr) {
        return wl_seat_global;
    }

    wl_seat_global = OHOS::sptr<WaylandSeat>(new WaylandSeat(display));
    wl_seat_global->GetCapabilities();
    wl_seat_global->inputListener_ = std::make_shared<WaylandInputDeviceListener>(wl_seat_global);
    InputManager::GetInstance()->RegisterDevListener("change", wl_seat_global->inputListener_);

    return wl_seat_global;
}

OHOS::sptr<WaylandSeat> WaylandSeat::GetWaylandSeatGlobal()
{
    std::lock_guard<std::mutex> lock(wl_seat_global_mutex_);
    return wl_seat_global;
}

void WaylandSeat::FreeSeatResource(struct wl_client *client, struct wl_resource *resource)
{
    std::lock_guard<std::mutex> lock(seatResourcesMutex_);
    auto iter = seatResourcesMap_.find(client);
    if (iter == seatResourcesMap_.end()) {
        return;
    }

    auto seatObjectList = &iter->second;
    for (auto itr = seatObjectList->begin(); itr != seatObjectList->end(); itr++) {
        if ((*itr)->WlResource() == resource) {
            seatObjectList->erase(itr);
            break;
        }
    }
    if (seatResourcesMap_[client].empty()) {
        seatResourcesMap_.erase(client);
    }
}

WaylandSeat::WaylandSeat(struct wl_display *display)
    : WaylandGlobal(display, &wl_seat_interface, WL_SEAT_MAX_VERSION) {}

WaylandSeat::~WaylandSeat() noexcept
{
    if (inputListener_ != nullptr) {
        InputManager::GetInstance()->UnregisterDevListener("change", inputListener_);
        inputListener_ = nullptr;
    }
}

void WaylandSeat::Bind(struct wl_client *client, uint32_t version, uint32_t id)
{
    auto object = OHOS::sptr<WaylandSeatObject>(new WaylandSeatObject(client, version, id));
    if (object == nullptr) {
        LOG_ERROR("No memory");
        return;
    }

    std::lock_guard<std::mutex> lock(seatResourcesMutex_);
    WaylandObjectsPool::GetInstance().AddObject(ObjectId(object->WlClient(), object->Id()), object);
    seatResourcesMap_[client].emplace_back(object);
    UpdateCapabilities(object->WlResource());
}

void WaylandSeat::GetKeyboardResource(struct wl_client *client, std::list<OHOS::sptr<WaylandKeyboard>>& list)
{
    std::lock_guard<std::mutex> lock(seatResourcesMutex_);
    auto iter = seatResourcesMap_.find(client);
    if (iter == seatResourcesMap_.end()) {
        return;
    }

    /* A wl_client object maybe has many seatResourceObjects, each seatResourceObject maybe has many keyboardResourceObjects
     * so we need get all keyboardResourceObjects in this wl_client object.
     */
    auto seatList = iter->second;
    for (auto& seatResourceItem : seatList) {
        std::list<OHOS::sptr<WaylandKeyboard>> KeyboardList;
        seatResourceItem->GetChildKeyboard(KeyboardList);
        if (KeyboardList.empty()) {
            continue;
        }
        for (auto& keyboardResourceItem : KeyboardList) {
            list.emplace_back(keyboardResourceItem);
        }
    }
}

void WaylandSeat::GetPointerResource(struct wl_client *client, std::list<OHOS::sptr<WaylandPointer>>& list)
{
    std::lock_guard<std::mutex> lock(seatResourcesMutex_);
    auto iter = seatResourcesMap_.find(client);
    if (iter == seatResourcesMap_.end()) {
        return;
    }

    /* A wl_client object maybe has many seatResourceObjects, each seatResourceObject maybe has many pointerResourceObjects
     * so we need get all pointerResourceObjects in this wl_client object.
     */
    auto seatList = iter->second;
    for (auto& seatResourceItem : seatList) {
        std::list<OHOS::sptr<WaylandPointer>> pointerList;
        seatResourceItem->GetChildPointer(pointerList);
        if (pointerList.empty()) {
            continue;
        }
        for (auto& pointerResourceItem : pointerList) {
            list.emplace_back(pointerResourceItem);
        }
    }
}

bool WaylandSeat::IsHotPlugIn()
{
    return isHotPlugIn_;
}

void WaylandSeat::ResetHotPlugIn()
{
    isHotPlugIn_ = false;
}

void WaylandSeat::GetCapabilities()
{
    LOG_INFO("GetCapabilities in");
    int32_t DevNums = 0;
    int32_t hasGetDevNums = 0;
    bool isGetIds = false;
    uint32_t oldCaps = caps_;
    caps_ = 0;

    auto GetDeviceCb = [&hasGetDevNums, this](std::shared_ptr<InputDevice> inputDevice) {
        LOG_INFO("Get device success, id=%{public}d, name=%{public}s, type=%{public}d",
            inputDevice->GetId(), inputDevice->GetName().c_str(), inputDevice->GetType());
        if (inputDevice->GetType() == static_cast<int32_t>(DEVICE_TYPE_MOUSE)) {
            caps_ |= WL_SEAT_CAPABILITY_POINTER;
        } else if (inputDevice->GetType() == static_cast<int32_t>(DEVICE_TYPE_KEYBOARD)) {
            caps_ |= WL_SEAT_CAPABILITY_KEYBOARD;
        }
        hasGetDevNums++;
    };
    auto GetDeviceIdsCb = [&DevNums, &isGetIds](std::vector<int32_t> ids) {
        DevNums = ids.size();
        isGetIds = true;
    };

    int32_t ret = InputManager::GetInstance()->GetDeviceIds(GetDeviceIdsCb);
    int32_t wait_count = 0;
    while (ret != 0 && wait_count < 10) {
        usleep(3 * 1000); // wait for MMI service online
        ret = InputManager::GetInstance()->GetDeviceIds(GetDeviceIdsCb);
        wait_count++;
    }
    if (ret != 0) {
        LOG_ERROR("Failed to GetDeviceIds, MMI service offline");
        return;
    }

    wait_count = 0;
    while (!isGetIds && wait_count < 100) {
        usleep(3 * 1000); // wait for GetDeviceIdsCb finish
        wait_count++;
    }

    for (int32_t i = 0; i < DevNums; i++) {
        InputManager::GetInstance()->GetDevice(i, GetDeviceCb);
    }

    wait_count = 0;
    while (hasGetDevNums != DevNums && wait_count < 100) {
        usleep(3 * 1000); // wait for GetDeviceCb finish
        wait_count++;
    }

    if (caps_ > oldCaps) {
        isHotPlugIn_ = true;
    }
}

void WaylandSeat::UpdateCapabilities(struct wl_resource *resource)
{
    LOG_INFO("UpdateCapabilities in");
    wl_seat_send_capabilities(resource, caps_);
}

void WaylandSeat::SendNewCapabilities()
{
    uint32_t oldCaps = caps_;
    GetCapabilities();
    if (oldCaps == caps_) {
        LOG_INFO("Caps unchange, no need to report");
        return;
    } else {
        for (auto it = seatResourcesMap_.begin(); it != seatResourcesMap_.end(); ++it) {
            auto seatList = it->second;
            for (auto it1 = seatList.begin(); it1 != seatList.end(); ++it1) {
                UpdateCapabilities((*it1)->WlResource());
            }
        }
    }
}

void WaylandSeat::OnDeviceAdded(int32_t deviceId)
{
    std::thread capsThread([this]() {
        std::lock_guard<std::mutex> lock(capsMutex_);
        const static int32_t CPAS_MOUSE_AND_KEYBOARD = 3;
        if (caps_ == CPAS_MOUSE_AND_KEYBOARD) {
            LOG_INFO("Device added: already connected the mouse and keyboard, no need to report");
        } else {
            SendNewCapabilities();
        }
    });
    capsThread.detach();
}

void WaylandSeat::OnDeviceRemoved(int32_t deviceId)
{
    std::thread capsThread([this]() {
        std::lock_guard<std::mutex> lock(capsMutex_);
        SendNewCapabilities();
    });
    capsThread.detach();
}

WaylandSeatObject::WaylandSeatObject(struct wl_client *client, uint32_t version, uint32_t id)
    : WaylandResourceObject(client, &wl_seat_interface, version, id, &IWaylandSeat::impl_)
{
    LOG_DEBUG("WaylandSeatObject create, this=%{public}p, id=%{public}u", this, id);
}

void WaylandSeatObject::OnResourceDestroy()
{
    // free pointer
    auto pointerIter = pointerResourcesMap_.find(WlClient());
    if (pointerIter != pointerResourcesMap_.end()) {
        pointerIter->second.clear();
    }

    // free keyboard
    auto keyboardIter = keyboardResourcesMap_.find(WlClient());
    if (keyboardIter != keyboardResourcesMap_.end()) {
        keyboardIter->second.clear();
    }

    OHOS::sptr<WaylandSeat> wlSeat = WaylandSeat::GetWaylandSeatGlobal();
    if (wlSeat == nullptr) {
        return;
    }
    wlSeat->FreeSeatResource(WlClient(), WlResource());
}

WaylandSeatObject::~WaylandSeatObject() noexcept
{
    LOG_DEBUG("WaylandSeatObject release, this=%{public}p", this);
}

void WaylandSeatObject::GetChildPointer(std::list<OHOS::sptr<WaylandPointer>> &list)
{
    auto iter = pointerResourcesMap_.find(WlClient());
    if (iter == pointerResourcesMap_.end()) {
        return;
    }
    list = iter->second;
}

void WaylandSeatObject::GetChildKeyboard(std::list<OHOS::sptr<WaylandKeyboard>> &list)
{
    auto iter = keyboardResourcesMap_.find(WlClient());
    if (iter == keyboardResourcesMap_.end()) {
        return;
    }
    list = iter->second;
}

void WaylandSeatObject::GetPointer(uint32_t id)
{
    auto pointer = WaylandPointer::Create(WlClient(), wl_resource_get_version(WlResource()), id);
    if (pointer == nullptr) {
        LOG_ERROR("No memory");
        return;
    }

    pointerResourcesMap_[WlClient()].emplace_back(pointer);
}

void WaylandSeatObject::GetKeyboard(uint32_t id)
{
    auto keyboard = WaylandKeyboard::Create(WlClient(), wl_resource_get_version(WlResource()), id);
    if (keyboard == nullptr) {
        LOG_ERROR("No memory");
        return;
    }
    keyboardResourcesMap_[WlClient()].emplace_back(keyboard);
}

void WaylandSeatObject::GetTouch(uint32_t id)
{
}
} // namespace Wayland
} // namespace FT
