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

#include "wayland_xdg_surface.h"

#include "wayland_objects_pool.h"
#include "wayland_xdg_toplevel.h"
#include "wayland_xdg_wm_base.h"

namespace FT {
namespace Wayland {
namespace {
    constexpr HiLogLabel LABEL = {LOG_CORE, HILOG_DOMAIN_WAYLAND, "WaylandXdgSurface"};
}

struct xdg_surface_interface IWaylandXdgSurface::impl_ = {
    .destroy = &WaylandResourceObject::DefaultDestroyResource,
    .get_toplevel = GetToplevel,
    .get_popup = GetPopup,
    .set_window_geometry = SetWindowGeometry,
    .ack_configure = AckConfigure};

void IWaylandXdgSurface::GetToplevel(struct wl_client *client, struct wl_resource *resource, uint32_t id)
{
    CAST_OBJECT_AND_CALL_FUNC(WaylandXdgSurface, resource,
        "WaylandXdgSurface::GetToplevel: failed to find object.", GetToplevel, id);
}

void IWaylandXdgSurface::GetPopup(struct wl_client *client, struct wl_resource *resource,
    uint32_t id, struct wl_resource *parent, struct wl_resource *positioner)
{
    CAST_OBJECT_AND_CALL_FUNC(WaylandXdgSurface, resource,
        "WaylandXdgSurface::GetPopup: failed to find object.", GetPopup, id, parent, positioner);
}

void IWaylandXdgSurface::SetWindowGeometry(struct wl_client *client, struct wl_resource *resource,
    int32_t x, int32_t y, int32_t width, int32_t height)
{
    CAST_OBJECT_AND_CALL_FUNC(WaylandXdgSurface, resource,
        "WaylandXdgSurface::SetWindowGeometry: failed to find object.", SetWindowGeometry, x, y, width, height);
}

void IWaylandXdgSurface::AckConfigure(struct wl_client *client, struct wl_resource *resource, uint32_t serial)
{
}

OHOS::sptr<WaylandXdgSurface> WaylandXdgSurface::Create(const OHOS::sptr<WaylandXdgWmObject> &xdgWm,
    const OHOS::sptr<WaylandSurface> &surface, uint32_t id)
{
    if (xdgWm == nullptr) {
        LOG_WARN("xdgWm is nullptr");
        return nullptr;
    }
    if (surface == nullptr) {
        LOG_WARN("surface is nullptr");
        return nullptr;
    }

    auto xdgSurface = OHOS::sptr<WaylandXdgSurface>(new WaylandXdgSurface(xdgWm, surface, id));
    WaylandObjectsPool::GetInstance().AddObject(ObjectId(xdgSurface->WlClient(), xdgSurface->Id()), xdgSurface);
    xdgSurface->windowOption_ = surface->GetWindowOption();
    xdgSurface->windowOptionExt_ = surface->GetWindowOptionExt();

    return xdgSurface;
}

WaylandXdgSurface::WaylandXdgSurface(const OHOS::sptr<WaylandXdgWmObject> &xdgWm,
    const OHOS::sptr<WaylandSurface> &surface, uint32_t id)
    : WaylandResourceObject(xdgWm->WlClient(), &xdg_surface_interface, xdgWm->Version(),
      id, &IWaylandXdgSurface::impl_),
      xdgWm_(xdgWm),
      surface_(surface)
{
    surface->AddCommitCallback([this]() { OnSurfaceCommit(); });
    surface->AddRectCallback([this](OHOS::Rosen::Rect rect) { OnSurfaceRect(rect); });
    surface->AddWindowCreateCallback([this](OHOS::sptr<OHOS::Rosen::Window> window) { OnWindowCreate(window); });
    windowTitle_ = std::to_string((long)((void *)this)) + std::string("-Untitled");
    LOG_DEBUG("enter : %{public}s.", windowTitle_.c_str());
}

WaylandXdgSurface::~WaylandXdgSurface() noexcept
{
    LOG_DEBUG("exit : %{public}s.", windowTitle_.c_str());
    if (window_ != nullptr) {
        window_->Close();
    }
}

OHOS::Rosen::Rect WaylandXdgSurface::GetRect()
{
    OHOS::Rosen::Rect rect;
    if (window_ != nullptr) {
        rect = window_->GetRect();
    }
    return rect;
}

void WaylandXdgSurface::GetToplevel(uint32_t id)
{
    LOG_DEBUG("Window %{public}s.", windowTitle_.c_str());
    toplevel_ = WaylandXdgToplevel::Create(this, id, windowOption_, windowOptionExt_);
    if (toplevel_ == nullptr) {
        LOG_ERROR("no memory");
        return;
    }

    role_ = SurfaceRole::XDG_TOPLEVEL;
}

void WaylandXdgSurface::GetPopup(uint32_t id, struct wl_resource *parent, struct wl_resource *positioner)
{
    LOG_DEBUG("Window %{public}s.", windowTitle_.c_str());
    auto parentXdgSurface = CastFromResource<WaylandXdgSurface>(parent);
    if (parentXdgSurface == nullptr) {
        LOG_ERROR("parentXdgSurface is nullptr.");
        return;
    }

    auto xdgPositioner = CastFromResource<WaylandXdgPositioner>(positioner);
    if (xdgPositioner == nullptr) {
        LOG_ERROR("xdgPositioner is nullptr.");
        return;
    }

    popUp_ = WaylandXdgPopup::Create(this, parentXdgSurface, xdgPositioner, id, windowOption_);
    if (popUp_ == nullptr) {
        LOG_ERROR("no memory");
        return;
    }
    role_ = SurfaceRole::XDG_POPUP;
}

void WaylandXdgSurface::SetWindowGeometry(int32_t x, int32_t y, int32_t width, int32_t height)
{
    LOG_DEBUG("Window %{public}s. x:%{public}d y:%{public}d width:%{public}d height:%{public}d",
        windowTitle_.c_str(), x, y, width, height);
    auto surface = surface_.promote();
    if (surface != nullptr) {
        OHOS::Rosen::Rect rect = {x, y, static_cast<uint32_t>(width), static_cast<uint32_t>(height)};
        surface->SetWindowGeometry(rect);
    }
}

void WaylandXdgSurface::AckConfigure(uint32_t serial)
{
}

void WaylandXdgSurface::OnSurfaceCommit()
{
    if (window_ == nullptr) {
        LOG_ERROR("window_ is nullptr");
        return;
    }

    if (isFirstCommit_) {
        isFirstCommit_ = false;
        OHOS::Rosen::Rect rect = window_->GetRect();
        if (role_ == SurfaceRole::XDG_TOPLEVEL) {
            auto topLevel = toplevel_.promote();
            if (topLevel != nullptr) {
                topLevel->SetRect(rect);
            }
        } else if (role_ == SurfaceRole::XDG_POPUP) {
            auto popup = popUp_.promote();
            if (popup != nullptr) {
                popup->SetRect(rect);
            }
        }
        xdg_surface_send_configure(WlResource(), wl_display_next_serial(WlDisplay()));
    }
}

void WaylandXdgSurface::OnSurfaceRect(OHOS::Rosen::Rect rect)
{
    LOG_DEBUG("Window %{public}s.", windowTitle_.c_str());
    if (role_ == SurfaceRole::XDG_TOPLEVEL) {
        auto topLevel = toplevel_.promote();
        if (topLevel != nullptr) {
            topLevel->SetRect(rect);
        }
    } else if (role_ == SurfaceRole::XDG_POPUP) {
        auto popup = popUp_.promote();
        if (popup != nullptr) {
            popup->SetRect(rect);
        }
    }
    xdg_surface_send_configure(WlResource(), wl_display_next_serial(WlDisplay()));
}

void WaylandXdgSurface::OnWindowCreate(OHOS::sptr<OHOS::Rosen::Window> window)
{
    window_ = window;
    if (role_ == SurfaceRole::XDG_TOPLEVEL) {
        auto topLevel = toplevel_.promote();
        if (topLevel != nullptr) {
            topLevel->SetWindow(window);
        }
    } else if (role_ == SurfaceRole::XDG_POPUP) {
        auto popup = popUp_.promote();
        if (popup != nullptr) {
            popup->SetWindow(window);
        }
    }
}
} // namespace Wayland
} // namespace FT
