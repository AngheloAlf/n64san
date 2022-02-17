.set gp=64     # allow use of 64-bit general purpose registers

.section .text

/*
s64 U64_RightShift(s64 value, s64 shift) {
    return value >> shift;
}
*/

.balign 4
.globl    U64_RightShift
.ent      U64_RightShift
.type     U64_RightShift, @function
U64_RightShift:
    sw          $a0, 0x0($sp)
    sw          $a1, 0x4($sp)
    sw          $a2, 0x8($sp)
    sw          $a3, 0xc($sp)

    ld          $t7, 0x8($sp)
    ld          $t6, 0x0($sp)

    dsrav       $v0, $t6, $t7

    dsll32      $v1, $v0, 0
    dsra32      $v1, $v1, 0
    jr          $ra
     dsra32     $v0, $v0, 0

.size    U64_RightShift, .-U64_RightShift
.end     U64_RightShift
