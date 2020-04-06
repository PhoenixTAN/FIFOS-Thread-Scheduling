

void divide_by_zero_exception() {
    print("You just divide a zero.");
    __asm__ volatile("jmp schedule_finish");
}

