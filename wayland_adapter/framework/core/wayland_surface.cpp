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

#include <linux/input.h>
#include "wayland_surface.h"

#include "wayland_objects_pool.h"
#include "wayland_event_loop.h"
#include "ui/rs_surface_extractor.h"
#include "wayland_region.h"
#include "wayland_seat.h"
#include "input_manager.h"

namespace FT {
namespace Wayland {
namespace {
    constexpr HiLogLabel LABEL = {LOG_CORE, HILOG_DOMAIN_WAYLAND, "WaylandSurface"};
    constexpr uint32_t US_TO_MS = 1000;
}

#ifdef ENABLE_GPU
static std::unique_ptr<OHOS::Rosen::RenderContext> render_context_global;
#endif

class InputEventConsumer : public OHOS::Rosen::IInputEventConsumer
{
public:
    InputEventConsumer(OHOS::wptr<WaylandSurface> wlSurface)
    {
        wlSurface_ = wlSurface;
    }

    bool OnInputEvent(const std::shared_ptr<OHOS::MMI::KeyEvent>& keyEvent) const override;
    bool OnInputEvent(const std::shared_ptr<OHOS::MMI::AxisEvent>& axisEvent) const override;
    bool OnInputEvent(const std::shared_ptr<OHOS::MMI::PointerEvent>& pointerEvent) const override;
private:
    OHOS::wptr<WaylandSurface> wlSurface_ = nullptr;
    int32_t MapPointerActionButton(int32_t PointerActionButtonType) const;
    int32_t MapKeyAction(int32_t keyAction) const;
    const std::map<uint32_t, int32_t> ptrActionMap_ = {
        {OHOS::MMI::PointerEvent::MOUSE_BUTTON_LEFT,   BTN_LEFT},
        {OHOS::MMI::PointerEvent::MOUSE_BUTTON_RIGHT,  BTN_RIGHT},
    };
    const std::map<uint32_t, int32_t> keyActionMap_ = {
        {OHOS::MMI::KeyEvent::KEY_ACTION_UP, WL_KEYBOARD_KEY_STATE_RELEASED},
        {OHOS::MMI::KeyEvent::KEY_ACTION_DOWN, WL_KEYBOARD_KEY_STATE_PRESSED},
    };
    const int32_t INVALID_KEYACTION = -1;
};

int32_t InputEventConsumer::MapKeyAction(int32_t keyAction) const
{
    auto it = keyActionMap_.find(keyAction);
    if (it == keyActionMap_.end()) {
        return INVALID_KEYACTION;
    }
    return it->second;
}

int32_t InputEventConsumer::MapPointerActionButton(int32_t PointerActionButtonType) const
{
    auto it = ptrActionMap_.find(PointerActionButtonType);
    if (it == ptrActionMap_.end()) {
        return OHOS::MMI::PointerEvent::BUTTON_NONE;
    }
    return it->second;
}

bool InputEventConsumer::OnInputEvent(const std::shared_ptr<OHOS::MMI::KeyEvent>& keyEvent) const
{
    keyEvent->MarkProcessed();
    WaylandEventLoop::GetInstance().QueueToLoop([this, keyEvent]{
        OHOS::sptr<WaylandSeat> wlSeat = WaylandSeat::GetWaylandSeatGlobal();
        if (wlSeat == nullptr) {
            return;
        }
        auto wlSurface = wlSurface_.promote();
        if (wlSurface == nullptr) {
            return;
        }
        std::list<OHOS::sptr<WaylandKeyboard>> keyboardList;
        wlSeat->GetKeyboardResource(wlSurface->WlClient(), keyboardList);
        int32_t keyAction = MapKeyAction(keyEvent->GetKeyAction());
        if (keyAction == INVALID_KEYACTION) {
            return;
        }

        for (auto &keyboard : keyboardList) {
            keyboard->OnKeyboardKey(keyEvent->GetKeyCode(), keyAction, keyEvent->GetActionTime() / US_TO_MS);
        }
        wl_display_flush_clients(wlSurface->WlDisplay());
    });

    return true;
}

bool InputEventConsumer::OnInputEvent(const std::shared_ptr<OHOS::MMI::AxisEvent>& axisEvent) const
{
    axisEvent->MarkProcessed();
    return true;
}

bool InputEventConsumer::OnInputEvent(const std::shared_ptr<OHOS::MMI::PointerEvent>& pointerEvent) const
{
    pointerEvent->MarkProcessed();
    WaylandEventLoop::GetInstance().QueueToLoop([this, pointerEvent]{
        OHOS::sptr<WaylandSeat> wlSeat = WaylandSeat::GetWaylandSeatGlobal();
        if (wlSeat == nullptr) {
            return;
        }
        auto wlSurface = wlSurface_.promote();
        if (wlSurface == nullptr) {
            return;
        }
        std::list<OHOS::sptr<WaylandPointer>> pointerList;
        std::list<OHOS::sptr<WaylandKeyboard>> keyboardList;
        wlSeat->GetPointerResource(wlSurface->WlClient(), pointerList);
        wlSeat->GetKeyboardResource(wlSurface->WlClient(), keyboardList);

        OHOS::MMI::PointerEvent::PointerItem pointerItem;
        int32_t pointId = pointerEvent->GetPointerId();
        if (!pointerEvent->GetPointerItem(pointId, pointerItem)) {
            LOG_WARN("GetPointerItem fail");
            return;
        }

        OHOS::Rosen::Rect rect = wlSurface->GetWindowGeometry();
        if (rect.posX_ >= 0 && rect.posY_ >= 0 && rect.width_ > 0 && rect.height_ > 0) {
            pointerItem.SetWindowX(pointerItem.GetWindowX() + rect.posX_);
            pointerItem.SetWindowY(pointerItem.GetWindowY() + rect.posY_);
        }

        if (wlSeat->IsHotPlugIn()) {
            for (auto &pointer : pointerList) {
                pointer->OnPointerEnter(pointerItem.GetWindowX(), pointerItem.GetWindowY(), wlSurface->WlResource());
            }
            for (auto &keyboard : keyboardList) {
                keyboard->OnKeyboardEnter(wlSurface->WlResource());
            }
            wlSeat->ResetHotPlugIn();
        }

        if (pointerEvent->GetPointerAction() ==  OHOS::MMI::PointerEvent::POINTER_ACTION_ENTER_WINDOW) {
            for (auto &pointer : pointerList) {
                pointer->OnPointerEnter(pointerItem.GetWindowX(), pointerItem.GetWindowY(), wlSurface->WlResource());
            }
            for (auto &keyboard : keyboardList) {
                keyboard->OnKeyboardEnter(wlSurface->WlResource());
            }
        } else if (pointerEvent->GetPointerAction() ==  OHOS::MMI::PointerEvent::POINTER_ACTION_LEAVE_WINDOW) {
            for (auto &pointer : pointerList) {
                pointer->OnPointerLeave(wlSurface->WlResource());
            }
            for (auto &keyboard : keyboardList) {
                keyboard->OnKeyboardLeave(wlSurface->WlResource());
            }
        } else if (pointerEvent->GetPointerAction() == OHOS::MMI::PointerEvent::POINTER_ACTION_BUTTON_DOWN ||
            pointerEvent->GetPointerAction() == OHOS::MMI::PointerEvent::POINTER_ACTION_BUTTON_UP) {
            int32_t buttonId = MapPointerActionButton(pointerEvent->GetButtonId());
            if (buttonId != OHOS::MMI::PointerEvent::BUTTON_NONE) {
                for (auto &pointer : pointerList) {
                    pointer->OnPointerButton(pointerEvent->GetActionTime() / US_TO_MS, buttonId, pointerItem.IsPressed());
                }
            }
        } else if (pointerEvent->GetPointerAction() == OHOS::MMI::PointerEvent::POINTER_ACTION_MOVE) {
            for (auto &pointer : pointerList) {
                pointer->OnPointerMotionAbsolute(pointerEvent->GetActionTime() / US_TO_MS, pointerItem.GetWindowX(), pointerItem.GetWindowY());
            }
        }
        wl_display_flush_clients(wlSurface->WlDisplay());
    });
    return true;
}


struct wl_surface_interface IWaylandSurface::impl_ = {
    .destroy = &WaylandResourceObject::DefaultDestroyResource,
    .attach = Attach,
    .damage = Damage,
    .frame = Frame,
    .set_opaque_region = SetOpaqueRegion,
    .set_input_region = SetInputRegion,
    .commit = Commit,
    .set_buffer_transform = SetBufferTransform,
    .set_buffer_scale = SetBufferScale,
    .damage_buffer = DamageBuffer,
    .offset = Offset};

void IWaylandSurface::Attach(struct wl_client *client, struct wl_resource *surfaceResource,
    struct wl_resource *bufferResource, int32_t x, int32_t y)
{
    CAST_OBJECT_AND_CALL_FUNC(WaylandSurface, surfaceResource,
        "IWaylandSurface::Attach: failed to find object.", Attach, bufferResource, x, y);
}

void IWaylandSurface::Damage(struct wl_client *client, struct wl_resource *resource,
    int32_t x, int32_t y, int32_t width, int32_t height)
{
    CAST_OBJECT_AND_CALL_FUNC(WaylandSurface, resource,
        "IWaylandSurface::Damage: failed to find object.", Damage, x, y, width, height);
}

void IWaylandSurface::Frame(struct wl_client *client, struct wl_resource *resource, uint32_t callback)
{
    CAST_OBJECT_AND_CALL_FUNC(WaylandSurface, resource,
        "IWaylandSurface::Frame: failed to find object.", Frame, callback);
}

void IWaylandSurface::SetOpaqueRegion(struct wl_client *client,
    struct wl_resource *surfaceResource, struct wl_resource *regionResource)
{
    CAST_OBJECT_AND_CALL_FUNC(WaylandSurface, surfaceResource,
        "IWaylandSurface::SetOpaqueRegion: failed to find object.", SetOpaqueRegion, regionResource);
}

void IWaylandSurface::SetInputRegion(struct wl_client *client,
    struct wl_resource *surfaceResource, struct wl_resource *regionResource)
{
    CAST_OBJECT_AND_CALL_FUNC(WaylandSurface, surfaceResource,
        "IWaylandSurface::SetInputRegion: failed to find object.", SetInputRegion, regionResource);
}

void IWaylandSurface::Commit(struct wl_client *client, struct wl_resource *resource)
{
    CAST_OBJECT_AND_CALL_FUNC(WaylandSurface, resource, "IWaylandSurface::Commit: failed to find object.", Commit);
}

void IWaylandSurface::SetBufferTransform(struct wl_client *client, struct wl_resource *resource, int32_t transform)
{
    CAST_OBJECT_AND_CALL_FUNC(WaylandSurface, resource,
        "IWaylandSurface::SetBufferTransform: failed to find object.", SetBufferTransform, transform);
}

void IWaylandSurface::SetBufferScale(struct wl_client *client, struct wl_resource *resource, int32_t scale)
{
    CAST_OBJECT_AND_CALL_FUNC(WaylandSurface, resource,
        "IWaylandSurface::SetBufferScale: failed to find object.", SetBufferScale, scale);
}

void IWaylandSurface::DamageBuffer(struct wl_client *client, struct wl_resource *resource,
    int32_t x, int32_t y, int32_t width, int32_t height)
{
    CAST_OBJECT_AND_CALL_FUNC(WaylandSurface, resource,
        "IWaylandSurface::DamageBuffer: failed to find object.", DamageBuffer, x, y, width, height);
}

void IWaylandSurface::Offset(struct wl_client *client, struct wl_resource *resource, int32_t x, int32_t y)
{
    CAST_OBJECT_AND_CALL_FUNC(WaylandSurface, resource,
        "IWaylandSurface::Offset: failed to find object.", Offset, x, y);
}

OHOS::sptr<WaylandSurface> WaylandSurface::Create(struct wl_client *client,
    struct wl_resource *parent, uint32_t version, uint32_t id)
{
    if (client == nullptr) {
        LOG_ERROR("Invalid client parameter");
        return nullptr;
    }

