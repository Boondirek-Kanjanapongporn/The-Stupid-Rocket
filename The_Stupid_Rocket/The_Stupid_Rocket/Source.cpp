#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <string>
#include <vector>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <raylib.h>
#include <math.h>
#include "dfs.h"
#include "bfs.h"
#define screenWidth 1280
#define screenHeight 720
#define MAXLEN 10
#define MAXWORD 2
#define LIGHTGRAY  Color{200, 200, 200, 230}	// Light Gray
//#define BLACKTRANS80 Color{0, 0, 0, 204}		// BLACK color transparent 80%
#define BLACKTRANS85 Color{0, 0, 0, 216}		// BLACK color transparent 85%
//#define BLACKTRANS90 Color{0, 0, 0, 230}		// BLACK color transparent 90%
#define DARKRED Color{139, 0, 0, 255}			// Dark Red
#define CHERRYRED Color{225, 27, 34, 255}		// Cherry Red

//Variables for DFS algorithm
std::vector <int> coordinatesDFS;
std::string pathDFS;

//Variables for BFS algorithm
std::vector <int> coordinatesBFS;
std::string pathBFS;

typedef enum {TITLE, INITIALIZE, SIMULATE, END} Gamescreen;

void pathtoCoordinates(std::string* inpath, std::vector <int>& incoordinates) {
	char* token;
	char* temp;
	int len;
	temp = (char*)malloc((*inpath).length() * sizeof(char));
	strcpy(temp, (*inpath).c_str());

	for (len = 0; temp[len] != '\0'; len++);

	token = strtok(temp, "[,] ");
	for (int i = 0; i < len; i++) {
		if (token != NULL) {
			incoordinates.push_back(atoi(token));
		}
		token = strtok(NULL, "[,] ");
	}
}

void splitStringtoNum(char inStr[MAXLEN], int* pointY, int* pointX) {
	char* token;
	char temp[MAXLEN];
	for (int i = 0; i < MAXLEN; i++) {
		temp[i] = inStr[i];
	}
	token = strtok(temp, " ");
	for (int i = 0; i < MAXWORD; i++) {
		if (token != NULL) {
			if (i == 0)
				*pointY = atoi(token);
			else
				*pointX = atoi(token);
		}
		token = strtok(NULL, " ");
	}
}

