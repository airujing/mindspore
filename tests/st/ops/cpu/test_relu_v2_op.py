# Copyright 2022 Huawei Technologies Co., Ltd
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
# http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
# ============================================================================

import numpy as np
import pytest

import mindspore.context as context
import mindspore.nn as nn
from mindspore import Tensor
from mindspore.common.initializer import initializer
from mindspore.common.parameter import Parameter
from mindspore.ops import operations as P

context.set_context(mode=context.GRAPH_MODE, device_target='CPU')


class Net4DReLUV2(nn.Cell):
    def __init__(self):
        super(Net4DReLUV2, self).__init__()
        self.reluv2 = P.ReLUV2()
        self.x = Parameter(initializer(Tensor(np.array([[[[-1, 1, 10],
                                                          [1, -1, 1],
                                                          [10, 1, -1]]]]).astype(np.float32)), [1, 1, 3, 3]), name='x')

    def construct(self):
        return self.reluv2(self.x)


@pytest.mark.level0
@pytest.mark.platform_x86_cpu
@pytest.mark.env_onecard
def test_relu_v2():
    """
    Feature: ReLUV2 cpu kernel
    Description: test the rightness of ReLUV2 cpu kernel, note: the mask output is useless.
    Expectation: the output[0] is same as numpy
    """
    reluv2 = Net4DReLUV2()
    output, _ = reluv2()
    expect = np.array([[[[0, 1, 10],
                         [1, 0, 1],
                         [10, 1, 0]]]]).astype(np.float32)
    assert (output.asnumpy() == expect).all()
