/** Generated by itemis CREATE code generator. */


#include "../src/sc_types.h"

#include "VendingMachine.h"
#include "VendingMachine_required.h"

/*! \file
Implementation of the state machine 'VendingMachine'
*/

#ifndef SC_UNUSED
#define SC_UNUSED(P) (void)(P)
#endif

/* prototypes of all internal functions */
static void enact_main_region_Operation(VendingMachine* handle);
static void enact_main_region_Refund(VendingMachine* handle);
static void enact_main_region_Buying(VendingMachine* handle);
static void enact_main_region_Buy_rejected(VendingMachine* handle);
static void enseq_main_region_Operation_default(VendingMachine* handle);
static void enseq_main_region_Refund_default(VendingMachine* handle);
static void enseq_main_region_Buying_default(VendingMachine* handle);
static void enseq_main_region_Buy_rejected_default(VendingMachine* handle);
static void enseq_main_region_default(VendingMachine* handle);
static void exseq_main_region_Operation(VendingMachine* handle);
static void exseq_main_region_Refund(VendingMachine* handle);
static void exseq_main_region_Buying(VendingMachine* handle);
static void exseq_main_region_Buy_rejected(VendingMachine* handle);
static void exseq_main_region(VendingMachine* handle);
static void react_main_region__choice_0(VendingMachine* handle);
static void react_main_region__entry_Default(VendingMachine* handle);

/*! State machine reactions. */
static sc_integer react(VendingMachine* handle, const sc_integer transitioned_before);

/*! The reactions of state Operation. */
static sc_integer main_region_Operation_react(VendingMachine* handle, const sc_integer transitioned_before);

/*! The reactions of state Refund. */
static sc_integer main_region_Refund_react(VendingMachine* handle, const sc_integer transitioned_before);

/*! The reactions of state Buying. */
static sc_integer main_region_Buying_react(VendingMachine* handle, const sc_integer transitioned_before);

/*! The reactions of state Buy rejected. */
static sc_integer main_region_Buy_rejected_react(VendingMachine* handle, const sc_integer transitioned_before);


static void clear_in_events(VendingMachine* handle);

static void micro_step(VendingMachine* handle);

/*! Performs a 'run to completion' step. */
static void run_cycle(VendingMachine* handle);






static void vendingmachine_eventqueue_init(vendingmachine_eventqueue * eq, vendingmachine_event *buffer, sc_integer capacity);
static sc_integer vendingmachine_eventqueue_size(vendingmachine_eventqueue * eq);
static void vendingmachine_event_init(vendingmachine_event * ev, VendingMachineEventID name);
static vendingmachine_event vendingmachine_eventqueue_pop(vendingmachine_eventqueue * eq);
static sc_boolean vendingmachine_eventqueue_push(vendingmachine_eventqueue * eq, vendingmachine_event ev);
static void vendingMachine_add_event_to_queue(vendingmachine_eventqueue * eq, VendingMachineEventID name);
static sc_boolean vendingMachine_dispatch_event(VendingMachine* handle, const vendingmachine_event * event);
static vendingmachine_event vendingMachine_get_next_event(VendingMachine* handle);
static sc_boolean vendingMachine_dispatch_next_event(VendingMachine* handle);


void vendingMachine_init(VendingMachine* handle)
{
	sc_integer i;
	
	for (i = 0; i < VENDINGMACHINE_MAX_ORTHOGONAL_STATES; ++i)
	{
		handle->stateConfVector[i] = VendingMachine_last_state;
	}
	
				
	clear_in_events(handle);
	
	
	/* Default init sequence for statechart VendingMachine */
	vendingMachine_set_credito(handle, 0);
	vendingMachine_set_produto(handle, 0);
	
	handle->isExecuting = bool_false;
	vendingmachine_eventqueue_init(&handle->in_event_queue, handle->in_buffer, VENDINGMACHINE_IN_EVENTQUEUE_BUFFERSIZE);
}

void vendingMachine_enter(VendingMachine* handle)
{
	/* Activates the state machine. */
	if (handle->isExecuting == bool_true)
	{ 
		return;
	} 
	handle->isExecuting = bool_true;
	/* Default enter sequence for statechart VendingMachine */
	enseq_main_region_default(handle);
	handle->isExecuting = bool_false;
}

