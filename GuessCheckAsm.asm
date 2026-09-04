    .cdecls C,LIST,"stdint.h"

    .global GuessCheckAsm
    .ref winning
    .ref guess
    .ref res

    .text
    .retain
    .retainrefs

GuessCheckAsm:
    PUSH.W  R4           
    PUSH.W  R5           
    PUSH.W  R6
    PUSH.W  R7
    PUSH.W  R8
    PUSH.W  R9
    PUSH.W  R10
    PUSH.W  R11

    ;rezervisem 8 bajtova za tmp[4]
    SUB.W   #8, SP                                  ;alocira 8 bajtova na SP za tmp
    ;OBAVEZNO!!!!  ADD.W    #8, SP

    MOV.W   SP, R4                                  ; R4 drzi adresu od tmp
    MOV.W   #winning, R5                            ; R5 drzi adresu od winning
    MOV.W   #guess, R6                              ; R6 drzi adresu od guess
    MOV.W   #res, R7                                ; R7 drzi adresu od res

    CLR.W   R8

;-------------------------------------------------------
;PETLJA KOJA KOPIRA winning U tmp
;-------------------------------------------------------

copy_loop:
    CMP.B   #4, R8
    JEQ     copy_done

    MOV.B   @R5, R12
    MOV.B   R12, 0(R4)
    MOV.B   #0, 0(R7)
    
    INC.W   R4
    INC.W   R5
    INC.W   R7
    INC.W   R8

    JMP     copy_loop
    

copy_done:
    SUB.W   #4, R4
    SUB.W   #4, R7
    MOV.W   R4, R5
    CLR.W   R8

;-------------------------------------------------------
;PETLJA KOJA PROVERAVA TACAN ZNAK I TACNO MESTO
;-------------------------------------------------------

exact_loop:
    CMP.B   #4, R8
    JEQ     general_reset

    MOV.B   @R4, R12
    MOV.B   @R6+, R13

    CMP.B R12, R13
    JNE no_exact_match

    MOV.B #2, 0(R7)
    MOV.B #7, 0(R4)


no_exact_match:
    INC.W   R4
    INC.W   R7
    INC.W   R8
    JMP exact_loop;

;-------------------------------------------------------
;PETLJA KOJA PROVERAVA TACAN ZNAK I POGRESNO MESTO
;-------------------------------------------------------

general_reset:
    CLR.W   R8
    CLR.W   R11
    SUB.W   #4, R4
    MOV.W   R4, R5
    SUB.W   #4, R6
    SUB.W   #4, R7
    MOV.W   @R4, R12
    MOV.W   @R6, R13
    

outer_loop:
    CMP.B   #4, R8
    JEQ     done

    CMP.B   #2, 0(R7)
    JEQ     inc_i

    MOV.B   #0, R15


inner_loop:
    CMP.B   #4, R11
    JEQ     is_match_found

    CMP.B   R12, R13
    JNE     inc_j

    MOV.W   #1, R15


is_match_found:
    CMP.B   #0, R15
    JEQ     write_b

    MOV.W   R4, R5
    ADD.W   R11, R5
    MOV.B   #7, 0(R5)


write_b:
    MOV.B   R15, 0(R7)    
inc_i:
    INC.W   R6
    INC.W   R7
    INC.B   R8
    MOV.W   R4, R5
    MOV.B   @R4, R12
    MOV.B   @R6, R13
    CLR.W   R11
    JMP     outer_loop


inc_j:
    INC.B   R11
    INC.W   R5
    MOV.B   @R5, R12
    JMP     inner_loop


;-------------------------------------------------------
;VRACANJE SP I SADRZAJA IZ REGISTARA PA POVRATAK U C KOD
;-------------------------------------------------------

done:    
    ADD.W   #8, SP

    POP.W   R11
    POP.W   R10
    POP.W   R9
    POP.W   R8
    POP.W   R7
    POP.W   R6
    POP.W   R5
    POP.W   R4

    RETA
