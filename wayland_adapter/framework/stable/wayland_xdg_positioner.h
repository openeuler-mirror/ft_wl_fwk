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
#include "wayland_utils.h"

namespace FT {
namespace Wayland {
struct WaylandXdgPositionerInterface {
    static void SetSize(struct wl_client *client, struct wl_resource *resource, int32_t width, int32_t height);
    static void SetAnchorRect(struct wl_client *client, struct wl_resource *resource,
        int32_t x, int32_t y, int32_t width, int32_t height);
    static void SetAnchor(struct wl_client *client, struct wl_resource *resource, uint32_t anchor);
    static void SetGravity(struct wl_client *client, struct wl_resource *resource, uint32_t gravity);
    static void SetConstraintAdjustment(struct wl_client *client, struct wl_resource *resource, uint32_t constraintAdjustment);
    static void SetOffset(struct wl_client *client, struct wl_resource *resource, int32_t x, int32_t y);
    static void SetReactive(struct wl_client *client, struct wl_resource *resource);
    static void SetParentSize(struct wl_client *client, struct wl_resource *resource,
        int32_t parentWidth, int32_t parentHeight);
    static void SetParentConfigure(struct wl_client *client, struct wl_resource *resource, uint32_t serial);
    static struct xdg_positioner_interface impl_;
};

class WaylandXdgPositioner final : public WaylandResourceObject {
public:
    static OHOS::sptr<WaylandXdgPositioner> Create(struct wl_client *client, uint32_t version, uint32_t id);
    ~WaylandXdgPositioner() noexcept override;

    Size GetSize() const
    {
        return size_;
    }
    OHOS::Rosen::Rect GetAnchorRect() const
    {
        return anchorRect_;
    }
    enum xdg_positioner_anchor GetAnchor() const
    {
        return anchor_;
    }
    enum xdg_positioner_gravity GetGravity() const
    {
        return gravity_;
    }
    enum xdg_positioner_constraint_adjustment GetConstraintAdjustment() const
    {
        return constraintAdjustment_;
    }
    Offset GetOffset() const
    {
        return offset_;
    }
    bool GetReactive() const
    {
        return reactive_;
    }
    Size GetParentSize() const
    {
        return parentSize_;
    }
    uint32_t GetParentConfigureSerial() const
    {
        return parentConfigureSerial_;
    }

private:
    WaylandXdgPositioner(struct wl_client *client, uint32_t version, uint32_t id);

private:
    friend struct WaylandXdgPositionerInterface;
    // WaylandXdgPositionerInterface will call these member functions.
    void SetSize(int32_t width, int32_t height);
    void SetAnchorRect(int32_t x, int32_t y, int32_t width, int32_t height);
    void SetAnchor(uint32_t anchor);
    void SetGravity(uint32_t gravity);
    void SetConstraintAdjustment(uint32_t constraintAdjustment);
    void SetOffset(int32_t x, int32_t y);
    void SetReactive();
    void SetParentSize(int32_t parentWidth, int32_t parentHeight);
    void SetParentConfigure(uint32_t serial);

private:
    Size size_;
    OHOS::Rosen::Rect anchorRect_;
    enum xdg_positioner_anchor anchor_ = XDG_POSITIONER_ANCHOR_NONE;
    enum xdg_positioner_gravity gravity_ = XDG_POSITIONER_GRAVITY_NONE;
    enum xdg_positioner_constraint_adjustment constraintAdjustment_ = XDG_POSITIONER_CONSTRAINT_ADJUSTMENT_NONE;
    Offset offset_;
    bool reactive_ = false;
    Size parentSize_;
    uint32_t parentConfigureSerial_ = 0;
};
} // namespace Wayland
} // namespace FT
