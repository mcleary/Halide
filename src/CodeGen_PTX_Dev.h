#ifndef HALIDE_CODEGEN_PTX_DEV_H
#define HALIDE_CODEGEN_PTX_DEV_H

/** \file
 * Defines the code-generator for producing CUDA host code
 */

#include <memory>

#include "IRMutator.h"

namespace Halide {

struct Target;

namespace Internal {

struct CodeGen_GPU_Dev;

class ExtractTensorCoreOperations : public IRMutator {
    using IRMutator::visit;

    int32_t M = -1;
    int32_t N = -1;
    int32_t K = -1;

    Expr block_id_x;
    Expr block_id_y;
    Expr block_dim_x;
    Expr block_dim_y;
    Expr block_size;

public:

    ExtractTensorCoreOperations();

    Stmt visit(const For *loop) override;
};

std::unique_ptr<CodeGen_GPU_Dev> new_CodeGen_PTX_Dev(const Target &target);

}  // namespace Internal
}  // namespace Halide

#endif
