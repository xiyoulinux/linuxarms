#ifndef _PROTOCOL_H
#define _PROTOCOL_H
/*
 * 主线程交互协议
 */
typedef enum _Mthread {
        LOGIN,    /* 用户登录 */
	LOGOUT,   /* 用户注销 */
        RESTART,  /* 重启arm系统 */
	SHUTDOWN, /* 关闭arm系统 */
        CSTHREAD, /* 控制系统信息显示和实时监视线程 */
        CFTHREAD, /* 控制文件浏览线程 */
        CCTHREAD, /* 控制实时控制线程 */
        MMAX      /* 无效命令 */
}protocol_mthread;
/*
 * 系统信息显示和实时监视线程交互协议
 */
typedef enum _Sthread {
	SYSINFO,  /* 系统信息显示 */
	SPROCESS, /* 实时监视(进程信息显示) */
	KILL,     /* 杀死进程 */
	KILLSUC,  /* 杀死进程成功 */
	KILLERR,  /* 杀死进程失败*/
	SENDALL,  /* 发送完毕 */
	RECVALL,  /* 接收完毕 */
	RECVSUC,  /* 成功接收 */
	SMAX,     /* 无效命令 */
}protocol_sthread;
/*
 * 文件浏览和文件传输交互协议
 */
typedef enum _Fthread {
	UP,     /* 上传文件 */
	DOWN,   /* 下载文件 */
	VIEW,   /* 文件浏览 */
	RENAME, /* 重命名文件 */
	DELETE, /* 删除文件 */
	FMAX    /* 无效命令 */
}protocol_fthread;
/*
 * hostclient和armserver交互过程中请求的执行返回值
 */
typedef enum _Return {
	SUCCESS,     /* 执行成功 */
	NOUSER,      /* 没有用户信息 */
	HASUSER,     /* 已经有用户登录 */
	NOCOMPETENCE,/* 没有权限执行 */
	TRANSERR,    /* 文件传输错误 */
	TRANSSUCCESS,/* 文件传输成功 */
	RMAX         /* 无效返回值 */
}protocol_return;
#endif
