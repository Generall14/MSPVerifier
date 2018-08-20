
#define LED1NR 0x10
#define LED2NR 0x80

_init_debLED macro
        bic.w   #LOCKLPM5, PM5CTL0
        bic.b   #LED1NR, P3SELC
        bis.b   #LED1NR, P3DIR
        bic.b   #LED1NR, P3OUT
        bic.b   #LED2NR, P4SELC
        bis.b   #LED2NR, P4DIR
        bic.b   #LED2NR, P4OUT
        endm

_LED1_ON macro
        bis.b   #LED1NR, P3OUT
        endm
          
_LED1_OFF macro
        bic.b   #LED1NR, P3OUT
        endm
          
_LED1_SWAP macro
        xor.b   #LED1NR, P3OUT
        endm
          
_LED2_ON macro
        bis.b   #LED2NR, P4OUT
        endm
          
_LED2_OFF macro
        bic.b   #LED2NR, P4OUT
        endm
          
_LED2_SWAP macro
        xor.b   #LED2NR, P4OUT
        endm
         
_IDLE_IN macro
        _LED2_OFF
        endm
                  
_IDLE_OUT macro
        _LED2_ON
        endm

;===============================================================================

_eint macro
        nop
        eint
        nop
        endm
          
_dint macro
        nop
        nop
        dint
        nop
        nop
        nop
        endm

_IRQ_IN macro
        extern  stack_irq_start_ptr
        mov.w   SP, stack_irq_start_ptr
        mov.w   #stack_irq_start_ptr, SP
        _LED1_ON
        endm
          
_IRQ_OUT macro
        mov.w   stack_irq_start_ptr, SP
        ;nop
        _LED1_OFF
        endm