#include <iostream>
#include <iomanip>

#include "Halide.h"

using namespace Halide;
using namespace Halide::ConciseCasts;

namespace Halide {

namespace Internal {

class MyIRVisitor : public IRVisitor {
public:
    using IRVisitor::visit;

    MyIRVisitor(std::ostream &s)
        : out(s) {
    }

    void visit(const StringImm *op) override {
        print_indent();
        out << "[String] : " << op->value << std::endl;
    }

    void visit(const IntImm *op) override {
        print_indent();
        out << "[Const]" << std::endl;
        ++level;
        print_indent();
        out << "[Type]  : " << op->type << std::endl;
        print_indent();
        out << "[Value] : " << op->value << std::endl;
        --level;
    }

    void visit(const UIntImm *op) override {
        print_indent();
        out << "[Const]" << std::endl;
        ++level;
        print_indent();
        out << "[Type]  : " << op->type << std::endl;
        print_indent();
        out << "[Value] : " << op->value << std::endl;
        --level;
    }

    void visit(const Add *op) override {
        print_indent();
        out << "[Add] " << std::endl;

        // Print Expr A
        ++level;
        op->a.accept(this);
        --level;

        // Print Expr B
        ++level;
        op->b.accept(this);
        --level;
    }

    void visit(const Sub *op) override {
        print_indent();
        out << "[Sub] " << std::endl;

        // Print Expr A
        ++level;
        op->a.accept(this);
        --level;

        // Print Expr B
        ++level;
        op->b.accept(this);
        --level;
    }

    void visit(const Mul *op) override {
        in_binary_op = true;
        print_indent();
        out << "[Mul] " << std::endl;

        // Print Expr A
        ++level;
        op->a.accept(this);
        --level;

        // Print Expr B
        ++level;
        op->b.accept(this);
        --level;
    }

    void visit(const NE *op) override {
        print_indent();
        out << "[NE]" << std::endl;

        // Print Expr A
        ++level;
        op->a.accept(this);
        --level;

        // Print Expr B
        ++level;
        op->b.accept(this);
        --level;
    }

    void visit(const Let *op) override {
        print_indent();
        out << "[Let]" << std::endl;
        ++level;
        print_indent();
        out << "[Name]: " << op->name << std::endl;
        --level;
        ++level;
        print_indent();
        out << "[Value]: " << std::endl;
        ++level;
        op->value.accept(this);
        --level;
        --level;

        ++level;
        print_indent();
        out << "[Body]: " << std::endl;
        ++level;
        op->body.accept(this);
        --level;
    }

    void visit(const Call *op) override {
        if (!print_asserts && inside_assert) {
            return;
        }

        print_indent();
        out << "[Call] " << std::endl;
        ++level;
        print_indent();
        out << "[Type]: " << op->type << std::endl;
        print_indent();
        out << "[Name]: " << op->name << std::endl;
        print_indent();
        out << "[Args]: " << std::endl;
        for (size_t i = 0; i < op->args.size(); ++i) {
            ++level;
            op->args[i].accept(this);
            --level;
        }
        --level;
    }

    void visit(const Cast *op) override {
        if (!print_asserts && inside_assert) {
            return;
        }

        print_indent();
        out << "[Cast] " << std::endl;
        ++level;
        print_indent();
        out << "[Type]: " << op->type << std::endl;
        --level;
        ++level;
        op->value.accept(this);
        --level;
    }

    void visit(const Variable *var) override {
        if (!print_asserts && inside_assert) {
            return;
        }

        print_indent();
        out << "[Var] " << std::endl;
        ++level;
        print_indent();
        out << "[Name] : " << var->name << std::endl;
        print_indent();
        out << "[Type] : " << var->type << std::endl;
        /*print_indent();
        out << "[Value]: ";*/
        --level;
    }

    void visit(const Block *op) override {
        /*print_indent();
        out << "[Block]" << std::endl;
        ++level;
        print_indent();
        out << "[First]: " << std::endl;
        ++level;
        op->first.accept(this);
        --level;

        --level;
        print_indent();
        out << "[Rest]: " << std::endl;
        ++level;
        op->rest.accept(this);
        --level;*/
        op->first.accept(this);
        op->rest.accept(this);
    }

