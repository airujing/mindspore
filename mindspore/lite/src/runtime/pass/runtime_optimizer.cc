/**
 * Copyright 2022 Huawei Technologies Co., Ltd
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

#include "src/runtime/pass/runtime_optimizer.h"

namespace mindspore::lite::pass {
RuntimeOptimizer::~RuntimeOptimizer() { passes_.clear(); }

int RuntimeOptimizer::AddPass(RuntimePassPtr pass) {
  CHECK_NULL_RETURN(pass);
  this->passes_.emplace_back(pass);
  return RET_OK;
}

int RuntimeOptimizer::Run(kernel::SubGraphKernel *subgraph, std::vector<Tensor *> *tensors) {
  CHECK_NULL_RETURN(subgraph);
  for (auto pass : this->passes_) {
    CHECK_NULL_RETURN(pass);
    auto status = pass->Run(subgraph, tensors);
    if (status != RET_OK) {
      MS_LOG(ERROR) << "Run GraphPass failed";
      return status;
    }
  }
  return RET_OK;
}
}  // namespace mindspore::lite::pass
