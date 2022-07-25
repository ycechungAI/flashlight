/*
 * Copyright (c) Facebook, Inc. and its affiliates.
 *
 * This source code is licensed under the MIT-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "flashlight/fl/tensor/backend/onednn/OneDnnCPUStream.h"

#include <stdexcept>

#include "flashlight/fl/runtime/DeviceManager.h"

namespace fl {

OneDnnCPUStream::OneDnnCPUStream(const dnnl::engine& engine) {
  stream_ = std::make_unique<dnnl::stream>(engine);
}

X64Device& OneDnnCPUStream::device() {
  return device_;
}

const X64Device& OneDnnCPUStream::device() const {
  return device_;
}

std::shared_ptr<OneDnnCPUStream> OneDnnCPUStream::create(
    const dnnl::engine& engine) {
  if (engine.get_kind() != dnnl::engine::kind::cpu) {
    throw std::invalid_argument("OneDnnCPUStream expects a CPU engine");
  }
  const auto rawStreamPtr = new OneDnnCPUStream(engine);
  const auto stream = std::shared_ptr<OneDnnCPUStream>(rawStreamPtr);
  rawStreamPtr->device_.addStream(stream);
  return stream;
}

void OneDnnCPUStream::sync() const {
  stream_->wait();
}

void OneDnnCPUStream::relativeSync(const OneDnnCPUStream& waitOn) const {
  waitOn.sync();
}

dnnl::engine OneDnnCPUStream::engine() const {
  return stream_->get_engine();
}

const dnnl::stream& OneDnnCPUStream::handle() const {
  return *stream_;
}

} // namespace fl