    void visit(const AssertStmt *op) override {

        if (!print_asserts) {
            return;
        }

        inside_assert = true;

        print_indent();
        out << "[Assert]" << std::endl;
        ++level;
        print_indent();
        out << "[Message]  : " << std::endl;
        ++level;
        op->message.accept(this);
        --level;
        print_indent();
        out << "[Condition]: " << std::endl;
        ++level;
        op->condition.accept(this);
        --level;

        inside_assert = false;
    }

    void visit(const LetStmt *op) override {
        if (!print_asserts && inside_assert) {
            return;
        }

        print_indent();
        out << "[LetStmt]" << std::endl;
        ++level;
        print_indent();
        out << "[Name]  : " << op->name << std::endl;
        print_indent();
        out << "[Value] : " << std::endl;
        ++level;
        op->value.accept(this);
        --level;
        print_indent();
        out << "[Body] : " << std::endl;
        ++level;
        op->body.accept(this);
        --level;
    }

    void visit(const IfThenElse *op) override {
        if (!print_asserts && inside_assert) {
            return;
        }

        print_indent();
        out << "[IfThenElse]" << std::endl;
        ++level;
        print_indent();
        out << "[Condition]: " << std::endl;
        ++level;
        op->condition.accept(this);
        --level;
        print_indent();
        out << "[Then] : " << std::endl;
        ++level;
        op->then_case.accept(this);
        --level;
        --level;
    }

    void visit(const ProducerConsumer *op) override {
        if (!print_asserts && inside_assert) {
            return;
        }

        print_indent();
        if (op->is_producer) {
            out << "[Produce]: " << op->name << std::endl;
        } else {
            out << "[Consume]: " << op->name << std::endl;
        }
        ++level;
        op->body.accept(this);
        --level;
    }

    void visit(const For *op) override {
        if (!print_asserts && inside_assert) {
            return;
        }

        print_indent();
        out << "[For] " << std::endl;
        ++level;
        print_indent();
        out << "[Name]: " << op->name << std::endl;
        print_indent();
        out << "[Type]: " << op->for_type << std::endl;
        print_indent();
        out << "[API] : " << op->device_api << std::endl;
        print_indent();
        out << "[Body]: " << std::endl;
        ++level;
        op->body.accept(this);
        --level;
        --level;
    }

    void visit(const VectorReduce *op) override {
        print_indent();
        out << "[VectorReduce]" << std::endl;
        ++level;
        print_indent();
        out << "[Type]  : " << op->type << std::endl;
        print_indent();
        out << "[Op]    : " << op->op << std::endl;
        print_indent();
        out << "[Value] : " << std::endl;
        ++level;
        op->value.accept(this);
        --level;
        --level;
    }

    void visit(const Ramp *op) override {
        print_indent();
        out << "[Ramp]" << std::endl;
        ++level;
        print_indent();
        out << "[Lanes] : " << op->lanes << std::endl;
        print_indent();
        out << "[Base] : " << std::endl;
        ++level;
        op->base.accept(this);
        --level;
        print_indent();
        out << "[Stride] : " << std::endl;
        ++level;
        op->stride.accept(this);
        --level;
    }

    void visit(const Load* op) override {
        print_indent();
        out << "[Load]" << std::endl;
        ++level;
        print_indent();
        out << "[Name]  : " << op->name << std::endl;
        print_indent();
        out << "[Index] : " << std::endl;
        ++level;
        op->index.accept(this);
        --level;
        print_indent();
        out << "[Predicate] : " << std::endl;
        ++level;
        op->predicate.accept(this);
        --level;
    }

    void visit(const Store *op) override {
        print_indent();
        out << "[Store]" << std::endl;
        ++level;
        print_indent();
        out << "[Name]  : " << op->name << std::endl;
        print_indent();
        out << "[Index] : " << std::endl;
        ++level;
        op->index.accept(this);
        --level;
        print_indent();
        out << "[Predicate]: " << std::endl;
        ++level;
        op->predicate.accept(this);
        --level;
        print_indent();
        out << "[Value]" << std::endl;
        ++level;
        op->value.accept(this);
        --level;
    }

