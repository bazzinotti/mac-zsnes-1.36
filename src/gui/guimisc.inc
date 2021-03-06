;Copyright (C) 1997-2001 ZSNES Team ( zsknight@zsnes.com / _demo_@zsnes.com )
;
;This program is free software; you can redistribute it and/or
;modify it under the terms of the GNU General Public License
;as published by the Free Software Foundation; either
;version 2 of the License, or (at your option) any later
;version.
;
;This program is distributed in the hope that it will be useful,
;but WITHOUT ANY WARRANTY; without even the implied warranty of
;MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
;GNU General Public License for more details.
;
;You should have received a copy of the GNU General Public License
;along with this program; if not, write to the Free Software
;Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.



; Movie, Joystick setting (display) routines, SNES Reset Function

MovieRecord:
    mov byte[GUICBHold],0
    mov dword[MovieCounter],0
    cmp byte[MovieProcessing],0
    jne near .dontrecord
    ; check if movie exists already
    mov ebx,[statefileloc]
    mov eax,[fnamest+ebx-3]
    push eax
    mov dword[fnamest+ebx-3],'.zmv'
    mov al,[CMovieExt]
    mov byte[fnamest+ebx],al

    ; check if file exists
    cmp byte[MovieRecordWinVal],1
    je .nocheck
    mov edx,fnamest+1
    call Open_File
    jc .nocheck
    mov bx,ax
    call Close_File
    mov byte[MovieRecordWinVal],1
    jmp .skipstate
.nocheck
    mov dword[CFWriteHead],0
    mov dword[CReadHead],0
    mov dword[ReadHead],0
    mov dword[CFWriteStart],64
    mov byte[MovieRecordWinVal],0
    call ChangetoSRAMdir
    mov byte[NoPictureSave],1
    call statesaver
    mov byte[NoPictureSave],0
    mov edx,fnamest+1
    call Open_File_Write
    mov bx,ax
    mov [MovieFileHand],bx
    mov cx,0
    mov dx,0
    call File_Seek_End
    mov al,[soundon]
    mov [RecData],al
    mov eax,[welcome+8]
    mov [RecData+1],eax
    mov byte[RecData+2],1
    mov eax,[timer2upd]
    mov [RecData+3],eax
    mov eax,[curexecstate]
    mov [RecData+7],eax
    mov edx,RecData
    mov ecx,16
    call Write_File
    cmp dword[ramsize],0
    je .noram
    mov edx,[sram]
    mov ecx,[ramsize]
    call Write_File
.noram
    mov byte[MovieProcessing],2
    mov dword[MovieBuffSize],0
    mov dword[MovieBuffFrame],0
    mov byte[RepeatFrame],0
    cmp byte[CNetType],20
    je near .netstuff
    cmp byte[CNetType],21
    je near .netstuff
    mov dword[nmiprevaddrl],0
    mov dword[nmiprevaddrh],0
    mov dword[nmirept],0
    mov dword[nmiprevline],224
    mov dword[nmistatus],0
    mov byte[spcnumread],0
    mov byte[NextLineCache],0
    mov dword[PJoyAOrig],0
    mov dword[PJoyBOrig],0
    mov dword[PJoyCOrig],0
    mov dword[PJoyDOrig],0
    mov dword[PJoyEOrig],0
    mov byte[DSPMem+08h],0
    mov byte[DSPMem+18h],0
    mov byte[DSPMem+28h],0
    mov byte[DSPMem+38h],0
    mov byte[DSPMem+48h],0
    mov byte[DSPMem+58h],0
    mov byte[DSPMem+68h],0
    mov byte[DSPMem+78h],0
.netstuff
    call ChangetoLOADdir
.skipstate
    pop eax
    mov ebx,[statefileloc]
    mov [fnamest+ebx-3],eax
.dontrecord
    ret

MovieRecordWinVal db 0
NEWSYM MovieProcessing, db 0
NEWSYM MovieFileHand, dw 0
NEWSYM RepeatFrame, db 0
NEWSYM RecData, times 16 db 0
NEWSYM NoPictureSave, db 0

SkipMovie:
    mov byte[MovieRecordWinVal],0
    mov byte[GUICBHold],0
    ret

MovieStop:
    mov byte[GUICBHold],0
    cmp byte[MovieProcessing],0
    je .skipfileend
    mov bx,[MovieFileHand]
    mov byte[MovieProcessing],0
    call Close_File
.skipfileend
    ret

CalibrateDispA:
    xor ebx,ebx
    mov ecx,256
.a
    mov byte[pressed+ebx],0
    inc ebx
    dec ecx
    jnz .a
    call GUIUnBuffer
    call DisplayBoxes
    call DisplayMenu
    GUIBox 75,103,192,135,160
    GUIBox 75,103,192,103,162
    GUIBox 75,103,75,135,161
    GUIBox 192,103,192,135,159
    GUIBox 75,135,192,135,158
    GUIOuttext 81,109,guicalberror1,220-15
    GUIOuttext 80,108,guicalberror1,220
    GUIOuttext 81,117,guicalberror2,220-15
    GUIOuttext 80,116,guicalberror2,220
    GUIOuttext 81,125,guicalberror3,220-15
    GUIOuttext 80,124,guicalberror3,220
    call vidpastecopyscr
    call WaitForKey
    ret

