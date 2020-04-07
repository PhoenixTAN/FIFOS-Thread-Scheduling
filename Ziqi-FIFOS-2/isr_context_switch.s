.global isr_context_protection
.global isr_context_retrieve

isr_context_protection:
    # retrieve the error code (if any)

    pushal
    pushw   %SS
    pushw   %DS
    pushw   %ES
    pushw   %FS
    pushw   %GS
    # pass some parameters to C if needed
    # call the handler code in C
    ret

isr_context_retrieve:
    popw    %GS
    popw    %FS
    popw    %ES
    popw    %DS
    popw    %SS
    popal

    iret

