#include <iostream>
#include <csignal>
#include <unistd.h>
#include <string.h>
#include <ucontext.h>
#include <sys/types.h>
#include <pwd.h>


// getting user name by uid
std::string get_username(uid_t uid) {
    struct passwd *pws;
    pws = getpwuid(uid);
    return pws->pw_name;
}

void signal_handler(int sig_num, siginfo_t *info, void *context) {
    // error handling
    if(info->si_errno){
        std::cout << "Error " << strerror(info->si_errno);
        exit(info->si_errno);
    }

    if(sig_num == SIGUSR1){
        std::cout << "The PID of the sender: " << info->si_pid << std::endl;
        std::cout << "The UID of the sender: " << info->si_uid << std::endl;
        std::cout << "The User Name of the sender: " << get_username(info->si_uid) << std::endl;
	
        ucontext_t *context = (ucontext_t *)context;
	if(context != NULL) {

        	std::cout << "EIP: " << context->uc_mcontext.gregs[REG_RIP] << std::endl;
        	std::cout << "EAX: " << context->uc_mcontext.gregs[REG_RAX] << std::endl;
        	std::cout << "EBX: " << context->uc_mcontext.gregs[REG_RBX] << std::endl;
        	exit(sig_num);
	}
    }
    return;
    
}

int main() {
    
    struct sigaction action;
    action.sa_sigaction = &signal_handler;
    action.sa_flags = SA_SIGINFO;
    int res  = sigaction(SIGUSR1, &action, NULL);
    // error handling
    if (res < 0) {
        std::cout << "Error: " << strerror(errno);
        exit(errno);
    }
    while(true) {
        sleep(10);
    }
    return 0;
}
