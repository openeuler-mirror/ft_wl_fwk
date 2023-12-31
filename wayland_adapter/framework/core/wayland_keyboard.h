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

#include "wayland_resource_object.h"

namespace FT {
namespace Wayland {
struct IWaylandKeyboard {
    static struct wl_keyboard_interface impl_;
};

class WaylandKeyboard final : public WaylandResourceObject {
    friend struct IWaylandKeyboard;

public:
    static OHOS::sptr<WaylandKeyboard> Create(struct wl_client *client, uint32_t version, uint32_t id);
    ~WaylandKeyboard() noexcept override;

    void OnKeyboardKey(int32_t key, int32_t state, uint32_t time);
    void OnKeyboardEnter(struct wl_resource *surface_resource);
    void OnKeyboardLeave(struct wl_resource *surface_resource);


private:
    WaylandKeyboard(struct wl_client *client, uint32_t version, uint32_t id);
};
} // namespace Wayland
} // namespace FT