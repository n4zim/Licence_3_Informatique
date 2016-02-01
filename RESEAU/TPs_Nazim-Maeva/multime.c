#include "bor-timer.h"
#include "bor-util.h"

int main() {
    int t1 = bor_timer_add(2000, NULL);
    int t2 = bor_timer_add(4000, NULL);
    int t3 = bor_timer_add(8000, NULL);
    int t4 = bor_timer_add(10000, NULL);

    int loop = 1;
    while(loop) {
        int selec = select(0, NULL, NULL, NULL, bor_timer_delay());
        if(selec < 0) break;

        int current = bor_timer_handle();
        if(current == t1) {
            printf("Timer toutes les 2 secondes\n");
            t1 = bor_timer_add(2000, NULL);
        
        } else if(current == t2) {
            printf("Timer à partir de 4 secondes\n");
        }
        else if(current == t3) {
            printf("Timer à partir de 8 secondes\n");
        }
        else {
            printf("Dernier timer : %d\n", t4);
            loop = 0;
        }
        bor_timer_remove(current);
    }

}