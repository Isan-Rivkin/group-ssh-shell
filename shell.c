#include <stdlib.h>
#include <memory.h>

char* build_ssh_command(char * terminal_cmd, char * user_name, char * ip){
    char * ssh_cmd = (char *) malloc(4 + strlen(user_name)+ strlen(ip));
    if(ssh_cmd == NULL){
        return NULL;
    }
    #ifdef __APPLE__ || __MACH__
    strcpy(ssh_cmd,"\"");
    #elif
    strcpy(ssh_cmd,"");
    #endif
    strcat(ssh_cmd,"ssh ");
    strcat(ssh_cmd,user_name);
    strcat(ssh_cmd,"@");
    strcat(ssh_cmd,ip);
    #ifdef __APPLE__ || __MACH__
    strcat(ssh_cmd,"\"");
    #endif
    // append the ssh command with terminal openning 
    char * ssh_with_terminal_cmd = (char *) malloc(2 + strlen(terminal_cmd)+ strlen(ssh_cmd));
    if(ssh_with_terminal_cmd == NULL){
        return NULL;
    }
    strcpy(ssh_with_terminal_cmd,terminal_cmd);
    strcat(ssh_with_terminal_cmd,ssh_cmd);
    strcat(ssh_with_terminal_cmd,"'");
    return ssh_with_terminal_cmd;
}
char * build_terminal_launch_cmd(){
    #ifdef _WIN32
    return "win32";
    #elif _WIN64
    return "win64";
    #elif __unix || __unix__
    return "gnome-terminal -x sh -c '";
    #elif __APPLE__ || __MACH__
    return "osascript -e 'tell application \"Terminal.app\" to do script ";
    #elif __linux__
    return "gnome-terminal -x sh -c '"
    #elif __FreeBSD__
    return "freeBSD";
    #else
    return "other";
    #endif
}
int main(int argc, char *argv[]) {
    // input must have a user name param and atleast 1 ip address 
    if(argc < 3){
        return -1; 
    }
    char* user_name = argv[1]; 
    char * terminal_cmd = build_terminal_launch_cmd();
    for(int i=2;i<argc;i++){
        // build the ssh command 
        char * ip = argv[i];
        //gnome-terminal -x sh -c 'ssh user_name@ip'
        //osascript -e 'tell application Terminal.app to do script "ssh yoav.yanilov@10.10.16.100"'
        char * ssh_with_terminal_cmd = build_ssh_command(terminal_cmd,user_name,ip);
        if(ssh_with_terminal_cmd == NULL){
            return -2; 
        }
        printf("executing =|%s|",ssh_with_terminal_cmd);
        // execute the command 
        int exit_status = system(ssh_with_terminal_cmd);
        if(exit_status != 0){
            printf("\n ERROR %d executing command %s",exit_status,ssh_with_terminal_cmd);
        }else{
            printf("Ok! %s", ip);
        }
    }
}
