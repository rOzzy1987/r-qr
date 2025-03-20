mkdir temp
g++ -o temp\main.o -c -I..\..\include -I..\..\src main.cpp
g++ -o temp\qr.o -c -I..\..\include -I..\..\src ..\..\src\qr.cpp
g++ -o temp\qr_encoder.o -c -I..\..\include -I..\..\src ..\..\src\qr_encoder.cpp
g++ -o temp\qr_render.o -c -I..\..\include -I..\..\src ..\..\src\qr_render.cpp
g++ -o temp\qr_segment.o -c -I..\..\include -I..\..\src ..\..\src\qr_segment.cpp
g++ -o temp\qr_gf256.o -c -I..\..\include -I..\..\src ..\..\src\qr_gf256.cpp
g++ -o temp\qr_data.o -c -I..\..\include -I..\..\src ..\..\src\qr_data.cpp
g++ -o temp\qr_rank.o -c -I..\..\include -I..\..\src ..\..\src\qr_rank.cpp
g++ -o rqr.exe temp\main.o temp\qr.o temp\qr_data.o temp\qr_encoder.o temp\qr_gf256.o temp\qr_rank.o temp\qr_render.o temp\qr_segment.o -Ltemp
