; Assemble with Microsoft MASM 5.0 as follows:
;
; C:>masm /b63 /v /w2 /z HDREC286.ASM
;
;
	.286                            ; Generate 80286 code for PC ATs
	DOSSEG                          ;
	.MODEL SMALL                    ; Default for Microsoft C
					; 64K Code and 64K Data/Stack

	extrn   _load_value:word        ; Sample Timer DSP
	extrn   _status:word            ; Programm im DSP? dann=1
	extrn   _no_of_blocks:word      ; Anzahl der 49152 byte data Bl”cke
	extrn   _file_handle:word       ; File-Gr”áe
	extrn   _no_of_errors:word      ; Anzahl der Fehler

	.DATA

no_samples      EQU     12288           ; 12288 samples/block
no_bytes        EQU     4 * no_samples  ; 12288 x 16-bit stereo words
					; = 49152 bytes
	ALIGN 2
PC_buff_A       db 49152 DUP(0)         ; = 49152 bytes memory for PC_buff_A

A_rxbuff_32c    dd 00080000H            ; Position of rx buffer A in DSP32C
B_rxbuff_32c    dd 0008C000H            ; Position of rx buffer B in DSP32C
block_counter   dw 0
new             dw 0                    ; for test_disk_check
old             dw 0                    ;

.CODE

	PUBLIC _record_to_disk
	PUBLIC _playback_from_disk
	PUBLIC _junk
	PUBLIC _test_disk_check

;****************************************************************************

_record_to_disk proc near               ; procedure

	push    bp                      ; Entry sequence - save old BP
	mov     bp, sp                  ; Set stack framepointer
	pusha

	;mov     ax, _load_value         ; AX = Counter Load Value
	;push    ax
	;push    WORD PTR 0000H          ; HIGH address word
	;push    WORD PTR 0050H          ; LOW  address word
	;call    _PutInt16               ; put counter load value into 0x050
	;add     sp, 6                   ; normalise stack

	;mov     ax, _status             ; AX = status (record = 0, play = 1)
	;push    ax                      ; 0000H = Record-to-Disk
	;push    WORD PTR 0000H          ; HIGH address word
	;push    WORD PTR 0052H          ; LOW  address word
	;call    _PutInt16               ; 0x000052
	;add     sp, 6                   ; normalise stack

	;mov     ax, _no_of_blocks       ; AX = number of 49152 byte blocks
	;mov     block_counter, ax
	;push    ax                      ;
	;push    WORD PTR 0000H          ; HIGH address word
	;push    WORD PTR 0054H          ; LOW  address word
	;call    _PutInt16               ; 0x000054
	;add     sp, 6                   ; normalise stack

	;push    WORD PTR no_samples     ; no. of samples for 32C rx_buffer A
	;push    WORD PTR 0000H          ; HIGH address word
	;push    WORD PTR 0056H          ; LOW  address word
	;call    _PutInt16               ; 0x000056
	;add     sp, 6                   ; normalise stack

	;push    WORD PTR no_samples     ; no. of samples for 32C rx_buffer B
	;push    WORD PTR 0000H          ; HIGH address word
	;push    WORD PTR 0058H          ; LOW  address word
	;call    _PutInt16               ; 0x000058
	;add     sp, 6                   ; normalise stack

; End of Initialisation

	;call    _Reset                  ; Start DSP32C running
A_loop:
	;push    WORD PTR 0000H          ; HIGH address word
	;push    WORD PTR 0056H          ; LOW  address word
	;call    _GetInt16               ; Get STATUS of A_rxbuff_32c
	;add     sp, 4
	;or      ax, ax                  ; Test for ZERO (zero = FULL buffer)
	;jne     A_loop
					; Get stereo samples and Dump to Disk
; Fill pc_buff_a from 32C memory

	push    OFFSET pc_buff_a        ; PC_Buff_A address in DS
	push    WORD PTR (no_bytes SHR 1) ; length of array
	push    WORD PTR a_rxbuff_32c+2 ; HIGH 16-bits
	push    WORD PTR a_rxbuff_32c   ;  LOW 16-bits
	;call    _RdBlkI16               ; read in A rx buffer in 32c
	add     sp, 8                   ; normalise stack
					; Dump pc_buff_a to Hard Disk
	mov     ah, 40H
	mov     bx, WORD PTR _file_handle
	mov     cx, WORD PTR no_bytes
	mov     dx, OFFSET pc_buff_a
	int     21H                     ; Buffer to Hard Disk Done

	push    WORD PTR no_samples     ; Re-initialise A_rxbuff_32C to Empty
	push    WORD PTR 0000H          ; HIGH address word
	push    WORD PTR 0056H          ; LOW  address word
	;call    _PutInt16               ; 0x000056 = status buffer A
	add     sp, 6                   ; normalise stack
	dec     WORD PTR block_counter
	cmp     WORD PTR block_counter, 0000H
	je      finished
	jmp     A_loop                  ; Go around again

finished:
	;call    _Halt                   ; Stop processor running
	popa
	mov     sp, bp
	pop     bp                      ; Exit sequence - restore old BP
	ret                             ; NEAR Return to calling program
_record_to_disk ENDP

;****************************************************************************

