; Минец Максим БПИ193
; Вариант 15
; Формулировка задания:
; Разработать программу, которая вводит одномерный массив A[N], формирует из элементов массива A новый массив B по правилам, 
; указанным в таблице, и выводит его. Память под массивы может выделяться как статически, так и динамически по выбору разработчика.

; Разбить решение задачи на функции следующим образом:
; 1) Ввод и вывод массивов оформить как подпрограммы.
; 2) Выполнение задания по варианту оформить как процедуру
; 3) Организовать вывод как исходного, так и сформированного массивов
; Указанные процедуры могут использовать данные напрямую (имитация процедур без параметров). Имитация работы с параметрами также допустима.

; Формулировка варианта 15:
; Массив B с заменой всех нулевых элементов значением минимального.


format PE console

entry _start
include 'win32a.inc'

section '.data' data readable writable;

        formatStr db '%s', 0
        formatInt db '%d', 0
        arrayFormatInt db '%d ', 0
        formatIntWithSpace db '%d ', 0
        typeSize db 'Type size (between 0 and 100) of an array: ', 10, 0
        typeArr db 'Type your array: ', 10, 0
        typeMin db 'Min number is %d', 10, 0
        wrongInput db 'Incorrect input. Please try again', 10, 0
        oldArray db 'Array A:', 10, 0
        newArray db 'Array B:' , 10, 0
        newLine db 10, 0

        N dd ?
        num dd ?
        i dd 0
        arr rd 100
        arrB rd 100
        min dd 2147483647
        tmpStack dd ?
        tmp      dd ?
        NULL = 0

section '.code' code readable executable
        _start:

                call InputArray

                push newLine
                call [printf]
                add esp,  4

                mov [i], 0
                call ChangeNegativeElements


                push [min]
                push typeMin
                call [printf]
                add esp, 8

                push oldArray
                call [printf]
                add esp, 4
                mov ebx, arr
                call OutputArray;
                push newLine
                call [printf]
                add esp, 4
                push newArray
                call [printf]
                add esp, 4
                mov ebx, arrB
                call OutputArray

                invoke  VirtualFree, [arr], [N], MEM_DECOMMIT

                call [getch]

                jmp ExitProgram

        InputArray:
                push typeSize
                call [printf]
                add esp, 4
                push N
                push formatInt
                call [scanf]
                add esp, 8
                cmp [N], 0
                jle WrongInput
                cmp [N], 100
                jge WrongInput
                push typeArr
                call [printf]
                add esp, 4
                mov eax, 0
                mov ebx, arr
                InputLoop:
                        mov [tmp], ebx
                        cmp eax, [N]
                        jge EndOfInput
                        mov [i], eax
                        push ebx
                        push formatInt
                        call [scanf]
                        add esp, 8
                        mov ebx, ecx
                        mov eax, [i]
                        inc eax
                        mov ebx, [tmp]
                        mov ecx, [ebx]
                        cmp ecx, [min]
                        jl UpdateMin
                        ContinueLoop:
                        add ebx, 4
                        jmp InputLoop
                EndOfInput:
                        ret
                WrongInput:
                push wrongInput
                call [printf]
                add esp, 4

                jmp InputArray

        UpdateMin:
                mov [min], ecx
                jmp ContinueLoop


        OutputArray:
                mov [tmpStack], esp
                xor ecx, ecx
                jmp OutputLoop
                OutputLoop:
                     mov [tmp], ebx
                     cmp ecx, [N]
                     je EndOfOutput
                     mov [i], ecx
                     push dword [ebx]
                     push arrayFormatInt
                     call [printf]
                     mov ecx, [i]
                     inc ecx
                     mov ebx, [tmp]
                     add ebx, 4
                     jmp OutputLoop
                EndOfOutput:
                        mov esp, [tmpStack]
                        ret

        ChangeNegativeElements:
                xor ecx, ecx
                mov edx, arr
                mov ebx, arrB
                ChangeLoop:
                cmp ecx, [N]
                je EndChange
                mov eax, [edx]
                cmp eax, 0
                je ChangeEax
                ContinueChangeLoop:
                mov [ebx], eax
                add ebx, 4
                add edx, 4
                inc ecx
                jmp ChangeLoop
                EndChange:
                ret
                ChangeEax:
                mov eax, [min]
                jmp ContinueChangeLoop

        ExitProgram:
                push NULL
                call [ExitProcess]


section '.idata' import data readable
        library kernel, 'kernel32.dll',\
                msvcrt, 'msvcrt.dll'

        import kernel,\
               ExitProcess, 'ExitProcess',\
               VirtualAlloc, 'VirtualAlloc',\
               VirtualFree, 'VirtualFree'


        import msvcrt,\
               printf, 'printf',\
               scanf, 'scanf',\
               getch, '_getch'
