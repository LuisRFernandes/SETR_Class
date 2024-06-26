/* auto-generated by gen_syscalls.py, don't edit */

#ifndef Z_INCLUDE_SYSCALLS_UART_MUX_H
#define Z_INCLUDE_SYSCALLS_UART_MUX_H


#include <zephyr/tracing/tracing_syscall.h>

#ifndef _ASMLANGUAGE

#include <stdarg.h>

#include <syscall_list.h>
#include <zephyr/syscall.h>

#include <zephyr/linker/sections.h>


#ifdef __cplusplus
extern "C" {
#endif

extern const struct device * z_impl_uart_mux_find(int dlci_address);

__pinned_func
static inline const struct device * uart_mux_find(int dlci_address)
{
#ifdef CONFIG_USERSPACE
	if (z_syscall_trap()) {
		union { uintptr_t x; int val; } parm0 = { .val = dlci_address };
		return (const struct device *) arch_syscall_invoke1(parm0.x, K_SYSCALL_UART_MUX_FIND);
	}
#endif
	compiler_barrier();
	return z_impl_uart_mux_find(dlci_address);
}

#if defined(CONFIG_TRACING_SYSCALL)
#ifndef DISABLE_SYSCALL_TRACING

#define uart_mux_find(dlci_address) ({ 	const struct device * syscall__retval; 	sys_port_trace_syscall_enter(K_SYSCALL_UART_MUX_FIND, uart_mux_find, dlci_address); 	syscall__retval = uart_mux_find(dlci_address); 	sys_port_trace_syscall_exit(K_SYSCALL_UART_MUX_FIND, uart_mux_find, dlci_address, syscall__retval); 	syscall__retval; })
#endif
#endif


#ifdef __cplusplus
}
#endif

#endif
#endif /* include guard */
