#include <stdio.h>
#include <ncurses.h>
#include <string.h>
#include "gamepad.h"	//gamepad libary from elanthis/gamepad


//defines for global Booleans
#define BUTTON_X_define_mos 0
#define BUTTON_Y_define_mos 1
#define BUTTON_A_define_mos 2
#define BUTTON_B_define_mos 3
#define BUTTON_RB_define_mos 4
#define BUTTON_LB_define_mos 5
#define BUTTON_START_define_mos 6
#define BUTTON_BACK_define_mos 7
#define BUTTON_RIGHT_STICK_define_mos 8
#define BUTTON_LEFT_STICK_define_mos 9
#define KREUZ_UP_define_mos 10
#define KREUZ_DOWN_define_mos 11
#define KREUZ_LEFT_define_mos 12
#define KREUZ_RIGHT_define_mos 13

#define BUTTON_LEFT_TRIGGER_define_mos 0
#define BUTTON_RIGHT_TRIGGER_define_mos 1
#define STICK_LEFT_STICK_define_mos 2
#define STICK_LEFT_ANGEL_define_mos 3
#define STICK_RIGHT_STICK_define_mos 4
#define STICK_RIGHT_ANGEL_define_mos 5


enum gamepad_all_items {BUT_X, BUT_Y, BUT_A, BUT_B, BUT_RB, BUT_LB, BUT_START, BUT_BACK, BUT_RIGHT_STICK, BUT_LEFT_STICK, BUT_LEFT_TRIGGER, BUT_RIGHT_TRIGGER,STICK_LEFT_STICK,STICK_RIGHT_STICK,KREUZ_UP,KREUZ_RIGHT,KREUZ_LEFT,KREUZ_DOWN, UNKNOWN };	//all implemented one click items

//global booleans in these the buttons are saved from the functions
bool g_mos[14];
//global double for TRIGGERS
double g_trigger_mos[6];


struct gamepad_items	//object used to find the chosen item, for case with string
{
	char *name;
	enum gamepad_all_items id;
} gamepad_but_list[] = {
	{"k_up",KREUZ_UP},
	{"k_down",KREUZ_DOWN},
	{"k_right",KREUZ_RIGHT},
	{"k_left",KREUZ_LEFT},
	{"sl",STICK_LEFT_STICK},
	{"sr",STICK_RIGHT_STICK},
	{"rt",BUT_RIGHT_TRIGGER},
	{"lt",BUT_LEFT_TRIGGER},
	{"rb",BUT_RB},
	{"lb",BUT_LB},
	{"start",BUT_START},
	{"back",BUT_BACK},
	{"left_stick",BUT_LEFT_STICK},
	{"right_stick",BUT_RIGHT_STICK},
	{"a",BUT_A},
	{"b",BUT_B},
	{"x",BUT_X},
	{"y",BUT_Y}
};


void get_button_mos(GAMEPAD_DEVICE dev,char const button_wanted[11]); //get a single button upgraded, wont trigger multiple times
void update_all_but(GAMEPAD_DEVICE dev);	// get all buttons upgraded, wont trigger multiple times
// if both functions are used events can trigger 2 times cause they dont share the boolean memory
void test_all_but_mos(GAMEPAD_DEVICE GAMEPAD_0);	//function to test if all buttions are right
void g_mos_reset();	//resets all buttons to FALSE
void init_gamepad_rogmos(); //initialisation of the gamepad


