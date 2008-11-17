#ifndef _PROTOCOL_H
#define _PROTOCOL_H
/*
 * main thread action.it means what will arm system do
 * when host send a message to arm system.
 */
#define ACTION_NUM 8
typedef enum _Action {
        LOGIN,LOGOUT, /* user longin or logout */
        RESTART,SHUTDOWN, /* restart or shutdown arm system */
        CSTHREAD, /* control show arm system information/process thread */
        CFTHREAD, /* control view arm system file thread */
        CCTHREAD,  /* control control arm system thread */
        NONE /* default action, do nothing */
}maction;

/* return state of execute */
#define EXEC_STATE_NUM 3
typedef enum _Exec_state {
	SUCESS,
	NOUSER,
	HASUSER,
	NOCOMPETENCE,
}exec_state;
#endif
