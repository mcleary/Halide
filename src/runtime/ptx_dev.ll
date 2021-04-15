declare void @llvm.nvvm.barrier0()
declare  i32 @llvm.nvvm.read.ptx.sreg.tid.x()
declare  i32 @llvm.nvvm.read.ptx.sreg.ctaid.x()
declare  i32 @llvm.nvvm.read.ptx.sreg.ntid.x()
declare  i32 @llvm.nvvm.read.ptx.sreg.nctaid.x()
declare  i32 @llvm.nvvm.read.ptx.sreg.tid.y()
declare  i32 @llvm.nvvm.read.ptx.sreg.ctaid.y()
declare  i32 @llvm.nvvm.read.ptx.sreg.ntid.y()
declare  i32 @llvm.nvvm.read.ptx.sreg.nctaid.y()
declare  i32 @llvm.nvvm.read.ptx.sreg.tid.z()
declare  i32 @llvm.nvvm.read.ptx.sreg.ctaid.z()
declare  i32 @llvm.nvvm.read.ptx.sreg.ntid.z()
declare  i32 @llvm.nvvm.read.ptx.sreg.nctaid.z()
declare  i32 @llvm.nvvm.read.ptx.sreg.tid.w()
declare  i32 @llvm.nvvm.read.ptx.sreg.ctaid.w()
declare  i32 @llvm.nvvm.read.ptx.sreg.ntid.w()
declare  i32 @llvm.nvvm.read.ptx.sreg.nctaid.w()
declare  i32 @llvm.nvvm.read.ptx.sreg.warpsize()

; Remove these two once the minimum required llvm version is 9.0
declare float @llvm.nvvm.atomic.load.add.f32.p0f32(float*, float)
declare double @llvm.nvvm.atomic.load.add.f64.p0f64(double *, double)

; Legacy - to replace
;declare void @llvm.ptx.red.global.add.s32(i32*, i32)
;declare void @llvm.ptx.red.global.add.f32(float*, float)
;declare void @llvm.ptx.red.shared.add.s32(i32 addrspace(4)*, i32)

define weak_odr float @nan_f32() nounwind uwtable readnone alwaysinline {
       ret float 0x7FF8000000000000;
}

define weak_odr float @neg_inf_f32() nounwind uwtable readnone alwaysinline {
       ret float 0xFFF0000000000000;
}

define weak_odr float @inf_f32() nounwind uwtable readnone alwaysinline {
       ret float 0x7FF0000000000000;
}

declare float @__nv_sqrtf(float) nounwind readnone
declare double @__nv_sqrt(double) nounwind readnone

define weak_odr float @sqrt_f32(float %x) nounwind uwtable readnone alwaysinline {
       %y = tail call float @__nv_sqrtf(float %x) nounwind readnone
       ret float %y
}

define weak_odr double @sqrt_f64(double %x) nounwind uwtable readnone alwaysinline {
       %y = tail call double @__nv_sqrt(double %x) nounwind readnone
       ret double %y
}

declare float @__nv_frcp_rn(float) nounwind readnone

define weak_odr float @fast_inverse_f32(float %x) nounwind uwtable readnone alwaysinline {
       %y = tail call float @__nv_frcp_rn(float %x) nounwind readnone
       ret float %y
}

declare float @llvm.nvvm.rsqrt.approx.ftz.f(float) nounwind readnone

define weak_odr float @fast_inverse_sqrt_f32(float %x) nounwind uwtable readnone alwaysinline {
       %y = tail call float @llvm.nvvm.rsqrt.approx.ftz.f(float %x) nounwind readnone
       ret float %y
}

declare float @__nv_sinf(float) nounwind readnone
declare double @__nv_sin(double) nounwind readnone

define weak_odr float @sin_f32(float %x) nounwind uwtable readnone alwaysinline {
       %y = tail call float @__nv_sinf(float %x) nounwind readnone
       ret float %y
}

