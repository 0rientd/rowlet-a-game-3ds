#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <3ds.h>
#include <time.h>

int main() {
	gfxInitDefault();
	consoleInit(GFX_TOP, NULL);
	amInit();
	amAppInit();

	u32 quantidadeDeTitulos, hours, minutes, seconds;
	u64 idDosTitulosAchados[900] = {};
	AM_GetTitleList(&quantidadeDeTitulos, MEDIATYPE_SD, 900, idDosTitulosAchados);

	printf("\x1b[1;1HCriado por 0rientd");
	printf("\x1b[6;1HTitulos encontrados: %lu\n", quantidadeDeTitulos);
	printf("\x1b[200;28HAperte START para sair");

pegarJogoAleatorio:
	u64 tituloAleatorio = 0;
	while(tituloAleatorio == 0){
		u32 tituloAleatorioEscolhido = rand() % quantidadeDeTitulos;
		u32 conteudoEscolhido = ((unsigned char*)(&idDosTitulosAchados[tituloAleatorioEscolhido]))[4];

		if (conteudoEscolhido == 0x00 || conteudoEscolhido == 0x02) {
			tituloAleatorio = idDosTitulosAchados[tituloAleatorioEscolhido];
		}
	}

	printf("\x1b[7;1HAperte Y para tentar um novo jogo.\n\n");

	while (aptMainLoop()) {
		hidScanInput();
		u32 kDown = hidKeysDown();

		if (kDown & KEY_START) {
			break;

		} else if (kDown & KEY_A) {
			aptSetChainloader(tituloAleatorio, MEDIATYPE_SD);
			break;

		} else if (kDown & KEY_Y) {
			goto pegarJogoAleatorio;

		}

		time_t unixTime = time(NULL);
		struct tm* timeStruct = gmtime((const time_t *)&unixTime);

		hours = timeStruct->tm_hour;
		minutes = timeStruct->tm_min;
		seconds = timeStruct->tm_sec;

		printf("\x1b[1;43H%02lu:%02lu:%02lu", hours, minutes, seconds);

		printf("\x1b[10;1HAperte A para carregar esse titulo:\n%016llx\n\n", tituloAleatorio);

		gfxFlushBuffers();
		gfxSwapBuffers();
		
		gspWaitForVBlank();
	}
	
	amExit();
	gfxExit();
	return 0;
}