void vendingMachine_exit(VendingMachine* handle)
{
	/* Deactivates the state machine. */
	if (handle->isExecuting == bool_true)
	{ 
		return;
	} 
	handle->isExecuting = bool_true;
	/* Default exit sequence for statechart VendingMachine */
	exseq_main_region(handle);
	handle->isExecuting = bool_false;
}

/*!
Can be used by the client code to trigger a run to completion step without raising an event.
*/
void vendingMachine_trigger_without_event(VendingMachine* handle) {
	run_cycle(handle);
}


sc_boolean vendingMachine_is_active(const VendingMachine* handle)
{
	sc_boolean result = bool_false;
	sc_integer i;
	
	for(i = 0; i < VENDINGMACHINE_MAX_ORTHOGONAL_STATES; i++)
	{
		result = result || handle->stateConfVector[i] != VendingMachine_last_state;
	}
	
	return result;
}

/* 
 * Always returns 'false' since this state machine can never become final.
 */
sc_boolean vendingMachine_is_final(const VendingMachine* handle)
{
	SC_UNUSED(handle);
	return bool_false;
}

sc_boolean vendingMachine_is_state_active(const VendingMachine* handle, VendingMachineStates state)
{
	sc_boolean result = bool_false;
	switch (state)
	{
		case VendingMachine_main_region_Operation :
			result = (sc_boolean) (handle->stateConfVector[SCVI_VENDINGMACHINE_MAIN_REGION_OPERATION] == VendingMachine_main_region_Operation
			);
				break;
		case VendingMachine_main_region_Refund :
			result = (sc_boolean) (handle->stateConfVector[SCVI_VENDINGMACHINE_MAIN_REGION_REFUND] == VendingMachine_main_region_Refund
			);
				break;
		case VendingMachine_main_region_Buying :
			result = (sc_boolean) (handle->stateConfVector[SCVI_VENDINGMACHINE_MAIN_REGION_BUYING] == VendingMachine_main_region_Buying
			);
				break;
		case VendingMachine_main_region_Buy_rejected :
			result = (sc_boolean) (handle->stateConfVector[SCVI_VENDINGMACHINE_MAIN_REGION_BUY_REJECTED] == VendingMachine_main_region_Buy_rejected
			);
				break;
			default:
				result = bool_false;
				break;
		}
		return result;
	}

static void clear_in_events(VendingMachine* handle)
{
	handle->iface.botao1euro_raised = bool_false;
	handle->iface.botao2euros_raised = bool_false;
	handle->iface.botao3_browse_raised = bool_false;
	handle->iface.botao4_select_raised = bool_false;
}

static void micro_step(VendingMachine* handle)
{
	switch(handle->stateConfVector[ 0 ])
	{
		case VendingMachine_main_region_Operation :
		{
			main_region_Operation_react(handle,-1);
			break;
		}
		case VendingMachine_main_region_Refund :
		{
			main_region_Refund_react(handle,-1);
			break;
		}
		case VendingMachine_main_region_Buying :
		{
			main_region_Buying_react(handle,-1);
			break;
		}
		case VendingMachine_main_region_Buy_rejected :
		{
			main_region_Buy_rejected_react(handle,-1);
			break;
		}
		default: 
			/* do nothing */
			break;
	}
}

static void run_cycle(VendingMachine* handle)
{
	/* Performs a 'run to completion' step. */
	if (handle->isExecuting == bool_true)
	{ 
		return;
	} 
	handle->isExecuting = bool_true;
	vendingMachine_dispatch_next_event(handle);
	do
	{ 
		micro_step(handle);
		clear_in_events(handle);
	} while (vendingMachine_dispatch_next_event(handle) == bool_true);
	handle->isExecuting = bool_false;
}


void vendingMachine_raise_botao1euro(VendingMachine* handle)
{
	vendingMachine_add_event_to_queue(&(handle->in_event_queue), VendingMachine_botao1euro);
	run_cycle(handle);
}

void vendingMachine_raise_botao2euros(VendingMachine* handle)
{
	vendingMachine_add_event_to_queue(&(handle->in_event_queue), VendingMachine_botao2euros);
	run_cycle(handle);
}

void vendingMachine_raise_botao3_browse(VendingMachine* handle)
{
	vendingMachine_add_event_to_queue(&(handle->in_event_queue), VendingMachine_botao3_browse);
	run_cycle(handle);
}