define weak_odr double @sin_f64(double %x) nounwind uwtable readnone alwaysinline {
       %y = tail call double @__nv_sin(double %x) nounwind readnone
       ret double %y
}

declare float @__nv_cosf(float) nounwind readnone
declare double @__nv_cos(double) nounwind readnone

define weak_odr float @cos_f32(float %x) nounwind uwtable readnone alwaysinline {
       %y = tail call float @__nv_cosf(float %x) nounwind readnone
       ret float %y
}

define weak_odr double @cos_f64(double %x) nounwind uwtable readnone alwaysinline {
       %y = tail call double @__nv_cos(double %x) nounwind readnone
       ret double %y
}

declare float @__nv_expf(float) nounwind readnone
declare double @__nv_exp(double) nounwind readnone

define weak_odr float @exp_f32(float %x) nounwind uwtable readnone alwaysinline {
       %y = tail call float @__nv_expf(float %x) nounwind readnone
       ret float %y
}

define weak_odr double @exp_f64(double %x) nounwind uwtable readnone alwaysinline {
       %y = tail call double @__nv_exp(double %x) nounwind readnone
       ret double %y
}

declare float @__nv_logf(float) nounwind readnone
declare double @__nv_log(double) nounwind readnone

define weak_odr float @log_f32(float %x) nounwind uwtable readnone alwaysinline {
       %y = tail call float @__nv_logf(float %x) nounwind readnone
       ret float %y
}

define weak_odr double @log_f64(double %x) nounwind uwtable readnone alwaysinline {
       %y = tail call double @__nv_log(double %x) nounwind readnone
       ret double %y
}

declare float @__nv_fabsf(float) nounwind readnone
declare double @__nv_fabs(double) nounwind readnone

define weak_odr float @abs_f32(float %x) nounwind uwtable readnone alwaysinline {
       %y = tail call float @__nv_fabsf(float %x) nounwind readnone
       ret float %y
}

define weak_odr double @abs_f64(double %x) nounwind uwtable readnone alwaysinline {
       %y = tail call double @__nv_fabs(double %x) nounwind readnone
       ret double %y
}

declare float @__nv_floorf(float) nounwind readnone
declare double @__nv_floor(double) nounwind readnone

define weak_odr float @floor_f32(float %x) nounwind uwtable readnone alwaysinline {
       %y = tail call float @__nv_floorf(float %x) nounwind readnone
       ret float %y
}

define weak_odr double @floor_f64(double %x) nounwind uwtable readnone alwaysinline {
       %y = tail call double @__nv_floor(double %x) nounwind readnone
       ret double %y
}

declare float @__nv_ceilf(float) nounwind readnone
declare double @__nv_ceil(double) nounwind readnone

define weak_odr float @ceil_f32(float %x) nounwind uwtable readnone alwaysinline {
       %y = tail call float @__nv_ceilf(float %x) nounwind readnone
       ret float %y
}

define weak_odr double @ceil_f64(double %x) nounwind uwtable readnone alwaysinline {
       %y = tail call double @__nv_ceil(double %x) nounwind readnone
       ret double %y
}

declare float @__nv_nearbyintf(float) nounwind readnone
declare double @__nv_nearbyint(double) nounwind readnone

define weak_odr float @round_f32(float %x) nounwind uwtable readnone alwaysinline {
       %y = tail call float @__nv_nearbyintf(float %x) nounwind readnone
       ret float %y
}

define weak_odr double @round_f64(double %x) nounwind uwtable readnone alwaysinline {
       %y = tail call double @__nv_nearbyint(double %x) nounwind readnone
       ret double %y
}

declare float @__nv_truncf(float) nounwind readnone
declare double @__nv_trunc(double) nounwind readnone

define weak_odr float @trunc_f32(float %x) nounwind uwtable readnone alwaysinline {
       %y = tail call float @__nv_truncf(float %x) nounwind readnone
       ret float %y
}

