# Инициализация массива в памяти
.data
array:
.word 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
a:
.word 22
b:
.word 8
c:
.word 7
msg_eq:
.asciz "------------"
for:
.word 36
const:
.word 4
treshhold:
.word 0
res1:
.word 0
res2:
.word 0
.text

main:
# Регистровая адресация для инициализации a, b, c
la t4, array #помещаем указатель на начало массива в t4
la s3, array
lw a1, a #помещаем константы в регистры
lw a2, b
lw a3, c
la a4, 4
lw a5, for
add a1, a1, a2 #a1 = a + b
add a1, a1, a3 #a1= a+b+c
sub a2, a1, a3
sub a2, a2, a3 #a2 = a+ b -c
sw a1, 0(t4) #arr[0] = a + b + c
sub s3, s3, a5
loop:
lw s4, const
sub t4, t4, s4
add a1, a1, a2
sw a1, 0(t4)
beq t4, s3, continue
j loop
continue:
lw a0, 36(t4)
li a7, 1
ecall
la a0, 10
li a7, 11
ecall
lw a0, 32(t4)
li a7, 1
ecall
la a0, 10
li a7, 11
ecall
lw a0, 28(t4)
li a7, 1
ecall
la a0, 10
li a7, 11
ecall
lw a0, 24(t4)
li a7, 1
ecall
la a0, 10
li a7, 11
ecall
lw a0, 20(t4)
li a7, 1
ecall
la a0, 10
li a7, 11
ecall
lw a0, 16(t4)
li a7, 1
ecall
la a0, 10
li a7, 11
ecall
lw a0, 12(t4)
li a7, 1
ecall
la a0, 10
li a7, 11
ecall
lw a0, 8(t4)
li a7, 1
ecall
la a0, 10
li a7, 11
ecall
lw a0, 4(t4)
li a7, 1
ecall
la a0, 10
li a7, 11
ecall
lw a0, 0(t4)
li a7, 1
ecall
la a0, 10
li a7, 11
ecall
lw a1, 32(t4) #a1 = arr[1]
lw s1, 28(t4)
lw a2, 12(t4) #a2 = arr[4]
lw a3, 36(t4) #a3 = arr[0]
lw a5, 16(t4) #a5 = arr[5]
lw a6, 4(t4) #a6 = arr[8]
add a4, a1, a2 #a4 = arr[1] + arr[9]
#a4 = arr[1] + arr[9] + arr[4]
add a4, a4, a3
lw t4, treshhold #lw = treshhold
ble a4, t4, result1 #если больше или равно прыгаем в res1
j result2 #иначе в res2

result1:
la s7, res1 #помешаем адрес res1 в s5
add s4, a3, a5 #s4 = arr[0] + arr[5]
sw s4, 0(s7) # записываем результат в res1
j end

result2:
la a6, res2 #помешаем адрес res2 в t4
lw s2, a # s2 = a
or s2, s1, s2 #s2 = arr[2] | a
sw s2, 0(a6) #записываем результат в res2
j end

end:
la a0, msg_eq
li a7, 4
ecall
la a0, 10
li a7, 11
ecall
#вывод res1
lw a0, res1
li a7, 1
ecall
#вывод конца строки
la a0, 10
li a7, 11
ecall
#вывод res2
lw a0, res2
li a7,1
ecall
#конец программы
li a7, 10
ecall
