/*
 * Copyright 2015 Advanced Micro Devices, Inc.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE COPYRIGHT HOLDER(S) OR AUTHOR(S) BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */

#ifndef __AMD_SHARED_H__
#define __AMD_SHARED_H__

enum amd_ip_block_type {
	AMD_IP_BLOCK_TYPE_COMMON,
	AMD_IP_BLOCK_TYPE_GMC,
	AMD_IP_BLOCK_TYPE_IH,
	AMD_IP_BLOCK_TYPE_SMC,
	AMD_IP_BLOCK_TYPE_DCE,
	AMD_IP_BLOCK_TYPE_GFX,
	AMD_IP_BLOCK_TYPE_SDMA,
	AMD_IP_BLOCK_TYPE_UVD,
	AMD_IP_BLOCK_TYPE_VCE,
};

enum amd_clockgating_state {
	AMD_CG_STATE_GATE = 0,
	AMD_CG_STATE_UNGATE,
};

enum amd_powergating_state {
	AMD_PG_STATE_GATE = 0,
	AMD_PG_STATE_UNGATE,
};

struct amd_ip_funcs {
	/* sets up early driver state (pre sw_init), does not configure hw - Optional */
	int (*early_init)(void *handle);
	/* sets up late driver/hw state (post hw_init) - Optional */
	int (*late_init)(void *handle);
	/* sets up driver state, does not configure hw */
	int (*sw_init)(void *handle);
	/* tears down driver state, does not configure hw */
	int (*sw_fini)(void *handle);
	/* sets up the hw state */
	int (*hw_init)(void *handle);
	/* tears down the hw state */
	int (*hw_fini)(void *handle);
	/* handles IP specific hw/sw changes for suspend */
	int (*suspend)(void *handle);
	/* handles IP specific hw/sw changes for resume */
	int (*resume)(void *handle);
	/* returns current IP block idle status */
	bool (*is_idle)(void *handle);
	/* poll for idle */
	int (*wait_for_idle)(void *handle);
	/* soft reset the IP block */
	int (*soft_reset)(void *handle);
	/* dump the IP block status registers */
	void (*print_status)(void *handle);
	/* enable/disable cg for the IP block */
	int (*set_clockgating_state)(void *handle,
				     enum amd_clockgating_state state);
	/* enable/disable pg for the IP block */
	int (*set_powergating_state)(void *handle,
				     enum amd_powergating_state state);
};

#endif /* __AMD_SHARED_H__ */
