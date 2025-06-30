CC = gcc
CFLAGS = -I/opt/homebrew/include -Wall -Wextra
LDFLAGS = -L/opt/homebrew/lib -lraylib -framework OpenGL -framework Cocoa -framework IOKit
SRC = codigo_fonte/main.c codigo_fonte/jogador.c codigo_fonte/inimigo.c codigo_fonte/mapa.c codigo_fonte/bomba.c
OUT = bin/jogo

all: $(OUT)

$(OUT): $(SRC)
	$(CC) $(SRC) -o $(OUT) $(CFLAGS) $(LDFLAGS)

clean:
	rm -f $(OUT)


