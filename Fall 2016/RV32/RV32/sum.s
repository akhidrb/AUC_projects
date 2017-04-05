main:
    li a7, 5        # enter the value
    SCALL

    mv a2, a7
    auipc a1, 0x0
    j sum

    li a7, 1
    SCALL

    li a7, 10
    SCALL

sum:
    li a0, 0
loop:
    ble a2, zero, stop
    add a0, a2, a0
    addi a2, a2, -1
    j loop

stop:
    addi a1, a1, 8
    jr a1

# should print the summation the values: enter 4 prints 10
