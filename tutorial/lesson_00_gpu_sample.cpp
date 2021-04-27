#include <iostream>
#include <iomanip>

#include "Halide.h"
#include "MyIRPrinter.h"
#include "float_comparison.h"

using namespace std;
using namespace Halide;
using namespace Halide::ConciseCasts;


#define USE_PARAMS 1

template<typename T>
void InitMatrix(Buffer<T>& buffer)
{
	const int rows = buffer.height();
	const int cols = buffer.width();
	for (int y = 0; y < rows; ++y)
	{
		for (int x = 0; x < cols; ++x)
		{
			float v = x + y * cols;
			v /= rows * cols;
			// v = 1.0f;
			// std::cout << x << ", " << y << " = " << v << std::endl;
			buffer(x, y) = static_cast<float16_t>(v);
		}
	}

}

template<typename T>
void PrintBuffer(const Buffer<T>& buffer)
{
	// Print the result.
	const int rows = buffer.height();
	const int cols = buffer.width();
	for (int y = 0; y < rows; ++y)
	{
		for (int x = 0; x < cols; ++x)
		{
			printf("%5.2f ", (float)buffer(x, y));
		}
		printf("\n");
	}
}

ImageParam A(Float(16), 2, "A");
ImageParam B(Float(16), 2, "B");
Func out("matmul");

const bool test_single = false;
const bool verbose = test_single;

// mk x kn
// rows (M) x columns (N)
void TestMatMul(int M, int N, int K)
{
	std::cout << "M: " << M << " N: " << N << " K: " << K << std::endl;

	try
	{
		const int x_tile = 16;
		const int y_tile = 16;

		const int warp_size = 32;
		const int vec_size = 2;
		const int y_unroll = 1;
		const int r_unroll = 1;

		Target target = get_host_target();
		Target jitTarget = get_jit_target_from_environment();

		// Enable the CUDA target
		target.set_feature(Target::CUDA);
		target.set_feature(Target::CUDACapability70);

		if (verbose)
		{
			// Enable debugging hopefully to see the CUDA API calls
			target.set_feature(Target::Debug);
		}

		cout << "Halide Host Target: " << target << endl;
		cout << "Halide JIT Target : " << jitTarget << endl;

		Var x("x"), y("y"), xo("xo"), xi("xi"), yo("yo"), yi("y1");
		Var xio("xio"), xii("xii"), yii("xii"), x_pair("x_pair"), xiio("xiio"), ty("ty");
		RVar rxo("rxo"), rxi("rxi");

		Func matmul("matmul");
		RDom k(0, K, "k");
		matmul(x, y) += f32((A(k, y)) * B(x, k));

		out = matmul;

		Var blockX("blockX"), blockY("blockY"), threadX("threadX"), threadY("threadY");
		out
			.update()
			.gpu_tile(x, y, blockX, blockY, threadX, threadY, x_tile, y_tile, TailStrategy::Auto)
			;

		if (verbose)
		{
			cout << "Matmul Loop Nest: " << std::endl;
			out.print_loop_nest();
			cout << endl << endl;

			// Internal::Stmt s = Internal::lower_main_stmt({ out.function() }, out.name(), target);
			// Internal::MyIRVisitor vis(std::cout);
			// s.accept(&vis);
		}

		std::vector<Argument> args = { A, B };

		if (verbose)
		{
			cout << "Compiling to HTML ... " << flush;
			out.compile_to_lowered_stmt("cuda.html", args, HTML, target);
			// matmul.compile_to_lowered_stmt("cuda.html", {}, HTML, target);
			cout << "done" << endl;

			cout << "Compiling to LLVM ... " << flush;
			// out.compile_to_llvm_assembly("cuda.ll", args, target);
			cout << "done" << endl;

			cout << "Compiling to Assembly ... " << flush;
			//out.compile_to_assembly("cuda.S", {}, target);
			cout << "done" << endl;

			cout << "Jitting the code ... " << flush;
			out.compile_jit(target);
			cout << "done" << endl << endl;
		}

		Buffer<float16_t> inputA(K, M);
		Buffer<float16_t> inputB(N, K);

		InitMatrix(inputA);
		InitMatrix(inputB);

		A.set(inputA);
		B.set(inputB);

		if (verbose)
		{
			cout << "Input A: (" << inputA.height() << " x " << inputA.width() << ")" << endl;
			PrintBuffer(inputA);
			cout << endl << endl;

			cout << "Input B: (" << inputB.height() << " x " << inputB.width() << ")" << endl;
			PrintBuffer(inputB);
			cout << endl << endl;
		}

		// Run it
		Buffer<float> result = out.realize(N, M, target);

		if (verbose)
		{
			cout << "Result: (" << result.height() << " x " << result.width() << ")" << endl;
			PrintBuffer(result);
			cout << endl << endl;
		}

		// cout << "Comparing: " << endl;
		for (int y = 0; y < result.height(); ++y)
		{
			for (int x = 0; x < result.width(); ++x)
			{
				float acc = 0.0f;
				for (int k = 0; k < K; ++k)
				{
					float16_t a = inputA(k, y);
					float16_t b = inputB(x, k);
					acc += static_cast<float>(a * b);
				}
				const std::string result_str = "result(" + std::to_string(x) + ", " + std::to_string(y) + ")";
				if (!expect_almost_equal(result_str.c_str(), "acc", "aa", result(x, y), acc, 4000))
				{
					std::cout << "-----------------" << std::endl;
					std::cout << "M: " << M << " N: " << N << " K: " << K << std::endl;
					std::cout << "-----------------" << std::endl;
					return;
				}
			}
		}
	}
	catch (RuntimeError& e)
	{
		cout << e.what() << endl;
	}
	catch (CompileError& e)
	{
		cout << e.what() << endl;
	}
	catch (InternalError& e)
	{
		cout << e.what() << endl;
	}
}

int main()
{
	try
	{
		std::cout << "Starting correctness tests ... " << std::endl;

		if (test_single)
		{
			const int m = 16;
			const int n = 16;
			const int k = 16;
			TestMatMul(m, n, k);
		}
		else
		{
			const int max_size = 64;
			const int tile_size = 16;
			for (int m = 16; m <= max_size; m += tile_size)
			{
				for (int n = 16; n <= max_size; n += tile_size)
				{
					for (int k = 16; k <= max_size; k += tile_size)
					{
						TestMatMul(m, n, k);
					}
				}
			}
		}

		std::cout << "done" << std::endl;
	}
	catch (RuntimeError& e)
	{
		cout << e.what() << endl;
	}
	catch (CompileError& e)
	{
		cout << e.what() << endl;
	}
	catch (InternalError& e)
	{
		cout << e.what() << endl;
	}

	return 0;
}
