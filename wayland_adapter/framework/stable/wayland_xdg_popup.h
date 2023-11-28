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

#include <xdg-shell-server-protocol.h>
#include "wayland_resource_object.h"
#include "wayland_xdg_positioner.h"
#include "wayland_utils.h"

namespace FT {
namespace Wayland {
class WaylandXdgSurface;

struct IWaylandXdgPopup {
    static void Grab(struct wl_client *client, struct wl_resource *resource, struct wl_resource *seat, uint32_t serial);
    static void Reposition(struct wl_client *client, struct wl_resource *resource,
        struct wl_resource *positioner, uint32_t token);
    static struct xdg_popup_interface impl_;
};

class WaylandXdgPopup : public WaylandResourceObject {
public:
    static OHOS::sptr<WaylandXdgPopup> Create(const OHOS::sptr<WaylandXdgSurface> &xdgSurface,
        const OHOS::sptr<WaylandXdgSurface> &parentXdgSurface,
        const OHOS::sptr<WaylandXdgPositioner> &positioner, uint32_t id,
        OHOS::sptr<OHOS::Rosen::WindowOption> windowOption);
    ~WaylandXdgPopup() noexcept;
    void HandleCommit();
    void SetWindow(OHOS::sptr<OHOS::Rosen::Window> window);

private:
    WaylandXdgPopup(const OHOS::sptr<WaylandXdgSurface> &xdgSurface,
        const OHOS::sptr<WaylandXdgSurface> &parentXdgSurface,
        const OHOS::sptr<WaylandXdgPositioner> &positioner, uint32_t id);
    friend struct IWaylandXdgPopup;

    void Grab(struct wl_resource *seat, uint32_t serial);
    void Reposition(struct wl_resource *positioner, uint32_t token);

private:
    OHOS::wptr<WaylandXdgSurface> xdgSurface_;
    OHOS::wptr<WaylandXdgSurface> parentXdgSurface_;
    OHOS::sptr<OHOS::Rosen::Window> window_;
    Rect rect_;
};
} // namespace Wayland
} // namespace FT
