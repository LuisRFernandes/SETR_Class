/* auto-generated by gen_syscalls.py, don't edit */

#ifndef Z_INCLUDE_SYSCALLS_SYSCON_H
#define Z_INCLUDE_SYSCALLS_SYSCON_H


#include <zephyr/tracing/tracing_syscall.h>

#ifndef _ASMLANGUAGE

#include <stdarg.h>

#include <syscall_list.h>
#include <zephyr/syscall.h>

#include <zephyr/linker/sections.h>


#ifdef __cplusplus
extern "C" {
#endif

extern int z_impl_syscon_get_base(const struct device * dev, uintptr_t * addr);

__pinned_func
static inline int syscon_get_base(const struct device * dev, uintptr_t * addr)
{
#ifdef CONFIG_USERSPACE
	if (z_syscall_trap()) {
		union { uintptr_t x; const struct device * val; } parm0 = { .val = dev };
		union { uintptr_t x; uintptr_t * val; } parm1 = { .val = addr };
		return (int) arch_syscall_invoke2(parm0.x, parm1.x, K_SYSCALL_SYSCON_GET_BASE);
	}
#endif
	compiler_barrier();
	return z_impl_syscon_get_base(dev, addr);
}

#if defined(CONFIG_TRACING_SYSCALL)
#ifndef DISABLE_SYSCALL_TRACING

#define syscon_get_base(dev, addr) ({ 	int syscall__retval; 	sys_port_trace_syscall_enter(K_SYSCALL_SYSCON_GET_BASE, syscon_get_base, dev, addr); 	syscall__retval = syscon_get_base(dev, addr); 	sys_port_trace_syscall_exit(K_SYSCALL_SYSCON_GET_BASE, syscon_get_base, dev, addr, syscall__retval); 	syscall__retval; })
#endif
#endif


extern int z_impl_syscon_read_reg(const struct device * dev, uint16_t reg, uint32_t * val);

__pinned_func
static inline int syscon_read_reg(const struct device * dev, uint16_t reg, uint32_t * val)
{
#ifdef CONFIG_USERSPACE
	if (z_syscall_trap()) {
		union { uintptr_t x; const struct device * val; } parm0 = { .val = dev };
		union { uintptr_t x; uint16_t val; } parm1 = { .val = reg };
		union { uintptr_t x; uint32_t * val; } parm2 = { .val = val };
		return (int) arch_syscall_invoke3(parm0.x, parm1.x, parm2.x, K_SYSCALL_SYSCON_READ_REG);
	}
#endif
	compiler_barrier();
	return z_impl_syscon_read_reg(dev, reg, val);
}

#if defined(CONFIG_TRACING_SYSCALL)
#ifndef DISABLE_SYSCALL_TRACING

#define syscon_read_reg(dev, reg, val) ({ 	int syscall__retval; 	sys_port_trace_syscall_enter(K_SYSCALL_SYSCON_READ_REG, syscon_read_reg, dev, reg, val); 	syscall__retval = syscon_read_reg(dev, reg, val); 	sys_port_trace_syscall_exit(K_SYSCALL_SYSCON_READ_REG, syscon_read_reg, dev, reg, val, syscall__retval); 	syscall__retval; })
#endif
#endif


extern int z_impl_syscon_write_reg(const struct device * dev, uint16_t reg, uint32_t val);

__pinned_func
static inline int syscon_write_reg(const struct device * dev, uint16_t reg, uint32_t val)
{
#ifdef CONFIG_USERSPACE
	if (z_syscall_trap()) {
		union { uintptr_t x; const struct device * val; } parm0 = { .val = dev };
		union { uintptr_t x; uint16_t val; } parm1 = { .val = reg };
		union { uintptr_t x; uint32_t val; } parm2 = { .val = val };
		return (int) arch_syscall_invoke3(parm0.x, parm1.x, parm2.x, K_SYSCALL_SYSCON_WRITE_REG);
	}
#endif
	compiler_barrier();
	return z_impl_syscon_write_reg(dev, reg, val);
}

#if defined(CONFIG_TRACING_SYSCALL)
#ifndef DISABLE_SYSCALL_TRACING

#define syscon_write_reg(dev, reg, val) ({ 	int syscall__retval; 	sys_port_trace_syscall_enter(K_SYSCALL_SYSCON_WRITE_REG, syscon_write_reg, dev, reg, val); 	syscall__retval = syscon_write_reg(dev, reg, val); 	sys_port_trace_syscall_exit(K_SYSCALL_SYSCON_WRITE_REG, syscon_write_reg, dev, reg, val, syscall__retval); 	syscall__retval; })
#endif
#endif


extern int z_impl_syscon_get_size(const struct device * dev, size_t * size);

__pinned_func
static inline int syscon_get_size(const struct device * dev, size_t * size)
{
#ifdef CONFIG_USERSPACE
	if (z_syscall_trap()) {
		union { uintptr_t x; const struct device * val; } parm0 = { .val = dev };
		union { uintptr_t x; size_t * val; } parm1 = { .val = size };
		return (int) arch_syscall_invoke2(parm0.x, parm1.x, K_SYSCALL_SYSCON_GET_SIZE);
	}
#endif
	compiler_barrier();
	return z_impl_syscon_get_size(dev, size);
}

#if defined(CONFIG_TRACING_SYSCALL)
#ifndef DISABLE_SYSCALL_TRACING

#define syscon_get_size(dev, size) ({ 	int syscall__retval; 	sys_port_trace_syscall_enter(K_SYSCALL_SYSCON_GET_SIZE, syscon_get_size, dev, size); 	syscall__retval = syscon_get_size(dev, size); 	sys_port_trace_syscall_exit(K_SYSCALL_SYSCON_GET_SIZE, syscon_get_size, dev, size, syscall__retval); 	syscall__retval; })
#endif
#endif


#ifdef __cplusplus
}
#endif

#endif
#endif /* include guard */
