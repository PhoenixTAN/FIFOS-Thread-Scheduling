.global context_protection

context_protection:

    testl %ESI,%ESI
    jz context_retrieve

	# context protection for the current thread
    pushfl          # push flag register
    pushal          # push EAX,EBX,ECX,EDX,ESP,EBP,ESI,EDI
    pushw %DS
    pushw %ES
    pushw %FS
    pushw %GS

    movl %ESP, (%ESI)   # protect stack pointer in the (ESI)

context_retrieve:
    # retrieve the context from (EDI) for the next thead
    movl (%EDI) , %ESP

    popw %GS
    popw %FS
    popw %ES
    popw %DS

    popal
    popfl

    ret
    