void my_project() {
	// Initialization
	//--------------------------------------------------------------------------------------
	InitWindow(screenWidth, screenHeight, "The Stupid Rocket");

	Gamescreen currentScreen = TITLE;

	// All integers, floats, and pointers
	int CURRENT_TIME = 0;
	int** MAZE = nullptr;
	int ROWNUM = 5, COLNUM = 5;
	int STARTX, STARTY;
	int ENDX, ENDY;
	int REFX = screenWidth / 8, REFY = screenHeight / 10 - 10;
	float WIDTH = 850.0f, HEIGHT = 535.0f;
	float ROWLENGTH, COLLENGTH;
	int CLICKX, CLICKY;
	int lettercount = 0;
	int lettercount1 = 0;
	int lettercount2 = 0;
	int lettercount3 = 0;

	// Booleans
	bool mouseOnText = false;
	bool executeAction = false;
	bool Warning = false;
	bool maze_done = false;
	bool mouseOnCheckbox = false;
	bool nextAction = false;
	bool simulateAction = false;
	bool backAction = false;
	bool launchAction = false;
	bool DFS = false;
	bool BFS = false;
	bool launch = false;
	bool pathfoundDFS = false;
	bool pathfoundBFS = false;
	
	// Input Texts for Initialize Screen
	char rocket_name_text[MAXLEN + 6] = "\0";
	char width_height_text[MAXLEN + 1] = "\0";
	char start_node_text[MAXLEN + 1] = "\0";
	char end_node_text[MAXLEN + 1] = "\0";
	char end_screen_text[MAXLEN + 35] = "\0";

	// Scrolling back materials
	float scrollingBack = 0.0f;
	float scrollingUp = 0.0f;

	Texture2D start_background = LoadTexture("space background.png");
	Texture2D rocket_logo = LoadTexture("rocket.png");
	Texture2D rocket_icon = LoadTexture("rocket1.png");
	Texture2D logo_name = LoadTexture("Stupid Rocket title.png");
	Texture2D asteroid = LoadTexture("asteroid.png");
	Texture2D planet = LoadTexture("planet3.png");

	Vector2 mousePosition = { 0.0f, 0.0f };
	Vector2 start_background1 = {scrollingBack, -10};
	Vector2 start_background2 = {float(start_background.width * 0.25 + scrollingBack), -10};
	Vector2 rocket_vector2 = {screenWidth / 2, screenHeight / 4 - 40};
	Vector2 logo_vector2 = {screenWidth / 4 + 25, screenHeight / 2};
	Vector2 sprite_vector2 = {};
	Vector2 asteroid_vector2 = {};
	Vector2 planet_vector2 = {};

	Rectangle start_rectangle = {screenWidth / 4 + 15, screenHeight / 2 - 5, 620, 80};
	Rectangle main_rectangle = { (screenWidth - 1100) / 2, (screenHeight - 700) / 2, 1100, 700};
	Rectangle rocket_name_textBox = { (screenWidth / 2) + 30, screenHeight / 20, 400, 50 };
	Rectangle width_height_textBox = {(screenWidth / 2) + 130, screenHeight / 20 + 90, 300, 50 };
	Rectangle start_node_textBox = { (screenWidth / 2) + 130, screenHeight / 20 + 180, 300, 50 };
	Rectangle end_node_textBox = { (screenWidth / 2) + 130, screenHeight / 20 + 270, 300, 50 };
	Rectangle next_button = { screenWidth / 8, screenHeight/20 + 400, 300, 70 };
	Rectangle simulate_button = { REFX, REFY + HEIGHT + 20, 300, 70};
	Rectangle back_button = { REFX + 685, REFY + HEIGHT + 20, 300, 70 };
	Rectangle ref_maze = { REFX, REFY, WIDTH, HEIGHT };
	Rectangle checkbox1 = {REFX + WIDTH + 30, REFY + 20, 20, 20};
	Rectangle checkbox2 = { REFX + WIDTH + 30, REFY + 80, 20, 20 };
	Rectangle checkbox3 = { REFX + WIDTH + 30, REFY + 150, 20, 20 };

	//Buffer Check
	char BufferWidth[MAXLEN];
	char display_width_height_name[MAXLEN*3] = "5 X 5";

	SetTargetFPS(60);             // Set our game to run at 60 frames-per-second
	//--------------------------------------------------------------------------------------

	// Main game loop
	while (!WindowShouldClose()) {						// Detect window close button or ESC key
		// Update----------------------------------------------------------------------------------
		CURRENT_TIME += 1;
		mousePosition = GetMousePosition();
		scrollingBack -= 0.5f;

		if (scrollingBack <= -start_background.width * 0.25)
			scrollingBack = 0;

		// Draw----------------------------------------------------------------------------------
		BeginDrawing();

		// Display background
		ClearBackground(GetColor(0x052c46ff));
		DrawTextureEx(start_background, Vector2 { scrollingBack, -10 }, 0.0f, 0.25f, WHITE);
		DrawTextureEx(start_background, Vector2 {float(start_background.width * 0.25 + scrollingBack), -10}, 0.0f, 0.25f, WHITE);

		switch (currentScreen) {
			case TITLE:

				DrawRectangleRounded(start_rectangle, 0.5f, 1, BLACK);
				DrawTextureEx(rocket_logo, rocket_vector2, 42.0f, 0.15f, WHITE);
				DrawTextureEx(logo_name, logo_vector2, 0.0f, 0.5f, WHITE);
				if ((CURRENT_TIME)/10 % 10 != 0)
					DrawText("PRESS [ENTER] to START!", screenWidth / 3.5 + 10, screenHeight / 1.3 - 15, 40, WHITE);

				if (IsKeyPressed(KEY_ENTER)) {
					CURRENT_TIME = 0;
					currentScreen = INITIALIZE;
				}
				break;

			case INITIALIZE:

				mouseOnText = false;
				if (CheckCollisionPointRec(mousePosition, rocket_name_textBox) || CheckCollisionPointRec(mousePosition, width_height_textBox) || CheckCollisionPointRec(mousePosition, start_node_textBox) || CheckCollisionPointRec(mousePosition, end_node_textBox))
					mouseOnText = true;
				if (mouseOnText) {
					// Get pressed key (character) on the queue
					int key = GetKeyPressed();
					while (key > 0) {
						if (CheckCollisionPointRec(mousePosition, rocket_name_textBox)) {
							if (key >= 32 && key <= 122 && lettercount < MAXLEN + 5) {
								rocket_name_text[lettercount] = (char)key;
								lettercount++;
							}
						}
						// NOTE: Only allow keys in range [45..57] int values and white space [32] only
						else if (key >= 48 && key <= 57 || key == 32) {
							if (CheckCollisionPointRec(mousePosition, width_height_textBox) && lettercount1 < MAXLEN) {
								width_height_text[lettercount1] = (char)key;
								lettercount1++;
							}
							else if (CheckCollisionPointRec(mousePosition, start_node_textBox) && lettercount2 < MAXLEN) {
								start_node_text[lettercount2] = (char)key;
								lettercount2++;
							}
							else if (CheckCollisionPointRec(mousePosition, end_node_textBox) && lettercount3 < MAXLEN) {
								end_node_text[lettercount3] = (char)key;
								lettercount3++;
							}
						}
						key = GetKeyPressed();
					}
					// Check backspace key
					if (IsKeyPressed(KEY_BACKSPACE)) {
						if (CheckCollisionPointRec(mousePosition, rocket_name_textBox) && lettercount > 0) {
							lettercount--;
							rocket_name_text[lettercount] = '\0';		// Check next character in the queue
							if (lettercount < 0)
								lettercount = 0;
						}
						else if (CheckCollisionPointRec(mousePosition, width_height_textBox) && lettercount1 > 0) {
							lettercount1--;
							width_height_text[lettercount1] = '\0';		// Check next character in the queue
							if (lettercount1 < 0)
								lettercount1 = 0;
						}
						else if (CheckCollisionPointRec(mousePosition, start_node_textBox) && lettercount2 > 0) {
							lettercount2--;
							start_node_text[lettercount2] = '\0';		// Check next character in the queue
							if (lettercount2 < 0)
								lettercount2 = 0;
						}
						else if (CheckCollisionPointRec(mousePosition, end_node_textBox) && lettercount3 > 0) {
							lettercount3--;
							end_node_text[lettercount3] = '\0';		// Check next character in the queue
							if (lettercount3 < 0)
								lettercount3 = 0;
						}
					}
				}
				DrawRectangleRounded(main_rectangle, 0.1f, 1, BLACKTRANS85);
				DrawText("Enter Your Rocket Name :", screenWidth/8, screenHeight/15, 35, LIGHTGRAY);
				DrawText("Enter Row and Column (%d %d) :", screenWidth/8, screenHeight/15 + 90, 35, LIGHTGRAY);
				DrawText("Enter Start Node (Row Col) :", screenWidth/8, screenHeight/15 + 180, 35, LIGHTGRAY);
				DrawText("Enter End Node (Row Col) :", screenWidth / 8, screenHeight / 15 + 270, 35, LIGHTGRAY);
				DrawRectangleRounded(rocket_name_textBox, 0.5f, 1, LIGHTGRAY);
				DrawRectangleRounded(width_height_textBox, 0.5f, 1, LIGHTGRAY);
				DrawRectangleRounded(start_node_textBox, 0.5f, 1, LIGHTGRAY);
				DrawRectangleRounded(end_node_textBox, 0.5f, 1, LIGHTGRAY);

				// Draw Red border line when cursor hovers over
				if (mouseOnText) {
					if (CheckCollisionPointRec(mousePosition, rocket_name_textBox))
						DrawRectangleRoundedLines(rocket_name_textBox, 0.1f, 1, 2, RED);
					else if (CheckCollisionPointRec(mousePosition, width_height_textBox))
						DrawRectangleRoundedLines(width_height_textBox, 0.1f, 1, 2, RED);
					else if (CheckCollisionPointRec(mousePosition, start_node_textBox))
						DrawRectangleRoundedLines(start_node_textBox, 0.1f, 1, 2, RED);
					else if (CheckCollisionPointRec(mousePosition, end_node_textBox))
						DrawRectangleRoundedLines(end_node_textBox, 0.1f, 1, 2, RED);
				}

				//Draw Update Texts for different section Inputs
				DrawText(rocket_name_text, rocket_name_textBox.x + 10, rocket_name_textBox.y + 10, 30, BLACK);
				DrawText(width_height_text, width_height_textBox.x + 10, width_height_textBox.y + 10, 30, BLACK);
				DrawText(start_node_text, start_node_textBox.x + 10, start_node_textBox.y + 10, 30, BLACK);
				DrawText(end_node_text, end_node_textBox.x + 10, end_node_textBox.y + 10, 30, BLACK);

				// Draw Pop-up Next button upon entering all sections and its action
				if (strlen(rocket_name_text) >= 1 && strlen(width_height_text) >= 3 && strlen(start_node_text) >= 3 && strlen(end_node_text) >= 3) {
					DrawRectangleRounded(next_button, 0.5f, 1, CHERRYRED);
					DrawText("NEXT", next_button.x + 95, next_button.y + 15, 40, WHITE);
					if (CheckCollisionPointRec(mousePosition, next_button)) {
						if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
							DrawRectangleRounded(next_button, 0.5f, 1, DARKRED);
							DrawText("NEXT", next_button.x + 95, next_button.y + 15, 40, GRAY);
						}
						if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
							nextAction = true;
					}
				}

				// Check node boundaries before entering next Screen
				if (nextAction == true) {
					splitStringtoNum(width_height_text,&ROWNUM, &COLNUM);
					splitStringtoNum(start_node_text, &STARTY, &STARTX);
					splitStringtoNum(end_node_text, &ENDY, &ENDX);
					if (STARTX < COLNUM && STARTY < ROWNUM && ENDX < COLNUM && ENDY < ROWNUM && STARTX >= 0 && STARTY >= 0 && ENDX >= 0 && ENDY >= 0) {
						Warning = false;
						snprintf(display_width_height_name, sizeof(display_width_height_name), "%d", ROWNUM);
						snprintf(BufferWidth, sizeof(BufferWidth), "%d", COLNUM);
						strcat(display_width_height_name, " X ");
						strcat(display_width_height_name, BufferWidth);
						strcat(display_width_height_name, "  [ ");
						strcat(display_width_height_name, rocket_name_text);
						strcat(display_width_height_name, " ]");
						currentScreen = SIMULATE;
					}
					else {
						Warning = true;
					}
					nextAction = false;
				}

				// Warning Pop up when nodes are out of boundaries
				if (Warning == true) {
					DrawText("*Start or End Node is/are out of boundaries...", next_button.x + 350, next_button.y + 5, 20, RED);
					DrawText(" Hint: Array starts with 0!!!", next_button.x + 350, next_button.y + 45, 20, RED);
				}
				break;

			case SIMULATE:

				if (maze_done == false) {
					ROWLENGTH = HEIGHT / ROWNUM;
					COLLENGTH = WIDTH / COLNUM;

					// Initialize MAZE by allocate ROW MAZE
					MAZE = (int **)malloc(sizeof(int*) * ROWNUM);

					// Allocate COL MAZE
					for (int i = 0; i < ROWNUM; i++)
						MAZE[i] = (int *)malloc(sizeof(int) * COLNUM);

					// Populate all MAZE[ROWNUM][COLNUM] values to 0
					for (int i = 0; i < ROWNUM; i++) {
						for (int j = 0; j < COLNUM; j++) {
							MAZE[i][j] = 0; 
						}
					}
					// Set Planet Goal
					planet_vector2 = { REFX + COLLENGTH * (ENDX), REFY + ROWLENGTH * (ENDY + 1/10.0f)};
					maze_done = true;
				}

				// Update Sprite Rocket (Player)
				sprite_vector2 = {REFX + (COLLENGTH * (STARTX + 1/7.0f)), REFY + (ROWLENGTH * (STARTY + 1 / 15.0f))};

				// Draw Main Maze
				DrawRectangleRounded(main_rectangle, 0.1f, 1, BLACKTRANS85);
				DrawText(display_width_height_name, REFX, screenHeight / 15 - 25, 30, LIGHTGRAY);
				DrawRectangleLines(ref_maze.x, ref_maze.y, ref_maze.width, ref_maze.height, LIGHTGRAY);
				for (int i = 1; i < ROWNUM; i++) {
					DrawLine(REFX, REFY + i * (ROWLENGTH), REFX + WIDTH, REFY + i * (ROWLENGTH), LIGHTGRAY);
				}
				for (int i = 1; i < COLNUM; i++) {
					DrawLine(REFX + i * (COLLENGTH), REFY, REFX + i * (COLLENGTH), REFY + HEIGHT, LIGHTGRAY);
				}

				// Pin-point Mouse position to know the indexes of the Row and Column in MAZE
				if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
					CLICKX = int(floor((mousePosition.x - REFX) / COLLENGTH));
					CLICKY = int(floor((mousePosition.y - REFY) / ROWLENGTH));
					if (CLICKX >= 0 && CLICKX < COLNUM && CLICKY >= 0 && CLICKY < ROWNUM && (CLICKX != STARTX || CLICKY != STARTY) && (CLICKX != ENDX || CLICKY != ENDY)) {
						pathfoundDFS = false;		// Path might change
						pathfoundBFS = false;		// Path might change
						if (MAZE[CLICKY][CLICKX] == 0)
							MAZE[CLICKY][CLICKX] = 1;
						else
							MAZE[CLICKY][CLICKX] = 0;
						for (int i = 0; i < ROWNUM; i++) {
							for (int j = 0; j < COLNUM; j++) {
								printf("%d ", MAZE[i][j]);
							}
							printf("\n");
						}
						printf("\n");
					}
				}

				// Draw asteroid on the pin-point mouse position
				for (int i = 0; i < ROWNUM; i++) {
					for (int j = 0; j < COLNUM; j++) {
						if (MAZE[i][j] == 1) {
							asteroid_vector2 = { REFX + (COLLENGTH * (j)), REFY + (ROWLENGTH * (i + 1 / 10.0f)) };
							DrawTextureEx(asteroid, asteroid_vector2, 0.0f, 0.5f / ((ROWNUM) / 4.0f), WHITE);
						}
					}
				}

				// Check Checkbox DFS/BFS boolean conditions
				if (CheckCollisionPointRec(mousePosition, checkbox1) || CheckCollisionPointRec(mousePosition, checkbox2) || CheckCollisionPointRec(mousePosition, checkbox3))
					mouseOnCheckbox = true;
				if (mouseOnCheckbox == true) {
					if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
						if (CheckCollisionPointRec(mousePosition, checkbox1)) {
							if (DFS == false) {
								DFS = true;
								BFS = false;
							}
							else
								DFS = false;
						}
						else if (CheckCollisionPointRec(mousePosition, checkbox2)) {
							if (BFS == false) {
								BFS = true;
								DFS = false;
							}
							else
								BFS = false;
						}
						else if (CheckCollisionPointRec(mousePosition, checkbox3)) {
							if (launch == false)
								launch = true;
							else
								launch = false;
						}
					}
				}

				// Draw Checkboxes for DFS, BFS, and LAUNCH
				if (DFS == true)
					DrawRectangleRec(checkbox1, RED);
				if (BFS == true)
					DrawRectangleRec(checkbox2, RED);
				if (launch == true)
					DrawRectangleRec(checkbox3, RED);
				DrawRectangleLinesEx(checkbox1, 1.0f, WHITE);
				DrawRectangleLinesEx(checkbox2, 1.0f, WHITE);
				DrawRectangleLinesEx(checkbox3, 1.0f, WHITE);
				DrawText("DFS", checkbox1.x + 35, checkbox1.y - 5, 35, LIGHTGRAY);
				DrawText("BFS", checkbox2.x + 35, checkbox2.y - 5, 35, LIGHTGRAY);
				DrawText("LAUNCH", checkbox3.x + 35, checkbox3.y - 2, 28, LIGHTGRAY);

				// Draw Simulate Button and its action
				DrawRectangleRounded(simulate_button, 0.5f, 1, CHERRYRED);
				DrawText("SIMULATE", simulate_button.x + 45, simulate_button.y + 15, 40, WHITE);
				if (CheckCollisionPointRec(mousePosition, simulate_button)) {
					if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
						DrawRectangleRounded(simulate_button, 0.5f, 1, DARKRED);
						DrawText("SIMULATE", simulate_button.x + 45, simulate_button.y + 15, 40, GRAY);
					}
					if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
						simulateAction = true;
				}
				if (simulateAction == true) {
					if (launch == false) {
						if (DFS == true && pathfoundDFS == false) {
							printf("Simualting DFS\n");
							pathDFS.clear();
							cleardfs();
							if (dfs(STARTY, STARTX, "", ENDY, ENDX, MAZE, ROWNUM, COLNUM, pathDFS)) {
								std::cout << pathDFS << std::endl;
								coordinatesDFS.clear();
								pathtoCoordinates(&pathDFS, coordinatesDFS);
								pathfoundDFS = true;
							}
							else {
								printf("No Path found DFS\n");
								//pathfailedDFS = true;
							}
						}
						else if (BFS == true && pathfoundBFS == false) {
							printf("Simulating BFS\n");
							pathBFS.clear();
							clearbfs();
							if (bfs(STARTY, STARTX, ENDY, ENDX, MAZE, ROWNUM, COLNUM, pathBFS)) {
								std::cout << pathBFS << std::endl;
								coordinatesBFS.clear();
								pathtoCoordinates(&pathBFS, coordinatesBFS);
								pathfoundBFS = true;
							}
							else {
								printf("No Path found BFS\n");
								//pathfailedBFS = true;
							}
						}
					}
					else {
						if ((DFS == true && pathfoundDFS == true) || (BFS == true && pathfoundBFS == true)) {
							launchAction = true;
							system("Apollo_11_Launch.mp4");
							strcat(end_screen_text, rocket_name_text);
							strcat(end_screen_text, " has Launched Succesfully!!!");
							currentScreen = END;
						}
					}
					simulateAction = false;
				}

				// Draw Back Button and its action
				DrawRectangleRounded(back_button, 0.5f, 1, BLUE);
				DrawText("BACK", back_button.x + 95, back_button.y + 17, 40, WHITE);
				if (CheckCollisionPointRec(mousePosition, back_button)) {
					if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
						DrawRectangleRounded(back_button, 0.5f, 1, DARKBLUE);
						DrawText("BACK", back_button.x + 95, back_button.y + 17, 40, GRAY);
					}
					if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
						backAction = true;
				}
				if (backAction == true) {
					// De-allocating/freeing 2d MAZE array to start a new
					for (int i = 0; i < ROWNUM; i++)
						free(MAZE[i]);
					free(MAZE);

					// Set essential booleans to default
					maze_done = false;
					DFS = false;
					BFS = false;
					pathfoundDFS = false;
					pathfoundBFS = false;
					backAction = false;

					// Go back to INITIALIZE page for fixes
					currentScreen = INITIALIZE;
				}

				// Warning Pop up when no AI path is chosen 
				if (DFS == false && BFS == false) {
					DrawText("*AI Path not Chosen!!!", simulate_button.x + 320, simulate_button.y + 15, 25, RED);
				}

				// If DFS path is found
				if (DFS == true && pathfoundDFS == true) {
					for (int i = 0; i < coordinatesDFS.size() - 1; i += 2)
						DrawRectangle(REFX + COLLENGTH * (coordinatesDFS[i+1])+0.5, REFY + ROWLENGTH * (coordinatesDFS[i])+0.5, COLLENGTH-1, ROWLENGTH-1, DARKRED);
				}

				// If BFS path is found
				if (BFS == true && pathfoundBFS == true) {
					for (int i = 0; i < coordinatesBFS.size() - 1; i += 2)
						DrawRectangle(REFX + COLLENGTH * (coordinatesBFS[i + 1]) + 0.5, REFY + ROWLENGTH * (coordinatesBFS[i]) + 0.5, COLLENGTH - 1, ROWLENGTH - 1, DARKRED);
				}

				// Draw Sprite Rocket (Player)
				DrawTextureEx(planet, planet_vector2, 0.0f, 0.5f / ((ROWNUM) / 4.0f), WHITE);
				DrawTextureEx(rocket_icon, sprite_vector2, 0.0f, 0.4f / ((ROWNUM) / 4.0f), WHITE);
				break;

			case END:

				DrawRectangleRounded(main_rectangle, 0.1f, 1, BLACKTRANS85);
				DrawText(end_screen_text, 190, 300, 40, WHITE);
				if ((CURRENT_TIME) / 10 % 10 != 0)
					DrawText("PRESS [ESC] to QUIT...", screenWidth / 3.5 + 40, screenHeight / 1.3 - 15, 40, WHITE);

				break;

		}

		EndDrawing();
		//----------------------------------------------------------------------------------
	}
	// De-Initialization
	//--------------------------------------------------------------------------------------
	UnloadTexture(start_background);  // Unload start background texture
	UnloadTexture(rocket_logo);
	UnloadTexture(logo_name);
	UnloadTexture(rocket_icon);
	UnloadTexture(asteroid);
	UnloadTexture(planet);

	CloseWindow();              // Close window and OpenGL context
	//--------------------------------------------------------------------------------------
}

int main() {
	my_project();

	return 0;
}