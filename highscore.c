
#include "g_local.h"

void LoadHighScores(void)
{
	FILE     *HS_file;

	if (HS_file = fopen("./highscores","rb"))
	{
		fread(&game.highscore, sizeof(HS_STRUCT) * 2, 1, HS_file);
		fclose(HS_file);
	}
}

void SaveHighScores(void)
{
	FILE     *HS_file;

	if (HS_file = fopen("./highscores","wb"))
	{
		fwrite(&game.highscore, sizeof(HS_STRUCT) * 2, 1, HS_file);
		fclose(HS_file);
	}
}
