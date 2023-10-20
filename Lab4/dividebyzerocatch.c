

#include <signal.h>
void CATCH( );
int main() {

    
    int a = 4;
    a = a/0;
    signal(SIGFPE, CATCH);
    return 0;
}

void CATCH() {
    printf("Caught a SIGFPE");
}
