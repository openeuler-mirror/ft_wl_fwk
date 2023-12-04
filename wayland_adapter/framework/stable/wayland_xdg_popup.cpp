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

#include "wayland_xdg_popup.h"

#include "wayland_objects_pool.h"
#include "wayland_xdg_toplevel.h"
#include "wayland_xdg_wm_base.h"

namespace FT {
namespace Wayland {
namespace {
    constexpr HiLogLabel LABEL = {LOG_CORE, HILOG_DOMAIN_WAYLAND, "WaylandXdgPopup"};
}

struct xdg_popup_interface IWaylandXdgPopup::impl_ = {
    .destroy = WaylandResourceObject::DefaultDestroyResource,
    .grab = IWaylandXdgPopup::Grab,
    .reposition = IWaylandXdgPopup::Reposition};

void IWaylandXdgPopup::Grab(struct wl_client *client, struct wl_resource *resource,
    struct wl_resource *seat, uint32_t serial)
{
    CAST_OBJECT_AND_CALL_FUNC(WaylandXdgPopup, resource,
        "IWaylandXdgPopup::Grab: failed to find object.", Grab, seat, serial);
}

void IWaylandXdgPopup::Reposition(struct wl_client *client, struct wl_resource *resource,
    struct wl_resource *positioner, uint32_t token)
{
    CAST_OBJECT_AND_CALL_FUNC(WaylandXdgPopup, resource,
        "IWaylandXdgPopup::Reposition: failed to find object.", Reposition, positioner, token);
}

OHOS::sptr<WaylandXdgPopup> WaylandXdgPopup::Create(const OHOS::sptr<WaylandXdgSurface> &xdgSurface,
    const OHOS::sptr<WaylandXdgSurface> &parentXdgSurface,
    const OHOS::sptr<WaylandXdgPositioner> &positioner, uint32_t id,
    OHOS::sptr<OHOS::Rosen::WindowOption> windowOption)
{
    if (xdgSurface == nullptr) {
        LOG_ERROR("WaylandXdgPopup::Create: xdgSurface is nullptr.");
        return nullptr;
    }

    if (parentXdgSurface == nullptr) {
        LOG_ERROR("WaylandXdgPopup::Create: parentXdgSurface is nullptr.");
        return nullptr;
    }

    if (positioner == nullptr) {
        LOG_ERROR("WaylandXdgPopup::Create: positioner is nullptr.");
        return nullptr;
    }

    OHOS::Rosen::Rect anchorRect = positioner->GetAnchorRect();
    OHOS::Rosen::Rect parentRect = parentXdgSurface->GetRect();
    Size size = positioner->GetSize();
    Offset offset = positioner->GetOffset();
    OHOS::Rosen::Rect rect = {offset.x + parentRect.posX_, offset.y + parentRect.posY_, size.width, size.height};

    switch (positioner->GetAnchor()) {
        case XDG_POSITIONER_ANCHOR_TOP:
        case XDG_POSITIONER_ANCHOR_TOP_LEFT:
        case XDG_POSITIONER_ANCHOR_TOP_RIGHT:
            rect.posY_ += anchorRect.posY_;
            break;
        case XDG_POSITIONER_ANCHOR_BOTTOM:
        case XDG_POSITIONER_ANCHOR_BOTTOM_LEFT:
        case XDG_POSITIONER_ANCHOR_BOTTOM_RIGHT:
            rect.posY_ += anchorRect.posY_ + anchorRect.height_;
            break;
        default:
            rect.posY_ += anchorRect.posY_ + anchorRect.height_ / 2;
    }

    switch (positioner->GetAnchor()) {
        case XDG_POSITIONER_ANCHOR_LEFT:
        case XDG_POSITIONER_ANCHOR_TOP_LEFT:
        case XDG_POSITIONER_ANCHOR_BOTTOM_LEFT:
            rect.posX_ += anchorRect.posX_;
            break;
        case XDG_POSITIONER_ANCHOR_RIGHT:
        case XDG_POSITIONER_ANCHOR_TOP_RIGHT:
        case XDG_POSITIONER_ANCHOR_BOTTOM_RIGHT:
            rect.posX_ += anchorRect.posX_ + anchorRect.width_;
            break;
        default:
            rect.posX_ += anchorRect.posX_ + anchorRect.width_ / 2;
    }

    switch (positioner->GetGravity()) {
        case XDG_POSITIONER_GRAVITY_TOP:
        case XDG_POSITIONER_GRAVITY_TOP_LEFT:
        case XDG_POSITIONER_GRAVITY_TOP_RIGHT:
            rect.posY_ -= rect.height_;
            break;
        case XDG_POSITIONER_GRAVITY_BOTTOM:
        case XDG_POSITIONER_GRAVITY_BOTTOM_LEFT:
        case XDG_POSITIONER_GRAVITY_BOTTOM_RIGHT:
            rect.posY_ = rect.posY_;
            break;
        default:
            rect.posY_ -= rect.height_ / 2;
    }

    switch (positioner->GetGravity()) {
        case XDG_POSITIONER_GRAVITY_LEFT:
        case XDG_POSITIONER_GRAVITY_TOP_LEFT:
        case XDG_POSITIONER_GRAVITY_BOTTOM_LEFT:
            rect.posX_ -= rect.width_;
            break;
        case XDG_POSITIONER_GRAVITY_RIGHT:
        case XDG_POSITIONER_GRAVITY_TOP_RIGHT:
        case XDG_POSITIONER_GRAVITY_BOTTOM_RIGHT:
            rect.posX_ = rect.posX_;
            break;
        default:
            rect.posX_ -= rect.width_ / 2;
    }

    windowOption->SetWindowRect(rect);
    windowOption->SetFocusable(false);
    windowOption->SetTouchable(false);
    auto xdgPopUp = OHOS::sptr<WaylandXdgPopup>(new WaylandXdgPopup(xdgSurface, parentXdgSurface, positioner, id));
    WaylandObjectsPool::GetInstance().AddObject(ObjectId(xdgPopUp->WlClient(), xdgPopUp->Id()), xdgPopUp);
    return xdgPopUp;
}

WaylandXdgPopup::WaylandXdgPopup(const OHOS::sptr<WaylandXdgSurface> &xdgSurface,
    const OHOS::sptr<WaylandXdgSurface> &parentXdgSurface,
    const OHOS::sptr<WaylandXdgPositioner> &positioner, uint32_t id)
    : WaylandResourceObject(xdgSurface->WlClient(), &xdg_popup_interface, xdgSurface->Version(),
      id, &IWaylandXdgPopup::impl_),
      xdgSurface_(xdgSurface),
      parentXdgSurface_(parentXdgSurface)
{
    LOG_DEBUG("WaylandXdgPopup  ctor.");
}

WaylandXdgPopup::~WaylandXdgPopup() noexcept
{
    LOG_DEBUG("WaylandXdgPopup  dtor.");
}

void WaylandXdgPopup::SendConfigure()
{
    xdg_popup_send_configure(WlResource(), rect_.posX_, rect_.posY_, rect_.width_, rect_.height_);
}

void WaylandXdgPopup::SetRect(OHOS::Rosen::Rect rect)
{
    rect_ = rect;
    SendConfigure();
}

void WaylandXdgPopup::SetWindow(OHOS::sptr<OHOS::Rosen::Window> window)
{
    window_ = window;
}

void WaylandXdgPopup::Grab(struct wl_resource *seat, uint32_t serial)
{}

void WaylandXdgPopup::Reposition(struct wl_resource *positioner, uint32_t token)
{}
} // namespace Wayland
} // namespace FT
