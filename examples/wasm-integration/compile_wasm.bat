mkdir temp

call em++ -o temp\main.o -c -I..\..\include -I..\..\src main.cpp 
call em++ -o temp\qr.o -c -I..\..\include -I..\..\src ..\..\src\qr.cpp 
call em++ -o temp\qr_encoder.o -c -I..\..\include -I..\..\src ..\..\src\qr_encoder.cpp 
call em++ -o temp\qr_render.o -c -I..\..\include -I..\..\src ..\..\src\qr_render.cpp 
call em++ -o temp\qr_segment.o -c -I..\..\include -I..\..\src ..\..\src\qr_segment.cpp 
call em++ -o temp\qr_gf256.o -c -I..\..\include -I..\..\src ..\..\src\qr_gf256.cpp 
call em++ -o temp\qr_data.o -c -I..\..\include -I..\..\src ..\..\src\qr_data.cpp 
call em++ -o temp\qr_rank.o -c -I..\..\include -I..\..\src ..\..\src\qr_rank.cpp 

call em++ --bind -o qr.js temp\main.o temp\qr.o temp\qr_data.o temp\qr_encoder.o temp\qr_gf256.o temp\qr_rank.o temp\qr_render.o temp\qr_segment.o -Ltemp -sSTANDALONE_WASM --no-entry -sEXPORTED_FUNCTIONS=_createQr -sNO_FILESYSTEM=1 