CalibrateDispB:
    xor ebx,ebx
    mov ecx,256
.a
    mov byte[pressed+ebx],0
    inc ebx
    dec ecx
    jnz .a
    call GUIUnBuffer
    call DisplayBoxes
    call DisplayMenu
    GUIBox 75,103,192,143,160
    GUIBox 75,103,192,103,162
    GUIBox 75,103,75,143,161
    GUIBox 192,103,192,143,159
    GUIBox 75,143,192,143,158
    GUIOuttext 81,109,guicalberror4,220-15
    GUIOuttext 80,108,guicalberror4,220
    GUIOuttext 81,117,guicalberror5,220-15
    GUIOuttext 80,116,guicalberror5,220
    GUIOuttext 81,125,guicalberror6,220-15
    GUIOuttext 80,124,guicalberror6,220
    GUIOuttext 81,133,guicalberror7,220-15
    GUIOuttext 80,132,guicalberror7,220
    call vidpastecopyscr
    call WaitForKey
    ret

guicalberror1 db 'PRESS THE TOP LEFT',0
guicalberror2 db 'CORNER AND PRESS A',0
guicalberror3 db 'BUTTON OR KEY',0
guicalberror4 db 'PRESS THE BOTTOM',0
guicalberror5 db 'RIGHT CORNER AND',0
guicalberror6 db 'PRESS A BUTTON OR',0
guicalberror7 db 'KEY',0


SetAllKeys:
    xor ebx,ebx
    mov ecx,256+128+64+16
.a
    mov byte[pressed+ebx],0
    inc ebx
    dec ecx
    jnz .a

    mov byte[GUICBHold],0
    mov edi,pl1selk
    cmp byte[cplayernum],1
    jne .nopl2
    mov edi,pl2selk
.nopl2
    cmp byte[cplayernum],2
    jne .nopl3
    mov edi,pl3selk
.nopl3
    cmp byte[cplayernum],3
    jne .nopl4
    mov edi,pl4selk
.nopl4
    cmp byte[cplayernum],4
    jne .nopl5
    mov edi,pl5selk
.nopl5
    mov dword[guicpressptr],edi
    xor ebx,ebx
    mov ecx,12
    mov dword[guipressptr],guipresstext4b
.loop
    mov eax,[.ordercalb+ebx*4]
    shl eax,2
    add dword[guicpressptr],eax
    push ebx
    push ecx
    call guipresstestb
    pop ecx
    pop ebx
    add dword[guipressptr],21
    mov eax,[.ordercalb+ebx*4]
    shl eax,2
    sub dword[guicpressptr],eax
    inc ebx
    dec ecx
    jnz .loop
    ret
.ordercalb dd 2,3,4,5,1,0,7,10,6,9,8,11

CalibrateDev1:
    xor eax,eax
    mov al,[cplayernum]
    mov ebx,[GUIInputRefP+eax*4]
    mov bl,[ebx]
    mov byte[GUICBHold],0
%ifdef __MSDOS__
    cmp byte[pl1p209+eax],0
    jne near .port209
%endif
    cmp bl,1
    jbe .nojoy
    cmp bl,18
    je near .6button
    cmp bl,5
    je near .6button
    cmp bl,6
    jae .nojoy
    mov dx,201h
    call GetCoords
    mov eax,[JoyX]
    mov [.joybcx],eax
    mov eax,[JoyY]
    mov [.joybcy],eax
    call CalibrateDispA
    mov dx,201h
    call GetCoords
    mov eax,[JoyX]
    mov [.joyblx],eax
    mov eax,[JoyY]
    mov [.joybly],eax
    call CalibrateDispB
    mov dx,201h
    call GetCoords
    jmp .procdir
.nojoy
    ret
.6button
    mov dx,201h
    call GetCoords3
    mov eax,[JoyX]
    mov [.joybcx],eax
    mov eax,[JoyY]
    mov [.joybcy],eax
    call CalibrateDispA
    mov dx,201h
    call GetCoords3
    mov eax,[JoyX]
    mov [.joyblx],eax
    mov eax,[JoyY]
    mov [.joybly],eax
    call CalibrateDispB
    mov dx,201h
    call GetCoords3
.procdir
    mov eax,[.joybcx]
    add eax,[.joyblx]
    shr eax,1
    mov [JoyMinX],eax
    mov [CalibXmin],eax
    mov eax,[.joybcy]
    add eax,[.joybly]
    shr eax,1
    mov [JoyMinY],eax
    mov [CalibYmin],eax
    mov eax,[.joybcx]
    add eax,[JoyX]
    shr eax,1
    mov [JoyMaxX],eax
    mov [CalibXmax],eax
    mov eax,[.joybcy]
    add eax,[JoyY]
    shr eax,1
    mov [JoyMaxY],eax
    mov [CalibYmax],eax
    ret
