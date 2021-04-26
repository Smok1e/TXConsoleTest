#include <TXLib.h>

//------------------------

int main ()
{ 
	FILE* file = nullptr;
	fopen_s (&file, "dump.log", "a");
	assert (file);

	int size_x = 800;
	int size_y = 800;

	txCreateWindow (size_x, size_y);
	txDisableAutoPause ();
	txSelectFont ("consolas", 25, 12, FW_BOLD);

	_txConsole = -1;

	double total_fps    = 0;
	int    frames_count = 0;
	
	double timeout = 10000;

	LARGE_INTEGER start = {};
	QueryPerformanceCounter (&start);

	LARGE_INTEGER freq = {};
	QueryPerformanceFrequency (&freq);

	double time = 0;

	txBegin ();
	while (!GetAsyncKeyState (VK_ESCAPE) && time < timeout)
	{
		txSetFillColor (TX_BLACK);
		txClear        ();

		double fps = txGetFPS ();

		char str[MAX_PATH] = "";
		int len = sprintf_s (str, "%3.0lf FPS", fps);
		txTextOut (size_x / 2 - len*6, size_y / 2 - 12, str);

		total_fps += fps;
		frames_count ++;

		LARGE_INTEGER stop = {};
		QueryPerformanceCounter (&stop);

		time = 1000.0 * (double) (stop.QuadPart - start.QuadPart) / (double) freq.QuadPart;

		txSleep (0);
	}

	const char* build = "";

#ifdef _DEBUG
	build = "debug";
#else
	build = "release";
#endif

	double fps_median = total_fps / frames_count;
	fprintf_s (file, "Median fps: %.3lf [%d frames tested in %.0lf msec, %s build], console drawing %s\n", fps_median, frames_count, time, build, (_txConsole < 0)? "disabled": "enbled");

	fclose (file);
}

//------------------------