    auto surface = OHOS::sptr<WaylandSurface>(new WaylandSurface(client, parent, version, id));
    if (surface == nullptr) {
        LOG_ERROR("Failed to create WaylandSurface");
        return nullptr;
    }
    WaylandObjectsPool::GetInstance().AddObject(ObjectId(surface->WlClient(), surface->Id()), surface);
    return surface;
}

WaylandSurface::WaylandSurface(struct wl_client *client, struct wl_resource *parent, uint32_t version, uint32_t id)
    : WaylandResourceObject(client, &wl_surface_interface, version, id, &IWaylandSurface::impl_),
      parent_(parent)
{
    windowOptionExt_ = std::make_shared<WindowOptionExt>();
    windowOption_ = new OHOS::Rosen::WindowOption();
    windowOption_->SetWindowType(OHOS::Rosen::WindowType::APP_WINDOW_BASE);
    windowOption_->SetWindowMode(OHOS::Rosen::WindowMode::WINDOW_MODE_FLOATING);
    windowOption_->SetMainHandlerAvailable(false);
}

WaylandSurface::~WaylandSurface() noexcept
{
    if (window_ != nullptr) {
        if (listener_ != nullptr) {
            window_->UnregisterWindowChangeListener(listener_);
            listener_ = nullptr;
        }
        window_->Destroy();
        window_ = nullptr;
    }
}

void WaylandSurface::AddCommitCallback(SurfaceCommitCallback callback)
{
    commitCallbacks_.push_back(std::move(callback));
}

void WaylandSurface::AddRectCallback(SurfaceRectCallback callback)
{
    rectCallbacks_.push_back(std::move(callback));
}

void WaylandSurface::AddWindowCreateCallback(WindowCreateCallback callback)
{
    windowCreatebacks_.push_back(std::move(callback));
}

void WaylandSurface::Attach(struct wl_resource *bufferResource, int32_t x, int32_t y)
{
    if (new_.buffer != nullptr) {
        wl_callback_send_done(new_.buffer, 0);
    }

    new_.buffer = bufferResource;
    new_.offsetX = x;
    new_.offsetY = y;
}

void WaylandSurface::Damage(int32_t x, int32_t y, int32_t width, int32_t height)
{
    new_.damage.posX_ = x;
    new_.damage.posY_ = y;
    new_.damage.width_ = static_cast<uint32_t>(width);
    new_.damage.height_ = static_cast<uint32_t>(height);
}

void WaylandSurface::Frame(uint32_t callback)
{
    bool pending = false;
    if (new_.cb != nullptr) {
        pending = true;
        LOG_WARN("Duplicate frame request");
    }

    auto cb = FrameCallback::Create(WlClient(), WAYLAND_VERSION_MAJOR, callback);
    if (cb == nullptr) {
        LOG_ERROR("No memory");
        return;
    }

    WaylandObjectsPool::GetInstance().AddObject(ObjectId(cb->WlClient(), cb->Id()), cb);
    if (pending) {
        pengindCb_.push_back(cb);
        return;
    }
    new_.cb = cb;
}

void WaylandSurface::SetOpaqueRegion(struct wl_resource *regionResource)
{
    if (regionResource == nullptr) {
        LOG_ERROR("RegionResource is nullptr");
        return;
    }

    auto region = CastFromResource<WaylandRegion>(regionResource);
    if (region == nullptr) {
        LOG_ERROR("Failed to cast WaylandRegion from regionResource, maybe resource is not valid.");
        return;
    }

    new_.opaqueRegion = region->GetRect();
    LOG_DEBUG("SetOpaqueRegion, rect: x %{public}d, y %{public}d, width %{public}d, height %{public}d.",
        new_.opaqueRegion.posX_, new_.opaqueRegion.posY_, new_.opaqueRegion.width_, new_.opaqueRegion.height_);
}

void WaylandSurface::SetInputRegion(struct wl_resource *regionResource)
{
    if (regionResource == nullptr) {
        LOG_ERROR("RegionResource is nullptr");
        return;
    }

    auto region = CastFromResource<WaylandRegion>(regionResource);
    if (region == nullptr) {
        LOG_ERROR("Failed to cast WaylandRegion from regionResource, maybe resource is not valid.");
        return;
    }

    new_.inputRegion = region->GetRect();
    LOG_DEBUG("SetInputRegion, rect: x %{public}d, y %{public}d, width %{public}d, height %{public}d.",
        new_.inputRegion.posX_, new_.inputRegion.posY_, new_.inputRegion.width_, new_.inputRegion.height_);
}

void WaylandSurface::Commit()
{
    if (isPointerSurface_) {
        return; // it is pointer surface, we do not handle commit!
    }

    if (!isSubSurface_ && window_ == nullptr) {
        CreateWindow();
    }

    HandleCommit();
    for (auto &cb : commitCallbacks_) {
        cb();
    }
}

void WaylandSurface::SetBufferTransform(int32_t transform)
{
    new_.transform = static_cast<wl_output_transform>(transform);
}

void WaylandSurface::SetBufferScale(int32_t scale)
{
    new_.scale = scale;
}

void WaylandSurface::DamageBuffer(int32_t x, int32_t y, int32_t width, int32_t height)
{
    new_.damageBuffer.posX_ = x;
    new_.damageBuffer.posY_ = y;
    new_.damageBuffer.width_ = static_cast<uint32_t>(width);
    new_.damageBuffer.height_ = static_cast<uint32_t>(height);
}

void WaylandSurface::Offset(int32_t x, int32_t y)
{
    new_.offsetX = x;
    new_.offsetY = y;
}

void WaylandSurface::HandleCommit() {
    int32_t timeMs = 0;
    struct timespec ts = { 0, 0 };
    if (clock_gettime(CLOCK_MONOTONIC, &ts) == 0) {
        timeMs = (ts.tv_sec * 1000) + (ts.tv_nsec / 1000000);
    } else {
        LOG_ERROR("Failed to clock_gettime");
    }

    if (new_.buffer != nullptr) {
        wl_shm_buffer *shm = wl_shm_buffer_get(new_.buffer);
        if (shm == nullptr) {
            LOG_ERROR("wl_shm_buffer_get fail");
            wl_callback_send_done(new_.buffer, 0);
            return;
        }

        wl_shm_buffer_begin_access(shm);
        CopyBuffer(shm);
        wl_shm_buffer_end_access(shm);

        wl_callback_send_done(new_.buffer, timeMs);
        new_.buffer = nullptr;
    }

    if (new_.cb != nullptr) {
        wl_callback_send_done(new_.cb->WlResource(), timeMs);
        wl_resource_destroy(new_.cb->WlResource());
        new_.cb = nullptr;
        for (auto &cb : pengindCb_) {
            wl_callback_send_done(cb->WlResource(), timeMs);
            wl_resource_destroy(cb->WlResource());
        }
        pengindCb_.clear();
    }

    old_ = new_;
    new_.Reset();
}

void WaylandSurface::CheckIsPointerSurface()
{
    OHOS::sptr<WaylandSeat> wlSeat = WaylandSeat::GetWaylandSeatGlobal();
    if (wlSeat == nullptr) {
        return;
    }

    std::list<OHOS::sptr<WaylandPointer>> pointerList;
    wlSeat->GetPointerResource(WlClient(), pointerList);
    for (auto &pointer : pointerList) {
        isPointerSurface_ = pointer->IsCursorSurface(WlResource());
        if (isPointerSurface_) {
            break;
        }
    }

    LOG_DEBUG("This surface Pointer Surface: %{public}d", isPointerSurface_);
}

void WaylandSurface::CreateWindow()
{
    CheckIsPointerSurface();
    if (isPointerSurface_) {
        return;
    }

    static int count = 0;
    std::string windowName = "WaylandWindow" + std::to_string(count++);
    window_ = OHOS::Rosen::Window::Create(windowName, windowOption_);
    if (window_ == nullptr) {
        LOG_ERROR("Window::Create failed");
        return;
    }
    LOG_DEBUG("Window::Create success");
    auto listener = std::make_shared<InputEventConsumer>(this);
    window_->SetInputEventConsumer(listener);
    window_->SetAPPWindowLabel(windowOptionExt_->title);
    window_->Show();

    listener_ = new WaylandWindowListener(this);
    window_->RegisterWindowChangeListener(listener_);

    surfaceNode_ = window_->GetSurfaceNode();
    if (surfaceNode_ == nullptr) {
        LOG_ERROR("GetSurfaceNode failed");
        return;
    }

    rsSurface_ = OHOS::Rosen::RSSurfaceExtractor::ExtractRSSurface(surfaceNode_);
    if (rsSurface_ == nullptr) {
        LOG_ERROR("ExtractRSSurface failed");
        return;
    }

#ifdef ENABLE_GPU
    if (render_context_global == nullptr) {
        render_context_global = std::make_unique<OHOS::Rosen::RenderContext>();
        render_context_global->InitializeEglContext();
    }
    rsSurface_->SetRenderContext(render_context_global.get());
#endif

    for (auto &cb : windowCreatebacks_) {
        cb(window_);
    }

    rect_= window_->GetRect();
    for (auto &cb : rectCallbacks_) {
        cb(rect_);
    }

    if (windowOptionExt_->maximizeAfterShow) {
        window_->Maximize();
    } else if (windowOptionExt_->fullscreenAfterShow) {
        window_->SetFullScreen(true);
    } else if (windowOptionExt_->minimizeAfterShow) {
        window_->Minimize();
    }
}

void WaylandSurface::CopyBuffer(struct wl_shm_buffer *shm)
{
    SkColorType format = ShmFormatToSkia(wl_shm_buffer_get_format(shm));
    if (format == SkColorType::kUnknown_SkColorType) {
        LOG_ERROR("Unsupported format %{public}d", wl_shm_buffer_get_format(shm));
        return;
    }

    int32_t stride = wl_shm_buffer_get_stride(shm);
    int32_t width = wl_shm_buffer_get_width(shm);
    int32_t height = wl_shm_buffer_get_height(shm);
    if (stride <= 0 || width <= 0 || height <= 0) {
        LOG_ERROR("Invalid, stride:%{public}d width:%{public}d height:%{public}d", stride, width, height);
        return;
    }

    void *data = wl_shm_buffer_get_data(shm);
    if (data == nullptr) {
        LOG_ERROR("wl_shm_buffer_get_data fail");
        return;
    }
    SkImageInfo imageInfo = SkImageInfo::Make(width, height, format, kUnpremul_SkAlphaType);
    SkPixmap srcPixmap(imageInfo, data, stride);
    {
        std::lock_guard<std::mutex> lg(bitmapMutex_);
        srcBitmap_.installPixels(srcPixmap);
    }

    if (isSubSurface_) {
        auto surfaceParent = CastFromResource<WaylandSurface>(parentSurfaceRes_);
        if (parentSurfaceRes_ != nullptr) {
            surfaceParent->TriggerInnerCompose();
        }
        LOG_DEBUG("Return because without toplevel");
        return;
    }
    TriggerInnerCompose();
}

void WaylandSurface::OnSizeChange(const OHOS::Rosen::Rect& rect, OHOS::Rosen::WindowSizeChangeReason reason)
{
    rect_ = rect;
    for (auto &cb : rectCallbacks_) {
        cb(rect_);
    }
}

void WaylandSurface::OnModeChange(OHOS::Rosen::WindowMode mode)
{
    LOG_DEBUG("OnModeChange, window mode is %{public}d, ignore", mode);
}

void WaylandSurface::SetWindowGeometry(OHOS::Rosen::Rect rect)
{
    LOG_DEBUG("Window x:%{public}d y:%{public}d width:%{public}d height:%{public}d",
        rect.posX_, rect.posY_, rect.width_, rect.height_);
    geometryRect_ = rect;
}

OHOS::Rosen::Rect WaylandSurface::GetWindowGeometry()
{
    return geometryRect_;
}

void WaylandSurface::AddChild(struct wl_resource *child, int32_t x, int32_t y)
{
    if (child == nullptr) {
        LOG_ERROR("AddChild with nullptr resource");
        return;
    }
    if (childs_.count(child) > 0) {
        childs_[child].offsetX = x;
        childs_[child].offsetY = y;
        return;
    }
    SubSurfaceData data;
    data.surface = child;
    data.offsetX = x;
    data.offsetY = y;
    childs_[child] = data;
    for (auto &cb : rectCallbacks_) {
        cb(rect_);
    }
}

void WaylandSurface::AddParent(struct wl_resource *parent)
{
    parentSurfaceRes_ = parent;
}

void WaylandSurface::ProcessSrcBitmap(SkCanvas* canvas, int32_t x, int32_t y)
{
    std::lock_guard<std::mutex> lg(bitmapMutex_);
    canvas->drawBitmap(srcBitmap_, x, y);
    LOG_DEBUG("Draw child offsetx %{public}d, offsety %{public}d,", x, y);
}

void WaylandSurface::TriggerInnerCompose()
{
    if (rsSurface_ == nullptr) {
        LOG_ERROR("rsSurface_ is nullptr");
        return;
    }

    if (srcBitmap_.width() == 0 || srcBitmap_.height() == 0) {
        LOG_DEBUG("srcBitmap_ is nullptr");
        return;
    }
    uint32_t width;
    uint32_t height;
    bool vailedGeometry = (geometryRect_.posX_ >= 0 && geometryRect_.posY_ >= 0 &&
                           geometryRect_.width_ > 0 && geometryRect_.height_ > 0);
    if (vailedGeometry) {
        width = geometryRect_.width_;
        height = geometryRect_.height_;
    } else {
        width = srcBitmap_.width();
        height = srcBitmap_.height();
    }
    auto framePtr = rsSurface_->RequestFrame(width, height);
    if (framePtr == nullptr) {
        LOG_ERROR("RequestFrame failed");
        return;
    }

    auto canvas = framePtr->GetCanvas();
    if (canvas == nullptr) {
        LOG_ERROR("GetCanvas failed");
        return;
    }
    canvas->clear(SK_ColorTRANSPARENT);
    if (vailedGeometry) {
        SkPaint paint;
        paint.setAntiAlias(true);
        paint.setStyle(SkPaint::kFill_Style);
        canvas->drawBitmapRect(srcBitmap_,
            SkRect::MakeXYWH(geometryRect_.posX_, geometryRect_.posY_, geometryRect_.width_, geometryRect_.height_),
            SkRect::MakeXYWH(0, 0, geometryRect_.width_, geometryRect_.height_),
            &paint);
    } else {
        canvas->drawBitmap(srcBitmap_, 0, 0);
    }
    for (auto &&[childKey, data] : childs_) {
        if (data.surface == nullptr) {
            continue;
        }
        LOG_DEBUG("Draw Child");
        auto surfaceChild = CastFromResource<WaylandSurface>(data.surface);
        if (vailedGeometry) {
            surfaceChild->ProcessSrcBitmap(canvas, data.offsetX - geometryRect_.posX_, data.offsetY - geometryRect_.posY_);
        } else {
            surfaceChild->ProcessSrcBitmap(canvas, data.offsetX, data.offsetY);
        }
    }
    rsSurface_->FlushFrame(framePtr);
}

} // namespace Wayland
} // namespace FT