void vendingMachine_raise_botao4_select(VendingMachine* handle)
{
	vendingMachine_add_event_to_queue(&(handle->in_event_queue), VendingMachine_botao4_select);
	run_cycle(handle);
}



sc_integer vendingMachine_get_credito(const VendingMachine* handle)
{
	return handle->iface.credito;
}
void vendingMachine_set_credito(VendingMachine* handle, sc_integer value)
{
	handle->iface.credito = value;
}
sc_integer vendingMachine_get_produto(const VendingMachine* handle)
{
	return handle->iface.produto;
}
void vendingMachine_set_produto(VendingMachine* handle, sc_integer value)
{
	handle->iface.produto = value;
}



/* implementations of all internal functions */

/* Entry action for state 'Operation'. */
static void enact_main_region_Operation(VendingMachine* handle)
{
	/* Entry action for state 'Operation'. */
	vendingMachine_currentState(handle,0);
}

/* Entry action for state 'Refund'. */
static void enact_main_region_Refund(VendingMachine* handle)
{
	/* Entry action for state 'Refund'. */
	vendingMachine_currentState(handle,1);
	vendingMachine_set_credito(handle, 0);
}

/* Entry action for state 'Buying'. */
static void enact_main_region_Buying(VendingMachine* handle)
{
	/* Entry action for state 'Buying'. */
	vendingMachine_currentState(handle,2);
}

/* Entry action for state 'Buy rejected'. */
static void enact_main_region_Buy_rejected(VendingMachine* handle)
{
	/* Entry action for state 'Buy rejected'. */
	vendingMachine_currentState(handle,3);
}

/* 'default' enter sequence for state Operation */
static void enseq_main_region_Operation_default(VendingMachine* handle)
{
	/* 'default' enter sequence for state Operation */
	enact_main_region_Operation(handle);
	handle->stateConfVector[0] = VendingMachine_main_region_Operation;
}

/* 'default' enter sequence for state Refund */
static void enseq_main_region_Refund_default(VendingMachine* handle)
{
	/* 'default' enter sequence for state Refund */
	enact_main_region_Refund(handle);
	handle->stateConfVector[0] = VendingMachine_main_region_Refund;
}

/* 'default' enter sequence for state Buying */
static void enseq_main_region_Buying_default(VendingMachine* handle)
{
	/* 'default' enter sequence for state Buying */
	enact_main_region_Buying(handle);
	handle->stateConfVector[0] = VendingMachine_main_region_Buying;
}

/* 'default' enter sequence for state Buy rejected */
static void enseq_main_region_Buy_rejected_default(VendingMachine* handle)
{
	/* 'default' enter sequence for state Buy rejected */
	enact_main_region_Buy_rejected(handle);
	handle->stateConfVector[0] = VendingMachine_main_region_Buy_rejected;
}

/* 'default' enter sequence for region main region */
static void enseq_main_region_default(VendingMachine* handle)
{
	/* 'default' enter sequence for region main region */
	react_main_region__entry_Default(handle);
}

/* Default exit sequence for state Operation */
static void exseq_main_region_Operation(VendingMachine* handle)
{
	/* Default exit sequence for state Operation */
	handle->stateConfVector[0] = VendingMachine_last_state;
}

/* Default exit sequence for state Refund */
static void exseq_main_region_Refund(VendingMachine* handle)
{
	/* Default exit sequence for state Refund */
	handle->stateConfVector[0] = VendingMachine_last_state;
}

/* Default exit sequence for state Buying */
static void exseq_main_region_Buying(VendingMachine* handle)
{
	/* Default exit sequence for state Buying */
	handle->stateConfVector[0] = VendingMachine_last_state;
}

/* Default exit sequence for state Buy rejected */
static void exseq_main_region_Buy_rejected(VendingMachine* handle)
{
	/* Default exit sequence for state Buy rejected */
	handle->stateConfVector[0] = VendingMachine_last_state;
}

/* Default exit sequence for region main region */
static void exseq_main_region(VendingMachine* handle)
{
	/* Default exit sequence for region main region */
	/* Handle exit of all possible states (of VendingMachine.main_region) at position 0... */
	switch(handle->stateConfVector[ 0 ])
	{
		case VendingMachine_main_region_Operation :
		{
			exseq_main_region_Operation(handle);
			break;
		}
		case VendingMachine_main_region_Refund :
		{
			exseq_main_region_Refund(handle);
			break;
		}
		case VendingMachine_main_region_Buying :
		{
			exseq_main_region_Buying(handle);
			break;
		}
		case VendingMachine_main_region_Buy_rejected :
		{
			exseq_main_region_Buy_rejected(handle);
			break;
		}
		default: 
			/* do nothing */
			break;
	}
}