define weak_odr double @trunc_f64(double %x) nounwind uwtable readnone alwaysinline {
       %y = tail call double @__nv_trunc(double %x) nounwind readnone
       ret double %y
}

declare float @__nv_powf(float, float) nounwind readnone
declare double @__nv_pow(double, double) nounwind readnone

define weak_odr float @pow_f32(float %x, float %y) nounwind uwtable readnone alwaysinline {
       %z = tail call float @__nv_powf(float %x, float %y) nounwind readnone
       ret float %z
}

define weak_odr double @pow_f64(double %x, double %y) nounwind uwtable readnone alwaysinline {
       %z = tail call double @__nv_pow(double %x, double %y) nounwind readnone
       ret double %z
}

declare float @__nv_asinf(float) nounwind readnone
declare double @__nv_asin(double) nounwind readnone

define weak_odr float @asin_f32(float %x) nounwind uwtable readnone alwaysinline {
       %y = tail call float @__nv_asinf(float %x) nounwind readnone
       ret float %y
}

define weak_odr double @asin_f64(double %x) nounwind uwtable readnone alwaysinline {
       %y = tail call double @__nv_asin(double %x) nounwind readnone
       ret double %y
}

declare float @__nv_acosf(float) nounwind readnone
declare double @__nv_acos(double) nounwind readnone

define weak_odr float @acos_f32(float %x) nounwind uwtable readnone alwaysinline {
       %y = tail call float @__nv_acosf(float %x) nounwind readnone
       ret float %y
}

define weak_odr double @acos_f64(double %x) nounwind uwtable readnone alwaysinline {
       %y = tail call double @__nv_acos(double %x) nounwind readnone
       ret double %y
}

declare float @__nv_tanf(float) nounwind readnone
declare double @__nv_tan(double) nounwind readnone

define weak_odr float @tan_f32(float %x) nounwind uwtable readnone alwaysinline {
       %y = tail call float @__nv_tanf(float %x) nounwind readnone
       ret float %y
}

define weak_odr double @tan_f64(double %x) nounwind uwtable readnone alwaysinline {
       %y = tail call double @__nv_tan(double %x) nounwind readnone
       ret double %y
}

declare float @__nv_atanf(float) nounwind readnone
declare double @__nv_atan(double) nounwind readnone

define weak_odr float @atan_f32(float %x) nounwind uwtable readnone alwaysinline {
       %y = tail call float @__nv_atanf(float %x) nounwind readnone
       ret float %y
}

define weak_odr double @atan_f64(double %x) nounwind uwtable readnone alwaysinline {
       %y = tail call double @__nv_atan(double %x) nounwind readnone
       ret double %y
}

declare float @__nv_atan2f(float, float) nounwind readnone
declare double @__nv_atan2(double, double) nounwind readnone

define weak_odr float @atan2_f32(float %y, float %x) nounwind uwtable readnone alwaysinline {
       %z = tail call float @__nv_atan2f(float %y, float %x) nounwind readnone
       ret float %z
}

define weak_odr double @atan2_f64(double %y, double %x) nounwind uwtable readnone alwaysinline {
       %z = tail call double @__nv_atan2(double %y, double %x) nounwind readnone
       ret double %z
}

declare float @__nv_sinhf(float) nounwind readnone
declare double @__nv_sinh(double) nounwind readnone

define weak_odr float @sinh_f32(float %x) nounwind uwtable readnone alwaysinline {
       %y = tail call float @__nv_sinhf(float %x) nounwind readnone
       ret float %y
}

define weak_odr double @sinh_f64(double %x) nounwind uwtable readnone alwaysinline {
       %y = tail call double @__nv_sinh(double %x) nounwind readnone
       ret double %y
}

declare float @__nv_asinhf(float) nounwind readnone
declare double @__nv_asinh(double) nounwind readnone

