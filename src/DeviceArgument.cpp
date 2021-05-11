#include "DeviceArgument.h"
#include "CodeGen_GPU_Dev.h"

namespace Halide {
namespace Internal {

HostClosure::HostClosure(const Stmt &s, const std::string &loop_variable) {
    if (!loop_variable.empty()) {
        ignore.push(loop_variable);
    }
    s.accept(this);
}

std::vector<DeviceArgument> HostClosure::arguments() {
    std::vector<DeviceArgument> res;
    for (const auto &v : vars) {
        debug(2) << "var: " << v.first << "\n";
        res.emplace_back(v.first, false, MemoryType::Auto, v.second, 0);
    }
    for (const auto &b : buffers) {
        debug(2) << "buffer: " << b.first << " " << b.second.size;
        if (b.second.read) {
            debug(2) << " (read)";
        }
        if (b.second.write) {
            debug(2) << " (write)";
        }
        if (b.second.memory_type == MemoryType::GPUTexture) {
            debug(2) << " <texture>";
        }
        debug(2) << " dims=" << (int)b.second.dimensions;
        debug(2) << "\n";

        DeviceArgument arg(b.first, true, b.second.memory_type, b.second.type, b.second.dimensions, b.second.size);
        arg.read = b.second.read;
        arg.write = b.second.write;
        res.push_back(arg);
    }
    return res;
}

void HostClosure::visit(const Call *op) {
    if (op->is_intrinsic(Call::image_load) ||
        op->is_intrinsic(Call::image_store)) {

        // The argument to the call is either a StringImm or a broadcasted
        // StringImm if this is part of a vectorized expression

        const StringImm *string_imm = op->args[0].as<StringImm>();
        if (!string_imm) {
            internal_assert(op->args[0].as<Broadcast>());
            string_imm = op->args[0].as<Broadcast>()->value.as<StringImm>();
        }

        internal_assert(string_imm);

        std::string bufname = string_imm->value;
        Buffer &ref = buffers[bufname];
        ref.type = op->type;
        ref.memory_type = op->is_intrinsic(Call::image_load) ||
                                  op->is_intrinsic(Call::image_store) ?
                              MemoryType::GPUTexture :
                              MemoryType::Auto;

        if (op->is_intrinsic(Call::image_load)) {
            ref.read = true;
            ref.dimensions = (op->args.size() - 2) / 2;
        } else if (op->is_intrinsic(Call::image_store)) {
            ref.write = true;
            ref.dimensions = op->args.size() - 3;
        }

        // The Func's name and the associated .buffer are mentioned in the
        // argument lists, but don't treat them as free variables.
        ScopedBinding<> p1(ignore, bufname);
        ScopedBinding<> p2(ignore, bufname + ".buffer");
        Internal::Closure::visit(op);
    } else if (op->call_type == Call::Intrinsic && op->name == "wmma_m16n16k16_mma_f32_f32") {
        const Variable *var_a = op->args[0].as<Variable>();
        std::string name_a = var_a->name;
        Buffer &ref_a = buffers[name_a];
        ref_a.type = var_a->type;
        ref_a.dimensions = 2;
        ref_a.read = true;

        op->args[1].accept(this);
        op->args[2].accept(this);

        const Variable *var_b = op->args[3].as<Variable>();
        std::string name_b = var_b->name;
        Buffer &ref_b = buffers[name_b];
        ref_b.type = var_b->type;
        ref_b.dimensions = 2;
        ref_b.read = true;

        op->args[4].accept(this);
        op->args[5].accept(this);

        const Variable *var_c = op->args[6].as<Variable>();
        std::string name_c = var_c->name;
        Buffer &ref_c = buffers[name_c];
        ref_c.type = var_c->type;
        ref_c.dimensions = 2;
        ref_c.read = true;

        op->args[7].accept(this);
        op->args[8].accept(this);

        const Variable *var_d = op->args[9].as<Variable>();
        std::string name_d = var_d->name;
        Buffer &ref_d = buffers[name_d];
        ref_c.type = var_d->type;
        ref_c.dimensions = 2;
        ref_c.write = true;

        op->args[10].accept(this);
        op->args[11].accept(this);
    } else if (op->call_type == Call::Intrinsic && starts_with(op->name, "wmma.")) {
        const bool is_load = starts_with(op->name, "wmma.m16n16k16.load.");
        const bool is_store = starts_with(op->name, "wmma.m16n16k16.store.");

        if (is_load || is_store) {
            const Variable *var = op->args[0].as<Variable>();
            Buffer &ref = buffers[var->name];
            ref.type = var->type;
            ref.dimensions = 2;
            ref.read = is_load;
            ref.write = is_store;

            for (size_t i = 1; i < op->args.size(); ++i) {
                op->args[i].accept(this);
            }
        } else {
            Internal::Closure::visit(op);
        }
    } else {
        Internal::Closure::visit(op);
    }
}

void HostClosure::visit(const For *loop) {
    if (CodeGen_GPU_Dev::is_gpu_var(loop->name)) {
        // The size of the threads and blocks is not part of the closure
        ScopedBinding<> p(ignore, loop->name);
        loop->body.accept(this);
    } else {
        Internal::Closure::visit(loop);
    }
}

}  // namespace Internal
}  // namespace Halide
