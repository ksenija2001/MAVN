.globl main

.data
m1:                   .word 6
m2:                   .word 5

.text
main:
          la          $t0, m1
          lw          $t2, 0($t0)
          la          $t1, m2
lab:
          lw          $t0, 0($t1)
          sw          $t0, 0($t1)
         add          $t0, $t2, $t0
           b         lab
