/* auto-generated by gen_syscalls.py, don't edit */

#ifndef Z_INCLUDE_SYSCALLS_IPM_H
#define Z_INCLUDE_SYSCALLS_IPM_H


#include <zephyr/tracing/tracing_syscall.h>

#ifndef _ASMLANGUAGE

#include <stdarg.h>

#include <syscall_list.h>
#include <zephyr/syscall.h>

#include <zephyr/linker/sections.h>


#ifdef __cplusplus
extern "C" {
#endif

extern int z_impl_ipm_send(const struct device * ipmdev, int wait, uint32_t id, const void * data, int size);

__pinned_func
static inline int ipm_send(const struct device * ipmdev, int wait, uint32_t id, const void * data, int size)
{
#ifdef CONFIG_USERSPACE
	if (z_syscall_trap()) {
		union { uintptr_t x; const struct device * val; } parm0 = { .val = ipmdev };
		union { uintptr_t x; int val; } parm1 = { .val = wait };
		union { uintptr_t x; uint32_t val; } parm2 = { .val = id };
		union { uintptr_t x; const void * val; } parm3 = { .val = data };
		union { uintptr_t x; int val; } parm4 = { .val = size };
		return (int) arch_syscall_invoke5(parm0.x, parm1.x, parm2.x, parm3.x, parm4.x, K_SYSCALL_IPM_SEND);
	}
#endif
	compiler_barrier();
	return z_impl_ipm_send(ipmdev, wait, id, data, size);
}

#if defined(CONFIG_TRACING_SYSCALL)
#ifndef DISABLE_SYSCALL_TRACING

#define ipm_send(ipmdev, wait, id, data, size) ({ 	int syscall__retval; 	sys_port_trace_syscall_enter(K_SYSCALL_IPM_SEND, ipm_send, ipmdev, wait, id, data, size); 	syscall__retval = ipm_send(ipmdev, wait, id, data, size); 	sys_port_trace_syscall_exit(K_SYSCALL_IPM_SEND, ipm_send, ipmdev, wait, id, data, size, syscall__retval); 	syscall__retval; })
#endif
#endif


extern int z_impl_ipm_max_data_size_get(const struct device * ipmdev);

__pinned_func
static inline int ipm_max_data_size_get(const struct device * ipmdev)
{
#ifdef CONFIG_USERSPACE
	if (z_syscall_trap()) {
		union { uintptr_t x; const struct device * val; } parm0 = { .val = ipmdev };
		return (int) arch_syscall_invoke1(parm0.x, K_SYSCALL_IPM_MAX_DATA_SIZE_GET);
	}
#endif
	compiler_barrier();
	return z_impl_ipm_max_data_size_get(ipmdev);
}

#if defined(CONFIG_TRACING_SYSCALL)
#ifndef DISABLE_SYSCALL_TRACING

#define ipm_max_data_size_get(ipmdev) ({ 	int syscall__retval; 	sys_port_trace_syscall_enter(K_SYSCALL_IPM_MAX_DATA_SIZE_GET, ipm_max_data_size_get, ipmdev); 	syscall__retval = ipm_max_data_size_get(ipmdev); 	sys_port_trace_syscall_exit(K_SYSCALL_IPM_MAX_DATA_SIZE_GET, ipm_max_data_size_get, ipmdev, syscall__retval); 	syscall__retval; })
#endif
#endif


extern uint32_t z_impl_ipm_max_id_val_get(const struct device * ipmdev);

__pinned_func
static inline uint32_t ipm_max_id_val_get(const struct device * ipmdev)
{
#ifdef CONFIG_USERSPACE
	if (z_syscall_trap()) {
		union { uintptr_t x; const struct device * val; } parm0 = { .val = ipmdev };
		return (uint32_t) arch_syscall_invoke1(parm0.x, K_SYSCALL_IPM_MAX_ID_VAL_GET);
	}
#endif
	compiler_barrier();
	return z_impl_ipm_max_id_val_get(ipmdev);
}

#if defined(CONFIG_TRACING_SYSCALL)
#ifndef DISABLE_SYSCALL_TRACING

#define ipm_max_id_val_get(ipmdev) ({ 	uint32_t syscall__retval; 	sys_port_trace_syscall_enter(K_SYSCALL_IPM_MAX_ID_VAL_GET, ipm_max_id_val_get, ipmdev); 	syscall__retval = ipm_max_id_val_get(ipmdev); 	sys_port_trace_syscall_exit(K_SYSCALL_IPM_MAX_ID_VAL_GET, ipm_max_id_val_get, ipmdev, syscall__retval); 	syscall__retval; })
#endif
#endif


extern int z_impl_ipm_set_enabled(const struct device * ipmdev, int enable);

__pinned_func
static inline int ipm_set_enabled(const struct device * ipmdev, int enable)
{
#ifdef CONFIG_USERSPACE
	if (z_syscall_trap()) {
		union { uintptr_t x; const struct device * val; } parm0 = { .val = ipmdev };
		union { uintptr_t x; int val; } parm1 = { .val = enable };
		return (int) arch_syscall_invoke2(parm0.x, parm1.x, K_SYSCALL_IPM_SET_ENABLED);
	}
#endif
	compiler_barrier();
	return z_impl_ipm_set_enabled(ipmdev, enable);
}

#if defined(CONFIG_TRACING_SYSCALL)
#ifndef DISABLE_SYSCALL_TRACING

#define ipm_set_enabled(ipmdev, enable) ({ 	int syscall__retval; 	sys_port_trace_syscall_enter(K_SYSCALL_IPM_SET_ENABLED, ipm_set_enabled, ipmdev, enable); 	syscall__retval = ipm_set_enabled(ipmdev, enable); 	sys_port_trace_syscall_exit(K_SYSCALL_IPM_SET_ENABLED, ipm_set_enabled, ipmdev, enable, syscall__retval); 	syscall__retval; })
#endif
#endif


extern void z_impl_ipm_complete(const struct device * ipmdev);

__pinned_func
static inline void ipm_complete(const struct device * ipmdev)
{
#ifdef CONFIG_USERSPACE
	if (z_syscall_trap()) {
		union { uintptr_t x; const struct device * val; } parm0 = { .val = ipmdev };
		(void) arch_syscall_invoke1(parm0.x, K_SYSCALL_IPM_COMPLETE);
		return;
	}
#endif
	compiler_barrier();
	z_impl_ipm_complete(ipmdev);
}

#if defined(CONFIG_TRACING_SYSCALL)
#ifndef DISABLE_SYSCALL_TRACING

#define ipm_complete(ipmdev) do { 	sys_port_trace_syscall_enter(K_SYSCALL_IPM_COMPLETE, ipm_complete, ipmdev); 	ipm_complete(ipmdev); 	sys_port_trace_syscall_exit(K_SYSCALL_IPM_COMPLETE, ipm_complete, ipmdev); } while(false)
#endif
#endif


#ifdef __cplusplus
}
#endif

#endif
#endif /* include guard */
