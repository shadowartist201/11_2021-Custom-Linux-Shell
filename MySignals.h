#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

void signal_handler(){
    puts("");
}



void ignore_signals_in_parent()
{

    signal(SIGHUP, SIG_IGN);		//disconect from terminal
    signal(SIGINT, SIG_IGN);		//terminal interupt
    signal(SIGQUIT, SIG_IGN);		//quits upon specified fd
    signal(SIGILL, SIG_IGN);		//indication of corrupted exe
    signal(SIGTRAP, SIG_IGN);		//user breakpoint
    signal(SIGABRT, SIG_IGN);		//deliberate crash
    signal(SIGBUS, SIG_IGN);		//bus error
    signal(SIGFPE, SIG_IGN);		//arithmetic error
    signal(SIGKILL, SIG_IGN);		//ime termination
    signal(SIGSEGV, SIG_IGN);		//invalid mem reference
    signal(SIGPIPE, SIG_IGN);		//pipe error
    signal(SIGALRM, SIG_IGN);		//asynch time experiation
    signal(SIGTERM, SIG_IGN);		//generic termination
    signal(SIGSTKFLT, SIG_IGN);		//stack fault
    signal(SIGCHLD, SIG_IGN);		//catching child process, parent forwarding
    signal(SIGCONT, SIG_IGN);		//continue call
    signal(SIGSTOP, SIG_IGN);		//complete halt of process	
    signal(SIGTSTP, SIG_IGN);		//interactive halt of process
    signal(SIGTTIN, SIG_IGN);		//halts compiler input due to location within foreground
    signal(SIGTTOU, SIG_IGN);		//background process read
    signal(SIGURG, SIG_IGN);		//out of band data
    signal(SIGXCPU, SIG_IGN);		//limit consumed on processor time
    signal(SIGXFSZ, SIG_IGN);		//specified output is larger than alloted 
    signal(SIGVTALRM, SIG_IGN);		//time experation under processor
    signal(SIGPROF, SIG_IGN);		//signifies experation of cpu time alloted
    signal(SIGWINCH, SIG_IGN);		//resizing of window
    signal(SIGIO, SIG_IGN);			//utilized when fd is performing i/o
    signal(SIGPWR, SIG_IGN);		//pwr has been switched
    signal(SIGSYS, SIG_IGN);		//bad system call
    signal(SIGRTMIN, SIG_IGN);		//initialize static data 
	
}



void set_signals_to_default_in_child()
{
    setpgid(0, getpid());
    signal(SIGHUP, SIG_DFL);		//disconect from terminal
    signal(SIGINT, SIG_DFL);		//terminal interupt
    signal(SIGQUIT, SIG_DFL);		//quits upon specified fd
    signal(SIGILL, SIG_DFL);		//indication of corrupted exe
    signal(SIGTRAP, SIG_DFL);		//user breakpoint
    signal(SIGABRT, SIG_DFL);		//deliberate crash
    signal(SIGBUS, SIG_DFL);		//bus error
    signal(SIGFPE, SIG_DFL);		//arithmetic error
    signal(SIGKILL, SIG_DFL);		//ime termination
    signal(SIGSEGV, SIG_DFL);		//invalid mem reference
    signal(SIGPIPE, SIG_DFL);		//pipe error
    signal(SIGALRM, SIG_DFL);		//asynch time experiation
    signal(SIGTERM, SIG_DFL);		//generic termination
    signal(SIGSTKFLT, SIG_DFL);		//stack fault
    signal(SIGCHLD, SIG_DFL);		//catching child process, parent forwarding
    signal(SIGCONT, SIG_DFL);		//continue call
    signal(SIGSTOP, SIG_DFL);		//complete halt of process
    signal(SIGTSTP, SIG_DFL);		//interactive halt of process
    signal(SIGTTIN, SIG_DFL);		//halts compiler input due to location within foreground
    signal(SIGTTOU, SIG_DFL);		//background process read
    signal(SIGURG, SIG_DFL);		//out of band data
    signal(SIGXCPU, SIG_DFL);		//limit consumed on processor time
    signal(SIGXFSZ, SIG_DFL);		//specified output is larger than alloted
    signal(SIGVTALRM, SIG_DFL);		//time experation under processor
    signal(SIGPROF, SIG_DFL);		//signifies experation of cpu time alloted
    signal(SIGWINCH, SIG_DFL);		//resizing of window
    signal(SIGIO, SIG_DFL);			//utilized when fd is performing i/o
    signal(SIGPWR, SIG_DFL);		//pwr has been switched
    signal(SIGSYS, SIG_DFL);		//bad system call
    signal(SIGRTMIN, SIG_DFL);		//initialize static data
}
