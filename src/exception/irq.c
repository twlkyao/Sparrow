#include <linkage.h>
#include <interrupt.h>
#include <irq.h>


/*
 * NOP functions
 */
static void noop(unsigned int irq)
{
}

static unsigned int noop_ret(unsigned int irq)
{
	return 0;
}

static void ack_bad(unsigned int irq)
{
  return;
}

/*
 * Generic no controller implementation
 */
struct irq_chip no_irq_chip = {
	.name		= "none",
	.startup	= noop_ret,
	.shutdown	= noop,
	.enable		= noop,
	.disable	= noop,
	.ack		= ack_bad,
};

void handle_bad_irq(unsigned int irq, struct irq_desc *desc)
{
  printk("%s, irq = %d\n", __func__, irq);
  return;
}

struct irq_desc all_irq_desc[NR_IRQS] = {
	[0 ... NR_IRQS-1] = {
		.status = IRQ_DISABLED,
		.chip = &no_irq_chip,
		.handle_irq = handle_bad_irq,
		.depth = 1,
	}
};


void vector_irq_handler(void){
	printk("timer interrupt occured\n");
}

void __init init_IRQ(void) {
  struct irq_desc *desc;
  int irq;

  for (irq = 0; irq < NR_IRQS; irq++) {
	desc = &all_irq_desc[irq];
	desc->status |= IRQ_NOREQUEST | IRQ_NOPROBE;
  }

  arm_init_irq();
}

struct irq_desc *irq_to_desc(unsigned int irq)
{
	return (irq < NR_IRQS) ? all_irq_desc + irq : 0;
}

/**
 *	setup_irq - setup an interrupt
 *	@irq: Interrupt line to setup
 *	@act: irqaction for the interrupt
 *
 * Used to statically setup interrupts in the early boot process.
 */
int setup_irq(unsigned int irq, struct irqaction *act)
{
	struct irq_desc *desc = irq_to_desc(irq);
	desc->action = act;
	return 0;
}



void generic_handle_irq(unsigned int irq)
{
  struct irq_desc *desc = irq_to_desc(irq);

  if (0 != (unsigned int)desc->handler_data) {
	desc->chip->mask(irq);
	desc->chip->ack(irq);
	generic_handle_irq((unsigned int)desc->handler_data);
	desc->chip->unmask(irq);
  } else {
	desc->handle_irq(irq, irq_to_desc(irq));
  }
}