void get_button_mos(GAMEPAD_DEVICE dev,char const button_wanted[6])
{
	static bool new_bool,old_bool[14],first_run=TRUE;	//new bool is a temp var, old bool saves the state of the last new bool to deny multiple events
	int chosen_but;	//saves the chosen button number
	unsigned long int i;	//used for a loop

	enum gamepad_all_items my_gamepad_items;	//var to detect which item was hit
	struct gamepad_items *choice = NULL;

	bool trigger_flag=FALSE;

	GamepadUpdate();	//updates the var of the gamepad but not the var in the program
	move(dev, 0);
	if(first_run==TRUE)	// detects first run and sets all global booleans to false
	{
		first_run=FALSE;
		for(i=0;i<14;i++)
		{
			old_bool[i]=FALSE;
		}
	}
	
	for(i=0, choice=NULL;i< sizeof gamepad_but_list/sizeof (struct gamepad_items);i++) //runs max to the amount of items in the list
	{
		if (strcmp(button_wanted, gamepad_but_list[i].name) == 0)//checks if hit was archieved -> wanted button is existing
		{	//hit
			choice = gamepad_but_list + i;
			break;
		}
	}

	my_gamepad_items = choice ? choice ->id : UNKNOWN;	//saves the item in my_gamepad_itesm, if hit then for example the code for X-Button is saved

	switch(my_gamepad_items)	//Runs the case for the the gamepad item -> saves Button Number and reads Button state
	{
		case BUT_RB:
			new_bool=GamepadButtonDown(dev, BUTTON_RIGHT_SHOULDER);
			chosen_but=BUTTON_RB_define_mos;
			break;
		case BUT_LB:
			new_bool=GamepadButtonDown(dev, BUTTON_LEFT_SHOULDER);
			chosen_but=BUTTON_LB_define_mos;
			break;
		case BUT_START:
			new_bool=GamepadButtonDown(dev, BUTTON_START);
			chosen_but=BUTTON_START_define_mos;
			break;
		case BUT_BACK:
			new_bool=GamepadButtonDown(dev, BUTTON_BACK);
			chosen_but=BUTTON_BACK_define_mos;
			break;
		case BUT_A:
			new_bool=GamepadButtonDown(dev, BUTTON_A);
			chosen_but=BUTTON_A_define_mos;
			break;
		case BUT_B:
			new_bool=GamepadButtonDown(dev, BUTTON_B);
			chosen_but=BUTTON_B_define_mos;
			break;
		case BUT_Y:
			new_bool=GamepadButtonDown(dev, BUTTON_Y);
			chosen_but=BUTTON_Y_define_mos;
			break;
		case BUT_X:
			new_bool=GamepadButtonDown(dev, BUTTON_X);
			chosen_but=BUTTON_X_define_mos;
			break;
		case BUT_LEFT_STICK:
			new_bool=GamepadButtonDown(dev, BUTTON_LEFT_THUMB);
			chosen_but=BUTTON_LEFT_STICK_define_mos;
			break;
		case BUT_RIGHT_STICK:
			new_bool=GamepadButtonDown(dev, BUTTON_RIGHT_THUMB);
			chosen_but=BUTTON_RIGHT_STICK_define_mos;
			break;
		case BUT_LEFT_TRIGGER:
			g_trigger_mos[BUTTON_LEFT_TRIGGER_define_mos]=GamepadTriggerLength(dev, TRIGGER_LEFT);
			trigger_flag=TRUE;
			break;
		case BUT_RIGHT_TRIGGER:
			g_trigger_mos[BUTTON_RIGHT_TRIGGER_define_mos]=GamepadTriggerLength(dev, TRIGGER_RIGHT);
			trigger_flag=TRUE;
			break;
		case STICK_LEFT_STICK:
			g_trigger_mos[STICK_LEFT_STICK_define_mos]=GamepadStickLength(dev, STICK_LEFT);
			g_trigger_mos[STICK_LEFT_ANGEL_define_mos]=GamepadStickLength(dev, STICK_LEFT);
			trigger_flag=TRUE;
			break;
		case STICK_RIGHT_STICK:
			g_trigger_mos[STICK_RIGHT_STICK_define_mos]=GamepadStickLength(dev, STICK_RIGHT);
			g_trigger_mos[STICK_RIGHT_ANGEL_define_mos]=GamepadStickLength(dev, STICK_RIGHT);
			trigger_flag=TRUE;
			break;
		case KREUZ_DOWN:
			new_bool=GamepadButtonDown(dev, BUTTON_DPAD_DOWN);
			chosen_but=KREUZ_DOWN_define_mos;
			break;
		case KREUZ_UP:
			new_bool=GamepadButtonDown(dev, BUTTON_DPAD_UP);
			chosen_but=KREUZ_UP_define_mos;
			break;
		case KREUZ_LEFT:
			new_bool=GamepadButtonDown(dev, BUTTON_DPAD_LEFT);
			chosen_but=KREUZ_LEFT_define_mos;
			break;
		case KREUZ_RIGHT:
			new_bool=GamepadButtonDown(dev, BUTTON_DPAD_RIGHT);
			chosen_but=KREUZ_RIGHT_define_mos;
			break;
		default:	//ERROR Button wasnt found
			puts("Die Russen kommen");
			break;

	}

	if(trigger_flag==FALSE){
		if(old_bool[chosen_but]==TRUE&&new_bool==TRUE)//to deny multiple events
		{
			g_mos[chosen_but]=FALSE;		
		}
		else if(old_bool[chosen_but]==FALSE&&new_bool==TRUE)//new event found
		{
			g_mos[chosen_but]=TRUE;
		}
		else//button still 0 or not pressed
		{
			g_mos[chosen_but]=FALSE;
		}

		old_bool[chosen_but]=new_bool; // saves new_bool for multiple event detection
	}

}

