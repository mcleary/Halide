#include "Halide.h"

namespace Halide
{

    namespace Internal
    {

        class MyIRVisitor : public IRVisitor
        {
        public:
            using IRVisitor::visit;

            MyIRVisitor(std::ostream& s)
                : out(s)
            {    }

            void visit(const StringImm* op) override
            {
                print_indent();
                out << "[String] : " << op->value << std::endl;
            }

            void visit(const IntImm* op) override
            {
                print_indent();
                out << "[Const]" << std::endl;
                ++level;
                print_indent();
                out << "[Type]  : " << op->type << std::endl;
                print_indent();
                out << "[Value] : " << op->value << std::endl;
                --level;
            }

            void visit(const UIntImm* op) override
            {
                print_indent();
                out << "[Const]" << std::endl;
                ++level;
                print_indent();
                out << "[Type]  : " << op->type << std::endl;
                print_indent();
                out << "[Value] : " << op->value << std::endl;
                --level;
            }

            void visit(const Add* op) override
            {
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

            void visit(const Sub* op) override
            {
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

            void visit(const Mul* op) override
            {
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

            void visit(const NE* op) override
            {
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

            void visit(const Let* op) override
            {
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

            void visit(const Call* op) override
            {
                if (!print_asserts && inside_assert)
                {
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
                for (size_t i = 0; i < op->args.size(); ++i)
                {
                    ++level;
                    op->args[i].accept(this);
                    --level;
                }
                --level;
            }

            void visit(const Cast* op) override
            {
                if (!print_asserts && inside_assert)
                {
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

            void visit(const Variable* var) override
            {
                if (!print_asserts && inside_assert)
                {
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

            void visit(const Block* op) override
            {
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

            void visit(const AssertStmt* op) override
            {

                if (!print_asserts)
                {
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

            void visit(const LetStmt* op) override
            {
                if (!print_asserts && inside_assert)
                {
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

            void visit(const IfThenElse* op) override
            {
                if (!print_asserts && inside_assert)
                {
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

            void visit(const ProducerConsumer* op) override
            {
                if (!print_asserts && inside_assert)
                {
                    return;
                }

                print_indent();
                if (op->is_producer)
                {
                    out << "[Produce]: " << op->name << std::endl;
                }
                else
                {
                    out << "[Consume]: " << op->name << std::endl;
                }
                ++level;
                op->body.accept(this);
                --level;
            }

            void visit(const For* op) override
            {
                if (!print_asserts && inside_assert)
                {
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

            void visit(const VectorReduce* op) override
            {
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

            void visit(const Ramp* op) override
            {
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

            void visit(const Load* op) override
            {
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

            void visit(const Store* op) override
            {
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

            void visit(const Atomic* op) override
            {
                print_indent();
                out << "[Atomic]" << std::endl;
            }

            void print_indent()
            {
                for (int i = 0; i < level * 2; i++)
                {
                    out << " ";
                }
            }

            std::ostream& out;
            int level = 0;
            bool in_binary_op = false;
            bool print_asserts = false;
            bool inside_assert = false;
        };

    }  // namespace Internal
}  // namespace Halide