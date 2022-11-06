/*
file tree:

frea.exe
config.txt
# world/
	worlds.txt - list of all worlds
	<worldname>.data
	... eg
# players/
	players.txt - list of all players
	<playeruid>.data
	... eg
# plugins/
	plugins.txt - list of all plugins
	<pluginname>.<lua/js/py>

*/
// if files dont exist return false, if they do exist return true
bool *files_check() {
	bool *exists = (bool *) malloc(sizeof(bool)*4); // four files to check
	FILE *file;
	file = fopen("config.txt","r");
	if (file==NULL) {
		exists[0] = BOOL_FALSE;
	}
	file = fopen("world/worlds.txt","r");
	if (file==NULL) {
		exists[1] = BOOL_FALSE;
	}
	file = fopen("players/players.txt","r");
	if (file==NULL) {
		exists[2] = BOOL_FALSE;
	}
	file = fopen("plugins/plugins.txt","r");
	if (file==NULL) {
		exists[3] = BOOL_FALSE;
	}
	return exists;
}