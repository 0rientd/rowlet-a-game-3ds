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
	u64 idDosTitulosAchados[500] = {};
	AM_GetTitleList(&quantidadeDeTitulos, MEDIATYPE_SD, 500, idDosTitulosAchados);
	

pegarJogoAleatorio:
	u64 tituloAleatorio = 0;
	while(tituloAleatorio == 0){
		u32 tituloAleatorioEscolhido = rand() % quantidadeDeTitulos;
		u32 conteudoEscolhido = ((unsigned char*)(&idDosTitulosAchados[tituloAleatorioEscolhido]))[4];

		if (conteudoEscolhido == 0x00 || conteudoEscolhido == 0x02) {
			tituloAleatorio = idDosTitulosAchados[tituloAleatorioEscolhido];
		}
	}

	printf("\x1b[2;2HMade by \x1b[35;40m0rientd\x1b[0m");

	printf("\x1b[7;15H+--------------------+");
	printf("\x1b[8;15H|\x1b[8;36H|");
	printf("\x1b[9;22HTitle ID");
	printf("\x1b[9;15H|\x1b[9;36H|");
	printf("\x1b[10;15H|\x1b[10;36H|");
	printf("\x1b[11;15H|\x1b[11;36H|");
	printf("\x1b[12;15H|\x1b[12;36H|");
	printf("\x1b[13;15H+--------------------+");

	printf("\x1b[19;3H+------------------+");
	printf("\x1b[20;3H|      Press \x1b[31;40mY\x1b[0m     |");
	printf("\x1b[21;3H|     to random    |");
	printf("\x1b[22;3H+------------------+");

	printf("\x1b[19;29H+------------------+");
	printf("\x1b[20;29H|      Press \x1b[31;40mA\x1b[0m     |");
	printf("\x1b[21;29H|      to load     |");
	printf("\x1b[22;29H+------------------+");

	printf("\x1b[29;16HPress \x1b[31;40mSTART\x1b[0m to exit");

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

		printf("\x1b[2;42H%02lu:%02lu:%02lu", hours, minutes, seconds);

		printf("\x1b[32;40m\x1b[11;18H%016llx\x1b[0m", tituloAleatorio);

		gfxFlushBuffers();
		gfxSwapBuffers();
		
		gspWaitForVBlank();
	}
	
	amExit();
	gfxExit();
	return 0;
}
