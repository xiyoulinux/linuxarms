#ifndef _PROTOCOL_H
#define _PROTOCOL_H
/*
 * 主线程交互协议
 */
typedef enum _Mthread {
	LOGIN,       /* 用户登录                       */
	HAVEUSER,    /* 已经有用户登录                 */
	CHECKERR,    /* 验证用户信息错误               */
	CHECKMULT,   /* 尝试登录次数过多               */
	LOGERR,      /* 登录失败                       */
        RESTART,     /* 重启arm系统                    */
	NOPERMITION, /* 没有权限执行                   */
	LOGOUT,      /* 用户注销                       */
	SHUTDOWN,    /* 关闭arm系统                    */
	CLOSECLIENT, /* 关闭客户端                     */
        CSTHREAD,    /* 控制系统信息显示和实时监视线程 */
        CFTHREAD,    /* 控制文件浏览线程               */
        CCTHREAD,    /* 控制实时控制线程               */
	MSUCCESS,    /* 执行成功                       */
	MERROR,      /* 执行错误                       */
        MMAX         /* 无效命令                       */
}protocol_mthread;
#define  PROTOCOL_IS_MTHREAD(protocol) ((protocol) >= LOGIN && (protocol) < MMAX)
/*
 * 系统信息显示和实时监视线程交互协议
 */
typedef enum _Sthread {
	SSYSINFO = MMAX + 1, /* 系统信息显示 */
	SPROCESS,  /* 实时监视(进程信息显示) */
	SKILL,     /* 杀死进程               */
	SKILLSUC,  /* 杀死进程成功           */
	SKILLERR,  /* 杀死进程失败           */
	SSENDALL,  /* 发送完毕               */
	SRECVALL,  /* 接收完毕               */
	SRECVSUC,  /* 成功接收               */
	SRECVERR,  /* 接收错误               */
	SSUCCESS,  /* 执行成功               */
	SERROR,    /* 执行错误               */
	SMAX       /* 无效命令               */
}protocol_sthread;
#define  PROTOCOL_IS_STHREAD(protocol) ((protocol) >= SSYSINFO && (protocol) < SMAX)
/*
 * 文件浏览和文件传输交互协议
 */
typedef enum _Fthread {
	FUP = SMAX + 1, /* 上传文件           */
	FDOWN,      /* 下载文件               */
	CSERSUC,    /* 创建文件传输服务器成功 */
	FTRANSSUC,  /* 文件传输成功           */
	FVIEW,      /* 文件浏览               */
	FRENAME,    /* 重命名文件             */
	FRENAMESUC, /* 重命名文件成功         */
	FRENAMEERR, /* 重命名文件失败         */
	FDELETE,    /* 删除文件               */
	FDELETESUC, /* 删除文件成功           */
	FDELETEERR, /* 删除文件失败           */
	FSUCCESS,   /* 成功接收信息           */
	FERROR,     /* 接收信息失败           */
	FSEND,      /* 发送信息               */
	FRECV,      /* 接收信息               */
	FRECVALL,   /* 接收完毕               */
	FSENDALL,   /* 发送完毕               */
	FNOEXIST,   /* 文件不存在             */
	FEXIST,     /* 文件已经存在           */
	FMAX        /* 无效命令               */
}protocol_fthread;
#define  PROTOCOL_IS_FTHREAD(protocol) ((protocol) >= FUP && (protocol) < FMAX)
/*
 * 实时控制交互协议
 */
typedef enum _Cthread {
	CSEND = FMAX + 1, /* 发送数据 */
	CRECV,    /* 接收数据         */
	CSENDALL, /* 发送完毕         */
	CRECVALL, /* 接收完毕         */
	CSENDCD,  /* 发送cd命令结果   */
	CMAX      /* 无效命令         */
}protocol_cthread;
#define  PROTOCOL_IS_CTHREAD(protocol) ((protocol) >= CSEND && (protocol) < CMAX)

#endif
