#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#define RAND_SEED 42

enum drm_sched_priority {
        DRM_SCHED_PRIO_MIN,
        DRM_SCHED_PRIO_NORMAL,
        DRM_SCHED_PRIO_HIGH_SW,
        DRM_SCHED_PRIO_HIGH_HW,
        DRM_SCHED_PRIO_KERNEL,

        DRM_SCHED_PRIO_SIZE
};

enum gfx_pipe_priority {
	AMDGPU_GFX_PIPE_PRIO_NONE,
	AMDGPU_GFX_PIPE_PRIO_NORMAL,
	AMDGPU_GFX_PIPE_PRIO_HIGH,

	AMDGPU_GFX_PIPE_PRIO_SIZE
};

const enum gfx_pipe_priority s2p_prio_map[] = {
	[DRM_SCHED_PRIO_MIN]     = AMDGPU_GFX_PIPE_PRIO_NORMAL,
	[DRM_SCHED_PRIO_NORMAL]  = AMDGPU_GFX_PIPE_PRIO_NORMAL,
	[DRM_SCHED_PRIO_HIGH_SW] = AMDGPU_GFX_PIPE_PRIO_NORMAL,
	[DRM_SCHED_PRIO_HIGH_HW] = AMDGPU_GFX_PIPE_PRIO_HIGH,
	[DRM_SCHED_PRIO_KERNEL]  = AMDGPU_GFX_PIPE_PRIO_HIGH,
};

int f_map(int param)
{
	return s2p_prio_map[param];
}

int f_switch(int param)
{
	switch (param) {
	case DRM_SCHED_PRIO_HIGH_HW:
	case DRM_SCHED_PRIO_KERNEL:
		return AMDGPU_GFX_PIPE_PRIO_HIGH;
	default:
		return AMDGPU_GFX_PIPE_PRIO_NORMAL;
	}
}

double test_with_f(const int test_size, int (*f)(int), const char *desc)
{
	volatile int *test_data = malloc(sizeof(int) * test_size);
	volatile int store_data;
	struct timeval  tv1, tv2;
	volatile double t_diff;
	int i;

	/* Generate random look-up data.
	 */
	srand(42);
	for (i = 0; i < test_size; i++)
		test_data[i] = rand() % DRM_SCHED_PRIO_SIZE;

	gettimeofday(&tv1, NULL);
	for (i = 0; i < test_size; i++)
		store_data = f(test_data[i]);

	gettimeofday(&tv2, NULL);

	free((void *) test_data);

	t_diff = (tv2.tv_usec - tv1.tv_usec) / 1e6 + tv2.tv_sec - tv1.tv_sec;
	printf("%6s: %9.6f\n", desc, t_diff);

	return store_data + t_diff;
}

int test_perf(const int test_size)
{
	int res;

	printf("Testing data size of %g lookups, in seconds\n",
	       (double) test_size);

	srand(RAND_SEED);
	res = test_with_f(test_size, f_map, "map");

	srand(RAND_SEED);
	res += test_with_f(test_size, f_switch, "switch");

	srand(RAND_SEED);
	res += test_with_f(test_size, f_map, "map");

	srand(RAND_SEED);
	res += test_with_f(test_size, f_switch, "switch");

	return res;
}

int main(void)
{
	test_perf(1e6);
	test_perf(1e9);
}
