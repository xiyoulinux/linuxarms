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
	SSENDALL, /* 发送完毕 */
	SRECVALL, /* 接收完毕 */
	SRECVSUC, /* 成功接收 */
	SRECVERR，/* 接收错误 */ 
	SMAX,     /* 无效命令 */
}protocol_sthread;
/*
 * 文件浏览和文件传输交互协议
 */
typedef enum _Fthread {
	FUP,     /* 上传文件 */
	FDOWN,   /* 下载文件 */
	CSERSUC, /* 创建文件传输服务器成功 */
	FTRANSSUC, /* 文件传输成功*/
	FVIEW,   /* 文件浏览 */
	FRENAME, /* 重命名文件 */
	FDELETE, /* 删除文件 */
	FVIEWSUC,/* 成功接收信息 */
	FVIEWERR,/* 接收信息失败 */
	FRECVALL,/* 接收完毕 */
	FSENDALL,/* 发送完毕 */
	FMAX     /* 无效命令 */
}protocol_fthread;
/*
 * 实时控制交互协议
 */
typedef enum _Cthread {
	CSEND, /* 发送数据 */
	CRECV, /* 接收数据 */
	CSENDALL, /* 发送完毕 */
	CRECVALL /*  接收完毕 */
}protocol_cthread;
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