_playback_from_disk  proc near

	push    bp                      ; Entry sequence - save old BP
	mov     bp, sp                  ; Set stack framepointer
	pusha
					; DSP32C initialisation now follows
	mov     ax, _load_value         ; AX = Counter Load Value
	push    ax
	push    WORD PTR 0000H          ; HIGH address word
	push    WORD PTR 0050H          ; LOW  address word
	;call    _PutInt16               ; put counter load value into 0x050
	add     sp, 6                   ; normalise stack

	mov     ax, _status             ; AX = STATUS (not currently used)
	push    ax                      ; 0000H = Record-to-Disk
	push    WORD PTR 0000H          ; HIGH address word
	push    WORD PTR 0052H          ; LOW  address word
	;call    _PutInt16               ; 0x000052
	add     sp, 6                   ; normalise stack

	mov     ax, _no_of_blocks       ; AX = number of 49152 byte blocks
	mov     block_counter, ax
	push    ax                      ;
	push    WORD PTR 0000H          ; HIGH address word
	push    WORD PTR 0054H          ; LOW  address word
	;call    _PutInt16               ; 0x000054
	add     sp, 6                   ; normalise stack

	push    WORD PTR 0000H          ; STATUS for 32C buffer A = EMPTY
	push    WORD PTR 0000H          ; HIGH address word
	push    WORD PTR 0056H          ; LOW  address word
	;call    _PutInt16               ; 0x000056
	add     sp, 6                   ; normalise stack

	push    WORD PTR 0000H          ; STATUS for 32C buffer B = EMPTY
	push    WORD PTR 0000H          ; HIGH address word
	push    WORD PTR 0058H          ; LOW  address word
	;call    _PutInt16               ; 0x000058
	add     sp, 6                   ; normalise stack

; End of Initialisation

	;call    _Reset                  ; Start DSP32C running

Pmain_loop:
	mov     ah, 3fH                 ; Copy Hard Disk to PC_buff_A
	mov     bx, WORD PTR _file_handle
	mov     cx, WORD PTR no_bytes
	mov     dx, OFFSET pc_buff_a
	int     21H
PA_loop:
	push    WORD PTR 0000H          ; HIGH address word
	push    WORD PTR 0056H          ; LOW  address word
	;call    _GetInt16               ; Get STATUS of 32C buffer A
	add     sp, 4
	or      ax, ax                  ; Test for ZERO (zero = EMPTY buffer)
	jne     PA_loop                 ; Loop if DSP32C buffer is NOT EMPTY

	push    OFFSET pc_buff_a        ; PC_Buff_A address in DS
	push    WORD PTR (no_bytes SHR 1) ; length of array
	push    WORD PTR a_rxbuff_32c+2 ; HIGH 16-bits
	push    WORD PTR a_rxbuff_32c   ;  LOW 16-bits
	;call    _WrBlkI16               ; Write PC buff A to DSP32C buff A
	add     sp, 8                   ; normalise stack

	push    WORD PTR no_samples     ; Set 32C buffer A to FULL
	push    WORD PTR 0000H          ; HIGH address word
	push    WORD PTR 0056H          ; LOW  address word
	;call    _PutInt16               ; 0x000056 = status buffer A
	add     sp, 6                   ; normalise stack

	dec     WORD PTR block_counter  ; test for last data block
	cmp     WORD PTR block_counter, 0000H
	je      pfinished
	jmp     Pmain_loop
pfinished:
	;call    _Halt                   ; Stop processor running
	popa
	mov     sp, bp
	pop     bp                      ; Exit sequence - restore old BP
	ret                             ; NEAR Return to calling program
_playback_from_disk  endp

;****************************************************************************

_junk   proc    near                    ; Fills file full of junk to speed
					; up disk access time when writing.
	push    bp                      ; Entry sequence - save old BP
	mov     bp, sp                  ; Set stack framepointer
	pusha

	mov     ax, 0000H               ; Fill array full of zeros
	mov     bx, 0000H               ; Array pointer
	mov     cx, (no_bytes SHR 1)    ; Loop counter = no. of 16-bit ints
clear_A: mov     PC_buff_A[bx], ax
	add     bx, 2
	loop    SHORT clear_A

	mov     cx, WORD PTR _no_of_blocks
junk_loop:
	push    cx
	mov     ah, 40H                 ; Dump empty array to DOS File
	mov     bx, WORD PTR _file_handle
	mov     cx, WORD PTR no_bytes
	mov     dx, OFFSET pc_buff_a
	int     21H
	pop     cx
	loop    junk_loop               ; file size = no_of_blocks * no_bytes
	popa
	mov     sp, bp
	pop     bp                      ; Exit sequence - restore old BP
	ret                             ; Return NEAR to 'C'
					; Return to calling program
_junk   endp

;****************************************************************************

_test_disk_check  proc  near            ; Tests the file generated by test
					; disk and counts any errors which
					; may occur. The file is tested for
					; sequential 16-bit integers.
	push    bp                      ; Entry sequence - save old BP
	mov     bp, sp                  ; Set stack framepointer
	pusha
	mov     ax, 0FFFFH
	mov     old, ax                 ; first integer is Zero
	mov     cx, _no_of_blocks       ; outer loop

loopout:
	push    cx                      ; save outer counter
	mov     ah, 3FH
	mov     bx, WORD PTR _file_handle
	mov     cx, WORD PTR no_bytes
	mov     dx, OFFSET PC_buff_A
	int     21H                     ; get block of data
	mov     bx, 0
loopin:                                 ; array pointer
	mov     ax, PC_buff_A[bx]
	inc     WORD PTR old            ;
	cmp     old, ax
	je      SHORT data_ok           ; if not equal then inc. ERROR Count
	inc     WORD PTR _no_of_errors
data_ok:
	mov     old, ax                 ; old = new
	add     bx, 2
	cmp     bx, 49150               ; inner loop counter
	jbe     SHORT loopin
	pop     cx
	loop    loopout                 ; test outer loop
	popa
	mov     sp, bp
	pop     bp                      ; Exit sequence - restore old BP
	ret                             ; Return NEAR to 'C'
					; Return to calling program
_test_disk_check  endp

;****************************************************************************

	END                             ; End of CODE SEGMENT and Procedures
;

