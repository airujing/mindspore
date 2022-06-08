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
#include "ops/matrix_solve.h"
#include "ops/op_utils.h"
#include "utils/check_convert_utils.h"
#include "abstract/ops/primitive_infer_map.h"
#include "mindapi/src/helper.h"

namespace mindspore {
namespace ops {
namespace {
abstract::ShapePtr MatrixSolveInferShape(const PrimitivePtr &primitive,
                                         const std::vector<AbstractBasePtr> &input_args) {
  MS_EXCEPTION_IF_NULL(primitive);
  auto prim_name = primitive->name();
  auto matrix_shape_ptr = input_args[kInputIndex0]->BuildShape();
  MS_EXCEPTION_IF_NULL(matrix_shape_ptr);
  auto rhs_shape_ptr = input_args[kInputIndex1]->BuildShape();
  MS_EXCEPTION_IF_NULL(rhs_shape_ptr);
  if (matrix_shape_ptr->IsDynamic() || rhs_shape_ptr->IsDynamic()) {
    return input_args[kInputIndex0]->BuildShape()->cast<abstract::ShapePtr>();
  }

  auto matrix_shape = CheckAndConvertUtils::ConvertShapePtrToShapeMap(matrix_shape_ptr)[kShape];
  auto rhs_shape = CheckAndConvertUtils::ConvertShapePtrToShapeMap(rhs_shape_ptr)[kShape];

  const int64_t matrix_dim = SizeToLong(matrix_shape.size());
  const int64_t rhs_dim = SizeToLong(rhs_shape.size());

  constexpr int64_t min_dims = 2;
  (void)CheckAndConvertUtils::CheckValue("dimension of 'matrix'", matrix_dim, kEqual, min_dims, prim_name);
  (void)CheckAndConvertUtils::CheckValue("dimension of 'matrix'", matrix_dim, kEqual, "the dimension of 'rhs'", rhs_dim,
                                         prim_name);

  constexpr int64_t kIndex1 = 1;
  constexpr int64_t kIndex2 = 2;
  (void)CheckAndConvertUtils::CheckValue("M in the shape of 'matrix' [..., M, M]",
                                         matrix_shape.at(matrix_dim - kIndex1), kEqual,
                                         matrix_shape.at(matrix_dim - kIndex2), prim_name);
  (void)CheckAndConvertUtils::CheckValue(
    "M in the shape of 'matrix' [..., M, M]", matrix_shape.at(matrix_dim - kIndex2), kEqual,
    "M in the shape of 'rhs' [..., M, K]", rhs_shape.at(matrix_dim - kIndex2), prim_name);

  for (int64_t i = 0; i < matrix_dim - kIndex2; ++i) {
    (void)CheckAndConvertUtils::CheckValue(std::to_string(i) + "th dimension of 'matrix'", matrix_shape.at(i), kEqual,
                                           std::to_string(i) + "th dimension of 'rhs'", rhs_shape.at(i), prim_name);
  }

  return rhs_shape_ptr->cast<abstract::ShapePtr>();
}

TypePtr MatrixSolveInferType(const PrimitivePtr &primitive, const std::vector<AbstractBasePtr> &input_args) {
  auto prim_name = primitive->name();
  auto matrix_dtype = input_args[kInputIndex0]->BuildType();
  auto rhs_dtype = input_args[kInputIndex1]->BuildType();

  const std::map<std::string, TypePtr> type_dict = {{"matrix type", matrix_dtype}, {"rhs type", rhs_dtype}};
  return CheckAndConvertUtils::CheckTensorTypeSame(type_dict, {kFloat16, kFloat32, kFloat64, kComplex64, kComplex128},
                                                   prim_name);
}
}  // namespace
void MatrixSolve::Init(bool adjoint) { set_adjoint(adjoint); }

void MatrixSolve::set_adjoint(bool adjoint) { (void)AddAttr(kAdjoint, api::MakeValue(adjoint)); }

bool MatrixSolve::get_adjoint() const {
  auto value_ptr = GetAttr(kAdjoint);
  return GetValue<bool>(value_ptr);
}

MIND_API_OPERATOR_IMPL(MatrixSolve, BaseOperator);

AbstractBasePtr MatrixSolveInfer(const abstract::AnalysisEnginePtr &, const PrimitivePtr &primitive,
                                 const std::vector<AbstractBasePtr> &input_args) {
  MS_EXCEPTION_IF_NULL(primitive);
  const int64_t input_num = 2;
  CheckAndConvertUtils::CheckInputArgs(input_args, kEqual, input_num, primitive->name());
  auto infer_type = MatrixSolveInferType(primitive, input_args);
  auto infer_shape = MatrixSolveInferShape(primitive, input_args);
  return abstract::MakeAbstract(infer_shape, infer_type);
}
REGISTER_PRIMITIVE_EVAL_IMPL(MatrixSolve, prim::kPrimMatrixSolve, MatrixSolveInfer, nullptr, true);
}  // namespace ops
}  // namespace mindspore