define weak_odr float @asinh_f32(float %x) nounwind uwtable readnone alwaysinline {
       %y = tail call float @__nv_asinhf(float %x) nounwind readnone
       ret float %y
}

define weak_odr double @asinh_f64(double %x) nounwind uwtable readnone alwaysinline {
       %y = tail call double @__nv_asinh(double %x) nounwind readnone
       ret double %y
}

declare float @__nv_coshf(float) nounwind readnone
declare double @__nv_cosh(double) nounwind readnone

define weak_odr float @cosh_f32(float %x) nounwind uwtable readnone alwaysinline {
       %y = tail call float @__nv_coshf(float %x) nounwind readnone
       ret float %y
}

define weak_odr double @cosh_f64(double %x) nounwind uwtable readnone alwaysinline {
       %y = tail call double @__nv_cosh(double %x) nounwind readnone
       ret double %y
}

declare float @__nv_acoshf(float) nounwind readnone
declare double @__nv_acosh(double) nounwind readnone

define weak_odr float @acosh_f32(float %x) nounwind uwtable readnone alwaysinline {
       %y = tail call float @__nv_acoshf(float %x) nounwind readnone
       ret float %y
}

define weak_odr double @acosh_f64(double %x) nounwind uwtable readnone alwaysinline {
       %y = tail call double @__nv_acosh(double %x) nounwind readnone
       ret double %y
}

declare float @__nv_tanhf(float) nounwind readnone
declare double @__nv_tanh(double) nounwind readnone

define weak_odr float @tanh_f32(float %x) nounwind uwtable readnone alwaysinline {
       %y = tail call float @__nv_tanhf(float %x) nounwind readnone
       ret float %y
}

define weak_odr double @tanh_f64(double %x) nounwind uwtable readnone alwaysinline {
       %y = tail call double @__nv_tanh(double %x) nounwind readnone
       ret double %y
}

declare float @__nv_atanhf(float) nounwind readnone
declare double @__nv_atanh(double) nounwind readnone

define weak_odr float @atanh_f32(float %x) nounwind uwtable readnone alwaysinline {
       %y = tail call float @__nv_atanhf(float %x) nounwind readnone
       ret float %y
}

define weak_odr double @atanh_f64(double %x) nounwind uwtable readnone alwaysinline {
       %y = tail call double @__nv_atanh(double %x) nounwind readnone
       ret double %y
}

define weak_odr i32 @halide_ptx_trap() nounwind uwtable alwaysinline {
       tail call void asm sideeffect "
       trap;
       ", ""() nounwind
       ret i32 0
}

; llvm doesn't expose dot product instructions as intrinsics
define weak_odr i32 @dp4a_s32_s32(<4 x i8> %a, <4 x i8> %b, i32 %i) nounwind readnone alwaysinline {
       %a_32 = bitcast <4 x i8> %a to i32
       %b_32 = bitcast <4 x i8> %b to i32
       %d = tail call i32 asm "dp4a.s32.s32    $0, $1, $2, $3;", "=r,r,r,r"(i32 %a_32, i32 %b_32, i32 %i) nounwind readnone
       ret i32 %d
}

define weak_odr i32 @dp4a_s32_u32(<4 x i8> %a, <4 x i8> %b, i32 %i) nounwind readnone alwaysinline {
       %a_32 = bitcast <4 x i8> %a to i32
       %b_32 = bitcast <4 x i8> %b to i32
       %d = tail call i32 asm "dp4a.s32.u32    $0, $1, $2, $3;", "=r,r,r,r"(i32 %a_32, i32 %b_32, i32 %i) nounwind readnone
       ret i32 %d
}

define weak_odr i32 @dp4a_u32_s32(<4 x i8> %a, <4 x i8> %b, i32 %i) nounwind readnone alwaysinline {
       %a_32 = bitcast <4 x i8> %a to i32
       %b_32 = bitcast <4 x i8> %b to i32
       %d = tail call i32 asm "dp4a.u32.s32    $0, $1, $2, $3;", "=r,r,r,r"(i32 %a_32, i32 %b_32, i32 %i) nounwind readnone
       ret i32 %d
}