.port209
    cmp bl,1
    jbe .nojoy209
    cmp bl,18
    je near .6button209
    cmp bl,5
    je near .6button209
    cmp bl,6
    jae .nojoy209
    mov dx,209h
    call GetCoords
    mov eax,[JoyX]
    mov [.joybcx],eax
    mov eax,[JoyY]
    mov [.joybcy],eax
    call CalibrateDispA
    mov dx,209h
    call GetCoords
    mov eax,[JoyX]
    mov [.joyblx],eax
    mov eax,[JoyY]
    mov [.joybly],eax
    call CalibrateDispB
    mov dx,209h
    call GetCoords
    jmp .procdir209
.nojoy209
    ret
.6button209
    mov dx,209h
    call GetCoords3
    mov eax,[JoyX]
    mov [.joybcx],eax
    mov eax,[JoyY]
    mov [.joybcy],eax
    call CalibrateDispA
    mov dx,209h
    call GetCoords3
    mov eax,[JoyX]
    mov [.joyblx],eax
    mov eax,[JoyY]
    mov [.joybly],eax
    call CalibrateDispB
    mov dx,209h
    call GetCoords3
.procdir209
    mov eax,[.joybcx]
    add eax,[.joyblx]
    shr eax,1
    mov [JoyMinX209],eax
    mov [CalibXmin209],eax
    mov eax,[.joybcy]
    add eax,[.joybly]
    shr eax,1
    mov [JoyMinY209],eax
    mov [CalibYmin209],eax
    mov eax,[.joybcx]
    add eax,[JoyX]
    shr eax,1
    mov [JoyMaxX209],eax
    mov [CalibXmax209],eax
    mov eax,[.joybcy]
    add eax,[JoyY]
    shr eax,1
    mov [JoyMaxY209],eax
    mov [CalibYmax209],eax
    ret
.joyblx dd 0
.joybly dd 0
.joybcx dd 0
.joybcy dd 0


SetDevice:
    mov byte[GUICBHold],0
    xor eax,eax
    mov al,[cplayernum]
;    cmp byte[OSPort],1
;    ja .notdos
%ifdef __MSDOS__
    cmp byte[pl1p209+eax],0
    jne .port209
%endif
;.notdos
    mov dword[CalibXmin],0
    mov ecx,[DevicePtr+eax*4]
    mov eax,[GUIInputRefP+eax*4]
    mov ebx,[GUIcurrentinputcursloc]
    mov bl,[GUIInputRefF+ebx]
    mov [eax],bl
    mov eax,ecx
    mov bh,[cplayernum]
    call SetInputDevice
    call UpdateDevices
    jmp .procmtap
.port209
    mov dword[CalibXmin209],0
    mov ecx,[DevicePtr+eax*4]
    mov eax,[GUIInputRefP+eax*4]
    mov ebx,[GUIcurrentinputcursloc]
    mov bl,[GUIInputRefF+ebx]
    mov [eax],bl
    mov eax,ecx
    mov bh,[cplayernum]
    call SetInputDevice209
    call UpdateDevices
.procmtap
    mov byte[MultiTap],1
    cmp byte[SFXEnable],1
    je .dismtap
    cmp byte[pl3contrl],0
    jne .mtap
    cmp byte[pl4contrl],0
    jne .mtap
    cmp byte[pl5contrl],0
    jne .mtap
.dismtap
    mov byte[MultiTap],0
.mtap
    ret

NEWSYM GUIDoReset
    call ClearScreen
    call Clear2xSaIBuffer
;    mov byte[NetPlayNoMore],1
    mov byte[RestoreValues],0

    cmp byte[MovieProcessing],0
    je .skipfileend
    mov bx,[MovieFileHand]
    mov byte[MovieProcessing],0
    call Close_File
.skipfileend
    call RestoreSystemVars
    ; clear spc700 ram
    mov al,0FFh
    mov edi,spcRam
    mov ecx,65472
    rep stosb
    ; clear dsp ram
    xor al,al
    mov edi,DSPMem
    mov ecx,256
    rep stosb

    ; reset the snes
    call init65816
    call procexecloop

    mov eax,spcRam
    mov edx,0EFh
.loop
    mov byte[eax],0
    inc eax
    dec edx
    jnz .loop

    xor eax,eax
    mov edx,spcRam
    mov ax,0FFC9h
    add edx,eax
    mov [spcPCRam],edx
    mov dword [spcS],1EFh
    mov dword [spcRamDP],spcRam
    mov byte[spcA],0
    mov byte[spcX],0
    mov byte[spcY],0
    mov byte[spcP],0
    mov byte[spcNZ],0
    mov byte[Voice0Status],0
    mov byte[Voice1Status],0
    mov byte[Voice2Status],0
    mov byte[Voice3Status],0
    mov byte[Voice4Status],0
    mov byte[Voice5Status],0
    mov byte[Voice6Status],0
    mov byte[Voice7Status],0
    mov byte[GUIQuit],2
    ret
