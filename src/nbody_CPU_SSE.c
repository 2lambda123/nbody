/*
 *
 * nbody_CPU_SSE.cpp
 *
 * Multithreaded SSE CPU implementation of the O(N^2) N-body calculation.
 * Uses SOA (structure of arrays) representation because it is a much
 * better fit for SSE.
 *
 * Copyright (c) 2011-2012, Archaea Software, LLC.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 */

#ifdef __SSE__
#include "libtime.h"

#include "bodybodyInteraction_SSE.h"
#include "nbody_CPU_SIMD.h"

float
ComputeGravitation_SIMD(
    float *force[3],
    float * const pos[3],
    float * const mass,
    float softeningSquared,
    size_t N
)
{
    uint64_t start, end;

    start = libtime_cpu();
#pragma omp parallel for
    for ( size_t i = 0; i < N; i++ )
    {
        __m128 ax = _mm_setzero_ps();
        __m128 ay = _mm_setzero_ps();
        __m128 az = _mm_setzero_ps();
        __m128 *px = (__m128 *) pos[0];
        __m128 *py = (__m128 *) pos[1];
        __m128 *pz = (__m128 *) pos[2];
        __m128 *pmass = (__m128 *) mass;
        __m128 x0 = _mm_set_ps1( pos[0][i] );
        __m128 y0 = _mm_set_ps1( pos[1][i] );
        __m128 z0 = _mm_set_ps1( pos[2][i] );

        for ( size_t j = 0; j < N/4; j++ ) {

            bodyBodyInteraction(
                &ax, &ay, &az,
                x0, y0, z0,
                px[j], py[j], pz[j], pmass[j],
                _mm_set_ps1( softeningSquared ) );

        }
        // Accumulate sum of four floats in the SSE register
        ax = horizontal_sum_ps( ax );
        ay = horizontal_sum_ps( ay );
        az = horizontal_sum_ps( az );

        _mm_store_ss( (float *) &force[0][i], ax );
        _mm_store_ss( (float *) &force[1][i], ay );
        _mm_store_ss( (float *) &force[2][i], az );
    }

    end = libtime_cpu();

    return libtime_cpu_to_wall(end - start) * 1e-6;
}
#endif

/* vim: set ts=4 sts=4 sw=4 et: */