define weak_odr i32 @dp4a_u32_u32(<4 x i8> %a, <4 x i8> %b, i32 %i) nounwind readnone alwaysinline {
       %a_32 = bitcast <4 x i8> %a to i32
       %b_32 = bitcast <4 x i8> %b to i32
       %d = tail call i32 asm "dp4a.u32.u32    $0, $1, $2, $3;", "=r,r,r,r"(i32 %a_32, i32 %b_32, i32 %i) nounwind readnone
       ret i32 %d
}


define weak_odr i32 @dp2a_s32_s32(<4 x i16> %a, <4 x i8> %b, i32 %i) nounwind readnone alwaysinline {
       %a_32 = bitcast <4 x i16> %a to <2 x i32>
       %a_lo = extractelement <2 x i32> %a_32, i32 0
       %a_hi = extractelement <2 x i32> %a_32, i32 1
       %b_32 = bitcast <4 x i8> %b to i32
       %d = tail call i32 asm "dp2a.lo.s32.s32    $0, $1, $3, $4; dp2a.hi.s32.s32    $0, $2, $3, $0;", "=r,r,r,r,r"(i32 %a_lo, i32 %a_hi, i32 %b_32, i32 %i) nounwind readnone
       ret i32 %d
}

define weak_odr i32 @dp2a_s32_u32(<4 x i16> %a, <4 x i8> %b, i32 %i) nounwind readnone alwaysinline {
       %a_32 = bitcast <4 x i16> %a to <2 x i32>
       %a_lo = extractelement <2 x i32> %a_32, i32 0
       %a_hi = extractelement <2 x i32> %a_32, i32 1
       %b_32 = bitcast <4 x i8> %b to i32
       %d = tail call i32 asm "dp2a.lo.s32.u32    $0, $1, $3, $4; dp2a.hi.s32.u32    $0, $2, $3, $0;", "=r,r,r,r,r"(i32 %a_lo, i32 %a_hi, i32 %b_32, i32 %i) nounwind readnone
       ret i32 %d
}

define weak_odr i32 @dp2a_u32_s32(<4 x i16> %a, <4 x i8> %b, i32 %i) nounwind readnone alwaysinline {
       %a_32 = bitcast <4 x i16> %a to <2 x i32>
       %a_lo = extractelement <2 x i32> %a_32, i32 0
       %a_hi = extractelement <2 x i32> %a_32, i32 1
       %b_32 = bitcast <4 x i8> %b to i32
       %d = tail call i32 asm "dp2a.lo.u32.s32    $0, $1, $3, $4; dp2a.hi.u32.s32    $0, $2, $3, $0;", "=r,r,r,r,r"(i32 %a_lo, i32 %a_hi, i32 %b_32, i32 %i) nounwind readnone
       ret i32 %d
}

define weak_odr i32 @dp2a_u32_u32(<4 x i16> %a, <4 x i8> %b, i32 %i) nounwind readnone alwaysinline {
       %a_32 = bitcast <4 x i16> %a to <2 x i32>
       %a_lo = extractelement <2 x i32> %a_32, i32 0
       %a_hi = extractelement <2 x i32> %a_32, i32 1
       %b_32 = bitcast <4 x i8> %b to i32
       %d = tail call i32 asm "dp2a.lo.u32.u32    $0, $1, $3, $4; dp2a.hi.u32.u32    $0, $2, $3, $0;", "=r,r,r,r,r"(i32 %a_lo, i32 %a_hi, i32 %b_32, i32 %i) nounwind readnone
       ret i32 %d
}

