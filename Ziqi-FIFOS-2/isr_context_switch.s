.global isr_wrapper

isr_wrapper:
    # pushfl
    pushal

    # pass 

    # call interrupt_handler 

    popal
    # popfl
    iret