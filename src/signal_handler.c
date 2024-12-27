//B221210080 ÇAĞRI TÜKENMEZ
//B221210038 DENİZ UMUR
//B231210350 MELİH DEDEOĞLU
//B221210071 YAVUZ EMRE GÜNDÜZ
//B211210015 BUĞRA BASTABAN
#include "signal_handler.h"
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <prompt.h>
#include <sys/wait.h>

extern int background;
void handle_background_process(int sig) {
    int status;
    pid_t pid;
    // Tüm tamamlanan çocuk süreçleri işleme
    while ((pid = waitpid(-1, &status, WNOHANG)) > 0) {
        // Arka plandaki işlemin yalnızca gerçekten bittiğini kontrol edin
            if (WIFEXITED(status)) {
                
                printf("\n[%d] retval: <%d>\n", pid, WEXITSTATUS(status));
                print_prompt();
                if(background > 0)background--; // Arka plan sayaç değerini azalt
            }//[pid] retval: <exitcode>
        }
    
}

void setup_signal_handler() {
    struct sigaction sa;
    sa.sa_handler = handle_background_process;
    sa.sa_flags = SA_RESTART | SA_NOCLDSTOP;
    sigaction(SIGCHLD, &sa, NULL);
}