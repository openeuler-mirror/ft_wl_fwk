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

#pragma once

#include "wayland_global.h"
#include "linux-dmabuf-unstable-v1-server-protocol.h"

namespace FT {
namespace Wayland {
struct IWaylandZwpLinuxDmabuf {
    static struct zwp_linux_dmabuf_v1_interface impl_;
};

class WaylandZwpLinuxDmabuf final : public WaylandGlobal {
    friend struct IWaylandZwpLinuxDmabuf;

public:
    static OHOS::sptr<WaylandZwpLinuxDmabuf> Create(struct wl_display *display);
    ~WaylandZwpLinuxDmabuf() noexcept override;

private:
    WaylandZwpLinuxDmabuf(struct wl_display *display);

    void Bind(struct wl_client *client, uint32_t version, uint32_t id) override;
};

class WaylandZwpLinuxDmabufObject final : public WaylandResourceObject {
    friend struct IWaylandZwpLinuxDmabuf;

public:
    WaylandZwpLinuxDmabufObject(struct wl_client *client, uint32_t version, uint32_t id);
    ~WaylandZwpLinuxDmabufObject() noexcept;

    void SendModifier();
};

} // namespace Wayland
} // namespace FT
