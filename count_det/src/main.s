.text
.global count_det

count_det:
    push {r4-r9}
	ldr r1, [r0]
	ldr r2, [r0, #4]
	ldr r3, [r0, #8]
	ldr r4, [r0, #12]
	ldr r5, [r0, #16]
	ldr r6, [r0, #20]
	ldr r7, [r0, #24]
	ldr r8, [r0, #28]
	ldr r9, [r0, #32]
    mul r10, r1, r5
    mul r10, r10, r9
	mov r0,  r10
	mul r10, r2, r6
    mul r10, r10, r7
    add r0,  r0, r10
    mul r10, r3, r4
    mul r10, r10, r8
    add r0,  r0, r10
    mul r10, r3, r5
    mul r10, r10, r7
    sub r0,  r0, r10
    mul r10, r1, r6
    mul r10, r10, r8
    sub r0,  r0, r10
    mul r10, r2, r4
    mul r10, r10, r9
    sub r0,  r0, r10
    pop {r4-r9}
	bx lr
