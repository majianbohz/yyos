; usage: Descriptor base, limit, attr
;        base: dd
;        limit: dd (low 20 bits available)
;        attr: dw ( [8..11] 4 bits alway 0)

%macro  Descriptor 3
  dw %2 & 0ffffh       ; seg limit 1 (2 bytes)
  dw %1 & 0ffffh       ; seg base 1  (2 bytes)
  db (%1 >> 16) & 0ffh ; seg base 2  (1 byte)
  dw ((%2 >> 8) & 0f00h) | (%3 & 0f0ffh)  ; attr 1 + seg limt 2 + attr2 (2 bytes)
  db (%1 >>24) & 0ffh  ; seg base 3  (1 byte)
%endmacro 

Seg_Code_Attr_4G  equ 0c09ah
Seg_Data_Attr_4G  equ 0c092h
Seg_Stack_Attr_4G equ 0c096h

Seg_Code_Attr_1M  equ 0409ah
Seg_Data_Attr_1M  equ 04092h
Seg_Stack_Attr_1M equ 04096h

