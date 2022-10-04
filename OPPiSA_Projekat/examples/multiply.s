.globl main

.data
m1:                   .word 6
m2:                   .word 5
m3:                   .word 0

.text
main:
          la          $t0, m1
          lw          $t2, 0($t0)
          la          $t0, m2
          lw          $t3, 0($t0)
         div          $t0, 1
         div          $t1, 0
lab:
         add          $t1, $t1, $t2
         sub          $t0, $t0, $t3
        addi          $t0, $t0, 1
        bltz          $t0, lab
          la          $t0, m3
          sw          $t1, 0($t0)
         nop
