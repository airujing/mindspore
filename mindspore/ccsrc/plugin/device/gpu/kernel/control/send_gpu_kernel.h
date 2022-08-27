/**
 * Copyright 2020-2022 Huawei Technologies Co., Ltd
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef MINDSPORE_CCSRC_BACKEND_KERNEL_COMPILER_GPU_CONTROL_SEND_GPU_KERNEL_H_
#define MINDSPORE_CCSRC_BACKEND_KERNEL_COMPILER_GPU_CONTROL_SEND_GPU_KERNEL_H_

#include <vector>
#include "include/common/utils/utils.h"
#include "plugin/device/gpu/kernel/gpu_kernel.h"
#include "plugin/device/gpu/kernel/gpu_kernel_factory.h"

namespace mindspore {
namespace kernel {
class SendGpuKernelMod : public DeprecatedNativeGpuKernelMod {
 public:
  SendGpuKernelMod() {}
  ~SendGpuKernelMod() override = default;

  bool Launch(const std::vector<AddressPtr> &, const std::vector<AddressPtr> &, const std::vector<AddressPtr> &,
              void *stream_ptr) override {
    CHECK_CUDA_RET_WITH_EXCEPT(kernel_node_, cudaEventRecord(record_event_, reinterpret_cast<cudaStream_t>(stream_ptr)),
                               "Recording cuda event failed.");
    return true;
  }
  bool Init(const CNodePtr &kernel_node) override {
    MS_EXCEPTION_IF_NULL(kernel_node);
    kernel_node_ = kernel_node;
    record_event_ = reinterpret_cast<cudaEvent_t>(GetAttr<uintptr_t>(kernel_node, kAttrRecordEvent));
    InitSizeLists();
    return true;
  }

 protected:
  void InitSizeLists() override {
    input_size_list_.clear();
    output_size_list_.clear();
    workspace_size_list_.clear();
    return;
  }

 private:
  cudaEvent_t record_event_{nullptr};
};
}  // namespace kernel
}  // namespace mindspore

#endif  // MINDSPORE_CCSRC_BACKEND_KERNEL_COMPILER_GPU_CONTROL_SEND_GPU_KERNEL_H_