    void visit(const Atomic *op) override {
        print_indent();
        out << "[Atomic]" << std::endl;
    }

    void print_indent() {
        for (int i = 0; i < level * 2; i++) {
            out << " ";
        }
    }

    std::ostream &out;
    int level = 0;
    bool in_binary_op = false;
    bool print_asserts = false;
    bool inside_assert = false;
};

class MyIRMutator : public IRMutator
{
    // using IRMutator::visit;

    Expr visit(const Add* op) override
    {
        return Mul::make(op->a, op->b);
    }

    Stmt visit(const Store* op) override
    {
        Expr predicate = mutate(op->predicate);
        Expr value = mutate(op->value);
        Expr index = mutate(op->index);
        if (predicate.same_as(op->predicate) && value.same_as(op->value) && index.same_as(op->index))
        {
            return op;
        }
        return Store::make(op->name, std::move(value), std::move(index), op->param, std::move(predicate), op->alignment);
    }

    Stmt visit(const For* op) override
    {
        Expr min = mutate(op->min);
        Expr extent = mutate(op->extent);
        Stmt body = mutate(op->body);
        if (min.same_as(op->min) &&
            extent.same_as(op->extent) &&
            body.same_as(op->body))
        {
            return op;
        }
        return For::make(op->name, std::move(min), std::move(extent),
                         op->for_type, op->device_api, std::move(body));
    }

};

}  // namespace Internal
}  // namespace Halide


