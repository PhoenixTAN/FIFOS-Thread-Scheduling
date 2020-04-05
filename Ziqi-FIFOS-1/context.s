.global context_protection
.global context_retrieve

context_protection:
	# context protection for the current thread
    pushfl          # push flag register
    pushal          # push EAX,EBX,ECX,EDX,ESP,EBP,ESI,EDI
    movl %ESP, (%ESI)   # protect stack pointer in the (ESI)

context_retrieve:
    # retrieve the context from (EDI) for the next thead
    movl (%EDI) , %ESP
    popal
    popfl

    ret     # IP = SS:SP, SP = SP + 2

