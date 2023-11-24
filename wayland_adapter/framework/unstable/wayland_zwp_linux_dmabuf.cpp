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

#include "wayland_zwp_linux_dmabuf.h"

#include "wayland_objects_pool.h"
#include "version.h"
#include <drm_fourcc.h>

namespace FT {
namespace Wayland {
namespace {
    constexpr HiLogLabel LABEL = {LOG_CORE, HILOG_DOMAIN_WAYLAND, "WaylandZwpLinuxDmabuf"};
}

struct zwp_linux_dmabuf_v1_interface IWaylandZwpLinuxDmabuf::impl_ = {
    .destroy = WaylandResourceObject::DefaultDestroyResource,
};

OHOS::sptr<WaylandZwpLinuxDmabuf> WaylandZwpLinuxDmabuf::Create(struct wl_display *display)
{
    if (display == nullptr) {
        LOG_ERROR("display is nullptr");
        return nullptr;
    }

    return OHOS::sptr<WaylandZwpLinuxDmabuf>(new WaylandZwpLinuxDmabuf(display));
}

WaylandZwpLinuxDmabuf::WaylandZwpLinuxDmabuf(struct wl_display *display)
    : WaylandGlobal(display, &zwp_linux_dmabuf_v1_interface, ZWP_LINUX_DMABUF_V1_MAX_VERSION)
{
}

WaylandZwpLinuxDmabuf::~WaylandZwpLinuxDmabuf() noexcept
{
}

void WaylandZwpLinuxDmabuf::Bind(struct wl_client *client, uint32_t version, uint32_t id)
{
    auto object = OHOS::sptr<WaylandZwpLinuxDmabufObject>(new WaylandZwpLinuxDmabufObject(client, version, id));
    if (object == nullptr) {
        LOG_ERROR("no memory");
        return;
    }

    WaylandObjectsPool::GetInstance().AddObject(ObjectId(object->WlClient(), object->Id()), object);
    object->SendModifier();
}

WaylandZwpLinuxDmabufObject::WaylandZwpLinuxDmabufObject(struct wl_client *client, uint32_t version, uint32_t id)
    : WaylandResourceObject(client, &zwp_linux_dmabuf_v1_interface, version, id, &IWaylandZwpLinuxDmabuf::impl_)
{
}

WaylandZwpLinuxDmabufObject::~WaylandZwpLinuxDmabufObject() noexcept
{
}

void WaylandZwpLinuxDmabufObject::SendModifier()
{
    uint64_t mod = DRM_FORMAT_MOD_INVALID;
    zwp_linux_dmabuf_v1_send_modifier(WlResource(), DRM_FORMAT_XRGB8888, mod >> 32, mod & 0xFFFFFFFF);
    zwp_linux_dmabuf_v1_send_format(WlResource(), DRM_FORMAT_XRGB8888);
}
} // namespace Wayland
} // namespace FT
