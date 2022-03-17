#include "idt.h"
#include "lib.h"
#include "x86_desc.h"

/*initialize_idt
*Description: follows x86 standard OP and fills the IDT 
*Inputs: void
*Outputs: void
*Return Value: void
*Side Effects: initializes the IDT 
*/

void initialize_idt(void){
    int i; 
    for (i=0; i<NUM_VEC; i++){
        //set reserved and present bits
        idt[i].reserved0 = 0;
        idt[i].reserved1 = 1;
        idt[i].reserved2 = 1;
        idt[i].present = 1; 

        //set segment selector 
        idt[i].seg_selector = KERNEL_CS;

        // set size = 1 bc/ 32 bit
        idt[i].size = 1;

        //reserved bits if 0x20 <= i <= 0x2F
        if ((i >= 0x20) && (i <= 0x2F)){
            idt[i].reserved3= 0;
        }
        else idt[i].reserved3 = 1;

        // if i==0x80 set descriptor privelege level to 3, else 0 
        if (i== 0x80)
            idt[i].dpl = 0x03;
        else 
            idt[i].dpl = 0x00; 

        //finally populate IDT w default entries
        SET_IDT_ENTRY(idt[i], DEFAULT_EXCEPTION);
    }

    //fill the specific entries we care about (taken from x86 standards)
    SET_IDT_ENTRY(idt[0x00], EXCEPTION_DIVIDE_BY_ZERO);
    SET_IDT_ENTRY(idt[0x01], SINGLE_STEP_INTERRUPT);
    SET_IDT_ENTRY(idt[0x02], EXCEPTION_NMI);
    SET_IDT_ENTRY(idt[0x03], EXCEPTION_BREAKPOINT);
    SET_IDT_ENTRY(idt[0x04], EXCEPTION_OVERFLOW);
    SET_IDT_ENTRY(idt[0x05], EXCEPTION_BOUND_RANGE_EXCEEDED);
    SET_IDT_ENTRY(idt[0x06], EXCEPTION_INVALID_OPCODE);
    SET_IDT_ENTRY(idt[0x07], EXCEPTION_COPROCESSOR_NOT_AVAILABLE);
    SET_IDT_ENTRY(idt[0x08], EXCEPTION_DOUBLE_FAULT);
    SET_IDT_ENTRY(idt[0x09], EXCEPTION_COPROCESSOR_SEG_OVERRUN);
    SET_IDT_ENTRY(idt[0x0A], EXCEPTION_INVALID_TSS);
    SET_IDT_ENTRY(idt[0x0B], EXCEPTION_SEGMENT_NOT_PRESENT);
    SET_IDT_ENTRY(idt[0x0C], EXCEPTION_STACK_SEGMENT_FAULT);
    SET_IDT_ENTRY(idt[0x0D], EXCEPTION_GENERAL_PROTECTION);
    SET_IDT_ENTRY(idt[0x0E], EXCEPTION_PAGE_FAULT);
    SET_IDT_ENTRY(idt[0x0F], EXCEPTION_RESERVED);
    SET_IDT_ENTRY(idt[0x10], EXCEPTION_FLOATING_POINT);
    SET_IDT_ENTRY(idt[0x11], EXCEPTION_ALIGNMENT_CHECK);
    SET_IDT_ENTRY(idt[0x12], EXCEPTION_MACHINE_CHECK);
    SET_IDT_ENTRY(idt[0x13], EXCEPTION_SIMD_FLOATING_POINT);
    SET_IDT_ENTRY(idt[0x14], EXCEPTION_VIRTUALIZATION);
    SET_IDT_ENTRY(idt[0x15], EXCEPTION_CONTROL_PROTECTION);

    lidt(idt_desc_ptr); //specify size of IDT and set base address 
}
