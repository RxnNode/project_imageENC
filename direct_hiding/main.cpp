#include <stdio.h>
#include <stdlib.h>
#include "img_convert.h"
#include "math_rela.h"
#include "enc_file.h"
#include "enc_rand.h"






int main() {

    //convertoBMP3C("../images/lena.png","../images/lenaRGB.bmp");
    //convertoBMP4C("../images/lena.png","../images/lenaRGBA.bmp");

    //enc_file("../images/lenaRGB.bmp","../text_source/secret");

    //enc_rand("../images/lenaRGB.bmp","../text_source/secret");

    for (int i = 0; i < 8; ++i) {
        char filename[32];
        long double p = 0.0;
        sprintf(filename,"../RandOut/RandOut_%d.bmp",i);
        p = psnr(filename, "../images/lenaRGB.bmp");
        printf("%d bits hiding psnr : %Lf\n",i+1,p);
    }

    //double p = 0.0;
    /*p = psnr("../out/out_0.bmp", "../images/imgb.bmp");
    printf("1 bits hiding psnr : %8f\n",p);
    p = psnr("../out/out_4.bmp", "../images/imgb.bmp");
    printf("5 bits hiding psnr : %8f\n",p);*/
    //makemultpic(20);


    /*printf("%d ", 0b00000011<<0);
    printf("%d ", 0b00000011<<6);
    printf("%d ", 0b00000011<<4);
    printf("%d ", 0b00000011<<2);*/



    return 0;

}