declare {<2 x half>, <2 x half>, <2 x half>, <2 x half>, <2 x half>, <2 x half>, <2 x half>, <2 x half>} @llvm.nvvm.wmma.m16n16k16.load.a.row.f16.p0i8(i8 addrspace(0)* %src );
declare {<2 x half>, <2 x half>, <2 x half>, <2 x half>, <2 x half>, <2 x half>, <2 x half>, <2 x half>} @llvm.nvvm.wmma.m16n16k16.load.b.row.f16.p0i8(i8 addrspace(0)* %src );
declare {float, float, float, float, float, float, float, float} @llvm.nvvm.wmma.m16n16k16.load.c.row.f32.p0i8(i8 addrspace(0)* %src );
declare {float, float, float, float, float, float, float, float} @llvm.nvvm.wmma.m16n16k16.mma.row.row.f32.f32(
        <2 x half> %a0, <2 x half> %a1, <2 x half> %a2, <2 x half> %a3, <2 x half> %a4, <2 x half> %a5, <2 x half> %a6, <2 x half> %a7,
        <2 x half> %b0, <2 x half> %b1, <2 x half> %b2, <2 x half> %b3, <2 x half> %b4, <2 x half> %b5, <2 x half> %b6, <2 x half> %b7,
        float %c0, float %c1, float %c2, float %c3, float %c4, float %c5, float %c6, float %c7);
declare void @llvm.nvvm.wmma.m16n16k16.store.d.row.f32.p0i8(i8 addrspace(0)* %src, float %d0, float %d1, float %d2, float %d3, float %d4, float %d5, float %d6, float %d7);