void update_all_but(GAMEPAD_DEVICE dev)
{	//similar to get_button_mos only without chosing one button and updating all buttons
	static bool new_bool,old_bool[14],first_run=TRUE;
	//static char old_button[6];
	int chosen_but,i;

	bool trigger_flag=FALSE;

	GamepadUpdate();
	if(first_run==TRUE)
	{
		//strcpy(old_button, button_wanted);
		first_run=FALSE;
		for(i=0;i<sizeof gamepad_but_list/sizeof (struct gamepad_items);i++)
		{
			old_bool[i]=FALSE;
		}
	}
	move(dev, 0);
		for(i=0;i<sizeof gamepad_but_list/sizeof (struct gamepad_items);i++)
		{
			switch(i)	//da muasd nu alles ausfüllen
			{
				case BUTTON_RB_define_mos:
					new_bool=GamepadButtonDown(dev, BUTTON_RIGHT_SHOULDER);
					chosen_but=BUTTON_RB_define_mos;
					break;
				case BUTTON_LB_define_mos:
					new_bool=GamepadButtonDown(dev, BUTTON_LEFT_SHOULDER);
					chosen_but=BUTTON_LB_define_mos;
					break;
				case BUTTON_START_define_mos:
					new_bool=GamepadButtonDown(dev, BUTTON_START);
					chosen_but=BUTTON_START_define_mos;
					break;
				case BUTTON_BACK_define_mos:
					new_bool=GamepadButtonDown(dev, BUTTON_BACK);
					chosen_but=BUTTON_BACK_define_mos;
					break;
				case BUTTON_A_define_mos:
					new_bool=GamepadButtonDown(dev, BUTTON_A);
					chosen_but=BUTTON_A_define_mos;
					break;
				case BUTTON_B_define_mos:
					new_bool=GamepadButtonDown(dev, BUTTON_B);
					chosen_but=BUTTON_B_define_mos;
					break;
				case BUTTON_Y_define_mos:
					new_bool=GamepadButtonDown(dev, BUTTON_Y);
					chosen_but=BUTTON_Y_define_mos;
					break;
				case BUTTON_X_define_mos:
					new_bool=GamepadButtonDown(dev, BUTTON_X);
					chosen_but=BUTTON_X_define_mos;
					break;
				case BUTTON_LEFT_STICK_define_mos:
					new_bool=GamepadButtonDown(dev, BUTTON_LEFT_THUMB);
					chosen_but=BUTTON_LEFT_STICK_define_mos;
					break;
				case BUTTON_RIGHT_STICK_define_mos:
					new_bool=GamepadButtonDown(dev, BUTTON_RIGHT_THUMB);
					chosen_but=BUTTON_RIGHT_STICK_define_mos;
					break;
				case BUT_LEFT_TRIGGER:
					g_trigger_mos[BUTTON_LEFT_TRIGGER_define_mos]=GamepadTriggerLength(dev, TRIGGER_LEFT);
					trigger_flag=TRUE;
					break;
				case BUT_RIGHT_TRIGGER:
					g_trigger_mos[BUTTON_RIGHT_TRIGGER_define_mos]=GamepadTriggerLength(dev, TRIGGER_RIGHT);
					trigger_flag=TRUE;
					break;
				case STICK_LEFT_STICK:
					g_trigger_mos[STICK_LEFT_STICK_define_mos]=GamepadStickLength(dev, STICK_LEFT);
					g_trigger_mos[STICK_LEFT_ANGEL_define_mos]=GamepadStickAngle(dev, STICK_LEFT);
					trigger_flag=TRUE;
					break;
				case STICK_RIGHT_STICK:
					g_trigger_mos[STICK_RIGHT_STICK_define_mos]=GamepadStickLength(dev, STICK_RIGHT);
					g_trigger_mos[STICK_RIGHT_ANGEL_define_mos]=GamepadStickAngle(dev, STICK_RIGHT);
					trigger_flag=TRUE;
					break;
				case KREUZ_DOWN:	//only KREUZ Up is working
					new_bool=GamepadButtonDown(dev, BUTTON_DPAD_DOWN);
					//printf("KREUZ DOWN-- %d\n",GamepadButtonDown(dev, BUTTON_DPAD_DOWN));
					chosen_but=KREUZ_DOWN_define_mos;
					break;
				case KREUZ_UP:
					new_bool=GamepadButtonDown(dev, BUTTON_DPAD_UP);
					chosen_but=KREUZ_UP_define_mos;
					break;
				case KREUZ_LEFT:
					new_bool=GamepadButtonDown(dev, BUTTON_DPAD_LEFT);
					chosen_but=KREUZ_LEFT_define_mos;
					break;
				case KREUZ_RIGHT:
					new_bool=GamepadButtonDown(dev, BUTTON_DPAD_RIGHT);
					chosen_but=KREUZ_RIGHT_define_mos;
					break;
				default:
					puts("Die Russen kommen");
					break;
			}
			if(trigger_flag==FALSE){
				if(old_bool[chosen_but]==TRUE&&new_bool==TRUE)
				{
					g_mos[chosen_but]=FALSE;		
				}
				else if(old_bool[chosen_but]==FALSE&&new_bool==TRUE)
				{
					g_mos[chosen_but]=TRUE;
				}
				else
				{
					g_mos[chosen_but]=FALSE;
				}
				old_bool[chosen_but]=new_bool;
			}
	}

}

