.globl main

.data
m1:                   .word 6
m2:                   .word 5

.text
main:
          la          $t0, m1
          lw          $t1, 0($t0)
          la          $t0, m2
test:
          lw          $t0, 0($t0)
test:
         add          $t0, $t1, $t0
