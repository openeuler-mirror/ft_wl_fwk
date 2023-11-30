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

#include <string>

#include <include/core/SkImageInfo.h>
#include "wayland-server-protocol.h"
#include "wm/window.h"

namespace FT {
namespace Wayland {
struct Offset {
    int32_t x = 0;
    int32_t y = 0;
    void Reset()
    {
        x = 0;
        y = 0;
    }
};

struct Size {
    uint32_t width = 0;
    uint32_t height = 0;
    void Reset()
    {
        width = 0;
        height = 0;
    }
};

struct WindowOptionExt {
    bool maximizeAfterShow = false;
    bool fullscreenAfterShow = false;
    bool minimizeAfterShow = false;
    std::string title;
};

using SurfaceCommitCallback = std::function<void()>;
using SurfaceRectCallback = std::function<void(OHOS::Rosen::Rect)>;
using WindowCreateCallback = std::function<void(OHOS::sptr<OHOS::Rosen::Window>)>;

class FrameCallback final : public WaylandResourceObject {
public:
    static OHOS::sptr<FrameCallback> Create(struct wl_client *client, uint32_t version, uint32_t callback)
    {
        return OHOS::sptr<FrameCallback>(new FrameCallback(client, version, callback));
    }
    uint32_t Serial() const
    {
        return serial_;
    }
private:
    FrameCallback(struct wl_client *client, uint32_t version, uint32_t callback)
        : WaylandResourceObject(client, &wl_callback_interface, version, callback, nullptr), serial_(callback) {}
    ~FrameCallback() noexcept override {}
    uint32_t serial_;
};

struct SurfaceState {
    struct wl_resource *buffer = nullptr;
    wl_output_transform transform = WL_OUTPUT_TRANSFORM_NORMAL;
    int32_t scale = 0;
    int32_t offsetX = 0;
    int32_t offsetY = 0;
    OHOS::sptr<FrameCallback> cb;
    OHOS::Rosen::Rect damage;
    OHOS::Rosen::Rect damageBuffer;
    OHOS::Rosen::Rect inputRegion;
    OHOS::Rosen::Rect opaqueRegion;
    void Reset()
    {
        transform = WL_OUTPUT_TRANSFORM_NORMAL;
        scale = 0;
        offsetX = 0;
        offsetY = 0;
        damage.posX_ = 0;
        damage.posY_ = 0;
        damage.width_ = 0;
        damage.height_ = 0;
        damageBuffer.posX_ = 0;
        damageBuffer.posY_ = 0;
        damageBuffer.width_ = 0;
        damageBuffer.height_ = 0;
        inputRegion.posX_ = 0;
        inputRegion.posY_ = 0;
        inputRegion.width_ = 0;
        inputRegion.height_ = 0;
        opaqueRegion.posX_ = 0;
        opaqueRegion.posY_ = 0;
        opaqueRegion.width_ = 0;
        opaqueRegion.height_ = 0;
    }
};

enum class SurfaceRole : uint32_t {
    NONE = 0,
    XDG_TOPLEVEL,
    XDG_POPUP
};

static SkColorType ShmFormatToSkia(const uint32_t& shmFormat)
{
    switch (shmFormat) {
        case WL_SHM_FORMAT_ARGB8888:
             return SkColorType::kBGRA_8888_SkColorType;
        case WL_SHM_FORMAT_RGBA8888:
            return SkColorType::kRGBA_8888_SkColorType;
        default:
            return SkColorType::kUnknown_SkColorType;
    }
}

struct SubSurfaceData {
    struct wl_resource *surface = nullptr;
    int32_t offsetX = 0;
    int32_t offsetY = 0;
};

} // namespace Wayland
} // namespace FT