/* The reactions of state null. */
static void react_main_region__choice_0(VendingMachine* handle)
{
	/* The reactions of state null. */
	if ((((handle->iface.credito - handle->iface.produto))) >= (0))
	{ 
		vendingMachine_set_credito(handle, handle->iface.credito - handle->iface.produto);
		vendingMachine_leds(handle,4);
		enseq_main_region_Buying_default(handle);
	}  else
	{
		enseq_main_region_Buy_rejected_default(handle);
	}
}

/* Default react sequence for initial entry  */
static void react_main_region__entry_Default(VendingMachine* handle)
{
	/* Default react sequence for initial entry  */
	enseq_main_region_Operation_default(handle);
}


static sc_integer react(VendingMachine* handle, const sc_integer transitioned_before)
{
	/* State machine reactions. */
	SC_UNUSED(handle);
	return transitioned_before;
}

static sc_integer main_region_Operation_react(VendingMachine* handle, const sc_integer transitioned_before)
{
	/* The reactions of state Operation. */
 			sc_integer transitioned_after = transitioned_before;
	if ((transitioned_after) < (0))
	{ 
		if (((handle->iface.botao4_select_raised) == bool_true) && (((handle->iface.produto) == (0)) == bool_true))
		{ 
			exseq_main_region_Operation(handle);
			vendingMachine_leds(handle,5);
			enseq_main_region_Refund_default(handle);
			react(handle,0);
			transitioned_after = 0;
		}  else
		{
			if (((handle->iface.botao4_select_raised) == bool_true) && (((handle->iface.produto) != (0)) == bool_true))
			{ 
				exseq_main_region_Operation(handle);
				react_main_region__choice_0(handle);
				transitioned_after = 0;
			}  else
			{
				if (handle->iface.botao1euro_raised == bool_true)
				{ 
					exseq_main_region_Operation(handle);
					vendingMachine_set_credito(handle, handle->iface.credito + 1);
					enseq_main_region_Operation_default(handle);
					react(handle,0);
					transitioned_after = 0;
				}  else
				{
					if (handle->iface.botao2euros_raised == bool_true)
					{ 
						exseq_main_region_Operation(handle);
						vendingMachine_set_credito(handle, handle->iface.credito + 2);
						enseq_main_region_Operation_default(handle);
						react(handle,0);
						transitioned_after = 0;
					}  else
					{
						if (handle->iface.botao3_browse_raised == bool_true)
						{ 
							exseq_main_region_Operation(handle);
							vendingMachine_set_produto(handle, (((handle->iface.produto + 1)) % 4));
							vendingMachine_leds(handle,handle->iface.produto);
							enseq_main_region_Operation_default(handle);
							react(handle,0);
							transitioned_after = 0;
						} 
					}
				}
			}
		}
	} 
	/* If no transition was taken */
	if ((transitioned_after) == (transitioned_before))
	{ 
		/* then execute local reactions. */
		transitioned_after = react(handle,transitioned_before);
	} 
	return transitioned_after;
}

static sc_integer main_region_Refund_react(VendingMachine* handle, const sc_integer transitioned_before)
{
	/* The reactions of state Refund. */
 			sc_integer transitioned_after = transitioned_before;
	if ((transitioned_after) < (0))
	{ 
		if (handle->iface.botao4_select_raised == bool_true)
		{ 
			exseq_main_region_Refund(handle);
			vendingMachine_leds(handle,handle->iface.produto);
			enseq_main_region_Operation_default(handle);
			react(handle,0);
			transitioned_after = 0;
		} 
	} 
	/* If no transition was taken */
	if ((transitioned_after) == (transitioned_before))
	{ 
		/* then execute local reactions. */
		transitioned_after = react(handle,transitioned_before);
	} 
	return transitioned_after;
}

