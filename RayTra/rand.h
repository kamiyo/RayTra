#ifndef RAND_H_
#define RAND_H_

/* initializes mt[N] with a seed */
void init_genrand(unsigned long s);

void init_by_array(unsigned long init_key[], int key_length);

/* generates a random number on [0,0xffffffff]-interval */
unsigned long genrand_int32(void);

/* generates a random number on [0,0x7fffffff]-interval */
long genrand_int31(void);

/* generates a random number on [0,1]-real-interval */
double genrand_real1(void);

/* generates a random number on [0,1)-real-interval */
double genrand_real2(void);

/* generates a random number on (0,1)-real-interval */
double genrand_real3(void);
/* generates a random number on [0,1) with 53-bit resolution*/
double genrand_res53(void);
/* These real versions are due to Isaku Wada, 2002/01/09 added */

//int main(void)
//{
//    int i;
//    unsigned long init[4]={0x123, 0x234, 0x345, 0x456}, length=4;
//    init_by_array(init, length);
//    printf("1000 outputs of genrand_int32()\n");
//    for (i=0; i<1000; i++) {
//      printf("%10lu ", genrand_int32());
//      if (i%5==4) printf("\n");
//    }
//    printf("\n1000 outputs of genrand_real2()\n");
//    for (i=0; i<1000; i++) {
//      printf("%10.8f ", genrand_real2());
//      if (i%5==4) printf("\n");
//    }
//    return 0;
//}

#endif