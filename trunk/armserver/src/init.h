void check_user_permission(void);
void armserver_init(void);
int wait_user_connect(void);
void armserver_siganl_handle(int sig);
void create_session(int user);
extern char *login_user;
