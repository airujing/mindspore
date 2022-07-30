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

#ifndef MINDSPORE_CCSRC_PLUGIN_DEVICE_GPU_KERNEL_NN_SMOOTH_L1_LOSS_GPU_KERNEL_H_
#define MINDSPORE_CCSRC_PLUGIN_DEVICE_GPU_KERNEL_NN_SMOOTH_L1_LOSS_GPU_KERNEL_H_

#include <vector>
#include <map>
#include <string>
#include <utility>
#include "plugin/device/gpu/kernel/gpu_kernel.h"
#include "plugin/device/gpu/kernel/gpu_kernel_factory.h"
#include "plugin/device/gpu/kernel/cuda_impl/cuda_ops/smooth_l1_loss_impl.cuh"
namespace mindspore {
namespace kernel {
constexpr auto kUnKnown = "UnKnown";
constexpr auto kSmoothL1Loss = "SmoothL1Loss";
class SmoothL1LossGpuKernelMod : public NativeGpuKernelMod {
 public:
  SmoothL1LossGpuKernelMod() {}
  ~SmoothL1LossGpuKernelMod() override = default;

  bool Init(const BaseOperatorPtr &base_operator, const std::vector<KernelTensorPtr> &inputs,
            const std::vector<KernelTensorPtr> &outputs) override;

  int Resize(
    const BaseOperatorPtr &base_operator, const std::vector<KernelTensorPtr> &inputs,
    const std::vector<KernelTensorPtr> &outputs,
    const std::map<uint32_t, tensor::TensorPtr> &inputsOnHost = std::map<uint32_t, tensor::TensorPtr>()) override;

  bool Launch(const std::vector<AddressPtr> &inputs, const std::vector<AddressPtr> &workspace,
              const std::vector<AddressPtr> &outputs, void *stream_ptr) override {
    return kernel_func_(this, inputs, workspace, outputs, stream_ptr);
  }

  std::vector<KernelAttr> GetOpSupport() override;

 protected:
  template <typename T>
  bool LaunchKernel(const std::vector<AddressPtr> &inputs, const std::vector<AddressPtr> &workspace,
                    const std::vector<AddressPtr> &outputs, void *stream_ptr);

  using SmoothL1LossFunc =
    std::function<bool(SmoothL1LossGpuKernelMod *, const std::vector<kernel::AddressPtr> &,
                       const std::vector<kernel::AddressPtr> &, const std::vector<kernel::AddressPtr> &, void *)>;

 private:
  std::string kernel_name_{};
  BaseOperatorPtr kernel_ptr_{nullptr};
  SmoothL1LossFunc kernel_func_;
  static std::vector<std::pair<KernelAttr, SmoothL1LossFunc>> func_list_;

  float beta_{1.0};
  TypeId dtype_{kTypeUnknown};
  int64_t tensor_size_{1};
  SmoothL1LossReductionMode reduction_{SmoothL1LossReductionMode::NONE};
};
}  // namespace kernel
}  // namespace mindspore

#endif  // MINDSPORE_CCSRC_PLUGIN_DEVICE_GPU_KERNEL_NN_SMOOTH_L1_LOSS_GPU_KERNEL_H_
