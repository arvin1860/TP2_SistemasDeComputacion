.global convertir_gini
.type convertir_gini, @function

convertir_gini:
    pushq %rbp
    movq %rsp, %rbp

    # El noveno float esta en el stack
    movss 16(%rbp), %xmm0

    # Convertir float a entero
    cvttss2siq %xmm0, %rax

    # Sumar 1
    addq $1, %rax

    popq %rbp
    ret

.section .note.GNU-stack,"",@progbits