static sc_integer main_region_Buying_react(VendingMachine* handle, const sc_integer transitioned_before)
{
	/* The reactions of state Buying. */
 			sc_integer transitioned_after = transitioned_before;
	if ((transitioned_after) < (0))
	{ 
		if (handle->iface.botao4_select_raised == bool_true)
		{ 
			exseq_main_region_Buying(handle);
			vendingMachine_leds(handle,handle->iface.produto);
			enseq_main_region_Operation_default(handle);
			react(handle,0);
			transitioned_after = 0;
		} 
	} 
	/* If no transition was taken */
	if ((transitioned_after) == (transitioned_before))
	{ 
		/* then execute local reactions. */
		transitioned_after = react(handle,transitioned_before);
	} 
	return transitioned_after;
}

static sc_integer main_region_Buy_rejected_react(VendingMachine* handle, const sc_integer transitioned_before)
{
	/* The reactions of state Buy rejected. */
 			sc_integer transitioned_after = transitioned_before;
	if ((transitioned_after) < (0))
	{ 
		if (handle->iface.botao4_select_raised == bool_true)
		{ 
			exseq_main_region_Buy_rejected(handle);
			enseq_main_region_Operation_default(handle);
			react(handle,0);
			transitioned_after = 0;
		} 
	} 
	/* If no transition was taken */
	if ((transitioned_after) == (transitioned_before))
	{ 
		/* then execute local reactions. */
		transitioned_after = react(handle,transitioned_before);
	} 
	return transitioned_after;
}




static void vendingmachine_eventqueue_init(vendingmachine_eventqueue * eq, vendingmachine_event *buffer, sc_integer capacity)
{
	eq->events = buffer;
	eq->capacity = capacity;
	eq->push_index = 0;
	eq->pop_index = 0;
	eq->size = 0;
}

static sc_integer vendingmachine_eventqueue_size(vendingmachine_eventqueue * eq)
{
	return eq->size;
}

static vendingmachine_event vendingmachine_eventqueue_pop(vendingmachine_eventqueue * eq)
{
	vendingmachine_event event;
	if(vendingmachine_eventqueue_size(eq) <= 0) {
		vendingmachine_event_init(&event, VendingMachine_invalid_event);
	}
	else {
		event = eq->events[eq->pop_index];
		
		if(eq->pop_index < eq->capacity - 1) {
			eq->pop_index++;
		} 
		else {
			eq->pop_index = 0;
		}
		eq->size--;
	}
	return event;
}
static sc_boolean vendingmachine_eventqueue_push(vendingmachine_eventqueue * eq, vendingmachine_event ev)
{
	if(vendingmachine_eventqueue_size(eq) == eq->capacity) {
		return bool_false;
	}
	else {
		eq->events[eq->push_index] = ev;
		
		if(eq->push_index < eq->capacity - 1) {
			eq->push_index++;
		}
		else {
			eq->push_index = 0;
		}
		eq->size++;
		
		return bool_true;
	}
}
static void vendingmachine_event_init(vendingmachine_event * ev, VendingMachineEventID name)
{
	ev->name = name;
}

static void vendingMachine_add_event_to_queue(vendingmachine_eventqueue * eq, VendingMachineEventID name)
{
	vendingmachine_event event;
	vendingmachine_event_init(&event, name);
	vendingmachine_eventqueue_push(eq, event);
}

static sc_boolean vendingMachine_dispatch_event(VendingMachine* handle, const vendingmachine_event * event) {
	switch(event->name) {
		case VendingMachine_botao1euro:
		{
			handle->iface.botao1euro_raised = bool_true;
			return bool_true;
		}
		case VendingMachine_botao2euros:
		{
			handle->iface.botao2euros_raised = bool_true;
			return bool_true;
		}
		case VendingMachine_botao3_browse:
		{
			handle->iface.botao3_browse_raised = bool_true;
			return bool_true;
		}
		case VendingMachine_botao4_select:
		{
			handle->iface.botao4_select_raised = bool_true;
			return bool_true;
		}
		default:
			return bool_false;
	}
}

static vendingmachine_event vendingMachine_get_next_event(VendingMachine* handle)
{
	vendingmachine_event next_event;
	vendingmachine_event_init(&next_event, VendingMachine_invalid_event);
	if(vendingmachine_eventqueue_size(&(handle->in_event_queue)) > 0) {
		next_event = vendingmachine_eventqueue_pop(&(handle->in_event_queue));
	}
	return next_event;
}

static sc_boolean vendingMachine_dispatch_next_event(VendingMachine* handle)
{
	vendingmachine_event nextEvent;
	nextEvent = vendingMachine_get_next_event(handle);
	return vendingMachine_dispatch_event(handle, &nextEvent);
}