define weak_odr i8 addrspace(0)* @wmma.m16n16k16.mma.f32.f32(i8 addrspace(0)* %a, i8 addrspace(0)* %b, i8 addrspace(0)* %c, i8 addrspace(0)* %d) nounwind readnone alwaysinline {
    %v0 = call {<2 x half>, <2 x half>, <2 x half>, <2 x half>, <2 x half>, <2 x half>, <2 x half>, <2 x half>} @llvm.nvvm.wmma.m16n16k16.load.a.row.f16.p0i8(i8 addrspace(0)* %a );
    %a0 = extractvalue {<2 x half>, <2 x half>, <2 x half>, <2 x half>, <2 x half>, <2 x half>, <2 x half>, <2 x half>} %v0, 0
    %a1 = extractvalue {<2 x half>, <2 x half>, <2 x half>, <2 x half>, <2 x half>, <2 x half>, <2 x half>, <2 x half>} %v0, 1
    %a2 = extractvalue {<2 x half>, <2 x half>, <2 x half>, <2 x half>, <2 x half>, <2 x half>, <2 x half>, <2 x half>} %v0, 2
    %a3 = extractvalue {<2 x half>, <2 x half>, <2 x half>, <2 x half>, <2 x half>, <2 x half>, <2 x half>, <2 x half>} %v0, 3
    %a4 = extractvalue {<2 x half>, <2 x half>, <2 x half>, <2 x half>, <2 x half>, <2 x half>, <2 x half>, <2 x half>} %v0, 4
    %a5 = extractvalue {<2 x half>, <2 x half>, <2 x half>, <2 x half>, <2 x half>, <2 x half>, <2 x half>, <2 x half>} %v0, 5
    %a6 = extractvalue {<2 x half>, <2 x half>, <2 x half>, <2 x half>, <2 x half>, <2 x half>, <2 x half>, <2 x half>} %v0, 6
    %a7 = extractvalue {<2 x half>, <2 x half>, <2 x half>, <2 x half>, <2 x half>, <2 x half>, <2 x half>, <2 x half>} %v0, 7

    %v1 = call {<2 x half>, <2 x half>, <2 x half>, <2 x half>, <2 x half>, <2 x half>, <2 x half>, <2 x half>} @llvm.nvvm.wmma.m16n16k16.load.b.row.f16.p0i8(i8 addrspace(0)* %b );    
    %b0 = extractvalue {<2 x half>, <2 x half>, <2 x half>, <2 x half>, <2 x half>, <2 x half>, <2 x half>, <2 x half>} %v1, 0
    %b1 = extractvalue {<2 x half>, <2 x half>, <2 x half>, <2 x half>, <2 x half>, <2 x half>, <2 x half>, <2 x half>} %v1, 1
    %b2 = extractvalue {<2 x half>, <2 x half>, <2 x half>, <2 x half>, <2 x half>, <2 x half>, <2 x half>, <2 x half>} %v1, 2
    %b3 = extractvalue {<2 x half>, <2 x half>, <2 x half>, <2 x half>, <2 x half>, <2 x half>, <2 x half>, <2 x half>} %v1, 3
    %b4 = extractvalue {<2 x half>, <2 x half>, <2 x half>, <2 x half>, <2 x half>, <2 x half>, <2 x half>, <2 x half>} %v1, 4
    %b5 = extractvalue {<2 x half>, <2 x half>, <2 x half>, <2 x half>, <2 x half>, <2 x half>, <2 x half>, <2 x half>} %v1, 5
    %b6 = extractvalue {<2 x half>, <2 x half>, <2 x half>, <2 x half>, <2 x half>, <2 x half>, <2 x half>, <2 x half>} %v1, 6
    %b7 = extractvalue {<2 x half>, <2 x half>, <2 x half>, <2 x half>, <2 x half>, <2 x half>, <2 x half>, <2 x half>} %v1, 7

    %v2 = call {float, float, float, float, float, float, float, float} @llvm.nvvm.wmma.m16n16k16.load.c.row.f32.p0i8(i8 addrspace(0)* %c)
    %c0 = extractvalue {float, float, float, float, float, float, float, float} %v2, 0
    %c1 = extractvalue {float, float, float, float, float, float, float, float} %v2, 1
    %c2 = extractvalue {float, float, float, float, float, float, float, float} %v2, 2
    %c3 = extractvalue {float, float, float, float, float, float, float, float} %v2, 3
    %c4 = extractvalue {float, float, float, float, float, float, float, float} %v2, 4
    %c5 = extractvalue {float, float, float, float, float, float, float, float} %v2, 5
    %c6 = extractvalue {float, float, float, float, float, float, float, float} %v2, 6
    %c7 = extractvalue {float, float, float, float, float, float, float, float} %v2, 7

    %v3 = call {float, float, float, float, float, float, float, float} @llvm.nvvm.wmma.m16n16k16.mma.row.row.f32.f32(
        <2 x half> %a0, <2 x half> %a1, <2 x half> %a2, <2 x half> %a3, <2 x half> %a4, <2 x half> %a5, <2 x half> %a6, <2 x half> %a7,
        <2 x half> %b0, <2 x half> %b1, <2 x half> %b2, <2 x half> %b3, <2 x half> %b4, <2 x half> %b5, <2 x half> %b6, <2 x half> %b7,
        float %c0, float %c1, float %c2, float %c3, float %c4, float %c5, float %c6, float %c7)
    %d0 = extractvalue {float, float, float, float, float, float, float, float} %v3, 0
    %d1 = extractvalue {float, float, float, float, float, float, float, float} %v3, 1
    %d2 = extractvalue {float, float, float, float, float, float, float, float} %v3, 2
    %d3 = extractvalue {float, float, float, float, float, float, float, float} %v3, 3
    %d4 = extractvalue {float, float, float, float, float, float, float, float} %v3, 4
    %d5 = extractvalue {float, float, float, float, float, float, float, float} %v3, 5
    %d6 = extractvalue {float, float, float, float, float, float, float, float} %v3, 6
    %d7 = extractvalue {float, float, float, float, float, float, float, float} %v3, 7

    call void @llvm.nvvm.wmma.m16n16k16.store.d.row.f32.p0i8(i8 addrspace(0)* %d, float %d0, float %d1, float %d2, float %d3, float %d4, float %d5, float %d6, float %d7);
    ret i8 addrspace(0)* %d
}