template<typename T>
void PrintBuffer(const Buffer<T>& buffer, std::string name, int spacing = 10)
{
    std::cout << name << std::endl;
    for (int j = 0; j < buffer.height(); j++)
    {
        for (int i = 0; i < buffer.width(); i++)
        {
            std::cout << std::setw(spacing) << (int32_t) buffer(i, j);
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

const bool compileToFile = true;
const bool shouldRealize = false;
const bool shouldAutoSchedule = false;
const bool shouldPrintIR = true;

void AutoscheduleFunc(Pipeline pipeline)
{
    Target target = get_jit_target_from_environment();
    /*Target target;
    target.os = Target::Linux;
    target.arch = Target::ARM;
    target.bits = 64;
    target.set_feature(Target::Feature::JIT);*/

    load_plugin("D:\\Halide\\out\\build\\x64-Debug\\src\\autoschedulers\\adams2019\\autoschedule_adams2019.dll");
    load_plugin("D:\\Halide\\out\\build\\x64-Debug\\src\\autoschedulers\\li2018\\autoschedule_li2018.dll");
    load_plugin("D:\\Halide\\out\\build\\x64-Debug\\src\\autoschedulers\\mullapudi2016\\autoschedule_mullapudi2016.dll");

    Pipeline::set_default_autoscheduler_name("Adams2019");
    // Pipeline::set_default_autoscheduler_name("Li2018");
    // Pipeline::set_default_autoscheduler_name("Mullapudi2016");

    MachineParams params{24, 64*1024, 10};
    AutoSchedulerResults schedulerResults = pipeline.auto_schedule(target, params);

    std::cout << "Auto Scheduler Results: " << std::endl;
    std::cout << "Name           : " << schedulerResults.scheduler_name << std::endl;
    std::cout << "Target         : " << schedulerResults.target << std::endl;
    std::cout << "Machine Params : " << schedulerResults.machine_params_string << std::endl;
    std::cout << "Source         : " << schedulerResults.schedule_source << std::endl;
}

Func MutatorExample(std::vector<Argument>& args)
{
    Var x("x");
    Func f("f");
    f(x) = x;

    Target target = get_jit_target_from_environment();

    Internal::Stmt s = Internal::lower_main_stmt({ f.function() }, f.name(), target);

    Internal::IRPrinter irPrinter(std::cout);
    std::cout << "Halide IR Before: " << std::endl;
    s.accept(&irPrinter);
    std::cout << "\nAfter:" << std::endl;

    Internal::MyIRMutator mutator;
    s = mutator.mutate(s);
    s.accept(&irPrinter);
    std::cout << std::endl;

    return f;
}

Func SimplePipeline(std::vector<Argument>& args)
{
    Var x("x"), y("y");
    Func f("f");
    Func a("a");
    RDom r(0, 4, "rdom");
    a(x) = x;
    f(x) += a(4 * x + r) * a(4 * x + r);

    Pipeline pipeline(f);

    if (shouldAutoSchedule)
    {
        // f.set_estimates({{0, 10}});
        // AutoscheduleFunc(pipeline);
    }
    else
    {
        // f.trace_stores();
        // f.split(x, xo, xi, 2);
        // f.tile(x, y, xo, yo, xi, yi, 2, 2);
    }

    if (shouldPrintIR)
    {
        Target target = get_jit_target_from_environment();

        Internal::Stmt s = Internal::lower_main_stmt({f.function()}, f.name(), target);

        std::cout << std::endl;
        std::cout << "Halide IR Printer " << std::endl;
        Internal::IRPrinter p(std::cout);
        s.accept(&p);

        std::cout << std::endl;
        std::cout << "My IR Printer " << std::endl;
        Internal::MyIRVisitor IRVis(std::cout);
        // s.accept(&IRVis);
    }

    if (shouldRealize)
    {
        // Target target = get_jit_target_from_environment();
        Target target = get_host_target();

        std::cout << "Realizing ..." << std::endl;
        Buffer<int32_t> a_out = a.realize({10}, target);
        Buffer<int32_t> output = f.realize({5}, target);

        PrintBuffer(a_out, "input", 5);
        PrintBuffer(output, "output", 5);
    }

    return f;
}

Func SaturatingDotProduct(std::vector<Argument>& args)
{
    const int vector_width = 16;
    const int factor = 4;

    Var x("x"), y("y");
    Func in1("in1"), in2("in2");
    RDom r(0, factor, "rdom");
    in1(x) = i8(x);
    in2(x) = u8(x);

    Func f("f");
    f(x) += i32(i16(in1(factor * x + r))) * in2(factor * x + r);

    // check("vpdpwssds*zmm", 16, saturating_sum(i32(0), i32(in_i16(2 * x + r)) * in_i16(2 * x + r + 32)));

    // Expr mul = i32(i16(in1(factor * x + r))) * in2(factor * x + r);

    // Expr e = sum(mul);
    // Expr e = saturating_sum(i32(INT_MAX), mul);

    // Expr e = i32(i16(in1(4 * x + r)) * in2(4 * x + r));
    // Expr e = saturating_add(e, (u16(in1(4 * x + r)) * i16(in2(4 * x + r))));

    const bool override_associativity_test = true;

    // Func f("f");
    // f(x) = i32(100);
    // f(x) += mul;
    // f(x) = INT_MAX;
    // f(x) += mul;
    // f(x) = saturating_add(f(x), mul);
    // f(x) = e;

    Pipeline pipeline(f);

    if (shouldAutoSchedule)
    {
        f.set_estimates({{0, 31}});
        AutoscheduleFunc(pipeline);
    }
    else
    {
        // My Schedule
        #if 1
        // Handwritten schedule
        Var xo, xi;
        f.update()
            .split(x, xo, xi, vector_width)
            .atomic()
            .vectorize(r)
            .vectorize(xi);
        #endif

        // Mullapudi2016
        #if 0
        Var x_vi("x_vi");
        Var x_vo("x_vo");

        Func f = pipeline.get_func(2);
        {
            // Few tweaks
            Var x = f.args()[0];
            f.compute_root()
             .split(x, x_vo, x_vi, 16)
             .vectorize(x_vi)
             .parallel(x_vo);
            f.update(0)
             .split(x, x_vo, x_vi, 16, TailStrategy::GuardWithIf)
             .atomic()
             .vectorize(r)
             .vectorize(x_vi)
             .parallel(x_vo);
        }
        #endif

        // Li2018
        #if 0
        Var v2, v3, v4, v5, v8, v9, v10, v11;
        RVar rdom$x(f.update(0).get_schedule().dims()[0].var);
        f.compute_root()
            .split(x,v2,v3,8,TailStrategy::ShiftInwards)
            .reorder(v3,v2)
            .parallel(v2)
            .vectorize(v3)
        ;
        f.update(0)
        ;
        f.update(0)
            .split(x,v4,v5,8,TailStrategy::GuardWithIf)
            .reorder(v5,rdom$x,v4)
            .parallel(v4)
            .atomic()
            .vectorize(rdom$x)
            .vectorize(v5)
        ;
       /* in2.compute_root()
            .split(x,v8,v9,16,TailStrategy::ShiftInwards)
            .reorder(v9,v8)
            .parallel(v8)
            .vectorize(v9)
        ;
        in1.compute_root()
            .split(x,v10,v11,16,TailStrategy::ShiftInwards)
            .reorder(v11,v10)
            .parallel(v10)
            .vectorize(v11)*/
        ;
        #endif


        // Adams2019
        #if 0
        Func f = pipeline.get_func(2);
        Func in2 = pipeline.get_func(1);
        Func in1 = pipeline.get_func(0);
        Var x(f.update(0).get_schedule().dims()[1].var);
        Var xi("xi");
        RVar rdom_x(f.update(0).get_schedule().dims()[0].var);
        f.update(0)
         .split(x, x, xi, 32, TailStrategy::GuardWithIf)
         .atomic()
         .vectorize(rdom_x)
         .vectorize(xi)
         .// .reorder({xi, rdom_x, x});
            ;
        f.split(x, x, xi, 16, TailStrategy::ShiftInwards)
         .vectorize(xi)
         .compute_root()
         .reorder({xi, x})
         .parallel(x);
        #endif

        class HasInlineReduction : public Internal::IRVisitor {
            using Internal::IRVisitor::visit;

            void visit(const Internal::Call *op) override {
            if (op->call_type == Internal::Call::Halide) {
                Internal::Function f(op->func);
                if (f.has_update_definition()) {
                    inline_reduction = f;
                    result = true;
                    std::cout << Func(inline_reduction).name() << std::endl;
                }
            }
            IRVisitor::visit(op);
        }

        public:
            Internal::Function inline_reduction;
            bool result = false;
        } has_inline_reduction;
        // e.accept(&has_inline_reduction);

        if (has_inline_reduction.result)
        {
            Var xo, xi;

            Func g(has_inline_reduction.inline_reduction);

            g.compute_at(f, x)
                .update()
                .split(x, xo, xi, vector_width)
                .atomic(override_associativity_test)
                .vectorize(g.rvars()[0])
                .vectorize(xi);
        }
    }


    if (shouldRealize)
    {
        Target target = get_jit_target_from_environment();

        std::cout << "Realizing ..." << std::endl;
        Buffer<int16_t> input1 = in1.realize({2*vector_width}, target);
        Buffer<int16_t> input2 = in2.realize({2*vector_width}, target);
        Buffer<int32_t> output = f.realize({vector_width}, target);

        PrintBuffer(input1, "input1", 3);
        PrintBuffer(input2, "input2", 3);
        PrintBuffer(output, "output", 15);
    }

    return f;
}

Func TiledMatMul(std::vector<Argument>& args)
{
    const int row = 16;
    const int col = 16;
    const int acc = 16;

    Var x("x"), y("y");
    ImageParam A(Int(8), 2, "lhs");
    // NB the RHS matrix in AMX instructions should be tiled in "VNNI format",
    // where instead of being (cols, rows) where rows are adjacent in memory it
    // should be (4, cols, rows / 4) for int8, or (2, cols, rows / 2) for bf16.
    // This means that the rows must always be divisible by 4 (or 2 for bf16).
    ImageParam B(Int(8), 3, "rhs");

    RDom r(0, acc);

    Func mm("matmul");
    mm(y, x) = cast<int32_t>(0);
    mm(y, x) += cast<int16_t>(A(r.x, x)) * B(r.x % 4, y, r.x / 4);

    Pipeline pipeline(mm.in());

    if (shouldAutoSchedule)
    {
        Target target = get_jit_target_from_environment();

        load_plugin("D:\\Halide\\out\\build\\x64-Debug\\src\\autoschedulers\\adams2019\\autoschedule_adams2019.dll");
        load_plugin("D:\\Halide\\out\\build\\x64-Debug\\src\\autoschedulers\\li2018\\autoschedule_li2018.dll");
        load_plugin("D:\\Halide\\out\\build\\x64-Debug\\src\\autoschedulers\\mullapudi2016\\autoschedule_mullapudi2016.dll");

        Pipeline::set_default_autoscheduler_name("Adams2019");
        // Pipeline::set_default_autoscheduler_name("Li2018");
        // Pipeline::set_default_autoscheduler_name("Mullapudi2016");

        A.set_estimates({{0, col}, {0, row}});
        B.set_estimates({{0, 2}, {0, col}, {0, row}});
        mm.in().set_estimate(x, 0, row*col);
        mm.in().set_estimate(y, 0, row*col);

        MachineParams params{16, 64*1024, 160};
        AutoSchedulerResults schedulerResults = pipeline.auto_schedule(target, params);

        std::cout << "Auto Scheduler Results: " << std::endl;
        std::cout << "Name           : " << schedulerResults.scheduler_name << std::endl;
        std::cout << "Target         : " << schedulerResults.target << std::endl;
        std::cout << "Machine Params : " << schedulerResults.machine_params_string << std::endl;
        std::cout << "Source         : " << schedulerResults.schedule_source << std::endl;
    }
    else
    {
        int tile_y = 8;
        int tile_x = 6;
        int tile_r = 4;

        #if 0
        Var rxi("rxi"), ryi("ryi");
        RVar rri("rri"), rro("rro");
        mm
            .compute_at(mm.in(), y)
            .store_in(MemoryType::AMXTile)
            .update()
            .tile(y, x, ryi, rxi, tile_y, tile_x, TailStrategy::GuardWithIf)
            .split(r.x, rro, rri, tile_r)
            .reorder({rri, ryi, rxi, rro, y, x})
            .atomic()
            .vectorize(rri)
            .vectorize(ryi)
            .vectorize(rxi)
            ;
        #endif

        #if 0
        // Ensure all (x, y) tile sizes are the same so that loops are fused.
        // Schedule the reduction
        Var rxi("rxi"), ryi("ryi");
        RVar rri("rri"), rro("rro");
        mm.compute_at(mm.in(), y)
            .store_in(MemoryType::AMXTile)
            .update()
            // Split into (x,y) tile
            .tile(y, x, ryi, rxi, tile_y, tile_x, TailStrategy::GuardWithIf)
            // Split reduction dim by tile_r
            .split(r.x, rro, rri, tile_r)
            // Reorder so that the (x,y) tile is inside the inner ro loop
            .reorder({rri, ryi, rxi, rro, y, x})
            .atomic()
            .vectorize(rri)
            .vectorize(ryi)
            .vectorize(rxi)
            ;

        // Schedule the initialization
        Var ixi("ixi"), iyi("iyi");
        mm.compute_at(mm.in(), y)
            .tile(y, x, iyi, ixi, tile_y, tile_x)
            .vectorize(iyi)
            .vectorize(ixi);

        // Schedule the consumer
        Var mmxi("mmxi"), mmyi("mmyi");
        mm.in()
            .tile(y, x, mmyi, mmxi, tile_y, tile_x)
            .vectorize(mmyi)
            .vectorize(mmxi);
        #endif

        // Mullapudi2016
        #if 0
        Var y_i("y_i");
        Var y_i_vi("y_i_vi");
        Var y_i_vo("y_i_vo");
        Var y_o("y_o");
        Var y_vi("y_vi");
        Var y_vo("y_vo");

        Func matmul = pipeline.get_func(2);
        Func matmul_global_wrapper_0 = pipeline.get_func(3);

        {
            Var y = matmul.args()[0];
            RVar r9$x(matmul.update(0).get_schedule().rvars()[0].var);
            matmul
                .compute_at(matmul_global_wrapper_0, y_o)
                .split(y, y_vo, y_vi, 16)
                .vectorize(y_vi);

           matmul.update(0)
                .reorder(y, x, r9$x)
                .split(y, y_vo, y_vi, 16, TailStrategy::GuardWithIf)
                .vectorize(y_vi);
        }
        {
            Var y = matmul_global_wrapper_0.args()[0];
            Var x = matmul_global_wrapper_0.args()[1];
            matmul_global_wrapper_0
                .compute_root()
                .split(y, y_o, y_i, 64)
                .reorder(y_i, y_o, x)
                .split(y_i, y_i_vo, y_i_vi, 16)
                .vectorize(y_i_vi)
                .parallel(x);
        }
        #endif

        #if 0
        Func matmul_global_wrapper_0 = pipeline.get_func(3);
        Func matmul = pipeline.get_func(2);
        Var x(matmul_global_wrapper_0.get_schedule().dims()[1].var);
        Var xi("xi");
        Var y(matmul_global_wrapper_0.get_schedule().dims()[0].var);
        Var yi("yi");
        Var yii("yii");
        RVar r9_x(matmul.update(0).get_schedule().dims()[0].var);
        // RVar r9_y(matmul.update(0).get_schedule().dims()[1].var);
        Var rxi("rxi"), ryi("ryi");
        RVar rri("rri"), rro("rro");
        matmul_global_wrapper_0
            .split(y, y, yi, 64, TailStrategy::ShiftInwards)
            .split(x, x, xi, 8, TailStrategy::ShiftInwards)
            .split(yi, yi, yii, 16, TailStrategy::ShiftInwards)
            .vectorize(yii)
            .compute_root()
            .reorder({yii, yi, xi, y, x})
            .fuse(y, x, y)
            .parallel(y);
        matmul
            .store_in(MemoryType::AMXTile)
            .update(0)
            .tile(y, x, ryi, rxi, tile_y, tile_x, TailStrategy::GuardWithIf)
            .split(y, y, yi, 64, TailStrategy::GuardWithIf)
            // .unroll(x)
            .atomic()
            .vectorize(yi)
            .reorder({yi, y, x, r9_x});
        matmul
            .split(y, y, yi, 16, TailStrategy::RoundUp)
            .vectorize(yi)
            .compute_at(matmul_global_wrapper_0, y)
            .reorder({yi, y, x});
        #endif
    }

    args.push_back(A);
    args.push_back(B);

    return mm.in();
}

int main()
{
    Target host = get_host_target();
    Target hl_target = get_target_from_environment();
    printf("host is:      %s\n", host.to_string().c_str());
    printf("HL_TARGET is: %s\n", hl_target.to_string().c_str());

    try
    {
        std::vector<Argument> args;

        Func f = MutatorExample(args);
        // Func f = SimplePipeline(args);
        // Func f = SaturatingDotProduct(args);
        // Func f = TiledMatMul(args);

        Target target = get_jit_target_from_environment();

        if (false)
        {
            Internal::Stmt s = Internal::lower_main_stmt({f.function()}, f.name(), target);

            std::cout << std::endl;
            std::cout << "Halide IR Printer " << std::endl;
            Internal::IRPrinter p(std::cout);
            /*std::cout << "Init Value: " << std::endl;
            f.value().accept(&p);
            std::cout << std::endl << std::endl;

            std::cout << "Update Value: " << std::endl;
            f.update_value().accept(&p);
            std::cout << std::endl << std::endl;*/
            s.accept(&p);

            std::cout << std::endl;
            std::cout << "My IR Printer " << std::endl;
            Internal::MyIRVisitor IRVis(std::cout);
            /*std::cout << "Init Value: " << std::endl;
            f.value().accept(&IRVis);
            std::cout << std::endl << std::endl;

            std::cout << "Update Value: " << std::endl;
            f.update_value().accept(&IRVis);
            std::cout << std::endl << std::endl;*/
            // s.accept(&IRVis);
        }

        f.print_loop_nest();
        std::cout << "Compiling HTML..." << std::endl;
        if (compileToFile)
            f.compile_to_lowered_stmt("trying_stuff.html", args, HTML, target);

        std::cout << "Compiling to LLVM..." << std::endl;
        if (compileToFile) {
            // f.compile_to_c("trying_stuff.c", args, "trying_stuff", target);
            f.compile_to_llvm_assembly("trying_stuff.ll", args, target);
        }

        std::cout << "Compiling Assembly..." << std::endl;
        if (compileToFile)
            f.compile_to_assembly("trying_stuff.S", args, target);


    }
    catch (CompileError& e)
    {
        std::cout << e.what() << std::endl;
    }
}