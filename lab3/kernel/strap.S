.globl trap_handler
.globl swtch2user
.globl strap
.align 4
# strap:
#         # tp contains the address of the trapframe
#         # Save user registers to the trapframe.

#         # Save return address (ra)
#         sd ra, 40(tp)
#         # Save stack pointer (sp)
#         sd sp, 48(tp)
#         # Save global pointer (gp)
#         sd gp, 56(tp)
#         # Save temporary pointer (tp, not overwritten here)
#         sd tp, 64(tp)

#         # Save temporary registers
#         sd t0, 72(tp)
#         sd t1, 80(tp)
#         sd t2, 88(tp)

#         # Save saved registers
#         sd s0, 96(tp)
#         sd s1, 104(tp)

#         # Save function arguments / return values
#         sd a0, 112(tp)
#         sd a1, 120(tp)
#         sd a2, 128(tp)
#         sd a3, 136(tp)
#         sd a4, 144(tp)
#         sd a5, 152(tp)
#         sd a6, 160(tp)
#         sd a7, 168(tp)

#         # Save more saved registers
#         sd s2, 176(tp)
#         sd s3, 184(tp)
#         sd s4, 192(tp)
#         sd s5, 200(tp)
#         sd s6, 208(tp)
#         sd s7, 216(tp)
#         sd s8, 224(tp)
#         sd s9, 232(tp)
#         sd s10, 240(tp)
#         sd s11, 248(tp)

#         # Save additional temporary registers
#         sd t3, 256(tp)
#         sd t4, 264(tp)
#         sd t5, 272(tp)
#         sd t6, 280(tp)

#         # Call the C trap handler in trap.c
#         call trap_handler

#         # Restore registers from the trapframe
#         # Save return address (ra)
#         ld ra, 40(tp)
#         # Save stack pointer (sp)
#         ld sp, 48(tp)
#         # Save global pointer (gp)
#         ld gp, 56(tp)
#         # Save temporary pointer (tp, not overwritten here)
#         ld tp, 64(tp)

#         # Save temporary registers
#         ld t0, 72(tp)
#         ld t1, 80(tp)
#         ld t2, 88(tp)

#         # Save saved registers
#         ld s0, 96(tp)
#         ld s1, 104(tp)

#         # Save function arguments / return values
#         ld a0, 112(tp)
#         ld a1, 120(tp)
#         ld a2, 128(tp)
#         ld a3, 136(tp)
#         ld a4, 144(tp)
#         ld a5, 152(tp)
#         ld a6, 160(tp)
#         ld a7, 168(tp)

#         # Save more saved registers
#         ld s2, 176(tp)
#         ld s3, 184(tp)
#         ld s4, 192(tp)
#         ld s5, 200(tp)
#         ld s6, 208(tp)
#         ld s7, 216(tp)
#         ld s8, 224(tp)
#         ld s9, 232(tp)
#         ld s10, 240(tp)
#         ld s11, 248(tp)

#         # Save additional temporary registers
#         ld t3, 256(tp)
#         ld t4, 264(tp)
#         ld t5, 272(tp)
#         ld t6, 280(tp)

#         # Return to whatever we were doing in the kernel
#         sret
strap:
        # make room to save registers.
        addi sp, sp, -256

        # save the registers.
        sd ra, 0(sp)
        sd sp, 8(sp)
        sd gp, 16(sp)
        sd tp, 24(sp)
        sd t0, 32(sp)
        sd t1, 40(sp)
        sd t2, 48(sp)
        sd s0, 56(sp)
        sd s1, 64(sp)
        sd a0, 72(sp)
        sd a1, 80(sp)
        sd a2, 88(sp)
        sd a3, 96(sp)
        sd a4, 104(sp)
        sd a5, 112(sp)
        sd a6, 120(sp)
        sd a7, 128(sp)
        sd s2, 136(sp)
        sd s3, 144(sp)
        sd s4, 152(sp)
        sd s5, 160(sp)
        sd s6, 168(sp)
        sd s7, 176(sp)
        sd s8, 184(sp)
        sd s9, 192(sp)
        sd s10, 200(sp)
        sd s11, 208(sp)
        sd t3, 216(sp)
        sd t4, 224(sp)
        sd t5, 232(sp)
        sd t6, 240(sp)

        # call the C trap handler in trap.c
        call trap_handler

        # restore registers.
        ld ra, 0(sp)
        ld sp, 8(sp)
        ld gp, 16(sp)
        # not tp (contains hartid), in case we moved CPUs
        ld t0, 32(sp)
        ld t1, 40(sp)
        ld t2, 48(sp)
        ld s0, 56(sp)
        ld s1, 64(sp)
        ld a0, 72(sp)
        ld a1, 80(sp)
        ld a2, 88(sp)
        ld a3, 96(sp)
        ld a4, 104(sp)
        ld a5, 112(sp)
        ld a6, 120(sp)
        ld a7, 128(sp)
        ld s2, 136(sp)
        ld s3, 144(sp)
        ld s4, 152(sp)
        ld s5, 160(sp)
        ld s6, 168(sp)
        ld s7, 176(sp)
        ld s8, 184(sp)
        ld s9, 192(sp)
        ld s10, 200(sp)
        ld s11, 208(sp)
        ld t3, 216(sp)
        ld t4, 224(sp)
        ld t5, 232(sp)
        ld t6, 240(sp)

        # csrr t0, sepc
        # addi t0, t0, 4
        # csrw sepc, t0

        addi sp, sp, 256
        # call swtch2user
        # return to whatever we were doing in the kernel.
        sret
