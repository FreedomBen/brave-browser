/* Copyright (c) 2019 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef BRAVE_BROWSER_BRAVE_DRM_TAB_HELPER_H_
#define BRAVE_BROWSER_BRAVE_DRM_TAB_HELPER_H_

#include "content/public/browser/web_contents_binding_set.h"
#include "content/public/browser/web_contents_observer.h"
#include "content/public/browser/web_contents_user_data.h"
#include "brave/third_party/blink/public/platform/brave_drm.mojom.h"
#include "third_party/widevine/cdm/buildflags.h"

// Reacts to DRM content detected on the renderer side.
class BraveDrmTabHelper final
    : public content::WebContentsObserver,
      public content::WebContentsUserData<BraveDrmTabHelper>,
      public blink::mojom::BraveDRM {
 public:
  explicit BraveDrmTabHelper(content::WebContents* contents);
  ~BraveDrmTabHelper() override;

  bool ShouldShowWidevineOptIn() const;

  // content::WebContentsObserver
  void DidStartNavigation(
      content::NavigationHandle* navigation_handle) override;

  // blink::mojom::BraveDRM
  void OnWidevineKeySystemAccessRequest() override;

  WEB_CONTENTS_USER_DATA_KEY_DECL();

 private:
  content::WebContentsFrameBindingSet<blink::mojom::BraveDRM> bindings_;

#if BUILDFLAG(ENABLE_WIDEVINE_CDM_COMPONENT) || BUILDFLAG(BUNDLE_WIDEVINE_CDM)
  // Permission request is done only once during the navigation. If user
  // chooses dismiss/deny, additional request is added again only when new
  // main frame navigation is started.
  bool is_permission_requested_ = false;
#endif

  // True if we are notified that a page requested widevine availability.
  bool is_widevine_requested_ = false;
};

#endif  // BRAVE_BROWSER_BRAVE_DRM_TAB_HELPER_H_