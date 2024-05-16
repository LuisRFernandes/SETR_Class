/* auto-generated by gen_syscalls.py, don't edit */

#ifndef Z_INCLUDE_SYSCALLS_ENTROPY_H
#define Z_INCLUDE_SYSCALLS_ENTROPY_H


#include <zephyr/tracing/tracing_syscall.h>

#ifndef _ASMLANGUAGE

#include <stdarg.h>

#include <syscall_list.h>
#include <zephyr/syscall.h>

#include <zephyr/linker/sections.h>


#ifdef __cplusplus
extern "C" {
#endif

extern int z_impl_entropy_get_entropy(const struct device * dev, uint8_t * buffer, uint16_t length);

__pinned_func
static inline int entropy_get_entropy(const struct device * dev, uint8_t * buffer, uint16_t length)
{
#ifdef CONFIG_USERSPACE
	if (z_syscall_trap()) {
		union { uintptr_t x; const struct device * val; } parm0 = { .val = dev };
		union { uintptr_t x; uint8_t * val; } parm1 = { .val = buffer };
		union { uintptr_t x; uint16_t val; } parm2 = { .val = length };
		return (int) arch_syscall_invoke3(parm0.x, parm1.x, parm2.x, K_SYSCALL_ENTROPY_GET_ENTROPY);
	}
#endif
	compiler_barrier();
	return z_impl_entropy_get_entropy(dev, buffer, length);
}

#if defined(CONFIG_TRACING_SYSCALL)
#ifndef DISABLE_SYSCALL_TRACING

#define entropy_get_entropy(dev, buffer, length) ({ 	int syscall__retval; 	sys_port_trace_syscall_enter(K_SYSCALL_ENTROPY_GET_ENTROPY, entropy_get_entropy, dev, buffer, length); 	syscall__retval = entropy_get_entropy(dev, buffer, length); 	sys_port_trace_syscall_exit(K_SYSCALL_ENTROPY_GET_ENTROPY, entropy_get_entropy, dev, buffer, length, syscall__retval); 	syscall__retval; })
#endif
#endif


#ifdef __cplusplus
}
#endif

#endif
#endif /* include guard */
