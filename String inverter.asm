data segment
    
ends

stack segment 
    
    dw   128  dup(0)
ends

code segment   
    
start:
; dice al programma dove trovare lo stak segment e il data segment
    mov ax, data
    mov ds, ax
    mov es, ax

codice:
    ;prende una stringa di 20 caratteri come richiesto, un carattere per volta
    mov cx,20
    prendi_stringa: ;prendo 20 volte un carattere
    mov ah, 01h
    int 21h
    xor ah,ah    ;azzero ah
    push ax      ;cosi' resta solamente la parte bassa di ax che contine il carattere aquisito
    loop prendi_stringa  
    
    ;vado a capo        
    mov ah,02h
    mov dl,13
    int 21h        
    mov ah,02h
    mov dl,10
    int 21h         
    
    ;tiro fuori carattere per carattere dallo stack                                                                      
    mov cx,20
    gira_stringa:
    pop dx
    mov ah,02h ;stampa il carattere tirato fuori dallo stack
    int 21h
    loop gira_stringa
   
    
    mov ax, 4c00h ; esce dal programma.
    int 21h    
ends

end start ; imposta il punto di ingresso e arresta l'assembler
