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

#include "wayland_xdg_toplevel.h"

#include "wayland_objects_pool.h"

namespace FT {
namespace Wayland {
namespace {
    constexpr HiLogLabel LABEL = {LOG_CORE, HILOG_DOMAIN_WAYLAND, "WaylandXdgToplevel"};
}

struct xdg_toplevel_interface IWaylandXdgToplevel::impl_ = {
    .destroy = DestroyResource,
    .set_parent = SetParent,
    .set_title = SetTitle,
    .set_app_id = SetAppId,
    .show_window_menu = ShowWindowMenu,
    .move = Move,
    .resize = Resize,
    .set_max_size = SetMaxSize,
    .set_min_size = SetMinSize,
    .set_maximized = SetMaximized,
    .unset_maximized = UnsetMaximized,
    .set_fullscreen = SetFullscreen,
    .unset_fullscreen = UnsetFullscreen,
    .set_minimized = SetMinimized};

void IWaylandXdgToplevel::SetParent(struct wl_client *client, struct wl_resource *resource,
    struct wl_resource *parent) {}

void IWaylandXdgToplevel::SetTitle(struct wl_client *client, struct wl_resource *resource, const char *title)
{
    CAST_OBJECT_AND_CALL_FUNC(WaylandXdgToplevel, resource,
        "IWaylandXdgToplevel::SetTitle: failed to find object.", SetTitle, title);
}

void IWaylandXdgToplevel::SetAppId(struct wl_client *client, struct wl_resource *resource, const char *app_id)
{
    CAST_OBJECT_AND_CALL_FUNC(WaylandXdgToplevel, resource,
        "IWaylandXdgToplevel::SetTitle: failed to find object.", SetAppId, app_id);
}

void IWaylandXdgToplevel::ShowWindowMenu(struct wl_client *client, struct wl_resource *resource,
    struct wl_resource *seat, uint32_t serial, int32_t x, int32_t y) {}

void IWaylandXdgToplevel::Move(struct wl_client *client, struct wl_resource *resource,
    struct wl_resource *seat, uint32_t serial)
{
    CAST_OBJECT_AND_CALL_FUNC(WaylandXdgToplevel, resource,
        "IWaylandXdgToplevel::Move: failed to find object.", Move, serial);
}

void IWaylandXdgToplevel::Resize(struct wl_client *client, struct wl_resource *resource,
    struct wl_resource *seat, uint32_t serial, uint32_t edges)
{
    CAST_OBJECT_AND_CALL_FUNC(WaylandXdgToplevel, resource,
        "IWaylandXdgToplevel::Resize: failed to find object.", Resize, serial, edges);
}

void IWaylandXdgToplevel::SetMaxSize(struct wl_client *client, struct wl_resource *resource,
    int32_t width, int32_t height)
{
    CAST_OBJECT_AND_CALL_FUNC(WaylandXdgToplevel, resource,
        "IWaylandXdgToplevel::SetMaxSize: failed to find object.", SetMaxSize, width, height);
}

void IWaylandXdgToplevel::SetMinSize(struct wl_client *client, struct wl_resource *resource,
    int32_t width, int32_t height)
{
    CAST_OBJECT_AND_CALL_FUNC(WaylandXdgToplevel, resource,
        "IWaylandXdgToplevel::SetMinSize: failed to find object.", SetMinSize, width, height);
}

void IWaylandXdgToplevel::SetMaximized(struct wl_client *client, struct wl_resource *resource)
{
    CAST_OBJECT_AND_CALL_FUNC(WaylandXdgToplevel, resource,
        "IWaylandXdgToplevel::SetMaximized: failed to find object.", SetMaximized);
}

void IWaylandXdgToplevel::UnsetMaximized(struct wl_client *client, struct wl_resource *resource)
{
    CAST_OBJECT_AND_CALL_FUNC(WaylandXdgToplevel, resource,
        "IWaylandXdgToplevel::UnsetMaximized: failed to find object.", UnSetMaximized);
}

void IWaylandXdgToplevel::SetFullscreen(struct wl_client *client, struct wl_resource *resource,
    struct wl_resource *output)
{
    CAST_OBJECT_AND_CALL_FUNC(WaylandXdgToplevel, resource,
        "IWaylandXdgToplevel::SetFullscreen: failed to find object.", SetFullscreen);
}

void IWaylandXdgToplevel::UnsetFullscreen(struct wl_client *client, struct wl_resource *resource)
{
    CAST_OBJECT_AND_CALL_FUNC(WaylandXdgToplevel, resource,
        "IWaylandXdgToplevel::UnsetFullscreen: failed to find object.", UnSetFullscreen);
}

void IWaylandXdgToplevel::SetMinimized(struct wl_client *client, struct wl_resource *resource)
{
    CAST_OBJECT_AND_CALL_FUNC(WaylandXdgToplevel, resource,
        "IWaylandXdgToplevel::SetMinimized: failed to find object.", SetMinimized);
}

void IWaylandXdgToplevel::DestroyResource(struct wl_client *client, struct wl_resource *resource)
{
    CAST_OBJECT_AND_CALL_FUNC(WaylandXdgToplevel, resource,
        "IWaylandXdgToplevel::DestroyResource: failed to find object.", DestroyResource, client, resource);
}

void WaylandXdgToplevel::DestroyResource(struct wl_client *client, struct wl_resource *resource)
{
    LOG_DEBUG("Window %{public}s.", windowTitle_.c_str());
    WaylandResourceObject::DefaultDestroyResource(client, resource);
}

OHOS::sptr<WaylandXdgToplevel> WaylandXdgToplevel::Create(const OHOS::sptr<WaylandXdgSurface> &xdgSurface, uint32_t id,
    OHOS::sptr<OHOS::Rosen::WindowOption> windowOption, std::shared_ptr<WindowOptionExt> windowOptionExt)
{
    if (xdgSurface == nullptr) {
        LOG_WARN("xdgSurface is nullptr");
        return nullptr;
    }

    auto xdgTopLevel = OHOS::sptr<WaylandXdgToplevel>(new WaylandXdgToplevel(xdgSurface, id));
    WaylandObjectsPool::GetInstance().AddObject(ObjectId(xdgTopLevel->WlClient(), xdgTopLevel->Id()), xdgTopLevel);
    xdgTopLevel->windowOption_ = windowOption;
    xdgTopLevel->windowOptionExt_ = windowOptionExt;

    return xdgTopLevel;
}

WaylandXdgToplevel::WaylandXdgToplevel(const OHOS::sptr<WaylandXdgSurface> &xdgSurface, uint32_t id)
    : WaylandResourceObject(xdgSurface->WlClient(), &xdg_toplevel_interface, xdgSurface->Version(),
      id, &IWaylandXdgToplevel::impl_),
      xdgSurface_(xdgSurface)
{
    windowTitle_ = std::to_string((long)((void *)this)) + std::string("-Untitled");
    LOG_DEBUG("enter : %{public}s.", windowTitle_.c_str());
}

WaylandXdgToplevel::~WaylandXdgToplevel() noexcept
{
    LOG_DEBUG("exit : %{public}s.", windowTitle_.c_str());
}

void WaylandXdgToplevel::SetTitle(const char *title)
{
    LOG_DEBUG("Window %{public}s, set Title %{public}s.", windowTitle_.c_str(), title);
    windowTitle_ = std::to_string((long)((void *)this)) + std::string("-") + std::string(title);
    if (window_ != nullptr) {
        window_->SetAPPWindowLabel(title);
    } else {
        windowOptionExt_->title = title;
    }
}

void WaylandXdgToplevel::Move(uint32_t serial)
{
    if (window_ != nullptr) {
        window_->StartMove();
    }
}

void WaylandXdgToplevel::Resize(uint32_t serial, uint32_t edges)
{
    LOG_DEBUG("Window %{public}s.", windowTitle_.c_str());
}

void WaylandXdgToplevel::SetAppId(const char *appId)
{
    LOG_DEBUG("Window %{public}s.", windowTitle_.c_str());
    if (strstr(appId, "desktop") != nullptr && windowOption_ != nullptr) {
        windowOption_->SetWindowMode(OHOS::Rosen::WindowMode::WINDOW_MODE_FULLSCREEN);
        windowOption_->SetWindowType(OHOS::Rosen::WindowType::WINDOW_TYPE_DESKTOP);
    }
}

void WaylandXdgToplevel::SetMaxSize(int32_t width, int32_t height)
{
    maxW_ = width;
    maxH_ = height;
    if (maxW_ == 0 ||   maxH_ == 0 ){
        return;
    }
    if (minW_ == maxW_ && minH_ == maxH_) {
        windowOption_->SetDragHotZoneNone(true);
    }
    LOG_DEBUG("Window %{public}s.", windowTitle_.c_str());
}

void WaylandXdgToplevel::SetMinSize(int32_t width, int32_t height)
{
    minW_ = width;
    minH_ = height;
    if (minW_ == 0 ||   minH_ == 0 ){
        return;
    }
    if (minW_ == maxW_ && minH_ == maxH_) {
        windowOption_->SetDragHotZoneNone(true);
    }
    LOG_DEBUG("Window %{public}s.", windowTitle_.c_str());
}

void WaylandXdgToplevel::SetMaximized()
{
    LOG_DEBUG("Window %{public}s.", windowTitle_.c_str());
    if (state_.maximized) {
        LOG_DEBUG("Window %{public}s already Maximized.", windowTitle_.c_str());
        return UnSetMaximized();
    }
    state_.maximized = true;
    if (window_ != nullptr) {
        window_->Maximize();
    } else {
        windowOptionExt_->maximizeAfterShow = true;
        windowOptionExt_->fullscreenAfterShow = false;
        windowOptionExt_->minimizeAfterShow = false;
    }
}

void WaylandXdgToplevel::UnSetMaximized()
{
    LOG_DEBUG("Window %{public}s.", windowTitle_.c_str());
    state_.maximized = false;
    if (window_ != nullptr) {
        window_->Recover();
    }
}

void WaylandXdgToplevel::SetFullscreen()
{
    LOG_DEBUG("Window %{public}s.", windowTitle_.c_str());
    state_.fullscreen = true;
    if (window_ != nullptr) {
        window_->SetFullScreen(true);
    } else {
        windowOptionExt_->maximizeAfterShow = false;
        windowOptionExt_->fullscreenAfterShow = true;
        windowOptionExt_->minimizeAfterShow = false;
    }
}

void WaylandXdgToplevel::UnSetFullscreen()
{
    LOG_DEBUG("Window %{public}s.", windowTitle_.c_str());
    state_.fullscreen = false;
    if (window_ != nullptr) {
        window_->SetFullScreen(false);
    }
}

void WaylandXdgToplevel::SetMinimized()
{
    LOG_DEBUG("Window %{public}s.", windowTitle_.c_str());
    state_.minimized = true;
    if (window_ != nullptr) {
        window_->Minimize();
    } else {
        windowOptionExt_->maximizeAfterShow = false;
        windowOptionExt_->fullscreenAfterShow = false;
        windowOptionExt_->minimizeAfterShow = true;
    }
}

void WaylandXdgToplevel::SendConfigure()
{
    struct wl_array states;
    uint32_t *s;
    wl_array_init(&states);
    if (state_.maximized) {
        s = static_cast<uint32_t *>(wl_array_add(&states, sizeof(uint32_t)));
        *s = XDG_TOPLEVEL_STATE_MAXIMIZED;
    }
    if (state_.fullscreen) {
        s = static_cast<uint32_t *>(wl_array_add(&states, sizeof(uint32_t)));
        *s = XDG_TOPLEVEL_STATE_FULLSCREEN;
    }
    if (state_.resizing) {
        s = static_cast<uint32_t *>(wl_array_add(&states, sizeof(uint32_t)));
        *s = XDG_TOPLEVEL_STATE_RESIZING;
    }
    s = static_cast<uint32_t *>(wl_array_add(&states, sizeof(uint32_t)));
    *s = XDG_TOPLEVEL_STATE_ACTIVATED;
    xdg_toplevel_send_configure(WlResource(), rect_.width_, rect_.height_, &states);
    wl_array_release(&states);
}

void WaylandXdgToplevel::SetRect(OHOS::Rosen::Rect rect)
{
    rect_ = rect;
    SendConfigure();
}

void WaylandXdgToplevel::SetWindow(OHOS::sptr<OHOS::Rosen::Window> window)
{
    window_ = window;
}
} // namespace Wayland
} // namespace FT