void test_all_but_mos(GAMEPAD_DEVICE GAMEPAD_0)// little test programm
{
	char ch;
	ch='0';
	puts("TEST all button update. Press BACK to continue test");
	while(ch!='1'){
		update_all_but(GAMEPAD_0);
		if(g_mos[BUTTON_X_define_mos]==TRUE)
			puts("X-lore");
		if(g_mos[BUTTON_Y_define_mos]==TRUE)
			puts("Y-lore");
		if(g_mos[BUTTON_A_define_mos]==TRUE)
			puts("A-lore");
		if(g_mos[BUTTON_B_define_mos]==TRUE)
			puts("B-lore");
		if(g_mos[BUTTON_LEFT_STICK_define_mos]==TRUE)
			puts("light-stick-lore");
		if(g_mos[BUTTON_RIGHT_STICK_define_mos]==TRUE)
			puts("right-stick-lore");
		if(g_mos[BUTTON_START_define_mos]==TRUE)
			puts("START-lore");
		if(g_mos[KREUZ_UP_define_mos]==TRUE)
			puts("KREUZ-UP-lore");
		if(g_mos[KREUZ_DOWN_define_mos]==TRUE)
			puts("KREUZ-DOWN-lore");
		if(g_mos[KREUZ_LEFT_define_mos]==TRUE)
			puts("KREUZ-LEFT-lore");
		if(g_mos[KREUZ_RIGHT_define_mos]==TRUE)
			puts("KREUZ-RIGHT-lore");
		if(g_mos[BUTTON_BACK_define_mos]==TRUE){
			puts("BACK-lore");
			ch='1';
		}
		if(g_mos[BUTTON_LB_define_mos]==TRUE)
			puts("LB-lore");
		if(g_mos[BUTTON_RB_define_mos]==TRUE)
			puts("RB-lore");
		
		//printf("LENGTH: %f -- ANGEL: %f \n",g_trigger_mos[STICK_LEFT_STICK_define_mos],g_trigger_mos[STICK_LEFT_ANGEL_define_mos]);
	
		
		g_mos_reset();
	}
	ch='0';
	puts("TEST all single buttons update. Press START to exit test");
	while(ch!='1'){
		get_button_mos(GAMEPAD_0,"x");
		get_button_mos(GAMEPAD_0,"y");
		get_button_mos(GAMEPAD_0,"a");
		get_button_mos(GAMEPAD_0,"b");
		get_button_mos(GAMEPAD_0,"left_stick");
		get_button_mos(GAMEPAD_0,"right_stick");
		get_button_mos(GAMEPAD_0,"start");
		get_button_mos(GAMEPAD_0,"back");
		get_button_mos(GAMEPAD_0,"rb");
		get_button_mos(GAMEPAD_0,"lb");
		get_button_mos(GAMEPAD_0,"lt");
		//printf("%f\n",g_trigger_mos[BUTTON_LEFT_TRIGGER_define_mos]);
		//printf("LENGTH: %f -- ANGEL: %f \n",g_trigger_mos[STICK_LEFT_STICK_define_mos],g_trigger_mos[STICK_LEFT_ANGEL_define_mos]);
		if(g_trigger_mos[BUTTON_LEFT_TRIGGER_define_mos]==1)
			puts("LT-lore");
		if(g_trigger_mos[STICK_LEFT_ANGEL_define_mos]>1)
			puts("STICK-LEFT-lore");
		if(g_mos[BUTTON_X_define_mos]==TRUE)
			puts("X-lore");
		if(g_mos[BUTTON_Y_define_mos]==TRUE)
			puts("Y-lore");
		if(g_mos[BUTTON_A_define_mos]==TRUE)
			puts("A-lore");
		if(g_mos[BUTTON_B_define_mos]==TRUE)
			puts("B-lore");
		if(g_mos[BUTTON_LEFT_STICK_define_mos]==TRUE)
			puts("light-stick-lore");
		if(g_mos[BUTTON_RIGHT_STICK_define_mos]==TRUE)
			puts("right-stick-lore");
		if(g_mos[BUTTON_START_define_mos]==TRUE){
			puts("START-lore");
			ch='1';
		}
		if(g_mos[BUTTON_BACK_define_mos]==TRUE)
			puts("BACK-lore");
		if(g_mos[BUTTON_LB_define_mos]==TRUE)
			puts("LB-lore");
		if(g_mos[BUTTON_RB_define_mos]==TRUE)
			puts("RB-lore");
		g_mos_reset();
	}

}

void g_mos_reset(){	//resets all global Booleans, Doubles
	int i;
	for(i=0;i<sizeof gamepad_but_list/sizeof (struct gamepad_items);i++)
		g_mos[i]=FALSE;
}

void init_gamepad_rogmos()	//configures the gamepad
{
	cbreak();
    timeout(1);
    GamepadInit();

}

int main(){

	init_gamepad_rogmos();	//init Gamepad
	test_all_but_mos(GAMEPAD_0);	//run test
/*
	while((ch==getch())!='q'){
		get_button_mos(GAMEPAD_0,"left_stick");
		get_button_mos(GAMEPAD_0,"y");
		get_button_mos(GAMEPAD_0,"x");
		update_all_but(GAMEPAD_0);
		if(g_mos[BUTTON_X_define_mos]==TRUE)
			puts("X-lore");
		if(g_mos[BUTTON_Y_define_mos]==TRUE)
			puts("Y-lore");
		if(g_mos[BUTTON_LEFT_STICK_define_mos]==TRUE)
			puts("light stick lore");
		g_mos_reset();
	}*/

	return 0;
}